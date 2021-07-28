/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 *                Copyright (C) 2009 Sony Computer Entertainment Inc.
 *                                               All Rights Reserved.
 */


/*
 * 
 */

#include <stdio.h>
#include <PSGL/psgl.h>
#include <PSGL/private.h>
#include <PSGL/Debug.h>
#include <PSGL/Utils.h>
#include <JSGCM/JSGcmUtils.h>
#include <JSGCM/JSGcmDevice.h>
#include <JSGCM/JSGcmDriver.h>
#include <JSGCM/JSGcmFBOps.h>
#include <JSGCM/JSGcmSetRenderTarget.h>
#include "PlatformDeviceInit.h"
#include <string.h>
#include <sdk_version.h>
#include <cell/gcm.h>
#include <cell/resc.h>
#include <sysutil/sysutil_sysparam.h>
#include <sys/synchronization.h>

_JS_EXTERN_C GLboolean _psglDisableCompression;
GLboolean _psglDisableCompression = GL_FALSE;

#ifdef JS_RSXFIFODUMP
#include <PSGL/JSRSXFifoDump.h>
void _jsDecodeAndDumpFifo();
#endif

//#define _JSGCM_TILE_CHANGE_CHATTY
#ifdef _JSGCM_TILE_CHANGE_CHATTY
#define _JSGCM_TILE_CHANGE_REPORT(...) printf(__VA_ARGS__)
#else
#define _JSGCM_TILE_CHANGE_REPORT(...) do {} while(0)
#endif

using namespace cell::Gcm;


/////////////////////////////////////////////////////////////////////////////

// tiled memory manager
typedef struct
{
    int id;
    GLuint offset;
    GLuint size;        // 0 size indicates an unused tile
    GLuint pitch;       // in bytes
    GLenum compression;
    GLuint bank;
    GLuint memory;      // 0 for GPU, 1 for host
    GLuint ctags_base;
}
jsTiledRegion;

typedef struct
{
    jsTiledRegion region[_JSGCM_MAX_TILED_REGIONS];

    struct
    {
        void* bufferAddress;
        int zcullStart;
        int zWidth;
        int zHeight;
        unsigned char aaFormat;
    }
    zcull[_JSGCM_MAX_ZCULL_REGIONS];

    MemoryBlockManager_t zcullManager;
    MemoryBlockManager_t ctagsManager;
}
jsTiledMemoryManager;


// TODO: put in device state?
static jsTiledMemoryManager _jsGcmTiledMemoryManager;

static jsGcmResource _jsGcmResource;


void _jsGcmTiledMemoryInit( void )
{
    jsTiledMemoryManager* mm = &_jsGcmTiledMemoryManager;
    int32_t retVal;

    // create zcull memory manager
    _jsMemoryBlockManagerInit(
        &mm->zcullManager,
        ( void* )_JSGCM_ZCULL_BUFFER_ALIGNMENT, // don't return NULL address
        _JSGCM_ZCULL_BUFFER_SIZE-_JSGCM_ZCULL_BUFFER_ALIGNMENT, // subtract total size because we don't start at 0 in the parameter above
        _JSGCM_ZCULL_BUFFER_ALIGNMENT,
        _JS_ALLOC_BEST_FIT );

    // create ctags memory manager
    // See 9.4.3 section of RSX User's Manual 
    _jsMemoryBlockManagerInit(
        &mm->ctagsManager,
        ( void* )( unsigned long )_JSGCM_CTAG_CORRESPONDING_SIZE, // don't return NULL address        
        _JSGCM_CTAG_CORRESPONDING_TOTAL_SIZE-_JSGCM_CTAG_CORRESPONDING_SIZE, // subtract total size because we don't start at 0 in the parameter above
        _JSGCM_CTAG_CORRESPONDING_SIZE,
        _JS_ALLOC_BEST_FIT );

    memset( mm->region, 0, sizeof( mm->region ) );
    for ( int i = 0;i < _JSGCM_MAX_TILED_REGIONS;++i )
    {
        retVal = cellGcmUnbindTile( i );
		JS_ASSERT( retVal == CELL_OK ); 
    }
}

void _jsGcmTiledMemoryFinish( void )
{
    jsTiledMemoryManager* mm = &_jsGcmTiledMemoryManager;

    // clean up memory managers
    _jsMemoryBlockManagerDestroy( &mm->ctagsManager );
    _jsMemoryBlockManagerDestroy( &mm->zcullManager );
}


///////////////////////////////////////////////////////////////////////////

GLboolean _jsPlatformDeviceInit( PSGLinitOptions* options )
{
    GLuint fifoSize = _JSGCM_FIFO_SIZE_DEFAULT;
    GLuint hostSize = _JSGCM_HOST_SIZE_DEFAULT;

    if ( options != NULL )
    {
        if ( options->enable & PSGL_INIT_FIFO_SIZE )
            fifoSize = options->fifoSize;
        if ( options->enable & PSGL_INIT_HOST_MEMORY_SIZE )
            hostSize = options->hostMemorySize;
    }

    if ( !_jsGcmInitRM( &_jsGcmResource, hostSize, 0, fifoSize ) )
    {
        fprintf( stderr, "RM resource failed initialisation\n" );
        JS_ASSERT( 0 );
        return GL_FALSE;
    }

    return _jsGcmInit( options, &_jsGcmResource );
}


void _jsPlatformDeviceExit()
{
    _jsGcmDestroy();
    _jsGcmDestroyRM( &_jsGcmResource );
}


int _jsPlatformDeviceSize()
{
    return sizeof( jsGcmDevice );
}


void _jsPlatformMakeCurrent( void *dev )
{}


/////////////////////////////////////////////////////////////////////////////

#ifdef JS_DPM_V2
int _jsPlatformGetTiledRegionInfo( GLuint address, GLuint *pitch, GLenum *comp,
                                   int *zcullStart, int *zcullWidth, int *zcullHeight, unsigned char *zcullAAFormat )
{
    JS_ASSERT( pitch && comp && zcullStart && zcullWidth && zcullHeight && zcullAAFormat );
    *pitch = 0;
    *comp = CELL_GCM_COMPMODE_DISABLED;
    *zcullStart = NULL;
    *zcullWidth = *zcullHeight = 0;
    *zcullAAFormat = 0;
    int id = -1;
    int isTiled = 0;

    jsTiledMemoryManager* mm = &_jsGcmTiledMemoryManager;

    for ( GLuint i = 0; i < _JSGCM_MAX_TILED_REGIONS; ++i )
    {
        jsTiledRegion *region = &( mm->region[i] );
        if (( address >= region->offset ) && ( address < region->offset + region->size ) )
        {
            id     = region->id;
            *pitch = region->pitch;
            *comp  = region->compression;
            isTiled = 1;
            break;
        }
    }

    if ( id >= 0 )
    { // Look for a zcull region
        for ( GLuint j = 0; j < _JSGCM_MAX_ZCULL_REGIONS; ++j )
        {
            if ( mm->zcull[j].bufferAddress == JSGCM_OFFSET_TO_PTR( address ) )
            {
                *zcullStart    = mm->zcull[j].zcullStart;
                *zcullWidth    = mm->zcull[j].zWidth;
                *zcullHeight   = mm->zcull[j].zHeight;
                *zcullAAFormat = mm->zcull[j].aaFormat;
            }
        }
    }
    return isTiled;
}
#endif

/////////////////////////////////////////////////////////////////////////////

static unsigned int validPitch[] =
    {
        0x0200,
        0x0300,
        0x0400,
        0x0500,
        0x0600,
        0x0700,
        0x0800,
        0x0A00,
        0x0C00,
        0x0D00,
        0x0E00,
        0x1000,
        0x1400,
        0x1800,
        0x1A00,
        0x1C00,
        0x2000,
        0x2800,
        0x3000,
        0x3400,
        0x3800,
        0x4000,
        0x5000,
        0x6000,
        0x6800,
        0x7000,
        0x8000,
        0xA000,
        0xC000,
        0xD000,
        0xE000,
        0x10000,
    };
static const unsigned int validPitchCount = sizeof( validPitch ) / sizeof( validPitch[0] );

static unsigned int findValidPitch( unsigned int pitch )
{
    if ( pitch <= validPitch[0] ) return validPitch[0];
    else
    {
        // dummy linear search
        for ( GLuint i = 0;i < validPitchCount - 1;++i )
        {
            if (( pitch > validPitch[i] ) && ( pitch <= validPitch[i+1] ) ) return validPitch[i+1];
        }
        return validPitch[validPitchCount-1];
    }
}

static GLboolean _jsDuringDestroyDevice = GL_FALSE;

