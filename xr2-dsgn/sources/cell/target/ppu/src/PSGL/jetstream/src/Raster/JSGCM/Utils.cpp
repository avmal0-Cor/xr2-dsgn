/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 *                Copyright (C) 2009 Sony Computer Entertainment Inc.
 *                                               All Rights Reserved.
 */

 
/*
 * 
 */

#include <JSGCM/JSGcmUtils.h>
#include <JSGCM/JSGcmDriver.h>
#include <JSGCM/JSGcmValidateInlines.h>
#include <JSGCM/JSGcmFifoUtils.h>
#include <JSGCM/JSGcmVertexAttribPointer.h>
#include <JSGCM/JSGcmTexImage.h>
#include <JSGCM/JSGcmSetRenderTarget.h>
#include <PSGL/psgl.h>
#include <PSGL/private.h>
#include <PSGL/Matrix.h>
#include <PSGL/Debug.h>

void _jsDrawUtilQuad( GLboolean useFixedVP, GLboolean useFixedFP, GLuint x, GLuint y, GLuint width, GLuint height )
{
    PSGLcontext*	LContext = _CurrentContext;
    JS_ASSERT( LContext );
    jsGcmDriver *driver = _jsGetGcmDriver();
    JS_ASSERT( driver );

    _jsGcmFifoGlDisable( JSGCM_ALPHA_TEST );
    _jsGcmFifoGlDisable( JSGCM_COLOR_LOGIC_OP );
    _jsGcmFifoGlDisable( JSGCM_BLEND );
    _jsGcmFifoGlDisable( JSGCM_CULL_FACE );
    _jsGcmFifoGlDisable( JSGCM_POLYGON_OFFSET_FILL );
    _jsGcmFifoGlDisable( JSGCM_POLYGON_OFFSET_LINE );
    _jsGcmFifoGlDisable( JSGCM_PRIMITIVE_RESTART );
    _jsGcmFifoGlClipPlanes( 0 );

    GCM_FUNC( cellGcmSetFrontPolygonMode, CELL_GCM_POLYGON_MODE_FILL ); // GCM_PORT_TESTED [KHOFF]
    GCM_FUNC( cellGcmSetBackPolygonMode, CELL_GCM_POLYGON_MODE_FILL );
    GCM_FUNC( cellGcmSetUserClipPlaneControl,
              CELL_GCM_USER_CLIP_PLANE_DISABLE,
              CELL_GCM_USER_CLIP_PLANE_DISABLE,
              CELL_GCM_USER_CLIP_PLANE_DISABLE,
              CELL_GCM_USER_CLIP_PLANE_DISABLE,
              CELL_GCM_USER_CLIP_PLANE_DISABLE,
              CELL_GCM_USER_CLIP_PLANE_DISABLE );

    _jsGcmFifoGlViewport( x, y, width, height, 0.0f, 1.0f );
    //_jsGcmFifoGlDepthRangef(0.f,1.f);  [Jun] Set this in above function call

    if ( useFixedVP )
    {
        float p[4] = {0.f, 0.f, 0.f, 0.f}; // disable lighting, texgens
        for ( int i = 0;i < _JS_MAX_TEXTURE_UNITS;++i )
        {
            _jsGcmFifoGlProgramParameterfv( driver->vsFixedPipeline, driver->texunit[i].genMode, p, NULL );
            LContext->TextureCoordsUnits[i].revalidate |= _JS_TEXTURE_UNIT_REVALIDATE_TEX_GEN;
        }

        _jsGcmFifoGlProgramParameterfv( driver->vsFixedPipeline, driver->flags, p, NULL );
        _jsGcmFifoGlProgramParameterfv( driver->vsFixedPipeline, driver->projXform, _jsIdentityMatrixf, NULL );

        char* ucode = NULL;
        _jsGcmGetUCode( driver->vsFixedPipeline, &ucode, NULL );
        _jsGcmSetNativeCgVertexProgram( driver->vsFixedPipeline, ucode );
        LContext->needValidate |= PSGL_VALIDATE_FFX_VERTEX_PROGRAM | PSGL_VALIDATE_VERTEX_PROGRAM;
        LContext->ModelViewMatrixStack.dirty = GL_TRUE;
        LContext->ProjectionMatrixStack.dirty = GL_TRUE;
    }

    if ( useFixedFP )
    {
        _jsGcmFPShaderManagerSetFog( &driver->shaderManager, 0, ( float * )&LContext->FogColor, 0, 0, 0 );
        GLboolean res = _jsGcmFPLoadCurrentShader( &driver->shaderManager );
        JS_ASSERT( res );
        ( void )( res );
        LContext->needValidate |= PSGL_VALIDATE_FFX_FRAGMENT_PROGRAM | PSGL_VALIDATE_FRAGMENT_PROGRAM;
    }

    _jsGcmFifoGlInvalidateVertexCache();
    _jsGcmFifoGlDrawArrays( JSGCM_TRIANGLE_STRIP, 0, 4 );

    LContext->needValidate |= PSGL_VALIDATE_ALPHA_TEST
                              | PSGL_VALIDATE_LOGIC_OP
                              | PSGL_VALIDATE_BLENDING
                              | PSGL_VALIDATE_FACE_CULL
                              | PSGL_VALIDATE_POLYGON_OFFSET
                              | PSGL_VALIDATE_POLYGON_MODE
                              | PSGL_VALIDATE_PRIMITIVE_RESTART
                              | PSGL_VALIDATE_CLIP_PLANES
                              | PSGL_VALIDATE_VIEWPORT;
}




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
    GLboolean filter )
{
    PSGLcontext*	LContext = _CurrentContext;
    JS_ASSERT( LContext );
    jsGcmDriver *driver = _jsGetGcmDriver();
    JS_ASSERT( driver );
    jsGcmEnum format = JSGCM_ARGB8;
    switch ( bytesPerPixel )
    {
        case 4:
            format = JSGCM_ARGB8;
            break;
        case 8:
            format = JSGCM_FLOAT_RGBA16;
            break;
        default:
            JS_ASSERT( 0 );
            break;
    }

    _jsGcmFifoGlDisable( JSGCM_DEPTH_TEST );
    _jsGcmFifoGlDepthMask( GL_FALSE );
    _jsGcmFifoGlDisable( JSGCM_STENCIL_TEST );
    _jsGcmFifoGlColorMask( JSGCM_TRUE, JSGCM_TRUE, JSGCM_TRUE, JSGCM_TRUE );
    _jsGcmFifoGlDisable( JSGCM_SCISSOR_TEST );

    for ( int i = 1;i < _JS_MAX_TEXTURE_UNITS;++i )
    {
        _jsGcmFPShaderManagerSetTextureMode( &driver->shaderManager, i, 0 );
    }

    _jsGcmFPShaderManagerSetTextureMode( &driver->shaderManager, 0, GL_TEXTURE_2D );
    GLfloat blendColor[4] = {1.f, 1.f, 1.f, 1.f};
    _jsGcmFPShaderManagerSetTextureEnvMode( &driver->shaderManager, 0, GL_MODULATE, blendColor );
    _jsGcmFPShaderManagerSetTextureFormat( &driver->shaderManager, 0, GL_RGBA );
    static const float _jsBlitVertexBuffer[4*2] __attribute__(( aligned( JS_ALIGN_FAST_TRANSFER ) ) ) =
        {
            -1.f, -1.f,
            -1.f, 1.f,
            1.f, -1.f,
            1.f, 1.f,
        };
    static const float _jsBlitTCBuffer[4*2] __attribute__(( aligned( JS_ALIGN_FAST_TRANSFER ) ) ) =
        {
            0.f, 0.f,
            0.f, 1.f,
            1.f, 0.f,
            1.f, 1.f,
        };
    GLuint bufferId = _jsGcmAllocateMemory( sizeof( _jsBlitVertexBuffer ) + sizeof( _jsBlitTCBuffer ) );
    JS_ASSERT( bufferId != GMM_ERROR );
    fast_memcpy( gmmIdToAddress(bufferId), _jsBlitVertexBuffer, sizeof( _jsBlitVertexBuffer ) );
    fast_memcpy( gmmIdToAddress(bufferId) + sizeof( _jsBlitVertexBuffer ), _jsBlitTCBuffer, sizeof( _jsBlitTCBuffer ) );
    for ( int i = 0;i < _JS_MAX_VERTEX_ATTRIBS;++i )
    {
        switch ( i )
        {
            case _JS_ATTRIB_POSITION_INDEX:
                _jsGcmFifoGlVertexAttribPointer( i, 2, JSGCM_FLOAT, JSGCM_FALSE, 2*sizeof( GLfloat ), 1, 0, gmmIdToOffset(bufferId) );
                break;
            case _JS_ATTRIB_TEX_COORD0_INDEX:
                _jsGcmFifoGlVertexAttribPointer( i, 2, JSGCM_FLOAT, JSGCM_FALSE, 2*sizeof( GLfloat ), 1, 0, gmmIdToOffset(bufferId) + sizeof( _jsBlitVertexBuffer ) );
                break;
            default:
                _jsGcmFifoGlVertexAttribPointer( i, 0, JSGCM_FLOAT, 0, 0, 0, 0, 0 );
                break;
        }
        JSBIT_TRUE( LContext->attribs->DirtyMask, i );
    }
    static const float white[4] = {1.f, 1.f, 1.f, 1.f};
    _jsGcmFifoGlVertexAttrib4fv( _JS_ATTRIB_PRIMARY_COLOR_INDEX, white );
    _jsGcmFifoGlVertexFrequencyOperation( 0 );

    // update the setTextureFilter Portion  bias , min, mag filters, and convolution
    if ( filter )
    {
        GCM_FUNC( cellGcmSetTextureFilter, 0, 0,  CELL_GCM_TEXTURE_LINEAR,
                  CELL_GCM_TEXTURE_LINEAR, CELL_GCM_TEXTURE_CONVOLUTION_QUINCUNX );
    }
    else
    {
        GCM_FUNC( cellGcmSetTextureFilter, 0, 0,  CELL_GCM_TEXTURE_NEAREST,
                  CELL_GCM_TEXTURE_NEAREST, CELL_GCM_TEXTURE_CONVOLUTION_QUINCUNX );
    }

    // update the setTextureAddress Portion
    GCM_FUNC( cellGcmSetTextureAddress, 0, CELL_GCM_TEXTURE_CLAMP_TO_EDGE, CELL_GCM_TEXTURE_CLAMP_TO_EDGE, CELL_GCM_TEXTURE_CLAMP_TO_EDGE,
              CELL_GCM_TEXTURE_UNSIGNED_REMAP_NORMAL, CELL_GCM_TEXTURE_ZFUNC_NEVER, 0 );

    // GCM_PORT_TESTED [RSTENSON]
    // update the textuer control to setup antisotropic settings and min and max lod
    GCM_FUNC( cellGcmSetTextureControl, 0, CELL_GCM_TRUE, 0, 1 << 8, CELL_GCM_TEXTURE_MAX_ANISO_1 );

    _jsGcmFifoGlTexImage2( JSGCM_TEXTURE_2D, 0, 1, srcWidth, srcHeight, 1, format, GL_FALSE, GL_TRUE, srcPitch, gmmIdIsMain(srcVidId), gmmIdToOffset(srcVidId)+srcVidIdOffset );
    _jsGcmFifoGlInvalidateTextureCache();

    // save RT because we can get called from ValidateSurfaces which is modifying it.
    jsGcmRenderTargetEx savedRT = driver->rt;
    driver->rt.colorFormat = format;
    driver->rt.colorBufferCount = 1;
    driver->rt.colorId[0] = dstVidId;
    driver->rt.colorIdOffset[0] = dstVidIdOffset;
    driver->rt.colorPitch[0] = dstPitch;
    driver->rt.depthFormat = JSGCM_NONE;
    driver->rt.depthId = GMM_ERROR;
    driver->rt.depthIdOffset = 0;
    driver->rt.depthPitch = dstPitch;
    driver->rt.swizzled = JSGCM_FALSE;
    driver->rt.yInverted = JSGCM_FALSE;
    driver->rt.xOffset = 0;
    driver->rt.yOffset = 0;
    driver->rt.width = dstWidth;
    driver->rt.height = dstHeight;
    driver->rt.aaSamples = 1;
    _jsGcmFifoGlSetRenderTarget( &driver->rt );
    _jsDrawUtilQuad( GL_TRUE, GL_TRUE, 0, 0, dstWidth, dstHeight );

    driver->rt = savedRT;

    _jsGcmFreeMemory( bufferId );

    LContext->needValidate |= PSGL_VALIDATE_DEPTH_TEST
                              | PSGL_VALIDATE_STENCIL_TEST
                              | PSGL_VALIDATE_WRITE_MASK
                              | PSGL_VALIDATE_SCISSOR_BOX
                              | PSGL_VALIDATE_TEXTURES_USED
                              | PSGL_VALIDATE_FRAMEBUFFER;
}

