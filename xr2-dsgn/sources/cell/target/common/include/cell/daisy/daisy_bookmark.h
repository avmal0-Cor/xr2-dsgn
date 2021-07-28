/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2006 Sony Computer Entertainment Inc.
 * All Rights Reserved.
 */

/* libdaisy bookmark macro definitions */

#ifndef __CELL_DAISY_BOOKMARK_H__
#define __CELL_DAISY_BOOKMARK_H__
#include <stdlib.h>
#ifdef __powerpc__
#include <cell/perf/performance.h>
#endif

/* bookmark */
#define CELL_DAISY_BOOKMARK_PREFIX_PIPE                 (0)
#define CELL_DAISY_BOOKMARK_PREFIX_SNR_QCTL             (1)
#define CELL_DAISY_BOOKMARK_PREFIX_ATO_QCTL             (2)
#define CELL_DAISY_BOOKMARK_PREFIX_LQCTL                (3)
#define CELL_DAISY_BOOKMARK_PREFIX_LOCAL_BUFFER         (4)
#define CELL_DAISY_BOOKMARK_PREFIX_REMOTE_BUFFER        (5)
#define CELL_DAISY_BOOKMARK_PREFIX_STRIDE_REMOTE_BUFFER (8)
#define CELL_DAISY_BOOKMARK_PREFIX_GLUE_IN              (9)
#define CELL_DAISY_BOOKMARK_PREFIX_GLUE_OUT             (10)

#ifdef __SPU__
#define CELL_DAISY_BOOKMARK_MAX_ARRAY_COUNT 256
#define CELL_DAISY_BOOKMARK_DMA_TAG 31
namespace cell {
	namespace Daisy {
		typedef struct {
			uint32_t mDecrementer;
			uint16_t mBookmark;
			uint16_t mSpuNum;
		} BookmarkContent;
		
		extern BookmarkContent *bookmarkArray;
		extern uint32_t         bookmarkArrayIndex;
		
		void bookmarkInitialize(unsigned long long atomicAreaEa);
		void castoutBookmarkArray();
		void finalizeBookmarkArray();
		
		extern uint16_t sBookmarkSpuNum;
	}
}
#else /* __SPU__ */
#undef CELL_DAISY_BOOKMARK_DECREMENTER
#endif /* __SPU__ */

#ifndef CELL_DAISY_BOOKMARK_DECREMENTER
// Performance Analyzer bookmark
#ifdef __SPU__
#define CELL_DAISY_WRITE_BOOKMARK(x) __asm__ volatile("wrch $69, %0"::"r"(x));
#else
#define CELL_DAISY_WRITE_BOOKMARK(x) cellPerfInsertBookmark(x)
#endif

#else /* CELL_DAISY_BOOKMARK_DECREMENTER */
// Decrementer bookmark
#define CELL_DAISY_WRITE_BOOKMARK(x) \
register uint32_t decr = spu_read_decrementer(); \
cell::Daisy::bookmarkArray[cell::Daisy::bookmarkArrayIndex].mDecrementer = decr; \
cell::Daisy::bookmarkArray[cell::Daisy::bookmarkArrayIndex].mBookmark = (x); \
cell::Daisy::bookmarkArray[cell::Daisy::bookmarkArrayIndex].mSpuNum = sBookmarkSpuNum;\
cell::Daisy::bookmarkArrayIndex++;\
if (__builtin_expect(cell::Daisy::bookmarkArrayIndex % CELL_DAISY_BOOKMARK_MAX_ARRAY_COUNT == 0, 0)) { \
	cell::Daisy::castoutBookmarkArray(); \
    if (cell::Daisy::bookmarkArrayIndex == CELL_DAISY_BOOKMARK_MAX_ARRAY_COUNT*2) {\
      cell::Daisy::bookmarkArrayIndex = 0;\
    }\
}

#endif /* CELL_DAISY_BOOKMARK_DECREMENTER */

#endif /* __CELL_DAISY_BOOKMARK_H__ */

/* CELL_DAISY_BOOKMARK */
#undef CELL_DAISY_BOOKMARK
#undef CELL_DAISY_BOOKMARK_CLASS
#undef CELL_DAISY_USER_BOOKMARK

#define CELL_DAISY_BOOKMARK(f) \
do{ \
  CELL_DAISY_WRITE_BOOKMARK((1<<15) | \
	  (__CELL_DAISY_BOOKMARK_PREFIX__<<7) | \
          (this->mBookmarkId << 4)       | \
	  (f)) \
}while(0)

#define CELL_DAISY_BOOKMARK_CLASS(c,f) \
do{ \
  CELL_DAISY_WRITE_BOOKMARK((1<<15) | \
	  ((c)<<7) | \
          (this->mBookmarkId << 4)       | \
	  (f)) \
}while(0)

#define CELL_DAISY_USER_BOOKMARK(x) \
do{ \
  CELL_DAISY_WRITE_BOOKMARK(0x7fff & (x)) \
}while(0)

/*
 * Local Variables:
 * mode:C++
 * tab-width:4
 * End:
 * vim:ts=4:sw=4:
 */
