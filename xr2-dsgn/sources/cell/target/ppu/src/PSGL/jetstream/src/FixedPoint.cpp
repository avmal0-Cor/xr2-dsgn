/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 *                Copyright (C) 2009 Sony Computer Entertainment Inc.
 *                                               All Rights Reserved.
 */


/*
 * Jetstream graphics API fixed point entrypoints.
 * These fixed-point functions exist to satisfy the OpenGL ES 1.0 conformance
 * test suite; they are not recommended for use in production applications.
 */


#include <PSGL/psgl.h>
#include <PSGL/private.h>
#include <PSGL/TypeUtils.h>


// The routines in this file should not be instrumented for debug runtime (_JS_DPM_*) capture/trace.


/** @addtogroup Fixed
 *
 * @{
 */

/**
 * @short Specify the alpha test function (Not recommended for use)
 *
 * This function specifies the alpha test function.  It is the fixed point version of
 * glAlphaFunc() and differs only in argument type and performance.
 *
 * @param func Specifies the alpha comparison function.
 *
 * @param ref Specifies the reference value that incoming alpha values are
 *            compared to.
 *
 *
 * @note
 * For best performance, do not use this function. Use glAlphaFunc() instead.
 *
 * @sa glAlphaFunc
 */
GLAPI void APIENTRY glAlphaFuncx( GLenum func, GLclampx ref )
{
    JS_APIENTRY_TIMER( glAlphaFuncx );
    glAlphaFunc( func, X2F( ref ) );
}

/**
 * @short Specify clear values for the color buffers (Not recommended for use)
 *
 * This function specifies the red, green, blue, and alpha values used by
 * glClear() to clear the color buffer. If differs from glClearColor() only in argument type and performance.
 *
 * @param red  red value to use when the color buffer is cleared.
 * @param blue  blue value to use when the color buffer is cleared.
 * @param green  green value to use when the color buffer is cleared.
 * @param alpha  alpha value to use when the color buffer is cleared.
 *
 * @note
 * For best performance, do not use this function. Use glClearColor() instead.
 *
 * @sa glClearColor
 */
GLAPI void APIENTRY glClearColorx( GLclampx red, GLclampx green, GLclampx blue, GLclampx alpha )
{
    JS_APIENTRY_TIMER( glClearColorx );
    glClearColor( X2F( red ), X2F( green ), X2F( blue ), X2F( alpha ) );
}

/**
 * @short Specify the clear value for the depth buffer (Not recommended for use)
 *
 * glClearDepthx() specifies the depth value used by glClear() to clear the depth buffer. It differs
 * from glClearDepthf() only in the argument type and performance.
 *
 * @param depth Specifies the depth value used when the depth buffer is cleared.
 *
 * @note
 * For best performance, do not use this function. Use glClearDepthf() instead.
 *
 * @sa glClearDepthf
 */
GLAPI void APIENTRY glClearDepthx( GLclampx depth )
{
    JS_APIENTRY_TIMER( glClearDepthx );
    glClearDepthf( X2F( depth ) );
}

/**
 * @short Set the current color (Not recommended for use)
 *
 *  This function sets a new four-valued RGBA color as the current color.  It differs from glColor4f() only in
 *  argument type and performance.
 *
 *  @param red  New red value (0.0f to 1.0f)
 *  @param green  New green value (0.0f to 1.0f)
 *  @param blue  New blue value (0.0f to 1.0f)
 *  @param alpha New alpha value (0.0f to 1.0f)
 *
 * @note
 * For best performance, do not use this function. Use glColor4f() instead.
 *
 * @sa glColor4f
 */
GLAPI void APIENTRY glColor4x( GLfixed red, GLfixed green, GLfixed blue, GLfixed alpha )
{
    JS_APIENTRY_TIMER( glColor4x );
    glColor4f( X2F( red ), X2F( green ), X2F( blue ), X2F( alpha ) );
}


/**
 *  @short Set the current normal vector (Not recommended for use)
 *
 *  glNormal3x() sets the current normal to the given coordinates. It differs from glNormal3f()
 *  only in argument type and performance.
 *
 *  @param x  Specifies the <I>x</I> coordinate of the new current normal.
 *  @param y  Specifies the <I>y</I> coordinate of the new current normal.
 *  @param z  Specifies the <I>z</I> coordinate of the new current normal.
 *
 * @note
 * For best performance, do not use this function. Use glNormal3f() instead.
 *
 * @sa glNormal3f
 */
