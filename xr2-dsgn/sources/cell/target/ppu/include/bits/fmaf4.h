/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2004 Sony Computer Entertainment Inc. 
* All Rights Reserved.
*/
_FUNC_DEF(vec_float4, 
	  fmaf4,
	  (vec_float4 x, vec_float4 y, vec_float4 z))
{
  return vec_madd(x,y,z);
}
