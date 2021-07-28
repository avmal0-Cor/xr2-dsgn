/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 *                Copyright (C) 2009 Sony Computer Entertainment Inc.
 *                                               All Rights Reserved.
 */

 
/*
 * 
 */

#include <math.h>
#include <float.h>
#include <stdio.h>

#include <PSGL/psgl.h>
#include <PSGL/private.h>
#include <PSGL/Debug.h>
#include <PSGL/TypeUtils.h>
#include <PSGL/PerfCounters.h>
#include <PSGL/DPM2.h>
#include <ppu_intrinsics.h>
const uint32_t c_rounded_size_of_jsDrawParams = (sizeof(jsDrawParams)+0x7f)&~0x7f;
static uint8_t s_dparams_buff[ c_rounded_size_of_jsDrawParams ] __attribute__((aligned(128)));

#ifdef JS_DEBUG
#define _JS_CHECK_FOR_DRAW_OUT_OF_VBO_BOUNDS
#endif

#ifdef JS_DEBUG
GLboolean _jsCheckAttribCaches( jsAttributeState* as )

{
    JS_ASSERT( as );
    GLuint attribHasVBOMask = 0;
    for ( int i = 0; i < _JS_MAX_VERTEX_ATTRIBS; ++i )
    {
        attribHasVBOMask |= (( as->attrib[i].arrayBuffer != 0 ) ? 1 : 0 ) << i;
    }
    JS_ASSERT( attribHasVBOMask == as->HasVBOMask );

    return (
               ( attribHasVBOMask == as->HasVBOMask )
           );
}
#endif

/** @addtogroup OpenGL
 *
 * @{
 */

/**
 * @short Define an array of vertex coordinates
 *
 * glVertexPointer() specifies the location and data of an array of vertex coordinates to use when rendering.
 * <I><c>stride</c></I> specifies the byte stride
 * from one vertex to the next allowing vertices and attributes to be packed into a single array or stored in separate
 * arrays. (Single-array storage may be more efficient on some implementations.)
 *
 * When a vertex array is specified, <I><c>size</c></I>, <I><c>type</c></I>, <I><c>stride</c></I>, and <I><c>pointer</c></I>
 * are saved as client-side state.
 *  However, if there is a currently bound vertex buffer object, then <I><c>pointer</c></I>
 *  does not reference a client-side memory location.  Instead, <I><c>pointer</c></I>
 *  is an offset into a buffer object with its data store on the server side.
 *  glBufferData() or glBufferSubData()
 *  should have already been called to load the data into the server memory.
 *
 * If the vertex array is enabled, it is used when glDrawArrays(), glDrawElements(), or glDrawRangeElements() is called.
 * To enable and disable the vertex array, call glEnableClientState() and glDisableClientState() with the argument
 * <CODE>GL_VERTEX_ARRAY</CODE>. The vertex array is initially disabled and is not accessed when
 * glDrawArrays(), glDrawElements(), or glDrawRangeElements() is called.
 *
 * Use glDrawArrays() to construct a sequence of primitives (all of the same type) from prespecified vertex
 * and vertex attribute arrays. Use glDrawElements() or glDrawRangeElements() to construct a sequence of
 * primitives by indexing vertices and vertex attributes.
 *
 * @param size  Specifies the number of coordinates per vertex. Must be 1, 2, 3, or 4. The initial value is 4.
 * @param type  Specifies the data type of each vertex coordinate in the array.
 * Symbolic constants <CODE>GL_BYTE</CODE>, <CODE>GL_UNSIGNED_BYTE</CODE>, <CODE>GL_SHORT</CODE>, <CODE>GL_FLOAT</CODE>, <CODE>GL_HALF_FLOAT_ARB</CODE>, <CODE>GL_FIXED_11_11_10_SCE</CODE>, and
 * <CODE>GL_FIXED</CODE> are accepted.  The initial value is <CODE>GL_FLOAT</CODE>.
 * @param stride  Specifies the byte offset between consecutive vertices. If <I><c>stride</c></I> is 0,
 * the vertices are understood to be tightly packed in the array. The initial value is 0.
 * @param pointer  Usually specifies a pointer to the first coordinate of the first vertex in the array.
 * If a vertex buffer object is bound with the target <CODE>GL_ARRAY_BUFFER</CODE>, then <I><c>pointer</c></I>
 * is an offset into a buffer object with its data store on the server side.
 * The initial value is 0.
 *
 *  @note
 *  glVertexPointer() is typically implemented on the client side, unless there is a current buffer object
 *  bound with the target <CODE>GL_ARRAY_BUFFER</CODE>.
 *  @par
 *  The format for <CODE>GL_FIXED_11_11_10_SCE</CODE> is 3 signed components packed into 32 bits.
 *  The component placement within the word is:
 *  <TABLE>
 *  <TR>
 *  <TD align="center">Component #</TD>
 *  <TD align="center">Bits</TD>
 *  <TD align="center">Shift</TD>
 *  </TR>
 *  <TR>
 *  <TD>0</TD><TD>11</TD><TD>0</TD>
 *  </TR>
 *  <TR>
 *  <TD>1</TD><TD>11</TD><TD>11</TD>
 *  </TR>
 *  <TR>
 *  <TD>2</TD><TD>10</TD><TD>22</TD>
 *  </TR>
 *  </TABLE>
 *  The interpretation of each component in the vertex program is normalized from [-1.0, 1.0].
 *  @par
 *  When <I><c>type</c></I> is set to <CODE>GL_FIXED_11_11_10_SCE</CODE>, <I><c>size</c></I> must be 3.
 *
 *  @par Errors
Use glGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>GL_INVALID_VALUE</CODE></TD>
<TD><I><c>size</c></I> is not 1, 2, 3, or 4, or <I><c>type</c></I> is <CODE>GL_FIXED_11_11_10_SCE</CODE> and <I>size</I> is not 3.
</TR>
<TR>
<TD><CODE>GL_INVALID_ENUM</CODE></TD>
<TD><I><c>type</c></I> is not an accepted value.</TD>
</TR>
<TR>
<TD><CODE>GL_INVALID_VALUE</CODE></TD>
<TD><I><c>stride</c></I> is negative.</TD>
</TR>
</TABLE>
 *
 * @sa glBufferData, glBufferSubData, glColorPointer, glDisableClientState, glDrawArrays, glDrawElements, glDrawRangeElements, glEnableClientState, glNormalPointer, glTexCoordPointer
 *
 */
