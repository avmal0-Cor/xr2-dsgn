/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 *                Copyright (C) 2009 Sony Computer Entertainment Inc.
 *                                               All Rights Reserved.
 */

#include <PSGL/private.h>

#ifdef OS_VERSION_0_3_0
#include <sys/pu_thread.h>
#else
#include <sys/spu_initialize.h>
#include <sys/ppu_thread.h>
#endif
#include <sys/raw_spu.h>
#include <PSGL/psgl.h>
#include <PSGL/Debug.h>
#include <PSGL/PerfCounters.h>
#include <PSGL/Utils.h>
#include <PSGL/Thread.h>
#include <PSGL/readelf.h>
#include <spu/spuInterface.h>
#include <sys/fixed_addr.h>
#include "raw_spu_mmio.h"
#include <Cell/bpa_mfc.h>
#include <stdio.h>
#include <string.h>

// are these defined anywhere in the SDK ??
#define SPU_RUNCNTL_STOP    0L
#define SPU_RUNCNTL_RUNNABLE    1L

#define SPU_STATUS_STOPPED      0x0L
#define SPU_STATUS_RUNNING      0x1L
#define SPU_STATUS_STOPPED_BY_STOP  0x2L
#define SPU_STATUS_STOPPED_BY_HALT  0x4L
#define SPU_STATUS_WAITING_FOR_CHANNEL  0x8L
#define SPU_STATUS_SINGLE_STEP      0x10L

#define SPU_LS_SIZE					0x40000

#define EIEIO()                 asm volatile ("eieio":::"memory")

static const int TAG = 31;

#define _jsSpuInit 8
int _jsMaxSpu;

sys_raw_spu_t _jsRawSpu[_jsSpuInit];
static int spu_count = 0;

int _jsGetSpu()
{
    if ( spu_count < _jsMaxSpu )
    {
        sys_raw_spu_attribute_t *m_spu_attr = NULL;
        int r = sys_raw_spu_create( &_jsRawSpu[spu_count], &m_spu_attr );
        if ( r < 0 )
        {
            printf( "failed to get SPU %d\n", spu_count );
            return r;
        }
        return spu_count++;
    }
    else return -1;
}

sys_raw_spu_t _jsGetRawSpuId( int spu )
{
    if ( spu >= 0 && spu < spu_count )
        return _jsRawSpu[spu];
    else
        return -1;
}

int _jsInitSPUs( GLint maxSpus, GLboolean initializeSPUs )
{
    int r = 0;
    _jsMaxSpu = maxSpus;
    if ( initializeSPUs ) r = sys_spu_initialize( maxSpus, maxSpus );

    return r;
}

void _jsFinishSPUs()
{
    for ( int i = 0;i < spu_count;++i )
    {
        sys_raw_spu_destroy( _jsRawSpu[i] );
    }
    spu_count = 0;
}

volatile unsigned char *_jsGetSpuLS( int spuid )
{
    return ( volatile unsigned char * )JS_EA_TO_PTR( _jsRawSpu[spuid]*RAW_SPU_OFFSET + RAW_SPU_BASE_ADDR + RAW_SPU_LS_OFFSET );
}

#define DMA_ALIGN 16U
#define DMA_MASK (DMA_ALIGN-1)
#define DMA_CACHE_ALIGN 128U
#define DMA_CACHE_MASK (DMA_CACHE_ALIGN-1)

void dmaCmd( int spuid, int cmd, int tag, unsigned int lsa, jsEA ea, unsigned int size )
{
    JS_ASSERT(( lsa&DMA_MASK ) == ( ea&DMA_MASK ) );
    JS_ASSERT((( size&DMA_MASK ) == 0 ) || (( size < DMA_ALIGN ) && (( size&( size - 1 ) ) == 0 ) ) );
    JS_ASSERT(( size < DMA_CACHE_ALIGN ) || (( ea&127 ) == 0 ) );
    unsigned int status;
    int spu = _jsRawSpu[spuid];
    _JS_PERFORMANCE_START( DMACmdCounter );
    do
    {
        _JS_PERFORMANCE_WARN( DMACmdCounter, 1000000 );
        mmio_write_prob_reg( spu, DMA_LSA, lsa );
        mmio_write_prob_reg( spu, DMA_EAH, ( ea >> 32 ) );
        mmio_write_prob_reg( spu, DMA_EAL, ( ea & 0xffffffff ) );
        mmio_write_prob_reg( spu, DMA_Size_Tag, (( size << 16 ) | tag ) );
        mmio_write_prob_reg( spu, DMA_Class_CMD, cmd );
        status = mmio_read_prob_reg( spu, DMA_CMDStatus );
        if ( status ) _jsYieldThread();
    }
    while ( status );
    _JS_PERFORMANCE_STOP( DMACmdCounter );
}

