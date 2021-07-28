/*   SCE CONFIDENTIAL                                       */
/*   PlayStation(R)3 Programmer Tool Runtime Library 310.001 */
/*   Copyright (C) 2009 Sony Computer Entertainment Inc.    */
/*   All Rights Reserved.                                   */

/**
 * \defgroup all_syscall System Calls
 */

/**
 * \ingroup all_syscall
 * \defgroup proc Process Management
 * \brief system calls defined in sys/process.h
 */
/*@{*/

#ifndef __SYS_SYS_PROCESS_H__
#define __SYS_SYS_PROCESS_H__

#include <sys/cdefs.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <sys/integertypes.h>
#include <sys/return_code.h>

#include <sdk_version.h>

CDECL_BEGIN



/* \brief option flag for sys_game_process_exitspawn API */
#define SYS_PROCESS_PRIMARY_STACK_SIZE_32K	0x0000000000000010ULL
#define SYS_PROCESS_PRIMARY_STACK_SIZE_64K	0x0000000000000020ULL
#define SYS_PROCESS_PRIMARY_STACK_SIZE_96K	0x0000000000000030ULL
#define SYS_PROCESS_PRIMARY_STACK_SIZE_128K	0x0000000000000040ULL
#define SYS_PROCESS_PRIMARY_STACK_SIZE_256K	0x0000000000000050ULL
#define SYS_PROCESS_PRIMARY_STACK_SIZE_512K	0x0000000000000060ULL
#define SYS_PROCESS_PRIMARY_STACK_SIZE_1M	0x0000000000000070ULL


#define SYS_PROCESS_PARAM_SECTION_NAME		".sys_proc_param"
#define SYS_PROCESS_PARAM_INVALID_PRIO		-32768
#define SYS_PROCESS_PARAM_INVALID_STACK_SIZE	0xffffffffUL

#define SYS_PROCESS_PARAM_STACK_SIZE_MIN	0x1000	/* 4KB */
#define SYS_PROCESS_PARAM_STACK_SIZE_MAX	0x100000	/* 1MB */

#define SYS_PROCESS_PARAM_VERSION_INVALID	0xffffffff
#define SYS_PROCESS_PARAM_VERSION_1		0x00000001	/* for SDK 08X */
#define SYS_PROCESS_PARAM_VERSION_084_0		0x00008400
#define SYS_PROCESS_PARAM_VERSION_090_0		0x00009000

#define SYS_PROCESS_PARAM_MAGIC			0x13bcc5f6
#define SYS_PROCESS_PARAM_MALLOC_PAGE_SIZE_NONE	0x00000000
#define SYS_PROCESS_PARAM_MALLOC_PAGE_SIZE_64K	0x00010000
#define SYS_PROCESS_PARAM_MALLOC_PAGE_SIZE_1M	0x00100000

#define SYS_PROCESS_PARAM_PPC_SEG_DEFAULT	0x00000000
#define SYS_PROCESS_PARAM_PPC_SEG_OVLM		0x00000001
#define SYS_PROCESS_PARAM_PPC_SEG_FIXEDADDR_PRX	0x00000002

#define SYS_PROCESS_PARAM_SDK_VERSION_UNKNOWN	0xffffffff


/*
 * Process Local Object
 *
 */
#define SYS_MEM_OBJECT                     (0x08UL)
#define SYS_MUTEX_OBJECT                   (0x85UL)
#define SYS_COND_OBJECT                    (0x86UL)
#define SYS_RWLOCK_OBJECT                  (0x88UL)
#define SYS_INTR_TAG_OBJECT                (0x0AUL)
#define SYS_INTR_SERVICE_HANDLE_OBJECT     (0x0BUL)
#define SYS_EVENT_QUEUE_OBJECT             (0x8DUL)
#define SYS_EVENT_PORT_OBJECT              (0x0EUL)
#define SYS_TRACE_OBJECT                   (0x21UL)
#define SYS_SPUIMAGE_OBJECT                (0x22UL)
#define SYS_PRX_OBJECT                     (0x23UL)
#define SYS_SPUPORT_OBJECT                 (0x24UL)
#define SYS_LWMUTEX_OBJECT                 (0x95UL)
#define SYS_TIMER_OBJECT                   (0x11UL)
#define SYS_SEMAPHORE_OBJECT               (0x96UL)
#define SYS_FS_FD_OBJECT                   (0x73UL)
#define SYS_LWCOND_OBJECT                  (0x97UL)
#define SYS_EVENT_FLAG_OBJECT              (0x98UL)


