/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 *                Copyright (C) 2009 Sony Computer Entertainment Inc.
 *                                               All Rights Reserved.
 */


/*
 * Platform specific implementation for the glQuery* extension.
 */


#include <PSGL/psgl.h>
#include <PSGL/private.h>
#include <PSGL/Debug.h>
#include <PSGL/Thread.h>
#include <JSGCM/JSGcmUtils.h>
#include <JSGCM/JSGcmDriver.h>
#include <JSGCM/JSGcmFifoUtils.h>
#include <PSGL/DPM2.h>

// There are a fixed number of report slots in the hardware for queries.
// The "obvious" implementation is to map GL queries to hardware slots.
// Unfortunately, this doesn't work with with conditional rendering because
// we can potentially interfere with queries that are still in the pipe.
//
// Instead hardware slots are dynamically assigned to queries as they are
// initiated (in _jsPlatformBeginQuery).  This allows us to rotate the slots
// as long as we have more unused slots than queries.

typedef struct
{
    GLint id;		// JSGCM query index (not the PSGL query name)
    GLuint result;
}
jsGcmQuery;

GLuint _jsPlatformQuerySize( void )
{
    return sizeof( jsGcmQuery );
}

void _jsPlatformGetQueryConfig( GLenum pname, GLuint* params )
{
    switch ( pname )
    {
        case GL_QUERY_COUNTER_BITS_ARB:
            *params = 32;
            break;
        default:
            JS_ASSERT( 0 );
    }
}

void _jsPlatformCreateQuery( jsQuery* query )
{
    JS_ASSERT( query != NULL );

    jsGcmDriver *gcmDriver = _jsGetGcmDriver();
    JS_ASSERT( gcmDriver != NULL );

#ifndef JS_NO_ERROR_CHECK
    // limit queries to hardware slots
    if ( gcmDriver->queryCount >= JSGCM_MAX_USER_QUERIES )
    {
        _jsSetError( GL_OUT_OF_MEMORY );	// ??
        return;
    }
#endif

    jsGcmQuery* gcmQuery = ( jsGcmQuery* )query->platformQuery;
    gcmQuery->id = -1;
    ++gcmDriver->queryCount;
}

void _jsPlatformDestroyQuery( jsQuery* query )
{
    JS_ASSERT( query != NULL );
    jsGcmQuery* gcmQuery = ( jsGcmQuery* )query->platformQuery;

    jsGcmDriver *gcmDriver = _jsGetGcmDriver();
    JS_ASSERT( gcmDriver != NULL );

    if ( gcmQuery->id >= 0 )
        gcmDriver->query[gcmQuery->id] = GL_FALSE;

    --gcmDriver->queryCount;
}

void _jsPlatformBeginQuery( jsQuery* query )
{

    jsGcmDriver *gcmDriver = _jsGetGcmDriver();
    JS_ASSERT( gcmDriver != NULL );

    jsGcmQuery* gcmQuery = ( jsGcmQuery* )query->platformQuery;

    static int startIndex = 0;

    // look for an unused slot
    //  Start right after the previously found slot.
    int index = gcmQuery->id;
    for ( int i = 0; i < JSGCM_MAX_USER_QUERIES; ++i )
    {
        const int testIndex = ( i + startIndex ) % JSGCM_MAX_USER_QUERIES;
        if ( !gcmDriver->query[testIndex] )
        {
            index = testIndex;
            break;
        }
    }

    // test if conditional render is pending
    if ( gcmDriver->queryFence[index] != NULL )
    {
        // wait for conditional renders
        index = startIndex;
        if ( JS_UNLIKELY( _jsGcmFifoGlTestFenceRef( gcmDriver->queryFence[index] ) ) )
        {
            // If we haven't tested the reference for a long time, it is
            // possible they have wrapped.  To avoid that case, we don't
            // wait any longer than it takes to flush the pipe.
            GLuint ref;
            _jsGcmFifoGlIncFenceRef( &ref );
            while ( _jsGcmFifoGlTestFenceRef( gcmDriver->queryFence[index] ) )
            {
                if ( !_jsGcmFifoGlTestFenceRef( ref ) )
                {
                    // entire pipe flushed, reset all fences
                    for ( int i = 0; i < JSGCM_MAX_USER_QUERIES; ++i )
                        gcmDriver->queryFence[i] = NULL;
                    break;
                }

                _jsYieldThread();
            }
        }
    }

    // release old slot
    if ( gcmQuery->id >= 0 )
        gcmDriver->query[gcmQuery->id] = GL_FALSE;

    // assign new slot
    gcmQuery->id = index;
    gcmDriver->query[gcmQuery->id] = GL_TRUE;
    gcmDriver->queryFence[gcmQuery->id] = NULL;
    if ( query->queryType == GL_SAMPLES_PASSED_ARB )
        _jsGcmFifoGlBeginQuery( JSGCM_SAMPLES_PASSED, gcmQuery->id, GL_TRUE );
#if defined(JS_DPM) && !defined(JS_DPM_V2) // v1 only
    else
        if ( _jsDPMBeginQuery( query, index ) )
        {
            _jsSetError( GL_INVALID_OPERATION );
        }
#endif // JS_DPM v1
    startIndex = ( index + 1 ) % JSGCM_MAX_USER_QUERIES;
}

