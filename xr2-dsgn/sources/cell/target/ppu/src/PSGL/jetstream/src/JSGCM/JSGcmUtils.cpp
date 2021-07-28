/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 *                Copyright (C) 2009 Sony Computer Entertainment Inc.
 *                                               All Rights Reserved.
 */

 
/*
 * 
 */

#include <stdio.h>
#include <sys/memory.h>
#include <PSGL/psgl.h>
#include <PSGL/private.h>
#include <PSGL/Utils.h>
#include <PSGL/Debug.h>
#include <PSGL/PerfCounters.h>
#include <spu/spuTypes.h>
#include <PSGL/DPM2.h>
#include <JSGCM/JSGcmUtils.h>
#include <JSGCM/PlatformTypes.h>

PerformanceCounter_t stallCounter;



#ifdef JS_PLATFORM_CELL
// do not turn on the hack for now.
// It is currently broken because of the memory layout on the spu.
//#define SPU_NV_FIFO
#endif

#ifdef SPU_NV_FIFO
#include <sys/raw_spu.h>

extern raw_spu_t _jsAsyncCopySPU;	// SPU ID

void _jsGcmSetFifoSPU( JSGCM_FIFO *fifo, unsigned int spuid );

void initSpuFifo()
{
    if ( _jsAsyncCopySPU >= 0 )
    {
        printf( "using hack\n" );
        _jsGcmSetFifoSPU( &_jsGcmState.fifo, _jsAsyncCopySPU );
    }
}
#endif

#if JS_PLATFORM_CELL
// SPU initialization
//  For application SPUs, the IOIF configuration must be uploaded to enable
//  multi-way DMA.  The first 32 entries of this array are the window
//  addresses, and the 33rd is the default offset.  The array is padded to
//  16 bytes for DMA.
static unsigned long long _jsSPUData[34] __attribute__(( aligned( 16 ) ) );
#endif

/**
 * @addtogroup PSGL
 *
 * @{
 */

/**
 *  @short (Deprecated) Obtains PSGL information that is used to initialize the PSGL SPU library
 *
 *  This function has been deprecated. 
 */
void* psglGetSPUInitData( void )
{
#if JS_PLATFORM_CELL
    return _jsSPUData;
#else
    _jsSetError( GL_INVALID_OPERATION );
    return NULL;
#endif
}

/**
 * @}
 */

static GLuint MemoryClock = 0;
static GLuint GraphicsClock = 0;

GLuint _jsGcmGetMemoryClock()
{
    return MemoryClock;
}

GLuint _jsGcmGetGraphicsClock()
{
    return GraphicsClock;
}

GLboolean _jsGcmInit( PSGLinitOptions* options, jsGcmResource *resource )
{
    if ( !_jsGcmInitFromRM( resource ) )
    {
        fprintf( stderr, "PSGL GCM failed initialisation" );
        JS_ASSERT( 0 );
        return GL_FALSE;
    }
    MemoryClock = resource->MemoryClock;
    GraphicsClock = resource->GraphicsClock;

#ifdef SPU_NV_FIFO
    initSpuFifo();
#endif

    if ( gmmInit( resource->localAddress, // pass in the base address, which "could" diff from start address
                  resource->localAddress,
                  resource->localSize,
                  resource->hostMemoryBase, // pass in the base address
                  resource->hostMemoryBase + resource->hostMemoryReserved,
                  resource->hostMemorySize - resource->hostMemoryReserved ) == GMM_ERROR )
    {
        fprintf( stderr, "Could not init GPU memory manager" );
        _jsGcmDestroy();
        JS_ASSERT( 0 );
        return GL_FALSE;
    }

#if JS_PLATFORM_CELL
    // XXX Disable slow range until RSXIF provides valid IOIF window
    // addresses
    _jsAsyncCopySetSlowRange(
        _jsGcmState.localAddress,
        resource->localSize,
        resource->ioifMappings );

    // set up application SPU initialization block
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
    const int nBlocks = _jsPad( resource->localSize, 1 << 26 ) >> 26;
    const int nWindows = 4 * nBlocks;
    JS_ASSERT( nWindows <= 32 );
    _jsSPUData[32] = ( unsigned long long )( unsigned long )_jsGcmState.localAddress;
    for ( int i = 0; i < nWindows; ++i )
        _jsSPUData[i] = resource->ioifMappings[i];
#endif

    // initialize DMA sync mechanism
    _jsGcmInitSync();

    _JS_PERFORMANCE_RESET( stallCounter );

	// Going to need to allocate for the getReportValues for ZCull and Zpass queries
 	_jsGcmState.reportCompletionValues = (volatile GLuint*)( resource->hostMemoryBase);
	// Allocating the report queries out of the hostmemorybase just before the fifo.  
	_jsGcmState.reportCompletionValues += (resource->hostMemorySize - JSGCM_LM_MAX_TOTAL_QUERIES * sizeof( GLuint ))/4 ;

    _jsGcmPrintf( " _jsGcmState.reportCompletionValues %p \n", _jsGcmState.reportCompletionValues );
	
    JS_ASSERT( _jsGcmState.reportCompletionValues );

	// setting false for now until everything is in place 
	_jsGcmState.reportsInMainMemory = 0; 

#ifdef JSGCM_USE_MAIN_MEMORY_REPORTS
	_jsGcmState.reportsInMainMemory = 1; 
#endif 

	// init the report memory address
	_jsGcmState.reportAddressMain = NULL;

	// Reports in main memory allocation
	if ( _jsGcmState.reportsInMainMemory )
	{
		// Allocated and Map Main Memory for report to main memory.
		// Report Main Memory address must be mapped to Io address 0x0e000000.
		// Then using cellGcmMapEaIoAddress for mapping address explicitly.		

		// Need to allocate 1 mb at 1 mb alignment for the actual reports separate from memory allocated for cellGcmInit 
		//_jsGcmState.reportAddressMain = (CellGcmReportData *)memalign(1*1024*1024, 1024*1024); 

		// Using this sys memory allocate just incase the base memalign has been overloaded to ensure 1mb page sizes 
		if(sys_memory_allocate(1*1024*1024, SYS_MEMORY_PAGE_SIZE_1M, reinterpret_cast<sys_addr_t*>(&_jsGcmState.reportAddressMain)) != CELL_OK)
		{
			_jsGcmPrintf("PSGL:JSGcmUtils.cpp Report to Main memory, allocate main-memory for reports failed\n"); 
			JS_ASSERT( 0 ); 
		}

		// now explicityly map it as perscribed above 
		if(cellGcmMapEaIoAddress(_jsGcmState.reportAddressMain,0x0e000000, 1*1024*1024) != CELL_OK)
		{
			_jsGcmPrintf("PSGL:JSGcmUtils.cpp Report to Main memory, Map Ea address to Io Address  failed\n");			
			JS_ASSERT( 0 ); 
		}
	
		// init report address 
		memset( _jsGcmState.reportAddressMain, 0, 1024*1024 );
		

		_jsGcmState.totalMainMemoryReports = (1024*1024 / 16 );
		_jsGcmState.totalPSGLMainMemoryReports = _jsGcmState.totalMainMemoryReports; // - SN_TUNER_RSX_MARKER_OFFSET;
		//_jsGcmState.totalOcclusionReports = _jsGcmState.totalPSGLMainMemoryReports - 4; 
		//_jsGcmState.totalZCullReports = 4; 
	}

    // init values to 0
    memset(( void * ) _jsGcmState.reportCompletionValues, 0, JSGCM_LM_MAX_TOTAL_QUERIES * sizeof( GLuint ) );
    _jsGcmState.reportCompletionValue = JSGCM_REPORT_COMPLETE_VALUE;

	// finally init our offset for the dma as well. 
	if ( cellGcmAddressToOffset( (void*)(_jsGcmState.reportCompletionValues), &_jsGcmState.reportCompletionOffset ) != CELL_OK )
		JS_ASSERT( 0 );

	// also need to init the labelValue for waiting for idle 
	_jsGcmState.labelValue = 1; 

	// also again init the record mode stuff 
	_jsGcmState.recordFifoMode = GL_FALSE;
	_jsGcmState.recordPrevBegin = NULL;	
	_jsGcmState.recordPrevEnd = NULL;	
	_jsGcmState.recordPrevCurrent = NULL;
	_jsGcmState.recordOutOfSpaceCallback = NULL;
	_jsGcmState.recordStallCallback = NULL; 

    return GL_TRUE;
}

