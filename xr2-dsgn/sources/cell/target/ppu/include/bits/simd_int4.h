/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2008 Sony Computer Entertainment Inc. 
* All Rights Reserved.
*/

#ifndef _SCE_SIMD_INT4_H
#define _SCE_SIMD_INT4_H

#include <altivec.h>
#include "simd_scalars.h"

namespace simd {

class bool4;
class uint4;
class float4;
class int4_idx;

//--------------------------------------------------------------------------------------------------
// int4 class
//

class int4
{
   private:
      vec_int4 data;

   public:
      inline int4() {}
      inline int4( bool4 );
      inline int4( uint4 );
      inline int4( float4 );
      inline int4( int r0, int r1, int r2, int r3 );

      // set all slots to same int

      explicit inline int4( int );

      // allows implicit conversion from vector int

      inline int4( vec_int4 rhs );

      // allows implicit conversion to vector int

      inline operator vector signed int() const;

      // use array index operator to set and get slots

      inline int4_idx operator [] ( int i );
      inline int operator [] ( int i ) const;

      inline const int4 operator ++ ( int );
      inline const int4 operator -- ( int );
      inline int4& operator ++ ();
      inline int4& operator -- ();

      inline const int4 operator ~ () const;
      inline const bool4 operator ! () const;
      inline const int4 operator - () const;

      inline int4& operator = ( int4 rhs );
      inline int4& operator *= ( int4 rhs );
      inline int4& operator /= ( int4 rhs );
      inline int4& operator %= ( int4 rhs );
      inline int4& operator += ( int4 rhs );
      inline int4& operator -= ( int4 rhs );
      inline int4& operator <<= ( uint4 rhs );
      inline int4& operator >>= ( uint4 rhs );
      inline int4& operator &= ( int4 rhs );
      inline int4& operator ^= ( int4 rhs );
      inline int4& operator |= ( int4 rhs );
};

//--------------------------------------------------------------------------------------------------
// functions for int4
//

inline const int4 operator * ( int4 lhs, int4 rhs );
inline const int4 operator / ( int4 lhs, int4 rhs );
inline const int4 operator % ( int4 lhs, int4 rhs );
inline const int4 operator + ( int4 lhs, int4 rhs );
inline const int4 operator - ( int4 lhs, int4 rhs );

inline const int4 operator << ( int4 lhs, uint4 rhs );
inline const int4 operator >> ( int4 lhs, uint4 rhs );

inline const bool4 operator < ( int4 lhs, int4 rhs );
inline const bool4 operator <= ( int4 lhs, int4 rhs );
inline const bool4 operator > ( int4 lhs, int4 rhs );
inline const bool4 operator >= ( int4 lhs, int4 rhs );
inline const bool4 operator == ( int4 lhs, int4 rhs );
inline const bool4 operator != ( int4 lhs, int4 rhs );

// select between lhs and rhs slots using bool4.
// false selects lhs slot, true selects rhs slot

inline const int4 select( int4 lhs, int4 rhs, bool4 rhs_slots );

inline const int4 operator & ( int4 lhs, int4 rhs );
inline const int4 operator ^ ( int4 lhs, int4 rhs );
inline const int4 operator | ( int4 lhs, int4 rhs );

//--------------------------------------------------------------------------------------------------
// int4_idx: for setting int4 slots with [] operator
//

class int4_idx
{
   private:
      int4 &ref __attribute__ ((aligned(16)));
      int i __attribute__ ((aligned(16)));
   public:
      inline int4_idx( int4& vec, int idx ): ref(vec) { i = idx; }
      inline operator int() const;
      inline int operator = ( int rhs );
      inline int operator = ( const int4_idx& rhs );
      inline int operator ++ ( int );
      inline int operator -- ( int );
      inline int operator ++ ();
      inline int operator -- ();
      inline int operator *= ( int rhs );
      inline int operator /= ( int rhs );
      inline int operator %= ( int rhs );
      inline int operator += ( int rhs );
      inline int operator -= ( int rhs );
      inline int operator <<= ( uint rhs );
      inline int operator >>= ( uint rhs );
      inline int operator &= ( int rhs );
      inline int operator ^= ( int rhs );
      inline int operator |= ( int rhs );
};

}; // namespace simd

#include "simd_bool4.h"
#include "simd_uint4.h"
#include "simd_float4.h"

