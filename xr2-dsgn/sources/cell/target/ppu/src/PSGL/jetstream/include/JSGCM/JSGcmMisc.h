/* Copyright (C)2001-2006 by NVIDIA Corporation.  All rights reserved.
 * Modified by Sony Computer Entertainment Inc under license from NVIDIA Corporation.
 * Copyright (C) 2009 Sony Computer Entertainment Inc.
 * 
 * SCEI CONFIDENTIAL
 */

#ifndef _JSGCM_MISC_H_
#define _JSGCM_MISC_H_

/* defines the scissor box.
**
** notes:
**   - 1:1 port of glScissor (see spec for details)
**   - x, y, width, height are clamped to the surface limits
**   - initially, x and y are zero and width/height are set to
**     the maximum window width/height.
**   - (0,0) is in the lower left, (0, 0, 1, 1) limits the scissor to
**     the lower left pixel only [yInverted == TRUE]
*/
void static inline _jsGcmFifoGlScissor( GLint x, GLint y, GLsizei width, GLsizei height )
{
    jsGcmScissorState *scissor = &_jsGcmState.state.scissor;
    jsGcmRenderTarget *rt = &_jsGcmState.renderTarget;
    GLint clipX0, clipX1, clipY0, clipY1;

    // validate input parameters
    JS_ASSERT(( width >= 0 ) && ( height >= 0 ) );
    JS_ASSERT(( width <= JSGCM_MAX_RT_DIMENSION ) &&
              ( height <= JSGCM_MAX_RT_DIMENSION ) );

    // keep for yInverted handling/disable+enable
    scissor->x = x;
    scissor->y = y;
    scissor->w = width;
    scissor->h = height;

    // if scissor disabled, set to max size
    if ( !scissor->enabled )
    {
        x = y = 0;
        width = height = JSGCM_MAX_RT_DIMENSION;
    }

    // clamp to hw limits - yInverted: (0,0) is in the lower left
    clipX0 = x;
    clipX1 = x + width;
    if ( rt->yInverted )
    {
        clipY0 = rt->gcmRenderTarget.height - ( y + height );
        clipY1 = rt->gcmRenderTarget.height - y;
    }
    else
    {
        clipY0 = y;
        clipY1 = y + height;
    }
    if ( clipX0 < 0 )
    {
        clipX0 = 0;
    }
    if ( clipY0 < 0 )
    {
        clipY0 = 0;
    }
    if ( clipX1 >= JSGCM_MAX_RT_DIMENSION )
    {
        clipX1 = JSGCM_MAX_RT_DIMENSION;
    }
    if ( clipY1 >= JSGCM_MAX_RT_DIMENSION )
    {
        clipY1 = JSGCM_MAX_RT_DIMENSION;
    }
    if (( clipX1 <= clipX0 ) || ( clipY1 <= clipY0 ) )
    {
        clipX0 = clipY0 = clipX1 = clipY1 = 0;
    }

    GCM_FUNC( cellGcmSetScissor, clipX0, clipY0, clipX1 - clipX0, clipY1 - clipY0 );
}


// For quick float->int conversions
#define JSGCM_F0_DOT_0                              12582912.0f

// Clamp a float to [0,1]:
//   The order of clamp comparisons are arranged so that NaN maps to min
//   when compiled with VisualC or gcc.  IEEE says comparisons with NaN
//   should always fail (gcc implements this) but VisualC sometimes uses
//   signed integer compares rather than floating-point compares which is
//   incorrect in the case of NaN.
#define JSGCM_CLAMPF_01(x) \
    ((x) >= 0.0f ? ((x) > 1.0f ? 1.0f : (x)) : 0.0f)

// check if its a POW2 value
#define JSGCM_IS_POW2(v) \
    (((v) & ((v) - 1)) == 0)

// Fast conversion for values between 0.0 and 65535.0
GLuint inline static JSGCM_QUICK_FLOAT2UINT( const GLfloat f )
{
    union
    {
        GLfloat f;
        GLuint ui;
    } t;
    t.f = f + JSGCM_F0_DOT_0;
    return t.ui & 0xffff;
}

