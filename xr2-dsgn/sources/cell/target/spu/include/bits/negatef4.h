/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2004 Sony Computer Entertainment Inc. 
* All Rights Reserved.
*/
// negatef4 - for each of four float slots, negate the sign bit.

_FUNC_DEF(
   vec_float4, 
   negatef4,
   ( vec_float4 x ))
{
   return (vec_float4)spu_xor( (vec_uint4)x, spu_splats(0x80000000) );
}

