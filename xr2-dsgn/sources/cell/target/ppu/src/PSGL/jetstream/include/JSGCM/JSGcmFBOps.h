/* Copyright (C)2001-2006 by NVIDIA Corporation.  All rights reserved.
 * Modified by Sony Computer Entertainment Inc under license from NVIDIA Corporation.
 * Copyright (C) 2009 Sony Computer Entertainment Inc.
 * 
 * SCEI CONFIDENTIAL
 */


/*
 * JSGcmFBOps.h - framebuffer operations
 */

#ifndef _H_JSGCM_FBOPS_H_
#define _H_JSGCM_FBOPS_H_

#include <JSGCM/JSGcmFifoUtils.h>

/* specifies the red, green, blue, and alpha values used by glClear to clear
** the color buffers.
**
** notes:
**   - 1:1 port of glClearColor (see spec for details)
**   - the float values are clamped to [0,1]
**   - initially, the clear color value is 0.0 for all components
*/
void static inline _jsGcmFifoGlClearColor( GLfloat r, GLfloat g, GLfloat b, GLfloat a )
{
    jsGcmClearState *clear = &_jsGcmState.state.clear;
    GLuint hwColor;

    clear->r = r;
    clear->g = g;
    clear->b = b;
    clear->a = a;
    r = JSGCM_CLAMPF_01( r );
    g = JSGCM_CLAMPF_01( g );
    b = JSGCM_CLAMPF_01( b );
    a = JSGCM_CLAMPF_01( a );
    JSGCM_CALC_COLOR_LE_ARGB8( &hwColor, r, g, b, a );

    GCM_FUNC( cellGcmSetClearColor, hwColor );
}

/* specifies the stencil value used by glClear to clear the stencil buffer.
**
** notes:
**   - 1:1 port of glClearStencil (see spec for details)
**   - the s parameter is masked with 2^m  1, where m is the number of bits
**     in the stencil buffer. [for curie m is 8 bits]
**   - initially, the clear stencil value is 0
*/
void static inline _jsGcmFifoGlClearStencil( GLint s )
{
    jsGcmClearState *clear = &_jsGcmState.state.clear;
    jsGcmRenderTarget *rt = &_jsGcmState.renderTarget;

    // stencil should only be set when it's Z24D8
    if (rt->depthFormat == JSGCM_DEPTH_COMPONENT24)
    {
        // clamp stencil
        s &= 0xff;

        // stencil is in bits 0..7
        clear->hwDepthStencil = ( clear->hwDepthStencil & 0xffffff00 ) | s;

        GCM_FUNC( cellGcmSetClearDepthStencil, clear->hwDepthStencil );
    }
}

/* specifies the depth value used by glClear to clear the depth buffer.
**
** notes:
**   - 1:1 port of glClearDepthf (see spec for details)
**   - the depth value are clamped to [0,1]
**   - initially, the clear depth value is 1.0
*/
void static inline _jsGcmFifoGlClearDepthf( GLclampf z )
{
    jsGcmClearState *clear = &_jsGcmState.state.clear;
    jsGcmRenderTarget *rt = &_jsGcmState.renderTarget;
    GLuint hwDepthStencil;

    // clamp argument
    z = JSGCM_CLAMPF_01( z );

    if (rt->depthFormat == JSGCM_DEPTH_COMPONENT24)
    {
        // depth is in bits 31..8
        hwDepthStencil = clear->hwDepthStencil & 0x000000ff;

        // note: we decided not to use doubles and so might end up loosing
        //        a few lsbs here -- in most cases this won't matter...
        hwDepthStencil |= (( GLuint )( z * 0xffffff ) ) << 8; 
        clear->hwDepthStencil = hwDepthStencil;

        GCM_FUNC( cellGcmSetClearDepthStencil, clear->hwDepthStencil );
    }

    if (rt->depthFormat == JSGCM_DEPTH_COMPONENT16)
    {
        // GCM docs is not clear on how cellGcmSetClearDepthStencil
        // will behave for 16-bit z.
        // For 16-bit z, clear is set by lsb??
        hwDepthStencil = ( GLuint )( z * 0xffff ); 

        clear->hwDepthStencil = hwDepthStencil;

        GCM_FUNC( cellGcmSetClearDepthStencil, clear->hwDepthStencil );
    }
}


