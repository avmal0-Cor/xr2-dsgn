/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2008 Sony Computer Entertainment Inc.
* All Rights Reserved.
*/
#ifndef __CELL_FIBER_SPU_FIBER_H__
#define __CELL_FIBER_SPU_FIBER_H__	1

#include <stdint.h>

#define CELL_FIBER_SPU_CONTEXT_SIZE		(128 * 7)
#define	CELL_FIBER_SPU_CONTEXT_ALIGN	16
#define	CELL_FIBER_SPU_CONTEXT_STACK_ALIGN	16

typedef struct CellFiberSpuContext {
	uint8_t	skip[CELL_FIBER_SPU_CONTEXT_SIZE];
} __attribute__((aligned(CELL_FIBER_SPU_CONTEXT_ALIGN))) CellFiberSpuContext;

typedef void (*CellFiberSpuContextEntry)(
		uint64_t arg
	);

#endif /* __CELL_FIBER_SPU_FIBER_H__ */

