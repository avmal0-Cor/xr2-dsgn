/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2008 Sony Computer Entertainment Inc. 
* All Rights Reserved.
*/

#ifndef _SCE_SIMD_DOUBLE2_H
#define _SCE_SIMD_DOUBLE2_H

#include <math.h>
#include <spu_intrinsics.h>
#include "simd_scalars.h"

namespace simd {

class llong2;
class ullong2;
class bool2;
class bool2;
class double2_idx;

//--------------------------------------------------------------------------------------------------
// double2 class
//

class double2
{
   private:
      vec_double2 data;

   public:
      inline double2() {}
      inline double2( llong2 );
      inline double2( ullong2 );
      inline double2( bool2 );
      inline double2( double r0, double r1 );

      // set all slots to same double

      explicit inline double2( double );

      // allows implicit conversion from vector double

      inline double2( vec_double2 rhs );

      // allows implicit conversion to vector double

      inline operator vector double() const;

      // use array index operator to set and get slots

      inline double2_idx operator [] ( int i );
      inline double operator [] ( int i ) const;

      inline const double2 operator ++ ( int );
      inline const double2 operator -- ( int );
      inline double2& operator ++ ();
      inline double2& operator -- ();

      inline const double2 operator - () const;

      inline double2& operator = ( double2 rhs );
      inline double2& operator *= ( double2 rhs );
      inline double2& operator /= ( double2 rhs );
      inline double2& operator += ( double2 rhs );
      inline double2& operator -= ( double2 rhs );
};

//--------------------------------------------------------------------------------------------------
// functions for double2
//

inline const double2 operator * ( double2 lhs, double2 rhs );
inline const double2 operator / ( double2 lhs, double2 rhs );
inline const double2 operator + ( double2 lhs, double2 rhs );
inline const double2 operator - ( double2 lhs, double2 rhs );

inline const bool2 operator < ( double2 lhs, double2 rhs );
inline const bool2 operator <= ( double2 lhs, double2 rhs );
inline const bool2 operator > ( double2 lhs, double2 rhs );
inline const bool2 operator >= ( double2 lhs, double2 rhs );
inline const bool2 operator == ( double2 lhs, double2 rhs );
inline const bool2 operator != ( double2 lhs, double2 rhs );

// select between lhs and rhs slots using bool2.
// false selects lhs slot, true selects rhs slot

inline const double2 select( double2 lhs, double2 rhs, bool2 rhs_slots );

//--------------------------------------------------------------------------------------------------
// double2_idx: for setting double2 slots with [] operator
//

class double2_idx
{
   private:
      double2 &ref __attribute__ ((aligned(16)));
      int i __attribute__ ((aligned(16)));
   public:
      inline double2_idx( double2& vec, int idx ): ref(vec) { i = idx; }
      inline operator double() const;
      inline double operator = ( double rhs );
      inline double operator = ( const double2_idx& rhs );
      inline double operator ++ ( int );
      inline double operator -- ( int );
      inline double operator ++ ();
      inline double operator -- ();
      inline double operator *= ( double rhs );
      inline double operator /= ( double rhs );
      inline double operator += ( double rhs );
      inline double operator -= ( double rhs );
};

}; // namespace simd

#include "simd_llong2.h"
#include "simd_ullong2.h"
#include "simd_bool2.h"
#include "simd_bool2.h"

