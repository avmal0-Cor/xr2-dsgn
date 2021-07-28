/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2004 Sony Computer Entertainment Inc. 
* All Rights Reserved.
*/
#ifndef _SCE_STDLIB_H_
#define _SCE_STDLIB_H_

/** COMMON INCLUDES **/
#include <yvals.h>
#include <spu_intrinsics.h>

_C_STD_BEGIN

typedef struct
{
   vec_int4 quot; // quotient
   vec_int4 rem;  // remainder
} divi4_t;
                     
/** COMMON CONSTANTS AND MACROS **/




/** NORMAL PROTOTYPES **/
_C_LIB_DECL
vec_int4 absi4(vec_int4 x);
divi4_t divi4(vec_int4 numer, vec_int4 denom);
_END_C_LIB_DECL



/** INLINE DEFINITIONS **/
#ifndef _NO_INLINE_DEFINITIONS
#include <bits/spec.h>
// Simd
#include <bits/absi4.h>
#include <bits/divi4.h>
// Regular
#include <bits/abs.h>
#endif

_C_STD_END

#endif

 #if defined(_STD_USING)
using _CSTD divi4_t;
using _CSTD absi4;
using _CSTD divi4;
 #endif /* defined(_STD_USING) */
