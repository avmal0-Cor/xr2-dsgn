/* Copyright (C)2001-2006 by NVIDIA Corporation.  All rights reserved.
 * Modified by Sony Computer Entertainment Inc under license from NVIDIA Corporation.
 * Copyright (C) 2009 Sony Computer Entertainment Inc.
 * 
 * SCEI CONFIDENTIAL
 */


/*
 * Function definitions for FIFO buffer management
 */

#include <JSGCM/JSGcmUtils.h>
#include <JSGCM/JSGcmFifo.h>
#include <JSGCM/JSGcmDriver.h>
#ifdef GCM_HUD
#include <cell/gcm_hud.h>
#endif

#include <cell/gcm.h>
#include <cell/gcm/gcm_method_data.h>
using namespace cell::Gcm;

#include <ppu_intrinsics.h>
const uint32_t c_rounded_size_of_jsDrawParams = (sizeof(jsDrawParams)+0x7f)&~0x7f;
static uint8_t s_dparams_buff[ c_rounded_size_of_jsDrawParams ] __attribute__((aligned(128)));


//#define HOLE_DEBUG_PRINTF(...) printf(__VA_ARGS__)
#define HOLE_DEBUG_PRINTF(...)

// Send the commands in the FIFO to the gpu, and starts running the commands
// FIFO must be complete before this call is made.
// Function will not wait for completion of the commands before returning.
// _jsGcmFifoFinish() can be used to wait for the completion of the operations.
void _jsGcmFifoFlush( jsGcmFifo *fifo )
{
    // determine new put value (alias address gpu will stop)
    unsigned int offsetInBytes = 0;
    cellGcmAddressToOffset( fifo->current, ( uint32_t * )&offsetInBytes );

	// **IMPORTANT** invalidate cpu write combine caches
    // -- MUST guarantee all write to fifo have been completed
    //    (i.e. write combined agp memory)
    cellGcmFlush();

    // just write put
#ifdef GCM_HUD
    cellGcmHUDSetPutRegister(offsetInBytes);
#else
    fifo->dmaControl->Put = offsetInBytes;
#endif
    fifo->lastPutWritten = fifo->current;

	// track which refCount have been flushed out
    fifo->lastSWReferenceFlushed = fifo->lastSWReferenceWritten;
}

// Adds a gcm marker support 
void _jsGcmFifoPushMarker( const GLubyte* szName )
{
	GCM_FUNC_SAFE( cellGcmSetPerfMonPushMarker, (const char*)szName ); 
}

void _jsGcmFifoPopMarker()
{
	GCM_FUNC_SAFE_NO_ARGS( cellGcmSetPerfMonPopMarker ); 
}



// Adds a reference marker into the command buffer.
GLuint _jsGcmFifoPutReference( jsGcmFifo *fifo )
{
    // increment the reference count to be written
    fifo->lastSWReferenceWritten++;

    GCM_FUNC( cellGcmSetReferenceCommand, fifo->lastSWReferenceWritten );

    // wraparound ?
    if (( fifo->lastSWReferenceWritten & 0x7fffffff ) == 0 )
    {
        // wait for gpu to complete, so any existing references will be retired
        _jsGcmFifoFinish( fifo );
    }

    return fifo->lastSWReferenceWritten;
}

// Determine the last reference marker passed by the gpu
GLuint _jsGcmFifoReadReference( jsGcmFifo *fifo )
{
    GLuint ref = *(( volatile GLuint * ) & fifo->dmaControl->Reference );
    fifo->lastHWReferenceRead = ref;
    return ref;
}

// Determine if the given reference marker has been passed by the gpu
GLboolean _jsGcmFifoReferenceInUse( jsGcmFifo *fifo, GLuint reference )
{
    // compare against cached hw ref value (accounting wrap)
    if ( !(( fifo->lastHWReferenceRead - reference ) & 0x80000000 ) )
    {
        return GL_FALSE;
    }

    // has the reference already been flushed out ?
    if (( fifo->lastSWReferenceFlushed - reference ) & 0x80000000 )
    {
        _jsGcmFifoFlush( fifo );
    }

    // read current hw reference
    _jsGcmFifoReadReference( fifo );

    // compare against hw ref value (accounting wrap)
    if ( !(( fifo->lastHWReferenceRead - reference ) & 0x80000000 ) )
    {
        return GL_FALSE;
    }

    return GL_TRUE;
}

// Wait for all the calls in the current FIFO to be finished by the gpu
void _jsGcmFifoFinish( jsGcmFifo *fifo )
{
    _jsBeginGPUFinish();
    // put a marker to wait on
    GLuint ref = _jsGcmFifoPutReference( fifo );

    // kick off chip
    _jsGcmFifoFlush( fifo );

    // wait for completion
    for ( ;; )
    {
        // gpu passed reference ?
        if ( !_jsGcmFifoReferenceInUse( fifo, ref ) )
        {
            // ref should be the last reference in the fifo.
            JS_ASSERT( ref == fifo->lastHWReferenceRead );
            break;
        }

        // avoid polling on bus (interrupts memory traffic)
        _jsGcmFifoDelayRegisterRead( fifo );
    }
    _jsEndGPUFinish();
}

// Wait until the requested space is available.
// If not currently available, will call the out of space callback
uint32_t * _jsGcmFifoWaitForFreeSpace( jsGcmFifo *fifo, GLuint spaceInWords )
{
    if ( fifo->current + spaceInWords + 1024 > fifo->end )
    {
        _jsOutOfSpaceCallback( fifo, spaceInWords );
    }

    return _jsGcmState.fifo.current;
}

