/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 *                Copyright (C) 2009 Sony Computer Entertainment Inc.
 *                                               All Rights Reserved.
 */


/*
 * PSGLFX - GL States implementation
 */

#include <PSGL/psgl.h>
#include <PSGL/private.h>
#include <PSGL/Matrix.h>
#include <PSGLFX/Context.h>
#include <PSGLFX/State.h>
#include <PSGLFX/StateAssignment.h>
#include <string.h>
#include <limits.h>

// SIMPLE_BOOLEAN_STATE(NAME,FIELD,VALIDATE)
#define DEFINE_SIMPLE_BOOLEAN_STATES \
    SIMPLE_BOOLEAN_STATE(AlphaTestEnable,AlphaTest,PSGL_VALIDATE_ALPHA_TEST) \
    SIMPLE_BOOLEAN_STATE(BlendEnable,Blending,PSGL_VALIDATE_BLENDING) \
    SIMPLE_BOOLEAN_STATE(ColorLogicOpEnable,ColorLogicOp,PSGL_VALIDATE_LOGIC_OP) \
    SIMPLE_BOOLEAN_STATE(CullFaceEnable,FaceCulling,PSGL_VALIDATE_FACE_CULL) \
    SIMPLE_BOOLEAN_STATE(DepthTestEnable,DepthTest,PSGL_VALIDATE_DEPTH_TEST) \
    SIMPLE_BOOLEAN_STATE(DitherEnable,Dithering,0) \
    SIMPLE_BOOLEAN_STATE(FogEnable,Fog,PSGL_VALIDATE_FFX_FRAGMENT_PROGRAM) \
    SIMPLE_BOOLEAN_STATE(LightingEnable,Lighting,PSGL_VALIDATE_FFX_VERTEX_PROGRAM) \
    SIMPLE_BOOLEAN_STATE(LineSmoothEnable,LineSmooth,PSGL_VALIDATE_LINE_RASTER) \
    SIMPLE_BOOLEAN_STATE(MultisampleEnable,Multisample,PSGL_VALIDATE_MULTISAMPLING) \
    SIMPLE_BOOLEAN_STATE(NormalizeEnable,Normalize,PSGL_VALIDATE_FFX_VERTEX_PROGRAM) \
    SIMPLE_BOOLEAN_STATE(PointSmoothEnable,PointSmooth,PSGL_VALIDATE_POINT_RASTER) \
    SIMPLE_BOOLEAN_STATE(PointSpriteEnable,PointSprite,PSGL_VALIDATE_POINT_SPRITE) \
    SIMPLE_BOOLEAN_STATE(PolygonOffsetFillEnable,PolygonOffsetFill,PSGL_VALIDATE_POLYGON_OFFSET) \
    SIMPLE_BOOLEAN_STATE(PolygonOffsetLineEnable,PolygonOffsetLine,PSGL_VALIDATE_POLYGON_OFFSET) \
    SIMPLE_BOOLEAN_STATE(RescaleNormalEnable,RescaleNormals,PSGL_VALIDATE_FFX_VERTEX_PROGRAM) \
    SIMPLE_BOOLEAN_STATE(SampleAlphaToCoverageEnable,SampleAlphaToCoverage,PSGL_VALIDATE_MULTISAMPLING) \
    SIMPLE_BOOLEAN_STATE(SampleAlphaToOneEnable,SampleAlphaToOne,PSGL_VALIDATE_MULTISAMPLING) \
    SIMPLE_BOOLEAN_STATE(SampleCoverageEnable,SampleCoverage,PSGL_VALIDATE_MULTISAMPLING) \
    SIMPLE_BOOLEAN_STATE(ScissorTestEnable,ScissorTest,PSGL_VALIDATE_SCISSOR_BOX) \
    SIMPLE_BOOLEAN_STATE(ShaderSRGBRemapEnable,ShaderSRGBRemap,PSGL_VALIDATE_SHADER_SRGB_REMAP) \
    SIMPLE_BOOLEAN_STATE(StencilTestEnable,StencilTest,PSGL_VALIDATE_STENCIL_TEST) \
    SIMPLE_BOOLEAN_STATE(VertexProgramPointSizeEnable,VertexProgramPointSize,PSGL_VALIDATE_POINT_SPRITE)

// INDEXED_BOOLEAN_STATE(NAME,MAX,CONTEXT_FIELD,SUB_FIELD,VALIDATE,EXTRA_VALIDATION)
#define DEFINE_INDEXED_BOOLEAN_STATES \
    INDEXED_BOOLEAN_STATE(ClipPlaneEnable,_JS_MAX_CLIP_PLANES,ClipPlane,Active,PSGL_VALIDATE_CLIP_PLANES,) \
    INDEXED_BOOLEAN_STATE(LightEnable,_JS_MAX_LIGHTNUM,Lights,Active,PSGL_VALIDATE_FFX_VERTEX_PROGRAM,) \
    INDEXED_BOOLEAN_STATE(TexGenSEnable,_JS_MAX_TEXTURE_COORDS,TextureCoordsUnits,texGen[0].enabled,PSGL_VALIDATE_FFX_VERTEX_PROGRAM,_CurrentContext->TextureCoordsUnits[index].revalidate|=_JS_TEXTURE_UNIT_REVALIDATE_TEX_GEN;) \
    INDEXED_BOOLEAN_STATE(TexGenTEnable,_JS_MAX_TEXTURE_COORDS,TextureCoordsUnits,texGen[1].enabled,PSGL_VALIDATE_FFX_VERTEX_PROGRAM,_CurrentContext->TextureCoordsUnits[index].revalidate|=_JS_TEXTURE_UNIT_REVALIDATE_TEX_GEN;) \
    INDEXED_BOOLEAN_STATE(TexGenREnable,_JS_MAX_TEXTURE_COORDS,TextureCoordsUnits,texGen[2].enabled,PSGL_VALIDATE_FFX_VERTEX_PROGRAM,_CurrentContext->TextureCoordsUnits[index].revalidate|=_JS_TEXTURE_UNIT_REVALIDATE_TEX_GEN;) \
    INDEXED_BOOLEAN_STATE(TexGenQEnable,_JS_MAX_TEXTURE_COORDS,TextureCoordsUnits,texGen[3].enabled,PSGL_VALIDATE_FFX_VERTEX_PROGRAM,_CurrentContext->TextureCoordsUnits[index].revalidate|=_JS_TEXTURE_UNIT_REVALIDATE_TEX_GEN;) \
    INDEXED_BOOLEAN_STATE(Texture2DEnable,_JS_MAX_TEXTURE_IMAGE_UNITS,TextureImageUnits,enable2D,PSGL_VALIDATE_FFX_FRAGMENT_PROGRAM | PSGL_VALIDATE_TEXTURES_USED, \
            { \
                _jsUpdateCurrentTextureCache(_CurrentContext->TextureImageUnits+index); \
            } ) \
    INDEXED_BOOLEAN_STATE(Texture3DEnable,_JS_MAX_TEXTURE_IMAGE_UNITS,TextureImageUnits,enable3D,PSGL_VALIDATE_FFX_FRAGMENT_PROGRAM | PSGL_VALIDATE_TEXTURES_USED, \
            { \
                _jsUpdateCurrentTextureCache(_CurrentContext->TextureImageUnits+index); \
            } ) \
    INDEXED_BOOLEAN_STATE(TextureCubeMapEnable,_JS_MAX_TEXTURE_IMAGE_UNITS,TextureImageUnits,enableCube,PSGL_VALIDATE_FFX_FRAGMENT_PROGRAM | PSGL_VALIDATE_TEXTURES_USED, \
            { \
                _jsUpdateCurrentTextureCache(_CurrentContext->TextureImageUnits+index); \
            } )

