/*   SCE CONFIDENTIAL                                       */
/*   PlayStation(R)3 Programmer Tool Runtime Library 310.001 */
/*   Copyright (C) 2007 Sony Computer Entertainment Inc.    */
/*   All Rights Reserved.                                   */

//
// create command template
//
template <class Context>
void createCommand(Context &context)
{
	uint8_t i;

	context.SetAlphaFunc(CELL_GCM_ALWAYS, 0);
	context.SetAlphaTestEnable(CELL_GCM_FALSE);

	context.SetBackStencilFunc(CELL_GCM_ALWAYS, 0, 0xff);
	context.SetBackStencilMask(0xff);
	context.SetBackStencilOp(CELL_GCM_KEEP, CELL_GCM_KEEP, CELL_GCM_KEEP);
	context.SetBlendColor(0, 0);
	context.SetBlendEnable(CELL_GCM_FALSE);
	context.SetBlendEnableMrt(CELL_GCM_FALSE, CELL_GCM_FALSE, CELL_GCM_FALSE);
	context.SetBlendEquation(CELL_GCM_FUNC_ADD, CELL_GCM_FUNC_ADD);
	context.SetBlendFunc(CELL_GCM_ONE, CELL_GCM_ZERO, CELL_GCM_ONE, CELL_GCM_ZERO);

	context.SetClearDepthStencil(0xffffff00);
	context.SetClearSurface(0);
	context.SetColorMask(CELL_GCM_COLOR_MASK_A|CELL_GCM_COLOR_MASK_R|CELL_GCM_COLOR_MASK_G|CELL_GCM_COLOR_MASK_B);
	context.SetCullFaceEnable(CELL_GCM_FALSE);
	context.SetCullFace(CELL_GCM_BACK);

	context.SetDepthBounds(0.0f, 1.0f);
	context.SetDepthBoundsTestEnable(CELL_GCM_FALSE);
	context.SetDepthFunc(CELL_GCM_LESS);
	context.SetDepthMask(CELL_GCM_TRUE);
	context.SetDepthTestEnable(CELL_GCM_FALSE);
	context.SetDitherEnable(CELL_GCM_TRUE);

	context.SetFragmentProgramGammaEnable(CELL_GCM_FALSE);
	context.SetFrequencyDividerOperation(0);
	context.SetFrontFace(CELL_GCM_CCW);

	context.SetLineWidth(8);	// fixed point [0:6:3]
	context.SetLogicOpEnable(CELL_GCM_FALSE);
	context.SetLogicOp(CELL_GCM_COPY);

	context.SetPointSize(1.0f);
	context.SetPolygonOffsetFillEnable(CELL_GCM_FALSE);
	context.SetPolygonOffset(0.0f, 0.0f);

	context.SetRestartIndexEnable(CELL_GCM_FALSE);
	context.SetRestartIndex(0xffffffff);

	context.SetScissor(0,0,4096,4096);
	context.SetShadeMode(CELL_GCM_SMOOTH);
	context.SetStencilFunc(CELL_GCM_ALWAYS, 0, 0xff);
	context.SetStencilMask(0xff);
	context.SetStencilOp(CELL_GCM_KEEP, CELL_GCM_KEEP, CELL_GCM_KEEP);
	context.SetStencilTestEnable(CELL_GCM_FALSE);

	for(i=0;i<16;i++){
		context.SetTextureAddress(i, CELL_GCM_TEXTURE_WRAP, CELL_GCM_TEXTURE_WRAP, CELL_GCM_TEXTURE_CLAMP_TO_EDGE, CELL_GCM_TEXTURE_UNSIGNED_REMAP_NORMAL, CELL_GCM_TEXTURE_ZFUNC_NEVER, 0);
		context.SetTextureBorderColor(i, 0);
		context.SetTextureControl(i, CELL_GCM_FALSE, 0, 12<<8, CELL_GCM_TEXTURE_MAX_ANISO_1);
		context.SetTextureFilter(i, 0, CELL_GCM_TEXTURE_NEAREST_LINEAR, CELL_GCM_TEXTURE_LINEAR, CELL_GCM_TEXTURE_CONVOLUTION_QUINCUNX);
	}

	context.SetTwoSidedStencilTestEnable(CELL_GCM_FALSE);

	for(i=0;i<16;i++){
		context.SetVertexDataArray(i, 0, 0, 0, CELL_GCM_VERTEX_F, CELL_GCM_LOCATION_LOCAL, 0);
	}

	float scale[4] = {2048.0f, 2048.0f, 0.5f, 0.0f};	// w/2, h/2, (max-min)/2, 0
	float offset[4] = {2048.0f, 2048.0f, 0.5f, 0.0f};	// w/2, h/2, (max-min)/2, 0
	context.SetViewport(0, 0, 4096, 4096, 0.0f, 1.0f, scale, offset);

	context.SetAntiAliasingControl(CELL_GCM_FALSE, CELL_GCM_FALSE, CELL_GCM_FALSE, 0xffff);

	context.SetBackPolygonMode(CELL_GCM_POLYGON_MODE_FILL);

	context.SetClearColor(0);

	context.SetColorMaskMrt(0);
	context.SetFrontPolygonMode(CELL_GCM_POLYGON_MODE_FILL);
	context.SetLineSmoothEnable(CELL_GCM_FALSE);


	context.SetLineStippleEnable(CELL_GCM_FALSE);
	context.SetPointSpriteControl(CELL_GCM_FALSE, 0, 0);
	context.SetPolySmoothEnable(CELL_GCM_FALSE);
	context.SetPolygonStippleEnable(CELL_GCM_FALSE);

	context.SetRenderEnable(CELL_GCM_TRUE, 0);

	context.SetUserClipPlaneControl(CELL_GCM_FALSE,CELL_GCM_FALSE,CELL_GCM_FALSE,CELL_GCM_FALSE,CELL_GCM_FALSE,CELL_GCM_FALSE);

	context.SetVertexAttribInputMask(0xffff);

	context.SetZpassPixelCountEnable(CELL_GCM_FALSE);

	for(i=0;i<4;i++){
		context.SetVertexTextureAddress(i, CELL_GCM_TEXTURE_WRAP, CELL_GCM_TEXTURE_WRAP);
		context.SetVertexTextureBorderColor(i, 0);
		context.SetVertexTextureControl(i, CELL_GCM_FALSE, 0, 12<<8);
		context.SetVertexTextureFilter(i, 0);
	}


	context.SetCylindricalWrap(0,0);
	context.SetZMinMaxControl(CELL_GCM_TRUE, CELL_GCM_FALSE, CELL_GCM_FALSE);
	context.SetTwoSideLightEnable(CELL_GCM_FALSE);
	context.SetTransformBranchBits(0);

	context.SetReturnCommand();
}

