/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 *                Copyright (C) 2009 Sony Computer Entertainment Inc.
 *                                               All Rights Reserved.
 */

// DESCRIPTION:	JSGCM Fixed Pipeline shaders

#ifndef JSGCM_FP_SHADER_H
#define JSGCM_FP_SHADER_H

#include <PSGL/Types.h>
#include <PSGL/Utils.h>
#include <JSGCM/JSGcmShader.h>

#define _FP_STATE_LOAD_NONE 0
#define _FP_STATE_LOAD_2D 1
#define _FP_STATE_LOAD_3D 2
#define _FP_STATE_LOAD_CUBE 3

#define _FP_STATE_TEXENV_MODULATE 0
#define _FP_STATE_TEXENV_DECAL 1
#define _FP_STATE_TEXENV_BLEND 2
#define _FP_STATE_TEXENV_REPLACE 3
#define _FP_STATE_TEXENV_ADD 4

#define _FP_STATE_FORMAT_A 0
#define _FP_STATE_FORMAT_L 1
#define _FP_STATE_FORMAT_LA 2
#define _FP_STATE_FORMAT_RGB 3
#define _FP_STATE_FORMAT_RGBA 4
#define _FP_STATE_FORMAT_I 5

#define _FP_STATE_FOG_NONE 0
#define _FP_STATE_FOG_LINEAR 1
#define _FP_STATE_FOG_EXP 2
#define _FP_STATE_FOG_EXP2 3

#define _CC_PARAM_NAME "cc"
#define _FOG_PARAM_NAME "fogParam"
#define _FOG_COLOR_NAME "fogColor"

typedef struct _jsGcmFPShaderState_
{
    GLubyte loadType[_JS_MAX_TEXTURE_UNITS];
    GLubyte texEnv[_JS_MAX_TEXTURE_UNITS];
    GLubyte texFormat[_JS_MAX_TEXTURE_UNITS];
    GLubyte fog;
}
jsGcmFPShaderState;

struct jsGcmFPShader
{
    GLuint refCount;
    struct jsGcmFPShader *next;
    jsGcmFPShaderState state;
    jsGcmShader *nvShader;
    CgBinaryParameter *ccParam[_JS_MAX_TEXTURE_UNITS];
    CgBinaryParameter *fogParam;
    CgBinaryParameter *fogColor;
};

#define JS_SHADER_POOL_MAGIC 0x5046534a //"JSFP";
#define JS_SHADER_POOL_VERSION 0x10007

typedef struct _jsCgShaderPoolHeader_
{
    GLuint magic;
    GLuint version;
    GLuint shaderCount;
    GLuint cgSize;
}
jsCgShaderPoolHeader;

typedef struct _jsCgShaderPool_
{
    _JS_LIST_DEFINE( shaders, jsGcmFPShader )
}
jsCgShaderPool;

#endif
