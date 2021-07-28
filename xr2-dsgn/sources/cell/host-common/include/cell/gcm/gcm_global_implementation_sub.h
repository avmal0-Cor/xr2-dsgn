/*   SCE CONFIDENTIAL                                       */
/*   PlayStation(R)3 Programmer Tool Runtime Library 310.001 */
/*   Copyright (C) 2008 Sony Computer Entertainment Inc.    */
/*   All Rights Reserved.                                   */

CELL_GCM_GLOBAL_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetReferenceCommand)(CELL_GCM_ARGS(const uint32_t ref))
{
	return CELL_GCM_CALLEE(SetReferenceCommand)(CELL_GCM_THIS, ref);
}

CELL_GCM_GLOBAL_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetJumpCommand)(CELL_GCM_ARGS(const uint32_t offset))
{
	return CELL_GCM_CALLEE(SetJumpCommand)(CELL_GCM_THIS, offset);
}

CELL_GCM_GLOBAL_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetCallCommand)(CELL_GCM_ARGS(const uint32_t offset))
{
	return CELL_GCM_CALLEE(SetCallCommand)(CELL_GCM_THIS, offset);
}

CELL_GCM_GLOBAL_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetReturnCommand)(CELL_GCM_NO_ARGS())
{
	return CELL_GCM_CALLEE(SetReturnCommand)(CELL_GCM_THIS);
}

CELL_GCM_GLOBAL_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetNopCommand)(CELL_GCM_ARGS(const uint32_t count))
{
	return CELL_GCM_CALLEE(SetNopCommand)(CELL_GCM_THIS, count);
}

CELL_GCM_GLOBAL_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetSurface)(CELL_GCM_ARGS(const CellGcmSurface *surface))
{
	return CELL_GCM_CALLEE(SetSurface)(CELL_GCM_THIS, surface);
}

CELL_GCM_GLOBAL_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetSurfaceWindow)(CELL_GCM_ARGS(const CellGcmSurface *surface, const uint32_t origin, const uint32_t pixelCenter))
{
	return CELL_GCM_CALLEE(SetSurfaceWindow)(CELL_GCM_THIS, surface, origin, pixelCenter);
}

CELL_GCM_GLOBAL_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetAntiAliasingControl)(CELL_GCM_ARGS(const uint32_t enable, const uint32_t alphaToCoverage, const uint32_t alphaToOne, const uint32_t sampleMask))
{
	return CELL_GCM_CALLEE(SetAntiAliasingControl)(CELL_GCM_THIS, enable, alphaToCoverage, alphaToOne, sampleMask);
}

CELL_GCM_GLOBAL_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetWaitLabel)(CELL_GCM_ARGS(const uint8_t index, const uint32_t value))
{
	return CELL_GCM_CALLEE(SetWaitLabel)(CELL_GCM_THIS, index,value);
}

CELL_GCM_GLOBAL_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetWriteCommandLabel)(CELL_GCM_ARGS(const uint8_t index, const uint32_t value))
{
	return CELL_GCM_CALLEE(SetWriteCommandLabel)(CELL_GCM_THIS, index, value);
}

CELL_GCM_GLOBAL_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetWriteBackEndLabel)(CELL_GCM_ARGS(const uint8_t index, const uint32_t value))
{
	return CELL_GCM_CALLEE(SetWriteBackEndLabel)(CELL_GCM_THIS, index, value);
}

CELL_GCM_GLOBAL_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetWriteTextureLabel)(CELL_GCM_ARGS(const uint8_t index, const uint32_t value))
{
	return CELL_GCM_CALLEE(SetWriteTextureLabel)(CELL_GCM_THIS, index, value);
}

CELL_GCM_GLOBAL_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetTimeStamp)(CELL_GCM_ARGS(const uint32_t index))
{
	return CELL_GCM_CALLEE(SetTimeStamp)(CELL_GCM_THIS, index);
}

CELL_GCM_GLOBAL_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetInvalidateZcull)(CELL_GCM_NO_ARGS())
{
	return CELL_GCM_CALLEE(SetInvalidateZcull)(CELL_GCM_THIS);
}

CELL_GCM_GLOBAL_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetAlphaFunc)(CELL_GCM_ARGS(const uint32_t af, const uint32_t ref))
{
	return CELL_GCM_CALLEE(SetAlphaFunc)(CELL_GCM_THIS, af, ref);
}

CELL_GCM_GLOBAL_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetBlendColor)(CELL_GCM_ARGS(const uint32_t color, const uint32_t color2))
{
	return CELL_GCM_CALLEE(SetBlendColor)(CELL_GCM_THIS, color,color2);
}

CELL_GCM_GLOBAL_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetBlendEquation)(CELL_GCM_ARGS(const uint16_t color, const uint16_t alpha))
{
	return CELL_GCM_CALLEE(SetBlendEquation)(CELL_GCM_THIS, color,alpha);
}

CELL_GCM_GLOBAL_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetBlendFunc)(CELL_GCM_ARGS(const uint16_t sfcolor, const uint16_t dfcolor, const uint16_t sfalpha, const uint16_t dfalpha))
{
	return CELL_GCM_CALLEE(SetBlendFunc)(CELL_GCM_THIS, sfcolor,dfcolor,sfalpha,dfalpha);
}

CELL_GCM_GLOBAL_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetClearSurface)(CELL_GCM_ARGS(const uint32_t mask))
{
	return CELL_GCM_CALLEE(SetClearSurface)(CELL_GCM_THIS, mask);
}

CELL_GCM_GLOBAL_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetClearColor)(CELL_GCM_ARGS(const uint32_t color))
{
	return CELL_GCM_CALLEE(SetClearColor)(CELL_GCM_THIS, color);
}

CELL_GCM_GLOBAL_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetClearDepthStencil)(CELL_GCM_ARGS(const uint32_t value))
{
	return CELL_GCM_CALLEE(SetClearDepthStencil)(CELL_GCM_THIS, value);
}

CELL_GCM_GLOBAL_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetColorMask)(CELL_GCM_ARGS(const uint32_t mask))
{
	return CELL_GCM_CALLEE(SetColorMask)(CELL_GCM_THIS, mask);
}

CELL_GCM_GLOBAL_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetColorMaskMrt)(CELL_GCM_ARGS(const uint32_t mask))
{
	return CELL_GCM_CALLEE(SetColorMaskMrt)(CELL_GCM_THIS, mask);
}

CELL_GCM_GLOBAL_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetCullFace)(CELL_GCM_ARGS(const uint32_t cfm))
{
	return CELL_GCM_CALLEE(SetCullFace)(CELL_GCM_THIS, cfm);
}

