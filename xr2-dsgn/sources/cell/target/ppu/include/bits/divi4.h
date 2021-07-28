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
   vec_int4 minusone = (vec_int4)(-1);
   vec_uint4 zero = (vec_uint4)(0);
   vec_uint4 one = (vec_uint4)(1);
   vec_uint4 k158 = (vec_uint4)(158);
   vec_uint4 k23 = (vec_uint4)(23);
   
   divi4_t res;
   vec_uint4 numerPos, denomPos, quotNeg;
   vec_uint4 numerAbs, denomAbs;
   vec_uint4 denomZeros, numerZeros, shift, denomShifted, oneShifted;
   vec_uint4 quot, newQuot, skip, newNum, cont;
   int       anyCont;
       
   // determine whether result needs sign change
 
   numerPos = (vec_uint4)vec_cmpgt( numer, minusone );
   denomPos = (vec_uint4)vec_cmpgt( denom, minusone );
   quotNeg = vec_xor( numerPos, denomPos );
   
   // use absolute values of numerator, denominator
 
   numerAbs = (vec_uint4)vec_sel( vec_sub( (vec_int4)zero, numer ), numer, numerPos );
   denomAbs = (vec_uint4)vec_sel( vec_sub( (vec_int4)zero, denom ), denom, denomPos );
 
   // get difference of leading zeros to align denom with numer

   denomZeros = vec_sub( k158, vec_sr( (vec_uint4)vec_ctf( denomAbs, 0 ), k23 ) );
   numerZeros = vec_sub( k158, vec_sr( (vec_uint4)vec_ctf( numerAbs, 0 ), k23 ) );
      
   shift = vec_sub( denomZeros, numerZeros );
   denomShifted = vec_sl( denomAbs, shift );
   oneShifted = vec_sl( one, shift );
   oneShifted = vec_sel( oneShifted, zero, vec_or( vec_cmpeq( denomAbs, zero ), 
                                                   vec_cmpgt( denomAbs, numerAbs ) ) );
   
   // long division

   quot = zero;
   
   do
   {
      cont = (vec_uint4)vec_cmpgt( oneShifted, zero );
      anyCont = vec_any_gt( oneShifted, zero );
      skip = (vec_uint4)vec_cmpgt( denomShifted, numerAbs );
      
      newQuot = vec_or( quot, oneShifted );
      newNum = vec_sub( numerAbs, denomShifted );
      
      oneShifted = vec_sr( oneShifted, one );
      denomShifted = vec_sr( denomShifted, one );
      
      quot = vec_sel( newQuot, quot, skip );
      numerAbs = vec_sel( numerAbs, newNum, vec_andc( cont, skip ) );
   }
   while ( anyCont );

   res.quot = (vec_int4)vec_sel( quot, vec_sub( zero, quot ), quotNeg );
   res.rem = (vec_int4)vec_sel( (vec_uint4)vec_sub( (vec_int4)zero, (vec_int4)numerAbs ), numerAbs, numerPos );
   return res;
}

