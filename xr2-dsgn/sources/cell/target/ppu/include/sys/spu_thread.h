/*   SCE CONFIDENTIAL                                       */
/*   PlayStation(R)3 Programmer Tool Runtime Library 310.001 */
/*   Copyright (C) 2008 Sony Computer Entertainment Inc.    */
/*   All Rights Reserved.                                   */

/** \ingroup all_syscall
 * \defgroup spu_thread SPU Threads
 * \brief system calls defined in sys/spu_thread.h
 *
 * Interfaces defined in sys/spu_thread.h.
 *
 * Refer to <i>Cell OS Lv-2 User's Manual</i> for the detailed description.
 */
/*@{*/
#ifndef __SYS_SYS_SPU_THREAD_H__
#define __SYS_SYS_SPU_THREAD_H__

#include <sys/cdefs.h>
#include <sys/syscall.h>

#include <sys/integertypes.h>
#include <sys/return_code.h>
#include <sys/types.h>

#include "spu_image.h"


CDECL_BEGIN


#ifndef NULL
#define NULL    0
#endif

#define SYS_SPU_THREAD_ID_INVALID          0xFFFFFFFFU


#define SYS_SPU_THREAD_EVENT_USER 0x1
#define SYS_SPU_THREAD_EVENT_DMA  0x2
#define SYS_SPU_THREAD_EVENT_USER_KEY 0xFFFFFFFF53505501ULL
#define SYS_SPU_THREAD_EVENT_DMA_KEY  0xFFFFFFFF53505502ULL


#define SYS_SPU_THREAD_DMA_COMPLETION_STOP 0x0U
#define SYS_SPU_THREAD_DMA_COMPLETION_ANY  0x1U
#define SYS_SPU_THREAD_DMA_COMPLETION_ALL  0x2U


#define SYS_SPU_THREAD_ARGUMENT_LET_8(x) (((uint64_t)(x)) << 32U)
#define SYS_SPU_THREAD_ARGUMENT_LET_16(x) (((uint64_t)(x)) << 32U)
#define SYS_SPU_THREAD_ARGUMENT_LET_32(x) (((uint64_t)(x)) << 32U)
#define SYS_SPU_THREAD_ARGUMENT_LET_64(x) (x)


/** \brief SPU thread argument structure
 * 
 * The sys_spu_thread_argument_t structure type is used to pass arguments 
 * to an SPU thread by sys_spu_thread_initialize().
 *
 * \see sys_spu_thread_initialize
 */
typedef struct sys_spu_thread_argument {
	__CSTD uint64_t arg1;
						/**< arg1 (loaded to SPU GPR3) */
	__CSTD uint64_t arg2;
						/**< arg2 (loaded to SPU GPR4) */
	__CSTD uint64_t arg3;
						/**< arg3 (loaded to SPU GPR5) */
	__CSTD uint64_t arg4;
						/**< arg4 (loaded to SPU GPR6) */
} sys_spu_thread_argument_t;


#define SYS_SPU_THREAD_OPTION_NONE                 0x00000000U
#define SYS_SPU_THREAD_OPTION_ASYNC_INTR_ENABLE    0x00000001U
#define SYS_SPU_THREAD_OPTION_DEC_SYNC_TB_ENABLE   0x00000002U

/** \brief SPU thread attribute sturcture
 *
 * The sys_spu_thread_attribute_t structure type is used to set 
 * attributes to an SPU thread by sys_spu_thread_initialize().
 *
 * \see sys_spu_thread_initialize, sys_spu_thread_segment_t
 */
struct sys_spu_thread_attribute {
  /** A string of the name for this SPU thread */
	const char *name;
  /** The length of \a name including the null terminator */
	__CSTD size_t nsize;
  /** SPU thread options (NONE/SYNC_INTR_ENABLE/DEC_SYNC_TB_ENABLE) */
	__CSTD uint32_t option;
};
typedef struct sys_spu_thread_attribute sys_spu_thread_attribute_t;


