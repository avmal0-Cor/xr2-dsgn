/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2009 Sony Computer Entertainment Inc.
 * All Rights Reserved.
 */

#ifndef __CELL_SYNC2_THREAD_H__
#define __CELL_SYNC2_THREAD_H__

#include <cell/sync2/thread_types.h>

#ifdef __cplusplus
extern "C" {
#endif

/* PPU Thread */
extern CellSync2CallerThreadType gCellSync2CallerThreadTypePpuThread;
extern CellSync2Notifier gCellSync2NotifierPpuThread;

/* PPU Fiber */
extern CellSync2CallerThreadType gCellSync2CallerThreadTypePpuFiber;
extern CellSync2Notifier gCellSync2NotifierPpuFiber;

/* SPURS Task */
extern CellSync2Notifier gCellSync2NotifierSpursTask;

/* SPURS JobQueue Job */
extern CellSync2Notifier gCellSync2NotifierSpursJobQueueJob;

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* __CELL_SYNC2_THREAD_H__ */
