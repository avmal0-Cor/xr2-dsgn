/* Copyright (C)2001-2006 by NVIDIA Corporation.  All rights reserved.
 * Modified by Sony Computer Entertainment Inc under license from NVIDIA Corporation.
 * Copyright (C) 2009 Sony Computer Entertainment Inc.
 * 
 * SCEI CONFIDENTIAL
 */


/*
 * JSGcmGlEnums.h
 */

#ifndef _H_JSGCM_GL_ENUMS_H_
#define _H_JSGCM_GL_ENUMS_H_

#include <PSGL/psgl.h>

// Boolean
#define JSGCM_FALSE                          0
#define JSGCM_TRUE                           1

typedef enum jsGcmEnum
{
    // gleSetRenderTarget
    JSGCM_NONE                           = 0x0000,

    // glDrawArrays, glDrawElements, glBegin
    JSGCM_POINTS                         = 0x0000,
    JSGCM_LINES                          = 0x0001,
    JSGCM_LINE_LOOP                      = 0x0002,
    JSGCM_LINE_STRIP                     = 0x0003,
    JSGCM_TRIANGLES                      = 0x0004,
    JSGCM_TRIANGLE_STRIP                 = 0x0005,
    JSGCM_TRIANGLE_FAN                   = 0x0006,
    JSGCM_QUADS                          = 0x0007,
    JSGCM_QUAD_STRIP                     = 0x0008,
    JSGCM_POLYGON                        = 0x0009,

    // glClear
    JSGCM_DEPTH_BUFFER_BIT               = 0x0100,
    JSGCM_STENCIL_BUFFER_BIT             = 0x0400,
    JSGCM_COLOR_BUFFER_BIT               = 0x4000,

    // glBlendFunc, glStencilFunc
    JSGCM_ZERO                           =      0,
    JSGCM_ONE                            =      1,
    JSGCM_SRC_COLOR                      = 0x0300,
    JSGCM_ONE_MINUS_SRC_COLOR            = 0x0301,
    JSGCM_SRC_ALPHA                      = 0x0302,
    JSGCM_ONE_MINUS_SRC_ALPHA            = 0x0303,
    JSGCM_DST_ALPHA                      = 0x0304,
    JSGCM_ONE_MINUS_DST_ALPHA            = 0x0305,
    JSGCM_DST_COLOR                      = 0x0306,
    JSGCM_ONE_MINUS_DST_COLOR            = 0x0307,
    JSGCM_SRC_ALPHA_SATURATE             = 0x0308,

    // glAlphaFunc, glDepthFunc, glStencilFunc
    JSGCM_NEVER                          = 0x0200,
    JSGCM_LESS                           = 0x0201,
    JSGCM_EQUAL                          = 0x0202,
    JSGCM_LEQUAL                         = 0x0203,
    JSGCM_GREATER                        = 0x0204,
    JSGCM_NOTEQUAL                       = 0x0205,
    JSGCM_GEQUAL                         = 0x0206,
    JSGCM_ALWAYS                         = 0x0207,

    // glCullFace/etc
    JSGCM_FRONT                          = 0x0404,
    JSGCM_BACK                           = 0x0405,
    JSGCM_LEFT                           = 0x0406,
    JSGCM_RIGHT                          = 0x0407,
    JSGCM_FRONT_AND_BACK                 = 0x0408,

    // glFrontFace
    JSGCM_CW                             = 0x0900,
    JSGCM_CCW                            = 0x0901,

    // glLogicOp
    JSGCM_CLEAR                          = 0x1500,
    JSGCM_AND                            = 0x1501,
    JSGCM_AND_REVERSE                    = 0x1502,
    JSGCM_COPY                           = 0x1503,
    JSGCM_AND_INVERTED                   = 0x1504,
    JSGCM_NOOP                           = 0x1505,
    JSGCM_XOR                            = 0x1506,
    JSGCM_OR                             = 0x1507,
    JSGCM_NOR                            = 0x1508,
    JSGCM_EQUIV                          = 0x1509,
    JSGCM_INVERT                         = 0x150A,
    JSGCM_OR_REVERSE                     = 0x150B,
    JSGCM_COPY_INVERTED                  = 0x150C,
    JSGCM_OR_INVERTED                    = 0x150D,
    JSGCM_NAND                           = 0x150E,
    JSGCM_SET                            = 0x150F,

    // glShadingModel
    JSGCM_FLAT                           = 0x1D00,
    JSGCM_SMOOTH                         = 0x1D01,

    // glStencilOp
    JSGCM_KEEP                           = 0x1E00,
    JSGCM_REPLACE                        = 0x1E01,
    JSGCM_INCR                           = 0x1E02,
    JSGCM_DECR                           = 0x1E03,
    JSGCM_INCR_WRAP                      = 0x8507,
    JSGCM_DECR_WRAP                      = 0x8508,

    // BlendFunc
    JSGCM_CONSTANT_COLOR                 = 0x8001,
    JSGCM_ONE_MINUS_CONSTANT_COLOR       = 0x8002,
    JSGCM_CONSTANT_ALPHA                 = 0x8003,
    JSGCM_ONE_MINUS_CONSTANT_ALPHA       = 0x8004,
    JSGCM_BLEND_COLOR                    = 0x8005,
    JSGCM_FUNC_ADD                       = 0x8006,
    JSGCM_MIN                            = 0x8007,
    JSGCM_MAX                            = 0x8008,
    JSGCM_BLEND_EQUATION                 = 0x8009,
    JSGCM_FUNC_SUBTRACT                  = 0x800A,
    JSGCM_FUNC_REVERSE_SUBTRACT          = 0x800B,

    // glTexImage binary formats -- keep in sync with glTexImage tables!
    JSGCM_LUMINANCE8                     = 0x8040,
    JSGCM_LUMINANCE16                    = 0x8042,
    JSGCM_ALPHA8                         = 0x803C,
    JSGCM_ALPHA16                        = 0x803E,
    JSGCM_INTENSITY8                     = 0x804B,
    JSGCM_INTENSITY16                    = 0x804D,
    JSGCM_LUMINANCE8_ALPHA8              = 0x8045,
    JSGCM_LUMINANCE16_ALPHA16            = 0x8048,
    JSGCM_HILO8                          = 0x885E,
    JSGCM_HILO16                         = 0x86F8,
    JSGCM_ARGB8                          = 0x6007, // does not exist in classic OpenGL
    JSGCM_BGRA8                          = 0xff01, // does not exist in classic OpenGL
    JSGCM_RGBA8                          = 0x8058,
    JSGCM_ABGR8                          = 0xff02, // does not exist in classic OpenGL
    JSGCM_XBGR8                          = 0xff03, // does not exist in classic OpenGL
    JSGCM_RGBX8                          = 0xff07, // does not exist in classic OpenGL
    JSGCM_COMPRESSED_RGB_S3TC_DXT1       = 0x83F0,
    JSGCM_COMPRESSED_RGBA_S3TC_DXT1      = 0x83F1,
    JSGCM_COMPRESSED_RGBA_S3TC_DXT3      = 0x83F2,
    JSGCM_COMPRESSED_RGBA_S3TC_DXT5      = 0x83F3,
    JSGCM_DEPTH_COMPONENT16              = 0x81A5,
    JSGCM_DEPTH_COMPONENT24              = 0x81A6,
    JSGCM_FLOAT_R32                      = 0x8885,
    JSGCM_FLOAT_RGBA16                   = 0x888A,
    JSGCM_FLOAT_RGBA32                   = 0x888B,
    JSGCM_FLOAT_RGBX16                   = 0xff04,
    JSGCM_FLOAT_RGBX32                   = 0xff05,
    JSGCM_LUMINANCE32F_ARB               = 0x8818,
    JSGCM_ALPHA_LUMINANCE16F_SCE         = 0x600B,
    JSGCM_RGB5_A1_SCE                    = 0x600C,
    JSGCM_RGB565_SCE                     = 0x600D,

    // glEnable/glDisable
    JSGCM_ALPHA_TEST                     = 0x0bc0,
    JSGCM_BLEND                          = 0x0be0,
    JSGCM_COLOR_LOGIC_OP                 = 0x0bf2,
    JSGCM_CULL_FACE                      = 0x0b44,
    JSGCM_DEPTH_BOUNDS_TEST              = 0x8890,
    JSGCM_DEPTH_TEST                     = 0x0b71,
    JSGCM_DITHER                         = 0x0bd0,
    JSGCM_SCISSOR_TEST                   = 0x0c11,
    JSGCM_STENCIL_TEST                   = 0x0b90,
    JSGCM_STENCIL_TEST_TWO_SIDE          = 0x8910,
    JSGCM_LIGHT_MODEL_TWO_SIDE           = 0x0b52,
    JSGCM_POLYGON_OFFSET_FILL            = 0x8037,
    JSGCM_POLYGON_OFFSET_LINE            = 0x2A02,
    JSGCM_PRIMITIVE_RESTART              = 0x8558,
    JSGCM_PSHADER_SRGB_REMAPPING         = 0xff06,
    JSGCM_POINT_SPRITE                   = 0xff08,

    // glVertexAttribPointer
    JSGCM_VERTEX_ATTRIB_ARRAY0           = 0x8650,
    JSGCM_VERTEX_ATTRIB_ARRAY1           = 0x8651,
    JSGCM_VERTEX_ATTRIB_ARRAY2           = 0x8652,
    JSGCM_VERTEX_ATTRIB_ARRAY3           = 0x8653,
    JSGCM_VERTEX_ATTRIB_ARRAY4           = 0x8654,
    JSGCM_VERTEX_ATTRIB_ARRAY5           = 0x8655,
    JSGCM_VERTEX_ATTRIB_ARRAY6           = 0x8656,
    JSGCM_VERTEX_ATTRIB_ARRAY7           = 0x8657,
    JSGCM_VERTEX_ATTRIB_ARRAY8           = 0x8658,
    JSGCM_VERTEX_ATTRIB_ARRAY9           = 0x8659,
    JSGCM_VERTEX_ATTRIB_ARRAY10          = 0x865a,
    JSGCM_VERTEX_ATTRIB_ARRAY11          = 0x865b,
    JSGCM_VERTEX_ATTRIB_ARRAY12          = 0x865c,
    JSGCM_VERTEX_ATTRIB_ARRAY13          = 0x865d,
    JSGCM_VERTEX_ATTRIB_ARRAY14          = 0x865e,
    JSGCM_VERTEX_ATTRIB_ARRAY15          = 0x865f,

    // glTexImage
    JSGCM_TEXTURE_3D                     = 0x806F,
    JSGCM_TEXTURE_CUBE_MAP               = 0x8513,
    JSGCM_TEXTURE_1D                     = 0x0DE0,
    JSGCM_TEXTURE_2D                     = 0x0DE1,

    // glTexParameter/TextureMagFilter
    JSGCM_NEAREST                        = 0x2600,
    JSGCM_LINEAR                         = 0x2601,
    // glTexParameter/TextureMinFilter
    JSGCM_NEAREST_MIPMAP_NEAREST         = 0x2700,
    JSGCM_LINEAR_MIPMAP_NEAREST          = 0x2701,
    JSGCM_NEAREST_MIPMAP_LINEAR          = 0x2702,
    JSGCM_LINEAR_MIPMAP_LINEAR           = 0x2703,

    // glTexParameter/TextureWrapMode
    JSGCM_CLAMP                          = 0x2900,
    JSGCM_REPEAT                         = 0x2901,
    JSGCM_CLAMP_TO_EDGE                  = 0x812F,
    JSGCM_CLAMP_TO_BORDER                = 0x812D,
    JSGCM_MIRRORED_REPEAT                = 0x8370,
    JSGCM_MIRROR_CLAMP                   = 0x8742,
    JSGCM_MIRROR_CLAMP_TO_EDGE           = 0x8743,
    JSGCM_MIRROR_CLAMP_TO_BORDER         = 0x8912,

    // glTexParameter/GammaRemap
    JSGCM_GAMMA_REMAP_RED_BIT            = 0x0001,
    JSGCM_GAMMA_REMAP_GREEN_BIT          = 0x0002,
    JSGCM_GAMMA_REMAP_BLUE_BIT           = 0x0004,
    JSGCM_GAMMA_REMAP_ALPHA_BIT          = 0x0008,

    // glTexParameter
    JSGCM_TEXTURE_WRAP_S                 = 0x2802,
    JSGCM_TEXTURE_WRAP_T                 = 0x2803,
    JSGCM_TEXTURE_WRAP_R                 = 0x8072,
    JSGCM_TEXTURE_MIN_FILTER             = 0x2801,
    JSGCM_TEXTURE_MAG_FILTER             = 0x2800,
    JSGCM_TEXTURE_MAX_ANISOTROPY         = 0x84FE,
    JSGCM_TEXTURE_COMPARE_FUNC           = 0x884D,
    JSGCM_TEXTURE_MIN_LOD                = 0x813A,
    JSGCM_TEXTURE_MAX_LOD                = 0x813B,
    JSGCM_TEXTURE_LOD_BIAS               = 0x8501,
    JSGCM_TEXTURE_BORDER_COLOR           = 0x1004,
    JSGCM_TEXTURE_GAMMA_REMAP            = 0xff30,

    // ARB_vertex_program
    JSGCM_VERTEX_PROGRAM                 = 0x8620,
    JSGCM_FRAGMENT_PROGRAM               = 0x8804,

    // glVertexAttribPointer
    JSGCM_FLOAT                          = 0x1406,
    JSGCM_HALF_FLOAT                     = 0x140B,
    JSGCM_SHORT                          = 0x1402,
    JSGCM_UNSIGNED_BYTE                  = 0x1401,
    JSGCM_UNSIGNED_SHORT                 = 0x1403,
    JSGCM_UNSIGNED_INT                   = 0x1405,
    JSGCM_BYTE                           = 0x1400,
    JSGCM_INT                            = 0x1404,

    // query support
    JSGCM_SAMPLES_PASSED                 = 0xff10,

    // semaphore support
    JSGCM_SEMAPHORE_USING_GPU            = 0xff20,
    JSGCM_SEMAPHORE_USING_CPU            = 0xff21,
    JSGCM_SEMAPHORE_USING_GPU_NO_WRITE_FLUSH = 0xff22,

    // depth clamp
    JSGCM_DEPTH_CLAMP                    = 0x864F,

    // 11/11/10 bit 3-component attributes
    JSGCM_CMP                            = 0x6020,

} jsGcmEnum;

#endif
