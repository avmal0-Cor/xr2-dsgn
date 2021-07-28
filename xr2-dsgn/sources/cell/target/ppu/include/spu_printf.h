/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2005 Sony Computer Entertainment Inc. 
* All Rights Reserved.
*/
#ifndef __SPU_PRINTF_H__
#define __SPU_PRINTF_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <yvals.h>
#include <sys/spu_thread.h>
#include <sys/spu_thread_group.h>
  
/* PU helper functions for processing printf syscalls from SPU */
extern int spu_thread_sprintf (char *buf, sys_spu_thread_t id, _CSTD uint32_t arg_addr);
extern int spu_thread_snprintf (char *buf, size_t size, sys_spu_thread_t id, _CSTD uint32_t arg_addr);
extern int spu_raw_sprintf (char *buf, _CSTD uint64_t ls_base, _CSTD uint32_t arg_addr);
extern int spu_raw_snprintf (char *buf, size_t size, _CSTD uint64_t ls_base, _CSTD uint32_t arg_addr);
extern int spu_thread_printf (sys_spu_thread_t id, _CSTD uint32_t arg_addr);
extern int raw_spu_printf (_CSTD uint64_t ls_base, _CSTD uint32_t arg_addr);

/* SPU printf server */
extern int spu_printf_initialize(int prio, void (func)(const char *));
extern int spu_printf_attach_group(sys_spu_thread_group_t group);
extern int spu_printf_detach_group(sys_spu_thread_group_t group);
extern int spu_printf_attach_thread(sys_spu_thread_t thread);
extern int spu_printf_detach_thread(sys_spu_thread_t thread);
extern int spu_printf_finalize();

#ifdef __cplusplus
}
#endif

#endif /* __SPU_PRINTF_H__ */
