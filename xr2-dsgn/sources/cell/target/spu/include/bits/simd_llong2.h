/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2008 Sony Computer Entertainment Inc. 
* All Rights Reserved.
*/

#ifndef _SCE_SIMD_LLONG2_H
#define _SCE_SIMD_LLONG2_H

#include <spu_intrinsics.h>
#include "simd_scalars.h"

namespace simd {

class bool2;
class ullong2;
class double2;
class llong2_idx;

//--------------------------------------------------------------------------------------------------
// llong2 class
//

class llong2
{
   private:
      vec_llong2 data;

   public:
      inline llong2() {}
      inline llong2( bool2 );
      inline llong2( ullong2 );
      inline llong2( double2 );
      inline llong2( llong r0, llong r1 );

      // set all slots to same llong

      explicit inline llong2( llong );

      // allows implicit conversion from vector long long

      inline llong2( vec_llong2 rhs );

      // allows implicit conversion to vector long long

      inline operator vector signed long long() const;

      // use array index operator to set and get slots

      inline llong2_idx operator [] ( int i );
      inline llong operator [] ( int i ) const;

      inline const llong2 operator ++ ( int );
      inline const llong2 operator -- ( int );
      inline llong2& operator ++ ();
      inline llong2& operator -- ();

      inline const llong2 operator ~ () const;
      inline const bool2 operator ! () const;
      inline const llong2 operator - () const;

      inline llong2& operator = ( llong2 rhs );
      inline llong2& operator *= ( llong2 rhs );
      inline llong2& operator /= ( llong2 rhs );
      inline llong2& operator %= ( llong2 rhs );
      inline llong2& operator += ( llong2 rhs );
      inline llong2& operator -= ( llong2 rhs );
      inline llong2& operator <<= ( ullong2 rhs );
      inline llong2& operator >>= ( ullong2 rhs );
      inline llong2& operator &= ( llong2 rhs );
      inline llong2& operator ^= ( llong2 rhs );
      inline llong2& operator |= ( llong2 rhs );
};

//--------------------------------------------------------------------------------------------------
// functions for llong2
//

inline const llong2 operator * ( llong2 lhs, llong2 rhs );
inline const llong2 operator / ( llong2 lhs, llong2 rhs );
inline const llong2 operator % ( llong2 lhs, llong2 rhs );
inline const llong2 operator + ( llong2 lhs, llong2 rhs );
inline const llong2 operator - ( llong2 lhs, llong2 rhs );

inline const llong2 operator << ( llong2 lhs, ullong2 rhs );
inline const llong2 operator >> ( llong2 lhs, ullong2 rhs );

inline const bool2 operator < ( llong2 lhs, llong2 rhs );
inline const bool2 operator <= ( llong2 lhs, llong2 rhs );
inline const bool2 operator > ( llong2 lhs, llong2 rhs );
inline const bool2 operator >= ( llong2 lhs, llong2 rhs );
inline const bool2 operator == ( llong2 lhs, llong2 rhs );
inline const bool2 operator != ( llong2 lhs, llong2 rhs );

// select between lhs and rhs slots using bool2.
// false selects lhs slot, true selects rhs slot

inline const llong2 select( llong2 lhs, llong2 rhs, bool2 rhs_slots );

inline const llong2 operator & ( llong2 lhs, llong2 rhs );
inline const llong2 operator ^ ( llong2 lhs, llong2 rhs );
inline const llong2 operator | ( llong2 lhs, llong2 rhs );

//--------------------------------------------------------------------------------------------------
// llong2_idx: for setting llong2 slots with [] operator
//

class llong2_idx
{
   private:
      llong2 &ref __attribute__ ((aligned(16)));
      int i __attribute__ ((aligned(16)));
   public:
      inline llong2_idx( llong2& vec, int idx ): ref(vec) { i = idx; }
      inline operator llong() const;
      inline llong operator = ( llong rhs );
      inline llong operator = ( const llong2_idx& rhs );
      inline llong operator ++ ( int );
      inline llong operator -- ( int );
      inline llong operator ++ ();
      inline llong operator -- ();
      inline llong operator *= ( llong rhs );
      inline llong operator /= ( llong rhs );
      inline llong operator %= ( llong rhs );
      inline llong operator += ( llong rhs );
      inline llong operator -= ( llong rhs );
      inline llong operator <<= ( ullong rhs );
      inline llong operator >>= ( ullong rhs );
      inline llong operator &= ( llong rhs );
      inline llong operator ^= ( llong rhs );
      inline llong operator |= ( llong rhs );
};

}; // namespace simd

