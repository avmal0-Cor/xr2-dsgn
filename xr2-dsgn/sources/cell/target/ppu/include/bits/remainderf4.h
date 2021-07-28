/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2008 Sony Computer Entertainment Inc. 
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
   vec_float4 q, xabs, yabs, qabs, yabshalf;
   vec_int4   qi0, qi1, qi2;
   vec_float4 i0, i1, i2, i, rem;
   vec_uint4  inrange, odd0, odd1, odd2, cmp1, cmp2, odd;
   vec_float4 vzero = (vec_float4)vec_xor((vec_uint4)(0), (vec_uint4)(0));
   vec_uint4 iszero = (vec_uint4)vec_cmpeq(x, vzero);
 
   // Find i = truncated_integer(|x/y|)
 
   // If |divf4(x,y)| < 2^20, the quotient is at most off by 1.0.
   // Thus i is either the truncated quotient, one less, or one greater.
                                                                                
   q = divf4( x, y );
   xabs = fabsf4( x );
   yabs = fabsf4( y );
   qabs = fabsf4( q );
                                                                                
   inrange = (vec_uint4)vec_cmpgt( (vec_float4)((vec_uint4)(0x49800000)), qabs );
                                                                                
   qi1 = vec_cts( qabs, 0 );
   qi0 = vec_add( qi1, (vec_int4)(-1) );
   qi2 = vec_add( qi1, (vec_int4)(1) );
                                                                                                                                                          
   odd1 = (vec_uint4)vec_cmpeq( vec_and( qi1, (vec_int4)(1) ), (vec_int4)(1) );
   odd0 = odd2 = vec_nor( odd1, odd1 );

   i0 = vec_ctf( qi0, 0 );
   i1 = vec_ctf( qi1, 0 );
   i2 = vec_ctf( qi2, 0 );
                                                                                                                                                          
   // Correct i will be the largest one such that |x| - i*|y| >= 0.
                                                                                                                                                          
   cmp1 = (vec_uint4)vec_cmpgt( (vec_int4)vzero, (vec_int4)vec_nmsub( i1, yabs, xabs ) );
   cmp2 = (vec_uint4)vec_cmpgt( (vec_int4)vzero, (vec_int4)vec_nmsub( i2, yabs, xabs ) );
                                                                                                                    
   i = i0;
   i = vec_sel( i1, i, cmp1 );
   i = vec_sel( i2, i, cmp2 );
                                                                                                                    
   odd = odd0;
   odd = vec_sel( odd1, odd, cmp1 );
   odd = vec_sel( odd2, odd, cmp2 );
                                                                                                                    
   rem = vec_nmsub( i, yabs, xabs );
                                                                                                                    
   // Test whether i or i+1 = nearest_integer(|x/y|)
   //
   // i+1 is correct if:
   //
   // rem > 0.5*|y|
   // or
   // rem = 0.5*|y| and i is odd

   yabshalf = vec_madd( yabs, (vec_float4)(0.5f), (vec_float4)vzero );
   cmp1 = (vec_uint4)vec_cmpgt( rem, yabshalf );
   cmp2 = vec_and( (vec_uint4)vec_cmpeq( rem, yabshalf ), odd );
                                                                                                                    
   i = vec_sel( i, vec_add( i, (vec_float4)(1.0f) ), vec_or( cmp1, cmp2 ) );
   i = copysignf4( i, q );
                                                                                                                                                          
   return vec_sel(vec_sel((vec_float4)vzero, vec_nmsub( i, y, x ), inrange ),
		 x, iszero);
}

