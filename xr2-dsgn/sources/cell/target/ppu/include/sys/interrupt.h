/*   SCE CONFIDENTIAL                                       */
/*   PlayStation(R)3 Programmer Tool Runtime Library 310.001 */
/*   Copyright (C) 2008 Sony Computer Entertainment Inc.    */
/*   All Rights Reserved.                                   */

/*
 * $Date: $
 * $Revision: $
 */

/**
 * \ingroup all_syscall
 * \defgroup intr PPU Interrupts
 * \brief system calls defined in sys/interrupt.h
 *
 * Interfaces defined in sys/interrupt.h
 *
 * Refer to <i>Cell OS Lv-2 User's Manual</i> for the detailed description.
 */
/*@{*/

#ifndef __SYS_SYS_INTERRUPT_H__
#define __SYS_SYS_INTERRUPT_H__

#include <sys/cdefs.h>
#include <sys/syscall.h>

#include <sys/integertypes.h>
#include <sys/return_code.h>
#include <sys/types.h>

CDECL_BEGIN



typedef __CSTD uint64_t sys_irqoutlet_id_t;

/* \brief H/W thread id */
typedef __CSTD uint32_t sys_hw_thread_t;
#define SYS_HW_THREAD_ANY		0xfffffffeU
#define SYS_HW_THREAD_INVALID	0xffffffffU

/* \brief Invalid ID */
#define SYS_INTERRUPT_TAG_ID_INVALID 0xffffffffU


/** \brief interrupt thread handle (opaque value for users) */
typedef __CSTD uint32_t sys_interrupt_thread_handle_t;

/** \brief interrupt handler handle (opaque value for users) */
typedef __CSTD uint32_t sys_interrupt_handler_handle_t;

typedef void (*__ppu_intr_handler_entry) (__CSTD uint64_t, __CSTD uint64_t);

/*
 * intr_thread_t will be appeared in pu_thread.h.
 */

/* create interrupt tag */
extern inline int sys_interrupt_tag_create(sys_interrupt_tag_t * intrtag,
										   sys_irqoutlet_id_t irq,
										   sys_hw_thread_t hwthread);

extern inline int sys_interrupt_tag_create(sys_interrupt_tag_t * intrtag,
										   sys_irqoutlet_id_t irq,
										   sys_hw_thread_t hwthread)
{
	system_call_3(SYS_INTERRUPT_TAG_CREATE, (uint32_t) intrtag, irq, hwthread);

#ifdef REG_PASS_SYS_INTERRUPT_TAG_CREATE
	REG_PASS_SYS_INTERRUPT_TAG_CREATE;
#endif
	return_to_user_prog(int);
}


extern inline int sys_interrupt_tag_destroy(sys_interrupt_tag_t intrtag);

extern inline int sys_interrupt_tag_destroy(sys_interrupt_tag_t intrtag)
{
	system_call_1(SYS_INTERRUPT_TAG_DESTROY, intrtag);

#ifdef REG_PASS_SYS_INTERRUPT_TAG_DESTROY
	REG_PASS_SYS_INTERRUPT_TAG_DESTROY;
#endif
	return_to_user_prog(int);
}



extern inline int _sys_interrupt_thread_establish(sys_interrupt_thread_handle_t
												  * ih,
												  sys_interrupt_tag_t intrtag,
												  __CSTD uint64_t intrthread,
												  __CSTD uint64_t arg1,
												  __CSTD uint64_t arg2);

extern inline int _sys_interrupt_thread_establish(sys_interrupt_thread_handle_t
												  * ih,
												  sys_interrupt_tag_t intrtag,
												  __CSTD uint64_t intrthread,
												  __CSTD uint64_t arg1,
												  __CSTD uint64_t arg2)
{
	system_call_5(_SYS_INTERRUPT_THREAD_ESTABLISH, (uint32_t) ih, intrtag,
				  intrthread, arg1, arg2);

#ifdef REG_PASS__SYS_INTERRUPT_THREAD_ESTABLISH
	REG_PASS__SYS_INTERRUPT_THREAD_ESTABLISH;
#endif
	return_to_user_prog(int);
}


/**
 * \brief establish an interrupt tag on an interrupt thread
 *
 * The sys_interrupt_thread_establish() system call establishes an interrupt thread 
 * \a threadid with the given interrupt tag \a intrtagid and the argument
 * \a arg, and stores the created interrupt service handler to the location 
 * pointed by \a ith_addr. 
 *
 *  
 *
 * \param ih	(OUT) Pointer to the storage for the interrupt thread handle.
 * \param intrtag	  The interrupt tag ID
 * \param intrthread  The interrupt thread iD
 * \param arg	The argument for the interrupt thread
 * \retval CELL_OK	Success
 * \retval ESRCH    Invalid interrupt tag ID
 * \retval EAGAIN	Error due to kernel resource limitation
 * \retval EFAULT	\a ih is not valid addr
 * \retval ESTAT	The assigned interrupt thread has already been established
 *                  on the same interrupt tag.
 */
extern inline int sys_interrupt_thread_establish(sys_interrupt_thread_handle_t *
												 ih,
												 sys_interrupt_tag_t intrtag,
												 sys_ppu_thread_t intrthread,
												 __CSTD uint64_t arg);

extern inline int sys_interrupt_thread_establish(sys_interrupt_thread_handle_t *
												 ih,
												 sys_interrupt_tag_t intrtag,
												 sys_ppu_thread_t intrthread,
												 __CSTD uint64_t arg)
{
	return _sys_interrupt_thread_establish(ih, intrtag, intrthread, arg, 0);
}


extern inline void sys_interrupt_thread_eoi(void);

extern inline void sys_interrupt_thread_eoi(void)
{
	system_call_0(SYS_INTERRUPT_THREAD_EOI);

#ifdef REG_PASS_SYS_INTERRUPT_THREAD_EOI
	REG_PASS_SYS_INTERRUPT_THREAD_EOI;
#endif

}


extern int sys_interrupt_thread_disestablish(sys_interrupt_thread_handle_t ih);

extern inline int
_sys_interrupt_thread_disestablish(sys_interrupt_thread_handle_t ih,
								   uint64_t * tls_mem);

extern inline int
_sys_interrupt_thread_disestablish(sys_interrupt_thread_handle_t ih,
								   uint64_t * tls_mem)
{
	system_call_2(_SYS_INTERRUPT_THREAD_DISESTABLISH, ih, (uint32_t) tls_mem);

#ifdef REG_PASS__SYS_INTERRUPT_THREAD_DISESTABLISH
	REG_PASS__SYS_INTERRUPT_THREAD_DISESTABLISH;
#endif
	return_to_user_prog(int);
}



/*@}*/


/* establish an interrupt thread to an interrupt tag */

CDECL_END

#endif /* __SYS_SYS_INTERRUPT_H__ */
