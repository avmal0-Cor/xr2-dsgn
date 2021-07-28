/*   SCE CONFIDENTIAL                                       */
/*   PlayStation(R)3 Programmer Tool Runtime Library 310.001 */
/*   Copyright (C) 2006 Sony Computer Entertainment Inc.    */
/*   All Rights Reserved.                                   */

/**
 * \ingroup all_syscall
 * \defgroup sys_time System Time
 * \brief system calls defined in sys/sys_time.h
 *  
 * Interfaces defined in sys/sys_time.h
 *
 * Refer to <i>Cell OS Lv-2 User's Manual</i> for the detailed description. 
 */
/*@{*/

#ifndef __SYS_SYS_SYS_TIME_H__
#define __SYS_SYS_SYS_TIME_H__

#include <sys/cdefs.h>
#include <sys/syscall.h>

#include <sys/integertypes.h>
#include <sys/return_code.h>
#include <sys/types.h>

typedef __CSTD int64_t sys_time_sec_t;
typedef __CSTD int64_t sys_time_nsec_t;
typedef __CSTD int32_t sys_time_min_t;
typedef sys_time_min_t sys_time_timezone_t;
typedef sys_time_min_t sys_time_summertime_t;


CDECL_BEGIN

extern system_time_t sys_time_get_system_time(void);
extern inline __CSTD uint64_t sys_time_get_timebase_frequency(void);

extern inline __CSTD uint64_t sys_time_get_timebase_frequency(void)
{
	system_call_0(SYS_TIME_GET_TIMEBASE_FREQUENCY);

#ifdef REG_PASS_SYS_TIME_GET_TIMEBASE_FREQUENCY
	REG_PASS_SYS_TIME_GET_TIMEBASE_FREQUENCY;
#endif
	return_to_user_prog(__CSTD uint64_t);
}

extern inline int sys_time_get_current_time(sys_time_sec_t * sec,
											sys_time_nsec_t * nsec);

extern inline int sys_time_get_current_time(sys_time_sec_t * sec,
											sys_time_nsec_t * nsec)
{
	system_call_2(SYS_TIME_GET_CURRENT_TIME, (uint32_t) sec, (uint32_t) nsec);

#ifdef REG_PASS_SYS_TIME_GET_CURRENT_TIME
	REG_PASS_SYS_TIME_GET_CURRENT_TIME;
#endif
	return_to_user_prog(int);
}



CDECL_END

#endif /* __SYS_SYS_SYS_TIME_H__ */
/*@}*/
