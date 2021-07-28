/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 *                Copyright (C) 2009 Sony Computer Entertainment Inc.
 *                                               All Rights Reserved.
 */


/*
 * PSGL matrix functions.
 * This file includes both public OpenGL ES matrix functions and several
 * PSGL internal utility functions.
 */


#undef __STRICT_ANSI__
#include <assert.h>
#include <stdio.h>
#include <math.h>
#include <float.h>
#include <string.h>


#include <PSGL/psgl.h>
#include <PSGL/Matrix.h>
#include <PSGL/private.h>
#include <PSGL/DPM2.h>

/////////////////////////////////////////////////////////////////////////////

const GLfloat _jsIdentityMatrixf[jsELEMENTS_IN_MATRIX] =
    {
        1.f, 0.f, 0.f, 0.f,
        0.f, 1.f, 0.f, 0.f,
        0.f, 0.f, 1.f, 0.f,
        0.f, 0.f, 0.f, 1.f
    };


/**
 * @internal
 *
 * Initialize a MatrixStack
 *
 * Initializes the given MatrixStack by:<br>
 * - Allocating the first block of Matrices<BR>
 * - Setting the StackPointer to 0<BR>
 * - Loading an identity Matrix on the current Matrix
 *
 */
void _jsMatrixStackReset( jsMatrixStack* LMatrixStack )
{
    LMatrixStack->MatrixStackPtr = 0;
    memcpy( LMatrixStack->MatrixStackf, _jsIdentityMatrixf, jsMATRIX_SIZEf );
    LMatrixStack->dirty = GL_TRUE;
}

// allocate and initialize a matrix stack
void _jsMatrixStackInit( jsMatrixStack* LMatrixStack, GLuint depth )
{
    JS_ASSERT( depth > 0 );
    LMatrixStack->MatrixStackf = ( GLfloat * )jsMalloc( jsMATRIX_SIZEf * depth );
    if ( !LMatrixStack->MatrixStackf ) return;

    LMatrixStack->NumOfMatricesAllocated = depth;
    _jsMatrixStackReset( LMatrixStack );
}

// destroy/free a matrix stack
void _jsMatrixStackClear( jsMatrixStack* LMatrixStack )
{
    if ( LMatrixStack->MatrixStackf ) jsFree( LMatrixStack->MatrixStackf );
    LMatrixStack->MatrixStackf = NULL;
    LMatrixStack->NumOfMatricesAllocated = 0;
    LMatrixStack->MatrixStackPtr = 0;
    LMatrixStack->dirty = GL_FALSE;
}

/**
 * Print out a the contents of a Matrix
 *
 * Prints the contents of the given matrix to the standard output.
 *
 * @param LMatrix      The Matrix to dump out
 */
void jsPrintMatrixf( jsMatrixf LMatrix )
{
    printf( "%f, %f, %f, %f\n", LMatrix[M00], LMatrix[M01], LMatrix[M02], LMatrix[M03] );
    printf( "%f, %f, %f, %f\n", LMatrix[M10], LMatrix[M11], LMatrix[M12], LMatrix[M13] );
    printf( "%f, %f, %f, %f\n", LMatrix[M20], LMatrix[M21], LMatrix[M22], LMatrix[M23] );
    printf( "%f, %f, %f, %f\n", LMatrix[M30], LMatrix[M31], LMatrix[M32], LMatrix[M33] );
}

/////////////////////////////////////////////////////////////////////////////

/**
 * @addtogroup OpenGL
 *
 * @{
 */

/**
 * @short Replace the current matrix with the identity matrix
 *
 * glLoadIdentity() replaces the current matrix with the identity matrix.
 * It is semantically equivalent to calling glLoadMatrixf() with the identity matrix:

@code
     1 0 0 0
     0 1 0 0
     0 0 1 0
     0 0 0 1
@endcode

 * but in some cases it is more efficient.
 *
 * @sa glLoadMatrixf, glMatrixMode, glMultMatrixf, glPushMatrix
 */
GLAPI void APIENTRY glLoadIdentity()
{
    JS_APIENTRY_TIMER( glLoadIdentity );
#ifdef JS_DPM
    if ( _jsdpmState ) { _JS_DPM_API_ENTRY_0( glLoadIdentity, JSDPM_CALL_TYPE_STATE_GENERAL, ); }
#endif
    PSGLcontext* LContext = _CurrentContext;
    JS_ASSERT( LContext );
    jsMatrixStack* LMatrixStack = NULL;

    jsContextCurrentMatrixStack( LContext, LMatrixStack );
#ifndef JS_NO_ERROR_CHECK
    if ( !LMatrixStack )
    {
        _jsSetError( GL_INVALID_OPERATION );
        return;
    }
#endif
    memcpy( LMatrixStack->MatrixStackf + LMatrixStack->MatrixStackPtr*jsELEMENTS_IN_MATRIX, _jsIdentityMatrixf, jsMATRIX_SIZEf );

    LContext->needValidate |= PSGL_VALIDATE_FFX_VERTEX_PROGRAM;
    LMatrixStack->dirty = GL_TRUE;
}


/**
 * @short Push the current matrix stack
 *
 * There is a stack of matrices for each of the matrix modes. In <CODE>GL_MODELVIEW</CODE> mode,
 * the stack depth is at least 16. In the other modes, <CODE>GL_PROJECTION</CODE>,
 * and <CODE>GL_TEXTURE</CODE>, the depth is at least 2. The current matrix in any
 * mode is the matrix on the top of the stack for that mode.
 *
 * You can query the current values of the stack depth by calling
 * glGetIntegerv() with argument
 * <CODE>GL_MAX_MODELVIEW_STACK_DEPTH</CODE>,
 * <CODE>GL_MAX_PROJECTION_STACK_DEPTH</CODE>, or
 * <CODE>GL_MAX_TEXTURE_STACK_DEPTH</CODE>.
 *
 * glPushMatrix() pushes the current matrix stack down by one, duplicating the current matrix.
 * That is, after a glPushMatrix() call, the matrix on top of the stack is identical to the one below it.
 *
 * glPopMatrix() pops the current matrix stack, replacing the current matrix with the one below it on the stack.
 *
 * Initially, each of the stacks contains one matrix, an identity matrix.
 *
 * It is an error to push a full matrix stack, or to pop a matrix stack that contains only a single matrix.
 * In either case, the error flag is set and no other change is made to the GL state.
 *
 * @note
 * Each texture unit has its own texture matrix stack.
 * Use glActiveTexture() to select the desired texture matrix stack. You
 * can examine the number of texture units by calling glGetIntegerv()
 * with argument <CODE>GL_MAX_TEXTURE_UNITS</CODE>.
 *
 * @par Errors
Use glGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>GL_STACK_OVERFLOW</CODE></TD>
<TD>glPushMatrix() is called while the current matrix stack is full.</TD>
</TR>
<TR>
<TD><CODE>GL_STACK_UNDERFLOW</CODE></TD>
<TD>glPopMatrix() is called while the current matrix stack contains only a single matrix.</TD>
</TR>
</TABLE>

 *
 * @sa glActiveTexture, glFrustumf, glGetIntegerv, glLoadIdentity, glLoadMatrixf, glMatrixMode, glMultMatrixf, glOrthof,
 *     glRotatef, glScalef, glTranslatef, glViewport
 */
