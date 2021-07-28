/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2008 Sony Computer Entertainment Inc. 
 * All Rights Reserved.
 */
_FUNC_DEF(vec_float4, log1pf4fast, (vec_float4 x))
{
  vec_float4 vzero = (vec_float4)vec_xor((vec_uint4)(0), (vec_uint4)(0));
  vec_float4 x2 = vec_madd(x,x,vzero);
  vec_float4 d0, d1, n0, n1;

  d0 = vec_madd(x , (vec_float4)(1.5934420741f), (vec_float4)(0.8952856868f));
  d1 = vec_madd(x , (vec_float4)(0.1198195734f), (vec_float4)(0.8377145063f));
  d1 = vec_madd(x2, d1, d0);
  
  n0 = vec_madd(x , (vec_float4)(1.1457993413f), (vec_float4)(0.8952856678f));
  n1 = vec_madd(x , (vec_float4)(0.0082862580f), (vec_float4)(0.3394238808f));
  n1 = vec_madd(x2, n1, n0);

  return vec_madd(x, divf4(n1, d1), vzero);
}
