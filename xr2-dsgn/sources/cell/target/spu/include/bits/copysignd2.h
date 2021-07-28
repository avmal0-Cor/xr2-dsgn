/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2004 Sony Computer Entertainment Inc. 
* All Rights Reserved.
*/
// copysignd2 - for each of two double slots, return value with magnitude from x and sign from y.

_FUNC_DEF(
    vec_double2,
    copysignd2,
    ( vec_double2 x, vec_double2 y ))
{
   return spu_sel( x, y, spu_splats(0x8000000000000000ull) );
}

