/* SCE CONFIDENTIAL
 PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2006 Sony Computer Entertainment Inc.
 * All Rights Reserved.
 */

#ifndef __CELL_SYNC_LFQUEUE_H__
#define __CELL_SYNC_LFQUEUE_H__ 1

/* SYNC */
#include <cell/sync/lfqueue_types.h>
#include <cell/sync/error.h>

/* boolean constants for libsync */
#define __CELL_SYNC_FALSE 0
#define __CELL_SYNC_TRUE  3

#ifdef __cplusplus
extern "C" {
#endif

int cellSyncLFQueueInitialize(CellSyncLFQueue*,
							  const void *buffer,
							  unsigned int size,
							  unsigned int depth,
							  CellSyncQueueDirection,
							  void *pSignal);
int cellSyncLFQueueSize(CellSyncLFQueue*, unsigned int *size);
int cellSyncLFQueueDepth(CellSyncLFQueue*, unsigned int *depth);
int cellSyncLFQueueClear(CellSyncLFQueue*);
int cellSyncLFQueueGetDirection(const CellSyncLFQueue*, CellSyncQueueDirection* pDirection);
int cellSyncLFQueueGetEntrySize(const CellSyncLFQueue*, unsigned int* pSize);

/* internal function prototypes */
int _cellSyncLFQueuePushBody(CellSyncLFQueue*, const void *buffer, unsigned int isBlocking);
int _cellSyncLFQueuePopBody(CellSyncLFQueue*, void *buffer, unsigned int isBlocking);
int _cellSyncLFQueueGetSignalAddress(const CellSyncLFQueue*, void** ppSignal);

#ifdef __cplusplus
} /* extern "C" */
#endif


static inline
int cellSyncLFQueuePush(CellSyncLFQueue *queue, const void *buffer)
{
	return _cellSyncLFQueuePushBody(queue, buffer, 1);
}

static inline
int cellSyncLFQueueTryPush(CellSyncLFQueue *queue, const void *buffer)
{
	return _cellSyncLFQueuePushBody(queue, buffer, 0);
}

static inline
int cellSyncLFQueuePop(CellSyncLFQueue *queue,  void *buffer)
{
	return _cellSyncLFQueuePopBody(queue, buffer, 1);
}

static inline
int cellSyncLFQueueTryPop(CellSyncLFQueue *queue,  void *buffer)
{
	return _cellSyncLFQueuePopBody(queue, buffer, 0);
}

#endif /* __CELL_SYNC_QUEUE_H__ */

/*
 * Local Variables:
 * mode: C
 * c-file-style: "stroustrup"
 * tab-width: 4
 * End:
 * vim:sw=4:sts=4:ts=4
 */
