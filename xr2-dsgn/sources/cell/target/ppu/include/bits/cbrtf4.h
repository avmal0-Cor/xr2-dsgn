/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2008 Sony Computer Entertainment Inc. 
 * All Rights Reserved.
 */
#define __calcQuot(xexp) n = xexp;					\
  vec_uint4 negxexpmask = (vec_uint4)vec_cmpgt((vec_int4)(vzero), n);	\
  n = vec_sel(n, vec_add(n,(vec_int4)(2)), negxexpmask);		\
  									\
  quot = vec_add(vec_sra(n,(vec_uint4)(2)), vec_sra(n,(vec_uint4)(4))); \
  quot = vec_add(quot, vec_sra(quot, (vec_uint4)(4)));			\
  quot = vec_add(quot, vec_sra(quot, (vec_uint4)(8)));			\
  quot = vec_add(quot, vec_sra(quot,(vec_uint4)(16)));			\
  vec_int4 r = vec_sub(vec_sub(n,quot), vec_sl(quot,(vec_uint4)(1)));	\
  quot = vec_add(							\
		 quot,							\
		 vec_sra(						\
			 vec_add(					\
				 vec_add(r,(vec_int4)(5)),		\
				 vec_sl (r,(vec_uint4)(2))		\
				 ),					\
			 (vec_uint4)(4)					\
			 )						\
		 );							\

#define _CBRTF_H_cbrt2 1.2599210498948731648             // 2^(1/3)
#define _CBRTF_H_sqr_cbrt2 1.5874010519681994748         // 2^(2/3)

_FUNC_DEF(vec_float4, cbrtf4, (vec_float4 x)) 
{
  vec_float4 vzero = (vec_float4)vec_xor((vec_uint4)(0), (vec_uint4)(0));
  vec_int4 xexp, n;
  vec_uint4 sgnmask = (vec_uint4)(vec_int4)(-1);
  sgnmask = vec_vslw(sgnmask, sgnmask);
  vec_uint4 iszero = (vec_uint4)vec_cmpeq(x, vzero);
  vec_uint4 isnan = (vec_uint4)vec_cmpeq(x, x);
  vec_uint4 negmask = (vec_uint4)vec_cmpgt(vzero, x);
  vec_float4 xx = vec_andc(x, (vec_float4)sgnmask);

  xx = frexpf4(xx, &xexp);
  vec_float4 p = vec_madd(
			  vec_madd(xx, (vec_float4)(-0.191502161678719066f), (vec_float4)(0.697570460207922770f)),
			  xx,
			  (vec_float4)(0.492659620528969547f)
			  );
  vec_float4 p3 = vec_madd(p, vec_madd(p, p, vzero), vzero);

  vec_int4 quot;
  __calcQuot(xexp);
  vec_int4 modval = vec_sub(vec_sub(xexp,quot), vec_sl(quot,(vec_uint4)(1))); // mod = xexp - 3*quotient
  vec_float4 factor =  (vec_float4)(1.0/_CBRTF_H_sqr_cbrt2);
  factor = vec_sel(factor, (vec_float4)(1.0/_CBRTF_H_cbrt2), vec_cmpeq(modval,(vec_int4)(-1)));
  factor = vec_sel(factor, (vec_float4)(               1.0), vec_cmpeq(modval,(vec_int4)(vzero)));
  factor = vec_sel(factor, (vec_float4)(    _CBRTF_H_cbrt2), vec_cmpeq(modval,(vec_int4)( 1)));
  factor = vec_sel(factor, (vec_float4)(_CBRTF_H_sqr_cbrt2), vec_cmpeq(modval,(vec_int4)( 2)));

  vec_float4 pre  = vec_madd(p, factor, vzero);
  vec_float4 numr = vec_madd(xx , (vec_float4)(2.0f), p3);
  vec_float4 denr = vec_madd(p3, (vec_float4)(2.0f), xx );
  vec_float4 res = vec_madd(pre, divf4(numr, denr), vzero);
  res = ldexpf4(res, quot);

  res = vec_sel(res, vec_or(res,(vec_float4)sgnmask), negmask);
  res = vec_sel(res, x, iszero);
  return vec_sel(x, res, isnan);
}

/*
_FUNC_DEF(vec_float4, cbrtf4, (vec_float4 x))
{
  vec_uint4 neg  = (vec_uint4)vec_cmpgt((vec_float4)(0.0f), x);
  vec_float4  sbit = (vec_float4)(vec_float4)((int)0x80000000);
  vec_float4 absx = vec_andc(x, sbit);
  vec_float4 res = exp2f4(vec_mul((vec_float4)(0.3333333333333f), log2f4(absx)));
  res = vec_sel(res, vec_or(sbit, res), neg);
  return res;
}
*/
