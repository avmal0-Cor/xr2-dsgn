/*
	SCE CONFIDENTIAL
	PlayStation(R)3 Programmer Tool Runtime Library 310.001
	Copyright (C) 2008 Sony Computer Entertainment Inc.
	All Rights Reserved.
*/
/**
	\internal
	\file fios_platform_imp_ps3ppu.h

	PS3 PPU implementation of some platform-specific functions found in FIOS.
*/
#ifndef __CELL_FIOS_PLATFORM_IMP_PS3PPU_H__
#define __CELL_FIOS_PLATFORM_IMP_PS3PPU_H__

#include <sys/types.h>
#include <sys/fixed_addr.h>
#include <sys/system_types.h>
#include <sys/time_util.h>
#include <sys/sys_time.h>
#include <sys/process.h>
#include <cell/atomic.h>
#include <pthread.h>
#include <time.h>
#include <ppu_intrinsics.h>   // __isync() and __lwsync()
#include <sdk_version.h>
#include "alloca.h"


// We can use sys_lwmutex_t and sys_lwcond_t if they are available in the SDK.



namespace cell {
namespace fios {
	namespace platform {

		/** \def FIOS_SCHEDULERTHREAD_STACKSIZE
			\brief Size of the scheduler thread's stack.
                */
		#ifndef FIOS_SCHEDULERTHREAD_STACKSIZE
		#define FIOS_SCHEDULERTHREAD_STACKSIZE	(64*1024)
		#endif
		
		/** \def FIOS_SCHEDULERTHREAD_PRIORITY

			\brief Priority of the scheduler thread. For
                        PS3 at the time of writing this appears
                        basically arbitrary; 0 to 3071 with 0 being
                        the highest. I chose 100.
                */
		#ifndef FIOS_SCHEDULERTHREAD_PRIORITY
		#define FIOS_SCHEDULERTHREAD_PRIORITY   (100)
		#endif

		/** \def FIOS_IOTHREAD_STACKSIZE
		    \brief Size of any media I/O thread's stack.
                */
		#ifndef FIOS_IOTHREAD_STACKSIZE
        #define FIOS_IOTHREAD_STACKSIZE         (32*1024)
        #endif
        
		/** \def FIOS_IOTHREAD_PRIORITY
			\brief Priority of any media I/O thread. For
			PS3 at the time of writing this appears
			basically arbitrary; 0 to 3071 with 0 being
			the highest. I chose 99.
                */
		#ifndef FIOS_IOTHREAD_PRIORITY
		#define FIOS_IOTHREAD_PRIORITY          (99)
		#endif

		/** \def FIOS_COMPUTETHREAD_STACKSIZE
			\brief Size of any media compute thread's stack.
                */
		#ifndef FIOS_COMPUTETHREAD_STACKSIZE
		#define FIOS_COMPUTETHREAD_STACKSIZE    (64*1024)
		#endif
		
		/** \def FIOS_COMPUTETHREAD_PRIORITY
			\brief Priority of any media compute
                        thread. For PS3 at the time of writing this
                        appears basically arbitrary; 0 to 3071 with 0
                        being the highest. I chose 257.
                */
		#ifndef FIOS_COMPUTETHREAD_PRIORITY
		#define FIOS_COMPUTETHREAD_PRIORITY     (257)
		#endif

		/** \def FIOS_NORMALTHREAD_STACKSIZE
			\brief Size of any normal thread's stack.
                */
		#ifndef FIOS_NORMALTHREAD_STACKSIZE
		#define FIOS_NORMALTHREAD_STACKSIZE    (32*1024)
		#endif
		
		/** \def FIOS_NORMALTHREAD_PRIORITY
			\brief Priority of any normal thread. For PS3 at
			the time of writing this appears basically
			arbitrary; 0 to 3071 with 0 being the
			highest. I chose 1000.
                */
		#ifndef FIOS_NORMALTHREAD_PRIORITY
		#define FIOS_NORMALTHREAD_PRIORITY          (1000)
		#endif

		/** \def FIOS_LOWPRIORITYTHREAD_STACKSIZE
			\brief Size of any low-priority thread's stack.
                */
		#ifndef FIOS_LOWPRIORITYTHREAD_STACKSIZE
		#define FIOS_LOWPRIORITYTHREAD_STACKSIZE    (32*1024)
		#endif
		
		/** \def FIOS_LOWPRIORITYTHREAD_PRIORITY
			\brief Priority of any low-priority
			thread. For PS3 at the time of writing this
			appears basically arbitrary; 0 to 3071 with 0
			being the highest. I chose 1500.
                */
		#ifndef FIOS_LOWPRIORITYTHREAD_PRIORITY
		#define FIOS_LOWPRIORITYTHREAD_PRIORITY     (1500)
		#endif
		
		/** \def FIOS_PATH_MAX
			\brief Maximum length of a media-relative path.
                */
		#ifndef FIOS_PATH_MAX
		# define FIOS_PATH_MAX                 (256)
		#endif
		
		/** \internal
			\brief Abstime is platform-specific.  For PS3,
                        we use the PowerPC timebase as our abstime.
                */
		typedef int64_t abstime_t;
		
		/** \internal
			\brief Native date is platform-specific.  For
			PS3, we use struct tm. */
		typedef struct tm nativedate_t;
		
