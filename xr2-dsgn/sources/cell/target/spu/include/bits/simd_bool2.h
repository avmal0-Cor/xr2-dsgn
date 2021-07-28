/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2008 Sony Computer Entertainment Inc. 
* All Rights Reserved.
*/

#ifndef _SCE_SIMD_BOOL2_H
#define _SCE_SIMD_BOOL2_H

#include <spu_intrinsics.h>
#include "simd_scalars.h"

namespace simd {

class bool2;
class llong2;
class ullong2;
class double2;
class bool2_idx;

//--------------------------------------------------------------------------------------------------
// bool2 class
//

class bool2
{
   private:
      vec_ullong2 data;

   public:
      inline bool2() {}
      inline bool2( llong2 );
      inline bool2( ullong2 );
      inline bool2( double2 );
      inline bool2( bool r0, bool r1 );

      // set all slots to same bool

      explicit inline bool2( bool );

      // allows implicit conversion from vector unsigned long long
      // NOTE: converts zero slot to false, non-zero slot to true

      inline bool2( vec_ullong2 rhs );

      // allows implicit conversion to vector unsigned long long
      // NOTE: converts false slot to 0, true slot to all 1 bits

      inline operator vector unsigned long long() const;

      // use array index operator to set and get slots

      inline bool2_idx operator [] ( int i );
      inline bool operator [] ( int i ) const;

      inline const bool2 operator ! () const;

      inline bool2& operator = ( bool2 rhs );
      inline bool2& operator &= ( bool2 rhs );
      inline bool2& operator ^= ( bool2 rhs );
      inline bool2& operator |= ( bool2 rhs );

      friend inline const bool2 operator == ( bool2 lhs, bool2 rhs );
      friend inline const bool2 operator != ( bool2 lhs, bool2 rhs );
      friend inline const bool2 operator < ( llong2 lhs, llong2 rhs );
      friend inline const bool2 operator <= ( llong2 lhs, llong2 rhs );
      friend inline const bool2 operator > ( llong2 lhs, llong2 rhs );
      friend inline const bool2 operator >= ( llong2 lhs, llong2 rhs );
      friend inline const bool2 operator == ( llong2 lhs, llong2 rhs );
      friend inline const bool2 operator != ( llong2 lhs, llong2 rhs );
      friend inline const bool2 operator < ( ullong2 lhs, ullong2 rhs );
      friend inline const bool2 operator <= ( ullong2 lhs, ullong2 rhs );
      friend inline const bool2 operator > ( ullong2 lhs, ullong2 rhs );
      friend inline const bool2 operator >= ( ullong2 lhs, ullong2 rhs );
      friend inline const bool2 operator == ( ullong2 lhs, ullong2 rhs );
      friend inline const bool2 operator != ( ullong2 lhs, ullong2 rhs );
      friend inline const bool2 operator < ( double2 lhs, double2 rhs );
      friend inline const bool2 operator <= ( double2 lhs, double2 rhs );
      friend inline const bool2 operator > ( double2 lhs, double2 rhs );
      friend inline const bool2 operator >= ( double2 lhs, double2 rhs );
      friend inline const bool2 operator == ( double2 lhs, double2 rhs );
      friend inline const bool2 operator != ( double2 lhs, double2 rhs );
      friend class bool2_idx;
};

//--------------------------------------------------------------------------------------------------
// functions for bool2
//

inline const bool2 operator == ( bool2 lhs, bool2 rhs );
inline const bool2 operator != ( bool2 lhs, bool2 rhs );

// select between lhs and rhs slots using bool2.
// false selects lhs slot, true selects rhs slot

inline const bool2 select( bool2 lhs, bool2 rhs, bool2 rhs_slots );

inline const bool2 operator & ( bool2 lhs, bool2 rhs );
inline const bool2 operator ^ ( bool2 lhs, bool2 rhs );
inline const bool2 operator | ( bool2 lhs, bool2 rhs );

// gather bool slots into a bit vector

inline uint gather( bool2 rhs );

// test whether any slot is true

inline bool any( bool2 rhs );

// test whether all slots are true

inline bool all( bool2 rhs );

//--------------------------------------------------------------------------------------------------
// bool2_idx: for setting bool2 slots with [] operator
//

class bool2_idx
{
   private:
      bool2 &ref __attribute__ ((aligned(16)));
      int i __attribute__ ((aligned(16)));
   public:
      inline bool2_idx( bool2& vec, int idx ): ref(vec) { i = idx; }
      inline operator bool() const;
      inline bool operator = ( bool rhs );
      inline bool operator = ( const bool2_idx& rhs );
      inline bool operator &= ( bool rhs );
      inline bool operator ^= ( bool rhs );
      inline bool operator |= ( bool rhs );
};

}; // namespace simd

#include "simd_llong2.h"
#include "simd_ullong2.h"
#include "simd_double2.h"

