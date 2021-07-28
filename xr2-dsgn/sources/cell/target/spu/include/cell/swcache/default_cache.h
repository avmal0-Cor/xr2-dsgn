/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2009 Sony Computer Entertainment Inc.
 * All Rights Reserved.
 */

#ifndef __CELL_SWCACHE_DEFAULT_CACHE_H__
#define __CELL_SWCACHE_DEFAULT_CACHE_H__

// standard C++ header
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h> // for size_t

#ifdef __SPU__
#include <spu_intrinsics.h>
#include <spu_mfcio.h>
#include <cell/dma.h>
#endif

#include <cell/swcache/define.h>
#include <cell/swcache/types.h>
#include <cell/swcache/cam.h>
#include <cell/swcache/patch_vtable.h>

__CELL_SWCACHE_BEGIN

class DefaultHeap;

//! @brief default software cache implementation
template<class tHeap = DefaultHeap, unsigned int tDirSize = 4, unsigned int tRefDirSize = 5> class DefaultCache
{
private:
	struct DirEntry;
	typedef ContentAddressableMemory16<DirEntry,uint32_t,tDirSize> Directory;
#if CELL_SWCACHE_SAFE_LV > 0
	struct RefDirEntry;
	typedef ContentAddressableMemory16<RefDirEntry,uint16_t,tRefDirSize> RefDirectory;
#endif
public:
	// constants
	static const size_t       SIZE_OF_DIR   = sizeof(Directory);
#if CELL_SWCACHE_SAFE_LV > 0
	static const size_t       SIZE_OF_REF_DIR   = sizeof(RefDirectory);
#else
	static const size_t       SIZE_OF_REF_DIR   = 0;
#endif
	static const unsigned int MAX_DIR_ENTRY = 16 * (1<<tDirSize);

	union MemBlockHeader;

	/*! @brief Initialize function to hand memory to cache
	@param[in] pMemory                  Pointer to memory given to cache by user
	@param[in] size                     Size of memory
	@retval    CELL_OK                  Success
	@retval    CELL_SWCACHE_ERROR_NOMEM Memory size is insufficient
	*/
	static __attribute__((noinline)) int initialize(void *pMemory, size_t size)
	{
		int ret;
		uintptr_t ptr = (uintptr_t)pMemory;

		size_t size_of_dir = sizeof(Directory);
#if CELL_SWCACHE_SAFE_LV > 0
		size_of_dir += sizeof(RefDirectory);
#endif
		if (size < size_of_dir) return CELL_SWCACHE_ERROR_NOMEM;
		initialize((void *)ptr);
		ptr += size_of_dir; size -= size_of_dir;

		ret = tHeap::initialize((void *)ptr, size);
		if (ret == CELL_OK)
		{
			__CELL_SWCACHE_DPRINTF("%s Heap initialized(%p-%p)", "cell::swcache::DefaultCache:", ptr, ptr + size);
		}
		return ret;
	}

	/*! @brief finalize cache
	@param[in] dmaTag                DMA tag used for writeback
	@param[in] force                 Obsolete parameter, has no effect
	@retval CELL_OK                  Success
	*/
	static __attribute__((noinline)) int finalize(unsigned int dmaTag, unsigned int force = 0)
	{
		__CELL_SWCACHE_DPRINTF("%s::finalize(dmaTag=%u)", "cell::swcache::DefaultCache:", dmaTag);
		(void)force;
		MemBlockHeader *pHeader = 0;
		while((pHeader = tHeap::getPrevBlockOf(pHeader)) != 0)
		{
			if (tHeap::isAllocated(pHeader))
			{
				struct DirEntry *pDirEnt = pHeader->pDirEnt;
				if (pDirEnt)
				{
					pDirEnt->refCount = 0;
					__CELL_SWCACHE_DPRINTF_2("%s released cache object(lsa:%#x,ea:%#x,size:%#x%s%s)", "cell::swcache::DefaultCache:", pDirEnt->lsa, pDirEnt->ea, pHeader->size, pDirEnt->hasVtable?",VTAB":"", pDirEnt->needWriteBack?"WriteBack":"");
					pHeader->releaseObject(dmaTag);
					tHeap::free((void *)pHeader);
				}
			}
		}

		tHeap::finalize();

		return CELL_OK;
	}

	/*! @brief flush all cache
	@param[in] dmaTag                DMA tag used for writeback
	*/
	static __attribute__((noinline)) void flushAll(unsigned int dmaTag)
	{
		__CELL_SWCACHE_DPRINTF("%s::flushAll(dmaTag=%u)", "cell::swcache::DefaultCache:", dmaTag);
		MemBlockHeader *pHeader = 0;
		while((pHeader = tHeap::getPrevBlockOf(pHeader)) != 0)
		{
			if (tHeap::isAllocated(pHeader))
			{
				struct DirEntry *pDirEnt = pHeader->pDirEnt;
				if (pDirEnt)
				{
					void *ptr = pDirEnt->getPtr();
					pDirEnt->waitForDMA();
					if (__builtin_expect(pDirEnt->u32 & IS_WRITABLE, 1))
					{
						__CELL_SWCACHE_DPRINTF_2("%s write back cache object(lsa:%#x,ea:%#x,size:%#x%s)", "cell::swcache::DefaultCache:", pDirEnt->lsa, pDirEnt->ea, pHeader->size, pDirEnt->hasVtable?",VTAB":"");
						if (pDirEnt->hasVtable)
						{
							// revert VTABLE pointer
							*((void **)ptr) = ClassIdTabHeader::getVtabHeader(ptr)->mpOrgVtab;
						}
						// write back
						pHeader->writeBack(dmaTag);
					}
				}
			}
		}
#ifdef _DEBUG
		CELL_SWCACHE_DMA_WAIT_TAG_STATUS_ALL(1 << dmaTag);
#endif
	}

	/*! @brief refresh all cache
	@param[in] dmaTag                DMA tag used for writeback
	*/
	static __attribute__((noinline)) void refreshAll(unsigned int dmaTag)
	{
		__CELL_SWCACHE_DPRINTF("%s::refreshAll(dmaTag=%u)", "cell::swcache::DefaultCache:", dmaTag);
		ClassIdTabHeader *pClassIdTab = 0;
		MemBlockHeader *pHeader = 0;
		while((pHeader = tHeap::getPrevBlockOf(pHeader)) != 0)
		{
			if (tHeap::isAllocated(pHeader))
			{
				struct DirEntry *pDirEnt = pHeader->pDirEnt;
				if (pDirEnt)
				{
					// fetch
					__CELL_SWCACHE_DPRINTF_2("%s read back cache object(lsa:%#x,ea:%#x,size:%#x%s)", "cell::swcache::DefaultCache:", pDirEnt->lsa, pDirEnt->ea, pHeader->size, pDirEnt->hasVtable?",VTAB":"");
					uint32_t ea = pDirEnt->ea;
					void *lsa = pHeader->getPtr();
					size_t tsize = ceil16(pHeader->size + (ea&15));
					if (__builtin_expect(tsize <= 16 * 1024, 1))
						cellDmaGetf(lsa, (uintptr_t)floor16((void *)(uintptr_t)ea), tsize, dmaTag, 0, 0);
					else
						cellDmaLargeGetf(lsa, (uintptr_t)floor16((void *)(uintptr_t)ea), tsize, dmaTag, 0, 0);

					if (pDirEnt->hasVtable)
					{
						// patch VTABLE after refresh
						int ret;
						if (__builtin_expect(pClassIdTab == 0, 0))
						{
							pClassIdTab = ClassIdTabHeader::getClassIdTabOnSPU();
							__CELL_SWCACHE_ASSERT(pClassIdTab, "Class Id table no found");
						}
						CELL_SWCACHE_DMA_WAIT_TAG_STATUS_ALL(1 << dmaTag);

						ret = pClassIdTab->patchVtab((Vtab **)pDirEnt->getPtr());
						__CELL_SWCACHE_ASSERT(ret == CELL_OK, " Failed to patch pVTABLE!!");
					}
				}
			}
		}
#ifdef _DEBUG
		CELL_SWCACHE_DMA_WAIT_TAG_STATUS_ALL(1 << dmaTag);
#endif
	}

	/*! @brief Grab cache object pointed by 'ptr', 'ptr' is either EA or LSA.
	@param[in,out] ptr                            Pointer to be checked. if ptr is EA, LSA which has cached object is stored to 'ptr' on return.
	@param[in] size                               Object size
	@param[in] needWriteBack                      Write back attribute is set to cache object if IS_WRITABLE is specified.
	@param[in,out] dmaTag                         DMA Tag ID for main memory data loading. if DMA is already running, running DMA Tag ID
	                                              is set on return.
	@param[out] ea                                Variable where EA of cached object is stored.
	@param[in] hasVtable                          0:object doesn't have pointer to VTABLE,other value:object has pointer to VTABLE
	@param[in] noCacheCheck                       0:cache is checked,other value:cache is not checked
	@retval CACHED|[IS_LOADING]|[IS_WRITABLE]     ptr is cached data
	                                              - IS_LOADING is set if fetch DMA is on going
	                                              - IS_WRITABLE is set if cache data is flagged as write back enable
	@retval NOT_CACHED|[IS_LOADING]|[IS_WRITABLE]|[WRITEBACK_KICKED] ptr is not cached, so memory is allocated, and DMA is kicked
	                                              - IS_WRITABLE is set if cache data is flagged as write back enable
	                                              - IS_LOADING is set if fetch DMA is on going
	                                              - WRITEBACK_KICKED is set if write back occured due to object reallocation
	@retval CELL_SWCACHE_ERROR_NON_CACHED_POINTER ptr is SPU local data
	@retval CELL_SWCACHE_ERROR_NULL_POINTER       ptr is null pointer
	@retval CELL_SWCACHE_ERROR_NO_HTAB            Hash table is full
	@retval CELL_SWCACHE_ERROR_NOMEM              No heap memory
	@retval CELL_SWCACHE_ERROR_VTAB               Failed to patch VTABLE pointer
	*/
	static inline __CELL_SWCACHE_ATTRIBUTE_ALWAYS_INLINE int grab(void *&ptr, size_t size, unsigned int needWriteBack, unsigned int &dmaTag, uint32_t &ea, unsigned int hasVtable = 0, unsigned int noCacheCheck = 0)
	{
		__CELL_SWCACHE_DPRINTF_2_IN("%s:grab(%s:%p,size=%#x,%s,dmaTag=%d,hasVtable=%d)","cell::swcache::DefaultCache:", 
			((uintptr_t)ptr < 0x40000) ? "lsa" : "ea ", ptr, size, needWriteBack ? "RW_MODE" : "RO_MODE", dmaTag, hasVtable);
		int status = checkAndLoad(ptr, size, needWriteBack, dmaTag, 1, ea, hasVtable, noCacheCheck);
		__CELL_SWCACHE_DPRINTF_2_OUT(" lsa:%p,ea:%#x,dmaTag=%d,refCount=%d", ptr, ea, dmaTag,MemBlockHeader::getHeader(ptr)->pDirEnt->refCount);
		return status;
	}

	/*! @brief Grab cache if cache hit, otherwise returns negative value
	@param[in] ptr                                Pointer to be checked.
	@param[in] needWriteBack                      Write back attribute is set to cache object if IS_WRITABLE is specified.
	@param[out] pEa                               Main memory address is stored if cache hit
	@return                                       Pointer to cached object, NULL is returned if cache hit is not found.
	                                              IS_LOADING is logical ored if fetch DMA is on going
	*/
	static inline __CELL_SWCACHE_ATTRIBUTE_ALWAYS_INLINE void *grabIfCached(void *ptr, unsigned int needWriteBack, uint32_t *pEa = 0)
	{
		__CELL_SWCACHE_DPRINTF_2_IN("%s:grabIfCached(%s:%p,%s)","cell::swcache::DefaultCache:", 
			((uintptr_t)ptr < 0x40000) ? "lsa" : "ea ", ptr, needWriteBack ? "RW_MODE" : "RO_MODE");

		register uint32_t ea = (uintptr_t)ptr;
		register DirEntry *pDirEnt = getDirectory()->get(ea);
		register MemBlockHeader *pHeader;
		if (pDirEnt) {
			// Cache hit!

#ifndef NDEBUG
			pHeader = pDirEnt->getHeader();
#endif
			__CELL_SWCACHE_ASSERT(pHeader->pDirEnt == pDirEnt,
				"Cache hit for ea:%#x, but inconsistent MemBlockHeader\n"
				"\theader@%p(pDirEnt:%p, pHtab16:%p,slot:%d,size:%#x)\n"
				"\thit directory@%p, dir->refcount = %d, dir->ea = %#x, dir->lsa = %p",
				ea,
				pHeader, pHeader->pDirEnt, pHeader->pHtab16, pHeader->slot, pHeader->size,
				pDirEnt, pDirEnt->refCount, pDirEnt->ea, pDirEnt->getPtr());
		} else if ((uintptr_t)ptr < 0x40000) {
			// if address is LSA
			pHeader = isCachedPointer(ptr);
			if (__builtin_expect(!pHeader, 0)) return 0;
			// Cache hit!

			pDirEnt = pHeader->pDirEnt;
		} else {
			// Cache miss
			return 0;
		}
		register void *spu_ptr = pDirEnt->addGrab(needWriteBack);
		if (pEa) *pEa = pDirEnt->ea;
		__CELL_SWCACHE_DPRINTF_2("  Addr is LSA: Cache hit    ea:%#x ptr:%#x,size:%#x refCount=%d%s%s",
			pDirEnt->ea, (uintptr_t)spu_ptr&0x3ffff, pHeader->size, pDirEnt->refCount,
			pDirEnt->hasVtable?" HAS_VTABLE":"", ((uintptr_t)spu_ptr&IS_LOADING)?" IS_LOADING":"");

		__CELL_SWCACHE_DPRINTF_2_OUT(" lsa:%#x,ea:%#x,dmaTag=%d,refCount=%d",
			(uintptr_t)ptr&0x3ffff,
			MemBlockHeader::getHeader((void *)((uintptr_t)spu_ptr&0x3ffff))->pDirEnt->ea,
			MemBlockHeader::getHeader((void *)((uintptr_t)spu_ptr&0x3ffff))->pDirEnt->dmaTag,
			MemBlockHeader::getHeader((void *)((uintptr_t)spu_ptr&0x3ffff))->pDirEnt->refCount);

		return spu_ptr;
	}

	/*! @brief Grab cache object without heap allocation. (This function is used when fetch is already finished in advanced)
	@param[in,out] ptr                            Pointer for fecthed object. this object is registered to directory unless one with same ea is registered,
                                                  otherwise the one already registered is set to ptr.
	@param[in] ea                                 Main memory pointer of object
	@param[in] size                               Object size
	@param[in] needWriteBack                      Write back attribute is set to cache object if IS_WRITABLE is specified.
	@param[in,out] dmaTag                         DMA Tag ID for main memory data loading. if DMA is already running, running DMA Tag ID
	                                              is set on return.
	@param[in] hasVtable                          0:object doesn't have pointer to VTABLE,other value:object has pointer to VTABLE
	@retval CACHED|[IS_LOADING]|[IS_WRITABLE]     ea is found in directory
	                                              - IS_LOADING is set if fetch DMA is on going
	                                              - IS_WRITABLE is set if cache data is flagged as write back enable
	@retval NOT_CACHED|[IS_WRITABLE]              ea is not found id directory, so new entry is created on directory with lsa=ptr
	                                              - IS_WRITABLE is set if cache data is flagged as write back enable
	@retval CELL_SWCACHE_ERROR_NULL_POINTER       ptr is null pointer
	@retval CELL_SWCACHE_ERROR_ALIGN              ptr and ea has inconsistent alignement
	@retval CELL_SWCACHE_ERROR_INVAL              size is smaller than hash table and directory size, or ea is less than 0x40000
	@retval CELL_SWCACHE_ERROR_NO_HTAB            Hash table is full
	@retval CELL_SWCACHE_ERROR_VTAB               Failed to patch VTABLE pointer
	*/
	static inline __CELL_SWCACHE_ATTRIBUTE_ALWAYS_INLINE int grabWithNoAlloc(void *&ptr, uint32_t ea, size_t size, unsigned int needWriteBack, unsigned int &dmaTag, unsigned int hasVtable = 0)
	{
		__CELL_SWCACHE_DPRINTF_2_IN("%s:grabWithNoAlloc(lsa:%p,ea:%#x,size=%#x,%s,dmaTag=%d,hasVtable=%d)", "cell::swcache::DefaultCache:",
			ptr, ea, size, needWriteBack ? "RW_MODE" : "RO_MODE", dmaTag, hasVtable);
		if (__builtin_expect(ptr == 0 || ea == 0, 0)) return CELL_SWCACHE_ERROR_NULL_POINTER;
		else if (__builtin_expect(ea < 0x40000, 0)) return CELL_SWCACHE_ERROR_INVAL;
		else if (__builtin_expect((ea&15) != ((uintptr_t)ptr&15), 0)) return CELL_SWCACHE_ERROR_ALIGN;
		void *pFetched = ptr;
		ptr = (void *)(uintptr_t)ea;
		int status = checkAndLoad(ptr, size, needWriteBack, dmaTag, 1, ea, hasVtable, /* noCacheCheck=*/0, pFetched);
		__CELL_SWCACHE_DPRINTF_2_OUT(" lsa:%p,dmaTag=%d,refCount=%d", ptr, dmaTag,MemBlockHeader::getHeader(ptr)->pDirEnt->refCount);
		return status;
	}

	/*! @brief Query cache about object pointed by 'ptr', 'ptr' is either EA or LSA.
	@param[in] ptr                                Pointer to be checked.
	@param[in] size                               Object size
	@param[out] dmaTag                            DMA Tag ID for main memory data loading if DMA is already running
	@param[out] ea                                Variable where EA of cached object is stored.
	@retval CACHED|[IS_LOADING]|[IS_WRITABLE]     ptr is cached data
	                                              - IS_LOADING is set if DMA is ongoing
	                                              - IS_WRITABLE is set if cache data is flagged as write back enable
	@retval NOT_CACHED                            ptr is not cached
	@retval CELL_SWCACHE_ERROR_NON_CACHED_POINTER ptr is SPU local data
	@retval CELL_SWCACHE_ERROR_NULL_POINTER       ptr is null pointer
	*/
	static inline __CELL_SWCACHE_ATTRIBUTE_ALWAYS_INLINE int query(void *ptr, size_t size, unsigned int &dmaTag, uint32_t &ea)
	{
		__CELL_SWCACHE_DPRINTF_2_IN("%s:query(%s:%p,size=%#x)", "cell::swcache::DefaultCache:",
			((uintptr_t)ptr < 0x40000) ? "lsa" : "ea ", ptr, size);
		dmaTag = 0;
		int status = checkAndLoad(ptr, size, 0, dmaTag, 0, ea);
		__CELL_SWCACHE_DPRINTF_2_OUT(" %s%s%s,ea:%#x,dmaTag=%d",
			(status >= 0 && (status&0xf) == CACHED) ? "CACHED" :
			(status == NOT_CACHED) ? "NOT_CACHED" :
			(status == CELL_SWCACHE_ERROR_NON_CACHED_POINTER) ? "NON_CACHED_POINTER" : "NULL_POINTER",
			(status >= 0 && (status&0xf) == CACHED && (status & IS_LOADING)) ? " IS_LOADING" : "",
			(status >= 0 && (status&0xf) == CACHED && (status & IS_WRITABLE)) ? " IS_WRITABLE" : "",
			ea, dmaTag);
		return status;
	}

	/*! @brief Wait for main memory loading DMA completion
	@param[in] ptr                                Pointer(LSA) to cached object
	@retval CELL_OK                               DMA is completed
	@retval DMA_COMPLETED
	@retval CELL_SWCACHE_ERROR_NON_CACHED_POINTER ptr is SPU local data
	@retval CELL_SWCACHE_ERROR_NULL_POINTER       ptr is null pointer
	*/
	static inline __CELL_SWCACHE_ATTRIBUTE_ALWAYS_INLINE int waitForGrab(void *ptr)
	{
		__CELL_SWCACHE_DPRINTF_2("%s:%s: ptr=%p", "cell::swcache::DefaultCache:", __func__, ptr);
		if (__builtin_expect(!ptr, 0)) return CELL_SWCACHE_ERROR_NULL_POINTER;
		else if (__builtin_expect(!isCachedPointer(ptr), 0)) return CELL_SWCACHE_ERROR_NON_CACHED_POINTER;
		register struct DirEntry *pDirEnt = MemBlockHeader::getHeader(ptr)->pDirEnt;
		return pDirEnt->waitForDMA();
	}

	/*! @brief Verify if main memory loading DMA is completed
	@param[in] ptr                                Pointer(LSA) to cached object
	@retval CELL_OK                               DMA is completed
	@retval IS_LOADING                            DMA is on going
	@retval CELL_SWCACHE_ERROR_NON_CACHED_POINTER ptr is SPU local data
	@retval CELL_SWCACHE_ERROR_NULL_POINTER       ptr is null pointer
	*/
	static inline __CELL_SWCACHE_ATTRIBUTE_ALWAYS_INLINE int verifyGrab(void *ptr)
	{
		__CELL_SWCACHE_DPRINTF_2("%s:%s: ptr=%p", "cell::swcache::DefaultCache:", __func__, ptr);
		if (__builtin_expect(!ptr, 0)) return CELL_SWCACHE_ERROR_NULL_POINTER;
		else if (__builtin_expect(!isCachedPointer(ptr), 0)) return CELL_SWCACHE_ERROR_NON_CACHED_POINTER;
		register struct DirEntry *pDirEnt = MemBlockHeader::getHeader(ptr)->pDirEnt;
		pDirEnt->checkForDMA();

		return pDirEnt->u32 & IS_LOADING;
	}

	/*! @brief Change cache object to writable(writeback enabled on release)
	@param[in] ptr                                Pointer(LSA) to cached object
	@retval CELL_OK                               Success
	@retval CELL_SWCACHE_ERROR_NULL_POINTER       ptr is null pointer
	@retval CELL_SWCACHE_ERROR_INVAL              ptr is main memory address
	@retval CELL_SWCACHE_ERROR_NON_CACHED_POINTER ptr is SPU local data
	*/
	static inline __CELL_SWCACHE_ATTRIBUTE_ALWAYS_INLINE int changeToWritable(void *ptr)
	{
		__CELL_SWCACHE_DPRINTF_2("%s:%s: ptr=%p", "cell::swcache::DefaultCache:", __func__, ptr);
		if (__builtin_expect(!ptr, 0)) return CELL_SWCACHE_ERROR_NULL_POINTER;
		else if (__builtin_expect((uintptr_t)ptr >= 0x40000, 0)) return CELL_SWCACHE_ERROR_INVAL;
		else if (__builtin_expect(!isCachedPointer(ptr), 0)) return CELL_SWCACHE_ERROR_NON_CACHED_POINTER;
		register struct DirEntry *pDirEnt = MemBlockHeader::getHeader(ptr)->pDirEnt;
		pDirEnt->u32 |= IS_WRITABLE;

		return CELL_OK;
	}

	/*! @brief Flush cache object to main memory
	@param[in] ptr                                Pointer(LSA) to cached object
	@param[in] dmaTag                             DMA Tag ID
	@retval CELL_OK                               Success
	@retval CELL_SWCACHE_ERROR_NULL_POINTER       ptr is null pointer
	@retval CELL_SWCACHE_ERROR_INVAL              ptr is main memory address
	@retval CELL_SWCACHE_ERROR_NON_CACHED_POINTER ptr is SPU local data
	*/
	static __attribute__((noinline)) int flush(void *ptr, unsigned int dmaTag)
	{
		__CELL_SWCACHE_DPRINTF_2("%s:%s: ptr=%p, dmaTag=%d", "cell::swcache::DefaultCache:", __func__, ptr, dmaTag);
		if (__builtin_expect(!ptr, 0)) return CELL_SWCACHE_ERROR_NULL_POINTER;
		else if (__builtin_expect((uintptr_t)ptr >= 0x40000, 0)) return CELL_SWCACHE_ERROR_INVAL;
		else if (__builtin_expect(!isCachedPointer(ptr), 0)) return CELL_SWCACHE_ERROR_NON_CACHED_POINTER;
		register MemBlockHeader *pHeader = MemBlockHeader::getHeader(ptr);
		register struct DirEntry *pDirEnt = pHeader->pDirEnt;
		pDirEnt->waitForDMA();
		if (__builtin_expect(pDirEnt->u32 & IS_WRITABLE, 1))
		{
			if (pDirEnt->hasVtable)
			{
				// revert VTABLE pointer
				void *pFakeVtab = *((void **)ptr);
				*((void **)ptr) = ClassIdTabHeader::getVtabHeader(ptr)->mpOrgVtab;
				// write back
				pHeader->writeBack(dmaTag);
				CELL_SWCACHE_DMA_WAIT_TAG_STATUS_ALL(1 << dmaTag);
				// set fake VTABLE pointer again
				*((void **)ptr) = pFakeVtab;
			} else {
				pHeader->writeBack(dmaTag);
				CELL_SWCACHE_DMA_WAIT_TAG_STATUS_ALL(1 << dmaTag);
			}
		}

		return CELL_OK;
	}

	/*! @brief Release cached object
	@param[in] ptr                                Pointer(LSA) to cached object
	@retval CELL_OK                               Release successful. write back DMA is kicked with dmaTag
	@retval CELL_SWCACHE_ERROR_INVAL              ptr is main memory address
	@retval CELL_SWCACHE_ERROR_NULL_POINTER       ptr is null pointer
	@retval CELL_SWCACHE_ERROR_NON_CACHED_POINTER ptr is SPU local data
	*/
	static int release(void *ptr, unsigned int noErrorCheck = 0)
	{
		__CELL_SWCACHE_DPRINTF_2("%s:%s: ptr=%p, refCount=%d", "cell::swcache::DefaultCache:", __func__, ptr, MemBlockHeader::getHeader(ptr)->pDirEnt->refCount);
		register MemBlockHeader *pHeader;
		if (noErrorCheck)
		{
			pHeader = ((union MemBlockHeader *)((uintptr_t)ptr & 0x3fff0))-1;
			pHeader->release();
		} else {
			pHeader = isCachedPointer(ptr);
			if (__builtin_expect(!ptr, 0)) return CELL_SWCACHE_ERROR_NULL_POINTER;
			else if (__builtin_expect((uintptr_t)ptr >= 0x40000, 0)) return CELL_SWCACHE_ERROR_INVAL;
			else if (__builtin_expect(pHeader == 0, 0)) return CELL_SWCACHE_ERROR_NON_CACHED_POINTER;
			if (__builtin_expect(pHeader != 0, 1)) pHeader->release();
		}

		return CELL_OK;
	}

	static inline __CELL_SWCACHE_ATTRIBUTE_ALWAYS_INLINE unsigned int getDmaTag(void *ptr)
	{
		return MemBlockHeader::getHeader(ptr)->pDirEnt->dmaTag;
	}

	/*! @brief Final check to make sure all cached objects are freed */
	static __attribute__((noinline)) void finalCheck()
	{
		__CELL_SWCACHE_DPRINTF_2("%s:%s:", "cell::swcache::DefaultCache:", __func__);
		unsigned int errorFlag = 0;
		struct DirEntry *pDirEnt = getDirectory()->getContents();
		for(unsigned i = 0; i < Directory::MAX_ENTRY; i++,pDirEnt++)
		{
			if (__builtin_expect(pDirEnt->refCount, 0))
			{
				errorFlag = 1;
				__CELL_SWCACHE_DPRINTF(
					"\n\n%s: Unfinalized alias data exists!!\n"
					"\tea=%#x,refCount=%d,lsa=%#x\n", __PRETTY_FUNCTION__,
					pDirEnt->ea, pDirEnt->refCount, pDirEnt->getPtr());
			}
		}
		__CELL_SWCACHE_ASSERT(errorFlag == 0, "Grabbed cache objects exist!!");
	}

	/*! @brief check if reference is registered
	@param[in] obj Pointer to Pointer or PatchObject
	@return        1:Reference exists,0:Reference doesn't exist
	*/
	static int existsReference(void *obj)
	{
		int ret = 0;
#if CELL_SWCACHE_SAFE_LV == 0
		(void)obj;
#else
		__CELL_SWCACHE_DPRINTF_2_IN("%s:%s: obj=%p", "cell::swcache::DefaultCache:", __func__, obj);
		RefDirEntry *pRefDirEnt = getRefDirectory()->get((uintptr_t)obj>>3);
		if (__builtin_expect(pRefDirEnt != 0, 1))
		{
			__CELL_SWCACHE_DPRINTF_2_OUT(" Reference exists");
			ret = 1;
		} else {
			__CELL_SWCACHE_DPRINTF_2_OUT(" Reference doesn't exists");
		}
#endif
		return ret;
	}

	/*! @brief register reference to RefDirectory
	@param[in] obj                        Pointer to Pointer or PatchObject
	@retval CELL_OK                       Successfully registered
	@retval CELL_SWCACHE_ERROR_MEM_LEAKED This reference is already registered
	*/
	static int registerReference(void *obj)
	{
		int ret = CELL_OK;
#if CELL_SWCACHE_SAFE_LV == 0
		(void)obj;
#else
		RefDirectory *pRefDir = getRefDirectory();
		__CELL_SWCACHE_ASSERT(pRefDir, "%s:%s: RefDirectory is not initialized", "cell::swcache::DefaultCache:", __func__);
		RefDirEntry *pRefDirEnt = pRefDir->get((uintptr_t)obj>>3);
		if (__builtin_expect(pRefDirEnt != 0, 0))
		{
			__CELL_SWCACHE_DPRINTF_2("%s:%s: Memory leaked obj=%p", "cell::swcache::DefaultCache:", __func__, obj);
			ret = CELL_SWCACHE_ERROR_MEM_LEAKED;
		} else {
			pRefDirEnt = pRefDir->grab((uintptr_t)obj>>3);
			__CELL_SWCACHE_ASSERT(pRefDirEnt, "%s:%s: RefDirectory is full", "cell::swcache::DefaultCache:", __func__);
			__CELL_SWCACHE_DPRINTF_2("%s:%s: Reference was created at %p", "cell::swcache::DefaultCache:", __func__, obj);
		}
#endif
		return ret;
	}

	/*! @brief unregister reference from RefDirectory
	@param[in] obj Poiner to Pointer or PatchObject
	*/
	static void unregisterReference(void *obj)
	{
#if CELL_SWCACHE_SAFE_LV == 0
		(void)obj;
#else
		RefDirectory *pRefDir = getRefDirectory();
		__CELL_SWCACHE_ASSERT(pRefDir , "RefDirectory is not initialized");
		RefDirEntry *pRefDirEnt = pRefDir->get((uintptr_t)obj>>3);
		__CELL_SWCACHE_ASSERT(pRefDirEnt, "Cache release error: obj@%p is not grabbed", obj);
		pRefDir->release(pRefDirEnt);
		__CELL_SWCACHE_DPRINTF_2("%s:%s: Reference was removed %p", "cell::swcache::DefaultCache:", __func__, obj);
#endif
	}
private:
	/*! @brief check if specified pointer is one for cached object
	@param[in] ptr pointer to be checked
	@return Pointer to memory block header if ptr is for cached object, otherwise returns NULL
	*/
	static inline __CELL_SWCACHE_ATTRIBUTE_ALWAYS_INLINE MemBlockHeader *isCachedPointer(void *ptr)
	{
		register MemBlockHeader *pHeader = MemBlockHeader::getHeader(ptr);
		return pHeader;
	}
public:
	static const uint32_t sMemBlockMagicWord = 0x3e3b; //!< Magic number embedded in memory block header
	//! @brief Memory block header(inserted in front of memory block)
	typedef union MemBlockHeader {

		/*! @brief calculate size of memory block from size of object
		@param[in] size Object size
		@return size of memory block including header
		*/
		static inline __CELL_SWCACHE_ATTRIBUTE_ALWAYS_INLINE size_t getBlockSize(size_t size)
		{
			return ceil16(size+15) + sizeof(union MemBlockHeader);
		}

		/*! @brief get pointer to object, param ea is used for alignment calculation
		@param[in] ea main memory address of target object
		@return Pointer to object in memory block
		*/
		__CELL_SWCACHE_ALWAYS_INLINE void *getPtr(uint32_t ea = 0) {
			return (magicWord == sMemBlockMagicWord) ? (void *)((uintptr_t)(this + 1) + (ea & 15)) : 0;
		}

		/*! @brief get pointer to memory block header from pointer to object
		@param[in] ptr           Pointer to object
		@return Pointer to memory block header
		*/
		static inline __CELL_SWCACHE_ATTRIBUTE_ALWAYS_INLINE union MemBlockHeader *getHeader(void *ptr, unsigned int noCheck = 0) {
			union MemBlockHeader *pHeader = ((union MemBlockHeader *)floor16(ptr))-1;
			return (noCheck || (pHeader->magicWord == sMemBlockMagicWord)) ? pHeader : 0;
		}

		/*! @brief initialize memory block header
		@param[in] _pDirEnt Pointer to DirEntry
		@param[in] _size    Size of object
		*/
		__CELL_SWCACHE_ALWAYS_INLINE void initialize(DirEntry *_pDirEnt, size_t _size)
		{
			Directory *pDir = getDirectory();
			magicWord = sMemBlockMagicWord;
			pDirEnt   = _pDirEnt;
			pHtab16   = pDir->getHtab16Of(_pDirEnt);
			slot      = pDir->getSlotOf(_pDirEnt);
			size      = _size;

			__CELL_SWCACHE_DPRINTF_3("%s:%s: header@%p(pDirEnt=%p,pHtab16=%p,slot=%d,size=%#x)", "cell::swcache::DefaultCache::MemBlockHeader:",
				__func__, this, pDirEnt, pHtab16, slot, size);
		}

		/*! @brief begin write back DMA
		@param[in] dmaTag DMA Tag ID used for writeback
		*/
		__CELL_SWCACHE_ALWAYS_INLINE void writeBack(unsigned int dmaTag)
		{
			__CELL_SWCACHE_ASSERT(magicWord == sMemBlockMagicWord, "Memblock@%p is not cached object", this);
			register uint32_t lsa  = (uintptr_t)getPtr(pDirEnt->ea);
			register uint32_t ea   = pDirEnt->ea;
			cellDmaUnalignedPut(lsa, ea, size, dmaTag, 0, 0);
#ifdef _DEBUG
			CELL_SWCACHE_DMA_WAIT_TAG_STATUS_ALL(1<<dmaTag);
#endif
			__CELL_SWCACHE_DPRINTF_3("%s:%s: lsa=%#x, ea=%#x, size=%#x", "cell::swcache::DefaultCache::MemBlockHeader:", __func__, lsa, ea, size);
		}

		/*! @brief release object from cache,writeback DMA is initiated if needed.
		@param[in] dmaTag DMA Tag ID used for writeback
		@retval CELL_OK          No writeback is kicked
		@retval WRITEBACK_KICKED Writeback DMA is kicked
		*/
		__CELL_SWCACHE_ALWAYS_INLINE int releaseObject(unsigned int dmaTag)
		{
			__CELL_SWCACHE_ASSERT(magicWord == sMemBlockMagicWord, "Memblock@%p is not cached object", this);
			__CELL_SWCACHE_DPRINTF_3_IN("%s:%s: ea:%#x, ptr:%#x,size:%#x, slot:%d", "cell::swcache::DefaultCache::MemBlockHeader:", __func__,
				pDirEnt->ea, pDirEnt->getPtr(), size, slot);
			*pHtab16 = spu_insert(0, *pHtab16, slot); // hash clear

			int ret = CELL_OK;
			pDirEnt->waitForDMA();
			if (pDirEnt->u32 & IS_WRITABLE)
			{
				ret = WRITEBACK_KICKED;
				if (pDirEnt->hasVtable)
				{
					// revert VTABLE pointer
					void **ptr = (void **)getPtr(pDirEnt->ea);
					__CELL_SWCACHE_DPRINTF_3("Revert VTABLE pointer %p -> %p@%p", *ptr, ClassIdTabHeader::getVtabHeader(ptr)->mpOrgVtab, ptr);
					*ptr = ClassIdTabHeader::getVtabHeader(ptr)->mpOrgVtab;
				}
				writeBack(dmaTag);
			}

			// clear header
			pDirEnt = 0;
			pHtab16 = 0;
			magicWord = 0;

			__CELL_SWCACHE_DPRINTF_3_OUT(" %s", (ret == WRITEBACK_KICKED) ? "WRITEBACK_KICKED" : "DONE");

			return ret;
		}

		/*! @brief decrement refcount of cached object, if refcount reaches zero, releaseObject is called
		@param[in] dmaTag DMA Tag ID used for writeback
		*/
		__CELL_SWCACHE_ALWAYS_INLINE void release()
		{
			__CELL_SWCACHE_ASSERT(magicWord == sMemBlockMagicWord, "Memblock@%p is not cached object", this);
			__CELL_SWCACHE_ASSERT(pDirEnt->refCount > 0,
				"lsa = %#x is already freed memory\n"
				"\tDirEntry = %p\n"
				"\trefcount = %d, ea = %#x, memBlock = %p"
				, pDirEnt->lsa<<4
				, pDirEnt
				, pDirEnt->refCount, pDirEnt->ea, this);
			pDirEnt->u32 -= 1 << REFCOUNT_SA; // refCount--
		}

		// member variables
		__extension__ struct {
			DirEntry    *pDirEnt;        //!< Pointer to Directory entry
			vec_uchar16 *pHtab16;        //!< Pointer to htab array16
			uint16_t     aux;
			uint16_t     __reserved__:12;
			uint16_t     slot      :  4; //!< Slot number inside htab array16(vec_uchar16)
			uint32_t     magicWord : 14; //!< magic word
			uint32_t     size      : 18; //!< Size of cache object
		};
		vec_uint4 vec;
	} MemBlockHeader;

private:
	/*! @brief Initialize function to hand memory to cache
	@param[in,out] pMemory                  Pointer to memory given to cache Directory by user
	*/
	static __attribute__((noinline)) int initialize(void *pMemory)
	{
		uintptr_t ptr = (uintptr_t)pMemory;

		setDirectory((void *)ptr);
		__CELL_SWCACHE_DPRINTF("%s:Directory initialized(%p-%p)",	"cell::swcache::DefaultCache:", ptr, ptr + sizeof(Directory));
		ptr += sizeof(Directory);
#if CELL_SWCACHE_SAFE_LV >= 1
		setRefDirectory((void *)ptr);
		__CELL_SWCACHE_DPRINTF("%s:RefDirectory initialized(%p-%p)", "cell::swcache::DefaultCache:", ptr, ptr + sizeof(RefDirectory));
#endif
		return CELL_OK;
	}

	//! @brief Cache Directory Entry
	typedef struct DirEntry {
		static const unsigned int MAX_REF_COUNT = (1<<7)-1; //!< Maximum reference count

		/*! @brief get pointer to cached object
		@return pointer to cached object managed by this DirEntry
		*/
		void *getPtr()
		{
			return (void *)(uintptr_t)(spu_extract(spu_sel(spu_promote(u32, 0), spu_promote((uintptr_t)ea,0), spu_promote(0xfu,0)), 0) & 0x3ffff);
		}

		/*! @brief get pointer to memory block header
		@return pointer to memory block header for cached object managed by this DirEntry
		*/
		MemBlockHeader *getHeader()
		{
			return ((union MemBlockHeader *)(uintptr_t)(u32 & 0x3fff0))-1;
		}

		/*! @brief initialize DirEntry
		@param[in] ptr            Pointer to cached object
		@param[in] _dmaTag        DMA Tag ID allocated for this cache object
		@param[in] _needWriteBack 0:this cache object is read-only, other value:this cache object is writable
		@param[in] _hasVtable     0:this cache object doesn't have pointer to VTABLE, other value: this cache object has pointer to VTABLE
		*/
		void initialize(void *ptr, unsigned int _dmaTag, unsigned int _needWriteBack, unsigned int _hasVtable)
		{
			lsa           = (uintptr_t)ptr >> 4;
			refCount      = 1;
			isLoading     = 1;
			dmaTag        = _dmaTag;
			needWriteBack = _needWriteBack ? 1 : 0;
			hasVtable     = _hasVtable;
		}

		/*! @brief Increment refcount
		@param[in]                 _needWriteBack 0:this cache object is read-only, IS_WRITABLE:this cache object is writable
		@return                    Pointer to grabbed cache object
		                           - IS_LOADING is logical ored if fetch DMA is on going
		*/
		void *addGrab(unsigned int _needWriteBack)
		{
			// update directory
			__CELL_SWCACHE_ASSERT(refCount < MAX_REF_COUNT, "Reference count for ea:%#x exceeds its limit refCount=%d", ea, refCount);
			register uint32_t u32_ = u32;
			register void *ptr = (void *)(uintptr_t)(spu_extract(spu_sel(spu_promote(u32_, 0), spu_promote((uintptr_t)ea,0), spu_promote(0xfu,0)), 0) & (IS_LOADING | 0x3ffff));
			u32 = (u32_ + (1 << REFCOUNT_SA)) | // refCount++
				_needWriteBack; // needWriteBack
			return  ptr;
		}

		/*! @brief wait for completion of fetch DMA
		@retval DMA_COMPLETED fetch DMA has just completed
		@retval CELL_OK       fetch DMA had been already completed
		*/
		int waitForDMA()
		{
			if (__builtin_expect(u32 & IS_LOADING, 0))
			{
				CELL_SWCACHE_DMA_WAIT_TAG_STATUS_ALL(1 << dmaTag);
				u32 &= ~IS_LOADING;
				return DMA_COMPLETED;
			}

			return CELL_OK;
		}

		/*! @brief check for completion of fetch DMA
		@retval DMA_COMPLETED fetch DMA has just completed
		@retval DMA_RUNNING   fetch DMA is still running
		@retval CELL_OK       fetch DMA had been already completed
		*/
		int checkForDMA()
		{
			if (__builtin_expect(u32 & IS_LOADING, 0))
			{
				isLoading = (cellDmaWaitTagStatusImmediate(1u << dmaTag) != 0);
				return isLoading ? DMA_RUNNING : DMA_COMPLETED;
			}
			return CELL_OK;
		}

		// member variable
		__extension__ union {
			__extension__ struct {
				uint32_t dmaTag       :5;  //!< DMA tag ID for loading
				uint32_t refCount     :7;  //!< Reference count
				uint32_t needWriteBack:1;  //!< Memory block is updated by SPU
				uint32_t isLoading    :1;  //!< true if DMA is running
				uint32_t lsa          :14; //!< LSA in qword
				uint32_t __reserved__ :3;
				uint32_t hasVtable    :1;  //!< object has pointer to VTABLE
			};
			uint32_t u32;
		};
		__extension__ union {
		uint32_t ea;               //!< EA
		uint32_t key;              //!< key
		};
	} __attribute__((aligned(8))) DirEntry;

	/*! @brief get pointer to global Directory
	@return pointer to global Direcotry
	*/
	static inline __CELL_SWCACHE_ATTRIBUTE_ALWAYS_INLINE Directory *getDirectory() { return __setDirectory(); }

	/*! @brief set pointer to global Directory
	@param[in] pMemory memory area used for global Directory,it has to be size of sizeof(Directory)
	*/
	static inline __CELL_SWCACHE_ATTRIBUTE_ALWAYS_INLINE void setDirectory(void *pMemory) { (void)__setDirectory(pMemory); }

	//! wrapper for static data
	static inline __CELL_SWCACHE_ATTRIBUTE_ALWAYS_INLINE Directory *__setDirectory(void *pMemory = 0)
	{
		static Directory *spDirectory; //!< pointer to global Directory

		if (__builtin_expect((uintptr_t)pMemory, 0))
		{
			spDirectory = (Directory *)pMemory;
			__builtin_memset(spDirectory, 0, sizeof(Directory));
		}
		__CELL_SWCACHE_ASSERT(spDirectory, "cell::swcache::DefaultCache directory is not initialized");

		return spDirectory;
	}

	/*! @brief internal cache query function
	@param[in,out] ptr       Pointer to be queried, and query result is overwritten to this variable
	@param[in] size          Size of cached object
	@param[in] needWriteBack 0:object is read-only,IS_WRITABLE: object is writable
	@param[in,out] dmaTag    DMA Tag ID used for fetch DMA,if another DMA Tag ID is used, this variable is overwritten with that value
	@param[in] isGrab        Set when grab
	@param[out] ea           Main memory address of cached object
	@param[in] hasVtable     0:object doesn't have pointer to VTABLE, other value: object has pointer to VTABLE
	@param[in] noCacheCheck  0:cache is checked,other value:cache is not checked
	@param[in] pFetched      Pointer to object, this is used when data fetch is already completed before
	@return                  Query status
	*/
	static int checkAndLoad(void *&ptr, uint32_t size, unsigned int needWriteBack, unsigned int &dmaTag, unsigned int isGrab, uint32_t &ea_, unsigned int hasVtable = 0, unsigned int noCacheCheck = 0, void *pFetched = 0)
	{
		register Directory *pDirectory = getDirectory();
		register DirEntry *pDirEnt = 0;
		register MemBlockHeader *pHeader;
		ea_ = (uintptr_t)ptr;
		if ((uintptr_t)ptr >= 0x40000) {
			// if address is EA
			register uint32_t ea = (uintptr_t)ptr;
			if (__builtin_expect(!noCacheCheck && (pDirEnt = pDirectory->get(ea)), 0))
			{
				// Cache hit!

				__CELL_SWCACHE_DPRINTF_2("  Addr is EA : Cache hit    ea:%#x -> ptr:%p,size:%#x, refCount = %d%s",
					ea, pDirEnt->getPtr(), size, pDirEnt->refCount, pDirEnt->hasVtable?" HAS_VTABLE":"");
				if (__builtin_expect(isGrab, 1))
				{
					// update directory
					pDirEnt->addGrab(needWriteBack);
				}

				pHeader = pDirEnt->getHeader();
				__CELL_SWCACHE_ASSERT(pHeader->pDirEnt == pDirEnt,
					"Cache hit for ea:%#x, but inconsistent MemBlockHeader\n"
					"\theader@%p(pDirEnt:%p, pHtab16:%p,slot:%d,size:%#x)\n"
					"\thit directory@%p, dir->refcount = %d, dir->ea = %#x, dir->lsa = %p",
					ea,
					pHeader, pHeader->pDirEnt, pHeader->pHtab16, pHeader->slot, size,
					pDirEnt, pDirEnt->refCount, pDirEnt->ea, pDirEnt->getPtr());

				// return values
				dmaTag = pDirEnt->dmaTag;
				ptr    = pDirEnt->getPtr();

				return CACHED | (pDirEnt->u32 & (IS_LOADING | IS_WRITABLE));

			}

			// Cache miss
			__CELL_SWCACHE_DPRINTF_2("  Addr is EA : Cache miss!! ea:%#x,size:%#x%s", ea, size, hasVtable?" HAS_VTABLE":"");
			int ret = NOT_CACHED;
			if (__builtin_expect(isGrab, 1))
			{
				do {
					ret = NOT_CACHED | needWriteBack;
					// grab directory entry for ea
					do {
						pDirEnt = pDirectory->grab(ea);
						if (__builtin_expect(!pDirEnt, 0))
						{
							register DirEntry *pSiblings = pDirectory->getSiblingsOf(ea);
							register int i;
							for(i = 0; i < 16 && pSiblings[i].refCount; i++){}
							if (__builtin_expect(i == 16, 0)) return CELL_SWCACHE_ERROR_NO_HTAB;

							// victim entry found
							register MemBlockHeader *pVictimHeader = pSiblings[i].getHeader();
							__CELL_SWCACHE_ASSERT(pVictimHeader->magicWord == sMemBlockMagicWord, "Invalid memory block at %p", pVictimHeader);
							ret |= pVictimHeader->releaseObject(dmaTag);
							if (tHeap::isAllocated((void *)pVictimHeader))
							{
								if (ret & WRITEBACK_KICKED)
								{
									ret &= ~WRITEBACK_KICKED;
									// need to wait although dma put buffer is protected against succeeding data-in by fence becasue
									// succeeding heap reallocation may write management info on DMA buffer
									CELL_SWCACHE_DMA_WAIT_TAG_STATUS_ALL(1 << dmaTag);
								}
								tHeap::free((void *)pVictimHeader);
							}
						}
					} while(__builtin_expect(!pDirEnt, 0));

					// grab heap memory
					register size_t blockSize = MemBlockHeader::getBlockSize(size);
					pHeader = pFetched ? MemBlockHeader::getHeader(pFetched, 1) : (MemBlockHeader *)tHeap::malloc(blockSize);
					if (__builtin_expect(!pHeader, 0))
					{
						// malloc failed -> search big enough space for realloc
						while((uintptr_t)(pHeader = tHeap::getPrevBlockOf(pHeader)))
						{
							__CELL_SWCACHE_DPRINTF_2("  malloc failed!! -> search for realloc candidate size=%#x", blockSize);
							if ((pHeader->pDirEnt->refCount == 0 || !tHeap::isAllocated((void *)pHeader)) &&
								MemBlockHeader::getBlockSize(pHeader->size) >= blockSize)
							{
								// object for realloc found
								if (pHeader->magicWord == sMemBlockMagicWord)
								{
									ret |= pHeader->releaseObject(dmaTag);
									if (ret & WRITEBACK_KICKED)
									{
										ret &= ~WRITEBACK_KICKED;
										// need to wait although dma put buffer is protected against succeeding data-in by fence becasue
										// succeeding heap reallocation may write management info on DMA buffer
										CELL_SWCACHE_DMA_WAIT_TAG_STATUS_ALL(1 << dmaTag);
									}
								}
								tHeap::realloc((void *)pHeader, blockSize);
								__CELL_SWCACHE_DPRINTF_2(" realloc(ptr=%p, size=%#x)", pHeader, blockSize);
								break;
							}
						}
						if (!pHeader)
						{
							pDirectory->release(pDirEnt);
							return CELL_SWCACHE_ERROR_NOMEM;
						}
					}
					pHeader->initialize(pDirEnt, size);
					if (__builtin_expect(!pFetched, 1))
					{
						// fetch
						register void  *lsa   = pHeader->getPtr();
						register size_t tsize = ceil16(size + (ea&15));
						if (__builtin_expect(tsize <= 16 * 1024, 1))
							cellDmaGetf(lsa, (uintptr_t)floor16((void *)(uintptr_t)ea), tsize, dmaTag, 0, 0);
						else
							cellDmaLargeGetf(lsa, (uintptr_t)floor16((void *)(uintptr_t)ea), tsize, dmaTag, 0, 0);
#ifdef _DEBUG
						CELL_SWCACHE_DMA_WAIT_TAG_STATUS_ALL(1 << dmaTag);
#endif
						__CELL_SWCACHE_DPRINTF_2("fetch lsa=%p <- ea=%p, size=%#x, tag=%d",	lsa, floor16((void *)(uintptr_t)ea), tsize, dmaTag);
						ret |= IS_LOADING;
					}
					ptr = pHeader->getPtr(ea);

					//			__CELL_SWCACHE_ASSERT(sizeof(*pDirEnt) == 8, "sizeof(DirEntry) != 8");
					//! update directory
					pDirEnt->initialize(ptr, dmaTag, needWriteBack, hasVtable);

					//! patch VTABLE
					if (hasVtable)
					{
						CELL_SWCACHE_DMA_WAIT_TAG_STATUS_ALL(1 << dmaTag); ret &= ~IS_LOADING;
						// patch VTABLE pointer
						__CELL_SWCACHE_DPRINTF_2("Patch pVTABLE for class@%p by class id tab@%p(pVTABLE on main memory=%p)",
							ptr, ClassIdTabHeader::getClassIdTabOnSPU(), *(Vtab **)ptr);
						ClassIdTabHeader *pClassIdTab = ClassIdTabHeader::getClassIdTabOnSPU();
						if (__builtin_expect(!pClassIdTab, 0)) return CELL_SWCACHE_ERROR_VTAB;
						if (pClassIdTab->patchVtab((Vtab **)ptr))
						{
							__CELL_SWCACHE_DPRINTF_2(" Failed to patch pVTABLE!!");
							pDirectory->release(pDirEnt);
							tHeap::free(pHeader);
							return CELL_SWCACHE_ERROR_VTAB;
						} else {
							__CELL_SWCACHE_DPRINTF_2(" Patched pVTABLE=%p", *(Vtab **)ptr);
							unsigned actual_size = ClassIdTabHeader::getVtabHeader(ptr)->mSizeOfClass;
							if (ceil16((ea&15) + size) < (ea&15) + actual_size)
							{
								// need enlarge
								__CELL_SWCACHE_DPRINTF_3("Class size enlarged %#x -> %#x", size, actual_size);
								pDirectory->release(pDirEnt);
								if (pFetched)
								{
									pFetched = 0;
								} else {
									tHeap::free(pHeader);
								}
								size = actual_size;

								ret = CELL_SWCACHE_ERROR_AGAIN;
							}
						}
					}
				} while(__builtin_expect(ret == CELL_SWCACHE_ERROR_AGAIN, 0));
			}

			return ret;
		} else if (ptr) {
			// if address is LSA
			if (__builtin_expect(!noCacheCheck && isCachedPointer(ptr), 0))
			{
				pHeader = MemBlockHeader::getHeader(ptr);
				pDirEnt = pHeader->pDirEnt;
				__CELL_SWCACHE_DPRINTF_2("  Addr is LSA: Cache hit    ea:%#x ptr:%#x,size:%#x refCount=%d%s",
					pDirEnt->ea, ptr, pHeader->size, pDirEnt->refCount, pDirEnt->hasVtable?" HAS_VTABLE":"");
				if (__builtin_expect(isGrab, 1))
				{
					pDirEnt->addGrab(needWriteBack);
				}

				// return value
				dmaTag = pDirEnt->dmaTag;
				ea_    = pDirEnt->ea;

				return CACHED | (pDirEnt->u32 & (IS_LOADING | IS_WRITABLE));
			} else {
				__CELL_SWCACHE_DPRINTF_2("  Addr is LSA: Non cached object ptr:%p,size=%#x", ptr, size);

				return CELL_SWCACHE_ERROR_NON_CACHED_POINTER;
			}
		} else return CELL_SWCACHE_ERROR_NULL_POINTER;
	}

#if CELL_SWCACHE_SAFE_LV > 0
	/*! @brief get pointer to global RefDirectory
	@return pointer to global RefDirecotry
	*/
	static inline __CELL_SWCACHE_ATTRIBUTE_ALWAYS_INLINE RefDirectory *getRefDirectory() { return __setRefDirectory(); }

	/*! @brief set pointer to global RefDirectory
	@param[in] pMemory memory area used for global RefDirectory,it has to be size of sizeof(RefDirectory)
	*/
	static inline __CELL_SWCACHE_ATTRIBUTE_ALWAYS_INLINE void setRefDirectory(void *pMemory) { (void)__setRefDirectory(pMemory); }

	//! wrapper for static data
	static inline __CELL_SWCACHE_ATTRIBUTE_ALWAYS_INLINE RefDirectory *__setRefDirectory(void *pMemory = 0)
	{
		static RefDirectory *spRefDirectory = 0; //!< pointer to global RefDirectory

		if (__builtin_expect((uintptr_t)pMemory, 0))
		{
			spRefDirectory = (RefDirectory *)pMemory;
			__builtin_memset(spRefDirectory, 0, sizeof(RefDirectory));
		}
		__CELL_SWCACHE_ASSERT(spRefDirectory, "DefaultCache reference directory is not initialized");

		return spRefDirectory;
	}

	//! @brief Reference directory entry
	typedef struct RefDirEntry
	{
		uint16_t key;     //!< key value
	} RefDirEntry;
#endif
};

