/*   SCE CONFIDENTIAL                                       */
/*   PlayStation(R)3 Programmer Tool Runtime Library 310.001 */
/*   Copyright (C) 2008 Sony Computer Entertainment Inc.    */
/*   All Rights Reserved.                                   */

CELL_GCM_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetReferenceCommand)(CELL_GCM_ARGS(uint32_t ref))
{
	(void) ref;
	CELL_GCM_RESERVE(2);
	CELL_GCM_FUNC_RETURN;
}

CELL_GCM_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetJumpCommand)(CELL_GCM_ARGS(uint32_t offset))
{
	(void) offset;
	CELL_GCM_RESERVE(1);
	CELL_GCM_FUNC_RETURN;
}

CELL_GCM_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetCallCommand)(CELL_GCM_ARGS(uint32_t offset))
{
	(void) offset;
	CELL_GCM_RESERVE(1);
	CELL_GCM_FUNC_RETURN;
}

CELL_GCM_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetReturnCommand)(CELL_GCM_NO_ARGS())
{
	CELL_GCM_RESERVE(1);
	CELL_GCM_FUNC_RETURN;
}

CELL_GCM_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetNopCommand)(CELL_GCM_ARGS(uint32_t count))
{
	(void) count;
	CELL_GCM_RESERVE(count);
	CELL_GCM_FUNC_RETURN;
}

CELL_GCM_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetSurface)(CELL_GCM_ARGS(const CellGcmSurface *surface))
{
	(void) surface;
	CELL_GCM_RESERVE(32);
	CELL_GCM_FUNC_RETURN;
}

CELL_GCM_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetSurfaceWindow)(CELL_GCM_ARGS(
	const CellGcmSurface *surface, const uint32_t origin, const uint32_t pixelCenter))
{
	(void) surface;
	(void) origin;
	(void) pixelCenter;
	CELL_GCM_RESERVE(32);
	CELL_GCM_FUNC_RETURN;
}

CELL_GCM_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetAntiAliasingControl)(CELL_GCM_ARGS(
	uint32_t enable, uint32_t alphaToCoverage, uint32_t alphaToOne, uint32_t sampleMask))
{
	(void) enable;
	(void) alphaToCoverage;
	(void) alphaToOne;
	(void) sampleMask;
	CELL_GCM_RESERVE(2);
	CELL_GCM_FUNC_RETURN;
}

CELL_GCM_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetWaitLabel)(CELL_GCM_ARGS(uint8_t index, uint32_t value))
{
	(void) index;
	(void) value;
	CELL_GCM_RESERVE(4);
	CELL_GCM_FUNC_RETURN;
}

CELL_GCM_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetWriteCommandLabel)(CELL_GCM_ARGS(
	uint8_t index, uint32_t value))
{
	(void) index;
	(void) value;
	CELL_GCM_RESERVE(4);
	CELL_GCM_FUNC_RETURN;
}

CELL_GCM_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetWriteBackEndLabel)(CELL_GCM_ARGS(uint8_t index, uint32_t value))
{
	(void) index;
	(void) value;
	CELL_GCM_RESERVE(4);
	CELL_GCM_FUNC_RETURN;
}

CELL_GCM_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetWriteTextureLabel)(CELL_GCM_ARGS(uint8_t index, uint32_t value))
{
	(void) index;
	(void) value;
	CELL_GCM_RESERVE(4);
	CELL_GCM_FUNC_RETURN;
}

CELL_GCM_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetTimeStamp)(CELL_GCM_ARGS(uint32_t index))
{
	(void) index;
	CELL_GCM_RESERVE(2);
	CELL_GCM_FUNC_RETURN;
}

CELL_GCM_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetInvalidateZcull)(CELL_GCM_NO_ARGS())
{
	CELL_GCM_RESERVE(2);
	CELL_GCM_FUNC_RETURN;
}

CELL_GCM_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetAlphaFunc)(CELL_GCM_ARGS(uint32_t af, uint32_t ref))
{
	(void) af;
	(void) ref;
	CELL_GCM_RESERVE(3);
	CELL_GCM_FUNC_RETURN;
}

CELL_GCM_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetBlendColor)(CELL_GCM_ARGS(
	uint32_t color, uint32_t color2))
{
	(void) color;
	(void) color2;
	CELL_GCM_RESERVE(4);
	CELL_GCM_FUNC_RETURN;
}

CELL_GCM_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetBlendEquation)(CELL_GCM_ARGS(
	uint16_t color, uint16_t alpha))
{
	(void) color;
	(void) alpha;
	CELL_GCM_RESERVE(2);
	CELL_GCM_FUNC_RETURN;
}

CELL_GCM_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetBlendFunc)(CELL_GCM_ARGS(uint16_t sfcolor, 
	uint16_t dfcolor, uint16_t sfalpha, uint16_t dfalpha))
{
	(void) sfcolor;
	(void) dfcolor;
	(void) sfalpha;
	(void) dfalpha;
	CELL_GCM_RESERVE(3);
	CELL_GCM_FUNC_RETURN;
}

CELL_GCM_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetClearSurface)(CELL_GCM_ARGS(uint32_t mask))
{
	(void) mask;
	CELL_GCM_RESERVE(4);
	CELL_GCM_FUNC_RETURN;
}

CELL_GCM_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetClearColor)(CELL_GCM_ARGS(uint32_t color))
{
	(void) color;
	CELL_GCM_RESERVE(2);
	CELL_GCM_FUNC_RETURN;
}

CELL_GCM_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetClearDepthStencil)(CELL_GCM_ARGS(uint32_t value))
{
	(void) value;
	CELL_GCM_RESERVE(2);
	CELL_GCM_FUNC_RETURN;
}

CELL_GCM_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetColorMask)(CELL_GCM_ARGS(uint32_t mask))
{
	(void) mask;
	CELL_GCM_RESERVE(2);
	CELL_GCM_FUNC_RETURN;
}

CELL_GCM_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetColorMaskMrt)(CELL_GCM_ARGS(uint32_t mask))
{
	(void) mask;
	CELL_GCM_RESERVE(2);
	CELL_GCM_FUNC_RETURN;
}

CELL_GCM_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetCullFace)(CELL_GCM_ARGS(uint32_t cfm))
{
	(void) cfm;
	CELL_GCM_RESERVE(2);
	CELL_GCM_FUNC_RETURN;
}

CELL_GCM_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetDepthBounds)(CELL_GCM_ARGS(float zmin, float zmax))
{
	(void) zmin;
	(void) zmax;
	CELL_GCM_RESERVE(3);
	CELL_GCM_FUNC_RETURN;
}

CELL_GCM_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetDepthFunc)(CELL_GCM_ARGS(uint32_t zf))
{
	(void) zf;
	CELL_GCM_RESERVE(2);
	CELL_GCM_FUNC_RETURN;
}

CELL_GCM_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetDepthMask)(CELL_GCM_ARGS(uint32_t enable))
{
	(void) enable;
	CELL_GCM_RESERVE(2);
	CELL_GCM_FUNC_RETURN;
}

CELL_GCM_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetFrontFace)(CELL_GCM_ARGS(uint32_t dir))
{
	(void) dir;
	CELL_GCM_RESERVE(2);
	CELL_GCM_FUNC_RETURN;
}

CELL_GCM_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetLineWidth)(CELL_GCM_ARGS(uint32_t width))
{
	(void) width;
	CELL_GCM_RESERVE(2);
	CELL_GCM_FUNC_RETURN;
}

CELL_GCM_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetLineSmoothEnable)(CELL_GCM_ARGS(uint32_t enable))
{
	(void) enable;
	CELL_GCM_RESERVE(2);
	CELL_GCM_FUNC_RETURN;
}

CELL_GCM_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetLineStippleEnable)(CELL_GCM_ARGS(uint32_t enable))
{
	(void) enable;
	CELL_GCM_RESERVE(2);
	CELL_GCM_FUNC_RETURN;
}

CELL_GCM_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetLineStipplePattern)(CELL_GCM_ARGS(
	const uint16_t* pattern, uint8_t factor))
{
	(void) pattern;
	(void) factor;
	CELL_GCM_RESERVE(2);
	CELL_GCM_FUNC_RETURN;
}

CELL_GCM_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetLogicOp)(CELL_GCM_ARGS(uint32_t op))
{
	(void) op;
	CELL_GCM_RESERVE(2);
	CELL_GCM_FUNC_RETURN;
}

CELL_GCM_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetPointSize)(CELL_GCM_ARGS(float size))
{
	(void) size;
	CELL_GCM_RESERVE(2);
	CELL_GCM_FUNC_RETURN;
}

CELL_GCM_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetPolygonOffset)(CELL_GCM_ARGS(float factor, float units))
{
	(void) factor;
	(void) units;
	CELL_GCM_RESERVE(3);
	CELL_GCM_FUNC_RETURN;
}

CELL_GCM_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetPolySmoothEnable)(CELL_GCM_ARGS(uint32_t enable))
{
	(void) enable;
	CELL_GCM_RESERVE(2);
	CELL_GCM_FUNC_RETURN;
}

CELL_GCM_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetPolygonStippleEnable)(CELL_GCM_ARGS(uint32_t enable))
{
	(void) enable;
	CELL_GCM_RESERVE(2);
	CELL_GCM_FUNC_RETURN;
}

CELL_GCM_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetPolygonStipplePattern)(CELL_GCM_ARGS(const uint32_t* pattern))
{
	(void) pattern;
	CELL_GCM_RESERVE(33);
	CELL_GCM_FUNC_RETURN;
}

CELL_GCM_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetFrontPolygonMode)(CELL_GCM_ARGS(uint32_t mode))
{
	(void) mode;
	CELL_GCM_RESERVE(2);
	CELL_GCM_FUNC_RETURN;
}

CELL_GCM_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetBackPolygonMode)(CELL_GCM_ARGS(uint32_t mode))
{
	(void) mode;
	CELL_GCM_RESERVE(2);
	CELL_GCM_FUNC_RETURN;
}

CELL_GCM_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetScissor)(CELL_GCM_ARGS(uint16_t x, uint16_t y, uint16_t w, uint16_t h))
{
	(void) x;
	(void) y;
	(void) w;
	(void) h;
	CELL_GCM_RESERVE(3);
	CELL_GCM_FUNC_RETURN;
}

CELL_GCM_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetShadeMode)(CELL_GCM_ARGS(uint32_t sm))
{
	(void) sm;
	CELL_GCM_RESERVE(2);
	CELL_GCM_FUNC_RETURN;
}

