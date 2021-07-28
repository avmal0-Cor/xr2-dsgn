/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2006 Sony Computer Entertainment Inc.
 * All Rights Reserved.
 */

#ifndef __CELL_SPURS_EVENT_FLAG_H__
#define __CELL_SPURS_EVENT_FLAG_H__ 1

/* The C Standard Library */
#include <stdint.h>

/* SPURS */
#include <cell/spurs/types.h>				/* CellSpurs */
#include <cell/spurs/task_types.h>			/* CellSpursTaskset */
#include <cell/spurs/event_flag_types.h>	/* CellSpursEventFlag */
#include <cell/spurs/error.h>

static int cellSpursEventFlagInitializeIWL(CellSpurs*, CellSpursEventFlag*, CellSpursEventFlagClearMode clearMode, CellSpursEventFlagDirection direction);

static int cellSpursEventFlagInitialize(CellSpursTaskset*, CellSpursEventFlag*, CellSpursEventFlagClearMode clearMode, CellSpursEventFlagDirection direction);

#ifdef __cplusplus
extern "C" {
#endif

int cellSpursEventFlagAttachLv2EventQueue(CellSpursEventFlag*);
int cellSpursEventFlagDetachLv2EventQueue(CellSpursEventFlag*);
int cellSpursEventFlagSet(CellSpursEventFlag*, uint16_t bits);
int cellSpursEventFlagClear(CellSpursEventFlag*, uint16_t bits);
int cellSpursEventFlagWait(CellSpursEventFlag*, uint16_t *bits, CellSpursEventFlagWaitMode mode);
int cellSpursEventFlagTryWait(CellSpursEventFlag*, uint16_t *bits, CellSpursEventFlagWaitMode mode);
int cellSpursEventFlagGetDirection(const CellSpursEventFlag*, CellSpursEventFlagDirection*);
int cellSpursEventFlagGetClearMode(const CellSpursEventFlag*, CellSpursEventFlagClearMode*);
int cellSpursEventFlagGetTasksetAddress(const CellSpursEventFlag*, CellSpursTaskset**);

int _cellSpursEventFlagInitialize (CellSpurs* spurs, CellSpursTaskset* taskset, CellSpursEventFlag* eventFlag, CellSpursEventFlagClearMode clearMode, CellSpursEventFlagDirection direction);

#ifdef __cplusplus
} /* extern "C" */
#endif

static inline int
cellSpursEventFlagInitializeIWL(CellSpurs* spurs, CellSpursEventFlag* eventFlag, CellSpursEventFlagClearMode clearMode, CellSpursEventFlagDirection direction)
{
	return _cellSpursEventFlagInitialize (spurs, 0, eventFlag, clearMode, direction);
}

static inline int
cellSpursEventFlagInitialize(CellSpursTaskset* taskset, CellSpursEventFlag* eventFlag, CellSpursEventFlagClearMode clearMode, CellSpursEventFlagDirection direction)
{
	return _cellSpursEventFlagInitialize (0, taskset, eventFlag, clearMode, direction);
}


#endif /* __CELL_SPURS_EVENT_FLAG_H__ */

/*
 * Local Variables:
 * mode: C
 * c-file-style: "stroustrup"
 * tab-width: 4
 * End:
 * vim:sw=4:sts=4:ts=4
 */
