/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2008 Sony Computer Entertainment Inc.
 * All Rights Reserved.
 */

#ifndef __CELL_SPURS_EXCEPTION_H__
#define __CELL_SPURS_EXCEPTION_H__

#ifdef __cplusplus
extern "C" {
#endif

/* The C Standard Library */
#include <stdint.h>
#include <stdbool.h>
#include <sys/types.h>

/* SPURS */
#include <cell/spurs/types.h>			/* CellSpurs, CellSpursWorkloadId */
#include <cell/spurs/error.h>
#include <cell/spurs/exception_types.h>

int cellSpursEnableExceptionEventHandler(CellSpurs* spurs, bool flag);
int cellSpursSetExceptionEventHandler(
		CellSpurs* spurs,
		CellSpursWorkloadId id,
		CellSpursExceptionEventHandler eaHandler,
		void* arg
	);
int cellSpursUnsetExceptionEventHandler(
		CellSpurs* pSpurs,
		CellSpursWorkloadId id
	);

int cellSpursSetGlobalExceptionEventHandler(
		CellSpurs* spurs,
		CellSpursGlobalExceptionEventHandler eaHandler,
		void* arg
	);

int cellSpursUnsetGlobalExceptionEventHandler(
		CellSpurs* spurs
	);


#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* __CELL_SPURS_EXCEPTION_H__ */