CELL_GCM_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetTwoSideLightEnable)(CELL_GCM_ARGS(uint32_t enable))
{
	(void) enable;
	CELL_GCM_RESERVE(2);
	CELL_GCM_FUNC_RETURN;
}

CELL_GCM_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetStencilFunc)(CELL_GCM_ARGS(uint32_t func, 
	int32_t ref, uint32_t mask))
{
	(void) func;
	(void) ref;
	(void) mask;
	CELL_GCM_RESERVE(4);
	CELL_GCM_FUNC_RETURN;
}

CELL_GCM_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetBackStencilFunc)(CELL_GCM_ARGS(uint32_t func, 
	int32_t ref, uint32_t mask))
{
	(void) func;
	(void) ref;
	(void) mask;
	CELL_GCM_RESERVE(4);
	CELL_GCM_FUNC_RETURN;
}

CELL_GCM_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetStencilMask)(CELL_GCM_ARGS(uint32_t sm))
{
	(void) sm;
	CELL_GCM_RESERVE(2);
	CELL_GCM_FUNC_RETURN;
}

CELL_GCM_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetBackStencilMask)(CELL_GCM_ARGS(uint32_t sm))
{
	(void) sm;
	CELL_GCM_RESERVE(2);
	CELL_GCM_FUNC_RETURN;
}

CELL_GCM_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetStencilOp)(CELL_GCM_ARGS(uint32_t fail, 
	uint32_t depthFail, uint32_t depthPass))
{
	(void) fail;
	(void) depthFail;
	(void) depthPass;
	CELL_GCM_RESERVE(4);
	CELL_GCM_FUNC_RETURN;
}

CELL_GCM_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetBackStencilOp)(CELL_GCM_ARGS(uint32_t fail, 
	uint32_t depthFail, uint32_t depthPass))
{
	(void) fail;
	(void) depthFail;
	(void) depthPass;
	CELL_GCM_RESERVE(4);
	CELL_GCM_FUNC_RETURN;
}

CELL_GCM_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetZMinMaxControl)(CELL_GCM_ARGS(
	const uint32_t cullNearFarEnable, const uint32_t zclampEnable, const uint32_t cullIgnoreW))
{
	(void) cullNearFarEnable;
	(void) zclampEnable;
	(void) cullIgnoreW;
	CELL_GCM_RESERVE(2);
	CELL_GCM_FUNC_RETURN;
}

CELL_GCM_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetClipMinMax)(CELL_GCM_ARGS(float min, float max))
{
	(void) min;
	(void) max;
	CELL_GCM_RESERVE(3);
	CELL_GCM_FUNC_RETURN;
}

CELL_GCM_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetViewport)(CELL_GCM_ARGS(uint16_t x, 
	uint16_t y, uint16_t w, uint16_t h, float min, float max, const float scale[4], 
	const float offset[4]))
{
	(void) x;
	(void) y;
	(void) w;
	(void) h;
	(void) min;
	(void) max;
	(void) scale;
	(void) offset;
	CELL_GCM_RESERVE(24);
	CELL_GCM_FUNC_RETURN;
}

CELL_GCM_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetAlphaTestEnable)(CELL_GCM_ARGS(uint32_t enable))
{
	(void) enable;
	CELL_GCM_RESERVE(2);
	CELL_GCM_FUNC_RETURN;
}

CELL_GCM_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetBlendEnable)(CELL_GCM_ARGS(uint32_t enable))
{
	(void) enable;
	CELL_GCM_RESERVE(2);
	CELL_GCM_FUNC_RETURN;
}

CELL_GCM_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetBlendEnableMrt)(CELL_GCM_ARGS(uint32_t mrt1, 
	uint32_t mrt2, uint32_t mrt3))
{
	(void) mrt1;
	(void) mrt2;
	(void) mrt3;
	CELL_GCM_RESERVE(2);
	CELL_GCM_FUNC_RETURN;
}

CELL_GCM_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetLogicOpEnable)(CELL_GCM_ARGS(uint32_t enable))
{
	(void) enable;
	CELL_GCM_RESERVE(2);
	CELL_GCM_FUNC_RETURN;
}

CELL_GCM_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetCullFaceEnable)(CELL_GCM_ARGS(uint32_t enable))
{
	(void) enable;
	CELL_GCM_RESERVE(2);
	CELL_GCM_FUNC_RETURN;
}

CELL_GCM_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetDepthBoundsTestEnable)(CELL_GCM_ARGS(uint32_t enable))
{
	(void) enable;
	CELL_GCM_RESERVE(2);
	CELL_GCM_FUNC_RETURN;
}

CELL_GCM_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetDepthTestEnable)(CELL_GCM_ARGS(uint32_t enable))
{
	(void) enable;
	CELL_GCM_RESERVE(2);
	CELL_GCM_FUNC_RETURN;
}

CELL_GCM_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetDitherEnable)(CELL_GCM_ARGS(uint32_t enable))
{
	(void) enable;
	CELL_GCM_RESERVE(2);
	CELL_GCM_FUNC_RETURN;
}

CELL_GCM_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetStencilTestEnable)(CELL_GCM_ARGS(uint32_t enable))
{
	(void) enable;
	CELL_GCM_RESERVE(2);
	CELL_GCM_FUNC_RETURN;
}

CELL_GCM_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetTwoSidedStencilTestEnable)(CELL_GCM_ARGS(uint32_t enable))
{
	(void) enable;
	CELL_GCM_RESERVE(2);
	CELL_GCM_FUNC_RETURN;
}

CELL_GCM_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetPolygonOffsetFillEnable)(CELL_GCM_ARGS(uint32_t enable))
{
	(void) enable;
	CELL_GCM_RESERVE(2);
	CELL_GCM_FUNC_RETURN;
}

CELL_GCM_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetRestartIndexEnable)(CELL_GCM_ARGS(uint32_t enable))
{
	(void) enable;
	CELL_GCM_RESERVE(2);
	CELL_GCM_FUNC_RETURN;
}

CELL_GCM_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetPointSpriteControl)(CELL_GCM_ARGS(
	uint32_t enable, uint32_t rmode, uint32_t texcoord))
{
	(void) enable;
	(void) rmode;
	(void) texcoord;
	CELL_GCM_RESERVE(4);
	CELL_GCM_FUNC_RETURN;
}

CELL_GCM_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetInvalidateTextureCache)(CELL_GCM_ARGS(uint32_t value))
{
	(void) value;
	CELL_GCM_RESERVE(2);
	CELL_GCM_FUNC_RETURN;
}

CELL_GCM_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetTextureBorderColor)(CELL_GCM_ARGS(
	uint8_t index, uint32_t color))
{
	(void) index;
	(void) color;
	CELL_GCM_RESERVE(2);
	CELL_GCM_FUNC_RETURN;
}

CELL_GCM_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetTextureFilter)(CELL_GCM_ARGS(uint8_t index, 
	uint16_t bias, uint8_t min, uint8_t mag, uint8_t conv))
{
	(void) index;
	(void) bias;
	(void) min;
	(void) mag;
	(void) conv;
	CELL_GCM_RESERVE(2);
	CELL_GCM_FUNC_RETURN;
}

CELL_GCM_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetTextureFilterSigned)(CELL_GCM_ARGS(uint8_t index, 
	uint16_t bias, uint8_t min, uint8_t mag, uint8_t conv,
	uint8_t as, uint8_t rs, uint8_t gs, uint8_t bs))
{
	(void) index;
	(void) bias;
	(void) min;
	(void) mag;
	(void) conv;
	(void) as;
	(void) rs;
	(void) gs;
	(void) bs;
	CELL_GCM_RESERVE(2);
	CELL_GCM_FUNC_RETURN;
}

CELL_GCM_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetTextureAddress)(CELL_GCM_ARGS(uint8_t index, 
	uint8_t wraps, uint8_t wrapt, uint8_t wrapr, uint8_t unsignedRemap, 
	uint8_t zfunc, uint8_t gamma))
{
	(void) index;
	(void) wraps;
	(void) wrapt;
	(void) wrapr;
	(void) unsignedRemap;
	(void) zfunc;
	(void) gamma;
	CELL_GCM_RESERVE(2);
	CELL_GCM_FUNC_RETURN;
}

CELL_GCM_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetTextureControl)(CELL_GCM_ARGS(uint8_t index, 
	uint32_t enable, uint16_t minlod, uint16_t maxlod, uint8_t maxaniso))
{
	(void) index;
	(void) enable;
	(void) minlod;
	(void) maxlod;
	(void) maxaniso;
	CELL_GCM_RESERVE(2);
	CELL_GCM_FUNC_RETURN;
}

CELL_GCM_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetTextureOptimization)(CELL_GCM_ARGS(uint8_t index, const uint8_t slope, const uint8_t iso, const uint8_t aniso))
{
	(void) index;
	(void) slope;
	(void) iso;
	(void) aniso;
	CELL_GCM_RESERVE(2);
	CELL_GCM_FUNC_RETURN;
}

CELL_GCM_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetCylindricalWrap)(CELL_GCM_ARGS(uint32_t enable, uint32_t reserved))
{
	(void) enable;
	(void) reserved;
	CELL_GCM_RESERVE(2);
	CELL_GCM_FUNC_RETURN;
}

CELL_GCM_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetTextureBorder)(CELL_GCM_ARGS(uint8_t index, 	
	const CellGcmTexture *texture, uint8_t border))
{	
	(void) index;
	(void) texture;
	(void) border;
	CELL_GCM_RESERVE(9);
	CELL_GCM_FUNC_RETURN;
}

CELL_GCM_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetTexture)(CELL_GCM_ARGS(uint8_t index, 
	const CellGcmTexture *texture))
{
	CELL_GCM_FUNC_CALL(SetTextureBorder)(CELL_GCM_ARGS_FUNC(index, texture, CELL_GCM_TEXTURE_BORDER_COLOR));
	CELL_GCM_FUNC_RETURN;
}

CELL_GCM_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetDrawArrays)(CELL_GCM_ARGS(uint8_t mode, 
	uint32_t first, uint32_t count))
{
	(void) mode;
	(void) first;
	(void) count;

	--count;
	count >>= 8;

	// hw limit 0x7ff loop batches, if count > 256 * 0x7ff
	uint32_t loop, rest;
	loop = count / CELL_GCM_MAX_METHOD_COUNT;
	rest = count % CELL_GCM_MAX_METHOD_COUNT;

	// reserve buffer size
	CELL_GCM_RESERVE(10 + loop*(1+CELL_GCM_MAX_METHOD_COUNT) + (rest!=0 ? 1+rest : 0));
	CELL_GCM_FUNC_RETURN;
}

