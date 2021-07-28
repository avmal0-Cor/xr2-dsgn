/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2007 Sony Computer Entertainment Inc.
 * All Rights Reserved.
 */

/* f_nearbyintf function */

_FUNC_DEF(
	float,  
	f_nearbyintf, 
	(float x))
{
	float result;
#ifdef __SNC__
	result = __builtin_fcfid( __builtin_fctid( x ) );
#else  /* __SNC__ */
	__asm__ volatile ("fctid %0,%1\n"
	                  "fcfid %0,%0": "=f"(result) : "f"(x));
#endif /* __SNC__ */
	return result;
}
