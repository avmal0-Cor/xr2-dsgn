/*   SCE CONFIDENTIAL                                       */
/*   PlayStation(R)3 Programmer Tool Runtime Library 310.001 */
/*   Copyright (C) 2008 Sony Computer Entertainment Inc.    */
/*   All Rights Reserved.                                   */

/**
 * \ingroup all_syscall
 * \defgroup timer Timer
 * \brief system calls defined in sys/timer.h
 *  
 * Interfaces defined in sys/timer.h
 *
 * Refer to <i>Cell OS Lv-2 User's Manual</i> for the detailed description. 
 */
/*@{*/

#ifndef __SYS_SYS_TIMER_H__
#define __SYS_SYS_TIMER_H__

#include <sys/cdefs.h>
#include <sys/syscall.h>
#include <sys/types.h>


CDECL_BEGIN


extern inline int sys_timer_usleep(usecond_t sleep_time);

extern inline int sys_timer_usleep(usecond_t sleep_time)
{
	system_call_1(SYS_TIMER_USLEEP, sleep_time);

#ifdef REG_PASS_SYS_TIMER_USLEEP
	REG_PASS_SYS_TIMER_USLEEP;
#endif
	return_to_user_prog(int);
}

extern inline int sys_timer_sleep(second_t sleep_time);

extern inline int sys_timer_sleep(second_t sleep_time)
{
	system_call_1(SYS_TIMER_SLEEP, sleep_time);

#ifdef REG_PASS_SYS_TIMER_SLEEP
	REG_PASS_SYS_TIMER_SLEEP;
#endif
	return_to_user_prog(int);
}


#define SYS_TIMER_ID_INVALID		0xFFFFFFFFU

#define SYS_TIMER_STATE_STOP        0x00U
#define SYS_TIMER_STATE_RUN         0x01U

#define SYS_TIMER_EVENT_NO_NAME     0x00U

typedef struct {
	system_time_t next_expiration_time;
	usecond_t period;
	__CSTD uint32_t timer_state;
	__CSTD uint32_t pad;
} sys_timer_information_t;

extern inline int sys_timer_create(sys_timer_t * timer_id);

extern inline int sys_timer_create(sys_timer_t * timer_id)
{
	system_call_1(SYS_TIMER_CREATE, (uint32_t) timer_id);

#ifdef REG_PASS_SYS_TIMER_CREATE
	REG_PASS_SYS_TIMER_CREATE;
#endif
	return_to_user_prog(int);
}


extern inline int sys_timer_destroy(sys_timer_t timer_id);

extern inline int sys_timer_destroy(sys_timer_t timer_id)
{
	system_call_1(SYS_TIMER_DESTROY, timer_id);

#ifdef REG_PASS_SYS_TIMER_DESTROY
	REG_PASS_SYS_TIMER_DESTROY;
#endif
	return_to_user_prog(int);
}


extern inline int sys_timer_get_information(sys_timer_t timer_id,
											sys_timer_information_t * info);

extern inline int sys_timer_get_information(sys_timer_t timer_id,
											sys_timer_information_t * info)
{
	system_call_2(SYS_TIMER_GET_INFORMATION, timer_id, (uint32_t) info);

#ifdef REG_PASS_SYS_TIMER_GET_INFORMATION
	REG_PASS_SYS_TIMER_GET_INFORMATION;
#endif
	return_to_user_prog(int);
}


extern inline int _sys_timer_start(sys_timer_t timer_id,
								   system_time_t base_time, usecond_t period);

extern inline int _sys_timer_start(sys_timer_t timer_id,
								   system_time_t base_time, usecond_t period)
{
	system_call_3(_SYS_TIMER_START, timer_id, base_time, period);

#ifdef REG_PASS__SYS_TIMER_START
	REG_PASS__SYS_TIMER_START;
#endif
	return_to_user_prog(int);
}


extern inline int sys_timer_stop(sys_timer_t timer_id);

extern inline int sys_timer_stop(sys_timer_t timer_id)
{
	system_call_1(SYS_TIMER_STOP, timer_id);

#ifdef REG_PASS_SYS_TIMER_STOP
	REG_PASS_SYS_TIMER_STOP;
#endif
	return_to_user_prog(int);
}



extern inline int sys_timer_start_oneshot(sys_timer_t timer_id,
										  system_time_t expiration_time);

extern inline int sys_timer_start_oneshot(sys_timer_t timer_id,
										  system_time_t expiration_time)
{
	return _sys_timer_start(timer_id, expiration_time, 0);
}

extern inline int sys_timer_start_periodic(sys_timer_t timer_id,
										   usecond_t period);

extern inline int sys_timer_start_periodic(sys_timer_t timer_id,
										   usecond_t period)
{
	return _sys_timer_start(timer_id, 0, period);
}

extern inline int sys_timer_start_periodic_absolute(sys_timer_t timer_id,
													system_time_t exp_time,
													usecond_t period);

extern inline int sys_timer_start_periodic_absolute(sys_timer_t timer_id,
													system_time_t exp_time,
													usecond_t period)
{
	return _sys_timer_start(timer_id, exp_time, period);
}

extern inline int sys_timer_connect_event_queue(sys_timer_t timer_id,
												sys_event_queue_t queue_id,
												__CSTD uint64_t name,
												__CSTD uint64_t data1,
												__CSTD uint64_t data2);

extern inline int sys_timer_connect_event_queue(sys_timer_t timer_id,
												sys_event_queue_t queue_id,
												__CSTD uint64_t name,
												__CSTD uint64_t data1,
												__CSTD uint64_t data2)
{
	system_call_5(SYS_TIMER_CONNECT_EVENT_QUEUE, timer_id, queue_id, name,
				  data1, data2);

#ifdef REG_PASS_SYS_TIMER_CONNECT_EVENT_QUEUE
	REG_PASS_SYS_TIMER_CONNECT_EVENT_QUEUE;
#endif
	return_to_user_prog(int);
}


extern inline int sys_timer_disconnect_event_queue(sys_timer_t timer_id);

extern inline int sys_timer_disconnect_event_queue(sys_timer_t timer_id)
{
	system_call_1(SYS_TIMER_DISCONNECT_EVENT_QUEUE, timer_id);

#ifdef REG_PASS_SYS_TIMER_DISCONNECT_EVENT_QUEUE
	REG_PASS_SYS_TIMER_DISCONNECT_EVENT_QUEUE;
#endif
	return_to_user_prog(int);
}


CDECL_END

#endif /* __SYS_SYS_TIMER_H__ */
/*@}*/
