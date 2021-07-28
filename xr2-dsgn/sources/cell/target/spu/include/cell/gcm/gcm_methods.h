/*   SCE CONFIDENTIAL                                       */
/*   PlayStation(R)3 Programmer Tool Runtime Library 310.001 */
/*   Copyright (C) 2008 Sony Computer Entertainment Inc.    */
/*   All Rights Reserved.                                   */

#undef CELL_GCM_METHOD_SET_ALPHA_FUNC_REF
#define CELL_GCM_METHOD_SET_ALPHA_FUNC_REF(cmd, af, ref) do { \
	(cmd)[0] = CELL_GCM_METHOD(CELL_GCM_NV4097_SET_ALPHA_FUNC, 2); \
	(cmd)[1] = CELL_GCM_ENDIAN_SWAP(af); \
	(cmd)[2] = CELL_GCM_ENDIAN_SWAP(ref); \
	CELL_GCM_DEBUG_CHECK(cmd); \
	(cmd) += 3; \
	} while(false)

#undef CELL_GCM_METHOD_SET_BLEND_COLOR
#define CELL_GCM_METHOD_SET_BLEND_COLOR(cmd, color) do { \
	(cmd)[0] = CELL_GCM_METHOD(CELL_GCM_NV4097_SET_BLEND_COLOR, 1); \
	(cmd)[1] = CELL_GCM_ENDIAN_SWAP(color); \
	CELL_GCM_DEBUG_CHECK(cmd); \
	(cmd) += 2; \
	} while(false)

#undef CELL_GCM_METHOD_SET_BLEND_COLOR2
#define CELL_GCM_METHOD_SET_BLEND_COLOR2(cmd, color) do { \
	(cmd)[0] = CELL_GCM_METHOD(CELL_GCM_NV4097_SET_BLEND_COLOR2, 1); \
	(cmd)[1] = CELL_GCM_ENDIAN_SWAP(color); \
	CELL_GCM_DEBUG_CHECK(cmd); \
	(cmd) += 2; \
	} while(false)

#undef CELL_GCM_METHOD_SET_BLEND_EQUATION
#define CELL_GCM_METHOD_SET_BLEND_EQUATION(cmd, color, alpha) do { \
	(cmd)[0] = CELL_GCM_METHOD(CELL_GCM_NV4097_SET_BLEND_EQUATION, 1); \
	(cmd)[1] = CELL_GCM_ENDIAN_SWAP((color) | ((alpha) << 16)); \
	CELL_GCM_DEBUG_CHECK(cmd); \
	(cmd) += 2; \
	} while(false)

#undef CELL_GCM_METHOD_SET_BLEND_FUNC_SFACTOR_DFACTOR
#define CELL_GCM_METHOD_SET_BLEND_FUNC_SFACTOR_DFACTOR(cmd, sfcolor, sfalpha, dfcolor, dfalpha) do { \
	(cmd)[0] = CELL_GCM_METHOD(CELL_GCM_NV4097_SET_BLEND_FUNC_SFACTOR, 2); \
	(cmd)[1] = CELL_GCM_ENDIAN_SWAP((sfcolor) | ((sfalpha) << 16)); \
	(cmd)[2] = CELL_GCM_ENDIAN_SWAP((dfcolor) | ((dfalpha) << 16)); \
	CELL_GCM_DEBUG_CHECK(cmd); \
	(cmd) += 3; \
	} while(false)

#undef CELL_GCM_METHOD_CLEAR_SURFACE
#define CELL_GCM_METHOD_CLEAR_SURFACE(cmd, mask) do { \
	(cmd)[0] = CELL_GCM_METHOD(CELL_GCM_NV4097_CLEAR_SURFACE, 1); \
	(cmd)[1] = CELL_GCM_ENDIAN_SWAP(mask); \
	CELL_GCM_DEBUG_CHECK(cmd); \
	(cmd) += 2; \
	} while(false)

#undef CELL_GCM_METHOD_NO_OPERATION
#define CELL_GCM_METHOD_NO_OPERATION(cmd) do { \
	(cmd)[0] = CELL_GCM_METHOD(CELL_GCM_NV4097_NO_OPERATION, 1); \
	(cmd)[1] = 0; \
	CELL_GCM_DEBUG_CHECK(cmd); \
	(cmd) += 2; \
	} while(false)

#undef CELL_GCM_METHOD_SET_COLOR_CLEAR_VALUE
#define CELL_GCM_METHOD_SET_COLOR_CLEAR_VALUE(cmd, color) do { \
	(cmd)[0] = CELL_GCM_METHOD(CELL_GCM_NV4097_SET_COLOR_CLEAR_VALUE, 1); \
	(cmd)[1] = CELL_GCM_ENDIAN_SWAP(color); \
	CELL_GCM_DEBUG_CHECK(cmd); \
	(cmd) += 2; \
	} while(false)

#undef CELL_GCM_METHOD_SET_ZSTENCIL_CLEAR_VALUE
#define CELL_GCM_METHOD_SET_ZSTENCIL_CLEAR_VALUE(cmd, depthStencil) do { \
	(cmd)[0] = CELL_GCM_METHOD(CELL_GCM_NV4097_SET_ZSTENCIL_CLEAR_VALUE, 1); \
	(cmd)[1] = CELL_GCM_ENDIAN_SWAP(depthStencil); \
	CELL_GCM_DEBUG_CHECK(cmd); \
	(cmd) += 2; \
	} while(false)

#undef CELL_GCM_METHOD_SET_COLOR_MASK
#define CELL_GCM_METHOD_SET_COLOR_MASK(cmd, mask) do { \
	(cmd)[0] = CELL_GCM_METHOD(CELL_GCM_NV4097_SET_COLOR_MASK, 1); \
	(cmd)[1] = CELL_GCM_ENDIAN_SWAP(mask); \
	CELL_GCM_DEBUG_CHECK(cmd); \
	(cmd) += 2; \
	} while(false)

#undef CELL_GCM_METHOD_SET_COLOR_MASK_MRT
#define CELL_GCM_METHOD_SET_COLOR_MASK_MRT(cmd, mask) do { \
	(cmd)[0] = CELL_GCM_METHOD(CELL_GCM_NV4097_SET_COLOR_MASK_MRT, 1); \
	(cmd)[1] = CELL_GCM_ENDIAN_SWAP(mask); \
	CELL_GCM_DEBUG_CHECK(cmd); \
	(cmd) += 2; \
	} while(false)

#undef CELL_GCM_METHOD_SET_CULL_FACE
#define CELL_GCM_METHOD_SET_CULL_FACE(cmd, cfm) do { \
	(cmd)[0] = CELL_GCM_METHOD(CELL_GCM_NV4097_SET_CULL_FACE, 1); \
	(cmd)[1] = CELL_GCM_ENDIAN_SWAP(cfm); \
	CELL_GCM_DEBUG_CHECK(cmd); \
	(cmd) += 2; \
	} while(false)

#undef CELL_GCM_METHOD_SET_DEPTH_BOUNDS_MIN_MAX
#define CELL_GCM_METHOD_SET_DEPTH_BOUNDS_MIN_MAX(cmd, zmin, zmax) do { \
	(cmd)[0] = CELL_GCM_METHOD(CELL_GCM_NV4097_SET_DEPTH_BOUNDS_MIN, 2); \
	(cmd)[1] = CELL_GCM_ENDIAN_SWAP(zmin); \
	(cmd)[2] = CELL_GCM_ENDIAN_SWAP(zmax); \
	CELL_GCM_DEBUG_CHECK(cmd); \
	(cmd) += 3; \
	} while(false)

#undef CELL_GCM_METHOD_SET_DEPTH_FUNC
#define CELL_GCM_METHOD_SET_DEPTH_FUNC(cmd, zf) do { \
	(cmd)[0] = CELL_GCM_METHOD(CELL_GCM_NV4097_SET_DEPTH_FUNC, 1); \
	(cmd)[1] = CELL_GCM_ENDIAN_SWAP(zf); \
	CELL_GCM_DEBUG_CHECK(cmd); \
	(cmd) += 2; \
	} while(false)

#undef CELL_GCM_METHOD_SET_DEPTH_MASK
#define CELL_GCM_METHOD_SET_DEPTH_MASK(cmd, enable) do { \
	(cmd)[0] = CELL_GCM_METHOD(CELL_GCM_NV4097_SET_DEPTH_MASK, 1); \
	(cmd)[1] = CELL_GCM_ENDIAN_SWAP(enable); \
	CELL_GCM_DEBUG_CHECK(cmd); \
	(cmd) += 2; \
	} while(false)

#undef CELL_GCM_METHOD_SET_FRONT_FACE
#define CELL_GCM_METHOD_SET_FRONT_FACE(cmd, dir) do { \
	(cmd)[0] = CELL_GCM_METHOD(CELL_GCM_NV4097_SET_FRONT_FACE, 1); \
	(cmd)[1] = CELL_GCM_ENDIAN_SWAP(dir); \
	CELL_GCM_DEBUG_CHECK(cmd); \
	(cmd) += 2; \
	} while(false)

#undef CELL_GCM_METHOD_SET_LINE_WIDTH
#define CELL_GCM_METHOD_SET_LINE_WIDTH(cmd, width) do { \
	(cmd)[0] = CELL_GCM_METHOD(CELL_GCM_NV4097_SET_LINE_WIDTH, 1); \
	(cmd)[1] = CELL_GCM_ENDIAN_SWAP(width); \
	CELL_GCM_DEBUG_CHECK(cmd); \
	(cmd) += 2; \
	} while(false)

#undef CELL_GCM_METHOD_SET_LINE_SMOOTH_ENABLE
#define CELL_GCM_METHOD_SET_LINE_SMOOTH_ENABLE(cmd, enable) do { \
	(cmd)[0] = CELL_GCM_METHOD(CELL_GCM_NV4097_SET_LINE_SMOOTH_ENABLE, 1); \
	(cmd)[1] = CELL_GCM_ENDIAN_SWAP(enable); \
	CELL_GCM_DEBUG_CHECK(cmd); \
	(cmd) += 2; \
	} while(false)

#undef CELL_GCM_METHOD_SET_LINE_STIPPLE
#define CELL_GCM_METHOD_SET_LINE_STIPPLE(cmd, enable) do { \
	(cmd)[0] = CELL_GCM_METHOD(CELL_GCM_NV4097_SET_LINE_STIPPLE, 1); \
	(cmd)[1] = CELL_GCM_ENDIAN_SWAP(enable); \
	CELL_GCM_DEBUG_CHECK(cmd); \
	(cmd) += 2; \
	} while(false)

#undef CELL_GCM_METHOD_SET_LINE_STIPPLE_PATTERN
#define CELL_GCM_METHOD_SET_LINE_STIPPLE_PATTERN(cmd, pattern, factor) do { \
	(cmd)[0] = CELL_GCM_METHOD(CELL_GCM_NV4097_SET_LINE_STIPPLE_PATTERN, 1); \
	(cmd)[1] = CELL_GCM_ENDIAN_SWAP((factor) | ((pattern) << 16)); \
	CELL_GCM_DEBUG_CHECK(cmd); \
	(cmd) += 2; \
	} while(false)

#undef CELL_GCM_METHOD_SET_LOGIC_OP
#define CELL_GCM_METHOD_SET_LOGIC_OP(cmd, op) do { \
	(cmd)[0] = CELL_GCM_METHOD(CELL_GCM_NV4097_SET_LOGIC_OP, 1); \
	(cmd)[1] = CELL_GCM_ENDIAN_SWAP(op); \
	CELL_GCM_DEBUG_CHECK(cmd); \
	(cmd) += 2; \
	} while(false)

#undef CELL_GCM_METHOD_SET_POINT_SIZE
#define CELL_GCM_METHOD_SET_POINT_SIZE(cmd, size) do { \
	(cmd)[0] = CELL_GCM_METHOD(CELL_GCM_NV4097_SET_POINT_SIZE, 1); \
	(cmd)[1] = CELL_GCM_ENDIAN_SWAP(size); \
	CELL_GCM_DEBUG_CHECK(cmd); \
	(cmd) += 2; \
	} while(false)

#undef CELL_GCM_METHOD_SET_POLYGON_OFFSET_SCALE_FACTOR_BIAS
#define CELL_GCM_METHOD_SET_POLYGON_OFFSET_SCALE_FACTOR_BIAS(cmd, factor, bias) do { \
	(cmd)[0] = CELL_GCM_METHOD(CELL_GCM_NV4097_SET_POLYGON_OFFSET_SCALE_FACTOR, 2); \
	(cmd)[1] = CELL_GCM_ENDIAN_SWAP(factor); \
	(cmd)[2] = CELL_GCM_ENDIAN_SWAP(bias); \
	CELL_GCM_DEBUG_CHECK(cmd); \
	(cmd) += 3; \
	} while(false)

#undef CELL_GCM_METHOD_SET_POLY_SMOOTH_ENABLE
#define CELL_GCM_METHOD_SET_POLY_SMOOTH_ENABLE(cmd, enable) do { \
	(cmd)[0] = CELL_GCM_METHOD(CELL_GCM_NV4097_SET_POLY_SMOOTH_ENABLE, 1); \
	(cmd)[1] = CELL_GCM_ENDIAN_SWAP(enable); \
	CELL_GCM_DEBUG_CHECK(cmd); \
	(cmd) += 2; \
	} while(false)

#undef CELL_GCM_METHOD_SET_POLYGON_STIPPLE
#define CELL_GCM_METHOD_SET_POLYGON_STIPPLE(cmd, enable) do { \
	(cmd)[0] = CELL_GCM_METHOD(CELL_GCM_NV4097_SET_POLYGON_STIPPLE, 1); \
	(cmd)[1] = CELL_GCM_ENDIAN_SWAP(enable); \
	CELL_GCM_DEBUG_CHECK(cmd); \
	(cmd) += 2; \
	} while(false)

