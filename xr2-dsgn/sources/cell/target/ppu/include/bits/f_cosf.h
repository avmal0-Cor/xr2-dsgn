/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2007 Sony Computer Entertainment Inc.
 * All Rights Reserved.
 */

/* f_cosf function */

#include "bits/sincos_c.h"

_FUNC_DEF(
	float,
	f_cosf,
	(float x))
{
	float y, z;
	int n;
	
	typedef union trunc {
		float f;
	  	signed int i;
	 } trunc;
	 trunc xt;
	 xt.f = x;
	 
	if (xt.i < 0)
	{
	    return f_cosf(-x);
	}

	/* range reduction */
	
    	y = x * (float)(4.0 / _M_PI);
	n = (int)y;
    	y = y - (float)n;
            
	if (n & 1)
	{
	    	y = 1.0f - y;
	}
	if ((n+1)&2)
	{
	    	z = _SPOLY(y);
	}
	else
	{
		z = _CPOLY(y);
	}
	if (((n+2) & 7) > 3)
	{
	    	z = -z;
	}

	return z;
}
