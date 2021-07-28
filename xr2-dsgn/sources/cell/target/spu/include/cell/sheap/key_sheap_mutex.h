/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2006 Sony Computer Entertainment Inc.
* All Rights Reserved.
*/



#ifndef __CELL_SHEAP_KEY_SHEAP_MUTEX_H__
#define __CELL_SHEAP_KEY_SHEAP_MUTEX_H__

/**
 *  @addtogroup SHEAP SHEAP
 */
/**
 *  @addtogroup PU PU
 *  @ingroup SHEAP 
 */
/**
 *  @addtogroup CellKeySheapMutex
 *  @ingroup PU 
 */
/*@{*/


#include <cell/sheap/key_sheap.h>
#include <cell/sync/mutex.h>
#include <cell/sheap/error.h>
#include <cell/sheap/sheap_types.h>

#ifdef __cplusplus
extern "C" {
#endif

int 
cellKeySheapMutexNew(CellKeySheapMutex*, uint64_t ksheap, CellSheapKey key);

void
cellKeySheapMutexDelete(CellKeySheapMutex* obj);

#ifdef __cplusplus
}
#endif


#if __GNUC__ >= 3
__attribute__((always_inline))
#endif
static inline int
cellKeySheapMutexLock(CellKeySheapMutex* obj)
{
	return cellSyncMutexLock(obj->ea);
}

#if __GNUC__ >= 3
__attribute__((always_inline))
#endif
static inline int
cellKeySheapMutexTryLock(CellKeySheapMutex* obj)
{
	return cellSyncMutexTryLock(obj->ea);
}

#if __GNUC__ >= 3
__attribute__((always_inline))
#endif
static inline int
cellKeySheapMutexUnlock(CellKeySheapMutex* obj)
{
	return cellSyncMutexUnlock(obj->ea);
}




#endif /* __CELL_SHEAP_KEY_SHEAP_MUTEX_H__ */ 
