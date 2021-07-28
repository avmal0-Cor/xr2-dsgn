/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2008 Sony Computer Entertainment Inc.
 * All Rights Reserved.
 */

#ifndef __CELL_SPURS_JOBQUEUE_DEFINE_H__
#define __CELL_SPURS_JOBQUEUE_DEFINE_H__

#ifdef __cplusplus
#define __CELL_SPURS_JOBQUEUE_BEGIN \
namespace cell { \
	namespace Spurs { \
		namespace JobQueue {

#define __CELL_SPURS_JOBQUEUE_END \
		} \
	} \
}
#else
#define __CELL_SPURS_JOBQUEUE_BEGIN
#define __CELL_SPURS_JOBQUEUE_END
#endif

#ifndef __CELL_SPURS_FILELINE
#define __CELL_SPURS_STR(name) #name
#define __CELL_SPURS_STR_LINE(line) __CELL_SPURS_STR(line)
#define __CELL_SPURS_FILELINE __FILE__":"__CELL_SPURS_STR_LINE(__LINE__)": "
#endif // __CELL_SPURS_FILELINE

#ifdef _DEBUG
#ifdef __SPU__
#include <spu_printf.h>
#define __CELL_SPURS_JOBQUEUE_DPRINTF__(...) spu_printf(__CELL_SPURS_FILELINE __VA_ARGS__)
#else
#include <stdio.h>
#define __CELL_SPURS_JOBQUEUE_DPRINTF__(...) fprintf(stderr, __CELL_SPURS_FILELINE __VA_ARGS__)
#endif
#else
#define __CELL_SPURS_JOBQUEUE_DPRINTF__(...)
#endif

#undef __CELL_SPURS_RETURN_IF
#define __CELL_SPURS_RETURN_IF(exp)                                        \
do {                                                            \
	int __ret = (exp);                                          \
	if (__builtin_expect(__ret, 0)) {                           \
		__CELL_SPURS_JOBQUEUE_DPRINTF__(" ret = %#x\n", __ret); \
		return __ret;                                           \
	}                                                           \
} while(0)

#endif /* __CELL_SPURS_JOBQUEUE_DEFINE_H__ */
