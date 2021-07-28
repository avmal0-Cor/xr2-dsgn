/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 *                Copyright (C) 2009 Sony Computer Entertainment Inc.
 *                                               All Rights Reserved.
 */

 
/*
 * Jetstream graphics API basic type definitions
 */

#include <PSGL/psgl.h>
#include <PSGL/private.h>
#include <PSGL/Utils.h>
#include <PSGL/PerfCounters.h>
#include <PSGL/DPM2.h>

#include <RasterInlines.h>

/** @addtogroup OpenGL
 * @{
 */

/**
 * @short Clear buffers to preset values.
 *
 * glClear() sets the bitplane area of the window to values previously
 * selected by glClearColor(), glClearDepthf(), and glClearStencil().
 *
 * The pixel ownership test, the scissor test, dithering, and the color buffer masks
 * affect the operation of glClear(). The scissor box bounds the cleared region.
 * Alpha function, blend function, logical operation, stenciling, texture mapping,
 * and depth-buffering are ignored by glClear().
 *
 * glClear() takes a single argument that is the bitwise OR of several values
 * indicating which buffer is to be cleared.
 *
 * The values are as follows:

 * <TABLE rules=all>
 * <TR>
 * <TD><CODE>GL_COLOR_BUFFER_BIT</CODE></TD>
 * <TD>Indicates the color buffer</TD>
 * </TR>
 * <TR>
 * <TD><CODE>GL_DEPTH_BUFFER_BIT</CODE></TD>
 * <TD>Indicates the depth buffer</TD>
 * </TR>
 * <TR>
 * <TD><CODE>GL_STENCIL_BUFFER_BIT</CODE></TD>
 * <TD>Indicates the stencil buffer</TD>
 * </TR>
 * </TABLE>
 *
 * The value to which each buffer is cleared depends on the setting of the clear value for that buffer.
 *
 * @param mask Bitwise OR of masks that indicate the buffers to be cleared.
 * Valid masks are <CODE>GL_COLOR_BUFFER_BIT</CODE>, <CODE>GL_DEPTH_BUFFER_BIT</CODE>,
 * and <CODE>GL_STENCIL_BUFFER_BIT</CODE>.
 *
 * @note
 * To use the fast clear supported by the hardware, the color format of the
buffer must be <code>GL_RGBA8</code>, the buffer must not be a multi-rendered texture, and the buffer must be in RSX&trade; memory.
@par
 * If a buffer is not present, then a glClear() directed at that buffer has no effect.
@par
 * When using a 16-bit depth buffer, <CODE>glClear</CODE> ignores the clearing stencil regardless of setting. 
 Stencil is only supported with 24-bit and 8-bit stencil buffers.
 *
 * @par Errors
Use glGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>GL_INVALID_VALUE</CODE></TD>
<TD>Any bit other than the defined bits is set in <I><c>mask</c></I>.</TD>
</TR>
<TR>
<TD><CODE>GL_INVALID_FRAMEBUFFER_OPERATION_OES</CODE></TD>
<TD>One or more of the following conditions is not true:
<UL>
<LI>All textures are complete.</LI>
<LI>Color formats are consistent across the color textures</LI>
<LI>Swizzling hint is consistent across all the textures</LI>
<LI>Drawable color format for the color attachments if any</LI>
<LI>Drawable depth format for the depth attachment if any</LI>
<LI>No anti-aliasing if FP16 or FP32 color format</LI>
<LI>No swizzling with RGBA/FP16 or RGBA/FP32 (any format that is more than
32 bits per pixel)</LI>
<LI>Matching sizes, if swizzled</LI>
</UL></TD>
</TR>
</TABLE>
 *
 * @sa glClearColor, glClearDepthf, glClearStencil, glColorMask, glEnable, glScissor
 */

