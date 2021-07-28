/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 *                Copyright (C) 2009 Sony Computer Entertainment Inc.
 *                                               All Rights Reserved.
 */

 
/*
 * 
 */

#include <PSGL/psgl.h>
#include <PSGL/private.h>
#include <PSGL/Debug.h>
#include <PSGL/Utils.h>
#include <JSGCM/JSGcmUtils.h>
#include <JSGCM/JSGcmFifoUtils.h>
#include <JSGCM/JSGcmDevice.h>
#include <PSGL/PerfCounters.h>

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

void _jsCPUSwizzle( char *swizzledTexture, const char*texture, GLuint width, GLuint height, GLuint depth, GLenum internalFormat )
{
    GLenum platformFormat = _jsPlatformChooseInternalFormat( internalFormat );
    if ( platformFormat == GL_INVALID_ENUM ) return;
    GLenum format;
    GLenum type;
    _jsPlatformExpandInternalFormat( platformFormat, &format, &type );

    GLuint bpp = _jsGetPixelSize( format, type );

    _jsGcmSwizzleDataCPU( swizzledTexture, texture,
                          0, 0, 0, width, height, depth, 0, 0, 0, width, height, depth,
                          width, height, depth, _jsLog2( bpp ), bpp,
                          0,		// border
                          ( depth == 1 ) ? 3 : 2,		// dimension
                          _jsChooseCopyPixel( bpp ) );
}

void _jsGcmCopyGPUToGPU(
    const jsGcmSurface* src,
    GLuint srcX, GLuint srcY,
    const jsGcmSurface* dst,
    GLuint dstX, GLuint dstY,
    GLuint width, GLuint height )
{
    JS_ASSERT( src != NULL );
    JS_ASSERT( src->pool == _JSGCM_SURFACE_POOL_TILED_COLOR ||
               src->pool == _JSGCM_SURFACE_POOL_TILED_DEPTH ||
               src->pool == _JSGCM_SURFACE_POOL_LINEAR ||
               src->pool == _JSGCM_SURFACE_POOL_SYSTEM );
    JS_ASSERT( dst != NULL );
    JS_ASSERT( dst->pool == _JSGCM_SURFACE_POOL_TILED_COLOR ||
               dst->pool == _JSGCM_SURFACE_POOL_TILED_DEPTH ||
               dst->pool == _JSGCM_SURFACE_POOL_LINEAR ||
               dst->pool == _JSGCM_SURFACE_POOL_SYSTEM );
    JS_ASSERT( src->flip == dst->flip );

    JS_ASSERT( srcX + width <= src->width );
    JS_ASSERT( dstX + width <= dst->width );
    JS_ASSERT( srcY + height <= src->height );
    JS_ASSERT( dstY + height <= dst->height );

    // get a real pitch
    //  The pitch member is set to 0 when swizzled.
    const GLuint srcPitch = src->pitch ? src->pitch : src->bpp * src->width;
    const GLuint dstPitch = dst->pitch ? dst->pitch : dst->bpp * dst->width;

    // horrible case if either pitch >=65536. Handle with mem2mem_format
    if (( srcPitch >= 0x10000 ) || ( dstPitch >= 0x10000 ) )
    {
        _jsGcmTransferData( dst->dataId, dst->dataIdOffset+(dstPitch*dstY+dstX*dst->bpp), dstPitch,
                            src->dataId, src->dataIdOffset+(srcPitch*srcY+srcX*src->bpp), srcPitch,
                            width*src->bpp, height );
        return;
    }

    switch ( src->bpp )
    {
        case 1:
            // _jsGcmFifoGlTransferDataVidToVid doesn't support 1 byte/pixel so use
            // 2 bytes/pixel if we can.
            if (( dstX % 2 ) == 0 && ( srcX % 2 ) == 0 && ( width % 2 ) == 0 )
            {
                _jsGcmFifoGlTransferDataVidToVid(
                    dst->dataId, dst->dataIdOffset, dstPitch, dstX / 2, dstY, 
                    src->dataId, src->dataIdOffset, srcPitch, srcX / 2, srcY, 
                    width / 2, height, 2 );
            }
            else
            {
                _jsGcmTransferData( dst->dataId, dst->dataIdOffset+(dstPitch*dstY+dstX*dst->bpp), dstPitch,
                                    src->dataId, src->dataIdOffset+(srcPitch*srcY+srcX*src->bpp), srcPitch,
                                    width*src->bpp, height );
            }
            break;
        case 2:
        case 4:
            _jsGcmFifoGlTransferDataVidToVid( dst->dataId, dst->dataIdOffset, dstPitch, dstX, dstY,
                                              src->dataId, src->dataIdOffset, srcPitch, srcX, srcY, 
                                              width, height, src->bpp );
            break;
        case 8:
        case 16:
            _jsGcmFifoGlTransferDataVidToVid( dst->dataId, dst->dataIdOffset, dstPitch, dstX*4, dstY,
                                              src->dataId, src->dataIdOffset, srcPitch, srcX*4, srcY, 
                                              width*4, height, src->bpp / 4 );
            break;
    }
}

