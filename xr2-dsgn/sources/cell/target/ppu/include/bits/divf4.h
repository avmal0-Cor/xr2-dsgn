/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2008 Sony Computer Entertainment Inc. 
 * All Rights Reserved.
 */
// divf4 - for each of four float slots, divide numer by denom.

_FUNC_DEF(
   vec_float4, 
   divf4,
   ( vec_float4 numer, vec_float4 denom ))
{  
   // Reciprocal estimate and 1 Newton-Raphson iteration.
   vec_uint4 mask = (vec_uint4)(vec_int4)(-1);
   vec_float4 vnzero = (vec_float4)vec_vslw(mask, mask);
   vec_float4 y0, y0numer;

   y0 = vec_re(denom);
   y0numer = vec_madd(numer, y0, vnzero);
   y0 = vec_madd(vec_nmsub(denom, y0, (vec_float4)(1.0f) ), y0numer, y0numer);

   return vec_sel(y0numer, y0, vec_cmpeq(y0, y0));
}

