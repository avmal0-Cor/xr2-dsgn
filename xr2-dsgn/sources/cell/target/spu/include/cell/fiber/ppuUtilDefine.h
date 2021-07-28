// SCE CONFIDENTIAL
// PlayStation(R)3 Programmer Tool Runtime Library 310.001
// Copyright (C) 2008 Sony Computer Entertainment Inc.
// All Rights Reserved.

#ifndef __CELL_FIBER_PPU_UTIL_DEFINE_H__
#define __CELL_FIBER_PPU_UTIL_DEFINE_H__ 1

#define __CELL_FIBER_PPU_UTIL ::cell::Fiber::Ppu::Util::

#define __CELL_FIBER_PPU_UTIL_BEGIN \
namespace cell {\
	namespace Fiber {\
		namespace Ppu {\
			namespace Util {

#define __CELL_FIBER_PPU_UTIL_END \
			}\
		}\
	}\
}

#ifndef __FILELINE
#define __STR(name) #name
#define __STR_LINE(line) __STR(line)
#define __FILELINE __FILE__":"__STR_LINE(__LINE__)": "
#endif // __FILELINE

#define __RETURN_IF(exp)						\
	do {										\
		int __ret = (exp);						\
		if (__ret) {							\
			return __ret;						\
		}										\
	} while(0);

#endif // __CELL_FIBER_PPU_UTIL_DEFINE_H__

// Local Variables:
// mode: C++
// c-file-style: "stroustrup"
// tab-width: 4
// End:
// vim:sw=4:sts=4:ts=4
