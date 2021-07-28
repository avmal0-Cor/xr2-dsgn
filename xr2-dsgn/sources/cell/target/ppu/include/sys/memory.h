/*   SCE CONFIDENTIAL                                       */
/*   PlayStation(R)3 Programmer Tool Runtime Library 310.001 */
/*   Copyright (C) 2008 Sony Computer Entertainment Inc.    */
/*   All Rights Reserved.                                   */

/**
 * \ingroup all_syscall
 * \defgroup memory Process Address Space Management 
 * \brief system calls defined in sys/memory.h
 *
 * Interfaces defined in sys/memory.h
 *
 * Refer to <i>Cell OS Lv-2 User's Manual</i> for the detailed description.
 *
 */
/*@{*/


#ifndef __SYS_SYS_MEMORY_H__
#define __SYS_SYS_MEMORY_H__

#include <sys/cdefs.h>
#include <sys/syscall.h>


#include <sys/integertypes.h>
#include <sys/return_code.h>
#include <sys/types.h>


CDECL_BEGIN

/***
 *** type definition
 ***/

typedef struct sys_page_attr {
	__CSTD uint64_t attribute;
	__CSTD uint64_t access_right;
	__CSTD size_t page_size;
	__CSTD uint32_t pad;
} sys_page_attr_t;


typedef struct sys_memory_info {
	__CSTD size_t total_user_memory;
	__CSTD size_t available_user_memory;
} sys_memory_info_t;





/***
 *** macro definition
 ***/
/* page size flags */
#define SYS_MEMORY_PAGE_SIZE_1M          0x0000000000000400ULL
#define SYS_MEMORY_PAGE_SIZE_64K         0x0000000000000200ULL
#define SYS_MEMORY_PAGE_SIZE_MASK        0x0000000000000f00ULL

/* access right flags */
#define SYS_MEMORY_ACCESS_RIGHT_PPU_THR  0x0000000000000008ULL
#define SYS_MEMORY_ACCESS_RIGHT_HANDLER  0x0000000000000004ULL
#define SYS_MEMORY_ACCESS_RIGHT_SPU_THR  0x0000000000000002ULL
#define SYS_MEMORY_ACCESS_RIGHT_RAW_SPU  0x0000000000000001ULL
#define SYS_MEMORY_ACCESS_RIGHT_ANY      (SYS_MEMORY_ACCESS_RIGHT_PPU_THR | \
                                          SYS_MEMORY_ACCESS_RIGHT_HANDLER | \
                                          SYS_MEMORY_ACCESS_RIGHT_SPU_THR | \
                                          SYS_MEMORY_ACCESS_RIGHT_RAW_SPU)
#define SYS_MEMORY_ACCESS_RIGHT_NONE     0x00000000000000f0ULL
#define SYS_MEMORY_ACCESS_RIGHT_MASK     0x00000000000000ffULL

#define SYS_MEMORY_PAGE_FAULT_NOTIFICATION_SPU_ALLOW \
                                         0x0000000000010000ULL

/* user handling page fault flags */
#define SYS_MEMORY_PAGE_FAULT_EVENT_KEY  0xfffe000000000000ULL
#define SYS_MEMORY_PAGE_FAULT_CAUSE_NON_MAPPED  0x00000002U
#define SYS_MEMORY_PAGE_FAULT_CAUSE_READ_ONLY   0x00000001U
#define SYS_MEMORY_PAGE_FAULT_TYPE_PPU_THREAD   0x00000000U
#define SYS_MEMORY_PAGE_FAULT_TYPE_SPU_THREAD   0x00000001U
#define SYS_MEMORY_PAGE_FAULT_TYPE_RAW_SPU      0x00000002U

/* accessor for user handling page fault event */
#define SYS_MEMORY_PAGE_FAULT_GET_ADDRESS(ev)	(ev.data1)
#define SYS_MEMORY_PAGE_FAULT_GET_TARGET_ID(ev)	(ev.data2 & 0xffffffffU)
#define SYS_MEMORY_PAGE_FAULT_GET_CAUSE(ev)	(ev.data3 & 0x3U)
#define SYS_MEMORY_PAGE_FAULT_GET_TYPE(ev)	(ev.data2 >> 32)


/* memory granularity flags */
#define SYS_MEMORY_GRANULARITY_1M        0x0000000000000400ULL
#define SYS_MEMORY_GRANULARITY_64K       0x0000000000000200ULL
#define SYS_MEMORY_GRANULARITY_MASK      0x0000000000000f00ULL

/* mapping attribute */
#define SYS_MEMORY_PROT_READ_ONLY        0x0000000000080000ULL
#define SYS_MEMORY_PROT_READ_WRITE       0x0000000000040000ULL
#define SYS_MEMORY_PROT_MASK             0x00000000000f0000ULL

