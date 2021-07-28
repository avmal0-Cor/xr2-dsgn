/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 *                Copyright (C) 2009 Sony Computer Entertainment Inc.
 *                                               All Rights Reserved.
 */


/*
 * Contains implementations of inline functions which validate current state.
 */

#ifndef _GCM_VALIDATE_INLINES_H_
#define _GCM_VALIDATE_INLINES_H_
#include <JSGCM/JSGcmUtils.h>
#include <JSGCM/JSGcmFifo.h>
#include <JSGCM/JSGcmFifoUtils.h>
#include <JSGCM/JSGcmDriver.h>
#include <JSGCM/JSGcmFBOps.h>


inline static GLboolean _jsIsFloat32ColorFormat(GLenum colorFormat)
{
	switch (colorFormat)
	{
		case JSGCM_FLOAT_RGBA32:
		case JSGCM_FLOAT_RGBX32:
		case JSGCM_LUMINANCE32F_ARB:
		case JSGCM_FLOAT_R32:
			return GL_TRUE;
		default:
			return GL_FALSE;
	}
    return GL_FALSE; 
}

inline static GLboolean _jsIsFloatColorFormat(GLenum colorFormat)
{
	switch (colorFormat)
	{
		case JSGCM_FLOAT_RGBA32:
		case JSGCM_FLOAT_RGBX32:
		case JSGCM_FLOAT_RGBA16:
		case JSGCM_FLOAT_RGBX16:
		case JSGCM_LUMINANCE32F_ARB:
		case JSGCM_FLOAT_R32:
			return GL_TRUE;
		default:
			return GL_FALSE;
	}
    return GL_FALSE; 
}

inline static void _jsValidateViewport()
{
	PSGLcontext*	LContext = _CurrentContext;
	jsGcmDriver *driver=_jsGetGcmDriver();

	_jsGcmFifoGlViewport(
		LContext->ViewPort.X*driver->xSuperSampling, 
		LContext->ViewPort.Y*driver->ySuperSampling, 
		LContext->ViewPort.XSize*driver->xSuperSampling, 
		LContext->ViewPort.YSize*driver->ySuperSampling,
		LContext->DepthNear,
		LContext->DepthFar);

	if (LContext->DepthClamp)
		_jsGcmFifoGlEnable(JSGCM_DEPTH_CLAMP);
	else
		_jsGcmFifoGlDisable(JSGCM_DEPTH_CLAMP);

}

inline static void _jsValidateCullFace()
{
	PSGLcontext*	LContext = _CurrentContext;

	if (LContext->FaceCulling)
	{
		GCM_FUNC( cellGcmSetCullFaceEnable, CELL_GCM_TRUE); 
		jsGcmEnum mode=(jsGcmEnum)LContext->CullFaceMode;
		GCM_FUNC( cellGcmSetCullFace, mode); 
	}
	else
		GCM_FUNC( cellGcmSetCullFaceEnable, CELL_GCM_FALSE); 

	// [RSTENSON] this is important to call in order to get the 
	// ccw/cw switching correcte based on the screen yInversion state.   
	_jsGcmFifoGlFrontFace((jsGcmEnum)LContext->FrontFace);
	//GCM_FUNC( cellGcmSetFrontFace, LContext->FrontFace); 

	// two-sided lighting
	if (LContext->LightModel.Light2Sided)
	{
		GCM_FUNC( cellGcmSetTwoSideLightEnable, CELL_GCM_TRUE); 
	}
	else
		GCM_FUNC(cellGcmSetTwoSideLightEnable, CELL_GCM_FALSE); 
}

inline static void _jsValidatePointRaster()
{
	PSGLcontext*	LContext = _CurrentContext;
	GCM_FUNC( cellGcmSetPointSize, LContext->PointSize); 
}

inline static void _jsValidateLineRaster()
{
	PSGLcontext*	LContext = _CurrentContext;

	jsGcmDriver *driver=_jsGetGcmDriver();
	const GLboolean isFloat=_jsIsFloatColorFormat(driver->rt.colorFormat);

	GCM_FUNC( cellGcmSetLineSmoothEnable, ((!isFloat) && (LContext->LineSmooth)) ? CELL_GCM_TRUE : CELL_GCM_FALSE ); 
	_jsGcmFifoGlLineWidth(LContext->LineWidth);
}

