/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2008 Sony Computer Entertainment Inc. 
 * All Rights Reserved.
 */
// fmodf4 - for each of four float slots, compute remainder of x/y defined as x - truncated_integer(x/y) * y.
//
// This returns an accurate result when |divf4(x,y)| < 2^20 and |x| < 2^128, and otherwise returns zero.
// If x == 0, the result is 0.
// If x != 0 and y == 0, the result is undefined.

_FUNC_DEF(
   vec_float4,
   fmodf4,
   ( vec_float4 x, vec_float4 y ))
{
   vec_float4 q, xabs, yabs, qabs;
   vec_int4   qi0, qi1, qi2;
   vec_float4 i0, i1, i2, r1, r2, i;
   vec_uint4  inrange;
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
                                                                                                                                                          
   i0 = vec_ctf( qi0, 0 );
   i1 = vec_ctf( qi1, 0 );
   i2 = vec_ctf( qi2, 0 );
                                                                                                                                                          
   // Correct i will be the largest one such that |x| - i*|y| >= 0.
                                                                                                                                                          
   r1 = vec_nmsub( i1, yabs, xabs );
   r2 = vec_nmsub( i2, yabs, xabs );
                                                                                                                                                          
   i = i0;
   i = vec_sel( i1, i, (vec_uint4)vec_cmpgt( vzero, r1 ) );
   i = vec_sel( i2, i, (vec_uint4)vec_cmpgt( vzero, r2 ) );
                                                                                                                                                          
   i = copysignf4( i, q );
                                                                                                                                                          
   return vec_sel(vec_sel((vec_float4)vzero, vec_nmsub( i, y, x ), inrange),
		  x, iszero);
}

