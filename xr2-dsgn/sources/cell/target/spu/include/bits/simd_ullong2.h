/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2008 Sony Computer Entertainment Inc. 
* All Rights Reserved.
*/

#ifndef _SCE_SIMD_ULLONG2_H
#define _SCE_SIMD_ULLONG2_H

#include <spu_intrinsics.h>
#include "simd_scalars.h"

namespace simd {

class bool2;
class llong2;
class double2;
class ullong2_idx;

//--------------------------------------------------------------------------------------------------
// ullong2 class
//

class ullong2
{
   private:
      vec_ullong2 data;

   public:
      inline ullong2() {}
      inline ullong2( bool2 );
      inline ullong2( llong2 );
      inline ullong2( double2 );
      inline ullong2( ullong r0, ullong r1 );

      // set all slots to same ullong

      explicit inline ullong2( ullong );

      // allows implicit conversion from vector unsigned long long

      inline ullong2( vec_ullong2 rhs );

      // allows implicit conversion to vector unsigned long long

      inline operator vector unsigned long long() const;

      // use array index operator to set and get slots

      inline ullong2_idx operator [] ( int i );
      inline ullong operator [] ( int i ) const;

      inline const ullong2 operator ++ ( int );
      inline const ullong2 operator -- ( int );
      inline ullong2& operator ++ ();
      inline ullong2& operator -- ();

      inline const ullong2 operator ~ () const;
      inline const bool2 operator ! () const;
      inline const llong2 operator - () const;

      inline ullong2& operator = ( ullong2 rhs );
      inline ullong2& operator *= ( ullong2 rhs );
      inline ullong2& operator /= ( ullong2 rhs );
      inline ullong2& operator %= ( ullong2 rhs );
      inline ullong2& operator += ( ullong2 rhs );
      inline ullong2& operator -= ( ullong2 rhs );
      inline ullong2& operator <<= ( ullong2 rhs );
      inline ullong2& operator >>= ( ullong2 rhs );
      inline ullong2& operator &= ( ullong2 rhs );
      inline ullong2& operator ^= ( ullong2 rhs );
      inline ullong2& operator |= ( ullong2 rhs );
};

//--------------------------------------------------------------------------------------------------
// functions for ullong2
//

inline const ullong2 operator * ( ullong2 lhs, ullong2 rhs );
inline const ullong2 operator / ( ullong2 lhs, ullong2 rhs );
inline const ullong2 operator % ( ullong2 lhs, ullong2 rhs );
inline const ullong2 operator + ( ullong2 lhs, ullong2 rhs );
inline const ullong2 operator - ( ullong2 lhs, ullong2 rhs );

inline const ullong2 operator << ( ullong2 lhs, ullong2 rhs );
inline const ullong2 operator >> ( ullong2 lhs, ullong2 rhs );

inline const bool2 operator < ( ullong2 lhs, ullong2 rhs );
inline const bool2 operator <= ( ullong2 lhs, ullong2 rhs );
inline const bool2 operator > ( ullong2 lhs, ullong2 rhs );
inline const bool2 operator >= ( ullong2 lhs, ullong2 rhs );
inline const bool2 operator == ( ullong2 lhs, ullong2 rhs );
inline const bool2 operator != ( ullong2 lhs, ullong2 rhs );

// select between lhs and rhs slots using bool2.
// false selects lhs slot, true selects rhs slot

inline const ullong2 select( ullong2 lhs, ullong2 rhs, bool2 rhs_slots );

inline const ullong2 operator & ( ullong2 lhs, ullong2 rhs );
inline const ullong2 operator ^ ( ullong2 lhs, ullong2 rhs );
inline const ullong2 operator | ( ullong2 lhs, ullong2 rhs );

//--------------------------------------------------------------------------------------------------
// ullong2_idx: for setting ullong2 slots with [] operator
//

class ullong2_idx
{
   private:
      ullong2 &ref __attribute__ ((aligned(16)));
      int i __attribute__ ((aligned(16)));
   public:
      inline ullong2_idx( ullong2& vec, int idx ): ref(vec) { i = idx; }
      inline operator ullong() const;
      inline ullong operator = ( ullong rhs );
      inline ullong operator = ( const ullong2_idx& rhs );
      inline ullong operator ++ ( int );
      inline ullong operator -- ( int );
      inline ullong operator ++ ();
      inline ullong operator -- ();
      inline ullong operator *= ( ullong rhs );
      inline ullong operator /= ( ullong rhs );
      inline ullong operator %= ( ullong rhs );
      inline ullong operator += ( ullong rhs );
      inline ullong operator -= ( ullong rhs );
      inline ullong operator <<= ( ullong rhs );
      inline ullong operator >>= ( ullong rhs );
      inline ullong operator &= ( ullong rhs );
      inline ullong operator ^= ( ullong rhs );
      inline ullong operator |= ( ullong rhs );
};

}; // namespace simd