inline static void _jsValidatePolygonOffset()
{
	PSGLcontext*	LContext = _CurrentContext;

  if (LContext->PolygonOffsetFill) 
	  _jsGcmFifoGlEnable(JSGCM_POLYGON_OFFSET_FILL);
  else 
	  _jsGcmFifoGlDisable(JSGCM_POLYGON_OFFSET_FILL);
  if (LContext->PolygonOffsetLine) 
	  _jsGcmFifoGlEnable(JSGCM_POLYGON_OFFSET_LINE);
  else 
	  _jsGcmFifoGlDisable(JSGCM_POLYGON_OFFSET_LINE);
	_jsGcmFifoGlPolygonOffset(LContext->PolygonOffsetFactor,LContext->PolygonOffsetUnits);
	_CurrentContext->needValidate &= ~PSGL_VALIDATE_POLYGON_OFFSET;
}

inline static void _jsValidatePrimitiveRestart()
{
	PSGLcontext*	LContext = _CurrentContext;

	if (LContext->attribs->PrimitiveRestart) 
		_jsGcmFifoGlEnable(JSGCM_PRIMITIVE_RESTART);
	else 
		_jsGcmFifoGlDisable(JSGCM_PRIMITIVE_RESTART);
	_jsGcmFifoGlPrimitiveRestartIndex(LContext->attribs->RestartIndex);
}

inline static void _jsValidatePolygonMode()
{
	PSGLcontext*	LContext = _CurrentContext;
	GCM_FUNC( cellGcmSetFrontPolygonMode, LContext->FrontPolygonMode); 
	GCM_FUNC( cellGcmSetBackPolygonMode, LContext->BackPolygonMode); 
}

inline static void _jsValidateShadeModel()
{
	PSGLcontext*	LContext = _CurrentContext;

	_jsGcmFifoGlShadeModel((jsGcmEnum)LContext->ShadeModel);
}

inline static void _jsValidateShaderSRGBRemap()
{
	PSGLcontext*	LContext = _CurrentContext;

	GCM_FUNC( cellGcmSetFragmentProgramGammaEnable, LContext->ShaderSRGBRemap ? CELL_GCM_TRUE : CELL_GCM_FALSE); 

	LContext->needValidate &= ~PSGL_VALIDATE_SHADER_SRGB_REMAP;

}

inline static void _jsValidatePointSprite()
{
	PSGLcontext*	LContext = _CurrentContext;

	// should use this CELL_GCM_POINT_SPRITE_TEX0
	GCM_FUNC( cellGcmSetPointSpriteControl, CELL_GCM_TRUE, CELL_GCM_POINT_SPRITE_RMODE_FROM_R, LContext->TexCoordReplaceMask<<8 ) // GCM_PORT_TESTED [TSCOVILL and RSTENSON]

	jsGcmInterpolantState *istate = &_jsGcmState.state.interpolant;
	if (LContext->PointSprite)
	{
		GLuint coord = LContext->TexCoordReplaceMask;
		// the vertex attrib output mask texcoords are not contiguous...
		istate->pointSpriteMask = ((coord & 0xff) << 14) | (((coord & 0x300) >> 8) << 12)
				| (LContext->VertexProgramPointSize ? 0x20 : 0); // enable point size attribute
	}
	else
	{
		istate->pointSpriteMask = 0;
	}

  GCM_FUNC( cellGcmSetVertexAttribOutputMask, (istate->vertexProgramAttribMask | istate->pointSpriteMask) &
		istate->fragmentProgramAttribMask & istate->clipMask & istate->sideMask ); 
}

inline static void _jsValidateMultisampling(void)
{
	PSGLcontext* LContext = _CurrentContext;
	JS_ASSERT(LContext != NULL);
	jsGcmDriver *driver=_jsGetGcmDriver();

	//  Each hex digit is a 2x2 sample submask (one multisampled pixel).
	//  Four digits produces a 4x4 sample mask.
	
  static GLuint maskTable[] = {
		0x0000,
		0x0001, 0x1001, 0x1011, 0x1111,
		0x1119, 0x9119, 0x9199, 0x9999,
		0x999d, 0xd99d, 0xd9dd, 0xdddd,
		0xdddf, 0xfddf, 0xfdff, 0xffff,
	};
	
	// pick mask
	JS_ASSERT(LContext->SampleCoverageValue >= 0.0f &&
			  LContext->SampleCoverageValue <= 1.0f);
	const int index = (int)(LContext->SampleCoverageValue*16);
	const GLuint mask = LContext->SampleCoverageInvert ? ~maskTable[index] : maskTable[index];

	// no multisampling with float buffers
	GLboolean multisample=_jsIsFloatColorFormat(driver->rt.colorFormat)?GL_FALSE:LContext->Multisample;
	// no alpha-to-coverage with MRT
	GLboolean alphaToCoverage=(driver->rt.colorBufferCount>1)?GL_FALSE:LContext->SampleAlphaToCoverage;
  
	GCM_FUNC( cellGcmSetAntiAliasingControl, 
            multisample ? CELL_GCM_TRUE : CELL_GCM_FALSE, 
            alphaToCoverage ? CELL_GCM_TRUE : CELL_GCM_FALSE,
            LContext->SampleAlphaToOne ? CELL_GCM_TRUE : CELL_GCM_FALSE, 
            mask ); // GCM_PORT_TESTED [KHOFF]

	GCM_FUNC( cellGcmSetBlendOptimization, LContext->ReduceDstColor ); 

	LContext->needValidate &= ~PSGL_VALIDATE_MULTISAMPLING;
}