// upright or flipped
void _jsGcmCopyGPUToPPU(
    const jsGcmSurface* src,
    GLuint srcX, GLuint srcY,
    const jsGcmSurface* dst,
    GLuint dstX, GLuint dstY,
    GLuint width, GLuint height )
{
    JS_ASSERT( src != NULL );
    JS_ASSERT( src->pool == _JSGCM_SURFACE_POOL_TILED_COLOR ||
               src->pool == _JSGCM_SURFACE_POOL_TILED_DEPTH ||
               src->pool == _JSGCM_SURFACE_POOL_LINEAR );
    JS_ASSERT( dst != NULL );
    JS_ASSERT( dst->pool == _JSGCM_SURFACE_POOL_PPU );

    JS_ASSERT( srcX + width <= src->width );
    JS_ASSERT( dstX + width <= dst->width );
    JS_ASSERT( srcY + height <= src->height );
    JS_ASSERT( dstY + height <= dst->height );

    // get a real pitch
    //  The pitch member is set to 0 when swizzled.
    GLuint srcPitch = src->pitch ? src->pitch : src->bpp * src->width;
    GLuint dstPitch = dst->pitch ? dst->pitch : dst->bpp * dst->width;

    if ( srcPitch == dstPitch &&
            src->flip == dst->flip &&
            width == MIN( src->width, dst->width ) )
    {
        _jsGcmFifoGlFinish();
        fast_memcpy(
            dst->ppuData + dstY*dstPitch,
            gmmIdToAddress( src->dataId ) + src->dataIdOffset + srcY*srcPitch,
            dstPitch*height );
    }
    else
    {
        if (( src->flip != dst->flip ) && ( srcPitch < 0x10000 ) && ( dstPitch < 0x10000 ) )
        {
            // allocate temporary buffer
            // TODO: Optimize buffer pitch - for full transfers it is better
            // to use the destination pitch so the final transfer is a
            // single copy instead of line by line.
            const GLuint tmpPitch = _jsPad( src->bpp * src->width, 64 );
            GLuint id = _jsGcmAllocateMemory( tmpPitch * src->height );
#ifndef JS_NO_ERROR_CHECK
            if ( id == GMM_ERROR )
            {
                _jsSetError( GL_OUT_OF_MEMORY );
                return;
            }
#endif

            // flip to temporary buffer
            //  TODO: flip only needed portion of buffer
            jsGcmSurface tmp = *src;
            tmp.source = _JSGCM_SURFACE_SOURCE_TEMPORARY;
            tmp.flip = GL_FALSE;
            tmp.pitch = tmpPitch;
            tmp.pool = _JSGCM_SURFACE_POOL_LINEAR;
            tmp.dataId = id;
            tmp.dataIdOffset = 0;
            _jsGcmFlipGPUToGPU( src, 0, 0, &tmp, 0, 0, tmp.width, tmp.height );

            _jsGcmFifoGlFinish();
            if ( src->flip )
            {
                const char* tmpData = gmmIdToAddress( tmp.dataId ) + tmp.dataIdOffset +
                                      srcY * tmp.pitch + srcX * tmp.bpp;
                char* dstData = dst->ppuData + dstY * dstPitch + dstX * dst->bpp;
                const GLuint lineBytes = width * dst->bpp;
                for ( GLuint i = 0; i < height; ++i )
                    fast_memcpy( dstData + i*dstPitch, tmpData + i*tmp.pitch, lineBytes );
            }
            else
                // don't think we need this case
                //  This is source right side up, and destination flipped.
                //  The pointer arguments to fast_memcpy will be different.
                JS_ASSERT( 0 );

            _jsGcmFreeMemory( id );
        }
        else // pitch mismatch, or pitch too high
        {
            // copy line by line
            _jsGcmFifoGlFinish();
            const char* srcData = gmmIdToAddress( src->dataId ) + src->dataIdOffset +
                                  srcY * srcPitch + srcX * src->bpp;
            char* dstData = dst->ppuData + dstY * dstPitch + dstX * dst->bpp;
            const GLuint lineBytes = width * dst->bpp;
            if ( src->flip )
            {
                srcData += ( height - 1 ) * srcPitch;
                srcPitch = -srcPitch;
            }
            if ( dst->flip )
            {
                dstData += ( height - 1 ) * dstPitch;
                dstPitch = -dstPitch;
            }
            for ( GLuint i = 0; i < height; ++i )
                fast_memcpy( dstData + i*dstPitch, srcData + i*srcPitch, lineBytes );
        }
    }
}


// source is flipped, meaning its pitch should be is considered negative,
// and the first pixel is in src->data+src->pitch*(src->height-1).
void _jsGcmFlipGPUToGPU(
    const jsGcmSurface* src,
    GLuint srcX, GLuint srcY,
    const jsGcmSurface* dst,
    GLuint dstX, GLuint dstY,
    GLuint width, GLuint height )
{
    JS_ASSERT( src != NULL );
    JS_ASSERT( src->pool == _JSGCM_SURFACE_POOL_TILED_COLOR ||
               src->pool == _JSGCM_SURFACE_POOL_TILED_DEPTH ||
               src->pool == _JSGCM_SURFACE_POOL_LINEAR ||
               src->pool == _JSGCM_SURFACE_POOL_SYSTEM );
    JS_ASSERT( dst != NULL );
    JS_ASSERT( dst->pool == _JSGCM_SURFACE_POOL_TILED_COLOR ||
               dst->pool == _JSGCM_SURFACE_POOL_TILED_DEPTH ||
               dst->pool == _JSGCM_SURFACE_POOL_LINEAR ||
               dst->pool == _JSGCM_SURFACE_POOL_SYSTEM );

    // won't work for swizzled surface
    JS_ASSERT( src->pitch != 0 );
    JS_ASSERT( dst->pitch != 0 );

    // srcData represents the first byte of the data.
    // The pixel at srcData+(height-1)*srcPitch will
    // be copied at dstData+dstPitch*dstY+bpp*dstX.

    unsigned int srcOffset = ( src->height - srcY - height ) * src->pitch
                             + srcX * src->bpp;

    if (( src->pitch >= 0x10000 ) || ( dst->pitch >= 0x10000 ) )
    {
        _jsGcmTransferData( dst->dataId, dst->dataIdOffset+dstY*dst->pitch + dstX*dst->bpp, dst->pitch,
                            src->dataId, src->dataIdOffset+srcOffset + ( src->pitch )*( height - 1 ), 
                            -src->pitch,
                            width*src->bpp, height );
    }
    else
    {
        _jsGcmTransferDataVidToVidYInv( dst->dataId, dst->pitch, dstX, dstY,
                                        src->dataId, srcOffset, src->pitch, width, height, dst->bpp );
    }
}

void _jsGcmSwizzleGPUToGPU(
    const jsGcmSurface* src,
    GLuint srcX, GLuint srcY,
    const jsGcmSurface* dst,
    GLuint dstX, GLuint dstY,
    GLuint width, GLuint height )
{
    JS_ASSERT( src != NULL );
    JS_ASSERT( src->pool == _JSGCM_SURFACE_POOL_TILED_COLOR ||
               src->pool == _JSGCM_SURFACE_POOL_TILED_DEPTH ||
               src->pool == _JSGCM_SURFACE_POOL_LINEAR ||
               src->pool == _JSGCM_SURFACE_POOL_SYSTEM );
    JS_ASSERT( dst != NULL );
    JS_ASSERT( dst->pool == _JSGCM_SURFACE_POOL_LINEAR ||
               dst->pool == _JSGCM_SURFACE_POOL_SYSTEM );

    // we could handle that one, but it's such a mess
    if (( dst->bpp > 1 ) && ( dst->width >= 4 ) && ( dst-> height >= 2 ) && ( src->pitch < 0x10000 ) )
    {
        _jsGcmSwizzleDataVidToVid( dst->dataId, dst->dataIdOffset, dst->width, dst->height, dstX, dstY,
                                   src->dataId, src->dataIdOffset, src->pitch, srcX, srcY, width, height, dst->bpp );
    }
    else
    {
        // TODO: draw with linear texture into swizzled buffer?
        // This is a slow path, but we don't warn here because it may not be
        // avoidable (e.g. uploading 1-byte texture, or small mipmap
        // levels).
        _jsGcmFifoGlFinish();
        _jsGcmSwizzleDataCPU( gmmIdToAddress( dst->dataId ) + dst->dataIdOffset, 
                              gmmIdToAddress( src->dataId ) + src->dataIdOffset,
                              dstX, dstY, 0, dst->width, dst->height, 1, srcX, srcY, 0, srcX + width, srcY + height, 1,
                              src->pitch / src->bpp, src->height, 1, _jsLog2( dst->bpp ), src->bpp,
                              0,		// border
                              2,		// dimension
                              _jsChooseCopyPixel( src->bpp ) );	// copy function
    }
}

