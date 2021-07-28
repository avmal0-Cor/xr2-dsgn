/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2008 Sony Computer Entertainment Inc. 
 * All Rights Reserved.
 */
#define _LOG10F_H_loga2msb  ((float) 0.3010299205780f) 
#define _LOG10F_H_loga2lsb  ((float) 7.5085978266e-8f)
#define _LOG10F_H_logaemsb  ((float) 0.4342944622040f)
#define _LOG10F_H_logaelsb  ((float) 1.9699272335e-8f)
#define _LOG10F_H_neglogae ((float)-0.4342944819033f)

#define _LOG10F_H_c0 ((float)(-0.2988439998f)) 
#define _LOG10F_H_c1 ((float)(-0.3997655209f))
#define _LOG10F_H_c2 ((float)(-0.6666679125f))

_FUNC_DEF(vec_float4, log10f4, (vec_float4 x)) 
{
  vec_float4 vzero = (vec_float4)vec_xor((vec_uint4)(0), (vec_uint4)(0));
  vec_uint4 smask = (vec_uint4)(vec_int4)(-1);
  smask = vec_vslw(smask, smask);
  vec_float4 ones = (vec_float4)(1.0f);
  vec_int4 expmask = (vec_int4)(0x7F800000);
  vec_uint4 iszero = (vec_uint4)vec_cmpeq(x, vzero);
  vec_uint4 isnan = (vec_uint4)vec_cmpeq(x, x);
  vec_uint4 isinf = (vec_uint4)vec_cmpeq((vec_uint4)x, (vec_uint4)expmask);

  vec_int4 xexp = vec_add(vec_sr(vec_and((vec_int4)x, expmask), (vec_uint4)(23)), (vec_int4)(-126));
  vec_float4 xx = vec_sel(x, (vec_float4)((vec_int4)(0x3F000000)), (vec_uint4)expmask);

  vec_uint4 mask = (vec_uint4)vec_cmpgt((vec_float4)((float)0.7071067811865f), xx);
  xx = vec_sel(xx, vec_add(xx, xx), mask);
  xexp = vec_sel(xexp, vec_sub(xexp, (vec_int4)(1)), mask);
  
  vec_float4 x1 = vec_sub(xx, ones);
  vec_float4 z = divf4(x1, vec_add(xx, ones));
  vec_float4 w = vec_madd(z, z, vzero);
  vec_float4 polywneg;
  polywneg = vec_madd((vec_float4)(_LOG10F_H_c0), w, (vec_float4)(_LOG10F_H_c1));
  polywneg = vec_madd(polywneg                  , w, (vec_float4)(_LOG10F_H_c2));
  
  vec_float4 y = vec_madd(z, vec_madd(polywneg, w, x1), vzero);
  vec_float4 wnew = vec_ctf(xexp, 0);
  
  vec_float4 zz1 = vec_madd((vec_float4)(_LOG10F_H_logaemsb), x1, 
			    vec_madd((vec_float4)(_LOG10F_H_loga2msb), wnew, vzero));
  vec_float4 zz2 = vec_madd((vec_float4)(_LOG10F_H_logaelsb), x1,
			    vec_madd((vec_float4)(_LOG10F_H_loga2lsb), wnew, 
				     vec_madd((vec_float4)(_LOG10F_H_neglogae), y, vzero)));
  
  vec_float4 res = vec_sel(vec_add(zz1, zz2), (vec_float4)vec_or((vec_uint4)expmask, smask), iszero);
  res = vec_sel(res, x, isinf);
  return vec_sel(x, res, isnan);
}


