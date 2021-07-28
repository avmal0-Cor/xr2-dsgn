/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2004 Sony Computer Entertainment Inc. 
* All Rights Reserved.
*/
_FUNC_DEF(vec_float4, logbf4, (vec_float4 x))
{
  vec_int4 e1 = spu_and((vec_int4)x, spu_splats((int)0x7F800000));
  vec_uchar16 zeromask = (vec_uchar16)spu_cmpeq(e1, 0);
  e1 = spu_sub(e1, spu_splats((int)0x3F800000));
  return spu_sel(spu_convtf(e1,23), (vec_float4)spu_splats(-HUGE_VALF), zeromask);
}

