/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2004 Sony Computer Entertainment Inc. 
 * All Rights Reserved.
 */
_FUNC_DEF(vec_float4, ldexpf4, (vec_float4 x, vec_int4 exp)) 
{
  vec_float4 vzero = (vec_float4)vec_xor((vec_uint4)(0), (vec_uint4)(0));
  vec_uint4 smask = (vec_uint4)(vec_int4)(-1);
  smask = vec_vslw(smask, smask);

  vec_int4 expmask = (vec_int4)(0x7F800000);
  vec_int4 e1 = vec_and((vec_int4)x, expmask);
  vec_int4 e2 = vec_sr(e1,(vec_uint4)(23));

  vec_uint4 maxmask = (vec_uint4)vec_cmpgt(exp, (vec_int4)(255));
  vec_uint4 minmask = (vec_uint4)vec_cmpgt((vec_int4)(-255), exp);
  minmask = vec_or (minmask, (vec_uint4)vec_cmpeq(x, (vec_float4)vzero));

  vec_int4 esum = vec_add(e2, exp);

  maxmask = vec_or (maxmask, (vec_uint4)vec_cmpgt(esum, (vec_int4)(255)));
  maxmask = vec_andc(maxmask, smask);
  minmask = vec_or (minmask, (vec_uint4)vec_cmpgt((vec_int4)vzero, esum));

  x = vec_sel(x, (vec_float4)vec_sl(esum,(vec_uint4)(23)), (vec_uint4)expmask);
  x = vec_sel(x, (vec_float4)vzero, minmask);
  x = vec_sel(x, (vec_float4)maxmask, maxmask);
  return x;
}
