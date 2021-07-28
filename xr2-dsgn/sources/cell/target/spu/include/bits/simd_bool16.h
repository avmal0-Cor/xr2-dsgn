/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2008 Sony Computer Entertainment Inc. 
* All Rights Reserved.
*/

#ifndef _SCE_SIMD_BOOL16_H
#define _SCE_SIMD_BOOL16_H

#include <spu_intrinsics.h>
#include "simd_scalars.h"

namespace simd {

class bool16;
class char16;
class uchar16;
class bool16_idx;

// bool16 class
//

class bool16
{
   private:
      vec_uchar16 data;

   public:
      inline bool16() {}
      inline bool16( char16 );
      inline bool16( uchar16 );
      inline bool16( bool r0, bool r1, bool r2, bool r3,
                     bool r4, bool r5, bool r6, bool r7,
                     bool r8, bool r9, bool r10, bool r11,
                     bool r12, bool r13, bool r14, bool r15 );

      // set all slots to same bool

      explicit inline bool16( bool );

      // allows implicit conversion from vector unsigned char
      // NOTE: converts zero slot to false, non-zero slot to true

      inline bool16( vec_uchar16 rhs );

      // allows implicit conversion to vector unsigned char
      // NOTE: converts false slot to 0, true slot to all 1 bits

      inline operator vector unsigned char() const;

      // use array index operator to set and get slots

      inline bool16_idx operator [] ( int i );
      inline bool operator [] ( int i ) const;

      inline const bool16 operator ! () const;

      inline bool16& operator = ( bool16 rhs );
      inline bool16& operator &= ( bool16 rhs );
      inline bool16& operator ^= ( bool16 rhs );
      inline bool16& operator |= ( bool16 rhs );

      friend inline const bool16 operator == ( bool16 lhs, bool16 rhs );
      friend inline const bool16 operator != ( bool16 lhs, bool16 rhs );
      friend inline const bool16 operator < ( char16 lhs, char16 rhs );
      friend inline const bool16 operator <= ( char16 lhs, char16 rhs );
      friend inline const bool16 operator > ( char16 lhs, char16 rhs );
      friend inline const bool16 operator >= ( char16 lhs, char16 rhs );
      friend inline const bool16 operator == ( char16 lhs, char16 rhs );
      friend inline const bool16 operator != ( char16 lhs, char16 rhs );
      friend inline const bool16 operator < ( uchar16 lhs, uchar16 rhs );
      friend inline const bool16 operator <= ( uchar16 lhs, uchar16 rhs );
      friend inline const bool16 operator > ( uchar16 lhs, uchar16 rhs );
      friend inline const bool16 operator >= ( uchar16 lhs, uchar16 rhs );
      friend inline const bool16 operator == ( uchar16 lhs, uchar16 rhs );
      friend inline const bool16 operator != ( uchar16 lhs, uchar16 rhs );
      friend class bool16_idx;
};

//--------------------------------------------------------------------------------------------------
// functions for bool16
//

inline const bool16 operator == ( bool16 lhs, bool16 rhs );
inline const bool16 operator != ( bool16 lhs, bool16 rhs );

// select between lhs and rhs slots using bool16.
// false selects lhs slot, true selects rhs slot

inline const bool16 select( bool16 lhs, bool16 rhs, bool16 rhs_slots );

inline const bool16 operator & ( bool16 lhs, bool16 rhs );
inline const bool16 operator ^ ( bool16 lhs, bool16 rhs );
inline const bool16 operator | ( bool16 lhs, bool16 rhs );

// gather bool slots into a bit vector

inline uint gather( bool16 rhs );

// test whether any slot is true

inline bool any( bool16 rhs );

// test whether all slots are true

inline bool all( bool16 rhs );

//--------------------------------------------------------------------------------------------------
// bool16_idx: for setting bool16 slots with [] operator
//

class bool16_idx
{
   private:
      bool16 &ref __attribute__ ((aligned(16)));
      int i __attribute__ ((aligned(16)));
   public:
      inline bool16_idx( bool16& vec, int idx ): ref(vec) { i = idx; }
      inline operator bool() const;
      inline bool operator = ( bool rhs );
      inline bool operator = ( const bool16_idx& rhs );
      inline bool operator &= ( bool rhs );
      inline bool operator ^= ( bool rhs );
      inline bool operator |= ( bool rhs );
};

}; // namespace simd

#include "simd_char16.h"
#include "simd_uchar16.h"