GLAPI void APIENTRY glPushMatrix()
{
    JS_APIENTRY_TIMER( glPushMatrix );
#ifdef JS_DPM
    if ( _jsdpmState ) { _JS_DPM_API_ENTRY_0( glPushMatrix, JSDPM_CALL_TYPE_STATE_GENERAL, ); }
#endif
    PSGLcontext* LContext = _CurrentContext;
    JS_ASSERT( LContext );
    jsMatrixStack* LMatrixStack = NULL;
    GLfloat* LMatrix = NULL;

    jsContextCurrentMatrixStack( LContext, LMatrixStack );
#ifndef JS_NO_ERROR_CHECK
    if ( !LMatrixStack )
    {
        _jsSetError( GL_INVALID_OPERATION );
        return;
    }

    if ( LMatrixStack->MatrixStackPtr >= LMatrixStack->NumOfMatricesAllocated - 1 )
    {
        _jsSetError( GL_STACK_OVERFLOW );
        return;
    }
#endif

    LMatrix = LMatrixStack->MatrixStackf + LMatrixStack->MatrixStackPtr * jsELEMENTS_IN_MATRIX;

    memcpy( LMatrix + jsELEMENTS_IN_MATRIX, LMatrix, jsMATRIX_SIZEf );
    LMatrixStack->MatrixStackPtr += 1;

#if __JS_DEBUG_MATRIX_STACK == 1
    _jsLog( 0, "%s() MatrixStackPtr: %d", __func__, LMatrixStack->MatrixStackPtr );
#endif

    LContext->needValidate |= PSGL_VALIDATE_FFX_VERTEX_PROGRAM;
    LMatrixStack->dirty = GL_TRUE;
}

/**
 *  @short Pop the current matrix stack
 *
 *  glPopMatrix() pops the current matrix stack, replacing the current matrix with the one below it on the stack.
 *
 *  For more information, see the related function glPushMatrix()
 *
 *  @par Errors
Use glGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>GL_STACK_UNDERFLOW</CODE></TD>
<TD>glPopMatrix() is called while the current matrix stack contains only a single matrix.</TD>
</TR>
</TABLE>
 *
 */

GLAPI void APIENTRY glPopMatrix()
{
    JS_APIENTRY_TIMER( glPopMatrix );
#ifdef JS_DPM
    if ( _jsdpmState ) { _JS_DPM_API_ENTRY_0( glPopMatrix, JSDPM_CALL_TYPE_STATE_GENERAL, ); }
#endif
    PSGLcontext* LContext = _CurrentContext;
    JS_ASSERT( LContext );
    jsMatrixStack* LMatrixStack = NULL;

    jsContextCurrentMatrixStack( LContext, LMatrixStack );
#ifndef JS_NO_ERROR_CHECK
    if ( !LMatrixStack )
    {
        _jsSetError( GL_INVALID_OPERATION );
        return;
    }
    if ( LMatrixStack->MatrixStackPtr == 0 )
    {
        _jsSetError( GL_STACK_UNDERFLOW );
        return;
    }
#endif

    LMatrixStack->MatrixStackPtr -= 1;

#if __JS_DEBUG_MATRIX_STACK == 1
    _jsLog( 0, "%s() MatrixStackPtr: %d", __func__, LMatrixStack->MatrixStackPtr );
#endif

    LContext->needValidate |= PSGL_VALIDATE_FFX_VERTEX_PROGRAM;
    LMatrixStack->dirty = GL_TRUE;
    if ( LContext->MatrixMode == GL_MODELVIEW )
        LContext->InverseModelViewValid = GL_FALSE;
}

/** @} Matrix */

/////////////////////////////////////////////////////////////////////////////

/**
 *
 * Pre-multiply LMatrix1 with LMatrix2
 *
 * _jsMatrixMult3f Pre-multiplies LMatrix1 with LMatrix2 and stores the result in LDestMtx.
 *
 * @param LMatrix1       Pointer to source Matrix-1
 * @param LMatrix2       Pointer to source Matrix-2
 * @param LMatrix       Pointer to the destination Matrix
 *
 * @sa E3d_MatrixLoadIdentity, E3d_MatrixScale, E3d_MatrixRotate, E3d_MatrixRotateSinCos, E3d_MatrixTranslate
 */
void _jsMatrixMult4f( const GLfloat *LMatrix1, const GLfloat *LMatrix2, GLfloat *LMatrix )
{
    GLfloat L00, L01, L02, L03, L10, L11, L12, L13, L20, L21, L22, L23, L30, L31, L32, L33;
    GLfloat m00, m01, m02, m03, m10, m11, m12, m13, m20, m21, m22, m23, m30, m31, m32, m33;


    L00 = LMatrix1[M00];L01 = LMatrix1[M01];L02 = LMatrix1[M02];L03 = LMatrix1[M03];
    L10 = LMatrix1[M10];L11 = LMatrix1[M11];L12 = LMatrix1[M12];L13 = LMatrix1[M13];
    L20 = LMatrix1[M20];L21 = LMatrix1[M21];L22 = LMatrix1[M22];L23 = LMatrix1[M23];
    L30 = LMatrix1[M30];L31 = LMatrix1[M31];L32 = LMatrix1[M32];L33 = LMatrix1[M33];

    m00 = LMatrix2[M00];m01 = LMatrix2[M01];m02 = LMatrix2[M02];m03 = LMatrix2[M03];
    m10 = LMatrix2[M10];m11 = LMatrix2[M11];m12 = LMatrix2[M12];m13 = LMatrix2[M13];
    m20 = LMatrix2[M20];m21 = LMatrix2[M21];m22 = LMatrix2[M22];m23 = LMatrix2[M23];
    m30 = LMatrix2[M30];m31 = LMatrix2[M31];m32 = LMatrix2[M32];m33 = LMatrix2[M33];

    LMatrix[M00] = L00 * m00 + L01 * m10 + L02 * m20 + L03 * m30;
    LMatrix[M01] = L00 * m01 + L01 * m11 + L02 * m21 + L03 * m31;
    LMatrix[M02] = L00 * m02 + L01 * m12 + L02 * m22 + L03 * m32;
    LMatrix[M03] = L00 * m03 + L01 * m13 + L02 * m23 + L03 * m33;

    LMatrix[M10] = L10 * m00 + L11 * m10 + L12 * m20 + L13 * m30;
    LMatrix[M11] = L10 * m01 + L11 * m11 + L12 * m21 + L13 * m31;
    LMatrix[M12] = L10 * m02 + L11 * m12 + L12 * m22 + L13 * m32;
    LMatrix[M13] = L10 * m03 + L11 * m13 + L12 * m23 + L13 * m33;

    LMatrix[M20] = L20 * m00 + L21 * m10 + L22 * m20 + L23 * m30;
    LMatrix[M21] = L20 * m01 + L21 * m11 + L22 * m21 + L23 * m31;
    LMatrix[M22] = L20 * m02 + L21 * m12 + L22 * m22 + L23 * m32;
    LMatrix[M23] = L20 * m03 + L21 * m13 + L22 * m23 + L23 * m33;

    LMatrix[M30] = L30 * m00 + L31 * m10 + L32 * m20 + L33 * m30;
    LMatrix[M31] = L30 * m01 + L31 * m11 + L32 * m21 + L33 * m31;
    LMatrix[M32] = L30 * m02 + L31 * m12 + L32 * m22 + L33 * m32;
    LMatrix[M33] = L30 * m03 + L31 * m13 + L32 * m23 + L33 * m33;
}