CELL_GCM_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetDrawIndexArray)(CELL_GCM_ARGS(uint8_t mode, 
	uint32_t count, uint8_t type, uint8_t location, uint32_t indicies))
{
	uint32_t misalignedIndexCount;
	(void) mode;
	(void) location;

	// type == 32
	if(type == CELL_GCM_DRAW_INDEX_ARRAY_TYPE_32)
		misalignedIndexCount = (((indicies + 127) & ~127) - indicies) >> 2;
	// type == 16
	else
		misalignedIndexCount = (((indicies + 127) & ~127) - indicies) >> 1;

	
	CELL_GCM_RESERVE(7);

	if(misalignedIndexCount && (misalignedIndexCount < count)) {
		CELL_GCM_RESERVE(2);
		count -= misalignedIndexCount;
	}

	while(count > 0x7FF00)
	{
		CELL_GCM_RESERVE(1+CELL_GCM_MAX_METHOD_COUNT);
		count -= 0x7ff00;
	}

	CELL_GCM_RESERVE(1+((count + 0xff)>>8));

	CELL_GCM_RESERVE(2);
	CELL_GCM_FUNC_RETURN;
}

CELL_GCM_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetInvalidateVertexCache)(CELL_GCM_NO_ARGS())
{
	CELL_GCM_RESERVE(8);
	CELL_GCM_FUNC_RETURN;
}

CELL_GCM_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetRestartIndex)(CELL_GCM_ARGS(uint32_t index))
{
	(void) index;
	CELL_GCM_RESERVE(2);
	CELL_GCM_FUNC_RETURN;
}

CELL_GCM_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetVertexData4f)(CELL_GCM_ARGS(uint8_t index, const float v[4]))
{
	(void) index;
	(void) v;
	CELL_GCM_RESERVE(5);
	CELL_GCM_FUNC_RETURN;
}

CELL_GCM_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetVertexDataArray)(CELL_GCM_ARGS(uint8_t index, uint16_t frequency, uint8_t stride, uint8_t size, uint8_t type, uint8_t location, uint32_t offset))
{
	(void) index;
	(void) frequency;
	(void) stride;
	(void) size;
	(void) type;
	(void) location;
	(void) offset;
	CELL_GCM_RESERVE(4);
	CELL_GCM_FUNC_RETURN;
}

CELL_GCM_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetFrequencyDividerOperation)(CELL_GCM_ARGS(uint16_t operation))
{
	(void) operation;
	CELL_GCM_RESERVE(2);
	CELL_GCM_FUNC_RETURN;
}

CELL_GCM_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetTransformBranchBits)(CELL_GCM_ARGS(uint32_t branchBits))
{
	(void) branchBits;
	CELL_GCM_RESERVE(2);
	CELL_GCM_FUNC_RETURN;
}

CELL_GCM_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetVertexAttribInputMask)(CELL_GCM_ARGS(uint16_t mask))
{
	(void) mask;
	CELL_GCM_RESERVE(2);
	CELL_GCM_FUNC_RETURN;
}


CELL_GCM_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetVertexProgramParameterBlock)(CELL_GCM_ARGS(uint32_t baseConst, uint32_t constCount, const float * __restrict value))
{
	(void) baseConst;
	(void) value;
	uint32_t blockCount  = (constCount*4) >> 5;		// # 32 blocks
	uint32_t blockRemain = (constCount*4) & 0x1f;		// remainder 

	CELL_GCM_RESERVE(blockCount*34+(blockRemain!=0 ? 2+blockRemain : 0));
	CELL_GCM_FUNC_RETURN;
}

CELL_GCM_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetUpdateFragmentProgramParameter)(CELL_GCM_ARGS(uint32_t offset))
{
	CELL_GCM_FUNC_CALL(SetUpdateFragmentProgramParameterLocation)(CELL_GCM_ARGS_FUNC(offset, CELL_GCM_LOCATION_LOCAL));
	CELL_GCM_FUNC_RETURN;
}

CELL_GCM_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetFragmentProgramGammaEnable)(CELL_GCM_ARGS(uint32_t enable))
{
	(void) enable;
	CELL_GCM_RESERVE(2);
	CELL_GCM_FUNC_RETURN;
}

CELL_GCM_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetUserClipPlaneControl)(CELL_GCM_ARGS(
	uint32_t plane0, uint32_t plane1, uint32_t plane2, uint32_t plane3, 
	uint32_t plane4, uint32_t plane5))
{
	(void) plane0;
	(void) plane1;
	(void) plane2;
	(void) plane3;
	(void) plane4;
	(void) plane5;
	CELL_GCM_RESERVE(2);
	CELL_GCM_FUNC_RETURN;
}

CELL_GCM_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetVertexProgramConstants)(CELL_GCM_ARGS(
	uint32_t first, uint32_t count, const float *data))
{
	(void) first;
	(void) data;
	uint32_t loop = count >> 5;
	uint32_t rest = count & 0x1F;

	CELL_GCM_RESERVE(loop*34+(rest!=0 ? 2+rest : 0));
	CELL_GCM_FUNC_RETURN;
}

CELL_GCM_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetInlineTransfer)(CELL_GCM_ARGS(const uint32_t dstOffset, const void *srcAdr, const uint32_t sizeInWords))
{
	(void) dstOffset;
	(void) srcAdr;
	uint32_t paddedSizeInWords;

	// setup remaining image from cpu blit stuff
	paddedSizeInWords = (sizeInWords + 1) & ~1; // even width only

	CELL_GCM_RESERVE(10+paddedSizeInWords);
	CELL_GCM_FUNC_RETURN;
}

CELL_GCM_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(InlineTransfer)(CELL_GCM_ARGS(const uint32_t dstOffset, const void *srcAdr, const uint32_t sizeInWords, const uint8_t location))
{
	(void) dstOffset;
	(void) srcAdr;
	(void) location;
	uint32_t paddedSizeInWords;

	// setup remaining image from cpu blit stuff
	paddedSizeInWords = (sizeInWords + 1) & ~1; // even width only

	CELL_GCM_RESERVE(12+paddedSizeInWords);
	CELL_GCM_FUNC_RETURN;
}

CELL_GCM_DECL CELL_GCM_FUNC_TYPE_UINT32 CELL_GCM_FUNC(SetTransferImage)(CELL_GCM_ARGS(
	uint8_t mode, uint32_t dstOffset, uint32_t dstPitch, uint32_t dstX, 
	uint32_t dstY, uint32_t srcOffset, uint32_t srcPitch, uint32_t srcX, 
	uint32_t srcY, uint32_t width, uint32_t height, uint32_t bytesPerPixel))
{
	(void) mode;
	(void) dstOffset;
	(void) dstPitch;
	(void) srcOffset;
	(void) srcPitch;
	(void) srcX;
	(void) srcY;
	(void) bytesPerPixel;

	uint32_t BLOCKSIZE = 1 << 10;
	uint32_t x;
	uint32_t y;
	uint32_t finalDstX;
	uint32_t finalDstY;

	CELL_GCM_RESERVE(6);

	// split large blits
	finalDstX = dstX + width;
	finalDstY = dstY + height;
	for(y = dstY; y < finalDstY;)
	{
		// determine this blits height
		uint32_t dstTop = y & ~(BLOCKSIZE - 1);
		uint32_t dstBot = dstTop + BLOCKSIZE;
		uint32_t dstBltHeight = ((dstBot<finalDstY) ? dstBot : finalDstY) - y;
		for(x = dstX; x < finalDstX;)
		{
			// determine this blits width
			uint32_t dstLeft = x & ~(BLOCKSIZE - 1);
			uint32_t dstRight = dstLeft + BLOCKSIZE;
			uint32_t dstBltWidth = ((dstRight<finalDstX) ? dstRight : finalDstX) - x;

			CELL_GCM_RESERVE(20);

			x += dstBltWidth;
		}
		y += dstBltHeight;
	}

	CELL_GCM_FUNC_RETURN_CELL_OK;
}

CELL_GCM_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(TransferData)(CELL_GCM_ARGS(uint32_t dstOffset, int32_t dstPitch, uint32_t srcOffset, int32_t srcPitch, int32_t bytesPerRow, int32_t rowCount))
{
	int32_t CL0039_MIN_PITCH = -32768;
	int32_t CL0039_MAX_PITCH = 32767;
	int32_t CL0039_MAX_ROWS = 0x7ff;
	uint32_t CL0039_MAX_LINES = 0x3fffff;
	uint32_t colCount;
	uint32_t rows;
	uint32_t cols;

	// can we turn this into a contigous blit ?
	if ((srcPitch == bytesPerRow) && (dstPitch == bytesPerRow))
	{
		bytesPerRow *= rowCount;
		rowCount = 1;
		srcPitch = 0;
		dstPitch = 0;
	}

	// unusual pitch values
	if ((srcPitch < CL0039_MIN_PITCH) || (srcPitch > CL0039_MAX_PITCH) ||
		(dstPitch < CL0039_MIN_PITCH) || (dstPitch > CL0039_MAX_PITCH))
	{
		// fallback: blit per line (could improve this case)
		// Blit one line at a time
		while(--rowCount >= 0)
		{
			for(colCount = bytesPerRow; colCount>0; colCount -= cols)
			{
				// clamp to limit
				cols = (colCount > CL0039_MAX_LINES) ? CL0039_MAX_LINES : colCount;

				// do the blit
				CELL_GCM_RESERVE(9);
			}

			dstOffset += dstPitch;
			srcOffset += srcPitch;
		}
	}
	else
	{
		// for each batch of rows
		for(;rowCount>0; rowCount -= rows)
		{
			// clamp to limit ?
			rows = (rowCount > CL0039_MAX_ROWS) ? CL0039_MAX_ROWS : rowCount;

			// for each batch of cols
			for(colCount = bytesPerRow; colCount>0; colCount -= cols)
			{
				// clamp to limit
				cols = (colCount > CL0039_MAX_LINES) ? CL0039_MAX_LINES : colCount;

				// do the blit
				CELL_GCM_RESERVE(9);
			}

			// Advance to next set of rows
			srcOffset += rows * srcPitch;
			dstOffset += rows * dstPitch;
		}
	}

	CELL_GCM_RESERVE(2);
	CELL_GCM_FUNC_RETURN;
}

