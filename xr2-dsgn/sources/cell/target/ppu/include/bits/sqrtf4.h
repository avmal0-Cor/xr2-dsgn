/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2007 Sony Computer Entertainment Inc. 
 * All Rights Reserved.
 */
// sqrtf4 - for each of four float slots, compute square root.
// Undefined if input < 0.
      
_FUNC_DEF(
   vec_float4,
   sqrtf4,
   ( vec_float4 x ))
{
   // Reciprocal square root estimate and 1 Newton-Raphson iteration.
 
   vec_float4 zero = (vec_float4)(0.0f);
   vec_float4 half = (vec_float4)(0.5f);
   vec_float4 one = (vec_float4)(1.0f);
   vec_float4 y0, y0x, y0xhalf;
   vec_uint4 cmp_zero;

   y0 = vec_rsqrte( x );
   cmp_zero = (vec_uint4)vec_cmpeq( x, zero );
   cmp_zero = vec_or(cmp_zero, (vec_uint4)vec_cmpeq(y0, zero));
   y0x = vec_madd( y0, x, zero );
   y0xhalf = vec_madd( y0x, half, zero );
   return vec_sel(vec_madd(vec_nmsub(y0, y0x, one), y0xhalf, y0x), x, cmp_zero);
}