// region update callback
//  This callback is passed to _jsGcmAllocCreateRegion to notify when the
//  region is resized or deleted.
GLboolean _jsGcmTryResizeTileRegion( GLuint address, GLuint size, void* data )
{
    jsTiledMemoryManager* mm = &_jsGcmTiledMemoryManager;
    jsTiledRegion* region = ( jsTiledRegion* )data;
	int32_t retVal = 0;

    // delete always succeeds
    if ( size == 0 )
    {
        _JSGCM_TILE_CHANGE_REPORT( "un-tile region %d at address 0x%08x size=%d\n", region->id, region->offset, region->size );
        if ( region->ctags_base )
        {
            _JSGCM_TILE_CHANGE_REPORT( "free ctags at 0x%08x", region->ctags_base );
            _jsMemoryBlockManagerFree( &mm->ctagsManager, ( void * )( unsigned long )region->ctags_base );
        }
        region->offset = 0;
        region->size = 0;
        region->pitch = 0;
        region->compression = CELL_GCM_COMPMODE_DISABLED;
        region->memory = 0;
        region->ctags_base = 0;
		
        if ( ! _jsDuringDestroyDevice ) 
		{
		    // must wait until RSX is completely idle before calling cellGcmUnbindTile 
		    _jsGcmUtilWaitForIdle(); 

			retVal = cellGcmUnbindTile( region->id );
			JS_ASSERT( retVal == CELL_OK ); 
            _jsGcmFifoFinish( &_jsGcmState.fifo );
		}
        return GL_TRUE;
    }
    _JSGCM_TILE_CHANGE_REPORT( "try re-tile region %d from address 0x%08x size=%d to address 0x%08x size=%d\n", region->id, region->offset, region->size, address, size );
    if ( region->compression != CELL_GCM_COMPMODE_DISABLED )
    {
        // compression can only be turned on for local memory
        JS_ASSERT( region->memory == CELL_GCM_LOCATION_LOCAL );

        // big limits on compression.
        // we have to allocate ctag bits, but we can't read them nor clear them
        // and they need to be kept in sync for existing data
        //
        // so
        //
        // if we already have data in the region the only thing we're allowed to do
        // is to grow or shrink the existing ctag bits allocation (assuming this
        // function is only called to grow the region over unset data).
        if ( region->size > 0 )
        {
            if ( region->ctags_base )
            {
                if ( region->offset == address )
                {
                    _JSGCM_TILE_CHANGE_REPORT( "try reallocating tag bits at 0x%08x\n", region->ctags_base );
                    // if we're changing the size of an existing region, and have tag bits
                    // we can try to grow or shrink the tag bits (shrink should always succeed)
                    // if we had tag bits, and we're using the same address we can try to grow them
                    GLboolean result = _jsMemoryBlockManagerTryRealloc( &mm->ctagsManager, ( void * )( unsigned long )region->ctags_base, size );
                    if ( !result )
                    {
                        _JSGCM_TILE_CHANGE_REPORT( "failed reallocating tag bits at 0x%08x\n", region->ctags_base );
                        return GL_FALSE;
                    }
                }
                else
                {
                    _JSGCM_TILE_CHANGE_REPORT( "cannot change address because of tag bits\n" );
                    // else, we can't just take arbitrary tag bits, or forget the old ones, or whatever,
                    // that would corrupt the existing data
                    return GL_FALSE;
                }
            }
            // if we didn't have bits, we can continue without compression
            else _JSGCM_TILE_CHANGE_REPORT( "compression but no tag bits\n" );
        }
        else
        {
            // we used not to have data. we simply try to allocate ctag bits
            //
            // if we run out of tag bits, it's still a success (we can keep tiling)
            JS_ASSERT( region->ctags_base == 0 );
            region->ctags_base = ( GLuint )( unsigned long )_jsMemoryBlockManagerAlloc( &mm->ctagsManager, size );
            _JSGCM_TILE_CHANGE_REPORT( "allocated tag bits at 0x%08x\n", region->ctags_base );
        }
    }
    region->offset = address;
    region->size = size;

    _JSGCM_TILE_CHANGE_REPORT( "re-tile success\n" );

	// must wait until RSX is completely idle before calling cellGcmSetTileInfo 
	_jsGcmUtilWaitForIdle(); 

	retVal = cellGcmSetTileInfo(
		region->id,
        region->memory,
        region->offset,
        region->size,
        region->pitch,
        region->ctags_base ? region->compression : CELL_GCM_COMPMODE_DISABLED, // if no tag bits, disable compression
        region->ctags_base / _JSGCM_CTAG_CORRESPONDING_SIZE,
        region->bank );
	JS_ASSERT( retVal == CELL_OK ); 

	retVal = cellGcmBindTile( region->id ); 
	JS_ASSERT( retVal == CELL_OK ); 

    _jsGcmFifoFinish( &_jsGcmState.fifo );
    return GL_TRUE;
}

void _jsGcmGetTileRegionInfo( void* data, GLuint *address, GLuint *size )
{
    jsTiledRegion* region = ( jsTiledRegion* )data;

    *address = region->offset;
    *size = region->size;
}

#define _JSGCM_TILED_BUFFER_ALIGNMENT 0x10000 // 64KB
#define _JSGCM_TILED_BUFFER_HEIGHT_ALIGNMENT 64

GLuint _jsGcmAllocCreateRegion(
    uint8_t memoryLocation,
    GLboolean isZBuffer,
    GLuint size,
    GLint tag,
    void* data )
{
    uint32_t id = gmmAlloc((CellGcmContextData*)&_jsGcmState.fifo,
                           memoryLocation,
                           1,
                           size);

    if ( id != GMM_ERROR )
    {
        if ( _jsGcmTryResizeTileRegion( (GLuint)gmmIdToOffset(id), gmmGetBlockSize(id), data ) )
        {
            gmmSetTileAttrib( id, tag, data );
        }
        else
        {
            gmmFree( id );
            id = GMM_ERROR;
        }
    }

    return id; 
}

/////////////////////////////////////////////////////////////////////////////
// tiled surface allocation

static void _jsGcmAllocateTiledSurface(
    jsTiledMemoryManager* mm,
    GLboolean isLocalMemory,
    GLboolean isZBuffer,
    GLuint width,
    GLuint height,
    GLuint bitsPerPixel,
    GLuint antiAliasing,
    GLenum compression,
    GLuint* id,
    GLuint* pitchAllocated,
    GLuint* bytesAllocated )
{
    // XXX no compression on A01 silicon
    // also disabled it if not on local memory
    if ( _psglDisableCompression || !isLocalMemory )
    {
        compression = CELL_GCM_COMPMODE_DISABLED;
    }

    // adjust for antialiasing
    switch ( antiAliasing )
    {
        case 1:
            break;
        case 2:
            width *= 2;
            break;
        case 3:
        case 4:
            width *= 2;
            height *= 2;
            break;
        default:
            JS_ASSERT( 0 );
    }

    // determine pitch (in bytes)
    const unsigned int pitch = width * bitsPerPixel / 8;
    const unsigned int tiledPitch = findValidPitch( pitch );
    if ( tiledPitch < pitch )
        *pitchAllocated = _jsPad( pitch, tiledPitch );
    else
        *pitchAllocated = tiledPitch;

    // fix alignment
    //  Render targets must be aligned to 8*pitch from the start of their
    //  region.  In addition, tiled regions must be aligned to 65536.  In
    //  order to keep both requirements satisfied as the region is extended
    //  or shrunken, the allocation size is padded to the smallest common
    //  multiple.
    //
    //  This can result in a fairly large percentage of wasted memory for
    //  certain dimension combinations, but this is simple and may conserve
    //  tiled region usage over some alternatives.
    GLuint padSize = _JSGCM_TILED_BUFFER_ALIGNMENT; // 64KB
    while (( padSize % ( tiledPitch*8 ) ) != 0 )
        padSize += _JSGCM_TILED_BUFFER_ALIGNMENT;

    // determine allocation size
    height = _jsPad( height, _JSGCM_TILED_BUFFER_HEIGHT_ALIGNMENT );
    *bytesAllocated = _jsPad(( *pitchAllocated ) * height, padSize );

    // attempt to extend an existing region
    //  The region tag is a hash of the pitch, compression, and isZBuffer.
    const GLuint tag = *pitchAllocated | compression | ( isZBuffer ? 0x80000000 : 0x0 );
   
    *id = gmmAllocExtendedTileBlock(isLocalMemory ? CELL_GCM_LOCATION_LOCAL : CELL_GCM_LOCATION_MAIN,
                                    *bytesAllocated,
                                    tag);

    if ( *id == GMM_ERROR )
    {
        // find an unused region
        for ( int i = 0; i < _JSGCM_MAX_TILED_REGIONS; ++i )
        {
            if ( mm->region[i].size == 0 )
            {
                // assign a region
                //  Address and size will be set in the callback.
                mm->region[i].id = i;
                mm->region[i].pitch = *pitchAllocated;
                mm->region[i].compression = compression;
                mm->region[i].bank = isZBuffer ? 0x3 : 0x0; // XXX experiment
                mm->region[i].memory = isLocalMemory ? CELL_GCM_LOCATION_LOCAL : CELL_GCM_LOCATION_MAIN;

                // allocate space for our region
                *id = _jsGcmAllocCreateRegion(
                            mm->region[i].memory,
                            isZBuffer,
                            *bytesAllocated,
                            tag,
                            &mm->region[i] );

                break;
            }
        } // loop to find an unused region
    }

    // if we don't have a valid id, give up
    if ( *id == GMM_ERROR )
    {
        *bytesAllocated = 0;
        *pitchAllocated = 0;
    }
    else
    {
        _JS_REPORT_EXTRA( PSGL_REPORT_GPU_MEMORY_ALLOC, "Allocating GPU memory (tiled): %d bytes allocated at id 0x%08x", *bytesAllocated, *id );
    }
}

