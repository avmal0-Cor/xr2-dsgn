/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2008 Sony Computer Entertainment Inc. 
* All Rights Reserved.
*/

#ifndef _SCE_SIMD_BOOL4_H
#define _SCE_SIMD_BOOL4_H

#include <altivec.h>
#include "simd_scalars.h"

namespace simd {

class bool4;
class int4;
class uint4;
class float4;
class bool4_idx;

//--------------------------------------------------------------------------------------------------
// bool4 class
//

class bool4
{
   private:
      vec_uint4 data;

   public:
      inline bool4() {}
      inline bool4( int4 );
      inline bool4( uint4 );
      inline bool4( float4 );
      inline bool4( bool r0, bool r1, bool r2, bool r3 );

      // set all slots to same bool

      explicit inline bool4( bool );

      // allows implicit conversion from vector unsigned int
      // NOTE: converts zero slot to false, non-zero slot to true

      inline bool4( vec_uint4 rhs );

      // allows implicit conversion to vector unsigned int
      // NOTE: converts false slot to 0, true slot to all 1 bits

      inline operator vector unsigned int() const;

      // use array index operator to set and get slots

      inline bool4_idx operator [] ( int i );
      inline bool operator [] ( int i ) const;

      inline const bool4 operator ! () const;

      inline bool4& operator = ( bool4 rhs );
      inline bool4& operator &= ( bool4 rhs );
      inline bool4& operator ^= ( bool4 rhs );
      inline bool4& operator |= ( bool4 rhs );

      friend inline const bool4 operator == ( bool4 lhs, bool4 rhs );
      friend inline const bool4 operator != ( bool4 lhs, bool4 rhs );
      friend inline const bool4 operator < ( int4 lhs, int4 rhs );
      friend inline const bool4 operator <= ( int4 lhs, int4 rhs );
      friend inline const bool4 operator > ( int4 lhs, int4 rhs );
      friend inline const bool4 operator >= ( int4 lhs, int4 rhs );
      friend inline const bool4 operator == ( int4 lhs, int4 rhs );
      friend inline const bool4 operator != ( int4 lhs, int4 rhs );
      friend inline const bool4 operator < ( uint4 lhs, uint4 rhs );
      friend inline const bool4 operator <= ( uint4 lhs, uint4 rhs );
      friend inline const bool4 operator > ( uint4 lhs, uint4 rhs );
      friend inline const bool4 operator >= ( uint4 lhs, uint4 rhs );
      friend inline const bool4 operator == ( uint4 lhs, uint4 rhs );
      friend inline const bool4 operator != ( uint4 lhs, uint4 rhs );
      friend inline const bool4 operator < ( float4 lhs, float4 rhs );
      friend inline const bool4 operator <= ( float4 lhs, float4 rhs );
      friend inline const bool4 operator > ( float4 lhs, float4 rhs );
      friend inline const bool4 operator >= ( float4 lhs, float4 rhs );
      friend inline const bool4 operator == ( float4 lhs, float4 rhs );
      friend inline const bool4 operator != ( float4 lhs, float4 rhs );
      friend class bool4_idx;
};

//--------------------------------------------------------------------------------------------------
// functions for bool4
//

inline const bool4 operator == ( bool4 lhs, bool4 rhs );
inline const bool4 operator != ( bool4 lhs, bool4 rhs );

// select between lhs and rhs slots using bool4.
// false selects lhs slot, true selects rhs slot

inline const bool4 select( bool4 lhs, bool4 rhs, bool4 rhs_slots );

inline const bool4 operator & ( bool4 lhs, bool4 rhs );
inline const bool4 operator ^ ( bool4 lhs, bool4 rhs );
inline const bool4 operator | ( bool4 lhs, bool4 rhs );

// gather bool slots into a bit vector

inline uint gather( bool4 rhs );

// test whether any slot is true

inline bool any( bool4 rhs );

// test whether all slots are true

inline bool all( bool4 rhs );

//--------------------------------------------------------------------------------------------------
// bool4_idx: for setting bool4 slots with [] operator
//

class bool4_idx
{
   private:
      bool4 &ref __attribute__ ((aligned(16)));
      int i __attribute__ ((aligned(16)));
   public:
      inline bool4_idx( bool4& vec, int idx ): ref(vec) { i = idx; }
      inline operator bool() const;
      inline bool operator = ( bool rhs );
      inline bool operator = ( const bool4_idx& rhs );
      inline bool operator &= ( bool rhs );
      inline bool operator ^= ( bool rhs );
      inline bool operator |= ( bool rhs );
};

}; // namespace simd

#include "simd_int4.h"
#include "simd_uint4.h"
#include "simd_float4.h"

