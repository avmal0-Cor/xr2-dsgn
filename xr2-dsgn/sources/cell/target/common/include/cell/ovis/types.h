/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2008 Sony Computer Entertainment Inc.
* All Rights Reserved.
*/

#ifndef __CELL_OVIS_TYPES_H__
#define __CELL_OVIS_TYPES_H__

#ifdef __SPU__
#include <spu_intrinsics.h>
typedef vec_uint4 _ovly_table_t;
#endif

#ifdef __PPU__
#include <stdint.h>
typedef struct _ovly_table_t {
	uint32_t vma;
	uint32_t size;
	uint32_t lma;
	uint32_t mapped;
} __attribute__((aligned(16))) _ovly_table_t;
#endif

#endif /* #ifndef __CELL_OVIS_TYPES_H__ */
