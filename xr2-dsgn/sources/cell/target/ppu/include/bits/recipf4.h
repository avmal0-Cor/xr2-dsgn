/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2008 Sony Computer Entertainment Inc. 
 * All Rights Reserved.
 */
// recipf4 - for each of four float slots, compute reciprocal.
                                                                  
_FUNC_DEF(
   vec_float4,
   recipf4,
   ( vec_float4 x ))
{ 
   // Reciprocal estimate and 1 Newton-Raphson iteration.
 
   vec_float4 y0 = vec_re( x );
   vec_float4 res = vec_madd( vec_nmsub( x, y0, (vec_float4)(1.0f) ), y0, y0 );
   return vec_sel(y0, res, vec_cmpeq(res, res));
}

