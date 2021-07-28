/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2006 Sony Computer Entertainment Inc.
* All Rights Reserved.
*/

#ifndef __CELL_SPURS_QUEUE_TYPES_H__
#define __CELL_SPURS_QUEUE_TYPES_H__ 1

typedef enum CellSpursQueueDirection {
	CELL_SPURS_QUEUE_SPU2SPU,
	CELL_SPURS_QUEUE_SPU2PPU,
	CELL_SPURS_QUEUE_PPU2SPU
} CellSpursQueueDirection;

#define CELL_SPURS_QUEUE_ALIGN		128
#define CELL_SPURS_QUEUE_SIZE		128
typedef struct CellSpursQueue {
	unsigned char skip[CELL_SPURS_QUEUE_SIZE];
} __attribute__((aligned(CELL_SPURS_QUEUE_ALIGN))) CellSpursQueue;

#endif /* __CELL_SPURS_QUEUE_TYPES_H__ */

/*
 * Local Variables:
 * mode: C
 * c-file-style: "stroustrup"
 * tab-width: 4
 * End:
 * vim:sw=4:sts=4:ts=4
 */
