/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2008 Sony Computer Entertainment Inc.
 * All Rights Reserved.
 */

#ifndef __CELL_PPU_FIBER_WORKER_CONTROL_H__
#define __CELL_PPU_FIBER_WORKER_CONTROL_H__

#include <stdint.h>
#include <stdbool.h>

#ifdef	__PPU__
#include <sys/synchronization.h>
#include <sys/event.h>
#endif	/* __PPU__ */
#include <cell/fiber/version.h>
#include <cell/fiber/ppu_fiber_types.h>
#include <cell/spurs/types.h>

#define	CELL_FIBER_PPU_UTIL_WORKER_CONTROL_SIZE	(CELL_FIBER_PPU_SCHEDULER_SIZE + 128 * 2)
#define	CELL_FIBER_PPU_UTIL_WORKER_CONTROL_ALIGN	128

typedef struct CellFiberPpuUtilWorkerControl
{
	uint8_t	skip[CELL_FIBER_PPU_UTIL_WORKER_CONTROL_SIZE];
} __attribute__((aligned(CELL_FIBER_PPU_UTIL_WORKER_CONTROL_ALIGN))) CellFiberPpuUtilWorkerControl;

#define	CELL_FIBER_PPU_UTIL_WORKER_CONTROL_ATTRIBUTE_SIZE	(CELL_FIBER_PPU_SCHEDULER_ATTRIBUTE_SIZE + 128)
#define	CELL_FIBER_PPU_UTIL_WORKER_CONTROL_ATTRIBUTE_ALIGN	8

typedef	struct CellFiberPpuUtilWorkerControlAttribute
{
	CellFiberPpuSchedulerAttribute	scheduler;
	uint64_t	privateHeader[16 / sizeof(uint64_t)];
	uint8_t	__reserved[
		CELL_FIBER_PPU_UTIL_WORKER_CONTROL_ATTRIBUTE_SIZE
		- sizeof(CellFiberPpuSchedulerAttribute)
		- 16
		];
} __attribute__((aligned(CELL_FIBER_PPU_UTIL_WORKER_CONTROL_ATTRIBUTE_ALIGN))) CellFiberPpuUtilWorkerControlAttribute;

#define	CELL_FIBER_PPU_UTIL_WORKER_CONTROL_POLLING_DISABLE	0
#define	CELL_FIBER_PPU_UTIL_WORKER_CONTROL_POLLING_ENABLE	1

#define	CELL_FIBER_PPU_UTIL_NUM_MAX_WORKER	32

#ifdef __cplusplus
extern "C" {
#endif
/* fiber control interface */

#ifdef	__PPU__
int
cellFiberPpuUtilWorkerControlInitialize(CellFiberPpuUtilWorkerControl *control);

int
_cellFiberPpuUtilWorkerControlAttributeInitialize(CellFiberPpuUtilWorkerControlAttribute*, uint32_t  sdkVersion);

static inline
int cellFiberPpuUtilWorkerControlAttributeInitialize(CellFiberPpuUtilWorkerControlAttribute* attr)
{
	return _cellFiberPpuUtilWorkerControlAttributeInitialize(attr, _CELL_FIBER_PPU_INTERNAL_VERSION);
}

int
cellFiberPpuUtilWorkerControlInitializeWithAttribute(CellFiberPpuUtilWorkerControl *control, CellFiberPpuUtilWorkerControlAttribute*);

int
cellFiberPpuUtilWorkerControlFinalize(CellFiberPpuUtilWorkerControl *control);

int
cellFiberPpuUtilWorkerControlRunFibers(CellFiberPpuUtilWorkerControl *control);

int
cellFiberPpuUtilWorkerControlShutdown(CellFiberPpuUtilWorkerControl *control);

int
cellFiberPpuUtilWorkerControlWakeup(CellFiberPpuUtilWorkerControl *control);

int
cellFiberPpuUtilWorkerControlSetPollingMode(CellFiberPpuUtilWorkerControl *control, int mode, int timeout);

int
cellFiberPpuUtilWorkerControlCreateFiber(CellFiberPpuUtilWorkerControl *control, CellFiberPpu* fiber, CellFiberPpuEntry entry, uint64_t arg, unsigned int priority, void* eaStack, size_t sizeStack, const CellFiberPpuAttribute* attr);

int
cellFiberPpuUtilWorkerControlJoinFiber(CellFiberPpuUtilWorkerControl *control, CellFiberPpu* fiber, int* exitCode);

int
cellFiberPpuUtilWorkerControlSendSignal(CellFiberPpu* fiber, unsigned* numWorker);
#else	/* __PPU__ */
int
cellFiberPpuUtilWorkerControlWakeup(uint32_t eaControl);

int
cellFiberPpuUtilWorkerControlSendSignal(uint32_t eaFiber, unsigned* numWorker);
#endif	/* __PPU__ */

#ifdef __cplusplus
} /* extern "C" */
#endif

#ifdef	__PPU__
static inline
CellFiberPpuScheduler* cellFiberPpuUtilWorkerControlGetScheduler(CellFiberPpuUtilWorkerControl *control)
{
	return (CellFiberPpuScheduler*)(uintptr_t)control->skip;
}
#else	/* __PPU__ */
static inline
uint32_t cellFiberPpuUtilWorkerControlGetScheduler(uint32_t eaControl)
{
	return eaControl;
}
#endif	/* __PPU__ */

#endif /* __CELL_PPU_FIBER_WORKER_CONTROL_H__ */

/*
 * Local Variables:
 * mode: C
 * c-file-style: "stroustrup"
 * tab-width: 4
 * End:
 * vim:sw=4:sts=4:ts=4
 */
