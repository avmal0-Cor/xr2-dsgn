/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2008 Sony Computer Entertainment Inc. 
* All Rights Reserved.
*/

#ifndef _SCE_SIMD_USHORT8_H
#define _SCE_SIMD_USHORT8_H

#include <altivec.h>
#include "simd_scalars.h"

namespace simd {

class bool8;
class short8;
class ushort8_idx;

//--------------------------------------------------------------------------------------------------
// ushort8 class
//

class ushort8
{
   private:
      vec_ushort8 data;

   public:
      inline ushort8() {}
      inline ushort8( bool8 );
      inline ushort8( short8 );
      inline ushort8( ushort r0, ushort r1, ushort r2, ushort r3,
                      ushort r4, ushort r5, ushort r6, ushort r7 );

      // set all slots to same ushort

      explicit inline ushort8( ushort );

      // allows implicit conversion from vector unsigned short

      inline ushort8( vec_ushort8 rhs );

      // allows implicit conversion to vector unsigned short

      inline operator vector unsigned short() const;

      // use array index operator to set and get slots

      inline ushort8_idx operator [] ( int i );
      inline ushort operator [] ( int i ) const;

      inline const ushort8 operator ++ ( int );
      inline const ushort8 operator -- ( int );
      inline ushort8& operator ++ ();
      inline ushort8& operator -- ();

      inline const ushort8 operator ~ () const;
      inline const bool8 operator ! () const;
      inline const short8 operator - () const;

      inline ushort8& operator = ( ushort8 rhs );
      inline ushort8& operator *= ( ushort8 rhs );
      inline ushort8& operator /= ( ushort8 rhs );
      inline ushort8& operator %= ( ushort8 rhs );
      inline ushort8& operator += ( ushort8 rhs );
      inline ushort8& operator -= ( ushort8 rhs );
      inline ushort8& operator <<= ( ushort8 rhs );
      inline ushort8& operator >>= ( ushort8 rhs );
      inline ushort8& operator &= ( ushort8 rhs );
      inline ushort8& operator ^= ( ushort8 rhs );
      inline ushort8& operator |= ( ushort8 rhs );
};

//--------------------------------------------------------------------------------------------------
// functions for ushort8
//

inline const ushort8 operator * ( ushort8 lhs, ushort8 rhs );
inline const ushort8 operator / ( ushort8 lhs, ushort8 rhs );
inline const ushort8 operator % ( ushort8 lhs, ushort8 rhs );
inline const ushort8 operator + ( ushort8 lhs, ushort8 rhs );
inline const ushort8 operator - ( ushort8 lhs, ushort8 rhs );

inline const ushort8 operator << ( ushort8 lhs, ushort8 rhs );
inline const ushort8 operator >> ( ushort8 lhs, ushort8 rhs );

inline const bool8 operator < ( ushort8 lhs, ushort8 rhs );
inline const bool8 operator <= ( ushort8 lhs, ushort8 rhs );
inline const bool8 operator > ( ushort8 lhs, ushort8 rhs );
inline const bool8 operator >= ( ushort8 lhs, ushort8 rhs );
inline const bool8 operator == ( ushort8 lhs, ushort8 rhs );
inline const bool8 operator != ( ushort8 lhs, ushort8 rhs );

// select between lhs and rhs slots using bool8.
// false selects lhs slot, true selects rhs slot

inline const ushort8 select( ushort8 lhs, ushort8 rhs, bool8 rhs_slots );

inline const ushort8 operator & ( ushort8 lhs, ushort8 rhs );
inline const ushort8 operator ^ ( ushort8 lhs, ushort8 rhs );
inline const ushort8 operator | ( ushort8 lhs, ushort8 rhs );

//--------------------------------------------------------------------------------------------------
// ushort8_idx: for setting ushort8 slots with [] operator
//

class ushort8_idx
{
   private:
      ushort8 &ref __attribute__ ((aligned(16)));
      int i __attribute__ ((aligned(16)));
   public:
      inline ushort8_idx( ushort8& vec, int idx ): ref(vec) { i = idx; }
      inline operator ushort() const;
      inline ushort operator = ( ushort rhs );
      inline ushort operator = ( const ushort8_idx& rhs );
      inline ushort operator ++ ( int );
      inline ushort operator -- ( int );
      inline ushort operator ++ ();
      inline ushort operator -- ();
      inline ushort operator *= ( ushort rhs );
      inline ushort operator /= ( ushort rhs );
      inline ushort operator %= ( ushort rhs );
      inline ushort operator += ( ushort rhs );
      inline ushort operator -= ( ushort rhs );
      inline ushort operator <<= ( ushort rhs );
      inline ushort operator >>= ( ushort rhs );
      inline ushort operator &= ( ushort rhs );
      inline ushort operator ^= ( ushort rhs );
      inline ushort operator |= ( ushort rhs );
};

}; // namespace simd

