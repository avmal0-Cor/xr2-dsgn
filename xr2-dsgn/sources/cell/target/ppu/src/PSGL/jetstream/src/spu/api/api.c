/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 *                Copyright (C) 2009 Sony Computer Entertainment Inc.
 *                                               All Rights Reserved.
 */

 
/*
 * 
 */

#include <stdlib.h>
#include <spu_printf.h>
#include <cell/dma.h>
#include <cell/gcm_spu.h>
//#include <cell/gcm/gcm_method_data.h>   not supported on SPU
#include <PSGL/spu_psgl.h>

//#define HOLE_DEBUG_PRINTF(...) spu_printf(__VA_ARGS__)
#define HOLE_DEBUG_PRINTF(...)

#ifdef OS_VERSION_0_3_0
#define MFC_WrTagMask mfc_wr_tag_mask
#define SPU_WrOutMbox mfc_wr_mailbox
#define SPU_RdInMbox mfc_rd_mailbox
#endif

#define _JS_WSIZE 1024	// window buffer size
#define _JS_NWINDOWS 2	// number of IOIF windows to use

#define _JS_DMA_MAX_SIZE 16384

// IOIF window addresses
//  There are 4 windows for each of 4 or 8 64MB blocks in GPU memory:
//
//	0-3		1st 64MB
//	4-7		2nd 64MB
//	8-11	3rd 64MB
//	12-15	4th 64MB
//	... (if necessary)
//
//  config[0], config[1], config[2], and config[3] are pointers to the same
//  64MB of memory on the GPU.  Similarly, config[4], config[5], config[6],
//  and config[7] all point to the next 64MB, and so on.
//
//	DMA using multiple windows can be faster than to a single window.  This
//	requires sizing the DMA so we actually get parallelism.
//
//	The final entry, config[32], is the default mapping that must be
//	subtracted from the address input by the user.
//
//	The array is padded to be a multiple of 16 bytes.
static unsigned long long config[34] __attribute__(( aligned( 16 ) ) ) = {0};
static unsigned int zero[4] __attribute__(( aligned( 16 ) ) ) = {0};

// convenience translation to IOIF window
//  Input arguments are the address within the buffer object mapped using
//  glMapBuffer, and a window id 0-3.
static unsigned long long _jsMapGPUAddress( unsigned long long addr, int wid )
{
    // remove default mapping
    addr -= config[32];

    // compute window index
    //  - determine which 64MB block the address is in
    //  - multiply the block by 4 to get the index of window 0
    //  - add the window id (0-3)
    const int index = (( addr >> 24 ) & 0x1c ) + wid;

    // lower bits are offset from the window base address
    const unsigned long long offset = addr & 0x3ffffff;

    return config[index] + offset;
}

// Patch command buffer to make each 128 byte independent
//  cmd: pointer to command buffer to be patched
//  size: size in byte
//  returns patch size in byte
static inline uint32_t _jsPatchForLocalStallSync(uint32_t *cmd, uint32_t size)
{
	uint32_t *cmdStart = cmd;
	uint32_t *cmdEnd = cmd + size / 4;
	do 
	{
		// Examine an instruction
		uint32_t get = *cmd;
		uint32_t num = (get >> 18) & 0x7FF; // Method count
		uint32_t *start = cmd;
		uint32_t *end = cmd + num;
		if(__builtin_expect(cmd >= cmdEnd, false))
			break; // Done with whole command buffer
		cmd = end + 1; // Move to next instruction

		if(__builtin_expect(((uint32_t)start & ~0x7F) == ((uint32_t)end & ~0x7F), true))
			continue; // This instruction is contained in a 128 bytes block. Done with this instruction
		
		// Need to split this instruction
		uint32_t inc = (get >> 28) & 4 ^ 4; // 0 for NonIncrementMethod, 1 for IncrementMethod 
		uint32_t *splitAt = (uint32_t*)(((uint32_t)start + 0x80) & ~0x7F); // Split at the beggining of next 128 byte block
		uint32_t newNum = splitAt - start - 1; 
        if (newNum == 0)
        {
            *start = 0x0;
        }
        else
        {
    		*start = (get & 0xE003FFFF) | (newNum << 18); // Reduce method cound of first method to newNum
        }
		num -= newNum;

		// Shift down the rest of instruction by 1 to split point
		for(uint32_t *cpy = cmdEnd-1; cpy >= splitAt; cpy-=1)
			cpy[1] = cpy[0];
		// Copy method to split point. In case of inc, increase method offset by newNum
		*splitAt = ((get & 0xE003FFFF) | (num << 18)) + (inc * newNum); 
		cmd = splitAt; // Move to shifted method
		cmdEnd += 1;
		size += 4;

	} while(true);

	return size;
}

