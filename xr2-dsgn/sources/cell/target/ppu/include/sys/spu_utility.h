/*   SCE CONFIDENTIAL                                       */
/*   PlayStation(R)3 Programmer Tool Runtime Library 310.001 */
/*   Copyright (C) 2006 Sony Computer Entertainment Inc.    */
/*   All Rights Reserved.                                   */

/**
 * \defgroup spu_utility SPU Utility Library Functions
 * \brief SPU utility library functions defined in spu_utility.h
 * 
 * Interfaces defined in spu_utility.h
 *
 * Refer to Cell OS Lv-2 User's Manual for the detailed description.
 */
/*@{*/
#ifndef __SYS_SYS_SPU_UTILITY_H__
#define __SYS_SYS_SPU_UTILITY_H__

#include <sys/cdefs.h>
#include <sys/types.h>
#include <sys/spu_thread.h>
#include <sys/raw_spu.h>
#include <sys/spu_image.h>

CDECL_BEGIN

/** \brief get spu program information */
int sys_spu_elf_get_information(sys_addr_t elf_img,
								__CSTD uint32_t * entry, int *nseg);

/** \brief get spu segments */
int sys_spu_elf_get_segments(sys_addr_t elf_img,
							 sys_spu_segment_t * segments, int nseg);

/** \brief load spu program into memory */
int sys_spu_thread_elf_loader(const char *elf_name,
							  sys_spu_thread_attribute_t * attr,
							  sys_spu_segment_t ** segs, void **elf_img);

/** \brief load spu program into LS of Raw SPU */
int sys_raw_spu_load(sys_raw_spu_t id, const char *path,
					 __CSTD uint32_t * entry);

/** \brief load spu image into LS of Raw SPU */
int sys_raw_spu_image_load(sys_raw_spu_t id, sys_spu_image_t * img);

CDECL_END

#endif /* __SYS_SYS_SPU_UTILITY_H__ */
/*@}*/