// Initialize the specified Fifo, and make it the current gcm context
void _jsGcmFifoInit( jsGcmFifo *fifo, void *dmaControl, unsigned long dmaPushBufferOffset, jsFifoData *dmaPushBuffer,
                     GLuint dmaPushBufferSize )
{
    // make sure we have enough room for at least 2 blocks in the fifo
    JS_ASSERT( dmaPushBufferSize >= DEFAULT_FIFO_BLOCK_SIZE *2 );

    // init fifoBlockSize
    fifo->fifoBlockSize = DEFAULT_FIFO_BLOCK_SIZE;

    // init fifo context pointers to first fifo block which will be set at the the dmaPushPuffer position
    fifo->begin     = ( jsFifoData* ) dmaPushBuffer;
    fifo->end       = fifo->begin + ( fifo->fifoBlockSize / sizeof( uint32_t ) ) - 1;
    // init rest of context
    fifo->current        = fifo->begin;
    fifo->lastGetRead    = fifo->current;
    fifo->lastPutWritten = fifo->current;

    // store fifo values
    fifo->dmaPushBufferBegin       = dmaPushBuffer;
    fifo->dmaPushBufferEnd         = ( jsFifoData* )(( size_t )dmaPushBuffer + dmaPushBufferSize ) - 1;
    fifo->dmaControl               = ( jsGcmControlDma* )dmaControl;
    fifo->dmaPushBufferOffset      = dmaPushBufferOffset;
    fifo->dmaPushBufferSizeInWords = dmaPushBufferSize / sizeof( jsFifoData );

    fifo->lastHWReferenceRead    = 0;
    fifo->lastSWReferenceWritten = 0;
    fifo->lastSWReferenceFlushed = 0;
	
    // note that jsGcmFifo is-a CellGcmContextData
    gCellGcmCurrentContext = fifo;
    // setting our own out of space callback here to handle our fifo
    gCellGcmCurrentContext->callback = ( CellGcmContextCallback )_jsOutOfSpaceCallback;

    // ensure the ref is initted to 0.
    if ( _jsGcmFifoReadReference( fifo ) != 0 )
    {
        GCM_FUNC( cellGcmSetReferenceCommand, 0 );
        _jsGcmFifoFlush( fifo ); // Here, we jump to this new buffer

        // a finish that waits for 0 specifically.
        for ( ;; )
        {
            if ( _jsGcmFifoReadReference( fifo ) == 0 ) break;
            _jsGcmFifoDelayRegisterRead( fifo );
        }
    }
    fifo->dmaPushBufferGPU = dmaPushBuffer;
    fifo->spuid = 0;

}

void * psglEndCommandRecordNoReturn()
{
	if ( _jsGcmState.recordFifoMode )
	{
		// make sure any previous work has been validated to the fifo before exiting 
		glFlush();
		// turn off record mode 
		_jsGcmState.recordFifoMode = GL_FALSE;

		// get the current before we reset it 
		void * current = (void *)_jsGcmState.fifo.current;	

		// reset the prev command buffer begin end and current; 
		_jsGcmState.fifo.begin		= _jsGcmState.recordPrevBegin;
		_jsGcmState.fifo.end		= _jsGcmState.recordPrevEnd;
		_jsGcmState.fifo.current	= _jsGcmState.recordPrevCurrent;
	
		return current; 
	}

	// if not recording this means nothing.   
	return NULL; 
};

// utility function to set nop at the current location in a command buffer
static inline void _jsSetJumpToSelf(CellGcmContextData *ctx)
{
	uint32_t jumpOffset;
	cellGcmAddressToOffset(ctx->current, &jumpOffset);

	HOLE_DEBUG_PRINTF("%x --- JTS with offset %d \n", (uint32_t)ctx->current, jumpOffset);

	cellGcmSetJumpCommandUnsafeInline(ctx, jumpOffset);
}

/** @addtogroup PSGL
 *
 * @{
 */

/**
 *  @short Redirects fifo commands from the GCM command buffer to the user-defined command buffer.  
 *
 *  psglBeginCommandRecord() is used as follows in order to allocate, compile, and execute 
 *  psgl generated RSX&trade; command buffers that can be re-used to considerably improve efficiency on the PPU: 
 *  
 *  1.	Allocate and Map Command Buffer memory:
 @code
      psglAllocateCommandBuffer( commandBufferSize, commandBufferOffset );
 @endcode
 *  
 *  2.	Compile PSGL Commands to:
 @code
     psglBeginCommandRecord( gMaterial1, gCompBufferSize);
     PSGL COMMANDS (
     psglEndCommandRecordWithReturn();
 @endcode 
 *  
 *  3.	Set or Call compiled command buffers:
 @code
     psglPushCommandBuffer( gDrawCube, gCompCubeSize );
 @endcode
 *  
 *      or 
@code
     psglCallCommandBuffer( gDrawCube );
@endcode
 *  
 *  Samples can be found in the sdk at ($SDK_DIR)/samples/sdk/graphics/psgl/Advanced/PreCompbuffer_01 and _02.  
 *
 *  @param commandBuffer The pointer to the user command buffer that PSGL redirects 
 *  the command buffer to.  This buffer must be mapped so that the RSX&trade; can access it using <CODE>cellGcmMapMemory</CODE>.  
 *  The use of <CODE>psglAllocateCommandBuffer()</CODE>, which allocates and calls <CODE>cellGcmMapMemory</CODE>, should be 
 *  used to allocate this buffer to remain TRC-compliant for PSGL lib users.
 *  
 *  @param sizeInBytes Specifies the size of the <CODE><I>commandBuffer</I></CODE> memory in bytes.
 *
 *  @par Errors
<TABLE>
<TR>
<TD><CODE>GL_TRUE</CODE></TD>
<TD>Returned if <CODE><I>commandBuffer</I></CODE> memory passed in is mapped, and PSGL has been added that records 
mode compiling commands. </TD>
</TR>
<TR>
<TD><CODE>GL_FALSE</CODE></TD>
<TD>Returned if <CODE><I>commandBuffer</I></CODE> memory passed in is not mapped and is invisible to the RSX&trade;. </TD>
</TR></TABLE>
 *
 *  @sa psglGetCommandRecordCurrent, psglEndCommandRecord, psglCallCommandBuffer,
 *  psglPushCommandBuffer, psglAllocateCommandBuffer, psglFreeCommandBuffer, psglSetStallCallback
 */