/**
 * @addtogroup SPU
 *
 * @{
 */

/**
 *  @short (Deprecated) Initializes the PSGL SPU library
 *
 *  This function has been deprecated.
 *
 *  @param initAddr initialization data that must be obtained on
 *  the PPU with psglGetSPUInitData().
 *
 */
void psglSPUInit( unsigned long long initAddr )
{
    // fetch configuration data
    spu_mfcdma64(
        config,
        initAddr >> 32,
        initAddr & 0xffffffffULL,
        sizeof( config ),
        PSGL_READ_TAG,
        0x40 );

    // wait for completion
    spu_writech( MFC_WrTagMask, 1 << PSGL_READ_TAG );
    spu_mfcstat( 2 );
}

/**
 *  @short (Deprecated) Performs DMA transfer of local data to the address 
 * indicated by a memory-mapped pointer.
 *
 *  psglSPUWriteMappedBuffer() has been deprecated. Use 
 * <c>cellDmaLargePut(localAddress,mappedAddress,size,PSGL_WRITE_TAG,0,0)</c>.
 *
 *  @param mappedAddress A pointer to the memory-mapped address of a buffer object
 *  @param localAddress A pointer to a local memory location
 *  @param size The size of the local buffer, in bytes
 */

// Address and size arguments have the same restrictions as DMA, e.g. 1, 2,
// 4, or 8 bytes naturally aligned, or a multiple of 16 bytes with 16-byte
// alignment.

void psglSPUWriteMappedBuffer( unsigned long long mappedAddress, const void* localAddress, size_t size )
{
    if (( mappedAddress & config[32] ) == config[32] )
    {
        static int window = 0;
        while ( size )
        {
            unsigned int dmaSize = ( size < _JS_WSIZE ) ? size : _JS_WSIZE;

            unsigned long long windowAddress = _jsMapGPUAddress( mappedAddress, window );
            spu_mfcdma64(
                ( void* )localAddress,
                windowAddress >> 32,
                windowAddress & 0xffffffffULL,
                /*                mappedAddress >> 32,*/
                /*                mappedAddress & 0xffffffffULL,*/
                dmaSize,
                PSGL_WRITE_TAG,
                0x20 );

            mappedAddress += dmaSize;
            localAddress += dmaSize;
            size -= dmaSize;

            window = ( window + 1 ) % _JS_NWINDOWS;
        }
    }
    else
    {
        while ( size )
        {
            unsigned int dmaSize = ( size < _JS_DMA_MAX_SIZE ) ? size : _JS_DMA_MAX_SIZE;
            spu_mfcdma64(
                ( void* )localAddress,
                mappedAddress >> 32,
                mappedAddress & 0xffffffffULL,
                dmaSize,
                PSGL_WRITE_TAG,
                0x20 );

            mappedAddress += dmaSize;
            localAddress += dmaSize;
            size -= dmaSize;
        }
    }
}

/**
 *  @short (Deprecated) Performs DMA transfer of local data from the address 
 * indicated by a memory-mapped pointer
 *
 *  psglSPUReadMappedBuffer() has been deprecated. Use 
 * <c>cellDmaLargeGet(localAddress,mappedAddress,size,PSGL_READ_TAG,0,0)</c>
 *
 *  @param mappedAddress A pointer to the memory-mapped address of a buffer object
 *  @param localAddress A pointer to a local memory location
 *  @param size The size of the local buffer, in bytes
 */

