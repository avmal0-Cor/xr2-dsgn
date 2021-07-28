/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2007 Sony Computer Entertainment Inc. 
* All Rights Reserved.
*/
// cmpnegsignd2 - for each of two double slots, if input has negative sign bit return mask of ones, else 0

_FUNC_DEF(
   vec_ullong2,
   cmpnegsignd2,
   ( vec_double2 x ))
{
   vec_ullong2 cmp;
   vec_uchar16 even = __extension__ (vec_uchar16)(vec_uint4){ 0x00010203, 0x00010203, 0x08090a0b, 0x08090a0b };

   cmp = (vec_ullong2)spu_cmpgt( spu_splats(0), (vec_int4)x );
   cmp = spu_shuffle( cmp, cmp, even );

   return cmp;
}

