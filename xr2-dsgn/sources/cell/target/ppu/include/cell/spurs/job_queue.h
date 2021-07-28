/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2009 Sony Computer Entertainment Inc.
 * All Rights Reserved.
 */

#ifndef __CELL_SPURS_JOBQUEUE_H__
#define __CELL_SPURS_JOBQUEUE_H__

/* standard C++ header */
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h> /* size_t */

#include <assert.h>
#include <ppu_intrinsics.h>

#include <cell/sysmodule.h>

/* spurs */
#include <cell/spurs/types.h>
#include <cell/spurs/job_descriptor.h>

/* job queue */
#include <cell/spurs/job_queue_define.h>
#include <cell/spurs/job_queue_types.h>
#include <cell/spurs/version.h>

#ifdef __cplusplus
extern "C" {
#endif
	/* APIs for CellSpursJobQueue */

	/* functions to set a parameter to CellSpursJobQueueAttribute */
	int cellSpursJobQueueAttributeInitialize(CellSpursJobQueueAttribute *attr);

	int cellSpursJobQueueAttributeSetMaxGrab(CellSpursJobQueueAttribute *attr, unsigned int maxGrab);
	int cellSpursJobQueueAttributeSetSubmitWithEntryLock(CellSpursJobQueueAttribute *attr, bool submitWithEntryLock);
	int cellSpursJobQueueAttributeSetDoBusyWaiting(CellSpursJobQueueAttribute *attr, bool doBusyWaiting);
	int cellSpursJobQueueAttributeSetIsHaltOnError(CellSpursJobQueueAttribute *attr, bool isHaltOnError);
	int cellSpursJobQueueAttributeSetIsJobTypeMemoryCheck(CellSpursJobQueueAttribute *attr, bool isJobTypeMemoryCheck);
	int cellSpursJobQueueAttributeSetMaxSizeJobDescriptor(CellSpursJobQueueAttribute *attr, unsigned int maxSizeJobDescriptor);
	//int cellSpursJobQueueAttributeSetMaxNumJobsOnSpu(CellSpursJobQueueAttribute *attr, unsigned int maxNumJobsOnSpu);
	int cellSpursJobQueueAttributeSetGrabParameters(CellSpursJobQueueAttribute *attr, unsigned int maxNumJobsOnSpu, unsigned int maxGrab);

	/* cellSpursJobQueueCreate internal implementation */
	int _cellSpursCreateJobQueue(
		unsigned int jqRevision,
		unsigned int sdkRevision,
		CellSpurs *pSpurs,
		CellSpursJobQueue *pJobQueue,
		const CellSpursJobQueueAttribute *attr,
		const char *pName,
		uint64_t *pCommandList,
		unsigned int depth,
		unsigned int numSpus,
		const uint8_t priorityTable[CELL_SPURS_MAX_SPU]
		);

	/* cellSpursJobQueueCreate external interface */
	static inline int cellSpursCreateJobQueue(
		CellSpurs *pSpurs,
		CellSpursJobQueue *pJobQueue,
		const CellSpursJobQueueAttribute *attr,
		const char *pName,
		uint64_t *pCommandList,
		unsigned int depth,
		unsigned int numSpus,
		const uint8_t priorityTable[CELL_SPURS_MAX_SPU]
		)
	{
		int ret = _cellSpursCreateJobQueue(
			CELL_SPURS_JOBQUEUE_REVISION,
			_CELL_SPURS_JQ_INTERNAL_VERSION,
			pSpurs,
			pJobQueue,
			attr,
			pName,
			pCommandList,
			depth,
			numSpus,
			priorityTable
			);
		return ret;
	}

	/* Information */
	CellSpurs *cellSpursJobQueueGetSpurs(const CellSpursJobQueue *pJobQueue);
	unsigned int cellSpursJobQueueGetHandleCount(const CellSpursJobQueue *pJobQueue);
	int	cellSpursJobQueueGetError(const CellSpursJobQueue *pJobQueue,
				int* exitCode, void** cause);
	unsigned int cellSpursJobQueueGetMaxSizeJobDescriptor(const CellSpursJobQueue *pJobQueue);
	int cellSpursGetJobQueueId(const CellSpursJobQueue *pJobQueue, CellSpursWorkloadId *pId);
	int cellSpursJobQueueGetSuspendedJobSize(const CellSpursJobHeader *pJob, size_t sizeJobDesc, enum CellSpursJobQueueSuspendedJobAttribute attr, unsigned int *pSize);

	/* Exception Handling */
	int cellSpursJobQueueSetExceptionEventHandler(
			CellSpursJobQueue* pJobQueue,
			CellSpursJobQueueExceptionEventHandler handler,
			void* arg
		);
	int cellSpursJobQueueUnsetExceptionEventHandler(CellSpursJobQueue* pJobQueue);

	/* State control */
	int cellSpursJobQueueSetWaitingMode(CellSpursJobQueue *pJobQueue, unsigned mode);

	int cellSpursShutdownJobQueue(CellSpursJobQueue *pJobQueue);
	int cellSpursJoinJobQueue(CellSpursJobQueue *pJobQueue, int* exitCode);

	int cellSpursJobQueueOpen(CellSpursJobQueue *pJobQueue, CellSpursJobQueueHandle *handle);
	int cellSpursJobQueueClose(CellSpursJobQueue *pJobQueue, CellSpursJobQueueHandle handle);

	int _cellSpursJobQueuePushJobBody2(CellSpursJobQueue *pJobQueue, CellSpursJobQueueHandle handle, CellSpursJobHeader *pJob, size_t sizeJobDesc, unsigned tag, CellSpursJobQueueSemaphore *pSemaphore, bool isExclusive, bool isBlocking);

	int _cellSpursJobQueuePushJobListBody(CellSpursJobQueue *pJobQueue, CellSpursJobQueueHandle handle, CellSpursJobList *pJoblist, unsigned tag, CellSpursJobQueueSemaphore *pSemaphore, bool isBlocking);

	static inline int _cellSpursJobQueuePushJob2(CellSpursJobQueue *pJobQueue, CellSpursJobQueueHandle handle, CellSpursJobHeader *pJob, size_t sizeJobDesc, unsigned tag, CellSpursJobQueueSemaphore *pSemaphore, bool isExclusive, bool isBlocking)
	{
#ifdef CELL_SPURS_JOBDESCRIPTOR_ERROR_CHECK
		__CELL_SPURS_RETURN_IF(cellSpursJobQueueCheckJob((const CellSpursJob256 *)(uintptr_t)pJob, sizeJobDesc, cellSpursJobQueueGetMaxSizeJobDescriptor(pJobQueue)));
#endif
		return _cellSpursJobQueuePushJobBody2(pJobQueue, handle, pJob, sizeJobDesc, tag, pSemaphore, isExclusive, isBlocking);
	}

	static inline int _cellSpursJobQueuePushJobList(CellSpursJobQueue *pJobQueue, CellSpursJobQueueHandle handle, CellSpursJobList *pJoblist, unsigned tag, CellSpursJobQueueSemaphore *pSemaphore, bool isBlocking)
	{
#ifdef CELL_SPURS_JOBDESCRIPTOR_ERROR_CHECK
		for(unsigned index = 0; index < pJoblist->numJobs; index++) {
			CellSpursJob256*	pJob = (CellSpursJob256*)(uintptr_t)pJoblist->eaJobList + pJoblist->sizeOfJob * index;
			__CELL_SPURS_RETURN_IF(cellSpursJobQueueCheckJob((const CellSpursJob256 *)pJob, pJoblist->sizeOfJob, cellSpursJobQueueGetMaxSizeJobDescriptor(pJobQueue)));
		}
#endif
		return _cellSpursJobQueuePushJobListBody(pJobQueue, handle, pJoblist, tag, pSemaphore, isBlocking);
	}

	static inline int cellSpursJobQueuePush(CellSpursJobQueue *pJobQueue, CellSpursJobQueueHandle handle, CellSpursJobHeader *pJob, size_t sizeJobDesc, CellSpursJobQueueSemaphore *pSemaphore)
	{
		return _cellSpursJobQueuePushJob2(pJobQueue, handle, pJob, sizeJobDesc, 0, pSemaphore, false, true);
	}

	static inline int cellSpursJobQueuePushJob(CellSpursJobQueue *pJobQueue, CellSpursJobQueueHandle handle, CellSpursJobHeader *pJob, size_t sizeJobDesc, unsigned tag, CellSpursJobQueueSemaphore *pSemaphore)
	{
		return _cellSpursJobQueuePushJob2(pJobQueue, handle, pJob, sizeJobDesc, tag, pSemaphore, false, true);
	}

	static inline int cellSpursJobQueuePushExclusiveJob(CellSpursJobQueue *pJobQueue, CellSpursJobQueueHandle handle, CellSpursJobHeader *pJob, size_t sizeJobDesc, unsigned tag, CellSpursJobQueueSemaphore *pSemaphore)
	{
		return _cellSpursJobQueuePushJob2(pJobQueue, handle, pJob, sizeJobDesc, tag, pSemaphore, true, true);
	}

	static inline int cellSpursJobQueuePushJobList(CellSpursJobQueue *pJobQueue, CellSpursJobQueueHandle handle, CellSpursJobList *pJoblist, unsigned tag, CellSpursJobQueueSemaphore *pSemaphore)
	{
		return _cellSpursJobQueuePushJobList(pJobQueue, handle, pJoblist, tag, pSemaphore, true);
	}

	static inline int cellSpursJobQueueTryPush(CellSpursJobQueue *pJobQueue, CellSpursJobQueueHandle handle, CellSpursJobHeader *pJob, size_t sizeJobDesc, CellSpursJobQueueSemaphore *pSemaphore)
	{
		return _cellSpursJobQueuePushJob2(pJobQueue, handle, pJob, sizeJobDesc, 0, pSemaphore, false, false);
	}

	static inline int cellSpursJobQueueTryPushJob(CellSpursJobQueue *pJobQueue, CellSpursJobQueueHandle handle, CellSpursJobHeader *pJob, size_t sizeJobDesc, unsigned tag, CellSpursJobQueueSemaphore *pSemaphore)
	{
		return _cellSpursJobQueuePushJob2(pJobQueue, handle, pJob, sizeJobDesc, tag, pSemaphore, false, false);
	}

	static inline int cellSpursJobQueueTryPushExclusiveJob(CellSpursJobQueue *pJobQueue, CellSpursJobQueueHandle handle, CellSpursJobHeader *pJob, size_t sizeJobDesc, unsigned tag, CellSpursJobQueueSemaphore *pSemaphore)
	{
		return _cellSpursJobQueuePushJob2(pJobQueue, handle, pJob, sizeJobDesc, tag, pSemaphore, true, false);
	}

	static inline int cellSpursJobQueueTryPushJobList(CellSpursJobQueue *pJobQueue, CellSpursJobQueueHandle handle, CellSpursJobList *pJoblist, unsigned tag, CellSpursJobQueueSemaphore *pSemaphore)
	{
		return _cellSpursJobQueuePushJobList(pJobQueue, handle, pJoblist, tag, pSemaphore, false);
	}

	int _cellSpursJobQueuePushFlush(CellSpursJobQueue *pJobQueue, CellSpursJobQueueHandle handle, bool isBlocking);

	static inline int cellSpursJobQueuePushFlush(CellSpursJobQueue *pJobQueue, CellSpursJobQueueHandle handle)
	{
		return _cellSpursJobQueuePushFlush(pJobQueue, handle, true);
	}

	static inline int cellSpursJobQueueTryPushFlush(CellSpursJobQueue *pJobQueue, CellSpursJobQueueHandle handle)
	{
		return _cellSpursJobQueuePushFlush(pJobQueue, handle, false);
	}

	int _cellSpursJobQueuePushSync(CellSpursJobQueue *pJobQueue, CellSpursJobQueueHandle handle, unsigned tagMask, bool isBlocking);

	static inline int cellSpursJobQueuePushSync(CellSpursJobQueue *pJobQueue, CellSpursJobQueueHandle handle, unsigned tagMask)
	{
		return _cellSpursJobQueuePushSync(pJobQueue, handle, tagMask, true);
	}

	static inline int cellSpursJobQueueTryPushSync(CellSpursJobQueue *pJobQueue, CellSpursJobQueueHandle handle, unsigned tag)
	{
		return _cellSpursJobQueuePushSync(pJobQueue, handle, tag, false);
	}

	int cellSpursJobQueueSendSignal(CellSpursJobQueueWaitingJob *job);

	/* backward compatilibity */
	int _cellSpursJobQueuePushBody(CellSpursJobQueue *pJobQueue, CellSpursJobQueueHandle handle, CellSpursJobHeader *pJob, size_t sizeJobDesc, CellSpursJobQueueSemaphore *pSemaphore, bool isBlocking);

	int _cellSpursJobQueuePushJobBody(CellSpursJobQueue *pJobQueue, CellSpursJobQueueHandle handle, CellSpursJobHeader *pJob, size_t sizeJobDesc, unsigned tag, CellSpursJobQueueSemaphore *pSemaphore, bool isBlocking);


#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
/* C++ interfaces */


__CELL_SPURS_JOBQUEUE_BEGIN

/* job queue internal structure */
class JobQueueBase :public CellSpursJobQueue
{
public:
	static const size_t	kAlign = CELL_SPURS_JOBQUEUE_ALIGN;
	static const size_t	kSize = CELL_SPURS_JOBQUEUE_SIZE;

protected:
	JobQueueBase(const JobQueueBase&);
	JobQueueBase& operator = (const JobQueueBase&);

	static int create(
		JobQueueBase *pJobQueueBase,
		CellSpurs *pSpurs,
		const char *pName,
		uint64_t *pCommandList,
		unsigned int depth,
		unsigned int numSpus,
		const uint8_t priorityTable[CELL_SPURS_MAX_SPU],
		unsigned int maxSizeJobDescriptor,
		uint8_t	maxGrab,
		bool submitWithEntryLock,
		bool doBusyWaiting,
		bool isHaltOnError,
		uint8_t maxNumJobsOnASpu = CELL_SPURS_JOBQUEUE_DEFAULT_MAX_NUM_JOBS_ON_SPU,
		bool isJobTypeMemoryCheck = false)
	{
		CellSpursJobQueueAttribute attr;
		__CELL_SPURS_RETURN_IF(cellSpursJobQueueAttributeInitialize(&attr));
		__CELL_SPURS_RETURN_IF(cellSpursJobQueueAttributeSetGrabParameters(&attr, maxNumJobsOnASpu, maxGrab));
		__CELL_SPURS_RETURN_IF(cellSpursJobQueueAttributeSetMaxSizeJobDescriptor(&attr, maxSizeJobDescriptor));
		__CELL_SPURS_RETURN_IF(cellSpursJobQueueAttributeSetSubmitWithEntryLock(&attr, submitWithEntryLock));
		__CELL_SPURS_RETURN_IF(cellSpursJobQueueAttributeSetDoBusyWaiting(&attr, doBusyWaiting));
		__CELL_SPURS_RETURN_IF(cellSpursJobQueueAttributeSetIsHaltOnError(&attr, isHaltOnError));
		__CELL_SPURS_RETURN_IF(cellSpursJobQueueAttributeSetIsJobTypeMemoryCheck(&attr, isJobTypeMemoryCheck));
		return cellSpursCreateJobQueue(
			pSpurs,
			pJobQueueBase,
			&attr,
			pName,
			pCommandList,
			depth,
			numSpus,
			priorityTable);
	}

	JobQueueBase() {}

public:

	int shutdown()
	{
		return cellSpursShutdownJobQueue(this);
	}

	int join(int* exitCode)
	{
		return cellSpursJoinJobQueue(this, exitCode);
	}

	int pushFlush(CellSpursJobQueueHandle handle)
	{
		return cellSpursJobQueuePushFlush(this, handle);
	}

	int tryPushFlush(CellSpursJobQueueHandle handle)
	{
		return cellSpursJobQueueTryPushFlush(this, handle);
	}

	int pushSync(CellSpursJobQueueHandle handle, unsigned tagMask)
	{
		return cellSpursJobQueuePushSync(this, handle, tagMask);
	}

	int tryPushSync(CellSpursJobQueueHandle handle, unsigned tagMask)
	{
		return cellSpursJobQueueTryPushSync(this, handle, tagMask);
	}

	int push(CellSpursJobQueueHandle handle, CellSpursJobHeader *pJob, size_t sizeJobDesc, CellSpursJobQueueSemaphore *pSemaphore = NULL)
	{
		return cellSpursJobQueuePush(this, handle, pJob, sizeJobDesc, pSemaphore);
	}

	int tryPush(CellSpursJobQueueHandle handle, CellSpursJobHeader *pJob, size_t sizeJobDesc, CellSpursJobQueueSemaphore *pSemaphore = NULL)
	{
		return cellSpursJobQueueTryPush(this, handle, pJob, sizeJobDesc, pSemaphore);
	}

	int pushJob(CellSpursJobQueueHandle handle, CellSpursJobHeader *pJob, size_t sizeJobDesc, unsigned tag, CellSpursJobQueueSemaphore *pSemaphore = NULL)
	{
		return cellSpursJobQueuePushJob(this, handle, pJob, sizeJobDesc, tag, pSemaphore);
	}

	int tryPushJob(CellSpursJobQueueHandle handle, CellSpursJobHeader *pJob, size_t sizeJobDesc, unsigned tag, CellSpursJobQueueSemaphore *pSemaphore = NULL)
	{
		return cellSpursJobQueueTryPushJob(this, handle, pJob, sizeJobDesc, tag, pSemaphore);
	}

	int pushExclusiveJob(CellSpursJobQueueHandle handle, CellSpursJobHeader *pJob, size_t sizeJobDesc, unsigned tag, CellSpursJobQueueSemaphore *pSemaphore = NULL)
	{
		return cellSpursJobQueuePushExclusiveJob(this, handle, pJob, sizeJobDesc, tag, pSemaphore);
	}

	int tryPushExclusiveJob(CellSpursJobQueueHandle handle, CellSpursJobHeader *pJob, size_t sizeJobDesc, unsigned tag, CellSpursJobQueueSemaphore *pSemaphore = NULL)
	{
		return cellSpursJobQueueTryPushExclusiveJob(this, handle, pJob, sizeJobDesc, tag, pSemaphore);
	}

	int pushJobList(CellSpursJobQueueHandle handle, CellSpursJobList *pJobList, unsigned tag, CellSpursJobQueueSemaphore *pSemaphore = NULL)
	{
		return cellSpursJobQueuePushJobList(this, handle, pJobList, tag, pSemaphore);
	}

	int tryPushJobList(CellSpursJobQueueHandle handle, CellSpursJobList *pJobList, unsigned tag, CellSpursJobQueueSemaphore *pSemaphore = NULL)
	{
		return cellSpursJobQueueTryPushJobList(this, handle, pJobList, tag, pSemaphore);
	}

	//
	// Information
	//
	CellSpurs *getSpurs()
	{
		return cellSpursJobQueueGetSpurs(this);
	}

	unsigned int getHandleCount()
	{
		return cellSpursJobQueueGetHandleCount(this);
	}

	unsigned int getMaxSizeJobDescriptor()
	{
		return cellSpursJobQueueGetMaxSizeJobDescriptor(this);
	}

	int getId(CellSpursWorkloadId *pId)
	{
		return cellSpursGetJobQueueId(this, pId);
	}

	int getError(int *exitCode, void **cause)
	{
		return cellSpursJobQueueGetError(this, exitCode, cause);
	}

	int setWaitingMode(unsigned mode)
	{
		return cellSpursJobQueueSetWaitingMode(this, mode);
	}

	//
	// Exception Handling
	//
	int setExceptionEventHandler(CellSpursJobQueueExceptionEventHandler handler, void* argHandler)
	{
		return cellSpursJobQueueSetExceptionEventHandler(this, handler, argHandler);
	}

	int unsetExceptionEventHandler()
	{
		return cellSpursJobQueueUnsetExceptionEventHandler(this);
	}
};

/* JobQueue external interface */
template<unsigned int tDepth, unsigned int tMaxSizeJobDescriptor = 256> class JobQueue : public cell::Spurs::JobQueue::JobQueueBase
{
public:
	static const size_t	kSize = CELL_SPURS_JOBQUEUE_SIZE
			+ (CELL_SPURS_JOBQUEUE_SIZE_COMMAND_BUFFER(tDepth) + sizeof(uint32_t) + 127) & ~127;

private:
	// member variables
	uint64_t    mCommandList[
					CELL_SPURS_JOBQUEUE_SIZE_COMMAND_BUFFER(tDepth) /
						sizeof(uint64_t)];
	uint32_t    mAutoPrxLoad;
	uint8_t		__pad__[128
					- (CELL_SPURS_JOBQUEUE_SIZE_COMMAND_BUFFER(tDepth) + sizeof(uint32_t)) % 128];

public:
	static int create(JobQueue<tDepth,tMaxSizeJobDescriptor> *pJobQueue,
		CellSpurs *pSpurs,
		const char *pName,
		uint8_t	numSpus,
		const uint8_t priorityTable[CELL_SPURS_MAX_SPU],
		uint8_t	maxGrab = CELL_SPURS_JOBQUEUE_DEFAULT_MAX_GRAB,
		bool submitWithEntryLock = false,
		bool doBusyWaiting = false,
		bool isHaltOnError = false,
		bool autoPrxLoad = true,
		uint8_t	maxNumJobsOnASpu = CELL_SPURS_JOBQUEUE_DEFAULT_MAX_NUM_JOBS_ON_SPU,
		bool isJobTypeMemoryCheck = false)
	{
		if (autoPrxLoad)
		{
			__CELL_SPURS_RETURN_IF(cellSysmoduleLoadModule(CELL_SYSMODULE_SPURS_JQ));
		}
		int ret = cell::Spurs::JobQueue::JobQueueBase::create(
			pJobQueue,
			pSpurs,
			pName,
			pJobQueue->mCommandList, (tDepth + 15) & ~15,
			numSpus, priorityTable,
			tMaxSizeJobDescriptor, maxGrab,
			submitWithEntryLock, doBusyWaiting,
			isHaltOnError, maxNumJobsOnASpu, isJobTypeMemoryCheck);
		if (ret) {
			if (autoPrxLoad)
			{
				cellSysmoduleUnloadModule(CELL_SYSMODULE_SPURS_JQ);
			}
			return ret;
		}
		pJobQueue->mAutoPrxLoad = autoPrxLoad;
		return ret;
	}


	static int create(JobQueue<tDepth,tMaxSizeJobDescriptor> *pJobQueue,
		CellSpurs *pSpurs,
		const char *pName,
		uint8_t	numSpus,
		uint8_t priority,
		uint8_t	maxGrab = CELL_SPURS_JOBQUEUE_DEFAULT_MAX_GRAB,
		bool submitWithEntryLock = false,
		bool doBusyWaiting = false,
		bool isHaltOnError = false,
		bool autoPrxLoad = true,
		uint8_t	maxNumJobsOnASpu = CELL_SPURS_JOBQUEUE_DEFAULT_MAX_NUM_JOBS_ON_SPU,
		bool isJobTypeMemoryCheck = false)
	{
		uint8_t priorityTable[CELL_SPURS_MAX_SPU];
		for(unsigned int i = 0; i < CELL_SPURS_MAX_SPU; i++)
		{
			priorityTable[i] = priority;
		}
		return create(
			pJobQueue,
			pSpurs,
			pName,
			numSpus,
			priorityTable,
			maxGrab,
			submitWithEntryLock,
			doBusyWaiting,
			isHaltOnError,
			autoPrxLoad, 
			maxNumJobsOnASpu,
			isJobTypeMemoryCheck);
	}

	int join(int* exitCode)
	{
		int ret;
		__CELL_SPURS_RETURN_IF(cell::Spurs::JobQueue::JobQueueBase::join(exitCode));
		if (mAutoPrxLoad)
		{
			ret = cellSysmoduleUnloadModule(CELL_SYSMODULE_SPURS_JQ);
			assert(ret == CELL_OK);
		}

		return CELL_OK;
	}
};

__CELL_SPURS_JOBQUEUE_END

#endif /* #ifdef __cplusplus */

#endif /* __CELL_SPURS_JOBQUEUE_H__ */
