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

//
// Computes the inverse cosine of all four slots of x. 
//
_FUNC_DEF(vec_float4, acosf4, (vec_float4 x))
{
    vec_float4 result, xabs;
    vec_float4 t1;
    vec_float4 xabs2, xabs4;
    vec_float4 hi, lo;
    vec_float4 neg, pos;
    vec_uint4 select;
    vec_float4 vzero = (vec_float4)vec_xor((vec_uint4)(0), (vec_uint4)(0));
    
    xabs = vec_abs(x);
    select = (vec_uint4)(vec_sra((vec_int4)(x), (vec_uint4)(31)));
    
    t1 = sqrtf4(vec_sub((vec_float4)(1.0), xabs));
    
    /* Instruction counts can be reduced if the polynomial was
     * computed entirely from nested (dependent) fma's. However, 
     * to reduce the number of pipeline stalls, the polygon is evaluated 
     * in two halves (hi amd lo). 
     */
    xabs2 = vec_madd(xabs,  xabs, vzero);
    xabs4 = vec_madd(xabs2, xabs2, vzero);
    hi = vec_madd((vec_float4)(-0.0012624911), xabs, (vec_float4)(0.0066700901));
    hi = vec_madd(hi, xabs, (vec_float4)(-0.0170881256));
    hi = vec_madd(hi, xabs, (vec_float4)( 0.0308918810));
    lo = vec_madd((vec_float4)(-0.0501743046), xabs, (vec_float4)(0.0889789874));
    lo = vec_madd(lo, xabs, (vec_float4)(-0.2145988016));
    lo = vec_madd(lo, xabs, (vec_float4)( 1.5707963050));
    
    result = vec_madd(hi, xabs4, lo);
    
    /* Adjust the result if x is negactive.
     */
    neg = vec_nmsub(t1, result, (vec_float4)(3.1415926535898f));
    pos = vec_madd(t1, result, vzero);
    
    result = vec_sel(pos, neg, select);
    
    return result;
}


