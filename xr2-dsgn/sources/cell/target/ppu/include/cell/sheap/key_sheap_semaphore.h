/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2006 Sony Computer Entertainment Inc.
* All Rights Reserved.
*/




#ifndef __CELL_SHEAP_KEY_SHEAP_SEMAPHORE_H__
#define __CELL_SHEAP_KEY_SHEAP_SEMAPHORE_H__



#include <cell/sheap/key_sheap.h>
#include <cell/sheap/error.h>
#include <cell/atomic.h>
#include <cell/sync.h>
#include <cell/sheap/sheap_types.h>


#ifdef __cplusplus
extern "C" {
#endif

int 
cellKeySheapSemaphoreNew(CellKeySheapSemaphore*, void* ksheap, 
						 CellSheapKey key, int count);

void
cellKeySheapSemaphoreDelete(CellKeySheapSemaphore* obj);

#ifdef __cplusplus
}
#endif


#if __GNUC__ >= 3
__attribute__((always_inline))
#endif
static inline int
cellKeySheapSemaphoreTryP(CellKeySheapSemaphore* obj)
{
	if(*(int*)(uintptr_t)obj->ea <= 0){
		return CELL_SYNC_ERROR_BUSY;
	}
	uint32_t old = cellAtomicTestAndDecr32((uint32_t*)(uintptr_t)obj->ea);
	if(old == 0){
		return CELL_SYNC_ERROR_BUSY;
	}else{
		return CELL_OK;
	}
}

#if __GNUC__ >= 3
__attribute__((always_inline))
#endif
static inline void
cellKeySheapSemaphoreP(CellKeySheapSemaphore* obj)
{
	while(*(int*)(uintptr_t)obj->ea <= 0){/* do nothing */	}
	uint32_t old;
	do{
		old = cellAtomicTestAndDecr32((uint32_t*)(uintptr_t)obj->ea);
	}while(old == 0);
	return;
}

#if __GNUC__ >= 3
__attribute__((always_inline))
#endif
static inline void
cellKeySheapSemaphoreV(CellKeySheapSemaphore* obj)
{
	
	uint32_t ret = cellAtomicIncr32((uint32_t*)(uintptr_t)obj->ea);
	(void)ret;
	return;
}


#endif /* __CELL_SHEAP_KEY_SHEAP_SEMAPHORE_H__ */
