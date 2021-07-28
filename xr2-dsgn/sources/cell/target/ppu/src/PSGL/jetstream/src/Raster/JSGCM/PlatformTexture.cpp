/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 *                Copyright (C) 2009 Sony Computer Entertainment Inc.
 *                                               All Rights Reserved.
 */


/*
 * Platform-specific internal functions to manage GCM textures
 * 
 */

#include <PSGL/psgl.h>
#include <PSGL/Debug.h>
#include <PSGL/private.h>
#include <PSGL/Utils.h>
#include <PSGL/PerfCounters.h>
#include <JSGCM/JSGcmUtils.h>
#include <JSGCM/JSGcmFifoUtils.h>
#include <JSGCM/JSGcmDevice.h>
#include <JSGCM/JSGcmDriver.h>
#include <JSGCM/JSGcmSetRenderTarget.h>
#include <JSGCM/JSGcmTexImage.h>


// Calculate required size in bytes for given texture layout
GLuint _jsGetGcmTextureSize( jsGcmTextureLayout *layout )
{
    GLuint bytesNeeded = 0;
    GLuint faceAlign = layout->pitch ? 1 : 128;

    GLuint minWidth = 1;
    GLuint minHeight = 1;
    switch ( layout->internalFormat )
    {
        case GL_COMPRESSED_RGB_S3TC_DXT1_EXT:
        case GL_COMPRESSED_RGBA_S3TC_DXT1_EXT:
        case GL_COMPRESSED_RGBA_S3TC_DXT3_EXT:
        case GL_COMPRESSED_RGBA_S3TC_DXT5_EXT:
            minWidth = minHeight = 4;
            break;
        default:
            break;
    }
    for ( GLuint face = 0;face < layout->faces;++face )
    {
        GLuint width = layout->baseWidth;
        GLuint height = layout->baseHeight;
        GLuint depth = layout->baseDepth;
        for ( GLuint i = 0;i < layout->levels; ++i )
        {
            width = MAX( minWidth, width );
            height = MAX( minHeight, height );
            depth = MAX( 1U, depth );

            // correct height for DXT compression (4x4 blocks, padded to 4 if necessary)
            GLuint heightAdjusted = (minHeight==1) ? height : (height+3)/4;
            GLuint widthAdjusted = (minWidth==1) ? width : (width+3)/4;
            GLuint blockSizeAdjusted = (minHeight==1) ? 1 : 16;

            if ( !layout->pitch )
              bytesNeeded += blockSizeAdjusted * layout->pixelBits * widthAdjusted * heightAdjusted * depth / 8;
            else
              bytesNeeded += heightAdjusted * depth * layout->pitch;

            width >>= 1;
            height >>= 1;
            depth >>= 1;
        }

        bytesNeeded = _jsPad( bytesNeeded, faceAlign );
    }

    return bytesNeeded;
}

// Calculate byte offset for given texture layout, number of faces, and levels
GLuint _jsGetGcmImageOffset( jsGcmTextureLayout *layout, GLuint face, GLuint level )
{
    GLuint bytes = 0;
    GLuint faceAlign = layout->pitch ? 1 : 128;

    GLuint minWidth = 1;
    GLuint minHeight = 1;
    switch ( layout->internalFormat )
    {
        case GL_COMPRESSED_RGB_S3TC_DXT1_EXT:
        case GL_COMPRESSED_RGBA_S3TC_DXT1_EXT:
        case GL_COMPRESSED_RGBA_S3TC_DXT3_EXT:
        case GL_COMPRESSED_RGBA_S3TC_DXT5_EXT:
            minWidth = minHeight = 4;
            break;
        default:
            break;
    }

    for ( GLuint f = 0;f < layout->faces;++f )
    {
        GLuint width = layout->baseWidth;
        GLuint height = layout->baseHeight;
        GLuint depth = layout->baseDepth;
        for ( GLuint i = 0;i < layout->levels; ++i )
        {
            if (( level == i ) && ( face == f ) ) return bytes;

            width = MAX( minWidth, width );
            height = MAX( minHeight, height );
            depth = MAX( 1U, depth );

            // correct height for DXT compression (4x4 blocks, padded to 4 if necessary)
            GLuint heightAdjusted = (minHeight==1) ? height : (height+3)/4;
            GLuint widthAdjusted = (minWidth==1) ? width : (width+3)/4;
            GLuint blockSizeAdjusted = (minHeight==1) ? 1 : 16;

            if ( !layout->pitch ) 
              bytes += blockSizeAdjusted * layout->pixelBits * widthAdjusted * heightAdjusted * depth / 8;
            else 
              bytes += heightAdjusted * depth * layout->pitch;

            width >>= 1;
            height >>= 1;
            depth >>= 1;
        }

        bytes = _jsPad( bytes, faceAlign );
    }

    JS_ASSERT( 0 );
    return 0;
}
// Get size of a texture
int _jsPlatformTextureSize()
{
    return sizeof( jsGcmTexture );
}

// Calculate pitch for a texture
static GLuint _getTexturePitch( const jsTexture * texture )
{
    return _jsPad( _jsGetStorageSize( texture->images[0]->format, texture->images[0]->type, texture->images[0]->width, 1, 1 ), 64 ); // TransferVid2Vid needs 64byte pitch alignment
}

// Return maximum number of texture image units
int _jsPlatformTextureMaxUnits()
{
    return JSGCM_MAX_TEXIMAGE_COUNT;
}

void _jsPlatformCopyGPUTexture( jsTexture* texture );

// Inline function to detach a texture from a PBO
// If copyData is true, texture data is copied from GPU memory
static inline void _jsPlatformTextureDetachFromPBOLazy( jsTexture *texture, GLboolean copyData )
{
    jsGcmTexture * gcmTexture = ( jsGcmTexture * )texture->platformTexture;
    if ( gcmTexture->pbo != NULL )
    {
        if ( copyData ) _jsPlatformCopyGPUTexture( texture );
        _jsFreeBufferObject( gcmTexture->pbo );
        gcmTexture->pbo = NULL;
        gcmTexture->gpuAddressId = GMM_ERROR;
        gcmTexture->gpuAddressIdOffset = 0;
        gcmTexture->pool = _JSGCM_SURFACE_POOL_NONE;
        gcmTexture->gpuSize = 0;
    }
}

// Create a gcm texture by initializing memory to 0
void _jsPlatformCreateTexture( jsTexture* texture )
{
    jsGcmTexture *gcmTexture = ( jsGcmTexture * )texture->platformTexture;
    memset( gcmTexture, 0, sizeof( jsGcmTexture ) );
    gcmTexture->gpuAddressId = GMM_ERROR;
}


void _jsPlatformFreeGcmTexture( jsTexture* texture );

// Destroy a texture by freeing a gcm texture and an associated PBO
void _jsPlatformDestroyTexture( jsTexture* texture )
{
    if ( !texture->referenceBuffer )
    {
        jsGcmTexture *gcmTexture = ( jsGcmTexture * )texture->platformTexture;
        if ( gcmTexture->pbo != NULL )
        {
            _jsFreeBufferObject( gcmTexture->pbo );
            gcmTexture->pbo = NULL;
            gcmTexture->pool = _JSGCM_SURFACE_POOL_NONE;
            gcmTexture->gpuAddressId = GMM_ERROR;
            gcmTexture->gpuAddressIdOffset = 0;
            gcmTexture->gpuSize = 0;
        }

        _jsPlatformFreeGcmTexture( texture );
    }
    _jsTextureTouchFBOs( texture );
}

// Get size of texture in GPU layout
inline static GLuint _jsPlatformTextureGetGPUSize( const jsTexture* texture )
{
    jsGcmTexture *gcmTexture = ( jsGcmTexture * )texture->platformTexture;
    return _jsGetGcmTextureSize( &gcmTexture->gpuLayout );
}

// copy pixel functions. CPU swizzle needs that, should we rewrite it ?
//  NOTE: The CPU swizzle function treats 8/16-byte textures as wider 4-byte
//  textures.  For 8/16-byte textures, the 4-byte copy function should be
//  used.
typedef void( *CopyPixelFunction )( void *dst, const void *src );
static void _jsCopyPixel1( void *dst, const void *src ) {*( GLubyte * )dst = *( const GLubyte * )src;}
static void _jsCopyPixel2( void *dst, const void *src ) {*( GLushort * )dst = *( const GLushort * )src;}
static void _jsCopyPixel4( void *dst, const void *src ) {*( GLuint * )dst = *( const GLuint * )src;}

//  NOTE: The CPU swizzle function treats 8/16-byte textures as wider 4-byte
//  textures.  For 8/16-byte textures, the 4-byte copy function should be
//  used.
static inline CopyPixelFunction _jsChooseCopyPixel( GLuint bpp )
{
    switch ( bpp )
    {
        case 1:
            return _jsCopyPixel1;
        case 2:
            return _jsCopyPixel2;
        case 4:
        case 8:
        case 16:
            // CPU swizzle treats 8/16-byte textures as wide 4-byte textures
            return _jsCopyPixel4;
        default:
            return NULL;
    }
}

// Utility function to transfer a texture from GPU memory to the host
//  Data is copied image by image to the host, provided the data exists on
//  the GPU and newer data does not exist on the host.
void _jsPlatformCopyGPUTexture( jsTexture* texture )
{
    JS_ASSERT( texture != NULL );
    jsGcmTexture *gcmTexture = ( jsGcmTexture * )texture->platformTexture;

    if ( gcmTexture->gpuAddressId == GMM_ERROR )
        return;

    jsGcmTextureLayout *layout = &gcmTexture->gpuLayout;
    JS_ASSERT( gcmTexture->gpuSize >= _jsGetGcmTextureSize( layout ) );

    GLboolean syncNeeded = GL_TRUE;
    for ( GLuint face = 0; face < texture->faceCount; ++face )
    {
        for ( GLuint i = 0; i < layout->levels; ++i )
        {
            jsImage* image = &texture->images[face][i];
            if ( image->dataState == _JS_IMAGE_DATASTATE_GPU )
            {
                if (gcmTexture->pool != _JSGCM_SURFACE_POOL_SYSTEM)
                {
                    _JS_REPORT_EXTRA( PSGL_REPORT_TEXTURE_COPY_BACK, "Copying back texture %d from GPU", texture->name );
                }
                _jsImageAllocCPUStorage( image );
                JS_ASSERT( image->data != NULL );
                GLuint offset = _jsGetGcmImageOffset( layout, face, i );

                if ( syncNeeded )
                {
                    // must wait for render to texture, copy texture, upload
                    syncNeeded = GL_FALSE;
                    _JS_PERFORMANCE_START( TextureCopyBackStallCounter );
                    _jsGcmFifoGlFinish();
                    _JS_PERFORMANCE_STOP( TextureCopyBackStallCounter );
                }

                char* gpuData = gmmIdToAddress(gcmTexture->gpuAddressId) + gcmTexture->gpuAddressIdOffset + offset;

                if ( image->storageType != _JS_IMAGE_STORAGE_RASTER )
                {
                    // easiest case, both GPU and host are swizzled, just copy

                    // copy to image buffer
                    JS_ASSERT( offset + image->storageSize <= gcmTexture->gpuSize );
                    fast_memcpy( image->data, gpuData, image->storageSize );

                }
                else // host not swizzled
                {
                    JS_ASSERT( layout->pixelBits != 0 && ( layout->pixelBits % 8 ) == 0 );
                    const GLuint pixelBytes = layout->pixelBits / 8;

                    if ( !layout->pitch )
                    {
                        // deswizzle
                        JS_ASSERT( offset + image->storageSize <= gcmTexture->gpuSize );

                        CopyPixelFunction func = _jsChooseCopyPixel( pixelBytes );
                        _jsGcmDeswizzleDataCPU(
                            gpuData,
                            image->data,
                            0, 0, 0,										// swizzled position
                            image->width, image->height, image->depth,	// swizzled size
                            0, 0, 0,										// unswizzled position
                            image->width, image->height, image->depth,	// unswizzled size
                            image->width, image->height, image->depth,	// unswizzled extent
                            _jsLog2( pixelBytes ),
                            pixelBytes,
                            0,											// border
                            3,											// dim
                            func );			// copy function
                    }
                    else
                    {
                        // linear to linear
                        //  The catch with this is that all levels of a mipmap
                        //  have the same pitch in GPU memory.
                        jsRaster raster = {
                      format : image->format,
                      type   : image->type,
                      width  : image->width,
                      height : image->height,
                      depth  : image->depth,
                      xstride : image->xstride,
                      ystride : layout->pitch,
                      zstride : layout->pitch*image->height,
                      data    : gpuData
                                          };

                        _jsRasterToImage( &raster, image, 0, 0, 0 );

                    }
                } // host not swizzled

                // host now contains current data
                image->dataState = _JS_IMAGE_DATASTATE_HOST;
            }
        } // loop over mipmap levels
    } // loop over cube faces
}

// Drop a texture from the GPU memory by detaching it from a PBO
void _jsPlatformDropTexture( jsTexture *texture )
{
    JS_ASSERT( texture->referenceBuffer == 0 );
    jsGcmTexture *gcmTexture = ( jsGcmTexture * )texture->platformTexture;

    // pixel buffer object
    _jsPlatformTextureDetachFromPBOLazy( texture, GL_TRUE );

    if ( gcmTexture->pool != _JSGCM_SURFACE_POOL_NONE )
    {
        _jsPlatformCopyGPUTexture( texture );
        _jsPlatformFreeGcmTexture( texture );
    }

    gcmTexture->pool = _JSGCM_SURFACE_POOL_NONE;
    gcmTexture->gpuAddressId = GMM_ERROR;
    gcmTexture->gpuAddressIdOffset = 0;
    gcmTexture->gpuSize = 0;
    texture->revalidate |= _JS_TEXTURE_REVALIDATE_IMAGES;
    _jsTextureTouchFBOs( texture );
}

// Drop unbound textures from the GPU memory
// This is kind of slow, but we hit a slow path anyway.
//  If the pool argument is not _JSGCM_SURFACE_POOL_NONE, then only textures
//  in the specified pool will be dropped.
void _jsPlatformDropUnboundTextures( GLenum pool )
{
    PSGLcontext*	LContext = _CurrentContext;
    GLuint i, j;
    for ( i = 0;i < LContext->textureNameSpace.capacity;++i )
    {
        GLboolean bound = GL_FALSE;
        jsTexture *texture = ( jsTexture * )LContext->textureNameSpace.data[i];
        if ( !texture || ( texture->referenceBuffer != 0 ) ) continue;

        // check if bound
        for ( j = 0; j < _JS_MAX_VERTEX_TEXTURE_IMAGE_UNITS; ++j )
        {
            if ( LContext->VertexTextureImages[j] == texture )
            {
                bound = GL_TRUE;
                break;
            }
        }
        for ( j = 0;j < _JS_MAX_TEXTURE_IMAGE_UNITS;++j )
        {
            jsTextureImageUnit *tu = LContext->TextureImageUnits + j;
            if ( tu->bound2D == i || tu->bound3D == i || tu->boundCube == i )
            {
                bound = GL_TRUE;
                break;
            }
        }
        if ( bound )
            continue;

        // check if data shared with PBO
        //  XXX The actual test here is only that the data is shared.  It is
        //  likely that it is shared with a PBO, but it is possible that it
        //  is shared with another texture.  Fix this by testing against all
        //  named PBOs.
        jsGcmTexture *gcmTexture = ( jsGcmTexture * )texture->platformTexture;
        if ( gcmTexture->pbo != NULL && gcmTexture->pbo->refCount > 1 )
            continue;

        // check pool
        if ( pool != _JSGCM_SURFACE_POOL_NONE &&
                pool != gcmTexture->pool )
            continue;

        _jsPlatformDropTexture( texture );
    }
}

// Drop filitering mode for FP32 texture
static inline GLenum unFilter( GLenum filter )
{
    GLenum newFilter;
    switch ( filter )
    {
        case GL_NEAREST:
        case GL_LINEAR:
            newFilter = GL_NEAREST;
            break;
        case GL_NEAREST_MIPMAP_NEAREST:
        case GL_NEAREST_MIPMAP_LINEAR:
        case GL_LINEAR_MIPMAP_NEAREST:
        case GL_LINEAR_MIPMAP_LINEAR:
            newFilter = GL_NEAREST_MIPMAP_NEAREST;
            break;
        default:
            JS_ASSERT( 0 );
            newFilter = GL_NEAREST;
    }
#ifdef JS_REPORT
    if ( newFilter != filter ) _JS_REPORT_EXTRA( PSGL_REPORT_FP32_FILTERING, "Filtering mode %s not supported for FP32 textures, using %s instead", _jsGetGLEnumName( filter ), _jsGetGLEnumName( newFilter ) );
#endif
    return newFilter;
}