#include "simd_bool2.h"
#include "simd_ullong2.h"
#include "simd_double2.h"

namespace simd {

inline
llong2::llong2( llong rhs )
{
   data = spu_splats((llong)rhs);
}

inline
llong2::llong2( vec_llong2 rhs )
{
   data = rhs;
}

inline
llong2::llong2( bool2 rhs )
{
   data = (vec_llong2)spu_and( (vec_ullong2)rhs, spu_splats((ullong)1) );
}

inline
llong2::llong2( ullong2 rhs )
{
   data = (vec_llong2)(vec_ullong2)rhs;
}

inline
llong2::llong2( double2 rhs )
{
   data = __extension__ (vec_llong2){ (llong)spu_extract( (vec_double2)rhs, 0 ), (llong)spu_extract( (vec_double2)rhs, 1 ) };
}

inline
llong2::llong2( llong r0, llong r1 )
{
   data = __extension__ (vec_llong2){ r0, r1 };
}

inline
llong2::operator vector signed long long() const
{
   return data;
}

inline
llong2_idx
llong2::operator [] ( int i )
{
   return llong2_idx(*this,i);
}

inline
llong
llong2::operator [] ( int i ) const
{
   return (llong)spu_extract((vec_llong2)data, i);
}

inline
const llong2
llong2::operator ++ ( int )
{
   vec_llong2 olddata = data;
   operator ++();
   return llong2(olddata);
}

inline
const llong2
llong2::operator -- ( int )
{
   vec_llong2 olddata = data;
   operator --();
   return llong2(olddata);
}

inline
llong2&
llong2::operator ++ ()
{
   *this += llong2(1);
   return *this;
}

inline
llong2&
llong2::operator -- ()
{
   *this -= llong2(1);
   return *this;
}

inline
const llong2 
llong2::operator ~ () const
{
   return llong2( spu_nor( data, data ) );
}

inline
const bool2
llong2::operator ! () const
{
   return ( *this == llong2(0) );
}

inline
const llong2
llong2::operator - () const
{
   return llong2(0) - *this;
}

inline
llong2&
llong2::operator = ( llong2 rhs )
{
   data = rhs.data;
   return *this;
}

inline
llong2&
llong2::operator *= ( llong2 rhs )
{
   *this = *this * rhs;
   return *this;
}

inline
llong2&
llong2::operator /= ( llong2 rhs )
{
   *this = *this / rhs;
   return *this;
}

inline
llong2&
llong2::operator %= ( llong2 rhs )
{
   *this = *this % rhs;
   return *this;
}

inline
llong2&
llong2::operator += ( llong2 rhs )
{
   *this = *this + rhs;
   return *this;
}

inline
llong2&
llong2::operator -= ( llong2 rhs )
{
   *this = *this - rhs;
   return *this;
}

inline
llong2&
llong2::operator <<= ( ullong2 rhs )
{
   *this = *this << rhs;
   return *this;
}

inline
llong2&
llong2::operator >>= ( ullong2 rhs )
{
   *this = *this >> rhs;
   return *this;
}

inline
llong2&
llong2::operator &= ( llong2 rhs )
{
   *this = *this & rhs;
   return *this;
}

inline
llong2&
llong2::operator ^= ( llong2 rhs )
{
   *this = *this ^ rhs;
   return *this;
}

inline
llong2&
llong2::operator |= ( llong2 rhs )
{
   *this = *this | rhs;
   return *this;
}

inline
const llong2
operator * ( llong2 lhs, llong2 rhs )
{
   return llong2( spu_extract( (vec_llong2)lhs, 0 ) * spu_extract( (vec_llong2)rhs, 0 ),
                  spu_extract( (vec_llong2)lhs, 1 ) * spu_extract( (vec_llong2)rhs, 1 ) );
}

inline
const llong2
operator / ( llong2 num, llong2 den )
{
   return llong2( spu_extract( (vec_llong2)num, 0 ) / spu_extract( (vec_llong2)den, 0 ),
                  spu_extract( (vec_llong2)num, 1 ) / spu_extract( (vec_llong2)den, 1 ) );
}

inline
const llong2
operator % ( llong2 num, llong2 den )
{
   return llong2( spu_extract( (vec_llong2)num, 0 ) % spu_extract( (vec_llong2)den, 0 ),
                  spu_extract( (vec_llong2)num, 1 ) % spu_extract( (vec_llong2)den, 1 ) );
}

inline
const llong2
operator + ( llong2 lhs, llong2 rhs )
{
   const vec_uchar16 shiftcarry = __extension__ (vec_uchar16)(vec_uint4){ 0x04050607, 0x80808080, 0x0c0d0e0f, 0x80808080 };
   vec_uint4 ldata, rdata, carry;

   ldata = (vec_uint4)(vec_llong2)lhs;
   rdata = (vec_uint4)(vec_llong2)rhs;
   carry = spu_genc( ldata, rdata );
   carry = spu_shuffle( carry, carry, shiftcarry );

   return llong2( (vec_llong2)spu_addx( ldata, rdata, carry ) );
}

inline
const llong2
operator - ( llong2 lhs, llong2 rhs )
{
   const vec_uchar16 shiftborrow = __extension__ (vec_uchar16)(vec_uint4){ 0x04050607, 0xc0c0c0c0, 0x0c0d0e0f, 0xc0c0c0c0 };
   vec_uint4 ldata, rdata, borrow;
   
   ldata = (vec_uint4)(vec_llong2)lhs;
   rdata = (vec_uint4)(vec_llong2)rhs;
   borrow = spu_genb( ldata, rdata );
   borrow = spu_shuffle( borrow, borrow, shiftborrow );
   
   return llong2( (vec_llong2)spu_subx( ldata, rdata, borrow ) );
}

inline
const llong2
operator << ( llong2 lhs, ullong2 rhs )
{
   return llong2( spu_extract( (vec_llong2)lhs, 0 ) << spu_extract( (vec_ullong2)rhs, 0 ),
                  spu_extract( (vec_llong2)lhs, 1 ) << spu_extract( (vec_ullong2)rhs, 1 ) );
}

inline
const llong2
operator >> ( llong2 lhs, ullong2 rhs )
{
   return llong2( spu_extract( (vec_llong2)lhs, 0 ) >> spu_extract( (vec_ullong2)rhs, 0 ),
                  spu_extract( (vec_llong2)lhs, 1 ) >> spu_extract( (vec_ullong2)rhs, 1 ) );
}

inline
const bool2
operator < ( llong2 lhs, llong2 rhs )
{
   return rhs > lhs;
}

inline
const bool2
operator <= ( llong2 lhs, llong2 rhs )
{
   return !( lhs > rhs );
}

inline
const bool2
operator > ( llong2 lhs, llong2 rhs )
{
   const vec_uchar16 even = __extension__ (vec_uchar16)(vec_uint4){ 0x00010203, 0x00010203, 0x08090a0b, 0x08090a0b };
   const vec_uchar16 odd = __extension__ (vec_uchar16)(vec_uint4){ 0x04050607, 0x04050607, 0x0c0d0e0f, 0x0c0d0e0f };
   bool2 result;
   vec_llong2 ldata, rdata;
   vec_uint4 cmpgti, cmpeqi, cmpgtui;
   vec_ullong2 cmpgt;
   
   ldata = (vec_llong2)lhs;
   rdata = (vec_llong2)rhs;
   cmpgti = spu_cmpgt( (vec_int4)ldata, (vec_int4)rdata );
   cmpeqi = spu_cmpeq( (vec_int4)ldata, (vec_int4)rdata );
   cmpgtui = spu_cmpgt( (vec_uint4)ldata, (vec_uint4)rdata );
   cmpgt = (vec_ullong2)spu_or( spu_shuffle( cmpgti, cmpgti, even ),
                                spu_and( spu_shuffle( cmpeqi, cmpeqi, even ),
                                         spu_shuffle( cmpgtui, cmpgtui, odd ) ) );
   result.data = cmpgt;
   return result;
}

inline
const bool2
operator >= ( llong2 lhs, llong2 rhs )
{
   return !( rhs > lhs );
}

inline
const bool2
operator == ( llong2 lhs, llong2 rhs )
{
   const vec_uchar16 even = __extension__ (vec_uchar16)(vec_uint4){ 0x00010203, 0x00010203, 0x08090a0b, 0x08090a0b };
   const vec_uchar16 odd = __extension__ (vec_uchar16)(vec_uint4){ 0x04050607, 0x04050607, 0x0c0d0e0f, 0x0c0d0e0f };
   bool2 result;
   vec_uint4 cmpeqi;
   vec_ullong2 cmpeq;
   
   cmpeqi = spu_cmpeq( (vec_int4)(vec_llong2)lhs, (vec_int4)(vec_llong2)rhs );
   cmpeq = (vec_ullong2)spu_and( spu_shuffle( cmpeqi, cmpeqi, even ), spu_shuffle( cmpeqi, cmpeqi, odd ) );
   result.data = cmpeq;
   return result;
}

inline
const bool2
operator != ( llong2 lhs, llong2 rhs )
{
   return !( lhs == rhs );
}
   
inline
const llong2
select( llong2 lhs, llong2 rhs, bool2 rhs_slots )
{
   return llong2( spu_sel( (vec_llong2)lhs, (vec_llong2)rhs, (vec_ullong2)rhs_slots ) );
}

inline
const llong2
operator & ( llong2 lhs, llong2 rhs )
{
   return llong2( spu_and( (vec_llong2)lhs, (vec_llong2)rhs ) );
}

inline
const llong2
operator | ( llong2 lhs, llong2 rhs )
{
   return llong2( spu_or( (vec_llong2)lhs, (vec_llong2)rhs ) );
}

inline
const llong2
operator ^ ( llong2 lhs, llong2 rhs )
{
   return llong2( spu_xor( (vec_llong2)lhs, (vec_llong2)rhs ) );
}
   
inline
llong2_idx::operator llong() const
{ 
   return (llong)spu_extract( (vec_llong2)ref, i );
}

inline 
llong
llong2_idx::operator = ( llong rhs )
{
   ref = spu_insert( (llong)rhs, (vec_llong2)ref, i );
   return rhs;
}

inline
llong
llong2_idx::operator = ( const llong2_idx& rhs )
{
   return ( *this = (llong)rhs );
}

inline
llong
llong2_idx::operator ++( int )
{
   llong tmp = spu_extract( (vec_llong2)ref, i );
   ref = spu_insert( tmp+1, (vec_llong2)ref, i );
   return tmp;
}

inline
llong
llong2_idx::operator --( int )
{
   llong tmp = spu_extract( (vec_llong2)ref, i );
   ref = spu_insert( tmp-1, (vec_llong2)ref, i );
   return tmp;
}

inline
llong
llong2_idx::operator ++()
{
   llong tmp = spu_extract( (vec_llong2)ref, i );
   ++tmp;
   ref = spu_insert( tmp, (vec_llong2)ref, i );
   return tmp;
}

inline
llong
llong2_idx::operator --()
{
   llong tmp = spu_extract( (vec_llong2)ref, i );
   --tmp;
   ref = spu_insert( tmp, (vec_llong2)ref, i );
   return tmp;
}

inline
llong
llong2_idx::operator *= ( llong rhs )
{
   llong tmp = spu_extract( (vec_llong2)ref, i ) * rhs;
   ref = spu_insert( tmp, (vec_llong2)ref, i );
   return tmp;
}

inline
llong
llong2_idx::operator /= ( llong rhs )
{
   llong tmp = spu_extract( (vec_llong2)ref, i ) / rhs;
   ref = spu_insert( tmp, (vec_llong2)ref, i );
   return tmp;
}

inline
llong
llong2_idx::operator %= ( llong rhs )
{
   llong tmp = spu_extract( (vec_llong2)ref, i ) % rhs;
   ref = spu_insert( tmp, (vec_llong2)ref, i );
   return tmp;
}

inline
llong
llong2_idx::operator += ( llong rhs )
{
   llong tmp = spu_extract( (vec_llong2)ref, i ) + rhs;
   ref = spu_insert( tmp, (vec_llong2)ref, i );
   return tmp;
}

inline
llong
llong2_idx::operator -= ( llong rhs )
{
   llong tmp = spu_extract( (vec_llong2)ref, i ) - rhs;
   ref = spu_insert( tmp, (vec_llong2)ref, i );
   return tmp;
}

inline
llong
llong2_idx::operator <<= ( ullong rhs )
{
   llong tmp = spu_extract( (vec_llong2)ref, i ) << rhs;
   ref = spu_insert( tmp, (vec_llong2)ref, i );
   return tmp;
}

inline
llong
llong2_idx::operator >>= ( ullong rhs )
{
   llong tmp = spu_extract( (vec_llong2)ref, i ) >> rhs;
   ref = spu_insert( tmp, (vec_llong2)ref, i );
   return tmp;
}

inline
llong
llong2_idx::operator &= ( llong rhs )
{
   llong tmp = spu_extract( (vec_llong2)ref, i ) & rhs;
   ref = spu_insert( tmp, (vec_llong2)ref, i );
   return tmp;
}

inline
llong
llong2_idx::operator ^= ( llong rhs )
{
   llong tmp = spu_extract( (vec_llong2)ref, i ) ^ rhs;
   ref = spu_insert( tmp, (vec_llong2)ref, i );
   return tmp;
}

inline
llong
llong2_idx::operator |= ( llong rhs )
{
   llong tmp = spu_extract( (vec_llong2)ref, i ) | rhs;
   ref = spu_insert( tmp, (vec_llong2)ref, i );
   return tmp;
}

}; // namespace simd

#endif // llong2_h