/* invalid memory ID */
#define SYS_MEMORY_ID_INVALID            0xFFFFFFFFU

#define SYS_MEMORY_CONTAINER_DELETE_CRAN_FORCE  0x00000001U
#define SYS_MEMORY_CONTAINER_ID_INVALID         0xFFFFFFFFU

/***
 *** API declaration
 ***/
extern inline int sys_mmapper_allocate_address(__CSTD size_t size,
											   __CSTD uint64_t flags,
											   __CSTD size_t alignment,
											   sys_addr_t * alloc_addr);

extern inline int sys_mmapper_allocate_address(__CSTD size_t size,
											   __CSTD uint64_t flags,
											   __CSTD size_t alignment,
											   sys_addr_t * alloc_addr)
{
	system_call_4(SYS_MMAPPER_ALLOCATE_ADDRESS, size, flags, alignment,
				  (uint32_t) alloc_addr);

#ifdef REG_PASS_SYS_MMAPPER_ALLOCATE_ADDRESS
	REG_PASS_SYS_MMAPPER_ALLOCATE_ADDRESS;
#endif
	return_to_user_prog(int);
}


extern inline int sys_mmapper_allocate_fixed_address(void);

extern inline int sys_mmapper_allocate_fixed_address(void)
{
	system_call_0(SYS_MMAPPER_ALLOCATE_FIXED_ADDRESS);

#ifdef REG_PASS_SYS_MMAPPER_ALLOCATE_FIXED_ADDRESS
	REG_PASS_SYS_MMAPPER_ALLOCATE_FIXED_ADDRESS;
#endif
	return_to_user_prog(int);
}


extern inline int sys_mmapper_free_address(sys_addr_t start_addr);

extern inline int sys_mmapper_free_address(sys_addr_t start_addr)
{
	system_call_1(SYS_MMAPPER_FREE_ADDRESS, start_addr);

#ifdef REG_PASS_SYS_MMAPPER_FREE_ADDRESS
	REG_PASS_SYS_MMAPPER_FREE_ADDRESS;
#endif
	return_to_user_prog(int);
}



extern inline int sys_mmapper_change_address_access_right(sys_addr_t start_addr,
														  __CSTD uint64_t
														  flags);

extern inline int sys_mmapper_change_address_access_right(sys_addr_t start_addr,
														  __CSTD uint64_t flags)
{
	system_call_2(SYS_MMAPPER_CHANGE_ADDRESS_ACCESS_RIGHT, start_addr, flags);

#ifdef REG_PASS_SYS_MMAPPER_CHANGE_ADDRESS_ACCESS_RIGHT
	REG_PASS_SYS_MMAPPER_CHANGE_ADDRESS_ACCESS_RIGHT;
#endif
	return_to_user_prog(int);
}


extern inline int sys_mmapper_search_and_map(sys_addr_t start_addr,
											 sys_memory_t mem_id,
											 __CSTD uint64_t flags,
											 sys_addr_t * alloc_addr);

extern inline int sys_mmapper_search_and_map(sys_addr_t start_addr,
											 sys_memory_t mem_id,
											 __CSTD uint64_t flags,
											 sys_addr_t * alloc_addr)
{
	system_call_4(SYS_MMAPPER_SEARCH_AND_MAP, start_addr, mem_id, flags,
				  (uint32_t) alloc_addr);

#ifdef REG_PASS_SYS_MMAPPER_SEARCH_AND_MAP
	REG_PASS_SYS_MMAPPER_SEARCH_AND_MAP;
#endif
	return_to_user_prog(int);
}


extern int sys_mmapper_allocate_memory(__CSTD size_t size,
									   __CSTD uint64_t flags,
									   sys_memory_t * mem_id);

extern int sys_mmapper_allocate_memory_from_container(size_t size,
													  sys_memory_container_t
													  container, uint64_t flags,
													  sys_memory_t * mem_id);


extern int sys_mmapper_free_memory(sys_memory_t mem_id);

extern int sys_mmapper_map_memory(sys_addr_t start_addr,
								  sys_memory_t mem_id, __CSTD uint64_t flags);

extern int sys_mmapper_unmap_memory(sys_addr_t start_addr,
									sys_memory_t * mem_id);

extern inline int sys_memory_get_page_attribute(sys_addr_t addr,
												sys_page_attr_t * attr);

extern inline int sys_memory_get_page_attribute(sys_addr_t addr,
												sys_page_attr_t * attr)
{
	system_call_2(SYS_MEMORY_GET_PAGE_ATTRIBUTE, addr, (uint32_t) attr);

#ifdef REG_PASS_SYS_MEMORY_GET_PAGE_ATTRIBUTE
	REG_PASS_SYS_MEMORY_GET_PAGE_ATTRIBUTE;
#endif
	return_to_user_prog(int);
}