namespace simd {

inline
double2::double2( double rhs )
{
   data = spu_splats((double)rhs);
}

inline
double2::double2( vec_double2 rhs )
{
   data = rhs;
}

inline
double2::double2( llong2 rhs )
{
   data = __extension__ (vec_double2){ (double)spu_extract( (vec_llong2)rhs, 0 ), (double)spu_extract( (vec_llong2)rhs, 1 ) };
}

inline
double2::double2( ullong2 rhs )
{
   data = __extension__ (vec_double2){ (double)spu_extract( (vec_ullong2)rhs, 0 ), (double)spu_extract( (vec_ullong2)rhs, 1 ) };
}

inline
double2::double2( bool2 rhs )
{
   *this = llong2( rhs );
}

inline
double2::double2( double r0, double r1 )
{
   data = __extension__ (vec_double2){ r0, r1 };
}

inline
double2::operator vector double() const
{
   return data;
}

inline
double2_idx
double2::operator [] ( int i )
{
   return double2_idx(*this,i);
}

inline
double
double2::operator [] ( int i ) const
{
   return (double)spu_extract((vec_double2)data, i);
}

inline
const double2
double2::operator ++ ( int )
{
   vec_double2 olddata = data;
   operator ++();
   return double2(olddata);
}

inline
const double2
double2::operator -- ( int )
{
   vec_double2 olddata = data;
   operator --();
   return double2(olddata);
}

inline
double2&
double2::operator ++ ()
{
   *this += double2(1.0);
   return *this;
}

inline
double2&
double2::operator -- ()
{
   *this -= double2(1.0);
   return *this;
}

inline
const double2
double2::operator - () const
{
   return double2( (vec_double2)spu_xor( (vec_ullong2)data, spu_splats(0x8000000000000000ull) ) );
}

inline
double2&
double2::operator = ( double2 rhs )
{
   data = rhs.data;
   return *this;
}

inline
double2&
double2::operator *= ( double2 rhs )
{
   *this = *this * rhs;
   return *this;
}

inline
double2&
double2::operator /= ( double2 rhs )
{
   *this = *this / rhs;
   return *this;
}

inline
double2&
double2::operator += ( double2 rhs )
{
   *this = *this + rhs;
   return *this;
}

inline
double2&
double2::operator -= ( double2 rhs )
{
   *this = *this - rhs;
   return *this;
}

inline
const double2
operator * ( double2 lhs, double2 rhs )
{
   return double2( spu_mul( (vec_double2)lhs, (vec_double2)rhs ) );
}

inline
const double2
operator / ( double2 num, double2 den )
{
   return double2( divd2( (vec_double2)num, (vec_double2)den ) );
}

inline
const double2
operator + ( double2 lhs, double2 rhs )
{
   return double2( spu_add( (vec_double2)lhs, (vec_double2)rhs ) );
}

inline
const double2
operator - ( double2 lhs, double2 rhs )
{
   return double2( spu_sub( (vec_double2)lhs, (vec_double2)rhs ) );
}

inline
const bool2
operator < ( double2 lhs, double2 rhs )
{
   bool2 result;
   result.data = cmpgtd2( (vec_double2)rhs, (vec_double2)lhs );
   return result;
}

inline
const bool2
operator <= ( double2 lhs, double2 rhs )
{
   bool2 result;
   result.data = cmpged2( (vec_double2)rhs, (vec_double2)lhs );
   return result;
}

inline
const bool2
operator > ( double2 lhs, double2 rhs )
{
   bool2 result;
   result.data = cmpgtd2( (vec_double2)lhs, (vec_double2)rhs );
   return result;
}

inline
const bool2
operator >= ( double2 lhs, double2 rhs )
{
   bool2 result;
   result.data = cmpged2( (vec_double2)lhs, (vec_double2)rhs );
   return result;
}

inline
const bool2
operator == ( double2 lhs, double2 rhs )
{
   bool2 result;
   result.data = cmpeqd2( (vec_double2)lhs, (vec_double2)rhs );
   return result;
}

inline
const bool2
operator != ( double2 lhs, double2 rhs )
{
   return !( lhs == rhs );
}
   
inline
const double2
select( double2 lhs, double2 rhs, bool2 rhs_slots )
{
   return double2( spu_sel( (vec_double2)lhs, (vec_double2)rhs, (vec_ullong2)rhs_slots ) );
}

inline
double2_idx::operator double() const
{ 
   return (double)spu_extract( (vec_double2)ref, i );
}

inline 
double
double2_idx::operator = ( double rhs )
{
   ref = spu_insert( (double)rhs, (vec_double2)ref, i );
   return rhs;
}

inline
double
double2_idx::operator = ( const double2_idx& rhs )
{
   return ( *this = (double)rhs );
}

inline
double
double2_idx::operator ++( int )
{
   double tmp = spu_extract( (vec_double2)ref, i );
   ref = spu_insert( tmp+1, (vec_double2)ref, i );
   return tmp;
}

inline
double
double2_idx::operator --( int )
{
   double tmp = spu_extract( (vec_double2)ref, i );
   ref = spu_insert( tmp-1, (vec_double2)ref, i );
   return tmp;
}

inline
double
double2_idx::operator ++()
{
   double tmp = spu_extract( (vec_double2)ref, i );
   ++tmp;
   ref = spu_insert( tmp, (vec_double2)ref, i );
   return tmp;
}

inline
double
double2_idx::operator --()
{
   double tmp = spu_extract( (vec_double2)ref, i );
   --tmp;
   ref = spu_insert( tmp, (vec_double2)ref, i );
   return tmp;
}

inline
double
double2_idx::operator *= ( double rhs )
{
   double tmp = spu_extract( (vec_double2)ref, i ) * rhs;
   ref = spu_insert( tmp, (vec_double2)ref, i );
   return tmp;
}

inline
double
double2_idx::operator /= ( double rhs )
{
   double tmp = spu_extract( (vec_double2)ref, i ) / rhs;
   ref = spu_insert( tmp, (vec_double2)ref, i );
   return tmp;
}

inline
double
double2_idx::operator += ( double rhs )
{
   double tmp = spu_extract( (vec_double2)ref, i ) + rhs;
   ref = spu_insert( tmp, (vec_double2)ref, i );
   return tmp;
}

inline
double
double2_idx::operator -= ( double rhs )
{
   double tmp = spu_extract( (vec_double2)ref, i ) - rhs;
   ref = spu_insert( tmp, (vec_double2)ref, i );
   return tmp;
}

}; // namespace simd

#endif // double2_h
