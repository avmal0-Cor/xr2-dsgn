/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2008 Sony Computer Entertainment Inc.
 * All Rights Reserved.
 */

#ifndef __CELL_FIBER_PPU_FIBER_TRACE_H__
#define __CELL_FIBER_PPU_FIBER_TRACE_H__ 1

/* The C Standard Library */
#include <stdint.h>
#include <sys/types.h> /* size_t */

/* FIBER */
#include <cell/fiber/ppu_fiber_types.h>			/* CellFiber */
#include <cell/fiber/ppu_fiber_trace_types.h>		/* CellFiberTracePacket */
#include <cell/fiber/error.h>

#ifdef __cplusplus
extern "C" {
#endif

int cellFiberPpuSchedulerTraceInitialize(CellFiberPpuScheduler*, void *buffer, size_t size, uint32_t mode);
int cellFiberPpuSchedulerTraceStart(CellFiberPpuScheduler*);
int cellFiberPpuSchedulerTraceStop(CellFiberPpuScheduler*);
int cellFiberPpuSchedulerTraceFinalize(CellFiberPpuScheduler*);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* __CELL_FIBER_PPU_FIBER_TRACE_H__ */

/*
 * Local Variables:
 * mode: C
 * c-file-style: "stroustrup"
 * tab-width: 4
 * End:
 * vim:sw=4:sts=4:ts=4
 */