extern inline int sys_memory_get_user_memory_size(sys_memory_info_t * mem_info);

extern inline int sys_memory_get_user_memory_size(sys_memory_info_t * mem_info)
{
	system_call_1(SYS_MEMORY_GET_USER_MEMORY_SIZE, (uint32_t) mem_info);

#ifdef REG_PASS_SYS_MEMORY_GET_USER_MEMORY_SIZE
	REG_PASS_SYS_MEMORY_GET_USER_MEMORY_SIZE;
#endif
	return_to_user_prog(int);
}


extern inline int sys_memory_allocate(__CSTD size_t size, __CSTD uint64_t flags,
									  sys_addr_t * alloc_addr);

extern inline int sys_memory_allocate(__CSTD size_t size, __CSTD uint64_t flags,
									  sys_addr_t * alloc_addr)
{
	system_call_3(SYS_MEMORY_ALLOCATE, size, flags, (uint32_t) alloc_addr);

#ifdef REG_PASS_SYS_MEMORY_ALLOCATE
	REG_PASS_SYS_MEMORY_ALLOCATE;
#endif
	return_to_user_prog(int);
}

extern inline int sys_memory_allocate_from_container(__CSTD size_t size,
													 sys_memory_container_t
													 container,
													 __CSTD uint64_t flags,
													 sys_addr_t * alloc_addr);

extern inline int sys_memory_allocate_from_container(__CSTD size_t size,
													 sys_memory_container_t
													 container,
													 __CSTD uint64_t flags,
													 sys_addr_t * alloc_addr)
{
	system_call_4(SYS_MEMORY_ALLOCATE_FROM_CONTAINER, size, container, flags,
				  (uint32_t) alloc_addr);

#ifdef REG_PASS_SYS_MEMORY_ALLOCATE_FROM_CONTAINER
	REG_PASS_SYS_MEMORY_ALLOCATE_FROM_CONTAINER;
#endif
	return_to_user_prog(int);
}


extern inline int sys_memory_free(sys_addr_t start_addr);

extern inline int sys_memory_free(sys_addr_t start_addr)
{
	system_call_1(SYS_MEMORY_FREE, start_addr);

#ifdef REG_PASS_SYS_MEMORY_FREE
	REG_PASS_SYS_MEMORY_FREE;
#endif
	return_to_user_prog(int);
}



extern inline int sys_memory_container_create(sys_memory_container_t * cid,
											  __CSTD size_t yield_size);

extern inline int sys_memory_container_create(sys_memory_container_t * cid,
											  __CSTD size_t yield_size)
{
	system_call_2(SYS_MEMORY_CONTAINER_CREATE, (uint32_t) cid, yield_size);

#ifdef REG_PASS_SYS_MEMORY_CONTAINER_CREATE
	REG_PASS_SYS_MEMORY_CONTAINER_CREATE;
#endif
	return_to_user_prog(int);
}

extern inline int sys_memory_container_destroy(sys_memory_container_t cid);

extern inline int sys_memory_container_destroy(sys_memory_container_t cid)
{
	system_call_1(SYS_MEMORY_CONTAINER_DESTROY, cid);

#ifdef REG_PASS_SYS_MEMORY_CONTAINER_DESTROY
	REG_PASS_SYS_MEMORY_CONTAINER_DESTROY;
#endif
	return_to_user_prog(int);
}



extern inline int sys_memory_container_get_size(sys_memory_info_t * mem_info,
												sys_memory_container_t cid);

extern inline int sys_memory_container_get_size(sys_memory_info_t * mem_info,
												sys_memory_container_t cid)
{
	system_call_2(SYS_MEMORY_CONTAINER_GET_SIZE, (uint32_t) mem_info, cid);

#ifdef REG_PASS_SYS_MEMORY_CONTAINER_GET_SIZE
	REG_PASS_SYS_MEMORY_CONTAINER_GET_SIZE;
#endif
	return_to_user_prog(int);
}
















extern inline int sys_mmapper_enable_page_fault_notification(sys_addr_t
															 start_addr,
															 sys_event_queue_t
															 queue_id);

extern inline int sys_mmapper_enable_page_fault_notification(sys_addr_t
															 start_addr,
															 sys_event_queue_t
															 queue_id)
{
	system_call_2(SYS_MMAPPER_ENABLE_PAGE_FAULT_NOTIFICATION, start_addr,
				  queue_id);

#ifdef REG_PASS_SYS_MMAPPER_ENABLE_PAGE_FAULT_NOTIFICATION
	REG_PASS_SYS_MMAPPER_ENABLE_PAGE_FAULT_NOTIFICATION;
#endif
	return_to_user_prog(int);
}




CDECL_END

#endif /* __SYS_SYS_MEMORY_H__ */
/*@}*/
