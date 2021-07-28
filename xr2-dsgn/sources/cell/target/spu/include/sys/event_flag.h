/*   SCE CONFIDENTIAL                                       */
/*   PlayStation(R)3 Programmer Tool Runtime Library 310.001 */
/*   Copyright (C) 2008 Sony Computer Entertainment Inc.    */
/*   All Rights Reserved.                                   */

/*@{*/
#ifndef __SYS_SYS_EVENT_FLAG_H__
#define __SYS_SYS_EVENT_FLAG_H__

#include <sys/cdefs.h>
#include <sys/types.h>
#include <sys/return_code.h>

CDECL_BEGIN

extern int sys_event_flag_set_bit(sys_event_flag_t ef, uint8_t bitn);
extern int sys_event_flag_set_bit_impatient(sys_event_flag_t ef, uint8_t bitn);

CDECL_END

#endif /* __SYS_SYS_EVENT_FLAG_H__ */
/*@}*/
