/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2005 Sony Computer Entertainment Inc.
 * All Rights Reserved.
 */

#ifndef _FASTMATH_H
#define _FASTMATH_H

#ifndef __YVALS
#include <yvals.h>
#endif
#include <math.h>

_C_STD_BEGIN

_C_LIB_DECL
#include <bits/fastmath-int.h>

#if defined __GNUC__ 

#undef fabsf
#define fabsf(x)        	__builtin_fabsf(x)

#undef isgreater
#define isgreater(x, y) 	__builtin_isgreater (x, y)

#undef isgreaterequal
#define isgreaterequal(x, y) 	__builtin_isgreaterequal (x, y)

#undef isless
#define isless(x, y) 		__builtin_isless (x, y)

#undef islessequal
#define islessequal(x, y) 	__builtin_islessequal (x, y)

#undef islessgreater
#define islessgreater(x, y) 	__builtin_islessgreater (x, y)

#undef isunordered
#define isunordered(x, y) 	__builtin_isunordered (x, y)

#endif /* __GNUC__ */

/** INLINE DEFINITIONS **/
#ifndef _NO_INLINE_DEFINITIONS

#include <bits/f_fmaxf.h>
#include <bits/f_fminf.h>
#include <bits/f_copysignf.h>
#include <bits/f_fdimf.h>
#include <bits/f_sqrtf.h>
#include <bits/f_hypotf.h>
#include <bits/f_rintf.h>
#include <bits/f_lrintf.h>
#include <bits/f_llrintf.h>
#include <bits/f_nearbyintf.h>
#include <bits/f_fmaf.h>
#include <bits/f_fmodf.h>
#include <bits/f_log2f.h>
#include <bits/f_logf.h>
#include <bits/f_log10f.h>
#include <bits/f_exp2f.h>
#include <bits/f_expf.h>
#include <bits/f_ceilf.h>
#include <bits/f_floorf.h>
#include <bits/f_sinf.h>
#include <bits/f_cosf.h>

#endif /* _NO_INLINE_DEFINITIONS */

float f_acosf(float);
float f_asinf(float);
float f_atanf(float);
float f_atan2f(float, float);
float f_frexpf(float, int*);
float f_ldexpf(float, int);
long long int f_llroundf(float);
float f_log1pf(float);
long int f_lroundf(float);
float f_tanf(float);
float f_powf(float, float);

_END_C_LIB_DECL
_C_STD_END

#undef fmaxf
#define fmaxf(x,y)      f_fmaxf(x,y)

#undef fminf
#define fminf(x,y)      f_fminf(x,y)

#undef copysignf
#define copysignf(x,y)  f_copysignf(x,y)

#undef fdimf
#define fdimf(x,y)      f_fdimf(x,y)

#undef sqrtf
#define sqrtf(x)       	f_sqrtf(x)

#undef hypotf
#define hypotf(x,y)     f_hypotf(x,y)

#undef rintf
#define rintf(x)       	f_rintf(x)

#undef lrintf
#define lrintf(x)       f_lrintf(x)

#undef llrintf
#define llrintf(x)      f_llrintf(x)

#undef nearbyintf
#define nearbyintf(x) 	f_nearbyintf(x)

#undef fmaf
#define fmaf(x,y,z)     f_fmaf(x,y,z)

#undef fmodf
#define fmodf(x,y)	f_fmodf(x,y)

#undef log2f
#define log2f(x) 	f_log2f(x)

#undef logf
#define logf(x)         f_logf(x)

#undef log10f
#define log10f(x)       f_log10f(x)

#undef exp2f
#define exp2f(x)	f_exp2f(x)

#undef expf
#define expf(x)         f_expf(x)

#undef ceilf
#define ceilf(x)        f_ceilf(x)

#undef floorf
#define floorf(x)       f_floorf(x)

#undef sinf
#define sinf(x)         f_sinf(x)

#undef cosf
#define cosf(x)         f_cosf(x)

#undef acosf
#define acosf(x)	f_acosf(x)

#undef asinf
#define asinf(x)	f_asinf(x)

#undef atanf
#define atanf(x)	f_atanf(x)

#undef atan2f
#define atan2f(x,y)	f_atan2f(x,y)

#undef frexpf
#define frexpf(x,y)	f_frexpf(x,y)

#undef ldexpf
#define ldexpf(x,y)	f_ldexpf(x,y)

#undef llroundf
#define llroundf(x)	f_llroundf(x)

#undef log1pf
#define log1pf(x)	f_log1pf(x)

#undef lroundf
#define lroundf(x)	f_lroundf(x)

#undef tanf
#define tanf(x)		f_tanf(x)

#undef powf
#define powf(x,y)	f_powf(x,y)

#if defined(_STD_USING)
using _CSTD f_fmaxf;
using _CSTD f_fminf;
using _CSTD f_copysignf;
using _CSTD f_fdimf;
using _CSTD f_sqrtf;
using _CSTD f_hypotf;
using _CSTD f_rintf; 
using _CSTD f_lrintf;
using _CSTD f_llrintf;
using _CSTD f_nearbyintf;
using _CSTD f_fmaf; 
using _CSTD f_fmodf; 
using _CSTD f_log2f; 
using _CSTD f_logf;
using _CSTD f_log10f;
using _CSTD f_exp2f;
using _CSTD f_expf;
using _CSTD f_ceilf; 
using _CSTD f_floorf;
using _CSTD f_sinf;
using _CSTD f_cosf;
using _CSTD f_acosf;
using _CSTD f_asinf;
using _CSTD f_atanf;
using _CSTD f_atan2f;
using _CSTD f_frexpf;
using _CSTD f_ldexpf;
using _CSTD f_llroundf;
using _CSTD f_log1pf;
using _CSTD f_lroundf;
using _CSTD f_tanf;
using _CSTD f_powf;
#endif /* _STD_USING */

#endif /* _FASTMATH_H_ */

