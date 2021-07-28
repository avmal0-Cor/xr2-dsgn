/* -*- mode: c; tab-width: 4; c-basic-offset: 4 -*- */

/*   SCE CONFIDENTIAL                                       */
/*   PlayStation(R)3 Programmer Tool Runtime Library 310.001 */
/*   Copyright (C) 2006 Sony Computer Entertainment Inc.    */
/*   All Rights Reserved.                                   */
/**
 * DECI3 Manager
 *
 * @file deci3.h
 * @brief Header of DECI3 Manager System Call I/F for Protocol Driver
 */

#ifndef __LV2_SYS_DEBUG_DECI3_SYSCALL_DECI3_H__
#define __LV2_SYS_DEBUG_DECI3_SYSCALL_DECI3_H__

#include <sys/cdefs.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <cell/error.h>

CDECL_BEGIN

/**
 * @{
 * @name Error Code Definition
 *
 * Error codes for DECI3 are generated from CELL_ERROR_FACILITY_DEBUG(=0x8051)
 * and indivisual code between 0x0201 - 0x02ff.
 */
									/** Invalid argument specified */
#define CELL_DEBUG_ERROR_INVAL			CELL_ERROR_CAST(0x80510201)
									/** Bad session ID specified */
#define CELL_DEBUG_ERROR_BADF			CELL_ERROR_CAST(0x80510202)
									/** The session already opened */
#define CELL_DEBUG_ERROR_ALREADYUSE		CELL_ERROR_CAST(0x80510203)
									/** Too many session opened */
#define CELL_DEBUG_ERROR_MFILE			CELL_ERROR_CAST(0x80510204)
									/** Too large packet size */
#define CELL_DEBUG_ERROR_PKTSIZE		CELL_ERROR_CAST(0x80510205)
									/** Host communication not available */
#define CELL_DEBUG_ERROR_NOHOSTIF		CELL_ERROR_CAST(0x80510206)
									/** No space left on device */
#define CELL_DEBUG_ERROR_NOSPACE		CELL_ERROR_CAST(0x80510207)
									/** Operation not permitted */
#define CELL_DEBUG_ERROR_PERM			CELL_ERROR_CAST(0x80510208)
									/** Not enough memory */
#define CELL_DEBUG_ERROR_NOMEM			CELL_ERROR_CAST(0x80510209)
									/** Resource busy */
#define CELL_DEBUG_ERROR_BUSY			CELL_ERROR_CAST(0x8051020A)
									/** No communication device */
#define CELL_DEBUG_ERROR_NXIO			CELL_ERROR_CAST(0x8051020B)
									/** Internal error */
#define CELL_DEBUG_ERROR_FAULT			CELL_ERROR_CAST(0x8051020C)
/**
 * @}
 */

/* Basic type definition */

/**
 * Status code description. Defined as alias of unified error code.
 */
typedef enum sys_deci3_status_t {
	SYS_DECI3_OK = CELL_OK,
	SYS_DECI3_EINVAL = CELL_DEBUG_ERROR_INVAL,
	SYS_DECI3_EBADF = CELL_DEBUG_ERROR_BADF,
	SYS_DECI3_EALREADYUSE = CELL_DEBUG_ERROR_ALREADYUSE,
	SYS_DECI3_EMFILE = CELL_DEBUG_ERROR_MFILE,
	SYS_DECI3_EPKTSIZE = CELL_DEBUG_ERROR_PKTSIZE,
	SYS_DECI3_ENOHOSTIF = CELL_DEBUG_ERROR_NOHOSTIF,
	SYS_DECI3_ENOSPACE = CELL_DEBUG_ERROR_NOSPACE,
	SYS_DECI3_EPERM = CELL_DEBUG_ERROR_PERM,
	SYS_DECI3_ENOMEM = CELL_DEBUG_ERROR_NOMEM,
	SYS_DECI3_EBUSY = CELL_DEBUG_ERROR_BUSY,
	SYS_DECI3_ENXIO = CELL_DEBUG_ERROR_NXIO,
	SYS_DECI3_EFAULT = CELL_DEBUG_ERROR_FAULT,
} sys_deci3_status_t;

typedef __CSTD int32_t sys_deci3_session_t;
typedef __CSTD uint32_t sys_deci3_protocol_t;
typedef __CSTD uint8_t sys_deci3_port_t;
typedef __CSTD uint64_t sys_deci3_event_parameter_t;

/**
 * Event definition sent from DECI3 Manager to Protocol Drivers.
 */
