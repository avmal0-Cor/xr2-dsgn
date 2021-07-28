/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2006 Sony Computer Entertainment Inc.
 * All Rights Reserved.
 */

/* sync_pipe_interlock.h
 */

#ifndef __CELL_DAISY_SYNC_PIPE_INTERLOCK_TYPES_H__
#define __CELL_DAISY_SYNC_PIPE_INTERLOCK_TYPES_H__

namespace cell {
	namespace Daisy {
		typedef volatile struct _AtomicInterlock {
			uint8_t skip[128];
		} __attribute__((aligned(128))) AtomicInterlock;

	} /* namespace Daisy */
} /* namespace cell */

#endif /* __CELL_DAISY_SYNC_PIPE_INTERLOCK_TYPES_H__ */

/*
 * Local Variables:
 * mode:C++
 * tab-width:4
 * End:
 * vim:ts=4:sw=4:
 */