GLAPI void APIENTRY glClear( GLbitfield mask )
{
    JS_APIENTRY_TIMER( glClear );
#ifdef JS_DPM
    if ( _jsdpmState ) { _JS_DPM_API_ENTRY_1( glClear, JSDPM_CALL_TYPE_STATE_RASTER, , 1, mask, sizeof( GLbitfield ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM_COMBINATION ); }
#endif
    PSGLcontext*	LContext = _CurrentContext;
#ifndef JS_NO_ERROR_CHECK
    if ( mask&~( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT ) )
    {
        _jsSetError( GL_INVALID_VALUE );
        return;
    }
#endif

    _JS_PERFORMANCE_START( ClearCounter );
    // validate framebuffer first. It may have to draw a quad to filter half float buffers,
    // that would in turn invalidate many states
    if ( LContext->needValidate & PSGL_VALIDATE_FRAMEBUFFER ) _jsValidateFramebuffer();
    if ( LContext->needValidate & PSGL_VALIDATE_WRITE_MASK ) _jsValidateWriteMask();
    if ( LContext->needValidate & PSGL_VALIDATE_DEPTH_TEST ) _jsValidateDepthTest();
    if ( LContext->needValidate & PSGL_VALIDATE_STENCIL_TEST ) _jsValidateStencilTest();
    if ( LContext->needValidate & PSGL_VALIDATE_SCISSOR_BOX ) _jsValidateScissorBox();
    if ( LContext->needValidate & PSGL_VALIDATE_STENCIL_OP_AND_MASK ) _jsValidateStencilOpAndMask();
    if ( LContext->needValidate & PSGL_VALIDATE_LINE_RASTER ) _jsValidateLineRaster();
    _jsFBClear( mask );
    _JS_PERFORMANCE_STOP( ClearCounter );

}

/**
 * @short Specify clear values for the color buffers
 *
 * glClearColor() specifies the red, green, blue, and alpha values used by
 * glClear() to clear the color buffer. The initial values are (0, 0, 0, 0).
 *
 * Values specified by glClearColor()
 * are clamped to the range [0, 1].
 *
 * @param red  Red value to use when the color buffer is cleared.
 * @param blue  Blue value to use when the color buffer is cleared.
 * @param green  Green value to use when the color buffer is cleared.
 * @param alpha  Alpha value to use when the color buffer is cleared.
 *
 * @sa glClear, glClearDepthf, glClearStencil, glColorMask
 */
GLAPI void APIENTRY glClearColor( GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha )
{
    JS_APIENTRY_TIMER( glClearColor );
#ifdef JS_DPM
    if ( _jsdpmState )
    {
        _JS_DPM_API_ENTRY_4( glClearColor, JSDPM_CALL_TYPE_STATE_RASTER, , 4,
                             red, sizeof( GLclampf ), INPUT_BY_VALUE, DPM_API_ARGUMENT_FLOAT_VALUE,
                             green, sizeof( GLclampf ), INPUT_BY_VALUE, DPM_API_ARGUMENT_FLOAT_VALUE,
                             blue, sizeof( GLclampf ), INPUT_BY_VALUE, DPM_API_ARGUMENT_FLOAT_VALUE,
                             alpha, sizeof( GLclampf ), INPUT_BY_VALUE, DPM_API_ARGUMENT_FLOAT_VALUE );
    }
#endif
    PSGLcontext*	LContext = _CurrentContext;
    JS_ASSERT( LContext );
    LContext->ClearColor.R = _jsClampf( red );
    LContext->ClearColor.G = _jsClampf( green );
    LContext->ClearColor.B = _jsClampf( blue );
    LContext->ClearColor.A = _jsClampf( alpha );
}

/**
 * @short Specify the clear value for the depth buffer
 *
 * glClearDepthf() specifies the depth value used by glClear() to clear the depth buffer.
 * Values specified by glClearDepthf() are clamped to the range [0, 1]. The initial value is 1.
 *
 * @param depth Specifies the depth value used when the depth buffer is cleared.
 *
 * @sa glClear, glClearColor, glClearStencil, glDepthFunc, glDepthMask
 */
GLAPI void APIENTRY glClearDepthf( GLclampf depth )
{
    JS_APIENTRY_TIMER( glClearDepthf );
#ifdef JS_DPM
    if ( _jsdpmState ) { _JS_DPM_API_ENTRY_1( glClearDepthf, JSDPM_CALL_TYPE_STATE_RASTER, , 1, depth, sizeof( GLclampf ), INPUT_BY_VALUE, DPM_API_ARGUMENT_FLOAT_VALUE ); }
#endif
    PSGLcontext*	LContext = _CurrentContext;
    JS_ASSERT( LContext );
    LContext->ClearDepth = _jsClampf( depth );
}

/**
 * @short Specify the clear value for the stencil buffer
 *
 * glClearStencil() specifies the index used by glClear() to clear the stencil buffer.
 * <I><c>value</c></I> is masked with 2<sup>m</sup> - 1, where <I>m</I> is the number of bits in the stencil buffer.
 * To examine the number of bitplanes in the stencil buffer, call glGetIntegerv() with argument <CODE>GL_STENCIL_BITS</CODE>.
 *
 * @param value Specifies the index used when the stencil buffer is cleared. The initial value is 0.
 *
 * @sa glClear, glClearColor, glClearDepthf, glGetIntegerv, glStencilFunc, glStencilOp, glStencilMask
 */
GLAPI void APIENTRY glClearStencil( GLint value )
{
    JS_APIENTRY_TIMER( glClearStencil );
#ifdef JS_DPM
    if ( _jsdpmState ) { _JS_DPM_API_ENTRY_1( glClearStencil, JSDPM_CALL_TYPE_STATE_RASTER, , 1, value, sizeof( GLint ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE ); }
#endif
    PSGLcontext*	LContext = _CurrentContext;
    JS_ASSERT( LContext );
    LContext->ClearStencil = value;
}

/**
 * @short Enable and disable writing of frame buffer color components
 *
 * glColorMask() specifies  whether  the individual color components in the
 * frame buffer can or cannot be written.  If <I><c>red</c></I> is <CODE>GL_FALSE</CODE>,  for  example,
 * no change is made to the red component of any pixel in any of the color
 * buffers, regardless of the drawing operation attempted, including glClear().
 *
 * Changes to individual bits of components cannot be controlled.
 * Rather, changes are either enabled or disabled for entire color components.
 *
 * The initial values are all <CODE>GL_TRUE</CODE>, indicating that
 * all color components can be written.
 *
 * @param red Specifies whether red can or cannot be written into the color buffer.
 * @param green Specifies whether green can or cannot be written into the color buffer.
 * @param blue Specifies whether blue can or cannot be written into the color buffer.
 * @param alpha Specifies whether alpha can or cannot be written into the color buffer.
 *
 * @sa glClear, glColor4fv, glColorPointer, glDepthMask, glStencilMask
 */
GLAPI void APIENTRY glColorMask( GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha )
{
    JS_APIENTRY_TIMER( glColorMask );
#ifdef JS_DPM
    if ( _jsdpmState )
    {
        _JS_DPM_API_ENTRY_4( glColorMask, JSDPM_CALL_TYPE_STATE_RASTER, , 4,
                             red, sizeof( GLboolean ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             green, sizeof( GLboolean ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             blue, sizeof( GLboolean ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             alpha, sizeof( GLboolean ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE );
    }
#endif
    PSGLcontext*	LContext = _CurrentContext;

    LContext->WriteMask &= ~_JS_CONTEXT_COLOR_MASK;
    LContext->WriteMask |= red ? _JS_CONTEXT_RED_MASK : 0;
    LContext->WriteMask |= green ? _JS_CONTEXT_GREEN_MASK : 0;
    LContext->WriteMask |= blue ? _JS_CONTEXT_BLUE_MASK : 0;
    LContext->WriteMask |= alpha ? _JS_CONTEXT_ALPHA_MASK : 0;
    LContext->needValidate |= PSGL_VALIDATE_WRITE_MASK | PSGL_VALIDATE_FRAGMENT_PROGRAM;
}

/**
 * @short Enable or disable writing into the depth buffer
 *
 * glDepthMask() specifies whether the depth buffer is enabled for writing.
 * If flag is <CODE>GL_FALSE</CODE>, depth buffer writing is disabled. Otherwise, it is enabled.
 * Initially, depth buffer writing is enabled.
 *
 * @param flag Specifies whether the depth buffer is enabled for writing. If
 *             <I><c>flag</c></I> is <CODE>GL_FALSE</CODE>, depth buffer writing is disabled. Otherwise,
 *             it is enabled. Initially, depth buffer writing is enabled.
 *
 * @note glDepthMask() does not affect glClear()
 *
 * @sa glClear, glColorMask, glDepthFunc, glDepthRangef, glStencilMask
 */
GLAPI void APIENTRY glDepthMask( GLboolean flag )
{
    JS_APIENTRY_TIMER( glDepthMask );
#ifdef JS_DPM
    if ( _jsdpmState ) { _JS_DPM_API_ENTRY_1( glDepthMask, JSDPM_CALL_TYPE_STATE_RASTER, , 1, flag, sizeof( GLboolean ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE ); }
#endif
    PSGLcontext*	LContext = _CurrentContext;
    JS_ASSERT( LContext );

    LContext->WriteMask &= ~_JS_CONTEXT_DEPTH_MASK;
    LContext->WriteMask |= flag ? _JS_CONTEXT_DEPTH_MASK : 0;
    LContext->needValidate |= PSGL_VALIDATE_WRITE_MASK;
}

/**
 * @short Control the writing of individual bits in the stencil planes
 *
 * glStencilMask() controls the writing of individual bits in the stencil
 * planes. The least significant n bits of <I><c>mask</c></I>, where n is the number of
 * bits in the stencil buffer, specify a mask. Where a 1 appears in the
 * mask, it&rsquo;s possible to write to the corresponding bit in the stencil
 * buffer. Where a 0 appears, the corresponding bit is write-protected.
 * Initially, all bits are enabled for writing. To examine the number of bitplanes
 * in the stencil buffer, call glGetIntegerv() with argument <CODE>GL_STENCIL_BITS</CODE>.
 *
 * @param mask  Specifies a bit mask to enable and disable writing of individual
 *        bits in the stencil planes. Initially, the mask is all 1&rsquo;s.
 *
 * @sa glClear, glColorMask, glDepthMask, glGetIntegerv, glStencilFunc, glStencilOp
 */
GLAPI void APIENTRY glStencilMask( GLuint mask )
{
    JS_APIENTRY_TIMER( glStencilMask );
#ifdef JS_DPM
    if ( _jsdpmState ) { _JS_DPM_API_ENTRY_1( glStencilMask, JSDPM_CALL_TYPE_STATE_RASTER, , 1, mask, sizeof( GLuint ), INPUT_BY_VALUE, DPM_API_ARGUMENT_FLOAT_VALUE ); }
#endif
    PSGLcontext*	LContext = _CurrentContext;
    JS_ASSERT( LContext );

    switch ( LContext->ActiveStencilFace )
    {
        case GL_FRONT:
            LContext->StencilWriteMask = mask;
            break;
        case GL_BACK:
            LContext->StencilBackfaceWriteMask = mask;
            break;
        default:
            JS_ASSERT( 0 );
    }
    LContext->needValidate |= PSGL_VALIDATE_STENCIL_OP_AND_MASK;
}

/** @} OpenGL */

/** @addtogroup OpenGL
 *
 * @{
 */

/**
 * @short Set stencil test actions
 *
 * Stenciling, like depth-buffering, enables and disables drawing on a
 * per-pixel basis. You draw into the stencil planes using GL drawing
 * primitives, then render geometry and images, using the stencil planes
 * to mask out portions of the screen. Stenciling is typically used in
 * multipass rendering algorithms to achieve special effects, such as
 * decals, outlining, and constructive solid geometry rendering.
 *
 * The stencil test conditionally eliminates a pixel based on the outcome
 * of a comparison between the value in the stencil buffer and a reference
 * value. To enable and disable the test, call glEnable() and glDisable() with
 * argument <CODE>GL_STENCIL_TEST</CODE>; to control it, call glStencilFunc().
 *
 * glStencilOp() takes three arguments that indicate what happens to the
 * stored stencil value while stenciling is enabled. If the stencil test
 * fails, no change is made to the pixel&rsquo;s color or depth buffers, and
 * <I><c>fail</c></I> specifies what happens to the stencil buffer contents. The following
 * six actions are possible.
 *
 * <TABLE rules=all>
 * <TR><TD>	<CODE>GL_KEEP</CODE>  	</TD><TD>	Keeps the current value.	</TD></TR>
 * <TR><TD>	<CODE>GL_ZERO</CODE>  	</TD><TD>	Sets the stencil buffer value to 0.	</TD></TR>
 * <TR><TD>	<CODE>GL_REPLACE</CODE>	</TD>
 *     <TD>	Sets the stencil buffer value to <I><c>ref</c></I>, as specified by glStencilFunc().	</TD></TR>
 * <TR><TD>	<CODE>GL_INCR</CODE> 	</TD>
 *     <TD>	Increments the current stencil buffer value. Clamps to the maximum representable unsigned value.	</TD></TR>
 * <TR><TD>	<CODE>GL_INCR_WRAP</CODE> </TD>
 *     <TD>	Increments the current stencil buffer value. Wraps stencil buffer value to 0 when incrementing the maximum representable unsigned value.	</TD></TR>
 * <TR><TD>	<CODE>GL_DECR</CODE>  	</TD><TD>	Decrements the current stencil buffer value. Clamps to 0.	</TD></TR>
 * <TR><TD>	<CODE>GL_DECR_WRAP</CODE> </TD><TD>	Decrements the current stencil buffer value. Wraps stencil buffer value to the maximum representable unsigned value when decrementing 0.	</TD></TR>
 * <TR><TD>	<CODE>GL_INVERT</CODE>	</TD><TD>	Bitwise inverts the current stencil buffer value.	</TD></TR>
 * </TABLE>
 *
 * Stencil buffer values are treated as unsigned integers. When incremented and
 * decremented, values are clamped to 0 and (2<sup>n</sup>)-1, where n is the value
 * returned by querying <CODE>GL_STENCIL_BITS</CODE>.
 *
 * The other two arguments to glStencilOp() specify stencil buffer actions
 * that depend on whether subsequent depth buffer tests succeed (<I><c>zpass</c></I>) or
 * fail (<I><c>zfail</c></I>) (see glDepthFunc()). The actions are specified using the
 * same six symbolic constants as <I>fail</I>. Note that <I><c>zfail</c></I> is ignored when
 * there is no depth buffer, or when the depth buffer is not enabled. In
 * these cases, <I><c>fail</c></I> and <I><c>zpass</c></I> specify stencil action when the stencil test fails and
 * passes, respectively.
 *
 * @param fail Specifies the action to take when the stencil test fails. Six
 *        symbolic constants are accepted, defined in the table below. The
 *        initial value is <CODE>GL_KEEP</CODE>.
 * @param zfail Specifies the stencil action when the stencil test passes,
 *        but the depth test fails. <I><c>zfail</c></I> accepts the same symbolic constants
 *        as <I>fail</I>. The initial value is <CODE>GL_KEEP</CODE>.
 * @param zpass Specifies the stencil action when both the stencil test and
 *        the depth test pass, or when the stencil test passes and either
 *        there is no depth buffer or depth testing is not enabled. <I><c>zpass</c></I>
 *        accepts the same symbolic constants as <I><c>fail</c></I>. The initial value
 *        is <CODE>GL_KEEP</CODE>.
 *
 * @par  Errors
Use glGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>GL_INVALID_ENUM</CODE></TD>
<TD><I><c>fail</c></I>, <I><c>zfail</c></I>, or <I><c>zpass</c></I> are any value other than the six defined constant values.</TD>
</TR>
</TABLE>
 *
 * @sa
 * glAlphaFunc, glBlendFunc, glDepthFunc, glEnable, glGetIntegerv, glLogicOp, glStencilFunc
 */
GLAPI void APIENTRY glStencilOp( GLenum fail, GLenum zfail, GLenum zpass )
{
    JS_APIENTRY_TIMER( glStencilOp );
#ifdef JS_DPM
    if ( _jsdpmState )
    {
        _JS_DPM_API_ENTRY_3( glStencilOp, JSDPM_CALL_TYPE_STATE_RASTER, , 3,
                             fail, sizeof( GLenum ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM,
                             zfail, sizeof( GLenum ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM,
                             zpass, sizeof( GLenum ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM );
    }
#endif
    PSGLcontext*	LContext = _CurrentContext;
    JS_ASSERT( LContext );

#ifndef JS_NO_ERROR_CHECK
    if (( !_jsIsStencilOp( fail ) ) || ( !_jsIsStencilOp( zfail ) ) || ( !_jsIsStencilOp( zpass ) ) )
    {
        _jsSetError( GL_INVALID_ENUM );
        return;
    }
#endif

    switch ( LContext->ActiveStencilFace )
    {
        case GL_FRONT:
            LContext->StencilOpFail = fail;
            LContext->StencilOpZFail = zfail;
            LContext->StencilOpZPass = zpass;
            break;
        case GL_BACK:
            LContext->StencilBackfaceOpFail = fail;
            LContext->StencilBackfaceOpZFail = zfail;
            LContext->StencilBackfaceOpZPass = zpass;
            break;
        default:
            JS_ASSERT( 0 );
    }
    LContext->needValidate |= PSGL_VALIDATE_STENCIL_OP_AND_MASK;
}

/**
 * @short Set the blend equation
 *
 * glBlendEquation() establishes the blend equation, which determines how a new pixel (the source color) is combined with a pixel already in the frame buffer (the destination color), yielding a result color.

 * The <I><c>mode</c></I> parameter is an enumerant specifying the blend equation:

 * <CODE>GL_MIN: </CODE> The blend equation in which each component of the result color consists of the minimum of the corresponding components of the source and destination colors.

 * <CODE>GL_MAX: </CODE> The blend equation in which each component of the result color consists of the maximum of the corresponding components of the source and destination colors.

* The possible blend equations other than <CODE>GL_MIN</CODE> and <CODE>GL_MAX</CODE> use the
* source and destination blend factors specified by glBlendFunc().
* See glBlendFunc() for descriptions of the blend factors.

* In the equations that follow, source and destination color components
* are referred to as ( Rs, Gs, Bs, As ) and ( Rd, Gd, Bd, Ad ). The
* result color is referred to as ( Rr, Gr, Br, Ar ).  Source and
* destination blend factors are referred to as ( sR, sG, sB, sA ) and (
* dR, dG, dB, dA ).  All of these color components are understood to
* have values in the range [0, 1].

<TABLE>
<TR>
<TD><CODE>GL_FUNC_ADD</CODE></TD>
<TD>The blend equation in which the source and destination data are added.  Each component is multiplied by its
corresponding blend factor, the corresponding source and destination products are added, and the result is clamped to the range [0, 1]:

Rr = min(1, Rs sR + Rd dR)

Gr = min(1, Gs sG + Gd dG)

Br = min(1, Bs sB + Bd dB)

Ar = min(1, As sA + Ad dA)
</TD>
</TR>
<TR>
<TD><CODE>GL_FUNC_SUBTRACT</CODE></TD>
<TD>The blend equation analogous to <CODE>GL_FUNC_ADD</CODE>, except that the product of the destination factor and the destination color are  subtracted from the product of the source factor and the source color:

Rr = min(1, Rs sR - Rd dR)

Gr = min(1, Gs sG - Gd dG)

Br = min(1, Bs sB - Bd dB)

Ar = min(1, As sA - Ad dA)
</TD>
</TR>
<TR>
<TD><CODE>GL_FUNC_REVERSE_SUBTRACT</CODE></TD>
<TD>The blend equation analogous to <CODE>GL_FUNC_ADD</CODE>, except that the product of the source factor and the source color are
subtracted from the product of the destination factor and the destination color:

Rr = min(1, Rd dR - Rs sR)

Gr = min(1, Gd dG - Gs sG)

Br = min(1, Bd dB - Bs sB)

Ar = min(1, Ad dA - As sA)
</TD>
</TR>
</TABLE>
 * @param mode Specifies how the source and destination colors are combined to produce the result color. <I><c>mode</c></I> must be one of  <CODE>GL_FUNC_ADD</code>, <CODE>GL_FUNC_SUBTRACT</code>,  <CODE>GL_FUNC_REVERSE_SUBTRACT</code>, <CODE>GL_MIN</code>, or <CODE>GL_MAX</code>.
 *
 * @note
 * The <CODE>GL_MIN</code> and <CODE>GL_MAX</code> equations do not use the source or destination blend factors; they use only the source and destination colors.
 * @par Errors
Use glGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>GL_INVALID_ENUM</CODE></TD>
<TD><I><c>mode</c></I> is not one of the five valid blend equations.</TD>
</TR>
</TABLE>
 *
 * @sa glGetString, glBlendColor, glBlendFunc, glBlendFuncSeparate
 */
GLAPI void APIENTRY glBlendEquation( GLenum mode )
{
    JS_APIENTRY_TIMER( glBlendEquation );
#ifdef JS_DPM
    if ( _jsdpmState )
        _JS_DPM_API_ENTRY_1( glBlendEquation, JSDPM_CALL_TYPE_STATE_RASTER, , 1, mode, sizeof( GLenum ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM );
#endif
#ifndef JS_NO_ERROR_CHECK
    if ( !_jsIsBlendEquation( mode ) )
    {
        _jsSetError( GL_INVALID_ENUM );
        return;
    }
#endif

    PSGLcontext*	LContext = _CurrentContext;
    JS_ASSERT( LContext != NULL );

    LContext->BlendEquationRGB = LContext->BlendEquationAlpha = mode;
    LContext->needValidate |= PSGL_VALIDATE_BLENDING;
}

/**
 * @short Set the blend equation for RGB and alpha components separately
 *
 * glBlendEquationSeparate() establishes the blend equation, which
 * determines how a new pixel (the source) is combined with a pixel
 * already in the frame buffer (the destination), yielding a result.
 * For more details, see glBlendEquation().

 * The <I><c>modeRGB</c></I> parameter is an enumerant specifying the blend
 * equation for the RGB-color components.
 * The <I><c>modeAlpha</c></I> parameter is an enumerant specifying the blend
 * equation for the alpha components.

 * <CODE>GL_MIN: </CODE> The blend equation in which each component of
 * the result consists of the minimum of the corresponding components
 * of the source and destination.

 * <CODE>GL_MAX: </CODE> The blend equation in which each component of
 * the result color consists of the maximum of the corresponding
 * components of the source and destination.

* The possible blend equations other than <c>GL_MIN</c> and <c>GL_MAX</c> use the
* source and destination blend factors specified by
* glBlendFunc().  See glBlendFunc() for descriptions of the
* blend factors.

* In the equations that follow, source and destination color
* components are referred to as ( Rs, Gs, Bs ) and ( Rd, Gd, Bd ). The
* result color is referred to as ( Rr, Gr, Br ).  Source and
* destination alpha components are referred to as As and Ad. The
* result alpha component is referred to as Ar.  Source and
* destination blend factors for RGB-color are referred to as ( sR, sG,
* sB ) and ( dR, dG, dB ).  Source and destination blend factors for
* alpha are referred to as sA and dA.

* All of these color components are understood to have values
* in the range [0, 1].

<TABLE>
<TR>
<TD><CODE>GL_FUNC_ADD</CODE></TD>
<TD>The blend equation in which the source
and destination data are added.  Each component is multiplied by its
corresponding blend factor, the corresponding source and destination
products are added, and the result is clamped to the range [0, 1]:

Rr = min(1, Rs sR + Rd dR) (for <I><c>modeRGB</c></I>)

Gr = min(1, Gs sG + Gd dG) (for <I><c>modeRGB</c></I>)

Br = min(1, Bs sB + Bd dB) (for <I><c>modeRGB</c></I>)

Ar = min(1, As sA + Ad dA) (for <I><c>modeAlpha</c></I>)
</TD>
</TR>
<TR>
<TD><CODE>GL_FUNC_SUBTRACT</CODE></TD>
<TD>The blend equation analogous to
 * <CODE>GL_FUNC_ADD</CODE>, except that the product of the
 * destination factor and the destination color are subtracted from
 * the product of the source factor and the source color:

Rr = min(1, Rs sR - Rd dR) (for <I><c>modeRGB</c></I>)

Gr = min(1, Gs sG - Gd dG) (for <I><c>modeRGB</c></I>)

Br = min(1, Bs sB - Bd dB) (for <I><c>modeRGB</c></I>)

Ar = min(1, As sA - Ad dA) (for <I><c>modeAlpha</c></I>)
</TD>
</TR>
<TR>
<TD><CODE>GL_FUNC_REVERSE_SUBTRACT</CODE></TD>
<TD>The blend equation
 * analogous to <CODE>GL_FUNC_ADD</CODE>, except that the product of
 * the source factor and the source color are subtracted from the
 * product of the destination factor and the destination color:

Rr = min(1, Rd dR - Rs sR) (for <I>modeRGB</I>)

Gr = min(1, Gd dG - Gs sG) (for <I>modeRGB</I>)

Br = min(1, Bd dB - Bs sB) (for <I>modeRGB</I>)

Ar = min(1, Ad dA - As sA) (for <I>modeAlpha</I>)
</TD>
</TR>
</TABLE>
 * @param modeRGB Specifies how the RGB-color components of the source
 * and destination colors are combined to produce the result
 * color. <I><c>modeRGB</c></I> must be one of <CODE>GL_FUNC_ADD</code>,
 * <CODE>GL_FUNC_SUBTRACT</code>,
 * <CODE>GL_FUNC_REVERSE_SUBTRACT</code>, <CODE>GL_MIN</code>, or
 * <CODE>GL_MAX</code>.
 * @param modeAlpha Specifies how the alpha components of the source
 * and destination colors are combined to produce the result
 * color. <I><c>modeAlpha</c></I> must be one of <CODE>GL_FUNC_ADD</code>,
 * <CODE>GL_FUNC_SUBTRACT</code>,
 * <CODE>GL_FUNC_REVERSE_SUBTRACT</code>, <CODE>GL_MIN</code>, or
 * <CODE>GL_MAX</code>.

 * @note
 * The <CODE>GL_MIN</code> and <CODE>GL_MAX</code> equations do
 * not use the source or destination blend factors; they use only the
 * source and destination colors.

 * @par Errors
Use glGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>GL_INVALID_ENUM</CODE></TD>
<TD><I><c>modeRGB</c></I> or  <I><c>modeAlpha</c></I> is not one of the five valid blend
equations. </TD>
</TR>
</TABLE>
 *
 * @sa glGetString, glBlendColor, glBlendEquation, glBlendFunc, glBlendFuncSeparate
 */
GLAPI void APIENTRY glBlendEquationSeparate( GLenum modeRGB, GLenum modeAlpha )
{
    JS_APIENTRY_TIMER( glBlendEquationSeparate );
#ifdef JS_DPM
    if ( _jsdpmState )
        _JS_DPM_API_ENTRY_2( glBlendEquationSeparate, JSDPM_CALL_TYPE_STATE_RASTER, , 2,
                             modeRGB, sizeof( GLenum ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM,
                             modeAlpha, sizeof( GLenum ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM );
#endif
#ifndef JS_NO_ERROR_CHECK
    if (( !_jsIsBlendEquation( modeRGB ) ) || ( !_jsIsBlendEquation( modeAlpha ) ) )
    {
        _jsSetError( GL_INVALID_ENUM );
        return;
    }
#endif

    PSGLcontext*	LContext = _CurrentContext;
    JS_ASSERT( LContext != NULL );

    LContext->BlendEquationRGB = modeRGB;
    LContext->BlendEquationAlpha = modeAlpha;
    LContext->needValidate |= PSGL_VALIDATE_BLENDING;
}

/**
  @short Set the blend color

  glBlendColor() calculates the source and destination blending
  factors and then clamps them to [0, 1] before storing them to
  <CODE>GL_BLEND_COLOR</CODE>.  See glBlendFunc() for a complete
  description of blending operations.

  @param red specifies the red component of <C>GL_BLEND_COLOR</C>
  @param green specifies the green component of <C>GL_BLEND_COLOR</C>
  @param blue specifies the blue component of <C>GL_BLEND_COLOR</C>
  @param alpha specifies the alpha component of <C>GL_BLEND_COLOR</C>

  @sa glBlendEquation, glBlendFunc, glBlendFuncSeparate, glGetString
 */

GLAPI void APIENTRY glBlendColor( GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha )
{
    JS_APIENTRY_TIMER( glBlendColor );
#ifdef JS_DPM
    if ( _jsdpmState )
        _JS_DPM_API_ENTRY_4( glBlendColor, JSDPM_CALL_TYPE_STATE_RASTER, , 4,
                             red, sizeof( GLclampf ), INPUT_BY_VALUE, DPM_API_ARGUMENT_FLOAT_VALUE,
                             green, sizeof( GLclampf ), INPUT_BY_VALUE, DPM_API_ARGUMENT_FLOAT_VALUE,
                             blue, sizeof( GLclampf ), INPUT_BY_VALUE, DPM_API_ARGUMENT_FLOAT_VALUE,
                             alpha, sizeof( GLclampf ), INPUT_BY_VALUE, DPM_API_ARGUMENT_FLOAT_VALUE );
#endif

    PSGLcontext*	LContext = _CurrentContext;
    JS_ASSERT( LContext );
    LContext->BlendColor.R = _jsClampf( red );
    LContext->BlendColor.G = _jsClampf( green );
    LContext->BlendColor.B = _jsClampf( blue );
    LContext->BlendColor.A = _jsClampf( alpha );

    LContext->needValidate |= PSGL_VALIDATE_BLENDING;
}

/**
 * @short Specify pixel arithmetic
 *
 * Pixels can be drawn using a function that blends the incoming (source) values with the values that are already in the color buffer (the destination values). Use glEnable() and glDisable() with argument <CODE>GL_BLEND</CODE> to enable and disable blending. Blending is initially disabled.
 *
 * When using blending with multiple render targets (MRTs), the <c>GL_BLEND</c> argument applies 
 * to all active render targets. If you require separate enable control per render target, 
 * use the <c>GL_BLEND_MRT<i>i</i>_SCE</c> argument (where <i>i</i> is 0, 1, 2, or 3).
 *
 * glBlendFunc() defines the operation of blending when it is enabled. <I><c>sfactor</c></I> specifies which of nine methods is used to scale the source color components. <I><c>dfactor</c></I> specifies which of eight methods is used to scale the destination color components. The eleven possible methods are described in the following table. Each method defines four scale factors, one each for red, green, blue, and alpha.
 *
In the table and in subsequent equations, source and destination color components are referred to as ( Rs, Gs, Bs, As ) and ( Rd, Gd, Bd, Ad ). They are understood to have integer values between 0 and ( kR, kG, kB, kA ), where
 *
 * kC = (2<sup>mC</sup>) - 1, where mC is the number of color bitplanes.
 *
 * and ( mR, mG, mB, mA ) is the number of red, green, blue, and alpha bitplanes.
 *
 * Source and destination scale factors are referred to as ( sR, sG, sB, sA ) and ( dR, dG, dB, dA ).  (Rc, Gc, Bc, Ac) represents the
 * values set by glBlendColor().  The color components are understood to have integer values between 0
 * and ( kR, kG, kB, kA ) where <code>kC = (2<sup>mC</sup>) - 1</code>,
 * where <code>mC</code> is the number of color bitplanes and ( mR, mG, mB, mA )
 * is the number of red, green, blue, and alpha bitplanes.
 *
 * The scale factors described in the table, denoted ( fR, fG, fB, fA ), represent either source or destination factors.
 * All scale factors have range [0, 1].

 * <TABLE rules=all>
 * <TR>
 * <TD>Parameter</TD>
 * <TD>( fR, fG, fB, fA )</TD>
 * </TR>
 * <TR>
 * <TD><CODE>GL_ZERO</CODE></TD>
 * <TD>( 0, 0, 0, 0 )</TD>
 * </TR>
 * <TR>
 * <TD><CODE>GL_ONE</CODE></TD>
 * <TD>( 1, 1, 1, 1 )</TD>
 * </TR>
 * <TR>
 * <TD><CODE>GL_SRC_COLOR</CODE></TD>
 * <TD>( Rs / kR, Gs / kG, Bs / kB, As / kA )</TD>
 * </TR>
 * <TR>
 * <TD><CODE>GL_ONE_MINUS_SRC_COLOR</CODE></TD>
 * <TD>( 1, 1, 1, 1 ) - ( Rs / kR, Gs / kG, Bs / kB, As / kA )</TD>
 * </TR>
 * <TR>
 * <TD><CODE>GL_DST_COLOR</CODE></TD>
 * <TD>( Rd / kR, Gd / kG, Bd / kB, Ad / kA )</TD>
 * </TR>
 * <TR>
 * <TD><CODE>GL_ONE_MINUS_DST_COLOR</CODE></TD>
 * <TD>( 1, 1, 1, 1 ) - ( Rd / kR, Gd / kG, Bd / kB, Ad / kA )</TD>
 * </TR>
 * <TR>
 * <TD><CODE>GL_SRC_ALPHA</CODE> </TD>
 * <TD>( As / kA, As / kA, As / kA, As / kA )</TD>
 * </TR>
 * <TR>
 * <TD><CODE>GL_ONE_MINUS_SRC_ALPHA</CODE></TD>
 * <TD>( 1, 1, 1, 1 ) - ( As / kA, As / kA, As / kA, As / kA )</TD>
 * </TR>
 * <TR>
 * <TD><CODE>GL_DST_ALPHA</CODE></TD>
 * <TD>( Ad / kA, Ad / kA, Ad / kA, Ad / kA )</TD>
 * </TR>
 * <TR>
 * <TD><CODE>GL_ONE_MINUS_DST_ALPHA</CODE></TD>
 * <TD>( 1, 1, 1, 1 ) - ( Ad / kA, Ad / kA, Ad / kA, Ad / kA )</TD>
 * </TR>
 * <TR>
 * <TD><CODE>GL_CONSTANT_COLOR</CODE></TD>
 * <TD>(Rc, Gc, Bc, Ac)</TD>
 * </TR>
 * <TR>
 * <TD><CODE>GL_ONE_MINUS_CONSTANT_COLOR</CODE></TD>
 * <TD>(1, 1, 1, 1) - (Rc, Gc, Bc, Ac)</TD>
 * </TR>
 * <TR>
 * <TD><CODE>GL_CONSTANT_ALPHA</CODE></TD>
 * <TD>(Ac, Ac, Ac, Ac)</TD>
 * </TR>
 * <TR>
 * <TD><CODE>GL_ONE_MINUS_CONSTANT_ALPHA</CODE></TD>
 * <TD>(1, 1, 1, 1) - (Ac, Ac, Ac, Ac)</TD>
 * </TR>
 * <TR>
 * <TD><CODE>GL_SRC_ALPHA_SATURATE</CODE></TD>
 * <TD>( i, i, i, 1 )</TD>
 * </TR>
 * </TABLE>
 *
 * In the table,
 *
i = min( As, kA - Ad ) / kA
 *
 * To determine the blended values of a pixel, the system uses the following equations:
<ul>
<li>Rd = min( kR, Rs sR + Rd dR )</li>
<li>Gd = min( kG, Gs sG + Gd dG )</li>
<li>Bd = min( kB, Bs sB + Bd dB )</li>
<li>Ad = min( kA, As sA + Ad dA )</li>
</ul>
 *
 * Despite the apparent precision of the above equations, blending arithmetic is not exactly specified, because blending operates with imprecise integer color values. However, a blend factor that should be equal to 1 is guaranteed not to modify its multiplicand, and a blend factor equal to 0 reduces its multiplicand to 0. For example, when <I><c>sfactor</c></I> is <CODE>GL_SRC_ALPHA</CODE>, <I><c>dfactor</c></I> is <CODE>GL_ONE_MINUS_SRC_ALPHA</CODE>, and As is equal to kA, the equations reduce to simple replacement:
<ul>
<li>Rd = Rs</li>
<li>Gd = Gs</li>
<li>Bd = Bs</li>
<li>Ad = As</li>
</ul>

glBlendFunc() operates on all pixel write operations, including the scan conversion of points, lines, and polygons.
glBlendFunc() does not affect glClear().
 *
 * @param sfactor Specifies how the red, green, blue, and alpha source
    blending factors are computed. The following symbolic constants
    are accepted: <CODE>GL_ZERO</CODE>, <CODE>GL_ONE</CODE>,
    <CODE>GL_DST_COLOR</CODE>, <CODE>GL_ONE_MINUS_DST_COLOR</CODE>,
    <CODE>GL_SRC_ALPHA</CODE>, <CODE>GL_ONE_MINUS_SRC_ALPHA</CODE>,
    <CODE>GL_DST_ALPHA</CODE>, <CODE>GL_ONE_MINUS_DST_ALPHA</CODE>,
    <CODE>GL_CONSTANT_COLOR,</CODE>
    <CODE>GL_ONE_MINUS_CONSTANT_COLOR,</CODE>
    <CODE>GL_CONSTANT_ALPHA</CODE>,
    <CODE>GL_ONE_MINUS_CONSTANT_ALPHA</CODE>, and
    <CODE>GL_SRC_ALPHA_SATURATE</CODE>.  The initial value is
    <CODE>GL_ONE</CODE>.

* @param dfactor Specifies how the red, green, blue, and alpha
   destination blending factors are computed.  The following
   constants are accepted: <CODE>GL_ZERO</CODE>, <CODE>GL_ONE</CODE>,
   <CODE>GL_SRC_COLOR</CODE>, <CODE>GL_ONE_MINUS_SRC_COLOR</CODE>,
   <CODE>GL_SRC_ALPHA</CODE>, <CODE>GL_ONE_MINUS_SRC_ALPHA</CODE>,
   <CODE>GL_DST_ALPHA</CODE>, <CODE>GL_ONE_MINUS_DST_ALPHA</CODE>,
   <CODE>GL_CONSTANT_COLOR,</CODE>
   <CODE>GL_ONE_MINUS_CONSTANT_COLOR,</CODE>
   <CODE>GL_CONSTANT_ALPHA</CODE>, and
   <CODE>GL_ONE_MINUS_CONSTANT_ALPHA</CODE>.  The initial value is
   <CODE>GL_ZERO</CODE>.
 *
 * @note
 * Incoming (source) alpha is correctly thought of as a material opacity, ranging from 1.0 (kA),
 * representing complete opacity, to 0.0 (0), representing complete transparency.
 * @par
 * Transparency is best implemented using <CODE>glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA)</CODE>
 * with primitives sorted from farthest to nearest. Note that this transparency calculation does not require
 * the presence of alpha bitplanes in the color buffer.
 * @par
 * <CODE>glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA)</CODE> is also useful for rendering antialiased points and lines.
 *
 * @par Errors
Use glGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>GL_INVALID_ENUM</CODE></TD>
<TD>Either <I><c>sfactor</c></I> or <I><c>dfactor</c></I> is not an accepted value.</TD>
</TR>
</TABLE>
 *
 * @sa glAlphaFunc, glBlendColor, glBlendFuncSeparate, glClear, glDepthFunc, glEnable, glLogicOp, glStencilFunc
 */

GLAPI void APIENTRY glBlendFunc( GLenum sfactor, GLenum dfactor )
{
    JS_APIENTRY_TIMER( glBlendFunc );
#ifdef JS_DPM
    if ( _jsdpmState )
    {
        _JS_DPM_API_ENTRY_2( glBlendFunc, JSDPM_CALL_TYPE_STATE_RASTER, , 2,
                             sfactor, sizeof( GLenum ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM,
                             dfactor, sizeof( GLenum ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM );
    }
#endif
    PSGLcontext*	LContext = _CurrentContext;

#ifndef JS_NO_ERROR_CHECK
    if (( !_jsIsSrcBlendFunc( sfactor ) ) || ( !_jsIsDstBlendFunc( dfactor ) ) )
    {
        _jsSetError( GL_INVALID_ENUM );
        return;
    }
#endif

    LContext->BlendFactorSrcRGB = sfactor;
    LContext->BlendFactorSrcAlpha = sfactor;
    LContext->BlendFactorDestRGB = dfactor;
    LContext->BlendFactorDestAlpha = dfactor;
    LContext->needValidate |= PSGL_VALIDATE_BLENDING;
}

/**
 * @short Specify pixel arithmetic for RGB and alpha components separately

 * Pixels can be drawn using a function that blends the incoming
 * (source) values with the values that are already in the color
 * buffer (the destination values). Use glEnable() and glDisable()
 * with argument <CODE>GL_BLEND</CODE> to enable and disable
 * blending.

 * glBlendFuncSeparate() defines the operation of blending when it is
 * enabled.  <I><c>sfactorRGB</c></I> specifies which method is used to scale
 * the source RGB-color components. <I><c>dfactorRGB</c></I> specifies which
 * method is used to scale the destination RGB-color components.
 * Likewise, <I><c>sfactorAlpha</c></I> specifies which method is used to
 * scale the source alpha component, and <I><c>dfactorAlpha</c></I> specifies
 * which method is used to scale the destination alpha component.
 * The possible methods are described in the following table.
 * Each method defines four scale factors, one each for red, green,
 * blue, and alpha.

 * In the table and in subsequent equations, source and destination
 * color components are referred to as ( Rs, Gs, Bs, As ) and ( Rd,
 * Gd, Bd, Ad ).  The color specified by glBlendColor() is referred to
 * as ( Rc, Gc, Bc, Ac ).   The color components are understood to have integer values between 0
 * and ( kR, kG, kB, kA ), where <code>kC = (2<sup>mC</sup>) - 1</code>,
 * Where <code>mC</code> is the number of color bitplanes and ( mR, mG, mB, mA )
 * is the number of red, green, blue, and alpha bitplanes.
 *
 * Source and destination scale factors are referred to as ( sR, sG, sB, sA ) and ( dR, dG, dB, dA ).
 * All scale factors have range [0, 1].

 * <TABLE rules=all>
 * <TR>
 * <TD>Parameter</TD>
 * <TD>RGB Factor</TD>
 * <TD>Alpha Factor</TD>
 * </TR>
 * <TR>
 * <TD><CODE>GL_ZERO</CODE></TD>
 * <TD>( 0, 0, 0 )</TD>
 * <TD>0</TD>
 * </TR>
 * <TR>
 * <TD><CODE>GL_ONE</CODE></TD>
 * <TD>( 1, 1, 1 )</TD>
 * <TD>1</TD>
 * </TR>
 * <TR>
 * <TD><CODE>GL_SRC_COLOR</CODE></TD>
 * <TD>( Rs / kR, Gs / kG, Bs / kB )</TD>
 * <TD>As / kA</TD>
 * </TR>
 * <TR>
 * <TD><CODE>GL_ONE_MINUS_SRC_COLOR</CODE></TD>
 * <TD>( 1, 1, 1 ) - ( Rs / kR, Gs / kG, Bs / kB )</TD>
 * <TD>1 - As / kA</TD>
 * </TR>
 * <TR>
 * <TD><CODE>GL_DST_COLOR</CODE></TD>
 * <TD>( Rd / kR, Gd / kG, Bd / kB )</TD>
 * <TD>Ad / kA</TD>
 * </TR>
 * <TR>
 * <TD><CODE>GL_ONE_MINUS_DST_COLOR</CODE></TD>
 * <TD>( 1, 1, 1 ) - ( Rd / kR, Gd / kG, Bd / kB )</TD>
 * <TD>1 - Ad / kA</TD>
 * </TR>
 * <TR>
 * <TD><CODE>GL_SRC_ALPHA</CODE> </TD>
 * <TD>( As / kA, As / kA, As / kA )</TD>
 * <TD>As / kA</TD>
 * </TR>
 * <TR>
 * <TD><CODE>GL_ONE_MINUS_SRC_ALPHA</CODE></TD>
 * <TD>( 1, 1, 1 ) - ( As / kA, As / kA, As / kA )</TD>
ppp * <TD>1 - As / kA</TD>
 * </TR>
 * <TR>
 * <TD><CODE>GL_DST_ALPHA</CODE></TD>
 * <TD>( Ad / kA, Ad / kA, Ad / kA )</TD>
 * <TD>Ad / kA</TD>
 * </TR>
 * <TR>
 * <TD><CODE>GL_ONE_MINUS_DST_ALPHA</CODE></TD>
 * <TD>( 1, 1, 1 ) - ( Ad / kA, Ad / kA, Ad / kA )</TD>
 * <TD>1 - Ad / kA</TD>
 * </TR>
 * <TR>
 * <TD><CODE>GL_CONSTANT_COLOR</CODE></TD>
 * <TD>(Rc, Gc, Bc)</TD>
 * <TD>Ac</TD>
 * </TR>
 * <TR>
 * <TD><CODE>GL_ONE_MINUS_CONSTANT_COLOR</CODE></TD>
 * <TD>(1, 1, 1) - (Rc, Gc, Bc)</TD>
 * <TD>1 - Ac</TD>
 * </TR>
 * <TR>
 * <TD><CODE>GL_CONSTANT_ALPHA</CODE></TD>
 * <TD>(Ac, Ac, Ac)</TD>
 * <TD>Ac</TD>
 * </TR>
 * <TR>
 * <TD><CODE>GL_ONE_MINUS_CONSTANT_ALPHA</CODE></TD>
 * <TD>(1, 1, 1) - (Ac, Ac, Ac)</TD>
 * <TD>1 - Ac</TD>
 * </TR>
 * <TR>
 * <TD><CODE>GL_SRC_ALPHA_SATURATE</CODE></TD>
 * <TD>( i, i, i )</TD>
 * <TD>1</TD>
 * </TR>
 * </TABLE>
 *
 * In the table,

i = min( As, 1 - Ad )

 * To determine the blended RGBA values of a pixel when drawing in
 * RGBA mode, the system uses the following equations:

Rd = min( kR, Rs sR + Rd dR )

Gd = min( kG, Gs sG + Gd dG )

Bd = min( kB, Bs sB + Bd dB )

Ad = min( kA, As sA + Ad dA )

 * Despite the apparent precision of the above equations, blending
 * arithmetic is not exactly specified, because blending operates with
 * imprecise integer color values. However, a blend factor that should
 * be equal to 1 is guaranteed not to modify its multiplicand, and a
 * blend factor equal to 0 reduces its multiplicand to 0. For example,
 * when <I><c>sfactorRGB</c></I> is <CODE>GL_SRC_ALPHA</CODE>,
 * <I><c>dfactorRGB</c></I> is <CODE>GL_ONE_MINUS_SRC_ALPHA</CODE>, and As is
 * equal to kA, the equations reduce to simple replacement:

Rd = Rs

Gd = Gs

Bd = Bs

Ad = As

glBlendFuncSeparate() operates on all pixel write operations, including the scan conversion of points, lines, and polygons.
glBlendFuncSeparate() does not affect glClear().

 * @param sfactorRGB Specifies how the red, green, blue, and alpha source
    blending factors are computed. The following symbolic constants
    are accepted: <CODE>GL_ZERO</CODE>, <CODE>GL_ONE</CODE>,
    <CODE>GL_DST_COLOR</CODE>, <CODE>GL_ONE_MINUS_DST_COLOR</CODE>,
    <CODE>GL_SRC_ALPHA</CODE>, <CODE>GL_ONE_MINUS_SRC_ALPHA</CODE>,
    <CODE>GL_DST_ALPHA</CODE>, <CODE>GL_ONE_MINUS_DST_ALPHA</CODE>,
    <CODE>GL_CONSTANT_COLOR,</CODE>
    <CODE>GL_ONE_MINUS_CONSTANT_COLOR,</CODE>
    <CODE>GL_CONSTANT_ALPHA</CODE>,
    <CODE>GL_ONE_MINUS_CONSTANT_ALPHA</CODE>, and
    <CODE>GL_SRC_ALPHA_SATURATE</CODE>.  The initial value is
    <CODE>GL_ONE</CODE>.

* @param dfactorRGB Specifies how the red, green, blue, and alpha
   destination blending factors are computed.  The following
   constants are accepted: <CODE>GL_ZERO</CODE>, <CODE>GL_ONE</CODE>,
   <CODE>GL_SRC_COLOR</CODE>, <CODE>GL_ONE_MINUS_SRC_COLOR</CODE>,
   <CODE>GL_SRC_ALPHA</CODE>, <CODE>GL_ONE_MINUS_SRC_ALPHA</CODE>,
   <CODE>GL_DST_ALPHA</CODE>, <CODE>GL_ONE_MINUS_DST_ALPHA</CODE>,
   <CODE>GL_CONSTANT_COLOR,</CODE>
   <CODE>GL_ONE_MINUS_CONSTANT_COLOR,</CODE>
   <CODE>GL_CONSTANT_ALPHA</CODE>, and
   <CODE>GL_ONE_MINUS_CONSTANT_ALPHA</CODE>.  The initial value is
   <CODE>GL_ZERO</CODE>.

 * @param sfactorAlpha Specifies how the alpha source blending factor
    is computed.  The same symbolic constants are accepted as for
    <I><c>sfactorRGB</c></I>.  The initial value is <CODE>GL_ONE</CODE>.

 * @param dfactorAlpha Specifies how the alpha destination blending
    factor is computed.  The same symbolic constants are accepted as
    for <I><c>dfactorRGB</c></I>.  The initial value is <CODE>GL_ZERO</CODE>.

 * @note
 * Incoming (source) alpha is correctly thought of as a material opacity, ranging from 1.0 (kA),
 * representing complete opacity, to 0.0 (0), representing complete transparency.

 * @par Errors
Use glGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>GL_INVALID_ENUM</CODE></TD>
<TD>Either <I><c>sfactorRGB</c></I> or <I><c>dfactorRGB</c></I> is not an accepted value.</TD>
</TR>
</TABLE>

 * @sa glAlphaFunc, glBlendColor, glBlendFunc, glClear, glDepthFunc, glEnable, glLogicOp, glStencilFunc
 */
GLAPI void APIENTRY glBlendFuncSeparate( GLenum sfactorRGB, GLenum dfactorRGB, GLenum sfactorAlpha, GLenum dfactorAlpha )
{
    JS_APIENTRY_TIMER( glBlendFuncSeparate );
#ifdef JS_DPM
    if ( _jsdpmState )
    {
        _JS_DPM_API_ENTRY_4(
            glBlendFuncSeparate, JSDPM_CALL_TYPE_STATE_RASTER, , 4,
            sfactorRGB, sizeof( GLenum ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM,
            dfactorRGB, sizeof( GLenum ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM,
            sfactorAlpha, sizeof( GLenum ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM,
            dfactorAlpha, sizeof( GLenum ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM );
    }
#endif
    PSGLcontext*	LContext = _CurrentContext;
    JS_ASSERT( LContext != NULL );

#ifndef JS_NO_ERROR_CHECK
    if (( !_jsIsSrcBlendFunc( sfactorRGB ) ) ||
            ( !_jsIsDstBlendFunc( dfactorRGB ) ) ||
            ( !_jsIsSrcBlendFunc( sfactorAlpha ) ) ||
            ( !_jsIsDstBlendFunc( dfactorAlpha ) ) )
    {
        _jsSetError( GL_INVALID_ENUM );
        return;
    }
#endif

    LContext->BlendFactorSrcRGB = sfactorRGB;
    LContext->BlendFactorDestRGB = dfactorRGB;
    LContext->BlendFactorSrcAlpha = sfactorAlpha;
    LContext->BlendFactorDestAlpha = dfactorAlpha;
    LContext->needValidate |= PSGL_VALIDATE_BLENDING;
}

/**
 *  @short specify a logical pixel operation
 *
 *  glLogicOp() specifies a logical operation that, when enabled, is applied between the incoming color and the color at
the corresponding location in the frame buffer. To enable or disable the logical operation, call glEnable() and glDisable()
with argument <CODE>GL_COLOR_LOGIC_OP</CODE>. Logical operation is initially disabled.

 * <TABLE rules=all>
 * <TR>
 * <TD>Opcode</TD>
 * <TD>Resulting Operation</TD>
 * </TR>
 * <TR>
 * <TD><CODE>GL_CLEAR</CODE></TD>
 * <TD>0</TD>
 * </TR>
 * <TR>
 * <TD><CODE>GL_SET</CODE></TD>
 * <TD>1</TD>
 * </TR>
 * <TR>
 * <TD><CODE>GL_COPY</CODE></TD>
 * <TD>s</TD>
 * </TR>
 * <TR>
 * <TD><CODE>GL_COPY_INVERTED</CODE></TD>
 * <TD>~ s</TD>
 * </TR>
 * <TR>
 * <TD><CODE>GL_NOOP</CODE></TD>
 * <TD>d</TD>
 * </TR>
 * <TR>
 * <TD><CODE>GL_INVERT</CODE></TD>
 * <TD>~ d</TD>
 * </TR>
 * <TR>
 * <TD><CODE>GL_AND</CODE></TD>
 * <TD>s & d</TD>
 * </TR>
 * <TR>
 * <TD><CODE>GL_NAND</CODE></TD>
 * <TD>~ (s & d)</TD>
 * </TR>
 * <TR>
 * <TD><CODE>GL_OR</CODE></TD>
 * <TD>s | d</TD>
 * </TR>
 * <TR>
 * <TD><CODE>GL_NOR</CODE></TD>
 * <TD>~ (s | d)</TD>
 * </TR>
 * <TR>
 * <TD><CODE>GL_XOR</CODE></TD>
 * <TD>s ^ d</TD>
 * </TR>
 * <TR>
 * <TD><CODE>GL_EQUIV</CODE></TD>
 * <TD>~ (s ^ d)</TD>
 * </TR>
 * <TR>
 * <TD><CODE>GL_AND_REVERSE</CODE></TD>
 * <TD>s & ~ d</TD>
 * </TR>
 * <TR>
 * <TD><CODE>GL_AND_INVERTED</CODE></TD>
 * <TD>~ s & d</TD>
 * </TR>
 * <TR>
 * <TD><CODE>GL_OR_REVERSE</CODE></TD>
 * <TD>s | ~ d</TD>
 * </TR>
 * <TR>
 * <TD><CODE>GL_OR_INVERTED</CODE></TD>
 * <TD>~ s | d</TD>
 * </TR>
 * </TABLE>
 *
 *  <I><c>opcode</c></I> is a symbolic constant chosen from the list above. In the explanation of the logical operations,
<I>s</I> represents the incoming color and <I>d</I> represents the color in the frame buffer.
Standard C-language operators are used. As these bitwise operators suggest, the logical operation is applied
independently to each bit pair of the source and destination indices or colors.
 *
 *  @param opcode Specifies a symbolic constant that selects a logical operation. The following symbols are accepted: <CODE>GL_CLEAR</CODE>, <CODE>GL_SET</CODE>, <CODE>GL_COPY</CODE>, <CODE>GL_COPY_INVERTED</CODE>, <CODE>GL_NOOP</CODE>, <CODE>GL_INVERT</CODE>, <CODE>GL_AND</CODE>, <CODE>GL_NAND</CODE>, <CODE>GL_OR</CODE>, <CODE>GL_NOR</CODE>, <CODE>GL_XOR</CODE>, <CODE>GL_EQUIV</CODE>, <CODE>GL_AND_REVERSE</CODE>, <CODE>GL_AND_INVERTED</CODE>, <CODE>GL_OR_REVERSE</CODE>, and <CODE>GL_OR_INVERTED</CODE>. The initial value is <CODE>GL_COPY</CODE>.
 *
 *  @par Errors
Use glGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>GL_INVALID_ENUM</CODE></TD>
<TD><I><c>opcode</c></I> is not an accepted value.</TD>
</TR>
</TABLE>
 *
 *  @sa glAlphaFunc, glBlendFunc, glEnable, glStencilOp
 */

GLAPI void APIENTRY glLogicOp( GLenum opcode )
{
    JS_APIENTRY_TIMER( glLogicOp );
#ifdef JS_DPM
    if ( _jsdpmState ) { _JS_DPM_API_ENTRY_1( glLogicOp, JSDPM_CALL_TYPE_STATE_RASTER, , 1, opcode, sizeof( GLenum ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM ); }
#endif
    PSGLcontext*	LContext = _CurrentContext;
    JS_ASSERT( LContext );

#ifndef JS_NO_ERROR_CHECK
    if ( !_jsIsLogicOp( opcode ) )
    {
        _jsSetError( GL_INVALID_ENUM );
        return;
    }
#endif
    LContext->LogicOp = opcode;
    LContext->needValidate |= PSGL_VALIDATE_LOGIC_OP;
}

/**
 * @short Specify buffer attachment to read from
 *
 * glReadBuffer() specifies the color buffer attachment to read from when a
 * user frame buffer is bound.  This buffer attachment will be used for
 * subsequent glReadPixels(), glCopyTexImage2D(), and glCopyTexSubImage2D()
 * operations for color internal formats.
 *
 * Because there is only one depth buffer attachment, you should not call glReadBuffer() to specify the
 * depth buffer attachment.
 *
 * This function is valid only for application-defined frame buffers.
 * 
 * The default setting for glReadBuffer() is
 * <CODE>GL_COLOR_ATTACHMENT0_EXT</CODE>.
 *
 * @param mode Specify buffer attachment.  Must be one of
 * <CODE>GL_COLOR_ATTACHMENT0_EXT</CODE>,
 * <CODE>GL_COLOR_ATTACHMENT1_EXT</CODE>,
 * <CODE>GL_COLOR_ATTACHMENT2_EXT</CODE>, or
 * <CODE>GL_COLOR_ATTACHMENT3_EXT</CODE>.
 *
 * @par Errors
Use glGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>GL_INVALID_ENUM</CODE></TD>
<TD><I><c>mode</c></I> is not one of <CODE>GL_COLOR_ATTACHMENT<I>i</I>_EXT</CODE>, where 0 <= <I>i</I> <= 15.
</TD>
</TR>
<TR>
<TD><CODE>GL_INVALID_OPERATION</CODE></TD>
<TD>The default frame buffer is bound, or <I><c>mode</c></I> is <CODE>GL_COLOR_ATTACHMENT<I>i</I>_EXT</CODE>.
where 4 <= <I>i</I> <= 15.</TD>
</TR>
</TABLE>
 *
 * @sa glFramebufferRenderbufferOES, glFramebufferTexture2DOES,
 * glReadPixels, glCopyTexImage2D, glCopyTexSubImage2D
 */
GLAPI void APIENTRY glReadBuffer( GLenum mode )
{
    JS_APIENTRY_TIMER( glReadBuffer );
#ifdef JS_DPM
    if ( _jsdpmState )
        _JS_DPM_API_ENTRY_1( glReadBuffer, JSDPM_CALL_TYPE_STATE_BUFFER, , 1, mode, sizeof( GLenum ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM );
#endif

    PSGLcontext*	LContext = _CurrentContext;
    JS_ASSERT( LContext );

#ifndef JS_NO_ERROR_CHECK
    switch ( mode )
    {
        case GL_COLOR_ATTACHMENT0_EXT:
        case GL_COLOR_ATTACHMENT1_EXT:
        case GL_COLOR_ATTACHMENT2_EXT:
        case GL_COLOR_ATTACHMENT3_EXT:
            if ( LContext->framebuffer )
#endif
                LContext->ReadBuffer = mode;
#ifndef JS_NO_ERROR_CHECK
            else
                _jsSetError( GL_INVALID_OPERATION );
            break;
        case GL_COLOR_ATTACHMENT4_EXT:
        case GL_COLOR_ATTACHMENT5_EXT:
        case GL_COLOR_ATTACHMENT6_EXT:
        case GL_COLOR_ATTACHMENT7_EXT:
        case GL_COLOR_ATTACHMENT8_EXT:
        case GL_COLOR_ATTACHMENT9_EXT:
        case GL_COLOR_ATTACHMENT10_EXT:
        case GL_COLOR_ATTACHMENT11_EXT:
        case GL_COLOR_ATTACHMENT12_EXT:
        case GL_COLOR_ATTACHMENT13_EXT:
        case GL_COLOR_ATTACHMENT14_EXT:
        case GL_COLOR_ATTACHMENT15_EXT:
            _jsSetError( GL_INVALID_OPERATION );
            break;
        default:
            _jsSetError( GL_INVALID_ENUM );
            break;
    }
#endif
}

/**
 *  @short Read a block of pixels from the color buffer
 *
 *  glReadPixels() returns pixel data from the color buffer, starting with the pixel whose lower left corner is at location (x, y), into client memory starting at location <I><c>pixels</c></I>. The processing of the pixel data before it is placed into client memory can be controlled with glPixelStorei().
 *
 *  glReadPixels() returns values from each pixel with lower left corner at (<I><c>x</c> + i, <c>y</c> + j</i>)
for 0 <= <I>i</I> < <I><c>width</c></I> and 0 <= <I>j</I> < <I><c>height</c></I>.
 *  This pixel is said to be the <I>i</I>th pixel
in the <I>j</I>th row. Pixels are returned in row order from the lowest to the highest row, left to right in each row.
 *  To return pixels from the highest to lowest row, specify a negative <I><c>height</c></I>. 
 *
 *  <I><c>format</c></I> specifies the format of the returned pixel values. Formats that
 *  are consistent with the PSGL surface and <I><c>type</c></I> of pixel data provide the best performance. The
 *  <CODE>GL_BGRA</CODE> format is consistent with component order for pixel storage of color buffers.
 *  The <CODE>GL_DEPTH_COMPONENT</CODE> format is the preferred <I><c>format</c></I> for depth buffer values.
 * <I><c>type</c></I> <CODE>GL_UNSIGNED_BYTE</CODE> may be used with <I><c>format</c></I> <CODE>GL_RGBA</CODE>,
 * but this combination does not have the best performance, because the color components must be
 * swapped as they are extracted from the color buffer.
 *
 *  The following table describes best-performing combination of
 *  values for <I><c>type</c></I> and <I><c>format</c></I> in glReadPixels()
 *  and <I><c>internalFormat</c></I> in frame buffer attachments.
 *
 *  The device frame buffer has an implicit internal format of
 *  <code>GL_ARGB_SCE</code> for the color buffer and <code>GL_DEPTH24_STENCIL8_SCE</code>
 *  for the depth/stencil buffer.  User frame buffer object
 *  attachments have internal formats set in glRenderbufferStorageOES()
 *  for render buffers and glTexImage2D() for textures. <CODE>glReadPixels()</CODE> correctly reads 16-bit depth buffers.
 *
 * <TABLE rules=all>
 * <TR>
 * <TD>glReadPixels <I><c>format</c></I></TD>
 * <TD>glReadPixels <I><c>type</c></I></TD>
 * <TD>attachment <I><c>internalFormat</c></I></TD>
 * </TR>
 * <TR>
 * <TD><CODE>GL_BGRA</CODE></TD>
 * <TD><CODE>GL_UNSIGNED_INT_8_8_8_8_REV</CODE></TD>
 * <TD><CODE>GL_ARGB_SCE</CODE></TD>
 * </TR>
 * <TR>
 * <TD><CODE>GL_RGBA</CODE></TD>
 * <TD><CODE>GL_HALF_FLOAT_ARB</CODE></TD>
 * <TD><CODE>GL_RGBA16F_ARB</CODE></TD>
 * </TR>
 * <TR>
 * <TD><CODE>GL_RGBA</CODE></TD>
 * <TD><CODE>GL_FLOAT</CODE></TD>
 * <TD><CODE>GL_RGBA32F_ARB</CODE></TD>
 * </TR>
 * <TR>
 * <TD><CODE>GL_DEPTH_COMPONENT</CODE></TD>
 * <TD><CODE>GL_UNSIGNED_INT_24_8_SCE</CODE></TD>
 * <TD><CODE>GL_DEPTH24_STENCIL8_SCE</CODE></TD>
 * </TR>
 * </TABLE>
 *
 * The components are converted to the proper format, as specified by <I><c>type</c></I>.
 * For floating point data types, the value is merely passed as is.
 * For other data types, each component is multiplied
 * by 2<sup>n</sup> - 1, where n is the number of bits per component.
 *
 * Return values are placed in memory as follows. If <I><c>format</c></I> is <CODE>GL_DEPTH_COMPONENT</CODE>,
 * a single value is returned and the data for the <I>i</I>th pixel in the <I>j</I>th row is placed in
 * location <I>j</I> * <I><c>width</c></I> + <I>i</I>.
 * If <I><c>format</c></I> is <CODE>GL_BGRA</CODE> and <CODE>GL_RGBA</CODE>,
 * four values are returned for each pixel.
 *
If there is a currently bound pixel buffer object, then <I><c>pixels</c></I> is not a direct pointer
to a client memory location but is an offset into a buffer object with its data store on the server side.
glBufferData() should have previously allocated adequate server-side memory to receive the pixel array.
In this case, if <I><c>pixels</c></I> is <CODE>NULL</CODE>, then the start of the pixel buffer object is referenced.
 *
 * When reading pixels from a multi-sampled frame buffer object, the samples are filtered and converted to pixels.
 *
 *  @param x Specifies the x window coordinate of the first pixel read from the color buffer.
 *  The first pixel is the lower left corner of a rectangular block of pixels.
 *  @param y Specifies the y window coordinate of the first pixel read from the color buffer.
 *  @param width Specifies the horizontal dimension of the pixel rectangle.
 *  @param height Specifies the vertical dimension of the pixel rectangle. 
 *  Use negative number to return pixels from top to bottom.
 *  @param format Specifies the format of the pixel data.
 *  Must be one of <CODE>GL_RGBA</CODE>, <CODE>GL_BGRA</CODE>, or <CODE>GL_DEPTH_COMPONENT</CODE>.
 *  @param type Specifies the data type of the pixel data.
 *  Must be one of <CODE>GL_UNSIGNED_BYTE</CODE>, <CODE>GL_UNSIGNED_SHORT</CODE>,
 *  <CODE>GL_UNSIGNED_INT_8_8_8_8_REV</CODE>,
 *  <CODE>GL_UNSIGNED_INT_24_8_SCE</CODE>, <CODE>GL_HALF_FLOAT_ARB</CODE>, or <CODE>GL_FLOAT</CODE>.
 *  @param pixels Usually specifies a pointer to where the image data is returned in client side memory.
If a pixel buffer object is bound with the target
<CODE>GL_PIXEL_PACK_BUFFER_ARB</CODE>, then <I><c>pixels</c></I> is not
a pointer, but an offset into a buffer object with its data store on the server side.
 *
 * @note
 * If <I><c>width</c></I> and <I><c>height</c></I> are both one, <I><c>pixels</c></I> correspond to a single pixel.
 * Values for pixels that lie outside the window connected to the current GL context are undefined.
 * @par
 * If an error is generated, no change is made to the contents of <I><c>pixels</c></I>.
 *
 *  @par Errors
Use glGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>GL_INVALID_ENUM</CODE></TD>
<TD><I><c>format</c></I> is not one of
<CODE>GL_RGBA</CODE>, <CODE>GL_BGRA</CODE>, or <CODE>GL_DEPTH_COMPONENT</CODE>.</TD>
</TR>
<TR>
<TD><CODE>GL_INVALID_ENUM</CODE></TD>
<TD><I><c>type</c></I> is not one of <CODE>GL_UNSIGNED_BYTE</CODE>,
<CODE>GL_UNSIGNED_SHORT</CODE>, <CODE>GL_UNSIGNED_INT_8_8_8_8_REV</CODE>,
<CODE>GL_UNSIGNED_INT_24_8_SCE</CODE>, <CODE>GL_HALF_FLOAT_ARB</CODE>, or <CODE>GL_FLOAT</CODE>. </TD>
</TR>
<TR>
<TD><CODE>GL_INVALID_VALUE</CODE></TD>
<TD><I><c>width</c></I> is negative.</TD>
</TR>
<TR>
<TD><CODE>GL_INVALID_OPERATION</CODE></TD>
<TD><I><c>format</c></I> and <I><c>type</c></I> are
incompatible with one another or incompatible with the type and format of the surface. </TD>
</TR>
<TR>
<TD><CODE>GL_INVALID_FRAMEBUFFER_OPERATION_OES</CODE></TD>
<TD>One or more of the following conditions is not true:
<UL>
<LI>All textures are complete.</LI>
<LI>Color formats are consistent across the color textures</LI>
<LI>Swizzling hint is consistent across all the textures</LI>
<LI>Drawable color format for the color attachments if any</LI>
<LI>Drawable depth format for the depth attachment if any</LI>
<LI>No anti-aliasing if FP16 or FP32 color format</LI>
<LI>No swizzling with RGBA/FP16 or RGBA/FP32 (any format that is more than
32 bits per pixel)</LI>
<LI>Matching sizes, if swizzled</LI>
</UL></TD>
</TR>
</TABLE>
 *
 *  @sa glPixelStorei, glFramebufferParameteriSCE
 *
 */

GLAPI void APIENTRY glReadPixels( GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLvoid *pixels )
{
    JS_APIENTRY_TIMER( glReadPixels );
    PSGLcontext*	LContext = _CurrentContext;
    JS_ASSERT( LContext );
    GLboolean flipImage = GL_FALSE;

#ifdef JS_DPM
    int pixelStorageSize = width * height * _jsGetPixelSize( format, type );
    if ( _jsdpmState )
    {
        if (( LContext->PixelPackBuffer != 0 ) || ( pixels == NULL && _JS_DPM_STATE( CAPTURE ) ) )
        {
            _JS_DPM_API_ENTRY_7( glReadPixels, JSDPM_CALL_TYPE_STATE_BUFFER, , 7,
                                 x, sizeof( GLint ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                                 y, sizeof( GLint ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                                 width, sizeof( GLsizei ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                                 height, sizeof( GLsizei ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                                 format, sizeof( GLenum ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM,
                                 type, sizeof( GLenum ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM,
                                 pixels, sizeof( pixels ), INPUT_BY_VALUE, DPM_API_ARGUMENT_POINTER );
        }
        else
        {
            _JS_DPM_API_ENTRY_7( glReadPixels, JSDPM_CALL_TYPE_STATE_BUFFER, , 7,
                                 x, sizeof( GLint ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                                 y, sizeof( GLint ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                                 width, sizeof( GLsizei ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                                 height, sizeof( GLsizei ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                                 format, sizeof( GLenum ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM,
                                 type, sizeof( GLenum ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM,
                                 pixels, pixelStorageSize, INPUT_OUTPUT, DPM_API_ARGUMENT_POINTER );
        }
    }	// if
#endif

    if ( height < 0 )
    {
        y += height;
        height = -height;
        flipImage = GL_TRUE;
    }

#ifndef JS_NO_ERROR_CHECK
    if (( width < 0 ) || ( height < 0 ) )
    {
        _jsSetError( GL_INVALID_VALUE );
        return;
    }
    //if (!_jsIsType(type)) // this seems too general, replaced with the specific valid types
    if ( !( type == GL_UNSIGNED_BYTE || type == GL_UNSIGNED_SHORT || type == GL_UNSIGNED_INT_8_8_8_8_REV ||
            type == GL_UNSIGNED_INT_24_8_SCE || type == GL_HALF_FLOAT_ARB || type == GL_FLOAT ) )
    {
        _jsSetError( GL_INVALID_ENUM );
        return;
    }
    if ( !_jsIsFormat( format ) )
    {
        _jsSetError( GL_INVALID_ENUM );
        return;
    }

#endif

    _JS_PERFORMANCE_START( ReadPixelsCounter );

    if ( LContext->needValidate & PSGL_VALIDATE_FRAMEBUFFER )
        _jsValidateFramebuffer();

    if ( LContext->PixelPackBuffer != 0 )
    {

        // read into pixel buffer object
        if ( !_jsPlatformReadPBOPixels( x, y, width, height, flipImage, format, type, pixels ) )
        {
            // slow path with map/unmap
            pixels = ( char * )glMapBuffer( GL_PIXEL_PACK_BUFFER_ARB, GL_WRITE_ONLY ) +
                     (( const GLubyte* )pixels - ( const GLubyte* )NULL );

            _jsPlatformReadPixels( x, y, width, height, flipImage, format, type, pixels );

            glUnmapBuffer( GL_PIXEL_PACK_BUFFER_ARB );
        }
    }
    else
    {
        _jsPlatformReadPixels( x, y, width, height, flipImage, format, type, pixels );
    }
    _JS_PERFORMANCE_STOP( ReadPixelsCounter );
}

/** @} OpenGL */
