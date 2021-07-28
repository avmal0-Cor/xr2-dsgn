/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 *                Copyright (C) 2009 Sony Computer Entertainment Inc.
 *                                               All Rights Reserved.
 */


/*
 * 
 */


#include <PSGL/psgl.h>
#include <PSGL/private.h>
#include <stdio.h>
#include <string.h>
#include <sys/memory.h>
#include <JSGCM/JSGcmUtils.h>
#include "PlatformDeviceInit.h"
#include <JSGCM/JSGcmFifoUtils.h>

#define _JSGCM_DMA_PUSH_BUFFER_PREFETCH_PADDING 0x1000 // 4KB
#define _JSGCM_FIFO_SIZE (64<<10) // 64 kb

#include <cell/gcm.h>

#ifdef JS_DPM
#include <sys/rsx.h>
#define MAP_BAR0_REGISTERS
#define DISABLE_POWER_MANAGEMENT
_JS_EXTERN_C void _cellGcmDisablePowerManagement( void );
#ifdef JS_DPM_V2
#include <PSGL/DPM2.h>
#endif
#endif // JS_DPM

#define _JSGCM_RESERVED_VIDEO_MEMORY (0)


// allocation handles
#define JSGCM_CHANNEL_HANDLE_ID                     0xFACE0001
#define JSGCM_FRAME_BUFFER_OBJECT_HANDLE_ID         0xFACE0002
#define JSGCM_HOST_BUFFER_OBJECT_HANDLE_ID          0xFACE0003

#define JSGCM_PUSHBUF_MEMORY_HANDLE_ID              0xBEEF1000
#define JSGCM_HOST_NOTIFIER_MEMORY_HANDLE_ID        0xBEEF1001
#define JSGCM_VID_NOTIFIER_MEMORY_HANDLE_ID         0xBEEF1002
#define JSGCM_SEMAPHORE_MEMORY_HANDLE_ID            0xBEEF1003

// dma handles
#define JSGCM_CHANNEL_DMA_SCRATCH_NOTIFIER          0xBEEF2000
#define JSGCM_CHANNEL_DMA_ERROR_NOTIFIER            0xBEEF2001
#define JSGCM_CONTEXT_DMA_MEMORY_FRAME_BUFFER       0xBEEF2002
#define JSGCM_CONTEXT_DMA_FROM_MEMORY_PUSHBUF       0xBEEF2003
#define JSGCM_CONTEXT_DMA_TO_MEMORY_GET_REPORT      0xBEEF2004
#define JSGCM_CONTEXT_DMA_MEMORY_HOST_BUFFER        0xBEEF2005
#define JSGCM_CONTEXT_DMA_MEMORY_SEMAPHORE_RW       0xBEEF2006
#define JSGCM_CONTEXT_DMA_MEMORY_SEMAPHORE_RO       0xBEEF2007

// clas contexts
#define JSGCM_CURIE_PRIMITIVE                       0xBEEF4097
#define JSGCM_MEM2MEM_HOST_TO_VIDEO                 0xBBBB0000
#define JSGCM_IMAGEFROMCPU                          0xBBBB1000
#define JSGCM_SCALEDIMAGE                           0xBBBB1001
#define JSGCM_CONTEXT_2D_SURFACE                    0xBBBB2000
#define JSGCM_CONTEXT_SWIZ_SURFACE                  0xBBBB2001

using namespace cell::Gcm;


/*
 * User func for WaitForFreeSpace used by libgpu
 */
