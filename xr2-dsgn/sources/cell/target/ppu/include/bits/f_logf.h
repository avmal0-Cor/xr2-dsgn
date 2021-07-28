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

#include <bits/fastmath-int.h>

/*
 * FUNCTION
 *	float f_logf(float x)
 *
 * DESCRIPTION
 *	f_logf computes the natural log (base e) of the input value x. ln
 *	is computed using log2 as follows:
 *	
 *	   lnf(x) = log2f(x) / log2f(e);
 */

_FUNC_DEF(
	float,
	f_logf,
	(float x))
{
  return (f_log2f(x) * 0.69314718055995f);
}

