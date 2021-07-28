/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 *                Copyright (C) 2009 Sony Computer Entertainment Inc.
 *                                               All Rights Reserved.
 */

 
#include <PSGL/psgl.h>
#include <PSGL/private.h>
#include <PSGL/Matrix.h>
#include <PSGL/DPM2.h>
#include <limits.h>

void _jsSetTexGenEyePlane( jsTexGen *texGen, const float *param )
{
    if ( !_CurrentContext->InverseModelViewValid )
    {
        jsMatrixInverse(
            _CurrentContext->ModelViewMatrixStack.MatrixStackf +
            _CurrentContext->ModelViewMatrixStack.MatrixStackPtr*jsELEMENTS_IN_MATRIX,
            _CurrentContext->InverseModelViewMatrixf );

        _CurrentContext->InverseModelViewValid = GL_TRUE;
    }

    texGen->eyePlane[0] =
        param[0] * _CurrentContext->InverseModelViewMatrixf[M00] +
        param[1] * _CurrentContext->InverseModelViewMatrixf[M10] +
        param[2] * _CurrentContext->InverseModelViewMatrixf[M20] +
        param[3] * _CurrentContext->InverseModelViewMatrixf[M30];
    texGen->eyePlane[1] =
        param[0] * _CurrentContext->InverseModelViewMatrixf[M01] +
        param[1] * _CurrentContext->InverseModelViewMatrixf[M11] +
        param[2] * _CurrentContext->InverseModelViewMatrixf[M21] +
        param[3] * _CurrentContext->InverseModelViewMatrixf[M31];
    texGen->eyePlane[2] =
        param[0] * _CurrentContext->InverseModelViewMatrixf[M02] +
        param[1] * _CurrentContext->InverseModelViewMatrixf[M12] +
        param[2] * _CurrentContext->InverseModelViewMatrixf[M22] +
        param[3] * _CurrentContext->InverseModelViewMatrixf[M32];
    texGen->eyePlane[3] =
        param[0] * _CurrentContext->InverseModelViewMatrixf[M03] +
        param[1] * _CurrentContext->InverseModelViewMatrixf[M13] +
        param[2] * _CurrentContext->InverseModelViewMatrixf[M23] +
        param[3] * _CurrentContext->InverseModelViewMatrixf[M33];
}
/** @addtogroup OpenGL
 *
 * @{
 */
/**
 *  @short Set texture environment parameter
 *
 *  glTexEnvi() sets texture environment parameters.  It differs from glTexEnvfv() only in argument type.
 *  Please refer to glTexEnvfv() for more information.
 *
 *  @param target Specifies a texture environment.
 *  Must be either <CODE>GL_TEXTURE_ENV</CODE> or <CODE>GL_POINT_SPRITE_OES</CODE>.
 *  @param pname  Specifies the symbolic name of a single-valued texture environment
 *  parameter. Accepted values are <CODE>GL_TEXTURE_ENV_MODE</CODE>,
 *  or <CODE>GL_COORD_REPLACE_OES</CODE>.
 *  @param param  If <I><c>pname</c></I> is <CODE>GL_TEXTURE_ENV_MODE</CODE>, then
 *  <I><c>param</c></I> specifies a single symbolic constant, one of
 *  <CODE>GL_MODULATE</CODE>, <CODE>GL_DECAL</CODE>, <CODE>GL_BLEND</CODE>,
 *  <CODE>GL_REPLACE</CODE>, or <CODE>GL_ADD</CODE>.
 *  If <I><c>pname</c></I> is <CODE>GL_COORD_REPLACE_OES</CODE>, then
 *  <I><c>param</c></I> is either <CODE>GL_TRUE</CODE> or <CODE>GL_FALSE</CODE>.
 */

