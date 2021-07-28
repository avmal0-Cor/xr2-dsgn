/*
	SCE CONFIDENTIAL
	PlayStation(R)3 Programmer Tool Runtime Library 310.001
	Copyright (C) 2008 Sony Computer Entertainment Inc.
	All Rights Reserved.
*/
/**
	\file	edge_decompressor.h
	
	EDGE decompression interface.
*/

#ifndef __CELL_EDGE_DECOMPRESSOR_H__
#define __CELL_EDGE_DECOMPRESSOR_H__

#include <cell/fios/compression/compression_decompressor.h>
#include <cell/atomic.h>

#include <cell/spurs.h>
#include <stddef.h>

#ifndef DOXYGEN_IGNORE

#ifndef EDGE_DECOMPRESSOR_ASSERT
//! Controls whether we use assert or not.
# define EDGE_DECOMPRESSOR_ASSERT     1
#endif

/* Standard C includes */

#ifndef edge_memcpy
# include <string.h>
# define edge_memcpy(d,s,l) memcpy(d,s,l)
#endif

#ifndef edge_assert
# if EDGE_DECOMPRESSOR_ASSERT
#  include <assert.h>
#  define edge_assert(x)    assert(x)
# else
#  define edge_assert(x)    ((void)0)
#endif
#endif

#endif // DOXYGEN_IGNORE

namespace cell {
namespace fios {
namespace Compression {

/** \brief Common base class of all EDGE SPU decompressors.
	The EdgeDecompressor class is an abstract subclass of #AsyncDecompressor,
	and contains the logic common to all EDGE decompressors.

	Concrete subclasses are provided for EDGE zlib and EDGE LZMA.
	\see EdgeZlibDecompressor
	\see EdgeLZMADecompressor */
class EdgeDecompressor : public AsyncDecompressor
{
public:
	/** \brief Default constructor with no arguments.
		If you use this constructor, you must manually call init() to set the
		CellSpurs and CellSpursTaskset before use. */
	EdgeDecompressor() : m_initialized(false)
	{
		init(NULL,NULL);
	}
	
	/** \brief Constructor with a CellSpurs pointer.
		\param[in] pSpurs   SPURS instance to use.
		A CellSpursTaskset will be automatically created. */
	EdgeDecompressor(CellSpurs *pSpurs) : m_initialized(false)
	{
		init(pSpurs);
	}
	
	/** \brief Constructor with CellSpurs and CellSpursTaskset. 
		\param[in] pSpurs   SPURS instance to use.
		\param[in] pTaskset SPURS taskset to use. */
	EdgeDecompressor(CellSpurs *pSpurs, CellSpursTaskset *pTaskset) : m_initialized(false)
	{
		init(pSpurs,pTaskset);
	}

	/** \brief Destructor.
		Subclasses must shut down the inflate queue from their destructor. */
	virtual ~EdgeDecompressor()
	{
		int err;
		(void)&err;
		
		// We're no longer initialized.
		m_initialized = false;
		
		// Detach the event flag.
		if (m_pSpurs != NULL) {
			err = cellSpursEventFlagDetachLv2EventQueue( &m_eventFlag );
			edge_assert(err == CELL_OK);
		}
		
		// Shut down the taskset.
		if (m_pTaskset != NULL) {
			err = cellSpursShutdownTaskset( m_pTaskset );
			edge_assert(err == CELL_OK);
			err = cellSpursJoinTaskset( m_pTaskset );
			edge_assert(err == CELL_OK);
			m_pTaskset = NULL;
		}
	}
	
		// ---------------------------------------
		// Pure virtual functions

#ifndef DOXYGEN_IGNORE
	/** \brief Returns the required task context save size.
		Must be implemented by concrete subclasses.
		\return Size needed for task context. */
	virtual size_t getTaskContextSaveSize() const = 0;