// [RSTENSON] PSGL interfaces for recording and playing back fifo commands 
void psglBeginCommandRecord( void * commandBuffer, GLuint sizeInBytes )
{	 
	// make sure any previous work has been validated to the fifo before exiting 
	glFlush();

	GLuint sizeInWords = sizeInBytes/4; 

#ifdef JS_DEBUG	
	// if this fails the address passed in wasn't mapped 
	// but just calling this just to make sure 
	uint32_t commandBufferOffset = 0;
	JS_ASSERT( cellGcmAddressToOffset( (uint32_t *)commandBuffer, &commandBufferOffset) == CELL_OK );
#endif 

	// init
	_jsGcmState.recordFifoMode = GL_TRUE;

	_jsGcmState.recordFifoSizeInWords = sizeInWords; 

	// store the previous fifo begin, end and current 
	_jsGcmState.recordPrevBegin		= _jsGcmState.fifo.begin; 
	_jsGcmState.recordPrevEnd		= _jsGcmState.fifo.end; 
	_jsGcmState.recordPrevCurrent	= _jsGcmState.fifo.current; 

	// now redirect to new commandBufferer 
	_jsGcmState.fifo.begin		= (GLuint *)commandBuffer;
	_jsGcmState.fifo.end		= (GLuint *)commandBuffer + sizeInWords; 
	_jsGcmState.fifo.current	= (GLuint *)commandBuffer;

	GLuint nopsAtBegin = 8; 
	// need to add some nops here at the beginning for a issue with the get and the put being at the 
	// same position when the fifo is in GPU memory. 
	for ( GLuint i = 0; i < nopsAtBegin; i++ )
	{
		_jsGcmState.fifo.current[0] = JSGCM_NOP();
		_jsGcmState.fifo.current++;
	}

    // pin allocation in static command buffer
    gmmPinAllocations();

	//printf(" fifo b %p e %p c %p \n", _jsGcmState.fifo.begin, 
	//	_jsGcmState.fifo.end, _jsGcmState.fifo.current ); 
};

/**
 *  @short Ends redirect of fifo commands from the PSGL active GCM command buffer to the user defined command buffer for later use.  
 *
 *  <CODE>psglEndCommandRecord()</CODE> used to end PSGL&rsquo;s redirection of the RSX&trade; commands from the user fifo back to PSGL&rsquo;s internal fifo.  
 *  It returns the current position in the user command buffer.   Also this function can be set to add a return to the current 
 *  position then return if the fifo is to be use in a <CODE>psglCallCommandBuffer</CODE> method later or set to have no 
 *  return set when to be used with <CODE>psglPushCommandBuffer</CODE> instead which simply copies in the fifo in to the 
 *  active command buffer and therefore does not require a return.
 *  
 *  Samples can be found in the sdk at <CODE>($SDK_DIR)/samples/sdk/graphics/psgl/Advanced/PreCompbuffer_01 and _02</CODE>.
 * 
 *  @param AddReturn Type: Bool.  If True, a return is added to the command buffer before returning and 
 *  ending command record mode.  Use as true if you plan to call the command buffer later as a 
 *  <CODE>psglCallCommandBuffer</CODE> which will set a call to the user passed in buffer and then the 
 *  return will be there to automatically return to the current fifo position.  Please note calls are 
 *  expensive to be processed by the RSX&trade; and should be used on large stream.  Try and use 
 *  <CODE>psglPushCommandBuffer</CODE> and therefore <CODE>psglEndCommandRecord(PSGL_DO_NOT_ADD_RETURN)</CODE> whenever 
 *  there is a high frequency number of command buffers to be set in the command stream.
 *  
 *  @return Returns <CODE>void *</CODE> pointer to current fifo location in the user command buffer 
 *  so that the user can then start another command buffer at the next location.  See <CODE>StaticCommandBuffer01</CODE> 
 *  and <CODE>02</CODE> for further examples of this in the SDK. 
 *  
 *  @par Errors
 *  Returns <CODE>NULL</CODE> if called while not in record mode. Also returns an error if the PSGL runs out of 
 *  space in the user command buffer specified by <CODE>psglBeginCommandRecord</CODE> 
 *  while recording. If set, the <CODE>psglStaticCommandBufferCallBack</CODE> is called, allowing the user to call the 
 *  appropriate <CODE>psglEndCommandRecord()</CODE> with appropriate parameters, and then set <code>psglBeginCommandRecord</CODE> 
 *  on a different memory block to record the entire fifo stream for later use.   
 *
 *  @sa psglGetCommandRecordCurrent, psglBeginCommandRecord, psglCallCommandBuffer,
 *  psglPushCommandBuffer, psglAllocateCommandBuffer, psglFreeCommandBuffer, psglSetStallCallback
 */
void * psglEndCommandRecord( bool AddReturn )
{
    // unpin allocation in static command buffer
    gmmUnpinAllocations();

	if ( _jsGcmState.recordFifoMode )
	{
		// make sure any previous work has been validated to the fifo before exiting 
		glFlush();
		
		if ( AddReturn )
		{
			#ifdef JS_DEBUG	
				uint32_t commandBufferOffset = 0;
				// if this fails the address passed in wasn't mapped 
				// but just calling this just to make sure 
				JS_ASSERT( cellGcmAddressToOffset( (uint32_t *)_jsGcmState.fifo.current, &commandBufferOffset) == CELL_OK );
			#endif 

			// the user will set this if they are intending to use this buffer
			// with the call command vs the Set Command interface. 
			// set return command at the end of the buffer	
			GCM_FUNC_NO_ARGS( cellGcmSetReturnCommand);
		}

		// turn off record mode 
		_jsGcmState.recordFifoMode = GL_FALSE;

		// get the current before we reset it 
		void * current = (void *)_jsGcmState.fifo.current;	

		// reset the prev command buffer begin end and current; 
		_jsGcmState.fifo.begin		= _jsGcmState.recordPrevBegin;
		_jsGcmState.fifo.end		= _jsGcmState.recordPrevEnd;
		_jsGcmState.fifo.current	= _jsGcmState.recordPrevCurrent;
	
		return current; 
	}

	// if not recording this means nothing.   
	return NULL; 
};



