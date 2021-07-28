/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 *                Copyright (C) 2009 Sony Computer Entertainment Inc.
 *                                               All Rights Reserved.
 */

 
/*
 * 
 */

#include <PSGL/psgl.h>
#include <PSGL/private.h>
#include <PSGL/Utils.h>
#include <PSGL/DPM2.h>
#include <limits.h>

/** @addtogroup OpenGL
 *
 * @{
 */

/**
 * @short Specify the value used for depth buffer comparisons
 *
 * glDepthFunc() specifies the function used to compare each incoming pixel depth value
 * with the depth value present in the depth buffer. The comparison is performed only
 * if depth testing is enabled. To enable and disable depth testing,
 * call glEnable() and glDisable() with argument <CODE>GL_DEPTH_TEST</CODE>.
 * Depth testing is initially disabled.

 * <I><c>func</c></I> specifies the conditions under which the pixel will be drawn.
 * The comparison functions are as follows:

 * <TABLE rules=all>
 * <TR>
 * <TD><CODE>GL_NEVER</CODE></TD>
 * <TD>Never passes.</TD>
 * </TR>
 * <TR>
 * <TD><CODE>GL_LESS</CODE></TD>
 * <TD>Passes if the incoming depth value is less than the stored depth value.</TD>
 * </TR>
 * <TR>
 * <TD><CODE>GL_EQUAL</CODE></TD>
 * <TD>Passes if the incoming depth value is equal to the stored depth value.</TD>
 * </TR>
 * <TR>
 * <TD><CODE>GL_LEQUAL</CODE></TD>
 * <TD>Passes if the incoming depth value is less than or equal to the stored depth value.</TD>
 * </TR>
 * <TR>
 * <TD><CODE>GL_GREATER</CODE></TD>
 * <TD>Passes if the incoming depth value is greater than the stored depth value.</TD>
 * </TR>
 * <TR>
 * <TD><CODE>GL_NOTEQUAL</CODE></TD>
 * <TD>Passes if the incoming depth value is not equal to the stored depth value.</TD>
 * </TR>
 * <TR>
 * <TD><CODE>GL_GEQUAL</CODE></TD>
 * <TD>Passes if the incoming depth value is greater than or equal to the stored depth value.</TD>
 * </TR>
 * <TR>
 * <TD><CODE>GL_ALWAYS</CODE></TD>
 * <TD>Always passes.</TD>
 * </TR>
 * </TABLE>

 * The initial value of <I><c>func</c></I> is <CODE>GL_LESS</CODE>. Initially, depth testing is disabled.
 * Even if the depth buffer exists and the depth
 * mask is non-zero, the depth buffer is not updated if the depth test is disabled.
 *
 * @param func Specifies the depth comparison function. Symbolic constants defined below are accepted.
 * The initial value is <CODE>GL_LESS</CODE>.
 *
 * @par Errors
Use glGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>GL_INVALID_ENUM</CODE></TD>
<TD><I><c>func</c></I> is not an accepted value.</TD>
</TR>
</TABLE>
 *
 * @sa glDepthRangef, glEnable, glPolygonOffset
 */
