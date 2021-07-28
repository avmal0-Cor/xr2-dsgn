/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2008 Sony Computer Entertainment Inc. 
 * All Rights Reserved.
 */
_FUNC_DEF(vec_float4, 
	  hypotf4,
	  (vec_float4 x, vec_float4 y))
{
  vec_float4 vzero = (vec_float4)vec_xor((vec_uint4)(0), (vec_uint4)(0));
  vec_float4 sum = vec_madd(x,x,vzero);
  sum = vec_madd(y,y,sum);
  return sqrtf4(sum);
}