// Choose a texture layout and store it to newLayout, based on texture's filtering mode, swizzling, and size
void _jsPlatformChooseGPUFormatAndLayout(
    const jsTexture* texture,
    GLboolean forceLinear,
    GLuint pitch,
    jsGcmTextureLayout* newLayout )
{
    JS_ASSERT( _jsTextureIsValid( texture ) );

    jsImage *image = texture->images[0] + texture->baseLevel;

    GLuint levels = _jsLog2( MAX( MAX( image->width, image->height ), image->depth ) ) + 1;
    levels = MIN( levels, texture->maxLevel + 1 );

    // if we don't want mipmapping, but still have valid mipmaps, load all the mipmaps.
    // This is to avoid a big cost when switching from mipmaps to non-mipmaps.
    if (( texture->minFilter == GL_LINEAR ) || ( texture->minFilter == GL_NEAREST ) )
    {
        if (( levels > texture->imageCount ) ||
                ( !_jsTextureHasValidLevels( texture, levels, image->width, image->height,
                                             image->depth, image->format, image->type, image->internalFormat ) ) )
        {
            levels = 1;
        }
    }

    newLayout->levels = levels;
    newLayout->faces = texture->faceCount;
    newLayout->baseWidth = image->width;
    newLayout->baseHeight = image->height;
    newLayout->baseDepth = image->depth;
    newLayout->internalFormat = ( jsGcmEnum )image->internalFormat;
    newLayout->pixelBits = _jsPlatformGetBitsPerPixel( newLayout->internalFormat );
    newLayout->pitch = 0;
    GLboolean isHostSwizzled = ( image->storageType != _JS_IMAGE_STORAGE_RASTER );
    if ( !isHostSwizzled )
    {
        GLboolean swizzle = GL_TRUE;
        // figure out whether to swizzle
        if ( forceLinear )
            // requested because of tiled memory
            swizzle = GL_FALSE;
        else if ( texture->vertexEnable )
            // vertex texture
            swizzle = GL_FALSE;
        else if ( !_jsIsPow2( newLayout->baseWidth ) || !_jsIsPow2( newLayout->baseHeight ) )
            // not a POT
            swizzle = GL_FALSE;
        else if ( texture->isRenderTarget )
        {
            if ( newLayout->pixelBits > 32 )
                // can't render to swizzled semifat or fat format
                swizzle = GL_FALSE;
            else if ( newLayout->baseWidth < 16 || newLayout->baseHeight < 16 )
                swizzle = GL_FALSE;
        }

        if ( !swizzle ) newLayout->pitch = pitch ? pitch : _getTexturePitch( texture );
    }
    else
    {
        if (( !_jsIsPow2( newLayout->baseWidth ) ) || ( !_jsIsPow2( newLayout->baseHeight ) ) )
        {
            // NPOT DXTn
            const unsigned int numPixelsPerBlock = 16; // dxt block is 4x4
            unsigned int numBlocksPerRow = ( newLayout->baseWidth + 3 ) / 4;
            unsigned int numPixelsPerRow = numBlocksPerRow * numPixelsPerBlock;
            unsigned int numBitsPerRow = numPixelsPerRow * newLayout->pixelBits; // pixelBits is BitsPerDxtPixel
            unsigned int numBytesPerRow = numBitsPerRow / 8;
            newLayout->pitch = pitch ? pitch : numBytesPerRow;
        }
    }
}

// texture strategy actions
//  A texture strategy is a sequence of actions represented by these tokens.
//  _JS_TEXTURE_STRATEGY_END must be the last token in any strategy.
enum _jsTextureStrategy {
    _JS_TEXTURE_STRATEGY_END,			// allocation failed, give up
    _JS_TEXTURE_STRATEGY_FORCE_LINEAR,
    _JS_TEXTURE_STRATEGY_TILED_ALLOC,
    _JS_TEXTURE_STRATEGY_TILED_CLEAR,
    _JS_TEXTURE_STRATEGY_UNTILED_ALLOC,
    _JS_TEXTURE_STRATEGY_UNTILED_CLEAR,
    _JS_TEXTURE_STRATEGY_SYSTEM_ALLOC,
    _JS_TEXTURE_STRATEGY_SYSTEM_CLEAR,	// XXX probably not useful
};

static enum _jsTextureStrategy tiledGPUStrategy[] =
    {
        _JS_TEXTURE_STRATEGY_TILED_ALLOC,	// try tiled alloction
        _JS_TEXTURE_STRATEGY_FORCE_LINEAR,
        _JS_TEXTURE_STRATEGY_UNTILED_ALLOC,	// if failure, try linear allocation
        _JS_TEXTURE_STRATEGY_UNTILED_CLEAR,	// if failure, drop linear textures
        _JS_TEXTURE_STRATEGY_UNTILED_ALLOC,	// try linear again
        _JS_TEXTURE_STRATEGY_END,			// give up
    };
static enum _jsTextureStrategy linearGPUStrategy[] =
    {
        _JS_TEXTURE_STRATEGY_FORCE_LINEAR,
        _JS_TEXTURE_STRATEGY_UNTILED_ALLOC,
        _JS_TEXTURE_STRATEGY_UNTILED_CLEAR,
        _JS_TEXTURE_STRATEGY_UNTILED_ALLOC,
        _JS_TEXTURE_STRATEGY_END,
    };
static enum _jsTextureStrategy swizzledGPUStrategy[] =
    {
        _JS_TEXTURE_STRATEGY_UNTILED_ALLOC,
        _JS_TEXTURE_STRATEGY_UNTILED_CLEAR,
        _JS_TEXTURE_STRATEGY_UNTILED_ALLOC,
        _JS_TEXTURE_STRATEGY_END,
    };
static enum _jsTextureStrategy linearSystemStrategy[] =
    {
        _JS_TEXTURE_STRATEGY_FORCE_LINEAR,
        _JS_TEXTURE_STRATEGY_SYSTEM_ALLOC,
        _JS_TEXTURE_STRATEGY_END,
    };
static enum _jsTextureStrategy swizzledSystemStrategy[] =
    {
        _JS_TEXTURE_STRATEGY_SYSTEM_ALLOC,
        _JS_TEXTURE_STRATEGY_END,
    };

// Reallocate texture based on usage, pool system, and strategy
void _jsPlatformReallocateGcmTexture( jsTexture* texture )
{
    jsGcmTexture *gcmTexture = ( jsGcmTexture * )texture->platformTexture;

    // select the allocation strategy
    enum _jsTextureStrategy *step = NULL;
    switch ( texture->usage )
    {
        case GL_TEXTURE_TILED_GPU_SCE:
            step = tiledGPUStrategy;
            break;
        case GL_TEXTURE_LINEAR_GPU_SCE:
            step = linearGPUStrategy;
            break;
        case GL_TEXTURE_SWIZZLED_GPU_SCE:
            step = swizzledGPUStrategy;
            break;
        case GL_TEXTURE_LINEAR_SYSTEM_SCE:
            step = linearSystemStrategy;
            break;
        case GL_TEXTURE_SWIZZLED_SYSTEM_SCE:
            step = swizzledSystemStrategy;
            break;
        default:
            step = swizzledGPUStrategy;
            break;
    }
    GLuint xscale=1, yscale=1, antiAliasingMode=1;
    switch ( texture->aaUsage )
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
            break;
    }

    GLuint size = 0;
    GLuint id = GMM_ERROR;

    // allow swizzled format unless explicitly disallowed
    //  PBO textures cannot be swizzled.
    GLboolean forceLinear = gcmTexture->pbo != NULL ? GL_TRUE : GL_FALSE;

    const jsGcmTextureLayout currentLayout = gcmTexture->gpuLayout;
    const GLuint currentSize = gcmTexture->gpuSize;

    // process strategy
    GLboolean done = GL_FALSE;
    while ( !done )
    {
        jsGcmTextureLayout newLayout;

        GLenum pool = 0;
        switch ( *step++ )
        {
            case _JS_TEXTURE_STRATEGY_FORCE_LINEAR:
                forceLinear = GL_TRUE;
                break;
            case _JS_TEXTURE_STRATEGY_UNTILED_ALLOC:
                // get layout and size compatible with this pool
                _jsPlatformChooseGPUFormatAndLayout( texture, forceLinear, 0, &newLayout );
                size = _jsGetGcmTextureSize( &newLayout );

                // determine if current allocation already works
                //  If the current allocation has the right size and pool, we
                //  don't have to do anything.  If not, we only drop from the
                //  target pool because we may reuse the allocation from a
                //  different pool in a later step.
                if ( gcmTexture->pool == _JSGCM_SURFACE_POOL_LINEAR )
                {
                    if ( currentSize >= size && newLayout.pitch == currentLayout.pitch )
                    {
                        gcmTexture->gpuLayout = newLayout;
                        done = GL_TRUE;
                    }
                    else
                        _jsPlatformDropTexture( texture );
                }

                if ( !done )
                {
                    // allocate in the specified pool
                    id = _jsGcmAllocateTextureMemory( size );
                    if ( id != GMM_ERROR )
                    {
                        // drop old allocation
                        if ( gcmTexture->pool != _JSGCM_SURFACE_POOL_NONE )
                            _jsPlatformDropTexture( texture );

                        // set new
                        gcmTexture->pool = _JSGCM_SURFACE_POOL_LINEAR;
                        gcmTexture->gpuAddressId = id;
                        gcmTexture->gpuAddressIdOffset = 0;
                        gcmTexture->gpuSize = size;
                        gcmTexture->gpuLayout = newLayout;

                        done = GL_TRUE;
                    }
                }
                break;

            case _JS_TEXTURE_STRATEGY_TILED_ALLOC:
                // get layout and size compatible with this pool
                //  Tiled allocation is a little tricky because we don't know
                //  what pitch we'll get until we allocate.  We get the
                //  minimum size here and adjust it later.
                //
                //  The second argument to _jsPlatformChooseGPUFormatAndLayout
                //  forces unswizzled layout.
                _jsPlatformChooseGPUFormatAndLayout( texture, GL_TRUE, 0, &newLayout );
                size = _jsGetGcmTextureSize( &newLayout );

                // render target cube map limitation
                //  A render target origin in tiled memory must be a multiple of
                //  16 lines from the beginning of the buffer.  This restricts
                //  cube maps in tiled memory to be nonmipmapped and a multiple
                //  of 16 in size since we may have to render to any face.
                if ( texture->isRenderTarget && texture->target == GL_TEXTURE_CUBE_MAP )
                {
                    if ( texture->hasMipmaps )
                        // can't allocate tiled
                        continue;

                    if (( newLayout.baseHeight % 16 ) != 0 )
                        // can't allocate tiled
                        continue;
                }

                // get prospective location
                switch ( newLayout.internalFormat )
                {
                    case JSGCM_DEPTH_COMPONENT16:
                    case JSGCM_DEPTH_COMPONENT24:
                        pool = _JSGCM_SURFACE_POOL_TILED_DEPTH;
                        break;
                    default:
                        pool = _JSGCM_SURFACE_POOL_TILED_COLOR;
                        break;
                }

                // determine if current allocation already works
                //  If the current allocation has the right size and pool, we
                //  don't have to do anything.  If not, we only drop from the
                //  target pool because we may reuse the allocation from a
                //  different pool in a later step.
                if ( gcmTexture->pool == pool )
                {
                    if ( newLayout.pitch <= currentLayout.pitch &&
                            size / newLayout.pitch <= currentSize / currentLayout.pitch )
                    {
                        newLayout.pitch = currentLayout.pitch;
                        gcmTexture->gpuLayout = newLayout;
                        done = GL_TRUE;
                    }
                    else
                        _jsPlatformDropTexture( texture );
                }

                if ( !done )
                {
                    // compute effective width and height
                    const GLuint padX = 8 * newLayout.pitch / newLayout.pixelBits;
                    const GLuint padY = size / newLayout.pitch;
                    JS_ASSERT( padX >= newLayout.baseWidth );
                    JS_ASSERT( padY >= newLayout.baseHeight );

                    // allocate tiled memory
                    //  Don't allow non-drawable formats.
                    switch ( pool )
                    {
                        case _JSGCM_SURFACE_POOL_TILED_COLOR:
                            _jsGcmAllocateColorSurface(
                                GL_TRUE,    // create in local memory
                                padX / xscale, padY / yscale,
                                newLayout.pixelBits,
                                JSGCM_FALSE,		// scan out
                                antiAliasingMode,				// antialiasing
                                &id,
                                &newLayout.pitch,
                                &size );
                            break;
                        case _JSGCM_SURFACE_POOL_TILED_DEPTH:
                            _jsGcmAllocateDepthStencilSurface(
                                GL_TRUE,    // create in local memory
                                padX / xscale, padY / yscale,
                                newLayout.internalFormat,
                                antiAliasingMode,				// antialiasing
                                &id,
                                &newLayout.pitch,
                                &size );
                            break;
                        default:
                            // TODO: report attempt to put non-drawable texture in
                            // tiled memory
                            id = GMM_ERROR;
                            break;
                    }

                    if ( id != GMM_ERROR )
                    {
                        // drop old allocation
                        if ( gcmTexture->pool != _JSGCM_SURFACE_POOL_NONE )
                            _jsPlatformDropTexture( texture );

                        // set new
                        gcmTexture->pool = pool;
                        gcmTexture->gpuAddressId = id;
                        gcmTexture->gpuAddressIdOffset = 0;
                        gcmTexture->gpuSize = size;
                        gcmTexture->gpuLayout = newLayout;

                        done = GL_TRUE;
                    }
                }
                break;

            case _JS_TEXTURE_STRATEGY_SYSTEM_ALLOC:
                // get layout and size compatible with this pool
                _jsPlatformChooseGPUFormatAndLayout( texture, forceLinear, 0, &newLayout );
                size = _jsGetGcmTextureSize( &newLayout );

                // determine if current allocation already works
                //  If the current allocation has the right size and pool, we
                //  don't have to do anything.  If not, we only drop from the
                //  target pool because we may reuse the allocation from a
                //  different pool in a later step.
                if ( gcmTexture->pool == _JSGCM_SURFACE_POOL_SYSTEM )
                {
                    if ( currentSize >= size && newLayout.pitch == currentLayout.pitch )
                    {
                        gcmTexture->gpuLayout = newLayout;
                        done = GL_TRUE;
                    }
                    else
                        _jsPlatformDropTexture( texture );
                }

                if ( !done )
                {
                    // allocate in the specified pool
                    id = _jsGcmAllocateHostMemory( size );
                    if ( id != GMM_ERROR )
                    {
                        // drop old allocation
                        if ( gcmTexture->pool != _JSGCM_SURFACE_POOL_NONE )
                            _jsPlatformDropTexture( texture );

                        // set new
                        gcmTexture->pool = _JSGCM_SURFACE_POOL_SYSTEM;
                        gcmTexture->gpuAddressId = id;
                        gcmTexture->gpuAddressIdOffset = 0;
                        gcmTexture->gpuSize = size;
                        gcmTexture->gpuLayout = newLayout;

                        done = GL_TRUE;
                    }
                }
                break;

            case _JS_TEXTURE_STRATEGY_UNTILED_CLEAR:
                _jsPlatformDropUnboundTextures( _JSGCM_SURFACE_POOL_LINEAR );
                break;

            case _JS_TEXTURE_STRATEGY_TILED_CLEAR:
                _jsPlatformDropUnboundTextures( _JSGCM_SURFACE_POOL_TILED_COLOR );
                _jsPlatformDropUnboundTextures( _JSGCM_SURFACE_POOL_TILED_DEPTH );
                break;

            case _JS_TEXTURE_STRATEGY_END:
                _jsSetError( GL_OUT_OF_MEMORY );
                done = GL_TRUE;
                break;
            default:
                JS_ASSERT( 0 );
        }
    } // while loop for allocation strategy steps
    _jsTextureTouchFBOs( texture );
}

// Free memory pooled by a GCM texture
void _jsPlatformFreeGcmTexture( jsTexture* texture )
{
    jsGcmTexture *gcmTexture = ( jsGcmTexture * )texture->platformTexture;
    switch ( gcmTexture->pool )
    {
        case _JSGCM_SURFACE_POOL_LINEAR:
            _jsGcmFreeTextureMemory( gcmTexture->gpuAddressId );
            break;
        case _JSGCM_SURFACE_POOL_SYSTEM:
            _jsGcmFreeHostMemory( gcmTexture->gpuAddressId );
            break;
        case _JSGCM_SURFACE_POOL_TILED_COLOR:
            _jsGcmFreeTiledSurface( gcmTexture->gpuAddressId );
            break;
        case _JSGCM_SURFACE_POOL_TILED_DEPTH:
            _jsGcmFreeDepthStencilSurface( GL_TRUE, gcmTexture->gpuAddressId );
            break;
        case _JSGCM_SURFACE_POOL_NONE:
            break;
        default:
            JS_ASSERT( 0 );
    }

    gcmTexture->gpuAddressId = GMM_ERROR;
    gcmTexture->gpuAddressIdOffset = 0;
    gcmTexture->gpuSize = 0;
}

