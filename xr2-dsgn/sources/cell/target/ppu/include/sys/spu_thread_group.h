/*   SCE CONFIDENTIAL                                       */
/*   PlayStation(R)3 Programmer Tool Runtime Library 310.001 */
/*   Copyright (C) 2009 Sony Computer Entertainment Inc.    */
/*   All Rights Reserved.                                   */

/** \ingroup all_syscall
 * \defgroup spu_thread_group SPU Thread Groups
 * \brief system calls defined in sys/spu_thread_group.h
 *
 * Interfaces defined in sys/spu_thread_group.h.
 *
 * Refer to <i>Cell OS Lv-2 User's Manual</i> for the detailed description.
 */


/*@{*/

#ifndef __SYS_SYS_SPU_THREAD_GROUP_H__
#define __SYS_SYS_SPU_THREAD_GROUP_H__

#include <sys/cdefs.h>
#include <sys/syscall.h>

#include <sys/integertypes.h>
#include <sys/return_code.h>
#include <sys/types.h>

CDECL_BEGIN


#ifndef NULL
#define NULL    0
#endif

#define SYS_SPU_THREAD_GROUP_TYPE_NORMAL                 0x00
#define SYS_SPU_THREAD_GROUP_TYPE_SEQUENTIAL             0x01
#define SYS_SPU_THREAD_GROUP_TYPE_SYSTEM                 0x02
#define SYS_SPU_THREAD_GROUP_TYPE_MEMORY_FROM_CONTAINER  0x04
#define SYS_SPU_THREAD_GROUP_TYPE_NON_CONTEXT            0x08
#define SYS_SPU_THREAD_GROUP_TYPE_EXCLUSIVE_NON_CONTEXT  0x18
#define SYS_SPU_THREAD_GROUP_TYPE_COOPERATE_WITH_SYSTEM  0x20


#define SYS_SPU_THREAD_GROUP_JOIN_GROUP_EXIT		0x0001
#define SYS_SPU_THREAD_GROUP_JOIN_ALL_THREADS_EXIT	0x0002
#define SYS_SPU_THREAD_GROUP_JOIN_TERMINATED		0x0004

#define SYS_SPU_THREAD_GROUP_EVENT_RUN               0x1
#define SYS_SPU_THREAD_GROUP_EVENT_RUN_KEY           0xFFFFFFFF53505500ULL
#define SYS_SPU_THREAD_GROUP_EVENT_EXCEPTION         0x2
#define SYS_SPU_THREAD_GROUP_EVENT_EXCEPTION_KEY     0xFFFFFFFF53505503ULL
#define SYS_SPU_THREAD_GROUP_EVENT_SYSTEM_MODULE     0x4
#define SYS_SPU_THREAD_GROUP_EVENT_SYSTEM_MODULE_KEY 0xFFFFFFFF53505504ULL

/* exception causes */
#define SYS_SPU_EXCEPTION_DMA_ALIGNMENT  0x0001U
#define SYS_SPU_EXCEPTION_DMA_COMMAND    0x0002U
#define SYS_SPU_EXCEPTION_SPU_ERROR      0x0004U
#define SYS_SPU_EXCEPTION_MFC_FIR        0x0008U
#define SYS_SPU_EXCEPTION_MFC_SEGMENT    0x0010U
#define SYS_SPU_EXCEPTION_MFC_STORAGE    0x0020U
#define SYS_SPU_EXCEPTION_STOP_CALL      0x0100U
#define SYS_SPU_EXCEPTION_STOP_BREAK     0x0200U
#define SYS_SPU_EXCEPTION_HALT           0x0400U
#define SYS_SPU_EXCEPTION_UNKNOWN_SIGNAL 0x0800U
#define SYS_SPU_EXCEPTION_NON_CONTEXT    0x1000U
#define SYS_SPU_EXCEPTION_MAT            0x2000U
#define SYS_SPU_EXCEPTION_NO_VALUE          0x0U



/** \brief SPU thread group attribute structure
 *
 * The sys_spu_thread_group_attribute_t structure type is used to set
 * attributes to an SPU thread group by sys_spu_thread_group_create().
 *
 * \a type must be one of the following values.
 *  - SYS_SPU_THREAD_GROUP_TYPE_NORMAL \n
 *    Normal SPU thread group.
 *  - SYS_SPU_THREAD_GROUP_TYPE_SEQUENTIAL \n
 *    An SPU thread group whose execution is triggered by termination of 
 *    other SPU thread groups.
 *
 * \see sys_spu_thread_group_create
 */
