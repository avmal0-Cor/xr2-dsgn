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
 *	float _log2f(float x)
 *
 * DESCRIPTION
 *	_log2f computes log (base 2) of the input value x. The log2f
 *	function is approximated as a polynomial of order 8
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

_FUNC_DEF(
	float,
	f_log2f,
	(float x))
{
  union {
    unsigned int ui;
    float f;
  } in;
  int exponent;
  float result;
  float x2, x4;
  float hi, lo;

  in.f = x;

  /* Extract the exponent from the input X. 
   */
  exponent = (signed)((in.ui >> 23) & 0xFF) - 127;

  /* Compute the remainder after removing the exponent.
   */
  in.ui -= exponent << 23;
  
  /* Calculate the log2 of the remainder using the polynomial
   * approximation.
   */
  x = in.f - 1.0f;

  /* Instruction counts can be reduced if the polynomial was
   * computed entirely from nested (dependent) fma's. However, 
   * to reduce the number of pipeline stalls, the polygon is evaluated 
   * in two halves (hi amd lo). 
   */
  x2 = x * x;
  x4 = x2 * x2;
  hi = -0.0093104962134977f*x + 0.052064690894143f;
  hi =                   hi*x - 0.13753123777116f;
  hi =                   hi*x + 0.24187369696082f;
  hi =                   hi*x - 0.34730547155299f;
  lo =  0.47868480909345f  *x - 0.72116591947498f;
  lo =                   lo*x + 1.4426898816672f;
  lo =                   lo*x;
  result = hi*x4 + lo;

  /* Add the exponent back into the result.
   */
  result += (float)(exponent);

  return (result);
}