namespace simd {

inline
bool4::bool4( bool rhs )
{
   uint tmp = (uint)-rhs;
   data = __extension__ (vec_uint4){ tmp, tmp, tmp, tmp };
}

inline
bool4::bool4( vec_uint4 rhs )
{
   data = (vec_uint4)vec_cmpgt( rhs, (vec_uint4)(0) );
}

inline
bool4::bool4( int4 rhs )
{
   data = (vec_uint4)vec_cmpgt( (vec_uint4)(vec_int4)rhs, (vec_uint4)(0) );
}

inline
bool4::bool4( uint4 rhs )
{
   data = (vec_uint4)vec_cmpgt( (vec_uint4)rhs, (vec_uint4)(0) );
}

inline
bool4::bool4( float4 rhs )
{
   *this = ( rhs != float4(0.0f) );
}

inline
bool4::bool4( bool r0, bool r1, bool r2, bool r3 )
{
   data = __extension__ (vec_uint4){ (uint)r0, (uint)r1, (uint)r2, (uint)r3 };
   data = (vec_uint4)vec_cmpgt( data, (vec_uint4)(0) );
}

inline
bool4::operator vector unsigned int() const
{
   return data;
}

inline
bool4_idx
bool4::operator [] ( int i )
{
   return bool4_idx(*this,i);
}

inline
bool
bool4::operator [] ( int i ) const
{
   return (bool)_vec_extract((vec_uint4)data, i);
}

inline
const bool4
bool4::operator ! () const
{
   bool4 result;
   result.data = vec_nor( data, data );
   return result;
}

inline
bool4&
bool4::operator = ( bool4 rhs )
{
   data = rhs.data;
   return *this;
}

inline
bool4&
bool4::operator &= ( bool4 rhs )
{
   *this = *this & rhs;
   return *this;
}

inline
bool4&
bool4::operator ^= ( bool4 rhs )
{
   *this = *this ^ rhs;
   return *this;
}

inline
bool4&
bool4::operator |= ( bool4 rhs )
{
   *this = *this | rhs;
   return *this;
}

inline
const bool4
operator == ( bool4 lhs, bool4 rhs )
{
   bool4 result;
   result.data = (vec_uint4)vec_cmpeq( (vec_uint4)lhs, (vec_uint4)rhs );
   return result;
}

inline
const bool4
operator != ( bool4 lhs, bool4 rhs )
{
   return !( lhs == rhs );
}
   
inline
const bool4
select( bool4 lhs, bool4 rhs, bool4 rhs_slots )
{
   return bool4( vec_sel( (vec_uint4)lhs, (vec_uint4)rhs, (vec_uint4)rhs_slots ) );
}

inline
const bool4
operator & ( bool4 lhs, bool4 rhs )
{
   return bool4( vec_and( (vec_uint4)lhs, (vec_uint4)rhs ) );
}

inline
const bool4
operator | ( bool4 lhs, bool4 rhs )
{
   return bool4( vec_or( (vec_uint4)lhs, (vec_uint4)rhs ) );
}

inline
const bool4
operator ^ ( bool4 lhs, bool4 rhs )
{
   return bool4( vec_xor( (vec_uint4)lhs, (vec_uint4)rhs ) );
}
   
inline
uint
gather( bool4 rhs )
{
   union {
      vec_int4 v;
      int s[4];
   } tmp;
   
   tmp.v = vec_sums( (vec_int4)vec_and( (vec_uint4)rhs, __extension__ (vec_uint4){ 8, 4, 2, 1 } ), 
                     (vec_int4)(0) );
   
   return (uint)tmp.s[3];
}

inline
bool
any( bool4 rhs )
{
   return ( vec_any_gt( (vec_uint4)rhs, (vec_uint4)(0) ) );
}

inline
bool
all( bool4 rhs )
{
   return ( vec_all_gt( (vec_uint4)rhs, (vec_uint4)(0) ) );
}

inline
bool4_idx::operator bool() const
{ 
   return (bool)_vec_extract( (vec_uint4)ref, i );
}

inline
bool
bool4_idx::operator = ( bool rhs )
{
   ref.data = _vec_insert( (uint)-rhs, (vec_uint4)ref, i );
   return rhs;
}
   
inline
bool
bool4_idx::operator = ( const bool4_idx& rhs )
{
   return ( *this = (bool)rhs );
}

inline
bool
bool4_idx::operator &= ( bool rhs )
{
   uint tmp = _vec_extract( (vec_uint4)ref, i ) & (uint)-rhs;
   ref = _vec_insert( tmp, (vec_uint4)ref, i );
   return tmp;
}

inline
bool
bool4_idx::operator ^= ( bool rhs )
{
   uint tmp = _vec_extract( (vec_uint4)ref, i ) ^ (uint)-rhs;
   ref = _vec_insert( tmp, (vec_uint4)ref, i );
   return tmp;
}

inline
bool
bool4_idx::operator |= ( bool rhs )
{
   uint tmp = _vec_extract( (vec_uint4)ref, i ) | (uint)-rhs;
   ref = _vec_insert( tmp, (vec_uint4)ref, i );
   return tmp;
}

}; // namespace simd

#endif // bool4_h
