/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2009 Sony Computer Entertainment Inc.
 * All Rights Reserved.
 */

#ifndef __CELL_SWCACHE_DMA_TAG_H__
#define __CELL_SWCACHE_DMA_TAG_H__

/* standard C++ header */
#include <stdint.h>
#include <stdbool.h>

#include <spu_intrinsics.h>

#include <cell/dma.h>

#include <cell/swcache/define.h>

__CELL_SWCACHE_BEGIN

//! @brief Class to hold the dma tag info for all derived classes
class DMATag
{
	public :

		/*! @brief Allocate DMA Tag ID for swcache system
		@param[in] dmaTag DMA Tag ID for system. Default is 31
		@retval CELL_OK                  Success
		@retval CELL_SWCACHE_ERROR_INVAL dmaTag is greater than 31
		*/
		static __attribute__((noinline)) int initialize(unsigned int dmaTag = 31)
		{
			if (dmaTag > 31) return CELL_SWCACHE_ERROR_INVAL;
			reserveChannels(1u << dmaTag);
			internalAllocateSystemTag((int)dmaTag);
			return CELL_OK;
		}

		/*! @brief Wait for DMA completion of system DMA Tag ID, and release system DMA Tag ID
		@retval CELL_OK                  Success
		@retval CELL_SWCACHE_ERROR_PERM  This function is called before initialize() is called
		*/
		static __attribute__((noinline)) int finalize()
		{
			int dmaTag = internalAllocateSystemTag();
			if (dmaTag < 0) return CELL_SWCACHE_ERROR_PERM;
			cellDmaWaitTagStatusAll(1u << dmaTag);
			releaseChannel(dmaTag);
			return CELL_OK;
		}

		/*! @brief Reserve channels by the user's spu code.  This assumes no swcache::Pointers have been initialized
		@param[in] channels A bitfield containing the reserved channels.  For example, to reserve channel 31, pass (1u << 31)
		*/
		static void reserveChannels(unsigned int channelsBitfield)
		{
			// go through each of the channels in the bitfield and reserve them
			for (int i = 0; i < 32; ++i)
			{
				if (channelsBitfield & 0x01)
				{
					internalAllocateTag(false, 0, true, i);
				}
				channelsBitfield = channelsBitfield >> 1;
			}
		}

	protected :

		/*! @brief Reserve a DMA channel
		@param[out] channel	The reserved channel.  -1 if there is an error or no more open channels
		*/
		static inline __CELL_SWCACHE_ATTRIBUTE_ALWAYS_INLINE int reserveChannel()
		{
			// find an open channel and reserve it
			return(internalAllocateTag(true, 0, false, 0));
		}

		/*! @brief Release a DMA channel that was reserved
		@param[in] channel	The channel to release
		*/
		static inline __CELL_SWCACHE_ATTRIBUTE_ALWAYS_INLINE void releaseChannel(int channel)
		{
			internalAllocateTag(false, channel, false, 0);
		}

		static inline __CELL_SWCACHE_ATTRIBUTE_ALWAYS_INLINE unsigned int getSystemDmaTag()
		{
			return internalAllocateSystemTag();
		}

	private :

