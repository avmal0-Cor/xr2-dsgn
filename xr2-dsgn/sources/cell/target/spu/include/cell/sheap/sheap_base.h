/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2006 Sony Computer Entertainment Inc.
* All Rights Reserved.
*/




#ifndef __CELL_SHEAP_BASE_H__
#define __CELL_SHEAP_BASE_H__

#include <stdint.h>
#include <cell/sheap/error.h>
#define CELL_SHEAP_MIN_HEAP_SIZE   (2*1024)
#define CELL_SHEAP_MIN_BLOCK_SIZE   128 

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


int
cellSheapInitialize(uint64_t ea_sheap,
					uint64_t size,
					uint32_t tag);

uint64_t
cellSheapAllocate(uint64_t ea_sheap,
				  uint64_t size);


int
cellSheapFree(uint64_t ea_sheap,
			  uint64_t ptr);

int
cellSheapQueryMax(uint64_t ea_sheap);

int
cellSheapQueryFree(uint64_t ea_sheap);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __CELL_SHEAP_BASE_H__ */