/* sets the pixel area of the window to values previously selected by
** glClearColor, glClearDepth, and glClearStencil. The pixel-ownership test,
** the scissor test, dithering, and the buffer writemasks affect the
** operation of glClear. The scissor box bounds the cleared region. The
** glClear function ignores the alpha function, blend function, logical
** operation, stenciling, texture mapping, and z-buffering.
**
** notes:
**   - 1:1 port of glClear (see spec for details)
**   - legal bits for mask are JSGCM_COLOR_BUFFER_BIT, JSGCM_DEPTH_BUFFER_BIT,
**     JSGCM_STENCIL_BUFFER_BIT
**   - we might consider supporting zeta_id if clears show up while profiling
*/
void static inline _jsGcmFifoGlClear( GLbitfield mask )
{
    jsGcmRenderTarget *rt = &_jsGcmState.renderTarget;
    GLuint hwMask = 0;

    // check argument for supported bits
    JS_ASSERT(( mask & ~( JSGCM_COLOR_BUFFER_BIT | JSGCM_DEPTH_BUFFER_BIT | JSGCM_STENCIL_BUFFER_BIT ) ) == 0 );

    if ( mask & JSGCM_COLOR_BUFFER_BIT )
    {
        switch ( _jsGcmState.renderTarget.colorFormat )
        {
            case JSGCM_NONE:
                break;

            case JSGCM_ARGB8:
                if ( rt->writeR )
                {
                    hwMask |= CELL_GCM_CLEAR_R;
                }
                if ( rt->writeG )
                {
                    hwMask |= CELL_GCM_CLEAR_G;
                }
                if ( rt->writeB )
                {
                    hwMask |= CELL_GCM_CLEAR_B;
                }
                if ( rt->writeA )
                {
                    hwMask |= CELL_GCM_CLEAR_A;
                }
                break;
            case JSGCM_FLOAT_RGBA16:
            case JSGCM_FLOAT_R32:
            case JSGCM_LUMINANCE32F_ARB:
            case JSGCM_FLOAT_RGBA32:
                // RSX has no native support for clears on fat pixel formats.
                // -- instead render a fullscreen quad with depth test disabled
                // -- we can't do this here unless we start tracking a lot of render
                // -- state...
                JS_ASSERT( 0 );
                break;
            default:
                JS_ASSERT( 0 );
                break;
        }
    }

    if (( mask & JSGCM_DEPTH_BUFFER_BIT ) && ( rt->depthFormat != JSGCM_NONE ) )
    {
        _jsGcmUpdateZCullDepthParams();
        _jsGcmUpdateZCullStencilParams();
        hwMask |= CELL_GCM_CLEAR_Z;
    }

    // stencil should only be set when it's Z24D8
    if (( mask & JSGCM_STENCIL_BUFFER_BIT ) && ( rt->depthFormat == JSGCM_DEPTH_COMPONENT24 ) )
    {
        hwMask |= CELL_GCM_CLEAR_S;
    }

    if ( hwMask )
    {
        GCM_FUNC( cellGcmSetClearSurface, hwMask );
    }
}


/* specifies the function used to compare each incoming pixel z value
** with the z value present in the depth buffer. The comparison is
** performed only if depth testing is enabled. (See glEnable with
** the argument JSGCM_DEPTH_TEST.)
**
** notes:
**   - 1:1 port of glDepthFunc (see spec for details)
**   - initially, depth testing is disabled
**   - legal function are JSGCM_NEVER, JSGCM_LESS, JSGCM_EQUAL, JSGCM_LEQUAL,
**     JSGCM_GREATER, JSGCM_NOTEQUAL, JSGCM_GEQUAL and JSGCM_ALWAYS
**   - initally, the depth funcion is JSGCM_LESS
*/
void static inline _jsGcmFifoGlDepthFunc( jsGcmEnum zf )
{
    jsGcmZCullState *zcull = &_jsGcmState.state.zcull;
    switch ( zf )
    {
        case JSGCM_LESS:
        case JSGCM_LEQUAL:
        case JSGCM_EQUAL:
        case JSGCM_NEVER:
            zcull->hwControl0 = CELL_GCM_ZCULL_LESS
                                | CELL_GCM_ZCULL_LONES << 4 ;
            break;
        case JSGCM_GREATER:
        case JSGCM_NOTEQUAL:
        case JSGCM_GEQUAL:
        case JSGCM_ALWAYS:
            zcull->hwControl0 = CELL_GCM_ZCULL_GREATER
                                | CELL_GCM_ZCULL_MSB << 4;
            break;
        default:
            JS_ASSERT( 0 ); // chip exception ahead!
            break;
    }

    GCM_FUNC( cellGcmSetDepthFunc, zf );
}


