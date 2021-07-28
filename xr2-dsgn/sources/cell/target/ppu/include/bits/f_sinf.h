/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2007 Sony Computer Entertainment Inc.
 * All Rights Reserved.
 */

/* f_sinf function */

#include "bits/sincos_c.h"

_FUNC_DEF(
	float,
	f_sinf,
	(float x))
{
	float y, z;
	int n;
	
	typedef union trunc {
		float f;
		signed int i;
	} trunc;
	trunc xt;
	xt.f=x;
	if (xt.i < 0)
	{
	    return -f_sinf(-x);
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
	    z = _CPOLY(y);
	}
	else
	{
		z = _SPOLY(y);
	}
	if ((n & 7) > 3)
	{
	    z = -z;
	}
	return z;
	
}

