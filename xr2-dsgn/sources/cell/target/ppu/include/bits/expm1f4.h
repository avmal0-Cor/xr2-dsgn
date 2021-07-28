/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2008 Sony Computer Entertainment Inc. 
 * All Rights Reserved.
 */
#define _EXPM1F_H_ln1by2 ((float)-0.6931471805599f)
#define _EXPM1F_H_ln3by2 ((float) 0.4054651081082f)

_FUNC_DEF(vec_float4, expm1f4, (vec_float4 x)) 
{
  vec_float4 vzero = (vec_float4)vec_xor((vec_uint4)(0), (vec_uint4)(0));
  vec_uint4 nearzeromask = (vec_uint4)vec_and(vec_cmpgt(x, (vec_float4)(_EXPM1F_H_ln1by2)),
					      vec_cmpgt((vec_float4)(_EXPM1F_H_ln3by2), x));
  vec_float4 x2 = vec_madd(x,x,vzero);
  vec_float4 d0, d1, n0, n1;

  d0 = vec_madd(x , (vec_float4)(-0.3203561199f), (vec_float4)(0.9483177697f));
  d1 = vec_madd(x2, (vec_float4)( 0.0326527809f), d0);
  
  n0 = vec_madd(x , (vec_float4)(0.1538026623f), (vec_float4)(0.9483177732f));
  n1 = vec_madd(x , (vec_float4)(0.0024490478f), (vec_float4)(0.0305274668f));
  n1 = vec_madd(x2, n1, n0);
 
  return vec_sel(vec_sub(expf4(x), (vec_float4)(1.0f)),
                 vec_madd(x, divf4(n1, d1), vzero),
                 nearzeromask);
}
