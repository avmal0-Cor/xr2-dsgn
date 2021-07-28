/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2008 Sony Computer Entertainment Inc.
 * All Rights Reserved.
 */

#ifndef __CELL_FIBER_PPU_FIBER_TYPES_H__
#define __CELL_FIBER_PPU_FIBER_TYPES_H__

#include <stdint.h>
#include <stdbool.h>

#include <cell/fiber/ppu_context_types.h>

/* fiber */

typedef void (*CellFiberPpuOnExitCallback)(uint64_t arg, int exitCode);

#define CELL_FIBER_PPU_NAME_MAX_LENGTH	CELL_FIBER_PPU_CONTEXT_NAME_MAX_LENGTH

#define	CELL_FIBER_PPU_ATTRIBUTE_ALIGN	8
#define	CELL_FIBER_PPU_ATTRIBUTE_SIZE	256

typedef struct CellFiberPpuAttribute {
	uint8_t privateHeader[16];

	char name[CELL_FIBER_PPU_NAME_MAX_LENGTH + 1];

	CellFiberPpuOnExitCallback onExitCallback;
	//uint8_t padding[4];
	uint32_t	__reserved0__;	/* reserved for internal use..MUST BE 0 */
	uint64_t onExitCallbackArg;

	uint64_t	__reserved1__;	/* reserved for internal use..MUST BE 0 */

	uint8_t skip[CELL_FIBER_PPU_ATTRIBUTE_SIZE - 16
	                                           - (CELL_FIBER_PPU_NAME_MAX_LENGTH + 1)
	                                           - sizeof(CellFiberPpuOnExitCallback)
	                                           - sizeof(uint32_t)
	                                           - sizeof(uint64_t) * 2
	                                         ];
} __attribute__((aligned(CELL_FIBER_PPU_ATTRIBUTE_ALIGN))) CellFiberPpuAttribute;

#define	CELL_FIBER_PPU_STACK_ALIGN	16

#define	CELL_FIBER_PPU_ALIGN	128
#define	CELL_FIBER_PPU_SIZE		(CELL_FIBER_PPU_CONTEXT_SIZE + 128 * 2)

typedef struct CellFiberPpu {
	unsigned char skip[CELL_FIBER_PPU_SIZE];
} __attribute__((aligned(CELL_FIBER_PPU_ALIGN))) CellFiberPpu;


/* fiber scheduler */

#define	CELL_FIBER_PPU_SCHEDULER_ATTRIBUTE_ALIGN	8
#define	CELL_FIBER_PPU_SCHEDULER_ATTRIBUTE_SIZE	256


typedef struct CellFiberPpuSchedulerAttribute {
	uint8_t privateHeader[16];

	/* For periodic timer callback */
	bool autoCheckFlags;
	bool debuggerSupport;
	uint8_t padding[2];
	uint32_t autoCheckFlagsIntervalUsec;

	uint8_t skip[CELL_FIBER_PPU_ATTRIBUTE_SIZE - 16
	                                           - sizeof(bool) * 2
	                                           - 2
	                                           - sizeof(uint32_t)
	                                           ];
} __attribute__((aligned(CELL_FIBER_PPU_SCHEDULER_ATTRIBUTE_ALIGN))) CellFiberPpuSchedulerAttribute;

#define	CELL_FIBER_PPU_NUM_PRIORITY	4
#define	CELL_FIBER_PPU_LOWEST_PRIORITY	(CELL_FIBER_PPU_NUM_PRIORITY - 1)

#define	CELL_FIBER_PPU_SCHEDULER_ALIGN	128
#define	CELL_FIBER_PPU_SCHEDULER_SIZE	(128 * 4)

typedef struct CellFiberPpuScheduler {
	unsigned char skip[CELL_FIBER_PPU_SCHEDULER_SIZE];
} __attribute__((aligned(CELL_FIBER_PPU_SCHEDULER_ALIGN))) CellFiberPpuScheduler;


/* fuction prototypes */
typedef int (*CellFiberPpuEntry)(uint64_t arg);


#endif /* __CELL_FIBER_PPU_FIBER_TYPES_H__ */

/*
 * Local Variables:
 * mode: C
 * c-file-style: "stroustrup"
 * tab-width: 4
 * End:
 * vim:sw=4:sts=4:ts=4
 */
