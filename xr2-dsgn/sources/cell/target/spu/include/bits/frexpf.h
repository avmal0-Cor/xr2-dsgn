/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2004 Sony Computer Entertainment Inc. 
* All Rights Reserved.
*/
_FUNC_DEF(float, frexpf, (float y, int *exp)) 
{
  vec_float4 x = spu_promote(y,0);
  vec_int4 zeros = spu_splats((int)0);
  vec_uchar16 zeromask = (vec_uchar16)spu_cmpeq(x, (vec_float4)zeros);
 
  vec_int4 expmask = spu_splats((int)0x7F800000);
  vec_int4 e1 = spu_and ((vec_int4)x, expmask);
  vec_int4 e2 = spu_sub(spu_rlmask(e1,-23), spu_splats((int)126));
  *exp = spu_extract(spu_sel(e2, zeros, zeromask), 0);
 
  vec_int4 m1 = spu_andc((vec_int4)x, expmask);
  vec_int4 m2 = spu_or(m1, spu_splats((int)0x3F000000));
 
  return spu_extract(spu_sel((vec_float4)m2, (vec_float4)zeros, zeromask), 0);

}