GLAPI void APIENTRY glNormal3x( GLfixed x, GLfixed y, GLfixed z )
{
    JS_APIENTRY_TIMER( glNormal3x );
    glNormal3f( X2F( x ), X2F( y ), X2F( z ) );
}


/**
 * @short Set the current texture coordinates (Not recommended for use)
 *
 *  glMultiTexCoord4x() specifies the four texture coordinates as (<I>s, t, r, q</I>).  It is identical
 *  to glMultiTexCoord4f() only in argument type and performance.
 *
 * @param target   Specifies texture unit whose coordinates should be modified.
 * @param s  Specifies the <I>s</I> texture coordinate for <I><c>target</c></I> texture unit.
 * @param t  Specifies the <I>t</I> texture coordinate for <I><c>target</c></I> texture unit.
 * @param r  Specifies the <I>r</I> texture coordinate for <I><c>target</c></I> texture unit.
 * @param q  Specifies the <I>q</I> texture coordinate for <I><c>target</c></I> texture unit.
 *
 * @note
 * For best performance, do not use this function. Use glMultiTexCoord4f() instead.
 *
 * @sa glMultiTexCoord4f
 */
GLAPI void APIENTRY glMultiTexCoord4x( GLenum target, GLfixed s, GLfixed t, GLfixed r, GLfixed q )
{
    JS_APIENTRY_TIMER( glMultiTexCoord4x );
    glMultiTexCoord4f( target, X2F( s ), X2F( t ), X2F( r ), X2F( q ) );
}


/**
 * @short Specify mapping of depth values from normalized depth coordinates to window coordinates (Not recommended for use)
 *
 * This function specifies a linear mapping of the normalized depth coordinates to window coordinates.
 * It differs from glDepthRangef() only in argument type and performance.
 *
 * @param zNear Specifies the mapping of the near clipping plane to window
 *        coordinates.
 * @param zFar Specifies the mapping of the far clipping plane to window
 *        coordinates.

 * @note
 * For best performance, do not use this function. Use glDepthRangef() instead.
 *
 * @sa glDepthRangef
 */
GLAPI void APIENTRY glDepthRangex( GLclampx zNear, GLclampx zFar )
{
    JS_APIENTRY_TIMER( glDepthRangex );
    glDepthRangef( X2F( zNear ), X2F( zFar ) );
}

/**
 * @short Specify fog parameters (Not recommended for use)
 *
 * glFogx() assigns the value or values in <I><c>params</c></I> to the fog parameter specified by <I><c>pname</c></I>. It
 * differs from glFogfv() only in argument type and performance.
 *
 * @param pname Specifies a fog parameter.
 *
 * @param param Specifies the value or values to be assigned to <I><c>pname</c></I>.
 *
 * @note
 * For best performance, do not use this function. Use the glFogf() instead.
 *
 * @sa glFogf
 */
GLAPI void APIENTRY glFogx( GLenum pname, GLfixed param )
{
    JS_APIENTRY_TIMER( glFogx );
    switch ( pname )
    {
        case GL_FOG_MODE:
        case GL_FOG_COORDINATE_SOURCE:
            // param is an enum. enum passed as a fixed are not scaled...
            glFogi( pname, param );
            return;
        case GL_FOG_DENSITY:
        case GL_FOG_START:
        case GL_FOG_END:
            glFogf( pname, X2F( param ) );
            return;
        default:
            _jsSetError( GL_INVALID_ENUM );
            return;
    }
}

/**
 * @short Specify fog parameters (Not recommended for use)
 *
 * glFogxv() assigns the value or values in <I><c>params</c></I> to the fog parameter specified by <I><c>pname</c></I>. It
 * differs from glFogfv() only in argument type and performance.
 *
 * @param pname Specifies a fog parameter.
 *
 * @param params Specifies the value or values to be assigned to <I><c>pname</c></I>.
 *
 * @note
 * For best performance, do not use this function. Use the glFogfv() instead.
 *
 * @sa glFogfv
 */
