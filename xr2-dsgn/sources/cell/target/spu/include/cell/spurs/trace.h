/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2008 Sony Computer Entertainment Inc.
 * All Rights Reserved.
 */
#ifndef __CELL_SPURS_TRACE_H__
#define __CELL_SPURS_TRACE_H__ 1

#include <cell/trace/trace_types.h>

/* SPURS */
#include <cell/spurs/trace_types.h>		/* CellSpursTracePacket */

#ifdef __cplusplus
extern "C" {
#endif

void cellSpursModulePutTrace(CellSpursTracePacket *packet, unsigned tag);
void cellSpursPutTrace(CellSpursTracePacket *packet, unsigned tag);
void cellSpursPutUserTrace(CellTraceHeader *packet, unsigned tag);

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