/* \brief process parameters */
typedef struct {
	uint32_t size;
	uint32_t magic;
	uint32_t version;
	uint32_t sdk_version;
	int32_t primary_prio;
	uint32_t primary_stacksize;
	uint32_t malloc_pagesize;
	uint32_t ppc_seg;
} sys_process_param_t;

#define SYS_PROCESS_PARAM_SECTION \
	__attribute__((aligned(8), section(SYS_PROCESS_PARAM_SECTION_NAME), unused))

#ifdef CELL_SDK_VERSION
#define SYS_PROCESS_PARAM( primary_prio, primary_stacksize ) \
	sys_process_param_t __sys_process_param SYS_PROCESS_PARAM_SECTION = { \
		sizeof(sys_process_param_t),		\
		SYS_PROCESS_PARAM_MAGIC,		\
		SYS_PROCESS_PARAM_VERSION_090_0,	\
		CELL_SDK_VERSION,			\
		primary_prio,				\
		primary_stacksize,			\
		SYS_PROCESS_PARAM_MALLOC_PAGE_SIZE_1M,	\
		SYS_PROCESS_PARAM_PPC_SEG_DEFAULT};
#define SYS_PROCESS_PARAM_OVLM( primary_prio, primary_stacksize ) \
	sys_process_param_t __sys_process_param SYS_PROCESS_PARAM_SECTION = { \
		sizeof(sys_process_param_t),		\
		SYS_PROCESS_PARAM_MAGIC,		\
		SYS_PROCESS_PARAM_VERSION_090_0,	\
		CELL_SDK_VERSION,			\
		primary_prio,				\
		primary_stacksize,			\
		SYS_PROCESS_PARAM_MALLOC_PAGE_SIZE_1M,	\
		SYS_PROCESS_PARAM_PPC_SEG_OVLM};
#define SYS_PROCESS_PARAM_FIXEDADDR_PRX( primary_prio, primary_stacksize ) \
	sys_process_param_t __sys_process_param SYS_PROCESS_PARAM_SECTION = { \
		sizeof(sys_process_param_t),		\
		SYS_PROCESS_PARAM_MAGIC,		\
		SYS_PROCESS_PARAM_VERSION_090_0,	\
		CELL_SDK_VERSION,			\
		primary_prio,				\
		primary_stacksize,			\
		SYS_PROCESS_PARAM_MALLOC_PAGE_SIZE_1M,	\
		SYS_PROCESS_PARAM_PPC_SEG_FIXEDADDR_PRX};
#else /* ifdef CELL_SDK_VERSION */
#define SYS_PROCESS_PARAM( primary_prio, primary_stacksize ) \
	sys_process_param_t __sys_process_param SYS_PROCESS_PARAM_SECTION = { \
		sizeof(sys_process_param_t),		\
		SYS_PROCESS_PARAM_MAGIC,		\
		SYS_PROCESS_PARAM_VERSION_090_0,	\
		SYS_PROCESS_PARAM_SDK_VERSION_UNKNOWN,	\
		primary_prio,				\
		primary_stacksize,			\
		SYS_PROCESS_PARAM_MALLOC_PAGE_SIZE_1M,	\
		SYS_PROCESS_PARAM_PPC_SEG_DEFAULT};
#endif /* ifdef CELL_SDK_VERSION */


/** \brief check whether an address is in stack or not */
extern int sys_process_is_stack(void *p);


/** \brief section specifiers
 *
 * There macros provide an access control for variables.  To specify these macro
 * to a tail of a variable,  user can change an access authority of the variable.
 *
 *                          PPU     SPU     RSX
 * _PPU_PRIVATE_DATA_       RW-     ---     ---
 * _PPU_PRIVATE_CONST_      R--     ---     ---
 * _RSX_IMAGE_              RW-     RW-     RW-
 *
 * For example;
 *
 *  char ppu_local_data[256] _PPU_PRIVATE_DATA_;
 *  static const char ppu_local_ro_data[] _PPU_PRIVATE_CONST_ = "secret-key";
 *
 * Note that these macro can not be used for auto variables.
 *
 */
#define _PPU_PRIVATE_DATA_  __attribute__((section (".ppu_data")))
#define _PPU_PRIVATE_CONST_ __attribute__((section (".ppu_rodata")))
#define _RSX_IMAGE_ __attribute__((section (".rsx_image")))


