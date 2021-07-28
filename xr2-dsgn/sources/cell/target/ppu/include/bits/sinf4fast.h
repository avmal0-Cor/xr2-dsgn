/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2008 Sony Computer Entertainment Inc. 
 * All Rights Reserved.
 */
#include "sincos_c.h"

//
// A faster implementation of sinf4.  Returns accurate (21+ bits of the mantissa) 
// results only for inputs in the range [-pi/2 .. pi/2]. While no error is reported,
// results are unpredictable for inputs outside this range.
//
_FUNC_DEF(vec_float4, sinf4fast, (vec_float4 x))
{
    vec_float4 vzero = (vec_float4)vec_xor((vec_uint4)(0), (vec_uint4)(0));
    vec_uint4 smask = (vec_uint4)(vec_int4)(-1);
    smask = vec_vslw(smask, smask);
    vec_float4 g = vec_madd(x,x,(vec_float4)vzero);
    vec_float4 f = (vec_float4)vec_andc((vec_uint4)x,smask);

    vec_float4 t1 = vec_madd(g,g,(vec_float4)vzero);
    vec_float4 t2 = vec_madd((vec_float4)(_SINCOS_R3),g,(vec_float4)(_SINCOS_R2));
    vec_float4 t3 = vec_madd((vec_float4)(_SINCOS_R1),g,(vec_float4)vzero);
    vec_float4 t4 = vec_madd(t1,t1,(vec_float4)vzero);
    vec_float4 t5 = vec_madd(t1,t2,t3);
    vec_float4 r  = vec_madd((vec_float4)(_SINCOS_R4),t4,t5);

    vec_float4 res = vec_madd(f,r,f);

    res = vec_sel(res, x, smask);

    return res;
}