void doDma( int spuid, int cmd, int tag, unsigned int lsa, jsEA ea, int size )
{
asm volatile( "sync"::: "memory" );
    JS_ASSERT(( lsa&DMA_MASK ) == ( ea&DMA_MASK ) );
    int startBit = cmd & MFC_START_ENABLE;
    cmd &= ~MFC_START_ENABLE;

    // align on a 16 byte boundary by doing a small DMA first
    if ( ea&DMA_MASK )
    {
        int alignSize = MIN( size, ( int )( DMA_ALIGN - ( ea & DMA_MASK ) ) );
        dmaCmd( spuid, alignSize < size ? cmd : cmd | startBit, tag, lsa, ea, alignSize );
        lsa += alignSize;
        ea += alignSize;
        size -= alignSize;
    }

    // align on a cache line boundary if we are going to do big DMAs, for
    // performance
    if (( ea&DMA_CACHE_MASK ) && ( size >= ( int )DMA_CACHE_ALIGN ) )
    {
        int alignSize = MIN( size, ( int )( DMA_CACHE_ALIGN - ( ea & DMA_CACHE_MASK ) ) );
        dmaCmd( spuid, alignSize < size ? cmd : cmd | startBit, tag, lsa, ea, alignSize );
        lsa += alignSize;
        ea += alignSize;
        size -= alignSize;
    }
    while ( size > 0 )
    {
        if ( size <= MFC_MAX_DMA_SIZE )
        {
            int clampedSize = size & ~DMA_MASK;
            if ( clampedSize )
            {
                dmaCmd( spuid, clampedSize < size ? cmd : cmd | startBit, tag, lsa, ea, clampedSize );
                lsa += clampedSize;
                ea += clampedSize;
                size -= clampedSize;
            }
            if ( size )
            {
                dmaCmd( spuid, cmd | startBit, tag, lsa, ea, size );
                lsa += size;
                ea += size;
                size = 0;
            }
        }
        else
        {
            dmaCmd( spuid, cmd, tag, lsa, ea, MFC_MAX_DMA_SIZE );
            lsa += MFC_MAX_DMA_SIZE;
            ea += MFC_MAX_DMA_SIZE;
            size -= MFC_MAX_DMA_SIZE;
        }
    }
}

void waitForDmaTransfer( int spuid )
{
    int spu = _jsRawSpu[spuid];
    unsigned int mask = 1 << TAG;

    mmio_write_prob_reg( spu, Prxy_QueryMask, mask );
    mmio_write_prob_reg( spu, Prxy_QueryType, 2 );
    EIEIO();
    while ( !( mmio_read_prob_reg( spu, DMA_QStatus )&mask ) );
    {
        _jsYieldThread();
        EIEIO();
    }
asm volatile( "sync"::: "memory" );
}

int loadElf( int index, ELF_t *elf )
{
    if ( !elf ) return -22; // EINVAL
    // load
    for ( GLuint i = 0;i < elf->segmentCount;++i )
    {
        Elf32_Addr p_vaddr;
        Elf32_Word p_filesz;
        Elf32_Word p_memsz;
        if ( elf->segments[i].header.p_type != PT_LOAD ) continue;

        p_vaddr  = elf->segments[i].header.p_vaddr;
        p_filesz = elf->segments[i].header.p_filesz;
        p_memsz  = elf->segments[i].header.p_memsz;

        if ( p_vaddr + p_memsz > SPU_LS_SIZE ) return -9; // ENOMEM
        if ( p_filesz > 0 )
        {
            if ( !elf->segments[i].pointer ) return -22; // EINVAL
            doDma( index, MFC_GET_CMD, TAG, p_vaddr, JS_PTR_TO_EA( elf->segments[i].data ), p_filesz );
        }
        /*
        // taken care of by crt0
        		if (p_filesz < p_memsz)
        		{
        			memset(memory+p_vaddr+p_filesz, 0, p_memsz - p_filesz);
        		}
        */
    }
    waitForDmaTransfer( index );
    setSpuNextPC( index, elf->entrypoint );
    return 0;
}

int runSpu( int index )
{
    EIEIO();
    mmio_write_prob_reg( _jsRawSpu[index], SPU_RunCntl, SPU_RUNCNTL_RUNNABLE );
    return 0;
}

int runSpuIfPaused( int index )
{
    int ret = 0;
    int spu = _jsRawSpu[index];
    static const int RUNNING_MASK = SPU_STATUS_RUNNING | SPU_STATUS_WAITING_FOR_CHANNEL | SPU_STATUS_SINGLE_STEP;
    if ( !(( mmio_read_prob_reg( spu, SPU_Status ) )&RUNNING_MASK ) )
    {
        runSpu( index );
        ret = 1;
    }
    return ret;
}

int pauseSpu( int index )
{
    waitForDmaTransfer( index );
    mmio_write_prob_reg( _jsRawSpu[index], SPU_RunCntl, SPU_RUNCNTL_STOP );
    return 0;
}

int waitForSpu( int index )
{
    int spu = _jsRawSpu[index];
    // busy wait
    static const int RUNNING_MASK = SPU_STATUS_RUNNING | SPU_STATUS_WAITING_FOR_CHANNEL | SPU_STATUS_SINGLE_STEP;
    while (( mmio_read_prob_reg( spu, SPU_Status ) )&RUNNING_MASK )
    {
        _jsYieldThread();
    }
    return 0;
}

void copyToSpuImmediate( int spuid, unsigned int LSaddress, const void *EA, unsigned int size )
{
#if 1
    // using raw memcpy
    memcpy(( unsigned char * )_jsGetSpuLS( spuid ) + LSaddress, EA, size );
#else
    // using DMA
    doDma( spuid, MFC_GET_CMD, TAG, LSaddress, JS_PTR_TO_EA( EA ), size );
    // busy wait until done
    waitForDmaTransfer( spuid );
#endif
}

void setSpuNextPC( int index, unsigned int pc )
{
    mmio_write_prob_reg( _jsRawSpu[index], SPU_NPC, pc );
}

unsigned int js_lwarx( void *ea )
{
    unsigned int value;
asm volatile( "lwarx %0, 0, %1" : "=&b"( value ) : "b"( ea ) );
    return value;
}

int  js_stwcx( unsigned int value, void *ea )
{
    int r = 0;
    asm volatile(
        "li %0, 0\n"
        "stwcx. %1, 0, %2\n"
        "bne 0f\n"
        "li %0, 1\n"
        "0:\n"
    : "=&b"( r ) : "b"( value ), "b"( ea ) : "cc" );
    return r;
}

void js_st( unsigned int value, void *ea )
{
asm volatile( "stw %0, 0(%1)" : : "b"( value ), "b"( ea ) );
}
