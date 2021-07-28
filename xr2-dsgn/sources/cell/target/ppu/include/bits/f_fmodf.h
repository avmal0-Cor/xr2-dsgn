/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2005 Sony Computer Entertainment Inc.
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
 *	float f_fmodf(float x, float y)
 *
 * DESCRIPTION
 *	The f_fmodf subroutine computes the remainder of
 *	dividing x by y. The return value is x - n*y, where n is
 *	the quotient of x/y, rounded towards zero.
 *
 *	The full range form (default) provides fmod computation on 
 *	all IEEE floating point values (excluding floating overflow
 *	or underflow).
 *
 *	The limited range form (selected by defining FMOD_INTEGER_RANGE)
 *	compute fmod of all floating-point x/y values in the 32-bit
 *	signed integer range. Values outside this range get clamped.
 */

_FUNC_DEF(		
	float,
	f_fmodf,
	(float x, float y))
{
  int exp;
  unsigned int mask, andMask, orMask;
  float abs_y;
  union {
    float f;
    signed int i;
    unsigned int ui;
  } trunc;
  float quotient;

#if defined __GNUC__
  abs_y = __builtin_fabsf(y);
#else
  abs_y = fabsf(y);
#endif
  quotient = trunc.f = x/abs_y;

  /* Construct a mask to remove the fraction bits. The
   * mask depends on the exponent of the biased floating
   * point quotient.
   */
  exp = (trunc.ui >> 23) & 0xFF;
  exp = 127 - exp;

  orMask  = ~((unsigned int)((signed int)(exp-1) >> 31)); 
  andMask = 0x7FFFFF >> (-exp);

  mask  = (exp > -31) ? andMask : 0;
  mask |= orMask;

  trunc.ui &= ~mask;

  return (abs_y*(quotient - trunc.f));

}
