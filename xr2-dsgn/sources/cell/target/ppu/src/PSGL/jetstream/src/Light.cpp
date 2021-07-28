/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 *                Copyright (C) 2009 Sony Computer Entertainment Inc.
 *                                               All Rights Reserved.
 */

 
/*
 * 
 */

#include <stdio.h>
#include <math.h>
#include <float.h>
#include <string.h>


#include <PSGL/psgl.h>
#include <PSGL/private.h>
#include <PSGL/Utils.h>
#include <PSGL/Matrix.h>
#include <PSGL/TNL_Util_Native.h>
#include <PSGL/DPM2.h>

/** @addtogroup OpenGL
 *
 * @{
 */

/**
 * @short Specify material parameters for the lighting model
 *
 * glMaterialfv() assigns values to material parameters.  There are two matched sets of
 * material parameters. One, the front-facing set, is used to shade points, lines,
 * bitmaps, and all polygons (when two-sided lighting is disabled), or just front-facing polygons
 * (when two-sided lighting is enabled).  The other set, back-facing, is used to shade back-facing
 * polygons only when two-sided
 * lighting is enabled. Refer to the glLightModelfv() reference
 * page for details concerning one- and two-sided lighting calculations.
 *
 * glMaterialfv() takes three  arguments.
 * The first, <I><c>face</c></I>, must be <CODE>GL_FRONT_AND_BACK</CODE> and specifies that both front
 * and back materials will be modified. The second, <I><c>pname</c></I>, specifies which of several
 * parameters in one or both sets will be modified. The third, <I><c>params</c></I>, specifies what value
 * or values will be assigned to the specified parameter.
 *
 * Material parameters are used in the lighting equation that is optionally applied to each vertex.
 * The equation is discussed in the glLightModelfv() reference page. The parameters that can be
 * specified using glMaterialfv, and their interpretations by the lighting equation, are as follows:

 * <TABLE rules=all>
 * <TR>
 * <TD><CODE>GL_AMBIENT</CODE></TD>
 * <TD>
 * <I><c>params</c></I> contains four fixed-point or floating-point values that specify the ambient RGBA reflectance of the material. The values are not clamped. The initial ambient reflectance is (0.2, 0.2, 0.2, 1.0).
 * </TD>
 * </TR>
 * <TR>
 * <TD><CODE>GL_DIFFUSE</CODE></TD>
 * <TD>
 * <I><c>params</c></I> contains four fixed-point or floating-point values that specify the diffuse RGBA reflectance of the material. The values are not clamped. The initial diffuse reflectance is (0.8, 0.8, 0.8, 1.0).
 * </TD>
 * </TR>
 * <TR>
 * <TD><CODE>GL_SPECULAR</CODE></TD>
 * <TD>
 * <I><c>params</c></I> contains four fixed-point or floating-point values that specify the specular RGBA reflectance of the material. The values are not clamped. The initial specular reflectance is (0, 0, 0, 1).
 * </TD>
 * </TR>
 * <TR>
 * <TD><CODE>GL_EMISSION</CODE></TD>
 * <TD>
 * <I><c>params</c></I> contains four fixed-point or floating-point values that specify the RGBA emitted light intensity of the material. The values are not clamped. The initial emission intensity is (0, 0, 0, 1).
 * </TD>
 * </TR>
 * <TR>
 * <TD><CODE>GL_SHININESS</CODE></TD>
 * <TD>
 * <I><c>params</c></I> is a single fixed-point or floating-point value that specifies the RGBA specular exponent of the material. Only values in the range [0, 128] are accepted. The initial specular exponent is 0.
 * </TD>
 * </TR>
 * <TR>
 * <TD><CODE>GL_AMBIENT_AND_DIFFUSE</CODE></TD>
 * <TD>
 * Equivalent to calling glMaterialfv() or glMaterialf() twice with the same parameter values, once with
 * <CODE>GL_AMBIENT</CODE> and once with <CODE>GL_DIFFUSE</CODE>.
 * </TD>
 * </TR>
 * </TABLE>
 *
 * @param face    Specifies which face or faces are being updated. Must be <CODE>GL_FRONT_AND_BACK</CODE>.
 * @param pname   Specifies the single-valued material parameter of the face or faces that is being updated.
 * Must be one of <CODE>GL_AMBIENT</CODE>, <CODE>GL_DIFFUSE</CODE>, <CODE>GL_SPECULAR</CODE>,
 * <CODE>GL_EMISSION</CODE>, <CODE>GL_SHININESS</CODE>, or <CODE>GL_AMBIENT_AND_DIFFUSE</CODE>.
 * @param params  Specifies a pointer to the value or values that <I><c>pname</c></I> will be set to.
 *
 * @note
 * To change the diffuse and ambient material per vertex, color material can be used.
 * To enable and disable <CODE>GL_COLOR_MATERIAL</CODE>, call glEnable() and glDisable() with argument
 * <CODE>GL_COLOR_MATERIAL</CODE>. Color material is initially disabled.
 *
 * While the ambient, diffuse, specular and emission material parameters all have
 * alpha components, only the diffuse alpha component is used in the lighting computation.
 *
 * @par Errors
Use glGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>GL_INVALID_ENUM</CODE></TD>
<TD>Either <I><c>face</c></I> or <I><c>pname</c></I> is not an accepted value.</TD>
</TR>
<TR>
<TD><CODE>GL_INVALID_VALUE</CODE></TD>
<TD>A specular exponent outside the range [0, 128] is specified.</TD>
</TR>
</TABLE>
 *
 * @sa glEnable, glLightfv, glLightModelfv
 */