// memcpy for NPOT DXT mipmap transfer 
static void memcpy_with_pitch(void *dst, void *src, size_t width, size_t pitch, size_t n)
{
    char *d = (char*)dst;
    char *s = (char*)src;
    size_t bytesCopied = 0;

    JS_ASSERT( (n % width) == 0 );
    JS_ASSERT( n >= width );
    JS_ASSERT( width > 0 );

    if (width == pitch)
    {
        fast_memcpy(d, s, n);
    }
    else
    {
        while(1)
        {
            fast_memcpy(d, s, width);
            bytesCopied += width;
            d += pitch;
            s += width;

            if (bytesCopied >= n)
            {
                break;
            }
        }
    }
}

// Upload texure from host memory to GPU memory
void _jsPlatformUploadTexture( jsTexture* texture )
{
    _JS_PERFORMANCE_START( UploadTextureCounter );
    jsGcmTexture *gcmTexture = ( jsGcmTexture * )texture->platformTexture;
    jsGcmTextureLayout *layout = &gcmTexture->gpuLayout;

    const GLuint pixelBytes = layout->pixelBits / 8;

    GLboolean isDXT = GL_FALSE;
    switch ( layout->internalFormat )
    {
        case GL_COMPRESSED_RGB_S3TC_DXT1_EXT:
        case GL_COMPRESSED_RGBA_S3TC_DXT1_EXT:
        case GL_COMPRESSED_RGBA_S3TC_DXT3_EXT:
        case GL_COMPRESSED_RGBA_S3TC_DXT5_EXT:
            isDXT = GL_TRUE;
            break;
        default:
            break;
    }

    // host texture requires sync
    const GLboolean isHostTexture =
        ( gcmTexture->pool == _JSGCM_SURFACE_POOL_SYSTEM ) ?
        GL_TRUE : GL_FALSE;
    if ( isHostTexture )
        // TODO: less draconian syncing
        _jsGcmFifoGlFinish();

    // create surface descriptors for image transfer
    jsGcmSurface src = {
                       source:		_JSGCM_SURFACE_SOURCE_TEMPORARY,
                       width:		0,		// replaced per image
                       height:		0,		// replaced per image
                       bpp:		pixelBytes,
                       pitch:		0,		// replaced per image
                       samples:	1,
                       format:		layout->internalFormat,
                       flip:		GL_FALSE,
                       pool:		isHostTexture ?
                           _JSGCM_SURFACE_POOL_PPU :	// direct from image
                           _JSGCM_SURFACE_POOL_LINEAR,		// via bounce buffer
                       ppuData:     NULL,   // replaced per image
                       dataId:      GMM_ERROR,
                       dataIdOffset:0,
                       };

    jsGcmSurface dst = {
                       source:		_JSGCM_SURFACE_SOURCE_TEXTURE,
                       width:		0,		// replaced per image
                       height:		0,		// replaced per image
                       bpp:		pixelBytes,
                       pitch:		layout->pitch,
                       samples:	1,
                       format:		layout->internalFormat,
                       flip:		GL_FALSE,
                       pool:		gcmTexture->pool,
                       ppuData:     NULL,   // replaced per image
                       dataId:      GMM_ERROR,
                       dataIdOffset:0,
                       };

    // use a bounce buffer to transfer to GPU
    GLuint bounceBufferId = GMM_ERROR;
    GLboolean isPow2 = _jsIsPow2(layout->baseWidth) && _jsIsPow2(layout->baseHeight); 
    for ( GLuint face = 0;face < texture->faceCount;++face )
    {
        for ( GLuint i = 0;i < layout->levels;++i )
        {
            // check if upload is needed for this image
            jsImage *image = texture->images[face] + i;
            if ( image->dataState == _JS_IMAGE_DATASTATE_HOST )
            {
                // determine image offset from base address
                // TODO: compute all offsets at once for efficiency
                //  This is the offset in bytes for this face/image from the
                //  texture base address.
                const GLuint dataOffset = _jsGetGcmImageOffset( layout, face, i );

                // used by NPOT DXT
                const unsigned int numPixelsPerBlock = 16;
                unsigned int numBlocksPerRow = (image->width + 3) / 4;
                unsigned int numPixelsPerRow = numPixelsPerBlock * numBlocksPerRow;
                unsigned int numBytesPerRow = numPixelsPerRow * layout->pixelBits / 8;
                unsigned int heightAdjusted = (image->height + 3) / 4;

                // set source pixel buffer
                src.ppuData = image->data;

                // lazy allocation of bounce buffer
                if ( bounceBufferId == GMM_ERROR && !isHostTexture && layout->baseDepth == 1 )
                {
                    _jsAllocSetHint( _JS_ALLOCTYPE_TEXTURE_UPLOAD, texture->name );
                    bounceBufferId = _jsGcmAllocateMemory( gcmTexture->gpuSize );
                    _jsAllocClearHint();
#ifndef JS_NO_ERROR_CHECK
                    if ( bounceBufferId == GMM_ERROR )
                    {
                        _jsSetError( GL_OUT_OF_MEMORY );
                        return;
                    }
#endif
                }

                if ( bounceBufferId != GMM_ERROR )
                {
                    // copy image to bounce buffer
                    src.dataId = bounceBufferId;
                    src.dataIdOffset = dataOffset;

                    // NPOT DXT
                    if (!isPow2 && isDXT)
                    {
                        memcpy_with_pitch( gmmIdToAddress( src.dataId ) + dataOffset,
                                           image->data, 
                                           numBytesPerRow, 
                                           layout->pitch,
                                           image->storageSize );
                    }
                    else
                    {
                        fast_memcpy( gmmIdToAddress( src.dataId ) + dataOffset, 
                                     image->data, 
                                     image->storageSize );
                    }
                }

                if ( isDXT )
                {
                    if ( isHostTexture )
                    {
                        if (isPow2)
                        {
                            fast_memcpy( gmmIdToAddress(gcmTexture->gpuAddressId) + gcmTexture->gpuAddressIdOffset + dataOffset, 
                                         src.ppuData, image->storageSize );
                        }
                        else
                        {
                            memcpy_with_pitch( gmmIdToAddress(gcmTexture->gpuAddressId) + gcmTexture->gpuAddressIdOffset + dataOffset,
                                               src.ppuData, 
                                               numBytesPerRow,
                                               layout->pitch,
                                               image->storageSize );
                        }
                    }
                    else
                    {
                        GLuint sizeAdjusted = (isPow2) ? image->storageSize : (heightAdjusted * layout->pitch);

                        _jsGcmCopyBounceToGPU(
                            gcmTexture->gpuAddressId, 
                            gcmTexture->gpuAddressIdOffset + dataOffset, 
                            layout->pitch,
                            src.dataId,
                            src.dataIdOffset,
                            sizeAdjusted );
                    }
                }
                else if ( layout->baseDepth != 1 )
                {
                    // 3D
                    // TODO: NPOT 3D textures
                    //  No hardware swizzle for 3D.
                    _jsGcmFifoGlFinish();
                    _jsGcmSwizzleDataCPU(
                        gmmIdToAddress( gcmTexture->gpuAddressId ) + gcmTexture->gpuAddressIdOffset + dataOffset,	// dst
                        image->data,	// src
                        0, 0, 0,			// dst position
                        image->width,	// dst size
                        image->height,
                        image->depth,
                        0, 0, 0,			// src subset min and max (x,y,z)
                        image->width,
                        image->height,
                        image->depth,
                        image->width,	// src size
                        image->height,
                        image->depth,
                        _jsLog2( pixelBytes ),	// log of depth
                        pixelBytes,				// depth
                        0,						// border
                        3,						// dimension (ignored if no border)
                        NULL );					// texel copy function pointer
                }
                else
                {
                    // use surface copy functions
                    src.width = image->width;
                    src.height = image->height;
                    src.pitch = pixelBytes * src.width;

                    dst.width = src.width;
                    dst.height = image->height;
                    dst.dataId = gcmTexture->gpuAddressId;
                    dst.dataIdOffset = gcmTexture->gpuAddressIdOffset + dataOffset;

                    GLuint offsetHeight = 0;
                    if ( dst.pitch )
                    {
                        // linear (not swizzled)
                        //  The tiled linear format requires that render
                        //  targets be aligned to 8*pitch from the start of
                        //  the tiled region.
                        offsetHeight = ( dataOffset / dst.pitch ) % 8;
                        dst.height += offsetHeight;
                        dst.dataIdOffset -= offsetHeight * dst.pitch;
                    }

                    _jsGcmCopySurface(
                        &src, 0, 0,
                        &dst, 0, offsetHeight,
                        src.width, src.height,
                        GL_TRUE );	// don't bypass GPU pipeline
                }

                // free CPU copy of data
                _jsImageFreeCPUStorage( image );
                image->dataState |= _JS_IMAGE_DATASTATE_GPU;
            } // newer data on host
        } // loop over levels
    } // loop over faces

    if ( bounceBufferId != GMM_ERROR )
        _jsGcmFreeMemory( bounceBufferId );

    _jsGcmFifoGlInvalidateTextureCache();


    _JS_PERFORMANCE_STOP( UploadTextureCounter );
}

// map PSGL internal types to GCM
static inline void _jsGcmUpdateGcmTexture( jsTexture * texture, jsGcmTextureLayout * layout, jsGcmTexture * platformTexture )
{
    // use color format for depth with no compare mode
    //  This hack is needed because the hardware will not read depth
    //  textures without performing a compare.  The depth value will need to
    //  be reconstructed in the shader from the color components.
    GLuint internalFormat = layout->internalFormat;
    if ( internalFormat == JSGCM_DEPTH_COMPONENT24 && 
         texture->compareMode == GL_NONE )
        internalFormat = JSGCM_RGBA8;

    if ( internalFormat == JSGCM_DEPTH_COMPONENT16 && 
         texture->compareMode == GL_NONE )
        internalFormat = JSGCM_RGB565_SCE;

    // set the format and remap( control 1)
    _jsGcmMapTextureFormat( internalFormat,
                            platformTexture->gcmTexture.format, platformTexture->gcmTexture.remap );

    // This is just to cover the conversion from swizzled to linear
    if ( layout->pitch )
    {
        platformTexture->gcmTexture.format += 0x20; // see class doc definitions for SZ_NR vs LN_NR...
    }

    // set the Control1 method params.
    // little bit of safe gaurding
    JS_ASSERT( layout->baseDepth >= 1 );
    JS_ASSERT(( layout->pitch & 7 ) == 0 ); // too strict alignment?


    platformTexture->gcmTexture.width = layout->baseWidth;
    platformTexture->gcmTexture.height = layout->baseHeight;
    platformTexture->gcmTexture.depth = layout->baseDepth;
    platformTexture->gcmTexture.pitch = layout->pitch;

    // mipmap parameter check
    JS_ASSERT(( layout->levels >= 1 ) && ( layout->levels <= JSGCM_MAX_LOD_COUNT ) );
    platformTexture->gcmTexture.mipmap = layout->levels;

    // set the dimention and cubmap settings
    // default is false
    platformTexture->gcmTexture.cubemap = CELL_GCM_FALSE;

    // set dimension, swizzled implies P2 width/height/depth
    switch ( texture->target )
    {
        case 0:
            // when disabling the unit, arbitrary hwFormat
            platformTexture->gcmTexture.dimension = CELL_GCM_TEXTURE_DIMENSION_2;
            break;
        case JSGCM_TEXTURE_1D:
            platformTexture->gcmTexture.dimension = CELL_GCM_TEXTURE_DIMENSION_1;
            break;
        case JSGCM_TEXTURE_2D:
            platformTexture->gcmTexture.dimension = CELL_GCM_TEXTURE_DIMENSION_2;
            break;
        case JSGCM_TEXTURE_3D:
            platformTexture->gcmTexture.dimension = CELL_GCM_TEXTURE_DIMENSION_3;
            break;
        case JSGCM_TEXTURE_CUBE_MAP:
            JS_ASSERT( platformTexture->gcmTexture.width == platformTexture->gcmTexture.height );
            JS_ASSERT( platformTexture->gcmTexture.depth == 1 );
            platformTexture->gcmTexture.dimension = CELL_GCM_TEXTURE_DIMENSION_2;
            platformTexture->gcmTexture.cubemap = CELL_GCM_TRUE;
            break;
        default:
            JS_ASSERT( 0 );
            break;
    }

    // system or local texture
    if ( gmmIdIsMain(platformTexture->gpuAddressId) )
        platformTexture->gcmTexture.location = CELL_GCM_LOCATION_MAIN;
    else
        platformTexture->gcmTexture.location = CELL_GCM_LOCATION_LOCAL;

}

// map PSGL internal types to GCM
void _jsGcmUpdateMethods( jsTexture * texture )
{

    jsGcmTexture *platformTexture = ( jsGcmTexture * )texture->platformTexture;
    jsGcmTextureLayout *layout = &platformTexture->gpuLayout;

    // max aniso
    // revalidate the texture registers cache.
    int maxAniso = ( int )texture->maxAnisotropy;
    GLuint minFilter = texture->minFilter;
    GLuint magFilter = texture->magFilter;

    // XXX make sure that REVALIDATE_PARAMETERS is set if the format of the texture changes
    // revalidate the texture registers cache just to ensure we are in the correct filtering mode
    // based on the internal format.
    switch ( layout->internalFormat )
    {
        case JSGCM_FLOAT_RGBA32:
        case JSGCM_FLOAT_RGBX32:
        case JSGCM_LUMINANCE32F_ARB:
        case JSGCM_FLOAT_R32:
            // float 32 doesn't support filtering
            minFilter = unFilter( minFilter );
            magFilter = unFilter( magFilter );
#ifdef JS_REPORT
            if ( maxAniso != 1 ) _JS_REPORT_EXTRA( PSGL_REPORT_FP32_FILTERING, "Anisotropic filtering is not supported for FP32 texture %d, using MAX_ANISOTROPY=1 instead", texture->name );
#endif
            maxAniso = 1;
            break;
        default:
            break;
    }

    // -----------------------------------------------------------------------
    // map the SET_TEXTURE_FILTER method.
    // set the min
    platformTexture->gcmMethods.filter.min = _jsGcmMapMinTextureFilter( minFilter );
    // set the mag
    platformTexture->gcmMethods.filter.mag = _jsGcmMapMagTextureFilter( magFilter );
    // set the QuinConx by default
    platformTexture->gcmMethods.filter.conv = CELL_GCM_TEXTURE_CONVOLUTION_QUINCUNX;
    // We don't actually expose this, but still need to set it up properly incase we expose this later
    // hw expects a 5.8 twos-complement fixed-point // XXX  what is the - .26f ?
    platformTexture->gcmMethods.filter.bias = ( GLint )(( texture->lodBias - .26f ) * 256.0f );

    // -----------------------------------------------------------------------
    // set the SET_TEXTURE_CONTROL0 params
    platformTexture->gcmMethods.control0.maxAniso = _jsGcmMapAniso( maxAniso );
    const GLfloat minLOD = MAX( texture->minLod, texture->baseLevel );
    const GLfloat maxLOD = MIN( texture->maxLod, texture->maxLevel );
    platformTexture->gcmMethods.control0.minLOD = ( GLuint )( MAX( minLOD, 0 ) * 256.0f );
    platformTexture->gcmMethods.control0.maxLOD = ( GLuint )( MIN( maxLOD, layout->levels ) * 256.0f );

    // -----------------------------------------------------------------------
    // set the SET_TEXTURE_ADDRESS method params.
    platformTexture->gcmMethods.address.wrapS = _jsGcmMapWrapMode( texture->wrapS );
    platformTexture->gcmMethods.address.wrapT = _jsGcmMapWrapMode( texture->wrapT );
    platformTexture->gcmMethods.address.wrapR = _jsGcmMapWrapMode( texture->wrapR );
    platformTexture->gcmMethods.address.zfunc = _jsGcmMapTexZFunMode( texture->compareFunc );
    // currently we don't have use but for normal remap so setting this by default
    platformTexture->gcmMethods.address.unsignedRemap = CELL_GCM_TEXTURE_UNSIGNED_REMAP_NORMAL;

    // now for gamma remap
    GLuint gamma = 0;
    GLuint remap = texture->gammaRemap;
    gamma |= ( remap & JSGCM_GAMMA_REMAP_RED_BIT ) 		? CELL_GCM_TEXTURE_GAMMA_R : 0;
    gamma |= ( remap & JSGCM_GAMMA_REMAP_GREEN_BIT )	? CELL_GCM_TEXTURE_GAMMA_G : 0;
    gamma |= ( remap & JSGCM_GAMMA_REMAP_BLUE_BIT ) 	? CELL_GCM_TEXTURE_GAMMA_B : 0;
    gamma |= ( remap & JSGCM_GAMMA_REMAP_ALPHA_BIT ) 	? CELL_GCM_TEXTURE_GAMMA_A : 0;

    platformTexture->gcmMethods.address.gamma = gamma;

    // set border colors
	JSGCM_CALC_COLOR_LE_ARGB8(&(platformTexture->gcmMethods.borderColor), 
        texture->borderColor.R, texture->borderColor.G, texture->borderColor.B, texture->borderColor.A); 

    // -----------------------------------------------------------------------
    // setup the GcmTexture
    // format, control1, control3, imagerect; setup for cellGcmSetTexture later
    _jsGcmUpdateGcmTexture( texture, layout, platformTexture );

}