namespace simd {

inline
bool2::bool2( bool rhs )
{
   data = spu_splats((ullong)-rhs);
}

inline
bool2::bool2( vec_ullong2 rhs )
{
   const vec_uchar16 even = __extension__ (vec_uchar16)(vec_uint4){ 0x00010203, 0x00010203, 0x08090a0b, 0x08090a0b };
   const vec_uchar16 odd = __extension__ (vec_uchar16)(vec_uint4){ 0x04050607, 0x04050607, 0x0c0d0e0f, 0x0c0d0e0f };
   data = (vec_ullong2)spu_cmpeq( (vec_uint4)rhs, 0 );
   data = spu_nand( spu_shuffle( data, data, even ), spu_shuffle( data, data, odd ) );
}

inline
bool2::bool2( llong2 rhs )
{
   const vec_uchar16 even = __extension__ (vec_uchar16)(vec_uint4){ 0x00010203, 0x00010203, 0x08090a0b, 0x08090a0b };
   const vec_uchar16 odd = __extension__ (vec_uchar16)(vec_uint4){ 0x04050607, 0x04050607, 0x0c0d0e0f, 0x0c0d0e0f };
   data = (vec_ullong2)spu_cmpeq( (vec_uint4)(vec_llong2)rhs, 0 );
   data = spu_nand( spu_shuffle( data, data, even ), spu_shuffle( data, data, odd ) );
}

inline
bool2::bool2( ullong2 rhs )
{
   const vec_uchar16 even = __extension__ (vec_uchar16)(vec_uint4){ 0x00010203, 0x00010203, 0x08090a0b, 0x08090a0b };
   const vec_uchar16 odd = __extension__ (vec_uchar16)(vec_uint4){ 0x04050607, 0x04050607, 0x0c0d0e0f, 0x0c0d0e0f };
   data = (vec_ullong2)spu_cmpeq( (vec_uint4)(vec_ullong2)rhs, 0 );
   data = spu_nand( spu_shuffle( data, data, even ), spu_shuffle( data, data, odd ) );
}

inline
bool2::bool2( double2 rhs )
{
   *this = ( rhs != double2(0.0) );
}

inline
bool2::bool2( bool r0, bool r1 )
{
   data = __extension__ (vec_ullong2){ (ullong)-r0, (ullong)-r1 };
}

inline
bool2::operator vector unsigned long long() const
{
   return data;
}

inline
bool2_idx
bool2::operator [] ( int i )
{
   return bool2_idx(*this,i);
}

inline
bool
bool2::operator [] ( int i ) const
{
   return (bool)spu_extract((vec_ullong2)data, i);
}

inline
const bool2
bool2::operator ! () const
{
   bool2 result;
   result.data = spu_nor( data, data );
   return result;
}

inline
bool2&
bool2::operator = ( bool2 rhs )
{
   data = rhs.data;
   return *this;
}

inline
bool2&
bool2::operator &= ( bool2 rhs )
{
   *this = *this & rhs;
   return *this;
}

inline
bool2&
bool2::operator ^= ( bool2 rhs )
{
   *this = *this ^ rhs;
   return *this;
}

inline
bool2&
bool2::operator |= ( bool2 rhs )
{
   *this = *this | rhs;
   return *this;
}

inline
const bool2
operator == ( bool2 lhs, bool2 rhs )
{
   return ( ullong2(lhs) == ullong2(rhs) );
}

inline
const bool2
operator != ( bool2 lhs, bool2 rhs )
{
   return !( lhs == rhs );
}
   
inline
const bool2
select( bool2 lhs, bool2 rhs, bool2 rhs_slots )
{
   return bool2( spu_sel( (vec_ullong2)lhs, (vec_ullong2)rhs, (vec_ullong2)rhs_slots ) );
}

inline
const bool2
operator & ( bool2 lhs, bool2 rhs )
{
   return bool2( spu_and( (vec_ullong2)lhs, (vec_ullong2)rhs ) );
}

inline
const bool2
operator | ( bool2 lhs, bool2 rhs )
{
   return bool2( spu_or( (vec_ullong2)lhs, (vec_ullong2)rhs ) );
}

inline
const bool2
operator ^ ( bool2 lhs, bool2 rhs )
{
   return bool2( spu_xor( (vec_ullong2)lhs, (vec_ullong2)rhs ) );
}
   
inline
uint
gather( bool2 rhs )
{
   return (uint)( spu_extract( (vec_ullong2)rhs, 0 ) & 2 ) | (uint)( spu_extract( (vec_ullong2)rhs, 1 ) & 1 );
}
   
inline
bool
any( bool2 rhs )
{
   return ( gather(rhs) != 0 );
}

inline
bool
all( bool2 rhs )
{
   return ( gather(rhs) == 0x3 );
}

inline
bool2_idx::operator bool() const
{ 
   return (bool)spu_extract( (vec_ullong2)ref, i );
}

inline
bool
bool2_idx::operator = ( bool rhs )
{
   ref.data = spu_insert( (ullong)-rhs, (vec_ullong2)ref, i );
   return rhs;
}
   
inline
bool
bool2_idx::operator = ( const bool2_idx& rhs )
{
   return ( *this = (bool)rhs );
}

inline
bool
bool2_idx::operator &= ( bool rhs )
{
   ullong tmp = spu_extract( (vec_ullong2)ref, i ) & (ullong)-rhs;
   ref = spu_insert( tmp, (vec_ullong2)ref, i );
   return tmp;
}

inline
bool
bool2_idx::operator ^= ( bool rhs )
{
   ullong tmp = spu_extract( (vec_ullong2)ref, i ) ^ (ullong)-rhs;
   ref = spu_insert( tmp, (vec_ullong2)ref, i );
   return tmp;
}

inline
bool
bool2_idx::operator |= ( bool rhs )
{
   ullong tmp = spu_extract( (vec_ullong2)ref, i ) | (ullong)-rhs;
   ref = spu_insert( tmp, (vec_ullong2)ref, i );
   return tmp;
}

}; // namespace simd

#endif // bool2_h