#include "simd_bool2.h"
#include "simd_llong2.h"
#include "simd_double2.h"

namespace simd {

inline
ullong2::ullong2( ullong rhs )
{
   data = spu_splats((ullong)rhs);
}

inline
ullong2::ullong2( vec_ullong2 rhs )
{
   data = rhs;
}

inline
ullong2::ullong2( bool2 rhs )
{
   data = (vec_ullong2)spu_and( (vec_ullong2)rhs, spu_splats((ullong)1) );
}

inline
ullong2::ullong2( llong2 rhs )
{
   data = (vec_ullong2)(vec_llong2)rhs;
}

inline
ullong2::ullong2( double2 rhs )
{
   data = __extension__ (vec_ullong2){ (ullong)spu_extract( (vec_double2)rhs, 0 ), (ullong)spu_extract( (vec_double2)rhs, 1 ) };
}

inline
ullong2::ullong2( ullong r0, ullong r1 )
{
   data = __extension__ (vec_ullong2){ r0, r1 };
}

inline
ullong2::operator vector unsigned long long() const
{
   return data;
}

inline
ullong2_idx
ullong2::operator [] ( int i )
{
   return ullong2_idx(*this,i);
}

inline
ullong
ullong2::operator [] ( int i ) const
{
   return (ullong)spu_extract((vec_ullong2)data, i);
}

inline
const ullong2
ullong2::operator ++ ( int )
{
   vec_ullong2 olddata = data;
   operator ++();
   return ullong2(olddata);
}

inline
const ullong2
ullong2::operator -- ( int )
{
   vec_ullong2 olddata = data;
   operator --();
   return ullong2(olddata);
}

inline
ullong2&
ullong2::operator ++ ()
{
   *this += ullong2(1);
   return *this;
}

inline
ullong2&
ullong2::operator -- ()
{
   *this -= ullong2(1);
   return *this;
}

inline
const ullong2 
ullong2::operator ~ () const
{
   return ullong2( spu_nor( data, data ) );
}

inline
const bool2
ullong2::operator ! () const
{
   return ( *this == ullong2(0) );
}

inline
const llong2
ullong2::operator - () const
{
   return ullong2(0) - *this;
}

inline
ullong2&
ullong2::operator = ( ullong2 rhs )
{
   data = rhs.data;
   return *this;
}

inline
ullong2&
ullong2::operator *= ( ullong2 rhs )
{
   *this = *this * rhs;
   return *this;
}

inline
ullong2&
ullong2::operator /= ( ullong2 rhs )
{
   *this = *this / rhs;
   return *this;
}

inline
ullong2&
ullong2::operator %= ( ullong2 rhs )
{
   *this = *this % rhs;
   return *this;
}

inline
ullong2&
ullong2::operator += ( ullong2 rhs )
{
   *this = *this + rhs;
   return *this;
}

inline
ullong2&
ullong2::operator -= ( ullong2 rhs )
{
   *this = *this - rhs;
   return *this;
}

inline
ullong2&
ullong2::operator <<= ( ullong2 rhs )
{
   *this = *this << rhs;
   return *this;
}

inline
ullong2&
ullong2::operator >>= ( ullong2 rhs )
{
   *this = *this >> rhs;
   return *this;
}

inline
ullong2&
ullong2::operator &= ( ullong2 rhs )
{
   *this = *this & rhs;
   return *this;
}

inline
ullong2&
ullong2::operator ^= ( ullong2 rhs )
{
   *this = *this ^ rhs;
   return *this;
}

inline
ullong2&
ullong2::operator |= ( ullong2 rhs )
{
   *this = *this | rhs;
   return *this;
}

inline
const ullong2
operator * ( ullong2 lhs, ullong2 rhs )
{
   return ullong2( spu_extract( (vec_ullong2)lhs, 0 ) * spu_extract( (vec_ullong2)rhs, 0 ),
                   spu_extract( (vec_ullong2)lhs, 1 ) * spu_extract( (vec_ullong2)rhs, 1 ) );
}

inline
const ullong2
operator / ( ullong2 num, ullong2 den )
{
   return ullong2( spu_extract( (vec_ullong2)num, 0 ) / spu_extract( (vec_ullong2)den, 0 ),
                   spu_extract( (vec_ullong2)num, 1 ) / spu_extract( (vec_ullong2)den, 1 ) );
}

inline
const ullong2
operator % ( ullong2 num, ullong2 den )
{
   return ullong2( spu_extract( (vec_ullong2)num, 0 ) % spu_extract( (vec_ullong2)den, 0 ),
                   spu_extract( (vec_ullong2)num, 1 ) % spu_extract( (vec_ullong2)den, 1 ) );
}

inline
const ullong2
operator + ( ullong2 lhs, ullong2 rhs )
{
   const vec_uchar16 shiftcarry = __extension__ (vec_uchar16)(vec_uint4){ 0x04050607, 0x80808080, 0x0c0d0e0f, 0x80808080 };
   vec_uint4 ldata, rdata, carry;

   ldata = (vec_uint4)(vec_ullong2)lhs;
   rdata = (vec_uint4)(vec_ullong2)rhs;
   carry = spu_genc( ldata, rdata );
   carry = spu_shuffle( carry, carry, shiftcarry );

   return ullong2( (vec_ullong2)spu_addx( ldata, rdata, carry ) );
}

inline
const ullong2
operator - ( ullong2 lhs, ullong2 rhs )
{
   const vec_uchar16 shiftborrow = __extension__ (vec_uchar16)(vec_uint4){ 0x04050607, 0xc0c0c0c0, 0x0c0d0e0f, 0xc0c0c0c0 };
   vec_uint4 ldata, rdata, borrow;
   
   ldata = (vec_uint4)(vec_ullong2)lhs;
   rdata = (vec_uint4)(vec_ullong2)rhs;
   borrow = spu_genb( ldata, rdata );
   borrow = spu_shuffle( borrow, borrow, shiftborrow );
   
   return ullong2( (vec_ullong2)spu_subx( ldata, rdata, borrow ) );
}

inline
const ullong2
operator << ( ullong2 lhs, ullong2 rhs )
{
   return ullong2( spu_extract( (vec_ullong2)lhs, 0 ) << spu_extract( (vec_ullong2)rhs, 0 ),
                   spu_extract( (vec_ullong2)lhs, 1 ) << spu_extract( (vec_ullong2)rhs, 1 ) );
}

inline
const ullong2
operator >> ( ullong2 lhs, ullong2 rhs )
{
   return ullong2( spu_extract( (vec_ullong2)lhs, 0 ) >> spu_extract( (vec_ullong2)rhs, 0 ),
                   spu_extract( (vec_ullong2)lhs, 1 ) >> spu_extract( (vec_ullong2)rhs, 1 ) );
}

inline
const bool2
operator < ( ullong2 lhs, ullong2 rhs )
{
   return rhs > lhs;
}

inline
const bool2
operator <= ( ullong2 lhs, ullong2 rhs )
{
   return !( lhs > rhs );
}

inline
const bool2
operator > ( ullong2 lhs, ullong2 rhs )
{
   const vec_uchar16 even = __extension__ (vec_uchar16)(vec_uint4){ 0x00010203, 0x00010203, 0x08090a0b, 0x08090a0b };
   const vec_uchar16 odd = __extension__ (vec_uchar16)(vec_uint4){ 0x04050607, 0x04050607, 0x0c0d0e0f, 0x0c0d0e0f };   
   bool2 result;
   vec_uint4 cmpgtui, cmpequi;
   vec_ullong2 cmpgt;
   
   cmpgtui = spu_cmpgt( (vec_uint4)(vec_ullong2)lhs, (vec_uint4)(vec_ullong2)rhs );
   cmpequi = spu_cmpeq( (vec_uint4)(vec_ullong2)lhs, (vec_uint4)(vec_ullong2)rhs );
   cmpgt = (vec_ullong2)spu_or( spu_shuffle( cmpgtui, cmpgtui, even ),
                                spu_and( spu_shuffle( cmpequi, cmpequi, even ),
                                         spu_shuffle( cmpgtui, cmpgtui, odd ) ) );
   result.data = cmpgt;
   return result;
}

inline
const bool2
operator >= ( ullong2 lhs, ullong2 rhs )
{
   return !( rhs > lhs );
}

inline
const bool2
operator == ( ullong2 lhs, ullong2 rhs )
{
   const vec_uchar16 even = __extension__ (vec_uchar16)(vec_uint4){ 0x00010203, 0x00010203, 0x08090a0b, 0x08090a0b };
   const vec_uchar16 odd = __extension__ (vec_uchar16)(vec_uint4){ 0x04050607, 0x04050607, 0x0c0d0e0f, 0x0c0d0e0f };
   bool2 result;
   vec_uint4 cmpequi;
   vec_ullong2 cmpeq;
   
   cmpequi = spu_cmpeq( (vec_uint4)(vec_ullong2)lhs, (vec_uint4)(vec_ullong2)rhs );
   cmpeq = (vec_ullong2)spu_and( spu_shuffle( cmpequi, cmpequi, even ), spu_shuffle( cmpequi, cmpequi, odd ) );
   result.data = cmpeq;
   return result;
}

inline
const bool2
operator != ( ullong2 lhs, ullong2 rhs )
{
   return !( lhs == rhs );
}
   
inline
const ullong2
select( ullong2 lhs, ullong2 rhs, bool2 rhs_slots )
{
   return ullong2( spu_sel( (vec_ullong2)lhs, (vec_ullong2)rhs, (vec_ullong2)rhs_slots ) );
}

inline
const ullong2
operator & ( ullong2 lhs, ullong2 rhs )
{
   return ullong2( spu_and( (vec_ullong2)lhs, (vec_ullong2)rhs ) );
}

inline
const ullong2
operator | ( ullong2 lhs, ullong2 rhs )
{
   return ullong2( spu_or( (vec_ullong2)lhs, (vec_ullong2)rhs ) );
}

inline
const ullong2
operator ^ ( ullong2 lhs, ullong2 rhs )
{
   return ullong2( spu_xor( (vec_ullong2)lhs, (vec_ullong2)rhs ) );
}
   
inline
ullong2_idx::operator ullong() const
{ 
   return (ullong)spu_extract( (vec_ullong2)ref, i );
}

inline 
ullong
ullong2_idx::operator = ( ullong rhs )
{
   ref = spu_insert( (ullong)rhs, (vec_ullong2)ref, i );
   return rhs;
}

inline
ullong
ullong2_idx::operator = ( const ullong2_idx& rhs )
{
   return ( *this = (ullong)rhs );
}

inline
ullong
ullong2_idx::operator ++( int )
{
   ullong tmp = spu_extract( (vec_ullong2)ref, i );
   ref = spu_insert( tmp+1, (vec_ullong2)ref, i );
   return tmp;
}

inline
ullong
ullong2_idx::operator --( int )
{
   ullong tmp = spu_extract( (vec_ullong2)ref, i );
   ref = spu_insert( tmp-1, (vec_ullong2)ref, i );
   return tmp;
}

inline
ullong
ullong2_idx::operator ++()
{
   ullong tmp = spu_extract( (vec_ullong2)ref, i );
   ++tmp;
   ref = spu_insert( tmp, (vec_ullong2)ref, i );
   return tmp;
}

inline
ullong
ullong2_idx::operator --()
{
   ullong tmp = spu_extract( (vec_ullong2)ref, i );
   --tmp;
   ref = spu_insert( tmp, (vec_ullong2)ref, i );
   return tmp;
}

inline
ullong
ullong2_idx::operator *= ( ullong rhs )
{
   ullong tmp = spu_extract( (vec_ullong2)ref, i ) * rhs;
   ref = spu_insert( tmp, (vec_ullong2)ref, i );
   return tmp;
}

inline
ullong
ullong2_idx::operator /= ( ullong rhs )
{
   ullong tmp = spu_extract( (vec_ullong2)ref, i ) / rhs;
   ref = spu_insert( tmp, (vec_ullong2)ref, i );
   return tmp;
}

inline
ullong
ullong2_idx::operator %= ( ullong rhs )
{
   ullong tmp = spu_extract( (vec_ullong2)ref, i ) % rhs;
   ref = spu_insert( tmp, (vec_ullong2)ref, i );
   return tmp;
}

inline
ullong
ullong2_idx::operator += ( ullong rhs )
{
   ullong tmp = spu_extract( (vec_ullong2)ref, i ) + rhs;
   ref = spu_insert( tmp, (vec_ullong2)ref, i );
   return tmp;
}

inline
ullong
ullong2_idx::operator -= ( ullong rhs )
{
   ullong tmp = spu_extract( (vec_ullong2)ref, i ) - rhs;
   ref = spu_insert( tmp, (vec_ullong2)ref, i );
   return tmp;
}

inline
ullong
ullong2_idx::operator <<= ( ullong rhs )
{
   ullong tmp = spu_extract( (vec_ullong2)ref, i ) << rhs;
   ref = spu_insert( tmp, (vec_ullong2)ref, i );
   return tmp;
}

inline
ullong
ullong2_idx::operator >>= ( ullong rhs )
{
   ullong tmp = spu_extract( (vec_ullong2)ref, i ) >> rhs;
   ref = spu_insert( tmp, (vec_ullong2)ref, i );
   return tmp;
}

inline
ullong
ullong2_idx::operator &= ( ullong rhs )
{
   ullong tmp = spu_extract( (vec_ullong2)ref, i ) & rhs;
   ref = spu_insert( tmp, (vec_ullong2)ref, i );
   return tmp;
}

inline
ullong
ullong2_idx::operator ^= ( ullong rhs )
{
   ullong tmp = spu_extract( (vec_ullong2)ref, i ) ^ rhs;
   ref = spu_insert( tmp, (vec_ullong2)ref, i );
   return tmp;
}

inline
ullong
ullong2_idx::operator |= ( ullong rhs )
{
   ullong tmp = spu_extract( (vec_ullong2)ref, i ) | rhs;
   ref = spu_insert( tmp, (vec_ullong2)ref, i );
   return tmp;
}

}; // namespace simd

#endif // ullong2_h