// Validate texture resources
void _jsPlatformValidateTextureResources( jsTexture *texture )
{
    if ( JS_UNLIKELY( !_jsTextureIsValid( texture ) ) )
    {
        texture->isComplete = GL_FALSE;
        return;
    }
    texture->isComplete = GL_TRUE;

    //  We may need to reallocate the texture when the parameters are changed
    //  from non-mipmap to mipmap filtering, even though the images have not
    //  changed.
    //
    //  NOTE: If we ever support accessing mipmaps from a PBO, this code
    //  must be changed.  As it is, if the texture has a shared PBO and the
    //  mipmap flag then the slow path (copy back to host) is invoked.
    if ( texture->revalidate & _JS_TEXTURE_REVALIDATE_IMAGES || texture->revalidate & _JS_TEXTURE_REVALIDATE_LAYOUT )
    {
        JS_ASSERT( texture->referenceBuffer == 0 );
        // upload images
#ifdef JS_ALLOCINFO
        if (( texture->isRenderTarget ) && !( texture->name > 0 ) )
        { // A renderbuffer is implemented as a nameless texture. We have to find its name in the renderbuffer namespace.
            GLuint bufferName = 1;
            jsTexture *buffer = NULL;
            do { buffer = _jsGetRenderbufferSafe( _CurrentContext, bufferName++ ); }
            while (( buffer != NULL ) && ( buffer != texture ) );
            if ( buffer != NULL ) _jsAllocSetHint( _JS_ALLOCTYPE_RENDERBUFFER, --bufferName );
        else { _jsAllocSetHint( _JS_ALLOCTYPE_UNKNOWN, 0 ); }
        }
        else
        {
            _jsAllocSetHint( _JS_ALLOCTYPE_TEXTURE, texture->name );
        }
#endif
        _jsPlatformReallocateGcmTexture( texture );
        _jsAllocClearHint();
        _jsPlatformUploadTexture( texture );
    }

    // gcmTexture method command
    _jsGcmUpdateMethods( texture );

    texture->revalidate = 0;
}

// [YLIN] We are going to use gcm macro directly!
#include <cell/gcm/gcm_method_data.h>

// Set a texture to a gcm texture unit
static inline void _jsGcmSetTextureUnit( GLuint unit, jsGcmTexture *platformTexture )
{
    const GLuint imageOffset = gmmIdToOffset(platformTexture->gpuAddressId) + platformTexture->gpuAddressIdOffset;
    JS_ASSERT(( imageOffset & 0x7f ) == 0 || platformTexture->gpuLayout.pitch );
    platformTexture->gcmTexture.offset = imageOffset; 

    // set up the texture unit with the info for the current texture
    // bind texture , control 1,3,format and remap
	// [YLIN] Contiguous GCM calls in fact will cause LHSs between them
	//  There is no walkaround for this but not to use GCM functions

	GCM_FUNC_SAFE( cellGcmSetTexture, unit, &platformTexture->gcmTexture );
	CellGcmContextData *gcm_context = (CellGcmContextData*)&_jsGcmState.fifo;
	cellGcmReserveMethodSizeInline(gcm_context, 11);
	uint32_t *current = gcm_context->current;
	current[0] = CELL_GCM_METHOD_HEADER_TEXTURE_OFFSET(unit, 8);
	current[1] = CELL_GCM_METHOD_DATA_TEXTURE_OFFSET(platformTexture->gcmTexture.offset);
	current[2] = CELL_GCM_METHOD_DATA_TEXTURE_FORMAT(platformTexture->gcmTexture.location,
		platformTexture->gcmTexture.cubemap, 
		platformTexture->gcmTexture.dimension,
		platformTexture->gcmTexture.format,
		platformTexture->gcmTexture.mipmap);
	current[3] = CELL_GCM_METHOD_DATA_TEXTURE_ADDRESS( platformTexture->gcmMethods.address.wrapS,
		platformTexture->gcmMethods.address.wrapT,
		platformTexture->gcmMethods.address.wrapR,
		platformTexture->gcmMethods.address.unsignedRemap,
		platformTexture->gcmMethods.address.zfunc,
		platformTexture->gcmMethods.address.gamma,
		0);
	current[4] = CELL_GCM_METHOD_DATA_TEXTURE_CONTROL0(CELL_GCM_TRUE,
		platformTexture->gcmMethods.control0.minLOD,
		platformTexture->gcmMethods.control0.maxLOD,
		platformTexture->gcmMethods.control0.maxAniso);
	current[5] = platformTexture->gcmTexture.remap;
	current[6] = CELL_GCM_METHOD_DATA_TEXTURE_FILTER(
		(platformTexture->gcmMethods.filter.bias & 0x1fff),
		platformTexture->gcmMethods.filter.min,
		platformTexture->gcmMethods.filter.mag,
		platformTexture->gcmMethods.filter.conv);
	current[7] = CELL_GCM_METHOD_DATA_TEXTURE_IMAGE_RECT(
		platformTexture->gcmTexture.height,
		platformTexture->gcmTexture.width);
	current[8] = CELL_GCM_METHOD_DATA_TEXTURE_BORDER_COLOR(
		platformTexture->gcmMethods.borderColor);
	current[9] = CELL_GCM_METHOD_HEADER_TEXTURE_CONTROL3(unit,1);
	current[10] = CELL_GCM_METHOD_DATA_TEXTURE_CONTROL3(
		platformTexture->gcmTexture.pitch,
		platformTexture->gcmTexture.depth);
	gcm_context->current = &current[11];


}

// Validate incomplete texture by remapping
inline static void _jsPlatformValidateIncompleteTexture( GLuint unit )
{

    GLuint remap = CELL_GCM_REMAP_MODE(
                       CELL_GCM_TEXTURE_REMAP_ORDER_XYXY,
                       CELL_GCM_TEXTURE_REMAP_FROM_A,
                       CELL_GCM_TEXTURE_REMAP_FROM_R,
                       CELL_GCM_TEXTURE_REMAP_FROM_G,
                       CELL_GCM_TEXTURE_REMAP_FROM_B,
                       CELL_GCM_TEXTURE_REMAP_ONE,
                       CELL_GCM_TEXTURE_REMAP_ZERO,
                       CELL_GCM_TEXTURE_REMAP_ZERO,
                       CELL_GCM_TEXTURE_REMAP_ZERO );

    // disable control 0
    GCM_FUNC( cellGcmSetTextureControl, unit, CELL_GCM_FALSE, 0, 0, 0 );
    // set texture remap only
    GCM_FUNC( cellGcmSetTextureRemap, unit, remap );

}

#undef JSGCM_REMAP_MODES

// Validate vertex texture
void _jsPlatformValidateVertexTextures()
{
    PSGLcontext *LContext = _CurrentContext;
    // XXX need to only validate the textures of the current program
    for ( int unit = 0; unit < _JS_MAX_VERTEX_TEXTURE_IMAGE_UNITS; ++unit )
    {
        jsTexture *texture = LContext->VertexTextureImages[unit];
        if ( texture )
            if ( JS_UNLIKELY( texture->revalidate ) )
                _jsPlatformValidateTextureResources( texture );

        // texture->isComplete is updated on resource validation (by _jsPlatformValidateTextureResources).
        if ( texture && texture->vertexEnable && texture->isComplete )
        {

            jsGcmTexture *gcmTexture = ( jsGcmTexture * )texture->platformTexture;
            JS_ASSERT( gcmTexture->gpuAddressId != GMM_ERROR );
            jsGcmTextureLayout *layout = &gcmTexture->gpuLayout;
            JS_ASSERT( layout->internalFormat );
            _jsVertexTexImage2D( unit, layout->levels,
                                 layout->baseWidth, layout->baseHeight,
                                 layout->internalFormat, layout->pitch,
                                 gmmIdIsMain(gcmTexture->gpuAddressId),
                                 gmmIdToOffset(gcmTexture->gpuAddressId) + gcmTexture->gpuAddressIdOffset );
            _jsVertexTexParameters( unit, GL_TRUE, texture->wrapS, texture->wrapT, texture->lodBias, 0, layout->levels );
        }
        else
        {
            _jsVertexTexParameters( unit, GL_FALSE, GL_REPEAT, GL_REPEAT, 0.f, 0.f, 1.f );
        }
    }
}

#ifdef JS_DPM
int __jsNumTexturesValidated;
#endif

// Valiate resources of a texture and set it to a unit
void _jsPlatformValidateTextureStage( int unit, jsTexture* texture )
{
    JS_ASSERT( texture );

    PSGLcontext *LContext = _CurrentContext;
    JS_ASSERT( LContext );

#ifdef JS_DPM
    __jsNumTexturesValidated++;
#endif

    if ( JS_UNLIKELY( texture->revalidate ) )
    {
        // this updates the isComplete bit.
        _jsPlatformValidateTextureResources( texture );
    }

    GLboolean isCompleteCache = texture->isComplete;
#ifdef JS_DEBUG
    // note that IsValid updates isComplete, so use a temp to verify the correctness of the cache.
    JS_ASSERT( isCompleteCache == _jsTextureIsValid( texture ) );
#endif

    if ( JS_LIKELY( isCompleteCache ) )
    {
        jsGcmTexture *platformTexture = ( jsGcmTexture * )texture->platformTexture;
       _jsGcmSetTextureUnit( unit, platformTexture );
    }
    else
    {
        _JS_REPORT_EXTRA( PSGL_REPORT_TEXTURE_INCOMPLETE, "Texture %d bound to unit %d(%s) is incomplete.", texture->name, unit, _jsGetGLEnumName( texture->target ) );
        _jsPlatformValidateIncompleteTexture( unit );
        // Note that this is only for the fixed function, but is on a slow path, so its not a big deal.
        LContext->needValidate |= PSGL_VALIDATE_FFX_FRAGMENT_PROGRAM;
    }


}

// Choose internal format closest to given format
GLenum _jsPlatformChooseInternalFormat( GLenum internalFormat )
{
    switch ( internalFormat )
    {
        case GL_ALPHA12:
        case GL_ALPHA16:
            return JSGCM_ALPHA16;
        case GL_ALPHA:
        case GL_ALPHA4:
        case GL_ALPHA8:
            return JSGCM_ALPHA8;
        case GL_INTENSITY12:
        case GL_INTENSITY16:
            return JSGCM_INTENSITY16;
        case GL_INTENSITY:
        case GL_INTENSITY4:
        case GL_INTENSITY8:
            return JSGCM_INTENSITY8;
        case GL_LUMINANCE12:
        case GL_LUMINANCE16:
            return JSGCM_LUMINANCE16;
        case 1:
        case GL_LUMINANCE:
        case GL_LUMINANCE4:
        case GL_LUMINANCE8:
            return JSGCM_LUMINANCE8;
        case GL_LUMINANCE12_ALPHA4:
        case GL_LUMINANCE12_ALPHA12:
        case GL_LUMINANCE16_ALPHA16:
            return JSGCM_LUMINANCE16_ALPHA16;
        case 2:
        case GL_ALPHA_LUMINANCE_SCE:
        case GL_LUMINANCE_ALPHA:
        case GL_LUMINANCE8_ALPHA8:
        case GL_LUMINANCE4_ALPHA4:
        case GL_LUMINANCE6_ALPHA2:
            return JSGCM_LUMINANCE8_ALPHA8;
        case GL_RGB10:
        case GL_RGB10_A2:
        case GL_RGB12:
        case GL_RGB16:
            return JSGCM_FLOAT_RGBX32;
        case GL_RGBA12:
        case GL_RGBA16:
            return JSGCM_FLOAT_RGBA32;
        case 3:
        case GL_R3_G3_B2:
        case GL_RGB4:
        case GL_RGB:
        case GL_RGB8:
        case JSGCM_RGBX8:
            return JSGCM_RGBX8;
        case 4:
        case GL_RGBA2:
        case GL_RGBA4:
        case GL_RGBA8:
        case GL_RGBA:
//        case JSGCM_RGBA8:  // (this should be added for completeness, but the enum value matches GL_RGBA8 causing a duplicate-case error)
            return JSGCM_RGBA8;
        case GL_RGB5_A1:
            return JSGCM_RGB5_A1_SCE;
        case GL_RGB5:
            return JSGCM_RGB565_SCE;
        case GL_BGRA:
        case JSGCM_BGRA8:
            return JSGCM_BGRA8;
        case GL_ARGB_SCE:
            return JSGCM_ARGB8;
        case GL_ALPHA16F_ARB:
            return JSGCM_FLOAT_RGBA16;
        case GL_LUMINANCE16F_ARB:
            return JSGCM_FLOAT_RGBX16;
        case GL_LUMINANCE_ALPHA16F_ARB:
            return JSGCM_ALPHA_LUMINANCE16F_SCE;
        case GL_RGB16F_ARB:
        case JSGCM_FLOAT_RGBX16:
            return JSGCM_FLOAT_RGBX16;
        case GL_RGBA16F_ARB:
        case JSGCM_FLOAT_RGBA16:
            return JSGCM_FLOAT_RGBA16;
        case GL_ALPHA32F_ARB:
            return JSGCM_FLOAT_RGBA32;
        case GL_LUMINANCE32F_ARB:
            return JSGCM_LUMINANCE32F_ARB;
        case GL_LUMINANCE_ALPHA32F_ARB:
            return JSGCM_FLOAT_RGBA32;
        case GL_RGB32F_ARB:
        case JSGCM_FLOAT_RGBX32:
            return JSGCM_FLOAT_RGBX32;
        case GL_RGBA32F_ARB:
        case JSGCM_FLOAT_RGBA32:
            return JSGCM_FLOAT_RGBA32;
        case GL_DEPTH_COMPONENT16:
            return JSGCM_DEPTH_COMPONENT16;
        case GL_DEPTH_COMPONENT:
        case GL_DEPTH_COMPONENT24:
        case GL_DEPTH_COMPONENT32:
        case GL_STENCIL_INDEX:
            return JSGCM_DEPTH_COMPONENT24;
        case GL_COMPRESSED_RGB_S3TC_DXT1_EXT:
            return internalFormat;
        case GL_COMPRESSED_RGBA_S3TC_DXT1_EXT:
            return internalFormat;
        case GL_COMPRESSED_RGBA_S3TC_DXT3_EXT:
            return internalFormat;
        case GL_COMPRESSED_RGBA_S3TC_DXT5_EXT:
            return internalFormat;
        default:
            return GL_INVALID_ENUM;
    }
    return GL_INVALID_ENUM;
}

