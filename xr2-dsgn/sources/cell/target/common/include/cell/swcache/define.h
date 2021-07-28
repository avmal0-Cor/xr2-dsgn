/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2009 Sony Computer Entertainment Inc.
 * All Rights Reserved.
 */

#ifndef __CELL_SWCACHE_DEFEINE_H__
#define __CELL_SWCACHE_DEFEINE_H__

#ifndef __CELL_ASSERT__
#define __CELL_ASSERT__
#endif

/* standard C++ header */
#include <assert.h>

#include <cell/swcache/error.h>

#undef _STR
#define _STR(x) #x

#ifdef __SPU__
#include <spu_printf.h>
#include <spu_intrinsics.h>
#define __CELL_SWCACHE_DPRINTF__(format,...) spu_printf("%s" format "\tin %s:%u\n", "[SPU]", ##__VA_ARGS__,__FILE__,__LINE__)
#else // #ifdef __SPU__
#include <stdio.h>
#define __CELL_SWCACHE_DPRINTF__(format,...) fprintf(stderr, "%s" format "\tin %s:%u\n", "[PPU]", ##__VA_ARGS__,__FILE__,__LINE__)
#endif // #ifdef __SPU__

#ifdef CELL_SWCACHE_DEBUG_1
#undef CELL_SWCACHE_DEBUG
#define CELL_SWCACHE_DEBUG
#endif
#ifdef CELL_SWCACHE_DEBUG_2
#undef CELL_SWCACHE_DEBUG
#undef CELL_SWCACHE_DEBUG_1
#define CELL_SWCACHE_DEBUG
#define CELL_SWCACHE_DEBUG_1
#endif
#ifdef CELL_SWCACHE_DEBUG_3
#undef CELL_SWCACHE_DEBUG
#undef CELL_SWCACHE_DEBUG_1
#undef CELL_SWCACHE_DEBUG_2
#define CELL_SWCACHE_DEBUG
#define CELL_SWCACHE_DEBUG_1
#define CELL_SWCACHE_DEBUG_2
#endif
#ifdef CELL_SWCACHE_DEBUG_4
#undef CELL_SWCACHE_DEBUG
#undef CELL_SWCACHE_DEBUG_1
#undef CELL_SWCACHE_DEBUG_2
#undef CELL_SWCACHE_DEBUG_3
#define CELL_SWCACHE_DEBUG
#define CELL_SWCACHE_DEBUG_1
#define CELL_SWCACHE_DEBUG_2
#define CELL_SWCACHE_DEBUG_3
#endif