GLAPI void APIENTRY glFogxv( GLenum pname, const GLfixed *params )
{
    JS_APIENTRY_TIMER( glFogxv );
    GLfloat converted[4];
    int count, i;
    switch ( pname )
    {
        case GL_FOG_MODE:
        case GL_FOG_COORDINATE_SOURCE:
            // param is an enum. enum passed as a fixed are not scaled...
            glFogi( pname, params[0] );
            return;
        case GL_FOG_DENSITY:
        case GL_FOG_START:
        case GL_FOG_END:
            count = 1;
            break;
        case GL_FOG_COLOR:
            count = 4;
            break;
        default:
            _jsSetError( GL_INVALID_ENUM );
            return;
    }
    for ( i = 0;i < count;++i )
    {
        converted[i] = X2F( params[i] );
    }
    glFogfv( pname, converted );
}

/**
 * @short Multiply the current matrix by a perspective matrix (Not recommended for use)
 *
 * glFrustumx() describes a perspective matrix that produces a perspective projection. It
 * differs from glFrustumf() only in argument type and performance.
 *
 * @param left,right   Specify the coordinates for the left and right vertical clipping planes.
 * @param bottom,top   Specify the coordinates for the bottom and top horizontal clipping planes.
 * @param zNear    Specify the distance to the near depth clipping planes
 * @param zFar    Specify the distances to the far depth clipping plane.
 *
 * @note
 * For best performance, do not use this function. Use glFrustumf() instead.
 *
 * @sa glFrustumf
 */
GLAPI void APIENTRY glFrustumx( GLfixed left, GLfixed right, GLfixed bottom, GLfixed top, GLfixed zNear, GLfixed zFar )
{
    JS_APIENTRY_TIMER( glFrustumx );
    glFrustumf( X2F( left ), X2F( right ), X2F( bottom ), X2F( top ), X2F( zNear ), X2F( zFar ) );
}

/**
 *  @short Set light source parameters (Not recommended for use)
 *
 *  glLightx() sets the values of individual light source parameters.  It differs from glLightfv()
 *  except for the type
 *  of arguments and performance.
 *
 *  @param light Specifies a light. The number of lights depends on the implementation,
 *  but at least eight lights are supported.
 *
 *  @param pname Specifies a single-valued light source parameter for <I><c>light</c></I>.
 *
 *  @param param Specifies the value that parameter <I><c>pname</c></I> of light source
 *  <I><c>light</c></I> will be set to.
 *
 * @note
 * For best performance, do not use this function. Use glLightf() instead.
 *
 * @sa glLightf
 */
GLAPI void APIENTRY glLightx( GLenum light, GLenum pname, GLfixed param )
{
    JS_APIENTRY_TIMER( glLightx );
    glLightf( light, pname, X2F( param ) );
}

/**
 *  @short Set light source parameters (Not recommended for use)
 *
 *  glLightxv() sets the values of individual light source parameters.  It differs from glLightfv()
 *  except for the type
 *  of arguments and performance.
 *
 *  @param light Specifies a light. The number of lights depends on the implementation,
 *  but at least eight lights are supported.
 *
 *  @param pname Specifies a single-valued light source parameter for <I><c>light</c></I>.
 *
 *  @param params Specifies a pointer to the value or values that parameter <I><c>pname</c></I>
 *  of light source <I><c>light</c></I> will be set to.
 *
 * @note
 * For best performance, do not use this function. Use glLightfv() instead.
 *
 * @sa glLightfv
 */
GLAPI void APIENTRY glLightxv( GLenum light, GLenum pname, const GLfixed* params )
{
    JS_APIENTRY_TIMER( glLightxv );
    GLfloat converted[4];
    int count, i;
    switch ( pname )
    {
        case GL_AMBIENT:
        case GL_DIFFUSE:
        case GL_SPECULAR:
        case GL_POSITION:
            count = 4;
            break;
        case GL_SPOT_DIRECTION:
            count = 3;
            break;
        case GL_SPOT_EXPONENT:
        case GL_SPOT_CUTOFF:
        case GL_CONSTANT_ATTENUATION:
        case GL_LINEAR_ATTENUATION:
        case GL_QUADRATIC_ATTENUATION:
            count = 1;
            break;
        default:
            _jsSetError( GL_INVALID_ENUM );
            return;
    }
    for ( i = 0;i < count;++i )
    {
        converted[i] = X2F( params[i] );
    }
    glLightfv( light, pname, converted );
}


