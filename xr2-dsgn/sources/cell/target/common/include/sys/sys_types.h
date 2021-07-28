/*   SCE CONFIDENTIAL                                       */
/*   PlayStation(R)3 Programmer Tool Runtime Library 310.001 */
/*   Copyright (C) 2009 Sony Computer Entertainment Inc.    */
/*   All Rights Reserved.                                   */

#ifndef __SYS_SYS_SYS_TYPES_H__
#define __SYS_SYS_SYS_TYPES_H__

#include <sys/cdefs.h>
#include <sys/integertypes.h>

#ifdef __cplusplus
#endif /* __cplusplus */


/*
 * Timer
 */
typedef __CSTD uint64_t callout_t;
typedef __CSTD uint64_t timer_t;

/*
 * Data types
 */
__C_STD_BEGIN
#ifdef __SPU__
#ifdef __COMMON_SIZE_T__
typedef uint32_t size_t;
#else /* __COMMON_SIZE_T__ */
typedef unsigned long size_t;
#endif /* __COMMON_SIZE_T__ */

#else /* __SPU__ */
#ifdef __LP32__
typedef __CSTD uint32_t size_t;
typedef __CSTD int32_t off_t;
#else /* __LP32__ */
typedef __CSTD uint64_t size_t;
typedef __CSTD int64_t off_t;
#endif /* __LP32__ */
typedef __CSTD int64_t off64_t;
typedef __CSTD int64_t time_t;
#define _TIME_T

#endif /* __SPU__ */

__C_STD_END
/*
 * Process
 */
/** \brief process id (opaque value) */
typedef __CSTD uint32_t sys_pid_t;

/* \brief program segment id (opaque value) */
typedef __CSTD uint32_t sys_program_segment_t;

/* \brief overlay module id (opaque value) */
typedef __CSTD uint32_t sys_overlay_t;


/*
 * PPU thread
 */
typedef __CSTD uint64_t sys_ppu_thread_t;

/** \brief interrupt tag id (opaque value for users) */
typedef __CSTD uint32_t sys_interrupt_tag_t;

/*
 * Synchronization
 */
typedef __CSTD uint32_t sys_mutex_t;
typedef __CSTD uint32_t sys_cond_t;
typedef __CSTD uint32_t sys_shared_mutex_t;
typedef __CSTD uint32_t sys_sync_point_t;
typedef __CSTD uint32_t sys_rwlock_t;

typedef __CSTD uint32_t sys_protocol_t;
typedef __CSTD uint32_t sys_recursive_t;
typedef __CSTD uint32_t sys_process_shared_t;
typedef __CSTD uint32_t sys_adaptive_t;

typedef __CSTD uint32_t _sys_sleep_queue_t;
typedef __CSTD uint32_t _sys_lwcond_queue_t;
typedef __CSTD uint32_t sys_semaphore_t;
typedef __CSTD uint32_t sys_event_flag_t;


/*
 * Event 
 */
typedef __CSTD uint32_t sys_event_queue_t;
typedef __CSTD uint32_t sys_event_port_t;
typedef __CSTD uint32_t sys_event_type_t;

/* 
 * SPU Thread
 */
typedef __CSTD uint32_t sys_spu_thread_group_t;
typedef __CSTD uint32_t sys_spu_thread_t;

/*
 * SPU System Module
 */
typedef __CSTD uint32_t sys_spu_system_module_t;


/*
 * Invalid values for IDs
 */
#define SYS_SPU_THREAD_GROUP_ID_INVALID     0xFFFFFFFFU
#define SYS_SPU_THREAD_ID_INVALID           0xFFFFFFFFU
#define SYS_SPU_SYSTEM_MODULE_ID_INVALID    0xFFFFFFFFU


/*
 * timer
 */
typedef __CSTD uint32_t sys_timer_t;


/*
 * Time
 */
typedef __CSTD int64_t system_time_t;
typedef __CSTD uint64_t usecond_t;
typedef __CSTD uint32_t second_t;

/* IPC key */
typedef __CSTD uint64_t sys_ipc_key_t;

/*
 * Addresses and memory
 */
typedef __CSTD uintptr_t lparaddr_t;
typedef __CSTD uintptr_t sys_addr_t;
typedef __CSTD uint32_t sys_memory_t;
typedef __CSTD uint32_t sys_memory_container_t;


/*
 * System call trace
 */
typedef __CSTD uint32_t sys_trace_t;
typedef __CSTD uint32_t sys_trace_buffer_t;



/*
 * Data type for PPU GUID information
 */
__C_STD_BEGIN typedef struct sys_ppu_guid_info {
	const char magic[5];		/*  5 bytes : '\177'GUID              */
	const __CSTD uint8_t id_revision;	/*  1 byte  : PPU GUID revision (0x1) */
	const __CSTD uint16_t reserved1;	/*  2 bytes : Reserved (0x0)          */
	const __CSTD uint32_t ppu_guid[5];	/* 20 bytes : PPU GUID                */
	const __CSTD uint32_t reserved2;	/*  4 bytes : Reserved (0x0)          */
} sys_ppu_guid_info_t;

__C_STD_END
/*
 * Data types for SDK Library
 */
	__C_STD_BEGIN
#ifndef __SPU__
#ifdef __LP32__
typedef int32_t ssize_t;
#else /* __LP32__ */
typedef int64_t ssize_t;
#endif /* __LP32__ */
typedef int mode_t;
typedef long int useconds_t;
typedef long key_t;
typedef uint32_t pid_t;
typedef unsigned long dev_t;
typedef unsigned long ino_t;
typedef unsigned long nlink_t;
typedef long blkcnt_t;
typedef long blksize_t;
#endif /* __SPU__ */

__C_STD_END
#if defined(__STD_USING)
	using __CSTD size_t;
#ifndef __SPU__
using __CSTD off_t;
using __CSTD off64_t;
using __CSTD ssize_t;
using __CSTD mode_t;
using __CSTD useconds_t;
using __CSTD key_t;
using __CSTD dev_t;
using __CSTD ino_t;
using __CSTD nlink_t;
using __CSTD blkcnt_t;
using __CSTD blksize_t;
using __CSTD time_t;
#endif /* __SPU__ */
using __CSTD sys_ppu_guid_info_t;

#endif /* defined(__STD_USING) */


#endif /* __SYS_SYS_SYS_TYPES_H__ */
