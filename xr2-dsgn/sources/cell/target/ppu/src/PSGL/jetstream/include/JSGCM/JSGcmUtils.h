/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 *                Copyright (C) 2009 Sony Computer Entertainment Inc.
 *                                               All Rights Reserved.
 */

 
 /*
 * File description here
 */

#ifndef _JSGCM_UTILS_H_
#define _JSGCM_UTILS_H_

#include <PSGL/private.h>
#include <Cg/cgBinary.h>
#include <stdio.h>
#include <JSGCM/GmmAlloc.h>


// GCM RESERVE and no JSGCM post fifo SKID Check
#define GCM_RESERVE_NO_SKID

#ifdef GCM_RESERVE_NO_SKID


#define GCM_FUNC_RESERVE( count )\
					  CELL_GCM_RESERVE(count);

#define GCM_DEBUG
//	printf(" gcm %s %d \n", __FILE__, __LINE__ ); fflush(stdout);


#define GCM_FUNC_SAFE( GCM_FUNCTION, ...) \
					  { \
                      GCM_FUNCTION( (CellGcmContextData*)&_jsGcmState.fifo, __VA_ARGS__ ); \
					  GCM_DEBUG \
					  }

#define GCM_FUNC_SAFE_NO_ARGS( GCM_FUNCTION, ...) \
					  { \
                      GCM_FUNCTION( (CellGcmContextData*)&_jsGcmState.fifo ); \
					  GCM_DEBUG \
					  }

#define GCM_FUNC( GCM_FUNCTION, ...) \
					  { \
                      GCM_FUNCTION ## Inline( (CellGcmContextData*)&_jsGcmState.fifo, __VA_ARGS__ ); \
					  GCM_DEBUG \
					  }

#define GCM_FUNC_NO_ARGS( GCM_FUNCTION ) \
					  { \
                      GCM_FUNCTION ## Inline( (CellGcmContextData*)&_jsGcmState.fifo ); \
					  GCM_DEBUG \
					  }
#endif

/* [RSTENSON] To be able to build up buffered push buffers with GCM we need to use the
// cellGcmSetCurrentBuffer and reset the buffer with cellGcmDefualtCommandBuffer afterwards.
// Buffer size checks are not handled by these macros because they operate on pointers
// to data and not cellGcmContextDmas, thus the use of UnsafeInline versions vs Inline as above */

// This for pushing in to a memory buffer verses the actual active fifo
#define GCM_FUNC_BUFFERED_UNSAFE_INLINED

#ifdef GCM_FUNC_BUFFERED_UNSAFE_INLINED
#define GCM_FUNC_BUFFERED( GCM_FUNCTION, COMMAND_BUFFER, ...) \
					  { \
					  CellGcmContextData gcmContext; \
					  gcmContext.current = (uint32_t *)COMMAND_BUFFER; \
					  GCM_FUNCTION ## UnsafeInline( &gcmContext, __VA_ARGS__ ); \
					  COMMAND_BUFFER = (typeof(COMMAND_BUFFER))gcmContext.current;   \
					  GCM_DEBUG \
					  }

#define GCM_FUNC_BUFFERED_NO_ARGS( GCM_FUNCTION, COMMAND_BUFFER ) \
					  { \
					  CellGcmContextData gcmContext; \
					  gcmContext.current = (uint32_t *)COMMAND_BUFFER; \
					  GCM_FUNCTION ## UnsafeInline( &gcmContext ); \
					  COMMAND_BUFFER = (typeof(COMMAND_BUFFER))gcmContext.current;   \
					  GCM_DEBUG \
					  }

#endif


extern void _jsBeginGPUWaitFifo();
extern void _jsEndGPUWaitFifo();
extern void _jsBeginGPUFinish();
extern void _jsEndGPUFinish();


// Fifo Dump Utility [RSTENSON]
void _jsDumpFifo( char * name );
void _jsDumpFifoFromCurrent( char * name, int numWords );

// This is used to make sure the pass the gpu the
// right endianness.  RS
#define JSGCM_BIG_ENDIAN

// get a few hw specific constants
#include <JSGCM/JSGcmConsts.h>

// channel/context we need to store
#include <JSGCM/JSGcmState.h>

// GL enums etc
#include <JSGCM/JSGcmGlEnums.h>

// Misc functions from fifo scissor to floor functions 
#include <JSGCM/JSGcmMisc.h>

