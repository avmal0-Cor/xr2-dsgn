/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2009 Sony Computer Entertainment Inc.
 * All Rights Reserved.
 */

#ifndef __CELL_SPURS_SYSTEM_WORKLOAD_TYPES_H__
#define __CELL_SPURS_SYSTEM_WORKLOAD_TYPES_H__

#include <stdint.h>
#include <cell/spurs/types.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct CellSpursSystemWorkloadAttribute CellSpursSystemWorkloadAttribute;

#define	CELL_SPURS_SYSTEM_WORKLOAD_ALIGN	128
#define	CELL_SPURS_SYSTEM_WORKLOAD_SIZE		256

typedef struct CellSpursSystemWorkload {
	uint8_t	skip[CELL_SPURS_SYSTEM_WORKLOAD_SIZE];
} __attribute__((aligned(CELL_SPURS_SYSTEM_WORKLOAD_ALIGN))) CellSpursSystemWorkload;

#define CELL_SPURS_SYSTEM_WORKLOAD_CATEGORY_LIGHT_BACKGROUND	(1U << 0)
#define CELL_SPURS_SYSTEM_WORKLOAD_CATEGORY_HEAVY_BACKGROUND	(1U << 1)
#define CELL_SPURS_SYSTEM_WORKLOAD_CATEGORY_LIGHT_UTILITY 		(1U << 2)
#define CELL_SPURS_SYSTEM_WORKLOAD_CATEGORY_HEAVY_UTILITY 		(1U << 3)

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* __CELL_SPURS_SYSTEM_WORKLOAD_TYPES_H__ */