/**
 *  @short Set the lighting model parameters (Not recommended for use)
 *
 *  glLightModelx() sets the lighting model parameter.  It differs from glLightModelfv() except for the type
 *  of arguments and performance.
 *
 *  @param pname Specifies a single-valued lighting model parameter.
 *  Must be <CODE>GL_LIGHT_MODEL_TWO_SIDE</CODE>.
 *
 *  @param param Specifies the value that <I><c>param</c></I> will be set to.
 *
 * @note
 * For best performance, do not use this function. Use glLightModelf() instead.
 *
 * @sa glLightModelf
 */
GLAPI void APIENTRY glLightModelx( GLenum pname, GLfixed param )
{
    JS_APIENTRY_TIMER( glLightModelx );
    glLightModelf( pname, X2F( param ) );
}

/**
 *  @short Set the lighting model parameters (Not recommended for use)
 *
 *  glLightModelxv() sets the lighting model parameter.  It differs from glLightModelfv() except for the type
 *  of arguments and performance.
 *
 *  @param pname Specifies a single-valued lighting model parameter.
 *  Must be <CODE>GL_LIGHT_MODEL_TWO_SIDE</CODE>.
 *
 *  @param params  Specifies a pointer to the value or values that <I><c>pname</c></I> will be set to.
 *
 * @note
 * For best performance, do not use this function. Use glLightModelfv() instead.
 *
 * @sa glLightModelfv
 */
GLAPI void APIENTRY glLightModelxv( GLenum pname, const GLfixed *params )
{
    JS_APIENTRY_TIMER( glLightModelxv );
    GLfloat converted[4];
    int count, i;
    switch ( pname )
    {
        case GL_LIGHT_MODEL_AMBIENT:
            count = 4;
            break;
        case GL_LIGHT_MODEL_TWO_SIDE:
            count = 1;
            break;
        default:
            _jsSetError( GL_INVALID_ENUM );
            return;
    }
    for ( i = 0;i < count;++i )
    {
        converted[i] = X2F( params[i] );
    }
    glLightModelfv( pname, converted );
}

/**
 *  @short Specify the width of rasterized lines (Not recommended for use)
 *
 * glLineWidthx() specifies the rasterized width of both aliased and antialiased lines. It is
 * identical to glLineWidth() only in argument type and performance.
 *
 *  @param width Specifies the width of rasterized lines. The initial value is 1.
 *
 * @note
 * For best performance, do not use this function. Use glLineWidth() instead.
 *
 * @sa glLineWidth
 */
GLAPI void APIENTRY glLineWidthx( GLfixed width )
{
    JS_APIENTRY_TIMER( glLineWidthx );
    glLineWidth( X2F( width ) );
}

/**
 * @short Replace the current matrix with the specified matrix (Not recommended for use)
 *
 * glLoadMatrixx() replaces the current matrix with the one whose elements are specified by <I><c>m</c></I>. It is
 * identical to glLoadMatrixf() only in argument type and performance.
 *
 * @param  m  Specifies a pointer to 16 consecutive values, which are used as the elements of a 4 X 4 column-major matrix.
 *
 * @note
 * For best performance, do not use this function. Use glLoadMatrixf() instead.
 *
 * @sa glLoadMatrixf
 */
GLAPI void APIENTRY glLoadMatrixx( const GLfixed *m )
{
    JS_APIENTRY_TIMER( glLoadMatrixx );
    GLfloat mf[16];
    int i;
    for ( i = 0;i < 16;++i ) mf[i] = X2F( m[i] );
    glLoadMatrixf( mf );
}

/**
 *  @short Set material parameters for the lighting model (Not recommended for use)
 *
 *  glMaterialx() assigns values to material parameters.  It differs from glMaterialfv() only in argument
 *  type and performance.
 *
 *  @param face Specifies which face or faces are being updated.
 *
 *  @param pname Specifies the single-valued material parameter of the face or faces that is being updated.
 *
 *  @param param Specifies the value that parameter <CODE>GL_SHININESS</CODE> will be set to.
 *
 * @note
 * For best performance, do not use this function. Use glMaterialf() instead.
 *
 * @sa glMaterialf
 */
GLAPI void APIENTRY glMaterialx( GLenum face, GLenum pname, GLfixed param )
{
    JS_APIENTRY_TIMER( glMaterialx );
    glMaterialf( face, pname, X2F( param ) );
}

