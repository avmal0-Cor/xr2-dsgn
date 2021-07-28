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

/*
 * FUNCTION
 *	float _log10f(float x)
 *
 * DESCRIPTION
 *	_log10f computes log (base 10) of the input value x. log10
 *	is computed using log2 as follows:
 *	
 *	   log10f(x) = log2(x) / log2(10);
 */
_FUNC_DEF(vec_float4, log10f4fast, (vec_float4 x)) 
{
  vec_float4 vzero = (vec_float4)vec_xor((vec_uint4)(0), (vec_uint4)(0));
  return vec_madd(log2f4fast(x), (vec_float4)(0.30102999566398f), (vec_float4)vzero);
}

