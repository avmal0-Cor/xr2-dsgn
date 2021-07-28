/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2008 Sony Computer Entertainment Inc. 
 * All Rights Reserved.
 */

//
// Inverse tangent function of two variables
//
_FUNC_DEF(vec_float4, atan2f4, (vec_float4 y, vec_float4 x))
{
    vec_uint4 smask = (vec_uint4)(vec_int4)(-1);
    smask = vec_vslw(smask, smask);
    vec_float4 vzero = (vec_float4)vec_xor((vec_uint4)(0), (vec_uint4)(0));
    vec_float4 xx = vec_sel(x, (vec_float4)(1.0f), vec_cmpeq(x, y));
    vec_float4 yy = vec_sel(y, (vec_float4)(1.0f), vec_cmpeq(x, y));
    vec_float4 res = atanf4(divf4(yy,xx));

    // Use the arguments to determine the quadrant of the result:
    // if (x < 0)
    //   if (y < 0)
    //      res = -PI + res
    //   else
    //      res = PI + res
    //
    vec_uint4 signy = (vec_uint4)vec_and((vec_uint4)y, smask);
    vec_uint4 isnan = vec_sel((vec_uint4)x, signy, vec_cmpeq(x, x));
    isnan = vec_sel((vec_uint4)y, isnan, vec_cmpeq(y, y));
    vec_uint4 yNeg = (vec_uint4)vec_cmpgt(vzero,y);
    vec_uint4 xNeg = (vec_uint4)vec_cmpgt(vzero,x);
    vec_uint4 szero = (vec_uint4)vec_cmpeq((vec_uint4)x, (vec_uint4)smask);

    vec_float4 bias = vec_sel((vec_float4)(3.14159265358979323846f),(vec_float4)(-3.14159265358979323846f),yNeg);

    vec_float4 newRes = vec_add(bias, res);

    res = vec_sel(vec_sel(res,newRes,xNeg),newRes,szero);

    return (vec_float4)vec_or((vec_uint4)res, isnan);
}