namespace simd {

inline
int4::int4( int rhs )
{
   int tmp = (int)rhs;
   data = __extension__ (vec_int4){ tmp, tmp, tmp, tmp };
}

inline
int4::int4( vec_int4 rhs )
{
   data = rhs;
}

inline
int4::int4( bool4 rhs )
{
   data = (vec_int4)vec_and( (vec_uint4)rhs, (vec_uint4)(1) );
}

inline
int4::int4( uint4 rhs )
{
   data = (vec_int4)(vec_uint4)rhs;
}

inline
int4::int4( float4 rhs )
{
   data = vec_cts( (vec_float4)rhs, 0 );
}

inline
int4::int4( int r0, int r1, int r2, int r3 )
{
   data = __extension__ (vec_int4){ r0, r1, r2, r3 };
}

inline
int4::operator vector signed int() const
{
   return data;
}

inline
int4_idx
int4::operator [] ( int i )
{
   return int4_idx(*this,i);
}

inline
int
int4::operator [] ( int i ) const
{
   return (int)_vec_extract((vec_int4)data, i);
}

inline
const int4
int4::operator ++ ( int )
{
   vec_int4 olddata = data;
   operator ++();
   return int4(olddata);
}

inline
const int4
int4::operator -- ( int )
{
   vec_int4 olddata = data;
   operator --();
   return int4(olddata);
}

inline
int4&
int4::operator ++ ()
{
   *this += int4(1);
   return *this;
}

inline
int4&
int4::operator -- ()
{
   *this -= int4(1);
   return *this;
}

inline
const int4 
int4::operator ~ () const
{
   return int4( vec_nor( data, data ) );
}

inline
const bool4
int4::operator ! () const
{
   return ( *this == int4(0) );
}

inline
const int4
int4::operator - () const
{
   return int4( (vec_int4)vec_sub( (vec_int4)(0), data ) );
}

inline
int4&
int4::operator = ( int4 rhs )
{
   data = rhs.data;
   return *this;
}

inline
int4&
int4::operator *= ( int4 rhs )
{
   *this = *this * rhs;
   return *this;
}

inline
int4&
int4::operator /= ( int4 rhs )
{
   *this = *this / rhs;
   return *this;
}

inline
int4&
int4::operator %= ( int4 rhs )
{
   *this = *this % rhs;
   return *this;
}

inline
int4&
int4::operator += ( int4 rhs )
{
   *this = *this + rhs;
   return *this;
}

inline
int4&
int4::operator -= ( int4 rhs )
{
   *this = *this - rhs;
   return *this;
}

inline
int4&
int4::operator <<= ( uint4 rhs )
{
   *this = *this << rhs;
   return *this;
}

inline
int4&
int4::operator >>= ( uint4 rhs )
{
   *this = *this >> rhs;
   return *this;
}

inline
int4&
int4::operator &= ( int4 rhs )
{
   *this = *this & rhs;
   return *this;
}

inline
int4&
int4::operator ^= ( int4 rhs )
{
   *this = *this ^ rhs;
   return *this;
}

inline
int4&
int4::operator |= ( int4 rhs )
{
   *this = *this | rhs;
   return *this;
}

inline
const int4
operator * ( int4 lhs, int4 rhs )
{
   const vec_uint4 sixteen = (vec_uint4)(16);
   
   vec_ushort8 lhs_short = (vec_ushort8)(vec_int4)lhs;
   vec_ushort8 rhs_short = (vec_ushort8)(vec_int4)rhs;

   vec_int4 cross_mul = (vec_int4)vec_msum( lhs_short, (vec_ushort8)vec_rl( (vec_int4)rhs, sixteen ), (vec_uint4)(0) );
   vec_int4 lo_mul = (vec_int4)vec_mulo( lhs_short, rhs_short );
   
   return int4( (vec_int4)vec_add( vec_sl( (vec_int4)cross_mul, sixteen ), lo_mul ) );
}

inline
const int4
operator / ( int4 num, int4 den )
{
   bool4 numPos = ( num > int4(-1) );
   bool4 denPos = ( den > int4(-1) );
   bool4 quotNeg = numPos ^ denPos;

   uint4 numAbs = (uint4)select( -num, num, numPos );
   uint4 denAbs = (uint4)select( -den, den, denPos );
   uint4 res = numAbs / denAbs;
   return select( (int4)res, -res, quotNeg );
}

inline
const int4
operator % ( int4 num, int4 den )
{
   bool4 numPos = ( num > int4(-1) );
   bool4 denPos = ( den > int4(-1) );

   uint4 numAbs = (uint4)select( -num, num, numPos );
   uint4 denAbs = (uint4)select( -den, den, denPos );
   uint4 res = numAbs % denAbs;
   return select( -res, (int4)res, numPos );
}

inline
const int4
operator + ( int4 lhs, int4 rhs )
{
   return int4( vec_add( (vec_int4)lhs, (vec_int4)rhs ) );
}

inline
const int4
operator - ( int4 lhs, int4 rhs )
{
   return int4( vec_sub( (vec_int4)lhs, (vec_int4)rhs ) );
}

inline
const int4
operator << ( int4 lhs, uint4 rhs )
{
   return int4( vec_sl( (vec_int4)lhs, (vec_uint4)rhs ) );
}

inline
const int4
operator >> ( int4 lhs, uint4 rhs )
{
   return int4( vec_sra( (vec_int4)lhs, (vec_uint4)rhs ) );
}

inline
const bool4
operator < ( int4 lhs, int4 rhs )
{
   bool4 result;
   result.data = (vec_uint4)vec_cmpgt( (vec_int4)rhs, (vec_int4)lhs );
   return result;
}

inline
const bool4
operator <= ( int4 lhs, int4 rhs )
{
   return !( lhs > rhs );
}

inline
const bool4
operator > ( int4 lhs, int4 rhs )
{
   bool4 result;
   result.data = (vec_uint4)vec_cmpgt( (vec_int4)lhs, (vec_int4)rhs );
   return result;
}

inline
const bool4
operator >= ( int4 lhs, int4 rhs )
{
   return !( lhs < rhs );
}

inline
const bool4
operator == ( int4 lhs, int4 rhs )
{
   bool4 result;
   result.data = (vec_uint4)vec_cmpeq( (vec_int4)lhs, (vec_int4)rhs );
   return result;
}

inline
const bool4
operator != ( int4 lhs, int4 rhs )
{
   return !( lhs == rhs );
}
   
inline
const int4
select( int4 lhs, int4 rhs, bool4 rhs_slots )
{
   return int4( vec_sel( (vec_int4)lhs, (vec_int4)rhs, (vec_uint4)rhs_slots ) );
}

inline
const int4
operator & ( int4 lhs, int4 rhs )
{
   return int4( vec_and( (vec_int4)lhs, (vec_int4)rhs ) );
}

inline
const int4
operator | ( int4 lhs, int4 rhs )
{
   return int4( vec_or( (vec_int4)lhs, (vec_int4)rhs ) );
}

inline
const int4
operator ^ ( int4 lhs, int4 rhs )
{
   return int4( vec_xor( (vec_int4)lhs, (vec_int4)rhs ) );
}
   
inline
int4_idx::operator int() const
{ 
   return (int)_vec_extract( (vec_int4)ref, i );
}

inline 
int
int4_idx::operator = ( int rhs )
{
   ref = _vec_insert( (int)rhs, (vec_int4)ref, i );
   return rhs;
}

inline
int
int4_idx::operator = ( const int4_idx& rhs )
{
   return ( *this = (int)rhs );
}

inline
int
int4_idx::operator ++( int )
{
   int tmp = _vec_extract( (vec_int4)ref, i );
   ref = _vec_insert( tmp+1, (vec_int4)ref, i );
   return tmp;
}

inline
int
int4_idx::operator --( int )
{
   int tmp = _vec_extract( (vec_int4)ref, i );
   ref = _vec_insert( tmp-1, (vec_int4)ref, i );
   return tmp;
}

inline
int
int4_idx::operator ++()
{
   int tmp = _vec_extract( (vec_int4)ref, i );
   ++tmp;
   ref = _vec_insert( tmp, (vec_int4)ref, i );
   return tmp;
}

inline
int
int4_idx::operator --()
{
   int tmp = _vec_extract( (vec_int4)ref, i );
   --tmp;
   ref = _vec_insert( tmp, (vec_int4)ref, i );
   return tmp;
}

inline
int
int4_idx::operator *= ( int rhs )
{
   int tmp = _vec_extract( (vec_int4)ref, i ) * rhs;
   ref = _vec_insert( tmp, (vec_int4)ref, i );
   return tmp;
}

inline
int
int4_idx::operator /= ( int rhs )
{
   int tmp = _vec_extract( (vec_int4)ref, i ) / rhs;
   ref = _vec_insert( tmp, (vec_int4)ref, i );
   return tmp;
}

inline
int
int4_idx::operator %= ( int rhs )
{
   int tmp = _vec_extract( (vec_int4)ref, i ) % rhs;
   ref = _vec_insert( tmp, (vec_int4)ref, i );
   return tmp;
}

inline
int
int4_idx::operator += ( int rhs )
{
   int tmp = _vec_extract( (vec_int4)ref, i ) + rhs;
   ref = _vec_insert( tmp, (vec_int4)ref, i );
   return tmp;
}

inline
int
int4_idx::operator -= ( int rhs )
{
   int tmp = _vec_extract( (vec_int4)ref, i ) - rhs;
   ref = _vec_insert( tmp, (vec_int4)ref, i );
   return tmp;
}

inline
int
int4_idx::operator <<= ( uint rhs )
{
   int tmp = _vec_extract( (vec_int4)ref, i ) << rhs;
   ref = _vec_insert( tmp, (vec_int4)ref, i );
   return tmp;
}

inline
int
int4_idx::operator >>= ( uint rhs )
{
   int tmp = _vec_extract( (vec_int4)ref, i ) >> rhs;
   ref = _vec_insert( tmp, (vec_int4)ref, i );
   return tmp;
}

inline
int
int4_idx::operator &= ( int rhs )
{
   int tmp = _vec_extract( (vec_int4)ref, i ) & rhs;
   ref = _vec_insert( tmp, (vec_int4)ref, i );
   return tmp;
}

inline
int
int4_idx::operator ^= ( int rhs )
{
   int tmp = _vec_extract( (vec_int4)ref, i ) ^ rhs;
   ref = _vec_insert( tmp, (vec_int4)ref, i );
   return tmp;
}

inline
int
int4_idx::operator |= ( int rhs )
{
   int tmp = _vec_extract( (vec_int4)ref, i ) | rhs;
   ref = _vec_insert( tmp, (vec_int4)ref, i );
   return tmp;
}

}; // namespace simd

#endif // int4_h
