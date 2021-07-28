/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2008 Sony Computer Entertainment Inc.
* All Rights Reserved.
*/

#ifndef _VECTORMATH_VEC_AOS_CPP_H
#define _VECTORMATH_VEC_AOS_CPP_H

//-----------------------------------------------------------------------------
// Constants
// for permutes words are labeled [x,y,z,w] [a,b,c,d]

#define _VECTORMATH_PERM_X 0x00010203
#define _VECTORMATH_PERM_Y 0x04050607
#define _VECTORMATH_PERM_Z 0x08090a0b
#define _VECTORMATH_PERM_W 0x0c0d0e0f
#define _VECTORMATH_PERM_A 0x10111213
#define _VECTORMATH_PERM_B 0x14151617
#define _VECTORMATH_PERM_C 0x18191a1b
#define _VECTORMATH_PERM_D 0x1c1d1e1f
#define _VECTORMATH_PERM_XYZA (vec_uchar16)(vec_uint4){ _VECTORMATH_PERM_X, _VECTORMATH_PERM_Y, _VECTORMATH_PERM_Z, _VECTORMATH_PERM_A }
#define _VECTORMATH_PERM_ZXYW (vec_uchar16)(vec_uint4){ _VECTORMATH_PERM_Z, _VECTORMATH_PERM_X, _VECTORMATH_PERM_Y, _VECTORMATH_PERM_W }
#define _VECTORMATH_PERM_YZXW (vec_uchar16)(vec_uint4){ _VECTORMATH_PERM_Y, _VECTORMATH_PERM_Z, _VECTORMATH_PERM_X, _VECTORMATH_PERM_W }
#define _VECTORMATH_PERM_YZAB (vec_uchar16)(vec_uint4){ _VECTORMATH_PERM_Y, _VECTORMATH_PERM_Z, _VECTORMATH_PERM_A, _VECTORMATH_PERM_B }
#define _VECTORMATH_PERM_ZABC (vec_uchar16)(vec_uint4){ _VECTORMATH_PERM_Z, _VECTORMATH_PERM_A, _VECTORMATH_PERM_B, _VECTORMATH_PERM_C }
#define _VECTORMATH_PERM_XYAW (vec_uchar16)(vec_uint4){ _VECTORMATH_PERM_X, _VECTORMATH_PERM_Y, _VECTORMATH_PERM_A, _VECTORMATH_PERM_W }
#define _VECTORMATH_PERM_XAZW (vec_uchar16)(vec_uint4){ _VECTORMATH_PERM_X, _VECTORMATH_PERM_A, _VECTORMATH_PERM_Z, _VECTORMATH_PERM_W }
#define _VECTORMATH_MASK_0xF000 (vec_uint4){ 0xffffffff, 0, 0, 0 }
#define _VECTORMATH_MASK_0x0F00 (vec_uint4){ 0, 0xffffffff, 0, 0 }
#define _VECTORMATH_MASK_0x00F0 (vec_uint4){ 0, 0, 0xffffffff, 0 }
#define _VECTORMATH_MASK_0x000F (vec_uint4){ 0, 0, 0, 0xffffffff }
#define _VECTORMATH_UNIT_1000 (vec_float4){ 1.0f, 0.0f, 0.0f, 0.0f }
#define _VECTORMATH_UNIT_0100 (vec_float4){ 0.0f, 1.0f, 0.0f, 0.0f }
#define _VECTORMATH_UNIT_0010 (vec_float4){ 0.0f, 0.0f, 1.0f, 0.0f }
#define _VECTORMATH_UNIT_0001 (vec_float4){ 0.0f, 0.0f, 0.0f, 1.0f }
#define _VECTORMATH_SLERP_TOL 0.999f

//-----------------------------------------------------------------------------
// Definitions

#ifndef _VECTORMATH_INTERNAL_FUNCTIONS
#define _VECTORMATH_INTERNAL_FUNCTIONS

static inline vec_float4 _vmathVfDot3( vec_float4 vec0, vec_float4 vec1 )
{
    vec_float4 result;
    result = vec_madd( vec0, vec1, (vec_float4)(0.0f) );
    result = vec_madd( vec_sld( vec0, vec0, 4 ), vec_sld( vec1, vec1, 4 ), result );
    return vec_madd( vec_sld( vec0, vec0, 8 ), vec_sld( vec1, vec1, 8 ), result );
}

static inline vec_float4 _vmathVfDot4( vec_float4 vec0, vec_float4 vec1 )
{
    vec_float4 result;
    result = vec_madd( vec0, vec1, (vec_float4)(0.0f) );
    result = vec_madd( vec_sld( vec0, vec0, 4 ), vec_sld( vec1, vec1, 4 ), result );
    return vec_add( vec_sld( result, result, 8 ), result );
}

static inline vec_float4 _vmathVfCross( vec_float4 vec0, vec_float4 vec1 )
{
    vec_float4 tmp0, tmp1, tmp2, tmp3, result;
    tmp0 = vec_perm( vec0, vec0, _VECTORMATH_PERM_YZXW );
    tmp1 = vec_perm( vec1, vec1, _VECTORMATH_PERM_ZXYW );
    tmp2 = vec_perm( vec0, vec0, _VECTORMATH_PERM_ZXYW );
    tmp3 = vec_perm( vec1, vec1, _VECTORMATH_PERM_YZXW );
    result = vec_madd( tmp0, tmp1, (vec_float4)(0.0f) );
    result = vec_nmsub( tmp2, tmp3, result );
    return result;
}

static inline vec_uint4 _vmathVfToHalfFloatsUnpacked(vec_float4 v)
{
    vec_int4 bexp;
    vec_uint4 mant, sign, hfloat;
    vec_uint4 notZero, isInf;
    const vec_uint4 hfloatInf = (vec_uint4)(0x00007c00u);
    const vec_uint4 mergeMant = (vec_uint4)(0x000003ffu);
    const vec_uint4 mergeSign = (vec_uint4)(0x00008000u);

    sign = vec_sr((vec_uint4)v, (vec_uint4)(16));
    mant = vec_sr((vec_uint4)v, (vec_uint4)(13));
    bexp = vec_and(vec_sr((vec_int4)v, (vec_uint4)(23)), (vec_int4)(0xff));

    notZero = (vec_uint4)vec_cmpgt(bexp, (vec_int4)(112));
    isInf = (vec_uint4)vec_cmpgt(bexp, (vec_int4)(142));

    bexp = vec_add(bexp, (vec_int4)(-112));
    bexp = vec_sl(bexp, (vec_uint4)(10));

    hfloat = vec_sel((vec_uint4)bexp, mant, mergeMant);
    hfloat = vec_sel((vec_uint4)(0), hfloat, notZero);
    hfloat = vec_sel(hfloat, hfloatInf, isInf);
    hfloat = vec_sel(hfloat, sign, mergeSign);

    return hfloat;
}

static inline vec_ushort8 _vmath2VfToHalfFloats(vec_float4 u, vec_float4 v)
{
    vec_uint4 hfloat_u, hfloat_v;
    const vec_uchar16 pack = (vec_uchar16){2,3,6,7,10,11,14,15,18,19,22,23,26,27,30,31};
    hfloat_u = _vmathVfToHalfFloatsUnpacked(u);
    hfloat_v = _vmathVfToHalfFloatsUnpacked(v);
    return (vec_ushort8)vec_perm(hfloat_u, hfloat_v, pack);
}

#ifndef __GNUC__
#define __builtin_constant_p(x) 0
#endif

static inline vec_float4 _vmathVfInsert(vec_float4 dst, vec_float4 src, int slot)
{
#ifdef __GNUC__
    if (__builtin_constant_p(slot)) {
        dst = vec_sld(dst, dst, slot<<2);
        dst = vec_sld(dst, src, 4);
        if (slot != 3) dst = vec_sld(dst, dst, (3-slot)<<2);
        return dst;
    } else
#endif
    {
        vec_uchar16 shiftpattern = vec_lvsr( 0, (float *)(size_t)(slot<<2) );
        vec_uint4 selectmask = (vec_uint4)vec_perm( (vec_uint4)(0), _VECTORMATH_MASK_0xF000, shiftpattern );
        return vec_sel( dst, src, selectmask );
    }
}

#define _vmathVfGetElement(vec, slot) ((float *)&(vec))[slot]
#ifdef _VECTORMATH_SET_CONSTS_IN_MEM
#define _vmathVfSetElement(vec, scalar, slot) ((float *)&(vec))[slot] = scalar
#else
#define _vmathVfSetElement(vec, scalar, slot)                                            \
{                                                                                        \
    if (__builtin_constant_p(scalar)) {                                                  \
        (vec) = _vmathVfInsert(vec, (vec_float4){scalar, scalar, scalar, scalar}, slot); \
    } else {                                                                             \
        ((float *)&(vec))[slot] = scalar;                                                \
    }                                                                                    \
}
#endif

