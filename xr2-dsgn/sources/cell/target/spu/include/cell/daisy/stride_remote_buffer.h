/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2007 Sony Computer Entertainment Inc.
 * All Rights Reserved.
 */

/* Buffer::StrideRemote - Stride Remote Buffer
 */

#ifndef __CELL_DAISY_STRIDE_REMOTE_BUFFER_H__
#define __CELL_DAISY_STRIDE_REMOTE_BUFFER_H__

#include <cell/daisy/remote_buffer.h>
#include <spu_mfcio.h>

#undef CELL_DAISY_DEBUG_PRINTF
#define CELL_DAISY_DEBUG_PRINTF(...)
#ifdef CELL_DAISY_DEBUG_STRIDE_REMOTE_BUFFER
#include <cell/daisy/daisy_debug.h>
#endif

namespace cell {
	namespace Daisy {
		namespace Buffer {
    
			template <typename tType, SizeType tSize, SizeType tElementSize>
			class StrideRemote:
				public Remote<tType, tSize, NO_PARAMETER>
			{

			protected:

				SizeType mRepeatNumber __attribute__((aligned(16)));

				SizeType mStride1      __attribute__((aligned(16)));

				SizeType mStride2      __attribute__((aligned(16)));

				SizeType mStride3      __attribute__((aligned(16)));

				/* list element buffer */
				uint32_t mListPointer  __attribute__((aligned(16)));
				vec_uint4 mMfcListElements[16][(sizeof(tType)/tElementSize+1)/2];

			public:
      
				static const BufferType sBufferType = BUFFER_TYPE_REMOTE;			
				static const ConstructorMode sConstructorMode = NO_PARAMETER;			

				virtual const char *getClassName() {
#ifdef CELL_DAISY_DEBUG_STRIDE_REMOTE_BUFFER
					static char __buf[64];
					cell::Daisy::_snprintf(__buf, 64, "StrideRemote[mBufferStartEa=0x%llx,tSize:0x%x]",
										   this->mBufferStartEa,
										   tSize);
					return (const char *)__buf;
#else
					return 0;
#endif
				}
					
				/* data type definition */

				typedef tType DataType;

				/* constructor definitions */
      
				explicit StrideRemote(){}
      
				explicit StrideRemote(
					uint64_t bufferStartEa,
					uint32_t dmaTag,
					SizeType repeatNumber,
					SizeType stride1,
					SizeType stride2,
					SizeType stride3,
					uint32_t dmaTagRangeMask = 0xffffffff
				);
      
				explicit StrideRemote(
					uint64_t bufferStartEa,
					uint32_t dmaTag,
					SizeType repeatNumber,
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
						return this->mBufferStartEa +
							ptr*mStride2 +
							(ptr/mRepeatNumber)*(mStride3-mStride2*mRepeatNumber);
					}

				/* setup MFC list elements
				 * @param listPtr
				 * @param ea
				 */
				__attribute__((always_inline)) inline
					uint64_t *getMfcListElements(PointerType ptr)
					{
						uint64_t ea = getEntryEa(ptr);
						vec_uint4 *le = &mMfcListElements[mListPointer][0];
						uint64_t *list = (uint64_t *)le;
						*le = spu_promote(tElementSize           ,      0);
						*le = spu_insert ((uint32_t)ea           , *le, 1);
						*le = spu_insert (tElementSize           , *le, 2);
						*le = spu_insert ((uint32_t)(ea+mStride1), *le, 3);
						register vec_uint4 delta = spu_splats(0u);
						delta = spu_insert(mStride1*2, delta, 1);
						delta = spu_insert(mStride1*2, delta, 3);
						for(uint32_t i=0 ; i<(sizeof(tType)/tElementSize-1)/2 ; i++) {
							CELL_DAISY_DEBUG_PRINTF("%s(getMfcListElements): (ea,size)=(0x%x,0x%x),(0x%x,0x%x)\n",
													getClassName(),
													spu_extract(*le,1),
													spu_extract(*le,0),
													spu_extract(*le,3),
													spu_extract(*le,2));
													
							*(le+1) = spu_add(*le, delta);
							le++;
						}
						CELL_DAISY_DEBUG_PRINTF("%s(getMfcListElements): (ea,size)=(0x%x,0x%x),(0x%x,0x%x)\n",
												getClassName(),
												spu_extract(*le,1),
												spu_extract(*le,0),
												spu_extract(*le,3),
												spu_extract(*le,2));
						mListPointer = (mListPointer+1)%16;

						return list;
					}

				/* start write src to buffer[ptr]
				 * @param ptr buffer entry pointer
				 * @param src reference to data
				 */
				__attribute__((always_inline)) inline
					void beginWrite(PointerType ptr, __restrict__ tType *src)
					{
						uint64_t *lsa = getMfcListElements(ptr);
						CELL_DAISY_DEBUG_PRINTF(
							"%s(beginWrite): mfc_putl(src=0x%p,getEntryEa(%d)=0x%llx,lsa=0x%p,size=0x%x,mDmaTag=0x%x,0,0)\n",
							getClassName(),
							src,
							ptr,
							getEntryEa(ptr),
							lsa,
							(sizeof(tType)/tElementSize)*8,
							(this->mDmaTag+(ptr&this->mDmaTagRangeMask))%32);
						cellDaisyAssert(((uintptr_t)(src) & 0xf) == 0);
						spu_dsync();
						mfc_putl(src, getEntryEa(ptr), lsa, (sizeof(tType)/tElementSize)*8,
								 (this->mDmaTag+(ptr&this->mDmaTagRangeMask))%32, 0, 0);
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
					void beginRead(PointerType ptr, __restrict__ tType *dst)
					{
						uint64_t *lsa = getMfcListElements(ptr);
						CELL_DAISY_DEBUG_PRINTF(
							"%s(beginRead): mfc_getl(dst=0x%p,getEntryEa(%d)=0x%llx,lsa=0x%p,size=0x%x,DmaTag=0x%x,0,0)\n",
							getClassName(),
							dst,
							ptr,
							getEntryEa(ptr),
							lsa,
							(sizeof(tType)/tElementSize)*8,
							(this->mDmaTag+(ptr&this->mDmaTagRangeMask))%32);
						cellDaisyAssert(((uintptr_t)(dst) & 0xf) == 0);
						mfc_getl(dst, getEntryEa(ptr), lsa, (sizeof(tType)/tElementSize)*8,
								 (this->mDmaTag+(ptr&this->mDmaTagRangeMask))%32, 0, 0);
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
      
			}; /* class StrideRemote */
    
			/* constructor descriptions */
    
			template <typename tType, SizeType tSize, SizeType tElementSize>
			StrideRemote<tType,tSize, tElementSize>::StrideRemote(
				uint64_t bufferStartEa,
				uint32_t dmaTag,
				SizeType repeatNumber,
				SizeType stride1,
				SizeType stride2,
				SizeType stride3,
				uint32_t dmaTagRangeMask
				):
				Remote<tType,tSize>(bufferStartEa, dmaTag, dmaTagRangeMask),
				mRepeatNumber (repeatNumber),
				mStride1      (stride1     ),
				mStride2      (stride2     ),
				mStride3      (stride3     )
			{

				cellDaisyAssert(CELL_DAISY_ENTRY_SIZE(tElementSize));
				cellDaisyAssert(tElementSize >= CELL_DAISY_MIN_ENTRY_SIZE);
				cellDaisyAssert(tElementSize <= CELL_DAISY_MAX_ENTRY_SIZE);
				cellDaisyAssert(sizeof(tType)%tElementSize==0);
				cellDaisyAssert(CELL_DAISY_EA_DMA((uint32_t)bufferStartEa));

				CELL_DAISY_DEBUG_PRINTF("%s(Constructor): Ea = 0x%llx, sizeof(tType)=%d,tSize=%d\n",
										getClassName(),
										this->mBufferStartEa,
										sizeof(tType), tSize);
			}

			template <typename tType, SizeType tSize, SizeType tElementSize>
			StrideRemote<tType,tSize, tElementSize>::StrideRemote(
				uint64_t bufferStartEa,
				uint32_t dmaTag,
				SizeType repeatNumber,
				uint32_t dmaTagRangeMask
				):
				Remote<tType,tSize>(bufferStartEa, dmaTag, dmaTagRangeMask),
				mRepeatNumber (repeatNumber)
			{

				cellDaisyAssert(CELL_DAISY_ENTRY_SIZE(tElementSize));
				cellDaisyAssert(tElementSize >= CELL_DAISY_MIN_ENTRY_SIZE);
				cellDaisyAssert(tElementSize <= CELL_DAISY_MAX_ENTRY_SIZE);
				cellDaisyAssert(sizeof(tType)%tElementSize==0);
				cellDaisyAssert(CELL_DAISY_EA_DMA((uint32_t)bufferStartEa));

				/* default tile stride seting */
				mStride1             = mRepeatNumber*tElementSize;
				mStride2             = tElementSize;
				mStride3             = sizeof(tType)*mRepeatNumber;
				CELL_DAISY_DEBUG_PRINTF("%s(Constructor): Ea = 0x%llx, sizeof(tType)=%d,tSize=%d\n",
										getClassName(),
										this->mBufferStartEa,
										sizeof(tType), tSize);
			}

		} /* namespace Buffer */    
	} /* namespace Daisy */
} /* namespace cell */

#endif /* __CELL_DAISY_STRIDE_REMOTE_BUFFER_H__ */

/*
 * Local Variables:
 * mode:C++
 * tab-width:4
 * End:
 * vim:ts=4:sw=4:
 */
