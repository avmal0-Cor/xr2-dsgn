/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2008 Sony Computer Entertainment Inc.
* All Rights Reserved.
*/

#ifndef _VECTORMATH_VEC_AOS_CPP_H
#define _VECTORMATH_VEC_AOS_CPP_H

//-----------------------------------------------------------------------------
// Constants
// for shuffles, words are labeled [x,y,z,w] [a,b,c,d]

#define _VECTORMATH_SHUF_X 0x00010203
#define _VECTORMATH_SHUF_Y 0x04050607
#define _VECTORMATH_SHUF_Z 0x08090a0b
#define _VECTORMATH_SHUF_W 0x0c0d0e0f
#define _VECTORMATH_SHUF_A 0x10111213
#define _VECTORMATH_SHUF_B 0x14151617
#define _VECTORMATH_SHUF_C 0x18191a1b
#define _VECTORMATH_SHUF_D 0x1c1d1e1f
#define _VECTORMATH_SHUF_0 0x80808080
#define _VECTORMATH_SHUF_XYZA (vec_uchar16)(vec_uint4){ _VECTORMATH_SHUF_X, _VECTORMATH_SHUF_Y, _VECTORMATH_SHUF_Z, _VECTORMATH_SHUF_A }
#define _VECTORMATH_SHUF_ZXYW (vec_uchar16)(vec_uint4){ _VECTORMATH_SHUF_Z, _VECTORMATH_SHUF_X, _VECTORMATH_SHUF_Y, _VECTORMATH_SHUF_W }
#define _VECTORMATH_SHUF_YZXW (vec_uchar16)(vec_uint4){ _VECTORMATH_SHUF_Y, _VECTORMATH_SHUF_Z, _VECTORMATH_SHUF_X, _VECTORMATH_SHUF_W }
#define _VECTORMATH_SHUF_WABC (vec_uchar16)(vec_uint4){ _VECTORMATH_SHUF_W, _VECTORMATH_SHUF_A, _VECTORMATH_SHUF_B, _VECTORMATH_SHUF_C }
#define _VECTORMATH_SHUF_ZWAB (vec_uchar16)(vec_uint4){ _VECTORMATH_SHUF_Z, _VECTORMATH_SHUF_W, _VECTORMATH_SHUF_A, _VECTORMATH_SHUF_B }
#define _VECTORMATH_SHUF_XYZA (vec_uchar16)(vec_uint4){ _VECTORMATH_SHUF_X, _VECTORMATH_SHUF_Y, _VECTORMATH_SHUF_Z, _VECTORMATH_SHUF_A }
#define _VECTORMATH_SHUF_YZAB (vec_uchar16)(vec_uint4){ _VECTORMATH_SHUF_Y, _VECTORMATH_SHUF_Z, _VECTORMATH_SHUF_A, _VECTORMATH_SHUF_B }
#define _VECTORMATH_SHUF_ZABC (vec_uchar16)(vec_uint4){ _VECTORMATH_SHUF_Z, _VECTORMATH_SHUF_A, _VECTORMATH_SHUF_B, _VECTORMATH_SHUF_C }
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
    result = spu_mul( vec0, vec1 );
    result = spu_madd( spu_rlqwbyte( vec0, 4 ), spu_rlqwbyte( vec1, 4 ), result );
    return spu_madd( spu_rlqwbyte( vec0, 8 ), spu_rlqwbyte( vec1, 8 ), result );
}

static inline vec_float4 _vmathVfDot4( vec_float4 vec0, vec_float4 vec1 )
{
    vec_float4 result;
    result = spu_mul( vec0, vec1 );
    result = spu_madd( spu_rlqwbyte( vec0, 4 ), spu_rlqwbyte( vec1, 4 ), result );
    return spu_add( spu_rlqwbyte( result, 8 ), result );
}

static inline vec_float4 _vmathVfCross( vec_float4 vec0, vec_float4 vec1 )
{
    vec_float4 tmp0, tmp1, tmp2, tmp3, result;
    tmp0 = spu_shuffle( vec0, vec0, _VECTORMATH_SHUF_YZXW );
    tmp1 = spu_shuffle( vec1, vec1, _VECTORMATH_SHUF_ZXYW );
    tmp2 = spu_shuffle( vec0, vec0, _VECTORMATH_SHUF_ZXYW );
    tmp3 = spu_shuffle( vec1, vec1, _VECTORMATH_SHUF_YZXW );
    result = spu_mul( tmp0, tmp1 );
    result = spu_nmsub( tmp2, tmp3, result );
    return result;
}

static inline vec_uint4 _vmathVfToHalfFloatsUnpacked(vec_float4 v)
{
    vec_int4 bexp;
    vec_uint4 mant, sign, hfloat;
    vec_uint4 notZero, isInf;
    const vec_uint4 hfloatInf = spu_splats(0x00007c00u);
    const vec_uint4 mergeMant = spu_splats(0x000003ffu);
    const vec_uint4 mergeSign = spu_splats(0x00008000u);

    sign = spu_rlmask((vec_uint4)v, -16);
    mant = spu_rlmask((vec_uint4)v, -13);
    bexp = spu_and(spu_rlmask((vec_int4)v, -23), 0xff);

    notZero = spu_cmpgt(bexp, 112);
    isInf = spu_cmpgt(bexp, 142);

    bexp = spu_add(bexp, -112);
    bexp = spu_sl(bexp, 10);

    hfloat = spu_sel((vec_uint4)bexp, mant, mergeMant);
    hfloat = spu_sel(spu_splats(0u), hfloat, notZero);
    hfloat = spu_sel(hfloat, hfloatInf, isInf);
    hfloat = spu_sel(hfloat, sign, mergeSign);

    return hfloat;
}

