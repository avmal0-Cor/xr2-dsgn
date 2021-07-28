/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2008 Sony Computer Entertainment Inc.
 * All Rights Reserved.
 */

#ifndef __CELL_FIBER_PPU_UTIL_RUNTIME_H__
#define __CELL_FIBER_PPU_UTIL_RUNTIME_H__

#include <stdint.h>
#include <stdbool.h>
#include <assert.h>

#include <cell/fiber/ppu_fiber_types.h>
#include <cell/fiber/ppu_fiber.h>

#include <cell/fiber/ppu_fiber_worker_control.h>
#include <cell/fiber/ppuUtilDefine.h>

__CELL_FIBER_PPU_UTIL_BEGIN

class Runtime {
public:
	static int wakeup(uint64_t eaRuntime) {
		return cellFiberPpuUtilWorkerControlWakeup(eaRuntime);
	};

	static int sendSignal(uint32_t eaFiber, unsigned *numWorker = 0) {
		return cellFiberPpuUtilWorkerControlSendSignal(eaFiber, numWorker);
	}
};

__CELL_FIBER_PPU_UTIL_END

#endif /* __CELL_FIBER_PPU_UTIL_RUNTIME_H__ */

/*
 * Local Variables:
 * mode: C++
 * c-file-style: "stroustrup"
 * tab-width: 4
 * End:
 * vim:sw=4:sts=4:ts=4
 */
