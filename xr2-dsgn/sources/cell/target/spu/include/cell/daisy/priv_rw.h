/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2007 Sony Computer Entertainment Inc.
 * All Rights Reserved.
 */

/* QueueControl::PrivateMemoryReader - private memory reader
 * QueueControl::PrivateMemoryWriter - private memory writer
 */

#ifndef __CELL_DAISY_PRIV_RW_H__
#define __CELL_DAISY_PRIV_RW_H__

#include <cell/daisy/lqctl.h>

#undef CELL_DAISY_DEBUG_PRINTF
#define CELL_DAISY_DEBUG_PRINTF(...)
#ifdef CELL_DAISY_DEBUG_PRIV_RW
#include <cell/daisy/daisy_debug.h>
#endif

namespace cell {
	namespace Daisy {
		namespace QueueControl {
    
			template<SizeType tSize>
			class PrivateMemoryReader:public Abstract<tSize, OUTPUT> {

			protected:

				/* flag indicating queue is already terminated */
				qword       mIsTerminated;

				PointerType mHeadPointer __attribute__((aligned(16)));

				int mDummy               __attribute__((aligned(16)));

			public:
      
				static const QueueControlType sQueueControlType = QCTL_TYPE_LOCAL;
 			    static const SizeType sQueueMaxSize = 0xffffffff;

				virtual const char *getClassName() {
#ifdef CELL_DAISY_DEBUG_PRIV_RW
					static char __buf[64];
					cell::Daisy::_snprintf(__buf, 64, "PrivateMemoryReader[tSize:%d]", tSize);
					return (const char *)__buf;
#else
					return 0;
#endif
				}
      
				/* constructor definition */
				explicit PrivateMemoryReader():
					mIsTerminated (si_from_uint(0)),
					mHeadPointer  ((uint32_t)-1)
					{}

				__attribute__((always_inline)) inline
				void dump(const char *messages = 0) {
#ifndef CELL_DAISY_DEBUG_PRIV_RW_QCTL
					(void)messages;
#else
					static char __dump_buf[256];
					
					cell::Daisy::_snprintf(__dump_buf, 256, "head=%d\n", mHeadPointer);
					if (messages != 0) strcat(__dump_buf, messages);
					CELL_DAISY_DEBUG_PRINTF("%s %s\n", getClassName(), __dump_buf);
#endif
				}

				/* Get next Head pointer */
				__attribute__((always_inline)) inline
				PointerType getNextHeadPointer(BlockMode stall) {
					(void)stall;
					
					if (__builtin_expect(si_to_uint(mIsTerminated) != 0u, 0)) {
						return PTR_TERMINATED;
					}
					mHeadPointer++;
					this->setPointer(mHeadPointer);
					if (__builtin_expect((uint32_t)mHeadPointer == tSize-1, 0)) {
						mIsTerminated = si_from_uint(1);
					}
					
					CELL_DAISY_DEBUG_PRINTF("%s(getNextTailPointer): mPointer=%d\n",
											getClassName(),this->getPointer()
						);
					this->dump();
					
					return mHeadPointer;
				}
      
			};
    
			template<SizeType tSize>
			class PrivateMemoryWriter:public Abstract<tSize, INPUT> {

			protected:

				PointerType mTailPointer __attribute__((aligned(16)));

				int mDummy               __attribute__((aligned(16)));

			public:
      
				static const QueueControlType sQueueControlType = QCTL_TYPE_LOCAL;
 			    static const SizeType sQueueMaxSize = 0xffffffff;

				virtual const char *getClassName() {
#ifdef CELL_DAISY_DEBUG_PRIV_RW
					static char __buf[64];
					cell::Daisy::_snprintf(__buf, 64, "PrivateMemoryWriter[tSize:%d]", tSize);
					return (const char *)__buf;
#else
					return 0;
#endif
				}
      
				/* constructor definition */
				explicit PrivateMemoryWriter():
					mTailPointer  ((uint32_t)-1)
					{}

				__attribute__((always_inline)) inline
				void dump(const char *messages = 0) {
#ifndef CELL_DAISY_DEBUG_PRIV_RW_QCTL
					(void)messages;
#else
					static char __dump_buf[256];
					
					cell::Daisy::_snprintf(__dump_buf, 256, "tail=%d\n", mTailPointer);
					if (messages != 0) strcat(__dump_buf, messages);
					CELL_DAISY_DEBUG_PRINTF("%s %s\n", getClassName(), __dump_buf);
#endif
				}

				/* Get next Tail pointer */
				PointerType getNextTailPointer(BlockMode stall) {
					(void)stall;
					
					mTailPointer++;
					this->setPointer(mTailPointer);

					cellDaisyAssert((uint32_t)mTailPointer < tSize);

					CELL_DAISY_DEBUG_PRINTF("%s(getNextTailPointer): mPointer=%d\n",
											getClassName(),this->getPointer()
						);
					this->dump();
					
					return mTailPointer;
				}
      
			};
    
		} /* namespace QueueControl */    
	} /* namespace Daisy */
} /* namespace cell */

#endif /* __CELL_DAISY_PRIV_RW_H__ */

/*
 * Local Variables:
 * mode:C++
 * tab-width:4
 * End:
 * vim:ts=4:sw=4:
 */
