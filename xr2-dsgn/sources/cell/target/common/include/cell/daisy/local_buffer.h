/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2007 Sony Computer Entertainment Inc.
 * All Rights Reserved.
 */

/* Buffer::Local - Local(to SPU) Buffer
 */

#ifndef __CELL_DAISY_LOCAL_BUFFER_H__
#define __CELL_DAISY_LOCAL_BUFFER_H__

#ifdef __SPU__
#include <cell/daisy/v_memcpy.h>
#endif
#include <cell/daisy/buffer.h>

#undef CELL_DAISY_DEBUG_PRINTF
#define CELL_DAISY_DEBUG_PRINTF(...)
#ifdef CELL_DAISY_DEBUG_LOCAL_BUFFER
#include <cell/daisy/daisy_debug.h>
#endif

namespace cell {
	namespace Daisy {
		namespace Buffer {
    
			template <typename tType, SizeType tSize
#ifdef __SPU__
					  , ConstructorMode tConstructorMode = NO_PARAMETER
#endif
					  >
			class Local:
				public Abstract<tType, tSize>
			{
			public:
      
				static const BufferType sBufferType = BUFFER_TYPE_LOCAL;			
#ifdef __SPU__
				static const ConstructorMode sConstructorMode = tConstructorMode;			
#endif

			protected:

				volatile tType *mLocalBuffer __CELL_DAISY_SPU_ALIGN16__;

				/* buffer area */
				uint8_t mBuffer[sizeof(tType)*tSize+127] __CELL_DAISY_SPU_ALIGN16__;

			public:
      
				/* data type definition */

				typedef tType DataType;

				virtual const char *getClassName(){
#ifdef CELL_DAISY_DEBUG_LOCAL_BUFFER
					static char __buf[64];
					cell::Daisy::_snprintf(__buf, 64, "Local[mLocalBuffer=%p,tSize:%d, sizeof(tType):%d]",
										   mLocalBuffer,
										   tSize,
										   sizeof(tType));
					return (const char *)__buf;
#else
					return 0;
#endif
				}
					
				/* constructor(s) */

				explicit Local();
      
#ifdef __SPU__
				/* Automatic association with shared memory
				 * @param parameterEa paramter exchange buffer Ea
				 * @param spuNum spuNum of myself
				 */
				explicit Local(
					uint64_t parameterEa, /* 128bytes */
					int spuNum
					);
#else /* __SPU__ */

				/*
				 * @param pBuffer pointer to buffer
				 */
				explicit Local(
					volatile void *pBuffer
					);
#endif /* __SPU__ */
				/* member function(s) */
      
				__attribute__((always_inline)) inline
					void beginWrite(PointerType ptr, tType * __restrict__ src)
					{
						register PointerType ptr_ = (ptr<(int)tSize)?ptr:ptr-tSize;
						CELL_DAISY_DEBUG_PRINTF(
							"%s(beginWrite): memcpy(&mLocalBuffer[%d]=%p, src=%p)\n",
							getClassName(), ptr_, &mLocalBuffer[ptr_], src, sizeof(tType));
#ifdef __SPU__
						_cellDaisyMemcpy((void *)(uintptr_t)&mLocalBuffer[ptr_], (const void *)(uintptr_t)src, sizeof(tType));
#else
						__builtin_memcpy((void *)(uintptr_t)&mLocalBuffer[ptr_], (const void *)(uintptr_t)src, sizeof(tType));
#endif
					}
      
				__attribute__((always_inline)) inline
					int tryBeginWrite(PointerType ptr, tType * __restrict__ src)
					{
						beginWrite(ptr, src);
						return CELL_OK;
					}
      
				__attribute__((always_inline)) inline
					void endWrite(PointerType ptr)
					{ (void)ptr; }
      
				__attribute__((always_inline)) inline
					int tryEndWrite(PointerType ptr)
					{
						endWrite(ptr);
						return CELL_OK;
					}
      
				__attribute__((always_inline)) inline
					void beginRead(PointerType ptr, tType * __restrict__ dst)
					{
						register PointerType ptr_ = (ptr<(int)tSize)?ptr:ptr-tSize;
						CELL_DAISY_DEBUG_PRINTF(
							"%s(read_begin): memcpy(dst=%p, &mLocalBuffer[%d]=%p\n",
								getClassName(), dst, ptr_, &mLocalBuffer[ptr_]);
#ifdef __SPU__
						_cellDaisyMemcpy((void *)(uintptr_t)dst, (const void *)(uintptr_t)&mLocalBuffer[ptr_], sizeof(tType));
#else
						__builtin_memcpy((void *)(uintptr_t)dst, (const void *)(uintptr_t)&mLocalBuffer[ptr_], sizeof(tType));
#endif
					}
      
				__attribute__((always_inline)) inline
					int tryBeginRead(PointerType ptr, tType * __restrict__ dst)
					{
						beginRead(ptr, dst);
						return CELL_OK;
					}
      
				__attribute__((always_inline)) inline
					void endRead(PointerType ptr)
					{ (void)ptr; }
      
				__attribute__((always_inline)) inline
					int tryEndRead(PointerType ptr)
					{
						endRead(ptr);
						return CELL_OK;
					}
      
				__attribute__((always_inline)) inline
					volatile tType* getEntryReference(PointerType ptr){
					register PointerType ptr_ = (ptr<(int)tSize)?ptr:ptr-tSize;
					CELL_DAISY_DEBUG_PRINTF("%s(getEntryReference): mLocalBuffer[%d]=%p\n",
											getClassName(),ptr_, &mLocalBuffer[ptr_]);
					return &mLocalBuffer[ptr_];
				}
      
			}; /* class Buffer::Local */
    