/**
 *  @short Set material parameters for the lighting model (Not recommended for use)
 *
 *  glMaterialxv() assigns values to material parameters.  It differs from glMaterialfv() only in argument
 *  type and performance.
 *
 *  @param face Specifies which face or faces are being updated.
 *
 *  @param pname Specifies the single-valued material parameter of the face or faces that is being updated.
 *
 *  @param params  Specifies a pointer to the value or values that <I><c>pname</c></I> will be set to.
 *
 * @note
 * For best performance, do not use this function. Use glMaterialfv() instead.
 *
 * @sa glMaterialfv
 */
GLAPI void APIENTRY glMaterialxv( GLenum face, GLenum pname, const GLfixed *params )
{
    JS_APIENTRY_TIMER( glMaterialxv );
    GLfloat converted[4];
    int count, i;
    switch ( pname )
    {
        case GL_AMBIENT:
        case GL_DIFFUSE:
        case GL_SPECULAR:
        case GL_EMISSION:
        case GL_AMBIENT_AND_DIFFUSE:
            count = 4;
            break;
        case GL_SHININESS:
            count = 1;
            break;
        default:
            _jsSetError( GL_INVALID_ENUM );
            return;
    }
    for ( i = 0;i < count;++i )
    {
        converted[i] = X2F( params[i] );
    }
    glMaterialfv( face, pname, converted );
}

/**
 * @short Multiply the current matrix with the specified matrix (Not recommended for use)
 *
 * glMultMatrixx() multiplies the current matrix with the one specified using <I><c>m</c></I>,
 * and replaces the current matrix with the product. It differs from glMultMatrixf() only in argument
 * type and performance.
 *
 * @param  m  Points to 16 consecutive values that are used as the elements of a 4 X 4 column-major matrix.
 *
 * @note
 * For best performance, do not use this function. Use glMultMatrixf() instead.
 *
 * @sa glMultMatrixf
 */
GLAPI void APIENTRY glMultMatrixx( const GLfixed* m )
{
    JS_APIENTRY_TIMER( glMultMatrixx );
    GLfloat mf[16];
    int i;
    for ( i = 0;i < 16;++i ) mf[i] = X2F( m[i] );
    glMultMatrixf( mf );
}

/**
 * @short Multiply the current matrix with an orthographic matrix (Not recommended for use)
 *
 * glOrthox() describes a transformation that produces a parallel projection. It differs from
 * glOrthof() only in argument type and performance.
 *
 * @param left     Specify the coordinates for the left vertical clipping plane.
 * @param right    Specify the coordinates for the right vertical clipping plane.
 * @param bottom   Specify the coordinates for the bottom horizontal clipping plane.
 * @param top      Specify the coordinates for the top horizontal clipping plane.
 * @param zNear    Specify the distances to the near depth clipping plane.
 * @param zFar     Specify the distances to the far depth clipping plane.
 *
 * @note
 * For best performance, do not use this function. Use glOrthof() instead.
 *
 * @sa glOrthof
 */
GLAPI void APIENTRY glOrthox( GLfixed left, GLfixed right, GLfixed bottom, GLfixed top, GLfixed zNear, GLfixed zFar )
{
    JS_APIENTRY_TIMER( glOrthox );
    glOrthof( X2F( left ), X2F( right ), X2F( bottom ), X2F( top ), X2F( zNear ), X2F( zFar ) );
}

/**
 * @short Specify the diameter of rasterized points (Not recommended for use)
 *
 * glPointSizex() specifies the rasterized diameter of both aliased and antialiased points.
 * It differs from glPointSize() only in argument type and performance.
 *
 * @param size Specifies the diameter of rasterized points.
 *
 * @note
 * For best performance, do not use this function. Use glPointSize() instead.
 *
 * @sa glPointSize
 */
GLAPI void APIENTRY glPointSizex( GLfixed size )
{
    JS_APIENTRY_TIMER( glPointSizex );
    glPointSize( X2F( size ) );
}

/**
 * @short Set the scale and units used to calculate depth values (Not recommended for use)
 *
 * glPolygonOffsetx() sets the scale and units used to calculate depth values. It differs from glPolygonOffset()
 * only in argument type and performance.
 *
 * @param factor Specifies a scale factor that is used to create a variable depth offset
 * for each polygon.
 * @param units Is multiplied by an implementation-specific value to create a constant depth offset.
 *
 * @note
 * For best performance, do not use this function. Use glPolygonOffset() instead.
 *
 * @sa glPolygonOffset
 */
GLAPI void APIENTRY glPolygonOffsetx( GLfixed factor, GLfixed units )
{
    JS_APIENTRY_TIMER( glPolygonOffsetx );
    glPolygonOffset( X2F( factor ), X2F( units ) );
}