int32_t _jsOutOfSpaceCallback( struct CellGcmContextData* fifoContext, uint32_t spaceInWords )
{
	// [RSTENSON] 
	// If not recording commands biz a usuall and wait for new or jump to the next if availible 
	// If in record mode and there is a call back call it to then let the user decide what to do next. 	
	// If in record mode and there isn't a call back, warn and hard assert in debug. 
		// if opt or ultra opt try and fail gracefully and simply exit record mode back to the regular active fifo.
	if ( _jsGcmState.recordFifoMode )
	{
		// if out of space and the user call back is set call it! 
		if ( _jsGcmState.recordOutOfSpaceCallback )
		{
			_jsGcmState.recordOutOfSpaceCallback( &_jsGcmState.fifo, spaceInWords ); 
		}
		else
		{
			// Otherwise we want to report no call back to call and assert in debug 
			_JS_REPORT_EXTRA(PSGL_REPORT_RESC_FLIP_ERROR, "WARNING: Currently in Static Command Buffer recorde mode and out of space");
			_JS_REPORT_EXTRA(PSGL_REPORT_RESC_FLIP_ERROR, "WARNING: The psglStaticCommandBufferCallBack has not been defined via psglSetRecordOutOfSpaceCallback()");
			JS_ASSERT( _jsGcmState.fifo.current + spaceInWords < _jsGcmState.fifo.begin + _jsGcmState.recordFifoSizeInWords ); 
		
			// if in opt or ultra opt just exit command record to fail gracefully 
			
			// for release want to fail gracefully so just going to break out of record mode if out off buffer space
			_jsGcmState.recordFifoMode = GL_FALSE;

			// reset the prev command buffer begin end and current; 
			_jsGcmState.fifo.begin		= _jsGcmState.recordPrevBegin;
			_jsGcmState.fifo.end		= _jsGcmState.recordPrevEnd;
			_jsGcmState.fifo.current	= _jsGcmState.recordPrevCurrent;			
		}

		return CELL_OK; 							
	}


	_jsBeginGPUWaitFifo();

	// NOTE the fifo passed in is our very own _jsGcmFifo.fifo 
	// but let's just make sure 
	JS_ASSERT( (CellGcmContextData*)fifoContext == (CellGcmContextData*)&_jsGcmState.fifo ); 
	jsGcmFifo * fifo = &_jsGcmState.fifo;

	// make sure that the space requested will actually fit in to
	// a single fifo block! 
	JS_ASSERT( spaceInWords < fifo->fifoBlockSize/sizeof(uint32_t));

	// auto flush
#ifdef GCM_HUD
  // use a flush that has linkage, thus guaranteeing that linking
  // against libgcm_hud will intercept the flush
  cellGcmFlushUnsafe((CellGcmContextData*)fifo); 
#else
	cellGcmFlushUnsafeInline((CellGcmContextData*)fifo);
#endif

	uint32_t *nextbegin, *nextend, nextbeginoffset, nextendoffset;

	//printf(" _jsOutOfSpaceCallback \n" ); 

	fifo->updateLastGetRead(); 


	// If the current end isn't the same as the full fifo end we 
	// aren't at the end.  Just go ahead and set the next begin and end 
	if(fifo->end != fifo->dmaPushBufferEnd)
	{
		//printf(" Next Block \n" ); 
		//printf(" b %p e %p c %p g %p \n", 
		//	   fifo->begin, fifo->end, fifo->current, fifo->lastGetRead ); 
		nextbegin = (uint32_t *)fifo->end + 1; 
		nextend = nextbegin + fifo->fifoBlockSize/sizeof(uint32_t) - 1;

		//	printf(" Current PSGL FIFO info \n" ); 
		//	printf(" Fifo Begin %p End %p Current %p and Get %p \n", 
		//		_jsGcmState.fifo.begin, _jsGcmState.fifo.end,_jsGcmState.fifo.current,_jsGcmState.fifo.lastGetRead ); 

		//	printf(" Last 10 words of the PSGL Fifo from the ppu put/current position \n" );  
		//	_jsPrintFifoFromPut( 10 ); 

	}
	// end up at fifo buffer
	else
	{
		//printf(" Jump To First! \n" ); 
		//printf(" b %p e %p c %p g %p \n", 
		//	   fifo->begin, fifo->end, fifo->current, fifo->lastGetRead ); 
		nextbegin = (uint32_t *)fifo->dmaPushBufferBegin;
		nextend = nextbegin + (fifo->fifoBlockSize)/sizeof(uint32_t) - 1;
	}

    cellGcmAddressToOffset(nextbegin, &nextbeginoffset);
	cellGcmAddressToOffset(nextend, &nextendoffset);

    //use this version so as not to trigger another callback
    cellGcmSetJumpCommandUnsafeInline((CellGcmContextData*)fifo, nextbeginoffset);

	
	//set up new context
	fifo->begin = nextbegin;
	fifo->current = nextbegin;
	fifo->end = nextend;

	const GLuint nopsAtBegin = 8;


	//if Gpu busy with the new area, stall and flush
	uint32_t get = fifo->dmaControl->Get;

	// if no user static command buffer stall call back 
	// biz as usual
	if ( !_jsGcmState.recordStallCallback )
	{
		// We are going to stall on 2 things here 
		// 1.  If the get is still within the new fifo block area 
		// 2.  If the get out side of our fifo buffer completely 
		while(((get >= nextbeginoffset) && (get <= nextendoffset)) 
			  || (get < fifo->dmaPushBufferOffset) || 			  
			  (get > fifo->dmaPushBufferOffset + 
			   fifo->dmaPushBufferSizeInWords*sizeof(uint32_t))) 
		{
			// Don't be a ppu hog ;)
			sys_timer_usleep(30);
			get = fifo->dmaControl->Get;
		}
	}
	else
	{
		void * getEA = NULL; 

		cellGcmIoOffsetToAddress( get, &getEA ); 

		// We are going to stall on 3 things 
		// 1. If the get is still with in the new fifo block area
		// 2. If the get is in gcm's initiazation fifo block 
		// 3. If the user stall call back returns PSGL_STALL/true that 
		// 3A. the get is in one of their called SCB fifos AND 3B. the last call 
		// position in PSGL's fifo is in the next block we want to jump to
		// we have to stall... few!  [RSTENSON] 
		while(((get >= nextbeginoffset) && (get <= nextendoffset)) 
			  || ((0 <= get) && (get < 0x10000)) ||
			  _jsGcmState.recordStallCallback( getEA, nextbegin, nextend)) 
		{
			// Don't be a ppu hog ;)
			sys_timer_usleep(30);
			get = fifo->dmaControl->Get;
			cellGcmIoOffsetToAddress( get, &getEA ); 
		}

	}

	// need to add some nops here at the beginning for a issue with the get and the put being at the 
	// same position when the fifo is in GPU memory. 
	for ( GLuint i = 0; i < nopsAtBegin; i++ )
	{
		fifo->current[0] = JSGCM_NOP();
		fifo->current++;
	}

	_jsEndGPUWaitFifo();

	return CELL_OK;
};

