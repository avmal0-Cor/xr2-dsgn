/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 *                Copyright (C) 2009 Sony Computer Entertainment Inc.
 *                                               All Rights Reserved.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <PSGL/Debug.h>
#include <PSGL/Thread.h>
#include <PSGL/Utils.h>
#include <PSGL/private.h>
// JSGCM_PORT_RSTENSON XXX Cell/AsyncCopy should remain Rasterizer independant.
#ifndef _JS_USE_JSGCM
#include <NV/NVUtils.h>
#include <NV/NVDriver.h>
#else
#include <JSGCM/JSGcmUtils.h>
#include <JSGCM/JSGcmDriver.h>
#endif // _JS_USE_JSGCM
#include <sys/timer.h>
#include <sys/raw_spu.h>

#include <spu/spuInterface.h>
#include <spu/AsyncCopy.h>
#include "raw_spu_mmio.h"

#define _JS_ASYNCCOPY_BARRIER_TOKEN 0xdeadbeef

sys_raw_spu_t _jsAsyncCopySPU = -1;	// SPU ID
static unsigned int _jsAsyncCopyQ;		// LS address of command array
static unsigned int _jsAsyncIOIF;		// LS address of IOIF window array

// GPU memory range
//  GPU memory must be treated differently in a couple cases:
//   - DMA from GPU can only be done 16 bytes at a time.
//   - GPU memory is not cached so preallocating output lines will fail.
static const char* slowRangeStart = NULL;
static size_t slowRangeSize = 0;

extern const char	_binary_image_elf_start[];
extern const char	_binary_image_elf_end[];
extern int _jsMaxSpu;

void _jsAsyncCopyInit( void )
{
    if (_jsMaxSpu==0) return; // if not requesting an SPU, do nothing

    // attach an SPU and start the program
    const int id = _jsGetSpu();
    if ( id < 0 )
    {
        printf("ERROR: _jsAsyncCopyInit failed! Requested maxSPUs=%d, but a raw SPU is unavailable (most likely initializeSPUs is not enabled in the psglInit options or sys_spu_initialize(6,1) is not called before psglInit).\n",_jsMaxSpu);
        JS_ASSERT(0);
        _jsMaxSpu=0;
        return;
    }

    ELF_t* spu_memcpy_elf = readElfFromMemory( _binary_image_elf_start, _binary_image_elf_end - _binary_image_elf_start );
    JS_ASSERT( spu_memcpy_elf );

    loadElf( id, spu_memcpy_elf );
    setSpuNextPC( id, spu_memcpy_elf->entrypoint );
    runSpu( id );
    freeElf( spu_memcpy_elf );

    // get command array address
    _jsAsyncCopySPU = _jsGetRawSpuId( id );
    JS_ASSERT( _jsAsyncCopySPU >= 0 );
    do
    {
asm volatile( "eieio"::: "memory" );
    }
    while ( ~mmio_read_prob_reg( _jsAsyncCopySPU, MB_Stat ) & 0x1 );
    _jsAsyncCopyQ = mmio_read_prob_reg( _jsAsyncCopySPU, PU_MB );

    // get IOIF window array address
    do
    {
asm volatile( "eieio"::: "memory" );
    }
    while ( ~mmio_read_prob_reg( _jsAsyncCopySPU, MB_Stat ) & 0x1 );
    _jsAsyncIOIF = mmio_read_prob_reg( _jsAsyncCopySPU, PU_MB );
}

void _jsAsyncCopySetSlowRange( const void* start, size_t size, unsigned long long* ioif )
{
    slowRangeStart = ( char * )start;
    slowRangeSize = size;

    if ( JS_UNLIKELY( _jsAsyncCopySPU < 0 ) )
        return;

    // copy IOIF windows
    //  There are 4 independent windows into a 64 MB block of GPU memory:
    //   ioif[0]	block 0 window 0
    //   ioif[1]	block 0 window 1
    //   ioif[2]	block 0 window 2
    //   ioif[3]	block 0 window 3
    //   ioif[4]	block 1 window 0
    //   ioif[5]	block 1 window 1
    //   ioif[6]	block 1 window 2
    //   ioif[7]	block 1 window 3
    //   ...
    //
    //  The number of blocks needed is determined by the upper bound of
    //  available GPU memory.
    const int nBlocks = _jsPad( size, 1 << 26 ) >> 26;
    const int nWindows = 4 * nBlocks;
    unsigned long long* ls = ( unsigned long long* )( LS_BASE_ADDR( _jsAsyncCopySPU ) + _jsAsyncIOIF );
    for ( int i = 0; i < nWindows; ++i )
        ls[i] = ioif[i];
}

