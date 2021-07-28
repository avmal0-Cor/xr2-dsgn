/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 *                Copyright (C) 2009 Sony Computer Entertainment Inc.
 *                                               All Rights Reserved.
 */


#include <PSGL/psgl.h>
#include <PSGL/Debug.h>
#include <PSGL/Types.h>
#include <PSGL/private.h>
#include <PSGL/DPM2.h>
#include <PSGL/Utils.h>
#include <PSGL/PerfCounters.h>
#include <string.h>
#include <assert.h>

jsBufferObject *_jsCreateBufferObject()
{
    GLuint size = sizeof( jsBufferObject ) + _jsPlatformBufferObjectSize();
    _JS_DPM_TRACE_OFF
    jsBufferObject *buffer = ( jsBufferObject * )jsMalloc( size );
    _JS_DPM_TRACE_ON
    if ( !buffer ) return NULL;
    memset( buffer, 0, size );

    // Buffer objects are reference counted so that PBOs can be attached to
    // textures without a copy.
    buffer->refCount = 1;
    new( &buffer->textureReferences ) PSGL::Vector<jsTexture *>();
    new( &buffer->attribSets ) PSGL::Vector<jsAttribSet *>();

    return buffer;
}

void _jsFreeBufferObject( jsBufferObject *buffer )
{
    JS_ASSERT( buffer->refCount > 0 );

    if ( --buffer->refCount == 0 )
    {
        _jsPlatformDestroyBufferObject( buffer );
        buffer->textureReferences.~Vector<jsTexture *>();
        buffer->attribSets.~Vector<jsAttribSet *>();
        _JS_DPM_TRACE_OFF
        jsFree( buffer );
        _JS_DPM_TRACE_ON
    }
}

static void _jsUnbindBufferObject( PSGLcontext *LContext, GLuint name )
{
    if ( LContext->ArrayBuffer == name ) LContext->ArrayBuffer = 0;
    if ( LContext->attribs->ElementArrayBuffer == name ) LContext->attribs->ElementArrayBuffer = 0;
    if ( LContext->PixelPackBuffer == name ) LContext->PixelPackBuffer = 0;
    if ( LContext->PixelUnpackBuffer == name ) LContext->PixelUnpackBuffer = 0;
    for ( int i = 0;i < _JS_MAX_VERTEX_ATTRIBS;++i )
    {
        if ( LContext->attribs->attrib[i].arrayBuffer == name )
        {
            LContext->attribs->attrib[i].arrayBuffer = 0; // Yuck, but that's the spec.
            // update caches
            LContext->attribs->HasVBOMask &= ~( 1 << i );
        }
    }
    JS_ASSERT( _jsCheckAttribCaches( LContext->attribs ) );
    _jsAttribSetDeleteBuffer( LContext, name );
}

/**
 * @addtogroup OpenGL
 * @{
 */

/**
 *  @short Bind a named vertex or pixel buffer to a target
 *
glBindBuffer() lets you create or use a named buffer.
Calling glBindBuffer() with <I><c>target</c></I> set to <CODE>GL_ARRAY_BUFFER</CODE>,  <CODE>GL_ELEMENT_ARRAY_BUFFER</CODE>,
<CODE>GL_PIXEL_PACK_BUFFER_ARB</CODE>, <CODE>GL_PIXEL_UNPACK_BUFFER_ARB</CODE>, or <CODE>GL_TEXTURE_REFERENCE_BUFFER_SCE</CODE>

and <I><c>name</c></I> set to the buffer name, binds the buffer name to the target.

Buffer names are unsigned integers. The value 0 is reserved for GL.

When a buffer is bound to a target, any previous binding for that target is automatically broken.

If an unused buffer name is specified, a buffer object is created.
The resulting default buffer object is a new state vector, initialized
with a write-only, zero-sized memory buffer that is optimized for static data.

glBindBuffer() may also be used to bind an existing buffer object.
If the bind is successful no change is made to the state of the newly bound buffer object.

While a buffer is bound, GL operations on the target to which it is bound affect the bound buffer object,
and queries of the target to which a buffer object is bound return state from the bound object.

@param target   Specifies the target to which the buffer is bound. Must be one of <CODE>GL_ARRAY_BUFFER</CODE>,
<CODE>GL_ELEMENT_ARRAY_BUFFER</CODE>, <CODE>GL_PIXEL_PACK_BUFFER_ARB</CODE>, <CODE>GL_PIXEL_UNPACK_BUFFER_ARB</CODE>,
or <CODE>GL_TEXTURE_REFERENCE_BUFFER_SCE</CODE>.
@param name     Specifies the name of a buffer.

@note
In the initial state the reserved name zero is bound to
<CODE>GL_ARRAY_BUFFER</CODE>, <CODE>GL_ELEMENT_ARRAY_BUFFER</CODE>, <CODE>GL_PIXEL_PACK_BUFFER_ARB</CODE>,
<CODE>GL_PIXEL_UNPACK_BUFFER_ARB</CODE>, and <CODE>GL_TEXTURE_REFERENCE_BUFFER_SCE</CODE>. There is no buffer object
corresponding to the name zero, so attempts to modify or query buffer object state for the
target <CODE>GL_ARRAY_BUFFER</CODE>, <CODE>GL_ELEMENT_ARRAY_BUFFER</CODE>, <CODE>GL_PIXEL_PACK_BUFFER_ARB</CODE>,
<CODE>GL_PIXEL_UNPACK_BUFFER_ARB</CODE>, or <CODE>GL_TEXTURE_REFERENCE_BUFFER_SCE</CODE>
while zero is bound will generate GL Errors.
@par
While a buffer object is bound, any GL operations on that object affect any other bindings of that object.

@par Errors
Use glGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>GL_INVALID_ENUM</CODE></TR>
<TD><I><c>name</c></I> is not one of the allowable values.</TD>
</TR>
</TABLE>
 *
 * @sa glBufferData, glBufferSubData, glDeleteBuffers, glGenBuffers, glMapBuffer, glUnmapBuffer
 */

