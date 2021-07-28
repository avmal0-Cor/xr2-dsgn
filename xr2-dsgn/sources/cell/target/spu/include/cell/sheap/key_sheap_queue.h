/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2006 Sony Computer Entertainment Inc.
* All Rights Reserved.
*/



#ifndef __CELL_SHEAP_KEY_SHEAP_QUEUE_H__
#define __CELL_SHEAP_KEY_SHEAP_QUEUE_H__

/**
 *  @addtogroup SHEAP SHEAP
 */
/**
 *  @addtogroup PU PU
 *  @ingroup SHEAP 
 */
/**
 *  @addtogroup CellKeySheapQueue
 *  @ingroup PU 
 */
/*@{*/


#include <cell/sheap/key_sheap.h>
#include <cell/sync/queue.h>
#include <cell/sheap/error.h>
#include <cell/sheap/sheap_types.h>

#ifdef __cplusplus
extern "C" {
#endif

int 
cellKeySheapQueueNew(CellKeySheapQueue*, uint64_t ksheap, CellSheapKey key, 
					 uint32_t buffer_size, uint32_t depth);

void
cellKeySheapQueueDelete(CellKeySheapQueue* obj);

#ifdef __cplusplus
}
#endif


#if __GNUC__ >= 3
__attribute__((always_inline))
#endif
static inline int
cellKeySheapQueuePush(CellKeySheapQueue* obj,const void *buf, 
				  unsigned int tag)
{
	return cellSyncQueuePush(obj->ea, buf, tag);
}

#if __GNUC__ >= 3
__attribute__((always_inline))
#endif
static inline int
cellKeySheapQueueTryPush(CellKeySheapQueue* obj,const void *buf, 
				  unsigned int tag)
{
	return cellSyncQueueTryPush(obj->ea, buf, tag);
}

#if __GNUC__ >= 3
__attribute__((always_inline))
#endif
static inline int
cellKeySheapQueuePop(CellKeySheapQueue* obj,void *buf, 
				  unsigned int tag)
{
	return cellSyncQueuePop(obj->ea, buf, tag);
}

#if __GNUC__ >= 3
__attribute__((always_inline))
#endif
static inline int
cellKeySheapQueueTryPop(CellKeySheapQueue* obj,void *buf, 
				  unsigned int tag)
{
	return cellSyncQueueTryPop(obj->ea, buf, tag);
}


#if __GNUC__ >= 3
__attribute__((always_inline))
#endif
static inline int
cellKeySheapQueueSize(CellKeySheapQueue* obj)
{
	return cellSyncQueueSize(obj->ea);
}

#if __GNUC__ >= 3
__attribute__((always_inline))
#endif
static inline int
cellKeySheapQueueClear(CellKeySheapQueue* obj)
{
	return cellSyncQueueClear(obj->ea);
}


#if __GNUC__ >= 3
__attribute__((always_inline))
#endif
static inline int
cellKeySheapQueuePeek(CellKeySheapQueue* obj,void *buf, 
				  unsigned int tag)
{
	return cellSyncQueuePeek(obj->ea, buf, tag);
}

#if __GNUC__ >= 3
__attribute__((always_inline))
#endif
static inline int
cellKeySheapQueueTryPeek(CellKeySheapQueue* obj,void *buf, 
				  unsigned int tag)
{
	return cellSyncQueueTryPeek(obj->ea, buf, tag);
}




#endif /* __CELL_SHEAP_KEY_SHEAP_QUEUE_H__ */