//! @brief Simple Heap class
class DefaultHeap
{
	friend class DefaultCache<DefaultHeap>;

	enum Opcode {
		INIT,    //!< initialize
		FIN,     //!< finalize
		MALLOC,  //!< allocate memory block from heap
		REALLOC, //!< reallocate block
		GETTOP   //!< returns heap top pointer
	};

public:

	/*! @brief Initialize heap with memory handed from user
	@param pHeap[in]                        Pointer to memory
	@param sizeHeap[in]                     Size of memory
	@retval CELL_OK                         Success
	@retval CELL_SWCACHE_ERROR_NULL_POINTER pHeap is NULL
	@retval CELL_SWCACHE_ERROR_ALIGN        pHeap is not 16bytes aligned
	*/
	static __attribute__((noinline)) int initialize(void *pHeap, size_t sizeHeap)
	{
		if (__builtin_expect(!pHeap, 0)) return CELL_SWCACHE_ERROR_NULL_POINTER;
		else if (__builtin_expect((uintptr_t)pHeap % 16, 0)) return CELL_SWCACHE_ERROR_ALIGN;
		getBottomPtr(sizeHeap, pHeap);
		return CELL_OK;
	}

	/*! @brief Finalize heap
	*/
	static __attribute__((noinline)) void finalize()
	{
		getPtr(FIN);
	}

