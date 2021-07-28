/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2009 Sony Computer Entertainment Inc.
 * All Rights Reserved.
 */

#ifndef __CELL_SPURS_JOBQUEUE_H__
#define __CELL_SPURS_JOBQUEUE_H__

/* standard C++ header */
#include <stdint.h>

#include <spu_intrinsics.h>

/* dma */
#include <cell/dma.h>

/* spurs */
#include <cell/spurs/types.h>
#include <cell/spurs/job_descriptor.h>
#include <cell/spurs/job_context.h>
#include <cell/spurs/trace.h>

/* job queue */
#include <cell/spurs/job_queue_define.h>
#include <cell/spurs/job_queue_types.h>

#ifdef __cplusplus
extern "C" {
#endif
	/* Information */
	uint64_t cellSpursJobQueueGetSpurs(uint64_t eaJobQueue);
	unsigned int cellSpursJobQueueGetHandleCount(uint64_t eaJobQueue);
	int	cellSpursJobQueueGetError(uint64_t eaJobQueue,
				int* exitCode, void** cause);
	unsigned int cellSpursJobQueueGetMaxSizeJobDescriptor(uint64_t eaJobQueue);
	int cellSpursGetJobQueueId(uint64_t eaJobQueue, CellSpursWorkloadId *pId);
	int cellSpursJobQueueGetSuspendedJobSize(const CellSpursJobHeader *pJob, size_t sizeJobDesc, enum CellSpursJobQueueSuspendedJobAttribute attr, unsigned int *pSize);

	int cellSpursJobQueueOpen(uint64_t eaJobQueue, CellSpursJobQueueHandle *handle);
	int cellSpursJobQueueClose(uint64_t eaJobQueue, CellSpursJobQueueHandle handle);

	int	_cellSpursJobQueuePushJobBody(uint64_t eaJobQueue, CellSpursJobQueueHandle handle, uint64_t eaJobHeader, size_t sizeJobDesc, unsigned tag, unsigned dmaTag, uint64_t eaSemaphore, unsigned isExclusive, unsigned isBlocking);

	int	_cellSpursJobQueuePushJobListBody(uint64_t eaJobQueue, CellSpursJobQueueHandle handle, uint64_t eaJobList, unsigned tag, unsigned dmaTag, uint64_t eaSemaphore, unsigned isBlocking);

	static inline int	_cellSpursJobQueuePushJob(uint64_t eaJobQueue, CellSpursJobQueueHandle handle, uint64_t eaJobHeader, size_t sizeJobDesc, unsigned tag, unsigned dmaTag, uint64_t eaSemaphore, unsigned isExclusive, unsigned isBlocking) {
#ifdef CELL_SPURS_JOBDESCRIPTOR_ERROR_CHECK
		char jobHeader[1024];
		cellDmaGet(jobHeader, eaJobHeader, sizeJobDesc, dmaTag, 0, 0); cellDmaWaitTagStatusAll(1<<dmaTag);
		__CELL_SPURS_RETURN_IF(cellSpursJobQueueCheckJob((const CellSpursJob256 *)(uintptr_t)jobHeader, sizeJobDesc, cellSpursJobQueueGetMaxSizeJobDescriptor(eaJobQueue)));
#endif
		return _cellSpursJobQueuePushJobBody(eaJobQueue, handle, eaJobHeader, sizeJobDesc, tag, dmaTag, eaSemaphore, isExclusive, isBlocking);
	}

	static inline int	_cellSpursJobQueuePushJobList(uint64_t eaJobQueue, CellSpursJobQueueHandle handle, uint64_t eaJobList, unsigned tag, unsigned dmaTag, uint64_t eaSemaphore, unsigned isBlocking) {
#ifdef CELL_SPURS_JOBDESCRIPTOR_ERROR_CHECK
		CellSpursJobList	joblist;
		cellDmaGet(&joblist, eaJobList, sizeof(CellSpursJobList), dmaTag, 0, 0);
		cellDmaWaitTagStatusAll(1 << dmaTag);
		unsigned	maxSizeOfJobDescriptor = cellSpursJobQueueGetMaxSizeJobDescriptor(eaJobQueue);
		for(unsigned index = 0; index < joblist.numJobs; index++) {
			char jobHeader[1024];
			uint64_t	eaJobHeader = joblist.eaJobList 
										+ joblist.sizeOfJob * index;
			cellDmaGet(jobHeader, eaJobHeader, joblist.sizeOfJob, dmaTag, 0, 0); cellDmaWaitTagStatusAll(1<<dmaTag);
			__CELL_SPURS_RETURN_IF(cellSpursJobQueueCheckJob((const CellSpursJob256 *)(uintptr_t)jobHeader, joblist.sizeOfJob, maxSizeOfJobDescriptor));
		}
#endif
		return _cellSpursJobQueuePushJobListBody(eaJobQueue, handle, eaJobList, tag, dmaTag, eaSemaphore, isBlocking);
	}

	static inline int	cellSpursJobQueuePush(uint64_t eaJobQueue, CellSpursJobQueueHandle handle, uint64_t eaJobHeader, size_t sizeJobDesc, unsigned dmaTag, uint64_t eaSemaphore) {
		return _cellSpursJobQueuePushJob(eaJobQueue, handle, eaJobHeader, sizeJobDesc, 0, dmaTag, eaSemaphore, 0, 1);
	}

	static inline int	cellSpursJobQueuePushJob(uint64_t eaJobQueue, CellSpursJobQueueHandle handle, uint64_t eaJobHeader, size_t sizeJobDesc, unsigned tag, unsigned dmaTag, uint64_t eaSemaphore) {
		return _cellSpursJobQueuePushJob(eaJobQueue, handle, eaJobHeader, sizeJobDesc, tag, dmaTag, eaSemaphore, 0, 1);
	}

	static inline int	cellSpursJobQueueTryPushJob(uint64_t eaJobQueue, CellSpursJobQueueHandle handle, uint64_t eaJobHeader, size_t sizeJobDesc, unsigned tag, unsigned dmaTag, uint64_t eaSemaphore) {
		return _cellSpursJobQueuePushJob(eaJobQueue, handle, eaJobHeader, sizeJobDesc, tag, dmaTag, eaSemaphore, 0, 0);
	}

	static inline int	cellSpursJobQueuePushExclusiveJob(uint64_t eaJobQueue, CellSpursJobQueueHandle handle, uint64_t eaJobHeader, size_t sizeJobDesc, unsigned tag, unsigned dmaTag, uint64_t eaSemaphore) {
		return _cellSpursJobQueuePushJob(eaJobQueue, handle, eaJobHeader, sizeJobDesc, tag, dmaTag, eaSemaphore, 1, 1);
	}

	static inline int	cellSpursJobQueueTryPushExclusiveJob(uint64_t eaJobQueue, CellSpursJobQueueHandle handle, uint64_t eaJobHeader, size_t sizeJobDesc, unsigned tag, unsigned dmaTag, uint64_t eaSemaphore) {
		return _cellSpursJobQueuePushJob(eaJobQueue, handle, eaJobHeader, sizeJobDesc, tag, dmaTag, eaSemaphore, 1, 0);
	}

	static inline int	cellSpursJobQueuePushJobList(uint64_t eaJobQueue, CellSpursJobQueueHandle handle, uint64_t eaJobList, unsigned tag, unsigned dmaTag, uint64_t eaSemaphore) {
		return _cellSpursJobQueuePushJobList(eaJobQueue, handle, eaJobList, tag, dmaTag, eaSemaphore, 1);
	}

	static inline int	cellSpursJobQueueTryPushJobList(uint64_t eaJobQueue, CellSpursJobQueueHandle handle, uint64_t eaJobList, unsigned tag, unsigned dmaTag, uint64_t eaSemaphore) {
		return _cellSpursJobQueuePushJobList(eaJobQueue, handle, eaJobList, tag, dmaTag, eaSemaphore, 0);
	}

	int	_cellSpursJobQueuePushFlush(uint64_t eaJobQueue, CellSpursJobQueueHandle handle, unsigned dmaTag, unsigned isBlocking);

	static inline int	cellSpursJobQueuePushFlush(uint64_t eaJobQueue, CellSpursJobQueueHandle handle, unsigned dmaTag) {
		return _cellSpursJobQueuePushFlush(eaJobQueue, handle, dmaTag, 1);
	}

	static inline int	cellSpursJobQueueTryPushFlush(uint64_t eaJobQueue, CellSpursJobQueueHandle handle, unsigned dmaTag) {
		return _cellSpursJobQueuePushFlush(eaJobQueue, handle, dmaTag, 0);
	}

	int	_cellSpursJobQueuePushSync(uint64_t eaJobQueue, CellSpursJobQueueHandle handle, unsigned tagMask, unsigned dmaTag, unsigned isBlocking);

	static inline int	cellSpursJobQueuePushSync(uint64_t eaJobQueue, CellSpursJobQueueHandle handle, unsigned tagMask, unsigned dmaTag) {
		return _cellSpursJobQueuePushSync(eaJobQueue, handle, tagMask, dmaTag, 1);
	}

	static inline int	cellSpursJobQueueTryPushSync(uint64_t eaJobQueue, CellSpursJobQueueHandle handle, unsigned tagMask, unsigned dmaTag) {
		return _cellSpursJobQueuePushSync(eaJobQueue, handle, tagMask, dmaTag, 0);
	}
	int	cellSpursJobQueueSendSignal(uint64_t eaJob);

	//
	// APIs for job
	//
	void cellSpursJobQueueMain(CellSpursJobContext2 *pContext, CellSpursJob256 *pJob);
	int cellSpursJobQueueWaitSignal(uint64_t eaSuspendedJob);
	int cellSpursJobQueueWaitSignal2(uint64_t eaSuspendedJob2, enum CellSpursJobQueueSuspendedJobAttribute attr);

	// trace
	extern unsigned int _gCellSpursJobQueueIsTraceEnabled;
	extern _CellSpursJobQueueTracePacket _gCellSpursJobQueueTracePacketYield;
	extern _CellSpursJobQueueTracePacket _gCellSpursJobQueueTracePacketSleep;
	extern _CellSpursJobQueueTracePacket _gCellSpursJobQueueTracePacketResume;

	extern unsigned int _gCellSpursJobQueueTraceRemainingCount;
	extern _CellSpursJobQueueTracePacket *_gCellSpursJobQueueTracePutPtr;
	extern void _cellSpursJobQueueTraceDump(void);

#define __CELL_SPURS_JOBQUEUE_TRACE_PUT__(packet, tb) \
	if (__builtin_expect(_gCellSpursJobQueueTraceRemainingCount, 1)) \
	{ \
		_gCellSpursJobQueueTraceRemainingCount--; \
	} else { \
		_cellSpursJobQueueTraceDump(); \
		_gCellSpursJobQueueTraceRemainingCount--; \
	} \
	packet.header.time = tb; \
	*_gCellSpursJobQueueTracePutPtr++ = packet;

	// function pointer to yield
	extern vec_uint4 (*_gfpCellSpursJobQueueYield)(uint32_t, uint32_t);

	// memory check on SPU fiber switch
	void _cellSpursJobQueueHashCheck(void);
	void _cellSpursJobQueueRehash(void);

	extern unsigned int _gCellSpursJobQueueIsMemCheckEnabled __attribute__((aligned(16)));
	extern unsigned int _gCellSpursJobQueueYieldHasAuxProc __attribute__((aligned(16)));
	static inline uint32_t cellSpursJobQueueDmaWaitTagStatusAll(unsigned int dmaTagMask)
	{
		register vec_uint4 (*fpYield)(uint32_t,uint32_t);
		fpYield = _gfpCellSpursJobQueueYield;
		register unsigned int compDmaTagMask = cellDmaWaitTagStatusImmediate(0xffffffff);
		if (__builtin_expect((compDmaTagMask | ~dmaTagMask) + 1, 1))
		{
			if (__builtin_expect(!_gCellSpursJobQueueYieldHasAuxProc, 1))
			{
				register vec_uint4 v_ret = (*fpYield)(compDmaTagMask, 0);
				compDmaTagMask = spu_extract(v_ret, 0);
				if (__builtin_expect((compDmaTagMask | ~dmaTagMask) + 1, 0))
				{
					return cellDmaWaitTagStatusAll(dmaTagMask);
				}
			} else {
				register uint32_t timebase = 0; (void)timebase;
				if (__builtin_expect(_gCellSpursJobQueueIsMemCheckEnabled, 0))
				{
					_cellSpursJobQueueHashCheck();
				}
				if (_gCellSpursJobQueueIsTraceEnabled) {
					timebase = -spu_read_decrementer();
					__CELL_SPURS_JOBQUEUE_TRACE_PUT__(_gCellSpursJobQueueTracePacketYield, timebase);
				}
				register vec_uint4 v_ret = (*fpYield)(compDmaTagMask, timebase);
				if (_gCellSpursJobQueueIsTraceEnabled) {
					timebase = spu_extract(v_ret, 1);
					__CELL_SPURS_JOBQUEUE_TRACE_PUT__(_gCellSpursJobQueueTracePacketResume, timebase + 2);
				}
				if (__builtin_expect(_gCellSpursJobQueueIsMemCheckEnabled, 0))
				{
					_cellSpursJobQueueRehash();
				}
				compDmaTagMask = spu_extract(v_ret, 0);
				if (__builtin_expect((compDmaTagMask | ~dmaTagMask) + 1, 0))
				{
					return cellDmaWaitTagStatusAll(dmaTagMask);
				}
			}
		}
		return compDmaTagMask & dmaTagMask;
	}

	static inline uint32_t cellSpursJobQueueDmaWaitTagStatusAny(unsigned int dmaTagMask)
	{
		register unsigned int compDmaTagMask = cellDmaWaitTagStatusImmediate(0xffffffff);
		if (__builtin_expect((compDmaTagMask & dmaTagMask) == 0, 1))
		{
			if (__builtin_expect(!_gCellSpursJobQueueYieldHasAuxProc, 1))
			{
				register vec_uint4 v_ret = (*_gfpCellSpursJobQueueYield)(compDmaTagMask, 0);
				compDmaTagMask = spu_extract(v_ret, 0);
				if (__builtin_expect((compDmaTagMask & dmaTagMask) == 0, 0))
				{
					return cellDmaWaitTagStatusAny(dmaTagMask);
				}
			} else {
				register uint32_t timebase;
				if (__builtin_expect(_gCellSpursJobQueueIsMemCheckEnabled, 0))
				{
					_cellSpursJobQueueHashCheck();
				}
				if (_gCellSpursJobQueueIsTraceEnabled) {
					timebase = -spu_read_decrementer();
					__CELL_SPURS_JOBQUEUE_TRACE_PUT__(_gCellSpursJobQueueTracePacketYield, timebase);
				}
				register vec_uint4 v_ret = (*_gfpCellSpursJobQueueYield)(compDmaTagMask, timebase);
				if (_gCellSpursJobQueueIsTraceEnabled) {
					timebase = spu_extract(v_ret, 1);
					__CELL_SPURS_JOBQUEUE_TRACE_PUT__(_gCellSpursJobQueueTracePacketResume, timebase + 2);
				}
				if (__builtin_expect(_gCellSpursJobQueueIsMemCheckEnabled, 0))
				{
					_cellSpursJobQueueRehash();
				}
				compDmaTagMask = spu_extract(v_ret, 0);
				if (__builtin_expect((compDmaTagMask & dmaTagMask) == 0, 0))
				{
					return cellDmaWaitTagStatusAny(dmaTagMask);
				}
			}
		}
		return compDmaTagMask & dmaTagMask;
	}

#ifdef __cplusplus
}
#endif

#endif /* __CELL_SPURS_JOBQUEUE_H__ */
