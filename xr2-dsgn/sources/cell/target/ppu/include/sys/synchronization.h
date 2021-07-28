/*   SCE CONFIDENTIAL                                       */
/*   PlayStation(R)3 Programmer Tool Runtime Library 310.001 */
/*   Copyright (C) 2009 Sony Computer Entertainment Inc.    */
/*   All Rights Reserved.                                   */

/**
 * \ingroup all_syscall
 * \defgroup synch Synchronous Primitives
 * \brief 
 */
/*@{*/

#ifndef __SYS_SYS_SYNCHRONIZATION_H__
#define __SYS_SYS_SYNCHRONIZATION_H__

#include <sys/cdefs.h>
#include <sys/syscall.h>

#include <sys/integertypes.h>
#include <sys/return_code.h>
#include <sys/types.h>


CDECL_BEGIN


/*
 * Values for attr_protocol
 */
#define   SYS_SYNC_FIFO                     0x00001
#define   SYS_SYNC_PRIORITY                 0x00002
#define   SYS_SYNC_PRIORITY_INHERIT         0x00003
#define   SYS_SYNC_RETRY                    0x00004

/*
 * Values for attr_recursive.
 */
#define   SYS_SYNC_RECURSIVE                0x00010
#define   SYS_SYNC_NOT_RECURSIVE            0x00020

/*
 * Values for attr_pshared
 */
#define   SYS_SYNC_NOT_PROCESS_SHARED       0x00200

/*
 * Values for attr_adaptive
 */
#define   SYS_SYNC_ADAPTIVE                 0x01000
#define   SYS_SYNC_NOT_ADAPTIVE             0x02000

/*
 * Values for flags
 */
#define SYS_SYNC_NEWLY_CREATED              0x00001
#define SYS_SYNC_NOT_CREATE                 0x00002
#define SYS_SYNC_NOT_CARE                   0x00003


/*
 * Values for type
 */
#define SYS_SYNC_WAITER_SINGLE              0x10000
#define SYS_SYNC_WAITER_MULTIPLE            0x20000


/*
 * Values for timeout
 */
#define SYS_NO_TIMEOUT                         0x00

/*
 * Values for event flag wait mode and options
 */

#define SYS_EVENT_FLAG_WAIT_AND            0x000001
#define SYS_EVENT_FLAG_WAIT_OR             0x000002
#define SYS_EVENT_FLAG_WAIT_CLEAR          0x000010
#define SYS_EVENT_FLAG_WAIT_CLEAR_ALL      0x000020

/*
 * Invalid values for ID
 */
#define SYS_MUTEX_ID_INVALID         0xFFFFFFFFU
#define SYS_COND_ID_INVALID	         0xFFFFFFFFU
#define SYS_RWLOCK_ID_INVALID        0xFFFFFFFFU
#define SYS_SEMAPHORE_ID_INVALID     0xFFFFFFFFU
#define SYS_EVENT_FLAG_ID_INVALID    0xFFFFFFFFU


#define SYS_SYNC_NAME_LENGTH        7
#define SYS_SYNC_NAME_SIZE          (SYS_SYNC_NAME_LENGTH + 1)



