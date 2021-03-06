/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2004 Sony Computer Entertainment Inc. 
* All Rights Reserved.
*/
// remainderf4 - for each of four float slots, compute remainder of x/y defined as x - nearest_integer(x/y) * y.
//
// This returns an accurate result when |divf4(x,y)| < 2^20 and |x| < 2^128, and otherwise returns zero.
// If x == 0, the result is 0.
// If x != 0 and y == 0, the result is undefined.

_FUNC_DEF(
   vec_float4,
   remainderf4,
   ( vec_float4 x, vec_float4 y ))
{
   vec_float4 q, xabs, yabs, qabs, xabs2, yabshalf;
   vec_int4   qi0, qi1, qi2;
   vec_float4 i0, i1, i2, i, rem;
   vec_uint4  inrange, odd0, odd1, odd2, cmp1, cmp2, odd;

   // Find i = truncated_integer(|x/y|)

   // By the error bounds of divf4, if |x/y| is < 2^20, the quotient is at most off by 1.0.
   // Thus the exact truncation is either the truncated quotient, one less, or one greater.

   q = divf4( x, y );
   xabs = fabsf4( x );
   yabs = fabsf4( y );
   qabs = fabsf4( q );
   xabs2 = spu_add( xabs, xabs );
    
   inrange = spu_cmpabsgt( (vec_float4)spu_splats(0x49800000), q );
   inrange = spu_and( inrange, spu_cmpabsgt( (vec_float4)spu_splats(0x7f800000), x ) );

   qi1 = spu_convts( qabs, 0 );
   qi0 = spu_add( qi1, -1 );
   qi2 = spu_add( qi1, 1 );

   odd1 = spu_cmpeq( spu_and( qi1, 1 ), 1 );
   odd0 = odd2 = spu_nor( odd1, odd1 );

   i0 = spu_convtf( qi0, 0 );
   i1 = spu_convtf( qi1, 0 );
   i2 = spu_convtf( qi2, 0 );

   // Correct i will be the largest one such that |x| - i*|y| >= 0.  Can test instead as 
   // 2*|x| - i*|y| >= |x|:
   // 
   // With exact inputs, the negative-multiply-subtract gives the exact result rounded towards zero.  
   // Thus |x| - i*|y| may be < 0 but still round to zero.  However, if 2*|x| - i*|y| < |x|, the computed
   // answer will be rounded down to < |x|.  2*|x| can be represented exactly provided |x| < 2^128.

   cmp1 = spu_cmpgt( xabs, spu_nmsub( i1, yabs, xabs2 ) );
   cmp2 = spu_cmpgt( xabs, spu_nmsub( i2, yabs, xabs2 ) );

   i = i0;
   i = spu_sel( i1, i, cmp1 );
   i = spu_sel( i2, i, cmp2 );

   odd = odd0;
   odd = spu_sel( odd1, odd, cmp1 );
   odd = spu_sel( odd2, odd, cmp2 );
   
   rem = spu_nmsub( i, yabs, xabs );

   // Test whether i or i+1 = nearest_integer(|x/y|)
   //
   // i+1 is correct if:
   //
   // rem > 0.5*|y|
   // or 
   // rem = 0.5*|y| and i is odd

   yabshalf = spu_mul( yabs, spu_splats(0.5f) );
   cmp1 = spu_cmpgt( rem, yabshalf );
   cmp2 = spu_and( spu_cmpeq( rem, yabshalf ), odd );

   i = spu_sel( i, spu_add( i, spu_splats(1.0f) ), spu_or( cmp1, cmp2 ) );
   i = copysignf4( i, q );

   return spu_sel( spu_splats(0.0f), spu_nmsub( i, y, x ), inrange );
}