/**
 *  @short Returns the current position in the user command buffer.  
 *
 *  <CODE>psglGetCommandRecordCurrent()</CODE> is used between <CODE>psglBeginCommandRecord</CODE> and 
 *  <CODE>psglEndCommandRecord</CODE> calls.  It returns the current position in the user command buffer.   
 *  The user can then use that return value to determine whether they are running out of space in their fifo.   
 *  
 *  Steps for using pre-compiled command buffer functions:
 *  
 *  1.	Allocate and Map Command Buffer memory:
 @code

       psglAllocateCommandBuffer( commandBufferSize, commandBufferOffset );

 @endcode

 *  2.	Compile PSGL Commands to 
 @code

       psglBeginCommandRecord( gMaterial1, gCompBufferSize);

            PSGL COMMANDS (

@endcode
       If planning to call this buffer with <CODE>psglCallCommandBuffer</CODE> later, use <CODE>PSGL_ADD_RETURN</CODE>:
@code
	   
	             psglEndCommandRecord(PSGL_ADD_RETURN);

@endcode

*  Or, if planning to call this buffer with <CODE>psglPushCommandBuffer</CODE> later, use <CODE>PSGL_DO_NOT_ADD_RETURN</CODE>:
@code
	   
	             psglEndCommandRecord(PSGL_DO_NOT_ADD_RETURN);

@endcode
*  3.	Set or Call compiled command buffers
@code

       psglPushCommandBuffer( gDrawCube, gCompCubeSize );

@endcode
*  or 
@code

       psglCallCommandBuffer( gDrawCube );

@endcode
 * 
 *  Samples can be found in the sdk at <CODE>($SDK_DIR)/samples/sdk/graphics/psgl/Advanced/PreCompbuffer_01</CODE> and <CODE>_02</CODE>.    
 *
 *  @return Returns the current command buffer position.
 *  
 *  @par Errors
 *  Returns <CODE>NULL</CODE> if called while not in record mode.
 *
 *  @sa psglBeginCommandRecord, psglEndCommandRecord, psglCallCommandBuffer,
 *  psglPushCommandBuffer, psglAllocateCommandBuffer, psglFreeCommandBuffer, psglSetStallCallback
 */

// returns the current command buffer position 
void * psglGetCommandRecordCurrent()
{
	// if not recording return NULL
	if ( !_jsGcmState.recordFifoMode )
	{
		printf(" This must becalled between psglBeginCommandRecord and psglEndCommandRecord\n" ); 	
		return NULL;
	}

	// return the current fifo position that when recording is actually is in the user buffer 
	return (void *)_jsGcmState.fifo.current; 	
}; 

/**
 *  @short Sets a call to the user command buffer.  
 *
 *  <CODE>psglCallCommandBuffer()</CODE> sets a call to the user command buffer and assumes there was a return set at 
 *  the end of the user command buffer by either <CODE>psglEndCommandRecord ( PSGL_ADD_RETURN )</CODE>, or the user has 
 *  set a return in their fifo manually with GCM.  
 *  
 *  Warning: If there is no return set in the user command buffer when calling this function, the RSX&trade; will crash! 
 *  
 *  Samples can be found in the sdk at <CODE>($SDK_DIR)/samples/sdk/graphics/psgl/Advanced/PreCompbuffer_01 and _02</CODE>.
 *
 *  @param commandBuffer void * Pointer to user command buffer allocated and mapped by <CODE>psglAllocateCommandBuffer()</CODE>.  
 *  
 *  @return Returns void * pointer which is the current position in the fifo after the call to the user&rsquo;s command buffer is made.
 *  
 *  @sa psglGetCommandRecordCurrent, psglBeginCommandRecord, psglEndCommandRecord, 
 *  psglPushCommandBuffer, psglAllocateCommandBuffer, psglFreeCommandBuffer, psglSetStallCallback
 */

// set a call to the command buffer.  The previous end command placed the return so we're good ;)
void * psglCallCommandBuffer( void * commandBuffer )
{
	// set call to recorded fifo
	uint32_t buffOffset = 0; 
	void * current = _jsGcmState.fifo.current;

	// if this fails the address passed in wasn't mapped 
	// but just calling this just to make sure 
	if ( cellGcmAddressToOffset( (uint32_t *)commandBuffer, &buffOffset) != CELL_OK )
	{
		printf(" psglCommandBuffer Error: Buffer pointer was not mapped\n" ); 
		printf(" Aborting CallCommandBuffer \n" ); 
		
		// Abort safely even in opt 
		return current; 
	}

	// set return command at the end of the buffer	
	GCM_FUNC( cellGcmSetCallCommand, buffOffset );

	// I know what your thinking, where is the return.
	// The return was pushed in the fifoBuffer by psglEndCommandBuffer all is well in fifo land. 

	// put a marker to wait on
    _jsGcmFifoPutReference( &_jsGcmState.fifo );

	return current; 
};

/**
 *  @short Copies the user command buffer into the Fifo command buffer.  
 *
 *  <CODE>psglPushCommandBuffer()</CODE> copies the user 
 *  command buffer into the active PSGL Fifo command buffer.  This can be a 
 *  buffer being recorded, or the command buffer fifo actively consumed by the RSX&trade;.  
 *  The method is more efficient than <CODE>psglCallCommandBuffer</CODE> for high 
 *  frequency setting of command buffers in the PSGL fifo, because of the RSX&trade;&rsquo;s overhead 
 *  for processing calls/returns. 
 *  
 *  Samples can be found in the sdk at <CODE>($SDK_DIR)/samples/sdk/graphics/psgl/Advanced/PreCompbuffer_01 and _02</CODE>.
 *
 *  @param commandBuffer void * Pointer to user command buffer allocated and mapped by <CODE>psglAllocateCommandBuffer</CODE>.  
 *  
 *  @param sizeInBytes Specifies the size of the <CODE>commandBuffer</CODE> memory in bytes.
 *  
 *  @sa psglGetCommandRecordCurrent, psglBeginCommandRecord, psglEndCommandRecord, 
 *  psglCallCommandBuffer, psglAllocateCommandBuffer, psglFreeCommandBuffer, psglSetStallCallback
 */

