/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2004 Sony Computer Entertainment Inc. 
* All Rights Reserved.
*/
_FUNC_DEF(
   float,
   modff,
   ( float x, float *iptr ))
{
   vec_float4 xv = spu_promote( x, 0 );
   vec_float4 integral, fraction;

   integral = truncf4( xv );
   fraction = spu_sub( xv, integral );

   *iptr = spu_extract( integral, 0 );
   return spu_extract( fraction, 0 );
}

