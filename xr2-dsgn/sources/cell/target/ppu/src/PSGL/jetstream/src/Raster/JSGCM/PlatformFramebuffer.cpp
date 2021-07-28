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
#include <PSGL/Types.h>
#include <JSGCM/JSGcmUtils.h>
#include <JSGCM/JSGcmDevice.h>
#include <JSGCM/JSGcmDriver.h>
#include <JSGCM/PlatformTypes.h>
#include <JSGCM/JSGcmSetRenderTarget.h>
#include <PSGL/PerfCounters.h>

jsFramebuffer* _jsCreateFramebuffer( void )
{
    jsFramebuffer* framebuffer = new jsPlatformFramebuffer();
#ifndef JS_NO_ERROR_CHECK
    if ( framebuffer == NULL )
    {
        _jsSetError( GL_OUT_OF_MEMORY );
        return NULL;
    }
#endif
    return framebuffer;
}

void _jsDestroyFramebuffer( jsFramebuffer* framebuffer )
{
    JS_ASSERT( framebuffer != NULL );
    delete framebuffer;
}

GLuint _jsPlatformFramebufferColorAttachments( void )
{
    return JSGCM_SETRENDERTARGET_MAXCOUNT;
}

GLuint _jsPlatformRenderbufferMaxSize( void )
{
    return _JS_MAX_TEXTURE_SIZE;
}

void _jsPlatformFramebufferGetParameteriv( GLenum pname, GLint* params )
{
    PSGLcontext *LContext = _CurrentContext;
    JS_ASSERT( LContext );

    if ( LContext->needValidate & PSGL_VALIDATE_FRAMEBUFFER )
        _jsValidateFramebuffer();

    if ( LContext->framebuffer )
    {
        // get the framebuffer struct
        jsFramebuffer* framebuffer = _jsGetFramebuffer( LContext, LContext->framebuffer );
        JS_ASSERT( framebuffer != NULL );
#ifndef JS_NO_ERROR_CHECK
        if ( _jsPlatformFramebufferCheckStatus( framebuffer ) != GL_FRAMEBUFFER_COMPLETE_OES )
        {
            _jsSetError( GL_INVALID_FRAMEBUFFER_OPERATION_OES );
            return;
        }
#endif

        // get a color texture
        jsTexture* colorTexture = NULL;
        GLuint face = 0;
        for ( int i = 0; i < JSGCM_SETRENDERTARGET_MAXCOUNT; ++i )
        {
            // get the texture and face
            _jsFramebufferGetAttachmentTexture( LContext, &framebuffer->color[i], &colorTexture, &face );
            if ( colorTexture != NULL )
                break;
        }

        // get depth texture
        jsTexture* depthTexture = NULL;
        _jsFramebufferGetAttachmentTexture( LContext, &framebuffer->depth, &depthTexture, &face );

        // get stencil texture
        //  A depth texture implicitly provides stencil support.
        jsTexture* stencilTexture = depthTexture;
        if ( stencilTexture == NULL )
            _jsFramebufferGetAttachmentTexture( LContext, &framebuffer->stencil, &stencilTexture, &face );

        switch ( pname )
        {
            case GL_IMPLEMENTATION_COLOR_READ_FORMAT_OES:
                *params = GL_RGBA;
                break;
            case GL_IMPLEMENTATION_COLOR_READ_TYPE_OES:
                *params = GL_UNSIGNED_BYTE;
                break;
            case GL_RED_BITS:
            case GL_GREEN_BITS:
            case GL_BLUE_BITS:
            case GL_ALPHA_BITS:
                if ( colorTexture )
                {
                    JS_ASSERT( colorTexture->images[0] != NULL );
                    switch ( colorTexture->images[0]->internalFormat )
                    {
                        case JSGCM_ARGB8:
                            *params = 8;
                            break;
                        case JSGCM_FLOAT_RGBA16:
                        case JSGCM_FLOAT_RGBX16:
                            *params = 16;
                            break;
                        case JSGCM_FLOAT_RGBA32:
                        case JSGCM_FLOAT_RGBX32:
                            *params = 32;
                            break;
                        default:
                            JS_ASSERT( 0 );
                    }
                }
                else
                    *params = 0;
                break;
            case GL_DEPTH_BITS:
                if ( depthTexture )
                {
                    if ( depthTexture->images[face]->internalFormat == JSGCM_DEPTH_COMPONENT24 )
                    {
                        *params = 24;
                    }
                    else
                    {
                        JS_ASSERT( depthTexture->images[face]->internalFormat == JSGCM_DEPTH_COMPONENT16 );
                        *params = 16;
                    }
                }
                else
                    *params = 0;
                break;
            case GL_STENCIL_BITS:
                if ( stencilTexture )
                    *params = 8;
                else
                    *params = 0;
                break;
            case GL_SUBPIXEL_BITS:
                *params = 4;
                break;
            default:
                JS_ASSERT( 0 );
        }
    }
    else
    {
        jsGcmDevice * gcmDevice = ( jsGcmDevice * )_CurrentDevice->platformDevice;
        switch ( pname )
        {
            case GL_IMPLEMENTATION_COLOR_READ_FORMAT_OES:
                *params = GL_RGBA;
                break;
            case GL_IMPLEMENTATION_COLOR_READ_TYPE_OES:
                *params = GL_UNSIGNED_BYTE;
                break;
            case GL_RED_BITS:
            case GL_GREEN_BITS:
            case GL_BLUE_BITS:
            case GL_ALPHA_BITS:
            {
                switch ( gcmDevice->color[ gcmDevice->drawBuffer].format )
                {
                    case JSGCM_FLOAT_RGBA16:
                        *params = 16;
                        break;
                    case JSGCM_ARGB8:
                        *params = 8;
                        break;
                    default:
                        *params = 0;
                        break;
                }
            }
            break;
            case GL_DEPTH_BITS:
                if ( gcmDevice->depthStencil.pool != _JSGCM_SURFACE_POOL_NONE )
                {
                    if ( gcmDevice->depthStencil.format == JSGCM_DEPTH_COMPONENT24 )
                    {
                        *params = 24;
                    }
                    else
                    {
                        JS_ASSERT( gcmDevice->depthStencil.format == JSGCM_DEPTH_COMPONENT16 );
                        *params = 16;
                    }
                }
                else
                    *params = 0;
                break;
            case GL_STENCIL_BITS:
                if ( gcmDevice->depthStencil.pool != _JSGCM_SURFACE_POOL_NONE )
                {
                    if ( gcmDevice->depthStencil.format == JSGCM_DEPTH_COMPONENT24 )
                    {
                        *params = 8;
                    }
                    else
                    {
                        JS_ASSERT( gcmDevice->depthStencil.format == JSGCM_DEPTH_COMPONENT16 );
                        *params = 0;
                    }
                }
                else
                    *params = 0;
                break;
            case GL_SUBPIXEL_BITS:
                *params = 4;
                break;
            default:
                JS_ASSERT( 0 );
        }
    }
}

