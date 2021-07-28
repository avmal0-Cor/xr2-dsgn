/*   SCE CONFIDENTIAL                                       */
/*   PlayStation(R)3 Programmer Tool Runtime Library 310.001 */
/*   Copyright (C) 2009 Sony Computer Entertainment Inc.    */
/*   All Rights Reserved.                                   */

#ifndef __LV2_KERNEL_PRX_H__
#define __LV2_KERNEL_PRX_H__

#include <sys/cdefs.h>
#include <sys/syscall.h>
#include <sys/types.h>

#include <sys/moduleexport.h>

#if defined(__cplusplus)
#define							CDECL_VOID
#else /* defined(__cplusplus) */
#define							CDECL_VOID    void
#endif /* defined(__cplusplus) */

CDECL_BEGIN

#define SYS_PRX_RESIDENT	0	/* result of start entry */
#define SYS_PRX_NO_RESIDENT	1

#define SYS_PRX_START_OK	SYS_PRX_RESIDENT

#define SYS_PRX_STOP_SUCCESS	0
#define SYS_PRX_STOP_OK		SYS_PRX_STOP_SUCCESS
#define SYS_PRX_STOP_FAILED	1

#define SYS_PRX_MODULE_FILENAME_SIZE		512

#define SYS_PRX_PROCESS_ELF_ID	0

typedef int32_t sys_prx_id_t;
typedef uint64_t sys_prx_flags_t;

/* segment information */
typedef struct {
	uint64_t base;				/* segment base */
	uint64_t filesz;			/* segment size */
	uint64_t memsz;				/* segment size */
	uint64_t index;				/* segment index */
	uint64_t type;				/* segment type == ELF type */
} sys_prx_segment_info_t;

/* this type is mode dependent */
typedef int (*sys_prx_entry_t) (size_t args, void *argv);

typedef int (*sys_prx_entry_pe_t) (uint64_t entry, size_t args, void *argv);

typedef struct sys_prx_start_option_t {
	uint64_t size;				/* sizeof(this) */
} sys_prx_start_option_t;

typedef struct sys_prx_stop_option_t {
	uint64_t size;				/* sizeof(this) */
} sys_prx_stop_option_t;

typedef struct sys_prx_load_module_option_t {
	uint64_t size;				/* sizeof(this) */
} sys_prx_load_module_option_t;

typedef struct sys_prx_load_module_list_option_t {
	uint64_t size;				/* sizeof(this) */
} sys_prx_load_module_list_option_t;

typedef struct sys_prx_start_module_option_t {
	uint64_t size;				/* sizeof(this) */
} sys_prx_start_module_option_t;

typedef struct sys_prx_stop_module_option_t {
	uint64_t size;				/* sizeof(this) */
} sys_prx_stop_module_option_t;

typedef struct sys_prx_unload_module_option_t {
	uint64_t size;				/* sizeof(this) */
} sys_prx_unload_module_option_t;

typedef struct sys_prx_register_module_option_t {
	uint64_t size;				/* sizeof(this) */
} sys_prx_register_module_option_t;

typedef struct sys_prx_get_module_id_by_name_option_t {
	uint64_t size;				/* sizeof(this) */
} sys_prx_get_module_id_by_name_option_t;

#ifdef	__LP64__
typedef uint32_t sys_prx_user_pchar_t;
typedef uint32_t sys_prx_user_segment_vector_t;
typedef uint32_t sys_prx_user_p_prx_id_t;
typedef uint32_t sys_prx_user_p_const_void_t;
typedef uint32_t sys_prx_user_p_stop_level_t;
#else
typedef char *sys_prx_user_pchar_t;
typedef sys_prx_segment_info_t *sys_prx_user_segment_vector_t;
typedef sys_prx_id_t *sys_prx_user_p_prx_id_t;
typedef const void *sys_prx_user_p_const_void_t;
typedef const void *sys_prx_user_p_stop_level_t;
#endif

