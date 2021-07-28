/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 *                Copyright (C) 2009 Sony Computer Entertainment Inc.
 *                                               All Rights Reserved.
 */


/*
 * Function definitions for transferring command buffer to GPU
 */

#include <JSGCM/JSGcmUtils.h>

#ifdef SPU_JSGCM_FIFO

// agp buffers need to be manually flushed
#include "bpa_map.h"
#include "bpa_mfc.h"
#define RAW_SPU_OFFSET      0x0000000000100000UL
#define RAW_SPU_LS_OFFSET   0x0000000000000000UL
#define RAW_SPU_PROB_OFFSET 0x0000000000040000UL
#define RAW_SPU_BASE_ADDR       0x00000000E0000000ULL

#define TAG 3
extern void _jsBeginGPUTransferFifo();
extern void _jsEndGPUTransferFifo();
inline void dmaFifo( jsGcmFifo *fifo, jsFifoData *start, jsFifoData *end )
{
    _jsBeginGPUTransferFifo();
    unsigned long long EAstart = ( unsigned long long )( start ) & ~127;
    unsigned long long EAend = (( unsigned long long )( end ) + 127 ) & ~127;
    unsigned int dmaLSFrom = EAstart - fifo->spuid * RAW_SPU_OFFSET + RAW_SPU_BASE_ADDR;
    unsigned long long dmaEAto = ( unsigned long long )fifo->dmaPushBufferGPU + ( EAstart - ( unsigned long long )fifo->dmaPushBuffer );
    unsigned int dmaSize = EAend - EAstart;
    volatile SPU_Problem *spu = ( volatile SPU_Problem * )( fifo->spuid * RAW_SPU_OFFSET + RAW_SPU_BASE_ADDR + RAW_SPU_PROB_OFFSET );
    volatile UINT32 *cmd_status = &spu->mfc_union_W.by32.mfc_class_cmd32;
    volatile UINT32 *statusR = &spu->dma_qstatus_R;

    while ( dmaSize > 0 )
    {
        unsigned int size = dmaSize > MFC_MAX_DMA_SIZE ? MFC_MAX_DMA_SIZE : dmaSize;
        UINT32 status;
        do
        {
            spu->mfc_lsa_W = dmaLSFrom;
            spu->mfc_ea_W = dmaEAto;
            spu->mfc_union_W.all64 = MFC_CMD_DWORD( size, TAG, MFC_PUT_CMD );
            asm volatile( "eieio" );
            status = *cmd_status;
        }
        while ( status );
        dmaSize -= size;
        dmaLSFrom += size;
        dmaEAto += size;
    }
    {
        unsigned int mask = 1 << TAG;
        spu->dma_querymask_RW = mask;
        spu->dma_querytype_RW = 2;
        asm volatile( "eieio" );
        while ( !( *statusR&mask ) );
    }
    _jsEndGPUTransferFifo();
}
#endif


/* avoids interrupting the host->gfx bus when the command fifo is full.
**
**
*/

#include <sys/timer.h>

void _jsGcmFifoDelayRegisterRead( jsGcmFifo *fifo )
{
    sys_timer_usleep( 10 );
}

#ifdef SPU_JSGCM_FIFO
void _jsGcmSetFifoSPU( jsGcmFifo *fifo, unsigned int spuid )
{
    _jsGcmFifoFinish( fifo );
    jsFifoData* newFifo = ( jsFifoData* )( spuid * RAW_SPU_OFFSET + RAW_SPU_BASE_ADDR + ( 64 << 10 ) );
    if ( newFifo != fifo->dmaPushBuffer )
    {
        memcpy( newFifo, fifo->dmaPushBuffer, fifo->dmaPushBufferSizeInWords ); // slow memcpy
        fifo->end = newFifo + ( fifo->end - fifo->dmaPushBuffer );
        fifo->current = newFifo + ( fifo->current - fifo->dmaPushBuffer );
        fifo->lastGetRead = newFifo + ( fifo->lastGetRead - fifo->dmaPushBuffer );
        fifo->lastPutWritten = newFifo + ( fifo->lastPutWritten - fifo->dmaPushBuffer );
        fifo->fence = newFifo + ( fifo->fence - fifo->dmaPushBuffer );
        fifo->dmaPushBuffer = newFifo;
        fifo->spuid = spuid;
    }
}
#endif
