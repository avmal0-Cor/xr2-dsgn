/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2004 Sony Computer Entertainment Inc. 
* All Rights Reserved.
*/
// recipd2 - for each of two double slots, compute reciprocal.
// 
// Handles exceptional values as follows:
// NaN -> NaN
// (+,-)Inf -> (+,-)0
// (+,-)0 -> (+,-)Inf
// Denormal inputs are treated as zero.
                                                                  
_FUNC_DEF(
   vec_double2,
   recipd2,
   ( vec_double2 x ))
{
   vec_ullong2 expmask, signmask;
   vec_double2 one, man, exp, nexp, y1, y2, y3, zero, inf, result;
   vec_float4  onef, manf, y0f, y1f;

   expmask = spu_splats(0x7ff0000000000000ull);
   signmask = spu_splats(0x8000000000000000ull);
   onef = spu_splats(1.0f);
   one = spu_extend( onef );

   // Factor ( mantissa x 2^exponent ) into ( mantissa x 2 ) and ( 2^(exponent-1) ).
   // Invert exponent part with subtraction.

   exp = spu_and( x, (vec_double2)expmask );
   nexp = (vec_double2)spu_sub( (vec_uint4)expmask, (vec_uint4)exp );
   man = spu_sel( x, (vec_double2)spu_splats(0x40000000), expmask );

   // Compute mantissa part with single and double precision Newton-Raphson steps.
   // Then multiply with 2^(1-exponent).

   manf = spu_roundtf( man );
   y0f = spu_re( manf );
   y1f = spu_madd( spu_nmsub( manf, y0f, onef ), y0f, y0f );
   y1 = spu_extend( y1f );
   y2 = spu_madd( spu_nmsub( man, y1, one ), y1, y1 );
   y3 = spu_madd( spu_nmsub( man, y2, one ), y2, y2 );
   y3 = spu_mul( y3, nexp );

   // Choose iterated result or special value.

   zero = spu_and( x, (vec_double2)signmask );
   inf = spu_sel( (vec_double2)expmask, x, signmask );

   result = spu_sel( y3, zero, cmpinfd2( x ) );
   result = spu_sel( result, inf, cmpzerodenormd2( x ) );
   result = spu_sel( result, x, cmpnand2( x ) );

   return result;
}