/**
 * \brief Initialize an SPU thread attribute structure
 *
 * The sys_spu_thread_attribute_initialize() function initializes an SPU thread
 * attribute structure \a attr with the default value.
 *
 * \param attr  an SPU thread attribute structure
 *
 */
#define sys_spu_thread_attribute_initialize(x) \
    do {									   \
        x.name = NULL;                         \
        x.nsize = 0;                           \
        x.option = SYS_SPU_THREAD_OPTION_NONE; \
    } while(0)


/**
 * \brief Set name and nsize member in an SPU thread attribute structure
 *
 * The sys_spu_thread_attribute_name() function sets a name and a nsize member
 * in an SPU thread attribute structure \a attr with \a name to name
 * an SPU thread.
 *
 * \param attr  an SPU thread attribute structure
 * \param name  a pointer to a string to name
 *
 */
#define sys_spu_thread_attribute_name(x,s)              \
    do {												\
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
 * \brief Set option member in an SPU thread attribute structure
 *
 * The sys_spu_thread_attribute_option() function sets an option member in
 * an SPU thread attribute structure \a attr with \a option.
 *
 * \param attr    an SPU thread attribute structure
 * \param option  option to be specified
 *
 */
#define sys_spu_thread_attribute_option(x,f)   \
    do {									   \
        x.option = f;						   \
    } while(0)


/**
 * \brief Initialize an SPU thread argument structure
 *
 * The sys_spu_thread_argument_initialize() function initialize an SPU thread
 * argument structure \arg with zeros.
 *
 * \param arg  an SPU thread argument structure
 *
 */
#define sys_spu_thread_argument_initialize(x)   \
    do {										\
        x.arg1 = x.arg2 = x.arg3 = x.arg4 = 0;  \
    } while(0)


/* Basic operations */
extern inline int sys_spu_thread_initialize(sys_spu_thread_t * thread,
											sys_spu_thread_group_t group,
											unsigned int spu_num,
											sys_spu_image_t * img,
											sys_spu_thread_attribute_t * attr,
											sys_spu_thread_argument_t * arg);

extern inline int sys_spu_thread_initialize(sys_spu_thread_t * thread,
											sys_spu_thread_group_t group,
											unsigned int spu_num,
											sys_spu_image_t * img,
											sys_spu_thread_attribute_t * attr,
											sys_spu_thread_argument_t * arg)
{
	system_call_6(SYS_SPU_THREAD_INITIALIZE, (uint32_t) thread, group, spu_num,
				  (uint32_t) img, (uint32_t) attr, (uint32_t) arg);

#ifdef REG_PASS_SYS_SPU_THREAD_INITIALIZE
	REG_PASS_SYS_SPU_THREAD_INITIALIZE;
#endif
	return_to_user_prog(int);
}


extern inline int sys_spu_thread_set_argument(sys_spu_thread_t id,
											  sys_spu_thread_argument_t * arg);

extern inline int sys_spu_thread_set_argument(sys_spu_thread_t id,
											  sys_spu_thread_argument_t * arg)
{
	system_call_2(SYS_SPU_THREAD_SET_ARGUMENT, id, (uint32_t) arg);

#ifdef REG_PASS_SYS_SPU_THREAD_SET_ARGUMENT
	REG_PASS_SYS_SPU_THREAD_SET_ARGUMENT;
#endif
	return_to_user_prog(int);
}


extern inline int sys_spu_thread_get_exit_status(sys_spu_thread_t id,
												 int *status);

extern inline int sys_spu_thread_get_exit_status(sys_spu_thread_t id,
												 int *status)
{
	system_call_2(SYS_SPU_THREAD_GET_EXIT_STATUS, id, (uint32_t) status);

#ifdef REG_PASS_SYS_SPU_THREAD_GET_EXIT_STATUS
	REG_PASS_SYS_SPU_THREAD_GET_EXIT_STATUS;
#endif
	return_to_user_prog(int);
}


extern inline int sys_spu_thread_connect_event(sys_spu_thread_t id,
											   sys_event_queue_t eq,
											   sys_event_type_t et,
											   __CSTD uint8_t spup);

extern inline int sys_spu_thread_connect_event(sys_spu_thread_t id,
											   sys_event_queue_t eq,
											   sys_event_type_t et,
											   __CSTD uint8_t spup)
{
	system_call_4(SYS_SPU_THREAD_CONNECT_EVENT, id, eq, et, spup);

#ifdef REG_PASS_SYS_SPU_THREAD_CONNECT_EVENT
	REG_PASS_SYS_SPU_THREAD_CONNECT_EVENT;
#endif
	return_to_user_prog(int);
}


extern inline int sys_spu_thread_disconnect_event(sys_spu_thread_t id,
												  sys_event_type_t et,
												  __CSTD uint8_t spup);

extern inline int sys_spu_thread_disconnect_event(sys_spu_thread_t id,
												  sys_event_type_t et,
												  __CSTD uint8_t spup)
{
	system_call_3(SYS_SPU_THREAD_DISCONNECT_EVENT, id, et, spup);

#ifdef REG_PASS_SYS_SPU_THREAD_DISCONNECT_EVENT
	REG_PASS_SYS_SPU_THREAD_DISCONNECT_EVENT;
#endif
	return_to_user_prog(int);
}


extern inline int sys_spu_thread_bind_queue(sys_spu_thread_t id,
											sys_event_queue_t spuq,
											__CSTD uint32_t spuq_num);

extern inline int sys_spu_thread_bind_queue(sys_spu_thread_t id,
											sys_event_queue_t spuq,
											__CSTD uint32_t spuq_num)
{
	system_call_3(SYS_SPU_THREAD_BIND_QUEUE, id, spuq, spuq_num);

#ifdef REG_PASS_SYS_SPU_THREAD_BIND_QUEUE
	REG_PASS_SYS_SPU_THREAD_BIND_QUEUE;
#endif
	return_to_user_prog(int);
}


extern inline int sys_spu_thread_unbind_queue(sys_spu_thread_t id,
											  __CSTD uint32_t spuq_num);

extern inline int sys_spu_thread_unbind_queue(sys_spu_thread_t id,
											  __CSTD uint32_t spuq_num)
{
	system_call_2(SYS_SPU_THREAD_UNBIND_QUEUE, id, spuq_num);

#ifdef REG_PASS_SYS_SPU_THREAD_UNBIND_QUEUE
	REG_PASS_SYS_SPU_THREAD_UNBIND_QUEUE;
#endif
	return_to_user_prog(int);
}



/* Context operations */
extern inline int sys_spu_thread_write_ls(sys_spu_thread_t id,
										  __CSTD uint32_t address,
										  __CSTD uint64_t value,
										  __CSTD size_t type);

extern inline int sys_spu_thread_write_ls(sys_spu_thread_t id,
										  __CSTD uint32_t address,
										  __CSTD uint64_t value,
										  __CSTD size_t type)
{
	system_call_4(SYS_SPU_THREAD_WRITE_LS, id, address, value, type);

#ifdef REG_PASS_SYS_SPU_THREAD_WRITE_LS
	REG_PASS_SYS_SPU_THREAD_WRITE_LS;
#endif
	return_to_user_prog(int);
}

extern inline int sys_spu_thread_read_ls(sys_spu_thread_t id,
										 __CSTD uint32_t address,
										 __CSTD uint64_t * value,
										 __CSTD size_t type);

extern inline int sys_spu_thread_read_ls(sys_spu_thread_t id,
										 __CSTD uint32_t address,
										 __CSTD uint64_t * value,
										 __CSTD size_t type)
{
	system_call_4(SYS_SPU_THREAD_READ_LS, id, address, (uint32_t) value, type);

#ifdef REG_PASS_SYS_SPU_THREAD_READ_LS
	REG_PASS_SYS_SPU_THREAD_READ_LS;
#endif
	return_to_user_prog(int);
}


extern inline int sys_spu_thread_write_snr(sys_spu_thread_t id, int number,
										   __CSTD uint32_t value);

extern inline int sys_spu_thread_write_snr(sys_spu_thread_t id, int number,
										   __CSTD uint32_t value)
{
	system_call_3(SYS_SPU_THREAD_WRITE_SNR, id, number, value);

#ifdef REG_PASS_SYS_SPU_THREAD_WRITE_SNR
	REG_PASS_SYS_SPU_THREAD_WRITE_SNR;
#endif
	return_to_user_prog(int);
}



extern inline int sys_spu_thread_set_spu_cfg(sys_spu_thread_t id,
											 __CSTD uint64_t value);

extern inline int sys_spu_thread_set_spu_cfg(sys_spu_thread_t id,
											 __CSTD uint64_t value)
{
	system_call_2(SYS_SPU_THREAD_SET_SPU_CFG, id, value);

#ifdef REG_PASS_SYS_SPU_THREAD_SET_SPU_CFG
	REG_PASS_SYS_SPU_THREAD_SET_SPU_CFG;
#endif
	return_to_user_prog(int);
}


extern inline int sys_spu_thread_get_spu_cfg(sys_spu_thread_t id,
											 __CSTD uint64_t * value);

extern inline int sys_spu_thread_get_spu_cfg(sys_spu_thread_t id,
											 __CSTD uint64_t * value)
{
	system_call_2(SYS_SPU_THREAD_GET_SPU_CFG, id, (uint32_t) value);

#ifdef REG_PASS_SYS_SPU_THREAD_GET_SPU_CFG
	REG_PASS_SYS_SPU_THREAD_GET_SPU_CFG;
#endif
	return_to_user_prog(int);
}


extern inline int sys_spu_thread_write_spu_mb(sys_spu_thread_t id,
											  __CSTD uint32_t value);

extern inline int sys_spu_thread_write_spu_mb(sys_spu_thread_t id,
											  __CSTD uint32_t value)
{
	system_call_2(SYS_SPU_THREAD_WRITE_SPU_MB, id, value);

#ifdef REG_PASS_SYS_SPU_THREAD_WRITE_SPU_MB
	REG_PASS_SYS_SPU_THREAD_WRITE_SPU_MB;
#endif
	return_to_user_prog(int);
}


extern inline int sys_spu_thread_recover_page_fault(sys_spu_thread_t id);

extern inline int sys_spu_thread_recover_page_fault(sys_spu_thread_t id)
{
	system_call_1(SYS_SPU_THREAD_RECOVER_PAGE_FAULT, id);

#ifdef REG_PASS_SYS_SPU_THREAD_RECOVER_PAGE_FAULT
	REG_PASS_SYS_SPU_THREAD_RECOVER_PAGE_FAULT;
#endif
	return_to_user_prog(int);
}



/* For compatible with SDK050 */
static int sys_spu_thread_write_to_ls(sys_spu_thread_t id,
									  __CSTD uint32_t address,
									  __CSTD uint64_t value, __CSTD size_t type)
	__attribute__ ((deprecated));

static int sys_spu_thread_read_from_ls(sys_spu_thread_t id,
									   __CSTD uint32_t address,
									   __CSTD uint64_t * value,
									   __CSTD size_t type)
	__attribute__ ((deprecated));

static int sys_spu_thread_write_signal(sys_spu_thread_t id,
									   int number, __CSTD uint32_t value)
	__attribute__ ((deprecated));


extern inline int sys_spu_thread_write_to_ls(sys_spu_thread_t id,
											 __CSTD uint32_t address,
											 __CSTD uint64_t value,
											 __CSTD size_t type)
{
	return sys_spu_thread_write_ls(id, address, value, type);
}


extern inline int sys_spu_thread_read_from_ls(sys_spu_thread_t id,
											  __CSTD uint32_t address,
											  __CSTD uint64_t * value,
											  __CSTD size_t type)
{
	return sys_spu_thread_read_ls(id, address, value, type);
}

extern inline int sys_spu_thread_write_signal(sys_spu_thread_t id,
											  int number, __CSTD uint32_t value)
{
	return sys_spu_thread_write_snr(id, number, value);
}




CDECL_END

#endif /* __SYS_SYS_SPU_THREAD_H__ */
/*@}*/