struct sys_spu_thread_group_attribute {
	/** The length of \a name including the null terminator */
	__CSTD size_t nsize;
	/** A string of the name for this SPU thread group */
	const char *name;
	/** SPU thread group type */
	int type;
	/** A memory container.  It is used only if
		SYS_SPU_THREAD_GROUP_TYPE_MEMORY_FROM_CONTAINER is specified */
	union {
		sys_memory_container_t ct;
	} option;
};
typedef struct sys_spu_thread_group_attribute sys_spu_thread_group_attribute_t;


/**
 * \brief Initialize an SPU thread group attribute structure
 *
 * The sys_spu_thread_group_attribute_initialize() function initializes an SPU
 * thread group attribute structure \a attr with the default value.
 *
 * \param attr  an SPU thread group attribute structure
 *
 */
#define sys_spu_thread_group_attribute_initialize(x) \
    do {                                             \
		x.name = NULL;								 \
        x.nsize = 0;                                 \
        x.type = SYS_SPU_THREAD_GROUP_TYPE_NORMAL;   \
    } while(0)


/**
 * \brief Set name and nsize member in an SPU thread group attribute structure
 *
 * The sys_spu_thread_group_attribute_name() function sets a name and a nsize
 * member in an SPU thread group attribute structure \a attr with \a name to
 * name an SPU thread group.
 *
 * \param attr an SPU thread group attribute structure
 * \param name a pointer to a string to name
 *
 */
#define sys_spu_thread_group_attribute_name(x,s)        \
    do {                                                \
        x.name = s;                                     \
        if (s == NULL) {                                \
            x.nsize = 0;                                \
        } else {                                        \
            int n = 0;                                  \
            for (; (n<127) && (s[n] != '\0'); n++)      \
                ;                                       \
            x.nsize = n + 1;                            \
        }                                               \
    } while(0)


/**
 * \brief Set type member in an SPU thread group attribute structure
 *
 * The sys_spu_thread_group_attribute_type() function sets a type member in
 * an SPU thread group attribute structure \a attr with \a type.
 *
 * \param attr    an SPU thread attribute structure
 * \param type    type to be specified
 *
 */
#define sys_spu_thread_group_attribute_type(x,f) \
	do {										 \
        x.type = f;								 \
    } while(0)


/**
 * \brief Set memory container member in an SPU thread group attribute structure
 *
 * The sys_spu_thread_group_attribute_memory_container() function sets a type
 * member in an SPU thread group attribute structure \a attr with \a type.
 *
 * \param attr       an SPU thread attribute structure
 * \param container  memory container to be specified
 *
 */
#define sys_spu_thread_group_attribute_memory_container(x,f)		\
	do {															\
        x.type |= SYS_SPU_THREAD_GROUP_TYPE_MEMORY_FROM_CONTAINER;	\
		x.option.ct = f;											\
    } while(0)




extern inline int sys_spu_thread_group_create(sys_spu_thread_group_t * id,
											  unsigned int num, int prio,
											  sys_spu_thread_group_attribute_t *
											  attr);

extern inline int sys_spu_thread_group_create(sys_spu_thread_group_t * id,
											  unsigned int num, int prio,
											  sys_spu_thread_group_attribute_t *
											  attr)
{
	system_call_4(SYS_SPU_THREAD_GROUP_CREATE, (uint32_t) id, num, prio,
				  (uint32_t) attr);

#ifdef REG_PASS_SYS_SPU_THREAD_GROUP_CREATE
	REG_PASS_SYS_SPU_THREAD_GROUP_CREATE;
#endif
	return_to_user_prog(int);
}

extern inline int sys_spu_thread_group_destroy(sys_spu_thread_group_t id);

extern inline int sys_spu_thread_group_destroy(sys_spu_thread_group_t id)
{
	system_call_1(SYS_SPU_THREAD_GROUP_DESTROY, id);

#ifdef REG_PASS_SYS_SPU_THREAD_GROUP_DESTROY
	REG_PASS_SYS_SPU_THREAD_GROUP_DESTROY;
#endif
	return_to_user_prog(int);
}


