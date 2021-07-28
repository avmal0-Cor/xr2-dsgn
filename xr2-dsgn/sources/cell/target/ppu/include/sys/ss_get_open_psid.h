/*   SCE CONFIDENTIAL                                       */
/*   PlayStation(R)3 Programmer Tool Runtime Library 310.001 */
/*   Copyright (C) 2006 Sony Computer Entertainment Inc.    */
/*   All Rights Reserved.                                   */

#ifndef __LV2_SS_APPLIANCE_INFO_MANAGER_SYSCALL_SS_GET_OPEN_PSID_H__
#define __LV2_SS_APPLIANCE_INFO_MANAGER_SYSCALL_SS_GET_OPEN_PSID_H__

#include <sys/cdefs.h>
#include <sys/syscall.h>
#include <sys/integertypes.h>
#include <sys/return_code.h>
#include <sys/types.h>

#include <stdio.h>
#include <errno.h>
#include <time.h>

CDECL_BEGIN

typedef struct CellSsOpenPSID {
	uint64_t high;
	uint64_t low;
} CellSsOpenPSID;

extern inline int sys_ss_get_open_psid(CellSsOpenPSID * ptr);

extern inline int sys_ss_get_open_psid(CellSsOpenPSID * ptr)
{
	system_call_1(SYS_SS_GET_OPEN_PSID, (uint32_t) ptr);

#ifdef REG_PASS_SYS_SS_GET_OPEN_PSID
	REG_PASS_SYS_SS_GET_OPEN_PSID;
#endif
	return_to_user_prog(int);
}


CDECL_END

#endif /* __LV2_SS_APPLIANCE_INFO_MANAGER_SYSCALL_SS_GET_OPEN_PSID_H__ */
