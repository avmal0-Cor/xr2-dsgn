/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2004 Sony Computer Entertainment Inc. 
* All Rights Reserved.
*/
#include "sincos_c.h"

//
// A faster implementation of sinf4.  Returns accurate (21+ bits of the mantissa) 
// results only for inputs in the range [-pi/2 .. pi/2]. While no error is reported,
// results are unpredictable for inputs outside this range.
// sinf4fast takes about half as long to run as sinf4
//
_FUNC_DEF(vec_float4, sinf4fast, (vec_float4 x))
{
    vec_float4 g = spu_mul(x,x);
    vec_float4 f = (vec_float4)spu_andc((vec_uint4)x,spu_splats(0x80000000));

    vec_float4 t1 = spu_mul(g,g);
    vec_float4 t2 = spu_madd(spu_splats(_SINCOS_R3),g,spu_splats(_SINCOS_R2));
    vec_float4 t3 = spu_mul(spu_splats(_SINCOS_R1),g);
    vec_float4 t4 = spu_mul(t1,t1);
    vec_float4 t5 = spu_madd(t1,t2,t3);
    vec_float4 r  = spu_madd(spu_splats(_SINCOS_R4),t4,t5);

    vec_float4 res = spu_madd(f,r,f);

    res = spu_sel(res, x, spu_splats(0x80000000));

    return res;
}