void _jsGcmAllocDestroy()
{
    gmmDestroy();

    _jsGcmDestroy();
#ifdef JS_PERFORMANCE_INFO
    printf( "Total push stall time: %f s\n", _JS_PERFORMANCE_VALUE( stallCounter ) );
#endif
}

void _jsGcmMemcpy( const GLuint dstId, unsigned dstOffset, unsigned int pitch, const GLuint srcId, GLuint srcOffset, unsigned int size )
{
    _JS_PERFORMANCE_START( NVMemcpyCounter );

    // check alignment
    //  Vid to vid copy requires 64-byte aligned base address (for dst pointer).
    if ((gmmIdToOffset(dstId) % 64 ) == 0 && ( dstOffset % 2 ) == 0 && 
        (gmmIdToOffset(srcId) % 2 ) == 0 && ( srcOffset % 2) == 0 &&
        ( size % 2 ) == 0 && ( pitch % 64 ) == 0 )
    {
        // configure a 2D transfer
        //
        // align destination
        {
            pitch = pitch ? : 64; // minimum pitch
            // target buffer isn't tiled, we just need to align on pitch
            const GLuint dstOffsetAlign = dstOffset % pitch;
            if ( dstOffsetAlign )
            {
                const GLuint firstBytes = MIN( pitch - dstOffsetAlign, size );

                _jsGcmFifoGlTransferDataVidToVid(
                    dstId,
                    0,
                    pitch,					// dst pitch
                    dstOffsetAlign / 2, dstOffset / pitch,		// dst x,y start
                    srcId,
                    srcOffset,
                    pitch,					// src pitch
                    0, 0,				// src x,y start
                    firstBytes / 2, 1,		// size in pixels
                    2 );					// pixel size in bytes
                dstOffset += firstBytes;
                srcOffset += firstBytes;
                size -= firstBytes;
            }
        }

        const GLuint fullLines = size / pitch;
        const GLuint extraBytes = size % pitch;
        if ( fullLines )
            _jsGcmFifoGlTransferDataVidToVid(
                dstId,
                0,
                pitch,					// dst pitch
                0, dstOffset / pitch,				// dst x,y start
                srcId,
                srcOffset,
                pitch,					// src pitch
                0, 0,				// src x,y start
                pitch / 2, fullLines,		// size in pixels
                2 );					// pixel size in bytes
        if ( extraBytes )
            _jsGcmFifoGlTransferDataVidToVid(
                dstId,
                0,
                pitch,					// dst pitch
                0, fullLines + dstOffset / pitch,		// dst x,y start
                srcId,
                srcOffset,
                pitch,					// src pitch
                0, fullLines,		// src x,y start
                extraBytes / 2, 1,		// size in pixels
                2 );					// pixel size in bytes
    }
    else
    {
      _jsGcmTransferData( dstId, dstOffset, size, srcId, 0, size, size, 1 );
    }

    _JS_PERFORMANCE_STOP( NVMemcpyCounter );

}