static inline vec_ushort8 _vmath2VfToHalfFloats(vec_float4 u, vec_float4 v)
{
    vec_uint4 hfloat_u, hfloat_v;
    const vec_uchar16 pack = (vec_uchar16){2,3,6,7,10,11,14,15,18,19,22,23,26,27,30,31};
    hfloat_u = _vmathVfToHalfFloatsUnpacked(u);
    hfloat_v = _vmathVfToHalfFloatsUnpacked(v);
    return (vec_ushort8)spu_shuffle(hfloat_u, hfloat_v, pack);
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

    sign = spu_sl(v, (vec_uint4){16,16,16,16});
    mant = spu_sl(v, (vec_uint4){13,13,13,13});
    bexp = (vec_int4)spu_and(mant, vhalfInf);
    notZero = (vec_uint4)spu_cmpgt(bexp, (vec_int4){0, 0, 0, 0});
    isInf = (vec_uint4)spu_cmpeq(bexp, (vec_int4)vhalfInf);
    bexp = spu_add(bexp, vfloatBase);

    vfloat = spu_sel((vec_uint4)bexp, mant, mergeMant);
    vfloat = spu_sel((vec_uint4){0,0,0,0}, vfloat, notZero);
    vfloat = spu_sel(vfloat, vfloatInf, isInf);
    vfloat = spu_sel(vfloat, sign, mergeSign);

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

    vec_uint4 vsrc0 = spu_shuffle(vsrc, vsrc, pat0);
    vec_uint4 vsrc1 = spu_shuffle(vsrc, vsrc, pat1);
    sign0 = spu_sl(vsrc0, (vec_uint4){16,16,16,16});
    sign1 = spu_sl(vsrc1, (vec_uint4){16,16,16,16});
    mant0 = spu_sl(vsrc0, (vec_uint4){13,13,13,13});
    mant1 = spu_sl(vsrc1, (vec_uint4){13,13,13,13});
    bexp0 = (vec_int4)spu_and(mant0, vhalfInf);
    bexp1 = (vec_int4)spu_and(mant1, vhalfInf);
    notZero0 = (vec_uint4)spu_cmpgt(bexp0, (vec_int4){0, 0, 0, 0});
    notZero1 = (vec_uint4)spu_cmpgt(bexp1, (vec_int4){0, 0, 0, 0});
    isInf0 = (vec_uint4)spu_cmpeq(bexp0, (vec_int4)vhalfInf);
    isInf1 = (vec_uint4)spu_cmpeq(bexp1, (vec_int4)vhalfInf);
    bexp0 = spu_add(bexp0, vfloatBase);
    bexp1 = spu_add(bexp1, vfloatBase);

    vfloat0 = spu_sel((vec_uint4)bexp0, mant0, mergeMant);
    vfloat1 = spu_sel((vec_uint4)bexp1, mant1, mergeMant);
    vfloat0 = spu_sel((vec_uint4){0,0,0,0}, vfloat0, notZero0);
    vfloat1 = spu_sel((vec_uint4){0,0,0,0}, vfloat1, notZero1);
    vfloat0 = spu_sel(vfloat0, vfloatInf, isInf0);
    vfloat1 = spu_sel(vfloat1, vfloatInf, isInf1);
    vfloat0 = spu_sel(vfloat0, sign0, mergeSign);
    vfloat1 = spu_sel(vfloat1, sign1, mergeSign);

    vec0 = (vec_float4)vfloat0;
    vec1 = (vec_float4)vfloat1;
}

static inline vec_uint4 _vmathHalfFloatsToVfUnpacked(vec_ushort8 v)
{
    vec_int4 bexp;
    vec_uint4 mant, sign, hfloat;
    vec_uint4 notZero, isInf;
    const vec_uint4 hfloatInf = spu_splats(0x00007c00u);
    const vec_uint4 mergeMant = spu_splats(0x000003ffu);
    const vec_uint4 mergeSign = spu_splats(0x00008000u);

    sign = spu_rlmask((vec_uint4)v, -16);
    mant = spu_rlmask((vec_uint4)v, -13);
    bexp = spu_and(spu_rlmask((vec_int4)v, -23), 0xff);

    notZero = spu_cmpgt(bexp, 112);
    isInf = spu_cmpgt(bexp, 142);

    bexp = spu_add(bexp, -112);
    bexp = spu_sl(bexp, 10);

    hfloat = spu_sel((vec_uint4)bexp, mant, mergeMant);
    hfloat = spu_sel(spu_splats(0u), hfloat, notZero);
    hfloat = spu_sel(hfloat, hfloatInf, isInf);
    hfloat = spu_sel(hfloat, sign, mergeSign);

    return hfloat;
}

