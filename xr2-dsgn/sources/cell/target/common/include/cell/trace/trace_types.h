/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2008 Sony Computer Entertainment Inc.
 * All Rights Reserved.
 */

#ifndef __CELL_TRACE_TRACE_TYPES_H__
#define __CELL_TRACE_TRACE_TYPES_H__ 1

#include <stdint.h>

/* Trace packet header */
typedef struct __attribute__((aligned(8))) CellTraceHeader {
	uint8_t tag;    /* packet tag */
	uint8_t length; /* length of payload in word */
	uint8_t cpu;    /* processor ID */
	uint8_t thread; /* thread ID */
	uint32_t time;  /* lower 32bit timebase */
} CellTraceHeader;

#endif /* __CELL_TRACE_TRACE_TYPES_H__ */
