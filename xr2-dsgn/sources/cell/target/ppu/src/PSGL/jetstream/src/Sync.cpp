/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 *                Copyright (C) 2009 Sony Computer Entertainment Inc.
 *                                               All Rights Reserved.
 */


/*
 * VertexArray functions
 */


#include <PSGL/psgl.h>
#include <PSGL/private.h>
#include <PSGL/Debug.h>
#include <PSGL/DPM2.h>

// called via _jsTexNameSpaceCreateNameLazy
jsFenceObject *_jsCreateFenceObject()
{
    GLuint size = sizeof( jsFenceObject ) + _jsPlatformFenceObjectSize();
    jsFenceObject *fence = ( jsFenceObject * )jsMalloc( size );
    if ( !fence ) return NULL;
    memset( fence, 0, size );

    if ( !_jsPlatformCreateFenceObject( fence ) )
    {
        jsFree( fence );
        return NULL;
    }

    fence->isSet = GL_FALSE;

    return fence;
}

// called via _jsTexNameSpaceCreateNameLazy
void _jsFreeFenceObject( jsFenceObject *fence )
{
    _jsPlatformDestroyFenceObject( fence );
    jsFree( fence );
}

// called via _jsTexNameSpaceCreateNameLazy
jsEventObject *_jsCreateEventObject()
{
    GLuint size = sizeof( jsEventObject ) + _jsPlatformEventObjectSize();
    jsEventObject *event = ( jsEventObject * )jsMalloc( size );
    if ( !event ) return NULL;
    memset( event, 0, size );

    if ( !_jsPlatformCreateEventObject( event ) )
    {
        jsFree( event );
        return NULL;
    }

    return event;
}

// called via _jsTexNameSpaceCreateNameLazy
void _jsFreeEventObject( jsEventObject *event )
{
    _jsPlatformDestroyEventObject( event );
    jsFree( event );
}

/**
 *  @addtogroup OpenGL
 *  @{
 */

/**
 *  @short Delete fence names
 *
 *  glDeleteFencesNV() deletes <I><c>n</c></I> fences named by the elements of the array <I><c>fences</c></I>.
 *  After a fence is deleted, it has no state, its name is unused, and its name is free for reuse
 *  by glGenFencesNV().
 *
 *  @param n Specifies the number of fence names to be deleted.
 *
 *  @param fences A pointer to an array of fences to be deleted.
 *
 *  @par Errors
Use glGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>GL_INVALID_VALUE</CODE></TD>
<TD><I><c>n</c></I> is negative.</TD>
</TR>
</TABLE>
 *
 *  @sa glGenFencesNV
 */

