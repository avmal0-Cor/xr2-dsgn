/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2009 Sony Computer Entertainment Inc.
 * All Rights Reserved.
 */

#ifndef __CELL_SPURS_SYSTEM_WORKLOAD_H__
#define __CELL_SPURS_SYSTEM_WORKLOAD_H__

#include <cell/spurs/types.h>
#include <cell/spurs/control.h>
#include <cell/spurs/system_workload_types.h>
#include <sys/sys_types.h>

#ifdef __cplusplus
extern "C" {
#endif

int	cellSpursAttributeEnableSystemWorkload(
			CellSpursAttribute* attr,
			const uint8_t priority[8],
			unsigned int maxSpu,
			const bool isPreemptible[8]
		);

int cellSpursSetPreemptionVictimHints(
			CellSpurs* spurs,
			const bool isPreemptible[8]
		);

int	cellSpursAddCategorySpecificSystemWorkload(
			CellSpurs* spurs,
			CellSpursSystemWorkload* systemWorkload,
			unsigned int category,
			const uint8_t priority[8],
			unsigned int maxSpu,
			const CellSpursSystemWorkloadAttribute* attr
		);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* __CELL_SPURS_SYSTEM_WORKLOAD_H__ */