GLenum _jsPlatformFramebufferCheckStatus( jsFramebuffer* framebuffer )
{
    JS_ASSERT( framebuffer != NULL );

    PSGLcontext* LContext = _CurrentContext;
    JS_ASSERT( LContext );

    GLuint nBuffers = 0;	// number of attached buffers
    int width = 0;
    int height = 0;
    GLboolean sizeMismatch = GL_FALSE;
    GLboolean depth16 = GL_FALSE;

    // record attached images
    //  We have to verify that no image is attached more than once.  The
    //  array is sized for color attachments plus depth and stencil.
    jsImage* image[_JS_MAX_COLOR_ATTACHMENTS + 2] = {0};

    // tracking swizzling
    //  The attachments must all be swizzled or unswizzled together.
    GLuint nSwizzledBuffers = 0;

    // get depth texture format information
    jsTexture* depthTexture = NULL;
    GLuint depthFace = 0;
    _jsFramebufferGetAttachmentTexture(
        LContext,
        &framebuffer->depth,
        &depthTexture,
        &depthFace );
    if ( depthTexture != NULL &&
         _jsPlatformGetBitsPerPixel(depthTexture->images[depthFace]->internalFormat) == 16 )
    {
        depth16 = GL_TRUE;
    }

    // test colors
    GLuint colorFormat = 0;
    for ( int i = 0; i < _JS_MAX_COLOR_ATTACHMENTS; ++i )
    {
        jsTexture* colorTexture = NULL;
        GLuint colorFace = 0;
        _jsFramebufferGetAttachmentTexture(
            LContext,
            &framebuffer->color[i],
            &colorTexture,
            &colorFace );
        // TODO: Complete texture may not be required.
        if ( colorTexture != NULL )
        {
            if ( !_jsTextureIsValid( colorTexture ) )
            {
                _JS_REPORT_EXTRA( PSGL_REPORT_FRAMEBUFFER_INCOMPLETE, "Framebuffer color attachment texture is not complete" );
                return GL_FRAMEBUFFER_UNSUPPORTED_OES;
            }
            if ( colorTexture->referenceBuffer && !colorTexture->isRenderTarget )
            {
                _JS_REPORT_EXTRA( PSGL_REPORT_FRAMEBUFFER_UNSUPPORTED, "Framebuffer color attachment texture is a reference in a format that cannot be rendered to (swizzled texture, smaller than 16x16 or with more than 32 bits per pixel)" );
                return GL_FRAMEBUFFER_UNSUPPORTED_OES;
            }

            // all attachments must have the same dimensions
            image[nBuffers] = colorTexture->images[colorFace];
            if (( width && width != image[nBuffers]->width ) ||
                    ( height && height != image[nBuffers]->height ) )
                sizeMismatch = GL_TRUE;

            width = image[nBuffers]->width;
            height = image[nBuffers]->height;

            // all color attachments need the same format
            if ( colorFormat && colorFormat != image[nBuffers]->internalFormat )
            {
                _JS_REPORT_EXTRA( PSGL_REPORT_FRAMEBUFFER_INCOMPLETE, "Framebuffer attachments have inconsistent color formats" );
                return GL_FRAMEBUFFER_INCOMPLETE_FORMATS_OES;
            }
            colorFormat = image[nBuffers]->internalFormat;

            // if the user explicitly hinted to swizzle we will inform 
            // them that mixing 16/32bit color/depth with swizzling 
            // is not supported by RSX
            if ( depth16 == GL_TRUE && 
                 ( colorTexture->usage == GL_TEXTURE_SWIZZLED_GPU_SCE ||
                   colorTexture->usage == GL_TEXTURE_SWIZZLED_SYSTEM_SCE ) )
            {
                _JS_REPORT_EXTRA( PSGL_REPORT_FRAMEBUFFER_UNSUPPORTED, "Swizzled is not allowed for 16-bit depth attachment mixed with 32-bit color attachment" );
                return GL_FRAMEBUFFER_UNSUPPORTED_OES;
            }

            // change default usage to GL_TEXTURE_TILED_GPU_SCE if 16-bit depth is used
            if ( depth16 == GL_TRUE && colorTexture->usage == 0 )
            {
                colorTexture->usage = GL_TEXTURE_TILED_GPU_SCE;
            }

            // check for swizzling
            //  (x & (x - 1)) drops the lowest bit.  If the result is 0,
            //  then x is a power of 2.
            if (( width & ( width - 1 ) ) == 0 && ( height & ( height - 1 ) ) == 0 )
            {
                // It's hard to know exactly if the texture is going to be
                // swizzled, since it depends on what memory pool it is
                // allocated from and that may not happen until later.  So
                // this is not an exact test - it only determines if we will
                // try to swizzle this texture.
                if (( colorTexture->usage == 0 ) ||
                        ( colorTexture->usage == GL_TEXTURE_SWIZZLED_GPU_SCE ) )
                    ++nSwizzledBuffers;
            }

            ++nBuffers;
        }
    }

    // check for supported color format
    if ( nBuffers )
    {
        if ( !_jsIsDrawableColorFromat( colorFormat ) )
        {
            _JS_REPORT_EXTRA( PSGL_REPORT_FRAMEBUFFER_UNSUPPORTED, "Color attachment to framebuffer must be a supported drawable format (GL_ARGB_SCE, GL_RGB16F_ARB, GL_RGBA16F_ARB, GL_RGB32F_ARB, GL_RGBA32F_ARB, GL_LUMINANCE32F_ARB)" );
            return GL_FRAMEBUFFER_UNSUPPORTED_OES;
        }
        switch ( colorFormat )
        {
            case JSGCM_ARGB8:
            case JSGCM_RGB5_A1_SCE:
            case JSGCM_RGB565_SCE:
                break;
            case JSGCM_FLOAT_R32:
            case JSGCM_LUMINANCE32F_ARB:
                if ( framebuffer->multisampling != GL_MULTISAMPLING_NONE_SCE )
                {
                    _JS_REPORT_EXTRA( PSGL_REPORT_FRAMEBUFFER_UNSUPPORTED, "Anti-aliasing isn't supported with FP16 or FP32 buffers." );
                    return GL_FRAMEBUFFER_UNSUPPORTED_OES;
                }
                break;
            case JSGCM_FLOAT_RGBX16:
            case JSGCM_FLOAT_RGBA16:
            case JSGCM_FLOAT_RGBX32:
            case JSGCM_FLOAT_RGBA32:
                if ( framebuffer->multisampling != GL_MULTISAMPLING_NONE_SCE )
                {
                    _JS_REPORT_EXTRA( PSGL_REPORT_FRAMEBUFFER_UNSUPPORTED, "Anti-aliasing isn't supported with FP16 or FP32 buffers." );
                    return GL_FRAMEBUFFER_UNSUPPORTED_OES;
                }
                if ( nSwizzledBuffers )
                {
                    _JS_REPORT_EXTRA( PSGL_REPORT_FRAMEBUFFER_UNSUPPORTED, "Texture color attachments greater than 32 bits/texel cannot be swizzled." );
                    return GL_FRAMEBUFFER_UNSUPPORTED_OES;
                }
                break;
            default:
                JS_ASSERT( 0 );
                return GL_FRAMEBUFFER_UNSUPPORTED_OES;
        }
    }

    // test depth
    if ( depthTexture != NULL )
    {
        if ( !_jsTextureIsValid( depthTexture ) )
        {
            _JS_REPORT_EXTRA( PSGL_REPORT_FRAMEBUFFER_INCOMPLETE, "Framebuffer depth attachment texture is not complete" );
            return GL_FRAMEBUFFER_UNSUPPORTED_OES;
        }
        if ( depthTexture->referenceBuffer && !depthTexture->isRenderTarget )
        {
            _JS_REPORT_EXTRA( PSGL_REPORT_FRAMEBUFFER_UNSUPPORTED, "Framebuffer depth attachment texture is a reference in a format that cannot be rendered to (swizzled texture, smaller than 16x16 or with more than 32 bits per pixel)" );
            return GL_FRAMEBUFFER_UNSUPPORTED_OES;
        }

        // all attachments must have the same dimensions
        image[nBuffers] = depthTexture->images[depthFace];
        if (( width && width != image[nBuffers]->width ) ||
                ( height && height != image[nBuffers]->height ) )
            sizeMismatch = GL_TRUE;

        width = image[nBuffers]->width;
        height = image[nBuffers]->height;

        // check for supported depth format
        if ( !_jsIsDrawableDepthFormat( image[nBuffers]->internalFormat ) )
        {
            _JS_REPORT_EXTRA( PSGL_REPORT_FRAMEBUFFER_UNSUPPORTED, "Depth attachment to framebuffer must be a supported drawable format (GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT24)" );
            return GL_FRAMEBUFFER_UNSUPPORTED_OES;
        }

        // if the user explicitly hinted to swizzle we will inform 
        // them that mixing 16/32bit color/depth with swizzling 
        // is not supported by RSX
        if ( colorFormat && depth16 == GL_TRUE && 
             ( depthTexture->usage == GL_TEXTURE_SWIZZLED_GPU_SCE ||
               depthTexture->usage == GL_TEXTURE_SWIZZLED_SYSTEM_SCE ) )
        {
            _JS_REPORT_EXTRA( PSGL_REPORT_FRAMEBUFFER_UNSUPPORTED, "Swizzled is not allowed for 16-bit depth attachment mixed with 32-bit color attachment" );
            return GL_FRAMEBUFFER_UNSUPPORTED_OES;
        }

        // change default usage to GL_TEXTURE_TILED_GPU_SCE if 16-bit depth is used
        // and colorFormat is set. 
        // If colorFormat == 0, there's no color texture. 
        // Then depth can be either swizzled/linear/tiled.
        if ( colorFormat && depth16 == GL_TRUE && depthTexture->usage == 0 )
        {
            depthTexture->usage = GL_TEXTURE_TILED_GPU_SCE;
        }

        // check for swizzling
        //  (x & (x - 1)) drops the lowest bit.  If the result is 0,
        //  then x is a power of 2.
        if (( width & ( width - 1 ) ) == 0 && ( height & ( height - 1 ) ) == 0 )
        {
            // It's hard to know exactly if the texture is going to be
            // swizzled, since it depends on what memory pool it is
            // allocated from and that may not happen until later.  So
            // this is not an exact test - it only determines if we will
            // try to swizzle this texture.
            if (( depthTexture->usage == 0 ) ||
                    ( depthTexture->usage == GL_TEXTURE_SWIZZLED_GPU_SCE ) )
                ++nSwizzledBuffers;
        }

        ++nBuffers;
    }

    // test stencil
    jsTexture* stencilTexture = NULL;
    GLuint stencilFace = 0;
    _jsFramebufferGetAttachmentTexture(
        LContext,
        &framebuffer->stencil,
        &stencilTexture,
        &stencilFace );
    if ( stencilTexture != NULL )
    {
        if ( !_jsTextureIsValid( stencilTexture ) )
        {
            _JS_REPORT_EXTRA( PSGL_REPORT_FRAMEBUFFER_INCOMPLETE, "Framebuffer stencil attachment texture is not complete" );
            return GL_FRAMEBUFFER_UNSUPPORTED_OES;
        }
        if ( stencilTexture->referenceBuffer && !stencilTexture->isRenderTarget )
        {
            _JS_REPORT_EXTRA( PSGL_REPORT_FRAMEBUFFER_UNSUPPORTED, "Framebuffer stencil attachment texture is a reference in a format that cannot be rendered to (swizzled texture, smaller than 16x16 or with more than 32 bits per pixel)" );
            return GL_FRAMEBUFFER_UNSUPPORTED_OES;
        }

        // all attachments must have the same dimensions
        image[nBuffers] = stencilTexture->images[stencilFace];
        if (( width && width != image[nBuffers]->width ) ||
                ( height && height != image[nBuffers]->height ) )
            sizeMismatch = GL_TRUE;

        width = image[nBuffers]->width;
        height = image[nBuffers]->height;

        // check for supported stencil format
        if ( !_jsIsDrawableDepthFormat( image[nBuffers]->internalFormat ) )
        {
            _JS_REPORT_EXTRA( PSGL_REPORT_FRAMEBUFFER_UNSUPPORTED, "Depth attachment to framebuffer must be a supported drawable format (GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT24)" );
            return GL_FRAMEBUFFER_UNSUPPORTED_OES;
        }

        // check for swizzling
        //  (x & (x - 1)) drops the lowest bit.  If the result is 0,
        //  then x is a power of 2.
        if (( width & ( width - 1 ) ) == 0 && ( height & ( height - 1 ) ) == 0 )
        {
            // It's hard to know exactly if the texture is going to be
            // swizzled, since it depends on what memory pool it is
            // allocated from and that may not happen until later.  So
            // this is not an exact test - it only determines if we will
            // try to swizzle this texture.
            if (( stencilTexture->usage == 0 ) ||
                    ( stencilTexture->usage == GL_TEXTURE_SWIZZLED_GPU_SCE ) )
                ++nSwizzledBuffers;
        }

        ++nBuffers;
    }

    // no simultaneous binding to depth and stencil
    //  Depth and stencil are packed into a single buffer, so they can't be
    //  bound independently.  When a 24-bit depth buffer is attached we will
    //  implicitly allow the stencil buffer.
    if ( depthTexture != NULL && stencilTexture != NULL )
    {
        _JS_REPORT_EXTRA( PSGL_REPORT_FRAMEBUFFER_UNSUPPORTED, "Depth and stencil attachments cannot be used simultaneously.  Binding to the depth attachment will provide a stencil buffer." );
        return GL_FRAMEBUFFER_UNSUPPORTED_OES;
    }

    // at least once attachment is required
    if ( nBuffers == 0 )
        return GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_OES;

    // if any buffers are swizzled, all must be swizzled
    if ( nSwizzledBuffers )
    {
        if ( nSwizzledBuffers != nBuffers )
        {
            _JS_REPORT_EXTRA( PSGL_REPORT_FRAMEBUFFER_UNSUPPORTED, "The layout of framebuffer attachments is not compatible.  This can happen when the dimensions are exact powers of two and attachments with different allocation hints are attached.  Renderbuffers have an implicit allocation hint of GL_TEXTURE_TILED_GPU_SCE." );
            return GL_FRAMEBUFFER_UNSUPPORTED_OES;
        }

        if ( sizeMismatch )
        {
            _JS_REPORT_EXTRA( PSGL_REPORT_FRAMEBUFFER_INCOMPLETE, "Framebuffer attachments have inconsistent sizes.  This is not allowed when the texture allocation hint is GL_TEXTURE_SWIZZLED_GPU_SCE (the default) or GL_TEXTURE_SWIZZLED_SYSTEM_SCE." );
            return GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_OES;
        }
    }

    // verify no image is attached more than once
    //  This is an n-squared algorithm - n*log(n) is possible but
    //  probably not necessary (or even faster).
    for ( GLuint i = 0; i < nBuffers; ++i )
        for ( GLuint j = i + 1; j < nBuffers; ++j )
            if ( image[i] == image[j] )
                return GL_FRAMEBUFFER_INCOMPLETE_DUPLICATE_ATTACHMENT_OES;

    return GL_FRAMEBUFFER_COMPLETE_OES;
}