/** \brief memory mapping in the process address space (32-bit mode)
 * <PRE>
 *
 *   0xFFFF_FFFF           +-------------------------------+
 *                         | SPU Thread Mapping Area       |
 *   SPU_THREAD_BASE_ADDR  +-------------------------------+
 *                         | Raw SPU Mapping Area          |
 *   RAW_SPU_BASE_ADDR     +-------------------------------+
 *                         | User Area                     |
 *   0xD000_0000           +-------------------------------+
 *                         | RSX Frame Buffer Mapping Area |
 *   RSX_FB_BASE_ADDR      +-------------------------------+
 *                         |                               |
 *                         | User Area (heap, ...)         |
 *                         |                               |
 *                         |                               |
 *                         |                               |
 *                         +-------------------------------+
 *                         | PPU/SPU/RSX Shared Segment    |
 *                         | (.rsx_image)                  |
 *                         +-------------------------------+
 *                         | PPU Local Segment             |
 *                         | (.ppu_rodata, .ppu_data)      |
 *                         +-------------------------------+
 *                         | PPU/SPU Shared Segment        |
 *                         | (.text, .data, .bss, ...)     |
 *  TEXT_SEGMENT_BASE_ADDR +-------------------------------+
 *                         | Unmapped Area                 |
 *  0x0000_0000            +-------------------------------+
 *
 * </PRE>
 */

/** \brief  get the process id */
extern inline sys_pid_t sys_process_getpid(void);

extern inline sys_pid_t sys_process_getpid(void)
{
	system_call_0(SYS_PROCESS_GETPID);

#ifdef REG_PASS_SYS_PROCESS_GETPID
	REG_PASS_SYS_PROCESS_GETPID;
#endif
	return_to_user_prog(sys_pid_t);
}


/** \brief get the parent process id */
extern inline sys_pid_t sys_process_getppid(void);

extern inline sys_pid_t sys_process_getppid(void)
{
	system_call_0(SYS_PROCESS_GETPPID);

#ifdef REG_PASS_SYS_PROCESS_GETPPID
	REG_PASS_SYS_PROCESS_GETPPID;
#endif
	return_to_user_prog(sys_pid_t);
}


/** \brief terminate a calling game process and create a new game process */
extern void sys_game_process_exitspawn(const char *path,
									   char const *argv[], char const *envp[],
									   sys_addr_t data, size_t data_size,
									   int prio, uint64_t flags);

/** \brief terminate a calling game process and create a new game process (2) */
extern void sys_game_process_exitspawn2(const char *path,
										char const *argv[], char const *envp[],
										sys_addr_t data,
										__CSTD size_t data_size, int prio,
										__CSTD uint64_t flags);

/** \brief terminate a process */
extern void sys_process_exit(int status);


extern inline int sys_process_get_number_of_object(uint32_t object,
												   size_t * nump);

extern inline int sys_process_get_number_of_object(uint32_t object,
												   size_t * nump)
{
	system_call_2(SYS_PROCESS_GET_NUMBER_OF_OBJECT, object, (uint32_t) nump);

#ifdef REG_PASS_SYS_PROCESS_GET_NUMBER_OF_OBJECT
	REG_PASS_SYS_PROCESS_GET_NUMBER_OF_OBJECT;
#endif
	return_to_user_prog(int);
}


extern inline int sys_process_get_id(uint32_t object, uint32_t * buff,
									 size_t size, size_t * set_size);

extern inline int sys_process_get_id(uint32_t object, uint32_t * buff,
									 size_t size, size_t * set_size)
{
	system_call_4(SYS_PROCESS_GET_ID, object, (uint32_t) buff, size,
				  (uint32_t) set_size);

#ifdef REG_PASS_SYS_PROCESS_GET_ID
	REG_PASS_SYS_PROCESS_GET_ID;
#endif
	return_to_user_prog(int);
}


extern inline int sys_process_is_spu_lock_line_reservation_address(sys_addr_t
																   addr,
																   uint64_t
																   flags);

extern inline int sys_process_is_spu_lock_line_reservation_address(sys_addr_t
																   addr,
																   uint64_t
																   flags)
{
	system_call_2(SYS_PROCESS_IS_SPU_LOCK_LINE_RESERVATION_ADDRESS, addr,
				  flags);

#ifdef REG_PASS_SYS_PROCESS_IS_SPU_LOCK_LINE_RESERVATION_ADDRESS
	REG_PASS_SYS_PROCESS_IS_SPU_LOCK_LINE_RESERVATION_ADDRESS;
#endif
	return_to_user_prog(int);
}


extern inline sys_addr_t sys_process_get_ppu_guid(void);

extern inline sys_addr_t sys_process_get_ppu_guid(void)
{
	system_call_0(SYS_PROCESS_GET_PPU_GUID);

#ifdef REG_PASS_SYS_PROCESS_GET_PPU_GUID
	REG_PASS_SYS_PROCESS_GET_PPU_GUID;
#endif
	return_to_user_prog(sys_addr_t);
}


CDECL_END

#endif /* __SYS_SYS_PROCESS_H__ */
/*@}*/
