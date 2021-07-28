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
//  Dependencies:
//     explict: SnMathLibDef.h
//     implicit: stdio.h, string.h (not included for compile performance)
//
//  Change History:
//
//  Vers    Date            Author        Changes
//  1.00    22-08-2005      AndyT         Created
//
////////////////////////////////////////////////////////////////////////////////

// scalars are vectors with all values the same
typedef vector float NativeFloat;
typedef vector signed int NativeInt;
typedef vector signed short NativeShort;
typedef vector unsigned int NativeUnsigned;
typedef vector unsigned char NativeUnsignedChar;
typedef vector unsigned short NativeUnsignedShort;

typedef NativeFloat NativeScalar;
typedef NativeFloat NativeQuad;

class scalar;
class vquad;
class quadquad;

typedef const scalar &scalar_arg;
typedef const vquad &vquad_arg;
typedef const quadquad &quadquad_arg;

#include <sn/SnMathLibDef.h>

#ifndef SN_MATHLIB_DEFINE_ONLY

#undef _K4
#ifdef __GCC__
	#define __ALTIVEC__
	#include <altivec.h>
	#define _K4( Type, X, Y, Z, W ) (Type){ X, Y, Z, W }
	#undef __ALTIVEC__
#else
	#define _K4( Type, X, Y, Z, W ) ((Type)( X, Y, Z, W ))
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  These helper functions are for implementation only and are subject to change
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace snh
{
	// generic permutation
	template < unsigned pc > SNML_INLINE NativeFloat perm( NativeFloat xyzw )
	{
		return vec_perm( xyzw, xyzw, (NativeUnsignedChar)_K4( NativeUnsigned,
			( ( pc >> 24 ) & 3 ) * 0x4040404 + ( pc & 0x20000000 ? 0x00010203 : 0x10111213 ),
			( ( pc >> 16 ) & 3 ) * 0x4040404 + ( pc & 0x00200000 ? 0x00010203 : 0x10111213 ),
			( ( pc >> 8  ) & 3 ) * 0x4040404 + ( pc & 0x00002000 ? 0x00010203 : 0x10111213 ),
			( ( pc       ) & 3 ) * 0x4040404 + ( pc & 0x00000020 ? 0x00010203 : 0x10111213 )
		) );
	}

	template < unsigned pc > SNML_INLINE NativeFloat perm( NativeFloat xyzw, NativeFloat abcd )
	{
	      return vec_perm( xyzw, abcd, (NativeUnsignedChar)_K4( NativeUnsigned,
			( ( pc >> 24 ) & 3 ) * 0x4040404 + ( pc & 0x20000000 ? 0x00010203 : 0x10111213 ),
			( ( pc >> 16 ) & 3 ) * 0x4040404 + ( pc & 0x00200000 ? 0x00010203 : 0x10111213 ),
			( ( pc >> 8  ) & 3 ) * 0x4040404 + ( pc & 0x00002000 ? 0x00010203 : 0x10111213 ),
			( ( pc       ) & 3 ) * 0x4040404 + ( pc & 0x00000020 ? 0x00010203 : 0x10111213 )
	      ) );
	}

	SNML_INLINE NativeFloat x( NativeFloat xyzw ) { return vec_splat( xyzw, 0 ); }
	SNML_INLINE NativeFloat y( NativeFloat xyzw ) { return vec_splat( xyzw, 1 ); }
	SNML_INLINE NativeFloat z( NativeFloat xyzw ) { return vec_splat( xyzw, 2 ); }
	SNML_INLINE NativeFloat w( NativeFloat xyzw ) { return vec_splat( xyzw, 3 ); }
	SNML_INLINE NativeFloat zwXY( NativeFloat xyzw, NativeFloat abcd ) { return vec_sld( xyzw, abcd, 8 ); }
	SNML_INLINE NativeFloat xXyY( NativeFloat xyzw, NativeFloat abcd ) { return vec_mergel( xyzw, abcd ); }
	SNML_INLINE NativeFloat zZwW( NativeFloat xyzw, NativeFloat abcd ) { return vec_mergeh( xyzw, abcd ); }

	SNML_INLINE NativeFloat negzero()
	{
		return (NativeFloat)(NativeUnsigned)( 0x80000000 );;
	}

	SNML_INLINE NativeFloat zero()
	{
		return (NativeFloat)0;
	}

	SNML_INLINE NativeFloat one()
	{
		return (NativeFloat)1;
	}

	SNML_INLINE NativeFloat half()
	{
		return (NativeFloat)0.5f;
	}

	SNML_INLINE NativeFloat mul( NativeFloat x, NativeFloat y )
	{
		return vec_madd( x, y, zero() );
	}

	SNML_INLINE NativeFloat madd( NativeFloat x, NativeFloat y, NativeFloat z )
	{
		return vec_madd( x, y, z );
	}

	SNML_INLINE NativeFloat nmsub( NativeFloat x, NativeFloat y, NativeFloat z )
	{
		return vec_nmsub( x, y, z );
	}

	SNML_INLINE NativeFloat add( NativeFloat x, NativeFloat y )
	{
		return vec_add( x, y );
	}

	SNML_INLINE NativeFloat sub( NativeFloat x, NativeFloat y )
	{
		return vec_sub( x, y );
	}

	SNML_INLINE NativeFloat min( NativeFloat x, NativeFloat y )
	{
		return vec_min( x, y );
	}

	SNML_INLINE NativeFloat max( NativeFloat x, NativeFloat y )
	{
		return vec_max( x, y );
	}

	template < typename Type > SNML_INLINE Type sel( Type x, Type y, NativeUnsigned selector )
	{
		return vec_sel( x, y, selector );
	}

	SNML_INLINE NativeFloat floor( NativeFloat x )
	{
		return vec_floor( x );
	}

        // these functions must be inlined, even in debug mode, hence SNML_INLINE2
	SNML_INLINE2 NativeInt cts( NativeFloat x, const int point )
	{
		return vec_cts( x, point );
	}

	SNML_INLINE2 NativeUnsigned ctu( NativeFloat x, const int point )
	{
		return vec_ctu( x, point );
	}

	SNML_INLINE2 NativeFloat ctf( NativeInt x, const int point )
	{
		return vec_ctf( x, point );
	}

	SNML_INLINE NativeFloat min4( NativeFloat value )
	{
		return min( min( x( value ), y( value ) ), min( z( value ), w( value ) ) );
	}

	SNML_INLINE NativeFloat max4( NativeFloat value )
	{
		return max( max( x( value ), y( value ) ), max( z( value ), w( value ) ) );
	}

	SNML_INLINE NativeFloat sum4( NativeFloat value )
	{
		return add( add( x( value ), y( value ) ), add( z( value ), w( value ) ) );
	}

	SNML_INLINE NativeFloat dot4( NativeFloat x, NativeFloat y )
	{
		return sum4( mul( x, y ) );
	}

	template < typename Type > SNML_INLINE Type sgn( Type x )
	{
		NativeFloat zero = snh::zero();
		return sel( vec_or( vec_and( x, (Type)negzero() ), (Type)one() ), zero, (NativeUnsigned)vec_cmpeq( x, zero ) );
	}

	template < typename Type > SNML_INLINE Type abs( Type x )
	{
		return ( vec_andc( x, Type( NativeUnsigned( 0x80000000 ) ) ) );
	}

	SNML_INLINE NativeFloat copysign( NativeFloat x, NativeFloat y )
	{
		return sel( x, y, (NativeUnsigned)( 0x80000000 ) );
	}

	SNML_INLINE NativeFloat mulsign( NativeFloat x, NativeFloat y )
	{
		return vec_xor( x, vec_and( y, negzero() ) );
	}

	SNML_INLINE NativeFloat select_ge( NativeFloat x, NativeFloat y, NativeFloat t, NativeFloat f )
	{
		return sel( f, t, (NativeUnsigned)vec_cmpge( x, y ) );
	}

	SNML_INLINE NativeFloat select_gt( NativeFloat x, NativeFloat y, NativeFloat t, NativeFloat f )
	{
		return sel( f, t, (NativeUnsigned)vec_cmpgt( x, y ) );
	}

	SNML_INLINE NativeFloat select_le( NativeFloat x, NativeFloat y, NativeFloat t, NativeFloat f )
	{
		return sel( f, t, (NativeUnsigned)vec_cmple( x, y ) );
	}

	SNML_INLINE NativeFloat select_lt( NativeFloat x, NativeFloat y, NativeFloat t, NativeFloat f )
	{
		return sel( f, t, (NativeUnsigned)vec_cmplt( x, y ) );
	}

	SNML_INLINE NativeFloat select_eq( NativeFloat x, NativeFloat y, NativeFloat t, NativeFloat f )
	{
		return sel( f, t, (NativeUnsigned)vec_cmpeq( x, y ) );
	}

	SNML_INLINE NativeFloat select_ne( NativeFloat x, NativeFloat y, NativeFloat t, NativeFloat f )
	{
		return sel( t, f, (NativeUnsigned)vec_cmpeq( x, y ) );
	}

	SNML_INLINE NativeFloat select( bool sel, NativeFloat t, NativeFloat f )
	{
		return sel ? t : f;
	}

	SNML_INLINE NativeFloat rsqrt( NativeFloat x )
	{
		NativeFloat zero = snh::zero();
		NativeFloat estimate = vec_rsqrte( x ); 
		NativeFloat estimateSquared = madd( estimate, estimate, zero );
		NativeFloat halfEstimate = madd( estimate, half(), zero );
		return madd( nmsub( x, estimateSquared, one() ), halfEstimate, estimate );
	}

	SNML_INLINE NativeFloat sqrt( NativeFloat x )
	{
		NativeFloat zero = snh::zero();
		return select_eq( x, zero, zero, madd( rsqrt( x ), x, zero ) );
	}

	SNML_INLINE NativeFloat recip( NativeFloat x )
	{
		NativeFloat estimate = vec_re( x ); 
		return madd( nmsub( estimate, x, one() ), estimate, estimate );
	}

	SNML_INLINE NativeFloat polynomial( NativeFloat x, NativeFloat c4, NativeFloat c3, NativeFloat c2, NativeFloat c1, NativeFloat c0 )
	{
		// shorten critical path by evaluating even & odd terms separately
		NativeFloat x2 = mul( x, x ); // stage 1
		NativeFloat even = madd( x2, madd( x2, c4, c2 ), c0 ); // stage 2, 3
		NativeFloat odd = madd( x2, c3, c1 );
		return madd( x, odd, even ); // stage 4

		// equivalent of
		//return madd( x, madd(x, madd(x, madd( x, c5, c4 ), c3 ), c2 ), c1 ), c0 );
	}

	SNML_INLINE NativeFloat polynomial( NativeFloat x, NativeFloat c5, NativeFloat c4, NativeFloat c3, NativeFloat c2, NativeFloat c1, NativeFloat c0 )
	{
		// shorten critical path by evaluating even & odd terms separately
		NativeFloat x2 = mul( x, x ); // stage 1
		NativeFloat even = madd( x2, madd( x2, c4, c2 ), c0 ); // stage 2, 3
		NativeFloat odd = madd( x2, madd( x2, c5, c3 ), c1 );
		return madd( x, odd, even ); // stage 4

		// equivalent of
		//return madd( x, madd(x, madd(x, madd( x, madd( x, c5, c4 ), c3 ), c2 ), c1 ), c0 );
	}

	SNML_INLINE NativeFloat polynomial( NativeFloat x, NativeFloat c7, NativeFloat c6, NativeFloat c5, NativeFloat c4, NativeFloat c3, NativeFloat c2, NativeFloat c1, NativeFloat c0 )
	{
		// shorten critical path by evaluating mod 4 terms separately
		NativeFloat x2 = mul( x, x ); // stage 1
		NativeFloat x4 = mul( x2, x2 ); // stage 2
		NativeFloat r0 = madd( x4, c4, c0 );  // stage 3
		NativeFloat r1 = madd( x4, c5, c1 );
		NativeFloat r2 = madd( x4, c6, c2 );
		NativeFloat r3 = madd( x4, c7, c3 );
		NativeFloat p0 = madd( x2, r2, r0 );  // stage 4
		NativeFloat p1 = madd( x2, r3, r1 );
		return madd( x, p1, p0 );  // stage 5
	}

	SNML_INLINE NativeFloat polynomial( NativeFloat x, NativeFloat c8, NativeFloat c7, NativeFloat c6, NativeFloat c5, NativeFloat c4, NativeFloat c3, NativeFloat c2, NativeFloat c1, NativeFloat c0 )
	{
		return madd( x, polynomial( x, c8, c7, c6, c5, c4, c3, c2, c1 ), c0 );
	}

	// see Sincos.mws
	SNML_INLINE NativeFloat sin( NativeFloat x )
	{
		//  get the values in range -Pi..Pi
		x = madd( x, (NativeFloat)(1.0f/6.283185308f), (NativeFloat)(0.25f) );
		x = sub( x, floor( x ) );
		x = madd( x, (NativeFloat)(6.283185308f), (NativeFloat)(-3.14159265359f) );
		NativeFloat x2 = mul( x, x );
		return snh::polynomial( x2, (NativeFloat)-.21978883928313581234e-6f, (NativeFloat).24204401591876647679e-4f, (NativeFloat)-.13858915967097865747e-2f, (NativeFloat).41659822210124139548e-1f, (NativeFloat)-.49999426811983243951f, (NativeFloat).99999922157308725937f );
	}

	SNML_INLINE NativeFloat cos( NativeFloat x )
	{
		//  get the values in range -Pi..Pi
		x = madd( x, (NativeFloat)(1.0f/6.283185308f), (NativeFloat)(0.5f) );
		x = sub( x, floor( x ) );
		x = madd( x, (NativeFloat)(6.283185308f), (NativeFloat)(-3.14159265359f) );
		NativeFloat x2 = mul( x, x );
		return snh::polynomial( x2, (NativeFloat)-.21978883928313581234e-6f, (NativeFloat).24204401591876647679e-4f, (NativeFloat)-.13858915967097865747e-2f, (NativeFloat).41659822210124139548e-1f, (NativeFloat)-.49999426811983243951f, (NativeFloat).99999922157308725937f );
	}

	SNML_INLINE NativeFloat tan( NativeFloat x )
	{
		x = madd( x, (NativeFloat)(1.0f/3.14159265f), (NativeFloat)(0.5) );
		x = sub( x, floor( x ) );
		x = madd( x, (NativeFloat)(3.14159265f/1.0f), (NativeFloat)(-3.14159265f/2.0f) );
		NativeFloat x2 = mul( x, x );
		NativeFloat xcotx = snh::polynomial( x2, (NativeFloat)(-.49612895233134499032e-4),(NativeFloat)(-.11880582746917291899e-3), (NativeFloat)(-.22463776739734214684e-2), (NativeFloat)(-.22142332086591302020e-1), (NativeFloat)(-.33335107248808522612), (NativeFloat)(1.0000006359937767446) );
		return mul( x, recip( xcotx ) );
	}

	SNML_INLINE NativeFloat asin( NativeFloat x )
	{
		NativeFloat original_x = x;
		x = sqrt( sub( one(), abs( x ) ) );
		NativeFloat asinx = snh::polynomial( x, (NativeFloat)(-.1170043127), (NativeFloat)(.3539238299), (NativeFloat)(-.4808809029), (NativeFloat)(.3145255403), (NativeFloat)(-.1358923281), (NativeFloat)(-.881873602e-1), (NativeFloat)(-.31971025e-2), (NativeFloat)(-1.414081910), (NativeFloat)(1.570795437) );
		return copysign( asinx, original_x );
	}

	SNML_INLINE NativeFloat acos( NativeFloat x )
	{
		return sub( (NativeFloat)(1.5707963267948966192f), asin( x ) );
	}

	SNML_INLINE NativeFloat pow2( NativeFloat x )
	{
		NativeFloat floor = snh::floor( x );
		NativeFloat exponent = vec_expte( floor );
		x = sub( x, floor );
		NativeFloat pow2x = snh::polynomial( x, (NativeFloat)(.1896388120e-2),(NativeFloat)(.8943010371e-2), (NativeFloat)(.5586608678e-1), (NativeFloat)(.2401397705), (NativeFloat)(.6931547443), (NativeFloat)(.9999998931) );
		return mul( exponent, pow2x );
	}

	SNML_INLINE NativeFloat log2( NativeFloat x )
	{
		NativeFloat exponent = floor( vec_loge( x ) );
		x = sel( x, one(), (NativeUnsigned)( 0xff800000 ) );
		x = snh::polynomial( x, (NativeFloat)(.1512526932e-1), (NativeFloat)(-.1839387520), (NativeFloat)(.9780911032), (NativeFloat)(-2.985012428), (NativeFloat)(5.781427173), (NativeFloat)(-7.494120386), (NativeFloat)(7.133965340), (NativeFloat)(-3.245537043) );
		return add( exponent, x );
	}

	SNML_INLINE NativeFloat exp( NativeFloat x )
	{
		return pow2( mul( x, (NativeFloat)(1.442695041) ) );
	}

	SNML_INLINE NativeFloat log( NativeFloat x )
	{
		return mul( log2( x ), (NativeFloat)(.6931471806) );
	}

	SNML_INLINE NativeFloat pow( NativeFloat x, NativeFloat y )
	{
		return pow2( mul( log2( x ), y ) );
	}

	SNML_INLINE NativeFloat atan2( NativeFloat y, NativeFloat x )
	{
		NativeFloat zero = snh::zero();
		// conditionally rotate by Pi
		NativeFloat abs_y = abs( y );
		NativeFloat abs_x = abs( x );

		NativeUnsigned s0 = (NativeUnsigned)vec_cmplt( x, zero );
		NativeFloat x0 = sel( abs_x, abs_y, s0 );
		NativeFloat y0 = sel( abs_y, abs_x, s0 );
		NativeFloat a0 = sel( zero, (NativeFloat)(3.14159265359/2), s0 );

		// conditionally rotate by Pi/4
		NativeUnsigned s1 = (NativeUnsigned)vec_cmpgt( y0, x0 );
		NativeFloat x1 = sel( x0, add( y0, x0 ), s1 );
		NativeFloat y1 = sel( y0, sub( y0, x0 ), s1 );
		NativeFloat a1 = sel( a0, add( a0, (NativeFloat)(3.14159265359/4) ), s1 );

		NativeFloat sy1 = mul( y1, (NativeFloat)(.4142135625) );
		NativeFloat sx1 = mul( x1, (NativeFloat)(.4142135625) );

		// conditionally rotate by Pi/8
		NativeUnsigned s2 = (NativeUnsigned)vec_cmpgt( y1, sx1 );
		NativeFloat x2 = sel( x1, add( sy1, x1 ), s2 );
		NativeFloat y2 = sel( y1, sub( y1, sx1 ), s2 );
		NativeFloat a2 = sel( a1, add( a1, (NativeFloat)(3.14159265359/8) ), s2 );

		// force x2 non-zero (for critical cases)
		x2 = add( x2, (NativeFloat)(.1175494351e-37) );

		// calculate atan( y / x ) with reduced range
		NativeFloat r = mul( y2, recip( x2 ) );
		NativeFloat r2 = mul( r, r );
		NativeFloat arctanr = madd(r2,madd(r2,madd(r2,(NativeFloat)(-.1151572706),(NativeFloat)(.1976106216)),(NativeFloat)(-.3332657252)),(NativeFloat)(.9999996993));
		
		//printf( "x=%10.5f y=%10.5f xy0=%10.5f/%10.5f xy1=%10.5f/%10.5f xy2=%10.5f/%10.5f\n", (float&)x, (float&)y, (float&)x0, (float&)y0, (float&)x1, (float&)y1, (float&)x2, (float&)y2 );
		return copysign( add( mul( arctanr, r ), a2 ), y );
	}


	// rx = lw * rx + lx * rw + ly * rz - lz * ry;
	// ry = lw * ry - lx * rz + ly * rw + lz * rx;
	// rz = lw * rz + lx * ry - ly * rx + lz * rw;
	// rw = lw * rw - lx * rx - ly * ry - lz * rz;
	template < typename Type > SNML_INLINE Type quat_mul( Type lxyzw, Type rxyzw )
	{
		//printf( "lxyzw=%f %f %f %f\n", ((float*)&lxyzw)[0], ((float*)&lxyzw)[1], ((float*)&lxyzw)[2], ((float*)&lxyzw)[3] );
		//printf( "rxyzw=%f %f %f %f\n", ((float*)&rxyzw)[0], ((float*)&rxyzw)[1], ((float*)&rxyzw)[2], ((float*)&rxyzw)[3] );
		NativeFloat lxxxx = (NativeFloat)vec_xor( x( lxyzw ), (Type)_K4( NativeInt, 0,0x80000000,0,0x80000000) );
		NativeFloat lyyyy = (NativeFloat)vec_xor( y( lxyzw ), (Type)_K4( NativeInt, 0,0,0x80000000,0x80000000) );
		NativeFloat lzzzz = (NativeFloat)vec_xor( z( lxyzw ), (Type)_K4( NativeInt, 0x80000000,0,0,0x80000000) );
		NativeFloat lwwww = w( lxyzw );

		NativeFloat rwzyx = perm<'wzyx'>( rxyzw, rxyzw );
		NativeFloat rzwxy = perm<'zwxy'>( rxyzw, rxyzw );
		NativeFloat ryxwz = perm<'yxwz'>( rxyzw, rxyzw );

		NativeFloat a = mul( lwwww, rxyzw );
		NativeFloat b = mul( lyyyy, rzwxy );
		a = madd( lxxxx, rwzyx, a );
		b = madd( lzzzz, ryxwz, b );
		return add( a, b );
	}
} // snh

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// scalar
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