void jsPlatformFramebuffer::validate( PSGLcontext *LContext )
{
    complete = ( _jsPlatformFramebufferCheckStatus( this ) == GL_FRAMEBUFFER_COMPLETE_OES );
    if ( !complete ) return;

    GLuint width = JSGCM_MAX_RT_DIMENSION;
    GLuint height = JSGCM_MAX_RT_DIMENSION;

    GLuint xscale = 1;
    GLuint yscale = 1;
    switch ( multisampling )
    {
        case GL_MULTISAMPLING_2X_DIAGONAL_CENTERED_SCE:
            xscale = 2;
            break;
        case GL_MULTISAMPLING_4X_SQUARE_CENTERED_SCE:
        case GL_MULTISAMPLING_4X_SQUARE_ROTATED_SCE:
            yscale = xscale = 2;
            break;
        default:
            break;
    }
    rt.aaSamples = xscale * yscale;

    // color
    rt.colorBufferCount = 0;
    rt.colorFormat = JSGCM_NONE;
    colorBufferMask = 0x0;
    GLuint defaultPitch = 0;
    GLuint defaultId = GMM_ERROR;
    GLuint defaultIdOffset = 0;
    for ( int i = 0; i < JSGCM_SETRENDERTARGET_MAXCOUNT; ++i )
    {
        // get the texture and face
        jsTexture* colorTexture = NULL;
        GLuint face = 0;
        _jsFramebufferGetAttachmentTexture( LContext, &color[i], &colorTexture, &face );
        if ( colorTexture == NULL ) continue;

        jsGcmTexture* nvTexture = ( jsGcmTexture * )colorTexture->platformTexture;

        // make sure texture is resident in a supported layout
        //  Some restrictions are added if a texture is used as a
        //  render target:
        //
        //  - no swizzled semifat or fat formats
        //  - no swizzled smaller than 16x16
        //  - no mipmapped cube maps in tiled memory
        //  - no cube maps with height not a multiple of 16 in tiled
        //    memory
        //
        //  We may need to reallocate the texture if any of these
        //  are true.
        //
        //  TODO: Measure time spent here and optimize if indicated.
        if ( !colorTexture->isRenderTarget )
        {
            JS_ASSERT( colorTexture->referenceBuffer == 0 );
            colorTexture->isRenderTarget = GL_TRUE;
            colorTexture->revalidate |= _JS_TEXTURE_REVALIDATE_LAYOUT;
        }
        _jsPlatformValidateTextureResources( colorTexture );
        colorTexture->images[face][0].dataState = _JS_IMAGE_DATASTATE_GPU;

        // set the render target
        rt.colorId[i] = nvTexture->gpuAddressId;
        rt.colorIdOffset[i] = nvTexture->gpuAddressIdOffset + _jsGetGcmImageOffset( &nvTexture->gpuLayout, face, 0 );
        rt.colorPitch[i] = nvTexture->gpuLayout.pitch ? nvTexture->gpuLayout.pitch : nvTexture->gpuLayout.pixelBits * nvTexture->gpuLayout.baseWidth / 8;
        rt.swizzled = nvTexture->gpuLayout.pitch ? GL_FALSE : GL_TRUE;
        colorBufferMask |= 1 << i;

        width = MIN( width, nvTexture->gpuLayout.baseWidth );
        height = MIN( height, nvTexture->gpuLayout.baseHeight );
        rt.colorFormat = nvTexture->gpuLayout.internalFormat;
        rt.colorBufferCount = i + 1;
        defaultId = rt.colorId[i];
        defaultIdOffset = rt.colorIdOffset[i];
        defaultPitch = rt.colorPitch[i];
    }
    for ( int i = 0; i < JSGCM_SETRENDERTARGET_MAXCOUNT; ++i )
    {
        if ( !( colorBufferMask & ( 1 << i ) ) )
        {
            rt.colorId[i] = defaultId;
            rt.colorIdOffset[i] = defaultIdOffset;
            rt.colorPitch[i] = defaultPitch;
        }
    }


    // initialize to no depth/stencil buffer
    rt.depthFormat = JSGCM_NONE;
    rt.depthId = GMM_ERROR;
    rt.depthPitch = 0;

    // depth
    //  Having both depth and stencil attachments is unsupported.  A
    //  valid depth attachment will provide a stencil buffer (but not
    //  vice versa).
    jsTexture* depthTexture;
    GLuint depthFace = 0;
    _jsFramebufferGetAttachmentTexture( LContext, &depth, &depthTexture, &depthFace );
    if ( depthTexture == NULL )
    {
        // stencil ?
        _jsFramebufferGetAttachmentTexture( LContext, &stencil, &depthTexture, &depthFace );
    }
    if ( depthTexture != NULL )
    {
        jsGcmTexture* nvTexture = ( jsGcmTexture * )depthTexture->platformTexture;

        // make sure texture is resident
        if ( !depthTexture->isRenderTarget )
        {
            JS_ASSERT( depthTexture->referenceBuffer == 0 );
            depthTexture->isRenderTarget = GL_TRUE;
            depthTexture->revalidate |= _JS_TEXTURE_REVALIDATE_LAYOUT;
        }
        _jsPlatformValidateTextureResources( depthTexture );
        depthTexture->images[depthFace][0].dataState = _JS_IMAGE_DATASTATE_GPU;

        // set the render target
        rt.swizzled = nvTexture->gpuLayout.pitch ? GL_FALSE : GL_TRUE;
        rt.depthId = nvTexture->gpuAddressId;
        rt.depthIdOffset = nvTexture->gpuAddressIdOffset + _jsGetGcmImageOffset( &nvTexture->gpuLayout, depthFace, 0 );
        rt.depthPitch = nvTexture->gpuLayout.pitch;

        width = MIN( width, nvTexture->gpuLayout.baseWidth );
        height = MIN( height, nvTexture->gpuLayout.baseHeight );
        rt.depthFormat = nvTexture->gpuLayout.internalFormat;
    }

    // framebuffer dimensions are the intersection of attachments
    rt.width = width / xscale;
    rt.height = height / yscale;

    rt.yInverted = JSGCM_FALSE;
    rt.xOffset = 0;
    rt.yOffset = 0;
    needValidate = GL_FALSE;
}