// Expand internal format to format and type
void _jsPlatformExpandInternalFormat( GLenum internalFormat, GLenum *format, GLenum *type )
{
    switch ( internalFormat )
    {
        case JSGCM_ALPHA16:
            *format = GL_ALPHA;
            *type = GL_UNSIGNED_SHORT;
            break;
        case JSGCM_ALPHA8:
            *format = GL_ALPHA;
            *type = GL_UNSIGNED_BYTE;
            break;
        case JSGCM_INTENSITY16:
            *format = GL_INTENSITY;
            *type = GL_UNSIGNED_SHORT;
            break;
        case JSGCM_INTENSITY8:
            *format = GL_INTENSITY;
            *type = GL_UNSIGNED_BYTE;
            break;
        case JSGCM_LUMINANCE16:
            *format = GL_LUMINANCE;
            *type = GL_UNSIGNED_SHORT;
            break;
        case JSGCM_LUMINANCE8:
            *format = GL_LUMINANCE;
            *type = GL_UNSIGNED_BYTE;
            break;
        case JSGCM_LUMINANCE16_ALPHA16:
            *format = GL_LUMINANCE_ALPHA;
            *type = GL_UNSIGNED_INT_16_16_REV_SCE;
            break;
        case JSGCM_LUMINANCE8_ALPHA8:
            *format = GL_LUMINANCE_ALPHA;
            *type = GL_UNSIGNED_SHORT_8_8_REV_SCE;
            break;
        case JSGCM_FLOAT_RGBX32:
            *format = GL_RGBA;
            *type = GL_FLOAT;
            break;
        case JSGCM_FLOAT_RGBA32:
            *format = GL_RGBA;
            *type = GL_FLOAT;
            break;
        case JSGCM_RGBX8:
            *format = GL_RGBA;
            *type = GL_UNSIGNED_INT_8_8_8_8;
            break;
        case JSGCM_RGBA8:
            *format = GL_RGBA;
            *type = GL_UNSIGNED_INT_8_8_8_8;
            break;
        case JSGCM_ARGB8:
            *format = GL_BGRA;
            *type = GL_UNSIGNED_INT_8_8_8_8_REV;
            break;
		case JSGCM_BGRA8:
			*format = GL_BGRA;
			*type = GL_UNSIGNED_INT_8_8_8_8;
			break;
        case JSGCM_FLOAT_RGBA16:
            *format = GL_RGBA;
            *type = GL_HALF_FLOAT_ARB;
            break;
        case JSGCM_FLOAT_RGBX16:
            *format = GL_RGBA;
            *type = GL_HALF_FLOAT_ARB;
            break;
        case JSGCM_RGB5_A1_SCE:
            *format = GL_RGBA;
            *type = GL_UNSIGNED_SHORT_1_5_5_5_REV;
            break;
        case JSGCM_RGB565_SCE:
            *format = GL_RGB;
            *type = GL_UNSIGNED_SHORT_5_6_5_REV;
            break;

        case JSGCM_ALPHA_LUMINANCE16F_SCE:
            // There is magic happening in the NV chips based on
            // endian swapping for texture memory interpretation. On
            // top of that, the crossbar functionnality available for
            // rerouting the texture components is not available for
            // LA16F... So we have to behave differently at the top
            // level on little and big endian in order to get the same
            // result in the shader in the end.
            // this affects pbo as well.
#if JS_ENDIAN == JS_BIG_ENDIAN
            *format = GL_ALPHA_LUMINANCE_SCE;
#else
            *format = GL_LUMINANCE_ALPHA;
#endif
            *type = GL_HALF_FLOAT_ARB;
            break;
        case JSGCM_LUMINANCE32F_ARB:
            *format = GL_LUMINANCE;
            *type = GL_FLOAT;
            break;
        case JSGCM_DEPTH_COMPONENT16:
            *format = GL_DEPTH_COMPONENT;
            *type = GL_UNSIGNED_SHORT;
            break;
        case JSGCM_DEPTH_COMPONENT24:
            *format = GL_DEPTH_COMPONENT;
            *type = GL_UNSIGNED_INT_24_8_SCE;
            break;
        case GL_COMPRESSED_RGB_S3TC_DXT1_EXT:
            *format = GL_RGB;
            *type = GL_UNSIGNED_BYTE;
            break;
        case GL_COMPRESSED_RGBA_S3TC_DXT1_EXT:
        case GL_COMPRESSED_RGBA_S3TC_DXT3_EXT:
        case GL_COMPRESSED_RGBA_S3TC_DXT5_EXT:
            *format = GL_RGBA;
            *type = GL_UNSIGNED_BYTE;
            break;
        default:
            JS_ASSERT( 0 );
            return;
    }
}

// Choose internal storage type and size, and set it to image, based on given format
GLenum _jsPlatformChooseInternalStorage( jsImage* image, GLenum internalFormat )
{
    // see note at bottom concerning storageSize
    image->storageSize = 0;

    GLenum platformInternalFormat = _jsPlatformChooseInternalFormat( internalFormat );

    if ( platformInternalFormat == GL_INVALID_ENUM ) return GL_INVALID_ENUM;
    image->internalFormat = platformInternalFormat;
    _jsPlatformExpandInternalFormat( platformInternalFormat, &image->format, &image->type );

    // Note that it is critical to get the storageSize value correct because
    // this member is used to configure texture loads and unloads.  If this
    // value is wrong (e.g. contains unnecessary padding) it will corrupt
    // the GPU memory layout.
    switch ( platformInternalFormat )
    {
        case GL_COMPRESSED_RGB_S3TC_DXT1_EXT:
        case GL_COMPRESSED_RGBA_S3TC_DXT1_EXT:
            image->storageType = _JS_IMAGE_STORAGE_BLOCK;
            image->xblk = 4;
            image->yblk = 4;
            image->zblk = 1;
            image->storageSize =
                _jsPad( image->width, image->xblk ) *
                _jsPad( image->height, image->yblk ) *
                image->depth / 2;
            break;
        case GL_COMPRESSED_RGBA_S3TC_DXT3_EXT:
        case GL_COMPRESSED_RGBA_S3TC_DXT5_EXT:
            image->storageType = _JS_IMAGE_STORAGE_BLOCK;
            image->xblk = 4;
            image->yblk = 4;
            image->zblk = 1;
            image->storageSize =
                _jsPad( image->width, image->xblk ) *
                _jsPad( image->height, image->yblk ) *
                image->depth;
            break;
        default:
            image->storageSize = _jsGetStorageSize(
                                     image->format, image->type,
                                     image->width, image->height, image->depth );
            break;
    }

    return GL_NO_ERROR;
}

// Translate platform-specific format to GL enum
GLenum _jsPlatformTranslateTextureFormat( GLenum internalFormat )
{
    switch ( internalFormat )
    {
        case JSGCM_RGBX8:
            return GL_RGBA8;
        case JSGCM_FLOAT_RGBA16:
        case JSGCM_FLOAT_RGBX16:
            return GL_RGBA16F_ARB;
        case JSGCM_FLOAT_RGBA32:
        case JSGCM_FLOAT_RGBX32:
            return GL_RGBA32F_ARB;
        default:	// same as GL
            return internalFormat;
    }
}

// Copy image data for 3D texture
// This is slow because image is copied from GPU to host memory, and
// copied back to GPU memory with format conversion
static void _jsPlatformSlowCopyTexSubImage3D(
    jsGcmSurface* src,
    jsTexture* texture,
    jsImage* image,
    GLint level,
    GLint xoffset, GLint yoffset, GLint zoffset,
    GLint x, GLint y,
    GLsizei width, GLsizei height )
{
    if ( texture->referenceBuffer != 0 )
    {
        _JS_REPORT_EXTRA( PSGL_REPORT_COPY_TEXTURE_WARNING, "Slow path texture copy doesn't work with texture references. The data will be invalid" );
        _jsSetError( GL_INVALID_OPERATION );
        return;
    }
    // bring back to host
    // TODO: Don't copy back to host if redefining the entire image.
    _jsPlatformCopyGPUTexture( texture );
    _jsImageAllocCPUStorage( image );

    // copy to temporary buffer in system memory
    //  This takes care of any downsampling, swizzling, or flip.
    jsGcmSurface tmp = *src;
    tmp.source	= _JSGCM_SURFACE_SOURCE_TEMPORARY;
    tmp.width	= width;
    tmp.height	= height;
    tmp.pitch	= _jsPad( tmp.bpp * tmp.width, 4 );
    tmp.samples	= 1;
    tmp.flip	= GL_FALSE;
    tmp.pool	= _JSGCM_SURFACE_POOL_PPU;
    tmp.ppuData	= ( char * )jsMalloc( tmp.pitch * tmp.height );
#ifndef JS_NO_ERROR_CHECK
    if ( tmp.ppuData == NULL )
    {
        _jsSetError( GL_OUT_OF_MEMORY );
        return;
    }
#endif
    // TODO: don't copy entire surface unless necessary
    _jsGcmCopySurface( src, x, y, &tmp, 0, 0, tmp.width, tmp.height, GL_FALSE );

    // describe framebuffer source
    GLenum srcFormat = 0;
    GLenum srcType = 0;
    switch ( src->format )
    {
        case JSGCM_ARGB8:
            srcFormat = GL_BGRA;
            srcType = GL_UNSIGNED_INT_8_8_8_8_REV;
            break;
        case JSGCM_FLOAT_RGBA16:
            srcFormat = GL_RGBA;
            srcType = GL_HALF_FLOAT_ARB;
            break;
        case JSGCM_FLOAT_RGBA32:
            srcFormat = GL_RGBA;
            srcType = GL_FLOAT;
            break;
        case JSGCM_DEPTH_COMPONENT16:
            srcFormat = GL_DEPTH_COMPONENT;
            srcType = GL_UNSIGNED_SHORT;
            break;
        case JSGCM_DEPTH_COMPONENT24:
            srcFormat = GL_DEPTH_COMPONENT;
            srcType = GL_UNSIGNED_INT_24_8_SCE;
            break;
        default:
            JS_ASSERT( 0 );
    }

    jsRaster raster =
        {
        format:		srcFormat,
        type:		srcType,
        width:		width,
        height:		height,
        depth:		1,
        xstride:	tmp.bpp,
        ystride:	tmp.pitch,
        zstride:	tmp.pitch*tmp.height,
        data:	    tmp.ppuData
        };

    // copy with conversion
    _jsGcmFifoGlFinish();
    _jsRasterToImage( &raster, image, xoffset, yoffset, 0 );

    jsFree( tmp.ppuData );

    image->dataState = _JS_IMAGE_DATASTATE_HOST;
    texture->revalidate |= _JS_TEXTURE_REVALIDATE_IMAGES;
}

// Update part of current 3D texture image by copying it from the color buffer
// If format of the color buffer does not match with texture or texture is incomplete, 
// this goes into slow path.
void _jsPlatformCopyTexSubImage3D( GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height )
{
    PSGLcontext*	LContext = _CurrentContext;
    JS_ASSERT( LContext );

    jsTexture *texture;
    jsImage *image;
    if ( _jsGetImage( target, level, &texture, &image, 0 ) )
    {
        return;
    }

    // determine source surface
    jsGcmSurface src;
    if ( JS_UNLIKELY( LContext->needValidate & PSGL_VALIDATE_FRAMEBUFFER ) )
        _jsValidateFramebuffer();
    if ( _jsGcmGetFramebufferSurface( image->internalFormat, &src ) )
    {
        _jsSetError( GL_INVALID_OPERATION );
        return;
    }

    // don't access outside the source surface
    if ( x + width > ( int )src.width )
        width = src.width - x;
    if ( y + height > ( int )src.height )
        height = src.height - y;

    // don't write outside the destination surface
    if ( xoffset + width > image->width )
        width = image->width - xoffset;
    if ( yoffset + height > image->height )
        height = image->height - yoffset;

    // check the internal format of the image
    if ( image->internalFormat != ( GLenum )src.format )
    {
        // slow path
        _JS_REPORT_EXTRA( PSGL_REPORT_COPY_TEXTURE_WARNING, "Copying buffer with format %s to texture %d with internal format %s will use the slow path", _jsGetGLEnumName( src.format ), texture->name, _jsGetGLEnumName( image->internalFormat ) );
        _jsPlatformSlowCopyTexSubImage3D(
            &src,
            texture,
            image,
            level,
            xoffset, yoffset, zoffset,
            x, y,
            width, height );
        return;
    }

    // if texture isn't complete, it can't be allocated on the GPU correctly
    if ( !_jsTextureIsValid( texture ) )
    {
        // slow path
        _JS_REPORT_EXTRA( PSGL_REPORT_COPY_TEXTURE_WARNING, "Copying to texture %d which is incomplete will use the slow path", texture->name );
        _jsPlatformSlowCopyTexSubImage3D(
            &src,
            texture,
            image,
            level,
            xoffset, yoffset, zoffset,
            x, y,
            width, height );
        return;
    }

    jsGcmTexture* gcmTexture  = ( jsGcmTexture* ) texture->platformTexture;
    _jsPlatformTextureDetachFromPBOLazy( texture, GL_FALSE );
    _jsPlatformValidateTextureResources( texture );

    // create destination surface descriptor
    const GLuint face =
        ( target >= GL_TEXTURE_CUBE_MAP_POSITIVE_X &&
          target <  GL_TEXTURE_CUBE_MAP_POSITIVE_X + 6 ) ?
        ( target - GL_TEXTURE_CUBE_MAP_POSITIVE_X ) : 0;
    const GLuint dataOffset = _jsGetGcmImageOffset( &gcmTexture->gpuLayout, face, level );
    jsGcmSurface dst =
        {
        source:		_JSGCM_SURFACE_SOURCE_TEXTURE,
        width:		image->width,
        height:		image->height,
        bpp:		gcmTexture->gpuLayout.pixelBits / 8,
        pitch:		gcmTexture->gpuLayout.pitch,
        samples:	1,
        format:		gcmTexture->gpuLayout.internalFormat,
        flip:		GL_FALSE,
        pool:		gcmTexture->pool,
        ppuData:    NULL,
        dataId:		gcmTexture->gpuAddressId,
        dataIdOffset: dataOffset,
        };

    GLuint offsetHeight = 0;
    if ( dst.pitch )
    {
        // linear (not swizzled)
        //  The tiled linear format requires that render
        //  targets be aligned to 8*pitch from the start of
        //  the tiled region.
        offsetHeight = ( dataOffset / dst.pitch ) % 8;
        dst.height += offsetHeight;
        dst.dataIdOffset -= offsetHeight * dst.pitch;
    }

    _jsGcmCopySurface(
        &src, x, y,
        &dst, xoffset, yoffset + offsetHeight,
        width, height,
        GL_TRUE );		// don't bypass GPU pipeline

    image->dataState = _JS_IMAGE_DATASTATE_GPU;
}

void _jsPlatformGetImageData( GLenum target, GLint level, jsTexture *texture, jsImage *image )
{
    _jsPlatformCopyGPUTexture( texture );
}

// Set compressed image data to a new image in a texture 
// Data is kept on the host memory
void _jsPlatformSetCompressedTexture(
    GLenum target,
    GLint level,
    GLenum internalFormat,
    GLsizei width,
    GLsizei height,
    GLsizei depth,
    GLint border,
    GLsizei imageSize,
    const GLvoid* data )
{
    // get the texture and image
    jsTexture* texture = NULL;
    jsImage* image = NULL;
    _jsGetImage( target, level, &texture, &image, MAX( width, height ) );
#ifndef JS_NO_ERROR_CHECK
    if ( texture->referenceBuffer )
    {
        _jsSetError( GL_INVALID_OPERATION );
        return;
    }
#endif

    // initialize image
    //  Default storage values are set here.  These values will be
    //  overwritten by _jsPlatformChooseInternalStorage.
    image->width = width;
    image->height = height;
    image->depth = depth;
    image->storageType = _JS_IMAGE_STORAGE_RASTER;
    image->storageSize = 0;
    image->xblk = 0;
    image->yblk = 0;
    image->zblk = 0;
    image->xstride = 0;
    image->ystride = 0;
    image->zstride = 0;

    // fill in struct with format-specific values
    const GLenum status = _jsPlatformChooseInternalStorage( image, internalFormat );
#ifndef JS_NO_ERROR_CHECK
    if ( status != GL_NO_ERROR )
    {
        _jsSetError( status );
        return;
    }

    // XXX these tests are done too late
    if ( image->storageSize != imageSize )
    {
        _jsSetError( GL_INVALID_VALUE );
        return;
    }
#else
    (( void )status );		// unused variable is ok
#endif

    // allocate storage (free previous)
    _jsImageAllocCPUStorage( image );
    if ( !image->data )
    {
        _jsSetError( GL_OUT_OF_MEMORY );
        return;
    }
    image->isSet = GL_TRUE;

    // compute default strides if necessary
    if ( image->storageType == _JS_IMAGE_STORAGE_RASTER )
    {
        if ( image->xstride == 0 )
            image->xstride = 1;
        if ( image->ystride == 0 )
            image->ystride = image->width * image->xstride;
        if ( image->zstride == 0 )
            image->zstride = image->height * image->ystride;
    }

    // copy compressed data into image
    if ( data != NULL )
    {
        fast_memcpy( image->data, data, imageSize );
        image->dataState = _JS_IMAGE_DATASTATE_HOST;
    }
    else
        image->dataState = _JS_IMAGE_DATASTATE_UNSET;
    if ( level > 0 ) texture->hasMipmaps = GL_TRUE;
}