void _jsGcmCopyBounceToGPU( unsigned int dstId, unsigned dstOffset, unsigned int pitch, unsigned int srcId, unsigned int srcOffset, unsigned int size )
{
    _jsGcmMemcpy( dstId, dstOffset, pitch, srcId, srcOffset, size );
}

void _jsGcmSend( unsigned int dstId, unsigned dstOffset, unsigned int pitch, const char *src, unsigned int size )
{
    // try allocating the whole block in the bounce buffer
    GLuint id = _jsGcmAllocateMemory( size );
    // we should be able to allocate bounce buffer all at once
    JS_ASSERT( id != GMM_ERROR );

    fast_memcpy( gmmIdToAddress(id), src, size );
    _jsGcmCopyBounceToGPU( dstId, dstOffset, pitch, id, 0, size );
    
    _jsGcmFreeMemory( id );
}

GLuint _jsGcmAllocateMemory( unsigned int size )
{
    GLuint retId = gmmAlloc((CellGcmContextData*)&_jsGcmState.fifo,
                            CELL_GCM_LOCATION_LOCAL,
                            0,
                            size);

    _JS_REPORT_EXTRA( PSGL_REPORT_GPU_MEMORY_ALLOC, "Allocating GPU memory: %d bytes allocated at id 0x%08x", size, retId );
    return retId;
}

GLuint _jsGcmAllocateHostMemory( unsigned int size )
{
    GLuint retId = gmmAlloc((CellGcmContextData*)&_jsGcmState.fifo, 
                            CELL_GCM_LOCATION_MAIN,
                            0,
                            size);

    _JS_REPORT_EXTRA( PSGL_REPORT_HOST_MEMORY_ALLOC, "Allocating host memory: %d bytes allocated at id 0x%08x", size, retId );
    return retId;
}

GLuint _jsGcmAllocateTextureMemory( unsigned int size )
{
    GLuint retId = gmmAlloc((CellGcmContextData*)&_jsGcmState.fifo, 
                            CELL_GCM_LOCATION_LOCAL,
                            0,
                            size);

    _JS_REPORT_EXTRA( PSGL_REPORT_GPU_MEMORY_ALLOC, "Allocating GPU memory: %d bytes allocated at id 0x%08x", size, retId );
    return retId;
}

// srcVidAdr represents the first byte of the data to copy.
// The pixel at srcVidAdr+(height-1)*srcPitch will be copied at
// dstVidAdr+dstPitch*dstY+bpp*dstX.
void _jsGcmTransferImageYInvert(  uint8_t mode, uint32_t dstOffset, 
							 uint32_t dstPitch, uint32_t dstX, uint32_t dstY, 
							 uint32_t srcOffset, uint32_t srcPitch, 
							 uint32_t dstWidth, uint32_t dstHeight, uint32_t bytesPerPixel)
{
	const uint32_t BLOCKSIZE = 1024; 

	uint32_t srcFormat, dstFormat;	
	uint32_t x;
	uint32_t y;
	
	// sanity test
	JS_ASSERT((dstOffset & 63) == 0); // 64byte alignment
	JS_ASSERT((dstPitch & 63) == 0); // 64byte alignment
	JS_ASSERT(srcPitch < 0xffff);
	JS_ASSERT(dstPitch < 0xffff);

	// determine color format
	switch(bytesPerPixel)
	{
	case 2:
		srcFormat = CELL_GCM_TRANSFER_SCALE_FORMAT_R5G6B5;
		dstFormat = CELL_GCM_TRANSFER_SURFACE_FORMAT_R5G6B5;
		break;
	case 4:
	case 8:
	case 16:
		srcFormat = CELL_GCM_TRANSFER_SCALE_FORMAT_A8R8G8B8;
		dstFormat = CELL_GCM_TRANSFER_SURFACE_FORMAT_A8R8G8B8;
		dstWidth*=bytesPerPixel/4;
		bytesPerPixel=4;
		break;
	case 1: // LE_Y8 is not supported on curie
	default:
		srcFormat = 0;
		dstFormat = 0;
		JS_ASSERT(0);
		break;
	}

	// source scale attribute
	CellGcmTransferScale src;
	{
		src.conversion = CELL_GCM_TRANSFER_CONVERSION_TRUNCATE;
		src.format = srcFormat; //CELL_GCM_TRANSFER_SCALE_FORMAT_A8R8G8B8;
		src.operation = CELL_GCM_TRANSFER_OPERATION_SRCCOPY;
		// 
		// XXX: Looks like we need to set up SetContext{Beta1|Beta4}
		//      in order to use Blending functionality
		//
		//src.operation = CELL_GCM_NV3089_SET_OPERATION_BLEND_AND;
		//
		src.clipX = 0;
		src.clipY = 0;
		src.clipW = BLOCKSIZE; //DST_BLOCK;		// dest width
		src.clipH = BLOCKSIZE; //DST_BLOCK;	// dest height
		src.outX = 0;
		src.outY = 0;
		src.outW = BLOCKSIZE; //DST_BLOCK;		// dest width
		src.outH = BLOCKSIZE; //DST_BLOCK;		// dest height
		src.inW  = BLOCKSIZE;//SRC_BLOCK;		// source width
		src.inH = BLOCKSIZE;	    // source height 

		src.ratioX = cellGcmGetFixedSint32((float)src.inW / (float)src.outW );		// 
		src.ratioY = cellGcmGetFixedSint32((float)src.inH / (float)src.outH );		// 

		src.pitch  = srcPitch;		// source pitch
		src.origin = CELL_GCM_TRANSFER_ORIGIN_CORNER;
		src.interp = CELL_GCM_TRANSFER_INTERPOLATOR_ZOH;
		src.offset = srcOffset;	// source offset

		// sampling point is texel's upper left,
		// so we shift 1 texel to pick center.
		src.inX = cellGcmGetFixedUint16(1.0f);
		src.inY = cellGcmGetFixedUint16(1.0f);
	}

	// destination surface
	CellGcmTransferSurface dst;
	{
		dst.format  = dstFormat;// CELL_GCM_TRANSFER_SURFACE_FORMAT_A8R8G8B8;
		dst.pitch   = dstPitch;//sFrameBuffer[sFrameIndex].pitch;
		dst.offset  = dstOffset;//sFrameBuffer[sFrameIndex].offset;
	}

	// set scale surface
	GCM_FUNC(cellGcmSetTransferScaleMode , mode, CELL_GCM_TRANSFER_SURFACE );

	// split large blits
	const uint32_t finalDstX = dstX + dstWidth;
	const uint32_t finalDstY = dstY + dstHeight;

	for(y = dstY; y < finalDstY; )
	{
		// determine this blits height
		uint32_t dstTop = y & ~(BLOCKSIZE - 1);
		uint32_t dstBot = dstTop + BLOCKSIZE;
		uint32_t dstBltHeight = ((dstBot<finalDstY) ? dstBot : finalDstY) - y;
		for(x = dstX; x < finalDstX; )
		{
			// determine this blits width
			uint32_t dstLeft = x & ~(BLOCKSIZE - 1);
			uint32_t dstRight = dstLeft + BLOCKSIZE;
			uint32_t dstBltWidth = ((dstRight<finalDstX) ? dstRight : finalDstX) - x;

			// align the surface/destination surface properly
			uint32_t dstBlockOffset = bytesPerPixel * dstLeft + dstPitch * dstTop;
			uint32_t srcBlockOffset = bytesPerPixel * (x-dstX) + srcPitch * (dstHeight-(y-dstY)-dstBltHeight);

			// handle class behavior
			uint32_t safeDstBltWidth = (dstBltWidth < 16) ? 16 : (dstBltWidth + 1) & ~1;

			// update the scale source struct for the inverted transfer 			
			src.clipX = x-dstLeft;
			src.clipY = y-dstTop;
			src.clipW = dstBltWidth;
			src.clipH = dstBltHeight;
			src.outX =  x-dstLeft;
			src.outY =  y-dstTop;
			src.outW = dstBltWidth;
			src.outH = dstBltHeight;
			src.ratioX = 1 << 20; 
			src.ratioY = -(1U << 20 ); 
			src.inW = safeDstBltWidth;
			src.inH = dstBltHeight; 
			
			src.offset = srcOffset +srcBlockOffset;
			src.inX = 0; 
			src.inY = ((dstBltHeight-1)<<4);

			// update dst surface 
			dst.offset = dstOffset + dstBlockOffset; 

			GCM_FUNC(cellGcmSetTransferScaleSurface,&src, &dst);

			x+=dstBltWidth;
		}
		y+=dstBltHeight;
	}
	
}




