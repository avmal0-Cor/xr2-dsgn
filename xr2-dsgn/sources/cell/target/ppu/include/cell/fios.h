/*
	SCE CONFIDENTIAL
	PlayStation(R)3 Programmer Tool Runtime Library 310.001
	Copyright (C) 2009 Sony Computer Entertainment Inc.
	All Rights Reserved.
*/
/**
	\file fios.h

	Public API for the File I/O Scheduler.
*/
/**
	\mainpage
 
	\version FIOS 1.3.2
	
*/

#ifndef __CELL_FIOS_H__
#define __CELL_FIOS_H__

/*  System headers.  */
#include <stddef.h> /* size_t, ptrdiff_t */

/*
	Configuration. This is the only place that we explicitly don't use a relative
	include, so that you may override it easily by putting your own copy of this file
	higher in the include search path.
*/
#include <cell/fios/fios_configuration.h>

/* Forward declarations. */
typedef struct fios_parameters fios_parameters;

/*  FIOS support classes.  */
#include <cell/fios/fios_memory.h>
#include <cell/fios/fios_common.h>
#include <cell/fios/fios_types.h>
#include <cell/fios/fios_platform.h>
#include <cell/fios/fios_collections.h>
#include <cell/fios/fios_paths.h>
#include <cell/fios/fios_decompressor.h>
#include <cell/fios/fios_watch.h>

/*  FIOS primary classes.  */
#include <cell/fios/fios_media.h>
#include <cell/fios/fios_scheduler.h>

/*  FIOS media filter layers.  */
#include <cell/fios/fios_dearchive.h>
#include <cell/fios/fios_caching.h>
#include <cell/fios/fios_overlay.h>
#include <cell/fios/fios_emulation.h>

#include <cell/fios/ps3/ppu/fios_ps3ppu.h>

