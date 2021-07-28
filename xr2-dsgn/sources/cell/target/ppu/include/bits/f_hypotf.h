/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2006 Sony Computer Entertainment Inc.
 * All Rights Reserved.
 */

/* f_hypotf function */

_FUNC_DEF(
	float,
	f_hypotf,
	(float x, float y))
{
	double result, input;
	double x1 = x;
	double y1 = y;
	input = x1*x1 + y1*y1;
#ifdef __SNC__
  	result = __builtin_fsqrt(input);
#else  /* __SNC_ */
	__asm__ volatile ("fsqrt %0,%1": "=f"(result) : "f"(input));
#endif	/* __SNC__ */
	return (float)result;
}

