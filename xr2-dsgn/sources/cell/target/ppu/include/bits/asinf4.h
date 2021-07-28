/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2008 Sony Computer Entertainment Inc. 
 * All Rights Reserved.
 */

//
// Computes the inverse sine of all four slots of x
//
_FUNC_DEF(vec_float4, asinf4, (vec_float4 x))
{
    vec_uint4 smask = (vec_uint4)(vec_int4)(-1);
    smask = vec_vslw(smask, smask);
    vec_float4 vzero = (vec_float4)vec_xor((vec_uint4)(0), (vec_uint4)(0));
    // positive = (x > 0)
    //
    vec_uint4 positive = (vec_uint4)vec_cmpgt(x, vzero);

    // x = absf(x)
    //
    x = vec_abs(x);

    // gtHalf = (|x| > 0.5)
    //
    vec_uint4 gtHalf = (vec_uint4)vec_cmpgt(x,(vec_float4)(0.5f));    


    // if (x > 0.5)
    //    g = 0.5 - 0.5*x
    //    x = -2 * sqrtf(g)
    // else
    //    g = x * x
    //
    vec_float4 g = vec_sel(vec_madd(x,x, vzero),vec_madd((vec_float4)(-0.5f),x,(vec_float4)(0.5f)),gtHalf);
    
    x = vec_sel(x,vec_madd((vec_float4)(-2.0f),sqrtf4(g), vzero),gtHalf);

    // Compute the polynomials and take their ratio
    //  denom = (1.0f*g + -0.554846723e+1f)*g + 5.603603363f
    //  num = x * g * (-0.504400557f * g + 0.933933258f)
    //
    vec_float4 denom = vec_add(g,(vec_float4)(-5.54846723f));
    vec_float4 num = vec_madd((vec_float4)(-0.504400557f),g,(vec_float4)(0.933933258f));
    denom = vec_madd(denom,g,(vec_float4)(5.603603363f));
    num = vec_madd(vec_madd(x,g, vzero),num, vzero);

    
    // x = x + num / denom
    //
    x = vec_add(x,divf4(num,denom));

    // if (x > 0.5)
    //    x = x + M_PI_2
    //
    x = vec_sel(x,vec_add(x,(vec_float4)(1.57079632679489661923f)),gtHalf);

    
    // if (!positive) x = -x
    //
    x = vec_sel((vec_float4)vec_xor((vec_int4)smask, (vec_int4)x),x,positive);

    return x;
}

