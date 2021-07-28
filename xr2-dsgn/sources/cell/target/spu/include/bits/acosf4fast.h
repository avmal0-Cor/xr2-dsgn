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

//
// A faster but less accurate implementation of the inverse cosine function.
// The results are correct to approximately 12 bits of the mantissa.
// This implementation is about 13% faster than acosf4.
//
_FUNC_DEF(vec_float4, acosf4fast, (vec_float4 x))
{
    vec_float4 result, xabs;
    vec_float4 t1;
    vec_float4 xabs2, xabs4;
    vec_float4 hi, lo;
    vec_float4 neg, pos;
    vec_uint4 select;
    
    xabs   = (vec_float4)(spu_rlmask(spu_sl((vec_uint4)(x), 1), -1));
    select = (vec_uint4)(spu_rlmaska((vector signed int)(x), -31));
    
    t1 = sqrtf4fast(spu_sub((vec_float4)(1.0), xabs));
    
    /* Instruction counts can be reduced if the polynomial was
     * computed entirely from nested (dependent) fma's. However, 
     * to reduce the number of pipeline stalls, the polygon is evaluated 
     * in two halves (hi amd lo). 
     */
    xabs2 = spu_mul(xabs,  xabs);
    xabs4 = spu_mul(xabs2, xabs2);
    hi = spu_madd((vec_float4)(-0.0012624911), xabs, (vec_float4)(0.0066700901));
    hi = spu_madd(hi, xabs, (vec_float4)(-0.0170881256));
    hi = spu_madd(hi, xabs, (vec_float4)( 0.0308918810));
    lo = spu_madd((vec_float4)(-0.0501743046), xabs, (vec_float4)(0.0889789874));
    lo = spu_madd(lo, xabs, (vec_float4)(-0.2145988016));
    lo = spu_madd(lo, xabs, (vec_float4)( 1.5707963050));
    
    result = spu_madd(hi, xabs4, lo);
    
    /* Adjust the result if x is negative.
     */
    neg = spu_nmsub(t1, result, (vec_float4)(3.1415926535898f));
    pos = spu_mul(t1, result);
    
    result = spu_sel(pos, neg, select);
    
    return result;
}


