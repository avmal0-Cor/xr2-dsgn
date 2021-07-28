/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2008 Sony Computer Entertainment Inc.
 * All Rights Reserved.
 */
#ifndef __CELL_SPURS_TYPES_H__
#define __CELL_SPURS_TYPES_H__ 1

/* The C Standard Library */
#include <stdint.h>
#include <stdbool.h>

/* SPU C/C++ Language Extention */
#ifdef __SPU__
#include <spu_intrinsics.h>
#endif /* __SPU__ */

/* For types provided by Cell OS Lv-2 */
#include <sys/sys_types.h>

/*
 * SPU runtime system
 */
/* typical usage:
 * CellSpurs *spurs = (CellSpurs*)memalign(CELL_SPURS_ALIGN, CELL_SPURS_SIZE); */

#define CELL_SPURS_ALIGN			128
#define CELL_SPURS_SIZE				4096

typedef struct CellSpurs {
	unsigned char skip[CELL_SPURS_SIZE];
} __attribute__((aligned(CELL_SPURS_ALIGN))) CellSpurs;

#define CELL_SPURS_MAX_SPU			8
#define CELL_SPURS_NAME_MAX_LENGTH	15

#define	CELL_SPURS_ATTRIBUTE_ALIGN	8
#define	CELL_SPURS_ATTRIBUTE_SIZE	512

typedef struct CellSpursAttribute {
	unsigned char	skip[CELL_SPURS_ATTRIBUTE_SIZE];
} __attribute__((aligned(CELL_SPURS_ATTRIBUTE_ALIGN))) CellSpursAttribute;

/*
 * workload
 */
#define CELL_SPURS_MAX_WORKLOAD		16
#define CELL_SPURS_MAX_PRIORITY		16
typedef unsigned CellSpursWorkloadId;
typedef struct {
	uint64_t _unused0;
	uint32_t _unused1;
	uint32_t flag;						/* zero or not */
} __attribute__((aligned(16))) CellSpursWorkloadFlag;

/*
 * SPURS Local Storage layout
 */
#define CELL_SPURS_INTERRUPT_VECTOR		0x000
#define CELL_SPURS_LOCK_LINE			0x080
#define CELL_SPURS_KERNEL_CONTEXT		0x100

/*
 * DMA tag used by the SPURS kernel and modules
 */
#define CELL_SPURS_KERNEL_DMA_TAG_ID	31

/* for backward compatibility */
#define CELL_SPURS_INTERUPT_VECTOR		0x000

/* SPU thread event port */
#define	CELL_SPURS_PORT_RANGE_END			63

#define	CELL_SPURS_STATIC_PORT_NUM			16
#define	CELL_SPURS_STATIC_PORT_RANGE_BOTTOM	(CELL_SPURS_STATIC_PORT_NUM - 1)

#define	CELL_SPURS_DYNAMIC_PORT_RANGE_TOP	(CELL_SPURS_STATIC_PORT_NUM)
#define	CELL_SPURS_DYNAMIC_PORT_RANGE_BOTTOM	CELL_SPURS_PORT_RANGE_END
#define	CELL_SPURS_DYNAMIC_PORT_NUM			(CELL_SPURS_DYNAMIC_PORT_RANGE_BOTTOM - CELL_SPURS_STATIC_PORT_BOTTOM)

/* for cellSpursGetInfo() */
typedef struct CellSpursInfo {
	int nSpus;
	int spuThreadGroupPriority;
	int ppuThreadPriority;
	bool exitIfNoWork;
	uint8_t	__padding24[3];
	void     *traceBuffer;
	uint32_t __padding32;
	uint64_t traceBufferSize;
	uint32_t traceMode;

	sys_spu_thread_group_t  spuThreadGroup;
	sys_spu_thread_t        spuThreads[CELL_SPURS_MAX_SPU];
	__extension__ union {
		sys_ppu_thread_t        spursHandelerThread0;
		sys_ppu_thread_t        spursHandlerThread0;
	};
	__extension__ union {
		sys_ppu_thread_t        spursHandelerThread1;
		sys_ppu_thread_t        spursHandlerThread1;
	};

	char namePrefix[CELL_SPURS_NAME_MAX_LENGTH+1];
	size_t namePrefixLength;

	uint32_t deadlineMissCounter;
	uint32_t deadlineMeetCounter;

	uint8_t padding[280
					- sizeof(int) * 3
					- sizeof(bool)
					- sizeof(uint8_t) * 3
					- sizeof(void*)
					- sizeof(uint32_t)
					- sizeof(uint64_t)
					- sizeof(uint32_t)
					- sizeof(sys_spu_thread_group_t)
					- sizeof(sys_spu_thread_t) * CELL_SPURS_MAX_SPU
					- sizeof(sys_ppu_thread_t) * 2
					- sizeof(char) * (CELL_SPURS_NAME_MAX_LENGTH+1)
					- sizeof(size_t)
					- sizeof(uint32_t) * 2
			];
} CellSpursInfo;

#endif /* __CELL_SPURS_TYPES_H__ */
/*
 * Local Variables:
 * mode: C
 * c-file-style: "stroustrup"
 * tab-width: 4
 * End:
 * vim:sw=4:sts=4:ts=4
 */
