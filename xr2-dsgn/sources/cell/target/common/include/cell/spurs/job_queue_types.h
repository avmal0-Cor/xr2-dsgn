/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2009 Sony Computer Entertainment Inc.
 * All Rights Reserved.
 */

#ifndef __CELL_SPURS_JOBQUEUE_TYPES_H__
#define __CELL_SPURS_JOBQUEUE_TYPES_H__

// standard C++ header
#include <stdint.h>

#include <cell/trace/trace_types.h>
#include <cell/spurs/job_descriptor.h>
#include <cell/spurs/types.h>
#include <cell/spurs/exception_common_types.h>

#ifdef __cplusplus
extern "C" {
#endif

#define CELL_SPURS_JOBQUEUE_REVISION 2
#define CELL_SPURS_JOBQUEUE_CLASS_NAME "JobQueue"

// constants
#define CELL_SPURS_JOBQUEUE_MAX_DEPTH (1<<15)
#define CELL_SPURS_JOBQUEUE_MAX_SYNC_JOBS	(1 << 15)
#define CELL_SPURS_JOBQUEUE_MAX_SIZE_JOB_MEMORY (221 * 1024)
#define	CELL_SPURS_JOBQUEUE_MAX_NUM_MAX_GRAB	16

#define CELL_SPURS_JOBQUEUE_MAX_CLIENTS	256

#define	CELL_SPURS_JOBQUEUE_MAX_HANDLE	1024
#define	CELL_SPURS_JOBQUEUE_HANDLE_INVALID	(-1)

#define	CELL_SPURS_JOBQUEUE_MAX_TAG	15

#define	CELL_SPURS_JOBQUEUE_SIZE_COMMAND_BUFFER(x)\
	((CELL_SPURS_JOBQUEUE_MAX_CLIENTS + ((x + 15) & ~15)) * sizeof(uint64_t))

#define	CELL_SPURS_JOBQUEUE_COMMAND_BUFFER_ALIGN	128

enum CellSpursJobQueueWaitingMode {
	CELL_SPURS_JOBQUEUE_WAITING_MODE_SLEEP = 0,
	CELL_SPURS_JOBQUEUE_WAITING_MODE_BUSY =	1
};

enum CellSpursJobQueueSuspendedJobAttribute {
	CELL_SPURS_JOBQUEUE_JOB_SAVE_ALL = 0,
	CELL_SPURS_JOBQUEUE_JOB_SAVE_WRITABLE = 1
};

/* default parammeters */
#define CELL_SPURS_JOBQUEUE_DEFAULT_MAX_GRAB 4
#define CELL_SPURS_JOBQUEUE_DEFAULT_MAX_NUM_JOBS_ON_SPU 255

/* type definitions */
#define CELL_SPURS_JOBQUEUE_ALIGN 128
#define CELL_SPURS_JOBQUEUE_SIZE (128 * 16 + 2048)

	typedef struct CellSpursJobQueue
	{
		unsigned char skip[CELL_SPURS_JOBQUEUE_SIZE];
	} __attribute__((aligned(CELL_SPURS_JOBQUEUE_ALIGN))) CellSpursJobQueue;

	typedef int32_t CellSpursJobQueueHandle;

#define CELL_SPURS_JOBQUEUE_SEMAPHORE_MAX_ACQUIRE_COUNT ((1<<(32-4))-1)
#define CELL_SPURS_JOBQUEUE_SEMAPHROE_MAX_ACQUIRE_COUNT	CELL_SPURS_JOBQUEUE_SEMAPHORE_MAX_ACQUIRE_COUNT

#define CELL_SPURS_JOBQUEUE_SEMAPHORE_SIZE 128
#define CELL_SPURS_JOBQUEUE_SEMAPHORE_ALIGN 128

	typedef struct CellSpursJobQueueSemaphore {
		unsigned char skip[CELL_SPURS_JOBQUEUE_SEMAPHORE_SIZE];
	} __attribute__((aligned(CELL_SPURS_JOBQUEUE_SEMAPHORE_ALIGN))) CellSpursJobQueueSemaphore;

#define CELL_SPURS_JOBQUEUE_WAITING_JOB_SIZE	128
#define CELL_SPURS_JOBQUEUE_WAITING_JOB_ALIGN	128

	typedef struct CellSpursJobQueueWaitingJob {
		unsigned char skip[CELL_SPURS_JOBQUEUE_WAITING_JOB_SIZE];
	} __attribute__((aligned(CELL_SPURS_JOBQUEUE_WAITING_JOB_ALIGN))) CellSpursJobQueueWaitingJob;

#define CELL_SPURS_JOBQUEUE_ATTRIBUTE_ALIGN 8
#define CELL_SPURS_JOBQUEUE_ATTRIBUTE_SIZE 512

	typedef struct CellSpursJobQueueAttribute
	{
		unsigned char skip[CELL_SPURS_JOBQUEUE_ATTRIBUTE_SIZE];
	} __attribute__((aligned(CELL_SPURS_JOBQUEUE_ATTRIBUTE_ALIGN))) CellSpursJobQueueAttribute;

#define CELL_SPURS_JOBQUEUE_SUSPENDED_JOB_HEADER_ALIGN 128
#define CELL_SPURS_JOBQUEUE_SUSPENDED_JOB_HEADER_SIZE (128*10)

	typedef struct CellSpursJobQueueSuspendedJobHeader
	{
		unsigned char skip[CELL_SPURS_JOBQUEUE_SUSPENDED_JOB_HEADER_SIZE];
	} __attribute__((aligned(CELL_SPURS_JOBQUEUE_SUSPENDED_JOB_HEADER_ALIGN))) CellSpursJobQueueSuspendedJobHeader;

#define CELL_SPURS_JOBQUEUE_SUSPENDED_JOB_ALIGN 128
#define CELL_SPURS_JOBQUEUE_SUSPENDED_JOB_SIZE (256*1024)

	typedef struct CellSpursJobQueueSuspendedJob
	{
		CellSpursJobQueueWaitingJob	job;
		unsigned char skip[CELL_SPURS_JOBQUEUE_SUSPENDED_JOB_SIZE -
						   CELL_SPURS_JOBQUEUE_WAITING_JOB_SIZE];
	} __attribute__((aligned(CELL_SPURS_JOBQUEUE_SUSPENDED_JOB_ALIGN))) CellSpursJobQueueSuspendedJob;

	typedef struct _CellSpursJobQueueTracePacket {
		CellTraceHeader header;
		unsigned char skip[8];
	} __attribute__((aligned(16))) _CellSpursJobQueueTracePacket;


	static inline int cellSpursJobQueueCheckJob(const CellSpursJob256 *pJob, unsigned int sizeJob, unsigned int maxSizeJob)
	{
		if (__builtin_expect(maxSizeJob < 256 || maxSizeJob >= 1024 || (maxSizeJob%128), 0))
		{
			return CELL_SPURS_JOB_ERROR_INVAL;
		}
		return _cellSpursCheckJob(pJob, sizeJob, maxSizeJob, CELL_SPURS_JOBQUEUE_MAX_SIZE_JOB_MEMORY, 128, 0);
	}

	typedef struct CellSpursJobQueuePipelineInfo {
		struct  {
			CellSpursJob256* job;
			size_t sizeJob;
			unsigned int dmaTag;
		} fetchStage;
		struct  {
			CellSpursJob256* job;
			size_t sizeJob;
			unsigned int dmaTag;
		} inputStage;
		struct  {
			CellSpursJob256* job;
			size_t sizeJob;
			uint32_t lsaJobContext;
			unsigned int dmaTag;
		} executeStage[2];
		struct {
			CellSpursJob256* job;
			size_t sizeJob;
			unsigned int dmaTag;
		} outputStage;
	} CellSpursJobQueuePipelineInfo;

	typedef void (*CellSpursJobQueueExceptionEventHandler)(
				CellSpurs* spurs,
				CellSpursJobQueue* jobQueue,
				const CellSpursExceptionInfo* exceptionInfo,
				const CellSpursJobQueuePipelineInfo* pipelineInfo,
				void* arg
			);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* __CELL_SPURS_JOBQUEUE_TYPES_H__ */