// set render targets
void _jsValidateFramebuffer( void )
{
    PSGLdevice *LDevice = _CurrentDevice;
    JS_ASSERT( LDevice != NULL );
    jsGcmDevice *gcmDevice = ( jsGcmDevice * )LDevice->platformDevice;

    PSGLcontext* LContext = _CurrentContext;
    JS_ASSERT( LContext != NULL );
    jsGcmDriver * gcmDriver = _jsGetGcmDriver();

    // reset buffer data
    gcmDriver->rtValid = GL_FALSE;
    // get buffer parameters
    //  This may come from a framebuffer_object or the default framebuffer.
    if ( LContext->framebuffer )
    {
        jsPlatformFramebuffer* framebuffer = static_cast<jsPlatformFramebuffer *>( _jsGetFramebuffer( LContext, LContext->framebuffer ) );
        JS_ASSERT( framebuffer != NULL );

        if ( framebuffer->needValidate ) framebuffer->validate( LContext );

#ifndef JS_NO_ERROR_CHECK
        if ( !framebuffer->complete )
        {
            _jsSetError( GL_INVALID_FRAMEBUFFER_OPERATION_OES );
            return;
        }
#endif
        gcmDriver->rt = framebuffer->rt;
        gcmDriver->colorBufferMask = framebuffer->colorBufferMask;
    }
    else	// use default framebuffer
    {
        gcmDriver->rt = gcmDevice->rt;
        gcmDriver->colorBufferMask = 0x1;
    }

    gcmDriver->rtValid = GL_TRUE;
    gcmDriver->xSuperSampling = 1;
    gcmDriver->ySuperSampling = 1;

    // update GPU configuration
    _jsGcmFifoGlSetRenderTarget( &gcmDriver->rt );

	// [YLIN]	Both L1&L2 caches will be implicitly invalidated by above render target setting,
	//			We don't need to explicitly call invalidation method here
    //_jsGcmFifoGlInvalidateTextureCache();

    LContext->needValidate &= ~PSGL_VALIDATE_FRAMEBUFFER;
    LContext->needValidate |= PSGL_VALIDATE_VIEWPORT | PSGL_VALIDATE_SCISSOR_BOX
                              | PSGL_VALIDATE_WRITE_MASK | PSGL_VALIDATE_STENCIL_TEST
                              | PSGL_VALIDATE_ALPHA_TEST | PSGL_VALIDATE_MULTISAMPLING;
}


