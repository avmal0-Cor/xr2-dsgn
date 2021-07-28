/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2008 Sony Computer Entertainment Inc. 
* All Rights Reserved.
*/

#ifndef _SCE_SIMD_SHORT8_H
#define _SCE_SIMD_SHORT8_H

#include <spu_intrinsics.h>
#include "simd_scalars.h"

namespace simd {

class bool8;
class ushort8;
class short8_idx;

//--------------------------------------------------------------------------------------------------
// short8 class
//

class short8
{
   private:
      vec_short8 data;

   public:
      inline short8() {}
      inline short8( bool8 );
      inline short8( ushort8 );
      inline short8( short r0, short r1, short r2, short r3,
                     short r4, short r5, short r6, short r7 );

      // set all slots to same short

      explicit inline short8( short );

      // allows implicit conversion from vector short

      inline short8( vec_short8 rhs );

      // allows implicit conversion to vector short

      inline operator vector signed short() const;

      // use array index operator to set and get slots

      inline short8_idx operator [] ( int i );
      inline short operator [] ( int i ) const;

      inline const short8 operator ++ ( int );
      inline const short8 operator -- ( int );
      inline short8& operator ++ ();
      inline short8& operator -- ();

      inline const short8 operator ~ () const;
      inline const bool8 operator ! () const;
      inline const short8 operator - () const;

      inline short8& operator = ( short8 rhs );
      inline short8& operator *= ( short8 rhs );
      inline short8& operator /= ( short8 rhs );
      inline short8& operator %= ( short8 rhs );
      inline short8& operator += ( short8 rhs );
      inline short8& operator -= ( short8 rhs );
      inline short8& operator <<= ( ushort8 rhs );
      inline short8& operator >>= ( ushort8 rhs );
      inline short8& operator &= ( short8 rhs );
      inline short8& operator ^= ( short8 rhs );
      inline short8& operator |= ( short8 rhs );
};

//--------------------------------------------------------------------------------------------------
// functions for short8
//

inline const short8 operator * ( short8 lhs, short8 rhs );
inline const short8 operator / ( short8 lhs, short8 rhs );
inline const short8 operator % ( short8 lhs, short8 rhs );
inline const short8 operator + ( short8 lhs, short8 rhs );
inline const short8 operator - ( short8 lhs, short8 rhs );

inline const short8 operator << ( short8 lhs, ushort8 rhs );
inline const short8 operator >> ( short8 lhs, ushort8 rhs );

inline const bool8 operator < ( short8 lhs, short8 rhs );
inline const bool8 operator <= ( short8 lhs, short8 rhs );
inline const bool8 operator > ( short8 lhs, short8 rhs );
inline const bool8 operator >= ( short8 lhs, short8 rhs );
inline const bool8 operator == ( short8 lhs, short8 rhs );
inline const bool8 operator != ( short8 lhs, short8 rhs );

// select between lhs and rhs slots using bool8.
// false selects lhs slot, true selects rhs slot

inline const short8 select( short8 lhs, short8 rhs, bool8 rhs_slots );

inline const short8 operator & ( short8 lhs, short8 rhs );
inline const short8 operator ^ ( short8 lhs, short8 rhs );
inline const short8 operator | ( short8 lhs, short8 rhs );

//--------------------------------------------------------------------------------------------------
// short8_idx: for setting short8 slots with [] operator
//

class short8_idx
{
   private:
      short8 &ref __attribute__ ((aligned(16)));
      int i __attribute__ ((aligned(16)));
   public:
      inline short8_idx( short8& vec, int idx ): ref(vec) { i = idx; }
      inline operator short() const;
      inline short operator = ( short rhs );
      inline short operator = ( const short8_idx& rhs );
      inline short operator ++ ( int );
      inline short operator -- ( int );
      inline short operator ++ ();
      inline short operator -- ();
      inline short operator *= ( short rhs );
      inline short operator /= ( short rhs );
      inline short operator %= ( short rhs );
      inline short operator += ( short rhs );
      inline short operator -= ( short rhs );
      inline short operator <<= ( ushort rhs );
      inline short operator >>= ( ushort rhs );
      inline short operator &= ( short rhs );
      inline short operator ^= ( short rhs );
      inline short operator |= ( short rhs );
};

}; // namespace simd

#include "simd_bool8.h"
#include "simd_ushort8.h"