/**
 * \brief Mutex attribute structure
 *
 * The \a sys_mutex_attribute_t structure type is used by sys_mutex_create() 
 * to specify the mutex attributes.  
 * The meaning of each member variable is as follows.
 * 
 * - \a attr_protocol \n
 *   \a attr_protocol specifies the scheduling policy for waiting PPU threads.
 *   If multiple PPU threads are waiting on the mutex, the kernel chooses a 
 *   PPU thread which acquires a lock of the mutex next based on the scheduling
 *   policy specified by this variable.  This variable must take one of the 
 *   following values.
 *     - SYS_SYNC_FIFO \n
 *       Waiting PPU threads acquire locks of the mutex in FIFO order 
 *       (first-in first-out).
 *     - SYS_SYNC_PRIORITY \n 
 *       Waiting PPU threads acquire locks of the mutex in priority order.
 *       If there are multiple PPU threads which have the highest priority,
 *       they are ordered with FIFO.
 *     - SYS_SYNC_PRIORITY_INHERIT \n
 *       <b>(Not implemented)</b>  
 *       The mutex behaves as BPIP(Basic Priority 
 *       Inheritance Protocol). When a thread tries to acquire the lock, if the
 *       owner thread exists and has lower priority than that thread, then the 
 *       owner temporarily inherits that thread's priority and continues the 
 *       execution. Priority inheritance continues until the lock is released.
 *
 * - \a attr_recursive \n
 *   \a attr_recursive specifies whether recursive locks on the mutex are 
 *   allowed or not.  This variable must take one of the following values.
 *     - SYS_SYNC_RECURSIVE \n
 *       Recursive locks are allowed.
 *     - SYS_SYNC_NOT_RECURSIVE \n
 *       Recursive locks are not allowed.
 *
 * - \a attr_pshared \n
 *   \a attr_pshared specifies whether this mutex is shareable among processes
 *   or not.  This variable must take one of the following values.
 *     - SYS_SYNC_PROCESS_SHARED \n
 *       The mutex is process-shareable.
 *     - SYS_SYNC_NOT_PROCESS_SHARED \n
 *       The mutex is not process-shareable.
 *
 * - \a attr_adaptive \n 
 *    <b>(Not implemented)</b>
 *    The sys_adaptive_t can specify whether the thread wait-spins or 
 *    not, when the thread can not acquire a lock.  
 *    - SYS_SYNC_ADAPTIVE \n
 *      When a thread can not acquire a lock, the thread wait spinning until
 *      the lock becomes released and available for the thread.
 *    - SYS_SYNC_NOT_ADAPTIVE \n
 *      When a thread can not acquire the lock, wait on the synchronization 
 *      primitive. 
 *   \a attr_adaptive is not implemented, and currently has no effect.
 *
 * - \a key \n
 *   \a key is used only when \a attr_pshared is SYS_SYNC_PROCESS_SHARED.  
 *   Its valid range is from 1 to (2^63 - 1).  Specifying an out-of-range value
 *   causes undefined behavior of the synchronous primitive. 
 *   A process-shared mutex has a key value which is unique in the LPAR, and 
 *   sys_mutex_create() uses this value when it obtains an reference of a 
 *   mutex which has already been created in another process.
 *   
 * - \a flags \n
 *   \a flags is used only when \a attr_pshared is SYS_SYNC_PROCESS_SHARED,
 *   and its value affects the behavior of sys_mutex_create() when it creates
 *   or obtains a reference of a process-shared mutex.
 *   This variable must take one of the following values.
 *   - SYS_SYNC_NEWLY_CREATED \n
 *     sys_mutex_create() creates a new process-shared mutex with the given 
 *     key.  If there is already a process-shared mutex with the key, 
 *     sys_mutex_create() fails.
 *   - SYS_SYNC_NOT_CREATE \n
 *     sys_mutex_create() tries to obtain a reference of a process-shared 
 *     mutex which has already been created in another process.  If there is 
 *     no mutex with the given key in the LPAR, sys_mutex_create() fails.
 *     Values in \a attr_protocol, \a attr_recursive and \a attr_adaptive are 
 *     ignored, and attributes of already existing synchronous primitive is 
 *     always prioritized.
 *   - SYS_SYNC_NOT_CARE \n
 *     If there is a process-shared mutex with the given key, obtain a 
 *     reference to it.  Otherwise, create a new process-shared mutex with the 
 *     given key.
 *
 * - \a name \n
 *    \a name of the mutex can be specified in \a name, with up to 7. 
 *    characters (excluding the null-terminator).
 *
 *
 * \see sys_mutex_create, sys_mutex_attribute_initialize()
 */
typedef struct mutex_attr {
	sys_protocol_t attr_protocol;	/**< Policy for waiting threads */
	sys_recursive_t attr_recursive;	/**< Whether recursive locks are effective */
	sys_process_shared_t attr_pshared; /**< Whether the mutex is shared among 
                                          multiple processes */
	sys_adaptive_t attr_adaptive;	/**< Adaptiveness */
	sys_ipc_key_t key;				/**< process-shared key */
	int flags;						/**< flags */
	__CSTD uint32_t pad;
	char name[SYS_SYNC_NAME_SIZE];	 /**< mutex name for debugging */
} sys_mutex_attribute_t;


/**
 * \brief Condition variable attribute structure
 *
 * The \a sys_cond_attribute_t structure type is used by sys_cond_create() to 
 * specify the condition variable attributes.  
 * The meanings of member variables are similar to those of 
 * sys_mutex_attribute_t.  
 * Refer to the specification of sys_mutex_attribute_t.
 *
 * \see sys_cond_create, sys_cond_attribute_initialize(),
 *      sys_mutex_attribute_t
 */
typedef struct cond_attr {
	sys_process_shared_t attr_pshared; /**< Whether the condition variable is
                                        * shared  among multiple processes */
	int flags;						   /**< flags */
	sys_ipc_key_t key;				   /**< process-shared key */
	char name[SYS_SYNC_NAME_SIZE];	   /**< cond name for debugging */
} sys_cond_attribute_t;


/**
 * \brief Reader/writer lock attribute structure
 *
 * The \a sys_rwlock_attribute_t structure type is used by 
 * sys_cond_create() to specify the reader/writer lock attributes.  
 * The meanings of member variables are similar to those of 
 * sys_mutex_attribute_t.
 * Refer to the specification of sys_mutex_attribute_t.
 *
 * \see sys_rwlock_create, sys_rwlock_attribute_initialize()
 *      sys_mutex_attribute_t
 */
