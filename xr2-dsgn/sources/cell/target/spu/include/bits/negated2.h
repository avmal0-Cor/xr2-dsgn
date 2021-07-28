/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2004 Sony Computer Entertainment Inc. 
* All Rights Reserved.
*/
// negated2 - for each of two double slots, negate the sign bit.

_FUNC_DEF(
    vec_double2,
    negated2,
    ( vec_double2 x ))
{
   return (vec_double2)spu_xor( (vec_ullong2)x, spu_splats(0x8000000000000000ull) );
}

