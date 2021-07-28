/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2007 Sony Computer Entertainment Inc. 
* All Rights Reserved.
*/
// cmpeqd2 - for each of two double slots, if x == y return mask of ones, else 0

_FUNC_DEF(
   vec_ullong2,
   cmpeqd2,
   ( vec_double2 x, vec_double2 y ))
{
   vec_uchar16 swapEvenOdd = __extension__ (vec_uchar16)(vec_uint4){ 0x04050607, 0x00010203, 0x0c0d0e0f, 0x08090a0b };
   vec_ullong2 sign = spu_splats(0x8000000000000000ull);
   vec_ullong2 cmpeq, bothzero;
   
   bothzero = spu_andc( spu_or( (vec_ullong2)x, (vec_ullong2)y ), sign );
   bothzero = (vec_ullong2)spu_cmpeq( (vec_uint4)bothzero, 0U );
   bothzero = spu_and( bothzero, spu_shuffle( bothzero, bothzero, swapEvenOdd ) );

   cmpeq = (vec_ullong2)spu_cmpeq( (vec_uint4)x, (vec_uint4)y );
   cmpeq = spu_and( cmpeq, spu_shuffle( cmpeq, cmpeq, swapEvenOdd ) );

   return spu_andc( spu_or( cmpeq, bothzero ), spu_or( cmpnand2( x ), cmpnand2( y ) ) );
}