SNML_INLINE scalar::scalar()
{
}

SNML_INLINE scalar::scalar( const scalar &rhs )
{
	m_Value = rhs.m_Value;
}

SNML_INLINE scalar::scalar( int value_i )
{
	int v[4] __attribute__ ( ( aligned( 16 ) ) ) = { value_i, value_i, value_i, value_i };
	m_Value = vec_ctf( (NativeInt&)v[0], 0 );
}

SNML_INLINE scalar::scalar( const NativeScalar &x_s )
{
	m_Value = x_s;
}

SNML_INLINE const NativeScalar &scalar::access() const
{
	return m_Value;
}

SNML_INLINE void scalar::load( const NativeScalar &x_s )
{
	m_Value = x_s;
}

SNML_INLINE scalar::scalar( float f )
{
	m_Value = (NativeFloat)f;
}

SNML_INLINE scalar::operator float() const
{
	return (float&)m_Value;
}

SNML_INLINE int scalar::ToInt() const
{
	return (int)(float&)m_Value;
}

SNML_INLINE void scalar::load( const float src[], const int byteOffset )
{
	NativeUnsignedChar perm = NativeUnsignedChar( snh::x( NativeFloat( vec_lvsl( byteOffset, src ) ) ) );
	NativeFloat a = vec_lde( byteOffset, src );
	m_Value = vec_perm( a, a, perm );
}

SNML_INLINE void scalar::store( float src[], const int byteOffset ) const
{
	vec_ste( m_Value, byteOffset, src );
}

SNML_INLINE scalar scalar::operator +( scalar_arg rhs ) const
{
	//return vec_add( m_Value, rhs.m_Value );
	return snh::add( m_Value, rhs.m_Value );
}

SNML_INLINE scalar scalar::operator -( scalar_arg rhs ) const
{
	return snh::sub( m_Value, rhs.m_Value );
}

SNML_INLINE scalar scalar::operator *( scalar_arg rhs ) const
{
	return snh::mul( m_Value, rhs.m_Value );
}

SNML_INLINE scalar scalar::operator /( scalar_arg rhs ) const
{
	return scalar( snh::mul( m_Value, snh::recip( rhs.m_Value ) ) );
}

SNML_INLINE void scalar::operator +=( scalar_arg rhs )
{
	m_Value = snh::add( m_Value, rhs.m_Value );
}

SNML_INLINE void scalar::operator -=( scalar_arg rhs )
{
	m_Value = snh::sub( m_Value, rhs.m_Value );
}

SNML_INLINE void scalar::operator *=( scalar_arg rhs )
{
	m_Value = snh::mul( m_Value, rhs.m_Value );
}

SNML_INLINE void scalar::operator /=( scalar_arg rhs )
{
	m_Value = snh::mul( m_Value, snh::recip( rhs.m_Value ) );
}

SNML_INLINE scalar scalar::operator -() const
{
	return snh::sub( snh::zero(), m_Value );
}

SNML_INLINE scalar scalar::operator +() const
{
	return *this;
}

SNML_INLINE vquad scalar::operator +( vquad_arg rhs ) const
{
	return snh::add( m_Value, rhs.m_Value );
}

SNML_INLINE vquad scalar::operator -( vquad_arg rhs ) const
{
	return snh::sub( m_Value, rhs.m_Value );
}

SNML_INLINE vquad scalar::operator *( vquad_arg rhs ) const
{
	return snh::mul( m_Value, rhs.m_Value );
}

SNML_INLINE vquad scalar::operator /( vquad_arg rhs ) const
{
	return snh::mul( m_Value, snh::recip( rhs.m_Value ) );
}

SNML_INLINE quadquad scalar::operator +( quadquad_arg rhs ) const
{
	return quadquad( snh::add( m_Value, rhs.m_X ), snh::add( m_Value, rhs.m_Y ), snh::add( m_Value, rhs.m_Z ), snh::add( m_Value, rhs.m_W ) );
}

SNML_INLINE quadquad scalar::operator -( quadquad_arg rhs ) const
{
	return quadquad( snh::sub( m_Value, rhs.m_X ), snh::sub( m_Value, rhs.m_Y ), snh::sub( m_Value, rhs.m_Z ), snh::sub( m_Value, rhs.m_W ) );
}

SNML_INLINE quadquad scalar::operator *( quadquad_arg rhs ) const
{
	return quadquad( snh::mul( m_Value, rhs.m_X ), snh::mul( m_Value, rhs.m_Y ), snh::mul( m_Value, rhs.m_Z ), snh::mul( m_Value, rhs.m_W ) );
}

SNML_INLINE quadquad scalar::operator /( quadquad_arg rhs ) const
{
	return quadquad( snh::mul( m_Value, snh::recip( rhs.m_X ) ), snh::mul( m_Value, snh::recip( rhs.m_Y ) ), snh::mul( m_Value, snh::recip( rhs.m_Z ) ), snh::mul( m_Value, snh::recip( rhs.m_W ) ) );
}

SNML_INLINE bool scalar::operator ==( scalar_arg rhs ) const
{
	return vec_all_eq( m_Value, rhs.m_Value );
}

