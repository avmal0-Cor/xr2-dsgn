/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2008 Sony Computer Entertainment Inc. 
* All Rights Reserved.
*/

#ifndef _SCE_SIMD_CHAR16_H
#define _SCE_SIMD_CHAR16_H

#include <spu_intrinsics.h>
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
   data = spu_splats((schar)rhs);
}

inline
char16::char16( vec_char16 rhs )
{
   data = rhs;
}

inline
char16::char16( bool16 rhs )
{
   data = (vec_char16)spu_and( (vec_uchar16)rhs, 1 );
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
   return (schar)spu_extract((vec_char16)data, i);
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
   return char16( spu_nor( data, data ) );
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
   const vec_uchar16 pack = __extension__ (vec_uchar16){ 0x01, 0x03, 0x11, 0x13, 0x05, 0x07, 0x15, 0x17,
                                                         0x09, 0x0b, 0x19, 0x1b, 0x0d, 0x0f, 0x1d, 0x1f };

   vec_short8 lhs_short = (vec_short8)(vec_char16)lhs;
   vec_short8 rhs_short = (vec_short8)(vec_char16)rhs;
   
   vec_short8 hi_char_lhs = spu_and( lhs_short, 0xff00 );
   vec_short8 hi_char_rhs = spu_and( rhs_short, 0xff00 );

   vec_short8 lo_char_lhs = spu_and( lhs_short, 0x00ff );
   vec_short8 lo_char_rhs = spu_and( rhs_short, 0x00ff );

   vec_int4 mul0 = spu_mule( hi_char_lhs, hi_char_rhs );
   vec_int4 mul1 = spu_mule( lo_char_lhs, lo_char_rhs );
   vec_int4 mul2 = spu_mulo( hi_char_lhs, hi_char_rhs );
   vec_int4 mul3 = spu_mulo( lo_char_lhs, lo_char_rhs );

   vec_int4 sum0 = spu_add( mul0, mul1 );
   vec_int4 sum1 = spu_add( mul2, mul3 );

   return char16( (vec_char16)spu_shuffle( sum0, sum1, pack ) );
}

inline
const char16
operator / ( char16 num, char16 den )
{
   const vec_uchar16 pack = __extension__ (vec_uchar16){ 0x01, 0x11, 0x03, 0x13, 0x05, 0x15, 0x07, 0x17,
                                                         0x09, 0x19, 0x0b, 0x1b, 0x0d, 0x1d, 0x0f, 0x1f };
                                     
   vec_char16 numer = (vec_char16)num;
   vec_char16 denom = (vec_char16)den;
   
   short8 hi_num( spu_rlmaska( (vec_short8)numer, -8 ) );
   short8 hi_den( spu_rlmaska( (vec_short8)denom, -8 ) );
   short8 lo_num( spu_extend( numer ) );
   short8 lo_den( spu_extend( denom ) );
   
   short8 hi_res = hi_num / hi_den;
   short8 lo_res = lo_num / lo_den;

   return char16( (vec_char16)spu_shuffle( (vec_short8)hi_res, (vec_short8)lo_res, pack ) );
}

inline
const char16
operator % ( char16 num, char16 den )
{
   const vec_uchar16 pack = __extension__ (vec_uchar16){ 0x01, 0x11, 0x03, 0x13, 0x05, 0x15, 0x07, 0x17,
                                                         0x09, 0x19, 0x0b, 0x1b, 0x0d, 0x1d, 0x0f, 0x1f };
                                     
   vec_char16 numer = (vec_char16)num;
   vec_char16 denom = (vec_char16)den;
   
   short8 hi_num( spu_rlmaska( (vec_short8)numer, -8 ) );
   short8 hi_den( spu_rlmaska( (vec_short8)denom, -8 ) );
   short8 lo_num( spu_extend( numer ) );
   short8 lo_den( spu_extend( denom ) );
   
   short8 hi_res = hi_num % hi_den;
   short8 lo_res = lo_num % lo_den;

   return char16( (vec_char16)spu_shuffle( (vec_short8)hi_res, (vec_short8)lo_res, pack ) );
}

inline
const char16
operator + ( char16 lhs, char16 rhs )
{
   vec_short8 lhs_short = (vec_short8)(vec_char16)lhs;
   vec_short8 rhs_short = (vec_short8)(vec_char16)rhs;

   vec_short8 hi_char = spu_add( lhs_short, spu_and( rhs_short, 0xff00 ) );
   vec_short8 lo_char = spu_add( lhs_short, rhs_short );

   return char16( (vec_char16)spu_sel( hi_char, lo_char, (vec_uchar16)spu_splats((short)0x00ff) ) );
}

inline
const char16
operator - ( char16 lhs, char16 rhs )
{
   vec_short8 lhs_short = (vec_short8)(vec_char16)lhs;
   vec_short8 rhs_short = (vec_short8)(vec_char16)rhs;
   
   vec_short8 hi_char = spu_sub( lhs_short, spu_and( rhs_short, 0xff00 ) );
   vec_short8 lo_char = spu_sub( lhs_short, rhs_short );

   return char16( (vec_char16)spu_sel( hi_char, lo_char, (vec_uchar16)spu_splats((short)0x00ff) ) );
}

inline
const char16
operator << ( char16 lhs, uchar16 rhs )
{
   vec_short8  lhs_short = (vec_short8)(vec_char16)lhs;
   vec_ushort8 rhs_ushort = (vec_ushort8)(vec_uchar16)rhs;
   
   vec_ushort8 hi_shift = spu_rlmask( rhs_ushort, -8 );
   vec_short8  hi_byte = spu_sl( spu_and( lhs_short, 0xff00 ), hi_shift );

   vec_short8  lo_byte = spu_sl( lhs_short, rhs_ushort );

   return char16( (vec_char16)spu_sel( hi_byte, lo_byte, (vec_uchar16)spu_splats((short)0x00ff) ) );
}