// GENERIC_STATE(NAME,TYPE,N,FLAG,SET,RESET,VALIDATE)
#define DEFINE_GENERIC_STATES \
    GENERIC_STATE(AlphaFunc,FLOAT,2,PSGL_VALIDATE_ALPHA_TEST, \
            {   \
                const float *float_values=static_cast<const float *>(value.getArrayData()); \
                _CurrentContext->AlphaFunc=(GLenum)float_values[0]; \
                _CurrentContext->AlphaRef=float_values[1]; \
            }, \
            { \
                _CurrentContext->AlphaFunc=GL_ALWAYS; \
                _CurrentContext->AlphaRef=0.f; \
            } , \
            { \
                float *float_values=static_cast<float *>(value.getArrayData()); \
                float_values[1]=_jsClampf(float_values[1]); \
                return _jsIsTestFunc((GLenum)float_values[0]); \
            } \
            ) \
    GENERIC_STATE(BlendFunc,INT,2,PSGL_VALIDATE_BLENDING, \
            {   \
                const int *int_values=static_cast<const int *>(value.getArrayData()); \
                _CurrentContext->BlendFactorSrcRGB=(GLenum)int_values[0]; \
                _CurrentContext->BlendFactorSrcAlpha=(GLenum)int_values[0]; \
                _CurrentContext->BlendFactorDestRGB=(GLenum)int_values[1]; \
                _CurrentContext->BlendFactorDestAlpha=(GLenum)int_values[1]; \
            }, \
            { \
                _CurrentContext->BlendFactorSrcRGB=GL_ONE; \
                _CurrentContext->BlendFactorSrcAlpha=GL_ONE; \
                _CurrentContext->BlendFactorDestRGB=GL_ZERO; \
                _CurrentContext->BlendFactorDestAlpha=GL_ZERO; \
            } , \
            { \
                int *int_values=static_cast<int *>(value.getArrayData()); \
                return _jsIsSrcBlendFunc((GLenum)int_values[0])&&_jsIsDstBlendFunc((GLenum)int_values[1]); \
            } \
        ) \
    GENERIC_STATE(BlendFuncSeparate,INT,4,PSGL_VALIDATE_BLENDING, \
            {   \
                const int *int_values=static_cast<const int *>(value.getArrayData()); \
                _CurrentContext->BlendFactorSrcRGB=(GLenum)int_values[0]; \
                _CurrentContext->BlendFactorDestRGB=(GLenum)int_values[1]; \
                _CurrentContext->BlendFactorSrcAlpha=(GLenum)int_values[2]; \
                _CurrentContext->BlendFactorDestAlpha=(GLenum)int_values[3]; \
            }, \
            { \
                _CurrentContext->BlendFactorSrcRGB=GL_ONE; \
                _CurrentContext->BlendFactorSrcAlpha=GL_ONE; \
                _CurrentContext->BlendFactorDestRGB=GL_ZERO; \
                _CurrentContext->BlendFactorDestAlpha=GL_ZERO; \
            } , \
            { \
                int *int_values=static_cast<int *>(value.getArrayData()); \
                return  _jsIsSrcBlendFunc((GLenum)int_values[0])&& \
                        _jsIsDstBlendFunc((GLenum)int_values[1])&& \
                        _jsIsSrcBlendFunc((GLenum)int_values[2])&& \
                        _jsIsDstBlendFunc((GLenum)int_values[3]); \
            } \
        ) \
    GENERIC_STATE(BlendEquation,INT,1,PSGL_VALIDATE_BLENDING, \
            _CurrentContext->BlendEquationRGB=_CurrentContext->BlendEquationAlpha=value.getInt();, \
            _CurrentContext->BlendEquationRGB=_CurrentContext->BlendEquationAlpha=GL_FUNC_ADD;, \
            return _jsIsBlendEquation(value.getInt()); \
        ) \
    GENERIC_STATE(BlendEquationSeparate,INT,2,PSGL_VALIDATE_BLENDING, \
            {   \
                const int *int_values=static_cast<const int *>(value.getArrayData()); \
                _CurrentContext->BlendEquationRGB=(GLenum)int_values[0]; \
                _CurrentContext->BlendEquationAlpha=(GLenum)int_values[1]; \
            }, \
            _CurrentContext->BlendEquationRGB=_CurrentContext->BlendEquationAlpha=GL_FUNC_ADD;, \
            { \
                int *int_values=static_cast<int *>(value.getArrayData()); \
                return  _jsIsBlendEquation((GLenum)int_values[0])&& \
                        _jsIsBlendEquation((GLenum)int_values[1]); \
            } \
        ) \
    GENERIC_STATE(BlendColor,FLOAT,4,PSGL_VALIDATE_BLENDING, \
            setColor(_CurrentContext->BlendColor, value);, \
            resetColor(_CurrentContext->BlendColor);, \
            validateColor(value); return true; \
        ) \
    GENERIC_STATE(ClearColor,FLOAT,4,0, \
            setColor(_CurrentContext->ClearColor, value);, \
            resetColor(_CurrentContext->ClearColor);, \
            validateColor(value); return true; \
        ) \
    GENERIC_STATE(ClearStencil,INT,1,0, \
            _CurrentContext->ClearStencil=value.getInt();, \
            _CurrentContext->ClearStencil=0;, \
            return true; \
        ) \
    GENERIC_STATE(ClearDepth,FLOAT,1,0, \
            _CurrentContext->ClearDepth=value.getFloat();, \
            _CurrentContext->ClearDepth=0;, \
            { \
                float *float_values=static_cast<float *>(value.getArrayData()); \
                float_values[0]=_jsClampf(float_values[0]); \
                return true; \
            } \
        ) \
    GENERIC_STATE(ClipPlane,FLOAT,4,PSGL_VALIDATE_CLIP_PLANES, \
            { \
                const float *float_values=static_cast<const float *>(value.getArrayData()); \
                _jsSetClipPlane(_CurrentContext,_CurrentContext->ClipPlane+sa->getArrayIndex(), float_values); \
            }, \
            { \
                float *c=_CurrentContext->ClipPlane[sa->getArrayIndex()].Coeff; \
                c[0]=c[1]=c[2]=c[3]=0.f; \
            }, \
            return (sa->getArrayIndex()<_JS_MAX_CLIP_PLANES); \
        ) \
    GENERIC_STATE(ColorMask,BOOL,4,PSGL_VALIDATE_WRITE_MASK|PSGL_VALIDATE_FRAGMENT_PROGRAM, \
            { \
                const CGbool *bool_values=static_cast<const CGbool *>(value.getArrayData()); \
                _CurrentContext->WriteMask &= ~_JS_CONTEXT_COLOR_MASK; \
                _CurrentContext->WriteMask |= bool_values[0]*_JS_CONTEXT_RED_MASK \
                    | bool_values[1]*_JS_CONTEXT_GREEN_MASK \
                    | bool_values[2]*_JS_CONTEXT_BLUE_MASK \
                    | bool_values[3]*_JS_CONTEXT_ALPHA_MASK; \
            }, \
            _CurrentContext->WriteMask |= _JS_CONTEXT_COLOR_MASK;, \
            return true; \
        ) \
    GENERIC_STATE(CullFace,INT,1,PSGL_VALIDATE_FACE_CULL, \
            _CurrentContext->CullFaceMode=(GLenum)value.getInt();, \
            _CurrentContext->CullFaceMode=GL_BACK;, \
            return _jsIsFaceMode((GLenum)value.getInt()); \
        ) \
    GENERIC_STATE(DepthFunc,INT,1,PSGL_VALIDATE_DEPTH_TEST, \
            _CurrentContext->DepthFunc=(GLenum)value.getInt();, \
            _CurrentContext->DepthFunc=GL_LESS;, \
            return _jsIsTestFunc((GLenum)value.getInt()); \
        ) \
    GENERIC_STATE(DepthMask,BOOL,1,PSGL_VALIDATE_WRITE_MASK, \
            _CurrentContext->WriteMask &= ~_JS_CONTEXT_DEPTH_MASK; \
            _CurrentContext->WriteMask |= value.getBool()*_JS_CONTEXT_DEPTH_MASK;, \
            _CurrentContext->WriteMask |= _JS_CONTEXT_DEPTH_MASK;, \
            return true; \
        ) \
    GENERIC_STATE(DepthRange,FLOAT,2,PSGL_VALIDATE_VIEWPORT, \
            {   \
                const float *float_values=static_cast<const float *>(value.getArrayData()); \
                _CurrentContext->DepthNear=float_values[0]; \
                _CurrentContext->DepthFar=float_values[1]; \
            }, \
            _CurrentContext->DepthNear=0.f; _CurrentContext->DepthFar=1.f;, \
            { \
                float *float_values=static_cast<float *>(value.getArrayData()); \
                float_values[0]=_jsClampf(float_values[0]); \
                float_values[1]=_jsClampf(float_values[1]); \
                return true; \
            } \
        ) \
    GENERIC_STATE(FogDensity,FLOAT,1,PSGL_VALIDATE_FFX_FRAGMENT_PROGRAM, \
            _CurrentContext->FogDensity=value.getFloat();, \
            _CurrentContext->FogDensity=1.f;, \
            return (value.getFloat()>=0.f); \
        ) \
    GENERIC_STATE(FogMode,INT,1,PSGL_VALIDATE_FFX_FRAGMENT_PROGRAM, \
            _CurrentContext->FogMode=(GLenum)value.getInt();, \
            _CurrentContext->FogMode=GL_EXP;, \
            return _jsIsFogMode((GLenum)value.getInt()); \
        ) \
    GENERIC_STATE(FogStart,FLOAT,1,PSGL_VALIDATE_FFX_FRAGMENT_PROGRAM, \
            _CurrentContext->FogStart=value.getFloat();, \
            _CurrentContext->FogStart=0.f;, \
            return true; \
        ) \
    GENERIC_STATE(FogEnd,FLOAT,1,PSGL_VALIDATE_FFX_FRAGMENT_PROGRAM, \
            _CurrentContext->FogEnd=value.getFloat();, \
            _CurrentContext->FogEnd=1.f;, \
            return true; \
        ) \
    GENERIC_STATE(FogColor,FLOAT,4,PSGL_VALIDATE_FFX_FRAGMENT_PROGRAM, \
            setColor(_CurrentContext->FogColor, value);, \
            resetColor(_CurrentContext->FogColor);, \
            validateColor(value); return true; \
        ) \
    GENERIC_STATE(FrontFace,INT,1,PSGL_VALIDATE_FACE_CULL, \
            _CurrentContext->FrontFace=(GLenum)value.getInt();, \
            _CurrentContext->FrontFace=GL_CCW;, \
            return _jsIsFaceOrientation((GLenum)value.getInt()); \
        ) \
    GENERIC_STATE(FragmentProgram,PROGRAM,1,0, \
            _cgGLBindFragmentProgram(value.getProgram()); _jsLeaveFFXFP(_CurrentContext);, \
            _cgGLUnbindFragmentProgram(); _CurrentContext->FragmentProgram = GL_FALSE; _jsEnterFFXFP(_CurrentContext);, \
            { \
                _CGprogram *prog=value.getProgram(); \
                if (prog) \
                { \
                    CGprofile profile=(CGprofile)prog->header.profile; \
                    return ((profile==CG_PROFILE_SCE_FP_TYPEB)||(profile==CG_PROFILE_SCE_FP_RSX)); \
                } \
                else return false; \
            } \
        ) \
    GENERIC_STATE(LightModelAmbient,FLOAT,4,PSGL_VALIDATE_FFX_VERTEX_PROGRAM, \
            setColor(_CurrentContext->LightModel.Ambient, value);, \
            resetColor(_CurrentContext->LightModel.Ambient);, \
            validateColor(value); return true; \
        ) \
    GENERIC_STATE(LightModelTwoSideEnable,BOOL,1,PSGL_VALIDATE_FFX_VERTEX_PROGRAM, \
            _CurrentContext->LightModel.Light2Sided = value.getBool();, \
            _CurrentContext->LightModel.Light2Sided = GL_FALSE;, \
            return true; \
        ) \
    GENERIC_STATE(LightAmbient,FLOAT,4,PSGL_VALIDATE_FFX_VERTEX_PROGRAM, \
            setColor(_CurrentContext->Lights[sa->getArrayIndex()].Ambient, value);, \
            resetColor1(_CurrentContext->Lights[sa->getArrayIndex()].Ambient);, \
            return (sa->getArrayIndex()<_JS_MAX_LIGHTNUM); \
        ) \
    GENERIC_STATE(LightConstantAttenuation,FLOAT,1,PSGL_VALIDATE_FFX_VERTEX_PROGRAM, \
            _CurrentContext->Lights[sa->getArrayIndex()].ConstantAttenuation=value.getFloat();, \
            _CurrentContext->Lights[sa->getArrayIndex()].ConstantAttenuation=1.f;, \
            return (sa->getArrayIndex()<_JS_MAX_LIGHTNUM); \
        ) \
    GENERIC_STATE(LightDiffuse,FLOAT,4,PSGL_VALIDATE_FFX_VERTEX_PROGRAM, \
            setColor(_CurrentContext->Lights[sa->getArrayIndex()].Diffuse, value);, \
            if (sa->getArrayIndex()>0) resetColor(_CurrentContext->Lights[sa->getArrayIndex()].Diffuse); \
            else \
            { \
                _CurrentContext->Lights[0].Diffuse.R= \
                _CurrentContext->Lights[0].Diffuse.G= \
                _CurrentContext->Lights[0].Diffuse.B= \
                _CurrentContext->Lights[0].Diffuse.A=1.f; \
            }, \
            return (sa->getArrayIndex()<_JS_MAX_LIGHTNUM); \
        ) \
    GENERIC_STATE(LightLinearAttenuation,FLOAT,1,PSGL_VALIDATE_FFX_VERTEX_PROGRAM, \
            _CurrentContext->Lights[sa->getArrayIndex()].LinearAttenuation=value.getFloat();, \
            _CurrentContext->Lights[sa->getArrayIndex()].LinearAttenuation=0.f;, \
            return (sa->getArrayIndex()<_JS_MAX_LIGHTNUM); \
        ) \
    GENERIC_STATE(LightPosition,FLOAT,4,PSGL_VALIDATE_FFX_VERTEX_PROGRAM, \
            _jsSetLightPosition(_CurrentContext->Lights+sa->getArrayIndex(),static_cast<const float*>(value.getArrayData()));, \
            _CurrentContext->Lights[sa->getArrayIndex()].Position.X=0.f; \
            _CurrentContext->Lights[sa->getArrayIndex()].Position.Y=0.f; \
            _CurrentContext->Lights[sa->getArrayIndex()].Position.Z=1.f; \
            _CurrentContext->Lights[sa->getArrayIndex()].Position.W=0.f; \
            _CurrentContext->Lights[sa->getArrayIndex()].ViewerPosition=_CurrentContext->Lights[sa->getArrayIndex()].Position;, \
            return (sa->getArrayIndex()<_JS_MAX_LIGHTNUM); \
        ) \
    GENERIC_STATE(LightQuadraticAttenuation,FLOAT,1,PSGL_VALIDATE_FFX_VERTEX_PROGRAM, \
            _CurrentContext->Lights[sa->getArrayIndex()].QuadraticAttenuation=value.getFloat();, \
            _CurrentContext->Lights[sa->getArrayIndex()].QuadraticAttenuation=0.f;, \
            return (sa->getArrayIndex()<_JS_MAX_LIGHTNUM); \
        ) \
    GENERIC_STATE(LightSpecular,FLOAT,4,PSGL_VALIDATE_FFX_VERTEX_PROGRAM, \
            setColor(_CurrentContext->Lights[sa->getArrayIndex()].Specular, value);, \
            if (sa->getArrayIndex()>0) resetColor(_CurrentContext->Lights[sa->getArrayIndex()].Diffuse); \
            else \
            { \
                _CurrentContext->Lights[0].Specular.R= \
                _CurrentContext->Lights[0].Specular.G= \
                _CurrentContext->Lights[0].Specular.B= \
                _CurrentContext->Lights[0].Specular.A=1.f; \
            }, \
            return (sa->getArrayIndex()<_JS_MAX_LIGHTNUM); \
        ) \
    GENERIC_STATE(LightSpotCutoff,FLOAT,1,PSGL_VALIDATE_FFX_VERTEX_PROGRAM, \
            _jsSetLightCutoff(_CurrentContext->Lights+sa->getArrayIndex(),value.getFloat());, \
            _CurrentContext->Lights[sa->getArrayIndex()].SpotCutOff=180.f; \
            _CurrentContext->Lights[sa->getArrayIndex()].SpotCosCutOff=-1.f;, \
            { \
                float param=value.getFloat(); \
                return (sa->getArrayIndex()<_JS_MAX_LIGHTNUM)&&((param >= 0.f && param <= 90.f) || param == 180.f); \
            } \
        ) \
    GENERIC_STATE(LightSpotExponent,FLOAT,1,PSGL_VALIDATE_FFX_VERTEX_PROGRAM, \
            _CurrentContext->Lights[sa->getArrayIndex()].SpotExponent=value.getFloat();, \
            _CurrentContext->Lights[sa->getArrayIndex()].SpotExponent=0.f;, \
            { \
                float param=value.getFloat(); \
                return (sa->getArrayIndex()<_JS_MAX_LIGHTNUM)&&(param >= 0.f) && (param <= 128.f); \
            } \
        ) \
    GENERIC_STATE(LightSpotDirection,FLOAT,3,PSGL_VALIDATE_FFX_VERTEX_PROGRAM, \
            _jsSetLightSpotDirection(_CurrentContext->Lights+sa->getArrayIndex(),static_cast<const float*>(value.getArrayData()));, \
            _CurrentContext->Lights[sa->getArrayIndex()].SpotDirection.X=0.f; \
            _CurrentContext->Lights[sa->getArrayIndex()].SpotDirection.Y=0.f; \
            _CurrentContext->Lights[sa->getArrayIndex()].SpotDirection.Z=-1.f; \
            _CurrentContext->Lights[sa->getArrayIndex()].SpotViewerDirection=_CurrentContext->Lights[sa->getArrayIndex()].SpotDirection;, \
            return (sa->getArrayIndex()<_JS_MAX_LIGHTNUM); \
        ) \
    GENERIC_STATE(LineWidth,FLOAT,1,PSGL_VALIDATE_LINE_RASTER, \
            _CurrentContext->LineWidth=value.getFloat();, \
            _CurrentContext->LineWidth=1.f;, \
            return (value.getFloat()>0.f); \
        ) \
    GENERIC_STATE(LogicOp,INT,1,PSGL_VALIDATE_LOGIC_OP, \
            _CurrentContext->LogicOp=(GLenum)value.getInt();, \
            _CurrentContext->LogicOp=GL_COPY;, \
            return _jsIsLogicOp((GLenum)value.getInt()); \
        ) \
    GENERIC_STATE(MaterialAmbient,FLOAT,4,PSGL_VALIDATE_FFX_VERTEX_PROGRAM, \
            setColor(_CurrentContext->FrontMaterial.Ambient, value); \
            _CurrentContext->BackMaterial.Ambient=_CurrentContext->FrontMaterial.Ambient;, \
            _CurrentContext->FrontMaterial.Ambient.R= \
            _CurrentContext->FrontMaterial.Ambient.G= \
            _CurrentContext->FrontMaterial.Ambient.B=0.2f; \
            _CurrentContext->FrontMaterial.Ambient.A=1.f; \
            _CurrentContext->BackMaterial.Ambient=_CurrentContext->FrontMaterial.Ambient;, \
            return true; \
        ) \
    GENERIC_STATE(MaterialDiffuse,FLOAT,4,PSGL_VALIDATE_FFX_VERTEX_PROGRAM, \
            setColor(_CurrentContext->FrontMaterial.Diffuse, value); \
            _CurrentContext->BackMaterial.Diffuse=_CurrentContext->FrontMaterial.Diffuse;, \
            _CurrentContext->FrontMaterial.Diffuse.R= \
            _CurrentContext->FrontMaterial.Diffuse.G= \
            _CurrentContext->FrontMaterial.Diffuse.B=0.8f; \
            _CurrentContext->FrontMaterial.Diffuse.A=1.f; \
            _CurrentContext->BackMaterial.Diffuse=_CurrentContext->FrontMaterial.Diffuse;, \
            return true; \
        ) \
    GENERIC_STATE(MaterialShininess,FLOAT,1,PSGL_VALIDATE_FFX_VERTEX_PROGRAM, \
            _CurrentContext->FrontMaterial.Shininess=value.getFloat(); \
            _CurrentContext->BackMaterial.Shininess=value.getFloat();, \
            _CurrentContext->FrontMaterial.Shininess=0.f; \
            _CurrentContext->BackMaterial.Shininess=0.f;, \
            if ((value.getFloat()<0.f)||(value.getFloat()>128.f)) return false; else return true; \
        ) \
    GENERIC_STATE(MaterialSpecular,FLOAT,4,PSGL_VALIDATE_FFX_VERTEX_PROGRAM, \
            setColor(_CurrentContext->FrontMaterial.Specular, value); \
            _CurrentContext->BackMaterial.Specular=_CurrentContext->FrontMaterial.Specular;, \
            resetColor1(_CurrentContext->FrontMaterial.Specular); \
            _CurrentContext->BackMaterial.Specular=_CurrentContext->FrontMaterial.Specular;, \
            return true; \
        ) \
    GENERIC_STATE(MaterialEmission,FLOAT,4,PSGL_VALIDATE_FFX_VERTEX_PROGRAM, \
            setColor(_CurrentContext->FrontMaterial.Emission, value); \
            _CurrentContext->BackMaterial.Emission=_CurrentContext->FrontMaterial.Emission;, \
            resetColor1(_CurrentContext->FrontMaterial.Emission); \
            _CurrentContext->BackMaterial.Emission=_CurrentContext->FrontMaterial.Emission;, \
            return true; \
        ) \
    GENERIC_STATE(ModelViewMatrix,FLOAT,16,PSGL_VALIDATE_FFX_VERTEX_PROGRAM, \
            { \
                jsMatrixStack &stack = _CurrentContext->ModelViewMatrixStack; \
                __builtin_memcpy(stack.MatrixStackf+stack.MatrixStackPtr*jsELEMENTS_IN_MATRIX,value.getArrayData(),jsMATRIX_SIZEf); \
                stack.dirty=GL_TRUE; \
                _CurrentContext->InverseModelViewValid=GL_FALSE; \
            }, \
            { \
                jsMatrixStack &stack = _CurrentContext->ModelViewMatrixStack; \
                __builtin_memcpy(stack.MatrixStackf+stack.MatrixStackPtr*jsELEMENTS_IN_MATRIX,_jsIdentityMatrixf,jsMATRIX_SIZEf); \
                stack.dirty=GL_TRUE; \
                _CurrentContext->InverseModelViewValid=GL_FALSE; \
            }, \
            return true; \
        ) \
    GENERIC_STATE(PointSize,FLOAT,1,PSGL_VALIDATE_POINT_RASTER, \
            _CurrentContext->PointSize=value.getFloat();, \
            _CurrentContext->PointSize=1.f;, \
            return (value.getFloat()>0.f); \
        ) \
    GENERIC_STATE(PointSpriteCoordReplace,BOOL,1,PSGL_VALIDATE_POINT_SPRITE, \
            _CurrentContext->TexCoordReplaceMask &= ~(1<<sa->getArrayIndex()); \
            _CurrentContext->TexCoordReplaceMask |= value.getBool()<<sa->getArrayIndex();, \
            _CurrentContext->TexCoordReplaceMask &= ~(1<<sa->getArrayIndex());, \
            return (sa->getArrayIndex()<_JS_MAX_TEXTURE_COORD_INTERPOLANTS); \
        ) \
    GENERIC_STATE(PolygonMode,INT,2,PSGL_VALIDATE_POLYGON_MODE, \
            {   \
                const int *int_values=static_cast<const int *>(value.getArrayData()); \
                switch(int_values[0]) \
                { \
                    case GL_FRONT: \
                        _CurrentContext->FrontPolygonMode=int_values[1]; \
                        break; \
                    case GL_BACK: \
                        _CurrentContext->BackPolygonMode=int_values[1]; \
                        break; \
                    case GL_FRONT_AND_BACK: \
                        _CurrentContext->FrontPolygonMode=int_values[1]; \
                        _CurrentContext->BackPolygonMode=int_values[1]; \
                        break; \
                    default: \
                        JS_ASSERT(0); \
                        break; \
                } \
            }, \
            _CurrentContext->FrontPolygonMode=_CurrentContext->BackPolygonMode=GL_FILL; , \
            { \
                int *int_values=static_cast<int *>(value.getArrayData()); \
                return _jsIsFaceMode((GLenum)int_values[0])&&_jsIsPolygonMode((GLenum)int_values[1]); \
            } \
        ) \
    GENERIC_STATE(ProjectionMatrix,FLOAT,16,PSGL_VALIDATE_FFX_VERTEX_PROGRAM, \
            { \
                jsMatrixStack &stack = _CurrentContext->ProjectionMatrixStack; \
                __builtin_memcpy(stack.MatrixStackf+stack.MatrixStackPtr*jsELEMENTS_IN_MATRIX,value.getArrayData(),jsMATRIX_SIZEf); \
                stack.dirty=GL_TRUE; \
            }, \
            { \
                jsMatrixStack &stack = _CurrentContext->ProjectionMatrixStack; \
                __builtin_memcpy(stack.MatrixStackf+stack.MatrixStackPtr*jsELEMENTS_IN_MATRIX,_jsIdentityMatrixf,jsMATRIX_SIZEf); \
                stack.dirty=GL_TRUE; \
            }, \
            return true; \
        ) \
    GENERIC_STATE(PolygonOffset,FLOAT,2,PSGL_VALIDATE_POLYGON_OFFSET, \
            { \
                const float *float_values=static_cast<const float *>(value.getArrayData()); \
                _CurrentContext->PolygonOffsetFactor=float_values[0]; \
                _CurrentContext->PolygonOffsetUnits=float_values[1]; \
            }, \
            _CurrentContext->PolygonOffsetFactor=0.f; \
            _CurrentContext->PolygonOffsetUnits=0.f;, \
            return true; \
        ) \
    GENERIC_STATE(Scissor,INT,4,PSGL_VALIDATE_SCISSOR_BOX, \
            {   \
                const int *int_values=static_cast<const int *>(value.getArrayData()); \
                _CurrentContext->ScissorBox.X=int_values[0]; \
                _CurrentContext->ScissorBox.Y=int_values[1]; \
                _CurrentContext->ScissorBox.XSize=int_values[2]; \
                _CurrentContext->ScissorBox.YSize=int_values[3]; \
            }, \
            _CurrentContext->ScissorBox.X=_CurrentContext->ScissorBox.Y=0; \
            _CurrentContext->ScissorBox.XSize=_CurrentContext->ScissorBox.YSize=INT_MAX;, \
            { \
                int *int_values=static_cast<int *>(value.getArrayData()); \
                return (int_values[2]>=0) && (int_values[3]>=0); \
            } \
        ) \
    GENERIC_STATE(ShadeModel,INT,1,PSGL_VALIDATE_SHADE_MODEL, \
            _CurrentContext->ShadeModel=(GLenum)value.getInt();, \
            _CurrentContext->ShadeModel=GL_SMOOTH;, \
            return _jsIsShadeModel((GLenum)value.getInt()); \
        ) \
    GENERIC_STATE(StencilFunc,INT,3,PSGL_VALIDATE_STENCIL_TEST, \
            {   \
                const int *int_values=static_cast<const int *>(value.getArrayData()); \
                _CurrentContext->StencilFunc=int_values[0]; \
                _CurrentContext->StencilRef=int_values[1]; \
                _CurrentContext->StencilFuncMask=int_values[2]; \
            }, \
            _CurrentContext->StencilFunc=GL_ALWAYS; \
            _CurrentContext->StencilRef=0; \
            _CurrentContext->StencilFuncMask=~0;, \
            { \
                int *int_values=static_cast<int *>(value.getArrayData()); \
                return (_jsIsTestFunc((GLenum)int_values[0])); \
            } \
        ) \
    GENERIC_STATE(StencilMask,INT,1,PSGL_VALIDATE_STENCIL_OP_AND_MASK, \
            _CurrentContext->StencilWriteMask=value.getInt();, \
            _CurrentContext->StencilWriteMask=~0;, \
            return true; \
        ) \
    GENERIC_STATE(StencilOp,INT,3,PSGL_VALIDATE_STENCIL_OP_AND_MASK, \
            {   \
                const int *int_values=static_cast<const int *>(value.getArrayData()); \
                _CurrentContext->StencilOpFail=int_values[0]; \
                _CurrentContext->StencilOpZFail=int_values[1]; \
                _CurrentContext->StencilOpZPass=int_values[2]; \
            }, \
            _CurrentContext->StencilOpFail=GL_KEEP; \
            _CurrentContext->StencilOpZFail=GL_KEEP; \
            _CurrentContext->StencilOpZPass=GL_KEEP;, \
            { \
                int *int_values=static_cast<int *>(value.getArrayData()); \
                return _jsIsStencilOp((GLenum)int_values[0])&& \
                    _jsIsStencilOp((GLenum)int_values[1])&& \
                    _jsIsStencilOp((GLenum)int_values[2]); \
            } \
        ) \
    GENERIC_STATE(StencilFuncSeparate,INT,4,PSGL_VALIDATE_STENCIL_TEST, \
            { \
                const int *int_values=static_cast<const int *>(value.getArrayData()); \
                if ((int_values[0]==GL_FRONT)||(int_values[0]==GL_FRONT_AND_BACK)) \
                { \
                    _CurrentContext->StencilFunc=int_values[1]; \
                    _CurrentContext->StencilRef=int_values[2]; \
                    _CurrentContext->StencilFuncMask=int_values[3]; \
                } \
                if ((int_values[0]==GL_BACK)||(int_values[0]==GL_FRONT_AND_BACK)) \
                { \
                    _CurrentContext->StencilBackfaceFunc=int_values[1]; \
                    _CurrentContext->StencilBackfaceRef=int_values[2]; \
                    _CurrentContext->StencilBackfaceFuncMask=int_values[3]; \
                } \
            }, \
            _CurrentContext->StencilFunc=_CurrentContext->StencilBackfaceFunc=GL_ALWAYS; \
            _CurrentContext->StencilRef=_CurrentContext->StencilBackfaceRef=0; \
            _CurrentContext->StencilFuncMask=_CurrentContext->StencilBackfaceFuncMask=~0;, \
            { \
                int *int_values=static_cast<int *>(value.getArrayData()); \
                return _jsIsFaceMode((GLenum)int_values[0])&&_jsIsTestFunc((GLenum)int_values[1]); \
            } \
        ) \
    GENERIC_STATE(StencilMaskSeparate,INT,2,PSGL_VALIDATE_STENCIL_OP_AND_MASK, \
            { \
                const int *int_values=static_cast<const int *>(value.getArrayData()); \
                if ((int_values[0]==GL_FRONT)||(int_values[0]==GL_FRONT_AND_BACK)) _CurrentContext->StencilWriteMask=value.getInt(); \
                if ((int_values[0]==GL_BACK)||(int_values[0]==GL_FRONT_AND_BACK)) _CurrentContext->StencilBackfaceWriteMask=value.getInt(); \
            }, \
            _CurrentContext->StencilWriteMask=_CurrentContext->StencilBackfaceWriteMask=~0;, \
            { \
                int *int_values=static_cast<int *>(value.getArrayData()); \
                return _jsIsFaceMode((GLenum)int_values[0]); \
            } \
        ) \
    GENERIC_STATE(StencilOpSeparate,INT,4,PSGL_VALIDATE_STENCIL_OP_AND_MASK, \
            { \
                const int *int_values=static_cast<const int *>(value.getArrayData()); \
                if ((int_values[0]==GL_FRONT)||(int_values[0]==GL_FRONT_AND_BACK)) \
                { \
                    _CurrentContext->StencilOpFail=int_values[1]; \
                    _CurrentContext->StencilOpZFail=int_values[2]; \
                    _CurrentContext->StencilOpZPass=int_values[3]; \
                } \
                if ((int_values[0]==GL_BACK)||(int_values[0]==GL_FRONT_AND_BACK)) \
                { \
                    _CurrentContext->StencilBackfaceOpFail=int_values[1]; \
                    _CurrentContext->StencilBackfaceOpZFail=int_values[2]; \
                    _CurrentContext->StencilBackfaceOpZPass=int_values[3]; \
                } \
            }, \
            _CurrentContext->StencilOpFail=GL_KEEP; \
            _CurrentContext->StencilOpZFail=GL_KEEP; \
            _CurrentContext->StencilOpZPass=GL_KEEP; \
            _CurrentContext->StencilBackfaceOpFail=GL_KEEP; \
            _CurrentContext->StencilBackfaceOpZFail=GL_KEEP; \
            _CurrentContext->StencilBackfaceOpZPass=GL_KEEP;, \
            { \
                int *int_values=static_cast<int *>(value.getArrayData()); \
                return _jsIsFaceMode((GLenum)int_values[0]) && \
                    _jsIsStencilOp((GLenum)int_values[1])&& \
                    _jsIsStencilOp((GLenum)int_values[2])&& \
                    _jsIsStencilOp((GLenum)int_values[3]); \
            } \
        ) \
    GENERIC_STATE(Texture2D,SAMPLER2D,1,0, \
            _jsBindTextureInternal(_CurrentContext->TextureImageUnits+sa->getArrayIndex(),*(GLuint*)(value.getSampler()->pushBufferPointer),GL_TEXTURE_2D);, \
            _jsBindTextureInternal(_CurrentContext->TextureImageUnits+sa->getArrayIndex(),0,GL_TEXTURE_2D);, \
            return (sa->getArrayIndex()<_JS_MAX_TEXTURE_IMAGE_UNITS); \
        ) \
    GENERIC_STATE(Texture3D,SAMPLER3D,1,0, \
            _jsBindTextureInternal(_CurrentContext->TextureImageUnits+sa->getArrayIndex(),*(GLuint*)(value.getSampler()->pushBufferPointer),GL_TEXTURE_3D);, \
            _jsBindTextureInternal(_CurrentContext->TextureImageUnits+sa->getArrayIndex(),0,GL_TEXTURE_3D);, \
            return (sa->getArrayIndex()<_JS_MAX_TEXTURE_IMAGE_UNITS); \
        ) \
    GENERIC_STATE(TextureCubeMap,SAMPLERCUBE,1,0, \
            _jsBindTextureInternal(_CurrentContext->TextureImageUnits+sa->getArrayIndex(),*(GLuint*)(value.getSampler()->pushBufferPointer),GL_TEXTURE_CUBE_MAP);, \
            _jsBindTextureInternal(_CurrentContext->TextureImageUnits+sa->getArrayIndex(),0,GL_TEXTURE_CUBE_MAP);, \
            return (sa->getArrayIndex()<_JS_MAX_TEXTURE_IMAGE_UNITS); \
        ) \
    GENERIC_STATE(TextureEnvMode,INT,1,PSGL_VALIDATE_FFX_FRAGMENT_PROGRAM, \
            _CurrentContext->TextureImageUnits[sa->getArrayIndex()].envMode=(GLenum)value.getInt();, \
            _CurrentContext->TextureImageUnits[sa->getArrayIndex()].envMode=GL_MODULATE;, \
            return (sa->getArrayIndex()<_JS_MAX_TEXTURE_IMAGE_UNITS)&&_jsIsTexEnvMode((GLenum)value.getInt()); \
        ) \
    GENERIC_STATE(TextureEnvColor,FLOAT,4,PSGL_VALIDATE_FFX_FRAGMENT_PROGRAM, \
            setColor(_CurrentContext->TextureImageUnits[sa->getArrayIndex()].envColor, value);, \
            resetColor(_CurrentContext->TextureImageUnits[sa->getArrayIndex()].envColor);, \
            return (sa->getArrayIndex()<_JS_MAX_TEXTURE_IMAGE_UNITS); \
        ) \
    GENERIC_STATE(TexGenSMode,INT,1,PSGL_VALIDATE_FFX_VERTEX_PROGRAM, \
            _CurrentContext->TextureCoordsUnits[sa->getArrayIndex()].texGen[0].mode=(GLenum)value.getInt();, \
            _CurrentContext->TextureCoordsUnits[sa->getArrayIndex()].texGen[0].mode=GL_EYE_LINEAR;, \
            return (sa->getArrayIndex()<_JS_MAX_TEXTURE_COORDS)&&_jsIsTexGenMode((GLenum)value.getInt(),GL_S); \
        ) \
    GENERIC_STATE(TexGenTMode,INT,1,PSGL_VALIDATE_FFX_VERTEX_PROGRAM, \
            _CurrentContext->TextureCoordsUnits[sa->getArrayIndex()].texGen[1].mode=(GLenum)value.getInt();, \
            _CurrentContext->TextureCoordsUnits[sa->getArrayIndex()].texGen[1].mode=GL_EYE_LINEAR;, \
            return (sa->getArrayIndex()<_JS_MAX_TEXTURE_COORDS)&&_jsIsTexGenMode((GLenum)value.getInt(),GL_T); \
        ) \
    GENERIC_STATE(TexGenRMode,INT,1,PSGL_VALIDATE_FFX_VERTEX_PROGRAM, \
            _CurrentContext->TextureCoordsUnits[sa->getArrayIndex()].texGen[2].mode=(GLenum)value.getInt();, \
            _CurrentContext->TextureCoordsUnits[sa->getArrayIndex()].texGen[2].mode=GL_EYE_LINEAR;, \
            return (sa->getArrayIndex()<_JS_MAX_TEXTURE_COORDS)&&_jsIsTexGenMode((GLenum)value.getInt(),GL_R); \
        ) \
    GENERIC_STATE(TexGenQMode,INT,1,PSGL_VALIDATE_FFX_VERTEX_PROGRAM, \
            _CurrentContext->TextureCoordsUnits[sa->getArrayIndex()].texGen[3].mode=(GLenum)value.getInt();, \
            _CurrentContext->TextureCoordsUnits[sa->getArrayIndex()].texGen[3].mode=GL_EYE_LINEAR;, \
            return (sa->getArrayIndex()<_JS_MAX_TEXTURE_COORDS)&&_jsIsTexGenMode((GLenum)value.getInt(),GL_Q); \
        ) \
    GENERIC_STATE(TexGenSEyePlane,FLOAT,4,PSGL_VALIDATE_FFX_VERTEX_PROGRAM, \
            _jsSetTexGenEyePlane(_CurrentContext->TextureCoordsUnits[sa->getArrayIndex()].texGen+0, static_cast<const float *>(value.getArrayData()));\
            _CurrentContext->TextureCoordsUnits[sa->getArrayIndex()].revalidate |= _JS_TEXTURE_UNIT_REVALIDATE_TEX_GEN;, \
            { \
                float *c=_CurrentContext->TextureCoordsUnits[sa->getArrayIndex()].texGen[0].eyePlane; \
                c[0]=1.f; \
                c[1]=c[2]=c[3]=0.f; \
                _CurrentContext->TextureCoordsUnits[sa->getArrayIndex()].revalidate |= _JS_TEXTURE_UNIT_REVALIDATE_TEX_GEN; \
            }, \
            return (sa->getArrayIndex()<_JS_MAX_TEXTURE_COORDS); \
        ) \
    GENERIC_STATE(TexGenTEyePlane,FLOAT,4,PSGL_VALIDATE_FFX_VERTEX_PROGRAM, \
            _jsSetTexGenEyePlane(_CurrentContext->TextureCoordsUnits[sa->getArrayIndex()].texGen+1, static_cast<const float *>(value.getArrayData()));\
            _CurrentContext->TextureCoordsUnits[sa->getArrayIndex()].revalidate |= _JS_TEXTURE_UNIT_REVALIDATE_TEX_GEN;, \
            { \
                float *c=_CurrentContext->TextureCoordsUnits[sa->getArrayIndex()].texGen[1].eyePlane; \
                c[1]=1.f; \
                c[0]=c[2]=c[3]=0.f; \
                _CurrentContext->TextureCoordsUnits[sa->getArrayIndex()].revalidate |= _JS_TEXTURE_UNIT_REVALIDATE_TEX_GEN; \
            }, \
            return (sa->getArrayIndex()<_JS_MAX_TEXTURE_COORDS); \
        ) \
    GENERIC_STATE(TexGenREyePlane,FLOAT,4,PSGL_VALIDATE_FFX_VERTEX_PROGRAM, \
            _jsSetTexGenEyePlane(_CurrentContext->TextureCoordsUnits[sa->getArrayIndex()].texGen+2, static_cast<const float *>(value.getArrayData()));\
            _CurrentContext->TextureCoordsUnits[sa->getArrayIndex()].revalidate |= _JS_TEXTURE_UNIT_REVALIDATE_TEX_GEN;, \
            { \
                float *c=_CurrentContext->TextureCoordsUnits[sa->getArrayIndex()].texGen[2].eyePlane; \
                c[0]=c[1]=c[2]=c[3]=0.f; \
                _CurrentContext->TextureCoordsUnits[sa->getArrayIndex()].revalidate |= _JS_TEXTURE_UNIT_REVALIDATE_TEX_GEN; \
            }, \
            return (sa->getArrayIndex()<_JS_MAX_TEXTURE_COORDS); \
        ) \
    GENERIC_STATE(TexGenQEyePlane,FLOAT,4,PSGL_VALIDATE_FFX_VERTEX_PROGRAM, \
            _jsSetTexGenEyePlane(_CurrentContext->TextureCoordsUnits[sa->getArrayIndex()].texGen+3, static_cast<const float *>(value.getArrayData()));\
            _CurrentContext->TextureCoordsUnits[sa->getArrayIndex()].revalidate |= _JS_TEXTURE_UNIT_REVALIDATE_TEX_GEN;, \
            { \
                float *c=_CurrentContext->TextureCoordsUnits[sa->getArrayIndex()].texGen[3].eyePlane; \
                c[0]=c[1]=c[2]=c[3]=0.f; \
                _CurrentContext->TextureCoordsUnits[sa->getArrayIndex()].revalidate |= _JS_TEXTURE_UNIT_REVALIDATE_TEX_GEN; \
            }, \
            return (sa->getArrayIndex()<_JS_MAX_TEXTURE_COORDS); \
        ) \
    GENERIC_STATE(TexGenSObjectPlane,FLOAT,4,PSGL_VALIDATE_FFX_VERTEX_PROGRAM, \
            setFloat4(_CurrentContext->TextureCoordsUnits[sa->getArrayIndex()].texGen[0].objectPlane, value);\
            _CurrentContext->TextureCoordsUnits[sa->getArrayIndex()].revalidate |= _JS_TEXTURE_UNIT_REVALIDATE_TEX_GEN;, \
            { \
                float *c=_CurrentContext->TextureCoordsUnits[sa->getArrayIndex()].texGen[0].objectPlane; \
                c[0]=1.f; \
                c[1]=c[2]=c[3]=0.f; \
                _CurrentContext->TextureCoordsUnits[sa->getArrayIndex()].revalidate |= _JS_TEXTURE_UNIT_REVALIDATE_TEX_GEN; \
            }, \
            return (sa->getArrayIndex()<_JS_MAX_TEXTURE_COORDS); \
        ) \
    GENERIC_STATE(TexGenTObjectPlane,FLOAT,4,PSGL_VALIDATE_FFX_VERTEX_PROGRAM, \
            setFloat4(_CurrentContext->TextureCoordsUnits[sa->getArrayIndex()].texGen[1].objectPlane, value);\
            _CurrentContext->TextureCoordsUnits[sa->getArrayIndex()].revalidate |= _JS_TEXTURE_UNIT_REVALIDATE_TEX_GEN;, \
            { \
                float *c=_CurrentContext->TextureCoordsUnits[sa->getArrayIndex()].texGen[1].objectPlane; \
                c[1]=1.f; \
                c[0]=c[2]=c[3]=0.f; \
                _CurrentContext->TextureCoordsUnits[sa->getArrayIndex()].revalidate |= _JS_TEXTURE_UNIT_REVALIDATE_TEX_GEN; \
            }, \
            return (sa->getArrayIndex()<_JS_MAX_TEXTURE_COORDS); \
        ) \
    GENERIC_STATE(TexGenRObjectPlane,FLOAT,4,PSGL_VALIDATE_FFX_VERTEX_PROGRAM, \
            setFloat4(_CurrentContext->TextureCoordsUnits[sa->getArrayIndex()].texGen[2].objectPlane, value);\
            _CurrentContext->TextureCoordsUnits[sa->getArrayIndex()].revalidate |= _JS_TEXTURE_UNIT_REVALIDATE_TEX_GEN;, \
            { \
                float *c=_CurrentContext->TextureCoordsUnits[sa->getArrayIndex()].texGen[2].objectPlane; \
                c[0]=c[1]=c[2]=c[3]=0.f; \
                _CurrentContext->TextureCoordsUnits[sa->getArrayIndex()].revalidate |= _JS_TEXTURE_UNIT_REVALIDATE_TEX_GEN; \
            }, \
            return (sa->getArrayIndex()<_JS_MAX_TEXTURE_COORDS); \
        ) \
    GENERIC_STATE(TexGenQObjectPlane,FLOAT,4,PSGL_VALIDATE_FFX_VERTEX_PROGRAM, \
            setFloat4(_CurrentContext->TextureCoordsUnits[sa->getArrayIndex()].texGen[3].objectPlane, value);\
            _CurrentContext->TextureCoordsUnits[sa->getArrayIndex()].revalidate |= _JS_TEXTURE_UNIT_REVALIDATE_TEX_GEN;, \
            { \
                float *c=_CurrentContext->TextureCoordsUnits[sa->getArrayIndex()].texGen[3].objectPlane; \
                c[0]=c[1]=c[2]=c[3]=0.f; \
                _CurrentContext->TextureCoordsUnits[sa->getArrayIndex()].revalidate |= _JS_TEXTURE_UNIT_REVALIDATE_TEX_GEN; \
            }, \
            return (sa->getArrayIndex()<_JS_MAX_TEXTURE_COORDS); \
        ) \
    GENERIC_STATE(VertexProgram,PROGRAM,1,PSGL_VALIDATE_VERTEX_PROGRAM | PSGL_VALIDATE_VERTEX_TEXTURES_USED, \
            _cgGLBindVertexProgram(value.getProgram()); _CurrentContext->VertexProgram=GL_TRUE;, \
            _cgGLUnbindVertexProgram(); _CurrentContext->VertexProgram=GL_FALSE;, \
            { \
                _CGprogram *prog=value.getProgram(); \
                if (prog) \
                { \
                    CGprofile profile=(CGprofile)prog->header.profile; \
                    return ((profile==CG_PROFILE_SCE_VP_TYPEB)||(profile==CG_PROFILE_SCE_VP_RSX)); \
                } \
                else return false; \
            } \
        ) \