void _jsGcmSetTransferScaleBlit(
	uint8_t mode,           // Mode
    uint32_t dstOffset,     // Destination Offset
    uint32_t dstPitch,      // Destination Pitch
    uint32_t srcOffset,     // Src Offset
	uint32_t srcPitch,      // Source Pitch
    uint32_t dstWidth,      // src width
    uint32_t dstHeight,     // src height 
	uint32_t bytesPerPixel, // Bytes Per Pixel
    int32_t log2XScale,     // Log 2X Scale
    int32_t log2YScale,     // Log 2Y Scale
	uint32_t filter         // Filter    
	)
{
    uint32_t NV_SURFACE2D_MAX_DIM = 10; // src image needs to be <=2046, we max it to 1024
    uint32_t SRC_BLOCKSIZE = 1 << NV_SURFACE2D_MAX_DIM;
    uint32_t BLOCKSIZE_X = 1 << (NV_SURFACE2D_MAX_DIM+log2XScale);
    uint32_t BLOCKSIZE_Y = 1 << (NV_SURFACE2D_MAX_DIM+log2YScale);
    uint32_t srcFormat, dstFormat;	
	uint32_t interp = filter ? CELL_GCM_TRANSFER_INTERPOLATOR_FOH : CELL_GCM_TRANSFER_INTERPOLATOR_ZOH;
	uint32_t xoffset = 0;
	uint32_t yoffset = 0;

    // sanity test
    CELL_GCM_ASSERT((dstOffset & 63) == 0); // 64byte alignment
    CELL_GCM_ASSERT((dstPitch & 63) == 0); // 64byte alignment
    CELL_GCM_ASSERT(srcPitch < 0xffff);
    CELL_GCM_ASSERT(dstPitch < 0xffff);

	if (filter)
	{
		xoffset=8<<(16-log2XScale)>>16;	
		yoffset=8<<(16-log2YScale)>>16;	
	}

		// determine color format
	switch(bytesPerPixel)
	{
	case 2:
		srcFormat = CELL_GCM_TRANSFER_SCALE_FORMAT_R5G6B5;
		dstFormat = CELL_GCM_TRANSFER_SURFACE_FORMAT_R5G6B5;
		break;
	case 4:
	case 8:
	case 16:
		srcFormat = CELL_GCM_TRANSFER_SCALE_FORMAT_A8R8G8B8;
		dstFormat = CELL_GCM_TRANSFER_SURFACE_FORMAT_A8R8G8B8;
		dstWidth*=bytesPerPixel/4;
		bytesPerPixel=4;
		break;
	case 1: // LE_Y8 is not supported on curie
	default:
		srcFormat = 0;
		dstFormat = 0;
		JS_ASSERT(0);
		break;
	}

	// source scale attribute
	CellGcmTransferScale src;
	{
		src.conversion = CELL_GCM_TRANSFER_CONVERSION_TRUNCATE;
		src.format = srcFormat; //CELL_GCM_TRANSFER_SCALE_FORMAT_A8R8G8B8;
		src.operation = CELL_GCM_TRANSFER_OPERATION_SRCCOPY;
		// 
		// XXX: Looks like we need to set up SetContext{Beta1|Beta4}
		//      in order to use Blending functionality
		//
		//src.operation = CELL_GCM_NV3089_SET_OPERATION_BLEND_AND;
		//
		src.clipX = 0;
		src.clipY = 0;
		src.clipW = BLOCKSIZE_X; //DST_BLOCK;		// dest width
		src.clipH = BLOCKSIZE_Y; //DST_BLOCK;	// dest height
		src.outX = 0;
		src.outY = 0;
		src.outW = BLOCKSIZE_X; //DST_BLOCK;		// dest width
		src.outH = BLOCKSIZE_Y; //DST_BLOCK;		// dest height
		src.inW  = SRC_BLOCKSIZE;//SRC_BLOCK;		// source width
		src.inH = SRC_BLOCKSIZE;	    // source height 

		src.ratioX = cellGcmGetFixedSint32((float)src.inW / (float)src.outW );		// 
		src.ratioY = cellGcmGetFixedSint32((float)src.inH / (float)src.outH );		// 

		src.pitch  = srcPitch;		// source pitch
		src.origin = CELL_GCM_TRANSFER_ORIGIN_CORNER;
		src.interp = interp;
		src.offset = srcOffset;	// source offset

		// sampling point is texel's upper left,
		// so we shift 1 texel to pick center.
		src.inX = cellGcmGetFixedUint16(1.0f);
		src.inY = cellGcmGetFixedUint16(1.0f);
	}

	// destination surface
	CellGcmTransferSurface dst;
	{
		dst.format  = dstFormat;// CELL_GCM_TRANSFER_SURFACE_FORMAT_A8R8G8B8;
		dst.pitch   = dstPitch;//sFrameBuffer[sFrameIndex].pitch;
		dst.offset  = dstOffset;//sFrameBuffer[sFrameIndex].offset;
	}


	// set scale surface
	GCM_FUNC( cellGcmSetTransferScaleMode , mode, CELL_GCM_TRANSFER_SURFACE );

    // split large blits
    for(uint32_t dsty = 0; dsty < dstHeight; dsty+=BLOCKSIZE_Y)
    {
		  uint32_t srcy = (dsty << (16 - log2YScale)) >> 16;
          // determine this blits height
		  uint32_t dstBltHeight = dstHeight - dsty;
		  if(dstBltHeight > BLOCKSIZE_Y) 
			  dstBltHeight = BLOCKSIZE_Y;

          for(uint32_t dstx = 0; dstx < dstWidth;dstx+=BLOCKSIZE_X)
          {
			      uint32_t srcx = (dstx << (16 - log2XScale)) >> 16;
                  // determine this blits width
                  uint32_t dstBltWidth = dstWidth - dstx;
			      if(dstBltWidth > BLOCKSIZE_X)
				      dstBltWidth = BLOCKSIZE_X;

                  // align the surface/destination surface properly
                  uint32_t dstBlockOffset = bytesPerPixel * dstx + dstPitch * dsty;
                  uint32_t srcBlockOffset = bytesPerPixel * srcx + srcPitch * srcy;

			      // update the scale struct 
			      src.clipX = src.clipY = src.outX = src.outY = 0; 
			      src.clipW = dstBltWidth;
			      src.clipH = dstBltHeight; 
			      src.ratioX = 1 << ( 20 - log2XScale ); 
			      src.ratioY = 1 << ( 20 - log2YScale ); 

			      src.offset = srcOffset + srcBlockOffset; 
			      src.inX = xoffset; 
			      src.inY = yoffset; 

			      // update dst surface 
			      dst.offset = dstOffset + dstBlockOffset; 

			      GCM_FUNC(cellGcmSetTransferScaleSurface, &src, &dst);
        }
    }

}


