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
//     VFPU vector math class sample class definition
//
//     Do not include this file directly.
//
//     There are three base types:
//    
//     name      # floats  use
//     ==========================================================
//     scalar    1         scalar constants
//     vquad     4         pair, tripple and quad ops, SOA ops, quaternions
//     quadquad 16         matrices or SOA ops
//    
//     arithmetic is arranged so that these elements can also be used in Structures Of Arrays
//     implementations (SOA).
//    
//     scalar type.
//     This avoids the use of floating point types which are expensive to covert to vector scalars.
//     For the fastest code, do not use "float" at all.
//
//
//  Operations:
//
//     All classes support
//
//     aritmetic:                 a + b    a - b    a * b    a / b   -a
//     in-place aritmetic:        a += b   a -= b   a *= b   a /= b
//     conversion to/from float:  (float)a   a = (float)
//     load/store:                a.load( memory )
//     fast constants:            zero() .. sqrt3_by_2()
//     branchless selection:      select_ge()...
//     debug support:             ToString()
//     
//     In addition, some classes support vector and matrix operations
//     
//
//  Change History:
//
//  Vers    Date            Author        Changes
//  1.00    09-08-2005      AndyT         Created
//
////////////////////////////////////////////////////////////////////////////////

#ifndef SNML_INLINE
	#ifdef _DEBUG
		#define SNML_INLINE
		#define SNML_NOINLINE
	#else
		#define SNML_INLINE inline __attribute__( ( always_inline ) )
		#define SNML_NOINLINE __attribute__( ( noinline ) )
	#endif
	#define SNML_INLINE2 inline __attribute__( ( always_inline ) )
#endif

class scalar;
class vquad;
class quadquad;

class scalar
{
public:
	NativeScalar m_Value;
	friend class vquad;
	friend class quadquad;

public:
	SNML_INLINE scalar();
	SNML_INLINE scalar( float value_f );
	SNML_INLINE scalar( int value_i );
	#ifndef SN_SCALAR_IS_FLOAT
		SNML_INLINE scalar( const NativeScalar &value_f );
	#endif
	SNML_INLINE scalar( const scalar &rhs );
	
	// import
	SNML_INLINE void load( const NativeScalar &x_s );
	SNML_INLINE void load( const float src[], const int byteOffset=0 );

	// export
	SNML_INLINE operator float() const;
	SNML_INLINE int ToInt() const;
	SNML_INLINE const NativeScalar &access() const;
	SNML_NOINLINE const char *ToString( const char *pFmt = "%g" ) const;
	SNML_INLINE void store( float dest[], const int byteOffset=0 ) const;

	SNML_INLINE scalar operator +( scalar_arg rhs ) const;
	SNML_INLINE scalar operator -( scalar_arg rhs ) const;
	SNML_INLINE scalar operator *( scalar_arg rhs ) const;
	SNML_INLINE scalar operator /( scalar_arg rhs ) const;
	SNML_INLINE void operator +=( scalar_arg rhs );
	SNML_INLINE void operator -=( scalar_arg rhs );
	SNML_INLINE void operator *=( scalar_arg rhs );
	SNML_INLINE void operator /=( scalar_arg rhs );
	SNML_INLINE scalar operator -() const;
	SNML_INLINE scalar operator +() const;

	SNML_INLINE vquad operator +( vquad_arg rhs ) const;
	SNML_INLINE vquad operator -( vquad_arg rhs ) const;
	SNML_INLINE vquad operator *( vquad_arg rhs ) const;
	SNML_INLINE vquad operator /( vquad_arg rhs ) const;

	SNML_INLINE quadquad operator +( quadquad_arg rhs ) const;
	SNML_INLINE quadquad operator -( quadquad_arg rhs ) const;
	SNML_INLINE quadquad operator *( quadquad_arg rhs ) const;
	SNML_INLINE quadquad operator /( quadquad_arg rhs ) const;

	SNML_INLINE bool operator ==( scalar_arg rhs ) const;
	SNML_INLINE bool operator !=( scalar_arg rhs ) const;