	/*! @brief Grab memory from heap
	@param[in] size Size of memory
	@return Pointer to grabbed memory. NULL is returned if heap has no memory
	*/
	static inline __CELL_SWCACHE_ATTRIBUTE_ALWAYS_INLINE void *malloc(size_t size)
	{
#ifdef CELL_SWCACHE_DEBUG
		__CELL_SWCACHE_ASSERT((size % 16) == 0 && size >= 16, "size=%u is not multiple of qword", size);
#endif
		void *ptr = getPtr(MALLOC, size);
		__CELL_SWCACHE_DPRINTF_4("%s: size=%#x => ptr=%p", __PRETTY_FUNCTION__, size, ptr);

		return ptr;
	}

	/*! @brief Reallocate memory
	@param[in] ptr  Pointer for reallocation
	@param[in] size Size of memory
	*/
	static inline __CELL_SWCACHE_ATTRIBUTE_ALWAYS_INLINE void realloc(void *ptr, size_t size)
	{
#ifdef CELL_SWCACHE_DEBUG
		__CELL_SWCACHE_ASSERT((size % 16) == 0 && size >= 16, "size=%u is not multiple of qword", size);
#endif
		(void)getPtr(REALLOC, size, ptr);
		__CELL_SWCACHE_DPRINTF_4("%s: ptr = %p, size=%#x", __PRETTY_FUNCTION__, ptr, size);
	}

