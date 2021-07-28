/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2004 Sony Computer Entertainment Inc. 
* All Rights Reserved.
*/
#ifndef _SCE_MATH_H
#define _SCE_MATH_H

/** COMMON INCLUDES **/
#include <yvals.h>
#include <altivec.h>

#include <vec_types.h>

_C_STD_BEGIN

/** PROTOTYPES **/
_C_LIB_DECL

/* SCE vector versions of estimated or non-standard functions. 
   These functions are estimated vector versions of standard C functions
   or functions that do not correspond to any standard C function.  */

_INLINE_DEF vec_float4 acosf4fast(vec_float4 x);
_INLINE_DEF vec_float4 asinf4fast(vec_float4 x);
_INLINE_DEF vec_float4 atan2f4fast(vec_float4 y, vec_float4 x);
_INLINE_DEF vec_float4 atanf4fast(vec_float4 x);
_INLINE_DEF vec_float4 cbrtf4fast(vec_float4 x);
_INLINE_DEF vec_float4 cosf4fast(vec_float4 x);
_INLINE_DEF vec_float4 divf4(vec_float4 x, vec_float4 y);
_INLINE_DEF vec_float4 divf4fast(vec_float4 x, vec_float4 y);
_INLINE_DEF vec_float4 exp2f4fast(vec_float4 x);
_INLINE_DEF vec_float4 expf4fast(vec_float4 x);
_INLINE_DEF vec_float4 expm1f4fast(vec_float4 x);
_INLINE_DEF vec_float4 log10f4fast(vec_float4 x);
_INLINE_DEF vec_float4 log1pf4fast(vec_float4 x);
_INLINE_DEF vec_float4 log2f4fast(vec_float4 x);
_INLINE_DEF vec_float4 logf4fast(vec_float4 x);
_INLINE_DEF vec_float4 negatef4(vec_float4 x);
_INLINE_DEF vec_float4 powf4fast(vec_float4 x, vec_float4 y);
_INLINE_DEF vec_float4 recipf4(vec_float4 x);
_INLINE_DEF vec_float4 recipf4fast(vec_float4 x);
_INLINE_DEF vec_float4 rsqrtf4(vec_float4 x);
_INLINE_DEF vec_float4 rsqrtf4fast(vec_float4 x);
_INLINE_DEF void       sincosf4fast(vec_float4 x, vec_float4* s, vec_float4* c);
_INLINE_DEF vec_float4 sinf4fast(vec_float4 x);
_INLINE_DEF vec_float4 sqrtf4fast(vec_float4 x);
_INLINE_DEF vec_float4 tanf4fast(vec_float4 x);

/* SCE vector version of standard functions. 
   These functions are vector versions of the standard C functions. */

_INLINE_DEF vec_float4 acosf4(vec_float4);
_INLINE_DEF vec_float4 asinf4(vec_float4);
_INLINE_DEF vec_float4 atanf4(vec_float4);
_INLINE_DEF vec_float4 atan2f4(vec_float4, vec_float4);
_INLINE_DEF vec_float4 ceilf4(vec_float4);
_INLINE_DEF vec_float4 copysignf4(vec_float4, vec_float4);
_INLINE_DEF vec_float4 cosf4(vec_float4 x);
_INLINE_DEF vec_float4 expf4(vec_float4);
_INLINE_DEF vec_float4 fabsf4(vec_float4);
_INLINE_DEF vec_float4 floorf4(vec_float4);
_INLINE_DEF vec_float4 fmaxf4(vec_float4, vec_float4);
_INLINE_DEF vec_float4 fminf4(vec_float4, vec_float4);
_INLINE_DEF vec_float4 fmodf4(vec_float4, vec_float4);
_INLINE_DEF vec_float4 frexpf4(vec_float4, vec_int4 *);
_INLINE_DEF vec_float4 ldexpf4(vec_float4, vec_int4);
_INLINE_DEF vec_float4 logf4(vec_float4);
_INLINE_DEF vec_float4 log10f4(vec_float4);
_INLINE_DEF vec_float4 modff4(vec_float4, vec_float4*);
_INLINE_DEF vec_float4 remainderf4(vec_float4, vec_float4);
_INLINE_DEF void       sincosf4(vec_float4 x, vec_float4* s, vec_float4* c);
_INLINE_DEF vec_float4 powf4(vec_float4, vec_float4);
_INLINE_DEF vec_float4 sinf4(vec_float4 x);
_INLINE_DEF vec_float4 sqrtf4(vec_float4 x);
_INLINE_DEF vec_float4 tanf4(vec_float4 x);
_INLINE_DEF vec_float4 truncf4(vec_float4);

_INLINE_DEF vec_float4 cbrtf4(vec_float4);
_INLINE_DEF vec_float4 expm1f4(vec_float4);
_INLINE_DEF vec_float4 exp2f4(vec_float4);
_INLINE_DEF vec_float4 fdimf4(vec_float4, vec_float4);
_INLINE_DEF vec_float4 fmaf4(vec_float4, vec_float4, vec_float4);
_INLINE_DEF vec_float4 hypotf4(vec_float4, vec_float4);
_INLINE_DEF vec_int4 ilogbf4(vec_float4);