CELL_GCM_DECL CELL_GCM_FUNC_TYPE_UINT32 CELL_GCM_FUNC(SetTransferData)(CELL_GCM_ARGS(
	uint8_t mode, uint32_t dstOffset, uint32_t dstPitch, uint32_t srcOffset, 
	uint32_t srcPitch, uint32_t bytesPerRow, uint32_t rowCount))
{
	(void) mode;
	CELL_GCM_RESERVE(3);
	CELL_GCM_FUNC_CALL(TransferData)(CELL_GCM_ARGS_FUNC(dstOffset, dstPitch, srcOffset, srcPitch, bytesPerRow, rowCount));

	CELL_GCM_FUNC_RETURN_CELL_OK;
}

CELL_GCM_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetRenderEnable)(CELL_GCM_ARGS(uint8_t mode, uint32_t index))
{
	(void) index;
	if(mode == CELL_GCM_CONDITIONAL)
	{
		CELL_GCM_RESERVE(4);
	}
	// mode == CELL_GCM_TRUE, CELL_GCM_FALSE
	else
	{
		CELL_GCM_RESERVE(2);
	}
	CELL_GCM_FUNC_RETURN;
}

CELL_GCM_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetZpassPixelCountEnable)(CELL_GCM_ARGS(uint32_t enable))
{
	(void) enable;
	CELL_GCM_RESERVE(2);
	CELL_GCM_FUNC_RETURN;
}

CELL_GCM_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetClearReport)(CELL_GCM_ARGS(uint32_t type))
{
	(void) type;
	CELL_GCM_RESERVE(2);
	CELL_GCM_FUNC_RETURN;
}

CELL_GCM_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetReport)(CELL_GCM_ARGS(uint32_t type, uint32_t index))
{
	(void) type;
	(void) index;
	CELL_GCM_RESERVE(2);
	CELL_GCM_FUNC_RETURN;
}


CELL_GCM_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetZcullStatsEnable)(CELL_GCM_ARGS(uint32_t enable))
{
	(void) enable;
	CELL_GCM_RESERVE(2);
	CELL_GCM_FUNC_RETURN;
}

CELL_GCM_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetZcullControl)(CELL_GCM_ARGS(const uint8_t zCullDir, const uint8_t zCullFormat))
{
	(void) zCullDir;
	(void) zCullFormat;
	CELL_GCM_RESERVE(2);
	CELL_GCM_FUNC_RETURN;
}

CELL_GCM_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetZcullLimit)(CELL_GCM_ARGS(uint16_t moveForwardLimit, uint16_t pushBackLimit))
{
	(void) moveForwardLimit;
	(void) pushBackLimit;
	CELL_GCM_RESERVE(2);
	CELL_GCM_FUNC_RETURN;
}

CELL_GCM_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetScullControl)(CELL_GCM_ARGS(const uint8_t sFunc, const uint8_t sRef, const uint8_t sMask))
{
	(void) sFunc;
	(void) sRef;
	(void) sMask;
	CELL_GCM_RESERVE(2);
	CELL_GCM_FUNC_RETURN;
}

CELL_GCM_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetConvertSwizzleFormat)(CELL_GCM_ARGS(
	uint32_t dstOffset, uint32_t dstWidth, uint32_t dstHeight, uint32_t dstX, uint32_t dstY,
	uint32_t srcOffset, uint32_t srcPitch, uint32_t srcX, uint32_t srcY, uint32_t width, uint32_t height, uint32_t bytesPerPixel,
	uint8_t mode))
{
	(void) mode;

    uint32_t NV_MEM2MEM_MAX_HEIGHT_VALUE = 2047;
    uint32_t NV_SURFACE_SWIZZLED_MAX_DIM = 10;

#ifdef __SPU__
	uint32_t dstwlog2 = 31 - ({__asm__("clz %0,%1" : "=r" (dstwlog2) : "r" (__builtin_si_from_uint(dstWidth))); dstwlog2;});
	uint32_t dsthlog2 = 31 - ({__asm__("clz %0,%1" : "=r" (dsthlog2) : "r" (__builtin_si_from_uint(dstHeight))); dsthlog2;});
#endif
#ifdef __PPU__
#ifdef __SNC__
	uint32_t dstwlog2 = 31 - __cntlzw(dstWidth);
	uint32_t dsthlog2 = 31 - __cntlzw(dstHeight);
#else
	uint32_t dstwlog2 = 31 - ({__asm__("cntlzw %0,%1" : "=r" (dstwlog2) : "r" (dstWidth)); dstwlog2;});
	uint32_t dsthlog2 = 31 - ({__asm__("cntlzw %0,%1" : "=r" (dsthlog2) : "r" (dstHeight)); dsthlog2;});
#endif
#endif
#if defined(__linux__) || defined(WIN32)
	uint32_t n,i;

	n = dstWidth;
	for(i=1; (n >> i) > 0; i++){}
	uint32_t dstwlog2 = i-1;

	n = dstHeight;
	for(i=1; (n >> i) > 0; i++){}
	uint32_t dsthlog2 = i-1;
#endif

    switch (bytesPerPixel)
    {
        case 2:
        case 4:
            break;
        case 8:
            dstWidth <<= 1;
            dstX <<= 1;
            srcX <<= 1;
            width <<= 1;
            bytesPerPixel >>= 1;
            dstwlog2 += 1;
            break;
        case 16:
            dstWidth <<= 2;
            dstX <<= 2;
            srcX <<= 2;
            width <<= 2;
            bytesPerPixel >>= 2;
            dstwlog2 += 2;
            break;
        default:
            break;
    }

    if ((dstwlog2 <= 1) || (dsthlog2 == 0))
    {
		// set src/dst location
		CELL_GCM_RESERVE(3);

        uint32_t dstPitch;
        uint32_t linesLeft;

        // get rid of src/dst position
		dstPitch = bytesPerPixel << dstwlog2;
		srcOffset = srcOffset + srcX * bytesPerPixel + srcY * srcPitch;
		dstOffset = dstOffset + dstX * bytesPerPixel + dstY * dstPitch;

        // MEM2MEM maximum height is 2047..
        for(linesLeft = height; linesLeft;)
        {
            // actualHeight = min(NV_MEM2MEM_MAX_HEIGHT_VALUE, linesLeft);
            uint32_t actualHeight = (linesLeft > NV_MEM2MEM_MAX_HEIGHT_VALUE)
                               ?  NV_MEM2MEM_MAX_HEIGHT_VALUE
                               :  linesLeft;

			// todo: this is incorrect for the vid->vid case
			CELL_GCM_FUNC_CALL(TransferData)(CELL_GCM_ARGS_FUNC(dstOffset, dstPitch, srcOffset, srcPitch, width*bytesPerPixel, actualHeight));

            srcOffset = srcOffset + actualHeight * srcPitch;
            dstOffset = dstOffset + actualHeight * dstPitch;
            linesLeft -= actualHeight;
        }
        CELL_GCM_FUNC_RETURN;
    }
    else
    {
		// set src/dst location
		CELL_GCM_RESERVE(6);

        uint32_t yTop;
        uint32_t xEnd;
        uint32_t yEnd;
        uint32_t x;
        uint32_t y;

        // align the Src Blt to the Dst, that way we can forget about srcX and srcY.
        srcOffset += (srcX - dstX) * bytesPerPixel + (srcY - dstY) * srcPitch;

        // blit limits
        xEnd = dstX + width;
        yEnd = dstY + height;

        // For the top row of blocks, yTop != y
        yTop = dstY & ~((1 << NV_SURFACE_SWIZZLED_MAX_DIM) - 1);
        for(y = dstY; y < yEnd;)
        {
            uint32_t xLeft;
            uint32_t yBottom;

            // determine actual copy height for this iteration
            yBottom = yTop + (1 << NV_SURFACE_SWIZZLED_MAX_DIM);
            if(yBottom > (1ul << dsthlog2))
            {
                yBottom = (1 << dsthlog2);
            }

            // for the left column of blocks, xLeft != x
            xLeft = dstX & ~((1 << NV_SURFACE_SWIZZLED_MAX_DIM) - 1);
            for(x = dstX; x < xEnd;)
            {
                uint32_t xRight;

                // determine actual copy width for this iteration
                xRight = xLeft + (1 << NV_SURFACE_SWIZZLED_MAX_DIM);

                // set dst format/offset
				CELL_GCM_RESERVE(18);

                // increment in X
                x = xLeft = xRight;
            }
        
            // increment in Y
            y = yTop = yBottom;
        }
    }

	CELL_GCM_FUNC_RETURN;
}

CELL_GCM_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetVertexTexture)(CELL_GCM_ARGS(const uint8_t index, const CellGcmTexture *texture))
{
	(void) index;
	(void) texture;
	CELL_GCM_RESERVE(7);
	CELL_GCM_FUNC_RETURN;
}

CELL_GCM_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetVertexTextureAddress)(CELL_GCM_ARGS(const uint8_t index, const uint8_t wraps, const uint8_t wrapt))
{
	(void) index;
	(void) wraps;
	(void) wrapt;
	CELL_GCM_RESERVE(2);
	CELL_GCM_FUNC_RETURN;
}

CELL_GCM_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetVertexTextureFilter)(CELL_GCM_ARGS(const uint8_t index, const uint16_t bias))
{
	(void) index;
	(void) bias;
	CELL_GCM_RESERVE(2);
	CELL_GCM_FUNC_RETURN;
}

CELL_GCM_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetVertexTextureControl)(CELL_GCM_ARGS(const uint8_t index, const uint32_t enable, const uint16_t minLod, const uint16_t maxLod))
{
	(void) index;
	(void) enable;
	(void) minLod;
	(void) maxLod;
	CELL_GCM_RESERVE(2);
	CELL_GCM_FUNC_RETURN;
}

CELL_GCM_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetVertexTextureBorderColor)(CELL_GCM_ARGS(const uint8_t index, const uint32_t color))
{
	(void) index;
	(void) color;
	CELL_GCM_RESERVE(2);
	CELL_GCM_FUNC_RETURN;
}

CELL_GCM_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetPerfMonTrigger)(CELL_GCM_NO_ARGS())
{
	CELL_GCM_RESERVE(4);
	CELL_GCM_FUNC_RETURN;
}

