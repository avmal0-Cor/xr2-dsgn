/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2009 Sony Computer Entertainment Inc.
 * All Rights Reserved.
 */

#ifndef __CELL_SYNC2_SEMAPHORE_H__
#define __CELL_SYNC2_SEMAPHORE_H__

#include <stddef.h>
#include <stdint.h>
#include <cell/sync2/thread.h>
#include <cell/sync2/semaphore_types.h>
#include <cell/sync2/error.h>
#include <cell/sync2/version.h>

#ifdef __cplusplus
extern "C" {
#endif

int _cellSync2SemaphoreAttributeInitialize(
		CellSync2SemaphoreAttribute* attr,
		uint32_t sdkVersion
	);

static inline
int cellSync2SemaphoreAttributeInitialize(
		CellSync2SemaphoreAttribute* attr
	)
{
	return _cellSync2SemaphoreAttributeInitialize(
			attr,
			_CELL_SYNC2_INTERNAL_VERSION
		);
}

int cellSync2SemaphoreEstimateBufferSize(
		const CellSync2SemaphoreAttribute* attr,
		size_t* bufferSize
	);

int cellSync2SemaphoreInitialize(
		CellSync2Semaphore* semaphore,
		void* buffer,
		int initialValue,
		const CellSync2SemaphoreAttribute* attr
	);

int cellSync2SemaphoreFinalize(CellSync2Semaphore* semaphore);

int cellSync2SemaphoreAcquire(
		CellSync2Semaphore* semaphore,
		unsigned int count,
		const CellSync2ThreadConfig* config
	);

int cellSync2SemaphoreTryAcquire(
		CellSync2Semaphore* semaphore,
		unsigned int count,
		const CellSync2ThreadConfig* config
	);

int cellSync2SemaphoreRelease(
		CellSync2Semaphore* semaphore,
		unsigned int count,
		const CellSync2ThreadConfig* config
	);

int cellSync2SemaphoreGetCount(
		CellSync2Semaphore* semaphore,
		int* pCount
	);

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* __CELL_SYNC2_SEMAPHORE_H__ */
