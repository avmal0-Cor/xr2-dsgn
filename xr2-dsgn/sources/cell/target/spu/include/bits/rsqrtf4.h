/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2004 Sony Computer Entertainment Inc. 
* All Rights Reserved.
*/
// rsqrtf4 - for each of four float slots, compute reciprocal square root.
// Undefined if input < 0.
      
_FUNC_DEF(
   vec_float4,
   rsqrtf4,
   ( vec_float4 x ))
{
   // Reciprocal square root estimate and 1 Newton-Raphson iteration.   

   vec_float4 y0, y0x, y0half;
   vec_float4 oneish = (vec_float4)spu_splats(0x3f800001);

   y0 = spu_rsqrte( x );
   y0x = spu_mul( y0, x );
   y0half = spu_mul( y0, spu_splats(0.5f) );
   return spu_madd( spu_nmsub( y0, y0x, oneish ), y0half, y0 );
}

