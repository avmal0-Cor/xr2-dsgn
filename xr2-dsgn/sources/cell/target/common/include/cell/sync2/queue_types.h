/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2009 Sony Computer Entertainment Inc.
 * All Rights Reserved.
 */

#ifndef __CELL_SYNC2_QUEUE_TYPES_H__
#define __CELL_SYNC2_QUEUE_TYPES_H__

#include <stdint.h>
#include <cell/sync2/types.h>

#define CELL_SYNC2_QUEUE_SIZE	128
#define CELL_SYNC2_QUEUE_ALIGN	128

typedef struct CellSync2Queue {
	uint8_t skip[CELL_SYNC2_QUEUE_SIZE];
} __attribute__((aligned(CELL_SYNC2_QUEUE_ALIGN))) CellSync2Queue;

#define CELL_SYNC2_QUEUE_ATTRIBUTE_SIZE		128
#define CELL_SYNC2_QUEUE_ATTRIBUTE_ALIGN	8

typedef struct CellSync2QueueAttribute {
	uint32_t sdkVersion;
	uint32_t threadTypes;
	size_t elementSize;
	uint32_t depth;
	uint16_t maxPushWaiters;
	uint16_t maxPopWaiters;
	char name[CELL_SYNC2_NAME_MAX_LENGTH+1];
	uint8_t reserved[CELL_SYNC2_QUEUE_ATTRIBUTE_SIZE
	                 - sizeof(uint32_t) * 2
	                 - sizeof(size_t)
	                 - sizeof(uint32_t)
	                 - sizeof(uint16_t) * 2
	                 - (CELL_SYNC2_NAME_MAX_LENGTH + 1)
	              ];
} __attribute__((aligned(CELL_SYNC2_QUEUE_ATTRIBUTE_ALIGN))) CellSync2QueueAttribute;

#define CELL_SYNC2_QUEUE_BUFFER_ALIGN	128

#endif /* __CELL_SYNC2_QUEUE_TYPES_H__ */
