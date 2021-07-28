/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2006 Sony Computer Entertainment Inc.
 * All Rights Reserved.
 */

/* copysignf function */

_FUNC_DEF(
	float,
	f_copysignf,
	(float x, float y))
{	/* copy sign from y to x */
	  union U {
	    unsigned int s;
	    float f;
	  } ux, uy;
	  ux.f = x;
	  uy.f = y;

	ux.s = (ux.s & ~0x80000000) | (uy.s & 0x80000000);
	return (ux.f);
}

