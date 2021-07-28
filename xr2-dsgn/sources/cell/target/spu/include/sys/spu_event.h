/*   SCE CONFIDENTIAL                                       */
/*   PlayStation(R)3 Programmer Tool Runtime Library 310.001 */
/*   Copyright (C) 2005 Sony Computer Entertainment Inc.    */
/*   All Rights Reserved.                                   */
/**
 * \ingroup spu_all_spulib
 * \defgroup spulib_event SPU Thread Event Functions
 * \brief Library functions defined in spu/spu_event.h
 *
 */

/*@{*/
#ifndef __SYS_SYS_SPU_EVENT_H__
#define __SYS_SYS_SPU_EVENT_H__

#include <sys/cdefs.h>
#include <sys/types.h>
#include <sys/return_code.h>

CDECL_BEGIN

#define EVENT_DATA0_MASK 0x00FFFFFFU
#define EVENT_PORT_SHIFT 24
#define EVENT_PORT_MAX_NUM 63

#define EVENT_PRINTF_PORT 1U

extern int sys_spu_thread_send_event(uint8_t spup, uint32_t data0,
									 uint32_t data1);
extern int sys_spu_thread_throw_event(uint8_t spup, uint32_t data0,
									  uint32_t data1);
extern int sys_spu_thread_receive_event(uint32_t spuq,
										uint32_t * d1, uint32_t * d2,
										uint32_t * d3);
extern int sys_spu_thread_tryreceive_event(uint32_t spuq,
										   uint32_t * d1, uint32_t * d2,
										   uint32_t * d3);

CDECL_END

#endif /* __SYS_SYS_SPU_EVENT_H__ */
/*@}*/