#include "simd_bool8.h"
#include "simd_short8.h"

namespace simd {

inline
ushort8::ushort8( ushort rhs )
{
   ushort tmp = (ushort)rhs;
   data = __extension__ (vec_ushort8){ tmp, tmp, tmp, tmp, tmp, tmp, tmp, tmp };
}

inline
ushort8::ushort8( vec_ushort8 rhs )
{
   data = rhs;
}

inline
ushort8::ushort8( bool8 rhs )
{
   data = (vec_ushort8)vec_and( (vec_ushort8)rhs, (vec_ushort8)(1) );
}

inline
ushort8::ushort8( short8 rhs )
{
   data = (vec_ushort8)(vec_short8)rhs;
}

inline
ushort8::ushort8( ushort r0, ushort r1, ushort r2, ushort r3,
                  ushort r4, ushort r5, ushort r6, ushort r7 )
{
   data = __extension__ (vec_ushort8){ r0, r1, r2, r3,
                                       r4, r5, r6, r7 };
}

inline
ushort8::operator vector unsigned short() const
{
   return data;
}

inline
ushort8_idx
ushort8::operator [] ( int i )
{
   return ushort8_idx(*this,i);
}

inline
ushort
ushort8::operator [] ( int i ) const
{
   return (ushort)_vec_extract((vec_ushort8)data, i);
}

inline
const ushort8
ushort8::operator ++ ( int )
{
   vec_ushort8 olddata = data;
   operator ++();
   return ushort8(olddata);
}

inline
const ushort8
ushort8::operator -- ( int )
{
   vec_ushort8 olddata = data;
   operator --();
   return ushort8(olddata);
}

inline
ushort8&
ushort8::operator ++ ()
{
   *this += ushort8(1);
   return *this;
}

inline
ushort8&
ushort8::operator -- ()
{
   *this -= ushort8(1);
   return *this;
}

inline
const ushort8 
ushort8::operator ~ () const
{
   return ushort8( vec_nor( data, data ) );
}

inline
const bool8
ushort8::operator ! () const
{
   return ( *this == ushort8(0) );
}

inline
const short8
ushort8::operator - () const
{
   return short8( (vec_short8)vec_sub( (vec_ushort8)(0), data ) );
}

inline
ushort8&
ushort8::operator = ( ushort8 rhs )
{
   data = rhs.data;
   return *this;
}

inline
ushort8&
ushort8::operator *= ( ushort8 rhs )
{
   *this = *this * rhs;
   return *this;
}

inline
ushort8&
ushort8::operator /= ( ushort8 rhs )
{
   *this = *this / rhs;
   return *this;
}

inline
ushort8&
ushort8::operator %= ( ushort8 rhs )
{
   *this = *this % rhs;
   return *this;
}

inline
ushort8&
ushort8::operator += ( ushort8 rhs )
{
   *this = *this + rhs;
   return *this;
}

inline
ushort8&
ushort8::operator -= ( ushort8 rhs )
{
   *this = *this - rhs;
   return *this;
}

inline
ushort8&
ushort8::operator <<= ( ushort8 rhs )
{
   *this = *this << rhs;
   return *this;
}

inline
ushort8&
ushort8::operator >>= ( ushort8 rhs )
{
   *this = *this >> rhs;
   return *this;
}

inline
ushort8&
ushort8::operator &= ( ushort8 rhs )
{
   *this = *this & rhs;
   return *this;
}

inline
ushort8&
ushort8::operator ^= ( ushort8 rhs )
{
   *this = *this ^ rhs;
   return *this;
}

inline
ushort8&
ushort8::operator |= ( ushort8 rhs )
{
   *this = *this | rhs;
   return *this;
}

inline
const ushort8
operator * ( ushort8 lhs, ushort8 rhs )
{
   return ushort8( vec_mladd( (vec_ushort8)lhs, (vec_ushort8)rhs, (vec_ushort8)(0) ) );
}

inline
const ushort8
operator / ( ushort8 num, ushort8 den )
{
   const vec_uchar16 pack = __extension__ (vec_uchar16)(vec_ushort8){ 0x0203, 0x1213, 0x0607, 0x1617, 
                                                                      0x0a0b, 0x1a1b, 0x0e0f, 0x1e1f };
   const vec_ushort8 zero = (vec_ushort8)(0);
   const vec_ushort8 one = (vec_ushort8)(1);
   const vec_uint4 mask = (vec_uint4)(0x0000ffff);
   const vec_uint4 k158 = (vec_uint4)(158);
   const vec_uint4 k142 = (vec_uint4)(142);
   const vec_uint4 k23 = (vec_uint4)(23);
   
   vec_ushort8 numer, denom;
   vec_uint4   hi_denom, lo_denom, hi_numer, lo_numer;
   vec_uint4   hi_denomZeros, lo_denomZeros, hi_numerZeros, lo_numerZeros;
   vec_ushort8 denomZeros, numerZeros, shift, denomShifted, oneShifted;
   vec_ushort8 quot, newQuot, skip, newNum, cont;
   int         anyCont;
       
   denom = (vec_ushort8)den;
   numer = (vec_ushort8)num;

   // to align denom with numer, get difference of leading zeros.
   
   hi_denom = vec_or( (vec_uint4)denom, mask );
   lo_denom = vec_and( (vec_uint4)denom, mask );
   hi_numer = vec_or( (vec_uint4)numer, mask );
   lo_numer = vec_and( (vec_uint4)numer, mask );
   
   hi_denomZeros = vec_sub( k158, vec_sr( (vec_uint4)vec_ctf( hi_denom, 0 ), k23 ) );
   lo_denomZeros = vec_sub( k142, vec_sr( (vec_uint4)vec_ctf( lo_denom, 0 ), k23 ) );
   hi_numerZeros = vec_sub( k158, vec_sr( (vec_uint4)vec_ctf( hi_numer, 0 ), k23 ) );
   lo_numerZeros = vec_sub( k142, vec_sr( (vec_uint4)vec_ctf( lo_numer, 0 ), k23 ) );
   
   denomZeros = (vec_ushort8)vec_perm( hi_denomZeros, lo_denomZeros, pack );
   numerZeros = (vec_ushort8)vec_perm( hi_numerZeros, lo_numerZeros, pack );
      
   shift = vec_sub( denomZeros, numerZeros );
   denomShifted = vec_sl( denom, shift );
   oneShifted = vec_sl( one, shift );
   oneShifted = vec_sel( oneShifted, zero, vec_or( vec_cmpeq( denom, zero ), vec_cmpgt( denom, numer ) ) );
   
   quot = zero;
   
   // long-division
   
   do
   {
      cont = (vec_ushort8)vec_cmpgt( oneShifted, zero );
      anyCont = vec_any_gt( oneShifted, zero );
      skip = (vec_ushort8)vec_cmpgt( denomShifted, numer );
      
      newQuot = vec_or( quot, oneShifted );
      newNum = vec_sub( numer, denomShifted );
      
      oneShifted = vec_sr( oneShifted, one );
      denomShifted = vec_sr( denomShifted, one );      
      
      quot = vec_sel( newQuot, quot, skip );
      numer = vec_sel( numer, newNum, vec_andc( cont, skip ) );
   }
   while ( anyCont );
   
   return ushort8( quot );
}

inline
const ushort8
operator % ( ushort8 num, ushort8 den )
{
   const vec_uchar16 pack = __extension__ (vec_uchar16)(vec_ushort8){ 0x0203, 0x1213, 0x0607, 0x1617, 
                                                                      0x0a0b, 0x1a1b, 0x0e0f, 0x1e1f };
   const vec_ushort8 zero = (vec_ushort8)(0);
   const vec_ushort8 one = (vec_ushort8)(1);
   const vec_uint4 mask = (vec_uint4)(0x0000ffff);
   const vec_uint4 k158 = (vec_uint4)(158);
   const vec_uint4 k142 = (vec_uint4)(142);
   const vec_uint4 k23 = (vec_uint4)(23);
   
   vec_ushort8 numer, denom;
   vec_uint4   hi_denom, lo_denom, hi_numer, lo_numer;
   vec_uint4   hi_denomZeros, lo_denomZeros, hi_numerZeros, lo_numerZeros;
   vec_ushort8 denomZeros, numerZeros, shift, denomShifted, oneShifted;
   vec_ushort8 quot, newQuot, skip, newNum, cont;
   int         anyCont;
       
   denom = (vec_ushort8)den;
   numer = (vec_ushort8)num;

   // to align denom with numer, get difference of leading zeros.
   
   hi_denom = vec_or( (vec_uint4)denom, mask );
   lo_denom = vec_and( (vec_uint4)denom, mask );
   hi_numer = vec_or( (vec_uint4)numer, mask );
   lo_numer = vec_and( (vec_uint4)numer, mask );
   
   hi_denomZeros = vec_sub( k158, vec_sr( (vec_uint4)vec_ctf( hi_denom, 0 ), k23 ) );
   lo_denomZeros = vec_sub( k142, vec_sr( (vec_uint4)vec_ctf( lo_denom, 0 ), k23 ) );
   hi_numerZeros = vec_sub( k158, vec_sr( (vec_uint4)vec_ctf( hi_numer, 0 ), k23 ) );
   lo_numerZeros = vec_sub( k142, vec_sr( (vec_uint4)vec_ctf( lo_numer, 0 ), k23 ) );
   
   denomZeros = (vec_ushort8)vec_perm( hi_denomZeros, lo_denomZeros, pack );
   numerZeros = (vec_ushort8)vec_perm( hi_numerZeros, lo_numerZeros, pack );
      
   shift = vec_sub( denomZeros, numerZeros );
   denomShifted = vec_sl( denom, shift );
   oneShifted = vec_sl( one, shift );
   oneShifted = vec_sel( oneShifted, zero, vec_or( vec_cmpeq( denom, zero ), vec_cmpgt( denom, numer ) ) );
   
   quot = zero;
   
   // long-division
   
   do
   {
      cont = (vec_ushort8)vec_cmpgt( oneShifted, zero );
      anyCont = vec_any_gt( oneShifted, zero );
      skip = (vec_ushort8)vec_cmpgt( denomShifted, numer );
      
      newQuot = vec_or( quot, oneShifted );
      newNum = vec_sub( numer, denomShifted );
      
      oneShifted = vec_sr( oneShifted, one );
      denomShifted = vec_sr( denomShifted, one );      
      
      quot = vec_sel( newQuot, quot, skip );
      numer = vec_sel( numer, newNum, vec_andc( cont, skip ) );
   }
   while ( anyCont );
   
   return ushort8( numer );
}

inline
const ushort8
operator + ( ushort8 lhs, ushort8 rhs )
{
   return ushort8( vec_add( (vec_ushort8)lhs, (vec_ushort8)rhs ) );
}

inline
const ushort8
operator - ( ushort8 lhs, ushort8 rhs )
{
   return ushort8( vec_sub( (vec_ushort8)lhs, (vec_ushort8)rhs ) );
}

inline
const ushort8
operator << ( ushort8 lhs, ushort8 rhs )
{
   return ushort8( vec_sl( (vec_ushort8)lhs, (vec_ushort8)rhs ) );
}

inline
const ushort8
operator >> ( ushort8 lhs, ushort8 rhs )
{
   return ushort8( vec_sr( (vec_ushort8)lhs, (vec_ushort8)rhs ) );
}

inline
const bool8
operator < ( ushort8 lhs, ushort8 rhs )
{
   bool8 result;
   result.data = (vec_ushort8)vec_cmpgt( (vec_ushort8)rhs, (vec_ushort8)lhs );
   return result;
}

inline
const bool8
operator <= ( ushort8 lhs, ushort8 rhs )
{
   return !( lhs > rhs );
}

inline
const bool8
operator > ( ushort8 lhs, ushort8 rhs )
{
   bool8 result;
   result.data = (vec_ushort8)vec_cmpgt( (vec_ushort8)lhs, (vec_ushort8)rhs );
   return result;
}

inline
const bool8
operator >= ( ushort8 lhs, ushort8 rhs )
{
   return !( lhs < rhs );
}

inline
const bool8
operator == ( ushort8 lhs, ushort8 rhs )
{
   bool8 result;
   result.data = (vec_ushort8)vec_cmpeq( (vec_ushort8)lhs, (vec_ushort8)rhs );
   return result;
}

inline
const bool8
operator != ( ushort8 lhs, ushort8 rhs )
{
   return !( lhs == rhs );
}
   
inline
const ushort8
select( ushort8 lhs, ushort8 rhs, bool8 rhs_slots )
{
   return ushort8( vec_sel( (vec_ushort8)lhs, (vec_ushort8)rhs, (vec_ushort8)rhs_slots ) );
}

inline
const ushort8
operator & ( ushort8 lhs, ushort8 rhs )
{
   return ushort8( vec_and( (vec_ushort8)lhs, (vec_ushort8)rhs ) );
}

inline
const ushort8
operator | ( ushort8 lhs, ushort8 rhs )
{
   return ushort8( vec_or( (vec_ushort8)lhs, (vec_ushort8)rhs ) );
}

inline
const ushort8
operator ^ ( ushort8 lhs, ushort8 rhs )
{
   return ushort8( vec_xor( (vec_ushort8)lhs, (vec_ushort8)rhs ) );
}
   
inline
ushort8_idx::operator ushort() const
{ 
   return (ushort)_vec_extract( (vec_ushort8)ref, i );
}

inline 
ushort
ushort8_idx::operator = ( ushort rhs )
{
   ref = _vec_insert( (ushort)rhs, (vec_ushort8)ref, i );
   return rhs;
}

inline
ushort
ushort8_idx::operator = ( const ushort8_idx& rhs )
{
   return ( *this = (ushort)rhs );
}

inline
ushort
ushort8_idx::operator ++( int )
{
   ushort tmp = _vec_extract( (vec_ushort8)ref, i );
   ref = _vec_insert( tmp+1, (vec_ushort8)ref, i );
   return tmp;
}

inline
ushort
ushort8_idx::operator --( int )
{
   ushort tmp = _vec_extract( (vec_ushort8)ref, i );
   ref = _vec_insert( tmp-1, (vec_ushort8)ref, i );
   return tmp;
}

inline
ushort
ushort8_idx::operator ++()
{
   ushort tmp = _vec_extract( (vec_ushort8)ref, i );
   ++tmp;
   ref = _vec_insert( tmp, (vec_ushort8)ref, i );
   return tmp;
}

inline
ushort
ushort8_idx::operator --()
{
   ushort tmp = _vec_extract( (vec_ushort8)ref, i );
   --tmp;
   ref = _vec_insert( tmp, (vec_ushort8)ref, i );
   return tmp;
}

inline
ushort
ushort8_idx::operator *= ( ushort rhs )
{
   ushort tmp = _vec_extract( (vec_ushort8)ref, i ) * rhs;
   ref = _vec_insert( tmp, (vec_ushort8)ref, i );
   return tmp;
}

inline
ushort
ushort8_idx::operator /= ( ushort rhs )
{
   ushort tmp = _vec_extract( (vec_ushort8)ref, i ) / rhs;
   ref = _vec_insert( tmp, (vec_ushort8)ref, i );
   return tmp;
}

inline
ushort
ushort8_idx::operator %= ( ushort rhs )
{
   ushort tmp = _vec_extract( (vec_ushort8)ref, i ) % rhs;
   ref = _vec_insert( tmp, (vec_ushort8)ref, i );
   return tmp;
}

inline
ushort
ushort8_idx::operator += ( ushort rhs )
{
   ushort tmp = _vec_extract( (vec_ushort8)ref, i ) + rhs;
   ref = _vec_insert( tmp, (vec_ushort8)ref, i );
   return tmp;
}

inline
ushort
ushort8_idx::operator -= ( ushort rhs )
{
   ushort tmp = _vec_extract( (vec_ushort8)ref, i ) - rhs;
   ref = _vec_insert( tmp, (vec_ushort8)ref, i );
   return tmp;
}

inline
ushort
ushort8_idx::operator <<= ( ushort rhs )
{
   ushort tmp = _vec_extract( (vec_ushort8)ref, i ) << rhs;
   ref = _vec_insert( tmp, (vec_ushort8)ref, i );
   return tmp;
}

inline
ushort
ushort8_idx::operator >>= ( ushort rhs )
{
   ushort tmp = _vec_extract( (vec_ushort8)ref, i ) >> rhs;
   ref = _vec_insert( tmp, (vec_ushort8)ref, i );
   return tmp;
}

inline
ushort
ushort8_idx::operator &= ( ushort rhs )
{
   ushort tmp = _vec_extract( (vec_ushort8)ref, i ) & rhs;
   ref = _vec_insert( tmp, (vec_ushort8)ref, i );
   return tmp;
}

inline
ushort
ushort8_idx::operator ^= ( ushort rhs )
{
   ushort tmp = _vec_extract( (vec_ushort8)ref, i ) ^ rhs;
   ref = _vec_insert( tmp, (vec_ushort8)ref, i );
   return tmp;
}

inline
ushort
ushort8_idx::operator |= ( ushort rhs )
{
   ushort tmp = _vec_extract( (vec_ushort8)ref, i ) | rhs;
   ref = _vec_insert( tmp, (vec_ushort8)ref, i );
   return tmp;
}

}; // namespace simd

#endif // ushort8_h
