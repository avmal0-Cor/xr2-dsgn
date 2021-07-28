/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2006 Sony Computer Entertainment Inc.
 * All Rights Reserved.
 */

#ifndef __CELL_SPURS_LFQUEUE_H__
#define __CELL_SPURS_LFQUEUE_H__ 1

/* The C Standard Library */
#include <stdint.h>
#include <stdbool.h>

/* SYNC */
#include <cell/sync/lfqueue.h>

/* SPURS */
#include <cell/spurs/task_types.h>		/* CellSpursTaskset */
#include <cell/spurs/types.h>		/* CellSpurs */
#include <cell/spurs/lfqueue_types.h>		/* CellSpursQueue */
#include <cell/spurs/error.h>
//#include <cell/spurs/error.h>

#ifdef __cplusplus
extern "C" {
#endif

int _cellSpursLFQueueInitialize(void *pTasksetOrSpurs,
								CellSpursLFQueue *pQueue,
								const void *buffer,
								unsigned int size,
								unsigned int depth,
								CellSpursLFQueueDirection direction);
int cellSpursLFQueueAttachLv2EventQueue(CellSpursLFQueue*);
int cellSpursLFQueueDetachLv2EventQueue(CellSpursLFQueue*);

int cellSpursLFQueueGetTasksetAddress(const CellSpursLFQueue*, CellSpursTaskset**);
int _cellSpursLFQueuePushBody(CellSpursLFQueue*, const void *buffer, unsigned int isBlocking);
int _cellSpursLFQueuePopBody(CellSpursLFQueue*, void *buffer, unsigned int isBlocking);

#ifdef __cplusplus
} /* extern "C" */
#endif

static inline
int cellSpursLFQueueInitialize(CellSpursTaskset *pTaskset,
							   CellSpursLFQueue *pQueue,
							   const void *buffer,
							   unsigned int size,
							   unsigned int depth,
							   CellSpursLFQueueDirection direction)
{
	if (!pTaskset) return CELL_SPURS_TASK_ERROR_NULL_POINTER;
	if ((uintptr_t)pTaskset & 0x7f) return CELL_SPURS_TASK_ERROR_ALIGN;
	return _cellSpursLFQueueInitialize((void *)pTaskset, pQueue, buffer, size, depth, direction);
}

static inline
int cellSpursLFQueueInitializeIWL(CellSpurs *pSpurs,
								  CellSpursLFQueue *pQueue,
								  const void *buffer,
								  unsigned int size,
								  unsigned int depth,
								  CellSpursLFQueueDirection direction)
{
	if (!pSpurs) return CELL_SPURS_TASK_ERROR_NULL_POINTER;
	if ((uintptr_t)pSpurs & 0x7f) return CELL_SPURS_TASK_ERROR_ALIGN;
	uint64_t eaSpurs = (uintptr_t)pSpurs | 0x1;
	return _cellSpursLFQueueInitialize((void *)(uintptr_t)eaSpurs, pQueue, buffer, size, depth, direction);
}

static inline
int cellSpursLFQueueSize(CellSpursLFQueue *pQueue, unsigned int *size)
{
	int ret;
	ret = cellSyncLFQueueSize(pQueue, size);
	return CELL_ERROR_IS_FAILURE(ret) ? CELL_ERROR_MAKE_SPURS_TASK_ERROR(ret&0xff) : ret;
}

static inline
int cellSpursLFQueueDepth(CellSpursLFQueue *pQueue, unsigned int *depth)
{
	int ret;
	ret = cellSyncLFQueueDepth(pQueue, depth);
	return CELL_ERROR_IS_FAILURE(ret) ? CELL_ERROR_MAKE_SPURS_TASK_ERROR(ret&0xff) : ret;
}

static inline 
int cellSpursLFQueueGetDirection(const CellSpursLFQueue* pQueue, 
								CellSpursLFQueueDirection* pDirection)
{
	int ret;
	ret = cellSyncLFQueueGetDirection(pQueue, pDirection);
	return CELL_ERROR_IS_FAILURE(ret) ? CELL_ERROR_MAKE_SPURS_TASK_ERROR(ret&0xff) : ret;
}

static inline
int cellSpursLFQueueGetEntrySize(const CellSpursLFQueue* pQueue, 
								unsigned int* pSize)
{
	int ret;
	ret = cellSyncLFQueueGetEntrySize(pQueue, pSize);
	return CELL_ERROR_IS_FAILURE(ret) ? CELL_ERROR_MAKE_SPURS_TASK_ERROR(ret&0xff) : ret;
}

static inline
int cellSpursLFQueueClear(CellSpursLFQueue *pQueue)
{
	int ret;
	ret = cellSyncLFQueueClear(pQueue);
	return CELL_ERROR_IS_FAILURE(ret) ? CELL_ERROR_MAKE_SPURS_TASK_ERROR(ret&0xff) : ret;
}

static inline
int cellSpursLFQueuePush(CellSpursLFQueue *queue, const void *buffer)
{
	return _cellSpursLFQueuePushBody(queue, buffer, __CELL_SYNC_TRUE);
}

static inline
int cellSpursLFQueueTryPush(CellSpursLFQueue *queue, const void *buffer)
{
	return _cellSpursLFQueuePushBody(queue, buffer, __CELL_SYNC_FALSE);
}

static inline
int cellSpursLFQueuePop(CellSpursLFQueue *queue,  void *buffer)
{
	return _cellSpursLFQueuePopBody(queue, buffer, __CELL_SYNC_TRUE);
}

static inline
int cellSpursLFQueueTryPop(CellSpursLFQueue *queue,  void *buffer)
{
	return _cellSpursLFQueuePopBody(queue, buffer, __CELL_SYNC_FALSE);
}

#endif /* __CELL_SPURS_QUEUE_H__ */

/*
 * Local Variables:
 * mode: C
 * c-file-style: "stroustrup"
 * tab-width: 4
 * End:
 * vim:sw=4:sts=4:ts=4
 */
