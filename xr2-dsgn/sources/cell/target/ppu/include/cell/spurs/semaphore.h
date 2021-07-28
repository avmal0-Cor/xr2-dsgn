/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2006 Sony Computer Entertainment Inc.
 * All Rights Reserved.
 */

#ifndef __CELL_SPURS_SEMAPHORE_H__
#define __CELL_SPURS_SEMAPHORE_H__ 1

/* The C Standard Library */
#include <stdint.h>

/* SPURS */
#include <cell/spurs/types.h>				/* CellSpurs */
#include <cell/spurs/task_types.h>		/* CellSpursTaskset */
#include <cell/spurs/semaphore_types.h>	/* CellSpursSemaphore */
#include <cell/spurs/error.h>

static int cellSpursSemaphoreInitialize(CellSpursTaskset*, CellSpursSemaphore*, int total);
static int cellSpursSemaphoreInitializeIWL(CellSpurs*,     CellSpursSemaphore*, int total);

#ifdef __cplusplus
extern "C" {
#endif

int _cellSpursSemaphoreInitialize(CellSpurs* pSpurs, CellSpursTaskset *pTaskset, CellSpursSemaphore *pSemaphore, int total);
int cellSpursSemaphoreGetTasksetAddress(const CellSpursSemaphore* pSemaphore, CellSpursTaskset** pTaskset);

#ifdef __cplusplus
} /* extern "C" */
#endif

static inline int
cellSpursSemaphoreInitialize(CellSpursTaskset* pTaskset, CellSpursSemaphore* pSemaphore, int total)
{
	return _cellSpursSemaphoreInitialize(0, pTaskset, pSemaphore, total);
}

static inline int
cellSpursSemaphoreInitializeIWL(CellSpurs* pSpurs, CellSpursSemaphore* pSemaphore, int total)
{
	return _cellSpursSemaphoreInitialize(pSpurs, 0, pSemaphore, total);
}

#endif /* __CELL_SPURS_SEMAPHORE_H__ */

/*
 * Local Variables:
 * mode: C
 * c-file-style: "stroustrup"
 * tab-width: 4
 * End:
 * vim:sw=4:sts=4:ts=4
 */
