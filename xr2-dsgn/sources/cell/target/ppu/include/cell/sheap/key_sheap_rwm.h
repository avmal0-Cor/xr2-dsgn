/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2006 Sony Computer Entertainment Inc.
* All Rights Reserved.
*/




#ifndef __CELL_SHEAP_KEY_SHEAP_RWM_H__
#define __CELL_SHEAP_KEY_SHEAP_RWM_H__

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
#include <cell/sync/rwm.h>
#include <cell/sheap/error.h>
#include <cell/sheap/sheap_types.h>

#ifdef __cplusplus
extern "C" {
#endif

int 
cellKeySheapRwmNew(CellKeySheapRwm*, void* ksheap, CellSheapKey key, uint32_t buffer_size);

void
cellKeySheapRwmDelete(CellKeySheapRwm* obj);

#ifdef __cplusplus
}
#endif


#if __GNUC__ >= 3
__attribute__((always_inline))
#endif
static inline int
cellKeySheapRwmRead(CellKeySheapRwm* obj, void* buffer)
{
	return cellSyncRwmRead((CellSyncRwm*)(uintptr_t)obj->ea,buffer);
}

#if __GNUC__ >= 3
__attribute__((always_inline))
#endif
static inline int 
cellKeySheapRwmTryRead(CellKeySheapRwm* obj, void* buffer)
{
	return cellSyncRwmTryRead((CellSyncRwm*)(uintptr_t)obj->ea,buffer);
}

#if __GNUC__ >= 3
__attribute__((always_inline))
#endif
static inline int
cellKeySheapRwmWrite(CellKeySheapRwm* obj, void* buffer)
{
	return cellSyncRwmWrite((CellSyncRwm*)(uintptr_t)obj->ea,buffer);
}


#if __GNUC__ >= 3
__attribute__((always_inline))
#endif
static inline int
cellKeySheapRwmTryWrite(CellKeySheapRwm* obj, void* buffer)
{
	return cellSyncRwmTryWrite((CellSyncRwm*)(uintptr_t)obj->ea,buffer);
}


#endif /* __CELL_SHEAP_KEY_SHEAP_RWM_H__ */