CELL_GCM_GLOBAL_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetDepthBounds)(CELL_GCM_ARGS(const float zmin, const float zmax))
{
	return CELL_GCM_CALLEE(SetDepthBounds)(CELL_GCM_THIS, zmin,zmax);
}

CELL_GCM_GLOBAL_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetDepthFunc)(CELL_GCM_ARGS(const uint32_t zf))
{
	return CELL_GCM_CALLEE(SetDepthFunc)(CELL_GCM_THIS, zf);
}

CELL_GCM_GLOBAL_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetDepthMask)(CELL_GCM_ARGS(const uint32_t enable))
{
	return CELL_GCM_CALLEE(SetDepthMask)(CELL_GCM_THIS, enable);
}

CELL_GCM_GLOBAL_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetFrontFace)(CELL_GCM_ARGS(const uint32_t dir))
{
	return CELL_GCM_CALLEE(SetFrontFace)(CELL_GCM_THIS, dir);
}

CELL_GCM_GLOBAL_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetLineWidth)(CELL_GCM_ARGS(const uint32_t width))
{
	return CELL_GCM_CALLEE(SetLineWidth)(CELL_GCM_THIS, width);
}

CELL_GCM_GLOBAL_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetLineSmoothEnable)(CELL_GCM_ARGS(const uint32_t enable))
{
	return CELL_GCM_CALLEE(SetLineSmoothEnable)(CELL_GCM_THIS, enable);
}

CELL_GCM_GLOBAL_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetLineStippleEnable)(CELL_GCM_ARGS(const uint32_t enable))
{
	return CELL_GCM_CALLEE(SetLineStippleEnable)(CELL_GCM_THIS, enable);
}

CELL_GCM_GLOBAL_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetLineStipplePattern)(CELL_GCM_ARGS(const uint16_t* pattern, const uint8_t factor))
{
	return CELL_GCM_CALLEE(SetLineStipplePattern)(CELL_GCM_THIS, pattern,factor);
}

CELL_GCM_GLOBAL_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetLogicOp)(CELL_GCM_ARGS(const uint32_t op))
{
	return CELL_GCM_CALLEE(SetLogicOp)(CELL_GCM_THIS, op);
}

CELL_GCM_GLOBAL_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetPointSize)(CELL_GCM_ARGS(const float size))
{
	return CELL_GCM_CALLEE(SetPointSize)(CELL_GCM_THIS, size);
}

CELL_GCM_GLOBAL_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetPolygonOffset)(CELL_GCM_ARGS(const float factor, const float units))
{
	return CELL_GCM_CALLEE(SetPolygonOffset)(CELL_GCM_THIS, factor,units);
}

CELL_GCM_GLOBAL_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetPolySmoothEnable)(CELL_GCM_ARGS(const uint32_t enable))
{
	return CELL_GCM_CALLEE(SetPolySmoothEnable)(CELL_GCM_THIS, enable);
}

CELL_GCM_GLOBAL_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetPolygonStippleEnable)(CELL_GCM_ARGS(const uint32_t enable))
{
	return CELL_GCM_CALLEE(SetPolygonStippleEnable)(CELL_GCM_THIS, enable);
}

CELL_GCM_GLOBAL_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetPolygonStipplePattern)(CELL_GCM_ARGS(const uint32_t* pattern))
{
	return CELL_GCM_CALLEE(SetPolygonStipplePattern)(CELL_GCM_THIS, pattern);
}

CELL_GCM_GLOBAL_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetFrontPolygonMode)(CELL_GCM_ARGS(const uint32_t mode))
{
	return CELL_GCM_CALLEE(SetFrontPolygonMode)(CELL_GCM_THIS, mode);
}

CELL_GCM_GLOBAL_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetBackPolygonMode)(CELL_GCM_ARGS(const uint32_t mode))
{
	return CELL_GCM_CALLEE(SetBackPolygonMode)(CELL_GCM_THIS, mode);
}

CELL_GCM_GLOBAL_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetScissor)(CELL_GCM_ARGS(const uint16_t x, const uint16_t y, const uint16_t w, const uint16_t h))
{
	return CELL_GCM_CALLEE(SetScissor)(CELL_GCM_THIS, x,y,w,h);
}

CELL_GCM_GLOBAL_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetShadeMode)(CELL_GCM_ARGS(const uint32_t sm))
{
	return CELL_GCM_CALLEE(SetShadeMode)(CELL_GCM_THIS, sm);
}

CELL_GCM_GLOBAL_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetTwoSideLightEnable)(CELL_GCM_ARGS(const uint32_t enable))
{
	return CELL_GCM_CALLEE(SetTwoSideLightEnable)(CELL_GCM_THIS, enable);
}

CELL_GCM_GLOBAL_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetStencilFunc)(CELL_GCM_ARGS(const uint32_t func, const int32_t ref, const uint32_t mask))
{
	return CELL_GCM_CALLEE(SetStencilFunc)(CELL_GCM_THIS, func,ref,mask);
}

CELL_GCM_GLOBAL_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetBackStencilFunc)(CELL_GCM_ARGS(const uint32_t func, const int32_t ref, const uint32_t mask))
{
	return CELL_GCM_CALLEE(SetBackStencilFunc)(CELL_GCM_THIS, func,ref,mask);
}

CELL_GCM_GLOBAL_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetStencilMask)(CELL_GCM_ARGS(const uint32_t sm))
{
	return CELL_GCM_CALLEE(SetStencilMask)(CELL_GCM_THIS, sm);
}

CELL_GCM_GLOBAL_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetBackStencilMask)(CELL_GCM_ARGS(const uint32_t sm))
{
	return CELL_GCM_CALLEE(SetBackStencilMask)(CELL_GCM_THIS, sm);
}

CELL_GCM_GLOBAL_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetStencilOp)(CELL_GCM_ARGS(const uint32_t fail, const uint32_t depthFail, const uint32_t depthPass))
{
	return CELL_GCM_CALLEE(SetStencilOp)(CELL_GCM_THIS, fail,depthFail,depthPass);
}

CELL_GCM_GLOBAL_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetBackStencilOp)(CELL_GCM_ARGS(const uint32_t fail, const uint32_t depthFail, const uint32_t depthPass))
{
	return CELL_GCM_CALLEE(SetBackStencilOp)(CELL_GCM_THIS, fail,depthFail,depthPass);
}

CELL_GCM_GLOBAL_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetZMinMaxControl)(CELL_GCM_ARGS(const uint32_t cullNearFarEnable, const uint32_t zclampEnable, const uint32_t cullIgnoreW))
{
	return CELL_GCM_CALLEE(SetZMinMaxControl)(CELL_GCM_THIS, cullNearFarEnable, zclampEnable, cullIgnoreW);
}

