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
extern CellSync2Notifier gCellSync2NotifierPpuThread;

/* PPU Fiber */
extern CellSync2Notifier gCellSync2NotifierPpuFiber;

/* SPURS Task */
extern CellSync2CallerThreadType gCellSync2CallerThreadTypeSpursTask;
extern CellSync2Notifier gCellSync2NotifierSpursTask;
extern CellSync2ThreadConfig gCellSync2ThreadConfigSpursTask;

/* SPURS Job */
extern CellSync2CallerThreadType gCellSync2CallerThreadTypeSpursJob;
extern CellSync2Notifier gCellSync2NotifierSpursJob;
extern CellSync2ThreadConfig gCellSync2ThreadConfigSpursJob;

/* SPURS JobQueue Job */
extern CellSync2CallerThreadType gCellSync2CallerThreadTypeSpursJobQueueJob;
extern CellSync2Notifier gCellSync2NotifierSpursJobQueueJob;
extern CellSync2ThreadConfig gCellSync2ThreadConfigSpursJobQueueJob;


#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* __CELL_SYNC2_THREAD_H__ */