namespace simd {

inline
short8::short8( short rhs )
{
   data = spu_splats((short)rhs);
}

inline
short8::short8( vec_short8 rhs )
{
   data = rhs;
}

inline
short8::short8( bool8 rhs )
{
   data = (vec_short8)spu_and( (vec_ushort8)rhs, 1 );
}

inline
short8::short8( ushort8 rhs )
{
   data = (vec_short8)(vec_ushort8)rhs;
}

inline
short8::short8( short r0, short r1, short r2, short r3,
                short r4, short r5, short r6, short r7 )
{
   data = __extension__ (vec_short8){ r0, r1, r2, r3,
                                      r4, r5, r6, r7 };
}

inline
short8::operator vector signed short() const
{
   return data;
}

inline
short8_idx
short8::operator [] ( int i )
{
   return short8_idx(*this,i);
}

inline
short
short8::operator [] ( int i ) const
{
   return (short)spu_extract((vec_short8)data, i);
}

inline
const short8
short8::operator ++ ( int )
{
   vec_short8 olddata = data;
   operator ++();
   return short8(olddata);
}

inline
const short8
short8::operator -- ( int )
{
   vec_short8 olddata = data;
   operator --();
   return short8(olddata);
}

inline
short8&
short8::operator ++ ()
{
   *this += short8(1);
   return *this;
}

inline
short8&
short8::operator -- ()
{
   *this -= short8(1);
   return *this;
}

inline
const short8 
short8::operator ~ () const
{
   return short8( spu_nor( data, data ) );
}

inline
const bool8
short8::operator ! () const
{
   return ( *this == short8(0) );
}

inline
const short8
short8::operator - () const
{
   return short8( (vec_short8)spu_sub( (short)0, data ) );
}

inline
short8&
short8::operator = ( short8 rhs )
{
   data = rhs.data;
   return *this;
}

inline
short8&
short8::operator *= ( short8 rhs )
{
   *this = *this * rhs;
   return *this;
}

inline
short8&
short8::operator /= ( short8 rhs )
{
   *this = *this / rhs;
   return *this;
}

inline
short8&
short8::operator %= ( short8 rhs )
{
   *this = *this % rhs;
   return *this;
}

inline
short8&
short8::operator += ( short8 rhs )
{
   *this = *this + rhs;
   return *this;
}

inline
short8&
short8::operator -= ( short8 rhs )
{
   *this = *this - rhs;
   return *this;
}

inline
short8&
short8::operator <<= ( ushort8 rhs )
{
   *this = *this << rhs;
   return *this;
}

inline
short8&
short8::operator >>= ( ushort8 rhs )
{
   *this = *this >> rhs;
   return *this;
}

inline
short8&
short8::operator &= ( short8 rhs )
{
   *this = *this & rhs;
   return *this;
}

inline
short8&
short8::operator ^= ( short8 rhs )
{
   *this = *this ^ rhs;
   return *this;
}

inline
short8&
short8::operator |= ( short8 rhs )
{
   *this = *this | rhs;
   return *this;
}

inline
const short8
operator * ( short8 lhs, short8 rhs )
{
   const vec_uchar16 pack = __extension__ (vec_uchar16)(vec_ushort8){ 0x0203, 0x1213, 0x0607, 0x1617, 
                                                                      0x0a0b, 0x1a1b, 0x0e0f, 0x1e1f };

   vec_short8 lhs_short = (vec_short8)(vec_short8)lhs;
   vec_short8 rhs_short = (vec_short8)(vec_short8)rhs;
   
   vec_int4 lo_mul = spu_mulo( lhs_short, rhs_short );
   vec_int4 hi_mul = spu_mule( lhs_short, rhs_short );

   return short8( (vec_short8)spu_shuffle( hi_mul, lo_mul, pack ) );
}

inline
const short8
operator / ( short8 num, short8 den )
{
   bool8 numPos = ( num > short8(-1) );
   bool8 denPos = ( den > short8(-1) );
   bool8 quotNeg = numPos ^ denPos;

   ushort8 numAbs = (ushort8)select( -num, num, numPos );
   ushort8 denAbs = (ushort8)select( -den, den, denPos );
   ushort8 res = numAbs / denAbs;
   return select( (short8)res, -res, quotNeg );
}

inline
const short8
operator % ( short8 num, short8 den )
{
   bool8 numPos = ( num > short8(-1) );
   bool8 denPos = ( den > short8(-1) );

   ushort8 numAbs = (ushort8)select( -num, num, numPos );
   ushort8 denAbs = (ushort8)select( -den, den, denPos );
   ushort8 res = numAbs % denAbs;
   return select( -res, (short8)res, numPos );
}

inline
const short8
operator + ( short8 lhs, short8 rhs )
{
   return short8( spu_add( (vec_short8)lhs, (vec_short8)rhs ) );
}

inline
const short8
operator - ( short8 lhs, short8 rhs )
{
   return short8( spu_sub( (vec_short8)lhs, (vec_short8)rhs ) );
}

inline
const short8
operator << ( short8 lhs, ushort8 rhs )
{
   return short8( spu_sl( (vec_short8)lhs, (vec_ushort8)rhs ) );
}

inline
const short8
operator >> ( short8 lhs, ushort8 rhs )
{
   return short8( spu_rlmaska( (vec_short8)lhs, (vec_short8)-rhs ) );
}

inline
const bool8
operator < ( short8 lhs, short8 rhs )
{
   bool8 result;
   result.data = spu_cmpgt( (vec_short8)rhs, (vec_short8)lhs );
   return result;
}

inline
const bool8
operator <= ( short8 lhs, short8 rhs )
{
   return !( lhs > rhs );
}

inline
const bool8
operator > ( short8 lhs, short8 rhs )
{
   bool8 result;
   result.data = spu_cmpgt( (vec_short8)lhs, (vec_short8)rhs );
   return result;
}

inline
const bool8
operator >= ( short8 lhs, short8 rhs )
{
   return !( lhs < rhs );
}

inline
const bool8
operator == ( short8 lhs, short8 rhs )
{
   bool8 result;
   result.data = spu_cmpeq( (vec_short8)lhs, (vec_short8)rhs );
   return result;
}

inline
const bool8
operator != ( short8 lhs, short8 rhs )
{
   return !( lhs == rhs );
}
   
inline
const short8
select( short8 lhs, short8 rhs, bool8 rhs_slots )
{
   return short8( spu_sel( (vec_short8)lhs, (vec_short8)rhs, (vec_ushort8)rhs_slots ) );
}

inline
const short8
operator & ( short8 lhs, short8 rhs )
{
   return short8( spu_and( (vec_short8)lhs, (vec_short8)rhs ) );
}

inline
const short8
operator | ( short8 lhs, short8 rhs )
{
   return short8( spu_or( (vec_short8)lhs, (vec_short8)rhs ) );
}

inline
const short8
operator ^ ( short8 lhs, short8 rhs )
{
   return short8( spu_xor( (vec_short8)lhs, (vec_short8)rhs ) );
}
   
inline
short8_idx::operator short() const
{ 
   return (short)spu_extract( (vec_short8)ref, i );
}

inline 
short
short8_idx::operator = ( short rhs )
{
   ref = spu_insert( (short)rhs, (vec_short8)ref, i );
   return rhs;
}

inline
short
short8_idx::operator = ( const short8_idx& rhs )
{
   return ( *this = (short)rhs );
}

inline
short
short8_idx::operator ++( int )
{
   short tmp = spu_extract( (vec_short8)ref, i );
   ref = spu_insert( (short)(tmp+1), (vec_short8)ref, i );
   return tmp;
}

inline
short
short8_idx::operator --( int )
{
   short tmp = spu_extract( (vec_short8)ref, i );
   ref = spu_insert( (short)(tmp-1), (vec_short8)ref, i );
   return tmp;
}

inline
short
short8_idx::operator ++()
{
   short tmp = spu_extract( (vec_short8)ref, i );
   ++tmp;
   ref = spu_insert( tmp, (vec_short8)ref, i );
   return tmp;
}

inline
short
short8_idx::operator --()
{
   short tmp = spu_extract( (vec_short8)ref, i );
   --tmp;
   ref = spu_insert( tmp, (vec_short8)ref, i );
   return tmp;
}

inline
short
short8_idx::operator *= ( short rhs )
{
   short tmp = spu_extract( (vec_short8)ref, i ) * rhs;
   ref = spu_insert( tmp, (vec_short8)ref, i );
   return tmp;
}

inline
short
short8_idx::operator /= ( short rhs )
{
   short tmp = spu_extract( (vec_short8)ref, i ) / rhs;
   ref = spu_insert( tmp, (vec_short8)ref, i );
   return tmp;
}

inline
short
short8_idx::operator %= ( short rhs )
{
   short tmp = spu_extract( (vec_short8)ref, i ) % rhs;
   ref = spu_insert( tmp, (vec_short8)ref, i );
   return tmp;
}

inline
short
short8_idx::operator += ( short rhs )
{
   short tmp = spu_extract( (vec_short8)ref, i ) + rhs;
   ref = spu_insert( tmp, (vec_short8)ref, i );
   return tmp;
}

inline
short
short8_idx::operator -= ( short rhs )
{
   short tmp = spu_extract( (vec_short8)ref, i ) - rhs;
   ref = spu_insert( tmp, (vec_short8)ref, i );
   return tmp;
}

inline
short
short8_idx::operator <<= ( ushort rhs )
{
   short tmp = spu_extract( (vec_short8)ref, i ) << rhs;
   ref = spu_insert( tmp, (vec_short8)ref, i );
   return tmp;
}

inline
short
short8_idx::operator >>= ( ushort rhs )
{
   short tmp = spu_extract( (vec_short8)ref, i ) >> rhs;
   ref = spu_insert( tmp, (vec_short8)ref, i );
   return tmp;
}

inline
short
short8_idx::operator &= ( short rhs )
{
   short tmp = spu_extract( (vec_short8)ref, i ) & rhs;
   ref = spu_insert( tmp, (vec_short8)ref, i );
   return tmp;
}

inline
short
short8_idx::operator ^= ( short rhs )
{
   short tmp = spu_extract( (vec_short8)ref, i ) ^ rhs;
   ref = spu_insert( tmp, (vec_short8)ref, i );
   return tmp;
}

inline
short
short8_idx::operator |= ( short rhs )
{
   short tmp = spu_extract( (vec_short8)ref, i ) | rhs;
   ref = spu_insert( tmp, (vec_short8)ref, i );
   return tmp;
}

}; // namespace simd

#endif // short8_h
