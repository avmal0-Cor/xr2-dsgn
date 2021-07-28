/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2004 Sony Computer Entertainment Inc. 
* All Rights Reserved.
*/
// divi4 - for each of four integer slots, compute quotient and remainder of numer/denom
// and store in divi4_t struct.  Divide by zero produces quotient = 0, remainder = numerator.

_FUNC_DEF(
   divi4_t,
   divi4,
   ( vec_int4 numer, vec_int4 denom ))
{
   divi4_t res;
   vec_int4 quot, newQuot, shift;
   vec_uint4 numerPos, denomPos, quotNeg;
   vec_uint4 numerAbs, denomAbs;
   vec_uint4 denomZeros, numerZeros, denomLeft, oneLeft, denomShifted, oneShifted;
   vec_uint4 newNum, skip, cont;
   int       anyCont;

   // Determine whether result needs sign change

   numerPos = spu_cmpgt( numer, -1 );
   denomPos = spu_cmpgt( denom, -1 );
   quotNeg = spu_xor( numerPos, denomPos );
    
   // Use absolute values of numerator, denominator

   numerAbs = (vec_uint4)spu_sel( spu_sub( 0, numer ), numer, numerPos );
   denomAbs = (vec_uint4)spu_sel( spu_sub( 0, denom ), denom, denomPos );

   // Get difference of leading zeros.
   // Any possible negative value will be interpreted as a shift > 31

   denomZeros = spu_cntlz( denomAbs );
   numerZeros = spu_cntlz( numerAbs );

   shift = (vec_int4)spu_sub( denomZeros, numerZeros );

   // Shift denom to align leading one with numerator's

   denomShifted = spu_sl( denomAbs, (vec_uint4)shift );
   oneShifted = spu_sl( (vec_uint4)spu_splats(1), (vec_uint4)shift );
   oneShifted = spu_sel( oneShifted, (vec_uint4)spu_splats(0), spu_cmpeq( denom, 0 ) );

   // Shift left all leading zeros.

   denomLeft = spu_sl( denomAbs, denomZeros );
   oneLeft = spu_sl( (vec_uint4)spu_splats(1), denomZeros );

   quot = spu_splats(0);

   do
   {
      cont = spu_cmpgt( oneShifted, 0U );
      anyCont = spu_extract( spu_gather( cont ), 0 );

      newQuot = spu_or( quot, (vec_int4)oneShifted );

      // Subtract shifted denominator from remaining numerator 
      // when denominator is not greater.

      skip = spu_cmpgt( denomShifted, numerAbs );
      newNum = spu_sub( numerAbs, denomShifted );

      // If denominator is greater, next shift is one more, otherwise
      // next shift is number of leading zeros of remaining numerator.

      numerZeros = spu_sel( spu_cntlz( newNum ), numerZeros, skip );
      shift = (vec_int4)spu_sub( skip, numerZeros );

      oneShifted = spu_rlmask( oneLeft, shift );
      denomShifted = spu_rlmask( denomLeft, shift );

      quot = spu_sel( newQuot, quot, skip );
      numerAbs = spu_sel( newNum, numerAbs, spu_orc(skip,cont) );
   } 
   while ( anyCont );

   res.quot = spu_sel( quot, spu_sub( 0, quot ), quotNeg );
   res.rem = spu_sel( spu_sub( 0, (vec_int4)numerAbs ), (vec_int4)numerAbs, numerPos );
   return res;
}

