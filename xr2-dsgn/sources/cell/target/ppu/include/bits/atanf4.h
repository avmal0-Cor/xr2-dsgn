/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2008 Sony Computer Entertainment Inc. 
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
    vec_float4 xabs;
    vec_float4 vzero = (vec_float4)vec_xor((vec_uint4)(0), (vec_uint4)(0));
    vec_uint4 smask = (vec_uint4)(vec_int4)(-1);
    smask = vec_vslw(smask, smask);
    vec_float4 vnzero = vec_or(vzero, (vec_float4)smask);
    
    sign = vec_and((vec_uint4)x, smask);
    xabs = (vec_float4)vec_andc((vec_uint4)x, smask);
    inv_x = recipf4(x);
    inv_x = (vec_float4)vec_xor((vec_uint4)inv_x, smask);
    select = (vec_uint4)vec_cmpgt(xabs, (vec_float4)(1.0f));
    bias = (vec_float4)(1.57079632679489661923f);
    bias = (vec_float4)vec_and((vec_uint4)bias, select);
    bias = (vec_float4)vec_or((vec_uint4)sign, (vec_uint4)bias);
    
    x = vec_sel(x, inv_x, select);
    
    /* Instruction counts can be reduced if the polynomial was
     * computed entirely from nested (dependent) fma's. However, 
     * to reduce the number of pipeline stalls, the polygon is evaluated 
     * in two halves(hi and lo).
     */
    bias = vec_add(bias, x);
    x2 = vec_madd(x,  x,  vzero);
    x3 = vec_madd(x2, x,  vzero);
    x4 = vec_madd(x2, x2, vzero);
    x8 = vec_madd(x4, x4, vzero);
    x9 = vec_madd(x8, x,  vnzero);
    hi = vec_madd((vec_float4)(0.0028662257f), x2, (vec_float4)(-0.0161657367f));
    hi = vec_madd(hi, x2, (vec_float4)(0.0429096138f));
    hi = vec_madd(hi, x2, (vec_float4)(-0.0752896400f));
    hi = vec_madd(hi, x2, (vec_float4)(0.1065626393f));
    lo = vec_madd((vec_float4)(-0.1420889944f), x2, (vec_float4)(0.1999355085f));
    lo = vec_madd(lo, x2, (vec_float4)(-0.3333314528f));
    lo = vec_madd(lo, x3, bias);
    
    result = vec_madd(hi, x9, lo);    
    return result;
}

