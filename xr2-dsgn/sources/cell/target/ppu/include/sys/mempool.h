/*   SCE CONFIDENTIAL                                       */
/*   PlayStation(R)3 Programmer Tool Runtime Library 310.001 */
/*   Copyright (C) 2005 Sony Computer Entertainment Inc.    */
/*   All Rights Reserved.                                   */
/**
 * \ingroup lib_all_library
 * \defgroup lib_mempool Memory Pool Library Functions
 * \brief Library defined in mempool.h
 *
 * Interfaces defined in mempool.h
 *
 * Refer to Cell OS Lv-2 User's Manual for the detailed description. 
 */
/*@{*/

/*
 * figure: chunk and memory blocks
 *
 *      chunk -> +----------------------------+ +
 *               |  mempool_header structure  | |
 *               +----------------------------+ |
 *               |      a memory block        | |
 *               +----------------------------+ | chunk size
 *               |      a memory block        | |
 *               +----------------------------+ |
 *  block_size | |      a memory block        | |
 *               +----------------------------+ +
 */

#ifndef __SYS_SYS_MEMPOOL_H__
#define __SYS_SYS_MEMPOOL_H__

#include <sys/cdefs.h>
#include <sys/synchronization.h>

CDECL_BEGIN

typedef struct {
	__CSTD uint64_t magic_number;
	__CSTD uint64_t free_count;
	__CSTD size_t chunk_size;
	sys_addr_t first_free_block;
	__CSTD size_t block_size;
	__CSTD size_t offset;
	sys_mutex_t mutex_id;
	sys_cond_t cond_id;
} mempool_header;

/**
 * A type representing a mempory pool
 */
typedef void *sys_mempool_t;

extern int sys_mempool_create(sys_mempool_t * mempool,
							  void *chunk, const __CSTD uint64_t chunk_size,
							  const __CSTD uint64_t block_size,
							  const __CSTD uint64_t ralignment);

extern void *sys_mempool_allocate_block(sys_mempool_t mempool);

extern void *sys_mempool_try_allocate_block(sys_mempool_t mempool);

extern int sys_mempool_free_block(sys_mempool_t mempool, void *block);

extern void sys_mempool_destroy(sys_mempool_t mempool);

extern __CSTD uint64_t sys_mempool_get_count(sys_mempool_t mempool);

CDECL_END

#endif /* __SYS_SYS_MEMPOOL_H__ */
/*@}*/