CELL_GCM_GLOBAL_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetClipMinMax)(CELL_GCM_ARGS(const float min, const float max))
{
	return CELL_GCM_CALLEE(SetClipMinMax)(CELL_GCM_THIS, min,max);
}

CELL_GCM_GLOBAL_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetViewport)(CELL_GCM_ARGS(const uint16_t x, const uint16_t y, const uint16_t w, const uint16_t h, const float min, const float max, const float scale[4], const float offset[4]))
{
	return CELL_GCM_CALLEE(SetViewport)(CELL_GCM_THIS, x,y,w,h,min,max,scale,offset);
}

CELL_GCM_GLOBAL_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetAlphaTestEnable)(CELL_GCM_ARGS(const uint32_t enable))
{
	return CELL_GCM_CALLEE(SetAlphaTestEnable)(CELL_GCM_THIS, enable);
}

CELL_GCM_GLOBAL_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetBlendEnable)(CELL_GCM_ARGS(const uint32_t enable))
{
	return CELL_GCM_CALLEE(SetBlendEnable)(CELL_GCM_THIS, enable);
}

CELL_GCM_GLOBAL_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetBlendEnableMrt)(CELL_GCM_ARGS(const uint32_t mrt1, const uint32_t mrt2, const uint32_t mrt3))
{
	return CELL_GCM_CALLEE(SetBlendEnableMrt)(CELL_GCM_THIS, mrt1,mrt2,mrt3);
}

CELL_GCM_GLOBAL_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetLogicOpEnable)(CELL_GCM_ARGS(const uint32_t enable))
{
	return CELL_GCM_CALLEE(SetLogicOpEnable)(CELL_GCM_THIS, enable);
}

CELL_GCM_GLOBAL_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetCullFaceEnable)(CELL_GCM_ARGS(const uint32_t enable))
{
	return CELL_GCM_CALLEE(SetCullFaceEnable)(CELL_GCM_THIS, enable);
}

CELL_GCM_GLOBAL_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetDepthBoundsTestEnable)(CELL_GCM_ARGS(const uint32_t enable))
{
	return CELL_GCM_CALLEE(SetDepthBoundsTestEnable)(CELL_GCM_THIS, enable);
}

CELL_GCM_GLOBAL_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetDepthTestEnable)(CELL_GCM_ARGS(const uint32_t enable))
{
	return CELL_GCM_CALLEE(SetDepthTestEnable)(CELL_GCM_THIS, enable);
}

CELL_GCM_GLOBAL_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetDitherEnable)(CELL_GCM_ARGS(const uint32_t enable))
{
	return CELL_GCM_CALLEE(SetDitherEnable)(CELL_GCM_THIS, enable);
}

CELL_GCM_GLOBAL_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetStencilTestEnable)(CELL_GCM_ARGS(const uint32_t enable))
{
	return CELL_GCM_CALLEE(SetStencilTestEnable)(CELL_GCM_THIS, enable);
}

CELL_GCM_GLOBAL_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetTwoSidedStencilTestEnable)(CELL_GCM_ARGS(const uint32_t enable))
{
	return CELL_GCM_CALLEE(SetTwoSidedStencilTestEnable)(CELL_GCM_THIS, enable);
}

CELL_GCM_GLOBAL_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetPolygonOffsetFillEnable)(CELL_GCM_ARGS(const uint32_t enable))
{
	return CELL_GCM_CALLEE(SetPolygonOffsetFillEnable)(CELL_GCM_THIS, enable);
}

CELL_GCM_GLOBAL_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetRestartIndexEnable)(CELL_GCM_ARGS(const uint32_t enable))
{
	return CELL_GCM_CALLEE(SetRestartIndexEnable)(CELL_GCM_THIS, enable);
}

CELL_GCM_GLOBAL_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetPointSpriteControl)(CELL_GCM_ARGS(const uint32_t enable, const uint32_t rmode, const uint32_t texcoord))
{
	return CELL_GCM_CALLEE(SetPointSpriteControl)(CELL_GCM_THIS, enable, rmode, texcoord);
}

CELL_GCM_GLOBAL_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetInvalidateTextureCache)(CELL_GCM_ARGS(const uint32_t value))
{
	return CELL_GCM_CALLEE(SetInvalidateTextureCache)(CELL_GCM_THIS, value);
}

CELL_GCM_GLOBAL_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetTextureBorderColor)(CELL_GCM_ARGS(const uint8_t index, const uint32_t color))
{
	return CELL_GCM_CALLEE(SetTextureBorderColor)(CELL_GCM_THIS, index, color);
}

CELL_GCM_GLOBAL_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetTextureFilter)(CELL_GCM_ARGS(const uint8_t index, const uint16_t bias, const uint8_t min, const uint8_t mag, const uint8_t conv))
{
	return CELL_GCM_CALLEE(SetTextureFilter)(CELL_GCM_THIS, index, bias, min, mag, conv);
}

CELL_GCM_GLOBAL_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetTextureFilterSigned)(CELL_GCM_ARGS(const uint8_t index, const uint16_t bias, const uint8_t min, const uint8_t mag, const uint8_t conv, const uint8_t as, const uint8_t rs, const uint8_t gs, const uint8_t bs))
{
	return CELL_GCM_CALLEE(SetTextureFilterSigned)(CELL_GCM_THIS, index, bias, min, mag, conv, as, rs, gs, bs);
}

CELL_GCM_GLOBAL_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetTextureAddress)(CELL_GCM_ARGS(const uint8_t index, const uint8_t wraps, const uint8_t wrapt, const uint8_t wrapr, const uint8_t unsignedRemap, const uint8_t zfunc, const uint8_t gamma))
{
	return CELL_GCM_CALLEE(SetTextureAddress)(CELL_GCM_THIS, index, wraps, wrapt, wrapr, unsignedRemap, zfunc, gamma);
}

CELL_GCM_GLOBAL_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetTextureControl)(CELL_GCM_ARGS(const uint8_t index, const uint32_t enable, const uint16_t minlod, const uint16_t maxlod, const uint8_t maxaniso))
{
	return CELL_GCM_CALLEE(SetTextureControl)(CELL_GCM_THIS, index, enable, minlod, maxlod, maxaniso);
}

CELL_GCM_GLOBAL_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetTextureOptimization)(CELL_GCM_ARGS(const uint8_t index, const uint8_t slope, const uint8_t iso, const uint8_t aniso))
{
	return CELL_GCM_CALLEE(SetTextureOptimization)(CELL_GCM_THIS, index, slope, iso, aniso);
}

CELL_GCM_GLOBAL_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetCylindricalWrap)(CELL_GCM_ARGS(const uint32_t enable, const uint32_t reserved))
{
	return CELL_GCM_CALLEE(SetCylindricalWrap)(CELL_GCM_THIS, enable, reserved);
}