CELL_GCM_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetDrawInlineArray)(CELL_GCM_ARGS(const uint8_t mode, const uint32_t count, const void *data))
{
	(void) mode;
	(void) data;
	uint32_t loop = count / CELL_GCM_MAX_METHOD_COUNT;
	uint32_t rest = count % CELL_GCM_MAX_METHOD_COUNT;

	CELL_GCM_RESERVE(8+loop*(1+CELL_GCM_MAX_METHOD_COUNT)+(rest!=0 ? 1+rest : 0));
	CELL_GCM_FUNC_RETURN;
}

CELL_GCM_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetDrawInlineIndexArray32)(CELL_GCM_ARGS(const uint8_t mode, const uint32_t start, const uint32_t count, const uint32_t *data))
{
	(void) mode;
	(void) start;
	(void) data;
	uint32_t loop = count / CELL_GCM_MAX_METHOD_COUNT;
	uint32_t rest = count % CELL_GCM_MAX_METHOD_COUNT;

	// reserve word size
	CELL_GCM_RESERVE(8+loop*(1+CELL_GCM_MAX_METHOD_COUNT)+(rest!=0 ? 1+rest : 0));
	CELL_GCM_FUNC_RETURN;
}

CELL_GCM_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetDrawInlineIndexArray16)(CELL_GCM_ARGS(const uint8_t mode, const uint32_t start, const uint32_t count, const uint16_t *data))
{
	(void) mode;
	(void) start;
	(void) data;
	uint32_t odd;
	uint32_t lcount;

	if(count & 1){	// odd count
		odd = 1;
		lcount = count - 1;
	}
	else{			// even count
		odd = 0;
		lcount = count;
	}

	uint32_t loop = (lcount>>1) / CELL_GCM_MAX_METHOD_COUNT;
	uint32_t rest = (lcount>>1) % CELL_GCM_MAX_METHOD_COUNT;

	// reserve word size
	CELL_GCM_RESERVE(8 + odd*2 + loop*(1+CELL_GCM_MAX_METHOD_COUNT)+(rest!=0 ? 1+rest : 0));
	CELL_GCM_FUNC_RETURN;
}

CELL_GCM_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetFogMode)(CELL_GCM_ARGS(const uint32_t mode))
{
	(void) mode;
	CELL_GCM_RESERVE(2);
	CELL_GCM_FUNC_RETURN;
}

CELL_GCM_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetFogParams)(CELL_GCM_ARGS(const float p0, const float p1))
{
	(void) p0;
	(void) p1;
	CELL_GCM_RESERVE(3);
	CELL_GCM_FUNC_RETURN;
}

CELL_GCM_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetTransferLocation)(CELL_GCM_ARGS(const uint32_t location))
{
	(void) location;
	CELL_GCM_RESERVE(2);
	CELL_GCM_FUNC_RETURN;
}

CELL_GCM_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetDepthFormat)(CELL_GCM_ARGS(const uint32_t format))
{
	(void) format;
	CELL_GCM_RESERVE(2);
	CELL_GCM_FUNC_RETURN;
}

CELL_GCM_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetBlendOptimization)(CELL_GCM_ARGS(const uint32_t enable))
{
	(void) enable;
	CELL_GCM_RESERVE(2);
	CELL_GCM_FUNC_RETURN;
}

CELL_GCM_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetPolygonOffsetLineEnable)(CELL_GCM_ARGS(const uint32_t enable))
{
	(void) enable;
	CELL_GCM_RESERVE(2);
	CELL_GCM_FUNC_RETURN;
}

CELL_GCM_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetVertexAttribOutputMask)(CELL_GCM_ARGS(const uint32_t mask))
{
	(void) mask;
	CELL_GCM_RESERVE(2);
	CELL_GCM_FUNC_RETURN;
}

CELL_GCM_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetTextureRemap)(CELL_GCM_ARGS(const uint8_t index, const uint32_t remap))
{   
	(void) index;
	(void) remap;
	CELL_GCM_RESERVE(2);
	CELL_GCM_FUNC_RETURN;
}

CELL_GCM_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetVertexProgramLoadSlot)(CELL_GCM_ARGS(const uint32_t loadSlot, const uint32_t instCount, const void *ucode))
{
	(void) loadSlot;
	(void) ucode;
	uint32_t loop, rest;
	loop = instCount / 8;
	rest = (instCount % 8) * 4;
	CELL_GCM_RESERVE(2 + loop*33 + (rest!=0 ? rest+1 : 0));
	CELL_GCM_FUNC_RETURN;
}

CELL_GCM_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetVertexProgramStartSlot)(CELL_GCM_ARGS(const uint32_t startSlot))
{
	(void) startSlot;
	CELL_GCM_RESERVE(2);
	CELL_GCM_FUNC_RETURN;
}

CELL_GCM_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetVertexProgramRegisterCount)(CELL_GCM_ARGS(const uint32_t registerCount))
{
	(void) registerCount;
	CELL_GCM_RESERVE(2);
	CELL_GCM_FUNC_RETURN;
}

CELL_GCM_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetVertexProgramLoad)(CELL_GCM_ARGS(const CellCgbVertexProgramConfiguration *conf, const void *ucode))
{
	(void) ucode;
	uint32_t instCount = conf->instructionCount;
	uint32_t loop, rest;
	loop = instCount / 8;
	rest = (instCount % 8) * 4;
	CELL_GCM_RESERVE(7 + loop*33 + (rest!=0 ? rest+1 : 0));
	CELL_GCM_FUNC_RETURN;
}

CELL_GCM_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetFragmentProgramLoad)(CELL_GCM_ARGS(const CellCgbFragmentProgramConfiguration *conf))
{
	CELL_GCM_FUNC_CALL(SetFragmentProgramLoadLocation)(CELL_GCM_ARGS_FUNC(conf, CELL_GCM_LOCATION_LOCAL));
	CELL_GCM_FUNC_RETURN;
}

CELL_GCM_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetInlineTransferPointer)(CELL_GCM_ARGS(const uint32_t offset, const uint32_t count, void **pointer))
{
	(void) offset;
	(void) pointer;
	uint32_t evenCount = (count + 1) & ~1;	// even width only

	CELL_GCM_RESERVE(10 + evenCount);
	CELL_GCM_FUNC_RETURN;
}

CELL_GCM_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetTransferDataMode)(CELL_GCM_ARGS(const uint8_t mode))
{
	(void) mode;
	CELL_GCM_RESERVE(3);
	CELL_GCM_FUNC_RETURN;
}

CELL_GCM_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetTransferDataFormat)(CELL_GCM_ARGS(const int32_t inPitch, const int32_t outPitch, const uint32_t lineLength, const uint32_t lineCount, const uint8_t inFormat, const uint8_t outFormat))
{
	(void) inPitch;
	(void) outPitch;
	(void) lineLength;
	(void) lineCount;
	(void) inFormat;
	(void) outFormat;
	CELL_GCM_RESERVE(6);
	CELL_GCM_FUNC_RETURN;
}

CELL_GCM_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetTransferDataOffset)(CELL_GCM_ARGS(const uint32_t dstOffset, const uint32_t srcOffset))
{
	(void) dstOffset;
	(void) srcOffset;
	CELL_GCM_RESERVE(6+2);
	CELL_GCM_FUNC_RETURN;
}

CELL_GCM_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetTransferScaleMode)(CELL_GCM_ARGS(const uint8_t mode, const uint8_t surface))
{
	(void) mode;
	(void) surface;
	CELL_GCM_RESERVE(6);
	CELL_GCM_FUNC_RETURN;
}

CELL_GCM_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetTransferScaleSurface)(CELL_GCM_ARGS(const CellGcmTransferScale *scale, CellGcmTransferSurface *surface))
{
	(void) scale;
	(void) surface;
	CELL_GCM_RESERVE(20);
	CELL_GCM_FUNC_RETURN;
}

CELL_GCM_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetTransferScaleSwizzle)(CELL_GCM_ARGS(const CellGcmTransferScale *scale, CellGcmTransferSwizzle *swizzle))
{
	(void) scale;
	(void) swizzle;
	CELL_GCM_RESERVE(18);
	CELL_GCM_FUNC_RETURN;
}

CELL_GCM_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetDrawBegin)(CELL_GCM_ARGS(const uint8_t mode))
{
	(void) mode;
	CELL_GCM_RESERVE(6);
	CELL_GCM_FUNC_RETURN;
}

CELL_GCM_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetDrawEnd)(CELL_GCM_NO_ARGS())
{
	CELL_GCM_RESERVE(2);
	CELL_GCM_FUNC_RETURN;
}

CELL_GCM_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetDrawInlineArrayPointer)(CELL_GCM_ARGS(const uint32_t count, void **pointer))
{
	(void) count;
	(void) pointer;
	CELL_GCM_RESERVE(count+1);
	CELL_GCM_FUNC_RETURN;
}

CELL_GCM_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetVertexProgramConstantsPointer)(CELL_GCM_ARGS(uint32_t first, uint32_t count, void **pointer))
{
	(void) first;
	(void) count;
	(void) pointer;
	CELL_GCM_RESERVE(count+2);
	CELL_GCM_FUNC_RETURN;
}

CELL_GCM_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetDrawInlineIndexArray32Pointer)(CELL_GCM_ARGS(const uint32_t count, void **pointer))
{
	(void) count;
	(void) pointer;
	CELL_GCM_RESERVE(count+1);
	CELL_GCM_FUNC_RETURN;
}

CELL_GCM_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetDrawInlineIndexArray16Pointer)(CELL_GCM_ARGS(const uint32_t count, void **pointer))
{
	(void) count;
	(void) pointer;
	CELL_GCM_RESERVE(count+1);
	CELL_GCM_FUNC_RETURN;
}

CELL_GCM_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetAnisoSpread)(CELL_GCM_ARGS(const uint8_t index, const uint8_t reduceSamplesEnable, const uint8_t hReduceSamplesEnable, const uint8_t vReduceSamplesEnable, const uint8_t spacingSelect, const uint8_t hSpacingSelect, const uint8_t vSpacingSelect))
{
	(void) index;
	(void) reduceSamplesEnable;
	(void) hReduceSamplesEnable;
	(void) vReduceSamplesEnable;
	(void) spacingSelect;
	(void) hSpacingSelect;
	(void) vSpacingSelect;
	CELL_GCM_RESERVE(2);
	CELL_GCM_FUNC_RETURN;
}

CELL_GCM_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetTextureAddressAnisoBias)(CELL_GCM_ARGS(uint8_t index, 
	uint8_t wraps, uint8_t wrapt, uint8_t wrapr, uint8_t unsignedRemap, 
	uint8_t zfunc, uint8_t gamma, uint8_t anisoBias))
{
	(void) index;
	(void) wraps;
	(void) wrapt;
	(void) wrapr;
	(void) unsignedRemap;
	(void) zfunc;
	(void) gamma;
	(void) anisoBias;
	CELL_GCM_RESERVE(2);
	CELL_GCM_FUNC_RETURN;
}

