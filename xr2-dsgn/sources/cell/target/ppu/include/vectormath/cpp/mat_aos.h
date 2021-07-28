/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2008 Sony Computer Entertainment Inc.
*/

#ifndef _VECTORMATH_MAT_AOS_CPP_H
#define _VECTORMATH_MAT_AOS_CPP_H

namespace Vectormath {
namespace Aos {

//-----------------------------------------------------------------------------
// Constants
// for shuffles, words are labeled [x,y,z,w] [a,b,c,d]

#define _VECTORMATH_PERM_ZBWX ((vec_uchar16)(vec_uint4){ _VECTORMATH_PERM_Z, _VECTORMATH_PERM_B, _VECTORMATH_PERM_W, _VECTORMATH_PERM_X })
#define _VECTORMATH_PERM_XCYX ((vec_uchar16)(vec_uint4){ _VECTORMATH_PERM_X, _VECTORMATH_PERM_C, _VECTORMATH_PERM_Y, _VECTORMATH_PERM_X })
#define _VECTORMATH_PERM_XYAB ((vec_uchar16)(vec_uint4){ _VECTORMATH_PERM_X, _VECTORMATH_PERM_Y, _VECTORMATH_PERM_A, _VECTORMATH_PERM_B })
#define _VECTORMATH_PERM_ZWCD ((vec_uchar16)(vec_uint4){ _VECTORMATH_PERM_Z, _VECTORMATH_PERM_W, _VECTORMATH_PERM_C, _VECTORMATH_PERM_D })
#define _VECTORMATH_PERM_XZBX ((vec_uchar16)(vec_uint4){ _VECTORMATH_PERM_X, _VECTORMATH_PERM_Z, _VECTORMATH_PERM_B, _VECTORMATH_PERM_X })
#define _VECTORMATH_PERM_CXXX ((vec_uchar16)(vec_uint4){ _VECTORMATH_PERM_C, _VECTORMATH_PERM_X, _VECTORMATH_PERM_X, _VECTORMATH_PERM_X })
#define _VECTORMATH_PERM_YAXX ((vec_uchar16)(vec_uint4){ _VECTORMATH_PERM_Y, _VECTORMATH_PERM_A, _VECTORMATH_PERM_X, _VECTORMATH_PERM_X })
#define _VECTORMATH_PERM_XAZC ((vec_uchar16)(vec_uint4){ _VECTORMATH_PERM_X, _VECTORMATH_PERM_A, _VECTORMATH_PERM_Z, _VECTORMATH_PERM_C })
#define _VECTORMATH_PERM_YXWZ ((vec_uchar16)(vec_uint4){ _VECTORMATH_PERM_Y, _VECTORMATH_PERM_X, _VECTORMATH_PERM_W, _VECTORMATH_PERM_Z })
#define _VECTORMATH_PERM_YBWD ((vec_uchar16)(vec_uint4){ _VECTORMATH_PERM_Y, _VECTORMATH_PERM_B, _VECTORMATH_PERM_W, _VECTORMATH_PERM_D })
#define _VECTORMATH_PERM_XYCX ((vec_uchar16)(vec_uint4){ _VECTORMATH_PERM_X, _VECTORMATH_PERM_Y, _VECTORMATH_PERM_C, _VECTORMATH_PERM_X })
#define _VECTORMATH_PERM_YCXY ((vec_uchar16)(vec_uint4){ _VECTORMATH_PERM_Y, _VECTORMATH_PERM_C, _VECTORMATH_PERM_X, _VECTORMATH_PERM_Y })
#define _VECTORMATH_PERM_CXYC ((vec_uchar16)(vec_uint4){ _VECTORMATH_PERM_C, _VECTORMATH_PERM_X, _VECTORMATH_PERM_Y, _VECTORMATH_PERM_C })
#define _VECTORMATH_PERM_ZAYX ((vec_uchar16)(vec_uint4){ _VECTORMATH_PERM_Z, _VECTORMATH_PERM_A, _VECTORMATH_PERM_Y, _VECTORMATH_PERM_X })
#define _VECTORMATH_PERM_BZXX ((vec_uchar16)(vec_uint4){ _VECTORMATH_PERM_B, _VECTORMATH_PERM_Z, _VECTORMATH_PERM_X, _VECTORMATH_PERM_X })
#define _VECTORMATH_PERM_XZYA ((vec_uchar16)(vec_uint4){ _VECTORMATH_PERM_X, _VECTORMATH_PERM_Z, _VECTORMATH_PERM_Y, _VECTORMATH_PERM_A })
#define _VECTORMATH_PERM_ZXXB ((vec_uchar16)(vec_uint4){ _VECTORMATH_PERM_Z, _VECTORMATH_PERM_X, _VECTORMATH_PERM_X, _VECTORMATH_PERM_B })
#define _VECTORMATH_PERM_YXXC ((vec_uchar16)(vec_uint4){ _VECTORMATH_PERM_Y, _VECTORMATH_PERM_X, _VECTORMATH_PERM_X, _VECTORMATH_PERM_C })
#define _VECTORMATH_PERM_BBYX ((vec_uchar16)(vec_uint4){ _VECTORMATH_PERM_B, _VECTORMATH_PERM_B, _VECTORMATH_PERM_Y, _VECTORMATH_PERM_X })
#define _VECTORMATH_PERM_WDDW ((vec_uchar16)(vec_uint4){ _VECTORMATH_PERM_W, _VECTORMATH_PERM_D, _VECTORMATH_PERM_D, _VECTORMATH_PERM_W })
#define _VECTORMATH_PERM_YXBA ((vec_uchar16)(vec_uint4){ _VECTORMATH_PERM_Y, _VECTORMATH_PERM_X, _VECTORMATH_PERM_B, _VECTORMATH_PERM_A })
#define _VECTORMATH_PERM_WZDC ((vec_uchar16)(vec_uint4){ _VECTORMATH_PERM_W, _VECTORMATH_PERM_Z, _VECTORMATH_PERM_D, _VECTORMATH_PERM_C })
#define _VECTORMATH_PERM_XAAX ((vec_uchar16)(vec_uint4){ _VECTORMATH_PERM_X, _VECTORMATH_PERM_A, _VECTORMATH_PERM_A, _VECTORMATH_PERM_X })
#define _VECTORMATH_PERM_YBBY ((vec_uchar16)(vec_uint4){ _VECTORMATH_PERM_Y, _VECTORMATH_PERM_B, _VECTORMATH_PERM_B, _VECTORMATH_PERM_Y })
#define _VECTORMATH_PERM_WCDZ ((vec_uchar16)(vec_uint4){ _VECTORMATH_PERM_W, _VECTORMATH_PERM_C, _VECTORMATH_PERM_D, _VECTORMATH_PERM_Z })
#define _VECTORMATH_PERM_YABX ((vec_uchar16)(vec_uint4){ _VECTORMATH_PERM_Y, _VECTORMATH_PERM_A, _VECTORMATH_PERM_B, _VECTORMATH_PERM_X })
#define _VECTORMATH_PERM_XYAX ((vec_uchar16)(vec_uint4){ _VECTORMATH_PERM_X, _VECTORMATH_PERM_Y, _VECTORMATH_PERM_A, _VECTORMATH_PERM_X })
#define _VECTORMATH_PERM_ZWBX ((vec_uchar16)(vec_uint4){ _VECTORMATH_PERM_Z, _VECTORMATH_PERM_W, _VECTORMATH_PERM_B, _VECTORMATH_PERM_X })
#define _VECTORMATH_PERM_XAYD ((vec_uchar16)(vec_uint4){ _VECTORMATH_PERM_X, _VECTORMATH_PERM_A, _VECTORMATH_PERM_Y, _VECTORMATH_PERM_D })
#define _VECTORMATH_PERM_XCYD ((vec_uchar16)(vec_uint4){ _VECTORMATH_PERM_X, _VECTORMATH_PERM_C, _VECTORMATH_PERM_Y, _VECTORMATH_PERM_D })
#define _VECTORMATH_PERM_ZBWD ((vec_uchar16)(vec_uint4){ _VECTORMATH_PERM_Z, _VECTORMATH_PERM_B, _VECTORMATH_PERM_W, _VECTORMATH_PERM_D })
#define _VECTORMATH_PI_OVER_2 1.570796327f

//-----------------------------------------------------------------------------
// Definitions

inline Matrix3::Matrix3( const Matrix3 & mat )
{
    mCol0 = mat.mCol0;
    mCol1 = mat.mCol1;
    mCol2 = mat.mCol2;
}

inline Matrix3::Matrix3( float scalar )
{
    mCol0 = Vector3( scalar );
    mCol1 = Vector3( scalar );
    mCol2 = Vector3( scalar );
}

inline Matrix3::Matrix3( floatInVec scalar )
{
    mCol0 = Vector3( scalar );
    mCol1 = Vector3( scalar );
    mCol2 = Vector3( scalar );
}

inline Matrix3::Matrix3( Quat unitQuat )
{
    vec_float4 xyzw_2, wwww, yzxw, zxyw, yzxw_2, zxyw_2;
    vec_float4 tmp0, tmp1, tmp2, tmp3, tmp4, tmp5;
    vec_uint4 select_x = _VECTORMATH_MASK_0xF000;
    vec_uint4 select_z = _VECTORMATH_MASK_0x00F0;
    xyzw_2 = vec_add( unitQuat.get128(), unitQuat.get128() );
    wwww = vec_splat( unitQuat.get128(), 3 );
    yzxw = vec_perm( unitQuat.get128(), unitQuat.get128(), _VECTORMATH_PERM_YZXW );
    zxyw = vec_perm( unitQuat.get128(), unitQuat.get128(), _VECTORMATH_PERM_ZXYW );
    yzxw_2 = vec_perm( xyzw_2, xyzw_2, _VECTORMATH_PERM_YZXW );
    zxyw_2 = vec_perm( xyzw_2, xyzw_2, _VECTORMATH_PERM_ZXYW );
    tmp0 = vec_madd( yzxw_2, wwww, (vec_float4)(0.0f) );
    tmp1 = vec_nmsub( yzxw, yzxw_2, (vec_float4)(1.0f) );
    tmp2 = vec_madd( yzxw, xyzw_2, (vec_float4)(0.0f) );
    tmp0 = vec_madd( zxyw, xyzw_2, tmp0 );
    tmp1 = vec_nmsub( zxyw, zxyw_2, tmp1 );
    tmp2 = vec_nmsub( zxyw_2, wwww, tmp2 );
    tmp3 = vec_sel( tmp0, tmp1, select_x );
    tmp4 = vec_sel( tmp1, tmp2, select_x );
    tmp5 = vec_sel( tmp2, tmp0, select_x );
    mCol0 = Vector3( vec_sel( tmp3, tmp2, select_z ) );
    mCol1 = Vector3( vec_sel( tmp4, tmp0, select_z ) );
    mCol2 = Vector3( vec_sel( tmp5, tmp1, select_z ) );
}

inline Matrix3::Matrix3( Vector3 _col0, Vector3 _col1, Vector3 _col2 )
{
    mCol0 = _col0;
    mCol1 = _col1;
    mCol2 = _col2;
}

inline Matrix3 & Matrix3::setCol0( Vector3 _col0 )
{
    mCol0 = _col0;
    return *this;
}

inline Matrix3 & Matrix3::setCol1( Vector3 _col1 )
{
    mCol1 = _col1;
    return *this;
}

inline Matrix3 & Matrix3::setCol2( Vector3 _col2 )
{
    mCol2 = _col2;
    return *this;
}

inline Matrix3 & Matrix3::setCol( int col, Vector3 vec )
{
    *(&mCol0 + col) = vec;
    return *this;
}

inline Matrix3 & Matrix3::setRow( int row, Vector3 vec )
{
    mCol0.setElem( row, vec.getElem( 0 ) );
    mCol1.setElem( row, vec.getElem( 1 ) );
    mCol2.setElem( row, vec.getElem( 2 ) );
    return *this;
}

inline Matrix3 & Matrix3::setElem( int col, int row, float val )
{
    (*this)[col].setElem(row, val);
    return *this;
}

inline Matrix3 & Matrix3::setElem( int col, int row, floatInVec val )
{
    Vector3 tmpV3_0;
    tmpV3_0 = this->getCol( col );
    tmpV3_0.setElem( row, val );
    this->setCol( col, tmpV3_0 );
    return *this;
}

inline const floatInVec Matrix3::getElem( int col, int row ) const
{
    return this->getCol( col ).getElem( row );
}

inline const Vector3 Matrix3::getCol0( ) const
{
    return mCol0;
}

inline const Vector3 Matrix3::getCol1( ) const
{
    return mCol1;
}

inline const Vector3 Matrix3::getCol2( ) const
{
    return mCol2;
}

inline const Vector3 Matrix3::getCol( int col ) const
{
    return *(&mCol0 + col);
}

inline const Vector3 Matrix3::getRow( int row ) const
{
    return Vector3( mCol0.getElem( row ), mCol1.getElem( row ), mCol2.getElem( row ) );
}

inline Vector3 & Matrix3::operator []( int col )
{
    return *(&mCol0 + col);
}

inline const Vector3 Matrix3::operator []( int col ) const
{
    return *(&mCol0 + col);
}

inline Matrix3 & Matrix3::operator =( const Matrix3 & mat )
{
    mCol0 = mat.mCol0;
    mCol1 = mat.mCol1;
    mCol2 = mat.mCol2;
    return *this;
}

inline const Matrix3 transpose( const Matrix3 & mat )
{
    vec_float4 tmp0, tmp1, res0, res1, res2;
    tmp0 = vec_mergeh( mat.getCol0().get128(), mat.getCol2().get128() );
    tmp1 = vec_mergel( mat.getCol0().get128(), mat.getCol2().get128() );
    res0 = vec_mergeh( tmp0, mat.getCol1().get128() );
    res1 = vec_perm( tmp0, mat.getCol1().get128(), _VECTORMATH_PERM_ZBWX );
    res2 = vec_perm( tmp1, mat.getCol1().get128(), _VECTORMATH_PERM_XCYX );
    return Matrix3(
        Vector3( res0 ),
        Vector3( res1 ),
        Vector3( res2 )
    );
}

inline const Matrix3 inverse( const Matrix3 & mat )
{
    vec_float4 tmp0, tmp1, tmp2, tmp3, tmp4, dot, invdet, inv0, inv1, inv2;
    vec_float4 zero = (vec_float4)(0.0f);
    tmp2 = _vmathVfCross( mat.getCol0().get128(), mat.getCol1().get128() );
    tmp0 = _vmathVfCross( mat.getCol1().get128(), mat.getCol2().get128() );
    tmp1 = _vmathVfCross( mat.getCol2().get128(), mat.getCol0().get128() );
    dot = _vmathVfDot3( tmp2, mat.getCol2().get128() );
    dot = vec_splat( dot, 0 );
    vec_float4 y0 = vec_re(dot);
    invdet = vec_madd( vec_nmsub( dot, y0, (vec_float4)(1.0f) ), y0, y0 );
    tmp3 = vec_mergeh( tmp0, tmp2 );
    tmp4 = vec_mergel( tmp0, tmp2 );
    inv0 = vec_mergeh( tmp3, tmp1 );
    inv1 = vec_perm( tmp3, tmp1, _VECTORMATH_PERM_ZBWX );
    inv2 = vec_perm( tmp4, tmp1, _VECTORMATH_PERM_XCYX );
    inv0 = vec_madd( inv0, invdet, zero );
    inv1 = vec_madd( inv1, invdet, zero );
    inv2 = vec_madd( inv2, invdet, zero );
    return Matrix3(
        Vector3( inv0 ),
        Vector3( inv1 ),
        Vector3( inv2 )
    );
}

inline const floatInVec determinant( const Matrix3 & mat )
{
    return dot( mat.getCol2(), cross( mat.getCol0(), mat.getCol1() ) );
}

inline const Matrix3 Matrix3::operator +( const Matrix3 & mat ) const
{
    return Matrix3(
        ( mCol0 + mat.mCol0 ),
        ( mCol1 + mat.mCol1 ),
        ( mCol2 + mat.mCol2 )
    );
}

inline const Matrix3 Matrix3::operator -( const Matrix3 & mat ) const
{
    return Matrix3(
        ( mCol0 - mat.mCol0 ),
        ( mCol1 - mat.mCol1 ),
        ( mCol2 - mat.mCol2 )
    );
}

inline Matrix3 & Matrix3::operator +=( const Matrix3 & mat )
{
    *this = *this + mat;
    return *this;
}

inline Matrix3 & Matrix3::operator -=( const Matrix3 & mat )
{
    *this = *this - mat;
    return *this;
}

inline const Matrix3 Matrix3::operator -( ) const
{
    return Matrix3(
        ( -mCol0 ),
        ( -mCol1 ),
        ( -mCol2 )
    );
}

inline const Matrix3 absPerElem( const Matrix3 & mat )
{
    return Matrix3(
        absPerElem( mat.getCol0() ),
        absPerElem( mat.getCol1() ),
        absPerElem( mat.getCol2() )
    );
}

inline const Matrix3 Matrix3::operator *( float scalar ) const
{
    return *this * floatInVec(scalar);
}

inline const Matrix3 Matrix3::operator *( floatInVec scalar ) const
{
    return Matrix3(
        ( mCol0 * scalar ),
        ( mCol1 * scalar ),
        ( mCol2 * scalar )
    );
}

inline Matrix3 & Matrix3::operator *=( float scalar )
{
    return *this *= floatInVec(scalar);
}

inline Matrix3 & Matrix3::operator *=( floatInVec scalar )
{
    *this = *this * scalar;
    return *this;
}

inline const Matrix3 operator *( float scalar, const Matrix3 & mat )
{
    return floatInVec(scalar) * mat;
}

inline const Matrix3 operator *( floatInVec scalar, const Matrix3 & mat )
{
    return mat * scalar;
}

inline const Vector3 Matrix3::operator *( Vector3 vec ) const
{
    vec_float4 res;
    vec_float4 xxxx, yyyy, zzzz;
    xxxx = vec_splat( vec.get128(), 0 );
    yyyy = vec_splat( vec.get128(), 1 );
    zzzz = vec_splat( vec.get128(), 2 );
    res = vec_madd( mCol0.get128(), xxxx, (vec_float4)(0.0f) );
    res = vec_madd( mCol1.get128(), yyyy, res );
    res = vec_madd( mCol2.get128(), zzzz, res );
    return Vector3( res );
}

inline const Matrix3 Matrix3::operator *( const Matrix3 & mat ) const
{
    return Matrix3(
        ( *this * mat.mCol0 ),
        ( *this * mat.mCol1 ),
        ( *this * mat.mCol2 )
    );
}

inline Matrix3 & Matrix3::operator *=( const Matrix3 & mat )
{
    *this = *this * mat;
    return *this;
}

inline const Matrix3 mulPerElem( const Matrix3 & mat0, const Matrix3 & mat1 )
{
    return Matrix3(
        mulPerElem( mat0.getCol0(), mat1.getCol0() ),
        mulPerElem( mat0.getCol1(), mat1.getCol1() ),
        mulPerElem( mat0.getCol2(), mat1.getCol2() )
    );
}

inline const Matrix3 Matrix3::identity( )
{
    return Matrix3(
        Vector3::xAxis( ),
        Vector3::yAxis( ),
        Vector3::zAxis( )
    );
}

inline const Matrix3 Matrix3::rotationX( float radians )
{
    return rotationX( floatInVec(radians) );
}

inline const Matrix3 Matrix3::rotationX( floatInVec radians )
{
    vec_float4 s, c, res1, res2;
    vec_uint4 select_y, select_z;
    vec_float4 zero;
    select_y = _VECTORMATH_MASK_0x0F00;
    select_z = _VECTORMATH_MASK_0x00F0;
    zero = (vec_float4)(0.0f);
    sincosf4( radians.get128(), &s, &c );
    res1 = vec_sel( zero, c, select_y );
    res1 = vec_sel( res1, s, select_z );
    res2 = vec_sel( zero, negatef4(s), select_y );
    res2 = vec_sel( res2, c, select_z );
    return Matrix3(
        Vector3::xAxis( ),
        Vector3( res1 ),
        Vector3( res2 )
    );
}

inline const Matrix3 Matrix3::rotationY( float radians )
{
    return rotationY( floatInVec(radians) );
}

inline const Matrix3 Matrix3::rotationY( floatInVec radians )
{
    vec_float4 s, c, res0, res2;
    vec_uint4 select_x, select_z;
    vec_float4 zero;
    select_x = _VECTORMATH_MASK_0xF000;
    select_z = _VECTORMATH_MASK_0x00F0;
    zero = (vec_float4)(0.0f);
    sincosf4( radians.get128(), &s, &c );
    res0 = vec_sel( zero, c, select_x );
    res0 = vec_sel( res0, negatef4(s), select_z );
    res2 = vec_sel( zero, s, select_x );
    res2 = vec_sel( res2, c, select_z );
    return Matrix3(
        Vector3( res0 ),
        Vector3::yAxis( ),
        Vector3( res2 )
    );
}

inline const Matrix3 Matrix3::rotationZ( float radians )
{
    return rotationZ( floatInVec(radians) );
}

inline const Matrix3 Matrix3::rotationZ( floatInVec radians )
{
    vec_float4 s, c, res0, res1;
    vec_uint4 select_x, select_y;
    vec_float4 zero;
    select_x = _VECTORMATH_MASK_0xF000;
    select_y = _VECTORMATH_MASK_0x0F00;
    zero = (vec_float4)(0.0f);
    sincosf4( radians.get128(), &s, &c );
    res0 = vec_sel( zero, c, select_x );
    res0 = vec_sel( res0, s, select_y );
    res1 = vec_sel( zero, negatef4(s), select_x );
    res1 = vec_sel( res1, c, select_y );
    return Matrix3(
        Vector3( res0 ),
        Vector3( res1 ),
        Vector3::zAxis( )
    );
}

inline const Matrix3 Matrix3::rotationZYX( Vector3 radiansXYZ )
{
    vec_float4 angles, s, negS, c, X0, X1, Y0, Y1, Z0, Z1, tmp;
    vec_float4 zero = (vec_float4)(0.0f);
    angles = Vector4( radiansXYZ, 0.0f ).get128();
    sincosf4( angles, &s, &c );
    negS = negatef4( s );
    Z0 = vec_mergel( c, s );
    Z1 = vec_mergel( negS, c );
    Z1 = vec_andc( Z1, (vec_float4)_VECTORMATH_MASK_0x000F );
    Y0 = vec_perm( negS, c, _VECTORMATH_PERM_BBYX );
    Y1 = vec_perm( c, s, _VECTORMATH_PERM_BBYX );
    X0 = vec_splat( s, 0 );
    X1 = vec_splat( c, 0 );
    tmp = vec_madd( Z0, Y1, zero );
    return Matrix3(
        Vector3( vec_madd( Z0, Y0, zero ) ),
        Vector3( vec_madd( Z1, X1, vec_madd( tmp, X0, zero ) ) ),
        Vector3( vec_nmsub( Z1, X0, vec_madd( tmp, X1, zero ) ) )
    );
}

inline const Matrix3 Matrix3::rotation( float radians, Vector3 unitVec )
{
    return rotation( floatInVec(radians), unitVec );
}

inline const Matrix3 Matrix3::rotation( floatInVec radians, Vector3 unitVec )
{
    vec_float4 axis, s, c, oneMinusC, axisS, negAxisS, xxxx, yyyy, zzzz, tmp0, tmp1, tmp2;
    vec_float4 zero = (vec_float4)(0.0f);
    axis = unitVec.get128();
    sincosf4( radians.get128(), &s, &c );
    xxxx = vec_splat( axis, 0 );
    yyyy = vec_splat( axis, 1 );
    zzzz = vec_splat( axis, 2 );
    oneMinusC = vec_sub( (vec_float4)(1.0f), c );
    axisS = vec_madd( axis, s, zero );
    negAxisS = negatef4( axisS );
    tmp0 = vec_perm( axisS, negAxisS, _VECTORMATH_PERM_XZBX );
    tmp1 = vec_perm( axisS, negAxisS, _VECTORMATH_PERM_CXXX );
    tmp2 = vec_perm( axisS, negAxisS, _VECTORMATH_PERM_YAXX );
    tmp0 = vec_sel( tmp0, c, _VECTORMATH_MASK_0xF000 );
    tmp1 = vec_sel( tmp1, c, _VECTORMATH_MASK_0x0F00 );
    tmp2 = vec_sel( tmp2, c, _VECTORMATH_MASK_0x00F0 );
    return Matrix3(
        Vector3( vec_madd( vec_madd( axis, xxxx, zero ), oneMinusC, tmp0 ) ),
        Vector3( vec_madd( vec_madd( axis, yyyy, zero ), oneMinusC, tmp1 ) ),
        Vector3( vec_madd( vec_madd( axis, zzzz, zero ), oneMinusC, tmp2 ) )
    );
}

inline const Matrix3 Matrix3::rotation( Quat unitQuat )
{
    return Matrix3( unitQuat );
}

inline const Matrix3 Matrix3::scale( Vector3 scaleVec )
{
    vec_float4 zero = (vec_float4)(0.0f);
    return Matrix3(
        Vector3( vec_sel( zero, scaleVec.get128(), _VECTORMATH_MASK_0xF000 ) ),
        Vector3( vec_sel( zero, scaleVec.get128(), _VECTORMATH_MASK_0x0F00 ) ),
        Vector3( vec_sel( zero, scaleVec.get128(), _VECTORMATH_MASK_0x00F0 ) )
    );
}

inline const Matrix3 appendScale( const Matrix3 & mat, Vector3 scaleVec )
{
    return Matrix3(
        ( mat.getCol0() * scaleVec.getX( ) ),
        ( mat.getCol1() * scaleVec.getY( ) ),
        ( mat.getCol2() * scaleVec.getZ( ) )
    );
}

inline const Matrix3 prependScale( Vector3 scaleVec, const Matrix3 & mat )
{
    return Matrix3(
        mulPerElem( mat.getCol0(), scaleVec ),
        mulPerElem( mat.getCol1(), scaleVec ),
        mulPerElem( mat.getCol2(), scaleVec )
    );
}

inline const Matrix3 select( const Matrix3 & mat0, const Matrix3 & mat1, bool select1 )
{
    boolInVec select1_( select1 );
    return Matrix3(
        select( mat0.getCol0(), mat1.getCol0(), select1_ ),
        select( mat0.getCol1(), mat1.getCol1(), select1_ ),
        select( mat0.getCol2(), mat1.getCol2(), select1_ )
    );
}

inline const Matrix3 select( const Matrix3 & mat0, const Matrix3 & mat1, boolInVec select1 )
{
    return Matrix3(
        select( mat0.getCol0(), mat1.getCol0(), select1 ),
        select( mat0.getCol1(), mat1.getCol1(), select1 ),
        select( mat0.getCol2(), mat1.getCol2(), select1 )
    );
}

#ifdef _VECTORMATH_DEBUG

inline void print( const Matrix3 & mat )
{
    print( mat.getRow( 0 ) );
    print( mat.getRow( 1 ) );
    print( mat.getRow( 2 ) );
}

inline void print( const Matrix3 & mat, const char * name )
{
    printf("%s:\n", name);
    print( mat );
}

#endif

inline Matrix4::Matrix4( const Matrix4 & mat )
{
    mCol0 = mat.mCol0;
    mCol1 = mat.mCol1;
    mCol2 = mat.mCol2;
    mCol3 = mat.mCol3;
}

inline Matrix4::Matrix4( float scalar )
{
    mCol0 = Vector4( scalar );
    mCol1 = Vector4( scalar );
    mCol2 = Vector4( scalar );
    mCol3 = Vector4( scalar );
}

inline Matrix4::Matrix4( floatInVec scalar )
{
    mCol0 = Vector4( scalar );
    mCol1 = Vector4( scalar );
    mCol2 = Vector4( scalar );
    mCol3 = Vector4( scalar );
}

inline Matrix4::Matrix4( const Transform3 & mat )
{
    mCol0 = Vector4( mat.getCol0(), 0.0f );
    mCol1 = Vector4( mat.getCol1(), 0.0f );
    mCol2 = Vector4( mat.getCol2(), 0.0f );
    mCol3 = Vector4( mat.getCol3(), 1.0f );
}

inline Matrix4::Matrix4( Vector4 _col0, Vector4 _col1, Vector4 _col2, Vector4 _col3 )
{
    mCol0 = _col0;
    mCol1 = _col1;
    mCol2 = _col2;
    mCol3 = _col3;
}

inline Matrix4::Matrix4( const Matrix3 & mat, Vector3 translateVec )
{
    mCol0 = Vector4( mat.getCol0(), 0.0f );
    mCol1 = Vector4( mat.getCol1(), 0.0f );
    mCol2 = Vector4( mat.getCol2(), 0.0f );
    mCol3 = Vector4( translateVec, 1.0f );
}

inline Matrix4::Matrix4( Quat unitQuat, Vector3 translateVec )
{
    Matrix3 mat;
    mat = Matrix3( unitQuat );
    mCol0 = Vector4( mat.getCol0(), 0.0f );
    mCol1 = Vector4( mat.getCol1(), 0.0f );
    mCol2 = Vector4( mat.getCol2(), 0.0f );
    mCol3 = Vector4( translateVec, 1.0f );
}

inline Matrix4 & Matrix4::setCol0( Vector4 _col0 )
{
    mCol0 = _col0;
    return *this;
}

inline Matrix4 & Matrix4::setCol1( Vector4 _col1 )
{
    mCol1 = _col1;
    return *this;
}

inline Matrix4 & Matrix4::setCol2( Vector4 _col2 )
{
    mCol2 = _col2;
    return *this;
}

inline Matrix4 & Matrix4::setCol3( Vector4 _col3 )
{
    mCol3 = _col3;
    return *this;
}

inline Matrix4 & Matrix4::setCol( int col, Vector4 vec )
{
    *(&mCol0 + col) = vec;
    return *this;
}

inline Matrix4 & Matrix4::setRow( int row, Vector4 vec )
{
    mCol0.setElem( row, vec.getElem( 0 ) );
    mCol1.setElem( row, vec.getElem( 1 ) );
    mCol2.setElem( row, vec.getElem( 2 ) );
    mCol3.setElem( row, vec.getElem( 3 ) );
    return *this;
}

inline Matrix4 & Matrix4::setElem( int col, int row, float val )
{
    (*this)[col].setElem(row, val);
    return *this;
}

inline Matrix4 & Matrix4::setElem( int col, int row, floatInVec val )
{
    Vector4 tmpV3_0;
    tmpV3_0 = this->getCol( col );
    tmpV3_0.setElem( row, val );
    this->setCol( col, tmpV3_0 );
    return *this;
}

inline const floatInVec Matrix4::getElem( int col, int row ) const
{
    return this->getCol( col ).getElem( row );
}

inline const Vector4 Matrix4::getCol0( ) const
{
    return mCol0;
}

inline const Vector4 Matrix4::getCol1( ) const
{
    return mCol1;
}

inline const Vector4 Matrix4::getCol2( ) const
{
    return mCol2;
}

inline const Vector4 Matrix4::getCol3( ) const
{
    return mCol3;
}

inline const Vector4 Matrix4::getCol( int col ) const
{
    return *(&mCol0 + col);
}

inline const Vector4 Matrix4::getRow( int row ) const
{
    return Vector4( mCol0.getElem( row ), mCol1.getElem( row ), mCol2.getElem( row ), mCol3.getElem( row ) );
}

inline Vector4 & Matrix4::operator []( int col )
{
    return *(&mCol0 + col);
}

inline const Vector4 Matrix4::operator []( int col ) const
{
    return *(&mCol0 + col);
}

inline Matrix4 & Matrix4::operator =( const Matrix4 & mat )
{
    mCol0 = mat.mCol0;
    mCol1 = mat.mCol1;
    mCol2 = mat.mCol2;
    mCol3 = mat.mCol3;
    return *this;
}

inline const Matrix4 transpose( const Matrix4 & mat )
{
    vec_float4 tmp0, tmp1, tmp2, tmp3, res0, res1, res2, res3;
    tmp0 = vec_mergeh( mat.getCol0().get128(), mat.getCol2().get128() );
    tmp1 = vec_mergeh( mat.getCol1().get128(), mat.getCol3().get128() );
    tmp2 = vec_mergel( mat.getCol0().get128(), mat.getCol2().get128() );
    tmp3 = vec_mergel( mat.getCol1().get128(), mat.getCol3().get128() );
    res0 = vec_mergeh( tmp0, tmp1 );
    res1 = vec_mergel( tmp0, tmp1 );
    res2 = vec_mergeh( tmp2, tmp3 );
    res3 = vec_mergel( tmp2, tmp3 );
    return Matrix4(
        Vector4( res0 ),
        Vector4( res1 ),
        Vector4( res2 ),
        Vector4( res3 )
    );
}

inline const Matrix4 inverse( const Matrix4 & mat )
{
    #ifndef _VECTORMATH_USE_SDK240_VERSION
    vector float vzero = (vector float)(0.0);
    vec_float4 in0 = mat.getCol0().get128(); // AEIM
    vec_float4 in1 = mat.getCol1().get128(); // BFJN
    vec_float4 in2 = mat.getCol2().get128(); // CGKO
    vec_float4 in3 = mat.getCol3().get128(); // DHLP
    
    vec_float4 inA0 = vec_mergeh(in0, in1); // ABEF
    vec_float4 inA1 = vec_mergeh(in2, in3); // CDGH
    vec_float4 inA2 = vec_mergel(in0, in1); // IJMN
    vec_float4 inA3 = vec_mergel(in2, in3); // KLOP
    
    // XYZW, ABCD
    vec_float4 KIIK = vec_perm(inA3, inA2, _VECTORMATH_PERM_XAAX); // KLOP, IJMN
    vec_float4 KKII = vec_sld(KIIK, KIIK, 12);
    vec_float4 NPPN = vec_perm(in1, in3, _VECTORMATH_PERM_WDDW); // BFJN,DHLP
    vec_float4 PPNN = vec_sld(NPPN, NPPN, 4);
    vec_float4 OMMO = vec_perm(in2, in0, _VECTORMATH_PERM_WDDW); // CGKO,AEIM
    vec_float4 OOMM = vec_sld(OMMO, OMMO, 12);
    vec_float4 JILK = vec_perm(inA2, inA3, _VECTORMATH_PERM_YXBA); // IJMN,KLOP
    vec_float4 LKJI = vec_sld(JILK, JILK, 8);
    vec_float4 PONM = vec_perm(inA3, inA2, _VECTORMATH_PERM_WZDC); // KLOP,IJMN
    vec_float4 NMPO = vec_sld(PONM, PONM, 8);
    vec_float4 JLLJ = vec_perm(inA2, inA3, _VECTORMATH_PERM_YBBY); // IJMN, KLOP
    vec_float4 LLJJ = vec_sld(JLLJ, JLLJ, 4);
    vec_float4 CAAC = vec_perm(in2, in0, _VECTORMATH_PERM_XAAX); // CGKO,AEIM
    vec_float4 CCAA = vec_sld(CAAC, CAAC, 12);
    vec_float4 GEEG = vec_perm(in2, in0, _VECTORMATH_PERM_YBBY); // CGKO,AEIM
    vec_float4 GGEE = vec_sld(GEEG, GEEG, 12);
    vec_float4 BADC = vec_perm(inA0, inA1, _VECTORMATH_PERM_YXBA); // ABEF,CDGH
    vec_float4 DCBA = vec_perm(inA1, inA0, _VECTORMATH_PERM_YXBA); // CDGH,ABEF
    vec_float4 HGFE = vec_perm(inA1, inA0, _VECTORMATH_PERM_WZDC); // CDGH,ABEF
    vec_float4 FEHG = vec_perm(inA0, inA1, _VECTORMATH_PERM_WZDC); // ABEF,CDGH
    vec_float4 FHHF = vec_perm(inA0, inA1, _VECTORMATH_PERM_WDDW); // ABEF, CDGH
    vec_float4 HHFF = vec_sld(FHHF, FHHF, 4);
    vec_float4 BDDB = vec_perm(in1, in3, _VECTORMATH_PERM_XAAX); // BFJN,DHLP
    vec_float4 DDBB = vec_sld(BDDB, BDDB, 4);
    
    vec_float4 KP__IP__IN__KN = vec_madd(KIIK, PPNN, vzero);
    vec_float4 JP__IO__LN__KM = vec_madd(JILK, PONM, vzero);
    vec_float4 KN__KP__IP__IN = vec_madd(KKII, NPPN, vzero);
    vec_float4 CH__AH__AF__CF = vec_madd(CAAC, HHFF, vzero);
    vec_float4 BH__AG__DF__CE = vec_madd(BADC, HGFE, vzero);
    vec_float4 CF__CH__AH__AF = vec_madd(CCAA, FHHF, vzero);
    
    vec_float4 FGHE = vec_perm(inA0, inA1, _VECTORMATH_PERM_WCDZ); // ABEF,CDGH
    vec_float4 GHEF = vec_sld(FGHE, FGHE, 4);
    vec_float4 HEFG = vec_perm(inA1, inA0, _VECTORMATH_PERM_WCDZ); // CDGH,ABEF
    vec_float4 BCDA = vec_perm(inA0, inA1, _VECTORMATH_PERM_YABX); // ABEF,CDGH
    vec_float4 CDAB = vec_sld(BCDA, BCDA, 4);
    vec_float4 DABC = vec_perm(inA1, inA0, _VECTORMATH_PERM_YABX); // CDGH,ABEF
    vec_float4 NOPM = vec_perm(inA2, inA3, _VECTORMATH_PERM_WCDZ); // IJMN,KLOP
    vec_float4 OPMN = vec_sld(NOPM, NOPM, 4);
    vec_float4 PMNO = vec_perm(inA3, inA2, _VECTORMATH_PERM_WCDZ); // KLOP,IJMN
    vec_float4 JKLI = vec_perm(inA2, inA3, _VECTORMATH_PERM_YABX); // IJMN,KLOP
    vec_float4 KLIJ = vec_sld(JKLI, JKLI, 4);
    vec_float4 LIJK = vec_perm(inA3, inA2, _VECTORMATH_PERM_YABX); // KLOP,IJMN
    
    vec_float4 KP_LO__IP_LM__IN_JM__KN_JO = vec_nmsub(LLJJ, OMMO, KP__IP__IN__KN);
    vec_float4 JP_LN__IO_KM__LN_JP__KM_IO = vec_nmsub(LKJI, NMPO, JP__IO__LN__KM);
    vec_float4 KN_JO__KP_LO__IP_LM__IN_JM = vec_nmsub(JLLJ, OOMM, KN__KP__IP__IN);
    vec_float4 CH_DG__AH_DE__AF_BE__CF_BG = vec_nmsub(DDBB, GEEG, CH__AH__AF__CF);
    vec_float4 BH_DF__AG_CE__DF_BH__CE_AG = vec_nmsub(DCBA, FEHG, BH__AG__DF__CE);
    vec_float4 CF_BG__CH_DG__AH_DE__AF_BE = vec_nmsub(BDDB, GGEE, CF__CH__AH__AF);
    
    vec_float4 tmpA1 = vec_madd(FGHE, KP_LO__IP_LM__IN_JM__KN_JO, vzero);
    vec_float4 tmpB1 = vec_nmsub(GHEF, JP_LN__IO_KM__LN_JP__KM_IO, tmpA1);
    vec_float4 out1 = vec_nmsub(HEFG, KN_JO__KP_LO__IP_LM__IN_JM, tmpB1);
    vec_float4 tmpB2 = vec_madd(CDAB, JP_LN__IO_KM__LN_JP__KM_IO, vzero);
    vec_float4 tmpA2 = vec_nmsub(BCDA, KP_LO__IP_LM__IN_JM__KN_JO, tmpB2);
    vec_float4 out2 = vec_madd(DABC, KN_JO__KP_LO__IP_LM__IN_JM, tmpA2);
    vec_float4 tmpA3 = vec_madd(NOPM, CH_DG__AH_DE__AF_BE__CF_BG, vzero);
    vec_float4 tmpB3 = vec_nmsub(OPMN, BH_DF__AG_CE__DF_BH__CE_AG, tmpA3);
    vec_float4 out3 = vec_nmsub(PMNO, CF_BG__CH_DG__AH_DE__AF_BE, tmpB3);
    vec_float4 tmpB4 = vec_madd(KLIJ, BH_DF__AG_CE__DF_BH__CE_AG, vzero);
    vec_float4 tmpA4 = vec_nmsub(JKLI, CH_DG__AH_DE__AF_BE__CF_BG, tmpB4);
    vec_float4 out4 = vec_madd(LIJK, CF_BG__CH_DG__AH_DE__AF_BE, tmpA4);
    
    //det
    vec_float4 AF_BE__AG_CE__CF_BG__AF_BE = 
        vec_perm(CH_DG__AH_DE__AF_BE__CF_BG, BH_DF__AG_CE__DF_BH__CE_AG, _VECTORMATH_PERM_ZBWX);
    vec_float4 KP_LO__JP_LN__IP_LM__IO_KM = 
        vec_mergeh(KP_LO__IP_LM__IN_JM__KN_JO, JP_LN__IO_KM__LN_JP__KM_IO);
    vec_float4 CH_DG__AH_DE__BH_DF__CH_DG =
        vec_perm(CH_DG__AH_DE__AF_BE__CF_BG, BH_DF__AG_CE__DF_BH__CE_AG, _VECTORMATH_PERM_XYAX);
    vec_float4 IN_JM__KN_JO__IO_KM__IN_JM =
        vec_perm(KP_LO__IP_LM__IN_JM__KN_JO, JP_LN__IO_KM__LN_JP__KM_IO, _VECTORMATH_PERM_ZWBX);
    vec_float4 det0 = vec_madd(AF_BE__AG_CE__CF_BG__AF_BE, KP_LO__JP_LN__IP_LM__IO_KM, vzero);
    vec_float4 det1 = vec_madd(CH_DG__AH_DE__BH_DF__CH_DG, IN_JM__KN_JO__IO_KM__IN_JM, det0);
    vec_float4 detA0 = vec_splat(det1, 0);
    vec_float4 detA1 = vec_splat(det1, 1);
    vec_float4 detA2 = vec_splat(det1, 2);
    vec_float4 detB0 = vec_sub(detA0, detA1);
    vec_float4 det = vec_sub(detB0, detA2);
    vec_float4 y0 = vec_re(det);
    vec_float4 invdet = vec_madd(vec_nmsub(det, y0, (vec_float4)(1.0f) ), y0, y0);
    #else
    /* function implementation based on code from STIDC SDK:           */
    /* --------------------------------------------------------------  */
    /* PLEASE DO NOT MODIFY THIS SECTION                               */
    /* This prolog section is automatically generated.                 */
    /*                                                                 */
    /* (C)Copyright                                                    */
    /* Sony Computer Entertainment, Inc.,                              */
    /* Toshiba Corporation,                                            */
    /* International Business Machines Corporation,                    */
    /* 2001,2002.                                                      */
    /* S/T/I Confidential Information                                  */
    /* --------------------------------------------------------------  */
    vector float in0, in1, in2, in3;
    vector float tmp0, tmp1, tmp2, tmp3;
    vector float out1, out2, out3, out4;
    vector float t0, t1, t2, t3;
    vector float t01, t02, t03, t12, t23;
    vector float t1r, t2r;
    vector float t01r, t02r, t03r, t12r, t23r;
    vector float t1r3, t1r3r;
    vector float det, det0, det1, det2, det3, invdet;
    vector float vzero = (vector float)(0.0);
    in0 = mat.getCol0().get128();
    in1 = mat.getCol1().get128();
    in2 = mat.getCol2().get128();
    in3 = mat.getCol3().get128();
    /* Perform transform of the input matrix of the form:
     *    A B C D
     *    E F G H
     *    I J K L
     *    M N O P
     *
     * The pseudo transpose of the input matrix is trans:
     *    A E I M
     *    J N B F
     *    C G K O
     *    L P D H
     */
    tmp0 = vec_perm(in0, in1, _VECTORMATH_PERM_XAZC);   /* A E C G */
    tmp1 = vec_perm(in2, in3, _VECTORMATH_PERM_XAZC);   /* I M K O */
    tmp2 = vec_perm(in0, in1, _VECTORMATH_PERM_YBWD);   /* B F D H */
    tmp3 = vec_perm(in2, in3, _VECTORMATH_PERM_YBWD);   /* J N L P */
    t0 = vec_perm(tmp0, tmp1, _VECTORMATH_PERM_XYAB);   /* A E I M */
    t1 = vec_perm(tmp3, tmp2, _VECTORMATH_PERM_XYAB);   /* J N B F */
    t2 = vec_perm(tmp0, tmp1, _VECTORMATH_PERM_ZWCD);   /* C G K O */
    t3 = vec_perm(tmp3, tmp2, _VECTORMATH_PERM_ZWCD);   /* L P D H */
    /* Generate a cofactor matrix. The computed cofactors reside in
     * out1, out2, out3, out4.
     */
    t23 = vec_madd(t2, t3, vzero);              /* CL GP KD OH */
    t23 = vec_perm(t23, t23, _VECTORMATH_PERM_YXWZ);    /* GP CL OH KD */
    out1 = vec_nmsub(t1, t23, vzero);           /* -(JGP NCL FOH BKD) */
    out2 = vec_nmsub(t0, t23, vzero);           /* -(AGP ECL IOH MKD) */
    t23r = vec_sld(t23, t23, 8);                        /* OH KD GP CL */
    out1 = vec_madd(t1, t23r, out1);            /* JOH NKD BGP FCL + out1 */
    out2 = vec_madd(t0, t23r, out2);            /* AOH EKD IGP MCL + out2 */
    out2 = vec_sld(out2, out2, 8);              /* IGP MCL AOH EKD - IOH MKD AGP ECL */
    t12 = vec_madd(t1, t2, vzero);              /* JC NG BK FO */
    t12 = vec_perm(t12, t12, _VECTORMATH_PERM_YXWZ);    /* NG JC FO BK */
    out1 = vec_madd(t3, t12, out1);             /* LNG PJC DFO HBK + out1 */
    out4 = vec_madd(t0, t12, vzero);            /* ANG EJC IFO MBK */
    t12r = vec_sld(t12, t12, 8);                        /* FO BK NG JC */
    out1 = vec_nmsub(t3, t12r, out1);           /* out1 - LFO PBK DNG HJC */
    out4 = vec_nmsub(t0, t12r, out4);           /* out4 - AFO EBK ING MJC */
    out4 = vec_sld(out4, out4, 8);              /* ING MJC AFO EBK - IFO MBK ANG EJC */
    t1r = vec_sld(t1, t1, 8);                   /* B F J N */
    t2r = vec_sld(t2, t2, 8);                   /* K O C G */
    t1r3 = vec_madd(t1r, t3, vzero);            /* BL FP JD NH */
    t1r3 = vec_perm(t1r3, t1r3, _VECTORMATH_PERM_YXWZ); /* FP BL NH JD */
    out1 = vec_madd(t2r, t1r3, out1);           /* KFP OBL CNH GJD + out1 */
    out3 = vec_madd(t0, t1r3, vzero);           /* AFP EBL INH MJD */
    t1r3r = vec_sld(t1r3, t1r3, 8);             /* NH JD FP BL */
    out1 = vec_nmsub(t2r, t1r3r, out1);         /* out1 - KNH OJD CFP GBL */
    out3 = vec_nmsub(t0, t1r3r, out3);          /* out3 - ANH EJD IFP MBL */
    out3 = vec_sld(out3, out3, 8);              /* IFP MBL ANH EJD - INH MJD AFP EBL */
    t01 = vec_madd(t0, t1, vzero);              /* AJ EN IB MF */
    t01 = vec_perm(t01, t01, _VECTORMATH_PERM_YXWZ);    /* EN AJ MF IB */
    out3 = vec_nmsub(t3, t01, out3);            /* out3 - LEN PAJ DMF HIB */
    out4 = vec_madd(t2r, t01, out4);            /* KEN OAJ CMF GIB + out4 */ 
    t01r = vec_sld(t01, t01, 8);                        /* MF IB EN AJ */
    out3 = vec_madd(t3, t01r, out3);            /* LMF PIB DEN HAJ + out3 */
    out4 = vec_nmsub(t2r, t01r, out4);          /* out4 - KMF OIB CEN GAJ */
    t03 = vec_madd(t0, t3, vzero);              /* AL EP ID MH */
    t03 = vec_perm(t03, t03, _VECTORMATH_PERM_YXWZ);    /* EP AL MH ID */
    out2 = vec_nmsub(t2r, t03, out2);           /* out2 - KEP OAL CMH GID */
    out3 = vec_madd(t1, t03, out3);             /* JEP NAL BMH FID + out3 */
    t03r = vec_sld(t03, t03, 8);                        /* MH ID EP AL */
    out2 = vec_madd(t2r, t03r, out2);           /* KMH OID CEP GAL + out2 */
    out3 = vec_nmsub(t1, t03r, out3);           /* out3 - JMH NID BEP FAL */ 
    t02 = vec_madd(t0, t2r, vzero);             /* AK EO IC MG */
    t02 = vec_perm(t02, t02, _VECTORMATH_PERM_YXWZ);    /* E0 AK MG IC */
    out2 = vec_madd(t3, t02, out2);             /* LEO PAK DMG HIC + out2 */
    out4 = vec_nmsub(t1, t02, out4);            /* out4 - JEO NAK BMG FIC */
    t02r = vec_sld(t02, t02, 8);                        /* MG IC EO AK */
    out2 = vec_nmsub(t3, t02r, out2);           /* out2 - LMG PIC DEO HAK */
    out4 = vec_madd(t1, t02r, out4);            /* JMG NIC BEO FAK + out4 */
    /* Compute the determinant of the matrix 
     *
     * det = sum_across(t0 * out1);
     *
     * We perform a sum across the entire vector so that 
     * we don't have to splat the result when multiplying the
     * cofactors by the inverse of the determinant.
     */
    det  = vec_madd(t0, out1, vzero);
    det0 = vec_splat(det, 0);
    det1 = vec_splat(det, 1);
    det2 = vec_splat(det, 2);
    det3 = vec_splat(det, 3);
    det  = vec_add(det0, det1);
    det2 = vec_add(det2, det3);
    det  = vec_add(det, det2);
    /* Compute the reciprocal of the determinant.
     */
    invdet = recipf4(det);
    #endif
    /* Multiply the cofactors by the reciprocal of the determinant.
     */ 
    return Matrix4(
        Vector4( vec_madd(out1, invdet, vzero) ),
        Vector4( vec_madd(out2, invdet, vzero) ),
        Vector4( vec_madd(out3, invdet, vzero) ),
        Vector4( vec_madd(out4, invdet, vzero) )
    );
}

inline const Matrix4 affineInverse( const Matrix4 & mat )
{
    Transform3 affineMat;
    affineMat.setCol0( mat.getCol0().getXYZ( ) );
    affineMat.setCol1( mat.getCol1().getXYZ( ) );
    affineMat.setCol2( mat.getCol2().getXYZ( ) );
    affineMat.setCol3( mat.getCol3().getXYZ( ) );
    return Matrix4( inverse( affineMat ) );
}

inline const Matrix4 orthoInverse( const Matrix4 & mat )
{
    vec_float4 vfcol0 = mat.getCol0().get128();
    vec_float4 vfcol1 = mat.getCol1().get128();
    vec_float4 vfcol2 = mat.getCol2().get128();
    vec_float4 vfcol3 = mat.getCol3().get128();
    vec_float4 tmp0 = vec_mergeh( vfcol0, vfcol2 );
    vec_float4 tmp1 = vec_mergel( vfcol0, vfcol2 );
    vec_float4 inv0 = vec_perm( tmp0, vfcol1, _VECTORMATH_PERM_XAYD );
    vec_float4 inv1 = vec_perm( tmp0, vfcol1, _VECTORMATH_PERM_ZBWD );
    vec_float4 inv2 = vec_perm( tmp1, vfcol1, _VECTORMATH_PERM_XCYD );
    vec_float4 inv3 = (vec_float4)vec_xor( (vec_uint4)vfcol3, (vec_uint4)(0x80000000) );
    vec_float4 xxxx = vec_splat( inv3, 0 );
    vec_float4 yyyy = vec_splat( inv3, 1 );
    vec_float4 zzzz = vec_splat( inv3, 2 );
    vec_float4 tmp2 = vec_madd( inv0, xxxx, (vec_float4)(0.0f) );
    vec_float4 tmp3 = vec_madd( inv1, yyyy, tmp2 );
    vec_float4 tmp4 = vec_madd( inv2, zzzz, tmp3 );
    inv3 = vec_sel(tmp4, (vec_float4)(1.f), _VECTORMATH_MASK_0x000F);
    return Matrix4(Vector4(inv0), Vector4(inv1), Vector4(inv2), Vector4(inv3));
}

inline const floatInVec determinant( const Matrix4 & mat )
{
    /* function implementation based on code from STIDC SDK:           */
    /* --------------------------------------------------------------  */
    /* PLEASE DO NOT MODIFY THIS SECTION                               */
    /* This prolog section is automatically generated.                 */
    /*                                                                 */
    /* (C)Copyright                                                    */
    /* Sony Computer Entertainment, Inc.,                              */
    /* Toshiba Corporation,                                            */
    /* International Business Machines Corporation,                    */
    /* 2001,2002.                                                      */
    /* S/T/I Confidential Information                                  */
    /* --------------------------------------------------------------  */
    vector float in0, in1, in2, in3;
    vector float tmp0, tmp1, tmp2, tmp3;
    vector float cof0;
    vector float t0, t1, t2, t3;
    vector float t12, t23;
    vector float t1r, t2r;
    vector float t12r, t23r;
    vector float t1r3, t1r3r;
    vector float vzero = (vector float)(0.0);
    in0 = mat.getCol0().get128();
    in1 = mat.getCol1().get128();
    in2 = mat.getCol2().get128();
    in3 = mat.getCol3().get128();
    /* Perform transform of the input matrix of the form:
     *    A B C D
     *    E F G H
     *    I J K L
     *    M N O P
     *
     * The pseudo transpose of the input matrix is trans:
     *    A E I M
     *    J N B F
     *    C G K O
     *    L P D H
     */
    tmp0 = vec_perm(in0, in1, _VECTORMATH_PERM_XAZC);   /* A E C G */
    tmp1 = vec_perm(in2, in3, _VECTORMATH_PERM_XAZC);   /* I M K O */
    tmp2 = vec_perm(in0, in1, _VECTORMATH_PERM_YBWD);   /* B F D H */
    tmp3 = vec_perm(in2, in3, _VECTORMATH_PERM_YBWD);   /* J N L P */
    t0 = vec_perm(tmp0, tmp1, _VECTORMATH_PERM_XYAB);   /* A E I M */
    t1 = vec_perm(tmp3, tmp2, _VECTORMATH_PERM_XYAB);   /* J N B F */
    t2 = vec_perm(tmp0, tmp1, _VECTORMATH_PERM_ZWCD);   /* C G K O */
    t3 = vec_perm(tmp3, tmp2, _VECTORMATH_PERM_ZWCD);   /* L P D H */
    /* Generate a cofactor matrix. The computed cofactors reside in
     * cof0, cof1, cof2, cof3.
     */
    t23 = vec_madd(t2, t3, vzero);              /* CL GP KD OH */
    t23 = vec_perm(t23, t23, _VECTORMATH_PERM_YXWZ);    /* GP CL OH KD */
    cof0 = vec_nmsub(t1, t23, vzero);           /* -(JGP NCL FOH BKD) */
    t23r = vec_sld(t23, t23, 8);                        /* OH KD GP CL */
    cof0 = vec_madd(t1, t23r, cof0);            /* JOH NKD BGP FCL + cof0 */
    t12 = vec_madd(t1, t2, vzero);              /* JC NG BK FO */
    t12 = vec_perm(t12, t12, _VECTORMATH_PERM_YXWZ);    /* NG JC FO BK */
    cof0 = vec_madd(t3, t12, cof0);             /* LNG PJC DFO HBK + cof0 */
    t12r = vec_sld(t12, t12, 8);                        /* FO BK NG JC */
    cof0 = vec_nmsub(t3, t12r, cof0);           /* cof0 - LFO PBK DNG HJC */
    t1r = vec_sld(t1, t1, 8);                   /* B F J N */
    t2r = vec_sld(t2, t2, 8);                   /* K O C G */
    t1r3 = vec_madd(t1r, t3, vzero);            /* BL FP JD NH */
    t1r3 = vec_perm(t1r3, t1r3, _VECTORMATH_PERM_YXWZ); /* FP BL NH JD */
    cof0 = vec_madd(t2r, t1r3, cof0);           /* KFP OBL CNH GJD + cof0 */
    t1r3r = vec_sld(t1r3, t1r3, 8);             /* NH JD FP BL */
    cof0 = vec_nmsub(t2r, t1r3r, cof0);         /* cof0 - KNH OJD CFP GBL */
    return floatInVec( _vmathVfDot4(t0,cof0), 0 );
}

inline const Matrix4 Matrix4::operator +( const Matrix4 & mat ) const
{
    return Matrix4(
        ( mCol0 + mat.mCol0 ),
        ( mCol1 + mat.mCol1 ),
        ( mCol2 + mat.mCol2 ),
        ( mCol3 + mat.mCol3 )
    );
}

inline const Matrix4 Matrix4::operator -( const Matrix4 & mat ) const
{
    return Matrix4(
        ( mCol0 - mat.mCol0 ),
        ( mCol1 - mat.mCol1 ),
        ( mCol2 - mat.mCol2 ),
        ( mCol3 - mat.mCol3 )
    );
}

inline Matrix4 & Matrix4::operator +=( const Matrix4 & mat )
{
    *this = *this + mat;
    return *this;
}

inline Matrix4 & Matrix4::operator -=( const Matrix4 & mat )
{
    *this = *this - mat;
    return *this;
}

inline const Matrix4 Matrix4::operator -( ) const
{
    return Matrix4(
        ( -mCol0 ),
        ( -mCol1 ),
        ( -mCol2 ),
        ( -mCol3 )
    );
}

inline const Matrix4 absPerElem( const Matrix4 & mat )
{
    return Matrix4(
        absPerElem( mat.getCol0() ),
        absPerElem( mat.getCol1() ),
        absPerElem( mat.getCol2() ),
        absPerElem( mat.getCol3() )
    );
}

inline const Matrix4 Matrix4::operator *( float scalar ) const
{
    return *this * floatInVec(scalar);
}

inline const Matrix4 Matrix4::operator *( floatInVec scalar ) const
{
    return Matrix4(
        ( mCol0 * scalar ),
        ( mCol1 * scalar ),
        ( mCol2 * scalar ),
        ( mCol3 * scalar )
    );
}

inline Matrix4 & Matrix4::operator *=( float scalar )
{
    return *this *= floatInVec(scalar);
}

inline Matrix4 & Matrix4::operator *=( floatInVec scalar )
{
    *this = *this * scalar;
    return *this;
}

inline const Matrix4 operator *( float scalar, const Matrix4 & mat )
{
    return floatInVec(scalar) * mat;
}

inline const Matrix4 operator *( floatInVec scalar, const Matrix4 & mat )
{
    return mat * scalar;
}

inline const Vector4 Matrix4::operator *( Vector4 vec ) const
{
    vec_float4 tmp0, tmp1, res;
    vec_float4 xxxx, yyyy, zzzz, wwww;
    vec_float4 zero = (vec_float4)(0.0f);
    xxxx = vec_splat( vec.get128(), 0 );
    yyyy = vec_splat( vec.get128(), 1 );
    zzzz = vec_splat( vec.get128(), 2 );
    wwww = vec_splat( vec.get128(), 3 );
    tmp0 = vec_madd( mCol0.get128(), xxxx, zero );
    tmp1 = vec_madd( mCol1.get128(), yyyy, zero );
    tmp0 = vec_madd( mCol2.get128(), zzzz, tmp0 );
    tmp1 = vec_madd( mCol3.get128(), wwww, tmp1 );
    res = vec_add( tmp0, tmp1 );
    return Vector4( res );
}

inline const Vector4 Matrix4::operator *( Vector3 vec ) const
{
    vec_float4 res;
    vec_float4 xxxx, yyyy, zzzz;
    xxxx = vec_splat( vec.get128(), 0 );
    yyyy = vec_splat( vec.get128(), 1 );
    zzzz = vec_splat( vec.get128(), 2 );
    res = vec_madd( mCol0.get128(), xxxx, (vec_float4)(0.0f) );
    res = vec_madd( mCol1.get128(), yyyy, res );
    res = vec_madd( mCol2.get128(), zzzz, res );
    return Vector4( res );
}

inline const Vector4 Matrix4::operator *( Point3 pnt ) const
{
    vec_float4 tmp0, tmp1, res;
    vec_float4 xxxx, yyyy, zzzz;
    vec_float4 zero = (vec_float4)(0.0f);
    xxxx = vec_splat( pnt.get128(), 0 );
    yyyy = vec_splat( pnt.get128(), 1 );
    zzzz = vec_splat( pnt.get128(), 2 );
    tmp0 = vec_madd( mCol0.get128(), xxxx, zero );
    tmp1 = vec_madd( mCol1.get128(), yyyy, mCol3.get128() );
    tmp0 = vec_madd( mCol2.get128(), zzzz, tmp0 );
    res = vec_add( tmp0, tmp1 );
    return Vector4( res );
}

inline const Matrix4 Matrix4::operator *( const Matrix4 & mat ) const
{
    return Matrix4(
        ( *this * mat.mCol0 ),
        ( *this * mat.mCol1 ),
        ( *this * mat.mCol2 ),
        ( *this * mat.mCol3 )
    );
}

inline Matrix4 & Matrix4::operator *=( const Matrix4 & mat )
{
    *this = *this * mat;
    return *this;
}

inline const Matrix4 Matrix4::operator *( const Transform3 & tfrm ) const
{
    return Matrix4(
        ( *this * tfrm.getCol0() ),
        ( *this * tfrm.getCol1() ),
        ( *this * tfrm.getCol2() ),
        ( *this * Point3( tfrm.getCol3() ) )
    );
}

inline Matrix4 & Matrix4::operator *=( const Transform3 & tfrm )
{
    *this = *this * tfrm;
    return *this;
}

inline const Matrix4 mulPerElem( const Matrix4 & mat0, const Matrix4 & mat1 )
{
    return Matrix4(
        mulPerElem( mat0.getCol0(), mat1.getCol0() ),
        mulPerElem( mat0.getCol1(), mat1.getCol1() ),
        mulPerElem( mat0.getCol2(), mat1.getCol2() ),
        mulPerElem( mat0.getCol3(), mat1.getCol3() )
    );
}

inline const Matrix4 Matrix4::identity( )
{
    return Matrix4(
        Vector4::xAxis( ),
        Vector4::yAxis( ),
        Vector4::zAxis( ),
        Vector4::wAxis( )
    );
}

inline Matrix4 & Matrix4::setUpper3x3( const Matrix3 & mat3 )
{
    mCol0.setXYZ( mat3.getCol0() );
    mCol1.setXYZ( mat3.getCol1() );
    mCol2.setXYZ( mat3.getCol2() );
    return *this;
}

inline const Matrix3 Matrix4::getUpper3x3( ) const
{
    return Matrix3(
        mCol0.getXYZ( ),
        mCol1.getXYZ( ),
        mCol2.getXYZ( )
    );
}

inline Matrix4 & Matrix4::setTranslation( Vector3 translateVec )
{
    mCol3.setXYZ( translateVec );
    return *this;
}

inline const Vector3 Matrix4::getTranslation( ) const
{
    return mCol3.getXYZ( );
}

inline const Matrix4 Matrix4::rotationX( float radians )
{
    return rotationX( floatInVec(radians) );
}

inline const Matrix4 Matrix4::rotationX( floatInVec radians )
{
    vec_float4 s, c, res1, res2;
    vec_uint4 select_y, select_z;
    vec_float4 zero;
    select_y = _VECTORMATH_MASK_0x0F00;
    select_z = _VECTORMATH_MASK_0x00F0;
    zero = (vec_float4)(0.0f);
    sincosf4( radians.get128(), &s, &c );
    res1 = vec_sel( zero, c, select_y );
    res1 = vec_sel( res1, s, select_z );
    res2 = vec_sel( zero, negatef4(s), select_y );
    res2 = vec_sel( res2, c, select_z );
    return Matrix4(
        Vector4::xAxis( ),
        Vector4( res1 ),
        Vector4( res2 ),
        Vector4::wAxis( )
    );
}

inline const Matrix4 Matrix4::rotationY( float radians )
{
    return rotationY( floatInVec(radians) );
}

inline const Matrix4 Matrix4::rotationY( floatInVec radians )
{
    vec_float4 s, c, res0, res2;
    vec_uint4 select_x, select_z;
    vec_float4 zero;
    select_x = _VECTORMATH_MASK_0xF000;
    select_z = _VECTORMATH_MASK_0x00F0;
    zero = (vec_float4)(0.0f);
    sincosf4( radians.get128(), &s, &c );
    res0 = vec_sel( zero, c, select_x );
    res0 = vec_sel( res0, negatef4(s), select_z );
    res2 = vec_sel( zero, s, select_x );
    res2 = vec_sel( res2, c, select_z );
    return Matrix4(
        Vector4( res0 ),
        Vector4::yAxis( ),
        Vector4( res2 ),
        Vector4::wAxis( )
    );
}

inline const Matrix4 Matrix4::rotationZ( float radians )
{
    return rotationZ( floatInVec(radians) );
}

inline const Matrix4 Matrix4::rotationZ( floatInVec radians )
{
    vec_float4 s, c, res0, res1;
    vec_uint4 select_x, select_y;
    vec_float4 zero;
    select_x = _VECTORMATH_MASK_0xF000;
    select_y = _VECTORMATH_MASK_0x0F00;
    zero = (vec_float4)(0.0f);
    sincosf4( radians.get128(), &s, &c );
    res0 = vec_sel( zero, c, select_x );
    res0 = vec_sel( res0, s, select_y );
    res1 = vec_sel( zero, negatef4(s), select_x );
    res1 = vec_sel( res1, c, select_y );
    return Matrix4(
        Vector4( res0 ),
        Vector4( res1 ),
        Vector4::zAxis( ),
        Vector4::wAxis( )
    );
}

inline const Matrix4 Matrix4::rotationZYX( Vector3 radiansXYZ )
{
    vec_float4 angles, s, negS, c, X0, X1, Y0, Y1, Z0, Z1, tmp;
    vec_float4 zero = (vec_float4)(0.0f);
    angles = Vector4( radiansXYZ, 0.0f ).get128();
    sincosf4( angles, &s, &c );
    negS = negatef4( s );
    Z0 = vec_mergel( c, s );
    Z1 = vec_mergel( negS, c );
    Z1 = vec_andc( Z1, (vec_float4)_VECTORMATH_MASK_0x000F );
    Y0 = vec_perm( negS, c, _VECTORMATH_PERM_BBYX );
    Y1 = vec_perm( c, s, _VECTORMATH_PERM_BBYX );
    X0 = vec_splat( s, 0 );
    X1 = vec_splat( c, 0 );
    tmp = vec_madd( Z0, Y1, zero );
    return Matrix4(
        Vector4( vec_madd( Z0, Y0, zero ) ),
        Vector4( vec_madd( Z1, X1, vec_madd( tmp, X0, zero ) ) ),
        Vector4( vec_nmsub( Z1, X0, vec_madd( tmp, X1, zero ) ) ),
        Vector4::wAxis( )
    );
}

inline const Matrix4 Matrix4::rotation( float radians, Vector3 unitVec )
{
    return rotation( floatInVec(radians), unitVec );
}

inline const Matrix4 Matrix4::rotation( floatInVec radians, Vector3 unitVec )
{
    vec_float4 axis, s, c, oneMinusC, axisS, negAxisS, xxxx, yyyy, zzzz, tmp0, tmp1, tmp2, zeroW;
    vec_float4 zero = (vec_float4)(0.0f);
    axis = unitVec.get128();
    sincosf4( radians.get128(), &s, &c );
    xxxx = vec_splat( axis, 0 );
    yyyy = vec_splat( axis, 1 );
    zzzz = vec_splat( axis, 2 );
    oneMinusC = vec_sub( (vec_float4)(1.0f), c );
    axisS = vec_madd( axis, s, zero );
    negAxisS = negatef4( axisS );
    tmp0 = vec_perm( axisS, negAxisS, _VECTORMATH_PERM_XZBX );
    tmp1 = vec_perm( axisS, negAxisS, _VECTORMATH_PERM_CXXX );
    tmp2 = vec_perm( axisS, negAxisS, _VECTORMATH_PERM_YAXX );
    tmp0 = vec_sel( tmp0, c, _VECTORMATH_MASK_0xF000 );
    tmp1 = vec_sel( tmp1, c, _VECTORMATH_MASK_0x0F00 );
    tmp2 = vec_sel( tmp2, c, _VECTORMATH_MASK_0x00F0 );
    zeroW = (vec_float4)_VECTORMATH_MASK_0x000F;
    axis = vec_andc( axis, zeroW );
    tmp0 = vec_andc( tmp0, zeroW );
    tmp1 = vec_andc( tmp1, zeroW );
    tmp2 = vec_andc( tmp2, zeroW );
    return Matrix4(
        Vector4( vec_madd( vec_madd( axis, xxxx, zero ), oneMinusC, tmp0 ) ),
        Vector4( vec_madd( vec_madd( axis, yyyy, zero ), oneMinusC, tmp1 ) ),
        Vector4( vec_madd( vec_madd( axis, zzzz, zero ), oneMinusC, tmp2 ) ),
        Vector4::wAxis( )
    );
}

inline const Matrix4 Matrix4::rotation( Quat unitQuat )
{
    return Matrix4( Transform3::rotation( unitQuat ) );
}

inline const Matrix4 Matrix4::scale( Vector3 scaleVec )
{
    vec_float4 zero = (vec_float4)(0.0f);
    return Matrix4(
        Vector4( vec_sel( zero, scaleVec.get128(), _VECTORMATH_MASK_0xF000 ) ),
        Vector4( vec_sel( zero, scaleVec.get128(), _VECTORMATH_MASK_0x0F00 ) ),
        Vector4( vec_sel( zero, scaleVec.get128(), _VECTORMATH_MASK_0x00F0 ) ),
        Vector4::wAxis( )
    );
}

inline const Matrix4 appendScale( const Matrix4 & mat, Vector3 scaleVec )
{
    return Matrix4(
        ( mat.getCol0() * scaleVec.getX( ) ),
        ( mat.getCol1() * scaleVec.getY( ) ),
        ( mat.getCol2() * scaleVec.getZ( ) ),
        mat.getCol3()
    );
}

inline const Matrix4 prependScale( Vector3 scaleVec, const Matrix4 & mat )
{
    Vector4 scale4;
    scale4 = Vector4( scaleVec, 1.0f );
    return Matrix4(
        mulPerElem( mat.getCol0(), scale4 ),
        mulPerElem( mat.getCol1(), scale4 ),
        mulPerElem( mat.getCol2(), scale4 ),
        mulPerElem( mat.getCol3(), scale4 )
    );
}

inline const Matrix4 Matrix4::translation( Vector3 translateVec )
{
    return Matrix4(
        Vector4::xAxis( ),
        Vector4::yAxis( ),
        Vector4::zAxis( ),
        Vector4( translateVec, 1.0f )
    );
}

inline const Matrix4 Matrix4::lookAt( Point3 eyePos, Point3 lookAtPos, Vector3 upVec )
{
    Matrix4 m4EyeFrame;
    Vector3 v3X, v3Y, v3Z;
    v3Y = normalize( upVec );
    v3Z = normalize( ( eyePos - lookAtPos ) );
    v3X = normalize( cross( v3Y, v3Z ) );
    v3Y = cross( v3Z, v3X );
    m4EyeFrame = Matrix4( Vector4( v3X ), Vector4( v3Y ), Vector4( v3Z ), Vector4( eyePos ) );
    return orthoInverse( m4EyeFrame );
}

inline const Matrix4 Matrix4::perspective( float fovyRadians, float aspect, float zNear, float zFar )
{
    float f, rangeInv;
    vec_float4 zero, col0, col1, col2, col3;
    union { vec_float4 v; float s[4]; } tmp;
    f = ::tanf( _VECTORMATH_PI_OVER_2 - fovyRadians * 0.5f );
    rangeInv = 1.0f / ( zNear - zFar );
    zero = (vec_float4)(0.0f);
    tmp.v = zero;
    tmp.s[0] = f / aspect;
    col0 = tmp.v;
    tmp.v = zero;
    tmp.s[1] = f;
    col1 = tmp.v;
    tmp.v = zero;
    tmp.s[2] = ( zNear + zFar ) * rangeInv;
    tmp.s[3] = -1.0f;
    col2 = tmp.v;
    tmp.v = zero;
    tmp.s[2] = zNear * zFar * rangeInv * 2.0f;
    col3 = tmp.v;
    return Matrix4(
        Vector4( col0 ),
        Vector4( col1 ),
        Vector4( col2 ),
        Vector4( col3 )
    );
}

inline const Matrix4 Matrix4::frustum( float left, float right, float bottom, float top, float zNear, float zFar )
{
    /* function implementation based on code from STIDC SDK:           */
    /* --------------------------------------------------------------  */
    /* PLEASE DO NOT MODIFY THIS SECTION                               */
    /* This prolog section is automatically generated.                 */
    /*                                                                 */
    /* (C)Copyright                                                    */
    /* Sony Computer Entertainment, Inc.,                              */
    /* Toshiba Corporation,                                            */
    /* International Business Machines Corporation,                    */
    /* 2001,2002.                                                      */
    /* S/T/I Confidential Information                                  */
    /* --------------------------------------------------------------  */
    vec_float4 lbf, rtn;
    vec_float4 diff, sum, inv_diff;
    vec_float4 diagonal, column, near2;
    vec_float4 zero = (vec_float4)(0.0f);
    union { vec_float4 v; float s[4]; } l, f, r, n, b, t;
    l.s[0] = left;
    f.s[0] = zFar;
    r.s[0] = right;
    n.s[0] = zNear;
    b.s[0] = bottom;
    t.s[0] = top;
    lbf = vec_mergeh( l.v, f.v );
    rtn = vec_mergeh( r.v, n.v );
    lbf = vec_mergeh( lbf, b.v );
    rtn = vec_mergeh( rtn, t.v );
    diff = vec_sub( rtn, lbf );
    sum  = vec_add( rtn, lbf );
    vec_float4 y0 = vec_re(diff);
    inv_diff = vec_madd( vec_nmsub( diff, y0, (vec_float4)(1.0f) ), y0, y0 );
    near2 = vec_splat( n.v, 0 );
    near2 = vec_add( near2, near2 );
    diagonal = vec_madd( near2, inv_diff, zero );
    column = vec_madd( sum, inv_diff, zero );
    return Matrix4(
        Vector4( vec_sel( zero, diagonal, _VECTORMATH_MASK_0xF000 ) ),
        Vector4( vec_sel( zero, diagonal, _VECTORMATH_MASK_0x0F00 ) ),
        Vector4( vec_sel( column, (vec_float4)(-1.0f), _VECTORMATH_MASK_0x000F ) ),
        Vector4( vec_sel( zero, vec_madd( diagonal, vec_splat( f.v, 0 ), zero ), _VECTORMATH_MASK_0x00F0 ) )
    );
}

inline const Matrix4 Matrix4::orthographic( float left, float right, float bottom, float top, float zNear, float zFar )
{
    /* function implementation based on code from STIDC SDK:           */
    /* --------------------------------------------------------------  */
    /* PLEASE DO NOT MODIFY THIS SECTION                               */
    /* This prolog section is automatically generated.                 */
    /*                                                                 */
    /* (C)Copyright                                                    */
    /* Sony Computer Entertainment, Inc.,                              */
    /* Toshiba Corporation,                                            */
    /* International Business Machines Corporation,                    */
    /* 2001,2002.                                                      */
    /* S/T/I Confidential Information                                  */
    /* --------------------------------------------------------------  */
    vec_float4 lbf, rtn;
    vec_float4 diff, sum, inv_diff, neg_inv_diff;
    vec_float4 diagonal, column;
    vec_float4 zero = (vec_float4)(0.0f);
    union { vec_float4 v; float s[4]; } l, f, r, n, b, t;
    l.s[0] = left;
    f.s[0] = zFar;
    r.s[0] = right;
    n.s[0] = zNear;
    b.s[0] = bottom;
    t.s[0] = top;
    lbf = vec_mergeh( l.v, f.v );
    rtn = vec_mergeh( r.v, n.v );
    lbf = vec_mergeh( lbf, b.v );
    rtn = vec_mergeh( rtn, t.v );
    diff = vec_sub( rtn, lbf );
    sum  = vec_add( rtn, lbf );
    vec_float4 y0 = vec_re(diff);
    inv_diff = vec_madd( vec_nmsub( diff, y0, (vec_float4)(1.0f) ), y0, y0 );
    neg_inv_diff = negatef4( inv_diff );
    diagonal = vec_add( inv_diff, inv_diff );
    column = vec_madd( sum, vec_sel( neg_inv_diff, inv_diff, _VECTORMATH_MASK_0x00F0 ), zero );
    return Matrix4(
        Vector4( vec_sel( zero, diagonal, _VECTORMATH_MASK_0xF000 ) ),
        Vector4( vec_sel( zero, diagonal, _VECTORMATH_MASK_0x0F00 ) ),
        Vector4( vec_sel( zero, diagonal, _VECTORMATH_MASK_0x00F0 ) ),
        Vector4( vec_sel( column, (vec_float4)(1.0f), _VECTORMATH_MASK_0x000F ) )
    );
}

inline const Matrix4 select( const Matrix4 & mat0, const Matrix4 & mat1, bool select1 )
{
    boolInVec select1_( select1 );
    return Matrix4(
        select( mat0.getCol0(), mat1.getCol0(), select1_ ),
        select( mat0.getCol1(), mat1.getCol1(), select1_ ),
        select( mat0.getCol2(), mat1.getCol2(), select1_ ),
        select( mat0.getCol3(), mat1.getCol3(), select1_ )
    );
}

inline const Matrix4 select( const Matrix4 & mat0, const Matrix4 & mat1, boolInVec select1 )
{
    return Matrix4(
        select( mat0.getCol0(), mat1.getCol0(), select1 ),
        select( mat0.getCol1(), mat1.getCol1(), select1 ),
        select( mat0.getCol2(), mat1.getCol2(), select1 ),
        select( mat0.getCol3(), mat1.getCol3(), select1 )
    );
}

#ifdef _VECTORMATH_DEBUG

inline void print( const Matrix4 & mat )
{
    print( mat.getRow( 0 ) );
    print( mat.getRow( 1 ) );
    print( mat.getRow( 2 ) );
    print( mat.getRow( 3 ) );
}

inline void print( const Matrix4 & mat, const char * name )
{
    printf("%s:\n", name);
    print( mat );
}

#endif

inline Transform3::Transform3( const Transform3 & tfrm )
{
    mCol0 = tfrm.mCol0;
    mCol1 = tfrm.mCol1;
    mCol2 = tfrm.mCol2;
    mCol3 = tfrm.mCol3;
}

inline Transform3::Transform3( float scalar )
{
    mCol0 = Vector3( scalar );
    mCol1 = Vector3( scalar );
    mCol2 = Vector3( scalar );
    mCol3 = Vector3( scalar );
}

inline Transform3::Transform3( floatInVec scalar )
{
    mCol0 = Vector3( scalar );
    mCol1 = Vector3( scalar );
    mCol2 = Vector3( scalar );
    mCol3 = Vector3( scalar );
}

inline Transform3::Transform3( Vector3 _col0, Vector3 _col1, Vector3 _col2, Vector3 _col3 )
{
    mCol0 = _col0;
    mCol1 = _col1;
    mCol2 = _col2;
    mCol3 = _col3;
}

inline Transform3::Transform3( const Matrix3 & tfrm, Vector3 translateVec )
{
    this->setUpper3x3( tfrm );
    this->setTranslation( translateVec );
}

inline Transform3::Transform3( Quat unitQuat, Vector3 translateVec )
{
    this->setUpper3x3( Matrix3( unitQuat ) );
    this->setTranslation( translateVec );
}

inline Transform3 & Transform3::setCol0( Vector3 _col0 )
{
    mCol0 = _col0;
    return *this;
}

inline Transform3 & Transform3::setCol1( Vector3 _col1 )
{
    mCol1 = _col1;
    return *this;
}

inline Transform3 & Transform3::setCol2( Vector3 _col2 )
{
    mCol2 = _col2;
    return *this;
}

inline Transform3 & Transform3::setCol3( Vector3 _col3 )
{
    mCol3 = _col3;
    return *this;
}

inline Transform3 & Transform3::setCol( int col, Vector3 vec )
{
    *(&mCol0 + col) = vec;
    return *this;
}

inline Transform3 & Transform3::setRow( int row, Vector4 vec )
{
    mCol0.setElem( row, vec.getElem( 0 ) );
    mCol1.setElem( row, vec.getElem( 1 ) );
    mCol2.setElem( row, vec.getElem( 2 ) );
    mCol3.setElem( row, vec.getElem( 3 ) );
    return *this;
}

inline Transform3 & Transform3::setElem( int col, int row, float val )
{
    (*this)[col].setElem(row, val);
    return *this;
}

inline Transform3 & Transform3::setElem( int col, int row, floatInVec val )
{
    Vector3 tmpV3_0;
    tmpV3_0 = this->getCol( col );
    tmpV3_0.setElem( row, val );
    this->setCol( col, tmpV3_0 );
    return *this;
}

inline const floatInVec Transform3::getElem( int col, int row ) const
{
    return this->getCol( col ).getElem( row );
}

inline const Vector3 Transform3::getCol0( ) const
{
    return mCol0;
}

inline const Vector3 Transform3::getCol1( ) const
{
    return mCol1;
}

inline const Vector3 Transform3::getCol2( ) const
{
    return mCol2;
}

inline const Vector3 Transform3::getCol3( ) const
{
    return mCol3;
}

inline const Vector3 Transform3::getCol( int col ) const
{
    return *(&mCol0 + col);
}

inline const Vector4 Transform3::getRow( int row ) const
{
    return Vector4( mCol0.getElem( row ), mCol1.getElem( row ), mCol2.getElem( row ), mCol3.getElem( row ) );
}

inline Vector3 & Transform3::operator []( int col )
{
    return *(&mCol0 + col);
}

inline const Vector3 Transform3::operator []( int col ) const
{
    return *(&mCol0 + col);
}

inline Transform3 & Transform3::operator =( const Transform3 & tfrm )
{
    mCol0 = tfrm.mCol0;
    mCol1 = tfrm.mCol1;
    mCol2 = tfrm.mCol2;
    mCol3 = tfrm.mCol3;
    return *this;
}

inline const Transform3 inverse( const Transform3 & tfrm )
{
    vec_float4 inv0, inv1, inv2, inv3;
    vec_float4 tmp0, tmp1, tmp2, tmp3, tmp4, dot, invdet;
    vec_float4 xxxx, yyyy, zzzz;
    vec_float4 zero = (vec_float4)(0.0f);
    tmp2 = _vmathVfCross( tfrm.getCol0().get128(), tfrm.getCol1().get128() );
    tmp0 = _vmathVfCross( tfrm.getCol1().get128(), tfrm.getCol2().get128() );
    tmp1 = _vmathVfCross( tfrm.getCol2().get128(), tfrm.getCol0().get128() );
    inv3 = negatef4( tfrm.getCol3().get128() );
    dot = _vmathVfDot3( tmp2, tfrm.getCol2().get128() );
    dot = vec_splat( dot, 0 );
    vec_float4 y0 = vec_re(dot);
    invdet = vec_madd( vec_nmsub( dot, y0, (vec_float4)(1.0f) ), y0, y0 );
    tmp3 = vec_mergeh( tmp0, tmp2 );
    tmp4 = vec_mergel( tmp0, tmp2 );
    inv0 = vec_mergeh( tmp3, tmp1 );
    xxxx = vec_splat( inv3, 0 );
    inv1 = vec_perm( tmp3, tmp1, _VECTORMATH_PERM_ZBWX );
    inv2 = vec_perm( tmp4, tmp1, _VECTORMATH_PERM_XCYX );
    yyyy = vec_splat( inv3, 1 );
    zzzz = vec_splat( inv3, 2 );
    inv3 = vec_madd( inv0, xxxx, zero );
    inv3 = vec_madd( inv1, yyyy, inv3 );
    inv3 = vec_madd( inv2, zzzz, inv3 );
    inv0 = vec_madd( inv0, invdet, zero );
    inv1 = vec_madd( inv1, invdet, zero );
    inv2 = vec_madd( inv2, invdet, zero );
    inv3 = vec_madd( inv3, invdet, zero );
    return Transform3(
        Vector3( inv0 ),
        Vector3( inv1 ),
        Vector3( inv2 ),
        Vector3( inv3 )
    );
}

inline const Transform3 orthoInverse( const Transform3 & tfrm )
{
    vec_float4 in0 = tfrm.getCol0().get128();
    vec_float4 in1 = tfrm.getCol1().get128();
    vec_float4 in2 = tfrm.getCol2().get128();
    vec_float4 in3 = tfrm.getCol3().get128();
    vec_float4 tmp0 = vec_mergeh(in0, in2);
    vec_float4 tmp1 = vec_mergel(in0, in2);
    vec_float4 inv3 = negatef4(in3);
    vec_float4 inv0 = vec_mergeh( tmp0, in1);
    vec_float4 xxxx = vec_splat( inv3, 0 );
    vec_float4 inv1 = vec_perm( tmp0, in1, _VECTORMATH_PERM_ZBWX );
    vec_float4 inv2 = vec_perm( tmp1, in1, _VECTORMATH_PERM_XCYX );
    vec_float4 yyyy = vec_splat( inv3, 1 );
    vec_float4 zzzz = vec_splat( inv3, 2 );
    inv3 = vec_madd( inv0, xxxx, (vec_float4)(0.0f) );
    inv3 = vec_madd( inv1, yyyy, inv3 );
    inv3 = vec_madd( inv2, zzzz, inv3 );
    return Transform3(
        Vector3( inv0 ),
        Vector3( inv1 ),
        Vector3( inv2 ),
        Vector3( inv3 )
    );
}

inline const Transform3 absPerElem( const Transform3 & tfrm )
{
    return Transform3(
        absPerElem( tfrm.getCol0() ),
        absPerElem( tfrm.getCol1() ),
        absPerElem( tfrm.getCol2() ),
        absPerElem( tfrm.getCol3() )
    );
}

inline const Vector3 Transform3::operator *( Vector3 vec ) const
{
    vec_float4 res;
    vec_float4 xxxx, yyyy, zzzz;
    xxxx = vec_splat( vec.get128(), 0 );
    yyyy = vec_splat( vec.get128(), 1 );
    zzzz = vec_splat( vec.get128(), 2 );
    res = vec_madd( mCol0.get128(), xxxx, (vec_float4)(0.0f) );
    res = vec_madd( mCol1.get128(), yyyy, res );
    res = vec_madd( mCol2.get128(), zzzz, res );
    return Vector3( res );
}

inline const Point3 Transform3::operator *( Point3 pnt ) const
{
    vec_float4 tmp0, tmp1, res;
    vec_float4 xxxx, yyyy, zzzz;
    vec_float4 zero = (vec_float4)(0.0f);
    xxxx = vec_splat( pnt.get128(), 0 );
    yyyy = vec_splat( pnt.get128(), 1 );
    zzzz = vec_splat( pnt.get128(), 2 );
    tmp0 = vec_madd( mCol0.get128(), xxxx, zero );
    tmp1 = vec_madd( mCol1.get128(), yyyy, mCol3.get128() );
    tmp0 = vec_madd( mCol2.get128(), zzzz, tmp0 );
    res = vec_add( tmp0, tmp1 );
    return Point3( res );
}

inline const Transform3 Transform3::operator *( const Transform3 & tfrm ) const
{
    return Transform3(
        ( *this * tfrm.mCol0 ),
        ( *this * tfrm.mCol1 ),
        ( *this * tfrm.mCol2 ),
        Vector3( ( *this * Point3( tfrm.mCol3 ) ) )
    );
}

inline Transform3 & Transform3::operator *=( const Transform3 & tfrm )
{
    *this = *this * tfrm;
    return *this;
}

inline const Transform3 mulPerElem( const Transform3 & tfrm0, const Transform3 & tfrm1 )
{
    return Transform3(
        mulPerElem( tfrm0.getCol0(), tfrm1.getCol0() ),
        mulPerElem( tfrm0.getCol1(), tfrm1.getCol1() ),
        mulPerElem( tfrm0.getCol2(), tfrm1.getCol2() ),
        mulPerElem( tfrm0.getCol3(), tfrm1.getCol3() )
    );
}

inline const Transform3 Transform3::identity( )
{
    return Transform3(
        Vector3::xAxis( ),
        Vector3::yAxis( ),
        Vector3::zAxis( ),
        Vector3( 0.0f )
    );
}

inline Transform3 & Transform3::setUpper3x3( const Matrix3 & tfrm )
{
    mCol0 = tfrm.getCol0();
    mCol1 = tfrm.getCol1();
    mCol2 = tfrm.getCol2();
    return *this;
}

inline const Matrix3 Transform3::getUpper3x3( ) const
{
    return Matrix3( mCol0, mCol1, mCol2 );
}

inline Transform3 & Transform3::setTranslation( Vector3 translateVec )
{
    mCol3 = translateVec;
    return *this;
}

inline const Vector3 Transform3::getTranslation( ) const
{
    return mCol3;
}

inline const Transform3 Transform3::rotationX( float radians )
{
    return rotationX( floatInVec(radians) );
}

inline const Transform3 Transform3::rotationX( floatInVec radians )
{
    vec_float4 s, c, res1, res2;
    vec_uint4 select_y, select_z;
    vec_float4 zero;
    select_y = _VECTORMATH_MASK_0x0F00;
    select_z = _VECTORMATH_MASK_0x00F0;
    zero = (vec_float4)(0.0f);
    sincosf4( radians.get128(), &s, &c );
    res1 = vec_sel( zero, c, select_y );
    res1 = vec_sel( res1, s, select_z );
    res2 = vec_sel( zero, negatef4(s), select_y );
    res2 = vec_sel( res2, c, select_z );
    return Transform3(
        Vector3::xAxis( ),
        Vector3( res1 ),
        Vector3( res2 ),
        Vector3( 0.0f )
    );
}

inline const Transform3 Transform3::rotationY( float radians )
{
    return rotationY( floatInVec(radians) );
}

inline const Transform3 Transform3::rotationY( floatInVec radians )
{
    vec_float4 s, c, res0, res2;
    vec_uint4 select_x, select_z;
    vec_float4 zero;
    select_x = _VECTORMATH_MASK_0xF000;
    select_z = _VECTORMATH_MASK_0x00F0;
    zero = (vec_float4)(0.0f);
    sincosf4( radians.get128(), &s, &c );
    res0 = vec_sel( zero, c, select_x );
    res0 = vec_sel( res0, negatef4(s), select_z );
    res2 = vec_sel( zero, s, select_x );
    res2 = vec_sel( res2, c, select_z );
    return Transform3(
        Vector3( res0 ),
        Vector3::yAxis( ),
        Vector3( res2 ),
        Vector3( 0.0f )
    );
}

inline const Transform3 Transform3::rotationZ( float radians )
{
    return rotationZ( floatInVec(radians) );
}

inline const Transform3 Transform3::rotationZ( floatInVec radians )
{
    vec_float4 s, c, res0, res1;
    vec_uint4 select_x, select_y;
    vec_float4 zero;
    select_x = _VECTORMATH_MASK_0xF000;
    select_y = _VECTORMATH_MASK_0x0F00;
    zero = (vec_float4)(0.0f);
    sincosf4( radians.get128(), &s, &c );
    res0 = vec_sel( zero, c, select_x );
    res0 = vec_sel( res0, s, select_y );
    res1 = vec_sel( zero, negatef4(s), select_x );
    res1 = vec_sel( res1, c, select_y );
    return Transform3(
        Vector3( res0 ),
        Vector3( res1 ),
        Vector3::zAxis( ),
        Vector3( 0.0f )
    );
}

inline const Transform3 Transform3::rotationZYX( Vector3 radiansXYZ )
{
    vec_float4 angles, s, negS, c, X0, X1, Y0, Y1, Z0, Z1, tmp;
    vec_float4 zero = (vec_float4)(0.0f);
    angles = Vector4( radiansXYZ, 0.0f ).get128();
    sincosf4( angles, &s, &c );
    negS = negatef4( s );
    Z0 = vec_mergel( c, s );
    Z1 = vec_mergel( negS, c );
    Z1 = vec_andc( Z1, (vec_float4)_VECTORMATH_MASK_0x000F );
    Y0 = vec_perm( negS, c, _VECTORMATH_PERM_BBYX );
    Y1 = vec_perm( c, s, _VECTORMATH_PERM_BBYX );
    X0 = vec_splat( s, 0 );
    X1 = vec_splat( c, 0 );
    tmp = vec_madd( Z0, Y1, zero );
    return Transform3(
        Vector3( vec_madd( Z0, Y0, zero ) ),
        Vector3( vec_madd( Z1, X1, vec_madd( tmp, X0, zero ) ) ),
        Vector3( vec_nmsub( Z1, X0, vec_madd( tmp, X1, zero ) ) ),
        Vector3( 0.0f )
    );
}

inline const Transform3 Transform3::rotation( float radians, Vector3 unitVec )
{
    return rotation( floatInVec(radians), unitVec );
}

inline const Transform3 Transform3::rotation( floatInVec radians, Vector3 unitVec )
{
    return Transform3( Matrix3::rotation( radians, unitVec ), Vector3( 0.0f ) );
}

inline const Transform3 Transform3::rotation( Quat unitQuat )
{
    return Transform3( Matrix3( unitQuat ), Vector3( 0.0f ) );
}

inline const Transform3 Transform3::scale( Vector3 scaleVec )
{
    vec_float4 zero = (vec_float4)(0.0f);
    return Transform3(
        Vector3( vec_sel( zero, scaleVec.get128(), _VECTORMATH_MASK_0xF000 ) ),
        Vector3( vec_sel( zero, scaleVec.get128(), _VECTORMATH_MASK_0x0F00 ) ),
        Vector3( vec_sel( zero, scaleVec.get128(), _VECTORMATH_MASK_0x00F0 ) ),
        Vector3( 0.0f )
    );
}

inline const Transform3 appendScale( const Transform3 & tfrm, Vector3 scaleVec )
{
    return Transform3(
        ( tfrm.getCol0() * scaleVec.getX( ) ),
        ( tfrm.getCol1() * scaleVec.getY( ) ),
        ( tfrm.getCol2() * scaleVec.getZ( ) ),
        tfrm.getCol3()
    );
}

inline const Transform3 prependScale( Vector3 scaleVec, const Transform3 & tfrm )
{
    return Transform3(
        mulPerElem( tfrm.getCol0(), scaleVec ),
        mulPerElem( tfrm.getCol1(), scaleVec ),
        mulPerElem( tfrm.getCol2(), scaleVec ),
        mulPerElem( tfrm.getCol3(), scaleVec )
    );
}

inline const Transform3 Transform3::translation( Vector3 translateVec )
{
    return Transform3(
        Vector3::xAxis( ),
        Vector3::yAxis( ),
        Vector3::zAxis( ),
        translateVec
    );
}

inline const Transform3 select( const Transform3 & tfrm0, const Transform3 & tfrm1, bool select1 )
{
    boolInVec select1_( select1 );
    return Transform3(
        select( tfrm0.getCol0(), tfrm1.getCol0(), select1_ ),
        select( tfrm0.getCol1(), tfrm1.getCol1(), select1_ ),
        select( tfrm0.getCol2(), tfrm1.getCol2(), select1_ ),
        select( tfrm0.getCol3(), tfrm1.getCol3(), select1_ )
    );
}

inline const Transform3 select( const Transform3 & tfrm0, const Transform3 & tfrm1, boolInVec select1 )
{
    return Transform3(
        select( tfrm0.getCol0(), tfrm1.getCol0(), select1 ),
        select( tfrm0.getCol1(), tfrm1.getCol1(), select1 ),
        select( tfrm0.getCol2(), tfrm1.getCol2(), select1 ),
        select( tfrm0.getCol3(), tfrm1.getCol3(), select1 )
    );
}

#ifdef _VECTORMATH_DEBUG

inline void print( const Transform3 & tfrm )
{
    print( tfrm.getRow( 0 ) );
    print( tfrm.getRow( 1 ) );
    print( tfrm.getRow( 2 ) );
}

inline void print( const Transform3 & tfrm, const char * name )
{
    printf("%s:\n", name);
    print( tfrm );
}

#endif

inline Quat::Quat( const Matrix3 & tfrm )
{
    vec_float4 res;
    vec_float4 col0, col1, col2;
    vec_float4 xx_yy, xx_yy_zz_xx, yy_zz_xx_yy, zz_xx_yy_zz, diagSum, diagDiff;
    vec_float4 zy_xz_yx, yz_zx_xy, sum, diff;
    vec_float4 radicand, invSqrt, scale;
    vec_float4 res0, res1, res2, res3;
    vec_float4 xx, yy, zz;
    vec_uint4 select_x = _VECTORMATH_MASK_0xF000;
    vec_uint4 select_y = _VECTORMATH_MASK_0x0F00;
    vec_uint4 select_z = _VECTORMATH_MASK_0x00F0;
    vec_uint4 select_w = _VECTORMATH_MASK_0x000F;
    vec_float4 zero = (vec_float4)(0.0f);

    col0 = tfrm.getCol0().get128();
    col1 = tfrm.getCol1().get128();
    col2 = tfrm.getCol2().get128();

    /* four cases: */
    /* trace > 0 */
    /* else */
    /*    xx largest diagonal element */
    /*    yy largest diagonal element */
    /*    zz largest diagonal element */

    /* compute quaternion for each case */

    xx_yy = vec_sel( col0, col1, select_y );
    xx_yy_zz_xx = vec_perm( xx_yy, col2, _VECTORMATH_PERM_XYCX );
    yy_zz_xx_yy = vec_perm( xx_yy, col2, _VECTORMATH_PERM_YCXY );
    zz_xx_yy_zz = vec_perm( xx_yy, col2, _VECTORMATH_PERM_CXYC );

    diagSum = vec_add( vec_add( xx_yy_zz_xx, yy_zz_xx_yy ), zz_xx_yy_zz );
    diagDiff = vec_sub( vec_sub( xx_yy_zz_xx, yy_zz_xx_yy ), zz_xx_yy_zz );
    radicand = vec_add( vec_sel( diagDiff, diagSum, select_w ), (vec_float4)(1.0f) );
    invSqrt = rsqrtf4( radicand );

    zy_xz_yx = vec_sel( col0, col1, select_z );
    zy_xz_yx = vec_perm( zy_xz_yx, col2, _VECTORMATH_PERM_ZAYX );
    yz_zx_xy = vec_sel( col0, col1, select_x );
    yz_zx_xy = vec_perm( yz_zx_xy, col2, _VECTORMATH_PERM_BZXX );

    sum = vec_add( zy_xz_yx, yz_zx_xy );
    diff = vec_sub( zy_xz_yx, yz_zx_xy );

    scale = vec_madd( invSqrt, (vec_float4)(0.5f), zero );
    res0 = vec_perm( sum, diff, _VECTORMATH_PERM_XZYA );
    res1 = vec_perm( sum, diff, _VECTORMATH_PERM_ZXXB );
    res2 = vec_perm( sum, diff, _VECTORMATH_PERM_YXXC );
    res3 = diff;
    res0 = vec_sel( res0, radicand, select_x );
    res1 = vec_sel( res1, radicand, select_y );
    res2 = vec_sel( res2, radicand, select_z );
    res3 = vec_sel( res3, radicand, select_w );
    res0 = vec_madd( res0, vec_splat( scale, 0 ), zero );
    res1 = vec_madd( res1, vec_splat( scale, 1 ), zero );
    res2 = vec_madd( res2, vec_splat( scale, 2 ), zero );
    res3 = vec_madd( res3, vec_splat( scale, 3 ), zero );

    /* determine case and select answer */

    xx = vec_splat( col0, 0 );
    yy = vec_splat( col1, 1 );
    zz = vec_splat( col2, 2 );
    res = vec_sel( res0, res1, vec_cmpgt( yy, xx ) );
    res = vec_sel( res, res2, vec_and( vec_cmpgt( zz, xx ), vec_cmpgt( zz, yy ) ) );
    res = vec_sel( res, res3, vec_cmpgt( vec_splat( diagSum, 0 ), zero ) );
    mVec128 = res;
}

inline const Matrix3 outer( Vector3 tfrm0, Vector3 tfrm1 )
{
    return Matrix3(
        ( tfrm0 * tfrm1.getX( ) ),
        ( tfrm0 * tfrm1.getY( ) ),
        ( tfrm0 * tfrm1.getZ( ) )
    );
}

inline const Matrix4 outer( Vector4 tfrm0, Vector4 tfrm1 )
{
    return Matrix4(
        ( tfrm0 * tfrm1.getX( ) ),
        ( tfrm0 * tfrm1.getY( ) ),
        ( tfrm0 * tfrm1.getZ( ) ),
        ( tfrm0 * tfrm1.getW( ) )
    );
}

inline const Vector3 rowMul( Vector3 vec, const Matrix3 & mat )
{
    vec_float4 tmp0, tmp1, mcol0, mcol1, mcol2, res;
    vec_float4 xxxx, yyyy, zzzz;
    tmp0 = vec_mergeh( mat.getCol0().get128(), mat.getCol2().get128() );
    tmp1 = vec_mergel( mat.getCol0().get128(), mat.getCol2().get128() );
    xxxx = vec_splat( vec.get128(), 0 );
    mcol0 = vec_mergeh( tmp0, mat.getCol1().get128() );
    mcol1 = vec_perm( tmp0, mat.getCol1().get128(), _VECTORMATH_PERM_ZBWX );
    mcol2 = vec_perm( tmp1, mat.getCol1().get128(), _VECTORMATH_PERM_XCYX );
    yyyy = vec_splat( vec.get128(), 1 );
    res = vec_madd( mcol0, xxxx, (vec_float4)(0.0f) );
    zzzz = vec_splat( vec.get128(), 2 );
    res = vec_madd( mcol1, yyyy, res );
    res = vec_madd( mcol2, zzzz, res );
    return Vector3( res );
}

inline const Matrix3 crossMatrix( Vector3 vec )
{
    vec_float4 neg, res0, res1, res2;
    neg = negatef4( vec.get128() );
    res0 = vec_perm( vec.get128(), neg, _VECTORMATH_PERM_XZBX );
    res1 = vec_perm( vec.get128(), neg, _VECTORMATH_PERM_CXXX );
    res2 = vec_perm( vec.get128(), neg, _VECTORMATH_PERM_YAXX );
    res0 = vec_andc( res0, (vec_float4)_VECTORMATH_MASK_0xF000 );
    res1 = vec_andc( res1, (vec_float4)_VECTORMATH_MASK_0x0F00 );
    res2 = vec_andc( res2, (vec_float4)_VECTORMATH_MASK_0x00F0 );
    return Matrix3(
        Vector3( res0 ),
        Vector3( res1 ),
        Vector3( res2 )
    );
}

inline const Matrix3 crossMatrixMul( Vector3 vec, const Matrix3 & mat )
{
    return Matrix3( cross( vec, mat.getCol0() ), cross( vec, mat.getCol1() ), cross( vec, mat.getCol2() ) );
}

} // namespace Aos
} // namespace Vectormath

#endif