inline static void _jsValidateTwoSideColor(void)
{
	PSGLcontext *LContext=_CurrentContext;
	JS_ASSERT(LContext != NULL);

 	// enable COLOR1 output
 	if (LContext->LightModel.Light2Sided)
 		_jsGcmFifoGlEnable(JSGCM_LIGHT_MODEL_TWO_SIDE);
 	else
 		_jsGcmFifoGlDisable(JSGCM_LIGHT_MODEL_TWO_SIDE);
}

inline static void _jsValidateAlphaTest()
{
	PSGLcontext*	LContext = _CurrentContext;

	jsGcmDriver *driver=_jsGetGcmDriver();
	const GLboolean isFloat32=_jsIsFloat32ColorFormat(driver->rt.colorFormat);
	
	// no hardware alpha test with multiple render targets or FP32
	const GLboolean mrt = driver->rt.colorBufferCount > 1;
#ifndef JS_NO_ERROR_CHECK
	if (mrt && LContext->AlphaTest)
	{
		_jsSetError(GL_INVALID_OPERATION);
		_JS_REPORT_EXTRA(PSGL_REPORT_GL_ERROR,"Alpha test is not supported with multiple render targets, disabling alpha test");
	}
    if (isFloat32 && LContext->AlphaTest)
    {
        _jsSetError(GL_INVALID_OPERATION);
        _JS_REPORT_EXTRA(PSGL_REPORT_GL_ERROR,"Alpha test is not supported with FP32 render targets, disabling alpha test");
    }
#endif

	// Because the GPU setting depends on the surface type, we will trigger
	// validation in _jsPlatformRasterBindSurface on binding a new color
	// buffer.
	if (LContext->AlphaTest&&(!isFloat32)&&(!mrt))
	{
		_jsGcmFifoGlEnable(JSGCM_ALPHA_TEST);
		_jsGcmFifoGlAlphaFunc((jsGcmEnum)LContext->AlphaFunc,LContext->AlphaRef);
	}
	else _jsGcmFifoGlDisable(JSGCM_ALPHA_TEST);

}

inline static void _jsValidateDepthTest()
{
	PSGLcontext*	LContext = _CurrentContext;

	if (LContext->DepthTest && (_jsGetGcmDriver()->rt.depthFormat!=JSGCM_NONE))
	{
		_jsGcmFifoGlEnable(JSGCM_DEPTH_TEST);
		_jsGcmFifoGlDepthFunc((jsGcmEnum)LContext->DepthFunc);
	}
	else 
        _jsGcmFifoGlDisable(JSGCM_DEPTH_TEST);

}

