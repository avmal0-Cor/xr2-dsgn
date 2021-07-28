/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2005 Sony Computer Entertainment Inc.
* All Rights Reserved.
*/


#ifndef __CELL_SYNC_BARRIER_H__
#define __CELL_SYNC_BARRIER_H__

#include <stdint.h>

/**
 *  @addtogroup SYNC SYNC
 */
/**
 *  @addtogroup PU PU
 *  @ingroup SYNC
 */
/**
 *  @addtogroup CellSyncBarrier
 *  @ingroup PU
 */
/*@{*/

#include <cell/sync/error.h>
#include <cell/sync/barrier_types.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


int
cellSyncBarrierInitialize(CellSyncBarrier *barrier, uint16_t total_count);

int
cellSyncBarrierNotify(CellSyncBarrier *barrier);

int
cellSyncBarrierTryNotify(CellSyncBarrier *barrier);

int
cellSyncBarrierWait(CellSyncBarrier *barrier);

int
cellSyncBarrierTryWait(CellSyncBarrier *barrier);


#ifdef __cplusplus
}
#endif /* __cplusplus */

/*@}*/

#endif /* __CELL_SYNC_BARRIER_H__ */
