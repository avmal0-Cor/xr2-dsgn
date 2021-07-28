/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2009 Sony Computer Entertainment Inc.
 * All Rights Reserved.
 */

#ifndef __CELL_SYNC2_QUEUE_H__
#define __CELL_SYNC2_QUEUE_H__

#include <stddef.h>
#include <stdint.h>
#include <cell/sync2/thread.h>
#include <cell/sync2/queue_types.h>
#include <cell/sync2/error.h>

#ifdef __cplusplus
extern "C" {
#endif

int cellSync2QueuePush(
		uint64_t eaQueue,
		const void* data,
		const CellSync2ThreadConfig* config,
		unsigned int dmaTag
	);

int cellSync2QueueTryPush(
		uint64_t eaQueue,
		const void* data,
		const CellSync2ThreadConfig* config,
		unsigned int dmaTag
	);

int cellSync2QueuePop(
		uint64_t eaQueue,
		void* buffer,
		const CellSync2ThreadConfig* config,
		unsigned int dmaTag
	);

int cellSync2QueueTryPop(
		uint64_t eaQueue,
		void* buffer,
		const CellSync2ThreadConfig* config,
		unsigned int dmaTag
	);

int cellSync2QueueGetSize(
		uint64_t eaQueue,
		unsigned int* size
	);

int cellSync2QueueGetDepth(
		uint64_t eaQueue,
		unsigned int* depth
	);

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* __CELL_SYNC2_QUEUE_H__ */