// these are not called in the main loop. we can just call GL directly.
#define DEFINE_GENERIC_SAMPLER_STATES \
    GENERIC_STATE(WrapS,INT,1,0, \
            glTexParameteri(sa->getParentParameter()->glType,GL_TEXTURE_WRAP_S,value.getInt());, \
            glTexParameteri(sa->getParentParameter()->glType,GL_TEXTURE_WRAP_S,GL_REPEAT);, \
            return _jsIsWrapMode(value.getInt()); \
        ) \
    GENERIC_STATE(WrapT,INT,1,0, \
            glTexParameteri(sa->getParentParameter()->glType,GL_TEXTURE_WRAP_T,value.getInt());, \
            glTexParameteri(sa->getParentParameter()->glType,GL_TEXTURE_WRAP_T,GL_REPEAT);, \
            return _jsIsWrapMode(value.getInt()); \
        ) \
    GENERIC_STATE(WrapR,INT,1,0, \
            glTexParameteri(sa->getParentParameter()->glType,GL_TEXTURE_WRAP_R,value.getInt());, \
            glTexParameteri(sa->getParentParameter()->glType,GL_TEXTURE_WRAP_R,GL_REPEAT);, \
            return _jsIsWrapMode(value.getInt()); \
        ) \
    GENERIC_STATE(CompareMode,INT,1,0, \
            glTexParameteri(sa->getParentParameter()->glType,GL_TEXTURE_COMPARE_MODE_ARB,value.getInt());, \
            glTexParameteri(sa->getParentParameter()->glType,GL_TEXTURE_COMPARE_MODE_ARB,GL_NONE);, \
            return _jsIsCompareMode(value.getInt()); \
        ) \
    GENERIC_STATE(CompareFunc,INT,1,0, \
            glTexParameteri(sa->getParentParameter()->glType,GL_TEXTURE_COMPARE_FUNC_ARB,value.getInt());, \
            glTexParameteri(sa->getParentParameter()->glType,GL_TEXTURE_COMPARE_FUNC_ARB,GL_LEQUAL);, \
            return _jsIsTestFunc(value.getInt()); \
        ) \
    GENERIC_STATE(DepthMode,INT,1,0, \
            glTexParameteri(sa->getParentParameter()->glType,GL_DEPTH_TEXTURE_MODE_ARB,value.getInt());, \
            glTexParameteri(sa->getParentParameter()->glType,GL_DEPTH_TEXTURE_MODE_ARB,GL_LUMINANCE);, \
            return _jsIsDepthTextureMode(value.getInt()); \
        ) \
    GENERIC_STATE(LodBias,FLOAT,1,0, \
            glTexParameterf(sa->getParentParameter()->glType,GL_TEXTURE_LOD_BIAS,value.getFloat());, \
            glTexParameterf(sa->getParentParameter()->glType,GL_TEXTURE_LOD_BIAS,0.f);, \
            return true; \
        ) \
    GENERIC_STATE(MinFilter,INT,1,0, \
            glTexParameteri(sa->getParentParameter()->glType,GL_TEXTURE_MIN_FILTER,value.getInt());, \
            glTexParameteri(sa->getParentParameter()->glType,GL_TEXTURE_MIN_FILTER,GL_NEAREST_MIPMAP_LINEAR);, \
            return _jsIsMinFilter(value.getInt()); \
        ) \
    GENERIC_STATE(MagFilter,INT,1,0, \
            glTexParameteri(sa->getParentParameter()->glType,GL_TEXTURE_MAG_FILTER,value.getInt());, \
            glTexParameteri(sa->getParentParameter()->glType,GL_TEXTURE_MAG_FILTER,GL_LINEAR);, \
            return _jsIsMagFilter(value.getInt()); \
        ) \
    GENERIC_STATE(MaxAnisotropy,FLOAT,1,0, \
            glTexParameterf(sa->getParentParameter()->glType,GL_TEXTURE_MAX_ANISOTROPY_EXT,value.getFloat());, \
            glTexParameterf(sa->getParentParameter()->glType,GL_TEXTURE_MAX_ANISOTROPY_EXT,1.f);, \
            return (value.getFloat()>=1.f); \
        ) \


