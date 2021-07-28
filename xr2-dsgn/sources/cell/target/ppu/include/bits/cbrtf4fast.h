/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2004 Sony Computer Entertainment Inc. 
* All Rights Reserved.
*/
_FUNC_DEF(vec_float4, cbrtf4fast, (vec_float4 x))
{
  return cbrtf4(x);
}

/*
_FUNC_DEF(vec_float4, cbrtf4fast, (vec_float4 x))
{
  vec_uchar16 neg  = (vec_uchar16)spu_cmpgt(spu_splats(0.0f), x);
  vec_float4 sbit = (vec_float4)spu_splats((int)0x80000000);
  vec_float4 absx = spu_andc(x, sbit);
  vec_float4 res = exp2f4fast(spu_mul(spu_splats((float)0.3333333333333f), log2f4fast(absx)));
  res = spu_sel(res, spu_or(sbit, res), neg);
  return res;
}
*/