/////////////////////////////////////////////////////////////////////////////
// color surface allocation

GLboolean _jsGcmAllocateColorSurface(
    GLboolean isLocalMemory,
    GLuint width,
    GLuint height,
    GLuint bitsPerPixel,
    GLuint scanoutSupported,
    GLuint antiAliasing,
    GLuint *id,
    GLuint *pitchAllocated,
    GLuint *bytesAllocated )
{
    jsTiledMemoryManager* mm = &_jsGcmTiledMemoryManager;

    // compression type depends on antialiasing
    GLenum compression = CELL_GCM_COMPMODE_DISABLED;
    switch ( antiAliasing )
    {
        case 1:
			break; 
        case 2:
            if ( bitsPerPixel == 4*8 )
                compression = CELL_GCM_COMPMODE_C32_2X1;
            break;
        case 3:
        case 4:
            if ( bitsPerPixel == 4*8 )
                compression = CELL_GCM_COMPMODE_C32_2X2;
            break;
        default:
            JS_ASSERT( false );
            break;
    }

    _jsGcmAllocateTiledSurface(
        mm,
        isLocalMemory,
        GL_FALSE,   // not a z buffer
        width, height, bitsPerPixel,
        antiAliasing,
        compression,
        id,
        pitchAllocated,
        bytesAllocated );

    return *bytesAllocated > 0;
}

/////////////////////////////////////////////////////////////////////////////
// depth/stencil surface allcation

GLboolean _jsGcmAllocateDepthStencilSurface(
    GLboolean isLocalMemory,
    GLuint width,
    GLuint height,
    GLenum internalFormat,
    GLuint antiAliasing,
    GLuint *bufferId,
    GLuint *pitchAllocated,
    GLuint *bytesAllocated )
{
    jsTiledMemoryManager* mm = &_jsGcmTiledMemoryManager;
    GLuint bitsPerPixel = _jsPlatformGetBitsPerPixel(internalFormat);
    unsigned int zFormat = CELL_GCM_ZCULL_Z24S8;
	int32_t retVal = 0;

    // compression type depends on antialiasing (see cellGcmSetTile in GCM library reference)
    GLenum compression = CELL_GCM_COMPMODE_DISABLED;
    switch ( antiAliasing )
    {
        case 1: // CELL_GCM_SURFACE_CENTER_1
            compression = CELL_GCM_COMPMODE_Z32_SEPSTENCIL_REGULAR;
            break;
        case 2: // CELL_GCM_SURFACE_DIAGONAL_CENTERED_2
            compression = CELL_GCM_COMPMODE_Z32_SEPSTENCIL_DIAGONAL;
            break;
        case 3: // CELL_GCM_SURFACE_SQUARE_CENTERED_4
            compression = CELL_GCM_COMPMODE_Z32_SEPSTENCIL_REGULAR;
            break;             
        case 4: // CELL_GCM_SURFACE_SQUARE_ROTATED_4
            compression = CELL_GCM_COMPMODE_Z32_SEPSTENCIL_ROTATED;
            break;
        default:
            JS_ASSERT( false );
            break;
    }

    // compression has to be turned off if using 16bit z-buffer
    if ( internalFormat == JSGCM_DEPTH_COMPONENT16 )
    {
        compression = CELL_GCM_COMPMODE_DISABLED;
        zFormat = CELL_GCM_ZCULL_Z16;
    }

    _jsGcmAllocateTiledSurface(
        mm,
        isLocalMemory,
        GL_TRUE,    // z buffer
        width, height,
        bitsPerPixel,
        antiAliasing,
        compression,
        bufferId,
        pitchAllocated,
        bytesAllocated );

    // create zcull region
    // only create zcull for local memory
    if ( *bytesAllocated && isLocalMemory )
    {
        // find unused region
        int id = -1;
        for ( int i = 0; i < _JSGCM_MAX_ZCULL_REGIONS; ++i )
        {
            if ( mm->zcull[i].bufferAddress == NULL )
            {
                id = i;
                break;
            }
        }

        if ( id >= 0 )
        {
            const int zWidth = _jsPad( width, 64 );
            const int zHeight = _jsPad( height, 64 );
            mm->zcull[id].zWidth = zWidth;
            mm->zcull[id].zHeight = zHeight;

            // allocate zcull space
            //  This allocation is in pixels.
            mm->zcull[id].zcullStart = ( intptr_t )_jsMemoryBlockManagerAllocAligned(
                                           &mm->zcullManager,
                                           zWidth * zHeight,
                                           _JSGCM_ZCULL_BUFFER_ALIGNMENT );

            if ( mm->zcull[id].zcullStart )
            {
                // associate zcull region with buffer
                mm->zcull[id].bufferAddress = gmmIdToAddress(*bufferId);

                // set up zcull region
                GLenum aaFormat = 0;
                switch ( antiAliasing )
                {
                    case 1:
                        aaFormat = CELL_GCM_SURFACE_CENTER_1;
                        break;
                    case 2:
                        aaFormat = CELL_GCM_SURFACE_DIAGONAL_CENTERED_2;
                        break;
                    case 3:
                        aaFormat = CELL_GCM_SURFACE_SQUARE_CENTERED_4;
                        break;
                    case 4:
                        aaFormat = CELL_GCM_SURFACE_SQUARE_ROTATED_4;
                        break;
                    default:
                        JS_ASSERT( 0 );
                }
                mm->zcull[id].aaFormat = aaFormat;

				// must wait until RSX is completely idle before calling cellGcmBindZcull 
				_jsGcmUtilWaitForIdle(); 

                retVal = cellGcmBindZcull(
                    id,
                    gmmIdToOffset(*bufferId),
                    zWidth, zHeight,
                    mm->zcull[id].zcullStart,   // cull start
                    zFormat,
                    aaFormat,
                    CELL_GCM_ZCULL_LESS,
                    CELL_GCM_ZCULL_LONES,
                    CELL_GCM_SCULL_SFUNC_LESS, 0x80, 0xff );    // stencil config
            	JS_ASSERT( retVal == CELL_OK );
                _jsGcmFifoFinish( &_jsGcmState.fifo );
            }
        }
    }

    return *bytesAllocated > 0;
}

void _jsGcmFreeTiledSurface( GLuint bufferId )
{
    _JS_REPORT_EXTRA( PSGL_REPORT_GPU_MEMORY_FREE, "Freeing GPU memory (tiled) at id 0x%08x", bufferId );
    gmmFree( bufferId );
}

void _jsGcmFreeDepthStencilSurface( GLboolean isLocalMemory, GLuint bufferId )
{
    jsTiledMemoryManager* mm = &_jsGcmTiledMemoryManager;
	int32_t retVal = 0;
    void *pointer = gmmIdToAddress(bufferId);

    // search for zcull region
    int id = -1;
    for ( int i = 0; i < _JSGCM_MAX_ZCULL_REGIONS; ++i )
    {
        if ( mm->zcull[i].bufferAddress == pointer )
        {
            id = i;
            break;
        }
    }

    if ( !isLocalMemory )
    {
        // if the depth buffer is in XDR
        // we should never have allocated zcull in the first place
        // id==-1 -> zcull region not found
        JS_ASSERT( id == -1 );
    }

    if ( id >= 0 )
    {
        // unassociate buffer and region
        mm->zcull[id].bufferAddress = NULL;

        // deallocate zcull buffer
        _jsMemoryBlockManagerFree( &mm->zcullManager, ( void* )( intptr_t )mm->zcull[id].zcullStart );

        // unregister zcull region
		_jsGcmUtilWaitForIdle(); 

        retVal = cellGcmUnbindZcull( id );
    	JS_ASSERT( retVal == CELL_OK ); 
        _jsGcmFifoFinish( &_jsGcmState.fifo );
    }

    _jsGcmFreeTiledSurface( bufferId );
}

/////////////////////////////////////////////////////////////////////////////
// video mode selection

typedef struct
{
    int width;
    int height;
    unsigned char hwMode;
}
VideoMode;

static const VideoMode sysutilModes[] =
    {
        {720, 480, CELL_VIDEO_OUT_RESOLUTION_480},
        {720, 576, CELL_VIDEO_OUT_RESOLUTION_576},
        {1280, 720, CELL_VIDEO_OUT_RESOLUTION_720},
        {1920, 1080, CELL_VIDEO_OUT_RESOLUTION_1080},
#if (OS_VERSION_NUMERIC >= 0x150)
        {1600, 1080, CELL_VIDEO_OUT_RESOLUTION_1600x1080}, // hardware scales to 1920x1080 from 1600x1080 buffer
        {1440, 1080, CELL_VIDEO_OUT_RESOLUTION_1440x1080}, // hardware scales to 1920x1080 from 1440x1080 buffer
        {1280, 1080, CELL_VIDEO_OUT_RESOLUTION_1280x1080}, // hardware scales to 1920x1080 from 1280x1080 buffer
        {960, 1080, CELL_VIDEO_OUT_RESOLUTION_960x1080},   // hardware scales to 1920x1080 from 960x1080 buffer
#endif
    };
