/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2008 Sony Computer Entertainment Inc.
 * All Rights Reserved.
 */

#ifndef __CELL_SPURS_JOBQUEUE_SEMAPHORE_H__
#define __CELL_SPURS_JOBQUEUE_SEMAPHORE_H__

#include <cell/spurs/job_queue_types.h>

#ifdef __cplusplus
extern "C" {
#endif
	int cellSpursJobQueueSemaphoreInitialize(CellSpursJobQueueSemaphore *pSemaphore, CellSpursJobQueue* pJobQueue);
	int cellSpursJobQueueSemaphoreAcquire(CellSpursJobQueueSemaphore *pSemaphore, unsigned int acquireCount);
	int cellSpursJobQueueSemaphoreTryAcquire(CellSpursJobQueueSemaphore *pSemaphore, unsigned int acquireCount);
	int cellSpursJobQueueSemaphoreFinalize(CellSpursJobQueueSemaphore *pSemaphore);

	/* */
	int cellSpursJobQueueSemaphoreRelease(CellSpursJobQueueSemaphore *pSemaphore, unsigned int releaseCount);

#ifdef __cplusplus
}
#endif

#endif /* __CELL_SPURS_JOBQUEUE_SEMAPHORE_H__ */
