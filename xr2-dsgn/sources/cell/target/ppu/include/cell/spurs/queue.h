/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2006 Sony Computer Entertainment Inc.
 * All Rights Reserved.
 */

#ifndef __CELL_SPURS_QUEUE_H__
#define __CELL_SPURS_QUEUE_H__ 1

/* The C Standard Library */
#include <stdint.h>
#include <stdbool.h>

/* SPURS */
#include <cell/spurs/types.h>			/* CellSpurs */
#include <cell/spurs/task_types.h>		/* CellSpursTaskset */
#include <cell/spurs/queue_types.h>		/* CellSpursQueue */
#include <cell/spurs/error.h>

#ifdef __cplusplus
extern "C" {
#endif

int _cellSpursQueueInitialize(CellSpurs*, CellSpursTaskset*, CellSpursQueue*, const void *buffer, unsigned int size, unsigned int depth, CellSpursQueueDirection);
int cellSpursQueueAttachLv2EventQueue(CellSpursQueue*);
int cellSpursQueueDetachLv2EventQueue(CellSpursQueue*);

int cellSpursQueuePushBody(CellSpursQueue*, const void *buffer, bool isBlocking);
int cellSpursQueuePopBody(CellSpursQueue*, void *buffer, bool isPeek, bool isBlocking);

int cellSpursQueueSize(CellSpursQueue*, unsigned int *size);
int cellSpursQueueDepth(CellSpursQueue*, unsigned int *depth);
int cellSpursQueueClear(CellSpursQueue*);
int cellSpursQueueGetDirection(CellSpursQueue*, CellSpursQueueDirection* direction);
int cellSpursQueueGetEntrySize(CellSpursQueue*, unsigned int *entry_size);
int cellSpursQueueGetTasksetAddress(const CellSpursQueue*, CellSpursTaskset** pTaskset);

#ifdef __cplusplus
} /* extern "C" */
#endif

static inline
int cellSpursQueueTryPush(CellSpursQueue *queue, const void *buffer)
{
	return cellSpursQueuePushBody(queue, buffer, false);
}

static inline
int cellSpursQueuePush(CellSpursQueue *queue, const void *buffer)
{
	return cellSpursQueuePushBody(queue, buffer, true);
}

static inline
int cellSpursQueueTryPop(CellSpursQueue *queue, void *buffer)
{
	return cellSpursQueuePopBody(queue, buffer, false, false);
}

static inline
int cellSpursQueuePop(CellSpursQueue *queue,  void *buffer)
{
	return cellSpursQueuePopBody(queue, buffer, false, true);
}

static inline
int cellSpursQueueTryPeek(CellSpursQueue *queue, void *buffer)
{
	return cellSpursQueuePopBody(queue, buffer, true, false);
}

static inline
int cellSpursQueuePeek(CellSpursQueue *queue,  void *buffer)
{
	return cellSpursQueuePopBody(queue, buffer, true, true);
}

static inline
int cellSpursQueueInitialize(CellSpursTaskset* taskset, CellSpursQueue* queue, const void *buffer, unsigned int size, unsigned int depth, CellSpursQueueDirection direction)
{
	return _cellSpursQueueInitialize(0, taskset, queue, buffer, size, depth, direction);
}

static inline
int cellSpursQueueInitializeIWL(CellSpurs* spurs, CellSpursQueue* queue, const void *buffer, unsigned int size, unsigned int depth, CellSpursQueueDirection direction)
{
	return _cellSpursQueueInitialize(spurs, 0, queue, buffer, size, depth, direction);
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