void _jsPlatformEndQuery( jsQuery* query )
{
    JS_ASSERT( query != NULL );

#ifdef JS_DEBUG
    jsGcmDriver *gcmDriver = _jsGetGcmDriver();
    JS_ASSERT( gcmDriver != NULL );

    jsGcmQuery* gcmQuery = ( jsGcmQuery* )query->platformQuery;
    JS_ASSERT( gcmQuery->id >= 0 && gcmQuery->id < JSGCM_MAX_USER_QUERIES );
#endif

    if ( query->queryType == GL_SAMPLES_PASSED_ARB )
        _jsGcmFifoGlEndQuery( JSGCM_SAMPLES_PASSED );
#if defined(JS_DPM) && !defined(JS_DPM_V2) // v1 only
    else
        if ( _jsDPMEndQuery() < 0 )
        {
            _jsSetError( GL_INVALID_OPERATION );
        }
#endif // JS_DPM_v1
}

void _jsPlatformGetQuery( jsQuery* query, GLenum pname, GLuint* params )
{
    JS_ASSERT( query != NULL );
    JS_ASSERT( params != NULL );

    jsGcmQuery* gcmQuery = ( jsGcmQuery* )query->platformQuery;
    switch ( pname )
    {
        case GL_QUERY_RESULT_ARB:
#if defined(JS_DPM) && !defined(JS_DPM_V2) // v1 only
            if ( query->queryType & 0xE0000000 )
                *params = ( GLuint )_jsDPMReturnValue( query );
            else
#endif
            {
                // wait for query result to become available
                while (!_jsGcmFifoGlTestQuery(JSGCM_SAMPLES_PASSED,gcmQuery->id,&query->result))                
                {
                    _jsGcmFifoGlFlush();
                    _jsYieldThread();
                }
                *params = query->result;
            }
            break;
	case GL_QUERY_RESULT_AVAILABLE_ARB:

            if (_jsGcmFifoGlTestQuery(JSGCM_SAMPLES_PASSED,gcmQuery->id,&query->result))            
                *params = GL_TRUE;
            else
            {
				// Result_Available queries should not wait or yield [RStenson] 
                //_jsGcmFifoGlFinish();
                //_jsYieldThread();
                *params = GL_FALSE;
            }
            break;

        default:
            JS_ASSERT( 0 );
    }
}

void _jsPlatformBeginConditionalRendering( jsQuery* query )
{
    JS_ASSERT( query != NULL );
    jsGcmQuery* gcmQuery = ( jsGcmQuery* )query->platformQuery;

    if ( gcmQuery->id >= 0 )
        _jsGcmFifoGlBeginConditionalRendering( gcmQuery->id );
}

void _jsPlatformEndConditionalRendering( jsQuery* query )
{
    JS_ASSERT( query != NULL );
    jsGcmQuery* gcmQuery = ( jsGcmQuery* )query->platformQuery;

    jsGcmDriver *gcmDriver = _jsGetGcmDriver();
    JS_ASSERT( gcmDriver != NULL );

    _jsGcmFifoGlEndConditionalRendering();

    // don't allow a NULL fence value, we use that to mean no fence
    do
    {
        _jsGcmFifoGlIncFenceRef( &gcmDriver->queryFence[gcmQuery->id] );
    }
    while ( gcmDriver->queryFence[gcmQuery->id] == NULL );
}
