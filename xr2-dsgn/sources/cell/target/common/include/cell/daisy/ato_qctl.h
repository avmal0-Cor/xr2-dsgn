/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2006 Sony Computer Entertainment Inc.
 * All Rights Reserved.
 */

/* QueueControl::Atomic - Atomic synchronization queue control
 */

#ifndef __CELL_DAISY_ATO_QCTL_H__
#define __CELL_DAISY_ATO_QCTL_H__

#include <cell/daisy/qctl.h>
#include <cell/daisy/lfqueue2.h>
#ifdef __PPU__
extern "C" {
int _cellSyncLFQueueAttachLv2EventQueue(sys_spu_thread_t *ids, unsigned int numSpus,
										CellSyncLFQueue *pQueue);
int _cellSyncLFQueueDetachLv2EventQueue(sys_spu_thread_t *ids, unsigned int numSpus,
										CellSyncLFQueue *pQueue);
}
#endif

/* Lv2 */
#include <sys/spu_thread.h>

#undef CELL_DAISY_DEBUG_PRINTF
#define CELL_DAISY_DEBUG_PRINTF(...)
#ifdef CELL_DAISY_DEBUG_ATO_QCTL
#include <cell/daisy/daisy_debug.h>
#endif

namespace cell {
	namespace Daisy {
		namespace QueueControl {
    
			template<SizeType tSize, QueueIO tQueueIO>
			class Atomic:public Abstract<tSize, tQueueIO> {
			private:

				/* static assertion */
				static int sAssertDummy[
					(tSize < 0x8000)
					? 1 : -1];

			protected:
#ifdef __PPU__
				LFQueue2        *mLFQueue;
				sys_spu_thread_t mIds[6];
				unsigned int     mNumSpus;
				void            *mSignal;
				int            (*mfpSendSignal)(void *, uint32_t);
#else
				uint64_t         mLFQueue                           __CELL_DAISY_SPU_ALIGN16__;
				uint64_t         mEaSignal                          __CELL_DAISY_SPU_ALIGN16__;
				uint32_t       (*mfpGetId)(void)                    __CELL_DAISY_SPU_ALIGN16__;
				int            (*mfpSendSignal)(uint64_t, uint32_t) __CELL_DAISY_SPU_ALIGN16__;
				int            (*mfpWaitSignal)(void)               __CELL_DAISY_SPU_ALIGN16__;
				unsigned         mIsInitializing                    __CELL_DAISY_SPU_ALIGN16__;
				/* dummy for padding */
				uint32_t         mDummy1                            __CELL_DAISY_SPU_ALIGN16__; 
#endif

			public:
      
				__attribute__((always_inline)) inline
				unsigned int isOutOfOrder() { return 1; }

				static const QueueControlType sQueueControlType = QCTL_TYPE_ATOMIC;
 			    static const SizeType sQueueMaxSize = 32767;

				/* dummy for padding */
				uint32_t mDummy2   __attribute__((aligned(16)));

				/* constructor definition */
#ifdef __PPU__
				explicit Atomic(){}
				explicit Atomic(
					LFQueue2 *pQueue,
					sys_spu_thread_t *ids,
					unsigned int numSpus,
					void *pSignal = 0,
					int (*fpSendSignal)(void *, uint32_t) = 0
					);

#else
				explicit Atomic(){}
				explicit Atomic(
					uint64_t lockEa,
					uint64_t eaSignal = 0,
					uint32_t (*fpGetId)(void) = 0,
					int (*fpSendSignal)(uint64_t, uint32_t) = 0,
					int (*fpWaitSignal)(void) = 0
					);
#endif

				/* virtual destructor definition */
				virtual ~Atomic();
      
				uint32_t  getCompleteNumber()
					{
						return CELL_DAISY_ERROR_NOT_IMPLEMENTED;
					}

