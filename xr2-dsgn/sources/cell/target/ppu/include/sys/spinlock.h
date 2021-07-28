/*   SCE CONFIDENTIAL                                       */
/*   PlayStation(R)3 Programmer Tool Runtime Library 310.001 */
/*   Copyright (C) 2006 Sony Computer Entertainment Inc.    */
/*   All Rights Reserved.                                   */

/**
 * \ingroup lib_all_library
 * \defgroup lib_sync Synchronization Library Functions
 * \brief Synchronization Library Functions, defined in spinlock.h
 *
 * Interfaces defined in sys/synchronization.h.
 *
 * Refer to Cell OS Lv-2 User's Manual for the detailed description. 
 */

/* This group provide synchronous primitives for synchronization between 
 * multiple PU threads in a process or between multiple processes. 
 *
 * <b><Spinlock></b> \n
 * Spinlock is a synchronous primitive which offers mutual exclusion of a 
 * critical section.  Only one thread can lock the spinlock at a time.
 * When a thread goes to lock while another thread locks the spinlock, it waits
 * until the lock is released by spinning.  Thus, the thread consumes the CPU
 * resources while it waits.  (In order to avoid deadlock, the user may want to
 * be careful with the priority values of PU threads, when synchronizing them
 *  with the spinlock.) 
 *
 */
/*@{*/
#ifndef __SYS_SYS_SPINLOCK_H__
#define __SYS_SYS_SPINLOCK_H__

#include <sys/cdefs.h>

CDECL_BEGIN

/**
 * The sys_spinlock_initialize() function initialize a lock variable.
 *
 * \param lock (OUT)Pointer to the area to initialize a lock variable.
 */
void sys_spinlock_initialize(int *lock);
/**
 * The sys_spinlock_lock() function locks the specified lock variable. 
 * If it cannot acquire the lock, then it busy-waits until the lock is 
 * released. 
 * Use this function with care to avoid deadlocking situation.
 *
 * \param lock Pointer to the lock variable
 */
void sys_spinlock_lock(int *lock);
/**
 * The sys_spinlock_trylock() function tries to lock the specified lock variable.
 * If it cannot acquire the lock, it retunrs EBUSY immediately. 
 *
 * \param lock Pointer to the lock variable
 *
 * \retval SUCCEEDED Successfully acquired the lock 
 * \retval EBUSY Couldn't acquire the lock.
 */
int sys_spinlock_trylock(int *lock);
/**
 * The sys_spinlock_unlock() function unlocks the specified lock variable.
 *
 * \param lock Pointer to the lock variable
 *
 */
void sys_spinlock_unlock(int *lock);

CDECL_END

#endif /* __SYS_SYS_SPINLOCK_H__ */
/*@}*/
