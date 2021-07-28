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
#include <PSGL/Matrix.h>
#include <PSGL/PerfCounters.h>
#include <JSGCM/JSGcmDriver.h>
#include <string.h>
#include <math.h>
#include <Cg/CgCommon.h>

static void _jsGcmUpdateModes( PSGLcontext *LContext );

void _jsValidateFFXVertexProgram()
{
    PSGLcontext* LContext = _CurrentContext;
    JS_ASSERT( LContext );
    _jsValidateMatrices();
    _jsValidateMaterial();
    for ( int i = 0; i < _JS_MAX_TEXTURE_UNITS; ++i )
        _jsValidateTexGen( i );
    _jsGcmUpdateModes( LContext );
    _jsValidateLighting();
    // only thing to do is install the vertex program.
    // (uniforms are loaded through TNL validation)
    if ( JS_UNLIKELY( LContext->needValidate & PSGL_VALIDATE_VERTEX_PROGRAM ) )
    {
        char* ucode = NULL;
        jsGcmDriver *driver = ( jsGcmDriver * )_CurrentDevice->rasterDriver;
        _jsGcmGetUCode( driver->vsFixedPipeline, &ucode, NULL );
        // vp programs do not need to be loaded to video memory.
        _jsGcmSetNativeCgVertexProgram( driver->vsFixedPipeline, ucode );
    }
}