void _jsGcmScaleBlit(            
    GLuint dstVidId,       // Destination Video memory id
    GLuint dstVidIdOffset,
    GLuint dstPitch,       // Destination Pitch
    GLuint srcVidId,       // Source Video memory id
    GLuint srcVidIdOffset,
    GLuint srcPitch,       // Source Pitch
    GLuint dstWidth,       // Destination Width
    GLuint dstHeight,      // Destination Height
    GLuint bytesPerPixel,  // Bytes Per Pixel
    int log2XScale,        // Log 2X Scale
    int log2YScale,        // Log 2Y Scale
    GLboolean filter       // Filter
)
{

    GLuint srcOffset = gmmIdToOffset( srcVidId ) + srcVidIdOffset;
    GLuint dstOffset = gmmIdToOffset( dstVidId ) + dstVidIdOffset;

    // [Jun] Choose mode
    uint8_t mode = CELL_GCM_TRANSFER_LOCAL_TO_LOCAL;

    if ( gmmIdIsMain(srcVidId) )
    {
        mode = CELL_GCM_TRANSFER_MAIN_TO_LOCAL;
    }

    if ( gmmIdIsMain(dstVidId) )
    {
        // [Jun] Host to host is not supported by cellGcmSetScaleBlit
        JS_ASSERT( gmmIdIsMain(srcVidId) == 0 );
        mode = CELL_GCM_TRANSFER_LOCAL_TO_MAIN;
    }

	// [RSTENSON] need new scale blit 
	_jsGcmSetTransferScaleBlit( mode, dstOffset, dstPitch, srcOffset, srcPitch, dstWidth, dstHeight, 
		bytesPerPixel, log2XScale, log2YScale, filter ); 
}