// GCM enable
#include <JSGCM/JSGcmEnable.h>

// GCM disable
#include <JSGCM/JSGcmDisable.h>

// JSGCM resource
#include <JSGCM/JSGcmResource.h>

// Validate Vertex Cache
#include <JSGCM/JSGcmInvalidateVertexCache.h>

#ifdef __cplusplus
extern "C"
{
#endif

#define _JSGCM_LINEAR_BUFFER_ALIGNMENT 128
#define _JSGCM_HOST_BUFFER_ALIGNMENT 128

#define _JSGCM_TRANSIENT_MEMORY_DEFAULT		(32 << 20)
#define _JSGCM_PERSISTENT_MEMORY_DEFAULT	(160 << 20)
#define _JSGCM_FIFO_SIZE_DEFAULT			(256 * 1024)
#define _JSGCM_HOST_SIZE_DEFAULT			(0)
#define _JSGCM_TRANSIENT_ENTRIES_DEFAULT	64


///////////////////////////////////////////////////////////////////////////
// verbose flag
#ifdef JS_DPM_V2
#undef JSGCM_CHATTY // off in DPM2
#else
#undef JSGCM_CHATTY // off in DPM
#endif
///////////////////////////////////////////////////////////////////////////


#ifdef _CHATTY
#define _jsGcmPrintf(...) printf(__VA_ARGS__)
#else
#define _jsGcmPrintf(...)
#endif


void _jsGcmTransferDataVidToVidYInv( GLuint dstVidId,
									 GLuint dstPitch,
									 GLuint dstX,
									 GLuint dstY,
									 GLuint srcVidId,
                                     GLuint srcVidIdOffset,
									 GLuint srcPitch,
									 GLuint width,
									 GLuint height,
									 GLuint bytesPerPixel );
void _jsGcmScaleBlit( GLuint dstVidId,
                      GLuint dstVidIdOffset,
					  GLuint dstPitch,
					  GLuint srcVidId,
                      GLuint srcVidIdOffset,
					  GLuint srcPitch,
					  GLuint dstWidth,
					  GLuint dstHeight,
					  GLuint bytesPerPixel,
					  int log2XScale,
					  int log2YScale,
					  GLboolean filter );
void _jsCurieScaleBlit(
	const GLuint dstVidId,
    GLuint dstVidIdOffset,
	GLuint dstPitch,
	GLuint dstWidth,
	GLuint dstHeight,
	const GLuint srcVidId,
    GLuint srcVidIdOffset,
	GLuint srcPitch,
	GLuint srcWidth,
	GLuint srcHeight,
	GLuint bytesPerPixel,
	GLboolean filter );
void _jsDrawUtilQuad( GLboolean useFixedVP, GLboolean useFixedFP, GLuint x, GLuint y, GLuint width, GLuint height );

GLboolean _jsGcmInit( PSGLinitOptions* options, jsGcmResource *resource );
void _jsGcmAllocDestroy();
GLuint _jsGcmGetGraphicsClock();
GLuint _jsGcmGetMemoryClock();
void _jsGcmMemcpy( unsigned int dstId, unsigned dstOffset, unsigned int pitch, unsigned int srcId, unsigned int srcOffset, unsigned int size );
void _jsGcmCopyBounceToGPU( unsigned int dstId, unsigned dstOffset, unsigned int pitch, unsigned int srcId, unsigned int srcOffset, unsigned int size );
void _jsGcmSend( unsigned int dstId, unsigned dstOffset, unsigned int pitch, const char *src, unsigned int size );
GLuint _jsGcmAllocateMemory( unsigned int size );
GLuint _jsGcmAllocateHostMemory( unsigned int size );
GLuint _jsGcmAllocateTextureMemory( unsigned int size );

inline void _jsGcmFreeMemory( GLuint id )
{
    _JS_REPORT_EXTRA( PSGL_REPORT_GPU_MEMORY_ASYNC_FREE, "Asynchronously freeing GPU memory at id 0x%08x", id );
    gmmFree( id );
}

inline void _jsGcmFreeHostMemory( GLuint id )
{
    _JS_REPORT_EXTRA( PSGL_REPORT_HOST_MEMORY_ASYNC_FREE, "Asynchronously freeing host memory at id 0x%08x", id );
    gmmFree( id );
}

inline void _jsGcmFreeTextureMemory( GLuint id )
{
    _JS_REPORT_EXTRA( PSGL_REPORT_GPU_MEMORY_ASYNC_FREE, "Asynchronously freeing GPU memory at id 0x%08x", id );
    gmmFree( id );
}

GLboolean _jsGcmAllocateColorSurface( GLboolean isLocalMemory, GLuint width, GLuint height, GLuint bitsPerPixel, GLuint scanoutSupported, GLuint antiAliasing, GLuint *id, GLuint *pitchAllocated, GLuint *bytesAllocated );
GLboolean _jsGcmAllocateDepthStencilSurface( GLboolean isLocalMemory, GLuint width, GLuint height, GLenum internalFormat, GLuint antiAliasing, GLuint *bufferId, GLuint *pitchAllocated, GLuint *bytesAllocated );
void _jsGcmFreeTiledSurface( GLuint bufferId );
void _jsGcmFreeDepthStencilSurface( GLboolean isLocalMemory, GLuint bufferId );

// RSX semaphore allocation
//  64-191	events
//  192		fence implementation (independent of nv_glFence)
//  253     used in JSGcmFifoUtils.h
#define _JSGCM_SEMA_NEVENTS	128
#define _JSGCM_SEMA_BASE	64	// libgcm uses 0-63
#define _JSGCM_SEMA_FENCE	(_JSGCM_SEMA_BASE+_JSGCM_SEMA_NEVENTS+0)

// synchronization
//  _jsGcmSync enables GPU waiting by sending nv_glAcquireSemaphore to the
//  GPU and returning a memory mapped pointer to the semaphore.  The GPU
//  will be released when 0 is written to the memory location.
//
//  _jsGcm{Inc,Test,Finish}FenceRef are intended to be drop-in replacements
//  for the corresponding JSGCM routines, using a semaphore instead of the
//  fence mechanism (so IncFence uses the 3D class).
void _jsGcmInitSync( void );
int* _jsGcmSync( void );
void _jsGcmIncFenceRef( GLuint* ref );
GLboolean _jsGcmTestFenceRef( const GLuint ref );
void _jsGcmFinishFenceRef( const GLuint ref );
GLuint _jsGcmGetFenceRef( void );

typedef struct
{
	GLuint fence;
}
jsGcmFenceObject;

typedef struct
{
	GLint sema;	// NV semaphore index
}
jsGcmEventObject;


typedef struct
{
	GLenum pool;		// LINEAR, SYSTEM, or NONE
	unsigned int bufferId;		// allocated Id
	unsigned int bufferSize;
	unsigned int pitch;

	GLuint mapCount;	// map reference count
	GLenum mapAccess;	// READ_ONLY, WRITE_ONLY, or READ_WRITE
}
jsGcmBufferObject;

// allocation unit for buffer objects
//  Each buffer object is allocated to a multiple of this block size.  This
//  must be at least 64 so that nv_glTransferDataVidToVid() can be used to
//  copy buffer objects within video memory.  This function performs a 2D
//  blit, and there is a 64-byte minimum pitch constraint.
//
//  Swizzled textures require 128-byte alignment, so this takes precedence.
#define _JS_BUFFER_OBJECT_BLOCK_SIZE 128


    void _jsGcmDeswizzleDataCPU
    (
        const char*swizzledTexture,
        char      *texture,
        GLuint               srcx0,
        GLuint               srcy0,
        GLuint               srcz0,
        GLuint               srcWidth,
        GLuint               srcHeight,
        GLuint               srcDepth,
        GLuint               dstx0,
        GLuint               dsty0,
        GLuint               dstz0,
        GLuint               dstx1,
        GLuint               dsty1,
        GLuint               dstz1,
        GLuint               dstWidth,
        GLuint               dstHeight,
        GLuint               dstDepth,
        GLuint               srcLog2cdepth,
        GLuint               dstColordepth,
        GLuint               border,
        GLuint               dim,
        void( *copyTexel )( void *src, const void *dst )
    );
    void _jsGcmSwizzleDataCPU( char *swizzledTexture, const char *texture, GLuint dstx0, GLuint dsty0, GLuint dstz0, GLuint dstWidth, GLuint dstHeight, GLuint dstDepth, GLuint srcx0, GLuint srcy0, GLuint srcz0, GLuint srcx1, GLuint srcy1, GLuint srcz1, GLuint srcWidth, GLuint srcHeight, GLuint srcDepth, GLuint dstLog2cdepth, GLuint srcColordepth, GLuint border, GLuint dim, void( *copyTexel )( void *dst, const void *src ) );
    void _jsGcmSwizzleDataHostToVid( void *dstVidAdr, GLuint dstWidth, GLuint dstHeight, GLuint dstX, GLuint dstY, const void *srcHostAdr, GLuint srcPitch, GLuint srcX, GLuint srcY, GLuint width, GLuint height, GLuint bytesPerPixel );
    void _jsGcmSwizzleDataVidToVid( GLuint dstVidId, GLuint dstVidIdOffset, GLuint dstWidth, GLuint dstHeight, GLuint dstX, GLuint dstY, GLuint srcVidId, GLuint srcVidIdOffset, GLuint srcPitch, GLuint srcX, GLuint srcY, GLuint width, GLuint height, GLuint bytesPerPixel );

    void _jsGcmTransferData( GLuint dstId, GLuint dstIdOffset, GLint dstPitch, GLuint srcId, GLuint srcIdOffset, GLint srcPitch, GLint bytesPerRow, GLint rowCount );
    void _jsGcmTransferDataVidToVid( GLuint dstId, GLuint dstIdOffset, GLuint dstPitch, GLuint dstX, GLuint dstY, GLuint srcId, GLuint srcIdOffset, GLuint srcPitch, GLuint srcX, GLuint srcY, GLuint width, GLuint height, GLuint bytesPerPixel );

    void _jsGcmDisableClearZCullStats( int disable );
    void _jsGcmRequestAndUpdateZCullFeedback( void );
    void _jsGcmUpdateZCullFeedbackParams( void );
    void _jsGcmUpdateZCullDepthParams( void );
    void _jsGcmUpdateZCullStencilParams( void );
    void _jsGcmSetStencilCullHint( jsGcmEnum func, GLint ref, GLuint mask );

    GLboolean _jsGcmSetPixelShaderConsistency( GLuint isPShaderPrecisonR );
    GLboolean _jsGcmCheckConsistency();

//TODO: remove _jsGcmSetNativeCgVertexProgram
    void _jsGcmSetNativeCgVertexProgram( const void *header, const void *ucode );
    void _jsSetNativeCgVertexProgram( const void *header );
    void _jsGcmSetNativeCgFragmentProgram( const GLvoid *header, const GLuint ucodeId );
    void _jsSetNativeCgFragmentProgram( const void *header );

// a native program needs to be initialized before any Cg* function is called
    void _jsGcmInitializeBinaryCgProgram( CGprogram prog );
    void _jsGcmGetUCode( CGprogram prog, char **pUCode, unsigned int *pUCodeSize );


// XXX in the following, CGtypes are not really CGtypes....
    CGparameter   _jsGcmFifoCgGetNamedParameter( CGprogram prog, const char *name );
    CGparameter   _jsGcmFifoCgGetFirstLeafParameter( CGprogram prog, CGenum name_space );
    CGparameter   _jsGcmFifoCgGetNextLeafParameter( CGprogram prog, CGparameter current );
    CGtype        _jsGcmFifoCgGetParameterType( CGprogram prog, CGparameter param );
    CGresource    _jsGcmFifoCgGetParameterResource( CGprogram prog, CGparameter param );
    const char   *_jsGcmFifoCgGetParameterName( CGprogram prog, CGparameter param );
    CGenum        _jsGcmFifoCgGetParameterVariability( CGprogram prog, CGparameter param );
    unsigned int  _jsGcmFifoCgGetParameterResourceIndex( CGprogram prog, CGparameter param );
    const float  *_jsGcmFifoCgGetParameterValues( CGprogram prog, CGparameter param );
    int           _jsGcmFifoCgGetParameterIndex( CGprogram prog, CGparameter param );
    CGenum        _jsGcmFifoCgGetParameterDirection( CGprogram prog, CGparameter param );
    const char   *_jsGcmFifoCgGetParameterSemantic( CGprogram prog, CGparameter param );
    CGprofile     _jsGcmFifoCgGetProgramProfile( CGprogram prog );

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
extern "C"
{
#endif


    int _jsSavePlatformBufferObjectAndUpdate( jsBufferObject * bufferObject, void * platformBufferObject, void * saveArea, unsigned long int * offset, unsigned long int offsetMax );
    int _jsRestorePlatformBufferObjectAndUpdate( jsBufferObject * bufferObject, void * platformBufferObject, jsBufferObject * jsbo, void * saveArea, unsigned long int * offset );

    int _jsSavePlatformFenceObjectAndUpdate( void * platformFenceObject, void * saveArea, unsigned long int * offset, unsigned long int offsetMax );
    int _jsRestorePlatformFenceObjectAndUpdate( void * platformFenceObject, void * saveArea, unsigned long int * offset );

    int _jsSavePlatformEventObjectAndUpdate( void * platformEventObject, void * saveArea, unsigned long int * offset, unsigned long int offsetMax );
    int _jsRestorePlatformEventObjectAndUpdate( void * platformEventObject, void * saveArea, unsigned long int * offset );


    int _jsSavePlatformMemoryManagerAndUpdate( void * MM, void * saveArea, unsigned long int * offset, unsigned long int offsetMax );
    int _jsRestorePlatformMemoryManagerAndUpdate( void * MM, void * saveArea, unsigned long int * offset );

    int _jsSavePlatformDataAndUpdate( void * saveArea, unsigned long int * offset, unsigned long int offsetMax );
    int _jsRestorePlatformDataAndUpdate( void * saveArea, unsigned long int * offset );

    int _jsSavePlatformFramebufferAndUpdate( void * framebuffer, void * saveArea, unsigned long int * offset, unsigned long int offsetMax );
    int _jsRestorePlatformFramebufferAndUpdate( void * framebuffer, void * saveArea, unsigned long int * offset );


// identify a block as a new region
//  A newly allocated block can be designated as a region by calling
//  _jsGcmAllocCreateRegion.  Both tag and data must be nonzero, and data
//  must be unique over all regions.  The region may then be extended by
//  calling _jsGcmAllocateExtend with a matching tag.
//
//  The intended use for regions is tiled memory management.  The region
//  pitch/compression can be encoded into the tag and the data can be a
//  pointer to other region data.
//
//  Any change to the region size (including the initial creation) will
//  invoke the callback (if not NULL), specifying the updated start address
//  and size of the region.  When all allocations in the region have been
//  freed, the region will no longer exist and the callback address and size
//  arguments are zero.
//
//  This function returns GL_TRUE if successful, or GL_FALSE if failed (e.g. not enough tag bits)

    GLboolean _jsGcmTryResizeTileRegion( GLuint address, GLuint size, void* data );
    void _jsGcmGetTileRegionInfo( void* data, GLuint *address, GLuint *size );

    GLuint _jsGcmAllocCreateRegion(
        uint8_t memoryLocation,
        GLboolean isZBuffer,
        GLuint size,
        GLint tag,
        void* data );


    static inline GLboolean _jsIsDrawableColorFromat( GLenum colorFormat )
    {
        switch ( colorFormat )
        {
            case JSGCM_ARGB8:
            case JSGCM_FLOAT_R32:
            case JSGCM_LUMINANCE32F_ARB:
            case JSGCM_FLOAT_RGBX16:
            case JSGCM_FLOAT_RGBA16:
            case JSGCM_FLOAT_RGBX32:
            case JSGCM_FLOAT_RGBA32:
                return GL_TRUE;
            default:
                return GL_FALSE;
        }
    }

    static inline GLboolean _jsIsDrawableDepthFormat( GLenum depthFormat )
    {
        switch ( depthFormat )
        {
            case JSGCM_DEPTH_COMPONENT16:
            case JSGCM_DEPTH_COMPONENT24:
                return GL_TRUE;
            default:
                return GL_FALSE;
        }
    }

    static inline GLuint _jsPlatformGetBitsPerPixel( GLenum internalFormat )
    {
        switch ( internalFormat )
        {
            case GL_COMPRESSED_RGB_S3TC_DXT1_EXT:
            case GL_COMPRESSED_RGBA_S3TC_DXT1_EXT:
                // 64 bits for 16 pixels = 4 bits/pixel
                return 4;
            case GL_COMPRESSED_RGBA_S3TC_DXT3_EXT:
            case GL_COMPRESSED_RGBA_S3TC_DXT5_EXT:
                // 128 bits for 16 pixels = 8 bits/pixel
                return 8;
            case JSGCM_ALPHA16:
            case JSGCM_INTENSITY16:
            case JSGCM_LUMINANCE16:
            case JSGCM_LUMINANCE8_ALPHA8:
            case JSGCM_HILO8:
            case JSGCM_RGB5_A1_SCE:
            case JSGCM_RGB565_SCE:
            case JSGCM_DEPTH_COMPONENT16:
                return 16;
            case JSGCM_ALPHA8:
            case JSGCM_INTENSITY8:
            case JSGCM_LUMINANCE8:
                return 8;
            case JSGCM_LUMINANCE16_ALPHA16:
            case JSGCM_RGBX8:
            case JSGCM_RGBA8:
            case JSGCM_ABGR8:
            case JSGCM_ARGB8:
            case JSGCM_BGRA8:
            case JSGCM_DEPTH_COMPONENT24:
            case JSGCM_LUMINANCE32F_ARB:
            case JSGCM_FLOAT_R32:
            case JSGCM_HILO16:
            case JSGCM_XBGR8:
                return 32;
            case JSGCM_FLOAT_RGBA16:
            case JSGCM_FLOAT_RGBX16:
                return 64;
            case JSGCM_FLOAT_RGBX32:
            case JSGCM_FLOAT_RGBA32:
                return 128;
            case JSGCM_ALPHA_LUMINANCE16F_SCE:
                return 32;
            default:
                JS_ASSERT( 0 );
                return 0;
        }
    }



    /* does not return until the effects of all previously called OpenGL functions
    ** are complete. Such effects include all changes to the OpenGL state and all
    ** changes to the framebuffer contents
    **
    ** notes:
    **   - 1:1 port of glFinish (see spec for details)
    **   - also invalidates vertex cache
    */
    void static inline _jsGcmFifoGlFinish( void )
    {
        _jsGcmFifoGlInvalidateVertexCache();
        _jsGcmFifoFinish( &_jsGcmState.fifo );
    }

#define SUBPIXEL_BITS 12
#define SUBPIXEL_ADJUST (0.5/(1<<SUBPIXEL_BITS))

    /* the affine transformation of x and y from normalized device coordinates to
    ** window coordinates.
    **
    ** notes:
    **   - 1:1 port of glViewport (see spec for details)
    **   - x, y, width, height are clamped to the surface limits
    **   - initially, x, y width and height are zero!!
    **   - (0,0) is in the lower left [yInverted == TRUE]
    **   - important: because there is no concept of a window it is the callers
    **     responsibility to set the viewport to non zero values
    **   - [Jun] Two new paramenters zNear and zFar with default values to make
    **     this function compatible with cellGcmSetViewport
    */
    void static inline _jsGcmFifoGlViewport( GLint x, GLint y, GLsizei width, GLsizei height, GLclampf zNear = 0.0f, GLclampf zFar = 1.0f )
    {
        jsGcmViewportState *vp = &_jsGcmState.state.viewport;
        jsGcmRenderTarget *rt = &_jsGcmState.renderTarget;
        GLint clipX0, clipX1, clipY0, clipY1;

        // syntax check (supposed to be a nop in release builds)
        JS_ASSERT(( width >= 0 ) && ( height >= 0 ) );
        JS_ASSERT(( width <= JSGCM_MAX_RT_DIMENSION ) &&
                  ( height <= JSGCM_MAX_RT_DIMENSION ) );

        // keep for yInverted handling/disable+enable
        vp->x = x;
        vp->y = y;
        vp->w = width;
        vp->h = height;

        // clamp to hw limits
        clipX0 = x;
        clipX1 = x + width;
        if ( rt->yInverted )
        {
            clipY0 = rt->gcmRenderTarget.height - ( y + height );
            clipY1 = rt->gcmRenderTarget.height - y;
        }
        else
        {
            clipY0 = y;
            clipY1 = y + height;
        }
        if ( clipX0 < 0 )
        {
            clipX0 = 0;
        }
        if ( clipY0 < 0 )
        {
            clipY0 = 0;
        }
        if ( clipX1 >= JSGCM_MAX_RT_DIMENSION )
        {
            clipX1 = JSGCM_MAX_RT_DIMENSION;
        }
        if ( clipY1 >= JSGCM_MAX_RT_DIMENSION )
        {
            clipY1 = JSGCM_MAX_RT_DIMENSION;
        }
        if (( clipX1 <= clipX0 ) || ( clipY1 <= clipY0 ) )
        {
            clipX0 = clipY0 = clipX1 = clipY1 = 0;
        }

        // update viewport info
        vp->xScale = width * 0.5f;
        vp->xCenter = ( GLfloat )( x + vp->xScale + JSGCM_SUBPIXEL_ADJUST );
        if ( rt->yInverted )
        {
            vp->yScale = height * -0.5f;
            vp->yCenter = ( GLfloat )( rt->gcmRenderTarget.height - JSGCM_VIEWPORT_EPSILON - y +  vp->yScale + JSGCM_SUBPIXEL_ADJUST );
        }
        else
        {
            vp->yScale = height * 0.5f;
            vp->yCenter = ( GLfloat )( y +  vp->yScale + JSGCM_SUBPIXEL_ADJUST );
        }

        // Clamp depth range to legal values
        zNear = JSGCM_CLAMPF_01( zNear );
        zFar  = JSGCM_CLAMPF_01( zFar );

        // compute viewport values for hw [no doubles, so we might loose a few lsb]
        vp->zScale  = ( GLfloat )( 0.5f * ( zFar - zNear ) );
        vp->zCenter = ( GLfloat )( 0.5f * ( zFar + zNear ) );

        // hw zNear/zFar clipper
        if ( zNear > zFar )
        {
            GLclampf tmp = zNear;
            zNear = zFar;
            zFar = tmp;
        }

        float scale[4] = {  vp->xScale,  vp->yScale,  vp->zScale, 0.0f };
        float offset[4] = {   vp->xCenter,  vp->yCenter,  vp->zCenter, 0.0f };

        GCM_FUNC( cellGcmSetViewport, clipX0, clipY0, clipX1 - clipX0,
                  clipY1 - clipY0, zNear, zFar, scale, offset );
    }

    /* When you call glDrawArrays, count sequential elements from each enabled
    ** array are used to construct a sequence of geometric primitives, beginning
    ** with the first element. The mode parameter specifies what kind of primitive
    ** to construct and how to use the array elements to construct the primitives.
    **
    ** note:
    **   - 1:1 port of glDrawArrays (see spec for details)
    **   - legal values for mode are JSGCM_POINTS, JSGCM_LINES, JSGCM_LINE_LOOP, JSGCM_LINE_STRIP,
    **     JSGCM_TRIANGLES, JSGCM_TRIANGLE_STRIP, JSGCM_TRIANGLE_FAN, JSGCM_QUADS, JSGCM_QUAD_STRIP
    **     and JSGCM_POLYGON
    **   - legal values for first, first+Count are [0, 0xfffff]
    **   - if vertex reusage exists, glDrawElements is preferred
    */
    void static inline _jsGcmFifoGlDrawArrays( jsGcmEnum mode, GLint first, GLsizei count )
    {
        uint8_t gcmMode = 0;
        switch ( mode )
        {
            case JSGCM_POINTS:
                gcmMode = CELL_GCM_PRIMITIVE_POINTS;
                break;
            case JSGCM_LINES:
                gcmMode = CELL_GCM_PRIMITIVE_LINES;
                break;
            case JSGCM_LINE_LOOP:
                gcmMode = CELL_GCM_PRIMITIVE_LINE_LOOP;
                break;
            case JSGCM_LINE_STRIP:
                gcmMode = CELL_GCM_PRIMITIVE_LINE_STRIP;
                break;
            case JSGCM_TRIANGLES:
                gcmMode = CELL_GCM_PRIMITIVE_TRIANGLES;
                break;
            case JSGCM_TRIANGLE_STRIP:
                gcmMode = CELL_GCM_PRIMITIVE_TRIANGLE_STRIP;
                break;
            case JSGCM_TRIANGLE_FAN:
                gcmMode = CELL_GCM_PRIMITIVE_TRIANGLE_FAN;
                break;
            case JSGCM_QUADS:
                gcmMode = CELL_GCM_PRIMITIVE_QUADS;
                break;
            case JSGCM_QUAD_STRIP:
                gcmMode = CELL_GCM_PRIMITIVE_QUAD_STRIP;
                break;
            case JSGCM_POLYGON:
                gcmMode = CELL_GCM_PRIMITIVE_POLYGON;
                break;
            default:
                JS_ASSERT( 0 ); // chip exception ahead!
                break;
        }
        JS_ASSERT( first >= 0 );
        JS_ASSERT( count >= 0 );
        JS_ASSERT( first <= 0xfffff );
        JS_ASSERT(( first + count ) <= 0xfffff ); // hw restriction
        JS_ASSERT( _jsGcmCheckConsistency() );

        if ( count )
        {
            GCM_FUNC_SAFE( cellGcmSetDrawArrays, gcmMode, first, count );
        }
    }

    /* enables or disables writing into the depth buffer.
    **
    ** notes:
    **   - 1:1 port of glDepthMask (see spec for details)
    **   - initially, depth-buffer writing is enabled
    */
    void static inline _jsGcmFifoGlDepthMask( GLboolean enabled )
    {
        // true can have multiple values - our hw is picky :(
        enabled = ( enabled != JSGCM_FALSE );
        GCM_FUNC( cellGcmSetDepthMask, enabled );
    }

    /* specifies whether the individual color components in the framebuffer
    ** can or cannot be written.
    **
    ** notes:
    **   - 1:1 port of glColorMask (see spec for details)
    **   - initially, all color components will be written
    */
    void static inline _jsGcmFifoGlColorMask( GLboolean r, GLboolean g, GLboolean b, GLboolean a )
    {
        jsGcmRenderTarget *rt = &_jsGcmState.renderTarget;
        GLuint hwColorMask;
        GLuint hwColorMaskMRT;

        // store arguments, so we can reload them when the surface format changes
        rt->writeR = r;
        rt->writeG = g;
        rt->writeB = b;
        rt->writeA = a;

        // colorFormat==NONE is implemented through the color writemask
        if ( rt->colorFormat == JSGCM_NONE )
        {
            r = g = b = a = JSGCM_FALSE;
        }

        // and for the color mask itself
        hwColorMask = ( r ? CELL_GCM_COLOR_MASK_R : 0 ) | ( g ? CELL_GCM_COLOR_MASK_G : 0 ) |
                      ( b ? CELL_GCM_COLOR_MASK_B : 0 ) | ( a ? CELL_GCM_COLOR_MASK_A : 0 );
        hwColorMaskMRT = ( r ? ( CELL_GCM_COLOR_MASK_MRT1_R | CELL_GCM_COLOR_MASK_MRT2_R | CELL_GCM_COLOR_MASK_MRT3_R ) : 0 ) |
                         ( g ? ( CELL_GCM_COLOR_MASK_MRT1_G | CELL_GCM_COLOR_MASK_MRT2_G | CELL_GCM_COLOR_MASK_MRT3_G ) : 0 ) |
                         ( b ? ( CELL_GCM_COLOR_MASK_MRT1_B | CELL_GCM_COLOR_MASK_MRT2_B | CELL_GCM_COLOR_MASK_MRT3_B ) : 0 ) |
                         ( a ? ( CELL_GCM_COLOR_MASK_MRT1_A | CELL_GCM_COLOR_MASK_MRT2_A | CELL_GCM_COLOR_MASK_MRT3_A ) : 0 );
        GCM_FUNC( cellGcmSetColorMask, hwColorMask );
    }

    void static inline _jsGcmFifoGlTransferDataVidToVid
    (
        GLuint dstVidId,   
        GLuint dstVidIdOffset,
        GLuint dstPitch,
        GLuint dstX,
        GLuint dstY,
        GLuint srcVidId, 
        GLuint srcVidIdOffset,
        GLuint srcPitch,
        GLuint srcX,
        GLuint srcY,
        GLuint width,            // size in pixel
        GLuint height,
        GLuint bytesPerPixel
    )
    {
        _jsGcmTransferDataVidToVid( dstVidId, dstVidIdOffset, dstPitch, dstX, dstY,
                                    srcVidId, srcVidIdOffset, srcPitch, srcX, srcY,
                                    width, height, bytesPerPixel );
    }

    void static inline _jsGcmFifoGlTransferDataHostToVid
    (
        GLuint dstVidId, 
        GLuint dstVidIdOffset,
        GLsizei dstPitch,
        GLuint srcVidId, 
        GLuint srcVidIdOffset,
        GLsizei srcPitch,
        GLsizei bytesPerRow,
        GLsizei rowCount
    )
    {
        _jsGcmTransferData( dstVidId, dstVidIdOffset, dstPitch,
                            srcVidId, srcVidIdOffset, srcPitch,
                            bytesPerRow, rowCount );
    }

    void _jsCPUSwizzle( char *swizzledTexture, const char*texture, GLuint width, GLuint height, GLuint depth, GLenum internalFormat );


#ifdef __cplusplus
}
#endif


#endif
