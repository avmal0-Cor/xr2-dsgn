/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 *                Copyright (C) 2009 Sony Computer Entertainment Inc.
 *                                               All Rights Reserved.
 */

 
/*
 * OES_framebuffer_object extension
 */

#include <string.h>
#include <PSGL/psgl.h>
#include <PSGL/private.h>
#include <PSGL/DPM2.h>
#include <PSGL/Debug.h>

void _jsFramebufferGetAttachmentTexture(
    PSGLcontext* LContext,
    const jsFramebufferAttachment* attachment,
    jsTexture** texture,
    GLuint* face )
{
    JS_ASSERT( LContext != NULL );
    JS_ASSERT( attachment != NULL );
    JS_ASSERT( texture != NULL );
    JS_ASSERT( face != NULL );

    switch ( attachment->type )
    {
        case _JS_FRAMEBUFFER_ATTACHMENT_NONE:
            *texture = NULL;
            *face = 0;
            break;
        case _JS_FRAMEBUFFER_ATTACHMENT_RENDERBUFFER:
            *texture = _jsGetRenderbufferSafe( LContext, attachment->name );
            *face = 0;
            break;
        case _JS_FRAMEBUFFER_ATTACHMENT_TEXTURE:
            *texture = _jsGetTextureSafe( LContext, attachment->name );
            if ( *texture )
            {
                switch ( attachment->textureTarget )
                {
                    case GL_TEXTURE_2D:
                        *face = 0;
                        break;
                    case GL_TEXTURE_CUBE_MAP_POSITIVE_X:
                    case GL_TEXTURE_CUBE_MAP_NEGATIVE_X:
                    case GL_TEXTURE_CUBE_MAP_POSITIVE_Y:
                    case GL_TEXTURE_CUBE_MAP_NEGATIVE_Y:
                    case GL_TEXTURE_CUBE_MAP_POSITIVE_Z:
                    case GL_TEXTURE_CUBE_MAP_NEGATIVE_Z:
                        *face = attachment->textureTarget - GL_TEXTURE_CUBE_MAP_POSITIVE_X;
                        break;
                    default:
                        *face = 0;
                        *texture = NULL;
                        JS_ASSERT( 0 );
                }
            }
            else *face = 0;
            break;
        default:
            *face = 0;
            *texture = NULL;
            JS_ASSERT( 0 );
    }
}

jsFramebufferAttachment* _jsFramebufferGetAttachment( jsFramebuffer *framebuffer, GLenum attachment )
{
    switch ( attachment )
    {
        case GL_COLOR_ATTACHMENT0_EXT:
        case GL_COLOR_ATTACHMENT1_EXT:
        case GL_COLOR_ATTACHMENT2_EXT:
        case GL_COLOR_ATTACHMENT3_EXT:
            return &framebuffer->color[attachment - GL_COLOR_ATTACHMENT0_EXT];
        case GL_DEPTH_ATTACHMENT_OES:
            return &framebuffer->depth;
        case GL_STENCIL_ATTACHMENT_OES:
            return &framebuffer->stencil;
        default:
            _jsSetError( GL_INVALID_ENUM );
            return NULL;
    }
}

void _jsGetFramebufferSize( GLuint* width, GLuint* height )
{
    PSGLcontext*	LContext = _CurrentContext;
    JS_ASSERT( LContext );

    *width = *height = 0;
    if ( LContext->framebuffer )
    {
        jsFramebuffer* framebuffer = _jsGetFramebuffer( LContext, LContext->framebuffer );
        JS_ASSERT( framebuffer != NULL );

        if ( _jsPlatformFramebufferCheckStatus( framebuffer ) != GL_FRAMEBUFFER_COMPLETE_OES )
        {
            return;
        }

        GLuint xscale = 1;
        GLuint yscale = 1;
        switch ( framebuffer->multisampling )
        {
            case GL_MULTISAMPLING_2X_DIAGONAL_CENTERED_SCE:
                xscale = 2;
                break;
            case GL_MULTISAMPLING_4X_SQUARE_CENTERED_SCE:
            case GL_MULTISAMPLING_4X_SQUARE_ROTATED_SCE:
                yscale = xscale = 2;
                break;
            default:
                break;
        }

        for ( int i = 0; i < _JS_MAX_COLOR_ATTACHMENTS; ++i )
        {
            // get the texture and face
            jsTexture* colorTexture = NULL;
            GLuint face = 0;
            _jsFramebufferGetAttachmentTexture( LContext, &framebuffer->color[i], &colorTexture, &face );
            if ( colorTexture == NULL ) continue;
            *width = MIN( *width, colorTexture->images[face][0].width / xscale );
            *height = MIN( *height, colorTexture->images[face][0].height / yscale );
        }

        jsTexture* depthTexture;
        GLuint depthFace = 0;
        _jsFramebufferGetAttachmentTexture( LContext, &framebuffer->depth, &depthTexture, &depthFace );
        if ( depthTexture == NULL )
        {
            // stencil ?
            _jsFramebufferGetAttachmentTexture( LContext, &framebuffer->stencil, &depthTexture, &depthFace );
        }
        if ( depthTexture != NULL )
        {
            *width = MIN( *width, depthTexture->images[depthFace][0].width / xscale );
            *height = MIN( *height, depthTexture->images[depthFace][0].height / yscale );
        }
    }
    else	// use default framebuffer
    {
        PSGLdevice *LDevice = _CurrentDevice;
        *width = LDevice->deviceParameters.width;
        *height = LDevice->deviceParameters.height;
    }
}

// API functions ----------------------------------------


/** @addtogroup OpenGL
 *@{
 */
/**
@short Test for render buffer existence

glIsRenderbufferOES() tests a name to determine if it represents
a render buffer.

@param       renderbuffer Specifies the render buffer name.

@return Returns <CODE>GL_TRUE</CODE> if <I><c>renderbuffer</c></I> is the
name of a render buffer object and returns <CODE>GL_FALSE</CODE> otherwise.

@sa     glBindRenderbufferOES
 */