/////////////////////////////////////////////////////////////////////////////

/**
 * @addtogroup OpenGL
 *
 * @{
 */

/**
 * @short Multiply the current matrix with the specified matrix
 *
 * glMultMatrixf() multiplies the current matrix with the one specified using <I><c>m</c></I>,
 * and replaces the current matrix with the product.
 *
 * The current matrix is determined by the current matrix mode (see glMatrixMode()).
 * It is either the projection matrix, modelview matrix, or the texture matrix.
 *
 * If the current matrix is C, and the coordinates to be transformed are,
 * v = (v[0], v[1], v[2], v[3]), then the current transformation is C X v, or

@code
| c [0]  c [4]  c [8]  c [12] |             v [0]
| c [1]  c [5]  c [9]  c [13] |      X      v [1]
| c [2]  c [6]  c [10] c [14] |             v [2]
| c [3]  c [7]  c [11] c [15] |             v [3]
@endcode

 * Calling glMultMatrixf() with an argument of <I><c>m</c></I> = m[0] , m[1] , ... m[15]
 * replaces the current transformation with (C X M) X v, or

@code
| c [0]  c [4]  c [8]  c [12] |    | m [0]  m [4]  m [8]  m [12] |    v [0]
| c [1]  c [5]  c [9]  c [13] | X | m [1]  m [5]  m [9]  m [13] | X  v [1]
| c [2]  c [6]  c [10] c [14] |   | m [2]  m [6]  m [10] m [14] |    v [2]
| c [3]  c [7]  c [11] c [15] |   | m [3]  m [7]  m [11] m [15] |    v [3]
@endcode

Where " X " denotes matrix multiplication, and v is represented as a 4 X 1 matrix.
 *
 * @param  m  Points to 16 consecutive values that are used as the elements of a 4 X 4 column-major matrix.
 *
 * @note
 * While the elements of the matrix may be specified with single or double precision,
 * The GL may store or operate on these values in less than single precision.
 * @par
 * In many computer languages 4 X 4 arrays are represented in row-major order. The transformations just described represent these matrices in column-major order. The order of the multiplication is important. For example, if the current transformation is a rotation, and glMultMatrixf() is called with a translation matrix, the translation is done directly on the coordinates to be transformed, while the rotation is done on the results of that translation.
 *
 * @sa glLoadIdentity, glLoadMatrixf, glMatrixMode, glPushMatrix
 */
GLAPI void APIENTRY glMultMatrixf( const GLfloat* m )
{
    JS_APIENTRY_TIMER( glMultMatrixf );
#ifdef JS_DPM
    if ( _jsdpmState ) { _JS_DPM_API_ENTRY_1( glMultMatrixf, JSDPM_CALL_TYPE_STATE_GENERAL, , 1, m, 16*sizeof( GLfloat ), INPUT_BY_REFERENCE, DPM_API_ARGUMENT_POINTER ); }
#endif
    PSGLcontext* LContext = _CurrentContext;
    JS_ASSERT( LContext );
    jsMatrixStack* LMatrixStack = NULL;
    jsMatrixf LMatrix = NULL;


    jsContextCurrentMatrixf( LContext, LMatrixStack, LMatrix );
#ifndef JS_NO_ERROR_CHECK
    if ( !LMatrixStack )
    {
        _jsSetError( GL_INVALID_OPERATION );
        return;
    }
#endif

#if __JS_DEBUG_MATRIX_STACK == 1
    _jsLog( 0, "%s() MatrixStackPtr: %d", __func__, LMatrixStack->MatrixStackPtr );
#endif
    _jsMatrixMult4f( LMatrix, m, LMatrix );

    LContext->needValidate |= PSGL_VALIDATE_FFX_VERTEX_PROGRAM;
    LMatrixStack->dirty = GL_TRUE;
    if ( LContext->MatrixMode == GL_MODELVIEW )
        LContext->InverseModelViewValid = GL_FALSE;
}


/**
 * @short Multiply the current matrix by a translation matrix
 *
 * glTranslatef() produces a translation by (<I><c>LX, LY, LZ</c></I>). The current matrix
 * (see glMatrixMode()) is multiplied by this translation matrix, with the product replacing
 * the current matrix,
 * as if glMultMatrixf() were called with the following matrix for its argument:

 * @code
 *      | 1 0 0 LX |
 *      | 0 1 0 LY |
 *      | 0 0 1 LZ |
 *      | 0 0 0 1  |
 * @endcode
 *
 * If the matrix mode is either <CODE>GL_MODELVIEW</CODE> or <CODE>GL_PROJECTION</CODE>,
 * all objects drawn after a call to glTranslatef() are translated.
 *
 * Use glPushMatrix() and glPopMatrix() to save and restore the untranslated coordinate system.
 *
 * @param LX  Specify the <I>LX</I> coordinate of a translation vector
 * @param LY  Specify the <I>LY</I> coordinate of a translation vector
 * @param LZ  Specify the <I>LZ</I> coordinate of a translation vector

 *
 * @sa glMatrixMode, glMultMatrixf, glPushMatrix, glRotatef, glScalef, glTranslatex
 */
GLAPI void APIENTRY glTranslatef( GLfloat LX, GLfloat LY, GLfloat LZ )
{
    JS_APIENTRY_TIMER( glTranslatef );
#ifdef JS_DPM
    if ( _jsdpmState )
    {
        _JS_DPM_API_ENTRY_3( glTranslatef, JSDPM_CALL_TYPE_STATE_GENERAL, , 3,
                             LX, sizeof( GLfloat ), INPUT_BY_VALUE, DPM_API_ARGUMENT_FLOAT_VALUE,
                             LY, sizeof( GLfloat ), INPUT_BY_VALUE, DPM_API_ARGUMENT_FLOAT_VALUE,
                             LZ, sizeof( GLfloat ), INPUT_BY_VALUE, DPM_API_ARGUMENT_FLOAT_VALUE );
    }
#endif
    PSGLcontext* LContext = _CurrentContext;
    JS_ASSERT( LContext );
    jsMatrixStack* LMatrixStack = NULL;
    jsMatrixf LMatrix = NULL;


    jsContextCurrentMatrixf( LContext, LMatrixStack, LMatrix );
#ifndef JS_NO_ERROR_CHECK
    if ( !LMatrixStack )
    {
        _jsSetError( GL_INVALID_OPERATION );
        return;
    }
#endif

    LMatrix[M03] += LMatrix[M00] * LX + LMatrix[M01] * LY + LMatrix[M02] * LZ;
    LMatrix[M13] += LMatrix[M10] * LX + LMatrix[M11] * LY + LMatrix[M12] * LZ;
    LMatrix[M23] += LMatrix[M20] * LX + LMatrix[M21] * LY + LMatrix[M22] * LZ;
    LMatrix[M33] += LMatrix[M30] * LX + LMatrix[M31] * LY + LMatrix[M32] * LZ;

    LContext->needValidate |= PSGL_VALIDATE_FFX_VERTEX_PROGRAM;
    LMatrixStack->dirty = GL_TRUE;
    if ( LContext->MatrixMode == GL_MODELVIEW )
        LContext->InverseModelViewValid = GL_FALSE;
}