inline
const char16
operator >> ( char16 lhs, uchar16 rhs )
{
   vec_short8 lhs_short = (vec_short8)(vec_char16)lhs;
   vec_short8 rhs_short = (vec_short8)(vec_uchar16)rhs;
   
   vec_short8 hi_shift = spu_rlmask( rhs_short, -8 );
   vec_short8 hi_byte = spu_rlmaska( lhs_short, spu_sub( (short)0, hi_shift ) );

   vec_short8 lo_byte = spu_rlmaska( spu_extend((vec_char16)lhs), spu_sub( (short)0, rhs_short ) );

   return char16( (vec_char16)spu_sel( hi_byte, lo_byte, (vec_uchar16)spu_splats((short)0x00ff) ) );
}

inline
const bool16
operator < ( char16 lhs, char16 rhs )
{
   bool16 result;
   result.data = spu_cmpgt( (vec_char16)rhs, (vec_char16)lhs );
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
   result.data = spu_cmpgt( (vec_char16)lhs, (vec_char16)rhs );
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
   result.data = spu_cmpeq( (vec_char16)lhs, (vec_char16)rhs );
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
   return char16( spu_sel( (vec_char16)lhs, (vec_char16)rhs, (vec_uchar16)rhs_slots ) );
}

inline
const char16
operator & ( char16 lhs, char16 rhs )
{
   return char16( spu_and( (vec_char16)lhs, (vec_char16)rhs ) );
}

inline
const char16
operator | ( char16 lhs, char16 rhs )
{
   return char16( spu_or( (vec_char16)lhs, (vec_char16)rhs ) );
}

inline
const char16
operator ^ ( char16 lhs, char16 rhs )
{
   return char16( spu_xor( (vec_char16)lhs, (vec_char16)rhs ) );
}
   
inline
char16_idx::operator schar() const
{ 
   return (schar)spu_extract( (vec_char16)ref, i );
}

inline 
schar
char16_idx::operator = ( schar rhs )
{
   ref = spu_insert( (schar)rhs, (vec_char16)ref, i );
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
   schar tmp = spu_extract( (vec_char16)ref, i );
   ref = spu_insert( (signed char)(tmp+1), (vec_char16)ref, i );
   return tmp;
}

inline
schar
char16_idx::operator --( int )
{
   schar tmp = spu_extract( (vec_char16)ref, i );
   ref = spu_insert( (signed char)(tmp-1), (vec_char16)ref, i );
   return tmp;
}

inline
schar
char16_idx::operator ++()
{
   schar tmp = spu_extract( (vec_char16)ref, i );
   ++tmp;
   ref = spu_insert( tmp, (vec_char16)ref, i );
   return tmp;
}

inline
schar
char16_idx::operator --()
{
   schar tmp = spu_extract( (vec_char16)ref, i );
   --tmp;
   ref = spu_insert( tmp, (vec_char16)ref, i );
   return tmp;
}

inline
schar
char16_idx::operator *= ( schar rhs )
{
   schar tmp = spu_extract( (vec_char16)ref, i ) * rhs;
   ref = spu_insert( tmp, (vec_char16)ref, i );
   return tmp;
}

inline
schar
char16_idx::operator /= ( schar rhs )
{
   schar tmp = spu_extract( (vec_char16)ref, i ) / rhs;
   ref = spu_insert( tmp, (vec_char16)ref, i );
   return tmp;
}

inline
schar
char16_idx::operator %= ( schar rhs )
{
   schar tmp = spu_extract( (vec_char16)ref, i ) % rhs;
   ref = spu_insert( tmp, (vec_char16)ref, i );
   return tmp;
}

inline
schar
char16_idx::operator += ( schar rhs )
{
   schar tmp = spu_extract( (vec_char16)ref, i ) + rhs;
   ref = spu_insert( tmp, (vec_char16)ref, i );
   return tmp;
}

inline
schar
char16_idx::operator -= ( schar rhs )
{
   schar tmp = spu_extract( (vec_char16)ref, i ) - rhs;
   ref = spu_insert( tmp, (vec_char16)ref, i );
   return tmp;
}

inline
schar
char16_idx::operator <<= ( uchar rhs )
{
   schar tmp = spu_extract( (vec_char16)ref, i ) << rhs;
   ref = spu_insert( tmp, (vec_char16)ref, i );
   return tmp;
}

inline
schar
char16_idx::operator >>= ( uchar rhs )
{
   schar tmp = spu_extract( (vec_char16)ref, i ) >> rhs;
   ref = spu_insert( tmp, (vec_char16)ref, i );
   return tmp;
}

inline
schar
char16_idx::operator &= ( schar rhs )
{
   schar tmp = spu_extract( (vec_char16)ref, i ) & rhs;
   ref = spu_insert( tmp, (vec_char16)ref, i );
   return tmp;
}

inline
schar
char16_idx::operator ^= ( schar rhs )
{
   schar tmp = spu_extract( (vec_char16)ref, i ) ^ rhs;
   ref = spu_insert( tmp, (vec_char16)ref, i );
   return tmp;
}

inline
schar
char16_idx::operator |= ( schar rhs )
{
   schar tmp = spu_extract( (vec_char16)ref, i ) | rhs;
   ref = spu_insert( tmp, (vec_char16)ref, i );
   return tmp;
}

}; // namespace simd

#endif // char16_h
