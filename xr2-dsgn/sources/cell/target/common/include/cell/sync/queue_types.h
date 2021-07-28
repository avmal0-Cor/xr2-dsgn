/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2008 Sony Computer Entertainment Inc.
* All Rights Reserved.
*/


#ifndef __CELL_SYNC_QUEUE_TYPES_H__
#define __CELL_SYNC_QUEUE_TYPES_H__

#include <stdint.h>

typedef	union _CellSyncQueueHead {
#ifdef __GNUC__
	__extension__
#endif
	struct _CellSyncQueueHeadStr {
		uint64_t rlock        :  8;  
		uint64_t index        : 24;
		uint64_t wlock        :  8;  
		uint64_t size         : 24;  
	} asStr;
	uint64_t asUint;
} _CellSyncQueueHead;
	

typedef struct  _CellSyncQueue {
	_CellSyncQueueHead head;
	uint32_t           buffer_size;
	uint32_t           depth;
	uint64_t           addr_buffer;
	uint64_t           _padding;
} CellSyncQueue;


#endif /* __CELL_SYNC_QUEUE_TYPES_H__ */



