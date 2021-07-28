/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2007 Sony Computer Entertainment Inc. 
* All Rights Reserved.
*/
// cmpgtd2 - for each of two double slots, if x > y return mask of ones, else 0

_FUNC_DEF(
   vec_ullong2,
   cmpgtd2,
   ( vec_double2 x, vec_double2 y ))
{
   vec_uchar16 even = __extension__ (vec_uchar16)(vec_uint4){ 0x00010203, 0x00010203, 0x08090a0b, 0x08090a0b };
   vec_uchar16 odd = __extension__ (vec_uchar16)(vec_uint4){ 0x04050607, 0x04050607, 0x0c0d0e0f, 0x0c0d0e0f };
   vec_uchar16 swapEvenOdd = __extension__ (vec_uchar16)(vec_uint4){ 0x04050607, 0x00010203, 0x0c0d0e0f, 0x08090a0b };
   vec_ullong2 sign = spu_splats(0x8000000000000000ull);
   vec_uint4   cmpgt_i, cmpgt_ui, cmpeq_i, cmpeq_i_even;
   vec_ullong2 cmpgt_ll, cmplt_ll, cmpeq_ll;
   vec_ullong2 bothneg, bothzero;
   
   cmpgt_i = spu_cmpgt( (vec_int4)x, (vec_int4)y );
   cmpeq_i = spu_cmpeq( (vec_int4)x, (vec_int4)y );
   cmpgt_ui = spu_cmpgt( (vec_uint4)x, (vec_uint4)y );

   cmpeq_i_even = spu_shuffle( cmpeq_i, cmpeq_i, even );
   cmpgt_ll = (vec_ullong2)spu_or( spu_shuffle( cmpgt_i, cmpgt_i, even ),
                                   spu_and( cmpeq_i_even, spu_shuffle( cmpgt_ui, cmpgt_ui, odd ) ) );
   cmpeq_ll = (vec_ullong2)spu_and( cmpeq_i_even, spu_shuffle( cmpeq_i, cmpeq_i, odd ) );
   cmplt_ll = spu_nor( cmpeq_ll, cmpgt_ll );

   bothzero = spu_andc( spu_or( (vec_ullong2)x, (vec_ullong2)y ), sign );
   bothzero = (vec_ullong2)spu_cmpeq( (vec_uint4)bothzero, 0U );
   bothzero = spu_and( bothzero, spu_shuffle( bothzero, bothzero, swapEvenOdd ) );

   bothneg = spu_and( (vec_ullong2)x, (vec_ullong2)y );
   bothneg = (vec_ullong2)spu_cmpgt( spu_splats(0), (vec_int4)bothneg );
   bothneg = spu_shuffle( bothneg, bothneg, even );
   
   return spu_andc( spu_sel( cmpgt_ll, cmplt_ll, bothneg ), 
                    spu_or( bothzero, spu_or( cmpnand2( x ), cmpnand2( y ) ) ) );
}

