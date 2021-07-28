/*   SCE CONFIDENTIAL                                       */
/*   PlayStation(R)3 Programmer Tool Runtime Library 310.001 */
/*   Copyright (C) 2007 Sony Computer Entertainment Inc.    */
/*   All Rights Reserved.                                   */

/**
 * \defgroup all_syscall System Calls
 */

/**
 * \ingroup all_syscall
 * \defgroup overlay Overlay Management
 * \brief system calls defined in sys/overlay.h
 */
/*@{*/

#ifndef __SYS_SYS_OVERLAY_H__
#define __SYS_SYS_OVERLAY_H__

#include <sys/cdefs.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <sys/integertypes.h>
#include <sys/return_code.h>
#include <sys/fs_external.h>
#include <sys/process.h>

CDECL_BEGIN


/* \brief Invalid overlay module ID */
#define SYS_OVERLAY_ID_INVALID			0xffffffffU

/* \brief Maximum number of segments in an overlay module */
#define SYS_OVERLAY_MAX_NUM_OF_SEGMENTS_IN_OVLM	4

/* \brief Invalid overlay module ID */
#define SYS_OVERLAY_SEGMENT_INFO_INDEX_INVALID	0xffffffffU

/* \brief Overlay module filename size (max) */
#define SYS_OVERLAY_MODULE_FILENAME_SIZE	CELL_FS_MAX_FS_PATH_LENGTH

/* \brief Overlay module filename structure (max 511 characters + NULL terminated) */
typedef char sys_overlay_module_filename_t[SYS_OVERLAY_MODULE_FILENAME_SIZE];

/* \brief Overlay module name size (max) */
#define SYS_OVERLAY_MODULE_NAME_SIZE		0x50U	/* 80 */

/* \brief Overlay module name structure (max 79 characters + NULL terminated) */
typedef char sys_overlay_module_name_t[SYS_OVERLAY_MODULE_NAME_SIZE];


/* \brief Segment information */
typedef struct sys_overlay_segment_info {
	__CSTD uint32_t base;		/* out : start address of a segment */
	__CSTD uint32_t memsz;		/* out : memory size of a segment */
	__CSTD uint32_t index;		/* out : segment index */
	__CSTD uint32_t reserved;	/* reserved for future use */
} sys_overlay_segment_info_t;


/* \brief Overlay module information */
typedef struct sys_overlay_module_info {
	__CSTD uint64_t size;		/* in : size of (this) */
	sys_overlay_module_filename_t *filename;	/* out : filename */
	__CSTD size_t filename_size;	/* out : length of a filename (not including '\0') */
	__CSTD size_t segments_num;	/* in : number of index in segments */
	sys_overlay_segment_info_t *segments;	/* out : an array of sys_overlay_segment_info_t objects */
} sys_overlay_module_info_t;

/* \brief Overlay module information (2) */
typedef struct sys_overlay_module_info2 {
	__CSTD uint64_t size;		/* in : size of (this) */
	sys_overlay_module_filename_t *filename;	/* out : filename */
	__CSTD size_t filename_size;	/* out : length of a filename (not including '\0') */
	__CSTD size_t segments_num;	/* in : number of index in segments */
	sys_overlay_segment_info_t *segments;	/* out : an array of sys_overlay_segment_info_t objects */
	sys_overlay_module_name_t *modulename;	/* out : module name */
	__CSTD size_t modulename_size;	/* out : length of a module name (not including '\0') */
} sys_overlay_module_info2_t;


#define SYS_OVERLAY_PARAM_VERSION_INVALID	0xffffffffU
#define SYS_OVERLAY_PARAM_VERSION_170_0		0x00017000U
#define SYS_OVERLAY_PARAM_MAGIC			0x4f564c4dU

/* \brief overlay module parameters */
typedef struct {
	uint32_t size;
	uint32_t magic;
	uint32_t version;
	uint32_t sdk_version;
	char module_name[SYS_OVERLAY_MODULE_NAME_SIZE];
} sys_overlay_param_t;

#ifdef CELL_SDK_VERSION
#define SYS_OVERLAY_PARAM_SDK_VERSION		CELL_SDK_VERSION
#else /* CELL_SDK_VERSION */
#define SYS_OVERLAY_PARAM_SDK_VERSION		SYS_PROCESS_PARAM_SDK_VERSION_UNKNOWN
#endif /* CELL_SDK_VERSION */

