/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2004 Sony Computer Entertainment Inc. 
* All Rights Reserved.
*/
// truncf4 - for each of four float slots, round towards zero to integer value.

_FUNC_DEF(
   vec_float4,
   truncf4,
   ( vec_float4 x ))
{
   vec_int4  xi;
   vec_uint4 inrange;
    
   // Can convert to and from signed integer to truncate values in range [-2^31, 2^31).
   // However, no truncation needed if exponent > 22.

   inrange = spu_cmpabsgt( (vec_float4)spu_splats(0x4b000000), x );

   xi = spu_convts( x, 0 );

   return spu_sel( x, spu_convtf( xi, 0 ), inrange );
}