/**
 * @short Multiply the current matrix by a rotation matrix (Not recommended for use)
 *
 * glRotatex() produces a rotation of <I><c>angle</c></I> degrees around the vector (<I><c>x, y, z</c></I>). It is identical
 * to glRotatef() only in argument type and performance.
 *
 * @param angle  Specifies the angle of rotation, in degrees.
 * @param x,y,z  Specify the <I>x</I>, <I>y</I>, and <I>z</I> coordinates of a vector, respectively.
 *
 * @note
 * For best performance, do not use this function. Use glRotatef() instead.
 *
 * @sa glRotatef
 */
GLAPI void APIENTRY glRotatex( GLfixed angle, GLfixed x, GLfixed y, GLfixed z )
{
    JS_APIENTRY_TIMER( glRotatex );
    glRotatef( X2F( angle ), X2F( x ), X2F( y ), X2F( z ) );
}

/**
 * @short Specify mask to modify multisampled pixel fragments (Not recommended for use)
 *
 * glSampleCoveragex() defines a mask to modify the coverage of multisampled pixel fragments.
 * It differs from glSampleCoverage() only in argument type and performance.
 *
 * @param value Specifies the coverage of the modification mask.
 * @param invert Specifies whether the modification mask implied by value is inverted or not.
 *
 * @note
 * For best performance, do not use this function. Use glSampleCoverage() instead.
 *
 * @sa glSampleCoverage
 */
GLAPI void APIENTRY glSampleCoveragex( GLclampx value, GLboolean invert )
{
    JS_APIENTRY_TIMER( glSampleCoveragex );
    glSampleCoverage( X2F( value ), invert );
}

/**
 * @short Multiply the current matrix by a general scaling matrix (Not recommended for use)
 *
 * glScalex() produces a nonuniform scaling along the <I>LXScale</I>, <I>LYScale</I>, and <I>LZScale</I> axes.
 * It differs from glScalef() only in argument type and performance.
 *
 * @param x  Specifies the scale factor along the <I>LXScale</I> axis.
 * @param y  Specifies the scale factor along the <I>LYScale</I> axis.
 * @param z  Specifies the scale factor along the <I>LZScale</I> axis.
 *
 * @note
 * For best performance, do not use this function. Use glScalef() instead.
 *
 * @sa glScalef
 */
GLAPI void APIENTRY glScalex( GLfixed x, GLfixed y, GLfixed z )
{
    JS_APIENTRY_TIMER( glScalex );
    glScalef( X2F( x ), X2F( y ), X2F( z ) );
}

/**
 *  @short Set texture environment parameter (Not recommended for use)
 *
 *  glTexEnvx() sets texture environment parameters.  It differs from glTexEnvfv() only in argument type.
 *  and performance.  Please refer to glTexEnvfv() for more information.
 *
 *  @param target Specifies a texture environment.
 *  @param pname  Specifies the symbolic name of a single-valued texture environment
 *  parameter.
 *  @param param  If <I><c>pname</c></I> is <CODE>GL_TEXTURE_ENV_MODE</CODE>, then
 *  <I><c>param</c></I> specifies a single symbolic constant
 *  If <I><c>pname</c></I> is <CODE>GL_COORD_REPLACE_OES</CODE>, then
 *  <I><c>param</c></I> is either <CODE>GL_TRUE</CODE> or <CODE>GL_FALSE</CODE>.
 *
 * @note
 * For best performance, do not use this function. Use glTexEnvf() instead.
 *
 * @sa glTexEnvf
 */
GLAPI void APIENTRY glTexEnvx( GLenum target, GLenum pname, GLfixed param )
{
    JS_APIENTRY_TIMER( glTexEnvx );
    switch ( pname )
    {
        case GL_TEXTURE_ENV_MODE:
            // param is an enum. enum passed as a fixed are not scaled...
            glTexEnvi( target, pname, param );
            return;
        default:
            _jsSetError( GL_INVALID_ENUM );
            return;
    }
}