void _jsVertexTexImage2D( GLuint unit, GLuint levels, GLuint width, GLuint height, GLenum internalFormat, GLuint pitch, GLboolean isMain, GLuint gpuOffset )
{
    JS_ASSERT( unit < 4 );
    JS_ASSERT( levels <= 12 );
    JS_ASSERT(( width <= 4096 ) && ( width > 0 ) );
    JS_ASSERT(( height <= 4096 ) && ( height > 0 ) );
    JS_ASSERT( pitch > 0 );

    // vertex texturing works from Local(GPU Video) memory only so let's
    // just make sure that's the case with the GPU Address passed in
    JS_ASSERT( isMain == 0 );

    CellGcmTexture texture;
    texture.cubemap = CELL_GCM_FALSE;
    texture.depth = 1;
    texture.width = width;
    texture.height = height;
    texture.mipmap = levels;
    texture.pitch = pitch;
    texture.offset = gpuOffset;
    texture.dimension = (( height == 1 ) ? CELL_GCM_TEXTURE_DIMENSION_1 : CELL_GCM_TEXTURE_DIMENSION_2 );
    texture.location =  CELL_GCM_LOCATION_LOCAL;
    texture.remap = 0; // remap ignored for vertex texturing

    // set format
    switch ( internalFormat )
    {
        case JSGCM_FLOAT_RGBA32: // for the linear version
            texture.format = CELL_GCM_TEXTURE_LN | CELL_GCM_TEXTURE_NR |
                             CELL_GCM_TEXTURE_W32_Z32_Y32_X32_FLOAT;
            break;
        case JSGCM_LUMINANCE32F_ARB: // for the linear version
            texture.format =  CELL_GCM_TEXTURE_LN | CELL_GCM_TEXTURE_NR |
                              CELL_GCM_TEXTURE_X32_FLOAT;
            break;
        default:
            JS_ASSERT( 0 );
            return;
    }

    // now set this vertex texture
    cellGcmSetVertexTexture( unit, &texture );
}


