/*   SCE CONFIDENTIAL                                       */
/*   PlayStation(R)3 Programmer Tool Runtime Library 310.001 */
/*   Copyright (C) 2006 Sony Computer Entertainment Inc.    */
/*   All Rights Reserved.                                   */

/**
 * \ingroup all_syscall
 * \defgroup spu SPU image
 * \brief system calls for SPU program images
 * 
 * Interfaces defined in sys/spu_image.h
 *
 * Refer to <i>Cell OS Lv-2 User's Manual</i> for the detailed description. 
 */

/*@{*/

#ifndef __SYS_SYS_SPU_IMAGE_H__
#define __SYS_SYS_SPU_IMAGE_H__

#include <sys/cdefs.h>
#include <sys/syscall.h>
#include <sys/integertypes.h>
#include <sys/return_code.h>
#include <sys/types.h>


CDECL_BEGIN


#define SYS_SPU_SEGMENT_TYPE_COPY  0x0001
#define SYS_SPU_SEGMENT_TYPE_FILL  0x0002
#define SYS_SPU_SEGMENT_TYPE_INFO  0x0004

/** \brief SPU segment structure
 *
 * The sys_spu_segment_t structure type describes an SPU segment.
 * The initial image to be loaded to an SPU is represented by an array 
 * of this structure type.
 *
 * \a type must be one of the following values.
 * - SYS_SPU_SEGMENT_TYPE_COPY \n
 *   The segment type is COPY.  A segment of this type copies a specified 
 *   area in the process address space to the local storage.
 * - SYS_SPU_SEGMENT_TYPE_FILL \n
 *   The segment type is FILL.  A segment of this type fills a specified 
 *   area in the local storage with an arbitrary 32-bit values.
 * - SYS_SPU_SEGMENT_TYPE_INFO \n
 *   The segment type is INFO.  The detail specification is TBD.
 *
 * \a size represents the size of this SPU segment, and its valid range is 
 * from 128 byte to 16 KB.
 *
 * \a src is a union type, and its meaning differs with the SPU segment type.
 * \a pa_start is used if \a type is SYS_SPU_SEGMENT_TYPE_COPY, and 
 * represents the top effective address of an area in the process address 
 * space that is copied to the SPU segment.  \a pa_start must be aligned to
 * 128 bytes.
 * \a value is used if \a type is 
 * SYS_SEGMENT_TYPE_FILL, and represents the 32-bit value to be filled in 
 * the SPU segment.
 * 
 */
struct sys_spu_segment {
	/** SPU segment type (COPY/FILL/INFO) */
	int type;
	/** Top LS address on which this SPU segment will be loaded */
	__CSTD uint32_t ls_start;
	/** Size of this SPU segment in bytes */
	int size;
	union {
		/** Top effective address which an SPU segment of COPY type will copy from */
		sys_addr_t pa_start;
		/** The value to be filled in an SPU segment of FILL type */
		__CSTD uint32_t value;
		uint64_t pad;
	} src;
};
typedef struct sys_spu_segment sys_spu_segment_t;



#define SYS_SPU_IMAGE_TYPE_USER     0x0U
#define SYS_SPU_IMAGE_TYPE_KERNEL   0x1U

struct sys_spu_image {
	uint32_t type;
	uint32_t entry_point;
	sys_spu_segment_t *segs;
	int nsegs;
};
typedef struct sys_spu_image sys_spu_image_t;


#define SYS_SPU_IMAGE_PROTECT       0x0U
#define SYS_SPU_IMAGE_DIRECT        0x1U

extern int sys_spu_image_import(sys_spu_image_t * img, const void *src,
								uint32_t type);
extern int sys_spu_image_close(sys_spu_image_t * img);

extern inline int sys_spu_image_open(sys_spu_image_t * img, const char *path);

extern inline int sys_spu_image_open(sys_spu_image_t * img, const char *path)
{
	system_call_2(SYS_SPU_IMAGE_OPEN, (uint32_t) img, (uint32_t) path);

#ifdef REG_PASS_SYS_SPU_IMAGE_OPEN
	REG_PASS_SYS_SPU_IMAGE_OPEN;
#endif
	return_to_user_prog(int);
}



extern inline int sys_spu_image_open_by_fd(sys_spu_image_t * img, int fd,
										   off64_t offset);

extern inline int sys_spu_image_open_by_fd(sys_spu_image_t * img, int fd,
										   off64_t offset)
{
	system_call_3(SYS_SPU_IMAGE_OPEN_BY_FD, (uint32_t) img, fd, offset);

#ifdef REG_PASS_SYS_SPU_IMAGE_OPEN_BY_FD
	REG_PASS_SYS_SPU_IMAGE_OPEN_BY_FD;
#endif
	return_to_user_prog(int);
}



/*@}*/

CDECL_END

#endif /* __SYS_SYS_SPU_IMAGE_H__ */