/* sets the function and reference value for stencil testing.
**
** notes:
**   - extension of glStencilFunc (see spec for details)
**   - interacts with two-sided stencil/ActiveStencilFace
**   - legal values for func are JSGCM_NEVER, JSGCM_LESS, JSGCM_EQUAL, JSGCM_LEQUAL,
**     JSGCM_GREATER, JSGCM_NOTEQUAL, JSGCM_GEQUAL and JSGCM_ALWAYS
**   - the stencil reference value is clamped to the min/max possible
**     stencil value possible
**   - initially, stencil func is JSGCM_ALWAYS, ref is 0 and mask is all 1's
*/
void static inline _jsGcmFifoGlStencilFunc( jsGcmEnum func, GLint ref, GLuint mask )
{
    jsGcmStencilState *s = &_jsGcmState.state.stencil;

    // syntax check
    switch ( func )
    {
        case JSGCM_NEVER:
        case JSGCM_LESS:
        case JSGCM_EQUAL:
        case JSGCM_LEQUAL:
        case JSGCM_GREATER:
        case JSGCM_NOTEQUAL:
        case JSGCM_GEQUAL:
        case JSGCM_ALWAYS:
            break;
        default:
            JS_ASSERT( 0 ); // chip exception ahead!
            break;
    }

    // comply with spec
    if ( ref < 0 )
    {
        ref = 0;
    }
    if ( ref > 0xff )
    {
        ref = 0xff;
    }

    if ( s->activeStencilFace )
    {
        GCM_FUNC( cellGcmSetBackStencilFunc, func, ref, mask );
    }
    else
    {
        GCM_FUNC( cellGcmSetStencilFunc, func, ref, mask );
    }
    // we use the most recent set stencil func to program the stencil cull hint
    _jsGcmSetStencilCullHint( func, ref, mask );
}


/*
**
** notes:
**   - extension of glStencilOp (see spec for details)
**   - interacts with two-sided stencil/ActiveStencilFace
**   - legal values for operations are JSGCM_KEEP, JSGCM_ZERO, JSGCM_REPLACE, JSGCM_INCR,
**     JSGCM_DECR, JSGCM_INVERT, JSGCM_INCR_WRAP, JSGCM_DECR_WRAP
**   - initially, all operations are JSGCM_KEEP
*/
void static inline _jsGcmFifoGlStencilOp( jsGcmEnum fail, jsGcmEnum depthFail, jsGcmEnum depthPass )
{
    jsGcmStencilState *s = &_jsGcmState.state.stencil;

    // syntax check
    switch ( fail )
    {
        case JSGCM_KEEP:
        case JSGCM_ZERO:
        case JSGCM_REPLACE:
        case JSGCM_INCR:
        case JSGCM_DECR:
        case JSGCM_INVERT:
        case JSGCM_INCR_WRAP:
        case JSGCM_DECR_WRAP:
            break;
        default:
            JS_ASSERT( 0 ); // chip exception ahead!
            break;
    }
    switch ( depthFail )
    {
        case JSGCM_KEEP:
        case JSGCM_ZERO:
        case JSGCM_REPLACE:
        case JSGCM_INCR:
        case JSGCM_DECR:
        case JSGCM_INVERT:
        case JSGCM_INCR_WRAP:
        case JSGCM_DECR_WRAP:
            break;
        default:
            JS_ASSERT( 0 ); // chip exception ahead!
            break;
    }
    switch ( depthPass )
    {
        case JSGCM_KEEP:
        case JSGCM_ZERO:
        case JSGCM_REPLACE:
        case JSGCM_INCR:
        case JSGCM_DECR:
        case JSGCM_INVERT:
        case JSGCM_INCR_WRAP:
        case JSGCM_DECR_WRAP:
            break;
        default:
            JS_ASSERT( 0 ); // chip exception ahead!
            break;
    }

    if ( s->activeStencilFace )
    {
        GCM_FUNC( cellGcmSetBackStencilOp, fail, depthFail, depthPass );
    }
    else
    {
        GCM_FUNC( cellGcmSetStencilOp, fail, depthFail, depthPass );
    }
}


/* controls the writing of individual bits in the stencil planes
**
** notes:
**   - extension of glStencilMask (see spec for details)
**   - interacts with two-sided stencil/ActiveStencilFace
**   - the stencil reference value is masked by the max stencil
**     value possible
**   - initially, stencil write mask is all 1's
*/
void static inline _jsGcmFifoGlStencilMask( GLuint sm )
{
    jsGcmStencilState *s = &_jsGcmState.state.stencil;

    if ( s->activeStencilFace )
    {
        GCM_FUNC( cellGcmSetBackStencilMask, sm );

    }
    else
    {
        GCM_FUNC( cellGcmSetStencilMask, sm );
    }
}

#endif
