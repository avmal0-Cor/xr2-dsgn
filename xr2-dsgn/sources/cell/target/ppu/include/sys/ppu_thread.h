/*   SCE CONFIDENTIAL                                       */
/*   PlayStation(R)3 Programmer Tool Runtime Library 310.001 */
/*   Copyright (C) 2009 Sony Computer Entertainment Inc.    */
/*   All Rights Reserved.                                   */

/* $Id: ppu_thread.h 39303 2009-04-16 06:13:51Z haya $ */

/**
 * \ingroup all_syscall
 * \defgroup pu_thread PPU Threads
 * \brief system calls defined in sys/ppu_thread.h
 */
/*@{*/

#ifndef	__SYS_SYS_PPU_THREAD_H__
#define	__SYS_SYS_PPU_THREAD_H__

#include <sys/cdefs.h>
#include <sys/syscall.h>

#include <sys/types.h>
#include <sys/integertypes.h>
#include <sys/return_code.h>

CDECL_BEGIN



/* An option for _sys_ppu_thread_create() */
#define	SYS_PPU_THREAD_CREATE_JOINABLE	0x0000000000000001

/* An option for _sys_ppu_thread_create() */
#define	SYS_PPU_THREAD_CREATE_INTERRUPT		0x0000000000000002

/* Invalid PPU thread ID*/
#define SYS_PPU_THREAD_ID_INVALID		0xffffffffu


/* this is an tls variable */
extern __thread sys_ppu_thread_t _sys_ppu_thread_id;


typedef struct {
	sys_addr_t pst_addr;	 /**< stack address */
	__CSTD size_t pst_size;		 /**< stack size    */
} sys_ppu_thread_stack_t;

/**
 * PPU Stack Information structure type
 *
 * \see sys_ppu_thread_get_stack_information
 */

typedef void (*__ppu_thread_entry) (__CSTD uint64_t, __CSTD uint64_t);


typedef struct {
	sys_addr_t entry;
	sys_addr_t tls_mem_addr;
} sys_ppu_thread_init;



/**
 * The type of PPU thread context.  Integer registers only.
 */
typedef struct {
	uint64_t gpr[32];
	uint32_t cr;
	uint32_t rsv1;				/* shim */
	uint64_t xer;
	uint64_t lr;
	uint64_t ctr;
	uint64_t pc;
} sys_ppu_thread_icontext_t;




/** \brief create a PPU thread.
 *
 * The sys_ppu_thread_create() system call creates a PPU thread.  
 * The created PPU thread ID will be stored in \a thread_id.  
 *
 * The created PPU thread will execute codes in \a entry with an 
 * argument \a arg.
 *
 * \a prio specifies the priority value of the PPU thread within the 
 * range from 0 to 3071 where 0 is the highest.
 *
 * The value of \a flags is an OR of zero or the following options.
 *
 * - 0 \n
 *   non-joinable non-interrupt thread 
 * - SYS_PPU_THREAD_CREATE_JOINABLE \n
 *   Create a joinable thread 
 * - SYS_PPU_THREAD_CREATE_INTERRUPT \n
 *   Create an interrupt thread 
 * 
 * The behavior of this function is undefined, if invalid value is specified in
 * \a flags.
 *
 * If a joinable PPU thread is able to join with another PPU thread which 
 * calls sys_ppu_thread_join() as it exits.  Some of the resources of a 
 * joinable PPU thread will be left unallocated until it is joined.
 * If a non-joinable thread exits, on the other hand, its resources are 
 * immediately deallocated. 
 *
 * The SYS_PPU_THREAD_CREATE_INTERRUPT option is used only when an 
 * interrupt PPU thread is created.  
 *
 * Name of the PPU thread can be specified in \a threadname, with upto 27 
 * characters (excluding the null-terminator).
 *
 * \param thread_id      (OUT) A pointer to the storage for the PPU thread 
 *                      ID
 * \param entry         The entry point.  Pointer of entry function.
 * \param arg           An argument
 * \param prio          Priority of the thread in range 0 (highest) from 3071
 * \param stacksize     Stack size in bytes
 * \param flags         PPU thread flags 
 * \param threadname    The name of this thread (used by debugger)
 *
 * \retval CELL_OK      Success
 * \retval ENOMEM       User memory shortage for user stack area
 * \retval EAGAIN       Thread ID shortage or kernel memory shortage
 * \retval EINVAL       Priority out of range
 */
int sys_ppu_thread_create(sys_ppu_thread_t * thread_id,
						  void (*entry) (__CSTD uint64_t),
						  __CSTD uint64_t arg,
						  int prio, __CSTD size_t stacksize,
						  __CSTD uint64_t flags, const char *threadname);


extern void sys_ppu_thread_exit(__CSTD uint64_t val);

extern int sys_ppu_thread_get_id(sys_ppu_thread_t * thread_id);

#define SYS_PPU_THREAD_ONCE_INIT    0
#define SYS_PPU_THREAD_DONE_INIT    1
extern void sys_ppu_thread_once(int *once_ctrl, void (*init) (void));


extern inline void sys_ppu_thread_yield(void);