typedef struct rwlock_attr {
	sys_protocol_t attr_protocol;		/**< Policy for waiting threads */
	sys_process_shared_t attr_pshared;	/**< Whether the reader/writer lock is 
                                             shared among multiple processes */
	sys_ipc_key_t key;					/**< process-shared key */
	int flags;							/**< flags */
	__CSTD uint32_t pad;
	char name[SYS_SYNC_NAME_SIZE]; /**< rwlock name for debugging */
} sys_rwlock_attribute_t;



/**
 * \brief Semaphore attribute structure
 *
 * The \a sys_semaphore_attribute_t structure type is used by 
 * sys_semaphore_create() to specify the semaphore attributes.  
 * The meanings of member variables are similar to those of 
 * sys_mutex_attribute_t.
 * Refer to the specification of sys_mutex_attribute_t.
 *
 * \see sys_semaphore_create, sys_semaphore_attribute_initialize()
 *      sys_semaphore_attribute_t
 */
typedef struct {
	sys_protocol_t attr_protocol;
	sys_process_shared_t attr_pshared;
	sys_ipc_key_t key;
	int flags;
	__CSTD uint32_t pad;
	char name[SYS_SYNC_NAME_SIZE]; /**< semaphore name for debugging */
} sys_semaphore_attribute_t;

typedef __CSTD int32_t sys_semaphore_value_t;



/**
 * \brief Event flag attribute structure
 *
 * The \a sys_event_flag_attribute_t structure type is used by 
 * sys_event_flag_create() to specify the event_flag attributes.  
 * The meanings of member variables are similar to those of 
 * sys_mutex_attribute_t.
 * Refer to the specification of sys_mutex_attribute_t.
 *
 * \see sys_event_flag_create, sys_event_flag_attribute_initialize()
 *      sys_event_flag_attribute_t
 */
typedef struct {
	sys_protocol_t attr_protocol;
	sys_process_shared_t attr_pshared;
	sys_ipc_key_t key;
	int flags;
	int type;
	char name[SYS_SYNC_NAME_SIZE]; /**< event_flag name for debugging */
} sys_event_flag_attribute_t;



typedef struct lwmutex_attr {
	sys_protocol_t attr_protocol;	/**< Policy for waiting threads */
	sys_recursive_t attr_recursive;	/**< Whether recursive locks are effective */
	char name[SYS_SYNC_NAME_SIZE]; /**< lwmutex name for debugging */
} sys_lwmutex_attribute_t;

typedef struct {
	volatile __CSTD uint32_t owner;
	volatile __CSTD uint32_t waiter;
} sys_lwmutex_lock_info_t;

typedef union {
	sys_lwmutex_lock_info_t info;
	volatile __CSTD uint64_t all_info;
} sys_lwmutex_variable_t;

typedef struct sys_lwmutex {
	sys_lwmutex_variable_t lock_var;
	__CSTD uint32_t attribute;
	__CSTD uint32_t recursive_count;
	_sys_sleep_queue_t sleep_queue;
	__CSTD uint32_t pad;
} sys_lwmutex_t;


typedef struct sys_lwcond {
	sys_lwmutex_t *lwmutex;
	_sys_lwcond_queue_t lwcond_queue;
} sys_lwcond_t;

typedef struct sys_lwcond_attribute {
	char name[SYS_SYNC_NAME_SIZE]; /**< lwmutex name for debugging */
} sys_lwcond_attribute_t;


#ifdef __SNC__
#define  _SYS_SYNC_ATTRIBUTE_NAME_SET(x,y)                              \
    do{                                                                 \
           uint64_t *d = (uint64_t*)(x);                                \
           uint64_t *s = (uint64_t*)(y);                                \
           *(d) = *(s);                                                 \
           uint8_t *db = (uint8_t*)(x);                                 \
           *(db + SYS_SYNC_NAME_LENGTH) = '\0';                         \
     }while(0)

#else /* ! __SNC__ */

#define _SYS_SYNC_ATTRIBUTE_NAME_SET(x,y)                               \
    do{                                                                 \
        uint64_t tmp1, tmp2;                                            \
        __asm volatile ("ld   %0, 0(%2);"                               \
                        "std  %0, 0(%3);"                               \
                        "li   %0, 0;"                                   \
                        "addi %1, %3, 7;"                               \
                        "stb  %0, 0(%1)"                                \
                        :[tmp1] "=&r" (tmp1), [tmp2] "=&b" (tmp2)       \
                        :[y] "b" (y), [x] "b" (x)                       \
                        :"memory");                                     \
     }while(0)
#endif

/**
 * The sys_mutex_attribute_initialize() macro sets default values to the specified mutex 
 * attribute structure.
 * The default values are  SYS_SYNC_PRIORITY, SYS_SYNC_NOT_RECURSIVE,
 * SYS_SYNC_NOT_PROCESS_SHARED, SYS_SYNC_NOT_ADAPTIVE.
 */
