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
 *  @addtogroup CellKeySheapBuffer
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
cellKeySheapQueueNew(CellKeySheapQueue*, void* ksheap, CellSheapKey key,
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
cellKeySheapQueuePush(CellKeySheapQueue* obj, const void* buf)
{
	return cellSyncQueuePush((CellSyncQueue*)(uintptr_t)obj->ea,buf);
}

#if __GNUC__ >= 3
__attribute__((always_inline))
#endif
static inline int
cellKeySheapQueueTryPush(CellKeySheapQueue* obj, const void* buf)
{
	return cellSyncQueueTryPush((CellSyncQueue*)(uintptr_t)obj->ea,buf);
}

#if __GNUC__ >= 3
__attribute__((always_inline))
#endif
static inline int
cellKeySheapQueuePop(CellKeySheapQueue* obj, void* buf)
{
	return cellSyncQueuePop((CellSyncQueue*)(uintptr_t)obj->ea,buf);
}

#if __GNUC__ >= 3
__attribute__((always_inline))
#endif
static inline int
cellKeySheapQueueTryPop(CellKeySheapQueue* obj, void* buf)
{
	return cellSyncQueueTryPop((CellSyncQueue*)(uintptr_t)obj->ea,buf);
}



#if __GNUC__ >= 3
__attribute__((always_inline))
#endif
static inline unsigned int
cellKeySheapQueueSize(CellKeySheapQueue* obj)
{
	return cellSyncQueueSize((CellSyncQueue*)(uintptr_t)obj->ea);
}

#if __GNUC__ >= 3
__attribute__((always_inline))
#endif
static inline int
cellKeySheapQueueClear(CellKeySheapQueue* obj)
{
	return cellSyncQueueClear((CellSyncQueue*)(uintptr_t)obj->ea);
}


#if __GNUC__ >= 3
__attribute__((always_inline))
#endif
static inline int
cellKeySheapQueuePeek(CellKeySheapQueue* obj, void* buf)
{
	return cellSyncQueuePeek((CellSyncQueue*)(uintptr_t)obj->ea,buf);
}

#if __GNUC__ >= 3
__attribute__((always_inline))
#endif
static inline int
cellKeySheapQueueTryPeek(CellKeySheapQueue* obj, void* buf)
{
	return cellSyncQueueTryPeek((CellSyncQueue*)(uintptr_t)obj->ea,buf);
}




#endif /* __CELL_SHEAP_KEY_SHEAP_QUEUE_H__ */
