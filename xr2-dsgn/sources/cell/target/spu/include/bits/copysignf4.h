/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2004 Sony Computer Entertainment Inc. 
* All Rights Reserved.
*/
// copysignf4 - for each of four float slots, return value with magnitude from x and sign from y.

_FUNC_DEF(
    vec_float4,
    copysignf4,
    ( vec_float4 x, vec_float4 y ))
{
   return spu_sel( x, y, spu_splats(0x80000000) );
}

