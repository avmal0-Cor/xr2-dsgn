/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2008 Sony Computer Entertainment Inc. 
* All Rights Reserved.
*/

#ifndef _SCE_SIMD_UCHAR16_H
#define _SCE_SIMD_UCHAR16_H

#include <spu_intrinsics.h>
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
   data = spu_splats((uchar)rhs);
}

inline
uchar16::uchar16( vec_uchar16 rhs )
{
   data = rhs;
}

inline
uchar16::uchar16( bool16 rhs )
{
   data = (vec_uchar16)spu_and( (vec_uchar16)rhs, 1 );
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
   return (uchar)spu_extract((vec_uchar16)data, i);
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
   return uchar16( spu_nor( data, data ) );
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
   const vec_uchar16 pack = __extension__ (vec_uchar16){ 0x01, 0x03, 0x11, 0x13, 0x05, 0x07, 0x15, 0x17,
                                                         0x09, 0x0b, 0x19, 0x1b, 0x0d, 0x0f, 0x1d, 0x1f };

   vec_short8 lhs_short = (vec_short8)(vec_uchar16)lhs;
   vec_short8 rhs_short = (vec_short8)(vec_uchar16)rhs;
   
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

   return uchar16( (vec_uchar16)spu_shuffle( sum0, sum1, pack ) );
}

inline
const uchar16
operator / ( uchar16 num, uchar16 den )
{
   const vec_uchar16 pack = __extension__ (vec_uchar16){ 0x01, 0x11, 0x03, 0x13, 0x05, 0x15, 0x07, 0x17,
                                                         0x09, 0x19, 0x0b, 0x1b, 0x0d, 0x1d, 0x0f, 0x1f };
                                     
   vec_ushort8 numer = (vec_ushort8)(vec_uchar16)num;
   vec_ushort8 denom = (vec_ushort8)(vec_uchar16)den;
   
   ushort8 hi_num( spu_rlmask( numer, -8 ) );
   ushort8 hi_den( spu_rlmask( denom, -8 ) );
   ushort8 lo_num( spu_and( numer, 0x00ff ) );
   ushort8 lo_den( spu_and( denom, 0x00ff ) );
   
   ushort8 hi_res = hi_num / hi_den;
   ushort8 lo_res = lo_num / lo_den;

   return uchar16( (vec_uchar16)spu_shuffle( (vec_ushort8)hi_res, (vec_ushort8)lo_res, pack ) );
}

inline
const uchar16
operator % ( uchar16 num, uchar16 den )
{
   const vec_uchar16 pack = __extension__ (vec_uchar16){ 0x01, 0x11, 0x03, 0x13, 0x05, 0x15, 0x07, 0x17,
                                                         0x09, 0x19, 0x0b, 0x1b, 0x0d, 0x1d, 0x0f, 0x1f };
                                     
   vec_ushort8 numer = (vec_ushort8)(vec_uchar16)num;
   vec_ushort8 denom = (vec_ushort8)(vec_uchar16)den;
   
   ushort8 hi_num( spu_rlmask( numer, -8 ) );
   ushort8 hi_den( spu_rlmask( denom, -8 ) );
   ushort8 lo_num( spu_and( numer, 0x00ff ) );
   ushort8 lo_den( spu_and( denom, 0x00ff ) );
   
   ushort8 hi_res = hi_num % hi_den;
   ushort8 lo_res = lo_num % lo_den;

   return uchar16( (vec_uchar16)spu_shuffle( (vec_ushort8)hi_res, (vec_ushort8)lo_res, pack ) );
}

inline
const uchar16
operator + ( uchar16 lhs, uchar16 rhs )
{
   vec_short8 lhs_short = (vec_short8)(vec_uchar16)lhs;
   vec_short8 rhs_short = (vec_short8)(vec_uchar16)rhs;

   vec_short8 hi_char = spu_add( lhs_short, spu_and( rhs_short, 0xff00 ) );
   vec_short8 lo_char = spu_add( lhs_short, rhs_short );

   return uchar16( (vec_uchar16)spu_sel( hi_char, lo_char, (vec_uchar16)spu_splats((short)0x00ff) ) );
}

inline
const uchar16
operator - ( uchar16 lhs, uchar16 rhs )
{
   vec_short8 lhs_short = (vec_short8)(vec_uchar16)lhs;
   vec_short8 rhs_short = (vec_short8)(vec_uchar16)rhs;
   
   vec_short8 hi_char = spu_sub( lhs_short, spu_and( rhs_short, 0xff00 ) );
   vec_short8 lo_char = spu_sub( lhs_short, rhs_short );

   return uchar16( (vec_uchar16)spu_sel( hi_char, lo_char, (vec_uchar16)spu_splats((short)0x00ff) ) );
}

inline
const uchar16
operator << ( uchar16 lhs, uchar16 rhs )
{
   vec_short8  lhs_short = (vec_short8)(vec_uchar16)lhs;
   vec_ushort8 rhs_ushort = (vec_ushort8)(vec_uchar16)rhs;
   
   vec_ushort8 hi_shift = spu_rlmask( rhs_ushort, -8 );
   vec_short8  hi_byte = spu_sl( spu_and( lhs_short, 0xff00 ), hi_shift );

   vec_short8  lo_byte = spu_sl( lhs_short, rhs_ushort );

   return uchar16( (vec_uchar16)spu_sel( hi_byte, lo_byte, (vec_uchar16)spu_splats((short)0x00ff) ) );
}

