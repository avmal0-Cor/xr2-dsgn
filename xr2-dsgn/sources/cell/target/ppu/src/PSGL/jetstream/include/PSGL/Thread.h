/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 *                Copyright (C) 2009 Sony Computer Entertainment Inc.
 *                                               All Rights Reserved.
 */


/*
 * JSThread function API (struct definition in source file).
 */

#ifndef _JS_THREAD_H_
#define _JS_THREAD_H_

#include <PSGL/psgl.h>

#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct jsThread jsThread;
    typedef struct jsMutex jsMutex;
    typedef struct jsCond jsCond;

    typedef void *( jsThreadFunc )( void * );

	// Thread function API
    jsThread *_jsCreateThread( jsThreadFunc *func, void *param, GLboolean detach );
    void _jsDestroyThread( jsThread *thread );
    void *_jsJoinThread( jsThread *thread );
    void _jsYieldThread();

	// Mutex function API
    jsMutex *_jsCreateMutex();
    void _jsDestroyMutex( jsMutex *mutex );
    void _jsLockMutex( jsMutex *mutex );
    void _jsUnlockMutex( jsMutex *mutex );
    GLboolean _jsTryLockMutex( jsMutex *mutex );

	// Condition variable function API
    jsCond *_jsCreateCond( jsMutex *mutex );
    void _jsDestroyCond( jsCond *cond );
    void _jsSignalCond( jsCond *cond );
    void _jsBroadcastCond( jsCond *cond );
    void _jsWaitCond( jsCond *cond );

#ifdef __cplusplus
}
#endif

#endif
