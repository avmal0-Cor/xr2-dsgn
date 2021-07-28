/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 *  Copyright (C) 2005 Sony Computer Entertainment Inc.
 * All Rights Reserved.
 */

/* f_fminf function */

_FUNC_DEF(
	float,
	f_fminf,
	(float x, float y))
{       /* compute larger of x and y */
	if (x < y)
		return (x);
	else
		return (y);
}
