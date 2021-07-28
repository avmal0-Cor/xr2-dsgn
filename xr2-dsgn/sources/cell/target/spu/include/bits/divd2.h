/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2004 Sony Computer Entertainment Inc. 
* All Rights Reserved.
*/
// divd2 - for each of two double slots, divide numer by denom.
//
// Equal to numer * recipd2(denom)
// See recipd2 for results of special values.

_FUNC_DEF(
    vec_double2,
    divd2,
    ( vec_double2 numer, vec_double2 denom ))
{
   return spu_mul( numer, recipd2( denom ) );
}