	// constants	
	SNML_INLINE static scalar zero();
	SNML_INLINE static scalar one();
	SNML_INLINE static scalar two();
	SNML_INLINE static scalar three();
	SNML_INLINE static scalar four();
	SNML_INLINE static scalar five();
	SNML_INLINE static scalar six();
	SNML_INLINE static scalar seven();
	SNML_INLINE static scalar eight();
	SNML_INLINE static scalar nine();
	SNML_INLINE static scalar ten();
	SNML_INLINE static scalar half();
	SNML_INLINE static scalar quarter();
	SNML_INLINE static scalar three_quarters();
	SNML_INLINE static scalar three_by_2();
	SNML_INLINE static scalar huge();
	SNML_INLINE static scalar sqrt2();
	SNML_INLINE static scalar sqrt_half();
	SNML_INLINE static scalar two_by_sqrtpi();
	SNML_INLINE static scalar two_by_pi();
	SNML_INLINE static scalar one_by_pi();
	SNML_INLINE static scalar pi_by_4();
	SNML_INLINE static scalar pi_by_2();
	SNML_INLINE static scalar pi();
	SNML_INLINE static scalar e();
	SNML_INLINE static scalar log2e();
	SNML_INLINE static scalar log10e();
	SNML_INLINE static scalar ln2();
	SNML_INLINE static scalar ln10();
	SNML_INLINE static scalar two_pi();
	SNML_INLINE static scalar pi_by_6();
	SNML_INLINE static scalar log10two();
	SNML_INLINE static scalar log2ten();
	SNML_INLINE static scalar sqrt3_by_2();

	// std functions
	SNML_INLINE friend scalar sin( scalar_arg x );
	SNML_INLINE friend scalar cos( scalar_arg x );
	SNML_INLINE friend scalar tan( scalar_arg x );
	SNML_INLINE friend scalar asin( scalar_arg x );
	SNML_INLINE friend scalar acos( scalar_arg x );
	SNML_INLINE friend scalar exp( scalar_arg x );
	SNML_INLINE friend scalar log( scalar_arg x );
	SNML_INLINE friend scalar pow( scalar_arg x, scalar_arg power );
	SNML_INLINE friend scalar atan2( scalar_arg y, scalar_arg x );
	SNML_INLINE friend scalar min( scalar_arg x, scalar_arg y );
	SNML_INLINE friend scalar max( scalar_arg x, scalar_arg y );
	SNML_INLINE friend scalar sgn( scalar_arg x );
	SNML_INLINE friend scalar abs( scalar_arg x );
	SNML_INLINE friend scalar sqrt( scalar_arg x );
	SNML_INLINE friend scalar rsqrt( scalar_arg x );
	SNML_INLINE friend scalar recip( scalar_arg x );

	// x op y ? t : f
	SNML_INLINE friend scalar select_ge( scalar_arg x, scalar_arg y, scalar_arg t, scalar_arg f );
	SNML_INLINE friend scalar select_gt( scalar_arg x, scalar_arg y, scalar_arg t, scalar_arg f );
	SNML_INLINE friend scalar select_le( scalar_arg x, scalar_arg y, scalar_arg t, scalar_arg f );
	SNML_INLINE friend scalar select_lt( scalar_arg x, scalar_arg y, scalar_arg t, scalar_arg f );
	SNML_INLINE friend scalar select_eq( scalar_arg x, scalar_arg y, scalar_arg t, scalar_arg f );
	SNML_INLINE friend scalar select_ne( scalar_arg x, scalar_arg y, scalar_arg t, scalar_arg f );

	SNML_INLINE friend scalar select( bool sel, scalar_arg t, scalar_arg f );

	SNML_INLINE friend bool any_ge( vquad_arg x, vquad_arg y );
	SNML_INLINE friend bool any_gt( vquad_arg x, vquad_arg y );
	SNML_INLINE friend bool any_le( vquad_arg x, vquad_arg y );
	SNML_INLINE friend bool any_lt( vquad_arg x, vquad_arg y );
	SNML_INLINE friend bool any_eq( vquad_arg x, vquad_arg y );
	SNML_INLINE friend bool any_ne( vquad_arg x, vquad_arg y );

	SNML_INLINE friend bool all_ge( vquad_arg x, vquad_arg y );
	SNML_INLINE friend bool all_gt( vquad_arg x, vquad_arg y );
	SNML_INLINE friend bool all_le( vquad_arg x, vquad_arg y );
	SNML_INLINE friend bool all_lt( vquad_arg x, vquad_arg y );
	SNML_INLINE friend bool all_eq( vquad_arg x, vquad_arg y );
	SNML_INLINE friend bool all_ne( vquad_arg x, vquad_arg y );
};

