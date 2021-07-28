/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 *                Copyright (C) 2009 Sony Computer Entertainment Inc.
 *                                               All Rights Reserved.
 */


/*
 * Implementation for the glQuery* extension.
 */

#include <PSGL/psgl.h>
#include <PSGL/private.h>
#include <PSGL/Debug.h>
#include <PSGL/DPM2.h>

jsQuery* _jsCreateQuery( void )
{
    GLuint size = sizeof( jsQuery ) + _jsPlatformQuerySize();

    jsQuery* query = ( jsQuery * )jsMalloc( size );
#ifndef JS_NO_ERROR_CHECK
    if ( query == NULL )
    {
        _jsSetError( GL_OUT_OF_MEMORY );
        return NULL;
    }
#endif

    __builtin_memset( query, 0, size );
    _jsPlatformCreateQuery( query );

    return query;
}

void _jsDestroyQuery( jsQuery* query )
{
    JS_ASSERT( query != NULL );

    _jsPlatformDestroyQuery( query );
    jsFree( query );
}

// API functions ----------------------------------------

/** @addtogroup OpenGL
 *@{
 */

/**
@short Generate query names

glGenQueriesARB() returns <I><c>n</c></I> query names in
<I><c>ids</c></I>.  There is no guarantee that the names form a contiguous
set of integers.  However, it is guaranteed that none of the returned names
was in use immediately before the call to glGenQueriesARB().

Render buffer names returned by a call to glGenQueriesARB() are not
returned by subsequent calls unless they are first deleted with
glDeleteQueriesARB().

The total number of queries generated cannot exceed 800.  If this is
exceeded, <CODE>GL_OUT_OF_MEMORY</CODE> is returned. 

@param       n Specifies the number of query names to be generated.

@param       ids Specifies an array in which the generated
query names are stored.

@note
As of SDK 2.5.0, PSGL uses reports in main memory by default, with
the maximum number of reports set to 5000.
To allow for more queries in your PSGL version, you can 
increase the value of <c>JSGCM_MM_MAX_TOTAL_QUERIES</c> up to 65,000, 
by editing JSGcmResource.h.
@code
#define JSGCM_MM_MAX_TOTAL_QUERIES 5000 
@endcode
@par
However, increasing the number of queries also increases the PSGL library 
internal storage for queries management, thereby increasing the size of 
the library.  A value of 5000 for user queries should be more than adequate 
for all queries in a given frame.
@par
If, due to memory constraints, you want to revert to reports in local memory, 
comment out the main memory reports define shown below, and rebuild 
PSGL as described in the "Occlusion Queries and Conditional Rendering" 
section of the <i>PlayStation&reg; Graphics Library Programming Guide</i>. 
@code
#define JSGCM_USE_MAIN_MEMORY_REPORTS 
#ifdef JSGCM_USE_MAIN_MEMORY_REPORTS 
        #define JSGCM_MAX_USER_QUERIES JSGCM_MM_MAX_USER_QUERIES 
#else 
        #define JSGCM_MAX_USER_QUERIES JSGCM_LM_MAX_USER_QUERIES 
#endif 
@endcode
@par  
If you use GPAD, and you create a custom version of PSGL that uses 
local reports, You need to be sure that the maximum number of
user queries (defined by <c>JSGCM_LM_MAX_USER_QUERIES</c>) is less
than 1024.
@par
GPAD does not use main memory reports and is not affected by the 
number of main memory queries, as it is by the local memory reports.   
@par
The PSGL variables directly affected by the number of queries are: 
@code
GLboolean query[JSGCM_MAX_USER_QUERIES]; 
GLuint queryFence[JSGCM_MAX_USER_QUERIES]; 
@endcode

@par Errors
Use glGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>GL_INVALID_VALUE</CODE></TD>
<TD><I><c>n</c></I> is negative.</TD>
</TR>
<TR>
<TD><CODE>GL_INVALID_OPERATION</CODE></TD>
<TD>A query is active.</TD>
</TR>
<TR>
<TD><CODE>GL_OUT_OF_MEMORY</CODE></TD>
<TD>The maximum number of queries is exceeded.</TD>
</TABLE>

@sa glDeleteQueriesARB
 */
