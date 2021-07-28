/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2008 Sony Computer Entertainment Inc.
* All Rights Reserved.
*/


#ifndef __CELL_SYNC_BARRIER_TYPES_H__
#define __CELL_SYNC_BARRIER_TYPES_H__

#include <stdint.h>

typedef union _CellSyncBarrier {
	struct _CellSyncBarrier_count{
		uint16_t count;
		uint16_t total_count;
	}count;
	uint32_t uint_val;
} CellSyncBarrier;

#endif /* __CELL_SYNC_BARRIER_TYPES_H__ */