// vquad type (4 ieee floats)
// Use this as a base for pair, triple and vquad types as well as an entry-level soa element
class vquad
{
public:
	NativeQuad m_Value;
	friend class scalar;
	friend class quadquad;
	SNML_INLINE friend quadquad &Inverse4x4( quadquad &result, quadquad &rhs );
	SNML_INLINE friend quadquad &Inverse3x4( quadquad &result, quadquad &rhs );
	SNML_INLINE friend scalar Determinant4x4( quadquad &rhs );
	SNML_INLINE friend scalar Determinant3x4( quadquad &rhs );
	SNML_INLINE friend quadquad &Transpose4x4( quadquad &result, quadquad &rhs );
public:

	SNML_INLINE vquad();
	SNML_INLINE vquad( float value_f );
	SNML_INLINE vquad( scalar_arg value_s );
	SNML_INLINE vquad( const NativeQuad value );
	SNML_INLINE vquad( float x_f, float y_f );
	SNML_INLINE vquad( scalar_arg x_s, scalar_arg y_s );
	SNML_INLINE vquad( float x_f, float y_f, float z_f );
	SNML_INLINE vquad( scalar_arg x_s, scalar_arg y_s, scalar_arg z_s );
	SNML_INLINE vquad( float x_f, float y_f, float z_f, float w_f );
	SNML_INLINE vquad( scalar_arg x_s, scalar_arg y_s, scalar_arg z_s, scalar_arg w_s );

	// import
	SNML_INLINE void load( const NativeScalar &x_s );
	SNML_INLINE void load( const NativeScalar &x_s, const NativeScalar &y_s );
	SNML_INLINE void load( const NativeScalar &x_s, const NativeScalar &y_s, const NativeScalar &z_s );
	SNML_INLINE void load( const NativeScalar &x_s, const NativeScalar &y_s, const NativeScalar &z_s, const NativeScalar &w_s );

	SNML_INLINE void load1( const float src[], const int byteOffset = 0 );
	SNML_INLINE void load_unaligned( const float src[], const int byteOffset = 0 );
	SNML_INLINE void load( const float src[], const int byteOffset = 0 );

	SNML_INLINE void load1( const signed int src[], const int byteOffset = 0 );
	SNML_INLINE void load_unaligned( const signed int src[], const int byteOffset = 0 );
	SNML_INLINE void load( const signed int src[], const int byteOffset = 0 );

	SNML_INLINE void load1( const signed short src[], const int byteOffset = 0 );
	SNML_INLINE void load_unaligned( const signed short src[], const int byteOffset = 0 );
	SNML_INLINE void load( const signed short src[], const int byteOffset = 0 );

	SNML_INLINE void load_nocache( const float src[], const int byteOffset = 0 ); // load from an aligned a pointer to uncached mem

	SNML_INLINE void set_x( scalar_arg value );
	SNML_INLINE void set_y( scalar_arg value );
	SNML_INLINE void set_z( scalar_arg value );
	SNML_INLINE void set_w( scalar_arg value );
	
	// export
	SNML_INLINE operator float() const;
	SNML_INLINE scalar x() const;
	SNML_INLINE scalar y() const;
	SNML_INLINE scalar z() const;
	SNML_INLINE scalar w() const;
	SNML_INLINE scalar operator[]( unsigned index ) const; // only use this with constant indices or for debugging.
	SNML_INLINE void store( NativeScalar &x_s ) const;
	SNML_INLINE void store( NativeScalar &x_s, NativeScalar &y_s ) const;
	SNML_INLINE void store( NativeScalar &x_s, NativeScalar &y_s, NativeScalar &z_s ) const;
	SNML_INLINE void store( NativeScalar &x_s, NativeScalar &y_s, NativeScalar &z_s, NativeScalar &w_s ) const;
	SNML_INLINE void store( float dest[] ) const;
	SNML_INLINE void store( signed int dest[] ) const;
	SNML_INLINE void store( signed short dest[] ) const;
	SNML_INLINE void store( signed char dest[] ) const;
	SNML_INLINE void store( unsigned int dest[] ) const;
	SNML_INLINE void store( unsigned short dest[] ) const;
	SNML_INLINE void store( unsigned char dest[] ) const;
	SNML_INLINE void store_nocache( float dest[] ) const; // store to an aligned a pointer to uncached mem
	SNML_INLINE const NativeQuad &access() const;
	SNML_NOINLINE const char *ToString( const char *pFmt = "%g" ) const;