	/** \brief Returns the buffer size needed for a given queue length.
		Must be implemented by concrete subclasses.
		\param[in] inflateQueueLength  Queue length
		\return Buffer size needed for a queue of that length. */
	virtual size_t getInflateQueueSize(uint32_t inflateQueueLength) const = 0;
	
	/** \brief Creates the inflate queue.
		Must be implemented by concrete subclasses.
		\return Inflate queue. */
	virtual void * createInflateQueue() = 0;
	
	/** \brief Creates the inflate SPU task.
		Must be implemented by concrete subclasses.
		\return Inflate task. */
	virtual CellSpursTaskId createInflateTask() = 0;
	
	/** \brief Adds an inflate queue element.
		Must be implemented by concrete subclasses.
		\param[in]     pCompressed           Compressed data to expand.
		\param[in]     compressedLength      Length of the compressed data.
		\param[in]     pCopyOutLocation      Destination for partial copyout of decompressed data.
		\param[in]     copyOutOffset         Number of bytes to skip when doing the partial copyout.
		\param[in]     copyOutLength         Number of bytes to copy out.
		\param[in]     copyOutRemainder      Number of remainder bytes left over at the end of the decompressed data.
		\param[in,out] pWorkToDoCounter      Work-to-do flag, decremented when the work is complete.
		\param[in]     pEventFlag            Event flag to signal when the work is complete.
		\param[in]     eventFlagBits         Event flag bits which should be signalled. */
	virtual void addInflateQueueElementPartialCopyOut(
		const uint8_t * pCompressed,
		uint32_t  compressedLength,
		uint8_t * pCopyOutLocation,
		uint32_t  copyOutOffset,
		uint32_t  copyOutLength,
		uint32_t  copyOutRemainder,
		uint32_t *pWorkToDoCounter,
		CellSpursEventFlag *pEventFlag,
		uint16_t  eventFlagBits) = 0;

		// ---------------------------------------
		// Implementation common to all EDGE decompressors.
	
	/** \brief Creates a default SPURS taskset.
		\param[in] pSpurs   SPURS instance.
		\return A new SPURS taskset. */
	CellSpursTaskset * createTaskset(CellSpurs *pSpurs)
	{
		uint8_t prios[8] = {15, 15, 15, 15, 15, 15, 15, 15};
		return createTaskset(pSpurs, prios);
	}

	/** \brief Creates a SPURS taskset with the given priorities.
		\param[in] pSpurs   SPURS instance.
		\param[in] prios    An array of 8 priority bytes.
		\return A new SPURS taskset. */
	CellSpursTaskset * createTaskset(CellSpurs *pSpurs, uint8_t prios[8])
	{
		// Create a basic taskset with the specified priorities.
		CellSpursTasksetAttribute tasksetAttribute;
		int32_t err = cellSpursTasksetAttributeInitialize( &tasksetAttribute, 0, prios, 8 );
		edge_assert(err == CELL_OK); if (err != CELL_OK) return NULL;
		err = cellSpursTasksetAttributeSetName(&tasksetAttribute, "edgeDecompressorTaskset");
		edge_assert(err == CELL_OK); if (err != CELL_OK) return NULL;
		err = cellSpursCreateTasksetWithAttribute( pSpurs, &m_taskset, &tasksetAttribute );
		edge_assert(err == CELL_OK); if (err != CELL_OK) return NULL;
		return &m_taskset;
	}
	