void _jsValidateMatrices()
{
    PSGLcontext* LContext = _CurrentContext;
    JS_ASSERT( LContext );
    jsGcmDriver *driver = _jsGetGcmDriver();
    JS_ASSERT( driver != NULL );

    jsMatrixStack*	LModelViewStack = &( LContext->ModelViewMatrixStack );
    jsMatrixStack*	LProjectionStack = &( LContext->ProjectionMatrixStack );

    // The fixed function vertex shader uses the MV and MVP matrices.
    if ( LModelViewStack->dirty || LProjectionStack->dirty )
    {
        float *MVMatrix = LModelViewStack->MatrixStackf + LModelViewStack->MatrixStackPtr * jsELEMENTS_IN_MATRIX;
        float *PMatrix = LProjectionStack->MatrixStackf + LProjectionStack->MatrixStackPtr * jsELEMENTS_IN_MATRIX;

        // MV, scaling factor, and inverse transpose
        if ( LModelViewStack->dirty )
        {
            float p[16] = {0};

            // scaling factor
            GLfloat scalingFactor = sqrtf(
                                        MVMatrix[M00] * MVMatrix[M00] +
                                        MVMatrix[M10] * MVMatrix[M10] +
                                        MVMatrix[M20] * MVMatrix[M20] );
            p[0] = p[1] = p[2] = p[3] = scalingFactor;
            _jsGcmFifoGlProgramParameterfv( driver->vsFixedPipeline,
                                            driver->normalScale, p, NULL ); // ucode

            // model view matrix
            _jsGcmFifoGlProgramParameterfv( driver->vsFixedPipeline, driver->viewXform, MVMatrix, NULL );

            // compute normal transform for fixed function lighting
            // TODO: We should put this off until it is definitely needed - when
            // using fixed function with lighting on.
            if ( !LContext->InverseModelViewValid )
            {
                jsMatrixInverse(
                    LContext->ModelViewMatrixStack.MatrixStackf +
                    LContext->ModelViewMatrixStack.MatrixStackPtr*jsELEMENTS_IN_MATRIX,
                    LContext->InverseModelViewMatrixf );

                LContext->InverseModelViewValid = GL_TRUE;
            }

            p[0] = LContext->InverseModelViewMatrixf[0];
            p[1] = LContext->InverseModelViewMatrixf[4];
            p[2] = LContext->InverseModelViewMatrixf[8];
            p[3] = LContext->InverseModelViewMatrixf[1];
            p[4] = LContext->InverseModelViewMatrixf[5];
            p[5] = LContext->InverseModelViewMatrixf[9];
            p[6] = LContext->InverseModelViewMatrixf[2];
            p[7] = LContext->InverseModelViewMatrixf[6];
            p[8] = LContext->InverseModelViewMatrixf[10];
            _jsGcmFifoGlProgramParameterfv( driver->vsFixedPipeline,
                                            driver->normXform, p, NULL ); // ucode

            LModelViewStack->dirty = GL_FALSE;
        }

        // MVP
        _jsMatrixMult4f( PMatrix, MVMatrix, LContext->LocalToScreenMatrixf );
        _jsGcmFifoGlProgramParameterfv( driver->vsFixedPipeline, driver->projXform, LContext->LocalToScreenMatrixf, NULL );
        LProjectionStack->dirty = GL_FALSE;
    }

    GLboolean enableTextureMatrix = GL_FALSE;
    for ( int i = 0;i < _JS_MAX_TEXTURE_UNITS;++i )
    {
        jsTextureCoordsUnit* tu = LContext->TextureCoordsUnits + i;
        jsMatrixStack* stack = &tu->TextureMatrixStack;
        if ( stack->dirty )
        {
            // memcmp is maybe not the best to use here, but it works.
            if ( memcmp( stack->MatrixStackf + stack->MatrixStackPtr*jsELEMENTS_IN_MATRIX, _jsIdentityMatrixf, sizeof( _jsIdentityMatrixf ) ) )
            {
                tu->enableTextureMatrix = GL_TRUE;
            }
            else tu->enableTextureMatrix = GL_FALSE;

            _jsGcmFifoGlProgramParameterfv( driver->vsFixedPipeline,
                                            driver->texunit[i].matrix, stack->MatrixStackf + stack->MatrixStackPtr*jsELEMENTS_IN_MATRIX, NULL );

            stack->dirty = GL_FALSE;
        }

        enableTextureMatrix |= tu->enableTextureMatrix;
    }

    float useTexMatrix[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
    if ( enableTextureMatrix )
        useTexMatrix[0] = 1.0f;

    _jsGcmFifoGlProgramParameterfv( driver->vsFixedPipeline,
                                    driver->useTexMatrix, useTexMatrix, NULL );

}

void _jsValidateMaterial( void )
{
    jsGcmDriver *driver = _jsGetGcmDriver();
    JS_ASSERT( driver != NULL );
    PSGLcontext *LContext = _CurrentContext;
    JS_ASSERT( LContext != NULL );

    float p[4];
    _jsGcmFifoGlProgramParameterfv( driver->vsFixedPipeline,
                                    driver->material.Ka, ( float* )&LContext->FrontMaterial.Ambient, NULL ); // ucode
    _jsGcmFifoGlProgramParameterfv( driver->vsFixedPipeline,
                                    driver->material.Kd, ( float* )&LContext->FrontMaterial.Diffuse, NULL );
    _jsGcmFifoGlProgramParameterfv( driver->vsFixedPipeline,
                                    driver->material.Ks, ( float* )&LContext->FrontMaterial.Specular, NULL );
    _jsGcmFifoGlProgramParameterfv( driver->vsFixedPipeline,
                                    driver->material.Ke, ( float* )&LContext->FrontMaterial.Emission, NULL );

    p[0] = p[1] = p[2] = p[3] = LContext->FrontMaterial.Shininess;
    _jsGcmFifoGlProgramParameterfv( driver->vsFixedPipeline,
                                    driver->material.shininess, ( float* )&LContext->FrontMaterial.Shininess, NULL );

}

// Several modes - lighting, two-sided lighting, color material, and
// normalize are collected into one Cg parameter to save constant
// registers.  This helper function sets them all.
static void _jsGcmUpdateModes( PSGLcontext *LContext )
{
    jsGcmDriver *driver = _jsGetGcmDriver();
    JS_ASSERT( driver != NULL );

    float p[4];
    p[0] = LContext->Lighting ? 1.0f : 0.0f;
    p[1] = LContext->LightModel.Light2Sided ? 1.0f : 0.0f;
    p[2] = LContext->ColorMaterial ? 1.0f : 0.0f;
    p[3] = LContext->Normalize ? 1.0f : 0.0f;

    _jsGcmFifoGlProgramParameterfv( driver->vsFixedPipeline,
                                    driver->flags, p, NULL ); // ucode
}

void _jsValidateLighting()
{
    jsGcmDriver *driver = _jsGetGcmDriver();
    JS_ASSERT( driver != NULL );
    PSGLcontext *LContext = _CurrentContext;
    JS_ASSERT( LContext != NULL );

    // set lighting flag, two-sided flag in vertex shader
    _jsGcmUpdateModes( LContext );

    // global ambient light
    _jsGcmFifoGlProgramParameterfv( driver->vsFixedPipeline,
                                    driver->globalAmbient, ( float* )&LContext->LightModel.Ambient, NULL ); // ucode

    int nLights = 0;
    for ( int i = 0; i < _JS_MAX_LIGHTNUM; ++i )
    {
        // skip unused lights
        if ( !LContext->Lights[i].Active )
            continue;

        float p[4];

        // position
        _jsGcmFifoGlProgramParameterfv( driver->vsFixedPipeline,
                                        driver->light[nLights].position, ( float* )&LContext->Lights[i].ViewerPosition, NULL );

        // component colors
        _jsGcmFifoGlProgramParameterfv( driver->vsFixedPipeline, driver->light[nLights].ambient,
                                        ( float* )&LContext->Lights[i].Ambient, NULL );
        _jsGcmFifoGlProgramParameterfv( driver->vsFixedPipeline, driver->light[nLights].diffuse,
                                        ( float* )&LContext->Lights[i].Diffuse, NULL );
        _jsGcmFifoGlProgramParameterfv( driver->vsFixedPipeline, driver->light[nLights].specular,
                                        ( float* )&LContext->Lights[i].Specular, NULL );

        // spot
        p[0] = LContext->Lights[i].SpotViewerDirection.X;
        p[1] = LContext->Lights[i].SpotViewerDirection.Y;
        p[2] = LContext->Lights[i].SpotViewerDirection.Z;
        _jsGcmFifoGlProgramParameterfv( driver->vsFixedPipeline,
                                        driver->light[nLights].spotDirection, p, NULL );
        p[0] = LContext->Lights[i].SpotCosCutOff;
        p[1] = LContext->Lights[i].SpotExponent;
        _jsGcmFifoGlProgramParameterfv( driver->vsFixedPipeline,
                                        driver->light[nLights].spotParams, p, NULL );

        // attenuation
        p[0] = LContext->Lights[i].ConstantAttenuation;
        p[1] = LContext->Lights[i].LinearAttenuation;
        p[2] = LContext->Lights[i].QuadraticAttenuation;
        _jsGcmFifoGlProgramParameterfv( driver->vsFixedPipeline,
                                        driver->light[nLights].attenuation, p, NULL );

        ++nLights;
    }

    // set number of active lights
    float p[4] = {( float )nLights, 0.0f, 0.0f, 0.0f };
    _jsGcmFifoGlProgramParameterfv( driver->vsFixedPipeline,
                                    driver->nLights, p, NULL );

}

void _jsValidateTexGen( int unit )
{

    jsGcmDriver *driver = _jsGetGcmDriver();
    JS_ASSERT( driver != NULL );
    PSGLcontext *LContext = _CurrentContext;
    JS_ASSERT( unit < _JS_MAX_TEXTURE_UNITS );
    jsTextureCoordsUnit *tu = LContext->TextureCoordsUnits + unit;
    GLuint validate = tu->revalidate;
    if ( validate&_JS_TEXTURE_UNIT_REVALIDATE_TEX_GEN )
    {
        // Separate parameters are used for each texgen type because
        // attempts to use a single texgen mode parameter exposes bugs
        // in Cg.
        // TODO: Use a single parameter if we run out of uniforms,
        // provided that the subsystem allows it.
        float genMode[4];
        float plane[16];
        memset( plane, 0, sizeof( plane ) );
        for ( int j = 0; j < _JS_MAX_TEXTURE_GEN_COORDS; ++j )
        {
            const jsTexGen* tg = &tu->texGen[j];

            // map GL texgen mode onto shader enum
            switch ( tg->enabled ? tg->mode : 0 )
            {
                case GL_OBJECT_LINEAR :
                    genMode[j] = 1.0f;
                    memcpy( &plane[4*j], tg->objectPlane, 4*sizeof( float ) );
                    break;
                case GL_EYE_LINEAR:
                    genMode[j] = 2.0f;
                    memcpy( &plane[4*j], tg->eyePlane, 4*sizeof( float ) );
                    break;
                case GL_SPHERE_MAP:
                    genMode[j] = 3.0f;
                    break;
                case GL_REFLECTION_MAP:
                    genMode[j] = 4.0f;
                    break;
                case GL_NORMAL_MAP:
                    genMode[j] = 5.0f;
                    break;
                case 0:
                default:
                    genMode[j] = 0.0f;
                    break;
            }

            _jsGcmFifoGlProgramParameterfv(
                driver->vsFixedPipeline,
                driver->texunit[unit].genMode,
                genMode,
                NULL );
            _jsGcmFifoGlProgramParameterfv(
                driver->vsFixedPipeline,
                driver->texunit[unit].genPlane,
                plane,
                NULL );
        }
    }
    tu->revalidate &= ~_JS_TEXTURE_UNIT_REVALIDATE_TEX_GEN;

}

#ifdef JS_DPM
int __jsNumVertexProgramsValidated;
#endif

void _jsValidateVertexProgram()
{
#ifdef JS_DPM
	__jsNumVertexProgramsValidated++;
#endif

    // if validation is required, it means the program  has to be downloaded.
    PSGLcontext*	LContext = _CurrentContext;
    JS_ASSERT( LContext->VertexProgram && LContext->BoundVertexProgram != NULL );

    _JS_PERFORMANCE_START( ValidateVertexProgramCounter );
	_jsSetNativeCgVertexProgram(LContext->BoundVertexProgram);
    _JS_PERFORMANCE_STOP( ValidateVertexProgramCounter );

    // Set all uniforms.
	if(!(LContext->needValidate & PSGL_VALIDATE_VERTEX_CONSTANTS) && LContext->BoundVertexProgram->parentContext)
        _jsValidateVertexConstants();
}

void _jsValidateVertexConstants()
{
    _JS_PERFORMANCE_START( ValidateVertexConstantsCounter );
    PSGLcontext*	LContext = _CurrentContext;

    // adding the check for parentContext will catch the case when a cgb shader is bound and we reached this call
    // this can happen when states are all invalidated after flip and glFlush is called before the next vertex program is bound    
    // when the last vertex program bound before flip was cgb which does not support the notion of vertex constant validation
    if (LContext->BoundVertexProgram->parentContext == NULL)
	return;

    JS_ASSERT( LContext->VertexProgram && LContext->BoundVertexProgram != NULL);

    // this assumes that no shared bits are set in the program and no unshared bits are set in the context
    _jsPlatformSetBoolVertexRegisters( LContext->BoundVertexProgram->controlFlowBools |
                                       LContext->BoundVertexProgram->parentContext->controlFlowBoolsShared );

    _jsGcmPushProgramPushBuffer( LContext->BoundVertexProgram );

    _JS_PERFORMANCE_STOP( ValidateVertexConstantsCounter );

}

int _jsPlatformGenerateVertexProgram( _CGprogram *program, const CgProgramHeader *programHeader, const void *ucode,
                                      const CgParameterTableHeader *parameterHeader, const char *stringTable, const float *defaultValues )
{
    // we currently have the same interface for vertex and fragment programs.
    return _jsGcmGenerateProgram( program, VERTEX_PROFILE_INDEX, programHeader,
                                  ucode, parameterHeader, NULL, stringTable, defaultValues );

}


CGbool _jsPlatformSupportsVertexProgram( CGprofile p )
{
    if ( p == CG_PROFILE_SCE_VP_TYPEB )
        return CG_TRUE;
    if ( p == CG_PROFILE_SCE_VP_TYPEC )
        return CG_TRUE;
    if ( p == CG_PROFILE_SCE_VP_RSX )
        return CG_TRUE;
    return CG_FALSE;
}
