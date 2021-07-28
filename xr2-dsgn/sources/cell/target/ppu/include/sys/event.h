/*   SCE CONFIDENTIAL                                       */
/*   PlayStation(R)3 Programmer Tool Runtime Library 310.001 */
/*   Copyright (C) 2008 Sony Computer Entertainment Inc.    */
/*   All Rights Reserved.                                   */

/**
 * \ingroup all_syscall
 * \defgroup event Event
 * \brief system calls defined in sys/event.h
 *
 * Interfaces defined in sys/event.h
 *
 * Refer to <i>Cell OS Lv-2 User's Manual</i> for the detailed description.

 *
 */
/*@{*/

#ifndef __SYS_SYS_EVENT_H__
#define __SYS_SYS_EVENT_H__

#include <sys/cdefs.h>
#include <sys/syscall.h>

#include <sys/types.h>
#include <sys/integertypes.h>
#include <sys/return_code.h>


#include <sys/synchronization.h>
#include <sys/process.h>

CDECL_BEGIN


#define SYS_EVENT_QUEUE_ID_INVALID            0xFFFFFFFFU
#define SYS_EVENT_PORT_ID_INVALID             0xFFFFFFFFU

#define SYS_PPU_QUEUE    0x01
#define SYS_SPU_QUEUE    0x02

#define SYS_EVENT_QUEUE_LOCAL 0x00
#define SYS_EVENT_PORT_NO_NAME 0x00

#define SYS_EVENT_PORT_LOCAL   0x01

#define SYS_EVENT_QUEUE_DESTROY_FORCE 0x01

/**
 * \brief Event queue attribute
 *
 * The sys_event_queue_attr structure type specifies the event queue attributes
 * This structure type can be initialized by the sys_event_queue_init() macro 
 * function.
 *
 * \see sys_event_queue_create, sys_event_queue_init()
 */
typedef struct sys_event_queue_attr {
	sys_protocol_t attr_protocol;	 /**< Policy for waiting threads */
	int type;					 /**< Event Queue type */
	char name[SYS_SYNC_NAME_SIZE];	 /**< event_queue name for debugging */
} sys_event_queue_attribute_t;

/**
 * \brief Event type
 *
 * Each event records the key value of the source port that sends the event, 
 * and three additional data.
 *
 * \see sys_event_queue_receive, sys_event_queue_tryreceive
 */
typedef struct sys_event {
	__CSTD uint64_t source;	/**< Source port name */
	__CSTD uint64_t data1;	/**< First data */
	__CSTD uint64_t data2;	/**< Second data */
	__CSTD uint64_t data3;	/**< Third data */
} sys_event_t;



/**
 * The sys_event_queue_attribute_initialize() macro sets default values to the 
 * specified event queue attributes.
 * The default values are SYS_SYNC_PRIORITY and SYS_PPU_QUEUE.
 */
#define sys_event_queue_attribute_initialize(x) \
	do{											\
		x.attr_protocol = SYS_SYNC_PRIORITY;	\
		x.type = SYS_PPU_QUEUE;					\
        x.name[0] = '\0';                       \
	}while(0)

/**
 * The sys_event_queue_attribute_name_set() function sets a specified name 
 * to the specified name area of an attribute structure.
 */
extern inline
	void sys_event_queue_attribute_name_set(char attr_name[], const char *name);

extern inline
	void sys_event_queue_attribute_name_set(char attr_name[], const char *name)
{
	_SYS_SYNC_ATTRIBUTE_NAME_SET(attr_name, name);
	return;
}

extern inline int sys_event_queue_create(sys_event_queue_t * equeue_id,
										 sys_event_queue_attribute_t * attr,
										 sys_ipc_key_t event_queue_key,
										 int size);

extern inline int sys_event_queue_create(sys_event_queue_t * equeue_id,
										 sys_event_queue_attribute_t * attr,
										 sys_ipc_key_t event_queue_key,
										 int size)
{
	system_call_4(SYS_EVENT_QUEUE_CREATE, (uint32_t) equeue_id, (uint32_t) attr,
				  event_queue_key, size);

#ifdef REG_PASS_SYS_EVENT_QUEUE_CREATE
	REG_PASS_SYS_EVENT_QUEUE_CREATE;
#endif
	return_to_user_prog(int);
}


extern inline int sys_event_queue_destroy(sys_event_queue_t equeue_id,
										  int mode);

extern inline int sys_event_queue_destroy(sys_event_queue_t equeue_id, int mode)
{
	system_call_2(SYS_EVENT_QUEUE_DESTROY, equeue_id, mode);

#ifdef REG_PASS_SYS_EVENT_QUEUE_DESTROY
	REG_PASS_SYS_EVENT_QUEUE_DESTROY;
#endif
	return_to_user_prog(int);
}