#undef CELL_GCM_METHOD_SET_POLYGON_STIPPLE_PATTERN
#define CELL_GCM_METHOD_SET_POLYGON_STIPPLE_PATTERN(cmd, d0,d1,d2,d3,d4,d5,d6,d7,d8,d9,d10,d11,d12,d13,d14,d15,d16,d17,d18,d19,d20,d21,d22,d23,d24,d25,d26,d27,d28,d29,d30,d31) do { \
	(cmd)[0] = CELL_GCM_METHOD(CELL_GCM_NV4097_SET_POLYGON_STIPPLE_PATTERN, 32); \
	(cmd)[1] = CELL_GCM_ENDIAN_SWAP(d0); \
	(cmd)[2] = CELL_GCM_ENDIAN_SWAP(d1); \
	(cmd)[3] = CELL_GCM_ENDIAN_SWAP(d2); \
	(cmd)[4] = CELL_GCM_ENDIAN_SWAP(d3); \
	(cmd)[5] = CELL_GCM_ENDIAN_SWAP(d4); \
	(cmd)[6] = CELL_GCM_ENDIAN_SWAP(d5); \
	(cmd)[7] = CELL_GCM_ENDIAN_SWAP(d6); \
	(cmd)[8] = CELL_GCM_ENDIAN_SWAP(d7); \
	(cmd)[9] = CELL_GCM_ENDIAN_SWAP(d8); \
	(cmd)[10] = CELL_GCM_ENDIAN_SWAP(d9); \
	(cmd)[11] = CELL_GCM_ENDIAN_SWAP(d10); \
	(cmd)[12] = CELL_GCM_ENDIAN_SWAP(d11); \
	(cmd)[13] = CELL_GCM_ENDIAN_SWAP(d12); \
	(cmd)[14] = CELL_GCM_ENDIAN_SWAP(d13); \
	(cmd)[15] = CELL_GCM_ENDIAN_SWAP(d14); \
	(cmd)[16] = CELL_GCM_ENDIAN_SWAP(d15); \
	(cmd)[17] = CELL_GCM_ENDIAN_SWAP(d16); \
	(cmd)[18] = CELL_GCM_ENDIAN_SWAP(d17); \
	(cmd)[19] = CELL_GCM_ENDIAN_SWAP(d18); \
	(cmd)[20] = CELL_GCM_ENDIAN_SWAP(d19); \
	(cmd)[21] = CELL_GCM_ENDIAN_SWAP(d20); \
	(cmd)[22] = CELL_GCM_ENDIAN_SWAP(d21); \
	(cmd)[23] = CELL_GCM_ENDIAN_SWAP(d22); \
	(cmd)[24] = CELL_GCM_ENDIAN_SWAP(d23); \
	(cmd)[25] = CELL_GCM_ENDIAN_SWAP(d24); \
	(cmd)[26] = CELL_GCM_ENDIAN_SWAP(d25); \
	(cmd)[27] = CELL_GCM_ENDIAN_SWAP(d26); \
	(cmd)[28] = CELL_GCM_ENDIAN_SWAP(d27); \
	(cmd)[29] = CELL_GCM_ENDIAN_SWAP(d28); \
	(cmd)[30] = CELL_GCM_ENDIAN_SWAP(d29); \
	(cmd)[31] = CELL_GCM_ENDIAN_SWAP(d30); \
	(cmd)[32] = CELL_GCM_ENDIAN_SWAP(d31); \
	CELL_GCM_DEBUG_CHECK(cmd); \
	(cmd) += 33; \
	} while(false)

#undef CELL_GCM_METHOD_SET_FRONT_POLYGON_MODE
#define CELL_GCM_METHOD_SET_FRONT_POLYGON_MODE(cmd, mode) do { \
	(cmd)[0] = CELL_GCM_METHOD(CELL_GCM_NV4097_SET_FRONT_POLYGON_MODE, 1); \
	(cmd)[1] = CELL_GCM_ENDIAN_SWAP(mode); \
	CELL_GCM_DEBUG_CHECK(cmd); \
	(cmd) += 2; \
	} while(false)

#undef CELL_GCM_METHOD_SET_BACK_POLYGON_MODE
#define CELL_GCM_METHOD_SET_BACK_POLYGON_MODE(cmd, mode) do { \
	(cmd)[0] = CELL_GCM_METHOD(CELL_GCM_NV4097_SET_BACK_POLYGON_MODE, 1); \
	(cmd)[1] = CELL_GCM_ENDIAN_SWAP(mode); \
	CELL_GCM_DEBUG_CHECK(cmd); \
	(cmd) += 2; \
	} while(false)

#undef CELL_GCM_METHOD_SET_SCISSOR_HORIZONTAL_VERTICAL
#define CELL_GCM_METHOD_SET_SCISSOR_HORIZONTAL_VERTICAL(cmd, x, w, y, h) do { \
	(cmd)[0] = CELL_GCM_METHOD(CELL_GCM_NV4097_SET_SCISSOR_HORIZONTAL, 2); \
	(cmd)[1] = CELL_GCM_ENDIAN_SWAP((x) | ((w) << 16)); \
	(cmd)[2] = CELL_GCM_ENDIAN_SWAP((y) | ((h) << 16)); \
	CELL_GCM_DEBUG_CHECK(cmd); \
	(cmd) += 3; \
	} while(false)

#undef CELL_GCM_METHOD_SET_SHADE_MODE
#define CELL_GCM_METHOD_SET_SHADE_MODE(cmd, sm) do { \
	(cmd)[0] = CELL_GCM_METHOD(CELL_GCM_NV4097_SET_SHADE_MODE, 1); \
	(cmd)[1] = CELL_GCM_ENDIAN_SWAP(sm); \
	CELL_GCM_DEBUG_CHECK(cmd); \
	(cmd) += 2; \
	} while(false)

#undef CELL_GCM_METHOD_SET_TWO_SIDE_LIGHT_ENABLE
#define CELL_GCM_METHOD_SET_TWO_SIDE_LIGHT_ENABLE(cmd, enable) do { \
	(cmd)[0] = CELL_GCM_METHOD(CELL_GCM_NV4097_SET_TWO_SIDE_LIGHT_EN, 1); \
	(cmd)[1] = CELL_GCM_ENDIAN_SWAP(enable); \
	CELL_GCM_DEBUG_CHECK(cmd); \
	(cmd) += 2; \
	} while(false)

#undef CELL_GCM_METHOD_SET_STENCIL_FUNC_REF_MASK
#define CELL_GCM_METHOD_SET_STENCIL_FUNC_REF_MASK(cmd, func, ref, mask) do { \
	(cmd)[0] = CELL_GCM_METHOD(CELL_GCM_NV4097_SET_STENCIL_FUNC, 3); \
	(cmd)[1] = CELL_GCM_ENDIAN_SWAP(func); \
	(cmd)[2] = CELL_GCM_ENDIAN_SWAP(ref); \
	(cmd)[3] = CELL_GCM_ENDIAN_SWAP(mask); \
	CELL_GCM_DEBUG_CHECK(cmd); \
	(cmd) += 4; \
	} while(false)

#undef CELL_GCM_METHOD_SET_BACK_STENCIL_FUNC_REF_MASK
#define CELL_GCM_METHOD_SET_BACK_STENCIL_FUNC_REF_MASK(cmd, func, ref, mask) do { \
	(cmd)[0] = CELL_GCM_METHOD(CELL_GCM_NV4097_SET_BACK_STENCIL_FUNC, 3); \
	(cmd)[1] = CELL_GCM_ENDIAN_SWAP(func); \
	(cmd)[2] = CELL_GCM_ENDIAN_SWAP(ref); \
	(cmd)[3] = CELL_GCM_ENDIAN_SWAP(mask); \
	CELL_GCM_DEBUG_CHECK(cmd); \
	(cmd) += 4; \
	} while(false)

#undef CELL_GCM_METHOD_SET_STENCIL_MASK
#define CELL_GCM_METHOD_SET_STENCIL_MASK(cmd, sm) do { \
	(cmd)[0] = CELL_GCM_METHOD(CELL_GCM_NV4097_SET_STENCIL_MASK, 1); \
	(cmd)[1] = CELL_GCM_ENDIAN_SWAP(sm); \
	CELL_GCM_DEBUG_CHECK(cmd); \
	(cmd) += 2; \
	} while(false)

#undef CELL_GCM_METHOD_SET_BACK_STENCIL_MASK
#define CELL_GCM_METHOD_SET_BACK_STENCIL_MASK(cmd, sm) do { \
	(cmd)[0] = CELL_GCM_METHOD(CELL_GCM_NV4097_SET_BACK_STENCIL_MASK, 1); \
	(cmd)[1] = CELL_GCM_ENDIAN_SWAP(sm); \
	CELL_GCM_DEBUG_CHECK(cmd); \
	(cmd) += 2; \
	} while(false)

#undef CELL_GCM_METHOD_SET_STENCIL_OP_FAIL_ZFAIL_ZPASS
#define CELL_GCM_METHOD_SET_STENCIL_OP_FAIL_ZFAIL_ZPASS(cmd, sfail, dfail, dpass) do { \
	(cmd)[0] = CELL_GCM_METHOD(CELL_GCM_NV4097_SET_STENCIL_OP_FAIL, 3); \
	(cmd)[1] = CELL_GCM_ENDIAN_SWAP(sfail); \
	(cmd)[2] = CELL_GCM_ENDIAN_SWAP(dfail); \
	(cmd)[3] = CELL_GCM_ENDIAN_SWAP(dpass); \
	CELL_GCM_DEBUG_CHECK(cmd); \
	(cmd) += 4; \
	} while(false)

#undef CELL_GCM_METHOD_SET_BACK_STENCIL_OP_FAIL_ZFAIL_ZPASS
#define CELL_GCM_METHOD_SET_BACK_STENCIL_OP_FAIL_ZFAIL_ZPASS(cmd, sfail, dfail, dpass) do { \
	(cmd)[0] = CELL_GCM_METHOD(CELL_GCM_NV4097_SET_BACK_STENCIL_OP_FAIL, 3); \
	(cmd)[1] = CELL_GCM_ENDIAN_SWAP(sfail); \
	(cmd)[2] = CELL_GCM_ENDIAN_SWAP(dfail); \
	(cmd)[3] = CELL_GCM_ENDIAN_SWAP(dpass); \
	CELL_GCM_DEBUG_CHECK(cmd); \
	(cmd) += 4; \
	} while(false)

#undef CELL_GCM_METHOD_SET_VIEWPORT_HORIZONTAL_VERTICAL
#define CELL_GCM_METHOD_SET_VIEWPORT_HORIZONTAL_VERTICAL(cmd, x, w, y, h) do { \
	(cmd)[0] = CELL_GCM_METHOD(CELL_GCM_NV4097_SET_VIEWPORT_HORIZONTAL, 2); \
	(cmd)[1] = CELL_GCM_ENDIAN_SWAP((x) | ((w) << 16)); \
	(cmd)[2] = CELL_GCM_ENDIAN_SWAP((y) | ((h) << 16)); \
	CELL_GCM_DEBUG_CHECK(cmd); \
	(cmd) += 3; \
	} while(false)

#undef CELL_GCM_METHOD_SET_CLIP_MIN_MAX
#define CELL_GCM_METHOD_SET_CLIP_MIN_MAX(cmd, min, max) do { \
	(cmd)[0] = CELL_GCM_METHOD(CELL_GCM_NV4097_SET_CLIP_MIN, 2); \
	(cmd)[1] = CELL_GCM_ENDIAN_SWAP(min); \
	(cmd)[2] = CELL_GCM_ENDIAN_SWAP(max); \
	CELL_GCM_DEBUG_CHECK(cmd); \
	(cmd) += 3; \
	} while(false)

#undef CELL_GCM_METHOD_SET_VIEWPORT_OFFSET_SCALE
#define CELL_GCM_METHOD_SET_VIEWPORT_OFFSET_SCALE(cmd, offset0, offset1, offset2, offset3, scale0, scale1, scale2, scale3) do { \
	(cmd)[0] = CELL_GCM_METHOD(CELL_GCM_NV4097_SET_VIEWPORT_OFFSET, 8); \
	(cmd)[1] = CELL_GCM_ENDIAN_SWAP(offset0); \
	(cmd)[2] = CELL_GCM_ENDIAN_SWAP(offset1); \
	(cmd)[3] = CELL_GCM_ENDIAN_SWAP(offset2); \
	(cmd)[4] = CELL_GCM_ENDIAN_SWAP(offset3); \
	(cmd)[5] = CELL_GCM_ENDIAN_SWAP(scale0); \
	(cmd)[6] = CELL_GCM_ENDIAN_SWAP(scale1); \
	(cmd)[7] = CELL_GCM_ENDIAN_SWAP(scale2); \
	(cmd)[8] = CELL_GCM_ENDIAN_SWAP(scale3); \
	CELL_GCM_DEBUG_CHECK(cmd); \
	(cmd) += 9; \
	} while(false)

#undef CELL_GCM_METHOD_SET_ALPHA_TEST_ENABLE
#define CELL_GCM_METHOD_SET_ALPHA_TEST_ENABLE(cmd, enable) do { \
	(cmd)[0] = CELL_GCM_METHOD(CELL_GCM_NV4097_SET_ALPHA_TEST_ENABLE, 1); \
	(cmd)[1] = CELL_GCM_ENDIAN_SWAP(enable); \
	CELL_GCM_DEBUG_CHECK(cmd); \
	(cmd) += 2; \
	} while(false)

#undef CELL_GCM_METHOD_SET_BLEND_ENABLE
#define CELL_GCM_METHOD_SET_BLEND_ENABLE(cmd, enable) do { \
	(cmd)[0] = CELL_GCM_METHOD(CELL_GCM_NV4097_SET_BLEND_ENABLE, 1); \
	(cmd)[1] = CELL_GCM_ENDIAN_SWAP(enable); \
	CELL_GCM_DEBUG_CHECK(cmd); \
	(cmd) += 2; \
	} while(false)

#undef CELL_GCM_METHOD_SET_BLEND_ENABLE_MRT
#define CELL_GCM_METHOD_SET_BLEND_ENABLE_MRT(cmd, mrt1, mrt2, mrt3) do { \
	(cmd)[0] = CELL_GCM_METHOD(CELL_GCM_NV4097_SET_BLEND_ENABLE_MRT, 1); \
	(cmd)[1] = CELL_GCM_ENDIAN_SWAP(((mrt1) << 1)|((mrt2) << 2)|((mrt3) << 3)); \
	CELL_GCM_DEBUG_CHECK(cmd); \
	(cmd) += 2; \
	} while(false)

#undef CELL_GCM_METHOD_SET_LOGIC_OP_ENABLE
#define CELL_GCM_METHOD_SET_LOGIC_OP_ENABLE(cmd, enable) do { \
	(cmd)[0] = CELL_GCM_METHOD(CELL_GCM_NV4097_SET_LOGIC_OP_ENABLE, 1); \
	(cmd)[1] = CELL_GCM_ENDIAN_SWAP(enable); \
	CELL_GCM_DEBUG_CHECK(cmd); \
	(cmd) += 2; \
	} while(false)