#define sys_mutex_attribute_initialize(x)               \
    do{                                                 \
        x.attr_protocol = SYS_SYNC_PRIORITY;            \
        x.attr_recursive = SYS_SYNC_NOT_RECURSIVE;      \
        x.attr_pshared = SYS_SYNC_NOT_PROCESS_SHARED;   \
        x.attr_adaptive = SYS_SYNC_NOT_ADAPTIVE;        \
        x.key = 0;                                      \
        x.flags = 0;                                    \
        x.name[0] = '\0';                               \
    }while(0)

/**
 * The sys_mutex_attribute_name_set() function sets a specified name to the 
 * specified name area of an attribute structure.
 */
extern inline
	void sys_mutex_attribute_name_set(char attr_name[], const char *name);

extern inline
	void sys_mutex_attribute_name_set(char attr_name[], const char *name)
{
	_SYS_SYNC_ATTRIBUTE_NAME_SET(attr_name, name);
	return;
}

extern inline int sys_mutex_create(sys_mutex_t * mutex_id,
								   sys_mutex_attribute_t * attr);

extern inline int sys_mutex_create(sys_mutex_t * mutex_id,
								   sys_mutex_attribute_t * attr)
{
	system_call_2(SYS_MUTEX_CREATE, (uint32_t) mutex_id, (uint32_t) attr);

#ifdef REG_PASS_SYS_MUTEX_CREATE
	REG_PASS_SYS_MUTEX_CREATE;
#endif
	return_to_user_prog(int);
}

extern inline int sys_mutex_destroy(sys_mutex_t mutex_id);

extern inline int sys_mutex_destroy(sys_mutex_t mutex_id)
{
	system_call_1(SYS_MUTEX_DESTROY, mutex_id);

#ifdef REG_PASS_SYS_MUTEX_DESTROY
	REG_PASS_SYS_MUTEX_DESTROY;
#endif
	return_to_user_prog(int);
}

extern inline int sys_mutex_lock(sys_mutex_t mutex_id, usecond_t timeout);

extern inline int sys_mutex_lock(sys_mutex_t mutex_id, usecond_t timeout)
{
	system_call_2(SYS_MUTEX_LOCK, mutex_id, timeout);

#ifdef REG_PASS_SYS_MUTEX_LOCK
	REG_PASS_SYS_MUTEX_LOCK;
#endif
	return_to_user_prog(int);
}

extern inline int sys_mutex_trylock(sys_mutex_t mutex_id);

extern inline int sys_mutex_trylock(sys_mutex_t mutex_id)
{
	system_call_1(SYS_MUTEX_TRYLOCK, mutex_id);

#ifdef REG_PASS_SYS_MUTEX_TRYLOCK
	REG_PASS_SYS_MUTEX_TRYLOCK;
#endif
	return_to_user_prog(int);
}

extern inline int sys_mutex_unlock(sys_mutex_t mutex_id);

extern inline int sys_mutex_unlock(sys_mutex_t mutex_id)
{
	system_call_1(SYS_MUTEX_UNLOCK, mutex_id);

#ifdef REG_PASS_SYS_MUTEX_UNLOCK
	REG_PASS_SYS_MUTEX_UNLOCK;
#endif
	return_to_user_prog(int);
}





/**
 * The sys_cond_attribute_initialize() macro sets default values to the 
 * specified condition variable attribute structure.
 * The default values are SYS_SYNC_NOT_PROCESS_SHARED.
 */
#define sys_cond_attribute_initialize(x)                \
    do {                                                \
        x.attr_pshared = SYS_SYNC_NOT_PROCESS_SHARED;   \
        x.key = 0;                                      \
        x.flags = 0;                                    \
        x.name[0] = '\0';                               \
    } while(0)

/**
 * The sys_cond_attribute_name_set() function sets a specified name to the 
 * specified name area of an attribute structure.
 */
extern inline
	void sys_cond_attribute_name_set(char attr_name[], const char *name);

extern inline
	void sys_cond_attribute_name_set(char attr_name[], const char *name)
{
	_SYS_SYNC_ATTRIBUTE_NAME_SET(attr_name, name);
	return;
}


extern inline int sys_cond_create(sys_cond_t * cond_id, sys_mutex_t mutex_id,
								  sys_cond_attribute_t * attr);

extern inline int sys_cond_create(sys_cond_t * cond_id, sys_mutex_t mutex_id,
								  sys_cond_attribute_t * attr)
{
	system_call_3(SYS_COND_CREATE, (uint32_t) cond_id, mutex_id,
				  (uint32_t) attr);

#ifdef REG_PASS_SYS_COND_CREATE
	REG_PASS_SYS_COND_CREATE;
#endif
	return_to_user_prog(int);
}

