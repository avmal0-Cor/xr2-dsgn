/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2008 Sony Computer Entertainment Inc.
* All Rights Reserved.
*/


#ifndef __CELL_SYNC_MUTEX_TYPES_H__
#define __CELL_SYNC_MUTEX_TYPES_H__

#include <stdint.h>

typedef union CellSyncMutex {
	struct CellSyncMutex_ticket{
		uint16_t current_ticket;
		uint16_t next_ticket;
	} ticket;
	uint32_t uint_val;
} CellSyncMutex;



#endif /* __CELL_SYNC_MUTEX_TYPES_H__ */
