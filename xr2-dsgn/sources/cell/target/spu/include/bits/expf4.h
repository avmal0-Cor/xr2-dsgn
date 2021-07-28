/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2004 Sony Computer Entertainment Inc. 
* All Rights Reserved.
*/
#define _EXPF_H_C1     ((float)-0.6931470632553101f)
#define _EXPF_H_C2     ((float)-1.1730463525082e-7f)

#define _EXPF_H_INVLN2 ((float)1.4426950408889634f)

_FUNC_DEF(vec_float4, expf4, (vec_float4 x)) 
{
  vec_uchar16 xnegmask = (vec_uchar16)spu_cmpgt(spu_splats(0.0f), x);
  vec_float4  goffset  = spu_sel(spu_splats((float) 0.5f),spu_splats((float)-0.5f),xnegmask);
  vec_float4 g  = spu_mul(x, spu_splats(_EXPF_H_INVLN2));  
  vec_int4 xexp = spu_convts(spu_add(g, goffset),0);
  
  g = spu_convtf(xexp, 0);
  g = spu_madd(g, spu_splats(_EXPF_H_C2), spu_madd(g, spu_splats(_EXPF_H_C1), x));
  vec_float4 z  = spu_mul(g, g);
  vec_float4 a = spu_mul(z, spu_splats((float)0.0999748594f));
  vec_float4 b = spu_mul(g, 
			 spu_madd(z, 
				  spu_splats((float)0.0083208258f), 
				  spu_splats((float)0.4999999992f)
				  )
			 );
  
  vec_float4 foo  = divf4(spu_add(spu_splats(1.0f), spu_add(a, b)),
			  spu_add(spu_splats(1.0f), spu_sub(a, b)));

  return ldexpf4(foo, xexp);
  
}