static const int sysutilModeCount = sizeof( sysutilModes ) / sizeof( sysutilModes[0] );

static const VideoMode *findModeByResolutionInTable( int width, int height, const VideoMode *table, int modeCount )
{
    for ( int i = 0;i < modeCount;++i )
    {
        const VideoMode *vm = table + i;
        if (( vm->width == width ) && ( vm->height  == height ) ) return vm;
    }
    return NULL;
}

static inline const VideoMode *findModeByResolution( int width, int height )
{
    return findModeByResolutionInTable( width, height, sysutilModes, sysutilModeCount );
}

static const VideoMode *findModeByEnum( GLenum TVStandard )
{
    const VideoMode *vm = NULL;
    switch ( TVStandard )
    {
        case PSGL_TV_STANDARD_NTSC_M:
        case PSGL_TV_STANDARD_NTSC_J:
        case PSGL_TV_STANDARD_HD480P:
        case PSGL_TV_STANDARD_HD480I:
            vm = &(sysutilModes[0]);
            break;
        case PSGL_TV_STANDARD_PAL_M:
        case PSGL_TV_STANDARD_PAL_B:
        case PSGL_TV_STANDARD_PAL_D:
        case PSGL_TV_STANDARD_PAL_G:
        case PSGL_TV_STANDARD_PAL_H:
        case PSGL_TV_STANDARD_PAL_I:
        case PSGL_TV_STANDARD_PAL_N:
        case PSGL_TV_STANDARD_PAL_NC:
        case PSGL_TV_STANDARD_HD576I:
        case PSGL_TV_STANDARD_HD576P:
            vm = &(sysutilModes[1]);
            break;
        case PSGL_TV_STANDARD_HD720P:
        case PSGL_TV_STANDARD_1280x720_ON_VESA_1280x768:
        case PSGL_TV_STANDARD_1280x720_ON_VESA_1280x1024:
            vm = &(sysutilModes[2]);
            break;
        case PSGL_TV_STANDARD_HD1080I:
        case PSGL_TV_STANDARD_HD1080P:
        case PSGL_TV_STANDARD_1920x1080_ON_VESA_1920x1200:
            vm = &(sysutilModes[3]);
            break;
        default:
            vm = &(sysutilModes[2]);
            break; // do nothing
    }

    return vm;
}

// XXX ugly global to be returned by the function
static VideoMode _sysutilDetectedVideoMode;

const VideoMode *_jsDetectVideoMode()
{
    CellVideoOutState videoState;
    int ret = cellVideoOutGetState( CELL_VIDEO_OUT_PRIMARY, 0, &videoState );
    if ( ret < 0 )
    {
        _JS_REPORT_EXTRA( PSGL_REPORT_ASSERT, "couldn't read the video configuration, using a default 720p resolution" );
        videoState.displayMode.scanMode = CELL_VIDEO_OUT_SCAN_MODE_PROGRESSIVE;
        videoState.displayMode.resolutionId = CELL_VIDEO_OUT_RESOLUTION_720;
    }
    CellVideoOutResolution resolution;
    cellVideoOutGetResolution( videoState.displayMode.resolutionId, &resolution );

    _sysutilDetectedVideoMode.width = resolution.width;
    _sysutilDetectedVideoMode.height = resolution.height;
    _sysutilDetectedVideoMode.hwMode = videoState.displayMode.resolutionId;
    return &_sysutilDetectedVideoMode;
}

/////////////////////////////////////////////////////////////////////////////
// resc is enabled by setting ANY of the resc related device parameters (using the enable mask)
static inline int rescIsEnabled( PSGLdeviceParameters* params )
{
    return params->enable & ( PSGL_DEVICE_PARAMETERS_RESC_RENDER_WIDTH_HEIGHT |
                              PSGL_DEVICE_PARAMETERS_RESC_RATIO_MODE |
                              PSGL_DEVICE_PARAMETERS_RESC_PAL_TEMPORAL_MODE |
                              PSGL_DEVICE_PARAMETERS_RESC_INTERLACE_MODE |
							                PSGL_DEVICE_PARAMETERS_RESC_ADJUST_ASPECT_RATIO );
}


static void rescInit( const PSGLdeviceParameters* params, jsGcmDevice *gcmDevice )
{
    _JS_REPORT_EXTRA(PSGL_REPORT_RESC,"WARNING: RESC is enabled.");
    GLboolean result = 0;

    CellRescBufferMode dstBufferMode;
    if ( params->width == 720  && params->height == 480 )  dstBufferMode = CELL_RESC_720x480;
    else if ( params->width == 720  && params->height == 576 )  dstBufferMode = CELL_RESC_720x576;
    else if ( params->width == 1280 && params->height == 720 )  dstBufferMode = CELL_RESC_1280x720;
    else if ( params->width == 1920 && params->height == 1080 ) dstBufferMode = CELL_RESC_1920x1080;
    else
    {
        dstBufferMode = CELL_RESC_720x480;
        fprintf( stderr, "Invalid display resolution for resolution conversion: %ux%u. Defaulting to 720x480...\n", params->width, params->height );
    }

    CellRescInitConfig conf;
    memset( &conf, 0, sizeof( CellRescInitConfig ) );
    conf.size            = sizeof( CellRescInitConfig );
    conf.resourcePolicy  = CELL_RESC_MINIMUM_GPU_LOAD | CELL_RESC_CONSTANT_VRAM;
    conf.supportModes    = CELL_RESC_720x480 | CELL_RESC_720x576 | CELL_RESC_1280x720 | CELL_RESC_1920x1080;
    conf.ratioMode       = ( params->rescRatioMode == RESC_RATIO_MODE_FULLSCREEN ) ? CELL_RESC_FULLSCREEN :
                           ( params->rescRatioMode == RESC_RATIO_MODE_PANSCAN ) ? CELL_RESC_PANSCAN :
                           CELL_RESC_LETTERBOX;
    conf.palTemporalMode = ( params->rescPalTemporalMode == RESC_PAL_TEMPORAL_MODE_60_DROP ) ? CELL_RESC_PAL_60_DROP :
                           ( params->rescPalTemporalMode == RESC_PAL_TEMPORAL_MODE_60_INTERPOLATE ) ? CELL_RESC_PAL_60_INTERPOLATE :
                           ( params->rescPalTemporalMode == RESC_PAL_TEMPORAL_MODE_60_INTERPOLATE_30_DROP ) ? CELL_RESC_PAL_60_INTERPOLATE_30_DROP :
                           ( params->rescPalTemporalMode == RESC_PAL_TEMPORAL_MODE_60_INTERPOLATE_DROP_FLEXIBLE ) ? CELL_RESC_PAL_60_INTERPOLATE_DROP_FLEXIBLE :
                           CELL_RESC_PAL_50;
    conf.interlaceMode   = ( params->rescInterlaceMode == RESC_INTERLACE_MODE_INTERLACE_FILTER ) ? CELL_RESC_INTERLACE_FILTER :
                           CELL_RESC_NORMAL_BILINEAR;
    cellRescInit( &conf );

    // allocate all the destination scanout buffers using the PSGL memory manager
    GLuint size;
    GLuint colorBuffersPitch;
    uint32_t numColorBuffers = cellRescGetNumColorBuffers( dstBufferMode, ( CellRescPalTemporalMode )conf.palTemporalMode, 0 );
    result = _jsGcmAllocateColorSurface( GL_TRUE, params->width, params->height * numColorBuffers,
                       4*8, JSGCM_TRUE, 1, &(gcmDevice->RescColorBuffersId), &colorBuffersPitch, &size );
    JS_ASSERT( result );

    // set the destination buffer format and pitch
    CellRescDsts dsts = { CELL_RESC_SURFACE_A8R8G8B8, colorBuffersPitch, 1 };
    cellRescSetDsts( dstBufferMode, &dsts );

    // set the resc output display mode (destination format)
    cellRescSetDisplayMode( dstBufferMode );

    // allocate space for vertex array and fragment shader for drawing the rescaling texture-mapped quad
    int32_t colorBuffersSize, vertexArraySize, fragmentShaderSize;
    cellRescGetBufferSize( &colorBuffersSize, &vertexArraySize, &fragmentShaderSize );
    gcmDevice->RescVertexArrayId    = _jsGcmAllocateMemory( vertexArraySize );
    gcmDevice->RescFragmentShaderId = _jsGcmAllocateMemory( fragmentShaderSize );

    // tell resc how to access the destination (scanout) buffer
    cellRescSetBufferAddress( gmmIdToAddress(gcmDevice->RescColorBuffersId),
                              gmmIdToAddress(gcmDevice->RescVertexArrayId),
                              gmmIdToAddress(gcmDevice->RescFragmentShaderId) );

    // scale to adjust for overscan
    cellRescAdjustAspectRatio( params->horizontalScale, params->verticalScale );

    // allocate an interlace table if interlace filtering is used
    if ((params->enable & PSGL_DEVICE_PARAMETERS_RESC_INTERLACE_MODE) &&
        (params->rescInterlaceMode == RESC_INTERLACE_MODE_INTERLACE_FILTER))
    {
      const unsigned int tableLength = 32; // this was based on the guidelines in the resc reference guide
      unsigned int tableSize = sizeof(uint16_t) * 4 * tableLength; // 2 bytes per FLOAT16 * 4 values per entry * length of table
      void *interlaceTable = gmmIdToAddress(_jsGcmAllocateMemory(tableSize));
      JS_ASSERT(interlaceTable);
      int32_t errorCode = cellRescCreateInterlaceTable(interlaceTable,params->renderHeight,CELL_RESC_ELEMENT_HALF,tableLength);
      JS_ASSERT(errorCode==CELL_OK);
      (void)errorCode;
    }
}

