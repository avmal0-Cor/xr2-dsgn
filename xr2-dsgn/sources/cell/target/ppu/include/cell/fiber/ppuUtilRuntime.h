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
#include <string.h>
#include <stdio.h>

#include <sys/ppu_thread.h>
#include <sys/synchronization.h>
#include <cell/sysmodule.h>

#include <cell/fiber/ppu_fiber_types.h>
#include <cell/fiber/ppu_fiber.h>

#include <cell/fiber/ppu_fiber_worker_control.h>
#include <cell/fiber/ppuUtilDefine.h>

#include <ppu_intrinsics.h>

__CELL_FIBER_PPU_UTIL_BEGIN

static inline void workerEntry(uint64_t);

class Runtime : public CellFiberPpuUtilWorkerControl {
public:
	static const uint32_t	MAX_WORKER_PPU_THREAD = 16;
	static const uint32_t	DEFAULT_NUM_PPU_THREAD = 2;
	static const uint32_t	DEFAULT_PPU_THREAD_PRIORITY = 1000;
	static const uint32_t	DEFAULT_PPU_THREAD_STACK_SIZE = 4096;

private:
	Runtime(const Runtime&);
	Runtime& operator = (const Runtime&);

	unsigned int		numWorkerThread;
	sys_ppu_thread_t	workerThread[MAX_WORKER_PPU_THREAD];
	bool				isAutoLoad;

	union State {
		uint64_t	u64;
		__extension__ struct {
			uint16_t			numCreatingFiber;
			uint8_t				isShutdowned;
			uint8_t				lockShutdown;
			uint32_t			__reserved__;
		};
	} state;

public:
	Runtime () {};

	static int initialize (Runtime* runtime,
							unsigned int priority = DEFAULT_PPU_THREAD_PRIORITY,
							unsigned int numWorker = DEFAULT_NUM_PPU_THREAD,
							bool autoPrxLoad = true,
							bool autoCheckFlags = false,
							uint32_t autoCheckFlagsIntervalUsec = 0)
	{
		int		ret;
		if (runtime == 0) {
			return CELL_FIBER_ERROR_NULL_POINTER;
		}
		if ((uintptr_t)runtime & 0x7f) {
			return CELL_FIBER_ERROR_ALIGN;
		}
		if (numWorker > MAX_WORKER_PPU_THREAD) {
			return CELL_FIBER_ERROR_INVAL;
		}

		if (autoPrxLoad) {
			/* load the libfiber PRX */
			ret = cellSysmoduleLoadModule(CELL_SYSMODULE_FIBER);
			if (ret) {
				return ret;
			}
		}
		ret = cellFiberPpuInitialize();
		assert(ret == CELL_OK);	/* never fails */

		/* initialize CellFiberPpuUtil */
		CellFiberPpuUtilWorkerControlAttribute	attr;
		ret = cellFiberPpuUtilWorkerControlAttributeInitialize(&attr);
		assert(ret == CELL_OK);	/* never fails */

		attr.scheduler.debuggerSupport = true;
		attr.scheduler.autoCheckFlags = autoCheckFlags;
		attr.scheduler.autoCheckFlagsIntervalUsec = autoCheckFlagsIntervalUsec;

		ret = cellFiberPpuUtilWorkerControlInitializeWithAttribute(runtime, &attr);
		if (ret) {
			if(autoPrxLoad) {
				cellSysmoduleUnloadModule(CELL_SYSMODULE_FIBER);
			}
			return ret;
		}

		/* create PPU worker thread */
		for(unsigned int i = 0; i < numWorker; i++) {
			char	threadName[28];
			sprintf(threadName, "_fiberRuntime_%08x_%02u", (uintptr_t)runtime, i);
			ret = sys_ppu_thread_create (&runtime->workerThread[i], workerEntry, (uintptr_t)runtime, priority, DEFAULT_PPU_THREAD_STACK_SIZE, SYS_PPU_THREAD_CREATE_JOINABLE, threadName);
			if (ret) {
				int	ret2;
				ret2 = cellFiberPpuUtilWorkerControlWakeup(runtime);
				assert(ret2 == CELL_OK);
				ret2 = cellFiberPpuUtilWorkerControlShutdown(runtime);
				assert(ret2 == CELL_OK);
				/* join worker ppu thread */
				for (unsigned j = 0; j < i; j++) {
					uint64_t	exitCode;
					sys_ppu_thread_join(runtime->workerThread[j], &exitCode);
				}
				ret2 = cellFiberPpuUtilWorkerControlFinalize(runtime);
				assert(ret2 == CELL_OK);
				if(autoPrxLoad) {
					cellSysmoduleUnloadModule(CELL_SYSMODULE_FIBER);
				}
				return ret;
			}
		}
		runtime->numWorkerThread = numWorker;
		runtime->isAutoLoad = autoPrxLoad;
		runtime->state.u64 = 0;
		return ret;
	};

