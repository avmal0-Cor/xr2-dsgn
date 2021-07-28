/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2009 Sony Computer Entertainment Inc.
 * All Rights Reserved.
 */

#ifndef __CELL_SYNC2_COND_H__
#define __CELL_SYNC2_COND_H__

#include <cell/sync2/mutex.h>
#include <cell/sync2/cond_types.h>
#include <cell/sync2/error.h>
#include <cell/sync2/version.h>

#ifdef __cplusplus
extern "C" {
#endif

int _cellSync2CondAttributeInitialize(CellSync2CondAttribute* attr, uint32_t sdkVersion);

static inline
int cellSync2CondAttributeInitialize(CellSync2CondAttribute* attr) {
	return _cellSync2CondAttributeInitialize(attr, _CELL_SYNC2_INTERNAL_VERSION);
}

int cellSync2CondEstimateBufferSize(
		const CellSync2CondAttribute* attr,
		size_t* bufferSize
	);

int cellSync2CondInitialize(
		CellSync2Cond* cond,
		CellSync2Mutex* mutex,
		void* waitingQueueBuffer,
		const CellSync2CondAttribute* attr
	);

int cellSync2CondFinalize(CellSync2Cond* cond);

int cellSync2CondWait(
		CellSync2Cond* cond,
		const CellSync2ThreadConfig* config
	);

int cellSync2CondSignal(
		CellSync2Cond* cond,
		const CellSync2ThreadConfig* config
	);

int cellSync2CondSignalAll(
		CellSync2Cond* cond,
		const CellSync2ThreadConfig* config
	);

#ifdef __cplusplus
} // extern "C"
#endif


#endif /* __CELL_SYNC2_COND_H__ */