SNML_INLINE bool scalar::operator !=( scalar_arg rhs ) const
{
	return vec_any_ne( m_Value, rhs.m_Value );
}

SNML_INLINE bool any_ge( scalar_arg x, scalar_arg y )
{
	return vec_any_ge( x.m_Value, y.m_Value );
}

SNML_INLINE bool any_gt( scalar_arg x, scalar_arg y )
{
	return vec_any_gt( x.m_Value, y.m_Value );
}

SNML_INLINE bool any_le( scalar_arg x, scalar_arg y )
{
	return vec_any_le( x.m_Value, y.m_Value );
}

SNML_INLINE bool any_lt( scalar_arg x, scalar_arg y )
{
	return vec_any_lt( x.m_Value, y.m_Value );
}

SNML_INLINE bool any_eq( scalar_arg x, scalar_arg y )
{
	return vec_any_eq( x.m_Value, y.m_Value );
}

SNML_INLINE bool any_ne( scalar_arg x, scalar_arg y )
{
	return vec_any_ne( x.m_Value, y.m_Value );
}

SNML_INLINE bool all_ge( scalar_arg x, scalar_arg y )
{
	return vec_all_ge( x.m_Value, y.m_Value );
}

SNML_INLINE bool all_gt( scalar_arg x, scalar_arg y )
{
	return vec_all_gt( x.m_Value, y.m_Value );
}

SNML_INLINE bool all_le( scalar_arg x, scalar_arg y )
{
	return vec_all_le( x.m_Value, y.m_Value );
}

SNML_INLINE bool all_lt( scalar_arg x, scalar_arg y )
{
	return vec_all_lt( x.m_Value, y.m_Value );
}

SNML_INLINE bool all_eq( scalar_arg x, scalar_arg y )
{
	return vec_all_eq( x.m_Value, y.m_Value );
}

SNML_INLINE bool all_ne( scalar_arg x, scalar_arg y )
{
	return vec_all_ne( x.m_Value, y.m_Value );
}

SNML_INLINE scalar scalar::zero()
{
	return (NativeFloat)0.0f;
}

SNML_INLINE scalar scalar::one()
{
	return (NativeFloat)1.0f;
}

SNML_INLINE scalar scalar::two()
{
	return (NativeFloat)2.0f;
}

SNML_INLINE scalar scalar::three()
{
	return (NativeFloat)3.0f;
}

SNML_INLINE scalar scalar::four()
{
	return (NativeFloat)4.0f;
}

SNML_INLINE scalar scalar::five()
{
	return (NativeFloat)5.0f;
}

SNML_INLINE scalar scalar::six()
{
	return (NativeFloat)6.0f;
}

SNML_INLINE scalar scalar::seven()
{
	return (NativeFloat)7.0f;
}

SNML_INLINE scalar scalar::eight()
{
	return (NativeFloat)8.0f;
}

SNML_INLINE scalar scalar::nine()
{
	return (NativeFloat)9.0f;
}

SNML_INLINE scalar scalar::ten()
{
	return (NativeFloat)10.0f;
}

SNML_INLINE scalar scalar::half()
{
	return (NativeFloat)0.5f;
}

SNML_INLINE scalar scalar::quarter()
{
	return (NativeFloat)0.25f;
}

SNML_INLINE scalar scalar::three_quarters()
{
	return (NativeFloat)0.75f;
}

SNML_INLINE scalar scalar::three_by_2()
{
	return (NativeFloat)1.5f;
}

SNML_INLINE scalar scalar::huge()
{
	return (NativeFloat)340282346638528886604286022844204804240.0f;
}

SNML_INLINE scalar scalar::sqrt2()
{
	return (NativeFloat)1.41421356237309504880f;
}

SNML_INLINE scalar scalar::sqrt_half()
{
	return (NativeFloat)0.707106781186547524401f;
}

SNML_INLINE scalar scalar::two_by_sqrtpi()
{
	return (NativeFloat)1.12837916709551257390f;
}

SNML_INLINE scalar scalar::two_by_pi()
{
	return (NativeFloat)0.636619772367581343076f;
}

SNML_INLINE scalar scalar::one_by_pi()
{
	return (NativeFloat)0.318309886183790671538f;
}

SNML_INLINE scalar scalar::pi_by_4()
{
	return (NativeFloat)0.785398163397448309616f;
}

SNML_INLINE scalar scalar::pi_by_2()
{
	return (NativeFloat)1.57079632679489661923f;
}

SNML_INLINE scalar scalar::pi()
{
	return (NativeFloat)3.14159265358979323846f;
}

SNML_INLINE scalar scalar::e()
{
	return (NativeFloat)2.71828182845904523536f;
}

SNML_INLINE scalar scalar::log2e()
{
	return (NativeFloat)1.44269504088896340736f;
}

SNML_INLINE scalar scalar::log10e()
{
	return (NativeFloat)0.434294481903251827651f;
}

SNML_INLINE scalar scalar::ln2()
{
	return (NativeFloat)0.693147180559945309417f;
}

SNML_INLINE scalar scalar::ln10()
{
	return (NativeFloat)2.30258509299404568402f;
}

SNML_INLINE scalar scalar::two_pi()
{
	return (NativeFloat)( 3.14159265358979323846f*2.0f );
}

SNML_INLINE scalar scalar::pi_by_6()
{
	return (NativeFloat)( 3.14159265358979323846f/6 );
}

SNML_INLINE scalar scalar::log10two()
{
	return (NativeFloat).30102999566398119522f;
}

SNML_INLINE scalar scalar::log2ten()
{
	return (NativeFloat)3.3219280948873623478f;
}

SNML_INLINE scalar scalar::sqrt3_by_2()
{
	return (NativeFloat).86602540378443864675f;
}

// see Sincos.mws
// 8 layers
SNML_INLINE  scalar sin( scalar_arg x )
{
	return snh::sin( x.m_Value );
}

SNML_INLINE  scalar cos( scalar_arg x )
{
	return snh::cos( x.m_Value );
}

SNML_INLINE  scalar tan( scalar_arg x )
{
	return snh::tan( x.m_Value );
}

SNML_INLINE  scalar asin( scalar_arg x )
{
	return snh::asin( x.m_Value );
}

SNML_INLINE  scalar acos( scalar_arg x )
{
	return snh::acos( x.m_Value );
}

// 2^(x/log(2))
SNML_INLINE  scalar exp( scalar_arg x )
{
	return snh::exp( x.m_Value );
}

// log[2](x)*log(2);
SNML_INLINE  scalar log( scalar_arg x )
{
	return snh::log( x.m_Value );
}

SNML_INLINE  scalar pow( scalar_arg x, scalar_arg power )
{
	return snh::pow( x.m_Value, power.m_Value );
}

SNML_INLINE  scalar atan2( scalar_arg y, scalar_arg x )
{
	return snh::atan2( y.m_Value, x.m_Value );
}

SNML_INLINE  scalar min( scalar_arg x, scalar_arg y )
{
	return snh::min( x.m_Value, y.m_Value );
}

SNML_INLINE  scalar max( scalar_arg x, scalar_arg y )
{
	return snh::max( x.m_Value, y.m_Value );
}

SNML_INLINE  scalar sgn( scalar_arg x )
{
	return snh::sgn( x.m_Value );
}

SNML_INLINE  scalar abs( scalar_arg x )
{
	return snh::abs( x.m_Value );
}

SNML_INLINE  scalar sqrt( scalar_arg x )
{
	return snh::sqrt( x.m_Value );
}

SNML_INLINE  scalar rsqrt( scalar_arg x )
{
	return snh::rsqrt( x.m_Value );
}

SNML_INLINE  scalar recip( scalar_arg x )
{
	return snh::recip( x.m_Value );
}

SNML_INLINE scalar select_ge( scalar_arg x, scalar_arg y, scalar_arg t, scalar_arg f )
{
	return snh::select_ge( x.m_Value, y.m_Value, t.m_Value, f.m_Value );
}

SNML_INLINE scalar select_gt( scalar_arg x, scalar_arg y, scalar_arg t, scalar_arg f )
{
	return snh::select_gt( x.m_Value, y.m_Value, t.m_Value, f.m_Value );
}

SNML_INLINE scalar select_le( scalar_arg x, scalar_arg y, scalar_arg t, scalar_arg f )
{
	return snh::select_le( x.m_Value, y.m_Value, t.m_Value, f.m_Value );
}

SNML_INLINE scalar select_lt( scalar_arg x, scalar_arg y, scalar_arg t, scalar_arg f )
{
	return snh::select_lt( x.m_Value, y.m_Value, t.m_Value, f.m_Value );
}

SNML_INLINE scalar select_eq( scalar_arg x, scalar_arg y, scalar_arg t, scalar_arg f )
{
	return snh::select_eq( x.m_Value, y.m_Value, t.m_Value, f.m_Value );
}

SNML_INLINE scalar select_ne( scalar_arg x, scalar_arg y, scalar_arg t, scalar_arg f )
{
	return snh::select_ne( x.m_Value, y.m_Value, t.m_Value, f.m_Value );
}

