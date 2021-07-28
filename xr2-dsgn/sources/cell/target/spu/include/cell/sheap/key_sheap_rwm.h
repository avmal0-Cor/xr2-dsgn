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
 *  @addtogroup CellKeySheapRwm
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
cellKeySheapRwmNew(CellKeySheapRwm*, uint64_t ksheap, CellSheapKey key, 
					 uint32_t buffer_size);

void
cellKeySheapRwmDelete(CellKeySheapRwm* obj);

#ifdef __cplusplus
}
#endif


#if __GNUC__ >= 3
__attribute__((always_inline))
#endif
static inline int
cellKeySheapRwmReadBegin(CellKeySheapRwm* obj,void *buf, 
					 unsigned int tag)
{
	return cellSyncRwmReadBegin(obj->ea, buf, tag);
}

#if __GNUC__ >= 3
__attribute__((always_inline))
#endif
static inline int
cellKeySheapRwmTryReadBegin(CellKeySheapRwm* obj,void *buf, 
							unsigned int tag)
{
	return cellSyncRwmTryReadBegin(obj->ea, buf, tag);
}



#if __GNUC__ >= 3
__attribute__((always_inline))
#endif
static inline int
cellKeySheapRwmReadEnd(CellKeySheapRwm* obj,
					   unsigned int tag)
{
	return cellSyncRwmReadEnd(obj->ea, tag);
}


#if __GNUC__ >= 3
__attribute__((always_inline))
#endif
static inline int
cellKeySheapRwmWrite(CellKeySheapRwm* obj,void *buf,
					 unsigned int tag)
{
	return cellSyncRwmWrite(obj->ea, buf, tag);
}

#if __GNUC__ >= 3
__attribute__((always_inline))
#endif
static inline int
cellKeySheapRwmTryWrite(CellKeySheapRwm* obj,void *buf,
					 unsigned int tag)
{
	return cellSyncRwmTryWrite(obj->ea, buf, tag);
}





#endif /* __CELL_SHEAP_KEY_SHEAP_RWM_H__ */
