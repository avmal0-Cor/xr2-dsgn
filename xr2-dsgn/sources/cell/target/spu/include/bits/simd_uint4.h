/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2008 Sony Computer Entertainment Inc. 
* All Rights Reserved.
*/

#ifndef _SCE_SIMD_UINT4_H
#define _SCE_SIMD_UINT4_H

#include <spu_intrinsics.h>
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
   data = spu_splats((uint)rhs);
}

inline
uint4::uint4( vec_uint4 rhs )
{
   data = rhs;
}

inline
uint4::uint4( bool4 rhs )
{
   data = (vec_uint4)spu_and( (vec_uint4)rhs, 1 );
}

inline
uint4::uint4( int4 rhs )
{
   data = (vec_uint4)(vec_int4)rhs;
}

inline
uint4::uint4( float4 rhs )
{
   data = spu_convtu( (vec_float4)rhs, 0 );
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
   return (uint)spu_extract((vec_uint4)data, i);
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
   return uint4( spu_nor( data, data ) );
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
   return int4( (vec_int4)spu_sub( (uint)0, data ) );
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
   vec_short8 lhs_short = (vec_short8)(vec_uint4)lhs;
   vec_short8 rhs_short = (vec_short8)(vec_uint4)rhs;
   
   return uint4( (vec_uint4)spu_add( spu_add( spu_mulh( lhs_short, rhs_short ),
                                             spu_mulh( rhs_short, lhs_short ) ),
                                    (vec_int4)spu_mulo( (vec_ushort8)lhs_short, (vec_ushort8)rhs_short ) ) );
}

inline
const uint4
operator / ( uint4 num, uint4 den )
{
   const vec_uint4 zero = spu_splats((uint)0);
   const vec_uint4 one = spu_splats((uint)1);
   
   vec_uint4 numer, denom;
   vec_uint4 denomZeros, numerZeros, shift, denomLeft, oneLeft, denomShifted, oneShifted;
   vec_uint4 quot, newQuot, newNum, skip, cont;
   int       anyCont;
                                                                                                                                                                       
   denom = (vec_uint4)den;
   numer = (vec_uint4)num;
   
   // to align denom with numer, get difference of leading zeros.
   // any possible negative value will make shifted results 0.
                                                                                                                                                                       
   denomZeros = spu_cntlz( denom );
   numerZeros = spu_cntlz( numer );
                                                                                                                                                                       
   shift = spu_sub( denomZeros, numerZeros );
                                                                                                                                                                       
   // shift denom to align leading one with numerator's
                                                                                                                                                                       
   denomShifted = spu_sl( denom, shift );
   oneShifted = spu_sl( one, shift );
   oneShifted = spu_sel( oneShifted, zero, spu_cmpeq( denom, 0 ) );
                                                                                                                                                                       
   // shift left all leading zeros
                                                                                                                                                                       
   denomLeft = spu_sl( denom, denomZeros );
   oneLeft = spu_sl( one, denomZeros );
                                                                                                                                                                       
   quot = zero;

   // long-division
   
   do
   {
      cont = spu_cmpgt( oneShifted, 0 );
      anyCont = spu_extract( spu_gather( cont ), 0 );
                                                                                                                                                                       
      newQuot = spu_or( quot, oneShifted );
                                                                                                                                                                       
      // subtract shifted denominator from remaining numerator
      // when denominator is not greater
                                                                                                                                                                       
      skip = spu_cmpgt( denomShifted, numer );
      newNum = spu_sub( numer, denomShifted );
                                                                                                                                                                       
      // if denominator is greater, next shift is one more, otherwise
      // next shift is number of leading zeros of remaining numerator
      
      numerZeros = spu_sel( spu_cntlz( newNum ), numerZeros, skip );
      shift = spu_sub( skip, numerZeros );
                                                                                                                                                                       
      oneShifted = spu_rlmask( oneLeft, (vec_int4)shift );
      denomShifted = spu_rlmask( denomLeft, (vec_int4)shift );
                                                                                                                                                                       
      quot = spu_sel( newQuot, quot, skip );
      numer = spu_sel( newNum, numer, spu_orc(skip,cont) );
   }
   while ( anyCont );
      
   return uint4( quot );
}