// Update part of a compressed texture image by copying data
void _jsPlatformSetCompressedTextureSub(
    GLenum target,
    GLint level,
    GLint xoffset,
    GLint yoffset,
    GLint zoffset,
    GLsizei width,
    GLsizei height,
    GLsizei depth,
    GLenum format,
    GLsizei imageSize,
    const GLvoid* data )
{
    // get the texture and image
    jsTexture* texture = NULL;
    jsImage* image = NULL;
    if ( _jsGetImage( target, level, &texture, &image, 0 ) )
    {
        _jsSetError( GL_INVALID_VALUE );
        return;
    }

#ifndef JS_NO_ERROR_CHECK
    if ( texture->referenceBuffer )
    {
        _jsSetError( GL_INVALID_OPERATION );
        return;
    }
    if ( !image->isSet )
    {
        _jsSetError( GL_INVALID_OPERATION );
        return;
    }

    // no support for 3D texture yet
    if ( depth > 1 || image->depth > 1 )
    {
        _jsSetError( GL_INVALID_VALUE );
        return;
    }

    // check args for consistency
    if ( format != image->internalFormat )
    {
        _jsSetError( GL_INVALID_ENUM );
        return;
    }

    if ( xoffset + width > image->width )
    {
        _jsSetError( GL_INVALID_VALUE );
        return;
    }

    if ( yoffset + height > image->height )
    {
        _jsSetError( GL_INVALID_VALUE );
        return;
    }

    if ( zoffset + depth > image->depth )
    {
        _jsSetError( GL_INVALID_VALUE );
        return;
    }

    // only allow whole 4x4 block replacement on DXTn compressed data
    if (( xoffset & 0x3 ) || ( width & 0x3 ) ||
            ( yoffset & 0x3 ) || ( height & 0x3 ) )
    {
        _jsSetError( GL_INVALID_OPERATION );
        return;
    }
#endif

    GLint xBlockStride = 0;
    switch ( image->internalFormat )
    {
        case GL_COMPRESSED_RGB_S3TC_DXT1_EXT:
        case GL_COMPRESSED_RGBA_S3TC_DXT1_EXT:
        case GL_COMPRESSED_RGBA_S3TC_DXT3_EXT:
        case GL_COMPRESSED_RGBA_S3TC_DXT5_EXT:
            xBlockStride = _jsPlatformGetBitsPerPixel( image->internalFormat ) * 4 * 4 / 8;
            break;
        default:
            _jsSetError( GL_INVALID_OPERATION );
            return;
    }

#ifndef JS_NO_ERROR_CHECK
    if ( xBlockStride*width*height / 16 != imageSize )
    {
        _jsSetError( GL_INVALID_VALUE );
        return;
    }
#endif

    const GLuint yBlockStride = xBlockStride * _jsPad( image->width, 4 ) / 4;

    // XXX Does this work?  Is it needed?
    if ( image->dataState & _JS_IMAGE_DATASTATE_GPU )
        _jsPlatformGetImageData( target, level, texture, image );

    JS_ASSERT( image->data );

    const char* sptr = ( char * )data;
    for ( int j = 0; j < height; j += 4 )
    {
        // copy a line
        void* dptr = image->data +
                     yBlockStride * ( yoffset + j ) / 4 +
                     xBlockStride * ( xoffset ) / 4;
        fast_memcpy( dptr, sptr, xBlockStride*width / 4 );

        sptr += xBlockStride * width / 4;
    }

    image->dataState = _JS_IMAGE_DATASTATE_HOST;
}

// Set an image to a texture either by referencing or copying data from current PBO
GLboolean _jsPlatformTexturePBOImage(
    jsTexture* texture,
    jsImage* image,
    GLint level,
    GLint internalFormat,
    GLsizei width, GLsizei height, GLsizei depth,
    GLenum format, GLenum type,
    const GLvoid *offset )
{
    PSGLcontext*	LContext = _CurrentContext;
    JS_ASSERT( LContext );

    jsGcmTexture *gcmTexture = ( jsGcmTexture * )texture->platformTexture;

    image->dataState = _JS_IMAGE_DATASTATE_UNSET;
    // remove current PBO, if any
    //  If we are replacing an existing PBO reference with new data from a
    //  PBO, then dropping the texture won't trigger a copy because the
    //  dataState status is reset.
    //
    //  If we are setting a different face/level than an existing PBO
    //  reference, then we copy the data back to the host because we have to
    //  create a new texture layout.
    if ( gcmTexture->pbo != NULL )
        _jsPlatformDropTexture( texture );

    // fast path
    //  Set image members but no data.  We need to preserve the image
    //  configuration in case the data is copied back from the GPU.
    _jsSetImage( image, internalFormat, width, height, depth, LContext->unpackAlignment, format, type, NULL );

    // test if input actually comes from a PBO
    if ( LContext->PixelUnpackBuffer == 0 )
        return GL_FALSE;

    // We need to ensure that the input texture has the correct pitch if linear.
    const GLuint pboPitch = _jsPad(
                                _jsGetStorageSize( format, type, width, 1, 1 ),
                                LContext->unpackAlignment );
    if (( pboPitch&3 ) != 0 )
    {
        _JS_REPORT_EXTRA( PSGL_REPORT_TEXTURE_COPY_BACK, "PBO image pitch not a multiple of 4, using slow path" );
        return GL_FALSE;
    }

    // aligned address
    //  Linear textures do not require 128-byte alignment (that swizzled
    //  textures do).  The doc says that linear textures may start at any
    //  offset, but since rows must be 4-byte aligned we enforce that for
    //  the beginning of the buffer.
    GLuint gpuId = _jsGcmGetBufferObjectOrigin( LContext->PixelUnpackBuffer );
    GLuint gpuIdOffset = (( GLubyte* )offset - ( GLubyte* )NULL );

    const GLenum bufferPool = gmmIdIsMain( gpuId ) ?
                              _JSGCM_SURFACE_POOL_SYSTEM :
                              _JSGCM_SURFACE_POOL_LINEAR;

    // XXX Antoine we need 64 bytes alignment for copy vid2vid requirements
    if ( gmmIdToOffset(gpuId)+gpuIdOffset & 63 )
    {
        _JS_REPORT_EXTRA( PSGL_REPORT_TEXTURE_COPY_BACK, "PBO offset not 64-byte aligned, using slow path" );
        return GL_FALSE;
    }

    // supported format/type
    //  Here we verify that the data buffer passed in matches the NV format.
    //  Before adding a new format, cross check with _jsPlatformChooseInternalStorage
    //  for consistency.
    GLboolean formatOK = GL_FALSE;
    switch ( internalFormat )
    {
        case 4:
        case GL_RGBA:
        case GL_RGBA8:
            if ( format == GL_RGBA && type == GL_UNSIGNED_INT_8_8_8_8 )
                formatOK = GL_TRUE;
            break;
        case GL_RGBA16F_ARB:
            if ( format == GL_RGBA && type == GL_HALF_FLOAT_ARB )
                formatOK = GL_TRUE;
            break;
        case GL_RGBA32F_ARB:
            if ( format == GL_RGBA && type == GL_FLOAT )
                formatOK = GL_TRUE;
            break;
        case 1:
        case GL_LUMINANCE:
        case GL_LUMINANCE8:
            if ( format == GL_LUMINANCE && type == GL_UNSIGNED_BYTE )
                formatOK = GL_TRUE;
            break;
        case GL_LUMINANCE16:
            if ( format == GL_LUMINANCE && type == GL_UNSIGNED_SHORT )
                formatOK = GL_TRUE;
            break;
        case GL_LUMINANCE32F_ARB:
            if ( format == GL_LUMINANCE && type == GL_FLOAT )
                formatOK = GL_TRUE;
            break;
        case GL_ALPHA:
        case GL_ALPHA8:
            if ( format == GL_ALPHA && type == GL_UNSIGNED_BYTE )
                formatOK = GL_TRUE;
            break;
        case GL_ALPHA16:
            if ( format == GL_ALPHA && type == GL_UNSIGNED_SHORT )
                formatOK = GL_TRUE;
            break;
        case GL_INTENSITY:
        case GL_INTENSITY8:
            if ( format == GL_INTENSITY && type == GL_UNSIGNED_BYTE )
                formatOK = GL_TRUE;
            break;
        case GL_INTENSITY16:
            if ( format == GL_INTENSITY && type == GL_UNSIGNED_SHORT )
                formatOK = GL_TRUE;
            break;
        case 2:
        case GL_LUMINANCE_ALPHA:
        case GL_LUMINANCE8_ALPHA8:
            if ( format == GL_LUMINANCE_ALPHA && type == GL_UNSIGNED_SHORT_8_8_REV_SCE )
                formatOK = GL_TRUE;
            break;
        case GL_LUMINANCE16_ALPHA16:
            if ( format == GL_LUMINANCE_ALPHA && type == GL_UNSIGNED_INT_16_16_REV_SCE )
                formatOK = GL_TRUE;
            break;
        case GL_LUMINANCE_ALPHA16F_ARB:
            // See the ChooseInternalStorage comment for more details.
#if JS_ENDIAN == JS_BIG_ENDIAN
            if ( format == GL_ALPHA_LUMINANCE_SCE && type == GL_HALF_FLOAT_ARB )
                formatOK = GL_TRUE;
#else
            if ( format == GL_LUMINANCE_ALPHA && type == GL_HALF_FLOAT_ARB )
                formatOK = GL_TRUE;
#endif
            break;
        case GL_DEPTH_COMPONENT16:
            if ( format == GL_DEPTH_COMPONENT && type == GL_UNSIGNED_SHORT )
                formatOK = GL_TRUE;
            break;
        case GL_DEPTH_COMPONENT:
        case GL_DEPTH_COMPONENT24:
            if ( format == GL_DEPTH_COMPONENT && type == GL_UNSIGNED_INT_24_8_SCE )
                formatOK = GL_TRUE;
            break;
        case GL_ARGB_SCE:
            if ( format == GL_BGRA && type == GL_UNSIGNED_INT_8_8_8_8_REV )
                formatOK = GL_TRUE;
            break;
        default:
            formatOK = GL_FALSE;
    }

    if ( !formatOK )
    {
        _JS_REPORT_EXTRA( PSGL_REPORT_TEXTURE_COPY_BACK, "PBO format/type requires conversion to texture internal format, using slow path" );
        return GL_FALSE;
    }

    // require complete texture so a layout can be assigned
    if ( !_jsTextureIsValid( texture ) )
    {
        _JS_REPORT_EXTRA( PSGL_REPORT_TEXTURE_COPY_BACK, "PBO transfering to incomplete texture, using slow path" );
        return GL_FALSE;
    }

    // get a provisional layout
    //  This layout is forced to be linear, to allow referencing PBO data
    //  directly.  If we end up copying PBO data instead, another layout
    //  will be computed via _jsPlatformValidateTextureResources which is
    //  not restricted to linear.
    jsGcmTextureLayout newLayout;
    _jsPlatformChooseGPUFormatAndLayout( texture, GL_TRUE, pboPitch, &newLayout );

    if ( level >= ( int )newLayout.levels )
    {
        _JS_REPORT_EXTRA( PSGL_REPORT_TEXTURE_COPY_BACK, "A non-zero mipmap level is being set on a texture that is not mipmap complete, using slow path." );
        return GL_FALSE;
    }

    // decide whether to assign image data by reference or copy
    //  We can only reference the PBO data for non-mipmap, non-cubemap
    //  textures.
    //
    //  If the buffer is mapped, we must copy.
    // TODO: allow S3TC
    // TODO: force proper memory, SYSTEM or GPU
    jsBufferObject* bufferObject = _jsGetBufferObject( LContext, LContext->PixelUnpackBuffer );
    JS_ASSERT( bufferObject != NULL );
    if ( texture->target != GL_TEXTURE_CUBE_MAP &&
            newLayout.levels == 1 &&
            newLayout.pitch != 0 &&
            !bufferObject->mapped )
    {
        // reference PBO
        gcmTexture->gpuLayout = newLayout;
        if ( gcmTexture->gpuAddressId != GMM_ERROR && gcmTexture->pbo == NULL )
        { // texture not bound to another PBO and already has mem. allocated for it
            _jsPlatformFreeGcmTexture( texture );
        }
        gcmTexture->pbo = bufferObject;
        gcmTexture->gpuAddressId = gpuId; 
        gcmTexture->gpuAddressIdOffset = gpuIdOffset;
        gcmTexture->pool = bufferPool;
        gcmTexture->gpuSize = _jsPlatformTextureGetGPUSize( texture );
        ++bufferObject->refCount;	// add ref to buffer object
    }
    else
    {
        // copy data from PBO into texture

        // create source surface descriptor
        const GLuint bytesPerPixel = newLayout.pixelBits / 8;
        jsGcmSurface src =
            {
        source:		_JSGCM_SURFACE_SOURCE_PBO,
        width:		image->width,
        height:		image->height,
        bpp:		bytesPerPixel,
        pitch:		pboPitch,
        samples:	1,
        format:		newLayout.internalFormat,
        flip:		GL_FALSE,
        pool:		bufferPool,
        ppuData:    NULL,
        dataId:     gpuId,
        dataIdOffset: gpuIdOffset, 
            };

        // reallocate texture if necessary
        JS_ASSERT( texture->referenceBuffer == 0 );
        texture->revalidate |= _JS_TEXTURE_REVALIDATE_LAYOUT;
        _jsPlatformValidateTextureResources( texture );

        // create destination surface descriptor
        const GLuint face =
            ( texture->target >= GL_TEXTURE_CUBE_MAP_POSITIVE_X &&
              texture->target <  GL_TEXTURE_CUBE_MAP_POSITIVE_X + 6 ) ?
            ( texture->target - GL_TEXTURE_CUBE_MAP_POSITIVE_X ) : 0;
        const GLuint dataOffset = _jsGetGcmImageOffset( &gcmTexture->gpuLayout, face, level );
        jsGcmSurface dst =
            {
        source:		_JSGCM_SURFACE_SOURCE_TEXTURE,
        width:		image->width,
        height:		image->height,
        bpp:		bytesPerPixel,
        pitch:		gcmTexture->gpuLayout.pitch,
        samples:	1,
        format:		gcmTexture->gpuLayout.internalFormat,
        flip:		GL_FALSE,
        pool:		gcmTexture->pool,
        ppuData:    NULL,
        dataId:     gcmTexture->gpuAddressId,
        dataIdOffset: gcmTexture->gpuAddressIdOffset + dataOffset,
            };

        GLuint offsetHeight = 0;
        if ( dst.pitch )
        {
            // linear (not swizzled)
            //  The tiled linear format requires that render
            //  targets be aligned to 8*pitch from the start of
            //  the tiled region.
            offsetHeight = ( dataOffset / dst.pitch ) % 8;
            dst.height += offsetHeight;
            dst.dataIdOffset -= offsetHeight * dst.pitch;
        }

        _jsGcmCopySurface(
            &src, 0, 0,
            &dst, 0, offsetHeight,
            width, height,
            GL_TRUE );		// don't bypass GPU pipeline
    }

    // flag host image buffer as not containing current data
    _jsImageFreeCPUStorage( image );
    image->dataState = _JS_IMAGE_DATASTATE_GPU;

    // trigger address update
    texture->revalidate &= ~( _JS_TEXTURE_REVALIDATE_LAYOUT | _JS_TEXTURE_REVALIDATE_IMAGES );
    texture->revalidate |= _JS_TEXTURE_REVALIDATE_PARAMETERS;
    _jsTextureTouchFBOs( texture );

    return GL_TRUE;
}

