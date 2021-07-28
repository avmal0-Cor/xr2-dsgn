/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2005 Sony Computer Entertainment Inc.
* All Rights Reserved.
*/


#ifndef __CELL_SYNC_BARRIER_H__
#define __CELL_SYNC_BARRIER_H__

#include <stdint.h>
#include <cell/atomic.h>
#include <cell/sync/error.h>
#include <cell/sync/barrier_types.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


int
cellSyncBarrierInitialize(uint64_t ea_obj, uint16_t count, unsigned int tag);

int
cellSyncBarrierNotify(uint64_t ea);

int
cellSyncBarrierTryNotify(uint64_t ea);

int
cellSyncBarrierWait(uint64_t ea);

int
cellSyncBarrierTryWait(uint64_t ea);

#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif /* __CELL_SYNC_BARRIER_H__ */
