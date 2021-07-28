/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2008 Sony Computer Entertainment Inc. 
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
 
   vec_float4 vzero = (vec_float4)vec_xor((vec_uint4)(0), (vec_uint4)(0));
   vec_float4 half = (vec_float4)(0.5f);
   vec_float4 one = (vec_float4)(1.0f);
   vec_float4 y0, y0x, y0half;

   y0 = vec_rsqrte( x );
   y0x = vec_madd( y0, x, vzero );
   y0half = vec_madd( y0, half, vzero );
   y0x = vec_madd( vec_nmsub( y0, y0x, one ), y0half, y0 );
   return vec_sel(y0, y0x, vec_cmpeq(y0x, y0x));
}

