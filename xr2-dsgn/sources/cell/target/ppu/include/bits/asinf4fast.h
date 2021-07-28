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
/* 2001,2002,2003. All rights reserved.                                 */
/* S/T/I Confidential Information                                  */
/* --------------------------------------------------------------  */
/* PROLOG END TAG zYx                                              */

//
// A faster but less accurate implementation of the inverse sine function.
// The results are expected to be accurate to at least 8 bits of the mantissa. On the
// average the results have been observed to be accurate to 19 bits. 
//
_FUNC_DEF(vec_float4, asinf4fast, (vec_float4 x))
{
    vec_uint4 mask = (vec_uint4)(vec_int4)(-1);
    mask = vec_vslw(mask, mask);
    vec_float4 vzero = (vec_float4)vec_xor((vec_uint4)(0), (vec_uint4)(0));
    vec_uint4 sign;
    vec_float4 result, xabs;
    vec_float4 t1;
    vec_float4 xabs2, xabs4;
    vec_float4 hi, lo;
    
    /* Extract the sign of x and compute the absolute value of x (in)
     */
    sign = vec_and((vec_uint4)(x), mask);
    xabs = vec_abs(x);
    
    t1 = sqrtf4(vec_sub((vec_float4)(1.0f), xabs));
    
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
    result = vec_nmsub(t1, result, (vec_float4)(1.5707963267949f));
    
    /* Correct resulting sign
     */
    result = (vec_float4)(vec_or((vec_uint4)(result), sign));
    
    return result;
}