GLAPI void APIENTRY glTexEnvi( GLenum target, GLenum pname, GLint param )
{
    JS_APIENTRY_TIMER( glTexEnvi );
    if ( _jsdpmState )
    {
        _JS_DPM_API_ENTRY_3( glTexEnvi, JSDPM_CALL_TYPE_STATE_TEX, , 3,
                             target, sizeof( GLenum ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM,
                             pname, sizeof( GLenum ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM,
                             param, sizeof( GLint ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE );
    }

    PSGLcontext*	LContext = _CurrentContext;
    JS_ASSERT( LContext );
    switch ( target )
    {
        case GL_TEXTURE_FILTER_CONTROL:
            glTexEnvf( target, pname, ( GLfloat )param );
            return;
        case GL_TEXTURE_ENV:
        {
#ifndef JS_NO_ERROR_CHECK
            if (( pname != GL_TEXTURE_ENV_MODE ) || ( !_jsIsTexEnvMode( param ) ) )
            {
                _jsSetError( GL_INVALID_ENUM );
                return;
            }
            if ( !LContext->CurrentImageUnit )
            {
                _jsSetError( GL_INVALID_OPERATION );
                return;
            }
#endif
            LContext->CurrentImageUnit->envMode = param;
            LContext->needValidate |= PSGL_VALIDATE_FFX_FRAGMENT_PROGRAM;
            break;
        }
        case GL_POINT_SPRITE_OES:
#ifndef JS_NO_ERROR_CHECK
            if ( pname != GL_COORD_REPLACE_OES )
            {
                _jsSetError( GL_INVALID_ENUM );
                return;
            }
            if ( LContext->ActiveTexture >= _JS_MAX_TEXTURE_COORD_INTERPOLANTS )
            {
                _jsSetError( GL_INVALID_OPERATION );
                return;
            }
#endif
            LContext->TexCoordReplaceMask &= ~( 1 << LContext->ActiveTexture );
            LContext->TexCoordReplaceMask |= (( param == 0 ) ? 0 : 1 ) << LContext->ActiveTexture;
            LContext->needValidate |= PSGL_VALIDATE_POINT_SPRITE;
            break;
        default:
            _jsSetError( GL_INVALID_ENUM );
            return;
    }
}

/**
 *  @short Set texture environment parameter
 *
 *  glTexEnvf() sets texture environment parameters.  It differs from glTexEnvfv() only in argument type.
 *  Please refer to glTexEnvfv() for more information.
 *
 *  @param target Specifies a texture environment.
 *  Must be either <CODE>GL_TEXTURE_ENV</CODE> or <CODE>GL_POINT_SPRITE_OES</CODE>.
 *  @param pname  Specifies the symbolic name of a single-valued texture environment
 *  parameter. Accepted values are <CODE>GL_TEXTURE_ENV_MODE</CODE>,
 *  or <CODE>GL_COORD_REPLACE_OES</CODE>.
 *  @param param  If <I><c>pname</c></I> is <CODE>GL_TEXTURE_ENV_MODE</CODE>, then
 *  <I><c>param</c></I> specifies a single symbolic constant, one of
 *  <CODE>GL_MODULATE</CODE>, <CODE>GL_DECAL</CODE>, <CODE>GL_BLEND</CODE>,
 *  <CODE>GL_REPLACE</CODE>, or <CODE>GL_ADD</CODE>.
 *  If <I><c>pname</c></I> is <CODE>GL_COORD_REPLACE_OES</CODE>, then
 *  <I><c>param</c></I> is either <CODE>GL_TRUE</CODE> or <CODE>GL_FALSE</CODE>.
 */

GLAPI void APIENTRY glTexEnvf( GLenum target, GLenum pname, GLfloat param )
{
    JS_APIENTRY_TIMER( glTexEnvf );
    PSGLcontext*	LContext = _CurrentContext;
    JS_ASSERT( LContext );
#ifndef JS_NO_ERROR_CHECK
    if ( !LContext->CurrentImageUnit )
    {
        _jsSetError( GL_INVALID_OPERATION );
        return;
    }
#endif
    if ( target == GL_TEXTURE_FILTER_CONTROL )
    {
        if ( _jsdpmState )
        {
            _JS_DPM_API_ENTRY_3( glTexEnvf, JSDPM_CALL_TYPE_STATE_TEX, , 3,
                                 target, sizeof( GLenum ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM,
                                 pname, sizeof( GLenum ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM,
                                 param, sizeof( GLfloat ), INPUT_BY_VALUE, DPM_API_ARGUMENT_FLOAT_VALUE );
        }

        if ( pname == GL_TEXTURE_LOD_BIAS )
        {
            LContext->CurrentImageUnit->lodBias = param;
            LContext->needValidate |= PSGL_VALIDATE_TEXTURES_USED;
        }
        else _jsSetError( GL_INVALID_ENUM );
    }
    else glTexEnvi( target, pname, ( GLint )param );
}

/**
 *  @short Set texture environment parameters
 *
 *  glTexEnviv() sets texture environment parameters.  It differs from glTexEnvfv() only in argument type.
 *  Please refer to glTexEnvfv() for more information.
 *
 *  @param target Specifies a texture environment.
 *  Must be either <CODE>GL_TEXTURE_ENV</CODE> or <CODE>GL_POINT_SPRITE_OES</CODE>.
 *  @param pname  Specifies the symbolic name of a single-valued texture environment
 *  parameter. Accepted values are <CODE>GL_TEXTURE_ENV_MODE</CODE>,
 *  or <CODE>GL_COORD_REPLACE_OES</CODE>.
 *  @param params  If <I><c>pname</c></I> is <CODE>GL_TEXTURE_ENV_MODE</CODE>, then
 *  <I><c>params</c></I> specifies a single symbolic constant, one of
 *  <CODE>GL_MODULATE</CODE>, <CODE>GL_DECAL</CODE>, <CODE>GL_BLEND</CODE>,
 *  <CODE>GL_REPLACE</CODE>, or <CODE>GL_ADD</CODE>.
 *  If <I><c>pname</c></I> is <CODE>GL_COORD_REPLACE_OES</CODE>, then
 *  <I><c>params</c></I> is either <CODE>GL_TRUE</CODE> or <CODE>GL_FALSE</CODE>.
 */

GLAPI void APIENTRY glTexEnviv( GLenum target, GLenum pname, const GLint *params )
{
    JS_APIENTRY_TIMER( glTexEnviv );
    if ( _jsdpmState )
    {
        _JS_DPM_API_ENTRY_3( glTexEnviv, JSDPM_CALL_TYPE_STATE_TEX, , 3,
                             target, sizeof( GLenum ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM,
                             pname, sizeof( GLenum ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM,
                             params, 4*sizeof( GLint ), INPUT_BY_REFERENCE, DPM_API_ARGUMENT_POINTER );
    }

    PSGLcontext*	LContext = _CurrentContext;
    JS_ASSERT( LContext );
#ifndef JS_NO_ERROR_CHECK
    if ( !LContext->CurrentImageUnit )
    {
        _jsSetError( GL_INVALID_OPERATION );
        return;
    }
#endif

    if ( target == GL_TEXTURE_ENV && pname == GL_TEXTURE_ENV_COLOR )
    {
        const GLfloat f = 1.f / ( INT_MAX );
        LContext->CurrentImageUnit->envColor.R = _jsClampf( f * ( GLfloat )params[0] );
        LContext->CurrentImageUnit->envColor.G = _jsClampf( f * ( GLfloat )params[1] );
        LContext->CurrentImageUnit->envColor.B = _jsClampf( f * ( GLfloat )params[2] );
        LContext->CurrentImageUnit->envColor.A = _jsClampf( f * ( GLfloat )params[3] );
        LContext->needValidate |= PSGL_VALIDATE_FFX_FRAGMENT_PROGRAM;
    }
    else glTexEnvi( target, pname, params[0] );
}

/**
 *  @short Set texture environment parameters
 *
A texture environment specifies how texture values are interpreted when a fragment is textured.
<I><c>target</c></I> must
be either <CODE>GL_TEXTURE_ENV</CODE> or <CODE>GL_POINT_SPRITE_OES</CODE>.
<I><c>pname</c></I> can be one of <CODE>GL_TEXTURE_ENV_MODE</CODE>,
<CODE>GL_TEXTURE_ENV_COLOR</CODE>, or
<CODE>GL_COORD_REPLACE_OES</CODE>.
 *
If <I><c>target</c></I> is <CODE>GL_TEXTURE_ENV</CODE> and
<I><c>pname</c></I> is <CODE>GL_TEXTURE_ENV_MODE</CODE>, then <I><c>params</c></I> is
the symbolic name of a texture function.
Five texture functions may be specified: <CODE>GL_MODULATE</CODE>, <CODE>GL_DECAL</CODE>, <CODE>GL_BLEND</CODE>, <CODE>GL_REPLACE</CODE>, or <CODE>GL_ADD</CODE>.
If <I><c>target</c></I> is <CODE>GL_TEXTURE_ENV</CODE> and
<I><c>pname</c></I> is <CODE>GL_TEXTURE_ENV_COLOR</CODE>, then <I><c>params</c></I>
is a pointer to an array that holds an RGBA color consisting of four values.
The values are clamped to the range [0, 1] when they are specified.
 *
A texture function acts on the fragment to be textured using the texture image value that applies to the fragment
(see glTexParameterf()) and produces an RGBA color for that fragment. The following table shows how the RGBA color
is produced for each of the three texture functions that can be chosen. C is a triple of color values (RGB) and A
is the associated alpha value. RGBA values extracted from a texture image are in the range [0, 1]. The subscript f
refers to the incoming fragment, the subscript t to the texture image, the subscript c to the texture environment color,
and subscript v indicates a value produced by the texture function.
(Cc is set by <CODE>GL_TEXTURE_ENV_COLOR</CODE>.)
 *
A texture image can have up to four components per texture element (see glTexImage2D() and glCopyTexImage2D()).
In a one-component image, Lt indicates that single component. A two-component image uses Lt and At.
A three-component image has only a color value, Ct. A four-component image has both a color value Ct and an alpha value At.
 *
 * <TABLE rules=all>
 * <TR>
 * <TD>Base internal format
 * </TD>
 * <TD><CODE>GL_MODULATE</CODE></TD>
 * <TD><CODE>GL_DECAL</CODE></TD>
 * <TD><CODE>GL_BLEND</CODE></TD>
 * <TD><CODE>GL_REPLACE</CODE></TD>
 * <TD><CODE>GL_ADD</CODE></TD>
 * </TR>
 * <TR>
 * <TD><CODE>GL_ALPHA</CODE></TD>
 * <TD>Cv = Cf
 *
 * Av = At &times; Af</TD>
 * <TD>undefined</TD>
 * <TD>Cv = Cf
 *
 * Av = At &times; Af</TD>
 * <TD>Cv = Cf
 *
 * Av = At</TD>
 * <TD>Cv = Cf
 *
 * Av = At &times; Af</TD>
 * </TR>
 * <TR>
 * <TD><CODE>GL_LUMINANCE</CODE></TD>
 * <TD>Cv = Lt &times; Cf
 *
 * Av = Af</TD>
 * <TD>undefined</TD>
 * <TD>Cv = (1 - Lt) &times; Cf + Lt &times; Cc
 *
 * Av = Af</TD>
 * <TD>Cv = Lt
 *
 * Av = Af</TD>
 * <TD>Cv = Lt + Cf
 *
 * Av = Af</TD>
 * </TR>
 * <TR>
 * <TD><CODE>GL_LUMINANCE_ALPHA</CODE></TD>
 * <TD>Cv = Lt &times; Cf
 *
 * Av = Af</TD>
 * <TD>undefined</TD>
 * <TD>Cv = (1 - Lt) &times; Cf + Lt &times; Cc
 *
 * Av = At &times; Af</TD>
 * <TD>Cv = Lt
 *
 * Av = At</TD>
 * <TD>Cv = Lt + Cf
 *
 * Av = At &times; Af</TD>
 * </TR>
 * <TR>
 * <TD><CODE>GL_RGB</CODE></TD>
 * <TD>Cv = Ct &times; Cf
 *
 * Av = Af</TD>
 * <TD>Cv = Ct
 *
 * Av = Af</TD>
 * <TD>Cv = (1 - Ct) &times; Cf + Ct &times; Cc
 *
 * Av = Af</TD>
 * <TD>Cv = Ct
 *
 * Av = Af</TD>
 * <TD>Cv = Cf + Ct
 *
 * Av = Af</TD>
 * </TR>
 * <TR>
 * <TD><CODE>GL_RGBA</CODE></TD>
 * <TD>Cv = Ct &times; Cf
 *
 * Av = At &times; Af</TD>
 * <TD>Cv = (1 - At) &times; Cf + At &times; Ct
 *
 * Av = Af</TD>
 * <TD>Cv = (1 - Ct) &times; Cf + Ct &times; Cc
 *
 * Av = At &times; Af</TD>
 * <TD>Cv = Ct
 *
 * Av = At</TD>
 * <TD>Cv = Cf + Ct
 *
 * Av = Af</TD>
 * </TR>
 * </TABLE>

If <I><c>target</c></I> is <CODE>GL_POINT_SPRITE_OES</CODE>, then
<I><c>pname</c></I> is <CODE>GL_COORD_REPLACE_OES</CODE>, and <I><c>params</c></I> is
either <CODE>GL_TRUE</CODE> or <CODE>GL_FALSE</CODE>.  If
<I><c>params</c></I> is <CODE>GL_TRUE</CODE> and the point sprite mode enabled,
the point size is typically increased.  The
points are rasterized with texture coordinates that vary across the
fragments used to render the point, from (0, 0) in the upper left to
(1,1) in the lower right of the point.

The initial values for <I><c>target</c></I>, <I><c>pname</c></I>, and <I><c>params</c></I> are
<CODE>GL_TEXTURE_ENV</CODE>, <CODE>GL_TEXTURE_ENV_MODE</CODE>,
and <CODE>GL_MODULATE</CODE>, respectively.
The initial value of <CODE>GL_TEXTURE_ENV_COLOR</CODE> is (0, 0, 0, 0).
The initial value of <CODE>GL_POINT_SPRITE_OES</CODE> is <CODE>GL_FALSE</CODE>.
 *
 *  @param target Specifies a texture environment.
 *  Must be either <CODE>GL_TEXTURE_ENV</CODE> or <CODE>GL_POINT_SPRITE_OES</CODE>.
 *  @param pname  Specifies the symbolic name of a single-valued texture environment
 *  parameter. Accepted values are <CODE>GL_TEXTURE_ENV_MODE</CODE>,
 *  <CODE>GL_TEXTURE_ENV_COLOR</CODE>, or <CODE>GL_COORD_REPLACE_OES</CODE>.
 *  @param params  If <I><c>pname</c></I> is <CODE>GL_TEXTURE_ENV_MODE</CODE>, then
 *  <I><c>params</c></I> specifies a single symbolic constant, one of
 *  <CODE>GL_MODULATE</CODE>, <CODE>GL_DECAL</CODE>, <CODE>GL_BLEND</CODE>,
 *  <CODE>GL_REPLACE</CODE>, or <CODE>GL_ADD</CODE>.
 *  If <I><c>pname</c></I> is <CODE>GL_TEXTURE_ENV_COLOR</CODE>, then
 *  <I><c>params</c></I> is a 4-valued color vector.
 *  If <I><c>pname</c></I> is <CODE>GL_COORD_REPLACE_OES</CODE>, then
 *  <I><c>params</c></I> is either <CODE>GL_TRUE</CODE> or <CODE>GL_FALSE</CODE>.

@par Errors
Use glGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>GL_INVALID_ENUM</CODE></TR>
<TD><I><c>target</c></I> or <I><c>pname</c></I> is not one of the accepted values,
or when <I><c>params</c></I> should have a defined constant value (based on the value of <I><c>pname</c></I>) and does not.</TD>
</TR>
</TABLE>

@note
If a fragment program is written, then the effect of a
texture environment function set with <CODE>GL_TEXTURE_ENV</CODE>
is replaced by fragment program code.  On the other hand,
a fragment program does <I>not</I> affect point sprite mode
(set by <CODE>GL_POINT_SPRITE_OES</CODE>).
@par
If the point sprite mode is enabled, point antialiasing (enabled by
<CODE>GL_POINT_SMOOTH</CODE>) is ignored.

@sa glActiveTexture, glCompressedTexImage2D, glCompressedTexImage3D, glCompressedTexSubImage2D, glCompressedTexSubImage3D,
    glCopyTexImage2D, glCopyTexSubImage2D, glCopyTexSubImage3D, glTexImage2D, glTexImage3D,
    glTexParameterf, glTexSubImage2D, glTexSubImage3D
 */

GLAPI void APIENTRY glTexEnvfv( GLenum target, GLenum pname, const GLfloat *params )
{
    JS_APIENTRY_TIMER( glTexEnvfv );
    if ( _jsdpmState )
    {
        _JS_DPM_API_ENTRY_3( glTexEnvfv, JSDPM_CALL_TYPE_STATE_TEX, , 3,
                             target, sizeof( GLenum ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM,
                             pname, sizeof( GLenum ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM,
                             params, 4*sizeof( GLfloat ), INPUT_BY_REFERENCE, DPM_API_ARGUMENT_POINTER );
    }

    PSGLcontext*	LContext = _CurrentContext;
    JS_ASSERT( LContext );
#ifndef JS_NO_ERROR_CHECK
    if ( !LContext->CurrentImageUnit )
    {
        _jsSetError( GL_INVALID_OPERATION );
        return;
    }
#endif

    if ( target == GL_TEXTURE_ENV && pname == GL_TEXTURE_ENV_COLOR )
    {

        LContext->CurrentImageUnit->envColor.R = _jsClampf( params[0] );
        LContext->CurrentImageUnit->envColor.G = _jsClampf( params[1] );
        LContext->CurrentImageUnit->envColor.B = _jsClampf( params[2] );
        LContext->CurrentImageUnit->envColor.A = _jsClampf( params[3] );
        LContext->needValidate |= PSGL_VALIDATE_FFX_FRAGMENT_PROGRAM;
    }
    else glTexEnvf( target, pname, params[0] );
}

/**
 *  @short Control the generation of texture coordinates
 *
 *  glTexGeni() controls the generation of texture coordinates.  It differs from glTexGenfv() only in
 *  argument type.  Please refer to glTexGenfv() for more information.

@param target  Specifies a texture coordinate.
Must be one of <CODE>GL_S</CODE>, <CODE>GL_T</CODE>, <CODE>GL_R</CODE>, or <CODE>GL_Q</CODE>.

@param pname  Specifies the symbolic name of the texture-coordinate generation function or function parameters.
Must be <CODE>GL_TEXTURE_GEN_MODE</CODE>.

@param param  Specifies a single-valued texture generation parameter,
 one of <CODE>GL_OBJECT_LINEAR</CODE>, <CODE>GL_EYE_LINEAR</CODE>, <CODE>GL_SPHERE_MAP</CODE>,
<CODE>GL_NORMAL_MAP</CODE>, or <CODE>GL_REFLECTION_MAP</CODE>.
 */

GLAPI void APIENTRY glTexGeni( GLenum target, GLenum pname, GLint param )
{
    JS_APIENTRY_TIMER( glTexGeni );
    if ( _jsdpmState )
    {
        _JS_DPM_API_ENTRY_3( glTexGeni, JSDPM_CALL_TYPE_STATE_TEX, , 3,
                             target, sizeof( GLenum ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM,
                             pname, sizeof( GLenum ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM,
                             param, sizeof( GLint ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE );
    }

    PSGLcontext*	LContext = _CurrentContext;
    JS_ASSERT( LContext );
#ifndef JS_NO_ERROR_CHECK
    if ( !LContext->CurrentCoordsUnit )
    {
        _jsSetError( GL_INVALID_OPERATION );
        return;
    }

    if ( pname != GL_TEXTURE_GEN_MODE )
    {
        _jsSetError( GL_INVALID_ENUM );
        return;
    }

    if ( !_jsIsTexGenMode( param, target ) )
    {
        _jsSetError( GL_INVALID_ENUM );
        return;
    }
#endif

    switch ( target )
    {
        case GL_S:
            LContext->CurrentCoordsUnit->texGen[0].mode = param;
            break;
        case GL_T:
            LContext->CurrentCoordsUnit->texGen[1].mode = param;
            break;
        case GL_R:
            LContext->CurrentCoordsUnit->texGen[2].mode = param;
            break;
        case GL_Q:
            LContext->CurrentCoordsUnit->texGen[3].mode = param;
            break;
        default:
            _jsSetError( GL_INVALID_ENUM );
            return;
    };
    LContext->CurrentCoordsUnit->revalidate |= _JS_TEXTURE_UNIT_REVALIDATE_TEX_GEN;
    LContext->needValidate |= PSGL_VALIDATE_FFX_VERTEX_PROGRAM;
}

/**
 *  @short Control the generation of texture coordinates
 *
 *  glTexGenf() controls the generation of texture coordinates.  It differs from glTexGenfv() only in
 *  argument type.  Please refer to glTexGenfv() for more information.

@param target  Specifies a texture coordinate.
Must be one of <CODE>GL_S</CODE>, <CODE>GL_T</CODE>, <CODE>GL_R</CODE>, or <CODE>GL_Q</CODE>.

@param pname  Specifies the symbolic name of the texture-coordinate generation function or function parameters.
Must be <CODE>GL_TEXTURE_GEN_MODE</CODE>.

@param param  Specifies a single-valued texture generation parameter,
one of <CODE>GL_OBJECT_LINEAR</CODE>, <CODE>GL_EYE_LINEAR</CODE>, <CODE>GL_SPHERE_MAP</CODE>,
<CODE>GL_NORMAL_MAP</CODE>, or <CODE>GL_REFLECTION_MAP</CODE>.
 */
GLAPI void APIENTRY glTexGenf( GLenum target, GLenum pname, GLfloat param )
{
    JS_APIENTRY_TIMER( glTexGenf );
    // dbgrt - don't trace this
    glTexGeni( target, pname, ( GLint )param );
}

/**
@short Control the generation of texture coordinates

@param target  Specifies a texture coordinate.
Must be one of <CODE>GL_S</CODE>, <CODE>GL_T</CODE>, <CODE>GL_R</CODE>, or <CODE>GL_Q</CODE>.

@param pname  Specifies the symbolic name of the texture-coordinate generation function or function parameters.
Must be <CODE>GL_TEXTURE_GEN_MODE</CODE>, <CODE>GL_OBJECT_PLANE</CODE>, or <CODE>GL_EYE_PLANE</CODE>.

@param param  Specifies a pointer to an array of texture generation parameters.
If <I><c>pname</c></I> is <CODE>GL_TEXTURE_GEN_MODE</CODE>, then the array must contain a single symbolic constant,
one of <CODE>GL_OBJECT_LINEAR</CODE>, <CODE>GL_EYE_LINEAR</CODE>, <CODE>GL_SPHERE_MAP</CODE>,
<CODE>GL_NORMAL_MAP</CODE>, or <CODE>GL_REFLECTION_MAP</CODE>.
Otherwise, <I><c>param</c></I> holds the coefficients for the texture-coordinate generation function specified by <I><c>pname</c></I>.

The function glTexGenfv() is one of four related functions, differing only in their parameter types.  All four operate as described below: glTexGenf(), glTexGenfv(), glTexGeni(), and glTexGeniv().

glTexGenfv() selects a texture-coordinate generation function or supplies coefficients for one of the functions. <I><c>target</c></I>
names one of the (<I>s, t, r, q</I>) texture coordinates; it must be one of the symbols <CODE>GL_S</CODE>,
<CODE>GL_T</CODE>, <CODE>GL_R</CODE>, or <CODE>GL_Q</CODE>. <I><c>pname</c></I> must be one of three symbolic constants:
<CODE>GL_TEXTURE_GEN_MODE</CODE>, <CODE>GL_OBJECT_PLANE</CODE>, or <CODE>GL_EYE_PLANE</CODE>.
If <I><c>pname</c></I> is <CODE>GL_TEXTURE_GEN_MODE</CODE>, then <I><c>param</c></I> chooses a mode, one of <CODE>GL_OBJECT_LINEAR</CODE>,
<CODE>GL_EYE_LINEAR</CODE>, <CODE>GL_SPHERE_MAP</CODE>, <CODE>GL_NORMAL_MAP</CODE>, or <CODE>GL_REFLECTION_MAP</CODE>.
If <I><c>pname</c></I> is either <CODE>GL_OBJECT_PLANE</CODE> or
<CODE>GL_EYE_PLANE</CODE>, <I><c>param</c></I> contains coefficients for the corresponding texture generation function.
If the texture generation function is <CODE>GL_OBJECT_LINEAR</CODE>, the function
@code
g = p1 &times; xo + p2 &times; yo + p3 &times; zo + p4 &times; wo
@endcode
is used, where g is the value computed for the coordinate named in <I><c>target</c></I>, p1, p2, p3, and p4 are the four values supplied in <I>param</I>, and xo, yo, zo, and wo are the object coordinates of the vertex. This function can be used, for example, to texture-map terrain using sea level as a reference plane (defined by p1, p2, p3, and p4). The altitude of a terrain vertex is computed by the <CODE>GL_OBJECT_LINEAR</CODE> coordinate generation function as its distance from sea level; that altitude can then be used to index the texture image to map white snow onto peaks and green grass onto foothills. If the texture generation function is <CODE>GL_EYE_LINEAR</CODE>, the function
@code
g = p1' &times; xe + p2' &times; ye + p3' &times; ze + p4' &times; we
@endcode
is used, where
@code
( p1' p2' p3' p4' ) = ( p1  p2  p3  p4 ) inv(M)
@endcode
and xe, ye, ze, and we are the eye coordinates of the vertex, p1, p2, p3, and p4 are the values supplied in <I><c>param</c></I>, and M is the modelview matrix when glTexGenfv() is invoked. If M is poorly conditioned or singular, texture coordinates generated by the resulting function may be inaccurate or undefined.

Note that the values in <I><c>param</c></I> define a reference plane in eye coordinates. The modelview matrix that is applied to them may not be the same one in effect when the polygon vertices are transformed. This function establishes a field of texture coordinates that can produce dynamic contour lines on moving objects.

If <I><c>pname</c></I> is <CODE>GL_SPHERE_MAP</CODE> and <I><c>target</c></I> is either <CODE>GL_S</CODE> or <CODE>GL_T</CODE>, s and t texture coordinates are generated as follows. Let u be the unit vector pointing from the origin to the polygon vertex (in eye coordinates). Let n' be the current normal, after transformation to eye coordinates. Let
@code
f = transpose ( fx  fy  fz )
@endcode
be the reflection vector such that
@code
f = u - 2 n' transpose(n') u
@endcode
Finally, let  m = 2 sqrt ( fx<sup>2</sup> + fy<sup>2</sup> + (fz + 1)<sup>2</sup> ). Then the values assigned to the s and t texture coordinates are
@code
s = fx / m + 1 / 2
t = fy / m + 1 / 2
@endcode
If <I><c>pname</c></I> is <CODE>GL_REFLECTION_MAP</CODE>, the reflection vector is calculated similarly to
<CODE>GL_SPHERE_MAP</CODE>:
@code
r = u - 2 n' transpose(n') u
@endcode
The resulting vector (<I>rx, ry, rz</I>) is used as the (<I>s, t, r</I>) texture coordinate, 
and texture-coordinate generation
should be enabled for <CODE>GL_S</CODE>, <CODE>GL_T</CODE>, and <CODE>GL_R</CODE>.

If <I><c>pname</c></I> is <CODE>GL_NORMAL_MAP</CODE>, the texture coordinates are determined by using the modelview matrix to
convert the current normal into eye coordinates. For <CODE>GL_NORMAL_MAP</CODE>, texture-coordinate generation
should be enabled for <CODE>GL_S</CODE>, <CODE>GL_T</CODE>, and <CODE>GL_R</CODE>.

To enable or disable a texture-coordinate generation function, call glEnable() or glDisable() with one of the symbolic
texture-coordinate names (<CODE>GL_TEXTURE_GEN_S</CODE>, <CODE>GL_TEXTURE_GEN_T</CODE>, <CODE>GL_TEXTURE_GEN_R</CODE>,
or <CODE>GL_TEXTURE_GEN_Q</CODE>) as the argument. When enabled, the specified texture coordinate is computed according
to the generating function associated with that coordinate. When disabled, subsequent vertices take the specified
texture coordinate from the current set of texture coordinates. Initially, all texture generation functions are set
to <CODE>GL_EYE_LINEAR</CODE> and are disabled. Both s plane equations are (1, 0, 0, 0), both t plane equations
are (0, 1, 0, 0), and all r and q plane equations are (0, 0, 0, 0).

glTexGenfv() sets the texture generation parameters for the currently active texture unit, selected with glActiveTexture().

@par Errors
Use glGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>GL_INVALID_ENUM</CODE></TD>
<TD><I><c>target</c></I> or <I><c>pname</c></I> is not an accepted defined value, or
when <I><c>pname</c></I> is <CODE>GL_TEXTURE_GEN_MODE</CODE> and <I><c>param</c></I> is not an accepted defined value. </TD>
</TR>
<TR>
<TD><CODE>GL_INVALID_ENUM</CODE></TD>
<TD><I><c>pname</c></I> is <CODE>GL_TEXTURE_GEN_MODE</CODE>,
<I><c>param</c></I> is <CODE>GL_SPHERE_MAP</CODE>, and <I><c>target</c></I> is either <CODE>GL_R</CODE> or <CODE>GL_Q</CODE>.</TD>
</TR>
</TABLE>

@sa glActiveTexture, glCopyTexImage2D, glCopyTexSubImage2D, glCopyTexSubImage3D, glTexEnvfv,
    glTexImage2D, glTexImage3D, glTexParameterf, glTexSubImage2D, glTexSubImage3D
 */

GLAPI void APIENTRY glTexGenfv( GLenum target, GLenum pname, const GLfloat * param )
{
    JS_APIENTRY_TIMER( glTexGenfv );
    if ( _jsdpmState )
    {
        _JS_DPM_API_ENTRY_3( glTexGenfv, JSDPM_CALL_TYPE_STATE_TEX, , 3,
                             target, sizeof( GLenum ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM,
                             pname, sizeof( GLenum ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM,
                             param, 4*sizeof( GLfloat ), INPUT_BY_REFERENCE, DPM_API_ARGUMENT_POINTER );
    }

    PSGLcontext*	LContext = _CurrentContext;
    JS_ASSERT( LContext );
#ifndef JS_NO_ERROR_CHECK
    if ( !LContext->CurrentCoordsUnit )
    {
        _jsSetError( GL_INVALID_OPERATION );
        return;
    }
#endif


    int coord = 0;
    switch ( target )
    {
        case GL_S:
            coord = 0;
            break;
        case GL_T:
            coord = 1;
            break;
        case GL_R:
            coord = 2;
            break;
        case GL_Q:
            coord = 3;
            break;
        default:
            _jsSetError( GL_INVALID_ENUM );
            return;
    };

    switch ( pname )
    {
        case GL_TEXTURE_GEN_MODE:
            glTexGenf( target, pname, param[0] );
            break;
        case GL_OBJECT_PLANE:
            LContext->CurrentCoordsUnit->texGen[coord].objectPlane[0] = param[0];
            LContext->CurrentCoordsUnit->texGen[coord].objectPlane[1] = param[1];
            LContext->CurrentCoordsUnit->texGen[coord].objectPlane[2] = param[2];
            LContext->CurrentCoordsUnit->texGen[coord].objectPlane[3] = param[3];
            break;
        case GL_EYE_PLANE:
            // multiply parameter vector by the inverse model view matrix
            _jsSetTexGenEyePlane( LContext->CurrentCoordsUnit->texGen + coord, param );
            break;
        default:
            _jsSetError( GL_INVALID_ENUM );
            return;
    };

    LContext->CurrentCoordsUnit->revalidate |= _JS_TEXTURE_UNIT_REVALIDATE_TEX_GEN;
    LContext->needValidate |= PSGL_VALIDATE_FFX_VERTEX_PROGRAM;
}

/**
 *  @short Control the generation of texture coordinates
 *
 *  glTexGeniv() controls the generation of texture coordinates.  It differs from glTexGenfv() only in
 *  argument type.  Please refer to glTexGenfv() for more information.

 *
@param target  Specifies a texture coordinate.
Must be one of <CODE>GL_S</CODE>, <CODE>GL_T</CODE>, <CODE>GL_R</CODE>, or <CODE>GL_Q</CODE>.

@param pname  Specifies the symbolic name of the texture-coordinate generation function or function parameters.
Must be <CODE>GL_TEXTURE_GEN_MODE</CODE>, <CODE>GL_OBJECT_PLANE</CODE>, or <CODE>GL_EYE_PLANE</CODE>.

@param param  Specifies a pointer to an array of texture generation parameters.
If <I><c>pname</c></I> is <CODE>GL_TEXTURE_GEN_MODE</CODE>, then the array must contain a single symbolic constant,
one of <CODE>GL_OBJECT_LINEAR</CODE>, <CODE>GL_EYE_LINEAR</CODE>, <CODE>GL_SPHERE_MAP</CODE>,
<CODE>GL_NORMAL_MAP</CODE>, or <CODE>GL_REFLECTION_MAP</CODE>.
Otherwise, <I><c>param</c></I> holds the coefficients for the texture-coordinate generation function specified by <I><c>pname</c></I>.
 */
GLAPI void APIENTRY glTexGeniv( GLenum target, GLenum pname, const GLint* param )
{
    JS_APIENTRY_TIMER( glTexGeniv );
    switch ( pname )
    {
        case GL_TEXTURE_GEN_MODE:
            glTexGeni( target, pname, param[0] );
            break;
        case GL_OBJECT_PLANE:
        case GL_EYE_PLANE:
        {
            GLfloat floatParams[4];
            floatParams[0] = ( GLfloat )param[0];
            floatParams[1] = ( GLfloat )param[1];
            floatParams[2] = ( GLfloat )param[2];
            floatParams[3] = ( GLfloat )param[3];
            glTexGenfv( target, pname, floatParams );
        }
        break;
        default:
            _jsSetError( GL_INVALID_ENUM );
            return;
    }
}
/** @} OpenGL */
