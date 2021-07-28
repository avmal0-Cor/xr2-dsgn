/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2008 Sony Computer Entertainment Inc.
 * All Rights Reserved.
 */

#ifndef __CELL_FIBER_PPU_FIBER_H__
#define __CELL_FIBER_PPU_FIBER_H__

#include <stdint.h>
#include <stdbool.h>
#include <cell/fiber/ppu_initialize.h>
#include <cell/fiber/ppu_fiber_types.h>
#include <cell/fiber/version.h>

#ifdef __cplusplus
extern "C" {
#endif

/* scheduler interface */
int _cellFiberPpuSchedulerAttributeInitialize(
		CellFiberPpuSchedulerAttribute* attr,
		uint32_t sdkVersion
	);

static inline
int cellFiberPpuSchedulerAttributeInitialize(CellFiberPpuSchedulerAttribute* attr)
{
	return _cellFiberPpuSchedulerAttributeInitialize(attr, _CELL_FIBER_PPU_INTERNAL_VERSION);
}

int cellFiberPpuInitializeScheduler(
		CellFiberPpuScheduler* scheduler,
		const CellFiberPpuSchedulerAttribute* attr
	);

int cellFiberPpuFinalizeScheduler(CellFiberPpuScheduler* scheduler);

int cellFiberPpuRunFibers(CellFiberPpuScheduler* scheduler);

int cellFiberPpuCheckFlags(CellFiberPpuScheduler* scheduler);

int cellFiberPpuHasRunnableFiber(CellFiberPpuScheduler* scheduler, bool* flag);

int cellFiberPpuGetScheduler(CellFiberPpu* fiber, CellFiberPpuScheduler** pScheduler);

/* fiber interface */
int _cellFiberPpuAttributeInitialize(
		CellFiberPpuAttribute* attr,
		uint32_t sdkVersion
	);

static inline
int cellFiberPpuAttributeInitialize(CellFiberPpuAttribute* attr)
{
	return _cellFiberPpuAttributeInitialize(attr, _CELL_FIBER_PPU_INTERNAL_VERSION);
}

int cellFiberPpuCreateFiber(
			CellFiberPpuScheduler* scheduler,
			CellFiberPpu* fiber,
			CellFiberPpuEntry entry,
			uint64_t arg,
			unsigned int priority,
			void* eaStack,
			size_t sizeStack,
			const CellFiberPpuAttribute* attr
		);

CellFiberPpu* cellFiberPpuSelf(void);

int cellFiberPpuWaitFlag(uint32_t* eaFlag, bool flagValue);

int cellFiberPpuWaitSignal(void);

int cellFiberPpuSendSignal(CellFiberPpu* fiber, unsigned int *numWorker);

int cellFiberPpuYield(void);

int cellFiberPpuExit(int);

int cellFiberPpuJoinFiber(CellFiberPpu* fiber, int* exitCode);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* __CELL_FIBER_PPU_FIBER_H__ */

/*
 * Local Variables:
 * mode: C
 * c-file-style: "stroustrup"
 * tab-width: 4
 * End:
 * vim:sw=4:sts=4:ts=4
 */