inline
const uint4
operator % ( uint4 num, uint4 den )
{
   const vec_uint4 zero = spu_splats((uint)0);
   const vec_uint4 one = spu_splats((uint)1);
   
   vec_uint4 numer, denom;
   vec_uint4 denomZeros, numerZeros, shift, denomLeft, oneLeft, denomShifted, oneShifted;
   vec_uint4 quot, newQuot, newNum, skip, cont;
   int       anyCont;
                                                                                                                                                                       
   denom = (vec_uint4)den;
   numer = (vec_uint4)num;
   
   // to align denom with numer, get difference of leading zeros.
   // any possible negative value will make shifted results 0.
                                                                                                                                                                       
   denomZeros = spu_cntlz( denom );
   numerZeros = spu_cntlz( numer );
                                                                                                                                                                       
   shift = spu_sub( denomZeros, numerZeros );
                                                                                                                                                                       
   // shift denom to align leading one with numerator's
                                                                                                                                                                       
   denomShifted = spu_sl( denom, shift );
   oneShifted = spu_sl( one, shift );
   oneShifted = spu_sel( oneShifted, zero, spu_cmpeq( denom, 0 ) );
                                                                                                                                                                       
   // shift left all leading zeros
                                                                                                                                                                       
   denomLeft = spu_sl( denom, denomZeros );
   oneLeft = spu_sl( one, denomZeros );
                                                                                                                                                                       
   quot = zero;

   // long-division
   
   do
   {
      cont = spu_cmpgt( oneShifted, 0 );
      anyCont = spu_extract( spu_gather( cont ), 0 );
                                                                                                                                                                       
      newQuot = spu_or( quot, oneShifted );
                                                                                                                                                                       
      // subtract shifted denominator from remaining numerator
      // when denominator is not greater
                                                                                                                                                                       
      skip = spu_cmpgt( denomShifted, numer );
      newNum = spu_sub( numer, denomShifted );
                                                                                                                                                                       
      // if denominator is greater, next shift is one more, otherwise
      // next shift is number of leading zeros of remaining numerator
      
      numerZeros = spu_sel( spu_cntlz( newNum ), numerZeros, skip );
      shift = spu_sub( skip, numerZeros );
                                                                                                                                                                       
      oneShifted = spu_rlmask( oneLeft, (vec_int4)shift );
      denomShifted = spu_rlmask( denomLeft, (vec_int4)shift );
                                                                                                                                                                       
      quot = spu_sel( newQuot, quot, skip );
      numer = spu_sel( newNum, numer, spu_orc(skip,cont) );
   }
   while ( anyCont );
      
   return uint4( numer );
}

inline
const uint4
operator + ( uint4 lhs, uint4 rhs )
{
   return uint4( spu_add( (vec_uint4)lhs, (vec_uint4)rhs ) );
}

inline
const uint4
operator - ( uint4 lhs, uint4 rhs )
{
   return uint4( spu_sub( (vec_uint4)lhs, (vec_uint4)rhs ) );
}

inline
const uint4
operator << ( uint4 lhs, uint4 rhs )
{
   return uint4( spu_sl( (vec_uint4)lhs, (vec_uint4)rhs ) );
}

inline
const uint4
operator >> ( uint4 lhs, uint4 rhs )
{
   return uint4( spu_rlmask( (vec_uint4)lhs, (vec_int4)-rhs ) );
}

inline
const bool4
operator < ( uint4 lhs, uint4 rhs )
{
   bool4 result;
   result.data = spu_cmpgt( (vec_uint4)rhs, (vec_uint4)lhs );
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
   result.data = spu_cmpgt( (vec_uint4)lhs, (vec_uint4)rhs );
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
   result.data = spu_cmpeq( (vec_uint4)lhs, (vec_uint4)rhs );
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
   return uint4( spu_sel( (vec_uint4)lhs, (vec_uint4)rhs, (vec_uint4)rhs_slots ) );
}

inline
const uint4
operator & ( uint4 lhs, uint4 rhs )
{
   return uint4( spu_and( (vec_uint4)lhs, (vec_uint4)rhs ) );
}