GLAPI void APIENTRY glGenQueriesARB( GLsizei n, GLuint *ids )
{
    JS_APIENTRY_TIMER( glGenQueriesARB );
#ifdef JS_DPM
    if ( _jsdpmState )
    {
        _JS_DPM_API_ENTRY_2(
            glGenQueriesARB, JSDPM_CALL_TYPE_STATE_GENERAL, , 2,
            n, sizeof( GLsizei ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
            ids, ( n*sizeof( GLuint ) ), INPUT_OUTPUT, DPM_API_ARGUMENT_POINTER );
    }
#endif

    PSGLcontext*	LContext = _CurrentContext;
    JS_ASSERT( LContext );

    _jsTexNameSpaceGenNames( &LContext->queryNameSpace, n, ids );

#ifdef JS_DPM
    _JS_DPM_API_RETURN( 2, ids, ( n*sizeof( GLuint ) ) );
#endif
}

/**
@short Delete query names

glDeleteQueriesARB() deletes <I><c>n</c></I> queries named by the elements
of the array <I><c>ids</c></I>.  After a query is deleted,
its name is free for reuse (for example
by glGenQueriesARB()).

glDeleteQueriesARB() silently ignores 0&rsquo;s and names that do not
correspond to existing queries.

@param       n Specifies the number of query names to be deleted.

@param       ids Specifies an array of queries to be deleted.

@par Errors
Use glGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>GL_INVALID_VALUE</CODE></TD>
<TD><I><c>n</c></I> is negative.</TD>
</TR>
<TR>
<TD><CODE>GL_INVALID_OPERATION</CODE></TD>
<TD>A query is active.</TD>
</TR>
</TABLE>


@sa glGenQueriesARB
 */
GLAPI void APIENTRY glDeleteQueriesARB( GLsizei n, const GLuint *ids )
{
    JS_APIENTRY_TIMER( glDeleteQueriesARB );
#ifdef JS_DPM
    if ( _jsdpmState )
    {
        _JS_DPM_API_ENTRY_2(
            glDeleteQueriesARB, JSDPM_CALL_TYPE_STATE_GENERAL, , 2,
            n, sizeof( GLsizei ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
            ids, ( n*sizeof( GLuint ) ), INPUT_BY_REFERENCE, DPM_API_ARGUMENT_POINTER );
    }
#endif

    PSGLcontext*	LContext = _CurrentContext;
    JS_ASSERT( LContext );

#ifndef JS_NO_ERROR_CHECK
    // illegal when a query is active
    if ( LContext->samplesPassedQuery )
    {
        _jsSetError( GL_INVALID_OPERATION );
        return;
    }
#endif

    _jsTexNameSpaceDeleteNames( &LContext->queryNameSpace, n, ids );
}

/**
@short Test for query existence

glIsQueryARB() tests a name to determine if it represents
a query.

@param       id Specifies the query name.

@return Returns <CODE>GL_TRUE</CODE> if <I><c>id</c></I> is the
name of a query object and returns <CODE>GL_FALSE</CODE> otherwise.

@sa glGenQueriesARB, glBeginQueryARB
 */
GLAPI GLboolean APIENTRY glIsQueryARB( GLuint id )
{
    JS_APIENTRY_TIMER( glIsQueryARB );
#ifdef JS_DPM
    if ( _jsdpmState )
    {
        _JS_DPM_API_ENTRY_1(
            glIsQueryARB, JSDPM_CALL_TYPE_STATE_GENERAL, GL_FALSE, 1,
            id, sizeof( GLuint ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE );
    }
#endif

    PSGLcontext*	LContext = _CurrentContext;
    JS_ASSERT( LContext );

    return _jsTexNameSpaceIsName( &LContext->queryNameSpace, id );;
}

/**
@short Begin a query

glBeginQueryARB() begins an occlusion query.  The current samples passed
counter is reset to 0, and the active query is set to <I><c>id</c></I>.  While
the query is active, the samples passed counter is incremented for each
fragment that passes the depth test.  An active query ends when glEndQueryARB()
is called.  Queries may not be nested.

The result of a query may be retrieved with glGetQueryObjectivARB() or
glGetQueryObjectuivARB().  A query can also be used for conditional
rendering with glBeginConditionalRenderingSCE() and glEndConditionalRenderingSCE().


@param target Must be
<CODE>GL_SAMPLES_PASSED_ARB</CODE>.

@param id Specifies the query name.

@par Errors
Use glGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>GL_INVALID_ENUM</CODE></TD>
<TD><I><c>target</c></I> is not <CODE>GL_SAMPLES_PASSED_ARB</CODE>.</TD>
</TR>
<TR>
<TD><CODE>GL_OUT_OF_MEMORY</CODE></TD>
<TD>Insufficient resources were available to create the query.</TD>
</TR>
<TR>
<TD><CODE>GL_INVALID_OPERATION</CODE></TD>
<TD>A query is already active.</TD>
</TR>
</TABLE>

@sa glGenQueriesARB, glEndQueryARB, glBeginConditionalRenderingSCE, glEndConditionalRenderingSCE
 */
GLAPI void APIENTRY glBeginQueryARB( GLenum target, GLuint id )
{
    JS_APIENTRY_TIMER( glBeginQueryARB );
#ifdef JS_DPM
    if ( _jsdpmState )
    {
        _JS_DPM_API_ENTRY_2(
            glBeginQueryARB, JSDPM_CALL_TYPE_GET, , 2,
            target, sizeof( GLenum ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM,
            id, sizeof( GLuint ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE );
    }
#endif

    PSGLcontext*	LContext = _CurrentContext;
    JS_ASSERT( LContext );

    switch ( target )
    {
        case GL_SAMPLES_PASSED_ARB:
#ifndef JS_NO_ERROR_CHECK
            if ( LContext->samplesPassedQuery )
            {
                // only one active query at a time
                _jsSetError( GL_INVALID_OPERATION );
                return;
            }
#endif
            break;

        default:
            _jsSetError( GL_INVALID_ENUM );
            return;
    }

#ifndef JS_NO_ERROR_CHECK
    if ( id == 0 )
    {
        _jsSetError( GL_INVALID_OPERATION );
        return;
    }
#endif

    _jsTexNameSpaceCreateNameLazy( &LContext->queryNameSpace, id );
    jsQuery* query = _jsGetQuery( LContext, id );
#ifndef JS_NO_ERROR_CHECK
    if ( query == NULL )
    {
        _jsSetError( GL_OUT_OF_MEMORY );
        return;
    }
#endif

    query->result = 0;
    query->queryType = target;

    if ( target == GL_SAMPLES_PASSED_ARB )
        LContext->samplesPassedQuery = id;
    else
        LContext->pmQuery = id;

    _jsPlatformBeginQuery( query );
}

/**
@short End an occlusion query

glEndQueryARB() ends the currently active query started with glBeginQueryARB().

The result of a query may be retrieved with glGetQueryObjectivARB() or
glGetQueryObjectuivARB().  A query can also be used for conditional
rendering with glBeginConditionalRenderingSCE() and glEndConditionalRenderingSCE().

@param target Must be <CODE>GL_SAMPLES_PASSED_ARB</CODE>. If you are linking
against a DPM version of PSGL then the following enums is also available:
<CODE> GL_EXPERIMENT_STOP_QUERY_SCE </CODE>.

@par Errors
Use glGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>GL_INVALID_ENUM</CODE></TD>
<TD><I><c>target</c></I> is not <CODE>GL_SAMPLES_PASSED_ARB</CODE>.</TD>
</TR>
<TR>
<TD><CODE>GL_INVALID_OPERATION</CODE></TD>
<TD>No query is active.</TD>
</TR>
</TABLE>

@sa glBeginQueryARB

 */
GLAPI void APIENTRY glEndQueryARB( GLenum target )
{
    JS_APIENTRY_TIMER( glEndQueryARB );
#ifdef JS_DPM
    if ( _jsdpmState )
    {
        _JS_DPM_API_ENTRY_1(
            glEndQueryARB, JSDPM_CALL_TYPE_STATE_GENERAL, , 1,
            target, sizeof( GLenum ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM );
    }
#endif

    PSGLcontext*	LContext = _CurrentContext;
    JS_ASSERT( LContext );
    jsQuery* query;

    switch ( target )
    {
        case GL_SAMPLES_PASSED_ARB:
#ifndef JS_NO_ERROR_CHECK
            if ( LContext->samplesPassedQuery == 0 )
            {
                // no query active
                _jsSetError( GL_INVALID_OPERATION );
                return;
            }
#endif
            query = _jsGetQuery( LContext, LContext->samplesPassedQuery );
            break;
        default:
            _jsSetError( GL_INVALID_ENUM );
            return;
    }

    _jsPlatformEndQuery( query );

    if ( target == GL_SAMPLES_PASSED_ARB )
        LContext->samplesPassedQuery = 0;
    else
        LContext->pmQuery = 0;
}

/**
@short Get query configuration

glGetQueryivARB() returns the query configuration.  If <I><c>pname</c></I>
is <CODE>GL_CURRENT_QUERY_ARB</CODE>, the name of the active
occlusion query (or <CODE>0</CODE> if no query is active) is
returned in <I><c>params</c></I>.  If <I><c>pname</c></I> is <CODE>GL_QUERY_COUNTER_BITS_ARB</CODE>,
the number of bits in the counter for <I><c>target</c></I> is returned
in <I><c>params</c></I>.

To retrieve the results of a query, use glGetQueryObjectivARB()
or glGetQueryObjectuivARB().

@param target If you are not linking with the DPM version of PSGL, <I><c>target</c></I> must be
<CODE>GL_SAMPLES_PASSED_ARB</CODE>. If you are linking
against a DPM version of PSGL, performance queries can also be specified.  See glBeginQueryARB() for a
complete list of valid performance queries.

@param pname Specifies the parameter to return.  Must be one of
<CODE>GL_CURRENT_QUERY_ARB</CODE> or
<CODE>GL_QUERY_COUNTER_BITS_ARB</CODE>.
@param params Specifies an address for the returned parameter value.

@par Errors
Use glGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>GL_INVALID_ENUM</CODE></TD>
<TD><I><c>target</c></I> is not <CODE>GL_SAMPLES_PASSED_ARB</CODE>.</TD>
</TR>
<TR>
<TD><CODE>GL_INVALID_ENUM</CODE></TD>
<TD><I><c>pname</c></I> is not <CODE>GL_CURRENT_QUERY_ARB</CODE> or
<CODE>GL_QUERY_COUNTER_BITS_ARB</CODE>. </TD>
</TR>
</TABLE>

@sa glGetQueryObjectivARB, glGetQueryObjectuivARB
 */
GLAPI void APIENTRY glGetQueryivARB( GLenum target, GLenum pname, GLint* params )
{
    JS_APIENTRY_TIMER( glGetQueryivARB );
#ifdef JS_DPM
    if ( _jsdpmState )
    {
        _JS_DPM_API_ENTRY_3(
            glGetQueryivARB, JSDPM_CALL_TYPE_STATE_GENERAL, , 3,
            target, sizeof( GLenum ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM,
            pname, sizeof( GLenum ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM,
            params, ( 1*sizeof( GLuint ) ), INPUT_OUTPUT, DPM_API_ARGUMENT_POINTER );
    }
#endif

    switch ( target )
    {
        case GL_SAMPLES_PASSED_ARB:
            break;

        default:
            _jsSetError( GL_INVALID_ENUM );
            return;
    }

    PSGLcontext*	LContext = _CurrentContext;
    JS_ASSERT( LContext );
    switch ( pname )
    {
        case GL_CURRENT_QUERY_ARB:
            *params = LContext->samplesPassedQuery;
            break;
        case GL_QUERY_COUNTER_BITS_ARB:
            _jsPlatformGetQueryConfig( pname, ( GLuint* )params );
            break;
        default:
            _jsSetError( GL_INVALID_ENUM );
            return;
    }

#ifdef JS_DPM
    _JS_DPM_API_RETURN( 1, params, ( 1*sizeof( GLuint ) ) );
#endif
}

/**
@short Get the result of a query

glGetQueryObjectivARB() returns the result of a query previously
defined by a glBeginQueryARB() and glEndQueryARB() block.  If <I><c>pname</c></I>
is <CODE>GL_QUERY_RESULT_ARB</CODE>, the value of the samples passed
counter is returned.  This result may be returned asynchronously with
respect to the OpenGL process, so it is possible that glGetQueryObjectivARB()
may block until the counter result is available.  If <I><c>pname</c></I>
is <CODE>GL_QUERY_RESULT_AVAILABLE_ARB</CODE>, <CODE>GL_TRUE</CODE>
is returned if the query result for <CODE>GL_QUERY_RESULT_ARB</CODE>
would be returned immediately without blocking, otherwise <CODE>GL_FALSE</CODE>
is returned.

If the purpose of a query is to implement conditional rendering, this
can also be accomplished without reading a value back by using
glBeginConditionalRenderingSCE() and glEndConditionalRenderingSCE().

@param id Specifies the query name.
@param pname Specifies the query parameter to return.  Must be one of
<CODE>GL_QUERY_RESULT_ARB</CODE> or
<CODE>GL_QUERY_RESULT_AVAILABLE_ARB</CODE>.
@param params Specifies an address for the returned parameter value.

@par Errors
Use glGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>GL_INVALID_OPERATION</CODE></TD>
<TD><I><c>id</c></I> does not represent a valid query.</TD>
</TR>
<TR>
<TD><CODE>GL_INVALID_ENUM</CODE></TD>
<TD><I><c>pname</c></I> is not one of
<CODE>GL_QUERY_RESULT_ARB</CODE> or
<CODE>GL_QUERY_RESULT_AVAILABLE_ARB</CODE>.</TD>
</TR>
</TABLE>

@sa glGetQueryObjectuivARB, glBeginConditionalRenderingSCE, glEndConditionalRenderingSCE, glGetQueryObjectul64vARB

 */
GLAPI void APIENTRY glGetQueryObjectivARB( GLuint id, GLenum pname, GLint* params )
{
    JS_APIENTRY_TIMER( glGetQueryObjectivARB );
#ifdef JS_DPM
    if ( _jsdpmState )
    {
        _JS_DPM_API_ENTRY_3(
            glGetQueryObjectivARB, JSDPM_CALL_TYPE_STATE_GENERAL, , 3,
            id, sizeof( GLenum ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
            pname, sizeof( GLenum ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM,
            params, ( 1*sizeof( GLint ) ), INPUT_OUTPUT, DPM_API_ARGUMENT_POINTER );
    }
#endif

    glGetQueryObjectuivARB( id, pname, ( GLuint * )params );

#ifdef JS_DPM
    _JS_DPM_API_RETURN( 1, params, ( 1*sizeof( GLint ) ) );
#endif
}

/**
@short Get the result of a query

glGetQueryObjectuivARB() returns the result of a query previously
defined by a glBeginQueryARB() and glEndQueryARB() block.  If <I><c>pname</c></I>
is <CODE>GL_QUERY_RESULT_ARB</CODE>, the value of the samples passed
counter is returned.  This result may be returned asynchronously with
respect to the OpenGL process, so it is possible that glGetQueryObjectuivARB()
may block until the counter result is available.  If <I><c>pname</c></I>
is <CODE>GL_QUERY_RESULT_AVAILABLE_ARB</CODE>, <CODE>GL_TRUE</CODE>
is returned if the query result for <CODE>GL_QUERY_RESULT_ARB</CODE>
would be returned immediately without blocking, otherwise <CODE>GL_FALSE</CODE>
is returned.

If the purpose of a query is to implement conditional rendering, this
can also be accomplished without reading a value back by using
glBeginConditionalRenderingSCE() and glEndConditionalRenderingSCE().

@param id Specifies the query name.
@param pname Specifies the query parameter to return.  Must be one of
<CODE>GL_QUERY_RESULT_ARB</CODE> or
<CODE>GL_QUERY_RESULT_AVAILABLE_ARB</CODE>.
@param params Specifies an address for the returned parameter value.

@par Errors
Use glGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>GL_INVALID_OPERATION</CODE></TD>
<TD><I><c>id</c></I> does not represent a valid query.</TD>
</TR>
<TR>
<TD><CODE>GL_INVALID_ENUM</CODE></TD>
<TD><I><c>pname</c></I> is not one of
<CODE>GL_QUERY_RESULT_ARB</CODE> or
<CODE>GL_QUERY_RESULT_AVAILABLE_ARB</CODE>.</TD>
</TR>
</TABLE>

@sa glGetQueryObjectivARB, glBeginConditionalRenderingSCE, glEndConditionalRenderingSCE, glGetQueryObjectul64vARB

 */
GLAPI void APIENTRY glGetQueryObjectuivARB( GLuint id, GLenum pname, GLuint* params )
{
    JS_APIENTRY_TIMER( glGetQueryObjectuivARB );
#ifdef JS_DPM
    if ( _jsdpmState )
    {
        _JS_DPM_API_ENTRY_3(
            glGetQueryObjectivARB, JSDPM_CALL_TYPE_STATE_GENERAL, , 3,
            id, sizeof( GLenum ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
            pname, sizeof( GLenum ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM,
            params, ( 1*sizeof( GLuint ) ), INPUT_OUTPUT, DPM_API_ARGUMENT_POINTER );
    }
#endif

    PSGLcontext*	LContext = _CurrentContext;
    JS_ASSERT( LContext );

#ifndef JS_NO_ERROR_CHECK
    if ( !_jsTexNameSpaceIsName( &LContext->queryNameSpace, id ) )
    {
        // not a valid query
        _jsSetError( GL_INVALID_OPERATION );
        return;
    }
#endif

    jsQuery* query = _jsGetQuery( LContext, id );
#ifndef JS_NO_ERROR_CHECK
    switch ( pname )
    {
        case GL_QUERY_RESULT_ARB:
        case GL_QUERY_RESULT_AVAILABLE_ARB:
            if ( LContext->samplesPassedQuery != id )
#endif
                _jsPlatformGetQuery( query, pname, ( GLuint* )params );
#ifndef JS_NO_ERROR_CHECK
            else
                // query is still in progress
                _jsSetError( GL_INVALID_OPERATION );
            break;
        default:
            _jsSetError( GL_INVALID_ENUM );
            return;
    }
#endif

#ifdef JS_DPM
    _JS_DPM_API_RETURN( 1, params, ( 1*sizeof( GLuint ) ) );
#endif
}

/**
@short Get the result of a query using a 64 bit value

glGetQueryObjectul64vARB() returns the result of a query previously
defined by a glBeginQueryARB() and glEndQueryARB() block.  If <I><c>pname</c></I>
is <CODE>GL_QUERY_RESULT_ARB</CODE>, the value of a performance counter
associated with <I><c>id</c></I> is returned.  This result may be returned asynchronously with
respect to the OpenGL process, so it is possible that glGetQueryObjectul64vARB()
may block until the counter result is available.  If <I><c>pname</c></I>
is <CODE>GL_QUERY_RESULT_AVAILABLE_ARB</CODE>, <CODE>GL_TRUE</CODE>
is returned if the query result for <CODE>GL_QUERY_RESULT_ARB</CODE>
would be returned immediately without blocking, otherwise <CODE>GL_FALSE</CODE>
is returned.

If the purpose of a query is to implement conditional rendering, this
can also be accomplished without reading a value back by using
glBeginConditionalRenderingSCE() and glEndConditionalRenderingSCE().

@param id Specifies the query name.
@param pname Specifies the query parameter to return.  Must be one of
<CODE>GL_QUERY_RESULT_ARB</CODE> or
<CODE>GL_QUERY_RESULT_AVAILABLE_ARB</CODE>.
@param params Specifies an address for the returned parameter value.

@par Errors
Use glGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>GL_INVALID_OPERATION</CODE></TD>
<TD><I><c>id</c></I> does not represent a valid query.</TD>
</TR>
<TR>
<TD><CODE>GL_INVALID_ENUM</CODE></TD>
<TD><I><c>pname</c></I> is not one of
<CODE>GL_QUERY_RESULT_ARB</CODE> or
<CODE>GL_QUERY_RESULT_AVAILABLE_ARB</CODE>.</TD>
</TR>
</TABLE>

@sa glGetQueryObjectivARB, glBeginConditionalRenderingSCE, glEndConditionalRenderingSCE

 */
GLAPI void APIENTRY glGetQueryObjectul64vARB( GLuint id, GLenum pname, unsigned long long* params )
{
    JS_APIENTRY_TIMER( glGetQueryObjectul64vARB );
#ifdef JS_DPM
    if ( _jsdpmState )
    {
        _JS_DPM_API_ENTRY_3(
            glGetQueryObjectivARB, JSDPM_CALL_TYPE_STATE_GENERAL, , 3,
            id, sizeof( GLenum ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
            pname, sizeof( GLenum ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM,
            params, ( 1*sizeof( GLuint ) ), INPUT_OUTPUT, DPM_API_ARGUMENT_POINTER );
    }
#endif

    PSGLcontext*	LContext = _CurrentContext;
    JS_ASSERT( LContext );

#ifndef JS_NO_ERROR_CHECK
    if ( !_jsTexNameSpaceIsName( &LContext->queryNameSpace, id ) )
    {
        // not a valid query
        _jsSetError( GL_INVALID_OPERATION );
        return;
    }
#endif

    jsQuery* query = _jsGetQuery( LContext, id );
    switch ( pname )
    {
        case GL_QUERY_RESULT_ARB:
        case GL_QUERY_RESULT_AVAILABLE_ARB:
#if defined(JS_DPM) && !defined(JS_DPM_V2)
            if ( LContext->pmQuery == 0 )
                *params = ( unsigned long long )_jsDPMReturnValue( query );
            else
                // query is still in progress
                _jsSetError( GL_INVALID_OPERATION );
#else
            if ( LContext->samplesPassedQuery != id )
            {
                GLuint tempParam;
                _jsPlatformGetQuery( query, pname, &tempParam );
                *params = ( unsigned long long )tempParam;
            }
            else
                // query is still in progress
                _jsSetError( GL_INVALID_OPERATION );
#endif
            break;
        default:
            _jsSetError( GL_INVALID_ENUM );
            return;
    }

#ifdef JS_DPM
    _JS_DPM_API_RETURN( 1, params, ( 1*sizeof( GLuint ) ) );
#endif
}

/**
@short Begin a conditional rendering block

glBeginConditionalRenderingSCE() begins a conditional rendering block in
which drawing is enabled or disabled based on the result of an earlier
occlusion query.  If the query result is nonzero then drawing is enabled,
otherwise drawing is disabled.

A conditional rendering block only affects whether primitives are drawn
to the frame buffer.  Changes to the OpenGL state within a conditional
rendering block still take effect regardless of the query state.

Conditional rendering blocks may not be nested.

@param id Specifies the name of the query

@par Errors
Use glGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>GL_INVALID_OPERATION</CODE></TD>
<TD><I><c>id</c></I> does not represent a valid query.</TD>
</TR>
<TR>
<TD><CODE>GL_INVALID_OPERATION</CODE></TD>
<TD>A conditional render is already in progress.</TD>
</TR>
</TABLE>

@sa glBeginQueryARB, glEndQueryARB

 */
GLAPI void APIENTRY glBeginConditionalRenderingSCE( GLuint id )
{
    JS_APIENTRY_TIMER( glBeginConditionalRenderingSCE );
    PSGLcontext*	LContext = _CurrentContext;
    JS_ASSERT( LContext );

#ifndef JS_NO_ERROR_CHECK
    if ( LContext->conditionalRenderQuery )
    {
        // conditional render already in progress
        _jsSetError( GL_INVALID_OPERATION );
        return;
    }

    if ( !_jsTexNameSpaceIsName( &LContext->queryNameSpace, id ) )
    {
        // not a valid query
        _jsSetError( GL_INVALID_OPERATION );
        return;
    }
#endif

    jsQuery* query = _jsGetQuery( LContext, id );
    JS_ASSERT( query != NULL );
    _jsPlatformBeginConditionalRendering( query );

    LContext->conditionalRenderQuery = id;
}

/**
@short End a conditional rendering block

glEndConditionalRenderingSCE() ends the current conditional
rendering block begun with glBeginConditionalRenderingSCE().

@par Errors
Use glGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>GL_INVALID_OPERATION</CODE></TD>
<TD>There is no conditional
render in progress.</TD>
</TR>
</TABLE>

@sa glBeginConditionalRenderingSCE

 */
GLAPI void APIENTRY glEndConditionalRenderingSCE( void )
{
    JS_APIENTRY_TIMER( glEndConditionalRenderingSCE );
    PSGLcontext*	LContext = _CurrentContext;
    JS_ASSERT( LContext );

#ifndef JS_NO_ERROR_CHECK
    if ( LContext->conditionalRenderQuery == 0 )
    {
        // no conditional render in progress
        _jsSetError( GL_INVALID_OPERATION );
        return;
    }

    if ( !_jsTexNameSpaceIsName( &LContext->queryNameSpace, LContext->conditionalRenderQuery ) )
    {
        _jsSetError( GL_INVALID_OPERATION );
        return;
    }
#endif

    jsQuery* query = _jsGetQuery( LContext, LContext->conditionalRenderQuery );
    JS_ASSERT( query != NULL );
    _jsPlatformEndConditionalRendering( query );

    LContext->conditionalRenderQuery = 0;
}

/**
 * @} OpenGL
 */
// ------------------------------------------------------
