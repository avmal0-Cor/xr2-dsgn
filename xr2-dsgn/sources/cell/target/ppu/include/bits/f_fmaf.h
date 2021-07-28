/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2005 Sony Computer Entertainment Inc.
 * All Rights Reserved.
 */

/* f_fmaf function */

_FUNC_DEF(
	float,  
	f_fmaf, 
	(float x, float y, float z))
{
	return (x*y)+z;
}