CELL_GCM_GLOBAL_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetTextureBorder)(CELL_GCM_ARGS(const uint8_t index, const CellGcmTexture *texture, uint8_t border))
{	
	return CELL_GCM_CALLEE(SetTextureBorder)(CELL_GCM_THIS, index, texture, border);
}

CELL_GCM_GLOBAL_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetTexture)(CELL_GCM_ARGS(const uint8_t index, const CellGcmTexture *texture))
{
	return CELL_GCM_CALLEE(SetTexture)(CELL_GCM_THIS, index, texture);
}

CELL_GCM_GLOBAL_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetDrawArrays)(CELL_GCM_ARGS(const uint8_t mode, uint32_t first, uint32_t count))
{
	return CELL_GCM_CALLEE(SetDrawArrays)(CELL_GCM_THIS, mode, first, count);
}

CELL_GCM_GLOBAL_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetDrawIndexArray)(CELL_GCM_ARGS(const uint8_t mode, uint32_t count, const uint8_t type, const uint8_t location, const uint32_t indicies))
{
	return CELL_GCM_CALLEE(SetDrawIndexArray)(CELL_GCM_THIS, mode, count, type, location, indicies);
}

CELL_GCM_GLOBAL_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetInvalidateVertexCache)(CELL_GCM_NO_ARGS())
{
	return CELL_GCM_CALLEE(SetInvalidateVertexCache)(CELL_GCM_THIS);
}

CELL_GCM_GLOBAL_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetRestartIndex)(CELL_GCM_ARGS(const uint32_t index))
{
	return CELL_GCM_CALLEE(SetRestartIndex)(CELL_GCM_THIS, index);
}

CELL_GCM_GLOBAL_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetVertexData4f)(CELL_GCM_ARGS(const uint8_t index, const float v[4]))
{
	return CELL_GCM_CALLEE(SetVertexData4f)(CELL_GCM_THIS, index, v);
}

CELL_GCM_GLOBAL_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetVertexDataArray)(CELL_GCM_ARGS(const uint8_t index, const uint16_t frequency, const uint8_t stride, const uint8_t size, const uint8_t type, const uint8_t location, const uint32_t offset))
{
	return CELL_GCM_CALLEE(SetVertexDataArray)(CELL_GCM_THIS, index, frequency, stride, size, type, location, offset);
}

CELL_GCM_GLOBAL_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetFrequencyDividerOperation)(CELL_GCM_ARGS(const uint16_t operation))
{
	return CELL_GCM_CALLEE(SetFrequencyDividerOperation)(CELL_GCM_THIS, operation);
}

CELL_GCM_GLOBAL_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetTransformBranchBits)(CELL_GCM_ARGS(const uint32_t branchBits))
{
	return CELL_GCM_CALLEE(SetTransformBranchBits)(CELL_GCM_THIS, branchBits);
}

CELL_GCM_GLOBAL_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetVertexAttribInputMask)(CELL_GCM_ARGS(const uint16_t mask))
{
	return CELL_GCM_CALLEE(SetVertexAttribInputMask)(CELL_GCM_THIS, mask);
}

CELL_GCM_GLOBAL_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetVertexProgramParameterBlock)(CELL_GCM_ARGS(const uint32_t baseConst, uint32_t constCount, const float * __restrict value))
{
	return CELL_GCM_CALLEE(SetVertexProgramParameterBlock)(CELL_GCM_THIS, baseConst, constCount, value);
}

CELL_GCM_GLOBAL_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetUpdateFragmentProgramParameter)(CELL_GCM_ARGS(const uint32_t offset))
{
	return CELL_GCM_CALLEE(SetUpdateFragmentProgramParameter)(CELL_GCM_THIS, offset);
}

CELL_GCM_GLOBAL_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetFragmentProgramGammaEnable)(CELL_GCM_ARGS(const uint32_t enable))
{
	return CELL_GCM_CALLEE(SetFragmentProgramGammaEnable)(CELL_GCM_THIS, enable);
}

CELL_GCM_GLOBAL_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetUserClipPlaneControl)(CELL_GCM_ARGS(const uint32_t plane0, const uint32_t plane1, const uint32_t plane2, const uint32_t plane3, const uint32_t plane4, const uint32_t plane5))
{
	return CELL_GCM_CALLEE(SetUserClipPlaneControl)(CELL_GCM_THIS, plane0, plane1, plane2, plane3, plane4, plane5);
}

CELL_GCM_GLOBAL_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetVertexProgramConstants)(CELL_GCM_ARGS(const uint32_t start, const uint32_t count, const float *data))
{
	return CELL_GCM_CALLEE(SetVertexProgramConstants)(CELL_GCM_THIS, start, count, data);
}

CELL_GCM_GLOBAL_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetInlineTransfer)(CELL_GCM_ARGS(const uint32_t dstOffset, const void *srcAdr, const uint32_t sizeInWords))
{
	return CELL_GCM_CALLEE(SetInlineTransfer)(CELL_GCM_THIS, dstOffset, srcAdr, sizeInWords);
}

CELL_GCM_GLOBAL_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(InlineTransfer)(CELL_GCM_ARGS(const uint32_t dstOffset, const void *srcAdr, const uint32_t sizeInWords, const uint8_t location = CELL_GCM_LOCATION_LOCAL))
{
	return CELL_GCM_CALLEE(InlineTransfer)(CELL_GCM_THIS, dstOffset, srcAdr, sizeInWords, location);
}

CELL_GCM_GLOBAL_DECL CELL_GCM_FUNC_TYPE_UINT32 CELL_GCM_FUNC(SetTransferImage)(CELL_GCM_ARGS(const uint8_t mode, const uint32_t dstOffset, const uint32_t dstPitch, const uint32_t dstX, const uint32_t dstY, const uint32_t srcOffset, const uint32_t srcPitch, const uint32_t srcX, const uint32_t srcY, const uint32_t width, const uint32_t height, const uint32_t bytesPerPixel))
{
	return CELL_GCM_CALLEE(SetTransferImage)(CELL_GCM_THIS, mode, dstOffset, dstPitch, dstX, dstY, srcOffset, srcPitch, srcX, srcY, width, height, bytesPerPixel);
}

CELL_GCM_GLOBAL_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(TransferData)(CELL_GCM_ARGS(uint32_t dstOffset, int32_t dstPitch, uint32_t srcOffset, int32_t srcPitch, int32_t bytesPerRow, int32_t rowCount))
{
	return CELL_GCM_CALLEE(TransferData)(CELL_GCM_THIS, dstOffset, dstPitch, srcOffset, srcPitch, bytesPerRow, rowCount);
}

