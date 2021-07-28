/* SCE CONFIDENTIAL
* PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2009 Sony Computer Entertainment Inc.
* All Rights Reserved.
*/

#ifndef __CELL_JDL_H__
#define __CELL_JDL_H__ 1

#include <cell/spurs/job_descriptor.h>
#include <cell/spurs/job_queue_types.h>

#ifdef __SNC__
// JDL_ASSERT triggers "warning 237: controlling expression is constant"
#pragma diag_suppress=237
#endif

// Optionally override with your own macros in user defined header.
#ifndef JDL_ASSERT
    #if defined(NDEBUG) || defined(CELL_JDL_NDEBUG)
        #define JDL_ASSERT(b)
    #else
        #ifdef __SPU__
            #include <spu_printf.h>
            #define JDL_ASSERT(b) do{if(!(b)) {spu_printf(#b " failed\n");__asm__ volatile("stopd $0,$0,$0\n");}} while(0)
        #else
            #include <assert.h>
            #define JDL_ASSERT(b) assert(b)
        #endif
    #endif
#endif

// Used for non-critical info, such as performance warnings.
#ifndef JDL_WARN
    #if defined(NDEBUG) || defined(CELL_JDL_NDEBUG) || defined(CELL_JDL_NWARN)
        #define JDL_WARN(b, str)
    #else
        #ifdef __SPU__
            #include <spu_printf.h>
            #define JDL_WARN(b, str) do{if(b) spu_printf(str);} while(0)
        #else
            #include <stdio.h>
            #define JDL_WARN(b, str) do{if(b) {printf(str);fflush(stdout);}} while(0)
        #endif
    #endif
#endif

#ifndef JDL_MIN
#define JDL_MIN(x,y) (((x)<(y))?(x):(y))
#endif