#undef CELL_GCM_METHOD_SET_CULL_FACE_ENABLE
#define CELL_GCM_METHOD_SET_CULL_FACE_ENABLE(cmd, enable) do { \
	(cmd)[0] = CELL_GCM_METHOD(CELL_GCM_NV4097_SET_CULL_FACE_ENABLE, 1); \
	(cmd)[1] = CELL_GCM_ENDIAN_SWAP(enable); \
	CELL_GCM_DEBUG_CHECK(cmd); \
	(cmd) += 2; \
	} while(false)

#undef CELL_GCM_METHOD_SET_DEPTH_BOUNDS_TEST_ENABLE
#define CELL_GCM_METHOD_SET_DEPTH_BOUNDS_TEST_ENABLE(cmd, enable) do { \
	(cmd)[0] = CELL_GCM_METHOD(CELL_GCM_NV4097_SET_DEPTH_BOUNDS_TEST_ENABLE, 1); \
	(cmd)[1] = CELL_GCM_ENDIAN_SWAP(enable); \
	CELL_GCM_DEBUG_CHECK(cmd); \
	(cmd) += 2; \
	} while(false)

#undef CELL_GCM_METHOD_SET_DEPTH_TEST_ENABLE
#define CELL_GCM_METHOD_SET_DEPTH_TEST_ENABLE(cmd, enable) do { \
	(cmd)[0] = CELL_GCM_METHOD(CELL_GCM_NV4097_SET_DEPTH_TEST_ENABLE, 1); \
	(cmd)[1] = CELL_GCM_ENDIAN_SWAP(enable); \
	CELL_GCM_DEBUG_CHECK(cmd); \
	(cmd) += 2; \
	} while(false)

#undef CELL_GCM_METHOD_SET_DITHER_ENABLE
#define CELL_GCM_METHOD_SET_DITHER_ENABLE(cmd, enable) do { \
	(cmd)[0] = CELL_GCM_METHOD(CELL_GCM_NV4097_SET_DITHER_ENABLE, 1); \
	(cmd)[1] = CELL_GCM_ENDIAN_SWAP(enable); \
	CELL_GCM_DEBUG_CHECK(cmd); \
	(cmd) += 2; \
	} while(false)

#undef CELL_GCM_METHOD_SET_STENCIL_TEST_ENABLE
#define CELL_GCM_METHOD_SET_STENCIL_TEST_ENABLE(cmd, enable) do { \
	(cmd)[0] = CELL_GCM_METHOD(CELL_GCM_NV4097_SET_STENCIL_TEST_ENABLE, 1); \
	(cmd)[1] = CELL_GCM_ENDIAN_SWAP(enable); \
	CELL_GCM_DEBUG_CHECK(cmd); \
	(cmd) += 2; \
	} while(false)

#undef CELL_GCM_METHOD_SET_TWO_SIDED_STENCIL_TEST_ENABLE
#define CELL_GCM_METHOD_SET_TWO_SIDED_STENCIL_TEST_ENABLE(cmd, enable) do { \
	(cmd)[0] = CELL_GCM_METHOD(CELL_GCM_NV4097_SET_TWO_SIDED_STENCIL_TEST_ENABLE, 1); \
	(cmd)[1] = CELL_GCM_ENDIAN_SWAP(enable); \
	CELL_GCM_DEBUG_CHECK(cmd); \
	(cmd) += 2; \
	} while(false)

#undef CELL_GCM_METHOD_SET_POLY_OFFSET_FILL_ENABLE
#define CELL_GCM_METHOD_SET_POLY_OFFSET_FILL_ENABLE(cmd, enable) do { \
	(cmd)[0] = CELL_GCM_METHOD(CELL_GCM_NV4097_SET_POLY_OFFSET_FILL_ENABLE, 1); \
	(cmd)[1] = CELL_GCM_ENDIAN_SWAP(enable); \
	CELL_GCM_DEBUG_CHECK(cmd); \
	(cmd) += 2; \
	} while(false)

#undef CELL_GCM_METHOD_SET_RESTART_INDEX_ENABLE
#define CELL_GCM_METHOD_SET_RESTART_INDEX_ENABLE(cmd, enable) do { \
	(cmd)[0] = CELL_GCM_METHOD(CELL_GCM_NV4097_SET_RESTART_INDEX_ENABLE, 1); \
	(cmd)[1] = CELL_GCM_ENDIAN_SWAP(enable); \
	CELL_GCM_DEBUG_CHECK(cmd); \
	(cmd) += 2; \
	} while(false)

#undef CELL_GCM_METHOD_SET_POINT_PARAMS_ENABLE
#define CELL_GCM_METHOD_SET_POINT_PARAMS_ENABLE(cmd, enable) do { \
	(cmd)[0] = CELL_GCM_METHOD(CELL_GCM_NV4097_SET_POINT_PARAMS_ENABLE, 1); \
	(cmd)[1] = CELL_GCM_ENDIAN_SWAP(enable); \
	CELL_GCM_DEBUG_CHECK(cmd); \
	(cmd) += 2; \
	} while(false)

#undef CELL_GCM_METHOD_SET_POINT_SPRITE_CONTROL
#define CELL_GCM_METHOD_SET_POINT_SPRITE_CONTROL(cmd, enable, rmode, texcoordMask) do { \
	(cmd)[0] = CELL_GCM_METHOD(CELL_GCM_NV4097_SET_POINT_SPRITE_CONTROL, 1); \
	(cmd)[1] = CELL_GCM_ENDIAN_SWAP((enable) | ((rmode) << 1) | (texcoordMask)); \
	CELL_GCM_DEBUG_CHECK(cmd); \
	(cmd) += 2; \
	} while(false)

#undef CELL_GCM_METHOD_INVALIDATE_L2
#define CELL_GCM_METHOD_INVALIDATE_L2(cmd, value) do { \
	(cmd)[0] = CELL_GCM_METHOD(CELL_GCM_NV4097_INVALIDATE_L2, 1); \
	(cmd)[1] = CELL_GCM_ENDIAN_SWAP(value); \
	CELL_GCM_DEBUG_CHECK(cmd); \
	(cmd) += 2; \
	} while(false)

#undef CELL_GCM_METHOD_SET_TEXTURE_BORDER_COLOR
#define CELL_GCM_METHOD_SET_TEXTURE_BORDER_COLOR(cmd, index, color) do { \
	(cmd)[0] = CELL_GCM_METHOD(CELL_GCM_NV4097_SET_TEXTURE_BORDER_COLOR + 0x20 * (index), 1); \
	(cmd)[1] = CELL_GCM_ENDIAN_SWAP(color); \
	CELL_GCM_DEBUG_CHECK(cmd); \
	(cmd) += 2; \
	} while(false)

#undef CELL_GCM_METHOD_SET_TEXTURE_FILTER
#define CELL_GCM_METHOD_SET_TEXTURE_FILTER(cmd, index, bias, min, mag, conv, as, rs, gs, bs) do { \
	(cmd)[0] = CELL_GCM_METHOD(CELL_GCM_NV4097_SET_TEXTURE_FILTER + 0x20 * (index), 1); \
	(cmd)[1] = CELL_GCM_ENDIAN_SWAP((bias) | ((conv) << 13) | ((min) << 16) | ((mag) << 24) | ((as) << 28) | ((rs) << 29) | ((gs) << 30) | ((bs) << 31) ); \
	CELL_GCM_DEBUG_CHECK(cmd); \
	(cmd) += 2; \
	} while(false)

#undef CELL_GCM_METHOD_SET_TEXTURE_ADDRESS
#define CELL_GCM_METHOD_SET_TEXTURE_ADDRESS(cmd, index, wraps, wrapt, wrapr, unsignedRemap, zfunc, gamma, anisoBias, signedRemap) do { \
	(cmd)[0] = CELL_GCM_METHOD(CELL_GCM_NV4097_SET_TEXTURE_ADDRESS + 0x20 * (index), 1); \
	(cmd)[1] = CELL_GCM_ENDIAN_SWAP((wraps) | ((anisoBias) << 4) | ((wrapt) << 8) | ((unsignedRemap) << 12) | ((wrapr) << 16) | ((gamma) << 20) |((signedRemap) << 24) | ((zfunc) << 28)); \
	CELL_GCM_DEBUG_CHECK(cmd); \
	(cmd) += 2; \
	} while(false)

#undef CELL_GCM_METHOD_SET_TEXTURE_CONTROL0
#define CELL_GCM_METHOD_SET_TEXTURE_CONTROL0(cmd, index, enable, minlod, maxlod, maxaniso) do { \
	(cmd)[0] = CELL_GCM_METHOD(CELL_GCM_NV4097_SET_TEXTURE_CONTROL0 + 0x20 * (index), 1); \
	(cmd)[1] = CELL_GCM_ENDIAN_SWAP(((maxaniso) << 4) | ((maxlod) << 7) | ((minlod) << 19) | ((enable) << 31)); \
	CELL_GCM_DEBUG_CHECK(cmd); \
	(cmd) += 2; \
	} while(false)

#undef CELL_GCM_METHOD_SET_TEXTURE_CONTROL2
#define CELL_GCM_METHOD_SET_TEXTURE_CONTROL2(cmd, index, value) do { \
	(cmd)[0] = CELL_GCM_METHOD(CELL_GCM_NV4097_SET_TEXTURE_CONTROL2 + (index) * 4, 1); \
	(cmd)[1] = CELL_GCM_ENDIAN_SWAP(value); \
	CELL_GCM_DEBUG_CHECK(cmd); \
	(cmd) += 2; \
	} while(false)

#undef CELL_GCM_METHOD_SET_TEXTURE_CONTROL3
#define CELL_GCM_METHOD_SET_TEXTURE_CONTROL3(cmd, index, control3) do { \
	(cmd)[0] = CELL_GCM_METHOD(CELL_GCM_NV4097_SET_TEXTURE_CONTROL3 + (index) * 4, 1); \
	(cmd)[1] = CELL_GCM_ENDIAN_SWAP(control3); \
	CELL_GCM_DEBUG_CHECK(cmd); \
	(cmd) += 2; \
	} while(false)

#undef CELL_GCM_METHOD_SET_CYLINDRICAL_WRAP
#define CELL_GCM_METHOD_SET_CYLINDRICAL_WRAP(cmd, enable) do { \
	(cmd)[0] = CELL_GCM_METHOD(CELL_GCM_NV4097_SET_CYLINDRICAL_WRAP, 1); \
	(cmd)[1] = CELL_GCM_ENDIAN_SWAP(enable); \
	CELL_GCM_DEBUG_CHECK(cmd); \
	(cmd) += 2; \
	} while(false)

#undef CELL_GCM_METHOD_SET_CYLINDRICAL_WRAP1
#define CELL_GCM_METHOD_SET_CYLINDRICAL_WRAP1(cmd, enable) do { \
	(cmd)[0] = CELL_GCM_METHOD(CELL_GCM_NV4097_SET_CYLINDRICAL_WRAP1, 1); \
	(cmd)[1] = CELL_GCM_ENDIAN_SWAP(enable); \
	CELL_GCM_DEBUG_CHECK(cmd); \
	(cmd) += 2; \
	} while(false)


#undef CELL_GCM_METHOD_SET_TEXTURE_OFFSET_FORMAT
#define CELL_GCM_METHOD_SET_TEXTURE_OFFSET_FORMAT(cmd, index, offset, format) do { \
	(cmd)[0] = CELL_GCM_METHOD(CELL_GCM_NV4097_SET_TEXTURE_OFFSET + (index) * 32, 2); \
	(cmd)[1] = CELL_GCM_ENDIAN_SWAP(offset); \
	(cmd)[2] = CELL_GCM_ENDIAN_SWAP(format); \
	CELL_GCM_DEBUG_CHECK(cmd); \
	(cmd) += 3; \
	} while(false)

#undef CELL_GCM_METHOD_SET_TEXTURE_CONTROL1
#define CELL_GCM_METHOD_SET_TEXTURE_CONTROL1(cmd, index, control1) do { \
	(cmd)[0] = CELL_GCM_METHOD(CELL_GCM_NV4097_SET_TEXTURE_CONTROL1 + (index) * 32, 1); \
	(cmd)[1] = CELL_GCM_ENDIAN_SWAP(control1); \
	CELL_GCM_DEBUG_CHECK(cmd); \
	(cmd) += 2; \
	} while(false)

#undef CELL_GCM_METHOD_SET_TEXTURE_IMAGE_RECT
#define CELL_GCM_METHOD_SET_TEXTURE_IMAGE_RECT(cmd, index, imagerect) do { \
	(cmd)[0] = CELL_GCM_METHOD(CELL_GCM_NV4097_SET_TEXTURE_IMAGE_RECT + (index) * 32, 1); \
	(cmd)[1] = CELL_GCM_ENDIAN_SWAP(imagerect); \
	CELL_GCM_DEBUG_CHECK(cmd); \
	(cmd) += 2; \
	} while(false)

#undef CELL_GCM_METHOD_SET_BEGIN_END
#define CELL_GCM_METHOD_SET_BEGIN_END(cmd, mode) do { \
	(cmd)[0] = CELL_GCM_METHOD(CELL_GCM_NV4097_SET_BEGIN_END, 1); \
	(cmd)[1] = CELL_GCM_ENDIAN_SWAP(mode); \
	CELL_GCM_DEBUG_CHECK(cmd); \
	(cmd) += 2; \
	} while(false)

#undef CELL_GCM_METHOD_DRAW_ARRAYS
#define CELL_GCM_METHOD_DRAW_ARRAYS(cmd, first, count) do { \
	(cmd)[0] = CELL_GCM_METHOD(CELL_GCM_NV4097_DRAW_ARRAYS, 1); \
	(cmd)[1] = CELL_GCM_ENDIAN_SWAP((first) | ((count)<<24)); \
	CELL_GCM_DEBUG_CHECK(cmd); \
	(cmd) += 2; \
	} while(false)