CELL_GCM_GLOBAL_DECL CELL_GCM_FUNC_TYPE_UINT32 CELL_GCM_FUNC(SetTransferData)(CELL_GCM_ARGS(const uint8_t mode, const uint32_t dstOffset, const uint32_t dstPitch, const uint32_t srcOffset, const uint32_t srcPitch, const uint32_t bytesPerRow, const uint32_t rowCount))
{
	return CELL_GCM_CALLEE(SetTransferData)(CELL_GCM_THIS, mode, dstOffset, dstPitch, srcOffset, srcPitch, bytesPerRow, rowCount);
}

CELL_GCM_GLOBAL_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetRenderEnable)(CELL_GCM_ARGS(const uint8_t mode, const uint32_t index))
{
	return CELL_GCM_CALLEE(SetRenderEnable)(CELL_GCM_THIS, mode, index);
}

CELL_GCM_GLOBAL_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetZpassPixelCountEnable)(CELL_GCM_ARGS(const uint32_t enable))
{
	return CELL_GCM_CALLEE(SetZpassPixelCountEnable)(CELL_GCM_THIS, enable);
}

CELL_GCM_GLOBAL_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetClearReport)(CELL_GCM_ARGS(const uint32_t type))
{
	return CELL_GCM_CALLEE(SetClearReport)(CELL_GCM_THIS, type);
}

CELL_GCM_GLOBAL_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetReport)(CELL_GCM_ARGS(const uint32_t type, const uint32_t index))
{
	return CELL_GCM_CALLEE(SetReport)(CELL_GCM_THIS, type, index);
}

CELL_GCM_GLOBAL_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetZcullStatsEnable)(CELL_GCM_ARGS(const uint32_t enable))
{
	return CELL_GCM_CALLEE(SetZcullStatsEnable)(CELL_GCM_THIS, enable);
}

CELL_GCM_GLOBAL_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetZcullControl)(CELL_GCM_ARGS(const uint8_t zCullDir, const uint8_t zCullFormat))
{
	return CELL_GCM_CALLEE(SetZcullControl)(CELL_GCM_THIS, zCullDir, zCullFormat);
}

CELL_GCM_GLOBAL_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetZcullLimit)(CELL_GCM_ARGS(const uint16_t moveForwardLimit, const uint16_t pushBackLimit))
{
	return CELL_GCM_CALLEE(SetZcullLimit)(CELL_GCM_THIS, moveForwardLimit, pushBackLimit);
}

CELL_GCM_GLOBAL_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetScullControl)(CELL_GCM_ARGS(const uint8_t sFunc, const uint8_t sRef, const uint8_t sMask))
{
	return CELL_GCM_CALLEE(SetScullControl)(CELL_GCM_THIS, sFunc, sRef, sMask);
}

CELL_GCM_GLOBAL_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetConvertSwizzleFormat)(CELL_GCM_ARGS(uint32_t dstOffset, uint32_t dstWidth, uint32_t dstHeight, uint32_t dstX, uint32_t dstY, uint32_t srcOffset, uint32_t srcPitch, uint32_t srcX, uint32_t srcY, uint32_t width, uint32_t height, uint32_t bytesPerPixel, uint8_t mode = CELL_GCM_TRANSFER_MAIN_TO_LOCAL))
{
	return CELL_GCM_CALLEE(SetConvertSwizzleFormat)(CELL_GCM_THIS, dstOffset, dstWidth, dstHeight, dstX, dstY, srcOffset, srcPitch, srcX, srcY, width, height, bytesPerPixel, mode);
}

CELL_GCM_GLOBAL_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetVertexTexture)(CELL_GCM_ARGS(const uint8_t index, const CellGcmTexture *texture))
{
	return CELL_GCM_CALLEE(SetVertexTexture)(CELL_GCM_THIS, index, texture);
}

CELL_GCM_GLOBAL_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetVertexTextureAddress)(CELL_GCM_ARGS(const uint8_t index, const uint8_t wraps, const uint8_t wrapt))
{
	return CELL_GCM_CALLEE(SetVertexTextureAddress)(CELL_GCM_THIS, index, wraps, wrapt);
}

CELL_GCM_GLOBAL_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetVertexTextureFilter)(CELL_GCM_ARGS(const uint8_t index, const uint16_t bias))
{
	return CELL_GCM_CALLEE(SetVertexTextureFilter)(CELL_GCM_THIS, index, bias);
}

CELL_GCM_GLOBAL_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetVertexTextureControl)(CELL_GCM_ARGS(const uint8_t index, const uint32_t enable, const uint16_t minLod, const uint16_t maxLod))
{
	return CELL_GCM_CALLEE(SetVertexTextureControl)(CELL_GCM_THIS, index, enable, minLod, maxLod);
}

CELL_GCM_GLOBAL_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetVertexTextureBorderColor)(CELL_GCM_ARGS(const uint8_t index, const uint32_t color))
{
	return CELL_GCM_CALLEE(SetVertexTextureBorderColor)(CELL_GCM_THIS, index, color);
}

CELL_GCM_GLOBAL_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetPerfMonTrigger)(CELL_GCM_NO_ARGS())
{
	return CELL_GCM_CALLEE(SetPerfMonTrigger)(CELL_GCM_THIS);
}

CELL_GCM_GLOBAL_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetDrawInlineArray)(CELL_GCM_ARGS(const uint8_t mode, const uint32_t count, const void *data))
{
	return CELL_GCM_CALLEE(SetDrawInlineArray)(CELL_GCM_THIS, mode, count, data);
}

CELL_GCM_GLOBAL_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetDrawInlineIndexArray32)(CELL_GCM_ARGS(const uint8_t mode, const uint32_t start, const uint32_t count, const uint32_t *data))
{
	return CELL_GCM_CALLEE(SetDrawInlineIndexArray32)(CELL_GCM_THIS, mode, start, count, data);
}

CELL_GCM_GLOBAL_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetDrawInlineIndexArray16)(CELL_GCM_ARGS(const uint8_t mode, const uint32_t start, const uint32_t count, const uint16_t *data))
{
	return CELL_GCM_CALLEE(SetDrawInlineIndexArray16)(CELL_GCM_THIS, mode, start, count, data);
}

CELL_GCM_GLOBAL_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetFogMode)(CELL_GCM_ARGS(const uint32_t mode))
{
	return CELL_GCM_CALLEE(SetFogMode)(CELL_GCM_THIS, mode);
}

CELL_GCM_GLOBAL_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetFogParams)(CELL_GCM_ARGS(const float p0, const float p1))
{
	return CELL_GCM_CALLEE(SetFogParams)(CELL_GCM_THIS, p0, p1);
}

