/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2008 Sony Computer Entertainment Inc.
 * All Rights Reserved.
 */

#ifndef __CELL_SPURS_WORKLOAD_TYPES_H__
#define __CELL_SPURS_WORKLOAD_TYPES_H__

#include <stdint.h>
#include <cell/spurs/types.h>

#define	CELL_SPURS_WORKLOAD_ATTRIBUTE_ALIGN	8
#define	CELL_SPURS_WORKLOAD_ATTRIBUTE_SIZE	512

typedef struct CellSpursWorkloadAttribute {
	unsigned char	skip[CELL_SPURS_WORKLOAD_ATTRIBUTE_SIZE];
} __attribute__((aligned(CELL_SPURS_WORKLOAD_ATTRIBUTE_ALIGN))) CellSpursWorkloadAttribute;

typedef void (*CellSpursShutdownCompletionEventHook)(
		CellSpurs*,
		CellSpursWorkloadId,
		void*
	);

typedef struct CellSpursWorkloadInfo {
	uint64_t data;
	uint8_t priority[8];
	const void* policyModule;
	unsigned int sizePolicyModule;
	const char* nameClass;
	const char* nameInstance;
	uint8_t contention;
	uint8_t minContention;
	uint8_t maxContention;
	uint8_t readyCount;
	uint8_t idleSpuRequest;
	uint8_t hasSignal;
	uint8_t padding[2];
	CellSpursShutdownCompletionEventHook shutdownCompletionEventHook;
	void* shutdownCompletionEventHookArgument;
	uint8_t reserved[256
	                     - sizeof(uint64_t)
	                     - sizeof(uint8_t) * 8
	                     - sizeof(const void*)
	                     - sizeof(unsigned int)
	                     - sizeof(const char*) * 2
	                     - sizeof(uint8_t) * 8
	                     - sizeof(CellSpursShutdownCompletionEventHook)
	                     - sizeof(void*)
	                ];
} CellSpursWorkloadInfo;

#endif /* __CELL_SPURS_WORKLOAD_TYPES_H__ */
