/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 *                Copyright (C) 2009 Sony Computer Entertainment Inc.
 *                                               All Rights Reserved.
 */


/*
 * File description here
 */

#ifndef _H_JSGCM_TEXIMAGE_H_
#define _H_JSGCM_TEXIMAGE_H_

static inline GLuint _jsGcmMapMinTextureFilter( GLenum filter )
{
    switch ( filter )
    {
        case GL_NEAREST:
            return CELL_GCM_TEXTURE_NEAREST;
            break;
        case GL_LINEAR:
            return CELL_GCM_TEXTURE_LINEAR;
            break;
        case GL_NEAREST_MIPMAP_NEAREST:
            return CELL_GCM_TEXTURE_NEAREST_NEAREST;
            break;
        case GL_NEAREST_MIPMAP_LINEAR:
            return CELL_GCM_TEXTURE_NEAREST_LINEAR;
            break;
        case GL_LINEAR_MIPMAP_NEAREST:
            return CELL_GCM_TEXTURE_LINEAR_NEAREST;
            break;
        case GL_LINEAR_MIPMAP_LINEAR:
            return CELL_GCM_TEXTURE_LINEAR_LINEAR;
            break;
        default:
            JS_ASSERT( 0 );
            return 0;
    }
    return filter;
}

static inline GLuint _jsGcmMapMagTextureFilter( GLenum filter )
{
    switch ( filter )
    {
        case GL_NEAREST:
            return CELL_GCM_TEXTURE_NEAREST;
            break;
        case GL_LINEAR:
            return CELL_GCM_TEXTURE_LINEAR;
            break;
        default:
            JS_ASSERT( 0 );
            return 0;
    }
    return filter;
}

static inline GLuint _jsGcmMapAniso( GLuint maxAniso )
{

    if ( maxAniso >= 16 )
        return CELL_GCM_TEXTURE_MAX_ANISO_16;
    if ( maxAniso == 1 )
        return CELL_GCM_TEXTURE_MAX_ANISO_1;

    JS_ASSERT(( maxAniso >= 0 ) && (( maxAniso / 2 ) <= 7 ) );

    switch ( maxAniso / 2 )
    {
        case 1:
            return CELL_GCM_TEXTURE_MAX_ANISO_2;
            break;
        case 2:
            return CELL_GCM_TEXTURE_MAX_ANISO_4;
            break;
        case 3:
            return CELL_GCM_TEXTURE_MAX_ANISO_6;
            break;
        case 4:
            return CELL_GCM_TEXTURE_MAX_ANISO_8;
            break;
        case 5:
            return CELL_GCM_TEXTURE_MAX_ANISO_10;
            break;
        case 6:
            return CELL_GCM_TEXTURE_MAX_ANISO_12;
            break;
        case 7:
            return CELL_GCM_TEXTURE_MAX_ANISO_16;
            break;
        default:
            JS_ASSERT( 0 );
            return 0;
            break;
    }
    return 0;
}

static inline GLuint _jsGcmMapWrapMode( GLuint mode )
{
    switch ( mode )
    {
        case JSGCM_CLAMP:
            return CELL_GCM_TEXTURE_CLAMP;
            break;
        case JSGCM_REPEAT:
            return CELL_GCM_TEXTURE_WRAP;
            break;
        case JSGCM_CLAMP_TO_EDGE:
            return CELL_GCM_TEXTURE_CLAMP_TO_EDGE;
            break;
        case JSGCM_CLAMP_TO_BORDER:
            return CELL_GCM_TEXTURE_BORDER;
            break;
        case JSGCM_MIRRORED_REPEAT:
            return CELL_GCM_TEXTURE_MIRROR;
            break;
        case JSGCM_MIRROR_CLAMP_TO_EDGE:
            return CELL_GCM_TEXTURE_MIRROR_ONCE_CLAMP_TO_EDGE;
            break;
        case JSGCM_MIRROR_CLAMP_TO_BORDER:
            return CELL_GCM_TEXTURE_MIRROR_ONCE_BORDER;
            break;
        case JSGCM_MIRROR_CLAMP:
            return CELL_GCM_TEXTURE_MIRROR_ONCE_CLAMP;
            break;
        default:
            JS_ASSERT( 0 );
            return 0;
            break;
    }
    return 0;
}