	// arith ops
	SNML_INLINE vquad operator +( vquad_arg rhs ) const;
	SNML_INLINE vquad operator -( vquad_arg rhs ) const;
	SNML_INLINE vquad operator *( vquad_arg rhs ) const;
	SNML_INLINE vquad operator /( vquad_arg rhs ) const;
	SNML_INLINE void operator +=( vquad_arg rhs );
	SNML_INLINE void operator -=( vquad_arg rhs );
	SNML_INLINE void operator *=( vquad_arg rhs );
	SNML_INLINE void operator /=( vquad_arg rhs );
	SNML_INLINE vquad operator -() const;
	SNML_INLINE vquad operator +() const;
	
	SNML_INLINE vquad operator +( scalar_arg rhs ) const;
	SNML_INLINE vquad operator -( scalar_arg rhs ) const;
	SNML_INLINE vquad operator *( scalar_arg rhs ) const;
	SNML_INLINE vquad operator /( scalar_arg rhs ) const;
	SNML_INLINE void operator +=( scalar_arg rhs );
	SNML_INLINE void operator -=( scalar_arg rhs );
	SNML_INLINE void operator *=( scalar_arg rhs );
	SNML_INLINE void operator /=( scalar_arg rhs );

	SNML_INLINE bool operator ==( scalar_arg rhs ) const;
	SNML_INLINE bool operator !=( scalar_arg rhs ) const;
	SNML_INLINE bool operator ==( vquad_arg rhs ) const;
	SNML_INLINE bool operator !=( vquad_arg rhs ) const;

	// constants
	SNML_INLINE static vquad zero();
	SNML_INLINE static vquad one();
	SNML_INLINE static vquad two();
	SNML_INLINE static vquad three();
	SNML_INLINE static vquad four();
	SNML_INLINE static vquad five();
	SNML_INLINE static vquad six();
	SNML_INLINE static vquad seven();
	SNML_INLINE static vquad eight();
	SNML_INLINE static vquad nine();
	SNML_INLINE static vquad ten();
	SNML_INLINE static vquad half();
	SNML_INLINE static vquad quarter();
	SNML_INLINE static vquad three_quarters();
	SNML_INLINE static vquad three_by_2();
	SNML_INLINE static vquad huge();
	SNML_INLINE static vquad sqrt2();
	SNML_INLINE static vquad sqrt_half();
	SNML_INLINE static vquad two_by_sqrtpi();
	SNML_INLINE static vquad two_by_pi();
	SNML_INLINE static vquad one_by_pi();
	SNML_INLINE static vquad pi_by_4();
	SNML_INLINE static vquad pi_by_2();
	SNML_INLINE static vquad pi();
	SNML_INLINE static vquad e();
	SNML_INLINE static vquad log2e();
	SNML_INLINE static vquad log10e();
	SNML_INLINE static vquad ln2();
	SNML_INLINE static vquad ln10();
	SNML_INLINE static vquad two_pi();
	SNML_INLINE static vquad pi_by_6();
	SNML_INLINE static vquad log10two();
	SNML_INLINE static vquad log2ten();
	SNML_INLINE static vquad sqrt3_by_2();

	// std functions
	SNML_INLINE friend vquad sin( vquad_arg x );
	SNML_INLINE friend vquad cos( vquad_arg x );
	SNML_INLINE friend vquad tan( vquad_arg x );
	SNML_INLINE friend vquad asin( vquad_arg x );
	SNML_INLINE friend vquad acos( vquad_arg x );
	SNML_INLINE friend vquad exp( vquad_arg x );
	SNML_INLINE friend vquad log( vquad_arg x );
	SNML_INLINE friend vquad pow( vquad_arg x, vquad_arg power );
	SNML_INLINE friend vquad atan2( vquad_arg y, vquad_arg x );
	SNML_INLINE friend vquad min( vquad_arg x, vquad_arg y );
	SNML_INLINE friend vquad max( vquad_arg x, vquad_arg y );
	SNML_INLINE friend vquad sgn( vquad_arg x );
	SNML_INLINE friend vquad abs( vquad_arg x );
	SNML_INLINE friend vquad sqrt( vquad_arg x );
	SNML_INLINE friend vquad rsqrt( vquad_arg x );
	SNML_INLINE friend vquad recip( vquad_arg x );