typedef struct sys_prx_get_module_list_t {
	uint64_t size;				/* sizeof(this) *//* 8 */
	uint32_t max;
	uint32_t count;				/* 16 */
	sys_prx_user_p_prx_id_t idlist;
	sys_prx_user_p_stop_level_t levellist;	/* 24 */
} sys_prx_get_module_list_t;

typedef struct sys_prx_module_info_t {
	uint64_t size;
	char name[30];
	char version[2];
	uint32_t modattribute;
	uint32_t start_entry;
	uint32_t stop_entry;
	uint32_t all_segments_num;
	sys_prx_user_pchar_t filename;
	uint32_t filename_size;
	sys_prx_user_segment_vector_t segments;
	uint32_t segments_num;
} sys_prx_module_info_t;

/* PRX (shared) library interface */
extern int sys_prx_version;

#define SYS_PRX_LOAD_MODULE_FLAGS_VALIDMASK	0x0000000000000001
#define SYS_PRX_LOAD_MODULE_FLAGS_FIXEDADDR	0x0000000000000001	/* PRX is pre fixed */

sys_prx_id_t sys_prx_load_module(const char *path, uint64_t flags,
								 sys_prx_load_module_option_t * pOpt);
sys_prx_id_t sys_prx_load_module_on_memcontainer(const char *path,
												 sys_memory_container_t
												 mem_container, uint64_t flags,
												 sys_prx_load_module_option_t *
												 pOpt);
sys_prx_id_t sys_prx_load_module_by_fd(int fd, off64_t offset, uint64_t flags,
									   sys_prx_load_module_option_t * pOpt);
sys_prx_id_t sys_prx_load_module_on_memcontainer_by_fd(int fd, off64_t offset,
													   sys_memory_container_t
													   mem_container,
													   uint64_t flags,
													   sys_prx_load_module_option_t
													   * pOpt);

int sys_prx_load_module_list(int n, const char **path_list, uint64_t flags,
							 sys_prx_load_module_list_option_t * pOpt,
							 sys_prx_id_t * idlist);
int sys_prx_load_module_list_on_memcontainer(int n, const char **path_list,
											 sys_memory_container_t
											 mem_container, uint64_t flags,
											 sys_prx_load_module_list_option_t *
											 pOpt, sys_prx_id_t * idlist);

int sys_prx_start_module(sys_prx_id_t id, size_t args, void *argp,
						 int *modres, sys_prx_flags_t flags,
						 sys_prx_start_module_option_t * pOpt);
int sys_prx_stop_module(sys_prx_id_t id, size_t args, void *argp,
						int *modres, sys_prx_flags_t flags,
						sys_prx_stop_module_option_t * pOpt);
int sys_prx_unload_module(sys_prx_id_t id, sys_prx_flags_t flags,
						  const sys_prx_unload_module_option_t * pOpt);

int sys_prx_register_module(const sys_prx_register_module_option_t * pOpt);

int sys_prx_get_module_list(sys_prx_flags_t flags,
							sys_prx_get_module_list_t * pInfo);
int sys_prx_get_module_info(sys_prx_id_t id, sys_prx_flags_t,
							sys_prx_module_info_t * p_info);
sys_prx_id_t sys_prx_get_module_id_by_name(const char *name,
										   sys_prx_flags_t flags,
										   sys_prx_get_module_id_by_name_option_t
										   * pOpt);
sys_prx_id_t sys_prx_get_module_id_by_address(void *addr);
sys_prx_id_t sys_prx_get_my_module_id(	CDECL_VOID);
										 sys_addr_t
										 sys_prx_get_ppu_guid(sys_prx_id_t
															  modid);
										 int sys_prx_register_library(void
																	  *pLibEnt);
										 int sys_prx_unregister_library(void
																		*pLibEnt);
CDECL_END
#endif /* __LV2_KERNEL_PRX_H__ */
