/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2006 Sony Computer Entertainment Inc.
 * All Rights Reserved.
 */

#ifndef __CELL_SPURS_BARRIER_H__
#define __CELL_SPURS_BARRIER_H__ 1

/* The C Standard Library */
#include <stdint.h>

/* SPURS */
#include <cell/spurs/task_types.h>		/* CellSpursTaskset */
#include <cell/spurs/barrier_types.h>	/* CellSpursBarrier */
#include <cell/spurs/error.h>

#ifdef __cplusplus
extern "C" {
#endif

int cellSpursBarrierInitialize(CellSpursTaskset*, CellSpursBarrier*, unsigned int total);
int cellSpursBarrierGetTasksetAddress(const CellSpursBarrier*, CellSpursTaskset**);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* __CELL_SPURS_BARRIER_H__ */

/*
 * Local Variables:
 * mode: C
 * c-file-style: "stroustrup"
 * tab-width: 4
 * End:
 * vim:sw=4:sts=4:ts=4
 */
