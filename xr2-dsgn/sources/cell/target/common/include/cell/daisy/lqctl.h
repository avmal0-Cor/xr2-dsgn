/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2006 Sony Computer Entertainment Inc.
 * All Rights Reserved.
 */

/* QueueControl::Local - Local synchronization queue control
 */

#ifndef __CELL_DAISY_LQCTL_H__
#define __CELL_DAISY_LQCTL_H__

#include <cell/daisy/qctl.h>
#include <cell/daisy/lock.h>

#undef CELL_DAISY_DEBUG_PRINTF
#define CELL_DAISY_DEBUG_PRINTF(...)
#ifdef CELL_DAISY_DEBUG_LQCTL
#include <cell/daisy/daisy_debug.h>
#endif

namespace cell {
	namespace Daisy {
		namespace QueueControl {
    
			template<SizeType tSize, QueueIO tQueueIO>
			class Local:public Abstract<tSize, tQueueIO> {
			private:

				/* static assertion */
				static int sAssertDummy[
					(tSize < 0x8000)
					? 1 : -1];

			protected:
				Lock&            mLock           __CELL_DAISY_SPU_ALIGN16__;
#ifdef __SPU__
				/* dummy for padding */
				uint32_t         mDummy          __CELL_DAISY_SPU_ALIGN16__; 
#endif

			public:
      
				__attribute__((always_inline)) inline
				unsigned int isOutOfOrder() {
					if (tQueueIO == INPUT) {
						return (mLock.prodRefCount > 1);
					} else {
						return (mLock.consRefCount > 1);
					}
				}

				static const QueueControlType sQueueControlType = QCTL_TYPE_LOCAL;
 			    static const SizeType sQueueMaxSize = 32767;

				/* constructor/destructor definition */
				explicit Local(){}
				explicit Local(Lock& lock);
				virtual ~Local() {}

				uint32_t  getCompleteNumber() {
					return CELL_DAISY_ERROR_NOT_IMPLEMENTED;
				}

				virtual const char *getClassName() {
#ifdef CELL_DAISY_DEBUG_LQCTL
					static char __buf[64];
					cell::Daisy::_snprintf(__buf, 64, "Local(%s)[mLock:%p,tSize:%d]",
										   tQueueIO == INPUT ? "In" : "Out",
										   &mLock,
										   tSize);
					return (const char *)__buf;
#else
					return 0;
#endif
				}
      
				virtual void dump(const char *);
      
				__attribute__((always_inline)) inline
					PointerType getPointerFromSequenceNumber(int sequenceNumber)
					{
						if (tSize == 1 || tSize == 2 || tSize == 4 || tSize == 8 ||
							tSize == 16 || tSize == 32) {
							return (sequenceNumber + 2*tSize) % (2*tSize);
						} else {
							if (__builtin_expect(sequenceNumber < 2*(int)tSize && sequenceNumber >= 0, 1)) {
								return sequenceNumber;
							} else {
								return (sequenceNumber + 2*tSize) % (2*tSize);
							}
						}
					}

				/* Consumer packet#=sequenceNumber completed */
				void endCompleteConsume(uint32_t sequenceNumber, unsigned int noQueueFull = false) ;
      
				/* Producer packet#=sequenceNumber completed */
				void endCompleteProduce(uint32_t sequenceNumber, unsigned int noQueueFull = false);
      
				/* Get next Tail pointer */
				PointerType getNextTailPointer(BlockMode stall);
      
				/* Get next Head pointer */
				PointerType getNextHeadPointer(BlockMode stall);
      
				/* Terminate produce */
				int terminate();

			};
    
			/* constructor descriptions */

			template<SizeType tSize, QueueIO tQueueIO>
			inline Local<tSize, tQueueIO>::Local(Lock& lock):
				Abstract<tSize, tQueueIO> (),
				mLock                     (lock)
			{
				cellDaisyAssert((uintptr_t)&lock);
				int ret = mLock.initialize(tSize);
				cellDaisyAssert(ret == CELL_OK);
				if (tQueueIO == INPUT) {
					mLock.pushOpen();
				}else {
					mLock.popOpen();
				}
			}

			/* Member functions description(s) */
    