void _jsGcmFlipSwizzleGPUToGPU(
    const jsGcmSurface* src,
    GLuint srcX, GLuint srcY,
    const jsGcmSurface* dst,
    GLuint dstX, GLuint dstY,
    GLuint width, GLuint height )
{
    JS_ASSERT( src != NULL );
    JS_ASSERT( src->pool == _JSGCM_SURFACE_POOL_TILED_COLOR ||
               src->pool == _JSGCM_SURFACE_POOL_TILED_DEPTH ||
               src->pool == _JSGCM_SURFACE_POOL_LINEAR ||
               src->pool == _JSGCM_SURFACE_POOL_SYSTEM );
    JS_ASSERT( dst != NULL );
    JS_ASSERT( dst->pool == _JSGCM_SURFACE_POOL_LINEAR ||
               dst->pool == _JSGCM_SURFACE_POOL_SYSTEM );

    // allocate temporary buffer
    const GLuint tmpPitch = _jsPad( src->bpp * src->width, 64 );
    GLuint id = _jsGcmAllocateMemory( tmpPitch * src->height );
#ifndef JS_NO_ERROR_CHECK
    if ( id == GMM_ERROR )
    {
        _jsSetError( GL_OUT_OF_MEMORY );
        return;
    }
#endif

    // flip to temporary buffer
    //  TODO: flip only needed portion of buffer
    jsGcmSurface tmp = *src;
    tmp.source = _JSGCM_SURFACE_SOURCE_TEMPORARY;
    tmp.flip = GL_FALSE;
    tmp.pitch = tmpPitch;
    tmp.pool = _JSGCM_SURFACE_POOL_LINEAR;
    tmp.dataId = id;
    tmp.dataIdOffset = 0;
    _jsGcmFlipGPUToGPU( src, 0, 0, &tmp, 0, 0, tmp.width, tmp.height );

    // and swizzle
    _jsGcmSwizzleGPUToGPU( &tmp, srcX, srcY, dst, dstX, dstY, width, height );

    _jsGcmFreeMemory( id );
}

static inline void _jsGcmFilterDown( const jsGcmSurface* src, GLuint dstDataId, GLuint dstDataIdOffset, GLuint dstPitch )
{
    GLboolean filter;
    switch ( src->format )
    {
        case JSGCM_ARGB8:
        case JSGCM_FLOAT_RGBA16:
        case JSGCM_FLOAT_RGBX16:
        case JSGCM_FLOAT_RGBA32:
        case JSGCM_FLOAT_RGBX32:
            filter = GL_TRUE;
            break;
        default:
            filter = GL_FALSE;
            break;
    }

    GLboolean scalex = GL_FALSE;
    GLboolean scaley = GL_FALSE;
    switch ( src->samples )
    {
        case 1:
            break;
        case 2:
            scalex = GL_TRUE;
            break;
        case 3:
        case 4:
            scaley = GL_TRUE;
            scalex = GL_TRUE;
            break;
        default:
            JS_ASSERT( 0 );
            break;
    }

    // downsample
    switch ( src->bpp )
    {
        case 1:
        case 2:
        case 4:
            _jsGcmScaleBlit( dstDataId, dstDataIdOffset, dstPitch, src->dataId, src->dataIdOffset, src->pitch, src->width,
                             src->height, src->bpp, scalex ? -1 : 0, scaley ? -1 : 0, filter );
            break;
        case 8:
            _jsCurieScaleBlit(
                dstDataId,		// dst address
                dstDataIdOffset,
                dstPitch,	// dst pitch
                src->width, src->height,
                src->dataId,
                src->dataIdOffset,
                src->pitch,
                src->width*( scalex ? 2 : 1 ), src->height*( scaley ? 2 : 1 ),
                src->bpp,
                filter );
            break;
        default:
            JS_ASSERT( 0 );
            break;
    }
}

void _jsGcmFlipSwizzleDownGPUToGPU(
    const jsGcmSurface* src,
    GLuint srcX, GLuint srcY,
    const jsGcmSurface* dst,
    GLuint dstX, GLuint dstY,
    GLuint width, GLuint height )
{
    JS_ASSERT( src != NULL );
    JS_ASSERT( src->pool == _JSGCM_SURFACE_POOL_TILED_COLOR ||
               src->pool == _JSGCM_SURFACE_POOL_TILED_DEPTH ||
               src->pool == _JSGCM_SURFACE_POOL_LINEAR ||
               src->pool == _JSGCM_SURFACE_POOL_SYSTEM );
    JS_ASSERT( dst != NULL );
    JS_ASSERT( dst->pool == _JSGCM_SURFACE_POOL_LINEAR ||
               dst->pool == _JSGCM_SURFACE_POOL_SYSTEM );

    JS_ASSERT( srcX + width <= src->width );
    JS_ASSERT( dstX + width <= dst->width );
    JS_ASSERT( srcY + height <= src->height );
    JS_ASSERT( dstY + height <= dst->height );

    // allocate temporary buffer
    // TODO: filter only used portion
    const GLuint tmpPitch = _jsPad( src->bpp * src->width, 64 );
    GLuint id = _jsGcmAllocateMemory( tmpPitch * src->height );
#ifndef JS_NO_ERROR_CHECK
    if ( id == GMM_ERROR )
    {
        _jsSetError( GL_OUT_OF_MEMORY );
        return;
    }
#endif

    _jsGcmFilterDown( src, id, 0, tmpPitch );

    // perform the rest of the transform
    jsGcmSurface tmp = *src;
    tmp.source = _JSGCM_SURFACE_SOURCE_TEMPORARY;
    tmp.pool = _JSGCM_SURFACE_POOL_LINEAR;
    tmp.pitch = tmpPitch;
    tmp.samples = 1;
    tmp.dataId = id;
    tmp.dataIdOffset = 0;
    _jsGcmFlipSwizzleGPUToGPU(
        &tmp, srcX, srcY,
        dst, dstX, dstY,
        width, height );

    _jsGcmFreeMemory( id );
}

