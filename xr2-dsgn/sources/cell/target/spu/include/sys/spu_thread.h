/*   SCE CONFIDENTIAL                                       */
/*   PlayStation(R)3 Programmer Tool Runtime Library 310.001 */
/*   Copyright (C) 2005 Sony Computer Entertainment Inc.    */
/*   All Rights Reserved.                                   */
/**
 * \ingroup spu_all_spulib
 * \defgroup spulib_spu_thread SPU Thread Functions
 * \brief Library functions defined in spu/spu_thread.h
 *
 * The functions in the category are used by SPU threads to manipulate 
 * the SPU thread and SPU thread group.
 */
/*@{*/

#ifndef __SYS_SYS_SPU_THREAD_H__
#define __SYS_SYS_SPU_THREAD_H__

#include <sys/cdefs.h>

CDECL_BEGIN


/*
 * Memory map definitions
 *
 * Addresses of resources of an SPU thread which is belongs to own thread
 * group can be calculated as following expression.
 *
 * \code
 * Local Storage = SYS_SPU_THREAD_OFFSET * spu_num + SYS_SPU_THREAD_BASE_LOW +
 *                 SYS_SPU_THREAD_LS_BASE
 * SNR1          = SYS_SPU_THREAD_OFFSET * spu_num + SYS_SPU_THREAD_BASE_LOW +
 *                 SYS_SPU_THREAD_SNR1
 * SNR2          = SYS_SPU_THREAD_OFFSET * spu_num + SYS_SPU_THREAD_BASE_LOW +
 *                 SYS_SPU_THREAD_SNR2
 * \endcode
 * 
 * See the spu_thread_configure for more detail.
 */
#define SYS_SPU_THREAD_BASE_HIGH 0x00000000U  /**< SPU thread base high address */
#define SYS_SPU_THREAD_BASE_LOW  0xF0000000U  /**< SPU thread base low address*/
#define SYS_SPU_THREAD_OFFSET      0x100000U  /**< SPU thread offset*/
#define SYS_SPU_THREAD_LS_BASE     0x000000U  /**< SPU thread local storage base*/
#define SYS_SPU_THREAD_SNR1        0x05400CU  /**< SPU thread SNR1 */
#define SYS_SPU_THREAD_SNR2        0x05C00CU  /**< SPU thread SNR2 */

extern void sys_spu_thread_group_exit(int status);
extern void sys_spu_thread_group_yield(void);
extern void sys_spu_thread_exit(int status);

CDECL_END

#endif /* __SYS_SYS_SPU_THREAD_H__ */

/*@}*/