	/*! @brief Release memory to heap
	@param[in] ptr Pointer to memory to be released
	*/
	static void free(void *ptr)
	{
		*((uint32_t *)ptr) = DELETE_MARK_MAGIC_WORD; // flag it as DELETED
		register size_t blockSize =
			DefaultCache<DefaultHeap>::MemBlockHeader::getBlockSize(((DefaultCache<DefaultHeap>::MemBlockHeader *)ptr)->size);
		__CELL_SWCACHE_DPRINTF_4("%s: ptr=%p(size=%#x)", __PRETTY_FUNCTION__, ptr, blockSize);
		register void *pHeap = getTopPtr();
		if (__builtin_expect((uintptr_t)ptr + blockSize == (uintptr_t)pHeap, 1))
		{
			// ptr is last allocated memory

			// rewind spHeap until active memory is found
			register uint32_t *pRewind = (uint32_t *)pHeap;
			register size_t prevSize = ((DefaultCache<DefaultHeap>::MemBlockHeader *)(uintptr_t)pRewind)->aux << 4; // aux is used to remember previous block size
			register size_t rewindSize = 0;
			do {
				if (prevSize == 0) break;
				rewindSize += prevSize;
				__CELL_SWCACHE_DPRINTF_4("%s:   rewind %p -> %p", __PRETTY_FUNCTION__, pRewind, (uintptr_t)pRewind - prevSize);
				pRewind = (uint32_t *)((uintptr_t)pRewind - prevSize);
				prevSize = ((DefaultCache<DefaultHeap>::MemBlockHeader *)(uintptr_t)pRewind)->aux << 4; // aux is used to remember previous block size
			} while(__builtin_expect(*((uint32_t *)((uintptr_t)pRewind - prevSize)) == DELETE_MARK_MAGIC_WORD, 0));
			__CELL_SWCACHE_ASSERT(rewindSize > 0, "rewindSize is zero, pRewind = %p", pRewind);
			getPtr(INIT, rewindSize, (void *)pRewind);
		} else {
			// ptr is not last allocate memory


			__CELL_SWCACHE_DPRINTF_4("%s: Cannot rewind due to fragmentation %p(size=%#x) Heap top=%p", __PRETTY_FUNCTION__, ptr, blockSize, pHeap);
		}
	}

