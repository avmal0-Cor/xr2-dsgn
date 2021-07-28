/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2004 Sony Computer Entertainment Inc. 
* All Rights Reserved.
*/

//
// A faster but less accurate implementation of the atan2f4 function.
// The results are correct to approximately 12 bits of the mantissa. 
// This implementation is about 20% faster than atan2f4.
//
_FUNC_DEF(vec_float4, atan2f4fast, (vec_float4 y, vec_float4 x))
{
    vec_float4 res = atanf4fast(divf4fast(y,x));

    // Use the arguments to determine the quadrant of the result:
    // if (x < 0)
    //   if (y < 0)
    //      res = -PI + res
    //   else
    //      res = PI + res
    //
    vec_uchar16 yNeg = (vec_uchar16)spu_cmpgt(spu_splats(0.0f),y);
    vec_uchar16 xNeg = (vec_uchar16)spu_cmpgt(spu_splats(0.0f),x);

    vec_float4 bias = spu_sel(spu_splats(3.14159265358979323846f),spu_splats(-3.14159265358979323846f),yNeg);

    vec_float4 newRes = spu_add(bias, res);

    res = spu_sel(res,newRes,xNeg);

    return res;
}