// downsample, upright or flipped
void _jsGcmCopyDownGPUToPPU(
    const jsGcmSurface* src,
    GLuint srcX, GLuint srcY,
    const jsGcmSurface* dst,
    GLuint dstX, GLuint dstY,
    GLuint width, GLuint height )
{
    JS_ASSERT( src != NULL );
    JS_ASSERT( src->pool == _JSGCM_SURFACE_POOL_TILED_COLOR ||
               src->pool == _JSGCM_SURFACE_POOL_TILED_DEPTH ||
               src->pool == _JSGCM_SURFACE_POOL_LINEAR ||
               src->pool == _JSGCM_SURFACE_POOL_SYSTEM );

    JS_ASSERT( dst != NULL );
    JS_ASSERT( dst->pool == _JSGCM_SURFACE_POOL_PPU );

    JS_ASSERT( srcX + width <= src->width );
    JS_ASSERT( dstX + width <= dst->width );
    JS_ASSERT( srcY + height <= src->height );
    JS_ASSERT( dstY + height <= dst->height );

    // allocate temporary buffer
    // TODO: filter only used portion
    const GLuint tmpPitch = _jsPad( src->bpp * src->width, 64 );
    GLuint id = _jsGcmAllocateMemory( tmpPitch * src->height );
#ifndef JS_NO_ERROR_CHECK
    if ( id == GMM_ERROR )
    {
        _jsSetError( GL_OUT_OF_MEMORY );
        return;
    }
#endif

    _jsGcmFilterDown( src, id, 0, tmpPitch );

    // copy
    jsGcmSurface tmp = *src;
    tmp.source = _JSGCM_SURFACE_SOURCE_TEMPORARY;
    tmp.pool = _JSGCM_SURFACE_POOL_LINEAR;
    tmp.pitch = tmpPitch;
    tmp.samples = 1;
    tmp.dataId = id;
    tmp.dataIdOffset = 0;
    _jsGcmCopyGPUToPPU(
        &tmp, srcX, srcY,
        dst, dstX, dstY,
        width, height );

    _jsGcmFreeMemory( id );
}

// downsample, upright or flipped
void _jsGcmCopyDownGPUToGPU(
    const jsGcmSurface* src,
    GLuint srcX, GLuint srcY,
    const jsGcmSurface* dst,
    GLuint dstX, GLuint dstY,
    GLuint width, GLuint height )
{
    JS_ASSERT( src != NULL );
    JS_ASSERT( src->pool == _JSGCM_SURFACE_POOL_TILED_COLOR ||
               src->pool == _JSGCM_SURFACE_POOL_TILED_DEPTH ||
               src->pool == _JSGCM_SURFACE_POOL_LINEAR ||
               src->pool == _JSGCM_SURFACE_POOL_SYSTEM );

    JS_ASSERT( dst != NULL );
    JS_ASSERT( dst->pool == _JSGCM_SURFACE_POOL_TILED_COLOR ||
               dst->pool == _JSGCM_SURFACE_POOL_TILED_DEPTH ||
               dst->pool == _JSGCM_SURFACE_POOL_LINEAR ||
               dst->pool == _JSGCM_SURFACE_POOL_SYSTEM );

    JS_ASSERT( srcX + width <= src->width );
    JS_ASSERT( dstX + width <= dst->width );
    JS_ASSERT( srcY + height <= src->height );
    JS_ASSERT( dstY + height <= dst->height );


    if (( srcX == 0 ) && ( srcY == 0 ) &&
            ( src->width == width ) && ( src->height == height ) &&
            ( src->flip == dst->flip ) &&
            ( dstX == 0 ) && ( dstY == 0 ) &&
            ( dst->width == width ) && ( dst->height == height )
       )
    {
        // simplest case, to filter down aa FBOs
        // do it in place
        _jsGcmFilterDown( src, dst->dataId, dst->dataIdOffset, dst->pitch );
    }
    else
    {
        // allocate temporary buffer
        const GLuint tmpPitch = _jsPad( src->bpp * src->width, 64 );
        GLuint id = _jsGcmAllocateMemory( tmpPitch * src->height );
#ifndef JS_NO_ERROR_CHECK
        if ( id == GMM_ERROR )
        {
            _jsSetError( GL_OUT_OF_MEMORY );
            return;
        }
#endif

        _jsGcmFilterDown( src, id, 0, tmpPitch );

        // copy
        jsGcmSurface tmp = *src;
        tmp.source = _JSGCM_SURFACE_SOURCE_TEMPORARY;
        tmp.pool = _JSGCM_SURFACE_POOL_LINEAR;
        tmp.pitch = tmpPitch;
        tmp.samples = 1;
        tmp.dataId = id;
        tmp.dataIdOffset = 0;
        if ( tmp.flip == dst->flip )
            _jsGcmCopyGPUToGPU(
                &tmp, srcX, srcY,
                dst, dstX, dstY,
                width, height );
        else if ( tmp.flip && !dst->flip )
            _jsGcmFlipGPUToGPU(
                &tmp, srcX, srcY,
                dst, dstX, dstY,
                width, height );
        else
            // don't think we need other cases
            JS_ASSERT( 0 );

        _jsGcmFreeMemory( id );
    }
}

void _jsGcmCopyPPUToPPU(
    const jsGcmSurface* src,
    GLuint srcX, GLuint srcY,
    const jsGcmSurface* dst,
    GLuint dstX, GLuint dstY,
    GLuint width, GLuint height )
{
    JS_ASSERT( src != NULL );
    JS_ASSERT( src->pool == _JSGCM_SURFACE_POOL_PPU );

    JS_ASSERT( dst != NULL );
    JS_ASSERT( dst->pool == _JSGCM_SURFACE_POOL_PPU );

    JS_ASSERT( srcX + width <= src->width );
    JS_ASSERT( dstX + width <= dst->width );
    JS_ASSERT( srcY + height <= src->height );
    JS_ASSERT( dstY + height <= dst->height );

    if ( src->pitch == dst->pitch &&
            src->width == dst->width &&
            width == src->width )
    {
        // one copy fast path
        fast_memcpy(
            dst->ppuData + dstY*dst->pitch,
            src->ppuData + srcY*src->pitch,
            height*dst->pitch );
    }
    else
    {
        const char* sptr = src->ppuData + srcY * src->pitch + srcX * src->bpp;
        char* dptr = dst->ppuData + dstY * dst->pitch + dstX * dst->bpp;
        const size_t nBytes = width * dst->bpp;
        for ( GLuint i = 0; i < height; ++i )
        {
            fast_memcpy( dptr, sptr, nBytes );
            sptr += src->pitch;
            dptr += dst->pitch;
        }
    }
}

