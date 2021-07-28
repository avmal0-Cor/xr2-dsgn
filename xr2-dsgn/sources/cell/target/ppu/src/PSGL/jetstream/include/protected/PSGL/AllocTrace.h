/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 *                Copyright (C) 2009 Sony Computer Entertainment Inc.
 *                                               All Rights Reserved.
 */


/*
 * Alloc trace helper API.
 */

#ifndef _PSGL_ALLOCTRACE_H
#define _PSGL_ALLOCTRACE_H

#ifdef __cplusplus
extern "C"
{
#endif

    // When _psglAllocTraceCallback is != NULL, the function will be called
    // by PSGL when it allocates memory from its host and RSX memory pools.
    // Allocations done through malloc/free do not go through this path.
    ///////////////////////////////////////////////////////////////////////////
    // When the allocation is made on host memory, eType is PSGL_ALLOCATOR_HOST
    // When the allocation is made on rsx memory,  eType is PSGL_ALLOCATOR_LOCAL
    //
    // There are five allocator functions that will trigger the trace callback:
    //    AllocateLow           addr, size  --> allocates from the bottom of the heap
    //    AllocateHigh          addr, size  --> allocates from the top of the heap
    //    AllocateExtend        addr, size  --> tries to find space by growing an existing allocation
    //
    //    Free                  addr, size  --> frees an allocation
    //    FreePending           addr, size  --> marks an allocation as free upon sweep
    //    Sweep                 addr, size  --> frees any pending allocations
    //
    ///////////////////////////////////////////////////////////////////////////

    typedef enum _psglAllocatorType {
        PSGL_ALLOCATOR_HOST,
        PSGL_ALLOCATOR_LOCAL
    } _psglAllocatorType_t;
    typedef void( *_psglAllocTraceFunc )( _psglAllocatorType_t eType, const char* szAllocName,
                                          void* pPtr, int iSize );
    extern _psglAllocTraceFunc _psglAllocTraceCallback;

    ///////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////

    // Warning, this is very very very slow
    // But it is useful
    ///////////////////////////////////////////////////////////////////////////
    extern void _jsNVAllocGetStats( _psglAllocatorType_t eType,
                                        uint32_t* puiMaxFreeBlock, uint32_t* puiBytesPendingFree, uint32_t* puiBytesFree );


#ifdef __cplusplus
}
#endif


#endif
