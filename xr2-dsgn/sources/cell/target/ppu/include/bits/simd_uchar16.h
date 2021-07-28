/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2008 Sony Computer Entertainment Inc. 
* All Rights Reserved.
*/

#ifndef _SCE_SIMD_UCHAR16_H
#define _SCE_SIMD_UCHAR16_H

#include <altivec.h>
#include "simd_scalars.h"

namespace simd {

class bool16;
class char16;
class uchar16_idx;

//--------------------------------------------------------------------------------------------------
// uchar16 class
//

class uchar16
{
   private:
      vec_uchar16 data;

   public:
      inline uchar16() {}
      inline uchar16( bool16 );
      inline uchar16( char16 );
      inline uchar16( uchar r0, uchar r1, uchar r2, uchar r3,
                      uchar r4, uchar r5, uchar r6, uchar r7,
                      uchar r8, uchar r9, uchar r10, uchar r11,
                      uchar r12, uchar r13, uchar r14, uchar r15 );

      // set all slots to same uchar

      explicit inline uchar16( uchar );

      // allows implicit conversion from vector unsigned char

      inline uchar16( vec_uchar16 rhs );

      // allows implicit conversion to vector unsigned char

      inline operator vector unsigned char() const;

      // use array index operator to set and get slots

      inline uchar16_idx operator [] ( int i );
      inline uchar operator [] ( int i ) const;

      inline const uchar16 operator ++ ( int );
      inline const uchar16 operator -- ( int );
      inline uchar16& operator ++ ();
      inline uchar16& operator -- ();

      inline const uchar16 operator ~ () const;
      inline const bool16 operator ! () const;
      inline const char16 operator - () const;

      inline uchar16& operator = ( uchar16 rhs );
      inline uchar16& operator *= ( uchar16 rhs );
      inline uchar16& operator /= ( uchar16 rhs );
      inline uchar16& operator %= ( uchar16 rhs );
      inline uchar16& operator += ( uchar16 rhs );
      inline uchar16& operator -= ( uchar16 rhs );
      inline uchar16& operator <<= ( uchar16 rhs );
      inline uchar16& operator >>= ( uchar16 rhs );
      inline uchar16& operator &= ( uchar16 rhs );
      inline uchar16& operator ^= ( uchar16 rhs );
      inline uchar16& operator |= ( uchar16 rhs );
};

//--------------------------------------------------------------------------------------------------
// functions for uchar16
//

inline const uchar16 operator * ( uchar16 lhs, uchar16 rhs );
inline const uchar16 operator / ( uchar16 lhs, uchar16 rhs );
inline const uchar16 operator % ( uchar16 lhs, uchar16 rhs );
inline const uchar16 operator + ( uchar16 lhs, uchar16 rhs );
inline const uchar16 operator - ( uchar16 lhs, uchar16 rhs );

inline const uchar16 operator << ( uchar16 lhs, uchar16 rhs );
inline const uchar16 operator >> ( uchar16 lhs, uchar16 rhs );

inline const bool16 operator < ( uchar16 lhs, uchar16 rhs );
inline const bool16 operator <= ( uchar16 lhs, uchar16 rhs );
inline const bool16 operator > ( uchar16 lhs, uchar16 rhs );
inline const bool16 operator >= ( uchar16 lhs, uchar16 rhs );
inline const bool16 operator == ( uchar16 lhs, uchar16 rhs );
inline const bool16 operator != ( uchar16 lhs, uchar16 rhs );

// select between lhs and rhs slots using bool16.
// false selects lhs slot, true selects rhs slot

inline const uchar16 select( uchar16 lhs, uchar16 rhs, bool16 rhs_slots );

inline const uchar16 operator & ( uchar16 lhs, uchar16 rhs );
inline const uchar16 operator ^ ( uchar16 lhs, uchar16 rhs );
inline const uchar16 operator | ( uchar16 lhs, uchar16 rhs );

//--------------------------------------------------------------------------------------------------
// uchar16_idx: for setting uchar16 slots with [] operator
//

class uchar16_idx
{
   private:
      uchar16 &ref __attribute__ ((aligned(16)));
      int i __attribute__ ((aligned(16)));
   public:
      inline uchar16_idx( uchar16& vec, int idx ): ref(vec) { i = idx; }
      inline operator uchar() const;
      inline uchar operator = ( uchar rhs );
      inline uchar operator = ( const uchar16_idx& rhs );
      inline uchar operator ++ ( int );
      inline uchar operator -- ( int );
      inline uchar operator ++ ();
      inline uchar operator -- ();
      inline uchar operator *= ( uchar rhs );
      inline uchar operator /= ( uchar rhs );
      inline uchar operator %= ( uchar rhs );
      inline uchar operator += ( uchar rhs );
      inline uchar operator -= ( uchar rhs );
      inline uchar operator <<= ( uchar rhs );
      inline uchar operator >>= ( uchar rhs );
      inline uchar operator &= ( uchar rhs );
      inline uchar operator ^= ( uchar rhs );
      inline uchar operator |= ( uchar rhs );
};

}; // namespace simd