void _jsGcmCopyPPUToSystem(
    const jsGcmSurface* src,
    GLuint srcX, GLuint srcY,
    const jsGcmSurface* dst,
    GLuint dstX, GLuint dstY,
    GLuint width, GLuint height )
{
    JS_ASSERT( src != NULL );
    JS_ASSERT( src->pool == _JSGCM_SURFACE_POOL_PPU );

    JS_ASSERT( dst != NULL );
    JS_ASSERT( dst->pool == _JSGCM_SURFACE_POOL_SYSTEM );

    JS_ASSERT( srcX + width <= src->width );
    JS_ASSERT( dstX + width <= dst->width );
    JS_ASSERT( srcY + height <= src->height );
    JS_ASSERT( dstY + height <= dst->height );

    // convert system offset to EA
    jsGcmSurface tmp = *dst;
    tmp.pool = _JSGCM_SURFACE_POOL_PPU;
    tmp.ppuData = gmmIdToAddress( dst->dataId ) + dst->dataIdOffset;

    // invoke generic EA copy

    _jsGcmFifoGlFinish();
    _jsGcmCopyPPUToPPU(
        src, srcX, srcY,
        &tmp, dstX, dstY,
        width, height );
}

void _jsGcmCopySystemToPPU(
    const jsGcmSurface* src,
    GLuint srcX, GLuint srcY,
    const jsGcmSurface* dst,
    GLuint dstX, GLuint dstY,
    GLuint width, GLuint height )
{
    JS_ASSERT( src != NULL );
    JS_ASSERT( src->pool == _JSGCM_SURFACE_POOL_SYSTEM );

    JS_ASSERT( dst != NULL );
    JS_ASSERT( dst->pool == _JSGCM_SURFACE_POOL_PPU );

    JS_ASSERT( srcX + width <= src->width );
    JS_ASSERT( dstX + width <= dst->width );
    JS_ASSERT( srcY + height <= src->height );
    JS_ASSERT( dstY + height <= dst->height );

    // convert system offset to EA
    jsGcmSurface tmp = *src;
    tmp.pool = _JSGCM_SURFACE_POOL_PPU;
    tmp.ppuData = gmmIdToAddress( src->dataId ) + src->dataIdOffset;

    // invoke generic EA copy
    _jsGcmFifoGlFinish();
    _jsGcmCopyPPUToPPU( &tmp, srcX, srcY, dst, dstX, dstY, width, height );
}

void _jsGcmSwizzlePPUToPPU(
    const jsGcmSurface* src,
    GLuint srcX, GLuint srcY,
    const jsGcmSurface* dst,
    GLuint dstX, GLuint dstY,
    GLuint width, GLuint height )
{
    JS_ASSERT( src != NULL );
    JS_ASSERT( src->pool == _JSGCM_SURFACE_POOL_PPU );

    JS_ASSERT( dst != NULL );
    JS_ASSERT( dst->pool == _JSGCM_SURFACE_POOL_PPU );

    JS_ASSERT( srcX + width <= src->width );
    JS_ASSERT( dstX + width <= dst->width );
    JS_ASSERT( srcY + height <= src->height );
    JS_ASSERT( dstY + height <= dst->height );

    _jsGcmSwizzleDataCPU( dst->ppuData, src->ppuData, dstX, dstY, 0, dst->width, dst->height, 1,
                          srcX, srcY, 0, srcX + width, srcY + height, 1, src->pitch / src->bpp, src->height, 1,
                          _jsLog2( dst->bpp ), src->bpp, 0,		// border
                          2,		// dimension
                          _jsChooseCopyPixel( src->bpp ) );	// copy function
}

void _jsGcmSwizzlePPUToSystem(
    const jsGcmSurface* src,
    GLuint srcX, GLuint srcY,
    const jsGcmSurface* dst,
    GLuint dstX, GLuint dstY,
    GLuint width, GLuint height )
{
    JS_ASSERT( src != NULL );
    JS_ASSERT( src->pool == _JSGCM_SURFACE_POOL_PPU );

    JS_ASSERT( dst != NULL );
    JS_ASSERT( dst->pool == _JSGCM_SURFACE_POOL_SYSTEM );

    JS_ASSERT( srcX + width <= src->width );
    JS_ASSERT( dstX + width <= dst->width );
    JS_ASSERT( srcY + height <= src->height );
    JS_ASSERT( dstY + height <= dst->height );

    // convert system offset to EA
    jsGcmSurface tmp = *dst;
    tmp.pool = _JSGCM_SURFACE_POOL_PPU;
    tmp.ppuData = gmmIdToAddress( dst->dataId ) + dst->dataIdOffset;

    // invoke generic EA swizzle
    _jsGcmFifoGlFinish();
    _jsGcmSwizzlePPUToPPU(
        src, srcX, srcY,
        &tmp, dstX, dstY,
        width, height );
}

void _jsGcmUnswizzleGPUToPPU(
    const jsGcmSurface* src,
    GLuint srcX, GLuint srcY,
    const jsGcmSurface* dst,
    GLuint dstX, GLuint dstY,
    GLuint width, GLuint height )
{
    JS_ASSERT( src != NULL );
    JS_ASSERT( src->pool == _JSGCM_SURFACE_POOL_LINEAR ||
               src->pool == _JSGCM_SURFACE_POOL_SYSTEM );

    JS_ASSERT( dst != NULL );
    JS_ASSERT( dst->pool == _JSGCM_SURFACE_POOL_PPU );

    JS_ASSERT( srcX + width <= src->width );
    JS_ASSERT( dstX + width <= dst->width );
    JS_ASSERT( srcY + height <= src->height );
    JS_ASSERT( dstY + height <= dst->height );

    _jsGcmFifoGlFinish();
    _jsGcmDeswizzleDataCPU( gmmIdToAddress( src->dataId ) + src->dataIdOffset, 
                            dst->ppuData, srcX, srcY, 0,
                            src->width, src->height, 1, dstX, dstY, 0, dstX + width, dstY + height, 1,
                            dst->width, dst->height, 1, _jsLog2( src->bpp ), dst->bpp,
                            0,		// border
                            2,		// dim
                            _jsChooseCopyPixel( src->bpp ) );	// copy texel func
}

