/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2008 Sony Computer Entertainment Inc. 
 * All Rights Reserved.
 */
_FUNC_DEF(vec_float4, logbf4, (vec_float4 x))
{
  vec_float4 vzero = (vec_float4)vec_xor((vec_uint4)(0), (vec_uint4)(0));
  vec_int4 expmask = (vec_int4)(0x7F800000);
  vec_uint4 isnan = (vec_uint4)vec_cmpeq(x, x);
  vec_int4 e1 = vec_and((vec_int4)x, expmask);
  vec_uint4 isinf = (vec_uint4)vec_cmpeq(e1, expmask);
  vec_uint4 zeromask = (vec_uint4)vec_cmpeq(e1, (vec_int4)vzero);
  e1 = vec_sub(e1, (vec_int4)(0x3F800000));
  vec_float4 res = vec_sel(vec_ctf(e1,23), __extension__ ((vec_float4){-HUGE_VALF, -HUGE_VALF, -HUGE_VALF, -HUGE_VALF}), zeromask);
  res = vec_sel(res, (vec_float4)expmask, isinf);
  return vec_sel(x, res, isnan);
}

