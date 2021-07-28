/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2004 Sony Computer Entertainment Inc. 
* All Rights Reserved.
*/
_FUNC_DEF(vec_float4, ldexpf4, (vec_float4 x, vec_int4 exp)) 
{
  vec_int4 zeros = spu_splats(0);

  vec_uchar16 expmask = (vec_uchar16)spu_splats((int)0x7F800000);
  vec_int4 e1 = spu_and((vec_int4)x, (vec_int4)expmask);
  vec_int4 e2 = spu_rlmask(e1,-23);

  vec_uchar16 maxmask = (vec_uchar16)spu_cmpgt(exp, 255);
  vec_uchar16 minmask = (vec_uchar16)spu_cmpgt(spu_splats(-255), exp);
  minmask = spu_or (minmask, (vec_uchar16)spu_cmpeq(x, (vec_float4)zeros));

  vec_int4 esum = spu_add(e2, exp);

  maxmask = spu_or (maxmask, (vec_uchar16)spu_cmpgt(esum, 255));
  maxmask = spu_and(maxmask, (vec_uchar16)spu_splats((int)0x7FFFFFFF));
  minmask = spu_or (minmask, (vec_uchar16)spu_cmpgt(zeros, esum));

  x = spu_sel(x, (vec_float4)spu_sl(esum,23), expmask);
  x = spu_sel(x, (vec_float4)zeros, minmask);
  //x = spu_sel(x, (vec_float4)spu_splats((int)0xFFFFFFFF), maxmask);
  x = spu_sel(x, (vec_float4)maxmask, maxmask);
  return x;
}