inline static void _jsValidateWriteMask()
{

	PSGLcontext*	LContext = _CurrentContext;
	GLuint mask=LContext->WriteMask;

	jsGcmDriver *driver=_jsGetGcmDriver();
	const GLboolean depthValid=(driver->rt.depthFormat!=JSGCM_NONE);
	_jsGcmFifoGlDepthMask(depthValid && ((mask & _JS_CONTEXT_DEPTH_MASK) != 0));

	// restrict color writes to active render targets
	if (JS_UNLIKELY(driver->rt.colorBufferCount > 1))
	{
		// multiple render targets
		GLuint colorMask = 0;
		if ( driver->colorBufferMask & 0x1 )
		{
			colorMask |= ( (mask & _JS_CONTEXT_RED_MASK)   ? CELL_GCM_COLOR_MASK_R : 0 );
			colorMask |= ( (mask & _JS_CONTEXT_GREEN_MASK) ? CELL_GCM_COLOR_MASK_G : 0 );
			colorMask |= ( (mask & _JS_CONTEXT_BLUE_MASK)  ? CELL_GCM_COLOR_MASK_B : 0 );
			colorMask |= ( (mask & _JS_CONTEXT_ALPHA_MASK) ? CELL_GCM_COLOR_MASK_A : 0 );
		}
		GCM_FUNC( cellGcmSetColorMask, colorMask );

		GLuint mrtMask = 0;
		if ( driver->colorBufferMask & 0x2 )
		{
			mrtMask |= ( (mask & _JS_CONTEXT_RED_MASK)   ? CELL_GCM_COLOR_MASK_MRT1_R : 0 );
			mrtMask |= ( (mask & _JS_CONTEXT_GREEN_MASK) ? CELL_GCM_COLOR_MASK_MRT1_G : 0 );
			mrtMask |= ( (mask & _JS_CONTEXT_BLUE_MASK)  ? CELL_GCM_COLOR_MASK_MRT1_B : 0 );
			mrtMask |= ( (mask & _JS_CONTEXT_ALPHA_MASK) ? CELL_GCM_COLOR_MASK_MRT1_A : 0 );
		}
		if ( driver->colorBufferMask & 0x4 )
		{
			mrtMask |= ( (mask & _JS_CONTEXT_RED_MASK)   ? CELL_GCM_COLOR_MASK_MRT2_R : 0 );
			mrtMask |= ( (mask & _JS_CONTEXT_GREEN_MASK) ? CELL_GCM_COLOR_MASK_MRT2_G : 0 );
			mrtMask |= ( (mask & _JS_CONTEXT_BLUE_MASK)  ? CELL_GCM_COLOR_MASK_MRT2_B : 0 );
			mrtMask |= ( (mask & _JS_CONTEXT_ALPHA_MASK) ? CELL_GCM_COLOR_MASK_MRT2_A : 0 );
		}
		if ( driver->colorBufferMask & 0x8 )
		{
			mrtMask |= ( (mask & _JS_CONTEXT_RED_MASK)   ? CELL_GCM_COLOR_MASK_MRT3_R : 0 );
			mrtMask |= ( (mask & _JS_CONTEXT_GREEN_MASK) ? CELL_GCM_COLOR_MASK_MRT3_G : 0 );
			mrtMask |= ( (mask & _JS_CONTEXT_BLUE_MASK)  ? CELL_GCM_COLOR_MASK_MRT3_B : 0 );
			mrtMask |= ( (mask & _JS_CONTEXT_ALPHA_MASK) ? CELL_GCM_COLOR_MASK_MRT3_A : 0 );
		}

		GCM_FUNC( cellGcmSetColorMaskMrt, mrtMask ); 
	}
	else
	{
		// single default render target
		//  JSGCM caches the color mask for use with clear, so in this case
		//  we just call JSGCM so it can update its cache.  We don't use JSGCM
		//  clear for MRT, so not calling it in the MRT case should be okay
		//  (though having two mechanisms is admittedly sloppy).
		const GLboolean colorValid=(driver->rt.colorBufferCount>0);
		_jsGcmFifoGlColorMask(
				colorValid &&((mask & _JS_CONTEXT_RED_MASK) != 0),
				colorValid &&((mask & _JS_CONTEXT_GREEN_MASK) != 0),
				colorValid &&((mask & _JS_CONTEXT_BLUE_MASK) != 0),
				colorValid &&((mask & _JS_CONTEXT_ALPHA_MASK) != 0)
				);
	}
}

inline static void _jsValidateStencilTest()
{
	PSGLcontext*	LContext = _CurrentContext;

    // stencil only works with D24S8
	if (LContext->StencilTest && (_jsGetGcmDriver()->rt.depthFormat==JSGCM_DEPTH_COMPONENT24))
	{
		_jsGcmFifoGlEnable(JSGCM_STENCIL_TEST);

		if (LContext->StencilTestTwoSide)
			_jsGcmFifoGlEnable(JSGCM_STENCIL_TEST_TWO_SIDE);
		else
			_jsGcmFifoGlDisable(JSGCM_STENCIL_TEST_TWO_SIDE);
	}
	else
		_jsGcmFifoGlDisable(JSGCM_STENCIL_TEST);

	// Need to always validate the function, because the stencil op REPLACE may use
	// the reference value, even if stencil test is disabled
	_jsGcmFifoGlActiveStencilFace(JSGCM_BACK);
	_jsGcmFifoGlStencilFunc(
		(jsGcmEnum)LContext->StencilBackfaceFunc,
		LContext->StencilBackfaceRef,
		LContext->StencilBackfaceFuncMask);
	_jsGcmFifoGlActiveStencilFace(JSGCM_FRONT);
	_jsGcmFifoGlStencilFunc(
		(jsGcmEnum)LContext->StencilFunc,
		LContext->StencilRef,
		LContext->StencilFuncMask);
}

