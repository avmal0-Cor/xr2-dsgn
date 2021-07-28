/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2007 Sony Computer Entertainment Inc.
 * All Rights Reserved.
 */

/* Buffer::Remote - Remote(to SPU) Buffer
 */

#ifndef __CELL_DAISY_REMOTE_BUFFER_H__
#define __CELL_DAISY_REMOTE_BUFFER_H__

#include <cell/daisy/buffer.h>
#include <spu_mfcio.h>

#undef CELL_DAISY_DEBUG_PRINTF
#define CELL_DAISY_DEBUG_PRINTF(...)
#ifdef CELL_DAISY_DEBUG_REMOTE_BUFFER
#include <cell/daisy/daisy_debug.h>
#endif

namespace cell {
	namespace Daisy {
		namespace Buffer {
    
			template <typename tType, SizeType tSize, ConstructorMode tConstructorMode = NO_PARAMETER>
			class Remote:
				public Abstract<tType, tSize>
			{
			private:

				/* static assertion */
				static int sAssertDummy[
					CELL_DAISY_ENTRY_SIZE(sizeof(tType)) &&         /* 1.entry size is multiple of 16 */
					(sizeof(tType) >= CELL_DAISY_MIN_ENTRY_SIZE)    /* 2.min size */
					? 1 : -1];

			public:
      
				static const BufferType sBufferType = BUFFER_TYPE_REMOTE;			
				static const ConstructorMode sConstructorMode = tConstructorMode;			

				__attribute__((always_inline)) inline
					uint32_t  getTag(){return mDmaTag;}

				virtual const char *getClassName(){
#ifdef CELL_DAISY_DEBUG_REMOTE_BUFFER
					static char __buf[64];
					cell::Daisy::_snprintf(__buf, 64, "Remote[mBufferStartEa=0x%llx,tSize:0x%x]",
										   mBufferStartEa,
										   tSize);
					return (const char *)__buf;
#else
					return 0;
#endif
				}
					
			protected:
      
				uint64_t mBufferStartEa   __attribute__((aligned(16)));
				uint32_t mDmaTag          __attribute__((aligned(16)));
				uint32_t mDmaTagRangeMask __attribute__((aligned(16)));
				/* dummy for padding */
				int mDummy                __attribute__((aligned(16)));
      
				/* stall until completion of dma specified by ptr
				 * @param ptr buffer entry pointer
				 */
				__attribute__((always_inline)) inline
					void waitDmaEnd(PointerType ptr)
					{
						register PointerType ptr_ = (ptr >= (int)tSize) ? ptr-tSize : ptr;
						/* clear update request */
						mfc_write_tag_update_immediate();
						while (__builtin_expect(mfc_stat_tag_update() != 1, 0)) {}
						mfc_read_tag_status();

						mfc_write_tag_mask(1<<((mDmaTag+(ptr_&mDmaTagRangeMask))%32));
						CELL_DAISY_DEBUG_PRINTF("%s(waitDmaEnd): mfc_read_tag_status(0x%x=1<<((%d+%d)%32))...",
												getClassName(),
												1<<((mDmaTag+(ptr_&mDmaTagRangeMask))%32),mDmaTag,ptr);
	
						mfc_read_tag_status_all();
						spu_dsync();
	
						CELL_DAISY_DEBUG_PRINTF("comp(0x%x)\n", 1<<((mDmaTag+(ptr_&mDmaTagRangeMask))%32));
					}
      
				/* return dma tag status mask
				 * @param ptr buffer entry pointer
				 * @retval dma tag status
				 */
				__attribute__((always_inline)) inline
					uint32_t getDmaTagStatus(PointerType ptr)
					{
						register PointerType ptr_ = (ptr >= (int)tSize) ? ptr-tSize : ptr;
						mfc_write_tag_mask(1<<((mDmaTag+(ptr_&mDmaTagRangeMask))%32));
						mfc_write_tag_update_immediate();
						while (__builtin_expect(mfc_stat_tag_update() != 1, 0)) {}
						
						uint32_t stat = mfc_read_tag_status();
						spu_dsync();
							
						CELL_DAISY_DEBUG_PRINTF(
							"%s(getDmaTagStatus(ptr:%d)): getDmaTagStatus(mask=0x%x) = %d\n",
							getClassName(),
							ptr_,
							1<<((mDmaTag+(ptr_&mDmaTagRangeMask))%32), stat);
						
						return stat;
					}
      
			public:
      
				/* data type definition */

				typedef tType DataType;

				/* constructor definitions */
      
				explicit Remote(){}
      
				explicit Remote(
					uint64_t ea,
					uint32_t dmaTag,
					uint32_t dmaTagRangeMask = 0xffffffff
				);
      
				/* member functions */
      
				/* get Effective Address from buffer entry pointer
				 * @param ptr buffer entry pointer
				 * @retval buffer Effective Address
				 */
				__attribute__((always_inline)) inline
					uint64_t getEntryEa(PointerType ptr)
					{
						register PointerType ptr_ = (ptr >= (int)tSize) ? ptr-tSize : ptr;
						return mBufferStartEa + ptr_*sizeof(tType);
					}
      
				/* get reference to buffer
				 * *** NOT AVAILABLE for Remote ***
				 */
				volatile tType* getEntryReference(PointerType ptr)
				{
					(void)ptr;
					return NULL; /* no operation */
				}
      
				void setDmaTagRangeMask(const uint32_t dmaTagRangeMask) {
					mDmaTagRangeMask = dmaTagRangeMask;
				}
      
				/* start write src to buffer[ptr]
				 * @param ptr buffer entry pointer
				 * @param src reference to data
				 */
				__attribute__((always_inline)) inline
					void beginWrite(PointerType ptr, __restrict__ tType *src)
					{
						uint64_t ea = getEntryEa(ptr);
						register PointerType ptr_ = (ptr >= (int)tSize) ? ptr-tSize : ptr;
						CELL_DAISY_DEBUG_PRINTF(
							"%s(beginWrite): mfc_put(src=0x%p,getEntryEa(%d)=0x%llx, sizeof(tType)=0x%x,mDmaTag=0x%x,0,0)\n",
							getClassName(),
							src,
							ptr_,
							ea,
							sizeof(tType),
							(mDmaTag+(ptr_&mDmaTagRangeMask))%32);
						cellDaisyAssert(((uintptr_t)(src) & 0xf) == 0);
						spu_dsync();
						mfc_put(src, ea, sizeof(tType), (mDmaTag+(ptr_&mDmaTagRangeMask))%32, 0, 0);
					}
      
				/* try start write src to buffer[ptr]
				 * @param ptr buffer entry pointer
				 * @param src reference to data
				 * @retval CELL_OK:write start succeeded,QUEUE_IS_BUSY:write start failed
				 */
				__attribute__((always_inline)) inline
					int tryBeginWrite(PointerType ptr, __restrict__ tType *src)
					{
						if (__builtin_expect(mfc_stat_cmd_queue() != 0, 1)) {
							beginWrite(ptr, src);

							return CELL_OK;
						}else
							return QUEUE_IS_BUSY;
					}
      
				__attribute__((always_inline)) inline
					void endWrite(PointerType ptr)
					{
						waitDmaEnd(ptr);
					}
      
				__attribute__((always_inline)) inline
					int tryEndWrite(PointerType ptr)
					{
						register uint32_t stat = getDmaTagStatus(ptr);
	
						if (__builtin_expect(stat != 0,1)) {
							return CELL_OK;
						} else {
							return QUEUE_IS_BUSY;
						}
					}
      
				__attribute__((always_inline)) inline
					void beginRead(PointerType ptr, __restrict__ tType *dst)
					{
						uint64_t ea = getEntryEa(ptr);
						register PointerType ptr_ = (ptr >= (int)tSize) ? ptr-tSize : ptr;
						CELL_DAISY_DEBUG_PRINTF(
							"%s(beginRead): mfc_get(dst=0x%p,getEntryEa(%d)=0x%llx, sizeof(tType)=0x%x,DmaTag=0x%x,0,0)\n",
							getClassName(),
							dst,
							ptr_,
							getEntryEa(ptr),
							sizeof(tType),
							(mDmaTag+(ptr_&mDmaTagRangeMask))%32);
						cellDaisyAssert(((uintptr_t)(dst) & 0xf) == 0);
						mfc_get(dst, ea, sizeof(tType), (mDmaTag+(ptr_&mDmaTagRangeMask))%32, 0, 0);
					}
      
				__attribute__((always_inline)) inline
					int tryBeginRead(PointerType ptr, __restrict__ tType *dst)
					{
						if (__builtin_expect(mfc_stat_cmd_queue() != 0, 1)) {
							beginRead(ptr, dst);
							return CELL_OK;
						} else {
							return QUEUE_IS_BUSY;
						}
					}
      
				__attribute__((always_inline)) inline
					void endRead(PointerType ptr)
					{
						waitDmaEnd(ptr);
					}
      
				__attribute__((always_inline)) inline
					int tryEndRead(PointerType ptr)
					{
						if (getDmaTagStatus(ptr) != 0) {
							return CELL_OK;
						} else {
							return QUEUE_IS_BUSY;
						}
					}
			}; /* class Remote */
    