static inline GLuint _jsGcmMapTexZFunMode( GLuint func )
{
    switch ( func )
    {
        case JSGCM_NEVER:
            return CELL_GCM_TEXTURE_ZFUNC_NEVER;
            break;
        case JSGCM_LESS:
            return CELL_GCM_TEXTURE_ZFUNC_GREATER;
            break;
        case JSGCM_EQUAL:
            return CELL_GCM_TEXTURE_ZFUNC_EQUAL;
            break;
        case JSGCM_LEQUAL:
            return CELL_GCM_TEXTURE_ZFUNC_GEQUAL;
            break;
        case JSGCM_GREATER:
            return CELL_GCM_TEXTURE_ZFUNC_LESS;
            break;
        case JSGCM_NOTEQUAL:
            return CELL_GCM_TEXTURE_ZFUNC_NOTEQUAL;
            break;
        case JSGCM_GEQUAL:
            return CELL_GCM_TEXTURE_ZFUNC_LEQUAL;
            break;
        case JSGCM_ALWAYS:
            return CELL_GCM_TEXTURE_ZFUNC_ALWAYS;
            break;
        default:
            JS_ASSERT( 0 );
            return 0;
            break;
    }
    return 0;
}


static inline void _jsGcmMapTextureFormat( GLuint internalFormat, uint8_t & gcmFormat, uint32_t & remap )
{
    gcmFormat = 0;

    switch ( internalFormat )
    {
        case JSGCM_LUMINANCE8:             // in_rgba = xxLx, out_rgba = LLL1
        {
            gcmFormat =  CELL_GCM_TEXTURE_B8;
            remap = CELL_GCM_REMAP_MODE(
                        CELL_GCM_TEXTURE_REMAP_ORDER_XYXY,
                        CELL_GCM_TEXTURE_REMAP_FROM_B,
                        CELL_GCM_TEXTURE_REMAP_FROM_B,
                        CELL_GCM_TEXTURE_REMAP_FROM_B,
                        CELL_GCM_TEXTURE_REMAP_FROM_B,
                        CELL_GCM_TEXTURE_REMAP_ONE,
                        CELL_GCM_TEXTURE_REMAP_REMAP,
                        CELL_GCM_TEXTURE_REMAP_REMAP,
                        CELL_GCM_TEXTURE_REMAP_REMAP );

        }
        break;
        case JSGCM_LUMINANCE16:            // in_rgba = xLLx, out_rgba = LLL1
        {
            gcmFormat =  CELL_GCM_TEXTURE_X16;
            remap = CELL_GCM_REMAP_MODE(
                        CELL_GCM_TEXTURE_REMAP_ORDER_XYXY,
                        CELL_GCM_TEXTURE_REMAP_FROM_G,
                        CELL_GCM_TEXTURE_REMAP_FROM_B,
                        CELL_GCM_TEXTURE_REMAP_FROM_G,
                        CELL_GCM_TEXTURE_REMAP_FROM_B,
                        CELL_GCM_TEXTURE_REMAP_ONE,
                        CELL_GCM_TEXTURE_REMAP_REMAP,
                        CELL_GCM_TEXTURE_REMAP_REMAP,
                        CELL_GCM_TEXTURE_REMAP_REMAP );

        }
        break;
        case JSGCM_ALPHA8:                 // in_rgba = xxAx, out_rgba = 000A
        {
            gcmFormat =  CELL_GCM_TEXTURE_B8;
            remap = CELL_GCM_REMAP_MODE(
                        CELL_GCM_TEXTURE_REMAP_ORDER_XYXY,
                        CELL_GCM_TEXTURE_REMAP_FROM_B,
                        CELL_GCM_TEXTURE_REMAP_FROM_R,
                        CELL_GCM_TEXTURE_REMAP_FROM_B,
                        CELL_GCM_TEXTURE_REMAP_FROM_B,
                        CELL_GCM_TEXTURE_REMAP_REMAP,
                        CELL_GCM_TEXTURE_REMAP_ZERO,
                        CELL_GCM_TEXTURE_REMAP_ZERO,
                        CELL_GCM_TEXTURE_REMAP_ZERO );

        }
        break;
        case JSGCM_ALPHA16:                // in_rgba = xAAx, out_rgba = 000A
        {
            gcmFormat =  CELL_GCM_TEXTURE_X16;
            remap = CELL_GCM_REMAP_MODE(
                        CELL_GCM_TEXTURE_REMAP_ORDER_XYXY,
                        CELL_GCM_TEXTURE_REMAP_FROM_G,
                        CELL_GCM_TEXTURE_REMAP_FROM_B,
                        CELL_GCM_TEXTURE_REMAP_FROM_G,
                        CELL_GCM_TEXTURE_REMAP_FROM_B,
                        CELL_GCM_TEXTURE_REMAP_REMAP,
                        CELL_GCM_TEXTURE_REMAP_ZERO,
                        CELL_GCM_TEXTURE_REMAP_ZERO,
                        CELL_GCM_TEXTURE_REMAP_ZERO );

        }
        break;
        case JSGCM_INTENSITY8:             // in_rgba = xxIx, out_rgba = IIII
        {
            gcmFormat =  CELL_GCM_TEXTURE_B8;
            remap = CELL_GCM_REMAP_MODE(
                        CELL_GCM_TEXTURE_REMAP_ORDER_XYXY,
                        CELL_GCM_TEXTURE_REMAP_FROM_B,
                        CELL_GCM_TEXTURE_REMAP_FROM_B,
                        CELL_GCM_TEXTURE_REMAP_FROM_B,
                        CELL_GCM_TEXTURE_REMAP_FROM_B,
                        CELL_GCM_TEXTURE_REMAP_REMAP,
                        CELL_GCM_TEXTURE_REMAP_REMAP,
                        CELL_GCM_TEXTURE_REMAP_REMAP,
                        CELL_GCM_TEXTURE_REMAP_REMAP );

        }
        break;
        case JSGCM_INTENSITY16:            // in_rgba = xIIx, out_rgba = IIII
        {
            gcmFormat =  CELL_GCM_TEXTURE_X16;
            remap = CELL_GCM_REMAP_MODE(
                        CELL_GCM_TEXTURE_REMAP_ORDER_XYXY,
                        CELL_GCM_TEXTURE_REMAP_FROM_G,
                        CELL_GCM_TEXTURE_REMAP_FROM_B,
                        CELL_GCM_TEXTURE_REMAP_FROM_G,
                        CELL_GCM_TEXTURE_REMAP_FROM_B,
                        CELL_GCM_TEXTURE_REMAP_REMAP,
                        CELL_GCM_TEXTURE_REMAP_REMAP,
                        CELL_GCM_TEXTURE_REMAP_REMAP,
                        CELL_GCM_TEXTURE_REMAP_REMAP );

        }
        break;
        case JSGCM_LUMINANCE8_ALPHA8:      // in_rgba = xALx, out_rgba = LLLA
        {
            gcmFormat =  CELL_GCM_TEXTURE_G8B8;
            remap = CELL_GCM_REMAP_MODE(
                        CELL_GCM_TEXTURE_REMAP_ORDER_XYXY,
                        CELL_GCM_TEXTURE_REMAP_FROM_G,
                        CELL_GCM_TEXTURE_REMAP_FROM_B,
                        CELL_GCM_TEXTURE_REMAP_FROM_B,
                        CELL_GCM_TEXTURE_REMAP_FROM_B,
                        CELL_GCM_TEXTURE_REMAP_REMAP,
                        CELL_GCM_TEXTURE_REMAP_REMAP,
                        CELL_GCM_TEXTURE_REMAP_REMAP,
                        CELL_GCM_TEXTURE_REMAP_REMAP );

        }
        break;
        case JSGCM_LUMINANCE16_ALPHA16:    // in_rgba = LALL, out_rgba = LLLA
        {
            gcmFormat =  CELL_GCM_TEXTURE_Y16_X16;
            remap = CELL_GCM_REMAP_MODE(
                        CELL_GCM_TEXTURE_REMAP_ORDER_XXXY,
                        CELL_GCM_TEXTURE_REMAP_FROM_G,
                        CELL_GCM_TEXTURE_REMAP_FROM_B,
                        CELL_GCM_TEXTURE_REMAP_FROM_A,
                        CELL_GCM_TEXTURE_REMAP_FROM_R,
                        CELL_GCM_TEXTURE_REMAP_REMAP,
                        CELL_GCM_TEXTURE_REMAP_REMAP,
                        CELL_GCM_TEXTURE_REMAP_REMAP,
                        CELL_GCM_TEXTURE_REMAP_REMAP );
        }
        break;
        case JSGCM_HILO8:                  // in_rgba = HLxx, out_rgba = HL11
        {
            gcmFormat =  CELL_GCM_TEXTURE_COMPRESSED_HILO8;
            remap = CELL_GCM_REMAP_MODE(
                        CELL_GCM_TEXTURE_REMAP_ORDER_XYXY,
                        CELL_GCM_TEXTURE_REMAP_FROM_A,
                        CELL_GCM_TEXTURE_REMAP_FROM_R,
                        CELL_GCM_TEXTURE_REMAP_FROM_G,
                        CELL_GCM_TEXTURE_REMAP_FROM_B,
                        CELL_GCM_TEXTURE_REMAP_ONE,
                        CELL_GCM_TEXTURE_REMAP_REMAP,
                        CELL_GCM_TEXTURE_REMAP_REMAP,
                        CELL_GCM_TEXTURE_REMAP_ONE );

        }
        break;
        case JSGCM_HILO16:                 // in_rgba = HLxx, out_rgba = HL11
        {
            gcmFormat =  CELL_GCM_TEXTURE_Y16_X16;
            remap = CELL_GCM_REMAP_MODE(
                        CELL_GCM_TEXTURE_REMAP_ORDER_XYXY,
                        CELL_GCM_TEXTURE_REMAP_FROM_A,
                        CELL_GCM_TEXTURE_REMAP_FROM_R,
                        CELL_GCM_TEXTURE_REMAP_FROM_G,
                        CELL_GCM_TEXTURE_REMAP_FROM_B,
                        CELL_GCM_TEXTURE_REMAP_ONE,
                        CELL_GCM_TEXTURE_REMAP_REMAP,
                        CELL_GCM_TEXTURE_REMAP_REMAP,
                        CELL_GCM_TEXTURE_REMAP_ONE );

        }
        break;
        case JSGCM_ARGB8:                  // in_rgba = RGBA, out_rgba = RGBA
        {
            gcmFormat =  CELL_GCM_TEXTURE_A8R8G8B8;
            remap = CELL_GCM_REMAP_MODE(
                        CELL_GCM_TEXTURE_REMAP_ORDER_XYXY,
                        CELL_GCM_TEXTURE_REMAP_FROM_A,
                        CELL_GCM_TEXTURE_REMAP_FROM_R,
                        CELL_GCM_TEXTURE_REMAP_FROM_G,
                        CELL_GCM_TEXTURE_REMAP_FROM_B,
                        CELL_GCM_TEXTURE_REMAP_REMAP,
                        CELL_GCM_TEXTURE_REMAP_REMAP,
                        CELL_GCM_TEXTURE_REMAP_REMAP,
                        CELL_GCM_TEXTURE_REMAP_REMAP );

        }
        break;
        case JSGCM_BGRA8:                  // in_rgba = GRAB, out_rgba = RGBA ** NEEDS TO BE TESTED
        {
            gcmFormat =  CELL_GCM_TEXTURE_A8R8G8B8;
            remap = CELL_GCM_REMAP_MODE(
                        CELL_GCM_TEXTURE_REMAP_ORDER_XYXY,
                        CELL_GCM_TEXTURE_REMAP_FROM_B,
                        CELL_GCM_TEXTURE_REMAP_FROM_G,
                        CELL_GCM_TEXTURE_REMAP_FROM_R,
                        CELL_GCM_TEXTURE_REMAP_FROM_A,
                        CELL_GCM_TEXTURE_REMAP_REMAP,
                        CELL_GCM_TEXTURE_REMAP_REMAP,
                        CELL_GCM_TEXTURE_REMAP_REMAP,
                        CELL_GCM_TEXTURE_REMAP_REMAP );

        }
        break;
        case JSGCM_RGBA8:                  // in_rgba = GBAR, out_rgba = RGBA ** NEEDS TO BE TESTED
        {
            gcmFormat =  CELL_GCM_TEXTURE_A8R8G8B8;
            remap = CELL_GCM_REMAP_MODE(
                        CELL_GCM_TEXTURE_REMAP_ORDER_XYXY,
                        CELL_GCM_TEXTURE_REMAP_FROM_B,
                        CELL_GCM_TEXTURE_REMAP_FROM_A,
                        CELL_GCM_TEXTURE_REMAP_FROM_R,
                        CELL_GCM_TEXTURE_REMAP_FROM_G,
                        CELL_GCM_TEXTURE_REMAP_REMAP,
                        CELL_GCM_TEXTURE_REMAP_REMAP,
                        CELL_GCM_TEXTURE_REMAP_REMAP,
                        CELL_GCM_TEXTURE_REMAP_REMAP );
        }
        break;
        case JSGCM_ABGR8:                  // in_rgba = BGRA, out_rgba = RGBA  ** NEEDS TO BE TESTED
        {
            gcmFormat =  CELL_GCM_TEXTURE_A8R8G8B8;
            remap = CELL_GCM_REMAP_MODE(
                        CELL_GCM_TEXTURE_REMAP_ORDER_XYXY,
                        CELL_GCM_TEXTURE_REMAP_FROM_A,
                        CELL_GCM_TEXTURE_REMAP_FROM_B,
                        CELL_GCM_TEXTURE_REMAP_FROM_G,
                        CELL_GCM_TEXTURE_REMAP_FROM_R,
                        CELL_GCM_TEXTURE_REMAP_REMAP,
                        CELL_GCM_TEXTURE_REMAP_REMAP,
                        CELL_GCM_TEXTURE_REMAP_REMAP,
                        CELL_GCM_TEXTURE_REMAP_REMAP );

        }
        break;
        case JSGCM_RGBX8:                  // in_rgba = BGRA, out_rgba = RGB1  ** NEEDS TO BE TESTED
        {
            gcmFormat =  CELL_GCM_TEXTURE_A8R8G8B8;
            remap = CELL_GCM_REMAP_MODE(
                        CELL_GCM_TEXTURE_REMAP_ORDER_XYXY,
                        CELL_GCM_TEXTURE_REMAP_FROM_B,
                        CELL_GCM_TEXTURE_REMAP_FROM_A,
                        CELL_GCM_TEXTURE_REMAP_FROM_R,
                        CELL_GCM_TEXTURE_REMAP_FROM_G,
                        CELL_GCM_TEXTURE_REMAP_ONE,
                        CELL_GCM_TEXTURE_REMAP_REMAP,
                        CELL_GCM_TEXTURE_REMAP_REMAP,
                        CELL_GCM_TEXTURE_REMAP_REMAP );

        }
        break;
        case JSGCM_XBGR8:                  // in_rgba = BGRA, out_rgba = RGB1  ** NEEDS TO BE TESTED
        {
            gcmFormat =  CELL_GCM_TEXTURE_A8R8G8B8;
            remap = CELL_GCM_REMAP_MODE(
                        CELL_GCM_TEXTURE_REMAP_ORDER_XYXY,
                        CELL_GCM_TEXTURE_REMAP_FROM_A,
                        CELL_GCM_TEXTURE_REMAP_FROM_B,
                        CELL_GCM_TEXTURE_REMAP_FROM_G,
                        CELL_GCM_TEXTURE_REMAP_FROM_R,
                        CELL_GCM_TEXTURE_REMAP_ONE,
                        CELL_GCM_TEXTURE_REMAP_REMAP,
                        CELL_GCM_TEXTURE_REMAP_REMAP,
                        CELL_GCM_TEXTURE_REMAP_REMAP );

        }
        break;
        case JSGCM_COMPRESSED_RGB_S3TC_DXT1: // in_rgba = RGBA, out_rgba = RGBA
        {
            gcmFormat =  CELL_GCM_TEXTURE_COMPRESSED_DXT1;
            remap = CELL_GCM_REMAP_MODE(
                        CELL_GCM_TEXTURE_REMAP_ORDER_XYXY,
                        CELL_GCM_TEXTURE_REMAP_FROM_A,
                        CELL_GCM_TEXTURE_REMAP_FROM_R,
                        CELL_GCM_TEXTURE_REMAP_FROM_G,
                        CELL_GCM_TEXTURE_REMAP_FROM_B,
                        CELL_GCM_TEXTURE_REMAP_ONE,
                        CELL_GCM_TEXTURE_REMAP_REMAP,
                        CELL_GCM_TEXTURE_REMAP_REMAP,
                        CELL_GCM_TEXTURE_REMAP_REMAP );

        }
        break;
        case JSGCM_COMPRESSED_RGBA_S3TC_DXT1: // in_rgba = RGBA, out_rgba = RGBA
        {
            gcmFormat =  CELL_GCM_TEXTURE_COMPRESSED_DXT1;
            remap = CELL_GCM_REMAP_MODE(
                        CELL_GCM_TEXTURE_REMAP_ORDER_XYXY,
                        CELL_GCM_TEXTURE_REMAP_FROM_A,
                        CELL_GCM_TEXTURE_REMAP_FROM_R,
                        CELL_GCM_TEXTURE_REMAP_FROM_G,
                        CELL_GCM_TEXTURE_REMAP_FROM_B,
                        CELL_GCM_TEXTURE_REMAP_REMAP,
                        CELL_GCM_TEXTURE_REMAP_REMAP,
                        CELL_GCM_TEXTURE_REMAP_REMAP,
                        CELL_GCM_TEXTURE_REMAP_REMAP );

        }
        break;
        case JSGCM_COMPRESSED_RGBA_S3TC_DXT3: // in_rgba = RGBA, out_rgba = RGBA
        {
            gcmFormat =  CELL_GCM_TEXTURE_COMPRESSED_DXT23;
            remap = CELL_GCM_REMAP_MODE(
                        CELL_GCM_TEXTURE_REMAP_ORDER_XYXY,
                        CELL_GCM_TEXTURE_REMAP_FROM_A,
                        CELL_GCM_TEXTURE_REMAP_FROM_R,
                        CELL_GCM_TEXTURE_REMAP_FROM_G,
                        CELL_GCM_TEXTURE_REMAP_FROM_B,
                        CELL_GCM_TEXTURE_REMAP_REMAP,
                        CELL_GCM_TEXTURE_REMAP_REMAP,
                        CELL_GCM_TEXTURE_REMAP_REMAP,
                        CELL_GCM_TEXTURE_REMAP_REMAP );

        }
        break;
        case JSGCM_COMPRESSED_RGBA_S3TC_DXT5: // in_rgba = RGBA, out_rgba = RGBA
        {
            gcmFormat =  CELL_GCM_TEXTURE_COMPRESSED_DXT45;
            remap = CELL_GCM_REMAP_MODE(
                        CELL_GCM_TEXTURE_REMAP_ORDER_XYXY,
                        CELL_GCM_TEXTURE_REMAP_FROM_A,
                        CELL_GCM_TEXTURE_REMAP_FROM_R,
                        CELL_GCM_TEXTURE_REMAP_FROM_G,
                        CELL_GCM_TEXTURE_REMAP_FROM_B,
                        CELL_GCM_TEXTURE_REMAP_REMAP,
                        CELL_GCM_TEXTURE_REMAP_REMAP,
                        CELL_GCM_TEXTURE_REMAP_REMAP,
                        CELL_GCM_TEXTURE_REMAP_REMAP );
        }
        break;
        case JSGCM_DEPTH_COMPONENT16: // in_rgba = ????, out_rgba = ????
        {
            gcmFormat =  CELL_GCM_TEXTURE_DEPTH16;
            remap = CELL_GCM_REMAP_MODE(
                        CELL_GCM_TEXTURE_REMAP_ORDER_XYXY,
                        CELL_GCM_TEXTURE_REMAP_FROM_A,
                        CELL_GCM_TEXTURE_REMAP_FROM_R,
                        CELL_GCM_TEXTURE_REMAP_FROM_G,
                        CELL_GCM_TEXTURE_REMAP_FROM_B,
                        CELL_GCM_TEXTURE_REMAP_REMAP,
                        CELL_GCM_TEXTURE_REMAP_REMAP,
                        CELL_GCM_TEXTURE_REMAP_REMAP,
                        CELL_GCM_TEXTURE_REMAP_REMAP );
        }
        break;
        case JSGCM_DEPTH_COMPONENT24: // in_rgba = ????, out_rgba = ????
        {
            gcmFormat =  CELL_GCM_TEXTURE_DEPTH24_D8;
            remap = CELL_GCM_REMAP_MODE(
                        CELL_GCM_TEXTURE_REMAP_ORDER_XYXY,
                        CELL_GCM_TEXTURE_REMAP_FROM_A,
                        CELL_GCM_TEXTURE_REMAP_FROM_R,
                        CELL_GCM_TEXTURE_REMAP_FROM_G,
                        CELL_GCM_TEXTURE_REMAP_FROM_B,
                        CELL_GCM_TEXTURE_REMAP_REMAP,
                        CELL_GCM_TEXTURE_REMAP_REMAP,
                        CELL_GCM_TEXTURE_REMAP_REMAP,
                        CELL_GCM_TEXTURE_REMAP_REMAP );
        }
        break;
        case JSGCM_FLOAT_R32:              // in_rgba = Rxxx, out_rgba = R001
        {
            gcmFormat =  CELL_GCM_TEXTURE_X32_FLOAT;
            remap = CELL_GCM_REMAP_MODE(
                        CELL_GCM_TEXTURE_REMAP_ORDER_XYXY,
                        CELL_GCM_TEXTURE_REMAP_FROM_A,
                        CELL_GCM_TEXTURE_REMAP_FROM_R,
                        CELL_GCM_TEXTURE_REMAP_FROM_G,
                        CELL_GCM_TEXTURE_REMAP_FROM_B,
                        CELL_GCM_TEXTURE_REMAP_ONE,
                        CELL_GCM_TEXTURE_REMAP_REMAP,
                        CELL_GCM_TEXTURE_REMAP_ZERO,
                        CELL_GCM_TEXTURE_REMAP_ZERO );

        }
        break;
        case JSGCM_LUMINANCE32F_ARB:
        {
            gcmFormat =  CELL_GCM_TEXTURE_X32_FLOAT;
            remap = CELL_GCM_REMAP_MODE(
                        CELL_GCM_TEXTURE_REMAP_ORDER_XYXY,
                        CELL_GCM_TEXTURE_REMAP_FROM_A,
                        CELL_GCM_TEXTURE_REMAP_FROM_R,
                        CELL_GCM_TEXTURE_REMAP_FROM_G,
                        CELL_GCM_TEXTURE_REMAP_FROM_B,
                        CELL_GCM_TEXTURE_REMAP_ONE,
                        CELL_GCM_TEXTURE_REMAP_REMAP,
                        CELL_GCM_TEXTURE_REMAP_REMAP,
                        CELL_GCM_TEXTURE_REMAP_REMAP );

        }
        break;
        case JSGCM_ALPHA_LUMINANCE16F_SCE:
        {
            // Contrary to the doc says, AL16F does not support a rerouting
            // of X and Y. There is a launch check for a straight crossbar.
            gcmFormat =  CELL_GCM_TEXTURE_Y16_X16_FLOAT;
            remap = CELL_GCM_REMAP_MODE(
                        CELL_GCM_TEXTURE_REMAP_ORDER_XXXY,
                        CELL_GCM_TEXTURE_REMAP_FROM_A,
                        CELL_GCM_TEXTURE_REMAP_FROM_R,
                        CELL_GCM_TEXTURE_REMAP_FROM_G,
                        CELL_GCM_TEXTURE_REMAP_FROM_B,
                        CELL_GCM_TEXTURE_REMAP_REMAP,
                        CELL_GCM_TEXTURE_REMAP_REMAP,
                        CELL_GCM_TEXTURE_REMAP_REMAP,
                        CELL_GCM_TEXTURE_REMAP_REMAP );

        }
        break;
        case JSGCM_FLOAT_RGBA16:          // in_rgba = RGBA, out_rgba = RGBA
        {
            gcmFormat =  CELL_GCM_TEXTURE_W16_Z16_Y16_X16_FLOAT;
            remap = CELL_GCM_REMAP_MODE(
                        CELL_GCM_TEXTURE_REMAP_ORDER_XXXY,
                        CELL_GCM_TEXTURE_REMAP_FROM_A,
                        CELL_GCM_TEXTURE_REMAP_FROM_R,
                        CELL_GCM_TEXTURE_REMAP_FROM_G,
                        CELL_GCM_TEXTURE_REMAP_FROM_B,
                        CELL_GCM_TEXTURE_REMAP_REMAP,
                        CELL_GCM_TEXTURE_REMAP_REMAP,
                        CELL_GCM_TEXTURE_REMAP_REMAP,
                        CELL_GCM_TEXTURE_REMAP_REMAP );

        }
        break;
        case JSGCM_FLOAT_RGBA32:          // in_rgba = RGBA, out_rgba = RGBA
        {
            gcmFormat =  CELL_GCM_TEXTURE_W32_Z32_Y32_X32_FLOAT;
            remap = CELL_GCM_REMAP_MODE(
                        CELL_GCM_TEXTURE_REMAP_ORDER_XXXY,
                        CELL_GCM_TEXTURE_REMAP_FROM_A,
                        CELL_GCM_TEXTURE_REMAP_FROM_R,
                        CELL_GCM_TEXTURE_REMAP_FROM_G,
                        CELL_GCM_TEXTURE_REMAP_FROM_B,
                        CELL_GCM_TEXTURE_REMAP_REMAP,
                        CELL_GCM_TEXTURE_REMAP_REMAP,
                        CELL_GCM_TEXTURE_REMAP_REMAP,
                        CELL_GCM_TEXTURE_REMAP_REMAP );

        }
        break;
        case JSGCM_FLOAT_RGBX16:          // in_rgba = RGBA, out_rgba = RGBA
        {
            gcmFormat =  CELL_GCM_TEXTURE_W16_Z16_Y16_X16_FLOAT;
            remap = CELL_GCM_REMAP_MODE(
                        CELL_GCM_TEXTURE_REMAP_ORDER_XXXY,
                        CELL_GCM_TEXTURE_REMAP_FROM_A,
                        CELL_GCM_TEXTURE_REMAP_FROM_R,
                        CELL_GCM_TEXTURE_REMAP_FROM_G,
                        CELL_GCM_TEXTURE_REMAP_FROM_B,
                        CELL_GCM_TEXTURE_REMAP_ONE,
                        CELL_GCM_TEXTURE_REMAP_REMAP,
                        CELL_GCM_TEXTURE_REMAP_REMAP,
                        CELL_GCM_TEXTURE_REMAP_REMAP );


        }
        break;
        case JSGCM_FLOAT_RGBX32:          // in_rgba = RGBA, out_rgba = RGBA
        {
            gcmFormat =  CELL_GCM_TEXTURE_W32_Z32_Y32_X32_FLOAT;
            remap = CELL_GCM_REMAP_MODE(
                        CELL_GCM_TEXTURE_REMAP_ORDER_XXXY,
                        CELL_GCM_TEXTURE_REMAP_FROM_A,
                        CELL_GCM_TEXTURE_REMAP_FROM_R,
                        CELL_GCM_TEXTURE_REMAP_FROM_G,
                        CELL_GCM_TEXTURE_REMAP_FROM_B,
                        CELL_GCM_TEXTURE_REMAP_ONE,
                        CELL_GCM_TEXTURE_REMAP_REMAP,
                        CELL_GCM_TEXTURE_REMAP_REMAP,
                        CELL_GCM_TEXTURE_REMAP_REMAP );

        }
        break;
        case JSGCM_RGB5_A1_SCE:          // in_rgba = RGBA, out_rgba = RGBA
        {
            gcmFormat =  CELL_GCM_TEXTURE_A1R5G5B5;
            remap = CELL_GCM_REMAP_MODE(
                        CELL_GCM_TEXTURE_REMAP_ORDER_XXXY,
                        CELL_GCM_TEXTURE_REMAP_FROM_A,
                        CELL_GCM_TEXTURE_REMAP_FROM_R,
                        CELL_GCM_TEXTURE_REMAP_FROM_G,
                        CELL_GCM_TEXTURE_REMAP_FROM_B,
                        CELL_GCM_TEXTURE_REMAP_REMAP,
                        CELL_GCM_TEXTURE_REMAP_REMAP,
                        CELL_GCM_TEXTURE_REMAP_REMAP,
                        CELL_GCM_TEXTURE_REMAP_REMAP );

        }
        break;
        case JSGCM_RGB565_SCE:          // in_rgba = RGBA, out_rgba = RGBA
        {
            gcmFormat =  CELL_GCM_TEXTURE_R5G6B5;
            remap = CELL_GCM_REMAP_MODE(
                        CELL_GCM_TEXTURE_REMAP_ORDER_XXXY,
                        CELL_GCM_TEXTURE_REMAP_FROM_A,
                        CELL_GCM_TEXTURE_REMAP_FROM_R,
                        CELL_GCM_TEXTURE_REMAP_FROM_G,
                        CELL_GCM_TEXTURE_REMAP_FROM_B,
                        CELL_GCM_TEXTURE_REMAP_ONE,
                        CELL_GCM_TEXTURE_REMAP_REMAP,
                        CELL_GCM_TEXTURE_REMAP_REMAP,
                        CELL_GCM_TEXTURE_REMAP_REMAP );

        }
        break;
        default:
            JS_ASSERT( 0 && "internal format not found" );

            break;
    };

    return;
}