// copy in command buffer to avoid the jump or call/return commands.
// Yes the memcopy of the fifo is actually faster 

// set a call to the command buffer.  The previous end command placed the return so we're good ;)
void psglPushCommandBuffer( void * commandBuffer, GLuint sizeInBytes )
{

	// wait for space for the command buffer 
	_jsGcmFifoWaitForFreeSpace( &_jsGcmState.fifo, sizeInBytes); 

	// copy the fifo in to our command buffer 
	memcpy( _jsGcmState.fifo.current, commandBuffer, sizeInBytes ); 
	_jsGcmState.fifo.current += sizeInBytes / 4; 

	// put a marker to wait on
    //_jsGcmFifoPutReference( &_jsGcmState.fifo );	

	//_jsGcmFifoGlInvalidateVertexCache();
    //_jsGcmFifoFlush( &_jsGcmState.fifo );	
};

/**
 *  @short Allocates and maps aligned XDR memory.  
 *
 *  <CODE>psglAllocateCommandBuffer()</CODE> allocates 1 mb aligned XDR memory, then maps it so that the 
 *  RSX&trade; can access it.  Note that this must be at least 1 megabyte in size, and is allocated in 
 *  1-megabyte increments due to RSX&trade; mapping requirements.  For example, if 1024*1024+1 is passed in,
 *  2 megabytes is allocated to support the alignment and allocation size.  
 *  
 *  Samples can be found in the sdk at <CODE>($SDK_DIR)/samples/sdk/graphics/psgl/Advanced/PreCompbuffer_01 and _02</CODE>.
 * 
 *
 *  @param sizeInBytes Type: Gluint. The size of the command buffer in bytes. 
 *
 *  @param offset The RSX&trade; offset of the mapped XDR memory. 
 *  
 *  @return Returns <CODE>void *</CODE> pointer to the allocated user, aligned to 1mb and RSX&trade;-mapped 
 *  memory that is now usable by the other PSGL command buffer functions.  
 *  
 *  @par Errors
 *  Returns <CODE>NULL</CODE> if memory could not be allocated or mapped.
 *  
 *  @sa psglGetCommandRecordCurrent, psglBeginCommandRecord, psglEndCommandRecord, psglCallCommandBuffer,
 *  psglPushCommandBuffer, psglFreeCommandBuffer, psglSetStallCallback
 */

// utility function that allocates 1 megabyte aligned memory and maps it so the rsx can access it
void * psglAllocateCommandBuffer( GLuint sizeInBytes, GLuint * offset )
{
	// init our static block for the fifo 
	void * commandBuffer = (GLuint *)memalign( 1024*1024, sizeInBytes );

	// over estimate the fifo size as to remain 1 megabyte aligned 
	GLuint alignedSize = ((sizeInBytes + 1023 ) >> 10) << 10; 

	// Map it so that the rsx can access it 
	if ( cellGcmMapMainMemory( commandBuffer, alignedSize, offset ) != CELL_OK )
	{		
		_JS_REPORT_EXTRA(PSGL_REPORT_HOST_MEMORY_ALLOC, "WARNING: cellGcmMapMainMemory in psglAllocateCommandBuffer Failed");		
		_JS_REPORT_EXTRA(PSGL_REPORT_HOST_MEMORY_ALLOC, "WARNING: Allocations with psglAllocateCommandBuffer must be at least 1mb");		
		offset = 0; 
		free( commandBuffer ); 
		return NULL; 
	}; 	
	
	return commandBuffer; 
};

/**
 *  @short Helper function for psglAllocateCommandBuffer.  
 *
 *  <CODE>psglFreeCommandBuffer()</CODE> is a helper function for <CODE>psglAllocateCommandBuffer</CODE>, 
 *  performing the same task as <CODE>free(commandBuffer)</CODE>.
 *  
 *  Samples can be found in the sdk at <CODE>($SDK_DIR)/samples/sdk/graphics/psgl/Advanced/PreCompbuffer_01 and _02</CODE>.
 *
 *  @param commandBuffer The buffer to be freed.  
 *  
 *  @sa psglGetCommandRecordCurrent, psglBeginCommandRecord, psglEndCommandRecord, 
 *  psglCallCommandBuffer, psglPushCommandBuffer, psglAllocateCommandBuffer, psglSetStallCallback
 */

// another utility to delete this buffer  
void psglFreeCommandBuffer( void * commandBuffer )
{
	if ( commandBuffer )
	{
		free( commandBuffer );
		commandBuffer = NULL; 
	}	
}; 

/**
 *  @short Sets function to call if the PSGL runs out of space.  
 *
 *  <CODE>psglSetRecordOutOfSpaceCallback()</CODE> sets the user-defined callback function to call if 
 *  the PSGL runs out of space in the user command buffer while recording PSGL fifo commands.  
 *  This function allows the user to call psglEndCommandRecord() or 
 *  psglAllocateCommandBuffer() if a new buffer is needed, and then call 
 *  psglBeginCommandRecord() to continue the command stream that PSGL was in the middle 
 *  of writing when it ran out of space.
 *  
 *  Samples can be found in the sdk at <CODE>($SDK_DIR)/samples/sdk/graphics/psgl/Advanced/PreCompbuffer_01 and _02</CODE>.
 *
 *  @param callBack User-defined callback function to call if the PSGL runs out of space in the user 
 *  command buffer while recording PSGL fifo commands.  
 *  
 *  @sa psglGetCommandRecordCurrent, psglBeginCommandRecord, psglEndCommandRecord, 
 *  psglCallCommandBuffer, psglPushCommandBuffer, psglAllocateCommandBuffer, psglFreeCommandBuffer, psglSetStallCallback
 */

