/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 *                Copyright (C) 2009 Sony Computer Entertainment Inc.
 *                                               All Rights Reserved.
 */


/*
 * Platform-dependent synchronization - JSGCM implementation
 */

#include <PSGL/psgl.h>
#include <PSGL/private.h>
#include <PSGL/Debug.h>
#include <PSGL/Utils.h>
#include <PSGL/Thread.h>
#include <PSGL/Types.h>
#include <JSGCM/JSGcmUtils.h>
#include <JSGCM/JSGcmFifoUtils.h>
#include <JSGCM/JSGcmSemaphore.h>
#include <string.h>


// semaphore free list
//  Events are implemented with semaphores, which must be allocated as they
//  are created.  The allocation scheme is a simple free list - allocation
//  removes an index from the end of the list and deallocation appends an
//  index to the list.
static GLuint _jsNumEventFree;
static GLint _jsEventFree[_JSGCM_SEMA_NEVENTS];

// semaphore states
#define _JSGCM_SEMA_STATE_WAITING 0xdeadbeef
#define _JSGCM_SEMA_STATE_TRIGGERED 0x00000000

// use one semaphore to implement fence
//  JSGCM already supports fence, but semaphores use the Curie class
//  so this may be more efficient.  _jsGcm{Inc,Test,Finish}FenceRef are
//  intended to be drop-in replacements for _jsGcmFifoGl{Inc,Test,Finish}FenceRef.
//  Note that Test returns FALSE when the fence has passed, which is the
//  opposite of glFinishFenceNV.
static GLuint nvFenceCounter = 0;
static GLuint nvFenceCache = 0;

#ifdef JS_DPM
// for DPM save/restore state
int _jsSaveSyncGlobals( void * saveArea, unsigned long int * offset, unsigned long int max )
{
#define _JS_DPM_SAVE_GLOBAL(x){if(*offset+sizeof(x)>max)return -1;memcpy((char *)saveArea+*offset,&x,sizeof(x));*offset+=sizeof(x);}
    _JS_DPM_SAVE_GLOBAL( _jsNumEventFree );
    _JS_DPM_SAVE_GLOBAL( _jsEventFree );
    _JS_DPM_SAVE_GLOBAL( nvFenceCounter );
    _JS_DPM_SAVE_GLOBAL( nvFenceCache );
    return 0;
#undef _JS_DPM_SAVE_GLOBAL
}
int _jsRestoreSyncGlobals( void * saveArea, unsigned long int * offset )
{
#define _JS_DPM_RESTORE_GLOBAL(x){memcpy(&x,(const char *)saveArea+*offset,sizeof(x));*offset+=sizeof(x);}
    _JS_DPM_RESTORE_GLOBAL( _jsNumEventFree );
    _JS_DPM_RESTORE_GLOBAL( _jsEventFree );
    _JS_DPM_RESTORE_GLOBAL( nvFenceCounter );
    _JS_DPM_RESTORE_GLOBAL( nvFenceCache );
    return 0;
#undef _JS_DPM_RESTORE_GLOBAL
}
#endif


void _jsGcmInitSync( void )
{
    JS_ASSERT( _JSGCM_SEMA_NEVENTS < JSGCM_MAX_USER_SEMAPHORES );
#ifndef NDEBUG
    // XXX Check for semaphore byte shuffling bug
    _jsGcmFifoGlReleaseSemaphore( JSGCM_SEMAPHORE_USING_CPU, _JSGCM_SEMA_BASE, 0xdeadbeef );
    _jsGcmFifoGlFinish();
    JS_ASSERT( *_jsGcmFifoGlMapSemaphore( _JSGCM_SEMA_BASE ) == 0xdeadbeef );
#endif

    // initialize event semaphores free list
    _jsNumEventFree = _JSGCM_SEMA_NEVENTS;
    for ( int i = 0; i < _JSGCM_SEMA_NEVENTS; ++i )
        _jsEventFree[i] = _JSGCM_SEMA_NEVENTS - 1 - i;

    // initialize fence semaphore
    _jsGcmFifoGlReleaseSemaphore( JSGCM_SEMAPHORE_USING_CPU, _JSGCM_SEMA_FENCE, nvFenceCounter );
}

void _jsGcmIncFenceRef( GLuint* ref )
{
    ++nvFenceCounter;
    _jsGcmFifoGlReleaseSemaphore( JSGCM_SEMAPHORE_USING_GPU, _JSGCM_SEMA_FENCE, nvFenceCounter );
    *ref = nvFenceCounter;
}

// return FALSE if fence has passed (consistent with _jsGcmFifoGlTestFenceRef)
GLboolean _jsGcmTestFenceRef( const GLuint ref )
{
    // try cached value first
    const GLuint value = ref;
    if ( !(( nvFenceCache - value ) & 0x80000000 ) )
        return GL_FALSE;

    // update cache and try again
    nvFenceCache = *_jsGcmFifoGlMapSemaphore( _JSGCM_SEMA_FENCE );
    if ( !(( nvFenceCache - value ) & 0x80000000 ) )
        return GL_FALSE;

    return GL_TRUE;
}

void _jsGcmFinishFenceRef( const GLuint ref )
{
    if ( _jsGcmTestFenceRef( ref ) )
    {
        _jsGcmFifoGlFlush();
        while ( _jsGcmTestFenceRef( ref ) )
            _jsYieldThread();
    }
}

GLuint _jsGcmGetFenceRef( void )
{
    return *_jsGcmFifoGlMapSemaphore( _JSGCM_SEMA_FENCE );
}

