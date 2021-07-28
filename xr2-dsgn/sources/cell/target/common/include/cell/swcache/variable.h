/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2009 Sony Computer Entertainment Inc.
 * All Rights Reserved.
 */

#ifndef __CELL_SWCACHE_VARIABLE_H__
#define __CELL_SWCACHE_VARIABLE_H__

#include <stdint.h>
#include <string.h>

#ifdef __SPU__
#include <cell/dma.h>

#include <cell/swcache/default_cache.h>
#include <cell/swcache/DMA_tag.h>
#endif

#include <cell/swcache/types.h>

#define __CELL_SWCACHE_VARIABLE_DPRINTF(format,...)\
	__CELL_SWCACHE_DPRINTF("%s" format, "cell::swcache::Variable::", ##__VA_ARGS__)
#define __CELL_SWCACHE_VARIABLE_DPRINTF_1(format,...)\
	__CELL_SWCACHE_DPRINTF_1("%s" format, "cell::swcache::Variable::", ##__VA_ARGS__)

__CELL_SWCACHE_BEGIN

/*! @brief Auto-ptr style template which transparently
           works on both the SPU and the PPU.  It automatically
           handles all of the DMA transfers.
*/
#ifdef __SPU__
template <class type, enum PatchMode tMode = RO_MODE> class Variable : protected Resource
#else
template <class type, enum PatchMode tMode = RO_MODE> class Variable
#endif
{
	public :
		//! @brief default Constructor
		__CELL_SWCACHE_ALWAYS_INLINE Variable():
		   mPPUAddress(0)
		{
		#ifdef __SPU__
			mU32 = 0;
			mDmaTag = reserveChannel();
		#endif
			__CELL_SWCACHE_VARIABLE_DPRINTF("Variable(): this=%p", this);
		}

		/*! @brief copy Constructor
		@param[in] var Source of copy constructor
		*/
		__CELL_SWCACHE_ALWAYS_INLINE Variable(Variable &var)
		{
			__CELL_SWCACHE_VARIABLE_DPRINTF("Variable(Variable &var): this=%p, var.mPPUAddress=%p", this, var.mPPUAddress);
		#ifdef __SPU__
			mU32 = 0;
			mDmaTag = reserveChannel();
			var.verifyDMA();
		#endif
			memcpy(this, &var, sizeof(*this));
		}

		/*! @brief Constructor
		@param[in] PPUaddress Pointer to object in main memory
		*/
		__CELL_SWCACHE_ALWAYS_INLINE Variable(type *PPUaddress) :
		   mPPUAddress(PPUaddress)
		{
		#ifdef __SPU__
			mU32 = 0;
			mDmaTag = reserveChannel();
		#endif
			__CELL_SWCACHE_VARIABLE_DPRINTF("Variable(type *PPUaddress=%p): this=%p", PPUaddress, this);
			// If we have an address, then start the DMA transfer
			if (__builtin_expect((uint32_t)PPUaddress, 1)) beginDMARead();
		}

		//! @brief Destructor
		__CELL_SWCACHE_ALWAYS_INLINE ~Variable()
		{
			__CELL_SWCACHE_VARIABLE_DPRINTF("~Variable(): this=%p", this);
			// make sure that the dma finishes
			// do the write back if appropriate
			if (mPPUAddress == 0) return;
			if (tMode == RW_MODE)
			{
				beginDMAWrite();
		#ifdef __SPU__
				cellDmaWaitTagStatusAll(1 << mDmaTag);
		#endif
			}
			verifyDMA();
		#ifdef __SPU__
			releaseChannel(mDmaTag);
		#endif
		}

		//! @brief Write back the memory to the PPU main memory.  This will be a memcpy on the PPU side
		__CELL_SWCACHE_ALWAYS_INLINE void flush()
		{
			__CELL_SWCACHE_VARIABLE_DPRINTF("flush(): this=%p", this);
			if (tMode != RW_MODE || mPPUAddress == 0) return;
			beginDMAWrite();
		#ifdef __SPU__
			cellDmaWaitTagStatusAll(1 << mDmaTag);
		#endif
		}

		/*! @brief operator=
		@param[in] pPtr		EA to set the swcache::Variable to
		@param[out] this	A reference to the object
		*/
		__CELL_SWCACHE_ALWAYS_INLINE Variable &operator=(type *PPUaddress)
		{
			__CELL_SWCACHE_VARIABLE_DPRINTF("operator=(type *PPUaddress=%p): this=%p", PPUaddress, this);
			if (tMode == RW_MODE && mPPUAddress != 0)
			{
				beginDMAWrite();
			}
			mPPUAddress = PPUaddress;
		#ifdef __SPU__
			mU32 = 0;
		#endif
			// If we have an address, then start the DMA transfer
			if (__builtin_expect((uint32_t)PPUaddress, 1))
			{
				beginDMARead();
			} else {
		#ifdef __SPU__
				cellDmaWaitTagStatusAll(1 << mDmaTag);
		#endif
			}
			return(*this);
		}

		/*! @brief operator=
		@param[in] pPtr		Change where the swcache::Variable is pointing
		@param[out] this	A reference to the object
		*/
		__CELL_SWCACHE_ALWAYS_INLINE Variable &operator=(const Variable &var)
		{
			__CELL_SWCACHE_VARIABLE_DPRINTF("operator=(const Variable &var): this=%p, var.mPPUAddress=%p", this, var.mPPUAddress);
			if (tMode == RW_MODE && mPPUAddress != 0)
			{
				beginDMAWrite();
		#ifdef __SPU__
				cellDmaWaitTagStatusAll(1 << mDmaTag);
		#endif
			}
			var.verifyDMA();
			memcpy(this, &var, sizeof(*this));

			return(*this);
		}

		/*! @brief operator* to de-reference the pointer
		@param[out] reference	A reference to the underlying object
		*/
		__CELL_SWCACHE_ALWAYS_INLINE type &operator*()
		{
			__CELL_SWCACHE_VARIABLE_DPRINTF("operator*(): this=%p,mPPUAddress=%p", this, mPPUAddress);
			__CELL_SWCACHE_ASSERT(mPPUAddress, "NULL pointer is accessed!!");
		#ifdef __SPU__
			verifyDMA();
			return *((type *)(uintptr_t)&mBuf[(uintptr_t)mPPUAddress&15]);
		#else
			return(mVar);
		#endif
		}

		/*! @brief operator-> to access the pointer
		@param[out] pointer		A pointer to the underlying object
		*/
		__CELL_SWCACHE_ALWAYS_INLINE type *operator->()
		{
			__CELL_SWCACHE_VARIABLE_DPRINTF("operator->(): this=%p,mPPUAddress=%p", this, mPPUAddress);
			__CELL_SWCACHE_ASSERT(mPPUAddress, "NULL pointer is accessed!!");
		#ifdef __SPU__
			verifyDMA();
			return (type *)(uintptr_t)&mBuf[(uintptr_t)mPPUAddress&15];
		#else
			return(&mVar);
		#endif
		}

		/*! @brief operator bool
		@param[out] bool The boolean operator
		*/
		__CELL_SWCACHE_ALWAYS_INLINE operator bool()
		{
			__CELL_SWCACHE_VARIABLE_DPRINTF("operator bool(): this=%p,mPPUAddress=%p", this, mPPUAddress);
			return(mPPUAddress);
		}

		/*! @brief operator bool const
		@param[out] bool The boolean operator const
		*/
		__CELL_SWCACHE_ALWAYS_INLINE operator bool() const
		{
			__CELL_SWCACHE_VARIABLE_DPRINTF("operator bool() const: this=%p,mPPUAddress=%p", this, mPPUAddress);
			return(mPPUAddress);
		}

		/*! @brief operator void *const
		@param[out] bool The void *operator
		*/
		__CELL_SWCACHE_ALWAYS_INLINE operator void *()
		{
			__CELL_SWCACHE_VARIABLE_DPRINTF("operator void *(): this=%p,mPPUAddress=%p", this, mPPUAddress);
			return((void *)mPPUAddress);
		}

		/*! @brief operator void * const
		@param[out] bool The void * operator const
		*/
		__CELL_SWCACHE_ALWAYS_INLINE operator void *() const
		{
			__CELL_SWCACHE_VARIABLE_DPRINTF("operator void *() const: this=%p,mPPUAddress=%p", this, mPPUAddress);
			return(mPPUAddress);
		}

	protected :

		//! @brief Start the DMA transfer. This is a memcpy on the PPU
		__CELL_SWCACHE_ALWAYS_INLINE void	beginDMARead()
		{
		#ifdef __SPU__
			register unsigned int dmaTag = mDmaTag;
			__CELL_SWCACHE_ASSERT(dmaTag <= 31, "DMA Tag is not reserved");
			__CELL_SWCACHE_ASSERT((uintptr_t)mPPUAddress >= 0x40000, "lsa:%p is specified to Variable", mPPUAddress);

			const int dataSize = ceil16(sizeof(type) + ((uintptr_t)mPPUAddress&15));

			__CELL_SWCACHE_VARIABLE_DPRINTF_1("beginDMARead(): ea:%p -> loading to ptr:%p,size:%#x,tag:%d",
				mPPUAddress, &mBuf[(uintptr_t)mPPUAddress&15], sizeof(type), dmaTag);
			// This branch is actually evaluated at compile time
			if (((sizeof(type) + 30) & ~15) <= 16 * 1024)
			{
				cellDmaGetf(mBuf, (uintptr_t)floor16((void *)mPPUAddress), dataSize, dmaTag, 0, 0);
			}
			else
			{
				cellDmaLargeGetf(mBuf, (uintptr_t)floor16((void *)mPPUAddress), dataSize, dmaTag, 0, 0);
			}
			mU32 = IS_LOADING | dmaTag;
		#ifdef _DEBUG
			cellDmaWaitTagStatusAll(1 << dmaTag);
		#endif
			yield();
		#else
			// On the PPU, just do a memcpy
			__CELL_SWCACHE_VARIABLE_DPRINTF_1("beginDMARead(): ea:%p -> loading to ptr:%p,size:%#x",
				mPPUAddress, &mVar, sizeof(type));
			memcpy(&mVar, mPPUAddress, sizeof(type));
		#endif
		}

		//! @brief Start the DMA transfer. This is a memcpy on the PPU
		__CELL_SWCACHE_ALWAYS_INLINE void	beginDMAWrite()
		{
		#ifdef __SPU__
			register unsigned int dmaTag = mDmaTag;
			__CELL_SWCACHE_ASSERT(dmaTag <= 31, "DMA Tag is not reserved");
			__CELL_SWCACHE_VARIABLE_DPRINTF_1("beginDMAWrite(): ptr:%p => ea:%p, size:%#x,tag:%d",
				&mBuf[(uintptr_t)mPPUAddress&15], mPPUAddress, sizeof(type), dmaTag);
			cellDmaUnalignedPutf(&mBuf[(uintptr_t)mPPUAddress&15], (uintptr_t)mPPUAddress, sizeof(type), dmaTag, 0, 0);
			mU32 = IS_LOADING | dmaTag;
		#ifdef _DEBUG
			cellDmaWaitTagStatusAll(1 << dmaTag);
		#endif
			yield();
		#else
			// On the PPU, just do a memcpy
			__CELL_SWCACHE_VARIABLE_DPRINTF_1("beginDMAWrite(): ptr:%p => ea:%p, size:%#x",
				&mVar, mPPUAddress, sizeof(type));
			memcpy(mPPUAddress, &mVar, sizeof(type));
		#endif
		}

		//! @brief Make sure that the DMA read has completed correctly.  This is a no-op on the PPU
		__CELL_SWCACHE_ALWAYS_INLINE void	verifyDMA()
		{
		#ifdef __SPU__
			if (__builtin_expect(mU32 & IS_LOADING, 0))
			{
				__CELL_SWCACHE_VARIABLE_DPRINTF_1("verifyDMA(): dmaTag=%d", mDmaTag);
				register unsigned int dmaTag = mDmaTag;
				cellDmaWaitTagStatusAll(1 << dmaTag);
				mU32 &= ~IS_LOADING;
			}
		#endif
		}

		// member variables
	#ifdef __SPU__
		uint8_t mBuf[(sizeof(type)+30)&~15];
		__extension__ union {
			__extension__ struct {
				uint32_t __reserved__ :13;
				uint32_t mIsLoading   :1;
				uint32_t mDmaTag      :18;
			};
			uint32_t mU32;
		};
	#else
		type	mVar;
	#endif

		type	*mPPUAddress;
}
#ifdef __SPU__
__attribute__((aligned(16)))
#endif
;

__CELL_SWCACHE_END

#endif		// for #define header