			/* constructor descriptions */

			template <typename tType, SizeType tSize
#ifdef __SPU__
					  , ConstructorMode tConstructorMode
#endif
					  >
			inline Local<tType, tSize
#ifdef __SPU__
				  , tConstructorMode
#endif
				  >::Local()
			{

#ifdef __SPU__
				static int sAssertDummy2[(tConstructorMode == NO_PARAMETER) ? 1: -1] __attribute__((unused));
#endif

				/* 128 bytes align */
				mLocalBuffer = (tType *)(((uintptr_t)mBuffer + 127) & ~127);
				CELL_DAISY_DEBUG_PRINTF("%s(Constructor):\n", getClassName());
			}
    
#ifdef __SPU__    
			template <typename tType, SizeType tSize, ConstructorMode tConstructorMode>
			inline Local<tType, tSize, tConstructorMode>::Local(
				uint64_t parameterEa, /* 128bytes */
				int spuNum
				)
			{

				static int sAssertDummy3[(tConstructorMode == PARAMETER) ? 1: -1] __attribute__((unused));
				cellDaisyAssert(CELL_DAISY_EA_ATOMIC((uint32_t)parameterEa));
				cellDaisyAssert(spuNum >= 0);
				cellDaisyAssert(spuNum <= 255);

				/* 128 bytes align */
				mLocalBuffer = (volatile tType *)(((uintptr_t)mBuffer + 127) & ~127);
      
				/* setup shared param buffer */
				uint8_t param0[sizeof(ParameterType)+127];

				ParameterType *param;
				param = (ParameterType *)(((long)param0 + 127) & ~127);

				mfc_getllar((void *)param, parameterEa, 0, 0);
				mfc_read_atomic_status();
				spu_dsync();
				cellDaisyAssert(param->bufferEa == 0);
      
				uint32_t local_buf_lsa = (uint32_t)mLocalBuffer;
      
				param->bufferEa = CELL_DAISY_GET_LS_AREA(spuNum) + (uint64_t)local_buf_lsa;
				param->size     = sizeof(tType);
				CELL_DAISY_DEBUG_PRINTF("%s(Constructor): EA=0x%llx\n",
										getClassName(),
										param->bufferEa);
     
				/* write param data to param shared buffer */
				spu_dsync();
				mfc_putlluc((void *)param, parameterEa, 0, 0);
				mfc_read_atomic_status();

			}

#else /* __SPU__ */

			template <typename tType, SizeType tSize>
			inline Local<tType, tSize>::Local(
				volatile void *pBuffer
				):
				mLocalBuffer((volatile tType *)pBuffer)
			{
				cellDaisyAssert(((uintptr_t)pBuffer & 15) == 0);
			}

#endif /* __SPU__ */

		} /* namespace Buffer */
	} /* namespace Daisy */
} /* namespace cell */

#endif /* __CELL_DAISY_LOCAL_BUFFER_H__ */

/*
 * Local Variables:
 * mode:C++
 * tab-width:4
 * End:
 * vim:ts=4:sw=4:
 */