extern inline int sys_event_queue_receive(sys_event_queue_t equeue_id,
										  sys_event_t * event,
										  usecond_t timeout);

extern inline int sys_event_queue_receive(sys_event_queue_t equeue_id,
										  sys_event_t * event,
										  usecond_t timeout)
{
	system_call_3(SYS_EVENT_QUEUE_RECEIVE, equeue_id, (uint32_t) event,
				  timeout);

#ifdef REG_PASS_SYS_EVENT_QUEUE_RECEIVE
	REG_PASS_SYS_EVENT_QUEUE_RECEIVE;
#endif
	return_to_user_prog(int);
}


extern inline int sys_event_queue_tryreceive(sys_event_queue_t equeue_id,
											 sys_event_t * event_array,
											 int size, int *number);

extern inline int sys_event_queue_tryreceive(sys_event_queue_t equeue_id,
											 sys_event_t * event_array,
											 int size, int *number)
{
	system_call_4(SYS_EVENT_QUEUE_TRYRECEIVE, equeue_id, (uint32_t) event_array,
				  size, (uint32_t) number);

#ifdef REG_PASS_SYS_EVENT_QUEUE_TRYRECEIVE
	REG_PASS_SYS_EVENT_QUEUE_TRYRECEIVE;
#endif
	return_to_user_prog(int);
}



extern inline int sys_event_queue_drain(sys_event_queue_t equeue_id);

extern inline int sys_event_queue_drain(sys_event_queue_t equeue_id)
{
	system_call_1(SYS_EVENT_QUEUE_DRAIN, equeue_id);

#ifdef REG_PASS_SYS_EVENT_QUEUE_DRAIN
	REG_PASS_SYS_EVENT_QUEUE_DRAIN;
#endif
	return_to_user_prog(int);
}


extern inline int sys_event_port_create(sys_event_port_t * eport_id,
										int port_type, uint64_t name);

extern inline int sys_event_port_create(sys_event_port_t * eport_id,
										int port_type, uint64_t name)
{
	system_call_3(SYS_EVENT_PORT_CREATE, (uint32_t) eport_id, port_type, name);

#ifdef REG_PASS_SYS_EVENT_PORT_CREATE
	REG_PASS_SYS_EVENT_PORT_CREATE;
#endif
	return_to_user_prog(int);
}


extern inline int sys_event_port_destroy(sys_event_port_t eport_id);

extern inline int sys_event_port_destroy(sys_event_port_t eport_id)
{
	system_call_1(SYS_EVENT_PORT_DESTROY, eport_id);

#ifdef REG_PASS_SYS_EVENT_PORT_DESTROY
	REG_PASS_SYS_EVENT_PORT_DESTROY;
#endif
	return_to_user_prog(int);
}


extern inline int sys_event_port_send(sys_event_port_t eport_id,
									  __CSTD uint64_t data1,
									  __CSTD uint64_t data2,
									  __CSTD uint64_t data3);

extern inline int sys_event_port_send(sys_event_port_t eport_id,
									  __CSTD uint64_t data1,
									  __CSTD uint64_t data2,
									  __CSTD uint64_t data3)
{
	system_call_4(SYS_EVENT_PORT_SEND, eport_id, data1, data2, data3);

#ifdef REG_PASS_SYS_EVENT_PORT_SEND
	REG_PASS_SYS_EVENT_PORT_SEND;
#endif
	return_to_user_prog(int);
}



extern inline int sys_event_port_connect_local(sys_event_port_t event_port_id,
											   sys_event_queue_t
											   event_queue_id);

extern inline int sys_event_port_connect_local(sys_event_port_t event_port_id,
											   sys_event_queue_t event_queue_id)
{
	system_call_2(SYS_EVENT_PORT_CONNECT_LOCAL, event_port_id, event_queue_id);

#ifdef REG_PASS_SYS_EVENT_PORT_CONNECT_LOCAL
	REG_PASS_SYS_EVENT_PORT_CONNECT_LOCAL;
#endif
	return_to_user_prog(int);
}



extern inline int sys_event_port_disconnect(sys_event_port_t event_port_id);

extern inline int sys_event_port_disconnect(sys_event_port_t event_port_id)
{
	system_call_1(SYS_EVENT_PORT_DISCONNECT, event_port_id);

#ifdef REG_PASS_SYS_EVENT_PORT_DISCONNECT
	REG_PASS_SYS_EVENT_PORT_DISCONNECT;
#endif
	return_to_user_prog(int);
}



CDECL_END

#endif /* __SYS_SYS_EVENT_H__ */
/*@}*/
