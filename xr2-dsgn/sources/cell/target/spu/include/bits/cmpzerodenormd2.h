/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2007 Sony Computer Entertainment Inc. 
* All Rights Reserved.
*/
// cmpzerodenormd2 - for each of two double slots, if input equals 0 or denorm return mask of ones, else 0

_FUNC_DEF(
   vec_ullong2,
   cmpzerodenormd2,
   ( vec_double2 x ))
{
   vec_double2 xexp;
   vec_ullong2 cmp;
   vec_uchar16 even = __extension__ (vec_uchar16)(vec_uint4){ 0x00010203, 0x00010203, 0x08090a0b, 0x08090a0b };

   xexp = (vec_double2)spu_and( (vec_ullong2)x, spu_splats(0x7ff0000000000000ull) );
   cmp = (vec_ullong2)spu_cmpeq( (vec_uint4)xexp, (vec_uint4)spu_splats(0) );
   cmp = spu_shuffle( cmp, cmp, even );

   return cmp;
}
