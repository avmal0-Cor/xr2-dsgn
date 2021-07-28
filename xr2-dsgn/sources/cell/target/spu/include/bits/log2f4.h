/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2004 Sony Computer Entertainment Inc. 
* All Rights Reserved.
*/
#define _LOG2F_H_l2emsb ((float)1.4426950216293f) 
#define _LOG2F_H_l2elsb ((float)1.9259629911e-8f) 
#define _LOG2F_H_l2e   ((float)1.4426950408890f) 

#define _LOG2F_H_c0 ((float)(0.2988439998f)) 
#define _LOG2F_H_c1 ((float)(0.3997655209f))
#define _LOG2F_H_c2 ((float)(0.6666679125f))

_FUNC_DEF(vec_float4, log2f4, (vec_float4 x))
{
  vec_int4 zeros = spu_splats((int)0);
  vec_float4 ones = spu_splats(1.0f);
  vec_uchar16 zeromask = (vec_uchar16)spu_cmpeq(x, (vec_float4)zeros);

  vec_int4 expmask = spu_splats((int)0x7F800000);
  vec_int4 xexp = spu_add( spu_rlmask(spu_and((vec_int4)x, expmask), -23), -126 );
  x = spu_sel(x, (vec_float4)spu_splats((int)0x3F000000), (vec_uchar16)expmask);


  vec_uint4  mask = spu_cmpgt(spu_splats((float)0.7071067811865f), x);
  x    = spu_sel(x   , spu_add(x, x)                   , mask);
  xexp = spu_sel(xexp, spu_sub(xexp,spu_splats((int)1)), mask);
  
  vec_float4 x1 = spu_sub(x , ones);
  vec_float4 z  = divf4(x1, spu_add(x, ones));
  vec_float4 w  = spu_mul(z , z);
  vec_float4 polyw;
  polyw = spu_madd(spu_splats(_LOG2F_H_c0), w, spu_splats(_LOG2F_H_c1));
  polyw = spu_madd(polyw                  , w, spu_splats(_LOG2F_H_c2));
  
  vec_float4 yneg = spu_mul(z, spu_msub(polyw, w, x1));
  vec_float4 zz1 = spu_madd(spu_splats(_LOG2F_H_l2emsb), x1, spu_convtf(xexp,0));
  vec_float4 zz2 = spu_madd(spu_splats(_LOG2F_H_l2elsb), x1,
			    spu_mul(spu_splats(_LOG2F_H_l2e), yneg)
			    );
  
  return spu_sel(spu_add(zz1,zz2), (vec_float4)zeromask, zeromask);
}
