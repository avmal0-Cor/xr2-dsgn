/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2009 Sony Computer Entertainment Inc.
 * All Rights Reserved.
 */

#ifndef __CELL_SYNC2_MUTEX_H__
#define __CELL_SYNC2_MUTEX_H__

#include <stddef.h>
#include <stdint.h>
#include <cell/sync2/thread.h>
#include <cell/sync2/mutex_types.h>
#include <cell/sync2/error.h>
#include <cell/sync2/version.h>

#ifdef __cplusplus
extern "C" {
#endif

int _cellSync2MutexAttributeInitialize(
		CellSync2MutexAttribute* attr,
		uint32_t sdkVersion
	);

static inline
int cellSync2MutexAttributeInitialize(
		CellSync2MutexAttribute* attr
	)
{
	return _cellSync2MutexAttributeInitialize(
			attr,
			_CELL_SYNC2_INTERNAL_VERSION
		);
}

int cellSync2MutexEstimateBufferSize(
		const CellSync2MutexAttribute* attr,
		size_t* bufferSize
	);

int cellSync2MutexInitialize(
		CellSync2Mutex* mutex,
		void* buffer,
		const CellSync2MutexAttribute* attr
	);

int cellSync2MutexFinalize(CellSync2Mutex* mutex);

int cellSync2MutexLock(CellSync2Mutex* mutex, const CellSync2ThreadConfig* config);

int cellSync2MutexTryLock(CellSync2Mutex* mutex, const CellSync2ThreadConfig* config);

int cellSync2MutexUnlock(
		CellSync2Mutex* mutex,
		const CellSync2ThreadConfig* config
	);

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* __CELL_SYNC2_MUTEX_H__ */
