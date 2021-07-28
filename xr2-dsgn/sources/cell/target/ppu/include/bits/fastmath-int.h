/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2006 Sony Computer Entertainment Inc.
 * All Rights Reserved.
 */
#ifndef _FASTMATHINT_H
#define _FASTMATHINT_H

#ifndef __YVALS
#include <yvals.h>
#endif

_INLINE_DEF float f_fmaxf(float, float);
_INLINE_DEF float f_fminf(float, float);
_INLINE_DEF float f_copysignf(float, float);
_INLINE_DEF float f_fdimf(float, float);
_INLINE_DEF float f_sqrtf(float);
_INLINE_DEF float f_hypotf(float, float);
_INLINE_DEF float f_rintf (float x);
_INLINE_DEF long int f_lrintf (float x);
_INLINE_DEF long long int f_llrintf (float x);
_INLINE_DEF float f_nearbyintf (float x);
_INLINE_DEF float f_fmaf(float, float, float);
_INLINE_DEF float f_fmodf(float, float);
_INLINE_DEF float f_log2f(float);
_INLINE_DEF float f_logf(float);
_INLINE_DEF float f_log10f(float);
_INLINE_DEF float f_exp2f(float);
_INLINE_DEF float f_expf(float);
_INLINE_DEF float f_ceilf(float);
_INLINE_DEF float f_floorf(float);
_INLINE_DEF float f_sinf(float);
_INLINE_DEF float f_cosf(float);

#endif


