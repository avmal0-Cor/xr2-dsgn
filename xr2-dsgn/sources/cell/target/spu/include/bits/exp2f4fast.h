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
/*
 * FUNCTION
 *	vec_float4 _exp2_v(vec_float4 x)
 *
 * DESCRIPTION
 *	_exp2_v computes 2 raised to the input vector x. Computation is 
 *	performed by observing the 2^(a+b) = 2^a * 2^b.
 *	We decompose x into a and b (above) by letting.
 *	a = ceil(x), b = x - a; 
 *
 *	2^a is easilty computed by placing a into the exponent
 *	or a floating point number whose mantissa is all zeros.
 *
 *	2^b is computed using the following polynomial approximation.
 *	(C. Hastings, Jr, 1955).
 *
 *                __7__
 *		  \
 *		   \ 
 *	2^(-x) =   /     Ci*x^i
 *                /____
 *                 i=1
 *
 *	for x in the range 0.0 to 1.0
 *
 *	C0 =  1.0
 *	C1 = -0.9999999995
 *	C2 =  0.4999999206
 *	C3 = -0.1666653019
 *	C4 =  0.0416573475
 *	C5 = -0.0083013598
 *	C6 =  0.0013298820
 *	C7 = -0.0001413161
 *
 *	This function does not handle out of range conditions. It
 *	assumes that x is in the range (-128.0, 127.0]. Values outside
 *	this range will produce undefined results.
 */


#define _EXP2F_H_LN2	0.69314718055995f	/* ln(2) */
_FUNC_DEF(vec_float4, exp2f4fast, (vec_float4 x))
{
  vec_int4 ix;
  vec_float4 frac, frac2, frac4;
  vec_float4 exp_int, exp_frac;
  vec_float4 result;
  vec_float4 hi, lo;

  vec_float4 bias;
  /* Break in the input x into two parts ceil(x), x - ceil(x).
   */
  bias = (vec_float4)(spu_rlmaska((vec_int4)(x), -31));
  bias = (vec_float4)(spu_andc((vec_uint4)(0x3F7FFFFF), (vec_uint4)bias));
  ix = spu_convts(spu_add(x, bias), 0);
  frac = spu_sub(spu_convtf(ix, 0), x);
  frac = spu_mul(frac, (vec_float4)(_EXP2F_H_LN2));

  exp_int = (vec_float4)(spu_sl(spu_add(ix, 127), 23));

  /* Instruction counts can be reduced if the polynomial was
   * computed entirely from nested (dependent) fma's. However, 
   * to reduce the number of pipeline stalls, the polygon is evaluated 
   * in two halves (hi amd lo). 
   */
  frac2 = spu_mul(frac, frac);
  frac4 = spu_mul(frac2, frac2);

  hi = spu_madd(frac, (vec_float4)(-0.0001413161), (vec_float4)(0.0013298820));
  hi = spu_madd(frac, hi, (vec_float4)(-0.0083013598));
  hi = spu_madd(frac, hi, (vec_float4)(0.0416573475));
  lo = spu_madd(frac, (vec_float4)(-0.1666653019), (vec_float4)(0.4999999206));
  lo = spu_madd(frac, lo, (vec_float4)(-0.9999999995));
  lo = spu_madd(frac, lo, (vec_float4)(1.0));

  exp_frac = spu_madd(frac4, hi, lo);
  ix = spu_add(ix, spu_rlmask((vec_int4)(exp_frac), -23));
  result = spu_mul(exp_frac, exp_int);

  return (result);

}