// Address and size arguments have the same restrictions as DMA, e.g. 1, 2,
// 4, or 8 bytes naturally aligned, or a multiple of 16 bytes with 16-byte
// alignment.
void psglSPUReadMappedBuffer( unsigned long long mappedAddress, void* localAddress, size_t size )
{
    // XXX read from GPU > 16 bytes stalls
    unsigned int maxDmaSize;
    if (( mappedAddress & config[32] ) == config[32] )
        maxDmaSize = 16;
    else
        maxDmaSize = _JS_DMA_MAX_SIZE;

    while ( size )
    {
        unsigned int dmaSize = ( size < maxDmaSize ) ? size : maxDmaSize;

        spu_mfcdma64(
            localAddress,
            mappedAddress >> 32,
            mappedAddress & 0xffffffffULL,
            dmaSize,
            PSGL_READ_TAG,
            0x40 );

        mappedAddress += dmaSize;
        localAddress += dmaSize;
        size -= dmaSize;
    }
}

/**
 *  @short Fills a command-buffer hole with an SPU draw command corresponding to glDrawElements().
 *
 *  This SPU function is used with the PPU function psglDrawCommandBufferHole()
 *  for SPU drawing.
 *
 *  This fuction generates a draw command on the local hole buffer in the SPU
 *  local store. The generated command has to be SMA transferred to main memory
 *  to actually fill out the command-buffer hole.
 *  The first two arguments are the same as for
 *  <c>glDrawElements()</c>. The next three (<c>type,
 *  isIndexMainMemory, indexOffset</c>) describe the index buffer. The last two
 *  arguments (<c>holeSizeInWord, localHoleBuffer</c>) specify a
 *  command-buffer hole.
 *
 *  @param mode Specifies what kind of primitives to render. Symbolic constants
 *    <c>GL_POINTS, GL_LINE_STRIP, GL_LINE_LOOP, GL_LINES, GL_TRIANGLE_STRIP,
 *    GL_TRIANGLE_FAN, GL_TRIANGLES, GL_QUAD_STRIP</c>, and <c>GL_QUADS</c> are accepted.
 *  @param count Specifies the number of elements to be rendered.
 *  @param type Specifies the type of the values in the index buffer.
 *  Must be the same as the type specified for psglDrawCommandBufferHole().
 *  @param isIndexMainMemory Specifies the location of the index buffer.
 *    Set to <c>GL_TRUE</c> if index buffer is in main memory, <c>GL_FALSE</c>
 *    if it is in local memory.
 *  @param indexOffset Specifies an offset value for the index buffer retrieved
 *     from psglDrawCommandBufferHole().
 *  @param holeSizeInWord The size of the hole in words.
 *  @param localHoleBuffer Specifies the address of a buffer in the local store
 *      for the function to create a draw command. Its size must be bigger
 *      than <i>holeSizeInWord</i>.
 *
 *  @return 0 for success, 1 for failure
 *  @sa psglDrawCommandBufferHole, glDrawElements
 */
 int psglFillCommandBufferHole(GLenum mode, GLsizei count, GLenum type, 
							  GLboolean isIndexMainMemory, uint32_t indexOffset, 
							  uint32_t holeSizeInWord, uint32_t* localHoleBuffer)
{
	// Convert GL enums to GCM enums

	uint8_t gcmMode = 0;
    switch ( mode )
    {
        case GL_POINTS:
            gcmMode = CELL_GCM_PRIMITIVE_POINTS;
            break;
        case GL_LINES:
            gcmMode = CELL_GCM_PRIMITIVE_LINES;
            break;
        case GL_LINE_LOOP:
            gcmMode = CELL_GCM_PRIMITIVE_LINE_LOOP;
            break;
        case GL_LINE_STRIP:
            gcmMode = CELL_GCM_PRIMITIVE_LINE_STRIP;
            break;
        case GL_TRIANGLES:
            gcmMode = CELL_GCM_PRIMITIVE_TRIANGLES;
            break;
        case GL_TRIANGLE_STRIP:
            gcmMode = CELL_GCM_PRIMITIVE_TRIANGLE_STRIP;
            break;
        case GL_TRIANGLE_FAN:
            gcmMode = CELL_GCM_PRIMITIVE_TRIANGLE_FAN;
            break;
        case GL_QUADS:
            gcmMode = CELL_GCM_PRIMITIVE_QUADS;
            break;
        case GL_QUAD_STRIP:
            gcmMode = CELL_GCM_PRIMITIVE_QUAD_STRIP;
            break;
        default:
            return 1; // Error
            break;
    }

    uint8_t gcmType = 0;
    switch ( type )
    {
        case GL_UNSIGNED_INT:
            gcmType = CELL_GCM_DRAW_INDEX_ARRAY_TYPE_32; // 0
            break;
        case GL_UNSIGNED_SHORT:
            gcmType = CELL_GCM_DRAW_INDEX_ARRAY_TYPE_16; // 1
            break;
        default:
            return 1;  // Error
            break;
    }

	// Fill command buffer

	// set up local command buffer
	CellGcmContextData ctx;
	ctx.begin = localHoleBuffer;
	ctx.current = ctx.begin;
	ctx.end = ctx.begin + holeSizeInWord;
	ctx.callback = 0;

	HOLE_DEBUG_PRINTF("=== SPU begin hole with size %d \n", holeSizeInWord);

	// push draw command using gcm if count is greater than 0
	if (count > 0)
	{
		HOLE_DEBUG_PRINTF("%x --- begin draw command \n", (uint32_t)ctx.current);

		// push draw command
		uint8_t indexLocation = isIndexMainMemory ? CELL_GCM_LOCATION_MAIN : CELL_GCM_LOCATION_LOCAL;
		cellGcmSetDrawIndexArrayUnsafeInline(
			&ctx, gcmMode, count, 
			gcmType, indexLocation, indexOffset); 

		HOLE_DEBUG_PRINTF("%x --- begin post process \n", (uint32_t)ctx.current);

		// post process command buffer to make each 128 byte block independent
		uint32_t patchedSize = _jsPatchForLocalStallSync( ctx.begin, (ctx.current - ctx.begin) * sizeof(uint32_t) );
		ctx.current = ctx.begin + patchedSize / sizeof(uint32_t);
	} // else no command will be pushed. command buffer hole will be filled with NOP

	// push NOP to the rest of command buffer
	while (ctx.current < ctx.end)
	{
		HOLE_DEBUG_PRINTF("%x --- NOP \n", (uint32_t)ctx.current);
		*ctx.current++ = 0x00000000; // CELL_GCM_METHOD_NOP;
	}

	HOLE_DEBUG_PRINTF("=== SPU end of hole ===\n\n");

	return 0;
}

