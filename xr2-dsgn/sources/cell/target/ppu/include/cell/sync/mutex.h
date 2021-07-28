/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2005 Sony Computer Entertainment Inc.
* All Rights Reserved.
*/


#ifndef __CELL_SYNC_MUTEX_H__
#define __CELL_SYNC_MUTEX_H__

#include <cell/sync/error.h>
#include <stdint.h>
#include <cell/atomic.h>
#include <cell/sync/mutex_types.h>

/**
 *  @addtogroup SYNC SYNC
 */
/**
 *  @addtogroup PU PU
 *  @ingroup SYNC
 */
/**
 *  @addtogroup CellSyncMutex
 *  @ingroup PU
 */
/*@{*/



#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

int
cellSyncMutexInitialize(CellSyncMutex *m);

int
cellSyncMutexTryLock(CellSyncMutex *m);


int
cellSyncMutexLock(CellSyncMutex *m);

int
cellSyncMutexUnlock(CellSyncMutex *m);

#ifdef __cplusplus
}
#endif /* __cplusplus */


/*@}*/

#endif /* __CELL_SYNC_MUTEX_H__ */
