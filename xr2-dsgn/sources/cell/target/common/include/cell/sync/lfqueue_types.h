/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2007 Sony Computer Entertainment Inc.
* All Rights Reserved.
*/

#ifndef __CELL_SYNC_LFQUEUE_TYPES_H__
#define __CELL_SYNC_LFQUEUE_TYPES_H__ 1

typedef enum CellSyncQueueDirection {
	CELL_SYNC_QUEUE_SPU2SPU,
	CELL_SYNC_QUEUE_SPU2PPU,
	CELL_SYNC_QUEUE_PPU2SPU,
	CELL_SYNC_QUEUE_ANY2ANY
} CellSyncQueueDirection;

#define CELL_SYNC_LFQUEUE_ALIGN	128
#define CELL_SYNC_LFQUEUE_SIZE		128

typedef struct CellSyncLFQueue {
	unsigned char skip[CELL_SYNC_LFQUEUE_SIZE];
} __attribute__((aligned(CELL_SYNC_LFQUEUE_ALIGN))) CellSyncLFQueue;

#ifdef __SPU__
typedef struct CellSyncLFQueuePushContainer {
	const void *buffer;
	int pointer;
	unsigned int tag;
} CellSyncLFQueuePushContainer;

typedef struct CellSyncLFQueuePopContainer {
	void *buffer;
	int pointer;
	unsigned int tag;
} CellSyncLFQueuePopContainer;
#endif /* __SPU__ */

#endif /* __CELL_SYNC_LFQUEUE_TYPES_H__ */

/*
 * Local Variables:
 * mode: C
 * c-file-style: "stroustrup"
 * tab-width: 4
 * End:
 * vim:sw=4:sts=4:ts=4
 */