CELL_GCM_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetVertexProgramParameterBlockPointer)(CELL_GCM_ARGS(uint32_t baseConst, uint32_t constCount, void **pointer))
{
	(void) baseConst;
	(void) pointer;
	CELL_GCM_RESERVE(constCount*4+2);
	CELL_GCM_FUNC_RETURN;
}

CELL_GCM_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetVertexDataArrayFormat)(CELL_GCM_ARGS(const uint8_t index, const uint16_t frequency, const uint8_t stride, const uint8_t size, const uint8_t type))
{
	(void) index;
	(void) frequency;
	(void) stride;
	(void) size;
	(void) type;
	CELL_GCM_RESERVE(2);
	CELL_GCM_FUNC_RETURN;
}

CELL_GCM_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetVertexDataArrayOffset)(CELL_GCM_ARGS(const uint8_t index, const uint8_t location, const uint32_t offset))
{
	(void) index;
	(void) location;
	(void) offset;
	CELL_GCM_RESERVE(2);
	CELL_GCM_FUNC_RETURN;
}

#if (CELL_GCM_MEASURE != 2)
CELL_GCM_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetInlineTransferAlignedPointer)(CELL_GCM_ARGS(const uint32_t offset, const uint32_t count, void **pointer))
{
	uint32_t nopCount = 4 - ((((uint32_t)CELL_GCM_CURRENT + 40) & 15) >> 2);	// 4 to 1
	if(nopCount != 4){	// not aligned
		CELL_GCM_FUNC_CALL(SetNopCommand)(CELL_GCM_ARGS_FUNC(nopCount));
	}
	CELL_GCM_FUNC_CALL(SetInlineTransferPointer)(CELL_GCM_ARGS_FUNC(offset, count, pointer));
	CELL_GCM_FUNC_RETURN;
}

CELL_GCM_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetVertexProgramConstantsAlignedPointer)(CELL_GCM_ARGS(uint32_t first, uint32_t count, void **pointer))
{
	uint32_t nopCount = 4 - ((((uint32_t)CELL_GCM_CURRENT + 8) & 15) >> 2);	// 4 to 1
	if(nopCount != 4){	// not aligned
		CELL_GCM_FUNC_CALL(SetNopCommand)(CELL_GCM_ARGS_FUNC(nopCount));
	}
	CELL_GCM_FUNC_CALL(SetVertexProgramConstantsPointer)(CELL_GCM_ARGS_FUNC(first, count, pointer));
	CELL_GCM_FUNC_RETURN;
}

CELL_GCM_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetVertexProgramParameterBlockAlignedPointer)(CELL_GCM_ARGS(const uint32_t baseConst, const uint32_t constCount, void **pointer))
{
	uint32_t nopCount = 4 - ((((uint32_t)CELL_GCM_CURRENT + 8) & 15) >> 2);	// 4 to 1
	if(nopCount != 4){	// not aligned
		CELL_GCM_FUNC_CALL(SetNopCommand)(CELL_GCM_ARGS_FUNC(nopCount));
	}
	CELL_GCM_FUNC_CALL(SetVertexProgramParameterBlockPointer)(CELL_GCM_ARGS_FUNC(baseConst, constCount, pointer));
	CELL_GCM_FUNC_RETURN;
}

CELL_GCM_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetDrawInlineArrayAlignedPointer)(CELL_GCM_ARGS(const uint32_t count, void **pointer))
{
	uint32_t nopCount = 4 - ((((uint32_t)CELL_GCM_CURRENT + 4) & 15) >> 2);	// 4 to 1
	if(nopCount != 4){	// not aligned
		CELL_GCM_FUNC_CALL(SetNopCommand)(CELL_GCM_ARGS_FUNC(nopCount));
	}
	CELL_GCM_FUNC_CALL(SetDrawInlineArrayPointer)(CELL_GCM_ARGS_FUNC(count, pointer));
	CELL_GCM_FUNC_RETURN;
}

CELL_GCM_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetDrawInlineIndexArray32AlignedPointer)(CELL_GCM_ARGS(const uint32_t count, void **pointer))
{
	uint32_t nopCount = 4 - ((((uint32_t)CELL_GCM_CURRENT + 4) & 15) >> 2);	// 4 to 1
	if(nopCount != 4){	// not aligned
		CELL_GCM_FUNC_CALL(SetNopCommand)(CELL_GCM_ARGS_FUNC(nopCount));
	}
	CELL_GCM_FUNC_CALL(SetDrawInlineIndexArray32Pointer)(CELL_GCM_ARGS_FUNC(count, pointer));
	CELL_GCM_FUNC_RETURN;
}

CELL_GCM_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetDrawInlineIndexArray16AlignedPointer)(CELL_GCM_ARGS(const uint32_t count, void **pointer))
{
	uint32_t nopCount = 4 - ((((uint32_t)CELL_GCM_CURRENT + 4) & 15) >> 2);	// 4 to 1
	if(nopCount != 4){ // not aligned
		CELL_GCM_FUNC_CALL(SetNopCommand)(CELL_GCM_ARGS_FUNC(nopCount));
	}
	CELL_GCM_FUNC_CALL(SetDrawInlineIndexArray16Pointer)(CELL_GCM_ARGS_FUNC(count, pointer));
	CELL_GCM_FUNC_RETURN;
}
#endif	// (CELL_GCM_MEASURE != 2)

CELL_GCM_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetFragmentProgramLoadLocation)(CELL_GCM_ARGS(const CellCgbFragmentProgramConfiguration *conf, const uint32_t location))
{
	(void) location;

	CELL_GCM_RESERVE(4);

	uint16_t texMask = conf->texCoordsInputMask;
	for(uint16_t i=0; texMask; i++)
	{
		if (texMask&1)
		{
			CELL_GCM_RESERVE(2);
		}
		texMask >>= 1;
	}
	CELL_GCM_RESERVE(2);
	CELL_GCM_FUNC_RETURN;
}

CELL_GCM_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetUpdateFragmentProgramParameterLocation)(CELL_GCM_ARGS(const uint32_t offset, const uint32_t location))
{
	(void) offset;
	(void) location;
	CELL_GCM_RESERVE(2);
	CELL_GCM_FUNC_RETURN;
}

CELL_GCM_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetVertexDataBase)(CELL_GCM_ARGS(const uint32_t baseOffset, const uint32_t baseIndex))
{
	(void) baseOffset;
	(void) baseIndex;
	CELL_GCM_RESERVE(3);
	CELL_GCM_FUNC_RETURN;
}

CELL_GCM_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetVertexProgramParameter)(CELL_GCM_ARGS(const CGparameter param, const float *value))
{
	(void) value;
	CgBinaryParameter *p = (CgBinaryParameter*) param;

	if (p->res == CG_UNDEFINED)
		CELL_GCM_FUNC_RETURN;

	switch (p->type)
	{
	case CG_FLOAT:
	case CG_FLOAT1:
	case CG_FLOAT2:
	case CG_FLOAT3:
	case CG_FLOAT4:
		{
			if (p->resIndex != -1) {
				CELL_GCM_RESERVE(6);
			}
		}
		break;

	case CG_FLOAT3x4:
		{
			{
				p++;
				for (int cnt = 0; cnt < 3; cnt++,p++) 
				{
					if ((p->res != CG_UNDEFINED) && (p->resIndex != -1)) 
					{
						CELL_GCM_RESERVE(6);
					}
				}
			}
		}
		break;

	case CG_FLOAT4x4:
		{
			if (((p+1)->res != CG_UNDEFINED) 
				&& ((p+2)->res != CG_UNDEFINED) 
				&& ((p+3)->res != CG_UNDEFINED) 
				&& ((p+4)->res != CG_UNDEFINED)) 
			{
				if (((p+1)->resIndex != -1)
					&& ((p+2)->resIndex != -1)
					&& ((p+3)->resIndex != -1)
					&& ((p+4)->resIndex != -1)) {
					CELL_GCM_RESERVE(18);
				}
			}
			else 
			{
				p++;
				for (int cnt = 0; cnt < 4; cnt++, p++) 
				{
					if ((p->res != CG_UNDEFINED) && (p->resIndex != -1)) 
					{
						CELL_GCM_RESERVE(6);
					}
				}
			}
		}
		break;

	case CG_FLOAT3x3:
		{
			{
				p++;
				for (int cnt = 0; cnt < 3; cnt++, p++) 
				{
					if (((p)->res != CG_UNDEFINED) && (p->resIndex != -1)) 
					{
						CELL_GCM_RESERVE(6);
					}
				}
			}
		}
		break;

	case CG_FLOAT4x3:
		{
			{
				p++;
				for (int cnt = 0; cnt < 4; cnt++,p++) 
				{
					if (((p)->res != CG_UNDEFINED) && (p->resIndex != -1)) 
					{
						CELL_GCM_RESERVE(6);
					}
				}
			}
		}
		break;

	default:
		break;
	}
	CELL_GCM_FUNC_RETURN;
}

CELL_GCM_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetVertexProgram)(CELL_GCM_ARGS(const CGprogram prog, const void *ucode))
{
	(void) ucode;

	const CgBinaryProgram *vs;
	CgBinaryVertexProgram *binaryVertexProgram;
	const CgBinaryParameter *param;
	uint32_t paramCount;
	uint32_t instCount;

	vs = (const CgBinaryProgram*) prog;
	binaryVertexProgram = (CgBinaryVertexProgram*) ((char*)vs + vs->program);
	instCount = binaryVertexProgram->instructionCount;

	uint32_t loop, rest;
	loop = instCount / 8;
	rest = (instCount % 8) * 4;
	CELL_GCM_RESERVE(7 + loop*33 + (rest!=0 ? rest+1 : 0));

	param = (const CgBinaryParameter *)((const char *)vs + vs->parameterArray);
	for(paramCount = vs->parameterCount; paramCount-- > 0;)
	{
		if (param->defaultValue &&
			((param->var == CG_CONSTANT) || (param->var == CG_UNIFORM)))
		{
			//Second argument: NULL -> 0
			CELL_GCM_FUNC_CALL(SetVertexProgramParameter)(CELL_GCM_ARGS_FUNC((CGparameter)param, 0));
		}
		++param;
	}
	CELL_GCM_FUNC_RETURN;
}

