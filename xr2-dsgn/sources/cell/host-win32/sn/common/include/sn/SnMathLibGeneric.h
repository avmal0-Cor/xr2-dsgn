////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) SN Systems Ltd 2005-2007
//
//  This source code file is provided as is with no warranty and SN Systems
//  will not be liable for any lost revenue or other damages resulting from
//  the use of this source code.
//
//  This is sample code, feel free to cut and paste.
//
//  Description:
//     VFPU vector math class sample class definition, generic implementation
//
//     Intended as a reference and for use in tools.
//
//  See:
//     SnMathLibDef for class description
//
//
//  Change History:
//
//  Vers    Date            Author        Changes
//  1.00    09-08-2005      AndyT         Created
//
////////////////////////////////////////////////////////////////////////////////

#define SN_SCALAR_IS_FLOAT
typedef float NativeScalar;
typedef float NativeQuad[ 4 ];

class scalar;
class vquad;
class quadquad;

typedef const scalar &scalar_arg;
typedef const vquad &vquad_arg;
typedef const quadquad &quadquad_arg;

#include "SnMathLibDef.h"

#ifndef SN_MATHLIB_DEFINE_ONLY

// helper function
SNML_INLINE void QMul( float r[], const float a[], const float b[] )
{
	r[0] = a[3] * b[0] + a[0] * b[3] + a[1] * b[2] - a[2] * b[1];
	r[1] = a[3] * b[1] - a[0] * b[2] + a[1] * b[3] + a[2] * b[0];
	r[2] = a[3] * b[2] + a[0] * b[1] - a[1] * b[0] + a[2] * b[3];
	r[3] = a[3] * b[3] - a[0] * b[0] - a[1] * b[1] - a[2] * b[2];
}

SNML_INLINE vquad Jacobi( quadquad &mat, int p, int q )
{
	typedef float f4[4];
	f4 *mx = (f4*)&mat;
	float angle = 0.5f * atan2f( 2*mx[q][p], (mx[p][p]-mx[q][q]) );
	//printf( "a=%f %f %f %f\n", angle, mx[q][p], mx[p][p],mx[q][q] );
	float c = cosf( angle ), s = sinf( angle );
	for( int i = 0; i < 4; ++i )
	{
		float ta = c * mx[i][p], tb = s * mx[i][q], tc = c * mx[i][q], td = s * mx[i][p];
		mx[i][p] = ta + tb;
		mx[i][q] = tc - td;
	}
	for( int i = 0; i < 4; ++i )
	{
		float ta = c * mx[p][i], tb = s * mx[q][i], tc = c * mx[q][i], td = s * mx[p][i];
		mx[p][i] = ta + tb;
		mx[q][i] = tc - td;
	}
	vquad result( c, s );
	return result;
}

SNML_INLINE void MakeEigenvectors( quadquad &mat, int p, int q, vquad &sincos )
{
	typedef float f4[4];
	f4 *mx = (f4*)&mat;
	float c = sincos.x(), s = sincos.y();
	for( int i = 0; i < 4; ++i )
	{
		float ta = c * mx[i][p], tb = s * mx[i][q], tc = c * mx[i][q], td = s * mx[i][p];
		mx[i][p] = ta + tb;
		mx[i][q] = tc - td;
	}
}

SNML_INLINE scalar::scalar()
{
}

SNML_INLINE scalar::scalar( float rhs )
{
	m_Value = rhs;
}

SNML_INLINE scalar::scalar( scalar_arg rhs )
{
	m_Value = rhs.m_Value;
}

SNML_INLINE scalar::scalar( int value_i )
{
	m_Value = (NativeScalar)value_i;
}

/*SNML_INLINE scalar::scalar( const NativeScalar &x_s )
{
	m_Value = x_s;
}*/

SNML_INLINE void scalar::load( const NativeScalar &x_s )
{
	m_Value = x_s;
}

void scalar::load( const float src[], const int byteOffset )
{
	m_Value = src[ byteOffset>>2 ];
}

void scalar::store( float dest[], const int byteOffset ) const
{
	dest[ byteOffset>>2 ] = m_Value;
}

SNML_INLINE scalar scalar::operator +( scalar_arg rhs ) const
{
	return scalar( (float)( m_Value + rhs.m_Value ) );
}

SNML_INLINE scalar scalar::operator -( scalar_arg rhs ) const
{
	return scalar( (float)( m_Value - rhs.m_Value ) );
}

SNML_INLINE scalar scalar::operator *( scalar_arg rhs ) const
{
	return scalar( (float)( m_Value * rhs.m_Value ) );
}

SNML_INLINE scalar scalar::operator /( scalar_arg rhs ) const
{
	return scalar( (float)( m_Value / rhs.m_Value ) );
}

SNML_INLINE void scalar::operator +=( scalar_arg rhs )
{
	m_Value += rhs.m_Value;
}

SNML_INLINE void scalar::operator -=( scalar_arg rhs )
{
	m_Value -= rhs.m_Value;
}

SNML_INLINE void scalar::operator *=( scalar_arg rhs )
{
	m_Value *= rhs.m_Value;
}

SNML_INLINE void scalar::operator /=( scalar_arg rhs )
{
	m_Value /= rhs.m_Value;
}

SNML_INLINE scalar scalar::operator -() const
{
	return scalar( -m_Value );
}

SNML_INLINE scalar scalar::operator +() const
{
	return *this;
}

SNML_INLINE vquad scalar::operator +( vquad_arg rhs ) const
{
	return vquad( m_Value + rhs.x(), m_Value + rhs.y(), m_Value + rhs.z(), m_Value + rhs.w() );
}

SNML_INLINE vquad scalar::operator -( vquad_arg rhs ) const
{
	return vquad( m_Value - rhs.x(), m_Value - rhs.y(), m_Value - rhs.z(), m_Value - rhs.w() );
}

SNML_INLINE vquad scalar::operator *( vquad_arg rhs ) const
{
	return vquad( m_Value * rhs.x(), m_Value * rhs.y(), m_Value * rhs.z(), m_Value * rhs.w() );
}

SNML_INLINE vquad scalar::operator /( vquad_arg rhs ) const
{
	return vquad( m_Value / rhs.x(), m_Value / rhs.y(), m_Value / rhs.z(), m_Value / rhs.w() );
}

SNML_INLINE quadquad scalar::operator +( quadquad_arg rhs ) const
{
	return quadquad( m_Value + rhs.x(), m_Value + rhs.y(), m_Value + rhs.z(), m_Value + rhs.w() );
}

SNML_INLINE quadquad scalar::operator -( quadquad_arg rhs ) const
{
	return quadquad( m_Value - rhs.x(), m_Value - rhs.y(), m_Value - rhs.z(), m_Value - rhs.w() );
}

SNML_INLINE quadquad scalar::operator *( quadquad_arg rhs ) const
{
	return quadquad( m_Value * rhs.x(), m_Value * rhs.y(), m_Value * rhs.z(), m_Value * rhs.w() );
}

SNML_INLINE quadquad scalar::operator /( quadquad_arg rhs ) const
{
	return quadquad( m_Value / rhs.x(), m_Value / rhs.y(), m_Value / rhs.z(), m_Value / rhs.w() );
}

SNML_INLINE bool scalar::operator ==( scalar_arg rhs ) const
{
	return m_Value == rhs.m_Value;
}

SNML_INLINE bool scalar::operator !=( scalar_arg rhs ) const
{
	return m_Value != rhs.m_Value;
}

SNML_INLINE bool any_ge( scalar_arg x, scalar_arg y )
{
	return x.m_Value >= y.m_Value;
}

SNML_INLINE bool any_gt( scalar_arg x, scalar_arg y )
{
	return x.m_Value > y.m_Value;
}

SNML_INLINE bool any_le( scalar_arg x, scalar_arg y )
{
	return x.m_Value <= y.m_Value;
}

SNML_INLINE bool any_lt( scalar_arg x, scalar_arg y )
{
	return x.m_Value < y.m_Value;
}

SNML_INLINE bool any_eq( scalar_arg x, scalar_arg y )
{
	return x.m_Value == y.m_Value;
}

SNML_INLINE bool any_ne( scalar_arg x, scalar_arg y )
{
	return x.m_Value != y.m_Value;
}

SNML_INLINE bool all_ge( scalar_arg x, scalar_arg y )
{
	return x.m_Value >= y.m_Value;
}

SNML_INLINE bool all_gt( scalar_arg x, scalar_arg y )
{
	return x.m_Value > y.m_Value;
}

SNML_INLINE bool all_le( scalar_arg x, scalar_arg y )
{
	return x.m_Value <= y.m_Value;
}

SNML_INLINE bool all_lt( scalar_arg x, scalar_arg y )
{
	return x.m_Value < y.m_Value;
}

SNML_INLINE bool all_eq( scalar_arg x, scalar_arg y )
{
	return x.m_Value == y.m_Value;
}

SNML_INLINE bool all_ne( scalar_arg x, scalar_arg y )
{
	return x.m_Value != y.m_Value;
}

SNML_INLINE scalar scalar::zero()
{
	return scalar( 0.0f );
}

SNML_INLINE scalar scalar::one()
{
	return scalar( 1.0f );
}

SNML_INLINE scalar scalar::two()
{
	return scalar( 2.0f );
}

SNML_INLINE scalar scalar::three()
{
	return scalar( 3.0f );
}

SNML_INLINE scalar scalar::four()
{
	return scalar( 4.0f );
}

SNML_INLINE scalar scalar::five()
{
	return scalar( 5.0f );
}

SNML_INLINE scalar scalar::six()
{
	return scalar( 6.0f );
}

SNML_INLINE scalar scalar::seven()
{
	return scalar( 7.0f );
}

SNML_INLINE scalar scalar::eight()
{
	return scalar( 8.0f );
}

SNML_INLINE scalar scalar::nine()
{
	return scalar( 9.0f );
}

SNML_INLINE scalar scalar::ten()
{
	return scalar( 10.0f );
}

SNML_INLINE scalar scalar::half()
{
	return scalar( 0.5f );
}

SNML_INLINE scalar scalar::quarter()
{
	return scalar( 0.25f );
}

SNML_INLINE scalar scalar::three_quarters()
{
	return scalar( 0.75f );
}

SNML_INLINE scalar scalar::three_by_2()
{
	return scalar( 1.5f );
}

SNML_INLINE scalar scalar::huge()
{
	return scalar( 340282346638528886604286022844204804240.0f );
}

SNML_INLINE scalar scalar::sqrt2()
{
	return scalar( 1.41421356237309504880f );
}

SNML_INLINE scalar scalar::sqrt_half()
{
	return scalar( 0.707106781186547524401f );
}

SNML_INLINE scalar scalar::two_by_sqrtpi()
{
	return scalar( 1.12837916709551257390f );
}

SNML_INLINE scalar scalar::two_by_pi()
{
	return scalar( 0.636619772367581343076f );
}

SNML_INLINE scalar scalar::one_by_pi()
{
	return scalar( 0.318309886183790671538f );
}

SNML_INLINE scalar scalar::pi_by_4()
{
	return scalar( 0.785398163397448309616f );
}

SNML_INLINE scalar scalar::pi_by_2()
{
	return scalar( 1.57079632679489661923f );
}

SNML_INLINE scalar scalar::pi()
{
	return scalar( 3.14159265358979323846f );
}

SNML_INLINE scalar scalar::e()
{
	return scalar( 2.71828182845904523536f );
}

SNML_INLINE scalar scalar::log2e()
{
	return scalar( 1.44269504088896340736f );
}

SNML_INLINE scalar scalar::log10e()
{
	return scalar( 0.434294481903251827651f );
}

SNML_INLINE scalar scalar::ln2()
{
	return scalar( 0.693147180559945309417f );
}

SNML_INLINE scalar scalar::ln10()
{
	return scalar( 2.30258509299404568402f );
}

SNML_INLINE scalar scalar::two_pi()
{
	return scalar( 3.14159265358979323846f*2.0f );
}

SNML_INLINE scalar scalar::pi_by_6()
{
	return scalar( 3.14159265358979323846f/6 );
}

SNML_INLINE scalar scalar::log10two()
{
	return scalar( .30102999566398119522f );
}

SNML_INLINE scalar scalar::log2ten()
{
	return scalar( 3.3219280948873623478f );
}

SNML_INLINE scalar scalar::sqrt3_by_2()
{
	return scalar( .86602540378443864675f );
}

SNML_INLINE vquad::vquad()
{
}

SNML_INLINE vquad::vquad( const NativeQuad value )
{
	m_Value[ 0 ] = value[ 0 ];
	m_Value[ 1 ] = value[ 1 ];
	m_Value[ 2 ] = value[ 2 ];
	m_Value[ 3 ] = value[ 3 ];
}

SNML_INLINE void vquad::load1( const float src[], const int byteOffset )
{
	m_Value[ 0 ] = m_Value[ 1 ] = m_Value[ 2 ] = m_Value[ 3 ] = src[ byteOffset>>2 ];
}

SNML_INLINE void vquad::load_unaligned( const float src[], const int byteOffset )
{
	m_Value[ 0 ] = src[ (byteOffset>>2) + 0 ];
	m_Value[ 1 ] = src[ (byteOffset>>2) + 1 ];
	m_Value[ 2 ] = src[ (byteOffset>>2) + 2 ];
	m_Value[ 3 ] = src[ (byteOffset>>2) + 3 ];
}

SNML_INLINE void vquad::load( const float src[], const int byteOffset )
{
	m_Value[ 0 ] = src[ (byteOffset>>2) + 0 ];
	m_Value[ 1 ] = src[ (byteOffset>>2) + 1 ];
	m_Value[ 2 ] = src[ (byteOffset>>2) + 2 ];
	m_Value[ 3 ] = src[ (byteOffset>>2) + 3 ];
}

SNML_INLINE void vquad::load_nocache( const float src[], const int byteOffset )
{
	m_Value[ 0 ] = src[ (byteOffset>>2) + 0 ];
	m_Value[ 1 ] = src[ (byteOffset>>2) + 1 ];
	m_Value[ 2 ] = src[ (byteOffset>>2) + 2 ];
	m_Value[ 3 ] = src[ (byteOffset>>2) + 3 ];
}

SNML_INLINE void vquad::load1( const signed int src[], const int byteOffset )
{
	m_Value[ 0 ] = m_Value[ 1 ] = m_Value[ 2 ] = m_Value[ 3 ] = (float)src[ (byteOffset>>2) + 0 ];
}

SNML_INLINE void vquad::load_unaligned( const signed int src[], const int byteOffset )
{
	m_Value[ 0 ] = (float)src[ (byteOffset>>2) + 0 ];
	m_Value[ 1 ] = (float)src[ (byteOffset>>2) + 1 ];
	m_Value[ 2 ] = (float)src[ (byteOffset>>2) + 2 ];
	m_Value[ 3 ] = (float)src[ (byteOffset>>2) + 3 ];
}

SNML_INLINE void vquad::load( const signed int src[], const int byteOffset )
{
	m_Value[ 0 ] = (float)src[ (byteOffset>>2) + 0 ];
	m_Value[ 1 ] = (float)src[ (byteOffset>>2) + 1 ];
	m_Value[ 2 ] = (float)src[ (byteOffset>>2) + 2 ];
	m_Value[ 3 ] = (float)src[ (byteOffset>>2) + 3 ];
}

SNML_INLINE void vquad::load1( const signed short src[], const int byteOffset )
{
	m_Value[ 0 ] = m_Value[ 1 ] = m_Value[ 2 ] = m_Value[ 3 ] = src[ (byteOffset>>1) + 0 ];
}

SNML_INLINE void vquad::load_unaligned( const signed short src[], const int byteOffset )
{
	m_Value[ 0 ] = src[ (byteOffset>>1) + 0 ];
	m_Value[ 1 ] = src[ (byteOffset>>1) + 1 ];
	m_Value[ 2 ] = src[ (byteOffset>>1) + 2 ];
	m_Value[ 3 ] = src[ (byteOffset>>1) + 3 ];
}

SNML_INLINE void vquad::load( const signed short src[], const int byteOffset )
{
	m_Value[ 0 ] = src[ (byteOffset>>1) + 0 ];
	m_Value[ 1 ] = src[ (byteOffset>>1) + 1 ];
	m_Value[ 2 ] = src[ (byteOffset>>1) + 2 ];
	m_Value[ 3 ] = src[ (byteOffset>>1) + 3 ];
}

SNML_INLINE void vquad::set_x( scalar_arg value )
{
	m_Value[ 0 ] = value.m_Value;
}

SNML_INLINE void vquad::set_y( scalar_arg value )
{
	m_Value[ 1 ] = value.m_Value;
}

SNML_INLINE void vquad::set_z( scalar_arg value )
{
	m_Value[ 2 ] = value.m_Value;
}

SNML_INLINE void vquad::set_w( scalar_arg value )
{
	m_Value[ 3 ] = value.m_Value;
}

SNML_INLINE void vquad::store( NativeScalar &x_s ) const
{
	x_s = m_Value[ 0 ];
}

SNML_INLINE void vquad::store( NativeScalar &x_s, NativeScalar &y_s ) const
{
	x_s = m_Value[ 0 ];
	y_s = m_Value[ 1 ];
}

SNML_INLINE void vquad::store( NativeScalar &x_s, NativeScalar &y_s, NativeScalar &z_s ) const
{
	x_s = m_Value[ 0 ];
	y_s = m_Value[ 1 ];
	z_s = m_Value[ 2 ];
}

SNML_INLINE void vquad::store( NativeScalar &x_s, NativeScalar &y_s, NativeScalar &z_s, NativeScalar &w_s ) const
{
	x_s = m_Value[ 0 ];
	y_s = m_Value[ 1 ];
	z_s = m_Value[ 2 ];
	w_s = m_Value[ 3 ];
}

SNML_INLINE void vquad::store( signed int dest[] ) const
{
	dest[ 0 ] = m_Value[ 0 ];
	dest[ 1 ] = m_Value[ 1 ];
	dest[ 2 ] = m_Value[ 2 ];
	dest[ 3 ] = m_Value[ 3 ];
}

SNML_INLINE void vquad::store( signed short dest[] ) const
{
	dest[ 0 ] = m_Value[ 0 ];
	dest[ 1 ] = m_Value[ 1 ];
	dest[ 2 ] = m_Value[ 2 ];
	dest[ 3 ] = m_Value[ 3 ];
}

SNML_INLINE void vquad::store( signed char dest[] ) const
{
	dest[ 0 ] = m_Value[ 0 ];
	dest[ 1 ] = m_Value[ 1 ];
	dest[ 2 ] = m_Value[ 2 ];
	dest[ 3 ] = m_Value[ 3 ];
}

SNML_INLINE void vquad::store( unsigned int dest[] ) const
{
	dest[ 0 ] = m_Value[ 0 ];
	dest[ 1 ] = m_Value[ 1 ];
	dest[ 2 ] = m_Value[ 2 ];
	dest[ 3 ] = m_Value[ 3 ];
}

SNML_INLINE void vquad::store( unsigned short dest[] ) const
{
	dest[ 0 ] = m_Value[ 0 ];
	dest[ 1 ] = m_Value[ 1 ];
	dest[ 2 ] = m_Value[ 2 ];
	dest[ 3 ] = m_Value[ 3 ];
}

SNML_INLINE void vquad::store( unsigned char dest[] ) const
{
	dest[ 0 ] = m_Value[ 0 ];
	dest[ 1 ] = m_Value[ 1 ];
	dest[ 2 ] = m_Value[ 2 ];
	dest[ 3 ] = m_Value[ 3 ];
}

SNML_INLINE void vquad::store( float dest[] ) const
{
	dest[ 0 ] = m_Value[ 0 ];
	dest[ 1 ] = m_Value[ 1 ];
	dest[ 2 ] = m_Value[ 2 ];
	dest[ 3 ] = m_Value[ 3 ];
}

SNML_INLINE void vquad::store_nocache( float dest[] ) const
{
	dest[ 0 ] = m_Value[ 0 ];
	dest[ 1 ] = m_Value[ 1 ];
	dest[ 2 ] = m_Value[ 2 ];
	dest[ 3 ] = m_Value[ 3 ];
}

SNML_INLINE vquad vquad::operator +( vquad_arg rhs ) const
{
	return vquad( m_Value[ 0 ] + rhs.m_Value[ 0 ], m_Value[ 1 ] + rhs.m_Value[ 1 ], m_Value[ 2 ] + rhs.m_Value[ 2 ], m_Value[ 3 ] + rhs.m_Value[ 3 ] );
}

SNML_INLINE vquad vquad::operator -( vquad_arg rhs ) const
{
	return vquad( m_Value[ 0 ] - rhs.m_Value[ 0 ], m_Value[ 1 ] - rhs.m_Value[ 1 ], m_Value[ 2 ] - rhs.m_Value[ 2 ], m_Value[ 3 ] - rhs.m_Value[ 3 ] );
}

SNML_INLINE vquad vquad::operator *( vquad_arg rhs ) const
{
	return vquad( m_Value[ 0 ] * rhs.m_Value[ 0 ], m_Value[ 1 ] * rhs.m_Value[ 1 ], m_Value[ 2 ] * rhs.m_Value[ 2 ], m_Value[ 3 ] * rhs.m_Value[ 3 ] );
}

SNML_INLINE vquad vquad::operator /( vquad_arg rhs ) const
{
	return vquad( m_Value[ 0 ] / rhs.m_Value[ 0 ], m_Value[ 1 ] / rhs.m_Value[ 1 ], m_Value[ 2 ] / rhs.m_Value[ 2 ], m_Value[ 3 ] / rhs.m_Value[ 3 ] );
}

SNML_INLINE void vquad::operator +=( vquad_arg rhs )
{
	m_Value[ 0 ] += rhs.m_Value[ 0 ];
	m_Value[ 1 ] += rhs.m_Value[ 1 ];
	m_Value[ 2 ] += rhs.m_Value[ 2 ];
	m_Value[ 3 ] += rhs.m_Value[ 3 ];
}

SNML_INLINE void vquad::operator -=( vquad_arg rhs )
{
	m_Value[ 0 ] -= rhs.m_Value[ 0 ];
	m_Value[ 1 ] -= rhs.m_Value[ 1 ];
	m_Value[ 2 ] -= rhs.m_Value[ 2 ];
	m_Value[ 3 ] -= rhs.m_Value[ 3 ];
}

SNML_INLINE void vquad::operator *=( vquad_arg rhs )
{
	m_Value[ 0 ] *= rhs.m_Value[ 0 ];
	m_Value[ 1 ] *= rhs.m_Value[ 1 ];
	m_Value[ 2 ] *= rhs.m_Value[ 2 ];
	m_Value[ 3 ] *= rhs.m_Value[ 3 ];
}

