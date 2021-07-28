/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2008 Sony Computer Entertainment Inc.
 * All Rights Reserved.
 */

#ifndef __CELL_FIBER_USER_SCHEDULER_H__
#define __CELL_FIBER_USER_SCHEDULER_H__

#include <cell/fiber/error.h>

#ifdef __cplusplus
extern "C" {
#endif

#include <cell/fiber/ppu_context_types.h>
#include <cell/fiber/user_scheduler_types.h>

int cellFiberPpuContextRunScheduler(
		CellFiberPpuSchedulerCallback scheduler,
		uint64_t arg0, uint64_t arg1,
		int *cause,
		CellFiberPpuContext **fiberFrom,
		const CellFiberPpuContextExecutionOption* option
	);

int cellFiberPpuContextEnterScheduler(
		CellFiberPpuSchedulerCallback scheduler,
		uint64_t arg0,
		uint64_t arg1,
		CellFiberPpuContext** fiberFrom,
		const CellFiberPpuContextExecutionOption* option
	);
	
#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* __CELL_FIBER_USER_SCHEDULER_H__ */

/*
 * Local Variables:
 * mode: C
 * c-file-style: "stroustrup"
 * tab-width: 4
 * End:
 * vim:sw=4:sts=4:ts=4
 */