#define TEST_FUNC_STATE_ENUM_MAP(STATE) \
    STATE_ENUM_MAP(STATE,GL_NEVER,"Never") \
    STATE_ENUM_MAP(STATE,GL_LESS,"Less") \
    STATE_ENUM_MAP(STATE,GL_LEQUAL,"LEqual") \
    STATE_ENUM_MAP(STATE,GL_EQUAL,"Equal") \
    STATE_ENUM_MAP(STATE,GL_GREATER,"Greater") \
    STATE_ENUM_MAP(STATE,GL_NOTEQUAL,"NotEqual") \
    STATE_ENUM_MAP(STATE,GL_GEQUAL,"GEqual") \
    STATE_ENUM_MAP(STATE,GL_ALWAYS,"Always")

#define BLEND_FUNC_STATE_ENUM_MAP(STATE) \
    STATE_ENUM_MAP(STATE,GL_ZERO,"Zero") \
    STATE_ENUM_MAP(STATE,GL_ONE,"One") \
    STATE_ENUM_MAP(STATE,GL_DST_COLOR,"DestColor") \
    STATE_ENUM_MAP(STATE,GL_DST_COLOR,"DstColor") \
    STATE_ENUM_MAP(STATE,GL_ONE_MINUS_DST_COLOR,"OneMinusDestColor") \
    STATE_ENUM_MAP(STATE,GL_ONE_MINUS_DST_COLOR,"OneMinusDstColor") \
    STATE_ENUM_MAP(STATE,GL_SRC_ALPHA,"SrcAlpha") \
    STATE_ENUM_MAP(STATE,GL_ONE_MINUS_SRC_ALPHA,"OneMinusSrcAlpha") \
    STATE_ENUM_MAP(STATE,GL_DST_ALPHA,"DstAlpha") \
    STATE_ENUM_MAP(STATE,GL_ONE_MINUS_DST_ALPHA,"OneMinusDstAlpha") \
    STATE_ENUM_MAP(STATE,GL_SRC_ALPHA_SATURATE,"SrcAlphaSaturate") \
    STATE_ENUM_MAP(STATE,GL_SRC_COLOR,"SrcColor") \
    STATE_ENUM_MAP(STATE,GL_ONE_MINUS_SRC_COLOR,"OneMinusSrcColor") \
    STATE_ENUM_MAP(STATE,GL_CONSTANT_COLOR,"ConstantColor") \
    STATE_ENUM_MAP(STATE,GL_ONE_MINUS_CONSTANT_COLOR,"OneMinusConstantColor") \
    STATE_ENUM_MAP(STATE,GL_CONSTANT_ALPHA,"ConstantAlpha") \
    STATE_ENUM_MAP(STATE,GL_ONE_MINUS_CONSTANT_ALPHA,"OneMinusConstantAlpha") \

