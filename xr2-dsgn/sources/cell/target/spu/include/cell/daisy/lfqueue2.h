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
#include <cell/daisy/error.h>

extern "C" {

	int _cellSyncLFQueueGetPushPointer(uint64_t ea, int *pPointer,
									   int (*fpWaitSignal)(void), uint32_t (*fpGetId)(void));
}

namespace cell {
	namespace Daisy {

		static inline
		int LFQueue2Initialize(uint64_t ea,
							   unsigned int depth,
							   uint64_t eaSignal)
		{
			int ret = cellSyncLFQueueInitialize(ea, 16, 0, depth, (CellSyncQueueDirection)0, eaSignal);
			return CELL_ERROR_IMPORT_SYNC_ERROR(ret);
		}

		static inline
		int LFQueue2GetPushPointer(uint64_t ea,
								   int *pPointer,
								   int (*fpWaitSignal)(void),
								   uint32_t (*fpGetId)(void))
		{
			int ret = _cellSyncLFQueueGetPushPointer(ea, pPointer, fpWaitSignal, fpGetId);
			return CELL_ERROR_IMPORT_SYNC_ERROR(ret);
		}

		extern int LFQueue2CompletePushPointer(uint64_t ea,
											   int pointer,
											   int (*fpSendSignal)(uint64_t, uint32_t),
											   unsigned int noQueueFull);

		extern int LFQueue2GetPopPointer(uint64_t ea,
										 int *pPointer,
										 int (*fpWaitSignal)(void),
										 uint32_t (*fpGetId)(void));

		extern int LFQueue2CompletePopPointer(uint64_t ea,
											  int pointer,
											  int (*fpSendSignal)(uint64_t, uint32_t),
											  unsigned int noQueueFull);

		extern int LFQueue2SetPushPointer(uint64_t ea,
										  int pointer,
										  int (*fpWaitSignal)(void),
										  uint32_t (*fpGetId)(void));

		extern void LFQueue2PushOpen(uint64_t ea);
		extern int LFQueue2PushClose(uint64_t ea,
									 int (*fpSendSignal)(uint64_t, uint32_t));
		extern void LFQueue2PopOpen(uint64_t ea);
		extern int LFQueue2PopClose(uint64_t ea,
									int (*fpSendSignal)(uint64_t, uint32_t));
		extern int LFQueue2HasUnfinishedConsumer(uint64_t ea, unsigned int isCancelled,
												 int (*fpWaitSignal)(void),
												 uint32_t (*fpGetId)(void));

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
