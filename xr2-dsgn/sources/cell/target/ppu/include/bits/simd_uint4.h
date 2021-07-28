/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2008 Sony Computer Entertainment Inc. 
* All Rights Reserved.
*/

#ifndef _SCE_SIMD_UINT4_H
#define _SCE_SIMD_UINT4_H

#include <altivec.h>
#include "simd_scalars.h"

namespace simd {

class bool4;
class int4;
class float4;
class uint4_idx;

//--------------------------------------------------------------------------------------------------
// uint4 class
//

class uint4
{
   private:
      vec_uint4 data;

   public:
      inline uint4() {}
      inline uint4( bool4 );
      inline uint4( int4 );
      inline uint4( float4 );
      inline uint4( uint r0, uint r1, uint r2, uint r3 );

      // set all slots to same uint

      explicit inline uint4( uint );

      // allows implicit conversion from vector unsigned int

      inline uint4( vec_uint4 rhs );

      // allows implicit conversion to vector unsigned int

      inline operator vector unsigned int() const;

      // use array index operator to set and get slots

      inline uint4_idx operator [] ( int i );
      inline uint operator [] ( int i ) const;

      inline const uint4 operator ++ ( int );
      inline const uint4 operator -- ( int );
      inline uint4& operator ++ ();
      inline uint4& operator -- ();

      inline const uint4 operator ~ () const;
      inline const bool4 operator ! () const;
      inline const int4 operator - () const;

      inline uint4& operator = ( uint4 rhs );
      inline uint4& operator *= ( uint4 rhs );
      inline uint4& operator /= ( uint4 rhs );
      inline uint4& operator %= ( uint4 rhs );
      inline uint4& operator += ( uint4 rhs );
      inline uint4& operator -= ( uint4 rhs );
      inline uint4& operator <<= ( uint4 rhs );
      inline uint4& operator >>= ( uint4 rhs );
      inline uint4& operator &= ( uint4 rhs );
      inline uint4& operator ^= ( uint4 rhs );
      inline uint4& operator |= ( uint4 rhs );
};

//--------------------------------------------------------------------------------------------------
// functions for uint4
//

inline const uint4 operator * ( uint4 lhs, uint4 rhs );
inline const uint4 operator / ( uint4 lhs, uint4 rhs );
inline const uint4 operator % ( uint4 lhs, uint4 rhs );
inline const uint4 operator + ( uint4 lhs, uint4 rhs );
inline const uint4 operator - ( uint4 lhs, uint4 rhs );

inline const uint4 operator << ( uint4 lhs, uint4 rhs );
inline const uint4 operator >> ( uint4 lhs, uint4 rhs );

inline const bool4 operator < ( uint4 lhs, uint4 rhs );
inline const bool4 operator <= ( uint4 lhs, uint4 rhs );
inline const bool4 operator > ( uint4 lhs, uint4 rhs );
inline const bool4 operator >= ( uint4 lhs, uint4 rhs );
inline const bool4 operator == ( uint4 lhs, uint4 rhs );
inline const bool4 operator != ( uint4 lhs, uint4 rhs );

// select between lhs and rhs slots using bool4.
// false selects lhs slot, true selects rhs slot

inline const uint4 select( uint4 lhs, uint4 rhs, bool4 rhs_slots );

inline const uint4 operator & ( uint4 lhs, uint4 rhs );
inline const uint4 operator ^ ( uint4 lhs, uint4 rhs );
inline const uint4 operator | ( uint4 lhs, uint4 rhs );

//--------------------------------------------------------------------------------------------------
// uint4_idx: for setting uint4 slots with [] operator
//

class uint4_idx
{
   private:
      uint4 &ref __attribute__ ((aligned(16)));
      int i __attribute__ ((aligned(16)));
   public:
      inline uint4_idx( uint4& vec, int idx ): ref(vec) { i = idx; }
      inline operator uint() const;
      inline uint operator = ( uint rhs );
      inline uint operator = ( const uint4_idx& rhs );
      inline uint operator ++ ( int );
      inline uint operator -- ( int );
      inline uint operator ++ ();
      inline uint operator -- ();
      inline uint operator *= ( uint rhs );
      inline uint operator /= ( uint rhs );
      inline uint operator %= ( uint rhs );
      inline uint operator += ( uint rhs );
      inline uint operator -= ( uint rhs );
      inline uint operator <<= ( uint rhs );
      inline uint operator >>= ( uint rhs );
      inline uint operator &= ( uint rhs );
      inline uint operator ^= ( uint rhs );
      inline uint operator |= ( uint rhs );
};

}; // namespace simd