extern inline int sys_cond_destroy(sys_cond_t cond_id);

extern inline int sys_cond_destroy(sys_cond_t cond_id)
{
	system_call_1(SYS_COND_DESTROY, cond_id);

#ifdef REG_PASS_SYS_COND_DESTROY
	REG_PASS_SYS_COND_DESTROY;
#endif
	return_to_user_prog(int);
}

extern inline int sys_cond_wait(sys_cond_t cond_id, usecond_t timeout);

extern inline int sys_cond_wait(sys_cond_t cond_id, usecond_t timeout)
{
	system_call_2(SYS_COND_WAIT, cond_id, timeout);

#ifdef REG_PASS_SYS_COND_WAIT
	REG_PASS_SYS_COND_WAIT;
#endif
	return_to_user_prog(int);
}

extern inline int sys_cond_signal(sys_cond_t cond_id);

extern inline int sys_cond_signal(sys_cond_t cond_id)
{
	system_call_1(SYS_COND_SIGNAL, cond_id);

#ifdef REG_PASS_SYS_COND_SIGNAL
	REG_PASS_SYS_COND_SIGNAL;
#endif
	return_to_user_prog(int);
}

extern inline int sys_cond_signal_to(sys_cond_t cond_id,
									 sys_ppu_thread_t thread);

extern inline int sys_cond_signal_to(sys_cond_t cond_id,
									 sys_ppu_thread_t thread)
{
	system_call_2(SYS_COND_SIGNAL_TO, cond_id, thread);

#ifdef REG_PASS_SYS_COND_SIGNAL_TO
	REG_PASS_SYS_COND_SIGNAL_TO;
#endif
	return_to_user_prog(int);
}

extern inline int sys_cond_signal_all(sys_cond_t cond_id);

extern inline int sys_cond_signal_all(sys_cond_t cond_id)
{
	system_call_1(SYS_COND_SIGNAL_ALL, cond_id);

#ifdef REG_PASS_SYS_COND_SIGNAL_ALL
	REG_PASS_SYS_COND_SIGNAL_ALL;
#endif
	return_to_user_prog(int);
}





/**
 * sys_rwlock_attribute_initialize macro sets default values to the specified 
 * reader/writer lock attribute structure.
 * The default values are  SYS_SYNC_PRIORITY, SYS_SYNC_NOT_PROCESS_SHARED.
 */
#define sys_rwlock_attribute_initialize(x)              \
    do{                                                 \
        x.attr_protocol = SYS_SYNC_PRIORITY;            \
        x.attr_pshared = SYS_SYNC_NOT_PROCESS_SHARED;   \
        x.key = 0;                                      \
        x.flags = 0;                                    \
        x.name[0] = '\0';                               \
    }while(0)

/**
 * The sys_rwlock_attribute_name_set() function sets a specified name to the 
 * specified name area of an attribute structure.
 */
extern inline
	void sys_rwlock_attribute_name_set(char attr_name[], const char *name);

extern inline
	void sys_rwlock_attribute_name_set(char attr_name[], const char *name)
{
	_SYS_SYNC_ATTRIBUTE_NAME_SET(attr_name, name);
	return;
}

extern inline int sys_rwlock_create(sys_rwlock_t * rw_lock_id,
									sys_rwlock_attribute_t * attr);

extern inline int sys_rwlock_create(sys_rwlock_t * rw_lock_id,
									sys_rwlock_attribute_t * attr)
{
	system_call_2(SYS_RWLOCK_CREATE, (uint32_t) rw_lock_id, (uint32_t) attr);

#ifdef REG_PASS_SYS_RWLOCK_CREATE
	REG_PASS_SYS_RWLOCK_CREATE;
#endif
	return_to_user_prog(int);
}

extern inline int sys_rwlock_destroy(sys_rwlock_t rw_lock_id);

extern inline int sys_rwlock_destroy(sys_rwlock_t rw_lock_id)
{
	system_call_1(SYS_RWLOCK_DESTROY, rw_lock_id);

#ifdef REG_PASS_SYS_RWLOCK_DESTROY
	REG_PASS_SYS_RWLOCK_DESTROY;
#endif
	return_to_user_prog(int);
}

extern inline int sys_rwlock_rlock(sys_rwlock_t rw_lock_id, usecond_t timeout);

extern inline int sys_rwlock_rlock(sys_rwlock_t rw_lock_id, usecond_t timeout)
{
	system_call_2(SYS_RWLOCK_RLOCK, rw_lock_id, timeout);

#ifdef REG_PASS_SYS_RWLOCK_RLOCK
	REG_PASS_SYS_RWLOCK_RLOCK;
#endif
	return_to_user_prog(int);
}

extern inline int sys_rwlock_tryrlock(sys_rwlock_t rw_lock_id);

