/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 *                Copyright (C) 2009 Sony Computer Entertainment Inc.
 *                                               All Rights Reserved.
 */

#ifndef _TYPE_UTILS_H
#define _TYPE_UTILS_H

/* Following NV_half_float specification
 * The half data type is a floating-point
 * data type encoded in an unsigned scalar data type.  If the unsigned scalar
 * holding a half has a value of N, the corresponding floating point number
 * is

 *     (-1)^S * 0.0,                        if E == 0 and M == 0,
 *     (-1)^S * 2^-14 * (M / 2^10),         if E == 0 and M != 0,
 *     (-1)^S * 2^(E-15) * (1 + M/2^10),    if 0 < E < 31,
 *     (-1)^S * INF,                        if E == 31 and M == 0, or
 *     NaN,                                 if E == 31 and M != 0,
 *
 * where
 *
 *     S = floor((N mod 65536) / 32768),
 *     E = floor((N mod 32768) / 1024), and
 *     M = N mod 1024.
 *
 * INF (Infinity) is a special representation indicating numerical overflow.
 * NaN (Not a Number) is a special representation indicating the result of
 * illegal arithmetic operations, such as division by zero.  Note that all
 * normal values, zero, and INF have an associated sign.  -0.0 and +0.0
 * should be considered equivalent for the purposes of comparisons.  Note
 * also that half is not a native type in most CPUs, so some special
 * processing may be required to generate or interpret half data.
 *
 * That is SEEEEEMMMMMMMMMM
 */

typedef union
{
    unsigned int i;
    float f;
} jsIntAndFloat;

static const jsIntAndFloat _jsNan = {i: 0x7fc00000U};
static const jsIntAndFloat _jsInfinity = {i: 0x7f800000U};
#define JS_NAN (_jsNan.f)
#define JS_INFINITY (_jsInfinity.f)

static inline GLhalfARB _jsFloatToHalf( float v )
{
jsIntAndFloat V = {f: v};
    // extract float components
    unsigned int S = ( V.i >> 31 ) & 1;
    int E = (( V.i >> 23 ) & 0xff ) - 0x7f;
    unsigned int M = V.i & 0x007fffff;
    if (( E == 0x80 ) && ( M ) ) return 0x7fff; // NaN
    else if ( E >= 15 ) return( S << 15 ) | 0x7c00; // Inf
    else if ( E <= -14 ) return( S << 15 ) | (( 0x800000 + M ) >> ( -14 - E ) ); // Denorm or zero. Works for input denorms and zero
    else return( S << 15 ) | ((( E + 15 )&0x1f ) << 10 ) | ( M >> 13 );
}

static inline float _jsHalfToFloat( GLhalfARB v )
{
    unsigned int S = v >> 15;
    unsigned int E = ( v & 0x7C00 ) >> 10;
    unsigned int M = v & 0x03ff;
    float f;
    if ( E == 31 )
    {
        if ( M == 0 ) f = JS_INFINITY;
        else f = JS_NAN;
    }
    else if ( E == 0 )
    {
        if ( M == 0 ) f = 0.f;
        else f = M * 1.f / ( 1 << 24 );
    }
    else f = ( 0x400 + M ) * 1.f / ( 1 << 25 ) * ( 1 << E );
    return S ? -f : f;
}

//----------------------------------------
// Fixed Point Conversion Macros
// FixedPoint.c
//----------------------------------------

#define _jsFixedToFloat 	1.f/65536.f
#define _jsFloatToFixed 	65536.f

#define X2F(X) (((float)(X))*_jsFixedToFloat)
#define F2X(A) ((int)((A)*_jsFloatToFixed))

#endif // _TYPE_UTILS_H