#undef CELL_GCM_METHOD_SET_INDEX_ARRAY_OFFSET_FORMAT
#define CELL_GCM_METHOD_SET_INDEX_ARRAY_OFFSET_FORMAT(cmd, location, offset, type) do { \
	(cmd)[0] = CELL_GCM_METHOD(CELL_GCM_NV4097_SET_INDEX_ARRAY_ADDRESS, 2); \
	(cmd)[1] = offset; \
	(cmd)[2] = CELL_GCM_ENDIAN_SWAP((location) | ((type) << 4)); \
	CELL_GCM_DEBUG_CHECK(cmd); \
	(cmd) += 3; \
	} while(false)

#undef CELL_GCM_METHOD_DRAW_INDEX_ARRAY
#define CELL_GCM_METHOD_DRAW_INDEX_ARRAY(cmd, first, count) do { \
	(cmd)[0] = CELL_GCM_METHOD(CELL_GCM_NV4097_DRAW_INDEX_ARRAY, 1); \
	(cmd)[1] = CELL_GCM_ENDIAN_SWAP((first) | ((count) << 24)); \
	CELL_GCM_DEBUG_CHECK(cmd); \
	(cmd) += 2; \
	} while(false)

#undef CELL_GCM_METHOD_INVALIDATE_VERTEX_CACHE_FILE
#define CELL_GCM_METHOD_INVALIDATE_VERTEX_CACHE_FILE(cmd) do { \
	(cmd)[0] = CELL_GCM_METHOD(CELL_GCM_NV4097_INVALIDATE_VERTEX_CACHE_FILE, 1); \
	(cmd)[1] = 0; \
	CELL_GCM_DEBUG_CHECK(cmd); \
	(cmd) += 2; \
	} while(false)

#undef CELL_GCM_METHOD_INVALIDATE_VERTEX_FILE
#define CELL_GCM_METHOD_INVALIDATE_VERTEX_FILE(cmd) do { \
	(cmd)[0] = CELL_GCM_METHOD(CELL_GCM_NV4097_INVALIDATE_VERTEX_FILE, 1); \
	(cmd)[1] = 0; \
	CELL_GCM_DEBUG_CHECK(cmd); \
	(cmd) += 2; \
	} while(false)

#undef CELL_GCM_METHOD_SET_RESTART_INDEX
#define CELL_GCM_METHOD_SET_RESTART_INDEX(cmd, index) do { \
	(cmd)[0] = CELL_GCM_METHOD(CELL_GCM_NV4097_SET_RESTART_INDEX, 1); \
	(cmd)[1] = CELL_GCM_ENDIAN_SWAP(index); \
	CELL_GCM_DEBUG_CHECK(cmd); \
	(cmd) += 2; \
	} while(false)

#undef CELL_GCM_METHOD_SET_VERTEX_DATA4F_M
#define CELL_GCM_METHOD_SET_VERTEX_DATA4F_M(cmd, index, v0, v1, v2, v3) do { \
	(cmd)[0] = CELL_GCM_METHOD(CELL_GCM_NV4097_SET_VERTEX_DATA4F_M + (index) * 16, 4); \
	(cmd)[1] = CELL_GCM_ENDIAN_SWAP(v0); \
	(cmd)[2] = CELL_GCM_ENDIAN_SWAP(v1); \
	(cmd)[3] = CELL_GCM_ENDIAN_SWAP(v2); \
	(cmd)[4] = CELL_GCM_ENDIAN_SWAP(v3); \
	CELL_GCM_DEBUG_CHECK(cmd); \
	(cmd) += 5; \
	} while(false)

#undef CELL_GCM_METHOD_SET_VERTEX_DATA_ARRAY_OFFSET
#define CELL_GCM_METHOD_SET_VERTEX_DATA_ARRAY_OFFSET(cmd, index, location, offset) do { \
	(cmd)[0] = CELL_GCM_METHOD(CELL_GCM_NV4097_SET_VERTEX_DATA_ARRAY_OFFSET + (index) * 4, 1); \
	(cmd)[1] = CELL_GCM_ENDIAN_SWAP(((location) << 31) | (offset)); \
	CELL_GCM_DEBUG_CHECK(cmd); \
	(cmd) += 2; \
	} while(false)

#undef CELL_GCM_METHOD_SET_VERTEX_DATA_ARRAY_FORMAT
#define CELL_GCM_METHOD_SET_VERTEX_DATA_ARRAY_FORMAT(cmd, index, frequency, stride, size, type) do { \
	(cmd)[0] = CELL_GCM_METHOD(CELL_GCM_NV4097_SET_VERTEX_DATA_ARRAY_FORMAT + (index) * 4, 1); \
	(cmd)[1] = CELL_GCM_ENDIAN_SWAP(((frequency) << 16) | ((stride) << 8) | ((size) << 4) | (type)); \
	CELL_GCM_DEBUG_CHECK(cmd); \
	(cmd) += 2; \
	} while(false)

#undef CELL_GCM_METHOD_SET_FREQUENCY_DIVIDER_OPERATION
#define CELL_GCM_METHOD_SET_FREQUENCY_DIVIDER_OPERATION(cmd, operation) do { \
	(cmd)[0] = CELL_GCM_METHOD(CELL_GCM_NV4097_SET_FREQUENCY_DIVIDER_OPERATION, 1); \
	(cmd)[1] = CELL_GCM_ENDIAN_SWAP(operation); \
	CELL_GCM_DEBUG_CHECK(cmd); \
	(cmd) += 2; \
	} while(false)

#undef CELL_GCM_METHOD_SET_TRANSFORM_BRANCH_BITS
#define CELL_GCM_METHOD_SET_TRANSFORM_BRANCH_BITS(cmd, enable) do { \
	(cmd)[0] = CELL_GCM_METHOD(CELL_GCM_NV4097_SET_TRANSFORM_BRANCH_BITS, 1); \
	(cmd)[1] = CELL_GCM_ENDIAN_SWAP(enable); \
	CELL_GCM_DEBUG_CHECK(cmd); \
	(cmd) += 2; \
	} while(false)

#undef CELL_GCM_METHOD_SET_VERTEX_ATTRIB_INPUT_MASK
#define CELL_GCM_METHOD_SET_VERTEX_ATTRIB_INPUT_MASK(cmd, mask) do { \
	(cmd)[0] = CELL_GCM_METHOD(CELL_GCM_NV4097_SET_VERTEX_ATTRIB_INPUT_MASK, 1); \
	(cmd)[1] = CELL_GCM_ENDIAN_SWAP(mask); \
	CELL_GCM_DEBUG_CHECK(cmd); \
	(cmd) += 2; \
	} while(false)

#undef CELL_GCM_METHOD_SET_VERTEX_ATTRIB_OUTPUT_MASK
#define CELL_GCM_METHOD_SET_VERTEX_ATTRIB_OUTPUT_MASK(cmd, mask) do { \
	(cmd)[0] = CELL_GCM_METHOD(CELL_GCM_NV4097_SET_VERTEX_ATTRIB_OUTPUT_MASK, 1); \
	(cmd)[1] = CELL_GCM_ENDIAN_SWAP(mask); \
	CELL_GCM_DEBUG_CHECK(cmd); \
	(cmd) += 2; \
	} while(false)

#undef CELL_GCM_METHOD_SET_TRANSFORM_PROGRAM_LOAD_START
#define CELL_GCM_METHOD_SET_TRANSFORM_PROGRAM_LOAD_START(cmd, load, start) do { \
	(cmd)[0] = CELL_GCM_METHOD(CELL_GCM_NV4097_SET_TRANSFORM_PROGRAM_LOAD, 2); \
	(cmd)[1] = CELL_GCM_ENDIAN_SWAP(load); \
	(cmd)[2] = CELL_GCM_ENDIAN_SWAP(start); \
	CELL_GCM_DEBUG_CHECK(cmd); \
	(cmd) += 3; \
	} while(false)

#undef CELL_GCM_METHOD_SET_TRANSFORM_PROGRAM_LOAD
#define CELL_GCM_METHOD_SET_TRANSFORM_PROGRAM_LOAD(cmd, load) do { \
	(cmd)[0] = CELL_GCM_METHOD(CELL_GCM_NV4097_SET_TRANSFORM_PROGRAM_LOAD, 1); \
	(cmd)[1] = CELL_GCM_ENDIAN_SWAP(load); \
	CELL_GCM_DEBUG_CHECK(cmd); \
	(cmd) += 2; \
	} while(false)

#undef CELL_GCM_METHOD_SET_TRANSFORM_PROGRAM_START
#define CELL_GCM_METHOD_SET_TRANSFORM_PROGRAM_START(cmd, start) do { \
	(cmd)[0] = CELL_GCM_METHOD(CELL_GCM_NV4097_SET_TRANSFORM_PROGRAM_START, 1); \
	(cmd)[1] = CELL_GCM_ENDIAN_SWAP(start); \
	CELL_GCM_DEBUG_CHECK(cmd); \
	(cmd) += 2; \
	} while(false)

#undef CELL_GCM_METHOD_SET_TRANSFORM_TIMEOUT
#define CELL_GCM_METHOD_SET_TRANSFORM_TIMEOUT(cmd, count, registerCount) do { \
	(cmd)[0] = CELL_GCM_METHOD(CELL_GCM_NV4097_SET_TRANSFORM_TIMEOUT, 1); \
	(cmd)[1] = CELL_GCM_ENDIAN_SWAP((count) | ((registerCount) << 16)); \
	CELL_GCM_DEBUG_CHECK(cmd); \
	(cmd) += 2; \
	} while(false)

#undef CELL_GCM_METHOD_SET_TRANSFORM_PROGRAM
#define CELL_GCM_METHOD_SET_TRANSFORM_PROGRAM(cmd, index, d0, d1, d2, d3) do { \
	(cmd)[0] = CELL_GCM_METHOD(CELL_GCM_NV4097_SET_TRANSFORM_PROGRAM + (index) * 16, 4); \
	(cmd)[1] = CELL_GCM_ENDIAN_SWAP(d0); \
	(cmd)[2] = CELL_GCM_ENDIAN_SWAP(d1); \
	(cmd)[3] = CELL_GCM_ENDIAN_SWAP(d2); \
	(cmd)[4] = CELL_GCM_ENDIAN_SWAP(d3); \
	CELL_GCM_DEBUG_CHECK(cmd); \
	(cmd) += 5; \
	} while(false)

#undef CELL_GCM_METHOD_SET_SHADER_PROGRAM
#define CELL_GCM_METHOD_SET_SHADER_PROGRAM(cmd, location, offset) do { \
	(cmd)[0] = CELL_GCM_METHOD(CELL_GCM_NV4097_SET_SHADER_PROGRAM, 1); \
	(cmd)[1] = CELL_GCM_ENDIAN_SWAP((location) | (offset)); \
	CELL_GCM_DEBUG_CHECK(cmd); \
	(cmd) += 2; \
	} while(false)

#undef CELL_GCM_METHOD_SET_TEX_COORD_CONTROL
#define CELL_GCM_METHOD_SET_TEX_COORD_CONTROL(cmd, index, hwTexCtrl) do { \
	(cmd)[0] = CELL_GCM_METHOD(CELL_GCM_NV4097_SET_TEX_COORD_CONTROL + (index) * 4, 1); \
	(cmd)[1] = CELL_GCM_ENDIAN_SWAP(hwTexCtrl); \
	CELL_GCM_DEBUG_CHECK(cmd); \
	(cmd) += 2; \
	} while(false)

#undef CELL_GCM_METHOD_SET_SHADER_CONTROL
#define CELL_GCM_METHOD_SET_SHADER_CONTROL(cmd, value) do { \
	(cmd)[0] = CELL_GCM_METHOD(CELL_GCM_NV4097_SET_SHADER_CONTROL, 1); \
	(cmd)[1] = CELL_GCM_ENDIAN_SWAP(value); \
	CELL_GCM_DEBUG_CHECK(cmd); \
	(cmd) += 2; \
	} while(false)

#undef CELL_GCM_METHOD_SET_ZMIN_MAX_CONTROL
#define CELL_GCM_METHOD_SET_ZMIN_MAX_CONTROL(cmd, cullNearFarEnable, zclampEnable, cullIgnoreW) do { \
	(cmd)[0] = CELL_GCM_METHOD(CELL_GCM_NV4097_SET_ZMIN_MAX_CONTROL, 1); \
	(cmd)[1] = CELL_GCM_ENDIAN_SWAP((cullNearFarEnable) | ((zclampEnable) << 4) | ((cullIgnoreW)<<8)); \
	CELL_GCM_DEBUG_CHECK(cmd); \
	(cmd) += 2; \
	} while(false)

#undef CELL_GCM_METHOD_SET_TRANSFORM_CONSTANT_LOAD_4U
#define CELL_GCM_METHOD_SET_TRANSFORM_CONSTANT_LOAD_4U(cmd, loadAt, d0, d1, d2, d3) do { \
	(cmd)[0] = CELL_GCM_METHOD(CELL_GCM_NV4097_SET_TRANSFORM_CONSTANT_LOAD, 5); \
	(cmd)[1] = CELL_GCM_ENDIAN_SWAP(loadAt); \
	(cmd)[2] = CELL_GCM_ENDIAN_SWAP(d0); \
	(cmd)[3] = CELL_GCM_ENDIAN_SWAP(d1); \
	(cmd)[4] = CELL_GCM_ENDIAN_SWAP(d2); \
	(cmd)[5] = CELL_GCM_ENDIAN_SWAP(d3); \
	CELL_GCM_DEBUG_CHECK(cmd); \
	(cmd) += 6; \
	} while(false)

#undef CELL_GCM_METHOD_SET_TRANSFORM_CONSTANT_LOAD_12U_XYZ0
#define CELL_GCM_METHOD_SET_TRANSFORM_CONSTANT_LOAD_12U_XYZ0(cmd, loadAt, d0,d1,d2,d3,d4,d5,d6,d7,d8) do { \
	(cmd)[0] = CELL_GCM_METHOD(CELL_GCM_NV4097_SET_TRANSFORM_CONSTANT_LOAD, 13); \
	(cmd)[1] = CELL_GCM_ENDIAN_SWAP(loadAt); \
	(cmd)[2] = CELL_GCM_ENDIAN_SWAP(d0); \
	(cmd)[3] = CELL_GCM_ENDIAN_SWAP(d1); \
	(cmd)[4] = CELL_GCM_ENDIAN_SWAP(d2); \
	(cmd)[5] = 0; \
	(cmd)[6] = CELL_GCM_ENDIAN_SWAP(d3); \
	(cmd)[7] = CELL_GCM_ENDIAN_SWAP(d4); \
	(cmd)[8] = CELL_GCM_ENDIAN_SWAP(d5); \
	(cmd)[9] = 0; \
	(cmd)[10] = CELL_GCM_ENDIAN_SWAP(d6); \
	(cmd)[11] = CELL_GCM_ENDIAN_SWAP(d7); \
	(cmd)[12] = CELL_GCM_ENDIAN_SWAP(d8); \
	(cmd)[13] = 0; \
	CELL_GCM_DEBUG_CHECK(cmd); \
	(cmd) += 14; \
	} while(false)

