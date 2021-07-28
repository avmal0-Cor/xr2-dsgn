/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2008 Sony Computer Entertainment Inc. 
 * All Rights Reserved.
 */
_FUNC_DEF(vec_int4, ilogbf4, (vec_float4 x))
{
  vec_float4 vzero = (vec_float4)vec_xor((vec_uint4)(0), (vec_uint4)(0));
  vec_int4 minus127 = (vec_int4)(-127);

  vec_int4 e1 = vec_and((vec_int4)x, (vec_int4)(0x7F800000));
  vec_uint4 zeromask = (vec_uint4)vec_cmpeq(e1, (vec_int4)vzero);
  vec_int4 e2 = vec_add(vec_sr(e1,(vec_uint4)(23)), minus127);
  
  return vec_sel(e2, (vec_int4)(FP_ILOGB0), zeromask);

}