extern inline int sys_rwlock_tryrlock(sys_rwlock_t rw_lock_id)
{
	system_call_1(SYS_RWLOCK_TRYRLOCK, rw_lock_id);

#ifdef REG_PASS_SYS_RWLOCK_TRYRLOCK
	REG_PASS_SYS_RWLOCK_TRYRLOCK;
#endif
	return_to_user_prog(int);
}

extern inline int sys_rwlock_runlock(sys_rwlock_t rw_lock_id);

extern inline int sys_rwlock_runlock(sys_rwlock_t rw_lock_id)
{
	system_call_1(SYS_RWLOCK_RUNLOCK, rw_lock_id);

#ifdef REG_PASS_SYS_RWLOCK_RUNLOCK
	REG_PASS_SYS_RWLOCK_RUNLOCK;
#endif
	return_to_user_prog(int);
}

extern inline int sys_rwlock_wlock(sys_rwlock_t rw_lock_id, usecond_t timeout);

extern inline int sys_rwlock_wlock(sys_rwlock_t rw_lock_id, usecond_t timeout)
{
	system_call_2(SYS_RWLOCK_WLOCK, rw_lock_id, timeout);

#ifdef REG_PASS_SYS_RWLOCK_WLOCK
	REG_PASS_SYS_RWLOCK_WLOCK;
#endif
	return_to_user_prog(int);
}

extern inline int sys_rwlock_trywlock(sys_rwlock_t rw_lock_id);

extern inline int sys_rwlock_trywlock(sys_rwlock_t rw_lock_id)
{
	system_call_1(SYS_RWLOCK_TRYWLOCK, rw_lock_id);

#ifdef REG_PASS_SYS_RWLOCK_TRYWLOCK
	REG_PASS_SYS_RWLOCK_TRYWLOCK;
#endif
	return_to_user_prog(int);
}

extern inline int sys_rwlock_wunlock(sys_rwlock_t rw_lock_id);

extern inline int sys_rwlock_wunlock(sys_rwlock_t rw_lock_id)
{
	system_call_1(SYS_RWLOCK_WUNLOCK, rw_lock_id);

#ifdef REG_PASS_SYS_RWLOCK_WUNLOCK
	REG_PASS_SYS_RWLOCK_WUNLOCK;
#endif
	return_to_user_prog(int);
}





/**
 * sys_semaphore_attribute_initialize macro sets default values to 
 * the specified semaphore attribute structure.
 * The default values are  SYS_SYNC_PRIORITY, SYS_SYNC_NOT_PROCESS_SHARED.
 */
#define sys_semaphore_attribute_initialize(x)              \
    do{                                                    \
        x.attr_protocol = SYS_SYNC_PRIORITY;               \
        x.attr_pshared = SYS_SYNC_NOT_PROCESS_SHARED;      \
        x.key = 0;                                         \
        x.flags = 0;                                       \
        x.name[0] = '\0';                                  \
    }while(0)

/**
 * The sys_semaphore_attribute_name_set() function sets a specified name to the 
 * specified name area of an attribute structure.
 */
extern inline
	void sys_semaphore_attribute_name_set(char attr_name[], const char *name);

extern inline
	void sys_semaphore_attribute_name_set(char attr_name[], const char *name)
{
	_SYS_SYNC_ATTRIBUTE_NAME_SET(attr_name, name);
	return;
}

extern inline int sys_semaphore_create(sys_semaphore_t * sem,
									   sys_semaphore_attribute_t * attr,
									   sys_semaphore_value_t initial_val,
									   sys_semaphore_value_t max_val);

extern inline int sys_semaphore_create(sys_semaphore_t * sem,
									   sys_semaphore_attribute_t * attr,
									   sys_semaphore_value_t initial_val,
									   sys_semaphore_value_t max_val)
{
	system_call_4(SYS_SEMAPHORE_CREATE, (uint32_t) sem, (uint32_t) attr,
				  initial_val, max_val);

#ifdef REG_PASS_SYS_SEMAPHORE_CREATE
	REG_PASS_SYS_SEMAPHORE_CREATE;
#endif
	return_to_user_prog(int);
}

extern inline int sys_semaphore_destroy(sys_semaphore_t sem);

extern inline int sys_semaphore_destroy(sys_semaphore_t sem)
{
	system_call_1(SYS_SEMAPHORE_DESTROY, sem);

#ifdef REG_PASS_SYS_SEMAPHORE_DESTROY
	REG_PASS_SYS_SEMAPHORE_DESTROY;
#endif
	return_to_user_prog(int);
}

extern inline int sys_semaphore_wait(sys_semaphore_t sem, usecond_t timeout);

extern inline int sys_semaphore_wait(sys_semaphore_t sem, usecond_t timeout)
{
	system_call_2(SYS_SEMAPHORE_WAIT, sem, timeout);

#ifdef REG_PASS_SYS_SEMAPHORE_WAIT
	REG_PASS_SYS_SEMAPHORE_WAIT;
#endif
	return_to_user_prog(int);
}

