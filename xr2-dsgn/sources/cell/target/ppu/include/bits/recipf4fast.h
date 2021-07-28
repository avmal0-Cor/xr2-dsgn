/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2004 Sony Computer Entertainment Inc. 
* All Rights Reserved.
*/
// recipf4fast - for each of four float slots, compute reciprocal estimate 
// accurate to 12 bits of mantissa.

_FUNC_DEF(
   vec_float4,
   recipf4fast,
   ( vec_float4 x ))
{
   return vec_re( x );
}

