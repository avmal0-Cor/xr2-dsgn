/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2008 Sony Computer Entertainment Inc. 
 * All Rights Reserved.
 */
_FUNC_DEF(vec_float4, 
	  fdimf4,
	  (vec_float4 x, vec_float4 y))
{
  vec_float4 vzero = (vec_float4)vec_xor((vec_uint4)(0), (vec_uint4)(0));
  vec_float4 diff = vec_sub(x,y);
  return vec_sel(vzero, diff, vec_cmpgt(x,y));
}