#define BLEND_EQUATION_STATE_ENUM_MAP(STATE) \
    STATE_ENUM_MAP(STATE,GL_FUNC_ADD,"FuncAdd") \
    STATE_ENUM_MAP(STATE,GL_FUNC_SUBTRACT,"FuncSubtract") \
    STATE_ENUM_MAP(STATE,GL_FUNC_REVERSE_SUBTRACT,"FuncReverseSubtract") \
    STATE_ENUM_MAP(STATE,GL_MIN,"Min") \
    STATE_ENUM_MAP(STATE,GL_MAX,"Max") \

#define FACE_STATE_ENUM_MAP(STATE) \
    STATE_ENUM_MAP(STATE,GL_FRONT,"Front") \
    STATE_ENUM_MAP(STATE,GL_BACK,"Back") \
    STATE_ENUM_MAP(STATE,GL_FRONT_AND_BACK,"FrontAndBack") \

#define WRAP_STATE_ENUM_MAP(STATE) \
    STATE_ENUM_MAP(STATE,GL_REPEAT,"Repeat") \
    STATE_ENUM_MAP(STATE,GL_CLAMP,"Clamp") \
    STATE_ENUM_MAP(STATE,GL_CLAMP_TO_EDGE,"ClampToEdge") \

#define DEFINE_STATE_ENUM_MAPS \
    TEST_FUNC_STATE_ENUM_MAP(AlphaFunc) \
    \
    BLEND_FUNC_STATE_ENUM_MAP(BlendFunc) \
    BLEND_FUNC_STATE_ENUM_MAP(BlendFuncSeparate) \
    \
    BLEND_EQUATION_STATE_ENUM_MAP(BlendEquation) \
    BLEND_EQUATION_STATE_ENUM_MAP(BlendEquationSeparate) \
    \
    FACE_STATE_ENUM_MAP(CullFace) \
    \
    TEST_FUNC_STATE_ENUM_MAP(DepthFunc) \
    \
    STATE_ENUM_MAP(FogMode,GL_LINEAR,"Linear") \
    STATE_ENUM_MAP(FogMode,GL_EXP,"Exp") \
    STATE_ENUM_MAP(FogMode,GL_EXP2,"Exp2") \
    \
    STATE_ENUM_MAP(FrontFace,GL_CW,"CW") \
    STATE_ENUM_MAP(FrontFace,GL_CCW,"CCW") \
    \
    STATE_ENUM_MAP(LogicOp,GL_CLEAR,"Clear") \
    STATE_ENUM_MAP(LogicOp,GL_AND,"And") \
    STATE_ENUM_MAP(LogicOp,GL_AND_REVERSE,"AndReverse") \
    STATE_ENUM_MAP(LogicOp,GL_COPY,"Copy") \
    STATE_ENUM_MAP(LogicOp,GL_AND_INVERTED,"AndInverted") \
    STATE_ENUM_MAP(LogicOp,GL_NOOP,"Noop") \
    STATE_ENUM_MAP(LogicOp,GL_XOR,"Xor") \
    STATE_ENUM_MAP(LogicOp,GL_OR,"Or") \
    STATE_ENUM_MAP(LogicOp,GL_NOR,"Nor") \
    STATE_ENUM_MAP(LogicOp,GL_EQUIV,"Equiv") \
    STATE_ENUM_MAP(LogicOp,GL_INVERT,"Invert") \
    STATE_ENUM_MAP(LogicOp,GL_OR_REVERSE,"OrReverse") \
    STATE_ENUM_MAP(LogicOp,GL_OR_INVERTED,"OrInverted") \
    STATE_ENUM_MAP(LogicOp,GL_COPY_INVERTED,"CopyInverted") \
    STATE_ENUM_MAP(LogicOp,GL_NAND,"Nand") \
    STATE_ENUM_MAP(LogicOp,GL_SET,"Set") \
    \
    FACE_STATE_ENUM_MAP(PolygonMode) \
    STATE_ENUM_MAP(PolygonMode,GL_POINT,"Point") \
    STATE_ENUM_MAP(PolygonMode,GL_LINE,"Line") \
    STATE_ENUM_MAP(PolygonMode,GL_FILL,"Fill") \
    \
    STATE_ENUM_MAP(ShadeModel,GL_FLAT,"Flat") \
    STATE_ENUM_MAP(ShadeModel,GL_SMOOTH,"Smooth") \
    \
    TEST_FUNC_STATE_ENUM_MAP(StencilFunc) \
    \
    STATE_ENUM_MAP(StencilOp,GL_KEEP,"Keep") \
    STATE_ENUM_MAP(StencilOp,GL_ZERO,"Zero") \
    STATE_ENUM_MAP(StencilOp,GL_REPLACE,"Replace") \
    STATE_ENUM_MAP(StencilOp,GL_INCR,"Incr") \
    STATE_ENUM_MAP(StencilOp,GL_DECR,"Decr") \
    STATE_ENUM_MAP(StencilOp,GL_INVERT,"Invert") \
    STATE_ENUM_MAP(StencilOp,GL_INCR_WRAP,"IncrWrap") \
    STATE_ENUM_MAP(StencilOp,GL_DECR_WRAP,"DecrWrap") \
    \
    STATE_ENUM_MAP(TexGenSMode,GL_OBJECT_LINEAR,"ObjectLinear") \
    STATE_ENUM_MAP(TexGenSMode,GL_EYE_LINEAR,"EyeLinear") \
    STATE_ENUM_MAP(TexGenSMode,GL_SPHERE_MAP,"SphereMap") \
    STATE_ENUM_MAP(TexGenSMode,GL_REFLECTION_MAP,"ReflectionMap") \
    STATE_ENUM_MAP(TexGenSMode,GL_NORMAL_MAP,"NormalMap") \
    \
    STATE_ENUM_MAP(TexGenTMode,GL_OBJECT_LINEAR,"ObjectLinear") \
    STATE_ENUM_MAP(TexGenTMode,GL_EYE_LINEAR,"EyeLinear") \
    STATE_ENUM_MAP(TexGenTMode,GL_SPHERE_MAP,"SphereMap") \
    STATE_ENUM_MAP(TexGenTMode,GL_REFLECTION_MAP,"ReflectionMap") \
    STATE_ENUM_MAP(TexGenTMode,GL_NORMAL_MAP,"NormalMap") \
    \
    STATE_ENUM_MAP(TexGenRMode,GL_OBJECT_LINEAR,"ObjectLinear") \
    STATE_ENUM_MAP(TexGenRMode,GL_EYE_LINEAR,"EyeLinear") \
    STATE_ENUM_MAP(TexGenRMode,GL_REFLECTION_MAP,"ReflectionMap") \
    STATE_ENUM_MAP(TexGenRMode,GL_NORMAL_MAP,"NormalMap") \
    \
    STATE_ENUM_MAP(TexGenQMode,GL_OBJECT_LINEAR,"ObjectLinear") \
    STATE_ENUM_MAP(TexGenQMode,GL_EYE_LINEAR,"EyeLinear") \
    \
    STATE_ENUM_MAP(TextureEnvMode,GL_MODULATE,"Modulate") \
    STATE_ENUM_MAP(TextureEnvMode,GL_DECAL,"Decal") \
    STATE_ENUM_MAP(TextureEnvMode,GL_BLEND,"Blend") \
    STATE_ENUM_MAP(TextureEnvMode,GL_REPLACE,"Replace") \
    STATE_ENUM_MAP(TextureEnvMode,GL_ADD,"Add") \


