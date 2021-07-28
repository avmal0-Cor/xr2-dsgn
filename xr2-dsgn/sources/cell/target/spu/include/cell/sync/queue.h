/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2005 Sony Computer Entertainment Inc.
* All Rights Reserved.
*/


#ifndef __CELL_SYNC_QUEUE_H__
#define __CELL_SYNC_QUEUE_H__

#include <stdint.h>
#include <cell/sync/error.h>
#include <cell/sync/queue_types.h>


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


int
cellSyncQueueInitialize(uint64_t ea, 
						uint64_t ptr_buffer, 
						uint32_t buffer_size, 
						unsigned int depth,
						unsigned int tag);


int
cellSyncQueuePush(uint64_t ptr_obj,  const void *buf, 
				  unsigned int tag);

int  
cellSyncQueueTryPush(uint64_t ptr_obj, 
					 const void *buf,
					 unsigned int tag);

int
cellSyncQueuePop(uint64_t ptr_obj, 
				 void *buf,
				 unsigned int tag);

int  
cellSyncQueueTryPop(uint64_t ptr_obj, 
					void *buf,
					unsigned int tag);


int
cellSyncQueueSize(uint64_t ea);

int
cellSyncQueueClear(uint64_t ea);


int
cellSyncQueuePeek(uint64_t ptr_obj, 
				  void *buf,
				  unsigned int tag);

int
cellSyncQueueTryPeek(uint64_t ptr_obj, 
					 void *buf,
					 unsigned int tag);

#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif /* __CELL_SYNC_QUEUE_H__ */