GLAPI void APIENTRY glVertexPointer( GLint size, GLenum type, GLsizei stride, const GLvoid* pointer )
{
    JS_APIENTRY_TIMER( glVertexPointer );
#ifdef JS_DPM
    if ( _jsdpmState )
    {
        _JS_DPM_API_ENTRY_4( glVertexPointer, JSDPM_CALL_TYPE_STATE_GEOM, , 4,
                             size, sizeof( GLint ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             type, sizeof( GLenum ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM,
                             stride, sizeof( GLsizei ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             pointer, sizeof( GLvoid * ), INPUT_BY_VALUE, DPM_API_ARGUMENT_POINTER );	// just save the pointer value (not the contents) for deferred evaluation
    }
#endif
    // argument checking is done in _jsVertexAttribPointerNV.

    // call generic attribute
    _jsVertexAttribPointerNV( _JS_ATTRIB_POSITION_INDEX, size, type, GL_FALSE, stride, pointer );
}


/**
 *  @short Define an array of normals
 *
 *  glNormalPointer() specifies the location and data of an array of normals to use when rendering. <I><c>type</c></I> specifies the data type of the normal coordinates and <I><c>stride</c></I> gives the byte stride from one normal to the next, allowing vertices and attributes to be packed into a single array or stored in separate arrays. (Single-array storage may be more efficient on some implementations.) When a normal array is specified, <I><c>type</c></I>, <I><c>stride</c></I>, and <I><c>pointer</c></I>
 *  are saved as client-side state.
 *  However, if there is a currently bound vertex buffer object, then <I><c>pointer</c></I>
 *  does not reference a client-side memory location.  Instead, <I><c>pointer</c></I>
 *  is an offset into a buffer object with its data store on the server side.
 *  glBufferData() or glBufferSubData()
 *  should have already been called to load the data into the server memory.
 *
 *  If the normal array is enabled, it is used when glDrawArrays(), glDrawElements(), or glDrawRangeElements() is called. To enable and disable the normal array, call glEnableClientState() and glDisableClientState() with the argument <CODE>GL_NORMAL_ARRAY</CODE>. The normal array is initially disabled and isn&rsquo;t accessed when glDrawArrays(), glDrawElements(), or glDrawRangeElements() is called.

 *  Use glDrawArrays() to construct a sequence of primitives (all of the same type) from prespecified vertex and vertex attribute arrays. Use glDrawElements() or glDrawRangeElements() to construct a sequence of primitives by indexing vertices and vertex attributes.
 *
 *  @param type   Specifies the data type of each coordinate in the array.
 * Symbolic constants <CODE>GL_BYTE</CODE>, <CODE>GL_UNSIGNED_BYTE</CODE>, <CODE>GL_SHORT</CODE>, <CODE>GL_FLOAT</CODE>, <CODE>GL_HALF_FLOAT_ARB</CODE>, <CODE>GL_FIXED_11_11_10_SCE</CODE>, and
 *  <CODE>GL_FIXED</CODE> are accepted.  The initial value is <CODE>GL_FLOAT</CODE>.
 *  @param stride  Specifies the byte offset between consecutive normals. If <I><c>stride</c></I> is 0,
 *  the normals are understood to be tightly packed in the array. The initial value is 0.
 *  @param pointer  Specifies a pointer to the first coordinate of the first normal in the array.
 * If a vertex buffer object is bound with the target <CODE>GL_ARRAY_BUFFER</CODE>, then <I><c>pointer</c></I>
 * is an offset into a buffer object with its data store on the server side.
 *  The initial value is 0.
 *
 *  @note
 *  glNormalPointer() is typically implemented on the client side, unless there is a current buffer object
 *  bound with the target <CODE>GL_ARRAY_BUFFER</CODE>.
 *  @par
 *  The format for <CODE>GL_FIXED_11_11_10_SCE</CODE> is 3 signed components packed into 32 bits.
 *  The component placement within the word is:
 *  <TABLE>
 *  <TR>
 *  <TD align="center">Component #</TD>
 *  <TD align="center">Bits</TD>
 *  <TD align="center">Shift</TD>
 *  </TR>
 *  <TR>
 *  <TD>0</TD><TD>11</TD><TD>0</TD>
 *  </TR>
 *  <TR>
 *  <TD>1</TD><TD>11</TD><TD>11</TD>
 *  </TR>
 *  <TR>
 *  <TD>2</TD><TD>10</TD><TD>22</TD>
 *  </TR>
 *  </TABLE>
 *  The interpretation of each component in the vertex program is normalized from [-1.0, 1.0].
 *  
 *
 *  @par Errors
Use glGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>GL_INVALID_ENUM</CODE></TD>
<TD><I><c>type</c></I> is not an accepted value.</TD>
</TR>
<TR>
<TD><CODE>GL_INVALID_VALUE</CODE></TD>
<TD><I><c>stride</c></I> is negative.</TD>
</TR>
</TABLE>
 *
 * @sa glBufferData, glBufferSubData, glColorPointer, glDrawArrays, glDrawElements, glDrawRangeElements, glEnable, glTexCoordPointer, glVertexPointer
 */
GLAPI void APIENTRY glNormalPointer( GLenum type, GLsizei stride, const GLvoid* pointer )
{
    JS_APIENTRY_TIMER( glNormalPointer );
    if ( _jsdpmState )
    {
        _JS_DPM_API_ENTRY_3( glNormalPointer, JSDPM_CALL_TYPE_STATE_GEOM, , 3,
                             type, sizeof( GLenum ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM,
                             stride, sizeof( GLsizei ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             pointer, sizeof( GLvoid * ), INPUT_BY_VALUE, DPM_API_ARGUMENT_POINTER );	// just save the pointer value (not the contents) for deferred evaluation
    }

    // argument checking is done in _jsVertexAttribPointerNV.

    // call generic attribute
    _jsVertexAttribPointerNV( _JS_ATTRIB_NORMAL_INDEX, 3, type, GL_TRUE, stride, pointer );
}

/**
 *  @short Set the current normal vector
 *
 *  This function sets the current normal to the given coordinates.
 *  Byte, short, or integer arguments are converted to floating-point with a linear mapping
 *  that maps the most positive representable integer value to 1.0, and the most negative
 *  representable integer value to -1.0.
 *
 *  Normals specified with glNormal3f() need not have unit length.
 *  If <CODE>GL_NORMALIZE</CODE> is enabled, then normals of any length specified with
 *  glNormal3f() are normalized after transformation. If <CODE>GL_RESCALE_NORMAL</CODE> is enabled,
 *  normals are scaled by a scaling factor derived from the modelview matrix.
 *  <CODE>GL_RESCALE_NORMAL</CODE> requires that the originally specified normals were of
 *  unit length, and that the modelview matrix contain only uniform scales for proper results.
 *  To enable and disable normalization, call glEnable() and glDisable() with
 *  either <CODE>GL_NORMALIZE</CODE> or <CODE>GL_RESCALE_NORMAL</CODE>. Normalization is initially disabled.
 *
 *  @param x  Specifies the <I>x</I> coordinate of the new current normal.
 *  @param y  Specifies the <I>y</I> coordinate of the new current normal.
 *  @param z  Specifies the <I>z</I> coordinate of the new current normal.
 *
 *  @sa glColor4f, glEnable, glMultiTexCoord4f, glNormalPointer
 *
 */

GLAPI void APIENTRY glNormal3f( GLfloat x, GLfloat y, GLfloat z )
{
    JS_APIENTRY_TIMER( glNormal3f );
    if ( _jsdpmState )
    {
        _JS_DPM_API_ENTRY_3( glNormal3f, JSDPM_CALL_TYPE_STATE_GEOM, , 3,
                             x, sizeof( GLfloat ), INPUT_BY_VALUE, DPM_API_ARGUMENT_FLOAT_VALUE,
                             y, sizeof( GLfloat ), INPUT_BY_VALUE, DPM_API_ARGUMENT_FLOAT_VALUE,
                             z, sizeof( GLfloat ), INPUT_BY_VALUE, DPM_API_ARGUMENT_FLOAT_VALUE );
    }

    _jsVertexAttrib3fNV( _JS_ATTRIB_NORMAL_INDEX, x, y, z );
}

/**
 *  @short Set the current normal vector
 *
 *  glNormal3fv() sets the current normal to the given coordinates.
 *  It differs from glNormal3f() only in argument type.
 *  For more information, see the related function glNormal3f()
 *
 *  @param v  Specify a pointer to an array of three elements: the <I>x</I>, <I>y</I>, and <I>z</I>
 *  coordinates of the new current normal.  The initial value is (0, 0, 1).
 */

GLAPI void APIENTRY glNormal3fv( const GLfloat *v )
{
    JS_APIENTRY_TIMER( glNormal3fv );
    if ( _jsdpmState ) { _JS_DPM_API_ENTRY_1( glNormal3fv, JSDPM_CALL_TYPE_STATE_GEOM, , 1, v, 3*sizeof( GLfloat ), INPUT_BY_REFERENCE, DPM_API_ARGUMENT_POINTER ); }

    _jsVertexAttrib3fvNV( _JS_ATTRIB_NORMAL_INDEX, v );
}

/**
 *  @short Define an array of texture coordinates
 *
 *  glTexCoordPointer() specifies the location and data of an array of texture coordinates to use when
 *  rendering. <I><c>size</c></I> specifies the number of coordinates per element, and must be 2, 3, or 4.
 *  <I><c>type</c></I> specifies the data type of each texture coordinate and <I><c>stride</c></I> specifies the byte
 *  stride from one array element to the next allowing vertices and attributes to be packed into a single
 *  array or stored in separate arrays. (Single-array storage may be more efficient on some implementations.)
 *
 *  When a texture coordinate array is specified, <I><c>size</c></I>, <I><c>type</c></I>, <I><c>stride</c></I>, and
 *  <I><c>pointer</c></I> are saved as client-side state.
 *  However, if there is a currently bound vertex buffer object, then <I><c>pointer</c></I>
 *  does not reference a client-side memory location.  Instead, <I><c>pointer</c></I>
 *  is an offset into a buffer object with its data store on the server side.
 *  glBufferData() or glBufferSubData() should have already been called to load the data into the server memory.
 *
 *  If the texture coordinate array is enabled, it is used when glDrawArrays(), glDrawElements(), or glDrawRangeElements()
 *  is called. To enable and disable the texture coordinate array for the client-side active texture unit,
 *  call glEnableClientState() and glDisableClientState() with the argument
 *  <CODE>GL_TEXTURE_COORD_ARRAY</CODE>. The texture coordinate array is initially disabled for all
 *  client-side active texture units and isn&rsquo;t accessed when glDrawArrays(), glDrawElements(), or glDrawRangeElements() is called.
 *
 *  Use glDrawArrays() to construct a sequence of primitives (all of the same type) from prespecified
 *  vertex and vertex attribute arrays. Use glDrawElements() or glDrawRangeElements() to construct a sequence of primitives by
 *  indexing vertices and vertex attributes.
 *
 *  @param size Specifies the number of coordinates per array element. Must be 1, 2, 3 or 4. The initial value is 4.
 *  @param type     Specifies the data type of each texture coordinate in the array.
 * Symbolic constants <CODE>GL_BYTE</CODE>, <CODE>GL_UNSIGNED_BYTE</CODE>, <CODE>GL_SHORT</CODE>, <CODE>GL_FLOAT</CODE>, <CODE>GL_HALF_FLOAT_ARB</CODE>, <CODE>GL_FIXED_11_11_10_SCE</CODE>, and
 *  <CODE>GL_FIXED</CODE> are accepted.  The initial value is <CODE>GL_FLOAT</CODE>.
 *  @param stride   Specifies the byte offset between consecutive array elements. If <I><c>stride</c></I> is 0,
 *  the array elements are understood to be tightly packed. The initial value is 0.
 *  @param pointer  Usually specifies a pointer to the first coordinate of the first element in the array.
 *  If a vertex buffer object is bound with the target <CODE>GL_ARRAY_BUFFER</CODE>, then <I><c>pointer</c></I>
 *  is an offset into a buffer object with its data store on the server side.
 *  The initial value is NULL.
 *
 *  @note
 *  glTexCoordPointer() is typically implemented on the client side, unless there is a current buffer object
 *  bound with the target <CODE>GL_ARRAY_BUFFER</CODE>.
 *  @par
 *  glTexCoordPointer() updates the texture coordinate array state of the client-side active texture unit, specified with glClientActiveTexture().
 *  @par
 *  The format for <CODE>GL_FIXED_11_11_10_SCE</CODE> is 3 signed components packed into 32 bits.
 *  The component placement within the word is:
 *  <TABLE>
 *  <TR>
 *  <TD align="center">component</TD>
 *  <TD align="center">bits</TD>
 *  <TD align="center">shift</TD>
 *  </TR>
 *  <TR>
 *  <TD>0</TD><TD>11</TD><TD>0</TD>
 *  </TR>
 *  <TR>
 *  <TD>1</TD><TD>11</TD><TD>11</TD>
 *  </TR>
 *  <TR>
 *  <TD>2</TD><TD>10</TD><TD>22</TD>
 *  </TR>
 *  </TABLE>
 *  The interpretation of each component in the vertex program is normalized from [-1.0, 1.0].
 *  @par
 *  When <I><c>type</c></I> is set to <CODE>GL_FIXED_11_11_10_SCE</CODE>, <I><c>size</c></I> must be 3.
 *
 *  @par Errors
Use glGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>GL_INVALID_VALUE</CODE></TD>
<TD><I><c>size</c></I> is not 1, 2, 3, or 4, or <I><c>type</c></I> is <CODE>GL_FIXED_11_11_10_SCE</CODE> and <I><c>size</c></I> is not 3.
</TR>
<TR>
<TD><CODE>GL_INVALID_ENUM</CODE></TD>
<TD><I><c>type</c></I> is not an accepted value.</TD>
</TR>
<TR>
<TD><CODE>GL_INVALID_VALUE</CODE></TD>
<TD><I><c>stride</c></I> is negative.</TD>
</TR>
</TABLE>
 *
 * @sa glBufferData, glBufferSubData, glClientActiveTexture, glColorPointer, glDrawArrays, glDrawElements, glDrawRangeElements, glEnable,
 * glMultiTexCoord4f, glNormalPointer, glVertexPointer
 */
GLAPI void APIENTRY glTexCoordPointer( GLint size, GLenum type, GLsizei stride, const GLvoid* pointer )
{
    JS_APIENTRY_TIMER( glTexCoordPointer );
    if ( _jsdpmState )
    {
        _JS_DPM_API_ENTRY_4( glTexCoordPointer, JSDPM_CALL_TYPE_STATE_GEOM, , 4,
                             size, sizeof( GLint ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             type, sizeof( GLenum ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM,
                             stride, sizeof( GLsizei ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             pointer, sizeof( GLvoid * ), INPUT_BY_VALUE, DPM_API_ARGUMENT_POINTER );	// just save the pointer value (not the contents) for deferred evaluation
    }

    PSGLcontext*	LContext = _CurrentContext;
    JS_ASSERT( LContext );

    // argument checking is done in _jsVertexAttribPointerNV.

    // call generic attribute
    _jsVertexAttribPointerNV(
        _JS_ATTRIB_TEX_COORD0_INDEX + LContext->CS_ActiveTexture,
        size,
        type,
        GL_FALSE,
        stride,
        pointer );
}

/**
 * @short Set the current texture coordinates
 *
 *  glMultiTexCoord4f() specifies the four texture coordinates as (<I>s, t, r, q</I>).
 *
 * The current texture coordinates are part of the data that is associated with each vertex.
 * The initial value is (0, 0, 0, 1).
 *
 * @param target   Specifies texture unit whose coordinates should be modified.
 * The number of texture units is implementation dependent, but must be at least one.
 * Must be one of <CODE>GL_TEXTURE</CODE><I>i</I>, where 0 <= <I>i</I> < <I>maxtexunits</I>, where <I>maxtexunits</I>
 * is the maximum of the values returned by using glGetIntegerv() to query <CODE>GL_MAX_TEXTURE_UNITS</CODE>
 * and <CODE>GL_MAX_TEXTURE_COORDS_ARB</CODE>
 *
 * @param s  Specifies the <I>s</I> texture coordinate for <I><c>target</c></I> texture unit.
 * @param t  Specifies the <I>t</I> texture coordinate for <I><c>target</c></I> texture unit.
 * @param r  Specifies the <I>r</I> texture coordinate for <I><c>target</c></I> texture unit.
 * @param q  Specifies the <I>q</I> texture coordinate for <I><c>target</c></I> texture unit.
 *
 *  @note
 *  It is always the case that <CODE>GL_TEXTURE</CODE><I>i</I> = <CODE>GL_TEXTURE0</CODE> + <I>i</I>.
 *  @par
 *  If a custom shader program is written and used, a texture unit consists of either
 *  a texture coordinate set, texture image unit, or both.
 *
 *  @par Errors
Use glGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>GL_INVALID_ENUM</CODE></TD>
<TD><I><c>target</c></I> is not one of <CODE>GL_TEXTURE</CODE><I>i</I>,
 where 0 <= <I>i</I> < k, where k is the maximum of the values returned by <CODE>GL_MAX_TEXTURE_UNITS</CODE>,
 and <CODE>GL_MAX_TEXTURE_COORDS_ARB</CODE>.
 If you are using the fixed-function processing pipeline and you reference
 a texture unit that is beyond <CODE>GL_MAX_TEXTURE_UNITS</CODE>, but less than the overall maximum,
 it does not generate a GL error. However, it will behave as if the texture unit is disabled.</TD>
</TR>
</TABLE>
 *
 *  @sa glActiveTexture, glClientActiveTexture, glColor4f, glGetIntegerv, glNormal3f, glTexCoordPointer
 *
 */

GLAPI void APIENTRY glMultiTexCoord4f( GLenum target, GLfloat s, GLfloat t, GLfloat r, GLfloat q )
{
    JS_APIENTRY_TIMER( glMultiTexCoord4f );
    if ( _jsdpmState )
    {
        _JS_DPM_API_ENTRY_5( glMultiTexCoord4f, JSDPM_CALL_TYPE_STATE_GEOM, , 5,
                             target, sizeof( GLenum ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM,
                             s, sizeof( GLfloat ), INPUT_BY_VALUE, DPM_API_ARGUMENT_FLOAT_VALUE,
                             t, sizeof( GLfloat ), INPUT_BY_VALUE, DPM_API_ARGUMENT_FLOAT_VALUE,
                             r, sizeof( GLfloat ), INPUT_BY_VALUE, DPM_API_ARGUMENT_FLOAT_VALUE,
                             q, sizeof( GLfloat ), INPUT_BY_VALUE, DPM_API_ARGUMENT_FLOAT_VALUE );
    }

#ifndef JS_NO_ERROR_CHECK
    if (( target < GL_TEXTURE0 ) || ( target >= GL_TEXTURE0 + _JS_MAX_TEXTURE_COORDS ) )
    {
        _jsSetError( GL_INVALID_ENUM );
        return;
    }
#endif

    _jsVertexAttrib4fNV( _JS_ATTRIB_TEX_COORD0_INDEX + target - GL_TEXTURE0, s, t, r, q );
}
/**
 *  @short Define an array of colors
 *
 *  glColorPointer() specifies the location and data of an array of color components to use when rendering.
 *  <I><c>size</c></I> specifies the number of components per color, and must be 4.
 *  <I><c>type</c></I> specifies the data type of each color component, and <I><c>stride</c></I> specifies
 *  the byte stride from one color to the next allowing vertices and attributes
 *  to be packed into a single array or stored in separate arrays.
 *  (Single-array storage may be more efficient on some implementations.)
 *
 *  When a color array is specified, <I><c>size</c></I>, <I><c>type</c></I>, <I><c>stride</c></I>, and <I><c>pointer</c></I>
 *  are saved as client-side state.  However, if there is a currently bound vertex buffer object, then <I><c>pointer</c></I>
 *  does not reference a client-side memory location.  Instead, <I><c>pointer</c></I>
 *  is an offset into a buffer object with its data store on the server side.
 *  glBufferData() or glBufferSubData()
 *  should have already been called to load the data into the server memory.
 *
 *  If the color array is enabled, it is used when glDrawArrays(), glDrawElements(), or glDrawRangeElements() is called.
 *  To enable and disable the color array, call glEnableClientState() and glDisableClientState()
 *  with the argument <CODE>GL_COLOR_ARRAY</CODE>. The color array is initially disabled and isn&rsquo;t accessed when
 *  glDrawArrays(), glDrawElements(), or glDrawRangeElements() is called.
 *
 *  Use glDrawArrays() to construct a sequence of primitives (all of the same type) from prespecified vertex
 *  and vertex attribute arrays. Use glDrawElements() or glDrawRangeElements() to construct a sequence of primitives by indexing
 *  vertices and vertex attributes.
 *
 * @param size  Specifies the number of components per color. Must be 1, 2, 3, or 4. The initial value is 4.
 * @param type  Specifies the data type of each color component in the array. Symbolic constants
 * <CODE>GL_BYTE</CODE>, <CODE>GL_UNSIGNED_BYTE</CODE>, <CODE>GL_FLOAT</CODE>, <CODE>GL_HALF_FLOAT_ARB</CODE>, <CODE>GL_FIXED_11_11_10_SCE</CODE>, and
 * <CODE>GL_FIXED</CODE> are accepted.  The initial value is <CODE>GL_FLOAT</CODE>.
 * @param stride  Specifies the byte offset between consecutive colors.
 * If <I><c>stride</c></I> is 0, the colors are understood to be tightly packed in the array. The initial value is 0.
 * @param pointer  Usually specifies a pointer to the first component of the first color element in the array.
 * If a vertex buffer object is bound with the target <CODE>GL_ARRAY_BUFFER</CODE>, then <I><c>pointer</c></I>
 * is an offset into a buffer object with its data store on the server side.
 *
 *  @note
 *  glColorPointer() is typically implemented on the client side, unless there is a current buffer object
 *  bound with the target <CODE>GL_ARRAY_BUFFER</CODE>.
 *  @par
 *  The format for <CODE>GL_FIXED_11_11_10_SCE</CODE> is 3 signed components packed into 32 bits.
 *  The component placement within the word is: @par
 *  <TABLE>
 *  <TR>
 *  <TD align="center">Component #</TD>
 *  <TD align="center">Bits</TD>
 *  <TD align="center">Shift</TD>
 *  </TR>
 *  <TR>
 *  <TD>0</TD><TD>11</TD><TD>0</TD>
 *  </TR>
 *  <TR>
 *  <TD>1</TD><TD>11</TD><TD>11</TD>
 *  </TR>
 *  <TR>
 *  <TD>2</TD><TD>10</TD><TD>22</TD>
 *  </TR>
 *  </TABLE>
 *  The interpretation of each component in the vertex program is normalized from [-1.0, 1.0].
 *  @par
 *  When <I><c>type</c></I> is set to <CODE>GL_FIXED_11_11_10_SCE</CODE>, <I><c>size</c></I> must be 3.
 *
 *  @par Errors
Use glGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>GL_INVALID_VALUE</CODE></TD>
<TD><I><c>size</c></I> is not 1, 2, 3, or 4, or <I><c>type</c></I> is <CODE>GL_FIXED_11_11_10_SCE</CODE> and <I><c>size</c></I> is not 3.
</TR>
<TR>
<TD><CODE>GL_INVALID_ENUM</CODE></TD>
<TD><I><c>type</c></I> is not an accepted value.</TD>
</TR>
<TR>
<TD><CODE>GL_INVALID_VALUE</CODE></TD>
<TD><I><c>stride</c></I> is negative.</TD>
</TR>
</TABLE>
 *
 *  @sa glBufferData, glBufferSubData, glColor4f, glDrawArrays, glDrawElements, glDrawRangeElements, glEnableClientState,
 *      glNormalPointer, glTexCoordPointer, glVertexPointer
 */
GLAPI void APIENTRY glColorPointer( GLint size, GLenum type, GLsizei stride, const GLvoid* pointer )
{
    JS_APIENTRY_TIMER( glColorPointer );
    if ( _jsdpmState )
    {
        _JS_DPM_API_ENTRY_4( glColorPointer, JSDPM_CALL_TYPE_STATE_GEOM, , 4,
                             size, sizeof( GLint ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             type, sizeof( GLenum ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM,
                             stride, sizeof( GLsizei ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             pointer, sizeof( GLvoid * ), INPUT_BY_VALUE, DPM_API_ARGUMENT_POINTER );	// just save the pointer value (not the contents) for deferred evaluation
    }

    // argument checking is done in _jsVertexAttribPointerNV.

    // call generic attribute
    _jsVertexAttribPointerNV( _JS_ATTRIB_PRIMARY_COLOR_INDEX, size, type, GL_TRUE, stride, pointer );
}

/**
 *  @short Set the current color
 *
 *  This function sets a new four-valued RGBA color as the current color.
 *
 *  PSGL stores a current four-valued RGBA color. The initial value is (1.0, 1.0, 1.0, 1.0).
 *
 *  The current color values are stored as specified, but are clamped to the range [0.0, 1.0] before
 *  they are written into the color buffer.
 *
 *  @param red  New red value (0.0f to 1.0f)
 *  @param green  New green value (0.0f to 1.0f)
 *  @param blue  New blue value (0.0f to 1.0f)
 *  @param alpha New alpha value (0.0f to 1.0f)
 *
 *  @sa glColorPointer, glMultiTexCoord4f, glNormal3f
 */

GLAPI void APIENTRY glColor4f( GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha )
{
    JS_APIENTRY_TIMER( glColor4f );
    if ( _jsdpmState )
    {
        _JS_DPM_API_ENTRY_4( glColor4f, JSDPM_CALL_TYPE_STATE_GEOM, , 4,
                             red, sizeof( GLfloat ), INPUT_BY_VALUE, DPM_API_ARGUMENT_FLOAT_VALUE,
                             green, sizeof( GLfloat ), INPUT_BY_VALUE, DPM_API_ARGUMENT_FLOAT_VALUE,
                             blue, sizeof( GLfloat ), INPUT_BY_VALUE, DPM_API_ARGUMENT_FLOAT_VALUE,
                             alpha, sizeof( GLfloat ), INPUT_BY_VALUE, DPM_API_ARGUMENT_FLOAT_VALUE );
    }

    _jsVertexAttrib4fNV( _JS_ATTRIB_PRIMARY_COLOR_INDEX, red, green, blue, alpha );
}

/**
 *  @short Set the current color
 *
 *  This function sets a new four-valued RGBA color as the current color.  It differs from glColor4f() only in
 *  argument type. The initial value is (255, 255, 255, 255).
 *
 *  The current color values are stored as specified, but are clamped to the range [0, 255] before
 *  they are written into the color buffer.
 *
 *  @param red  New red value (0 to 255)
 *  @param green  New green value (0 to 255)
 *  @param blue  New blue value (0 to 255)
 *  @param alpha New alpha value (0 to 255)
 *
 */

GLAPI void APIENTRY glColor4ub( GLubyte red, GLubyte green, GLubyte blue, GLubyte alpha )
{
    JS_APIENTRY_TIMER( glColor4ub );
    if ( _jsdpmState )
    {
        _JS_DPM_API_ENTRY_4( glColor4ub, JSDPM_CALL_TYPE_STATE_GEOM, , 4,
                             red, sizeof( GLubyte ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             green, sizeof( GLubyte ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             blue, sizeof( GLubyte ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             alpha, sizeof( GLubyte ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE );
    }

    const float f = 1.f / 255.f;
    _jsVertexAttrib4fNV( _JS_ATTRIB_PRIMARY_COLOR_INDEX, f*red, f*green, f*blue, f*alpha );
}

/**
 *  @short Set the current color
 *
 *  This function sets a new four-valued RGBA color as the current color.  It differs from glColor4f() only in
 *  argument type. For more details, see glColor4f().
 *
 *  PSGL stores a current four-valued RGBA color. The initial value is (1.0, 1.0, 1.0, 1.0).
 *
 *  The current color values are stored as specified, but are clamped to the range [0.0, 1.0] before
 *  they are written into the color buffer.
 *
 *  @param v Pointer to an array with new red, green, blue, and alpha values (0.0f to 1.0f)
 *
 */

GLAPI void APIENTRY glColor4fv( const GLfloat *v )
{
    JS_APIENTRY_TIMER( glColor4fv );
    if ( _jsdpmState ) { _JS_DPM_API_ENTRY_1( glColor4fv, JSDPM_CALL_TYPE_STATE_GEOM, , 1, v, 4*sizeof( GLfloat ), INPUT_BY_REFERENCE, DPM_API_ARGUMENT_POINTER ); }

    _jsVertexAttrib4fvNV( _JS_ATTRIB_PRIMARY_COLOR_INDEX, v );
}

/** @} Geometry */

// set vertex program attribute array
// XXX Should we expose this as API?
// NOTE: The ARB version of this function provides a normalized boolean
// argument that indicates how integer data is to be interpreted, e.g.
// scaled to [0,1] or [-1,1].  If we allow non-float inputs (which
// currently don't appear to work for other routines such as
// glVertexPointer) then we may want to consider a similar mechanism.
void _jsVertexAttribPointerNV(
    GLuint index,			// attribute array index
    GLint fsize,			// number of components 1-4
    GLenum type,			// {GL_SHORT, GL_INT, GL_FLOAT}
    GLboolean normalized,
    GLsizei stride,			// element-to-element distance in bytes
    const GLvoid* pointer )	// pointer to data
{
    PSGLcontext*	LContext = _CurrentContext;
    JS_ASSERT( LContext );
#ifndef JS_NO_ERROR_CHECK
    if ( stride < 0 )
    {
        _jsSetError( GL_INVALID_VALUE );
        return;
    }
    if (( fsize < 1 ) || ( fsize > 4 ) )
    {
        _jsSetError( GL_INVALID_VALUE );
        return;
    }
#endif

    GLsizei defaultStride = 0;
    switch ( type )
    {
        case GL_FLOAT:
        case GL_HALF_FLOAT_ARB:
        case GL_BYTE:
        case GL_UNSIGNED_BYTE:
        case GL_SHORT:
        case GL_FIXED:
            defaultStride = fsize * _jsGetTypeSize( type );
            break;
        case GL_FIXED_11_11_10_SCE:
#ifndef JS_NO_ERROR_CHECK
            if ( fsize != 3 )
            {
                _jsSetError( GL_INVALID_VALUE );
                return;
            }
#endif
            defaultStride = 4;
            break;
        default:
            _jsSetError( GL_INVALID_ENUM );
            return;
    }

    jsAttributeState* as = LContext->attribs;
    jsAttribute* attrib = as->attrib + index;
    attrib->clientSize = fsize;
    attrib->clientType = type;
    attrib->clientStride = stride ? stride : defaultStride;
    attrib->clientData = ( void* )pointer;
    GLuint oldArrayBuffer = attrib->arrayBuffer;
    attrib->arrayBuffer = LContext->ArrayBuffer;
    attrib->normalized = normalized;
    JSBIT_ASSIGN( as->HasVBOMask, index, attrib->arrayBuffer != 0 );
    GLboolean needConvert = _jsPlatformNeedsConversion( as, index );
    JSBIT_ASSIGN( as->NeedsConversionMask, index, needConvert );

    JSBIT_TRUE( as->DirtyMask, index );
    JS_ASSERT( _jsCheckAttribCaches( as ) );

    if ( LContext->attribSetName )
    {
        jsAttribSet* attribSet = _jsGetAttribSet( LContext->attribSetName );
        JS_ASSERT( attribSet );

        // if only one reference to the previous BufferObject, remove the AttribSet from the list.
        if ( oldArrayBuffer )
        {
            int refcount = 0;
            for ( unsigned int i = 0; i < _JS_MAX_VERTEX_ATTRIBS; ++i )
            {
                if ( attribSet->attribs.attrib[i].arrayBuffer == oldArrayBuffer ) ++refcount;
            }
            if ( refcount == 1 ) _jsGetBufferObject( LContext, oldArrayBuffer )->attribSets.removeElement( attribSet );
        }

        // if new reference to a BufferObject, add AttribSet to the list.
        if ( attrib->arrayBuffer )
        {
            _jsGetBufferObject( LContext, attrib->arrayBuffer )->attribSets.appendUnique( attribSet );
        }

        _jsAttribSetMarkDirty();
    }
}

void _jsVertexAttribElementFunc( GLuint index, GLenum func, GLuint frequency )
{
    PSGLcontext*	LContext = _CurrentContext;
    JS_ASSERT( LContext );
    JS_ASSERT( index < _JS_MAX_VERTEX_ATTRIBS );
#ifndef JS_NO_ERROR_CHECK
    if ( frequency == 0 )
    {
        _jsSetError( GL_INVALID_VALUE );
        return;
    }
#endif

    jsAttributeState* as = LContext->attribs;
    jsAttribute* attrib = as->attrib + index;

    switch ( func )
    {
        case GL_MODULO_SCE:
            JSBIT_TRUE( as->ModuloMask, index );
            break;
        case GL_DIVIDE_SCE:
            JSBIT_FALSE( as->ModuloMask, index );
            break;
        default:
            _jsSetError( GL_INVALID_ENUM );
            return;
    }
    attrib->frequency = frequency;

    GLboolean needConvert = _jsPlatformNeedsConversion( as, index );
    JSBIT_ASSIGN( as->NeedsConversionMask, index, needConvert );
    JSBIT_TRUE( as->DirtyMask, index );
    JS_ASSERT( _jsCheckAttribCaches( as ) );

    if ( LContext->attribSetName ) _jsAttribSetMarkDirty();
}

void _jsEnableVertexAttribArrayNV( GLuint index )
{
    JS_ASSERT( index >= 0 && index < _JS_MAX_VERTEX_ATTRIBS );
    PSGLcontext *LContext = _CurrentContext;
    JS_ASSERT( LContext );

    JSBIT_TRUE( LContext->attribs->EnabledMask, index );
    JSBIT_TRUE( LContext->attribs->DirtyMask, index );
    JS_ASSERT( _jsCheckAttribCaches( LContext->attribs ) );

    if ( LContext->attribSetName ) _jsAttribSetMarkDirty();
}

void _jsDisableVertexAttribArrayNV( GLuint index )
{
    JS_ASSERT( index >= 0 && index < _JS_MAX_VERTEX_ATTRIBS );
    PSGLcontext *LContext = _CurrentContext;
    JS_ASSERT( LContext );

    JSBIT_FALSE( LContext->attribs->EnabledMask, index );
    JSBIT_TRUE( LContext->attribs->DirtyMask, index );
    JS_ASSERT( _jsCheckAttribCaches( LContext->attribs ) );

    if ( LContext->attribSetName ) _jsAttribSetMarkDirty();
}

void _jsVertexAttrib1fNV( GLuint index, GLfloat x )
{
    JS_ASSERT( index < _JS_MAX_VERTEX_ATTRIBS );

    PSGLcontext*	LContext = _CurrentContext;
    JS_ASSERT( LContext );

    jsAttribute* attrib = LContext->attribs->attrib + index;
    attrib->value[0] = x;
    attrib->value[1] = 0.0f;
    attrib->value[2] = 0.0f;
    attrib->value[3] = 1.0f;
    JSBIT_TRUE( LContext->attribs->DirtyMask, index );

    if ( LContext->attribSetName ) _jsAttribSetMarkDirty();
}

void _jsVertexAttrib2fNV( GLuint index, GLfloat x, GLfloat y )
{
    JS_ASSERT( index < _JS_MAX_VERTEX_ATTRIBS );

    PSGLcontext*	LContext = _CurrentContext;
    JS_ASSERT( LContext );

    jsAttribute* attrib = LContext->attribs->attrib + index;
    attrib->value[0] = x;
    attrib->value[1] = y;
    attrib->value[2] = 0.0f;
    attrib->value[3] = 1.0f;
    JSBIT_TRUE( LContext->attribs->DirtyMask, index );

    if ( LContext->attribSetName ) _jsAttribSetMarkDirty();
}

void _jsVertexAttrib3fNV( GLuint index, GLfloat x, GLfloat y, GLfloat z )
{
    JS_ASSERT( index < _JS_MAX_VERTEX_ATTRIBS );

    PSGLcontext*	LContext = _CurrentContext;
    JS_ASSERT( LContext );

    jsAttribute* attrib = LContext->attribs->attrib + index;
    attrib->value[0] = x;
    attrib->value[1] = y;
    attrib->value[2] = z;
    attrib->value[3] = 1.0f;
    JSBIT_TRUE( LContext->attribs->DirtyMask, index );

    if ( LContext->attribSetName ) _jsAttribSetMarkDirty();
}

void _jsVertexAttrib4fNV( GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w )
{
    JS_ASSERT( index < _JS_MAX_VERTEX_ATTRIBS );

    PSGLcontext*	LContext = _CurrentContext;
    JS_ASSERT( LContext );

    jsAttribute* attrib = LContext->attribs->attrib + index;
    attrib->value[0] = x;
    attrib->value[1] = y;
    attrib->value[2] = z;
    attrib->value[3] = w;
    JSBIT_TRUE( LContext->attribs->DirtyMask, index );

    if ( LContext->attribSetName ) _jsAttribSetMarkDirty();
}

void _jsVertexAttrib1fvNV( GLuint index, const GLfloat* v )
{
    _jsVertexAttrib1fNV( index, v[0] );
}

void _jsVertexAttrib2fvNV( GLuint index, const GLfloat* v )
{
    _jsVertexAttrib2fNV( index, v[0], v[1] );
}

void _jsVertexAttrib3fvNV( GLuint index, const GLfloat* v )
{
    _jsVertexAttrib3fNV( index, v[0], v[1], v[2] );
}

void _jsVertexAttrib4fvNV( GLuint index, const GLfloat* v )
{
    _jsVertexAttrib4fNV( index, v[0], v[1], v[2], v[3] );
}




/* DPM support for performance measurements */
#ifdef JS_DPM_V2
void psglDrawMiddle()
{
    _JS_DPM_PSEUDO_ENTRY_0( psglDrawMiddle, JSDPM_CALL_TYPE_DRAW, );
}

void psglDrawAfter()
{
    _JS_DPM_PSEUDO_ENTRY_0( psglDrawAfter, JSDPM_CALL_TYPE_DRAW, );
}

#endif

int _psglCheckDrawElementsVBOBounds = 0;

#ifdef _JS_CHECK_FOR_DRAW_OUT_OF_VBO_BOUNDS
void _jsCheckForIndicesOutOfIBOBounds( const void *indices, GLenum type, unsigned int count )
{
    PSGLcontext*	LContext = _CurrentContext;
    JS_ASSERT( LContext );
    int ibo = LContext->attribs->ElementArrayBuffer;
    if ( !ibo ) return;
    jsBufferObject* bufferObject = _jsGetBufferObject( LContext, ibo );

    unsigned int typeSize = _jsGetTypeSize( type );
    unsigned long offset = ( unsigned long )( indices );
    unsigned long data = offset + typeSize * count;
    if ( data > ( unsigned long )bufferObject->size )
    {
        // out of bounds
        _JS_REPORT_EXTRA( PSGL_REPORT_BUFFER_OBJECT_OUT_OF_BOUNDS, "Drawing using index data out of buffer object bounds: buffer object %d, size=%d, offset=%d", ibo, bufferObject->size, data );
    }
}

void _jsCheckForDrawOutOfVBOBounds( unsigned int maxIndex )
{
    PSGLcontext*	LContext = _CurrentContext;
    JS_ASSERT( LContext );
    int outOfBounds = 0;
    for ( int i = 0; i < _JS_MAX_VERTEX_ATTRIBS; ++i )
    {
        if ( ! JSBIT_GET( LContext->attribs->EnabledMask, i ) ) continue;

        jsAttribute *attrib = LContext->attribs->attrib + i;
        int vbo = attrib->arrayBuffer;
        if ( !vbo ) continue;
        jsBufferObject* bufferObject = _jsGetBufferObject( LContext, vbo );
        unsigned long offset = ( unsigned long )( attrib->clientData );
        GLsizei stride = attrib->clientStride;
        unsigned int count = maxIndex;

        if ( JSBIT_GET( LContext->attribs->ModuloMask, i ) )
            count = count > ( attrib->frequency - 1 ) ? ( attrib->frequency - 1 ) : count;
        else
            count /= attrib->frequency;

        unsigned int dataSize;
        if ( JS_UNLIKELY( attrib->clientType == GL_FIXED_11_11_10_SCE ) )
        {
            dataSize = 4;
        }
        else
        {
            dataSize = attrib->clientSize * _jsGetTypeSize( attrib->clientType );
        }
        unsigned long data = stride * count + dataSize + offset;
        if ( data > ( unsigned long )bufferObject->size )
        {
            // out of bounds
            _JS_REPORT_EXTRA( PSGL_REPORT_BUFFER_OBJECT_OUT_OF_BOUNDS, "Drawing using data out of buffer object bounds: attribute %d, buffer object %d, size=%d, offset=%d", i, vbo, bufferObject->size, data );
            outOfBounds = 1;
        }
    }
}
#endif

/** @addtogroup OpenGL
 *
 * @{
 */

/**
 * @short Render primitives from array data
 *
 *  glDrawElements() specifies multiple geometric primitives with very few subroutine calls.
 *  You can prespecify separate arrays of vertices, normals, colors, and texture coordinates and
 *  use them to construct a sequence of primitives with a single call to glDrawElements().
 *
 *  When glDrawElements() is called, it uses <I><c>count</c></I> sequential indices from <I><c>indices</c></I> to lookup
 *  elements in enabled arrays to construct a sequence of geometric primitives.
 *  (If there is a currently bound vertex buffer object, then <I><c>indices</c></I>
 *  may be an offset into a buffer object with data previously loaded by glBufferData() or glBufferSubData().)
 *  <I><c>mode</c></I> specifies what kind of primitives are constructed, and how the array elements construct these primitives. If
 *  <CODE>GL_VERTEX_ARRAY</CODE> is not enabled, no geometric primitives are constructed.
 *
 *  If <CODE>GL_PRIMITIVE_RESTART_NV</CODE> is enabled, then glPrimitiveRestartIndexNV()
 *  describes a special array index that divides the typical sequence of geometric primitives.
 *  Every time the special array index is encountered in <I><c>indices</c></I>, the index marks the end of
 *  the previous primitive and the start of a new primitive.  The separation is most apparent
 *  with <CODE>GL_TRIANGLE_STRIP</CODE>, <CODE>GL_TRIANGLE_FAN</CODE>, and <CODE>GL_QUAD_STRIP</CODE> primitives.
 *  For example, with a <CODE>GL_TRIANGLE_FAN</CODE>, restarting the primitive changes the
 *  anchor point, which establishes a distinct triangle fan.
 *
 *  Vertex attributes that are modified by glDrawElements() have an unspecified value after
 *  glDrawElements() returns. For example, if <CODE>GL_COLOR_ARRAY</CODE> is enabled, the value
 *  of the current color is undefined after glDrawElements() executes. Attributes that aren&rsquo;t
 *  modified maintain their previous values.
 *
 * @param mode     Specifies what kind of primitives to render. Symbolic constants
 * <CODE>GL_POINTS</CODE>, <CODE>GL_LINE_STRIP</CODE>, <CODE>GL_LINE_LOOP</CODE>, <CODE>GL_LINES</CODE>,
 * <CODE>GL_TRIANGLE_STRIP</CODE>, <CODE>GL_TRIANGLE_FAN</CODE>, <CODE>GL_TRIANGLES</CODE>,
 * <CODE>GL_QUAD_STRIP</CODE>, and <CODE>GL_QUADS</CODE> are accepted.
 * @param count    Specifies the number of elements to be rendered.
 * @param type     Specifies the type of the values in <I><c>indices</c></I>.
 * Must be one of <CODE>GL_UNSIGNED_BYTE</CODE>, <CODE>GL_UNSIGNED_SHORT</CODE>, or <CODE>GL_UNSIGNED_INT</CODE>.
 * @param indices  Specifies a pointer to the location where the indices are stored.
 * If a vertex buffer object is bound with the target <CODE>GL_ELEMENT_ARRAY_BUFFER</CODE>, then <I><c>indices</c></I>
 * is an offset into a buffer object with its data store on the server side.
 *
 *  @par Errors
Use glGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>GL_INVALID_ENUM</CODE></TD>
<TD><I><c>mode</c></I> is not an accepted value.</TD>
</TR>
<TR>
<TD><CODE>GL_INVALID_ENUM</CODE></TD>
<TD><I><c>type</c></I> is not an accepted value.</TD>
</TR>
<TR>
<TD><CODE>GL_INVALID_VALUE</CODE></TD>
<TD><I><c>count</c></I> is negative.</TD>
</TR>
<TR>
<TD><CODE>GL_INVALID_FRAMEBUFFER_OPERATION_OES</CODE></TD>
<TD>The bound frame buffer does not meet one or more of the following conditions:
<UL>
<LI>Textures are complete.</LI>
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
 * @sa glBufferData, glBufferSubData, glClientActiveTexture, glColorPointer, glDrawArrays, glDrawRangeElements, glNormalPointer, glTexCoordPointer, glVertexPointer, glPrimitiveRestartIndexNV
 */

GLAPI void APIENTRY glDrawElements( GLenum mode, GLsizei count, GLenum type, const GLvoid* indices )
{
#ifdef JS_DPM
    { /* this makes sure that the following timer goes out of
               scope befor the LeaveDrawCall gets called */
#endif
        JS_APIENTRY_TIMER_SKIP_OVERALL( glDrawElements );
#ifdef JS_DPM
        GLuint minIndex = 0;
        GLuint maxIndex = 0;

#define _JS_DPM_PARAMETER_COUNT	4
        if ( _jsdpmState )
        {
            PSGLcontext*	LContext = _CurrentContext;
            JS_ASSERT( LContext );

            if (( LContext->attribs->ElementArrayBuffer ) || ( _JS_DPM_STATE( CAPTURE ) && indices == NULL ) )	// if capturing an indices array that lives on the server
            {
                _JS_DPM_API_ENTRY_4( glDrawElements, JSDPM_CALL_TYPE_DRAW, , _JS_DPM_PARAMETER_COUNT + _JS_MAX_VERTEX_ATTRIBS + 1,	// allocate phantom arguments to hold evaluated arrays
                                     mode, sizeof( GLenum ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM,
                                     count, sizeof( GLsizei ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                                     type, sizeof( GLenum ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM,
                                     indices, sizeof( GLvoid * ), INPUT_BY_VALUE, DPM_API_ARGUMENT_POINTER );	// store this as a 32-bit (integer) value for later replay
            }
            else	// if the indices array lives on the client
            {
#define TYPESIZE    ((type == GL_UNSIGNED_BYTE) ? sizeof(GLubyte) : type == GL_UNSIGNED_SHORT ? sizeof(GLushort) : sizeof(GLuint))
                _JS_DPM_API_ENTRY_4( glDrawElements, JSDPM_CALL_TYPE_DRAW, , _JS_DPM_PARAMETER_COUNT + _JS_MAX_VERTEX_ATTRIBS + 1,	// allocate phantom arguments to hold evaluated arrays
                                     mode, sizeof( GLenum ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM,
                                     count, sizeof( GLsizei ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                                     type, sizeof( GLenum ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM,
                                     indices, count*TYPESIZE, INPUT_BY_REFERENCE, DPM_API_ARGUMENT_POINTER );	// store this as a pointer to array data for use in capture and replay
#undef TYPESIZE
            }
        }
#ifndef JS_DPM_V2
        _jsDPMSubmitQueries();
#endif
#endif

        _JS_PERFORMANCE_START( DrawElementsCounter );
        PSGLcontext*	LContext = _CurrentContext;
        JS_ASSERT( LContext );

#ifndef JS_NO_ERROR_CHECK
        switch ( mode )
        {
            case GL_POINTS:
            case GL_LINES:
            case GL_LINE_STRIP:
            case GL_LINE_LOOP:
            case GL_TRIANGLES:
            case GL_TRIANGLE_STRIP:
            case GL_TRIANGLE_FAN:
            case GL_QUADS:
            case GL_QUAD_STRIP:
                break;
            default:
                _jsSetError( GL_INVALID_ENUM );
                return;
        }

        switch ( type )
        {
            case GL_UNSIGNED_BYTE:
            case GL_UNSIGNED_SHORT:
            case GL_UNSIGNED_INT: // JetStream extention. really needed ?
                break;
            default:
                _jsSetError( GL_INVALID_ENUM );
                return;
        }
#endif

        if ( JS_UNLIKELY( count <= 0 ) )
        {
#ifndef JS_NO_ERROR_CHECK
            if ( count < 0 ) _jsSetError( GL_INVALID_VALUE );
#endif
            return;
        }

        if ( JS_UNLIKELY( ! JSBIT_GET( LContext->attribs->EnabledMask, _JS_ATTRIB_POSITION_INDEX ) ) ) return;

		// [YLIN] use dcbz to zero init dparams
		uint32_t _tmp_clear_loop = c_rounded_size_of_jsDrawParams>>7;
		do{
			--_tmp_clear_loop;
			__dcbz(s_dparams_buff+(_tmp_clear_loop<<7));
		}while(_tmp_clear_loop);
		jsDrawParams *dparams = (jsDrawParams *)s_dparams_buff;
		dparams->mode = mode;
		dparams->indexCount = count;
		dparams->indexType = type;
		dparams->indices = indices;

#ifdef JS_DPM
        // Capture the maximum index value and store it for replay
        if ( _JS_DPM_STATE( CAPTURE ) )
        {
            _jsComputeMinMaxIndices( LContext, &minIndex, &maxIndex, indices, type, count );
#ifdef JS_DPM_V2
            _JS_DPM_API_STORE_ARGUMENT(( _JS_DPM_PARAMETER_COUNT + 1 ), maxIndex, sizeof( maxIndex ), INPUT_BY_VALUE, ( DPMAPIArgumentType )0 );
#else
            _JS_DPM_API_CAPTURE_SINGLETON(( _JS_DPM_PARAMETER_COUNT + 1 ), maxIndex, sizeof( maxIndex ), INPUT_BY_VALUE, 0 );
#endif
            int i;
            for ( i = 0; i < _JS_MAX_VERTEX_ATTRIBS; ++i )
                _jsdpmCaptureDeferredArray( LContext, _JS_ATTRIB_POSITION_INDEX + i, _JS_DPM_PARAMETER_COUNT + 2 + i, maxIndex + 1 );
        }
        // Replay the maximum index value
        else if ( _JS_DPM_STATE( REPLAY ) )
        {
            _JS_DPM_API_REPLAY_SINGLETON( _JS_DPM_PARAMETER_COUNT + 1, maxIndex, sizeof( maxIndex ), INPUT_BY_VALUE );
            int i;
            for ( i = 0; i < _JS_MAX_VERTEX_ATTRIBS; ++i )
            {
                _jsdpmReplayDeferredArray( LContext, _JS_ATTRIB_POSITION_INDEX + i, _JS_DPM_PARAMETER_COUNT + 2 + i );
            }	// for
        }
#undef _JS_DPM_PARAMETER_COUNT
#endif


#ifdef _JS_CHECK_FOR_DRAW_OUT_OF_VBO_BOUNDS
        _jsCheckForIndicesOutOfIBOBounds( indices, type, count );
        if ( _psglCheckDrawElementsVBOBounds )
        {
            GLuint min = 0, max = 0;
            _jsComputeMinMaxIndices( LContext, &min, &max, indices, type, count );
            _jsCheckForDrawOutOfVBOBounds( max );

        }
#endif


        if ( LContext->needValidate ) _jsValidateStates( PSGL_VALIDATE_ALL );

        // determine if this draw must be segmented
		GLboolean slowPath = _jsPlatformRequiresSlowPath( dparams, type, count);


#ifdef JS_DPM
        psglDrawMiddle();
#endif


        if ( JS_UNLIKELY( slowPath ) )
        {
			_jsDrawSlow( dparams );
        }
        else
        {
			_jsPlatformDraw( dparams );
        }

        _JS_PERFORMANCE_STOP( DrawElementsCounter );

#ifdef JS_DPM
    } /* this makes sure that the following timer goes out of
           scope befor the LeaveDrawCall gets called */
#endif

#ifdef JS_DPM
    psglDrawAfter();
#endif
}

/**
 * @short Render primitives from array data
 *
 *  glDrawRangeElements() is a restricted form of glDrawElements(). <I><c>mode</c></I>, <I><c>start</c></I>,
<I><c>end</c></I>, and <I><c>count</c></I> match the corresponding arguments to glDrawElements(), with the additional constraint that all values in the arrays
<I><c>indices</c></I> must lie between <I><c>start</c></I> and <I><c>end</c></I>, inclusive.

Implementations denote recommended maximum amounts of vertex and
index data, which may be queried by calling glGetIntegerv() with argument
<CODE>GL_MAX_ELEMENTS_VERTICES</CODE> and <CODE>GL_MAX_ELEMENTS_INDICES</CODE>. If
<I><c>end</c></I> - <I><c>start</c></I> + 1
is greater than the value of <CODE>GL_MAX_ELEMENTS_VERTICES</CODE>, or if
<I><c>count</c></I> is greater than the value of <CODE>GL_MAX_ELEMENTS_INDICES</CODE>, then the call
may operate at reduced performance. There is no requirement that all vertices in the range
[<I><c>start</c></I>,<I><c>end</c></I>] be referenced. However, the implementation may partially
process unused vertices, reducing performance from what could be achieved with an optimal index set.

When glDrawRangeElements() is called, it uses <I><c>count</c></I> sequential elements from
an enabled array, starting at <I><c>start</c></I> to construct a sequence of geometric primitives.
(If there is a currently bound vertex buffer object, then <I><c>indices</c></I>
may be an offset into a buffer object with data previously loaded by glBufferData() or glBufferSubData().)
<I><c>mode</c></I> specifies what kind of primitives are constructed, and how the array elements construct
these primitives. If more than one array is enabled, each is used. If <CODE>GL_VERTEX_ARRAY</CODE> is not
enabled, no geometric primitives are constructed.

 *  If <CODE>GL_PRIMITIVE_RESTART_NV</CODE> is enabled, then glPrimitiveRestartIndexNV()
 *  describes a special array index that divides the typical sequence of geometric primitives.
 *  Every time the special array index is encountered in <I><c>indices</c></I>, the index marks the end of
 *  the previous primitive and the start of a new primitive.  The separation is most apparent
 *  with <CODE>GL_TRIANGLE_STRIP</CODE>, <CODE>GL_TRIANGLE_FAN</CODE>, and <CODE>GL_QUAD_STRIP</CODE> primitives.
 *  For example, with a <CODE>GL_TRIANGLE_FAN</CODE>, restarting the primitive changes the
 *  anchor point, which establishes a distinct triangle fan.

Vertex attributes that are modified by glDrawRangeElements() have an unspecified value after
glDrawRangeElements() returns. For example, if <CODE>GL_COLOR_ARRAY</CODE> is enabled, the
value of the current color is undefined after glDrawRangeElements() executes. Attributes that aren&rsquo;t
modified maintain their previous values.

 * @param mode     Specifies what kind of primitives to render. Symbolic constants
 * <CODE>GL_POINTS</CODE>, <CODE>GL_LINE_STRIP</CODE>, <CODE>GL_LINE_LOOP</CODE>, <CODE>GL_LINES</CODE>,
 * <CODE>GL_TRIANGLE_STRIP</CODE>, <CODE>GL_TRIANGLE_FAN</CODE>, <CODE>GL_TRIANGLES</CODE>,
 * <CODE>GL_QUAD_STRIP</CODE>, and <CODE>GL_QUADS</CODE> are accepted.
 * @param start    Specifies the minimum array index contained in <I><c>indices</c></I>.
 * @param end      Specifies the maximum array index contained in <I><c>indices</c></I>.
 * @param count    Specifies the number of elements to be rendered.
 * @param type     Specifies the type of the values in <I><c>indices</c></I>.
 * Must be one of <CODE>GL_UNSIGNED_BYTE</CODE>, <CODE>GL_UNSIGNED_SHORT</CODE>, or <CODE>GL_UNSIGNED_INT</CODE>.
 * @param indices  Specifies a pointer to the location where the indices are stored.
 * If a vertex buffer object is bound with the target <CODE>GL_ELEMENT_ARRAY_BUFFER</CODE>, then <I><c>indices</c></I>
 * is an offset into a buffer object with its data store on the server side.
 *
 *  @par Errors
Use glGetError() to retrieve the value of the error flag.
It is an error for indices to lie outside the range [start,end], but
implementations may not check for this situation. Such indices cause implementation-dependent behavior.
<TABLE>
<TR>
<TD><CODE>GL_INVALID_ENUM</CODE></TD>
<TD><I><c>mode</c></I> is not an accepted value.</TD>
</TR>
<TR>
<TD><CODE>GL_INVALID_VALUE</CODE></TD>
<TD><I><c>count</c></I> is negative.</TD>
</TR>
<TR>
<TD><CODE>GL_INVALID_VALUE</CODE></TD>
<TD><I><c>end</c></I> < <I><c>start</c></I>.</TD>
</TR>
<TR>
<TD><CODE>GL_INVALID_FRAMEBUFFER_OPERATION_OES</CODE></TD>
<TD>The bound frame buffer does not meet one or more of the following conditions:
<UL>
<LI>Textures are complete.</LI>
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

 * @sa glBufferData, glBufferSubData, glColorPointer, glDrawArrays, glDrawElements, glGetIntegerv, glNormalPointer, glTexCoordPointer, glVertexPointer, glPrimitiveRestartIndexNV
 */

GLAPI void APIENTRY glDrawRangeElements( GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const GLvoid *indices )
{
#ifdef JS_DPM
    { /* this makes sure that the following timer goes out of
               scope befor the LeaveDrawCall gets called */
#endif
        JS_APIENTRY_TIMER_SKIP_OVERALL( glDrawRangeElements );
#ifdef JS_DPM
        if ( _jsdpmState )
        {
            PSGLcontext*	LContext = _CurrentContext;
            JS_ASSERT( LContext );
            const int parameterCount = 6;
            if (( LContext->attribs->ElementArrayBuffer ) || ( _JS_DPM_STATE( CAPTURE ) && indices == NULL ) )	// if capturing an indices array that lives on the server
            {
                _JS_DPM_API_ENTRY_6( glDrawRangeElements, JSDPM_CALL_TYPE_DRAW, , parameterCount + _JS_MAX_VERTEX_ATTRIBS,
                                     mode, sizeof( GLenum ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM,
                                     start, sizeof( GLuint ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                                     end, sizeof( GLuint ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                                     count, sizeof( GLsizei ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                                     type, sizeof( GLenum ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM,
                                     indices, sizeof( GLvoid * ), INPUT_BY_VALUE, DPM_API_ARGUMENT_POINTER );	// store this as a 32-bit (integer) value for later replay
            }
            else	// if the indices array lives on the client
            {
#define TYPESIZE    ((type == GL_UNSIGNED_BYTE) ? sizeof(GLubyte) : type == GL_UNSIGNED_SHORT ? sizeof(GLushort) : sizeof(GLuint))
                _JS_DPM_API_ENTRY_6( glDrawRangeElements, JSDPM_CALL_TYPE_DRAW, , parameterCount + _JS_MAX_VERTEX_ATTRIBS,
                                     mode, sizeof( GLenum ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM,
                                     start, sizeof( GLuint ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                                     end, sizeof( GLuint ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                                     count, sizeof( GLsizei ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                                     type, sizeof( GLenum ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM,
                                     indices, count*TYPESIZE, INPUT_BY_REFERENCE, DPM_API_ARGUMENT_POINTER );	// store this as a pointer to array data for use in capture and replay
#undef TYPESIZE
            }

            int i;
            if ( _JS_DPM_STATE( CAPTURE ) )
            {
                for ( i = 0; i < _JS_MAX_VERTEX_ATTRIBS; ++i ) _jsdpmCaptureDeferredArray( LContext, _JS_ATTRIB_POSITION_INDEX + i, parameterCount + i + 1, end + 1 );
            }	// _JS_DPM_IN_CAPTURE_MODE
            else if ( _JS_DPM_STATE( REPLAY ) )
            {
                for ( i = 0; i < _JS_MAX_VERTEX_ATTRIBS; ++i ) _jsdpmReplayDeferredArray( LContext, _JS_ATTRIB_POSITION_INDEX + i, parameterCount + i + 1 );
            }	// _JS_DPM_IN_REPLAY_MODE
        }
#ifndef JS_DPM_V2
        _jsDPMSubmitQueries();
#endif
#endif

        _JS_PERFORMANCE_START( DrawRangeElementsCounter );
        PSGLcontext*	LContext = _CurrentContext;
        JS_ASSERT( LContext );

#ifndef JS_NO_ERROR_CHECK
        switch ( mode )
        {
            case GL_POINTS:
            case GL_LINES:
            case GL_LINE_STRIP:
            case GL_LINE_LOOP:
            case GL_TRIANGLES:
            case GL_TRIANGLE_STRIP:
            case GL_TRIANGLE_FAN:
            case GL_QUADS:
            case GL_QUAD_STRIP:
                break;
            default:
                _jsSetError( GL_INVALID_ENUM );
                return;
        }

        switch ( type )
        {
            case GL_UNSIGNED_BYTE:
            case GL_UNSIGNED_SHORT:
            case GL_UNSIGNED_INT: // JetStream extention. really needed ?
                break;
            default:
                _jsSetError( GL_INVALID_ENUM );
                return;
        }

        if ( JS_UNLIKELY( end < start ) )
        {
            _jsSetError( GL_INVALID_VALUE );
            return;
        }
#endif

        if ( JS_UNLIKELY( count <= 0 ) )
        {
#ifndef JS_NO_ERROR_CHECK
            if ( count < 0 ) _jsSetError( GL_INVALID_VALUE );
#endif
            return;
        }

        if ( JS_UNLIKELY( ! JSBIT_GET( LContext->attribs->EnabledMask, _JS_ATTRIB_POSITION_INDEX ) ) ) return;

		// [YLIN] use dcbz to zero init dparams
		uint32_t _tmp_clear_loop = c_rounded_size_of_jsDrawParams>>7;
		do{
			--_tmp_clear_loop;
			__dcbz(s_dparams_buff+(_tmp_clear_loop<<7));
		}while(_tmp_clear_loop);
		jsDrawParams *dparams = (jsDrawParams *)s_dparams_buff;
		dparams->mode = mode;
		dparams->indexMin = start;
		dparams->indexMax = end;
		dparams->indexCount = count;
		dparams->indexType = type;
		dparams->indices = indices;

#ifdef _JS_CHECK_FOR_DRAW_OUT_OF_VBO_BOUNDS
        _jsCheckForIndicesOutOfIBOBounds( indices, type, count );
        _jsCheckForDrawOutOfVBOBounds( end );
#endif

        if ( LContext->needValidate ) _jsValidateStates( PSGL_VALIDATE_ALL );

        // determine if this draw must go through the slow path.
        GLboolean slowPath = _jsPlatformRequiresSlowPath( dparams , type, count);

#ifdef JS_DPM
        psglDrawMiddle();
#endif

        if ( JS_UNLIKELY( slowPath ) )
            _jsDrawSlow( dparams );
        else
            _jsPlatformDraw( dparams );

        _JS_PERFORMANCE_STOP( DrawRangeElementsCounter );

#ifdef JS_DPM
    } /* this makes sure that the following timer goes out of
           scope befor the LeaveDrawCall gets called */
#endif
#ifdef JS_DPM
    psglDrawAfter();
#endif
}


/**
 * @short Render primitives from array data
 *
 * glDrawArrays() specifies multiple geometric primitives with very few subroutine calls.
 * You can prespecify separate arrays of vertices, normals, colors, and texture coordinates
 * and use them to construct a sequence of primitives with a single call to glDrawArrays().
 *
 * When glDrawArrays() is called, it uses <I><c>count</c></I> sequential elements from each enabled array to
 * construct a sequence of geometric primitives, beginning with element <I><c>first</c></I>.
 * <I><c>mode</c></I> specifies what kind of primitives are constructed, and how the array elements
 * construct those primitives. If <CODE>GL_VERTEX_ARRAY</CODE> is not enabled, no geometric primitives are generated.
 *
 *  If <CODE>GL_PRIMITIVE_RESTART_NV</CODE> is enabled, then glPrimitiveRestartIndexNV()
 *  sets a position that divides the typical sequence of geometric primitives.
 *  The index set by glPrimitiveRestartIndexNV() marks the end of
 *  the previous primitive and the start of a new primitive, when counting from <I><c>first</c></I>,
 *  the initial position for rendering this vertex array.
 *
 * Vertex attributes that are modified by glDrawArrays() have an unspecified value after glDrawArrays() returns.
 * For example, if <CODE>GL_COLOR_ARRAY</CODE> is enabled, the value of the current color is undefined after
 * glDrawArrays() executes. Attributes that aren&rsquo;t modified remain well defined.
 *
 * @param mode	Specifies what kind of primitives to render. Symbolic constants
 * <CODE>GL_POINTS</CODE>, <CODE>GL_LINE_STRIP</CODE>, <CODE>GL_LINE_LOOP</CODE>, <CODE>GL_LINES</CODE>,
 * <CODE>GL_TRIANGLE_STRIP</CODE>, <CODE>GL_TRIANGLE_FAN</CODE>, <CODE>GL_TRIANGLES</CODE>,
 * <CODE>GL_QUAD_STRIP</CODE>, and <CODE>GL_QUADS</CODE>  are accepted.
 * @param first	Specifies the starting index in the enabled arrays.
 * @param count Specifies the number of indices to be rendered.
 *
 *  @par Errors
Use glGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>GL_INVALID_ENUM</CODE></TD>
<TD><I><c>mode</c></I> is not an accepted value.</TD>
</TR>
<TR>
<TD><CODE>GL_INVALID_VALUE</CODE></TD>
<TD><I><c>count</c></I> is negative.</TD>
</TR>
<TR>
<TD><CODE>GL_INVALID_FRAMEBUFFER_OPERATION_OES</CODE></TD>
<TD>The bound frame buffer does not meet one or more of the following conditions:
<UL>
<LI>Textures are complete.</LI>
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
 * @sa glClientActiveTexture, glColorPointer, glDrawElements, glDrawRangeElements, glNormalPointer, glTexCoordPointer, glVertexPointer, glPrimitiveRestartIndexNV
 */
GLAPI void APIENTRY glDrawArrays( GLenum mode, GLint first, GLsizei count )
{
#ifdef JS_DPM
    { /* this makes sure that the following timer goes out of
               scope befor the LeaveDrawCall gets called */
#endif
        JS_APIENTRY_TIMER_SKIP_OVERALL( glDrawArrays );
#ifdef JS_DPM
        if ( _jsdpmState )
        {
            const int parameterCount = 3;
            _JS_DPM_API_ENTRY_3( glDrawArrays, JSDPM_CALL_TYPE_DRAW, , parameterCount + _JS_MAX_VERTEX_ATTRIBS,	// allocate phantom parameters to hold captured arrays
                                 mode, sizeof( GLenum ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM,
                                 first, sizeof( GLint ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                                 count, sizeof( GLsizei ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE );

            PSGLcontext*	LContext = _CurrentContext;
            JS_ASSERT( LContext );
            int i;
            if ( _JS_DPM_STATE( CAPTURE ) )
            {
                for ( i = 0; i < _JS_MAX_VERTEX_ATTRIBS; ++i ) _jsdpmCaptureDeferredArray( LContext, _JS_ATTRIB_POSITION_INDEX + i, parameterCount + i + 1, count + first );
            }	// _JS_DPM_IN_CAPTURE_MODE
            else if ( _JS_DPM_STATE( REPLAY ) )
            {
                for ( i = 0; i < _JS_MAX_VERTEX_ATTRIBS; ++i ) _jsdpmReplayDeferredArray( LContext, _JS_ATTRIB_POSITION_INDEX + i, parameterCount + i + 1 );
            }	// _JS_DPM_IN_REPLAY_MODE
        }

#ifndef JS_DPM_V2
        _jsDPMSubmitQueries();
#endif
#endif

        _JS_PERFORMANCE_START( DrawArraysCounter );
        PSGLcontext*	LContext = _CurrentContext;
        JS_ASSERT( LContext );

#ifndef JS_NO_ERROR_CHECK
        switch ( mode )
        {
            case GL_POINTS:
            case GL_LINES:
            case GL_LINE_STRIP:
            case GL_LINE_LOOP:
            case GL_TRIANGLES:
            case GL_TRIANGLE_STRIP:
            case GL_TRIANGLE_FAN:
            case GL_QUADS:
            case GL_QUAD_STRIP:
                break;
            default:
                _jsSetError( GL_INVALID_ENUM );
                return;
        }
#endif

        if ( JS_UNLIKELY( count <= 0 ) )
        {
#ifndef JS_NO_ERROR_CHECK
            if ( count < 0 ) _jsSetError( GL_INVALID_VALUE );
#endif
            return;
        }

        if ( JS_UNLIKELY( ! JSBIT_GET( LContext->attribs->EnabledMask, _JS_ATTRIB_POSITION_INDEX ) ) ) return;
		
		// [YLIN] use dcbz to zero init dparams
		uint32_t _tmp_clear_loop = c_rounded_size_of_jsDrawParams>>7;
		do{
			--_tmp_clear_loop;
			__dcbz(s_dparams_buff+(_tmp_clear_loop<<7));
		}while(_tmp_clear_loop);
		jsDrawParams *dparams = (jsDrawParams *)s_dparams_buff;
		dparams->mode = mode;
		dparams->firstVertex = first;
		dparams->vertexCount = count;

#ifdef _JS_CHECK_FOR_DRAW_OUT_OF_VBO_BOUNDS
        _jsCheckForDrawOutOfVBOBounds( first + count - 1 );
#endif

        if ( LContext->needValidate ) _jsValidateStates( PSGL_VALIDATE_ALL );


#ifdef JS_DPM
        psglDrawMiddle();
#endif

        // determine if this draw must go through the slow path.
		GLboolean slowPath = _jsPlatformRequiresSlowPath( dparams, 0, 0);

        if ( JS_UNLIKELY( slowPath ) )
			_jsPlatformDraw( dparams );
        else
			_jsPlatformDraw( dparams );

        _JS_PERFORMANCE_STOP( DrawArraysCounter );

#ifdef JS_DPM
    } /* this makes sure that the following timer goes out of
           scope befor the LeaveDrawCall gets called */
#endif

#ifdef JS_DPM
    psglDrawAfter();
#endif
}

/**
 * @short Causes a primitive to be restarted from inside a vertex array
 *
 *  By default, calls to glDrawElements(), glDrawRangeElements(), or glDrawArrays()
 *  perform uninterrupted processing on all their vertices.
 *  If <CODE>GL_PRIMITIVE_RESTART_NV</CODE> is enabled, then glPrimitiveRestartIndexNV()
 *  describes a special array index that alters the standard processing of geometric primitives
 *  and restarts the primitives.  The separation is most apparent
 *  with <CODE>GL_TRIANGLE_STRIP</CODE>, <CODE>GL_TRIANGLE_FAN</CODE>, and <CODE>GL_QUAD_STRIP</CODE> primitives.
 *  For example, with a <CODE>GL_TRIANGLE_FAN</CODE>, restarting the primitive changes the
 *  anchor point, which may establish a distinct triangle fan.
 *
 *  glPrimitiveRestartIndexNV() sets the special array index for restarting.
 *  For glDrawElements() and glDrawRangeElements(), every time <I><c>index</c></I>
 *  is encountered in the array of indices, the current primitive is ended
 *  and a new one (of the same primitive mode) is started.  glDrawArrays() references
 *  indices sequentially, so <I><c>index</c></I> is used as a count.  A new primitive is started on the
 *  nth vertex, where n = <I><c>index</c></I>, rather than when <I><c>index</c></I> is encountered.
 *
 *  @param index     Specifies which index causes the primitive to restart.  The default value is 0.
 *
 * @sa glEnableClientState, glDisableClientState, glDrawElements, glDrawRangeElements, glDrawArrays
 */

GLAPI void APIENTRY glPrimitiveRestartIndexNV( GLuint index )
{
    JS_APIENTRY_TIMER( glPrimitiveRestartIndexNV );
    if ( _jsdpmState ) { _JS_DPM_API_ENTRY_1( glPrimitiveRestartIndexNV, JSDPM_CALL_TYPE_STATE_GEOM, , 1, index, sizeof( GLuint ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE ); }
    PSGLcontext*	LContext = _CurrentContext;
    JS_ASSERT( LContext );
    LContext->attribs->RestartIndex = index;
    LContext->needValidate |= PSGL_VALIDATE_PRIMITIVE_RESTART;

    if ( LContext->attribSetName ) _jsAttribSetMarkDirty();
}

/** @} Geometry */

// Converts a non native stream to the float native format.
// brings back the data if required
// replicates disabled streams

GLint _jsConvertStream(
    jsAttributeState* asDst, // destination of converted stream
    const jsAttributeState* asSrc, // source for conversion
    GLuint attrIndex,		// which attribute?
    GLint skip,
    GLint first,			// index of the first element to convert
    GLint count,			// number of elements to convert
    const void* indices,	// index array (0==none)
    GLenum indexType )		// 0 == not indexed
{
    jsAttribute* attrDst = asDst->attrib + attrIndex;
    const jsAttribute* attrSrc = asSrc->attrib + attrIndex;

    GLfloat* dataDst = ( GLfloat * )attrDst->clientData;
    const GLint sizeDst = attrDst->clientSize;
    JS_ASSERT( sizeDst <= 4 );

    // default to disabled array
    GLubyte *dataSrc = ( GLubyte * )attrSrc->value;
    GLint sizeSrc    = 4;
    GLuint typeSrc   = GL_FLOAT;
    GLint stride     = 0;

    if ( JSBIT_GET( asSrc->EnabledMask, attrIndex ) )
    {
        GLubyte *origin = NULL;
        if ( attrSrc->arrayBuffer )
        {
            jsBufferObject *buffer = _jsGetBufferObject( _CurrentContext, attrSrc->arrayBuffer );
            // no matter what, we need the latest updated data. This may mean wait for
            // the GPU that is waiting on SPU or another thread to upload the data. Basically
            // do a full finish.
            _jsPlatformRasterFinish();
            origin = ( GLubyte * )_jsPlatformBufferObjectMap( buffer, GL_READ_ONLY );
            JS_ASSERT( origin );
        }

        dataSrc = origin + (( GLubyte * )attrSrc->clientData - ( GLubyte * )NULL );
        sizeSrc = attrSrc->clientSize;
        typeSrc = attrSrc->clientType;
        stride  = attrSrc->clientStride;
    }

    GLfloat defaultValue[4] = {0.f, 0.f, 0.f, 1.f};
    // copy to separate stream percomponent
    // go over each vertex, with deindexing if required
    GLint ret = count;
    for ( int i = 0; i < count; i++ )
    {
        GLuint index = first + i;
        switch ( indexType )
        {
            case 0:
                // not indexed
                break;
            case GL_UNSIGNED_BYTE:
                index = (( GLubyte* )indices )[index];
                break;
            case GL_UNSIGNED_SHORT:
                index = (( GLushort* )indices )[index];
                break;
            case GL_UNSIGNED_INT:
                index = (( GLuint* )indices )[index];
                break;
            default:
                JS_ASSERT( 0 );
                break;
        }

        if ( asDst->PrimitiveRestart && asDst->RestartIndex == index )
        {
            ret = i;
            break;
        }

        if ( JSBIT_GET( asSrc->ModuloMask, attrIndex ) )
            index = index % attrSrc->frequency;
        else
            index = index / attrSrc->frequency;

        GLfloat normalFactor = 1.f;
        GLfloat normalOffset = 0.f;
        if ( attrSrc->normalized )
        {
            switch ( typeSrc )
            {
                case GL_BYTE:
                    normalFactor = 2.f / 255.f;
                    normalOffset = 1.f / 255.f;
                    break;
                case GL_UNSIGNED_BYTE:
                    normalFactor = 1.f / 255.f;
                    break;
                case GL_SHORT:
                    normalFactor = 2.f / 32767.f;
                    normalOffset = 1.f / 32767.f;
                    break;
                default:
                    break;
            }
        }

        // Copy the vertex data for this stream, each component
        if ( typeSrc != GL_FIXED_11_11_10_SCE )
        {
            GLubyte* srcAddress = dataSrc + stride * index; 

            for ( int j = 0;j < sizeDst; ++j )
            {
                if ( j < sizeSrc )
                {
                    switch ( typeSrc )
                    {
                        // For each case, we need to double check to see if the source address fits the natural alignment.
                        case GL_BYTE:
                            dataDst[sizeDst*( i+skip ) + j] = (( GLbyte* )srcAddress)[j];
                            break;
                        case GL_UNSIGNED_BYTE:
                            dataDst[sizeDst*( i+skip ) + j] = (( GLubyte* )srcAddress)[j];
                            break;
                        case GL_SHORT:
                            {
                                if ((unsigned int)srcAddress % sizeof(GLshort) == 0)
                                {
                                    dataDst[sizeDst*( i+skip ) + j] = (( GLshort* )srcAddress)[j];
                                }
                                else
                                {
                                    GLshort val;
                                    memcpy(&val, (srcAddress + j * sizeof(GLshort)), sizeof(GLshort));
                                    dataDst[sizeDst*( i+skip ) + j] = val;
                                }
                            }
                            break;
                        case GL_FIXED:
                            {
                                if ((unsigned int)srcAddress % sizeof(GLint) == 0)
                                {
                                    dataDst[sizeDst*( i+skip ) + j] = X2F((( GLint* )srcAddress)[j] );
                                }
                                else
                                {
                                    GLint val;
                                    memcpy(&val, (srcAddress + j * sizeof(GLint)), sizeof(GLint));
                                    dataDst[sizeDst*( i+skip ) + j] = X2F(val);
                                }
                            }
                            break;
                        case GL_FLOAT:
                            {
                                if ((unsigned int)srcAddress % sizeof(GLfloat) == 0)
                                {
                                    dataDst[sizeDst*( i+skip ) + j] = (( GLfloat* )srcAddress)[j];
                                }
                                else
                                {
                                    GLfloat val;
                                    memcpy(&val, (srcAddress + j * sizeof(GLfloat)), sizeof(GLfloat));
                                    dataDst[sizeDst*( i+skip ) + j] = val;
                                }
                            }
                            break;
                        case GL_HALF_FLOAT_ARB:
                            {
                                if ((unsigned int)srcAddress % sizeof(GLhalfARB) == 0)
                                {
                                    dataDst[sizeDst*( i+skip ) + j] = _jsHalfToFloat((( GLhalfARB* )srcAddress)[j] );                                }
                                else
                                {
                                    GLhalfARB val;
                                    memcpy(&val, (srcAddress + j * sizeof(GLhalfARB)), sizeof(GLhalfARB));
                                    dataDst[sizeDst*( i+skip ) + j] = _jsHalfToFloat(val);
                                }
                            }
                            break;
                        default:
                            JS_ASSERT( 0 );
                            break;
                    }

                    if ( attrSrc->normalized )
                        dataDst[sizeDst*( i+skip ) + j] = normalFactor * dataDst[sizeDst*( i+skip ) + j] + normalOffset;
                }
                else dataDst[sizeDst*( i+skip ) + j] = defaultValue[j];
            }
        }
        else
        {
            // convert from packed GL_FIXED_11_11_10_SCE:
            // 3 components packed into 32-bits
            //  #   bits  shift
            //  0    11     0
            //  1    11    11
            //  2    10    22
            //
            //  The first component is in the least significant bits.
            //
            //  Conversion to float requires first extending to shorts by
            //  padding with the MSBs excluding the sign bit, then
            //
            //  float = (2*short + 1)/(2^16 - 1)
            const GLint cmpData = *( GLint* )( dataSrc + stride * index );
#define BITS(SIZE,OFFSET) ((cmpData >> (OFFSET)) & ((1 << (SIZE)) - 1))
            const GLshort x = ( BITS( 11, 0 ) << 5 ) | BITS( 5, 5 );
            const GLshort y = ( BITS( 11, 11 ) << 5 ) | BITS( 5, 16 );
            const GLshort z = ( BITS( 10, 22 ) << 6 ) | BITS( 6, 25 );
#undef BITS

            dataDst[3*( i+skip ) + 0] = ( 2.0f * x + 1.0f ) / 65535.0f;
            dataDst[3*( i+skip ) + 1] = ( 2.0f * y + 1.0f ) / 65535.0f;
            dataDst[3*( i+skip ) + 2] = ( 2.0f * z + 1.0f ) / 65535.0f;
        }
    }
    if ( JSBIT_GET( asSrc->EnabledMask, attrIndex ) && attrSrc->arrayBuffer )
    {
        jsBufferObject *buffer = _jsGetBufferObject( _CurrentContext, attrSrc->arrayBuffer );
        _jsPlatformBufferObjectUnmap( buffer );
    }
    return ret;
}
