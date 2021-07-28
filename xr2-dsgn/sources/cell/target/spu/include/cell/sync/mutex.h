/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2005 Sony Computer Entertainment Inc.
* All Rights Reserved.
*/


#ifndef __CELL_SYNC_MUTEX_H__
#define __CELL_SYNC_MUTEX_H__

#include <stdint.h>
#include <cell/sync/error.h>
#include <cell/sync/mutex_types.h>


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


int
cellSyncMutexInitialize(uint64_t ea_obj, unsigned int tag);


int
cellSyncMutexTryLock(uint64_t ea);


int
cellSyncMutexLock(uint64_t ea);


int
cellSyncMutexUnlock(uint64_t ea);

#ifdef __cplusplus
}
#endif /* __cplusplus */



#endif /* __CELL_SYNC_MUTEX_H__ */