/**
 * @short Multiply the current matrix by a general scaling matrix
 *
 * glScalef() produces a nonuniform scaling along the <I><c>LXScale</c></I>, <I><c>LYScale</c></I>, and <I><c>LZScale</c></I> axes.
 * The three parameters indicate the desired scale factor along each of the three axes.
 *
 * The current matrix (see glMatrixMode()) is multiplied by this scale matrix, and the product
 * replaces the current matrix as if glScalef() were called with the following matrix as its argument:

 * @code
 *      | LXScale     0        0     0 |
 *      |    0     LYScale     0     0 |
 *      |    0        0     LZScale  0 |
 *      |    0        0        0     1 |
 * @endcode
 *
 * If the matrix mode is either <CODE>GL_MODELVIEW</CODE> or <CODE>GL_PROJECTION</CODE>,
 * all objects drawn after glScalef() is called are scaled.
 *
 * Use glPushMatrix() and glPopMatrix() to save and restore the unscaled coordinate system.
 *
 * @param LXScale  Specifies the scale factor along the <I>LXScale</I> axis.
 * @param LYScale  Specifies the scale factor along the <I>LYScale</I> axis.
 * @param LZScale  Specifies the scale factor along the <I>LZScale</I> axis.
 *
 * @note If scale factors other than 1 are applied to the modelview matrix and lighting is enabled,
 * lighting often appears wrong. In that case, enable automatic normalization of normals by calling
 * glEnable() with the argument <CODE>GL_NORMALIZE</CODE>.
 *
 * @sa glEnable, glMatrixMode, glMultMatrixf, glPushMatrix, glRotatef, glTranslatef
 */
GLAPI void APIENTRY glScalef( GLfloat LXScale, GLfloat LYScale, GLfloat LZScale )
{
    JS_APIENTRY_TIMER( glScalef );
#ifdef JS_DPM
    if ( _jsdpmState )
    {
        _JS_DPM_API_ENTRY_3( glScalef, JSDPM_CALL_TYPE_STATE_GENERAL, , 3,
                             LXScale, sizeof( GLfloat ), INPUT_BY_VALUE, DPM_API_ARGUMENT_FLOAT_VALUE,
                             LYScale, sizeof( GLfloat ), INPUT_BY_VALUE, DPM_API_ARGUMENT_FLOAT_VALUE,
                             LZScale, sizeof( GLfloat ), INPUT_BY_VALUE, DPM_API_ARGUMENT_FLOAT_VALUE );
    }
#endif

    PSGLcontext* LContext = _CurrentContext;
    JS_ASSERT( LContext );
    jsMatrixStack* LMatrixStack = NULL;
    jsMatrixf LMatrix = NULL;

    jsContextCurrentMatrixf( LContext, LMatrixStack, LMatrix );
#ifndef JS_NO_ERROR_CHECK
    if ( !LMatrixStack )
    {
        _jsSetError( GL_INVALID_OPERATION );
        return;
    }
#endif

    LMatrix[M00] *= LXScale;LMatrix[M10] *= LXScale;LMatrix[M20] *= LXScale;LMatrix[M30] *= LXScale;
    LMatrix[M01] *= LYScale;LMatrix[M11] *= LYScale;LMatrix[M21] *= LYScale;LMatrix[M31] *= LYScale;
    LMatrix[M02] *= LZScale;LMatrix[M12] *= LZScale;LMatrix[M22] *= LZScale;LMatrix[M32] *= LZScale;

    LContext->needValidate |= PSGL_VALIDATE_FFX_VERTEX_PROGRAM;
    LMatrixStack->dirty = GL_TRUE;
    if ( LContext->MatrixMode == GL_MODELVIEW )
        LContext->InverseModelViewValid = GL_FALSE;
}

/**
 * @short Multiply the current matrix by a rotation matrix
 *
 * glRotatef() produces a rotation of <I><c>angle</c></I> degrees around the vector (<I><c>x, y, z</c></I>) .
 * The current matrix (see glMatrixMode()) is multiplied by a rotation matrix
 * with the product replacing the current matrix, as if glMultMatrixf() was called
 * with the following matrix as its argument:
 *  
 * <code>|  x<sup>2</sup>(1-c)+c   xy(1-c)-zs    xz(1-c)+ys  0  |</code>
 *
 * <code>|  xy(1-c)+zs   y<sup>2</sup>(1-c)+c    yz(1-c)-xs  0  |</code>
 *
 * <code>|  xz(1-c)-ys   yz(1-c)+xs    z<sup>2</sup>(1-c)+c  0  |</code>
 *
 * <code>|      0             0             0                1  |</code>
 *
 * Where c = cos (<I>angle</I>) , s = sin (<I><c>angle</c></I>) , and || (<I><c>x, y, z</c></I>) || = 1,
 * (if not, the GL will normalize this vector).
 *
 * If the matrix mode is either <CODE>GL_MODELVIEW</CODE> or <CODE>GL_PROJECTION</CODE>,
 * all objects drawn after glRotatef() is called are rotated.
 * Use glPushMatrix() and glPopMatrix() to save and restore the unrotated coordinate system.
 *
 * @param angle  Specifies the angle of rotation, in degrees.
 * @param x,y,z  Specify the <I>x</I>, <I>y</I>, and <I>z</I> coordinates of a vector, respectively.
 *
 * @note This rotation follows the right-hand rule, so if the vector (<I><c>x, y, z</c></I>)
 * points toward the user, the rotation will be counterclockwise.
 * @par
 * glRotatef() calls single-precision versions of sine and cosine to
 * perform the rotation and can consequently cause a loss of accuracy in
 * the computation.  To avoid this source of computational error, you
 * can let the rendering engine perform rotations instead.  If you
 * compute the local-to-world matrix of a node yourself with a library
 * of whatever accuracy you want, and then cache it with the node, you can
 * use glLoadMatrixf() and glMultMatrixf() (or their related functions)
 * later to draw the object.  By
 * letting the engine perform the rotation through this matrix
 * multiplication, you may get the advantage of higher-accuracy
 * computation.
 *
 * @par Errors
Use glGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>GL_INVALID_VALUE</CODE></TD>
<TD>The rotation vector is (0, 0, 0).</TD>
</TR>
</TABLE>
 *
 * @sa glMatrixMode, glMultMatrixf, glPushMatrix, glScalef, glTranslatef
 */