// set the user call back function to call when we run out of space in their buffer when recording commands 
void psglSetRecordOutOfSpaceCallback( psglStaticCommandBufferCallBack callBack )
{
	_jsGcmState.recordOutOfSpaceCallback = callBack;
}; 
/**
 *  @short Sets function for PSGL to call to verify that precompiled buffers will not be overrun.  
 *
 *  <CODE>psglSetStallCallback()</CODE> sets the user-defined callback function to call for 
 *  dealing with the potential race condition when PSGL attempts to access the next block 
 *  in PSGL&rsquo;s fifo.  The PSGL fifo system must ensure that the users "called" fifo executed via 
 *  <CODE>psglCallCommandBuffer</CODE> isn&rsquo;t overrun in the next block.  To do this use
 *   <CODE>psglSetStallCallback()</CODE> to set a user defined function for managing stalls.  
 *  
 *  A sample can be found in the sdk at <CODE>($SDK_DIR)/samples/sdk/graphics/psgl/Advanced/PreCompbuffer_02</CODE>.
 * 
 *  @param callBack User-defined callback function to call if the PSGL stalls.  
 *  
 *  @sa psglGetCommandRecordCurrent, psglBeginCommandRecord, psglEndCommandRecord, 
 *  psglCallCommandBuffer, psglPushCommandBuffer, psglAllocateCommandBuffer, psglFreeCommandBuffer, 
 *  psglSetRecordOutOfSpaceCallback
 */

void psglSetStallCallback( psglStallCallBack callBack )
{
	_jsGcmState.recordStallCallback = callBack;
}


/**
 *  @short Generates a command-buffer hole.
 *
 *  This function generates a command-buffer hole of the specified size.
 *  The command-buffer hole has jump-to-self commands in the
 *  beginning of the hole and the beginning of every consecutive
 *  128-byte block.
 *
 *  @param holeSizeInWord Specifies the size in words of a
 *    command-buffer hole to generate.
 *  @param holeEA An argument into which this command stores
 *     the effective address of the generated hole.
 *  @return 0 for success, 1 for failure
 *  @sa psglDrawCommandBufferHole
 */
int psglGenerateCommandBufferHole( uint32_t holeSizeInWord, uint32_t *holeEA )
{

	HOLE_DEBUG_PRINTF("=== command buffer hole with size: %d ===\n", holeSizeInWord);

	// Make sure enough space for the hole ahead
	_jsGcmFifoWaitForFreeSpace( &_jsGcmState.fifo, holeSizeInWord + 31 ); // 31 for 128 byte alignment

	CellGcmContextData *ctx = &_jsGcmState.fifo;

	// 128 byte alignment
	while( ((uint32_t)ctx->current & 0x7F) != 0 )
		*ctx->current++ = CELL_GCM_METHOD_NOP;

	// remember current as starting address of the hole
	uint32_t *holePtr = ctx->current;
	*holeEA = (uint32_t)holePtr;

	// Make hole
	// set jump to self at the beggining of hole
	_jsSetJumpToSelf(ctx);
	// set NOP to the rest of hole, while settin JTS to beggining of every 128 byte block
	while (ctx->current < holePtr + holeSizeInWord)
	{
		if ( ((uint32_t)ctx->current & 0x7F) == 0 )
			_jsSetJumpToSelf(ctx);
		else
		{
			HOLE_DEBUG_PRINTF("%x --- NOP \n", (uint32_t)ctx->current);
			*ctx->current++ = CELL_GCM_METHOD_NOP;
		}
	}

	return 0;
}

