/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2008 Sony Computer Entertainment Inc. 
 * All Rights Reserved.
 */

//
// A faster but less accurate implementation of the atan2f4 function.
// The results are correct to approximately 12 bits of the mantissa. 
//
_FUNC_DEF(vec_float4, atan2f4fast, (vec_float4 y, vec_float4 x))
{
    vec_float4 vzero = (vec_float4)vec_xor((vec_uint4)(0), (vec_uint4)(0));
    vec_float4 res = atanf4fast(divf4fast(y,x));

    // Use the arguments to determine the quadrant of the result:
    // if (x < 0)
    //   if (y < 0)
    //      res = -PI + res
    //   else
    //      res = PI + res
    //
    vec_uint4 yNeg = (vec_uint4)vec_cmpgt(vzero,y);
    vec_uint4 xNeg = (vec_uint4)vec_cmpgt(vzero,x);

    vec_float4 bias = vec_sel((vec_float4)(3.14159265358979323846f),(vec_float4)(-3.14159265358979323846f),yNeg);

    vec_float4 newRes = vec_add(bias, res);

    res = vec_sel(res,newRes,xNeg);

    return res;
}
