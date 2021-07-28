/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2009 Sony Computer Entertainment Inc. 
* All Rights Reserved.
*/
#ifndef __SPU_PRINTF_H__
#define __SPU_PRINTF_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <sys/spu_event.h>

extern int _spu_call_event_va_arg(uint32_t _spup, const char *fmt, ...);

/* The call is the same for raw SPUs and thread controlled SPUs. */
#define spu_printf(...) \
	_spu_call_event_va_arg(EVENT_PRINTF_PORT<<EVENT_PORT_SHIFT, __VA_ARGS__)

#ifdef __cplusplus
}
#endif

#endif /* __SPU_PRINTF_H__ */
