/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2009 Sony Computer Entertainment Inc.
 * All Rights Reserved.
 */

#ifndef CELL_SYNC2_COND_TYPES_H
#define CELL_SYNC2_COND_TYPES_H

#include <stdint.h>
#include <cell/sync2/types.h>
#include <cell/sync2/mutex_types.h>

#define CELL_SYNC2_COND_SIZE	128
#define CELL_SYNC2_COND_ALIGN	128

typedef struct CellSync2Cond {
	uint8_t skip[CELL_SYNC2_COND_SIZE];
} __attribute__((aligned(CELL_SYNC2_COND_ALIGN))) CellSync2Cond;


#define CELL_SYNC2_COND_ATTRIBUTE_SIZE		128
#define CELL_SYNC2_COND_ATTRIBUTE_ALIGN	8

typedef struct CellSync2CondAttribute {
	uint32_t sdkVersion;
	uint16_t maxWaiters;
	char name[CELL_SYNC2_NAME_MAX_LENGTH+1];
	uint8_t reserved[CELL_SYNC2_COND_ATTRIBUTE_SIZE
						 - sizeof(uint32_t)
						 - sizeof(uint16_t)
	                     - (CELL_SYNC2_NAME_MAX_LENGTH + 1)
	                   ];
} __attribute__((aligned(CELL_SYNC2_COND_ATTRIBUTE_ALIGN))) CellSync2CondAttribute;

#endif /* CELL_SYNC2_COND_TYPES_H */
