/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2008 Sony Computer Entertainment Inc.
* All Rights Reserved.
*/


#ifndef __CELL_SYNC_RWM_TYPES_H__
#define __CELL_SYNC_RWM_TYPES_H__


#include <stdint.h>

typedef struct CellSyncRwm {
	union CellSyncRwm_lock {
		struct CellSyncRwm_rwlock {
			uint16_t rlock;
			uint16_t wlock;
		} rwlock;
		uint32_t uint_val;
	} lock;
	uint32_t buffer_size;
	uint64_t buffer;
} CellSyncRwm;


#endif /* __CELL_SYNC_RWM_TYPES_H__ */
