/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2004 Sony Computer Entertainment Inc. 
* All Rights Reserved.
*/
// ceilf4 - for each of four float slots, round up to smallest integer not less than the value.

_FUNC_DEF(
    vec_float4,
    ceilf4,
    ( vec_float4 x ))
{
   return vec_ceil( x );
}

