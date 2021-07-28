/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2004 Sony Computer Entertainment Inc. 
* All Rights Reserved.
*/
// floorf4 - for each of four float slots, round down to largest integer not greater than the value.

_FUNC_DEF(
   vec_float4,
   floorf4,
   ( vec_float4 x ))
{
   return vec_floor( x );
}

