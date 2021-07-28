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
 *	float _lnf(float x)
 *
 * DESCRIPTION
 *	_lnf computes the natural log (base e) of the input value x. ln
 *	is computed using log2 as follows:
 *	
 *	   lnf(x) = log2f(x) / log2f(e);
 */

_FUNC_DEF(vec_float4, logf4fast, (vec_float4 x))
{
  return spu_mul(log2f4fast(x), spu_splats(0.69314718055995f));
}