inline
const uchar16
operator >> ( uchar16 lhs, uchar16 rhs )
{
   vec_short8 lhs_short = (vec_short8)(vec_uchar16)lhs;
   vec_short8 rhs_short = (vec_short8)(vec_uchar16)rhs;
   
   vec_short8 hi_shift = spu_rlmask( rhs_short, -8 );
   vec_short8 hi_byte = spu_rlmask( lhs_short, spu_sub( (short)0, hi_shift ) );

   vec_short8 lo_byte = spu_rlmask( spu_and( lhs_short, 0x00ff ), spu_sub( (short)0, rhs_short ) );

   return uchar16( (vec_uchar16)spu_sel( hi_byte, lo_byte, (vec_uchar16)spu_splats((short)0x00ff) ) );
}

inline
const bool16
operator < ( uchar16 lhs, uchar16 rhs )
{
   bool16 result;
   result.data = spu_cmpgt( (vec_uchar16)rhs, (vec_uchar16)lhs );
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
   result.data = spu_cmpgt( (vec_uchar16)lhs, (vec_uchar16)rhs );
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
   result.data = spu_cmpeq( (vec_uchar16)lhs, (vec_uchar16)rhs );
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
   return uchar16( spu_sel( (vec_uchar16)lhs, (vec_uchar16)rhs, (vec_uchar16)rhs_slots ) );
}

inline
const uchar16
operator & ( uchar16 lhs, uchar16 rhs )
{
   return uchar16( spu_and( (vec_uchar16)lhs, (vec_uchar16)rhs ) );
}

inline
const uchar16
operator | ( uchar16 lhs, uchar16 rhs )
{
   return uchar16( spu_or( (vec_uchar16)lhs, (vec_uchar16)rhs ) );
}

inline
const uchar16
operator ^ ( uchar16 lhs, uchar16 rhs )
{
   return uchar16( spu_xor( (vec_uchar16)lhs, (vec_uchar16)rhs ) );
}
   
inline
uchar16_idx::operator uchar() const
{ 
   return (uchar)spu_extract( (vec_uchar16)ref, i );
}

inline 
uchar
uchar16_idx::operator = ( uchar rhs )
{
   ref = spu_insert( (uchar)rhs, (vec_uchar16)ref, i );
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
   uchar tmp = spu_extract( (vec_uchar16)ref, i );
   ref = spu_insert( (unsigned char)(tmp+1), (vec_uchar16)ref, i );
   return tmp;
}

inline
uchar
uchar16_idx::operator --( int )
{
   uchar tmp = spu_extract( (vec_uchar16)ref, i );
   ref = spu_insert( (unsigned char)(tmp-1), (vec_uchar16)ref, i );
   return tmp;
}

inline
uchar
uchar16_idx::operator ++()
{
   uchar tmp = spu_extract( (vec_uchar16)ref, i );
   ++tmp;
   ref = spu_insert( tmp, (vec_uchar16)ref, i );
   return tmp;
}

inline
uchar
uchar16_idx::operator --()
{
   uchar tmp = spu_extract( (vec_uchar16)ref, i );
   --tmp;
   ref = spu_insert( tmp, (vec_uchar16)ref, i );
   return tmp;
}

inline
uchar
uchar16_idx::operator *= ( uchar rhs )
{
   uchar tmp = spu_extract( (vec_uchar16)ref, i ) * rhs;
   ref = spu_insert( tmp, (vec_uchar16)ref, i );
   return tmp;
}

inline
uchar
uchar16_idx::operator /= ( uchar rhs )
{
   uchar tmp = spu_extract( (vec_uchar16)ref, i ) / rhs;
   ref = spu_insert( tmp, (vec_uchar16)ref, i );
   return tmp;
}

inline
uchar
uchar16_idx::operator %= ( uchar rhs )
{
   uchar tmp = spu_extract( (vec_uchar16)ref, i ) % rhs;
   ref = spu_insert( tmp, (vec_uchar16)ref, i );
   return tmp;
}

inline
uchar
uchar16_idx::operator += ( uchar rhs )
{
   uchar tmp = spu_extract( (vec_uchar16)ref, i ) + rhs;
   ref = spu_insert( tmp, (vec_uchar16)ref, i );
   return tmp;
}

inline
uchar
uchar16_idx::operator -= ( uchar rhs )
{
   uchar tmp = spu_extract( (vec_uchar16)ref, i ) - rhs;
   ref = spu_insert( tmp, (vec_uchar16)ref, i );
   return tmp;
}

inline
uchar
uchar16_idx::operator <<= ( uchar rhs )
{
   uchar tmp = spu_extract( (vec_uchar16)ref, i ) << rhs;
   ref = spu_insert( tmp, (vec_uchar16)ref, i );
   return tmp;
}

inline
uchar
uchar16_idx::operator >>= ( uchar rhs )
{
   uchar tmp = spu_extract( (vec_uchar16)ref, i ) >> rhs;
   ref = spu_insert( tmp, (vec_uchar16)ref, i );
   return tmp;
}

inline
uchar
uchar16_idx::operator &= ( uchar rhs )
{
   uchar tmp = spu_extract( (vec_uchar16)ref, i ) & rhs;
   ref = spu_insert( tmp, (vec_uchar16)ref, i );
   return tmp;
}

inline
uchar
uchar16_idx::operator ^= ( uchar rhs )
{
   uchar tmp = spu_extract( (vec_uchar16)ref, i ) ^ rhs;
   ref = spu_insert( tmp, (vec_uchar16)ref, i );
   return tmp;
}

inline
uchar
uchar16_idx::operator |= ( uchar rhs )
{
   uchar tmp = spu_extract( (vec_uchar16)ref, i ) | rhs;
   ref = spu_insert( tmp, (vec_uchar16)ref, i );
   return tmp;
}

}; // namespace simd

#endif // uchar16_h
