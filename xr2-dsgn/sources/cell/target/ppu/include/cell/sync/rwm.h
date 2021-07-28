/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2005 Sony Computer Entertainment Inc.
* All Rights Reserved.
*/


#ifndef __CELL_SYNC_RWM_H__
#define __CELL_SYNC_RWM_H__

#include <cell/sync/error.h>
#include <stdint.h>
#include <cell/sync/rwm_types.h>
/**
 *  @addtogroup SYNC SYNC
 */
/**
 *  @addtogroup PU PU
 *  @ingroup SYNC
 */
/**
 *  @addtogroup CellSyncRwm
 *  @ingroup PU
 */
/*@{*/



#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */



int
cellSyncRwmInitialize(CellSyncRwm *obj, 
					  void* buffer,  
					  uint32_t buffer_size);

int
cellSyncRwmRead(CellSyncRwm * obj, void* buffer);

int
cellSyncRwmTryRead(CellSyncRwm * obj, void* buffer);

int
cellSyncRwmWrite(CellSyncRwm * obj, void* buffer);


int
cellSyncRwmTryWrite(CellSyncRwm * obj, void* buffer);

#ifdef __cplusplus
}
#endif /* __cplusplus */


/*@}*/

#endif /* __CELL_SYNC_RWM_H__ */