// partial cache line copy
static void _jsVMXCopyExtra( void* dst, const void* src, size_t size )
{
    /*printf("vmx extra: 0x%p 0x%p %d\n",dst,src,size);*/
    JS_ASSERT(( JS_PTR_TO_EA( src )&15 ) == 0 );
    JS_ASSERT(( JS_PTR_TO_EA( dst )&15 ) == 0 );

    vector float* dptr = ( vector float* )dst;
    vector float* sptr = ( vector float* )src;
    const int n = size / 16;
    for ( int i = 0; i < n; ++i )
        vec_st( vec_ld( 0, sptr++ ), 0, dptr++ );

    if ( JS_UNLIKELY( size & 15 ) )
        memcpy( dptr, sptr, size & 15 );
}

// Two versions of VMX copy are used, one that preallocates destination
// cache lines and one that does not.

void _jsVMXCopyWithoutPrealloc( char* dst, const char* src, size_t size )
{
    /*printf("vmx wo: 0x%p 0x%p %d\n",dst,src,size);*/
#define USE_PREFETCH
#define USE_TOUCH
    /*#define USE_PREALLOC*/

    JS_ASSERT(( JS_PTR_TO_EA( src )&15 ) == 0 );
    JS_ASSERT(( JS_PTR_TO_EA( dst )&15 ) == 0 );

    // copy any beginning partial cache line
    if ( JS_UNLIKELY(( JS_PTR_TO_EA( src )&127 ) != 0 ) )
    {
        size_t partial = 128 - ( JS_PTR_TO_EA( src ) & 127 );
        partial = partial < size ? partial : size;

        _jsVMXCopyExtra( dst, src, partial );

        dst += partial;
        src += partial;
        size -= partial;
    }

    // prefetch stream
    //  The extended version of the dcbt instruction enables prefetching on
    //  a stream.  Cell actually uses a further extension with the TH field
    //  four bits wide (instead of two).
    //
    //  Prefetch stops at a page boundary (4K I think).
    //
    //  The ISA doc gives the syntax dcbt RA,RB,TH but looking at the
    //  assembler binary output the proper gcc inline ordering seems to
    //  be dcbt TH,RA,RB.
    //
    //  Two prefetch streams are used, one for even pages and one for odd
    //  pages (even is stream id 0, odd is stream id 1)
#define PF_UG	0x20		// go with unlimited size (0b1 in bit 58)
#define PF_STOP	0x40000000	// stop one stream (0b10 in bits 33:34)
#define PF_PBITS 12			// log2 of page size
#define PF_PAGE (1 << PF_PBITS)
#define PF_CHAN(ADDR) ((((unsigned long)(ADDR)) >> PF_PBITS) & 0x1)

#ifdef USE_PREFETCH
    // prefetch the first page
    __asm__ volatile(
		"dcbt %1,%0,0x8;\t\n"	// 0x8 is address setup mode
    :
    : "b"( src ), "b"( PF_UG | PF_CHAN( src ) )
    );

    // prefetch the next page
    const void* next = ( void * )(( unsigned long )( src + PF_PAGE ) & ~( PF_PAGE - 1 ) );
    __asm__ volatile(
        "dcbt %1,%0,0x8;\t\n"	// 0x8 is address setup mode
    :
    : "b"( next ), "b"( PF_UG | PF_CHAN( next ) )
    );
#endif

#ifdef USE_TOUCH
    // preload to L1 for first load address
    //  This dcbt instruction differs from the extended instruction in that
    //  it loads to L1.
    __asm__ volatile(
        "dcbt 0,%0;\t\n"
    :
    : "r"( src )
    );
#endif

    // unroll loop to write 8 quad words (1 cache line)
    while ( size >= 128 )
    {
#ifdef USE_PREALLOC
        // inhibit cache loading for first store address
        //  This allocates a cache line in L2 for the destination, but does
        //  not read it from memory.  This instruction will cause an alignment
        //  exception if the destination is not cacheable.
        //
        //  I tried calling dcbtst to attempt to get this cache line from L2 to
        //  L1, but this severely reduced performance.  I suspect that this
        //  causes a memory fetch.
        __asm__ volatile(
            "dcbz 0,%0;\t\n"
    :
    : "r"( dst )
        );
#endif

#ifdef USE_PREFETCH
        // check for page boundary
        if ( JS_UNLIKELY(((( unsigned long )src )&( PF_PAGE - 1 ) ) < 128 ) )
        {
            // restart prefetch for on the next page
            const void* next = ( void * )(( unsigned long )( src + PF_PAGE ) & ~( PF_PAGE - 1 ) );
            __asm__ volatile(
                "dcbt %1,%0,0x8;\t\n"	// 0x8 is address setup mode
    :
    : "r"( next ), "b"( PF_UG | PF_CHAN( next ) )
            );
        }
#endif

#ifdef USE_TOUCH
        // preload to L1 (for next iteration)
        //  This dcbt instruction differs from the extended instruction in that
        //  it loads to L1.
        __asm__ volatile(
            "dcbt %1,%0;\t\n"
    :
    : "r"( src ), "b"( 128 )
        );
#endif

        // read
        //  vec_ldl marks the cache line as LRU so hopefully we don't
        //  pollute more than one way of the cache.
        const vector unsigned char x0 = vec_ld( 0, ( unsigned char* )src );
        const vector unsigned char x1 = vec_ld( 16, ( unsigned char* )src );
        const vector unsigned char x2 = vec_ld( 32, ( unsigned char* )src );
        const vector unsigned char x3 = vec_ld( 48, ( unsigned char* )src );
        const vector unsigned char x4 = vec_ld( 64, ( unsigned char* )src );
        const vector unsigned char x5 = vec_ld( 80, ( unsigned char* )src );
        const vector unsigned char x6 = vec_ld( 96, ( unsigned char* )src );
        const vector unsigned char x7 = vec_ldl( 112, ( unsigned char* )src );

        // write the line
        //  vec_stl marks the cache line as LRU so hopefully we don't
        //  pollute more than one way of the cache.
        vec_st( x0,  0, ( unsigned char* )dst );
        vec_st( x1, 16, ( unsigned char* )dst );
        vec_st( x2, 32, ( unsigned char* )dst );
        vec_st( x3, 48, ( unsigned char* )dst );
        vec_st( x4, 64, ( unsigned char* )dst );
        vec_st( x5, 80, ( unsigned char* )dst );
        vec_st( x6, 96, ( unsigned char* )dst );
        vec_stl( x7, 112, ( unsigned char* )dst );

        src += 128;
        dst += 128;
        size -= 128;
    }

#ifdef USE_PREFETCH
    // stop prefetching
    __asm__ volatile(
        "dcbt %1,%0,0xa;\t\n"				// 0xa is control setup mode
    :
    : "b"( 0x0 ), "b"( PF_STOP | 0x0 )	// stop prefetching even page
    );
    __asm__ volatile(
        "dcbt %1,%0,0xa;\t\n"				// 0xa is control setup mode
    :
    : "b"( 0x0 ), "b"( PF_STOP | 0x1 )	// stop prefetching odd page
    );
#endif

    // finish any remaining partial cache line
    if ( JS_UNLIKELY( size != 0 ) )
        _jsVMXCopyExtra( dst, src, size );

#undef PF_CHAN
#undef PF_UG
#undef PF_STOP
#undef PF_PBITS
#undef PF_PAGE

#undef USE_PREFETCH
#undef USE_TOUCH
#undef USE_PREALLOC
}

