/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2007 Sony Computer Entertainment Inc.
 * All Rights Reserved.
 */

#ifndef __CELL_SPURS_EXCEPTION_COMMON_TYPES_H__
#define __CELL_SPURS_EXCEPTION_COMMON_TYPES_H__

#ifdef __cplusplus
extern "C" {
#endif

/* The C Standard Library */
#include <stdint.h>
#include <sys/sys_types.h>

/* size: 24 bytes */
typedef struct CellSpursExceptionInfo {
	sys_spu_thread_t spu_thread;
	uint32_t spu_npc;
	uint32_t cause;
	/* padding 4 bytes */
	uint64_t option;
} CellSpursExceptionInfo;

typedef void (*CellSpursExceptionEventHandler)(
		CellSpurs*,
		const CellSpursExceptionInfo*,
		void*
	);

typedef void (*CellSpursGlobalExceptionEventHandler)(
		CellSpurs*,
		const CellSpursExceptionInfo*,
		CellSpursWorkloadId id,
		void*
	);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* __CELL_SPURS_EXCEPTION_COMMON_TYPES_H__ */