// utility function to fill a surface descriptor
int _jsGcmGetFramebufferSurface( GLenum format, jsGcmSurface* gcmSurface )
{
    PSGLcontext* LContext = _CurrentContext;
    JS_ASSERT( LContext != NULL );

    if ( LContext->framebuffer )
    {
        jsFramebuffer* framebuffer = _jsGetFramebuffer( LContext, LContext->framebuffer );
        JS_ASSERT( framebuffer != NULL );	// assertion or error?

        GLuint xscale = 1;
        GLuint yscale = 1;
        switch ( framebuffer->multisampling )
        {
            case GL_MULTISAMPLING_2X_DIAGONAL_CENTERED_SCE:
                xscale = 2;
                break;
            case GL_MULTISAMPLING_4X_SQUARE_CENTERED_SCE:
            case GL_MULTISAMPLING_4X_SQUARE_ROTATED_SCE:
                yscale = xscale = 2;
                break;
            case GL_MULTISAMPLING_NONE_SCE:
            default:
                break;
        }
        // TODO: make sure texture is resident

        // get the texture and face
        jsTexture* texture = NULL;
        GLuint face = 0;
        switch ( format )
        {
            case GL_DEPTH_COMPONENT:
            case JSGCM_DEPTH_COMPONENT16:
            case JSGCM_DEPTH_COMPONENT24:
                _jsFramebufferGetAttachmentTexture(
                    LContext,
                    &framebuffer->depth,
                    &texture, &face );
                break;
            default:
                // choose selected color buffer
                JS_ASSERT( LContext->ReadBuffer >= GL_COLOR_ATTACHMENT0_EXT &&
                           LContext->ReadBuffer <= GL_COLOR_ATTACHMENT3_EXT );
                _jsFramebufferGetAttachmentTexture(
                    LContext,
                    &framebuffer->color[LContext->ReadBuffer - GL_COLOR_ATTACHMENT0_EXT],
                    &texture, &face );
                break;
        }

        if ( texture == NULL )
            return -1;

        jsGcmTexture* nvTexture = ( jsGcmTexture * )texture->platformTexture;
        gcmSurface->source	= _JSGCM_SURFACE_SOURCE_TEXTURE;
        gcmSurface->width	= nvTexture->gpuLayout.baseWidth / xscale;
        gcmSurface->height	= nvTexture->gpuLayout.baseHeight / yscale;
        gcmSurface->bpp		= nvTexture->gpuLayout.pixelBits / 8;
        gcmSurface->pitch	= nvTexture->gpuLayout.pitch;
        gcmSurface->samples = xscale * yscale;
        gcmSurface->format	= nvTexture->gpuLayout.internalFormat;
        gcmSurface->flip	= GL_FALSE;
        gcmSurface->pool	= nvTexture->pool;
        gcmSurface->dataId	= nvTexture->gpuAddressId;
        gcmSurface->dataIdOffset = _jsGetGcmImageOffset( &nvTexture->gpuLayout, face, 0 );
    }
    else // default framebuffer
    {
        PSGLdevice *LDevice = _CurrentDevice;
        JS_ASSERT( LDevice != NULL );
        jsGcmDevice * gcmDevice = ( jsGcmDevice * )LDevice->platformDevice;

        switch ( format )
        {
            case GL_DEPTH_COMPONENT:
            case JSGCM_DEPTH_COMPONENT16:
            case JSGCM_DEPTH_COMPONENT24:
                if ( gcmDevice->depthStencil.pool == _JSGCM_SURFACE_POOL_NONE )
                    return -1;
                *gcmSurface = gcmDevice->depthStencil;
                break;
            default:
                if ( gcmDevice->color[gcmDevice->drawBuffer].pool == _JSGCM_SURFACE_POOL_NONE )
                    return -1;
                *gcmSurface = gcmDevice->color[gcmDevice->drawBuffer];
                break;
        }
    }

    return 0;
}

