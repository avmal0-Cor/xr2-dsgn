/* SCE CONFIDENTIAL
 PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2006 Sony Computer Entertainment Inc.
 * All Rights Reserved.
 */

#ifndef __CELL_SYNC_LFQUEUE_H__
#define __CELL_SYNC_LFQUEUE_H__ 1

/* The C Standard Library */
#include <stdint.h>

#include <cell/sync/lfqueue_types.h>
#include <cell/sync/error.h>
//#include <cell/sync/error.h>
#include <spu_mfcio.h>

#ifdef __cplusplus
extern "C" {
#endif

int cellSyncLFQueueInitialize(uint64_t ea,
							  uint64_t buffer,
							  unsigned int size,
							  unsigned int depth,
							  CellSyncQueueDirection,
							  uint64_t eaSignal);
int cellSyncLFQueueSize(uint64_t ea, unsigned int *size);
int cellSyncLFQueueDepth(uint64_t ea, unsigned int *depth);
int cellSyncLFQueueClear(uint64_t ea);

int cellSyncLFQueueGetDirection(uint64_t ea, CellSyncQueueDirection* direction);
int cellSyncLFQueueGetEntrySize(uint64_t ea, unsigned int* pSize);

int _cellSyncLFQueuePushBeginBody(uint64_t ea,
								  CellSyncLFQueuePushContainer *pContainer,
								  unsigned int isBlocking);
int cellSyncLFQueuePushEnd(uint64_t ea,
						   CellSyncLFQueuePushContainer *pContainer);
int _cellSyncLFQueuePopBeginBody(uint64_t ea,
								 CellSyncLFQueuePopContainer *pContainer,
								 unsigned int isBlocking);
int cellSyncLFQueuePopEnd(uint64_t ea,
						  CellSyncLFQueuePopContainer *pContainer);

/* internal functions */
int _cellSyncLFQueueGetSignalAddress(uint64_t ea, uint64_t* pSignal);

#ifdef __cplusplus
} /* extern "C" */
#endif

static inline
void cellSyncLFQueuePushContainerInitialize(CellSyncLFQueuePushContainer *container,
										  const void *buffer,
										  const unsigned int tag)
{
	container->buffer = buffer;
	container->tag = tag;
}

static inline
void cellSyncLFQueuePopContainerInitialize(CellSyncLFQueuePopContainer *container,
										 void *buffer,
										 const unsigned int tag)
{
	container->buffer = buffer;
	container->tag = tag;
}

static inline
int cellSyncLFQueuePushBegin(uint64_t ea,
							 CellSyncLFQueuePushContainer *pContainer)
{
	return _cellSyncLFQueuePushBeginBody(ea, pContainer, 1);
}

static inline
int cellSyncLFQueueTryPushBegin(uint64_t ea,
							  CellSyncLFQueuePushContainer *pContainer)
{
	return _cellSyncLFQueuePushBeginBody(ea, pContainer, 0);
}

static inline
int cellSyncLFQueuePopBegin(uint64_t ea,
							CellSyncLFQueuePopContainer *pContainer)
{
	return _cellSyncLFQueuePopBeginBody(ea, pContainer, 1);
}

static inline
int cellSyncLFQueueTryPopBegin(uint64_t ea,
							   CellSyncLFQueuePopContainer *pContainer)
{
	return _cellSyncLFQueuePopBeginBody(ea, pContainer, 0);
}

#endif /* __CELL_SYNC_LFQUEUE_H__ */

/*
 * Local Variables:
 * mode: C
 * c-file-style: "stroustrup"
 * tab-width: 4
 * End:
 * vim:sw=4:sts=4:ts=4
 */
