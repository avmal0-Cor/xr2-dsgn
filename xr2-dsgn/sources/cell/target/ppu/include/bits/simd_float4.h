/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2008 Sony Computer Entertainment Inc. 
* All Rights Reserved.
*/

#ifndef _SCE_SIMD_FLOAT4_H
#define _SCE_SIMD_FLOAT4_H

#include <math.h>
#include <altivec.h>
#include "simd_scalars.h"

namespace simd {

class int4;
class uint4;
class bool4;
class bool4;
class float4_idx;

//--------------------------------------------------------------------------------------------------
// float4 class
//

class float4
{
   private:
      vec_float4 data;

   public:
      inline float4() {}
      inline float4( int4 );
      inline float4( uint4 );
      inline float4( bool4 );
      inline float4( float r0, float r1, float r2, float r3 );

      // set all slots to same float

      explicit inline float4( float );

      // allows implicit conversion from vector float

      inline float4( vec_float4 rhs );

      // allows implicit conversion to vector float

      inline operator vector float() const;

      // use array index operator to set and get slots

      inline float4_idx operator [] ( int i );
      inline float operator [] ( int i ) const;

      inline const float4 operator ++ ( int );
      inline const float4 operator -- ( int );
      inline float4& operator ++ ();
      inline float4& operator -- ();

      inline const float4 operator - () const;

      inline float4& operator = ( float4 rhs );
      inline float4& operator *= ( float4 rhs );
      inline float4& operator /= ( float4 rhs );
      inline float4& operator += ( float4 rhs );
      inline float4& operator -= ( float4 rhs );
};

//--------------------------------------------------------------------------------------------------
// functions for float4
//

inline const float4 operator * ( float4 lhs, float4 rhs );
inline const float4 operator / ( float4 lhs, float4 rhs );
inline const float4 operator + ( float4 lhs, float4 rhs );
inline const float4 operator - ( float4 lhs, float4 rhs );

inline const bool4 operator < ( float4 lhs, float4 rhs );
inline const bool4 operator <= ( float4 lhs, float4 rhs );
inline const bool4 operator > ( float4 lhs, float4 rhs );
inline const bool4 operator >= ( float4 lhs, float4 rhs );
inline const bool4 operator == ( float4 lhs, float4 rhs );
inline const bool4 operator != ( float4 lhs, float4 rhs );

// select between lhs and rhs slots using bool4.
// false selects lhs slot, true selects rhs slot

inline const float4 select( float4 lhs, float4 rhs, bool4 rhs_slots );

//--------------------------------------------------------------------------------------------------
// float4_idx: for setting float4 slots with [] operator
//

class float4_idx
{
   private:
      float4 &ref __attribute__ ((aligned(16)));
      int i __attribute__ ((aligned(16)));
   public:
      inline float4_idx( float4& vec, int idx ): ref(vec) { i = idx; }
      inline operator float() const;
      inline float operator = ( float rhs );
      inline float operator = ( const float4_idx& rhs );
      inline float operator ++ ( int );
      inline float operator -- ( int );
      inline float operator ++ ();
      inline float operator -- ();
      inline float operator *= ( float rhs );
      inline float operator /= ( float rhs );
      inline float operator += ( float rhs );
      inline float operator -= ( float rhs );
};

}; // namespace simd

#include "simd_int4.h"
#include "simd_uint4.h"
#include "simd_bool4.h"
#include "simd_bool4.h"

