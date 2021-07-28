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
#include <PSGL/Debug.h>
#include <PSGL/Matrix.h>
#include <PSGL/Utils.h>
#include <PSGL/DPM2.h>

void _jsSetClipPlane( PSGLcontext* LContext, jsClipPlane *plane, const GLfloat *equation )
{
    JS_ASSERT( LContext );

    // multiply plane coefficients by the inverse model view matrix
    if ( !LContext->InverseModelViewValid )
    {
        jsMatrixInverse(
            LContext->ModelViewMatrixStack.MatrixStackf +
            LContext->ModelViewMatrixStack.MatrixStackPtr*jsELEMENTS_IN_MATRIX,
            LContext->InverseModelViewMatrixf );

        LContext->InverseModelViewValid = GL_TRUE;
    }
    plane->Coeff[0] =
        equation[0] * LContext->InverseModelViewMatrixf[M00] +
        equation[1] * LContext->InverseModelViewMatrixf[M10] +
        equation[2] * LContext->InverseModelViewMatrixf[M20] +
        equation[3] * LContext->InverseModelViewMatrixf[M30];
    plane->Coeff[1] =
        equation[0] * LContext->InverseModelViewMatrixf[M01] +
        equation[1] * LContext->InverseModelViewMatrixf[M11] +
        equation[2] * LContext->InverseModelViewMatrixf[M21] +
        equation[3] * LContext->InverseModelViewMatrixf[M31];
    plane->Coeff[2] =
        equation[0] * LContext->InverseModelViewMatrixf[M02] +
        equation[1] * LContext->InverseModelViewMatrixf[M12] +
        equation[2] * LContext->InverseModelViewMatrixf[M22] +
        equation[3] * LContext->InverseModelViewMatrixf[M32];
    plane->Coeff[3] =
        equation[0] * LContext->InverseModelViewMatrixf[M03] +
        equation[1] * LContext->InverseModelViewMatrixf[M13] +
        equation[2] * LContext->InverseModelViewMatrixf[M23] +
        equation[3] * LContext->InverseModelViewMatrixf[M33];
}

/**
 * @addtogroup OpenGL
 *
 * @{
 */

/**
 * @short Specify a plane against which all geometry is clipped
 *
Geometry is always clipped against the boundaries of a six-plane frustum in x, y, and z.
glClipPlanef() allows the specification of additional planes, that are not necessarily perpendicular
to the x, y, or z axis, against which all geometry is clipped. To determine the maximum number of
additional clipping planes, call glGetIntegerv() with argument <CODE>GL_MAX_CLIP_PLANES</CODE>.
All implementations support at least one such clipping plane. Because the resulting clipping
region is the intersection of the defined half-spaces, it is always convex.

glClipPlanef() specifies a half-space using a four-component plane equation.
When glClipPlanef() is called, <I><c>equation</c></I> is transformed by the inverse of the modelview matrix
and stored in the resulting eye coordinates. Subsequent changes to the modelview matrix
have no effect on the stored plane-equation components. If the dot product of the eye
coordinates of a vertex and the stored plane equation components is positive or zero,
the vertex is <I>in</I> with respect to that clipping plane. Otherwise, it is <I>out</I>.

To enable and disable clipping planes, call glEnable() and glDisable() with
the argument <CODE>GL_CLIP_PLANEi</CODE>, where <I>i</I> is the plane number.

All clipping planes are initially defined as (0, 0, 0, 0) in eye coordinates and are disabled.
 *
 * @param plane  Specifies which clipping plane is being positioned.
 * Symbolic names of the form <CODE>GL_CLIP_PLANEi</CODE> where 0 < i < <CODE>GL_MAX_CLIP_PLANES</CODE> are accepted.
 *
 * @param equation
 * Specifies the address of an array of four floating-point values. These values are interpreted as a plane equation.
 *
 * @note
 * It is always the case that <CODE>GL_CLIP_PLANEi</CODE> = <CODE>GL_CLIP_PLANE0</CODE> + <I>i</I>.
 *
 * @par Errors
Use glGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>GL_INVALID_ENUM</CODE></TD>
<TD><I><c>plane</c></I> is not an accepted value.</TD>
</TR>
</TABLE>
 *
 * @sa glEnable, glGetIntegerv
 */

GLAPI void APIENTRY glClipPlanef( GLenum plane, const GLfloat *equation )
{
    JS_APIENTRY_TIMER( glClipPlanef );
    if ( _jsdpmState )
    {
        _JS_DPM_API_ENTRY_2( glClipPlanef, JSDPM_CALL_TYPE_STATE_GEOM, , 2,
                             plane, sizeof( GLenum ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM,
                             equation, 4*sizeof( GLfloat ), INPUT_BY_REFERENCE, DPM_API_ARGUMENT_POINTER );
    }

    PSGLcontext*	LContext = _CurrentContext;
    JS_ASSERT( LContext );
#ifndef JS_NO_ERROR_CHECK
    switch ( plane )
    {
        case GL_CLIP_PLANE0:
        case GL_CLIP_PLANE1:
        case GL_CLIP_PLANE2:
        case GL_CLIP_PLANE3:
        case GL_CLIP_PLANE4:
        case GL_CLIP_PLANE5:
            break;
        default:
            _jsSetError( GL_INVALID_ENUM );
            return;
    }
#endif

    const int index = plane - GL_CLIP_PLANE0;
    _jsSetClipPlane( LContext, LContext->ClipPlane + index, equation );
    LContext->needValidate |= PSGL_VALIDATE_CLIP_PLANES;
}

/** @} Matrix */