SNML_INLINE void vquad::operator /=( vquad_arg rhs )
{
	m_Value[ 0 ] /= rhs.m_Value[ 0 ];
	m_Value[ 1 ] /= rhs.m_Value[ 1 ];
	m_Value[ 2 ] /= rhs.m_Value[ 2 ];
	m_Value[ 3 ] /= rhs.m_Value[ 3 ];
}

SNML_INLINE vquad vquad::operator -() const
{
	return vquad( -m_Value[ 0 ], -m_Value[ 1 ], -m_Value[ 2 ], -m_Value[ 3 ] );
}

SNML_INLINE vquad vquad::operator +() const
{
	return *this;
}


SNML_INLINE vquad vquad::operator +( scalar_arg rhs ) const
{
	return vquad( m_Value[ 0 ] + rhs.m_Value, m_Value[ 1 ] + rhs.m_Value, m_Value[ 2 ] + rhs.m_Value, m_Value[ 3 ] + rhs.m_Value );
}

SNML_INLINE vquad vquad::operator -( scalar_arg rhs ) const
{
	return vquad( m_Value[ 0 ] - rhs.m_Value, m_Value[ 1 ] - rhs.m_Value, m_Value[ 2 ] - rhs.m_Value, m_Value[ 3 ] - rhs.m_Value );
}

SNML_INLINE vquad vquad::operator *( scalar_arg rhs ) const
{
	return vquad( m_Value[ 0 ] * rhs.m_Value, m_Value[ 1 ] * rhs.m_Value, m_Value[ 2 ] * rhs.m_Value, m_Value[ 3 ] * rhs.m_Value );
}

SNML_INLINE vquad vquad::operator /( scalar_arg rhs ) const
{
	return vquad( m_Value[ 0 ] / rhs.m_Value, m_Value[ 1 ] / rhs.m_Value, m_Value[ 2 ] / rhs.m_Value, m_Value[ 3 ] / rhs.m_Value );
}

SNML_INLINE void vquad::operator +=( scalar_arg rhs )
{
	m_Value[ 0 ] += rhs.m_Value;
	m_Value[ 1 ] += rhs.m_Value;
	m_Value[ 2 ] += rhs.m_Value;
	m_Value[ 3 ] += rhs.m_Value;
}

SNML_INLINE void vquad::operator -=( scalar_arg rhs )
{
	m_Value[ 0 ] -= rhs.m_Value;
	m_Value[ 1 ] -= rhs.m_Value;
	m_Value[ 2 ] -= rhs.m_Value;
	m_Value[ 3 ] -= rhs.m_Value;
}

SNML_INLINE void vquad::operator *=( scalar_arg rhs )
{
	m_Value[ 0 ] *= rhs.m_Value;
	m_Value[ 1 ] *= rhs.m_Value;
	m_Value[ 2 ] *= rhs.m_Value;
	m_Value[ 3 ] *= rhs.m_Value;
}

SNML_INLINE void vquad::operator /=( scalar_arg rhs )
{
	m_Value[ 0 ] /= rhs.m_Value;
	m_Value[ 1 ] /= rhs.m_Value;
	m_Value[ 2 ] /= rhs.m_Value;
	m_Value[ 3 ] /= rhs.m_Value;
}

SNML_INLINE bool vquad::operator ==( vquad_arg rhs ) const
{
	return m_Value[ 0 ] == rhs.m_Value[ 0 ] && m_Value[ 1 ] == rhs.m_Value[ 1 ] && m_Value[ 2 ] == rhs.m_Value[ 2 ] && m_Value[ 3 ] == rhs.m_Value[ 3 ];
}

SNML_INLINE bool vquad::operator !=( vquad_arg rhs ) const
{
	return m_Value[ 0 ] != rhs.m_Value[ 0 ] || m_Value[ 1 ] != rhs.m_Value[ 1 ] || m_Value[ 2 ] != rhs.m_Value[ 2 ] || m_Value[ 3 ] != rhs.m_Value[ 3 ];
}

SNML_INLINE bool any_ge( vquad_arg x, vquad_arg y )
{
	return x.m_Value[ 0 ] >= y.m_Value[ 0 ] || x.m_Value[ 1 ] >= y.m_Value[ 1 ] || x.m_Value[ 2 ] >= y.m_Value[ 2 ] || x.m_Value[ 3 ] >= y.m_Value[ 3 ];
}

SNML_INLINE bool any_gt( vquad_arg x, vquad_arg y )
{
	return x.m_Value[ 0 ] > y.m_Value[ 0 ] || x.m_Value[ 1 ] > y.m_Value[ 1 ] || x.m_Value[ 2 ] > y.m_Value[ 2 ] || x.m_Value[ 3 ] > y.m_Value[ 3 ];
}

SNML_INLINE bool any_le( vquad_arg x, vquad_arg y )
{
	return x.m_Value[ 0 ] <= y.m_Value[ 0 ] || x.m_Value[ 1 ] <= y.m_Value[ 1 ] || x.m_Value[ 2 ] <= y.m_Value[ 2 ] || x.m_Value[ 3 ] <= y.m_Value[ 3 ];
}

SNML_INLINE bool any_lt( vquad_arg x, vquad_arg y )
{
	return x.m_Value[ 0 ] < y.m_Value[ 0 ] || x.m_Value[ 1 ] < y.m_Value[ 1 ] || x.m_Value[ 2 ] < y.m_Value[ 2 ] || x.m_Value[ 3 ] < y.m_Value[ 3 ];
}

SNML_INLINE bool any_eq( vquad_arg x, vquad_arg y )
{
	return x.m_Value[ 0 ] == y.m_Value[ 0 ] || x.m_Value[ 1 ] == y.m_Value[ 1 ] || x.m_Value[ 2 ] == y.m_Value[ 2 ] || x.m_Value[ 3 ] == y.m_Value[ 3 ];
}

SNML_INLINE bool any_ne( vquad_arg x, vquad_arg y )
{
	return x.m_Value[ 0 ] != y.m_Value[ 0 ] || x.m_Value[ 1 ] != y.m_Value[ 1 ] || x.m_Value[ 2 ] != y.m_Value[ 2 ] || x.m_Value[ 3 ] != y.m_Value[ 3 ];
}

SNML_INLINE bool all_ge( vquad_arg x, vquad_arg y )
{
	return x.m_Value[ 0 ] >= y.m_Value[ 0 ] && x.m_Value[ 1 ] >= y.m_Value[ 1 ] && x.m_Value[ 2 ] >= y.m_Value[ 2 ] && x.m_Value[ 3 ] >= y.m_Value[ 3 ];
}

SNML_INLINE bool all_gt( vquad_arg x, vquad_arg y )
{
	return x.m_Value[ 0 ] > y.m_Value[ 0 ] && x.m_Value[ 1 ] > y.m_Value[ 1 ] && x.m_Value[ 2 ] > y.m_Value[ 2 ] && x.m_Value[ 3 ] > y.m_Value[ 3 ];
}

SNML_INLINE bool all_le( vquad_arg x, vquad_arg y )
{
	return x.m_Value[ 0 ] <= y.m_Value[ 0 ] && x.m_Value[ 1 ] <= y.m_Value[ 1 ] && x.m_Value[ 2 ] <= y.m_Value[ 2 ] && x.m_Value[ 3 ] <= y.m_Value[ 3 ];
}

SNML_INLINE bool all_lt( vquad_arg x, vquad_arg y )
{
	return x.m_Value[ 0 ] < y.m_Value[ 0 ] && x.m_Value[ 1 ] < y.m_Value[ 1 ] && x.m_Value[ 2 ] < y.m_Value[ 2 ] && x.m_Value[ 3 ] < y.m_Value[ 3 ];
}

SNML_INLINE bool all_eq( vquad_arg x, vquad_arg y )
{
	return x.m_Value[ 0 ] == y.m_Value[ 0 ] && x.m_Value[ 1 ] == y.m_Value[ 1 ] && x.m_Value[ 2 ] == y.m_Value[ 2 ] && x.m_Value[ 3 ] == y.m_Value[ 3 ];
}

SNML_INLINE bool all_ne( vquad_arg x, vquad_arg y )
{
	return x.m_Value[ 0 ] != y.m_Value[ 0 ] && x.m_Value[ 1 ] != y.m_Value[ 1 ] && x.m_Value[ 2 ] != y.m_Value[ 2 ] && x.m_Value[ 3 ] != y.m_Value[ 3 ];
}

SNML_INLINE vquad vquad::zero()
{
	return vquad( 0.0f );
}

SNML_INLINE vquad vquad::one()
{
	return vquad( 1.0f );
}

SNML_INLINE vquad vquad::two()
{
	return vquad( 2.0f );
}

SNML_INLINE vquad vquad::three()
{
	return vquad( 3.0f );
}

SNML_INLINE vquad vquad::four()
{
	return vquad( 4.0f );
}

SNML_INLINE vquad vquad::five()
{
	return vquad( 5.0f );
}

SNML_INLINE vquad vquad::six()
{
	return vquad( 6.0f );
}

SNML_INLINE vquad vquad::seven()
{
	return vquad( 7.0f );
}

SNML_INLINE vquad vquad::eight()
{
	return vquad( 8.0f );
}

SNML_INLINE vquad vquad::nine()
{
	return vquad( 9.0f );
}

SNML_INLINE vquad vquad::ten()
{
	return vquad( 10 );
}

SNML_INLINE vquad vquad::half()
{
	return vquad( 0.5f );
}

SNML_INLINE vquad vquad::quarter()
{
	return vquad( 0.25f );
}

SNML_INLINE vquad vquad::three_quarters()
{
	return vquad( 0.75f );
}

SNML_INLINE vquad vquad::three_by_2()
{
	return vquad( 1.5f );
}

SNML_INLINE vquad vquad::huge()
{
	return vquad( 340282346638528886604286022844204804240.0f );
}

SNML_INLINE vquad vquad::sqrt2()
{
	return vquad( 1.41421356237309504880f );
}

SNML_INLINE vquad vquad::sqrt_half()
{
	return vquad( 0.707106781186547524401f );
}

SNML_INLINE vquad vquad::two_by_sqrtpi()
{
	return vquad( 1.12837916709551257390f );
}

SNML_INLINE vquad vquad::two_by_pi()
{
	return vquad( 0.636619772367581343076f );
}

SNML_INLINE vquad vquad::one_by_pi()
{
	return vquad( 0.318309886183790671538f );
}

SNML_INLINE vquad vquad::pi_by_4()
{
	return vquad( 0.785398163397448309616f );
}

SNML_INLINE vquad vquad::pi_by_2()
{
	return vquad( 1.57079632679489661923f );
}

SNML_INLINE vquad vquad::pi()
{
	return vquad( 3.14159265358979323846f );
}

SNML_INLINE vquad vquad::e()
{
	return vquad( 2.71828182845904523536f );
}

SNML_INLINE vquad vquad::log2e()
{
	return vquad( 1.44269504088896340736f );
}

SNML_INLINE vquad vquad::log10e()
{
	return vquad( 0.434294481903251827651f );
}

SNML_INLINE vquad vquad::ln2()
{
	return vquad( 0.693147180559945309417f );
}

SNML_INLINE vquad vquad::ln10()
{
	return vquad( 2.30258509299404568402f );
}

SNML_INLINE vquad vquad::two_pi()
{
	return vquad( 3.14159265358979323846f*2.0f );
}

SNML_INLINE vquad vquad::pi_by_6()
{
	return vquad( 3.14159265358979323846f/6 );
}

SNML_INLINE vquad vquad::log10two()
{
	return vquad( .30102999566398119522f );
}

SNML_INLINE vquad vquad::log2ten()
{
	return vquad( 3.3219280948873623478f );
}

SNML_INLINE vquad vquad::sqrt3_by_2()
{
	return vquad( .86602540378443864675f );
}

SNML_INLINE vquad vquad::unit()
{
	return vquad( 0, 0, 0, 1.0f );
}

SNML_INLINE quadquad::quadquad()
{
}

SNML_INLINE quadquad::quadquad( float value_f )
{
	m_X[ 0 ] = value_f; m_X[ 1 ] = value_f; m_X[ 2 ] = value_f; m_X[ 3 ] = value_f;
	m_Y[ 0 ] = value_f; m_Y[ 1 ] = value_f; m_Y[ 2 ] = value_f; m_Y[ 3 ] = value_f;
	m_Z[ 0 ] = value_f; m_Z[ 1 ] = value_f; m_Z[ 2 ] = value_f; m_Z[ 3 ] = value_f;
	m_W[ 0 ] = value_f; m_W[ 1 ] = value_f; m_W[ 2 ] = value_f; m_W[ 3 ] = value_f;
}

SNML_INLINE quadquad::quadquad( scalar_arg value_s )
{
	m_X[ 0 ] = value_s.m_Value; m_X[ 1 ] = value_s.m_Value; m_X[ 2 ] = value_s.m_Value; m_X[ 3 ] = value_s.m_Value;
	m_Y[ 0 ] = value_s.m_Value; m_Y[ 1 ] = value_s.m_Value; m_Y[ 2 ] = value_s.m_Value; m_Y[ 3 ] = value_s.m_Value;
	m_Z[ 0 ] = value_s.m_Value; m_Z[ 1 ] = value_s.m_Value; m_Z[ 2 ] = value_s.m_Value; m_Z[ 3 ] = value_s.m_Value;
	m_W[ 0 ] = value_s.m_Value; m_W[ 1 ] = value_s.m_Value; m_W[ 2 ] = value_s.m_Value; m_W[ 3 ] = value_s.m_Value;
}

SNML_INLINE quadquad::quadquad( float xx, float xy, float xz, float xw, float yx, float yy, float yz, float yw, float zx, float zy, float zz, float zw, float wx, float wy, float wz, float ww )
{
	m_X[ 0 ] = xx; m_X[ 1 ] = xy; m_X[ 2 ] = xz; m_X[ 3 ] = xw;
	m_Y[ 0 ] = yx; m_Y[ 1 ] = yy; m_Y[ 2 ] = yz; m_Y[ 3 ] = yw;
	m_Z[ 0 ] = zx; m_Z[ 1 ] = zy; m_Z[ 2 ] = zz; m_Z[ 3 ] = zw;
	m_W[ 0 ] = wx; m_W[ 1 ] = wy; m_W[ 2 ] = wz; m_W[ 3 ] = ww;
}

SNML_INLINE quadquad::quadquad( vquad_arg x_s, vquad_arg y_s, vquad_arg z_s, vquad_arg w_s )
{
	m_X[ 0 ] = x_s.m_Value[ 0 ]; m_X[ 1 ] = x_s.m_Value[ 1 ]; m_X[ 2 ] = x_s.m_Value[ 2 ]; m_X[ 3 ] = x_s.m_Value[ 3 ];
	m_Y[ 0 ] = y_s.m_Value[ 0 ]; m_Y[ 1 ] = y_s.m_Value[ 1 ]; m_Y[ 2 ] = y_s.m_Value[ 2 ]; m_Y[ 3 ] = y_s.m_Value[ 3 ];
	m_Z[ 0 ] = z_s.m_Value[ 0 ]; m_Z[ 1 ] = z_s.m_Value[ 1 ]; m_Z[ 2 ] = z_s.m_Value[ 2 ]; m_Z[ 3 ] = z_s.m_Value[ 3 ];
	m_W[ 0 ] = w_s.m_Value[ 0 ]; m_W[ 1 ] = w_s.m_Value[ 1 ]; m_W[ 2 ] = w_s.m_Value[ 2 ]; m_W[ 3 ] = w_s.m_Value[ 3 ];
}

SNML_INLINE quadquad::quadquad( const float src[] )
{
	m_X[ 0 ] = src[ 0 ]; m_X[ 1 ] = src[ 1 ]; m_X[ 2 ] = src[ 2 ]; m_X[ 3 ] = src[ 3 ];
	m_Y[ 0 ] = src[ 0+4 ]; m_Y[ 1 ] = src[ 1+4 ]; m_Y[ 2 ] = src[ 2+4 ]; m_Y[ 3 ] = src[ 3+4 ];
	m_Z[ 0 ] = src[ 0+8 ]; m_Z[ 1 ] = src[ 1+8 ]; m_Z[ 2 ] = src[ 2+8 ]; m_Z[ 3 ] = src[ 3+8 ];
	m_W[ 0 ] = src[ 0+12 ]; m_W[ 1 ] = src[ 1+12 ]; m_W[ 2 ] = src[ 2+12 ]; m_W[ 3 ] = src[ 3+12 ];
}

SNML_INLINE quadquad::quadquad( const scalar src[] )
{
	m_X[ 0 ] = src[ 0 ].m_Value; m_X[ 1 ] = src[ 1 ].m_Value; m_X[ 2 ] = src[ 2 ].m_Value; m_X[ 3 ] = src[ 3 ].m_Value;
	m_Y[ 0 ] = src[ 0+4 ].m_Value; m_Y[ 1 ] = src[ 1+4 ].m_Value; m_Y[ 2 ] = src[ 2+4 ].m_Value; m_Y[ 3 ] = src[ 3+4 ].m_Value;
	m_Z[ 0 ] = src[ 0+8 ].m_Value; m_Z[ 1 ] = src[ 1+8 ].m_Value; m_Z[ 2 ] = src[ 2+8 ].m_Value; m_Z[ 3 ] = src[ 3+8 ].m_Value;
	m_W[ 0 ] = src[ 0+12 ].m_Value; m_W[ 1 ] = src[ 1+12 ].m_Value; m_W[ 2 ] = src[ 2+12 ].m_Value; m_W[ 3 ] = src[ 3+12 ].m_Value;
}

SNML_INLINE void quadquad::load( float xx, float xy, float xz, float xw, float yx, float yy, float yz, float yw, float zx, float zy, float zz, float zw, float wx, float wy, float wz, float ww )
{
	m_X[ 0 ] = xx; m_X[ 1 ] = xy; m_X[ 2 ] = xz; m_X[ 3 ] = xw;
	m_Y[ 0 ] = yx; m_Y[ 1 ] = yy; m_Y[ 2 ] = yz; m_Y[ 3 ] = yw;
	m_Z[ 0 ] = zx; m_Z[ 1 ] = zy; m_Z[ 2 ] = zz; m_Z[ 3 ] = zw;
	m_W[ 0 ] = wx; m_W[ 1 ] = wy; m_W[ 2 ] = wz; m_W[ 3 ] = ww;
}

SNML_INLINE void quadquad::load( const float src[] )
{
	m_X[ 0 ] = src[ 0 ]; m_X[ 1 ] = src[ 1 ]; m_X[ 2 ] = src[ 2 ]; m_X[ 3 ] = src[ 3 ];
	m_Y[ 0 ] = src[ 0+4 ]; m_Y[ 1 ] = src[ 1+4 ]; m_Y[ 2 ] = src[ 2+4 ]; m_Y[ 3 ] = src[ 3+4 ];
	m_Z[ 0 ] = src[ 0+8 ]; m_Z[ 1 ] = src[ 1+8 ]; m_Z[ 2 ] = src[ 2+8 ]; m_Z[ 3 ] = src[ 3+8 ];
	m_W[ 0 ] = src[ 0+12 ]; m_W[ 1 ] = src[ 1+12 ]; m_W[ 2 ] = src[ 2+12 ]; m_W[ 3 ] = src[ 3+12 ];
}

SNML_INLINE void quadquad::load( vquad_arg x_s, vquad_arg y_s, vquad_arg z_s, vquad_arg w_s )
{
	m_X[ 0 ] = x_s.m_Value[ 0 ]; m_X[ 1 ] = x_s.m_Value[ 1 ]; m_X[ 2 ] = x_s.m_Value[ 2 ]; m_X[ 3 ] = x_s.m_Value[ 3 ];
	m_Y[ 0 ] = y_s.m_Value[ 0 ]; m_Y[ 1 ] = y_s.m_Value[ 1 ]; m_Y[ 2 ] = y_s.m_Value[ 2 ]; m_Y[ 3 ] = y_s.m_Value[ 3 ];
	m_Z[ 0 ] = z_s.m_Value[ 0 ]; m_Z[ 1 ] = z_s.m_Value[ 1 ]; m_Z[ 2 ] = z_s.m_Value[ 2 ]; m_Z[ 3 ] = z_s.m_Value[ 3 ];
	m_W[ 0 ] = w_s.m_Value[ 0 ]; m_W[ 1 ] = w_s.m_Value[ 1 ]; m_W[ 2 ] = w_s.m_Value[ 2 ]; m_W[ 3 ] = w_s.m_Value[ 3 ];
}

SNML_INLINE vquad quadquad::x() const
{
	return vquad( m_X );
}

SNML_INLINE vquad quadquad::y() const
{
	return vquad( m_Y );
}

SNML_INLINE vquad quadquad::z() const
{
	return vquad( m_Z );
}

SNML_INLINE vquad quadquad::w() const
{
	return vquad( m_W );
}

SNML_INLINE quadquad quadquad::operator +( quadquad_arg rhs ) const
{
	quadquad result;
	result.m_X[ 0 ] = m_X[ 0 ] + rhs.m_X[ 0 ]; result.m_X[ 1 ] = m_X[ 1 ] + rhs.m_X[ 1 ]; result.m_X[ 2 ] = m_X[ 2 ] + rhs.m_X[ 2 ]; result.m_X[ 3 ] = m_X[ 3 ] + rhs.m_X[ 3 ];
	result.m_Y[ 0 ] = m_Y[ 0 ] + rhs.m_Y[ 0 ]; result.m_Y[ 1 ] = m_Y[ 1 ] + rhs.m_Y[ 1 ]; result.m_Y[ 2 ] = m_Y[ 2 ] + rhs.m_Y[ 2 ]; result.m_Y[ 3 ] = m_Y[ 3 ] + rhs.m_Y[ 3 ];
	result.m_Z[ 0 ] = m_Z[ 0 ] + rhs.m_Z[ 0 ]; result.m_Z[ 1 ] = m_Z[ 1 ] + rhs.m_Z[ 1 ]; result.m_Z[ 2 ] = m_Z[ 2 ] + rhs.m_Z[ 2 ]; result.m_Z[ 3 ] = m_Z[ 3 ] + rhs.m_Z[ 3 ];
	result.m_W[ 0 ] = m_W[ 0 ] + rhs.m_W[ 0 ]; result.m_W[ 1 ] = m_W[ 1 ] + rhs.m_W[ 1 ]; result.m_W[ 2 ] = m_W[ 2 ] + rhs.m_W[ 2 ]; result.m_W[ 3 ] = m_W[ 3 ] + rhs.m_W[ 3 ];
	return result;
}

