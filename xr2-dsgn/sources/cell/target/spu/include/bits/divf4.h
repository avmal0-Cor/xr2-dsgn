/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2004 Sony Computer Entertainment Inc. 
* All Rights Reserved.
*/
// divf4 - for each of four float slots, divide numer by denom.

_FUNC_DEF(
   vec_float4, 
   divf4,
   ( vec_float4 numer, vec_float4 denom ))
{
   // Reciprocal estimate and 1 Newton-Raphson iteration.
   // Uses constant of 1.0 + 1 ulp to improve accuracy.

   vec_float4 y0, y0numer;
   vec_float4 oneish = (vec_float4)spu_splats(0x3f800001);

   y0 = spu_re( denom );
   y0numer = spu_mul( numer, y0 );
   return spu_madd( spu_nmsub( denom, y0, oneish ), y0numer, y0numer );
}