// Update part of texture image by copying data from PBO
extern GLboolean _jsPlatformTexturePBOSubImage(
        jsTexture* texture,
        jsImage* image,
        GLint level,
        GLint xoffset, GLint yoffset, GLint zoffset,
        GLsizei width, GLsizei height, GLsizei depth,
        GLenum format, GLenum type,
        const GLvoid *offset )
{
    PSGLcontext*	LContext = _CurrentContext;
    JS_ASSERT( LContext );

    jsGcmTexture *gcmTexture = ( jsGcmTexture * )texture->platformTexture;

    image->dataState = _JS_IMAGE_DATASTATE_UNSET;

    // test if input actually comes from a PBO
    if ( LContext->PixelUnpackBuffer == 0 )
        return GL_FALSE;

    // We need to ensure that the input texture has the correct pitch if linear.
    const GLuint pboPitch = _jsPad(
                                _jsGetStorageSize( format, type, width, 1, 1 ),
                                LContext->unpackAlignment );
    if (( pboPitch&3 ) != 0 )
    {
        _JS_REPORT_EXTRA( PSGL_REPORT_TEXTURE_COPY_BACK, "PBO image pitch not a multiple of 4, using slow path" );
        return GL_FALSE;
    }

    // aligned address
    //  Linear textures do not require 128-byte alignment (that swizzled
    //  textures do).  The doc says that linear textures may start at any
    //  offset, but since rows must be 4-byte aligned we enforce that for
    //  the beginning of the buffer.
    GLuint gpuId = _jsGcmGetBufferObjectOrigin( LContext->PixelUnpackBuffer );
    GLuint gpuIdOffset = (( GLubyte* )offset - ( GLubyte* )NULL );
    const GLenum bufferPool = gmmIdIsMain( gpuId ) ?
                              _JSGCM_SURFACE_POOL_SYSTEM :
                              _JSGCM_SURFACE_POOL_LINEAR;

    // XXX Antoine we need 64 bytes alignment for copy vid2vid requirements
    if ( gmmIdToOffset(gpuId)+gpuIdOffset & 63 )
    {
        _JS_REPORT_EXTRA( PSGL_REPORT_TEXTURE_COPY_BACK, "PBO offset not 64-byte aligned, using slow path" );
        return GL_FALSE;
    }

    // check if the platform texture is already there
    if ( gcmTexture->gpuAddressId == GMM_ERROR )
    {
        _JS_REPORT_EXTRA( PSGL_REPORT_TEXTURE_COPY_BACK, "platform texture not created yet, using slow path" );
        return GL_FALSE;
    }

    // supported format/type
    //  Here we verify that the data buffer passed in matches the NV format.
    //  Before adding a new format, cross check with _jsPlatformChooseInternalStorage
    //  for consistency.
    jsGcmEnum internalFormat = gcmTexture->gpuLayout.internalFormat;
    GLboolean formatOK = GL_FALSE;
    switch ( internalFormat )
    {
        case 4:
        case GL_RGBA:
        case GL_RGBA8:
            if ( format == GL_RGBA && type == GL_UNSIGNED_INT_8_8_8_8 )
                formatOK = GL_TRUE;
            break;
        case GL_RGBA16F_ARB:
            if ( format == GL_RGBA && type == GL_HALF_FLOAT_ARB )
                formatOK = GL_TRUE;
            break;
        case GL_RGBA32F_ARB:
            if ( format == GL_RGBA && type == GL_FLOAT )
                formatOK = GL_TRUE;
            break;
        case 1:
        case GL_LUMINANCE:
        case GL_LUMINANCE8:
            if ( format == GL_LUMINANCE && type == GL_UNSIGNED_BYTE )
                formatOK = GL_TRUE;
            break;
        case GL_LUMINANCE16:
            if ( format == GL_LUMINANCE && type == GL_UNSIGNED_SHORT )
                formatOK = GL_TRUE;
            break;
        case GL_LUMINANCE32F_ARB:
            if ( format == GL_LUMINANCE && type == GL_FLOAT )
                formatOK = GL_TRUE;
            break;
        case GL_ALPHA:
        case GL_ALPHA8:
            if ( format == GL_ALPHA && type == GL_UNSIGNED_BYTE )
                formatOK = GL_TRUE;
            break;
        case GL_ALPHA16:
            if ( format == GL_ALPHA && type == GL_UNSIGNED_SHORT )
                formatOK = GL_TRUE;
            break;
        case GL_INTENSITY:
        case GL_INTENSITY8:
            if ( format == GL_INTENSITY && type == GL_UNSIGNED_BYTE )
                formatOK = GL_TRUE;
            break;
        case GL_INTENSITY16:
            if ( format == GL_INTENSITY && type == GL_UNSIGNED_SHORT )
                formatOK = GL_TRUE;
            break;
        case 2:
        case GL_LUMINANCE_ALPHA:
        case GL_LUMINANCE8_ALPHA8:
            if ( format == GL_LUMINANCE_ALPHA && type == GL_UNSIGNED_SHORT_8_8_REV_SCE )
                formatOK = GL_TRUE;
            break;
        case GL_LUMINANCE16_ALPHA16:
            if ( format == GL_LUMINANCE_ALPHA && type == GL_UNSIGNED_INT_16_16_REV_SCE )
                formatOK = GL_TRUE;
            break;
        case GL_LUMINANCE_ALPHA16F_ARB:
            // See the ChooseInternalStorage comment for more details.
#if JS_ENDIAN == JS_BIG_ENDIAN
            if ( format == GL_ALPHA_LUMINANCE_SCE && type == GL_HALF_FLOAT_ARB )
                formatOK = GL_TRUE;
#else
            if ( format == GL_LUMINANCE_ALPHA && type == GL_HALF_FLOAT_ARB )
                formatOK = GL_TRUE;
#endif
            break;
        case GL_DEPTH_COMPONENT16:
            if ( format == GL_DEPTH_COMPONENT && type == GL_UNSIGNED_SHORT )
                formatOK = GL_TRUE;
            break;
        case GL_DEPTH_COMPONENT:
        case GL_DEPTH_COMPONENT24:
            if ( format == GL_DEPTH_COMPONENT && type == GL_UNSIGNED_INT_24_8_SCE )
                formatOK = GL_TRUE;
            break;
        case GL_ARGB_SCE:
            if ( format == GL_BGRA && type == GL_UNSIGNED_INT_8_8_8_8_REV )
                formatOK = GL_TRUE;
            break;
        default:
            formatOK = GL_FALSE;
    }

    if ( !formatOK )
    {
        _JS_REPORT_EXTRA( PSGL_REPORT_TEXTURE_COPY_BACK, "PBO format/type requires conversion to texture internal format, using slow path" );
        return GL_FALSE;
    }

    // require complete texture so a layout can be assigned
    if ( !_jsTextureIsValid( texture ) )
    {
        _JS_REPORT_EXTRA( PSGL_REPORT_TEXTURE_COPY_BACK, "PBO transfering to incomplete texture, using slow path" );
        return GL_FALSE;
    }

    if ( level >= ( int )gcmTexture->gpuLayout.levels )
    {
        _JS_REPORT_EXTRA( PSGL_REPORT_TEXTURE_COPY_BACK, "A non-zero mipmap level is being set on a texture that is not mipmap complete, using slow path." );
        return GL_FALSE;
    }

    jsBufferObject* bufferObject = _jsGetBufferObject( LContext, LContext->PixelUnpackBuffer );
    JS_ASSERT( bufferObject != NULL );
    if ( bufferObject->internalFormat != (GLenum)internalFormat )
    {
        _JS_REPORT_EXTRA( PSGL_REPORT_TEXTURE_COPY_BACK, "PBO internal format doesn't match texture's, using slow path." );
        return GL_FALSE;
    }

    {
        // copy data from PBO into texture

        // create source surface descriptor
        const GLuint bytesPerPixel = gcmTexture->gpuLayout.pixelBits / 8;
        jsGcmSurface src =
            {
        source:		_JSGCM_SURFACE_SOURCE_PBO,
        width:		image->width,
        height:		image->height,
        bpp:		bytesPerPixel,
        pitch:		pboPitch,
        samples:	1,
        format:		internalFormat,
        flip:		GL_FALSE,
        pool:		bufferPool,
        ppuData:	NULL,
        dataId:     gpuId,
        dataIdOffset: gpuIdOffset,
            };

        // create destination surface descriptor
        const GLuint face =
            ( texture->target >= GL_TEXTURE_CUBE_MAP_POSITIVE_X &&
              texture->target <  GL_TEXTURE_CUBE_MAP_POSITIVE_X + 6 ) ?
            ( texture->target - GL_TEXTURE_CUBE_MAP_POSITIVE_X ) : 0;
        const GLuint dataOffset = _jsGetGcmImageOffset( &gcmTexture->gpuLayout, face, level );
        jsGcmSurface dst =
            {
        source:		_JSGCM_SURFACE_SOURCE_TEXTURE,
        width:		image->width,
        height:		image->height,
        bpp:		bytesPerPixel,
        pitch:		gcmTexture->gpuLayout.pitch,
        samples:	1,
        format:		internalFormat,
        flip:		GL_FALSE,
        pool:		gcmTexture->pool,
        ppuData:	NULL,
        dataId:     gcmTexture->gpuAddressId,
        dataIdOffset: gcmTexture->gpuAddressIdOffset + dataOffset,
            };

        GLuint offsetHeight = 0;
        if ( dst.pitch )
        {
            // linear (not swizzled)
            //  The tiled linear format requires that render
            //  targets be aligned to 8*pitch from the start of
            //  the tiled region.
            offsetHeight = ( dataOffset / dst.pitch ) % 8;
            dst.height += offsetHeight;
            dst.dataIdOffset -= offsetHeight * dst.pitch;
        }

        _jsGcmCopySurface(
            &src, 0, 0,
            &dst, xoffset, yoffset+offsetHeight,
            width, height,
            GL_TRUE );		// don't bypass GPU pipeline
    }

    // flag host image buffer as not containing current data
    _jsImageFreeCPUStorage( image );
    image->dataState = _JS_IMAGE_DATASTATE_GPU;

    // trigger address update
    texture->revalidate |= _JS_TEXTURE_REVALIDATE_IMAGES;
    _jsTextureTouchFBOs( texture );

    return GL_TRUE;
}

// Generate mip levels for a texture by using _jsCurieScaleBlit function
GLboolean _jsPlatformGenerateMipmap( jsTexture* texture, GLuint face )
{
    JS_ASSERT( texture != NULL );
    JS_ASSERT( face < 6 );

    // get image array
    jsImage* image = texture->images[face];

    // check base level for valid texels
    if ( image->dataState == _JS_IMAGE_DATASTATE_UNSET )
        return GL_FALSE;

    switch ( image->internalFormat )
    {
        case JSGCM_ARGB8:
        case JSGCM_RGBA8:
        case JSGCM_RGBX8:
        case JSGCM_FLOAT_RGBA16:
        case JSGCM_FLOAT_RGBX16:
            break;
        default:
            _JS_REPORT_EXTRA( PSGL_REPORT_TEXTURE_COPY_BACK, "Fast mipmap generation not supported for this texture format" );
            return GL_FALSE;
    }

    // check texture validity
    if ( !_jsTextureHasValidLevels(
                texture,
                texture->imageCount,
                image->width, image->height, image->depth,
                image->format, image->type, image->internalFormat ) )
        return GL_FALSE;

    // update GPU layout
    _jsPlatformValidateTextureResources( texture );

    // test if fast path is supported for this layout
    jsGcmTexture *gcmTexture = ( jsGcmTexture * )texture->platformTexture;
    const GLuint bpp = gcmTexture->gpuLayout.pixelBits / 8;

    // allocate temporary drawing buffer
    //  The use of a temporary buffer might be avoided for some levels if
    //  the level memory meets render target target pitch and alignment
    //  restrictions and base level texture alignment restrictions.  That
    //  optimization is not attempted here, so we always filter to the
    //  temporary buffer and copy (potentially with swizzle) to the texture
    //  level.
    const GLuint tmpPitch = _jsPad( bpp * ( gcmTexture->gpuLayout.baseWidth / 2 ), 128 );
    _jsAllocSetHint( _JS_ALLOCTYPE_MIPMAP_GEN_TEMP_BUFFER, 0 );
    GLuint tmpBufferId = _jsGcmAllocateTextureMemory( tmpPitch * gcmTexture->gpuLayout.baseHeight * 3 / 4 );
    _jsAllocClearHint();
    GLuint srcId = gcmTexture->gpuAddressId;
    GLuint srcOffset = gcmTexture->gpuAddressIdOffset + _jsGetGcmImageOffset( &gcmTexture->gpuLayout, face, 0 );
    GLuint srcPitch = gcmTexture->gpuLayout.pitch;
    GLuint tmpId = tmpBufferId;
    GLuint tmpOffset = 0;
    GLuint levels = gcmTexture->gpuLayout.levels;
    for ( GLuint i = 1; i < levels; ++i )
    {
        // downsample with filtering
        _jsAllocSetHint( _JS_ALLOCTYPE_MIPMAP_GEN_TEMP_BUFFER, 0 );
        _jsCurieScaleBlit(
            tmpId,
            tmpOffset,
            tmpPitch,
            image[i].width, image[i].height,
            srcId,
            srcOffset,
            srcPitch,
            image[i - 1].width, image[i - 1].height,
            bpp,
            GL_TRUE );	// filter on
        _jsAllocClearHint();

        jsGcmSurface tmp = {
                       source:		_JSGCM_SURFACE_SOURCE_TEMPORARY,
                       width:		image[i].width,
                       height:		image[i].height,
                       bpp:		bpp,
                       pitch:		tmpPitch,
                       samples:	1,
                       format:	( jsGcmEnum )image->internalFormat,
                       flip:		GL_FALSE,
                       pool:		_JSGCM_SURFACE_POOL_LINEAR,
                       ppuData:		NULL,
                       dataId:      tmpId,
                       dataIdOffset: tmpOffset,                        
                           };

        jsGcmSurface dst = tmp;
        dst.source = _JSGCM_SURFACE_SOURCE_TEXTURE;
        dst.pitch = gcmTexture->gpuLayout.pitch;
        dst.pool = gcmTexture->pool;

        const GLuint dataOffset = _jsGetGcmImageOffset( &gcmTexture->gpuLayout, face, i );
        dst.dataId = gcmTexture->gpuAddressId;
        dst.dataIdOffset = gcmTexture->gpuAddressIdOffset + dataOffset;

        GLuint offsetHeight = 0;
        if ( dst.pitch )
        {
            // linear (not swizzled)
            //  The tiled linear format requires that render
            //  targets be aligned to 8*pitch from the start of
            //  the tiled region.
            offsetHeight = ( dataOffset / dst.pitch ) % 8;
            dst.height += offsetHeight;
            dst.dataIdOffset -= offsetHeight * dst.pitch;
        }

        _jsGcmCopySurface(
            &tmp, 0, 0,
            &dst, 0, offsetHeight,
            tmp.width, tmp.height,
            GL_TRUE );

        if ( i == 1 )
        {
            srcId = tmpId;
            srcOffset = tmpOffset;
            srcPitch = tmpPitch;
            tmpId = tmpBufferId;
            tmpOffset = gcmTexture->gpuLayout.baseHeight / 2 * tmpPitch;
        }
        else
        {
            GLuint swapId = srcId;
            GLuint swapOffset = srcOffset;
            srcId = tmpId;
            srcOffset = tmpOffset;
            tmpId = swapId;
            tmpOffset = swapOffset;
        }
    }

    _jsGcmFreeTextureMemory( tmpBufferId );

    return GL_TRUE;
}

// Implementation of texture reference
// Associate bufferObject to texture by assigning buffer's gpu address to the gcm texture 
GLboolean _jsPlatformTextureReference( jsTexture *texture, GLuint pitch, jsBufferObject *bufferObject, GLintptr offset )
{
    jsGcmTexture *gcmTexture = ( jsGcmTexture * )texture->platformTexture;

    // XXX check pitch restrictions ?

    GLboolean swizzled = ( pitch == 0 );
    jsGcmTextureLayout newLayout;
    _jsPlatformChooseGPUFormatAndLayout( texture, !swizzled, pitch, &newLayout );


    GLboolean isRenderTarget = GL_FALSE;
    GLboolean vertexEnable = GL_FALSE;
    // can usually be a render target, except for restrictions below
    if ( _jsIsDrawableColorFromat( newLayout.internalFormat ) || _jsIsDrawableDepthFormat( newLayout.internalFormat ) )
    {
        isRenderTarget = GL_TRUE;
    }

    // handle swizzle restrictions
    if ( swizzled )
    {
        if ( newLayout.pitch != 0 )
        {
            _jsSetError( GL_INVALID_OPERATION );
            return GL_FALSE;
        }
        if (( newLayout.pixelBits > 32 ) || ( newLayout.baseWidth < 16 || newLayout.baseHeight < 16 ) )
        {
            isRenderTarget = GL_FALSE;
        }
    }
    else
    {
        JS_ASSERT( newLayout.pitch == pitch );
        switch ( newLayout.internalFormat )
        {
            case GL_FLOAT_RGBA32:
            case GL_RGBA32F_ARB:
            case GL_LUMINANCE32F_ARB:
                vertexEnable = GL_TRUE;
                break;
            default:
                break;
        }
    }

    texture->isRenderTarget = isRenderTarget;
    texture->vertexEnable = vertexEnable;


    if ( gcmTexture->gpuAddressId != GMM_ERROR )
    {
        _jsPlatformDestroyTexture( texture );
    }

    jsGcmBufferObject *gcmBuffer = ( jsGcmBufferObject * ) & bufferObject->platformBufferObject;

    gcmTexture->gpuLayout = newLayout;
    if ( gcmBuffer->pool == _JSGCM_SURFACE_POOL_SYSTEM_TILED_DEPTH ||
         gcmBuffer->pool == _JSGCM_SURFACE_POOL_SYSTEM_TILED_COLOR )
    {
        gcmTexture->pool = _JSGCM_SURFACE_POOL_SYSTEM;
    }
    else
    {
        gcmTexture->pool = gcmBuffer->pool;
    }
    gcmTexture->gpuAddressId = gcmBuffer->bufferId;
    gcmTexture->gpuAddressIdOffset = offset;
    gcmTexture->gpuSize = _jsGetGcmTextureSize( &newLayout );

    texture->revalidate &= ~( _JS_TEXTURE_REVALIDATE_LAYOUT | _JS_TEXTURE_REVALIDATE_IMAGES );
    texture->revalidate |= _JS_TEXTURE_REVALIDATE_PARAMETERS;
    _jsTextureTouchFBOs( texture );
    return GL_TRUE;
}