/**	\brief Namespace for PS3 SDK. */
namespace cell {
/**	\brief Everything in the FIOS API is in the fios namespace. */
namespace fios {

#ifndef DOXYGEN_IGNORE

#define kFIOS_VERSION_STAGE_d      0
#define kFIOS_VERSION_STAGE_a      1
#define kFIOS_VERSION_STAGE_b      2
#define kFIOS_VERSION_STAGE_rc     3
#define kFIOS_VERSION_STAGE_f      4

#define FIOS_VERSION_MAJOR            ((FIOS_VERSION >> 24) & 0x00FF)
#define FIOS_VERSION_MINOR            ((FIOS_VERSION >> 20) & 0x000F)
#define FIOS_VERSION_BUGFIX           ((FIOS_VERSION >> 16) & 0x000F)
#define FIOS_VERSION_STAGE            ((FIOS_VERSION >> 12) & 0x000F)
#define FIOS_VERSION_STAGENUM         ((FIOS_VERSION >>  0) & 0x0FFF)

#define FIOS_VERSION_FROM_COMPONENTS(major, minor, bugfix, stage, stagenum) \
	(                                                 \
	((major & 0xFF) << 24)                          | \
	((minor & 0x0F) << 20)                          | \
	((bugfix & 0x0F) << 16)                         | \
	((kFIOS_VERSION_STAGE_##stage & 0x0F) << 12)    | \
	((stagenum & 0x0FFF) << 0)                        \
	)

#define FIOS_VERSION_STRING_FROM_COMPONENTS(major, minor, bugfix, stage, stagenum) \
	 "" #major "." #minor "." #bugfix

/**
	\def	FIOS_VERSION
	\brief  The FIOS header version as BCD.

	This indicates the version of the FIOS API header you are using. Newer versions
	always compare numerically greater than older versions.
    
	A new version scheme was started at FIOS 1.0.0rc1. The version scheme used for
	0.91 and earlier releases was different: 0.91 was 0x0091, etc. The older
	scheme still obeys numeric ordering relative to the new scheme: 0x0091 is less than 0x01004001.
*/
#define FIOS_VERSION	FIOS_VERSION_FROM_COMPONENTS(1,3,2,f,0)

/**
	\def    FIOS_VERSION_STRING
	\brief  The FIOS header version as a string.

	This indicates the version of the FIOS API header you are using, in a human-readable
	format suitable for printing. Please note: this value is not a C-string constant.
	Instead, it's an expression that evaluates to a C-string constant.
*/
#define FIOS_VERSION_STRING    FIOS_VERSION_STRING_FROM_COMPONENTS(1,3,2,f,0)

#endif // DOXYGEN_IGNORE

/** \brief Function pointer used for large memcpy operations.
	\param[in] pDst   Destination buffer. Never overlaps the source buffer.
	\param[in] pSrc   Source buffer. Never overlaps the destination buffer.
	\param[in] len    Number of bytes to copy. Always at least 4KiB.
	\return The original value of pDst should be returned. */
typedef void *(*memcpy_proc)(void * FIOS_RESTRICT pDst, const void * FIOS_RESTRICT pSrc, size_t len);

/** \brief Function pointer used for text output.
	\param[in] pFmt   Format as in vprintf.
	\param[in] args   A va_list of additional arguments.
	\return The return value is always ignored by FIOS and is provided only for compatibility with the standard prototype for vprintf. */
typedef int (*vprintf_proc)(const char * FIOS_RESTRICT pFmt, va_list args);

/** \brief Bitmask used to control TTY profiling output. */
enum e_PROFILING {
	kProfileOps              = (1<<0),   //!< Show op lifecycle: creation, execution, completion
	kProfileCache            = (1<<1),   //!< Show cache events
	kProfileDearchiver       = (1<<2),   //!< Show dearchiver events
	kProfileOverlay          = (1<<3),   //!< Show dearchiver events
	kProfileMediaAccess      = (1<<4),   //!< Show media-access events (e.g. cellFsRead)
	
	kProfileAll              = kProfileOps | kProfileCache |
		kProfileDearchiver | kProfileOverlay | kProfileMediaAccess
};

/** \brief Initialization parameters. */
typedef struct fios_parameters {
public:
	// Required parameters
	size_t          sizeOfInitParameters;      //!< Set to sizeof(fios_parameters). Guards against header/library mismatch.
	uint32_t        fiosVersion;               //!< Set to FIOS_VERSION. Guards against header/library mismatch.
	allocator       *pAllocator;               //!< The global allocator used for allocations that are not tied to a scheduler. Schedulers can be set to use this allocator, or a separate allocator.
	
	uint16_t        pathSize;                  //!< Maximum path length supported. Defaults to FIOS_PATH_MAX and cannot be increased beyond FIOS_PATH_MAX. Can be decreased to save memory.

	// Optional hooks
	memcpy_proc     pLargeMemcpy;              //!< Function used for memcpy operations that are 4KiB or larger. If NULL, FIOS uses standard memcpy.
	vprintf_proc    pVprintf;                  //!< Function used for all debug/informative output. If pVprintf is NULL, FIOS uses standard vprintf.
	
	// TTY profiling output
	uint32_t        profiling;                 //!< Profiling mask, see e_PROFILING values.

	// Optional thread parameters
	int   threadPriority[platform::kNumberOfThreadTypes];  //!< Thread priorities. If 0, use the value defined in fios_platform_imp_ps3.h
	int   threadStackSize[platform::kNumberOfThreadTypes]; //!< Thread stack sizes. If 0, use the value defined in fios_platform_imp_ps3.h
} fios_parameters;

/** \brief Initializer to set a #cell::fios::fios_parameters struct to reasonable defaults. This is optional -- you can also just use zeroes. */
#define FIOS_PARAMETERS_INITIALIZER     { sizeof(cell::fios::fios_parameters), FIOS_VERSION, NULL, FIOS_PATH_MAX, NULL, NULL, 0, \
	{ FIOS_SCHEDULERTHREAD_PRIORITY, FIOS_IOTHREAD_PRIORITY, FIOS_COMPUTETHREAD_PRIORITY, FIOS_NORMALTHREAD_PRIORITY, FIOS_LOWPRIORITYTHREAD_PRIORITY }, \
	{ FIOS_SCHEDULERTHREAD_STACKSIZE, FIOS_IOTHREAD_STACKSIZE, FIOS_COMPUTETHREAD_STACKSIZE, FIOS_NORMALTHREAD_STACKSIZE, FIOS_LOWPRIORITYTHREAD_STACKSIZE }}

/*
	Global functions.
*/

/** \brief  Initializes FIOS (old)
	\param[in]  pAllocator   The global allocator used for allocations that are not tied to a scheduler, such as media objects. Schedulers can be set to use this allocator, or a separate allocator.
	
	This allocator pointer is remembered internally, and the object it points to must live for as long as
	FIOS is running. Avoid declaring your allocator object on the stack (except perhaps in main) -- it's usually
	better as a global. */
extern FIOS_LINKAGE
void FIOSInit(allocator *pAllocator)
	FIOS_ATTRIBUTE_NONNULL((__nonnull__));

/** \brief  Initializes FIOS (new)
	\param[in]  pParameters   A list of parameters for FIOS initialization.
	
	The contents of the param struct are copied to a global inside FIOS, so it doesn't need to be persistent.
	However, any objects that are pointed to (like the allocator and logger) need to persist for the lifetime
	of FIOS.
	\see fios_parameters */
extern FIOS_LINKAGE
void FIOSInit(const fios_parameters *pParameters)
	FIOS_ATTRIBUTE_NONNULL((__nonnull__));

/** \brief Indicates whether FIOS has been initialized.
	\return True if FIOS has been initialized. */
extern FIOS_LINKAGE
bool FIOSHasBeenInited(void);

#ifndef DOXYGEN_IGNORE
/** \brief Returns the global FIOS memory allocator.
	\return The global allocator, or NULL if FIOS has not been initialized. */
extern FIOS_LINKAGE
allocator * FIOSGetAllocator(void);
#endif // DOXYGEN_IGNORE

/** \brief Returns the global FIOS parameter struct.
	\return The FIOS parameter structure. Changing its contents at runtime may lead to crashes or unpredictable behavior. */
extern FIOS_LINKAGE
fios_parameters * FIOSGetParameters(void);

/** \brief Terminates FIOS

	This call is not normally necessary. However, it may be helpful if you are planning on fully unloading
	FIOS from memory. When you call it, FIOS frees any internally allocated memory and forgets the allocator
	that it was initialized with. Subsequent calls to #FIOSHasBeenInited() returns false.
	
	Before making this call you should terminate and release any active schedulers with scheduler::destroyScheduler(),
	and then free any FIOS objects (such as media layers) which you have allocated.

	Please note that FIOS keeps the performance data that it has gathered on your drive's real-world
	performance with the scheduler, so terminating the scheduler with scheduler::destroyScheduler() resets the
	performance data to less accurate default values.
*/
extern FIOS_LINKAGE
void FIOSTerminate(void);

}; /* namespace fios */
}; /* namespace cell */

/* Promote these functions to the top-level namespace so that they can be used without namespace qualifiers. */
using cell::fios::FIOSInit;
using cell::fios::FIOSHasBeenInited;
using cell::fios::FIOSGetAllocator;
using cell::fios::FIOSGetParameters;
using cell::fios::FIOSTerminate;

#endif /* __CELL_FIOS_H__ */

