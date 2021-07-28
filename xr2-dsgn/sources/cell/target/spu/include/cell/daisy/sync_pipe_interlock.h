/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2006 Sony Computer Entertainment Inc.
 * All Rights Reserved.
 */

/* sync_pipe_interlock.h
 */

#ifndef __CELL_DAISY_SYNC_PIPE_INTERLOCK_H__
#define __CELL_DAISY_SYNC_PIPE_INTERLOCK_H__

#include <cell/daisy/daisy_defs.h>
#include <cell/daisy/sync_pipe_interlock_types.h>

namespace cell {
	namespace Daisy {
    
		class ScatterGatherInterlock {

		private:

			AtomicInterlock *mpAtomic;
			uint64_t         mEa;
			SizeType         mSize;
			bool             mIsProbe;
			uint64_t         mEaSignal;
			uint32_t       (*mfpGetId)(void);
			int            (*mfpWaitSignal)(void);
			int            (*mfpSendSignal)(uint64_t, uint32_t);

			uint8_t mAtomicBuffer[128+127];

		public:
      
			/* constructor definition */
      
			explicit ScatterGatherInterlock(uint64_t ea, SizeType size,
											uint64_t eaSignal,
											int (*fpSendSignal)(uint64_t, uint32_t));

			explicit ScatterGatherInterlock(uint64_t ea, SizeType size,
											uint64_t eaSignal,
											uint32_t (*fpGetId)(void),
											int (*fpWaitSignal)(void));
												
			virtual ~ScatterGatherInterlock(){}
      
			int probe(unsigned int isBlocking);
			int release();
			void proceedSequenceNumber();

		};

	} /* namespace Daisy */
} /* namespace cell */

#endif /* __CELL_DAISY_SYNC_PIPE_INTERLOCK_H__ */

/*
 * Local Variables:
 * mode:C++
 * tab-width:4
 * End:
 * vim:ts=4:sw=4:
 */
