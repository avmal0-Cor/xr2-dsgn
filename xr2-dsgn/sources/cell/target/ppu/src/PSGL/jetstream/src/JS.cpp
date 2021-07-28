/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 *                Copyright (C) 2009 Sony Computer Entertainment Inc.
 *                                               All Rights Reserved.
 */


/*
 * Jetstream graphics API globals
 */


#include <assert.h>
#include <math.h>
#include <float.h>
#include <stdio.h>

#include <PSGL/psgl.h>
#include <PSGL/private.h>
#include <PSGL/Debug.h>
#include <PSGL/Utils.h>
#include <PSGL/Matrix.h>
#include <PSGL/Malloc.h>
#include <PSGL/PerfCounters.h>
#include <PSGL/APIEntryTimers.h>
#include <PSGL/DPM2.h>
#include <string.h>
#include <limits.h>

#include <Cg/CgCommon.h>

#include <RasterInlines.h>

#include <cell/sysmodule.h>

static char* _jsVendorString = "Sony Computer Entertainment Inc.";

#ifndef JS_DPM
static char* _jsRendererString = "JETSTREAM-A";
#else
#ifdef JS_DPM_V1
static char* _jsRendererString = "JETSTREAM-A DPM";
#endif
#ifdef JS_DPM_V2
static char* _jsRendererString = "JETSTREAM-A HUD";
#endif
#endif
static char* _jsExtensionsString = "";

static char* _jsVersionNumber = JS_LIBRARY_VERSION_NUMBER; // Just the number
char* jsVersion = JS_LIBRARY_VERSION;  // Number and OS


// the global instance of the PSGL context
PSGLcontext* _CurrentContext = NULL;

PSGL_EXPORT JScontextHookFunction _jsContextCreateHook = NULL;
PSGL_EXPORT JScontextHookFunction _jsContextDestroyHook = NULL;

// This function is there to hold compile time asserts.
/*static*/ void unusedFunction( void )
{
    JS_COMPILE_TIME_ASSERT( sizeof( jsUint8 ) == 1 );
    JS_COMPILE_TIME_ASSERT( sizeof( jsUint16 ) == 2 );
    JS_COMPILE_TIME_ASSERT( sizeof( jsUint32 ) == 4 );
    JS_COMPILE_TIME_ASSERT( sizeof( jsUint64 ) == 8 );
    JS_COMPILE_TIME_ASSERT( sizeof( JSint8 ) == 1 );
    JS_COMPILE_TIME_ASSERT( sizeof( JSint16 ) == 2 );
    JS_COMPILE_TIME_ASSERT( sizeof( JSint32 ) == 4 );
    JS_COMPILE_TIME_ASSERT( sizeof( JSint64 ) == 8 );
}

/////////////////////////////////////////////////////////////////////////////
// glError managment

void _jsSetError( GLenum error )
{
#ifndef JS_NO_ERROR_CHECK
    _JS_REPORT_EXTRA( PSGL_REPORT_GL_ERROR, "GL error %s (0x%x)", _jsGetGLErrorName( error ), error );
    if ( !_CurrentContext ) return;
    if ( _CurrentContext->error == GL_NO_ERROR ) _CurrentContext->error = error;
#endif
}

/**
 * @addtogroup OpenGL
 * @{
 */

/**
 * @short Return error information
 *
 * glGetError() returns the value of the error flag. Each detectable error is assigned a numeric code
 * and symbolic name. When an error occurs, the error flag is set to the appropriate error code value.
 * No other Errors are recorded until glGetError() is called, the error code is returned, and the
 * flag is reset to <CODE>GL_NO_ERROR</CODE>. If a call to glGetError() returns <CODE>GL_NO_ERROR</CODE>,
 * there has been no detectable error since the last call to glGetError(), or since the GL was initialized.
 *
 * To allow for distributed implementations, there may be several error flags. If any single error flag
 * has recorded an error, the value of that flag is returned and that flag is reset to
 * <CODE>GL_NO_ERROR</CODE> when glGetError() is called. If more than one flag has recorded an error,
 * glGetError() returns and clears an arbitrary error flag value. Thus, glGetError() should always be
 * called in a loop, until it returns <CODE>GL_NO_ERROR</CODE>, if all error flags are to be reset.
 *
 * Initially, all error flags are set to <CODE>GL_NO_ERROR</CODE>.
 *
 * The following errors are currently defined:

 * <TABLE rules=all>
 * <TR>
 * <TD><CODE>GL_NO_ERROR</CODE></TD>
 * <TD>No error has been recorded. The value of this symbolic constant is
 *   guaranteed to be 0.</TD>
 * </TR>
 * <TR>
 * <TD><CODE>GL_INVALID_ENUM</CODE></TD>
 * <TD>An unacceptable value is specified for an enumerated argument. The
 *   offending command is ignored, and has no other side effect than to set
 *   the error flag.</TD>
 * </TR>
 * <TR>
 * <TD><CODE>GL_INVALID_VALUE</CODE></TD>
 * <TD>A numeric argument is out of range. The offending command is ignored,
 *   and has no other side effect than to set the error flag.</TD>
 * </TR>
 * <TR>
 * <TD><CODE>GL_INVALID_OPERATION</CODE></TD>
 * <TD>The specified operation is not allowed in the current state. The
 *   offending command is ignored, and has no other side effect than to set
 *   the error flag.</TD>
 * </TR>
 * <TR>
 * <TD><CODE>GL_STACK_OVERFLOW</CODE></TD>
 * <TD>This command would cause a stack overflow. The offending command is
 *   ignored, and has no other side effect than to set the error flag.</TD>
 * </TR>
 * <TR>
 * <TD><CODE>GL_STACK_UNDERFLOW</CODE></TD>
 * <TD>This command would cause a stack underflow. The offending command is
 *   ignored, and has no other side effect than to set the error flag.</TD>
 * </TR>
 * <TR>
 * <TD><CODE>GL_OUT_OF_MEMORY</CODE></TD>
 * <TD>There is not enough memory left to execute the command. The state of
 *   the GL context is undefined, except for the state of the error
 *   flags, after this error is recorded.</TD>
 * </TR>
 <TR>
<TD><CODE>GL_INVALID_FRAMEBUFFER_OPERATION_OES</CODE></TD>
<TD>One or more of the following conditions is not true:
<UL>
<LI>All textures are complete.</LI>
<LI>Color formats are consistent across the color textures</LI>
<LI>Swizzling hint is consistent across all the textures</LI>
<LI>Drawable color format for the color attachments if any</LI>
<LI>Drawable depth format for the depth attachment if any</LI>
<LI>No anti-aliasing if FP16 or FP32 color format</LI>
<LI>No swizzling with RGBA/FP16 or RGBA/FP32 (any format that is more than
32 bits per pixel)</LI>
<LI>Matching sizes, if swizzled</LI>
</UL></TD>
</TR>
* </TABLE>
 *
 * When an error flag is set, results of a GL operation are undefined only
 * if <CODE>GL_OUT_OF_MEMORY</CODE> has occurred. In all other cases, the command generating
 * the error is ignored and has no effect on the GL state or frame buffer
 * contents. If the generating command returns a value, it returns 0. If
 * glGetError() itself generates an error, it returns 0.
 *
 * @return Returns the current error flag value
 *
 * @note
 * Cg and OpenGL have completely separate error reporting systems.  The error statuses stored for the
 * two systems are different.  Thus, cgGetError() and glGetError() refer to different error status codes.
 */
GLAPI GLenum APIENTRY glGetError()
{
    JS_APIENTRY_TIMER( glGetError );

    if ( _jsdpmState ) { _JS_DPM_API_ENTRY_0( glGetError, JSDPM_CALL_TYPE_GET, 0 ); }

    if ( !_CurrentContext ) return GL_INVALID_OPERATION;
    else
    {
        GLenum error = _CurrentContext->error;

        _CurrentContext->error = GL_NO_ERROR;
        return error;
    }
}

/**  @}  OpenGL  */

/////////////////////////////////////////////////////////////////////////////
// GL state validation
// _jsValidateStates() causes the RSX hardware to be updated with actual
// state changes that are encapsulated in the PSGL context.

GLuint _jsValidateStates( GLuint mask )
{
    _JS_PERFORMANCE_START( ValidateStatesCounter );
    PSGLcontext* LContext = _CurrentContext;
    JS_ASSERT( LContext );

    static const GLuint defaultImageUnitMapping[] = { 0, 1, 2, 3 };
    JS_COMPILE_TIME_ASSERT( _JS_MAX_TEXTURE_UNITS == sizeof( defaultImageUnitMapping ) / sizeof( defaultImageUnitMapping[0] ) );

	GLuint  dirty = LContext->needValidate & ~mask;
	LContext->needValidate &= mask;

    GLuint  needValidate = LContext->needValidate;
    GLboolean hasVP = ( LContext->VertexProgram && LContext->BoundVertexProgram != NULL );
    GLboolean hasFP = ( LContext->FragmentProgram && LContext->BoundFragmentProgram != NULL );

    // validate surfaces first. It may have to draw a quad to filter half float buffers,
    // that would in turn invalidate many states
    _JS_PERFORMANCE_START( ValidateSurfacesCounter );
    if ( JS_UNLIKELY( needValidate & PSGL_VALIDATE_FRAMEBUFFER ) )
    {
        _jsValidateFramebuffer();

        // update the cache of validation bits
        needValidate = LContext->needValidate;
    }
    _JS_PERFORMANCE_STOP( ValidateSurfacesCounter );

    // this is shared for fp and ffx fp
    // this has to be before ffx fp validation because of a corner case:
    // if a bound texture is incomplete, the validation of the texture will change the setting of the
    // ffx fp manager.
    _JS_PERFORMANCE_START( ValidateTextureCounter );
    if ( JS_UNLIKELY( needValidate & PSGL_VALIDATE_TEXTURES_USED ) )
    {
        long unitInUseCount;
        const GLuint* unitsInUse;
		//TODO: Ced, measure perf impact
		if ( hasFP && LContext->BoundFragmentProgram == LContext->ShellFragmentProgram )
		{
			for ( long i = 0; i < _JS_MAX_TEXTURE_IMAGE_UNITS; ++i )
			{
				jsTexture* texture = LContext->TextureImageUnits[i].currentTexture;
				// the test would be unnecessary if we were tracking the ffx units as well as the fp ones.
				if ( texture )
					_jsPlatformValidateTextureStage( i, texture );
			}
		}
		else
		{
			if ( JS_LIKELY( hasFP ) )
			{
				unitInUseCount = LContext->BoundFragmentProgram->samplerCount;
				unitsInUse = LContext->BoundFragmentProgram->samplerUnits;
			}
			else
			{
				unitInUseCount = _JS_MAX_TEXTURE_UNITS;
				unitsInUse = defaultImageUnitMapping;
			}
			for ( long i = 0; i < unitInUseCount; ++i )
			{
				long unit = unitsInUse[i];
				jsTexture* texture = LContext->TextureImageUnits[unit].currentTexture;

				// the test would be unnecessary if we were tracking the ffx units as well as the fp ones.
				if ( texture )
					_jsPlatformValidateTextureStage( unit, texture );
			}
		}
    }
    _JS_PERFORMANCE_STOP( ValidateTextureCounter );

    if ( JS_LIKELY( hasVP ) )
    {
        _JS_PERFORMANCE_START( ValidateVertexPipeCounter );
        if ( JS_UNLIKELY( needValidate & PSGL_VALIDATE_VERTEX_PROGRAM ) )
        {
            _jsValidateVertexProgram();
        }

        // mvp at least ?
        if ( JS_LIKELY( needValidate & PSGL_VALIDATE_VERTEX_CONSTANTS ) )
        {
            _jsValidateVertexConstants();
        }
        _JS_PERFORMANCE_STOP( ValidateVertexPipeCounter );

        _JS_PERFORMANCE_START( ValidateTextureCounter );
        if ( JS_UNLIKELY( needValidate & PSGL_VALIDATE_VERTEX_TEXTURES_USED ) )
        {
            _jsPlatformValidateVertexTextures();
        }
        _JS_PERFORMANCE_STOP( ValidateTextureCounter );
    }
    else
    {
        _JS_PERFORMANCE_START( ValidateFFXCounter );
        if ( JS_UNLIKELY( needValidate & PSGL_VALIDATE_FFX_VERTEX_PROGRAM ) )
        {
            _jsValidateFFXVertexProgram();
        }
        _JS_PERFORMANCE_STOP( ValidateFFXCounter );
    }

    if ( JS_LIKELY( hasFP ) )
    {
        _JS_PERFORMANCE_START( ValidateFragmentProgramCounter );
        if ( JS_UNLIKELY( needValidate & PSGL_VALIDATE_FRAGMENT_PROGRAM ) )
        {
            _jsValidateFragmentProgram();
        }
        _JS_PERFORMANCE_STOP( ValidateFragmentProgramCounter );
    }
    else
    {
        _JS_PERFORMANCE_START( ValidateFFXCounter );
        if ( JS_UNLIKELY( needValidate & PSGL_VALIDATE_FFX_FRAGMENT_PROGRAM ) )
        {
            _jsValidateFFXFragmentProgram();
        }
        _JS_PERFORMANCE_STOP( ValidateFFXCounter );
    }
	
	// early out
    if ( JS_LIKELY(( needValidate & ~( PSGL_VALIDATE_TEXTURES_USED |
                                       PSGL_VALIDATE_VERTEX_PROGRAM |
                                       PSGL_VALIDATE_VERTEX_CONSTANTS |
                                       PSGL_VALIDATE_VERTEX_TEXTURES_USED |
                                       PSGL_VALIDATE_FRAGMENT_PROGRAM ) ) == 0 ) )
    {
        LContext->needValidate = 0;
        _JS_PERFORMANCE_STOP( ValidateStatesCounter );
        return dirty;
    }

    // XXX should this one be in the fast path ?
    if ( JS_LIKELY( hasFP ) && JS_UNLIKELY( needValidate & PSGL_VALIDATE_FRAGMENT_SHARED_CONSTANTS ) )
    {
        _jsValidateFragmentProgramSharedConstants();
    }

    if ( JS_UNLIKELY( needValidate & PSGL_VALIDATE_VIEWPORT ) )
    {
        _jsValidateViewport();
    }

    if ( JS_UNLIKELY( needValidate & PSGL_VALIDATE_ALPHA_TEST ) )
    {
        _jsValidateAlphaTest();
    }

    if ( JS_UNLIKELY( needValidate & PSGL_VALIDATE_DEPTH_TEST ) )
    {
        _jsValidateDepthTest();
    }

    if ( JS_UNLIKELY( needValidate & PSGL_VALIDATE_WRITE_MASK ) )
    {
        _jsValidateWriteMask();
    }

    if ( JS_UNLIKELY( needValidate & PSGL_VALIDATE_STENCIL_TEST ) )
    {
        _jsValidateStencilTest();
    }

    if ( JS_UNLIKELY( needValidate & PSGL_VALIDATE_STENCIL_OP_AND_MASK ) )
    {
        _jsValidateStencilOpAndMask();
    }

    if ( JS_UNLIKELY( needValidate & PSGL_VALIDATE_SCISSOR_BOX ) )
    {
        _jsValidateScissorBox();
    }

    if ( JS_UNLIKELY( needValidate & PSGL_VALIDATE_FACE_CULL ) )
    {
        _jsValidateCullFace();
    }
    if ( JS_UNLIKELY( needValidate & PSGL_VALIDATE_BLENDING ) )
    {
        _jsValidateBlending();
    }

    if ( JS_UNLIKELY( needValidate & PSGL_VALIDATE_POINT_RASTER ) )
    {
        _jsValidatePointRaster();
    }

    if ( JS_UNLIKELY( needValidate & PSGL_VALIDATE_LINE_RASTER ) )
    {
        _jsValidateLineRaster();
    }

    if ( JS_UNLIKELY( needValidate & PSGL_VALIDATE_POLYGON_OFFSET ) )
    {
        _jsValidatePolygonOffset();
    }

    if ( JS_UNLIKELY( needValidate & PSGL_VALIDATE_SHADE_MODEL ) )
    {
        _jsValidateShadeModel();
    }

    if ( JS_UNLIKELY( needValidate & PSGL_VALIDATE_LOGIC_OP ) )
    {
        _jsValidateLogicOp();
    }

    if ( JS_UNLIKELY( needValidate & PSGL_VALIDATE_MULTISAMPLING ) )
    {
        _jsValidateMultisampling();
    }

    if ( JS_UNLIKELY( needValidate & PSGL_VALIDATE_POLYGON_MODE ) )
    {
        _jsValidatePolygonMode();
    }

    if ( JS_UNLIKELY( needValidate & PSGL_VALIDATE_PRIMITIVE_RESTART ) )
    {
        _jsValidatePrimitiveRestart();
    }

    if ( JS_UNLIKELY( needValidate & PSGL_VALIDATE_CLIP_PLANES ) )
    {
        _jsValidateClipPlanes();
    }

    if ( JS_UNLIKELY( needValidate & PSGL_VALIDATE_SHADER_SRGB_REMAP ) )
    {
        _jsValidateShaderSRGBRemap();
    }

    if ( JS_UNLIKELY( needValidate & PSGL_VALIDATE_POINT_SPRITE ) )
    {
        _jsValidatePointSprite();
    }

    if ( JS_UNLIKELY( needValidate & PSGL_VALIDATE_TWO_SIDE_COLOR ) )
    {
        _jsValidateTwoSideColor();
    }

    LContext->needValidate = 0;
    _JS_PERFORMANCE_STOP( ValidateStatesCounter );
    return dirty;
}