GLAPI GLboolean APIENTRY glIsRenderbufferOES( GLuint renderbuffer )
{
    JS_APIENTRY_TIMER( glIsRenderbufferOES );
#ifdef JS_DPM
    if ( _jsdpmState )
    {
        _JS_DPM_API_ENTRY_1( glIsRenderbufferOES, JSDPM_CALL_TYPE_STATE_BUFFER, GL_TRUE, 1,
                             renderbuffer, sizeof( GLuint ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE );
    }
#endif

    PSGLcontext* LContext = _CurrentContext;
    JS_ASSERT( LContext != NULL );

    // test if the name itself is valid
    if ( !_jsTexNameSpaceIsName( &LContext->renderbufferNameSpace, renderbuffer ) )
        return GL_FALSE;

    return GL_TRUE;
}

/**
@short Bind a named render buffer to the render buffer target

glBindRenderbufferOES() lets you create or use a named render buffer, and bind
it to the render buffer target.  When a render buffer is bound to the target,
the previous binding for that target is automatically broken.

Render Buffer names are unsigned integers.  The value 0 is reserved to
remove any existing binding, which is the initial state.

A new render buffer is created if <I><c>renderbuffer</c></I> does not exist.

@param       target     Must be <CODE>GL_RENDERBUFFER_OES</CODE>.

@param       renderbuffer Specifies the name of a render buffer.

@par Errors
Use glGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>GL_INVALID_ENUM</CODE></TD>
<TD><I><c>target</c></I> is not GL_RENDERBUFFER_OES.</TD>
</TR>
</TABLE>

@sa glGenRenderbuffersOES, glRenderbufferStorageOES, glFramebufferRenderbufferOES
 */
GLAPI void APIENTRY glBindRenderbufferOES( GLenum target, GLuint renderbuffer )
{
    JS_APIENTRY_TIMER( glBindRenderbufferOES );
#ifdef JS_DPM
    if ( _jsdpmState )
    {
        _JS_DPM_API_ENTRY_2( glBindRenderbufferOES, JSDPM_CALL_TYPE_STATE_BUFFER, , 2,
                             target, sizeof( GLenum ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM,
                             renderbuffer, sizeof( GLuint ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE );
    }
#endif

    PSGLcontext* LContext = _CurrentContext;
    JS_ASSERT( LContext != NULL );

#ifndef JS_NO_ERROR_CHECK
    switch ( target )
    {
        case GL_RENDERBUFFER_OES:
            break;
        default:
            _jsSetError( GL_INVALID_ENUM );
            return;
    }
#endif

    if ( renderbuffer )
        _jsTexNameSpaceCreateNameLazy( &LContext->renderbufferNameSpace, renderbuffer );

    LContext->renderbuffer = renderbuffer;
}

/**
@short Delete render buffer names

glDeleteRenderbuffersOES() deletes <I><c>n</c></I> render buffers named by the elements
of the array <I><c>renderbuffers</c></I>.  After a render buffer is deleted, it has
no contents or dimensionality, and its name is free for reuse (for example
by glGenRenderbuffersOES()).  If a render buffer that is currently bound is
deleted, the binding reverts to 0 (the absence of any render buffer object). 
However, it is recommended that you bind the buffer to 0 explicitly with 
<CODE>glBindBuffer(target, 0)</CODE> as part of deleting the render buffer. 
Additionally 
if the image of the render buffer is attached to the currently bound frame buffer 
object, the render buffer is automatically detached. Render buffer 
attachments to any other frame buffer objects are the responsibility of the application.

glDeleteRenderbuffersOES() silently ignores 0&rsquo;s and names that do not
correspond to existing render buffers.

@param       n Specifies the number of render buffer names to be deleted.

@param       renderbuffers Specifies an array of render buffers to be deleted.

@par Errors
Use glGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>GL_INVALID_VALUE</CODE></TD>
<TD><I><c>n</c></I> is negative.</TD>
</TR>
</TABLE>

@sa glBindRenderbufferOES, glGenRenderbuffersOES
 */
GLAPI void APIENTRY glDeleteRenderbuffersOES( GLsizei n, const GLuint* renderbuffers )
{
    JS_APIENTRY_TIMER( glDeleteRenderbuffersOES );
#ifdef JS_DPM
    if ( _jsdpmState )
    {
        _JS_DPM_API_ENTRY_2( glDeleteRenderbuffersOES, JSDPM_CALL_TYPE_STATE_BUFFER, , 2,
                             n, sizeof( GLsizei ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             renderbuffers, ( n * sizeof( GLuint ) ), INPUT_BY_REFERENCE, DPM_API_ARGUMENT_POINTER );
    }
#endif

    PSGLcontext *LContext = _CurrentContext;
    JS_ASSERT( LContext );

    _jsTexNameSpaceDeleteNames( &LContext->renderbufferNameSpace, n, renderbuffers );
}

/**
@short Generate render buffer names

glGenRenderbuffersOES() returns <I><c>n</c></I> render buffer names in
<I><c>renderbuffers</c></I>.  There is no guarantee that the names form a contiguous
set of integers.  However, it is guaranteed that none of the returned names
was in use immediately before the call to glGenRenderbuffersOES().

Render buffer names returned by a call to glGenRenderbuffersOES() are not
returned by subsequent calls unless they are first deleted with
glDeleteRenderbuffersOES().

@param       n Specifies the number of render buffer names to be generated.

@param       renderbuffers Specifies an array in which the generated
render buffer names are stored.

@par Errors
Use glGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>GL_INVALID_VALUE</CODE></TD>
<TD><I><c>n</c></I> is negative.</TD>
</TR>
</TABLE>

@sa glBindRenderbufferOES, glDeleteRenderbuffersOES, glRenderbufferStorageOES, glFramebufferRenderbufferOES
 */
GLAPI void APIENTRY glGenRenderbuffersOES( GLsizei n, GLuint *renderbuffers )
{
    JS_APIENTRY_TIMER( glGenRenderbuffersOES );
#ifdef JS_DPM
    if ( _jsdpmState )
    {
        _JS_DPM_API_ENTRY_2( glGenRenderbuffersOES, JSDPM_CALL_TYPE_STATE_BUFFER, , 2,
                             n, sizeof( GLsizei ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             renderbuffers, ( n * sizeof( GLuint ) ), INPUT_OUTPUT, DPM_API_ARGUMENT_POINTER );
    }
#endif

    PSGLcontext *LContext = _CurrentContext;
    JS_ASSERT( LContext );
    _jsTexNameSpaceGenNames( &LContext->renderbufferNameSpace, n, renderbuffers );
}

/**
@short Establish storage for a render buffer.

glRenderbufferStorageOES() specifies format and dimensions and allocates storage
for a render buffer.

@param target Must be GL_RENDERBUFFER_OES.

@param internalFormat Requests the internal format of the render buffer.  Must be
one of
<CODE>GL_RGB</CODE>, <CODE>GL_RGBA</CODE>, <CODE>GL_RGB5</CODE>, <CODE>GL_RGB5_A1</CODE>,
<CODE>GL_RGBA8</CODE>, <CODE>GL_ARGB_SCE</CODE>,
<CODE>GL_RGBA16F_ARB</CODE>, <CODE>GL_RGBA32F_ARB</CODE>,
<CODE>GL_DEPTH_COMPONENT</CODE>, <CODE>GL_DEPTH_COMPONENT16</CODE>, 
<CODE>GL_DEPTH_COMPONENT24</CODE>, or <CODE>GL_STENCIL_INDEX</CODE>.

@param width Specifies the width of the render buffer.  Must be greater than 0
and less than or equal to the returned value of <CODE>GL_MAX_RENDERBUFFER_SIZE_OES</CODE>.

@param height Specifies the height of the render buffer.  Must be greater than
0 and less than or equal to the returned value of <CODE>GL_MAX_RENDERBUFFER_SIZE_OES</CODE>.

@note
Although a render buffer may be specified with a valid internal format, it
is not guaranteed to form a supported frame buffer on a particular
implementation.  Recommended formats are <CODE>GL_ARGB_SCE</CODE>,
<CODE>GL_RGBA16F_ARB</CODE>, or <CODE>GL_RGBA32F_ARB</CODE> for
color attachments, <CODE>GL_DEPTH_COMPONENT</CODE> for depth attachments,
and <CODE>GL_STENCIL_INDEX</CODE> for stencil attachments.

@par Errors
Use glGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>GL_INVALID_OPERATION</CODE></TD>
<TD>A nonzero name is not bound to <CODE>GL_RENDERBUFFER_OES</CODE>.</TD>
</TR>
<TR>
<TD><CODE>GL_INVALID_ENUM</CODE></TD>
<TD><I><c>target</c></I> is not <CODE>GL_RENDERBUFFER_OES</CODE>.</TD>
</TR>
<TR>
<TD><CODE>GL_INVALID_ENUM</CODE></TD>
<TD><I><c>internalFormat</c></I> is not one of <CODE>GL_RGB</CODE>, <CODE>GL_RGBA</CODE>, <CODE>GL_RGB5</CODE>,
<CODE>GL_RGB5_A1</CODE>,
<CODE>GL_RGBA8</CODE>, <CODE>GL_ARGB_SCE</CODE>,
<CODE>GL_RGBA16F_ARB</CODE>, <CODE>GL_RGBA32F_ARB</CODE>, <CODE>GL_DEPTH_COMPONENT</CODE>, 
<CODE>GL_DEPTH_COMPONENT16</CODE>, <CODE>GL_DEPTH_COMPONENT24</CODE>,
or <CODE>GL_STENCIL_INDEX</CODE>.</TD>
</TR>
<TR>
<TD><CODE>GL_INVALID_VALUE</CODE></TD>
<TD><I><c>width</c></I> or <I><c>height</c></I>
is less than 0 or greater than the returned value of <CODE>GL_MAX_RENDERBUFFER_SIZE_OES</CODE>.</TD>
</TR>
<TR>
<TD><CODE>GL_OUT_OF_MEMORY</CODE></TD>
<TD>There is insufficient memory to allocate the render buffer.</TD>
</TR>
</TABLE>

@sa glBindRenderbufferOES, glFramebufferRenderbufferOES
 */
GLAPI void APIENTRY glRenderbufferStorageOES( GLenum target, GLenum internalFormat, GLsizei width, GLsizei height )
{
    JS_APIENTRY_TIMER( glRenderbufferStorageOES );
#ifdef JS_DPM
    if ( _jsdpmState )
    {
        _JS_DPM_API_ENTRY_4( glRenderbufferStorageOES, JSDPM_CALL_TYPE_STATE_BUFFER, , 4,
                             target, sizeof( GLenum ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM,
                             internalFormat, sizeof( GLenum ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM,
                             width, sizeof( GLsizei ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             height, sizeof( GLsizei ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE );
    }
#endif

    PSGLcontext *LContext = _CurrentContext;
    JS_ASSERT( LContext );

#ifndef JS_NO_ERROR_CHECK
    switch ( target )
    {
        case GL_RENDERBUFFER_OES:
            break;
        default:
            _jsSetError( GL_INVALID_ENUM );
            return;
    }
#endif

    GLenum format;
    GLenum type;
    switch ( internalFormat )
    {
        case GL_RGB:
            format = GL_RGB;
            type = GL_UNSIGNED_BYTE;
            break;
        case GL_RGBA:
        case GL_RGBA8:
            format = GL_RGBA;
            type = GL_UNSIGNED_BYTE;
            break;
        case GL_ARGB_SCE:
            format = GL_BGRA;
            type = GL_UNSIGNED_INT_8_8_8_8_REV;
            break;
        case GL_RGBA16F_ARB:
            format = GL_RGBA;
            type = GL_HALF_FLOAT_ARB;
            break;
        case GL_RGBA32F_ARB:
            format = GL_RGBA;
            type = GL_FLOAT;
            break;
        case GL_DEPTH_COMPONENT16:
            format = GL_DEPTH_COMPONENT;
            type = GL_UNSIGNED_BYTE;
            break;
        case GL_DEPTH_COMPONENT:
        case GL_DEPTH_COMPONENT24:
            format = GL_DEPTH_COMPONENT;
            type = GL_UNSIGNED_BYTE;
            break;
        case GL_STENCIL_INDEX:
            format = GL_STENCIL_INDEX;
            type = GL_UNSIGNED_BYTE;
            break;
        default:
            _jsSetError( GL_INVALID_ENUM );
            return;
    }

#ifndef JS_NO_ERROR_CHECK
    // check dimensions
    const int maxDim = _jsPlatformRenderbufferMaxSize();
    if ( width <= 0 ||
            width > maxDim ||
            height <= 0 ||
            height > maxDim )
    {
        _jsSetError( GL_INVALID_VALUE );
        return;
    }


    // check that a renderbuffer name is bound
    if ( LContext->renderbuffer == 0 )
    {
        _jsSetError( GL_INVALID_OPERATION );
        return;
    }
#endif

    // get renderbuffer
    //  A render buffer is implemented as an unnamed texture.
    jsTexture* renderbuffer = _jsGetRenderbuffer( LContext, LContext->renderbuffer );
    JS_ASSERT( renderbuffer != NULL );
    renderbuffer->target = GL_TEXTURE_2D;
    renderbuffer->faceCount = 1;
    renderbuffer->imageCount = 1;
    renderbuffer->minFilter = GL_NEAREST;	// no mipmap access, otherwise incomplete
    renderbuffer->magFilter = GL_NEAREST;
    renderbuffer->usage = GL_TEXTURE_TILED_GPU_SCE;
    renderbuffer->revalidate = _JS_TEXTURE_REVALIDATE_IMAGES;

    // get image
    if ( renderbuffer->images[0] == NULL )
    {
        renderbuffer->images[0] = ( jsImage * )jsMalloc( sizeof( jsImage ) );
        memset( renderbuffer->images[0], 0, sizeof( jsImage ) );
    }

    // initialize storage
    _jsSetImage(
        renderbuffer->images[0],
        internalFormat,
        width, height, 1,
        0,					// alignment
        format, type,
        NULL );

    _jsTextureTouchFBOs( renderbuffer );
}

/**
@short Get render buffer parameters

glGetRenderbufferParameterivOES() returns the state of the currently bound
render buffer.

@param target Must be <CODE>GL_RENDERBUFFER_OES</CODE>.

@param pname Specifies the value to be returned.  Must be one of
<CODE>GL_RENDERBUFFER_WIDTH_OES</CODE>,
<CODE>GL_RENDERBUFFER_HEIGHT_OES</CODE>, or
<CODE>GL_RENDERBUFFER_INTERNAL_FORMAT_OES</CODE>.

@param params Specifies an address for the returned parameter value.

@par Errors
Use glGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>GL_INVALID_OPERATION</CODE></TD>
<TD>No render buffer is bound.</TD>
</TR>
<TR>
<TD><CODE>GL_INVALID_ENUM</CODE></TD>
<TD><I><c>target</c></I> is not <CODE>GL_RENDERBUFFER_OES</CODE>.</TD>
</TR>
<TR>
<TD><CODE>GL_INVALID_ENUM</CODE></TD>
<TD><I><c>pname</c></I> is not one of
<CODE>GL_RENDERBUFFER_WIDTH_OES</CODE>,
<CODE>GL_RENDERBUFFER_HEIGHT_OES</CODE>, or
<CODE>GL_RENDERBUFFER_INTERNAL_FORMAT_OES</CODE>.</TD>
</TR>
</TABLE>

@sa glRenderbufferStorageOES, glBindRenderbufferOES
 */
GLAPI void APIENTRY glGetRenderbufferParameterivOES( GLenum target, GLenum pname, GLint *params )
{
    JS_APIENTRY_TIMER( glGetRenderbufferParameterivOES );
#ifdef JS_DPM
    if ( _jsdpmState )
    {
        _JS_DPM_API_ENTRY_3( glGetRenderbufferParameterivOES, JSDPM_CALL_TYPE_STATE_BUFFER, , 3,
                             target, sizeof( GLenum ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM,
                             pname, sizeof( GLenum ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM,
                             params, sizeof( GLint ), INPUT_OUTPUT, DPM_API_ARGUMENT_POINTER );
    }
#endif

    PSGLcontext *LContext = _CurrentContext;
    JS_ASSERT( LContext );

#ifndef JS_NO_ERROR_CHECK
    switch ( target )
    {
        case GL_RENDERBUFFER_OES:
            break;
        default:
            _jsSetError( GL_INVALID_ENUM );
            return;
    }

    if ( LContext->renderbuffer == 0 )
    {
        _jsSetError( GL_INVALID_OPERATION );
        return;
    }
#endif

    // get renderbuffer
    //  A render buffer is implemented as an unnamed texture.
    jsTexture* renderbuffer = _jsGetRenderbuffer( LContext, LContext->renderbuffer );
    jsImage* image = renderbuffer != NULL ? renderbuffer->images[0] : NULL;
    switch ( pname )
    {
        case GL_RENDERBUFFER_WIDTH_OES:
            if ( image )
                *params = image->width;
            else
                *params = 0;
            break;
        case GL_RENDERBUFFER_HEIGHT_OES:
            if ( image )
                *params = image->height;
            else
                *params = 0;
            break;
        case GL_RENDERBUFFER_INTERNAL_FORMAT_OES:
            if ( image )
                *params = _jsPlatformTranslateTextureFormat( image->internalFormat );
            else
                *params = 0;
            break;
        default:
            _jsSetError( GL_INVALID_ENUM );
            return;
    }
}

/**
@short Test for frame buffer existence

glIsFramebufferOES() tests a name to determine if it represents
a frame buffer.  A frame buffer is created when an unused name is
bound for the first time.

@param       framebuffer Specifies the frame buffer name.

@return Returns <CODE>GL_TRUE</CODE> if <I><c>framebuffer</c></I> is the
name of a frame buffer object and returns <CODE>GL_FALSE</CODE> otherwise.

@sa     glBindFramebufferOES
 */
GLAPI GLboolean APIENTRY glIsFramebufferOES( GLuint framebuffer )
{
    JS_APIENTRY_TIMER( glIsFramebufferOES );
#ifdef JS_DPM
    if ( _jsdpmState )
    {
        _JS_DPM_API_ENTRY_1( glIsFramebufferOES, JSDPM_CALL_TYPE_GET, GL_TRUE, 1,
                             framebuffer, sizeof( GLuint ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE );
    }
#endif

    PSGLcontext* LContext = _CurrentContext;
    JS_ASSERT( LContext != NULL );

    // test if the name itself is valid
    if ( !_jsTexNameSpaceIsName( &LContext->framebufferNameSpace, framebuffer ) )
        return GL_FALSE;

    return GL_TRUE;
}

/**
@short Bind a named frame buffer to the frame buffer target

glBindFramebufferOES() lets you create or use a named frame buffer, and bind
it to the frame buffer target.  When a frame buffer is bound to the target,
the previous binding for that target is automatically broken.

Frame buffer names are unsigned integers.  The value 0 is reserved to
remove any existing binding, which is the initial state of drawing to the
device frame buffer.

A new frame buffer is created if <I><c>framebuffer</c></I> does not exist.

@param       target     Must be <CODE>GL_FRAMEBUFFER_OES</CODE>.

@param       framebuffer Specifies the name of a frame buffer.

@par Errors
Use glGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>GL_INVALID_ENUM</CODE></TD>
<TD><I><c>target</c></I> is not GL_FRAMEBUFFER_OES.</TD>
</TR>
</TABLE>

@sa glGenFramebuffersOES
 */
GLAPI void APIENTRY glBindFramebufferOES( GLenum target, GLuint framebuffer )
{
    JS_APIENTRY_TIMER( glBindFramebufferOES );
#ifdef JS_DPM
    if ( _jsdpmState )
    {
        _JS_DPM_API_ENTRY_2( glBindFramebufferOES, JSDPM_CALL_TYPE_STATE_BUFFER, , 2,
                             target, sizeof( GLenum ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM,
                             framebuffer, sizeof( GLuint ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE );
    }
#endif

    PSGLcontext* LContext = _CurrentContext;
    JS_ASSERT( LContext != NULL );

#ifndef JS_NO_ERROR_CHECK
    switch ( target )
    {
        case GL_FRAMEBUFFER_OES:
            break;
        default:
            _jsSetError( GL_INVALID_ENUM );
            return;
    }
#endif

    if ( framebuffer )
        _jsTexNameSpaceCreateNameLazy( &LContext->framebufferNameSpace, framebuffer );

    LContext->framebuffer = framebuffer;
    LContext->needValidate |= PSGL_VALIDATE_SCISSOR_BOX | PSGL_VALIDATE_FRAMEBUFFER;
}

/**
@short Delete frame buffer names

glDeleteFramebuffersOES() deletes <I><c>n</c></I> frame buffers named by the elements
of the array <I><c>framebuffers</c></I>.  After a frame buffer is deleted, it has
no contents or dimensionality, and its name is free for reuse (for example
by glGenFramebuffersOES()).  If a frame buffer that is currently bound is
deleted, the binding reverts to 0.

glDeleteFramebuffersOES() silently ignores 0&rsquo;s and names that do not
correspond to existing frame buffers.

@param       n Specifies the number of frame buffer names to be deleted.

@param       framebuffers Specifies an array of frame buffers to be deleted.

@par Errors
Use glGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>GL_INVALID_VALUE</CODE></TD>
<TD><I><c>n</c></I> is negative.</TD>
</TR>
</TABLE>

@sa glBindFramebufferOES, glGenFramebuffersOES
 */
GLAPI void APIENTRY glDeleteFramebuffersOES( GLsizei n, const GLuint *framebuffers )
{
    JS_APIENTRY_TIMER( glDeleteFramebuffersOES );
#ifdef JS_DPM
    if ( _jsdpmState )
    {
        _JS_DPM_API_ENTRY_2( glDeleteFramebuffersOES, JSDPM_CALL_TYPE_STATE_BUFFER, , 2,
                             n, sizeof( GLsizei ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             framebuffers, ( n * sizeof( GLuint ) ), INPUT_BY_REFERENCE, DPM_API_ARGUMENT_POINTER );
    }
#endif

    JS_ASSERT( framebuffers != NULL );

    PSGLcontext *LContext = _CurrentContext;
    JS_ASSERT( LContext );

    // if active framebuffer is deleted, bind default
    for ( int i = 0; i < n; ++i )
    {
        if ( framebuffers[i] && framebuffers[i] == LContext->framebuffer )
        {
            _JS_DPM_TRACE_OFF
            glBindFramebufferOES( GL_FRAMEBUFFER_OES, 0 );
            _JS_DPM_TRACE_ON
        }
    }

    _jsTexNameSpaceDeleteNames( &LContext->framebufferNameSpace, n, framebuffers );
}

/**
@short Generates frame buffer names

glGenFramebuffersOES() returns <I><c>n</c></I> frame buffer names in
<I><c>framebuffers</c></I>.  There is no guarantee that the names form a contiguous
set of integers.  However, it is guaranteed that none of the returned names
was in use immediately before the call to glGenFramebuffersOES().

Frame buffer names returned by a call to glGenFramebuffersOES() are not
returned by subsequent calls unless they are first deleted with
glDeleteFramebuffersOES().

@param       n Specifies the number of frame buffer names to be generated.

@param       framebuffers Specifies an array in which the generated
frame buffer names are stored.

@par Errors
Use glGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>GL_INVALID_VALUE</CODE></TD>
<TD><I><c>n</c></I> is negative.</TD>
</TR>
</TABLE>

@sa glBindFramebufferOES, glDeleteFramebuffersOES
 */
GLAPI void APIENTRY glGenFramebuffersOES( GLsizei n, GLuint *framebuffers )
{
    JS_APIENTRY_TIMER( glGenFramebuffersOES );
#ifdef JS_DPM
    if ( _jsdpmState )
    {
        _JS_DPM_API_ENTRY_2( glGenFramebuffersOES, JSDPM_CALL_TYPE_STATE_BUFFER, , 2,
                             n, sizeof( GLsizei ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             framebuffers, ( n * sizeof( GLuint ) ), INPUT_OUTPUT, DPM_API_ARGUMENT_POINTER );
    }
#endif

    PSGLcontext *LContext = _CurrentContext;
    JS_ASSERT( LContext );
    _jsTexNameSpaceGenNames( &LContext->framebufferNameSpace, n, framebuffers );
}

/**
@short Check frame buffer status

glCheckFramebufferStatusOES() checks if the currently bound frame buffer
has a valid and supported configuration for rendering.  Such a
configuration is termed <I>frame buffer complete</I>.

A frame buffer binding of 0 references the device frame buffer, which is
always complete.  A nonzero frame buffer binding is complete if its
attachments meet certain conditions.
<ul>
<li>Each attachment that is not 0 must be a valid render buffer or texture
and must have nonzero width and height.</li>
<li>For attachment <CODE>GL_COLOR_ATTACHMENT0_EXT</CODE>
through <CODE>GL_COLOR_ATTACHMENTn_EXT</CODE>, the attachment must
have an internal format of <CODE>GL_ARGB_SCE</CODE>, <CODE>GL_RGB</CODE>, <CODE>GL_LUMINANCE32F_ARB</CODE>, <CODE>GL_RGBA16F_ARB</CODE>,
or <CODE>GL_RGBA32F_ARB</CODE>.  For attachment <CODE>GL_DEPTH_ATTACHMENT_OES</CODE>,
the attachment must have an internal format of <CODE>GL_DEPTH_COMPONENT16</CODE>
or <CODE>GL_DEPTH_COMPONENT24</CODE>.  For attachment <CODE>GL_STENCIL_ATTACHMENT_OES</CODE>,
the attachment must have an internal format of <CODE>GL_STENCIL_INDEX</CODE>.<li>
<li>A frame buffer must have at least one nonzero attachment to be complete.</li>
<li>If one of the attachments has a format of <CODE>GL_TEXTURE_SWIZZLED_GPU_SCE</CODE> or <CODE>GL_TEXTURE_SWIZZLED_SYSTEM_SCE</CODE>,
all attachments must be one of those two formats.  <CODE>GL_TEXTURE_SWIZZLED_GPU_SCE</CODE> attachments can be
mixed with <CODE>GL_TEXTURE_SWIZZLED_SYSTEM_SCE</CODE> attachments.
Note that <CODE>GL_TEXTURE_SWIZZLED_GPU_SCE</CODE> is the default.</li>
<li>If the attachments have a format of <CODE>GL_TEXTURE_SWIZZLED_GPU_SCE</CODE> or
<CODE>GL_TEXTURE_SWIZZLED_SYSTEM_SCE</CODE>, all non-zero attachments must be the same size, and their internal format must be 32 bits per pixels or less.</li>
<li>All attachments
to <CODE>GL_COLOR_ATTACHMENT0_EXT</CODE> through <CODE>GL_COLOR_ATTACHMENTn_EXT</CODE>
must have the same internal format.</li>
</ul>

@param target   Must be <CODE>GL_FRAMEBUFFER_OES</CODE>.

@return
<TABLE>
<TR>
<TD><CODE>GL_FRAMEBUFFER_COMPLETE_OES</CODE></TD>
<TD>The frame buffer is complete and supported on this implementation.</TD>
</TR>
<TR>
<TD><CODE>GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_OES</CODE></TD>
<TD>An attached render buffer or texture is not complete.</TD>
</TR>
<TR>
<TD><CODE>GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_OES</CODE></TD>
<TD>There is not at least one attached image.</TD>
</TR>
<TR>
<TD><CODE>GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_OES</CODE></TD>
<TD>All attached images do not have the same width and height.</TD>
</TR>
<TR>
<TD><CODE>GL_FRAMEBUFFER_INCOMPLETE_FORMATS_OES</CODE></TD>
<TD>All images attached to the attachment points <CODE>GL_COLOR_ATTACHMENT0_EXT</CODE>
through <CODE>GL_COLOR_ATTACHMENTn_EXT</CODE> do not have the same internal format.</TD>
</TR>
<TR>
<TD><CODE>GL_FRAMEBUFFER_UNSUPPORTED_OES</CODE></TD>
<TD>The implementation does not support this frame buffer configuration.</TD>
</TR>
</TABLE>

@note
If none of the attachments have a format of <CODE>GL_TEXTURE_SWIZZLED_GPU_SCE</CODE> or
<CODE>GL_TEXTURE_SWIZZLED_SYSTEM_SCE</CODE>, all non-zero attachments do not need to have the same width and height.
In such a case, the drawable size is the minimum width and minimum height of all the attachments.

@par Errors
Use glGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>GL_INVALID_ENUM</CODE></TD>
<TD><I><c>target</c></I> is not <CODE>GL_FRAMEBUFFER_OES</CODE>.</TD>
</TR>
</TABLE>

@sa glBindFramebufferOES
 */
GLAPI GLenum APIENTRY glCheckFramebufferStatusOES( GLenum target )
{
    JS_APIENTRY_TIMER( glCheckFramebufferStatusOES );
#ifdef JS_DPM
    if ( _jsdpmState )
    {
        _JS_DPM_API_ENTRY_1( glCheckFramebufferStatusOES, JSDPM_CALL_TYPE_STATE_BUFFER, GL_FRAMEBUFFER_COMPLETE_OES, 1,
                             target, sizeof( GLenum ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM );
    }
#endif

    PSGLcontext* LContext = _CurrentContext;
    JS_ASSERT( LContext != NULL );

#ifndef JS_NO_ERROR_CHECK
    switch ( target )
    {
        case GL_FRAMEBUFFER_OES:
            break;
        default:
            _jsSetError( GL_INVALID_ENUM );
            return 0;
    }
#endif

    if ( LContext->framebuffer )
    {
        jsFramebuffer* framebuffer = _jsGetFramebuffer( LContext, LContext->framebuffer );
        JS_ASSERT( framebuffer != NULL );	// assertion or error?

        // call down to the platform
        return _jsPlatformFramebufferCheckStatus( framebuffer );
    }

    return GL_FRAMEBUFFER_COMPLETE_OES;
}

/**
@short Attach a texture to a frame buffer

glFramebufferTexture2DOES() attaches an image from the named texture to the
currently bound frame buffer.  Draws calls issued render directly into
the attached texture, provided that the frame buffer is complete.

@param target Must be <CODE>GL_FRAMEBUFFER_OES</CODE>.
@param attachment Specifies the frame buffer attachment point.  Must be one of
<CODE>GL_COLOR_ATTACHMENT0_EXT</CODE>,
<CODE>GL_COLOR_ATTACHMENT1_EXT</CODE>,
<CODE>GL_COLOR_ATTACHMENT2_EXT</CODE>,
<CODE>GL_COLOR_ATTACHMENT3_EXT</CODE>,
<CODE>GL_DEPTH_ATTACHMENT_OES</CODE>, or
<CODE>GL_STENCIL_ATTACHMENT_OES</CODE>.
@param textarget Specifies the texture target binding.  Must be one of
<CODE>GL_TEXTURE_2D</CODE>,
<CODE>GL_TEXTURE_CUBE_MAP_POSITIVE_X</CODE>,
<CODE>GL_TEXTURE_CUBE_MAP_POSITIVE_Y</CODE>,
<CODE>GL_TEXTURE_CUBE_MAP_POSITIVE_Z</CODE>,
<CODE>GL_TEXTURE_CUBE_MAP_NEGATIVE_X</CODE>,
<CODE>GL_TEXTURE_CUBE_MAP_NEGATIVE_Y</CODE>, or
<CODE>GL_TEXTURE_CUBE_MAP_NEGATIVE_Z</CODE>.
@param texture Specifies the texture name.
@param level Specifies the target mipmap level.  Must be <CODE>0</CODE>.

@par Errors
Use glGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>GL_INVALID_ENUM</CODE></TD>
<TD><I><c>target</c></I> is not <CODE>GL_FRAMEBUFFER_OES</CODE>.</TD>
</TR>
<TR>
<TD><CODE>GL_INVALID_ENUM</CODE></TD>
<TD><I><c>attachment</c></I> is not one of
<CODE>GL_COLOR_ATTACHMENT0_EXT</CODE>,
<CODE>GL_COLOR_ATTACHMENT1_EXT</CODE>,
<CODE>GL_COLOR_ATTACHMENT2_EXT</CODE>,
<CODE>GL_COLOR_ATTACHMENT3_EXT</CODE>,
<CODE>GL_DEPTH_ATTACHMENT_OES</CODE>, or
<CODE>GL_STENCIL_ATTACHMENT_OES</CODE>.</TD>
</TR>
<TR>
<TD><CODE>GL_INVALID_ENUM</CODE></TD>
<TD><I><c>textarget</c></I> is not one of
<CODE>GL_TEXTURE_CUBE_MAP_POSITIVE_X</CODE>,
<CODE>GL_TEXTURE_CUBE_MAP_POSITIVE_Y</CODE>,
<CODE>GL_TEXTURE_CUBE_MAP_POSITIVE_Z</CODE>,
<CODE>GL_TEXTURE_CUBE_MAP_NEGATIVE_X</CODE>,
<CODE>GL_TEXTURE_CUBE_MAP_NEGATIVE_Y</CODE>, or
<CODE>GL_TEXTURE_CUBE_MAP_NEGATIVE_Z</CODE>.</TD>
</TR>
<TR>
<TD><CODE>GL_INVALID_VALUE</CODE></TD>
<TD><I><c>level</c></I> is not <CODE>0</CODE>.</TD>
</TR>
<TR>
<TD><CODE>GL_INVALID_OPERATION</CODE></TD>
<TD>The currently bound frame buffer is <CODE>0</CODE>.</TD>
</TR>
</TABLE>

@note
The rendered contents of a frame buffer may also be copied to the images of a
texture object by using glCopyTexImage2D() and glCopyTexSubImage2D().
@par
You cannot simultaneously attach to the depth and stencil buffers
because the hardware only supports depth and stencil buffers together. However, because a
depth buffer always implicitly includes a stencil buffer, you can use both depth and
stencil operations in a frame buffer whenever you attach a depth buffer.

@sa glBindTexture, glBindFramebufferOES, glFramebufferRenderbufferOES
 */
GLAPI void APIENTRY glFramebufferTexture2DOES( GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level )
{
    JS_APIENTRY_TIMER( glFramebufferTexture2DOES );
#ifdef JS_DPM
    if ( _jsdpmState )
    {
        _JS_DPM_API_ENTRY_5( glFramebufferTexture2DOES, JSDPM_CALL_TYPE_STATE_TEX, , 5,
                             target, sizeof( GLenum ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM,
                             attachment, sizeof( GLenum ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM,
                             textarget, sizeof( GLenum ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM,
                             texture, sizeof( GLuint ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             level, sizeof( GLint ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE );
    }
#endif

    PSGLcontext* LContext = _CurrentContext;
    JS_ASSERT( LContext != NULL );

#ifndef JS_NO_ERROR_CHECK
    switch ( target )
    {
        case GL_FRAMEBUFFER_OES:
            break;
        default:
            _jsSetError( GL_INVALID_ENUM );
            return;
    }

    switch ( textarget )
    {
        case GL_TEXTURE_2D:
        case GL_TEXTURE_CUBE_MAP_POSITIVE_X:
        case GL_TEXTURE_CUBE_MAP_NEGATIVE_X:
        case GL_TEXTURE_CUBE_MAP_POSITIVE_Y:
        case GL_TEXTURE_CUBE_MAP_NEGATIVE_Y:
        case GL_TEXTURE_CUBE_MAP_POSITIVE_Z:
        case GL_TEXTURE_CUBE_MAP_NEGATIVE_Z:
            break;
        default:
            _jsSetError( GL_INVALID_ENUM );
            return;
    }

    // OES extension only allows base level mipmap to be a target
    if ( level != 0 )
    {
        _jsSetError( GL_INVALID_VALUE );
        return;
    }

    // illegal on the default framebuffer
    if ( LContext->framebuffer == 0 )
    {
        _jsSetError( GL_INVALID_OPERATION );
        return;
    }
#endif

    jsFramebuffer* framebuffer = _jsGetFramebuffer( LContext, LContext->framebuffer );
    JS_ASSERT( framebuffer );

    // choose attachment
    jsFramebufferAttachment* attach = _jsFramebufferGetAttachment( framebuffer, attachment );
    if ( !attach ) return;

    jsTexture *textureObject = NULL;
    GLuint face;
    _jsFramebufferGetAttachmentTexture( LContext, attach, &textureObject, &face );
    if ( textureObject ) textureObject->framebuffers.removeElement( framebuffer );

    // configure attachment
    if ( texture )
    {
        attach->type = _JS_FRAMEBUFFER_ATTACHMENT_TEXTURE;
        textureObject = _jsGetTexture( LContext, texture );
        JS_ASSERT( textureObject );
        textureObject->framebuffers.pushBack( framebuffer );
    }
    else attach->type = _JS_FRAMEBUFFER_ATTACHMENT_NONE;
    attach->name = texture;
    attach->textureTarget = textarget;

    framebuffer->needValidate = GL_TRUE;
    LContext->needValidate |= PSGL_VALIDATE_SCISSOR_BOX | PSGL_VALIDATE_FRAMEBUFFER;
}

/**
@short Attach a render buffer to a frame buffer

glFramebufferRenderbufferOES() attaches the named render buffer to the currently bound frame buffer.
Draws calls issued will render directly into the attached render buffer, provided the frame buffer
is complete.

@param target Must be <CODE>GL_FRAMEBUFFER_OES</CODE>.
@param attachment Specifies the frame buffer attachment point.  Must be one of
<CODE>GL_COLOR_ATTACHMENT0_EXT</CODE>,
<CODE>GL_COLOR_ATTACHMENT1_EXT</CODE>,
<CODE>GL_COLOR_ATTACHMENT2_EXT</CODE>,
<CODE>GL_COLOR_ATTACHMENT3_EXT</CODE>,
<CODE>GL_DEPTH_ATTACHMENT_OES</CODE>, or
<CODE>GL_STENCIL_ATTACHMENT_OES</CODE>.
@param renderbuffertarget Must be <CODE>GL_RENDERBUFFER_OES</CODE>.
@param renderbuffer Specifies the render buffer name.

@par Errors
Use glGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>GL_INVALID_ENUM</CODE></TD>
<TD><I><c>target</c></I> is not <CODE>GL_FRAMEBUFFER_OES</CODE>.</TD>
</TR>
<TR>
<TD><CODE>GL_INVALID_ENUM</CODE></TD>
<TD><I><c>attachment</c></I> is not one of
<CODE>GL_COLOR_ATTACHMENT0_EXT</CODE>,
<CODE>GL_COLOR_ATTACHMENT1_EXT</CODE>,
<CODE>GL_COLOR_ATTACHMENT2_EXT</CODE>,
<CODE>GL_COLOR_ATTACHMENT3_EXT</CODE>,
<CODE>GL_DEPTH_ATTACHMENT_OES</CODE>, or
<CODE>GL_STENCIL_ATTACHMENT_OES</CODE>.</TD>
</TR>
<TR>
<TD><CODE>GL_INVALID_ENUM</CODE></TD>
<TD><I><c>renderbuffertarget</c></I> is not <CODE>GL_RENDERBUFFER_OES</CODE>.</TD>
</TR>
<TR>
<TD><CODE>GL_INVALID_OPERATION</CODE></TD>
<TD>The currently bound frame buffer is <CODE>0</CODE>.</TD>
</TR>
</TABLE>

@sa glBindFramebufferOES, glFramebufferTexture2DOES
 */
GLAPI void APIENTRY glFramebufferRenderbufferOES( GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer )
{
    JS_APIENTRY_TIMER( glFramebufferRenderbufferOES );
#ifdef JS_DPM
    if ( _jsdpmState )
    {
        _JS_DPM_API_ENTRY_4( glFramebufferRenderbufferOES, JSDPM_CALL_TYPE_STATE_BUFFER, , 4,
                             target, sizeof( GLenum ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM,
                             attachment, sizeof( GLenum ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM,
                             renderbuffertarget, sizeof( GLenum ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM,
                             renderbuffer, sizeof( GLuint ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE );
    }
#endif

    PSGLcontext* LContext = _CurrentContext;
    JS_ASSERT( LContext != NULL );

#ifndef JS_NO_ERROR_CHECK
    switch ( target )
    {
        case GL_FRAMEBUFFER_OES:
            break;
        default:
            _jsSetError( GL_INVALID_ENUM );
            return;
    }

    switch ( renderbuffertarget )
    {
        case GL_RENDERBUFFER_OES:
            break;
        default:
            _jsSetError( GL_INVALID_ENUM );
            return;
    }

    // illegal on the default framebuffer
    if ( LContext->framebuffer == 0 )
    {
        _jsSetError( GL_INVALID_OPERATION );
        return;
    }
#endif

    jsFramebuffer* framebuffer = _jsGetFramebuffer( LContext, LContext->framebuffer );
    JS_ASSERT( framebuffer );

    // choose attachment
    jsFramebufferAttachment* attach = _jsFramebufferGetAttachment( framebuffer, attachment );
    if ( !attach ) return;

    jsTexture *textureObject = NULL;
    GLuint face;
    _jsFramebufferGetAttachmentTexture( LContext, attach, &textureObject, &face );
    if ( textureObject ) textureObject->framebuffers.removeElement( framebuffer );

    // configure attachment
    if ( renderbuffer )
    {
        attach->type = _JS_FRAMEBUFFER_ATTACHMENT_RENDERBUFFER;
        textureObject = _jsGetRenderbuffer( LContext, renderbuffer );
        JS_ASSERT( textureObject );
        textureObject->framebuffers.pushBack( framebuffer );
    }
    else attach->type = _JS_FRAMEBUFFER_ATTACHMENT_NONE;
    // configure attachment
    attach->name = renderbuffer;

    framebuffer->needValidate = GL_TRUE;
    LContext->needValidate |= PSGL_VALIDATE_SCISSOR_BOX | PSGL_VALIDATE_FRAMEBUFFER;
}

/**
@short Set frame buffer parameters

glFramebufferParameteriSCE() can be used to set frame buffer parameters. Currently, only
<CODE>GL_FRAMEBUFFER_MULTISAMPLING_MODE_SCE</CODE> can be set, which specifies the multisampling mode to be used.

The default is <CODE>GL_MULTISAMPLING_NONE_SCE</CODE> means that no multisampling is enabled, and regular operation occurs.
When one of <CODE>GL_MULTISAMPLING_2X_DIAGONAL_CENTERED_SCE</CODE>, <CODE>GL_MULTISAMPLING_4X_SQUARE_CENTERED_SCE</CODE>, or
<CODE>GL_MULTISAMPLING_4X_SQUARE_ROTATED_SCE</CODE> is selected, multisampling is enabled, and the various attachments in the frame buffer contain samples instead of pixels.

The actual width and height of the frame buffer depends on the multi-sampling mode. If it is <CODE>GL_MULTISAMPLING_NONE_SCE</CODE>, the width and height are the attachments minimum widths and heights.
If it is <CODE>GL_MULTISAMPLING_2X_DIAGONAL_CENTERED_SCE</CODE>, the width is divided by 2, and the samples are written in consecutive texels (in the x direction).
If it is <CODE>GL_MULTISAMPLING_4X_SQUARE_CENTERED_SCE</CODE>, or <CODE>GL_MULTISAMPLING_4X_SQUARE_ROTATED_SCE</CODE> , both the width and height are divided by 2, and the 4 samples are written in consecutive texels in both x and y direction (a given pixel is represented by a 2x2 texel block).

For example, for a 720p render target, the size of the buffer created for depth and color
will be:
<ul>
<li>1280*2 &times; 720 if the multisampling mode is <code>GL_MULTISAMPLING_2X_DIAGONAL_CENTERED_SCE</CODE></LI>
<li>1280*2 &times; 720*2 if the multisampling mode is <code>GL_MULTISAMPLING_4X_SQUARE_CENTERED_SCE</CODE> or <code>GL_MULTISAMPLING_4X_SQUARE_ROTATED_SCE</CODE></li>
</ul>
Note that the depth buffer and the color buffer must be the same size.  If not, it is
possible that only half or a quarter of the screen will be drawn.

When using multisampling, the viewport should be set to the size of the render target, not
the size of the frame buffer.  In the example above, the viewport should be set to 1280
&times; 720, not 1280*2 &times; 720*2.

For best performance, the various textures that are attached to the frame buffer should have the corresponding multisampling hint set with glTexParameterf().

Filtering of samples is done when pixels are accessed by calling glCopyTexImage2D()or glReadPixels().
However, an application may choose to use the sample buffer directly by binding the texture, and doing filtering in the shader.

@note
For more information about multisampling, including an example demonstrating how to use
multisampling in a PSGL application, refer to the "Multisampling" section of the <i>PlayStation&reg; Graphics Library Programming Guide</i>.

@param target Must be <CODE>GL_FRAMEBUFFER_OES</CODE>.
@param pname The parameter to be set. Must be <CODE>GL_FRAMEBUFFER_MULTISAMPLING_MODE_SCE</CODE>
@param param The value of the parameter. For <CODE>GL_FRAMEBUFFER_MULTISAMPLING_MODE_SCE</CODE>,
this value must be one of
<CODE>GL_MULTISAMPLING_NONE_SCE</CODE>,
<CODE>GL_MULTISAMPLING_2X_DIAGONAL_CENTERED_SCE</CODE>,
<CODE>GL_MULTISAMPLING_4X_SQUARE_CENTERED_SCE</CODE>, or
<CODE>GL_MULTISAMPLING_4X_SQUARE_ROTATED_SCE</CODE>.

@par Errors
Use glGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>GL_INVALID_ENUM</CODE></TD>
<TD><I><c>target</c></I> is not <CODE>GL_FRAMEBUFFER_OES</CODE>.</TD>
</TR>
<TR>
<TD><CODE>GL_INVALID_ENUM</CODE></TD>
<TD><I><c>pname</c></I> is not <CODE>GL_FRAMEBUFFER_MULTISAMPLING_MODE_SCE</CODE>.</TD>
</TR>
<TR>
<TD><CODE>GL_INVALID_ENUM</CODE></TD>
<TD><I><c>param</c></I> is not one of
<CODE>GL_MULTISAMPLING_NONE_SCE</CODE>,
<CODE>GL_MULTISAMPLING_2X_DIAGONAL_CENTERED_SCE</CODE>,
<CODE>GL_MULTISAMPLING_4X_SQUARE_CENTERED_SCE</CODE>, or
<CODE>GL_MULTISAMPLING_4X_SQUARE_ROTATED_SCE</CODE>.</TD>
</TR>
<TR>
<TD><CODE>GL_INVALID_OPERATION</CODE></TD>
<TD>The currently bound frame buffer is <CODE>0</CODE>.</TD>
</TR>
</TABLE>

@sa glBindFramebufferOES, glFramebufferTexture2DOES, glTexParameterf
 */
GLAPI void APIENTRY glFramebufferParameteriSCE( GLenum target, GLenum pname, GLint param )
{
    JS_APIENTRY_TIMER( glFramebufferParameteriSCE );
    if ( _jsdpmState )
    {
        _JS_DPM_API_ENTRY_3( glFramebufferParameteriSCE, JSDPM_CALL_TYPE_STATE_BUFFER, , 3,
                             target, sizeof( GLenum ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM,
                             pname, sizeof( GLenum ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM,
                             param, sizeof( GLint ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE );
    }

    PSGLcontext* LContext = _CurrentContext;
    JS_ASSERT( LContext != NULL );

#ifndef JS_NO_ERROR_CHECK
    switch ( target )
    {
        case GL_FRAMEBUFFER_OES:
            break;
        default:
            _jsSetError( GL_INVALID_ENUM );
            return;
    }
    // illegal on the default framebuffer
    if ( LContext->framebuffer == 0 )
    {
        _jsSetError( GL_INVALID_OPERATION );
        return;
    }
#endif

    jsFramebuffer* framebuffer = _jsGetFramebuffer( LContext, LContext->framebuffer );
    JS_ASSERT( framebuffer );

#ifndef JS_NO_ERROR_CHECK
    switch ( pname )
    {
        case GL_FRAMEBUFFER_MULTISAMPLING_MODE_SCE:
        {
            switch ( param )
            {
                case GL_MULTISAMPLING_NONE_SCE:
                case GL_MULTISAMPLING_2X_DIAGONAL_CENTERED_SCE:
                case GL_MULTISAMPLING_4X_SQUARE_CENTERED_SCE:
                case GL_MULTISAMPLING_4X_SQUARE_ROTATED_SCE:
#endif
                    framebuffer->multisampling = param;
#ifndef JS_NO_ERROR_CHECK
                    break;
                default:
                    _jsSetError( GL_INVALID_ENUM );
                    return;
            }
        }
        break;
        default:
            _jsSetError( GL_INVALID_ENUM );
            return;
    }
#endif
    framebuffer->needValidate = GL_TRUE;
    LContext->needValidate |= PSGL_VALIDATE_SCISSOR_BOX | PSGL_VALIDATE_FRAMEBUFFER;
}

/**
@short Get frame buffer attachment parameters

glGetFramebufferAttachmentParameterivOES() retrieves information about an attachment
to the currently bound frame buffer.

If <I><c>pname</c></I> is <CODE>GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE_OES</CODE>,
one of
<CODE>GL_RENDERBUFFER_OES</CODE>,
<CODE>GL_TEXTURE_2D</CODE>, or
<CODE>GL_NONE</CODE> is returned in <I><c>params</c></I>.

If <I><c>pname</c></I> is <CODE>GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME_OES</CODE>
and the value of <CODE>GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE_OES</CODE> is
<CODE>GL_RENDERBUFFER_OES</CODE> or <CODE>GL_TEXTURE_2D</CODE>, the render buffer
or texture name it returned in <I><c>params</c></I>.

If <I><c>pname</c></I> is <CODE>GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_LEVEL_OES</CODE>
and the value of <CODE>GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE_OES</CODE> is
<CODE>GL_TEXTURE_2D</CODE>, the mipmap level of the attached texture image
is returned in <I><c>params</c></I>.  This value will always be <CODE>0</CODE>.

If <I><c>pname</c></I> is <CODE>GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_CUBE_MAP_FACE_OES</CODE>
and the value of <CODE>GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE_OES</CODE> is
<CODE>GL_TEXTURE_2D</CODE>, the cube map face of the attached texture image is
returned in <I><c>params</c></I>.  The returned value is <CODE>0</CODE> if the attached texture image is not a cube map face,
or is one of the following if the attached texture image is a cube map face:
<ul>
<li><CODE>GL_TEXTURE_CUBE_MAP_POSITIVE_X</CODE></li>
<li><CODE>GL_TEXTURE_CUBE_MAP_POSITIVE_Y</CODE></li>
<li><CODE>GL_TEXTURE_CUBE_MAP_POSITIVE_Z</CODE></li>
<li><CODE>GL_TEXTURE_CUBE_MAP_NEGATIVE_X</CODE></li>
<li><CODE>GL_TEXTURE_CUBE_MAP_NEGATIVE_Y</CODE></li>
<li><CODE>GL_TEXTURE_CUBE_MAP_NEGATIVE_Z</CODE></li>
</ul>

@param target Must be <CODE>GL_FRAMEBUFFER_OES</CODE>.
@param attachment Specifies the frame buffer attachment point.  Must be one of
<CODE>GL_COLOR_ATTACHMENT0_EXT</CODE>,
<CODE>GL_COLOR_ATTACHMENT1_EXT</CODE>,
<CODE>GL_COLOR_ATTACHMENT2_EXT</CODE>,
<CODE>GL_COLOR_ATTACHMENT3_EXT</CODE>,
<CODE>GL_DEPTH_ATTACHMENT_OES</CODE>, or
<CODE>GL_STENCIL_ATTACHMENT_OES</CODE>.
@param pname Specifies the parameter to be returned.  Must be one of
<CODE>GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE_OES</CODE>,
<CODE>GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME_OES</CODE>,
<CODE>GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_LEVEL_OES</CODE>, or
<CODE>GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_CUBE_MAP_FACE_OES</CODE>.
@param params Specifies an address for the returned parameter value.

@par Errors
Use glGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>GL_INVALID_ENUM</CODE></TD>
<TD><I><c>target</c></I> is not <CODE>GL_FRAMEBUFFER_OES</CODE>.</TD>
</TR>
<TR>
<TD><CODE>GL_INVALID_ENUM</CODE></TD>
<TD><I><c>attachment</c></I> is not one of
<CODE>GL_COLOR_ATTACHMENT0_EXT</CODE>,
<CODE>GL_COLOR_ATTACHMENT1_EXT</CODE>,
<CODE>GL_COLOR_ATTACHMENT2_EXT</CODE>,
<CODE>GL_COLOR_ATTACHMENT3_EXT</CODE>,
<CODE>GL_DEPTH_ATTACHMENT_OES</CODE>, or
<CODE>GL_STENCIL_ATTACHMENT_OES</CODE>.</TD>
</TR>
<TR>
<TD><CODE>GL_INVALID_ENUM</CODE></TD>
<TD><I><c>pname</c></I> is not
<CODE>GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE_OES</CODE>,
<CODE>GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME_OES</CODE>,
<CODE>GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_LEVEL_OES</CODE>, or
<CODE>GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_CUBE_MAP_FACE_OES</CODE>.</TD>
</TR>
<TR>
<TD><CODE>GL_INVALID_ENUM</CODE></TD>
<TD>The attachment is a
render buffer and <I>pname</I> is
<CODE>GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_LEVEL_OES</CODE> or
<CODE>GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_CUBE_MAP_FACE_OES</CODE>.</TD>
</TR>
<TR>
<TD><CODE>GL_INVALID_ENUM</CODE></TD>
<TD>There is no attachment
and <I>pname</I> is anything except <CODE>GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE_OES</CODE>.</TD>
</TR>
<TR>
<TD><CODE>GL_INVALID_OPERATION</CODE></TD>
<TD>The currently bound frame buffer is <CODE>0</CODE>.</TD>
</TR>
</TABLE>

@sa glFramebufferTexture2DOES, glFramebufferRenderbufferOES
 */
GLAPI void APIENTRY glGetFramebufferAttachmentParameterivOES( GLenum target, GLenum attachment, GLenum pname, GLint *params )
{
    JS_APIENTRY_TIMER( glGetFramebufferAttachmentParameterivOES );
#ifdef JS_DPM
    if ( _jsdpmState )
    {
        _JS_DPM_API_ENTRY_4( glGetFramebufferAttachmentParameterivOES, JSDPM_CALL_TYPE_STATE_BUFFER, , 4,
                             target, sizeof( GLenum ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM,
                             attachment, sizeof( GLenum ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM,
                             pname, sizeof( GLenum ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM,
                             params, sizeof( GLint ), INPUT_OUTPUT, DPM_API_ARGUMENT_POINTER );
    }
#endif

    PSGLcontext* LContext = _CurrentContext;
    JS_ASSERT( LContext != NULL );

#ifndef JS_NO_ERROR_CHECK
    switch ( target )
    {
        case GL_FRAMEBUFFER_OES:
            break;
        default:
            _jsSetError( GL_INVALID_ENUM );
            return;
    }

    // illegal on the default framebuffer
    if ( LContext->framebuffer == 0 )
    {
        _jsSetError( GL_INVALID_OPERATION );
        return;
    }
#endif

    jsFramebuffer* framebuffer = _jsGetFramebuffer( LContext, LContext->framebuffer );
    JS_ASSERT( framebuffer );

    // choose attachment
    jsFramebufferAttachment* attach = _jsFramebufferGetAttachment( framebuffer, attachment );
    if ( !attach ) return;

    switch ( attach->type )
    {
        case _JS_FRAMEBUFFER_ATTACHMENT_RENDERBUFFER:
            switch ( pname )
            {
                case GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE_OES:
                    *params = GL_RENDERBUFFER_OES;
                    break;
                case GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME_OES:
                    *params = attach->name;
                    break;
                default:
                    _jsSetError( GL_INVALID_ENUM );
                    return;
            }
            break;

        case _JS_FRAMEBUFFER_ATTACHMENT_TEXTURE:
            switch ( pname )
            {
                case GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE_OES:
                    *params = GL_TEXTURE_2D;
                    break;
                case GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME_OES:
                    *params = attach->name;
                    break;
                case GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_LEVEL_OES:
                    *params = 0;
                    break;
                case GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_CUBE_MAP_FACE_OES:
                    switch ( attach->textureTarget )
                    {
                        case GL_TEXTURE_CUBE_MAP_POSITIVE_X:
                        case GL_TEXTURE_CUBE_MAP_NEGATIVE_X:
                        case GL_TEXTURE_CUBE_MAP_POSITIVE_Y:
                        case GL_TEXTURE_CUBE_MAP_NEGATIVE_Y:
                        case GL_TEXTURE_CUBE_MAP_POSITIVE_Z:
                        case GL_TEXTURE_CUBE_MAP_NEGATIVE_Z:
                            *params = attach->textureTarget;
                            break;
                        default:
                            *params = 0;
                    }
                    break;
                case GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_3D_ZOFFSET_EXT:
                    *params = 0;
                    break;
                default:
                    _jsSetError( GL_INVALID_ENUM );
                    return;
            }
            break;

        case _JS_FRAMEBUFFER_ATTACHMENT_NONE:
            switch ( pname )
            {
                case GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE_OES:
                    *params = GL_NONE;
                    break;
                default:
                    _jsSetError( GL_INVALID_ENUM );
                    return;
            }
            break;

        default:
            JS_ASSERT( 0 );
    }
}

/**
@short Generate mipmaps

glGenerateMipmapOES() generates a mipmap chain for the 2D texture or
cube map face indicated by <I><c>target</c></I>.  All previously existing
levels other than the base level will be overwritten or removed.
All levels higher than the base level image are computed by
successive filtering.

You cannot automatically generate mipmaps for DXTn textures or for texture references.

@param target Specifies texture target on which to perform mipmap generation.
Must be one of
<CODE>GL_TEXTURE_2D</CODE>,
<CODE>GL_TEXTURE_3D</CODE>,
<CODE>GL_TEXTURE_CUBE_MAP_POSITIVE_X</CODE>,
<CODE>GL_TEXTURE_CUBE_MAP_POSITIVE_Y</CODE>,
<CODE>GL_TEXTURE_CUBE_MAP_POSITIVE_Z</CODE>,
<CODE>GL_TEXTURE_CUBE_MAP_NEGATIVE_X</CODE>,
<CODE>GL_TEXTURE_CUBE_MAP_NEGATIVE_Y</CODE>, or
<CODE>GL_TEXTURE_CUBE_MAP_NEGATIVE_Z</CODE>.

@note
glGenerateMipmapOES() is only fast for textures with the following color formats:
<ul>
<li><code>GL_RGBA</code></li>
<li><code>GL_ARGB_SCE</code></li>
<li><code>GL_RGBA32F_ARB</code></li>
</ul>
glGenerateMipmapOES() finishes for any other format.

@par Errors
Use glGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>GL_INVALID_ENUM</CODE></TD>
<TD><I><c>target</c></I> is not one of
<CODE>GL_TEXTURE_2D</CODE>,
<CODE>GL_TEXTURE_3D</CODE>,
<CODE>GL_TEXTURE_CUBE_MAP_POSITIVE_X</CODE>,
<CODE>GL_TEXTURE_CUBE_MAP_POSITIVE_Y</CODE>,
<CODE>GL_TEXTURE_CUBE_MAP_POSITIVE_Z</CODE>,
<CODE>GL_TEXTURE_CUBE_MAP_NEGATIVE_X</CODE>,
<CODE>GL_TEXTURE_CUBE_MAP_NEGATIVE_Y</CODE>, or
<CODE>GL_TEXTURE_CUBE_MAP_NEGATIVE_Z</CODE>.</TD>
</TR>
<TR>
<TD><CODE>GL_INVALID_OPERATION</CODE></TD>
<TD>The <I><c>internalFormat</c></I> of the base image is one of
<CODE>GL_COMPRESSED_RGB_S3TC_DXT1_EXT</CODE>,
<CODE>GL_COMPRESSED_RGBA_S3TC_DXT1_EXT</CODE>,
<CODE>GL_COMPRESSED_RGBA_S3TC_DXT3_EXT</CODE>, or
<CODE>GL_COMPRESSED_RGBA_S3TC_DXT5_EXT</CODE>.</TD>
</TR>
</TABLE>

 */
GLAPI void APIENTRY glGenerateMipmapOES( GLenum target )
{
    JS_APIENTRY_TIMER( glGenerateMipmapOES );
    PSGLcontext* LContext = _CurrentContext;
    JS_ASSERT( LContext != NULL );

#ifdef JS_DPM
    if ( _jsdpmState )
    {
        _JS_DPM_API_ENTRY_1( glGenerateMipmapOES, JSDPM_CALL_TYPE_STATE_TEX, , 1,
                             target, sizeof( GLenum ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM );
    }
#endif

#ifndef JS_NO_ERROR_CHECK
    switch ( target )
    {
        case GL_TEXTURE_2D:
        case GL_TEXTURE_3D:
        case GL_TEXTURE_CUBE_MAP_POSITIVE_X:
        case GL_TEXTURE_CUBE_MAP_NEGATIVE_X:
        case GL_TEXTURE_CUBE_MAP_POSITIVE_Y:
        case GL_TEXTURE_CUBE_MAP_NEGATIVE_Y:
        case GL_TEXTURE_CUBE_MAP_POSITIVE_Z:
        case GL_TEXTURE_CUBE_MAP_NEGATIVE_Z:
            break;
        default:
            _jsSetError( GL_INVALID_ENUM );
            return;
    }

    jsTextureImageUnit *unit = LContext->CurrentImageUnit;
    if ( !unit )
    {
        _jsSetError( GL_INVALID_OPERATION );
        return;
    }
#endif

    // get the texture (and image for level 0) bound to this target
    jsTexture* texture = NULL;
    jsImage* image = NULL;
    if ( _jsGetImage( target, 0, &texture, &image, 1 ) )
        return;	// error set in _jsGetImage

#ifndef JS_NO_ERROR_CHECK
    if ( texture->referenceBuffer )
    {
        _jsSetError( GL_INVALID_OPERATION );
        return;
    }
    switch ( image->internalFormat )
    {
        case GL_COMPRESSED_RGB_S3TC_DXT1_EXT:
        case GL_COMPRESSED_RGBA_S3TC_DXT1_EXT:
        case GL_COMPRESSED_RGBA_S3TC_DXT3_EXT:
        case GL_COMPRESSED_RGBA_S3TC_DXT5_EXT:
            _jsSetError( GL_INVALID_OPERATION );
            return;
        default:
            break;
    }
#endif

    // can not generate miplevels on something that can't have miplevels,
    // such as image of size 1x1 (which is created for bound textures with no set image)
    if ( texture->imageCount <= texture->baseLevel + 1 )
    {
        // should this call _jsSetError(GL_INVALID_OPERATION) ?
        return;
    }

    // determine face
    GLuint face = 0;
    switch ( target )
    {
        case GL_TEXTURE_CUBE_MAP_POSITIVE_X:
        case GL_TEXTURE_CUBE_MAP_NEGATIVE_X:
        case GL_TEXTURE_CUBE_MAP_POSITIVE_Y:
        case GL_TEXTURE_CUBE_MAP_NEGATIVE_Y:
        case GL_TEXTURE_CUBE_MAP_POSITIVE_Z:
        case GL_TEXTURE_CUBE_MAP_NEGATIVE_Z:
            face = target - GL_TEXTURE_CUBE_MAP_POSITIVE_X;
            break;
        case GL_TEXTURE_2D:
        case GL_TEXTURE_3D:
            face = 0;
            break;
        default:
            JS_ASSERT( 0 );
    }

    // allocate all levels
    _jsReallocateImages( texture, 0, MAX( image->width, MAX( image->height, image->depth ) ) );
    image = texture->images[face] + texture->baseLevel;

    // create all mipmap levels
    int level = texture->baseLevel;
    GLuint levelWidth = image->width;
    GLuint levelHeight = image->height;
    GLuint levelDepth = image->depth;
    do
    {
        ++level;
        levelWidth = levelWidth > 1 ? levelWidth / 2 : 1;
        levelHeight = levelHeight > 1 ? levelHeight / 2 : 1;
        levelDepth = levelDepth > 1 ? levelDepth / 2 : 1;

        _jsSetImage(
            &texture->images[face][level],
            image->internalFormat,
            levelWidth, levelHeight, levelDepth,
            0,		// alignment
            image->format, image->type,
            NULL );
    }
    while ( levelWidth > 1 || levelHeight > 1 || levelDepth > 1 );

    texture->hasMipmaps = GL_TRUE;
    texture->revalidate |= _JS_TEXTURE_REVALIDATE_LAYOUT;

    if ( !_jsPlatformGenerateMipmap( texture, face ) )
    {
        // get base level from GPU
        _jsPlatformGetImageData( target, texture->baseLevel, texture, image );

        // resample if base level data exists
        if ( image->data != NULL )
        {
            // loop over levels
            for ( GLuint i = 1; i < texture->imageCount; ++i )
            {
                _jsImageAllocCPUStorage( &image[i] );
                _jsResampleImage3D( &image[i - 1], &image[i] );

                image[i].dataState = _JS_IMAGE_DATASTATE_HOST;
            }
        }

        texture->revalidate |= _JS_TEXTURE_REVALIDATE_IMAGES;
        LContext->needValidate |= PSGL_VALIDATE_TEXTURES_USED;
    }
}

/**
 * @} OpenGL
 */
// ------------------------------------------------------
