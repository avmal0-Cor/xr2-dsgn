/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2007 Sony Computer Entertainment Inc.
 * All Rights Reserved.
 */
#ifndef __CELL_SPURS_QUEUE_H__
#define __CELL_SPURS_QUEUE_H__ 1

/* The C Standard Library */
#include <stdint.h>

/* SPURS */
#include <cell/spurs/queue_types.h>
#include <cell/spurs/error.h>

#ifdef __cplusplus
extern "C" {
#endif

int _cellSpursQueueInitialize(uint64_t ea, uint64_t buffer, unsigned int size, unsigned int depth, CellSpursQueueDirection, unsigned isIWL);
int _cellSpursQueuePushBegin(uint64_t ea, const void* buffer, unsigned int tag, unsigned isBlocking);
int cellSpursQueuePushEnd(uint64_t ea, unsigned int tag);
int _cellSpursQueuePopBegin(uint64_t ea, void* buffer, unsigned int tag, unsigned isBlocking);
int _cellSpursQueuePopEnd(uint64_t ea, unsigned int tag, unsigned isPeek);
int cellSpursQueueSize(uint64_t ea, unsigned int *size);
int cellSpursQueueDepth(uint64_t ea, unsigned int *size);
int cellSpursQueueClear(uint64_t ea);
int cellSpursQueueGetDirection(uint64_t ea, CellSpursQueueDirection* direction);
int cellSpursQueueGetEntrySize(uint64_t ea, unsigned int *entry_size);
int cellSpursQueueGetTasksetAddress(uint64_t ea, uint64_t* pEaTaskset);

#ifdef __cplusplus
} /* extern "C" */
#endif

#define cellSpursQueueInitialize(ea,buf,size,depth,direction) \
		_cellSpursQueueInitialize((ea),(buf),(size),(depth),(direction),0)
#define cellSpursQueueInitializeIWL(ea,buf,size,depth,direction) \
		_cellSpursQueueInitialize((ea),(buf),(size),(depth),(direction),1)

#define cellSpursQueueTryPushBegin(ea,buf,tag) \
		_cellSpursQueuePushBegin((ea),(buf),(tag),0)
#define cellSpursQueuePushBegin(ea,buf,tag) \
		_cellSpursQueuePushBegin((ea),(buf),(tag),1)

#define cellSpursQueueTryPopBegin(ea,buf,tag) \
		_cellSpursQueuePopBegin((ea),(buf),(tag),0)
#define cellSpursQueuePopBegin(ea,buf,tag) \
		_cellSpursQueuePopBegin((ea),(buf),(tag),1)
#define cellSpursQueueTryPeekBegin(ea,buf,tag) \
		_cellSpursQueuePopBegin((ea),(buf),(tag),0)
#define cellSpursQueuePeekBegin(ea,buf,tag) \
		_cellSpursQueuePopBegin((ea),(buf),(tag),1)

#define cellSpursQueuePopEnd(ea,tag) \
		_cellSpursQueuePopEnd((ea),(tag),0)
#define cellSpursQueuePeekEnd(ea,tag) \
		_cellSpursQueuePopEnd((ea),(tag),1)

#endif /* __CELL_SPURS_QUEUE_H__ */
/*
 * Local Variables:
 * mode: C
 * c-file-style: "stroustrup"
 * tab-width: 4
 * End:
 * vim:sw=4:sts=4:ts=4
 */