CELL_GCM_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetFragmentProgramOffset)(CELL_GCM_ARGS(const CGprogram prog, const uint32_t offset, const uint32_t location))
{
	const CgBinaryProgram *ps = (const CgBinaryProgram*) prog;
	CgBinaryFragmentProgram *binaryFragmentProgram;
	uint32_t texMask;
	uint32_t i;

	(void) offset;
	(void) location;

	binaryFragmentProgram = (CgBinaryFragmentProgram*) ((char*)ps +  ps->program);

	CELL_GCM_RESERVE(4);

	// deal with the 2d opt
	texMask = binaryFragmentProgram->texCoordsInputMask;
	for(i=0; texMask; i++)
	{
		if (texMask&1) 
		{
			CELL_GCM_RESERVE(2);
		}
		texMask >>= 1;
	}
	CELL_GCM_FUNC_RETURN;
}

CELL_GCM_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetFragmentProgramControl)(CELL_GCM_ARGS(const CGprogram prog, const uint8_t controlTxp, const uint8_t reserved0, const uint8_t reserved1))
{
	(void) prog;
	(void) controlTxp;
	(void) reserved0;
	(void) reserved1;

	CELL_GCM_RESERVE(2);
	CELL_GCM_FUNC_RETURN;
}

CELL_GCM_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetFragmentProgram)(CELL_GCM_ARGS(const CGprogram prog, uint32_t offset))
{
	const CgBinaryProgram *ps = (const CgBinaryProgram*) prog;
	CgBinaryFragmentProgram *binaryFragmentProgram;
	uint32_t texMask;
	uint32_t i;

	(void) offset;

	binaryFragmentProgram = (CgBinaryFragmentProgram*) ((char*)ps +  ps->program);

	CELL_GCM_RESERVE(6);

	// deal with the 2d opt
	texMask = binaryFragmentProgram->texCoordsInputMask;
	for(i=0; texMask; i++)
	{
		if (texMask&1) 
		{
			CELL_GCM_RESERVE(2);
		}
		texMask >>= 1;
	}
	CELL_GCM_FUNC_RETURN;
}

CELL_GCM_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetFragmentProgramParameter)(CELL_GCM_ARGS(const CGprogram prog, const CGparameter param, const float *value, const uint32_t offset))
{
	(void) value;
	(void) offset;

	CgBinaryProgram *prg = (CgBinaryProgram*) prog;
	CgBinaryParameter *p = (CgBinaryParameter*) param;

	switch (p->type) 
	{
	case CG_FLOAT:
	case CG_BOOL:
	case CG_FLOAT1:
	case CG_BOOL1:
		if (p->embeddedConst)
		{
			// set embedded constants
			CgBinaryEmbeddedConstantOffset ptr = p->embeddedConst;
			CgBinaryEmbeddedConstant *ec = (CgBinaryEmbeddedConstant*) ((char*)prg + ptr);
			uint32_t j;
			for(j=0; j<ec->ucodeCount; j++)
			{
				CELL_GCM_FUNC_CALL(SetInlineTransfer)(CELL_GCM_ARGS_FUNC(0, 0, 1));
			}
		}
		break;
	case CG_FLOAT2:
	case CG_BOOL2:
		if (p->embeddedConst)
		{
			// set embedded constants
			CgBinaryEmbeddedConstantOffset ptr = p->embeddedConst;
			CgBinaryEmbeddedConstant *ec = (CgBinaryEmbeddedConstant*) ((char*)prg + ptr);
			uint32_t j;
			for(j=0; j<ec->ucodeCount; j++)
			{
				CELL_GCM_FUNC_CALL(SetInlineTransfer)(CELL_GCM_ARGS_FUNC(0, 0, 2));
			}
		}
		break;
	case CG_FLOAT3:
	case CG_BOOL3:
		if (p->embeddedConst)
		{
			// set embedded constants
			CgBinaryEmbeddedConstantOffset ptr = p->embeddedConst;
			CgBinaryEmbeddedConstant *ec = (CgBinaryEmbeddedConstant*) ((char*)prg + ptr);
			uint32_t j;
			for(j=0; j<ec->ucodeCount; j++)
			{
				CELL_GCM_FUNC_CALL(SetInlineTransfer)(CELL_GCM_ARGS_FUNC(0, 0, 3));
			}
		}
		break;
	case CG_FLOAT4:
	case CG_BOOL4:
		if (p->embeddedConst)
		{
			// set embedded constants
			CgBinaryEmbeddedConstantOffset ptr = p->embeddedConst;
			CgBinaryEmbeddedConstant *ec = (CgBinaryEmbeddedConstant*) ((char*)prg + ptr);
			uint32_t j;
			for(j=0; j<ec->ucodeCount; j++)
			{
				CELL_GCM_FUNC_CALL(SetInlineTransfer)(CELL_GCM_ARGS_FUNC(0, 0, 4));
			}
		}
		break;
	case CG_FLOAT3x3:
	case CG_BOOL3x3:
		p++;
		for (uint32_t cnt = 0; cnt < 3; cnt++, p++) {
			// set embedded constants
			CgBinaryEmbeddedConstantOffset ptr = p->embeddedConst;
			if (ptr)
			{
				CgBinaryEmbeddedConstant *ec = (CgBinaryEmbeddedConstant*) ((char*)prg + ptr);
				uint32_t j;
				for(j=0; j<ec->ucodeCount; j++)
				{
					CELL_GCM_FUNC_CALL(SetInlineTransfer)(CELL_GCM_ARGS_FUNC(0, 0, 3));
				}
			}
		}
		break;
	case CG_FLOAT4x3:
	case CG_BOOL4x3:
		p++;
		for (uint32_t cnt = 0; cnt < 4; cnt++,p++) {
			// set embedded constants
			CgBinaryEmbeddedConstantOffset ptr = p->embeddedConst;
			if (ptr)
			{
				CgBinaryEmbeddedConstant *ec = (CgBinaryEmbeddedConstant*) ((char*)prg + ptr);
				uint32_t j;
				for(j=0; j<ec->ucodeCount; j++)
				{
					CELL_GCM_FUNC_CALL(SetInlineTransfer)(CELL_GCM_ARGS_FUNC(0,0,3));
				}
			}
		}
		break;
	case CG_FLOAT3x4:
	case CG_BOOL3x4:
		p++;
		for (uint32_t cnt = 0; cnt < 3; cnt++,p++) {
			// set embedded constants
			CgBinaryEmbeddedConstantOffset ptr = p->embeddedConst;
			if (ptr)
			{
				CgBinaryEmbeddedConstant *ec = (CgBinaryEmbeddedConstant*) ((char*)prg + ptr);
				uint32_t j;
				for(j=0; j<ec->ucodeCount; j++)
				{
					CELL_GCM_FUNC_CALL(SetInlineTransfer)(CELL_GCM_ARGS_FUNC(0,0,4));
				}
			}
		}
		break;
	case CG_FLOAT4x4:
	case CG_BOOL4x4:
		p++;
		for (uint32_t cnt = 0; cnt < 4; cnt++, p++) {
			// set embedded constants
			CgBinaryEmbeddedConstantOffset ptr = p->embeddedConst;
			if (ptr)
			{
				CgBinaryEmbeddedConstant *ec = (CgBinaryEmbeddedConstant*) ((char*)prg + ptr);
				uint32_t j;
				for(j=0; j<ec->ucodeCount; j++)
				{
					CELL_GCM_FUNC_CALL(SetInlineTransfer)(CELL_GCM_ARGS_FUNC(0, 0, 4));
				}
			}
		}
		break;
	default:
		break;
	}
	CELL_GCM_FUNC_RETURN;
}