/**
 *  @short Generates a command-buffer hole for SPU draw.
 *
 *  This function is used with psglFillCommandBufferHole() for SPU draw.
 *
 *  The function prepares for geometry drawing in a similar way to
 *  glDrawElements(). However, instead of issuing a draw command, a
 *  command-buffer hole is generated in the command buffer.
 *
 *  The command-buffer hole has jump-to-self commands in the
 *  beginning of the hole and the beginning of every consecutive
 *  128-byte block. After the hole is generated, use the SPU function
 *  psglFillCommandBufferHole() to generate a draw
 *  command on the SPU local store, and DMA transfer a draw command
 *  to fill out the hole. If SPU processing is done and
 *  the command-buffer hole is filled before RSX&trade; reaches the hole,
 *  the geometry would be drawn as if there were no hole from
 *  the beginning. However, if SPU processing has not finished,
 *  RSX&trade; stalls until the command-buffer hole is filled out.
 *
 *  Unlike glDrawElements(), this function does not support possibly
 *  slow immediate-mode indices. To use this function, a vertex
 *  buffer object for the indices must be bound with the target
 *  <c>GL_ELEMENT_ARRAY_BUFFER</c>.
 *
 *  Internally, this function uses psglValidateStates() and
 *  psglValidateAttributes() (equivalent internal functions) to
 *  prepare for draw, and uses psglGenerateCommandBufferHole()
 *  to generate a hole.
 *
 *  <b>Note:</b> This function could cause a deadlock between
 *  RSX&trade; and PPU. Because the command-buffer hole has a jump-to-self
 *  command, starting any operation to let the PPU wait for the
 *  RSX&trade; before kicking the SPU job results in deadlock, and the
 *  PPU would wait for the RSX&trade; forever. A typical symptom for
 *  this deadlock is the PPU spinning in PSGL internal functions
 *  such as the FIFO out-of-space callback or semaphore waits in
 *  psglSwap(). To prevent such a deadlock, kick the SPU job to
 *  fill the hole as soon as a hole is generated by this function.
 *
 *  @param mode Specifies what kind of primitives to render.
 *  Symbolic constants <c>GL_POINTS, GL_LINE_STRIP, GL_LINE_LOOP,
 *  GL_LINES, GL_TRIANGLE_STRIP, GL_TRIANGLE_FAN, GL_TRIANGLES,
 *  GL_QUAD_STRIP</c>, and <c>GL_QUADS</c> are accepted.
 *  @param count Specifies the number of elements to be rendered.
 *  @param type Specifies the type of the values in <i>indices</i>.
 *     Must be one of <c>GL_UNSIGNED_BYTE, GL_UNSIGNED_SHORT</c>,
 *     or <c>GL_UNSIGNED_INT</c>.
 *  @param indices Specifies an offset for the index data into a buffer object.
 *     If index data starts from the beginning of a buffer object, specify NULL.
 *  @param indexOffset An argument into which this command stores
 *     an offset value to the RSX&trade;-accessible location for the index data.
 *  @param holeEA An argument into which this command stores the effective
 *      address for the command-buffer hole.
 *  @param holeSizeInWord An argument into which this command stores
 *      the size of the command-buffer hole in words.
 *
 *  @return 0 for success, 1 for failure
 *  @sa psglFillCommandBufferHole, glDrawElements
 */
 PSGL_EXPORT int psglDrawCommandBufferHole( GLenum mode, GLsizei count, GLenum type, const GLvoid* indices, 
										  uint32_t *indexOffset, uint32_t *holeEA, uint32_t *holeSizeInWord )
{
	PSGLcontext*	LContext = _CurrentContext;
	JS_ASSERT( LContext );

#ifndef JS_NO_ERROR_CHECK
	// check input values
	switch ( mode )
	{
	case GL_POINTS:
	case GL_LINES:
	case GL_LINE_STRIP:
	case GL_LINE_LOOP:
	case GL_TRIANGLES:
	case GL_TRIANGLE_STRIP:
	case GL_TRIANGLE_FAN:
	case GL_QUADS:
	case GL_QUAD_STRIP:
		break;
	default:
		_jsSetError( GL_INVALID_ENUM );
		return 1;
	}

	switch ( type )
	{
	case GL_UNSIGNED_BYTE:
	case GL_UNSIGNED_SHORT:
	case GL_UNSIGNED_INT: // JetStream extention. really needed ?
		break;
	default:
		_jsSetError( GL_INVALID_ENUM );
		return 1;
	}

	if ( count < 0 ) 
	{
		_jsSetError( GL_INVALID_VALUE );
		return 1;
	}
#endif

	// [YLIN] use dcbz to zero init dparams
	uint32_t _tmp_clear_loop = c_rounded_size_of_jsDrawParams>>7;
	do{
		--_tmp_clear_loop;
		__dcbz(s_dparams_buff+(_tmp_clear_loop<<7));
	}while(_tmp_clear_loop);
	jsDrawParams *dparams = (jsDrawParams *)s_dparams_buff;
	dparams->mode = mode;
	dparams->indexCount = count;
	dparams->indexType = type;
	dparams->indices = indices;

	// validate states
	if ( LContext->needValidate ) _jsValidateStates( PSGL_VALIDATE_ALL );

	// Slow path is not supported
	if ( _jsPlatformRequiresSlowPath( dparams, type, count ) )
		return 1;

	// _jsPlatformDraw layer from here
	
	jsGcmDriver *driver = _jsGetGcmDriver();
	JS_ASSERT( driver->rtValid );

	// check for any writable mapped buffers
	if ( driver->flushBufferCount != 0 )
		driver->invalidateVertexCache = GL_TRUE;

	// validate attributes
	// only the fast pass is supported. 
	// make sure all attributes are bound and no transfer is required
    GLboolean isMain = 0;
	bool attribSetDirty = false;
    if ( LContext->attribSetDirty && LContext->attribSetName )
	{
    	jsAttribSet* attribSet = _jsGetAttribSet( LContext->attribSetName );
		attribSetDirty = attribSet->dirty;
	}
	uint32_t totalXfer = 0;
	for ( GLuint i = 0; i < _JS_MAX_VERTEX_ATTRIBS; ++i )
		totalXfer += dparams->attribXferSize[i];

	if( dparams->indexType && dparams->xferTotalSize == 0 && !attribSetDirty && 
		totalXfer == 0 && dparams->indexXferSize == 0 )
	{
		*indexOffset = _jsValidateAttributes( dparams->indices, &isMain );
	}
	else
		return 1;

#ifndef JS_NO_ERROR_CHECK
    if ( *indexOffset == GMM_ERROR )
    {
        return 1;
    }
#endif

	// check for validation of vertex cache
	if ( driver->invalidateVertexCache )
	{
		driver->invalidateVertexCache = GL_FALSE;
		_jsGcmFifoGlInvalidateVertexCache();
	}

	GCM_FUNC( cellGcmSetUpdateFragmentProgramParameter, 
              gmmIdToOffset( driver->fpLoadProgramId ) + driver->fpLoadProgramOffset );

	// _jsGcmFifoGlDrawElements layer from here

	// compute hole size in word based on cellGcmSetDrawIndexArray implementation
	*holeSizeInWord = 7 + 2 + 3 +
		((count/0x7ff00) * (0x7ff+1)) + 
		((count%0x7ff00) / 0x100) +
		(((count%0x7ff00) % 0x100)? 1: 0);
	
	// Need a space for JTS for each 128 byte block
	*holeSizeInWord += *holeSizeInWord / 32;

	// make it multiple of 4 words (16 byte for DMA requirement)
	*holeSizeInWord += (4 - *holeSizeInWord%4); 

	psglGenerateCommandBufferHole(*holeSizeInWord, holeEA);

	return 0;
}