	// x op y ? t : f
	SNML_INLINE friend vquad select_ge( vquad_arg x, vquad_arg y, vquad_arg t, vquad_arg f );
	SNML_INLINE friend vquad select_gt( vquad_arg x, vquad_arg y, vquad_arg t, vquad_arg f );
	SNML_INLINE friend vquad select_le( vquad_arg x, vquad_arg y, vquad_arg t, vquad_arg f );
	SNML_INLINE friend vquad select_lt( vquad_arg x, vquad_arg y, vquad_arg t, vquad_arg f );
	SNML_INLINE friend vquad select_eq( vquad_arg x, vquad_arg y, vquad_arg t, vquad_arg f );
	SNML_INLINE friend vquad select_ne( vquad_arg x, vquad_arg y, vquad_arg t, vquad_arg f );

	SNML_INLINE friend vquad select( bool sel, vquad_arg t, vquad_arg f );

	SNML_INLINE friend bool any_ge( vquad_arg x, vquad_arg y );
	SNML_INLINE friend bool any_gt( vquad_arg x, vquad_arg y );
	SNML_INLINE friend bool any_le( vquad_arg x, vquad_arg y );
	SNML_INLINE friend bool any_lt( vquad_arg x, vquad_arg y );
	SNML_INLINE friend bool any_eq( vquad_arg x, vquad_arg y );
	SNML_INLINE friend bool any_ne( vquad_arg x, vquad_arg y );

	SNML_INLINE friend bool all_ge( vquad_arg x, vquad_arg y );
	SNML_INLINE friend bool all_gt( vquad_arg x, vquad_arg y );
	SNML_INLINE friend bool all_le( vquad_arg x, vquad_arg y );
	SNML_INLINE friend bool all_lt( vquad_arg x, vquad_arg y );
	SNML_INLINE friend bool all_eq( vquad_arg x, vquad_arg y );
	SNML_INLINE friend bool all_ne( vquad_arg x, vquad_arg y );

	// vquad special ops	
	SNML_INLINE static vquad unit();

	SNML_INLINE friend vquad conj( vquad_arg rhs );
	SNML_INLINE friend vquad zerow( vquad_arg rhs );
	SNML_INLINE friend vquad onew( vquad_arg rhs );

	SNML_INLINE friend vquad cross( vquad_arg lhs, vquad_arg rhs );
	SNML_INLINE friend vquad axis_angle( vquad_arg axis, scalar_arg angle );
	SNML_INLINE friend vquad quat_mul( vquad_arg lhs, vquad_arg rhs );
	SNML_INLINE friend vquad quat_rotate( vquad_arg quat, vquad_arg rhs );
	SNML_INLINE friend vquad slerp( vquad_arg a, vquad_arg b, scalar_arg alpha );
	SNML_INLINE friend vquad squad( vquad_arg a, vquad_arg b, vquad_arg c, vquad_arg d, scalar_arg alpha );

	SNML_INLINE friend scalar angle( vquad_arg lhs, vquad_arg rhs );

	SNML_INLINE friend scalar dot2( vquad_arg lhs, vquad_arg rhs );
	SNML_INLINE friend scalar dot3( vquad_arg lhs, vquad_arg rhs );
	SNML_INLINE friend scalar dot4( vquad_arg lhs, vquad_arg rhs );
	
	// sideways ops
	SNML_INLINE friend scalar sum2( vquad_arg rhs );
	SNML_INLINE friend scalar sum3( vquad_arg rhs );
	SNML_INLINE friend scalar sum4( vquad_arg rhs );

	SNML_INLINE friend scalar sum_sideways( vquad_arg rhs );
	SNML_INLINE friend scalar max_sideways( vquad_arg rhs );
	SNML_INLINE friend scalar min_sideways( vquad_arg rhs );
	
	SNML_INLINE friend vquad normal2( vquad_arg rhs );
	SNML_INLINE friend vquad normal3( vquad_arg rhs );
	SNML_INLINE friend vquad normal4( vquad_arg rhs );
	
