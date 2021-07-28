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
 

#define _EXPF4EST_H_M_LOG2E         ((float)1.442695040889f)
 
/*
 * FUNCTION
 *      vector float _expf_v(vector float x)
 *
 * DESCRIPTION
 *      _expf_v computes e raised to the input vector x. expf_v is
 *      computed using exp2f_v as e^x = 2^(log2(e)*x)
 */
 
_FUNC_DEF(vec_float4, expf4fast, (vec_float4 x))
{
  return (exp2f4fast(spu_mul(spu_splats(_EXPF4EST_H_M_LOG2E), x)));
}
