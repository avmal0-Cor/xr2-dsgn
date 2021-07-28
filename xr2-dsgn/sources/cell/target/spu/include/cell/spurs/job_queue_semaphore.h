/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2008 Sony Computer Entertainment Inc.
 * All Rights Reserved.
 */

#ifndef __CELL_SPURS_JOBQUEUE_SEMAPHORE_H__
#define __CELL_SPURS_JOBQUEUE_SEMAPHORE_H__

/* standard C++ header */
#include <stdint.h>

/* job queue */
#include <cell/spurs/job_queue_types.h>

#ifdef __cplusplus
extern "C" {
#endif

	int cellSpursJobQueueSemaphoreInitialize(uint32_t eaSemaphore, uint32_t eaJobQueue);
	int cellSpursJobQueueSemaphoreAcquire(uint32_t eaSemaphore, unsigned int acquireCount);
	int cellSpursJobQueueSemaphoreTryAcquire(uint32_t eaSemaphore, unsigned int acquireCount);

#ifdef __cplusplus
}
#endif

#endif /* __CELL_SPURS_JOBQUEUE_SEMAPHORE_H__ */
