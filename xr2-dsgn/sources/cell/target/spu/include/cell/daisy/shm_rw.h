/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2007 Sony Computer Entertainment Inc.
 * All Rights Reserved.
 */

/* QueueControl::SharedMeoryReader - shared memory reader
 * QueueControl::SharedMeoryWriter - shared memory writer
 */

#ifndef __CELL_DAISY_SHM_RW_H__
#define __CELL_DAISY_SHM_RW_H__

#include <cell/daisy/qctl.h>

#undef CELL_DAISY_DEBUG_PRINTF
#define CELL_DAISY_DEBUG_PRINTF(...)
#ifdef CELL_DAISY_DEBUG_SHM_RW
#include <cell/daisy/daisy_debug.h>
#endif

namespace cell {
	namespace Daisy {
		namespace QueueControl {

			static struct {
				vec_uint4 tailPointer, headPointer;
				qword skip[6];
			} sLsCopy __attribute__((aligned(128)));

			template<SizeType tSize>
			class SharedMemoryReader:public Abstract<tSize, OUTPUT> {

			protected:

				uint64_t mLockEa     __attribute__((aligned(16)));
      
				/* flag indicating queue is already terminated */
				qword mIsTerminated;

			public:

				__attribute__((always_inline)) inline
				unsigned int isOutOfOrder(){ return 1; }

				static const QueueControlType sQueueControlType = QCTL_TYPE_ATOMIC;
 			    static const SizeType sQueueMaxSize = 0xffffffff;

				virtual const char *getClassName()
					{
#ifdef CELL_DAISY_DEBUG_SHM_RW
						static char buf[64];
						cell::Daisy::_snprintf(buf, 64, "SharedMemoryReader[mLockEa:0x%llx,tSize:%d]",
											   this->mLockEa,
											   tSize);
						return (const char *)buf;
#else
						return 0;
#endif
					}

				__attribute__((always_inline)) inline
				void dump(const char *messages = 0) {
#ifndef CELL_DAISY_DEBUG_SHM_RW
					(void)messages;
#else
					static char __dump_buf[256];

					mfc_getllar(&sLsCopy, mLockEa, 0, 0);
					(void)mfc_read_atomic_status();
					spu_dsync();
					cell::Daisy::_snprintf(__dump_buf, 256,
										   "head=%d\n", spu_extract(sLsCopy.headPointer, 0));
					if (messages != 0) strcat(__dump_buf, messages);
					CELL_DAISY_DEBUG_PRINTF("%s %s\n", getClassName(), __dump_buf);
#endif
				}

				/* constructor definition */
				explicit SharedMemoryReader():
					Abstract<tSize, OUTPUT> (),
					mIsTerminated           (si_from_uint(false)){}

				explicit SharedMemoryReader(uint64_t lockEa):
					Abstract<tSize, OUTPUT> (),
					mLockEa                 (lockEa),
					mIsTerminated           (si_from_uint(false))
					{
						cellDaisyAssert(CELL_DAISY_EA_ATOMIC((uint32_t)lockEa));

						/* initialize lock variable */
						do {
							mfc_getllar((void *)&sLsCopy, lockEa, 0, 0);
							(void)mfc_read_atomic_status();
							spu_dsync();
							
							if (spu_extract(sLsCopy.headPointer, 1) == 0) {
								sLsCopy.headPointer = (vec_uint4){(uint32_t)-1, 1, 0, 0};
								spu_dsync();
							}
							
							mfc_putllc((volatile void *)&sLsCopy, lockEa, 0, 0);
						} while(__builtin_expect(mfc_read_atomic_status(), 0));
					}

				/* Get next Head pointer */
				PointerType getNextHeadPointer(BlockMode stall);
      
			};
    
			/* Member functions description(s) */
    
			template<SizeType tSize>
			__attribute__((always_inline)) inline
				PointerType SharedMemoryReader<tSize>::getNextHeadPointer(BlockMode stall)
			{
				(void)stall;

				if (__builtin_expect(si_to_uint(mIsTerminated), 0)) {
					return PTR_TERMINATED;
				}

				register PointerType rHeadPointer;
				do {
					/* getllar lock_line -> sLsCopy */
					mfc_getllar(&sLsCopy, mLockEa, 0, 0);
					(void)mfc_read_atomic_status();
					spu_dsync();

					rHeadPointer = spu_extract(sLsCopy.headPointer, 0);
					if (__builtin_expect(rHeadPointer != tSize-1, 1)) {
						rHeadPointer++;
					} else {
						mIsTerminated = si_from_uint(true);
						return PTR_TERMINATED;
					}
					sLsCopy.headPointer = spu_promote((uint32_t)rHeadPointer, 0);

					spu_dsync();
					mfc_putllc((volatile void *)&sLsCopy, mLockEa, 0, 0);
				} while(__builtin_expect(mfc_read_atomic_status(), 0));
				this->setPointer(rHeadPointer);
				if (__builtin_expect((uint32_t)rHeadPointer == tSize-1, 0)) {
					mIsTerminated = si_from_uint(true);
				}

				CELL_DAISY_DEBUG_PRINTF("%s(getNextHeadPointer): mPointer=%d\n",
										getClassName(), this->getPointer());
				this->dump();
      
				return rHeadPointer;
			}
    
