/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2004 Sony Computer Entertainment Inc. 
* All Rights Reserved.
*/
// sqrtd2 - for each of two double slots, compute square root.
// 
// Handles exceptional values as follows:
// NaN -> NaN
// -Inf -> Nan
// -Finite -> Nan
// Denormal inputs are treated as zero.

_FUNC_DEF(
   vec_double2,
   sqrtd2,
   ( vec_double2 x ))
{
   vec_ullong2 expmask, onemask, signmask, evenexp;
   vec_double2 half, one, man, exp, nexp, y1, y2, y3, zero, inf, nan, result;
   vec_float4  halff, onef, manf, y0f, y1f;

   expmask = spu_splats(0x7ff0000000000000ull);
   onemask = spu_splats(0x0010000000000000ull);
   signmask = spu_splats(0x8000000000000000ull);
   onef = spu_splats(1.0f);
   one = spu_extend( onef );
   halff = spu_splats(0.5f);
   half = spu_extend( halff );
              
   // First compute reciprocal square root.
   // Factor input ( mantissa x 2^exponent ) into ( mantissa x 2^(-i) ) and ( 2^(exponent+i) )
   // where i = 0 when exponent is even and i = 1 when exponent is odd.
   // 
   // Compute reciprocal-square-root of second factor by finding -(exponent+i)/2:
   // 
   // biased_exp = 1023 + exponent
   // new_biased_exp = 1023 - (exponent+i)/2 
   //                = 1023 - (biased_exp-1023+i)/2
   //                = (3069 - (biased_exp+i)) / 2

   evenexp = spu_and( (vec_ullong2)x, onemask );
   man = spu_sel( x, (vec_double2)spu_add( spu_splats(0x3fe00000u), (vec_uint4)evenexp ), expmask );

   exp = spu_and( x, (vec_double2)expmask );
   nexp = spu_or( exp, (vec_double2)onemask );
   nexp = (vec_double2)spu_rlmask( spu_sub( (vec_uint4)spu_splats(0xbfd0000000000000ull), (vec_uint4)nexp ), -1 );

   // Compute mantissa part in single precision.
   // Convert back to double and multiply with 2^(-(exponent+i)/2), then
   // do two Newton-Raphson steps for full precision.

   manf = spu_roundtf( man );
   y0f = spu_rsqrte( manf );
   y1f = spu_madd( spu_mul( y0f, halff ), spu_nmsub( y0f, spu_mul( y0f, manf ), onef ), y0f );
   y1 = spu_mul( spu_extend( y1f ), nexp );
   y2 = spu_madd( spu_mul( y1, half ), spu_nmsub( y1, spu_mul( y1, x ), one ), y1 );
   y3 = spu_madd( spu_mul( y2, half ), spu_nmsub( y2, spu_mul( y2, x ), one ), y2 );

   // Multiply by input to get square root.

   y3 = spu_mul( y3, x );

   // Choose iterated result or special value.

   zero = spu_and( x, (vec_double2)signmask );
   inf = (vec_double2)expmask;
   nan = (vec_double2)spu_splats(0x7ff8000000000000ull);

   result = spu_sel( y3, inf, cmpinfd2( x ) );
   result = spu_sel( result, nan, cmpnegsignd2( x ) );
   result = spu_sel( result, zero, cmpzerodenormd2( x ) );

   return result;
}