#include "simd_bool4.h"
#include "simd_int4.h"
#include "simd_float4.h"

namespace simd {

inline
uint4::uint4( uint rhs )
{
   uint tmp = (uint)rhs;
   data = __extension__ (vec_uint4){ tmp, tmp, tmp, tmp };
}

inline
uint4::uint4( vec_uint4 rhs )
{
   data = rhs;
}

inline
uint4::uint4( bool4 rhs )
{
   data = (vec_uint4)vec_and( (vec_uint4)rhs, (vec_uint4)(1) );
}

inline
uint4::uint4( int4 rhs )
{
   data = (vec_uint4)(vec_int4)rhs;
}

inline
uint4::uint4( float4 rhs )
{
   data = vec_ctu( (vec_float4)rhs, 0 );
}

inline
uint4::uint4( uint r0, uint r1, uint r2, uint r3 )
{
   data = __extension__ (vec_uint4){ r0, r1, r2, r3 };
}

inline
uint4::operator vector unsigned int() const
{
   return data;
}

inline
uint4_idx
uint4::operator [] ( int i )
{
   return uint4_idx(*this,i);
}

inline
uint
uint4::operator [] ( int i ) const
{
   return (uint)_vec_extract((vec_uint4)data, i);
}

inline
const uint4
uint4::operator ++ ( int )
{
   vec_uint4 olddata = data;
   operator ++();
   return uint4(olddata);
}

inline
const uint4
uint4::operator -- ( int )
{
   vec_uint4 olddata = data;
   operator --();
   return uint4(olddata);
}

inline
uint4&
uint4::operator ++ ()
{
   *this += uint4(1);
   return *this;
}

inline
uint4&
uint4::operator -- ()
{
   *this -= uint4(1);
   return *this;
}

inline
const uint4 
uint4::operator ~ () const
{
   return uint4( vec_nor( data, data ) );
}

inline
const bool4
uint4::operator ! () const
{
   return ( *this == uint4(0) );
}

inline
const int4
uint4::operator - () const
{
   return int4( (vec_int4)vec_sub( (vec_uint4)(0), data ) );
}

inline
uint4&
uint4::operator = ( uint4 rhs )
{
   data = rhs.data;
   return *this;
}

inline
uint4&
uint4::operator *= ( uint4 rhs )
{
   *this = *this * rhs;
   return *this;
}

inline
uint4&
uint4::operator /= ( uint4 rhs )
{
   *this = *this / rhs;
   return *this;
}

inline
uint4&
uint4::operator %= ( uint4 rhs )
{
   *this = *this % rhs;
   return *this;
}

inline
uint4&
uint4::operator += ( uint4 rhs )
{
   *this = *this + rhs;
   return *this;
}

inline
uint4&
uint4::operator -= ( uint4 rhs )
{
   *this = *this - rhs;
   return *this;
}

inline
uint4&
uint4::operator <<= ( uint4 rhs )
{
   *this = *this << rhs;
   return *this;
}

inline
uint4&
uint4::operator >>= ( uint4 rhs )
{
   *this = *this >> rhs;
   return *this;
}

inline
uint4&
uint4::operator &= ( uint4 rhs )
{
   *this = *this & rhs;
   return *this;
}

inline
uint4&
uint4::operator ^= ( uint4 rhs )
{
   *this = *this ^ rhs;
   return *this;
}

inline
uint4&
uint4::operator |= ( uint4 rhs )
{
   *this = *this | rhs;
   return *this;
}

inline
const uint4
operator * ( uint4 lhs, uint4 rhs )
{
   const vec_uint4 sixteen = (vec_uint4)(16);
   
   vec_ushort8 lhs_short = (vec_ushort8)(vec_uint4)lhs;
   vec_ushort8 rhs_short = (vec_ushort8)(vec_uint4)rhs;

   vec_uint4 cross_mul = (vec_uint4)vec_msum( lhs_short, (vec_ushort8)vec_rl( (vec_uint4)rhs, sixteen ), (vec_uint4)(0) );
   vec_uint4 lo_mul = (vec_uint4)vec_mulo( lhs_short, rhs_short );
   
   return uint4( (vec_uint4)vec_add( vec_sl( (vec_uint4)cross_mul, sixteen ), lo_mul ) );
}

inline
const uint4
operator / ( uint4 num, uint4 den )
{
   const vec_uint4 zero = (vec_uint4)(0);
   const vec_uint4 one = (vec_uint4)(1);
   const vec_uint4 k158 = (vec_uint4)(158);
   const vec_uint4 k23 = (vec_uint4)(23);
   
   vec_uint4 numer, denom;
   vec_uint4 denomZeros, numerZeros, shift, denomShifted, oneShifted;
   vec_uint4 quot, newQuot, skip, newNum, cont;
   int       anyCont;
       
   denom = (vec_uint4)den;
   numer = (vec_uint4)num;
   
   // to align denom with numer, get difference of leading zeros.
   // find leading zeros by converting to float and using exponent
   
   denomZeros = vec_sub( k158, vec_sr( (vec_uint4)vec_ctf( denom, 0 ), k23 ) );
   numerZeros = vec_sub( k158, vec_sr( (vec_uint4)vec_ctf( numer, 0 ), k23 ) );
      
   shift = vec_sub( denomZeros, numerZeros );
   denomShifted = vec_sl( denom, shift );
   oneShifted = vec_sl( one, shift );
   oneShifted = vec_sel( oneShifted, zero, vec_or( vec_cmpeq( denom, zero ), 
                                                   vec_cmpgt( denom, numer ) ) );
   
   quot = zero;
   
   // long-division
   
   do
   {
      cont = (vec_uint4)vec_cmpgt( oneShifted, zero );
      anyCont = vec_any_gt( oneShifted, zero );
      skip = (vec_uint4)vec_cmpgt( denomShifted, numer );
      
      newQuot = vec_or( quot, oneShifted );
      newNum = vec_sub( numer, denomShifted );
      
      oneShifted = vec_sr( oneShifted, one );
      denomShifted = vec_sr( denomShifted, one );
      
      quot = vec_sel( newQuot, quot, skip );
      numer = vec_sel( numer, newNum, vec_andc( cont, skip ) );
   }
   while ( anyCont );
   
   return uint4( quot );
}

inline
const uint4
operator % ( uint4 num, uint4 den )
{
   const vec_uint4 zero = (vec_uint4)(0);
   const vec_uint4 one = (vec_uint4)(1);
   const vec_uint4 k158 = (vec_uint4)(158);
   const vec_uint4 k23 = (vec_uint4)(23);
   
   vec_uint4 numer, denom;
   vec_uint4 denomZeros, numerZeros, shift, denomShifted, oneShifted;
   vec_uint4 quot, newQuot, skip, newNum, cont;
   int       anyCont;
       
   denom = (vec_uint4)den;
   numer = (vec_uint4)num;
   
   // to align denom with numer, get difference of leading zeros.
   // find leading zeros by converting to float and using exponent
   
   denomZeros = vec_sub( k158, vec_sr( (vec_uint4)vec_ctf( denom, 0 ), k23 ) );
   numerZeros = vec_sub( k158, vec_sr( (vec_uint4)vec_ctf( numer, 0 ), k23 ) );
      
   shift = vec_sub( denomZeros, numerZeros );
   denomShifted = vec_sl( denom, shift );
   oneShifted = vec_sl( one, shift );
   oneShifted = vec_sel( oneShifted, zero, vec_or( vec_cmpeq( denom, zero ), 
                                                   vec_cmpgt( denom, numer ) ) );
   
   quot = zero;
   
   // long-division
   
   do
   {
      cont = (vec_uint4)vec_cmpgt( oneShifted, zero );
      anyCont = vec_any_gt( oneShifted, zero );
      skip = (vec_uint4)vec_cmpgt( denomShifted, numer );
      
      newQuot = vec_or( quot, oneShifted );
      newNum = vec_sub( numer, denomShifted );
      
      oneShifted = vec_sr( oneShifted, one );
      denomShifted = vec_sr( denomShifted, one );
      
      quot = vec_sel( newQuot, quot, skip );
      numer = vec_sel( numer, newNum, vec_andc( cont, skip ) );
   }
   while ( anyCont );
   
   return uint4( numer );
}

inline
const uint4
operator + ( uint4 lhs, uint4 rhs )
{
   return uint4( vec_add( (vec_uint4)lhs, (vec_uint4)rhs ) );
}

inline
const uint4
operator - ( uint4 lhs, uint4 rhs )
{
   return uint4( vec_sub( (vec_uint4)lhs, (vec_uint4)rhs ) );
}

inline
const uint4
operator << ( uint4 lhs, uint4 rhs )
{
   return uint4( vec_sl( (vec_uint4)lhs, (vec_uint4)rhs ) );
}

inline
const uint4
operator >> ( uint4 lhs, uint4 rhs )
{
   return uint4( vec_sr( (vec_uint4)lhs, (vec_uint4)rhs ) );
}

inline
const bool4
operator < ( uint4 lhs, uint4 rhs )
{
   bool4 result;
   result.data = (vec_uint4)vec_cmpgt( (vec_uint4)rhs, (vec_uint4)lhs );
   return result;
}

inline
const bool4
operator <= ( uint4 lhs, uint4 rhs )
{
   return !( lhs > rhs );
}

inline
const bool4
operator > ( uint4 lhs, uint4 rhs )
{
   bool4 result;
   result.data = (vec_uint4)vec_cmpgt( (vec_uint4)lhs, (vec_uint4)rhs );
   return result;
}

inline
const bool4
operator >= ( uint4 lhs, uint4 rhs )
{
   return !( lhs < rhs );
}

inline
const bool4
operator == ( uint4 lhs, uint4 rhs )
{
   bool4 result;
   result.data = (vec_uint4)vec_cmpeq( (vec_uint4)lhs, (vec_uint4)rhs );
   return result;
}

inline
const bool4
operator != ( uint4 lhs, uint4 rhs )
{
   return !( lhs == rhs );
}
   
inline
const uint4
select( uint4 lhs, uint4 rhs, bool4 rhs_slots )
{
   return uint4( vec_sel( (vec_uint4)lhs, (vec_uint4)rhs, (vec_uint4)rhs_slots ) );
}

inline
const uint4
operator & ( uint4 lhs, uint4 rhs )
{
   return uint4( vec_and( (vec_uint4)lhs, (vec_uint4)rhs ) );
}

inline
const uint4
operator | ( uint4 lhs, uint4 rhs )
{
   return uint4( vec_or( (vec_uint4)lhs, (vec_uint4)rhs ) );
}

inline
const uint4
operator ^ ( uint4 lhs, uint4 rhs )
{
   return uint4( vec_xor( (vec_uint4)lhs, (vec_uint4)rhs ) );
}
   
inline
uint4_idx::operator uint() const
{ 
   return (uint)_vec_extract( (vec_uint4)ref, i );
}

inline 
uint
uint4_idx::operator = ( uint rhs )
{
   ref = _vec_insert( (uint)rhs, (vec_uint4)ref, i );
   return rhs;
}

inline
uint
uint4_idx::operator = ( const uint4_idx& rhs )
{
   return ( *this = (uint)rhs );
}

inline
uint
uint4_idx::operator ++( int )
{
   uint tmp = _vec_extract( (vec_uint4)ref, i );
   ref = _vec_insert( tmp+1, (vec_uint4)ref, i );
   return tmp;
}

inline
uint
uint4_idx::operator --( int )
{
   uint tmp = _vec_extract( (vec_uint4)ref, i );
   ref = _vec_insert( tmp-1, (vec_uint4)ref, i );
   return tmp;
}

inline
uint
uint4_idx::operator ++()
{
   uint tmp = _vec_extract( (vec_uint4)ref, i );
   ++tmp;
   ref = _vec_insert( tmp, (vec_uint4)ref, i );
   return tmp;
}

inline
uint
uint4_idx::operator --()
{
   uint tmp = _vec_extract( (vec_uint4)ref, i );
   --tmp;
   ref = _vec_insert( tmp, (vec_uint4)ref, i );
   return tmp;
}

inline
uint
uint4_idx::operator *= ( uint rhs )
{
   uint tmp = _vec_extract( (vec_uint4)ref, i ) * rhs;
   ref = _vec_insert( tmp, (vec_uint4)ref, i );
   return tmp;
}

inline
uint
uint4_idx::operator /= ( uint rhs )
{
   uint tmp = _vec_extract( (vec_uint4)ref, i ) / rhs;
   ref = _vec_insert( tmp, (vec_uint4)ref, i );
   return tmp;
}

inline
uint
uint4_idx::operator %= ( uint rhs )
{
   uint tmp = _vec_extract( (vec_uint4)ref, i ) % rhs;
   ref = _vec_insert( tmp, (vec_uint4)ref, i );
   return tmp;
}

inline
uint
uint4_idx::operator += ( uint rhs )
{
   uint tmp = _vec_extract( (vec_uint4)ref, i ) + rhs;
   ref = _vec_insert( tmp, (vec_uint4)ref, i );
   return tmp;
}

inline
uint
uint4_idx::operator -= ( uint rhs )
{
   uint tmp = _vec_extract( (vec_uint4)ref, i ) - rhs;
   ref = _vec_insert( tmp, (vec_uint4)ref, i );
   return tmp;
}

inline
uint
uint4_idx::operator <<= ( uint rhs )
{
   uint tmp = _vec_extract( (vec_uint4)ref, i ) << rhs;
   ref = _vec_insert( tmp, (vec_uint4)ref, i );
   return tmp;
}

inline
uint
uint4_idx::operator >>= ( uint rhs )
{
   uint tmp = _vec_extract( (vec_uint4)ref, i ) >> rhs;
   ref = _vec_insert( tmp, (vec_uint4)ref, i );
   return tmp;
}

inline
uint
uint4_idx::operator &= ( uint rhs )
{
   uint tmp = _vec_extract( (vec_uint4)ref, i ) & rhs;
   ref = _vec_insert( tmp, (vec_uint4)ref, i );
   return tmp;
}

inline
uint
uint4_idx::operator ^= ( uint rhs )
{
   uint tmp = _vec_extract( (vec_uint4)ref, i ) ^ rhs;
   ref = _vec_insert( tmp, (vec_uint4)ref, i );
   return tmp;
}

inline
uint
uint4_idx::operator |= ( uint rhs )
{
   uint tmp = _vec_extract( (vec_uint4)ref, i ) | rhs;
   ref = _vec_insert( tmp, (vec_uint4)ref, i );
   return tmp;
}

}; // namespace simd

#endif // uint4_h
