/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2008 Sony Computer Entertainment Inc. 
* All Rights Reserved.
*/

#ifndef _SCE_SIMD_USHORT8_H
#define _SCE_SIMD_USHORT8_H

#include <spu_intrinsics.h>
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
   data = spu_splats((ushort)rhs);
}

inline
ushort8::ushort8( vec_ushort8 rhs )
{
   data = rhs;
}

inline
ushort8::ushort8( bool8 rhs )
{
   data = (vec_ushort8)spu_and( (vec_ushort8)rhs, 1 );
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
   return (ushort)spu_extract((vec_ushort8)data, i);
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
   return ushort8( spu_nor( data, data ) );
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
   return short8( (vec_short8)spu_sub( (ushort)0, data ) );
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
   const vec_uchar16 pack = __extension__ (vec_uchar16)(vec_ushort8){ 0x0203, 0x1213, 0x0607, 0x1617, 
                                                                      0x0a0b, 0x1a1b, 0x0e0f, 0x1e1f };

   vec_short8 lhs_short = (vec_short8)(vec_ushort8)lhs;
   vec_short8 rhs_short = (vec_short8)(vec_ushort8)rhs;
   
   vec_int4 lo_mul = spu_mulo( lhs_short, rhs_short );
   vec_int4 hi_mul = spu_mule( lhs_short, rhs_short );

   return ushort8( (vec_ushort8)spu_shuffle( hi_mul, lo_mul, pack ) );
}