static inline vec_float4 _vmathVfSplatScalar(float scalar)
{
    vec_float4 result;
    if (__builtin_constant_p(scalar)) {
        result = (vec_float4){scalar, scalar, scalar, scalar};
    } else {
        result = vec_ld(0, &scalar);
        result = vec_splat(vec_perm(result, result, vec_lvsl(0, &scalar)), 0);
    }
    return result;
}

static inline vec_uint4 _vmathVuiSplatScalar(unsigned int scalar)
{
    vec_uint4 result;
    if (__builtin_constant_p(scalar)) {
        result = (vec_uint4){scalar, scalar, scalar, scalar};
    } else {
        result = vec_ld(0, &scalar);
        result = vec_splat(vec_perm(result, result, vec_lvsl(0, &scalar)), 0);
    }
    return result;
}

#endif

static inline vec_float4 _vmathHalfFloatsToVfUnpacked(vec_uint4 v)
{
    vec_int4 bexp;
    vec_uint4 mant, sign;
    vec_uint4 notZero, isInf, vfloat;
    const vec_uint4 mergeMant = (vec_uint4){0x007fffff,0x007fffff,0x007fffff,0x007fffff};
    const vec_uint4 vfloatInf = (vec_uint4){0x7f800000,0x7f800000,0x7f800000,0x7f800000};
    const vec_uint4 vhalfInf = (vec_uint4){0x0f800000,0x0f800000,0x0f800000,0x0f800000};
    const vec_uint4 mergeSign = (vec_uint4){0x80000000,0x80000000,0x80000000,0x80000000};
    const vec_int4 vfloatBase = (vec_int4){0x38000000,0x38000000,0x38000000,0x38000000}; // base = (127-15)<<23

    sign = vec_sl(v, (vec_uint4){16,16,16,16});
    mant = vec_sl(v, (vec_uint4){13,13,13,13});
    bexp = (vec_int4)vec_and(mant, vhalfInf);
    notZero = (vec_uint4)vec_cmpgt(bexp, (vec_int4){0, 0, 0, 0});
    isInf = (vec_uint4)vec_cmpeq(bexp, (vec_int4)vhalfInf);
    bexp = vec_add(bexp, vfloatBase);

    vfloat = vec_sel((vec_uint4)bexp, mant, mergeMant);
    vfloat = vec_sel((vec_uint4){0,0,0,0}, vfloat, notZero);
    vfloat = vec_sel(vfloat, vfloatInf, isInf);
    vfloat = vec_sel(vfloat, sign, mergeSign);

    return (vec_float4)vfloat;
}

static inline void _vmath2HalfFloatsToVfUnpacked(vec_float4 & vec0, vec_float4 & vec1, vec_uint4 vsrc)
{
    const vec_uchar16 pat0 = (vec_uchar16){0x00,0x00,0x00,0x01, 0x00,0x00,0x02,0x03,
                                           0x00,0x00,0x04,0x05, 0x00,0x00,0x06,0x07};
    const vec_uchar16 pat1 = (vec_uchar16){0x00,0x00,0x08,0x09, 0x00,0x00,0x0a,0x0b,
                                           0x00,0x00,0x0c,0x0d, 0x00,0x00,0x0e,0x0f};
    vec_int4 bexp0, bexp1;
    vec_uint4 mant0, mant1, sign0, sign1;
    vec_uint4 notZero0, notZero1, isInf0, isInf1, vfloat0, vfloat1;
    const vec_uint4 mergeMant = (vec_uint4){0x007fffff,0x007fffff,0x007fffff,0x007fffff};
    const vec_uint4 vfloatInf = (vec_uint4){0x7f800000,0x7f800000,0x7f800000,0x7f800000};
    const vec_uint4 vhalfInf = (vec_uint4){0x0f800000,0x0f800000,0x0f800000,0x0f800000};
    const vec_uint4 mergeSign = (vec_uint4){0x80000000,0x80000000,0x80000000,0x80000000};
    const vec_int4 vfloatBase = (vec_int4){0x38000000,0x38000000,0x38000000,0x38000000}; // base = (127-15)<<23

    vec_uint4 vsrc0 = vec_perm(vsrc, vsrc, pat0);
    vec_uint4 vsrc1 = vec_perm(vsrc, vsrc, pat1);
    sign0 = vec_sl(vsrc0, (vec_uint4){16,16,16,16});
    sign1 = vec_sl(vsrc1, (vec_uint4){16,16,16,16});
    mant0 = vec_sl(vsrc0, (vec_uint4){13,13,13,13});
    mant1 = vec_sl(vsrc1, (vec_uint4){13,13,13,13});
    bexp0 = (vec_int4)vec_and(mant0, vhalfInf);
    bexp1 = (vec_int4)vec_and(mant1, vhalfInf);
    notZero0 = (vec_uint4)vec_cmpgt(bexp0, (vec_int4){0, 0, 0, 0});
    notZero1 = (vec_uint4)vec_cmpgt(bexp1, (vec_int4){0, 0, 0, 0});
    isInf0 = (vec_uint4)vec_cmpeq(bexp0, (vec_int4)vhalfInf);
    isInf1 = (vec_uint4)vec_cmpeq(bexp1, (vec_int4)vhalfInf);
    bexp0 = vec_add(bexp0, vfloatBase);
    bexp1 = vec_add(bexp1, vfloatBase);

    vfloat0 = vec_sel((vec_uint4)bexp0, mant0, mergeMant);
    vfloat1 = vec_sel((vec_uint4)bexp1, mant1, mergeMant);
    vfloat0 = vec_sel((vec_uint4){0,0,0,0}, vfloat0, notZero0);
    vfloat1 = vec_sel((vec_uint4){0,0,0,0}, vfloat1, notZero1);
    vfloat0 = vec_sel(vfloat0, vfloatInf, isInf0);
    vfloat1 = vec_sel(vfloat1, vfloatInf, isInf1);
    vfloat0 = vec_sel(vfloat0, sign0, mergeSign);
    vfloat1 = vec_sel(vfloat1, sign1, mergeSign);

    vec0 = (vec_float4)vfloat0;
    vec1 = (vec_float4)vfloat1;
}