#ifdef JS_DPM
///// These are used by the DPM save/load functions.

static void matchGPUAddressToPBO( jsGcmTexture * platformTexture, jsTexNameSpace * bufferObjectNameSpace,
                                  void ** gpuAddress, GLuint * gpuSize, GLuint * pboName )
{
    *gpuAddress = NULL;
    *gpuSize = 0;
    *pboName = 0;

    int	i;
    for ( i = 0; i < ( int )bufferObjectNameSpace->capacity; ++i )
    {
        jsBufferObject *	bufferObject = ( jsBufferObject * )bufferObjectNameSpace->data[i];
        if ( bufferObject == NULL )	continue;
        if ( bufferObject == platformTexture->pbo )
        {
            *pboName = i;
            *gpuSize = platformTexture->gpuSize;
            jsGcmBufferObject * platformBufferObject = ( jsGcmBufferObject * ) platformTexture->pbo;
            unsigned long int offset = platformTexture->gpuAddress - platformBufferObject->buffer; // XXX GPUPTR
            *gpuAddress = ( void * ) offset; // XXX GPUPTR
            return;
        }
    }	// for
    JS_ASSERT( 0 );
}


int	_jsSavePlatformTextureAndUpdate( void * pt, jsTexNameSpace * bufferObjectNameSpace, void * saveArea,
                                     unsigned long int * offset, unsigned long int offsetMax )
{
    void *	gpuAddress;
    GLuint	gpuSize;
    GLuint	pboName;
    jsGcmTexture *	platformTexture = ( jsGcmTexture * ) pt;
    if ( *offset + sizeof( jsGcmTexture ) > offsetMax )  return -1;
    memcpy(( char * )saveArea + *offset, platformTexture, sizeof( jsGcmTexture ) );
    *offset += sizeof( jsGcmTexture );
    if ( platformTexture->pbo == NULL )
    {
        if ( platformTexture->gpuSize > 0 )
        {
            if ( *offset + platformTexture->gpuSize > offsetMax )  return -1;
            fast_memcpy(( char * )saveArea + *offset, _jsGcmMapMemory( platformTexture->gpuAddress ), platformTexture->gpuSize ); // XXX GPUPTR
            *offset += platformTexture->gpuSize;
        }
        return 0;
    }
    matchGPUAddressToPBO( platformTexture, bufferObjectNameSpace, &gpuAddress, &gpuSize, &pboName );
    if ( *offset + sizeof( gpuAddress ) + sizeof( gpuSize ) + sizeof( pboName ) + sizeof( jsGcmTextureLayout ) > offsetMax )	return -1;
    memcpy(( char * )saveArea + *offset, &gpuAddress, sizeof( gpuAddress ) );
    *offset += sizeof( gpuAddress );
    memcpy(( char * )saveArea + *offset, &gpuSize, sizeof( gpuSize ) );
    *offset += sizeof( gpuSize );
    memcpy(( char * )saveArea + *offset, &pboName, sizeof( pboName ) );
    *offset += sizeof( pboName );
    memcpy(( char * )saveArea + *offset, &platformTexture->gpuLayout, sizeof( jsGcmTextureLayout ) );
    *offset += sizeof( jsGcmTextureLayout );
    return 0;
}


static void matchPBOtoGPUAddress( GLuint pboName, jsTexNameSpace * bufferObjectNameSpace, void * gpuAddress, GLuint gpuSize, jsGcmTexture * platformTexture )
{
    platformTexture->gpuSize = gpuSize;
    jsBufferObject *	pbo = ( jsBufferObject * )bufferObjectNameSpace->data[pboName];
    JS_ASSERT( pbo );
    platformTexture->pbo = pbo;
    jsGcmBufferObject *	nvpbo = ( jsGcmBufferObject * ) pbo->platformBufferObject;
    platformTexture->gpuAddress = nvpbo->buffer + ( unsigned long int ) gpuAddress; // XXX GPUPTR
}


int	_jsRestorePlatformTextureAndUpdate( void * pt, jsTexNameSpace * bufferObjectNameSpace, void * saveArea, unsigned long int * offset )
{
    void *	gpuAddress;
    GLuint	gpuSize;
    GLuint	pboName;
    jsGcmTexture *	platformTexture = ( jsGcmTexture * ) pt;
    memcpy( platformTexture, ( const char * )saveArea + *offset, sizeof( jsGcmTexture ) );
    *offset += sizeof( jsGcmTexture );
    if ( platformTexture->pbo == NULL )
    {
        if ( platformTexture->gpuSize > 0 )
        {
            fast_memcpy( _jsGcmMapMemory( platformTexture->gpuAddress ), ( const char * )saveArea + *offset, platformTexture->gpuSize ); // XXX GPUPTR
            *offset += platformTexture->gpuSize;
        }
        return 0;
    }
    memcpy( &gpuAddress, ( const char * )saveArea + *offset, sizeof( gpuAddress ) );
    *offset += sizeof( gpuAddress );
    memcpy( &gpuSize, ( const char * )saveArea + *offset, sizeof( gpuSize ) );
    *offset += sizeof( gpuSize );
    memcpy( &pboName, ( const char * )saveArea + *offset, sizeof( pboName ) );
    *offset += sizeof( pboName );
    memcpy( &platformTexture->gpuLayout, ( const char * )saveArea + *offset, sizeof( jsGcmTextureLayout ) );
    *offset += sizeof( jsGcmTextureLayout );
    matchPBOtoGPUAddress( pboName, bufferObjectNameSpace, gpuAddress, gpuSize, platformTexture );
    return 0;
}

#endif


// GlSetRenderTarget implementation starts here

// Render target rt's color and depth buffer parameters are updated with args
// Fifo functions are called as required
void static inline _jsGcmSetColorDepthBuffers( jsGcmRenderTarget *rt, jsGcmRenderTargetEx const * const args )
{
    CellGcmSurface *   grt = &rt->gcmRenderTarget;

    JS_ASSERT( rt->colorBufferCount >= 0 );
    JS_ASSERT( rt->colorBufferCount <= JSGCM_SETRENDERTARGET_MAXCOUNT );
    JS_ASSERT(( rt->colorBufferCount <= 1 ) || ( rt->aaSamples == 1 ) );
    rt->colorBufferCount = args->colorBufferCount;

    // remember rt for swap and clip related functions
    GLuint oldHeight;
    GLuint oldyInverted;

    oldyInverted = rt->yInverted;
    oldHeight = rt->gcmRenderTarget.height;

    if ( rt->colorFormat != ( GLuint )args->colorFormat )
    {
        // ARGB8 and FP16 interpret some registers differently
        jsGcmBlendState *blend = &_jsGcmState.state.blend;
        rt->colorFormat  = args->colorFormat;
        _jsGcmFifoGlBlendColor( blend->r, blend->g, blend->b, blend->a );
        _jsGcmFifoGlAlphaFunc(( jsGcmEnum )blend->alphaFunc, blend->alphaRef );
    }

    GLuint i = 0;

    for ( i = 0; i < args->colorBufferCount; i++ )
    {
        // choose context based on top bit of offset
        if ( args->colorPitch[i] == 0 )
        {
            grt->colorOffset[i] = 0;
            grt->colorPitch[i] = 0x200;
            grt->colorLocation[i] = CELL_GCM_LOCATION_LOCAL;
        }
        else
        {
            if ( args->colorId[i] != GMM_ERROR )
            {
                if ( gmmIdIsMain(args->colorId[i]) )
                {
                    grt->colorLocation[i] = CELL_GCM_LOCATION_MAIN;
                }
                else
                {
                    grt->colorLocation[i] = CELL_GCM_LOCATION_LOCAL;
                }
                grt->colorOffset[i] = gmmIdToOffset(args->colorId[i]) + args->colorIdOffset[i];
                grt->colorPitch[i] = args->colorPitch[i];
            }
        }
    }

    // fill in the other render targets that haven't been set
    for ( ; i < JSGCM_SETRENDERTARGET_MAXCOUNT; i++ )
    {
        grt->colorOffset[i] = grt->colorOffset[0];
        grt->colorPitch[i]   = grt->colorPitch[0];
        grt->colorLocation[i] = grt->colorLocation[0];
    }

    rt->yInverted = args->yInverted;
    rt->swizzled  = args->swizzled;
    rt->aaSamples = args->aaSamples;
    grt->x        = args->xOffset; 
    grt->y        = args->yOffset; 
    grt->width    = args->width;
    grt->height   = args->height;

    // figure out hw parameters given the requested parameters
    JS_ASSERT(( grt->width > 0 ) && ( grt->x + grt->width <= JSGCM_MAX_RT_DIMENSION ) &&
              ( grt->height > 0 ) && ( grt->y + grt->height <= JSGCM_MAX_RT_DIMENSION ) );

    
    // scissor enabled/viewport and height changed ? obey yInverted
    if (( grt->height != oldHeight ) | ( rt->yInverted != oldyInverted ) )
    {
		jsGcmViewportState *v = &_jsGcmState.state.viewport;
        jsGcmScissorState *s = &_jsGcmState.state.scissor;
        _jsGcmFifoGlViewport( v->x, v->y, v->w, v->h );
        _jsGcmFifoGlFrontFace(( jsGcmEnum )v->frontFace );
        _jsGcmFifoGlScissor( s->x, s->y, s->w, s->h );
    }

    rt->depthFormat = args->depthFormat;

    // init depth and color locations and offsets
    if ( args->depthId != GMM_ERROR )
    {
        if ( gmmIdIsMain(args->depthId) )
        {
            grt->depthLocation = CELL_GCM_LOCATION_MAIN;
        }
        else
        {
            grt->depthLocation = CELL_GCM_LOCATION_LOCAL;
        }
        grt->depthOffset = gmmIdToOffset(args->depthId) + args->depthIdOffset;
        grt->depthPitch = args->depthPitch;
    }
}


// Update rt's color and depth format with args
void static inline _jsGcmSetColorDepthFormats( jsGcmRenderTarget *rt, jsGcmRenderTargetEx const * const args )
{
    CellGcmSurface *   grt = &rt->gcmRenderTarget;

    // set the color format
    switch ( args->colorFormat )
    {
        case JSGCM_NONE:
            // choose a fake format
            // but choose a 16-bit format if depth is 16-bit
            if ( args->depthFormat == JSGCM_DEPTH_COMPONENT16 )
            {
                grt->colorFormat = CELL_GCM_SURFACE_R5G6B5;
            }
            else
            {
                grt->colorFormat = CELL_GCM_SURFACE_A8R8G8B8;
            }
            for ( int i = 0; i < JSGCM_SETRENDERTARGET_MAXCOUNT; i++ )
            {
                grt->colorOffset[i] = 0;
                grt->colorPitch[i] = args->depthPitch;
            }
            break;
        case JSGCM_ARGB8:
            grt->colorFormat = CELL_GCM_SURFACE_A8R8G8B8;
            break;
        case JSGCM_FLOAT_RGBX16:  // note: the floats are order as AGBR
        case JSGCM_FLOAT_RGBA16:  // note: the floats are order as AGBR
            grt->colorFormat = CELL_GCM_SURFACE_F_W16Z16Y16X16;
            JS_ASSERT( rt->aaSamples == 1 );// no AA with floating point colorbuffers
            break;
        case JSGCM_FLOAT_R32:
        case JSGCM_LUMINANCE32F_ARB:
            grt->colorFormat = CELL_GCM_SURFACE_F_X32;
            JS_ASSERT( rt->aaSamples == 1 );// no AA with floating point colorbuffers
            break;
        case JSGCM_FLOAT_RGBX32:  // note: the floats are order as AGBR
        case JSGCM_FLOAT_RGBA32: // note: the floats are order as AGBR
            grt->colorFormat = CELL_GCM_SURFACE_F_W32Z32Y32X32; // GCM_PORT_UNTESTED [KHOFF]
            JS_ASSERT( rt->aaSamples == 1 );// no AA with floating point colorbuffers
            break;
        default:
            JS_ASSERT( 0 );
            break;
    }

    // set the depth format
    switch ( args->depthFormat )
    {
        case JSGCM_NONE:
            // choose a fake format
            grt->depthFormat = CELL_GCM_SURFACE_Z24S8;
            grt->depthLocation = CELL_GCM_LOCATION_LOCAL;
            grt->depthOffset = 0;
            grt->depthPitch = 64;
            break;
        case JSGCM_DEPTH_COMPONENT16:
            grt->depthFormat = CELL_GCM_SURFACE_Z16;
            break;
        case JSGCM_DEPTH_COMPONENT24:
            grt->depthFormat = CELL_GCM_SURFACE_Z24S8;
            break;
        default:
            JS_ASSERT( 0 );
            break;
    }
}

// Update rt's AA and Swizzling parameters with args
void static inline _jsGcmSetAASwizzle( jsGcmRenderTarget *rt, jsGcmRenderTargetEx const * const args )
{
    CellGcmSurface *   grt = &rt->gcmRenderTarget;

    // set up aa
    switch ( args->aaSamples )
    {
        case 1:
            grt->antialias = CELL_GCM_SURFACE_CENTER_1;
            break;
        case 2:
            grt->antialias = CELL_GCM_SURFACE_DIAGONAL_CENTERED_2;
            break;
        case 3:
            grt->antialias = CELL_GCM_SURFACE_SQUARE_CENTERED_4;
            break;
        case 4:
            grt->antialias = CELL_GCM_SURFACE_SQUARE_ROTATED_4;
            break;
        default:
            JS_ASSERT( 0 );
            break;
    }

	GCM_FUNC( cellGcmSetAntiAliasingControl, 
		(rt->aaSamples > 1) ? CELL_GCM_TRUE : CELL_GCM_FALSE, 
		CELL_GCM_FALSE,
		CELL_GCM_FALSE, 
		0xFFFF);

    // set swizzled
    if ( args->swizzled )
    {
        JS_ASSERT( JSGCM_IS_POW2( grt->width ) && JSGCM_IS_POW2( grt->height ) );

        grt->type = CELL_GCM_SURFACE_SWIZZLE;
        // make hw happy
        grt->depthPitch = grt->width * 4;
        for ( int i = 0; i < JSGCM_SETRENDERTARGET_MAXCOUNT; i++ )
        {
            grt->colorPitch[i] = grt->depthPitch;
        }
    }
    else
    {
        grt->type = CELL_GCM_SURFACE_PITCH;

        for ( int i = 0; i < JSGCM_SETRENDERTARGET_MAXCOUNT; i++ )
        {
            JS_ASSERT( grt->colorPitch[i] <= 0xffff );
        }
    }
}

// Update rt's color targets
void static inline _jsGcmSetTarget( jsGcmRenderTarget *rt, jsGcmRenderTargetEx const * const args )
{
    CellGcmSurface *   grt = &rt->gcmRenderTarget;

    // set target combo
    switch ( rt->colorBufferCount )
    {
        case 0:
            grt->colorTarget = CELL_GCM_SURFACE_TARGET_NONE;
            break;
        case 1:
            grt->colorTarget = CELL_GCM_SURFACE_TARGET_1;
            break;
        case 2:
            grt->colorTarget = CELL_GCM_SURFACE_TARGET_MRT1;
            break;
        case 3:
            grt->colorTarget = CELL_GCM_SURFACE_TARGET_MRT2;
            break;
        case 4:
            grt->colorTarget = CELL_GCM_SURFACE_TARGET_MRT3;
            break;
        default:
            JS_ASSERT( 0 );
            break;
    }
}

// Set current render target to args
void _jsGcmFifoGlSetRenderTarget( jsGcmRenderTargetEx const * const args )
{
    jsGcmRenderTarget *rt = &_jsGcmState.renderTarget;
    CellGcmSurface *   grt = &_jsGcmState.renderTarget.gcmRenderTarget;

    _jsGcmSetColorDepthBuffers( rt, args );
    _jsGcmSetColorDepthFormats( rt, args );
    _jsGcmSetAASwizzle( rt, args );
    _jsGcmSetTarget( rt, args );

	// ensure if either width or height is 1 the other is one as well
	if ( grt->width == 1 )
		grt->height = 1; 
	else
		if ( grt->height == 1 )
			grt->width = 1; 

    GCM_FUNC( cellGcmSetSurface, grt );

    _jsGcmFifoGlColorMask( rt->writeR, rt->writeG, rt->writeB, rt->writeA );
    if ( rt->depthTest )
    {
        _jsGcmFifoGlEnable( JSGCM_DEPTH_TEST );
    }
    else
    {
        _jsGcmFifoGlDisable( JSGCM_DEPTH_TEST );
    }

    // pick up the most recent pb/mf values
    _jsGcmUpdateZCullFeedbackParams();
}
