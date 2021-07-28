/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2009 Sony Computer Entertainment Inc.
 * All Rights Reserved.
 */

#ifndef __CELL_SWCACHE_CAM_H__
#define __CELL_SWCACHE_CAM_H__

/* standard C++ header */
#include <stdint.h>

#include <spu_intrinsics.h>

#include <cell/swcache/define.h>

__CELL_SWCACHE_BEGIN

/*! @brief 16ways set associative content addressable memory
@param[in] tEntry Type of entry in CAM
@param[in] tKey   Data type of key
@param[in] tSize  Number of sets(in power of 2)
*/
template<class tEntry, typename tKey, unsigned int tSize> class ContentAddressableMemory16
{
public:
	// constants
	static const unsigned int NUM_OF_ENTRIES = 16 * (1<<tSize); //!< size of CAM

	/*! @brief grab entry with key
	@param[in] key Key value
	@return Pointer to grabbed entry,NULL is returned if CAM is full
	*/
	tEntry *grab(tKey key)
	{
		register unsigned set = setOf(key);
		register vec_uchar16 v_hash = mHtab16[set];
		register unsigned int freeSlot = spu_extract(spu_cntlz(spu_gather(spu_cmpeq(v_hash, spu_splats((uint8_t)0)))), 0)-16;
		if (__builtin_expect(freeSlot != 16, 1))
		{
			v_hash = spu_insert((uint8_t)hashOf(key), v_hash, freeSlot);
			mHtab16[set] = v_hash;
			mContents[set][freeSlot].key = key;
			return &mContents[set][freeSlot];
		} else {
			return 0;
		}
	}


	/*! @brief get sibling entries in same set
	@param[in] key Key value
	*/
	tEntry *getSiblingsOf(tKey key)
	{
		return mContents[setOf(key)];
	}

	/*! @brief release entry
	@param[in] pContent Pointer to entry which is to be released
	*/
	void release(tEntry *pContent)
	{
		unsigned set  = getSetOf(pContent);
		unsigned slot = getSlotOf(pContent);
		mHtab16[set] = spu_insert(0, mHtab16[set], slot);
	}

	/*! @brief release entry
	@param[in] key Key value of entry which is to be released
	*/
	void release(tKey key)
	{
		register unsigned set = setOf(key);
		register uint8_t hash = hashOf(key);
		register vec_uint4 v_hashMatchMask = spu_gather(spu_cmpeq(mHtab16[set], spu_splats(hash)));
		register tEntry *pSet = mContents[set];
		register unsigned slot;

		// iterate for slots(up to 16 slots) where hash value is matched
		while ((slot = spu_extract(spu_cntlz(v_hashMatchMask), 0) - 16) != 16) {

			// cache hit candidate address
			if (__builtin_expect(pSet[slot].key == key, 1))
			{
				mHtab16[set] = spu_andc(mHtab16[set], spu_maskb(0x8000u >> slot));
				return;
			}
			// update hash match mask for next iteration
			v_hashMatchMask = spu_andc(v_hashMatchMask, spu_splats(0x8000u >> slot));
		}
	}

	/*! @brief Search for entry with specified key value
	@param[in] key Key value
	@return Pointer to hit entry,NULL is returned in case of miss
	*/
	tEntry *get(tKey key)
	{
		register unsigned set = setOf(key);
		register uint8_t hash = hashOf(key);
		register vec_uint4 v_hashMatchMask = spu_gather(spu_cmpeq(mHtab16[set], spu_splats(hash)));
		register tEntry *__restrict__ pSet = mContents[set];
		register unsigned slot = spu_extract(spu_cntlz(v_hashMatchMask), 0) - 16;
		register tEntry *__restrict__ pResult = &pSet[slot];

		if (__builtin_expect(pResult->key == key, 1)) return (slot < 16) ? pResult : 0;
		v_hashMatchMask = spu_andc(v_hashMatchMask, spu_splats(0x8000u >> slot));

		// iterate for slots(up to 16 slots) where hash value is matched
		while ((slot = spu_extract(spu_cntlz(v_hashMatchMask), 0) - 16) != 16) {

			// cache hit candidate address
			if (__builtin_expect(pSet[slot].key == key, 1)) return &pSet[slot];
			// update hash match mask for next iteration
			v_hashMatchMask = spu_andc(v_hashMatchMask, spu_splats(0x8000u >> slot));
		}

		return 0;
	}

	/*! @brief get pointer to array of entries
	@return Pointer to array of entries
	*/
	tEntry *getContents()
	{
		return &mContents[0][0];
	}

	/*! @brief get pointer to hash table of specified entry
	@param[in] pContent Pointer to entry
	@return Pointer to hash table
	*/
	vec_uchar16 *getHtab16Of(tEntry *pContent)
	{
		return &mHtab16[getSetOf(pContent)];
	}

	/*! @brief get set of sepcified entry
	@param[in] pContent Pointer to entry
	@return Set
	*/
	unsigned getSetOf(tEntry *pContent)
	{
		return (((uintptr_t)pContent - (uintptr_t)mContents) / sizeof(tEntry)) / 16;
	}

	/*! @brief get slot of sepcified entry
	@param[in] pContent Pointer to entry
	@return Slot
	*/
	unsigned getSlotOf(tEntry *pContent)
	{
		return (((uintptr_t)pContent - (uintptr_t)mContents) / sizeof(tEntry)) % 16;
	}

private:
	static inline __CELL_SWCACHE_ATTRIBUTE_ALWAYS_INLINE unsigned setOf(tKey key) { return (key>>4) & ((1<<tSize)-1); }
	static inline __CELL_SWCACHE_ATTRIBUTE_ALWAYS_INLINE unsigned hashOf(tKey key) { return (((key>>(4+tSize)) & 0xff) ? ((key>>(4+tSize)) & 0xff) : 1); } // avoid zero as hash value

	// member variables
	vec_uchar16 mHtab16  [1<<tSize];     //!< hash table
	tEntry      mContents[1<<tSize][16]; //!< content
};

__CELL_SWCACHE_END

#endif /* __CELL_SWCACHE_CAM_H__ */