// srcVidAdr represents the first byte of the data to copy.
// The pixel at srcVidAdr+(height-1)*srcPitch will be copied at
// dstVidAdr+dstPitch*dstY+bpp*dstX.
void _jsGcmTransferDataVidToVidYInv
(
    GLuint dstVidId,        // linear destination surface
    GLuint dstPitch,
    GLuint dstX,
    GLuint dstY,
    GLuint srcVidId,        // linear source surface
    GLuint srcVidIdOffset,
    GLuint srcPitch,
    GLuint srcWidth,            // size in pixel
    GLuint srcHeight,
    GLuint bytesPerPixel
)
{

    GLuint srcOffset = gmmIdToOffset( srcVidId ) + srcVidIdOffset;
    GLuint dstOffset = gmmIdToOffset( dstVidId );

    // [RSTENSON] Context DMA mode
    uint8_t mode = CELL_GCM_TRANSFER_LOCAL_TO_LOCAL;

    if ( gmmIdIsMain(srcVidId) )
    {
        mode = CELL_GCM_TRANSFER_MAIN_TO_LOCAL;
    }

    if ( gmmIdIsMain(dstVidId) )
    {
        // [RSTENSON] Host to host is not supported by cellGcmSetScaleBlitYInvert
        JS_ASSERT( gmmIdIsMain(srcVidId) == 0 );
        mode = CELL_GCM_TRANSFER_LOCAL_TO_MAIN;
    }

    // [RSTENSON] GCM_PORT_TESTED Heavily... This is used all over the place
	_jsGcmTransferImageYInvert ( mode, dstOffset, dstPitch, dstX, dstY, srcOffset, srcPitch, 
		srcWidth, srcHeight, bytesPerPixel ); 
}





// This code is lifted and modified from JSGCM to provide deswizzling.  The
// original function is _jsGcmSwizzleDataCPU.  Changes were:
//
// - Function name change.
// - Switch const modifier between pointer arguments.
// - Global replace of src/dst and vice versa.
// - Reverse arguments to copyTexel.

/* calculate the swizzled address given texel coordinates
** and the log2 extents of the texture
**
** note:
**  - generic code, not optimized
*/
static GLuint _jsGcmCalcSwizzledAddress
(
    GLuint u,
    GLuint v,
    GLuint p,
    GLuint log2depth,
    GLuint log2heigth,
    GLuint log2width )
{
    GLuint offsetBitPos = 0;
    GLuint offset = 0;

    while ( log2depth + log2heigth + log2width )
    {
        if ( log2width )
        {
            log2width--;
            offset |= ( u & 1 ) << offsetBitPos;
            u >>= 1;
            offsetBitPos++;
        }
        if ( log2heigth )
        {
            log2heigth--;
            offset |= ( v & 1 ) << offsetBitPos;
            v >>= 1;
            offsetBitPos++;
        }
        if ( log2depth )
        {
            log2depth--;
            offset |= ( p & 1 ) << offsetBitPos;
            p >>= 1;
            offsetBitPos++;
        }
    }

    return offset;
}

/* swizzle data for non-square textures.  This is a generic function that
** uses the CPU to swizzle a texture (w/without power of two extents) into
** another texture that has power of two extents.
**
** note:
**  - a lot of parameters but there but essentially it is just a source and destintion
**    surface description
**  - this function does not sync anything (the caller is responsilbe for this)
**  - this function is slow because it is generic code, not even attempted to
**    have any optimization (does a per texel copy on the cpu)
*/
void _jsGcmDeswizzleDataCPU
(
    const char*swizzledTexture,
    char      *texture,
    GLuint               srcx0,
    GLuint               srcy0,
    GLuint               srcz0,
    GLuint               srcWidth,
    GLuint               srcHeight,
    GLuint               srcDepth,
    GLuint               dstx0,
    GLuint               dsty0,
    GLuint               dstz0,
    GLuint               dstx1,
    GLuint               dsty1,
    GLuint               dstz1,
    GLuint               dstWidth,
    GLuint               dstHeight,
    GLuint               dstDepth,
    GLuint               srcLog2cdepth,
    GLuint               dstColordepth,
    GLuint               border,
    GLuint               dim,
    void( *copyTexel )( void *src, const void *dst )
)
{
// handle semi-fat/fat as a wider 32-bit texture
    switch ( srcLog2cdepth )
    {
        case 0:
        case 1:
        case 2:
            break;
        case 3:
            srcWidth <<= 1;
            srcx0 <<= 1;
            dstx0 <<= 1;
            dstx1 <<= 1;
            dstWidth <<= 1;
            srcLog2cdepth -= 1;
            dstColordepth >>= 1;
            break;
        case 4:
            srcWidth <<= 2;
            srcx0 <<= 2;
            dstx0 <<= 2;
            dstx1 <<= 2;
            dstWidth <<= 2;
            srcLog2cdepth -= 2;
            dstColordepth >>= 2;
            break;
        default:
            JS_ASSERT( 0 );
            break;
    }

    GLuint srcLog2Width  = JSGCM_FLOOR_LOG2( srcWidth );
    GLuint srcLog2Height = JSGCM_FLOOR_LOG2( srcHeight );
    GLuint srcLog2Depth  = JSGCM_FLOOR_LOG2( srcDepth );
    GLuint texel = 0;
    GLuint srcU, srcV, srcP;
    GLuint dstU, dstV, dstP;

    // If we are doing a sub-texture update, we need to offset the
    // destination by 3 pixels.
    if ( border )
    {
        if ( dstx0 != 0 )
        {
            srcx0 += 3;
        }

        if ( dsty0 != 0 )
        {
            srcy0 += 3;
        }

        if ( dim > 2 )
        {
            srcz0 += 3;
        }
    }

    dstP = dstz0;
    srcP = srcz0;

    while ( dstP < dstz1 )
    {
        dstV = dsty0;
        srcV = srcy0;

        while ( dstV < dsty1 )
        {
            dstU = dstx0;
            srcU = srcx0;

            while ( dstU < dstx1 )
            {
                GLuint newTexel = _jsGcmCalcSwizzledAddress( srcU, srcV, srcP,
                                  srcLog2Depth,
                                  srcLog2Height,
                                  srcLog2Width ) << srcLog2cdepth;

                texel = (( dstP * dstWidth * dstHeight ) +
                         ( dstV * dstWidth ) +
                         ( dstU ) ) * dstColordepth;

                copyTexel( &texture[texel], &swizzledTexture[newTexel] );

                // Increment the dst location if:
                //
                // - We aren't dealing with a bordered texture
                // - We are a bordered texture, but we have replicated the border to be a
                //   four pixel border ( hardware feature )
                //
                if ( border &&
                        ((( dstU == 0 ) && ( srcU < 3 ) ) ||
                         (( dstU == ( dstWidth - 1 ) ) && ( srcU < ( dstWidth + 6 ) ) ) ) )
                {}
                else
                {
                    dstU++;
                }

                srcU++;
            }

            // See comment above
            if ( !( border &&
                    ((( dstV == 0 ) && ( srcV < 3 ) ) ||
                     (( dstV == ( dstHeight - 1 ) ) && ( srcV < ( dstHeight + 6 ) ) ) ) ) )
            {
                dstV++;
            }

            srcV++;
        }

        dstP++;
        srcP++;
    }
}

