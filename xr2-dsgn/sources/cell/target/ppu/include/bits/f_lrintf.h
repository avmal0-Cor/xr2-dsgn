/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2007 Sony Computer Entertainment Inc.
 * All Rights Reserved.
 */

/* f_lrintf function */


_FUNC_DEF(
	long int,  
	f_lrintf, 
	(float x))
{
        union {
		double d;
	        long int li[2];
	} udl;
#ifdef __SNC__
	udl.d = __builtin_fctid(x);
#else  /* __SNC__ */
        __asm__ volatile ("fctid %0,%1": "=f"(udl.d) : "f"(x));
#endif	/* __SNC__ */
        return udl.li[1];
}