// Convert from float to half
GLushort inline static _jsGcmGLfloattoGLushort( GLfloat f )
{
    union F32TOU32
    {
        GLfloat f;
        GLuint ui;
    } tmp;
    GLuint data;
    GLuint exp;
    GLuint mant;
    GLuint ui;

    // Extract the exponent and the 10 MSBs of the mantissa from the fp32
    // number.
    tmp.f = f;
    ui = tmp.ui;
    exp = ( ui >> 23 ) & 0xFF;
    mant = ( ui >> 13 ) & 0x3FF;

    // Round on type conversion.  Check mantissa bit 11 in the 32-bit number.
    // If set, round the mantissa up.  If the mantissa overflows, bump the
    // exponent by 1 and clear the mantissa.
    if ( ui & 0x00001000 )
    {
        mant++;
        if ( mant & 0x400 )
        {
            mant = 0;
            exp++;
        }
    }

    if ( exp < 113 )
    {
        // |x| < 2^-14, implies 0.0 or Denorm

        // If |x| < 2^-25, we will flush to zero.  Otherwise, we will or in
        // the leading one and shift to the appropriate location.
        if ( exp < 101 )
        {
            data = 0;           // 0.0
        }
        else
        {
            data = ( mant | 0x400 ) >> ( 113 - exp );
        }
    }
    else if ( exp > 142 )
    {
        // |x| > 2^15, implies overflow, an existing INF, or NaN.  NaN is any
        // non-zero mantissa with an exponent of +128 (255).  Note that our
        // rounding algorithm could have kicked the exponent up to 256.
        if ( exp > 255 || ( exp == 255 && mant ) )
        {
            data = 0x7FFF;
        }
        else
        {
            data = 0x7C00;
        }
    }
    else
    {
        exp -= 112;
        data = ( exp << 10 ) | mant;
    }

    return ( GLushort )( data | (( ui >> 16 ) & 0x8000 ) );
}

// construct a packed unsigned int ARGB8 color
void inline static JSGCM_CALC_COLOR_LE_ARGB8( GLuint *color0, const GLfloat r, const GLfloat g, const GLfloat b, const GLfloat a )
{
    GLuint r2 = JSGCM_QUICK_FLOAT2UINT( r * 255.0f );
    GLuint g2 = JSGCM_QUICK_FLOAT2UINT( g * 255.0f );
    GLuint b2 = JSGCM_QUICK_FLOAT2UINT( b * 255.0f );
    GLuint a2 = JSGCM_QUICK_FLOAT2UINT( a * 255.0f );
    *color0 = ( a2 << 24 ) | ( r2 << 16 ) | ( g2 << 8 ) | ( b2 << 0 );
}

// construct a float16 rgba color
void inline static JSGCM_CALC_COLOR_F_ARGB16( GLuint *color, GLuint *color2, const GLfloat r, const GLfloat g, const GLfloat b, const GLfloat a )
{
    *color  = ( _jsGcmGLfloattoGLushort( g ) << 16 ) | _jsGcmGLfloattoGLushort( r );
    *color2 = ( _jsGcmGLfloattoGLushort( a ) << 16 ) | _jsGcmGLfloattoGLushort( b );
}

typedef struct
{
unsigned int context: 1;
unsigned int offset: 31;
}
PSGLgpuPtr;

// some useful macros
#define JSGCM_ADDPTR(p, offset) \
    ((char*) (((char*)(p)) + (offset)))

// Compute the floor of the log base 2 of a unsigned integer

GLuint inline static JSGCM_FLOOR_LOG2( const GLuint n )
{
    GLuint i;
    for ( i = 1; ( n >> i ) > 0; i++ )
    {
        // empty
    }
    return i -1;
}

/* helper function to swap the two 16bit values of a 32bit integer
** (needed for ucode constant downloads in big endian systems)
*/
GLuint inline static _jsGcmSwapGLuint_16( const GLuint v )
{
    return ( v >> 16 ) | ( v << 16 );
}

GLuint inline static _jsGcmSwapGLuint_8( const GLuint v )
{
    return ((( v&0xff ) << 24 ) | (( v&0xff00 ) << 8 ) | (( v&0xff0000 ) >> 8 ) | (( v&0xff000000 ) >> 24 ) );
}

GLfloat inline static _jsGcmSwapGLfloat_16( const GLfloat f )
{
    union SwapGLfloat_16
    {
        GLuint ui;
        GLfloat f;
    } v;
    v.f = f;
    v.ui = _jsGcmSwapGLuint_16( v.ui );
    return v.f;
}

GLushort inline static _jsGcmSwapGLushort_8( const GLushort v )
{
    return ( v >> 8 ) | ( v << 8 );
}


#endif // _JSGCM_MISC_H_