// Semaphore for PPU wait
static sys_semaphore_t FlipSem;

// A flip callback function to release semaphore and write a label to lock the GPU
static void _jsFlipCallbackFunction(const uint32_t head)
{
    //printf("Flip callback: label value: %d -> 0\n", *labelAddress);
    int res = sys_semaphore_post(FlipSem,1);
    (void)res;  // unused variable is ok
    JS_ASSERT( res == CELL_OK );

    if(_CurrentDevice)
    {
        jsGcmDevice *gcmDevice = ( jsGcmDevice * )_CurrentDevice->platformDevice;
        JS_ASSERT( gcmDevice );
	    
        // If user flip hander presents, call it
        void (*handler)(const uint32_t) = gcmDevice->flipHandler;
        if (handler != NULL)
            handler((uint32_t)1);
    }
}

// A label for GPU to skip VSYNC
static volatile uint32_t *labelAddress = NULL;
static const uint32_t WaitLabelIndex = 111;

// VBlank callback function to write a label to release GPU
static void _jsVblankCallbackFunction(const uint32_t head)
{   
    (void)head;
    int status = *labelAddress;
    switch(status){
	  case 2:
		  if (cellGcmGetFlipStatus()==0){
			  cellGcmResetFlipStatus();
			  *labelAddress=1;
		  }
		  break;
        case 1:
            *labelAddress = 0;
            break;
        default:
            break;
            // wait until rsx set the status to 2
	}
    //printf("VBlank callback: label value: %d -> %d\n", status, *labelAddress);
}

// Resc version of VBlank callback function to write a label to release GPU
static void _jsRescVblankCallbackFunction(const uint32_t head)
{   
    (void)head;
    int status = *labelAddress;
    switch(status){
	  case 2:
		  if (cellRescGetFlipStatus()==0){
			  cellRescResetFlipStatus();
			  *labelAddress=1;
		  }
		  break;
        case 1:
            *labelAddress = 0;
            break;
        default:
            break;
            // wait until rsx set the status to 2
	}
    //printf("VBlank callback: label value: %d -> %d\n", status, *labelAddress);
}

void _jsSetFlipHandler( void( *handler )( const uint32_t head ), PSGLdevice* device )
{
    //cellGcmSetFlipHandler( handler );
    jsGcmDevice *gcmDevice = ( jsGcmDevice * )device->platformDevice;
    JS_ASSERT( gcmDevice );

    gcmDevice->flipHandler = handler;
    
}

/////////////////////////////////////////////////////////////////////////////
// create device

static void _jsSetDisplayMode( const VideoMode *vm, GLushort bitsPerPixel, GLuint pitch )
{
    CellVideoOutConfiguration videocfg;
    memset( &videocfg, 0, sizeof( videocfg ) );
    videocfg.resolutionId = vm->hwMode;
    videocfg.format = ( bitsPerPixel == 32 ) ? CELL_VIDEO_OUT_BUFFER_COLOR_FORMAT_X8R8G8B8 : CELL_VIDEO_OUT_BUFFER_COLOR_FORMAT_R16G16B16X16_FLOAT;
    videocfg.pitch = pitch;
    videocfg.aspect = CELL_VIDEO_OUT_ASPECT_AUTO;
    cellVideoOutConfigure( CELL_VIDEO_OUT_PRIMARY, &videocfg, NULL, 0 );
/*
int32_t ret = cellVideoOutConfigure( CELL_VIDEO_OUT_PRIMARY, &videocfg, NULL, 1 );
if (ret != CELL_OK){
	printf("cellVideoOutConfigure() failed. (0x%x)\n", ret);
}
CellVideoOutState videoState;
ret = cellVideoOutGetState(CELL_VIDEO_OUT_PRIMARY, 0, &videoState);
  if (ret != CELL_OK){
	printf("cellVideoOutGetState() failed. (0x%x)\n", ret);
}
printf("videoState.state = %s\n",
       videoState.state==CELL_VIDEO_OUT_OUTPUT_STATE_ENABLED ? "ENABLED" : 
       videoState.state==CELL_VIDEO_OUT_OUTPUT_STATE_DISABLED ? "DISABLED" : 
       "DUNNO!");
*/
}