SNML_INLINE quadquad quadquad::operator -( quadquad_arg rhs ) const
{
	quadquad result;
	result.m_X[ 0 ] = m_X[ 0 ] - rhs.m_X[ 0 ]; result.m_X[ 1 ] = m_X[ 1 ] - rhs.m_X[ 1 ]; result.m_X[ 2 ] = m_X[ 2 ] - rhs.m_X[ 2 ]; result.m_X[ 3 ] = m_X[ 3 ] - rhs.m_X[ 3 ];
	result.m_Y[ 0 ] = m_Y[ 0 ] - rhs.m_Y[ 0 ]; result.m_Y[ 1 ] = m_Y[ 1 ] - rhs.m_Y[ 1 ]; result.m_Y[ 2 ] = m_Y[ 2 ] - rhs.m_Y[ 2 ]; result.m_Y[ 3 ] = m_Y[ 3 ] - rhs.m_Y[ 3 ];
	result.m_Z[ 0 ] = m_Z[ 0 ] - rhs.m_Z[ 0 ]; result.m_Z[ 1 ] = m_Z[ 1 ] - rhs.m_Z[ 1 ]; result.m_Z[ 2 ] = m_Z[ 2 ] - rhs.m_Z[ 2 ]; result.m_Z[ 3 ] = m_Z[ 3 ] - rhs.m_Z[ 3 ];
	result.m_W[ 0 ] = m_W[ 0 ] - rhs.m_W[ 0 ]; result.m_W[ 1 ] = m_W[ 1 ] - rhs.m_W[ 1 ]; result.m_W[ 2 ] = m_W[ 2 ] - rhs.m_W[ 2 ]; result.m_W[ 3 ] = m_W[ 3 ] - rhs.m_W[ 3 ];
	return result;
}

SNML_INLINE quadquad quadquad::operator *( quadquad_arg rhs ) const
{
	quadquad result;
	result.m_X[ 0 ] = m_X[ 0 ] * rhs.m_X[ 0 ]; result.m_X[ 1 ] = m_X[ 1 ] * rhs.m_X[ 1 ]; result.m_X[ 2 ] = m_X[ 2 ] * rhs.m_X[ 2 ]; result.m_X[ 3 ] = m_X[ 3 ] * rhs.m_X[ 3 ];
	result.m_Y[ 0 ] = m_Y[ 0 ] * rhs.m_Y[ 0 ]; result.m_Y[ 1 ] = m_Y[ 1 ] * rhs.m_Y[ 1 ]; result.m_Y[ 2 ] = m_Y[ 2 ] * rhs.m_Y[ 2 ]; result.m_Y[ 3 ] = m_Y[ 3 ] * rhs.m_Y[ 3 ];
	result.m_Z[ 0 ] = m_Z[ 0 ] * rhs.m_Z[ 0 ]; result.m_Z[ 1 ] = m_Z[ 1 ] * rhs.m_Z[ 1 ]; result.m_Z[ 2 ] = m_Z[ 2 ] * rhs.m_Z[ 2 ]; result.m_Z[ 3 ] = m_Z[ 3 ] * rhs.m_Z[ 3 ];
	result.m_W[ 0 ] = m_W[ 0 ] * rhs.m_W[ 0 ]; result.m_W[ 1 ] = m_W[ 1 ] * rhs.m_W[ 1 ]; result.m_W[ 2 ] = m_W[ 2 ] * rhs.m_W[ 2 ]; result.m_W[ 3 ] = m_W[ 3 ] * rhs.m_W[ 3 ];
	return result;
}

SNML_INLINE quadquad quadquad::operator /( quadquad_arg rhs ) const
{
	quadquad result;
	result.m_X[ 0 ] = m_X[ 0 ] / rhs.m_X[ 0 ]; result.m_X[ 1 ] = m_X[ 1 ] / rhs.m_X[ 1 ]; result.m_X[ 2 ] = m_X[ 2 ] / rhs.m_X[ 2 ]; result.m_X[ 3 ] = m_X[ 3 ] / rhs.m_X[ 3 ];
	result.m_Y[ 0 ] = m_Y[ 0 ] / rhs.m_Y[ 0 ]; result.m_Y[ 1 ] = m_Y[ 1 ] / rhs.m_Y[ 1 ]; result.m_Y[ 2 ] = m_Y[ 2 ] / rhs.m_Y[ 2 ]; result.m_Y[ 3 ] = m_Y[ 3 ] / rhs.m_Y[ 3 ];
	result.m_Z[ 0 ] = m_Z[ 0 ] / rhs.m_Z[ 0 ]; result.m_Z[ 1 ] = m_Z[ 1 ] / rhs.m_Z[ 1 ]; result.m_Z[ 2 ] = m_Z[ 2 ] / rhs.m_Z[ 2 ]; result.m_Z[ 3 ] = m_Z[ 3 ] / rhs.m_Z[ 3 ];
	result.m_W[ 0 ] = m_W[ 0 ] / rhs.m_W[ 0 ]; result.m_W[ 1 ] = m_W[ 1 ] / rhs.m_W[ 1 ]; result.m_W[ 2 ] = m_W[ 2 ] / rhs.m_W[ 2 ]; result.m_W[ 3 ] = m_W[ 3 ] / rhs.m_W[ 3 ];
	return result;
}

SNML_INLINE quadquad quadquad::operator +( scalar_arg rhs ) const
{
	quadquad result;
	result.m_X[ 0 ] = m_X[ 0 ] + rhs.m_Value; result.m_X[ 1 ] = m_X[ 1 ] + rhs.m_Value; result.m_X[ 2 ] = m_X[ 2 ] + rhs.m_Value; result.m_X[ 3 ] = m_X[ 3 ] + rhs.m_Value;
	result.m_Y[ 0 ] = m_Y[ 0 ] + rhs.m_Value; result.m_Y[ 1 ] = m_Y[ 1 ] + rhs.m_Value; result.m_Y[ 2 ] = m_Y[ 2 ] + rhs.m_Value; result.m_Y[ 3 ] = m_Y[ 3 ] + rhs.m_Value;
	result.m_Z[ 0 ] = m_Z[ 0 ] + rhs.m_Value; result.m_Z[ 1 ] = m_Z[ 1 ] + rhs.m_Value; result.m_Z[ 2 ] = m_Z[ 2 ] + rhs.m_Value; result.m_Z[ 3 ] = m_Z[ 3 ] + rhs.m_Value;
	result.m_W[ 0 ] = m_W[ 0 ] + rhs.m_Value; result.m_W[ 1 ] = m_W[ 1 ] + rhs.m_Value; result.m_W[ 2 ] = m_W[ 2 ] + rhs.m_Value; result.m_W[ 3 ] = m_W[ 3 ] + rhs.m_Value;
	return result;
}

SNML_INLINE quadquad quadquad::operator -( scalar_arg rhs ) const
{
	quadquad result;
	result.m_X[ 0 ] = m_X[ 0 ] - rhs.m_Value; result.m_X[ 1 ] = m_X[ 1 ] - rhs.m_Value; result.m_X[ 2 ] = m_X[ 2 ] - rhs.m_Value; result.m_X[ 3 ] = m_X[ 3 ] - rhs.m_Value;
	result.m_Y[ 0 ] = m_Y[ 0 ] - rhs.m_Value; result.m_Y[ 1 ] = m_Y[ 1 ] - rhs.m_Value; result.m_Y[ 2 ] = m_Y[ 2 ] - rhs.m_Value; result.m_Y[ 3 ] = m_Y[ 3 ] - rhs.m_Value;
	result.m_Z[ 0 ] = m_Z[ 0 ] - rhs.m_Value; result.m_Z[ 1 ] = m_Z[ 1 ] - rhs.m_Value; result.m_Z[ 2 ] = m_Z[ 2 ] - rhs.m_Value; result.m_Z[ 3 ] = m_Z[ 3 ] - rhs.m_Value;
	result.m_W[ 0 ] = m_W[ 0 ] - rhs.m_Value; result.m_W[ 1 ] = m_W[ 1 ] - rhs.m_Value; result.m_W[ 2 ] = m_W[ 2 ] - rhs.m_Value; result.m_W[ 3 ] = m_W[ 3 ] - rhs.m_Value;
	return result;
}

SNML_INLINE quadquad quadquad::operator *( scalar_arg rhs ) const
{
	quadquad result;
	result.m_X[ 0 ] = m_X[ 0 ] * rhs.m_Value; result.m_X[ 1 ] = m_X[ 1 ] * rhs.m_Value; result.m_X[ 2 ] = m_X[ 2 ] * rhs.m_Value; result.m_X[ 3 ] = m_X[ 3 ] * rhs.m_Value;
	result.m_Y[ 0 ] = m_Y[ 0 ] * rhs.m_Value; result.m_Y[ 1 ] = m_Y[ 1 ] * rhs.m_Value; result.m_Y[ 2 ] = m_Y[ 2 ] * rhs.m_Value; result.m_Y[ 3 ] = m_Y[ 3 ] * rhs.m_Value;
	result.m_Z[ 0 ] = m_Z[ 0 ] * rhs.m_Value; result.m_Z[ 1 ] = m_Z[ 1 ] * rhs.m_Value; result.m_Z[ 2 ] = m_Z[ 2 ] * rhs.m_Value; result.m_Z[ 3 ] = m_Z[ 3 ] * rhs.m_Value;
	result.m_W[ 0 ] = m_W[ 0 ] * rhs.m_Value; result.m_W[ 1 ] = m_W[ 1 ] * rhs.m_Value; result.m_W[ 2 ] = m_W[ 2 ] * rhs.m_Value; result.m_W[ 3 ] = m_W[ 3 ] * rhs.m_Value;
	return result;
}

SNML_INLINE quadquad quadquad::operator /( scalar_arg rhs ) const
{
	quadquad result;
	result.m_X[ 0 ] = m_X[ 0 ] / rhs.m_Value; result.m_X[ 1 ] = m_X[ 1 ] / rhs.m_Value; result.m_X[ 2 ] = m_X[ 2 ] / rhs.m_Value; result.m_X[ 3 ] = m_X[ 3 ] / rhs.m_Value;
	result.m_Y[ 0 ] = m_Y[ 0 ] / rhs.m_Value; result.m_Y[ 1 ] = m_Y[ 1 ] / rhs.m_Value; result.m_Y[ 2 ] = m_Y[ 2 ] / rhs.m_Value; result.m_Y[ 3 ] = m_Y[ 3 ] / rhs.m_Value;
	result.m_Z[ 0 ] = m_Z[ 0 ] / rhs.m_Value; result.m_Z[ 1 ] = m_Z[ 1 ] / rhs.m_Value; result.m_Z[ 2 ] = m_Z[ 2 ] / rhs.m_Value; result.m_Z[ 3 ] = m_Z[ 3 ] / rhs.m_Value;
	result.m_W[ 0 ] = m_W[ 0 ] / rhs.m_Value; result.m_W[ 1 ] = m_W[ 1 ] / rhs.m_Value; result.m_W[ 2 ] = m_W[ 2 ] / rhs.m_Value; result.m_W[ 3 ] = m_W[ 3 ] / rhs.m_Value;
	return result;
}

SNML_INLINE void quadquad::operator +=( quadquad_arg rhs )
{
	m_X[ 0 ] = m_X[ 0 ] + rhs.m_X[ 0 ]; m_X[ 1 ] = m_X[ 1 ] + rhs.m_X[ 1 ]; m_X[ 2 ] = m_X[ 2 ] + rhs.m_X[ 2 ]; m_X[ 3 ] = m_X[ 3 ] + rhs.m_X[ 3 ];
	m_Y[ 0 ] = m_Y[ 0 ] + rhs.m_Y[ 0 ]; m_Y[ 1 ] = m_Y[ 1 ] + rhs.m_Y[ 1 ]; m_Y[ 2 ] = m_Y[ 2 ] + rhs.m_Y[ 2 ]; m_Y[ 3 ] = m_Y[ 3 ] + rhs.m_Y[ 3 ];
	m_Z[ 0 ] = m_Z[ 0 ] + rhs.m_Z[ 0 ]; m_Z[ 1 ] = m_Z[ 1 ] + rhs.m_Z[ 1 ]; m_Z[ 2 ] = m_Z[ 2 ] + rhs.m_Z[ 2 ]; m_Z[ 3 ] = m_Z[ 3 ] + rhs.m_Z[ 3 ];
	m_W[ 0 ] = m_W[ 0 ] + rhs.m_W[ 0 ]; m_W[ 1 ] = m_W[ 1 ] + rhs.m_W[ 1 ]; m_W[ 2 ] = m_W[ 2 ] + rhs.m_W[ 2 ]; m_W[ 3 ] = m_W[ 3 ] + rhs.m_W[ 3 ];
}

SNML_INLINE void quadquad::operator -=( quadquad_arg rhs )
{
	m_X[ 0 ] = m_X[ 0 ] - rhs.m_X[ 0 ]; m_X[ 1 ] = m_X[ 1 ] - rhs.m_X[ 1 ]; m_X[ 2 ] = m_X[ 2 ] - rhs.m_X[ 2 ]; m_X[ 3 ] = m_X[ 3 ] - rhs.m_X[ 3 ];
	m_Y[ 0 ] = m_Y[ 0 ] - rhs.m_Y[ 0 ]; m_Y[ 1 ] = m_Y[ 1 ] - rhs.m_Y[ 1 ]; m_Y[ 2 ] = m_Y[ 2 ] - rhs.m_Y[ 2 ]; m_Y[ 3 ] = m_Y[ 3 ] - rhs.m_Y[ 3 ];
	m_Z[ 0 ] = m_Z[ 0 ] - rhs.m_Z[ 0 ]; m_Z[ 1 ] = m_Z[ 1 ] - rhs.m_Z[ 1 ]; m_Z[ 2 ] = m_Z[ 2 ] - rhs.m_Z[ 2 ]; m_Z[ 3 ] = m_Z[ 3 ] - rhs.m_Z[ 3 ];
	m_W[ 0 ] = m_W[ 0 ] - rhs.m_W[ 0 ]; m_W[ 1 ] = m_W[ 1 ] - rhs.m_W[ 1 ]; m_W[ 2 ] = m_W[ 2 ] - rhs.m_W[ 2 ]; m_W[ 3 ] = m_W[ 3 ] - rhs.m_W[ 3 ];
}

SNML_INLINE void quadquad::operator *=( quadquad_arg rhs )
{
	m_X[ 0 ] = m_X[ 0 ] * rhs.m_X[ 0 ]; m_X[ 1 ] = m_X[ 1 ] * rhs.m_X[ 1 ]; m_X[ 2 ] = m_X[ 2 ] * rhs.m_X[ 2 ]; m_X[ 3 ] = m_X[ 3 ] * rhs.m_X[ 3 ];
	m_Y[ 0 ] = m_Y[ 0 ] * rhs.m_Y[ 0 ]; m_Y[ 1 ] = m_Y[ 1 ] * rhs.m_Y[ 1 ]; m_Y[ 2 ] = m_Y[ 2 ] * rhs.m_Y[ 2 ]; m_Y[ 3 ] = m_Y[ 3 ] * rhs.m_Y[ 3 ];
	m_Z[ 0 ] = m_Z[ 0 ] * rhs.m_Z[ 0 ]; m_Z[ 1 ] = m_Z[ 1 ] * rhs.m_Z[ 1 ]; m_Z[ 2 ] = m_Z[ 2 ] * rhs.m_Z[ 2 ]; m_Z[ 3 ] = m_Z[ 3 ] * rhs.m_Z[ 3 ];
	m_W[ 0 ] = m_W[ 0 ] * rhs.m_W[ 0 ]; m_W[ 1 ] = m_W[ 1 ] * rhs.m_W[ 1 ]; m_W[ 2 ] = m_W[ 2 ] * rhs.m_W[ 2 ]; m_W[ 3 ] = m_W[ 3 ] * rhs.m_W[ 3 ];
}

SNML_INLINE void quadquad::operator /=( quadquad_arg rhs )
{
	m_X[ 0 ] = m_X[ 0 ] / rhs.m_X[ 0 ]; m_X[ 1 ] = m_X[ 1 ] / rhs.m_X[ 1 ]; m_X[ 2 ] = m_X[ 2 ] / rhs.m_X[ 2 ]; m_X[ 3 ] = m_X[ 3 ] / rhs.m_X[ 3 ];
	m_Y[ 0 ] = m_Y[ 0 ] / rhs.m_Y[ 0 ]; m_Y[ 1 ] = m_Y[ 1 ] / rhs.m_Y[ 1 ]; m_Y[ 2 ] = m_Y[ 2 ] / rhs.m_Y[ 2 ]; m_Y[ 3 ] = m_Y[ 3 ] / rhs.m_Y[ 3 ];
	m_Z[ 0 ] = m_Z[ 0 ] / rhs.m_Z[ 0 ]; m_Z[ 1 ] = m_Z[ 1 ] / rhs.m_Z[ 1 ]; m_Z[ 2 ] = m_Z[ 2 ] / rhs.m_Z[ 2 ]; m_Z[ 3 ] = m_Z[ 3 ] / rhs.m_Z[ 3 ];
	m_W[ 0 ] = m_W[ 0 ] / rhs.m_W[ 0 ]; m_W[ 1 ] = m_W[ 1 ] / rhs.m_W[ 1 ]; m_W[ 2 ] = m_W[ 2 ] / rhs.m_W[ 2 ]; m_W[ 3 ] = m_W[ 3 ] / rhs.m_W[ 3 ];
}

SNML_INLINE void quadquad::operator +=( scalar_arg rhs )
{
	m_X[ 0 ] = m_X[ 0 ] + rhs.m_Value; m_X[ 1 ] = m_X[ 1 ] + rhs.m_Value; m_X[ 2 ] = m_X[ 2 ] + rhs.m_Value; m_X[ 3 ] = m_X[ 3 ] + rhs.m_Value;
	m_Y[ 0 ] = m_Y[ 0 ] + rhs.m_Value; m_Y[ 1 ] = m_Y[ 1 ] + rhs.m_Value; m_Y[ 2 ] = m_Y[ 2 ] + rhs.m_Value; m_Y[ 3 ] = m_Y[ 3 ] + rhs.m_Value;
	m_Z[ 0 ] = m_Z[ 0 ] + rhs.m_Value; m_Z[ 1 ] = m_Z[ 1 ] + rhs.m_Value; m_Z[ 2 ] = m_Z[ 2 ] + rhs.m_Value; m_Z[ 3 ] = m_Z[ 3 ] + rhs.m_Value;
	m_W[ 0 ] = m_W[ 0 ] + rhs.m_Value; m_W[ 1 ] = m_W[ 1 ] + rhs.m_Value; m_W[ 2 ] = m_W[ 2 ] + rhs.m_Value; m_W[ 3 ] = m_W[ 3 ] + rhs.m_Value;
}

SNML_INLINE void quadquad::operator -=( scalar_arg rhs )
{
	m_X[ 0 ] = m_X[ 0 ] - rhs.m_Value; m_X[ 1 ] = m_X[ 1 ] - rhs.m_Value; m_X[ 2 ] = m_X[ 2 ] - rhs.m_Value; m_X[ 3 ] = m_X[ 3 ] - rhs.m_Value;
	m_Y[ 0 ] = m_Y[ 0 ] - rhs.m_Value; m_Y[ 1 ] = m_Y[ 1 ] - rhs.m_Value; m_Y[ 2 ] = m_Y[ 2 ] - rhs.m_Value; m_Y[ 3 ] = m_Y[ 3 ] - rhs.m_Value;
	m_Z[ 0 ] = m_Z[ 0 ] - rhs.m_Value; m_Z[ 1 ] = m_Z[ 1 ] - rhs.m_Value; m_Z[ 2 ] = m_Z[ 2 ] - rhs.m_Value; m_Z[ 3 ] = m_Z[ 3 ] - rhs.m_Value;
	m_W[ 0 ] = m_W[ 0 ] - rhs.m_Value; m_W[ 1 ] = m_W[ 1 ] - rhs.m_Value; m_W[ 2 ] = m_W[ 2 ] - rhs.m_Value; m_W[ 3 ] = m_W[ 3 ] - rhs.m_Value;
}

SNML_INLINE void quadquad::operator *=( scalar_arg rhs )
{
	m_X[ 0 ] = m_X[ 0 ] * rhs.m_Value; m_X[ 1 ] = m_X[ 1 ] * rhs.m_Value; m_X[ 2 ] = m_X[ 2 ] * rhs.m_Value; m_X[ 3 ] = m_X[ 3 ] * rhs.m_Value;
	m_Y[ 0 ] = m_Y[ 0 ] * rhs.m_Value; m_Y[ 1 ] = m_Y[ 1 ] * rhs.m_Value; m_Y[ 2 ] = m_Y[ 2 ] * rhs.m_Value; m_Y[ 3 ] = m_Y[ 3 ] * rhs.m_Value;
	m_Z[ 0 ] = m_Z[ 0 ] * rhs.m_Value; m_Z[ 1 ] = m_Z[ 1 ] * rhs.m_Value; m_Z[ 2 ] = m_Z[ 2 ] * rhs.m_Value; m_Z[ 3 ] = m_Z[ 3 ] * rhs.m_Value;
	m_W[ 0 ] = m_W[ 0 ] * rhs.m_Value; m_W[ 1 ] = m_W[ 1 ] * rhs.m_Value; m_W[ 2 ] = m_W[ 2 ] * rhs.m_Value; m_W[ 3 ] = m_W[ 3 ] * rhs.m_Value;
}

SNML_INLINE void quadquad::operator /=( scalar_arg rhs )
{
	m_X[ 0 ] = m_X[ 0 ] / rhs.m_Value; m_X[ 1 ] = m_X[ 1 ] / rhs.m_Value; m_X[ 2 ] = m_X[ 2 ] / rhs.m_Value; m_X[ 3 ] = m_X[ 3 ] / rhs.m_Value;
	m_Y[ 0 ] = m_Y[ 0 ] / rhs.m_Value; m_Y[ 1 ] = m_Y[ 1 ] / rhs.m_Value; m_Y[ 2 ] = m_Y[ 2 ] / rhs.m_Value; m_Y[ 3 ] = m_Y[ 3 ] / rhs.m_Value;
	m_Z[ 0 ] = m_Z[ 0 ] / rhs.m_Value; m_Z[ 1 ] = m_Z[ 1 ] / rhs.m_Value; m_Z[ 2 ] = m_Z[ 2 ] / rhs.m_Value; m_Z[ 3 ] = m_Z[ 3 ] / rhs.m_Value;
	m_W[ 0 ] = m_W[ 0 ] / rhs.m_Value; m_W[ 1 ] = m_W[ 1 ] / rhs.m_Value; m_W[ 2 ] = m_W[ 2 ] / rhs.m_Value; m_W[ 3 ] = m_W[ 3 ] / rhs.m_Value;
}