namespace Vectormath {
namespace Aos {

inline VecIdx::operator float() const
{
    return spu_extract( ref, i );
}

inline float VecIdx::operator =( float scalar )
{
    ref = spu_insert( scalar, ref, i );
    return scalar;
}

inline float VecIdx::operator =( const VecIdx& scalar )
{
    return *this = float(scalar);
}

inline float VecIdx::operator *=( float scalar )
{
    float tmp = spu_extract( ref, i ) * scalar;
    ref = spu_insert( tmp, ref, i );
    return tmp;
}

inline float VecIdx::operator /=( float scalar )
{
    float tmp = spu_extract( ref, i ) / scalar;
    ref = spu_insert( tmp, ref, i );
    return tmp;
}

inline float VecIdx::operator +=( float scalar )
{
    float tmp = spu_extract( ref, i ) + scalar;
    ref = spu_insert( tmp, ref, i );
    return tmp;
}

inline float VecIdx::operator -=( float scalar )
{
    float tmp = spu_extract( ref, i ) - scalar;
    ref = spu_insert( tmp, ref, i );
    return tmp;
}

inline Vector3::Vector3( float _x, float _y, float _z )
{
    mVec128 = (vec_float4){ _x, _y, _z, 0.0f  };
}

inline Vector3::Vector3( Point3 pnt )
{
    mVec128 = pnt.get128();
}

inline Vector3::Vector3( float scalar )
{
    mVec128 = spu_splats( scalar );
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
    return ( vec0 + ( ( vec1 - vec0 ) * t ) );
}

inline const Vector3 slerp( float t, Vector3 unitVec0, Vector3 unitVec1 )
{
    vec_float4 scales, scale0, scale1, cosAngle, angle, tttt, oneMinusT, angles, sines;
    vec_uint4 selectMask;
    vec_uchar16 shuffle_xxxx = (vec_uchar16)spu_splats((int)0x00010203);
    vec_uchar16 shuffle_yyyy = (vec_uchar16)spu_splats((int)0x04050607);
    vec_uchar16 shuffle_zzzz = (vec_uchar16)spu_splats((int)0x08090a0b);
    cosAngle = _vmathVfDot3( unitVec0.get128(), unitVec1.get128() );
    cosAngle = spu_shuffle( cosAngle, cosAngle, shuffle_xxxx );
    selectMask = (vec_uint4)spu_cmpgt( spu_splats(_VECTORMATH_SLERP_TOL), cosAngle );
    angle = acosf4( cosAngle );
    tttt = spu_splats(t);
    oneMinusT = spu_sub( spu_splats(1.0f), tttt );
    angles = spu_sel( spu_splats(1.0f), oneMinusT, (vec_uint4)spu_maskb(0x0f00) );
    angles = spu_sel( angles, tttt, (vec_uint4)spu_maskb(0x00f0) );
    angles = spu_mul( angles, angle );
    sines = sinf4( angles );
    scales = divf4( sines, spu_shuffle( sines, sines, shuffle_xxxx ) );
    scale0 = spu_sel( oneMinusT, spu_shuffle( scales, scales, shuffle_yyyy ), selectMask );
    scale1 = spu_sel( tttt, spu_shuffle( scales, scales, shuffle_zzzz ), selectMask );
    return Vector3( spu_madd( unitVec0.get128(), scale0, spu_mul( unitVec1.get128(), scale1 ) ) );
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
#if 0
    vec_float4 vec0 = (vec_float4)si_lqd(fptr, 0);
    vec_float4 vec1 = (vec_float4)si_lqd(fptr, 16);
    uint32_t offset = (uint32_t)fptr & 0xf;
    vec0 = spu_slqwbyte(vec0, offset);
    vec1 = spu_rlmaskqwbyte(vec1, offset - 16);
    vec = Vector3( spu_or(vec0, vec1) );
#else
    const vec_float4 vfsrc0 = *((vec_float4*)((uintptr_t)fptr));
    const vec_float4 vfsrc1 = *((vec_float4*)(((uintptr_t)fptr) + 16));
    const vec_uchar16 vucpat =
      (vec_uchar16)spu_add((vec_ushort8)(spu_splats((unsigned char)(((int)fptr) & 0xf))),
                           (vec_ushort8)(0x0001, 0x0203, 0x0405, 0x0607,
                                         0x0809, 0x0A0B, 0x0C0D, 0x0E0F));
    const vec_float4 vfval = spu_shuffle(vfsrc0, vfsrc1, vucpat);
    vec = Vector3( vfval );
#endif
}

inline void storeXYZ( Vector3 vec, vec_float4 * quad )
{
    vec_float4 dstVec = *quad;
    vec_uint4 mask = (vec_uint4)spu_maskb(0x000f);
    dstVec = spu_sel(vec.get128(), dstVec, mask);
    *quad = dstVec;
}

inline void storeXYZ( Vector3 vec, float * fptr )
{
#if 0
    vec_float4 dstVec0 = (vec_float4)si_lqd(fptr, 0);
    vec_float4 dstVec1 = (vec_float4)si_lqd(fptr, 16);
#else
    vec_float4 * vptr0 = (vec_float4*)((uintptr_t)fptr);
    vec_float4 * vptr1 = (vec_float4*)(((uintptr_t)fptr) + 16);
    vec_float4 dstVec0 = *vptr0;
    vec_float4 dstVec1 = *vptr1;
#endif
    uint32_t offset = (uint32_t)fptr & 0xf;
    vec_uint4 mask = (vec_uint4)spu_maskb(0xfff0);
    vec_uint4 mask0 = (vec_uint4)spu_rlmaskqwbyte(mask, -offset);
    vec_uint4 mask1 = (vec_uint4)spu_slqwbyte(mask, 16 - offset);
    vec_float4 vec0 = spu_rlmaskqwbyte(vec.get128(), -offset);
    vec_float4 vec1 = spu_slqwbyte(vec.get128(), 16 - offset);
    dstVec0 = spu_sel(dstVec0, vec0, mask0);
    dstVec1 = spu_sel(dstVec1, vec1, mask1);
#if 0
    si_stqd(dstVec0, fptr, 0);
    si_stqd(dstVec1, fptr, 16);
#else
    *vptr0 = dstVec0;
    *vptr1 = dstVec1;
#endif
}

inline void loadXYZArray( Vector3 & vec0, Vector3 & vec1, Vector3 & vec2, Vector3 & vec3, const vec_float4 * threeQuads )
{
    vec_float4 xyzx, yzxy, zxyz, xyz1, xyz2, xyz3;
    xyzx = threeQuads[0];
    yzxy = threeQuads[1];
    zxyz = threeQuads[2];
    xyz1 = spu_shuffle( xyzx, yzxy, _VECTORMATH_SHUF_WABC );
    xyz2 = spu_shuffle( yzxy, zxyz, _VECTORMATH_SHUF_ZWAB );
    xyz3 = spu_rlqwbyte( zxyz, 4 );
    vec0 = Vector3( xyzx );
    vec1 = Vector3( xyz1 );
    vec2 = Vector3( xyz2 );
    vec3 = Vector3( xyz3 );
}

inline void storeXYZArray( Vector3 vec0, Vector3 vec1, Vector3 vec2, Vector3 vec3, vec_float4 * threeQuads )
{
    vec_float4 xyzx, yzxy, zxyz;
    xyzx = spu_shuffle( vec0.get128(), vec1.get128(), _VECTORMATH_SHUF_XYZA );
    yzxy = spu_shuffle( vec1.get128(), vec2.get128(), _VECTORMATH_SHUF_YZAB );
    zxyz = spu_shuffle( vec2.get128(), vec3.get128(), _VECTORMATH_SHUF_ZABC );
    threeQuads[0] = xyzx;
    threeQuads[1] = yzxy;
    threeQuads[2] = zxyz;
}

inline void loadHalfFloats( Vector3 & vec0, Vector3 & vec1, Vector3 & vec2, Vector3 & vec3, Vector3 & vec4, Vector3 & vec5, Vector3 & vec6, Vector3 & vec7, const vec_ushort8 * threeQuads )
{
    const vec_uint4 patWAB = (vec_uint4){_VECTORMATH_SHUF_W,_VECTORMATH_SHUF_A,_VECTORMATH_SHUF_B,_VECTORMATH_SHUF_C};
    const vec_uint4 patZWA = (vec_uint4){_VECTORMATH_SHUF_Z,_VECTORMATH_SHUF_W,_VECTORMATH_SHUF_A,_VECTORMATH_SHUF_B};
    const vec_uint4 patYZW = (vec_uint4){_VECTORMATH_SHUF_Y,_VECTORMATH_SHUF_Z,_VECTORMATH_SHUF_W,_VECTORMATH_SHUF_A};
    vec_float4 v0, v1, v2, v3, v4, v5;
    _vmath2HalfFloatsToVfUnpacked(v0, v1, (vec_uint4)threeQuads[0]);
    _vmath2HalfFloatsToVfUnpacked(v2, v3, (vec_uint4)threeQuads[1]);
    _vmath2HalfFloatsToVfUnpacked(v4, v5, (vec_uint4)threeQuads[2]);
    vec0 = Vector3(v0);
    vec1 = Vector3(spu_shuffle(v0, v1, (vec_uchar16)patWAB));
    vec2 = Vector3(spu_shuffle(v1, v2, (vec_uchar16)patZWA));
    vec3 = Vector3(spu_shuffle(v2, v2, (vec_uchar16)patYZW));
    vec4 = Vector3(v3);
    vec5 = Vector3(spu_shuffle(v3, v4, (vec_uchar16)patWAB));
    vec6 = Vector3(spu_shuffle(v4, v5, (vec_uchar16)patZWA));
    vec7 = Vector3(spu_shuffle(v5, v5, (vec_uchar16)patYZW));
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
    vec_uint4 vsrc = (vec_uint4)spu_shuffle(vdata0, vdata1, pat[idx]);
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
    const vec_uchar16 pat_front[8] =
      {{0x02,0x03, 0x06,0x07, 0x0a,0x0b, 0x16,0x17,0x18,0x19,0x1a,0x1b,0x1c,0x1d,0x1e,0x1f},
       {0x10,0x11, 0x02,0x03, 0x06,0x07, 0x0a,0x0b, 0x18,0x19,0x1a,0x1b,0x1c,0x1d,0x1e,0x1f},
       {0x10,0x11,0x12,0x13, 0x02,0x03, 0x06,0x07, 0x0a,0x0b, 0x1a,0x1b,0x1c,0x1d,0x1e,0x1f},
       {0x10,0x11,0x12,0x13,0x14,0x15, 0x02,0x03, 0x06,0x07, 0x0a,0x0b, 0x1c,0x1d,0x1e,0x1f},
       {0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17, 0x02,0x03, 0x06,0x07, 0x0a,0x0b, 0x1e,0x1f},
       {0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19, 0x02,0x03, 0x06,0x07, 0x0a,0x0b},
       {0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1a,0x1b, 0x02,0x03, 0x06,0x07},
       {0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1a,0x1b,0x1c,0x1d, 0x02,0x03}};
    const vec_uchar16 pat_back[8] =
      {{0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1a,0x1b,0x1c,0x1d,0x1e,0x1f},
       {0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1a,0x1b,0x1c,0x1d,0x1e,0x1f},
       {0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1a,0x1b,0x1c,0x1d,0x1e,0x1f},
       {0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1a,0x1b,0x1c,0x1d,0x1e,0x1f},
       {0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1a,0x1b,0x1c,0x1d,0x1e,0x1f},
       {0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1a,0x1b,0x1c,0x1d,0x1e,0x1f},
       {0x0a,0x0b, 0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1a,0x1b,0x1c,0x1d,0x1e,0x1f},
       {0x06,0x07, 0x0a,0x0b, 0x14,0x15,0x16,0x17,0x18,0x19,0x1a,0x1b,0x1c,0x1d,0x1e,0x1f}};
    const int idx = (((int)hfptr) & 0xf) >> 1;
    vec_ushort8 * vptr0 = (vec_ushort8*)((uintptr_t)hfptr);
    vec_ushort8 * vptr1 = (vec_ushort8*)(((uintptr_t)hfptr) + 16);
    vec_ushort8 vdata0 = *vptr0;
    vec_ushort8 vdata1 = *vptr1;
    vdata0 = spu_shuffle(hfloat, vdata0, pat_front[idx]);
    vdata1 = spu_shuffle(hfloat, vdata1, pat_back[idx]);
    *vptr0 = vdata0;
    *vptr1 = vdata1;
}

inline Vector3 & Vector3::operator =( Vector3 vec )
{
    mVec128 = vec.mVec128;
    return *this;
}

inline Vector3 & Vector3::setX( float _x )
{
    mVec128 = spu_insert( _x, mVec128, 0 );
    return *this;
}

inline float Vector3::getX( ) const
{
    return spu_extract( mVec128, 0 );
}

inline Vector3 & Vector3::setY( float _y )
{
    mVec128 = spu_insert( _y, mVec128, 1 );
    return *this;
}

inline float Vector3::getY( ) const
{
    return spu_extract( mVec128, 1 );
}

inline Vector3 & Vector3::setZ( float _z )
{
    mVec128 = spu_insert( _z, mVec128, 2 );
    return *this;
}

inline float Vector3::getZ( ) const
{
    return spu_extract( mVec128, 2 );
}

inline Vector3 & Vector3::setElem( int idx, float value )
{
    mVec128 = spu_insert( value, mVec128, idx );
    return *this;
}

inline float Vector3::getElem( int idx ) const
{
    return spu_extract( mVec128, idx );
}

inline VecIdx Vector3::operator []( int idx )
{
    return VecIdx( mVec128, idx );
}

inline float Vector3::operator []( int idx ) const
{
    return spu_extract( mVec128, idx );
}

inline const Vector3 Vector3::operator +( Vector3 vec ) const
{
    return Vector3( spu_add( mVec128, vec.mVec128 ) );
}

inline const Vector3 Vector3::operator -( Vector3 vec ) const
{
    return Vector3( spu_sub( mVec128, vec.mVec128 ) );
}

inline const Point3 Vector3::operator +( Point3 pnt ) const
{
    return Point3( spu_add( mVec128, pnt.get128() ) );
}

inline const Vector3 Vector3::operator *( float scalar ) const
{
    return Vector3( spu_mul( mVec128, spu_splats(scalar) ) );
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

inline const Vector3 Vector3::operator /( float scalar ) const
{
    return Vector3( divf4( mVec128, spu_splats(scalar) ) );
}

inline Vector3 & Vector3::operator /=( float scalar )
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
    return vec * scalar;
}

inline const Vector3 mulPerElem( Vector3 vec0, Vector3 vec1 )
{
    return Vector3( spu_mul( vec0.get128(), vec1.get128() ) );
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

inline float maxElem( Vector3 vec )
{
    vec_float4 result;
    result = fmaxf4( spu_promote( spu_extract( vec.get128(), 1 ), 0 ), vec.get128() );
    result = fmaxf4( spu_promote( spu_extract( vec.get128(), 2 ), 0 ), result );
    return spu_extract( result, 0 );
}

inline const Vector3 minPerElem( Vector3 vec0, Vector3 vec1 )
{
    return Vector3( fminf4( vec0.get128(), vec1.get128() ) );
}

inline float minElem( Vector3 vec )
{
    vec_float4 result;
    result = fminf4( spu_promote( spu_extract( vec.get128(), 1 ), 0 ), vec.get128() );
    result = fminf4( spu_promote( spu_extract( vec.get128(), 2 ), 0 ), result );
    return spu_extract( result, 0 );
}

inline float sum( Vector3 vec )
{
    return
        spu_extract( vec.get128(), 0 ) +
        spu_extract( vec.get128(), 1 ) +
        spu_extract( vec.get128(), 2 );
}

inline float dot( Vector3 vec0, Vector3 vec1 )
{
    return spu_extract( _vmathVfDot3( vec0.get128(), vec1.get128() ), 0 );
}

inline float lengthSqr( Vector3 vec )
{
    return spu_extract( _vmathVfDot3( vec.get128(), vec.get128() ), 0 );
}

inline float length( Vector3 vec )
{
    return ::sqrtf( lengthSqr( vec ) );
}

inline const Vector3 normalize( Vector3 vec )
{
    vec_float4 dot = _vmathVfDot3( vec.get128(), vec.get128() );
    dot = spu_shuffle( dot, dot, (vec_uchar16)spu_splats(0x00010203) );
    return Vector3( spu_mul( vec.get128(), rsqrtf4( dot ) ) );
}

inline const Vector3 cross( Vector3 vec0, Vector3 vec1 )
{
    return Vector3( _vmathVfCross( vec0.get128(), vec1.get128() ) );
}

inline const Vector3 select( Vector3 vec0, Vector3 vec1, bool select1 )
{
    return Vector3( spu_sel( vec0.get128(), vec1.get128(), spu_splats( (unsigned int)-(select1 > 0) ) ) );
}

#ifdef _VECTORMATH_DEBUG

inline void print( Vector3 vec )
{
    union { vec_float4 v; float s[4]; } tmp;
    tmp.v = vec.get128();
    spu_printf( "( %f %f %f )\n", tmp.s[0], tmp.s[1], tmp.s[2] );
}

inline void print( Vector3 vec, const char * name )
{
    union { vec_float4 v; float s[4]; } tmp;
    tmp.v = vec.get128();
    spu_printf( "%s: ( %f %f %f )\n", name, tmp.s[0], tmp.s[1], tmp.s[2] );
}

#endif

inline Vector4::Vector4( float _x, float _y, float _z, float _w )
{
    mVec128 = (vec_float4){ _x, _y, _z, _w };
}

inline Vector4::Vector4( Vector3 xyz, float _w )
{
    mVec128 = spu_shuffle( xyz.get128(), spu_promote( _w, 0 ), _VECTORMATH_SHUF_XYZA );
}

inline Vector4::Vector4( Vector3 vec )
{
    mVec128 = spu_sel( vec.get128(), spu_splats(0.0f), (vec_uint4)spu_maskb(0x000f) );
}

inline Vector4::Vector4( Point3 pnt )
{
    mVec128 = spu_sel( pnt.get128(), spu_splats(1.0f), (vec_uint4)spu_maskb(0x000f) );
}

inline Vector4::Vector4( Quat quat )
{
    mVec128 = quat.get128();
}

inline Vector4::Vector4( float scalar )
{
    mVec128 = spu_splats( scalar );
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
    return ( vec0 + ( ( vec1 - vec0 ) * t ) );
}

inline const Vector4 slerp( float t, Vector4 unitVec0, Vector4 unitVec1 )
{
    vec_float4 scales, scale0, scale1, cosAngle, angle, tttt, oneMinusT, angles, sines;
    vec_uint4 selectMask;
    vec_uchar16 shuffle_xxxx = (vec_uchar16)spu_splats((int)0x00010203);
    vec_uchar16 shuffle_yyyy = (vec_uchar16)spu_splats((int)0x04050607);
    vec_uchar16 shuffle_zzzz = (vec_uchar16)spu_splats((int)0x08090a0b);
    cosAngle = _vmathVfDot4( unitVec0.get128(), unitVec1.get128() );
    cosAngle = spu_shuffle( cosAngle, cosAngle, shuffle_xxxx );
    selectMask = (vec_uint4)spu_cmpgt( spu_splats(_VECTORMATH_SLERP_TOL), cosAngle );
    angle = acosf4( cosAngle );
    tttt = spu_splats(t);
    oneMinusT = spu_sub( spu_splats(1.0f), tttt );
    angles = spu_sel( spu_splats(1.0f), oneMinusT, (vec_uint4)spu_maskb(0x0f00) );
    angles = spu_sel( angles, tttt, (vec_uint4)spu_maskb(0x00f0) );
    angles = spu_mul( angles, angle );
    sines = sinf4( angles );
    scales = divf4( sines, spu_shuffle( sines, sines, shuffle_xxxx ) );
    scale0 = spu_sel( oneMinusT, spu_shuffle( scales, scales, shuffle_yyyy ), selectMask );
    scale1 = spu_sel( tttt, spu_shuffle( scales, scales, shuffle_zzzz ), selectMask );
    return Vector4( spu_madd( unitVec0.get128(), scale0, spu_mul( unitVec1.get128(), scale1 ) ) );
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
#if 0
    vec_float4 vec0 = (vec_float4)si_lqd(fptr, 0);
    vec_float4 vec1 = (vec_float4)si_lqd(fptr, 16);
    uint32_t offset = (uint32_t)fptr & 0xf;
    vec0 = spu_slqwbyte(vec0, offset);
    vec1 = spu_rlmaskqwbyte(vec1, offset - 16);
    vec = Vector4( spu_or(vec0, vec1) );
#else
    const vec_float4 vfsrc0 = *((vec_float4*)((uintptr_t)fptr));
    const vec_float4 vfsrc1 = *((vec_float4*)(((uintptr_t)fptr) + 16));
    const vec_uchar16 vucpat =
      (vec_uchar16)spu_add((vec_ushort8)(spu_splats((unsigned char)(((int)fptr) & 0xf))),
                           (vec_ushort8)(0x0001, 0x0203, 0x0405, 0x0607,
                                         0x0809, 0x0A0B, 0x0C0D, 0x0E0F));
    const vec_float4 vfval = spu_shuffle(vfsrc0, vfsrc1, vucpat);
    vec = Vector4( vfval );
#endif
}

inline void storeXYZW( Vector4 vec, float * fptr )
{
#if 0
    vec_float4 dstVec0 = (vec_float4)si_lqd(fptr, 0);
    vec_float4 dstVec1 = (vec_float4)si_lqd(fptr, 16);
#else
    vec_float4 * vptr0 = (vec_float4*)((uintptr_t)fptr);
    vec_float4 * vptr1 = (vec_float4*)(((uintptr_t)fptr) + 16);
    vec_float4 dstVec0 = *vptr0;
    vec_float4 dstVec1 = *vptr1;
#endif
    uint32_t offset = (uint32_t)fptr & 0xf;
    vec_uint4 mask = (vec_uint4)spu_splats(0xffffffff);
    vec_uint4 mask0 = (vec_uint4)spu_rlmaskqwbyte(mask, -offset);
    vec_uint4 mask1 = (vec_uint4)spu_slqwbyte(mask, 16 - offset);
    vec_float4 vec0 = spu_rlmaskqwbyte(vec.get128(), -offset);
    vec_float4 vec1 = spu_slqwbyte(vec.get128(), 16 - offset);
    dstVec0 = spu_sel(dstVec0, vec0, mask0);
    dstVec1 = spu_sel(dstVec1, vec1, mask1);
#if 0
    si_stqd(dstVec0, fptr, 0);
    si_stqd(dstVec1, fptr, 16);
#else
    *vptr0 = dstVec0;
    *vptr1 = dstVec1;
#endif
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
    vec_uint4 vsrc = (vec_uint4)spu_shuffle(vdata0, vdata1, pat[idx]);
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
    const vec_uchar16 pat_front[8] =
      {{0x02,0x03, 0x06,0x07, 0x0a,0x0b, 0x0e,0x0f, 0x18,0x19,0x1a,0x1b,0x1c,0x1d,0x1e,0x1f},
       {0x10,0x11, 0x02,0x03, 0x06,0x07, 0x0a,0x0b, 0x0e,0x0f, 0x1a,0x1b,0x1c,0x1d,0x1e,0x1f},
       {0x10,0x11,0x12,0x13, 0x02,0x03, 0x06,0x07, 0x0a,0x0b, 0x0e,0x0f, 0x1c,0x1d,0x1e,0x1f},
       {0x10,0x11,0x12,0x13,0x14,0x15, 0x02,0x03, 0x06,0x07, 0x0a,0x0b, 0x0e,0x0f, 0x1e,0x1f},
       {0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17, 0x02,0x03, 0x06,0x07, 0x0a,0x0b, 0x0e,0x0f},
       {0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19, 0x02,0x03, 0x06,0x07, 0x0a,0x0b},
       {0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1a,0x1b, 0x02,0x03, 0x06,0x07},
       {0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1a,0x1b,0x1c,0x1d, 0x02,0x03}};
    const vec_uchar16 pat_back[8] =
      {{0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1a,0x1b,0x1c,0x1d,0x1e,0x1f},
       {0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1a,0x1b,0x1c,0x1d,0x1e,0x1f},
       {0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1a,0x1b,0x1c,0x1d,0x1e,0x1f},
       {0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1a,0x1b,0x1c,0x1d,0x1e,0x1f},
       {0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1a,0x1b,0x1c,0x1d,0x1e,0x1f},
       {0x0e,0x0f, 0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1a,0x1b,0x1c,0x1d,0x1e,0x1f},
       {0x0a,0x0b, 0x0e,0x0f, 0x14,0x15,0x16,0x17,0x18,0x19,0x1a,0x1b,0x1c,0x1d,0x1e,0x1f},
       {0x06,0x07, 0x0a,0x0b, 0x0e,0x0f, 0x16,0x17,0x18,0x19,0x1a,0x1b,0x1c,0x1d,0x1e,0x1f}};
    const int idx = (((int)hfptr) & 0xf) >> 1;
    vec_ushort8 * vptr0 = (vec_ushort8*)((uintptr_t)hfptr);
    vec_ushort8 * vptr1 = (vec_ushort8*)(((uintptr_t)hfptr) + 16);
    vec_ushort8 vdata0 = *vptr0;
    vec_ushort8 vdata1 = *vptr1;
    vdata0 = spu_shuffle(hfloat, vdata0, pat_front[idx]);
    vdata1 = spu_shuffle(hfloat, vdata1, pat_back[idx]);
    *vptr0 = vdata0;
    *vptr1 = vdata1;
}

inline Vector4 & Vector4::operator =( Vector4 vec )
{
    mVec128 = vec.mVec128;
    return *this;
}

inline Vector4 & Vector4::setXYZ( Vector3 vec )
{
    mVec128 = spu_sel( vec.get128(), mVec128, (vec_uint4)spu_maskb(0x000f) );
    return *this;
}

inline const Vector3 Vector4::getXYZ( ) const
{
    return Vector3( mVec128 );
}

inline Vector4 & Vector4::setX( float _x )
{
    mVec128 = spu_insert( _x, mVec128, 0 );
    return *this;
}

inline float Vector4::getX( ) const
{
    return spu_extract( mVec128, 0 );
}

inline Vector4 & Vector4::setY( float _y )
{
    mVec128 = spu_insert( _y, mVec128, 1 );
    return *this;
}

inline float Vector4::getY( ) const
{
    return spu_extract( mVec128, 1 );
}

inline Vector4 & Vector4::setZ( float _z )
{
    mVec128 = spu_insert( _z, mVec128, 2 );
    return *this;
}

inline float Vector4::getZ( ) const
{
    return spu_extract( mVec128, 2 );
}

inline Vector4 & Vector4::setW( float _w )
{
    mVec128 = spu_insert( _w, mVec128, 3 );
    return *this;
}

inline float Vector4::getW( ) const
{
    return spu_extract( mVec128, 3 );
}

inline Vector4 & Vector4::setElem( int idx, float value )
{
    mVec128 = spu_insert( value, mVec128, idx );
    return *this;
}

inline float Vector4::getElem( int idx ) const
{
    return spu_extract( mVec128, idx );
}

inline VecIdx Vector4::operator []( int idx )
{
    return VecIdx( mVec128, idx );
}

inline float Vector4::operator []( int idx ) const
{
    return spu_extract( mVec128, idx );
}

inline const Vector4 Vector4::operator +( Vector4 vec ) const
{
    return Vector4( spu_add( mVec128, vec.mVec128 ) );
}

inline const Vector4 Vector4::operator -( Vector4 vec ) const
{
    return Vector4( spu_sub( mVec128, vec.mVec128 ) );
}

inline const Vector4 Vector4::operator *( float scalar ) const
{
    return Vector4( spu_mul( mVec128, spu_splats(scalar) ) );
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

inline const Vector4 Vector4::operator /( float scalar ) const
{
    return Vector4( divf4( mVec128, spu_splats(scalar) ) );
}

inline Vector4 & Vector4::operator /=( float scalar )
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
    return vec * scalar;
}

inline const Vector4 mulPerElem( Vector4 vec0, Vector4 vec1 )
{
    return Vector4( spu_mul( vec0.get128(), vec1.get128() ) );
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

inline float maxElem( Vector4 vec )
{
    vec_float4 result;
    result = fmaxf4( spu_promote( spu_extract( vec.get128(), 1 ), 0 ), vec.get128() );
    result = fmaxf4( spu_promote( spu_extract( vec.get128(), 2 ), 0 ), result );
    result = fmaxf4( spu_promote( spu_extract( vec.get128(), 3 ), 0 ), result );
    return spu_extract( result, 0 );
}

inline const Vector4 minPerElem( Vector4 vec0, Vector4 vec1 )
{
    return Vector4( fminf4( vec0.get128(), vec1.get128() ) );
}

inline float minElem( Vector4 vec )
{
    vec_float4 result;
    result = fminf4( spu_promote( spu_extract( vec.get128(), 1 ), 0 ), vec.get128() );
    result = fminf4( spu_promote( spu_extract( vec.get128(), 2 ), 0 ), result );
    result = fminf4( spu_promote( spu_extract( vec.get128(), 3 ), 0 ), result );
    return spu_extract( result, 0 );
}

inline float sum( Vector4 vec )
{
    return
        spu_extract( vec.get128(), 0 ) +
        spu_extract( vec.get128(), 1 ) +
        spu_extract( vec.get128(), 2 ) +
        spu_extract( vec.get128(), 3 );
}

inline float dot( Vector4 vec0, Vector4 vec1 )
{
    return spu_extract( _vmathVfDot4( vec0.get128(), vec1.get128() ), 0 );
}

inline float lengthSqr( Vector4 vec )
{
    return spu_extract( _vmathVfDot4( vec.get128(), vec.get128() ), 0 );
}

inline float length( Vector4 vec )
{
    return ::sqrtf( lengthSqr( vec ) );
}

inline const Vector4 normalize( Vector4 vec )
{
    vec_float4 dot = _vmathVfDot4( vec.get128(), vec.get128() );
    return Vector4( spu_mul( vec.get128(), rsqrtf4( dot ) ) );
}

inline const Vector4 select( Vector4 vec0, Vector4 vec1, bool select1 )
{
    return Vector4( spu_sel( vec0.get128(), vec1.get128(), spu_splats( (unsigned int)-(select1 > 0) ) ) );
}

#ifdef _VECTORMATH_DEBUG

inline void print( Vector4 vec )
{
    union { vec_float4 v; float s[4]; } tmp;
    tmp.v = vec.get128();
    spu_printf( "( %f %f %f %f )\n", tmp.s[0], tmp.s[1], tmp.s[2], tmp.s[3] );
}

inline void print( Vector4 vec, const char * name )
{
    union { vec_float4 v; float s[4]; } tmp;
    tmp.v = vec.get128();
    spu_printf( "%s: ( %f %f %f %f )\n", name, tmp.s[0], tmp.s[1], tmp.s[2], tmp.s[3] );
}

#endif

inline Point3::Point3( float _x, float _y, float _z )
{
    mVec128 = (vec_float4){ _x, _y, _z, 0.0f  };
}

inline Point3::Point3( Vector3 vec )
{
    mVec128 = vec.get128();
}

inline Point3::Point3( float scalar )
{
    mVec128 = spu_splats( scalar );
}

inline Point3::Point3( vec_float4 vf4 )
{
    mVec128 = vf4;
}

inline const Point3 lerp( float t, Point3 pnt0, Point3 pnt1 )
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
#if 0
    vec_float4 vec0 = (vec_float4)si_lqd(fptr, 0);
    vec_float4 vec1 = (vec_float4)si_lqd(fptr, 16);
    uint32_t offset = (uint32_t)fptr & 0xf;
    vec0 = spu_slqwbyte(vec0, offset);
    vec1 = spu_rlmaskqwbyte(vec1, offset - 16);
    pnt = Point3( spu_or(vec0, vec1) );
#else
    const vec_float4 vfsrc0 = *((vec_float4*)((uintptr_t)fptr));
    const vec_float4 vfsrc1 = *((vec_float4*)(((uintptr_t)fptr) + 16));
    const vec_uchar16 vucpat =
      (vec_uchar16)spu_add((vec_ushort8)(spu_splats((unsigned char)(((int)fptr) & 0xf))),
                           (vec_ushort8)(0x0001, 0x0203, 0x0405, 0x0607,
                                         0x0809, 0x0A0B, 0x0C0D, 0x0E0F));
    const vec_float4 vfval = spu_shuffle(vfsrc0, vfsrc1, vucpat);
    pnt = Point3( vfval );
#endif
}

inline void storeXYZ( Point3 pnt, vec_float4 * quad )
{
    vec_float4 dstVec = *quad;
    vec_uint4 mask = (vec_uint4)spu_maskb(0x000f);
    dstVec = spu_sel(pnt.get128(), dstVec, mask);
    *quad = dstVec;
}

inline void storeXYZ( Point3 pnt, float * fptr )
{
#if 0
    vec_float4 dstVec0 = (vec_float4)si_lqd(fptr, 0);
    vec_float4 dstVec1 = (vec_float4)si_lqd(fptr, 16);
#else
    vec_float4 * vptr0 = (vec_float4*)((uintptr_t)fptr);
    vec_float4 * vptr1 = (vec_float4*)(((uintptr_t)fptr) + 16);
    vec_float4 dstVec0 = *vptr0;
    vec_float4 dstVec1 = *vptr1;
#endif
    uint32_t offset = (uint32_t)fptr & 0xf;
    vec_uint4 mask = (vec_uint4)spu_maskb(0xfff0);
    vec_uint4 mask0 = (vec_uint4)spu_rlmaskqwbyte(mask, -offset);
    vec_uint4 mask1 = (vec_uint4)spu_slqwbyte(mask, 16 - offset);
    vec_float4 vec0 = spu_rlmaskqwbyte(pnt.get128(), -offset);
    vec_float4 vec1 = spu_slqwbyte(pnt.get128(), 16 - offset);
    dstVec0 = spu_sel(dstVec0, vec0, mask0);
    dstVec1 = spu_sel(dstVec1, vec1, mask1);
#if 0
    si_stqd(dstVec0, fptr, 0);
    si_stqd(dstVec1, fptr, 16);
#else
    *vptr0 = dstVec0;
    *vptr1 = dstVec1;
#endif
}

inline void loadXYZArray( Point3 & pnt0, Point3 & pnt1, Point3 & pnt2, Point3 & pnt3, const vec_float4 * threeQuads )
{
    vec_float4 xyzx, yzxy, zxyz, xyz1, xyz2, xyz3;
    xyzx = threeQuads[0];
    yzxy = threeQuads[1];
    zxyz = threeQuads[2];
    xyz1 = spu_shuffle( xyzx, yzxy, _VECTORMATH_SHUF_WABC );
    xyz2 = spu_shuffle( yzxy, zxyz, _VECTORMATH_SHUF_ZWAB );
    xyz3 = spu_rlqwbyte( zxyz, 4 );
    pnt0 = Point3( xyzx );
    pnt1 = Point3( xyz1 );
    pnt2 = Point3( xyz2 );
    pnt3 = Point3( xyz3 );
}

inline void storeXYZArray( Point3 pnt0, Point3 pnt1, Point3 pnt2, Point3 pnt3, vec_float4 * threeQuads )
{
    vec_float4 xyzx, yzxy, zxyz;
    xyzx = spu_shuffle( pnt0.get128(), pnt1.get128(), _VECTORMATH_SHUF_XYZA );
    yzxy = spu_shuffle( pnt1.get128(), pnt2.get128(), _VECTORMATH_SHUF_YZAB );
    zxyz = spu_shuffle( pnt2.get128(), pnt3.get128(), _VECTORMATH_SHUF_ZABC );
    threeQuads[0] = xyzx;
    threeQuads[1] = yzxy;
    threeQuads[2] = zxyz;
}

inline void loadHalfFloats( Point3 & vec0, Point3 & vec1, Point3 & vec2, Point3 & vec3, Point3 & vec4, Point3 & vec5, Point3 & vec6, Point3 & vec7, const vec_ushort8 * threeQuads )
{
    const vec_uint4 patWAB = (vec_uint4){_VECTORMATH_SHUF_W,_VECTORMATH_SHUF_A,_VECTORMATH_SHUF_B,_VECTORMATH_SHUF_C};
    const vec_uint4 patZWA = (vec_uint4){_VECTORMATH_SHUF_Z,_VECTORMATH_SHUF_W,_VECTORMATH_SHUF_A,_VECTORMATH_SHUF_B};
    const vec_uint4 patYZW = (vec_uint4){_VECTORMATH_SHUF_Y,_VECTORMATH_SHUF_Z,_VECTORMATH_SHUF_W,_VECTORMATH_SHUF_A};
    vec_float4 v0, v1, v2, v3, v4, v5;
    _vmath2HalfFloatsToVfUnpacked(v0, v1, (vec_uint4)threeQuads[0]);
    _vmath2HalfFloatsToVfUnpacked(v2, v3, (vec_uint4)threeQuads[1]);
    _vmath2HalfFloatsToVfUnpacked(v4, v5, (vec_uint4)threeQuads[2]);
    vec0 = Point3(v0);
    vec1 = Point3(spu_shuffle(v0, v1, (vec_uchar16)patWAB));
    vec2 = Point3(spu_shuffle(v1, v2, (vec_uchar16)patZWA));
    vec3 = Point3(spu_shuffle(v2, v2, (vec_uchar16)patYZW));
    vec4 = Point3(v3);
    vec5 = Point3(spu_shuffle(v3, v4, (vec_uchar16)patWAB));
    vec6 = Point3(spu_shuffle(v4, v5, (vec_uchar16)patZWA));
    vec7 = Point3(spu_shuffle(v5, v5, (vec_uchar16)patYZW));
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
    vec_uint4 vsrc = (vec_uint4)spu_shuffle(vdata0, vdata1, pat[idx]);
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
    const vec_uchar16 pat_front[8] =
      {{0x02,0x03, 0x06,0x07, 0x0a,0x0b, 0x16,0x17,0x18,0x19,0x1a,0x1b,0x1c,0x1d,0x1e,0x1f},
       {0x10,0x11, 0x02,0x03, 0x06,0x07, 0x0a,0x0b, 0x18,0x19,0x1a,0x1b,0x1c,0x1d,0x1e,0x1f},
       {0x10,0x11,0x12,0x13, 0x02,0x03, 0x06,0x07, 0x0a,0x0b, 0x1a,0x1b,0x1c,0x1d,0x1e,0x1f},
       {0x10,0x11,0x12,0x13,0x14,0x15, 0x02,0x03, 0x06,0x07, 0x0a,0x0b, 0x1c,0x1d,0x1e,0x1f},
       {0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17, 0x02,0x03, 0x06,0x07, 0x0a,0x0b, 0x1e,0x1f},
       {0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19, 0x02,0x03, 0x06,0x07, 0x0a,0x0b},
       {0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1a,0x1b, 0x02,0x03, 0x06,0x07},
       {0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1a,0x1b,0x1c,0x1d, 0x02,0x03}};
    const vec_uchar16 pat_back[8] =
      {{0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1a,0x1b,0x1c,0x1d,0x1e,0x1f},
       {0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1a,0x1b,0x1c,0x1d,0x1e,0x1f},
       {0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1a,0x1b,0x1c,0x1d,0x1e,0x1f},
       {0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1a,0x1b,0x1c,0x1d,0x1e,0x1f},
       {0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1a,0x1b,0x1c,0x1d,0x1e,0x1f},
       {0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1a,0x1b,0x1c,0x1d,0x1e,0x1f},
       {0x0a,0x0b, 0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1a,0x1b,0x1c,0x1d,0x1e,0x1f},
       {0x06,0x07, 0x0a,0x0b, 0x14,0x15,0x16,0x17,0x18,0x19,0x1a,0x1b,0x1c,0x1d,0x1e,0x1f}};
    const int idx = (((int)hfptr) & 0xf) >> 1;
    vec_ushort8 * vptr0 = (vec_ushort8*)((uintptr_t)hfptr);
    vec_ushort8 * vptr1 = (vec_ushort8*)(((uintptr_t)hfptr) + 16);
    vec_ushort8 vdata0 = *vptr0;
    vec_ushort8 vdata1 = *vptr1;
    vdata0 = spu_shuffle(hfloat, vdata0, pat_front[idx]);
    vdata1 = spu_shuffle(hfloat, vdata1, pat_back[idx]);
    *vptr0 = vdata0;
    *vptr1 = vdata1;
}

inline Point3 & Point3::operator =( Point3 pnt )
{
    mVec128 = pnt.mVec128;
    return *this;
}

inline Point3 & Point3::setX( float _x )
{
    mVec128 = spu_insert( _x, mVec128, 0 );
    return *this;
}

inline float Point3::getX( ) const
{
    return spu_extract( mVec128, 0 );
}

inline Point3 & Point3::setY( float _y )
{
    mVec128 = spu_insert( _y, mVec128, 1 );
    return *this;
}

inline float Point3::getY( ) const
{
    return spu_extract( mVec128, 1 );
}

inline Point3 & Point3::setZ( float _z )
{
    mVec128 = spu_insert( _z, mVec128, 2 );
    return *this;
}

inline float Point3::getZ( ) const
{
    return spu_extract( mVec128, 2 );
}

inline Point3 & Point3::setElem( int idx, float value )
{
    mVec128 = spu_insert( value, mVec128, idx );
    return *this;
}

inline float Point3::getElem( int idx ) const
{
    return spu_extract( mVec128, idx );
}

inline VecIdx Point3::operator []( int idx )
{
    return VecIdx( mVec128, idx );
}

inline float Point3::operator []( int idx ) const
{
    return spu_extract( mVec128, idx );
}

inline const Vector3 Point3::operator -( Point3 pnt ) const
{
    return Vector3( spu_sub( mVec128, pnt.mVec128 ) );
}

inline const Point3 Point3::operator +( Vector3 vec ) const
{
    return Point3( spu_add( mVec128, vec.get128() ) );
}

inline const Point3 Point3::operator -( Vector3 vec ) const
{
    return Point3( spu_sub( mVec128, vec.get128() ) );
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
    return Point3( spu_mul( pnt0.get128(), pnt1.get128() ) );
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

inline float maxElem( Point3 pnt )
{
    vec_float4 result;
    result = fmaxf4( spu_promote( spu_extract( pnt.get128(), 1 ), 0 ), pnt.get128() );
    result = fmaxf4( spu_promote( spu_extract( pnt.get128(), 2 ), 0 ), result );
    return spu_extract( result, 0 );
}

inline const Point3 minPerElem( Point3 pnt0, Point3 pnt1 )
{
    return Point3( fminf4( pnt0.get128(), pnt1.get128() ) );
}

inline float minElem( Point3 pnt )
{
    vec_float4 result;
    result = fminf4( spu_promote( spu_extract( pnt.get128(), 1 ), 0 ), pnt.get128() );
    result = fminf4( spu_promote( spu_extract( pnt.get128(), 2 ), 0 ), result );
    return spu_extract( result, 0 );
}

inline float sum( Point3 pnt )
{
    return
        spu_extract( pnt.get128(), 0 ) +
        spu_extract( pnt.get128(), 1 ) +
        spu_extract( pnt.get128(), 2 );
}

inline const Point3 scale( Point3 pnt, float scaleVal )
{
    return mulPerElem( pnt, Point3( scaleVal ) );
}

inline const Point3 scale( Point3 pnt, Vector3 scaleVec )
{
    return mulPerElem( pnt, Point3( scaleVec ) );
}

inline float projection( Point3 pnt, Vector3 unitVec )
{
    return spu_extract( _vmathVfDot3( pnt.get128(), unitVec.get128() ), 0 );
}

inline float distSqrFromOrigin( Point3 pnt )
{
    return lengthSqr( Vector3( pnt ) );
}

inline float distFromOrigin( Point3 pnt )
{
    return length( Vector3( pnt ) );
}

inline float distSqr( Point3 pnt0, Point3 pnt1 )
{
    return lengthSqr( ( pnt1 - pnt0 ) );
}

inline float dist( Point3 pnt0, Point3 pnt1 )
{
    return length( ( pnt1 - pnt0 ) );
}

inline const Point3 select( Point3 pnt0, Point3 pnt1, bool select1 )
{
    return Point3( spu_sel( pnt0.get128(), pnt1.get128(), spu_splats( (unsigned int)-(select1 > 0) ) ) );
}

#ifdef _VECTORMATH_DEBUG

inline void print( Point3 pnt )
{
    union { vec_float4 v; float s[4]; } tmp;
    tmp.v = pnt.get128();
    spu_printf( "( %f %f %f )\n", tmp.s[0], tmp.s[1], tmp.s[2] );
}

inline void print( Point3 pnt, const char * name )
{
    union { vec_float4 v; float s[4]; } tmp;
    tmp.v = pnt.get128();
    spu_printf( "%s: ( %f %f %f )\n", name, tmp.s[0], tmp.s[1], tmp.s[2] );
}

#endif

} // namespace Aos
} // namespace Vectormath

#endif