	SNML_INLINE friend scalar length2( vquad_arg rhs );
	SNML_INLINE friend scalar length3( vquad_arg rhs );
	SNML_INLINE friend scalar length4( vquad_arg rhs );
	
	SNML_INLINE friend scalar rlength2( vquad_arg rhs );
	SNML_INLINE friend scalar rlength3( vquad_arg rhs );
	SNML_INLINE friend scalar rlength4( vquad_arg rhs );
	
	SNML_INLINE friend scalar length_squared2( vquad_arg rhs );
	SNML_INLINE friend scalar length_squared3( vquad_arg rhs );
	SNML_INLINE friend scalar length_squared4( vquad_arg rhs );

	SNML_INLINE friend quadquad &OuterProduct4x4( quadquad &result, vquad_arg lhs, vquad_arg rhs );
};

// vquad vquad type (16 ieee floats)
// Use this as a base for matrix types as well as an efficient soa element
// This is a better soa element as every operation uses three latency slots.
// using a bigger element will start to trash the icache.
class quadquad
{
public:
	NativeQuad m_X;
	NativeQuad m_Y;
	NativeQuad m_Z;
	NativeQuad m_W;
	friend class scalar;
	friend class vquad;
public:

	// constructor
	SNML_INLINE quadquad();
	SNML_INLINE quadquad( float value_f );
	SNML_INLINE quadquad( scalar_arg value_s );
	SNML_INLINE quadquad( float xx, float xy, float xz, float xw, float yx, float yy, float yz, float yw, float zx, float zy, float zz, float zw, float wx, float wy, float wz, float ww );
	SNML_INLINE quadquad( vquad_arg x, vquad_arg y, vquad_arg z, vquad_arg w );
	SNML_INLINE quadquad( const float src[] );
	SNML_INLINE quadquad( const scalar src[] );
	SNML_INLINE quadquad( NativeQuad x, NativeQuad y, NativeQuad z, NativeQuad w );

	// import
	SNML_INLINE void load( float xx, float xy, float xz, float xw, float yx, float yy, float yz, float yw, float zx, float zy, float zz, float zw, float wx, float wy, float wz, float ww );
	SNML_INLINE void load( vquad_arg x_s, vquad_arg y_s, vquad_arg z_s, vquad_arg w_s );
	SNML_INLINE void load( const float src[] );
	SNML_INLINE void load( const scalar src[] );
	SNML_INLINE void load( NativeQuad x, NativeQuad y, NativeQuad z, NativeQuad w );
	
	// export
	SNML_INLINE operator float() const;
	SNML_INLINE vquad x() const;
	SNML_INLINE vquad y() const;
	SNML_INLINE vquad z() const;
	SNML_INLINE vquad w() const;
	SNML_INLINE vquad operator[]( unsigned index ) const; // only use this with constant indices or for debugging.
	SNML_INLINE const NativeQuad &access_x() const;
	SNML_INLINE const NativeQuad &access_y() const;
	SNML_INLINE const NativeQuad &access_z() const;
	SNML_INLINE const NativeQuad &access_w() const;
	SNML_NOINLINE const char *ToString( const char *pFmt = "%g" ) const;

	// arith ops
	SNML_INLINE quadquad operator +( quadquad_arg rhs ) const;
	SNML_INLINE quadquad operator -( quadquad_arg rhs ) const;
	SNML_INLINE quadquad operator *( quadquad_arg rhs ) const;
	SNML_INLINE quadquad operator /( quadquad_arg rhs ) const;
	SNML_INLINE quadquad operator +( scalar_arg rhs ) const;
	SNML_INLINE quadquad operator -( scalar_arg rhs ) const;
	SNML_INLINE quadquad operator *( scalar_arg rhs ) const;
	SNML_INLINE quadquad operator /( scalar_arg rhs ) const;
	SNML_INLINE void operator +=( quadquad_arg rhs );
	SNML_INLINE void operator -=( quadquad_arg rhs );
	SNML_INLINE void operator *=( quadquad_arg rhs );
	SNML_INLINE void operator /=( quadquad_arg rhs );
	SNML_INLINE void operator +=( scalar_arg rhs );
	SNML_INLINE void operator -=( scalar_arg rhs );
	SNML_INLINE void operator *=( scalar_arg rhs );
	SNML_INLINE void operator /=( scalar_arg rhs );
	SNML_INLINE quadquad operator -() const;
	SNML_INLINE quadquad operator +() const;
	
