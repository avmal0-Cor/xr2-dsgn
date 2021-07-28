/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2007 Sony Computer Entertainment Inc. 
* All Rights Reserved.
*/
// cmpnand2 - for each of two double slots, if input is any type of NaN return mask of ones, else 0

_FUNC_DEF(
   vec_ullong2,
   cmpnand2,
   ( vec_double2 x ))
{
   vec_double2 xneg;
   vec_ullong2 cmpgt, cmpeq, cmpnan;
   vec_uchar16 even = __extension__ (vec_uchar16)(vec_uint4){ 0x00010203, 0x00010203, 0x08090a0b, 0x08090a0b };
   vec_uchar16 odd = __extension__ (vec_uchar16)(vec_uint4){ 0x04050607, 0x04050607, 0x0c0d0e0f, 0x0c0d0e0f };
   vec_uint4   expmask = (vec_uint4)spu_splats(0xfff0000000000000ull);

   xneg = (vec_double2)spu_or( (vec_ullong2)x, spu_splats(0x8000000000000000ull) );
   cmpgt = (vec_ullong2)spu_cmpgt( (vec_uint4)xneg, expmask );
   cmpeq = (vec_ullong2)spu_cmpeq( (vec_uint4)xneg, expmask );

   cmpnan = spu_or( spu_shuffle( cmpgt, cmpgt, even ),
                    spu_and( spu_shuffle( cmpeq, cmpeq, even ), 
                             spu_shuffle( cmpgt, cmpgt, odd ) ) );

   return cmpnan;
}