/////////////////////////////////////////////////////////////////////////////
// PSGL context management

/**
 * @addtogroup PSGL
 *
 * @{
 */

/**
 * @short Retrieves the current context.
 *
 * psglGetCurrentContext returns a pointer to the current context.
 *
 * @return Pointer to the current context, or NULL if no context is current
 *
 * @sa psglCreateContext, psglMakeCurrent
 */
PSGLcontext *psglGetCurrentContext()
{
    return _CurrentContext;
}

/**
 * @} PSGL
 */

void _jsResetAttributeState( jsAttributeState* as )
{
    // vertex program attributes
    for ( int i = 0; i < _JS_MAX_VERTEX_ATTRIBS; ++i )
    {
        // client state (for jsVertexAttribPointer)
        as->attrib[i].clientSize = 4;
        as->attrib[i].clientType = GL_FLOAT;
        as->attrib[i].clientStride = 16;
        as->attrib[i].clientData = NULL;

        // current attribute value
        as->attrib[i].value[0] = 0.0f;
        as->attrib[i].value[1] = 0.0f;
        as->attrib[i].value[2] = 0.0f;
        as->attrib[i].value[3] = 1.0f;

        as->attrib[i].normalized = GL_FALSE;
        as->attrib[i].frequency = 1;

        as->attrib[i].arrayBuffer = 0;
    }

    // The fixed-function pipeline attributes alias the vertex program
    // attributes.  Here we set up specific fixed function defaults.
    as->attrib[_JS_ATTRIB_PRIMARY_COLOR_INDEX].value[0] = 1.0f;
    as->attrib[_JS_ATTRIB_PRIMARY_COLOR_INDEX].value[1] = 1.0f;
    as->attrib[_JS_ATTRIB_PRIMARY_COLOR_INDEX].value[2] = 1.0f;
    as->attrib[_JS_ATTRIB_PRIMARY_COLOR_INDEX].value[3] = 1.0f;

    as->attrib[_JS_ATTRIB_SECONDARY_COLOR_INDEX].value[0] = 1.0f;
    as->attrib[_JS_ATTRIB_SECONDARY_COLOR_INDEX].value[1] = 1.0f;
    as->attrib[_JS_ATTRIB_SECONDARY_COLOR_INDEX].value[2] = 1.0f;
    as->attrib[_JS_ATTRIB_SECONDARY_COLOR_INDEX].value[3] = 1.0f;

    as->attrib[_JS_ATTRIB_NORMAL_INDEX].value[0] = 0.f;
    as->attrib[_JS_ATTRIB_NORMAL_INDEX].value[1] = 0.f;
    as->attrib[_JS_ATTRIB_NORMAL_INDEX].value[2] = 1.f;

    as->DirtyMask = ( 1 << _JS_MAX_VERTEX_ATTRIBS ) - 1; // mark all dirty
    as->EnabledMask = 0;
    as->NeedsConversionMask = 0;
    as->HasVBOMask = 0;
    as->ModuloMask = 0;

    as->PrimitiveRestart = GL_FALSE;
    as->RestartIndex = 0;

    as->ElementArrayBuffer = 0;
}

static void _jsResetContext( PSGLcontext *LContext )
{
    _jsMatrixStackReset( &( LContext->ModelViewMatrixStack ) );
    _jsMatrixStackReset( &( LContext->ProjectionMatrixStack ) );
    _jsTexNameSpaceResetNames( &LContext->textureNameSpace );
    _jsTexNameSpaceResetNames( &LContext->bufferObjectNameSpace );
    _jsTexNameSpaceResetNames( &LContext->framebufferNameSpace );
    _jsTexNameSpaceResetNames( &LContext->renderbufferNameSpace );
    _jsTexNameSpaceResetNames( &LContext->eventObjectNameSpace );
    _jsTexNameSpaceResetNames( &LContext->fenceObjectNameSpace );
    _jsTexNameSpaceResetNames( &LContext->queryNameSpace );
    _jsTexNameSpaceResetNames( &LContext->attribSetNameSpace );

    LContext->InverseModelViewValid = GL_FALSE;
    LContext->ScalingFactor = 1.f;
    LContext->RescaleNormals = GL_FALSE;
    LContext->Normalize = GL_FALSE;

    LContext->ViewPort.X = 0;
    LContext->ViewPort.Y = 0;
    LContext->ViewPort.XSize = 0;
    LContext->ViewPort.YSize = 0;

    LContext->ScissorTest = GL_FALSE;
    LContext->ScissorBox.X = 0;
    LContext->ScissorBox.Y = 0;
    LContext->ScissorBox.XSize = 0;
    LContext->ScissorBox.YSize = 0;

    LContext->ColorMaterial = GL_FALSE;
    LContext->FaceCulling = GL_FALSE;
    LContext->CullFaceMode = GL_BACK;
    LContext->FrontFace = GL_CCW;
    LContext->PointSmooth = GL_FALSE;
    LContext->PointSize = 1.f;
    LContext->PointSmoothHint = GL_DONT_CARE;
    LContext->LineSmooth = GL_FALSE;
    LContext->LineWidth = 1.f;
    LContext->LineSmoothHint = GL_DONT_CARE;
    LContext->PolygonOffsetFactor = 0.f;
    LContext->PolygonOffsetUnits = 0.f;
    LContext->PolygonOffsetFill = GL_FALSE;
    LContext->PolygonOffsetLine = GL_FALSE;
    LContext->ShadeModel = GL_SMOOTH;
    LContext->PerspectiveCorrectHint = GL_DONT_CARE;

    // light model setting
    LContext->LightModel.Ambient.R = 0;
    LContext->LightModel.Ambient.G = 0;
    LContext->LightModel.Ambient.B = 0;
    LContext->LightModel.Ambient.A = 0;
    LContext->LightModel.Light2Sided = GL_FALSE;

    _jsMaterialInit( &( LContext->FrontMaterial ) );
    _jsMaterialInit( &( LContext->BackMaterial ) );


    jsLight* LLight = LContext->Lights;
    for ( int LC = 0;LC < _JS_MAX_LIGHTNUM;LC++, LLight++ )
    {
        _jsLightInit( LLight, LC + GL_LIGHT0 );
        LContext->ActiveLights[LC] = NULL;
    }

    LContext->NumOfActiveLights = 0;

    for ( int i = 0; i < _JS_MAX_CLIP_PLANES; ++i )
    {
        LContext->ClipPlane[i].Active = GL_FALSE;
        LContext->ClipPlane[i].Coeff[0] = 0.0f;
        LContext->ClipPlane[i].Coeff[1] = 0.0f;
        LContext->ClipPlane[i].Coeff[2] = 0.0f;
        LContext->ClipPlane[i].Coeff[3] = 0.0f;
    }

    LContext->DepthNear = 0.f;
    LContext->DepthFar = 1.f;
    LContext->DepthClamp = GL_FALSE;


    // Frame buffer-related fields

    // DrawBuffer is currently not supported at all.
    // ReadBuffer is supported only for multiple render targets for user
    // framebuffer objects, i.e. GL_COLOR_ATTACHMENTn_EXT.  Support for the
    // window framebuffer (GL_BACK, GL_FRONT, etc.) is not in OpenGL ES 1.0
    // and is not currently in PSGL.
    LContext->DrawBuffer = LContext->ReadBuffer = GL_COLOR_ATTACHMENT0_EXT;

    LContext->ClearColor.R = 0.f;
    LContext->ClearColor.G = 0.f;
    LContext->ClearColor.B = 0.f;
    LContext->ClearColor.A = 0.f;
    LContext->ClearDepth = 1.f;
    LContext->ClearStencil = 0;
    LContext->AccumClearColor.R = 0.f;
    LContext->AccumClearColor.G = 0.f;
    LContext->AccumClearColor.B = 0.f;
    LContext->AccumClearColor.A = 0.f;

    LContext->Fog = GL_FALSE;
    LContext->FogHint = GL_DONT_CARE;
    LContext->FogMode = GL_EXP;
    LContext->FogColor.R = 0.f;
    LContext->FogColor.G = 0.f;
    LContext->FogColor.B = 0.f;
    LContext->FogColor.A = 0.f;
    LContext->FogDensity = 1.f;
    LContext->FogStart = 0.f;
    LContext->FogEnd = 1.f;

    LContext->ShaderSRGBRemap = GL_FALSE;

    LContext->DepthTest = GL_FALSE;
    LContext->DepthFunc = GL_LESS;

    LContext->AlphaTest = GL_FALSE;
    LContext->AlphaFunc = GL_ALWAYS;
    LContext->AlphaRef = 0.f;

    LContext->StencilTest = GL_FALSE;
    LContext->StencilFunc = GL_ALWAYS;
    LContext->StencilRef = 0;
    LContext->StencilFuncMask = ~0;
    LContext->StencilOpFail = GL_KEEP;
    LContext->StencilOpZFail = GL_KEEP;
    LContext->StencilOpZPass = GL_KEEP;

    LContext->ActiveStencilFace = GL_FRONT;
    LContext->StencilTestTwoSide = GL_FALSE;
    LContext->StencilBackfaceFunc = GL_ALWAYS;
    LContext->StencilBackfaceRef = 0;
    LContext->StencilBackfaceFuncMask = ~0;
    LContext->StencilBackfaceOpFail = GL_KEEP;
    LContext->StencilBackfaceOpZFail = GL_KEEP;
    LContext->StencilBackfaceOpZPass = GL_KEEP;

    LContext->Blending = GL_FALSE;
    LContext->BlendingMrt[0] = GL_FALSE;
    LContext->BlendingMrt[1] = GL_FALSE;
    LContext->BlendingMrt[2] = GL_FALSE;
    LContext->BlendColor.R = 0.0f;
    LContext->BlendColor.G = 0.0f;
    LContext->BlendColor.B = 0.0f;
    LContext->BlendColor.A = 0.0f;
    LContext->BlendEquationRGB = GL_FUNC_ADD;
    LContext->BlendEquationAlpha = GL_FUNC_ADD;
    LContext->BlendFactorSrcRGB = GL_ONE;
    LContext->BlendFactorDestRGB = GL_ZERO;
    LContext->BlendFactorSrcAlpha = GL_ONE;
    LContext->BlendFactorDestAlpha = GL_ZERO;

    LContext->ColorLogicOp = GL_FALSE;
    LContext->LogicOp = GL_COPY;

    LContext->Dithering = GL_TRUE;

    LContext->ReduceDstColor = GL_TRUE;
    LContext->Multisample = GL_TRUE;
    LContext->SampleCoverage = GL_FALSE;
    LContext->SampleAlphaToCoverage = GL_FALSE;
    LContext->SampleAlphaToOne = GL_FALSE;
    LContext->SampleCoverageInvert = GL_FALSE;
    LContext->SampleCoverageValue = 1.f;

    LContext->StencilWriteMask = ~0;
    LContext->StencilBackfaceWriteMask = ~0;
    LContext->WriteMask = _JS_CONTEXT_COLOR_MASK | _JS_CONTEXT_DEPTH_MASK;


    LContext->FrontPolygonMode = LContext->BackPolygonMode = GL_FILL;

    LContext->Lighting = GL_FALSE;

    LContext->TexCoordReplaceMask = 0;
    LContext->PointSprite = GL_FALSE;
    LContext->VertexProgramPointSize = GL_FALSE;

    for ( int i = 0;i < _JS_MAX_TEXTURE_COORDS;++i )
    {
        jsTextureCoordsUnit *tu = LContext->TextureCoordsUnits + i;
        tu->revalidate = 0;
        _jsMatrixStackReset( &( tu->TextureMatrixStack ) );

        tu->enableTextureMatrix = GL_FALSE;
        tu->texGenSize = 0;

        for ( int j = 0;j < 4;++j )
        {
            tu->texGen[j].enabled = GL_FALSE;
            tu->texGen[j].mode = GL_EYE_LINEAR;
            for ( int k = 0;k < 4;++k )
            {
                if ( j < 4 && k == j )
                {
                    tu->texGen[j].objectPlane[k] = 1.f;
                    tu->texGen[j].eyePlane[k] = 1.f;
                }
                else
                {
                    tu->texGen[j].objectPlane[k] = 0.f;
                    tu->texGen[j].eyePlane[k] = 0.f;
                }
            }
        }
    }
    for ( int i = 0;i < _JS_MAX_TEXTURE_IMAGE_UNITS;++i )
    {
        jsTextureImageUnit *tu = LContext->TextureImageUnits + i;
        tu->bound2D = 0;
        tu->bound3D = 0;
        tu->boundCube = 0;

        tu->enable2D = GL_FALSE;
        tu->enable3D = GL_FALSE;
        tu->enableCube = GL_FALSE;
        tu->fragmentTarget = 0;

        tu->envMode = GL_MODULATE;
        tu->envColor.R = 0.f;
        tu->envColor.G = 0.f;
        tu->envColor.B = 0.f;
        tu->envColor.A = 0.f;

        tu->lodBias = 0.f;
        tu->currentTexture = NULL;
    }
    for ( int i = 0;i < _JS_MAX_VERTEX_TEXTURE_IMAGE_UNITS;++i )
    {
        LContext->VertexTextureImages[i] = NULL;
    }

    LContext->ActiveTexture = 0;
    LContext->CurrentImageUnit = LContext->TextureImageUnits;
    LContext->CurrentCoordsUnit = LContext->TextureCoordsUnits;

    LContext->packAlignment = 4;
    LContext->unpackAlignment = 4;

    // Client-states
    //
    LContext->CS_ActiveTexture = 0;

    _jsResetAttributeState( &LContext->defaultAttribs0 );
    JS_ASSERT( _jsCheckAttribCaches( &LContext->defaultAttribs0 ) );
    LContext->attribs = &LContext->defaultAttribs0;
    LContext->attribSetName = 0;
    LContext->attribSetDirty = GL_FALSE;

    LContext->framebuffer = 0;
    LContext->renderbuffer = 0;

    LContext->samplesPassedQuery = 0;
    LContext->conditionalRenderQuery = 0;

    LContext->VertexProgram = GL_FALSE;
    LContext->BoundVertexProgram = 0;

    LContext->FragmentProgram = GL_FALSE;
    LContext->BoundFragmentProgram = 0;

    LContext->ArrayBuffer = 0;
    LContext->PixelPackBuffer = 0;
    LContext->PixelUnpackBuffer = 0;
    LContext->TextureBuffer = 0;

    LContext->VSync = GL_FALSE;
    LContext->SkipFirstVSync = GL_FALSE;

	LContext->AllowTXPDemotion = GL_FALSE; 

}


/**
 * @addtogroup PSGL
 *
 * @{
 */

/**
 * @short Create a new context.
 *
 * Creates a new PSGLcontext and initializes it.
 *
 * @return Pointer to the newly created context, or NULL in case of an error
 *
 * @sa psglDestroyContext
 */
