/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2008 Sony Computer Entertainment Inc. 
 * All Rights Reserved.
 */
#define _EXPF_H_C1     ((float)-0.6931470632553101f)
#define _EXPF_H_C2     ((float)-1.1730463525082e-7f)

#define _EXPF_H_INVLN2 ((float)1.4426950408889634f)

_FUNC_DEF(vec_float4, expf4, (vec_float4 x)) 
{
  vec_float4 vzero = (vec_float4)vec_xor((vec_uint4)(0), (vec_uint4)(0));
  vec_uint4 xnegmask = (vec_uint4)vec_cmpgt(vzero, x);
  vec_float4 goffset  = vec_sel((vec_float4)( 0.5f),(vec_float4)(-0.5f),xnegmask);
  vec_float4 g  = vec_madd(x, (vec_float4)(_EXPF_H_INVLN2), vzero);  
  vec_int4 xexp = vec_cts(vec_add(g, goffset),0);
  
  g = vec_ctf(xexp, 0);
  g = vec_madd(g, (vec_float4)(_EXPF_H_C2), vec_madd(g, (vec_float4)(_EXPF_H_C1), x));
  vec_float4 z  = vec_madd(g, g, vzero);
  vec_float4 a = vec_madd(z, (vec_float4)(0.0999748594f), vzero);
  vec_float4 b = vec_madd(g, 
			  vec_madd(z, 
				   (vec_float4)(0.0083208258f), 
				   (vec_float4)(0.4999999992f)
				   ),
			  vzero);
  
  vec_float4 foo  = divf4(vec_add((vec_float4)(1.0f), vec_add(a, b)),
			  vec_add((vec_float4)(1.0f), vec_sub(a, b)));

  return ldexpf4(foo, xexp);
  
}
