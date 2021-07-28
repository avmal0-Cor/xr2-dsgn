/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2008 Sony Computer Entertainment Inc.
 * All Rights Reserved.
 */

#ifndef __CELL_SPURS_TRACE_TYPES_H__
#define __CELL_SPURS_TRACE_TYPES_H__ 1

#include <stdint.h>

typedef struct CellSpursTraceInfo {
	uint32_t spu_thread[8];
	uint32_t count[8];
	uint32_t spu_thread_grp;
	uint32_t nspu;
	uint8_t  padding[128 - sizeof(uint32_t)*(8+8+2)];
} CellSpursTraceInfo;

typedef struct CellSpursTraceHeader {
	uint8_t tag;						/* the type of information */
	uint8_t length;						/* fixed with 2 */
	uint8_t spu;						/* SPU number */
	uint8_t workload;					/* workload id */
	uint32_t time;						/* compliment of decrementer count */
} CellSpursTraceHeader;

/* CellSpursTracePacketHeader.tag */
#define CELL_SPURS_TRACE_TAG_KERNEL		0x20
#define CELL_SPURS_TRACE_TAG_SERVICE	0x21
#define CELL_SPURS_TRACE_TAG_TASK		0x22
#define CELL_SPURS_TRACE_TAG_JOB		0x23
#define CELL_SPURS_TRACE_TAG_OVIS		0x24
/* user trace */
#define CELL_SPURS_TRACE_TAG_LOAD		0x2a
#define CELL_SPURS_TRACE_TAG_MAP		0x2b
#define CELL_SPURS_TRACE_TAG_START		0x2c
#define CELL_SPURS_TRACE_TAG_STOP		0x2d
#define CELL_SPURS_TRACE_TAG_USER		0x2e
#define CELL_SPURS_TRACE_TAG_GUID		0x2f

#define CELL_SPURS_TRACE_TAG_CONTROL	0xf0

typedef struct CellSpursTraceControlData {
	uint32_t incident;
	uint32_t idSpuThread;
} CellSpursTraceControlData;

/* CellSpursTracePacketControlData.incident */
#define CELL_SPURS_TRACE_CONTROL_START	0x01
#define CELL_SPURS_TRACE_CONTROL_STOP	0x02

typedef struct CellSpursTraceServiceData {
	uint32_t incident;
	uint32_t __reserved__;
} CellSpursTraceServiceData;

/* CellSpursTracePacketServiceData.incident */
#define CELL_SPURS_TRACE_SERVICE_INIT	0x01
#define CELL_SPURS_TRACE_SERVICE_WAIT	0x02
#define CELL_SPURS_TRACE_SERVICE_EXIT	0x03

typedef struct CellSpursTraceTaskData {
	uint32_t incident;
	uint32_t task;
} CellSpursTraceTaskData;

/* CellSpursTraceTaskData.incident */
#define CELL_SPURS_TRACE_TASK_DISPATCH	0x01
#define CELL_SPURS_TRACE_TASK_YIELD		0x03
#define CELL_SPURS_TRACE_TASK_WAIT		0x04
#define CELL_SPURS_TRACE_TASK_EXIT		0x05

typedef struct CellSpursTraceJobData {
	uint8_t  __reserved__[3];
	uint8_t  binLSAhigh8;				/* higher 8bit address */
	uint32_t jobDescriptor;
} CellSpursTraceJobData;

typedef struct CellSpursTraceLoadData {
	uint32_t ea;
	uint16_t ls;						/* 16-byte aligned address (lower 4 bits are dropped) */
	uint16_t size;
} CellSpursTraceLoadData;

typedef struct CellSpursTraceMapData {
	uint32_t offset;					/* offset from base effective address */
	uint16_t ls;						/* 16-byte aligned address (lower 4 bits are dropped) */
	uint16_t size;
} CellSpursTraceMapData;

typedef struct CellSpursTraceStartData {
	char module[4];						/* 4 ASCII letter (e.g. "TASK") */
	uint16_t level;						/* level of execution (1:policy module, 2:its work unit */
	uint16_t ls;						/* 4-byte aligned address (lower 2 bits are dropped) */
} CellSpursTraceStartData;

#ifdef __GNUC__
#define CELL_SPURS_ALIGN_PREFIX(x)
#define CELL_SPURS_ALIGN_POSTFIX(x) __attribute__((aligned(x)))
#else /* __GNUC__ */
#define CELL_SPURS_ALIGN_PREFIX(x) __declspec(align(x))
#define CELL_SPURS_ALIGN_POSTFIX(x)
#endif /* __GNUC__ */

typedef CELL_SPURS_ALIGN_PREFIX(16) struct CellSpursTracePacket {
	CellSpursTraceHeader header;
	union {
		CellSpursTraceControlData control;
		CellSpursTraceServiceData service;
		CellSpursTraceTaskData task;
		CellSpursTraceJobData job;
										/* user trace */
		CellSpursTraceLoadData load;	/* CELL_SPURS_TRACE_TAG_LOAD */
		CellSpursTraceMapData map;		/* CELL_SPURS_TRACE_TAG_MAP */
		CellSpursTraceStartData start;	/* CELL_SPURS_TRACE_TAG_START */
		uint64_t stop;					/* CELL_SPURS_TRACE_TAG_STOP */
		uint64_t user;					/* CELL_SPURS_TRACE_TAG_USER */
		uint64_t guid;					/* CELL_SPURS_TRACE_TAG_GUID */
		uint64_t rawData;
	} data;
} CELL_SPURS_ALIGN_POSTFIX(16) CellSpursTracePacket;

#define	CELL_SPURS_TRACE_PACKET_SIZE	16
#define	CELL_SPURS_TRACE_BUFFER_ALIGN	16

#define CELL_SPURS_TRACE_MODE_FLAG_WRAP_BUFFER 0x1
#define CELL_SPURS_TRACE_MODE_FLAG_SYNCHRONOUS_START_STOP 0x2
#define CELL_SPURS_TRACE_MODE_FLAG_MASK 0x3

#endif /* __CELL_SPURS_TRACE_TYPES_H__ */

/*
 * Local Variables:
 * mode: C
 * c-file-style: "stroustrup"
 * tab-width: 4
 * End:
 * vim:sw=4:sts=4:ts=4
 */
