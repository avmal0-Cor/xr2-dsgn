/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2007 Sony Computer Entertainment Inc.
 * All Rights Reserved.
 */

#ifndef __CELL_SPURS_EVENT_FLAG_H__
#define __CELL_SPURS_EVENT_FLAG_H__ 1

/* The C Standard Library */
#include <stdint.h>

/* SPURS */
#include <cell/spurs/event_flag_types.h>
#include <cell/spurs/error.h>

#ifdef __cplusplus
extern "C" {
#endif

int _cellSpursEventFlagInitialize(uint64_t ea, CellSpursEventFlagClearMode clearMode, CellSpursEventFlagDirection direction, unsigned isIWL);
int cellSpursEventFlagSet(uint64_t ea, uint16_t bits);
int cellSpursEventFlagClear(uint64_t ea, uint16_t bits);
int _cellSpursEventFlagWait(uint64_t ea, uint16_t* bits, CellSpursEventFlagWaitMode mode, unsigned isBlocking);
int cellSpursEventFlagGetDirection(uint64_t ea, CellSpursEventFlagDirection* direction);
int cellSpursEventFlagGetClearMode(uint64_t ea, CellSpursEventFlagClearMode* clear_mode);
int cellSpursEventFlagGetTasksetAddress(uint64_t ea, uint64_t* pEaTaskset);

#ifdef __cplusplus
} /* extern "C" */
#endif

#define cellSpursEventFlagInitialize(ea,mode,direction) \
		_cellSpursEventFlagInitialize((ea),(mode),(direction),0)
#define cellSpursEventFlagInitializeIWL(ea,mode,direction) \
		_cellSpursEventFlagInitialize((ea),(mode),(direction),1)

#define cellSpursEventFlagTryWait(ea,bits,mode) \
		_cellSpursEventFlagWait((ea),(bits),(mode),0)
#define cellSpursEventFlagWait(ea,bits,mode) \
		_cellSpursEventFlagWait((ea),(bits),(mode),1)

#endif /* __CELL_SPURS_EVENT_FLAG_H__ */

/*
 * Local Variables:
 * mode: C
 * c-file-style: "stroustrup"
 * tab-width: 4
 * End:
 * vim:sw=4:sts=4:ts=4
 */
