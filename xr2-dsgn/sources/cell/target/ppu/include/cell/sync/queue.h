/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2005 Sony Computer Entertainment Inc.
* All Rights Reserved.
*/


#ifndef __CELL_SYNC_QUEUE_H__
#define __CELL_SYNC_QUEUE_H__

#include <cell/sync/error.h>
#include <stdint.h>
#include <cell/sync/queue_types.h>
/**
 *  @addtogroup SYNC SYNC
 */
/**
 *  @addtogroup PU PU
 *  @ingroup SYNC
 */
/**
 *  @addtogroup CellSyncQueue
 *  @ingroup PU
 */
/*@{*/






#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

int
cellSyncQueueInitialize(CellSyncQueue *obj, 
						void* ptr_buffer, 
						uint32_t buffer_size, 
						uint32_t depth);


int
cellSyncQueuePush(CellSyncQueue *obj, 
				  const void *buf);


int  
cellSyncQueueTryPush(CellSyncQueue *obj, 
					 const void *buf);


int
cellSyncQueuePop(CellSyncQueue *obj, 
				 void *buf);

int  
cellSyncQueueTryPop(CellSyncQueue *obj, 
					void *buf);


unsigned int 
cellSyncQueueSize(CellSyncQueue *obj);

int
cellSyncQueueClear(CellSyncQueue *obj);


int 
cellSyncQueuePeek(CellSyncQueue *obj, void *buf);

int 
cellSyncQueueTryPeek(CellSyncQueue *obj, void *buf);

#ifdef __cplusplus
}
#endif /* __cplusplus */


/*@}*/

#endif /* __CELL_SYNC_QUEUE_H__ */



/*
 * Local Variables:
 * mode: C
 * tab-width: 4
 * End:
 * vim:sw=4:sts=4:ts=4
 */





