/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2007 Sony Computer Entertainment Inc.
 * All Rights Reserved.
 */

/* f_sqrtf function */

_FUNC_DEF(
	float,
	f_sqrtf,
	(float x))
{
	float result;
#ifdef __SNC__
	result = __builtin_fsqrts(x);
#else  /* __SNC__ */
	__asm__ volatile ("fsqrts %0,%1": "=f"(result) : "f"(x));
#endif	/* __SNC__ */
	return result;
}
