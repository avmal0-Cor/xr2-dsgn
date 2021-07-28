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
/* Lv2 */
#include <sys/event.h>

namespace cell {
	namespace Daisy {
    
		class ScatterGatherInterlock {

		private:

			AtomicInterlock *mpAtomic;
			SizeType         mSize;
			bool             mIsProbe;
			void            *mEaSignal;
			sys_event_port_t mEventPort;
			unsigned int     mNumSpus;
			sys_spu_thread_t mIds[8];
			int            (*mfpSendSignal)(void *, uint32_t);

		public:
      
			explicit ScatterGatherInterlock(AtomicInterlock *ea, SizeType size,
											void *eaSignal,
											int (*fpSendSignal)(void *, uint32_t));

			explicit ScatterGatherInterlock(AtomicInterlock *ea, SizeType size,
											sys_spu_thread_t *ids,
											unsigned int numSpus,
											uint8_t spup);

			~ScatterGatherInterlock();
			
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