	/*! @brief Return previous active memory block
	@param[in] pHeader Current pointer to memory block, top pointer is used when NULL is specified
	@return Pointer to previous active memory block of pHeader. NULL is returned if heap bottom is reached.
	*/
	static DefaultCache<DefaultHeap>::MemBlockHeader *getPrevBlockOf(DefaultCache<DefaultHeap>::MemBlockHeader *pHeader = 0)
	{
		if (!pHeader) pHeader = (DefaultCache<DefaultHeap>::MemBlockHeader *)getTopPtr();
		return (pHeader->aux) ? pHeader - pHeader->aux : 0;
	}

	/*! @brief Return if memory pointed by ptr is already allocated area by heap
	@param[in] ptr  Pointer
	@return         0 is returned if not allocated, otherwise non zero value is returned
	*/
	static inline __CELL_SWCACHE_ATTRIBUTE_ALWAYS_INLINE unsigned int isAllocated(void *ptr) { return *((uint32_t *)ptr) != DELETE_MARK_MAGIC_WORD; }

private:
	//! magic word to be put on the head of already deleted memory block
	static const uint32_t DELETE_MARK_MAGIC_WORD = 0xdeadb10c;

	static inline __CELL_SWCACHE_ATTRIBUTE_ALWAYS_INLINE void *getTopPtr()
	{
		return getPtr(GETTOP);
	}