			/* constructor descriptions */
    
			template <typename tType, SizeType tSize, ConstructorMode tConstructorMode>
			inline Remote<tType, tSize, tConstructorMode>::Remote(
				uint64_t ea,
				uint32_t dmaTag,
				uint32_t dmaTagRangeMask
				):
				mBufferStartEa  (ea),
				mDmaTag         (dmaTag),
				mDmaTagRangeMask(dmaTagRangeMask)
			{

				cellDaisyAssert(
					(sizeof(tType) <= CELL_DAISY_MAX_ENTRY_SIZE) &&
					(tConstructorMode == NO_PARAMETER &&
						CELL_DAISY_EA_DMA((uint32_t)ea)) ||
					(tConstructorMode == PARAMETER &&
						CELL_DAISY_EA_ATOMIC((uint32_t)ea)) );

				if (tConstructorMode != NO_PARAMETER) {
					/* ea : paramter start ea */

					/* allocate param receive area */
					uint8_t param0[sizeof(ParameterType)+127];
					ParameterType *param;
					param = (ParameterType *)(((uintptr_t)param0 + 127) & ~127);
	
					do {
						/* poll param from shared memory */
						mfc_getllar((void *)param, ea, 0, 0);
						mfc_read_atomic_status();
						spu_dsync();
					} while (param->bufferEa == 0);
	
					cellDaisyAssert(((uint32_t)param->bufferEa & 0xf) == 0);
					cellDaisyAssert(param->size == sizeof(tType));
					mBufferStartEa = param->bufferEa;

				}
				CELL_DAISY_DEBUG_PRINTF("%s(Constructor): Ea = 0x%llx, sizeof(tType)=%d,tSize=%d\n",
										getClassName(),
										mBufferStartEa,
										sizeof(tType), tSize);
			}

		} /* namespace Buffer */    
	} /* namespace Daisy */
} /* namespace cell */

#endif /* __CELL_DAISY_REMOTE_BUFFER_H__ */

/*
 * Local Variables:
 * mode:C++
 * tab-width:4
 * End:
 * vim:ts=4:sw=4:
 */
