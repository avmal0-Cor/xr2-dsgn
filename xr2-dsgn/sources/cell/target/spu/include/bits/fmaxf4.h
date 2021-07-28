/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2004 Sony Computer Entertainment Inc. 
* All Rights Reserved.
*/
// fmaxf4 - for each of four float slots, compute maximum of x and y
                          
_FUNC_DEF(
   vec_float4,
   fmaxf4,
   ( vec_float4 x, vec_float4 y ))
{
   return spu_sel( x, y, spu_cmpgt( y, x ) );
}

