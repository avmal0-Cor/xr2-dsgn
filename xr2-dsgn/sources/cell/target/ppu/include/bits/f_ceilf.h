/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2006 Sony Computer Entertainment Inc.
 * All Rights Reserved.
 */

/* ceilf--  returns smallest integer(as a float) not less than x(round x -> infinity) */ 
_FUNC_DEF(
	float,
	f_ceilf,
	(float x))
{
	int i=(int)x;

	typedef union trunc {
		float f;
		signed int i;
	} trunc;
        trunc xt, yt;
	xt.f=x;
	yt.f=x-(float)i;
        if(!yt.i || (xt.i&0x7f800000) >= 0x4B800000)
        	return x;
        else if(xt.i&0x80000000)
                return (float)i;
        return (float)++i;
}
