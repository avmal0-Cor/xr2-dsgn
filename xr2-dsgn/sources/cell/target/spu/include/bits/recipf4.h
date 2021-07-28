/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2004 Sony Computer Entertainment Inc. 
* All Rights Reserved.
*/
// recipf4 - for each of four float slots, compute reciprocal.
                                                                  
_FUNC_DEF(
   vec_float4,
   recipf4,
   ( vec_float4 x ))
{
   // Reciprocal estimate and 1 Newton-Raphson iteration.
   // A constant of 1.0 + 1 ulp in the Newton-Raphson step results in exact 
   // answers for powers of 2, and a slightly smaller relative error bound.

   vec_float4 y0;
   vec_float4 oneish = (vec_float4)spu_splats(0x3f800001);

   y0 = spu_re( x );
   return spu_madd( spu_nmsub( x, y0, oneish ), y0, y0 );
}

