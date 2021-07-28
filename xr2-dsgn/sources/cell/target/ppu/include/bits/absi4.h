/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2004 Sony Computer Entertainment Inc. 
* All Rights Reserved.
*/
// absi4 - for each of four integer slots, compute absolute value.

_FUNC_DEF(
    vec_int4,
    absi4,
    ( vec_int4 x ))
{
   return vec_abs( x );
}

