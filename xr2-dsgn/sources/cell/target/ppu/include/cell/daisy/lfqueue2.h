/* SCE CONFIDENTIAL
 PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2006 Sony Computer Entertainment Inc.
 * All Rights Reserved.
 */

#ifndef __CELL_DAISY_LFQUEUE2_H__
#define __CELL_DAISY_LFQUEUE2_H__ 1

/* The C Standard Library */
#include <stdint.h>

#include <cell/sync/lfqueue.h>
#include <cell/daisy/lfqueue2_types.h>

extern "C" {

	int _cellSyncLFQueueGetPushPointer(CellSyncLFQueue *pQueue, int *pPointer, unsigned int isBlocking,
									   unsigned int useEventQueue);
	int _cellSyncLFQueueCompletePushPointer(CellSyncLFQueue *pQueue, int pointer,
											int (*fpSendSignal)(void*, uint32_t));
}

namespace cell {
	namespace Daisy {

		static inline
		int LFQueue2Initialize(LFQueue2 *lfqueue,
							   unsigned int depth,
							   CellSyncQueueDirection dir,
							   void *pSignal)
		{
			int ret = cellSyncLFQueueInitialize((CellSyncLFQueue *)lfqueue, (void *)16, 0, depth, dir, pSignal);
			return CELL_ERROR_IMPORT_SYNC_ERROR(ret);
		}


		static inline
		int LFQueue2GetPushPointer(LFQueue2 *pQueue, int *pPointer, unsigned int isBlocking)
		{
			int ret = _cellSyncLFQueueGetPushPointer((CellSyncLFQueue *)pQueue, pPointer, isBlocking, true);
			return CELL_ERROR_IMPORT_SYNC_ERROR(ret);
		}

		static inline
		int LFQueue2CompletePushPointer(LFQueue2 *pQueue, int pointer,
										int (*fpSendSignal)(void*, uint32_t))
		{
			int ret = _cellSyncLFQueueCompletePushPointer((CellSyncLFQueue *)pQueue, pointer, fpSendSignal);
			return CELL_ERROR_IMPORT_SYNC_ERROR(ret);
		}

		extern int LFQueue2GetPopPointer(LFQueue2 *ea,
										 int *pPointer,
										 unsigned int isBlocking);

		extern int LFQueue2CompletePopPointer(LFQueue2 *ea,
											  int pointer,
											  int (*fpSendSignal)(void*, uint32_t),
											  unsigned int noQueueFull);

		extern void LFQueue2PushOpen(LFQueue2 *ea);
		extern int LFQueue2PushClose(LFQueue2 *ea,
									 int (*fpSendSignal)(void*, uint32_t));
		extern void LFQueue2PopOpen(LFQueue2 *ea);
		extern int LFQueue2PopClose(LFQueue2 *ea,
									int (*fpSendSignal)(void*, uint32_t));
		extern int LFQueue2HasUnfinishedConsumer(LFQueue2 *ea, unsigned int isCancelled);

	} /* namespace Daisy */
} /* namespace cell */

#endif /* __CELL_DAISY_LFQUEUE2_H__ */

/*
 * Local Variables:
 * mode: C++
 * c-file-style: "stroustrup"
 * tab-width: 4
 * End:
 * vim:sw=4:sts=4:ts=4
 */