_INLINE_DEF vec_float4 log1pf4(vec_float4);
_INLINE_DEF vec_float4 log2f4(vec_float4);
_INLINE_DEF vec_float4 logbf4(vec_float4);

_END_C_LIB_DECL


/** INLINE DEFINITIONS **/
#ifndef _NO_INLINE_DEFINITIONS
#include <bits/spec.h>

/* SIMD */
/* Eric */
#include <bits/truncf4.h>
#include <bits/ceilf4.h>
#include <bits/copysignf4.h>
#include <bits/divf4.h>
#include <bits/divf4fast.h>
#include <bits/fabsf4.h>
#include <bits/floorf4.h>
#include <bits/fmaxf4.h>
#include <bits/fminf4.h>
#include <bits/fmodf4.h>
#include <bits/modff4.h>
#include <bits/negatef4.h>
#include <bits/recipf4.h>
#include <bits/recipf4fast.h>
#include <bits/remainderf4.h>
#include <bits/rsqrtf4.h>
#include <bits/rsqrtf4fast.h>
#include <bits/sqrtf4.h>
#include <bits/sqrtf4fast.h>

/* Vangelis */
#include <bits/sinf4.h>
#include <bits/cosf4.h>
#include <bits/sincosf4.h>
#include <bits/tanf4.h>
#include <bits/asinf4.h>
#include <bits/acosf4.h>
#include <bits/atanf4.h>
#include <bits/atan2f4.h>
#include <bits/sinf4fast.h>
#include <bits/cosf4fast.h>
#include <bits/sincosf4fast.h>
#include <bits/tanf4fast.h>
#include <bits/acosf4fast.h>
#include <bits/asinf4fast.h>
#include <bits/atanf4fast.h>
#include <bits/atan2f4fast.h>

/* Rick */
#include <bits/fmaf4.h>
#include <bits/fdimf4.h>
#include <bits/hypotf4.h>

/* Rishi */
#include <bits/frexpf4.h>
#include <bits/ldexpf4.h>
#include <bits/log2f4.h>
#include <bits/log2f4fast.h>
#include <bits/logf4.h>
#include <bits/logf4fast.h>
#include <bits/log10f4.h>
#include <bits/log10f4fast.h>
#include <bits/log1pf4.h>
#include <bits/log1pf4fast.h>
#include <bits/logbf4.h>
#include <bits/ilogbf4.h>
#include <bits/exp2f4.h>
#include <bits/exp2f4fast.h>
#include <bits/expf4.h>
#include <bits/expf4fast.h>
#include <bits/expm1f4.h>
#include <bits/expm1f4fast.h>
#include <bits/powf4.h>
#include <bits/powf4fast.h>
#include <bits/cbrtf4.h>
#include <bits/cbrtf4fast.h>
                                                                                                                                                                                  
#endif

_C_STD_END

#endif /* _SCE_MATH_H_ */

 #if defined (_STD_USING)
using _CSTD acosf4fast;
using _CSTD asinf4fast;
using _CSTD atan2f4fast;
using _CSTD atanf4fast;
using _CSTD cbrtf4fast;
using _CSTD cosf4fast;
using _CSTD divf4;
using _CSTD divf4fast;
using _CSTD exp2f4fast;
using _CSTD expf4fast;
using _CSTD expm1f4fast;
using _CSTD log10f4fast;
using _CSTD log1pf4fast;
using _CSTD log2f4fast;
using _CSTD logf4fast;
using _CSTD negatef4;
using _CSTD powf4fast;
using _CSTD recipf4;
using _CSTD recipf4fast;
using _CSTD rsqrtf4;
using _CSTD rsqrtf4fast;
using _CSTD sincosf4fast;
using _CSTD sinf4fast;
using _CSTD sqrtf4fast;
using _CSTD tanf4fast;

using _CSTD acosf4;
using _CSTD asinf4;
using _CSTD atanf4;
using _CSTD atan2f4;
using _CSTD ceilf4;
using _CSTD copysignf4;
using _CSTD cosf4;
using _CSTD expf4;
using _CSTD fabsf4;
using _CSTD floorf4;
using _CSTD fmaxf4;
using _CSTD fminf4;
using _CSTD fmodf4;
using _CSTD frexpf4;
using _CSTD ldexpf4;
using _CSTD logf4;
using _CSTD log10f4;
using _CSTD modff4;
using _CSTD remainderf4;
using _CSTD sincosf4;
using _CSTD powf4;
using _CSTD sinf4;
using _CSTD sqrtf4;
using _CSTD tanf4;
using _CSTD truncf4;

using _CSTD cbrtf4;
using _CSTD expm1f4;
using _CSTD exp2f4;
using _CSTD fdimf4;
using _CSTD fmaf4;
using _CSTD hypotf4;
using _CSTD ilogbf4;


using _CSTD log1pf4;
using _CSTD log2f4;
using _CSTD logbf4;
 #endif /* defined(_STD_USING) */
