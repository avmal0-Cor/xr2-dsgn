/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2008 Sony Computer Entertainment Inc. 
 * All Rights Reserved.
 */
_FUNC_DEF(vec_float4, frexpf4, (vec_float4 x, vec_int4 *exp))
{
  vec_float4 vzero = (vec_float4)vec_xor((vec_uint4)(0), (vec_uint4)(0));
  vec_uint4 zeromask = (vec_uint4)vec_cmpeq(x, (vec_float4)vzero);
 
  vec_int4 expmask = (vec_int4)(0x7F800000);
  vec_int4 e1 = vec_and ( (vec_int4)x, expmask);
  vec_int4 e2 = vec_sub(vec_sr(e1, (vec_uint4)(23) ), (vec_int4)(126) );
  *exp = vec_sel(e2, (vec_int4)vzero, zeromask);
 
  vec_float4 m2 = vec_sel(x, (vec_float4)((vec_int4)(0x3F000000)), (vec_uint4)expmask);

  return vec_sel(m2, (vec_float4)vzero, zeromask);
}


/*
{
  *exp = ((vec_int4)(0));
  return ((vec_float4)(0.0f));
}
*/