void _jsVertexTexParameters( GLuint unit, GLboolean enable, GLenum wrapS, GLenum wrapT, GLfloat lodBias, GLfloat minLod, GLfloat maxLod )
{
    JS_ASSERT( unit < 4 );

    uint8_t gcmWrapS, gcmWrapT;
    switch ( wrapS )
    {
        case GL_REPEAT:        gcmWrapS = CELL_GCM_TEXTURE_WRAP;          break;
        case GL_CLAMP:         gcmWrapS = CELL_GCM_TEXTURE_CLAMP;         break;
        case GL_CLAMP_TO_EDGE: gcmWrapS = CELL_GCM_TEXTURE_CLAMP_TO_EDGE; break;
            // other supported modes are BORDER,MIRROR_ONCE_CLAMP_TO_EDGE,MIRROR_ONCE_BORDER,MIRROR_ONCE_CLAMP_OGL
        default:               JS_ASSERT( 0 );                            return;
    }
    switch ( wrapT )
    {
        case GL_REPEAT:        gcmWrapT = CELL_GCM_TEXTURE_WRAP;          break;
        case GL_CLAMP:         gcmWrapT = CELL_GCM_TEXTURE_CLAMP;         break;
        case GL_CLAMP_TO_EDGE: gcmWrapT = CELL_GCM_TEXTURE_CLAMP_TO_EDGE; break;
            // other supported modes are BORDER,MIRROR_ONCE_CLAMP_TO_EDGE,MIRROR_ONCE_BORDER,MIRROR_ONCE_CLAMP_OGL
        default:               JS_ASSERT( 0 );                            return;
    }

    GCM_FUNC( cellGcmSetVertexTextureAddress, unit, gcmWrapS, gcmWrapT ); // GCM_PORT_TESTED [KHOFF]
    GCM_FUNC( cellGcmSetVertexTextureControl, unit, enable, ( GLuint )( minLod*256.f ), ( GLuint )( maxLod*256.f ) );
    GCM_FUNC( cellGcmSetVertexTextureFilter, unit, ( GLint )( lodBias*256.f ) );
}