/**
 *  @short Validates specified GL states encapsulated in
 *  the PSGL context.
 *
 *  This function validates specified PSGL states by pushing
 *  commands onto the command buffer. After this function is called,
 *  the PSGL context is updated as if all states are valid.
 *  If mask is not <c>PSGL_VALIDATE_ALL</c>, only the specified states
 *  are validated. In that case, the return value indicates
 *  states that were not validated. It would be the caller&rsquo;s
 *  responsibility to make those states clean either by calling
 *  psglInvalidateStates() or by using GCM to update RSX&trade; states.
 *
 *  Valid bit masks for states are:
 <TABLE>
 <tr><td><code>PSGL_VALIDATE_NONE</code></td><td>Validates no states</TD></TR>
 <tr><td><code>PSGL_VALIDATE_FRAMEBUFFER</code></td><td>Validates frame buffer</TD></TR>
 <tr><td><code>PSGL_VALIDATE_TEXTURES_USED</code></td><td>Validates used textures</TD></TR>
 <tr><td><code>PSGL_VALIDATE_VERTEX_PROGRAM</code></td><td>Validates a vertex program</TD></TR>
 <tr><td><code>PSGL_VALIDATE_VERTEX_CONSTANTS </code></td><td>Validates vertex program constants</TD></TR>
 <tr><td><code>PSGL_VALIDATE_VERTEX_TEXTURES_USED </code></td><td>Validates textures used by vertex shader</TD></TR>
 <tr><td><code>PSGL_VALIDATE_FFX_VERTEX_PROGRAM </code></td><td>Validates fixed-function vertex program</TD></TR>
 <tr><td><code>PSGL_VALIDATE_FRAGMENT_PROGRAM </code></td><td>Validates a fragment program</TD></TR>
 <tr><td><code>PSGL_VALIDATE_FFX_FRAGMENT_PROGRAM </code></td><td>Validates fixed-function fragment program</TD></TR>
 <tr><td><code>PSGL_VALIDATE_FRAGMENT_SHARED_CONSTANTS </code></td><td>Validates shared constants for fragment shader</TD></TR>
 <tr><td><code>PSGL_VALIDATE_VIEWPORT </code></td><td>Validates viewport</TD></TR>
 <tr><td><code>PSGL_VALIDATE_ALPHA_TEST </code></td><td>Validates alpha test</TD></TR>
 <tr><td><code>PSGL_VALIDATE_DEPTH_TEST </code></td><td>Validates depth test</TD></TR>
 <tr><td><code>PSGL_VALIDATE_WRITE_MASK </code></td><td>Validates write masks</TD></TR>
 <tr><td><code>PSGL_VALIDATE_STENCIL_TEST </code></td><td>Validates stencil test</TD></TR>
 <tr><td><code>PSGL_VALIDATE_STENCIL_OP_AND_MASK </code></td><td>Validates stencil operation and masks</TD></TR>
 <tr><td><code>PSGL_VALIDATE_SCISSOR_BOX </code></td><td>Validates scissor box</TD></TR>
 <tr><td><code>PSGL_VALIDATE_FACE_CULL </code></td><td>Validates face culling</TD></TR>
 <tr><td><code>PSGL_VALIDATE_BLENDING </code></td><td>Validates blending</TD></TR>
 <tr><td><code>PSGL_VALIDATE_POINT_RASTER </code></td><td>Validates point raster</TD></TR>
 <tr><td><code>PSGL_VALIDATE_LINE_RASTER </code></td><td>Validates line raster</TD></TR>
 <tr><td><code>PSGL_VALIDATE_POLYGON_OFFSET </code></td><td>Validates polygon offset</TD></TR>
 <tr><td><code>PSGL_VALIDATE_SHADE_MODEL </code></td><td>Validates shade model</TD></TR>
 <tr><td><code>PSGL_VALIDATE_LOGIC_OP </code></td><td>Validates logical operation</TD></TR>
 <tr><td><code>PSGL_VALIDATE_MULTISAMPLING </code></td><td>Validates multisampling</TD></TR>
 <tr><td><code>PSGL_VALIDATE_POLYGON_MODE </code></td><td>Validates polygon mode</TD></TR>
 <tr><td><code>PSGL_VALIDATE_PRIMITIVE_RESTART </code></td><td>Validates primitive restart</TD></TR>
 <tr><td><code>PSGL_VALIDATE_CLIP_PLANES </code></td><td>Validates clip planes</TD></TR>
 <tr><td><code>PSGL_VALIDATE_SHADER_SRGB_REMAP </code></td><td>Validates shader SRGB remap</TD></TR>
 <tr><td><code>PSGL_VALIDATE_POINT_SPRITE </code></td><td>Validates point sprite</TD></TR>
 <tr><td><code>PSGL_VALIDATE_TWO_SIDE_COLOR </code></td><td>Validates two-side color</TD></TR>
 <tr><td><code>PSGL_VALIDATE_ALL </code></td><td>Validates all states</TD></TR>
 </TABLE>
 *
 *  @param mask Specifies the states to be validated with a combination
 *      of bit masks.
 *  @return A combination of bit masks of any states that were not validated because of the mask.
 *  @sa psglInvalidateStates, psglDrawCommandBufferHole
 */
GLuint psglValidateStates( GLuint mask )
{
	return _jsValidateStates( mask );
}

/**
 *  @short Invalidates specified states encapsulated in the PSGL context.
 *
 *  This function invalidates PSGL states specified with <i>mask</i>.
 *  Invalidated states are validated the next time that validation happens because
 *  of a draw call, glFlush(), glFinish(), psglSwap(), or psglValidateStates().
 *
 *  @param mask Specifies states to be invalidated with a combination of bit masks.
 *      See psglValidateStates() for the list of valid bit masks.
 *  @sa psglValidateStates
 */
void psglInvalidateStates( GLuint mask )
{
	PSGLcontext*	LContext = _CurrentContext;
	JS_ASSERT( LContext );

	LContext->needValidate |= mask;
}

/**
 *  @short Validates attributes.
 *
 *  This function validates all attributes, which are bound vertex buffer objects,
 *  in preparation for the draw.
 *
 *  @param indices Specifies an offset for the index data into a buffer object.
 *     If index data starts from the beginning of the buffer object,
 *     specify NULL.
 *  @param isMain Indicates whether the index buffer is in
 *  main memory. Returns GL_TRUE if the buffer for indices is in main memory.
 *      Returns GL_FALSE if it is in local memory.
 *
 *  @return RSX&trade; offset to the index buffer.
 *  @sa psglInvalidateAttributes, psglDrawCommandBufferHole
 */
GLuint psglValidateAttributes( const GLvoid* indices, GLboolean *isMain )
{
	return _jsValidateAttributes( indices, isMain );
}

/**
 *  @short Invalidates all attributes.
 *
 *  This function invalidates all attributes. The next time that validation of attributes happens,
 *  either by a draw call or with psglValidateAttributes(),
 *  all bound vertex buffer objects are validated as attributes.
 *  @sa psglValidateAttributes
 */
void psglInvalidateAttributes()
{
	_jsInvalidateAttributes();
}


/** @} */
