/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2004 Sony Computer Entertainment Inc. 
* All Rights Reserved.
*/
_FUNC_DEF(vec_int4, ilogbf4, (vec_float4 x))
{
  vec_int4 minus127 = spu_splats((int)-127);

  vec_int4 e1 = spu_and((vec_int4)x, spu_splats((int)0x7F800000));
  vec_uchar16 zeromask = (vec_uchar16)spu_cmpeq(e1, 0);
  vec_int4 e2 = spu_add(spu_rlmask(e1,-23), minus127);
  
  return spu_sel(e2, (vec_int4)spu_splats(FP_ILOGB0), zeromask);

}
