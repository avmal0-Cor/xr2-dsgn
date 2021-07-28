/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2008 Sony Computer Entertainment Inc. 
* All Rights Reserved.
*/

#ifndef _SCE_SIMD_CHAR16_H
#define _SCE_SIMD_CHAR16_H

#include <altivec.h>
#include "simd_scalars.h"

namespace simd {

class bool16;
class uchar16;
class char16_idx;

//--------------------------------------------------------------------------------------------------
// char16 class
//

class char16
{
   private:
      vec_char16 data;

   public:
      inline char16() {}
      inline char16( bool16 );
      inline char16( uchar16 );
      inline char16( schar r0, schar r1, schar r2, schar r3,
                     schar r4, schar r5, schar r6, schar r7,
                     schar r8, schar r9, schar r10, schar r11,
                     schar r12, schar r13, schar r14, schar r15 );

      // set all slots to same schar

      explicit inline char16( schar );

      // allows implicit conversion from vector signed char

      inline char16( vec_char16 rhs );

      // allows implicit conversion to vector signed char

      inline operator vector signed char() const;

      // use array index operator to set and get slots

      inline char16_idx operator [] ( int i );
      inline schar operator [] ( int i ) const;

      inline const char16 operator ++ ( int );
      inline const char16 operator -- ( int );
      inline char16& operator ++ ();
      inline char16& operator -- ();

      inline const char16 operator ~ () const;
      inline const bool16 operator ! () const;
      inline const char16 operator - () const;

      inline char16& operator = ( char16 rhs );
      inline char16& operator *= ( char16 rhs );
      inline char16& operator /= ( char16 rhs );
      inline char16& operator %= ( char16 rhs );
      inline char16& operator += ( char16 rhs );
      inline char16& operator -= ( char16 rhs );
      inline char16& operator <<= ( uchar16 rhs );
      inline char16& operator >>= ( uchar16 rhs );
      inline char16& operator &= ( char16 rhs );
      inline char16& operator ^= ( char16 rhs );
      inline char16& operator |= ( char16 rhs );
};

//--------------------------------------------------------------------------------------------------
// functions for char16
//

inline const char16 operator * ( char16 lhs, char16 rhs );
inline const char16 operator / ( char16 lhs, char16 rhs );
inline const char16 operator % ( char16 lhs, char16 rhs );
inline const char16 operator + ( char16 lhs, char16 rhs );
inline const char16 operator - ( char16 lhs, char16 rhs );

inline const char16 operator << ( char16 lhs, uchar16 rhs );
inline const char16 operator >> ( char16 lhs, uchar16 rhs );

inline const bool16 operator < ( char16 lhs, char16 rhs );
inline const bool16 operator <= ( char16 lhs, char16 rhs );
inline const bool16 operator > ( char16 lhs, char16 rhs );
inline const bool16 operator >= ( char16 lhs, char16 rhs );
inline const bool16 operator == ( char16 lhs, char16 rhs );
inline const bool16 operator != ( char16 lhs, char16 rhs );

// select between lhs and rhs slots using bool16.
// false selects lhs slot, true selects rhs slot

inline const char16 select( char16 lhs, char16 rhs, bool16 rhs_slots );

inline const char16 operator & ( char16 lhs, char16 rhs );
inline const char16 operator ^ ( char16 lhs, char16 rhs );
inline const char16 operator | ( char16 lhs, char16 rhs );

//--------------------------------------------------------------------------------------------------
// char16_idx: for setting char16 slots with [] operator
//

class char16_idx
{
   private:
      char16 &ref __attribute__ ((aligned(16)));
      int i __attribute__ ((aligned(16)));
   public:
      inline char16_idx( char16& vec, int idx ): ref(vec) { i = idx; }
      inline operator schar() const;
      inline schar operator = ( schar rhs );
      inline schar operator = ( const char16_idx& rhs );
      inline schar operator ++ ( int );
      inline schar operator -- ( int );
      inline schar operator ++ ();
      inline schar operator -- ();
      inline schar operator *= ( schar rhs );
      inline schar operator /= ( schar rhs );
      inline schar operator %= ( schar rhs );
      inline schar operator += ( schar rhs );
      inline schar operator -= ( schar rhs );
      inline schar operator <<= ( uchar rhs );
      inline schar operator >>= ( uchar rhs );
      inline schar operator &= ( schar rhs );
      inline schar operator ^= ( schar rhs );
      inline schar operator |= ( schar rhs );
};

}; // namespace simd