#define CELL_JDL_CHECK_BUFFER_SMALL(jobName, bufName, maxSize, eal, size) \
    JDL_ASSERT(((eal)&0xf0000000) != 0xd0000000 && "buffer " #bufName " must not be in PPU stack memory");\
    JDL_ASSERT(((eal)&((size)-1))==0 && "buffer " #bufName " is not aligned properly for a <16 byte DMA");

#define CELL_JDL_CHECK_BUFFER_ANY(jobName, bufName, maxSize, eal, size, alignment) \
    JDL_ASSERT(((eal)&0xf0000000) != 0xd0000000 && "buffer " #bufName " must not be in PPU stack memory");\
    JDL_ASSERT(((eal)&(alignment-1))==0 && "buffer " #bufName " is not " #alignment " byte aligned");\
    JDL_ASSERT(((size)&(alignment-1))==0 && "buffer " #bufName " size is not a multiple of " #alignment " bytes");\
    JDL_ASSERT((size) <= (maxSize) && "buffer " #bufName " too big, max size is " #maxSize);

#define CELL_JDL_CHECK_BUFFER(jobName, bufName, maxSize, eal, size, alignment) \
    do{ CELL_JDL_CHECK_BUFFER_ANY(jobName, bufName, maxSize, eal, size, alignment); }while(0)

#ifndef CELL_JDL_CHECK_HEADER_POINTER
    #ifdef __PPU__
    #define CELL_JDL_CHECK_HEADER_POINTER(jobName, header) \
        JDL_WARN( (((uint32_t)&(header))&0xf0000000) == 0xd0000000, "Warning: job descriptor for " #jobName " must not be used from PPU stack memory")
    #else
    #define CELL_JDL_CHECK_HEADER_POINTER(jobName, header)
    #endif
#endif


#ifdef __SPU__

#include <spu_mfcio.h>

#define _jdl_mfc_any(ls,ea,size,tag,cmd) spu_mfcdma64(ls,mfc_ea2h(ea),mfc_ea2l(ea),size,tag,MFC_CMD_WORD(0,0,cmd))

static inline void _jdl_mfc_put_large(void* ls, uint32_t ea, uint32_t size, uint32_t tag, uint32_t dmaCmd)
{
    uint32_t uls = (uint32_t)ls;
    do
    {
        uint32_t chunk = (MFC_MAX_DMA_SIZE < size)? MFC_MAX_DMA_SIZE : size;
        _jdl_mfc_any((void*)uls,ea,chunk,tag,dmaCmd);
        size -= chunk;
        uls += chunk;
        ea += chunk;
    } while(size > 0);
}

static inline void _jdl_mfc_put_large_unaligned(void* ls, uint32_t ea, uint32_t size, uint32_t tag, uint32_t dmaCmd, uint32_t alignAmount=1)
{
    //////////////////////////////////////////////////////////////////////////
    //
    //	Send out the output buffer.
    //	Note that the LS address and the EA address must both have the same
    //	offset within a qword.
    //	The transfer has to be split into various smaller sections.
    //		-	In the first qword, we need to do 1,2,4,8 byte transfers as
    //			necessary to get us aligned.
    //		-	Then transfer in multiples of qwords up to 64K max in a single
    //			transfer.
    //		-	Then in final qword, we need to do 8,4,2,1 byte transfers as
    //			necessary to send out the remaining data.
    //
    //////////////////////////////////////////////////////////////////////////

    uint32_t lsAddress = (uint32_t)ls;

    while(size > 0)
    {
        uint32_t currSize;
        // Calculate the size of the current dma to transfer
        // alignAmount is known at compile time, so this switch will be optimized out
        switch(alignAmount)
        {
        case 1:
            currSize =  (lsAddress & 1) ? 1 :
                        ((lsAddress & 2) && (size >= 2)) ? 2 :
                        ((lsAddress & 4) && (size >= 4)) ? 4 :
                        ((lsAddress & 8) && (size >= 8)) ? 8 :
                        (size >= MFC_MAX_DMA_SIZE) ? MFC_MAX_DMA_SIZE :
                        (size >= MFC_MIN_DMA_SIZE) ? (size & ~MFC_MIN_DMA_SIZE_MASK) :
                        (size >= 8) ? 8 :
                        (size >= 4) ? 4 :
                        (size >= 2) ? 2 :
                        1;
            break;
        case 2:
            currSize =  (lsAddress & 2) ? 2 :
                        ((lsAddress & 4) && (size >= 4)) ? 4 :
                        ((lsAddress & 8) && (size >= 8)) ? 8 :
                        (size >= MFC_MAX_DMA_SIZE) ? MFC_MAX_DMA_SIZE :
                        (size >= MFC_MIN_DMA_SIZE) ? (size & ~MFC_MIN_DMA_SIZE_MASK) :
                        (size >= 8) ? 8 :
                        (size >= 4) ? 4 :
                        2;
            break;
        case 4:
            currSize =  (lsAddress & 4) ? 4 :
                        ((lsAddress & 8) && (size >= 8)) ? 8 :
                        (size >= MFC_MAX_DMA_SIZE) ? MFC_MAX_DMA_SIZE :
                        (size >= MFC_MIN_DMA_SIZE) ? (size & ~MFC_MIN_DMA_SIZE_MASK) :
                        (size >= 8) ? 8 :
                        4;
            break;
        case 8:
            currSize =  (lsAddress & 8) ? 8 :
                        (size >= MFC_MAX_DMA_SIZE) ? MFC_MAX_DMA_SIZE :
                        (size >= MFC_MIN_DMA_SIZE) ? (size & ~MFC_MIN_DMA_SIZE_MASK) :
                        8;
            break;
        default:
            // >= 16 byte alignment
            currSize =  (MFC_MAX_DMA_SIZE < size)? MFC_MAX_DMA_SIZE : size;
            break;
        }

        _jdl_mfc_any((void*)lsAddress, ea, currSize, tag, dmaCmd);

        size      -= currSize;
        lsAddress += currSize;
        ea        += currSize;
    }
}

#endif

#endif
