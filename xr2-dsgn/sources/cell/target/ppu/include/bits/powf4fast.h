/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2004 Sony Computer Entertainment Inc. 
* All Rights Reserved.
*/
_FUNC_DEF(vec_float4, powf4fast, (vec_float4 x, vec_float4 y))
{
  vec_int4 zeros = (vec_int4)(0);
  vec_uint4 zeromask = (vec_uint4)vec_cmpeq((vec_float4)zeros, x);

  vec_uint4 negmask  = (vec_uint4)vec_cmpgt((vec_float4)zeros, x);
  
  vec_float4 sbit = (vec_float4)((vec_int4)(0x80000000));
  vec_float4 absx = vec_andc(x, sbit);
  vec_float4 absy = vec_andc(y, sbit);
  vec_uint4 oddy = vec_and(vec_ctu(absy, 0), (vec_uint4)(0x00000001));
  negmask = vec_and(negmask, (vec_uint4)vec_cmpgt(oddy, (vec_uint4)zeros));

  vec_float4 res = exp2f4fast(vec_madd(y, log2f4(absx), (vec_float4)zeros));
  res = vec_sel(res, vec_or(sbit, res), negmask);


  return vec_sel(res, (vec_float4)zeros, zeromask);
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