			template<SizeType tSize>
			class SharedMemoryWriter:public Abstract<tSize, INPUT> {

			protected:

				uint64_t mLockEa     __attribute__((aligned(16)));
      
				/* set 'true' when winner get started */
				qword mStartFlag     __attribute__((aligned(16)));
      
				/* dummy for padding */
				uint32_t mDummy      __attribute__((aligned(16)));

			public:
      
				__attribute__((always_inline)) inline
				unsigned int isOutOfOrder(){ return 1; }

				static const QueueControlType sQueueControlType = QCTL_TYPE_ATOMIC;
 			    static const SizeType sQueueMaxSize = 0xffffffff;

				virtual const char *getClassName() {
#ifdef CELL_DAISY_DEBUG_SHM_RW
					static char __buf[64];
					cell::Daisy::_snprintf(__buf, 64, "SharedMemoryWriter[mLockEa:0x%llx,tSize:%d]",
										   this->mLockEa,
										   tSize);
					return (const char *)__buf;
#else
					return 0;
#endif
				}

				__attribute__((always_inline)) inline
				void dump(const char *messages = 0) {
#ifndef CELL_DAISY_DEBUG_SHM_RW
					(void)messages;
#else
					static char __dump_buf[256];
					
					mfc_getllar(&sLsCopy, mLockEa, 0, 0);
					(void)mfc_read_atomic_status();
					spu_dsync();
					cell::Daisy::_snprintf(__dump_buf, 256,
										   "tail=%d\n", spu_extract(sLsCopy.tailPointer, 0));
					if (messages != 0) strcat(__dump_buf, messages);
					CELL_DAISY_DEBUG_PRINTF("%s %s\n", getClassName(), __dump_buf);
#endif
				}

				/* constructor definition */
				explicit SharedMemoryWriter():
					Abstract<tSize, INPUT> (),
					mStartFlag             (si_from_uint(false)){}

				explicit SharedMemoryWriter(uint64_t lockEa):
					Abstract<tSize, INPUT> (),
					mLockEa                (lockEa),
					mStartFlag             (si_from_uint(false))
					{
						cellDaisyAssert(CELL_DAISY_EA_ATOMIC((uint32_t)mLockEa));

						/* initialize lock variable */
						do {
							mfc_getllar((void *)&sLsCopy, mLockEa, 0, 0);
							(void)mfc_read_atomic_status();
							spu_dsync();
							
							if (spu_extract(sLsCopy.tailPointer, 1) == 0) {
								sLsCopy.tailPointer = (vec_uint4){(uint32_t)-1, 1, 0, 0};
								spu_dsync();
							}
							
							mfc_putllc((volatile void *)&sLsCopy, mLockEa, 0, 0);
						} while(__builtin_expect(mfc_read_atomic_status(), 0));
					}


				/* Get next Tail pointer */
				PointerType getNextTailPointer(BlockMode stall);
      
			};
    
			/* Member functions description(s) */
    
			template<SizeType tSize>
			__attribute__((always_inline)) inline
				PointerType SharedMemoryWriter<tSize>::getNextTailPointer(BlockMode stall)
			{
				(void)stall;

				/* check if I am 1st visitor */
				if (__builtin_expect(!si_to_uint(this->mStartFlag), 0)) {
					this->mStartFlag = si_from_uint(true);
				}

				register PointerType rTailPointer;
				do {
					mfc_getllar((void *)&sLsCopy, mLockEa, 0, 0);
					(void)mfc_read_atomic_status();
					spu_dsync();

					rTailPointer = spu_extract(sLsCopy.tailPointer, 0);
					rTailPointer++;
					sLsCopy.tailPointer = spu_promote((uint32_t)rTailPointer, 0);

					spu_dsync();
					mfc_putllc((volatile void *)&sLsCopy, mLockEa, 0, 0);
				} while(__builtin_expect(mfc_read_atomic_status(), 0));
				this->setPointer(rTailPointer);

				cellDaisyAssert((uint32_t)rTailPointer < tSize);
				
				CELL_DAISY_DEBUG_PRINTF("%s(getNextTailPointer): mPointer=%d\n",
										getClassName(), this->getPointer());
				this->dump();

				return rTailPointer;
			}
    
		} /* namespace QueueControl */    
	} /* namespace Daisy */
} /* namespace cell */

#endif /* __CELL_DAISY_SHM_RW_H__ */

/*
 * Local Variables:
 * mode:C++
 * tab-width:4
 * End:
 * vim:ts=4:sw=4:
 */