int _jsPlatformFenceObjectSize()
{
    return sizeof( jsGcmFenceObject );
}

GLboolean _jsPlatformCreateFenceObject( jsFenceObject* fenceObject )
{
    return GL_TRUE;
}

void _jsPlatformDestroyFenceObject( jsFenceObject* fenceObject )
{}

void _jsPlatformSetFence( jsFenceObject* fenceObject )
{
    jsGcmFenceObject* gcmFenceObject = ( jsGcmFenceObject* )fenceObject->platformFenceObject;
    _jsGcmIncFenceRef( &gcmFenceObject->fence );
}

GLboolean _jsPlatformTestFence( jsFenceObject* fenceObject )
{
    jsGcmFenceObject* gcmFenceObject = ( jsGcmFenceObject* )fenceObject->platformFenceObject;

    // _jsGcmTestFenceRef returns FALSE if the fence has passed, which is the
    // opposite sense of glTestFenceNV and this function.
    if ( _jsGcmTestFenceRef( gcmFenceObject->fence ) )
    {
        _jsGcmFifoGlFlush();
        return GL_FALSE;
    }
    else
    {
        return GL_TRUE;
    }
}

void _jsPlatformFinishFence( jsFenceObject* fenceObject )
{
    jsGcmFenceObject* gcmFenceObject = ( jsGcmFenceObject* )fenceObject->platformFenceObject;
    _jsGcmFinishFenceRef( gcmFenceObject->fence );
}

int _jsPlatformEventObjectSize()
{
    return sizeof( jsGcmEventObject );
}

GLboolean _jsPlatformCreateEventObject( jsEventObject* eventObject )
{
    JS_ASSERT( eventObject != NULL );
    jsGcmEventObject* gcmEventObject = ( jsGcmEventObject* )eventObject->platformEventObject;

    // allocate a semaphore
    if ( _jsNumEventFree == 0 )
    {
        // no more semaphores available
        gcmEventObject->sema = -1;
        _jsSetError( GL_INVALID_OPERATION );
        return GL_FALSE;
    }

    gcmEventObject->sema = _jsEventFree[--_jsNumEventFree];

    // initialize to waiting state
    _jsGcmFifoGlReleaseSemaphore(
        JSGCM_SEMAPHORE_USING_CPU,
        gcmEventObject->sema + _JSGCM_SEMA_BASE,
        _JSGCM_SEMA_STATE_WAITING );

    return GL_TRUE;
}

void _jsPlatformDestroyEventObject( jsEventObject* eventObject )
{
    JS_ASSERT( eventObject != NULL );
    jsGcmEventObject* gcmEventObject = ( jsGcmEventObject* )eventObject->platformEventObject;

    // return the semaphore to the free list
    if ( gcmEventObject->sema >= 0 )
        _jsEventFree[_jsNumEventFree++] = gcmEventObject->sema;
}

void _jsPlatformSetEvent( jsEventObject* eventObject )
{
    JS_ASSERT( eventObject != NULL );
    jsGcmEventObject* gcmEventObject = ( jsGcmEventObject* )eventObject->platformEventObject;

    if ( gcmEventObject->sema >= 0 )
        _jsGcmFifoGlReleaseSemaphore(
            JSGCM_SEMAPHORE_USING_CPU,
            gcmEventObject->sema + _JSGCM_SEMA_BASE,
            _JSGCM_SEMA_STATE_TRIGGERED );
}

void _jsPlatformResetEvent( jsEventObject* eventObject )
{
    JS_ASSERT( eventObject != NULL );
    jsGcmEventObject* gcmEventObject = ( jsGcmEventObject* )eventObject->platformEventObject;

    if ( gcmEventObject->sema >= 0 )
        _jsGcmFifoGlReleaseSemaphore(
            JSGCM_SEMAPHORE_USING_CPU,
            gcmEventObject->sema + _JSGCM_SEMA_BASE,
            _JSGCM_SEMA_STATE_WAITING );
}

void _jsPlatformWaitEvent( jsEventObject* eventObject, GLboolean autoReset )
{
    JS_ASSERT( eventObject != NULL );
    jsGcmEventObject* gcmEventObject = ( jsGcmEventObject* )eventObject->platformEventObject;

    if ( gcmEventObject->sema >= 0 )
    {
        _jsGcmFifoGlAcquireSemaphore(
            JSGCM_SEMAPHORE_USING_GPU,
            gcmEventObject->sema + _JSGCM_SEMA_BASE,
            _JSGCM_SEMA_STATE_TRIGGERED );

        if ( autoReset )
        {
            _jsGcmFifoGlReleaseSemaphore(
                JSGCM_SEMAPHORE_USING_GPU,
                gcmEventObject->sema + _JSGCM_SEMA_BASE,
                _JSGCM_SEMA_STATE_WAITING );
        }
    }
}

unsigned long long _jsPlatformMapEvent( jsEventObject* eventObject )
{
    JS_ASSERT( eventObject != NULL );
    jsGcmEventObject* gcmEventObject = ( jsGcmEventObject* )eventObject->platformEventObject;

    // return the memory-mapped pointer to the semaphore
    if ( gcmEventObject->sema >= 0 )
        return ( unsigned long long )( JSintptr )_jsGcmFifoGlMapSemaphore( gcmEventObject->sema + _JSGCM_SEMA_BASE );
    else
        return 0;
}

void _jsPlatformSetMappedEvent( unsigned long long mappedEvent )
{
    *( int* )( JSintptr )mappedEvent = _JSGCM_SEMA_STATE_TRIGGERED;
}

