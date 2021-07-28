/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2006 Sony Computer Entertainment Inc.
* All Rights Reserved.
*/




#ifndef __CELL_SHEAP_KEY_SHEAP_BARRIER_H__
#define __CELL_SHEAP_KEY_SHEAP_BARRIER_H__

/**
 *  @addtogroup SHEAP SHEAP
 */
/**
 *  @addtogroup PU PU
 *  @ingroup SHEAP 
 */
/**
 *  @addtogroup CellKeySheapBarrier
 *  @ingroup PU 
 */
/*@{*/


#include <cell/sheap/key_sheap.h>
#include <cell/sync/barrier.h>
#include <cell/sheap/error.h>
#include <cell/sheap/sheap_types.h>


#ifdef __cplusplus
extern "C" {
#endif

int 
cellKeySheapBarrierNew(CellKeySheapBarrier*, uint64_t ksheap, CellSheapKey key, uint16_t count);

void
cellKeySheapBarrierDelete(CellKeySheapBarrier* obj);

#ifdef __cplusplus
}
#endif


#if __GNUC__ >= 3
__attribute__((always_inline))
#endif
static inline int
cellKeySheapBarrierNotify(CellKeySheapBarrier* obj)
{
	return cellSyncBarrierNotify(obj->ea);
}

#if __GNUC__ >= 3
__attribute__((always_inline))
#endif
static inline int
cellKeySheapBarrierTryNotify(CellKeySheapBarrier* obj)
{
	return cellSyncBarrierTryNotify(obj->ea);
}


#if __GNUC__ >= 3
__attribute__((always_inline))
#endif
static inline int
cellKeySheapBarrierWait(CellKeySheapBarrier* obj)
{
	return cellSyncBarrierWait(obj->ea);
}

#if __GNUC__ >= 3
__attribute__((always_inline))
#endif
static inline int
cellKeySheapBarrierTryWait(CellKeySheapBarrier* obj)
{
	return cellSyncBarrierTryWait(obj->ea);
}




#endif /* __CELL_SHEAP_KEY_SHEAP_BARRIER_H__ */
