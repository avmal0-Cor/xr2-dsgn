/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2009 Sony Computer Entertainment Inc.
 * All Rights Reserved.
 */

#ifndef __CELL_SYNC2_MUTEX_H__
#define __CELL_SYNC2_MUTEX_H__

#include <stddef.h>
#include <stdint.h>
#include <sdk_version.h>
#include <cell/sync2/thread.h>
#include <cell/sync2/mutex_types.h>
#include <cell/sync2/error.h>

#ifdef __cplusplus
extern "C" {
#endif

int cellSync2MutexLock(
		uint64_t eaMutex,
		const CellSync2ThreadConfig* config,
		unsigned int dmaTag
	);

int cellSync2MutexTryLock(
		uint64_t eaMutex,
		const CellSync2ThreadConfig* config,
		unsigned int dmaTag
	);

int cellSync2MutexUnlock(
		uint64_t eaMutex,
		const CellSync2ThreadConfig* config,
		unsigned int dmaTag
	);

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* __CELL_SYNC2_MUTEX_H__ */