/////////////////////////////////////////////////////////////////////////////
// Platform device interface to setting up textures.
void static inline _jsGcmFifoGlTexImage2
(
    jsGcmEnum        target,                 // JSGCM_TEXTURE_[123]D
    GLint           imageIndex,             // teximage unit 0..15
    GLint           lodCount,               // lod levels
    GLsizei         width,
    GLsizei         height,
    GLsizei         depth,
    jsGcmEnum        binaryFormat,           // raw data format gpu pulls from
    GLboolean       isSigned,               // only valid for none float formats
    GLboolean       useNormalizedTexCoords, // texel reads using [0,1] coords
    GLuint          pitch,                  // 0=swizzled else the pitch value of a linear texture
    GLboolean       isMain,
    const GLuint    imageOffset )                 // vidmem pointer to texture data
{
    uint8_t format = 0;
    uint32_t remap = 0;
    uint32_t unit = imageIndex;

    _jsGcmMapTextureFormat( binaryFormat, format, remap );

    // conversion from swizzled to linear
    if ( pitch )
        format += 0x20;

    // bit of safeguarding
    JS_ASSERT( depth >= 1 );
    JS_ASSERT(( pitch & 7 ) == 0 ); // too strict alignment?
    JS_ASSERT(( lodCount >= 1 ) && ( lodCount <= JSGCM_MAX_LOD_COUNT ) );

    // setup the GCM Texture structure
    CellGcmTexture gcmTexture;
    gcmTexture.offset = imageOffset;
    gcmTexture.format = format;
    gcmTexture.remap = remap;
    gcmTexture.width = width;
    gcmTexture.height = height;
    gcmTexture.depth = depth;
    gcmTexture.pitch = pitch;
    gcmTexture.mipmap = lodCount;
    gcmTexture.cubemap = CELL_GCM_FALSE;

    // pick dimension and cubemapping, swizzled implies P2 width/height/depth
    switch ( target )
    {
        case 0:
            // disabling the unit, arbitrary hwFormat
            gcmTexture.dimension = CELL_GCM_TEXTURE_DIMENSION_2;
            break;
        case JSGCM_TEXTURE_1D:
            JS_ASSERT(( width <= JSGCM_MAX_TEX_DIMENSION ) && ( height == 1 ) && ( depth == 1 ) );
            gcmTexture.dimension = CELL_GCM_TEXTURE_DIMENSION_1;
            break;
        case JSGCM_TEXTURE_2D:
            JS_ASSERT( pitch || ( JSGCM_IS_POW2( width ) && JSGCM_IS_POW2( height ) ) );
            JS_ASSERT(( width <= JSGCM_MAX_TEX_DIMENSION ) && ( height <= JSGCM_MAX_TEX_DIMENSION ) && ( depth == 1 ) );
            gcmTexture.dimension = CELL_GCM_TEXTURE_DIMENSION_2;
            break;
        case JSGCM_TEXTURE_3D:
            JS_ASSERT( pitch || ( JSGCM_IS_POW2( width ) && JSGCM_IS_POW2( height ) && JSGCM_IS_POW2( depth ) ) );
            // TODO: some formats with depth==512 have a bug in nv40
            //JS_ASSERT((width <= 512) && (height <= 512) && (depth <= 512));
            gcmTexture.dimension = CELL_GCM_TEXTURE_DIMENSION_3;
            break;
        case JSGCM_TEXTURE_CUBE_MAP:
            JS_ASSERT( width == height );
            JS_ASSERT( depth == 1 );
            JS_ASSERT( useNormalizedTexCoords );
            gcmTexture.dimension = CELL_GCM_TEXTURE_DIMENSION_2;
            gcmTexture.cubemap = CELL_GCM_TRUE;
            break;
        default:
            JS_ASSERT( 0 );
            break;
    }

    // finally set location in system or local texture based on the top bit of offset
    if (isMain)
        gcmTexture.location = CELL_GCM_LOCATION_MAIN;
    else
        gcmTexture.location = CELL_GCM_LOCATION_LOCAL;

    GCM_FUNC( cellGcmSetTexture, unit, &gcmTexture );
}

#endif
