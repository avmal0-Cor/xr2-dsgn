/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2008 Sony Computer Entertainment Inc.
 * All Rights Reserved.
 */

#ifndef __CELL_FIBER_PPU_CONTEXT_H__
#define __CELL_FIBER_PPU_CONTEXT_H__

#include <stdint.h>

#include <cell/fiber/ppu_context_types.h>
#include <cell/fiber/ppu_initialize.h>
#include <cell/fiber/version.h>

#ifdef __cplusplus
extern "C" {
#endif

int _cellFiberPpuContextAttributeInitialize(
		CellFiberPpuContextAttribute* attr,
		uint32_t sdkVersion
	);

static inline
int cellFiberPpuContextAttributeInitialize(
		CellFiberPpuContextAttribute* attr
	)
{
	return _cellFiberPpuContextAttributeInitialize(attr, _CELL_FIBER_PPU_INTERNAL_VERSION);
}

int cellFiberPpuContextInitialize(
		CellFiberPpuContext* fiber,
		CellFiberPpuContextEntry entry,
		uint64_t arg,
		void* eaStack,
		size_t stackSize,
		const CellFiberPpuContextAttribute* attr
	);

int cellFiberPpuContextFinalize(CellFiberPpuContext* fiber);

int cellFiberPpuContextRun(
		CellFiberPpuContext	*fiberTo,
		int *cause,
		CellFiberPpuContext **fiberFrom,
		const CellFiberPpuContextExecutionOption* option
	);

int cellFiberPpuContextSwitch(
		CellFiberPpuContext* fiberTo,
		CellFiberPpuContext** fiberFrom,
		const CellFiberPpuContextExecutionOption* option
	);

int cellFiberPpuContextReturnToThread(int cause);

CellFiberPpuContext* cellFiberPpuContextSelf(void);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* __CELL_FIBER_PPU_CONTEXT_H__ */

/*
 * Local Variables:
 * mode: C
 * c-file-style: "stroustrup"
 * tab-width: 4
 * End:
 * vim:sw=4:sts=4:ts=4
 */
