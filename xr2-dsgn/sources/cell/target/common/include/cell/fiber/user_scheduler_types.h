/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2008 Sony Computer Entertainment Inc.
 * All Rights Reserved.
 */

#ifndef __CELL_FIBER_PPU_USER_SCHEDULER_TYPES_H__
#define __CELL_FIBER_PPU_USER_SCHEDULER_TYPES_H__

#include <stdint.h>

typedef CellFiberPpuContext* (*CellFiberPpuSchedulerCallback)(
		uint64_t arg0,
		uint64_t arg1
	);

#endif /* __CELL_FIBER_PPU_USER_SCHEDULER_TYPES_H__ */

/*
 * Local Variables:
 * mode: C
 * c-file-style: "stroustrup"
 * tab-width: 4
 * End:
 * vim:sw=4:sts=4:ts=4
 */
