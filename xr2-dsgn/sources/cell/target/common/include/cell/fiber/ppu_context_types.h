/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2008 Sony Computer Entertainment Inc.
 * All Rights Reserved.
 */

#ifndef __CELL_FIBER_PPU_CONTEXT_TYPES_H__
#define __CELL_FIBER_PPU_CONTEXT_TYPES_H__

#include <stdint.h>
#include <stdbool.h>

typedef struct CellFiberPpuContextExecutionOption CellFiberPpuContextExecutionOption;

#define	CELL_FIBER_PPU_CONTEXT_STACK_ALIGN	16

#define	CELL_FIBER_PPU_CONTEXT_ALIGN	16
#define	CELL_FIBER_PPU_CONTEXT_SIZE		(128 * 5)

#define CELL_FIBER_PPU_CONTEXT_NAME_MAX_LENGTH	31

#define	CELL_FIBER_PPU_CONTEXT_ATTRIBUTE_ALIGN	8
#define	CELL_FIBER_PPU_CONTEXT_ATTRIBUTE_SIZE	128

typedef struct CellFiberPpuContextAttribute {
	uint8_t privateHeader[16];
	char name[CELL_FIBER_PPU_CONTEXT_NAME_MAX_LENGTH + 1];
	bool debuggerSupport;

	uint8_t skip[CELL_FIBER_PPU_CONTEXT_ATTRIBUTE_SIZE - 16
	                                                   - (CELL_FIBER_PPU_CONTEXT_NAME_MAX_LENGTH + 1)
	                                                   - sizeof(bool)
	                                                  ];
} __attribute__((aligned(CELL_FIBER_PPU_CONTEXT_ATTRIBUTE_ALIGN))) CellFiberPpuContextAttribute;

typedef struct CellFiberPpuContext {
    unsigned char skip[CELL_FIBER_PPU_CONTEXT_SIZE];
} __attribute__((aligned(CELL_FIBER_PPU_CONTEXT_ALIGN))) CellFiberPpuContext;

typedef void (*CellFiberPpuContextEntry)(
		uint64_t arg,
		CellFiberPpuContext* fiberFrom
	);

#endif /* __CELL_FIBER_PPU_CONTEXT_TYPES_H__ */

/*
 * Local Variables:
 * mode: C
 * c-file-style: "stroustrup"
 * tab-width: 4
 * End:
 * vim:sw=4:sts=4:ts=4
 */