CELL_GCM_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetFragmentProgramParameterPointer)(CELL_GCM_ARGS(const CGprogram prog, const CGparameter param, const uint32_t offset, const uint32_t srcOffset))
{
	CgBinaryProgram *prg = (CgBinaryProgram*) prog;
	CgBinaryParameter *p = (CgBinaryParameter*) param;
	uint32_t src = srcOffset;

	switch (p->type) 
	{
	case CG_FLOAT:
	case CG_BOOL:
	case CG_FLOAT1:
	case CG_BOOL1:
		if (p->embeddedConst)
		{
			// set embedded constants
			CgBinaryEmbeddedConstantOffset ptr = p->embeddedConst;
			CgBinaryEmbeddedConstant *ec = (CgBinaryEmbeddedConstant*) ((char*)prg + ptr);
			uint32_t j;
			for(j=0; j<ec->ucodeCount; j++)
			{
				uint32_t dst = offset + ec->ucodeOffset[j];
				// WAR bug 161692 'shader ucode requires 16bit swapping when big endian'
				// data in src needs to be 16bit swapped value
				CELL_GCM_FUNC_CALL(SetTransferData)(CELL_GCM_ARGS_FUNC(CELL_GCM_TRANSFER_MAIN_TO_LOCAL, dst, 4, src, 4, 4, 1));
			}
		}
		break;

	case CG_FLOAT2:
	case CG_BOOL2:
		if (p->embeddedConst)
		{
			// set embedded constants
			CgBinaryEmbeddedConstantOffset ptr = p->embeddedConst;
			CgBinaryEmbeddedConstant *ec = (CgBinaryEmbeddedConstant*) ((char*)prg + ptr);
			uint32_t j;
			for(j=0; j<ec->ucodeCount; j++)
			{
				uint32_t dst = offset + ec->ucodeOffset[j];
				// WAR bug 161692 'shader ucode requires 16bit swapping when big endian'
				// data in src needs to be 16bit swapped value
				CELL_GCM_FUNC_CALL(SetTransferData)(CELL_GCM_ARGS_FUNC(CELL_GCM_TRANSFER_MAIN_TO_LOCAL, dst, 8, src, 8, 8, 1));
			}
		}
		break;

	case CG_FLOAT3:
	case CG_BOOL3:
		if (p->embeddedConst)
		{
			// set embedded constants
			CgBinaryEmbeddedConstantOffset ptr = p->embeddedConst;
			CgBinaryEmbeddedConstant *ec = (CgBinaryEmbeddedConstant*) ((char*)prg + ptr);
			uint32_t j;
			for(j=0; j<ec->ucodeCount; j++)
			{
				uint32_t dst = offset + ec->ucodeOffset[j];
				// WAR bug 161692 'shader ucode requires 16bit swapping when big endian'
				// data in src needs to be 16bit swapped value
				CELL_GCM_FUNC_CALL(SetTransferData)(CELL_GCM_ARGS_FUNC(CELL_GCM_TRANSFER_MAIN_TO_LOCAL, dst, 12, src, 12, 12, 1));
			}
		}
		break;

	case CG_FLOAT4:
	case CG_BOOL4:
		if (p->embeddedConst)
		{
			// set embedded constants
			CgBinaryEmbeddedConstantOffset ptr = p->embeddedConst;
			CgBinaryEmbeddedConstant *ec = (CgBinaryEmbeddedConstant*) ((char*)prg + ptr);
			uint32_t j;
			for(j=0; j<ec->ucodeCount; j++)
			{
				uint32_t dst = offset + ec->ucodeOffset[j];
				// WAR bug 161692 'shader ucode requires 16bit swapping when big endian'
				// data in src needs to be 16bit swapped value
				CELL_GCM_FUNC_CALL(SetTransferData)(CELL_GCM_ARGS_FUNC(CELL_GCM_TRANSFER_MAIN_TO_LOCAL, dst, 16, src, 16, 16, 1));
			}
		}
		break;
	case CG_FLOAT3x3:
	case CG_BOOL3x3:
		p++;
		for (uint32_t cnt = 0; cnt < 3; cnt++, p++, src+=12) {
			// set embedded constants
			CgBinaryEmbeddedConstantOffset ptr = p->embeddedConst;
			if (ptr)
			{
				CgBinaryEmbeddedConstant *ec = (CgBinaryEmbeddedConstant*) ((char*)prg + ptr);
				uint32_t j;
				for(j=0; j<ec->ucodeCount; j++)
				{
					uint32_t dst = offset + ec->ucodeOffset[j];
					// WAR bug 161692 'shader ucode requires 16bit swapping when big endian'
					// data in src needs to be 16bit swapped value
					CELL_GCM_FUNC_CALL(SetTransferData)(CELL_GCM_ARGS_FUNC(CELL_GCM_TRANSFER_MAIN_TO_LOCAL, dst, 12, src, 12, 12, 1));
				}
			}
		}
		break;
	case CG_FLOAT4x3:
	case CG_BOOL4x3:
		p++;
		for (uint32_t cnt = 0; cnt < 4; cnt++, p++, src+=12) {
			// set embedded constants
			CgBinaryEmbeddedConstantOffset ptr = p->embeddedConst;
			if (ptr)
			{
				CgBinaryEmbeddedConstant *ec = (CgBinaryEmbeddedConstant*) ((char*)prg + ptr);
				uint32_t j;
				for(j=0; j<ec->ucodeCount; j++)
				{
					uint32_t dst = offset + ec->ucodeOffset[j];
					// WAR bug 161692 'shader ucode requires 16bit swapping when big endian'
					// data in src needs to be 16bit swapped value
					CELL_GCM_FUNC_CALL(SetTransferData)(CELL_GCM_ARGS_FUNC(CELL_GCM_TRANSFER_MAIN_TO_LOCAL, dst, 12, src, 12, 12, 1));
				}
			}
		}
		break;
	case CG_FLOAT3x4:
	case CG_BOOL3x4:
		p++;
		for (uint32_t cnt = 0; cnt < 3; cnt++, p++, src+=16) {
			// set embedded constants
			CgBinaryEmbeddedConstantOffset ptr = p->embeddedConst;
			if (ptr)
			{
				CgBinaryEmbeddedConstant *ec = (CgBinaryEmbeddedConstant*) ((char*)prg + ptr);
				uint32_t j;
				for(j=0; j<ec->ucodeCount; j++)
				{
					uint32_t dst = offset + ec->ucodeOffset[j];
					// WAR bug 161692 'shader ucode requires 16bit swapping when big endian'
					// data in src needs to be 16bit swapped value
					CELL_GCM_FUNC_CALL(SetTransferData)(CELL_GCM_ARGS_FUNC(CELL_GCM_TRANSFER_MAIN_TO_LOCAL, dst, 16, src, 16, 16, 1));
				}
			}
		}
		break;
	case CG_FLOAT4x4:
	case CG_BOOL4x4:
		p++;
		for (uint32_t cnt = 0; cnt < 4; cnt++, p++, src+=16) {
			// set embedded constants
			CgBinaryEmbeddedConstantOffset ptr = p->embeddedConst;
			if (ptr)
			{
				CgBinaryEmbeddedConstant *ec = (CgBinaryEmbeddedConstant*) ((char*)prg + ptr);
				uint32_t j;
				for(j=0; j<ec->ucodeCount; j++)
				{
					uint32_t dst = offset + ec->ucodeOffset[j];
					// WAR bug 161692 'shader ucode requires 16bit swapping when big endian'
					// data in src needs to be 16bit swapped value
					CELL_GCM_FUNC_CALL(SetTransferData)(CELL_GCM_ARGS_FUNC(CELL_GCM_TRANSFER_MAIN_TO_LOCAL, dst, 16, src, 16, 16, 1));
				}
			}
		}
		break;
	default:
		break;
	}
	CELL_GCM_FUNC_RETURN;
}

CELL_GCM_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetClearZcullSurface)(CELL_GCM_ARGS(const uint32_t depth, const uint32_t stencil))
{
	(void) depth;
	(void) stencil;
	CELL_GCM_RESERVE(2);
	CELL_GCM_FUNC_RETURN;
}

CELL_GCM_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetZcullEnable)(CELL_GCM_ARGS(const uint32_t depth, const uint32_t stencil))
{
	(void) depth;
	(void) stencil;
	CELL_GCM_RESERVE(2);
	CELL_GCM_FUNC_RETURN;
}

CELL_GCM_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetTransferReportData)(CELL_GCM_ARGS(const uint32_t offset, const uint32_t index, const uint32_t count))
{
	(void) offset;
	(void) index;
	(void) count;
	CELL_GCM_RESERVE(12);
	CELL_GCM_FUNC_RETURN;
}

CELL_GCM_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetWriteBackEndLabelForConditional)(CELL_GCM_ARGS(uint8_t index, uint32_t value))
{
	(void) index;
	(void) value;
	CELL_GCM_RESERVE(6);
	CELL_GCM_FUNC_RETURN;
}

CELL_GCM_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetWriteTextureLabelForConditional)(CELL_GCM_ARGS(uint8_t index, uint32_t value))
{
	(void) index;
	(void) value;
	CELL_GCM_RESERVE(6);
	CELL_GCM_FUNC_RETURN;
}

CELL_GCM_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetUserCommand)(CELL_GCM_ARGS(const uint32_t cause))
{
	(void) cause;
	CELL_GCM_RESERVE(2);
	CELL_GCM_FUNC_RETURN;
}

CELL_GCM_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetWaitFlip)(CELL_GCM_NO_ARGS())
{
	CELL_GCM_RESERVE(4);
	CELL_GCM_FUNC_RETURN;
}

CELL_GCM_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetReportLocation)(CELL_GCM_ARGS(uint32_t location))
{
	(void) location;
	CELL_GCM_RESERVE(2);
	CELL_GCM_FUNC_RETURN;
}

CELL_GCM_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetTextureAddressAnisoBiasRemap)(CELL_GCM_ARGS(uint8_t index, 
	uint8_t wraps, uint8_t wrapt, uint8_t wrapr, uint8_t unsignedRemap, 
	uint8_t zfunc, uint8_t gamma, uint8_t anisoBias, uint8_t signedRemap))
{
	(void) index;
	(void) wraps;
	(void) wrapt;
	(void) wrapr;
	(void) unsignedRemap;
	(void) zfunc;
	(void) gamma;
	(void) anisoBias;
	(void) signedRemap;
	CELL_GCM_RESERVE(2);
	CELL_GCM_FUNC_RETURN;
}

CELL_GCM_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetLabelLocation)(CELL_GCM_ARGS(uint32_t location))
{
	(void) location;
	CELL_GCM_RESERVE(2);
	CELL_GCM_FUNC_RETURN;
}

CELL_GCM_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetSkipNop)(CELL_GCM_ARGS(uint32_t count))
{
	CELL_GCM_RESERVE(1+count);
	CELL_GCM_FUNC_RETURN;
}

CELL_GCM_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetNotifyIndex)(CELL_GCM_ARGS(const uint32_t index))
{
	(void) index;
	CELL_GCM_RESERVE(2);
	CELL_GCM_FUNC_RETURN;
}

CELL_GCM_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetNotify)(CELL_GCM_NO_ARGS())
{
	CELL_GCM_RESERVE(4);
	CELL_GCM_FUNC_RETURN;
}

CELL_GCM_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetWaitForIdle)(CELL_GCM_NO_ARGS())
{
	CELL_GCM_RESERVE(2);
	CELL_GCM_FUNC_RETURN;
}

CELL_GCM_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetVertexData3f)(CELL_GCM_ARGS(uint8_t index, const float v[3]))
{
	(void) index;
	(void) v;
	CELL_GCM_RESERVE(4);
	CELL_GCM_FUNC_RETURN;
}

CELL_GCM_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetVertexData2f)(CELL_GCM_ARGS(uint8_t index, const float v[2]))
{
	(void) index;
	(void) v;
	CELL_GCM_RESERVE(3);
	CELL_GCM_FUNC_RETURN;
}

CELL_GCM_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetVertexData1f)(CELL_GCM_ARGS(uint8_t index, const float v))
{
	(void) index;
	(void) v;
	CELL_GCM_RESERVE(2);
	CELL_GCM_FUNC_RETURN;
}

CELL_GCM_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetVertexData4s)(CELL_GCM_ARGS(uint8_t index, const int16_t v[4]))
{
	(void) index;
	(void) v;
	CELL_GCM_RESERVE(3);
	CELL_GCM_FUNC_RETURN;
}

CELL_GCM_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetVertexDataScaled4s)(CELL_GCM_ARGS(uint8_t index, const int16_t v[4]))
{
	(void) index;
	(void) v;
	CELL_GCM_RESERVE(3);
	CELL_GCM_FUNC_RETURN;
}

CELL_GCM_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetVertexData2s)(CELL_GCM_ARGS(uint8_t index, const int16_t v[2]))
{
	(void) index;
	(void) v;
	CELL_GCM_RESERVE(2);
	CELL_GCM_FUNC_RETURN;
}

CELL_GCM_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetVertexData4ub)(CELL_GCM_ARGS(uint8_t index, const uint8_t v[4]))
{
	(void) index;
	(void) v;
	CELL_GCM_RESERVE(2);
	CELL_GCM_FUNC_RETURN;
}

