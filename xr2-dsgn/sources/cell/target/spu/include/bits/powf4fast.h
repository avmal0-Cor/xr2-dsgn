/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2004 Sony Computer Entertainment Inc. 
* All Rights Reserved.
*/
_FUNC_DEF(vec_float4, powf4fast, (vec_float4 x, vec_float4 y))
{

  vec_int4 zeros = spu_splats(0);
  vec_uchar16 zeromask = (vec_uchar16)spu_cmpeq((vec_float4)zeros, x);
  vec_uchar16 neg  = (vec_uchar16)spu_cmpgt(spu_splats(0.0f), x);
  
  vec_float4 sbit = (vec_float4)spu_splats((int)0x80000000);
  vec_float4 absx = spu_andc(x, sbit);
  vec_float4 absy = spu_andc(y, sbit);
  vec_uint4 inty = spu_and(spu_convtu(absy, 0), (vec_uint4)spu_splats(0x00000001));
  neg = spu_and(neg, (vec_uchar16)spu_cmpgt(inty, (vec_uint4)zeros));

  vec_float4 res = exp2f4fast(spu_mul(y, log2f4fast(absx)));
  res = spu_sel(res, spu_or(sbit, res), neg);

  return spu_sel(res, (vec_float4)zeros, zeromask);
}
