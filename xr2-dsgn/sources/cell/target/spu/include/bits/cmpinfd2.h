/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2007 Sony Computer Entertainment Inc. 
* All Rights Reserved.
*/
// cmpinfd2 - for each of two double slots, if input equals +Inf or -Inf return mask of ones, else 0

_FUNC_DEF(
   vec_ullong2,
   cmpinfd2,
   ( vec_double2 x ))
{
   vec_uchar16 swapEvenOdd = __extension__ (vec_uchar16)(vec_uint4){ 0x04050607, 0x00010203, 0x0c0d0e0f, 0x08090a0b };
   vec_double2 xabs;
   vec_ullong2 cmp;

   xabs = (vec_double2)spu_andc( (vec_ullong2)x, spu_splats(0x8000000000000000ull) );
   cmp = (vec_ullong2)spu_cmpeq( (vec_uint4)xabs, (vec_uint4)spu_splats(0x7ff0000000000000ull) );
   cmp = spu_and( cmp, spu_shuffle( cmp, cmp, swapEvenOdd ) );

   return cmp;
}