#undef CELL_GCM_METHOD_SET_TRANSFORM_CONSTANT_LOAD_12U_XYZW
#define CELL_GCM_METHOD_SET_TRANSFORM_CONSTANT_LOAD_12U_XYZW(cmd, loadAt, d0,d1,d2,d3,d4,d5,d6,d7,d8,d9,d10,d11) do { \
	(cmd)[0] = CELL_GCM_METHOD(CELL_GCM_NV4097_SET_TRANSFORM_CONSTANT_LOAD, 13); \
	(cmd)[1] = CELL_GCM_ENDIAN_SWAP(loadAt); \
	(cmd)[2] = CELL_GCM_ENDIAN_SWAP(d0); \
	(cmd)[3] = CELL_GCM_ENDIAN_SWAP(d1); \
	(cmd)[4] = CELL_GCM_ENDIAN_SWAP(d2); \
	(cmd)[5] = CELL_GCM_ENDIAN_SWAP(d3); \
	(cmd)[6] = CELL_GCM_ENDIAN_SWAP(d4); \
	(cmd)[7] = CELL_GCM_ENDIAN_SWAP(d5); \
	(cmd)[8] = CELL_GCM_ENDIAN_SWAP(d6); \
	(cmd)[9] = CELL_GCM_ENDIAN_SWAP(d7); \
	(cmd)[10] = CELL_GCM_ENDIAN_SWAP(d8); \
	(cmd)[11] = CELL_GCM_ENDIAN_SWAP(d9); \
	(cmd)[12] = CELL_GCM_ENDIAN_SWAP(d10); \
	(cmd)[13] = CELL_GCM_ENDIAN_SWAP(d11); \
	CELL_GCM_DEBUG_CHECK(cmd); \
	(cmd) += 14; \
	} while(false)

#undef CELL_GCM_METHOD_SET_TRANSFORM_CONSTANT_LOAD_16U_XYZ0
#define CELL_GCM_METHOD_SET_TRANSFORM_CONSTANT_LOAD_16U_XYZ0(cmd, loadAt, d0,d1,d2,d3,d4,d5,d6,d7,d8,d9,d10,d11) do { \
	(cmd)[0] = CELL_GCM_METHOD(CELL_GCM_NV4097_SET_TRANSFORM_CONSTANT_LOAD, 17); \
	(cmd)[1] = CELL_GCM_ENDIAN_SWAP(loadAt); \
	(cmd)[2] = CELL_GCM_ENDIAN_SWAP(d0); \
	(cmd)[3] = CELL_GCM_ENDIAN_SWAP(d1); \
	(cmd)[4] = CELL_GCM_ENDIAN_SWAP(d2); \
	(cmd)[5] = 0; \
	(cmd)[6] = CELL_GCM_ENDIAN_SWAP(d3); \
	(cmd)[7] = CELL_GCM_ENDIAN_SWAP(d4); \
	(cmd)[8] = CELL_GCM_ENDIAN_SWAP(d5); \
	(cmd)[9] = 0; \
	(cmd)[10] = CELL_GCM_ENDIAN_SWAP(d6); \
	(cmd)[11] = CELL_GCM_ENDIAN_SWAP(d7); \
	(cmd)[12] = CELL_GCM_ENDIAN_SWAP(d8); \
	(cmd)[13] = 0; \
	(cmd)[14] = CELL_GCM_ENDIAN_SWAP(d9); \
	(cmd)[15] = CELL_GCM_ENDIAN_SWAP(d10); \
	(cmd)[16] = CELL_GCM_ENDIAN_SWAP(d11); \
	(cmd)[17] = 0; \
	CELL_GCM_DEBUG_CHECK(cmd); \
	(cmd) += 18; \
	} while(false)

#undef CELL_GCM_METHOD_SET_TRANSFORM_CONSTANT_LOAD_16U_XYZW
#define CELL_GCM_METHOD_SET_TRANSFORM_CONSTANT_LOAD_16U_XYZW(cmd, loadAt, d0,d1,d2,d3,d4,d5,d6,d7,d8,d9,d10,d11,d12,d13,d14,d15) do { \
	(cmd)[0] = CELL_GCM_METHOD(CELL_GCM_NV4097_SET_TRANSFORM_CONSTANT_LOAD, 17); \
	(cmd)[1] = CELL_GCM_ENDIAN_SWAP(loadAt); \
	(cmd)[2] = CELL_GCM_ENDIAN_SWAP(d0); \
	(cmd)[3] = CELL_GCM_ENDIAN_SWAP(d1); \
	(cmd)[4] = CELL_GCM_ENDIAN_SWAP(d2); \
	(cmd)[5] = CELL_GCM_ENDIAN_SWAP(d3); \
	(cmd)[6] = CELL_GCM_ENDIAN_SWAP(d4); \
	(cmd)[7] = CELL_GCM_ENDIAN_SWAP(d5); \
	(cmd)[8] = CELL_GCM_ENDIAN_SWAP(d6); \
	(cmd)[9] = CELL_GCM_ENDIAN_SWAP(d7); \
	(cmd)[10] = CELL_GCM_ENDIAN_SWAP(d8); \
	(cmd)[11] = CELL_GCM_ENDIAN_SWAP(d9); \
	(cmd)[12] = CELL_GCM_ENDIAN_SWAP(d10); \
	(cmd)[13] = CELL_GCM_ENDIAN_SWAP(d11); \
	(cmd)[14] = CELL_GCM_ENDIAN_SWAP(d12); \
	(cmd)[15] = CELL_GCM_ENDIAN_SWAP(d13); \
	(cmd)[16] = CELL_GCM_ENDIAN_SWAP(d14); \
	(cmd)[17] = CELL_GCM_ENDIAN_SWAP(d15); \
	CELL_GCM_DEBUG_CHECK(cmd); \
	(cmd) += 18; \
	} while(false)

#undef CELL_GCM_METHOD_SET_TRANSFORM_CONSTANT_LOAD_32U
#define CELL_GCM_METHOD_SET_TRANSFORM_CONSTANT_LOAD_32U(cmd, loadAt, d0,d1,d2,d3,d4,d5,d6,d7,d8,d9,d10,d11,d12,d13,d14,d15,d16,d17,d18,d19,d20,d21,d22,d23,d24,d25,d26,d27,d28,d29,d30,d31) do { \
	(cmd)[0] = CELL_GCM_METHOD(CELL_GCM_NV4097_SET_TRANSFORM_CONSTANT_LOAD, 33); \
	(cmd)[1] = CELL_GCM_ENDIAN_SWAP(loadAt); \
	(cmd)[2] = CELL_GCM_ENDIAN_SWAP(d0); \
	(cmd)[3] = CELL_GCM_ENDIAN_SWAP(d1); \
	(cmd)[4] = CELL_GCM_ENDIAN_SWAP(d2); \
	(cmd)[5] = CELL_GCM_ENDIAN_SWAP(d3); \
	(cmd)[6] = CELL_GCM_ENDIAN_SWAP(d4); \
	(cmd)[7] = CELL_GCM_ENDIAN_SWAP(d5); \
	(cmd)[8] = CELL_GCM_ENDIAN_SWAP(d6); \
	(cmd)[9] = CELL_GCM_ENDIAN_SWAP(d7); \
	(cmd)[10] = CELL_GCM_ENDIAN_SWAP(d8); \
	(cmd)[11] = CELL_GCM_ENDIAN_SWAP(d9); \
	(cmd)[12] = CELL_GCM_ENDIAN_SWAP(d10); \
	(cmd)[13] = CELL_GCM_ENDIAN_SWAP(d11); \
	(cmd)[14] = CELL_GCM_ENDIAN_SWAP(d12); \
	(cmd)[15] = CELL_GCM_ENDIAN_SWAP(d13); \
	(cmd)[16] = CELL_GCM_ENDIAN_SWAP(d14); \
	(cmd)[17] = CELL_GCM_ENDIAN_SWAP(d15); \
	(cmd)[18] = CELL_GCM_ENDIAN_SWAP(d16); \
	(cmd)[19] = CELL_GCM_ENDIAN_SWAP(d17); \
	(cmd)[20] = CELL_GCM_ENDIAN_SWAP(d18); \
	(cmd)[21] = CELL_GCM_ENDIAN_SWAP(d19); \
	(cmd)[22] = CELL_GCM_ENDIAN_SWAP(d20); \
	(cmd)[23] = CELL_GCM_ENDIAN_SWAP(d21); \
	(cmd)[24] = CELL_GCM_ENDIAN_SWAP(d22); \
	(cmd)[25] = CELL_GCM_ENDIAN_SWAP(d23); \
	(cmd)[26] = CELL_GCM_ENDIAN_SWAP(d24); \
	(cmd)[27] = CELL_GCM_ENDIAN_SWAP(d25); \
	(cmd)[28] = CELL_GCM_ENDIAN_SWAP(d26); \
	(cmd)[29] = CELL_GCM_ENDIAN_SWAP(d27); \
	(cmd)[30] = CELL_GCM_ENDIAN_SWAP(d28); \
	(cmd)[31] = CELL_GCM_ENDIAN_SWAP(d29); \
	(cmd)[32] = CELL_GCM_ENDIAN_SWAP(d30); \
	(cmd)[33] = CELL_GCM_ENDIAN_SWAP(d31); \
	CELL_GCM_DEBUG_CHECK(cmd); \
	(cmd) += 34; \
	} while(false)

#undef CELL_GCM_METHOD_SET_SHADER_PACKER
#define CELL_GCM_METHOD_SET_SHADER_PACKER(cmd, enable) do { \
	(cmd)[0] = CELL_GCM_METHOD(CELL_GCM_NV4097_SET_SHADER_PACKER, 1); \
	(cmd)[1] = CELL_GCM_ENDIAN_SWAP(enable); \
	CELL_GCM_DEBUG_CHECK(cmd); \
	(cmd) += 2; \
	} while(false)

#undef CELL_GCM_METHOD_SET_USER_CLIP_PLANE_CONTROL
#define CELL_GCM_METHOD_SET_USER_CLIP_PLANE_CONTROL(cmd, plane0, plane1, plane2, plane3, plane4, plane5) do { \
	(cmd)[0] = CELL_GCM_METHOD(CELL_GCM_NV4097_SET_USER_CLIP_PLANE_CONTROL, 1); \
	(cmd)[1] = CELL_GCM_ENDIAN_SWAP((plane0) | ((plane1) << 4) | ((plane2) << 8) | ((plane3) << 12) | ((plane4) << 16) | ((plane5) << 20)); \
	CELL_GCM_DEBUG_CHECK(cmd); \
	(cmd) += 2; \
	} while(false)

#undef CELL_GCM_METHOD_SET_RENDER_ENABLE
#define CELL_GCM_METHOD_SET_RENDER_ENABLE(cmd, mode, offset) do { \
	(cmd)[0] = CELL_GCM_METHOD(CELL_GCM_NV4097_SET_RENDER_ENABLE, 1); \
	(cmd)[1] = CELL_GCM_ENDIAN_SWAP((offset) | ((mode) << 24)); \
	CELL_GCM_DEBUG_CHECK(cmd); \
	(cmd) += 2; \
	} while(false)

#undef CELL_GCM_METHOD_SET_ZPASS_PIXEL_COUNT_ENABLE
#define CELL_GCM_METHOD_SET_ZPASS_PIXEL_COUNT_ENABLE(cmd, enable) do { \
	(cmd)[0] = CELL_GCM_METHOD(CELL_GCM_NV4097_SET_ZPASS_PIXEL_COUNT_ENABLE, 1); \
	(cmd)[1] = CELL_GCM_ENDIAN_SWAP(enable); \
	CELL_GCM_DEBUG_CHECK(cmd); \
	(cmd) += 2; \
	} while(false)

#undef CELL_GCM_METHOD_CLEAR_REPORT_VALUE
#define CELL_GCM_METHOD_CLEAR_REPORT_VALUE(cmd, type) do { \
	(cmd)[0] = CELL_GCM_METHOD(CELL_GCM_NV4097_CLEAR_REPORT_VALUE, 1); \
	(cmd)[1] = CELL_GCM_ENDIAN_SWAP(type); \
	CELL_GCM_DEBUG_CHECK(cmd); \
	(cmd) += 2; \
	} while(false)

#undef CELL_GCM_METHOD_GET_REPORT
#define CELL_GCM_METHOD_GET_REPORT(cmd, type, offset) do { \
	(cmd)[0] = CELL_GCM_METHOD(CELL_GCM_NV4097_GET_REPORT, 1); \
	(cmd)[1] = CELL_GCM_ENDIAN_SWAP((offset) | ((type) << 24)); \
	CELL_GCM_DEBUG_CHECK(cmd); \
	(cmd) += 2; \
	} while(false)

#undef CELL_GCM_METHOD_SET_ZCULL_STATS_ENABLE
#define CELL_GCM_METHOD_SET_ZCULL_STATS_ENABLE(cmd, enable) do { \
	(cmd)[0] = CELL_GCM_METHOD(CELL_GCM_NV4097_SET_ZCULL_STATS_ENABLE, 1); \
	(cmd)[1] = CELL_GCM_ENDIAN_SWAP(enable); \
	CELL_GCM_DEBUG_CHECK(cmd); \
	(cmd) += 2; \
	} while(false)

#undef CELL_GCM_METHOD_SET_ZCULL_CONTROL0
#define CELL_GCM_METHOD_SET_ZCULL_CONTROL0(cmd, zCullDir, zCullFormat) do { \
	(cmd)[0] = CELL_GCM_METHOD(CELL_GCM_NV4097_SET_ZCULL_CONTROL0, 1); \
	(cmd)[1] = CELL_GCM_ENDIAN_SWAP((zCullDir) | (zCullFormat<<4)); \
	CELL_GCM_DEBUG_CHECK(cmd); \
	(cmd) += 2; \
	} while(false)


