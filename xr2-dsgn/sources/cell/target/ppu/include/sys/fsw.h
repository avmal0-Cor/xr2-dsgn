/*
 *   SCE CONFIDENTIAL                                      
 *   PlayStation(R)3 Programmer Tool Runtime Library 310.001
 *   Copyright (C) 2007 Sony Computer Entertainment Inc.   
 *   All Rights Reserved.                                  
 */

#ifndef __LV2_IOSYS_SYSCALL_FSW_H__
#define __LV2_IOSYS_SYSCALL_FSW_H__

#include <sys/cdefs.h>
#include <sys/syscall.h>
#include <sys/types.h>

#define SYS_FSW_EVENT_KEY 0xFFFFFFFF46535701ULL

CDECL_BEGIN

extern inline int sys_fsw_connect_event(sys_event_queue_t eq_id);

extern inline int sys_fsw_connect_event(sys_event_queue_t eq_id)
{
	system_call_1(SYS_FSW_CONNECT_EVENT, eq_id);

#ifdef REG_PASS_SYS_FSW_CONNECT_EVENT
	REG_PASS_SYS_FSW_CONNECT_EVENT;
#endif
	return_to_user_prog(int);
}

extern inline int sys_fsw_disconnect_event();

extern inline int sys_fsw_disconnect_event()
{
	system_call_0(SYS_FSW_DISCONNECT_EVENT);

#ifdef REG_PASS_SYS_FSW_DISCONNECT_EVENT
	REG_PASS_SYS_FSW_DISCONNECT_EVENT;
#endif
	return_to_user_prog(int);
}


CDECL_END

#endif // __LV2_IOSYS_SYSCALL_FSW_H__