CELL_GCM_GLOBAL_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetTransferLocation)(CELL_GCM_ARGS(const uint32_t location))
{
	return CELL_GCM_CALLEE(SetTransferLocation)(CELL_GCM_THIS, location);
}

CELL_GCM_GLOBAL_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetDepthFormat)(CELL_GCM_ARGS(const uint32_t format))
{
	return CELL_GCM_CALLEE(SetDepthFormat)(CELL_GCM_THIS, format);
}

CELL_GCM_GLOBAL_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetBlendOptimization)(CELL_GCM_ARGS(const uint32_t enable))
{
	return CELL_GCM_CALLEE(SetBlendOptimization)(CELL_GCM_THIS, enable);
}

CELL_GCM_GLOBAL_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetTextureRemap)(CELL_GCM_ARGS(const uint8_t index, const uint32_t remap))
{
	return CELL_GCM_CALLEE(SetTextureRemap)(CELL_GCM_THIS, index, remap);
}

CELL_GCM_GLOBAL_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetPolygonOffsetLineEnable)(CELL_GCM_ARGS(const uint32_t enable))
{
    return CELL_GCM_CALLEE(SetPolygonOffsetLineEnable)(CELL_GCM_THIS, enable);
}

CELL_GCM_GLOBAL_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetVertexAttribOutputMask)(CELL_GCM_ARGS(const uint32_t mask))
{
	return CELL_GCM_CALLEE(SetVertexAttribOutputMask)(CELL_GCM_THIS, mask);
}

CELL_GCM_GLOBAL_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetVertexProgramLoadSlot)(CELL_GCM_ARGS(const uint32_t loadSlot, const uint32_t instCount, const void *ucode))
{
	return CELL_GCM_CALLEE(SetVertexProgramLoadSlot)(CELL_GCM_THIS, loadSlot, instCount, ucode);
}

CELL_GCM_GLOBAL_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetVertexProgramStartSlot)(CELL_GCM_ARGS(const uint32_t startSlot))
{
	return CELL_GCM_CALLEE(SetVertexProgramStartSlot)(CELL_GCM_THIS, startSlot);
}

CELL_GCM_GLOBAL_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetVertexProgramRegisterCount)(CELL_GCM_ARGS(const uint32_t registerCount))
{
	return CELL_GCM_CALLEE(SetVertexProgramRegisterCount)(CELL_GCM_THIS, registerCount);
}

CELL_GCM_GLOBAL_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetVertexProgramLoad)(CELL_GCM_ARGS(const CellCgbVertexProgramConfiguration *conf, const void *ucode))
{
	return CELL_GCM_CALLEE(SetVertexProgramLoad)(CELL_GCM_THIS, conf, ucode);
}

CELL_GCM_GLOBAL_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetFragmentProgramLoad)(CELL_GCM_ARGS(const CellCgbFragmentProgramConfiguration *conf))
{
	return CELL_GCM_CALLEE(SetFragmentProgramLoad)(CELL_GCM_THIS, conf);
}

CELL_GCM_GLOBAL_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetInlineTransferPointer)(CELL_GCM_ARGS(const uint32_t offset, const uint32_t count, void **pointer))
{
	return CELL_GCM_CALLEE(SetInlineTransferPointer)(CELL_GCM_THIS, offset, count, pointer);
}

CELL_GCM_GLOBAL_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetTransferDataMode)(CELL_GCM_ARGS(const uint8_t mode))
{
	return CELL_GCM_CALLEE(SetTransferDataMode)(CELL_GCM_THIS, mode);
}

CELL_GCM_GLOBAL_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetTransferDataFormat)(CELL_GCM_ARGS(const int32_t inPitch, const int32_t outPitch, const uint32_t lineLength, const uint32_t lineCount, const uint8_t inFormat, const uint8_t outFormat))
{
	return CELL_GCM_CALLEE(SetTransferDataFormat)(CELL_GCM_THIS, inPitch, outPitch, lineLength, lineCount, inFormat, outFormat);
}

CELL_GCM_GLOBAL_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetTransferDataOffset)(CELL_GCM_ARGS(const uint32_t dstOffset, const uint32_t srcOffset))
{
	return CELL_GCM_CALLEE(SetTransferDataOffset)(CELL_GCM_THIS, dstOffset, srcOffset);
}

CELL_GCM_GLOBAL_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetTransferScaleMode)(CELL_GCM_ARGS(const uint8_t mode, const uint8_t surface))
{
	return CELL_GCM_CALLEE(SetTransferScaleMode)(CELL_GCM_THIS, mode, surface);
}

CELL_GCM_GLOBAL_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetTransferScaleSurface)(CELL_GCM_ARGS(const CellGcmTransferScale *scale, CellGcmTransferSurface *surface))
{
	return CELL_GCM_CALLEE(SetTransferScaleSurface)(CELL_GCM_THIS, scale, surface);
}

CELL_GCM_GLOBAL_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetTransferScaleSwizzle)(CELL_GCM_ARGS(const CellGcmTransferScale *scale, CellGcmTransferSwizzle *swizzle))
{
	return CELL_GCM_CALLEE(SetTransferScaleSwizzle)(CELL_GCM_THIS, scale, swizzle);
}

CELL_GCM_GLOBAL_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetDrawBegin)(CELL_GCM_ARGS(const uint8_t mode))
{
	return CELL_GCM_CALLEE(SetDrawBegin)(CELL_GCM_THIS, mode);
}

CELL_GCM_GLOBAL_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetDrawEnd)(CELL_GCM_NO_ARGS())
{
	return CELL_GCM_CALLEE(SetDrawEnd)(CELL_GCM_THIS);
}

CELL_GCM_GLOBAL_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetDrawInlineArrayPointer)(CELL_GCM_ARGS(const uint32_t count, void **pointer))
{
	return CELL_GCM_CALLEE(SetDrawInlineArrayPointer)(CELL_GCM_THIS, count, pointer);
}

CELL_GCM_GLOBAL_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetVertexProgramConstantsPointer)(CELL_GCM_ARGS(uint32_t first, uint32_t count, void **pointer))
{
	return CELL_GCM_CALLEE(SetVertexProgramConstantsPointer)(CELL_GCM_THIS, first, count, pointer);
}

CELL_GCM_GLOBAL_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetDrawInlineIndexArray32Pointer)(CELL_GCM_ARGS(const uint32_t count, void **pointer))
{
	return CELL_GCM_CALLEE(SetDrawInlineIndexArray32Pointer)(CELL_GCM_THIS, count, pointer);
}

CELL_GCM_GLOBAL_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetDrawInlineIndexArray16Pointer)(CELL_GCM_ARGS(const uint32_t count, void **pointer))
{
	return CELL_GCM_CALLEE(SetDrawInlineIndexArray16Pointer)(CELL_GCM_THIS, count, pointer);
}