#include "simd_bool16.h"
#include "simd_uchar16.h"
#include "simd_short8.h"

namespace simd {

inline
char16::char16( schar rhs )
{
   schar tmp = (schar)rhs;
   data = __extension__ (vec_char16){ tmp, tmp, tmp, tmp, tmp, tmp, tmp, tmp,
                                      tmp, tmp, tmp, tmp, tmp, tmp, tmp, tmp };
}

inline
char16::char16( vec_char16 rhs )
{
   data = rhs;
}

inline
char16::char16( bool16 rhs )
{
   data = (vec_char16)vec_and( (vec_uchar16)rhs, (vec_uchar16)(1) );
}

inline
char16::char16( uchar16 rhs )
{
   data = (vec_char16)(vec_uchar16)rhs;
}

inline
char16::char16( schar r0, schar r1, schar r2, schar r3,
                schar r4, schar r5, schar r6, schar r7,
                schar r8, schar r9, schar r10, schar r11,
                schar r12, schar r13, schar r14, schar r15 )
{
   data = __extension__ (vec_char16){ r0, r1, r2, r3,
                                      r4, r5, r6, r7,
                                      r8, r9, r10, r11,
                                      r12, r13, r14, r15 };
}

inline
char16::operator vector signed char() const
{
   return data;
}

inline
char16_idx
char16::operator [] ( int i )
{
   return char16_idx(*this,i);
}

inline
schar
char16::operator [] ( int i ) const
{
   return (schar)_vec_extract((vec_char16)data, i);
}

inline
const char16
char16::operator ++ ( int )
{
   vec_char16 olddata = data;
   operator ++();
   return char16(olddata);
}

inline
const char16
char16::operator -- ( int )
{
   vec_char16 olddata = data;
   operator --();
   return char16(olddata);
}

inline
char16&
char16::operator ++ ()
{
   *this += char16(1);
   return *this;
}

inline
char16&
char16::operator -- ()
{
   *this -= char16(1);
   return *this;
}

inline
const char16 
char16::operator ~ () const
{
   return char16( vec_nor( data, data ) );
}

inline
const bool16
char16::operator ! () const
{
   return ( *this == char16(0) );
}

inline
const char16
char16::operator - () const
{
   return char16(0) - *this;
}

inline
char16&
char16::operator = ( char16 rhs )
{
   data = rhs.data;
   return *this;
}

inline
char16&
char16::operator *= ( char16 rhs )
{
   *this = *this * rhs;
   return *this;
}

inline
char16&
char16::operator /= ( char16 rhs )
{
   *this = *this / rhs;
   return *this;
}

inline
char16&
char16::operator %= ( char16 rhs )
{
   *this = *this % rhs;
   return *this;
}

inline
char16&
char16::operator += ( char16 rhs )
{
   *this = *this + rhs;
   return *this;
}

inline
char16&
char16::operator -= ( char16 rhs )
{
   *this = *this - rhs;
   return *this;
}

inline
char16&
char16::operator <<= ( uchar16 rhs )
{
   *this = *this << rhs;
   return *this;
}

inline
char16&
char16::operator >>= ( uchar16 rhs )
{
   *this = *this >> rhs;
   return *this;
}

inline
char16&
char16::operator &= ( char16 rhs )
{
   *this = *this & rhs;
   return *this;
}

inline
char16&
char16::operator ^= ( char16 rhs )
{
   *this = *this ^ rhs;
   return *this;
}

inline
char16&
char16::operator |= ( char16 rhs )
{
   *this = *this | rhs;
   return *this;
}

inline
const char16
operator * ( char16 lhs, char16 rhs )
{
   vec_short8 hi_mul = (vec_short8)vec_mule( (vec_char16)lhs, (vec_char16)rhs );
   vec_short8 lo_mul = (vec_short8)vec_mulo( (vec_char16)lhs, (vec_char16)rhs );
   vec_char16 mul = (vec_char16)vec_pack( hi_mul, lo_mul );
   
   return char16( vec_mergeh( mul, vec_sld( mul, mul, 8 ) ) );
}

inline
const char16
operator / ( char16 num, char16 den )
{
   bool16 numPos = ( num > char16(-1) );
   bool16 denPos = ( den > char16(-1) );
   bool16 quotNeg = numPos ^ denPos;

   uchar16 numAbs = (uchar16)select( -num, num, numPos );
   uchar16 denAbs = (uchar16)select( -den, den, denPos );
   uchar16 res = numAbs / denAbs;
   return select( (char16)res, -res, quotNeg );
}

inline
const char16
operator % ( char16 num, char16 den )
{
   bool16 numPos = ( num > char16(-1) );
   bool16 denPos = ( den > char16(-1) );

   uchar16 numAbs = (uchar16)select( -num, num, numPos );
   uchar16 denAbs = (uchar16)select( -den, den, denPos );
   uchar16 res = numAbs % denAbs;
   return select( -res, (char16)res, numPos );
}

inline
const char16
operator + ( char16 lhs, char16 rhs )
{
   return char16( vec_add( (vec_char16)lhs, (vec_char16)rhs ) );
}

inline
const char16
operator - ( char16 lhs, char16 rhs )
{
   return char16( vec_sub( (vec_char16)lhs, (vec_char16)rhs ) );
}

inline
const char16
operator << ( char16 lhs, uchar16 rhs )
{
   return char16( vec_sl( (vec_char16)lhs, (vec_uchar16)rhs ) );
}

inline
const char16
operator >> ( char16 lhs, uchar16 rhs )
{
   return char16( vec_sra( (vec_char16)lhs, (vec_uchar16)rhs ) );
}

inline
const bool16
operator < ( char16 lhs, char16 rhs )
{
   bool16 result;
   result.data = (vec_uchar16)vec_cmpgt( (vec_char16)rhs, (vec_char16)lhs );
   return result;
}

inline
const bool16
operator <= ( char16 lhs, char16 rhs )
{
   return !( lhs > rhs );
}

inline
const bool16
operator > ( char16 lhs, char16 rhs )
{
   bool16 result;
   result.data = (vec_uchar16)vec_cmpgt( (vec_char16)lhs, (vec_char16)rhs );
   return result;
}

inline
const bool16
operator >= ( char16 lhs, char16 rhs )
{
   return !( lhs < rhs );
}

inline
const bool16
operator == ( char16 lhs, char16 rhs )
{
   bool16 result;
   result.data = (vec_uchar16)vec_cmpeq( (vec_char16)lhs, (vec_char16)rhs );
   return result;
}

inline
const bool16
operator != ( char16 lhs, char16 rhs )
{
   return !( lhs == rhs );
}
   
inline
const char16
select( char16 lhs, char16 rhs, bool16 rhs_slots )
{
   return char16( vec_sel( (vec_char16)lhs, (vec_char16)rhs, (vec_uchar16)rhs_slots ) );
}

inline
const char16
operator & ( char16 lhs, char16 rhs )
{
   return char16( vec_and( (vec_char16)lhs, (vec_char16)rhs ) );
}

inline
const char16
operator | ( char16 lhs, char16 rhs )
{
   return char16( vec_or( (vec_char16)lhs, (vec_char16)rhs ) );
}

inline
const char16
operator ^ ( char16 lhs, char16 rhs )
{
   return char16( vec_xor( (vec_char16)lhs, (vec_char16)rhs ) );
}
   
inline
char16_idx::operator schar() const
{ 
   return (schar)_vec_extract( (vec_char16)ref, i );
}

inline 
schar
char16_idx::operator = ( schar rhs )
{
   ref = _vec_insert( (schar)rhs, (vec_char16)ref, i );
   return rhs;
}

inline
schar
char16_idx::operator = ( const char16_idx& rhs )
{
   return ( *this = (schar)rhs );
}

inline
schar
char16_idx::operator ++( int )
{
   schar tmp = _vec_extract( (vec_char16)ref, i );
   ref = _vec_insert( tmp+1, (vec_char16)ref, i );
   return tmp;
}

inline
schar
char16_idx::operator --( int )
{
   schar tmp = _vec_extract( (vec_char16)ref, i );
   ref = _vec_insert( tmp-1, (vec_char16)ref, i );
   return tmp;
}

inline
schar
char16_idx::operator ++()
{
   schar tmp = _vec_extract( (vec_char16)ref, i );
   ++tmp;
   ref = _vec_insert( tmp, (vec_char16)ref, i );
   return tmp;
}

inline
schar
char16_idx::operator --()
{
   schar tmp = _vec_extract( (vec_char16)ref, i );
   --tmp;
   ref = _vec_insert( tmp, (vec_char16)ref, i );
   return tmp;
}

inline
schar
char16_idx::operator *= ( schar rhs )
{
   schar tmp = _vec_extract( (vec_char16)ref, i ) * rhs;
   ref = _vec_insert( tmp, (vec_char16)ref, i );
   return tmp;
}

inline
schar
char16_idx::operator /= ( schar rhs )
{
   schar tmp = _vec_extract( (vec_char16)ref, i ) / rhs;
   ref = _vec_insert( tmp, (vec_char16)ref, i );
   return tmp;
}

inline
schar
char16_idx::operator %= ( schar rhs )
{
   schar tmp = _vec_extract( (vec_char16)ref, i ) % rhs;
   ref = _vec_insert( tmp, (vec_char16)ref, i );
   return tmp;
}

inline
schar
char16_idx::operator += ( schar rhs )
{
   schar tmp = _vec_extract( (vec_char16)ref, i ) + rhs;
   ref = _vec_insert( tmp, (vec_char16)ref, i );
   return tmp;
}

inline
schar
char16_idx::operator -= ( schar rhs )
{
   schar tmp = _vec_extract( (vec_char16)ref, i ) - rhs;
   ref = _vec_insert( tmp, (vec_char16)ref, i );
   return tmp;
}

inline
schar
char16_idx::operator <<= ( uchar rhs )
{
   schar tmp = _vec_extract( (vec_char16)ref, i ) << rhs;
   ref = _vec_insert( tmp, (vec_char16)ref, i );
   return tmp;
}

inline
schar
char16_idx::operator >>= ( uchar rhs )
{
   schar tmp = _vec_extract( (vec_char16)ref, i ) >> rhs;
   ref = _vec_insert( tmp, (vec_char16)ref, i );
   return tmp;
}

inline
schar
char16_idx::operator &= ( schar rhs )
{
   schar tmp = _vec_extract( (vec_char16)ref, i ) & rhs;
   ref = _vec_insert( tmp, (vec_char16)ref, i );
   return tmp;
}

inline
schar
char16_idx::operator ^= ( schar rhs )
{
   schar tmp = _vec_extract( (vec_char16)ref, i ) ^ rhs;
   ref = _vec_insert( tmp, (vec_char16)ref, i );
   return tmp;
}

inline
schar
char16_idx::operator |= ( schar rhs )
{
   schar tmp = _vec_extract( (vec_char16)ref, i ) | rhs;
   ref = _vec_insert( tmp, (vec_char16)ref, i );
   return tmp;
}

}; // namespace simd

#endif // char16_h