		/*! @brief INTERNAL FUNCTION to handle DMA channel allocation
		@param[in] isReserve		true if reserving a channel, false if releasing it
		@param[in] releaseChannel	If releasing, the channel to release
		@param[in] forceReserve		Force a reservation of the forceChannel dma tag
		@param[in] forceChannel		The channel to force a reservation on
		@param[out] channel			The reserved channel.  -1 if there is an error or no more open channels
		*/
		static inline __CELL_SWCACHE_ATTRIBUTE_ALWAYS_INLINE int internalAllocateTag(bool isReserve, int releaseChannel, bool forceReserve, int forceChannel)
		{
			// this is actually a bit field to represent the DMA channels
			static uint32_t sDataChannel = 0;
			static uint32_t sSystemDataChannel = 0;
			static const int scNumChannels = sizeof(sDataChannel) * 8;
			
			// just to make sure we don't go over the number of DMA channels
			__CELL_SWCACHE_COMPILE_ASSERT(scNumChannels <= 32);
			
			if (__builtin_expect(forceReserve, 0))
			{
				__CELL_SWCACHE_ASSERT(forceChannel < scNumChannels, "%s::internalAllocateTag(): dmaTag=%d exceeds its max=%d\n", "cell::swcache::DMATag", forceChannel, scNumChannels);

				// force a reservation of the channel
				const unsigned int bitChannel = 0x01 << forceChannel;
				sSystemDataChannel = sSystemDataChannel | bitChannel;
			}
			else if (__builtin_expect(isReserve, 1))
			{
				register int channel = 31-(int)spu_extract(spu_cntlz(spu_xor(spu_promote(sDataChannel|sSystemDataChannel, 0), (unsigned)-1)), 0);
				if (__builtin_expect(channel > 0, 1))
				{
					sDataChannel |= 1u << channel;
					return channel;
				} else {
					// return any DMA tag except ones for system
					return 31-(int)spu_extract(spu_cntlz(spu_promote(sDataChannel, 0)), 0);
				}
			}
			else
			{
				__CELL_SWCACHE_ASSERT(releaseChannel < scNumChannels, "%s::internalAllocateTag(): dmaTag=%d exceeds its max=%d\n", "cell::swcache::DMATag", forceChannel, scNumChannels);
				
				// release the channel
				const unsigned int bitChannel = 0x01 << releaseChannel;

				// set the channel back to 0
				const unsigned int bitMask = ~bitChannel;
				sDataChannel = sDataChannel & bitMask;
			}

			// return an invalid channel
			return(-1);
		}

		static int internalAllocateSystemTag(int dmaTag = -1)
		{
			static int sSystemDmaTag = -1;
			int old = sSystemDmaTag;
			if (dmaTag >= 0)
			{
				sSystemDmaTag = dmaTag;
			}
			return old;
		}

};

//! @brief class to hold yield function pointer
class Resource : protected DMATag
{
public:
	typedef void (*FuncPtr)();

	//! @brief register yield function
	static void registerYieldFunc(FuncPtr pYieldFunc)
	{
		internalRegisterYieldFunc(pYieldFunc);
	}

	//! @brief yield is executed if registered
	// @retval CELL_OK                 yield was executed
	// @retval CELL_SWCACHE_ERROR_PERM yield was not executed
	static inline __CELL_SWCACHE_ATTRIBUTE_ALWAYS_INLINE int yield()
	{
		register FuncPtr pYieldFunc;
		if (__builtin_expect((uintptr_t)(pYieldFunc = internalRegisterYieldFunc(NULL)), 0))
		{
			(*pYieldFunc)();
			return CELL_OK;
		} else {
			return CELL_SWCACHE_ERROR_PERM;
		}
	}

private:
	//! @brief internal yield function pointer wrapper
	static inline __CELL_SWCACHE_ATTRIBUTE_ALWAYS_INLINE
		FuncPtr internalRegisterYieldFunc(FuncPtr pYieldFunc)
	{
		static FuncPtr spYieldFunc = NULL;
		if (__builtin_expect((uintptr_t)pYieldFunc, 0))
			spYieldFunc = pYieldFunc;
		return spYieldFunc;
	}

};

template<class tCache> class CacheResource : protected Resource
{
protected:
	// wrapper for static variable
	static inline __CELL_SWCACHE_ATTRIBUTE_ALWAYS_INLINE uint32_t setFetchDmaTag(unsigned int dmaTag)
	{
		static uint32_t sFetchDmaTagMask = 0;
		uint32_t old = sFetchDmaTagMask;
		sFetchDmaTagMask = (dmaTag < 32) ? (sFetchDmaTagMask | (1<<dmaTag)) : 0;
		return old;
	}
public:
	static __attribute__((noinline)) int initialize(void *pMemory, size_t size)
	{
		DMATag::initialize();
		return tCache::initialize(pMemory, size);
	}

	static __attribute__((noinline)) int finalize(unsigned int dmaTag = -1)
	{
		int ret;
		ret = tCache::finalize((dmaTag == (unsigned int)-1) ? DMATag::getSystemDmaTag() : dmaTag);
		if (ret) return ret;
		return DMATag::finalize();
	}
};

__CELL_SWCACHE_END

#endif	// for header #define