extern inline int sys_spu_thread_group_start(sys_spu_thread_group_t id);

extern inline int sys_spu_thread_group_start(sys_spu_thread_group_t id)
{
	system_call_1(SYS_SPU_THREAD_GROUP_START, id);

#ifdef REG_PASS_SYS_SPU_THREAD_GROUP_START
	REG_PASS_SYS_SPU_THREAD_GROUP_START;
#endif
	return_to_user_prog(int);
}

extern inline int sys_spu_thread_group_start_on_exit(sys_spu_thread_group_t gid,
													 int ngroups,
													 sys_spu_thread_group_t *
													 groups);

extern inline int sys_spu_thread_group_start_on_exit(sys_spu_thread_group_t gid,
													 int ngroups,
													 sys_spu_thread_group_t *
													 groups)
{
	system_call_3(SYS_SPU_THREAD_GROUP_START_ON_EXIT, gid, ngroups,
				  (uint32_t) groups);

#ifdef REG_PASS_SYS_SPU_THREAD_GROUP_START_ON_EXIT
	REG_PASS_SYS_SPU_THREAD_GROUP_START_ON_EXIT;
#endif
	return_to_user_prog(int);
}


extern inline int sys_spu_thread_group_terminate(sys_spu_thread_group_t id,
												 int value);

extern inline int sys_spu_thread_group_terminate(sys_spu_thread_group_t id,
												 int value)
{
	system_call_2(SYS_SPU_THREAD_GROUP_TERMINATE, id, value);

#ifdef REG_PASS_SYS_SPU_THREAD_GROUP_TERMINATE
	REG_PASS_SYS_SPU_THREAD_GROUP_TERMINATE;
#endif
	return_to_user_prog(int);
}


extern inline int sys_spu_thread_group_suspend(sys_spu_thread_group_t id);

extern inline int sys_spu_thread_group_suspend(sys_spu_thread_group_t id)
{
	system_call_1(SYS_SPU_THREAD_GROUP_SUSPEND, id);

#ifdef REG_PASS_SYS_SPU_THREAD_GROUP_SUSPEND
	REG_PASS_SYS_SPU_THREAD_GROUP_SUSPEND;
#endif
	return_to_user_prog(int);
}

extern inline int sys_spu_thread_group_resume(sys_spu_thread_group_t id);

extern inline int sys_spu_thread_group_resume(sys_spu_thread_group_t id)
{
	system_call_1(SYS_SPU_THREAD_GROUP_RESUME, id);

#ifdef REG_PASS_SYS_SPU_THREAD_GROUP_RESUME
	REG_PASS_SYS_SPU_THREAD_GROUP_RESUME;
#endif
	return_to_user_prog(int);
}


extern inline int sys_spu_thread_group_yield(sys_spu_thread_group_t id);

extern inline int sys_spu_thread_group_yield(sys_spu_thread_group_t id)
{
	system_call_1(SYS_SPU_THREAD_GROUP_YIELD, id);

#ifdef REG_PASS_SYS_SPU_THREAD_GROUP_YIELD
	REG_PASS_SYS_SPU_THREAD_GROUP_YIELD;
#endif
	return_to_user_prog(int);
}


extern inline int sys_spu_thread_group_set_priority(sys_spu_thread_group_t id,
													int priority);

extern inline int sys_spu_thread_group_set_priority(sys_spu_thread_group_t id,
													int priority)
{
	system_call_2(SYS_SPU_THREAD_GROUP_SET_PRIORITY, id, priority);

#ifdef REG_PASS_SYS_SPU_THREAD_GROUP_SET_PRIORITY
	REG_PASS_SYS_SPU_THREAD_GROUP_SET_PRIORITY;
#endif
	return_to_user_prog(int);
}

extern inline int sys_spu_thread_group_get_priority(sys_spu_thread_group_t id,
													int *priority);

extern inline int sys_spu_thread_group_get_priority(sys_spu_thread_group_t id,
													int *priority)
{
	system_call_2(SYS_SPU_THREAD_GROUP_GET_PRIORITY, id, (uint32_t) priority);

#ifdef REG_PASS_SYS_SPU_THREAD_GROUP_GET_PRIORITY
	REG_PASS_SYS_SPU_THREAD_GROUP_GET_PRIORITY;
#endif
	return_to_user_prog(int);
}


