/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2006 Sony Computer Entertainment Inc.
 * All Rights Reserved.
 */

#ifndef __CELL_SPURS_TRACE_H__
#define __CELL_SPURS_TRACE_H__ 1

/* The C Standard Library */
#include <stdint.h>
#include <sys/types.h> /* size_t */

/* SPURS */
#include <cell/spurs/trace_types.h>		/* CellSpursTracePacket */
#include <cell/spurs/types.h>			/* CellSpurs */
#include <cell/spurs/error.h>

#ifdef __cplusplus
extern "C" {
#endif

int cellSpursTraceInitialize(CellSpurs*, void *buffer, size_t size, uint32_t mode);
int cellSpursTraceStart(CellSpurs*);
int cellSpursTraceStop(CellSpurs*);
int cellSpursTraceFinalize(CellSpurs*);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* __CELL_SPURS_TRACE_H__ */

/*
 * Local Variables:
 * mode: C
 * c-file-style: "stroustrup"
 * tab-width: 4
 * End:
 * vim:sw=4:sts=4:ts=4
 */
