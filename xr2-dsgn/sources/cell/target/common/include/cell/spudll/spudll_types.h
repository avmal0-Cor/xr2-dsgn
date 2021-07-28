/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2008 Sony Computer Entertainment Inc.
* All Rights Reserved.
*/

#ifndef __CELL_SPUDLL_TYPES_H__
#define __CELL_SPUDLL_TYPES_H__

#include <stdint.h>

#define CELL_SPUDLL_HANDLE_SIZE 256

typedef struct CellSpudllHandle
{
#ifdef __PPU__
	uint64_t body[CELL_SPUDLL_HANDLE_SIZE / sizeof(uint64_t)];
#else
	qword body[CELL_SPUDLL_HANDLE_SIZE / sizeof(qword)];
#endif
}  __attribute__((aligned(16))) CellSpudllHandle;




#define CELL_SPUDLL_ALLOW_UNRESOLVED_SYMBOL 1

typedef struct CellSpudllHandleConfig
{
	uint32_t			mode;
	uint32_t			dmaTag;
	uint32_t            numMaxReferred;
	uint32_t            numMaxDepend;

	void*				unresolvedSymbolValueForFunc;
	void*				unresolvedSymbolValueForObject;
	void*				unresolvedSymbolValueForOther;
	uint32_t            __reserved__[9];
} CellSpudllHandleConfig;



#endif /* __CELL_SPUDLL_TYPES_H__ */
