/*   SCE CONFIDENTIAL                                       */
/*   PlayStation(R)3 Programmer Tool Runtime Library 310.001 */
/*   Copyright (C) 2005 Sony Computer Entertainment Inc.    */
/*   All Rights Reserved.                                   */

#if !defined(__SYS_TIME_H__)
#define __SYS_TIME_H__

#include <sys/types.h>

#if defined(__cplusplus)
extern "C" {
#endif

	struct timeval {
		int64_t tv_sec;			/* seconds */
		int64_t tv_usec;		/* and microseconds */
	};

#ifdef __cplusplus
}
#endif
#endif							/* __SYS_TIME_H__ */