#undef CELL_GCM_METHOD_SET_ZCULL_CONTROL1
#define CELL_GCM_METHOD_SET_ZCULL_CONTROL1(cmd, moveForwardLimit, pushBackLimit) do { \
	(cmd)[0] = CELL_GCM_METHOD(CELL_GCM_NV4097_SET_ZCULL_CONTROL1, 1); \
	(cmd)[1] = CELL_GCM_ENDIAN_SWAP((moveForwardLimit<<16) | (pushBackLimit)); \
	CELL_GCM_DEBUG_CHECK(cmd); \
	(cmd) += 2; \
	} while(false)

#undef CELL_GCM_METHOD_SET_SCULL_CONTROL
#define CELL_GCM_METHOD_SET_SCULL_CONTROL(cmd, sFunc, sRef, sMask) do { \
	(cmd)[0] = CELL_GCM_METHOD(CELL_GCM_NV4097_SET_SCULL_CONTROL, 1); \
	(cmd)[1] = CELL_GCM_ENDIAN_SWAP((sFunc) | (sRef<<16) | (sMask<<24)); \
	CELL_GCM_DEBUG_CHECK(cmd); \
	(cmd) += 2; \
	} while(false)


#undef CELL_GCM_METHOD_INVALIDATE_ZCULL
#define CELL_GCM_METHOD_INVALIDATE_ZCULL(cmd) do { \
	(cmd)[0] = CELL_GCM_METHOD(CELL_GCM_NV4097_INVALIDATE_ZCULL, 1); \
	(cmd)[1] = 0; \
	CELL_GCM_DEBUG_CHECK(cmd); \
	(cmd) += 2; \
	} while(false)

#undef CELL_GCM_METHOD_SET_SEMAPHORE_OFFSET
#define CELL_GCM_METHOD_SET_SEMAPHORE_OFFSET(cmd, offset) do { \
	(cmd)[0] = CELL_GCM_METHOD(CELL_GCM_NV4097_SET_SEMAPHORE_OFFSET, 1); \
	(cmd)[1] = CELL_GCM_ENDIAN_SWAP(offset); \
	CELL_GCM_DEBUG_CHECK(cmd); \
	(cmd) += 2; \
	} while(false)

#undef CELL_GCM_METHOD_TEXTURE_READ_SEMAPHORE_RELEASE
#define CELL_GCM_METHOD_TEXTURE_READ_SEMAPHORE_RELEASE(cmd, value) do { \
	(cmd)[0] = CELL_GCM_METHOD(CELL_GCM_NV4097_TEXTURE_READ_SEMAPHORE_RELEASE, 1); \
	(cmd)[1] = CELL_GCM_ENDIAN_SWAP(value); \
	CELL_GCM_DEBUG_CHECK(cmd); \
	(cmd) += 2; \
	} while(false)

#undef CELL_GCM_METHOD_BACK_END_WRITE_SEMAPHORE_RELEASE
#define CELL_GCM_METHOD_BACK_END_WRITE_SEMAPHORE_RELEASE(cmd, value) do { \
	(cmd)[0] = CELL_GCM_METHOD(CELL_GCM_NV4097_BACK_END_WRITE_SEMAPHORE_RELEASE, 1); \
	(cmd)[1] = CELL_GCM_ENDIAN_SWAP(value); \
	CELL_GCM_DEBUG_CHECK(cmd); \
	(cmd) += 2; \
	} while(false)

#undef CELL_GCM_METHOD_CHANNEL_SEMAPHORE_OFFSET
#define CELL_GCM_METHOD_CHANNEL_SEMAPHORE_OFFSET(cmd, offset) do { \
	(cmd)[0] = CELL_GCM_METHOD(CELL_GCM_NV406E_SEMAPHORE_OFFSET, 1); \
	(cmd)[1] = CELL_GCM_ENDIAN_SWAP(offset); \
	CELL_GCM_DEBUG_CHECK(cmd); \
	(cmd) += 2; \
	} while(false)

#undef CELL_GCM_METHOD_CHANNEL_SEMAPHORE_ACQUIRE
#define CELL_GCM_METHOD_CHANNEL_SEMAPHORE_ACQUIRE(cmd, value) do { \
	(cmd)[0] = CELL_GCM_METHOD(CELL_GCM_NV406E_SEMAPHORE_ACQUIRE, 1); \
	(cmd)[1] = CELL_GCM_ENDIAN_SWAP(value); \
	CELL_GCM_DEBUG_CHECK(cmd); \
	(cmd) += 2; \
	} while(false)

#undef CELL_GCM_METHOD_CHANNEL_SEMAPHORE_RELEASE
#define CELL_GCM_METHOD_CHANNEL_SEMAPHORE_RELEASE(cmd, value) do { \
	(cmd)[0] = CELL_GCM_METHOD(CELL_GCM_NV406E_SEMAPHORE_RELEASE, 1); \
	(cmd)[1] = CELL_GCM_ENDIAN_SWAP(value); \
	CELL_GCM_DEBUG_CHECK(cmd); \
	(cmd) += 2; \
	} while(false)

#undef CELL_GCM_METHOD_SET_ANTI_ALIASING_CONTROL
#define CELL_GCM_METHOD_SET_ANTI_ALIASING_CONTROL(cmd, enable, alphaToCoverage, alphaToOne, sampleMask) do { \
	(cmd)[0] = CELL_GCM_METHOD(CELL_GCM_NV4097_SET_ANTI_ALIASING_CONTROL, 1); \
	(cmd)[1] = CELL_GCM_ENDIAN_SWAP((enable) | ((alphaToCoverage) << 4) | ((alphaToOne) << 8) | ((sampleMask) << 16)); \
	CELL_GCM_DEBUG_CHECK(cmd); \
	(cmd) += 2; \
	} while(false)

#undef CELL_GCM_METHOD_SET_CONTEXT_DMA_COLOR_A
#define CELL_GCM_METHOD_SET_CONTEXT_DMA_COLOR_A(cmd, context) do { \
	(cmd)[0] = CELL_GCM_METHOD(CELL_GCM_NV4097_SET_CONTEXT_DMA_COLOR_A, 1); \
	(cmd)[1] = CELL_GCM_ENDIAN_SWAP(context); \
	CELL_GCM_DEBUG_CHECK(cmd); \
	(cmd) += 2; \
	} while(false)

#undef CELL_GCM_METHOD_SET_CONTEXT_DMA_COLOR_B
#define CELL_GCM_METHOD_SET_CONTEXT_DMA_COLOR_B(cmd, context) do { \
	(cmd)[0] = CELL_GCM_METHOD(CELL_GCM_NV4097_SET_CONTEXT_DMA_COLOR_B, 1); \
	(cmd)[1] = CELL_GCM_ENDIAN_SWAP(context); \
	CELL_GCM_DEBUG_CHECK(cmd); \
	(cmd) += 2; \
	} while(false)

#undef CELL_GCM_METHOD_SET_CONTEXT_DMA_COLOR_C
#define CELL_GCM_METHOD_SET_CONTEXT_DMA_COLOR_C(cmd, context) do { \
	(cmd)[0] = CELL_GCM_METHOD(CELL_GCM_NV4097_SET_CONTEXT_DMA_COLOR_C, 1); \
	(cmd)[1] = CELL_GCM_ENDIAN_SWAP(context); \
	CELL_GCM_DEBUG_CHECK(cmd); \
	(cmd) += 2; \
	} while(false)

#undef CELL_GCM_METHOD_SET_CONTEXT_DMA_COLOR_D
#define CELL_GCM_METHOD_SET_CONTEXT_DMA_COLOR_D(cmd, context) do { \
	(cmd)[0] = CELL_GCM_METHOD(CELL_GCM_NV4097_SET_CONTEXT_DMA_COLOR_D, 1); \
	(cmd)[1] = CELL_GCM_ENDIAN_SWAP(context); \
	CELL_GCM_DEBUG_CHECK(cmd); \
	(cmd) += 2; \
	} while(false)

#undef CELL_GCM_METHOD_SET_CONTEXT_DMA_COLOR_C_D
#define CELL_GCM_METHOD_SET_CONTEXT_DMA_COLOR_C_D(cmd, contextC, contextD) do { \
	(cmd)[0] = CELL_GCM_METHOD(CELL_GCM_NV4097_SET_CONTEXT_DMA_COLOR_C, 2); \
	(cmd)[1] = CELL_GCM_ENDIAN_SWAP(contextC); \
	(cmd)[2] = CELL_GCM_ENDIAN_SWAP(contextD); \
	CELL_GCM_DEBUG_CHECK(cmd); \
	(cmd) += 3; \
	} while(false)

#undef CELL_GCM_METHOD_SET_SURFACE_FORMAT_PITCH_A_B_OFFSET_A_B_Z
#define CELL_GCM_METHOD_SET_SURFACE_FORMAT_PITCH_A_B_OFFSET_A_B_Z(cmd, colorFormat, depthFormat, antialias, type, width, height, pitchA, offsetA, offsetZ, offsetB, pitchB) do { \
	(cmd)[0] = CELL_GCM_METHOD(CELL_GCM_NV4097_SET_SURFACE_FORMAT, 6); \
	(cmd)[1] = CELL_GCM_ENDIAN_SWAP((colorFormat) | ((depthFormat) << 5) | ((type) << 8) | ((antialias) << 12) | ((width) << 16) | ((height) << 24)); \
	(cmd)[2] = CELL_GCM_ENDIAN_SWAP(pitchA); \
	(cmd)[3] = CELL_GCM_ENDIAN_SWAP(offsetA); \
	(cmd)[4] = CELL_GCM_ENDIAN_SWAP(offsetZ); \
	(cmd)[5] = CELL_GCM_ENDIAN_SWAP(offsetB); \
	(cmd)[6] = CELL_GCM_ENDIAN_SWAP(pitchB); \
	CELL_GCM_DEBUG_CHECK(cmd); \
	CELL_GCM_DEBUG_CHECK(cmd); \
	(cmd) += 7; \
	} while(false)

#undef CELL_GCM_METHOD_SET_SURFACE_PITCH_Z
#define CELL_GCM_METHOD_SET_SURFACE_PITCH_Z(cmd, pitch) do { \
	(cmd)[0] = CELL_GCM_METHOD(CELL_GCM_NV4097_SET_SURFACE_PITCH_Z, 1); \
	(cmd)[1] = CELL_GCM_ENDIAN_SWAP(pitch); \
	CELL_GCM_DEBUG_CHECK(cmd); \
	(cmd) += 2; \
	} while(false)

#undef CELL_GCM_METHOD_SET_SURFACE_PITCH_C_D_OFFSET_C_D
#define CELL_GCM_METHOD_SET_SURFACE_PITCH_C_D_OFFSET_C_D(cmd, pitchC, pitchD, offsetC, offsetD) do { \
	(cmd)[0] = CELL_GCM_METHOD(CELL_GCM_NV4097_SET_SURFACE_PITCH_C, 4); \
	(cmd)[1] = CELL_GCM_ENDIAN_SWAP(pitchC); \
	(cmd)[2] = CELL_GCM_ENDIAN_SWAP(pitchD); \
	(cmd)[3] = CELL_GCM_ENDIAN_SWAP(offsetC); \
	(cmd)[4] = CELL_GCM_ENDIAN_SWAP(offsetD); \
	CELL_GCM_DEBUG_CHECK(cmd); \
	(cmd) += 5; \
	} while(false)

#undef CELL_GCM_METHOD_SET_SURFACE_COLOR_TARGET
#define CELL_GCM_METHOD_SET_SURFACE_COLOR_TARGET(cmd, mask) do { \
	(cmd)[0] = CELL_GCM_METHOD(CELL_GCM_NV4097_SET_SURFACE_COLOR_TARGET, 1); \
	(cmd)[1] = CELL_GCM_ENDIAN_SWAP(mask); \
	CELL_GCM_DEBUG_CHECK(cmd); \
	(cmd) += 2; \
	} while(false)

#undef CELL_GCM_METHOD_SET_WINDOW_OFFSET
#define CELL_GCM_METHOD_SET_WINDOW_OFFSET(cmd, x, y) do { \
	(cmd)[0] = CELL_GCM_METHOD(CELL_GCM_NV4097_SET_WINDOW_OFFSET, 1); \
	(cmd)[1] = CELL_GCM_ENDIAN_SWAP((x) | ((y) << 16)); \
	CELL_GCM_DEBUG_CHECK(cmd); \
	(cmd) += 2; \
	} while(false)

#undef CELL_GCM_METHOD_SET_SURFACE_CLIP_HORIZONTAL_VERTICAL
#define CELL_GCM_METHOD_SET_SURFACE_CLIP_HORIZONTAL_VERTICAL(cmd, x, w, y, h) do { \
	(cmd)[0] = CELL_GCM_METHOD(CELL_GCM_NV4097_SET_SURFACE_CLIP_HORIZONTAL, 2); \
	(cmd)[1] = CELL_GCM_ENDIAN_SWAP((x) | ((w) << 16)); \
	(cmd)[2] = CELL_GCM_ENDIAN_SWAP((y) | ((h) << 16)); \
	CELL_GCM_DEBUG_CHECK(cmd); \
	(cmd) += 3; \
	} while(false)

#undef CELL_GCM_METHOD_SET_SHADER_WINDOW
#define CELL_GCM_METHOD_SET_SHADER_WINDOW(cmd, height, origin, pixelCenters) do { \
	(cmd)[0] = CELL_GCM_METHOD(CELL_GCM_NV4097_SET_SHADER_WINDOW, 1); \
	(cmd)[1] = CELL_GCM_ENDIAN_SWAP((height) | ((origin) << 12) | ((pixelCenters) << 16)); \
	CELL_GCM_DEBUG_CHECK(cmd); \
	(cmd) += 2; \
	} while(false)

#undef CELL_GCM_METHOD_SET_REFERENCE
#define CELL_GCM_METHOD_SET_REFERENCE(cmd, ref) do { \
	(cmd)[0] = CELL_GCM_METHOD(CELL_GCM_NV406E_SET_REFERENCE, 1); \
	(cmd)[1] = CELL_GCM_ENDIAN_SWAP(ref); \
	CELL_GCM_DEBUG_CHECK(cmd); \
	(cmd) += 2; \
	} while(false)

