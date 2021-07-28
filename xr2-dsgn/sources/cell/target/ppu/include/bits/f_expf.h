/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2007 Sony Computer Entertainment Inc.
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

#include <bits/fastmath-int.h>

#ifndef _M_LOG2E
#define _M_LOG2E		1.442695040889f
#endif /* _M_LOG2E */

/*
 * FUNCTION
 *	float f_expf(float x)
 *
 * DESCRIPTION
 *	f_expf computes e raised to the input x. expf is 
 *	computed using exp2f as e^x = 2^(log2(e)*x)
 */

_FUNC_DEF(
	float,
	f_expf,
	(float x))
{
  return (f_exp2f(_M_LOG2E * x));
}



