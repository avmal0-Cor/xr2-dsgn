/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 *                Copyright (C) 2009 Sony Computer Entertainment Inc.
 *                                               All Rights Reserved.
 */


#ifndef _JSGCM_SHADER_POOL_H
#define _JSGCM_SHADER_POOL_H

#include <PSGL/psgl.h>
#include <JSGCM/JSGcmFPShader.h>

#ifdef __cplusplus // [RSTENSON] ToDo Look in to removing these.  We shouldn't need them anymore.
extern "C"
{
#endif

    struct jsGcmShaderSerial
    {
        GLuint programOffset;
        GLuint dummy;
    };

    struct jsGcmFPShaderSerial
    {
        GLuint dummy1;
        jsGcmFPShaderState state;
        jsGcmShaderSerial nvShader;
        GLuint dummy2[_JS_MAX_TEXTURE_UNITS];
        GLuint dummy3;
        GLuint dummy4;
    };

    void _jsGcmFPShaderPoolCreate( jsCgShaderPool *pool, int shaderCount );
    void _jsGcmFPShaderPoolFree( jsCgShaderPool *pool );
    void _jsGcmFPPrepareShader( jsGcmFPShader *shader );
    int _jsGcmFPShaderPoolLoad( jsCgShaderPool *pool, const char *fileName );

#ifdef __cplusplus
}
#endif

#endif