#if defined(CELL_SWCACHE_ASSERT_VERBOSE) && !defined(NDEBUG)
#define __CELL_SWCACHE_ASSERT(x,format,...) \
	if (__builtin_expect(!(x), 0)) { __CELL_SWCACHE_DPRINTF__("%s" format, "libswcache assert!!: ", ##__VA_ARGS__); _CSTD _SCE_Assert("",""); }
#elif !defined(NDEBUG)
#define __CELL_SWCACHE_ASSERT(x,...) ((x)?(void)0:_CSTD _SCE_Assert("libswcaceh assert!!: ",""))
#else
#define __CELL_SWCACHE_ASSERT(x,...) ((void)0)
#endif

#ifdef CELL_SWCACHE_DEBUG
#define __CELL_SWCACHE_DPRINTF(format,...) __CELL_SWCACHE_DPRINTF__("%s" format,cell::swcache::__LogMessageBlk::getIndent(0),##__VA_ARGS__)
#define __CELL_SWCACHE_DPRINTF_IN(format,...) cell::swcache::__LogMessageBlk::flip(0);__CELL_SWCACHE_DPRINTF__(format,##__VA_ARGS__)
#define __CELL_SWCACHE_DPRINTF_OUT(format,...) cell::swcache::__LogMessageBlk::flip(0);__CELL_SWCACHE_DPRINTF__("+->" format,##__VA_ARGS__)
#else
#define __CELL_SWCACHE_DPRINTF(...)
#define __CELL_SWCACHE_DPRINTF_IN(...)
#define __CELL_SWCACHE_DPRINTF_OUT(...)
#endif

#ifdef CELL_SWCACHE_DEBUG_1
#define __CELL_SWCACHE_DPRINTF_1(format,...) __CELL_SWCACHE_DPRINTF__("%s  %s" format,cell::swcache::__LogMessageBlk::getIndent(0),cell::swcache::__LogMessageBlk::getIndent(1),##__VA_ARGS__)
#define __CELL_SWCACHE_DPRINTF_1_IN(format,...) cell::swcache::__LogMessageBlk::flip(1);__CELL_SWCACHE_DPRINTF__("%s  " format,cell::swcache::__LogMessageBlk::getIndent(0),##__VA_ARGS__)
#define __CELL_SWCACHE_DPRINTF_1_OUT(format,...) cell::swcache::__LogMessageBlk::flip(1);__CELL_SWCACHE_DPRINTF__("%s  +->" format,cell::swcache::__LogMessageBlk::getIndent(0),##__VA_ARGS__)
#else
#define __CELL_SWCACHE_DPRINTF_1(...)
#define __CELL_SWCACHE_DPRINTF_1_IN(...)
#define __CELL_SWCACHE_DPRINTF_1_OUT(...)
#endif

#ifdef CELL_SWCACHE_DEBUG_2
#define __CELL_SWCACHE_DPRINTF_2(format,...) __CELL_SWCACHE_DPRINTF__("%s  %s  %s" format, cell::swcache::__LogMessageBlk::getIndent(0),cell::swcache::__LogMessageBlk::getIndent(1),cell::swcache::__LogMessageBlk::getIndent(2),##__VA_ARGS__)
#define __CELL_SWCACHE_DPRINTF_2_IN(format,...) cell::swcache::__LogMessageBlk::flip(2);__CELL_SWCACHE_DPRINTF__("%s  %s  " format, cell::swcache::__LogMessageBlk::getIndent(0),cell::swcache::__LogMessageBlk::getIndent(1),##__VA_ARGS__)
#define __CELL_SWCACHE_DPRINTF_2_OUT(format,...) cell::swcache::__LogMessageBlk::flip(2);__CELL_SWCACHE_DPRINTF__("%s  %s  +->" format, cell::swcache::__LogMessageBlk::getIndent(0),cell::swcache::__LogMessageBlk::getIndent(1),##__VA_ARGS__)
#else
#define __CELL_SWCACHE_DPRINTF_2(...)
#define __CELL_SWCACHE_DPRINTF_2_IN(...)
#define __CELL_SWCACHE_DPRINTF_2_OUT(...)
#endif

#ifdef CELL_SWCACHE_DEBUG_3
#define __CELL_SWCACHE_DPRINTF_3(format,...) __CELL_SWCACHE_DPRINTF__("%s  %s  %s  %s" format, cell::swcache::__LogMessageBlk::getIndent(0),cell::swcache::__LogMessageBlk::getIndent(1),cell::swcache::__LogMessageBlk::getIndent(2),cell::swcache::__LogMessageBlk::getIndent(3),##__VA_ARGS__)
#define __CELL_SWCACHE_DPRINTF_3_IN(format,...) cell::swcache::__LogMessageBlk::flip(3);__CELL_SWCACHE_DPRINTF__("%s  %s  %s  " format, cell::swcache::__LogMessageBlk::getIndent(0),cell::swcache::__LogMessageBlk::getIndent(1),cell::swcache::__LogMessageBlk::getIndent(2),##__VA_ARGS__)
#define __CELL_SWCACHE_DPRINTF_3_OUT(format,...) cell::swcache::__LogMessageBlk::flip(3);__CELL_SWCACHE_DPRINTF__("%s  %s  %s  +->" format, cell::swcache::__LogMessageBlk::getIndent(0),cell::swcache::__LogMessageBlk::getIndent(1),cell::swcache::__LogMessageBlk::getIndent(2),##__VA_ARGS__)
#else
#define __CELL_SWCACHE_DPRINTF_3(...)
#define __CELL_SWCACHE_DPRINTF_3_IN(...)
#define __CELL_SWCACHE_DPRINTF_3_OUT(...)
#endif

#ifdef CELL_SWCACHE_DEBUG_4
#define __CELL_SWCACHE_DPRINTF_4(format,...) __CELL_SWCACHE_DPRINTF__("%s  %s  %s  %s  %s" format, cell::swcache::__LogMessageBlk::getIndent(0),cell::swcache::__LogMessageBlk::getIndent(1),cell::swcache::__LogMessageBlk::getIndent(2),cell::swcache::__LogMessageBlk::getIndent(3),cell::swcache::__LogMessageBlk::getIndent(4),##__VA_ARGS__)
#define __CELL_SWCACHE_DPRINTF_4_IN(format,...) cell::swcache::__LogMessageBlk::flip(4);__CELL_SWCACHE_DPRINTF__("%s  %s  %s  %s  " format, cell::swcache::__LogMessageBlk::getIndent(0),cell::swcache::__LogMessageBlk::getIndent(1),cell::swcache::__LogMessageBlk::getIndent(2),cell::swcache::__LogMessageBlk::getIndent(3),##__VA_ARGS__)
#define __CELL_SWCACHE_DPRINTF_4_OUT(format,...) cell::swcache::__LogMessageBlk::flip(4);__CELL_SWCACHE_DPRINTF__("%s  %s  %s  %s  +->" format, cell::swcache::__LogMessageBlk::getIndent(0),cell::swcache::__LogMessageBlk::getIndent(1),cell::swcache::__LogMessageBlk::getIndent(2),cell::swcache::__LogMessageBlk::getIndent(3),##__VA_ARGS__)
#else
#define __CELL_SWCACHE_DPRINTF_4(...)
#define __CELL_SWCACHE_DPRINTF_4_IN(...)
#define __CELL_SWCACHE_DPRINTF_4_OUT(...)
#endif

// Compile Time Assert
////////////////////////
template<bool> struct __CellSwcacheCompileTimeAssertCheck;
template<> struct __CellSwcacheCompileTimeAssertCheck<true> { };
#define __CELL_SWCACHE_COMPILE_ASSERT(expr) \
{ (void)sizeof(__CellSwcacheCompileTimeAssertCheck<(expr) != 0>); }

#define __CELL_SWCACHE_BEGIN \
namespace cell {             \
	namespace swcache {      \

#define __CELL_SWCACHE_END   \
	}                        \
}

#define __CELL_SWCACHE \
	::cell::swcache::

#define __CELL_SWCACHE_INLINE inline __attribute__((always_inline))

#ifndef CELL_SWCACHE_NO_ALWAYS_INLINE
#if defined(__PPU__) && defined(CELL_SWCACHE_PPU_NO_ALWAYS_INLINE)
#define CELL_SWCACHE_NO_ALWAYS_INLINE
#endif
#if defined(__SPU__) && defined(CELL_SWCACHE_SPU_NO_ALWAYS_INLINE)
#define CELL_SWCACHE_NO_ALWAYS_INLINE
#endif
#else
#ifndef CELL_SWCACHE_SPU_NO_ALWAYS_INLINE
#define CELL_SWCACHE_SPU_NO_ALWAYS_INLINE
#endif
#ifndef CELL_SWCACHE_PPU_NO_ALWAYS_INLINE
#define CELL_SWCACHE_PPU_NO_ALWAYS_INLINE
#endif
#endif

#ifdef CELL_SWCACHE_NO_ALWAYS_INLINE
#define __CELL_SWCACHE_ALWAYS_INLINE
#define __CELL_SWCACHE_ATTRIBUTE_ALWAYS_INLINE
#else
#define __CELL_SWCACHE_ALWAYS_INLINE inline __attribute__((always_inline))
#define __CELL_SWCACHE_ATTRIBUTE_ALWAYS_INLINE __attribute__((always_inline))
#endif

__CELL_SWCACHE_BEGIN

static __CELL_SWCACHE_INLINE size_t ceil16(size_t size)		{ return (size + 15) & ~15; }
static __CELL_SWCACHE_INLINE void *floor16(void *ptr)		{ return (void *)((uintptr_t)ptr&~15); }

struct __LogMessageBlk {
	static void flip(int lev) { (void)getIndent(lev,1); }
	static const char *getIndent(int lev, int isFlip = 0)
	{
		static int isBlkArray[5] = {0,0,0,0,0};
		if (isFlip) isBlkArray[lev] ^= 1;
		return isBlkArray[lev] ? "|" : "";
	}
};

__CELL_SWCACHE_END

#ifndef CELL_SWCACHE_SAFE_LV
#define CELL_SWCACHE_SAFE_LV 0
#endif

#ifndef CELL_SWCACHE_DMA_WAIT_TAG_STATUS_ALL
#ifdef __SPU__
#include <cell/dma.h>
#endif
#define CELL_SWCACHE_DMA_WAIT_TAG_STATUS_ALL cellDmaWaitTagStatusAll
#endif

#endif /* __CELL_SWCACHE_DEFEINE_H__ */