namespace Vectormath {
namespace Aos {

#ifdef _VECTORMATH_NO_SCALAR_CAST
inline VecIdx::operator floatInVec() const
{
    return floatInVec(ref, i);
}

inline float VecIdx::getAsFloat() const
#else
inline VecIdx::operator float() const
#endif
{
    return _vmathVfGetElement(ref, i);
}

inline float VecIdx::operator =( float scalar )
{
    _vmathVfSetElement(ref, scalar, i);
    return scalar;
}

inline floatInVec VecIdx::operator =( floatInVec scalar )
{
    ref = _vmathVfInsert(ref, scalar.get128(), i);
    return scalar;
}

inline floatInVec VecIdx::operator =( const VecIdx& scalar )
{
    return *this = floatInVec(scalar.ref, scalar.i);
}

inline floatInVec VecIdx::operator *=( float scalar )
{
    return *this *= floatInVec(scalar);
}

inline floatInVec VecIdx::operator *=( floatInVec scalar )
{
    return *this = floatInVec(ref, i) * scalar;
}

inline floatInVec VecIdx::operator /=( float scalar )
{
    return *this /= floatInVec(scalar);
}

inline floatInVec VecIdx::operator /=( floatInVec scalar )
{
    return *this = floatInVec(ref, i) / scalar;
}

inline floatInVec VecIdx::operator +=( float scalar )
{
    return *this += floatInVec(scalar);
}

inline floatInVec VecIdx::operator +=( floatInVec scalar )
{
    return *this = floatInVec(ref, i) + scalar;
}

inline floatInVec VecIdx::operator -=( float scalar )
{
    return *this -= floatInVec(scalar);
}

inline floatInVec VecIdx::operator -=( floatInVec scalar )
{
    return *this = floatInVec(ref, i) - scalar;
}

inline Vector3::Vector3( float _x, float _y, float _z )
{
    if (__builtin_constant_p(_x) & __builtin_constant_p(_y) & __builtin_constant_p(_z)) {
        mVec128 = (vec_float4){_x, _y, _z, 0.0f};
    } else {
        float *pf = (float *)&mVec128;
        pf[0] = _x;
        pf[1] = _y;
        pf[2] = _z;
    }
}

inline Vector3::Vector3( floatInVec _x, floatInVec _y, floatInVec _z )
{
    vec_float4 xz = vec_mergeh( _x.get128(), _z.get128() );
    mVec128 = vec_mergeh( xz, _y.get128() );
}

inline Vector3::Vector3( Point3 pnt )
{
    mVec128 = pnt.get128();
}

inline Vector3::Vector3( float scalar )
{
    mVec128 = floatInVec(scalar).get128();
}

inline Vector3::Vector3( floatInVec scalar )
{
    mVec128 = scalar.get128();
}

inline Vector3::Vector3( vec_float4 vf4 )
{
    mVec128 = vf4;
}

inline const Vector3 Vector3::xAxis( )
{
    return Vector3( _VECTORMATH_UNIT_1000 );
}

inline const Vector3 Vector3::yAxis( )
{
    return Vector3( _VECTORMATH_UNIT_0100 );
}

inline const Vector3 Vector3::zAxis( )
{
    return Vector3( _VECTORMATH_UNIT_0010 );
}

inline const Vector3 lerp( float t, Vector3 vec0, Vector3 vec1 )
{
    return lerp( floatInVec(t), vec0, vec1 );
}

inline const Vector3 lerp( floatInVec t, Vector3 vec0, Vector3 vec1 )
{
    return ( vec0 + ( ( vec1 - vec0 ) * t ) );
}

inline const Vector3 slerp( float t, Vector3 unitVec0, Vector3 unitVec1 )
{
    return slerp( floatInVec(t), unitVec0, unitVec1 );
}

inline const Vector3 slerp( floatInVec t, Vector3 unitVec0, Vector3 unitVec1 )
{
    vec_float4 scales, scale0, scale1, cosAngle, angle, tttt, oneMinusT, angles, sines;
    vec_uint4 selectMask;
    cosAngle = _vmathVfDot3( unitVec0.get128(), unitVec1.get128() );
    cosAngle = vec_splat( cosAngle, 0 );
    selectMask = (vec_uint4)vec_cmpgt( (vec_float4)(_VECTORMATH_SLERP_TOL), cosAngle );
    angle = acosf4( cosAngle );
    tttt = t.get128();
    oneMinusT = vec_sub( (vec_float4)(1.0f), tttt );
    angles = vec_mergeh( (vec_float4)(1.0f), tttt );
    angles = vec_mergeh( angles, oneMinusT );
    angles = vec_madd( angles, angle, (vec_float4)(0.0f) );
    sines = sinf4( angles );
    scales = divf4( sines, vec_splat( sines, 0 ) );
    scale0 = vec_sel( oneMinusT, vec_splat( scales, 1 ), selectMask );
    scale1 = vec_sel( tttt, vec_splat( scales, 2 ), selectMask );
    return Vector3( vec_madd( unitVec0.get128(), scale0, vec_madd( unitVec1.get128(), scale1, (vec_float4)(0.0f) ) ) );
}

inline vec_float4 Vector3::get128( ) const
{
    return mVec128;
}

inline void loadXYZ( Vector3 & vec, const vec_float4 * quad )
{
    vec = Vector3( *quad );
}

inline void loadXYZ( Vector3 & vec, const float * fptr )
{
    vec_float4 vec0 = vec_ld(0, fptr);
    vec_float4 vec1 = vec_ld(16, fptr);
    vec = Vector3( vec_perm(vec0, vec1, vec_lvsl(0, fptr)) );
}

inline void storeXYZ( Vector3 vec, vec_float4 * quad )
{
    vec_float4 dstVec = *quad;
    vec_uint4 mask = _VECTORMATH_MASK_0x000F;
    dstVec = vec_sel(vec.get128(), dstVec, mask);
    *quad = dstVec;
}

inline void storeXYZ( Vector3 vec, float * fptr )
{
    vec_float4 vsrc = vec.get128();
    vec_float4 x = vec_splat(vsrc, 0);
    vec_float4 y = vec_splat(vsrc, 1);
    vec_float4 z = vec_splat(vsrc, 2);
    vec_ste(x, 0, fptr);
    vec_ste(y, 4, fptr);
    vec_ste(z, 8, fptr);
}

inline void loadXYZArray( Vector3 & vec0, Vector3 & vec1, Vector3 & vec2, Vector3 & vec3, const vec_float4 * threeQuads )
{
    vec_float4 xyzx, yzxy, zxyz, xyz1, xyz2, xyz3;
    xyzx = threeQuads[0];
    yzxy = threeQuads[1];
    zxyz = threeQuads[2];
    xyz1 = vec_sld( xyzx, yzxy, 12 );
    xyz2 = vec_sld( yzxy, zxyz, 8 );
    xyz3 = vec_sld( zxyz, zxyz, 4 );
    vec0 = Vector3( xyzx );
    vec1 = Vector3( xyz1 );
    vec2 = Vector3( xyz2 );
    vec3 = Vector3( xyz3 );
}

inline void storeXYZArray( Vector3 vec0, Vector3 vec1, Vector3 vec2, Vector3 vec3, vec_float4 * threeQuads )
{
    vec_float4 xyzx, yzxy, zxyz;
    xyzx = vec_perm( vec0.get128(), vec1.get128(), _VECTORMATH_PERM_XYZA );
    yzxy = vec_perm( vec1.get128(), vec2.get128(), _VECTORMATH_PERM_YZAB );
    zxyz = vec_perm( vec2.get128(), vec3.get128(), _VECTORMATH_PERM_ZABC );
    threeQuads[0] = xyzx;
    threeQuads[1] = yzxy;
    threeQuads[2] = zxyz;
}

inline void loadHalfFloats( Vector3 & vec0, Vector3 & vec1, Vector3 & vec2, Vector3 & vec3, Vector3 & vec4, Vector3 & vec5, Vector3 & vec6, Vector3 & vec7, const vec_ushort8 * threeQuads )
{
    const vec_uint4 patWAB = (vec_uint4){_VECTORMATH_PERM_W,_VECTORMATH_PERM_A,_VECTORMATH_PERM_B,_VECTORMATH_PERM_C};
    const vec_uint4 patZWA = (vec_uint4){_VECTORMATH_PERM_Z,_VECTORMATH_PERM_W,_VECTORMATH_PERM_A,_VECTORMATH_PERM_B};
    const vec_uint4 patYZW = (vec_uint4){_VECTORMATH_PERM_Y,_VECTORMATH_PERM_Z,_VECTORMATH_PERM_W,_VECTORMATH_PERM_A};
    vec_float4 v0, v1, v2, v3, v4, v5;
    _vmath2HalfFloatsToVfUnpacked(v0, v1, (vec_uint4)threeQuads[0]);
    _vmath2HalfFloatsToVfUnpacked(v2, v3, (vec_uint4)threeQuads[1]);
    _vmath2HalfFloatsToVfUnpacked(v4, v5, (vec_uint4)threeQuads[2]);
    vec0 = Vector3(v0);
    vec1 = Vector3(vec_perm(v0, v1, (vec_uchar16)patWAB));
    vec2 = Vector3(vec_perm(v1, v2, (vec_uchar16)patZWA));
    vec3 = Vector3(vec_perm(v2, v2, (vec_uchar16)patYZW));
    vec4 = Vector3(v3);
    vec5 = Vector3(vec_perm(v3, v4, (vec_uchar16)patWAB));
    vec6 = Vector3(vec_perm(v4, v5, (vec_uchar16)patZWA));
    vec7 = Vector3(vec_perm(v5, v5, (vec_uchar16)patYZW));
}

inline void loadHalfFloats( Vector3 & vec, const uint16_t * hfptr )
{
    const vec_uchar16 pat[8] =
      {{0x00,0x00,0x00,0x01, 0x00,0x00,0x02,0x03, 0x00,0x00,0x04,0x05, 0x00,0x00,0x00,0x00},
       {0x00,0x00,0x02,0x03, 0x00,0x00,0x04,0x05, 0x00,0x00,0x06,0x07, 0x00,0x00,0x00,0x00},
       {0x00,0x00,0x04,0x05, 0x00,0x00,0x06,0x07, 0x00,0x00,0x08,0x09, 0x00,0x00,0x00,0x00},
       {0x00,0x00,0x06,0x07, 0x00,0x00,0x08,0x09, 0x00,0x00,0x0a,0x0b, 0x00,0x00,0x00,0x00},
       {0x00,0x00,0x08,0x09, 0x00,0x00,0x0a,0x0b, 0x00,0x00,0x0c,0x0d, 0x00,0x00,0x00,0x00},
       {0x00,0x00,0x0a,0x0b, 0x00,0x00,0x0c,0x0d, 0x00,0x00,0x0e,0x0f, 0x00,0x00,0x00,0x00},
       {0x00,0x00,0x0c,0x0d, 0x00,0x00,0x0e,0x0f, 0x00,0x00,0x10,0x11, 0x00,0x00,0x00,0x00},
       {0x00,0x00,0x0e,0x0f, 0x00,0x00,0x10,0x11, 0x00,0x00,0x12,0x13, 0x00,0x00,0x00,0x00}};
    const int idx = (((int)hfptr) & 0xf) >> 1;
    vec_ushort8 vdata0 = *(vec_ushort8*)((uintptr_t)hfptr);
    vec_ushort8 vdata1 = *(vec_ushort8*)(((uintptr_t)hfptr) + 16);
    vec_uint4 vsrc = (vec_uint4)vec_perm(vdata0, vdata1, pat[idx]);
    vec = Vector3(_vmathHalfFloatsToVfUnpacked(vsrc));
}

inline void storeHalfFloats( Vector3 vec0, Vector3 vec1, Vector3 vec2, Vector3 vec3, Vector3 vec4, Vector3 vec5, Vector3 vec6, Vector3 vec7, vec_ushort8 * threeQuads )
{
    vec_float4 xyz0[3];
    vec_float4 xyz1[3];
    storeXYZArray( vec0, vec1, vec2, vec3, xyz0 );
    storeXYZArray( vec4, vec5, vec6, vec7, xyz1 );
    threeQuads[0] = _vmath2VfToHalfFloats(xyz0[0], xyz0[1]);
    threeQuads[1] = _vmath2VfToHalfFloats(xyz0[2], xyz1[0]);
    threeQuads[2] = _vmath2VfToHalfFloats(xyz1[1], xyz1[2]);
}

inline void storeHalfFloats( Vector3 vec, uint16_t * hfptr )
{
    const vec_float4 vfval = vec.get128();
    const vec_ushort8 hfloat = (vec_ushort8)_vmathVfToHalfFloatsUnpacked(vfval);
    const vec_ushort8 x = vec_splat(hfloat, 1);
    const vec_ushort8 y = vec_splat(hfloat, 3);
    const vec_ushort8 z = vec_splat(hfloat, 5);
    vec_ste(x, 0, hfptr);
    vec_ste(y, 2, hfptr);
    vec_ste(z, 4, hfptr);
}

inline Vector3 & Vector3::operator =( Vector3 vec )
{
    mVec128 = vec.mVec128;
    return *this;
}

inline Vector3 & Vector3::setX( float _x )
{
    _vmathVfSetElement(mVec128, _x, 0);
    return *this;
}

inline Vector3 & Vector3::setX( floatInVec _x )
{
    mVec128 = _vmathVfInsert(mVec128, _x.get128(), 0);
    return *this;
}

inline const floatInVec Vector3::getX( ) const
{
    return floatInVec( mVec128, 0 );
}

inline Vector3 & Vector3::setY( float _y )
{
    _vmathVfSetElement(mVec128, _y, 1);
    return *this;
}

inline Vector3 & Vector3::setY( floatInVec _y )
{
    mVec128 = _vmathVfInsert(mVec128, _y.get128(), 1);
    return *this;
}

inline const floatInVec Vector3::getY( ) const
{
    return floatInVec( mVec128, 1 );
}

inline Vector3 & Vector3::setZ( float _z )
{
    _vmathVfSetElement(mVec128, _z, 2);
    return *this;
}

inline Vector3 & Vector3::setZ( floatInVec _z )
{
    mVec128 = _vmathVfInsert(mVec128, _z.get128(), 2);
    return *this;
}

inline const floatInVec Vector3::getZ( ) const
{
    return floatInVec( mVec128, 2 );
}

inline Vector3 & Vector3::setElem( int idx, float value )
{
    _vmathVfSetElement(mVec128, value, idx);
    return *this;
}

inline Vector3 & Vector3::setElem( int idx, floatInVec value )
{
    mVec128 = _vmathVfInsert(mVec128, value.get128(), idx);
    return *this;
}

inline const floatInVec Vector3::getElem( int idx ) const
{
    return floatInVec( mVec128, idx );
}

inline VecIdx Vector3::operator []( int idx )
{
    return VecIdx( mVec128, idx );
}

inline const floatInVec Vector3::operator []( int idx ) const
{
    return floatInVec( mVec128, idx );
}

inline const Vector3 Vector3::operator +( Vector3 vec ) const
{
    return Vector3( vec_add( mVec128, vec.mVec128 ) );
}

inline const Vector3 Vector3::operator -( Vector3 vec ) const
{
    return Vector3( vec_sub( mVec128, vec.mVec128 ) );
}

inline const Point3 Vector3::operator +( Point3 pnt ) const
{
    return Point3( vec_add( mVec128, pnt.get128() ) );
}

inline const Vector3 Vector3::operator *( float scalar ) const
{
    return *this * floatInVec(scalar);
}

inline const Vector3 Vector3::operator *( floatInVec scalar ) const
{
    return Vector3( vec_madd( mVec128, scalar.get128(), (vec_float4)(0.0f) ) );
}

inline Vector3 & Vector3::operator +=( Vector3 vec )
{
    *this = *this + vec;
    return *this;
}

inline Vector3 & Vector3::operator -=( Vector3 vec )
{
    *this = *this - vec;
    return *this;
}

inline Vector3 & Vector3::operator *=( float scalar )
{
    *this = *this * scalar;
    return *this;
}

inline Vector3 & Vector3::operator *=( floatInVec scalar )
{
    *this = *this * scalar;
    return *this;
}

inline const Vector3 Vector3::operator /( float scalar ) const
{
    return *this / floatInVec(scalar);
}

inline const Vector3 Vector3::operator /( floatInVec scalar ) const
{
    return Vector3( divf4( mVec128, scalar.get128() ) );
}

inline Vector3 & Vector3::operator /=( float scalar )
{
    *this = *this / scalar;
    return *this;
}

inline Vector3 & Vector3::operator /=( floatInVec scalar )
{
    *this = *this / scalar;
    return *this;
}

inline const Vector3 Vector3::operator -( ) const
{
    return Vector3( negatef4( mVec128 ) );
}

inline const Vector3 operator *( float scalar, Vector3 vec )
{
    return floatInVec(scalar) * vec;
}

inline const Vector3 operator *( floatInVec scalar, Vector3 vec )
{
    return vec * scalar;
}

inline const Vector3 mulPerElem( Vector3 vec0, Vector3 vec1 )
{
    return Vector3( vec_madd( vec0.get128(), vec1.get128(), (vec_float4)(0.0f) ) );
}

inline const Vector3 divPerElem( Vector3 vec0, Vector3 vec1 )
{
    return Vector3( divf4( vec0.get128(), vec1.get128() ) );
}

inline const Vector3 recipPerElem( Vector3 vec )
{
    return Vector3( recipf4( vec.get128() ) );
}

inline const Vector3 sqrtPerElem( Vector3 vec )
{
    return Vector3( sqrtf4( vec.get128() ) );
}

inline const Vector3 rsqrtPerElem( Vector3 vec )
{
    return Vector3( rsqrtf4( vec.get128() ) );
}

inline const Vector3 absPerElem( Vector3 vec )
{
    return Vector3( fabsf4( vec.get128() ) );
}

inline const Vector3 copySignPerElem( Vector3 vec0, Vector3 vec1 )
{
    return Vector3( copysignf4( vec0.get128(), vec1.get128() ) );
}

inline const Vector3 maxPerElem( Vector3 vec0, Vector3 vec1 )
{
    return Vector3( fmaxf4( vec0.get128(), vec1.get128() ) );
}

inline const floatInVec maxElem( Vector3 vec )
{
    vec_float4 result;
    result = fmaxf4( vec_splat( vec.get128(), 1 ), vec.get128() );
    result = fmaxf4( vec_splat( vec.get128(), 2 ), result );
    return floatInVec( result, 0 );
}

inline const Vector3 minPerElem( Vector3 vec0, Vector3 vec1 )
{
    return Vector3( fminf4( vec0.get128(), vec1.get128() ) );
}

inline const floatInVec minElem( Vector3 vec )
{
    vec_float4 result;
    result = fminf4( vec_splat( vec.get128(), 1 ), vec.get128() );
    result = fminf4( vec_splat( vec.get128(), 2 ), result );
    return floatInVec( result, 0 );
}

inline const floatInVec sum( Vector3 vec )
{
    vec_float4 result;
    result = vec_add( vec_splat( vec.get128(), 1 ), vec.get128() );
    result = vec_add( vec_splat( vec.get128(), 2 ), result );
    return floatInVec( result, 0 );
}

inline const floatInVec dot( Vector3 vec0, Vector3 vec1 )
{
    return floatInVec( _vmathVfDot3( vec0.get128(), vec1.get128() ), 0 );
}

inline const floatInVec lengthSqr( Vector3 vec )
{
    return floatInVec(  _vmathVfDot3( vec.get128(), vec.get128() ), 0 );
}

inline const floatInVec length( Vector3 vec )
{
    return floatInVec(  sqrtf4(_vmathVfDot3( vec.get128(), vec.get128() )), 0 );
}

inline const Vector3 normalize( Vector3 vec )
{
    vec_float4 dot = _vmathVfDot3( vec.get128(), vec.get128() );
    dot = vec_splat( dot, 0 );
    return Vector3( vec_madd( vec.get128(), rsqrtf4( dot ), (vec_float4)(0.0f) ) );
}

inline const Vector3 cross( Vector3 vec0, Vector3 vec1 )
{
    return Vector3( _vmathVfCross( vec0.get128(), vec1.get128() ) );
}

inline const Vector3 select( Vector3 vec0, Vector3 vec1, bool select1 )
{
    return select( vec0, vec1, boolInVec(select1) );
}

inline const Vector3 select( Vector3 vec0, Vector3 vec1, boolInVec select1 )
{
    return Vector3( vec_sel( vec0.get128(), vec1.get128(), select1.get128() ) );
}

#ifdef _VECTORMATH_DEBUG

inline void print( Vector3 vec )
{
    union { vec_float4 v; float s[4]; } tmp;
    tmp.v = vec.get128();
    printf( "( %f %f %f )\n", tmp.s[0], tmp.s[1], tmp.s[2] );
}

inline void print( Vector3 vec, const char * name )
{
    union { vec_float4 v; float s[4]; } tmp;
    tmp.v = vec.get128();
    printf( "%s: ( %f %f %f )\n", name, tmp.s[0], tmp.s[1], tmp.s[2] );
}

#endif

inline Vector4::Vector4( float _x, float _y, float _z, float _w )
{
    if (__builtin_constant_p(_x) & __builtin_constant_p(_y) &
        __builtin_constant_p(_z) & __builtin_constant_p(_w)) {
        mVec128 = (vec_float4){_x, _y, _z, _w};
    } else {
        float *pf = (float *)&mVec128;
        pf[0] = _x;
        pf[1] = _y;
        pf[2] = _z;
        pf[3] = _w;
    }
}

inline Vector4::Vector4( floatInVec _x, floatInVec _y, floatInVec _z, floatInVec _w )
{
    vec_float4 xz = vec_mergeh( _x.get128(), _z.get128() );
    vec_float4 yw = vec_mergeh( _y.get128(), _w.get128() );
    mVec128 = vec_mergeh( xz, yw );
}

inline Vector4::Vector4( Vector3 xyz, float _w )
{
    mVec128 = xyz.get128();
    _vmathVfSetElement(mVec128, _w, 3);
}

inline Vector4::Vector4( Vector3 xyz, floatInVec _w )
{
    mVec128 = xyz.get128();
    mVec128 = _vmathVfInsert(mVec128, _w.get128(), 3);
}

inline Vector4::Vector4( Vector3 vec )
{
    mVec128 = vec.get128();
    mVec128 = _vmathVfInsert(mVec128, (vec_float4)(0.0f), 3);
}

inline Vector4::Vector4( Point3 pnt )
{
    mVec128 = pnt.get128();
    mVec128 = _vmathVfInsert(mVec128, (vec_float4)(1.0f), 3);
}

inline Vector4::Vector4( Quat quat )
{
    mVec128 = quat.get128();
}

inline Vector4::Vector4( float scalar )
{
    mVec128 = floatInVec(scalar).get128();
}

inline Vector4::Vector4( floatInVec scalar )
{
    mVec128 = scalar.get128();
}

inline Vector4::Vector4( vec_float4 vf4 )
{
    mVec128 = vf4;
}

inline const Vector4 Vector4::xAxis( )
{
    return Vector4( _VECTORMATH_UNIT_1000 );
}

inline const Vector4 Vector4::yAxis( )
{
    return Vector4( _VECTORMATH_UNIT_0100 );
}

inline const Vector4 Vector4::zAxis( )
{
    return Vector4( _VECTORMATH_UNIT_0010 );
}

inline const Vector4 Vector4::wAxis( )
{
    return Vector4( _VECTORMATH_UNIT_0001 );
}

inline const Vector4 lerp( float t, Vector4 vec0, Vector4 vec1 )
{
    return lerp( floatInVec(t), vec0, vec1 );
}

inline const Vector4 lerp( floatInVec t, Vector4 vec0, Vector4 vec1 )
{
    return ( vec0 + ( ( vec1 - vec0 ) * t ) );
}

inline const Vector4 slerp( float t, Vector4 unitVec0, Vector4 unitVec1 )
{
    return slerp( floatInVec(t), unitVec0, unitVec1 );
}

inline const Vector4 slerp( floatInVec t, Vector4 unitVec0, Vector4 unitVec1 )
{
    vec_float4 scales, scale0, scale1, cosAngle, angle, tttt, oneMinusT, angles, sines;
    vec_uint4 selectMask;
    cosAngle = _vmathVfDot4( unitVec0.get128(), unitVec1.get128() );
    cosAngle = vec_splat( cosAngle, 0 );
    selectMask = (vec_uint4)vec_cmpgt( (vec_float4)(_VECTORMATH_SLERP_TOL), cosAngle );
    angle = acosf4( cosAngle );
    tttt = t.get128();
    oneMinusT = vec_sub( (vec_float4)(1.0f), tttt );
    angles = vec_mergeh( (vec_float4)(1.0f), tttt );
    angles = vec_mergeh( angles, oneMinusT );
    angles = vec_madd( angles, angle, (vec_float4)(0.0f) );
    sines = sinf4( angles );
    scales = divf4( sines, vec_splat( sines, 0 ) );
    scale0 = vec_sel( oneMinusT, vec_splat( scales, 1 ), selectMask );
    scale1 = vec_sel( tttt, vec_splat( scales, 2 ), selectMask );
    return Vector4( vec_madd( unitVec0.get128(), scale0, vec_madd( unitVec1.get128(), scale1, (vec_float4)(0.0f) ) ) );
}

inline vec_float4 Vector4::get128( ) const
{
    return mVec128;
}

inline void loadXYZW( Vector4 & vec, const vec_float4 * quad )
{
    vec = Vector4( *quad );
}

inline void loadXYZW( Vector4 & vec, const float * fptr )
{
    vec_float4 vec0 = vec_ld(0, fptr);
    vec_float4 vec1 = vec_ld(16, fptr);
    vec = Vector4( vec_perm(vec0, vec1, vec_lvsl(0, fptr)) );
}

inline void storeXYZW( Vector4 vec, float * fptr )
{
    vec_float4 vsrc = vec.get128();
    vec_float4 x = vec_splat(vsrc, 0);
    vec_float4 y = vec_splat(vsrc, 1);
    vec_float4 z = vec_splat(vsrc, 2);
    vec_float4 w = vec_splat(vsrc, 3);
    vec_ste(x, 0, fptr);
    vec_ste(y, 4, fptr);
    vec_ste(z, 8, fptr);
    vec_ste(w, 12, fptr);
}

inline void loadHalfFloats( Vector4 & vec0, Vector4 & vec1, Vector4 & vec2, Vector4 & vec3, const vec_ushort8 * twoQuads )
{
    vec_float4 v0,v1,v2,v3;
    _vmath2HalfFloatsToVfUnpacked(v0, v1, (vec_uint4)twoQuads[0]);
    _vmath2HalfFloatsToVfUnpacked(v2, v3, (vec_uint4)twoQuads[1]);
    vec0 = Vector4(v0);
    vec1 = Vector4(v1);
    vec2 = Vector4(v2);
    vec3 = Vector4(v3);
}

inline void loadHalfFloats( Vector4 & vec, const uint16_t * hfptr )
{
    const vec_uchar16 pat[8] =
      {{0x00,0x00,0x00,0x01, 0x00,0x00,0x02,0x03, 0x00,0x00,0x04,0x05, 0x00,0x00,0x06,0x07},
       {0x00,0x00,0x02,0x03, 0x00,0x00,0x04,0x05, 0x00,0x00,0x06,0x07, 0x00,0x00,0x08,0x09},
       {0x00,0x00,0x04,0x05, 0x00,0x00,0x06,0x07, 0x00,0x00,0x08,0x09, 0x00,0x00,0x0a,0x0b},
       {0x00,0x00,0x06,0x07, 0x00,0x00,0x08,0x09, 0x00,0x00,0x0a,0x0b, 0x00,0x00,0x0c,0x0d},
       {0x00,0x00,0x08,0x09, 0x00,0x00,0x0a,0x0b, 0x00,0x00,0x0c,0x0d, 0x00,0x00,0x0e,0x0f},
       {0x00,0x00,0x0a,0x0b, 0x00,0x00,0x0c,0x0d, 0x00,0x00,0x0e,0x0f, 0x00,0x00,0x10,0x11},
       {0x00,0x00,0x0c,0x0d, 0x00,0x00,0x0e,0x0f, 0x00,0x00,0x10,0x11, 0x00,0x00,0x12,0x13},
       {0x00,0x00,0x0e,0x0f, 0x00,0x00,0x10,0x11, 0x00,0x00,0x12,0x13, 0x00,0x00,0x14,0x15}};
    const int idx = (((int)hfptr) & 0xf) >> 1;
    vec_ushort8 vdata0 = *(vec_ushort8*)((uintptr_t)hfptr);
    vec_ushort8 vdata1 = *(vec_ushort8*)(((uintptr_t)hfptr) + 16);
    vec_uint4 vsrc = (vec_uint4)vec_perm(vdata0, vdata1, pat[idx]);
    vec = Vector4(_vmathHalfFloatsToVfUnpacked(vsrc));
}

inline void storeHalfFloats( Vector4 vec0, Vector4 vec1, Vector4 vec2, Vector4 vec3, vec_ushort8 * twoQuads )
{
    twoQuads[0] = _vmath2VfToHalfFloats(vec0.get128(), vec1.get128());
    twoQuads[1] = _vmath2VfToHalfFloats(vec2.get128(), vec3.get128());
}

inline void storeHalfFloats( Vector4 vec, uint16_t * hfptr )
{
    const vec_float4 vfval = vec.get128();
    const vec_ushort8 hfloat = (vec_ushort8)_vmathVfToHalfFloatsUnpacked(vfval);
    const vec_ushort8 x = vec_splat(hfloat, 1);
    const vec_ushort8 y = vec_splat(hfloat, 3);
    const vec_ushort8 z = vec_splat(hfloat, 5);
    const vec_ushort8 w = vec_splat(hfloat, 7);
    vec_ste(x, 0, hfptr);
    vec_ste(y, 2, hfptr);
    vec_ste(z, 4, hfptr);
    vec_ste(w, 6, hfptr);
}

inline Vector4 & Vector4::operator =( Vector4 vec )
{
    mVec128 = vec.mVec128;
    return *this;
}

inline Vector4 & Vector4::setXYZ( Vector3 vec )
{
    mVec128 = vec_sel( vec.get128(), mVec128, _VECTORMATH_MASK_0x000F );
    return *this;
}

inline const Vector3 Vector4::getXYZ( ) const
{
    return Vector3( mVec128 );
}

inline Vector4 & Vector4::setX( float _x )
{
    _vmathVfSetElement(mVec128, _x, 0);
    return *this;
}

inline Vector4 & Vector4::setX( floatInVec _x )
{
    mVec128 = _vmathVfInsert(mVec128, _x.get128(), 0);
    return *this;
}

inline const floatInVec Vector4::getX( ) const
{
    return floatInVec( mVec128, 0 );
}

inline Vector4 & Vector4::setY( float _y )
{
    _vmathVfSetElement(mVec128, _y, 1);
    return *this;
}

inline Vector4 & Vector4::setY( floatInVec _y )
{
    mVec128 = _vmathVfInsert(mVec128, _y.get128(), 1);
    return *this;
}

inline const floatInVec Vector4::getY( ) const
{
    return floatInVec( mVec128, 1 );
}

inline Vector4 & Vector4::setZ( float _z )
{
    _vmathVfSetElement(mVec128, _z, 2);
    return *this;
}

inline Vector4 & Vector4::setZ( floatInVec _z )
{
    mVec128 = _vmathVfInsert(mVec128, _z.get128(), 2);
    return *this;
}

inline const floatInVec Vector4::getZ( ) const
{
    return floatInVec( mVec128, 2 );
}

inline Vector4 & Vector4::setW( float _w )
{
    _vmathVfSetElement(mVec128, _w, 3);
    return *this;
}

inline Vector4 & Vector4::setW( floatInVec _w )
{
    mVec128 = _vmathVfInsert(mVec128, _w.get128(), 3);
    return *this;
}

inline const floatInVec Vector4::getW( ) const
{
    return floatInVec( mVec128, 3 );
}

inline Vector4 & Vector4::setElem( int idx, float value )
{
    _vmathVfSetElement(mVec128, value, idx);
    return *this;
}

inline Vector4 & Vector4::setElem( int idx, floatInVec value )
{
    mVec128 = _vmathVfInsert(mVec128, value.get128(), idx);
    return *this;
}

inline const floatInVec Vector4::getElem( int idx ) const
{
    return floatInVec( mVec128, idx );
}

inline VecIdx Vector4::operator []( int idx )
{
    return VecIdx( mVec128, idx );
}

inline const floatInVec Vector4::operator []( int idx ) const
{
    return floatInVec( mVec128, idx );
}

inline const Vector4 Vector4::operator +( Vector4 vec ) const
{
    return Vector4( vec_add( mVec128, vec.mVec128 ) );
}

inline const Vector4 Vector4::operator -( Vector4 vec ) const
{
    return Vector4( vec_sub( mVec128, vec.mVec128 ) );
}

inline const Vector4 Vector4::operator *( float scalar ) const
{
    return *this * floatInVec(scalar);
}

inline const Vector4 Vector4::operator *( floatInVec scalar ) const
{
    return Vector4( vec_madd( mVec128, scalar.get128(), (vec_float4)(0.0f) ) );
}

inline Vector4 & Vector4::operator +=( Vector4 vec )
{
    *this = *this + vec;
    return *this;
}

inline Vector4 & Vector4::operator -=( Vector4 vec )
{
    *this = *this - vec;
    return *this;
}

inline Vector4 & Vector4::operator *=( float scalar )
{
    *this = *this * scalar;
    return *this;
}

inline Vector4 & Vector4::operator *=( floatInVec scalar )
{
    *this = *this * scalar;
    return *this;
}

inline const Vector4 Vector4::operator /( float scalar ) const
{
    return *this / floatInVec(scalar);
}

inline const Vector4 Vector4::operator /( floatInVec scalar ) const
{
    return Vector4( divf4( mVec128, scalar.get128() ) );
}

inline Vector4 & Vector4::operator /=( float scalar )
{
    *this = *this / scalar;
    return *this;
}

inline Vector4 & Vector4::operator /=( floatInVec scalar )
{
    *this = *this / scalar;
    return *this;
}

inline const Vector4 Vector4::operator -( ) const
{
    return Vector4( negatef4( mVec128 ) );
}

inline const Vector4 operator *( float scalar, Vector4 vec )
{
    return floatInVec(scalar) * vec;
}

inline const Vector4 operator *( floatInVec scalar, Vector4 vec )
{
    return vec * scalar;
}

inline const Vector4 mulPerElem( Vector4 vec0, Vector4 vec1 )
{
    return Vector4( vec_madd( vec0.get128(), vec1.get128(), (vec_float4)(0.0f) ) );
}

inline const Vector4 divPerElem( Vector4 vec0, Vector4 vec1 )
{
    return Vector4( divf4( vec0.get128(), vec1.get128() ) );
}

inline const Vector4 recipPerElem( Vector4 vec )
{
    return Vector4( recipf4( vec.get128() ) );
}

inline const Vector4 sqrtPerElem( Vector4 vec )
{
    return Vector4( sqrtf4( vec.get128() ) );
}

inline const Vector4 rsqrtPerElem( Vector4 vec )
{
    return Vector4( rsqrtf4( vec.get128() ) );
}

inline const Vector4 absPerElem( Vector4 vec )
{
    return Vector4( fabsf4( vec.get128() ) );
}

inline const Vector4 copySignPerElem( Vector4 vec0, Vector4 vec1 )
{
    return Vector4( copysignf4( vec0.get128(), vec1.get128() ) );
}

inline const Vector4 maxPerElem( Vector4 vec0, Vector4 vec1 )
{
    return Vector4( fmaxf4( vec0.get128(), vec1.get128() ) );
}

inline const floatInVec maxElem( Vector4 vec )
{
    vec_float4 result;
    result = fmaxf4( vec_splat( vec.get128(), 1 ), vec.get128() );
    result = fmaxf4( vec_splat( vec.get128(), 2 ), result );
    result = fmaxf4( vec_splat( vec.get128(), 3 ), result );
    return floatInVec( result, 0 );
}

inline const Vector4 minPerElem( Vector4 vec0, Vector4 vec1 )
{
    return Vector4( fminf4( vec0.get128(), vec1.get128() ) );
}

inline const floatInVec minElem( Vector4 vec )
{
    vec_float4 result;
    result = fminf4( vec_splat( vec.get128(), 1 ), vec.get128() );
    result = fminf4( vec_splat( vec.get128(), 2 ), result );
    result = fminf4( vec_splat( vec.get128(), 3 ), result );
    return floatInVec( result, 0 );
}

inline const floatInVec sum( Vector4 vec )
{
    vec_float4 result;
    result = vec_add( vec_splat( vec.get128(), 1 ), vec.get128() );
    result = vec_add( vec_splat( vec.get128(), 2 ), result );
    result = vec_add( vec_splat( vec.get128(), 3 ), result );
    return floatInVec( result, 0 );
}

inline const floatInVec dot( Vector4 vec0, Vector4 vec1 )
{
    return floatInVec( _vmathVfDot4( vec0.get128(), vec1.get128() ), 0 );
}

inline const floatInVec lengthSqr( Vector4 vec )
{
    return floatInVec(  _vmathVfDot4( vec.get128(), vec.get128() ), 0 );
}

inline const floatInVec length( Vector4 vec )
{
    return floatInVec(  sqrtf4(_vmathVfDot4( vec.get128(), vec.get128() )), 0 );
}

inline const Vector4 normalize( Vector4 vec )
{
    vec_float4 dot = _vmathVfDot4( vec.get128(), vec.get128() );
    return Vector4( vec_madd( vec.get128(), rsqrtf4( dot ), (vec_float4)(0.0f) ) );
}

inline const Vector4 select( Vector4 vec0, Vector4 vec1, bool select1 )
{
    return select( vec0, vec1, boolInVec(select1) );
}

inline const Vector4 select( Vector4 vec0, Vector4 vec1, boolInVec select1 )
{
    return Vector4( vec_sel( vec0.get128(), vec1.get128(), select1.get128() ) );
}

#ifdef _VECTORMATH_DEBUG

inline void print( Vector4 vec )
{
    union { vec_float4 v; float s[4]; } tmp;
    tmp.v = vec.get128();
    printf( "( %f %f %f %f )\n", tmp.s[0], tmp.s[1], tmp.s[2], tmp.s[3] );
}

inline void print( Vector4 vec, const char * name )
{
    union { vec_float4 v; float s[4]; } tmp;
    tmp.v = vec.get128();
    printf( "%s: ( %f %f %f %f )\n", name, tmp.s[0], tmp.s[1], tmp.s[2], tmp.s[3] );
}

#endif

inline Point3::Point3( float _x, float _y, float _z )
{
    if (__builtin_constant_p(_x) & __builtin_constant_p(_y) & __builtin_constant_p(_z)) {
        mVec128 = (vec_float4){_x, _y, _z, 0.0f};
    } else {
        float *pf = (float *)&mVec128;
        pf[0] = _x;
        pf[1] = _y;
        pf[2] = _z;
    }
}

inline Point3::Point3( floatInVec _x, floatInVec _y, floatInVec _z )
{
    vec_float4 xz = vec_mergeh( _x.get128(), _z.get128() );
    mVec128 = vec_mergeh( xz, _y.get128() );
}

inline Point3::Point3( Vector3 vec )
{
    mVec128 = vec.get128();
}

inline Point3::Point3( float scalar )
{
    mVec128 = floatInVec(scalar).get128();
}

inline Point3::Point3( floatInVec scalar )
{
    mVec128 = scalar.get128();
}

inline Point3::Point3( vec_float4 vf4 )
{
    mVec128 = vf4;
}

inline const Point3 lerp( float t, Point3 pnt0, Point3 pnt1 )
{
    return lerp( floatInVec(t), pnt0, pnt1 );
}

inline const Point3 lerp( floatInVec t, Point3 pnt0, Point3 pnt1 )
{
    return ( pnt0 + ( ( pnt1 - pnt0 ) * t ) );
}

inline vec_float4 Point3::get128( ) const
{
    return mVec128;
}

inline void loadXYZ( Point3 & pnt, const vec_float4 * quad )
{
    pnt = Point3( *quad );
}

inline void loadXYZ( Point3 & pnt, const float * fptr )
{
    vec_float4 vec0 = vec_ld(0, fptr);
    vec_float4 vec1 = vec_ld(16, fptr);
    pnt = Point3( vec_perm(vec0, vec1, vec_lvsl(0, fptr)) );
}

inline void storeXYZ( Point3 pnt, vec_float4 * quad )
{
    vec_float4 dstVec = *quad;
    vec_uint4 mask = _VECTORMATH_MASK_0x000F;
    dstVec = vec_sel(pnt.get128(), dstVec, mask);
    *quad = dstVec;
}

inline void storeXYZ( Point3 pnt, float * fptr )
{
    vec_float4 vsrc = pnt.get128();
    vec_float4 x = vec_splat(vsrc, 0);
    vec_float4 y = vec_splat(vsrc, 1);
    vec_float4 z = vec_splat(vsrc, 2);
    vec_ste(x, 0, fptr);
    vec_ste(y, 4, fptr);
    vec_ste(z, 8, fptr);
}

inline void loadXYZArray( Point3 & pnt0, Point3 & pnt1, Point3 & pnt2, Point3 & pnt3, const vec_float4 * threeQuads )
{
    vec_float4 xyzx, yzxy, zxyz, xyz1, xyz2, xyz3;
    xyzx = threeQuads[0];
    yzxy = threeQuads[1];
    zxyz = threeQuads[2];
    xyz1 = vec_sld( xyzx, yzxy, 12 );
    xyz2 = vec_sld( yzxy, zxyz, 8 );
    xyz3 = vec_sld( zxyz, zxyz, 4 );
    pnt0 = Point3( xyzx );
    pnt1 = Point3( xyz1 );
    pnt2 = Point3( xyz2 );
    pnt3 = Point3( xyz3 );
}

inline void storeXYZArray( Point3 pnt0, Point3 pnt1, Point3 pnt2, Point3 pnt3, vec_float4 * threeQuads )
{
    vec_float4 xyzx, yzxy, zxyz;
    xyzx = vec_perm( pnt0.get128(), pnt1.get128(), _VECTORMATH_PERM_XYZA );
    yzxy = vec_perm( pnt1.get128(), pnt2.get128(), _VECTORMATH_PERM_YZAB );
    zxyz = vec_perm( pnt2.get128(), pnt3.get128(), _VECTORMATH_PERM_ZABC );
    threeQuads[0] = xyzx;
    threeQuads[1] = yzxy;
    threeQuads[2] = zxyz;
}

inline void loadHalfFloats( Point3 & vec0, Point3 & vec1, Point3 & vec2, Point3 & vec3, Point3 & vec4, Point3 & vec5, Point3 & vec6, Point3 & vec7, const vec_ushort8 * threeQuads )
{
    const vec_uint4 patWAB = (vec_uint4){_VECTORMATH_PERM_W,_VECTORMATH_PERM_A,_VECTORMATH_PERM_B,_VECTORMATH_PERM_C};
    const vec_uint4 patZWA = (vec_uint4){_VECTORMATH_PERM_Z,_VECTORMATH_PERM_W,_VECTORMATH_PERM_A,_VECTORMATH_PERM_B};
    const vec_uint4 patYZW = (vec_uint4){_VECTORMATH_PERM_Y,_VECTORMATH_PERM_Z,_VECTORMATH_PERM_W,_VECTORMATH_PERM_A};
    vec_float4 v0, v1, v2, v3, v4, v5;
    _vmath2HalfFloatsToVfUnpacked(v0, v1, (vec_uint4)threeQuads[0]);
    _vmath2HalfFloatsToVfUnpacked(v2, v3, (vec_uint4)threeQuads[1]);
    _vmath2HalfFloatsToVfUnpacked(v4, v5, (vec_uint4)threeQuads[2]);
    vec0 = Point3(v0);
    vec1 = Point3(vec_perm(v0, v1, (vec_uchar16)patWAB));
    vec2 = Point3(vec_perm(v1, v2, (vec_uchar16)patZWA));
    vec3 = Point3(vec_perm(v2, v2, (vec_uchar16)patYZW));
    vec4 = Point3(v3);
    vec5 = Point3(vec_perm(v3, v4, (vec_uchar16)patWAB));
    vec6 = Point3(vec_perm(v4, v5, (vec_uchar16)patZWA));
    vec7 = Point3(vec_perm(v5, v5, (vec_uchar16)patYZW));
}

inline void loadHalfFloats( Point3 & vec, const uint16_t * hfptr )
{
    const vec_uchar16 pat[8] =
      {{0x00,0x00,0x00,0x01, 0x00,0x00,0x02,0x03, 0x00,0x00,0x04,0x05, 0x00,0x00,0x00,0x00},
       {0x00,0x00,0x02,0x03, 0x00,0x00,0x04,0x05, 0x00,0x00,0x06,0x07, 0x00,0x00,0x00,0x00},
       {0x00,0x00,0x04,0x05, 0x00,0x00,0x06,0x07, 0x00,0x00,0x08,0x09, 0x00,0x00,0x00,0x00},
       {0x00,0x00,0x06,0x07, 0x00,0x00,0x08,0x09, 0x00,0x00,0x0a,0x0b, 0x00,0x00,0x00,0x00},
       {0x00,0x00,0x08,0x09, 0x00,0x00,0x0a,0x0b, 0x00,0x00,0x0c,0x0d, 0x00,0x00,0x00,0x00},
       {0x00,0x00,0x0a,0x0b, 0x00,0x00,0x0c,0x0d, 0x00,0x00,0x0e,0x0f, 0x00,0x00,0x00,0x00},
       {0x00,0x00,0x0c,0x0d, 0x00,0x00,0x0e,0x0f, 0x00,0x00,0x10,0x11, 0x00,0x00,0x00,0x00},
       {0x00,0x00,0x0e,0x0f, 0x00,0x00,0x10,0x11, 0x00,0x00,0x12,0x13, 0x00,0x00,0x00,0x00}};
    const int idx = (((int)hfptr) & 0xf) >> 1;
    vec_ushort8 vdata0 = *(vec_ushort8*)((uintptr_t)hfptr);
    vec_ushort8 vdata1 = *(vec_ushort8*)(((uintptr_t)hfptr) + 16);
    vec_uint4 vsrc = (vec_uint4)vec_perm(vdata0, vdata1, pat[idx]);
    vec = Point3(_vmathHalfFloatsToVfUnpacked(vsrc));
}

inline void storeHalfFloats( Point3 pnt0, Point3 pnt1, Point3 pnt2, Point3 pnt3, Point3 pnt4, Point3 pnt5, Point3 pnt6, Point3 pnt7, vec_ushort8 * threeQuads )
{
    vec_float4 xyz0[3];
    vec_float4 xyz1[3];
    storeXYZArray( pnt0, pnt1, pnt2, pnt3, xyz0 );
    storeXYZArray( pnt4, pnt5, pnt6, pnt7, xyz1 );
    threeQuads[0] = _vmath2VfToHalfFloats(xyz0[0], xyz0[1]);
    threeQuads[1] = _vmath2VfToHalfFloats(xyz0[2], xyz1[0]);
    threeQuads[2] = _vmath2VfToHalfFloats(xyz1[1], xyz1[2]);
}

inline void storeHalfFloats( Point3 vec, uint16_t * hfptr )
{
    const vec_float4 vfval = vec.get128();
    const vec_ushort8 hfloat = (vec_ushort8)_vmathVfToHalfFloatsUnpacked(vfval);
    const vec_ushort8 x = vec_splat(hfloat, 1);
    const vec_ushort8 y = vec_splat(hfloat, 3);
    const vec_ushort8 z = vec_splat(hfloat, 5);
    vec_ste(x, 0, hfptr);
    vec_ste(y, 2, hfptr);
    vec_ste(z, 4, hfptr);
}

inline Point3 & Point3::operator =( Point3 pnt )
{
    mVec128 = pnt.mVec128;
    return *this;
}

inline Point3 & Point3::setX( float _x )
{
    _vmathVfSetElement(mVec128, _x, 0);
    return *this;
}

inline Point3 & Point3::setX( floatInVec _x )
{
    mVec128 = _vmathVfInsert(mVec128, _x.get128(), 0);
    return *this;
}

inline const floatInVec Point3::getX( ) const
{
    return floatInVec( mVec128, 0 );
}

inline Point3 & Point3::setY( float _y )
{
    _vmathVfSetElement(mVec128, _y, 1);
    return *this;
}

inline Point3 & Point3::setY( floatInVec _y )
{
    mVec128 = _vmathVfInsert(mVec128, _y.get128(), 1);
    return *this;
}

inline const floatInVec Point3::getY( ) const
{
    return floatInVec( mVec128, 1 );
}

inline Point3 & Point3::setZ( float _z )
{
    _vmathVfSetElement(mVec128, _z, 2);
    return *this;
}

inline Point3 & Point3::setZ( floatInVec _z )
{
    mVec128 = _vmathVfInsert(mVec128, _z.get128(), 2);
    return *this;
}

inline const floatInVec Point3::getZ( ) const
{
    return floatInVec( mVec128, 2 );
}

inline Point3 & Point3::setElem( int idx, float value )
{
    _vmathVfSetElement(mVec128, value, idx);
    return *this;
}

inline Point3 & Point3::setElem( int idx, floatInVec value )
{
    mVec128 = _vmathVfInsert(mVec128, value.get128(), idx);
    return *this;
}

inline const floatInVec Point3::getElem( int idx ) const
{
    return floatInVec( mVec128, idx );
}

inline VecIdx Point3::operator []( int idx )
{
    return VecIdx( mVec128, idx );
}

inline const floatInVec Point3::operator []( int idx ) const
{
    return floatInVec( mVec128, idx );
}

inline const Vector3 Point3::operator -( Point3 pnt ) const
{
    return Vector3( vec_sub( mVec128, pnt.mVec128 ) );
}

inline const Point3 Point3::operator +( Vector3 vec ) const
{
    return Point3( vec_add( mVec128, vec.get128() ) );
}

inline const Point3 Point3::operator -( Vector3 vec ) const
{
    return Point3( vec_sub( mVec128, vec.get128() ) );
}

inline Point3 & Point3::operator +=( Vector3 vec )
{
    *this = *this + vec;
    return *this;
}

inline Point3 & Point3::operator -=( Vector3 vec )
{
    *this = *this - vec;
    return *this;
}

inline const Point3 mulPerElem( Point3 pnt0, Point3 pnt1 )
{
    return Point3( vec_madd( pnt0.get128(), pnt1.get128(), (vec_float4)(0.0f) ) );
}

inline const Point3 divPerElem( Point3 pnt0, Point3 pnt1 )
{
    return Point3( divf4( pnt0.get128(), pnt1.get128() ) );
}

inline const Point3 recipPerElem( Point3 pnt )
{
    return Point3( recipf4( pnt.get128() ) );
}

inline const Point3 sqrtPerElem( Point3 pnt )
{
    return Point3( sqrtf4( pnt.get128() ) );
}

inline const Point3 rsqrtPerElem( Point3 pnt )
{
    return Point3( rsqrtf4( pnt.get128() ) );
}

inline const Point3 absPerElem( Point3 pnt )
{
    return Point3( fabsf4( pnt.get128() ) );
}

inline const Point3 copySignPerElem( Point3 pnt0, Point3 pnt1 )
{
    return Point3( copysignf4( pnt0.get128(), pnt1.get128() ) );
}

inline const Point3 maxPerElem( Point3 pnt0, Point3 pnt1 )
{
    return Point3( fmaxf4( pnt0.get128(), pnt1.get128() ) );
}

inline const floatInVec maxElem( Point3 pnt )
{
    vec_float4 result;
    result = fmaxf4( vec_splat( pnt.get128(), 1 ), pnt.get128() );
    result = fmaxf4( vec_splat( pnt.get128(), 2 ), result );
    return floatInVec( result, 0 );
}

inline const Point3 minPerElem( Point3 pnt0, Point3 pnt1 )
{
    return Point3( fminf4( pnt0.get128(), pnt1.get128() ) );
}

inline const floatInVec minElem( Point3 pnt )
{
    vec_float4 result;
    result = fminf4( vec_splat( pnt.get128(), 1 ), pnt.get128() );
    result = fminf4( vec_splat( pnt.get128(), 2 ), result );
    return floatInVec( result, 0 );
}

inline const floatInVec sum( Point3 pnt )
{
    vec_float4 result;
    result = vec_add( vec_splat( pnt.get128(), 1 ), pnt.get128() );
    result = vec_add( vec_splat( pnt.get128(), 2 ), result );
    return floatInVec( result, 0 );
}

inline const Point3 scale( Point3 pnt, float scaleVal )
{
    return scale( pnt, floatInVec( scaleVal ) );
}

inline const Point3 scale( Point3 pnt, floatInVec scaleVal )
{
    return mulPerElem( pnt, Point3( scaleVal ) );
}

inline const Point3 scale( Point3 pnt, Vector3 scaleVec )
{
    return mulPerElem( pnt, Point3( scaleVec ) );
}

inline const floatInVec projection( Point3 pnt, Vector3 unitVec )
{
    return floatInVec( _vmathVfDot3( pnt.get128(), unitVec.get128() ), 0 );
}

inline const floatInVec distSqrFromOrigin( Point3 pnt )
{
    return lengthSqr( Vector3( pnt ) );
}

inline const floatInVec distFromOrigin( Point3 pnt )
{
    return length( Vector3( pnt ) );
}

inline const floatInVec distSqr( Point3 pnt0, Point3 pnt1 )
{
    return lengthSqr( ( pnt1 - pnt0 ) );
}

inline const floatInVec dist( Point3 pnt0, Point3 pnt1 )
{
    return length( ( pnt1 - pnt0 ) );
}

inline const Point3 select( Point3 pnt0, Point3 pnt1, bool select1 )
{
    return select( pnt0, pnt1, boolInVec(select1) );
}

inline const Point3 select( Point3 pnt0, Point3 pnt1, boolInVec select1 )
{
    return Point3( vec_sel( pnt0.get128(), pnt1.get128(), select1.get128() ) );
}

#ifdef _VECTORMATH_DEBUG

inline void print( Point3 pnt )
{
    union { vec_float4 v; float s[4]; } tmp;
    tmp.v = pnt.get128();
    printf( "( %f %f %f )\n", tmp.s[0], tmp.s[1], tmp.s[2] );
}

inline void print( Point3 pnt, const char * name )
{
    union { vec_float4 v; float s[4]; } tmp;
    tmp.v = pnt.get128();
    printf( "%s: ( %f %f %f )\n", name, tmp.s[0], tmp.s[1], tmp.s[2] );
}

#endif

} // namespace Aos
} // namespace Vectormath

#endif