SNML_INLINE quadquad quadquad::operator -() const
{
	quadquad result;
	result.m_X[ 0 ] = -m_X[ 0 ]; result.m_X[ 1 ] = -m_X[ 1 ]; result.m_X[ 2 ] = -m_X[ 2 ]; result.m_X[ 3 ] = -m_X[ 3 ];
	result.m_Y[ 0 ] = -m_Y[ 0 ]; result.m_Y[ 1 ] = -m_Y[ 1 ]; result.m_Y[ 2 ] = -m_Y[ 2 ]; result.m_Y[ 3 ] = -m_Y[ 3 ];
	result.m_Z[ 0 ] = -m_Z[ 0 ]; result.m_Z[ 1 ] = -m_Z[ 1 ]; result.m_Z[ 2 ] = -m_Z[ 2 ]; result.m_Z[ 3 ] = -m_Z[ 3 ];
	result.m_W[ 0 ] = -m_W[ 0 ]; result.m_W[ 1 ] = -m_W[ 1 ]; result.m_W[ 2 ] = -m_W[ 2 ]; result.m_W[ 3 ] = -m_W[ 3 ];
	return result;
}

SNML_INLINE quadquad quadquad::operator +() const
{
	return *this;
}

SNML_INLINE bool quadquad::operator ==( quadquad_arg rhs ) const
{
	return
		m_X[ 0 ] == rhs.m_X[ 0 ] && m_X[ 1 ] == rhs.m_X[ 1 ] && m_X[ 2 ] == rhs.m_X[ 2 ] && m_X[ 3 ] == rhs.m_X[ 3 ] &&
		m_Y[ 0 ] == rhs.m_Y[ 0 ] && m_Y[ 1 ] == rhs.m_Y[ 1 ] && m_Y[ 2 ] == rhs.m_Y[ 2 ] && m_Y[ 3 ] == rhs.m_Y[ 3 ] &&
		m_Z[ 0 ] == rhs.m_Z[ 0 ] && m_Z[ 1 ] == rhs.m_Z[ 1 ] && m_Z[ 2 ] == rhs.m_Z[ 2 ] && m_Z[ 3 ] == rhs.m_Z[ 3 ] &&
		m_W[ 0 ] == rhs.m_W[ 0 ] && m_W[ 1 ] == rhs.m_W[ 1 ] && m_W[ 2 ] == rhs.m_W[ 2 ] && m_W[ 3 ] == rhs.m_W[ 3 ]
	;
}

SNML_INLINE bool quadquad::operator !=( quadquad_arg rhs ) const
{
	return
		m_X[ 0 ] != rhs.m_X[ 0 ] || m_X[ 1 ] != rhs.m_X[ 1 ] || m_X[ 2 ] != rhs.m_X[ 2 ] || m_X[ 3 ] != rhs.m_X[ 3 ] ||
		m_Y[ 0 ] != rhs.m_Y[ 0 ] || m_Y[ 1 ] != rhs.m_Y[ 1 ] || m_Y[ 2 ] != rhs.m_Y[ 2 ] || m_Y[ 3 ] != rhs.m_Y[ 3 ] ||
		m_Z[ 0 ] != rhs.m_Z[ 0 ] || m_Z[ 1 ] != rhs.m_Z[ 1 ] || m_Z[ 2 ] != rhs.m_Z[ 2 ] || m_Z[ 3 ] != rhs.m_Z[ 3 ] ||
		m_W[ 0 ] != rhs.m_W[ 0 ] || m_W[ 1 ] != rhs.m_W[ 1 ] || m_W[ 2 ] != rhs.m_W[ 2 ] || m_W[ 3 ] != rhs.m_W[ 3 ]
	;
}

SNML_INLINE bool any_ge( quadquad_arg x, quadquad_arg y )
{
	return
		x.m_X[ 0 ] >= y.m_X[ 0 ] || x.m_X[ 1 ] >= y.m_X[ 1 ] || x.m_X[ 2 ] >= y.m_X[ 2 ] || x.m_X[ 3 ] >= y.m_X[ 3 ] ||
		x.m_Y[ 0 ] >= y.m_Y[ 0 ] || x.m_Y[ 1 ] >= y.m_Y[ 1 ] || x.m_Y[ 2 ] >= y.m_Y[ 2 ] || x.m_Y[ 3 ] >= y.m_Y[ 3 ] ||
		x.m_Z[ 0 ] >= y.m_Z[ 0 ] || x.m_Z[ 1 ] >= y.m_Z[ 1 ] || x.m_Z[ 2 ] >= y.m_Z[ 2 ] || x.m_Z[ 3 ] >= y.m_Z[ 3 ] ||
		x.m_W[ 0 ] >= y.m_W[ 0 ] || x.m_W[ 1 ] >= y.m_W[ 1 ] || x.m_W[ 2 ] >= y.m_W[ 2 ] || x.m_W[ 3 ] >= y.m_X[ 3 ]
	;
}

SNML_INLINE bool any_gt( quadquad_arg x, quadquad_arg y )
{
	return
		x.m_X[ 0 ] > y.m_X[ 0 ] || x.m_X[ 1 ] > y.m_X[ 1 ] || x.m_X[ 2 ] > y.m_X[ 2 ] || x.m_X[ 3 ] > y.m_X[ 3 ] ||
		x.m_Y[ 0 ] > y.m_Y[ 0 ] || x.m_Y[ 1 ] > y.m_Y[ 1 ] || x.m_Y[ 2 ] > y.m_Y[ 2 ] || x.m_Y[ 3 ] > y.m_Y[ 3 ] ||
		x.m_Z[ 0 ] > y.m_Z[ 0 ] || x.m_Z[ 1 ] > y.m_Z[ 1 ] || x.m_Z[ 2 ] > y.m_Z[ 2 ] || x.m_Z[ 3 ] > y.m_Z[ 3 ] ||
		x.m_W[ 0 ] > y.m_W[ 0 ] || x.m_W[ 1 ] > y.m_W[ 1 ] || x.m_W[ 2 ] > y.m_W[ 2 ] || x.m_W[ 3 ] > y.m_W[ 3 ]
	;
}

SNML_INLINE bool any_le( quadquad_arg x, quadquad_arg y )
{
	return
		x.m_X[ 0 ] <= y.m_X[ 0 ] || x.m_X[ 1 ] <= y.m_X[ 1 ] || x.m_X[ 2 ] <= y.m_X[ 2 ] || x.m_X[ 3 ] <= y.m_X[ 3 ] ||
		x.m_Y[ 0 ] <= y.m_Y[ 0 ] || x.m_Y[ 1 ] <= y.m_Y[ 1 ] || x.m_Y[ 2 ] <= y.m_Y[ 2 ] || x.m_Y[ 3 ] <= y.m_Y[ 3 ] ||
		x.m_Z[ 0 ] <= y.m_Z[ 0 ] || x.m_Z[ 1 ] <= y.m_Z[ 1 ] || x.m_Z[ 2 ] <= y.m_Z[ 2 ] || x.m_Z[ 3 ] <= y.m_Z[ 3 ] ||
		x.m_W[ 0 ] <= y.m_W[ 0 ] || x.m_W[ 1 ] <= y.m_W[ 1 ] || x.m_W[ 2 ] <= y.m_W[ 2 ] || x.m_W[ 3 ] <= y.m_W[ 3 ]
	;
}

SNML_INLINE bool any_lt( quadquad_arg x, quadquad_arg y )
{
	return
		x.m_X[ 0 ] < y.m_X[ 0 ] || x.m_X[ 1 ] < y.m_X[ 1 ] || x.m_X[ 2 ] < y.m_X[ 2 ] || x.m_X[ 3 ] < y.m_X[ 3 ] ||
		x.m_Y[ 0 ] < y.m_Y[ 0 ] || x.m_Y[ 1 ] < y.m_Y[ 1 ] || x.m_Y[ 2 ] < y.m_Y[ 2 ] || x.m_Y[ 3 ] < y.m_Y[ 3 ] ||
		x.m_Z[ 0 ] < y.m_Z[ 0 ] || x.m_Z[ 1 ] < y.m_Z[ 1 ] || x.m_Z[ 2 ] < y.m_Z[ 2 ] || x.m_Z[ 3 ] < y.m_Z[ 3 ] ||
		x.m_W[ 0 ] < y.m_W[ 0 ] || x.m_W[ 1 ] < y.m_W[ 1 ] || x.m_W[ 2 ] < y.m_W[ 2 ] || x.m_W[ 3 ] < y.m_W[ 3 ]
	;
}

SNML_INLINE bool any_eq( quadquad_arg x, quadquad_arg y )
{
	return
		x.m_X[ 0 ] == y.m_X[ 0 ] || x.m_X[ 1 ] == y.m_X[ 1 ] || x.m_X[ 2 ] == y.m_X[ 2 ] || x.m_X[ 3 ] == y.m_X[ 3 ] ||
		x.m_Y[ 0 ] == y.m_Y[ 0 ] || x.m_Y[ 1 ] == y.m_Y[ 1 ] || x.m_Y[ 2 ] == y.m_Y[ 2 ] || x.m_Y[ 3 ] == y.m_Y[ 3 ] ||
		x.m_Z[ 0 ] == y.m_Z[ 0 ] || x.m_Z[ 1 ] == y.m_Z[ 1 ] || x.m_Z[ 2 ] == y.m_Z[ 2 ] || x.m_Z[ 3 ] == y.m_Z[ 3 ] ||
		x.m_W[ 0 ] == y.m_W[ 0 ] || x.m_W[ 1 ] == y.m_W[ 1 ] || x.m_W[ 2 ] == y.m_W[ 2 ] || x.m_W[ 3 ] == y.m_W[ 3 ]
	;
}

SNML_INLINE bool any_ne( quadquad_arg x, quadquad_arg y )
{
	return
		x.m_X[ 0 ] != y.m_X[ 0 ] || x.m_X[ 1 ] != y.m_X[ 1 ] || x.m_X[ 2 ] != y.m_X[ 2 ] || x.m_X[ 3 ] != y.m_X[ 3 ] ||
		x.m_Y[ 0 ] != y.m_Y[ 0 ] || x.m_Y[ 1 ] != y.m_Y[ 1 ] || x.m_Y[ 2 ] != y.m_Y[ 2 ] || x.m_Y[ 3 ] != y.m_Y[ 3 ] ||
		x.m_Z[ 0 ] != y.m_Z[ 0 ] || x.m_Z[ 1 ] != y.m_Z[ 1 ] || x.m_Z[ 2 ] != y.m_Z[ 2 ] || x.m_Z[ 3 ] != y.m_Z[ 3 ] ||
		x.m_W[ 0 ] != y.m_W[ 0 ] || x.m_W[ 1 ] != y.m_W[ 1 ] || x.m_W[ 2 ] != y.m_W[ 2 ] || x.m_W[ 3 ] != y.m_W[ 3 ]
	;
}

SNML_INLINE bool all_ge( quadquad_arg x, quadquad_arg y )
{
	return
		x.m_X[ 0 ] >= y.m_X[ 0 ] && x.m_X[ 1 ] >= y.m_X[ 1 ] && x.m_X[ 2 ] >= y.m_X[ 2 ] && x.m_X[ 3 ] >= y.m_X[ 3 ] &&
		x.m_Y[ 0 ] >= y.m_Y[ 0 ] && x.m_Y[ 1 ] >= y.m_Y[ 1 ] && x.m_Y[ 2 ] >= y.m_Y[ 2 ] && x.m_Y[ 3 ] >= y.m_Y[ 3 ] &&
		x.m_Z[ 0 ] >= y.m_Z[ 0 ] && x.m_Z[ 1 ] >= y.m_Z[ 1 ] && x.m_Z[ 2 ] >= y.m_Z[ 2 ] && x.m_Z[ 3 ] >= y.m_Z[ 3 ] &&
		x.m_W[ 0 ] >= y.m_W[ 0 ] && x.m_W[ 1 ] >= y.m_W[ 1 ] && x.m_W[ 2 ] >= y.m_W[ 2 ] && x.m_W[ 3 ] >= y.m_W[ 3 ]
	;
}

SNML_INLINE bool all_gt( quadquad_arg x, quadquad_arg y )
{
	return
		x.m_X[ 0 ] > y.m_X[ 0 ] && x.m_X[ 1 ] > y.m_X[ 1 ] && x.m_X[ 2 ] > y.m_X[ 2 ] && x.m_X[ 3 ] > y.m_X[ 3 ] &&
		x.m_Y[ 0 ] > y.m_Y[ 0 ] && x.m_Y[ 1 ] > y.m_Y[ 1 ] && x.m_Y[ 2 ] > y.m_Y[ 2 ] && x.m_Y[ 3 ] > y.m_Y[ 3 ] &&
		x.m_Z[ 0 ] > y.m_Z[ 0 ] && x.m_Z[ 1 ] > y.m_Z[ 1 ] && x.m_Z[ 2 ] > y.m_Z[ 2 ] && x.m_Z[ 3 ] > y.m_Z[ 3 ] &&
		x.m_W[ 0 ] > y.m_W[ 0 ] && x.m_W[ 1 ] > y.m_W[ 1 ] && x.m_W[ 2 ] > y.m_W[ 2 ] && x.m_W[ 3 ] > y.m_W[ 3 ]
	;
}

SNML_INLINE bool all_le( quadquad_arg x, quadquad_arg y )
{
	return
		x.m_X[ 0 ] <= y.m_X[ 0 ] && x.m_X[ 1 ] <= y.m_X[ 1 ] && x.m_X[ 2 ] <= y.m_X[ 2 ] && x.m_X[ 3 ] <= y.m_X[ 3 ] &&
		x.m_Y[ 0 ] <= y.m_Y[ 0 ] && x.m_Y[ 1 ] <= y.m_Y[ 1 ] && x.m_Y[ 2 ] <= y.m_Y[ 2 ] && x.m_Y[ 3 ] <= y.m_Y[ 3 ] &&
		x.m_Z[ 0 ] <= y.m_Z[ 0 ] && x.m_Z[ 1 ] <= y.m_Z[ 1 ] && x.m_Z[ 2 ] <= y.m_Z[ 2 ] && x.m_Z[ 3 ] <= y.m_Z[ 3 ] &&
		x.m_W[ 0 ] <= y.m_W[ 0 ] && x.m_W[ 1 ] <= y.m_W[ 1 ] && x.m_W[ 2 ] <= y.m_W[ 2 ] && x.m_W[ 3 ] <= y.m_W[ 3 ]
	;
}

SNML_INLINE bool all_lt( quadquad_arg x, quadquad_arg y )
{
	return
		x.m_X[ 0 ] < y.m_X[ 0 ] && x.m_X[ 1 ] < y.m_X[ 1 ] && x.m_X[ 2 ] < y.m_X[ 2 ] && x.m_X[ 3 ] < y.m_X[ 3 ] &&
		x.m_Y[ 0 ] < y.m_Y[ 0 ] && x.m_Y[ 1 ] < y.m_Y[ 1 ] && x.m_Y[ 2 ] < y.m_Y[ 2 ] && x.m_Y[ 3 ] < y.m_Y[ 3 ] &&
		x.m_Z[ 0 ] < y.m_Z[ 0 ] && x.m_Z[ 1 ] < y.m_Z[ 1 ] && x.m_Z[ 2 ] < y.m_Z[ 2 ] && x.m_Z[ 3 ] < y.m_Z[ 3 ] &&
		x.m_W[ 0 ] < y.m_W[ 0 ] && x.m_W[ 1 ] < y.m_W[ 1 ] && x.m_W[ 2 ] < y.m_W[ 2 ] && x.m_W[ 3 ] < y.m_W[ 3 ]
	;
}

SNML_INLINE bool all_eq( quadquad_arg x, quadquad_arg y )
{
	return
		x.m_X[ 0 ] == y.m_X[ 0 ] && x.m_X[ 1 ] == y.m_X[ 1 ] && x.m_X[ 2 ] == y.m_X[ 2 ] && x.m_X[ 3 ] == y.m_X[ 3 ] &&
		x.m_Y[ 0 ] == y.m_Y[ 0 ] && x.m_Y[ 1 ] == y.m_Y[ 1 ] && x.m_Y[ 2 ] == y.m_Y[ 2 ] && x.m_Y[ 3 ] == y.m_Y[ 3 ] &&
		x.m_Z[ 0 ] == y.m_Z[ 0 ] && x.m_Z[ 1 ] == y.m_Z[ 1 ] && x.m_Z[ 2 ] == y.m_Z[ 2 ] && x.m_Z[ 3 ] == y.m_Z[ 3 ] &&
		x.m_W[ 0 ] == y.m_W[ 0 ] && x.m_W[ 1 ] == y.m_W[ 1 ] && x.m_W[ 2 ] == y.m_W[ 2 ] && x.m_W[ 3 ] == y.m_W[ 3 ]
	;
}

SNML_INLINE bool all_ne( quadquad_arg x, quadquad_arg y )
{
	return
		x.m_X[ 0 ] != y.m_X[ 0 ] && x.m_X[ 1 ] != y.m_X[ 1 ] && x.m_X[ 2 ] != y.m_X[ 2 ] && x.m_X[ 3 ] != y.m_X[ 3 ] &&
		x.m_Y[ 0 ] != y.m_Y[ 0 ] && x.m_Y[ 1 ] != y.m_Y[ 1 ] && x.m_Y[ 2 ] != y.m_Y[ 2 ] && x.m_Y[ 3 ] != y.m_Y[ 3 ] &&
		x.m_Z[ 0 ] != y.m_Z[ 0 ] && x.m_Z[ 1 ] != y.m_Z[ 1 ] && x.m_Z[ 2 ] != y.m_Z[ 2 ] && x.m_Z[ 3 ] != y.m_Z[ 3 ] &&
		x.m_W[ 0 ] != y.m_W[ 0 ] && x.m_W[ 1 ] != y.m_W[ 1 ] && x.m_W[ 2 ] != y.m_W[ 2 ] && x.m_W[ 3 ] != y.m_W[ 3 ]
	;
}

SNML_INLINE quadquad quadquad::zero()
{
	return quadquad( 0.0f );
}

SNML_INLINE quadquad quadquad::one()
{
	return quadquad( 1.0f );
}

SNML_INLINE quadquad quadquad::two()
{
	return quadquad( 2.0f );
}

SNML_INLINE quadquad quadquad::three()
{
	return quadquad( 3.0f );
}

SNML_INLINE quadquad quadquad::four()
{
	return quadquad( 4.0f );
}

SNML_INLINE quadquad quadquad::five()
{
	return quadquad( 5.0f );
}

SNML_INLINE quadquad quadquad::six()
{
	return quadquad( 6.0f );
}

SNML_INLINE quadquad quadquad::seven()
{
	return quadquad( 7.0f );
}

SNML_INLINE quadquad quadquad::eight()
{
	return quadquad( 8.0f );
}

SNML_INLINE quadquad quadquad::nine()
{
	return quadquad( 9.0f );
}

SNML_INLINE quadquad quadquad::ten()
{
	return quadquad( 10 );
}

SNML_INLINE quadquad quadquad::half()
{
	return quadquad( 0.5f );
}

SNML_INLINE quadquad quadquad::quarter()
{
	return quadquad( 0.25f );
}

SNML_INLINE quadquad quadquad::three_quarters()
{
	return quadquad( 0.75f );
}

SNML_INLINE quadquad quadquad::three_by_2()
{
	return quadquad( 1.5f );
}

SNML_INLINE quadquad quadquad::huge()
{
	return quadquad( 340282346638528886604286022844204804240.0f );
}

SNML_INLINE quadquad quadquad::sqrt2()
{
	return quadquad( 1.41421356237309504880f );
}

SNML_INLINE quadquad quadquad::sqrt_half()
{
	return quadquad( 0.707106781186547524401f );
}

SNML_INLINE quadquad quadquad::two_by_sqrtpi()
{
	return quadquad( 1.12837916709551257390f );
}

SNML_INLINE quadquad quadquad::two_by_pi()
{
	return quadquad( 0.636619772367581343076f );
}

SNML_INLINE quadquad quadquad::one_by_pi()
{
	return quadquad( 0.318309886183790671538f );
}

SNML_INLINE quadquad quadquad::pi_by_4()
{
	return quadquad( 0.785398163397448309616f );
}

SNML_INLINE quadquad quadquad::pi_by_2()
{
	return quadquad( 1.57079632679489661923f );
}

SNML_INLINE quadquad quadquad::pi()
{
	return quadquad( 3.14159265358979323846f );
}

SNML_INLINE quadquad quadquad::e()
{
	return quadquad( 2.71828182845904523536f );
}

SNML_INLINE quadquad quadquad::log2e()
{
	return quadquad( 1.44269504088896340736f );
}

SNML_INLINE quadquad quadquad::log10e()
{
	return quadquad( 0.434294481903251827651f );
}

SNML_INLINE quadquad quadquad::ln2()
{
	return quadquad( 0.693147180559945309417f );
}

SNML_INLINE quadquad quadquad::ln10()
{
	return quadquad( 2.30258509299404568402f );
}

SNML_INLINE quadquad quadquad::two_pi()
{
	return quadquad( 3.14159265358979323846f*2.0f );
}

SNML_INLINE quadquad quadquad::pi_by_6()
{
	return quadquad( 3.14159265358979323846f/6 );
}

SNML_INLINE quadquad quadquad::log10two()
{
	return quadquad( .30102999566398119522f );
}

SNML_INLINE quadquad quadquad::log2ten()
{
	return quadquad( 3.3219280948873623478f );
}

SNML_INLINE quadquad quadquad::sqrt3_by_2()
{
	return quadquad( .86602540378443864675f );
}

/*SNML_INLINE scalar::scalar( float value_f )
{
}*/

SNML_INLINE scalar::operator float() const
{
	return m_Value;
}

SNML_INLINE int scalar::ToInt() const
{
	return (int)m_Value;
}

SNML_INLINE scalar sin( scalar_arg x )
{
	return sinf( x.m_Value );
}

SNML_INLINE scalar cos( scalar_arg x )
{
	return cosf( x.m_Value );
}

SNML_INLINE scalar tan( scalar_arg x )
{
	return tanf( x.m_Value );
}

SNML_INLINE scalar asin( scalar_arg x )
{
	return asinf( x.m_Value );
}

SNML_INLINE scalar acos( scalar_arg x )
{
	return acosf( x.m_Value );
}

SNML_INLINE scalar exp( scalar_arg x )
{
	return expf( x.m_Value );
}

SNML_INLINE scalar log( scalar_arg x )
{
	return logf( x.m_Value );
}

