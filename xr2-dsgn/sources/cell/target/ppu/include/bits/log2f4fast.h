/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2004 Sony Computer Entertainment Inc. 
* All Rights Reserved.
*/
_FUNC_DEF(vec_float4, log2f4fast, (vec_float4 x))
{
  return vec_loge(x);
}

/*
{
    return ((vec_float4)(0.0f));
}
*/