PSGLcontext* psglCreateContext()
{
    PSGLcontext* LContext = ( PSGLcontext* )jsMalloc( sizeof( PSGLcontext ) );
    if ( !LContext ) return NULL;

    memset( LContext, 0, sizeof( PSGLcontext ) );

    LContext->error = GL_NO_ERROR;
    LContext->MatrixMode = GL_MODELVIEW;

    _jsMatrixStackInit( &( LContext->ModelViewMatrixStack ), _JS_MAX_MODELVIEW_STACK_DEPTH );
    if ( !LContext->ModelViewMatrixStack.MatrixStackf )
    {
        psglDestroyContext( LContext );
        return NULL;
    }
    _jsMatrixStackInit( &( LContext->ProjectionMatrixStack ), _JS_MAX_PROJECTION_STACK_DEPTH );
    if ( !LContext->ProjectionMatrixStack.MatrixStackf )
    {
        psglDestroyContext( LContext );
        return NULL;
    }

    for ( int i = 0; i < _JS_MAX_TEXTURE_COORDS; i++ )
    {
        _jsMatrixStackInit( &( LContext->TextureCoordsUnits[i].TextureMatrixStack ), _JS_MAX_TEXTURE_STACK_DEPTH );
        if ( !LContext->TextureCoordsUnits[i].TextureMatrixStack.MatrixStackf )
        {
            psglDestroyContext( LContext );
            return NULL;
        }
    }

    _jsTexNameSpaceInit( &LContext->textureNameSpace, ( jsTexNameSpaceCreateFunction )_jsAllocateTexture, ( jsTexNameSpaceDestroyFunction )_jsFreeTexture );

    for ( int i = 0;i < _JS_MAX_TEXTURE_IMAGE_UNITS;++i )
    {
        jsTextureImageUnit *tu = LContext->TextureImageUnits + i;

        tu->default2D = _jsAllocateTexture();
        if ( !tu->default2D )
        {
            psglDestroyContext( LContext );
            return NULL;
        }
        tu->default2D->target = GL_TEXTURE_2D;

        tu->default3D = _jsAllocateTexture();
        if ( !tu->default3D )
        {
            psglDestroyContext( LContext );
            return NULL;
        }
        tu->default3D->target = GL_TEXTURE_3D;

        tu->defaultCube = _jsAllocateTexture();
        if ( !tu->defaultCube )
        {
            psglDestroyContext( LContext );
            return NULL;
        }
        tu->defaultCube->faceCount = 6;
        tu->defaultCube->target = GL_TEXTURE_CUBE_MAP;
    }

    _jsTexNameSpaceInit( &LContext->bufferObjectNameSpace, ( jsTexNameSpaceCreateFunction )_jsCreateBufferObject, ( jsTexNameSpaceDestroyFunction )_jsFreeBufferObject );
    _jsTexNameSpaceInit( &LContext->fenceObjectNameSpace, ( jsTexNameSpaceCreateFunction )_jsCreateFenceObject, ( jsTexNameSpaceDestroyFunction )_jsFreeFenceObject );
    _jsTexNameSpaceInit( &LContext->eventObjectNameSpace, ( jsTexNameSpaceCreateFunction )_jsCreateEventObject, ( jsTexNameSpaceDestroyFunction )_jsFreeEventObject );
    _jsTexNameSpaceInit( &LContext->framebufferNameSpace, ( jsTexNameSpaceCreateFunction )_jsCreateFramebuffer, ( jsTexNameSpaceDestroyFunction )_jsDestroyFramebuffer );
    // renderbuffers are textures in a different namespace
    _jsTexNameSpaceInit( &LContext->renderbufferNameSpace, ( jsTexNameSpaceCreateFunction )_jsAllocateTexture, ( jsTexNameSpaceDestroyFunction )_jsFreeTexture );
    _jsTexNameSpaceInit( &LContext->queryNameSpace, ( jsTexNameSpaceCreateFunction )_jsCreateQuery, ( jsTexNameSpaceDestroyFunction )_jsDestroyQuery );
    _jsTexNameSpaceInit( &LContext->attribSetNameSpace, ( jsTexNameSpaceCreateFunction )_jsCreateAttribSet, ( jsTexNameSpaceDestroyFunction )_jsDestroyAttribSet );

    LContext->needValidate = 0;
    LContext->everAttached = 0;

    // cg api
    // almost all the cg state hangs off of context head.
    LContext->JScgLastError = CG_NO_ERROR;
    LContext->JScgErrorCallbackFunction = NULL;
    LContext->JScgContextHead = ( CGcontext )NULL;
    //shell vertex and fragment program for user managed shaders
    char *shellVertexBlock = (char *)jsMalloc(sizeof(_CGprogram)*2);
    memset(shellVertexBlock,0,sizeof(_CGprogram)*2);
    LContext->ShellVertexProgram = (_CGprogram*)shellVertexBlock;
    LContext->ShellFragmentProgram = (_CGprogram*)(shellVertexBlock + sizeof(_CGprogram));
	
    // namespaces
    _jsInitNameSpace( &LContext->cgProgramNameSpace );
    _jsInitNameSpace( &LContext->cgParameterNameSpace );
    _jsInitNameSpace( &LContext->cgContextNameSpace );

    _jsResetContext( LContext );

    if ( _jsContextCreateHook ) _jsContextCreateHook( LContext );

    return( LContext );
}

/**
 * @short Re-establishes default values for the current context.
 *
 * Re-establishes default values for the rendering state of the current context, including (but not limited to)
 * default lighting, matrix, texture, fog, and fragment operation states.
 */
void PSGL_EXPORT psglResetCurrentContext()
{
    if ( _jsdpmState ) { _JS_DPM_API_ENTRY_0( psglResetCurrentContext, JSDPM_CALL_TYPE_CONTEXT, ); }

    PSGLcontext *context = _CurrentContext;
    JS_ASSERT( context );
    _jsResetContext( context );
    context->needValidate |= PSGL_VALIDATE_ALL;
    int unit;
    for ( unit = 0;unit < _JS_MAX_TEXTURE_UNITS;unit++ )
    {
        context->TextureCoordsUnits[unit].revalidate = _JS_TEXTURE_UNIT_REVALIDATE_TEX_GEN;
    }
}

/**
 * @short Destroys a given context.
 *
 * Destroys a given PSGLcontext and associated resources, such as textures.
 *
 * @param LContext the context to be destroyed
 *
 * @sa psglCreateContext
 */

void PSGL_EXPORT psglDestroyContext( PSGLcontext* LContext )
{
    if ( _jsdpmState ) { _JS_DPM_API_ENTRY_0( psglDestroyContext, JSDPM_CALL_TYPE_CONTEXT, ); }

    if ( _CurrentContext == LContext )
    {
        _jsPlatformRasterFinish();
    }

    // destroy all cg contexts inside the gl context.
    while ( LContext->JScgContextHead != ( CGcontext )NULL )
    {
        // ugly trick. The current context might not be the one we are destroying.
        // yet we want to rely on the cg API to destroy the context.
        // So change the current context temporarily
        PSGLcontext* current = _CurrentContext;
        _CurrentContext = LContext;
        cgDestroyContext( LContext->JScgContextHead );
        _CurrentContext = current;
    }
    _jsFreeNameSpace( &LContext->cgProgramNameSpace );
    _jsFreeNameSpace( &LContext->cgParameterNameSpace );
    _jsFreeNameSpace( &LContext->cgContextNameSpace );

    if ( _jsContextDestroyHook ) _jsContextDestroyHook( LContext );

    _jsMatrixStackClear( &( LContext->ModelViewMatrixStack ) );
    _jsMatrixStackClear( &( LContext->ProjectionMatrixStack ) );

    for ( int i = 0; i < _JS_MAX_TEXTURE_COORDS; i++ )
    {
        _jsMatrixStackClear( &( LContext->TextureCoordsUnits[i].TextureMatrixStack ) );
    }

    LContext->NumOfActiveLights = 0;

    for ( int i = 0; i < _JS_MAX_TEXTURE_IMAGE_UNITS; ++i )
    {
        jsTextureImageUnit* tu = LContext->TextureImageUnits + i;
        if ( tu->default2D ) _jsFreeTexture( tu->default2D );
        if ( tu->default3D ) _jsFreeTexture( tu->default3D );
        if ( tu->defaultCube ) _jsFreeTexture( tu->defaultCube );
    }

	_jsTexNameSpaceFree( &LContext->textureNameSpace );
    _jsTexNameSpaceFree( &LContext->bufferObjectNameSpace );
    _jsTexNameSpaceFree( &LContext->fenceObjectNameSpace );
    _jsTexNameSpaceFree( &LContext->eventObjectNameSpace );
    _jsTexNameSpaceFree( &LContext->framebufferNameSpace );
    _jsTexNameSpaceFree( &LContext->renderbufferNameSpace );
    _jsTexNameSpaceFree( &LContext->attribSetNameSpace );
    _jsTexNameSpaceFree( &LContext->queryNameSpace );

	if ( _CurrentContext == LContext )
    {
        psglMakeCurrent( NULL, NULL );
    }

    //free shell vertex and fragment program for user managed shaders
    char *shellVertexBlock = (char *)LContext->ShellVertexProgram;
    jsFree(shellVertexBlock);

    jsFree( LContext );
}
/**
 * @}  PSGL
 */

void _jsInvalidateAllStates( PSGLcontext* context )
{
    context->needValidate = PSGL_VALIDATE_ALL;
    int unit;
    for ( unit = 0;unit < _JS_MAX_TEXTURE_UNITS;unit++ )
    {
        context->TextureCoordsUnits[unit].revalidate = _JS_TEXTURE_UNIT_REVALIDATE_TEX_GEN;
        context->TextureCoordsUnits[unit].TextureMatrixStack.dirty = GL_TRUE;
    }
    context->ModelViewMatrixStack.dirty = GL_TRUE;
    context->ProjectionMatrixStack.dirty = GL_TRUE;
    context->attribs->DirtyMask = ( 1 << _JS_MAX_VERTEX_ATTRIBS ) - 1; // mark all dirty
}

void _jsAttachContext( PSGLdevice *device, PSGLcontext* context )
{
    if ( !context->everAttached )
    {
        context->ViewPort.XSize = device->deviceParameters.width;
        context->ViewPort.YSize = device->deviceParameters.height;
        context->ScissorBox.XSize = device->deviceParameters.width;
        context->ScissorBox.YSize = device->deviceParameters.height;
        context->needValidate |= PSGL_VALIDATE_VIEWPORT | PSGL_VALIDATE_SCISSOR_BOX;
        context->everAttached = GL_TRUE;
    }
    _jsInvalidateAllStates( context );
}

/////////////////////////////////////////////////////////////////////////////
// glGet queries

/**
 * @addtogroup OpenGL
 * @{
 */

/**
 *  @short Retrieve float values for a state variable
 *
 *  glGetFloatv() returns values for simple state variables in GL.
 *  <I><c>pname</c></I> is a symbolic constant indicating the state variable to be returned, and <I><c>params</c></I> is a pointer to an array of the indicated type in which to place the returned data.
 *  The following symbolic constants are accepted by <I><c>pname</c></I>:

 * <TABLE rules=all>
 * <TR><TD><CODE>GL_MODELVIEW_MATRIX</CODE></TD>
 * <TD><I><c>params</c></I> returns sixteen values: the matrix on the top of the modelview matrix stack.
 * Initially this matrix is the identity matrix.
 * </TD></TR>
 * <TR><TD><CODE>GL_PROJECTION_MATRIX</CODE></TD>
 * <TD><I><c>params</c></I> returns sixteen values: the matrix on the top of the projection matrix stack.
 * Initially this matrix is the identity matrix.
 * </TD></TR>
 * <TR><TD><CODE>GL_TEXTURE_MATRIX</CODE></TD>
 * <TD><I><c>params</c></I> returns sixteen values: the matrix on the top of the texture matrix stack.
 * Initially this matrix is  the identity matrix.
 * </TD></TR>
 * <TR><TD><CODE>GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT</CODE></TD>
 * <TD><I><c>params</c></I> returns one value: the maximum level of texture anisotropy supported by this implementation.
 * </TD></TR>
 * </TABLE>
 *
 *  @param pname      Specifies the parameter value to be returned. The symbolic constants in the list below are accepted.
 *  @param params     Returns the value or values of the specified parameter.
 *
 * @par Errors
Use glGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>GL_INVALID_ENUM</CODE></TD>
<TD><I><c>pname</c></I> is not an accepted value.</TD>
</TR>
</TABLE>
 */
