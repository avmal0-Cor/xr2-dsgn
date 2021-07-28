/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 *                Copyright (C) 2009 Sony Computer Entertainment Inc.
 *                                               All Rights Reserved.
 */


#ifndef CGNV2RT_HEADER
#define CGNV2RT_HEADER

#include <stdio.h>
#include <vector>

#ifndef STL_NAMESPACE
#define STL_NAMESPACE ::std::
#endif

#ifdef JS_DEBUG

#ifdef JS_PLATFORM_WIN32
#define JS_ASSERT2(condition, str) { \
	if (!(condition)) \
	{ \
	printf str; \
	__asm int 3 \
	} \
}
#else

#ifdef JS_PLATFORM_LINUX
#define JS_ASSERT2(condition, str) { \
	if (!(condition)) \
	{ \
	printf str; \
	__asm__ ("int $0x3"); \
	} \
}
#else
//TODO CELL...
#define JS_ASSERT2(condition, str) { \
	if (!(condition)) \
	{ \
	printf str; \
	} \
}
#endif
#endif

#else
#define JS_ASSERT2(condition, str) {}
#endif

#define CNV2END(val) convert_endianness((val), elfEndianness)
#define ENDSWAP(val) convert_endianness((val), (host_endianness() == 1) ? 2 : 1)

#ifdef JS_CG_CONVERTER_REPORT
#include "consoleHelper.h"
#endif

int convertNvToElfFromFile(const char *sourceFile, int endianness, int constTableOffset, void **binaryShader, int *size,
	STL_NAMESPACE vector<char> &stringTable, STL_NAMESPACE vector<float> &defaultValues);
int convertNvToElfFromMemory(const void *sourceData, size_t size, int endianness, int constTableOffset, void **binaryShader, int *binarySize, 
	STL_NAMESPACE vector<char> &stringTable, STL_NAMESPACE vector<float> &defaultValues);

int convertNvToElfFreeBinaryShader(void *binaryShader);

#endif