void _jsGcmUnswizzleGPUToGPU(
    const jsGcmSurface* src,
    GLuint srcX, GLuint srcY,
    const jsGcmSurface* dst,
    GLuint dstX, GLuint dstY,
    GLuint width, GLuint height )
{
    JS_ASSERT( src != NULL );
    JS_ASSERT( src->pool == _JSGCM_SURFACE_POOL_LINEAR ||
               src->pool == _JSGCM_SURFACE_POOL_SYSTEM );

    JS_ASSERT( dst != NULL );
    JS_ASSERT( dst->pool == _JSGCM_SURFACE_POOL_TILED_COLOR ||
               dst->pool == _JSGCM_SURFACE_POOL_TILED_DEPTH ||
               dst->pool == _JSGCM_SURFACE_POOL_LINEAR ||
               dst->pool == _JSGCM_SURFACE_POOL_SYSTEM );

    JS_ASSERT( srcX + width <= src->width );
    JS_ASSERT( dstX + width <= dst->width );
    JS_ASSERT( srcY + height <= src->height );
    JS_ASSERT( dstY + height <= dst->height );

    _JS_REPORT_EXTRA( PSGL_REPORT_COPY_TEXTURE_WARNING, "Reading texture with GL_TEXTURE_SWIZZLED_GPU_SCE or GL_TEXTURE_SWIZZLED_SYSTEM_SCE allocation will use the slow path." );
    _jsGcmFifoGlFinish();

    // create temporary buffer
    //  Needed for tiled memory destination.  Do it for all cases since this
    //  is not a recommended path anyway.
    jsGcmSurface tmp = *src;
    tmp.source	= _JSGCM_SURFACE_SOURCE_TEMPORARY;
    tmp.width	= width;
    tmp.height	= height;
    tmp.pitch	= _jsPad( tmp.bpp * tmp.width, 4 );
    tmp.samples	= 1;
    tmp.flip	= GL_FALSE;
    tmp.pool	= _JSGCM_SURFACE_POOL_LINEAR;
    tmp.dataId	= _jsGcmAllocateMemory( tmp.pitch * height );
    tmp.dataIdOffset = 0;
#ifndef JS_NO_ERROR_CHECK
    if ( tmp.dataId == GMM_ERROR )
    {
        _jsSetError( GL_OUT_OF_MEMORY );
        return;
    }
#endif

    // unswizzle to temporary buffer
    _jsGcmDeswizzleDataCPU( gmmIdToAddress( src->dataId ) + src->dataIdOffset, 
                            gmmIdToAddress( tmp.dataId ) + tmp.dataIdOffset,
                            srcX, srcY, 0, src->width, src->height, 1, 0, 0, 0, width, height, 1, width, height, 1,
                            _jsLog2( src->bpp ), tmp.bpp, 0,		// border
                            2,		// dim
                            _jsChooseCopyPixel( src->bpp ) );	// copy texel func

    // copy to destination
    _jsGcmCopyGPUToGPU(
        &tmp, 0, 0,
        dst, dstX, dstY,
        width, height );

    // free temporary buffer
    _jsGcmFreeMemory( tmp.dataId );
}

void _jsGcmCrossSwizzleGPUToGPU(
    const jsGcmSurface* src,
    GLuint srcX, GLuint srcY,
    const jsGcmSurface* dst,
    GLuint dstX, GLuint dstY,
    GLuint width, GLuint height )
{
    JS_ASSERT( src != NULL );
    JS_ASSERT( src->pool == _JSGCM_SURFACE_POOL_LINEAR ||
               src->pool == _JSGCM_SURFACE_POOL_SYSTEM );

    JS_ASSERT( dst != NULL );
    JS_ASSERT( dst->pool == _JSGCM_SURFACE_POOL_LINEAR ||
               dst->pool == _JSGCM_SURFACE_POOL_SYSTEM );

    JS_ASSERT( srcX + width <= src->width );
    JS_ASSERT( dstX + width <= dst->width );
    JS_ASSERT( srcY + height <= src->height );
    JS_ASSERT( dstY + height <= dst->height );

    _JS_REPORT_EXTRA( PSGL_REPORT_COPY_TEXTURE_WARNING, "Reading texture with GL_TEXTURE_SWIZZLED_GPU_SCE or GL_TEXTURE_SWIZZLED_SYSTEM_SCE allocation will use the slow path." );
    _jsGcmFifoGlFinish();

    // create temporary buffer
    jsGcmSurface tmp = *src;
    tmp.source	= _JSGCM_SURFACE_SOURCE_TEMPORARY;
    tmp.width	= width;
    tmp.height	= height;
    tmp.pitch	= _jsPad( tmp.bpp * tmp.width, 4 );
    tmp.samples	= 1;
    tmp.flip	= GL_FALSE;
    tmp.pool	= _JSGCM_SURFACE_POOL_LINEAR;
    tmp.dataId	= _jsGcmAllocateMemory( tmp.pitch * height );
    tmp.dataIdOffset = 0;
#ifndef JS_NO_ERROR_CHECK
    if ( tmp.dataId == GMM_ERROR )
    {
        _jsSetError( GL_OUT_OF_MEMORY );
        return;
    }
#endif

    // unswizzle to temporary buffer
    _jsGcmDeswizzleDataCPU( gmmIdToAddress( src->dataId ) + src->dataIdOffset, 
                            gmmIdToAddress( tmp.dataId ) + tmp.dataIdOffset,
                            srcX, srcY, 0, src->width, src->height, 1, 0, 0, 0, width, height, 1,
                            width, height, 1, _jsLog2( src->bpp ), tmp.bpp, 0,		// border
                            2,		// dim
                            _jsChooseCopyPixel( src->bpp ) );	// copy texel func

    // swizzle to destination
    _jsGcmSwizzleGPUToGPU( &tmp, 0, 0, dst, dstX, dstY, width, height );

    // free temporary buffer
    _jsGcmFreeMemory( tmp.dataId );
}

#define _JSGCM_COPY_SRC_GPU			(1 << 0)
#define _JSGCM_COPY_SRC_SYSTEM		(1 << 1)
#define _JSGCM_COPY_SRC_PPU		    (1 << 2)
#define _JSGCM_COPY_DST_GPU			(1 << 3)
#define _JSGCM_COPY_DST_SYSTEM		(1 << 4)
#define _JSGCM_COPY_DST_PPU		    (1 << 5)
#define _JSGCM_COPY_SRC_SWIZZLE		(1 << 6)
#define _JSGCM_COPY_DST_SWIZZLE		(1 << 7)
#define _JSGCM_COPY_FILTER_DOWN		(1 << 8)
#define _JSGCM_COPY_FILTER_UP		(1 << 9)
#define _JSGCM_COPY_FLIP			(1 << 10)