GLAPI void APIENTRY glGetFloatv( GLenum pname, GLfloat* params )
{
    JS_APIENTRY_TIMER( glGetFloatv );
    if ( _jsdpmState )
    {
        _JS_DPM_API_ENTRY_2( glGetFloatv, JSDPM_CALL_TYPE_GET, , 2,
                             pname, sizeof( GLenum ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM,
                             params, 16*sizeof( GLfloat ), INPUT_OUTPUT, DPM_API_ARGUMENT_POINTER );
    }

    PSGLcontext* LContext = _CurrentContext;
    jsMatrixStack* LMatrixStack = NULL;
    jsMatrixf  LMatrix = NULL;

    switch ( pname )
    {
        case GL_MODELVIEW_MATRIX:
            jsContextGetMatrixf( LContext, GL_MODELVIEW, LMatrixStack, LMatrix );
            break;

        case GL_PROJECTION_MATRIX:
            jsContextGetMatrixf( LContext, GL_PROJECTION, LMatrixStack, LMatrix );
            break;

        case GL_TEXTURE_MATRIX:
            jsContextGetMatrixf( LContext, GL_TEXTURE, LMatrixStack, LMatrix );
            break;
        case GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT:
        {
            int maxAniso = 0;
            _jsGetTextureIntegerv( pname, &maxAniso );
            *params = ( float )maxAniso;
        }
        return; // Does not need to memcpy in this case
        default:
            _jsSetError( GL_INVALID_ENUM );
            return;
    }
    memcpy( params, LMatrixStack->MatrixStackf + LMatrixStack->MatrixStackPtr * jsELEMENTS_IN_MATRIX, jsMATRIX_SIZEf );
}

/**
 *  @short Retrieve a boolean value for a state variable
 *
 *  glGetBooleanv() returns values for simple state variables in GL.
 *  <I><c>pname</c></I> is a symbolic constant indicating the state variable to be returned, and <I><c>params</c></I> is a pointer to an array of the indicated type in which to place the returned data.
 *  The following symbolic constants are accepted by <I><c>pname</c></I>:
 *
 * <TABLE rules=all>
 * <TR><TD><CODE>GL_LIGHTING</CODE></TD>
 * <TD><I><c>params</c></I> returns the value of the current lighting state
 * </TD></TR>
 * </TABLE>
 *
 *  @param pname      Specifies the parameter value to be returned. The symbolic constants in the list below are accepted
 *  @param params     Returns the value or values of the specified parameter
 *
 * @par Errors
Use glGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>GL_INVALID_ENUM</CODE></TD>
<TD><I><c>pname</c></I> is not an accepted value.</TD>
</TR>
</TABLE>
 */
GLAPI void APIENTRY glGetBooleanv( GLenum pname, GLboolean* params )
{
    JS_APIENTRY_TIMER( glGetBooleanv );
    if ( _jsdpmState )
    {
        _JS_DPM_API_ENTRY_2( glGetBooleanv, JSDPM_CALL_TYPE_GET, , 2,
                             pname, sizeof( GLenum ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM,
                             params, sizeof( GLboolean ), INPUT_OUTPUT, DPM_API_ARGUMENT_POINTER );
    }

    PSGLcontext* LContext = _CurrentContext;

    switch ( pname )
    {
        case GL_LIGHTING:
            *params = LContext->Lighting;
            break;
        default:
            _jsSetError( GL_INVALID_ENUM );
            break;
    }
}

/**
 *  @short Retrieve an integer value for a state variable
 *
 *  glGetIntegerv() returns values for static state variables.
 *  <I><c>pname</c></I> is a symbolic constant indicating the state variable to be returned,
 *  and <I><c>params</c></I> is a pointer to an array of the indicated type in which to place the returned data.
 *
 *  The following symbolic constants are accepted by <I><c>pname</c></I>:
 *
 * <TABLE rules=all>
 * <TR><TD><CODE>GL_ALIASED_POINT_SIZE_RANGE</CODE></TD><TD>
 * <I><c>params</c></I> returns two values, the smallest and largest supported sizes for aliased points. The range
 * must include 1.
 *
 * See also glPointSize()
 * </TD></TR>
 * <TR><TD><CODE>GL_ALIASED_LINE_WIDTH_RANGE</CODE></TD><TD>
 * <I><c>params</c></I> returns two values, the smallest and largest supported widths for aliased lines. The range
 * must include 1.
 *
 * See also glLineWidth()
 * </TD></TR>
 * <TR><TD><CODE>GL_ALPHA_BITS</CODE></TD><TD>
 * <I><c>params</c></I> returns one value, the number of alpha bitplanes in the color buffer.
 * </TD></TR>
 * <TR><TD><CODE>GL_BLUE_BITS</CODE></TD><TD>
 * <I><c>params</c></I> returns one value, the number of blue bitplanes in the color buffer.
 * </TD></TR>
 * <TR><TD><CODE>GL_COMPRESSED_TEXTURE_FORMATS</CODE></TD><TD>
 * <I><c>params</c></I> returns <CODE>GL_NUM_COMPRESSED_TEXTURE_FORMATS</CODE> values, the supported compressed texture formats.
 *
 * See also glCompressedTexImage2D(), glCompressedTexSubImage2D()
 * </TD></TR>
 * <TR><TD><CODE>GL_DEPTH_BITS</CODE></TD><TD>
 * <I><c>params</c></I> returns one value, the number of bitplanes in the depth buffer.
 * </TD></TR>
 * <TR><TD><CODE>GL_GREEN_BITS</CODE></TD><TD>
 * <I><c>params</c></I> returns one value, the number of green bitplanes in the color buffer.
 * </TD></TR>
 * <TR><TD><CODE>GL_IMPLEMENTATION_COLOR_READ_FORMAT_OES</CODE></TD><TD>
 * <I><c>params</c></I> returns one value, the preferred format for pixel read back.
 *
 * See also glReadPixels()
 * </TD></TR>
 * <TR><TD><CODE>GL_IMPLEMENTATION_COLOR_READ_TYPE_OES</CODE></TD><TD>
 * <I><c>params</c></I> returns one value, the preferred type for pixel read back.
 *
 * See also glReadPixels()
 * </TD></TR>
 * <TR><TD><CODE>GL_MAX_CLIP_PLANES</CODE></TD><TD>
 * <I><c>params</c></I> returns one value, the maximum number of user-defined clipping planes.
 *
 * See also glClipPlanef()
 * </TD></TR>
 * <TR><TD><CODE>GL_MAX_DRAW_BUFFERS_ATI, GL_MAX_COLOR_ATTACHMENTS_OES</CODE></TD><TD>
 * <I><c>params</c></I> returns one value, the maximum number of drawable
 * color attachments for a frame buffer.
 *
 * See also glFramebufferRenderbufferOES(), glFramebufferTexture2DOES()
 * </TD></TR>
 * <TR><TD><CODE>GL_MAX_ELEMENTS_INDICES</CODE></TD><TD>
 * <I><c>params</c></I> returns one value, the recommended maximum number of vertex array indices.
 *
 * See also glDrawElements(), glDrawRangeElements()
 * </TD></TR>
 * <TR><TD><CODE>GL_MAX_ELEMENTS_VERTICES</CODE></TD><TD>
 * <I><c>params</c></I> returns one value, the recommended maximum number of vertex array vertices.
 *
 * See also glDrawArrays(), glDrawElements(), glDrawRangeElements()
 * </TD></TR>
 * <TR><TD><CODE>GL_MAX_LIGHTS</CODE></TD><TD>
 * <I><c>params</c></I> returns one value, the maximum number of lights. The value must be at least 8.
 *
 * See also glLightfv()
 * </TD></TR>
 * <TR><TD><CODE>GL_MAX_MODELVIEW_STACK_DEPTH</CODE></TD><TD>
 * <I><c>params</c></I> returns one value, the maximum supported depth of the modelview matrix stack. The value
 * must be at least 16.
 *
 * See also glPushMatrix()
 * </TD></TR>
 * <TR><TD><CODE>GL_MAX_PROJECTION_STACK_DEPTH</CODE></TD><TD>
 * <I><c>params</c></I> returns one value, the maximum supported depth of the projection matrix stack. The value
 * must be at least 2.
 *
 * See also glPushMatrix()
 * </TD></TR>
 * <TR><TD><CODE>GL_MAX_TEXTURE_COORDS_ARB</CODE></TD><TD>
 * <I><c>params</c></I> returns a single value indicating the number of texture coordinate sets supported. This value may
 * be greater than the value returned using <CODE>GL_MAX_TEXTURE_UNITS</CODE>, and the additional texture coordinate
 * sets are accessible from a shader program. The value must be at least 1.
 *
 * See also glClientActiveTexture(), glMultiTexCoord4f()
 * </TD></TR>
 * <TR><TD><CODE>GL_MAX_TEXTURE_IMAGE_UNITS_ARB</CODE></TD><TD>
 * <I><c>params</c></I> returns a single value indicating the total number of texture units supported, including
 * units supported only by shader programs. The value must be
 * at least 1.
 *
 * See also glActiveTexture()
 * </TD></TR>
 * <TR><TD><CODE>GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT</CODE></TD><TD>
 * <I><c>params</c></I> returns a single value indicating the maximum ratio of anisotropic texture filtering.
 * The value must be greater than or equal to 1.
 *
 * See also glTexParameterf()
 * </TD></TR>
 * <TR><TD><CODE>GL_MAX_TEXTURE_SIZE</CODE></TD><TD>
 * <I><c>params</c></I> returns one value. The value gives a rough estimate of the largest 2D texture that the GL can
 * handle. The value must be at least 64.
 *
 * See also glTexImage2D(), glCompressedTexImage2D(), glCopyTexImage2D()
 * </TD></TR>
 * <TR><TD><CODE>GL_MAX_3D_TEXTURE_SIZE</CODE></TD><TD>
 * <I><c>params</c></I> returns one value. The value gives a rough estimate of the largest 3D texture that the GL can
 * handle. The value must be at least 64.
 *
 * See also glTexImage3D()
 * </TD></TR>
 * <TR><TD><CODE>GL_MAX_TEXTURE_STACK_DEPTH</CODE></TD><TD>
 * <I><c>params</c></I> returns one value, the maximum supported depth of the texture matrix stack. The value must be at least 2.
 *
 * See also glPushMatrix()
 * </TD></TR>
 * <TR><TD><CODE>GL_MAX_TEXTURE_UNITS</CODE></TD><TD>
 * <I><c>params</c></I> returns a single value indicating the number of texture units supported by fixed-pipeline OpenGL functions. The value
 * returned does not include any texture units that are only supported by shader programs. The value must be
 * at least 1.
 *
 * See also glActiveTexture(), glClientActiveTexture(), glMultiTexCoord4f()
 * </TD></TR>
 * <TR><TD><CODE>GL_MAX_VIEWPORT_DIMS</CODE></TD><TD>
 * <I><c>params</c></I> returns two values: the maximum supported width and height of the viewport. These must be
 * at least as large as the visible dimensions of the display being rendered to.
 *
 * See also glViewport()
 * </TD></TR>
 * <TR><TD><CODE>GL_NUM_COMPRESSED_TEXTURE_FORMATS</CODE></TD><TD>
 * <I><c>params</c></I> returns one value, the number of supportex compressed texture formats. The value must be at
 * least 10.
 *
 * See also glCompressedTexImage2D(), glCompressedTexSubImage2D()
 * </TD></TR>
 * <TR><TD><CODE>GL_RED_BITS</CODE></TD><TD>
 * <I><c>params</c></I> returns one value, the number of red bitplanes in each color buffer.
 * </TD></TR>
 * <TR><TD><CODE>GL_SMOOTH_LINE_WIDTH_RANGE</CODE></TD><TD>
 * <I>params</I> returns two values, the smallest and largest supported widths for antialiased lines. The
 * range must include 1.
 *
 * See also glLineWidth()
 * </TD></TR>
 * <TR><TD><CODE>GL_SMOOTH_POINT_SIZE_RANGE</CODE></TD><TD>
 * <I><c>params</c></I> returns two values, the smallest and largest supported widths for antialiased points. The
 * range must include 1.
 *
 * See also glPointSize()
 * </TD></TR>
 * <TR><TD><CODE>GL_STENCIL_BITS</CODE></TD><TD>
 * <I><c>params</c></I> returns one value, the number of bitplanes in the stencil buffer.
 * </TD></TR>
 * <TR><TD><CODE>GL_SUBPIXEL_BITS</CODE></TD><TD>
 * <I><c>params</c></I> returns one value, an estimate of the number of bits of subpixel resolution that are used
 * to position rasterized geometry in window coordinates. The value must be at least 4.
 * </TD></TR>
 * </TABLE>
 *
 *  @param pname      Specifies the parameter value to be returned. The symbolic constants in the list below are accepted
 *  @param params     Returns the value or values of the specified parameter
 *
 * @par Errors
Use glGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>GL_INVALID_ENUM</CODE></TD>
<TD><I><c>pname</c></I> is not an accepted value.</TD>
</TR>
</TABLE>
 *
 */
GLAPI void APIENTRY glGetIntegerv( GLenum pname, GLint* params )
{
    JS_APIENTRY_TIMER( glGetIntegerv );
#ifdef JS_DPM
    GLint paramCount;
    if ( _jsdpmState )
    {
        paramCount = 2;
        if ( pname == GL_COMPRESSED_TEXTURE_FORMATS ) _jsGetTextureIntegerv( GL_NUM_COMPRESSED_TEXTURE_FORMATS, &paramCount );
        _JS_DPM_API_ENTRY_2( glGetIntegerv, JSDPM_CALL_TYPE_GET, , 2,
                             pname, sizeof( GLenum ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM,
                             params, paramCount*sizeof( GLint ), INPUT_OUTPUT, DPM_API_ARGUMENT_POINTER );
    }
#endif

    switch ( pname )
    {
        case GL_IMPLEMENTATION_COLOR_READ_FORMAT_OES:
        case GL_IMPLEMENTATION_COLOR_READ_TYPE_OES:
        case GL_RED_BITS:
        case GL_GREEN_BITS:
        case GL_BLUE_BITS:
        case GL_ALPHA_BITS:
        case GL_STENCIL_BITS:
        case GL_DEPTH_BITS:
        case GL_SUBPIXEL_BITS:
            _jsPlatformFramebufferGetParameteriv( pname, params );
            break;
        case GL_MAX_VIEWPORT_DIMS:
            params[0] = INT_MAX;
            params[1] = INT_MAX;
            break;
        case GL_MAX_ELEMENTS_VERTICES:
        case GL_MAX_ELEMENTS_INDICES:
        case GL_MAX_DRAW_BUFFERS_ATI:
		case GL_MAX_COLOR_ATTACHMENTS_OES:
            _jsPlatformRasterGetIntegerv( pname, params );
            break;
        case GL_SMOOTH_POINT_SIZE_RANGE:
        case GL_ALIASED_POINT_SIZE_RANGE:
            params[0] = 1;
            params[1] = 63;
            break;
        case GL_SMOOTH_LINE_WIDTH_RANGE:
        case GL_ALIASED_LINE_WIDTH_RANGE:
            params[0] = 0;
            params[1] = 64; // the actual value is 63.875 (fixed-point 6.3 format), returns rounded int value, but it will be clamped properly
            break;
        case GL_MAX_MODELVIEW_STACK_DEPTH:
            *params = _JS_MAX_MODELVIEW_STACK_DEPTH;
            break;
        case GL_MAX_TEXTURE_STACK_DEPTH:
            *params = _JS_MAX_TEXTURE_STACK_DEPTH;
            break;
        case GL_MAX_PROJECTION_STACK_DEPTH:
            *params = _JS_MAX_PROJECTION_STACK_DEPTH;
            break;
        case GL_MAX_TEXTURE_UNITS:
            *params = MIN( _JS_MAX_TEXTURE_UNITS, _jsPlatformTextureMaxUnits() );
            break;
        case GL_MAX_TEXTURE_IMAGE_UNITS_ARB:
            *params = MIN( _JS_MAX_TEXTURE_IMAGE_UNITS, _jsPlatformTextureMaxUnits() );
            break;
        case GL_MAX_TEXTURE_COORDS_ARB:
            *params = _JS_MAX_TEXTURE_COORDS;
            break;
        case GL_MAX_LIGHTS:
            *params = _JS_MAX_LIGHTNUM;
            break;
        case GL_NUM_COMPRESSED_TEXTURE_FORMATS:
        case GL_COMPRESSED_TEXTURE_FORMATS:
        case GL_MAX_TEXTURE_SIZE:
        case GL_MAX_3D_TEXTURE_SIZE:
        case GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT:
            _jsGetTextureIntegerv( pname, params );
            break;
        case GL_MAX_CLIP_PLANES:
            params[0] = _JS_MAX_CLIP_PLANES;
            break;
        case GL_MAX_RENDERBUFFER_SIZE_OES:
            *params = _jsPlatformRenderbufferMaxSize();
            break;
        default:
            _jsSetError( GL_INVALID_ENUM );
            break;
    }
}
/** @} OpenGL
 */

/////////////////////////////////////////////////////////////////////////////
// server-side enable/disable/hint

/**
 *  @addtogroup OpenGL
 *  @{
 */

/**
 *  @short Specify implementation-specific hints
 *
 * Certain aspects of GL behavior, when there is room for interpretation, can be controlled with hints.
 * A hint is specified with two arguments.
 * <I><c>mode</c></I> can be one of the following:
 *
 * <TABLE rules=all>
 * <TR><TD><CODE>GL_FASTEST</CODE></TD>
 * <TD>The most efficient option should be chosen.
 * </TD></TR>
 * <TR><TD><CODE>GL_NICEST</CODE></TD>
 * <TD>The most correct, or highest quality, option should be chosen.
 * </TD></TR>
 * <TR><TD><CODE>GL_DONT_CARE</CODE></TD>
 * <TD>No preference.
 * </TD></TR>
 * </TABLE>
 *
Though the implementation aspects that can be hinted are well defined, the interpretation of the hints depends on the implementation. The hint aspects that can be specified with <I><c>target</c></I>, along with suggested semantics, are as follows:
 *
 * <TABLE rules=all>
 * <TR><TD><CODE>GL_FOG_HINT</CODE></TD>
 * <TD>Indicates the accuracy of fog calculation. If per-pixel fog calculation is not efficiently supported by the GL implementation, hinting <CODE>GL_DONT_CARE</CODE> or <CODE>GL_FASTEST</CODE> can result in per-vertex calculation of fog effects.
 * </TD></TR>
 * <TR><TD><CODE>GL_LINE_SMOOTH_HINT</CODE></TD>
 * <TD>Indicates the sampling quality of antialiased lines. If a larger filter function is applied, hinting <CODE>GL_NICEST</CODE> can result in more pixel fragments being generated during rasterization.
 * </TD></TR>
 * <TR><TD><CODE>GL_PERSPECTIVE_CORRECTION_HINT</CODE></TD>
 * <TD>Indicates the quality of color and texture coordinate interpolation. If perspective-corrected parameter interpolation is not efficiently supported by the GL implementation, hinting <CODE>GL_DONT_CARE</CODE> or <CODE>GL_FASTEST</CODE> can result in simple linear interpolation of colors and/or texture coordinates.
 * </TD></TR>
 * <TR><TD><CODE>GL_POINT_SMOOTH_HINT</CODE></TD>
 * <TD>Indicates the sampling quality of antialiased points. If a larger filter function is applied, hinting <CODE>GL_NICEST</CODE> can result in more pixel fragments being generated during rasterization.
 * </TD></TR>
 * </TABLE>
 *
 *  @param target     A symbolic constant indicating the behavior to be controlled. The initial value for each <I><c>target</c></I> is <CODE>GL_DONT_CARE</CODE>.
 *
 *  @param mode       A symbolic constant indicating the desired behavior
 *
 *  @note
 *  The interpretation of hints depends on the implementation. Some implementations ignore <CODE>glHint</CODE> settings.
 *
 *  @par Errors
Use glGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>GL_INVALID_ENUM</CODE></TD>
<TD>Either <I><c>target</c></I> or <I><c>mode</c></I> is not an accepted value.</TD>
</TR>
</TABLE>
 */
GLAPI void APIENTRY glHint( GLenum target, GLenum mode )
{
    JS_APIENTRY_TIMER( glHint );
    if ( _jsdpmState )
    {
        _JS_DPM_API_ENTRY_2( glHint, JSDPM_CALL_TYPE_STATE_GENERAL, , 2,
                             target, sizeof( GLenum ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM,
                             mode, sizeof( GLenum ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM );
    }

    PSGLcontext* LContext = _CurrentContext;
    JS_ASSERT( LContext );

#ifndef JS_NO_ERROR_CHECK
    if (( mode != GL_DONT_CARE ) && ( mode != GL_FASTEST ) && ( mode != GL_NICEST ) )
    {
        _jsSetError( GL_INVALID_ENUM );
        return;
    }
#endif

    switch ( target )
    {
        case GL_FOG_HINT:
            LContext->FogHint = mode;
            LContext->needValidate |= PSGL_VALIDATE_FFX_FRAGMENT_PROGRAM;
            break;
        case GL_POINT_SMOOTH_HINT:
            LContext->PointSmoothHint = mode;
            LContext->needValidate |= PSGL_VALIDATE_POINT_RASTER;
            break;
        case GL_LINE_SMOOTH_HINT:
            LContext->LineSmoothHint = mode;
            LContext->needValidate |= PSGL_VALIDATE_LINE_RASTER;
            break;
        case GL_PERSPECTIVE_CORRECTION_HINT:
            LContext->PerspectiveCorrectHint = mode;
            // silently ignored
            break;
        default:
            _jsSetError( GL_INVALID_ENUM );
            return;
    }
}

/**
 *  @short Enable server-side GL capabilities
 *
 *  glEnable() and glDisable() enable and disable various capabilities.
 * The initial value for each capability with the exception of <CODE>GL_DITHER</CODE>,
 * <CODE>GL_MULTISAMPLE</CODE> and <CODE>GL_REDUCE_DST_COLOR_SCE</CODE> is <CODE>GL_FALSE</CODE>.
 * The initial value for <CODE>GL_DITHER</CODE>, <CODE>GL_MULTISAMPLE</CODE> and
 * <CODE>GL_REDUCE_DST_COLOR_SCE</CODE> is <CODE>GL_TRUE</CODE>.
 *
 * Both glEnable() and glDisable() take a single argument, <I><c>cap</c></I>, which can
 * assume one of the following values:
 *
 * <TABLE rules=all>
 * <TR><TD><CODE>GL_ALPHA_TEST</CODE></TD>
 * <TD>If enabled, do alpha testing.</TD>
 * <TD>see glAlphaFunc()</TD>
 * </TR>
 * <TR><TD><CODE>GL_BLEND</CODE></TD>
 * <TD>If enabled, blend the incoming color values with the values in the color buffers.
 * This applies to all color targets if multiple render targets are used. If independent 
 * per-color-target enabling is needed, use the GL_BLEND_MRTi_SCE enums instead.</TD>
 * <TD>see glBlendFunc()</TD>
 * </TR>
 * <TR><TD><CODE>GL_BLEND_MRT<i>i</i>_SCE</CODE></TD>
 * <TD>If enabled, allows blending per color target when multiple render 
 * targets are used (<i>i</i> can be 0, 1, 2, or 3).</TD>
 * <TD>see glBlendFunc()</TD>
 * </TR>
 * <TR><TD><CODE>GL_CLIP_PLANE<i>i</i></CODE></TD>
 * <TD>If enabled, geometry is clipped against a user-defined clipping plane <I>i</I>.</TD>
 * <TD>see glClipPlanef()</TD>
 * </TR>
 * <TR><TD><CODE>GL_COLOR_LOGIC_OP</CODE></TD><TD>
 * If enabled, apply the currently selected logical operation to the incoming color and color buffer values.
 * </TD><TD> see glLogicOp()
 * </TD></TR>&nbsp;
 * <TR><TD><CODE>GL_COLOR_MATERIAL</CODE></TD><TD>
 * If enabled, have ambient and diffuse material parameters track the current color.
 * </TD><TD>&nbsp;
 * </TD></TR>
 * <TR><TD><CODE>GL_CULL_FACE</CODE></TD><TD>
 * If enabled, cull polygons based on their winding in window coordinates.
 * </TD><TD> see glCullFace()
 * </TD></TR>
 * <TR><TD><CODE>GL_DEPTH_CLAMP_NV</CODE></TD><TD>
 * If enabled, fragments that would otherwise be clipped by the near or far clipping planes instead
 * have their depth value clamped to the near or far value, respectively.
 * </TD><TD> see glDepthFunc(), glDepthMask(), and glDepthRangef()
 * </TD></TR>
 * <TR><TD><CODE>GL_DEPTH_TEST</CODE></TD><TD>
 * If enabled, do depth comparisons and update the depth buffer. Note that even if the depth
 * buffer exists and the depth mask is non-zero, the depth buffer is not updated if the
 * depth test is disabled.
 * </TD><TD> see glDepthFunc(), glDepthMask(), and glDepthRangef()
 * </TD></TR>&nbsp;
 * <TR><TD><CODE>GL_DITHER</CODE></TD><TD>
 * If enabled, dither color components or indices before they are written to the color buffer.
 * </TD><TD>&nbsp;
 * </TD></TR>
 * <TR><TD><CODE>GL_FOG</CODE></TD><TD>
 * If enabled, blend a fog color into the posttexturing color.
 * </TD><TD> see glFogfv()
 * </TD></TR>
 * <TR><TD><CODE>GL_LIGHTi</CODE></TD><TD>
 * If enabled, include light i in the evaluation of the lighting equation.
 * </TD><TD> see glLightModelfv() and glLightfv()
 * </TD></TR>
 * <TR><TD><CODE>GL_LIGHTING</CODE></TD><TD>
 * If enabled, use the current lighting parameters to compute the vertex color.
 * Otherwise, simply associate the current color with each vertex.
 * </TD><TD> see glMaterialfv(), glLightModelfv(), and glLightfv()
 * </TD></TR>
 * <TR><TD><CODE>GL_LINE_SMOOTH</CODE></TD><TD>
 * If enabled, draw lines with correct filtering. Otherwise, draw aliased lines.
 * </TD><TD> see glLineWidth()
 * </TD></TR>
 * <TR><TD><CODE>GL_MULTISAMPLE</CODE></TD><TD>
 * If enabled, perform multisampling of fragments for single-pass antialiasing and other effects.
 * </TD><TD> see glSampleCoverage()
 * </TD></TR>&nbsp;
 * <TR><TD><CODE>GL_REDUCE_DST_COLOR_SCE</CODE></TD><TD>
 * If enabled, performs color reduction (resolve) on 4x multisampled fragments when blending is used.
 * </TD><TD>&nbsp;
 * </TD></TR>
 * <TR><TD><CODE>GL_NORMALIZE</CODE></TD><TD>
 * If enabled, normal vectors are scaled to unit length after transformation.
 * </TD><TD> see glNormal3fv() and glNormalPointer()
 * </TD></TR>
 * <TR>
 * <TD><CODE>GL_POINT_SMOOTH</CODE></TD>
 * <TD>
 * If enabled, draw points with proper filtering. Otherwise, draw aliased points.
 * </TD>
 * <TD> see glPointSize()</TD>
 * </TR>
 * <TR>
 * <TD><CODE>GL_POINT_SPRITE_OES</CODE></TD>
 * <TD>
 * If enabled, points are rendered with texture coordinates that vary
 * across the point. For texture coordinates, a point is treated as
 * a two-dimensional square, with an upper left texture coordinate (s, t)
 * of (0, 0) and a lower right texture coordinate (1, 1).
 * </TD>
 * <TD> see glTexEnvfv()</TD>
 * </TR>
 * <TR><TD><CODE>GL_POLYGON_OFFSET_FILL</CODE></TD><TD>
 * If enabled, an offset is added to depth values of a polygon&rsquo;s fragments
 * before the depth comparison is performed (for PolygonMode GL_FILL).
 * </TD><TD> see glPolygonOffset()
 * </TD></TR>
 * <TR><TD><CODE>GL_POLYGON_OFFSET_LINE</CODE></TD><TD>
 * If enabled, an offset is added to depth values of a polygon&rsquo;s fragments
 * before the depth comparison is performed (for PolygonMode GL_LINE).
 * </TD><TD> see glPolygonOffset()
 * </TD></TR>
 * <TR><TD><CODE>GL_RESCALE_NORMAL</CODE></TD><TD>
 * If enabled, normal vectors are scaled by a factor derived from the modelview matrix.
 * </TD><TD> see glNormal3fv() and glNormalPointer()
 * </TD></TR>
 * <TR><TD><CODE>GL_SAMPLE_ALPHA_TO_COVERAGE</CODE></TD><TD>
 * If enabled, convert fragment alpha values to multisample coverage modification masks.
 * </TD><TD> see glSampleCoverage()
 * </TD></TR>
 * <TR><TD><CODE>GL_SAMPLE_ALPHA_TO_ONE</CODE></TD><TD>
 * If enabled, set fragment alpha to the maximum permissible value after computing multisample coverage modification masks.
 * </TD><TD> see glSampleCoverage()
 * </TD></TR>
 * <TR><TD><CODE>GL_SAMPLE_COVERAGE</CODE></TD><TD>
 * If enabled, apply a mask to modify fragment coverage during multisampling.
 * </TD><TD> see glSampleCoverage()
 * </TD></TR>
 * <TR><TD><CODE>GL_SCISSOR_TEST</CODE></TD><TD>
 * If enabled, discard fragments that are outside the scissor rectangle.
 * </TD><TD> see glScissor()
 * </TD></TR>
 * <TR><TD><CODE>GL_STENCIL_TEST</CODE></TD><TD>
 * If enabled, do stencil testing and update the stencil buffer.
 * </TD><TD> see glStencilFunc(), glStencilMask(), and glStencilOp()
 * </TD></TR>
 * <TR><TD><CODE>GL_STENCIL_TEST_TWO_SIDE_EXT</CODE></TD><TD>
 * If enabled with <CODE>GL_STENCIL_TEST</CODE>, do two-sided stencil
 * operations.
 * </TD><TD> see glActiveStencilFaceEXT(), glStencilFunc(), glStencilMask(), and glStencilOp()
 * </TD></TR>
 * <TR><TD><CODE>GL_TEXTURE_2D</CODE></TD><TD>
 * If enabled, two-dimensional texturing is performed for the active texture unit.
 * </TD><TD> see glTexImage2D(), glTexSubImage2D(), glCompressedTexImage2D(), glCopyTexImage2D(), and glCopyTexSubImage2D()
 * </TD></TR>
 * <TR><TD><CODE>GL_TEXTURE_3D</CODE></TD><TD>
 * If enabled, three-dimensional texturing is performed for the active texture unit.
 * </TD><TD> see glTexImage3D(), glTexSubImage3D(), glCompressedTexImage3D(), and glCopyTexSubImage3D()
 * </TD></TR>
 * <TR><TD><CODE>GL_TEXTURE_CUBE_MAP</CODE></TD><TD>
 * If enabled, one of a set six two-dimensional textures is applied, depending upon the direction vector from
 * the origin.
 * </TD><TD> see glTexImage2D(), glTexSubImage2D(), glCompressedTexImage2D(), glCopyTexImage2D(), glCopyTexSubImage2D(), and glTexParameterf()
 * </TD></TR>
 * <TR><TD><CODE>GL_TEXTURE_GEN_Q</CODE></TD><TD>
 * If enabled, the <I>q</I> texture coordinate is computed using the texture generation function defined with
glTexGenfv(). Otherwise, the current <I>q</I> texture coordinate is used.
 * </TD><TD> see glTexGenfv()
 * </TD></TR>
 * <TR><TD><CODE>GL_TEXTURE_GEN_R</CODE></TD><TD>
 * If enabled, the <I>r</I> texture coordinate is computed using the texture generation function defined with
glTexGenfv(). Otherwise, the current <I>r</I> texture coordinate is used.
 * </TD><TD> see glTexGenfv()
 * </TD></TR>
 * <TR><TD><CODE>GL_TEXTURE_GEN_S</CODE></TD><TD>
 * If enabled, the <I>s</I> texture coordinate is computed using the texture generation function defined with
glTexGenfv(). Otherwise, the current <I>s</I> texture coordinate is used.
 * </TD><TD> see glTexGenfv()
 * </TD></TR>
 * <TR><TD><CODE>GL_TEXTURE_GEN_T</CODE></TD><TD>
 * If enabled, the <I>t</I> texture coordinate is computed using the texture generation function defined with
glTexGenfv(). Otherwise, the current <I>t</I> texture coordinate is used.
 * </TD><TD> see glTexGenfv()
 * </TD></TR>
 * <TR><TD><CODE>GL_VERTEX_PROGRAM_POINT_SIZE_ARB</CODE></TD><TD>
 * If enabled, the point size is not determined by glPointSize(), but is determined
by the output of the vertex program, the value of the semantic <CODE>PSIZE</CODE>.
 * </TD><TD> see glPointSize()
 * </TD></TR>
 * <TR><TD><CODE>GL_VSYNC_SCE</CODE></TD><TD>
 * If enabled, swapping the frame buffer is synchronized with the monitor refresh, and overall frame rate is 
 limited by the refresh rate of the monitor. By default, vsync is disabled.

Note that if a device is created with libresc active (see psglCreateDeviceExtended()),
<code>GL_VSYNC_SCE</code> is always enabled, and cannot be disabled.
 * </TD><TD>&nbsp;
 * </TD></TR>
 * <TR><TD><CODE>GL_SKIP_FIRST_VSYNC_SCE</CODE></TD><TD>
 * If enabled, the first vblank signal is skipped and not used to trigger the flip of frame buffers. The flip then happens at the
 * second or later vblank signal.
 *
 * For example, if the VSync is 60Hz, the frame rate will be at most 30Hz. If the VSynch is 50Hz, the frame
 * rate will be at most 25Hz. 
 *
 * If the flip misses
 * the second vblank, it will use the third, making the frame 20Hz (in the case of 60Hz VSync).
 * </TD><TD>&nbsp;
 * </TD></TR>
 * </TABLE>
 *
 *  @param cap Specifies a symbolic constant indicating a GL capability.
 *
 * @par Errors
Use glGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>GL_INVALID_ENUM</CODE></TD>
<TD><I><c>cap</c></I> is not one of the values listed previously.</TD>
</TR>
</TABLE>
 *
 * @sa glActiveStencilFaceEXT, glActiveTexture, glAlphaFunc, glBlendFunc, glCompressedTexImage2D, glCopyTexImage2D,
 * glCullFace, glDepthFunc, glDepthRangef, glEnableClientState, glFogfv, glLightfv, glLightModelfv,
 * glLineWidth, glLogicOp, glMaterialfv, glNormal3fv, glPointSize, glPolygonOffset, glSampleCoverage,
 * glScissor, glStencilFunc, glStencilOp, glTexEnvfv, glTexImage2D, glTexGenfv
 *
 */
GLAPI void APIENTRY glEnable( GLenum cap )
{
    JS_APIENTRY_TIMER( glEnable );
    if ( _jsdpmState ) { _JS_DPM_API_ENTRY_1( glEnable, JSDPM_CALL_TYPE_STATE_GENERAL, , 1, cap, sizeof( GLenum ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM ); } // PSGL debug runtime

    PSGLcontext* LContext = _CurrentContext;

    JS_ASSERT( LContext );
    switch ( cap )
    {
        case GL_TEXTURE_3D:
#ifndef JS_NO_ERROR_CHECK
            if ( !LContext->CurrentImageUnit )
            {
                _jsSetError( GL_INVALID_OPERATION );
                return;
            }
#endif
            LContext->CurrentImageUnit->enable3D = GL_TRUE;
            _jsUpdateCurrentTextureCache( LContext->CurrentImageUnit );
            LContext->needValidate |= PSGL_VALIDATE_FFX_FRAGMENT_PROGRAM | PSGL_VALIDATE_TEXTURES_USED;
            break;

        case GL_TEXTURE_2D:
#ifndef JS_NO_ERROR_CHECK
            if ( !LContext->CurrentImageUnit )
            {
                _jsSetError( GL_INVALID_OPERATION );
                return;
            }
#endif
            LContext->CurrentImageUnit->enable2D = GL_TRUE;
            _jsUpdateCurrentTextureCache( LContext->CurrentImageUnit );
            LContext->needValidate |= PSGL_VALIDATE_FFX_FRAGMENT_PROGRAM | PSGL_VALIDATE_TEXTURES_USED;
            break;

        case GL_TEXTURE_CUBE_MAP:
#ifndef JS_NO_ERROR_CHECK
            if ( !LContext->CurrentImageUnit )
            {
                _jsSetError( GL_INVALID_OPERATION );
                return;
            }
#endif
            LContext->CurrentImageUnit->enableCube = GL_TRUE;
            _jsUpdateCurrentTextureCache( LContext->CurrentImageUnit );
            LContext->needValidate |= PSGL_VALIDATE_FFX_FRAGMENT_PROGRAM | PSGL_VALIDATE_TEXTURES_USED;
            break;

        case GL_TEXTURE_GEN_S:
#ifndef JS_NO_ERROR_CHECK
            if ( !LContext->CurrentCoordsUnit )
            {
                _jsSetError( GL_INVALID_OPERATION );
                return;
            }
#endif
            LContext->CurrentCoordsUnit->texGen[0].enabled = GL_TRUE;
            LContext->CurrentCoordsUnit->revalidate |= _JS_TEXTURE_UNIT_REVALIDATE_TEX_GEN;
            LContext->needValidate |= PSGL_VALIDATE_FFX_VERTEX_PROGRAM;
            break;

        case GL_TEXTURE_GEN_T:
#ifndef JS_NO_ERROR_CHECK
            if ( !LContext->CurrentCoordsUnit )
            {
                _jsSetError( GL_INVALID_OPERATION );
                return;
            }
#endif
            LContext->CurrentCoordsUnit->texGen[1].enabled = GL_TRUE;
            LContext->CurrentCoordsUnit->revalidate |= _JS_TEXTURE_UNIT_REVALIDATE_TEX_GEN;
            LContext->needValidate |= PSGL_VALIDATE_FFX_VERTEX_PROGRAM;
            break;

        case GL_TEXTURE_GEN_R:
#ifndef JS_NO_ERROR_CHECK
            if ( !LContext->CurrentCoordsUnit )
            {
                _jsSetError( GL_INVALID_OPERATION );
                return;
            }
#endif
            LContext->CurrentCoordsUnit->texGen[2].enabled = GL_TRUE;
            LContext->CurrentCoordsUnit->revalidate |= _JS_TEXTURE_UNIT_REVALIDATE_TEX_GEN;
            LContext->needValidate |= PSGL_VALIDATE_FFX_VERTEX_PROGRAM;
            break;

        case GL_TEXTURE_GEN_Q:
#ifndef JS_NO_ERROR_CHECK
            if ( !LContext->CurrentCoordsUnit )
            {
                _jsSetError( GL_INVALID_OPERATION );
                return;
            }
#endif
            LContext->CurrentCoordsUnit->texGen[3].enabled = GL_TRUE;
            LContext->CurrentCoordsUnit->revalidate |= _JS_TEXTURE_UNIT_REVALIDATE_TEX_GEN;
            LContext->needValidate |= PSGL_VALIDATE_FFX_VERTEX_PROGRAM;
            break;

        case GL_ALPHA_TEST:
            LContext->AlphaTest = GL_TRUE;
            LContext->needValidate |= PSGL_VALIDATE_ALPHA_TEST;
            break;

        case GL_DEPTH_TEST:
            LContext->DepthTest = GL_TRUE;
            LContext->needValidate |= PSGL_VALIDATE_DEPTH_TEST;
            break;

        case GL_SHADER_SRGB_REMAP_SCE:
            LContext->ShaderSRGBRemap = GL_TRUE;
            LContext->needValidate |= PSGL_VALIDATE_SHADER_SRGB_REMAP;
            break;

        case GL_STENCIL_TEST:
            LContext->StencilTest = GL_TRUE;
            LContext->needValidate |= PSGL_VALIDATE_STENCIL_TEST;
            break;

        case GL_STENCIL_TEST_TWO_SIDE_EXT:
            LContext->StencilTestTwoSide = GL_TRUE;
            LContext->needValidate |= PSGL_VALIDATE_STENCIL_TEST;
            break;

        case GL_SCISSOR_TEST:
            LContext->ScissorTest = GL_TRUE;
            LContext->needValidate |= PSGL_VALIDATE_SCISSOR_BOX;
            break;

        case GL_COLOR_MATERIAL:
            LContext->ColorMaterial = GL_TRUE;
            LContext->needValidate |= PSGL_VALIDATE_FFX_VERTEX_PROGRAM;
            break;

        case GL_CULL_FACE:
            LContext->FaceCulling = GL_TRUE;
            LContext->needValidate |= PSGL_VALIDATE_FACE_CULL;
            break;

        case GL_BLEND:
            LContext->Blending = GL_TRUE;
            LContext->BlendingMrt[0] = GL_TRUE;
            LContext->BlendingMrt[1] = GL_TRUE;
            LContext->BlendingMrt[2] = GL_TRUE;
            LContext->needValidate |= PSGL_VALIDATE_BLENDING;
            break;

        case GL_BLEND_MRT0_SCE:
            LContext->Blending = GL_TRUE;
            LContext->needValidate |= PSGL_VALIDATE_BLENDING;
            break;

        case GL_BLEND_MRT1_SCE:
            LContext->BlendingMrt[0] = GL_TRUE;
            LContext->needValidate |= PSGL_VALIDATE_BLENDING;
            break;

        case GL_BLEND_MRT2_SCE:
            LContext->BlendingMrt[1] = GL_TRUE;
            LContext->needValidate |= PSGL_VALIDATE_BLENDING;
            break;

        case GL_BLEND_MRT3_SCE:
            LContext->BlendingMrt[2] = GL_TRUE;
            LContext->needValidate |= PSGL_VALIDATE_BLENDING;
            break;

        case GL_FOG:
            LContext->Fog = GL_TRUE;
            LContext->needValidate |= PSGL_VALIDATE_FFX_FRAGMENT_PROGRAM;
            break;

        case GL_POINT_SMOOTH:
            LContext->PointSmooth = GL_TRUE;
            LContext->needValidate |= PSGL_VALIDATE_POINT_RASTER;
            break;

        case GL_LINE_SMOOTH:
            LContext->LineSmooth = GL_TRUE;
            LContext->needValidate |= PSGL_VALIDATE_LINE_RASTER;
            break;

        case GL_COLOR_LOGIC_OP:
            LContext->ColorLogicOp = GL_TRUE;
            LContext->needValidate |= PSGL_VALIDATE_LOGIC_OP;
            break;

        case GL_DITHER:
            LContext->Dithering = GL_TRUE;
            break;

        case GL_MULTISAMPLE:
            LContext->Multisample = GL_TRUE;
            LContext->needValidate |= PSGL_VALIDATE_MULTISAMPLING;
            break;

        case GL_REDUCE_DST_COLOR_SCE:
            LContext->ReduceDstColor = GL_TRUE;
            LContext->needValidate |= PSGL_VALIDATE_MULTISAMPLING;
            break;

        case GL_SAMPLE_COVERAGE:
            LContext->SampleCoverage = GL_TRUE;
            LContext->needValidate |= PSGL_VALIDATE_MULTISAMPLING;
            break;

        case GL_SAMPLE_ALPHA_TO_COVERAGE:
            LContext->SampleAlphaToCoverage = GL_TRUE;
            LContext->needValidate |= PSGL_VALIDATE_MULTISAMPLING;
            break;

        case GL_SAMPLE_ALPHA_TO_ONE:
            LContext->SampleAlphaToOne = GL_TRUE;
            LContext->needValidate |= PSGL_VALIDATE_MULTISAMPLING;
            break;

        case GL_POLYGON_OFFSET_FILL:
            LContext->PolygonOffsetFill = GL_TRUE;
            LContext->needValidate |= PSGL_VALIDATE_POLYGON_OFFSET;
            break;

        case GL_POLYGON_OFFSET_LINE:
            LContext->PolygonOffsetLine = GL_TRUE;
            LContext->needValidate |= PSGL_VALIDATE_POLYGON_OFFSET;
            break;

        case GL_RESCALE_NORMAL:
            LContext->RescaleNormals = GL_TRUE;
            LContext->needValidate |= PSGL_VALIDATE_FFX_VERTEX_PROGRAM;
            break;

        case GL_NORMALIZE:
            LContext->Normalize = GL_TRUE;
            LContext->needValidate |= PSGL_VALIDATE_FFX_VERTEX_PROGRAM;
            break;

        case GL_LIGHTING:
            LContext->Lighting = GL_TRUE;
            LContext->needValidate |= PSGL_VALIDATE_FFX_VERTEX_PROGRAM;
            break;

        case GL_LIGHT0:
        case GL_LIGHT1:
        case GL_LIGHT2:
        case GL_LIGHT3:
        case GL_LIGHT4:
        case GL_LIGHT5:
        case GL_LIGHT6:
        case GL_LIGHT7:
            LContext->Lights[cap-GL_LIGHT0].Active = GL_TRUE;
            LContext->needValidate |= PSGL_VALIDATE_FFX_VERTEX_PROGRAM;
            break;

        case GL_CLIP_PLANE0:
        case GL_CLIP_PLANE1:
        case GL_CLIP_PLANE2:
        case GL_CLIP_PLANE3:
        case GL_CLIP_PLANE4:
        case GL_CLIP_PLANE5:
            LContext->ClipPlane[cap - GL_CLIP_PLANE0].Active = GL_TRUE;
            LContext->needValidate |= PSGL_VALIDATE_CLIP_PLANES;
            break;

        case GL_POINT_SPRITE_OES:
            LContext->PointSprite = GL_TRUE;
            LContext->needValidate |= PSGL_VALIDATE_POINT_SPRITE;
            break;

        case GL_VERTEX_PROGRAM_POINT_SIZE_ARB:
            LContext->VertexProgramPointSize = GL_TRUE;
            LContext->needValidate |= PSGL_VALIDATE_POINT_SPRITE;
            break;

        case GL_VSYNC_SCE:
            LContext->VSync = GL_TRUE;
            break;

        case GL_SKIP_FIRST_VSYNC_SCE:
            LContext->SkipFirstVSync = GL_TRUE;
            break;

        case GL_DEPTH_CLAMP_NV:
            LContext->DepthClamp = GL_TRUE;
            LContext->needValidate |= PSGL_VALIDATE_VIEWPORT;
            break;

		case GL_FRAGMENT_PROGRAM_CONTROL_CONTROLTXP_SCE:
			LContext->AllowTXPDemotion = GL_TRUE;
			if ( LContext->BoundFragmentProgram )
				LContext->needValidate |= PSGL_VALIDATE_FRAGMENT_PROGRAM;
			else
				LContext->needValidate |= PSGL_VALIDATE_FFX_FRAGMENT_PROGRAM; 
			break; 

        default:
            _jsSetError( GL_INVALID_ENUM );
            return;
    }
}

/**
 *
 *  @short Disable server-side GL capabilities
 *
 *  glEnable() and glDisable() enable and disable various capabilities.
 *  For more information, see the related function glEnable()
 *
 *  @param cap Specifies a symbolic constant indicating a GL capability.
 *
 */
GLAPI void APIENTRY glDisable( GLenum cap )
{
    JS_APIENTRY_TIMER( glDisable );
    if ( _jsdpmState ) { _JS_DPM_API_ENTRY_1( glDisable, JSDPM_CALL_TYPE_STATE_GENERAL, , 1, cap, sizeof( GLenum ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM ); }

    PSGLcontext* LContext = _CurrentContext;

    JS_ASSERT( LContext );
    switch ( cap )
    {
        case GL_TEXTURE_3D:
#ifndef JS_NO_ERROR_CHECK
            if ( !LContext->CurrentImageUnit )
            {
                _jsSetError( GL_INVALID_OPERATION );
                return;
            }
#endif
            LContext->CurrentImageUnit->enable3D = GL_FALSE;
            _jsUpdateCurrentTextureCache( LContext->CurrentImageUnit );
            LContext->needValidate |= PSGL_VALIDATE_FFX_FRAGMENT_PROGRAM | PSGL_VALIDATE_TEXTURES_USED;
            break;

        case GL_TEXTURE_2D:
#ifndef JS_NO_ERROR_CHECK
            if ( !LContext->CurrentImageUnit )
            {
                _jsSetError( GL_INVALID_OPERATION );
                return;
            }
#endif
            LContext->CurrentImageUnit->enable2D = GL_FALSE;
            _jsUpdateCurrentTextureCache( LContext->CurrentImageUnit );
            LContext->needValidate |= PSGL_VALIDATE_FFX_FRAGMENT_PROGRAM | PSGL_VALIDATE_TEXTURES_USED;
            break;

        case GL_TEXTURE_CUBE_MAP:
#ifndef JS_NO_ERROR_CHECK
            if ( !LContext->CurrentImageUnit )
            {
                _jsSetError( GL_INVALID_OPERATION );
                return;
            }
#endif
            LContext->CurrentImageUnit->enableCube = GL_FALSE;
            _jsUpdateCurrentTextureCache( LContext->CurrentImageUnit );
            LContext->needValidate |= PSGL_VALIDATE_FFX_FRAGMENT_PROGRAM | PSGL_VALIDATE_TEXTURES_USED;
            break;

        case GL_TEXTURE_GEN_S:
#ifndef JS_NO_ERROR_CHECK
            if ( !LContext->CurrentCoordsUnit )
            {
                _jsSetError( GL_INVALID_OPERATION );
                return;
            }
#endif
            LContext->CurrentCoordsUnit->texGen[0].enabled = GL_FALSE;
            LContext->CurrentCoordsUnit->revalidate |= _JS_TEXTURE_UNIT_REVALIDATE_TEX_GEN;
            LContext->needValidate |= PSGL_VALIDATE_FFX_VERTEX_PROGRAM;

#if __JS_DEBUG_TEXGEN == 1
            _jsLog( 0, "Disabling texture gen S" );
#endif
            break;

        case GL_TEXTURE_GEN_T:
#ifndef JS_NO_ERROR_CHECK
            if ( !LContext->CurrentCoordsUnit )
            {
                _jsSetError( GL_INVALID_OPERATION );
                return;
            }
#endif
            LContext->CurrentCoordsUnit->texGen[1].enabled = GL_FALSE;
            LContext->CurrentCoordsUnit->revalidate |= _JS_TEXTURE_UNIT_REVALIDATE_TEX_GEN;
            LContext->needValidate |= PSGL_VALIDATE_FFX_VERTEX_PROGRAM;

#if __JS_DEBUG_TEXGEN == 1
            _jsLog( 0, "Disabling texture gen T" );
#endif
            break;

        case GL_TEXTURE_GEN_R:
#ifndef JS_NO_ERROR_CHECK
            if ( !LContext->CurrentCoordsUnit )
            {
                _jsSetError( GL_INVALID_OPERATION );
                return;
            }
#endif
            LContext->CurrentCoordsUnit->texGen[2].enabled = GL_FALSE;
            LContext->CurrentCoordsUnit->revalidate |= _JS_TEXTURE_UNIT_REVALIDATE_TEX_GEN;
            LContext->needValidate |= PSGL_VALIDATE_FFX_VERTEX_PROGRAM;
            break;

        case GL_TEXTURE_GEN_Q:
#ifndef JS_NO_ERROR_CHECK
            if ( !LContext->CurrentCoordsUnit )
            {
                _jsSetError( GL_INVALID_OPERATION );
                return;
            }
#endif
            LContext->CurrentCoordsUnit->texGen[3].enabled = GL_FALSE;
            LContext->CurrentCoordsUnit->revalidate |= _JS_TEXTURE_UNIT_REVALIDATE_TEX_GEN;
            LContext->needValidate |= PSGL_VALIDATE_FFX_VERTEX_PROGRAM;
            break;

        case GL_ALPHA_TEST:
            LContext->AlphaTest = GL_FALSE;
            LContext->needValidate |= PSGL_VALIDATE_ALPHA_TEST;
            break;

        case GL_DEPTH_TEST:
            LContext->DepthTest = GL_FALSE;
            LContext->needValidate |= PSGL_VALIDATE_DEPTH_TEST;
            break;

        case GL_SHADER_SRGB_REMAP_SCE:
            LContext->ShaderSRGBRemap = GL_FALSE;
            LContext->needValidate |= PSGL_VALIDATE_SHADER_SRGB_REMAP;
            break;

        case GL_STENCIL_TEST:
            LContext->StencilTest = GL_FALSE;
            LContext->needValidate |= PSGL_VALIDATE_STENCIL_TEST;
            break;

        case GL_STENCIL_TEST_TWO_SIDE_EXT:
            LContext->StencilTestTwoSide = GL_FALSE;
            LContext->needValidate |= PSGL_VALIDATE_STENCIL_TEST;
            break;

        case GL_SCISSOR_TEST:
            LContext->ScissorTest = GL_FALSE;
            LContext->needValidate |= PSGL_VALIDATE_SCISSOR_BOX;
            break;

        case GL_COLOR_MATERIAL:
            LContext->ColorMaterial = GL_FALSE;
            LContext->needValidate |= PSGL_VALIDATE_FFX_VERTEX_PROGRAM;
            break;

        case GL_CULL_FACE:
            LContext->FaceCulling = GL_FALSE;
            LContext->needValidate |= PSGL_VALIDATE_FACE_CULL;
            break;

        case GL_BLEND:
            LContext->Blending = GL_FALSE;
            LContext->BlendingMrt[0] = GL_FALSE;
            LContext->BlendingMrt[1] = GL_FALSE;
            LContext->BlendingMrt[2] = GL_FALSE;
            LContext->needValidate |= PSGL_VALIDATE_BLENDING;
            break;

        case GL_BLEND_MRT0_SCE:
            LContext->Blending = GL_FALSE;
            LContext->needValidate |= PSGL_VALIDATE_BLENDING;
            break;

        case GL_BLEND_MRT1_SCE:
            LContext->BlendingMrt[0] = GL_FALSE;
            LContext->needValidate |= PSGL_VALIDATE_BLENDING;
            break;

        case GL_BLEND_MRT2_SCE:
            LContext->BlendingMrt[1] = GL_FALSE;
            LContext->needValidate |= PSGL_VALIDATE_BLENDING;
            break;

        case GL_BLEND_MRT3_SCE:
            LContext->BlendingMrt[2] = GL_FALSE;
            LContext->needValidate |= PSGL_VALIDATE_BLENDING;
            break;

        case GL_FOG:
            LContext->Fog = GL_FALSE;
            LContext->needValidate |= PSGL_VALIDATE_FFX_FRAGMENT_PROGRAM;
            break;

        case GL_POINT_SMOOTH:
            LContext->PointSmooth = GL_FALSE;
            LContext->needValidate |= PSGL_VALIDATE_POINT_RASTER;
            break;

        case GL_LINE_SMOOTH:
            LContext->LineSmooth = GL_FALSE;
            LContext->needValidate |= PSGL_VALIDATE_LINE_RASTER;
            break;

        case GL_COLOR_LOGIC_OP:
            LContext->ColorLogicOp = GL_FALSE;
            LContext->needValidate |= PSGL_VALIDATE_LOGIC_OP;
            break;

        case GL_DITHER:
            LContext->Dithering = GL_FALSE;
            break;

        case GL_MULTISAMPLE:
            LContext->Multisample = GL_FALSE;
            LContext->needValidate |= PSGL_VALIDATE_MULTISAMPLING;
            break;

        case GL_REDUCE_DST_COLOR_SCE:
            LContext->ReduceDstColor = GL_FALSE;
            LContext->needValidate |= PSGL_VALIDATE_MULTISAMPLING;
            break;

        case GL_SAMPLE_COVERAGE:
            LContext->SampleCoverage = GL_FALSE;
            LContext->needValidate |= PSGL_VALIDATE_MULTISAMPLING;
            break;

        case GL_SAMPLE_ALPHA_TO_COVERAGE:
            LContext->SampleAlphaToCoverage = GL_FALSE;
            LContext->needValidate |= PSGL_VALIDATE_MULTISAMPLING;
            break;

        case GL_SAMPLE_ALPHA_TO_ONE:
            LContext->SampleAlphaToOne = GL_FALSE;
            LContext->needValidate |= PSGL_VALIDATE_MULTISAMPLING;
            break;

        case GL_POLYGON_OFFSET_FILL:
            LContext->PolygonOffsetFill = GL_FALSE;
            LContext->needValidate |= PSGL_VALIDATE_POLYGON_OFFSET;
            break;

        case GL_POLYGON_OFFSET_LINE:
            LContext->PolygonOffsetLine = GL_FALSE;
            LContext->needValidate |= PSGL_VALIDATE_POLYGON_OFFSET;
            break;

        case GL_RESCALE_NORMAL:
            LContext->RescaleNormals = GL_FALSE;
            LContext->needValidate |= PSGL_VALIDATE_FFX_VERTEX_PROGRAM;
            break;

        case GL_NORMALIZE:
            LContext->Normalize = GL_FALSE;
            LContext->needValidate |= PSGL_VALIDATE_FFX_VERTEX_PROGRAM;
            break;

        case GL_LIGHTING:
            LContext->Lighting = GL_FALSE;
            LContext->needValidate |= PSGL_VALIDATE_FFX_VERTEX_PROGRAM;
            break;

        case GL_LIGHT0:
        case GL_LIGHT1:
        case GL_LIGHT2:
        case GL_LIGHT3:
        case GL_LIGHT4:
        case GL_LIGHT5:
        case GL_LIGHT6:
        case GL_LIGHT7:
            LContext->Lights[cap-GL_LIGHT0].Active = GL_FALSE;
            LContext->needValidate |= PSGL_VALIDATE_FFX_VERTEX_PROGRAM;
            break;

        case GL_CLIP_PLANE0:
        case GL_CLIP_PLANE1:
        case GL_CLIP_PLANE2:
        case GL_CLIP_PLANE3:
        case GL_CLIP_PLANE4:
        case GL_CLIP_PLANE5:
            LContext->ClipPlane[cap - GL_CLIP_PLANE0].Active = GL_FALSE;
            LContext->needValidate |= PSGL_VALIDATE_CLIP_PLANES;
            break;

        case GL_POINT_SPRITE_OES:
            LContext->PointSprite = GL_FALSE;
            LContext->needValidate |= PSGL_VALIDATE_POINT_SPRITE;
            break;

        case GL_VERTEX_PROGRAM_POINT_SIZE_ARB:
            LContext->VertexProgramPointSize = GL_FALSE;
            LContext->needValidate |= PSGL_VALIDATE_POINT_SPRITE;
            break;

        case GL_VSYNC_SCE:
            LContext->VSync = GL_FALSE;
            break;

        case GL_SKIP_FIRST_VSYNC_SCE:
            LContext->SkipFirstVSync = GL_FALSE;
            break;

        case GL_DEPTH_CLAMP_NV:
            LContext->DepthClamp = GL_FALSE;
            LContext->needValidate |= PSGL_VALIDATE_VIEWPORT;
            break;

		case GL_FRAGMENT_PROGRAM_CONTROL_CONTROLTXP_SCE:
			LContext->AllowTXPDemotion = GL_FALSE;
			if ( LContext->BoundFragmentProgram )
				LContext->needValidate |= PSGL_VALIDATE_FRAGMENT_PROGRAM;
			else
				LContext->needValidate |= PSGL_VALIDATE_FFX_FRAGMENT_PROGRAM; 
			break; 

        default:
            _jsSetError( GL_INVALID_ENUM );
            return;
    }
}

/** @}  OpenGL  */

/////////////////////////////////////////////////////////////////////////////
// client-side enable/disable

/**
 * @addtogroup OpenGL
 *
 * @{
 */
/**
 *  @short Enable client-side capability
 *
 *  glEnableClientState() and glDisableClientState() enable or disable individual client-side capabilities.
 *  By default, all client-side capabilities are disabled. Both glEnableClientState() and
 *  glDisableClientState() take a single argument, <I><c>array</c></I>, which can assume one of the following values:
 *
 * <TABLE rules=all>
 * <TR>
 * <TD><CODE>GL_COLOR_ARRAY</CODE></TD>
 * <TD>If enabled, the color array is enabled for writing and used during rendering when glDrawArrays(),
 * glDrawRangeElements(), or glDrawElements() is called.</TD>
 * <TD> see glColorPointer()</TD>
 * </TR>
 * <TR>
 * <TD><CODE>GL_NORMAL_ARRAY</CODE></TD>
 * <TD>If enabled, the normal array is enabled for writing and used during rendering when glDrawArrays(),
 * glDrawRangeElements(), or glDrawElements() is called.</TD>
 * <TD> see glNormalPointer()</TD>
 * </TR>
 * <TR>
 * <TD><CODE>GL_TEXTURE_COORD_ARRAY</CODE></TD>
 * <TD>If enabled, the texture coordinate array is enabled for writing and used during rendering when
 *  glDrawArrays(), glDrawRangeElements(), or glDrawElements() is called.</TD>
 * <TD> see glTexCoordPointer()</TD>
 * </TR>
 * <TR>
 * <TD><CODE>GL_VERTEX_ARRAY</CODE></TD>
 * <TD>If enabled, the vertex array is enabled for writing and used during rendering when glDrawArrays(),
 * glDrawRangeElements(), or glDrawElements() is called.</TD>
 * <TD> see glVertexPointer()</TD>
 * </TR>
 * <TR>
 * <TD><CODE>GL_PRIMITIVE_RESTART_NV</CODE></TD>
 * <TD>If enabled, a specified index is used to restart primitives within a vertex array.
 * </TD>
 * <TD> see glPrimitiveRestartIndexNV()</TD>
 * </TR>
 * </TABLE>
 *
 *  @param array Specifies the capability to enable or disable. Symbolic constants
 * <CODE>GL_COLOR_ARRAY</CODE>, <CODE>GL_NORMAL_ARRAY</CODE>, <CODE>GL_TEXTURE_COORD_ARRAY</CODE>,
 * <CODE>GL_VERTEX_ARRAY</CODE>, and <CODE>GL_PRIMITIVE_RESTART_NV</CODE> are accepted.
 *
 *  @note
 *  Enabling and disabling <CODE>GL_TEXTURE_COORD_ARRAY</CODE> affects the active client texture unit.
 *  The active client texture unit is controlled with glClientActiveTexture().
 *
 *  @par Errors
Use glGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>GL_INVALID_ENUM</CODE></TD>
<TD><I><c>array</c></I> is not an accepted value.</TD>
</TR>
</TABLE>
 *
 *  @sa glClientActiveTexture, glColorPointer, glDrawArrays, glDrawElements, glDrawRangeElements, glEnable,
 *  glNormalPointer, glTexCoordPointer, glVertexPointer, glPrimitiveRestartIndexNV
 *
 */

GLAPI void APIENTRY glEnableClientState( GLenum array )
{
    JS_APIENTRY_TIMER( glEnableClientState );
    if ( _jsdpmState ) { _JS_DPM_API_ENTRY_1( glEnableClientState, JSDPM_CALL_TYPE_STATE_GENERAL, , 1, array, sizeof( GLenum ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM ); }

    PSGLcontext* LContext = _CurrentContext;

    JS_ASSERT( LContext );
    switch ( array )
    {
        case GL_VERTEX_ARRAY:
            _jsEnableVertexAttribArrayNV( _JS_ATTRIB_POSITION_INDEX );
            break;
        case GL_COLOR_ARRAY:
            _jsEnableVertexAttribArrayNV( _JS_ATTRIB_PRIMARY_COLOR_INDEX );
            break;
        case GL_NORMAL_ARRAY:
            _jsEnableVertexAttribArrayNV( _JS_ATTRIB_NORMAL_INDEX );
            break;
        case GL_TEXTURE_COORD_ARRAY:
            _jsEnableVertexAttribArrayNV( _JS_ATTRIB_TEX_COORD0_INDEX + LContext->CS_ActiveTexture );
            break;
        case GL_PRIMITIVE_RESTART_NV:
            LContext->attribs->PrimitiveRestart = GL_TRUE;
            LContext->needValidate |= PSGL_VALIDATE_PRIMITIVE_RESTART;
            if ( LContext->attribSetName ) _jsAttribSetMarkDirty();
            break;
        default:
            _jsSetError( GL_INVALID_ENUM );
            return;
    }
}

/**
 *  @short Disable client-side capability
 *
 *  glEnableClientState() and glDisableClientState() enable or disable individual client-side capabilities. By default, all client-side capabilities are disabled.
 *
 *  For more information, see the related function glEnableClientState()
 *
 *  @param array Specifies the capability to disable. Symbolic constants
 * <CODE>GL_COLOR_ARRAY</CODE>, <CODE>GL_NORMAL_ARRAY</CODE>, <CODE>GL_TEXTURE_COORD_ARRAY</CODE>,
 * <CODE>GL_VERTEX_ARRAY</CODE>, and <CODE>GL_PRIMITIVE_RESTART_NV</CODE> are accepted.
 *
 */

GLAPI void APIENTRY glDisableClientState( GLenum array )
{
    JS_APIENTRY_TIMER( glDisableClientState );
    if ( _jsdpmState ) { _JS_DPM_API_ENTRY_1( glDisableClientState, JSDPM_CALL_TYPE_STATE_GENERAL, , 1, array, sizeof( GLenum ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM ); }

    PSGLcontext* LContext = _CurrentContext;

    JS_ASSERT( LContext );

    switch ( array )
    {
        case GL_VERTEX_ARRAY:
            _jsDisableVertexAttribArrayNV( _JS_ATTRIB_POSITION_INDEX );
            break;
        case GL_COLOR_ARRAY:
            _jsDisableVertexAttribArrayNV( _JS_ATTRIB_PRIMARY_COLOR_INDEX );
            break;
        case GL_NORMAL_ARRAY:
            _jsDisableVertexAttribArrayNV( _JS_ATTRIB_NORMAL_INDEX );
            break;
        case GL_TEXTURE_COORD_ARRAY:
            _jsDisableVertexAttribArrayNV( _JS_ATTRIB_TEX_COORD0_INDEX + LContext->CS_ActiveTexture );
            break;
        case GL_PRIMITIVE_RESTART_NV:
            LContext->attribs->PrimitiveRestart = GL_FALSE;
            LContext->needValidate |= PSGL_VALIDATE_PRIMITIVE_RESTART;
            if ( LContext->attribSetName ) _jsAttribSetMarkDirty();
            break;
        default:
            _jsSetError( GL_INVALID_ENUM );
            return;
    }
}

/** @}  OpenGL */

/////////////////////////////////////////////////////////////////////////////
// glFlush, glFinish

/**
 * @addtogroup OpenGL
 *
 *  @{
 */

/**
 *  @short Force execution of GL commands in finite time
 *
 *  Different GL implementations buffer commands in several different locations, including network buffers and the graphics accelerator itself. glFlush() empties all of these buffers, causing all issued commands to be executed as quickly as they are accepted by the actual rendering engine. Though this execution may not be completed in any particular time period, it does complete in finite time.
 *
 *  Because any GL program might be executed over a network, or on an accelerator that buffers commands, all programs should call glFlush() whenever they count on having all of their previously issued commands completed. For example, call glFlush() before waiting for user input that depends on the generated image.
 *
 *  @note
 *  glFlush() can return at any time. It does not wait until the execution of all previously issued GL commands is complete.
 *
 *  @sa glFinish
 */
GLAPI void APIENTRY glFlush()
{
    JS_APIENTRY_TIMER( glFlush );
    if ( _jsdpmState ) { _JS_DPM_API_ENTRY_0( glFlush, JSDPM_CALL_TYPE_SYNC, ); }

    PSGLcontext* LContext = _CurrentContext;
    JS_ASSERT( LContext );
    if ( JS_UNLIKELY( LContext->needValidate ) ) _jsValidateStates( PSGL_VALIDATE_ALL );
    _jsPlatformRasterFlush();
}

/**
 *  @short Block until all GL execution is complete
 *
 *  glFinish() does not return until the effects of all previously called GL commands are complete. Such effects include all changes to GL state, all changes to connection state, and all changes to the frame buffer contents.
 *
 *  @note
 *  glFinish() requires a round trip to the server.
 *
 *  @sa glFlush
 */
GLAPI void APIENTRY glFinish()
{
    JS_APIENTRY_TIMER( glFinish );
    if ( _jsdpmState ) { _JS_DPM_API_ENTRY_0( glFinish, JSDPM_CALL_TYPE_SYNC, ); } // this will generate a spurious trace of glFlush, too bad

    glFlush();
    _jsPlatformRasterFinish();
}

/** @} OpenGL */

/////////////////////////////////////////////////////////////////////////////
// Misc PSGL utility functions

/**
 * @addtogroup PSGL
 *
 *  @{
 */

/**
 *  @short Returns system time with spoofing in case of performance monitoring
 *
 *  psglGetSystemTime() returns the operating-system clock which may be spoofed during performance monitoring.
 *
 *  @return Returns a 64-bit unsigned integer time stamp
 */
PSGL_EXPORT PSGLuint64 psglGetSystemTime()
{
#ifdef JS_DPM
    return _jsdpmSystemTimeAtCaptureStart > 0 ? _jsdpmSystemTimeAtCaptureStart : _jsGetSystemTime();
#else
    return _jsGetSystemTime();
#endif
}

/**
 *  @short Returns time of the actual frame flip.
 *
 *  psglGetLastFlipTime() returns the timebase value of the last flip in microseconds.
 *
 *  @return Returns a 64-bit unsigned integer time stamp
 *
 *  @sa cellGcmGetLastFlipTime (see <i>libgcm Reference</i>)
 */
PSGL_EXPORT PSGLuint64 psglGetLastFlipTime()
{
    return _jsGetLastFlipTime();
}



/** @} PSGL */

/////////////////////////////////////////////////////////////////////////////

/**
 * @addtogroup OpenGL
 *
 *  @{
 */

/**
 * @short Return a string describing the current GL connection
 *
 * glGetString() returns a pointer to a static string describing some aspect of the current GL connection.
 * <I><c>name</c></I> can be one of the following:

 * <TABLE rules=all>
 * <TR>
 * <TD><CODE>GL_VENDOR</CODE></TD>
 * <TD>Returns the company responsible for this GL implementation. This name does not change from release to release.
 * </TD>
 * </TR>
 * <TR>
 * <TD><CODE>GL_RENDERER</CODE></TD>
 * <TD>Returns the name of the renderer. This name is typically specific to a particular configuration of a hardware platform.
 * It does not change from release to release.</TD>
 * </TR>
 * <TR>
 * <TD><CODE>GL_VERSION</CODE></TD>
 * <TD>Returns a version or release number.
 * </TD>
 * </TR>
 * <TR>
 * <TD><CODE>GL_EXTENSIONS</CODE></TD>
 * <TD>Returns a space-separated list of supported extensions to GL.
 * </TD>
 * </TR>
 * </TABLE>
 *
Because the GL does not include queries for the performance characteristics of an implementation,
some applications are written to recognize known platforms and modify their GL usage based on known
performance characteristics of these platforms. Strings <CODE>GL_VENDOR</CODE> and <CODE>GL_RENDERER</CODE>
together uniquely specify a platform. They do not change from release to release and should be used
by platform-recognition algorithms.
 *
Some applications want to make use of features that are not part of the standard GL. These features
may be implemented as extensions to the standard GL. The <CODE>GL_EXTENSIONS</CODE> string is a
space-separated list of supported GL extensions. (Extension names never contain a space character.)
 *
The <CODE>GL_VERSION</CODE> string begins with a version number. The version number uses one of these forms:

<I>major_number.minor_number</I>

<I>major_number.minor_number.release_number</I>
 *
Vendor-specific information may follow the version number. It depends on the implementation, but a space always separates the version number and the vendor-specific information.
 *
All strings are null-terminated.
 *
 * @param name Specifies a symbolic constant, one of <CODE>GL_VENDOR</CODE>, <CODE>GL_RENDERER</CODE>,
 * <CODE>GL_VERSION</CODE>, or <CODE>GL_EXTENSIONS</CODE>.
 *
 * @return Returns a string describing some aspect of the current GL connection, such as vendor name,
 * version number, or list of extensions.
 *
 * @note If an error is generated, glGetString() returns <CODE>NULL</CODE>.
 * The client and server may support different versions or extensions.
 * glGetString() always returns a compatible version number or list of extensions.
 * The release number always describes the server.
 *
 * @par Errors
Use glGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>GL_INVALID_ENUM</CODE></TD>
<TD><I><c>name</c></I> is not an accepted value.</TD>
</TR>
</TABLE>
 */

GLAPI const GLubyte* APIENTRY glGetString( GLenum name )
{
    JS_APIENTRY_TIMER( glGetString );
    if ( _jsdpmState ) { _JS_DPM_API_ENTRY_1( glGetString, JSDPM_CALL_TYPE_GET, NULL, 1, name, sizeof( GLenum ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM ); }

    switch ( name )
    {
        case GL_VENDOR:
            return(( GLubyte* )_jsVendorString );
        case GL_RENDERER:
            return(( GLubyte* )_jsRendererString );
        case GL_VERSION:
            return(( GLubyte* )_jsVersionNumber );
        case GL_EXTENSIONS:
            return(( GLubyte* )_jsExtensionsString );
        default:
        {
            _jsSetError( GL_INVALID_ENUM );
            return(( GLubyte* )NULL );
        }
    }
}


/** @} OpenGL */

/////////////////////////////////////////////////////////////////////////////
// PSGL library initialization/exit

static int _jsInitCompleted = 0;
int _getJsInitCompleted(){ return _jsInitCompleted; } // accessor

#ifdef JS_PERFORMANCE_INFO
#define COUNTER(Sem,Name,Text) PerformanceCounter_t Name;
COUNTER_LIST
#undef COUNTER
#endif

#ifdef JS_APIENTRY_TIMERS
jsUint64 _jsEntryTimer_Overall = 0;
#define APIENTRY_TIMER(name) APIEntryTimer_t JS_APIENTRY_TIMER_NAME(name);
APIENTRY_TIMER_LIST
#undef APIENTRY_TIMER
#endif

/**
 * @addtogroup PSGL
 *
 * @{
 */

/**
 * @short Initializes the PSGL library and configures use of resources
 *
 * psglInit() initializes the PSGL library.  psglInit() must be called before psglCreateContext(),
 * psglCreateDeviceAuto(), and psglCreateDeviceExtended().  psglInit() initializes SPUs for the PSGL library to use. 
 * Do not call psglInit() more than once.
 *
 * Calling <CODE>psglInit(NULL)</CODE> is a legal, simple way to initialize the library.  <CODE>psglInit(NULL)</CODE> sets all
 * the options to their default values.
 *
 * <I><c>options</c></I> is a pointer to a structure that is described in this table:

 * <TABLE rules=all>
 * <TR>
 * <TD><CODE>enable</CODE></TD>
 * <TD>A bitfield with option flags to enable or disable the other members.
 * Legal enumerants for <CODE>enable</CODE> are
 * <CODE>PSGL_INIT_MAX_SPUS</CODE>, <CODE>PSGL_INIT_INITIALIZE_SPUS</CODE>,
 * <CODE>PSGL_INIT_PERSISTENT_MEMORY_SIZE</CODE>, <CODE>PSGL_INIT_TRANSIENT_MEMORY_SIZE</CODE>,
 * <CODE>PSGL_INIT_HOST_MEMORY_SIZE</CODE>, <CODE>PSGL_INIT_ERROR_CONSOLE</CODE>, and
 * <CODE>PSGL_INIT_FIFO_SIZE</CODE>.
 * </TD>
 * </TR>
 * <TR>
 * <TD><CODE>maxSPUs</CODE></TD>
 * <TD>Many PSGL calls require memory transfers to and from RSX&trade; local memory (e.g. gl*Tex*, 
 * glReadPixels, glBuffer*, glDraw*, cgCopyProgram, cgCreateProgram). PSGL uses an SPU by 
 * default (maxSPUs=1) to improve the performance of frequent transfers larger than 128 bytes. 
 * This SPU is used in "raw" mode, which means it is locked and cannot be used for other purposes. 
 * If an additional SPU is needed, it is recommended to set this value to 0. This should 
 * typically not affect performance since operations and resources that require excessive large 
 * copies should be avoided except during initialization (e.g. texture and shader loading on 
 * startup, etc).

 * </TD>
 * </TR>
 * <TR>
 * <TD><CODE>initializeSPUs</CODE></TD>
 * <TD>Whether or not the system should be called to initialize the SPUs.
 * The default is <CODE>GL_TRUE</CODE>, which indicates that the system should initialize the SPUs.
 * </TD>
 * </TR>
 * <TR>
 * <TD><CODE>persistentMemorySize</CODE></TD>
 * <TD>Not used and ignored.
 * </TD>
 * </TR>
 * <TR>
 * <TD><CODE>transientMemorySize</CODE></TD>
 * <TD>Not used and ignored.
 * </TD>
 * </TR>
 * <TR>
 * <TD><CODE>errorConsole</CODE></TD>
 * <TD>A flag to enable output from the PSGL reporting API. A non-zero value enables output; 0 disables output.
 * When output is enabled, report messages are written to <c>stderr</c>.
 * </TD>
 * </TR>
 * <TR>
 * <TD><CODE>fifoSize</CODE></TD>
 * <TD>The size of the queue for memory transfer to the RSX&trade; video memory (VRAM).
 * The default is 256 KB (256 << 10 bytes).
 * </TD>
 * </TR>
 * <TR>
 * <TD><CODE>hostMemorySize</CODE></TD>
 * <TD>The size of host memory allocated for use by the RSX&trade;.  Host memory is used for system memory
 * textures and buffer objects (both vertex buffer objects and pixel buffer objects).  The default value is 0.
 * </TD>
 * </TR>
 * </TABLE>
 *
 * After some of the RSX&trade; memory is allocated during initialization,
 * the remaining RSX&trade; memory is used for the frame buffer, for
 * render buffers, and for textures with a <c>GL_TEXTURE_ALLOCATION_HINT_SCE</c>
 * parameter setting of <c>GL_TEXTURE_TILED_GPU_SCE setting</c>.
 *
 * One allocated SPU is typically ideal for PSGL library performance, because one SPU is sufficient
 * to transfer data to the RSX&trade;.
 * It is legal to request that no (zero) SPUs be initialized, but
 * graphics performance may be severely degraded, unless an alternative
 * method of transferring data is implemented.
 *
 * The SPUs allocated by psglInit() are raw SPUs, not an SPU thread.
 * A raw SPU is provided limited system calls. For example, it is
 * not scheduled by the OS kernel. For more information on raw SPUs and
 * SPU threads, see the <I>Cell OS Lv-2 User&rsquo;s Manual.</I>
 *
 * @param options A pointer to a structure that describes PSGL Library resource use.
 *
 * @sa psglCreateContext, psglCreateDeviceAuto, psglCreateDeviceExtended, psglExit
 */
void psglInit( PSGLinitOptions* options )
{
#ifdef JS_PERFORMANCE_INFO
#define COUNTER(Sem,Name,Text) _JS_PERFORMANCE_RESET(Name);
    COUNTER_LIST
#undef COUNTER
#endif

#ifdef JS_APIENTRY_TIMERS
#define APIENTRY_TIMER(Name) JS_APIENTRY_TIMER_RESET(Name);
    APIENTRY_TIMER_LIST
#undef APIENTRY_TIMER
#endif
   
	if ( !_jsInitCompleted )
	{
		_jsReportInit();

		int ret = cellSysmoduleLoadModule( CELL_SYSMODULE_GCM_SYS );
		switch ( ret )
		{
			case CELL_OK:
				//printf( "GCM wasn't loaded; good thing we did\n" );
				break;
			case CELL_SYSMODULE_ERROR_DUPLICATED:
				//printf( "GCM was loaded already\n" );
				break;
			default:
				//JS_ASSERT( ret == CELL_OK );
				break;
		}

		ret = cellSysmoduleLoadModule( CELL_SYSMODULE_RESC );
		switch ( ret )
		{
			case CELL_OK:
				//printf( "RESC wasn't loaded; good thing we did\n" );
				break;
			case CELL_SYSMODULE_ERROR_DUPLICATED:
				//printf( "RESC was loaded already\n" );
				break;
			default:
				//JS_ASSERT( ret == CELL_OK );
				break;
		}

		_jsPlatformInit( options );
		_jsAsyncCopyInit();
		_jsInitPerformanceCounter();
		_jsDeviceInit( options );
		_jsSegmentInit();
		_CurrentContext = NULL;
		_CurrentDevice = NULL;
		#ifdef JS_DPM
			if ( _jsdpmInit( options ? options->enable : true ) != _JS_DPM_GOOD_RETURN )    _JS_DPM_ASSERT( "failed to initialize DPM runtime" );
		#endif
	}

	_jsInitCompleted = 1;
}

/** @} PSGL */

// located in Utils/Malloc.cpp, used in psglExit().
void _jsResetAllocatorFuncs( void );

/**
 * @addtogroup PSGL
 *
 * @{
 */

/**
 * @short Terminates the PSGL library.
 *
 * Gracefully terminates the PSGL library and associated resources.
 *
 * @sa psglInit
 */
void psglExit()
{
    // glFinish();
    PSGLcontext* LContext = _CurrentContext;
    if ( LContext )
    {
        glFlush();
    
		_jsPlatformRasterFinish();
		psglMakeCurrent( NULL, NULL );
		_jsSegmentExit();
		_jsDeviceExit();

#ifdef JS_DPM
		_jsdpmTerminate();
#endif

#ifdef JS_PERFORMANCE_INFO
#define COUNTER(Sem,Name,Text) printf(Text,(float)_JS_PERFORMANCE_VALUE(Name));
		COUNTER_LIST
#undef COUNTER
#endif

#ifdef JS_APIENTRY_TIMERS
		printf( "Time spent inside PSGL\n" );
		printf( "%35s %10s %s\n", "Function", "Total MS", "Times called" );
#define APIENTRY_TIMER(Name) printf("%35s %10f %i\n", #Name, \
										JS_APIENTRY_TIMER_VALUE(Name) * 1000.0f,\
										JS_APIENTRY_TIMER_HITCOUNT(Name));
		APIENTRY_TIMER_LIST
#undef APIENTRY_TIMER
#endif

		_jsPlatformExit();
#ifdef E_MEMDEBUG
		EPrintUnfreedMemory();
#endif
	    _jsResetAllocatorFuncs();
		
		_CurrentContext = NULL; 

		_jsInitCompleted = 0;
	}
}
/** @} PSGL */

/**
 * @addtogroup OpenGL
 *
 * @{
 */

/////////////////////////////////////////////////////////////////////////////
// string marker management

/**
 * @short Pushes a string onto the marker stack
 *
 * glPushStringMarkerSCE() pushes a string onto the marker stack.
 * A wrapper function for <c>cellGcmSetPerfMonPushMarker()</c>.  See
 * the <i>libgcm_pm Reference</i> for further information.
 *
 * @param szName The string to push onto the stack
 *
 * @sa glPopStringMarkerSCE
 */
GLAPI void APIENTRY glPushStringMarkerSCE( const GLubyte* szName )
{
    JS_APIENTRY_TIMER( glPushStringMarkerSCE );
#ifdef JS_DPM
    if ( _jsdpmState )
    {
        _JS_DPM_API_ENTRY_1( glPushStringMarkerSCE, JSDPM_CALL_TYPE_GET, , 1,
                             szName, sizeof( const GLubyte* ), INPUT_BY_VALUE, DPM_API_ARGUMENT_BYTE_POINTER );
    }
#endif

	_jsGcmFifoPushMarker( szName ); 
}


/**
 * @short Pops a string off of the marker stack
 *
 * glPopStringMarkerSCE() pops a string off of the marker stack.  
 * A wrapper function for <c>cellGcmSetPerfMonPopMarker()</c>.  See
 * the <i>libgcm_pm Reference</i> for further information.
 *
 * @sa glPushStringMarkerSCE
 */
GLAPI void APIENTRY glPopStringMarkerSCE()
{
    JS_APIENTRY_TIMER( glPopStringMarkerSCE );
#ifdef JS_DPM
    if ( _jsdpmState )
    {
        _JS_DPM_API_ENTRY_0( glPopStringMarkerSCE, JSDPM_CALL_TYPE_GET, );
    }
#endif

	_jsGcmFifoPopMarker(); 
}

/** 
 * @short (Deprecated) Replace the top value in the marker stack
 *
 * This function has been deprecated.
 *
 * @param len Length of <i><c>szString</c></i>
 * @param szString The string to swap into the stack
 *
 * @sa glPushStringMarkerSCE, glPopStringMarkerSCE
 */
GLAPI void APIENTRY glStringMarkerGREMEDY( GLsizei len, const void *szString )
{
    JS_APIENTRY_TIMER( glStringMarkerGREMEDY );
#ifdef JS_DPM
    if ( _jsdpmState )
    {
        if ( len == 0 ) // string is null-terminated
        {
            _JS_DPM_API_ENTRY_2( glStringMarkerGREMEDY, JSDPM_CALL_TYPE_GET, , 2,
                                 len, sizeof( GLsizei ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                                 szString, sizeof( const void * ), INPUT_BY_VALUE, DPM_API_ARGUMENT_BYTE_POINTER );
        }
        // don't handle the non-terminated string case for now.
    }
#endif
}
/** @} OpenGL */

/**
 * @addtogroup PSGL
 *
 * @{
 */

/////////////////////////////////////////////////////////////////////////////
// memory allocator management

/**
 * @short Override default memory allocation functions.
 *
 * psglSetAllocatorFuncs() sets the functions to be used by PSGL for memory allocation.
 * If it is used, it must be called before psglInit(), and all function pointers must be specified.
 *
 * @param mallocFunc pointer to the replacement <CODE>malloc()</CODE> function.  Result expected to be 16-byte aligned.
 * @param memalignFunc pointer to the replacement <CODE>memalign()</CODE> function.
 * @param reallocFunc pointer to the replacement <CODE>realloc()</CODE> function.
 * @param freeFunc pointer to the replacement <CODE>free()</CODE> function.
 *
 * @return Returns <CODE>GL_TRUE</CODE> for success, or <CODE>GL_FALSE</CODE> in case of error.
 *
 * @sa psglInit
 */
PSGL_EXPORT GLboolean psglSetAllocatorFuncs(
    PSGLmallocFunc mallocFunc,
    PSGLmemalignFunc memalignFunc,
    PSGLreallocFunc reallocFunc,
    PSGLfreeFunc freeFunc )
{
    if ( _getJsInitCompleted() )
    {
        printf( "psglSetAllocatorFuncs: do not call after psglInit()!!\n" );
        return GL_FALSE;
    }

    if ( !mallocFunc || !memalignFunc || !reallocFunc || !freeFunc )
    {
        printf( "psglSetAllocatorFuncs: must specify all allocator functions\n" );
        return GL_FALSE;
    }

    jsMallocFunc = mallocFunc;
    jsMemalignFunc = memalignFunc;
    jsReallocFunc  = reallocFunc;
    jsFreeFunc  = freeFunc;

    return GL_TRUE;
}

/////////////////////////////////////////////////////////////////////////////

/**
 * @short (Deprecated) Sets the size of the bounce buffer.
 *
 * PSGL no longer uses bounce buffers; therefore, this function is now 
 * deprecated. No matter what value is passed, it does not change any
 * behavior of PSGL internally. 
 *
 * @param size The size, in bytes, of the bounce buffer to be created.
 *
 * @sa psglGetBounceBufferSize
 */
PSGL_EXPORT void psglSetBounceBufferSize( GLsizei size )
{
}

/**
 * @short (Deprecated) Gets the size of the bounce buffer.
 *
 * PSGL no longer uses bounce buffers; therefore, this function is now
 * deprecated. It always returns 0 when called.
 *
 * @return Always returns 0.
 *
 * @sa psglSetBounceBufferSize
 */
PSGL_EXPORT GLsizei psglGetBounceBufferSize()
{
    return 0;
}

/** @} PSGL */

/////////////////////////////////////////////////////////////////////////////

#ifdef JS_APIENTRY_TIMERS
extern "C" void _jsDumpAPIEntryTimers()
{
    printf( "Time spent inside PSGL\n" );
    printf( "%35s %10s %s\n", "Function", "Total MS", "Times called" );
#define APIENTRY_TIMER(Name) printf("%35s %10f %i\n", #Name, \
                                    JS_APIENTRY_TIMER_VALUE(Name) * 1000.0f,\
                                    JS_APIENTRY_TIMER_HITCOUNT(Name));
    APIENTRY_TIMER_LIST
#undef APIENTRY_TIMER
}
extern "C" void _jsResetAPIEntryTimers()
{
    _jsEntryTimer_Overall = 0;
#define APIENTRY_TIMER(Name) JS_APIENTRY_TIMER_RESET(Name);
    APIENTRY_TIMER_LIST
#undef APIENTRY_TIMER
}


#ifdef JS_DPM_V2
#include <RSXTools/PadAPI.h>
#endif
extern "C" void psglHUDPadInput( struct CellPadData* pPadData )
{
#ifdef JS_DPM_V2
    hudc_padapi_giveInput( pPadData );
#else
    // noop
#endif // dpm v2
}

#endif

