/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2004 Sony Computer Entertainment Inc. 
* All Rights Reserved.
*/
// sqrtf4fast - for each of four float slots, compute square root estimate 
// accurate to 12 bits of mantissa.
      
_FUNC_DEF(
   vec_float4,
   sqrtf4fast,
   ( vec_float4 x ))
{
   vec_float4 zero = (vec_float4)(0.0f);
   return vec_sel( vec_madd( x, vec_rsqrte( x ), zero ), zero, vec_cmpeq( x, zero ) );
}

