/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2004 Sony Computer Entertainment Inc. 
* All Rights Reserved.
*/
// modff4 - for each of four float slots, compute fractional and integral parts.
// Returns fractional part and stores integral part in *iptr.

_FUNC_DEF(
   vec_float4,
   modff4,
   ( vec_float4 x, vec_float4 *iptr ))
{
   vec_float4 integral, fraction;

   integral = truncf4( x );
   fraction = spu_sub( x, integral );

   *iptr = integral;
   return fraction;
}

