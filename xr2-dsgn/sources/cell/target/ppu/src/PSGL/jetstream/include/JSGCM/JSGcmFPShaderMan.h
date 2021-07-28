/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 *                Copyright (C) 2009 Sony Computer Entertainment Inc.
 *                                               All Rights Reserved.
 */

// DESCRIPTION:	JSGCM pixel shader manager

#ifndef FIXED_PIPELINE_SHADER_MAN_H
#define FIXED_PIPELINE_SHADER_MAN_H

#include <PSGL/psgl.h>
#include <JSGCM/JSGcmFPShader.h>
#include <PSGL/Utils.h>

#define HASH_SIZE 509 // better if prime

struct jsGcmFPShaderManager
{
    jsGcmFPShaderState   currentState;
    GLfloat envColor[_JS_MAX_TEXTURE_UNITS][4];
    GLfloat fogParam[4];
    GLfloat fogColor[4];
    GLboolean dirty;
    jsGcmFPShader    *lastShader;
    jsGcmFPShader   *shaders[HASH_SIZE];
    _JS_LIST_DEFINE( missingStates, jsGcmFPShaderState )
};

#ifdef __cplusplus
extern "C"
{
#endif

    void _jsGcmFPShaderManagerInit( jsGcmFPShaderManager *manager );
    void _jsGcmFPShaderManagerDestroy( jsGcmFPShaderManager *manager );
    void _jsGcmFPAddShadersFromPool( jsGcmFPShaderManager *manager, jsCgShaderPool *pool );
    void _jsGcmFPAddShader( jsGcmFPShaderManager *manager, jsGcmFPShader *shader );
    void _jsGcmFPRemoveShader( jsGcmFPShaderManager *manager, jsGcmFPShader *shader );
    jsGcmFPShader *_jsGcmFPFindShader( jsGcmFPShaderManager *manager, const jsGcmFPShaderState *state );
    jsGcmFPShader *_jsGcmFPFindCurrentShader( jsGcmFPShaderManager *manager );
    GLboolean _jsGcmFPLoadCurrentShader( jsGcmFPShaderManager *manager );
    void _jsGcmFPShaderManagerSetTextureMode( jsGcmFPShaderManager *manager, GLuint unit, GLenum mode );
    void _jsGcmFPShaderManagerSetTextureEnvMode( jsGcmFPShaderManager *manager, GLuint unit, GLenum envMode, GLfloat *color );
    void _jsGcmFPShaderManagerSetTextureFormat( jsGcmFPShaderManager *manager, GLuint unit, GLenum format );
    void _jsGcmFPShaderManagerSetFog( jsGcmFPShaderManager *manager, GLenum mode, GLfloat *color, GLfloat start, GLfloat end, GLfloat density );

    jsCgShaderPool *_jsGcmInitDefaultShaders();
    jsGcmShader *_jsGcmFPGetDummyShader();
    jsGcmShader *_jsGcmFPGetOneTextureShader();
    void _jsGcmUninitDefaultShaders();

#ifdef __cplusplus
}
#endif

#endif
