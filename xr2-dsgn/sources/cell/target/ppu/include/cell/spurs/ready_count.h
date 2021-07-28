/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2006 Sony Computer Entertainment Inc.
 * All Rights Reserved.
 */

#ifndef __CELL_SPURS_READY_COUNT_H__
#define __CELL_SPURS_READY_COUNT_H__ 1

/* The C Standard Library */
#include <stdint.h>

/* SPURS */
#include <cell/spurs/types.h>			/* CellSpursWorkloadId */
#include <cell/spurs/error.h>

#ifdef __cplusplus
extern "C" {
#endif

int cellSpursReadyCountStore(CellSpurs*, CellSpursWorkloadId, unsigned int value);
int cellSpursReadyCountSwap(CellSpurs*, CellSpursWorkloadId, unsigned int *old, unsigned int swap);
int cellSpursReadyCountCompareAndSwap(CellSpurs*, CellSpursWorkloadId, unsigned int *old, unsigned int compare, unsigned int swap);
int cellSpursReadyCountAdd(CellSpurs*, CellSpursWorkloadId, unsigned int *old, int value);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* __CELL_SPURS_READY_COUNT_H__ */

/*
 * Local Variables:
 * mode: C
 * c-file-style: "stroustrup"
 * tab-width: 4
 * End:
 * vim:sw=4:sts=4:ts=4
 */
