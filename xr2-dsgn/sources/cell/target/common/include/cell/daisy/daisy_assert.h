/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2006 Sony Computer Entertainment Inc.
 * All Rights Reserved.
 */

/* libdaisy assert macro definitions */

#ifndef __CELL_DAISY_ASSERT_H__
#define __CELL_DAISY_ASSERT_H__

#define CELL_DAISY_MIN_QUEUE_SIZE		1
#define CELL_DAISY_MAX_LOCAL_QUEUE_SIZE		32
#define CELL_DAISY_MAX_ATO_QUEUE_SIZE		32
#define CELL_DAISY_MAX_SNR_QUEUE_SIZE	12

#define CELL_DAISY_MIN_ENTRY_SIZE		16
#define CELL_DAISY_MAX_ENTRY_SIZE		(16*1024)
#define CELL_DAISY_ENTRY_SIZE(x)	(!((x) & 0xf))

#define CELL_DAISY_MAX_DMA_TAG			31

#define CELL_DAISY_EA_ATOMIC(x)		(!((uint32_t)(x) & 0x7f))
#define CELL_DAISY_EA_DMA(x)		(!((x) & 0xf))
#ifndef __CELL_ASSERT__
#define __CELL_ASSERT__
#endif
#include <assert.h>
#ifdef __SPU__
#include <spu_printf.h>
#include <spu_intrinsics.h>
#endif

/* cellDaisyAssert */
/* Assert Definition (Dafault : Enable Assert) */

#ifdef CELL_DAISY_NO_ASSERT
#define cellDaisyAssert(cond) do{}while (false)

#else /* CELL_DAISY_NO_ASSERT */

#ifdef CELL_DAISY_ASSERT_VERBOSE

#ifdef __SPU__
#define cellDaisyAssert(cond) \
do { \
	if (__builtin_expect(!(cond), 0)) { \
		spu_printf("daisy assert!! %s %d\n",__FILE__,__LINE__); \
		assert(0);\
	} \
} while (false)
#else
#define cellDaisyAssert(cond) \
do { \
	if (!(cond)) { \
		printf("daisy assert!! %s %d\n",__FILE__,__LINE__); \
		assert(0);\
	} \
} while (false)
#endif /* __SPU__ */

#else /* CELL_DAISY_ASSERT_VERBOSE */

#define cellDaisyAssert(cond) assert(cond)

#endif /* CELL_DAISY_ASSERT_VERBOSE */
#endif /* CELL_DAISY_NO_ASSERT */

#endif /* __CELL_DAISY_ASSERT_H__ */

/*
 * Local Variables:
 * mode:C++
 * tab-width:4
 * End:
 * vim:ts=4:sw=4:
 */
