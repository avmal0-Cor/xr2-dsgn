/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2004 Sony Computer Entertainment Inc. 
* All Rights Reserved.
*/
_FUNC_DEF(vec_float4, 
	  fdimf4,
	  (vec_float4 x, vec_float4 y))
{
  vec_float4 diff = spu_sub(x,y);
  return spu_sel(spu_splats(0.0f),diff, spu_cmpgt(x,y));
}