GLAPI void APIENTRY glRotatef( GLfloat angle, GLfloat x, GLfloat y, GLfloat z )
{
    JS_APIENTRY_TIMER( glRotatef );
#ifdef JS_DPM
    if ( _jsdpmState )
    {
        _JS_DPM_API_ENTRY_4( glRotatef, JSDPM_CALL_TYPE_STATE_GENERAL, , 4,
                             angle, sizeof( GLfloat ), INPUT_BY_VALUE, DPM_API_ARGUMENT_FLOAT_VALUE,
                             x, sizeof( GLfloat ), INPUT_BY_VALUE, DPM_API_ARGUMENT_FLOAT_VALUE,
                             y, sizeof( GLfloat ), INPUT_BY_VALUE, DPM_API_ARGUMENT_FLOAT_VALUE,
                             z, sizeof( GLfloat ), INPUT_BY_VALUE, DPM_API_ARGUMENT_FLOAT_VALUE );
    }
#endif
    PSGLcontext* LContext = _CurrentContext;
    JS_ASSERT( LContext );
    jsMatrixStack* LMatrixStack = NULL;
    jsMatrixf LMatrix = NULL;


    jsContextCurrentMatrixf( LContext, LMatrixStack, LMatrix );
#ifndef JS_NO_ERROR_CHECK
    if ( !LMatrixStack )
    {
        _jsSetError( GL_INVALID_OPERATION );
        return;
    }
#endif


    GLfloat a = angle * jsDEGREES_TO_RADIANS;
    GLfloat c = cosf( a );
    GLfloat one_minus_c = 1.f - c;
    GLfloat s = sinf( a );
    GLfloat n = x * x + y * y + z * z;
    GLfloat m00, m01, m02, m10, m11, m12, m20, m21, m22;
    GLfloat L00, L01, L02, L10, L11, L12, L20, L21, L22, L30, L31, L32;


    if ( n != 1.f )
    {
#ifndef JS_NO_ERROR_CHECK
        if ( n == 0.f )
        {
            _jsSetError( GL_INVALID_VALUE );
            return;
        }
#endif
        // normalize (x,y,z)
        //
        n = 1.f / sqrtf( n );
        x *= n;
        y *= n;
        z *= n;
    }

    // x2(1-c)+c   xy(1-c)-zs  xz(1-c)+ys  0
    // yx(1-c)+zs  y2(1-c)+c   yz(1-c)-xs  0
    // xz(1-c)-ys  yz(1-c)+xs  z2(1-c)+c   0
    // 0           0           0           1
    //
    // Where c=cos(angle), s=sin(angle), and ||(x,y,z)||=1 (if not, the GL will normalize this vector)
    //
    m00 = x * x * one_minus_c + c;
    m01 = x * y * one_minus_c - z * s;
    m02 = x * z * one_minus_c + y * s;
    m10 = y * x * one_minus_c + z * s;
    m11 = y * y * one_minus_c + c;
    m12 = y * z * one_minus_c - x * s;
    m20 = z * x * one_minus_c - y * s;
    m21 = z * y * one_minus_c + x * s;
    m22 = z * z * one_minus_c + c;

    L00 = LMatrix[M00];L01 = LMatrix[M01];L02 = LMatrix[M02];
    L10 = LMatrix[M10];L11 = LMatrix[M11];L12 = LMatrix[M12];
    L20 = LMatrix[M20];L21 = LMatrix[M21];L22 = LMatrix[M22];
    L30 = LMatrix[M30];L31 = LMatrix[M31];L32 = LMatrix[M32];

    LMatrix[M00] = L00 * m00 + L01 * m10 + L02 * m20;
    LMatrix[M01] = L00 * m01 + L01 * m11 + L02 * m21;
    LMatrix[M02] = L00 * m02 + L01 * m12 + L02 * m22;

    LMatrix[M10] = L10 * m00 + L11 * m10 + L12 * m20;
    LMatrix[M11] = L10 * m01 + L11 * m11 + L12 * m21;
    LMatrix[M12] = L10 * m02 + L11 * m12 + L12 * m22;

    LMatrix[M20] = L20 * m00 + L21 * m10 + L22 * m20;
    LMatrix[M21] = L20 * m01 + L21 * m11 + L22 * m21;
    LMatrix[M22] = L20 * m02 + L21 * m12 + L22 * m22;

    LMatrix[M30] = L30 * m00 + L31 * m10 + L32 * m20;
    LMatrix[M31] = L30 * m01 + L31 * m11 + L32 * m21;
    LMatrix[M32] = L30 * m02 + L31 * m12 + L32 * m22;

    LContext->needValidate |= PSGL_VALIDATE_FFX_VERTEX_PROGRAM;
    LMatrixStack->dirty = GL_TRUE;
    if ( LContext->MatrixMode == GL_MODELVIEW )
        LContext->InverseModelViewValid = GL_FALSE;
}



/**
 * @short Specify which matrix is the current matrix
 *
 * glMatrixMode() sets the current matrix mode. <I><c>mode</c></I> can assume one of three values:

 * <TABLE rules=all>
 * <TR>
 * <TD><CODE>GL_MODELVIEW</CODE></TD>
 * <TD>
 * Applies subsequent matrix operations to the modelview matrix stack.
 * </TD>
 * </TR>
 * <TR>
 * <TD><CODE>GL_PROJECTION</CODE></TD>
 * <TD>
 * Applies subsequent matrix operations to the projection matrix stack.
 * </TD>
 * </TR>
 * <TR>
 * <TD><CODE>GL_TEXTURE</CODE></TD>
 * <TD>
 * Applies subsequent matrix operations to the texture matrix stack.
 * </TD>
 * </TR>
 * </TABLE>
 *
 * @param mode  Specifies which matrix stack is the target for subsequent matrix operations.
 * Three values are accepted: <CODE>GL_MODELVIEW</CODE>, <CODE>GL_PROJECTION</CODE>, and
 * <CODE>GL_TEXTURE</CODE>. The initial value is <CODE>GL_MODELVIEW</CODE>.
 *
 * @par Errors
Use glGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>GL_INVALID_ENUM</CODE></TD>
<TD><I><c>mode</c></I> is not an accepted value.</TD>
</TR>
</TABLE>
 *
 * @sa glLoadMatrixf, glPushMatrix, glPopMatrix
 */