GLAPI void APIENTRY glDepthFunc( GLenum func )
{
    JS_APIENTRY_TIMER( glDepthFunc );
#ifdef JS_DPM
    if ( _jsdpmState ) { _JS_DPM_API_ENTRY_1( glDepthFunc, JSDPM_CALL_TYPE_STATE_RASTER, , 1, func, sizeof( GLenum ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM ); }
#endif
    PSGLcontext*	LContext = _CurrentContext;
    JS_ASSERT( LContext );

#ifndef JS_NO_ERROR_CHECK
    if ( !_jsIsTestFunc( func ) )
    {
        _jsSetError( GL_INVALID_ENUM );
        return;
    }
#endif

    LContext->DepthFunc = func;
    LContext->needValidate |= PSGL_VALIDATE_DEPTH_TEST;
}

/**
 * @short Specify the alpha test function
 *
 * This function specifies the alpha test function.
 *
 * The alpha test discards fragments depending on the outcome of a
 * comparison between an incoming fragment&rsquo;s alpha value and a constant
 * reference value. glAlphaFunc() specifies the reference value and the
 * comparison function. The comparison is performed only if alpha testing
 * is enabled. By default, it is not enabled. (See glEnable() and glDisable() of
 * <CODE>GL_ALPHA_TEST</CODE>.)

 * <I><c>func</c></I> and <I><c>ref</c></I> specify the conditions under which the pixel is drawn.
 * The incoming alpha value is compared to <I><c>ref</c></I> using the function specified
 * by <I><c>func</c></I>. If the value passes the comparison, the incoming fragment is
 * drawn if it also passes subsequent stencil and depth buffer
 * tests. If the value fails the comparison, no change is made to the
 * frame buffer at that pixel location. The comparison functions are as
 * follows:

 * <TABLE rules=all>
 * <TR>
 * <TD><CODE>GL_NEVER</CODE></TD>
 * <TD>Never passes.</TD>
 * </TR>
 * <TR>
 * <TD><CODE>GL_LESS</CODE></TD>
 * <TD>Passes if the incoming alpha value is less than the reference value.</TD>
 * </TR>
 * <TR>
 * <TD><CODE>GL_EQUAL</CODE></TD>
 * <TD>Passes if the incoming alpha value is equal to the reference value.</TD>
 * </TR>
 * <TR>
 * <TD><CODE>GL_LEQUAL</CODE></TD>
 * <TD>Passes if the incoming alpha value is less than or equal to the reference value.</TD>
 * </TR>
 * <TR>
 * <TD><CODE>GL_GREATER</CODE></TD>
 * <TD>Passes if the incoming alpha value is greater than the reference value.</TD>
 * </TR>
 * <TR>
 * <TD><CODE>GL_NOTEQUAL</CODE></TD>
 * <TD>Passes if the incoming alpha value is not equal to the reference value.</TD>
 * </TR>
 * <TR>
 * <TD><CODE>GL_GEQUAL</CODE></TD>
 * <TD>Passes if the incoming alpha value is greater than or equal to the reference value.</TD>
 * </TR>
 * <TR>
 * <TD><CODE>GL_ALWAYS</CODE></TD>
 * <TD>Always passes.</TD>
 * </TR>
 * </TABLE>

 * glAlphaFunc() operates on all pixel write operations, including those resulting from
 * the scan conversion of points, lines, and polygons. glAlphaFunc() does not affect glClear()
 *
 * @param func Specifies the alpha comparison function. Symbolic constants
 *             in the table below are accepted. The initial value is <CODE>GL_ALWAYS</CODE>.
 *
 * @param ref Specifies the reference value that incoming alpha values are
 *            compared to. This value is clamped to the range [0, 1], where
 *            0 represents the lowest possible alpha value and 1 the highest
 *            possible value. The initial reference value is 0.
 *
 * @par Errors
Use glGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>GL_INVALID_ENUM</CODE></TD>
<TD><I><c>func</c></I> is not an accepted value.</TD>
</TR>
</TABLE>
 *
 * @sa glBlendFunc, glClear, glDepthFunc, glEnable, glStencilFunc
 */
GLAPI void APIENTRY glAlphaFunc( GLenum func, GLclampf ref )
{
    JS_APIENTRY_TIMER( glAlphaFunc );
#ifdef JS_DPM
    if ( _jsdpmState )
    {
        _JS_DPM_API_ENTRY_2( glAlphaFunc, JSDPM_CALL_TYPE_STATE_RASTER, , 2,
                             func, sizeof( GLenum ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM,
                             ref, sizeof( GLclampf ), INPUT_BY_VALUE, DPM_API_ARGUMENT_FLOAT_VALUE );
    }
#endif
    PSGLcontext*	LContext = _CurrentContext;

#ifndef JS_NO_ERROR_CHECK
    if ( !_jsIsTestFunc( func ) )
    {
        _jsSetError( GL_INVALID_ENUM );
        return;
    }
#endif

    LContext->AlphaFunc = func;
    LContext->AlphaRef = _jsClampf( ref );
    LContext->needValidate |= PSGL_VALIDATE_ALPHA_TEST;
}

/**
  @short Set the active stencil face

  There are two independent sets of stencil state set by glStencilFunc(),
  glStencilOp(), and glStencilMask(), one for <CODE>GL_FRONT</CODE> and one
  for <CODE>GL_BACK</CODE>.  When <CODE>GL_STENCIL_TEST</CODE> is enabled
  and <CODE>GL_STENCIL_TEST_TWO_SIDE_EXT</CODE> is enabled, stencil
  operations are executed using the <CODE>GL_FRONT</CODE> state for
  front-facing polygons and the <CODE>GL_BACK</CODE> state for back-facing
  polygons, as defined by glFrontFace().  When <CODE>GL_STENCIL_TEST</CODE>
  is enabled and <CODE>GL_STENCIL_TEST_TWO_SIDE_EXT</CODE> is <B>not</B>
  enabled, stencil operations are executed with the <CODE>GL_FRONT</CODE>
  state regardless of facing.
  
  glActiveStencilFaceEXT() sets the active stencil face to <I><c>face</c></I>.
  Subsequent calls to glStencilFunc(), glStencilOp(), and glStencilMask()
  update the state for the active stencil face.
  
  Clearing the stencil buffer always uses the <CODE>GL_FRONT</CODE>
  stencil write mask.

  @param face Specifies the active stencil face.  Must be one of
  <CODE>GL_FRONT</CODE> or
  <CODE>GL_BACK</CODE>.

  @par Errors
Use glGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>GL_INVALID_ENUM</CODE></TD>
<TD><I><c>face</c></I> is neither <CODE>GL_FRONT</CODE> nor <CODE>GL_BACK</CODE>.</TD>
</TR>
</TABLE>

  @sa glEnable, glDisable, glStencilFunc, glStencilOp, glStencilMask
 */

GLAPI void APIENTRY glActiveStencilFaceEXT( GLenum face )
{
    JS_APIENTRY_TIMER( glActiveStencilFaceEXT );
#ifdef JS_DPM
    if ( _jsdpmState )
    {
        _JS_DPM_API_ENTRY_1( glActiveStencilFaceEXT, JSDPM_CALL_TYPE_STATE_RASTER, , 1,
                             face, sizeof( GLenum ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM );
    }
#endif

#ifndef JS_NO_ERROR_CHECK
    switch ( face )
    {
        case GL_FRONT:
        case GL_BACK:
            break;
        default:
            _jsSetError( GL_INVALID_ENUM );
            return;
    }
#endif

    PSGLcontext* LContext = _CurrentContext;
    JS_ASSERT( LContext != NULL );

    LContext->ActiveStencilFace = face;
}

/**
 * @short Set function and reference value for stencil testing
 *
 * Stenciling, like depth-buffering, enables and disables drawing on a per-pixel basis. You draw into the stencil planes using GL drawing primitives, then render geometry and images, using the stencil planes to mask out portions of the screen. Stenciling is typically used in multipass rendering algorithms to achieve special effects, such as decals, outlining, and constructive solid geometry rendering.

 * The stencil test conditionally eliminates a pixel based on the outcome
 * of a comparison between the reference value and the value in the stencil
 * buffer. To enable and disable the test, call glEnable() and glDisable() with
 * argument <CODE>GL_STENCIL_TEST</CODE>. To specify actions based on the outcome of the
 * stencil test, call glStencilOp(). To examine the number of bitplanes in the stencil buffer,
 * call glGetIntegerv() with argument <CODE>GL_STENCIL_BITS</CODE>.

 * <I><c>func</c></I> is a symbolic constant that determines the stencil comparison
 * function. It accepts one of eight values, shown in the following list.
 * <I><c>ref</c></I> is an integer reference value that is used in the stencil comparison.
 * It is clamped to the range [0,2<sup>n</sup>-1], where <I>n</I> is the number of
 * bitplanes in the stencil buffer. <I><c>mask</c></I> is bitwise ANDed with both the
 * reference value and the stored stencil value, with the ANDed values
 * participating in the comparison.

 * If <I><c>stencil</c></I> represents the value stored in the corresponding stencil
 * buffer location, the following list shows the effect of each comparison
 * function that can be specified by <I><c>func</c></I>. Only if the comparison succeeds
 * is the pixel passed through to the next stage in the rasterization
 * process (see glStencilOp()). All tests treat <I><c>stencil</c></I> values as unsigned
 * integers in the range [0,2<sup>n</sup>-1], where <I>n</I> is the number of bitplanes in
 * the stencil buffer.

 * The following values are accepted by <I><c>func</c></I>:

 * <TABLE rules=all>
 * <TR>
 * <TD><CODE>GL_NEVER</CODE></TD>
 * <TD>Always fails</TD>
 * </TR>
 * <TR>
 * <TD><CODE>GL_LESS</CODE></TD>
 * <TD>Passes if ( ref & mask ) < ( stencil & mask )</TD>
 * </TR>
 * <TR>
 * <TD><CODE>GL_LEQUAL</CODE></TD>
 * <TD>Passes if ( ref & mask ) <= ( stencil & mask )</TD>
 * </TR>
 * <TR>
 * <TD><CODE>GL_GREATER</CODE></TD>
 * <TD>Passes if ( ref & mask ) > ( stencil & mask )</TD>
 * </TR>
 * <TR>
 * <TD><CODE>GL_GEQUAL</CODE></TD>
 * <TD>Passes if ( ref & mask ) >= ( stencil & mask )</TD>
 * </TR>
 * <TR>
 * <TD><CODE>GL_EQUAL</CODE></TD>
 * <TD>Passes if ( ref & mask ) = ( stencil & mask )</TD>
 * </TR>
 * <TR>
 * <TD><CODE>GL_NOTEQUAL</CODE></TD>
 * <TD>Passes if ( ref & mask ) != ( stencil & mask )</TD>
 * </TR>
 * <TR>
 * <TD><CODE>GL_ALWAYS</CODE></TD>
 * <TD>Always passes</TD>
 * </TR>
 * </TABLE>
 *
 * @param func Specifies the test function.
 * Eight tokens are valid: <CODE>GL_NEVER</CODE>, <CODE>GL_LESS</CODE>, <CODE>GL_LEQUAL</CODE>, <CODE>GL_GREATER</CODE>,
 * <CODE>GL_GEQUAL</CODE>, <CODE>GL_EQUAL</CODE>, <CODE>GL_NOTEQUAL</CODE>, and <CODE>GL_ALWAYS</CODE>.
 * The initial value is <CODE>GL_ALWAYS</CODE>.
 *
 * @param ref Specifies the reference value for the stencil test. <I><c>ref</c></I> is
 *        clamped to the range [0,2<sup>n</sup>-1], where <I>n</I> is the number of bitplanes
 *        in the stencil buffer. The initial value is 0.
 *
 * @param mask Specifies a mask that is ANDed with both the reference value and
 *        the stored stencil value when the test is done. The initial value
 *        is all 1&rsquo;s.
 *
 * @note
 * Initially, the stencil test is disabled. If there is no stencil buffer,
 * no stencil modification can occur and it is as if the stencil test always passes.
 *
 * @par Errors
Use glGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>GL_INVALID_ENUM</CODE></TD>
<TD><I><c>func</c></I> is not one of the eight accepted values.</TD>
</TR>
</TABLE>
 *
 * @sa glAlphaFunc, glBlendFunc, glDepthFunc, glEnable, glGetIntegerv, glLogicOp, glStencilOp
 */
GLAPI void APIENTRY glStencilFunc( GLenum func, GLint ref, GLuint mask )
{
    JS_APIENTRY_TIMER( glStencilFunc );
#ifdef JS_DPM
    if ( _jsdpmState )
    {
        _JS_DPM_API_ENTRY_3( glStencilFunc, JSDPM_CALL_TYPE_STATE_RASTER, , 3,
                             func, sizeof( GLenum ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM,
                             ref, sizeof( GLint ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             mask, sizeof( GLuint ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE );
    }
#endif
    PSGLcontext*	LContext = _CurrentContext;
    JS_ASSERT( LContext );

#ifndef JS_NO_ERROR_CHECK
    if ( !_jsIsTestFunc( func ) )
    {
        _jsSetError( GL_INVALID_ENUM );
        return;
    }
#endif

    switch ( LContext->ActiveStencilFace )
    {
        case GL_FRONT:
            LContext->StencilFunc = func;
            LContext->StencilRef = ref;
            LContext->StencilFuncMask = mask;
            break;
        case GL_BACK:
            LContext->StencilBackfaceFunc = func;
            LContext->StencilBackfaceRef = ref;
            LContext->StencilBackfaceFuncMask = mask;
            break;
        default:
            JS_ASSERT( 0 );
    }
    LContext->needValidate |= PSGL_VALIDATE_STENCIL_TEST;
}

/**
 * @short Define the scissor box
 *
 * glScissor() defines a rectangle, called the scissor box, in window coordinates.
 * The first two arguments, <I><c>x</c></I> and <I><c>y</c></I>, specify the lower left corner of the box.
 * <I><c>width</c></I> and <I><c>height</c></I> specify the width and height of the box.

 * To enable and disable the scissor test, call glEnable() and glDisable()
 * with argument <CODE>GL_SCISSOR_TEST</CODE>. The test is initially disabled. While
 * the test is enabled, only pixels that lie within the scissor box can be
 * modified by drawing commands. Window coordinates have integer values
 * at the shared corners of frame buffer pixels. <CODE>glScissor(0,0,1,1)</CODE>
 * allows modification of only the lower left pixel in the device, and
 * <CODE>glScissor(0,0,0,0)</CODE> doesn&rsquo;t allow modification of any pixels in the window.

 * When the scissor test is disabled, it is as though the scissor box
 * includes the entire device.
 *
 * @param x Specify the x coordinate of the lower left corner of the scissor box.  Initial value is 0.
 * @param y Specify the y coordinate of the lower left corner of the scissor box.  Initial value is 0.
 * @param width Specify the width of the scissor box. When a GL context is first attached to a surface (e.g. window),
 * <I><c>width</c></I> is initially set to the x dimension of that surface.
 * @param height Specify the height of the scissor box. When a GL context is first attached to a surface (e.g. window),
 * <I><c>height</c></I> is initially set to the y dimension of that surface.
 *
 * @par Errors
Use glGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>GL_INVALID_VALUE</CODE></TD>
<TD>Either <I><c>width</c></I> or <I><c>height</c></I> is negative.</TD>
</TR>
</TABLE>
 *
 * @sa glEnable, glViewport
 */

GLAPI void APIENTRY glScissor( GLint x, GLint y, GLsizei width, GLsizei height )
{
    JS_APIENTRY_TIMER( glScissor );
#ifdef JS_DPM
    if ( _jsdpmState )
    {
        _JS_DPM_API_ENTRY_4( glScissor, JSDPM_CALL_TYPE_STATE_RASTER, , 4,
                             x, sizeof( GLint ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             y, sizeof( GLint ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             width, sizeof( GLsizei ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             height, sizeof( GLsizei ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE );
    }
#endif
    PSGLcontext*	LContext = _CurrentContext;
    JS_ASSERT( LContext );
#ifndef JS_NO_ERROR_CHECK
    if (( width < 0 ) || ( height < 0 ) )
    {
        _jsSetError( GL_INVALID_VALUE );
        return;
    }
#endif

    LContext->ScissorBox.X = x;
    LContext->ScissorBox.Y = y;
    LContext->ScissorBox.XSize = width;
    LContext->ScissorBox.YSize = height;
    LContext->needValidate |= PSGL_VALIDATE_SCISSOR_BOX;
}

/**
 * @short Specify fog parameters
 *
 * glFogf() assigns the value or values in <I><c>params</c></I> to the fog parameter specified by <I><c>pname</c></I>. It
 * differs from glFogfv() only in argument type.  Refer to glFogfv() for more information.
 *
 * @param pname Specifies a single-valued fog parameter.
 *
 * @param param Specifies the value that <I><c>pname</c></I> will be set to.
 */

GLAPI void APIENTRY glFogf( GLenum pname, GLfloat param )
{
    JS_APIENTRY_TIMER( glFogf );
#ifdef JS_DPM
    if ( _jsdpmState )
    {
        _JS_DPM_API_ENTRY_2( glFogf, JSDPM_CALL_TYPE_STATE_RASTER, , 2,
                             pname, sizeof( GLenum ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM,
                             param, sizeof( GLfloat ), INPUT_BY_VALUE, DPM_API_ARGUMENT_FLOAT_VALUE );
    }
#endif
    PSGLcontext*	LContext = _CurrentContext;

    switch ( pname )
    {
        case GL_FOG_DENSITY:
#ifndef JS_NO_ERROR_CHECK
            if ( param < 0.f )
            {
                _jsSetError( GL_INVALID_VALUE );
                return;
            }
#endif
            LContext->FogDensity = param;
            break;
        case GL_FOG_START:
            LContext->FogStart = param;
            break;
        case GL_FOG_END:
            LContext->FogEnd = param;
            break;
        case GL_FOG_MODE:
        case GL_FOG_COORDINATE_SOURCE:
            glFogi( pname, ( GLint )param );
            return;
        default:
            _jsSetError( GL_INVALID_ENUM );
            return;
    }
    LContext->needValidate |= PSGL_VALIDATE_FFX_FRAGMENT_PROGRAM;
}

/**
 * @short Specify fog parameters
 *
 * glFogi() assigns the value or values in <I><c>params</c></I> to the fog parameter specified by <I><c>pname</c></I>. It
 * differs from glFogfv() only in argument type.  Refer to glFogfv() for more information.
 *
 * @param pname Specifies a single-valued fog parameter.
 *
 * @param param Specifies the value that <I><c>pname</c></I> will be set to.
 */
GLAPI void APIENTRY glFogi( GLenum pname, GLint param )
{
    JS_APIENTRY_TIMER( glFogi );
#ifdef JS_DPM
    if ( _jsdpmState )
    {
        _JS_DPM_API_ENTRY_2( glFogi, JSDPM_CALL_TYPE_STATE_RASTER, , 2,
                             pname, sizeof( GLenum ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM,
                             param, sizeof( GLint ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE );
    }
#endif
    PSGLcontext*	LContext = _CurrentContext;

    switch ( pname )
    {
        case GL_FOG_MODE:
#ifndef JS_NO_ERROR_CHECK
            if ( !_jsIsFogMode( param ) )
            {
                _jsSetError( GL_INVALID_ENUM );
                return;
            }
#endif
            LContext->FogMode = param;
            break;
        case GL_FOG_DENSITY:
        case GL_FOG_START:
        case GL_FOG_END:
            glFogf( pname, ( GLfloat )param );
            return;
        default:
            _jsSetError( GL_INVALID_ENUM );
            return;
    }
    LContext->needValidate |= PSGL_VALIDATE_FFX_FRAGMENT_PROGRAM;
}

/**
 * @short Specify fog parameters
 *
 * If fog is enabled, fog affects rasterized geometry, bitmaps, and pixel blocks, but not buffer clear
 * operations. To enable and disable fog, call glEnable() and glDisable() with argument
 * <CODE>GL_FOG</CODE>. Fog is initially disabled.
 *
 * glFogfv() assigns the value or values in <I>params</I> to the fog parameter specified by <I><c>pname</c></I>.
 * The following values are accepted for <I><c>pname</c></I>:

 * <TABLE rules=all>
 * <TR>
 * <TD><CODE>GL_FOG_MODE</CODE></TD>
 * <TD><I><c>params</c></I> is a single fixed-point or floating-point value that specifies the equation
 * to be used to compute the fog blend factor <I>f</I>. Three symbolic constants are accepted:
 * <CODE>GL_LINEAR</CODE>, <CODE>GL_EXP</CODE>, and <CODE>GL_EXP2</CODE>. The equations corresponding to these symbolic constants
 * are defined below. The initial fog mode is <CODE>GL_EXP</CODE>.
 * </TD>
 * </TR>
 * <TR>
 * <TD><CODE>GL_FOG_DENSITY</CODE></TD>
 * <TD><I><c>params</c></I> is a single fixed-point or floating-point value that specifies <I>density</I>,
 * the fog density used in both exponential fog equations. Only nonnegative densities are accepted.
 * The initial fog density is 1.</TD>
 * </TR>
 * <TR>
 * <TD><CODE>GL_FOG_START</CODE></TD>
 * <TD><I><c>params</c></I> is a single fixed-point or floating-point value that specifies <I>start</I>, the near distance used in the linear fog equation. The initial near distance is 0.</TD>
 * </TR>
 * <TR>
 * <TD><CODE>GL_FOG_END</CODE></TD>
 * <TD><I><c>params</c></I> is a single fixed-point or floating-point value that specifies <I>end</I>, the far distance used in the linear fog equation. The initial far distance is 1.</TD>
 * </TR>
 * <TR>
 * <TD><CODE>GL_FOG_COLOR</CODE></TD>
 * <TD><I><c>params</c></I> contains four fixed-point or floating-point values that specify Cf, the fog color. Both fixed-point and floating-point values are mapped directly. After conversion, all color components are clamped to the range [0, 1]. The initial fog color is (0, 0, 0, 0).</TD>
 * </TR>
 * </TABLE>

 * <P>
 * Fog blends a fog color with each rasterized pixel fragment&rsquo;s posttexturing color using a
 * blending factor <I>f</I>. Factor <I>f</I> is computed in one of three ways, depending on the fog mode.
 * Let <I>z</I> be the distance in eye coordinates from the origin to the fragment being fogged.
 * The equation for <CODE>GL_LINEAR</CODE> fog is
 * @code
 * f = (end - z)/(end - start)
 * @endcode
 * The equation for <CODE>GL_EXP</CODE> fog is
 * @code
 * f = exp(-(density - z))
 * @endcode
 * The equation for <CODE>GL_EXP2</CODE> fog is
 * @code
 * f = exp(-((density - z)<sup>2</sup>))
 * @endcode
 * Regardless of the fog mode, f is clamped to the range [0, 1] after it is computed.
 * Then, the fragment&rsquo;s red, green, and blue colors, represented by C_r, are replaced by
 * @code
 * C'_r = f * C_r + (1 - f) &times; C_f
 * @endcode
 * Fog does not affect a fragment&rsquo;s alpha component.
 *
 * @param pname Specifies a fog parameter. <CODE>GL_FOG_MODE</CODE>, <CODE>GL_FOG_DENSITY</CODE>,
 * <CODE>GL_FOG_START</CODE>, <CODE>GL_FOG_END</CODE>, and <CODE>GL_FOG_COLOR</CODE> are accepted.
 *
 * @param params Specifies the value or values to be assigned to <I><c>pname</c></I>.
 * <CODE>GL_FOG_COLOR</CODE> requires an array of four values. All other parameters
 * accept an array containing only a single value.
 *
 * @par Errors
Use glGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>GL_INVALID_ENUM</CODE></TD>
<TD><I><c>pname</c></I> is not an accepted value, or if <I><c>pname</c></I> is
<CODE>GL_FOG_MODE</CODE> and <I><c>params</c></I> is not an accepted value.</TD>
</TR>
<TR>
<TD><CODE>GL_INVALID_VALUE</CODE></TD>
<TD><I><c>pname</c></I> is <CODE>GL_FOG_DENSITY</CODE>, and <I><c>params</c></I> is negative.</TD>
</TR>
</TABLE>
 *
 * @sa glEnable
 */

GLAPI void APIENTRY glFogfv( GLenum pname, const GLfloat *params )
{
    JS_APIENTRY_TIMER( glFogfv );
#ifdef JS_DPM
    if ( _jsdpmState )
    {
        _JS_DPM_API_ENTRY_2( glFogfv, JSDPM_CALL_TYPE_STATE_RASTER, , 2,
                             pname, sizeof( GLenum ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM,
                             params, ( pname == GL_FOG_COLOR ? 4 : 1 )*sizeof( GLfloat ), INPUT_BY_REFERENCE, DPM_API_ARGUMENT_POINTER );
    }
#endif
    PSGLcontext*	LContext = _CurrentContext;
    switch ( pname )
    {
        case GL_FOG_COLOR:
            LContext->FogColor.R = _jsClampf( params[0] );
            LContext->FogColor.G = _jsClampf( params[1] );
            LContext->FogColor.B = _jsClampf( params[2] );
            LContext->FogColor.A = _jsClampf( params[3] );
            break;
        case GL_FOG_DENSITY:
        case GL_FOG_START:
        case GL_FOG_END:
        case GL_FOG_MODE:
        case GL_FOG_COORDINATE_SOURCE:
            glFogf( pname, params[0] );
            return;
        default:
            _jsSetError( GL_INVALID_ENUM );
            return;
    }
    LContext->needValidate |= PSGL_VALIDATE_FFX_FRAGMENT_PROGRAM;
}

/**
 * @short Specify fog parameters
 *
 * glFogiv() assigns the value or values in <I><c>params</c></I> to the fog parameter specified by <I><c>pname</c></I>. It
 * differs from glFogfv() only in argument type.  Refer to glFogfv() for more information.
 *
 * @param pname Specifies a fog parameter.
 *
 * @param params Specifies the value or values to be assigned to <I><c>pname</c></I>.
 */
GLAPI void APIENTRY glFogiv( GLenum pname, const GLint *params )
{
    JS_APIENTRY_TIMER( glFogiv );
#ifdef JS_DPM
    if ( _jsdpmState )
    {
        _JS_DPM_API_ENTRY_2( glFogiv, JSDPM_CALL_TYPE_STATE_RASTER, , 2,
                             pname, sizeof( GLenum ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM,
                             params, ( pname == GL_FOG_COLOR ? 4 : 1 )*sizeof( GLint ), INPUT_BY_REFERENCE, DPM_API_ARGUMENT_POINTER );
    }
#endif
    PSGLcontext*	LContext = _CurrentContext;
    switch ( pname )
    {
        case GL_FOG_COLOR:
        {
            const GLfloat f = 1.f / INT_MAX;
            LContext->FogColor.R = _jsClampf(( GLfloat )params[0] * f );
            LContext->FogColor.G = _jsClampf(( GLfloat )params[1] * f );
            LContext->FogColor.B = _jsClampf(( GLfloat )params[2] * f );
            LContext->FogColor.A = _jsClampf(( GLfloat )params[3] * f );
        }
        break;
        case GL_FOG_DENSITY:
        case GL_FOG_START:
        case GL_FOG_END:
        case GL_FOG_MODE:
        case GL_FOG_COORDINATE_SOURCE:
            glFogi( pname, params[0] );
            return;
        default:
            _jsSetError( GL_INVALID_ENUM );
            return;
    }
    LContext->needValidate |= PSGL_VALIDATE_FFX_FRAGMENT_PROGRAM;
}
/** @} OpenGL */