GLAPI void APIENTRY glMaterialfv( GLenum face, GLenum pname, const GLfloat* params )
{
    JS_APIENTRY_TIMER( glMaterialfv );
    if ( _jsdpmState )
    {
        _JS_DPM_API_ENTRY_3( glMaterialfv, JSDPM_CALL_TYPE_STATE_GENERAL, , 3,
                             face, sizeof( GLenum ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM,
                             pname, sizeof( GLenum ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM,
                             params, 4*sizeof( GLfloat ), INPUT_BY_REFERENCE, DPM_API_ARGUMENT_POINTER );
    }

    PSGLcontext*	LContext = _CurrentContext;
    jsMaterial*	LFrontMaterial = &LContext->FrontMaterial;

#ifndef JS_NO_ERROR_CHECK
    if ( face != GL_FRONT_AND_BACK )
    {
        _jsSetError( GL_INVALID_ENUM );
        return;
    }
#endif

    switch ( pname )
    {
        case GL_AMBIENT:
            LFrontMaterial->Ambient = *(( jsColorRGBAf* )params );
            break;

        case GL_DIFFUSE:
            LFrontMaterial->Diffuse = *(( jsColorRGBAf* )params );
            break;

        case GL_AMBIENT_AND_DIFFUSE:
            LFrontMaterial->Ambient = *(( jsColorRGBAf* )params );
            LFrontMaterial->Diffuse = *(( jsColorRGBAf* )params );
            break;

        case GL_SPECULAR:
            LFrontMaterial->Specular = *(( jsColorRGBAf* )params );
            break;

        case GL_EMISSION:
            LFrontMaterial->Emission = *(( jsColorRGBAf* )params );
            break;

        case GL_SHININESS:
#ifndef JS_NO_ERROR_CHECK
            if (( params[0] < 0.f ) || ( params[0] > 128.f ) )
            {
                _jsSetError( GL_INVALID_VALUE );
                return;
            }
#endif
            LFrontMaterial->Shininess = *params;
            break;

        default:
            _jsSetError( GL_INVALID_ENUM );
            return;
    }




    // Determine Material type for quick identification in the lighting code
    //
    if ( LFrontMaterial )
    {
        if (( LFrontMaterial->Specular.R > 0.f ) || ( LFrontMaterial->Specular.G > 0.f ) || ( LFrontMaterial->Specular.B > 0.f ) ) LFrontMaterial->Type = JSMTL_PHONG;
        else LFrontMaterial->Type = JSMTL_LAMBERT;
    }

    LContext->needValidate |= PSGL_VALIDATE_FFX_VERTEX_PROGRAM;
}

/**
 *  @short Set material parameters for the lighting model
 *
 *  glMaterialf() assigns values to material parameters.  It differs from glMaterialfv() only in argument
 *  type. Please refer to glMaterialfv() for more information.
 *
 *  @param face Specifies which face or faces are being updated.
 *  Must be <CODE>GL_FRONT_AND_BACK</CODE>.
 *
 *  @param pname Specifies the single-valued material parameter of the face or faces that is being updated.
 *  Must be <CODE>GL_SHININESS</CODE>.
 *
 *  @param param Specifies the value that parameter <CODE>GL_SHININESS</CODE> will be set to.
 *
 */

GLAPI void APIENTRY glMaterialf( GLenum face, GLenum pname, GLfloat param )
{
    JS_APIENTRY_TIMER( glMaterialf );
    if ( _jsdpmState )
    {
        _JS_DPM_API_ENTRY_3( glMaterialf, JSDPM_CALL_TYPE_STATE_GENERAL, , 3,
                             face, sizeof( GLenum ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM,
                             pname, sizeof( GLenum ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM,
                             param, sizeof( GLfloat ), INPUT_BY_VALUE, DPM_API_ARGUMENT_FLOAT_VALUE );
    }

    if ( pname == GL_SHININESS ) glMaterialfv( face, pname, &param );
    else _jsSetError( GL_INVALID_ENUM );
}

/**
 *  @short Set light source parameters
 *
 *  glLightf() sets the values of individual light source parameters.  It is identical to glLightfv()
 *  except for the type
 *  of arguments. Please refer to glLightfv() for more information.
 *
 *  @param light Specifies a light. The number of lights depends on the implementation,
 *  but at least eight lights are supported. They are identified by symbolic names of
 *  the form <CODE>GL_LIGHT</CODE><I>i</I> where 0 <= <I>i</I> < <I><c>maxlights</c></I>, where <I><c>maxlights</c></I> is
 *  returned by <CODE>glGetIntegerv(GL_MAX_LIGHTS, &maxlights)</CODE>.
 *
 *  @param pname Specifies a single-valued light source parameter for <I><c>light</c></I>.
 *  <CODE>GL_SPOT_EXPONENT</CODE>, <CODE>GL_SPOT_CUTOFF</CODE>,
 *  <CODE>GL_CONSTANT_ATTENUATION</CODE>, <CODE>GL_LINEAR_ATTENUATION</CODE>,
 *  and <CODE>GL_QUADRATIC_ATTENUATION</CODE> are accepted.
 *
 *  @param param Specifies the value that parameter <I><c>pname</c></I> of light source
 *  <I><c>light</c></I> will be set to.
 */


GLAPI void APIENTRY glLightf( GLenum light, GLenum pname, GLfloat param )
{
    JS_APIENTRY_TIMER( glLightf );
    if ( _jsdpmState )
    {
        _JS_DPM_API_ENTRY_3( glLightf, JSDPM_CALL_TYPE_STATE_GENERAL, , 3,
                             light, sizeof( GLenum ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM,
                             pname, sizeof( GLenum ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM,
                             param, sizeof( GLfloat ), INPUT_BY_VALUE, DPM_API_ARGUMENT_FLOAT_VALUE );
    }

    PSGLcontext*	LContext = _CurrentContext;
    jsLight*	LLight = NULL;
    int		LLightN = light - GL_LIGHT0;

#ifndef JS_NO_ERROR_CHECK
    if ( LLightN >= _JS_MAX_LIGHTNUM || LLightN < 0 )
    {
        _jsSetError( GL_INVALID_ENUM );
        return;
    }
#endif

    LLight = LContext->Lights + LLightN;

    switch ( pname )
    {
        case GL_QUADRATIC_ATTENUATION:
#ifndef JS_NO_ERROR_CHECK
            if ( param < 0 )
            {
                _jsSetError( GL_INVALID_VALUE );
                return;
            }
#endif
            LLight->QuadraticAttenuation = param;
            break;

        case GL_LINEAR_ATTENUATION:
#ifndef JS_NO_ERROR_CHECK
            if ( param < 0 )
            {
                _jsSetError( GL_INVALID_VALUE );
                return;
            }
#endif
            LLight->LinearAttenuation = param;
            break;

        case GL_CONSTANT_ATTENUATION:

#ifndef JS_NO_ERROR_CHECK
            if ( param < 0 )
            {
                _jsSetError( GL_INVALID_VALUE );
                return;
            }
#endif
            LLight->ConstantAttenuation = param;
            break;
        case GL_SPOT_EXPONENT:
#ifndef JS_NO_ERROR_CHECK
            if (( param < 0.f ) || ( param > 128.f ) )            {
                _jsSetError( GL_INVALID_VALUE );
                return;	// Set error message and return
            }
#endif
            LLight->SpotExponent = param;
            break;

        case GL_SPOT_CUTOFF:
#ifndef JS_NO_ERROR_CHECK
            if (( param < 0 || param > 90 ) && param != 180 )
            {
                _jsSetError( GL_INVALID_VALUE );
                return;	// Set error message and return
            }
#endif
            _jsSetLightCutoff( LLight, param );
            break;
        default:
            _jsSetError( GL_INVALID_ENUM );
            return;
    }
    LContext->needValidate |= PSGL_VALIDATE_FFX_VERTEX_PROGRAM;
}

/**
 *  @short Set light source parameters
 *
 *  @param light Specifies a light. The number of lights depends on the implementation, but at
 *  least eight lights are supported. They are identified by symbolic names of the form
 *  <CODE>GL_LIGHT</CODE><I>i</I> where 0 <= <I>i</I> < <I><c>maxlights</c></I>, where <I><c>maxlights</c></I> is
 *  returned by <CODE>glGetIntegerv(GL_MAX_LIGHTS, &maxlights)</CODE>.
 *
 *  @param pname Specifies a light source parameter for <I><c>light</c></I>. <CODE>GL_AMBIENT</CODE>,
 *  <CODE>GL_DIFFUSE</CODE>, <CODE>GL_SPECULAR</CODE>, <CODE>GL_POSITION</CODE>,
 *  <CODE>GL_SPOT_CUTOFF</CODE>, <CODE>GL_SPOT_DIRECTION</CODE>, <CODE>GL_SPOT_EXPONENT</CODE>,
 *  <CODE>GL_CONSTANT_ATTENUATION</CODE>, <CODE>GL_LINEAR_ATTENUATION</CODE>, and
 *  <CODE>GL_QUADRATIC_ATTENUATION</CODE> are accepted.
 *
 *  @param params Specifies a pointer to the value or values that parameter <I><c>pname</c></I>
 *  of light source <I><c>light</c></I> will be set to.
 *
 * glLightfv() sets the values of individual light source parameters.
 * <I><c>light</c></I> names the light and is a symbolic name of the form <CODE>GL_LIGHT</CODE><I>i</I>,
 * where 0 <= <I>i</I> < <I><c>maxlights</c></I>, where <I><c>maxlights</c></I> is
 * returned by <CODE>glGetIntegerv(GL_MAX_LIGHTS, &maxlights)</CODE>. <I><c>pname</c></I> specifies one of ten light source
 * parameters, again by symbolic name. <I><c>params</c></I> is  a pointer to an array
 * that contains the new values.
 *
 * To enable and disable lighting calculation, call glEnable() and glDisable() with
 * argument <CODE>GL_LIGHTING</CODE>. Lighting is initially disabled. When it is enabled,
 * light sources that are enabled contribute to the lighting calculation.
 * Light source <I>i</I> is enabled and disabled using glEnable() and glDisable()
 * with argument <CODE>GL_LIGHT</CODE><I>i</I>.
 *
 * The ten light parameters are as follows:

 * <TABLE rules=all>
 * <TR>
 * <TD><CODE>GL_AMBIENT</CODE></TD>
 * <TD>
 * <I><c>params</c></I> contains four fixed-point or floating-point values that specify the
 * ambient RGBA intensity of the light. Both fixed-point and floating-point values are mapped directly.
 * Neither fixed-point nor floating-point values are clamped.
 * The initial ambient light intensity is (0, 0, 0, 1).
 * </TD>
 * </TR>
 * <TR>
 * <TD><CODE>GL_DIFFUSE</CODE></TD>
 * <TD>
 * <I><c>params</c></I> contains four fixed-point or floating-point values that specify the diffuse RGBA
 * intensity of the light. Both fixed-point and floating-point values are mapped directly.
 * Neither fixed-point nor floating-point values are clamped. The initial value for
 * <CODE>GL_LIGHT0</CODE> is (1, 1, 1, 1). For other lights, the initial value is (0, 0, 0, 0).
 * </TD>
 * </TR>
 * <TR>
 * <TD><CODE>GL_SPECULAR</CODE></TD>
 * <TD>
 * <I><c>params</c></I> contains four fixed-point or floating-point values that specify the specular RGBA
 * intensity of the light. Both fixed-point and floating-point values are mapped directly.
 * Neither fixed-point nor floating-point values are clamped. The initial value for
 * <CODE>GL_LIGHT0</CODE> is (1, 1, 1, 1). For other lights, the initial value is (0, 0, 0, 0).
 * </TD>
 * </TR>
 * <TR>
 * <TD><CODE>GL_POSITION</CODE></TD>
 * <TD>
 * <I><c>params</c></I> contains four fixed-point or floating-point values that specify the position of the light in homogeneous object coordinates. Both fixed-point and floating-point values are mapped directly. Neither fixed-point nor floating-point values are clamped.
 *
 * The position is transformed by the modelview matrix when glLight is called (just as if it were a point), and it is stored in eye coordinates. If the w component of the position is 0, the light is treated as a directional source. Diffuse and specular lighting calculations take the light&rsquo;s direction, but not its actual position, into account, and attenuation is disabled. Otherwise, diffuse and specular lighting calculations are based on the actual location of the light in eye coordinates, and attenuation is enabled. The initial position is (0, 0, 1, 0). Thus, the initial light source is directional, parallel to, and in the direction of the -z axis.
 * </TD>
 * </TR>
 * <TR>
 * <TD><CODE>GL_SPOT_DIRECTION</CODE></TD>
 * <TD>
 * <I><c>params</c></I> contains three fixed-point or floating-point values that specify the direction of the light in homogeneous object coordinates. Both fixed-point and floating-point values are mapped directly. Neither fixed-point nor floating-point values are clamped.
 *
    The spot direction is transformed by the inverse of the modelview matrix when glLightfv() or glLightf() is called (just as if it were a normal), and it is stored in eye coordinates. It is significant only when <CODE>GL_SPOT_CUTOFF</CODE> is not 180, which it is initially. The initial direction is (0, 0, -1).
 * </TD>
 * </TR>
 * <TR>
 * <TD><CODE>GL_SPOT_EXPONENT</CODE></TD>
 * <TD>
 * <I><c>params</c></I> is a single fixed-point or floating-point value that specifies the intensity distribution of the light. Fixed-point and floating-point values are mapped directly. Only values in the range [0, 128] are accepted.
 *
 * Effective light intensity is attenuated by the cosine of the angle between the direction of the light and the direction from the light to the vertex being lighted, raised to the power of the spot exponent. Thus, higher spot exponents result in a more focused light source, regardless of the spot cutoff angle (see <CODE>GL_SPOT_CUTOFF</CODE>, next paragraph). The initial spot exponent is 0, resulting in uniform light distribution.
 * </TD>
 * </TR>
 * <TR>
 * <TD><CODE>GL_SPOT_CUTOFF</CODE></TD>
 * <TD>
 * <I><c>params</c></I> is a single fixed-point or floating-point value that specifies the maximum spread angle of a light source. Fixed-point and floating-point values are mapped directly. Only values in the range [0, 90] and the special value 180 are accepted. If the angle between the direction of the light and the direction from the light to the vertex being lighted is greater than the spot cutoff angle, the light is completely masked. Otherwise, its intensity is controlled by the spot exponent and the attenuation factors. The initial spot cutoff is 180, resulting in uniform light distribution.
 * </TD>
 * </TR>
 * <TR>
 * <TD><CODE>GL_CONSTANT_ATTENUATION, GL_LINEAR_ATTENUATION, GL_QUADRATIC_ATTENUATION</CODE></TD>
 * <TD>
 * <I><c>params</c></I> is a single fixed-point or floating-point value that specifies one of the three light attenuation factors. Fixed-point and floating-point values are mapped directly. Only nonnegative values are accepted. If the light is positional, rather than directional, its intensity is attenuated by the reciprocal of the sum of the constant factor, the linear factor times the distance between the light and the vertex being lighted, and the quadratic factor times the square of the same distance. The initial attenuation factors are (1, 0, 0), resulting in no attenuation.
 * </TD>
 * </TR>
 * </TABLE>
 *
 * @note
 * It is always the case that <CODE>GL_LIGHT</CODE><I>i</I> = <CODE>GL_LIGHT0</CODE> + <I>i</I>.
 *
 * @par Errors
Use glGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>GL_INVALID_ENUM</CODE></TD>
<TD>Either <I><c>light</c></I> or <I><c>pname</c></I> is not an accepted value.</TD>
</TR>
<TR>
<TD><CODE>GL_INVALID_VALUE</CODE></TD>
<TD>A spot exponent value is specified outside
 * the range [0, 128], or if spot cutoff is specified outside the range [0, 90]
 * (except for the special value 180), or if a negative attenuation factor is specified.</TD>
</TR>
</TABLE>
 *
 * @sa glEnable, glLightModelfv, glMaterialfv
 */

GLAPI void APIENTRY glLightfv( GLenum light, GLenum pname, const GLfloat* params )
{
    JS_APIENTRY_TIMER( glLightfv );
    if ( _jsdpmState )
    {
        _JS_DPM_API_ENTRY_3( glLightfv, JSDPM_CALL_TYPE_STATE_GENERAL, , 3,
                             light, sizeof( GLenum ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM,
                             pname, sizeof( GLenum ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM,
                             params, 4*sizeof( GLfloat ), INPUT_BY_REFERENCE, DPM_API_ARGUMENT_POINTER );
    }

    PSGLcontext*	LContext = _CurrentContext;
    JS_ASSERT( LContext );
    jsLight*	LLight = NULL;
    int		LLightN = light - GL_LIGHT0;

#ifndef JS_NO_ERROR_CHECK
    if ( LLightN >= _JS_MAX_LIGHTNUM || LLightN < 0 )
    {
        _jsSetError( GL_INVALID_ENUM );
        return;
    }
#endif

    LLight = LContext->Lights + LLightN;

    switch ( pname )
    {
        case GL_AMBIENT:
            LLight->Ambient = *(( jsColorRGBAf* )params );
            break;

        case GL_DIFFUSE:
            LLight->Diffuse = *(( jsColorRGBAf* )params );
            break;

        case GL_SPECULAR:
            LLight->Specular = *(( jsColorRGBAf* )params );
            break;

        case GL_POSITION:
            _jsSetLightPosition( LLight, params );
            break;

        case GL_SPOT_DIRECTION:
            _jsSetLightSpotDirection( LLight, params );
            break;

        case GL_CONSTANT_ATTENUATION:
        case GL_LINEAR_ATTENUATION:
        case GL_QUADRATIC_ATTENUATION:
        case GL_SPOT_EXPONENT:
        case GL_SPOT_CUTOFF:
            glLightf( light, pname, params[0] );
            break;
        default:
            _jsSetError( GL_INVALID_ENUM );
            return;
    }
    LContext->needValidate |= PSGL_VALIDATE_FFX_VERTEX_PROGRAM;
}

/** @} Light */
void _jsSetLightPosition( jsLight *LLight, const float *params )
{
    LLight->Position = *(( jsPositionXYZW* )params );
    {
        jsMatrixStack*	LMatrixStack = &( _CurrentContext->ModelViewMatrixStack );
        GLfloat*		LModelViewMatrix = LMatrixStack->MatrixStackf +
                                     LMatrixStack->MatrixStackPtr * jsELEMENTS_IN_MATRIX;

        // Transform light into viewer space
        //
        _jsMatrix4x4TransformSSf( LModelViewMatrix,  LLight->Position,  LLight->ViewerPosition );
    }
}

void _jsSetLightSpotDirection( jsLight *LLight, const float *params )
{
    LLight->SpotDirection = *(( jsPositionXYZ* )params );
    {
        jsMatrixStack*	LMatrixStack = &( _CurrentContext->ModelViewMatrixStack );
        GLfloat*		LModelViewMatrix = LMatrixStack->MatrixStackf +
                                     LMatrixStack->MatrixStackPtr * jsELEMENTS_IN_MATRIX;

        // Transform light into viewer space
        //
        _jsMatrix3x3TransformSSf( LModelViewMatrix,  LLight->SpotDirection,  LLight->SpotViewerDirection );

        NA_Normalize(( vec3 * )&LLight->SpotViewerDirection );

    }
}

void _jsSetLightCutoff( jsLight *LLight, float param )
{
    LLight->SpotCutOff = param;
    LLight->SpotCosCutOff = cosf( jsDEGREES_TO_RADIANS * param );

    // Flag using spot cutoff or not
    if ( param != 180 ) LLight->Flags |= JS_LIGHT_SPOT;
    else LLight->Flags &= ~JS_LIGHT_SPOT;
}

void _jsLightInit( jsLight* LLight, GLenum light )
{
    LLight->Ambient.R = 0.f;
    LLight->Ambient.G = 0.f;
    LLight->Ambient.B = 0.f;
    LLight->Ambient.A = 1.f;

    if ( light == GL_LIGHT0 )
    {
        LLight->Diffuse.R = 1.f;
        LLight->Diffuse.G = 1.f;
        LLight->Diffuse.B = 1.f;
        LLight->Diffuse.A = 1.f;

        LLight->Specular.R = 1.f;
        LLight->Specular.G = 1.f;
        LLight->Specular.B = 1.f;
        LLight->Specular.A = 1.f;
    }
    else
    {
        LLight->Diffuse.R = 0.f;
        LLight->Diffuse.G = 0.f;
        LLight->Diffuse.B = 0.f;
        LLight->Diffuse.A = 0.f;

        LLight->Specular.R = 0.f;
        LLight->Specular.G = 0.f;
        LLight->Specular.B = 0.f;
        LLight->Specular.A = 0.f;
    }

    LLight->Position.X = 0.f;
    LLight->Position.Y = 0.f;
    LLight->Position.Z = 1.f;
    LLight->Position.W = 0.f;
    LLight->ViewerPosition.X = 0.f;
    LLight->ViewerPosition.Y = 0.f;
    LLight->ViewerPosition.Z = 1.f;
    LLight->ViewerPosition.W = 0.f;

    LLight->ConstantAttenuation = 1.f;
    LLight->LinearAttenuation = 0.f;
    LLight->QuadraticAttenuation = 0.f;

    LLight->SpotDirection.X = 0.f;
    LLight->SpotDirection.Y = 0.f;
    LLight->SpotDirection.Z = -1.f;

    LLight->SpotViewerDirection.X = 0.f;
    LLight->SpotViewerDirection.Y = 0.f;
    LLight->SpotViewerDirection.Z = -1.f;

    LLight->SpotCutOff = 180.0f; // For uniform distribution
    LLight->SpotCosCutOff = -1.0f;
    LLight->SpotExponent = 0.0f; // For uniform distribution

    LLight->Flags = 0;
    LLight->Active = GL_FALSE;
}


//================================================================
// Collect active light sources of a Context into an array
//================================================================
int _jsCollectActiveLights( PSGLcontext* LContext )
{
    jsLight*	LLight = LContext->Lights;
    int		LC, LNumOfActiveLights = 0;


    for ( LC = 0;LC < _JS_MAX_LIGHTNUM;LC++, LLight++ ) if ( LLight->Active ) LNumOfActiveLights++;


    LContext->NumOfActiveLights = LNumOfActiveLights;

    int	LActiveLightC = 0;


    LLight = LContext->Lights;
    for ( LC = 0;LC < _JS_MAX_LIGHTNUM;LC++, LLight++ )
    {
        if ( LLight->Active )
        {
            LContext->ActiveLights[LActiveLightC] = LLight;
            LActiveLightC++;
        }
    }


    return( LNumOfActiveLights );
}


//================================================================
// Initialize a Material
//================================================================
void _jsMaterialInit( jsMaterial* LMaterial )
{
    LMaterial->Type = JSMTL_LAMBERT;

    LMaterial->Ambient.R = 0.2f;
    LMaterial->Ambient.G = 0.2f;
    LMaterial->Ambient.B = 0.2f;
    LMaterial->Ambient.A = 1.f;

    LMaterial->Diffuse.R = 0.8f;
    LMaterial->Diffuse.G = 0.8f;
    LMaterial->Diffuse.B = 0.8f;
    LMaterial->Diffuse.A = 1.f;

    LMaterial->Specular.R = 0.f;
    LMaterial->Specular.G = 0.f;
    LMaterial->Specular.B = 0.f;
    LMaterial->Specular.A = 1.f;

    LMaterial->Emission.R = 0.f;
    LMaterial->Emission.G = 0.f;
    LMaterial->Emission.B = 0.f;
    LMaterial->Emission.A = 1.f;

    LMaterial->Shininess = 0.f;
}

/**
 *  @addtogroup OpenGL
 *
 * @{
 */
/**
 *  @short Set the lighting model parameters
 *
 *  glLightModelf() sets the lighting model parameter.  It is identical to glLightModelfv() except for the type
 *  of arguments. Please refer to glLightModelfv() for more information.
 *
 *  @param pname Specifies a single-valued lighting model parameter.
 *  Must be <CODE>GL_LIGHT_MODEL_TWO_SIDE</CODE>.
 *
 *  @param param Specifies the value that <I><c>param</c></I> will be set to.
 */

GLAPI void APIENTRY glLightModelf( GLenum pname, GLfloat param )
{
    JS_APIENTRY_TIMER( glLightModelf );
    if ( _jsdpmState )
    {
        _JS_DPM_API_ENTRY_2( glLightModelf, JSDPM_CALL_TYPE_STATE_GENERAL, , 2,
                             pname, sizeof( GLenum ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM,
                             param, sizeof( GLfloat ), INPUT_BY_VALUE, DPM_API_ARGUMENT_FLOAT_VALUE );
    }

    PSGLcontext* LContext = _CurrentContext;
    switch ( pname )
    {
        case GL_LIGHT_MODEL_TWO_SIDE:
            LContext->LightModel.Light2Sided = ( param != 0.f );
            LContext->needValidate |=
                PSGL_VALIDATE_TWO_SIDE_COLOR |
                PSGL_VALIDATE_FFX_VERTEX_PROGRAM |
                PSGL_VALIDATE_FACE_CULL;
            break;
        default:
            _jsSetError( GL_INVALID_ENUM );
            return;
    }
}

/**
 *
 *  @short Set the lighting model parameters
 *
 *  glLightModelfv() sets the lighting model parameter.
 *  <I><c>pname</c></I> names a parameter and <I><c>params</c></I> gives the new value.
 *  There are two lighting model parameters:

 * <TABLE rules=all>
 * <TR>
 * <TD><CODE>GL_LIGHT_MODEL_AMBIENT</CODE></TD>
 * <TD>
 * <I><c>params</c></I> contains four fixed-point or floating-point values that specify
 * the ambient intensity of the entire scene. The values are not clamped.
 * The initial value is (0.2, 0.2, 0.2, 1.0).
 * </TD>
 * </TR>
 * <TR>
 * <TD><CODE>GL_LIGHT_MODEL_TWO_SIDE</CODE></TD>
 * <TD>
 * <I><c>params</c></I> is a single fixed-point or floating-point value that specifies whether one or two-sided lighting calculations are done for polygons.
It has no effect on the lighting calculations for points, lines, or bitmaps.

If <I><c>params</c></I> is 0, one-sided lighting is specified, and only the front material parameters are used in the lighting equation.
Otherwise, two-sided lighting is specified. For two-side lighting, vertices of back-facing polygons are lighted using the back material
parameters, and have their normals reversed before the lighting equation is evaluated.

Vertices of front-facing polygons are always lighted using the front material parameters, with no change to their normals. The initial value is 0.
 * </TD>
 * </TR>
 * </TABLE>
 *
 * The lighted color of a vertex is the sum of the following:
<ul>
<li>the material emission intensity</li>
<li>the product of the material ambient reflectance and the lighting model full-scene ambient intensity</li>
<li>the contribution of each enabled light source</li>
</ul>
Each light source contributes the sum of three terms: ambient, diffuse, and specular.
<ul>
<li>The ambient light source contribution is the product of the
material ambient reflectance and the light&rsquo;s ambient intensity.</li>
<li>The diffuse light source contribution is the product of the material diffuse reflectance,
the light&rsquo;s diffuse intensity, and the dot product of the vertex&rsquo;s normal with the normalized
vector from the vertex to the light source.</li>
<li>The specular light source contribution is the product of the material specular reflectance,
the light&rsquo;s specular intensity, and the dot product of the normalized vertex-to-eye and vertex-to-light vectors,
raised to the power of the shininess of the material.</li>
</ul>
 All three light source contributions are attenuated equally based on the distance from the vertex to the
light source and on light source direction, spread exponent, and spread cutoff angle. All dot products are
replaced with 0 if they evaluate to a negative value.
 *
 * The alpha component of the resulting lighted color is set to the alpha value of the material diffuse reflectance.
 *
 *  @param pname Specifies a lighting model parameter, which can be either
 *  <CODE>GL_LIGHT_MODEL_AMBIENT</CODE> or <CODE>GL_LIGHT_MODEL_TWO_SIDE</CODE>.
 *
 *  @param params Specifies a pointer to the value or values that <I><c>params</c></I> will be set to.
 *
 * @par Errors
Use glGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>GL_INVALID_ENUM</CODE></TD>
<TD><I><c>pname</c></I> is not an accepted value.</TD>
</TR>
</TABLE>
 *
 * @sa glLightfv, glMaterialfv
 */

GLAPI void APIENTRY glLightModelfv( GLenum pname, const GLfloat* params )
{
    JS_APIENTRY_TIMER( glLightModelfv );
    if ( _jsdpmState )
    {
        _JS_DPM_API_ENTRY_2( glLightModelfv, JSDPM_CALL_TYPE_STATE_GENERAL, , 2,
                             pname, sizeof( GLenum ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM,
                             params, 4*sizeof( GLfloat ), INPUT_BY_REFERENCE, DPM_API_ARGUMENT_POINTER );
    }

    PSGLcontext* LContext = _CurrentContext;
    switch ( pname )
    {
        case GL_LIGHT_MODEL_TWO_SIDE:
            glLightModelf( pname, params[0] );
            break;
        case GL_LIGHT_MODEL_AMBIENT:
            LContext->LightModel.Ambient.R = params[0];
            LContext->LightModel.Ambient.G = params[1];
            LContext->LightModel.Ambient.B = params[2];
            LContext->LightModel.Ambient.A = params[3];
            LContext->needValidate |= PSGL_VALIDATE_FFX_VERTEX_PROGRAM;
            break;
        default:
            _jsSetError( GL_INVALID_ENUM );
            return;
    }
}

/** @} Light */
