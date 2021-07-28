/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2005 Sony Computer Entertainment Inc.
* All Rights Reserved.
*/


#ifndef __CELL_SYNC_RWM_H__
#define __CELL_SYNC_RWM_H__

#include <cell/sync/error.h>
#include <cell/sync/rwm_types.h>
#include <cell/atomic.h>


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


int
cellSyncRwmInitialize(uint64_t ea, 
					  uint64_t ptr_buffer,  
					  uint32_t buffer_size,
					  unsigned int tag);

int
cellSyncRwmReadBegin(uint64_t ea, void* buffer, unsigned int tag);

int
cellSyncRwmTryReadBegin(uint64_t ea, void* buffer, unsigned int tag);

int
cellSyncRwmReadEnd(uint64_t ea, unsigned int tag);

int
cellSyncRwmWrite(uint64_t ea, void* buffer, unsigned int tag);
int
cellSyncRwmTryWrite(uint64_t ea, void* buffer, unsigned int tag);

#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif /* __CELL_SYNC_RWM_H__ */
