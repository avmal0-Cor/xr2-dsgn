/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2004 Sony Computer Entertainment Inc. 
* All Rights Reserved.
*/
_FUNC_DEF(vec_float4, log1pf4, (vec_float4 x))
{
  vec_uchar16 nearzeromask = (vec_uchar16)spu_and(spu_cmpgt(x, spu_splats(-0.5f)),
                                                  spu_cmpgt(spu_splats(0.5f), x));
  vec_float4 x2 = spu_mul(x,x);
  vec_float4 d0, d1, n0, n1;

  d0 = spu_madd(x , spu_splats((float)1.5934420741f), spu_splats((float)0.8952856868f));
  d1 = spu_madd(x , spu_splats((float)0.1198195734f), spu_splats((float)0.8377145063f));
  d1 = spu_madd(x2, d1, d0);
  
  n0 = spu_madd(x , spu_splats((float)1.1457993413f), spu_splats((float)0.8952856678f));
  n1 = spu_madd(x , spu_splats((float)0.0082862580f), spu_splats((float)0.3394238808f));
  n1 = spu_madd(x2, n1, n0);
 
  return spu_sel(logf4(spu_add(x, spu_splats(1.0f))),
                 spu_mul(x, divf4(n1, d1)),
                 nearzeromask);
}