void _jsPlatformReadPixels(
    GLint x, GLint y,
    GLsizei width, GLsizei height,
    GLboolean flip,
    GLenum format, GLenum type, GLvoid *pixels )
{
    // determine source surface
    jsGcmSurface src;
    if ( _jsGcmGetFramebufferSurface( format, &src ) )
    {
        _jsSetError( GL_INVALID_OPERATION );
        return;
    }

    // If we want to read it flipped, we need to adjust y.
    if ( flip )
    {
        y = src.height - y - height;
    }

    GLint xoffset = MAX( x, 0 );
    GLint yoffset = MAX( y, 0 );

    // copy to temporary buffer in system memory
    //  This takes care of any downsampling, swizzling, or flip.
    jsGcmSurface tmp = src;
    tmp.source	= _JSGCM_SURFACE_SOURCE_TEMPORARY;
    tmp.width	= width;
    tmp.height	= height;
    tmp.pitch	= _jsPad( tmp.bpp * tmp.width, 4 );
    tmp.samples	= 1;
    tmp.flip	= flip;
    tmp.pool	= _JSGCM_SURFACE_POOL_PPU;
    tmp.ppuData	= ( char * )jsMalloc( tmp.pitch * tmp.height );
    tmp.dataId  = GMM_ERROR;
    tmp.dataIdOffset = 0;
#ifndef JS_NO_ERROR_CHECK
    if ( tmp.ppuData == NULL )
    {
        _jsSetError( GL_OUT_OF_MEMORY );
        return;
    }
#endif

#ifndef JS_NO_ERROR_CHECK
    if ( yoffset + tmp.height > src.height )
    {
        _jsSetError( GL_INVALID_VALUE );
        return;
    }
#endif

    // TODO: don't copy entire surface unless necessary
    _jsGcmCopySurface( &src, xoffset, yoffset, &tmp, 0, 0, tmp.width, tmp.height, GL_FALSE );

    // describe framebuffer source
    GLenum srcFormat = 0;
    GLenum srcType = 0;
    switch ( src.format )
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
        data:		tmp.ppuData
        };

    // describe destination
    const GLint dstPixelSize = _jsGetPixelSize( format, type );
    jsImage image;
    image.format = format;
    image.type = type;
    image.width = width;
    image.height = height;
    image.depth = 1;
    image.storageType = _JS_IMAGE_STORAGE_RASTER;
    image.storageSize = _jsGetStorageSize( format, type, width, height, 1 );
    image.xstride = dstPixelSize;
    image.ystride = dstPixelSize * width;
    image.zstride = dstPixelSize * width * height;
    image.data = ( char * )pixels;

    // copy with conversion
    _jsGcmFifoGlFinish();	// in case _jsGcmCopySurface uses GPU
    _jsRasterToImage( &raster, &image, 0, 0, 0 );

    jsFree( tmp.ppuData );
}

