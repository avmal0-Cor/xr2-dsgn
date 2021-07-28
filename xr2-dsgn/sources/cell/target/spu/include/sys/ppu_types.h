/*   SCE CONFIDENTIAL                                       */
/*   PlayStation(R)3 Programmer Tool Runtime Library 310.001 */
/*   Copyright (C) 2009 Sony Computer Entertainment Inc.    */
/*   All Rights Reserved.                                   */
#ifndef __SYS_SYS_PPU_TYPES_H__
#define __SYS_SYS_PPU_TYPES_H__

#include <sys/sys_types.h>


typedef struct {
	uint32_t val_0;
	uint32_t val_1;
} sys_memory_info_t;

typedef struct {
	uint64_t val_0;
	uint64_t val_1;
	uint64_t val_2;
} sys_page_attr_t;

typedef struct {
	uint64_t val_0;
	uint64_t val_1;
	uint64_t val_2;
	uint64_t val_3;
	uint64_t val_4;
	uint64_t val_5;
} sys_vm_statistics_t;

typedef struct {
	uint32_t val_0;
	uint32_t val_1;
} sys_ppu_thread_stack_t;

typedef struct {
	uint64_t val_0;
	uint64_t val_1;
	uint64_t val_2;
} sys_spu_segment_t;

typedef struct {
	uint32_t val_0;
	uint32_t val_1;
	uint32_t val_2;
	uint32_t val_3;
} sys_spu_image_t;

typedef struct {
	uint64_t val_0;
	uint64_t val_1;
	uint64_t val_2;
	uint64_t val_3;
} sys_spu_thread_argument_t;

typedef struct {
	uint32_t val_0;
	uint32_t val_1;
	uint32_t val_2;
} sys_spu_thread_attribute_t;

typedef struct {
	uint32_t val_0;
	uint32_t val_1;
	uint32_t val_2;
	uint32_t val_3;
} sys_spu_thread_group_attribute_t;

typedef struct {
	uint64_t val_0;
	uint64_t val_1;
	uint64_t val_2;
	uint64_t val_3;
	uint64_t val_4;
} sys_mutex_attribute_t;

typedef struct {
	uint64_t val_0;
	uint64_t val_1;
	uint64_t val_2;
} sys_cond_attribute_t;

typedef struct {
	uint64_t val_0;
	uint64_t val_1;
	uint64_t val_2;
	uint64_t val_3;
} sys_rwlock_attribute_t;

typedef struct {
	uint32_t val_0;
	uint32_t val_1;
	uint32_t val_2;
	uint32_t val_3;
} sys_lwmutex_attribute_t;

typedef struct {
	uint8_t val_0;
	uint8_t val_1;
	uint8_t val_2;
	uint8_t val_3;
	uint8_t val_4;
	uint8_t val_5;
	uint8_t val_6;
	uint8_t val_7;
} sys_lwcond_attribute_t;

typedef struct {
	uint64_t val_0;
	uint64_t val_1;
	uint64_t val_2;
	uint64_t val_3;
} sys_semaphore_attribute_t;

typedef struct {
	uint64_t val_0;
	uint64_t val_1;
	uint64_t val_2;
	uint64_t val_3;
} sys_event_flag_attribute_t;

typedef struct {
	uint32_t val_0;
	uint32_t val_1;
	uint32_t val_2;
	uint32_t val_3;
} sys_event_queue_attribute_t;

typedef struct {
	uint64_t val_0;
	uint64_t val_1;
	uint64_t val_2;
	uint64_t val_3;
} sys_event_t;

typedef struct {
	uint64_t val_0;
	uint64_t val_1;
	uint64_t val_2;
} sys_timer_information_t;

typedef struct {
	uint64_t val_0;
	uint64_t val_1;
	uint64_t val_2;
} sys_lwmutex_t;

typedef struct {
	uint32_t val_0;
	uint32_t val_1;
} sys_lwcond_t;

typedef struct {
	uint64_t val_0;
} sys_prx_start_option_t;

typedef struct {
	uint64_t val_0;
} sys_prx_stop_option_t;

typedef struct {
	uint64_t val_0;
} sys_prx_load_module_option_t;

typedef struct {
	uint64_t val_0;
} sys_prx_load_module_list_option_t;

typedef struct {
	uint64_t val_0;
} sys_prx_start_module_option_t;

typedef struct {
	uint64_t val_0;
} sys_prx_stop_module_option_t;

typedef struct {
	uint64_t val_0;
} sys_prx_unload_module_option_t;

typedef struct {
	uint64_t val_0;
} sys_prx_register_module_option_t;


#endif /* __SYS_SYS_PPU_TYPES_H__ */
