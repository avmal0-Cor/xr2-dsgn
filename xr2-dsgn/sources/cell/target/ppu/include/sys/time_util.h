/*   SCE CONFIDENTIAL                                       */
/*   PlayStation(R)3 Programmer Tool Runtime Library 310.001 */
/*   Copyright (C) 2008 Sony Computer Entertainment Inc.    */
/*   All Rights Reserved.                                   */

/**
 * \ingroup lib_all_library
 * \defgroup lib_time Time Facility Functions
 * \brief Library calls defined in time_util.h
 * 
 * Interfaces defined in time_util.h
 */

/*@{*/
#ifndef __SYS_SYS_TIME_UTIL_H__
#define __SYS_SYS_TIME_UTIL_H__

#include <sys/types.h>

/**
 * Get the current timebase.
 *
 * <b>Example:</b>\n
 * \code
 *   uint64_t cur_tb;
 *   SYS_TIMEBASE_GET(cur_tb);
 * \endcode
 */
#ifdef __SNC__

#define SYS_TIMEBASE_GET(tb)   \
  do{                                                    \
    long long  tmp;                                       \
    do                                                     \
    {                                                      \
        tmp  = __builtin_mftb();                           \
    } while( __builtin_expect( (unsigned)tmp == 0,  0 ) ); \
    (tb) =  tmp;                                           \
  }while(0)

#else /* !__SNC__ */

#define SYS_TIMEBASE_GET(tb)	\
  do{							   \
		__asm__ volatile ("1: mftb %[current_tb];" \
				  "cmpwi 7, %[current_tb], 0;"	\
				  "beq-  7, 1b;"		\
				  : [current_tb] "=r" (tb):	\
				  :"cr7");			\
  }while(0)

#endif /* __SNC__ */

#endif /* __SYS_SYS_TIME_UTIL_H__ */
/*@}*/
