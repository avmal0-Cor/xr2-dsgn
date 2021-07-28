/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2004 Sony Computer Entertainment Inc. 
* All Rights Reserved.
*/
_FUNC_DEF(vec_float4, expm1f4fast, (vec_float4 x))
{
  vec_float4 x2 = spu_mul(x,x);
  vec_float4 d0, d1, n0, n1;
  
  d0 = spu_madd(x , spu_splats((float)-0.3203561199f), spu_splats((float)0.9483177697f));
  d1 = spu_madd(x2, spu_splats((float) 0.0326527809f), d0);
  
  n0 = spu_madd(x , spu_splats((float)0.1538026623f), spu_splats((float)0.9483177732f));
  n1 = spu_madd(x , spu_splats((float)0.0024490478f), spu_splats((float)0.0305274668f));
  n1 = spu_madd(x2, n1, n0);
 
  return spu_mul(x, divf4(n1, d1));

}