static void _jsVMXCopyWithPrealloc( char* dst, const char* src, size_t size )
{
    /*printf("vmx: 0x%p 0x%p %d\n",dst,src,size);*/
#define USE_PREFETCH
#define USE_TOUCH
#define USE_PREALLOC

    JS_ASSERT(( JS_PTR_TO_EA( src )&15 ) == 0 );
    JS_ASSERT(( JS_PTR_TO_EA( dst )&15 ) == 0 );

    // copy any beginning partial cache line
    if ( JS_UNLIKELY(( JS_PTR_TO_EA( src )&127 ) != 0 ) )
    {
        size_t partial = 128 - ( JS_PTR_TO_EA( src ) & 127 );
        partial = partial < size ? partial : size;

        _jsVMXCopyExtra( dst, src, partial );

        dst += partial;
        src += partial;
        size -= partial;
    }

    // check if destination is now cache aligned
    //  The preallocation code assumes that both the source and destination
    //  are now on a cache line boundary.  If this is not true, don't use
    //  prealloc.
    if ( JS_UNLIKELY(( JS_PTR_TO_EA( dst )&127 ) != 0 ) )
    {
        if ( size )
            _jsVMXCopyWithoutPrealloc( dst, src, size );
        return;
    }

    // prefetch stream
    //  The extended version of the dcbt instruction enables prefetching on
    //  a stream.  Cell actually uses a further extension with the TH field
    //  four bits wide (instead of two).
    //
    //  Prefetch stops at a page boundary (4K I think).
    //
    //  The ISA doc gives the syntax dcbt RA,RB,TH but looking at the
    //  assembler binary output the proper gcc inline ordering seems to
    //  be dcbt TH,RA,RB.
    //
    //  Two prefetch streams are used, one for even pages and one for odd
    //  pages (even is stream id 0, odd is stream id 1)
#define PF_UG	0x20		// go with unlimited size (0b1 in bit 58)
#define PF_STOP	0x40000000	// stop one stream (0b10 in bits 33:34)
#define PF_PBITS 12			// log2 of page size
#define PF_PAGE (1 << PF_PBITS)
#define PF_CHAN(ADDR) ((((unsigned long)(ADDR)) >> PF_PBITS) & 0x1)

#ifdef USE_PREFETCH
    // prefetch the first page
    __asm__ volatile(
        "dcbt %1,%0,0x8;\t\n"	// 0x8 is address setup mode
    :
    : "b"( src ), "b"( PF_UG | PF_CHAN( src ) )
    );

    // prefetch the next page
    const void* next = ( void * )(( unsigned long )( src + PF_PAGE ) & ~( PF_PAGE - 1 ) );
    __asm__ volatile(
        "dcbt %1,%0,0x8;\t\n"	// 0x8 is address setup mode
    :
    : "b"( next ), "b"( PF_UG | PF_CHAN( next ) )
    );
#endif

#ifdef USE_TOUCH
    // preload to L1 for first load address
    //  This dcbt instruction differs from the extended instruction in that
    //  it loads to L1.
    __asm__ volatile(
        "dcbt 0,%0;\t\n"
    :
    : "r"( src )
    );
#endif

    // unroll loop to write 8 quad words (1 cache line)
    while ( size >= 128 )
    {
#ifdef USE_PREALLOC
        // inhibit cache loading for first store address
        //  This allocates a cache line in L2 for the destination, but does
        //  not read it from memory.  This instruction will cause an alignment
        //  exception if the destination is not cacheable.
        //
        //  I tried calling dcbtst to attempt to get this cache line from L2 to
        //  L1, but this severely reduced performance.  I suspect that this
        //  causes a memory fetch.
        __asm__ volatile(
            "dcbz 0,%0;\t\n"
    :
    : "r"( dst )
        );
#endif

#ifdef USE_PREFETCH
        // check for page boundary
        if ( JS_UNLIKELY(((( unsigned long )src )&( PF_PAGE - 1 ) ) < 128 ) )
        {
            // restart prefetch for on the next page
            const void* next = ( void * )(( unsigned long )( src + PF_PAGE ) & ~( PF_PAGE - 1 ) );
            __asm__ volatile(
                "dcbt %1,%0,0x8;\t\n"	// 0x8 is address setup mode
    :
    : "r"( next ), "b"( PF_UG | PF_CHAN( next ) )
            );
        }
#endif

#ifdef USE_TOUCH
        // preload to L1 (for next iteration)
        //  This dcbt instruction differs from the extended instruction in that
        //  it loads to L1.
        __asm__ volatile(
            "dcbt %1,%0;\t\n"
    :
    : "r"( src ), "b"( 128 )
        );
#endif

        // read
        //  vec_ldl marks the cache line as LRU so hopefully we don't
        //  pollute more than one way of the cache.
        const vector unsigned char x0 = vec_ld( 0, ( unsigned char* )src );
        const vector unsigned char x1 = vec_ld( 16, ( unsigned char* )src );
        const vector unsigned char x2 = vec_ld( 32, ( unsigned char* )src );
        const vector unsigned char x3 = vec_ld( 48, ( unsigned char* )src );
        const vector unsigned char x4 = vec_ld( 64, ( unsigned char* )src );
        const vector unsigned char x5 = vec_ld( 80, ( unsigned char* )src );
        const vector unsigned char x6 = vec_ld( 96, ( unsigned char* )src );
        const vector unsigned char x7 = vec_ldl( 112, ( unsigned char* )src );

        // write the line
        //  vec_stl marks the cache line as LRU so hopefully we don't
        //  pollute more than one way of the cache.
        vec_st( x0,  0, ( unsigned char* )dst );
        vec_st( x1, 16, ( unsigned char* )dst );
        vec_st( x2, 32, ( unsigned char* )dst );
        vec_st( x3, 48, ( unsigned char* )dst );
        vec_st( x4, 64, ( unsigned char* )dst );
        vec_st( x5, 80, ( unsigned char* )dst );
        vec_st( x6, 96, ( unsigned char* )dst );
        vec_stl( x7, 112, ( unsigned char* )dst );

        src += 128;
        dst += 128;
        size -= 128;
    }

#ifdef USE_PREFETCH
    // stop prefetching
    __asm__ volatile(
        "dcbt %1,%0,0xa;\t\n"				// 0xa is control setup mode
    :
    : "b"( 0x0 ), "b"( PF_STOP | 0x0 )	// stop prefetching even page
    );
    __asm__ volatile(
        "dcbt %1,%0,0xa;\t\n"				// 0xa is control setup mode
    :
    : "b"( 0x0 ), "b"( PF_STOP | 0x1 )	// stop prefetching odd page
    );
#endif

    // finish any remaining partial cache line
    if ( JS_UNLIKELY( size != 0 ) )
        _jsVMXCopyExtra( dst, src, size );

#undef PF_CHAN
#undef PF_UG
#undef PF_STOP
#undef PF_PBITS
#undef PF_PAGE

#undef USE_PREFETCH
#undef USE_TOUCH
#undef USE_PREALLOC
}