#define SYS_OVERLAY_PARAM( module_name ) \
	sys_overlay_param_t __sys_proc_param SYS_PROCESS_PARAM_SECTION = { \
		sizeof(sys_overlay_param_t),		\
                SYS_OVERLAY_PARAM_MAGIC,		\
                SYS_OVERLAY_PARAM_VERSION_170_0,	\
                SYS_OVERLAY_PARAM_SDK_VERSION,		\
		#module_name };

/** \brief Load an overlay module */
extern inline int sys_overlay_load_module(sys_overlay_t * ovlmid,
										  const char *path,
										  __CSTD uint64_t flags,
										  sys_addr_t * entry);

extern inline int sys_overlay_load_module(sys_overlay_t * ovlmid,
										  const char *path,
										  __CSTD uint64_t flags,
										  sys_addr_t * entry)
{
	system_call_4(SYS_OVERLAY_LOAD_MODULE, (uint32_t) ovlmid, (uint32_t) path,
				  flags, (uint32_t) entry);

#ifdef REG_PASS_SYS_OVERLAY_LOAD_MODULE
	REG_PASS_SYS_OVERLAY_LOAD_MODULE;
#endif
	return_to_user_prog(int);
}


/** \brief Load an overlay module by a file descriptor */
extern inline int sys_overlay_load_module_by_fd(sys_overlay_t * ovlmid, int fd,
												off64_t offset,
												__CSTD uint64_t flags,
												sys_addr_t * entry);

extern inline int sys_overlay_load_module_by_fd(sys_overlay_t * ovlmid, int fd,
												off64_t offset,
												__CSTD uint64_t flags,
												sys_addr_t * entry)
{
	system_call_5(SYS_OVERLAY_LOAD_MODULE_BY_FD, (uint32_t) ovlmid, fd, offset,
				  flags, (uint32_t) entry);

#ifdef REG_PASS_SYS_OVERLAY_LOAD_MODULE_BY_FD
	REG_PASS_SYS_OVERLAY_LOAD_MODULE_BY_FD;
#endif
	return_to_user_prog(int);
}


/** \brief Unload an overlay module */
extern inline int sys_overlay_unload_module(sys_overlay_t ovlmid);

extern inline int sys_overlay_unload_module(sys_overlay_t ovlmid)
{
	system_call_1(SYS_OVERLAY_UNLOAD_MODULE, ovlmid);

#ifdef REG_PASS_SYS_OVERLAY_UNLOAD_MODULE
	REG_PASS_SYS_OVERLAY_UNLOAD_MODULE;
#endif
	return_to_user_prog(int);
}


/** \brief Get an overlay module ID list in a process */
extern inline int sys_overlay_get_module_list(sys_pid_t pid,
											  __CSTD size_t ovlmids_num,
											  sys_overlay_t * ovlmids,
											  __CSTD size_t * num_of_modules);

extern inline int sys_overlay_get_module_list(sys_pid_t pid,
											  __CSTD size_t ovlmids_num,
											  sys_overlay_t * ovlmids,
											  __CSTD size_t * num_of_modules)
{
	system_call_4(SYS_OVERLAY_GET_MODULE_LIST, pid, ovlmids_num,
				  (uint32_t) ovlmids, (uint32_t) num_of_modules);

#ifdef REG_PASS_SYS_OVERLAY_GET_MODULE_LIST
	REG_PASS_SYS_OVERLAY_GET_MODULE_LIST;
#endif
	return_to_user_prog(int);
}


/** \brief Get an overlay module information in a process */
extern inline int sys_overlay_get_module_info(sys_pid_t pid,
											  sys_overlay_t ovlmid,
											  sys_overlay_module_info_t * info);

extern inline int sys_overlay_get_module_info(sys_pid_t pid,
											  sys_overlay_t ovlmid,
											  sys_overlay_module_info_t * info)
{
	system_call_3(SYS_OVERLAY_GET_MODULE_INFO, pid, ovlmid, (uint32_t) info);

#ifdef REG_PASS_SYS_OVERLAY_GET_MODULE_INFO
	REG_PASS_SYS_OVERLAY_GET_MODULE_INFO;
#endif
	return_to_user_prog(int);
}


/** \brief Get an overlay module information in a process (2) */
extern inline int sys_overlay_get_module_info2(sys_pid_t pid,
											   sys_overlay_t ovlmid,
											   sys_overlay_module_info2_t *
											   info);

extern inline int sys_overlay_get_module_info2(sys_pid_t pid,
											   sys_overlay_t ovlmid,
											   sys_overlay_module_info2_t *
											   info)
{
	system_call_3(SYS_OVERLAY_GET_MODULE_INFO2, pid, ovlmid, (uint32_t) info);

#ifdef REG_PASS_SYS_OVERLAY_GET_MODULE_INFO2
	REG_PASS_SYS_OVERLAY_GET_MODULE_INFO2;
#endif
	return_to_user_prog(int);
}




CDECL_END


#endif /* __SYS_SYS_OVERLAY_H__ */
/*@}*/