namespace simd {

inline
float4::float4( float rhs )
{
   float tmp = (float)rhs;
   data = __extension__ (vec_float4){ tmp, tmp, tmp, tmp };
}

inline
float4::float4( vec_float4 rhs )
{
   data = rhs;
}

inline
float4::float4( int4 rhs )
{
   data = vec_ctf( (vec_int4)rhs, 0 );
}

inline
float4::float4( uint4 rhs )
{
   data = vec_ctf( (vec_uint4)rhs, 0 );
}

inline
float4::float4( bool4 rhs )
{
   *this = int4( rhs );
}

inline
float4::float4( float r0, float r1, float r2, float r3 )
{
   data = __extension__ (vec_float4){ r0, r1, r2, r3 };
}

inline
float4::operator vector float() const
{
   return data;
}

inline
float4_idx
float4::operator [] ( int i )
{
   return float4_idx(*this,i);
}

inline
float
float4::operator [] ( int i ) const
{
   return (float)_vec_extract((vec_float4)data, i);
}

inline
const float4
float4::operator ++ ( int )
{
   vec_float4 olddata = data;
   operator ++();
   return float4(olddata);
}

inline
const float4
float4::operator -- ( int )
{
   vec_float4 olddata = data;
   operator --();
   return float4(olddata);
}

inline
float4&
float4::operator ++ ()
{
   *this += float4(1.0f);
   return *this;
}

inline
float4&
float4::operator -- ()
{
   *this -= float4(1.0f);
   return *this;
}

inline
const float4
float4::operator - () const
{
   return float4( (vec_float4)vec_xor( (vec_uint4)data, (vec_uint4)(0x80000000) ) );
}

inline
float4&
float4::operator = ( float4 rhs )
{
   data = rhs.data;
   return *this;
}

inline
float4&
float4::operator *= ( float4 rhs )
{
   *this = *this * rhs;
   return *this;
}

inline
float4&
float4::operator /= ( float4 rhs )
{
   *this = *this / rhs;
   return *this;
}

inline
float4&
float4::operator += ( float4 rhs )
{
   *this = *this + rhs;
   return *this;
}

inline
float4&
float4::operator -= ( float4 rhs )
{
   *this = *this - rhs;
   return *this;
}

inline
const float4
operator * ( float4 lhs, float4 rhs )
{
   return float4( vec_madd( (vec_float4)lhs, (vec_float4)rhs, (vec_float4)(0) ) );
}

inline
const float4
operator / ( float4 num, float4 den )
{
   return float4( divf4( (vec_float4)num, (vec_float4)den ) );
}

inline
const float4
operator + ( float4 lhs, float4 rhs )
{
   return float4( vec_add( (vec_float4)lhs, (vec_float4)rhs ) );
}

inline
const float4
operator - ( float4 lhs, float4 rhs )
{
   return float4( vec_sub( (vec_float4)lhs, (vec_float4)rhs ) );
}

inline
const bool4
operator < ( float4 lhs, float4 rhs )
{
   bool4 result;
   result.data = (vec_uint4)vec_cmpgt( (vec_float4)rhs, (vec_float4)lhs );
   return result;
}

inline
const bool4
operator <= ( float4 lhs, float4 rhs )
{
   return !( lhs > rhs );
}

inline
const bool4
operator > ( float4 lhs, float4 rhs )
{
   bool4 result;
   result.data = (vec_uint4)vec_cmpgt( (vec_float4)lhs, (vec_float4)rhs );
   return result;
}

inline
const bool4
operator >= ( float4 lhs, float4 rhs )
{
   return !( lhs < rhs );
}

inline
const bool4
operator == ( float4 lhs, float4 rhs )
{
   bool4 result;
   result.data = (vec_uint4)vec_cmpeq( (vec_float4)lhs, (vec_float4)rhs );
   return result;
}

inline
const bool4
operator != ( float4 lhs, float4 rhs )
{
   return !( lhs == rhs );
}
   
inline
const float4
select( float4 lhs, float4 rhs, bool4 rhs_slots )
{
   return float4( vec_sel( (vec_float4)lhs, (vec_float4)rhs, (vec_uint4)rhs_slots ) );
}

inline
float4_idx::operator float() const
{ 
   return (float)_vec_extract( (vec_float4)ref, i );
}

inline 
float
float4_idx::operator = ( float rhs )
{
   ref = _vec_insert( (float)rhs, (vec_float4)ref, i );
   return rhs;
}

inline
float
float4_idx::operator = ( const float4_idx& rhs )
{
   return ( *this = (float)rhs );
}

inline
float
float4_idx::operator ++( int )
{
   float tmp = _vec_extract( (vec_float4)ref, i );
   ref = _vec_insert( tmp+1, (vec_float4)ref, i );
   return tmp;
}

inline
float
float4_idx::operator --( int )
{
   float tmp = _vec_extract( (vec_float4)ref, i );
   ref = _vec_insert( tmp-1, (vec_float4)ref, i );
   return tmp;
}

inline
float
float4_idx::operator ++()
{
   float tmp = _vec_extract( (vec_float4)ref, i );
   ++tmp;
   ref = _vec_insert( tmp, (vec_float4)ref, i );
   return tmp;
}

inline
float
float4_idx::operator --()
{
   float tmp = _vec_extract( (vec_float4)ref, i );
   --tmp;
   ref = _vec_insert( tmp, (vec_float4)ref, i );
   return tmp;
}

inline
float
float4_idx::operator *= ( float rhs )
{
   float tmp = _vec_extract( (vec_float4)ref, i ) * rhs;
   ref = _vec_insert( tmp, (vec_float4)ref, i );
   return tmp;
}

inline
float
float4_idx::operator /= ( float rhs )
{
   float tmp = _vec_extract( (vec_float4)ref, i ) / rhs;
   ref = _vec_insert( tmp, (vec_float4)ref, i );
   return tmp;
}

inline
float
float4_idx::operator += ( float rhs )
{
   float tmp = _vec_extract( (vec_float4)ref, i ) + rhs;
   ref = _vec_insert( tmp, (vec_float4)ref, i );
   return tmp;
}

inline
float
float4_idx::operator -= ( float rhs )
{
   float tmp = _vec_extract( (vec_float4)ref, i ) - rhs;
   ref = _vec_insert( tmp, (vec_float4)ref, i );
   return tmp;
}

}; // namespace simd

#endif // float4_h
