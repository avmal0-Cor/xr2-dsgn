/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 *                Copyright (C) 2009 Sony Computer Entertainment Inc.
 *                                               All Rights Reserved.
 */


/*
 * Function definitions for bounce buffer use
 */

#include <stdio.h>

#include <PSGL/private.h>
#include <PSGL/Debug.h>
#include <PSGL/Utils.h>

#include <JSGCM/JSGcmUtils.h>
#include <PSGL/PerfCounters.h>

#define _JS_BOUNCE_ALIGNMENT 4096

// states for bounce buffer block
//  The first two states are simply free or allocated.  The third state is
//  used when the block may be freed after the JSGCM FIFO has passed a
//  fence.
enum {
    _JS_BOUNCE_ENTRY_FREE,
    _JS_BOUNCE_ENTRY_STATIC,
    _JS_BOUNCE_ENTRY_PENDING
};

// allocator for linked list entries
static jsFixedAllocator* allocator = NULL;

// linked list
static JSbounceEntry* head;

// clear heap of pending frees past their fence
//  The return value is the pending data size remaining.
static unsigned int _jsGcmBounceClean( void );

// initialize the memory manager
void _jsGcmBounceInit( void* address, GLuint size, GLuint entries )
{
    // create allocator for linked list entries
    allocator = _jsFixedAllocatorCreate( sizeof( JSbounceEntry ), entries );
    JS_ASSERT( allocator != NULL );
    if ( allocator == NULL )
    {
        _jsSetError( GL_OUT_OF_MEMORY );
        return;
    }

    // pad address to alignment
    void* padAddress = _jsPadPtr( address, _JS_BOUNCE_ALIGNMENT );
    GLuint padSize = size - (( GLbyte* )padAddress - ( GLbyte* )address );

    // create first block
    head = _jsFixedAllocatorAlloc( allocator );
    JS_ASSERT( head != NULL );
    if ( head == NULL )
    {
        _jsSetError( GL_OUT_OF_MEMORY );
        return;
    }
    head->next = NULL;
    head->prev = NULL;
    head->ptr = padAddress;
    head->size = padSize;
    head->status = _JS_BOUNCE_ENTRY_FREE;
    head->fence = NULL;
}

// clean up the memory manager
void _jsGcmBounceFinish( void )
{
    JS_ASSERT( allocator != NULL );
    _jsFixedAllocatorDestroy( allocator );
    allocator = NULL;
}

// allocate from managed memory
JSbounceEntry* _jsGcmBounceAlloc( GLuint size )
{
    JS_ASSERT( allocator != NULL );

    // fix size for alignment
    size = _jsPad( size, _JS_BOUNCE_ALIGNMENT );

    // traverse list
    JSbounceEntry* current = head;
    while ( current != NULL )
    {
        // find first free fit
        if ( current->status == _JS_BOUNCE_ENTRY_FREE && size <= current->size )
        {
            // split block if necessary
            if ( size < current->size )
            {
                // create new linked list entry
                JSbounceEntry* extra = _jsFixedAllocatorAlloc( allocator );
                JS_ASSERT( extra != NULL );
                if ( extra == NULL )
                {
                    _jsSetError( GL_OUT_OF_MEMORY );
                    return NULL;
                }

                extra->prev = current;
                extra->next = current->next;
                extra->ptr = current->ptr + size;
                extra->size = current->size - size;
                extra->status = _JS_BOUNCE_ENTRY_FREE;
                extra->fence = NULL;

                current->next = extra;
                if ( extra->next != NULL )
                    extra->next->prev = extra;

                current->size = size;
            }

            // assign block
            current->status = _JS_BOUNCE_ENTRY_STATIC;
            break;
        }

        current = current->next;
    }

    if ( current == NULL )
    {
        // A static flag is used to tell us if we are on the first try at
        // allocating this memory.  This is a questionable coding style, but
        // it keeps the implementation simple and the entire module is not
        // thread-safe anyway.
        static GLboolean retry = GL_FALSE;
        if ( retry )
            return NULL;
        // keep retrying until we succeed or no frees are pending
        retry = GL_TRUE;
        GLuint stillPending;
        _JS_PERFORMANCE_START( BounceAllocCounter );
        do
        {
            stillPending = _jsGcmBounceClean();

            // try again
            current = _jsGcmBounceAlloc( size );
            if ( current != NULL )
                break;

            // wait a while
            _jsGcmFifoDelayRegisterRead( &_jsGcmState.fifo );
            _jsGcmCheckForChannelError();
        }
        while ( stillPending != 0 );
        _JS_PERFORMANCE_STOP( BounceAllocCounter );
        retry = GL_FALSE;
    }

    return current;
}

// free allocated block
void _jsGcmBounceFree( JSbounceEntry* entry )
{
    JS_ASSERT( entry != NULL );
    JS_ASSERT( allocator != NULL );

    // mark as free
    entry->status = _JS_BOUNCE_ENTRY_FREE;

    // try to merge with next block
    JSbounceEntry* next = entry->next;
    if ( next != NULL && next->status == _JS_BOUNCE_ENTRY_FREE )
    {
        // fix up pointers
        entry->next = next->next;
        if ( entry->next != NULL )
            entry->next->prev = entry;

        // sum block sizes
        entry->size += next->size;

        // free linked list entry
        _jsFixedAllocatorFree( allocator, next );
    }

    // try to merge with previous block
    JSbounceEntry* prev = entry->prev;
    if ( prev != NULL && prev->status == _JS_BOUNCE_ENTRY_FREE )
    {
        // fix up pointers
        prev->next = entry->next;
        if ( entry->next != NULL )
            entry->next->prev = prev;

        // sum block sizes
        prev->size += entry->size;

        // free linked list entry
        _jsFixedAllocatorFree( allocator, entry );
    }
}

// free after the fence is cleared
void _jsGcmBounceFreePendingFence( JSbounceEntry* entry, void* fenceRef )
{
    JS_ASSERT( entry != NULL );
    entry->status = _JS_BOUNCE_ENTRY_PENDING;
    entry->fence = fenceRef;
}

// This helper function traverses the linked list and frees any pending
// blocks that are past their fence.
static unsigned int _jsGcmBounceClean( void )
{
    // clear out FIFO
    _jsGcmFifoGlFlush();

    // calculate amount still pending
    GLuint stillPending = 0;

    JSbounceEntry* current = head;
    while ( current != NULL )
    {
        if ( current->status == _JS_BOUNCE_ENTRY_PENDING )
        {
            if ( !_jsGcmFifoGlTestFenceRef( current->fence ) )
            {
                // no longer pending, free it
                if ( current == head )
                    _jsGcmBounceFree( current );
                else
                {
                    current = current->prev;
                    _jsGcmBounceFree( current->next );
                }
            }
            else
                stillPending += current->size;
        }

        current = current->next;
    }

    return stillPending;
}