#undef CELL_GCM_METHOD_SURFACE2D_SET_CONTEXT_DMA_IMAGE_DESTIN
#define CELL_GCM_METHOD_SURFACE2D_SET_CONTEXT_DMA_IMAGE_DESTIN(cmd, context) do { \
	(cmd)[0] = CELL_GCM_METHOD(CELL_GCM_NV3062_SET_CONTEXT_DMA_IMAGE_DESTIN, 1); \
	(cmd)[1] = CELL_GCM_ENDIAN_SWAP(context); \
	CELL_GCM_DEBUG_CHECK(cmd); \
	(cmd) += 2; \
	} while(false)

#undef CELL_GCM_METHOD_SURFACE2D_SET_OFFSET_DESTIN
#define CELL_GCM_METHOD_SURFACE2D_SET_OFFSET_DESTIN(cmd, dst) do { \
	(cmd)[0] = CELL_GCM_METHOD(CELL_GCM_NV3062_SET_OFFSET_DESTIN, 1); \
	(cmd)[1] = CELL_GCM_ENDIAN_SWAP(dst); \
	CELL_GCM_DEBUG_CHECK(cmd); \
	(cmd) += 2; \
	} while(false)

#undef CELL_GCM_METHOD_SURFACE2D_SET_COLOR_FORMAT_PITCH
#define CELL_GCM_METHOD_SURFACE2D_SET_COLOR_FORMAT_PITCH(cmd, format, srcPitch, dstPitch) do { \
	(cmd)[0] = CELL_GCM_METHOD(CELL_GCM_NV3062_SET_COLOR_FORMAT, 2); \
	(cmd)[1] = CELL_GCM_ENDIAN_SWAP(format); \
	(cmd)[2] = CELL_GCM_ENDIAN_SWAP((srcPitch) | ((dstPitch) << 16)); \
	CELL_GCM_DEBUG_CHECK(cmd); \
	(cmd) += 3; \
	} while(false)

#undef CELL_GCM_METHOD_SURFACE2D_SET_COLOR_FORMAT_PITCH_OFFSET
#define CELL_GCM_METHOD_SURFACE2D_SET_COLOR_FORMAT_PITCH_OFFSET(cmd, format, pitch, offset) do { \
	(cmd)[0] = CELL_GCM_METHOD(CELL_GCM_NV3062_SET_COLOR_FORMAT, 4); \
	(cmd)[1] = CELL_GCM_ENDIAN_SWAP(format); \
	(cmd)[2] = CELL_GCM_ENDIAN_SWAP((64) | ((pitch) << 16)); \
	(cmd)[3] = 0; \
	(cmd)[4] = CELL_GCM_ENDIAN_SWAP(offset); \
	CELL_GCM_DEBUG_CHECK(cmd); \
	(cmd) += 5; \
	} while(false)

#undef CELL_GCM_METHOD_INLINE2D_POINT_SIZE_OUT_IN
#define CELL_GCM_METHOD_INLINE2D_POINT_SIZE_OUT_IN(cmd, pointX, pointY, outSizeX, outSizeY, inSizeX, inSizeY) do { \
	(cmd)[0] = CELL_GCM_METHOD(CELL_GCM_NV308A_POINT, 3); \
	(cmd)[1] = CELL_GCM_ENDIAN_SWAP(((pointY) << 16) | (pointX)); \
	(cmd)[2] = CELL_GCM_ENDIAN_SWAP(((outSizeY) << 16) | (outSizeX)); \
	(cmd)[3] = CELL_GCM_ENDIAN_SWAP(((inSizeY) << 16) | (inSizeX)); \
	CELL_GCM_DEBUG_CHECK(cmd); \
	(cmd) += 4; \
	} while(false)

#undef CELL_GCM_METHOD_SCALE2D_SET_CONTEXT_DMA_IMAGE
#define CELL_GCM_METHOD_SCALE2D_SET_CONTEXT_DMA_IMAGE(cmd, context) do { \
	(cmd)[0] = CELL_GCM_METHOD(CELL_GCM_NV3089_SET_CONTEXT_DMA_IMAGE, 1); \
	(cmd)[1] = CELL_GCM_ENDIAN_SWAP(context); \
	CELL_GCM_DEBUG_CHECK(cmd); \
	(cmd) += 2; \
	} while(false)

#undef CELL_GCM_METHOD_SCALE2D_SET_CONTEXT_SURFACE
#define CELL_GCM_METHOD_SCALE2D_SET_CONTEXT_SURFACE(cmd, context) do { \
	(cmd)[0] = CELL_GCM_METHOD(CELL_GCM_NV3089_SET_CONTEXT_SURFACE, 1); \
	(cmd)[1] = CELL_GCM_ENDIAN_SWAP(context); \
	CELL_GCM_DEBUG_CHECK(cmd); \
	(cmd) += 2; \
	} while(false)

#undef CELL_GCM_METHOD_SCALE2D_SET_COLOR_OPERATION_CLIP_IMAGE_OUT_DS_DX_DT_DY
#define CELL_GCM_METHOD_SCALE2D_SET_COLOR_OPERATION_CLIP_IMAGE_OUT_DS_DX_DT_DY(cmd, conv, fmt, op, inX, inY, inW, inH, outX, outY, outW, outH, dsdx, dtdy) do { \
	(cmd)[0] = CELL_GCM_METHOD(CELL_GCM_NV3089_SET_COLOR_CONVERSION, 9); \
	(cmd)[1] = CELL_GCM_ENDIAN_SWAP(conv); \
	(cmd)[2] = CELL_GCM_ENDIAN_SWAP(fmt); \
	(cmd)[3] = CELL_GCM_ENDIAN_SWAP(op); \
	(cmd)[4] = CELL_GCM_ENDIAN_SWAP(((inY) << 16) | (inX)); \
	(cmd)[5] = CELL_GCM_ENDIAN_SWAP(((inH) << 16) | (inW)); \
	(cmd)[6] = CELL_GCM_ENDIAN_SWAP(((outY) << 16) | (outX)); \
	(cmd)[7] = CELL_GCM_ENDIAN_SWAP(((outH) << 16) | (outW)); \
	(cmd)[8] = CELL_GCM_ENDIAN_SWAP(dsdx); \
	(cmd)[9] = CELL_GCM_ENDIAN_SWAP(dtdy); \
	CELL_GCM_DEBUG_CHECK(cmd); \
	(cmd) += 10; \
	} while(false)

#undef CELL_GCM_METHOD_SCALE2D_IMAGE_IN
#define CELL_GCM_METHOD_SCALE2D_IMAGE_IN(cmd, w, h, pitch, origin, interpolator, offset, u, v) do { \
	(cmd)[0] = CELL_GCM_METHOD(CELL_GCM_NV3089_IMAGE_IN_SIZE, 4); \
	(cmd)[1] = CELL_GCM_ENDIAN_SWAP(((h) << 16) | (w)); \
	(cmd)[2] = CELL_GCM_ENDIAN_SWAP((pitch) | ((origin) << 16) | ((interpolator) << 24)); \
	(cmd)[3] = CELL_GCM_ENDIAN_SWAP(offset); \
	(cmd)[4] = CELL_GCM_ENDIAN_SWAP(((v) << 16) | (u)); \
	CELL_GCM_DEBUG_CHECK(cmd); \
	(cmd) += 5; \
	} while(false)

#undef CELL_GCM_METHOD_COPY2D_SET_CONTEXT_DMA_BUFFER
#define CELL_GCM_METHOD_COPY2D_SET_CONTEXT_DMA_BUFFER(cmd, srcContext, dstContext) do { \
	(cmd)[0] = CELL_GCM_METHOD(CELL_GCM_NV0039_SET_CONTEXT_DMA_BUFFER_IN, 2); \
	(cmd)[1] = CELL_GCM_ENDIAN_SWAP(srcContext); \
	(cmd)[2] = CELL_GCM_ENDIAN_SWAP(dstContext); \
	CELL_GCM_DEBUG_CHECK(cmd); \
	(cmd) += 3; \
	} while(false)

#undef CELL_GCM_METHOD_COPY2D_OFFSET_PITCH_LINE_FORMAT_NOTIFY
#define CELL_GCM_METHOD_COPY2D_OFFSET_PITCH_LINE_FORMAT_NOTIFY(cmd, inOffset, outOffset, inPitch, outPitch, lineLength, lineCount, inFormat, outFormat, notify) do { \
	(cmd)[0] = CELL_GCM_METHOD(CELL_GCM_NV0039_OFFSET_IN, 8); \
	(cmd)[1] = CELL_GCM_ENDIAN_SWAP(inOffset); \
	(cmd)[2] = CELL_GCM_ENDIAN_SWAP(outOffset); \
	(cmd)[3] = CELL_GCM_ENDIAN_SWAP(inPitch); \
	(cmd)[4] = CELL_GCM_ENDIAN_SWAP(outPitch); \
	(cmd)[5] = CELL_GCM_ENDIAN_SWAP(lineLength); \
	(cmd)[6] = CELL_GCM_ENDIAN_SWAP(lineCount); \
	(cmd)[7] = CELL_GCM_ENDIAN_SWAP(((outFormat) << 8) | (inFormat)); \
	(cmd)[8] = CELL_GCM_ENDIAN_SWAP(notify); \
	CELL_GCM_DEBUG_CHECK(cmd); \
	(cmd) += 9; \
	} while(false)

#undef CELL_GCM_METHOD_COPY2D_OFFSET_OUT
#define CELL_GCM_METHOD_COPY2D_OFFSET_OUT(cmd, offset) do { \
	(cmd)[0] = CELL_GCM_METHOD(CELL_GCM_NV0039_OFFSET_OUT, 1); \
	(cmd)[1] = CELL_GCM_ENDIAN_SWAP(offset); \
	CELL_GCM_DEBUG_CHECK(cmd); \
	(cmd) += 2; \
	} while(false)

#undef CELL_GCM_METHOD_SWIZZLE2D_SET_CONTEXT_DMA_IMAGE
#define CELL_GCM_METHOD_SWIZZLE2D_SET_CONTEXT_DMA_IMAGE(cmd, context) do { \
	(cmd)[0] = CELL_GCM_METHOD(CELL_GCM_NV309E_SET_CONTEXT_DMA_IMAGE, 1); \
	(cmd)[1] = CELL_GCM_ENDIAN_SWAP(context); \
	CELL_GCM_DEBUG_CHECK(cmd); \
	(cmd) += 2; \
	} while(false)

#undef CELL_GCM_METHOD_SWIZZLE2D_SET_FORMAT_OFFSET
#define CELL_GCM_METHOD_SWIZZLE2D_SET_FORMAT_OFFSET(cmd, format, width, height, offset) do { \
	(cmd)[0] = CELL_GCM_METHOD(CELL_GCM_NV309E_SET_FORMAT, 2); \
	(cmd)[1] = CELL_GCM_ENDIAN_SWAP((format) | ((width) << 16) | ((height) << 24));  \
	(cmd)[2] = CELL_GCM_ENDIAN_SWAP(offset); \
	CELL_GCM_DEBUG_CHECK(cmd); \
	(cmd) += 3; \
	} while(false)

#undef CELL_GCM_METHOD_SET_VERTEX_TEXTURE_OFFSET_FORMAT
#define CELL_GCM_METHOD_SET_VERTEX_TEXTURE_OFFSET_FORMAT(cmd, index, offset, format) do { \
	(cmd)[0] = CELL_GCM_METHOD(CELL_GCM_NV4097_SET_VERTEX_TEXTURE_OFFSET + (index) * 32, 2); \
	(cmd)[1] = CELL_GCM_ENDIAN_SWAP(offset); \
	(cmd)[2] = CELL_GCM_ENDIAN_SWAP(format); \
	CELL_GCM_DEBUG_CHECK(cmd); \
	(cmd) += 3; \
	} while(false)

#undef CELL_GCM_METHOD_SET_VERTEX_TEXTURE_CONTROL3
#define CELL_GCM_METHOD_SET_VERTEX_TEXTURE_CONTROL3(cmd, index, control3) do { \
	(cmd)[0] = CELL_GCM_METHOD(CELL_GCM_NV4097_SET_VERTEX_TEXTURE_CONTROL3 + (index) * 32, 1); \
	(cmd)[1] = CELL_GCM_ENDIAN_SWAP(control3); \
	CELL_GCM_DEBUG_CHECK(cmd); \
	(cmd) += 2; \
	} while(false)

#undef CELL_GCM_METHOD_SET_VERTEX_TEXTURE_IMAGE_RECT
#define CELL_GCM_METHOD_SET_VERTEX_TEXTURE_IMAGE_RECT(cmd, index, imagerect) do { \
	(cmd)[0] = CELL_GCM_METHOD(CELL_GCM_NV4097_SET_VERTEX_TEXTURE_IMAGE_RECT + (index) * 32, 1); \
	(cmd)[1] = CELL_GCM_ENDIAN_SWAP(imagerect); \
	CELL_GCM_DEBUG_CHECK(cmd); \
	(cmd) += 2; \
	} while(false)

#undef CELL_GCM_METHOD_SET_VERTEX_TEXTURE_ADDRESS
#define CELL_GCM_METHOD_SET_VERTEX_TEXTURE_ADDRESS(cmd, index, wrapu, wrapv) do { \
	(cmd)[0] = CELL_GCM_METHOD(CELL_GCM_NV4097_SET_VERTEX_TEXTURE_ADDRESS + (index) * 32, 1); \
	(cmd)[1] = CELL_GCM_ENDIAN_SWAP((wrapu) | ((wrapv) << 8)); \
	CELL_GCM_DEBUG_CHECK(cmd); \
	(cmd) += 2; \
	} while(false)

#undef CELL_GCM_METHOD_SET_VERTEX_TEXTURE_FILTER
#define CELL_GCM_METHOD_SET_VERTEX_TEXTURE_FILTER(cmd, index, bias) do { \
	(cmd)[0] = CELL_GCM_METHOD(CELL_GCM_NV4097_SET_VERTEX_TEXTURE_FILTER + (index) * 32, 1); \
	(cmd)[1] = CELL_GCM_ENDIAN_SWAP(bias); \
	CELL_GCM_DEBUG_CHECK(cmd); \
	(cmd) += 2; \
	} while(false)