GLAPI void APIENTRY glMatrixMode( GLenum mode )
{
    JS_APIENTRY_TIMER( glMatrixMode );
#ifdef JS_DPM
    if ( _jsdpmState ) { _JS_DPM_API_ENTRY_1( glMatrixMode, JSDPM_CALL_TYPE_STATE_GENERAL, , 1, mode, sizeof( GLenum ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM ); }
#endif
    PSGLcontext* LContext = _CurrentContext;
    JS_ASSERT( LContext );

#ifndef JS_NO_ERROR_CHECK
    switch ( mode )
    {
        case GL_MODELVIEW:
        case GL_PROJECTION:
        case GL_TEXTURE:
            break;
        default:
            _jsSetError( GL_INVALID_ENUM );
            return;
    }
#endif

    LContext->MatrixMode = mode;
}


/**
 * @short Replace the current matrix with the specified matrix
 *
 * glLoadMatrixf() replaces the current matrix with the one whose elements are specified by <I><c>m</c></I>.
 * The current matrix is the projection matrix, modelview matrix, or texture matrix, depending on
 * the current matrix mode (see glMatrixMode()).
 *
 * The current matrix, M, defines a transformation of coordinates.
 * For instance, assume M refers to the modelview matrix. If v = (v [0] , v [1] , v [2] , v [3] )
 * is the set of object coordinates of a vertex, and m points to an array of
 * 16 fixed-point or single-precision floating-point values m [0] , m [1] , ... m [15] ,
 * then the modelview transformation M(v) does the following:

@code
          | m [0]   m [4]   m [8]   m [12] |       v [0]
M(v) = | m [1]   m [5]   m [9]   m [13] |   X   v [1]
          | m [2]   m [6]   m [10]  m [14] |       v [2]
          | m [3]   m [7]   m [11]  m [15] |       v [3]
@endcode

 * Where " X " denotes matrix multiplication.
 *
 * Projection and texture transformations are similarly defined.
 *
 * @param  m  Specifies a pointer to 16 consecutive values, which are used as the elements of a 4 X 4 column-major matrix.
 *
 * @sa glLoadIdentity, glMatrixMode, glMultMatrixf, glPushMatrix
 */
GLAPI void APIENTRY glLoadMatrixf( const GLfloat* m )
{
    JS_APIENTRY_TIMER( glLoadMatrixf );
#ifdef JS_DPM
    if ( _jsdpmState ) { _JS_DPM_API_ENTRY_1( glLoadMatrixf, JSDPM_CALL_TYPE_STATE_GENERAL, , 1, m, 16*sizeof( GLfloat ), INPUT_BY_REFERENCE, DPM_API_ARGUMENT_POINTER ); }
#endif
    PSGLcontext* LContext = _CurrentContext;
    JS_ASSERT( LContext );
    jsMatrixStack* LMatrixStack = NULL;
    jsMatrixf LMatrix = NULL;


    jsContextCurrentMatrixf( LContext, LMatrixStack, LMatrix );
#ifndef JS_NO_ERROR_CHECK
    if ( !LMatrixStack )
    {
        _jsSetError( GL_INVALID_OPERATION );
        return;
    }
#endif


    memcpy( LMatrix, m, jsMATRIX_SIZEf );

#if __JS_DEBUG_MATRIX_STACK == 1
    _jsLog( 0, "%s() MatrixStackPtr: %d", __func__, LMatrixStack->MatrixStackPtr );
#endif

    LContext->needValidate |= PSGL_VALIDATE_FFX_VERTEX_PROGRAM;
    LMatrixStack->dirty = GL_TRUE;
    if ( LContext->MatrixMode == GL_MODELVIEW )
        LContext->InverseModelViewValid = GL_FALSE;
}


/**
 * @short Multiply the current matrix by a perspective matrix
 *
 * glFrustumf() describes a perspective matrix that produces a perspective projection.
 * The current matrix (see glMatrixMode()) is multiplied by this matrix and the result
 * replaces the current matrix, as if glMultMatrixf() was called with the following matrix as its argument:

@code
  |   2/(right - left)        0                A          0   |
  |      0            2/(top - bottom)         B          0   |
  |      0                    0                C          D   |
  |      0                    0               -1          0   |
@endcode

 * where
 * @code
 * A = - (right + left)/(right - left)
 * B = - (top + bottom)/(top - bottom)
 * C = - (zFar + zNear)/(zFar - zNear)
 * D = - (2 &times; zFar &times; zNear)/(zFar - zNear)
 * @endcode
 *
 * Typically, the matrix mode is <CODE>GL_PROJECTION</CODE>, and (<I><c>left</c></I>, <I><c>bottom</c></I>, <I><c>-zNear</c></I>)
 * and (<I><c>right</c></I>, <I><c>top</c></I>, <I><c>-zNear</c></I>) specify the points on the near clipping plane that are mapped
 * to the lower left and upper right corners of the window, assuming that the eye is located at
 * (0, 0, 0). <I><c>-zFar</c></I> specifies the location of the far clipping plane.
 * Both <I><c>zNear</c></I> and <I><c>zFar</c></I> must be positive.
 *
 * Use glPushMatrix() and glPopMatrix() to save and restore the current matrix stack.
 *
 * @param left,right   Specify the coordinates for the left and right vertical clipping planes.
 * @param bottom,top   Specify the coordinates for the bottom and top horizontal clipping planes.
 * @param zNear    Specify the distance to the near depth clipping planes
 * @param zFar    Specify the distances to the far depth clipping plane.
 *
 * @note
 * Depth buffer precision is affected by the values specified for <I><c>zNear</c></I> and <I><c>zFar</c></I>.
 * The greater the ratio of <I><c>zFar</c></I> to <I><c>zNear</c></I> is, the less effective the depth buffer
 * will be at distinguishing between surfaces that are near each other. If r = <I><c>zFar</c></I>/<I><c>zNear</c></I>,
 * roughly log2(r) bits of depth buffer precision are lost.
 * Because r approaches infinity as <I><c>zNear</c></I> approaches 0, <I><c>zNear</c></I> must never be set to 0.
 *
 * @par Errors
Use glGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>GL_INVALID_VALUE</CODE></TD>
<TD><I><c>zNear</c></I> or <I><c>zFar</c></I> is not positive,
or if <I><c>left</c></I> = <I><c>right</c></I>, <I><c>bottom</c></I> = <I><c>top</c></I>, or <I><c>zNear</c></I> = <I><c>zFar</c></I>.</TD>
</TR>
</TABLE>
 *
 * @sa glOrthof, glMatrixMode, glMultMatrixf, glPushMatrix, glViewport
 */
GLAPI void APIENTRY glFrustumf( GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat zNear, GLfloat zFar )
{
    JS_APIENTRY_TIMER( glFrustumf );
#ifdef JS_DPM
    if ( _jsdpmState )
    {
        _JS_DPM_API_ENTRY_6( glFrustumf, JSDPM_CALL_TYPE_STATE_GENERAL, , 6,
                             left, sizeof( GLfloat ), INPUT_BY_VALUE, DPM_API_ARGUMENT_FLOAT_VALUE,
                             right, sizeof( GLfloat ), INPUT_BY_VALUE, DPM_API_ARGUMENT_FLOAT_VALUE,
                             bottom, sizeof( GLfloat ), INPUT_BY_VALUE, DPM_API_ARGUMENT_FLOAT_VALUE,
                             top, sizeof( GLfloat ), INPUT_BY_VALUE, DPM_API_ARGUMENT_FLOAT_VALUE,
                             zNear, sizeof( GLfloat ), INPUT_BY_VALUE, DPM_API_ARGUMENT_FLOAT_VALUE,
                             zFar, sizeof( GLfloat ), INPUT_BY_VALUE, DPM_API_ARGUMENT_FLOAT_VALUE );
    }
#endif
    PSGLcontext* LContext = _CurrentContext;
    jsMatrixStack* LMatrixStack = NULL;
    jsMatrixf LMatrix = NULL;

#ifndef JS_NO_ERROR_CHECK
    if (( left == right ) || ( bottom == top ) || ( zNear == zFar ) || ( zNear <= 0 ) || ( zFar <= 0 ) )
    {
        _jsSetError( GL_INVALID_VALUE );
        return;
    }
#endif
    jsContextCurrentMatrixf( LContext, LMatrixStack, LMatrix );
#ifndef JS_NO_ERROR_CHECK
    if ( !LMatrixStack )
    {
        _jsSetError( GL_INVALID_OPERATION );
        return;
    }
#endif

    GLfloat two_znear, r_minus_l, t_minus_b, zf_minus_zn;
    GLfloat m00, m02, m11, m12, m22, m23, m32;
    GLfloat L00, L01, L02, L03, L10, L11, L12, L13, L20, L21, L22, L23, L30, L31, L32, L33;


    two_znear = 2.0 * zNear;
    r_minus_l = right - left;
    t_minus_b = top - bottom;
    zf_minus_zn = zFar - zNear;
    m00 = two_znear / ( r_minus_l );
    m02 = ( right + left ) / ( r_minus_l );
    m11 = two_znear / ( t_minus_b );
    m12 = ( top + bottom ) / ( t_minus_b );
    m22 = -( zFar + zNear ) / ( zf_minus_zn );
    m23 = -two_znear * zFar / ( zf_minus_zn );
    m32 = -1.0;

    L00 = LMatrix[M00];L01 = LMatrix[M01];L02 = LMatrix[M02];L03 = LMatrix[M03];
    L10 = LMatrix[M10];L11 = LMatrix[M11];L12 = LMatrix[M12];L13 = LMatrix[M13];
    L20 = LMatrix[M20];L21 = LMatrix[M21];L22 = LMatrix[M22];L23 = LMatrix[M23];
    L30 = LMatrix[M30];L31 = LMatrix[M31];L32 = LMatrix[M32];L33 = LMatrix[M33];

    LMatrix[M00] = L00 * m00;
    LMatrix[M01] = L01 * m11;
    LMatrix[M02] = L00 * m02 + L01 * m12 + L02 * m22 + L03 * m32;
    LMatrix[M03] = L02 * m23;

    LMatrix[M10] = L10 * m00;
    LMatrix[M11] = L11 * m11;
    LMatrix[M12] = L10 * m02 + L11 * m12 + L12 * m22 + L13 * m32;
    LMatrix[M13] = L12 * m23;

    LMatrix[M20] = L20 * m00;
    LMatrix[M21] = L21 * m11;
    LMatrix[M22] = L20 * m02 + L21 * m12 + L22 * m22 + L23 * m32;
    LMatrix[M23] = L22 * m23;

    LMatrix[M30] = L30 * m00;
    LMatrix[M31] = L31 * m11;
    LMatrix[M32] = L30 * m02 + L31 * m12 + L32 * m22 + L33 * m32;
    LMatrix[M33] = L32 * m23;

    LContext->needValidate |= PSGL_VALIDATE_FFX_VERTEX_PROGRAM;
    LMatrixStack->dirty = GL_TRUE;
    if ( LContext->MatrixMode == GL_MODELVIEW )
        LContext->InverseModelViewValid = GL_FALSE;
}


/**
 * @short Multiply the current matrix with an orthographic matrix
 *
 * glOrthof() describes a transformation that produces a parallel projection.
 * The current matrix (see glMatrixMode()) is multiplied by this matrix and the result replaces
 * the current matrix, as if glMultMatrixf() was called with the following matrix as its argument:

@code
  |  2/(right-left)       0               0          tx   |
  |        0        2/(top-bottom)        0          ty   |
  |        0              0        -2/(zFar-zNear)   tz   |
  |        0              0                   0          1    |
@endcode

 * where
 * @code
 *     tx = - (right + left)/(right - left)
 *     ty = - (top + bottom)/(top - bottom)
 *     tz = - (zFar + zNear)/(zFar - zNear)
 * @endcode
 *
 * Typically, the matrix mode is <CODE>GL_PROJECTION</CODE>, and (<I><c>left</c></I>, <I><c>bottom</c></I>, <I><c>-zNear</c></I>)
 * and (<I><c>right</c></I>, <I><c>top</c></I>, <I><c>-zNear</c></I>) specify the points on the near clipping plane that are mapped
 * to the lower left and upper right corners of the window, respectively, assuming that the eye is located at
 * (0, 0, 0). <I><c>-zFar</c></I> specifies the location of the far clipping plane.
 * Both <I><c>zNear</c></I> and <I><c>zFar</c></I> can be either positive or negative.
 *
 * Use glPushMatrix() and glPopMatrix() to save and restore the current matrix stack.
 *
 * @param left     Specify the coordinates for the left vertical clipping plane.
 * @param right    Specify the coordinates for the right vertical clipping plane.
 * @param bottom   Specify the coordinates for the bottom horizontal clipping plane.
 * @param top      Specify the coordinates for the top horizontal clipping plane.
 * @param zNear    Specify the distances to the near depth clipping plane.
 * @param zFar     Specify the distances to the far depth clipping plane.
 * These values are negative if the plane is to be behind the viewer.
 *
 * @par Errors
Use glGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>GL_INVALID_VALUE</CODE></TD>
<TD><I><c>zNear</c></I> = <I><c>zFar</c></I>.</TD>
</TR>
</TABLE>
 *
 * @sa glFrustumf, glMatrixMode, glMultMatrixf, glPushMatrix, glViewport
 */
GLAPI void APIENTRY glOrthof( GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat zNear, GLfloat zFar )
{
    JS_APIENTRY_TIMER( glOrthof );
#ifdef JS_DPM
    if ( _jsdpmState )
    {
        _JS_DPM_API_ENTRY_6( glOrthof, JSDPM_CALL_TYPE_STATE_GENERAL, , 6,
                             left, sizeof( GLfloat ), INPUT_BY_VALUE, DPM_API_ARGUMENT_FLOAT_VALUE,
                             right, sizeof( GLfloat ), INPUT_BY_VALUE, DPM_API_ARGUMENT_FLOAT_VALUE,
                             bottom, sizeof( GLfloat ), INPUT_BY_VALUE, DPM_API_ARGUMENT_FLOAT_VALUE,
                             top, sizeof( GLfloat ), INPUT_BY_VALUE, DPM_API_ARGUMENT_FLOAT_VALUE,
                             zNear, sizeof( GLfloat ), INPUT_BY_VALUE, DPM_API_ARGUMENT_FLOAT_VALUE,
                             zFar, sizeof( GLfloat ), INPUT_BY_VALUE, DPM_API_ARGUMENT_FLOAT_VALUE );
    }
#endif
    PSGLcontext* LContext = _CurrentContext;
    jsMatrixStack* LMatrixStack = NULL;
    jsMatrixf LMatrix = NULL;

#ifndef JS_NO_ERROR_CHECK
    if (( left == right ) || ( bottom == top ) || ( zNear == zFar ) )
    {
        _jsSetError( GL_INVALID_VALUE );
        return;
    }
#endif

    jsContextCurrentMatrixf( LContext, LMatrixStack, LMatrix );
#ifndef JS_NO_ERROR_CHECK
    if ( !LMatrixStack )
    {
        _jsSetError( GL_INVALID_OPERATION );
        return;
    }
#endif

    GLfloat L00, L01, L02, L03, L10, L11, L12, L13, L20, L21, L22, L23, L30, L31, L32, L33;


    GLfloat m00 = 2.f / ( right - left );
    GLfloat m03 = -( right + left ) / ( right - left );
    GLfloat m11 = 2.f / ( top - bottom );
    GLfloat m13 = -( top + bottom ) / ( top - bottom );
    GLfloat m22 = -2.f / ( zFar - zNear );
    GLfloat m23 = -( zFar + zNear ) / ( zFar - zNear );

    L00 = LMatrix[M00];L01 = LMatrix[M01];L02 = LMatrix[M02];L03 = LMatrix[M03];
    L10 = LMatrix[M10];L11 = LMatrix[M11];L12 = LMatrix[M12];L13 = LMatrix[M13];
    L20 = LMatrix[M20];L21 = LMatrix[M21];L22 = LMatrix[M22];L23 = LMatrix[M23];
    L30 = LMatrix[M30];L31 = LMatrix[M31];L32 = LMatrix[M32];L33 = LMatrix[M33];

    LMatrix[M00] = L00 * m00;
    LMatrix[M01] = L01 * m11;
    LMatrix[M02] = L02 * m22;
    LMatrix[M03] = L00 * m03 + L01 * m13 + L02 * m23 + L03;

    LMatrix[M10] = L10 * m00;
    LMatrix[M11] = L11 * m11;
    LMatrix[M12] = L12 * m22;
    LMatrix[M13] = L10 * m03 + L11 * m13 + L12 * m23 + L13;

    LMatrix[M20] = L20 * m00;
    LMatrix[M21] = L21 * m11;
    LMatrix[M22] = L22 * m22;
    LMatrix[M23] = L20 * m03 + L21 * m13 + L22 * m23 + L23;

    LMatrix[M30] = L30 * m00;
    LMatrix[M31] = L31 * m11;
    LMatrix[M32] = L32 * m22;
    LMatrix[M33] = L30 * m03 + L31 * m13 + L32 * m23 + L33;

    LContext->needValidate |= PSGL_VALIDATE_FFX_VERTEX_PROGRAM;
    LMatrixStack->dirty = GL_TRUE;
    if ( LContext->MatrixMode == GL_MODELVIEW )
        LContext->InverseModelViewValid = GL_FALSE;
}

/** @} Matrix */

/////////////////////////////////////////////////////////////////////////////

/**
 *
 *  The remainder of this file contains internal functions.
 *
 */

// matrix inversion function need to get the inverse model view matrix
typedef GLfloat JSMatrix4[4][4];
void jsMatrixInverse( GLfloat * matIn, GLfloat * matOut )
{
    GLfloat fA0 = matIn[ 0] * matIn[ 5] - matIn[ 1] * matIn[ 4];
    GLfloat fA1 = matIn[ 0] * matIn[ 6] - matIn[ 2] * matIn[ 4];
    GLfloat fA2 = matIn[ 0] * matIn[ 7] - matIn[ 3] * matIn[ 4];
    GLfloat fA3 = matIn[ 1] * matIn[ 6] - matIn[ 2] * matIn[ 5];
    GLfloat fA4 = matIn[ 1] * matIn[ 7] - matIn[ 3] * matIn[ 5];
    GLfloat fA5 = matIn[ 2] * matIn[ 7] - matIn[ 3] * matIn[ 6];
    GLfloat fB0 = matIn[ 8] * matIn[13] - matIn[ 9] * matIn[12];
    GLfloat fB1 = matIn[ 8] * matIn[14] - matIn[10] * matIn[12];
    GLfloat fB2 = matIn[ 8] * matIn[15] - matIn[11] * matIn[12];
    GLfloat fB3 = matIn[ 9] * matIn[14] - matIn[10] * matIn[13];
    GLfloat fB4 = matIn[ 9] * matIn[15] - matIn[11] * matIn[13];
    GLfloat fB5 = matIn[10] * matIn[15] - matIn[11] * matIn[14];

    GLfloat fDet = fA0 * fB5 - fA1 * fB4 + fA2 * fB3 + fA3 * fB2 - fA4 * fB1 + fA5 * fB0;

    if ( fabs( fDet ) == 0.f )
    {
        memset( matOut, 0, sizeof( JSMatrix4 ) );
        return;
    }

    JSMatrix4 kInv;
    kInv[0][0] = + matIn[ 5] * fB5 - matIn[ 6] * fB4 + matIn[ 7] * fB3;
    kInv[1][0] = - matIn[ 4] * fB5 + matIn[ 6] * fB2 - matIn[ 7] * fB1;
    kInv[2][0] = + matIn[ 4] * fB4 - matIn[ 5] * fB2 + matIn[ 7] * fB0;
    kInv[3][0] = - matIn[ 4] * fB3 + matIn[ 5] * fB1 - matIn[ 6] * fB0;
    kInv[0][1] = - matIn[ 1] * fB5 + matIn[ 2] * fB4 - matIn[ 3] * fB3;
    kInv[1][1] = + matIn[ 0] * fB5 - matIn[ 2] * fB2 + matIn[ 3] * fB1;
    kInv[2][1] = - matIn[ 0] * fB4 + matIn[ 1] * fB2 - matIn[ 3] * fB0;
    kInv[3][1] = + matIn[ 0] * fB3 - matIn[ 1] * fB1 + matIn[ 2] * fB0;
    kInv[0][2] = + matIn[13] * fA5 - matIn[14] * fA4 + matIn[15] * fA3;
    kInv[1][2] = - matIn[12] * fA5 + matIn[14] * fA2 - matIn[15] * fA1;
    kInv[2][2] = + matIn[12] * fA4 - matIn[13] * fA2 + matIn[15] * fA0;
    kInv[3][2] = - matIn[12] * fA3 + matIn[13] * fA1 - matIn[14] * fA0;
    kInv[0][3] = - matIn[ 9] * fA5 + matIn[10] * fA4 - matIn[11] * fA3;
    kInv[1][3] = + matIn[ 8] * fA5 - matIn[10] * fA2 + matIn[11] * fA1;
    kInv[2][3] = - matIn[ 8] * fA4 + matIn[ 9] * fA2 - matIn[11] * fA0;
    kInv[3][3] = + matIn[ 8] * fA3 - matIn[ 9] * fA1 + matIn[10] * fA0;

    GLfloat fInvDet = (( GLfloat )1.0 ) / fDet;
    for ( int iRow = 0; iRow < 4; iRow++ )
    {
        for ( int iCol = 0; iCol < 4; iCol++ )
            kInv[iRow][iCol] *= fInvDet;
    }

    memcpy( matOut, kInv, sizeof( kInv ) );
}
