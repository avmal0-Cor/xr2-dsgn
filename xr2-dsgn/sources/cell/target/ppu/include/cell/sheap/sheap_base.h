/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2006 Sony Computer Entertainment Inc.
* All Rights Reserved.
*/


#ifndef __CELL_SHEAP_BASE_H__
#define __CELL_SHEAP_BASE_H__

#include <stdint.h>
#include <cell/sheap/error.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


#define CELL_SHEAP_MIN_HEAP_SIZE   (2*1024)
#define CELL_SHEAP_MIN_BLOCK_SIZE   128 


int
cellSheapInitialize(void* ea_sheap,
					uint64_t size,
					uint32_t spu_dma_tag);

void*
cellSheapAllocate(void* ea_sheap,
				  uint64_t size);


int
cellSheapFree(void* ea_sheap,
			  void* ptr);

int
cellSheapQueryMax(void* ea_sheap);

int
cellSheapQueryFree(void* ea_sheap);

#ifdef __cplusplus
}
#endif /* __cplusplus */



#endif /* __CELL_SHEAP_BASE_H__ */