/**
 *  @short Set texture environment parameter (Not recommended for use)
 *
 *  glTexEnvxv() sets texture environment parameters.  It differs from glTexEnvfv() only in argument type.
 *  and performance.  Please refer to glTexEnvfv() for more information.
 *
 *  @param target Specifies a texture environment.
 *  @param pname  Specifies the symbolic name of a single-valued texture environment
 *  parameter.
 *  @param params  If <I><c>pname</c></I> is <CODE>GL_TEXTURE_ENV_MODE</CODE>, then
 *  <I><c>params</c></I> specifies a single symbolic constant, one of
 *  <CODE>GL_MODULATE</CODE>, <CODE>GL_DECAL</CODE>, <CODE>GL_BLEND</CODE>,
 *  <CODE>GL_REPLACE</CODE>, or <CODE>GL_ADD</CODE>.
 *  If <I><c>pname</c></I> is <CODE>GL_TEXTURE_ENV_COLOR</CODE>, then
 *  <I><c>params</c></I> is a 4-valued color vector.
 *  If <I><c>pname</c></I> is <CODE>GL_COORD_REPLACE_OES</CODE>, then
 *  <I><c>params</c></I> is either <CODE>GL_TRUE</CODE> or <CODE>GL_FALSE</CODE>.
 *
 * @note
 * For best performance, do not use this function. Use glTexEnvfv() instead.
 *
 * @sa glTexEnvfv
 */
GLAPI void APIENTRY glTexEnvxv( GLenum target, GLenum pname, const GLfixed *params )
{
    JS_APIENTRY_TIMER( glTexEnvxv );
    if ( target != GL_TEXTURE_ENV )
    {
        _jsSetError( GL_INVALID_ENUM );
        return;
    }
    GLfloat converted[4];
    int count, i;
    switch ( pname )
    {
        case GL_TEXTURE_ENV_MODE:
            // param is an enum. enum passed as a fixed are not scaled...
            glTexEnvi( target, pname, params[0] );
            return;
        case GL_TEXTURE_ENV_COLOR:
            count = 4;
            break;
        default:
            _jsSetError( GL_INVALID_ENUM );
            return;
    }
    for ( i = 0;i < count;++i )
    {
        converted[i] = X2F( params[i] );
    }
    glTexEnvfv( target, pname, converted );
}

/**
@short Set texture parameters (Not recommended for use)

 *  glTexParameterx() assigns the value or values in <I><c>param</c></I> to the texture parameter specified as
 *  <I><c>pname</c></I>.  It differs from glTexParameterf() only in argument type and performance.
 *  Please refer to glTexParameterf() for more information.

 *  @param target  Specifies the target texture.
 *
 *  @param pname   Specifies the symbolic name of a texture parameter.
 *
 *  @param param   Specifies the value of <I><c>pname</c></I>.
 *
 * @note
 * For best performance, do not use this function. Use glTexParameterf() instead.
 *
 * @sa glTexParameterf
 */
GLAPI void APIENTRY glTexParameterx( GLenum target, GLenum pname, GLfixed param )
{
    JS_APIENTRY_TIMER( glTexParameterx );
    switch ( pname )
    {
        case GL_TEXTURE_MIN_FILTER:
        case GL_TEXTURE_MAG_FILTER:
        case GL_TEXTURE_WRAP_S:
        case GL_TEXTURE_WRAP_T:
        case GL_TEXTURE_WRAP_R:
            // param is an enum. enum passed as a fixed are not scaled...
            glTexParameteri( target, pname, ( GLint )param );
            break;
            /*        case GL_TEXTURE_BASE_LEVEL:*/
            /*        case GL_TEXTURE_MAX_LEVEL:*/
        case GL_TEXTURE_MIN_LOD:
        case GL_TEXTURE_MAX_LOD:
        default:
            glTexParameterf( target, pname, X2F( param ) );
            break;
    }
}

/**
 * @short Multiply the current matrix by a translation matrix (Not recommended for use)
 *
 * glTranslatex() produces a translation by (<I>LX, LY, LZ</I>). It differs from glTranslatef() except
 * for argument type and performance.
 *
 * @param x  Specify the <I>LX</I> coordinate of a translation vector
 * @param y  Specify the <I>LY</I> coordinate of a translation vector
 * @param z  Specify the <I>LZ</I> coordinate of a translation vector

 * @note
 * For best performance, do not use this function. Use glTranslatef() instead.
 *
 * @sa glTranslatef
 */
GLAPI void APIENTRY glTranslatex( GLfixed x, GLfixed y, GLfixed z )
{
    JS_APIENTRY_TIMER( glTranslatex );
    glTranslatef( X2F( x ), X2F( y ), X2F( z ) );
}

/** @} Fixed */