int _jsPlatformCreateDevice( PSGLdevice* device )
{
    jsGcmDevice *gcmDevice = ( jsGcmDevice * )device->platformDevice;
    PSGLdeviceParameters* params = &device->deviceParameters;
    _jsDuringDestroyDevice = GL_FALSE;
    GLboolean result = 0;

    // Tile memory manager init
    _jsGcmTiledMemoryInit();

    const VideoMode *vm = NULL;
    if ( params->enable & PSGL_DEVICE_PARAMETERS_TV_STANDARD )
    {
        vm = findModeByEnum( params->TVStandard );
        if ( !vm ) return -1;
        params->width = vm->width;
        params->height = vm->height;
    }
    else if ( params->enable & PSGL_DEVICE_PARAMETERS_WIDTH_HEIGHT )
    {
        vm = findModeByResolution( params->width, params->height );
        if ( !vm ) return -1;
    }
    else
    {
        vm = _jsDetectVideoMode();
        if ( !vm ) return -1;
        params->width = vm->width;
        params->height = vm->height;
    }
    JS_ASSERT( vm );

    // set render width and height to match the display width and height, unless resolution conversion is specified
    if ( !(params->enable & PSGL_DEVICE_PARAMETERS_RESC_RENDER_WIDTH_HEIGHT) )
    {
        params->renderWidth = params->width;
        params->renderHeight = params->height;
    }

    if ( rescIsEnabled( params ) )
    {
        rescInit( params, gcmDevice );
    }

    gcmDevice->deviceType = 0;
    gcmDevice->TVStandard = params->TVStandard;

    // if resc enabled, vsync is always enabled
    gcmDevice->vsync = rescIsEnabled( params ) ? GL_TRUE : GL_FALSE;

    gcmDevice->skipFirstVsync = GL_FALSE;

	gcmDevice->skipFirstVsyncCountDown = -1;

    gcmDevice->ms = NULL;

    const GLuint width = params->renderWidth;
    const GLuint height = params->renderHeight;

    GLboolean fpColor = GL_FALSE;
    switch ( params->colorFormat )
    {
        case GL_RGBA16F_ARB:
            fpColor = GL_TRUE;
            break;
        case GL_ARGB_SCE:
            break;
            // GL_RGBA ?
        default:
            return -1;
    }

    GLboolean depth24 = GL_FALSE;
    switch ( params->depthFormat )
    {
        case GL_DEPTH_COMPONENT24:
            depth24 = GL_TRUE;
        case GL_DEPTH_COMPONENT16:
        case GL_NONE:
            break;
        default:
            return -1;
    }

    GLuint xscale=1, yscale=1, antiAliasingMode=1;
    switch ( params->multisamplingMode )
    {
        case GL_MULTISAMPLING_NONE_SCE:
            antiAliasingMode = 1;
            xscale = 1;
            yscale = 1;
            break;
        case GL_MULTISAMPLING_2X_DIAGONAL_CENTERED_SCE:
            antiAliasingMode = 2;
            xscale = 2;
            yscale = 1;
            break;
        case GL_MULTISAMPLING_4X_SQUARE_CENTERED_SCE:
            antiAliasingMode = 3;
            xscale = 2;
            yscale = 2;
            break;
        case GL_MULTISAMPLING_4X_SQUARE_ROTATED_SCE:
            antiAliasingMode = 4;
            xscale = 2;
            yscale = 2;
            break;
        default:
            return -1;
    }

    // if floating point framebuffer, no multi-sampling
    if (fpColor) antiAliasingMode = 1;

    // create color buffers
    //  The default color buffer format is currently always ARGB8.
    // single, double, or triple buffering
    for ( int i = 0; i < params->bufferingMode; ++i )
    {
        gcmDevice->color[i].source = _JSGCM_SURFACE_SOURCE_DEVICE;
        gcmDevice->color[i].width = width;
        gcmDevice->color[i].height = height;
        gcmDevice->color[i].bpp = fpColor ? 8 : 4;
        gcmDevice->color[i].samples = antiAliasingMode;
        gcmDevice->color[i].format = fpColor ? JSGCM_FLOAT_RGBA16 : JSGCM_ARGB8;
        gcmDevice->color[i].flip = GL_TRUE;
        gcmDevice->color[i].pool = _JSGCM_SURFACE_POOL_TILED_COLOR;

        // allocate tiled memory
        _jsAllocSetHint( _JS_ALLOCTYPE_COLOR_FRAMEBUFFER, i );
        GLuint size;
        result = _jsGcmAllocateColorSurface(
                               GL_TRUE,     // create in local memory 
                               width, height,           // dimensions
                               gcmDevice->color[i].bpp*8,  // bits per sample
                               JSGCM_TRUE,               // scan out enable
                               antiAliasingMode,                // antiAliasing
                               &gcmDevice->color[i].dataId,    // returned buffer
                               &gcmDevice->color[i].pitch,
                               &size );
        JS_ASSERT( result );
        _jsAllocClearHint();

        // set scan buffer to color buffer
        //  If antialiasing is not used, then we scan directly from the
        //  color buffer.  If antialiasing is used, this will be
        //  overwritten with the scanout buffer.
        gcmDevice->scanoutColor[i] = gcmDevice->color[i];
    }

    // allocate scanout buffer
    //  No filter on scan, so a scan size buffer is needed.
    //  The scanout buffer creation is separated to minimize the number
    //  of tiled regions needed.
    if ( antiAliasingMode > 1 )
    {
        for ( int i = 0; i < ( int )params->bufferingMode; ++i )
        {
            JS_ASSERT( gcmDevice->color[i].bpp == 4 );
            _jsAllocSetHint( _JS_ALLOCTYPE_SCANOUT_FRAMEBUFFER, i );
            GLuint size;
            result = _jsGcmAllocateColorSurface(
                                         GL_TRUE,   // create in local memory
                                         width, height,     // dimensions
                                         4*8,                 // bits per sample
                                         JSGCM_TRUE,         // scan out enable
                                         1,                 // antiAliasing
                                         &gcmDevice->scanoutColor[i].dataId,   // returned buffer
                                         &gcmDevice->scanoutColor[i].pitch,
                                         &size );
            JS_ASSERT( result );
            _jsAllocClearHint();
        }
    }

    // create depth/stencil buffer
    if ( params->depthFormat != GL_NONE )
    {
        gcmDevice->depthStencil.source = _JSGCM_SURFACE_SOURCE_DEVICE;
        gcmDevice->depthStencil.width = width;
        gcmDevice->depthStencil.height = height;
        gcmDevice->depthStencil.bpp = (depth24) ? 4 : 2;
        gcmDevice->depthStencil.samples = antiAliasingMode;
        gcmDevice->depthStencil.format = (depth24) ? JSGCM_DEPTH_COMPONENT24 : JSGCM_DEPTH_COMPONENT16;
        gcmDevice->depthStencil.flip = GL_TRUE;
        gcmDevice->depthStencil.pool = _JSGCM_SURFACE_POOL_TILED_DEPTH;

        GLuint size;
        _jsAllocSetHint( _JS_ALLOCTYPE_DEPTH_FRAMEBUFFER, 0 );
        result = _jsGcmAllocateDepthStencilSurface(
                               GL_TRUE,     // create in local memory
                               width, height,   // dimensions
                               gcmDevice->depthStencil.format,
                               antiAliasingMode,        // antiAliasing
                               &gcmDevice->depthStencil.dataId,    // returned buffer
                               &gcmDevice->depthStencil.pitch,
                               &size );
        JS_ASSERT( result );
        _jsAllocClearHint();
    }
    else
    {
        gcmDevice->depthStencil.source = _JSGCM_SURFACE_SOURCE_DEVICE;
        gcmDevice->depthStencil.pool = _JSGCM_SURFACE_POOL_NONE;
    }

    memset( &gcmDevice->rt, 0, sizeof( jsGcmRenderTargetEx ) );
    gcmDevice->rt.colorBufferCount = 1;
    if ( gcmDevice->depthStencil.pool != _JSGCM_SURFACE_POOL_NONE )
    {
        gcmDevice->rt.depthFormat = gcmDevice->depthStencil.format;
        gcmDevice->rt.depthId = gcmDevice->depthStencil.dataId;
        gcmDevice->rt.depthPitch = gcmDevice->depthStencil.pitch;
    }
    else
    {
        gcmDevice->rt.depthId = GMM_ERROR;
    }
    gcmDevice->rt.aaSamples = antiAliasingMode;
    gcmDevice->rt.swizzled = GL_FALSE;
    gcmDevice->rt.yInverted = GL_TRUE;
    gcmDevice->rt.width = width;
    gcmDevice->rt.height = height;

    _jsGcmFifoGlViewport( 0, 0, width, height );
    _jsGcmFifoGlScissor( 0, 0, width, height );
    _jsGcmFifoGlClearColor( 0.f, 0.f, 0.f, 0.f );
    _jsGcmFifoGlClearDepthf( 1.f );
    _jsGcmFifoGlClearStencil( 0 );

    if ( fpColor )
    {
        // we don't yet have a fragment program to clear the buffer with a quad.
        // so we'll cheat pretending we have a RGBA buffer with twice the width.
        // Since we clear with 0 (which is the same in fp16), it works.

        if ( gcmDevice->depthStencil.pool != _JSGCM_SURFACE_POOL_NONE )
        {
            // clear depth
            gcmDevice->rt.colorFormat = JSGCM_NONE;
            _jsGcmFifoGlSetRenderTarget( &gcmDevice->rt );
            _jsGcmFifoGlClear( JSGCM_DEPTH_BUFFER_BIT | JSGCM_STENCIL_BUFFER_BIT );
        }
        gcmDevice->rt.width = 2 * width;
        gcmDevice->rt.depthFormat = JSGCM_NONE;
        gcmDevice->rt.colorFormat = JSGCM_ARGB8;
        for ( int i = 0; i < params->bufferingMode; ++i )
        {
            gcmDevice->rt.colorId[0] = gcmDevice->color[i].dataId;
            gcmDevice->rt.colorPitch[0] = gcmDevice->color[i].pitch;
            _jsGcmFifoGlSetRenderTarget( &gcmDevice->rt );
            _jsGcmFifoGlClear( JSGCM_COLOR_BUFFER_BIT );
        }
        // restore parameters
        gcmDevice->rt.width = width;
        gcmDevice->rt.colorFormat = gcmDevice->color[0].format;
        if ( gcmDevice->depthStencil.pool != _JSGCM_SURFACE_POOL_NONE )
        {
            gcmDevice->rt.depthFormat = gcmDevice->depthStencil.format;
            gcmDevice->rt.depthId = gcmDevice->depthStencil.dataId;
            gcmDevice->rt.depthPitch = gcmDevice->depthStencil.pitch;
        }
    }
    else
    {
        // clear the buffers for compression to work best
        gcmDevice->rt.colorFormat = JSGCM_ARGB8;
        for ( int i = 0; i < params->bufferingMode; ++i )
        {
            gcmDevice->rt.colorId[0] = gcmDevice->color[i].dataId;
            gcmDevice->rt.colorPitch[0] = gcmDevice->color[i].pitch;
            _jsGcmFifoGlSetRenderTarget( &gcmDevice->rt );
            _jsGcmFifoGlClear( JSGCM_COLOR_BUFFER_BIT | ( i == 0 ? ( JSGCM_DEPTH_BUFFER_BIT | JSGCM_STENCIL_BUFFER_BIT ) : 0 ) );
        }
    }

    gcmDevice->scanBuffer = 0;
    if ( params->bufferingMode == PSGL_BUFFERING_MODE_SINGLE )
        gcmDevice->drawBuffer = 0;
    else if ( params->bufferingMode == PSGL_BUFFERING_MODE_DOUBLE )
        gcmDevice->drawBuffer = 1;
    else if ( params->bufferingMode == PSGL_BUFFERING_MODE_TRIPLE )
        gcmDevice->drawBuffer = 2;


	// Create semaphore
    sys_semaphore_attribute_t attr;//todo: configure
    sys_semaphore_attribute_initialize(attr);

    // Set initial and max count of semaphore based on buffering mode
    sys_semaphore_value_t initial_val = 0;
    sys_semaphore_value_t max_val = 1;
    switch (device->deviceParameters.bufferingMode)
    {
    case PSGL_BUFFERING_MODE_SINGLE:
        initial_val = 0;
        max_val = 1;
        break;
    case PSGL_BUFFERING_MODE_DOUBLE:
        initial_val = 1;
        max_val = 2;
        break;
    case PSGL_BUFFERING_MODE_TRIPLE:
        initial_val = 2;
        max_val = 3;
        break;
    default:
        JS_ASSERT(0);
    }

    int res = sys_semaphore_create(&FlipSem, &attr, initial_val, max_val);
    (void)res;  // unused variable is ok
    JS_ASSERT (res == CELL_OK);

    // Register flip callback
    if ( rescIsEnabled( params ) )
		cellRescSetFlipHandler(_jsFlipCallbackFunction);
	else
		cellGcmSetFlipHandler(_jsFlipCallbackFunction);


    if ( rescIsEnabled( params ) )
    {
        for ( int i = 0; i < params->bufferingMode; ++i ) // bufferingMode should always be single, but for now....
        {
            // Set the RESC CellRescSrc buffer (source buffer to scale - psgl's render target):
            // **NOT SURE WHAT TO DO ABOUT FP BUFFERS YET** case CELL_GCM_TEXTURE_W16_Z16_Y16_X16_FLOAT for fp16

            CellRescSrc rescSrc = { CELL_GCM_TEXTURE_A8R8G8B8 | CELL_GCM_TEXTURE_LN | CELL_GCM_TEXTURE_NR, // uint8_t format
                                    gcmDevice->scanoutColor[i].pitch,                           // uint32_t pitch
                                    width,                                                     // uint16_t width
                                    height,                                                    // uint16_t height
                                    gmmIdToOffset( gcmDevice->scanoutColor[i].dataId ) }; // uint32_t offset

            if ( cellRescSetSrc( i, &rescSrc ) != CELL_OK )
            {
                fprintf( stderr, "Registering display buffer %d failed\n", i );
                JS_ASSERT( 0 );
                return -1;
            }
        }
    }
    else
    {
        _jsSetDisplayMode( vm, gcmDevice->color[0].bpp*8, gcmDevice->scanoutColor[0].pitch );

        cellGcmSetFlipMode( gcmDevice->vsync ? CELL_GCM_DISPLAY_VSYNC : CELL_GCM_DISPLAY_HSYNC );
        _jsGcmFifoGlFinish();

        for ( int i = 0; i < params->bufferingMode; ++i )
        {
            if ( cellGcmSetDisplayBuffer( i, gmmIdToOffset( gcmDevice->scanoutColor[i].dataId ), gcmDevice->scanoutColor[i].pitch , width, height ) != CELL_OK )
            {
                fprintf( stderr, "Registering display buffer %d failed\n", i );
                JS_ASSERT( 0 );
                return -1;
            }
        }
    }

    // Initialize label by getting address
    labelAddress = (volatile uint32_t *)cellGcmGetLabelAddress(WaitLabelIndex);	
    *labelAddress = 0;

    gcmDevice->mutex = _jsCreateMutex();
    if ( !gcmDevice->mutex )
    {
        return -1;
    }
    gcmDevice->cond = _jsCreateCond( gcmDevice->mutex );
    if ( !gcmDevice->cond )
    {
        _jsDestroyMutex( gcmDevice->mutex );
        return -1;
    }

    _jsGcmFifoGlIncFenceRef( &gcmDevice->swapFifoRef );

    //swapFifoRef2 used for triple buffering
    gcmDevice->swapFifoRef2 = gcmDevice->swapFifoRef;

    return 0;
}

