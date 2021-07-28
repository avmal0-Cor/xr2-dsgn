/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 *                Copyright (C) 2009 Sony Computer Entertainment Inc.
 *                                               All Rights Reserved.
 */


/*
 * Function definitions related to thread management on the cell
 */

#include <PSGL/psgl.h>
#include <PSGL/Debug.h>
#include <PSGL/Types.h>
#include <PSGL/Thread.h>
#include <PSGL/private.h>

#include <sys/ppu_thread.h>
#include <sys/synchronization.h>

#define JS_STACK_SIZE (16<<10) // 16KB

static sys_ppu_thread_t map[16] = {0};
static int mapNext = 0;

// Find the current thread in the thread map.  If not already in the 
// map, add to the map.  Return the location in the map of the thread.
int lookupCurrentThread()
{
    JS_ASSERT( mapNext <= 16 );
    sys_ppu_thread_t id;
    sys_ppu_thread_get_id( &id );
    for ( int i = 0;i < mapNext;++i )
    {
        if ( map[i] == id ) return i;
    }
    map[mapNext] = id;
    return mapNext++;
}

// jsThread, JSMutex, and jsCond structures
// Wrappers for the OS structures.
struct jsThread
{
    sys_ppu_thread_t thread;
    jsThreadFunc *func;
    void *param;
};

struct jsMutex
{
    sys_mutex_t mutex;
};

struct jsCond
{
    sys_cond_t cond;
};

// The function called when a thread is created, which simply wrappers the 
// function call passed in to _jsCreateThread
static void ThreadFunc( uint64_t param )
{
    jsThread *thread = ( jsThread * )( unsigned long )( param );
    void *ret = thread->func( thread->param );
    sys_ppu_thread_exit(( uint64_t )( unsigned long )ret );
}

// Create and start a new thread.  Thread will call func(param).
// detach boolean indicates whether or not to make the thread joinable.
jsThread *_jsCreateThread( jsThreadFunc *func, void *param, GLboolean detach )
{
    jsThread *thread = ( jsThread * )jsMalloc( sizeof( jsThread ) );
    if ( !thread ) return NULL;

    thread->func = func;
    thread->param = param;
    sys_ppu_thread_t thread_id;
    sys_ppu_thread_get_id( &thread_id );
    int prio;
    sys_ppu_thread_get_priority( thread_id, &prio );

    int r = sys_ppu_thread_create( &thread->thread, ThreadFunc, ( uint64_t )( unsigned long )thread, prio, JS_STACK_SIZE, detach ? 0 : SYS_PPU_THREAD_CREATE_JOINABLE, "JS_THREAD" );
    if ( r != 0 )
    {
        jsFree( thread );
        return NULL;
    }
    return thread;
}

// Destroy the given fread.
void _jsDestroyThread( jsThread *thread )
{
    JS_ASSERT( thread );
    jsFree( thread );
}

// Join the given thread, if possible.
void *_jsJoinThread( jsThread *thread )
{
    JS_ASSERT( thread );
    uint64_t result;
    sys_ppu_thread_join( thread->thread, &result );
    return ( void * )( unsigned long )result;
}

void _jsYieldThread()
{
    sys_ppu_thread_yield();
}

// Create and return a mutex
jsMutex *_jsCreateMutex()
{
    jsMutex *mutex = ( jsMutex * )jsMalloc( sizeof( jsMutex ) );
    if ( !mutex ) return NULL;
    sys_mutex_attribute_t attr;
    sys_mutex_attribute_initialize( attr );
    int r = sys_mutex_create( &mutex->mutex, &attr );
    if ( r != 0 )
    {
        jsFree( mutex );
        return NULL;
    }
    return mutex;
}

void _jsDestroyMutex( jsMutex *mutex )
{
    JS_ASSERT( mutex );
    sys_mutex_destroy( mutex->mutex );
    jsFree( mutex );
}

void _jsLockMutex( jsMutex *mutex )
{
    JS_ASSERT( mutex );
    sys_mutex_lock( mutex->mutex, 0 );
}

void _jsUnlockMutex( jsMutex *mutex )
{
    JS_ASSERT( mutex );
    sys_mutex_unlock( mutex->mutex );
}

// Try to lock a the given mutex, but do not wait if unsuccessful, return immediately.
GLboolean _jsTryLockMutex( jsMutex *mutex )
{
    JS_ASSERT( mutex );
    int r = sys_mutex_trylock( mutex->mutex );
    if ( r != 0 ) return GL_FALSE;
    else return GL_TRUE;
}

// Create and return a condition variable
jsCond *_jsCreateCond( jsMutex *mutex )
{
    JS_ASSERT( mutex );
    jsCond *cond = ( jsCond * )jsMalloc( sizeof( jsCond ) );
    if ( !cond ) return NULL;
    sys_cond_attribute_t attr;
    sys_cond_attribute_initialize( attr );
    int r = sys_cond_create( &cond->cond, mutex->mutex, &attr );
    if ( r != 0 )
    {
        jsFree( cond );
        return NULL;
    }
    return cond;
}

void _jsDestroyCond( jsCond *cond )
{
    JS_ASSERT( cond );
    sys_cond_destroy( cond->cond );
    jsFree( cond );
}

void _jsSignalCond( jsCond *cond )
{
    JS_ASSERT( cond );
    sys_cond_signal( cond->cond );
}

void _jsBroadcastCond( jsCond *cond )
{
    JS_ASSERT( cond );
    sys_cond_signal_all( cond->cond );
}

void _jsWaitCond( jsCond *cond )
{
    JS_ASSERT( cond );
    sys_cond_wait( cond->cond, 0 );
}