	/** \brief Initializes the decompressor.
		\param[in] pSpurs    SPURS instance. May be NULL, but the decompressor will remain uninitialized if it is.
		\param[in] pTaskset  SPURS taskset. May be NULL, but the decompressor will remain uninitialized if it is. */
	void init(CellSpurs *pSpurs, CellSpursTaskset *pTaskset = NULL)
	{
		int err = 0;
		
		edge_assert(!m_initialized);
		edge_assert(COMPRESSION_ISALIGNED(&m_taskContext, COMPRESSION_CONSTANT_LOG2(CELL_SPURS_TASK_CONTEXT_ALIGN)));
		edge_assert(COMPRESSION_ISALIGNED(&m_taskset, COMPRESSION_CONSTANT_LOG2(CELL_SPURS_TASKSET_ALIGN)));
		edge_assert(COMPRESSION_ISALIGNED(&m_eventFlag, COMPRESSION_CONSTANT_LOG2(CELL_SPURS_EVENT_FLAG_ALIGN)));
		edge_assert(COMPRESSION_ISALIGNED(&m_queueBuffer, COMPRESSION_CONSTANT_LOG2(128)));
		
		// If the user passed in a NULL taskset, create a default one.
		if (pSpurs != NULL && pTaskset == NULL)
			pTaskset = createTaskset(pSpurs);
		
		// Initial setup of member variables.
		m_busyJobMask = 0;
		m_pSpurs = pSpurs;
		m_pTaskset = pTaskset;
		m_inflateQueue = NULL;
		m_inflateQueueLength = 0;
		m_inflateTask = CellSpursTaskId(-1);
		m_initialized = false;
		
		// Initialize decompression parameters.
		m_dp.m_sourceMaxSize = 65536;
		m_dp.m_destinationMaxSize = 65536;
		m_dp.m_contextAlignment = 0;
		m_dp.m_contextSize = 0;
		m_dp.m_sourceAlignment = 0;
		m_dp.m_destinationAlignment = 0;
		m_dp.m_flagDecompressInPlaceDMA = 1; // yes, we can decompress in-place
		m_dp.m_flagDecompressInPlaceNonDMA = 0;
		m_dp.m_flagDecompressStreamsToOutputDMA = 1; // yes, we stream to output
		m_dp.m_flagDecompressStreamsToOutputNonDMA = 0;
		m_dp.m_flagPartialCopyoutDMA = 1; // yes, we can do a partial copyout
		m_dp.m_flagPartialCopyoutNonDMA = 0;
		
		// Only continue with the rest of initialization if we have been
		// given a SPURS instance and taskset.
		if (m_pSpurs != NULL && m_pTaskset != NULL)
		{
			// Assert that our context space is big enough.
			edge_assert(getTaskContextSaveSize() <= sizeof(m_taskContext));
			if (getTaskContextSaveSize() > sizeof(m_taskContext)) return;
			
			// Create an event flag.
			err = cellSpursEventFlagInitializeIWL( m_pSpurs, &m_eventFlag,
				CELL_SPURS_EVENT_FLAG_CLEAR_AUTO, CELL_SPURS_EVENT_FLAG_SPU2PPU );
			edge_assert(err == CELL_OK); if (err != CELL_OK) return;
			
			err = cellSpursEventFlagAttachLv2EventQueue( &m_eventFlag );
			edge_assert(err == CELL_OK); if (err != CELL_OK) return;
			
			// Create an inflate queue. We have a fixed-size buffer and we
			// use a queue that is just big enough to fit in it.
			while (getInflateQueueSize(++m_inflateQueueLength) <= sizeof(m_queueBuffer)) {}
			m_inflateQueueLength -= 1;
			edge_assert(m_inflateQueueLength > 0);
			m_inflateQueue = createInflateQueue();
			edge_assert(m_inflateQueue != NULL);
			if (m_inflateQueue == NULL) return;
			
			// Create the task.
			m_inflateTask = createInflateTask();
			edge_assert(m_inflateTask != CellSpursTaskId(-1));
			if (m_inflateTask == CellSpursTaskId(-1)) return;
			
			memset(&m_jobInfo, 0, sizeof(m_jobInfo));

			// We're ready to go.
			m_initialized = true;
		}
	}
	
