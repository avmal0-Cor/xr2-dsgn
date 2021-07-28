/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2004 Sony Computer Entertainment Inc. 
* All Rights Reserved.
*/
// fminf4 - for each of four float slots, compute minimum of x and y
         
_FUNC_DEF(
   vec_float4,
   fminf4,
   ( vec_float4 x, vec_float4 y ))
{
   return vec_min( x, y );
}