			template<SizeType tSize, QueueIO tQueueIO>
			inline void Local<tSize, tQueueIO>::dump(const char *messages)
			{
#ifndef CELL_DAISY_DEBUG_LQCTL
				(void)messages;
#else
      
				static char __dump_buf[256];
				const static char *stateStr[3] = {"VOID","STARTED","PUSH_STARTED"};
      
				cell::Daisy::_snprintf(__dump_buf, 256,
									   "%s,depth=%d,state=%s,refCount=(P:%d,C:%d),head=(%d,%s,la=%d),tail=(%d,%s,la=%d)\n",
									   tQueueIO==INPUT?"INPUT":"OUTPUT",
									   mLock.depth,
									   stateStr[mLock.queueState],
									   mLock.prodRefCount,
									   mLock.consRefCount,
#ifdef __SPU__
									   (unsigned int)spu_extract((vec_ushort8)mLock.head, 0),
									   getMaskString((unsigned int)spu_extract((vec_ushort8)mLock.head, 1), 16),
									   spu_extract((vec_uint4)mLock.head, 1),
									   (unsigned int)spu_extract((vec_ushort8)mLock.tail, 0),
									   getMaskString((unsigned int)spu_extract((vec_ushort8)mLock.tail, 1), 16),
									   spu_extract((vec_uint4)mLock.tail, 1)
#else /* __SPU__ */
									   (unsigned int)(mLock.head >> 48),
									   getMaskString((mLock.head >> 32) & 0xffff),
									   (unsigned int)(mLock.head & 0xffffffff),
									   (unsigned int)(mLock.tail >> 48),
									   getMaskString((mLock.tail >> 32) & 0xffff),
									   (unsigned int)(mLock.tail & 0xffffffff)
#endif /* __SPU__ */
					);
				strcat(__dump_buf, messages);
				CELL_DAISY_DEBUG_PRINTF("%s %s\n", getClassName(), __dump_buf);
#endif
			}
    
			template<SizeType tSize, QueueIO tQueueIO>
			__attribute__((always_inline)) inline
				PointerType Local<tSize, tQueueIO>::getNextHeadPointer(BlockMode stall)
			{
				(void)stall;
				PointerType pointer;
				pointer = mLock.getNextHeadPointer();
				CELL_DAISY_DEBUG_PRINTF("%s(getNextHeadPointer): mHeadPointer=%d\n", getClassName(), pointer);
#ifdef CELL_DAISY_DEBUG_LQCTL
				dump("");
#endif

				this->setPointer(pointer);
				return pointer;
			}
    
			template<SizeType tSize, QueueIO tQueueIO>
			__attribute__((always_inline)) inline
				PointerType Local<tSize,tQueueIO>::getNextTailPointer(BlockMode stall)
			{
				(void)stall;
				PointerType pointer;
				pointer = mLock.getNextTailPointer();
				CELL_DAISY_DEBUG_PRINTF("%s(getNextTailPointer): mTailPointer=%d\n", getClassName(), pointer);
#ifdef CELL_DAISY_DEBUG_LQCTL
				dump("");
#endif

				this->setPointer(pointer);
				return pointer;
 			}
    
			template<SizeType tSize, QueueIO tQueueIO>
			__attribute__((always_inline)) inline
				void Local<tSize, tQueueIO>::endCompleteConsume(uint32_t sequenceNumber, unsigned int noQueueFull)
			{
				(void)noQueueFull;
				mLock.completeConsume(sequenceNumber);
				CELL_DAISY_DEBUG_PRINTF("%s(endCompleteConsume): sequenceNumber=%d\n", getClassName(), sequenceNumber);
			}
    
			template<SizeType tSize, QueueIO tQueueIO>
			__attribute__((always_inline)) inline
				void Local<tSize, tQueueIO>::endCompleteProduce(uint32_t sequenceNumber, unsigned int noQueueFull)
			{
				(void)noQueueFull;
				mLock.completeProduce(sequenceNumber);
				CELL_DAISY_DEBUG_PRINTF("%s(endCompleteProduce): sequenceNumber=%d\n", getClassName(), sequenceNumber);
			}

			/* Terminate produce */
			template<SizeType tSize, QueueIO tQueueIO>
			__attribute__((always_inline)) inline
				int Local<tSize, tQueueIO>::terminate()
			{
				CELL_DAISY_DEBUG_PRINTF("%s(terminate): terminate sent\n", getClassName());
				if (tQueueIO == INPUT) {
					mLock.pushClose();
				} else {
					mLock.popClose();
				}
				return CELL_OK;
			}

		} /* namespace QueueControl */    
	} /* namespace Daisy */
} /* namespace cell */

#endif /* __CELL_DAISY_LQCTL_H__ */

/*
 * Local Variables:
 * mode:C++
 * tab-width:4
 * End:
 * vim:ts=4:sw=4:
 */
