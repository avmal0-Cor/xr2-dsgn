/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2009 Sony Computer Entertainment Inc.
 * All Rights Reserved.
 */

#ifndef CELL_SYNC2_CONDITION_H
#define CELL_SYNC2_CONDITION_H

#include <sdk_version.h>
#include <cell/sync2/mutex.h>
#include <cell/sync2/cond_types.h>
#include <cell/sync2/error.h>

#ifdef __cplusplus
extern "C" {
#endif

int cellSync2CondWait(
		uint64_t eaCondition,
		const CellSync2ThreadConfig* config,
		unsigned int dmaTag
	);

int cellSync2CondSignal(
		uint64_t eaCondition,
		const CellSync2ThreadConfig* config,
		unsigned int dmaTag
	);

int cellSync2CondSignalAll(
		uint64_t eaCondition,
		const CellSync2ThreadConfig* config,
		unsigned int dmaTag
	);

#ifdef __cplusplus
} // extern "C"
#endif


#endif /* CELL_SYNC2_CONDITION_H */
