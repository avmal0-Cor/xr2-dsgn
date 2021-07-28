/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2006 Sony Computer Entertainment Inc.
 * All Rights Reserved.
 */

/* lock.h
 * lock type definition
 */

#ifndef __CELL_DAISY_LOCK_H__
#define __CELL_DAISY_LOCK_H__

#include "cell/daisy/daisy_defs.h"

namespace cell {
	namespace Daisy {

		struct Lock {
#ifdef __SPU__
			qword head;
			qword tail;
#else
			uint64_t head, tail;
#endif
			uint32_t depth, queueState, prodRefCount, consRefCount;

			int initialize(unsigned int depth);
			int getNextHeadPointer();
			int getNextTailPointer();
			void completeConsume(uint32_t pointer);
			void completeProduce(uint32_t pointer);
			void pushOpen();
			void pushClose();
			void popOpen();
			void popClose();
		};
    
	} /* namespace Daisy */
} /* namespace cell */

#endif /* __CELL_DAISY_LOCK_H__ */

/*
 * Local Variables:
 * mode:C++
 * tab-width:4
 * End:
 * vim:ts=4:sw=4:
 */