inline
const uint4
operator | ( uint4 lhs, uint4 rhs )
{
   return uint4( spu_or( (vec_uint4)lhs, (vec_uint4)rhs ) );
}

inline
const uint4
operator ^ ( uint4 lhs, uint4 rhs )
{
   return uint4( spu_xor( (vec_uint4)lhs, (vec_uint4)rhs ) );
}
   
inline
uint4_idx::operator uint() const
{ 
   return (uint)spu_extract( (vec_uint4)ref, i );
}

inline 
uint
uint4_idx::operator = ( uint rhs )
{
   ref = spu_insert( (uint)rhs, (vec_uint4)ref, i );
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
   uint tmp = spu_extract( (vec_uint4)ref, i );
   ref = spu_insert( tmp+1, (vec_uint4)ref, i );
   return tmp;
}

inline
uint
uint4_idx::operator --( int )
{
   uint tmp = spu_extract( (vec_uint4)ref, i );
   ref = spu_insert( tmp-1, (vec_uint4)ref, i );
   return tmp;
}

inline
uint
uint4_idx::operator ++()
{
   uint tmp = spu_extract( (vec_uint4)ref, i );
   ++tmp;
   ref = spu_insert( tmp, (vec_uint4)ref, i );
   return tmp;
}

inline
uint
uint4_idx::operator --()
{
   uint tmp = spu_extract( (vec_uint4)ref, i );
   --tmp;
   ref = spu_insert( tmp, (vec_uint4)ref, i );
   return tmp;
}

inline
uint
uint4_idx::operator *= ( uint rhs )
{
   uint tmp = spu_extract( (vec_uint4)ref, i ) * rhs;
   ref = spu_insert( tmp, (vec_uint4)ref, i );
   return tmp;
}

inline
uint
uint4_idx::operator /= ( uint rhs )
{
   uint tmp = spu_extract( (vec_uint4)ref, i ) / rhs;
   ref = spu_insert( tmp, (vec_uint4)ref, i );
   return tmp;
}

inline
uint
uint4_idx::operator %= ( uint rhs )
{
   uint tmp = spu_extract( (vec_uint4)ref, i ) % rhs;
   ref = spu_insert( tmp, (vec_uint4)ref, i );
   return tmp;
}

inline
uint
uint4_idx::operator += ( uint rhs )
{
   uint tmp = spu_extract( (vec_uint4)ref, i ) + rhs;
   ref = spu_insert( tmp, (vec_uint4)ref, i );
   return tmp;
}

inline
uint
uint4_idx::operator -= ( uint rhs )
{
   uint tmp = spu_extract( (vec_uint4)ref, i ) - rhs;
   ref = spu_insert( tmp, (vec_uint4)ref, i );
   return tmp;
}

inline
uint
uint4_idx::operator <<= ( uint rhs )
{
   uint tmp = spu_extract( (vec_uint4)ref, i ) << rhs;
   ref = spu_insert( tmp, (vec_uint4)ref, i );
   return tmp;
}

inline
uint
uint4_idx::operator >>= ( uint rhs )
{
   uint tmp = spu_extract( (vec_uint4)ref, i ) >> rhs;
   ref = spu_insert( tmp, (vec_uint4)ref, i );
   return tmp;
}

inline
uint
uint4_idx::operator &= ( uint rhs )
{
   uint tmp = spu_extract( (vec_uint4)ref, i ) & rhs;
   ref = spu_insert( tmp, (vec_uint4)ref, i );
   return tmp;
}

inline
uint
uint4_idx::operator ^= ( uint rhs )
{
   uint tmp = spu_extract( (vec_uint4)ref, i ) ^ rhs;
   ref = spu_insert( tmp, (vec_uint4)ref, i );
   return tmp;
}

inline
uint
uint4_idx::operator |= ( uint rhs )
{
   uint tmp = spu_extract( (vec_uint4)ref, i ) | rhs;
   ref = spu_insert( tmp, (vec_uint4)ref, i );
   return tmp;
}

}; // namespace simd

#endif // uint4_h