namespace simd {

inline
bool16::bool16( bool rhs )
{
   data = spu_splats((uchar)-rhs);
}

inline
bool16::bool16( vec_uchar16 rhs )
{
   data = spu_cmpgt( rhs, (uchar)0 );
}

inline
bool16::bool16( char16 rhs )
{
   data = spu_cmpgt( (vec_uchar16)(vec_char16)rhs, (uchar)0 );
}

inline
bool16::bool16( uchar16 rhs )
{
   data = spu_cmpgt( (vec_uchar16)rhs, (uchar)0 );
}

inline
bool16::bool16( bool r0, bool r1, bool r2, bool r3,
                bool r4, bool r5, bool r6, bool r7,
                bool r8, bool r9, bool r10, bool r11,
                bool r12, bool r13, bool r14, bool r15 )
{
   data = __extension__ (vec_uchar16){ (uchar)r0, (uchar)r1, (uchar)r2, (uchar)r3,
                                       (uchar)r4, (uchar)r5, (uchar)r6, (uchar)r7,
                                       (uchar)r8, (uchar)r9, (uchar)r10, (uchar)r11,
                                       (uchar)r12, (uchar)r13, (uchar)r14, (uchar)r15 };
   data = spu_cmpgt( data, (uchar)0 );
}

inline
bool16::operator vector unsigned char() const
{
   return data;
}

inline
bool16_idx
bool16::operator [] ( int i )
{
   return bool16_idx(*this,i);
}

inline
bool
bool16::operator [] ( int i ) const
{
   return (bool)spu_extract((vec_uchar16)data, i);
}

inline
const bool16
bool16::operator ! () const
{
   bool16 result;
   result.data = spu_nor( data, data );
   return result;
}

inline
bool16&
bool16::operator = ( bool16 rhs )
{
   data = rhs.data;
   return *this;
}

inline
bool16&
bool16::operator &= ( bool16 rhs )
{
   *this = *this & rhs;
   return *this;
}

inline
bool16&
bool16::operator ^= ( bool16 rhs )
{
   *this = *this ^ rhs;
   return *this;
}

inline
bool16&
bool16::operator |= ( bool16 rhs )
{
   *this = *this | rhs;
   return *this;
}

inline
const bool16
operator == ( bool16 lhs, bool16 rhs )
{
   bool16 result;
   result.data = spu_cmpeq( (vec_uchar16)lhs, (vec_uchar16)rhs );
   return result;
}

inline
const bool16
operator != ( bool16 lhs, bool16 rhs )
{
   return !( lhs == rhs );
}
   
inline
const bool16
select( bool16 lhs, bool16 rhs, bool16 rhs_slots )
{
   return bool16( spu_sel( (vec_uchar16)lhs, (vec_uchar16)rhs, (vec_uchar16)rhs_slots ) );
}

inline
const bool16
operator & ( bool16 lhs, bool16 rhs )
{
   return bool16( spu_and( (vec_uchar16)lhs, (vec_uchar16)rhs ) );
}

inline
const bool16
operator | ( bool16 lhs, bool16 rhs )
{
   return bool16( spu_or( (vec_uchar16)lhs, (vec_uchar16)rhs ) );
}

inline
const bool16
operator ^ ( bool16 lhs, bool16 rhs )
{
   return bool16( spu_xor( (vec_uchar16)lhs, (vec_uchar16)rhs ) );
}
   
inline
uint
gather( bool16 rhs )
{
   return spu_extract( spu_gather( (vec_uchar16)rhs ), 0 );
}

inline
bool
any( bool16 rhs )
{
   return ( gather(rhs) != 0 );
}

inline
bool
all( bool16 rhs )
{
   return ( gather(rhs) == 0xffff );
}

inline
bool16_idx::operator bool() const
{ 
   return (bool)spu_extract( (vec_uchar16)ref, i );
}

inline
bool
bool16_idx::operator = ( bool rhs )
{
   ref.data = spu_insert( (uchar)-rhs, (vec_uchar16)ref, i );
   return rhs;
}
   
inline
bool
bool16_idx::operator = ( const bool16_idx& rhs )
{
   return ( *this = (bool)rhs );
}

inline
bool
bool16_idx::operator &= ( bool rhs )
{
   uchar tmp = spu_extract( (vec_uchar16)ref, i ) & (uchar)-rhs;
   ref = spu_insert( tmp, (vec_uchar16)ref, i );
   return tmp;
}

inline
bool
bool16_idx::operator ^= ( bool rhs )
{
   uchar tmp = spu_extract( (vec_uchar16)ref, i ) ^ (uchar)-rhs;
   ref = spu_insert( tmp, (vec_uchar16)ref, i );
   return tmp;
}

inline
bool
bool16_idx::operator |= ( bool rhs )
{
   uchar tmp = spu_extract( (vec_uchar16)ref, i ) | (uchar)-rhs;
   ref = spu_insert( tmp, (vec_uchar16)ref, i );
   return tmp;
}

}; // namespace simd

#endif // bool16_h
