/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2004 Sony Computer Entertainment Inc. 
* All Rights Reserved.
*/
#include <math.h>

//
// Computes the inverse sine of all four slots of x
//
_FUNC_DEF(vec_float4, asinf4, (vec_float4 x))
{
    // positive = (x > 0)
    //
    vec_uchar16 positive = (vec_uchar16)spu_cmpgt(x,spu_splats(0.0f));

    // gtHalf = (|x| > 0.5)
    //
    vec_uchar16 gtHalf = (vec_uchar16)spu_cmpabsgt(x,spu_splats(0.5f));    

    // x = absf(x)
    //
    x = (vec_float4)spu_and((vec_int4)x,spu_splats((int)0x7fffffff));


    // if (x > 0.5)
    //    g = 0.5 - 0.5*x
    //    x = -2 * sqrtf(g)
    // else
    //    g = x * x
    //
    vec_float4 g = spu_sel(spu_mul(x,x),spu_madd(spu_splats(-0.5f),x,spu_splats(0.5f)),gtHalf);
    
    x = spu_sel(x,spu_mul(spu_splats(-2.0f),sqrtf4(g)),gtHalf);

    // Compute the polynomials and take their ratio
    //  denom = (1.0f*g + -0.554846723e+1f)*g + 5.603603363f
    //  num = x * g * (-0.504400557f * g + 0.933933258f)
    //
    vec_float4 denom = spu_add(g,spu_splats(-5.54846723f));
    vec_float4 num = spu_madd(spu_splats(-0.504400557f),g,spu_splats(0.933933258f));
    denom = spu_madd(denom,g,spu_splats(5.603603363f));
    num = spu_mul(spu_mul(x,g),num);

    
    // x = x + num / denom
    //
    x = spu_add(x,divf4(num,denom));

    // if (x > 0.5)
    //    x = x + M_PI_2
    //
    x = spu_sel(x,spu_add(x,spu_splats(1.57079632679489661923f)),gtHalf);

    
    // if (!positive) x = -x
    //
    x = spu_sel((vec_float4)spu_xor(spu_splats((int)0x80000000),(vec_int4)x),x,positive);

    return x;
}