/////////////////////////////////////////////////////////////////////////////
// destroy device

void _jsPlatformDestroyDevice( PSGLdevice* device )
{
    JS_ASSERT( device != NULL );
    jsGcmDevice *gcmDevice = ( jsGcmDevice * )device->platformDevice;
    PSGLdeviceParameters* params = &device->deviceParameters;

    _jsGcmFifoGlFinish();

    // Stop flip callback
    if ( rescIsEnabled( params ) )
		cellRescSetFlipHandler(NULL);
	else
		cellGcmSetFlipHandler(NULL);

    // Stop VBlank callback
	if ( rescIsEnabled( &device->deviceParameters ) )
	    cellRescSetVBlankHandler(NULL);
	else
	    cellGcmSetVBlankHandler(NULL);

    // Destroy semaphore
    int res = sys_semaphore_destroy(FlipSem);
    (void)res;  // prevent unused variable warning in opt build
    JS_ASSERT(res == CELL_OK);

    if ( rescIsEnabled( params ) )
    {
        cellRescExit();
        _jsGcmFreeTiledSurface(gcmDevice->RescColorBuffersId);
        _jsGcmFreeMemory(gcmDevice->RescVertexArrayId);
        _jsGcmFreeMemory(gcmDevice->RescFragmentShaderId);
    }

    // single, double, or triple buffering
    _jsDuringDestroyDevice = GL_TRUE;
    for ( int i = 0; i < params->bufferingMode; ++i )
    {
        if ( gcmDevice->color[i].pool != _JSGCM_SURFACE_POOL_NONE )
            _jsGcmFreeTiledSurface( gcmDevice->color[i].dataId );
    }
    _jsDuringDestroyDevice = GL_FALSE;

    if ( gcmDevice->depthStencil.pool != _JSGCM_SURFACE_POOL_NONE )
        _jsGcmFreeDepthStencilSurface( GL_TRUE, gcmDevice->depthStencil.dataId );

    if ( gcmDevice->cond )
        _jsDestroyCond( gcmDevice->cond );

    if ( gcmDevice->mutex )
        _jsDestroyMutex( gcmDevice->mutex );

    _jsGcmTiledMemoryFinish();
}

///////////////////////////////////////////////////////////////////////////
// internal fifo debugging

#if 0
// going to make 2 static 1 megabit fifos
static char * currentFifo = NULL;
static int sizeFifo = 0;

static void _jsGetCurrentFifo()
{
    // allocated for the current and old fifo and check if it has been modifided
    if ( !currentFifo )
    {
        currentFifo = ( char * )malloc( _jsGcmResource.hostMemorySize + 1 );
    }


    // set begin
    __GLNVdata* begin = _jsGcmState.fifo.dmaPushBuffer;
    // set fence
    // jsFifoData* end = _jsGcmState.fifo.fence;
    // set current
    __GLNVdata* current = _jsGcmState.fifo.current;

    sizeFifo = ( int )( current - begin );
    memcpy( currentFifo, ( void * )begin, sizeFifo );
    printf( "Size Fifo Before %d \n", sizeFifo );
}

static void _jsDumpFifo( char * name, char * data, int s )
{
    FILE * file = NULL ;
    file = fopen( name, "w+b" );

    fwrite( data, s, 1, file );

    fclose( file );
}

void _jsDumpFifo( char * name )
{
    printf( "[RSTENSON TEMP] Dumping Fifo \n" );
    _jsGetCurrentFifo();
    _jsDumpFifo( name, currentFifo, sizeFifo );
}
#endif


//#define _JS_CALLCRASH_WORKAROUND
#ifdef _JS_CALLCRASH_WORKAROUND
#include "LightweightPushbufferIterator.h"
void _jsPerformCallcrashWorkaround( uint32_t* pGet, uint32_t* pPut )
{
    jsLightweightPushbufferIterator it;
    it.init( pGet );
    it.setPut( pPut );
    while ( it.hasMore() )
    {
        if ( it.isCallLong() )
        {
            *it.getCur() = CELL_GCM_METHOD( 0x0000, 0 ); // noop this
            return;
        }
        else
        {
            it.step();
        }
    }
}
#endif

///////////////////////////////////////////////////////////////////////////
// swap buffers
void _jsRestoreContextAfterFlip()
{
    // we have to reset all the DMA contexts, they may have been changed.

    // [RSTENSON] don't need these anymore
    //_jsGcmInitBlitterContextDMAs();
    //_jsGcmInit3DContextDMAs();

    // restore states not owned by the context
    {
//		jsFifoData *  __restrict jsGcmCurrent = _jsGcmState.fifo.current;

        // vertex program shared constants
        const char * __restrict v = _jsGetGcmDriver()->sharedVPConstants;
        /*
        		JSGCM_GET_VARIABLE_SPACE_IF_NEEDED((JS_MAX_VP_SHARED_CONSTANTS/8)*(3+4*8));
        		for (int i=0;i<JS_MAX_VP_SHARED_CONSTANTS;i+=8)
        		{
        			JSGCM_PUSH_1U(JSGCM_SC_3D, JSGCM_3DMETHOD(SET_TRANSFORM_CONSTANT_LOAD),i);
        			JSGCM_PUSH_COMMAND(4*8, JSGCM_SC_3D, JSGCM_3DMETHOD(SET_TRANSFORM_CONSTANT)(0));
        			__builtin_memcpy(jsGcmCurrent,v,4*4*8);
        			v+=4*4*8;
        			jsGcmCurrent+=4*8;
        		}
        		JSGCM_GET_SKID_SPACE_IF_NEEDED();
        */
        GCM_FUNC( cellGcmSetVertexProgramParameterBlock, 0, 8, ( float* )v ); // GCM_PORT_UNTESTED [KHOFF]

        // we never set those, and they may be lost by the flip
//		JSGCM_PUSH_1U(JSGCM_SC_3D, JSGCM_3DMETHOD(SET_POLY_SMOOTH_ENABLE), 0);
//		JSGCM_PUSH_1U(JSGCM_SC_3D, JSGCM_3DMETHOD(SET_LINE_STIPPLE), 0);
//		JSGCM_PUSH_1U(JSGCM_SC_3D, JSGCM_3DMETHOD(SET_POLYGON_STIPPLE), 0);
//		JSGCM_GET_SKID_SPACE_IF_NEEDED();
        GCM_FUNC( cellGcmSetPolySmoothEnable, CELL_GCM_FALSE );
        GCM_FUNC( cellGcmSetLineStippleEnable, CELL_GCM_FALSE );
        GCM_FUNC( cellGcmSetPolygonStippleEnable, CELL_GCM_FALSE );
    }

    _jsGcmFifoGlDisable( JSGCM_DEPTH_BOUNDS_TEST );
    _jsGcmFifoGlEnable( JSGCM_DITHER );

    _jsInvalidateAllStates( _CurrentContext );

	if ( _jsGcmState.reportsInMainMemory )
	{
		//If using report to main memory, cellGcmSetReportLocation(CELL_GCM_LOCATION_MAIN) must called every frame.
		//After flip, Report Location is always set CELL_GCM_LOCATION_LOCAL.
		cellGcmSetReportLocation(CELL_GCM_LOCATION_MAIN);
	}
}