/* free all dynamically allocated resources
**
** note
**  - shouldn't a root free free all dependents as well ? I seem
**    to leak resource from timeto time...
*/
void _jsGcmDestroyRM( jsGcmResource* gcmResource )
{
    if ( gcmResource->hostMemoryBase ) 
		jsFree( gcmResource->hostMemoryBase );
	
	if ( _jsGcmState.reportAddressMain )
	{
		// using sys memory free to go with the sys_memory_allign used 
		// to allocate this block
		sys_memory_free( *reinterpret_cast<sys_addr_t*>(&_jsGcmState.reportAddressMain) ); 
		_jsGcmState.reportAddressMain = NULL; 
	}
		
    memset(( void* )gcmResource, 0, sizeof( jsGcmResource ) );

    return;
}

void _jsGcmGraphicsHandler( const uint32_t head )
{
    // GCM will call this Graphics Handler if there is a channel error which 
	// can be caused by bad fifo commands, and GPU error, or GPU memory access. 
    
    printf( "========================================\n" );
    printf( " PSGL [_jsGcmGraphicsHandler]  \n" );
    printf( " GCM triggers this because of RSX error \n" );
    printf( "  due to invalid Fifo Commands, \n" );
    printf( "  invalid GPU state, or invalid memory access\n" );
    printf( "========================================\n" );

	// print out the previous 10 words from the current position;
	_jsGcmState.fifo.updateLastGetRead(); 
	
	// Dumping current fifo state 
	printf(" Current PSGL FIFO info \n" ); 
	printf(" Fifo Begin %p End %p Current %p and Get %p \n", 
		   _jsGcmState.fifo.begin, 
		   _jsGcmState.fifo.end,
		   _jsGcmState.fifo.current,
		   _jsGcmState.fifo.lastGetRead ); 

	printf(" Last 10 words of the PSGL Fifo from the ppu put/current position \n" );  
	_jsPrintFifoFromPut( 10 ); 

	printf(" Last 10 words of the PSGL Fifo from the gpu get position \n" );  
	_jsPrintFifoFromGet( 10 ); 


#ifdef JSGCM_DEBUG_CHANNEL_ERROR
    printf( "========================================\n" );
    printf( " PSGL [_jsBackTrace] below \n" );
    printf( " Use objdump or debugger to track source\n" );
    printf( " of the invalid commands or data 	\n" );
    printf( "========================================\n" );

#endif

}

