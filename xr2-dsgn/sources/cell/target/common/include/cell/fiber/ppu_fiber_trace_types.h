/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2008 Sony Computer Entertainment Inc.
 * All Rights Reserved.
 */

#ifndef __CELL_FIBER_PPU_TRACE_TYPES_H__
#define __CELL_FIBER_PPU_TRACE_TYPES_H__ 1

#include <stdint.h>

/* one trace buffer for each fiber scheduler */

/*
 * Fiber scheduler trace 
 */

typedef struct CellFiberPpuTraceInfo {
	uint32_t	writeIndex;
	uint32_t	numEntry;
	uint8_t		__reserved;
	uint8_t		padding[128 
					- sizeof(uint32_t) * 2
					- sizeof(uint8_t)
					];
} CellFiberPpuTraceInfo;

typedef struct CellFiberPpuTraceHeader {
	uint8_t		tag;		/* the type of information */
	uint8_t		length;		/* fixed with 2 */
	uint8_t		ppu;		/* PPU thread number: not used */
	uint8_t		__reserved;	/* not used */
	uint32_t	timebase;	/* lower 32 bits of TB */
} CellFiberPpuTraceHeader;

/* CellFiberPpuTracePacketHeader.tag */
#define	CELL_FIBER_PPU_TRACE_TAG_BASE		0x80

#define CELL_FIBER_PPU_TRACE_TAG_DISPATCH		(CELL_FIBER_PPU_TRACE_TAG_BASE + 0)
#define CELL_FIBER_PPU_TRACE_TAG_RESUME			(CELL_FIBER_PPU_TRACE_TAG_BASE + 1)
#define CELL_FIBER_PPU_TRACE_TAG_EXIT			(CELL_FIBER_PPU_TRACE_TAG_BASE + 2)
#define CELL_FIBER_PPU_TRACE_TAG_YIELD			(CELL_FIBER_PPU_TRACE_TAG_BASE + 3)
#define CELL_FIBER_PPU_TRACE_TAG_SLEEP			(CELL_FIBER_PPU_TRACE_TAG_BASE + 4)

#define	CELL_FIBER_PPU_TRACE_TAG_NAME			(CELL_FIBER_PPU_TRACE_TAG_BASE + 5)

typedef struct CellFiberPpuTraceSchedulerPayload {
	uint32_t	idFiber;
	uint32_t	idPpuThread;
} CellFiberPpuTraceSchedulerPayload;


#ifdef __GNUC__
#define CELL_FIBER_ALIGN_PREFIX(x)
#define CELL_FIBER_ALIGN_POSTFIX(x) __attribute__((aligned(x)))
#else /* __GNUC__ */
#define CELL_FIBER_ALIGN_PREFIX(x) __declspec(align(x))
#define CELL_FIBER_ALIGN_POSTFIX(x)
#endif /* __GNUC__ */

typedef CELL_FIBER_ALIGN_PREFIX(16) struct CellFiberPpuTracePacket {
	CellFiberPpuTraceHeader header;
	CellFiberPpuTraceSchedulerPayload	data;
} CELL_FIBER_ALIGN_POSTFIX(16) CellFiberPpuTracePacket;


#define	CELL_FIBER_PPU_TRACE_PACKET_SIZE	16
#define	CELL_FIBER_PPU_TRACE_BUFFER_ALIGN	16

#define CELL_FIBER_PPU_TRACE_MODE_FLAG_WRAP_BUFFER 0x1
#define CELL_FIBER_PPU_TRACE_MODE_FLAG_SYNCHRONOUS_START_STOP 0x2
#define CELL_FIBER_PPU_TRACE_MODE_FLAG_MASK 0x3

#ifdef	__PPU__
#include <cell/fiber/ppu_context.h>

/*
 * Fiber name trace 
 */
typedef CELL_FIBER_ALIGN_PREFIX(16) struct CellFiberPpuNameTraceInfo {
	uint32_t write;						/* write offset */
	uint32_t read;						/* read offset */
	uint32_t last;						/* last offset */
	uint32_t mode;						/* 0=stop, 1=overflow */
	uint32_t is_started;
	uint8_t  padding[128 - sizeof(uint32_t)*5];
} CellFiberPpuNameTraceInfo CELL_FIBER_ALIGN_POSTFIX(16);

typedef CELL_FIBER_ALIGN_PREFIX(16) struct CellFiberPpuNameTracePacket {
	CellFiberPpuTraceHeader header;
	uint32_t	idFiber;
	char		name[CELL_FIBER_PPU_CONTEXT_NAME_MAX_LENGTH + 1];
} CellFiberPpuNameTracePacket CELL_FIBER_ALIGN_POSTFIX(16);

#endif	/* __PPU__ */

#endif /* __CELL_FIBER_PPU_TRACE_TYPES_H__ */

/*
 * Local Variables:
 * mode: C
 * c-file-style: "stroustrup"
 * tab-width: 4
 * End:
 * vim:sw=4:sts=4:ts=4
 */