#define DEFINE_SAMPLER_STATE_ENUM_MAPS \
    WRAP_STATE_ENUM_MAP(WrapS) \
    WRAP_STATE_ENUM_MAP(WrapT) \
    WRAP_STATE_ENUM_MAP(WrapR) \
    \
    STATE_ENUM_MAP(CompareMode,GL_NONE,"None") \
    STATE_ENUM_MAP(CompareMode,GL_COMPARE_R_TO_TEXTURE_ARB,"CompareRToTexture") \
    \
    TEST_FUNC_STATE_ENUM_MAP(CompareFunc) \
    \
    STATE_ENUM_MAP(MinFilter,GL_NEAREST,"Nearest") \
    STATE_ENUM_MAP(MinFilter,GL_LINEAR,"Linear") \
    STATE_ENUM_MAP(MinFilter,GL_NEAREST_MIPMAP_NEAREST,"NearestMipMapNearest") \
    STATE_ENUM_MAP(MinFilter,GL_LINEAR_MIPMAP_NEAREST,"LinearMipMapNearest") \
    STATE_ENUM_MAP(MinFilter,GL_NEAREST_MIPMAP_LINEAR,"NearestMipMapLinear") \
    STATE_ENUM_MAP(MinFilter,GL_LINEAR_MIPMAP_LINEAR,"LinearMipMapLinear") \
    \
    STATE_ENUM_MAP(MagFilter,GL_NEAREST,"Nearest") \
    STATE_ENUM_MAP(MagFilter,GL_LINEAR,"Linear") \


namespace PSGLFX
{

    static void validateColor( AnyValue &value )
    {
        float *float_values = static_cast<float *>( value.getArrayData() );
        float_values[0] = _jsClampf( float_values[0] );
        float_values[1] = _jsClampf( float_values[1] );
        float_values[2] = _jsClampf( float_values[2] );
        float_values[3] = _jsClampf( float_values[3] );
    }

    static inline void setColor( jsColorRGBAf &color, const AnyValue &value )
    {
        const float *float_values = static_cast<const float *>( value.getArrayData() );
        color.R = float_values[0];
        color.G = float_values[1];
        color.B = float_values[2];
        color.A = float_values[3];
    }

    static inline void resetColor( jsColorRGBAf &color )
    {
        color.R = color.G = color.B = color.A = 0.f;
    }

    static inline void resetColor1( jsColorRGBAf &color )
    {
        color.R = color.G = color.B = 0.f;
        color.A = 1.f;
    }

    static inline void setFloat4( float *c, const AnyValue &value )
    {
        const float *float_values = static_cast<const float *>( value.getArrayData() );
        c[0] = float_values[0];
        c[1] = float_values[1];
        c[2] = float_values[2];
        c[3] = float_values[3];
    }

    class SimpleBooleanState: public State
    {
        GLboolean PSGLcontext::*field;

        GLuint revalidateFlag;

        static bool simpleBooleanSetFunction( StateAssignment *sa )
        {
            JS_ASSERT( sa->getValue().getType() == AnyValue::BOOL );
            SimpleBooleanState *state = static_cast<SimpleBooleanState*>( sa->getState() );
            JS_ASSERT( state );
            _CurrentContext->*( state->field ) = sa->getValue().getBool();
            _CurrentContext->needValidate |= state->revalidateFlag;
            return true;
        }

        static bool simpleBooleanResetFunction( StateAssignment *sa )
        {
            JS_ASSERT( sa->getValue().getType() == AnyValue::BOOL );
            SimpleBooleanState *state = static_cast<SimpleBooleanState*>( sa->getState() );
            JS_ASSERT( state );
            _CurrentContext->*( state->field ) = GL_FALSE;
            _CurrentContext->needValidate |= state->revalidateFlag;
            return true;
        }

    public:
        SimpleBooleanState( _CGcontext *parent, const char *n, GLboolean PSGLcontext::*f, GLuint revalidate ):
                State( parent, n, AnyValue::BOOL, 1 ),
                field( f ),
                revalidateFlag( revalidate )
        {
            setFunction = &SimpleBooleanState::simpleBooleanSetFunction;
            resetFunction = &SimpleBooleanState::simpleBooleanResetFunction;
        }
        ~SimpleBooleanState() {}};

// The linker that comes with gcc4 seems to have issues with references to static functions in some cases.
// These are scoped anyway, so it isn't a big deal to have them global.
//
//#define STATIC static
#define STATIC

#define INDEXED_BOOLEAN_STATE(NAME,MAX,CONTEXT_FIELD,SUB_FIELD,VALIDATE,EXTRA_VALIDATION) \
STATIC bool NAME##ValidateFunction(StateAssignment *sa) \
{ \
    JS_ASSERT(sa->getValue().getType()==AnyValue::BOOL); \
    return sa->getArrayIndex()<MAX; \
} \
 \
STATIC bool NAME##SetFunction(StateAssignment *sa) \
{ \
    JS_ASSERT(sa->getValue().getType()==AnyValue::BOOL); \
    int index=sa->getArrayIndex(); \
    JS_ASSERT(index<MAX); \
    _CurrentContext->CONTEXT_FIELD[index].SUB_FIELD=sa->getValue().getBool(); \
    _CurrentContext->needValidate|=VALIDATE; \
    EXTRA_VALIDATION \
    return true; \
} \
 \
STATIC bool NAME##ResetFunction(StateAssignment *sa) \
{ \
    JS_ASSERT(sa->getValue().getType()==AnyValue::BOOL); \
    int index=sa->getArrayIndex(); \
    JS_ASSERT(index<MAX); \
    _CurrentContext->CONTEXT_FIELD[index].SUB_FIELD=sa->getValue().getBool(); \
    _CurrentContext->needValidate|=VALIDATE; \
    EXTRA_VALIDATION \
    return true; \
}
    DEFINE_INDEXED_BOOLEAN_STATES
#undef INDEXED_BOOLEAN_STATE

#define GENERIC_STATE(NAME,TYPE,N,FLAG,SET,RESET,VALIDATE) \
STATIC bool NAME##ValidateFunction(StateAssignment *sa) \
{ \
    AnyValue &value=sa->getValue();\
    (void)value; \
    JS_ASSERT(value.getType()==AnyValue::TYPE); \
    JS_ASSERT(value.getElemCount()==N); \
    VALIDATE \
} \
 \
STATIC bool NAME##SetFunction(StateAssignment *sa) \
{ \
    AnyValue &value=sa->getValue();\
    (void)value; \
    JS_ASSERT(value.getType()==AnyValue::TYPE); \
    JS_ASSERT(value.getElemCount()==N); \
    SET \
    _CurrentContext->needValidate|=FLAG; \
    return true; \
} \
 \
STATIC bool NAME##ResetFunction(StateAssignment *sa) \
{ \
    AnyValue &value=sa->getValue();\
    (void)value; \
    JS_ASSERT(value.getType()==AnyValue::TYPE); \
    JS_ASSERT(value.getElemCount()==N); \
    RESET \
    _CurrentContext->needValidate|=FLAG; \
    return true; \
}
    DEFINE_GENERIC_STATES
    DEFINE_GENERIC_SAMPLER_STATES
#undef GENERIC_STATE


