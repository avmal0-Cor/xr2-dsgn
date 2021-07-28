/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2008 Sony Computer Entertainment Inc. 
 * All Rights Reserved.
 */
// negatef4 - for each of four float slots, negate the sign bit.

_FUNC_DEF(
   vec_float4, 
   negatef4,
   ( vec_float4 x ))
{
   vec_uint4 smask = (vec_uint4)(vec_int4)(-1);
   smask = vec_vslw(smask, smask);
   return (vec_float4)vec_xor( (vec_uint4)x, smask );
}