extern inline int sys_spu_thread_group_join(sys_spu_thread_group_t gid,
											int *cause, int *status);

extern inline int sys_spu_thread_group_join(sys_spu_thread_group_t gid,
											int *cause, int *status)
{
	system_call_3(SYS_SPU_THREAD_GROUP_JOIN, gid, (uint32_t) cause,
				  (uint32_t) status);

#ifdef REG_PASS_SYS_SPU_THREAD_GROUP_JOIN
	REG_PASS_SYS_SPU_THREAD_GROUP_JOIN;
#endif
	return_to_user_prog(int);
}


extern inline int sys_spu_thread_group_connect_event(sys_spu_thread_group_t id,
													 sys_event_queue_t eq,
													 sys_event_type_t et);

extern inline int sys_spu_thread_group_connect_event(sys_spu_thread_group_t id,
													 sys_event_queue_t eq,
													 sys_event_type_t et)
{
	system_call_3(SYS_SPU_THREAD_GROUP_CONNECT_EVENT, id, eq, et);

#ifdef REG_PASS_SYS_SPU_THREAD_GROUP_CONNECT_EVENT
	REG_PASS_SYS_SPU_THREAD_GROUP_CONNECT_EVENT;
#endif
	return_to_user_prog(int);
}

extern inline int sys_spu_thread_group_disconnect_event(sys_spu_thread_group_t
														id,
														sys_event_type_t et);

extern inline int sys_spu_thread_group_disconnect_event(sys_spu_thread_group_t
														id, sys_event_type_t et)
{
	system_call_2(SYS_SPU_THREAD_GROUP_DISCONNECT_EVENT, id, et);

#ifdef REG_PASS_SYS_SPU_THREAD_GROUP_DISCONNECT_EVENT
	REG_PASS_SYS_SPU_THREAD_GROUP_DISCONNECT_EVENT;
#endif
	return_to_user_prog(int);
}



extern inline int
sys_spu_thread_group_connect_event_all_threads(sys_spu_thread_group_t id,
											   sys_event_queue_t eq,
											   uint64_t req, uint8_t * spup);

extern inline int
sys_spu_thread_group_connect_event_all_threads(sys_spu_thread_group_t id,
											   sys_event_queue_t eq,
											   uint64_t req, uint8_t * spup)
{
	system_call_4(SYS_SPU_THREAD_GROUP_CONNECT_EVENT_ALL_THREADS, id, eq, req,
				  (uint32_t) spup);

#ifdef REG_PASS_SYS_SPU_THREAD_GROUP_CONNECT_EVENT_ALL_THREADS
	REG_PASS_SYS_SPU_THREAD_GROUP_CONNECT_EVENT_ALL_THREADS;
#endif
	return_to_user_prog(int);
}

extern inline int
sys_spu_thread_group_disconnect_event_all_threads(sys_spu_thread_group_t id,
												  uint8_t spup);

extern inline int
sys_spu_thread_group_disconnect_event_all_threads(sys_spu_thread_group_t id,
												  uint8_t spup)
{
	system_call_2(SYS_SPU_THREAD_GROUP_DISCONNECT_EVENT_ALL_THREADS, id, spup);

#ifdef REG_PASS_SYS_SPU_THREAD_GROUP_DISCONNECT_EVENT_ALL_THREADS
	REG_PASS_SYS_SPU_THREAD_GROUP_DISCONNECT_EVENT_ALL_THREADS;
#endif
	return_to_user_prog(int);
}




extern inline int
sys_spu_thread_group_set_cooperative_victims(sys_spu_thread_group_t id,
											 uint32_t victims);

extern inline int
sys_spu_thread_group_set_cooperative_victims(sys_spu_thread_group_t id,
											 uint32_t victims)
{
	system_call_2(SYS_SPU_THREAD_GROUP_SET_COOPERATIVE_VICTIMS, id, victims);

#ifdef REG_PASS_SYS_SPU_THREAD_GROUP_SET_COOPERATIVE_VICTIMS
	REG_PASS_SYS_SPU_THREAD_GROUP_SET_COOPERATIVE_VICTIMS;
#endif
	return_to_user_prog(int);
}



CDECL_END

#endif /* __SYS_SYS_SPU_THREAD_GROUP_H__ */
/*@}*/