// TODO: make this more efficient.
SNML_INLINE scalar select( bool sel, scalar_arg t, scalar_arg f )
{
	return sel ? t : f;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// vquad
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

SNML_INLINE vquad::vquad()
{
}

SNML_INLINE vquad::vquad( const NativeQuad value )
{
	m_Value = value;
}

SNML_INLINE const NativeQuad &vquad::access() const
{
	return m_Value;
}

SNML_INLINE vquad vquad::operator +( vquad_arg rhs ) const
{
	return snh::add( m_Value, rhs.m_Value );
}

SNML_INLINE vquad vquad::operator -( vquad_arg rhs ) const
{
	return snh::sub( m_Value, rhs.m_Value );
}

SNML_INLINE vquad vquad::operator *( vquad_arg rhs ) const
{
	return snh::mul( m_Value, rhs.m_Value );
}

SNML_INLINE vquad vquad::operator /( vquad_arg rhs ) const
{
	return snh::mul( m_Value, snh::recip( rhs.m_Value ) );
}

SNML_INLINE void vquad::operator +=( vquad_arg rhs )
{
	m_Value = snh::add( m_Value, rhs.m_Value );
}

SNML_INLINE void vquad::operator -=( vquad_arg rhs )
{
	m_Value = snh::sub( m_Value, rhs.m_Value );
}

SNML_INLINE void vquad::operator *=( vquad_arg rhs )
{
	m_Value = snh::mul( m_Value, rhs.m_Value );
}

SNML_INLINE void vquad::operator /=( vquad_arg rhs )
{
	m_Value = snh::mul( m_Value, snh::recip( rhs.m_Value ) );
}

SNML_INLINE vquad vquad::operator +( scalar_arg rhs ) const
{
	return snh::add( m_Value, rhs.m_Value );
}

SNML_INLINE vquad vquad::operator -( scalar_arg rhs ) const
{
	return snh::sub( m_Value, rhs.m_Value );
}

SNML_INLINE vquad vquad::operator *( scalar_arg rhs ) const
{
	return snh::mul( m_Value, rhs.m_Value );
}

SNML_INLINE vquad vquad::operator /( scalar_arg rhs ) const
{
	return snh::mul( m_Value, snh::recip( rhs.m_Value ) );
}

SNML_INLINE void vquad::operator +=( scalar_arg rhs )
{
	m_Value = snh::add( m_Value, rhs.m_Value );
}

SNML_INLINE void vquad::operator -=( scalar_arg rhs )
{
	m_Value = snh::sub( m_Value, rhs.m_Value );
}

SNML_INLINE void vquad::operator *=( scalar_arg rhs )
{
	m_Value = snh::mul( m_Value, rhs.m_Value );
}

SNML_INLINE void vquad::operator /=( scalar_arg rhs )
{
	m_Value = snh::mul( m_Value, snh::recip( rhs.m_Value ) );
}

SNML_INLINE vquad vquad::operator -() const
{
	return snh::sub( snh::zero(), m_Value );
}

SNML_INLINE vquad vquad::operator +() const
{
	return *this;
}

SNML_INLINE bool vquad::operator ==( vquad_arg rhs ) const
{
	return vec_all_eq( m_Value, rhs.m_Value );
}

SNML_INLINE bool vquad::operator !=( vquad_arg rhs ) const
{
	return vec_any_ne( m_Value, rhs.m_Value );
}

SNML_INLINE bool any_ge( vquad_arg x, vquad_arg y )
{
	return vec_any_ge( x.m_Value, y.m_Value );
}

SNML_INLINE bool any_gt( vquad_arg x, vquad_arg y )
{
	return vec_any_gt( x.m_Value ,y.m_Value );
}

SNML_INLINE bool any_le( vquad_arg x, vquad_arg y )
{
	return vec_any_le( x.m_Value, y.m_Value );
}

SNML_INLINE bool any_lt( vquad_arg x, vquad_arg y )
{
	return vec_any_lt( x.m_Value ,y.m_Value );
}

SNML_INLINE bool any_eq( vquad_arg x, vquad_arg y )
{
	return vec_any_eq( x.m_Value, y.m_Value );
}

SNML_INLINE bool any_ne( vquad_arg x, vquad_arg y )
{
	return vec_any_ne( x.m_Value, y.m_Value );
}

SNML_INLINE bool all_ge( vquad_arg x, vquad_arg y )
{
	return vec_all_ge( x.m_Value, y.m_Value );
}

SNML_INLINE bool all_gt( vquad_arg x, vquad_arg y )
{
	return vec_all_gt( x.m_Value ,y.m_Value );
}

SNML_INLINE bool all_le( vquad_arg x, vquad_arg y )
{
	return vec_all_le( x.m_Value, y.m_Value );
}

SNML_INLINE bool all_lt( vquad_arg x, vquad_arg y )
{
	return vec_all_lt( x.m_Value ,y.m_Value );
}

SNML_INLINE bool all_eq( vquad_arg x, vquad_arg y )
{
	return vec_all_eq( x.m_Value, y.m_Value );
}

SNML_INLINE bool all_ne( vquad_arg x, vquad_arg y )
{
	return vec_all_ne( x.m_Value, y.m_Value );
}

SNML_INLINE vquad vquad::zero()
{
	return vquad( snh::zero() );
}

SNML_INLINE vquad vquad::one()
{
	return vquad( (NativeFloat)1.0f );
}

SNML_INLINE vquad vquad::two()
{
	return vquad( (NativeFloat)2.0f );
}

SNML_INLINE vquad vquad::three()
{
	return vquad( (NativeFloat)3.0f );
}

SNML_INLINE vquad vquad::four()
{
	return vquad( (NativeFloat)4.0f );
}

SNML_INLINE vquad vquad::five()
{
	return vquad( (NativeFloat)5.0f );
}

SNML_INLINE vquad vquad::six()
{
	return vquad( (NativeFloat)6.0f );
}

SNML_INLINE vquad vquad::seven()
{
	return vquad( (NativeFloat)7.0f );
}

SNML_INLINE vquad vquad::eight()
{
	return vquad( (NativeFloat)8.0f );
}

SNML_INLINE vquad vquad::nine()
{
	return vquad( (NativeFloat)9.0f );
}

SNML_INLINE vquad vquad::ten()
{
	return vquad( (NativeFloat)10.0f );
}

SNML_INLINE vquad vquad::half()
{
	return vquad( (NativeFloat)0.5f );
}

SNML_INLINE vquad vquad::quarter()
{
	return vquad( (NativeFloat)0.25f );
}

SNML_INLINE vquad vquad::three_quarters()
{
	return vquad( (NativeFloat)0.75f );
}

SNML_INLINE vquad vquad::three_by_2()
{
	return vquad( (NativeFloat)1.5f );
}

SNML_INLINE vquad vquad::huge()
{
	return vquad( (NativeFloat)340282346638528886604286022844204804240.0f );
}

SNML_INLINE vquad vquad::sqrt2()
{
	return vquad( (NativeFloat)1.41421356237309504880f );
}

SNML_INLINE vquad vquad::sqrt_half()
{
	return vquad( (NativeFloat)0.707106781186547524401f );
}

SNML_INLINE vquad vquad::two_by_sqrtpi()
{
	return vquad( (NativeFloat)1.12837916709551257390f );
}

SNML_INLINE vquad vquad::two_by_pi()
{
	return vquad( (NativeFloat)0.636619772367581343076f );
}

SNML_INLINE vquad vquad::one_by_pi()
{
	return vquad( (NativeFloat)0.318309886183790671538f );
}

SNML_INLINE vquad vquad::pi_by_4()
{
	return vquad( (NativeFloat)0.785398163397448309616f );
}

SNML_INLINE vquad vquad::pi_by_2()
{
	return vquad( (NativeFloat)1.57079632679489661923f );
}

SNML_INLINE vquad vquad::pi()
{
	return vquad( (NativeFloat)3.14159265358979323846f );
}

SNML_INLINE vquad vquad::e()
{
	return vquad( (NativeFloat)2.71828182845904523536f );
}

SNML_INLINE vquad vquad::log2e()
{
	return vquad( (NativeFloat)1.44269504088896340736f );
}

SNML_INLINE vquad vquad::log10e()
{
	return vquad( (NativeFloat)0.434294481903251827651f );
}

SNML_INLINE vquad vquad::ln2()
{
	return vquad( (NativeFloat)0.693147180559945309417f );
}

SNML_INLINE vquad vquad::ln10()
{
	return vquad( (NativeFloat)2.30258509299404568402f );
}

SNML_INLINE vquad vquad::two_pi()
{
	return vquad( (NativeFloat)( 3.14159265358979323846f*2.0f ) );
}

SNML_INLINE vquad vquad::pi_by_6()
{
	return vquad( (NativeFloat)( 3.14159265358979323846f/6 ) );
}

SNML_INLINE vquad vquad::log10two()
{
	return vquad( (NativeFloat).30102999566398119522f );
}

SNML_INLINE vquad vquad::log2ten()
{
	return vquad( (NativeFloat)3.3219280948873623478f );
}

SNML_INLINE vquad vquad::sqrt3_by_2()
{
	return vquad( (NativeFloat).86602540378443864675f );
}

SNML_INLINE vquad vquad::unit()
{
	return vquad( (NativeFloat)( 0, 0, 0, 1 ) );
}

SNML_INLINE vquad::vquad( float value_f )
{
	m_Value = (NativeFloat)( value_f );
}

SNML_INLINE vquad::vquad( scalar_arg value_s )
{
	m_Value = value_s.m_Value;
}

SNML_INLINE vquad::vquad( float x_f, float y_f )
{
	m_Value = (NativeQuad)( x_f, y_f, y_f, y_f );
}

SNML_INLINE vquad::vquad( scalar_arg x_s, scalar_arg y_s )
{
	m_Value = vec_mergel( x_s.m_Value, y_s.m_Value );
}

SNML_INLINE vquad::vquad( float x_f, float y_f, float z_f )
{
	m_Value = (NativeQuad)( x_f, y_f, z_f, z_f );
}

SNML_INLINE vquad::vquad( scalar_arg x_s, scalar_arg y_s, scalar_arg z_s )
{
	m_Value = vec_sld( vec_mergel( x_s.m_Value, y_s.m_Value ), z_s.m_Value, 8 );
}

SNML_INLINE vquad::vquad( float x_f, float y_f, float z_f, float w_f )
{
	m_Value = (NativeQuad)( x_f, y_f, z_f, w_f );
}

SNML_INLINE vquad::vquad( scalar_arg x_s, scalar_arg y_s, scalar_arg z_s, scalar_arg w_s )
{
	m_Value = vec_sld( vec_mergel( x_s.m_Value, y_s.m_Value ), vec_mergel( z_s.m_Value, w_s.m_Value ), 8 );
}

SNML_INLINE vquad::operator float() const
{
	return (float&)m_Value;
}

SNML_INLINE void vquad::load1( const float src[], const int byteOffset )
{
	NativeUnsignedChar perm = (NativeUnsignedChar)vec_splat( NativeUnsigned( vec_lvsl( byteOffset, src ) ), 0 );
	NativeFloat a = vec_lde( byteOffset, src );
	m_Value = vec_perm( a, a, perm );
}

SNML_INLINE void vquad::load_unaligned( const float src[], const int byteOffset )
{
	NativeUnsignedChar perm = vec_lvsl( byteOffset, src );
	NativeFloat a = vec_ld( byteOffset, src );
	NativeFloat b = vec_ld( byteOffset+16, src );
	m_Value = vec_perm( a, b, perm );
}

SNML_INLINE void vquad::load( const float src[], const int byteOffset )
{
	m_Value = vec_ld( byteOffset, src );
}

SNML_INLINE void vquad::load1( const signed int src[], const int byteOffset )
{
	NativeUnsignedChar perm = NativeUnsignedChar( snh::x( NativeFloat( vec_lvsl( byteOffset, src ) ) ) );
	NativeInt a = vec_lde( byteOffset, (int*)src );
	m_Value = vec_ctf( vec_perm( a, a, perm ), 0 );
}

SNML_INLINE void vquad::load_unaligned( const signed int src[], const int byteOffset )
{
	NativeUnsignedChar perm = vec_lvsl( byteOffset, src );
	NativeInt a = vec_ld( byteOffset, (int*)src );
	NativeInt b = vec_ld( byteOffset+16, (int*)src );
	m_Value = vec_ctf( vec_perm( a, b, perm ), 0 );
}

SNML_INLINE void vquad::load( const signed int src[], const int byteOffset )
{
	m_Value = vec_ctf( vec_ld( byteOffset, src ), 0 );
}


SNML_INLINE void vquad::load1( const signed short src[], const int byteOffset )
{
	NativeUnsignedChar perm = (NativeUnsignedChar)vec_splat( NativeShort( vec_lvsl( byteOffset, (short*)src ) ), 0 );
	NativeShort a = vec_lde( byteOffset, (short*)src );
	m_Value = vec_ctf( vec_unpackh( vec_perm( a, a, perm ) ), 0 );
}

SNML_INLINE void vquad::load_unaligned( const signed short src[], const int byteOffset )
{
	NativeUnsignedChar perm = vec_lvsl( byteOffset, (short*)src );
	NativeShort a = vec_ld( byteOffset, (short*)src );
	NativeShort b = vec_ld( byteOffset+16, (short*)src );
	//printf( "src=%d %d %d %d %d %d %d %d\n", src[0], src[1], src[2], src[3], src[4+0], src[4+1], src[4+2], src[4+3] );
	//printf( "a=%d %d %d %d %d %d %d %d\n", ((short*)&a)[0], ((short*)&a)[1], ((short*)&a)[2], ((short*)&a)[3], ((short*)&a)[4+0], ((short*)&a)[4+1], ((short*)&a)[4+2], ((short*)&a)[4+3] );
	//printf( "b=%d %d %d %d %d %d %d %d\n", ((short*)&b)[0], ((short*)&b)[1], ((short*)&b)[2], ((short*)&b)[3], ((short*)&b)[4+0], ((short*)&b)[4+1], ((short*)&b)[4+2], ((short*)&b)[4+3] );
	m_Value = vec_ctf( vec_unpackh( vec_perm( a, b, perm ) ), 0 );
}

SNML_INLINE void vquad::load( const signed short src[], const int byteOffset )
{
	NativeUnsignedChar perm = vec_lvsl( byteOffset, (short*)src );
	NativeShort a = vec_ld( byteOffset, (short*)src );
	//printf( "src=%d %d %d %d %d %d %d %d\n", src[0], src[1], src[2], src[3], src[4+0], src[4+1], src[4+2], src[4+3] );
	//printf( "a=%d %d %d %d %d %d %d %d\n", ((short*)&a)[0], ((short*)&a)[1], ((short*)&a)[2], ((short*)&a)[3], ((short*)&a)[4+0], ((short*)&a)[4+1], ((short*)&a)[4+2], ((short*)&a)[4+3] );
	m_Value = vec_ctf( vec_unpackh( vec_perm( a, a, perm ) ), 0 );
}

SNML_INLINE void vquad::load_nocache( const float src[], const int byteOffset )
{
	m_Value = vec_ldl( byteOffset, src );
}

SNML_INLINE void vquad::set_x( scalar_arg value )
{
	m_Value = snh::perm<'Xyzw'>( m_Value, value.m_Value );
}

SNML_INLINE void vquad::set_y( scalar_arg value )
{
	m_Value = snh::perm<'xYzw'>( m_Value, value.m_Value );
}

SNML_INLINE void vquad::set_z( scalar_arg value )
{
	m_Value = snh::perm<'xyZw'>( m_Value, value.m_Value );
}

SNML_INLINE void vquad::set_w( scalar_arg value )
{
	m_Value = snh::perm<'xyzW'>( m_Value, value.m_Value );
}

SNML_INLINE void vquad::store( NativeScalar &x_s ) const
{
	x_s = m_Value;
}

SNML_INLINE void vquad::store( NativeScalar &x_s, NativeScalar &y_s ) const
{
	x_s = snh::x( m_Value );
	y_s = snh::y( m_Value );
}

SNML_INLINE void vquad::store( NativeScalar &x_s, NativeScalar &y_s, NativeScalar &z_s ) const
{
	x_s = snh::x( m_Value );
	y_s = snh::y( m_Value );
	z_s = snh::z( m_Value );
}

SNML_INLINE void vquad::store( NativeScalar &x_s, NativeScalar &y_s, NativeScalar &z_s, NativeScalar &w_s ) const
{
	x_s = snh::x( m_Value );
	y_s = snh::y( m_Value );
	z_s = snh::z( m_Value );
	w_s = snh::w( m_Value );
}

SNML_INLINE void vquad::store( signed int dest[] ) const
{
	*(vector signed int*)dest = snh::cts( m_Value, 0 );
}

SNML_INLINE void vquad::store( signed short dest[] ) const
{
	vector signed int dest32 = snh::cts( m_Value, 0 );
	vector signed short dest16 = vec_packs( dest32, dest32 );
	vec_ste( (vector signed int)dest16, 0, (signed*)dest );
	vec_ste( (vector signed int)dest16, 4, (signed*)dest );
}

SNML_INLINE void vquad::store( signed char dest[] ) const
{
	vector signed int dest32 = snh::cts( m_Value, 0 );
	vector signed short dest16 = vec_packs( dest32, dest32 );
	vector signed char dest8 = vec_packs( dest16, dest16 );
	vec_ste( (vector signed int)dest8, 0, (signed*)dest );
}

SNML_INLINE void vquad::store( unsigned int dest[] ) const
{
	*(vector unsigned int*)dest = snh::ctu( m_Value, 0 );
}

SNML_INLINE void vquad::store( unsigned short dest[] ) const
{
	vector unsigned int dest32 = snh::ctu( m_Value, 0 );
	vector unsigned short dest16 = vec_packsu( dest32, dest32 );
	vec_ste( (vector unsigned int)dest16, 0, (unsigned*)dest );
	vec_ste( (vector unsigned int)dest16, 4, (unsigned*)dest );
}

SNML_INLINE void vquad::store( unsigned char dest[] ) const
{
	vector unsigned int dest32 = snh::ctu( m_Value, 0 );
	vector unsigned short dest16 = vec_packsu( dest32, dest32 );
	vector unsigned char dest8 = vec_packsu( dest16, dest16 );
	vec_ste( (vector unsigned int)dest8, 0, (unsigned*)dest );
}

SNML_INLINE void vquad::store( float dest[] ) const
{
}

SNML_INLINE void vquad::store_nocache( float dest[] ) const
{
}

SNML_INLINE void vquad::load( const NativeScalar &x_s )
{
	m_Value = x_s;
}

SNML_INLINE void vquad::load( const NativeScalar &x_s, const NativeScalar &y_s )
{
	m_Value = vec_sld( vec_mergel( x_s, y_s ), snh::zero(), 8 );
}

SNML_INLINE void vquad::load( const NativeScalar &x_s, const NativeScalar &y_s, const NativeScalar &z_s )
{
	m_Value = vec_sld( vec_mergel( x_s, y_s ), vec_mergel( z_s, snh::zero() ), 8 );
}

SNML_INLINE void vquad::load( const NativeScalar &x_s, const NativeScalar &y_s, const NativeScalar &z_s, const NativeScalar &w_s )
{
	m_Value = vec_sld( vec_mergel( x_s, y_s ), vec_mergel( z_s, w_s ), 8 );
}

SNML_INLINE scalar vquad::x() const
{
	return scalar( snh::x( m_Value ) );
}
		
SNML_INLINE scalar vquad::y() const
{
	return scalar( snh::y( m_Value ) );
}
		
SNML_INLINE scalar vquad::z() const
{
	return scalar( snh::z( m_Value ) );
}
		
SNML_INLINE scalar vquad::w() const
{
	return scalar( snh::w( m_Value ) );
}

SNML_INLINE vquad conj( vquad_arg rhs )
{
	return vec_xor( rhs.m_Value, (NativeFloat)_K4( NativeInt, 0x80000000, 0x80000000, 0x80000000, 0 ) );
}

SNML_INLINE vquad zerow( vquad_arg rhs )
{
	return (NativeFloat)vec_and( (NativeInt){ -1, -1, -1, 0 }, (NativeInt)rhs.m_Value );
}

SNML_INLINE vquad onew( vquad_arg rhs )
{
	return (NativeFloat)vec_or( vec_and( (NativeInt){ -1, -1, -1, 0 }, (NativeInt)rhs.m_Value ), (NativeInt){ 0, 0, 0, 0x3f800000 } );
}

SNML_INLINE vquad cross( vquad_arg lhs, vquad_arg rhs )
{
	NativeFloat lyzx = snh::perm<'yzxw'>( lhs.m_Value );
	NativeFloat ryzx = snh::perm<'yzxw'>( rhs.m_Value );
	NativeFloat rzxy = snh::perm<'zxyw'>( rhs.m_Value );
	NativeFloat lzxy = snh::perm<'zxyw'>( lhs.m_Value );
	return vec_nmsub( lzxy, ryzx, snh::mul( lyzx, rzxy ) );
}

SNML_INLINE vquad axis_angle( vquad_arg axis, scalar_arg angle )
{
	NativeFloat halfAngle = ( angle * scalar::half() ).m_Value;
	NativeFloat xsin = snh::sin( halfAngle );
	NativeFloat xcos = snh::cos( halfAngle );
	return snh::perm<'xyzW'>( snh::mul( axis.m_Value, xsin ), xcos );
}

SNML_INLINE vquad quat_mul( vquad_arg lhs, vquad_arg rhs )
{
	return snh::quat_mul( lhs.m_Value, rhs.m_Value );
}

SNML_INLINE vquad quat_rotate( vquad_arg lhs, vquad_arg rhs )
{
	return quat_mul( quat_mul( lhs, rhs ), conj( lhs ) );
}

SNML_INLINE vquad slerp( vquad_arg a, vquad_arg b, scalar_arg alpha )
{
	scalar dot = dot4( a, b );
	scalar angle = acos( dot );
	scalar scale = rsqrt( scalar::one() - dot * dot );
	scalar one_minus_alpha = scalar::one() - alpha;
	return select_ge(
		abs( dot ), (NativeFloat)(0.99999),
		a * one_minus_alpha + b * alpha,
		( a * sin( one_minus_alpha * angle ) + b * sin( alpha * angle ) ) * scale
	);
}

SNML_INLINE vquad squad( vquad_arg a, vquad_arg b, vquad_arg c, vquad_arg d, scalar_arg alpha )
{
	return slerp( slerp( a, d, alpha ), slerp( b, c, alpha ), scalar::two() * alpha * ( scalar::one() - alpha ) );
}

SNML_INLINE scalar angle( vquad_arg lhs, vquad_arg rhs )
{
	return acos( dot3( lhs, rhs ) * rsqrt( length_squared3( lhs ) * length_squared3( rhs ) ) );
}

SNML_INLINE scalar dot2( vquad_arg lhs, vquad_arg rhs )
{
	return sum2( lhs * rhs );
}

SNML_INLINE scalar dot3( vquad_arg lhs, vquad_arg rhs )
{
	return sum3( lhs * rhs );
}

SNML_INLINE scalar dot4( vquad_arg lhs, vquad_arg rhs )
{
	return sum4( lhs * rhs );
}

SNML_INLINE scalar sum2( vquad_arg rhs )
{
	return rhs.x() + rhs.y();
}

SNML_INLINE scalar sum3( vquad_arg rhs )
{
	return ( rhs.x() + rhs.y() + rhs.z() );
}

SNML_INLINE scalar sum4( vquad_arg rhs )
{
	return ( rhs.x() + rhs.y() ) + ( rhs.z() + rhs.w() );
}

SNML_INLINE scalar sum_sideways( vquad_arg rhs )
{
	scalar xpy = rhs.x() + rhs.y();
	scalar zpw = rhs.w();
	return ( rhs.x() + rhs.y() ) + ( rhs.z() + rhs.w() );
}

SNML_INLINE scalar max_sideways( vquad_arg rhs )
{
	return max( max( rhs.x(), rhs.y() ), max( rhs.z(), rhs.w() ) );
}

SNML_INLINE scalar min_sideways( vquad_arg rhs )
{
	return min( min( rhs.x(), rhs.y() ), min( rhs.z(), rhs.w() ) );
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
	return sqrt( dot2( rhs, rhs ) );
}

SNML_INLINE scalar length3( vquad_arg rhs )
{
	return sqrt( dot3( rhs, rhs ) );
}

SNML_INLINE scalar length4( vquad_arg rhs )
{
	return sqrt( dot4( rhs, rhs ) );
}

SNML_INLINE scalar rlength2( vquad_arg rhs )
{
	return rsqrt( dot2( rhs, rhs ) );
}

SNML_INLINE scalar rlength3( vquad_arg rhs )
{
	return rsqrt( dot3( rhs, rhs ) );
}

SNML_INLINE scalar rlength4( vquad_arg rhs )
{
	return rsqrt( dot4( rhs, rhs ) );
}

SNML_INLINE scalar length_squared2( vquad_arg rhs )
{
	return dot2( rhs, rhs );
}

SNML_INLINE scalar length_squared3( vquad_arg rhs )
{
	return dot3( rhs, rhs );
}

SNML_INLINE scalar length_squared4( vquad_arg rhs )
{
	return dot4( rhs, rhs );
}

SNML_INLINE vquad sin( vquad_arg x )
{
	return snh::sin( x.m_Value );
}

SNML_INLINE vquad cos( vquad_arg x )
{
	return snh::cos( x.m_Value );
}

SNML_INLINE vquad tan( vquad_arg x )
{
	return snh::tan( x.m_Value );
}

SNML_INLINE vquad asin( vquad_arg x )
{
	return snh::asin( x.m_Value );
}

SNML_INLINE vquad acos( vquad_arg x )
{
	return snh::acos( x.m_Value );
}

SNML_INLINE vquad exp( vquad_arg x )
{
	return snh::exp( x.m_Value );
}

SNML_INLINE vquad log( vquad_arg x )
{
	return snh::log( x.m_Value );
}

SNML_INLINE vquad pow( vquad_arg x, vquad_arg power )
{
	return snh::pow( x.m_Value, power.m_Value );
}

SNML_INLINE vquad atan2( vquad_arg y, vquad_arg x )
{
	return snh::atan2( y.m_Value, x.m_Value );
}

SNML_INLINE vquad min( vquad_arg x, vquad_arg y )
{
	return snh::min( x.m_Value, y.m_Value );
}

SNML_INLINE vquad max( vquad_arg x, vquad_arg y )
{
	return snh::max( x.m_Value, y.m_Value );
}

SNML_INLINE vquad sgn( vquad_arg x )
{
	return snh::sgn( x.m_Value );
}

SNML_INLINE vquad abs( vquad_arg x )
{
	return snh::abs( x.m_Value );
}

SNML_INLINE vquad sqrt( vquad_arg x )
{
	return snh::sqrt( x.m_Value );
}

SNML_INLINE vquad rsqrt( vquad_arg x )
{
	return snh::rsqrt( x.m_Value );
}
				
SNML_INLINE vquad recip( vquad_arg x )
{
	return snh::recip( x.m_Value );
}
				
SNML_INLINE vquad select_ge( vquad_arg x, vquad_arg y, vquad_arg t, vquad_arg f )
{
	return snh::select_ge( x.m_Value, y.m_Value, t.m_Value, f.m_Value );
}

SNML_INLINE vquad select_gt( vquad_arg x, vquad_arg y, vquad_arg t, vquad_arg f )
{
	return snh::select_gt( x.m_Value, y.m_Value, t.m_Value, f.m_Value );
}

SNML_INLINE vquad select_le( vquad_arg x, vquad_arg y, vquad_arg t, vquad_arg f )
{
	return snh::select_le( x.m_Value, y.m_Value, t.m_Value, f.m_Value );
}

SNML_INLINE vquad select_lt( vquad_arg x, vquad_arg y, vquad_arg t, vquad_arg f )
{
	return snh::select_lt( x.m_Value, y.m_Value, t.m_Value, f.m_Value );
}

SNML_INLINE vquad select_eq( vquad_arg x, vquad_arg y, vquad_arg t, vquad_arg f )
{
	return snh::select_eq( x.m_Value, y.m_Value, t.m_Value, f.m_Value );
}

SNML_INLINE vquad select_ne( vquad_arg x, vquad_arg y, vquad_arg t, vquad_arg f )
{
	return snh::select_ne( x.m_Value, y.m_Value, t.m_Value, f.m_Value );
}

// TODO: make this more efficient.
SNML_INLINE vquad select( bool sel, vquad_arg t, vquad_arg f )
{
	return sel ? t : f;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// quadquad
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

SNML_INLINE quadquad::quadquad()
{
}

SNML_INLINE quadquad::quadquad( float value_f )
{
	m_X = m_Y = m_Z = m_W = NativeFloat( value_f );
}

SNML_INLINE quadquad::quadquad( scalar_arg value_s )
{
	m_X = m_Y = m_Z = m_W = value_s.m_Value;
}

SNML_INLINE quadquad::quadquad( float xx, float xy, float xz, float xw, float yx, float yy, float yz, float yw, float zx, float zy, float zz, float zw, float wx, float wy, float wz, float ww )
{
	m_X = (NativeFloat){ xx, xy, xz, xw };
	m_Y = (NativeFloat){ yx, yy, yz, yw };
	m_Z = (NativeFloat){ zx, zy, zz, zw };
	m_W = (NativeFloat){ wx, wy, wz, ww };
}

SNML_INLINE quadquad::quadquad( vquad_arg x_s, vquad_arg y_s, vquad_arg z_s, vquad_arg w_s )
{
	m_X = x_s.m_Value;
	m_Y = y_s.m_Value;
	m_Z = z_s.m_Value;
	m_W = w_s.m_Value;
}

SNML_INLINE quadquad::quadquad( const float src[] )
{
	m_X = (NativeFloat&)src[ 0 ];
	m_Y = (NativeFloat&)src[ 4 ];
	m_Z = (NativeFloat&)src[ 8 ];
	m_W = (NativeFloat&)src[ 12 ];
	//m_X = (NativeFloat){ src[ 0 ], src[ 1 ], src[ 2 ], src[ 3 ] };
	//m_Y = (NativeFloat){ src[ 0 + 4 ], src[ 1 + 4 ], src[ 2 + 4 ], src[ 3 + 4 ] };
	//m_Z = (NativeFloat){ src[ 0 + 8 ], src[ 1 + 8 ], src[ 2 + 8 ], src[ 3 + 8 ] };
	//m_W = (NativeFloat){ src[ 0 + 12 ], src[ 1 + 12 ], src[ 2 + 12 ], src[ 3 + 12 ] };
}

SNML_INLINE quadquad::quadquad( const scalar src[] )
{
	m_X = vec_sld( vec_mergel( src[0].m_Value, src[1].m_Value ), vec_mergel( src[2].m_Value, src[3].m_Value ), 8 );
	m_Y = vec_sld( vec_mergel( src[4+0].m_Value, src[4+1].m_Value ), vec_mergel( src[4+2].m_Value, src[4+3].m_Value ), 8 );
	m_Z = vec_sld( vec_mergel( src[8+0].m_Value, src[8+1].m_Value ), vec_mergel( src[8+2].m_Value, src[8+3].m_Value ), 8 );
	m_W = vec_sld( vec_mergel( src[12+0].m_Value, src[12+1].m_Value ), vec_mergel( src[12+2].m_Value, src[12+3].m_Value ), 8 );
}

SNML_INLINE quadquad::quadquad( NativeQuad x, NativeQuad y, NativeQuad z, NativeQuad w )
{
	m_X = x;
	m_Y = y;
	m_Z = z;
	m_W = w;
}

SNML_INLINE void quadquad::load( float xx, float xy, float xz, float xw, float yx, float yy, float yz, float yw, float zx, float zy, float zz, float zw, float wx, float wy, float wz, float ww )
{
	m_X = (NativeFloat){ xx, xy, xz, xw };
	m_Y = (NativeFloat){ yx, yy, yz, yw };
	m_Z = (NativeFloat){ zx, zy, zz, zw };
	m_W = (NativeFloat){ wx, wy, wz, ww };
}

SNML_INLINE void quadquad::load( const float src[] )
{
	m_X = (NativeFloat&)src[ 0 ];
	m_Y = (NativeFloat&)src[ 4 ];
	m_Z = (NativeFloat&)src[ 8 ];
	m_W = (NativeFloat&)src[ 12 ];

	//m_X = (NativeFloat){ src[ 0 ], src[ 1 ], src[ 2 ], src[ 3 ] };
	//m_Y = (NativeFloat){ src[ 0 + 4 ], src[ 1 + 4 ], src[ 2 + 4 ], src[ 3 + 4 ] };
	//m_Z = (NativeFloat){ src[ 0 + 8 ], src[ 1 + 8 ], src[ 2 + 8 ], src[ 3 + 8 ] };
	//m_W = (NativeFloat){ src[ 0 + 12 ], src[ 1 + 12 ], src[ 2 + 12 ], src[ 3 + 12 ] };
}

SNML_INLINE void quadquad::load( vquad_arg x, vquad_arg y, vquad_arg z, vquad_arg w )
{
	m_X = x.m_Value;
	m_Y = y.m_Value;
	m_Z = z.m_Value;
	m_W = w.m_Value;
}

SNML_INLINE void quadquad::load( NativeQuad x, NativeQuad y, NativeQuad z, NativeQuad w )
{
	m_X = x;
	m_Y = y;
	m_Z = z;
	m_W = w;
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
	result.m_X = snh::add( m_X, rhs.m_X );
	result.m_Y = snh::add( m_Y, rhs.m_Y );
	result.m_Z = snh::add( m_Z, rhs.m_Z );
	result.m_W = snh::add( m_W, rhs.m_W );
	return result;
}

SNML_INLINE quadquad quadquad::operator -( quadquad_arg rhs ) const
{
	quadquad result;
	result.m_X = snh::sub( m_X, rhs.m_X );
	result.m_Y = snh::sub( m_Y, rhs.m_Y );
	result.m_Z = snh::sub( m_Z, rhs.m_Z );
	result.m_W = snh::sub( m_W, rhs.m_W );
	return result;
}

SNML_INLINE quadquad quadquad::operator *( quadquad_arg rhs ) const
{
	return quadquad( snh::mul( m_X, rhs.m_X ), snh::mul( m_Y, rhs.m_Y ), snh::mul( m_Z, rhs.m_Z ), snh::mul( m_W, rhs.m_W ) );
}

SNML_INLINE quadquad quadquad::operator /( quadquad_arg rhs ) const
{
	quadquad r = recip( rhs );
	return quadquad( snh::mul( m_X, r.m_X ), snh::mul( m_Y, r.m_Y ), snh::mul( m_Z, r.m_Z ), snh::mul( m_W, r.m_W ) );
}

SNML_INLINE quadquad quadquad::operator +( scalar_arg rhs ) const
{
	return quadquad( snh::add( m_X, rhs.m_Value ), snh::add( m_Y, rhs.m_Value ), snh::add( m_Z, rhs.m_Value ), snh::add( m_W, rhs.m_Value ) );
}

SNML_INLINE quadquad quadquad::operator -( scalar_arg rhs ) const
{
	return quadquad( snh::sub( m_X, rhs.m_Value ), snh::sub( m_Y, rhs.m_Value ), snh::sub( m_Z, rhs.m_Value ), snh::sub( m_W, rhs.m_Value ) );
}

SNML_INLINE quadquad quadquad::operator *( scalar_arg rhs ) const
{
	return quadquad( snh::mul( m_X, rhs.m_Value ), snh::mul( m_Y, rhs.m_Value ), snh::mul( m_Z, rhs.m_Value ), snh::mul( m_W, rhs.m_Value ) );
}

SNML_INLINE quadquad quadquad::operator /( scalar_arg rhs ) const
{
	scalar r = recip( rhs );
	return quadquad( snh::mul( m_X, r.m_Value ), snh::mul( m_Y, r.m_Value ), snh::mul( m_Z, r.m_Value ), snh::mul( m_W, r.m_Value ) );
}

SNML_INLINE void quadquad::operator +=( quadquad_arg rhs )
{
	m_X = snh::add( m_X, rhs.m_X ); m_Y = snh::add( m_Y, rhs.m_Y ); m_Z = snh::add( m_Z, rhs.m_Z ); m_W = snh::add( m_W, rhs.m_W );
}

SNML_INLINE void quadquad::operator -=( quadquad_arg rhs )
{
	m_X = snh::sub( m_X, rhs.m_X ); m_Y = snh::sub( m_Y, rhs.m_Y ); m_Z = snh::sub( m_Z, rhs.m_Z ); m_W = snh::sub( m_W, rhs.m_W );
}

SNML_INLINE void quadquad::operator *=( quadquad_arg rhs )
{
	m_X = snh::mul( m_X, rhs.m_X );
	m_Y = snh::mul( m_Y, rhs.m_Y );
	m_Z = snh::mul( m_Z, rhs.m_Z );
	m_W = snh::mul( m_W, rhs.m_W );
}

SNML_INLINE void quadquad::operator /=( quadquad_arg rhs )
{
	quadquad r = recip( rhs );
	load( snh::mul( m_X, r.m_X ), snh::mul( m_Y, r.m_Y ), snh::mul( m_Z, r.m_Z ), snh::mul( m_W, r.m_W ) );
}

SNML_INLINE void quadquad::operator +=( scalar_arg rhs )
{
	load( snh::add( m_X, rhs.m_Value ), snh::add( m_Y, rhs.m_Value ), snh::add( m_Z, rhs.m_Value ), snh::add( m_W, rhs.m_Value ) );
}

SNML_INLINE void quadquad::operator -=( scalar_arg rhs )
{
	load( snh::sub( m_X, rhs.m_Value ), snh::sub( m_Y, rhs.m_Value ), snh::sub( m_Z, rhs.m_Value ), snh::sub( m_W, rhs.m_Value ) );
}

SNML_INLINE void quadquad::operator *=( scalar_arg rhs )
{
	load( snh::mul( m_X, rhs.m_Value ), snh::mul( m_Y, rhs.m_Value ), snh::mul( m_Z, rhs.m_Value ), snh::mul( m_W, rhs.m_Value ) );
}

SNML_INLINE void quadquad::operator /=( scalar_arg rhs )
{
	scalar r = recip( rhs );
	load( snh::add( m_X, r.m_Value ), snh::add( m_Y, r.m_Value ), snh::add( m_Z, r.m_Value ), snh::add( m_W, r.m_Value ) );
}

SNML_INLINE quadquad quadquad::operator -() const
{
	NativeFloat zero = snh::zero();
	return quadquad( snh::sub( zero, m_X ), snh::sub( zero, m_Y ), snh::sub( zero, m_Z ), snh::sub( zero, m_W ) );
}

SNML_INLINE quadquad quadquad::operator +() const
{
	return *this;
}

SNML_INLINE bool quadquad::operator ==( quadquad_arg rhs ) const
{
	return
		vec_all_eq(
			vec_or(
				vec_or( vec_sub( m_X, rhs.m_X ), vec_sub( m_Y, rhs.m_Y ) ),
				vec_or( vec_sub( m_Z, rhs.m_Z ), vec_sub( m_W, rhs.m_W ) )
			),
			snh::zero()
		)
	;
}

SNML_INLINE bool quadquad::operator !=( quadquad_arg rhs ) const
{
	return
		vec_any_ne(
			vec_or(
				vec_or( vec_sub( m_X, rhs.m_X ), vec_sub( m_Y, rhs.m_Y ) ),
				vec_or( vec_sub( m_Z, rhs.m_Z ), vec_sub( m_W, rhs.m_W ) )
			),
			snh::zero()
		)
	;
}

SNML_INLINE bool any_ge( quadquad_arg x, quadquad_arg y )
{
	return vec_any_ge( x.m_X, y.m_X ) || vec_any_ge( x.m_Y, y.m_Y ) || vec_any_ge( x.m_Z, y.m_Z ) || vec_any_ge( x.m_W, y.m_W );
}

SNML_INLINE bool any_gt( quadquad_arg x, quadquad_arg y )
{
	return vec_any_gt( x.m_X,y.m_X ) || vec_any_gt( x.m_Y,y.m_Y ) || vec_any_gt( x.m_Z,y.m_Z ) || vec_any_gt( x.m_W,y.m_W );
}

SNML_INLINE bool any_le( quadquad_arg x, quadquad_arg y )
{
	return vec_any_le( x.m_X, y.m_X ) || vec_any_le( x.m_Y, y.m_Y ) || vec_any_le( x.m_Z, y.m_Z ) || vec_any_le( x.m_W, y.m_W );
}

SNML_INLINE bool any_lt( quadquad_arg x, quadquad_arg y )
{
	return vec_any_lt( x.m_X,y.m_X ) || vec_any_lt( x.m_Y,y.m_Y ) || vec_any_lt( x.m_Z,y.m_Z ) || vec_any_lt( x.m_W,y.m_W );
}

SNML_INLINE bool any_eq( quadquad_arg x, quadquad_arg y )
{
	return vec_any_eq( x.m_X, y.m_X ) || vec_any_eq( x.m_Y, y.m_Y ) || vec_any_eq( x.m_Z, y.m_Z ) || vec_any_eq( x.m_W, y.m_W );
}

SNML_INLINE bool any_ne( quadquad_arg x, quadquad_arg y )
{
	return vec_any_ne( x.m_X, y.m_X ) || vec_any_ne( x.m_Y, y.m_Y ) || vec_any_ne( x.m_Z, y.m_Z ) || vec_any_ne( x.m_W, y.m_W );
}

SNML_INLINE bool all_ge( quadquad_arg x, quadquad_arg y )
{
	return vec_all_ge( x.m_X, y.m_X ) && vec_all_ge( x.m_Y, y.m_Y ) && vec_all_ge( x.m_Z, y.m_Z ) && vec_all_ge( x.m_W, y.m_W );
}

SNML_INLINE bool all_gt( quadquad_arg x, quadquad_arg y )
{
	return vec_all_gt( x.m_X,y.m_X ) && vec_all_gt( x.m_Y,y.m_Y ) && vec_all_gt( x.m_Z,y.m_Z ) && vec_all_gt( x.m_W,y.m_W );
}

SNML_INLINE bool all_le( quadquad_arg x, quadquad_arg y )
{
	return vec_all_le( x.m_X, y.m_X ) && vec_all_le( x.m_Y, y.m_Y ) && vec_all_le( x.m_Z, y.m_Z ) && vec_all_le( x.m_W, y.m_W );
}

SNML_INLINE bool all_lt( quadquad_arg x, quadquad_arg y )
{
	return vec_all_lt( x.m_X,y.m_X ) && vec_all_lt( x.m_Y,y.m_Y ) && vec_all_lt( x.m_Z,y.m_Z ) && vec_all_lt( x.m_W,y.m_W );
}

SNML_INLINE bool all_eq( quadquad_arg x, quadquad_arg y )
{
	return vec_all_eq( x.m_X, y.m_X ) && vec_all_eq( x.m_Y, y.m_Y ) && vec_all_eq( x.m_Z, y.m_Z ) && vec_all_eq( x.m_W, y.m_W );
}

SNML_INLINE bool all_ne( quadquad_arg x, quadquad_arg y )
{
	return vec_all_ne( x.m_X, y.m_X ) && vec_all_ne( x.m_Y, y.m_Y ) && vec_all_ne( x.m_Z, y.m_Z ) && vec_all_ne( x.m_W, y.m_W );
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

SNML_INLINE quadquad sin( quadquad_arg x )
{
	return quadquad( snh::sin( x.m_X ), snh::sin( x.m_Y ), snh::sin( x.m_Z ), snh::sin( x.m_W ) );
}

SNML_INLINE quadquad cos( quadquad_arg x )
{
	return quadquad( snh::cos( x.m_X ), snh::cos( x.m_Y ), snh::cos( x.m_Z ), snh::cos( x.m_W ) );
}

SNML_INLINE quadquad tan( quadquad_arg x )
{
	return quadquad( snh::tan( x.m_X ), snh::tan( x.m_Y ), snh::tan( x.m_Z ), snh::tan( x.m_W ) );
}

SNML_INLINE quadquad asin( quadquad_arg x )
{
	return quadquad( snh::asin( x.m_X ), snh::asin( x.m_Y ), snh::asin( x.m_Z ), snh::asin( x.m_W ) );
}

SNML_INLINE quadquad acos( quadquad_arg x )
{
	return quadquad( snh::acos( x.m_X ), snh::acos( x.m_Y ), snh::acos( x.m_Z ), snh::acos( x.m_W ) );
}

SNML_INLINE quadquad exp( quadquad_arg x )
{
	return quadquad( snh::exp( x.m_X ), snh::exp( x.m_Y ), snh::exp( x.m_Z ), snh::exp( x.m_W ) );
}

SNML_INLINE quadquad log( quadquad_arg x )
{
	return quadquad( snh::log( x.m_X ), snh::log( x.m_Y ), snh::log( x.m_Z ), snh::log( x.m_W ) );
}

SNML_INLINE quadquad pow( quadquad_arg x, quadquad_arg power )
{
	return quadquad( snh::pow( x.m_X, power.m_X ), snh::pow( x.m_Y, power.m_Y ), snh::pow( x.m_Z, power.m_Z ), snh::pow( x.m_W, power.m_W ) );
}

SNML_INLINE quadquad atan2( quadquad_arg y, quadquad_arg x )
{
	return quadquad( snh::atan2( y.m_X, x.m_X ), snh::atan2( y.m_Y, x.m_Y ), snh::atan2( y.m_Z, x.m_Z ), snh::atan2( y.m_W, x.m_W ) );
}

SNML_INLINE quadquad min( quadquad_arg x, quadquad_arg y )
{
	return quadquad( snh::min( x.m_X, y.m_X ), snh::min( x.m_Y, y.m_Y ), snh::min( x.m_Z, y.m_Z ), snh::min( x.m_W, y.m_W ) );
}

SNML_INLINE quadquad max( quadquad_arg x, quadquad_arg y )
{
	return quadquad( snh::max( x.m_X, y.m_X ), snh::max( x.m_Y, y.m_Y ), snh::max( x.m_Z, y.m_Z ), snh::max( x.m_W, y.m_W ) );
}

SNML_INLINE quadquad sgn( quadquad_arg x )
{
	return quadquad( snh::sgn( x.m_X ), snh::sgn( x.m_Y ), snh::sgn( x.m_Z ), snh::sgn( x.m_W ) );
}

SNML_INLINE quadquad abs( quadquad_arg x )
{
	return quadquad( snh::abs( x.m_X ), snh::abs( x.m_Y ), snh::abs( x.m_Z ), snh::abs( x.m_W ) );
}

SNML_INLINE quadquad sqrt( quadquad_arg x )
{
	return quadquad( snh::sqrt( x.m_X ), snh::sqrt( x.m_Y ), snh::sqrt( x.m_Z ), snh::sqrt( x.m_W ) );
}

SNML_INLINE quadquad rsqrt( quadquad_arg x )
{
	return quadquad( snh::rsqrt( x.m_X ), snh::rsqrt( x.m_Y ), snh::rsqrt( x.m_Z ), snh::rsqrt( x.m_W ) );
}
				
SNML_INLINE quadquad recip( quadquad_arg x )
{
	return quadquad( snh::recip( x.m_X ), snh::recip( x.m_Y ), snh::recip( x.m_Z ), snh::recip( x.m_W ) );
}
				
SNML_INLINE quadquad select_ge( quadquad_arg x, quadquad_arg y, quadquad_arg t, quadquad_arg f )
{
	return quadquad( snh::select_ge( x.m_X, y.m_X, t.m_X, f.m_X ), snh::select_ge( x.m_Y, y.m_Y, t.m_Y, f.m_Y ), snh::select_ge( x.m_Z, y.m_Z, t.m_Z, f.m_Z ), snh::select_ge( x.m_W, y.m_W, t.m_W, f.m_W ) );
}

SNML_INLINE quadquad select_gt( quadquad_arg x, quadquad_arg y, quadquad_arg t, quadquad_arg f )
{
	return quadquad( snh::select_gt( x.m_X, y.m_X, t.m_X, f.m_X ), snh::select_gt( x.m_Y, y.m_Y, t.m_Y, f.m_Y ), snh::select_gt( x.m_Z, y.m_Z, t.m_Z, f.m_Z ), snh::select_gt( x.m_W, y.m_W, t.m_W, f.m_W ) );
}

SNML_INLINE quadquad select_le( quadquad_arg x, quadquad_arg y, quadquad_arg t, quadquad_arg f )
{
	return quadquad( snh::select_le( x.m_X, y.m_X, t.m_X, f.m_X ), snh::select_le( x.m_Y, y.m_Y, t.m_Y, f.m_Y ), snh::select_le( x.m_Z, y.m_Z, t.m_Z, f.m_Z ), snh::select_le( x.m_W, y.m_W, t.m_W, f.m_W ) );
}

SNML_INLINE quadquad select_lt( quadquad_arg x, quadquad_arg y, quadquad_arg t, quadquad_arg f )
{
	return quadquad( snh::select_lt( x.m_X, y.m_X, t.m_X, f.m_X ), snh::select_lt( x.m_Y, y.m_Y, t.m_Y, f.m_Y ), snh::select_lt( x.m_Z, y.m_Z, t.m_Z, f.m_Z ), snh::select_lt( x.m_W, y.m_W, t.m_W, f.m_W ) );
}

SNML_INLINE quadquad select_eq( quadquad_arg x, quadquad_arg y, quadquad_arg t, quadquad_arg f )
{
	return quadquad( snh::select_eq( x.m_X, y.m_X, t.m_X, f.m_X ), snh::select_eq( x.m_Y, y.m_Y, t.m_Y, f.m_Y ), snh::select_eq( x.m_Z, y.m_Z, t.m_Z, f.m_Z ), snh::select_eq( x.m_W, y.m_W, t.m_W, f.m_W ) );
}

SNML_INLINE quadquad select_ne( quadquad_arg x, quadquad_arg y, quadquad_arg t, quadquad_arg f )
{
	return quadquad( snh::select_ne( x.m_X, y.m_X, t.m_X, f.m_X ), snh::select_ne( x.m_Y, y.m_Y, t.m_Y, f.m_Y ), snh::select_ne( x.m_Z, y.m_Z, t.m_Z, f.m_Z ), snh::select_ne( x.m_W, y.m_W, t.m_W, f.m_W ) );
}

// TODO: make this more efficient.
SNML_INLINE quadquad select( bool sel, quadquad_arg t, quadquad_arg f )
{
	return sel ? t : f;
}

SNML_INLINE quadquad::operator float() const
{
	return (float&)m_X;
}

SNML_INLINE quadquad quadquad::unit4x4()
{
	return quadquad( (NativeFloat)( 1, 0, 0, 0 ), (NativeFloat)( 0, 1, 0, 0 ), (NativeFloat)( 0, 0, 1, 0 ), (NativeFloat)( 0, 0, 0, 1 ) );
}

SNML_INLINE scalar sum_sideways( quadquad_arg rhs )
{
	return snh::sum4( snh::add( snh::add( rhs.m_X, rhs.m_Y ), snh::add( rhs.m_Z, rhs.m_W ) ) );
}

SNML_INLINE scalar max_sideways( quadquad_arg rhs )
{
	return snh::max4( snh::max( snh::max( rhs.m_X, rhs.m_Y ), snh::max( rhs.m_Z, rhs.m_W ) ) );
}

SNML_INLINE scalar min_sideways( quadquad_arg rhs )
{
	return snh::min4( snh::min( snh::min( rhs.m_X, rhs.m_Y ), snh::min( rhs.m_Z, rhs.m_W ) ) );
}

SNML_INLINE void quadquad::Rotate( scalar_arg angle, vquad_arg xyz )
{
	return MultQuat( axis_angle( xyz, angle * scalar(3.14159265359f/180) ) );
}

SNML_INLINE void quadquad::Translate( vquad_arg xyz )
{
	m_W = snh::madd( m_X, snh::x( xyz.m_Value ), snh::madd( m_Y, snh::y( xyz.m_Value ), snh::madd( m_Z, snh::z( xyz.m_Value ), m_W ) ) );
}

SNML_INLINE void quadquad::Scale( vquad_arg xyz )
{
	m_X = snh::mul( m_X, snh::x( xyz.m_Value ) );
	m_Y = snh::mul( m_Y, snh::y( xyz.m_Value ) );
	m_Z = snh::mul( m_Z, snh::z( xyz.m_Value ) );
}

SNML_INLINE void quadquad::MultMatrix( quadquad_arg rhs )
{
	NativeFloat x = snh::madd( m_X, snh::x( rhs.m_X ), snh::madd( m_Y, snh::y( rhs.m_X ), snh::madd( m_Z, snh::z( rhs.m_X ), snh::mul( m_W, snh::w( rhs.m_X ) ) ) ) );
	NativeFloat y = snh::madd( m_X, snh::x( rhs.m_Y ), snh::madd( m_Y, snh::y( rhs.m_Y ), snh::madd( m_Z, snh::z( rhs.m_Y ), snh::mul( m_W, snh::w( rhs.m_Y ) ) ) ) );
	NativeFloat z = snh::madd( m_X, snh::x( rhs.m_Z ), snh::madd( m_Y, snh::y( rhs.m_Z ), snh::madd( m_Z, snh::z( rhs.m_Z ), snh::mul( m_W, snh::w( rhs.m_Z ) ) ) ) );
	NativeFloat w = snh::madd( m_X, snh::x( rhs.m_W ), snh::madd( m_Y, snh::y( rhs.m_W ), snh::madd( m_Z, snh::z( rhs.m_W ), snh::mul( m_W, snh::w( rhs.m_W ) ) ) ) );
	m_X = x; m_Y = y; m_Z = z; m_W = w;
}

SNML_INLINE void quadquad::MultQuat( vquad_arg rhs )
{
	NativeFloat Conj = conj( rhs ).m_Value;
	NativeFloat x = snh::quat_mul( m_X, Conj );
	NativeFloat y = snh::quat_mul( m_Y, Conj );
	NativeFloat z = snh::quat_mul( m_Z, Conj );
	m_X = snh::quat_mul( rhs.m_Value, x );
	m_Y = snh::quat_mul( rhs.m_Value, y );
	m_Z = snh::quat_mul( rhs.m_Value, z );
}

SNML_INLINE void quadquad::Perspective( scalar_arg fieldOfView, scalar_arg aspectRatio, scalar_arg _near, scalar_arg _far )
{
	scalar cot = recip( tan( fieldOfView * scalar( 0.5f * 3.14159265359f/180 ) ) );
	scalar zero = scalar::zero();
	quadquad matrix(
		vquad( cot / aspectRatio, zero, zero, zero ),
		vquad( zero, cot, zero, zero ),
		vquad( zero, zero, ( _near + _far ) / ( _near - _far ), -scalar::one() ),
		vquad( zero, zero, scalar::two() * _near * _far / ( _near - _far ), zero )
	);
	MultMatrix( matrix );
}

SNML_INLINE void quadquad::Ortho( scalar_arg left, scalar_arg right, scalar_arg bottom, scalar_arg top, scalar_arg _near, scalar_arg _far )
{
	scalar zero = scalar::zero();
	quadquad matrix(
		vquad( scalar::two()/(right - left), zero, zero, zero ),
		vquad( zero, scalar::two()/(top - bottom), zero, zero ),
		vquad( zero, zero, scalar::two()/(_near - _far), zero ),
		vquad( -(right + left)/(right - left), -(top+ bottom)/(top - bottom), (_near + _far)/(_near - _far), scalar::one() )
	);
	MultMatrix( matrix );
}

SNML_INLINE void quadquad::LookAt( vquad_arg eyePos, vquad_arg center, vquad_arg up )
{
	vquad z = eyePos - center, x = cross( up, z ), y = cross( z, x );
	quadquad matrix( x * rlength3( x ), y * rlength3( y ), z * rlength3( z ), vquad::unit() );
	quadquad matrixt;
	Transpose4x4( matrixt, matrix );
	MultMatrix( matrixt );
	Translate( -eyePos );
}

// see Inv4x4.mws
SNML_INLINE quadquad &Inverse4x4( quadquad &result, quadquad &rhs )
{
	NativeFloat WwWxWwZx = snh::perm<'wxwX'>( rhs.m_W, rhs.m_Z );
	NativeFloat WwWyWwZy = snh::perm<'wywY'>( rhs.m_W, rhs.m_Z );
	NativeFloat WxWwWxZw = snh::perm<'xwxW'>( rhs.m_W, rhs.m_Z );
	NativeFloat WxWyWxZy = snh::perm<'xyxY'>( rhs.m_W, rhs.m_Z );
	NativeFloat WxWzWxZz = snh::perm<'xzxZ'>( rhs.m_W, rhs.m_Z );
	NativeFloat WyWwWyZw = snh::perm<'ywyW'>( rhs.m_W, rhs.m_Z );
	NativeFloat WyWxWyZx = snh::perm<'yxyX'>( rhs.m_W, rhs.m_Z );
	NativeFloat WyWzWyZz = snh::perm<'yzyZ'>( rhs.m_W, rhs.m_Z );
	NativeFloat WzWxWzZx = snh::perm<'zxzX'>( rhs.m_W, rhs.m_Z );
	NativeFloat WzWyWzZy = snh::perm<'zyzY'>( rhs.m_W, rhs.m_Z );

	NativeFloat YwXwXwXw = snh::perm<'wWWW'>( rhs.m_Y, rhs.m_X );
	NativeFloat YxXxXxXx = snh::perm<'xXXX'>( rhs.m_Y, rhs.m_X );
	NativeFloat YyXyXyXy = snh::perm<'yYYY'>( rhs.m_Y, rhs.m_X );
	NativeFloat YzXzXzXz = snh::perm<'zZZZ'>( rhs.m_Y, rhs.m_X );

	NativeFloat ZwZxYwYx = snh::perm<'wxWX'>( rhs.m_Z, rhs.m_Y );
	NativeFloat ZwZyYwYy = snh::perm<'wyWY'>( rhs.m_Z, rhs.m_Y );
	NativeFloat ZxZwYxYw = snh::perm<'xwXW'>( rhs.m_Z, rhs.m_Y );
	NativeFloat ZxZyYxYy = snh::perm<'xyXY'>( rhs.m_Z, rhs.m_Y );
	NativeFloat ZxZzYxYz = snh::perm<'xzXZ'>( rhs.m_Z, rhs.m_Y );
	NativeFloat ZyZwYyYw = snh::perm<'ywYW'>( rhs.m_Z, rhs.m_Y );
	NativeFloat ZyZxYyYx = snh::perm<'yxYX'>( rhs.m_Z, rhs.m_Y );
	NativeFloat ZyZzYyYz = snh::perm<'yzYZ'>( rhs.m_Z, rhs.m_Y );
	NativeFloat ZzZxYzYx = snh::perm<'zxZX'>( rhs.m_Z, rhs.m_Y );
	NativeFloat ZzZyYzYy = snh::perm<'zyZY'>( rhs.m_Z, rhs.m_Y );
	
	NativeFloat WzZzWzYz = vec_sld( snh::perm<'zZzZ'>( rhs.m_W, rhs.m_Z ), snh::perm<'zZzZ'>( rhs.m_W, rhs.m_Y ), 8 );
	NativeFloat WwZwWwYw = vec_sld( snh::perm<'wWwW'>( rhs.m_W, rhs.m_Z ), snh::perm<'wWwW'>( rhs.m_W, rhs.m_Y ), 8 );
	NativeFloat ZwWwYwZw = vec_sld( snh::perm<'wWwW'>( rhs.m_Z, rhs.m_W ), snh::perm<'wWwW'>( rhs.m_Y, rhs.m_Z ), 8 );
	NativeFloat ZzWzYzZz = vec_sld( snh::perm<'zZzZ'>( rhs.m_Z, rhs.m_W ), snh::perm<'zZzZ'>( rhs.m_Y, rhs.m_Z ), 8 );
	
	NativeFloat mul_ZzZyYzYy_WyWzWyZz = snh::mul(ZzZyYzYy,WyWzWyZz);
	NativeFloat mul_ZyZzYyYz_WzWyWzZy = snh::mul(ZyZzYyYz,WzWyWzZy);
	NativeFloat mul_ZyZwYyYw_WwWyWwZy = snh::mul(ZyZwYyYw,WwWyWwZy);
	NativeFloat mul_ZwZyYwYy_WyWwWyZw = snh::mul(ZwZyYwYy,WyWwWyZw);
	NativeFloat mul_WzZzWzYz_ZwWwYwZw = snh::mul(WzZzWzYz,ZwWwYwZw);
	NativeFloat mul_ZzWzYzZz_WwZwWwYw = snh::mul(ZzWzYzZz,WwZwWwYw);
	NativeFloat mul_ZxZzYxYz_WzWxWzZx = snh::mul(ZxZzYxYz,WzWxWzZx);
	NativeFloat mul_ZzZxYzYx_WxWzWxZz = snh::mul(ZzZxYzYx,WxWzWxZz);
	NativeFloat mul_ZwZxYwYx_WxWwWxZw = snh::mul(ZwZxYwYx,WxWwWxZw);
	NativeFloat mul_ZxZwYxYw_WwWxWwZx = snh::mul(ZxZwYxYw,WwWxWwZx);
	NativeFloat mul_ZyZxYyYx_WxWyWxZy = snh::mul(ZyZxYyYx,WxWyWxZy);
	NativeFloat mul_ZxZyYxYy_WyWxWyZx = snh::mul(ZxZyYxYy,WyWxWyZx);

	NativeFloat x = snh::nmsub(YwXwXwXw,mul_ZzZyYzYy_WyWzWyZz,snh::madd(YwXwXwXw,mul_ZyZzYyYz_WzWyWzZy,snh::nmsub(YzXzXzXz,mul_ZyZwYyYw_WwWyWwZy,snh::madd(YzXzXzXz,mul_ZwZyYwYy_WyWwWyZw,snh::nmsub(YyXyXyXy,mul_WzZzWzYz_ZwWwYwZw,snh::mul(YyXyXyXy,mul_ZzWzYzZz_WwZwWwYw))))));
	NativeFloat y = snh::nmsub(YwXwXwXw,mul_ZxZzYxYz_WzWxWzZx,snh::madd(YwXwXwXw,mul_ZzZxYzYx_WxWzWxZz,snh::nmsub(YzXzXzXz,mul_ZwZxYwYx_WxWwWxZw,snh::madd(YzXzXzXz,mul_ZxZwYxYw_WwWxWwZx,snh::nmsub(YxXxXxXx,mul_ZzWzYzZz_WwZwWwYw,snh::mul(YxXxXxXx,mul_WzZzWzYz_ZwWwYwZw))))));
	NativeFloat z = snh::nmsub(YwXwXwXw,mul_ZyZxYyYx_WxWyWxZy,snh::madd(YwXwXwXw,mul_ZxZyYxYy_WyWxWyZx,snh::nmsub(YyXyXyXy,mul_ZxZwYxYw_WwWxWwZx,snh::madd(YyXyXyXy,mul_ZwZxYwYx_WxWwWxZw,snh::nmsub(YxXxXxXx,mul_ZwZyYwYy_WyWwWyZw,snh::mul(YxXxXxXx,mul_ZyZwYyYw_WwWyWwZy))))));
	NativeFloat w = snh::nmsub(YzXzXzXz,mul_ZxZyYxYy_WyWxWyZx,snh::madd(YzXzXzXz,mul_ZyZxYyYx_WxWyWxZy,snh::nmsub(YyXyXyXy,mul_ZzZxYzYx_WxWzWxZz,snh::madd(YyXyXyXy,mul_ZxZzYxYz_WzWxWzZx,snh::nmsub(YxXxXxXx,mul_ZyZzYyYz_WzWyWzZy,snh::mul(YxXxXxXx,mul_ZzZyYzYy_WyWzWyZz))))));
	
	NativeFloat tx = vec_mergeh( vec_mergeh( x, z ), vec_mergeh( y, w ) );
	NativeFloat recipdet = snh::recip( snh::dot4( tx, rhs.m_X ) );

	result.load( snh::mul( x, recipdet ), snh::mul( y, recipdet ), snh::mul( z, recipdet ), snh::mul( w, recipdet ) );
	return result;
}

SNML_INLINE quadquad &Transpose4x4( quadquad &result, quadquad &rhs )
{
	NativeFloat x = vec_mergeh( rhs.m_X, rhs.m_Z );
	NativeFloat y = vec_mergeh( rhs.m_Y, rhs.m_W );
	NativeFloat z = vec_mergel( rhs.m_X, rhs.m_Z );
	NativeFloat w = vec_mergel( rhs.m_Y, rhs.m_W );
	result.load(
		vec_mergeh( x, y ),
		vec_mergel( x, y ),
		vec_mergeh( z, w ),
		vec_mergel( z, w )
	);
	return result;
}

SNML_INLINE scalar Determinant4x4( quadquad & rhs )
{
	NativeFloat WwWxWwWx = snh::perm<'wxwx'>( rhs.m_W, rhs.m_W );
	NativeFloat WyWwWxWz = snh::perm<'ywxz'>( rhs.m_W, rhs.m_W );
	NativeFloat WyWzWxWy = snh::perm<'yzxy'>( rhs.m_W, rhs.m_W );
	NativeFloat WzWxWyWx = snh::perm<'zxyx'>( rhs.m_W, rhs.m_W );
	NativeFloat YwYwYwYz = snh::perm<'wwwz'>( rhs.m_Y, rhs.m_Y );
	NativeFloat YyYxYxYx = snh::perm<'yxxx'>( rhs.m_Y, rhs.m_Y );
	NativeFloat YzYzYyYy = snh::perm<'zzyy'>( rhs.m_Y, rhs.m_Y );
	NativeFloat ZwZxZwZx = snh::perm<'wxwx'>( rhs.m_Z, rhs.m_Z );
	NativeFloat ZyZwZxZz = snh::perm<'ywxz'>( rhs.m_Z, rhs.m_Z );
	NativeFloat ZyZzZxZy = snh::perm<'yzxy'>( rhs.m_Z, rhs.m_Z );
	NativeFloat ZzZxZyZx = snh::perm<'zxyx'>( rhs.m_Z, rhs.m_Z );

	NativeFloat WzZzZwZy = snh::perm<'zZWY'>( rhs.m_W, rhs.m_Z );
	NativeFloat ZwWwWyWz = snh::perm<'wWYZ'>( rhs.m_Z, rhs.m_W );

	NativeFloat WwZwWwWy = snh::perm<'wWwy'>( rhs.m_W, rhs.m_Z );
	NativeFloat ZzWzZyZz = snh::perm<'zZyz'>( rhs.m_Z, rhs.m_W );
	
	NativeFloat tx = snh::nmsub(YwYwYwYz,snh::mul(ZzZxZyZx,WyWzWxWy),snh::madd(YwYwYwYz,snh::mul(ZyZzZxZy,WzWxWyWx),snh::nmsub(YzYzYyYy,snh::mul(ZyZwZxZz,WwWxWwWx),snh::madd(YzYzYyYy,snh::mul(ZwZxZwZx,WyWwWxWz),snh::nmsub(YyYxYxYx,snh::mul(WzZzZwZy,ZwWwWyWz),snh::mul(YyYxYxYx,snh::mul(ZzWzZyZz,WwZwWwWy)))))));
	//printf( "tx=%s\n", ((vquad&)tx).ToString() );
	return snh::dot4( tx, rhs.m_X );
}

SNML_INLINE quadquad &Inverse3x4( quadquad &result, quadquad &rhs )
{
	scalar det = dot4( cross( rhs.y(), rhs.z() ), rhs.x() );
	scalar recipdet = recip( det );
	vquad minusw = zerow( -rhs.w() * recipdet );
	vquad cx = cross( rhs.y(), rhs.z() );
	vquad cy = cross( rhs.z(), rhs.x() );
	vquad cz = cross( rhs.x(), rhs.y() );
	vquad cw = vquad::zero();
	NativeFloat x = vec_mergeh( cx.m_Value, cz.m_Value );
	NativeFloat y = vec_mergeh( cy.m_Value, cw.m_Value );
	NativeFloat z = vec_mergel( cx.m_Value, cz.m_Value );
	NativeFloat w = vec_mergel( cy.m_Value, cw.m_Value );
	vquad trans_x = vec_mergeh( x, y );
	vquad trans_y = vec_mergel( x, y );
	vquad trans_z = vec_mergeh( z, w );
	result.load
	(
		trans_x * recipdet,
		trans_y * recipdet,
		trans_z * recipdet,
		trans_x * minusw.x() + trans_y * minusw.y() + trans_z * minusw.z() + vquad::unit()
	);
	return result;
}

SNML_INLINE scalar Determinant3x4( quadquad & rhs )
{
	return dot4( cross( rhs.y(), rhs.z() ), rhs.x() );
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
	// TODO: finish this
	/*{
		vquad x = rhs.x(), y = rhs.y(), z = rhs.z(), w = rhs.w();
		scalar angle = scalar::half() * atan2( scalar::two() * x.y(), y.y() - x.x() );
		scalar c = cosf( angle ), s = sinf( angle );

		vquad tmp = x * c - y * s;
		y = x * s + y * c;
		x = tmp;

		NativeFloat r1 = snh::perm<'xX00'>( c.m_Value, (-s).m_Value );
		NativeFloat r2 = snh::perm<'xX00'>( s.m_Value, c.m_Value );
		rhs.load(
			snh::perm<'xyXY'>( snh::madd( x.access(), r1, snh::mul( snh::perm<'yx00'>( x.access(), x.access() ), r2 ) ), x.access() ),
			snh::perm<'xyXY'>( snh::madd( x.access(), r1, snh::mul( snh::perm<'yx00'>( x.access(), x.access() ), r2 ) ), x.access() ),
			snh::perm<'xyXY'>( snh::madd( x.access(), r1, snh::mul( snh::perm<'yx00'>( x.access(), x.access() ), r2 ) ), x.access() ),
			snh::perm<'xyXY'>( snh::madd( x.access(), r1, snh::mul( snh::perm<'yx00'>( x.access(), x.access() ), r2 ) ), x.access() )
		);
	}*/

	/*vquad xy, xz, yz;
	xy = Jacobi( rhs, 0, 1 );
	xz = Jacobi( rhs, 0, 2 );
	yz = Jacobi( rhs, 1, 2 );
	vquad result( rhs.x().x(), rhs.y().y(), rhs.z().z(), scalar( 0 ) );
	MakeEigenvectors( eigenvectors, 0, 1, xy );
	MakeEigenvectors( eigenvectors, 0, 2, xz );
	MakeEigenvectors( eigenvectors, 1, 2, yz );*/
	return vquad();
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

SNML_INLINE const NativeQuad &quadquad::access_w() const
{
	return m_W;
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
