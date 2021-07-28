/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2004 Sony Computer Entertainment Inc. 
* All Rights Reserved.
*/
// ceilf4 - for each of four float slots, round up to smallest integer not less than the value.

_FUNC_DEF(
    vec_float4,
    ceilf4,
    ( vec_float4 x ))
{
   vec_int4   xi, xi1;
   vec_uint4  inrange;
   vec_float4 truncated, truncated1;
    
   // Find truncated value and one greater.

   inrange = spu_cmpabsgt( (vec_float4)spu_splats(0x4b000000), x );

   xi = spu_convts( x, 0 );
   xi1 = spu_add( xi, 1 );

   truncated = spu_sel( x, spu_convtf( xi, 0 ), inrange );
   truncated1 = spu_sel( x, spu_convtf( xi1, 0 ), inrange );

   // If truncated value is less than input, add one.

   return spu_sel( truncated, truncated1, spu_cmpgt( x, truncated ) );
}