#undef CELL_GCM_METHOD_SET_VERTEX_TEXTURE_CONTROL0
#define CELL_GCM_METHOD_SET_VERTEX_TEXTURE_CONTROL0(cmd, index, enable, minLod, maxLod) do { \
	(cmd)[0] = CELL_GCM_METHOD(CELL_GCM_NV4097_SET_VERTEX_TEXTURE_CONTROL0 + (index) * 32, 1); \
	(cmd)[1] = CELL_GCM_ENDIAN_SWAP(((maxLod) << 7) | ((minLod) << 19) | ((enable) << 31)); \
	CELL_GCM_DEBUG_CHECK(cmd); \
	(cmd) += 2; \
	} while(false)

#undef CELL_GCM_METHOD_SET_VERTEX_TEXTURE_BORDER_COLOR
#define CELL_GCM_METHOD_SET_VERTEX_TEXTURE_BORDER_COLOR(cmd, index, color) do { \
	(cmd)[0] = CELL_GCM_METHOD(CELL_GCM_NV4097_SET_VERTEX_TEXTURE_BORDER_COLOR + (index) * 32, 1); \
	(cmd)[1] = CELL_GCM_ENDIAN_SWAP(color); \
	CELL_GCM_DEBUG_CHECK(cmd); \
	(cmd) += 2; \
	} while(false)

#undef CELL_GCM_METHOD_WAIT_FOR_IDLE
#define CELL_GCM_METHOD_WAIT_FOR_IDLE(cmd) do { \
	(cmd)[0] = CELL_GCM_METHOD(CELL_GCM_NV4097_WAIT_FOR_IDLE, 1); \
	(cmd)[1] = 0; \
	CELL_GCM_DEBUG_CHECK(cmd); \
	(cmd) += 2; \
	} while(false)

#undef CELL_GCM_METHOD_PM_TRIGGER
#define CELL_GCM_METHOD_PM_TRIGGER(cmd) do { \
	(cmd)[0] = CELL_GCM_METHOD(CELL_GCM_NV4097_PM_TRIGGER, 1); \
	(cmd)[1] = CELL_GCM_ENDIAN_SWAP(1); \
	CELL_GCM_DEBUG_CHECK(cmd); \
	(cmd) += 2; \
	} while(false)

#undef CELL_GCM_METHOD_INVALIDATE_VERTEX_FILE_3
#define CELL_GCM_METHOD_INVALIDATE_VERTEX_FILE_3(cmd) do { \
	(cmd)[0] = CELL_GCM_METHOD_NI(CELL_GCM_NV4097_INVALIDATE_VERTEX_FILE, 3); \
	(cmd)[1] = 0; \
	(cmd)[2] = 0; \
	(cmd)[3] = 0; \
	CELL_GCM_DEBUG_CHECK(cmd); \
	(cmd) += 4; \
	} while(false)

#undef CELL_GCM_METHOD_SET_CONTEXT_DMA_Z
#define CELL_GCM_METHOD_SET_CONTEXT_DMA_Z(cmd, context) do { \
	(cmd)[0] = CELL_GCM_METHOD(CELL_GCM_NV4097_SET_CONTEXT_DMA_ZETA, 1); \
	(cmd)[1] = CELL_GCM_ENDIAN_SWAP(context); \
	CELL_GCM_DEBUG_CHECK(cmd); \
	(cmd) += 2; \
	} while(false)

#undef CELL_GCM_METHOD_SET_FOG_MODE
#define CELL_GCM_METHOD_SET_FOG_MODE(cmd, mode) do { \
	(cmd)[0] = CELL_GCM_METHOD(CELL_GCM_NV4097_SET_FOG_MODE, 1); \
	(cmd)[1] = CELL_GCM_ENDIAN_SWAP(mode); \
	CELL_GCM_DEBUG_CHECK(cmd); \
	(cmd) += 2; \
	} while(false)

#undef CELL_GCM_METHOD_SET_FOG_PARAMS
#define CELL_GCM_METHOD_SET_FOG_PARAMS(cmd, p0, p1) do { \
	(cmd)[0] = CELL_GCM_METHOD(CELL_GCM_NV4097_SET_FOG_PARAMS, 2); \
	(cmd)[1] = CELL_GCM_ENDIAN_SWAP(p0); \
    (cmd)[2] = CELL_GCM_ENDIAN_SWAP(p1); \
	CELL_GCM_DEBUG_CHECK(cmd); \
	(cmd) += 3; \
	} while(false)

#undef CELL_GCM_METHOD_SET_CONTROL0
#define CELL_GCM_METHOD_SET_CONTROL0(cmd, value) do { \
	(cmd)[0] = CELL_GCM_METHOD(CELL_GCM_NV4097_SET_CONTROL0, 1); \
	(cmd)[1] = CELL_GCM_ENDIAN_SWAP(value); \
	CELL_GCM_DEBUG_CHECK(cmd); \
	(cmd) += 2; \
	} while(false)

#undef CELL_GCM_METHOD_SET_REDUCE_DST_COLOR
#define CELL_GCM_METHOD_SET_REDUCE_DST_COLOR(cmd, value) do { \
	(cmd)[0] = CELL_GCM_METHOD(CELL_GCM_NV4097_SET_REDUCE_DST_COLOR, 1); \
	(cmd)[1] = CELL_GCM_ENDIAN_SWAP(value); \
	CELL_GCM_DEBUG_CHECK(cmd); \
	(cmd) += 2; \
	} while(false)

#undef CELL_GCM_METHOD_COPY2D_PITCH_LINE_FORMAT
#define CELL_GCM_METHOD_COPY2D_PITCH_LINE_FORMAT(cmd, inPitch, outPitch, lineLength, lineCount, inFormat, outFormat) do { \
	(cmd)[0] = CELL_GCM_METHOD(CELL_GCM_NV0039_PITCH_IN, 5); \
	(cmd)[1] = CELL_GCM_ENDIAN_SWAP(inPitch); \
	(cmd)[2] = CELL_GCM_ENDIAN_SWAP(outPitch); \
	(cmd)[3] = CELL_GCM_ENDIAN_SWAP(lineLength); \
	(cmd)[4] = CELL_GCM_ENDIAN_SWAP(lineCount); \
	(cmd)[5] = CELL_GCM_ENDIAN_SWAP(((outFormat) << 8) | (inFormat)); \
	CELL_GCM_DEBUG_CHECK(cmd); \
	(cmd) += 6; \
} while(false)

#undef CELL_GCM_METHOD_COPY2D_OFFSET_IN
#define CELL_GCM_METHOD_COPY2D_OFFSET_IN(cmd, inOffset) do { \
	(cmd)[0] = CELL_GCM_METHOD(CELL_GCM_NV0039_OFFSET_IN, 1); \
	(cmd)[1] = CELL_GCM_ENDIAN_SWAP(inOffset); \
	CELL_GCM_DEBUG_CHECK(cmd); \
	(cmd) += 2; \
} while(false)

#undef CELL_GCM_METHOD_COPY2D_BUFFER_NOTIFY
#define CELL_GCM_METHOD_COPY2D_BUFFER_NOTIFY(cmd, notify) do { \
	(cmd)[0] = CELL_GCM_METHOD(CELL_GCM_NV0039_BUFFER_NOTIFY, 1); \
	(cmd)[1] = CELL_GCM_ENDIAN_SWAP(notify); \
	CELL_GCM_DEBUG_CHECK(cmd); \
	(cmd) += 2; \
} while(false)

#undef CELL_GCM_METHOD_SET_POLY_OFFSET_LINE_ENABLE
#define CELL_GCM_METHOD_SET_POLY_OFFSET_LINE_ENABLE(cmd, enable) do { \
	(cmd)[0] = CELL_GCM_METHOD(CELL_GCM_NV4097_SET_POLY_OFFSET_LINE_ENABLE, 1); \
	(cmd)[1] = CELL_GCM_ENDIAN_SWAP(enable); \
	CELL_GCM_DEBUG_CHECK(cmd); \
	(cmd) += 2; \
} while(false)

#undef CELL_GCM_METHOD_SET_ANISO_SPREAD
#define CELL_GCM_METHOD_SET_ANISO_SPREAD(cmd, index, value) do { \
	(cmd)[0] = CELL_GCM_METHOD(CELL_GCM_NV4097_SET_ANISO_SPREAD + (index) * 4, 1); \
	(cmd)[1] = CELL_GCM_ENDIAN_SWAP(value); \
	CELL_GCM_DEBUG_CHECK(cmd); \
	(cmd) += 2; \
} while(false)

#undef CELL_GCM_METHOD_SET_VERTEX_DATA_BASE
#define CELL_GCM_METHOD_SET_VERTEX_DATA_BASE(cmd, baseOffset, baseIndex) do { \
	(cmd)[0] = CELL_GCM_METHOD(CELL_GCM_NV4097_SET_VERTEX_DATA_BASE_OFFSET, 2); \
	(cmd)[1] = CELL_GCM_ENDIAN_SWAP(baseOffset); \
	(cmd)[2] = CELL_GCM_ENDIAN_SWAP(baseIndex); \
	CELL_GCM_DEBUG_CHECK(cmd); \
	(cmd) += 3; \
} while(false)

#undef CELL_GCM_METHOD_SET_CLEAR_ZCULL_SURFACE
#define CELL_GCM_METHOD_SET_CLEAR_ZCULL_SURFACE(cmd, depth, stencil) do { \
	(cmd)[0] = CELL_GCM_METHOD(CELL_GCM_NV4097_CLEAR_ZCULL_SURFACE, 1); \
	(cmd)[1] = CELL_GCM_ENDIAN_SWAP(depth | (stencil<<1)); \
	CELL_GCM_DEBUG_CHECK(cmd); \
	(cmd) += 2; \
} while(false)

#undef CELL_GCM_METHOD_SET_ZCULL_ENABLE
#define CELL_GCM_METHOD_SET_ZCULL_ENABLE(cmd, depth, stencil) do { \
	(cmd)[0] = CELL_GCM_METHOD(CELL_GCM_NV4097_SET_ZCULL_EN, 1); \
	(cmd)[1] = CELL_GCM_ENDIAN_SWAP(depth | (stencil<<1)); \
	CELL_GCM_DEBUG_CHECK(cmd); \
	(cmd) += 2; \
} while(false)

#undef CELL_GCM_METHOD_DRIVER_INTERRUPT
#define CELL_GCM_METHOD_DRIVER_INTERRUPT(cmd, cause) do { \
	(cmd)[0] = CELL_GCM_METHOD(CELL_GCM_DRIVER_INTERRUPT, 1); \
	(cmd)[1] = CELL_GCM_ENDIAN_SWAP(cause); \
	(cmd) += 2; \
	} while(false)

#undef CELL_GCM_METHOD_DRIVER_QUEUE
#define CELL_GCM_METHOD_DRIVER_QUEUE(cmd, id) do { \
	(cmd)[0] = CELL_GCM_METHOD(CELL_GCM_DRIVER_QUEUE, 1); \
	(cmd)[1] = CELL_GCM_ENDIAN_SWAP(id); \
	(cmd) += 2; \
	} while(false)

#undef CELL_GCM_METHOD_DRIVER_FLIP
#define CELL_GCM_METHOD_DRIVER_FLIP(cmd, value) do { \
	(cmd)[0] = CELL_GCM_METHOD(CELL_GCM_DRIVER_FLIP, 1); \
	(cmd)[1] = CELL_GCM_ENDIAN_SWAP(value); \
	(cmd) += 2; \
	} while(false)

#undef CELL_GCM_METHOD_CHANNEL_SET_CONTEXT_DMA_SEMAPHORE
#define CELL_GCM_METHOD_CHANNEL_SET_CONTEXT_DMA_SEMAPHORE(cmd, handle) do { \
	(cmd)[0] = CELL_GCM_METHOD(CELL_GCM_NV406E_SET_CONTEXT_DMA_SEMAPHORE, 1); \
	(cmd)[1] = CELL_GCM_ENDIAN_SWAP(handle); \
	(cmd) += 2; \
	} while(false)

#undef CELL_GCM_METHOD_SET_CONTEXT_DMA_REPORT
#define CELL_GCM_METHOD_SET_CONTEXT_DMA_REPORT(cmd, handle) do { \
	(cmd)[0] = CELL_GCM_METHOD(CELL_GCM_NV4097_SET_CONTEXT_DMA_REPORT, 1); \
	(cmd)[1] = CELL_GCM_ENDIAN_SWAP(handle); \
	CELL_GCM_DEBUG_CHECK(cmd); \
	(cmd) += 2; \
} while(false)

#undef CELL_GCM_METHOD_SET_ZCULL_SYNC
#define CELL_GCM_METHOD_SET_ZCULL_SYNC(cmd, value) do { \
	(cmd)[0] = CELL_GCM_METHOD(CELL_GCM_NV4097_ZCULL_SYNC, 1); \
	(cmd)[1] = CELL_GCM_ENDIAN_SWAP(value); \
	CELL_GCM_DEBUG_CHECK(cmd); \
	(cmd) += 2; \
} while(false)

#undef CELL_GCM_METHOD_SET_CONTEXT_DMA_SEMAPHORE
#define CELL_GCM_METHOD_SET_CONTEXT_DMA_SEMAPHORE(cmd, handle) do { \
	(cmd)[0] = CELL_GCM_METHOD(CELL_GCM_NV4097_SET_CONTEXT_DMA_SEMAPHORE, 1); \
	(cmd)[1] = CELL_GCM_ENDIAN_SWAP(handle); \
	CELL_GCM_DEBUG_CHECK(cmd); \
	(cmd) += 2; \
} while(false)

#undef CELL_GCM_METHOD_NV4097_CONTEXT_DMA_NOTIFIES
#define CELL_GCM_METHOD_NV4097_CONTEXT_DMA_NOTIFIES(cmd, handle) do {					\
    (cmd)[0] = CELL_GCM_METHOD(CELL_GCM_NV4097_SET_CONTEXT_DMA_NOTIFIES, 1);\
    (cmd)[1] = CELL_GCM_ENDIAN_SWAP(handle);\
	CELL_GCM_DEBUG_CHECK(cmd); \
	(cmd) += 2; \
	} while(false)

#undef CELL_GCM_METHOD_NV4097_NOTIFY
#define CELL_GCM_METHOD_NV4097_NOTIFY(cmd, type) do { \
	(cmd)[0] = CELL_GCM_METHOD(CELL_GCM_NV4097_NOTIFY, 1); \
	(cmd)[1] = CELL_GCM_ENDIAN_SWAP(type); \
	CELL_GCM_DEBUG_CHECK(cmd); \
	(cmd) += 2; \
	} while(false)