	int	finalize() {
		int	ret;
		if (this == 0) {
			return CELL_FIBER_ERROR_NULL_POINTER;
		}
		if ((uintptr_t)this & 127) {
			return CELL_FIBER_ERROR_ALIGN;
		}
		if (!state.isShutdowned) {
			return CELL_FIBER_ERROR_STAT;
		}
		/* join all worker thread */
		for (unsigned i = 0; i < numWorkerThread; i++) {
		uint64_t	exitCode;
			ret =sys_ppu_thread_join(workerThread[i], &exitCode);
			/* ignore error */
		}

		/* finalize utility */
		ret = cellFiberPpuUtilWorkerControlFinalize(this);
		/* ignore error */

		if(isAutoLoad) {
			/* unload the libfiber PRX */
			ret = cellSysmoduleUnloadModule(CELL_SYSMODULE_FIBER);
			assert(ret == CELL_OK);
		}

		return CELL_OK;
	};

	int	wakeup(void) {
		return cellFiberPpuUtilWorkerControlWakeup(this);
	};

	int	enablePolling(int timeout) {
		return cellFiberPpuUtilWorkerControlSetPollingMode(this, CELL_FIBER_PPU_UTIL_WORKER_CONTROL_POLLING_ENABLE, timeout);
	};
	int	disablePolling() {
		return cellFiberPpuUtilWorkerControlSetPollingMode(this, CELL_FIBER_PPU_UTIL_WORKER_CONTROL_POLLING_DISABLE, 0);
	};

	int	shutdown() {
		if (this == 0) {
			return CELL_FIBER_ERROR_NULL_POINTER;
		}
		if ((uintptr_t)this & 127) {
			return CELL_FIBER_ERROR_ALIGN;
		}
		State	atomic;
		do {
			atomic.u64 = __ldarx(&state.u64);
			if (atomic.numCreatingFiber > 0) {
				return CELL_FIBER_ERROR_BUSY;
			}
			atomic.lockShutdown = true;
		} while (0 == __stdcx(&state.u64, atomic.u64));

		int	ret = cellFiberPpuUtilWorkerControlShutdown(this);
		if (ret == CELL_OK) {
			state.isShutdowned = true;
		} else {
			state.lockShutdown = false;
		}
		return ret;
	};

	/* scheduler interfaces */
	/* fiber interfaces */
	int createFiber(CellFiberPpu *fiber,
					CellFiberPpuEntry entry,
					uint64_t arg,
					void *eaStack,
					size_t sizeStack,
					const char* name,
					size_t lenName,
					unsigned int priority = 1,
					CellFiberPpuOnExitCallback callback = 0)
	{
		if (this == 0) {
			return CELL_FIBER_ERROR_NULL_POINTER;
		}
		if ((uintptr_t)this & 127) {
			return CELL_FIBER_ERROR_ALIGN;
		}
		CellFiberPpuAttribute	attr;
		cellFiberPpuAttributeInitialize(&attr);
		if (name && (lenName > 0)) {
			if(lenName > CELL_FIBER_PPU_NAME_MAX_LENGTH) {
				return CELL_FIBER_ERROR_INVAL;
			}
			strncpy(attr.name, name, lenName);
		}
		if (callback) {
			attr.onExitCallback = callback;
		}
		State	atomic;
		do {
			atomic.u64 = __ldarx(&state.u64);
			if (atomic.lockShutdown > 0) {
				return CELL_FIBER_ERROR_STAT;
			}
			atomic.numCreatingFiber++;
		} while (0 == __stdcx(&state.u64, atomic.u64));
		int	ret;
		ret = cellFiberPpuUtilWorkerControlCreateFiber(this, fiber, entry, arg, priority, eaStack, sizeStack, &attr);
		do {
				atomic.u64 = __ldarx(&state.u64);
				atomic.numCreatingFiber--;
		} while (0 == __stdcx(&state.u64, atomic.u64));
		return ret;
	}

	int createFiber(CellFiberPpu *fiber,
					CellFiberPpuEntry entry,
					uint64_t arg,
					void *eaStack,
					size_t sizeStack,
					unsigned int priority = 1,
					CellFiberPpuOnExitCallback callback = 0)
	{
		return createFiber(fiber, entry, arg, eaStack, sizeStack, 0, 0, priority, callback);
	};

	int joinFiber(CellFiberPpu *fiber, int* exitCode)
	{
		int	ret;
		ret = cellFiberPpuUtilWorkerControlJoinFiber(this, fiber, exitCode);
		return ret;
	};

	static int sendSignal(CellFiberPpu *fiber, unsigned* numWorker = 0)
	{
		return cellFiberPpuUtilWorkerControlSendSignal(fiber, numWorker);
	}

} __attribute__((aligned(CELL_FIBER_PPU_UTIL_WORKER_CONTROL_ALIGN)));

static inline void
workerEntry(uint64_t arg)
{
	Runtime	*runtime = (Runtime*)(uintptr_t)arg;
	cellFiberPpuUtilWorkerControlRunFibers(runtime);
	sys_ppu_thread_exit(0);
}

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
