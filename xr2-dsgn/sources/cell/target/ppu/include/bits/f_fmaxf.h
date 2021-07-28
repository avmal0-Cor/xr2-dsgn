/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 *  Copyright (C) 2005 Sony Computer Entertainment Inc.
 * All Rights Reserved.
 */

/* f_fmaxf function */

_FUNC_DEF(
	float,
	f_fmaxf,
	(float x, float y))
{       /* compute larger of x and y */
	if (x < y)
		return (y);
	else
		return x;
}

