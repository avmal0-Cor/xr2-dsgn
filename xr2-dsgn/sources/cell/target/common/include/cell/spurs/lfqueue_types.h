/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2007 Sony Computer Entertainment Inc.
* All Rights Reserved.
*/

#ifndef __CELL_SPURS_LFQUEUE_TYPES_H__
#define __CELL_SPURS_LFQUEUE_TYPES_H__ 1

/* SYNC */
#include <cell/sync/lfqueue_types.h>

#define CELL_SPURS_LFQUEUE_ALIGN CELL_SYNC_LFQUEUE_ALIGN
#define CELL_SPURS_LFQUEUE_SIZE CELL_SYNC_LFQUEUE_SIZE

#define CELL_SPURS_LFQUEUE_SPU2PPU CELL_SYNC_QUEUE_SPU2PPU
#define CELL_SPURS_LFQUEUE_PPU2SPU CELL_SYNC_QUEUE_PPU2SPU
#define CELL_SPURS_LFQUEUE_SPU2SPU CELL_SYNC_QUEUE_SPU2SPU
#define CELL_SPURS_LFQUEUE_ANY2ANY CELL_SYNC_QUEUE_ANY2ANY

typedef CellSyncQueueDirection CellSpursLFQueueDirection;

typedef CellSyncLFQueue CellSpursLFQueue;

#ifdef __SPU__
typedef CellSyncLFQueuePushContainer CellSpursLFQueuePushContainer;
typedef CellSyncLFQueuePopContainer CellSpursLFQueuePopContainer;
#endif /* __SPU__ */

#endif /* __CELL_SPURS_LFQUEUE_TYPES_H__ */

/*
 * Local Variables:
 * mode: C
 * c-file-style: "stroustrup"
 * tab-width: 4
 * End:
 * vim:sw=4:sts=4:ts=4
 */
