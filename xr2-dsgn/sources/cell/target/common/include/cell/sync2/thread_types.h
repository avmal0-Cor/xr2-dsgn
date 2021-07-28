/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2009 Sony Computer Entertainment Inc.
 * All Rights Reserved.
 */

#ifndef __CELL_SYNC2_THREAD_TYPES_H__
#define __CELL_SYNC2_THREAD_TYPES_H__

#include <stdint.h>
#include <cell/sync2/error.h>

#define CELL_SYNC2_WAITING_QUEUE_ALIGN	8

#define	CELL_SYNC2_THREAD_TYPE_INVALID 		0
#define	CELL_SYNC2_THREAD_TYPE_PPU_THREAD		(1U << 0)
#define	CELL_SYNC2_THREAD_TYPE_PPU_FIBER		(1U << 1)
#define	CELL_SYNC2_THREAD_TYPE_SPURS_TASK		(1U << 2)
#define	CELL_SYNC2_THREAD_TYPE_SPURS_JOBQUEUE_JOB	(1U << 3)
#define	CELL_SYNC2_THREAD_TYPE_USER_DEFINED_BLOCKABLE0	(1U << 6)	// For a blockable thread
#define	CELL_SYNC2_THREAD_TYPE_USER_DEFINED_BLOCKABLE1	(1U << 7)	// For a blockable thread
#define	CELL_SYNC2_THREAD_TYPE_SPURS_JOB			(1U << 8)
#define	CELL_SYNC2_THREAD_TYPE_USER_DEFINED_UNBLOCKABLE0	(1U << 14)	// For a unblockable thread
#define	CELL_SYNC2_THREAD_TYPE_USER_DEFINED_UNBLOCKABLE1	(1U << 15)	// For a unblockable thread

typedef enum CellSync2ObjectTypeId {
	CELL_SYNC2_OBJECT_TYPE_MUTUEX = 1,
	CELL_SYNC2_OBJECT_TYPE_COND,
	CELL_SYNC2_OBJECT_TYPE_QUEUE,
	CELL_SYNC2_OBJECT_TYPE_SEMAPHORE,
} CellSync2ObjectTypeId;

typedef uint16_t CellSync2ThreadTypeId;
typedef uint64_t CellSync2ThreadId;
typedef uint64_t CellSync2SignalReceiverId;

typedef struct CellSync2CallerThreadType {
	CellSync2ThreadTypeId threadTypeId;
	CellSync2ThreadId (*self)(uint64_t);
	int (*waitSignal)(CellSync2SignalReceiverId, CellSync2ObjectTypeId, uint64_t, uint64_t);
	int (*allocateSignalReceiver)(CellSync2SignalReceiverId*, CellSync2ObjectTypeId, uint64_t, uint64_t);
	int (*freeSignalReceiver)(CellSync2SignalReceiverId, uint64_t);
	unsigned int spinWaitNanoSec;
	uint64_t callbackArg;
} CellSync2CallerThreadType;

typedef struct CellSync2Notifier {
	CellSync2ThreadTypeId threadTypeId;
	int (*sendSignal)(CellSync2SignalReceiverId, uint64_t);
	uint64_t callbackArg;
} CellSync2Notifier;

typedef struct CellSync2ThreadConfig {
	CellSync2CallerThreadType* callerThreadType;
	CellSync2Notifier** notifierTable;
	unsigned int numNotifier;
} CellSync2ThreadConfig;

#endif /* __CELL_SYNC2_THREAD_TYPES_H__ */