CELL_GCM_GLOBAL_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetAnisoSpread)(CELL_GCM_ARGS(const uint8_t index, const uint8_t reduceSamplesEnable, const uint8_t hReduceSamplesEnable, const uint8_t vReduceSamplesEnable, const uint8_t spacingSelect, const uint8_t hSpacingSelect, const uint8_t vSpacingSelect))
{
	return CELL_GCM_CALLEE(SetAnisoSpread)(CELL_GCM_THIS, index, reduceSamplesEnable, hReduceSamplesEnable, vReduceSamplesEnable, spacingSelect, hSpacingSelect, vSpacingSelect);
}

CELL_GCM_GLOBAL_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetTextureAddressAnisoBias)(CELL_GCM_ARGS(const uint8_t index, const uint8_t wraps, const uint8_t wrapt, const uint8_t wrapr, const uint8_t unsignedRemap, const uint8_t zfunc, const uint8_t gamma, const uint8_t anisoBias))
{
	return CELL_GCM_CALLEE(SetTextureAddressAnisoBias)(CELL_GCM_THIS, index, wraps, wrapt, wrapr, unsignedRemap, zfunc, gamma, anisoBias);
}

CELL_GCM_GLOBAL_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetVertexProgramParameterBlockPointer)(CELL_GCM_ARGS(const uint32_t baseConst, const uint32_t constCount, void **pointer))
{
	return CELL_GCM_CALLEE(SetVertexProgramParameterBlockPointer)(CELL_GCM_THIS, baseConst, constCount, pointer);
}

CELL_GCM_GLOBAL_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetVertexDataArrayFormat)(CELL_GCM_ARGS(const uint8_t index, const uint16_t frequency, const uint8_t stride, const uint8_t size, const uint8_t type))
{
	return CELL_GCM_CALLEE(SetVertexDataArrayFormat)(CELL_GCM_THIS, index, frequency, stride, size, type);
}

CELL_GCM_GLOBAL_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetVertexDataArrayOffset)(CELL_GCM_ARGS(const uint8_t index, const uint8_t location, const uint32_t offset))
{
	return CELL_GCM_CALLEE(SetVertexDataArrayOffset)(CELL_GCM_THIS, index, location, offset);
}

#if (CELL_GCM_MEASURE != 2)
CELL_GCM_GLOBAL_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetInlineTransferAlignedPointer)(CELL_GCM_ARGS(const uint32_t offset, const uint32_t count, void **pointer))
{
	return CELL_GCM_CALLEE(SetInlineTransferAlignedPointer)(CELL_GCM_THIS, offset, count, pointer);
}

CELL_GCM_GLOBAL_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetVertexProgramConstantsAlignedPointer)(CELL_GCM_ARGS(uint32_t first, uint32_t count, void **pointer))
{
	return CELL_GCM_CALLEE(SetVertexProgramConstantsAlignedPointer)(CELL_GCM_THIS, first, count, pointer);
}

CELL_GCM_GLOBAL_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetVertexProgramParameterBlockAlignedPointer)(CELL_GCM_ARGS(const uint32_t baseConst, const uint32_t constCount, void **pointer))
{
	return CELL_GCM_CALLEE(SetVertexProgramParameterBlockAlignedPointer)(CELL_GCM_THIS, baseConst, constCount, pointer);
}

CELL_GCM_GLOBAL_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetDrawInlineArrayAlignedPointer)(CELL_GCM_ARGS(const uint32_t count, void **pointer))
{
	return CELL_GCM_CALLEE(SetDrawInlineArrayAlignedPointer)(CELL_GCM_THIS, count, pointer);
}

CELL_GCM_GLOBAL_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetDrawInlineIndexArray32AlignedPointer)(CELL_GCM_ARGS(const uint32_t count, void **pointer))
{
	return CELL_GCM_CALLEE(SetDrawInlineIndexArray32AlignedPointer)(CELL_GCM_THIS, count, pointer);
}

CELL_GCM_GLOBAL_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetDrawInlineIndexArray16AlignedPointer)(CELL_GCM_ARGS(const uint32_t count, void **pointer))
{
	return CELL_GCM_CALLEE(SetDrawInlineIndexArray16AlignedPointer)(CELL_GCM_THIS, count, pointer);
}
#endif	// (CELL_GCM_MEASURE != 2)

CELL_GCM_GLOBAL_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetFragmentProgramLoadLocation)(CELL_GCM_ARGS(const CellCgbFragmentProgramConfiguration *conf, const uint32_t location))
{
	return CELL_GCM_CALLEE(SetFragmentProgramLoadLocation)(CELL_GCM_THIS, conf, location);
}

CELL_GCM_GLOBAL_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetUpdateFragmentProgramParameterLocation)(CELL_GCM_ARGS(const uint32_t offset, const uint32_t location))
{
	return CELL_GCM_CALLEE(SetUpdateFragmentProgramParameterLocation)(CELL_GCM_THIS, offset, location);
}

CELL_GCM_GLOBAL_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetVertexDataBase)(CELL_GCM_ARGS(const uint32_t baseOffset, const uint32_t baseIndex))
{
	return CELL_GCM_CALLEE(SetVertexDataBase)(CELL_GCM_THIS, baseOffset, baseIndex);
}

CELL_GCM_GLOBAL_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetVertexProgramParameter)(CELL_GCM_ARGS(const CGparameter param, const float *value))
{
	return CELL_GCM_CALLEE(SetVertexProgramParameter)(CELL_GCM_THIS, param, value);
}

CELL_GCM_GLOBAL_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetVertexProgram)(CELL_GCM_ARGS(const CGprogram prog, const void *ucode))
{
	return CELL_GCM_CALLEE(SetVertexProgram)(CELL_GCM_THIS, prog, ucode);
}

CELL_GCM_GLOBAL_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetFragmentProgramOffset)(CELL_GCM_ARGS(const CGprogram prog, const uint32_t offset, const uint32_t location))
{
	return CELL_GCM_CALLEE(SetFragmentProgramOffset)(CELL_GCM_THIS, prog, offset, location);
}

CELL_GCM_GLOBAL_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetFragmentProgramControl)(CELL_GCM_ARGS(const CGprogram prog, const uint8_t controlTxp, const uint8_t reserved0, const uint8_t reserved1))
{
	return CELL_GCM_CALLEE(SetFragmentProgramControl)(CELL_GCM_THIS, prog, controlTxp, reserved0, reserved1);
}

CELL_GCM_GLOBAL_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetFragmentProgram)(CELL_GCM_ARGS(const CGprogram prog, uint32_t offset))
{
	return CELL_GCM_CALLEE(SetFragmentProgram)(CELL_GCM_THIS, prog, offset);
}

