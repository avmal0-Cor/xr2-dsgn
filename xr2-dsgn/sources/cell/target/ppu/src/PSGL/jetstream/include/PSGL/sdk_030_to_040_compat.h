#ifndef __SDK_030_TO_040_COMPAT__
#define __SDK_030_TO_040_COMPAT__

#ifdef OS_VERSION_0_3_0

#include <sys/sys_time.h>

#define sys_pid_t pid_t
#define sys_process_spawn proc_spawn
#define sys_raw_spu_t raw_spu_t
#define sys_raw_spu_attribute_t raw_spu_attr_t
#define sys_raw_spu_create raw_spu_create
#define sys_raw_spu_destroy raw_spu_destroy
#define sys_spu_initialize spu_initialize
#define sys_ppu_thread_t pu_thread_t
#define sys_mutex_t mutex_t
#define sys_mutex_attribute_t mutex_attr_t
#define sys_cond_t cond_t
#define sys_cond_attribute_t cond_attr_t
#define sys_ppu_thread_get_id pu_thread_get_id
#define sys_ppu_thread_exit pu_thread_exit 
#define sys_ppu_thread_get_priority pu_thread_get_priority
#define sys_ppu_thread_create(tid,func,arg,prio,stack,flags,name) pu_thread_create(tid,prio,func,arg,0,stack,flags)
#define sys_ppu_thread_join pu_thread_join
#define sys_ppu_thread_yield pu_thread_yield
#define sys_mutex_attribute_initialize mutex_init
#define sys_mutex_create mutex_create
#define sys_mutex_destroy mutex_destroy
#define sys_mutex_lock mutex_lock
#define sys_mutex_unlock mutex_unlock
#define sys_mutex_trylock mutex_trylock
#define sys_cond_attribute_initialize cond_init
#define sys_cond_create cond_create
#define sys_cond_destroy cond_destroy
#define sys_cond_signal cond_signal
#define sys_cond_signal_all cond_signal_all
#define sys_cond_wait cond_wait

#define sys_pseudo_fs_read pseudo_fs_read
#define sys_pseudo_fs_write pseudo_fs_write
#define sys_pseudo_fs_append pseudo_fs_append

static inline system_time_t sys_time_get_system_time(void)
{
	system_time_t t;
	system_time_get(&t);
	return t;
}

/* Statically defined base addresses and offsets */
#define  RAW_SPU_OFFSET       0x0000000000100000ULL
#define  RAW_SPU_BASE_ADDR    0x00000000E0000000ULL
#define  RAW_SPU_LS_OFFSET    0x0000000000000000ULL
#define  RAW_SPU_PROB_OFFSET  0x0000000000040000ULL

#endif

#endif
