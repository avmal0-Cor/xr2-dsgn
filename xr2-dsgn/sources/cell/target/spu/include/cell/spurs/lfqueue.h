/* SCE CONFIDENTIAL
 PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2006 Sony Computer Entertainment Inc.
 * All Rights Reserved.
 */

#ifndef __CELL_SPURS_LFQUEUE_H__
#define __CELL_SPURS_LFQUEUE_H__ 1

/* The C Standard Library */
#include <stdint.h>

/* SYNC */
#include <cell/sync/lfqueue.h>

/* SPURS */
#include <cell/spurs/lfqueue_types.h>
#include <cell/spurs/task_types.h>
#include <cell/spurs/error.h>
//#include <cell/spurs/error.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

int cellSpursLFQueueInitialize(uint64_t ea,
							   uint64_t buffer,
							   unsigned int size,
							   unsigned int depth,
							   CellSpursLFQueueDirection direction);

int cellSpursLFQueueInitializeIWL(uint64_t ea,
								  uint64_t buffer,
								  unsigned int size,
								  unsigned int depth,
								  CellSpursLFQueueDirection direction);

int cellSpursLFQueueGetTasksetAddress(uint64_t ea,
							   uint64_t* pEaTaskset);
							   
int _cellSpursLFQueuePushBeginBody(uint64_t ea,
								   CellSpursLFQueuePushContainer *pContainer,
								   unsigned int isBlocking);
int _cellSpursLFQueuePopBeginBody(uint64_t ea,
								  CellSpursLFQueuePopContainer *pContainer,
								  unsigned int isBlocking);
int cellSpursLFQueuePushEnd(uint64_t ea,
							CellSpursLFQueuePushContainer *pContainer);
int cellSpursLFQueuePopEnd(uint64_t ea,
						   CellSpursLFQueuePopContainer *pContainer);

#ifdef __cplusplus
} /* extern "C" */
#endif

static inline
void cellSpursLFQueuePushContainerInitialize(CellSpursLFQueuePushContainer *container,
											 const void *buffer,
											 const unsigned int tag)
{
	container->buffer = buffer;
	container->tag = tag;
}

static inline
void cellSpursLFQueuePopContainerInitialize(CellSpursLFQueuePopContainer *container,
											void *buffer,
											const unsigned int tag)
{
	container->buffer = buffer;
	container->tag = tag;
}

static inline
int cellSpursLFQueueSize(uint64_t ea, unsigned int *size)
{
	int ret;
	ret = cellSyncLFQueueSize(ea, size);
	return CELL_ERROR_IS_FAILURE(ret) ? CELL_ERROR_MAKE_SPURS_TASK_ERROR(ret&0xff) : ret;
}

static inline
int cellSpursLFQueueDepth(uint64_t ea, unsigned int *depth)
{
	int ret;
	ret = cellSyncLFQueueDepth(ea, depth);
	return CELL_ERROR_IS_FAILURE(ret) ? CELL_ERROR_MAKE_SPURS_TASK_ERROR(ret&0xff) : ret;
}

static inline 
int cellSpursLFQueueGetDirection(uint64_t ea,
								  CellSpursLFQueueDirection* pDirection)
{
	int ret;
	ret = cellSyncLFQueueGetDirection(ea, pDirection);
	return CELL_ERROR_IS_FAILURE(ret) ? CELL_ERROR_MAKE_SPURS_TASK_ERROR(ret&0xff) : ret;
}

static inline 
int cellSpursLFQueueGetEntrySize(uint64_t ea, unsigned int* pSize)
{
	int ret;
	ret = cellSyncLFQueueGetEntrySize(ea, pSize);
	return CELL_ERROR_IS_FAILURE(ret) ? CELL_ERROR_MAKE_SPURS_TASK_ERROR(ret&0xff) : ret;
}

static inline
int cellSpursLFQueueClear(uint64_t ea)
{
	int ret;
	ret = cellSyncLFQueueClear(ea);
	return CELL_ERROR_IS_FAILURE(ret) ? CELL_ERROR_MAKE_SPURS_TASK_ERROR(ret&0xff) : ret;
}

static inline
int cellSpursLFQueuePushBegin(uint64_t ea,
							  CellSpursLFQueuePushContainer *pContainer)
{
	return _cellSpursLFQueuePushBeginBody(ea, pContainer, 1);
}

static inline
int cellSpursLFQueueTryPushBegin(uint64_t ea,
								 CellSpursLFQueuePushContainer *pContainer)
{
	return _cellSpursLFQueuePushBeginBody(ea, pContainer, 0);
}

static inline
int cellSpursLFQueuePopBegin(uint64_t ea,
							 CellSpursLFQueuePopContainer *pContainer)
{
	return _cellSpursLFQueuePopBeginBody(ea, pContainer, 1);
}

static inline
int cellSpursLFQueueTryPopBegin(uint64_t ea,
								CellSpursLFQueuePopContainer *pContainer)
{
	return _cellSpursLFQueuePopBeginBody(ea, pContainer, 0);
}

#endif /* __CELL_SPURS_LFQUEUE_H__ */

/*
 * Local Variables:
 * mode: C
 * c-file-style: "stroustrup"
 * tab-width: 4
 * End:
 * vim:sw=4:sts=4:ts=4
 */