#include "simd_bool16.h"
#include "simd_char16.h"
#include "simd_ushort8.h"

namespace simd {

inline
uchar16::uchar16( uchar rhs )
{
   uchar tmp = (uchar)rhs;
   data = __extension__ (vec_uchar16){ tmp, tmp, tmp, tmp, tmp, tmp, tmp, tmp,
                                       tmp, tmp, tmp, tmp, tmp, tmp, tmp, tmp };
}

inline
uchar16::uchar16( vec_uchar16 rhs )
{
   data = rhs;
}

inline
uchar16::uchar16( bool16 rhs )
{
   data = (vec_uchar16)vec_and( (vec_uchar16)rhs, (vec_uchar16)(1) );
}

inline
uchar16::uchar16( char16 rhs )
{
   data = (vec_uchar16)(vec_char16)rhs;
}

inline
uchar16::uchar16( uchar r0, uchar r1, uchar r2, uchar r3,
                  uchar r4, uchar r5, uchar r6, uchar r7,
                  uchar r8, uchar r9, uchar r10, uchar r11,
                  uchar r12, uchar r13, uchar r14, uchar r15 )
{
   data = __extension__ (vec_uchar16){ r0, r1, r2, r3,
                                       r4, r5, r6, r7,
                                       r8, r9, r10, r11,
                                       r12, r13, r14, r15 };
}

inline
uchar16::operator vector unsigned char() const
{
   return data;
}

inline
uchar16_idx
uchar16::operator [] ( int i )
{
   return uchar16_idx(*this,i);
}

inline
uchar
uchar16::operator [] ( int i ) const
{
   return (uchar)_vec_extract((vec_uchar16)data, i);
}

inline
const uchar16
uchar16::operator ++ ( int )
{
   vec_uchar16 olddata = data;
   operator ++();
   return uchar16(olddata);
}

inline
const uchar16
uchar16::operator -- ( int )
{
   vec_uchar16 olddata = data;
   operator --();
   return uchar16(olddata);
}

inline
uchar16&
uchar16::operator ++ ()
{
   *this += uchar16(1);
   return *this;
}

inline
uchar16&
uchar16::operator -- ()
{
   *this -= uchar16(1);
   return *this;
}

inline
const uchar16 
uchar16::operator ~ () const
{
   return uchar16( vec_nor( data, data ) );
}

inline
const bool16
uchar16::operator ! () const
{
   return ( *this == uchar16(0) );
}

inline
const char16
uchar16::operator - () const
{
   return uchar16(0) - *this;
}

inline
uchar16&
uchar16::operator = ( uchar16 rhs )
{
   data = rhs.data;
   return *this;
}

inline
uchar16&
uchar16::operator *= ( uchar16 rhs )
{
   *this = *this * rhs;
   return *this;
}

inline
uchar16&
uchar16::operator /= ( uchar16 rhs )
{
   *this = *this / rhs;
   return *this;
}

inline
uchar16&
uchar16::operator %= ( uchar16 rhs )
{
   *this = *this % rhs;
   return *this;
}

inline
uchar16&
uchar16::operator += ( uchar16 rhs )
{
   *this = *this + rhs;
   return *this;
}

inline
uchar16&
uchar16::operator -= ( uchar16 rhs )
{
   *this = *this - rhs;
   return *this;
}

inline
uchar16&
uchar16::operator <<= ( uchar16 rhs )
{
   *this = *this << rhs;
   return *this;
}

inline
uchar16&
uchar16::operator >>= ( uchar16 rhs )
{
   *this = *this >> rhs;
   return *this;
}

inline
uchar16&
uchar16::operator &= ( uchar16 rhs )
{
   *this = *this & rhs;
   return *this;
}

inline
uchar16&
uchar16::operator ^= ( uchar16 rhs )
{
   *this = *this ^ rhs;
   return *this;
}

inline
uchar16&
uchar16::operator |= ( uchar16 rhs )
{
   *this = *this | rhs;
   return *this;
}

inline
const uchar16
operator * ( uchar16 lhs, uchar16 rhs )
{
   vec_short8 hi_mul = (vec_short8)vec_mule( (vec_uchar16)lhs, (vec_uchar16)rhs );
   vec_short8 lo_mul = (vec_short8)vec_mulo( (vec_uchar16)lhs, (vec_uchar16)rhs );
   vec_uchar16 mul = (vec_uchar16)vec_pack( hi_mul, lo_mul );
   
   return uchar16( vec_mergeh( mul, vec_sld( mul, mul, 8 ) ) );
}

inline
const uchar16
operator / ( uchar16 num, uchar16 den )
{
   const vec_uchar16 lzTable = __extension__ (vec_uchar16){ 4, 3, 2, 2, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0 };
   const vec_uchar16 zero = (vec_uchar16)(0);
   const vec_uchar16 one = (vec_uchar16)(1);
   const vec_uchar16 four = (vec_uchar16)(4);
     
   vec_uchar16 denom, numer;
   vec_uchar16 lo_denomZeros, hi_denomZeros, lo_numerZeros, hi_numerZeros;
   vec_uchar16 denomZeros, numerZeros, shift, denomShifted, oneShifted;
   vec_uchar16 quot, newQuot, skip, newNum, cont;
   int         anyCont;
   
   denom = (vec_uchar16)den;
   numer = (vec_uchar16)num;

   // to align denom with numer, get difference of leading zeros.
   // find leading zeros of each nibble by table lookup
      
   lo_denomZeros = vec_perm( lzTable, lzTable, denom );
   hi_denomZeros = vec_perm( lzTable, lzTable, vec_sr( denom, four ) );
   lo_numerZeros = vec_perm( lzTable, lzTable, numer );
   hi_numerZeros = vec_perm( lzTable, lzTable, vec_sr( numer, four ) );
   
   denomZeros = vec_sel( hi_denomZeros, vec_add( lo_denomZeros, four ), vec_cmpeq( hi_denomZeros, four ) );
   numerZeros = vec_sel( hi_numerZeros, vec_add( lo_numerZeros, four ), vec_cmpeq( hi_numerZeros, four ) );
   shift = vec_sub( denomZeros, numerZeros );
   
   denomShifted = vec_sl( denom, shift );
   oneShifted = vec_sl( one, shift );
   oneShifted = vec_sel( oneShifted, zero, vec_or( vec_cmpeq( denom, zero ), vec_cmpgt( denom, numer ) ) );
   
   quot = zero;

   // long-division
      
   do
   {
      cont = (vec_uchar16)vec_cmpgt( oneShifted, zero );
      anyCont = vec_any_gt( oneShifted, zero );
      skip = (vec_uchar16)vec_cmpgt( denomShifted, numer );   
      
      newQuot = vec_or( quot, oneShifted );      
      newNum = vec_sub( numer, denomShifted );      
      
      oneShifted = vec_sr( oneShifted, one );
      denomShifted = vec_sr( denomShifted, one );
      
      quot = vec_sel( newQuot, quot, skip );
      numer = vec_sel( numer, newNum, vec_andc( cont, skip ) );
   }
   while ( anyCont );
      
   return uchar16( quot );
}

inline
const uchar16
operator % ( uchar16 num, uchar16 den )
{
   const vec_uchar16 lzTable = __extension__ (vec_uchar16){ 4, 3, 2, 2, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0 };
   const vec_uchar16 zero = (vec_uchar16)(0);
   const vec_uchar16 one = (vec_uchar16)(1);
   const vec_uchar16 four = (vec_uchar16)(4);
     
   vec_uchar16 denom, numer;
   vec_uchar16 lo_denomZeros, hi_denomZeros, lo_numerZeros, hi_numerZeros;
   vec_uchar16 denomZeros, numerZeros, shift, denomShifted, oneShifted;
   vec_uchar16 quot, newQuot, skip, newNum, cont;
   int         anyCont;
   
   denom = (vec_uchar16)den;
   numer = (vec_uchar16)num;

   // to align denom with numer, get difference of leading zeros.
   // find leading zeros of each nibble by table lookup
      
   lo_denomZeros = vec_perm( lzTable, lzTable, denom );
   hi_denomZeros = vec_perm( lzTable, lzTable, vec_sr( denom, four ) );
   lo_numerZeros = vec_perm( lzTable, lzTable, numer );
   hi_numerZeros = vec_perm( lzTable, lzTable, vec_sr( numer, four ) );
   
   denomZeros = vec_sel( hi_denomZeros, vec_add( lo_denomZeros, four ), vec_cmpeq( hi_denomZeros, four ) );
   numerZeros = vec_sel( hi_numerZeros, vec_add( lo_numerZeros, four ), vec_cmpeq( hi_numerZeros, four ) );
   shift = vec_sub( denomZeros, numerZeros );
   
   denomShifted = vec_sl( denom, shift );
   oneShifted = vec_sl( one, shift );
   oneShifted = vec_sel( oneShifted, zero, vec_or( vec_cmpeq( denom, zero ), vec_cmpgt( denom, numer ) ) );
   
   quot = zero;

   // long-division
      
   do
   {
      cont = (vec_uchar16)vec_cmpgt( oneShifted, zero );
      anyCont = vec_any_gt( oneShifted, zero );
      skip = (vec_uchar16)vec_cmpgt( denomShifted, numer );   
      
      newQuot = vec_or( quot, oneShifted );      
      newNum = vec_sub( numer, denomShifted );      
      
      oneShifted = vec_sr( oneShifted, one );
      denomShifted = vec_sr( denomShifted, one );
      
      quot = vec_sel( newQuot, quot, skip );
      numer = vec_sel( numer, newNum, vec_andc( cont, skip ) );
   }
   while ( anyCont );
      
   return uchar16( numer );
}

inline
const uchar16
operator + ( uchar16 lhs, uchar16 rhs )
{
   return uchar16( vec_add( (vec_uchar16)lhs, (vec_uchar16)rhs ) );
}

inline
const uchar16
operator - ( uchar16 lhs, uchar16 rhs )
{
   return uchar16( vec_sub( (vec_uchar16)lhs, (vec_uchar16)rhs ) );
}

inline
const uchar16
operator << ( uchar16 lhs, uchar16 rhs )
{
   return uchar16( vec_sl( (vec_uchar16)lhs, (vec_uchar16)rhs ) );
}

inline
const uchar16
operator >> ( uchar16 lhs, uchar16 rhs )
{
   return uchar16( vec_sr( (vec_uchar16)lhs, (vec_uchar16)rhs ) );
}

inline
const bool16
operator < ( uchar16 lhs, uchar16 rhs )
{
   bool16 result;
   result.data = (vec_uchar16)vec_cmpgt( (vec_uchar16)rhs, (vec_uchar16)lhs );
   return result;
}

inline
const bool16
operator <= ( uchar16 lhs, uchar16 rhs )
{
   return !( lhs > rhs );
}

inline
const bool16
operator > ( uchar16 lhs, uchar16 rhs )
{
   bool16 result;
   result.data = (vec_uchar16)vec_cmpgt( (vec_uchar16)lhs, (vec_uchar16)rhs );
   return result;
}

inline
const bool16
operator >= ( uchar16 lhs, uchar16 rhs )
{
   return !( lhs < rhs );
}

inline
const bool16
operator == ( uchar16 lhs, uchar16 rhs )
{
   bool16 result;
   result.data = (vec_uchar16)vec_cmpeq( (vec_uchar16)lhs, (vec_uchar16)rhs );
   return result;
}

inline
const bool16
operator != ( uchar16 lhs, uchar16 rhs )
{
   return !( lhs == rhs );
}
   
inline
const uchar16
select( uchar16 lhs, uchar16 rhs, bool16 rhs_slots )
{
   return uchar16( vec_sel( (vec_uchar16)lhs, (vec_uchar16)rhs, (vec_uchar16)rhs_slots ) );
}

inline
const uchar16
operator & ( uchar16 lhs, uchar16 rhs )
{
   return uchar16( vec_and( (vec_uchar16)lhs, (vec_uchar16)rhs ) );
}

inline
const uchar16
operator | ( uchar16 lhs, uchar16 rhs )
{
   return uchar16( vec_or( (vec_uchar16)lhs, (vec_uchar16)rhs ) );
}

inline
const uchar16
operator ^ ( uchar16 lhs, uchar16 rhs )
{
   return uchar16( vec_xor( (vec_uchar16)lhs, (vec_uchar16)rhs ) );
}
   
inline
uchar16_idx::operator uchar() const
{ 
   return (uchar)_vec_extract( (vec_uchar16)ref, i );
}

inline 
uchar
uchar16_idx::operator = ( uchar rhs )
{
   ref = _vec_insert( (uchar)rhs, (vec_uchar16)ref, i );
   return rhs;
}

inline
uchar
uchar16_idx::operator = ( const uchar16_idx& rhs )
{
   return ( *this = (uchar)rhs );
}

inline
uchar
uchar16_idx::operator ++( int )
{
   uchar tmp = _vec_extract( (vec_uchar16)ref, i );
   ref = _vec_insert( tmp+1, (vec_uchar16)ref, i );
   return tmp;
}

inline
uchar
uchar16_idx::operator --( int )
{
   uchar tmp = _vec_extract( (vec_uchar16)ref, i );
   ref = _vec_insert( tmp-1, (vec_uchar16)ref, i );
   return tmp;
}

inline
uchar
uchar16_idx::operator ++()
{
   uchar tmp = _vec_extract( (vec_uchar16)ref, i );
   ++tmp;
   ref = _vec_insert( tmp, (vec_uchar16)ref, i );
   return tmp;
}

inline
uchar
uchar16_idx::operator --()
{
   uchar tmp = _vec_extract( (vec_uchar16)ref, i );
   --tmp;
   ref = _vec_insert( tmp, (vec_uchar16)ref, i );
   return tmp;
}

inline
uchar
uchar16_idx::operator *= ( uchar rhs )
{
   uchar tmp = _vec_extract( (vec_uchar16)ref, i ) * rhs;
   ref = _vec_insert( tmp, (vec_uchar16)ref, i );
   return tmp;
}

inline
uchar
uchar16_idx::operator /= ( uchar rhs )
{
   uchar tmp = _vec_extract( (vec_uchar16)ref, i ) / rhs;
   ref = _vec_insert( tmp, (vec_uchar16)ref, i );
   return tmp;
}

inline
uchar
uchar16_idx::operator %= ( uchar rhs )
{
   uchar tmp = _vec_extract( (vec_uchar16)ref, i ) % rhs;
   ref = _vec_insert( tmp, (vec_uchar16)ref, i );
   return tmp;
}

inline
uchar
uchar16_idx::operator += ( uchar rhs )
{
   uchar tmp = _vec_extract( (vec_uchar16)ref, i ) + rhs;
   ref = _vec_insert( tmp, (vec_uchar16)ref, i );
   return tmp;
}

inline
uchar
uchar16_idx::operator -= ( uchar rhs )
{
   uchar tmp = _vec_extract( (vec_uchar16)ref, i ) - rhs;
   ref = _vec_insert( tmp, (vec_uchar16)ref, i );
   return tmp;
}

inline
uchar
uchar16_idx::operator <<= ( uchar rhs )
{
   uchar tmp = _vec_extract( (vec_uchar16)ref, i ) << rhs;
   ref = _vec_insert( tmp, (vec_uchar16)ref, i );
   return tmp;
}

inline
uchar
uchar16_idx::operator >>= ( uchar rhs )
{
   uchar tmp = _vec_extract( (vec_uchar16)ref, i ) >> rhs;
   ref = _vec_insert( tmp, (vec_uchar16)ref, i );
   return tmp;
}

inline
uchar
uchar16_idx::operator &= ( uchar rhs )
{
   uchar tmp = _vec_extract( (vec_uchar16)ref, i ) & rhs;
   ref = _vec_insert( tmp, (vec_uchar16)ref, i );
   return tmp;
}

inline
uchar
uchar16_idx::operator ^= ( uchar rhs )
{
   uchar tmp = _vec_extract( (vec_uchar16)ref, i ) ^ rhs;
   ref = _vec_insert( tmp, (vec_uchar16)ref, i );
   return tmp;
}

inline
uchar
uchar16_idx::operator |= ( uchar rhs )
{
   uchar tmp = _vec_extract( (vec_uchar16)ref, i ) | rhs;
   ref = _vec_insert( tmp, (vec_uchar16)ref, i );
   return tmp;
}

}; // namespace simd

#endif // uchar16_h