extern inline void sys_ppu_thread_yield(void)
{
	system_call_0(SYS_PPU_THREAD_YIELD);

#ifdef REG_PASS_SYS_PPU_THREAD_YIELD
	REG_PASS_SYS_PPU_THREAD_YIELD;
#endif

}

extern inline int sys_ppu_thread_join(sys_ppu_thread_t thread_id,
									  __CSTD uint64_t * vptr);

extern inline int sys_ppu_thread_join(sys_ppu_thread_t thread_id,
									  __CSTD uint64_t * vptr)
{
	system_call_2(SYS_PPU_THREAD_JOIN, thread_id, (uint32_t) vptr);

#ifdef REG_PASS_SYS_PPU_THREAD_JOIN
	REG_PASS_SYS_PPU_THREAD_JOIN;
#endif
	return_to_user_prog(int);
}

extern inline int sys_ppu_thread_detach(sys_ppu_thread_t thread_id);

extern inline int sys_ppu_thread_detach(sys_ppu_thread_t thread_id)
{
	system_call_1(SYS_PPU_THREAD_DETACH, thread_id);

#ifdef REG_PASS_SYS_PPU_THREAD_DETACH
	REG_PASS_SYS_PPU_THREAD_DETACH;
#endif
	return_to_user_prog(int);
}

extern inline void sys_ppu_thread_get_join_state(int *isjoinable);

extern inline void sys_ppu_thread_get_join_state(int *isjoinable)
{
	system_call_1(SYS_PPU_THREAD_GET_JOIN_STATE, (uint32_t) isjoinable);

#ifdef REG_PASS_SYS_PPU_THREAD_GET_JOIN_STATE
	REG_PASS_SYS_PPU_THREAD_GET_JOIN_STATE;
#endif

}

extern inline int sys_ppu_thread_set_priority(sys_ppu_thread_t thread_id,
											  int prio);

extern inline int sys_ppu_thread_set_priority(sys_ppu_thread_t thread_id,
											  int prio)
{
	system_call_2(SYS_PPU_THREAD_SET_PRIORITY, thread_id, prio);

#ifdef REG_PASS_SYS_PPU_THREAD_SET_PRIORITY
	REG_PASS_SYS_PPU_THREAD_SET_PRIORITY;
#endif
	return_to_user_prog(int);
}

extern inline int sys_ppu_thread_get_priority(sys_ppu_thread_t thread_id,
											  int *priop);

extern inline int sys_ppu_thread_get_priority(sys_ppu_thread_t thread_id,
											  int *priop)
{
	system_call_2(SYS_PPU_THREAD_GET_PRIORITY, thread_id, (uint32_t) priop);

#ifdef REG_PASS_SYS_PPU_THREAD_GET_PRIORITY
	REG_PASS_SYS_PPU_THREAD_GET_PRIORITY;
#endif
	return_to_user_prog(int);
}

extern inline int sys_ppu_thread_get_stack_information(sys_ppu_thread_stack_t *
													   sp);

extern inline int sys_ppu_thread_get_stack_information(sys_ppu_thread_stack_t *
													   sp)
{
	system_call_1(SYS_PPU_THREAD_GET_STACK_INFORMATION, (uint32_t) sp);

#ifdef REG_PASS_SYS_PPU_THREAD_GET_STACK_INFORMATION
	REG_PASS_SYS_PPU_THREAD_GET_STACK_INFORMATION;
#endif
	return_to_user_prog(int);
}


extern inline int sys_ppu_thread_recover_page_fault(sys_ppu_thread_t thread_id);

extern inline int sys_ppu_thread_recover_page_fault(sys_ppu_thread_t thread_id)
{
	system_call_1(SYS_PPU_THREAD_RECOVER_PAGE_FAULT, thread_id);

#ifdef REG_PASS_SYS_PPU_THREAD_RECOVER_PAGE_FAULT
	REG_PASS_SYS_PPU_THREAD_RECOVER_PAGE_FAULT;
#endif
	return_to_user_prog(int);
}

extern inline int sys_ppu_thread_get_page_fault_context(sys_ppu_thread_t
														thread_id,
														sys_ppu_thread_icontext_t
														* ctxp);

extern inline int sys_ppu_thread_get_page_fault_context(sys_ppu_thread_t
														thread_id,
														sys_ppu_thread_icontext_t
														* ctxp)
{
	system_call_2(SYS_PPU_THREAD_GET_PAGE_FAULT_CONTEXT, thread_id,
				  (uint32_t) ctxp);

#ifdef REG_PASS_SYS_PPU_THREAD_GET_PAGE_FAULT_CONTEXT
	REG_PASS_SYS_PPU_THREAD_GET_PAGE_FAULT_CONTEXT;
#endif
	return_to_user_prog(int);
}




extern inline int sys_ppu_thread_rename(sys_ppu_thread_t thread_id,
										const char *name);

extern inline int sys_ppu_thread_rename(sys_ppu_thread_t thread_id,
										const char *name)
{
	system_call_2(SYS_PPU_THREAD_RENAME, thread_id, (uint32_t) name);

#ifdef REG_PASS_SYS_PPU_THREAD_RENAME
	REG_PASS_SYS_PPU_THREAD_RENAME;
#endif
	return_to_user_prog(int);
}


CDECL_END

#endif /* __SYS_SYS_PPU_THREAD_H__ */
/*@}*/