SNML_INLINE scalar pow( scalar_arg x, scalar_arg power )
{
	return powf( x.m_Value, power.m_Value );
}

SNML_INLINE scalar atan2( scalar_arg y, scalar_arg x )
{
	return atan2f( y.m_Value, x.m_Value );
}

SNML_INLINE scalar min( scalar_arg x, scalar_arg y )
{
	return x.m_Value < y.m_Value ? x.m_Value : y.m_Value;
}

SNML_INLINE scalar max( scalar_arg x, scalar_arg y )
{
	return x.m_Value > y.m_Value ? x.m_Value : y.m_Value;
}

SNML_INLINE scalar sgn( scalar_arg x )
{
	return x.m_Value > 0 ? 1.0f : x.m_Value < 0 ? -1.0f : 0;
}

SNML_INLINE scalar abs( scalar_arg x )
{
	return fabsf( x.m_Value );
}

SNML_INLINE scalar sqrt( scalar_arg x )
{
	return sqrtf( x.m_Value );
}

SNML_INLINE scalar rsqrt( scalar_arg x )
{
	return 1.0f / sqrtf( x.m_Value );
}

SNML_INLINE scalar recip( scalar_arg x )
{
	return 1.0f / x.m_Value;
}

SNML_INLINE scalar select_ge( scalar_arg x, scalar_arg y, scalar_arg t, scalar_arg f )
{
	return x.m_Value >= y.m_Value ? t : f;
}

SNML_INLINE scalar select_gt( scalar_arg x, scalar_arg y, scalar_arg t, scalar_arg f )
{
	return x.m_Value > y.m_Value ? t : f;
}

SNML_INLINE scalar select_le( scalar_arg x, scalar_arg y, scalar_arg t, scalar_arg f )
{
	return x.m_Value <= y.m_Value ? t : f;
}

SNML_INLINE scalar select_lt( scalar_arg x, scalar_arg y, scalar_arg t, scalar_arg f )
{
	return x.m_Value < y.m_Value ? t : f;
}

SNML_INLINE scalar select_eq( scalar_arg x, scalar_arg y, scalar_arg t, scalar_arg f )
{
	return x.m_Value == y.m_Value ? t : f;
}

SNML_INLINE scalar select_ne( scalar_arg x, scalar_arg y, scalar_arg t, scalar_arg f )
{
	return x.m_Value != y.m_Value ? t : f;
}

SNML_INLINE scalar select( bool sel, scalar_arg t, scalar_arg f )
{
	return sel ? t : f;
}

SNML_INLINE vquad::vquad( float value_f )
{
	m_Value[ 0 ] = m_Value[ 1 ] = m_Value[ 2 ] = m_Value[ 3 ] = value_f;
}

SNML_INLINE vquad::vquad( scalar_arg value_s )
{
	m_Value[ 0 ] = m_Value[ 1 ] = m_Value[ 2 ] = m_Value[ 3 ] = value_s.m_Value;
}

SNML_INLINE vquad::vquad( float x_f, float y_f )
{
	m_Value[ 0 ] = x_f; m_Value[ 1 ] = y_f;
}

SNML_INLINE vquad::vquad( scalar_arg x_s, scalar_arg y_s )
{
	m_Value[ 0 ] = x_s.m_Value; m_Value[ 1 ] = y_s.m_Value;
}

SNML_INLINE vquad::vquad( float x_f, float y_f, float z_f )
{
	m_Value[ 0 ] = x_f; m_Value[ 1 ] = y_f; m_Value[ 2 ] = z_f;
}

SNML_INLINE vquad::vquad( scalar_arg x_s, scalar_arg y_s, scalar_arg z_s )
{
	m_Value[ 0 ] = x_s.m_Value; m_Value[ 1 ] = y_s.m_Value; m_Value[ 2 ] = z_s.m_Value;
}

SNML_INLINE vquad::vquad( float x_f, float y_f, float z_f, float w_f )
{
	m_Value[ 0 ] = x_f; m_Value[ 1 ] = y_f; m_Value[ 2 ] = z_f; m_Value[ 3 ] = w_f;
}

SNML_INLINE vquad::vquad( scalar_arg x_s, scalar_arg y_s, scalar_arg z_s, scalar_arg w_s )
{
	m_Value[ 0 ] = x_s.m_Value; m_Value[ 1 ] = y_s.m_Value; m_Value[ 2 ] = z_s.m_Value; m_Value[ 3 ] = w_s.m_Value;
}

SNML_INLINE vquad::operator float() const
{
	return m_Value[ 0 ];
}

SNML_INLINE void vquad::load( const NativeScalar & x_s )
{
	m_Value[ 0 ] = m_Value[ 1 ] = m_Value[ 2 ] = m_Value[ 3 ] = x_s;
}

SNML_INLINE void vquad::load( const NativeScalar & x_s, const NativeScalar & y_s )
{
	m_Value[ 0 ] = x_s; m_Value[ 1 ] = y_s; m_Value[ 2 ] = m_Value[ 3 ] = 0;
}

SNML_INLINE void vquad::load( const NativeScalar & x_s, const NativeScalar & y_s, const NativeScalar & z_s )
{
	m_Value[ 0 ] = x_s; m_Value[ 1 ] = y_s; m_Value[ 2 ] = z_s; m_Value[ 3 ] = 0;
}

SNML_INLINE void vquad::load( const NativeScalar &x_s, const NativeScalar &y_s, const NativeScalar &z_s, const NativeScalar &w_s )
{
	m_Value[ 0 ] = x_s; m_Value[ 1 ] = y_s; m_Value[ 2 ] = z_s; m_Value[ 3 ] = w_s;
}

SNML_INLINE scalar vquad::x() const
{
	return m_Value[ 0 ];
}
		
SNML_INLINE scalar vquad::y() const
{
	return m_Value[ 1 ];
}
		
SNML_INLINE scalar vquad::z() const
{
	return m_Value[ 2 ];
}
		
SNML_INLINE scalar vquad::w() const
{
	return m_Value[ 3 ];
}

SNML_INLINE vquad conj( vquad_arg rhs )
{
	return vquad( -rhs.m_Value[ 0 ], -rhs.m_Value[ 1 ], -rhs.m_Value[ 2 ], rhs.m_Value[ 3 ] );
}

SNML_INLINE vquad zerow( vquad_arg rhs )
{
	return vquad( rhs.m_Value[ 0 ], rhs.m_Value[ 1 ], rhs.m_Value[ 2 ], 0 );
}

SNML_INLINE vquad onew( vquad_arg rhs )
{
	return vquad( rhs.m_Value[ 0 ], rhs.m_Value[ 1 ], rhs.m_Value[ 2 ], 1 );
}

SNML_INLINE vquad cross( vquad_arg lhs, vquad_arg rhs )
{
	return vquad( lhs.m_Value[ 1 ] * rhs.m_Value[ 2 ] - lhs.m_Value[ 2 ] * rhs.m_Value[ 1 ], lhs.m_Value[ 2 ] * rhs.m_Value[ 0 ] - lhs.m_Value[ 0 ] * rhs.m_Value[ 2 ], lhs.m_Value[ 0 ] * rhs.m_Value[ 1 ] - lhs.m_Value[ 1 ] * rhs.m_Value[ 0 ], 0 );
}

SNML_INLINE vquad axis_angle( vquad_arg axis, scalar_arg angle )
{
	float s = sinf( angle * scalar::half() ), c = cosf( angle * scalar::half() );
	return vquad( s * axis.m_Value[ 0 ], s * axis.m_Value[ 1 ], s * axis.m_Value[ 2 ], c );
}

SNML_INLINE vquad quat_mul( vquad_arg lhs, vquad_arg rhs )
{
	vquad result;
	QMul( result.m_Value, lhs.m_Value, rhs.m_Value );
	return result;
}

SNML_INLINE vquad quat_rotate( vquad_arg lhs, vquad_arg rhs )
{
	vquad result;
	float conj[] = { -lhs.m_Value[ 0 ], -lhs.m_Value[ 1 ], -lhs.m_Value[ 2 ], lhs.m_Value[ 3 ] };
	float tmp[4];
	QMul( tmp, lhs.m_Value, rhs.m_Value );
	QMul( result.m_Value, tmp, conj );
	return result;
}

SNML_INLINE vquad slerp( vquad_arg a, vquad_arg b, scalar_arg alpha )
{
	scalar dot = min( dot4( a, b ), 0.9999f );
	scalar angle = acos( dot );
	scalar scale = rsqrt( scalar::one() - dot * dot );
	return ( a * sin( ( scalar::one() - alpha ) * angle ) + b * sin( alpha * angle ) ) * scale;
}

SNML_INLINE vquad squad( vquad_arg a, vquad_arg b, vquad_arg c, vquad_arg d, scalar_arg alpha )
{
	return slerp( slerp( a, d, alpha ), slerp( b, c, alpha ), scalar::two() * alpha * ( scalar::one() - alpha ) );
}

SNML_INLINE scalar angle( vquad_arg lhs, vquad_arg rhs )
{
	return acosf( dot3( lhs, rhs ) / sqrt( length_squared3( lhs ) * length_squared3( rhs ) ) );
}

SNML_INLINE scalar dot2( vquad_arg lhs, vquad_arg rhs )
{
	return lhs.m_Value[ 0 ] * rhs.m_Value[ 0 ] + lhs.m_Value[ 1 ] * rhs.m_Value[ 1 ];
}

SNML_INLINE scalar dot3( vquad_arg lhs, vquad_arg rhs )
{
	return lhs.m_Value[ 0 ] * rhs.m_Value[ 0 ] + lhs.m_Value[ 1 ] * rhs.m_Value[ 1 ] + lhs.m_Value[ 2 ] * rhs.m_Value[ 2 ];
}

SNML_INLINE scalar dot4( vquad_arg lhs, vquad_arg rhs )
{
	return lhs.m_Value[ 0 ] * rhs.m_Value[ 0 ] + lhs.m_Value[ 1 ] * rhs.m_Value[ 1 ] + lhs.m_Value[ 2 ] * rhs.m_Value[ 2 ] + lhs.m_Value[ 3 ] * rhs.m_Value[ 3 ];
}

SNML_INLINE scalar sum2( vquad_arg rhs )
{
	return rhs.m_Value[ 0 ] + rhs.m_Value[ 1 ];
}

SNML_INLINE scalar sum3( vquad_arg rhs )
{
	return rhs.m_Value[ 0 ] + rhs.m_Value[ 1 ] + rhs.m_Value[ 2 ];
}

SNML_INLINE scalar sum4( vquad_arg rhs )
{
	return rhs.m_Value[ 0 ] + rhs.m_Value[ 1 ] + rhs.m_Value[ 2 ] + rhs.m_Value[ 3 ];
}

SNML_INLINE scalar sum_sideways( vquad_arg rhs )
{
	return rhs.m_Value[ 0 ] + rhs.m_Value[ 1 ] + rhs.m_Value[ 2 ] + rhs.m_Value[ 3 ];
}

SNML_INLINE scalar max_sideways( vquad_arg rhs )
{
	float max01 = rhs.m_Value[ 0 ] > rhs.m_Value[ 1 ] ? rhs.m_Value[ 0 ] : rhs.m_Value[ 1 ];
	float max23 = rhs.m_Value[ 2 ] > rhs.m_Value[ 3 ] ? rhs.m_Value[ 2 ] : rhs.m_Value[ 3 ];
	return max01 > max23 ? max01 : max23;
}

SNML_INLINE scalar min_sideways( vquad_arg rhs )
{
	float min01 = rhs.m_Value[ 0 ] < rhs.m_Value[ 1 ] ? rhs.m_Value[ 0 ] : rhs.m_Value[ 1 ];
	float min23 = rhs.m_Value[ 2 ] < rhs.m_Value[ 3 ] ? rhs.m_Value[ 2 ] : rhs.m_Value[ 3 ];
	return min01 < min23 ? min01 : min23;
}

SNML_INLINE vquad normal2( vquad_arg rhs )
{
	return rhs * rlength2( rhs );
}

SNML_INLINE vquad normal3( vquad_arg rhs )
{
	return rhs * rlength3( rhs );
}

SNML_INLINE vquad normal4( vquad_arg rhs )
{
	return rhs * rlength4( rhs );
}

SNML_INLINE scalar length2( vquad_arg rhs )
{
	return sqrtf( dot2( rhs, rhs ) );
}

SNML_INLINE scalar length3( vquad_arg rhs )
{
	return sqrtf( dot3( rhs, rhs ) );
}

SNML_INLINE scalar length4( vquad_arg rhs )
{
	return sqrtf( dot4( rhs, rhs ) );
}

SNML_INLINE scalar rlength2( vquad_arg rhs )
{
	return 1.0f / sqrtf( dot2( rhs, rhs ) );
}

SNML_INLINE scalar rlength3( vquad_arg rhs )
{
	return 1.0f / sqrtf( dot3( rhs, rhs ) );
}

SNML_INLINE scalar rlength4( vquad_arg rhs )
{
	return 1.0f / sqrtf( dot4( rhs, rhs ) );
}

SNML_INLINE scalar length_squared2( vquad_arg rhs )
{
	return rhs.m_Value[ 0 ] * rhs.m_Value[ 0 ] + rhs.m_Value[ 1 ] * rhs.m_Value[ 1 ];
}

SNML_INLINE scalar length_squared3( vquad_arg rhs )
{
	return rhs.m_Value[ 0 ] * rhs.m_Value[ 0 ] + rhs.m_Value[ 1 ] * rhs.m_Value[ 1 ] + rhs.m_Value[ 2 ] * rhs.m_Value[ 2 ];
}

SNML_INLINE scalar length_squared4( vquad_arg rhs )
{
	return rhs.m_Value[ 0 ] * rhs.m_Value[ 0 ] + rhs.m_Value[ 1 ] * rhs.m_Value[ 1 ] + rhs.m_Value[ 2 ] * rhs.m_Value[ 2 ] + rhs.m_Value[ 3 ] * rhs.m_Value[ 3 ];
}

SNML_INLINE vquad sin( vquad_arg x )
{
	return vquad( sinf( x.m_Value[ 0 ] ), sinf( x.m_Value[ 1 ] ), sinf( x.m_Value[ 2 ] ), sinf( x.m_Value[ 3 ] ) );
}

SNML_INLINE vquad cos( vquad_arg x )
{
	return vquad( cosf( x.m_Value[ 0 ] ), cosf( x.m_Value[ 1 ] ), cosf( x.m_Value[ 2 ] ), cosf( x.m_Value[ 3 ] ) );
}

SNML_INLINE vquad tan( vquad_arg x )
{
	return vquad( tanf( x.m_Value[ 0 ] ), tanf( x.m_Value[ 1 ] ), tanf( x.m_Value[ 2 ] ), tanf( x.m_Value[ 3 ] ) );
}

SNML_INLINE vquad asin( vquad_arg x )
{
	return vquad( asinf( x.m_Value[ 0 ] ), asinf( x.m_Value[ 1 ] ), asinf( x.m_Value[ 2 ] ), asinf( x.m_Value[ 3 ] ) );
}

SNML_INLINE vquad acos( vquad_arg x )
{
	return vquad( acosf( x.m_Value[ 0 ] ), acosf( x.m_Value[ 1 ] ), acosf( x.m_Value[ 2 ] ), acosf( x.m_Value[ 3 ] ) );
}

SNML_INLINE vquad exp( vquad_arg x )
{
	return vquad( expf( x.m_Value[ 0 ] ), expf( x.m_Value[ 1 ] ), expf( x.m_Value[ 2 ] ), expf( x.m_Value[ 3 ] ) );
}

SNML_INLINE vquad log( vquad_arg x )
{
	return vquad( logf( x.m_Value[ 0 ] ), logf( x.m_Value[ 1 ] ), logf( x.m_Value[ 2 ] ), logf( x.m_Value[ 3 ] ) );
}

SNML_INLINE vquad pow( vquad_arg x, vquad_arg power )
{
	return vquad( powf( x.m_Value[ 0 ], power.m_Value[ 0 ] ), powf( x.m_Value[ 1 ], power.m_Value[ 1 ] ), powf( x.m_Value[ 2 ], power.m_Value[ 2 ] ), powf( x.m_Value[ 3 ], power.m_Value[ 3 ] ) );
}

SNML_INLINE vquad atan2( vquad_arg y, vquad_arg x )
{
	return vquad( atan2f( y.m_Value[ 0 ], x.m_Value[ 0 ] ), atan2f( y.m_Value[ 1 ], x.m_Value[ 1 ] ), atan2f( y.m_Value[ 2 ], x.m_Value[ 2 ] ), atan2f( y.m_Value[ 3 ], x.m_Value[ 3 ] ) );
}

SNML_INLINE vquad min( vquad_arg x, vquad_arg y )
{
	return vquad( x.m_Value[ 0 ] < y.m_Value[ 0 ] ? x.m_Value[ 0 ] : y.m_Value[ 0 ], x.m_Value[ 1 ] < y.m_Value[ 1 ] ? x.m_Value[ 1 ] : y.m_Value[ 1 ], x.m_Value[ 2 ] < y.m_Value[ 2 ] ? x.m_Value[ 2 ] : y.m_Value[ 2 ], x.m_Value[ 3 ] < y.m_Value[ 3 ] ? x.m_Value[ 3 ] : y.m_Value[ 3 ] );
}

SNML_INLINE vquad max( vquad_arg x, vquad_arg y )
{
	return vquad( x.m_Value[ 0 ] > y.m_Value[ 0 ] ? x.m_Value[ 0 ] : y.m_Value[ 0 ], x.m_Value[ 1 ] > y.m_Value[ 1 ] ? x.m_Value[ 1 ] : y.m_Value[ 1 ], x.m_Value[ 2 ] > y.m_Value[ 2 ] ? x.m_Value[ 2 ] : y.m_Value[ 2 ], x.m_Value[ 3 ] > y.m_Value[ 3 ] ? x.m_Value[ 3 ] : y.m_Value[ 3 ] );
}

SNML_INLINE vquad sgn( vquad_arg x )
{
	return vquad( x.m_Value[ 0 ] < 0 ? -1.0f : x.m_Value[ 0 ] > 0 ? 1.0f : 0, x.m_Value[ 1 ] < 0 ? -1.0f : x.m_Value[ 1 ] > 0 ? 1.0f : 0, x.m_Value[ 2 ] < 0 ? -1.0f : x.m_Value[ 2 ] > 0 ? 1.0f : 0, x.m_Value[ 3 ] < 0 ? -1.0f : x.m_Value[ 3 ] > 0 ? 1.0f : 0 );
}

SNML_INLINE vquad abs( vquad_arg x )
{
	return vquad( fabsf( x.m_Value[ 0 ] ), fabsf( x.m_Value[ 1 ] ), fabsf( x.m_Value[ 2 ] ), fabsf( x.m_Value[ 3 ] ) );
}

SNML_INLINE vquad sqrt( vquad_arg x )
{
	return vquad( sqrtf( x.m_Value[ 0 ] ), sqrtf( x.m_Value[ 1 ] ), sqrtf( x.m_Value[ 2 ] ), sqrtf( x.m_Value[ 3 ] ) );
}

SNML_INLINE vquad rsqrt( vquad_arg x )
{
	return vquad( 1.0f/sqrtf( x.m_Value[ 0 ] ), 1.0f/sqrtf( x.m_Value[ 1 ] ), 1.0f/sqrtf( x.m_Value[ 2 ] ), 1.0f/sqrtf( x.m_Value[ 3 ] ) );
}
				
SNML_INLINE vquad recip( vquad_arg x )
{
	return vquad( 1.0f/( x.m_Value[ 0 ] ), 1.0f/( x.m_Value[ 1 ] ), 1.0f/( x.m_Value[ 2 ] ), 1.0f/( x.m_Value[ 3 ] ) );
}
				
SNML_INLINE vquad select_ge( vquad_arg x, vquad_arg y, vquad_arg t, vquad_arg f )
{
	return vquad( x.m_Value[ 0 ] >= y.m_Value[ 0 ] ? t.m_Value[ 0 ] : f.m_Value[ 0 ], x.m_Value[ 1 ] >= y.m_Value[ 1 ] ? t.m_Value[ 1 ] : f.m_Value[ 1 ], x.m_Value[ 2 ] >= y.m_Value[ 2 ] ? t.m_Value[ 2 ] : f.m_Value[ 2 ], x.m_Value[ 3 ] >= y.m_Value[ 3 ] ? t.m_Value[ 3 ] : f.m_Value[ 3 ] );
}

SNML_INLINE vquad select_gt( vquad_arg x, vquad_arg y, vquad_arg t, vquad_arg f )
{
	return vquad( x.m_Value[ 0 ] > y.m_Value[ 0 ] ? t.m_Value[ 0 ] : f.m_Value[ 0 ], x.m_Value[ 1 ] > y.m_Value[ 1 ] ? t.m_Value[ 1 ] : f.m_Value[ 1 ], x.m_Value[ 2 ] > y.m_Value[ 2 ] ? t.m_Value[ 2 ] : f.m_Value[ 2 ], x.m_Value[ 3 ] > y.m_Value[ 3 ] ? t.m_Value[ 3 ] : f.m_Value[ 3 ] );
}

SNML_INLINE vquad select_le( vquad_arg x, vquad_arg y, vquad_arg t, vquad_arg f )
{
	return vquad( x.m_Value[ 0 ] <= y.m_Value[ 0 ] ? t.m_Value[ 0 ] : f.m_Value[ 0 ], x.m_Value[ 1 ] <= y.m_Value[ 1 ] ? t.m_Value[ 1 ] : f.m_Value[ 1 ], x.m_Value[ 2 ] <= y.m_Value[ 2 ] ? t.m_Value[ 2 ] : f.m_Value[ 2 ], x.m_Value[ 3 ] <= y.m_Value[ 3 ] ? t.m_Value[ 3 ] : f.m_Value[ 3 ] );
}

SNML_INLINE vquad select_lt( vquad_arg x, vquad_arg y, vquad_arg t, vquad_arg f )
{
	return vquad( x.m_Value[ 0 ] < y.m_Value[ 0 ] ? t.m_Value[ 0 ] : f.m_Value[ 0 ], x.m_Value[ 1 ] < y.m_Value[ 1 ] ? t.m_Value[ 1 ] : f.m_Value[ 1 ], x.m_Value[ 2 ] < y.m_Value[ 2 ] ? t.m_Value[ 2 ] : f.m_Value[ 2 ], x.m_Value[ 3 ] < y.m_Value[ 3 ] ? t.m_Value[ 3 ] : f.m_Value[ 3 ] );
}

