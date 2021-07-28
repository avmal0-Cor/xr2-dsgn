/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2008 Sony Computer Entertainment Inc.
 * All Rights Reserved.
 */

#include "task_types.h"

#ifndef __CELL_SPURS_TASK1_TYPES_CPP_H__
#define __CELL_SPURS_TASK1_TYPES_CPP_H__ 1

namespace cell {
	namespace Spurs {
		class Taskset : public CellSpursTaskset
		{
		public:
			static const uint32_t kSize  = CELL_SPURS_TASKSET_SIZE;
			static const uint32_t kAlign = CELL_SPURS_TASKSET_ALIGN;

#ifdef __PPU__
			inline static int createWithAttribute (CellSpurs* spurs, CellSpursTaskset* taskset, const CellSpursTasksetAttribute* attr);
			inline static int create(CellSpurs* spurs , CellSpursTaskset* taskset, uint64_t argTaskset, const uint8_t priority[8], unsigned int max_contention);
			inline int shutdown(void);
			inline int join(void);
			inline static int lookUpTasksetAddress(CellSpurs *spurs, CellSpursTaskset **taskset, CellSpursWorkloadId id);
			  
			inline int getTasksetId(CellSpursWorkloadId* wid) const;
			inline int getSpursAddress(CellSpurs** ppSpurs) const ;
			inline int createTaskWithAttribute (CellSpursTaskId* tid,	const CellSpursTaskAttribute* attr);
			inline int createTask(CellSpursTaskId* tid, const void *eaElf,	const void *eaContext,
								  uint32_t sizeContext, const CellSpursTaskLsPattern* lspattern, const CellSpursTaskArgument* arg);
			inline int sendSignal(CellSpursTaskId id);
			inline int setExceptionEventHandler(CellSpursTasksetExceptionEventHandler handler,	void* arg);
			inline int unsetExceptionEventHandler(void);
			inline int getTasksetInfo(CellSpursTasksetInfo* ) const;
			
			
#endif /* __PPU__ */
		};
	}
}


#endif /* __CELL_SPURS_TASK_TYPES_CPP_H__ */

/*
 * Local Variables:
 * mode: C++
 * c-file-style: "stroustrup"
 * tab-width: 4
 * End:
 * vim:sw=4:sts=4:ts=4
 */