		/** \internal \brief Native thread type */
		typedef sys_ppu_thread_t nativethread_t;

		// Use lwmutex/lwcond on newer SDKs because they're lighterweight (often no kernel transition needed)
		/** \internal \brief Native mutex type */
		typedef sys_lwmutex_t nativemutex_t FIOS_ATTRIBUTE((__aligned__(8)));

		/** \internal \brief Native cond type */
		typedef sys_lwcond_t nativecond_t FIOS_ATTRIBUTE((__aligned__(8)));

		/** \internal \brief Native rwlock type */
		typedef sys_rwlock_t nativerwlock_t;

		/** \def FIOS_HAS_NATIVERWLOCK
			\brief If we are using lwmutex/lwcond, don't
			use the native rwlock -- implement our own
			using lwmutex/lwcond.
                */
		#ifndef FIOS_HAS_NATIVERWLOCK
		#define FIOS_HAS_NATIVERWLOCK     (!FIOS_USE_LWMUTEX)
		#endif

		/** \internal \brief Native filehandle type
			PS3 uses an int, but because they stupidly force us to close and reopen files when the BD is ejected,
			we use a pointer to a tracking struct as our native FD type. */
		typedef struct ps3fd * nativefd_t;
		
		/** \internal \brief Illegal filehandle value, used as a marker */
		const nativefd_t kINVALID_FILEHANDLE = NULL;

		/** \internal \brief Fake filehandle value, used by catalog cache and others. Must be different from #kINVALID_FILEHANDLE and either invalid or unlikely. */
		const nativefd_t kFAKE_FILEHANDLE = reinterpret_cast<nativefd_t>(ptrdiff_t(-1LL));
		
		// --------------------------------------------------------------------
		// Atomic operations
		// --------------------------------------------------------------------
		//
		inline bool atomicCompareAndSwap(volatile uint32_t *pPtr, uint32_t oldValue, uint32_t newValue) {
			return FIOS_LIKELY(cellAtomicCompareAndSwap32(const_cast<uint32_t*>(pPtr),oldValue,newValue) == oldValue);
		}
		inline bool atomicCompareAndSwap(volatile uint64_t *pPtr, uint64_t oldValue, uint64_t newValue) {
			return FIOS_LIKELY(cellAtomicCompareAndSwap64(const_cast<uint64_t*>(pPtr),oldValue,newValue) == oldValue);
		}
		inline void atomicStore(volatile uint32_t *pPtr, uint32_t value) {
			__lwsync(); // flush pending stores
			*pPtr = value;
		}
		inline void atomicStore(volatile uint64_t *pPtr, uint64_t value) {
			__lwsync(); // flush pending stores
			*pPtr = value;
		}
		inline uint32_t atomicLoad(const volatile uint32_t *pPtr) {
			uint32_t result = *pPtr;
			__isync(); // flush speculative loads
			return result;
		}
		inline uint64_t atomicLoad(const volatile uint64_t *pPtr) {
			uint64_t result = *pPtr;
			__isync(); // flush speculative loads
			return result;
		}

		// --------------------------------------------------------------------
		//	Time
		// --------------------------------------------------------------------
		
		extern const double g_nanosecondsToAbstime;
		
		/* Gets the current native time. */
		inline FIOS_LINKAGE abstime_t currentTime()
		{
			register abstime_t now;
			SYS_TIMEBASE_GET(now);
			return now;
		}

		/* Converts from native time to nanoseconds. */
		inline FIOS_LINKAGE uint64_t timeToNanoseconds(abstime_t abstime)
		{
			// Converting with FP is way faster than doing the conversion in 128-bit integer
			// math. To get any faster you'd have to hardcode the frequency (either directly,
			// indirectly by making certain assumptions, or via dynamic codegen).
			// We lose precision beyond 64 bits, but normally this function is only used for
			// delta-time values which are far less than 64 bits.
			return (uint64_t)(abstime / g_nanosecondsToAbstime);
		}

		/* Converts from nanoseconds to native time. */
		inline FIOS_LINKAGE abstime_t nanosecondsToTime(uint64_t nanos)
		{
			// Converting with FP is way faster than doing the conversion in 128-bit integer
			// math. To get any faster you'd have to hardcode the frequency (either directly,
			// indirectly by making certain assumptions, or via dynamic codegen).
			// We lose precision beyond 64 bits, but normally this function is only used for
			// delta-time values which are far less than 64 bits.
			return (uint64_t)(nanos * g_nanosecondsToAbstime);
		}

		/* Performs a quick address-only check to see if a pointer is in uncached memory. */
		inline FIOS_LINKAGE bool isUncachedMemory(const void *pPtr) {
			uint32_t addr = (uint32_t)pPtr;
			return (addr >= uint32_t(RSX_FB_BASE_ADDR) && addr <= uint32_t(0xD0000000UL)) ||
			       (addr >= uint32_t(RAW_SPU_BASE_ADDR));
		}

		/* Checks to see if the pointer is in non-DMAable memory. */
		inline FIOS_LINKAGE bool isNonDMAableMemory(const void *pPtr) {
			return sys_process_is_stack(const_cast<void*>(pPtr));
		}
	}; /* namespace platform */
}; /* namespace fios */
}; /* namespace cell */

#endif /* __CELL_FIOS_PLATFORM_IMP_PS3PPU_H__ */