SNML_INLINE vquad select_eq( vquad_arg x, vquad_arg y, vquad_arg t, vquad_arg f )
{
	return vquad( x.m_Value[ 0 ] == y.m_Value[ 0 ] ? t.m_Value[ 0 ] : f.m_Value[ 0 ], x.m_Value[ 1 ] == y.m_Value[ 1 ] ? t.m_Value[ 1 ] : f.m_Value[ 1 ], x.m_Value[ 2 ] == y.m_Value[ 2 ] ? t.m_Value[ 2 ] : f.m_Value[ 2 ], x.m_Value[ 3 ] == y.m_Value[ 3 ] ? t.m_Value[ 3 ] : f.m_Value[ 3 ] );
}

SNML_INLINE vquad select_ne( vquad_arg x, vquad_arg y, vquad_arg t, vquad_arg f )
{
	return vquad( x.m_Value[ 0 ] != y.m_Value[ 0 ] ? t.m_Value[ 0 ] : f.m_Value[ 0 ], x.m_Value[ 1 ] != y.m_Value[ 1 ] ? t.m_Value[ 1 ] : f.m_Value[ 1 ], x.m_Value[ 2 ] != y.m_Value[ 2 ] ? t.m_Value[ 2 ] : f.m_Value[ 2 ], x.m_Value[ 3 ] != y.m_Value[ 3 ] ? t.m_Value[ 3 ] : f.m_Value[ 3 ] );
}

SNML_INLINE vquad select( bool sel, vquad_arg t, vquad_arg f )
{
	return sel ? t : f;
}


SNML_INLINE quadquad sin( quadquad_arg x )
{
	return quadquad( sinf( x.m_X[ 0 ] ), sinf( x.m_X[ 1 ] ), sinf( x.m_X[ 2 ] ), sinf( x.m_X[ 3 ] ), sinf( x.m_Y[ 0 ] ), sinf( x.m_Y[ 1 ] ), sinf( x.m_Y[ 2 ] ), sinf( x.m_Y[ 3 ] ), sinf( x.m_Z[ 0 ] ), sinf( x.m_Z[ 1 ] ), sinf( x.m_Z[ 2 ] ), sinf( x.m_Z[ 3 ] ), sinf( x.m_W[ 0 ] ), sinf( x.m_W[ 1 ] ), sinf( x.m_W[ 2 ] ), sinf( x.m_W[ 3 ] ) );
}

SNML_INLINE quadquad cos( quadquad_arg x )
{
	return quadquad( cosf( x.m_X[ 0 ] ), cosf( x.m_X[ 1 ] ), cosf( x.m_X[ 2 ] ), cosf( x.m_X[ 3 ] ), cosf( x.m_Y[ 0 ] ), cosf( x.m_Y[ 1 ] ), cosf( x.m_Y[ 2 ] ), cosf( x.m_Y[ 3 ] ), cosf( x.m_Z[ 0 ] ), cosf( x.m_Z[ 1 ] ), cosf( x.m_Z[ 2 ] ), cosf( x.m_Z[ 3 ] ), cosf( x.m_W[ 0 ] ), cosf( x.m_W[ 1 ] ), cosf( x.m_W[ 2 ] ), cosf( x.m_W[ 3 ] ) );
}

SNML_INLINE quadquad tan( quadquad_arg x )
{
	return quadquad( tanf( x.m_X[ 0 ] ), tanf( x.m_X[ 1 ] ), tanf( x.m_X[ 2 ] ), tanf( x.m_X[ 3 ] ), tanf( x.m_Y[ 0 ] ), tanf( x.m_Y[ 1 ] ), tanf( x.m_Y[ 2 ] ), tanf( x.m_Y[ 3 ] ), tanf( x.m_Z[ 0 ] ), tanf( x.m_Z[ 1 ] ), tanf( x.m_Z[ 2 ] ), tanf( x.m_Z[ 3 ] ), tanf( x.m_W[ 0 ] ), tanf( x.m_W[ 1 ] ), tanf( x.m_W[ 2 ] ), tanf( x.m_W[ 3 ] ) );
}

SNML_INLINE quadquad asin( quadquad_arg x )
{
	return quadquad( asinf( x.m_X[ 0 ] ), asinf( x.m_X[ 1 ] ), asinf( x.m_X[ 2 ] ), asinf( x.m_X[ 3 ] ), asinf( x.m_Y[ 0 ] ), asinf( x.m_Y[ 1 ] ), asinf( x.m_Y[ 2 ] ), asinf( x.m_Y[ 3 ] ), asinf( x.m_Z[ 0 ] ), asinf( x.m_Z[ 1 ] ), asinf( x.m_Z[ 2 ] ), asinf( x.m_Z[ 3 ] ), asinf( x.m_W[ 0 ] ), asinf( x.m_W[ 1 ] ), asinf( x.m_W[ 2 ] ), asinf( x.m_W[ 3 ] ) );
}

SNML_INLINE quadquad acos( quadquad_arg x )
{
	return quadquad( acosf( x.m_X[ 0 ] ), acosf( x.m_X[ 1 ] ), acosf( x.m_X[ 2 ] ), acosf( x.m_X[ 3 ] ), acosf( x.m_Y[ 0 ] ), acosf( x.m_Y[ 1 ] ), acosf( x.m_Y[ 2 ] ), acosf( x.m_Y[ 3 ] ), acosf( x.m_Z[ 0 ] ), acosf( x.m_Z[ 1 ] ), acosf( x.m_Z[ 2 ] ), acosf( x.m_Z[ 3 ] ), acosf( x.m_W[ 0 ] ), acosf( x.m_W[ 1 ] ), acosf( x.m_W[ 2 ] ), acosf( x.m_W[ 3 ] ) );
}

SNML_INLINE quadquad exp( quadquad_arg x )
{
	return quadquad( expf( x.m_X[ 0 ] ), expf( x.m_X[ 1 ] ), expf( x.m_X[ 2 ] ), expf( x.m_X[ 3 ] ), expf( x.m_Y[ 0 ] ), expf( x.m_Y[ 1 ] ), expf( x.m_Y[ 2 ] ), expf( x.m_Y[ 3 ] ), expf( x.m_Z[ 0 ] ), expf( x.m_Z[ 1 ] ), expf( x.m_Z[ 2 ] ), expf( x.m_Z[ 3 ] ), expf( x.m_W[ 0 ] ), expf( x.m_W[ 1 ] ), expf( x.m_W[ 2 ] ), expf( x.m_W[ 3 ] ) );
}

SNML_INLINE quadquad log( quadquad_arg x )
{
	return quadquad( logf( x.m_X[ 0 ] ), logf( x.m_X[ 1 ] ), logf( x.m_X[ 2 ] ), logf( x.m_X[ 3 ] ), logf( x.m_Y[ 0 ] ), logf( x.m_Y[ 1 ] ), logf( x.m_Y[ 2 ] ), logf( x.m_Y[ 3 ] ), logf( x.m_Z[ 0 ] ), logf( x.m_Z[ 1 ] ), logf( x.m_Z[ 2 ] ), logf( x.m_Z[ 3 ] ), logf( x.m_W[ 0 ] ), logf( x.m_W[ 1 ] ), logf( x.m_W[ 2 ] ), logf( x.m_W[ 3 ] ) );
}

SNML_INLINE quadquad pow( quadquad_arg x, quadquad_arg power )
{
	return quadquad(
		powf( x.m_X[ 0 ], power.m_X[ 0 ] ), powf( x.m_X[ 1 ], power.m_X[ 1 ] ), powf( x.m_X[ 2 ], power.m_X[ 2 ] ), powf( x.m_X[ 3 ], power.m_X[ 3 ] ),
		powf( x.m_Y[ 0 ], power.m_Y[ 0 ] ), powf( x.m_Y[ 1 ], power.m_Y[ 1 ] ), powf( x.m_Y[ 2 ], power.m_Y[ 2 ] ), powf( x.m_Y[ 3 ], power.m_Y[ 3 ] ),
		powf( x.m_Z[ 0 ], power.m_Z[ 0 ] ), powf( x.m_Z[ 1 ], power.m_Z[ 1 ] ), powf( x.m_Z[ 2 ], power.m_Z[ 2 ] ), powf( x.m_Z[ 3 ], power.m_Z[ 3 ] ),
		powf( x.m_W[ 0 ], power.m_W[ 0 ] ), powf( x.m_W[ 1 ], power.m_W[ 1 ] ), powf( x.m_W[ 2 ], power.m_W[ 2 ] ), powf( x.m_W[ 3 ], power.m_W[ 3 ] )
	);
}

SNML_INLINE quadquad atan2( quadquad_arg y, quadquad_arg x )
{
	return quadquad(
		atan2f( y.m_X[ 0 ], x.m_X[ 0 ] ), atan2f( y.m_X[ 1 ], x.m_X[ 1 ] ), atan2f( y.m_X[ 2 ], x.m_X[ 2 ] ), atan2f( y.m_X[ 3 ], x.m_X[ 3 ] ),
		atan2f( y.m_Y[ 0 ], x.m_Y[ 0 ] ), atan2f( y.m_Y[ 1 ], x.m_Y[ 1 ] ), atan2f( y.m_Y[ 2 ], x.m_Y[ 2 ] ), atan2f( y.m_Y[ 3 ], x.m_Y[ 3 ] ),
		atan2f( y.m_Z[ 0 ], x.m_Z[ 0 ] ), atan2f( y.m_Z[ 1 ], x.m_Z[ 1 ] ), atan2f( y.m_Z[ 2 ], x.m_Z[ 2 ] ), atan2f( y.m_Z[ 3 ], x.m_Z[ 3 ] ),
		atan2f( y.m_W[ 0 ], x.m_W[ 0 ] ), atan2f( y.m_W[ 1 ], x.m_W[ 1 ] ), atan2f( y.m_W[ 2 ], x.m_W[ 2 ] ), atan2f( y.m_W[ 3 ], x.m_W[ 3 ] )
	);
}

SNML_INLINE quadquad min( quadquad_arg x, quadquad_arg y )
{
	return quadquad( x.m_X[ 0 ] < y.m_X[ 0 ] ? x.m_X[ 0 ] : y.m_X[ 0 ], x.m_X[ 1 ] < y.m_X[ 1 ] ? x.m_X[ 1 ] : y.m_X[ 1 ], x.m_X[ 2 ] < y.m_X[ 2 ] ? x.m_X[ 2 ] : y.m_X[ 2 ], x.m_X[ 3 ] < y.m_X[ 3 ] ? x.m_X[ 3 ] : y.m_X[ 3 ], x.m_Y[ 0 ] < y.m_Y[ 0 ] ? x.m_Y[ 0 ] : y.m_Y[ 0 ], x.m_Y[ 1 ] < y.m_Y[ 1 ] ? x.m_Y[ 1 ] : y.m_Y[ 1 ], x.m_Y[ 2 ] < y.m_Y[ 2 ] ? x.m_Y[ 2 ] : y.m_Y[ 2 ], x.m_Y[ 3 ] < y.m_Y[ 3 ] ? x.m_Y[ 3 ] : y.m_Y[ 3 ], x.m_Z[ 0 ] < y.m_Z[ 0 ] ? x.m_Z[ 0 ] : y.m_Z[ 0 ], x.m_Z[ 1 ] < y.m_Z[ 1 ] ? x.m_Z[ 1 ] : y.m_Z[ 1 ], x.m_Z[ 2 ] < y.m_Z[ 2 ] ? x.m_Z[ 2 ] : y.m_Z[ 2 ], x.m_Z[ 3 ] < y.m_Z[ 3 ] ? x.m_Z[ 3 ] : y.m_Z[ 3 ], x.m_W[ 0 ] < y.m_W[ 0 ] ? x.m_W[ 0 ] : y.m_W[ 0 ], x.m_W[ 1 ] < y.m_W[ 1 ] ? x.m_W[ 1 ] : y.m_W[ 1 ], x.m_W[ 2 ] < y.m_W[ 2 ] ? x.m_W[ 2 ] : y.m_W[ 2 ], x.m_W[ 3 ] < y.m_W[ 3 ] ? x.m_W[ 3 ] : y.m_W[ 3 ] );
}

SNML_INLINE quadquad max( quadquad_arg x, quadquad_arg y )
{
	return quadquad( x.m_X[ 0 ] > y.m_X[ 0 ] ? x.m_X[ 0 ] : y.m_X[ 0 ], x.m_X[ 1 ] > y.m_X[ 1 ] ? x.m_X[ 1 ] : y.m_X[ 1 ], x.m_X[ 2 ] > y.m_X[ 2 ] ? x.m_X[ 2 ] : y.m_X[ 2 ], x.m_X[ 3 ] > y.m_X[ 3 ] ? x.m_X[ 3 ] : y.m_X[ 3 ], x.m_Y[ 0 ] > y.m_Y[ 0 ] ? x.m_Y[ 0 ] : y.m_Y[ 0 ], x.m_Y[ 1 ] > y.m_Y[ 1 ] ? x.m_Y[ 1 ] : y.m_Y[ 1 ], x.m_Y[ 2 ] > y.m_Y[ 2 ] ? x.m_Y[ 2 ] : y.m_Y[ 2 ], x.m_Y[ 3 ] > y.m_Y[ 3 ] ? x.m_Y[ 3 ] : y.m_Y[ 3 ], x.m_Z[ 0 ] > y.m_Z[ 0 ] ? x.m_Z[ 0 ] : y.m_Z[ 0 ], x.m_Z[ 1 ] > y.m_Z[ 1 ] ? x.m_Z[ 1 ] : y.m_Z[ 1 ], x.m_Z[ 2 ] > y.m_Z[ 2 ] ? x.m_Z[ 2 ] : y.m_Z[ 2 ], x.m_Z[ 3 ] > y.m_Z[ 3 ] ? x.m_Z[ 3 ] : y.m_Z[ 3 ], x.m_W[ 0 ] > y.m_W[ 0 ] ? x.m_W[ 0 ] : y.m_W[ 0 ], x.m_W[ 1 ] > y.m_W[ 1 ] ? x.m_W[ 1 ] : y.m_W[ 1 ], x.m_W[ 2 ] > y.m_W[ 2 ] ? x.m_W[ 2 ] : y.m_W[ 2 ], x.m_W[ 3 ] > y.m_W[ 3 ] ? x.m_W[ 3 ] : y.m_W[ 3 ] );
}

SNML_INLINE quadquad sgn( quadquad_arg x )
{
	return quadquad( x.m_X[ 0 ] < 0 ? -1.0f : x.m_X[ 0 ] > 0 ? 1.0f : 0, x.m_X[ 1 ] < 0 ? -1.0f : x.m_X[ 1 ] > 0 ? 1.0f : 0, x.m_X[ 2 ] < 0 ? -1.0f : x.m_X[ 2 ] > 0 ? 1.0f : 0, x.m_X[ 3 ] < 0 ? -1.0f : x.m_X[ 3 ] > 0 ? 1.0f : 0, x.m_Y[ 0 ] < 0 ? -1.0f : x.m_Y[ 0 ] > 0 ? 1.0f : 0, x.m_Y[ 1 ] < 0 ? -1.0f : x.m_Y[ 1 ] > 0 ? 1.0f : 0, x.m_Y[ 2 ] < 0 ? -1.0f : x.m_Y[ 2 ] > 0 ? 1.0f : 0, x.m_Y[ 3 ] < 0 ? -1.0f : x.m_Y[ 3 ] > 0 ? 1.0f : 0, x.m_Z[ 0 ] < 0 ? -1.0f : x.m_Z[ 0 ] > 0 ? 1.0f : 0, x.m_Z[ 1 ] < 0 ? -1.0f : x.m_Z[ 1 ] > 0 ? 1.0f : 0, x.m_Z[ 2 ] < 0 ? -1.0f : x.m_Z[ 2 ] > 0 ? 1.0f : 0, x.m_Z[ 3 ] < 0 ? -1.0f : x.m_Z[ 3 ] > 0 ? 1.0f : 0, x.m_W[ 0 ] < 0 ? -1.0f : x.m_W[ 0 ] > 0 ? 1.0f : 0, x.m_W[ 1 ] < 0 ? -1.0f : x.m_W[ 1 ] > 0 ? 1.0f : 0, x.m_W[ 2 ] < 0 ? -1.0f : x.m_W[ 2 ] > 0 ? 1.0f : 0, x.m_W[ 3 ] < 0 ? -1.0f : x.m_W[ 3 ] > 0 ? 1.0f : 0 );
}

SNML_INLINE quadquad abs( quadquad_arg x )
{
	return quadquad( fabs( x.m_X[ 0 ] ), fabs( x.m_X[ 1 ] ), fabs( x.m_X[ 2 ] ), fabs( x.m_X[ 3 ] ), fabs( x.m_Y[ 0 ] ), fabs( x.m_Y[ 1 ] ), fabs( x.m_Y[ 2 ] ), fabs( x.m_Y[ 3 ] ), fabs( x.m_Z[ 0 ] ), fabs( x.m_Z[ 1 ] ), fabs( x.m_Z[ 2 ] ), fabs( x.m_Z[ 3 ] ), fabs( x.m_W[ 0 ] ), fabs( x.m_W[ 1 ] ), fabs( x.m_W[ 2 ] ), fabs( x.m_W[ 3 ] ) );
}

SNML_INLINE quadquad sqrt( quadquad_arg x )
{
	return quadquad( sqrtf( x.m_X[ 0 ] ), sqrtf( x.m_X[ 1 ] ), sqrtf( x.m_X[ 2 ] ), sqrtf( x.m_X[ 3 ] ), sqrtf( x.m_Y[ 0 ] ), sqrtf( x.m_Y[ 1 ] ), sqrtf( x.m_Y[ 2 ] ), sqrtf( x.m_Y[ 3 ] ), sqrtf( x.m_Z[ 0 ] ), sqrtf( x.m_Z[ 1 ] ), sqrtf( x.m_Z[ 2 ] ), sqrtf( x.m_Z[ 3 ] ), sqrtf( x.m_W[ 0 ] ), sqrtf( x.m_W[ 1 ] ), sqrtf( x.m_W[ 2 ] ), sqrtf( x.m_W[ 3 ] ) );
}

SNML_INLINE quadquad rsqrt( quadquad_arg x )
{
	return quadquad( 1.0f/sqrtf( x.m_X[ 0 ] ), 1.0f/sqrtf( x.m_X[ 1 ] ), 1.0f/sqrtf( x.m_X[ 2 ] ), 1.0f/sqrtf( x.m_X[ 3 ] ), 1.0f/sqrtf( x.m_X[ 0 ] ), 1.0f/sqrtf( x.m_Y[ 1 ] ), 1.0f/sqrtf( x.m_Y[ 2 ] ), 1.0f/sqrtf( x.m_Y[ 3 ] ), 1.0f/sqrtf( x.m_Y[ 0 ] ), 1.0f/sqrtf( x.m_Z[ 1 ] ), 1.0f/sqrtf( x.m_Z[ 2 ] ), 1.0f/sqrtf( x.m_Z[ 3 ] ), 1.0f/sqrtf( x.m_Z[ 0 ] ), 1.0f/sqrtf( x.m_W[ 1 ] ), 1.0f/sqrtf( x.m_W[ 2 ] ), 1.0f/sqrtf( x.m_W[ 3 ] ) );
}
				
SNML_INLINE quadquad recip( quadquad_arg x )
{
	return quadquad( 1.0f/( x.m_X[ 0 ] ), 1.0f/( x.m_X[ 1 ] ), 1.0f/( x.m_X[ 2 ] ), 1.0f/( x.m_X[ 3 ] ), 1.0f/( x.m_X[ 0 ] ), 1.0f/( x.m_Y[ 1 ] ), 1.0f/( x.m_Y[ 2 ] ), 1.0f/( x.m_Y[ 3 ] ), 1.0f/( x.m_Y[ 0 ] ), 1.0f/( x.m_Z[ 1 ] ), 1.0f/( x.m_Z[ 2 ] ), 1.0f/( x.m_Z[ 3 ] ), 1.0f/( x.m_Z[ 0 ] ), 1.0f/( x.m_W[ 1 ] ), 1.0f/( x.m_W[ 2 ] ), 1.0f/( x.m_W[ 3 ] ) );
}
				
SNML_INLINE quadquad select_ge( quadquad_arg x, quadquad_arg y, quadquad_arg t, quadquad_arg f )
{
	return quadquad( x.m_X[ 0 ] >= y.m_X[ 0 ] ? t.m_X[ 0 ] : f.m_X[ 0 ], x.m_X[ 1 ] >= y.m_X[ 1 ] ? t.m_X[ 1 ] : f.m_X[ 1 ], x.m_X[ 2 ] >= y.m_X[ 2 ] ? t.m_X[ 2 ] : f.m_X[ 2 ], x.m_X[ 3 ] >= y.m_X[ 3 ] ? t.m_X[ 3 ] : f.m_X[ 3 ], x.m_Y[ 0 ] >= y.m_Y[ 0 ] ? t.m_Y[ 0 ] : f.m_Y[ 0 ], x.m_Y[ 1 ] >= y.m_Y[ 1 ] ? t.m_Y[ 1 ] : f.m_Y[ 1 ], x.m_Y[ 2 ] >= y.m_Y[ 2 ] ? t.m_Y[ 2 ] : f.m_Y[ 2 ], x.m_Y[ 3 ] >= y.m_Y[ 3 ] ? t.m_Y[ 3 ] : f.m_Y[ 3 ], x.m_Z[ 0 ] >= y.m_Z[ 0 ] ? t.m_Z[ 0 ] : f.m_Z[ 0 ], x.m_Z[ 1 ] >= y.m_Z[ 1 ] ? t.m_Z[ 1 ] : f.m_Z[ 1 ], x.m_Z[ 2 ] >= y.m_Z[ 2 ] ? t.m_Z[ 2 ] : f.m_Z[ 2 ], x.m_Z[ 3 ] >= y.m_Z[ 3 ] ? t.m_Z[ 3 ] : f.m_Z[ 3 ], x.m_W[ 0 ] >= y.m_W[ 0 ] ? t.m_W[ 0 ] : f.m_W[ 0 ], x.m_W[ 1 ] >= y.m_W[ 1 ] ? t.m_W[ 1 ] : f.m_W[ 1 ], x.m_W[ 2 ] >= y.m_W[ 2 ] ? t.m_W[ 2 ] : f.m_W[ 2 ], x.m_W[ 3 ] >= y.m_W[ 3 ] ? t.m_W[ 3 ] : f.m_W[ 3 ] );
}

