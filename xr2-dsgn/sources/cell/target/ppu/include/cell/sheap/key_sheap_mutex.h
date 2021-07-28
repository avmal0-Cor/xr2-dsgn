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
 *  @addtogroup CellKeySheapBuffer
 *  @ingroup PU 
 */
/*@{*/


#include <assert.h>
#include <cell/sheap/key_sheap.h>
#include <cell/sync/mutex.h>
#include <cell/sheap/error.h>
#include <cell/sheap/sheap_types.h>


#ifdef __cplusplus
extern "C" {
#endif

int 
cellKeySheapMutexNew(CellKeySheapMutex*, void* ksheap, CellSheapKey key);

void
cellKeySheapMutexDelete(CellKeySheapMutex* obj);

#ifdef __cplusplus
}
#endif


#if __GNUC__ >= 3
__attribute__((always_inline))
#endif
static inline int
cellKeySheapMutexTryLock(CellKeySheapMutex* obj)
{
	return cellSyncMutexTryLock((CellSyncMutex*)(uintptr_t)obj->ea);
}

#if __GNUC__ >= 3
__attribute__((always_inline))
#endif
static inline void
cellKeySheapMutexLock(CellKeySheapMutex* obj)
{
	int r;
	r = cellSyncMutexLock((CellSyncMutex*)(uintptr_t)obj->ea);
	assert(r == CELL_OK);
	return ;
}

#if __GNUC__ >= 3
__attribute__((always_inline))
#endif
static inline void
cellKeySheapMutexUnlock(CellKeySheapMutex* obj)
{
	int r;
	r = cellSyncMutexUnlock((CellSyncMutex*)(uintptr_t)obj->ea);
	assert(r == CELL_OK);
	return;
}


#endif /* __CELL_SHEAP_KEY_SHEAP_MUTEX_H__ */