	SNML_INLINE bool operator ==( scalar_arg rhs ) const;
	SNML_INLINE bool operator !=( scalar_arg rhs ) const;
	SNML_INLINE bool operator ==( quadquad_arg rhs ) const;
	SNML_INLINE bool operator !=( quadquad_arg rhs ) const;

	// constants
	SNML_INLINE static quadquad zero();
	SNML_INLINE static quadquad one();
	SNML_INLINE static quadquad two();
	SNML_INLINE static quadquad three();
	SNML_INLINE static quadquad four();
	SNML_INLINE static quadquad five();
	SNML_INLINE static quadquad six();
	SNML_INLINE static quadquad seven();
	SNML_INLINE static quadquad eight();
	SNML_INLINE static quadquad nine();
	SNML_INLINE static quadquad ten();
	SNML_INLINE static quadquad half();
	SNML_INLINE static quadquad quarter();
	SNML_INLINE static quadquad three_quarters();
	SNML_INLINE static quadquad three_by_2();
	SNML_INLINE static quadquad huge();
	SNML_INLINE static quadquad sqrt2();
	SNML_INLINE static quadquad sqrt_half();
	SNML_INLINE static quadquad two_by_sqrtpi();
	SNML_INLINE static quadquad two_by_pi();
	SNML_INLINE static quadquad one_by_pi();
	SNML_INLINE static quadquad pi_by_4();
	SNML_INLINE static quadquad pi_by_2();
	SNML_INLINE static quadquad pi();
	SNML_INLINE static quadquad e();
	SNML_INLINE static quadquad log2e();
	SNML_INLINE static quadquad log10e();
	SNML_INLINE static quadquad ln2();
	SNML_INLINE static quadquad ln10();
	SNML_INLINE static quadquad two_pi();
	SNML_INLINE static quadquad pi_by_6();
	SNML_INLINE static quadquad log10two();
	SNML_INLINE static quadquad log2ten();
	SNML_INLINE static quadquad sqrt3_by_2();

	// special quadquad
	SNML_INLINE static quadquad unit4x4();

	// sideways ops
	SNML_INLINE friend scalar sum_sideways( quadquad_arg rhs );
	SNML_INLINE friend scalar max_sideways( quadquad_arg rhs );
	SNML_INLINE friend scalar min_sideways( quadquad_arg rhs );
	
	// scalar soa operators, all 16 elements are operated on.
	SNML_INLINE friend quadquad sin( quadquad_arg x );
	SNML_INLINE friend quadquad cos( quadquad_arg x );
	SNML_INLINE friend quadquad tan( quadquad_arg x );
	SNML_INLINE friend quadquad asin( quadquad_arg x );
	SNML_INLINE friend quadquad acos( quadquad_arg x );
	SNML_INLINE friend quadquad exp( quadquad_arg x );
	SNML_INLINE friend quadquad log( quadquad_arg x );
	SNML_INLINE friend quadquad pow( quadquad_arg x, quadquad_arg power );
	SNML_INLINE friend quadquad atan2( quadquad_arg y, quadquad_arg x );
	SNML_INLINE friend quadquad min( quadquad_arg x, quadquad_arg y );
	SNML_INLINE friend quadquad max( quadquad_arg x, quadquad_arg y );
	SNML_INLINE friend quadquad sgn( quadquad_arg x );
	SNML_INLINE friend quadquad abs( quadquad_arg x );
	SNML_INLINE friend quadquad sqrt( quadquad_arg x );
	SNML_INLINE friend quadquad rsqrt( quadquad_arg x );
	SNML_INLINE friend quadquad recip( quadquad_arg x );