/**
 * @}
 */

#ifndef DOXYGEN_IGNORE


void glSetMappedEventSCE( unsigned long long event )
{
    // bottom four bits of DMA src and dst addresses must match
    const int index = ( event & 0xc ) >> 2;

    // DMA 0 to the memory mapped semaphore
    /*    unsigned long long windowAddress = _jsMapGPUAddress(event,0);*/
    spu_mfcdma64(
        &zero[index],
        /*        windowAddress >> 32,*/
        /*        windowAddress & 0xffffffffULL,*/
        event >> 32,
        event & 0xffffffffULL,
        sizeof( int ),
        PSGL_WRITE_TAG,
        0x22 );	// use fence to guarantee write completion
}
#endif


/**
 *  @short Sets the event state in a memory-mapped pointer
 *
 *  glSetMappedEventWithAddressTagSCE is a SPU-only function that performs exactly like
 *  glSetMappedEventSCE, but letting the user specifying both a user buffer containing 4
 *  zero ints, and the DMA tag. This function is meant to be used with SPURS jobs.
 *
 *  @param event A pointer in effective address space, mapped to an event in GPU memory
 *  @param zeroBuffer A pointer to a buffer in SPU local store, containing 4 zero unsigned ints,
 *                    and aligned to 16 bytes.
 *  @param tag The DMA tag to use to set the event.
 *
 *  @note
 *  glSetMappedEventWithAddressTagSCE() may only be called from an SPU.
 *
 *  @note
 *  glSetMappedEventWithAddressTagSCE() ensures that prior transfers with the
 *  same DMA tag have been completed before the event is set by using a
 *  non-blocking DMA fence.
 *
 *  @sa glMapEventSCE, glWaitEventSCE, glSetEventSCE, glSetMappedEventSCE
 */
void glSetMappedEventWithAddressTagSCE( unsigned long long event, unsigned int *zeroBuffer, unsigned int tag )
{
    // bottom four bits of DMA src and dst addresses must match
    const int index = ( event & 0xc ) >> 2;

    // DMA 0 to the memory mapped semaphore
    spu_mfcdma64(
        &zeroBuffer[index],
        event >> 32,
        event & 0xffffffffULL,
        sizeof( int ),
        tag,
        0x22 );	// use fence to guarantee write completion
}