void _jsPlatformSwapBuffers( PSGLdevice* device )
{
    // update pending free allocations to free list 
    gmmUpdateFreeList(CELL_GCM_LOCATION_LOCAL);
    gmmUpdateFreeList(CELL_GCM_LOCATION_MAIN);

    jsGcmDevice *gcmDevice = ( jsGcmDevice * )device->platformDevice;
    JS_ASSERT( gcmDevice );

    const GLuint drawBuffer = gcmDevice->drawBuffer;
    GLuint nsamples = gcmDevice->color[drawBuffer].samples;
    if ( nsamples > 1 )
    {
        _jsGcmScaleBlit(
            gcmDevice->scanoutColor[drawBuffer].dataId,    // dst address
            gcmDevice->scanoutColor[drawBuffer].dataIdOffset,
            gcmDevice->scanoutColor[drawBuffer].pitch,   // dst pitch
            gcmDevice->color[drawBuffer].dataId,
            gcmDevice->color[drawBuffer].dataIdOffset,
            gcmDevice->color[drawBuffer].pitch,
            gcmDevice->color[drawBuffer].width,
            gcmDevice->color[drawBuffer].height,
            gcmDevice->color[drawBuffer].bpp,
            -1, ( nsamples > 2 ) ? -1 : 0,     // log of scale
            GL_TRUE );
    }
    else
    {
        JS_ASSERT( gcmDevice->color[drawBuffer].dataId == gcmDevice->scanoutColor[drawBuffer].dataId );
        JS_ASSERT( gcmDevice->color[drawBuffer].pitch == gcmDevice->scanoutColor[drawBuffer].pitch );
    }

#ifdef JS_RSXFIFODUMP
    _jsDecodeAndDumpFifo();
#endif

    GLboolean vsync = _CurrentContext->VSync;
    if ( vsync != gcmDevice->vsync )
    {
        // if resc disabled, vsync may be changed; if resc enabled, vsync must always remain enabled.
        if ( ! rescIsEnabled( &device->deviceParameters ) )
        {
            cellGcmSetFlipMode( vsync ? CELL_GCM_DISPLAY_VSYNC : CELL_GCM_DISPLAY_HSYNC );
            gcmDevice->vsync = vsync;
        }
    }

	// start or shut down skip-first-vsync
    if ( _CurrentContext->SkipFirstVSync != gcmDevice->skipFirstVsync )
    {   // Need to update callback
        if ( _CurrentContext->SkipFirstVSync == GL_TRUE )
        {

	        if ( rescIsEnabled( &device->deviceParameters ) )
		        cellRescSetVBlankHandler(_jsRescVblankCallbackFunction);
	        else
	            cellGcmSetVBlankHandler(_jsVblankCallbackFunction);
        }
        else
        {
			// set count down depending on buffering mode
			gcmDevice->skipFirstVsyncCountDown = device->deviceParameters.bufferingMode;
        }
        gcmDevice->skipFirstVsync = _CurrentContext->SkipFirstVSync;
    }

	// shut down skip-first-vsync
	switch( gcmDevice->skipFirstVsyncCountDown )
	{
	case 3:
	case 2:
	case 1:
		gcmDevice->skipFirstVsyncCountDown--;
		break;
	case 0:
		// Unregister VBlank handler
        if ( rescIsEnabled( &device->deviceParameters ) )
	        cellRescSetVBlankHandler(NULL);
        else
            cellGcmSetVBlankHandler(NULL);
		gcmDevice->skipFirstVsyncCountDown = -1;
		break;
	default:
		break;
	}
    
	// push commands for skip-first-vsync
    if ( gcmDevice->skipFirstVsync == GL_TRUE )
    {
        // Make RSX wait for label
        cellGcmSetWaitLabel( WaitLabelIndex, 0 );
        cellGcmSetWriteCommandLabel( WaitLabelIndex, 2 );   
        //printf("cellGcmSetWaitLabel: label value: %d\n", *labelAddress);
    }

	if ( device->deviceParameters.bufferingMode == PSGL_BUFFERING_MODE_TRIPLE )
	{
		if ( rescIsEnabled( &device->deviceParameters ) )
		{		
			cellRescSetWaitFlip(); // GPU will wait until flip actually occurs
		}
		else
		{
			cellGcmSetWaitFlip(); // GPU will wait until flip actually occurs
		}
	}

	if ( rescIsEnabled( &device->deviceParameters ) )
    {
#if (CELL_SDK_VERSION < 0x190000 )
		cellGcmSetTransferLocation(CELL_GCM_LOCATION_LOCAL); // WAR for Bugzilla 36243
#endif
		int32_t res = cellRescSetConvertAndFlip(( uint8_t ) drawBuffer ); 
        if ( res != CELL_OK )
        {
		    _JS_REPORT_EXTRA(PSGL_REPORT_RESC_FLIP_ERROR, "WARNING: RESC cellRescSetConvertAndFlip returned error code %d.\n", res);
		    if ( _CurrentContext ) _CurrentContext->needValidate |= PSGL_VALIDATE_FRAMEBUFFER;
			return;
		}
    }
    else
    {
#ifdef _JS_CALLCRASH_WORKAROUND
        uint32_t* pCurBefore = ( uint32_t* )_jsGcmState.fifo.current;
#endif
        if ( cellGcmSetFlip(( uint8_t ) drawBuffer ) != CELL_OK )
        {
            JS_ASSERT( 0 );
        }

#ifdef _JS_CALLCRASH_WORKAROUND
        uint32_t* pCurAfter = ( uint32_t* )_jsGcmState.fifo.current;
        _jsPerformCallcrashWorkaround( pCurBefore, pCurAfter );
#endif
    }

	if ( device->deviceParameters.bufferingMode != PSGL_BUFFERING_MODE_TRIPLE )
	{
		if ( rescIsEnabled( &device->deviceParameters ) )
		{		
			cellRescSetWaitFlip(); // GPU will wait until flip actually occurs
		}
		else
		{
			cellGcmSetWaitFlip(); // GPU will wait until flip actually occurs
		}
	}

    _jsRestoreContextAfterFlip();

    _jsGcmFifoGlFlush(); 

    // Let PPU wait for semaphore
    while (true)
    {
        int ret = sys_semaphore_wait(FlipSem,1000);
        
        if ( ret == CELL_OK )
            break;
        else if ( ret == ETIMEDOUT )
            continue;
        else if ( ret == EABORT )
            continue; // XXX What to do if process is suspended?
        else 
            JS_ASSERT (0);
    }

    // handle dynamic zcull parameters
    _jsGcmRequestAndUpdateZCullFeedback();

    // kick off all pending work.
    _jsGcmFifoGlFlush();

    // swap buffers
    if ( device->deviceParameters.bufferingMode == PSGL_BUFFERING_MODE_DOUBLE )
    {
        gcmDevice->drawBuffer = gcmDevice->scanBuffer;
        gcmDevice->scanBuffer = drawBuffer;

        gcmDevice->rt.colorId[0] = gcmDevice->color[gcmDevice->drawBuffer].dataId;
        gcmDevice->rt.colorPitch[0] = gcmDevice->color[gcmDevice->drawBuffer].pitch;
    }
    else if ( device->deviceParameters.bufferingMode == PSGL_BUFFERING_MODE_TRIPLE )
    {
        gcmDevice->drawBuffer = gcmDevice->scanBuffer;
        if ( gcmDevice->scanBuffer == 2 )
            gcmDevice->scanBuffer = 0;
        else
            gcmDevice->scanBuffer++;

        gcmDevice->rt.colorId[0] = gcmDevice->color[gcmDevice->drawBuffer].dataId;
        gcmDevice->rt.colorPitch[0] = gcmDevice->color[gcmDevice->drawBuffer].pitch;
    }
    // do nothing if single buffer
}