CELL_GCM_GLOBAL_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetFragmentProgramParameter)(CELL_GCM_ARGS(const CGprogram prog, const CGparameter param, const float *value, const uint32_t offset))
{
	return CELL_GCM_CALLEE(SetFragmentProgramParameter)(CELL_GCM_THIS, prog, param, value, offset);
}

CELL_GCM_GLOBAL_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetFragmentProgramParameterPointer)(CELL_GCM_ARGS(const CGprogram prog, const CGparameter param, const uint32_t offset, const uint32_t srcOffset))
{
	return CELL_GCM_CALLEE(SetFragmentProgramParameterPointer)(CELL_GCM_THIS, prog, param, offset, srcOffset);
}

CELL_GCM_GLOBAL_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetClearZcullSurface)(CELL_GCM_ARGS(const uint32_t depth, const uint32_t stencil))
{
	return CELL_GCM_CALLEE(SetClearZcullSurface)(CELL_GCM_THIS, depth, stencil);
}

CELL_GCM_GLOBAL_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetZcullEnable)(CELL_GCM_ARGS(const uint32_t depth, const uint32_t stencil))
{
	return CELL_GCM_CALLEE(SetZcullEnable)(CELL_GCM_THIS, depth, stencil);
}

CELL_GCM_GLOBAL_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetTransferReportData)(CELL_GCM_ARGS(const uint32_t offset, const uint32_t index, const uint32_t count))
{
	return CELL_GCM_CALLEE(SetTransferReportData)(CELL_GCM_THIS, offset, index, count);
}

CELL_GCM_GLOBAL_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetWriteBackEndLabelForConditional)(CELL_GCM_ARGS(uint8_t index, uint32_t value))
{
	return CELL_GCM_CALLEE(SetWriteBackEndLabelForConditional)(CELL_GCM_THIS, index, value);
}

CELL_GCM_GLOBAL_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetWriteTextureLabelForConditional)(CELL_GCM_ARGS(uint8_t index, uint32_t value))
{
	return CELL_GCM_CALLEE(SetWriteTextureLabelForConditional)(CELL_GCM_THIS, index, value);
}

CELL_GCM_GLOBAL_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetUserCommand)(CELL_GCM_ARGS(const uint32_t cause))
{
	return CELL_GCM_CALLEE(SetUserCommand)(CELL_GCM_THIS, cause);
}

CELL_GCM_GLOBAL_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetWaitFlip)(CELL_GCM_NO_ARGS())
{
	return CELL_GCM_CALLEE(SetWaitFlip)(CELL_GCM_THIS);
}

#if (CELL_GCM_UNSAFE==0)
CELL_GCM_GLOBAL_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(ReserveMethodSize)(CELL_GCM_ARGS(uint32_t size))
{
	return CELL_GCM_CALLEE(ReserveMethodSize)(CELL_GCM_THIS, size);
}
#endif

CELL_GCM_GLOBAL_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetReportLocation)(CELL_GCM_ARGS(const uint32_t location))
{
	return CELL_GCM_CALLEE(SetReportLocation)(CELL_GCM_THIS, location);
}

CELL_GCM_GLOBAL_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetTextureAddressAnisoBiasRemap)(CELL_GCM_ARGS(const uint8_t index, const uint8_t wraps, const uint8_t wrapt, const uint8_t wrapr, const uint8_t unsignedRemap, const uint8_t zfunc, const uint8_t gamma, const uint8_t anisoBias, const uint8_t signedRemap))
{
	return CELL_GCM_CALLEE(SetTextureAddressAnisoBiasRemap)(CELL_GCM_THIS, index, wraps, wrapt, wrapr, unsignedRemap, zfunc, gamma, anisoBias, signedRemap);
}

CELL_GCM_GLOBAL_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetLabelLocation)(CELL_GCM_ARGS(const uint32_t location))
{
	return CELL_GCM_CALLEE(SetLabelLocation)(CELL_GCM_THIS, location);
}

CELL_GCM_GLOBAL_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetSkipNop)(CELL_GCM_ARGS(uint32_t count))
{
	return CELL_GCM_CALLEE(SetSkipNop)(CELL_GCM_THIS, count);
}

CELL_GCM_GLOBAL_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetNotifyIndex)(CELL_GCM_ARGS(const uint32_t index))
{
	return CELL_GCM_CALLEE(SetNotifyIndex)(CELL_GCM_THIS, index);
}

CELL_GCM_GLOBAL_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetNotify)(CELL_GCM_NO_ARGS())
{
	return CELL_GCM_CALLEE(SetNotify)(CELL_GCM_THIS);
}

CELL_GCM_GLOBAL_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetWaitForIdle)(CELL_GCM_NO_ARGS())
{
	return CELL_GCM_CALLEE(SetWaitForIdle)(CELL_GCM_THIS);
}

CELL_GCM_GLOBAL_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetVertexData3f)(CELL_GCM_ARGS(const uint8_t index, const float v[3]))
{
	return CELL_GCM_CALLEE(SetVertexData3f)(CELL_GCM_THIS, index, v);
}

CELL_GCM_GLOBAL_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetVertexData2f)(CELL_GCM_ARGS(const uint8_t index, const float v[2]))
{
	return CELL_GCM_CALLEE(SetVertexData2f)(CELL_GCM_THIS, index, v);
}

CELL_GCM_GLOBAL_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetVertexData1f)(CELL_GCM_ARGS(const uint8_t index, const float v))
{
	return CELL_GCM_CALLEE(SetVertexData1f)(CELL_GCM_THIS, index, v);
}

CELL_GCM_GLOBAL_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetVertexData4s)(CELL_GCM_ARGS(const uint8_t index, const int16_t v[4]))
{
	return CELL_GCM_CALLEE(SetVertexData4s)(CELL_GCM_THIS, index, v);
}

CELL_GCM_GLOBAL_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetVertexDataScaled4s)(CELL_GCM_ARGS(const uint8_t index, const int16_t v[4]))
{
	return CELL_GCM_CALLEE(SetVertexDataScaled4s)(CELL_GCM_THIS, index, v);
}

CELL_GCM_GLOBAL_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetVertexData2s)(CELL_GCM_ARGS(const uint8_t index, const int16_t v[2]))
{
	return CELL_GCM_CALLEE(SetVertexData2s)(CELL_GCM_THIS, index, v);
}

CELL_GCM_GLOBAL_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetVertexData4ub)(CELL_GCM_ARGS(const uint8_t index, const uint8_t v[4]))
{
	return CELL_GCM_CALLEE(SetVertexData4ub)(CELL_GCM_THIS, index, v);
}