extern inline int sys_semaphore_trywait(sys_semaphore_t sem);

extern inline int sys_semaphore_trywait(sys_semaphore_t sem)
{
	system_call_1(SYS_SEMAPHORE_TRYWAIT, sem);

#ifdef REG_PASS_SYS_SEMAPHORE_TRYWAIT
	REG_PASS_SYS_SEMAPHORE_TRYWAIT;
#endif
	return_to_user_prog(int);
}

extern inline int sys_semaphore_post(sys_semaphore_t sem,
									 sys_semaphore_value_t val);

extern inline int sys_semaphore_post(sys_semaphore_t sem,
									 sys_semaphore_value_t val)
{
	system_call_2(SYS_SEMAPHORE_POST, sem, val);

#ifdef REG_PASS_SYS_SEMAPHORE_POST
	REG_PASS_SYS_SEMAPHORE_POST;
#endif
	return_to_user_prog(int);
}

extern inline int sys_semaphore_get_value(sys_semaphore_t sem,
										  sys_semaphore_value_t * val);

extern inline int sys_semaphore_get_value(sys_semaphore_t sem,
										  sys_semaphore_value_t * val)
{
	system_call_2(SYS_SEMAPHORE_GET_VALUE, sem, (uint32_t) val);

#ifdef REG_PASS_SYS_SEMAPHORE_GET_VALUE
	REG_PASS_SYS_SEMAPHORE_GET_VALUE;
#endif
	return_to_user_prog(int);
}





/**
 * sys_event_flag_attribute_initialize macro sets default values to 
 * the specified event_flag attribute structure.
 * The default values are  SYS_SYNC_PRIORITY, SYS_SYNC_NOT_PROCESS_SHARED.
 */
#define sys_event_flag_attribute_initialize(x)             \
    do{                                                    \
        x.attr_protocol = SYS_SYNC_PRIORITY;               \
        x.attr_pshared = SYS_SYNC_NOT_PROCESS_SHARED;      \
        x.key = 0;                                         \
        x.flags = 0;                                       \
        x.type = SYS_SYNC_WAITER_MULTIPLE;                 \
        x.name[0] = '\0';                                  \
    }while(0)

/**
 * The sys_event_flag_attribute_name_set() function sets a specified name to the 
 * specified name area of an attribute structure.
 */
extern inline
	void sys_event_flag_attribute_name_set(char attr_name[], const char *name);

extern inline
	void sys_event_flag_attribute_name_set(char attr_name[], const char *name)
{
	_SYS_SYNC_ATTRIBUTE_NAME_SET(attr_name, name);
	return;
}

extern inline int sys_event_flag_create(sys_event_flag_t * id,
										sys_event_flag_attribute_t * attr,
										uint64_t init);

extern inline int sys_event_flag_create(sys_event_flag_t * id,
										sys_event_flag_attribute_t * attr,
										uint64_t init)
{
	system_call_3(SYS_EVENT_FLAG_CREATE, (uint32_t) id, (uint32_t) attr, init);

#ifdef REG_PASS_SYS_EVENT_FLAG_CREATE
	REG_PASS_SYS_EVENT_FLAG_CREATE;
#endif
	return_to_user_prog(int);
}

extern inline int sys_event_flag_destroy(sys_event_flag_t id);

extern inline int sys_event_flag_destroy(sys_event_flag_t id)
{
	system_call_1(SYS_EVENT_FLAG_DESTROY, id);

#ifdef REG_PASS_SYS_EVENT_FLAG_DESTROY
	REG_PASS_SYS_EVENT_FLAG_DESTROY;
#endif
	return_to_user_prog(int);
}

extern inline int sys_event_flag_wait(sys_event_flag_t id, uint64_t bitptn,
									  uint32_t mode, uint64_t * result,
									  usecond_t timeout);

extern inline int sys_event_flag_wait(sys_event_flag_t id, uint64_t bitptn,
									  uint32_t mode, uint64_t * result,
									  usecond_t timeout)
{
	system_call_5(SYS_EVENT_FLAG_WAIT, id, bitptn, mode, (uint32_t) result,
				  timeout);

#ifdef REG_PASS_SYS_EVENT_FLAG_WAIT
	REG_PASS_SYS_EVENT_FLAG_WAIT;
#endif
	return_to_user_prog(int);
}

extern inline int sys_event_flag_trywait(sys_event_flag_t id, uint64_t bitptn,
										 uint32_t mode, uint64_t * result);

extern inline int sys_event_flag_trywait(sys_event_flag_t id, uint64_t bitptn,
										 uint32_t mode, uint64_t * result)
{
	system_call_4(SYS_EVENT_FLAG_TRYWAIT, id, bitptn, mode, (uint32_t) result);

#ifdef REG_PASS_SYS_EVENT_FLAG_TRYWAIT
	REG_PASS_SYS_EVENT_FLAG_TRYWAIT;
#endif
	return_to_user_prog(int);
}

