/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2004 Sony Computer Entertainment Inc. 
* All Rights Reserved.
*/
// divf4fast - for each of four float slots, compute estimate of numer/denom as
// product of numer and 12-bit-accurate reciprocal estimate of denom.

_FUNC_DEF(
   vec_float4, 
   divf4fast,
   ( vec_float4 numer, vec_float4 denom ))
{
   return spu_mul( numer, spu_re( denom ) );
}

