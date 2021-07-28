/* Copyright (C)2001-2006 by NVIDIA Corporation.  All rights reserved.
 * Modified by Sony Computer Entertainment Inc under license from NVIDIA Corporation.
 * Copyright (C) 2009 Sony Computer Entertainment Inc.
 * 
 * SCEI CONFIDENTIAL
 */


/*
 * JSGcmSemaphore.h
 */

#ifndef _H_JSGCM_SESEMAPHORE_H_
#define _H_JSGCM_SESEMAPHORE_H_

/* writes the supplied new semaphore value once the gpu has completed all
** currently pending work.
**
** note:
**  - we do not enforce pairing of Acquire/Release, so you can (ab)use it to
**    write synchronized signal values...
*/

void static inline _jsGcmFifoGlReleaseSemaphore( jsGcmEnum target, GLuint semaphoreId, GLuint newSemphoreValue )
{
    jsGcmSemaphoreMemory *semaphores = _jsGcmState.semaphores;

    JS_ASSERT( semaphoreId < JSGCM_MAX_USER_SEMAPHORES );
    switch ( target )
    {
        case JSGCM_SEMAPHORE_USING_GPU:
            // let the backend(rop/fb) write the release value
            // -- guarantees all reads/writes have completed
            GCM_FUNC( cellGcmSetWriteBackEndLabel, semaphoreId, newSemphoreValue );
            break;
        case JSGCM_SEMAPHORE_USING_GPU_NO_WRITE_FLUSH:
            // write the semaphore value once host/vb/ib/tex are no longer referencing
            // any data prior to the method.
            // -- does _NOT_ guarantee that read/writes on the render target surfaces
            //    have completed (iow: cpu read on the color buffer will be undefined)

            GCM_FUNC( cellGcmSetWriteTextureLabel, semaphoreId, newSemphoreValue );
            break;
        case JSGCM_SEMAPHORE_USING_CPU:
            semaphores->userSemaphores[semaphoreId].val = newSemphoreValue;
            break;
        default:
            JS_ASSERT( 0 );
            break;
    }
}

/* return the memory mapped address of a user semaphore
*/
static inline GLuint* _jsGcmFifoGlMapSemaphore( GLuint semaphoreId )
{
    JS_ASSERT( semaphoreId < JSGCM_MAX_USER_SEMAPHORES );
    return ( GLuint* )&_jsGcmState.semaphores->userSemaphores[semaphoreId].val;
}

/* lets the gpu/cpu wait until the specific semaphore is equal to the requested
** semaphore value.
**
** note:
**  - we do not enforce pairing of Acquire/Release, so you can (ab)use it to
**  - What about aquire timeouts (after a few seconds) ?
*/
void static inline _jsGcmFifoGlAcquireSemaphore( jsGcmEnum target, GLuint semaphoreId, GLuint reqSemphoreValue )
{
    jsGcmSemaphoreMemory *semaphores = _jsGcmState.semaphores;

    // pick location
    JS_ASSERT( semaphoreId < JSGCM_MAX_USER_SEMAPHORES );
    switch ( target )
    {
        case JSGCM_SEMAPHORE_USING_GPU:
            // let the frontend aquire the semaphore...
            GCM_FUNC( cellGcmSetWaitLabel, semaphoreId, reqSemphoreValue );
            break;
        case JSGCM_SEMAPHORE_USING_CPU:
            // lame polling for now...
            for ( ;semaphores->userSemaphores[semaphoreId].val != reqSemphoreValue; )
            {
                // do something else
                jsGcmFifo *fifo = &_jsGcmState.fifo;
                _jsGcmFifoDelayRegisterRead( fifo );
            }
            break;
        default:
            JS_ASSERT( 0 );
            break;
    }
}

#endif
