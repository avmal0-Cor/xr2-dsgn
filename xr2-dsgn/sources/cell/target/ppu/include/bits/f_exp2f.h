/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * * Copyright (C) 2007 Sony Computer Entertainment Inc.
 * * All Rights Reserved.
 * */

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

#ifndef _M_LN2
#define _M_LN2	0.69314718055995f	/* ln(2) */
#endif /* _M_LN2 */

/*
 * FUNCTION
 *	float _exp2f(float x)
 *
 * DESCRIPTION
 *	_exp2f computes 2 raised to the input x. Computation is 
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

#ifdef __SPU__
#define _MAX_RESULT	0x7FFFFFFF
#else
#define _MAX_RESULT	0x7F800000
#endif

_FUNC_DEF(
	float,
	f_exp2f,
	(float x))
{
  union {
    float f;
    unsigned int ui;
  } bias, exp_int, exp_frac;
  int ix;
  float frac, frac2, frac4;
  float hi, lo;

  /* Break in the input x into two parts ceil(x), x - ceil(x).
   */
  bias.f = x;
  bias.ui = ~(unsigned int)((signed)(bias.ui) >> 31) & 0x3F7FFFFF;
  ix = (int)(x + bias.f);
  frac = (float)ix - x;
  frac *= _M_LN2;

  exp_int.ui  = (ix + 127) << 23;

  /* Instruction counts can be reduced if the polynomial was
   * computed entirely from nested (dependent) fma's. However, 
   * to reduce the number of pipeline stalls, the polygon is evaluated 
   * in two halves (hi amd lo). 
   */
  frac2 = frac  * frac;
  frac4 = frac2 * frac2;
  hi = -0.0001413161f * frac + 0.0013298820f;
  hi =             hi * frac - 0.0083013598f;
  hi =             hi * frac + 0.0416573475f;
  lo = -0.1666653019f * frac + 0.4999999206f;
  lo =             lo * frac - 0.9999999995f;
  lo =             lo * frac + 1.0f;
  exp_frac.f =     hi * frac4 + lo;

  ix += exp_frac.ui >> 23;
  exp_frac.f *= exp_int.f;

  /* Handle overflow */
  if ((unsigned int)(ix) > 255) {
    exp_frac.ui = (ix < 0) ? 0x0 : _MAX_RESULT;
  }

  return (exp_frac.f);
}



