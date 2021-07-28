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
/* 2001,2002,2003. All rights reserved.                                 */
/* S/T/I Confidential Information                                  */
/* --------------------------------------------------------------  */
/* PROLOG END TAG zYx                                              */

/*
 * FUNCTION
 *	vector float _log2f_v(vector float x)
 *
 * DESCRIPTION
 *	_log2f_v computes log (base 2) on a vector if inputs values x. The 
 *	log2f_v function is approximated as a polynomial of order 8
 *	(C. Hastings, Jr, 1955).
 *
 *                   __8__
 *		     \
 *		      \ 
 *	log2f(1+x) =   /     Ci*x^i
 *                   /____
 *                    i=1
 *
 *	for x in the range 0.0 to 1.0
 *
 *	C1 =  1.4426898816672
 *	C2 = -0.72116591947498
 *	C3 =  0.47868480909345
 *	C4 = -0.34730547155299
 *	C5 =  0.24187369696082
 *	C6 = -0.13753123777116
 *	C7 =  0.052064690894143
 *	C8 = -0.0093104962134977
 *
 *	This function assumes that x is a non-zero positive value.
 */
_FUNC_DEF(vec_float4, log2f4fast, (vec_float4 x)) 
{
  vector signed int exponent;
  vector float result;
  vector float x2, x4;
  vector float hi, lo;

  /* Extract the exponent from the input X. 
   */
  exponent = (vector signed int)spu_and(spu_rlmask((vector unsigned int)(x), -23), 0xFFU);
  exponent = spu_add(exponent, -127);

  /* Compute the remainder after removing the exponent.
   */
  x = (vector float)spu_sub((vector signed int)(x), spu_sl(exponent, 23));

  /* Calculate the log2 of the remainder using the polynomial
   * approximation.
   */
  x = spu_sub(x, (vector float)(1.0));

  /* Instruction counts can be reduced if the polynomial was
   * computed entirely from nested (dependent) fma's. However, 
   * to reduce the number of pipeline stalls, the polygon is evaluated 
   * in two halves (hi amd lo). 
   */
  x2 = spu_mul(x, x);
  x4 = spu_mul(x2, x2);

  hi = spu_madd(x, (vector float)(-0.0093104962134977), (vector float)(0.052064690894143));
  hi = spu_madd(x, hi, (vector float)(-0.13753123777116));
  hi = spu_madd(x, hi, (vector float)( 0.24187369696082));
  hi = spu_madd(x, hi, (vector float)(-0.34730547155299));
  lo = spu_madd(x, (vector float)(0.47868480909345), (vector float)(-0.72116591947498));
  lo = spu_madd(x, lo, (vector float)(1.4426898816672));
  lo = spu_mul(x, lo);
  result = spu_madd(x4, hi, lo);

  /* Add the exponent back into the result.
   */
  result = spu_add(result, spu_convtf(exponent, 0));

  
  return (result);
}

