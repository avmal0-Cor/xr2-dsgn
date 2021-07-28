/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 *  Copyright (C) 2005 Sony Computer Entertainment Inc.
 * All Rights Reserved.
 */

/* f_fdimf function */

_FUNC_DEF(
	float,  
	f_fdimf, 
	(float x, float y))
{
	return y < x ? x - y : 0;
}


