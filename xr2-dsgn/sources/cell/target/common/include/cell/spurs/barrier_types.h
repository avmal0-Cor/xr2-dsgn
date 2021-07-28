/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2006 Sony Computer Entertainment Inc.
* All Rights Reserved.
*/

#ifndef __CELL_SPURS_BARRIER_TYPES_H__
#define __CELL_SPURS_BARRIER_TYPES_H__ 1

#define CELL_SPURS_BARRIER_ALIGN	128
#define CELL_SPURS_BARRIER_SIZE		128

typedef struct CellSpursBarrier {
	unsigned char skip[CELL_SPURS_BARRIER_SIZE];
} __attribute__((aligned(CELL_SPURS_BARRIER_ALIGN))) CellSpursBarrier;

#endif /* __CELL_SPURS_BARRIER_TYPES_H__ */

/*
 * Local Variables:
 * mode: C
 * c-file-style: "stroustrup"
 * tab-width: 4
 * End:
 * vim:sw=4:sts=4:ts=4
 */