extern inline int sys_event_flag_set(sys_event_flag_t id, uint64_t bitptn);

extern inline int sys_event_flag_set(sys_event_flag_t id, uint64_t bitptn)
{
	system_call_2(SYS_EVENT_FLAG_SET, id, bitptn);

#ifdef REG_PASS_SYS_EVENT_FLAG_SET
	REG_PASS_SYS_EVENT_FLAG_SET;
#endif
	return_to_user_prog(int);
}

extern inline int sys_event_flag_clear(sys_event_flag_t id, uint64_t bitptn);

extern inline int sys_event_flag_clear(sys_event_flag_t id, uint64_t bitptn)
{
	system_call_2(SYS_EVENT_FLAG_CLEAR, id, bitptn);

#ifdef REG_PASS_SYS_EVENT_FLAG_CLEAR
	REG_PASS_SYS_EVENT_FLAG_CLEAR;
#endif
	return_to_user_prog(int);
}

extern inline int sys_event_flag_cancel(sys_event_flag_t id, int *num);

extern inline int sys_event_flag_cancel(sys_event_flag_t id, int *num)
{
	system_call_2(SYS_EVENT_FLAG_CANCEL, id, (uint32_t) num);

#ifdef REG_PASS_SYS_EVENT_FLAG_CANCEL
	REG_PASS_SYS_EVENT_FLAG_CANCEL;
#endif
	return_to_user_prog(int);
}

extern inline int sys_event_flag_get(sys_event_flag_t id, uint64_t * bitptn);

extern inline int sys_event_flag_get(sys_event_flag_t id, uint64_t * bitptn)
{
	system_call_2(SYS_EVENT_FLAG_GET, id, (uint32_t) bitptn);

#ifdef REG_PASS_SYS_EVENT_FLAG_GET
	REG_PASS_SYS_EVENT_FLAG_GET;
#endif
	return_to_user_prog(int);
}





/**
 * sys_lwmutex_attribute_initialize macro sets default values to the specified 
 * lwmutex attribute structure.
 * The default values are  SYS_SYNC_PRIORITY, SYS_SYNC_NOT_RECURSIVE.
 */
#define sys_lwmutex_attribute_initialize(x)              \
    do{                                                  \
        x.attr_protocol = SYS_SYNC_PRIORITY;             \
        x.attr_recursive =  SYS_SYNC_NOT_RECURSIVE;      \
        x.name[0] = '\0';                                \
    }while(0)

/**
 * The sys_lwmutex_attribute_name_set() function sets a specified name to the 
 * specified name area of an attribute structure.
 */
extern inline
	void sys_lwmutex_attribute_name_set(char attr_name[], const char *name);

extern inline
	void sys_lwmutex_attribute_name_set(char attr_name[], const char *name)
{
	_SYS_SYNC_ATTRIBUTE_NAME_SET(attr_name, name);
	return;
}

extern int sys_lwmutex_create(sys_lwmutex_t * mutex_id,
							  sys_lwmutex_attribute_t * attr);
extern int sys_lwmutex_destroy(sys_lwmutex_t * lwmutex_id);
extern int sys_lwmutex_lock(sys_lwmutex_t * lwmutex_id, usecond_t timeout);
extern int sys_lwmutex_trylock(sys_lwmutex_t * lwmutex_id);
extern int sys_lwmutex_unlock(sys_lwmutex_t * lwmutex_id);




#define sys_lwcond_attribute_initialize(x)                 \
    do{                                                    \
        x.name[0] = '\0';                                  \
    }while(0)

/**
 * The sys_lwcond_attribute_name_set() function sets a specified name to the 
 * specified name area of an attribute structure.
 */
extern inline
	void sys_lwcond_attribute_name_set(char attr_name[], const char *name);

extern inline
	void sys_lwcond_attribute_name_set(char attr_name[], const char *name)
{
	_SYS_SYNC_ATTRIBUTE_NAME_SET(attr_name, name);
	return;
}

extern int sys_lwcond_create(sys_lwcond_t * lwcond, sys_lwmutex_t * lwmutex,
							 sys_lwcond_attribute_t * attr);
extern int sys_lwcond_destroy(sys_lwcond_t * lwcond);
extern int sys_lwcond_wait(sys_lwcond_t * lwcond, usecond_t timeout);
extern int sys_lwcond_signal(sys_lwcond_t * lwcond);
extern int sys_lwcond_signal_all(sys_lwcond_t * lwcond);
extern int sys_lwcond_signal_to(sys_lwcond_t * lwcond, sys_ppu_thread_t thr);






CDECL_END

#endif /* __SYS_SYS_SYNCHRONIZATION_H__ */
/*@}*/