void _jsVMXCopy( char* dst, const char* src, size_t size )
{
    // copying to GPU cannot preallocate destination cache lines.
    if ( JS_LIKELY(( dst + size <= slowRangeStart ) | ( dst >= slowRangeStart + slowRangeSize ) ) )
        _jsVMXCopyWithPrealloc( dst, src, size );
    else
        _jsVMXCopyWithoutPrealloc( dst, src, size );
}

static inline GLboolean _jsIsInStack( const char *ptr )
{
    // Text if the pointer is in the stack. This is done by comparing the upper bits
    // because protection is done at the 256MB segment granularity.
    unsigned long register stackPtr asm( "sp" );
    return ( stackPtr >> 28 ) == (( unsigned long )ptr ) >> 28;
}

// copy engine
static void _jsAsyncCopyExec(
    char*			dst,
    const char*		src,
    size_t			size,
    int				notifyValue,
    int*			notifyAddress,
    unsigned int	options )
{
    // Test if source or destination is in the stack. Since 0.8.5, SPU access to the stack silently fails (DMA never terminates)
    if ( JS_UNLIKELY(( _jsAsyncCopySPU < 0 ) || _jsIsInStack( src ) || _jsIsInStack( dst ) ) )
    {
        // use the CPU if the SPU is not running
        _jsVMXCopy( dst, src, size );

        // notify if requested
        if ( options & _JS_ASYNCCOPY_NOTIFY_MEMORY )
            *notifyAddress = notifyValue;
        return;
    }

    // ordering between data write and spu commands
asm volatile( "sync"::: "memory" );

    // check for source slow range
    //  XXX DMA from GPU larger than 16 bytes hangs.
    if ( JS_UNLIKELY(( src + size > slowRangeStart ) & ( src < slowRangeStart + slowRangeSize ) ) )
        options |= _JS_ASYNCCOPY_GPU_SRC;

    // check for destination slow range
    //  Using multiple IOIF windows is faster.
    //  Can't use cache prealloc.
    if (( dst + size > slowRangeStart ) & ( dst < slowRangeStart + slowRangeSize ) )
    {
        options |= _JS_ASYNCCOPY_GPU_DST;
        //  	dst = _JS_NV_UNMAP_GPU_ADDRESS(dst);  RS XXX need to add generic wrapper for this as AsysCopy should be Rasterizer independant
    }

    static unsigned int qIndex = 0;

    // prepare command struct
    static union U
    {
        jsMemcpyEntry entry;
        vector float v[2];
    }
    u __attribute__(( aligned( 16 ) ) );
    u.entry.src				= ( unsigned long long )( unsigned long )src;
    u.entry.dst				= ( unsigned long long )( unsigned long )dst;
    u.entry.size			= size;
    u.entry.notifyValue		= notifyValue;
    u.entry.notifyAddress	= ( unsigned long long )( unsigned long )notifyAddress;

    // copy command into mapped memory with VMX
    JS_ASSERT( sizeof( jsMemcpyEntry ) == 32 );
    union U* ls = ( union U* )( LS_BASE_ADDR( _jsAsyncCopySPU ) +
                                            _jsAsyncCopyQ + qIndex * sizeof( jsMemcpyEntry ) );
    ls->v[0] = u.v[0];
    ls->v[1] = u.v[1];

    // wait for mailbox slot
    while (( mmio_read_prob_reg( _jsAsyncCopySPU, MB_Stat ) & 0xff00 ) == 0 )
{
        sys_timer_usleep( 10 );
    }

    // ordering between command writes and spu kick
asm volatile( "eieio"::: "memory" );

    // notify SPU of new entry
    mmio_write_prob_reg( _jsAsyncCopySPU, SPU_MB, qIndex | options );

    qIndex = ( qIndex + 1 ) % _JS_ASYNCCOPY_QSIZE;
}