inline static void _jsValidateStencilOpAndMask()
{
	PSGLcontext*	LContext = _CurrentContext;

	_jsGcmFifoGlActiveStencilFace(JSGCM_BACK);
	_jsGcmFifoGlStencilOp(
		(jsGcmEnum)LContext->StencilBackfaceOpFail,
		(jsGcmEnum)LContext->StencilBackfaceOpZFail,
		(jsGcmEnum)LContext->StencilBackfaceOpZPass);
	_jsGcmFifoGlStencilMask(LContext->StencilBackfaceWriteMask);

	_jsGcmFifoGlActiveStencilFace(JSGCM_FRONT);
	_jsGcmFifoGlStencilOp(
		(jsGcmEnum)LContext->StencilOpFail,
		(jsGcmEnum)LContext->StencilOpZFail,
		(jsGcmEnum)LContext->StencilOpZPass);
	_jsGcmFifoGlStencilMask(LContext->StencilWriteMask);
}

inline static void _jsValidateScissorBox()
{
	PSGLcontext*	LContext = _CurrentContext;
	jsGcmDriver *driver=_jsGetGcmDriver();

	if (LContext->ScissorTest)
	{
		_jsGcmFifoGlEnable(JSGCM_SCISSOR_TEST);
		_jsGcmFifoGlScissor(LContext->ScissorBox.X*driver->xSuperSampling,LContext->ScissorBox.Y*driver->ySuperSampling,LContext->ScissorBox.XSize*driver->xSuperSampling,LContext->ScissorBox.YSize*driver->ySuperSampling);
	}
	else _jsGcmFifoGlDisable(JSGCM_SCISSOR_TEST);
}

inline static void _jsValidateLogicOp()
{
	PSGLcontext*	LContext = _CurrentContext;

	if (LContext->ColorLogicOp)
	{
		_jsGcmFifoGlEnable(JSGCM_COLOR_LOGIC_OP);
		_jsGcmFifoGlLogicOp((jsGcmEnum)LContext->LogicOp);
	}
	else 
        _jsGcmFifoGlDisable(JSGCM_COLOR_LOGIC_OP);
}

inline static void _jsValidateBlending()
{
	PSGLcontext*	LContext = _CurrentContext;

	jsGcmDriver *driver=_jsGetGcmDriver();
	const GLboolean isFloat32=_jsIsFloat32ColorFormat(driver->rt.colorFormat);
	
	// Because the GPU setting depends on the surface type, we will trigger
	// validation in _jsPlatformRasterBindSurface on binding a new color
	// buffer.
	if ((LContext->Blending || LContext->BlendingMrt[0] || LContext->BlendingMrt[1] || LContext->BlendingMrt[2]) && (!isFloat32))
	{
    // cannot call _jsGcmFifoGlEnable(GL_BLEND) because they must be set separately per MRT
    GCM_FUNC( cellGcmSetBlendEnable, LContext->Blending );
    GCM_FUNC( cellGcmSetBlendEnableMrt, LContext->BlendingMrt[0], LContext->BlendingMrt[1], LContext->BlendingMrt[2] );

		_jsGcmFifoGlBlendColor(
			LContext->BlendColor.R,
			LContext->BlendColor.G,
			LContext->BlendColor.B,
			LContext->BlendColor.A);
		_jsGcmFifoGlBlendEquation(
			(jsGcmEnum)LContext->BlendEquationRGB,
			(jsGcmEnum)LContext->BlendEquationAlpha);
		_jsGcmFifoGlBlendFunc((jsGcmEnum)LContext->BlendFactorSrcRGB,(jsGcmEnum)LContext->BlendFactorDestRGB,(jsGcmEnum)LContext->BlendFactorSrcAlpha,(jsGcmEnum)LContext->BlendFactorDestAlpha);
	}
	else 
        _jsGcmFifoGlDisable(JSGCM_BLEND);
}

#endif
