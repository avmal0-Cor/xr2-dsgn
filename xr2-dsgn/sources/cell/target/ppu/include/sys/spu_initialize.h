/*   SCE CONFIDENTIAL                                       */
/*   PlayStation(R)3 Programmer Tool Runtime Library 310.001 */
/*   Copyright (C) 2006 Sony Computer Entertainment Inc.    */
/*   All Rights Reserved.                                   */

/**
 * \ingroup all_syscall
 * \defgroup spu SPU Initialization
 * \brief system calls for SPU management.
 * 
 * Interfaces defined in sys/spu_initialize.h
 *
 * Refer to <i>Cell OS Lv-2 User's Manual</i> for the detailed description. 
 */

/*@{*/

#ifndef __SYS_SYS_SPU_INITIALIZE_H__
#define __SYS_SYS_SPU_INITIALIZE_H__

#include <sys/cdefs.h>
#include <sys/syscall.h>

CDECL_BEGIN

/* spu initialize */
extern inline int sys_spu_initialize(unsigned int max_usable_spu,
									 unsigned int max_raw_spu);

extern inline int sys_spu_initialize(unsigned int max_usable_spu,
									 unsigned int max_raw_spu)
{
	system_call_2(SYS_SPU_INITIALIZE, max_usable_spu, max_raw_spu);

#ifdef REG_PASS_SYS_SPU_INITIALIZE
	REG_PASS_SYS_SPU_INITIALIZE;
#endif
	return_to_user_prog(int);
}




/*@}*/

CDECL_END

#endif /* __SYS_SYS_SPU_INITIALIZE_H__ */