	/** \brief Allocates a single job index.
		\internal
		\return Job index in the range (0, kMaximumParallelJobs-1). */
	uint32_t allocateJobIndex()
	{
		// Atomically allocate a bit from m_busyJobMask. The result is a number
		// between 0 and (kMaximumParallelJobs-1), and is placed into jobIndex.
		uint32_t jobIndex = 0;
		uint32_t oldMask, newMask = 0;
		do {
			__lwsync();

			oldMask = m_busyJobMask;                                // read job mask
			if (oldMask == ((1<<kMaximumParallelJobs)-1)) continue; // if no bits are free, retry
			while ((oldMask & (1<<jobIndex)) != 0)                  // loop til we find a free bit
				jobIndex = (jobIndex + 1) % kMaximumParallelJobs;   //  didn't find one? move to the next bit
			newMask = oldMask | (1 << jobIndex);                    // mark this bit as allocated
		} while (cellAtomicCompareAndSwap32(const_cast<uint32_t*>(&m_busyJobMask), oldMask, newMask) != oldMask); // CAS

		edge_assert(jobIndex < kMaximumParallelJobs);
		return jobIndex;
	}
	
	/** \brief Deallocates a single job index.
		\internal
		\param[in] jobIndex   Index which was previously allocated by allocateJobIndex. */
	void deallocateJobIndex(uint32_t jobIndex)
	{
		// Make sure the index is in fact allocated!
		edge_assert(jobIndex < kMaximumParallelJobs);
		edge_assert((m_busyJobMask & (1<<jobIndex)) != 0);
		
		uint32_t oldMask, newMask;
		do {
			__lwsync();

			oldMask = m_busyJobMask;                                // read job mask
			newMask = oldMask & ~(1<<jobIndex);                     // clear the bit
		} while (cellAtomicCompareAndSwap32(const_cast<uint32_t*>(&m_busyJobMask), oldMask, newMask) != oldMask); // CAS
	}

	/** \brief Starts an async decompression of a buffer of data.
		No context or allocator is required. The maximum buffer size is 64KiB.
		\param[in]     pJob            Job description.
		\return A pointer which can be passed to decompressionTaskWait() to get the result. */
	virtual TaskPtr decompressAsync(JobDescription *pJob)
	{
		// We must be initialized.
		edge_assert(m_initialized);
		if (!m_initialized) return NULL;
		
		// Allocate a job index, fill out m_jobInfo, and clear the event flag.
		uint32_t jobIndex = allocateJobIndex();
		edge_assert(m_jobInfo[jobIndex].m_workToDo == 0);
		m_jobInfo[jobIndex].m_job = *pJob;
		m_jobInfo[jobIndex].m_workToDo = 1;
		cellSpursEventFlagClear(&m_eventFlag, (1 << jobIndex));
		
		// Start the decompression.
		if (pJob->pCopyOut != NULL && pJob->pDecompressed == NULL)
		{
			// Partial copy-out into pCopyOut.
			addInflateQueueElementPartialCopyOut(
				pJob->pCompressed, pJob->compressedLength,
				pJob->pCopyOut, pJob->copyOutOffset, pJob->copyOutLength,
				pJob->decompressedLength - (pJob->copyOutOffset + pJob->copyOutLength),
				&m_jobInfo[jobIndex].m_workToDo,
				&m_eventFlag,
				(1 << jobIndex));
		}
		else
		{
			// Decompress full buffer into pDecompressedData.
			addInflateQueueElementPartialCopyOut(
				pJob->pCompressed, pJob->compressedLength,
				pJob->pDecompressed, 0, pJob->decompressedLength, 0,
				&m_jobInfo[jobIndex].m_workToDo,
				&m_eventFlag,
				(1 << jobIndex));
		}
		
		// The task ptr that we return is our internal JobInfo struct.
		return (TaskPtr)&m_jobInfo[jobIndex];
	}
	
	/** \brief Polls for async completion of a previous async decompression request.
		After this returns true, you must still call decompressionTaskWait to
		destroy the task and find out how big the decompressed data is.
		\param[in]     pTask           Pointer previously returned by decompressAsync.
		\return True if the decompression task is complete. */
	virtual bool decompressionTaskIsComplete(TaskPtr pTask)
	{
		JobInfo *pJobInfo = (JobInfo*)pTask;
		__lwsync();
		return (pJobInfo->m_workToDo == 0);
	}
	