SNML_INLINE quadquad select_gt( quadquad_arg x, quadquad_arg y, quadquad_arg t, quadquad_arg f )
{
	return quadquad( x.m_X[ 0 ] > y.m_X[ 0 ] ? t.m_X[ 0 ] : f.m_X[ 0 ], x.m_X[ 1 ] > y.m_X[ 1 ] ? t.m_X[ 1 ] : f.m_X[ 1 ], x.m_X[ 2 ] > y.m_X[ 2 ] ? t.m_X[ 2 ] : f.m_X[ 2 ], x.m_X[ 3 ] > y.m_X[ 3 ] ? t.m_X[ 3 ] : f.m_X[ 3 ], x.m_Y[ 0 ] > y.m_Y[ 0 ] ? t.m_Y[ 0 ] : f.m_Y[ 0 ], x.m_Y[ 1 ] > y.m_Y[ 1 ] ? t.m_Y[ 1 ] : f.m_Y[ 1 ], x.m_Y[ 2 ] > y.m_Y[ 2 ] ? t.m_Y[ 2 ] : f.m_Y[ 2 ], x.m_Y[ 3 ] > y.m_Y[ 3 ] ? t.m_Y[ 3 ] : f.m_Y[ 3 ], x.m_Z[ 0 ] > y.m_Z[ 0 ] ? t.m_Z[ 0 ] : f.m_Z[ 0 ], x.m_Z[ 1 ] > y.m_Z[ 1 ] ? t.m_Z[ 1 ] : f.m_Z[ 1 ], x.m_Z[ 2 ] > y.m_Z[ 2 ] ? t.m_Z[ 2 ] : f.m_Z[ 2 ], x.m_Z[ 3 ] > y.m_Z[ 3 ] ? t.m_Z[ 3 ] : f.m_Z[ 3 ], x.m_W[ 0 ] > y.m_W[ 0 ] ? t.m_W[ 0 ] : f.m_W[ 0 ], x.m_W[ 1 ] > y.m_W[ 1 ] ? t.m_W[ 1 ] : f.m_W[ 1 ], x.m_W[ 2 ] > y.m_W[ 2 ] ? t.m_W[ 2 ] : f.m_W[ 2 ], x.m_W[ 3 ] > y.m_W[ 3 ] ? t.m_W[ 3 ] : f.m_W[ 3 ] );
}

SNML_INLINE quadquad select_le( quadquad_arg x, quadquad_arg y, quadquad_arg t, quadquad_arg f )
{
	return quadquad( x.m_X[ 0 ] <= y.m_X[ 0 ] ? t.m_X[ 0 ] : f.m_X[ 0 ], x.m_X[ 1 ] <= y.m_X[ 1 ] ? t.m_X[ 1 ] : f.m_X[ 1 ], x.m_X[ 2 ] <= y.m_X[ 2 ] ? t.m_X[ 2 ] : f.m_X[ 2 ], x.m_X[ 3 ] <= y.m_X[ 3 ] ? t.m_X[ 3 ] : f.m_X[ 3 ], x.m_Y[ 0 ] <= y.m_Y[ 0 ] ? t.m_Y[ 0 ] : f.m_Y[ 0 ], x.m_Y[ 1 ] <= y.m_Y[ 1 ] ? t.m_Y[ 1 ] : f.m_Y[ 1 ], x.m_Y[ 2 ] <= y.m_Y[ 2 ] ? t.m_Y[ 2 ] : f.m_Y[ 2 ], x.m_Y[ 3 ] <= y.m_Y[ 3 ] ? t.m_Y[ 3 ] : f.m_Y[ 3 ], x.m_Z[ 0 ] <= y.m_Z[ 0 ] ? t.m_Z[ 0 ] : f.m_Z[ 0 ], x.m_Z[ 1 ] <= y.m_Z[ 1 ] ? t.m_Z[ 1 ] : f.m_Z[ 1 ], x.m_Z[ 2 ] <= y.m_Z[ 2 ] ? t.m_Z[ 2 ] : f.m_Z[ 2 ], x.m_Z[ 3 ] <= y.m_Z[ 3 ] ? t.m_Z[ 3 ] : f.m_Z[ 3 ], x.m_W[ 0 ] <= y.m_W[ 0 ] ? t.m_W[ 0 ] : f.m_W[ 0 ], x.m_W[ 1 ] <= y.m_W[ 1 ] ? t.m_W[ 1 ] : f.m_W[ 1 ], x.m_W[ 2 ] <= y.m_W[ 2 ] ? t.m_W[ 2 ] : f.m_W[ 2 ], x.m_W[ 3 ] <= y.m_W[ 3 ] ? t.m_W[ 3 ] : f.m_W[ 3 ] );
}

SNML_INLINE quadquad select_lt( quadquad_arg x, quadquad_arg y, quadquad_arg t, quadquad_arg f )
{
	return quadquad( x.m_X[ 0 ] < y.m_X[ 0 ] ? t.m_X[ 0 ] : f.m_X[ 0 ], x.m_X[ 1 ] < y.m_X[ 1 ] ? t.m_X[ 1 ] : f.m_X[ 1 ], x.m_X[ 2 ] < y.m_X[ 2 ] ? t.m_X[ 2 ] : f.m_X[ 2 ], x.m_X[ 3 ] < y.m_X[ 3 ] ? t.m_X[ 3 ] : f.m_X[ 3 ], x.m_Y[ 0 ] < y.m_Y[ 0 ] ? t.m_Y[ 0 ] : f.m_Y[ 0 ], x.m_Y[ 1 ] < y.m_Y[ 1 ] ? t.m_Y[ 1 ] : f.m_Y[ 1 ], x.m_Y[ 2 ] < y.m_Y[ 2 ] ? t.m_Y[ 2 ] : f.m_Y[ 2 ], x.m_Y[ 3 ] < y.m_Y[ 3 ] ? t.m_Y[ 3 ] : f.m_Y[ 3 ], x.m_Z[ 0 ] < y.m_Z[ 0 ] ? t.m_Z[ 0 ] : f.m_Z[ 0 ], x.m_Z[ 1 ] < y.m_Z[ 1 ] ? t.m_Z[ 1 ] : f.m_Z[ 1 ], x.m_Z[ 2 ] < y.m_Z[ 2 ] ? t.m_Z[ 2 ] : f.m_Z[ 2 ], x.m_Z[ 3 ] < y.m_Z[ 3 ] ? t.m_Z[ 3 ] : f.m_Z[ 3 ], x.m_W[ 0 ] < y.m_W[ 0 ] ? t.m_W[ 0 ] : f.m_W[ 0 ], x.m_W[ 1 ] < y.m_W[ 1 ] ? t.m_W[ 1 ] : f.m_W[ 1 ], x.m_W[ 2 ] < y.m_W[ 2 ] ? t.m_W[ 2 ] : f.m_W[ 2 ], x.m_W[ 3 ] < y.m_W[ 3 ] ? t.m_W[ 3 ] : f.m_W[ 3 ] );
}

SNML_INLINE quadquad select_eq( quadquad_arg x, quadquad_arg y, quadquad_arg t, quadquad_arg f )
{
	return quadquad( x.m_X[ 0 ] == y.m_X[ 0 ] ? t.m_X[ 0 ] : f.m_X[ 0 ], x.m_X[ 1 ] == y.m_X[ 1 ] ? t.m_X[ 1 ] : f.m_X[ 1 ], x.m_X[ 2 ] == y.m_X[ 2 ] ? t.m_X[ 2 ] : f.m_X[ 2 ], x.m_X[ 3 ] == y.m_X[ 3 ] ? t.m_X[ 3 ] : f.m_X[ 3 ], x.m_Y[ 0 ] == y.m_Y[ 0 ] ? t.m_Y[ 0 ] : f.m_Y[ 0 ], x.m_Y[ 1 ] == y.m_Y[ 1 ] ? t.m_Y[ 1 ] : f.m_Y[ 1 ], x.m_Y[ 2 ] == y.m_Y[ 2 ] ? t.m_Y[ 2 ] : f.m_Y[ 2 ], x.m_Y[ 3 ] == y.m_Y[ 3 ] ? t.m_Y[ 3 ] : f.m_Y[ 3 ], x.m_Z[ 0 ] == y.m_Z[ 0 ] ? t.m_Z[ 0 ] : f.m_Z[ 0 ], x.m_Z[ 1 ] == y.m_Z[ 1 ] ? t.m_Z[ 1 ] : f.m_Z[ 1 ], x.m_Z[ 2 ] == y.m_Z[ 2 ] ? t.m_Z[ 2 ] : f.m_Z[ 2 ], x.m_Z[ 3 ] == y.m_Z[ 3 ] ? t.m_Z[ 3 ] : f.m_Z[ 3 ], x.m_W[ 0 ] == y.m_W[ 0 ] ? t.m_W[ 0 ] : f.m_W[ 0 ], x.m_W[ 1 ] == y.m_W[ 1 ] ? t.m_W[ 1 ] : f.m_W[ 1 ], x.m_W[ 2 ] == y.m_W[ 2 ] ? t.m_W[ 2 ] : f.m_W[ 2 ], x.m_W[ 3 ] == y.m_W[ 3 ] ? t.m_W[ 3 ] : f.m_W[ 3 ] );
}

SNML_INLINE quadquad select_ne( quadquad_arg x, quadquad_arg y, quadquad_arg t, quadquad_arg f )
{
	return quadquad( x.m_X[ 0 ] != y.m_X[ 0 ] ? t.m_X[ 0 ] : f.m_X[ 0 ], x.m_X[ 1 ] != y.m_X[ 1 ] ? t.m_X[ 1 ] : f.m_X[ 1 ], x.m_X[ 2 ] != y.m_X[ 2 ] ? t.m_X[ 2 ] : f.m_X[ 2 ], x.m_X[ 3 ] != y.m_X[ 3 ] ? t.m_X[ 3 ] : f.m_X[ 3 ], x.m_Y[ 0 ] != y.m_Y[ 0 ] ? t.m_Y[ 0 ] : f.m_Y[ 0 ], x.m_Y[ 1 ] != y.m_Y[ 1 ] ? t.m_Y[ 1 ] : f.m_Y[ 1 ], x.m_Y[ 2 ] != y.m_Y[ 2 ] ? t.m_Y[ 2 ] : f.m_Y[ 2 ], x.m_Y[ 3 ] != y.m_Y[ 3 ] ? t.m_Y[ 3 ] : f.m_Y[ 3 ], x.m_Z[ 0 ] != y.m_Z[ 0 ] ? t.m_Z[ 0 ] : f.m_Z[ 0 ], x.m_Z[ 1 ] != y.m_Z[ 1 ] ? t.m_Z[ 1 ] : f.m_Z[ 1 ], x.m_Z[ 2 ] != y.m_Z[ 2 ] ? t.m_Z[ 2 ] : f.m_Z[ 2 ], x.m_Z[ 3 ] != y.m_Z[ 3 ] ? t.m_Z[ 3 ] : f.m_Z[ 3 ], x.m_W[ 0 ] != y.m_W[ 0 ] ? t.m_W[ 0 ] : f.m_W[ 0 ], x.m_W[ 1 ] != y.m_W[ 1 ] ? t.m_W[ 1 ] : f.m_W[ 1 ], x.m_W[ 2 ] != y.m_W[ 2 ] ? t.m_W[ 2 ] : f.m_W[ 2 ], x.m_W[ 3 ] != y.m_W[ 3 ] ? t.m_W[ 3 ] : f.m_W[ 3 ] );
}

SNML_INLINE quadquad select( bool sel, quadquad_arg t, quadquad_arg f )
{
	return sel ? t : f;
}


SNML_INLINE quadquad::operator float() const
{
	return m_X[ 0 ];
}

SNML_INLINE quadquad quadquad::unit4x4()
{
	return quadquad
	(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	);
}

SNML_INLINE scalar sum_sideways( quadquad_arg rhs )
{
	return
		rhs.m_X[ 0 ] + rhs.m_X[ 1 ] + rhs.m_X[ 2 ] + rhs.m_X[ 3 ] +
		rhs.m_Y[ 0 ] + rhs.m_Y[ 1 ] + rhs.m_Y[ 2 ] + rhs.m_Y[ 3 ] +
		rhs.m_Z[ 0 ] + rhs.m_Z[ 1 ] + rhs.m_Z[ 2 ] + rhs.m_Z[ 3 ] +
		rhs.m_W[ 0 ] + rhs.m_W[ 1 ] + rhs.m_W[ 2 ] + rhs.m_W[ 3 ]
	;
}

SNML_INLINE scalar max_sideways( quadquad_arg rhs )
{
	float max01 = rhs.m_X[ 0 ] > rhs.m_X[ 1 ] ? rhs.m_X[ 0 ] : rhs.m_X[ 1 ];
	float max23 = rhs.m_X[ 2 ] > rhs.m_X[ 3 ] ? rhs.m_X[ 2 ] : rhs.m_X[ 3 ];
	float maxx = max01 > max23 ? max01 : max23;
	max01 = rhs.m_Y[ 0 ] > rhs.m_Y[ 1 ] ? rhs.m_Y[ 0 ] : rhs.m_Y[ 1 ];
	max23 = rhs.m_Y[ 2 ] > rhs.m_Y[ 3 ] ? rhs.m_Y[ 2 ] : rhs.m_Y[ 3 ];
	float maxy = max01 > max23 ? max01 : max23;
	max01 = rhs.m_Z[ 0 ] > rhs.m_Z[ 1 ] ? rhs.m_Z[ 0 ] : rhs.m_Z[ 1 ];
	max23 = rhs.m_Z[ 2 ] > rhs.m_Z[ 3 ] ? rhs.m_Z[ 2 ] : rhs.m_Z[ 3 ];
	float maxz = max01 > max23 ? max01 : max23;
	max01 = rhs.m_W[ 0 ] > rhs.m_W[ 1 ] ? rhs.m_W[ 0 ] : rhs.m_W[ 1 ];
	max23 = rhs.m_W[ 2 ] > rhs.m_W[ 3 ] ? rhs.m_W[ 2 ] : rhs.m_W[ 3 ];
	float maxw = max01 > max23 ? max01 : max23;
	float maxxy = maxx > maxy ? maxx : maxy;
	float maxzw = maxz > maxw ? maxz : maxw;
	return maxxy > maxzw ? maxxy : maxzw;
}

SNML_INLINE scalar min_sideways( quadquad_arg rhs )
{
	float max01 = rhs.m_X[ 0 ] < rhs.m_X[ 1 ] ? rhs.m_X[ 0 ] : rhs.m_X[ 1 ];
	float max23 = rhs.m_X[ 2 ] < rhs.m_X[ 3 ] ? rhs.m_X[ 2 ] : rhs.m_X[ 3 ];
	float maxx = max01 < max23 ? max01 : max23;
	max01 = rhs.m_Y[ 0 ] < rhs.m_Y[ 1 ] ? rhs.m_Y[ 0 ] : rhs.m_Y[ 1 ];
	max23 = rhs.m_Y[ 2 ] < rhs.m_Y[ 3 ] ? rhs.m_Y[ 2 ] : rhs.m_Y[ 3 ];
	float maxy = max01 < max23 ? max01 : max23;
	max01 = rhs.m_Z[ 0 ] < rhs.m_Z[ 1 ] ? rhs.m_Z[ 0 ] : rhs.m_Z[ 1 ];
	max23 = rhs.m_Z[ 2 ] < rhs.m_Z[ 3 ] ? rhs.m_Z[ 2 ] : rhs.m_Z[ 3 ];
	float maxz = max01 < max23 ? max01 : max23;
	max01 = rhs.m_W[ 0 ] < rhs.m_W[ 1 ] ? rhs.m_W[ 0 ] : rhs.m_W[ 1 ];
	max23 = rhs.m_W[ 2 ] < rhs.m_W[ 3 ] ? rhs.m_W[ 2 ] : rhs.m_W[ 3 ];
	float maxw = max01 < max23 ? max01 : max23;
	float maxxy = maxx < maxy ? maxx : maxy;
	float maxzw = maxz < maxw ? maxz : maxw;
	return maxxy < maxzw ? maxxy : maxzw;
}

SNML_INLINE void quadquad::Rotate( scalar_arg angle, vquad_arg xyz )
{
	float s = sinf( angle.m_Value * (0.5f * 3.14159265359f/180) );
	float c = cosf( angle.m_Value * (0.5f * 3.14159265359f/180) );
	float conj[ 4 ] = { -s * xyz.m_Value[ 0 ], -s * xyz.m_Value[ 1 ], -s * xyz.m_Value[ 2 ], c };
	float quat[ 4 ] = { s * xyz.m_Value[ 0 ], s * xyz.m_Value[ 1 ], s * xyz.m_Value[ 2 ], c };
	float x[ 4 ], y[ 4 ], z[ 4 ];
	QMul( x, m_X, conj );
	QMul( y, m_Y, conj );
	QMul( z, m_Z, conj );
	QMul( m_X, quat, x );
	QMul( m_Y, quat, y );
	QMul( m_Z, quat, z );
}

SNML_INLINE void quadquad::Translate( vquad_arg xyz )
{
	m_W[ 0 ] += m_X[ 0 ] * xyz.m_Value[ 0 ] + m_Y[ 0 ] * xyz.m_Value[ 1 ] + m_Z[ 0 ] * xyz.m_Value[ 2 ];
	m_W[ 1 ] += m_X[ 1 ] * xyz.m_Value[ 0 ] + m_Y[ 1 ] * xyz.m_Value[ 1 ] + m_Z[ 1 ] * xyz.m_Value[ 2 ];
	m_W[ 2 ] += m_X[ 2 ] * xyz.m_Value[ 0 ] + m_Y[ 2 ] * xyz.m_Value[ 1 ] + m_Z[ 2 ] * xyz.m_Value[ 2 ];
}

SNML_INLINE void quadquad::Scale( vquad_arg xyz )
{
	m_X[ 0 ] *= xyz.m_Value[ 0 ]; m_X[ 1 ] *= xyz.m_Value[ 0 ]; m_X[ 2 ] *= xyz.m_Value[ 0 ];
	m_Y[ 0 ] *= xyz.m_Value[ 1 ]; m_Y[ 1 ] *= xyz.m_Value[ 1 ]; m_Y[ 2 ] *= xyz.m_Value[ 1 ];
	m_Z[ 0 ] *= xyz.m_Value[ 2 ]; m_Z[ 1 ] *= xyz.m_Value[ 2 ]; m_Z[ 2 ] *= xyz.m_Value[ 2 ];
}

SNML_INLINE void quadquad::MultMatrix( quadquad_arg rhs )
{
	*this = quadquad(
		rhs.m_X[0]*m_X[0]+rhs.m_X[1]*m_Y[0]+rhs.m_X[2]*m_Z[0]+rhs.m_X[3]*m_W[0],
		rhs.m_X[0]*m_X[1]+rhs.m_X[1]*m_Y[1]+rhs.m_X[2]*m_Z[1]+rhs.m_X[3]*m_W[1],
		rhs.m_X[0]*m_X[2]+rhs.m_X[1]*m_Y[2]+rhs.m_X[2]*m_Z[2]+rhs.m_X[3]*m_W[2],
		rhs.m_X[0]*m_X[3]+rhs.m_X[1]*m_Y[3]+rhs.m_X[2]*m_Z[3]+rhs.m_X[3]*m_W[3],
		rhs.m_Y[0]*m_X[0]+rhs.m_Y[1]*m_Y[0]+rhs.m_Y[2]*m_Z[0]+rhs.m_Y[3]*m_W[0],
		rhs.m_Y[0]*m_X[1]+rhs.m_Y[1]*m_Y[1]+rhs.m_Y[2]*m_Z[1]+rhs.m_Y[3]*m_W[1],
		rhs.m_Y[0]*m_X[2]+rhs.m_Y[1]*m_Y[2]+rhs.m_Y[2]*m_Z[2]+rhs.m_Y[3]*m_W[2],
		rhs.m_Y[0]*m_X[3]+rhs.m_Y[1]*m_Y[3]+rhs.m_Y[2]*m_Z[3]+rhs.m_Y[3]*m_W[3],
		rhs.m_Z[0]*m_X[0]+rhs.m_Z[1]*m_Y[0]+rhs.m_Z[2]*m_Z[0]+rhs.m_Z[3]*m_W[0],
		rhs.m_Z[0]*m_X[1]+rhs.m_Z[1]*m_Y[1]+rhs.m_Z[2]*m_Z[1]+rhs.m_Z[3]*m_W[1],
		rhs.m_Z[0]*m_X[2]+rhs.m_Z[1]*m_Y[2]+rhs.m_Z[2]*m_Z[2]+rhs.m_Z[3]*m_W[2],
		rhs.m_Z[0]*m_X[3]+rhs.m_Z[1]*m_Y[3]+rhs.m_Z[2]*m_Z[3]+rhs.m_Z[3]*m_W[3],
		rhs.m_W[0]*m_X[0]+rhs.m_W[1]*m_Y[0]+rhs.m_W[2]*m_Z[0]+rhs.m_W[3]*m_W[0],
		rhs.m_W[0]*m_X[1]+rhs.m_W[1]*m_Y[1]+rhs.m_W[2]*m_Z[1]+rhs.m_W[3]*m_W[1],
		rhs.m_W[0]*m_X[2]+rhs.m_W[1]*m_Y[2]+rhs.m_W[2]*m_Z[2]+rhs.m_W[3]*m_W[2],
		rhs.m_W[0]*m_X[3]+rhs.m_W[1]*m_Y[3]+rhs.m_W[2]*m_Z[3]+rhs.m_W[3]*m_W[3]
	);
}

SNML_INLINE void quadquad::MultQuat( vquad_arg rhs )
{
	float conj[ 4 ] = { -rhs.m_Value[ 0 ], -rhs.m_Value[ 1 ], -rhs.m_Value[ 2 ], rhs.m_Value[ 3 ] };
	float x[ 4 ], y[ 4 ], z[ 4 ];
	QMul( x, m_X, conj );
	QMul( y, m_Y, conj );
	QMul( z, m_Z, conj );
	QMul( m_X, rhs.m_Value, x );
	QMul( m_Y, rhs.m_Value, y );
	QMul( m_Z, rhs.m_Value, z );
}

SNML_INLINE void quadquad::Perspective( scalar_arg fieldOfView, scalar_arg aspectRatio, scalar_arg _near, scalar_arg _far )
{
	float cot = 1.0f / tanf( fieldOfView.m_Value * 0.5f * 3.14159265359f/180 );
	quadquad matrix(
		cot / aspectRatio.m_Value, 0, 0, 0,
		0, cot, 0, 0,
		0, 0, ( _near.m_Value + _far.m_Value ) / ( _near.m_Value - _far.m_Value ), -1,
		0, 0, 2 * _near.m_Value * _far.m_Value / ( _near.m_Value - _far.m_Value ), 0
	);
	MultMatrix( matrix );
}

