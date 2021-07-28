/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2004 Sony Computer Entertainment Inc. 
* All Rights Reserved.
*/
// fabsf4 - for each of four float slots, compute absolute value.

_FUNC_DEF(
   vec_float4, 
   fabsf4,
   ( vec_float4 x ))
{
   return vec_abs( x );
}