	//! wrapper for static data
	static void *getPtr(enum Opcode opcode, size_t size = 0, void *pMemory = 0)
	{
		static void  *spHeapTop = 0; //!< current heap pointer
		static size_t sSize = 0; //!< current heap size

		switch(opcode)
		{
			case INIT:
				spHeapTop = pMemory;
				sSize += size;
				return spHeapTop;
			case FIN:
				sSize = 0;
				return 0;
			case MALLOC:
			{
				// make sure that the heap has been initialized.
				// If this assert is hit, then you need to call
				//   	cell::swcache::DefaultCache<cell::swcache::DefaultHeap>::initialize(sHeap, scHeapSize);
				// to initialize the heap for the cache
				__CELL_SWCACHE_ASSERT(spHeapTop, "DefaultHeap is not initialized!!");
				__CELL_SWCACHE_ASSERT((spHeapTop == getBottomPtr()) || (((DefaultCache<DefaultHeap>::MemBlockHeader *)spHeapTop)->aux),
					"DefaultHeap has corrupted!! heap active area(%p to %p)", getBottomPtr(), spHeapTop);
				register void *grab = 0;
				if (__builtin_expect(size + sizeof(DefaultCache<DefaultHeap>::MemBlockHeader) <= sSize, 1))
				{
					__CELL_SWCACHE_ASSERT(size >= 16, "Illegal grab size = %#x", size);
					__CELL_SWCACHE_ASSERT((size % 16) == 0, "Illegal grab size = %#x", size);
					__CELL_SWCACHE_ASSERT(((uintptr_t)spHeapTop % 16) == 0, "Heap pointer no aligned spHeapTop = %p", spHeapTop);
					grab = spHeapTop;
					*((uint32_t *)grab) = 0;
					sSize -= size;
					spHeapTop = (void *)((uintptr_t)spHeapTop + size);
					((DefaultCache<DefaultHeap>::MemBlockHeader *)spHeapTop)->aux =  size >> 4; // aux is used to remember previous block size
				}
				return grab;
			}
			case REALLOC:
			{
				// make sure that the heap has been initialized.
				// If this assert is hit, then you need to call
				//   	cell::swcache::DefaultCache<cell::swcache::DefaultHeap>::initialize(sHeap, scHeapSize);
				// to initialize the heap for the cache
				__CELL_SWCACHE_ASSERT(spHeapTop, "DefaultHeap is not initialized!!");
				__CELL_SWCACHE_ASSERT((spHeapTop == getBottomPtr()) || (((DefaultCache<DefaultHeap>::MemBlockHeader *)spHeapTop)->aux),
					"DefaultHeap has corrupted!! heap active area(%p to %p)", getBottomPtr(), spHeapTop);
				__CELL_SWCACHE_ASSERT(size >= 16, "Illegal grab size = %#x", size);
				__CELL_SWCACHE_ASSERT((size % 16) == 0, "Illegal grab size = %#x", size);
				register DefaultCache<DefaultHeap>::MemBlockHeader *pHeader = (DefaultCache<DefaultHeap>::MemBlockHeader *)pMemory;
				register size_t memblock_size = DefaultCache<DefaultHeap>::MemBlockHeader::getBlockSize(pHeader->size);
				__CELL_SWCACHE_ASSERT(memblock_size >= size, "Realloc is called with smaller memblock memblock_size=%#x, size=%#x", memblock_size, size);
				if (__builtin_expect(size == memblock_size, 1))
				*((uint32_t *)pHeader) = 0x0; // clear delete mark
				if (__builtin_expect(size != memblock_size, 1))
				{
					// split pHeader into two blocks
					register DefaultCache<DefaultHeap>::MemBlockHeader *pHeader2 = pHeader + (size >> 4);
					pHeader2->aux = size >> 4;
					register DefaultCache<DefaultHeap>::MemBlockHeader *pNextHeader = pHeader + (memblock_size >> 4);
					if ((void *)pNextHeader < getTopPtr())
					{
						pNextHeader->aux = (memblock_size - size) >> 4;
						*((uint32_t *)pHeader2) = DELETE_MARK_MAGIC_WORD;
					} else {
						spHeapTop = pHeader2;
					}
				}
				return pHeader;
			}
			case GETTOP:
				return spHeapTop;
			default:
				__CELL_SWCACHE_ASSERT(0, "Unknow opcode = %#x", opcode);
		}

		return (void *)0; // never reach here
	}

	static inline __CELL_SWCACHE_ATTRIBUTE_ALWAYS_INLINE void *getBottomPtr(size_t sizeHeap = 0, void *pHeap = 0)
	{
		static void *spHeapBottom = 0;
		if (__builtin_expect(pHeap != 0, 0))
		{
			spHeapBottom = pHeap;
			*((vec_uint4 *)pHeap) = (vec_uint4){0,0,0,0}; // clear
			getPtr(INIT, sizeHeap, pHeap);
		}
		return spHeapBottom;
	}

};

__CELL_SWCACHE_END

#endif /* __CELL_SWCACHE_DEFAULT_CACHE_H__ */