	/** \brief Waits for async decompression of a buffer of data to be complete.
		\param[in]     pTask             Pointer previously returned by decompressAsync.
		\param[out]    pDecompressedLen  Final size of the decompressed data.
		\return 0 for success. Any other value indicates a codec-specific error. */
	virtual int decompressionTaskWait(TaskPtr pTask, uint32_t *pDecompressedLen)
	{
		edge_assert(pTask != NULL);
		if (pTask == NULL) return -1;
		
		// Get the job info and index.
		JobInfo *pJobInfo = (JobInfo*)pTask;
		uint32_t jobIndex = (uint32_t)(pJobInfo - &m_jobInfo[0]);
		edge_assert(jobIndex < kMaximumParallelJobs);
		JobDescription *pJob = &pJobInfo->m_job;

		// Wait for the event flag to be signalled.
		uint16_t bits = (uint16_t)(1 << jobIndex);
		while (pJobInfo->m_workToDo != 0)
			cellSpursEventFlagWait(&m_eventFlag, &bits, CELL_SPURS_EVENT_FLAG_AND);
		
		// Handle copyout with temp buffer by calling memcpy. This only occurs
		// when you are reading into a stack buffer, which means that we can't DMA!
		if ((pJob->pCopyOut != NULL) && (pJob->pDecompressed != NULL))
			edge_memcpy(pJob->pCopyOut, pJob->pDecompressed + pJob->copyOutOffset, pJob->copyOutLength);
		
		// The decompressed buffer size is assumed to be exactly the 
		// same as the final decompressed data.
		*pDecompressedLen = pJob->pCopyOut ? pJob->copyOutLength:pJob->decompressedLength;

		// Deallocate the job index.
		memset(pJobInfo, 0, sizeof(JobInfo));
		deallocateJobIndex(jobIndex);
		return 0;
	}
#endif // DOXYGEN_IGNORE
	
protected:
#ifndef DOXYGEN_IGNORE
	uint8_t    m_taskContext[4096]
               __attribute__((__aligned__(CELL_SPURS_TASK_CONTEXT_ALIGN))); //!< EDGE SPURS task context.
	
	CellSpursTaskset       m_taskset
                __attribute__((__aligned__(CELL_SPURS_TASKSET_ALIGN))); //!< Our locally created taskset, if used.
	
	CellSpursEventFlag     m_eventFlag
                __attribute__((__aligned__(CELL_SPURS_EVENT_FLAG_ALIGN))); //!< Event flag used to signal completion.

	uint8_t    m_queueBuffer[512]
                __attribute__((__aligned__(128))); //!< \internal Buffer to hold queue elements.
	
	enum {
		kMaximumParallelJobs = 16                //!< Controls the maximum number of parallel jobs. Must be a power of 2 and can't be more than 16.
	};
	typedef struct {
		JobDescription m_job;                    //!< Job description
		uint32_t            m_workToDo;               //!< Work flag needed by EDGE APIs
	} JobInfo;
	
	volatile uint32_t       m_busyJobMask;            //!< Bitmask to keep track of allocated jobs
	JobInfo            m_jobInfo[kMaximumParallelJobs]; //!< Job information
	CellSpurs *        m_pSpurs;                 //!< SPURS instance
	CellSpursTaskset * m_pTaskset;               //!< SPURS taskset where our task will be created
	void *             m_inflateQueue;           //!< Our inflate work queue
	uint32_t                m_inflateQueueLength;     //!< Maximum length of the inflate work queue
	CellSpursTaskId    m_inflateTask;            //!< Our SPURS task
	bool               m_initialized;            //!< Whether we initialized successfully.
#endif // DOXYGEN_IGNORE
};


}; /* namespace Compression */
}; /* namespace fios */
}; /* namespace cell */

#undef edge_assert
#undef edge_memcpy

#endif /*  __CELL_EDGE_DECOMPRESSOR_H__ */

