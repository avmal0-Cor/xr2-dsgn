/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2004 Sony Computer Entertainment Inc. 
* All Rights Reserved.
*/
_FUNC_DEF(vec_float4, 
	  hypotf4,
	  (vec_float4 x, vec_float4 y))
{
  vec_float4 sum = spu_mul(x,x);
  sum = spu_madd(y,y,sum);
  return sqrtf4(sum);
}
