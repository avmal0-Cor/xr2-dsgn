/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2009 Sony Computer Entertainment Inc.
 * All Rights Reserved.
 */

#ifndef __CELL_SYNC2_SEMAPHORE_H__
#define __CELL_SYNC2_SEMAPHORE_H__

#include <stdint.h>
#include <cell/sync2/thread.h>
#include <cell/sync2/semaphore_types.h>
#include <cell/sync2/error.h>

#ifdef __cplusplus
extern "C" {
#endif

int cellSync2SemaphoreAcquire(
		uint64_t eaSemaphore,
		unsigned int count,
		const CellSync2ThreadConfig* config,
		unsigned int dmaTag
	);

int cellSync2SemaphoreTryAcquire(
		uint64_t eaSemaphore,
		unsigned int count,
		const CellSync2ThreadConfig* config,
		unsigned int dmaTag
	);

int cellSync2SemaphoreRelease(
		uint64_t eaSemaphore,
		unsigned int count,
		const CellSync2ThreadConfig* config,
		unsigned int dmaTag
	);

int cellSync2SemaphoreGetCount(
		uint64_t eaSemaphore,
		int* pCount
	);

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* __CELL_SYNC2_SEMAPHORE_H__ */