inline
const ushort8
operator / ( ushort8 num, ushort8 den )
{
   const vec_uchar16 pack = __extension__ (vec_uchar16)(vec_ushort8){ 0x0203, 0x1213, 0x0607, 0x1617, 
                                                                      0x0a0b, 0x1a1b, 0x0e0f, 0x1e1f };
   const vec_ushort8 mask = (vec_ushort8)spu_maskb( 0x3333 );
   const vec_ushort8 zero = spu_splats((ushort)0);
   const vec_ushort8 one = spu_splats((ushort)1);
   const vec_uint4 sixteen = spu_splats((uint)16);
   
   vec_ushort8 numer, denom;
   vec_ushort8 hi_denom, lo_denom, hi_numer, lo_numer;
   vec_uint4   hi_denomZeros, lo_denomZeros, hi_numerZeros, lo_numerZeros;
   vec_ushort8 denomZeros, numerZeros, shift, denomShifted, oneShifted;
   vec_ushort8 quot, newQuot, skip, newNum, cont;
   uint        anyCont;
       
   denom = (vec_ushort8)den;
   numer = (vec_ushort8)num;
   
   // to align denom with numer, get difference of leading zeros.
   // any possible negative value will make shifted results 0.
   
   hi_denom = spu_or( denom, mask );
   lo_denom = spu_and( denom, mask );
   hi_numer = spu_or( numer, mask );
   lo_numer = spu_and( numer, mask );
   
   hi_denomZeros = spu_cntlz( (vec_uint4)hi_denom );
   lo_denomZeros = spu_sub( spu_cntlz( (vec_uint4)lo_denom ), sixteen );
   hi_numerZeros = spu_cntlz( (vec_uint4)hi_numer );
   lo_numerZeros = spu_sub( spu_cntlz( (vec_uint4)lo_numer ), sixteen );
   
   denomZeros = (vec_ushort8)spu_shuffle( hi_denomZeros, lo_denomZeros, pack );
   numerZeros = (vec_ushort8)spu_shuffle( hi_numerZeros, lo_numerZeros, pack );
   
   shift = spu_sub( denomZeros, numerZeros );
   denomShifted = spu_sl( denom, shift );
   oneShifted = spu_sl( one, shift );
   oneShifted = spu_sel( oneShifted, zero, spu_cmpeq( denom, 0 ) );
   
   quot = zero;
   
   // long-division
   
   do
   {
      cont = spu_cmpgt( oneShifted, 0 );
      anyCont = spu_extract( spu_gather( cont ), 0 );
      skip = spu_cmpgt( denomShifted, numer );
   
      newQuot = spu_or( quot, oneShifted );
      newNum = spu_sub( numer, denomShifted );
      
      oneShifted = spu_rlmask( oneShifted, -1 );
      denomShifted = spu_rlmask( denomShifted, -1 );
      
      quot = spu_sel( newQuot, quot, skip );
      numer = spu_sel( newNum, numer, spu_orc( skip, cont ) );
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
   const vec_ushort8 mask = (vec_ushort8)spu_maskb( 0x3333 );
   const vec_ushort8 zero = spu_splats((ushort)0);
   const vec_ushort8 one = spu_splats((ushort)1);
   const vec_uint4 sixteen = spu_splats((uint)16);
   
   vec_ushort8 numer, denom;
   vec_ushort8 hi_denom, lo_denom, hi_numer, lo_numer;
   vec_uint4   hi_denomZeros, lo_denomZeros, hi_numerZeros, lo_numerZeros;
   vec_ushort8 denomZeros, numerZeros, shift, denomShifted, oneShifted;
   vec_ushort8 quot, newQuot, skip, newNum, cont;
   uint        anyCont;
       
   denom = (vec_ushort8)den;
   numer = (vec_ushort8)num;
   
   // to align denom with numer, get difference of leading zeros.
   // any possible negative value will make shifted results 0.
   
   hi_denom = spu_or( denom, mask );
   lo_denom = spu_and( denom, mask );
   hi_numer = spu_or( numer, mask );
   lo_numer = spu_and( numer, mask );
   
   hi_denomZeros = spu_cntlz( (vec_uint4)hi_denom );
   lo_denomZeros = spu_sub( spu_cntlz( (vec_uint4)lo_denom ), sixteen );
   hi_numerZeros = spu_cntlz( (vec_uint4)hi_numer );
   lo_numerZeros = spu_sub( spu_cntlz( (vec_uint4)lo_numer ), sixteen );
   
   denomZeros = (vec_ushort8)spu_shuffle( hi_denomZeros, lo_denomZeros, pack );
   numerZeros = (vec_ushort8)spu_shuffle( hi_numerZeros, lo_numerZeros, pack );
   
   shift = spu_sub( denomZeros, numerZeros );
   denomShifted = spu_sl( denom, shift );
   oneShifted = spu_sl( one, shift );
   oneShifted = spu_sel( oneShifted, zero, spu_cmpeq( denom, 0 ) );
   
   quot = zero;
   
   // long-division
   
   do
   {
      cont = spu_cmpgt( oneShifted, 0 );
      anyCont = spu_extract( spu_gather( cont ), 0 );
      skip = spu_cmpgt( denomShifted, numer );
   
      newQuot = spu_or( quot, oneShifted );
      newNum = spu_sub( numer, denomShifted );
      
      oneShifted = spu_rlmask( oneShifted, -1 );
      denomShifted = spu_rlmask( denomShifted, -1 );
      
      quot = spu_sel( newQuot, quot, skip );
      numer = spu_sel( newNum, numer, spu_orc( skip, cont ) );
   }
   while ( anyCont );
   
   return ushort8( numer );
}

inline
const ushort8
operator + ( ushort8 lhs, ushort8 rhs )
{
   return ushort8( spu_add( (vec_ushort8)lhs, (vec_ushort8)rhs ) );
}

inline
const ushort8
operator - ( ushort8 lhs, ushort8 rhs )
{
   return ushort8( spu_sub( (vec_ushort8)lhs, (vec_ushort8)rhs ) );
}

inline
const ushort8
operator << ( ushort8 lhs, ushort8 rhs )
{
   return ushort8( spu_sl( (vec_ushort8)lhs, (vec_ushort8)rhs ) );
}

inline
const ushort8
operator >> ( ushort8 lhs, ushort8 rhs )
{
   return ushort8( spu_rlmask( (vec_ushort8)lhs, (vec_short8)-rhs ) );
}

inline
const bool8
operator < ( ushort8 lhs, ushort8 rhs )
{
   bool8 result;
   result.data = spu_cmpgt( (vec_ushort8)rhs, (vec_ushort8)lhs );
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
   result.data = spu_cmpgt( (vec_ushort8)lhs, (vec_ushort8)rhs );
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
   result.data = spu_cmpeq( (vec_ushort8)lhs, (vec_ushort8)rhs );
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
   return ushort8( spu_sel( (vec_ushort8)lhs, (vec_ushort8)rhs, (vec_ushort8)rhs_slots ) );
}

inline
const ushort8
operator & ( ushort8 lhs, ushort8 rhs )
{
   return ushort8( spu_and( (vec_ushort8)lhs, (vec_ushort8)rhs ) );
}

inline
const ushort8
operator | ( ushort8 lhs, ushort8 rhs )
{
   return ushort8( spu_or( (vec_ushort8)lhs, (vec_ushort8)rhs ) );
}

inline
const ushort8
operator ^ ( ushort8 lhs, ushort8 rhs )
{
   return ushort8( spu_xor( (vec_ushort8)lhs, (vec_ushort8)rhs ) );
}
   
inline
ushort8_idx::operator ushort() const
{ 
   return (ushort)spu_extract( (vec_ushort8)ref, i );
}

inline 
ushort
ushort8_idx::operator = ( ushort rhs )
{
   ref = spu_insert( (ushort)rhs, (vec_ushort8)ref, i );
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
   ushort tmp = spu_extract( (vec_ushort8)ref, i );
   ref = spu_insert( (unsigned short)(tmp+1), (vec_ushort8)ref, i );
   return tmp;
}

inline
ushort
ushort8_idx::operator --( int )
{
   ushort tmp = spu_extract( (vec_ushort8)ref, i );
   ref = spu_insert( (unsigned short)(tmp-1), (vec_ushort8)ref, i );
   return tmp;
}

inline
ushort
ushort8_idx::operator ++()
{
   ushort tmp = spu_extract( (vec_ushort8)ref, i );
   ++tmp;
   ref = spu_insert( tmp, (vec_ushort8)ref, i );
   return tmp;
}

inline
ushort
ushort8_idx::operator --()
{
   ushort tmp = spu_extract( (vec_ushort8)ref, i );
   --tmp;
   ref = spu_insert( tmp, (vec_ushort8)ref, i );
   return tmp;
}

inline
ushort
ushort8_idx::operator *= ( ushort rhs )
{
   ushort tmp = spu_extract( (vec_ushort8)ref, i ) * rhs;
   ref = spu_insert( tmp, (vec_ushort8)ref, i );
   return tmp;
}

inline
ushort
ushort8_idx::operator /= ( ushort rhs )
{
   ushort tmp = spu_extract( (vec_ushort8)ref, i ) / rhs;
   ref = spu_insert( tmp, (vec_ushort8)ref, i );
   return tmp;
}

inline
ushort
ushort8_idx::operator %= ( ushort rhs )
{
   ushort tmp = spu_extract( (vec_ushort8)ref, i ) % rhs;
   ref = spu_insert( tmp, (vec_ushort8)ref, i );
   return tmp;
}

inline
ushort
ushort8_idx::operator += ( ushort rhs )
{
   ushort tmp = spu_extract( (vec_ushort8)ref, i ) + rhs;
   ref = spu_insert( tmp, (vec_ushort8)ref, i );
   return tmp;
}

inline
ushort
ushort8_idx::operator -= ( ushort rhs )
{
   ushort tmp = spu_extract( (vec_ushort8)ref, i ) - rhs;
   ref = spu_insert( tmp, (vec_ushort8)ref, i );
   return tmp;
}

inline
ushort
ushort8_idx::operator <<= ( ushort rhs )
{
   ushort tmp = spu_extract( (vec_ushort8)ref, i ) << rhs;
   ref = spu_insert( tmp, (vec_ushort8)ref, i );
   return tmp;
}

inline
ushort
ushort8_idx::operator >>= ( ushort rhs )
{
   ushort tmp = spu_extract( (vec_ushort8)ref, i ) >> rhs;
   ref = spu_insert( tmp, (vec_ushort8)ref, i );
   return tmp;
}

inline
ushort
ushort8_idx::operator &= ( ushort rhs )
{
   ushort tmp = spu_extract( (vec_ushort8)ref, i ) & rhs;
   ref = spu_insert( tmp, (vec_ushort8)ref, i );
   return tmp;
}

inline
ushort
ushort8_idx::operator ^= ( ushort rhs )
{
   ushort tmp = spu_extract( (vec_ushort8)ref, i ) ^ rhs;
   ref = spu_insert( tmp, (vec_ushort8)ref, i );
   return tmp;
}

inline
ushort
ushort8_idx::operator |= ( ushort rhs )
{
   ushort tmp = spu_extract( (vec_ushort8)ref, i ) | rhs;
   ref = spu_insert( tmp, (vec_ushort8)ref, i );
   return tmp;
}

}; // namespace simd

#endif // ushort8_h