// copy without notification
void _jsAsyncCopy( void* dst, const void* src, size_t size )
{
    JS_ASSERT( dst != NULL );
    JS_ASSERT( src != NULL );

    _jsAsyncCopyExec(( char * )dst, ( const char * )src, size, 0, NULL, 0 );
}

// set memory location when copy is complete
void _jsAsyncCopyNotify( void* dst, const void* src, size_t size, int notifyValue, int* notifyAddress )
{
    JS_ASSERT( dst != NULL );
    JS_ASSERT( src != NULL );
    JS_ASSERT( notifyAddress != NULL );

    _jsAsyncCopyExec(( char * )dst, ( const char * )src, size, notifyValue, notifyAddress, _JS_ASYNCCOPY_NOTIFY_MEMORY );
}

// notify when all previous copies have finished
void _jsAsyncCopyBarrier( int notifyValue, int* notifyAddress )
{
    if ( JS_UNLIKELY( _jsAsyncCopySPU < 0 ) )
    {
        if ( notifyAddress != NULL )
            *notifyAddress = notifyValue;
        return;
    }

    _jsAsyncCopyExec(
        NULL, NULL, 0,
        notifyValue, notifyAddress,
        _JS_ASYNCCOPY_BARRIER | (( notifyAddress != NULL ) ? _JS_ASYNCCOPY_NOTIFY_MEMORY : 0 ) );
}

// wait for all previous asynchronous copies to finish
void _jsAsyncCopyFinish( void )
{
    if ( JS_UNLIKELY( _jsAsyncCopySPU < 0 ) )
        return;

    _jsAsyncCopyExec(
        NULL, NULL, 0,
        _JS_ASYNCCOPY_BARRIER_TOKEN, NULL,
        _JS_ASYNCCOPY_BARRIER | _JS_ASYNCCOPY_NOTIFY_MAILBOX );

    // wait for mailbox return
    do
    {
        do
        {
            asm volatile( "eieio" );
        }
        while ( ~mmio_read_prob_reg( _jsAsyncCopySPU, MB_Stat ) & 0x1 );
    }
    while ( mmio_read_prob_reg( _jsAsyncCopySPU, PU_MB ) != _JS_ASYNCCOPY_BARRIER_TOKEN );
}