typedef enum sys_deci3_event_t {
	SYS_DECI3_EVENT_COMM_ENABLED,	/**< Connection established */
	SYS_DECI3_EVENT_COMM_DISABLED,	/**< Connection not established */
	SYS_DECI3_EVENT_DATA_READY,		/**< Enable to receive packet */
	SYS_DECI3_EVENT_TX_FAILED,		/**< Packet transmission failed */
} sys_deci3_event_t;

/* DECI3 packet size definition */

/**
 * Upper limit of DECI3 packet size.
 */
#define SYS_DECI3_MAX_PACKET_LENGTH		65535

/**
 * Size of DECI3 packet header which must be equal to
 * sizeof(@ref deci3_packet_header_t).
 */
#define SYS_DECI3_HEADER_LENGTH			16

/**
 * Upper limit of data size to send by DECI3.
 */
#define SYS_DECI3_MAX_PAYLOAD_LENGTH	\
				(SYS_DECI3_MAX_PACKET_LENGTH - SYS_DECI3_HEADER_LENGTH)
/**
 * Reasonable value of the message queue size for general protocol driver
 * to receive events from DECI3 Manager.
 */
#define SYS_DECI3_DEFAULT_QUEUE_SIZE	10

/**
 * Reasonable value of the buffer size for general protocol driver
 * specified by sys_deci3_create_event_path() to store un-received packet data
 * in DECI3 Manager.
 */
#define SYS_DECI3_DEFAULT_BUFFER_SIZE	0x40000

/**
 * event port name is unique for DECI Manager: "DECI3MGR"
 */
#define SYS_DECI3_EVENT_PORT_NAME  (0x44454349334D4752ULL)

/**
 * @defgroup syscall DECI3 Manager system call API
 *
 * Lv.2 OS system call API. Prefix "" is deleted by system call
 * generator, so actual function name is "sys_deci3_*".
 *
 * @{
 */
extern inline int sys_deci3_open(sys_deci3_protocol_t protocol,
								 sys_deci3_port_t port,
								 void *option,
								 sys_deci3_session_t * session_id);

extern inline int sys_deci3_open(sys_deci3_protocol_t protocol,
								 sys_deci3_port_t port,
								 void *option, sys_deci3_session_t * session_id)
{
	system_call_4(SYS_DECI3_OPEN, protocol, port, (uint32_t) option,
				  (uint32_t) session_id);

#ifdef REG_PASS_SYS_DECI3_OPEN
	REG_PASS_SYS_DECI3_OPEN;
#endif
	return_to_user_prog(int);
}



extern inline int sys_deci3_create_event_path(sys_deci3_session_t session_id,
											  __CSTD size_t bufsize,
											  sys_event_queue_t event_queue_id);

extern inline int sys_deci3_create_event_path(sys_deci3_session_t session_id,
											  __CSTD size_t bufsize,
											  sys_event_queue_t event_queue_id)
{
	system_call_3(SYS_DECI3_CREATE_EVENT_PATH, session_id, bufsize,
				  event_queue_id);

#ifdef REG_PASS_SYS_DECI3_CREATE_EVENT_PATH
	REG_PASS_SYS_DECI3_CREATE_EVENT_PATH;
#endif
	return_to_user_prog(int);
}


extern inline int sys_deci3_close(sys_deci3_session_t session_id);

extern inline int sys_deci3_close(sys_deci3_session_t session_id)
{
	system_call_1(SYS_DECI3_CLOSE, session_id);

#ifdef REG_PASS_SYS_DECI3_CLOSE
	REG_PASS_SYS_DECI3_CLOSE;
#endif
	return_to_user_prog(int);
}


extern inline int sys_deci3_send(sys_deci3_session_t session_id,
								 uint8_t * buf, __CSTD size_t size);

extern inline int sys_deci3_send(sys_deci3_session_t session_id,
								 uint8_t * buf, __CSTD size_t size)
{
	system_call_3(SYS_DECI3_SEND, session_id, (uint32_t) buf, size);

#ifdef REG_PASS_SYS_DECI3_SEND
	REG_PASS_SYS_DECI3_SEND;
#endif
	return_to_user_prog(int);
}


extern inline int sys_deci3_receive(sys_deci3_session_t session_id,
									uint8_t * buf, __CSTD size_t size);

extern inline int sys_deci3_receive(sys_deci3_session_t session_id,
									uint8_t * buf, __CSTD size_t size)
{
	system_call_3(SYS_DECI3_RECEIVE, session_id, (uint32_t) buf, size);

#ifdef REG_PASS_SYS_DECI3_RECEIVE
	REG_PASS_SYS_DECI3_RECEIVE;
#endif
	return_to_user_prog(int);
}

/**
 * @}
 */

/* special system calls for debug use only */

CDECL_END

#endif /* ! __LV2_SYS_DEBUG_DECI3_SYSCALL_DECI3_H__ */

/* end of deci3.h */