				virtual const char *getClassName()
					{
#ifdef CELL_DAISY_DEBUG_ATO_QCTL
						static char __buf[64];
						cell::Daisy::_snprintf(__buf, 64, "Atomic(%s)[mLFQueue:0x%llx,tSize:%d]",
											   tQueueIO == INPUT ? "In" : "Out",
											   mLFQueue,
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
#ifdef __SPU__      
				/* Set next Tail pointer */
				int setNextTailPointer(BlockMode stall, PointerType pointer);
#endif /* __SPU__ */
				/* Get next Head pointer */
				PointerType getNextHeadPointer(BlockMode stall);
      
				/* Terminate produce */
				int terminate();

				/* Check if there are any unfinished consumers
				 * @param isCancelled true if last operation was cancel
				 * @retval true: unfinished consumers exist
				 */
				bool hasUnfinishedConsumer(bool isCancelled);

			};
    
#ifdef __PPU__ /* PPU */

			/* constructor descriptions */
			template<SizeType tSize, QueueIO tQueueIO>
			inline Atomic<tSize, tQueueIO>::Atomic(
				LFQueue2 *pQueue,
				sys_spu_thread_t *ids,
				unsigned int      numSpus,
				void             *pSignal,
				int             (*fpSendSignal)(void *, uint32_t)
				):
				Abstract<tSize, tQueueIO> (),
				mLFQueue                  (pQueue),
				mNumSpus                  (numSpus),
				mSignal                   (pSignal),
				mfpSendSignal             (fpSendSignal)
			{
				cellDaisyAssert(CELL_DAISY_EA_ATOMIC((uintptr_t)pQueue));

				int ret = LFQueue2Initialize(pQueue, tSize,
											 (tQueueIO == INPUT) ?
											 CELL_SYNC_QUEUE_PPU2SPU :
											 CELL_SYNC_QUEUE_SPU2PPU,
											 pSignal);
				cellDaisyAssert(ret == CELL_OK);
				for(unsigned i=0 ; i<numSpus ; i++) {
					mIds[i] = ids[i];
				}
				ret = _cellSyncLFQueueAttachLv2EventQueue(ids, numSpus, (CellSyncLFQueue *)pQueue);
				cellDaisyAssert(ret == CELL_OK);

				if (tQueueIO == INPUT) {
					LFQueue2PushOpen(mLFQueue);
				} else {
					LFQueue2PopOpen(mLFQueue);
				}
			}

			/* desstructor descriptions */
			template<SizeType tSize, QueueIO tQueueIO>
			inline Atomic<tSize, tQueueIO>::~Atomic()
			{
				int ret = _cellSyncLFQueueDetachLv2EventQueue(mIds, mNumSpus, (CellSyncLFQueue *)mLFQueue);
				cellDaisyAssert(ret == CELL_OK);
			}
#else /* SPU */
			/* constructor descriptions */
			template<SizeType tSize, QueueIO tQueueIO>
			inline Atomic<tSize, tQueueIO>::Atomic(
				uint64_t   lockEa,
				uint64_t   eaSignal,
				uint32_t (*fpGetId)(void),
				int      (*fpSendSignal)(uint64_t, uint32_t),
				int      (*fpWaitSignal)(void)
				):
				Abstract<tSize, tQueueIO> (),
				mLFQueue                   (lockEa),
				mEaSignal                 (eaSignal),
				mfpGetId                  (fpGetId),
				mfpSendSignal             (fpSendSignal),
				mfpWaitSignal             (fpWaitSignal),
				mIsInitializing           (false)
			{
				cellDaisyAssert(CELL_DAISY_EA_ATOMIC(lockEa));

				int ret = LFQueue2Initialize(lockEa, tSize, eaSignal);
				if (ret == CELL_DAISY_ERROR_BUSY) {
					mIsInitializing = true;
					return;
				}
				cellDaisyAssert(ret == CELL_OK);
				if (tQueueIO == INPUT) {
					LFQueue2PushOpen(mLFQueue);
				} else {
					LFQueue2PopOpen(mLFQueue);
				}
			}

			/* destructor descriptions */
			template<SizeType tSize, QueueIO tQueueIO>
			inline Atomic<tSize, tQueueIO>::~Atomic(){}
#endif /* __PPU __ */

			/* Member functions description(s) */
    
			template<SizeType tSize, QueueIO tQueueIO>
			void Atomic<tSize, tQueueIO>::dump(const char *messages)
			{
				(void)messages;
			}
    
			template<SizeType tSize, QueueIO tQueueIO>
			__attribute__((always_inline)) inline
			PointerType Atomic<tSize, tQueueIO>::getNextHeadPointer(BlockMode stall)
			{
#ifdef __SPU__
				if (__builtin_expect(mIsInitializing, 0)) {
					int ret = LFQueue2Initialize(mLFQueue, tSize, mEaSignal);
					if (ret) return ret;
					mIsInitializing = false;
				}
#endif /* __SPU__ */
				PointerType pointer = PTR_UNAVAILABLE;
				int ret, _pointer;
				do {
#ifdef __SPU__
					ret = LFQueue2GetPopPointer(mLFQueue, &_pointer,
												(stall == STALL) ? mfpWaitSignal : 0,
												mfpGetId);
					if (__builtin_expect(ret < 0 && mfpWaitSignal && stall == STALL, 0)) {
						return ret; /* cannot block */
					}
#else
					ret = LFQueue2GetPopPointer(mLFQueue, &_pointer,

												/* FIXME! */
												(stall == STALL) ? __CELL_SYNC_TRUE : __CELL_SYNC_FALSE);
#endif
				} while(__builtin_expect(stall == STALL && ret < 0, 0));
				if (__builtin_expect(ret == CELL_OK, 1)) {
					pointer = _pointer;
					CELL_DAISY_DEBUG_PRINTF("%s(getNextHeadPointer): pointer=%d\n", getClassName(), pointer%tSize);
				} else if (ret == TERMINATED) {
					CELL_DAISY_DEBUG_PRINTF("%s(getNextHeadPointer): TERMINATE received\n", getClassName());
					pointer = PTR_TERMINATED;
				}

				this->setPointer(pointer);
				return pointer;
			}
    
			template<SizeType tSize, QueueIO tQueueIO>
			__attribute__((always_inline)) inline
			PointerType Atomic<tSize,tQueueIO>::getNextTailPointer(BlockMode stall)
			{
#ifdef __SPU__
				if (__builtin_expect(mIsInitializing, 0)) {
					int ret = LFQueue2Initialize(mLFQueue, tSize, mEaSignal);
					if (ret) return ret;
					mIsInitializing = false;
				}
#endif /* __SPU__ */
				PointerType pointer = PTR_UNAVAILABLE;
				int ret, _pointer;
				do {
#ifdef __SPU__
					ret = LFQueue2GetPushPointer(mLFQueue, &_pointer,
												 (stall == STALL) ? mfpWaitSignal : 0,
												 mfpGetId);
					if (__builtin_expect(ret < 0 && mfpWaitSignal && stall == STALL, 0)) {
						return ret; /* cannot block */
					}
#else
					ret = LFQueue2GetPushPointer(mLFQueue, &_pointer,

												 /* FIXME! */
												 (stall == STALL) ? __CELL_SYNC_TRUE : __CELL_SYNC_FALSE);
#endif
				} while(__builtin_expect(stall == STALL && ret < 0, 0));
				if (__builtin_expect(ret == CELL_OK, 1)) {
					pointer = _pointer;
				}

				this->setPointer(pointer);
				return pointer;
 			}

#ifdef __SPU__
			template<SizeType tSize, QueueIO tQueueIO>
			__attribute__((always_inline)) inline
			int Atomic<tSize,tQueueIO>::setNextTailPointer(BlockMode stall, PointerType _pointer)
			{
#ifdef __SPU__
				if (__builtin_expect(mIsInitializing, 0)) {
					int ret = LFQueue2Initialize(mLFQueue, tSize, mEaSignal);
					if (ret) return ret;
					mIsInitializing = false;
				}
#endif /* __SPU__ */
				PointerType pointer = PTR_UNAVAILABLE;
				int ret;
				do {
					ret = LFQueue2SetPushPointer(mLFQueue, _pointer,
												 (stall == STALL) ? mfpWaitSignal : 0,
												 mfpGetId);
					if (__builtin_expect(ret < 0 && mfpWaitSignal && stall == STALL, 0)) {
						return ret; /* cannot block */
					}
				} while(__builtin_expect(stall == STALL && ret < 0, 0));
				if (__builtin_expect(ret == CELL_OK, 1)) {
					pointer = _pointer;
				}

				this->setPointer(pointer);
				return ret ? QUEUE_IS_BUSY : CELL_OK;
 			}
#endif /* __SPU__ */
    
			template<SizeType tSize, QueueIO tQueueIO>
			__attribute__((always_inline)) inline
			void Atomic<tSize, tQueueIO>::endCompleteConsume(uint32_t sequenceNumber, unsigned int noQueueFull)
			{
				(void)LFQueue2CompletePopPointer(mLFQueue, sequenceNumber, mfpSendSignal
												 ,noQueueFull);
			}
    
			template<SizeType tSize, QueueIO tQueueIO>
			__attribute__((always_inline)) inline
			void Atomic<tSize, tQueueIO>::endCompleteProduce(uint32_t sequenceNumber, unsigned int noQueueFull)
			{
#ifndef __SPU__
				(void)noQueueFull;
#endif
				(void)LFQueue2CompletePushPointer(mLFQueue, sequenceNumber, mfpSendSignal
#ifdef __SPU__
														  ,noQueueFull
#endif
					);
			}

			/* Terminate produce */
			template<SizeType tSize, QueueIO tQueueIO>
			__attribute__((always_inline)) inline
			int Atomic<tSize, tQueueIO>::terminate()
			{
#ifdef __SPU__
				if (__builtin_expect(mIsInitializing, 0)) {
					int ret = LFQueue2Initialize(mLFQueue, tSize, mEaSignal);
					if (ret) return ret;
					mIsInitializing = false;
				}
#endif /* __SPU__ */
				CELL_DAISY_DEBUG_PRINTF("%s(terminate): terminate sent\n", getClassName());
				int ret;
				if (tQueueIO == INPUT) {
					ret = LFQueue2PushClose(mLFQueue, mfpSendSignal);
				} else {
					ret = LFQueue2PopClose(mLFQueue, mfpSendSignal);
				}
				cellDaisyAssert(ret == CELL_OK);
				return ret;
			}

			/* Check if there are any unfinished consumers
			 * @param isCancelled true if last operation was cancel
			 * @retval true: unfinished consumers exist
			 */
			template<SizeType tSize, QueueIO tQueueIO>
			__attribute__((always_inline)) inline
			bool Atomic<tSize, tQueueIO>::hasUnfinishedConsumer(bool isCancelled)
			{
				int ret;
#ifdef __SPU__
				ret =LFQueue2HasUnfinishedConsumer(mLFQueue, isCancelled,
												   mfpWaitSignal, mfpGetId);
#else
				ret = LFQueue2HasUnfinishedConsumer(mLFQueue,

													/* FIXME! */
													isCancelled | __CELL_DAISY_COMPATIBILITY_MASK);
#endif
				return (ret == CELL_DAISY_ERROR_AGAIN);
			}

		} /* namespace QueueControl */    
	} /* namespace Daisy */
} /* namespace cell */

#endif /* __CELL_DAISY_ATO_QCTL_H__ */

/*
 * Local Variables:
 * mode:C++
 * tab-width:4
 * End:
 * vim:ts=4:sw=4:
 */