// This utility function copies whole 2D regions described with the
// jsGcmSurface struct.
void _jsGcmCopySurface(
    const jsGcmSurface* src,
    GLuint srcX, GLuint srcY,
    const jsGcmSurface* dst,
    GLuint dstX, GLuint dstY,
    GLuint width, GLuint height,
    GLboolean writeSync )	// don't overwrite dst directly (not used yet)
{
    JS_ASSERT( src != NULL );
    JS_ASSERT( dst != NULL );
    JS_ASSERT( srcX + width <= src->width );
    JS_ASSERT( dstX + width <= dst->width );
    JS_ASSERT( srcY + height <= src->height );
    JS_ASSERT( dstY + height <= dst->height );
    JS_ASSERT( src->bpp == dst->bpp );

    // set up function hash
    GLuint key = 0;

    // source memory pool
    //  SYSTEM memory is accessible by the GPU
    //  PPU memory is any EA
    switch ( src->pool )
    {
        case _JSGCM_SURFACE_POOL_TILED_COLOR:
        case _JSGCM_SURFACE_POOL_TILED_DEPTH:
        case _JSGCM_SURFACE_POOL_LINEAR:
            key |= _JSGCM_COPY_SRC_GPU;
            break;
        case _JSGCM_SURFACE_POOL_SYSTEM:
            key |= _JSGCM_COPY_SRC_SYSTEM;
            break;
        case _JSGCM_SURFACE_POOL_PPU:
            key |= _JSGCM_COPY_SRC_PPU;
            break;
        default:
            JS_ASSERT( 0 );
    }

    // destination memory pool
    //  SYSTEM memory is accessible by the GPU
    //  PPU memory is any EA
    switch ( dst->pool )
    {
        case _JSGCM_SURFACE_POOL_TILED_COLOR:
        case _JSGCM_SURFACE_POOL_TILED_DEPTH:
        case _JSGCM_SURFACE_POOL_LINEAR:
            key |= _JSGCM_COPY_DST_GPU;
            break;
        case _JSGCM_SURFACE_POOL_SYSTEM:
            key |= _JSGCM_COPY_DST_SYSTEM;
            break;
        case _JSGCM_SURFACE_POOL_PPU:
            key |= _JSGCM_COPY_DST_PPU;
            break;
        default:
            JS_ASSERT( 0 );
    }

    // swizzling
    if ( src->pitch == 0 )
        key |= _JSGCM_COPY_SRC_SWIZZLE;
    if ( dst->pitch == 0 )
        key |= _JSGCM_COPY_DST_SWIZZLE;

    // antialiasing
    if ( src->samples > dst->samples )
        key |= _JSGCM_COPY_FILTER_DOWN;
    if ( src->samples < dst->samples )
        key |= _JSGCM_COPY_FILTER_UP;

    // inverted
    if ( src->flip != dst->flip )
        key |= _JSGCM_COPY_FLIP;

    _jsAllocSetHint( _JS_ALLOCTYPE_SURFACE_COPY_TEMP_BUFFER, 0 );

    // find a matching function
    switch ( key )
    {
            // GPU to GPU straight copy
            //  copy texture from user framebuffer
            //  read pixels to PBO from user framebuffer
        case ( _JSGCM_COPY_SRC_GPU | _JSGCM_COPY_DST_GPU ):
                    case ( _JSGCM_COPY_SRC_GPU | _JSGCM_COPY_DST_SYSTEM ):
                        case ( _JSGCM_COPY_SRC_SYSTEM | _JSGCM_COPY_DST_GPU ):
                            case ( _JSGCM_COPY_SRC_SYSTEM | _JSGCM_COPY_DST_SYSTEM ):
                                    _jsGcmCopyGPUToGPU(
                                        src, srcX, srcY,
                                        dst, dstX, dstY,
                                        width, height );
            break;

            // GPU to PPU straight copy or flipped
            //  read pixels
            //  slow copy texture
        case ( _JSGCM_COPY_SRC_GPU | _JSGCM_COPY_DST_PPU ):
                    case ( _JSGCM_COPY_SRC_GPU | _JSGCM_COPY_DST_PPU | _JSGCM_COPY_FLIP ):
                            _jsGcmCopyGPUToPPU(
                                src, srcX, srcY,
                                dst, dstX, dstY,
                                width, height );
            break;

            // GPU to GPU flipped
            //  copy texture from device framebuffer
            //  read pixels to PBO from device framebuffer
        case ( _JSGCM_COPY_SRC_GPU | _JSGCM_COPY_DST_GPU | _JSGCM_COPY_FLIP ):
                    case ( _JSGCM_COPY_SRC_GPU | _JSGCM_COPY_DST_SYSTEM | _JSGCM_COPY_FLIP ):
                        case ( _JSGCM_COPY_SRC_SYSTEM | _JSGCM_COPY_DST_GPU | _JSGCM_COPY_FLIP ):
                                _jsGcmFlipGPUToGPU(
                                    src, srcX, srcY,
                                    dst, dstX, dstY,
                                    width, height );
            break;

            // GPU to GPU swizzled
            //  copy texture from user framebuffer
        case ( _JSGCM_COPY_SRC_GPU | _JSGCM_COPY_DST_GPU | _JSGCM_COPY_DST_SWIZZLE ):
                    case ( _JSGCM_COPY_SRC_GPU | _JSGCM_COPY_DST_SYSTEM | _JSGCM_COPY_DST_SWIZZLE ):
                        case ( _JSGCM_COPY_SRC_SYSTEM | _JSGCM_COPY_DST_GPU | _JSGCM_COPY_DST_SWIZZLE ):
                            case ( _JSGCM_COPY_SRC_SYSTEM | _JSGCM_COPY_DST_SYSTEM | _JSGCM_COPY_DST_SWIZZLE ):
                                    _jsGcmSwizzleGPUToGPU(
                                        src, srcX, srcY,
                                        dst, dstX, dstY,
                                        width, height );
            break;

            // GPU to GPU flipped and swizzled
            //  copy texture from device framebuffer
        case ( _JSGCM_COPY_SRC_GPU | _JSGCM_COPY_DST_GPU | _JSGCM_COPY_FLIP | _JSGCM_COPY_DST_SWIZZLE ):
                    case ( _JSGCM_COPY_SRC_GPU | _JSGCM_COPY_DST_SYSTEM | _JSGCM_COPY_FLIP | _JSGCM_COPY_DST_SWIZZLE ):
                        case ( _JSGCM_COPY_SRC_SYSTEM | _JSGCM_COPY_DST_GPU | _JSGCM_COPY_FLIP | _JSGCM_COPY_DST_SWIZZLE ):
                                _jsGcmFlipSwizzleGPUToGPU(
                                    src, srcX, srcY,
                                    dst, dstX, dstY,
                                    width, height );
            break;

            // GPU to GPU downsampled, flipped and swizzled
            //  copy texture from antialiased device framebuffer
        case ( _JSGCM_COPY_SRC_GPU | _JSGCM_COPY_DST_GPU | _JSGCM_COPY_FILTER_DOWN | _JSGCM_COPY_FLIP | _JSGCM_COPY_DST_SWIZZLE ):
                    case ( _JSGCM_COPY_SRC_GPU | _JSGCM_COPY_DST_SYSTEM | _JSGCM_COPY_FILTER_DOWN | _JSGCM_COPY_FLIP | _JSGCM_COPY_DST_SWIZZLE ):
                            _jsGcmFlipSwizzleDownGPUToGPU(
                                src, srcX, srcY,
                                dst, dstX, dstY,
                                width, height );
            break;

            // GPU to GPU downsampled, copy or flipped
            //  copy texture from antialiased framebuffer
            //  read pixels to PBO from antialiased framebuffer
        case ( _JSGCM_COPY_SRC_GPU | _JSGCM_COPY_DST_GPU | _JSGCM_COPY_FILTER_DOWN ):
                    case ( _JSGCM_COPY_SRC_GPU | _JSGCM_COPY_DST_GPU | _JSGCM_COPY_FILTER_DOWN | _JSGCM_COPY_FLIP ):
                        case ( _JSGCM_COPY_SRC_GPU | _JSGCM_COPY_DST_SYSTEM | _JSGCM_COPY_FILTER_DOWN ):
                            case ( _JSGCM_COPY_SRC_GPU | _JSGCM_COPY_DST_SYSTEM | _JSGCM_COPY_FILTER_DOWN | _JSGCM_COPY_FLIP ):
                                    _jsGcmCopyDownGPUToGPU(
                                        src, srcX, srcY,
                                        dst, dstX, dstY,
                                        width, height );
            break;

            // GPU to PPU downsampled, copy or flipped
            //  read pixels from antialiased framebuffer
        case ( _JSGCM_COPY_SRC_GPU | _JSGCM_COPY_DST_PPU | _JSGCM_COPY_FILTER_DOWN ):
                    case ( _JSGCM_COPY_SRC_GPU | _JSGCM_COPY_DST_PPU | _JSGCM_COPY_FILTER_DOWN | _JSGCM_COPY_FLIP ):
                            _jsGcmCopyDownGPUToPPU(
                                src, srcX, srcY,
                                dst, dstX, dstY,
                                width, height );
            break;

            // PPU to system
            //  upload to host texture
        case ( _JSGCM_COPY_SRC_PPU | _JSGCM_COPY_DST_SYSTEM ):
                        _jsGcmCopyPPUToSystem(
                            src, srcX, srcY,
                            dst, dstX, dstY,
                            width, height );
            break;

            // PPU to system, swizzled
            //  upload to host texture
        case ( _JSGCM_COPY_SRC_PPU | _JSGCM_COPY_DST_SYSTEM | _JSGCM_COPY_DST_SWIZZLE ):
                        _jsGcmSwizzlePPUToSystem(
                            src, srcX, srcY,
                            dst, dstX, dstY,
                            width, height );
            break;

            // system to PPU
            //  read pixels from host framebuffer
        case ( _JSGCM_COPY_SRC_SYSTEM | _JSGCM_COPY_DST_PPU ):
                        _jsGcmCopySystemToPPU(
                            src, srcX, srcY,
                            dst, dstX, dstY,
                            width, height );
            break;

            // swizzled source (GPU, system) to PPU
            //  read pixels from FBO
        case ( _JSGCM_COPY_SRC_GPU | _JSGCM_COPY_DST_PPU | _JSGCM_COPY_SRC_SWIZZLE ):
                    case ( _JSGCM_COPY_SRC_SYSTEM | _JSGCM_COPY_DST_PPU | _JSGCM_COPY_SRC_SWIZZLE ):
                            _jsGcmUnswizzleGPUToPPU(
                                src, srcX, srcY,
                                dst, dstX, dstY,
                                width, height );
            break;

            // swizzled source (GPU, system) to unswizzled destination (GPU, system)
            //  copy texture from FBO
            //  read pixels from FBO to PBO
        case ( _JSGCM_COPY_SRC_GPU | _JSGCM_COPY_DST_GPU | _JSGCM_COPY_SRC_SWIZZLE ):
                    case ( _JSGCM_COPY_SRC_SYSTEM | _JSGCM_COPY_DST_GPU | _JSGCM_COPY_SRC_SWIZZLE ):
                        case ( _JSGCM_COPY_SRC_GPU | _JSGCM_COPY_DST_SYSTEM | _JSGCM_COPY_SRC_SWIZZLE ):
                            case ( _JSGCM_COPY_SRC_SYSTEM | _JSGCM_COPY_DST_SYSTEM | _JSGCM_COPY_SRC_SWIZZLE ):
                                    _jsGcmUnswizzleGPUToGPU(
                                        src, srcX, srcY,
                                        dst, dstX, dstY,
                                        width, height );
            break;

            // swizzled source (GPU, system) to swizzled destination (GPU, system)
            //  copy texture from FBO
        case ( _JSGCM_COPY_SRC_GPU | _JSGCM_COPY_DST_GPU | _JSGCM_COPY_SRC_SWIZZLE | _JSGCM_COPY_DST_SWIZZLE ):
                    case ( _JSGCM_COPY_SRC_SYSTEM | _JSGCM_COPY_DST_GPU | _JSGCM_COPY_SRC_SWIZZLE | _JSGCM_COPY_DST_SWIZZLE ):
                        case ( _JSGCM_COPY_SRC_GPU | _JSGCM_COPY_DST_SYSTEM | _JSGCM_COPY_SRC_SWIZZLE | _JSGCM_COPY_DST_SWIZZLE ):
                            case ( _JSGCM_COPY_SRC_SYSTEM | _JSGCM_COPY_DST_SYSTEM | _JSGCM_COPY_SRC_SWIZZLE | _JSGCM_COPY_DST_SWIZZLE ):
                                    _jsGcmCrossSwizzleGPUToGPU(
                                        src, srcX, srcY,
                                        dst, dstX, dstY,
                                        width, height );
            break;

            //
        default:
            // missing implementation
            JS_ASSERT( 0 );
    }

    _jsAllocClearHint();
}