extern GLboolean _psglDisableCompression;

int _jsGcmInitRM( jsGcmResource *gcmResource, unsigned int hostMemorySize, int inSysMem, unsigned int dmaPushBufferSize )
{
    memset( gcmResource, 0, sizeof( jsGcmResource ) );

    // DPM2 systems (that is, HUD, needs a bit of memory on XDR available for geometry.
    // Compute this amoutn via a callback.
#ifdef JS_DPM
#ifdef JS_DPM_V2
    unsigned int iDPM2DataAreaSize = _jsdpmEstimateDataAreaSize();
#else
    const unsigned int iDPM2DataAreaSize = 0;
#endif // JS_DPM_V2
#else
    const unsigned int iDPM2DataAreaSize = 0;
#endif

    // XXX currently we need to decide how much host memory is needed before we know the GPU type
    // It sucks because we don't know if the push buffer is in host memory or not.
    // So, assume that it is...
    dmaPushBufferSize = _jsPad( dmaPushBufferSize, _JSGCM_HOST_BUFFER_ALIGNMENT );

    // in case of host push buffer we need to add padding to avoid GPU push buffer prefetch to
    // cause a problem fetching invalid addresses at the end of the push buffer.
    gcmResource->hostMemorySize = _jsPad( _JSGCM_FIFO_SIZE + hostMemorySize + dmaPushBufferSize + _JSGCM_DMA_PUSH_BUFFER_PREFETCH_PADDING 
		//+ iDPM2DataAreaSize, 1 << 20 ); // also need to add in the space needed for the report query locations 
		+ iDPM2DataAreaSize + (JSGCM_LM_MAX_TOTAL_QUERIES * sizeof( GLuint )), 1 << 20 );

    if ( gcmResource->hostMemorySize > 0 )
    {
        gcmResource->hostMemoryBase = ( char * )jsMemalign( 1 << 20, gcmResource->hostMemorySize  );
    }

    // Initialize RSXIF
    if ( cellGcmInitExpectedReturn != cellGcmInit( _JSGCM_FIFO_SIZE, gcmResource->hostMemorySize, gcmResource->hostMemoryBase ) ) // 64 KB is minimum fifo size for libgpu (for now)
    {
        fprintf( stderr, "RSXIF failed initialization\n" );
        JS_ASSERT( 0 );
        return GL_FALSE;
    }
    
    cellGcmSetDebugOutputLevel( CELL_GCM_DEBUG_LEVEL2 );
    // set the jsGcm graphics error callback
    cellGcmSetGraphicsHandler( &_jsGcmGraphicsHandler );

    // Get Gpu configration
    CellGcmConfig config;
    cellGcmGetConfiguration( &config );

#ifdef DISABLE_POWER_MANAGEMENT
    _cellGcmDisablePowerManagement();
#endif

    gcmResource->localAddress = ( char * )config.localAddress;
    gcmResource->localSize = config.localSize;
    gcmResource->MemoryClock = config.memoryFrequency;
    gcmResource->GraphicsClock = config.coreFrequency;

    gcmResource->semaphores = ( jsGcmSemaphoreMemory * )cellGcmGetLabelAddress( 0 );
    gcmResource->dmaControl = ( char* ) cellGcmGetControlRegister() - (( char * ) & (( jsGcmControlDma* )0 )->Put - ( char * )0 );
    int hostPushBuffer = 0;
    hostPushBuffer = 1;


//#define FORCE_VIDMEM_PUSHBUFFER
#ifdef FORCE_VIDMEM_PUSHBUFFER
    hostPushBuffer = 0;
#endif

    // the IOIF mapping don't work. work-around here.
    for ( GLuint i = 0;i < 32;++i ) gcmResource->ioifMappings[i] = ( unsigned long long )( unsigned long )( gcmResource->localAddress + ( 64 << 20 ) * ( i / 4 ) );

    cellGcmFinish( 1 ); // added just a constant value for now to adjust to the inline libgcm interface change

    if ( hostPushBuffer )
    {
        gcmResource->hostMemorySize -= dmaPushBufferSize + _JSGCM_DMA_PUSH_BUFFER_PREFETCH_PADDING;
        gcmResource->dmaPushBuffer = gcmResource->hostMemoryBase + gcmResource->hostMemorySize;
        gcmResource->dmaPushBufferOffset = ( char * )gcmResource->dmaPushBuffer - ( char * )gcmResource->hostMemoryBase;
        gcmResource->linearMemory = ( char* )0x0;
        gcmResource->persistentMemorySize = gcmResource->localSize - _JSGCM_RESERVED_VIDEO_MEMORY;
    }
    else
    {
        // Allocate Fifo at begining of vmem map
        gcmResource->dmaPushBuffer = gcmResource->localAddress;
        gcmResource->dmaPushBufferOffset = ( char * )gcmResource->dmaPushBuffer - ( char * )gcmResource->localAddress;
        gcmResource->linearMemory = ( char* )0x0 + dmaPushBufferSize;
        gcmResource->persistentMemorySize = gcmResource->localSize - dmaPushBufferSize - _JSGCM_RESERVED_VIDEO_MEMORY;
    }
    gcmResource->dmaPushBufferSize = dmaPushBufferSize;
    gcmResource->hostMemoryReserved = _JSGCM_FIFO_SIZE;

    // Set Jump command to our fifo structure
    cellGcmSetJumpCommand(( char * )gcmResource->dmaPushBuffer - ( char * )gcmResource->hostMemoryBase );

    // Set our Fifo functions
    gCellGcmCurrentContext->callback = ( CellGcmContextCallback )_jsOutOfSpaceCallback;


    _jsGcmPrintf( "JSGCM resource: MClk: %f Mhz NVClk: %f Mhz\n", ( float )gcmResource->MemoryClock / 1E6, ( float )gcmResource->GraphicsClock / 1E6 );
    _jsGcmPrintf( "JSGCM resource: Video Memory: %i MB\n", gcmResource->localSize / ( 1024*1024 ) );
    _jsGcmPrintf( "JSGCM resource: localAddress mapped at %p\n", gcmResource->localAddress );
    _jsGcmPrintf( "JSGCM resource: push buffer at %p - %p (size = 0x%X), offset=0x%lx\n",
                  gcmResource->dmaPushBuffer, ( char* )gcmResource->dmaPushBuffer + gcmResource->dmaPushBufferSize, gcmResource->dmaPushBufferSize, gcmResource->dmaPushBufferOffset );
    _jsGcmPrintf( "JSGCM resource: dma control at %p\n", gcmResource->dmaControl );
    
	if ( _psglDisableCompression )
    {
        _JS_REPORT_EXTRA( PSGL_REPORT_DEBUG, "Compression disabled" );
    }

    return 1;
}