// various API to deal with performance counters.
void jsResetCounters()
{
#define COUNTER(Group,Name,Text) _JS_PERFORMANCE_RESET(Name);
    COUNTER_LIST
#undef COUNTER
}


void _jsBeginGPUWaitFifo()
{
    _JS_PERFORMANCE_START( GPUWaitFifoCounter );
}

void _jsEndGPUWaitFifo()
{
    _JS_PERFORMANCE_STOP( GPUWaitFifoCounter );
}

void _jsBeginGPUFinish()
{
    _JS_PERFORMANCE_START( GPUFinishCounter );
}

void _jsEndGPUFinish()
{
    _JS_PERFORMANCE_STOP( GPUFinishCounter );
}

void _jsBeginGPUFinishFence()
{
    _JS_PERFORMANCE_START( GPUFinishFenceCounter );
}

void _jsEndGPUFinishFence()
{
    _JS_PERFORMANCE_STOP( GPUFinishFenceCounter );
}

void _jsBeginGPUTransferFifo()
{
    _JS_PERFORMANCE_START( GPUTransferFifoCounter );
}

void _jsEndGPUTransferFifo()
{
    _JS_PERFORMANCE_STOP( GPUTransferFifoCounter );
}


#ifdef JS_DPM

///////////////////////////////////////////////////////////////////////////////////////////

int _jsSavePlatformBufferObjectAndUpdate( jsBufferObject * bufferObject, void * platformBufferObject, void * saveArea, unsigned long int * offset, unsigned long int offsetMax )
{
    if ( platformBufferObject == NULL )	return 0;
    JS_ASSERT( saveArea != NULL );
    JS_ASSERT( *offset >= 0 );
    JS_ASSERT( *offset < offsetMax );
    if ( *offset + sizeof( JSnvBufferObject ) > offsetMax )	return -1;	// out of storage
    JSnvBufferObject * nvBufferObject = ( JSnvBufferObject * ) platformBufferObject;
    memcpy(( char * )saveArea + *offset, nvBufferObject, sizeof( JSnvBufferObject ) );
    *offset += sizeof( JSnvBufferObject );
    if ( nvBufferObject->buffer != NULL )
    {
        if ( *offset + nvBufferObject->bufferSize > offsetMax )	return -1;
        fast_memcpy(( char * )saveArea + *offset, _JSGCM_MAP_GPU_ADDRESS( nvBufferObject->buffer ), bufferObject->size );
        *offset += bufferObject->size;
    }
    return 0;
}


///////////////////////////////////////////////////////////////////////////////////////////

int _jsRestorePlatformBufferObjectAndUpdate( jsBufferObject * bufferObject, void * platformBufferObject, jsBufferObject * jsbo, void * saveArea, unsigned long int * offset )
{
    if ( platformBufferObject == NULL )	return 0;
    JS_ASSERT( saveArea != NULL );
    JS_ASSERT( *offset >= 0 );
    JSnvBufferObject * nvBufferObject = ( JSnvBufferObject * ) platformBufferObject;
    memcpy( nvBufferObject, ( char * )saveArea + *offset, sizeof( JSnvBufferObject ) );
    *offset += sizeof( JSnvBufferObject );
    if ( nvBufferObject->buffer != NULL )
    {
        nvBufferObject->buffer = _jsGcmAllocateMemory( nvBufferObject->bufferSize );
        fast_memcpy( _JSGCM_MAP_GPU_ADDRESS( nvBufferObject->buffer ), ( char * )saveArea + *offset, bufferObject->size );
        *offset += bufferObject->size;
    }

    // mapAddress, bounceEntry
    nvBufferObject->mapCount = 0;
    if ( nvBufferObject->mapAccess != GL_NONE )
        _jsPlatformBufferObjectMap( bufferObject, nvBufferObject->mapAccess );
    fast_memcpy( _JSGCM_MAP_GPU_ADDRESS( nvBufferObject->buffer ), ( char * )saveArea + *offset, bufferObject->size );

    return 0;
}


///////////////////////////////////////////////////////////////////////////////////////////

int _jsSavePlatformFenceObjectAndUpdate( void * platformFenceObject, void * saveArea, unsigned long int * offset, unsigned long int offsetMax )
{
    if ( platformFenceObject == NULL )	return 0;
    JS_ASSERT( saveArea != NULL );
    JS_ASSERT( *offset >= 0 );
    JS_ASSERT( *offset < offsetMax );
    //JSnvFenceObject * nvFenceObject = (JSnvFenceObject *) platformFenceObject;
    // to do - save nvFenceObject->fence
    return 0;
}