GLboolean _jsPlatformReadPBOPixels( GLint x, GLint y, GLsizei width, GLsizei height, GLboolean flip, GLenum format, GLenum type, GLvoid *pixels )
{
    PSGLcontext*	LContext = _CurrentContext;
    JS_ASSERT( LContext != NULL );

    if ( LContext->packAlignment != 4 )
        return GL_FALSE;

    // determine source surface
    jsGcmSurface src;
    if ( JS_UNLIKELY( LContext->needValidate & PSGL_VALIDATE_FRAMEBUFFER ) )
        _jsValidateFramebuffer();
    if ( _jsGcmGetFramebufferSurface( format, &src ) )
    {
        _jsSetError( GL_INVALID_OPERATION );
        return GL_FALSE;
    }

    // If we want to read it flipped, we need to adjust y.
    if ( flip )
    {
        y = src.height - y - height;
    }

    // check surface type against PBO type
    switch ( format )
    {
        case GL_BGRA:
            switch ( src.format )
            {
                case JSGCM_ARGB8:
                    if ( type != GL_UNSIGNED_INT_8_8_8_8_REV )
                        return GL_FALSE;
                    break;
                default:
                    return GL_FALSE;
            }
            break;
        case GL_RGBA:
            switch ( src.format )
            {
                case GL_RGBA16:
                    if ( type != GL_UNSIGNED_SHORT )
                        return GL_FALSE;
                    break;
                case JSGCM_FLOAT_RGBA16:
                    if ( type != GL_HALF_FLOAT_ARB )
                        return GL_FALSE;
                    break;
                case JSGCM_FLOAT_RGBA32:
                    if ( type != GL_FLOAT )
                        return GL_FALSE;
                    break;
                default:
                    return GL_FALSE;
            }
            break;
        case GL_DEPTH_COMPONENT:
            switch ( src.format )
            {
                case JSGCM_DEPTH_COMPONENT16:
                    if ( type != GL_UNSIGNED_SHORT )
                        return GL_FALSE;
                    break;
                case JSGCM_DEPTH_COMPONENT24:
                    if ( type != GL_UNSIGNED_INT_24_8_SCE )
                        return GL_FALSE;
                    break;
                default:
                    return GL_FALSE;
            }
            break;
        default:	// TODO: stencil
            return GL_FALSE;
    }

    // get buffer object
    jsBufferObject* bufferObject = _jsGetBufferObject( LContext, LContext->PixelPackBuffer );
    JS_ASSERT( bufferObject != NULL );

    const int size = _jsGetStorageSize( format, type, width, height, 1 );
    if ( size <= bufferObject->size )
    {
        jsGcmBufferObject *gcmBuffer = ( jsGcmBufferObject * ) & bufferObject->platformBufferObject;
        const GLuint pixelSize = _jsGetPixelSize( format, type );
        jsGcmSurface dst =
            {
        source:		_JSGCM_SURFACE_SOURCE_PBO,
        width:		width,
        height:		height,
        bpp:		pixelSize,
        pitch:		pixelSize*width,
        samples:	1,
        format:		src.format,
        flip:		flip,
        pool:		gcmBuffer->pool,
        ppuData:    ( gcmBuffer->pool == _JSGCM_SURFACE_POOL_PPU ) ? 
                    gmmIdToAddress( gcmBuffer->bufferId ) + ( intptr_t )pixels :
                    NULL,
        dataId:		( gcmBuffer->pool != _JSGCM_SURFACE_POOL_PPU ) ?
                    gcmBuffer->bufferId :
                    GMM_ERROR,
        dataIdOffset: ( gcmBuffer->pool != _JSGCM_SURFACE_POOL_PPU ) ?
                      ( intptr_t )pixels :
                      0
            };

        _jsGcmCopySurface( &src, x, y, &dst, 0, 0, width, height, GL_TRUE );

        // be conservative here. Whenever we write to any Buffer Object, invalidate the vertex cache
        jsGcmDriver *driver = _jsGetGcmDriver();
        driver->invalidateVertexCache = GL_TRUE;
    }
    else
        _jsSetError( GL_INVALID_OPERATION );

    return GL_TRUE;
}