GLAPI void APIENTRY glBindBuffer( GLenum target, GLuint name )
{
    JS_APIENTRY_TIMER( glBindBuffer );
    if ( _jsdpmState )
    {
        _JS_DPM_API_ENTRY_2( glBindBuffer, JSDPM_CALL_TYPE_STATE_TEX, , 2,
                             target, sizeof( GLenum ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM,
                             name, sizeof( GLuint ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE );
    }

    PSGLcontext *LContext = _CurrentContext;
    JS_ASSERT( LContext );
#ifndef JS_NO_ERROR_CHECK
    switch ( target )
    {
        case GL_ARRAY_BUFFER:
        case GL_ELEMENT_ARRAY_BUFFER:
        case GL_PIXEL_PACK_BUFFER_ARB:
        case GL_PIXEL_UNPACK_BUFFER_ARB:
        case GL_TEXTURE_REFERENCE_BUFFER_SCE:
            break;
        default:
            _jsSetError( GL_INVALID_ENUM );
            return;
    }
#endif
    if ( name ) _jsTexNameSpaceCreateNameLazy( &LContext->bufferObjectNameSpace, name );

#ifdef JS_ALLOCINFO
    if ( name )
    {
        jsBufferObject *bufferObject = _jsGetBufferObject( LContext, name );
        bufferObject->target = target;
    }
#endif

    switch ( target )
    {
        case GL_ARRAY_BUFFER:
            LContext->ArrayBuffer = name;
            break;
        case GL_ELEMENT_ARRAY_BUFFER:
            LContext->attribs->ElementArrayBuffer = name;
            if ( LContext->attribSetName ) _jsAttribSetMarkDirty();
            break;
        case GL_PIXEL_PACK_BUFFER_ARB:
            LContext->PixelPackBuffer = name;
            break;
        case GL_PIXEL_UNPACK_BUFFER_ARB:
            LContext->PixelUnpackBuffer = name;
            break;
        case GL_TEXTURE_REFERENCE_BUFFER_SCE:
            LContext->TextureBuffer = name;
            break;
        default:
            JS_ASSERT( 0 );
            break;
    }
}

/**
 *  @short Delete named buffer objects
 *
glDeleteBuffers() deletes <I><c>n</c></I> buffer objects named by the elements of the array <I><c>buffers</c></I>.
After a buffer object is deleted, it has no contents, and its name is free for reuse.

glDeleteBuffers() silently ignores zero and names that do not correspond to existing buffer objects.

 *  @param n        Specifies the number of buffer objects to be deleted.
 *  @param buffers  Specifies an array of buffer object names to be deleted.
 *
 *  @note
If a buffer object is deleted while it is bound, all bindings to that object in the current context (i.e. in the thread that called glDeleteBuffers()) are reset to zero. Bindings to that buffer in other contexts and other threads are not affected, but attempting to use a deleted buffer in another thread produces undefined results, including but not limited to possible GL Errors
and rendering corruption. Using a deleted buffer in another context or thread may not, however, result in program termination.
 *
 * @par Errors
Use glGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>GL_INVALID_VALUE</CODE></TD>
<TD><I><c>n</c></I> is negative.</TD>
</TR>
</TABLE>
 *
 * @sa glBufferData, glBufferSubData, glBindBuffer, glGenBuffers, glMapBuffer, glUnmapBuffer
 */
#include <stdio.h>

GLAPI void APIENTRY glDeleteBuffers( GLsizei n, const GLuint *buffers )
{
    JS_APIENTRY_TIMER( glDeleteBuffers );
    if ( _jsdpmState )
    {
        _JS_DPM_API_ENTRY_2( glDeleteBuffers, JSDPM_CALL_TYPE_STATE_TEX, , 2,
                             n, sizeof( GLsizei ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             buffers, ( n * sizeof( GLuint ) ), INPUT_BY_REFERENCE, DPM_API_ARGUMENT_POINTER );
    }

    PSGLcontext *LContext = _CurrentContext;
    JS_ASSERT( LContext );
    for ( int i = 0;i < n;++i )
    {
        if ( !_jsTexNameSpaceIsName( &LContext->bufferObjectNameSpace, buffers[i] ) ) continue;
        if ( buffers[i] ) _jsUnbindBufferObject( LContext, buffers[i] );
    }
    _jsTexNameSpaceDeleteNames( &LContext->bufferObjectNameSpace, n, buffers );
}

/**
 *  @short Generate buffer object names
 *
glGenBuffers() returns <I><c>n</c></I> buffer object names in <I><c>buffers</c></I>.

These names are marked as used, for the purposes of glGenBuffers() only,
but they acquire buffer state only when they are first bound, just as if they were unused.

 *  @param n        Specifies the number of buffer objects to be generated.
 *  @param buffers  Specifies an array in which the generated buffer object names are stored.
 *
 * @par Errors
Use glGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>GL_INVALID_VALUE</CODE></TD>
<TD><I><c>n</c></I> is negative.</TD>
</TR>
</TABLE>
 *
 * @sa glBindBuffer, glBufferData, glBufferSubData, glDeleteBuffers, glMapBuffer, glUnmapBuffer
 */
GLAPI void APIENTRY glGenBuffers( GLsizei n, GLuint *buffers )
{
    JS_APIENTRY_TIMER( glGenBuffers );
    if ( _jsdpmState )
    {
        _JS_DPM_API_ENTRY_2( glGenBuffers, JSDPM_CALL_TYPE_STATE_TEX, , 2,
                             n, sizeof( GLsizei ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             buffers, ( n * sizeof( GLuint ) ), INPUT_OUTPUT, DPM_API_ARGUMENT_POINTER );
    }

    PSGLcontext *LContext = _CurrentContext;
    JS_ASSERT( LContext );
    _jsTexNameSpaceGenNames( &LContext->bufferObjectNameSpace, n, buffers );
}

/**
 *  @short Creates and initializes the data store of a buffer object.
 *
glBufferData() lets you create and initialize the data store of a buffer object.

If data is non-null, then the source data is copied to the buffer object&rsquo;s data store.
If data is null, then the contents of the buffer object&rsquo;s data store are undefined.

The options for <I><c>usage</c></I> are:

 * <TABLE rules=all>
 * <TR><TD><CODE>GL_STATIC_DRAW</CODE></TD>
 * <TD>the data store contents are typically specified once by the application during initialization
and then used many times as the source for GL drawing commands.
 * </TD></TR>
 * <TR><TD><CODE>GL_DYNAMIC_DRAW</CODE></TD>
 * <TD>the data store contents are typically respecified by the application several times during each frame
and then used as the source for GL drawing commands.
 * </TD></TR>
 * <TR><TD><CODE>GL_STREAM_DRAW</CODE></TD>
 * <TD>the data store contents are typically respecified by the application once per frame
and then used as the source for GL drawing commands.
 * </TD></TR>
 * <TR><TD><CODE>GL_SYSTEM_DRAW_SCE</CODE></TD>
 * <TD>the data store contents are allocated in host memory within the area specified using the <CODE>hostMemorySize</CODE> argument to psglInit().
 * </TD></TR>
 * </TABLE>

glBufferData() deletes any existing data store, and sets the values of the buffer object&rsquo;s state variables thus:
<ul>
<li><CODE>GL_BUFFER_PITCH_SCE</CODE> initialized to 0.</li>
<li><CODE>GL_BUFFER_SIZE</CODE> initialized to <I><c>size</c></I>.</li>
<li><CODE>GL_BUFFER_USAGE</CODE> initialized to <I><c>usage</c></I>.</li>
<li><CODE>GL_BUFFER_ACCESS</CODE> initialized to <CODE>GL_WRITE_ONLY</CODE>.</li>
</ul>

Data element alignment must be consistent with the requirements of client memory,
with an additional base-level requirement that an offset within a buffer to a datum
comprising N basic machine units be a multiple of N.

@param target  Specifies the buffer object target. Must be one of <CODE>GL_ARRAY_BUFFER</CODE>, <CODE>GL_ELEMENT_ARRAY_BUFFER</CODE>,
<CODE>GL_PIXEL_PACK_BUFFER_ARB</CODE>, <CODE>GL_PIXEL_UNPACK_BUFFER_ARB</CODE>, or <CODE>GL_TEXTURE_REFERENCE_BUFFER_SCE</CODE>.
@param size    Specifies the size of the data store in basic machine units.
@param data    Specifies the source data in client memory.
@param usage   Specifies the expected application usage pattern of the data store.
Accepted values are <CODE>GL_STATIC_DRAW</CODE>, <CODE>GL_DYNAMIC_DRAW</CODE>, <CODE>GL_STREAM_DRAW</CODE>, and <CODE>GL_SYSTEM_DRAW_SCE</CODE>.

@note
For values other than <CODE>GL_SYSTEM_DRAW_SCE</CODE>, <I><c>usage</c></I> is provided as a performance hint only&mdash;the specified usage value does not constrain the actual usage pattern of the data store.  If you specify <CODE>GL_SYSTEM_DRAW_SCE</CODE>, the data store is always created in host memory.
@par 
glBufferData() and glBufferSubData() define two new types, <CODE>GLintptr</CODE> and <CODE>GLsizeiptr</CODE>,
that will work well on 64-bit systems, analogous to C&rsquo;s "intptr t". Both types are defined as signed integers
large enough to contain any pointer value. As a result, they naturally scale to larger numbers of bits on
systems with 64-bit or even larger pointers.
@note
For more information about using mapped buffer objects, refer to the "Mapped Buffer
Objects" section of the
<i>PlayStation&reg; Graphics Library Programming Guide</i>.

 *
 * @par Errors
Use glGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>GL_INVALID_ENUM</CODE></TD>
<TD><I><c>target</c></I> is not one of the allowable values.</TD>
</TR>
<TR>
<TD><CODE>GL_INVALID_ENUM</CODE></TD>
<TD><I><c>usage</c></I> is not one of the allowable values.</TD>
</TR>
<TR>
<TD><CODE>GL_OUT_OF_MEMORY</CODE></TD>
<TD>The GL is unable to create a data store of the requested size.</TD>
</TR>
</TABLE>
 *
 * @sa glBufferSubData, glBindBuffer, glDeleteBuffers, glGenBuffers, glMapBuffer, glUnmapBuffer, glTextureReferenceSCE
 */
GLAPI void APIENTRY glBufferData( GLenum target, GLsizeiptr size, const GLvoid * data, GLenum usage )
{
    JS_APIENTRY_TIMER( glBufferData );
#ifdef JS_DPM
    if ( _jsdpmState )
    {
        if (( data == NULL ) && _JS_DPM_STATE( CAPTURE ) )
        {
            _JS_DPM_API_ENTRY_4( glBufferData, JSDPM_CALL_TYPE_STATE_TEX, , 4,
                                 target, sizeof( GLenum ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM,
                                 size, sizeof( GLsizeiptr ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                                 data, sizeof( GLvoid * ), INPUT_BY_VALUE, DPM_API_ARGUMENT_POINTER,
                                 usage, sizeof( GLenum ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM );
        }
        else
        {
            _JS_DPM_API_ENTRY_4( glBufferData, JSDPM_CALL_TYPE_STATE_TEX, , 4,
                                 target, sizeof( GLenum ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM,
                                 size, sizeof( GLsizeiptr ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                                 data, size, INPUT_BY_REFERENCE, DPM_API_ARGUMENT_POINTER,
                                 usage, sizeof( GLenum ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM );
        }
    }
#endif

    PSGLcontext *LContext = _CurrentContext;
    JS_ASSERT( LContext );

    GLuint name = 0;
    switch ( target )
    {
        case GL_ARRAY_BUFFER:
            name = LContext->ArrayBuffer;
            break;
        case GL_ELEMENT_ARRAY_BUFFER:
            name = LContext->attribs->ElementArrayBuffer;
            break;
        case GL_PIXEL_PACK_BUFFER_ARB:
            name = LContext->PixelPackBuffer;
            break;
        case GL_PIXEL_UNPACK_BUFFER_ARB:
            name = LContext->PixelUnpackBuffer;
            break;
        case GL_TEXTURE_REFERENCE_BUFFER_SCE:
            name = LContext->TextureBuffer;
            break;
        default:
            _jsSetError( GL_INVALID_ENUM );
            return;
    }
#ifndef JS_NO_ERROR_CHECK
    if ( name == 0 )
    {
        _jsSetError( GL_INVALID_OPERATION );
        return;
    }
    switch ( usage )
    {
        case GL_STATIC_DRAW:
        case GL_DYNAMIC_DRAW:
            break;
        case GL_STREAM_DRAW: // Not in OpenGL ES 1.1
        case GL_STREAM_READ:
        case GL_STREAM_COPY:
        case GL_STATIC_READ:
        case GL_STATIC_COPY:
        case GL_DYNAMIC_READ:
        case GL_DYNAMIC_COPY:
        case GL_SYSTEM_DRAW_SCE:
            break;
        default:
            _jsSetError( GL_INVALID_ENUM );
            return;
    }
#endif
    jsBufferObject* bufferObject = _jsGetBufferObject( LContext, name );

#ifndef JS_NO_ERROR_CHECK
    // not in the spec, but I am not sure what we should do.
    if ( bufferObject->mapped )
    {
        _jsSetError( GL_INVALID_OPERATION );
        return;
    }
#endif


    // recreate if multiply referenced
    //  This happens when a texture references a PBO.
    _JS_PERFORMANCE_START( BufferDataCounter );
    if ( bufferObject->refCount > 1 )
    {
        _jsTexNameSpaceDeleteNames( &LContext->bufferObjectNameSpace, 1, &name );
        _jsTexNameSpaceCreateNameLazy( &LContext->bufferObjectNameSpace, name );

        bufferObject = _jsGetBufferObject( LContext, name );
    }
    JS_ASSERT( bufferObject != NULL );

    if ( bufferObject->size > 0 ) _jsPlatformDestroyBufferObject( bufferObject );
    bufferObject->usage = usage;
    bufferObject->size = size;
    bufferObject->isSurface = GL_FALSE;
    bufferObject->width = 0;
    bufferObject->height = 0;
    bufferObject->multisampling = GL_MULTISAMPLING_NONE_SCE;
    bufferObject->internalFormat = GL_NONE;

    if ( size > 0 )
    {
        _jsAllocSetHint( _JS_ALLOCTYPE_BUFFER_OBJECT, name );
        GLboolean created = _jsPlatformCreateBufferObject( bufferObject );
        _jsAllocClearHint();
        if ( !created )
        {
            _jsSetError( GL_OUT_OF_MEMORY );
            _JS_PERFORMANCE_STOP( BufferDataCounter );
            return;
        }
        _jsAllocSetHint( _JS_ALLOCTYPE_BUFFER_OBJECT, name );
        if ( data ) _jsPlatformBufferObjectSetData( bufferObject, 0, size, data, GL_TRUE );
        _jsAllocClearHint();
    }

    _jsAttribSetBufferData( LContext, name );
    _JS_PERFORMANCE_STOP( BufferDataCounter );
}

/**
 *  @short Creates the data store of a buffer object as a 2D tiled surface.
 *
glBufferSurfaceSCE() creates the data store of a buffer object as a
2D tiled surface. This is mainly useful for texture references that are used
as a render target.

Note that using tiled surfaces for textures improves
performance, especially for render targets, but has several limitations (see glTextureReferenceSCE()).

The initial contents of the buffer are undefined.

The options for <I><c>usage</c></I> are:

 * <TABLE rules=all>
 * <TR><TD><CODE>GL_STATIC_DRAW</CODE></TD>
 * <TD>The data store contents are typically specified once by the application during initialization
and then used many times as the source for GL drawing commands.
 * </TD></TR>
 * <TR><TD><CODE>GL_DYNAMIC_DRAW</CODE></TD>
 * <TD>The data store contents are typically respecified by the application several times during each frame
and then used as the source for GL drawing commands.
 * </TD></TR>
 * <TR><TD><CODE>GL_STREAM_DRAW</CODE></TD>
 * <TD>The data store contents are typically respecified by the application once per frame
and then used as the source for GL drawing commands.
 * </TD></TR>
 * <TR><TD><CODE>GL_SYSTEM_DRAW_SCE</CODE></TD>
 * <TD>The data store contents are allocated in host memory within the area specified using the <CODE>hostMemorySize</CODE> argument to psglInit().
 * </TD></TR>
 * </TABLE>

glBufferSurfaceSCE() deletes any existing data store, and sets the values of the buffer object&rsquo;s state variables as follows:
<UL>
<LI><CODE>GL_BUFFER_PITCH_SCE</CODE> is initialized to a platform-specific pitch value, at least <I><c>width</c></I> times the
number of bytes per pixel specified by <I><c>internalFormat</c></I>.</LI>
<LI><CODE>GL_BUFFER_SIZE</CODE> is initialized to <I><c>height</c></I> times the pitch value.</LI>
<LI><CODE>GL_BUFFER_USAGE</CODE> is initialized to <I><c>usage</c></I>.</LI>
<LI><CODE>GL_BUFFER_ACCESS</CODE> is initialized to <CODE>GL_WRITE_ONLY</CODE>.</LI>
</UL>
@param target         Specifies the buffer object target. Must be one of <CODE>GL_ARRAY_BUFFER</CODE>, <CODE>GL_ELEMENT_ARRAY_BUFFER</CODE>,
<CODE>GL_PIXEL_PACK_BUFFER_ARB</CODE>, <CODE>GL_PIXEL_UNPACK_BUFFER_ARB</CODE>, or <CODE>GL_TEXTURE_REFERENCE_BUFFER_SCE</CODE>.
@param width          Specifies the width of the surface.
@param height         Specifies the height of the surface.
@param multisampling  Specifies the multisampling type of the surface. A value of <CODE>GL_MULTISAMPLING_2X_DIAGONAL_CENTERED_SCE</CODE> will
effectively double the width, and <CODE>GL_MULTISAMPLING_4X_SQUARE_ROTATED_SCE</CODE> or <CODE>GL_MULTISAMPLING_4X_SQUARE_CENTERED_SCE</CODE> will
effectively double both the width and the height of the buffer. For best performance, this should match the multisampling mode used to render to
textures stored into this buffer.
@param internalFormat Specifies the internal format of the buffer surface. This includes the number of bytes per texel of the texture.
For best performance, this should match the internal format of the textures stored into the buffer
@param usage   Specifies the expected application usage pattern of the data store.
Accepted values are <CODE>GL_STATIC_DRAW</CODE>, <CODE>GL_DYNAMIC_DRAW</CODE>, <CODE>GL_STREAM_DRAW</CODE>, and <CODE>GL_SYSTEM_DRAW_SCE</CODE>.

@note
For values other than <CODE>GL_SYSTEM_DRAW_SCE</CODE>, <I><c>usage</c></I> is provided as a performance hint 
only&mdash;the specified usage value does not constrain the actual usage pattern of the data store.  If you specify 
<CODE>GL_SYSTEM_DRAW_SCE</CODE>, the data store is always created in host memory.

@note
<CODE>GL_DEPTH_COMPONENT16</CODE> can now be specified as <CODE>internalFormat</CODE>.

@note
Prior to SDK 2.2.0, PSGL allocated <CODE>GL_SYSTEM_DRAW_SCE</CODE> in tiled local memory, which was not 
the expected behavior. PSGL now allocates <CODE>GL_SYSTEM_DRAW_SCE</CODE> in tiled main memory. This change means 
that tiled main memory buffer allocations are scrambled according to PSGL internal tile region attributes and settings.
RSX&trade; can correctly render to and sample from such buffers because it knows the tile region settings. PPU/SPU cannot 
access the main memory tile layout without knowing the tile region attribute and settings, and attempts to 
read/write the buffer with PPU/SPU using <CODE>glMapBuffer</CODE> and 
<CODE>glBufferSubData</CODE> will result in scrambled data. This restriction only applies to tiled main memory allocations, not tiled
local/RSX&trade; memory. Tiled local memory address translation is automatically handled by RSX&trade; and can be accessed linearly. Tiled main
memory allocation should only be used when PPU/SPU doesn&rsquo;t need to read/write the buffer. To read/write using PPU/SPU, you can either 
use <CODE>glBufferSurfaceSCE</CODE> with a usage flag of <CODE>GL_STATIC_DRAW</CODE>, <CODE>GL_DYNAMIC_DRAW</CODE>, or 
<CODE>GL_STREAM_DRAW</CODE>; or use <CODE>glBufferData</CODE> with a usage flag of <CODE>GL_SYSTEM_DRAW_SCE</CODE>. In that case, 
<CODE>glBufferData</CODE> creates linear main memory allocations that can be accessed by PPU/SPU.

@note
For more information, including sample code, refer to the "Texture Buffer Objects" section of the
<i>PlayStation&reg; Graphics Library Programming Guide</i>.

 * @par Errors
Use glGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>GL_INVALID_ENUM</CODE></TD>
<TD><I><c>target</c></I> is not one of the allowable values.</TD>
</TR>
<TR>
<TD><CODE>GL_INVALID_ENUM</CODE></TD>
<TD><I><c>multisampling</c></I> is not one of the allowable values.</TD>
</TR>
<TR>
<TD><CODE>GL_INVALID_ENUM</CODE></TD>
<TD><I><c>usage</c></I> is not one of the allowable values.</TD>
</TR>
<TR>
<TD><CODE>GL_INVALID_VALUE</CODE></TD>
<TD><I><c>internalFormat</c></I> is not one of the allowable values.</TD>
</TR>
<TR>
<TD><CODE>GL_INVALID_VALUE</CODE></TD>
<TD><I><c>width</c></I> or <I><c>height</c></I> is 0.</TD>
</TR>
<TR>
<TD><CODE>GL_INVALID_OPERATION</CODE></TD>
<TD>No buffer is currently bound to the specified target.</TD>
</TR>
<TR>
<TD><CODE>GL_OUT_OF_MEMORY</CODE></TD>
<TD>The GL is unable to create a data store of the requested size.</TD>
</TR>
</TABLE>
 *
 * @sa glBufferData, glBindBuffer, glDeleteBuffers, glGenBuffers, glMapBuffer, glUnmapBuffer, glTextureReferenceSCE
 */
GLAPI void APIENTRY glBufferSurfaceSCE( GLenum target, GLuint width, GLuint height, GLenum multisampling, GLenum internalFormat, GLenum usage )
{
    JS_APIENTRY_TIMER( glBufferSurfaceSCE );
#ifdef JS_DPM
    if ( _jsdpmState )
    {
        _JS_DPM_API_ENTRY_6( glBufferSurfaceSCE, JSDPM_CALL_TYPE_STATE_TEX, , 6,
                             target, sizeof( GLenum ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM,
                             width, sizeof( GLuint ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             height, sizeof( GLuint ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             multisampling, sizeof( GLenum ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM,
                             internalFormat, sizeof( GLenum ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM,
                             usage, sizeof( GLenum ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM );
    }
#endif

    PSGLcontext *LContext = _CurrentContext;
    JS_ASSERT( LContext );

    GLuint name = 0;
    switch ( target )
    {
        case GL_ARRAY_BUFFER:
            name = LContext->ArrayBuffer;
            break;
        case GL_ELEMENT_ARRAY_BUFFER:
            name = LContext->attribs->ElementArrayBuffer;
            break;
        case GL_PIXEL_PACK_BUFFER_ARB:
            name = LContext->PixelPackBuffer;
            break;
        case GL_PIXEL_UNPACK_BUFFER_ARB:
            name = LContext->PixelUnpackBuffer;
            break;
        case GL_TEXTURE_REFERENCE_BUFFER_SCE:
            name = LContext->TextureBuffer;
            break;
        default:
            _jsSetError( GL_INVALID_ENUM );
            return;
    }
    internalFormat = _jsPlatformChooseInternalFormat( internalFormat );
#ifndef JS_NO_ERROR_CHECK
    if ( internalFormat == GL_INVALID_ENUM )
    {
        _jsSetError( GL_INVALID_ENUM );
        return;
    }
    if ( name == 0 )
    {
        _jsSetError( GL_INVALID_OPERATION );
        return;
    }
    switch ( usage )
    {
        case GL_STATIC_DRAW:
        case GL_DYNAMIC_DRAW:
            break;
        case GL_STREAM_DRAW: // Not in OpenGL ES 1.1
        case GL_STREAM_READ:
        case GL_STREAM_COPY:
        case GL_STATIC_READ:
        case GL_STATIC_COPY:
        case GL_DYNAMIC_READ:
        case GL_DYNAMIC_COPY:
        case GL_SYSTEM_DRAW_SCE:
            break;
        default:
            _jsSetError( GL_INVALID_ENUM );
            return;
    }
    if ( !_jsIsMultisampling( multisampling ) )
    {
        _jsSetError( GL_INVALID_ENUM );
        return;
    }
    if (( width <= 0 ) || ( height <= 0 ) )
    {
        _jsSetError( GL_INVALID_VALUE );
        return;
    }
#endif
    jsBufferObject* bufferObject = _jsGetBufferObject( LContext, name );

#ifndef JS_NO_ERROR_CHECK
    // not in the spec, but I am not sure what we should do.
    if ( bufferObject->mapped )
    {
        _jsSetError( GL_INVALID_OPERATION );
        return;
    }
#endif


    // recreate if multiply referenced
    //  This happens when a texture references a PBO.
    if ( bufferObject->refCount > 1 )
    {
        _jsTexNameSpaceDeleteNames( &LContext->bufferObjectNameSpace, 1, &name );
        _jsTexNameSpaceCreateNameLazy( &LContext->bufferObjectNameSpace, name );

        bufferObject = _jsGetBufferObject( LContext, name );
    }
    JS_ASSERT( bufferObject != NULL );

    if ( bufferObject->size > 0 ) _jsPlatformDestroyBufferObject( bufferObject );
    bufferObject->usage = usage;
    bufferObject->size = 0;
    bufferObject->isSurface = GL_TRUE;
    bufferObject->internalFormat = internalFormat;
    bufferObject->width = width;
    bufferObject->height = height;
    bufferObject->multisampling = multisampling;

    _jsAllocSetHint( _JS_ALLOCTYPE_BUFFER_OBJECT, name );
    GLboolean created = _jsPlatformCreateBufferObject( bufferObject );
    _jsAllocClearHint();
    if ( !created )
    {
        _jsSetError( GL_OUT_OF_MEMORY );
        return;
    }

    _jsAttribSetBufferData( LContext, name );
}

/**
 *  @short Retrieves buffer object parameter
 *
 *  glGetBufferParameteriv() retrieves buffer object parameters such size or pitch.
 *
 * @param target         Specifies the buffer object target. Must be one of <CODE>GL_ARRAY_BUFFER</CODE>, <CODE>GL_ELEMENT_ARRAY_BUFFER</CODE>,
<CODE>GL_PIXEL_PACK_BUFFER_ARB</CODE>, <CODE>GL_PIXEL_UNPACK_BUFFER_ARB</CODE>, or <CODE>GL_TEXTURE_REFERENCE_BUFFER_SCE</CODE>.
 * @param pname          Specifies the parameter to retrieve. Must be one of
 * <CODE>GL_BUFFER_SIZE</CODE> or <CODE>GL_BUFFER_PITCH_SCE</CODE>.
 * @param params         Specifies a pointer to an <CODE>int</CODE> variable to be
 * filled with the value of the parameter.
 *
 * @par Errors
Use glGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>GL_INVALID_ENUM</CODE></TD>
<TD><I><c>target</c></I> is not one of the allowable values.</TD>
</TR>
<TR>
<TD><CODE>GL_INVALID_ENUM</CODE></TD>
<TD><I><c>pname</c></I> is not one of the allowable values.</TD>
</TR>
<TR>
<TD><CODE>GL_INVALID_OPERATION</CODE></TD>
<TD>No buffer is currently bound to the specified target.</TD>
</TR>
</TABLE>
 */
GLAPI void APIENTRY glGetBufferParameteriv( GLenum target, GLenum pname, GLint *params )
{
    JS_APIENTRY_TIMER( glGetBufferParameteriv );
#ifdef JS_DPM
    if ( _jsdpmState )
    {
        _JS_DPM_API_ENTRY_3( glGetBufferParameteriv, JSDPM_CALL_TYPE_STATE_TEX, , 3,
                             target, sizeof( GLenum ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM,
                             pname, sizeof( GLenum ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM,
                             params, sizeof( GLint ), INPUT_OUTPUT, DPM_API_ARGUMENT_POINTER );
    }
#endif

    PSGLcontext *LContext = _CurrentContext;
    JS_ASSERT( LContext );

    GLuint name = 0;
    switch ( target )
    {
        case GL_ARRAY_BUFFER:
            name = LContext->ArrayBuffer;
            break;
        case GL_ELEMENT_ARRAY_BUFFER:
            name = LContext->attribs->ElementArrayBuffer;
            break;
        case GL_PIXEL_PACK_BUFFER_ARB:
            name = LContext->PixelPackBuffer;
            break;
        case GL_PIXEL_UNPACK_BUFFER_ARB:
            name = LContext->PixelUnpackBuffer;
            break;
        case GL_TEXTURE_REFERENCE_BUFFER_SCE:
            name = LContext->TextureBuffer;
            break;
        default:
            _jsSetError( GL_INVALID_ENUM );
            return;
    }
#ifndef JS_NO_ERROR_CHECK
    if ( name == 0 )
    {
        _jsSetError( GL_INVALID_OPERATION );
        return;
    }
#endif
    jsBufferObject* bufferObject = _jsGetBufferObject( LContext, name );
    switch ( pname )
    {
        case GL_BUFFER_SIZE:
            params[0] = bufferObject->size;
            break;
        case GL_BUFFER_PITCH_SCE:
            _jsPlatformGetBufferParameteriv( bufferObject, pname, params );
            break;
        default:
            _jsSetError( GL_INVALID_ENUM );
            return;
    }
}

/**
 *  @short Modifies some or all of the data contained in a buffer object&rsquo;s data store
 *
glBufferSubData() lets you modify some or all of the data contained in a buffer object&rsquo;s data store.

@param target  Specifies the buffer object target. Must be one of
<CODE>GL_ARRAY_BUFFER</CODE>, <CODE>GL_ELEMENT_ARRAY_BUFFER</CODE>,
<CODE>GL_PIXEL_PACK_BUFFER_ARB</CODE>, <CODE>GL_PIXEL_UNPACK_BUFFER_ARB</CODE>, or <CODE>GL_TEXTURE_REFERENCE_BUFFER_SCE</CODE>.
@param offset  Specifies the starting offset of the data to be replaced in basic machine units.
@param size    Specifies the size of the data to be replaced in basic machine units.
@param data    Specifies a region of client memory <I><c>size</c></I> basic machine units in length,
containing the data that replace the specified buffer range.

@note
glBufferData() and glBufferSubData() define two new types, <CODE>GLintptr</CODE> and <CODE>GLsizeiptr</CODE>,
that will work well on 64-bit systems, analogous to C&rsquo;s "intptr t". Both types are defined as signed integers
large enough to contain any pointer value. As a result, they naturally scale to larger numbers of bits on
systems with 64-bit or even larger pointers.
 *
 * @par Errors
Use glGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>GL_INVALID_ENUM</CODE></TD>
<TD><I><c>target</c></I> is not one of the allowable values.</TD>
</TR>
<TR>
<TD><CODE>GL_INVALID_VALUE</CODE></TD>
<TD><I><c>offset</c></I> or <I><c>size</c></I> is less than zero,
or if <I><c>offset</c></I> + <I><c>size</c></I> is greater than the value of <CODE>GL_BUFFER_SIZE</CODE>.</TD>
</TR>
</TABLE>
 *
 * @sa glBufferData, glBindBuffer, glDeleteBuffers, glGenBuffers, glMapBuffer, glUnmapBuffer
 */
GLAPI void APIENTRY glBufferSubData( GLenum target, GLintptr offset, GLsizeiptr size, const GLvoid *data )
{
    JS_APIENTRY_TIMER( glBufferSubData );
#ifdef JS_DPM
    if ( _jsdpmState )
    {
        if (( data == NULL ) && _JS_DPM_STATE( CAPTURE ) )
        {
            _JS_DPM_API_ENTRY_4( glBufferSubData, JSDPM_CALL_TYPE_STATE_TEX, , 4,
                                 target, sizeof( GLenum ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM,
                                 offset, sizeof( GLintptr ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                                 size, sizeof( GLsizeiptr ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                                 data, sizeof( GLvoid * ), INPUT_BY_VALUE, DPM_API_ARGUMENT_POINTER );
        }
        else
        {
            _JS_DPM_API_ENTRY_4( glBufferSubData, JSDPM_CALL_TYPE_STATE_TEX, , 4,
                                 target, sizeof( GLenum ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM,
                                 offset, sizeof( GLintptr ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                                 size, sizeof( GLsizeiptr ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                                 data, size, INPUT_BY_REFERENCE, DPM_API_ARGUMENT_POINTER );
        }
    }
#endif

    PSGLcontext *LContext = _CurrentContext;
    JS_ASSERT( LContext );
    GLuint name = 0;
    switch ( target )
    {
        case GL_ARRAY_BUFFER:
            name = LContext->ArrayBuffer;
            break;
        case GL_ELEMENT_ARRAY_BUFFER:
            name = LContext->attribs->ElementArrayBuffer;
            break;
        case GL_PIXEL_PACK_BUFFER_ARB:
            name = LContext->PixelPackBuffer;
            break;
        case GL_PIXEL_UNPACK_BUFFER_ARB:
            name = LContext->PixelUnpackBuffer;
            break;
        case GL_TEXTURE_REFERENCE_BUFFER_SCE:
            name = LContext->TextureBuffer;
            break;
        default:
            _jsSetError( GL_INVALID_ENUM );
            return;
    }
#ifndef JS_NO_ERROR_CHECK
    if ( name == 0 )
    {
        _JS_PERFORMANCE_STOP( BufferSubDataCounter );
        _jsSetError( GL_INVALID_OPERATION );
        return;
    }
#endif

    _JS_PERFORMANCE_START( BufferSubDataCounter );
    jsBufferObject* bufferObject = _jsGetBufferObject( LContext, name );

#ifndef JS_NO_ERROR_CHECK
    if ( bufferObject->mapped )
    {
        _JS_PERFORMANCE_STOP( BufferSubDataCounter );
        _jsSetError( GL_INVALID_OPERATION );
        return;
    }
#endif

    // recreate if multiply referenced
    //  This happens when a texture references a PBO.
    if ( bufferObject->refCount > 1 )
    {
        jsBufferObject* oldBufferObject = bufferObject;

        _jsTexNameSpaceDeleteNames( &LContext->bufferObjectNameSpace, 1, &name );
        _jsTexNameSpaceCreateNameLazy( &LContext->bufferObjectNameSpace, name );

        bufferObject = _jsGetBufferObject( LContext, name );
        bufferObject->size = oldBufferObject->size;
        bufferObject->usage = oldBufferObject->usage;

        // copy buffer
        _jsAllocSetHint( _JS_ALLOCTYPE_BUFFER_OBJECT, name );
        GLboolean created = _jsPlatformCreateBufferObject( bufferObject );
        _jsAllocClearHint();
        if ( !created )
        {
            // XXX This error is not in the spec.
            _JS_PERFORMANCE_STOP( BufferSubDataCounter );
            _jsSetError( GL_OUT_OF_MEMORY );
            return;
        }
        _jsPlatformBufferObjectCopyData( bufferObject, oldBufferObject );
    }
    JS_ASSERT( bufferObject != NULL );

#ifndef JS_NO_ERROR_CHECK
    if (( size < 0 ) || ( offset < 0 ) || ( size + offset > bufferObject->size ) )
    {
        _JS_PERFORMANCE_STOP( BufferSubDataCounter );
        _jsSetError( GL_INVALID_VALUE );
        return;
    }
#endif
    _jsAllocSetHint( _JS_ALLOCTYPE_BUFFER_OBJECT, name );
    _jsPlatformBufferObjectSetData( bufferObject, offset, size, data, GL_FALSE );
    _jsAllocClearHint();
    _JS_PERFORMANCE_STOP( BufferSubDataCounter );
}


//
// DPM treatment of buffer objects follows this sequence of events:
// mapBufferCapture - store buffer characteristics as phantom call parameters
// unmapBufferCapture - allocate and store contents of buffer for use in replay
// mapBufferReplay - restore characteristics of buffer
// unmapBufferReplay - copy the stored buffer contents to the server

#ifdef JS_DPM_V1

static _JS_DPM_CALL *calls[] = { NULL, NULL, NULL, NULL, NULL }
                               ;	// one per target


// For each of the targets, keep track of one "current" call

static void putCurrentBuffer( GLenum target, _JS_DPM_CALL *call )
{
    switch ( target )
    {
        case GL_ARRAY_BUFFER:			calls[0] = call; break;
        case GL_ELEMENT_ARRAY_BUFFER:	calls[1] = call; break;
        case GL_PIXEL_PACK_BUFFER_ARB:	calls[2] = call; break;
        case GL_PIXEL_UNPACK_BUFFER_ARB:	calls[3] = call; break;
        case GL_TEXTURE_REFERENCE_BUFFER_SCE:	calls[4] = call; break;
    }	// switch
}


static _JS_DPM_CALL * getCurrentBuffer( GLenum target )
{
    switch ( target )
    {
        case GL_ARRAY_BUFFER:			return calls[0]; break;
        case GL_ELEMENT_ARRAY_BUFFER:	return calls[1]; break;
        case GL_PIXEL_PACK_BUFFER_ARB:	return calls[2]; break;
        case GL_PIXEL_UNPACK_BUFFER_ARB:	return calls[3]; break;
        case GL_TEXTURE_REFERENCE_BUFFER_SCE:	return calls[4]; break;
    }	// switch
    return NULL;
}



static void mapBufferCapture( GLenum target, GLenum access, jsBufferObject * bufferObject, void * address )
{
    if ( access != GL_WRITE_ONLY && access != GL_READ_WRITE )
    {
        _JS_DPM_CURRENT_ARGUMENT_PTR( 4 ) = NULL;
        putCurrentBuffer( target, _JS_DPM_CURRENT_CALL_STRUCT_PTR );
        return;
    }
    assert( sizeof( target ) <= sizeof( void * ) );
    assert( sizeof( bufferObject->size ) <= sizeof( void * ) );
    int size = bufferObject->size;
#ifdef JS_DPM_V2
    _JS_DPM_API_PROCESS_ARGUMENT( 3, size, sizeof( size ), INPUT_BY_VALUE, 0 ); // TODO:FIX THIS
    _JS_DPM_API_PROCESS_ARGUMENT( 4, address, sizeof( address ), INPUT_BY_VALUE, 0 ); // TODO:FIX THIS
#else
    _JS_DPM_API_CAPTURE_SINGLETON( 3, size, sizeof( size ), INPUT_BY_VALUE, 0 ); // TODO:FIX THIS
    _JS_DPM_API_CAPTURE_SINGLETON( 4, address, sizeof( address ), INPUT_BY_VALUE, 0 ); // TODO:FIX THIS
#endif
    putCurrentBuffer( target, _JS_DPM_CURRENT_CALL_STRUCT_PTR );
}


static void mapBufferReplay( GLenum target )
{
    if ( _JS_DPM_CURRENT_ARGUMENT_PTR( 4 ) == NULL )	return;
    putCurrentBuffer( target, _JS_DPM_CURRENT_CALL_STRUCT_PTR );
    int size;
    _JS_DPM_API_REPLAY_SINGLETON( 3, size, sizeof( size ), INPUT_BY_VALUE );
}


static void unmapBufferCapture( GLenum target )
{
    _JS_DPM_CALL * call = getCurrentBuffer( target );
    if ( call == NULL )	return;
    if ( _JS_DPM_CALL_ARGUMENT_PTR( call, 4 ) == NULL )	return;
    int size;
    int *iptr = ( int * )_JS_DPM_CALL_ARGUMENT_PTR( call, 3 );
    size = *iptr;
    _JS_DPM_TRACE_OFF
    void * temporaryBuffer = jsMalloc( size );
    _JS_DPM_TRACE_ON
    if ( temporaryBuffer == NULL )	_JS_DPM_ASSERT( "jsMalloc for DPM buffer storage" );
    void ** p = ( void ** )_JS_DPM_CALL_ARGUMENT_PTR( call, 4 );
    void *sourceAddress = *p;
    memcpy( temporaryBuffer, sourceAddress, size );
    _JS_DPM_CALL_ARGUMENT_PTR( call, 4 ) = temporaryBuffer;
    putCurrentBuffer( target, NULL );
}


static void unmapBufferReplay( GLenum target )
{
    _JS_DPM_CALL * call = getCurrentBuffer( target );
    if ( call == NULL )	return;
    if ( _JS_DPM_CALL_ARGUMENT_PTR( call, 4 ) == NULL )	return;
    int *iptr = ( int * )_JS_DPM_CALL_ARGUMENT_PTR( call, 3 );
    int size = *iptr;
    void * address = _JS_DPM_CALL_ARGUMENT_PTR( call, 4 );
    assert( _JS_DPM_STATE( REPLAY ) );
    _jsdpmState = _JS_DPM_STATE_CLEAR;
    glBufferSubData( target, 0, size, address );
    _jsdpmState = _JS_DPM_STATE_REPLAY;
    if ( address != NULL )
    {
        _JS_DPM_TRACE_OFF
//	   	jsFree(address);
        printf( "**** (%s, %d) %s: not freeing temp buffer at %p\n", __FILE__, __LINE__, __FUNCTION__, address );
        _JS_DPM_TRACE_ON
    }
    putCurrentBuffer( target, NULL );
}


#endif



/**
 *  @short Maps the entire store of a buffer object into the client memory address space
 *
glMapBuffer() lets you map the entire data store of a buffer object into the
client memory address space. The returned pointer is guaranteed to be a
128-byte aligned address, which is ideal for a DMA block transfer of data.

If <i><c>access</c></i> is <code>GL_WRITE_ONLY</code>, calling glMapBuffer() replaces
the existing buffer with a new buffer allocation.

If <i><c>access</c></i> is <code>GL_READ_ONLY</code> or <code>GL_READ_WRITE</code>,
calling glMapBuffer() forces the RSX&trade; to finish.

If <i><c>access</c></i> is <code>GL_WRITE_ONLY</code> or <code>GL_READ_WRITE</code>, calling glMapBuffer() reallocates a new buffer if a texture is bound to this
buffer.

@param target  Specifies the buffer object target.
Must be one of <CODE>GL_ARRAY_BUFFER</CODE>, <CODE>GL_ELEMENT_ARRAY_BUFFER</CODE>,
<CODE>GL_PIXEL_PACK_BUFFER_ARB</CODE>, <CODE>GL_PIXEL_UNPACK_BUFFER_ARB</CODE>, or <CODE>GL_TEXTURE_REFERENCE_BUFFER_SCE</CODE>.
@param access  Specifies the operations that may be performed on the data store.
Must be <CODE>GL_READ_ONLY</CODE>, <CODE>GL_WRITE_ONLY</CODE>, or <CODE>GL_READ_WRITE</CODE>.

@note
You do not have to unmap the buffer before issuing a drawing operation.
PSGL allows you to render the buffer data while the buffer is still mapped.
@par
If the GL is able to map the buffer object&rsquo;s data store into the client memory address space, glMapBuffer() returns the pointer value to the data store.  Otherwise glMapBuffer() returns <CODE>NULL</CODE> and the error <CODE>GL_OUT_OF_MEMORY</CODE> is generated.
@par
Note that leaving any buffers mapped writable causes the vertex cache to be invalidated on a draw call.
@note
For more information about using mapped buffer objects, refer to the "Mapped Buffer
Objects" section of the
<i>PlayStation&reg; Graphics Library Programming Guide</i>.

@return Returns a pointer to the mapped buffer or <CODE>NULL</CODE> in case of an error.
If a pointer is returned, it is guaranteed to be a 128-byte aligned address.
 *
 * @par Errors
Use glGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>GL_INVALID_ENUM</CODE></TD>
<TD><I><c>target</c></I> or <I><c>access</c></I> is not one of the allowable values.</TD>
</TR>
<TR>
<TD><CODE>GL_INVALID_OPERATION</CODE></TD>
<TD>The buffer store is already in the mapped state.</TD>
</TR>
<TR>
<TD><CODE>GL_OUT_OF_MEMORY</CODE></TD>
<TD>The GL is unable to map the data store and <CODE>NULL</CODE> is returned.</TD>
</TR>
</TABLE>
 *
 * @sa glUnmapBuffer, glBufferData, glBindBuffer, glDeleteBuffers, glGenBuffers, glTextureReferenceSCE
 */
GLAPI GLvoid* APIENTRY glMapBuffer( GLenum target, GLenum access )
{
    JS_APIENTRY_TIMER( glMapBuffer );
    if ( _jsdpmState )
    {
        _JS_DPM_API_ENTRY_2( glMapBuffer, JSDPM_CALL_TYPE_STATE_TEX, ( GLvoid * ) NULL, 2 + 2,
                             target, sizeof( GLenum ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM,
                             access, sizeof( GLenum ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM );
    }

    // check args and pass to the platform
    PSGLcontext *LContext = _CurrentContext;
    JS_ASSERT( LContext );
    GLuint name = 0;
    switch ( target )
    {
        case GL_ARRAY_BUFFER:
            name = LContext->ArrayBuffer;
            break;
        case GL_ELEMENT_ARRAY_BUFFER:
            name = LContext->attribs->ElementArrayBuffer;
            break;
        case GL_PIXEL_PACK_BUFFER_ARB:
            name = LContext->PixelPackBuffer;
            break;
        case GL_PIXEL_UNPACK_BUFFER_ARB:
            name = LContext->PixelUnpackBuffer;
            break;
        case GL_TEXTURE_REFERENCE_BUFFER_SCE:
            name = LContext->TextureBuffer;
            break;
        default:
            _jsSetError( GL_INVALID_ENUM );
            return NULL;
    }
#ifndef JS_NO_ERROR_CHECK
    if ( name == 0 )
    {
        _jsSetError( GL_INVALID_OPERATION );
        return NULL;
    }
#endif
    _JS_PERFORMANCE_START( MapBufferCounter );
    jsBufferObject* bufferObject = _jsGetBufferObject( LContext, name );

#ifndef JS_NO_ERROR_CHECK
    if ( bufferObject->mapped )
    {
        _JS_PERFORMANCE_STOP( MapBufferCounter );
        _jsSetError( GL_INVALID_OPERATION );
        return NULL;
    }
#endif

    switch ( access )
    {
        case GL_READ_ONLY:
            break;
        case GL_WRITE_ONLY:
        case GL_READ_WRITE:
            // recreate if multiply referenced
            //  This happens when a texture references a PBO.
            if ( bufferObject->refCount > 1 )
            {
                jsBufferObject* oldBufferObject = bufferObject;

                _jsTexNameSpaceDeleteNames( &LContext->bufferObjectNameSpace, 1, &name );
                _jsTexNameSpaceCreateNameLazy( &LContext->bufferObjectNameSpace, name );

                bufferObject = _jsGetBufferObject( LContext, name );
                bufferObject->size = oldBufferObject->size;
                bufferObject->usage = oldBufferObject->usage;

                // copy buffer
                _jsAllocSetHint( _JS_ALLOCTYPE_BUFFER_OBJECT, name );
                GLboolean created = _jsPlatformCreateBufferObject( bufferObject );
                _jsAllocClearHint();
                if ( !created )
                {
                    // XXX This error is not in the spec.
                    _JS_PERFORMANCE_STOP( MapBufferCounter );
                    _jsSetError( GL_OUT_OF_MEMORY );
                    return NULL;
                }
                _jsPlatformBufferObjectCopyData( bufferObject, oldBufferObject );
            }
            _jsAttribSetMapBuffer( LContext, name );
            break;
        default:
            _JS_PERFORMANCE_STOP( MapBufferCounter );
            _jsSetError( GL_INVALID_ENUM );
            return NULL;
    }

    JS_ASSERT( bufferObject != NULL );
    bufferObject->mapped = GL_TRUE;
    _jsAllocSetHint( _JS_ALLOCTYPE_BUFFER_OBJECT, name );
    void *result = _jsPlatformBufferObjectMap( bufferObject, access );
    _jsAllocClearHint();

#ifdef JS_DPM_V1
    if ( _JS_DPM_STATE( CAPTURE ) ) mapBufferCapture( target, access, bufferObject, result );
    else if ( _JS_DPM_STATE( REPLAY ) )	mapBufferReplay( target );
#endif

    _JS_PERFORMANCE_STOP( MapBufferCounter );
    return result;
}



/**
 *  @short Releases the data store mapped by glMapBuffer.
 *
glUnmapBuffer() releases the data store mapped by glMapBuffer().

@param target  Specifies the buffer object target.
Must be one of <CODE>GL_ARRAY_BUFFER</CODE>, <CODE>GL_ELEMENT_ARRAY_BUFFER</CODE>,
<CODE>GL_PIXEL_PACK_BUFFER_ARB</CODE>, <CODE>GL_PIXEL_UNPACK_BUFFER_ARB</CODE>, or <CODE>GL_TEXTURE_REFERENCE_BUFFER_SCE</CODE>.

@note
You do not have to unmap the buffer before issuing a drawing operation.
PSGL allows you to render the buffer data while the buffer is still mapped.
@par
Unmapping a mapped buffer object invalidates the pointers to its data store.
@par
glUnmapBuffer() returns <CODE>GL_TRUE</CODE> unless data values in the buffer&rsquo;s data store have become corrupted during the period that the buffer was mapped.  Such corruption can be the result of a screen resolution change or other window-system-dependent event that causes system heaps such as those for high-performance graphics memory to be discarded.  If such corruption has occurred, glUnmapBuffer() returns <CODE>GL_FALSE</CODE>, and the contents of the buffer&rsquo;s data store become undefined.

@return Returns <CODE>GL_TRUE</CODE> unless data values in the buffer&rsquo;s data store have become corrupted while the buffer was mapped, <CODE>GL_FALSE</CODE> otherwise.
 *
@note
For more information about using mapped buffer objects, refer to the "Mapped Buffer
Objects" section of the
<i>PlayStation&reg; Graphics Library Programming Guide</i>.

 * @par Errors
Use glGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>GL_INVALID_ENUM</CODE></TD>
<TD><I><c>target</c></I> is not one of the allowable values.</TD>
</TR>
<TR>
<TD><CODE>GL_INVALID_OPERATION</CODE></TD>
<TD><I><c>target</c></I> is not in the mapped state.</TD>
</TR>
</TABLE>
 *
 * @sa glMapBuffer, glBufferData, glBindBuffer, glDeleteBuffers, glGenBuffers
 */
GLAPI GLboolean APIENTRY glUnmapBuffer( GLenum target )
{
    JS_APIENTRY_TIMER( glUnmapBuffer );
    if ( _jsdpmState ) { _JS_DPM_API_ENTRY_1( glUnmapBuffer, JSDPM_CALL_TYPE_STATE_TEX, ( GLboolean ) 0, 1, target, sizeof( GLenum ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM ); }

    // check args and pass to the platform
    PSGLcontext *LContext = _CurrentContext;
    JS_ASSERT( LContext );
    GLuint name = 0;
    switch ( target )
    {
        case GL_ARRAY_BUFFER:
            name = LContext->ArrayBuffer;
            break;
        case GL_ELEMENT_ARRAY_BUFFER:
            name = LContext->attribs->ElementArrayBuffer;
            break;
        case GL_PIXEL_PACK_BUFFER_ARB:
            name = LContext->PixelPackBuffer;
            break;
        case GL_PIXEL_UNPACK_BUFFER_ARB:
            name = LContext->PixelUnpackBuffer;
            break;
        case GL_TEXTURE_REFERENCE_BUFFER_SCE:
            name = LContext->TextureBuffer;
            break;
        default:
            _jsSetError( GL_INVALID_ENUM );
            return GL_FALSE;
    }
#ifndef JS_NO_ERROR_CHECK
    if ( name == 0 )
    {
        _jsSetError( GL_INVALID_OPERATION );
        return GL_FALSE;
    }
#endif
    _JS_PERFORMANCE_START( UnmapBufferCounter );
    jsBufferObject* bufferObject = _jsGetBufferObject( LContext, name );

#ifndef JS_NO_ERROR_CHECK
    if ( !bufferObject->mapped )
    {
        _JS_PERFORMANCE_STOP( UnmapBufferCounter );
        _jsSetError( GL_INVALID_OPERATION );
        return GL_FALSE;
    }
#endif

    bufferObject->mapped = GL_FALSE;

#ifdef JS_DPM_V1
    if ( _JS_DPM_STATE( CAPTURE ) )	unmapBufferCapture( target );
#endif

    GLboolean result = _jsPlatformBufferObjectUnmap( bufferObject );

#ifdef JS_DPM_V1
    if ( _JS_DPM_STATE( REPLAY ) ) unmapBufferReplay( target );
#endif

    _JS_PERFORMANCE_STOP( UnmapBufferCounter );
    return result;
}

/** @} OpenGL */