	// x op y ? t : f
	SNML_INLINE friend quadquad select_ge( quadquad_arg x, quadquad_arg y, quadquad_arg t, quadquad_arg f );
	SNML_INLINE friend quadquad select_gt( quadquad_arg x, quadquad_arg y, quadquad_arg t, quadquad_arg f );
	SNML_INLINE friend quadquad select_le( quadquad_arg x, quadquad_arg y, quadquad_arg t, quadquad_arg f );
	SNML_INLINE friend quadquad select_lt( quadquad_arg x, quadquad_arg y, quadquad_arg t, quadquad_arg f );
	SNML_INLINE friend quadquad select_eq( quadquad_arg x, quadquad_arg y, quadquad_arg t, quadquad_arg f );
	SNML_INLINE friend quadquad select_ne( quadquad_arg x, quadquad_arg y, quadquad_arg t, quadquad_arg f );

	SNML_INLINE friend quadquad select( bool sel, quadquad_arg t, quadquad_arg f );

	SNML_INLINE friend bool any_ge( quadquad_arg x, quadquad_arg y );
	SNML_INLINE friend bool any_gt( quadquad_arg x, quadquad_arg y );
	SNML_INLINE friend bool any_le( quadquad_arg x, quadquad_arg y );
	SNML_INLINE friend bool any_lt( quadquad_arg x, quadquad_arg y );
	SNML_INLINE friend bool any_eq( quadquad_arg x, quadquad_arg y );
	SNML_INLINE friend bool any_ne( quadquad_arg x, quadquad_arg y );

	SNML_INLINE friend bool all_ge( quadquad_arg x, quadquad_arg y );
	SNML_INLINE friend bool all_gt( quadquad_arg x, quadquad_arg y );
	SNML_INLINE friend bool all_le( quadquad_arg x, quadquad_arg y );
	SNML_INLINE friend bool all_lt( quadquad_arg x, quadquad_arg y );
	SNML_INLINE friend bool all_eq( quadquad_arg x, quadquad_arg y );
	SNML_INLINE friend bool all_ne( quadquad_arg x, quadquad_arg y );

	// gl-style matrix ops (member functions)
	SNML_INLINE void Rotate( scalar_arg angle, vquad_arg xyz );
	SNML_INLINE void Translate( vquad_arg xyz );
	SNML_INLINE void Scale( vquad_arg xyz );
	SNML_INLINE void MultMatrix( quadquad_arg rhs );
	SNML_INLINE void MultQuat( vquad_arg rhs );
	SNML_INLINE void Perspective( scalar_arg fieldOfView, scalar_arg aspectRatio, scalar_arg _near, scalar_arg _far );
	SNML_INLINE void Ortho( scalar_arg left, scalar_arg right, scalar_arg bottom, scalar_arg top, scalar_arg _near, scalar_arg _far );
	SNML_INLINE void LookAt( vquad_arg eyePos, vquad_arg center, vquad_arg up );

	// matrix inverse/determinant
	SNML_INLINE friend quadquad &Inverse4x4( quadquad &result, quadquad &rhs );
	SNML_INLINE friend quadquad &Inverse3x4( quadquad &result, quadquad &rhs );
	SNML_INLINE friend scalar Determinant4x4( quadquad &rhs );
	SNML_INLINE friend scalar Determinant3x4( quadquad &rhs );
	SNML_INLINE friend quadquad &Transpose4x4( quadquad &result, quadquad &rhs );
	SNML_INLINE friend quadquad &OuterProduct4x4( quadquad &result, vquad_arg lhs, vquad_arg rhs );
	
	// symetric eigenvalues and eigenvectors (destroys rhs) call multiple times to refine
	// use for s3tc texture generation, AI discrimination, rotation extraction, intertia tensor simplifcation
	// speech recognition etc.
	SNML_INLINE friend vquad Eigenvalues3x3( quadquad &eigenvectors, quadquad &rhs );
};

// sideways norms
SNML_INLINE scalar L1_norm( vquad_arg rhs )
{
	return sum_sideways( abs( rhs ) );
}

SNML_INLINE scalar L2_norm( vquad_arg rhs )
{
	return sum_sideways( rhs * rhs );
}

SNML_INLINE scalar Linf_norm( vquad_arg rhs )
{
	return max_sideways( abs( rhs ) );
}

SNML_INLINE scalar L1_norm( quadquad_arg rhs )
{
	return sum_sideways( abs( rhs ) );
}

SNML_INLINE scalar L2_norm( quadquad_arg rhs )
{
	return sum_sideways( rhs * rhs );
}

SNML_INLINE scalar Linf_norm( quadquad_arg rhs )
{
	return max_sideways( abs( rhs ) );
}