SNML_INLINE void quadquad::Ortho( scalar_arg left, scalar_arg right, scalar_arg bottom, scalar_arg top, scalar_arg _near, scalar_arg _far )
{
	quadquad matrix(
		scalar::two()/(right - left), 0, 0, 0,
		0, scalar::two()/(top - bottom), 0, 0,
		0, 0, scalar::two()/(_near - _far), 0,
		-(right + left)/(right - left), -(top+ bottom)/(top - bottom), (_near + _far)/(_near - _far), 1
	);
	MultMatrix( matrix );
}

SNML_INLINE void quadquad::LookAt( vquad_arg eyePos, vquad_arg center, vquad_arg up )
{
	vquad z = eyePos - center, x = cross( up, z ), y = cross( z, x );
	x *= rlength3( x );
	y *= rlength3( y );
	z *= rlength3( z );
	quadquad matrix(
		x.m_Value[ 0 ], y.m_Value[ 0 ], z.m_Value[ 0 ], 0,
		x.m_Value[ 1 ], y.m_Value[ 1 ], z.m_Value[ 1 ], 0,
		x.m_Value[ 2 ], y.m_Value[ 2 ], z.m_Value[ 2 ], 0,
		-eyePos.m_Value[ 0 ] * x.m_Value[ 0 ] - eyePos.m_Value[ 1 ] * x.m_Value[ 1 ] - eyePos.m_Value[ 2 ] * x.m_Value[ 2 ],
		-eyePos.m_Value[ 0 ] * y.m_Value[ 0 ] - eyePos.m_Value[ 1 ] * y.m_Value[ 1 ] - eyePos.m_Value[ 2 ] * y.m_Value[ 2 ],
		-eyePos.m_Value[ 0 ] * z.m_Value[ 0 ] - eyePos.m_Value[ 1 ] * z.m_Value[ 1 ] - eyePos.m_Value[ 2 ] * z.m_Value[ 2 ],
		1
	);
	MultMatrix( matrix );
}

SNML_INLINE quadquad &Inverse4x4( quadquad &result, quadquad &rhs )
{
	result.load(
		rhs.m_Y[1]*rhs.m_Z[2]*rhs.m_W[3]-rhs.m_Y[1]*rhs.m_Z[3]*rhs.m_W[2]+rhs.m_Z[1]*rhs.m_W[2]*rhs.m_Y[3]-rhs.m_Z[1]*rhs.m_Y[2]*rhs.m_W[3]+rhs.m_W[1]*rhs.m_Y[2]*rhs.m_Z[3]-rhs.m_W[1]*rhs.m_Z[2]*rhs.m_Y[3],
		-rhs.m_X[1]*rhs.m_Z[2]*rhs.m_W[3]+rhs.m_X[1]*rhs.m_Z[3]*rhs.m_W[2]-rhs.m_Z[1]*rhs.m_W[2]*rhs.m_X[3]+rhs.m_Z[1]*rhs.m_X[2]*rhs.m_W[3]-rhs.m_W[1]*rhs.m_X[2]*rhs.m_Z[3]+rhs.m_W[1]*rhs.m_Z[2]*rhs.m_X[3],
		rhs.m_X[1]*rhs.m_Y[2]*rhs.m_W[3]-rhs.m_X[1]*rhs.m_W[2]*rhs.m_Y[3]+rhs.m_Y[1]*rhs.m_W[2]*rhs.m_X[3]-rhs.m_Y[1]*rhs.m_X[2]*rhs.m_W[3]+rhs.m_W[1]*rhs.m_X[2]*rhs.m_Y[3]-rhs.m_W[1]*rhs.m_Y[2]*rhs.m_X[3],
		-rhs.m_X[1]*rhs.m_Y[2]*rhs.m_Z[3]+rhs.m_X[1]*rhs.m_Z[2]*rhs.m_Y[3]-rhs.m_Y[1]*rhs.m_Z[2]*rhs.m_X[3]+rhs.m_Y[1]*rhs.m_X[2]*rhs.m_Z[3]-rhs.m_Z[1]*rhs.m_X[2]*rhs.m_Y[3]+rhs.m_Z[1]*rhs.m_Y[2]*rhs.m_X[3],

		-rhs.m_Y[0]*rhs.m_Z[2]*rhs.m_W[3]+rhs.m_Y[0]*rhs.m_Z[3]*rhs.m_W[2]-rhs.m_Z[0]*rhs.m_W[2]*rhs.m_Y[3]+rhs.m_Z[0]*rhs.m_Y[2]*rhs.m_W[3]-rhs.m_W[0]*rhs.m_Y[2]*rhs.m_Z[3]+rhs.m_W[0]*rhs.m_Z[2]*rhs.m_Y[3],
		rhs.m_X[0]*rhs.m_Z[2]*rhs.m_W[3]-rhs.m_X[0]*rhs.m_Z[3]*rhs.m_W[2]+rhs.m_Z[0]*rhs.m_W[2]*rhs.m_X[3]-rhs.m_Z[0]*rhs.m_X[2]*rhs.m_W[3]+rhs.m_W[0]*rhs.m_X[2]*rhs.m_Z[3]-rhs.m_W[0]*rhs.m_Z[2]*rhs.m_X[3],
		-rhs.m_X[0]*rhs.m_Y[2]*rhs.m_W[3]+rhs.m_X[0]*rhs.m_W[2]*rhs.m_Y[3]-rhs.m_Y[0]*rhs.m_W[2]*rhs.m_X[3]+rhs.m_Y[0]*rhs.m_X[2]*rhs.m_W[3]-rhs.m_W[0]*rhs.m_X[2]*rhs.m_Y[3]+rhs.m_W[0]*rhs.m_Y[2]*rhs.m_X[3],
		rhs.m_X[0]*rhs.m_Y[2]*rhs.m_Z[3]-rhs.m_X[0]*rhs.m_Z[2]*rhs.m_Y[3]+rhs.m_Y[0]*rhs.m_Z[2]*rhs.m_X[3]-rhs.m_Y[0]*rhs.m_X[2]*rhs.m_Z[3]+rhs.m_Z[0]*rhs.m_X[2]*rhs.m_Y[3]-rhs.m_Z[0]*rhs.m_Y[2]*rhs.m_X[3],

		rhs.m_Y[0]*rhs.m_Z[1]*rhs.m_W[3]-rhs.m_Y[0]*rhs.m_Z[3]*rhs.m_W[1]+rhs.m_Z[0]*rhs.m_W[1]*rhs.m_Y[3]-rhs.m_Z[0]*rhs.m_Y[1]*rhs.m_W[3]+rhs.m_W[0]*rhs.m_Y[1]*rhs.m_Z[3]-rhs.m_W[0]*rhs.m_Z[1]*rhs.m_Y[3],
		-rhs.m_X[0]*rhs.m_Z[1]*rhs.m_W[3]+rhs.m_X[0]*rhs.m_Z[3]*rhs.m_W[1]-rhs.m_Z[0]*rhs.m_W[1]*rhs.m_X[3]+rhs.m_Z[0]*rhs.m_X[1]*rhs.m_W[3]-rhs.m_W[0]*rhs.m_X[1]*rhs.m_Z[3]+rhs.m_W[0]*rhs.m_Z[1]*rhs.m_X[3],
		rhs.m_X[0]*rhs.m_Y[1]*rhs.m_W[3]-rhs.m_X[0]*rhs.m_W[1]*rhs.m_Y[3]+rhs.m_Y[0]*rhs.m_W[1]*rhs.m_X[3]-rhs.m_Y[0]*rhs.m_X[1]*rhs.m_W[3]+rhs.m_W[0]*rhs.m_X[1]*rhs.m_Y[3]-rhs.m_W[0]*rhs.m_Y[1]*rhs.m_X[3],
		-rhs.m_X[0]*rhs.m_Y[1]*rhs.m_Z[3]+rhs.m_X[0]*rhs.m_Z[1]*rhs.m_Y[3]-rhs.m_Y[0]*rhs.m_Z[1]*rhs.m_X[3]+rhs.m_Y[0]*rhs.m_X[1]*rhs.m_Z[3]-rhs.m_Z[0]*rhs.m_X[1]*rhs.m_Y[3]+rhs.m_Z[0]*rhs.m_Y[1]*rhs.m_X[3],

		-rhs.m_Y[0]*rhs.m_Z[1]*rhs.m_W[2]+rhs.m_Y[0]*rhs.m_Z[2]*rhs.m_W[1]-rhs.m_Z[0]*rhs.m_W[1]*rhs.m_Y[2]+rhs.m_Z[0]*rhs.m_Y[1]*rhs.m_W[2]-rhs.m_W[0]*rhs.m_Y[1]*rhs.m_Z[2]+rhs.m_W[0]*rhs.m_Z[1]*rhs.m_Y[2],
		rhs.m_X[0]*rhs.m_Z[1]*rhs.m_W[2]-rhs.m_X[0]*rhs.m_Z[2]*rhs.m_W[1]+rhs.m_Z[0]*rhs.m_W[1]*rhs.m_X[2]-rhs.m_Z[0]*rhs.m_X[1]*rhs.m_W[2]+rhs.m_W[0]*rhs.m_X[1]*rhs.m_Z[2]-rhs.m_W[0]*rhs.m_Z[1]*rhs.m_X[2],
		-rhs.m_X[0]*rhs.m_Y[1]*rhs.m_W[2]+rhs.m_X[0]*rhs.m_W[1]*rhs.m_Y[2]-rhs.m_Y[0]*rhs.m_W[1]*rhs.m_X[2]+rhs.m_Y[0]*rhs.m_X[1]*rhs.m_W[2]-rhs.m_W[0]*rhs.m_X[1]*rhs.m_Y[2]+rhs.m_W[0]*rhs.m_Y[1]*rhs.m_X[2],
		rhs.m_X[0]*rhs.m_Y[1]*rhs.m_Z[2]-rhs.m_X[0]*rhs.m_Z[1]*rhs.m_Y[2]+rhs.m_Y[0]*rhs.m_Z[1]*rhs.m_X[2]-rhs.m_Y[0]*rhs.m_X[1]*rhs.m_Z[2]+rhs.m_Z[0]*rhs.m_X[1]*rhs.m_Y[2]-rhs.m_Z[0]*rhs.m_Y[1]*rhs.m_X[2]
	);
	scalar det = rhs.m_X[ 0 ] * result.m_X[ 0 ] + rhs.m_Y[ 0 ] * result.m_X[ 1 ] + rhs.m_Z[ 0 ] * result.m_X[ 2 ] + rhs.m_W[ 0 ] * result.m_X[ 3 ];
	result *= recip( det );
	return result;
}

SNML_INLINE quadquad &Transpose4x4( quadquad &result, quadquad &rhs )
{
	result.load(
		rhs.m_X[ 0 ], rhs.m_Y[ 0 ], rhs.m_Z[ 0 ], rhs.m_W[ 0 ],
		rhs.m_X[ 1 ], rhs.m_Y[ 1 ], rhs.m_Z[ 1 ], rhs.m_W[ 1 ],
		rhs.m_X[ 2 ], rhs.m_Y[ 2 ], rhs.m_Z[ 2 ], rhs.m_W[ 2 ],
		rhs.m_X[ 3 ], rhs.m_Y[ 3 ], rhs.m_Z[ 3 ], rhs.m_W[ 3 ]
	);
	return result;
}

SNML_INLINE scalar Determinant4x4( quadquad & rhs )
{
	vquad x(
		rhs.m_Y[1]*rhs.m_Z[2]*rhs.m_W[3]-rhs.m_Y[1]*rhs.m_Z[3]*rhs.m_W[2]+rhs.m_Z[1]*rhs.m_W[2]*rhs.m_Y[3]-rhs.m_Z[1]*rhs.m_Y[2]*rhs.m_W[3]+rhs.m_W[1]*rhs.m_Y[2]*rhs.m_Z[3]-rhs.m_W[1]*rhs.m_Z[2]*rhs.m_Y[3],
		-rhs.m_X[1]*rhs.m_Z[2]*rhs.m_W[3]+rhs.m_X[1]*rhs.m_Z[3]*rhs.m_W[2]-rhs.m_Z[1]*rhs.m_W[2]*rhs.m_X[3]+rhs.m_Z[1]*rhs.m_X[2]*rhs.m_W[3]-rhs.m_W[1]*rhs.m_X[2]*rhs.m_Z[3]+rhs.m_W[1]*rhs.m_Z[2]*rhs.m_X[3],
		rhs.m_X[1]*rhs.m_Y[2]*rhs.m_W[3]-rhs.m_X[1]*rhs.m_W[2]*rhs.m_Y[3]+rhs.m_Y[1]*rhs.m_W[2]*rhs.m_X[3]-rhs.m_Y[1]*rhs.m_X[2]*rhs.m_W[3]+rhs.m_W[1]*rhs.m_X[2]*rhs.m_Y[3]-rhs.m_W[1]*rhs.m_Y[2]*rhs.m_X[3],
		-rhs.m_X[1]*rhs.m_Y[2]*rhs.m_Z[3]+rhs.m_X[1]*rhs.m_Z[2]*rhs.m_Y[3]-rhs.m_Y[1]*rhs.m_Z[2]*rhs.m_X[3]+rhs.m_Y[1]*rhs.m_X[2]*rhs.m_Z[3]-rhs.m_Z[1]*rhs.m_X[2]*rhs.m_Y[3]+rhs.m_Z[1]*rhs.m_Y[2]*rhs.m_X[3]
	);
	return rhs.m_X[ 0 ] * x.m_Value[ 0 ] + rhs.m_Y[ 0 ] * x.m_Value[ 1 ] + rhs.m_Z[ 0 ] * x.m_Value[ 2 ] + rhs.m_W[ 0 ] * x.m_Value[ 3 ];
}

SNML_INLINE quadquad &Inverse3x4( quadquad &result, quadquad &rhs )
{
	result.load(
		rhs.m_Z[ 2 ]*rhs.m_Y[ 1 ]-rhs.m_Z[ 1 ]*rhs.m_Y[ 2 ], rhs.m_Z[ 1 ]*rhs.m_X[ 2 ]-rhs.m_Z[ 2 ]*rhs.m_X[ 1 ], rhs.m_X[ 1 ]*rhs.m_Y[ 2 ]-rhs.m_X[ 2 ]*rhs.m_Y[ 1 ], 0,
		rhs.m_Z[ 0 ]*rhs.m_Y[ 2 ]-rhs.m_Z[ 2 ]*rhs.m_Y[ 0 ], -rhs.m_Z[ 0 ]*rhs.m_X[ 2 ]+rhs.m_Z[ 2 ]*rhs.m_X[ 0 ], -rhs.m_Y[ 2 ]*rhs.m_X[ 0 ]+rhs.m_Y[ 0 ]*rhs.m_X[ 2 ], 0,
		-rhs.m_Z[ 0 ]*rhs.m_Y[ 1 ]+rhs.m_Z[ 1 ]*rhs.m_Y[ 0 ], -rhs.m_Z[ 1 ]*rhs.m_X[ 0 ]+rhs.m_Z[ 0 ]*rhs.m_X[ 1 ], rhs.m_Y[ 1 ]*rhs.m_X[ 0 ]-rhs.m_Y[ 0 ]*rhs.m_X[ 1 ], 0,
		-rhs.m_Y[ 1 ]*rhs.m_W[ 0 ]*rhs.m_Z[ 2 ]+rhs.m_Y[ 1 ]*rhs.m_W[ 2 ]*rhs.m_Z[ 0 ]+rhs.m_W[ 0 ]*rhs.m_Z[ 1 ]*rhs.m_Y[ 2 ]-rhs.m_W[ 2 ]*rhs.m_Z[ 1 ]*rhs.m_Y[ 0 ]-rhs.m_W[ 1 ]*rhs.m_Y[ 2 ]*rhs.m_Z[ 0 ]+rhs.m_Y[ 0 ]*rhs.m_W[ 1 ]*rhs.m_Z[ 2 ],
		rhs.m_X[ 0 ]*rhs.m_W[ 2 ]*rhs.m_Z[ 1 ]-rhs.m_X[ 0 ]*rhs.m_W[ 1 ]*rhs.m_Z[ 2 ]+rhs.m_W[ 1 ]*rhs.m_Z[ 0 ]*rhs.m_X[ 2 ]-rhs.m_W[ 0 ]*rhs.m_X[ 2 ]*rhs.m_Z[ 1 ]+rhs.m_W[ 0 ]*rhs.m_X[ 1 ]*rhs.m_Z[ 2 ]-rhs.m_W[ 2 ]*rhs.m_Z[ 0 ]*rhs.m_X[ 1 ],
		-rhs.m_W[ 2 ]*rhs.m_X[ 0 ]*rhs.m_Y[ 1 ]+rhs.m_W[ 2 ]*rhs.m_Y[ 0 ]*rhs.m_X[ 1 ]+rhs.m_W[ 0 ]*rhs.m_X[ 2 ]*rhs.m_Y[ 1 ]+rhs.m_W[ 1 ]*rhs.m_X[ 0 ]*rhs.m_Y[ 2 ]-rhs.m_W[ 1 ]*rhs.m_Y[ 0 ]*rhs.m_X[ 2 ]-rhs.m_W[ 0 ]*rhs.m_X[ 1 ]*rhs.m_Y[ 2 ],
		rhs.m_Z[ 2 ]*rhs.m_X[ 0 ]*rhs.m_Y[ 1 ]-rhs.m_Z[ 2 ]*rhs.m_Y[ 0 ]*rhs.m_X[ 1 ]-rhs.m_Z[ 0 ]*rhs.m_X[ 2 ]*rhs.m_Y[ 1 ]-rhs.m_Z[ 1 ]*rhs.m_X[ 0 ]*rhs.m_Y[ 2 ]+rhs.m_Z[ 1 ]*rhs.m_Y[ 0 ]*rhs.m_X[ 2 ]+rhs.m_Z[ 0 ]*rhs.m_X[ 1 ]*rhs.m_Y[ 2 ]
	);
	scalar det = rhs.m_X[ 0 ] * result.m_X[ 0 ] + rhs.m_Y[ 0 ] * result.m_X[ 1 ] + rhs.m_Z[ 0 ] * result.m_X[ 2 ];
	result *= recip( det );
	return result;
}

SNML_INLINE scalar Determinant3x4( quadquad & rhs )
{
	vquad x(
		rhs.m_Z[ 2 ]*rhs.m_Y[ 1 ]-rhs.m_Z[ 1 ]*rhs.m_Y[ 2 ], rhs.m_Z[ 1 ]*rhs.m_X[ 2 ]-rhs.m_Z[ 2 ]*rhs.m_X[ 1 ], rhs.m_X[ 1 ]*rhs.m_Y[ 2 ]-rhs.m_X[ 2 ]*rhs.m_Y[ 1 ], 0
	);
	return rhs.m_X[ 0 ] * x.m_Value[ 0 ] + rhs.m_Y[ 0 ] * x.m_Value[ 1 ] + rhs.m_Z[ 0 ] * x.m_Value[ 2 ];
}

SNML_INLINE quadquad &OuterProduct4x4( quadquad &result, vquad_arg lhs, vquad_arg rhs )
{
	result.load( lhs * rhs.x(), lhs * rhs.y(), lhs * rhs.z(), lhs * rhs.w() );
	return result;
}

// eigenvalues and eigenvectors
// eigenvectors must be unit on entry, call multiple times to refine
SNML_INLINE vquad Eigenvalues3x3( quadquad &eigenvectors, quadquad &rhs )
{
	vquad xy, xz, yz;
	xy = Jacobi( rhs, 0, 1 );
	xz = Jacobi( rhs, 0, 2 );
	yz = Jacobi( rhs, 1, 2 );
	vquad result( rhs.x().x(), rhs.y().y(), rhs.z().z(), scalar( 0 ) );
	MakeEigenvectors( eigenvectors, 0, 1, xy );
	MakeEigenvectors( eigenvectors, 0, 2, xz );
	MakeEigenvectors( eigenvectors, 1, 2, yz );
	return result;
}

SNML_INLINE const NativeScalar &scalar::access() const
{
	return m_Value;
}

SNML_INLINE const NativeQuad &vquad::access() const
{
	return m_Value;
}

SNML_INLINE const NativeQuad &quadquad::access_x() const
{
	return m_X;
}

SNML_INLINE const NativeQuad &quadquad::access_y() const
{
	return m_Y;
}

SNML_INLINE const NativeQuad &quadquad::access_z() const
{
	return m_Z;
}

// debug support functions now on a switch
// requires string.h and stdio.h
#ifdef SNML_ENABLE_DEBUG

SNML_NOINLINE const char *scalar::ToString( const char *pFmt ) const
{
	static char buffers[ 16 ][ 16 ];
	static int buffer_index;
	char *pDest = buffers[ buffer_index & 15 ];
	sprintf( pDest, pFmt, (float)*this );
	return buffers[ buffer_index++ & 15 ];
}

SNML_NOINLINE const char *vquad::ToString( const char *pFmt ) const
{
	static char buffers[ 4 ][ 64 ];
	static int buffer_index;
	char *pDest = buffers[ buffer_index & 3 ];
	*pDest++ = '{'; *pDest++ = ' ';
	strcpy( pDest, x().ToString( pFmt ) ); pDest += strlen( pDest );
	*pDest++ = ','; *pDest++ = ' ';
	strcpy( pDest, y().ToString( pFmt ) ); pDest += strlen( pDest );
	*pDest++ = ','; *pDest++ = ' ';
	strcpy( pDest, z().ToString( pFmt ) ); pDest += strlen( pDest );
	*pDest++ = ','; *pDest++ = ' ';
	strcpy( pDest, w().ToString( pFmt ) ); pDest += strlen( pDest );
	*pDest++ = ' '; *pDest++ = '}'; *pDest++ = 0;
	return buffers[ buffer_index++ & 3 ];
}

SNML_NOINLINE const char *quadquad::ToString( const char *pFmt ) const
{
	static char buffer[ 256 ];
	char *pDest = buffer;
	*pDest++ = '{'; *pDest++ = ' ';
	strcpy( pDest, x().ToString( pFmt ) ); pDest += strlen( pDest );
	*pDest++ = ','; *pDest++ = ' ';
	strcpy( pDest, y().ToString( pFmt ) ); pDest += strlen( pDest );
	*pDest++ = ','; *pDest++ = ' ';
	strcpy( pDest, z().ToString( pFmt ) ); pDest += strlen( pDest );
	*pDest++ = ','; *pDest++ = ' ';
	strcpy( pDest, w().ToString( pFmt ) ); pDest += strlen( pDest );
	*pDest++ = ' '; *pDest++ = '}'; *pDest++ = 0;
	return buffer;
}

#endif // SNML_ENABLE_DEBUG

#endif