GLAPI void APIENTRY glDeleteFencesNV( GLsizei n, const GLuint *fences )
{
    JS_APIENTRY_TIMER( glDeleteFencesNV );
    if ( _jsdpmState )
    {
        _JS_DPM_API_ENTRY_2( glDeleteFencesNV, JSDPM_CALL_TYPE_SYNC, , 2,
                             n, sizeof( GLsizei ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             fences, ( n * sizeof( GLuint ) ), INPUT_BY_REFERENCE, DPM_API_ARGUMENT_POINTER );
    }

    PSGLcontext *LContext = _CurrentContext;
    JS_ASSERT( LContext );
    _jsTexNameSpaceDeleteNames( &LContext->fenceObjectNameSpace, n, fences );
}

/**
 *  @short Generate fence names
 *
 *  glGenFencesNV() returns <I><c>n</c></I> unused fence names in <I><c>fences</c></I>. There is no guarantee that
 *  the names form a contiguous set of integers.  The fences are initially marked as used, but
 *  acquire meaningful boolean state only after they have been set by glSetFenceNV().
 *  A name returned by glGenFencesNV(), but not yet set by glSetFenceNV(), is not yet considered the name of a fence.
 *
 *  Fence names returned by a call to glGenFencesNV() are not returned by subsequent calls,
 *  unless they are first deleted with glDeleteFencesNV().
 *
 *  @param n Specifies the number of fence names to be generated.
 *
 *  @param fences A pointer to an array in which the generated fence names are stored.
 *
 *  @par Errors
Use glGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>GL_INVALID_VALUE</CODE></TD>
<TD><I><c>n</c></I> is negative.</TD>
</TR>
</TABLE>
 *
 *  @sa glSetFenceNV, glDeleteFencesNV
 */

GLAPI void APIENTRY glGenFencesNV( GLsizei n, GLuint *fences )
{
    JS_APIENTRY_TIMER( glGenFencesNV );
    if ( _jsdpmState )
    {
        _JS_DPM_API_ENTRY_2( glGenFencesNV, JSDPM_CALL_TYPE_SYNC, , 2,
                             n, sizeof( GLsizei ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             fences, ( n * sizeof( GLuint ) ), INPUT_OUTPUT, DPM_API_ARGUMENT_POINTER );
    }

    PSGLcontext *LContext = _CurrentContext;
    JS_ASSERT( LContext );
    _jsTexNameSpaceGenNames( &LContext->fenceObjectNameSpace, n, fences );
}

/**
 *  @short Determine if a name corresponds to a fence
 *
 *  glIsFenceNV() returns <c>GL_TRUE</c> if <I><c>fence</c></I> is the name of a currently existing fence.
 *  If <I><c>fence</c></I> is zero, or is a non-zero value that is not currently the name of a fence, glIsFenceNV() returns <c>GL_FALSE</c>.
 *
 *  @param fence Specifies the name of the fence
 *
 *  @return A boolean indicating whether or not a fence currently exists
 *
 *  @sa glGenFencesNV, glSetFenceNV
 */

GLAPI GLboolean APIENTRY glIsFenceNV( GLuint fence )
{
    JS_APIENTRY_TIMER( glIsFenceNV );
    if ( _jsdpmState ) { _JS_DPM_API_ENTRY_1( glIsFenceNV, JSDPM_CALL_TYPE_SYNC, GL_TRUE, 1, fence, sizeof( GLuint ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE ); }

    // check if the name itself is valid
    PSGLcontext *LContext = _CurrentContext;
    JS_ASSERT( LContext );
    if ( !_jsTexNameSpaceIsName( &LContext->fenceObjectNameSpace, fence ) )
        return GL_FALSE;

    jsFenceObject* f = _jsGetFenceObject( LContext, fence );
    JS_ASSERT( f != NULL );

    // spec says that only names that have been set are really fences
    return f->isSet;
}

/**
 *  @short Check to see if a given fence has been completed
 *
 *  glTestFenceNV() Determine if the named <I><c>fence</c></I> has been
 *  completed.  glTestFenceNV() does not block program execution.
 *
 *  @param fence Specifies the name of the fence to query for
 *  completion.
 *
 *  @return Returns <CODE>GL_TRUE</CODE> if the named <I><c>fence</c></I> has
 *  been completed; <CODE>GL_FALSE</CODE> if it has not.
 *
 *  @note A fence should not be left untested or unfinished for a long
 *  period.  Fences are implemented with a signed 32-bit counter, so
 *  if more than (2<sup>31</sup>)-1 intervening fences are issued before a fence
 *  is tested (glTestFenceNV()) or finished (glFinishFenceNV()), that
 *  fence may indicate an incorrect status.  In practice, this is
 *  unlikely, as it requires over a billion intervening fences.
 *
 *  @par Errors
Use glGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>GL_INVALID_OPERATION</CODE></TD>
<TD>The named fence does not exist.</TD>
</TR>
</TABLE>
 *
 *  @sa glSetFenceNV, glFinishFenceNV, glGetFenceivNV
 */
GLAPI GLboolean APIENTRY glTestFenceNV( GLuint fence )
{
    JS_APIENTRY_TIMER( glTestFenceNV );
    if ( _jsdpmState )
        _JS_DPM_API_ENTRY_1( glTestFenceNV, JSDPM_CALL_TYPE_SYNC, GL_TRUE, 1, fence, sizeof( GLuint ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE );

#ifndef JS_NO_ERROR_CHECK
    if ( !glIsFenceNV( fence ) )
    {
        _jsSetError( GL_INVALID_OPERATION );
        return GL_TRUE;
    }
#endif

    // get fence object
    PSGLcontext *LContext = _CurrentContext;
    JS_ASSERT( LContext );
    jsFenceObject* f = _jsGetFenceObject( LContext, fence );
    JS_ASSERT( f != NULL );

    return _jsPlatformTestFence( f );
}

/**
 *  @short Obtain the fence state for the specified fence
 *
 *  glGetFenceivNV() returns the fence state for the specified fence named <I><c>fence</c></I>
 *  in <I><c>params</c></I>.  <I><c>pname</c></I> indicates what kind of state information is
 *  requested and must be either <CODE>GL_FENCE_STATUS_NV</CODE> or
 *  <CODE>GL_FENCE_CONDITION_NV</CODE>.  The state information returned with
 *  <CODE>GL_FENCE_STATUS_NV</CODE> is the same as the boolean value returned
 *  by glTestFenceNV().  The state information returned with
 *  <CODE>GL_FENCE_CONDITION_NV</CODE> is the condition set for the fence by glSetFenceNV().
 *
 *  glGetFenceivNV() does not block program execution.
 *
 *  @param fence Specifies the name of the fence.
 *  @param pname Indicates the type of fence state information requested.
 *  @param params A pointer to an array in which the fence state information is stored.
 *
 *  @par Errors
Use glGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>GL_INVALID_OPERATION</CODE></TD>
<TD>The named fence does not exist.</TD>
</TR>
</TABLE>
 *
 *  @sa glSetFenceNV, glTestFenceNV
 */

GLAPI void APIENTRY glGetFenceivNV( GLuint fence, GLenum pname, GLint *params )
{
    JS_APIENTRY_TIMER( glGetFenceivNV );
    if ( _jsdpmState )
        _JS_DPM_API_ENTRY_3( glGetFenceivNV, JSDPM_CALL_TYPE_SYNC, , 3,
                             fence, sizeof( GLuint ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             pname, sizeof( GLenum ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM,
                             params, sizeof( GLint ), INPUT_OUTPUT, DPM_API_ARGUMENT_POINTER );

#ifndef JS_NO_ERROR_CHECK
    PSGLcontext *LContext = _CurrentContext;
    JS_ASSERT( LContext );
    if ( !_jsTexNameSpaceIsName( &LContext->fenceObjectNameSpace, fence ) )
    {
        _jsSetError( GL_INVALID_OPERATION );
        return;
    }
#endif

    switch ( pname )
    {
        case GL_FENCE_STATUS_NV:
            params[0] = glTestFenceNV( fence );
            break;
        case GL_FENCE_CONDITION_NV:
            params[0] = GL_ALL_COMPLETED_NV;
            break;
        default:
            _jsSetError( GL_INVALID_ENUM );
            break;
    }
}

/**
 *  @short Block PPU execution until condition of the specified fence is satisfied
 *
 *  glFinishFenceNV() is a fine-grained version of glFinish().
 *  glFinishFenceNV() blocks PPU execution until a selected condition of the specified fence is satisfied.
 *  There is only one allowed condition, <CODE>GL_ALL_COMPLETED_NV</CODE>,
 *  which means the PPU waits for the RSX&trade; to complete all calls up to and including the named fence.
 *
 *  @param fence Specifies the name of the fence.
 *
 *  @note A fence should not be left untested or unfinished for a long period.
 *  Fences are implemented with a signed 32-bit counter, so if more than (2<sup>31</sup>)-1
 *  intervening fences are issued before a fence is tested (glTestFenceNV()) or finished (glFinishFenceNV()), that fence
 *  may indicate an incorrect status.  In practice, this is unlikely, as it requires
 *  over a billion intervening fences.
 *
 *  @par Errors
Use glGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>GL_INVALID_OPERATION</CODE></TD>
<TD>The named fence does not exist.</TD>
</TR>
</TABLE>
 *
 *  @sa glFinish, glSetFenceNV, glTestFenceNV
 */
GLAPI void APIENTRY glFinishFenceNV( GLuint fence )
{
    JS_APIENTRY_TIMER( glFinishFenceNV );
    if ( _jsdpmState )
        _JS_DPM_API_ENTRY_1( glFinishFenceNV, JSDPM_CALL_TYPE_SYNC, , 1, fence, sizeof( GLuint ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE );

#ifndef JS_NO_ERROR_CHECK
    if ( !glIsFenceNV( fence ) )
    {
        _jsSetError( GL_INVALID_OPERATION );
        return;
    }
#endif

    // get fence object
    PSGLcontext *LContext = _CurrentContext;
    JS_ASSERT( LContext );
    jsFenceObject* f = _jsGetFenceObject( LContext, fence );

    _jsPlatformFinishFence( f );
}

/**
 *  @short Places a token (<i><c>fence</c></i>) into the RSX&trade; command stream as a program-execution marker.
 *
 *  glSetFenceNV() creates a fence (if one does not already exist) which acts as a
 *  token within the OpenGL command stream. Fences are typically used to cause the
 *  PPU to delay execution until the completion of commands in the graphics stream (RSX&trade;).
 *
 *  glSetFenceNV() assigns the named fence object
 *  a status of <CODE>GL_FALSE</CODE> and a condition, which must be <CODE>GL_ALL_COMPLETED_NV</CODE>.
 *  To satisfy the <CODE>GL_ALL_COMPLETED_NV</CODE> condition, the fence and all preceding
 *  OpenGL commands must be completed. When the condition is satisfied, the status
 *  of the fence is changed to <CODE>GL_TRUE</CODE>.
 *
 *  @param fence Specifies the name of the fence to use as a marker.
 *  @param condition Specifies the condition that must be met to set the status of the fence to <CODE>GL_TRUE</CODE>.
 *  The only allowed value for <I><c>condition</c></I> is <CODE>GL_ALL_COMPLETED_NV</CODE>.
 *
 *  @note A fence should not be left untested or unfinished for a long period.
 *  Fences are implemented with a signed 32-bit counter, so if more than (2<sup>31</sup>)-1
 *  intervening fences are issued before a fence is tested (glTestFenceNV()) or finished (glFinishFenceNV), that fence
 *  may indicate an incorrect status.  In practice, this is unlikely, as it requires
 *  over a billion intervening fences.
 *
 *  @note
 *  If you set the same fence object twice (call glSetFenceNV() twice with the same fence name),
 *  the second call clobbers the first.  Whatever status the first fence had, it
 *  is now set to <CODE>GL_TRUE</CODE>.  When that first fence (first glSetFenceNV() call)
 *  is actually encountered by the RSX&trade;, its completion no longer changes its status, so it is basically
 *  ignored. The second call sets a new fence object with an initial status of <CODE>GL_FALSE</CODE>
 *  that may be changed by execution.
 *
 *  @par Errors
Use glGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>GL_INVALID_OPERATION</CODE></TD>
<TD>The named fence does not exist.</TD>
</TR>
<TR>
<TD><CODE>GL_INVALID_ENUM</CODE></TD>
<TD><I><c>condition</c></I> is not <CODE>GL_ALL_COMPLETED_NV</CODE>.</TD>
</TR>
</TABLE>
 *
 *  @sa glTestFenceNV, glGenFencesNV, glDeleteFencesNV, glFinishFenceNV, glGetFenceivNV
 */
GLAPI void APIENTRY glSetFenceNV( GLuint fence, GLenum condition )
{
    JS_APIENTRY_TIMER( glSetFenceNV );
    if ( _jsdpmState )
        _JS_DPM_API_ENTRY_2( glSetFenceNV, JSDPM_CALL_TYPE_SYNC, , 2,
                             fence, sizeof( GLuint ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             condition, sizeof( GLenum ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM );

#ifndef JS_NO_ERROR_CHECK
    if ( fence == 0 )
    {
        _jsSetError( GL_INVALID_OPERATION );
        return;
    }

    if ( condition != GL_ALL_COMPLETED_NV )
    {
        _jsSetError( GL_INVALID_ENUM );
        return;
    }
#endif

    // get fence object
    PSGLcontext *LContext = _CurrentContext;
    JS_ASSERT( LContext );
    _jsTexNameSpaceCreateNameLazy( &LContext->fenceObjectNameSpace, fence );
    jsFenceObject* f = _jsGetFenceObject( LContext, fence );
    JS_ASSERT( f );

    f->isSet = GL_TRUE;
    _jsPlatformSetFence( f );
}

/**
 *  @short Delete event names
 *
 *  glDeleteEventsSCE() deletes <I><c>n</c></I> events named by the elements of the array <I><c>events</c></I>.
 *  After an event is deleted, it no longer occurs, its name is unused, and its name is free for reuse
 *  by glGenEventsSCE().
 *
 *  @param n Specifies the number of event names to be deleted.
 *
 *  @param events A pointer to an array of events to be deleted.
 *
 *  @par Errors
Use glGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>GL_INVALID_VALUE</CODE></TD>
<TD><I><c>n</c></I> is negative.</TD>
</TR>
</TABLE>
 *
 *  @note There are a limited number of events available, currently 128.
 *  Deleting events makes them available for reuse.
 *
 *  @sa glGenEventsSCE
 */
GLAPI void APIENTRY glDeleteEventsSCE( GLsizei n, const GLuint *events )
{
    JS_APIENTRY_TIMER( glDeleteEventsSCE );
    if ( _jsdpmState )
    {
        _JS_DPM_API_ENTRY_2( glDeleteEventsSCE, JSDPM_CALL_TYPE_SYNC, , 2,
                             n, sizeof( GLsizei ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             events, ( n * sizeof( GLuint ) ), INPUT_BY_REFERENCE, DPM_API_ARGUMENT_POINTER );
    }

    PSGLcontext *LContext = _CurrentContext;
    JS_ASSERT( LContext );
    _jsTexNameSpaceDeleteNames( &LContext->eventObjectNameSpace, n, events );
}

/**
 *  @short Generate event names
 *
 *  glGenEventsSCE() returns <I><c>n</c></I> unused event names in <I><c>events</c></I>. There is no guarantee that
 *  the names form a contiguous set of integers. An event may be in one of two states: "set"
 *  and "reset". Events are initially in the "reset" state.
 *
 *  Events names returned by a call to glGenEventsSCE() are not returned by subsequent calls,
 *  unless they are first deleted with glDeleteEventsSCE().
 *
 *  @param n Specifies the number of event names to be generated.
 *
 *  @param events A pointer to an array in which the generated event names are stored.
 *
 *  @par Errors
Use glGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>GL_INVALID_VALUE</CODE></TD>
<TD><I><c>n</c></I> is negative.</TD>
</TR>
</TABLE>
 *
 *  @note There are a limited number of events available, currently 128.
 *  Deleting events makes them available for reuse.
 *
 *  @sa glSetEventSCE, glDeleteEventsSCE
 */
GLAPI void APIENTRY glGenEventsSCE( GLsizei n, GLuint *events )
{
    JS_APIENTRY_TIMER( glGenEventsSCE );
    if ( _jsdpmState )
    {
        _JS_DPM_API_ENTRY_2( glGenEventsSCE, JSDPM_CALL_TYPE_SYNC, , 2,
                             n, sizeof( GLsizei ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             events, ( n * sizeof( GLuint ) ), INPUT_OUTPUT, DPM_API_ARGUMENT_POINTER );
    }

    PSGLcontext *LContext = _CurrentContext;
    JS_ASSERT( LContext );
    _jsTexNameSpaceGenNames( &LContext->eventObjectNameSpace, n, events );
}

/**
 *  @short Assigns "set" as the value of an event and signals resumption of RSX&trade; execution
 *
 *  glSetEventSCE() assigns "set" as the value of an event.  If the value of the event changes, then it also
 *  acts as a signal to restart RSX&trade; execution of the graphics stream of commands.
 *  glSetEventSCE() is usually called after glWaitEventSCE() has put the RSX&trade; into a wait state.
 *
 *  A new event object is created if an event named <I><c>event</c></I> does not exist.  
 *
 *  @param event Specifies the name of the event
 *
 *  @note
 Using glSetEventSCE() in the same thread as
 glWaitEventSCE() can lead to deadlock.
 The primary use for events is to allow SPUs and the RSX&trade;
 to synchronize access to mapped buffer objects.
 Therefore, you are much more likely to use glSetMappedEventSCE() from the SPU
 to set an event.
 *
 *  @par Errors
Use glGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>GL_INVALID_OPERATION</CODE></TD>
<TD><I><c>event</c></I> is 0.</TD>
</TR>
<TR>
<TD><CODE>GL_OUT_OF_MEMORY</CODE></TD>
<TD>Insufficient memory to create new event.</TD>
</TR>
</TABLE>
 *
 *  @sa glResetEventSCE, glWaitEventSCE, glSetMappedEventSCE
 */
GLAPI void APIENTRY glSetEventSCE( GLuint event )
{
    JS_APIENTRY_TIMER( glSetEventSCE );
    if ( _jsdpmState )
        _JS_DPM_API_ENTRY_1( glSetEventSCE, JSDPM_CALL_TYPE_SYNC, , 1, event, sizeof( GLuint ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE );

#ifndef JS_NO_ERROR_CHECK
    if ( event == 0 )
    {
        _jsSetError( GL_INVALID_OPERATION );
        return;
    }
#endif

    PSGLcontext *LContext = _CurrentContext;
    JS_ASSERT( LContext );
    _jsTexNameSpaceCreateNameLazy( &LContext->eventObjectNameSpace, event );
    jsEventObject* eventObject = _jsGetEventObject( LContext, event );
#ifndef JS_NO_ERROR_CHECK
    if ( eventObject == NULL )
    {
        _jsSetError( GL_OUT_OF_MEMORY );
        return;
    }
#endif

    _jsPlatformSetEvent( eventObject );
}

/**
 *  @short Changes state of an event to "reset"
 *
 *  glResetEventSCE() assigns "reset" as the state of an event, which is its default state.
 *
 *  A new event object is created if an event named <I><c>event</c></I> does not exist.  
 *
 *  @param event Specifies the name of the event
 *
 *  @par Errors
Use glGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>GL_INVALID_OPERATION</CODE></TD>
<TD><I><c>event</c></I> is 0.</TD>
</TR>
<TR>
<TD><CODE>GL_OUT_OF_MEMORY</CODE></TD>
<TD>Insufficient memory to create new event.</TD>
</TR>
</TABLE>
 *
 *  @sa glSetEventSCE, glWaitEventSCE
 */
GLAPI void APIENTRY glResetEventSCE( GLuint event )
{
    JS_APIENTRY_TIMER( glResetEventSCE );
    if ( _jsdpmState )
        _JS_DPM_API_ENTRY_1( glResetEventSCE, JSDPM_CALL_TYPE_SYNC, , 1, event, sizeof( GLuint ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE );

#ifndef JS_NO_ERROR_CHECK
    if ( event == 0 )
    {
        _jsSetError( GL_INVALID_OPERATION );
        return;
    }
#endif

    PSGLcontext *LContext = _CurrentContext;
    JS_ASSERT( LContext );
    _jsTexNameSpaceCreateNameLazy( &LContext->eventObjectNameSpace, event );
    jsEventObject* eventObject = _jsGetEventObject( LContext, event );
#ifndef JS_NO_ERROR_CHECK
    if ( eventObject == NULL )
    {
        _jsSetError( GL_OUT_OF_MEMORY );
        return;
    }
#endif

    _jsPlatformResetEvent( eventObject );
}

/**
 *  @short Causes RSX&trade; execution to wait until the event state is set
 *
 *  Calling glWaitEventSCE() places a token into the RSX&trade; command stream.
 *  When the token is encountered, if the state of the event is "reset",
 *  then glWaitEventSCE() causes the RSX&trade; to wait.  (The RSX&trade; processing of
 *  the "wait" token is asynchronous from execution of the PPU program.)
 *  Execution of further RSX&trade; graphics commands is delayed.  When the event state is changed, with either the
 *  function glSetEventSCE() or glSetMappedEventSCE(), then RSX&trade; execution resumes.
 *
 *  <I><c>autoReset</c></I> is a boolean value that indicates what the event state will be after encountering
 *  the event that reawakens the RSX&trade;.  <CODE>GL_TRUE</CODE> for <I><c>autoReset</c></I>
 *  means the post-awakening event state will be "reset"; <CODE>GL_FALSE</CODE> means the event state remains "set".
 *
 *  @param event Specifies the name of the event
 *  @param autoReset Specifies the state the event will be after a set event occurs
 *
 *  @par Errors
Use glGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>GL_INVALID_OPERATION</CODE></TD>
<TD>The named event does not exist or <I><c>event</c></I> is 0.</TD>
</TR>
</TABLE>
 *
 *  @sa glDeleteEventsSCE, glGenEventsSCE, glSetEventSCE, glSetMappedEventSCE
 */

GLAPI void APIENTRY glWaitEventSCE( GLuint event, GLboolean autoReset )
{
    JS_APIENTRY_TIMER( glWaitEventSCE );
    if ( _jsdpmState )
        _JS_DPM_API_ENTRY_2( glWaitEventSCE, JSDPM_CALL_TYPE_SYNC, , 2,
                             event, sizeof( GLuint ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             autoReset, sizeof( GLboolean ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE );

#ifndef JS_NO_ERROR_CHECK
    if ( event == 0 )
    {
        _jsSetError( GL_INVALID_OPERATION );
        return;
    }
#endif

    PSGLcontext *LContext = _CurrentContext;
    JS_ASSERT( LContext );
    _jsTexNameSpaceCreateNameLazy( &LContext->eventObjectNameSpace, event );
    jsEventObject* eventObject = _jsGetEventObject( LContext, event );
#ifndef JS_NO_ERROR_CHECK
    if ( eventObject == NULL ) // creation may fail if we're out of resources
    {
        _jsSetError( GL_OUT_OF_MEMORY );
        return;
    }
#endif

    _jsPlatformWaitEvent( eventObject, autoReset );
}

/**
 *  @short Returns a memory-mapped pointer that corresponds to an event in RSX&trade; memory
 *
 *  glMapEventSCE() allows mapped access to a location in RSX&trade; memory that corresponds to the
 *  event named <I><c>event</c></I>.  This location is a direct handle to the event that is
 *  typically intended to be passed onto the SPU so that the SPU can synchronize with the RSX&trade;.
 *
 *  A new event object is created if an event named <I><c>event</c></I> does not exist.  
 *
 *  @param event Specifies the name of the event
 *
 *  @return A pointer in local address space, mapped to an event in RSX&trade; memory
 *
 *  @par Errors
Use glGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>GL_INVALID_OPERATION</CODE></TD>
<TD><I><c>event</c></I> is 0.</TD>
</TR>
<TR>
<TD><CODE>GL_OUT_OF_MEMORY</CODE></TD>
<TD>Insufficient memory to create new event.</TD>
</TR>
</TABLE>
 *
 *  @sa glSetMappedEventSCE, glGenEventsSCE
 */

GLAPI unsigned long long APIENTRY glMapEventSCE( GLuint event )
{
    JS_APIENTRY_TIMER( glMapEventSCE );
    if ( _jsdpmState )
        _JS_DPM_API_ENTRY_1( glMapEventSCE, JSDPM_CALL_TYPE_SYNC, 0, 1, event, sizeof( GLuint ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE );

#ifndef JS_NO_ERROR_CHECK
    if ( event == 0 )
    {
        _jsSetError( GL_INVALID_OPERATION );
        return 0;
    }
#endif

    PSGLcontext *LContext = _CurrentContext;
    JS_ASSERT( LContext );
    _jsTexNameSpaceCreateNameLazy( &LContext->eventObjectNameSpace, event );
    jsEventObject* eventObject = _jsGetEventObject( LContext, event );
#ifndef JS_NO_ERROR_CHECK
    if ( eventObject == NULL )
    {
        _jsSetError( GL_OUT_OF_MEMORY );
        return 0;
    }
#endif

    return _jsPlatformMapEvent( eventObject );
}

/**
 *  @short Sets the event state in a memory-mapped pointer
 *
 *  glSetMappedEventSCE() acts as a signal to resume RSX&trade; execution after glWaitEventSCE()
 *  has put the RSX&trade; into a wait state.  glSetMappedEventSCE() identifies the associated event
 *  by a memory-mapped location in local address space previously provided by glMapEventSCE().
 *
 *  The SPU can use glSetMappedEventSCE() to synchronize with
 *  the RSX&trade;, but the SPU can not use glSetEventSCE().
 *
 *  A new event object is created if an event named <I><c>mappedEvent</c></I> does not exist.  
 *
 *  @param mappedEvent A pointer in local address space, mapped to an event in RSX&trade; memory
 *
 *  @par Errors
Use glGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>GL_INVALID_OPERATION</CODE></TD>
<TD><I><c>mappedEvent</c></I> is 0.</TD>
</TR>
<TR>
<TD><CODE>GL_OUT_OF_MEMORY</CODE></TD>
<TD>Insufficient memory to create new event.</TD>
</TR>
</TABLE>
 *
 *  @note
 *  glSetMappedEventSCE() may be called from either an SPU or the PPU.
 *
 *  @note
 *  On the SPU, glSetMappedEventSCE() is a non-blocking DMA fence that ensures 
 *  completion of prior DMA transfers with the PSGL_WRITE_TAG, using, for example,
 *  cellDmaLargePut() from libdma.  (See the <i>libdma Reference</i> for further 
 *  information.) 
 *
 *  @sa glMapEventSCE, glWaitEventSCE, glSetEventSCE
 */

GLAPI void APIENTRY glSetMappedEventSCE( unsigned long long mappedEvent )
{
    JS_APIENTRY_TIMER( glSetMappedEventSCE );
    if ( _jsdpmState )
        _JS_DPM_API_ENTRY_1( glSetMappedEventSCE, JSDPM_CALL_TYPE_SYNC, , 1, mappedEvent, sizeof( unsigned long long ), INPUT_BY_VALUE, DPM_API_ARGUMENT_POINTER );

    _jsPlatformSetMappedEvent( mappedEvent );
}

/** @}  Modes  */