    void registerGLStates( _CGcontext *context )
    {
#define SIMPLE_BOOLEAN_STATE(NAME,FIELD,VALIDATE) \
        { \
            State *state=new SimpleBooleanState(context,#NAME,&PSGLcontext::FIELD,VALIDATE); \
            CgContextExtension::fromContext(context)->appendState(state); \
        }
        DEFINE_SIMPLE_BOOLEAN_STATES
#undef SIMPLE_BOOLEAN_STATE
#define INDEXED_BOOLEAN_STATE(NAME,MAX,CONTEXT_FIELD,SUB_FIELD,VALIDATE,EXTRA_VALIDATION) \
        { \
            State *state=new State(context,#NAME,AnyValue::BOOL); \
            state->setUserFunctions(NAME##SetFunction,NAME##ResetFunction,NAME##ValidateFunction); \
            CgContextExtension::fromContext(context)->appendState(state); \
        }
        DEFINE_INDEXED_BOOLEAN_STATES
#undef INDEXED_BOOLEAN_STATE
#define GENERIC_STATE(NAME,TYPE,N,FLAG,SET,RESET,VALIDATE) \
        { \
            State *state=new State(context,#NAME,AnyValue::TYPE,N); \
            state->setUserFunctions(NAME##SetFunction,NAME##ResetFunction,NAME##ValidateFunction); \
            CgContextExtension::fromContext(context)->appendState(state); \
        }
        DEFINE_GENERIC_STATES
#undef GENERIC_STATE

#define GENERIC_STATE(NAME,TYPE,N,FLAG,SET,RESET,VALIDATE) \
        { \
            State *state=new State(context,#NAME,AnyValue::TYPE,N); \
            state->setUserFunctions(NAME##SetFunction,NAME##ResetFunction,NAME##ValidateFunction); \
            CgContextExtension::fromContext(context)->appendSamplerState(state); \
        }
        DEFINE_GENERIC_SAMPLER_STATES
#undef GENERIC_STATE

#define STATE_ENUM_MAP(STATE,ENUM,NAME) \
        { \
            CGstate state=cgGetNamedState(context->id,#STATE); \
            JS_ASSERT(state); \
            cgAddStateEnumerant(state, NAME, ENUM); \
        }
        DEFINE_STATE_ENUM_MAPS
#undef STATE_ENUM_MAP

#define STATE_ENUM_MAP(STATE,ENUM,NAME) \
        { \
            CGstate state=cgGetNamedSamplerState(context->id,#STATE); \
            JS_ASSERT(state); \
            cgAddStateEnumerant(state, NAME, ENUM); \
        }
        DEFINE_SAMPLER_STATE_ENUM_MAPS
#undef STATE_ENUM_MAP
    }
}

// API functions ----------------------------------------

/** @addtogroup PSGLFX
 *@{
 */
/**
@short Creates all the GL states in a context

cgGLRegisterStates() creates a Cg representation of all the GL states, and populates
enumerant map.

The following table lists the names of the states supported by the state manager, their types, and valid enumerants.


<TABLE rules=all>
<TR>
<TD align="center"><B>State Name</B></TH>
<TD align="center"><B>Type</B></TH>
<TD align="center"><B>Valid Enumerants</B></TH>
</TR>
<TR>
<TD><CODE>AlphaFunc</CODE></TD>
<TD><CODE>float2(enum, reference_value)</CODE></TD>
<TD><CODE>Never, Less, LEqual, Equal, Greater, NotEqual, GEqual, Always</CODE></TD>
</TR>
<TR>
<TD><CODE>BlendFunc</CODE></TD>
<TD><CODE>int2(src_factor, det_factor)</CODE></TD>
<TD><CODE>Zero, One, DestColor, OneMinusDestColor, SrcAlpha, OneMinusSrcAlpha, DetAlpha,
OneMinusDetAlpha, SrcAlphaSaturate, SrcColor, OneMinusSrcColor, ConstantColor, OneMinusConstantColor, ConstantAlpha, OneMinusConstantAlpha</CODE></TD>
</TR>
<TR>
<TD><CODE>BlendFuncSeparate</CODE></TD>
<TD><CODE>int4(rgb_src, rgb_dst, a_src, a_dst)</CODE></TD>
<TD><CODE>Zero, One, DestColor, OneMinusDestColor, SrcAlpha, OneMinusSrcAlpha, DetAlpha,
OneMinusDetAlpha, SrcAlphaSaturate, SrcColor, OneMinusSrcColor, ConstantColor, OneMinusConstantColor, ConstantAlpha, OneMinusConstantAlpha</CODE></TD>
</TR>
<TR>
<TD><CODE>BlendEquation</CODE></TD>
<TD><CODE>int</CODE></TD>
<TD><CODE>FuncAdd, FuncSubtract, Min, Max, LogicOp</CODE></TD>
</TR>
<TR>
<TD><CODE>BlendEquationSeparate</CODE></TD>
<TD><CODE>int2(rgb, alpha)</CODE></TD>
<TD><CODE>FuncAdd, FuncSubtract, Min, Max, LogicOp</CODE></TD>
</TR>
<TR>
<TD><CODE>BlendColor</CODE></TD>
<TD><CODE>float4</CODE></TD>
<TD>&nbsp;</TD>
</TR>
<TR>
<TD><CODE>ClearColor</CODE></TD>
<TD><CODE>float4</CODE></TD>
<TD>&nbsp;</TD>
</TR>
<TR>
<TD><CODE>ClearStencil</CODE></TD>
<TD><CODE>int</CODE></TD>
<TD>&nbsp;</TD>
</TR>
<TR>
<TD><CODE>ClearDepth</CODE></TD>
<TD><CODE>float</CODE></TD>
<TD>&nbsp;</TD>
</TR>
<TR>
<TD><CODE>ClipPlane[ndx]</CODE></TD>
<TD><CODE>float4</CODE></TD>
<TD>&nbsp;</TD>
</TR>
<TR>
<TD><CODE>ColorMask</CODE></TD>
<TD><CODE>bool4</CODE></TD>
<TD>&nbsp;</TD>
</TR>
<TR>
<TD><CODE>CullFace</CODE></TD>
<TD><CODE>int</CODE></TD>
<TD><CODE>Front, Back, FrontAndBack</CODE></TD>
</TR>
<TR>
<TD><CODE>DepthFunc</CODE></TD>
<TD><CODE>int</CODE></TD>
<TD><CODE>Never, Less, LEqual, Equal, Greater, NotEqual, GEqual, Always</CODE></TD>
</TR>
<TR>
<TD><CODE>DepthMask</CODE></TD>
<TD><CODE>bool</CODE></TD>
<TD>&nbsp;</TD>
</TR>
<TR>
<TD><CODE>DepthRange</CODE></TD>
<TD><CODE>float2</CODE></TD>
<TD>&nbsp;</TD>
</TR>
<TR>
<TD><CODE>FogMode</CODE></TD>
<TD><CODE>int</CODE></TD>
<TD><CODE>Linear, Exp, Exp2</CODE></TD>
</TR>
<TR>
<TD><CODE>FogDensity</CODE></TD>
<TD><CODE>float</CODE></TD>
<TD>&nbsp;</TD>
</TR>
<TR>
<TD><CODE>FogStart</CODE></TD>
<TD><CODE>float</CODE></TD>
<TD>&nbsp;</TD>
</TR>
<TR>
<TD><CODE>FogEnd</CODE></TD>
<TD><CODE>float</CODE></TD>
<TD>&nbsp;</TD>
</TR>
<TR>
<TD><CODE>FogColor</CODE></TD>
<TD><CODE>float4</CODE></TD>
<TD>&nbsp;</TD>
</TR>
<TR>
<TD><CODE>FragmentProgram</CODE></TD>
<TD><CODE>compile</CODE> statement</TD>
<TD>&nbsp;</TD>
</TR>
<TR>
<TD><CODE>FrontFace</CODE></TD>
<TD><CODE>int</CODE></TD>
<TD><CODE>CW, CCW</CODE></TD>
</TR>
<TR>
<TD><CODE>LightModelAmbient</CODE></TD>
<TD><CODE>float4</CODE></TD>
<TD>&nbsp;</TD>
</TR>
<TR>
<TD><CODE>LightAmbient[ndx]</CODE></TD>
<TD><CODE>float4</CODE></TD>
<TD>&nbsp;</TD>
</TR>
<TR>
<TD><CODE>LightConstantAttenuation[ndx]</CODE></TD>
<TD><CODE>float</CODE></TD>
<TD>&nbsp;</TD>
</TR>
<TR>
<TD><CODE>LightDiffuse[ndx]</CODE></TD>
<TD><CODE>float4</CODE></TD>
<TD>&nbsp;</TD>
</TR>
<TR>
<TD><CODE>LightLinearAttenuation[ndx]</CODE></TD>
<TD><CODE>float</CODE></TD>
<TD>&nbsp;</TD>
</TR>
<TR>
<TD><CODE>LightPosition[ndx]</CODE></TD>
<TD><CODE>float4</CODE></TD>
<TD>&nbsp;</TD>
</TR>
<TR>
<TD><CODE>LightQuadraticAttenuation[ndx]</CODE></TD>
<TD><CODE>float</CODE></TD>
<TD>&nbsp;</TD>
</TR>
<TR>
<TD><CODE>LightSpecular[ndx]</CODE></TD>
<TD><CODE>float4</CODE></TD>
<TD>&nbsp;</TD>
</TR>
<TR>
<TD><CODE>LightSpotCutoff[ndx]</CODE></TD>
<TD><CODE>float</CODE></TD>
<TD>&nbsp;</TD>
</TR>
<TR>
<TD><CODE>LightSpotDirection[ndx]</CODE></TD>
<TD><CODE>float3</CODE></TD>
<TD>&nbsp;</TD>
</TR>
<TR>
<TD><CODE>LightSpotExponent[ndx]</CODE></TD>
<TD><CODE>float</CODE></TD>
<TD>&nbsp;</TD>
</TR>
<TR>
<TD><CODE>LineWidth</CODE></TD>
<TD><CODE>float</CODE></TD>
<TD>&nbsp;</TD>
</TR>
<TR>
<TD><CODE>LogicOp</CODE></TD>
<TD><CODE>int</CODE></TD>
<TD><CODE>Clear, And, AndReverse, Copy, AndInverted, Noop, Xor, Or, Nor, Equiv, Invert, OrReverse, CopyInverted, Nand, Set</CODE></TD>
</TR>
<TR>
<TD><CODE>MaterialAmbient</CODE></TD>
<TD><CODE>float4</CODE></TD>
<TD>&nbsp;</TD>
</TR>
<TR>
<TD><CODE>MaterialDiffuse</CODE></TD>
<TD><CODE>float4</CODE></TD>
<TD>&nbsp;</TD>
</TR>
<TR>
<TD><CODE>MaterialEmission</CODE></TD>
<TD><CODE>float4</CODE></TD>
<TD>&nbsp;</TD>
</TR>
<TR>
<TD><CODE>MaterialShininess</CODE></TD>
<TD><CODE>float</CODE></TD>
<TD>&nbsp;</TD>
</TR>
<TR>
<TD><CODE>MaterialSpecular</CODE></TD>
<TD><CODE>float4</CODE></TD>
<TD>&nbsp;</TD>
</TR>
<TR>
<TD><CODE>ModelViewMatrix</CODE></TD>
<TD><CODE>float4x4</CODE></TD>
<TD>&nbsp;</TD>
</TR>
<TR>
<TD><CODE>PointSize</CODE></TD>
<TD><CODE>float</CODE></TD>
<TD>&nbsp;</TD>
</TR>
<TR>
<TD><CODE>PointSpriteCoordReplace[ndx]</CODE></TD>
<TD><CODE>bool</CODE></TD>
<TD>&nbsp;</TD>
</TR>
<TR>
<TD><CODE>PolygonMode</CODE></TD>
<TD><CODE>int2</CODE></TD>
<TD><CODE>Front, Back, FrontAndBack, Point, Line, Fill</CODE></TD>
</TR>
<TR>
<TD><CODE>PolygonOffset</CODE></TD>
<TD><CODE>float2</CODE></TD>
<TD>&nbsp;</TD>
</TR>
<TR>
<TD><CODE>ProjectionMatrix</CODE></TD>
<TD><CODE>float4x4</CODE></TD>
<TD>&nbsp;</TD>
</TR>
<TR>
<TD><CODE>Scissor</CODE></TD>
<TD><CODE>int4</CODE></TD>
<TD>&nbsp;</TD>
</TR>
<TR>
<TD><CODE>ShadeModel</CODE></TD>
<TD><CODE>int</CODE></TD>
<TD><CODE>Flat, Smooth</CODE></TD>
</TR>
<TR>
<TD><CODE>StencilFunc</CODE></TD>
<TD><CODE>int3</CODE></TD>
<TD><CODE>Never, Less, LEqual, Equal, Greater, NotEqual, GEqual, Always</CODE></TD>
</TR>
<TR>
<TD><CODE>StencilMask</CODE></TD>
<TD><CODE>int</CODE></TD>
<TD>&nbsp;</TD>
</TR>
<TR>
<TD><CODE>StencilOp</CODE></TD>
<TD><CODE>int3</CODE></TD>
<TD><CODE>Keep, Zero, Replace, Incr, Decr, Invert, IncrWrap, DecrWrap</CODE></TD>
</TR>
<TR>
<TD><CODE>StencilFuncSeparate</CODE></TD>
<TD><CODE>int4</CODE></TD>
<TD><CODE>Front, Back, FrontAndBack, Never, Less, LEqual, Equal, Greater, NotEqual, GEqual, Always</CODE></TD>
</TR>
<TR>
<TD><CODE>StencilMaskSeparate</CODE></TD>
<TD><CODE>int2</CODE></TD>
<TD><CODE>Front, Back, FrontAndBack</CODE></TD>
</TR>
<TR>
<TD><CODE>StencilOpSeparate</CODE></TD>
<TD><CODE>int4</CODE></TD>
<TD><CODE>Keep, Zero, Replace, Incr, Decr, Invert, IncrWrap, DecrWrap</CODE></TD>
</TR>
<TR>
<TD><CODE>TexGenSMode[ndx]</CODE></TD>
<TD><CODE>int</CODE></TD>
<TD><CODE>ObjectLinear, EyeLinear, SphereMap, ReflectionMap, NormalMap</CODE></TD>
</TR>
<TR>
<TD><CODE>TexGenTMode[ndx]</CODE></TD>
<TD><CODE>int</CODE></TD>
<TD>&nbsp;</TD>
</TR>
<TR>
<TD><CODE>TexGenRMode[ndx]</CODE></TD>
<TD><CODE>int</CODE></TD>
<TD><CODE>ObjectLinear, EyeLinear, ReflectionMap, NormalMap</CODE></TD>
</TR>
<TR>
<TD><CODE>TexGenQMode[ndx]</CODE></TD>
<TD><CODE>int</CODE></TD>
<TD><CODE>ObjectLinear, EyeLinear</CODE></TD>
</TR>
<TR>
<TD><CODE>TexGenSEyePlane[ndx]</CODE></TD>
<TD><CODE>float4</CODE></TD>
<TD>&nbsp;</TD>
</TR>
<TR>
<TD><CODE>TexGenTEyePlane[ndx]</CODE></TD>
<TD><CODE>float4</CODE></TD>
<TD>&nbsp;</TD>
</TR>
<TR>
<TD><CODE>TexGenREyePlane[ndx]</CODE></TD>
<TD><CODE>float4</CODE></TD>
<TD>&nbsp;</TD>
</TR>
<TR>
<TD><CODE>TexGenQEyePlane[ndx]</CODE></TD>
<TD><CODE>float4</CODE></TD>
<TD>&nbsp;</TD>
</TR>
<TR>
<TD><CODE>TexGenSObjectPlane[ndx]</CODE></TD>
<TD><CODE>float4</CODE></TD>
<TD>&nbsp;</TD>
</TR>
<TR>
<TD><CODE>TexGenTObjectPlane[ndx]</CODE></TD>
<TD><CODE>float4</CODE></TD>
<TD>&nbsp;</TD>
</TR>
<TR>
<TD><CODE>TexGenRObjectPlane[ndx]</CODE></TD>
<TD><CODE>float4</CODE></TD>
<TD>&nbsp;</TD>
</TR>
<TR>
<TD><CODE>TexGenQObjectPlane[ndx]</CODE></TD>
<TD><CODE>float4</CODE></TD>
<TD>&nbsp;</TD>
</TR>
<TR>
<TD><CODE>Texture2D[ndx]</CODE></TD>
<TD><CODE>sampler2D</CODE></TD>
<TD>&nbsp;</TD>
</TR>
<TR>
<TD><CODE>Texture3D[ndx]</CODE></TD>
<TD><CODE>sampler3D</CODE></TD>
<TD>&nbsp;</TD>
</TR>
<TR>
<TD><CODE>TextureCubeMap[ndx]</CODE></TD>
<TD><CODE>samplerCUBE</CODE></TD>
<TD>&nbsp;</TD>
</TR>
<TR>
<TD><CODE>TextureEnvColor[ndx]</CODE></TD>
<TD><CODE>float4</CODE></TD>
<TD>&nbsp;</TD>
</TR>
<TR>
<TD><CODE>TextureEnvMode[ndx]</CODE></TD>
<TD><CODE>int</CODE></TD>
<TD><CODE>Modulate, Decal, Blend, Replace, Add</CODE></TD>
</TR>
<TR>
<TD><CODE>VertexProgram</CODE></TD>
<TD><CODE>compile</CODE> statement</TD>
<TD>&nbsp;</TD>
</TR>
</TABLE>

Similarly, there is a simple algorithm for determining the relationship
between enumerants for <code>glEnable()</code> and for
<code>glDisable()</code> and each of the states in the table below.  For
example, the state assignment <code>BlendEnable = false</code> corresponds to a call to <code>glDisable(GL_BLEND)</code>.

<TABLE rules=all>
<TR>
<TH align="center"><B>State Name</B></TH>
<TH align="center"><B>Type</B></TH>
</TR>
<TR>
<TD><CODE>AlphaTestEnable</CODE></TD>
<TD><CODE>bool</CODE></TD>
</TR>
<TR>
<TD><CODE>BlendEnable</CODE></TD>
<TD><CODE>bool</CODE></TD>
</TR>
<TR>
<TD><CODE>ClipPlaneEnable[ndx]</CODE></TD>
<TD><CODE>bool</CODE></TD>
</TR>
<TR>
<TD><CODE>ColorLogicOpEnable</CODE></TD>
<TD><CODE>bool</CODE></TD>
</TR>
<TR>
<TD><CODE>CullFaceEnable</CODE></TD>
<TD><CODE>bool</CODE></TD>
</TR>
<TR>
<TD><CODE>DepthTestEnable</CODE></TD>
<TD><CODE>bool</CODE></TD>
</TR>
<TR>
<TD><CODE>DitherEnable</CODE></TD>
<TD><CODE>bool</CODE></TD>
</TR>
<TR>
<TD><CODE>FogEnable</CODE></TD>
<TD><CODE>bool</CODE></TD>
</TR>
<TR>
<TD><CODE>LightEnable[ndx]</CODE></TD>
<TD><CODE>bool</CODE></TD>
</TR>
<TR>
<TD><CODE>LightingEnable</CODE></TD>
<TD><CODE>bool</CODE></TD>
</TR>
<TR>
<TD><CODE>LightModelTwoSideEnable</CODE></TD>
<TD><CODE>bool</CODE></TD>
</TR>
<TR>
<TD><CODE>LineSmoothEnable</CODE></TD>
<TD><CODE>bool</CODE></TD>
</TR>
<TR>
<TD><CODE>MultisampleEnable</CODE></TD>
<TD><CODE>bool</CODE></TD>
</TR>
<TR>
<TD><CODE>NormalizeEnable</CODE></TD>
<TD><CODE>bool</CODE></TD>
</TR>
<TR>
<TD><CODE>PointSmoothEnable</CODE></TD>
<TD><CODE>bool</CODE></TD>
</TR>
<TR>
<TD><CODE>PointSpriteEnable</CODE></TD>
<TD><CODE>bool</CODE></TD>
</TR>
<TR>
<TD><CODE>PolygonOffsetFillEnable</CODE></TD>
<TD><CODE>bool</CODE></TD>
</TR>
<TR>
<TD><CODE>RescaleNormalEnable</CODE></TD>
<TD><CODE>bool</CODE></TD>
</TR>
<TR>
<TD><CODE>SampleAlphaToCoverageEnable</CODE></TD>
<TD><CODE>bool</CODE></TD>
</TR>
<TR>
<TD><CODE>SampleAlphaToOneEnable</CODE></TD>
<TD><CODE>bool</CODE></TD>
</TR>
<TR>
<TD><CODE>SampleCoverageEnable[ndx]</CODE></TD>
<TD><CODE>bool</CODE></TD>
</TR>
<TR>
<TD><CODE>ScissorTestEnable</CODE></TD>
<TD><CODE>bool</CODE></TD>
</TR>
<TR>
<TD><CODE>StencilTestEnable</CODE></TD>
<TD><CODE>bool</CODE></TD>
</TR>
<TR>
<TD><CODE>TexGenSEnable[ndx]</CODE></TD>
<TD><CODE>bool</CODE></TD>
</TR>
<TR>
<TD><CODE>TexGenTEnable[ndx]</CODE></TD>
<TD><CODE>bool</CODE></TD>
</TR>
<TR>
<TD><CODE>TexGenREnable[ndx]</CODE></TD>
<TD><CODE>bool</CODE></TD>
</TR>
<TR>
<TD><CODE>TexGenQEnable[ndx]</CODE></TD>
<TD><CODE>bool</CODE></TD>
</TR>
<TR>
<TD><CODE>Texture2DEnable[ndx]</CODE></TD>
<TD><CODE>bool</CODE></TD>
</TR>
<TR>
<TD><CODE>Texture3dEnable[ndx]</CODE></TD>
<TD><CODE>bool</CODE></TD>
</TR>
<TR>
<TD><CODE>TextureCubeMapEnable[ndx]</CODE></TD>
<TD><CODE>bool</CODE></TD>
</TR>
<TR>
<TD><CODE>VertexProgramPointSizeEnable[ndx]</CODE></TD>
<TD><CODE>bool</CODE></TD>
</TR>
</TABLE>

The following table lists the state assignments available in <code>sampler_state</code> blocks
when using the PSGLFX state manager.  Any state values given are set when the <code>cgSetSamplerState()</code> routine is called with the <code>CGparameter</code> handle for a particular sample.

<TABLE rules=all>
<TR>
<TD align="center"><B>Name</B></TH>
<TD align="center"><B>Type</B></TH>
<TD align="center"><B>Valid Values</B></TH>
</TR>
<TR>
<TD><CODE>WrapS, WrapT, WrapR</CODE></TD>
<TD><CODE>int</CODE></TD>
<TD><CODE>Repeat, Clamp, ClampToEdge, ClampToBorder, MirroredRepeat,
MirrorClamp, MirrorClampToEdge, MirrorClampToBorder</CODE></TD>
</TR>
<TR>
<TD><CODE>CompareMode</CODE></TD>
<TD><CODE>int</CODE></TD>
<TD><CODE>None, CompareRToTexture</CODE></TD>
</TR>
<TR>
<TD><CODE>CompareFunc</CODE></TD>
<TD><CODE>int</CODE></TD>
<TD><CODE>Never, Less, LEqual, Equal, Greater, NotEqual, GEqual, Always</CODE></TD>
</TR>
<TR>
<TD><CODE>DepthMode</CODE></TD>
<TD><CODE>int</CODE></TD>
<TD><CODE>Alpha, Intensity, Luminance</CODE></TD>
</TR>
<TR>
<TD><CODE>LODBias</CODE></TD>
<TD><CODE>float</CODE></TD>
<TD>&nbsp;</TD>
</TR>
<TR>
<TD><CODE>MinFilter</CODE></TD>
<TD><CODE>int</CODE></TD>
<TD><CODE>Nearest, Linear, LinearMipMapNearest, NearestMipMapNearest,
NearestMipMapLinear, LinearMipMapLinear</CODE></TD>
</TR>
<TR>
<TD><CODE>MagFilter</CODE></TD>
<TD><CODE>int</CODE></TD>
<TD><CODE>Nearest, Linear</CODE></TD>
</TR>
<TR>
<TD><CODE>MaxAnisotropy</CODE></TD>
<TD><CODE>float</CODE></TD>
<TD>&nbsp;</TD>
</TR>
</TABLE>

@param context Specifies the context that the states will be added to.

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_CONTEXT_HANDLE_ERROR</CODE></TD>
<TD><I><c>context</c></I> is invalid.</TD>
</TR>
</TABLE>

@sa cgCreateState, cgCreateSamplerState, cgAddStateEnumerant
 */
CGGL_API void cgGLRegisterStates( CGcontext context )
{
    if ( !CG_IS_CONTEXT( context ) )
    {
        _jsCgRaiseError( CG_INVALID_CONTEXT_HANDLE_ERROR );
        return;
    }
    PSGLFX::registerGLStates( _cgGetContextPtr( context ) );
}
/**
 * @}
 */
// ------------------------------------------------------
