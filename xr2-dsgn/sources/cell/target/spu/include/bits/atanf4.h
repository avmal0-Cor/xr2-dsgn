/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2004 Sony Computer Entertainment Inc. 
* All Rights Reserved.
*/
/* --------------------------------------------------------------  */
/* PLEASE DO NOT MODIFY THIS SECTION                               */
/* This prolog section is automatically generated.                 */
/*                                                                 */
/* (C)Copyright                                                    */
/* Sony Computer Entertainment, Inc.,                              */
/* Toshiba Corporation,                                            */
/* International Business Machines Corporation,                    */
/* 2001,2002,2003. All rights reserved.                            */
/* S/T/I Confidential Information                                  */
/* --------------------------------------------------------------  */
/* PROLOG END TAG zYx                                              */
 

//
// Computes the inverse tangent of all four slots of x. 
//
_FUNC_DEF(vec_float4, atanf4, (vec_float4 x))
{
    vec_float4 bias;
    vec_float4 x2, x3, x4, x8, x9;
    vec_float4 hi, lo;
    vec_float4 result;
    vec_float4 inv_x;
    vec_uint4 sign;
    vec_uint4 select;
    
    sign = spu_sl(spu_rlmask((vec_uint4)x, -31), 31);
    inv_x = recipf4(x);
    inv_x = (vec_float4)spu_xor((vec_uint4)inv_x, (vec_uint4)(0x80000000));
    
    select = (vec_uint4)spu_cmpabsgt(x, (vec_float4)(1.0));
    bias = (vec_float4)spu_or(sign, (vec_uint4)((vec_float4)(1.57079632679489661923f)));
    bias = (vec_float4)spu_and((vec_uint4)bias, select);
    
    x = spu_sel(x, inv_x, select);
    
    bias = spu_add(bias, x);
    x2 = spu_mul(x, x);
    x3 = spu_mul(x2, x);
    x4 = spu_mul(x2, x2);
    x8 = spu_mul(x4, x4);
    x9 = spu_mul(x8, x);
    hi = spu_madd((vec_float4)(0.0028662257), x2, (vec_float4)(-0.0161657367));
    hi = spu_madd(hi, x2, (vec_float4)(0.0429096138));
    hi = spu_madd(hi, x2, (vec_float4)(-0.0752896400));
    hi = spu_madd(hi, x2, (vec_float4)(0.1065626393));
    lo = spu_madd((vec_float4)(-0.1420889944), x2, (vec_float4)(0.1999355085));
    lo = spu_madd(lo, x2, (vec_float4)(-0.3333314528));
    lo = spu_madd(lo, x3, bias);
    
    result = spu_madd(hi, x9, lo);
    
    return result;
}

