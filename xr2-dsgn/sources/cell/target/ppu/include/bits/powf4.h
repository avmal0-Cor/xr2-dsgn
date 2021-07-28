/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2008 Sony Computer Entertainment Inc. 
 * All Rights Reserved.
 */
_FUNC_DEF(vec_float4, powf4, (vec_float4 x, vec_float4 y))
{
  vec_float4 vzero = (vec_float4)vec_xor((vec_uint4)(0),(vec_uint4)(0));
//  vec_uint4 xzero = (vec_uint4)vec_cmpeq((vec_float4)vzero, x);
  vec_uint4 yzero = (vec_uint4)vec_cmpeq((vec_float4)vzero, y);
//  vec_uint4 negy = (vec_uint4)vec_cmpgt((vec_float4)vzero, y);
  vec_uint4 negx  = (vec_uint4)vec_cmpgt((vec_int4)vzero, (vec_int4)x);

  vec_uint4 sbit = (vec_uint4)(vec_int4)(-1);
  sbit = vec_vslw(sbit, sbit);
  vec_float4 absx = vec_andc(x, (vec_float4)sbit);
  vec_float4 absy = vec_andc(y, (vec_float4)sbit);
  vec_uint4 oddy = vec_and(vec_ctu(absy, 0), (vec_uint4)(0x00000001));
  negx = vec_and(negx, (vec_uint4)vec_cmpgt(oddy, (vec_uint4)vzero));

  vec_float4 res = log2f4(absx);
  res = vec_sel(res, y, yzero);
  vec_float4 res1 = vec_madd(y, res, (vec_float4)vzero);
  res1 = exp2f4(vec_sel(res1, res, vec_and(vec_cmpeq(res, vzero),
                        (vec_uint4)vec_cmpgt((vec_int4)x,(vec_int4)vzero))));
  res1 = vec_sel(res1, vec_or((vec_float4)sbit, res1), negx);
  res1 = vec_sel(x, res1, vec_cmpeq(res, res));
  return res1;
}

/*
{
  vec_int4 zeros = vec_splats(0);
  vec_int4 ones  = (vec_int4)vec_splats((char)0xFF);
  vec_uint4 zeromask = (vec_uint4)vec_cmpeq((vec_float4)zeros, x);
  vec_uint4 onemask  = (vec_uint4)vec_cmpeq((vec_float4)ones , y);
  vec_uint4 negmask  = (vec_uint4)vec_cmpgt(vec_splats(0.0f),  x);
  vec_float4 sbit = (vec_float4)(vec_int4)(0x80000000);
  vec_float4 absx = vec_andc(x, sbit);
  vec_float4 absy = vec_andc(y, sbit);
  vec_uint4  oddy = vec_and(vec_convtu(absy, 0), (vec_uint4)vec_splats(0x00000001));
  negmask         = vec_and(negmask, (vec_uint4)vec_cmpgt(oddy, (vec_uint4)zeros));

  

}

*/
