/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2009 Sony Computer Entertainment Inc.
 * All Rights Reserved.
 */

#ifndef CELL_SYNC2_MUTEX_TYPES_H
#define CELL_SYNC2_MUTEX_TYPES_H

#include <stdint.h>
#include <stdbool.h>
#include <cell/sync2/types.h>

#define CELL_SYNC2_MUTEX_SIZE	128
#define CELL_SYNC2_MUTEX_ALIGN	128

typedef struct CellSync2Mutex {
	uint8_t skip[CELL_SYNC2_MUTEX_SIZE];
} __attribute__((aligned(CELL_SYNC2_MUTEX_ALIGN))) CellSync2Mutex;


#define CELL_SYNC2_MUTEX_ATTRIBUTE_SIZE		128
#define CELL_SYNC2_MUTEX_ATTRIBUTE_ALIGN	8

typedef struct CellSync2MutexAttribute {
	uint32_t sdkVersion;
	uint16_t threadTypes;
	uint16_t maxWaiters;
	bool recursive;
	uint8_t	padding;
	char name[CELL_SYNC2_NAME_MAX_LENGTH+1];
	uint8_t reserved[CELL_SYNC2_MUTEX_ATTRIBUTE_SIZE
	             - sizeof(uint32_t)
	             - sizeof(uint16_t) * 2
	             - sizeof(bool)
	             - sizeof(uint8_t)
	             - (CELL_SYNC2_NAME_MAX_LENGTH + 1)
	            ];
} __attribute__((aligned(CELL_SYNC2_MUTEX_ATTRIBUTE_ALIGN))) CellSync2MutexAttribute;

#endif /* CELL_SYNC2_MUTEX_TYPES_H */