///////////////////////////////////////////////////////////////////////////////////////////

int _jsRestorePlatformFenceObjectAndUpdate( void * platformFenceObject, void * saveArea, unsigned long int * offset )
{
    if ( platformFenceObject == NULL )	return 0;
    JS_ASSERT( saveArea != NULL );
    JS_ASSERT( *offset >= 0 );
    //JSnvFenceObject * nvFenceObject = (JSnvFenceObject *) platformFenceObject;
    // to do - restore nvFenceObject->fence
    return 0;
}


///////////////////////////////////////////////////////////////////////////////////////////

int _jsSavePlatformEventObjectAndUpdate( void * platformEventObject, void * saveArea, unsigned long int * offset, unsigned long int offsetMax )
{
    if ( platformEventObject == NULL )	return 0;
    JS_ASSERT( saveArea != NULL );
    JS_ASSERT( *offset >= 0 );
    JS_ASSERT( *offset < offsetMax );
    if ( *offset + sizeof( JSnvEventObject ) > offsetMax )	return -1;	// out of storage
    JSnvEventObject * nvEventObject = ( JSnvEventObject * ) platformEventObject;
    memcpy(( char * )saveArea + *offset, nvEventObject, sizeof( JSnvEventObject ) );
    *offset += sizeof( JSnvEventObject );
    return 0;
}


///////////////////////////////////////////////////////////////////////////////////////////

int _jsRestorePlatformEventObjectAndUpdate( void * platformEventObject, void * saveArea, unsigned long int * offset )
{
    if ( platformEventObject == NULL )	return 0;
    JS_ASSERT( saveArea != NULL );
    JS_ASSERT( *offset >= 0 );
    JSnvEventObject * nvEventObject = ( JSnvEventObject * ) platformEventObject;
    memcpy( nvEventObject, ( char * )saveArea + *offset, sizeof( JSnvEventObject ) );
    *offset += sizeof( JSnvEventObject );
    return 0;
}


///////////////////////////////////////////////////////////////////////////////////////////

int _jsSavePlatformMemoryManagerAndUpdate( void * MM, void * saveArea, unsigned long int * offset, unsigned long int offsetMax )
{
    if ( MM == NULL )	return 0;
    JS_ASSERT( saveArea != NULL );
    JS_ASSERT( *offset >= 0 );
    JS_ASSERT( *offset < offsetMax );
    MemoryBlockManager_t *	mm = ( MemoryBlockManager_t * ) MM;
    int	bookSize = sizeof( int ) * mm->bookSize;
    int	totalSize = sizeof( *mm ) + bookSize;
    if ( *offset + totalSize > offsetMax )	return -1;	// out of storage
    memcpy(( char * )saveArea + *offset, mm, sizeof( *mm ) );
    *offset += sizeof( *mm );
    if ( bookSize )
    {
        memcpy(( char * )saveArea + *offset, mm->book, bookSize );
        *offset += bookSize;
    }
    return 0;
}


///////////////////////////////////////////////////////////////////////////////////////////

int _jsRestorePlatformMemoryManagerAndUpdate( void * MM, void * saveArea, unsigned long int * offset )
{
    if ( MM == NULL )	return 0;
    JS_ASSERT( saveArea != NULL );
    JS_ASSERT( *offset >= 0 );
    MemoryBlockManager_t *	mm = ( MemoryBlockManager_t * ) MM;
    memcpy( mm, ( char * )saveArea + *offset, sizeof( *mm ) );
    *offset += sizeof( *mm );
    int	bookSize = sizeof( int ) * mm->bookSize;
    if ( bookSize )
    {
        mm->book = ( unsigned int * ) jsMalloc( bookSize );
        JS_ASSERT( mm->book );
        memcpy( mm->book, ( char * )saveArea + *offset, bookSize );
        *offset += bookSize;
    }
    return 0;
}


///////////////////////////////////////////////////////////////////////////////////////////

int _jsSavePlatformDataAndUpdate( void * saveArea, unsigned long int * offset, unsigned long int offsetMax )
{
    return _jsGcmAllocSave( _jsGcmAllocator, saveArea, offset, offsetMax );
}


///////////////////////////////////////////////////////////////////////////////////////////

int _jsRestorePlatformDataAndUpdate( void * saveArea, unsigned long int * offset )
{
    return _jsGcmAllocRestore( _jsGcmAllocator, saveArea, offset );
}

///////////////////////////////////////////////////////////////////////////////////////////

int _jsSavePlatformFramebufferAndUpdate( void * framebuffer, void * saveArea, unsigned long int * offset, unsigned long int offsetMax )
{
    JS_ASSERT( framebuffer != NULL );
    JS_ASSERT( saveArea != NULL );
    JS_ASSERT( *offset >= 0 );
    JS_ASSERT( *offset < offsetMax );
    int size = sizeof( JSplatformFramebuffer );
    if ( *offset + size > offsetMax )	return -1;
    memcpy(( char * )saveArea + *offset, ( char * )framebuffer, size );
    *offset += size;
    return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////

int _jsRestorePlatformFramebufferAndUpdate( void * framebuffer, void * saveArea, unsigned long int * offset )
{
    JS_ASSERT( framebuffer != NULL );
    JS_ASSERT( saveArea != NULL );
    JS_ASSERT( *offset >= 0 );
    int size = sizeof( JSplatformFramebuffer );
    memcpy(( char * )framebuffer, ( char * )saveArea + *offset, size );
    *offset += size;
    return 0;
}


#endif // JS_DPM
