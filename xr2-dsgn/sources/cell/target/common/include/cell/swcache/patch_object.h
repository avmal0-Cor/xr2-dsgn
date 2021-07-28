/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2009 Sony Computer Entertainment Inc.
 * All Rights Reserved.
 */

#ifndef __CELL_SWCACHE_PATCH_OBJECT_H__
#define __CELL_SWCACHE_PATCH_OBJECT_H__

#include <cell/swcache/define.h>

#ifdef __SPU__

/* standard C++ header */
#include <stdint.h>

/* Lv-2 OS headers */
#include <sys/types.h>

#include <cell/dma.h>

#include <cell/swcache/default_cache.h>
#include <cell/swcache/DMA_tag.h>

#define __CELL_SWCACHE_PATCH_DPRINTF_1(format,...)\
	__CELL_SWCACHE_DPRINTF_1("%s" format, "cell::swcache::PatchObject::", ##__VA_ARGS__)
#define __CELL_SWCACHE_PATCH_DPRINTF_1_IN(format,...)\
	__CELL_SWCACHE_DPRINTF_1_IN("%s" format, "cell::swcache::PatchObject::", ##__VA_ARGS__)

__CELL_SWCACHE_BEGIN

template<class tCache> class PatchObjectBase: protected CacheResource<tCache>
{
private:
	void grabSPUMem(void *&ptr, size_t size, PatchMode mode, ClassAttribute attr)
	{
		(void)size;
		register unsigned int dmaTag = mDmaTag;
		if (dmaTag == 0x3f) dmaTag = (unsigned int)this->reserveChannel();
		uint32_t ea;
		unsigned int actual_dmaTag = dmaTag;
#ifndef CELL_SWCACHE_SPU_NO_ALWAYS_INLINE
		register int status = tCache::grab(ptr, mSize, (mode == RW_MODE)?IS_WRITABLE:0, actual_dmaTag, ea, attr == HAS_VTABLE, 1);
#else
		register int status = tCache::grab(ptr, mSize, (mode == RW_MODE)?IS_WRITABLE:0, actual_dmaTag, ea, attr == HAS_VTABLE, 0);
#endif
		__CELL_SWCACHE_ASSERT(status >= 0 ||
			status == CELL_SWCACHE_ERROR_NULL_POINTER ||
			status == CELL_SWCACHE_ERROR_NON_CACHED_POINTER,
			"cache grab failed ret = %#x cell::swcache::PatchObject::grabSPUMem(): ptr=%p,size=%#x, dmaTag=%d%s",
			status, ptr, size, actual_dmaTag, (mode == RW_MODE)?" IS_WRITABLE":"");
		if (status >= 0)
		{
			if (__builtin_expect(status & IS_LOADING, 0))
			{
				this->setFetchDmaTag(actual_dmaTag);
				this->yield();
			}
		}
		if (__builtin_expect(actual_dmaTag != dmaTag || status < 0 || !(status & IS_LOADING), 0))
		{
			this->releaseChannel(dmaTag); dmaTag = 0x3f;
		}
		mOriginalPtr = (void *)(uintptr_t)ea;
		mDmaTag      = dmaTag;
		mPatchedPtr  = ptr;
	}

protected:
	__CELL_SWCACHE_ALWAYS_INLINE void	grabSPUMemInline(void *&ptr, size_t size, PatchMode mode, ClassAttribute attr)
	{
#ifndef CELL_SWCACHE_SPU_NO_ALWAYS_INLINE
		uint32_t ea = 0;
		register void *spu_ptr = (void *)((uintptr_t)tCache::grabIfCached(ptr, (mode == RW_MODE)?IS_WRITABLE:0, &ea) & 0x3ffff);
		if (__builtin_expect((uintptr_t)spu_ptr, 1))
		{
			// Cache hit!
			mPatchedPtr = spu_ptr;
			mOriginalPtr = (void *)(uintptr_t)ea;
			ptr = spu_ptr;
		} else
			// Cache miss
#endif
			this->grabSPUMem(ptr, size, mode, attr);
	}

	__CELL_SWCACHE_ALWAYS_INLINE PatchObjectBase(void *&ptr, size_t size, PatchMode mode, ClassAttribute attr, unsigned int n, unsigned int isAutoRelease)
		:mIsAutoRelease (isAutoRelease )
		,mDmaTag        (0x3f          )
		,mSize          (size * n      )
		,mpPtr          (&ptr          )
	{
		grabSPUMemInline(ptr, size * n, mode, attr);
	}

	// member variables
	__extension__ union {
		__extension__ struct {
			uint32_t       mIsAutoRelease :1;  //!< 1:release object on destructor,0:No release object on destructor
			uint32_t       mDmaTag        :6;  //!< DMA Tag ID used for transferring object from/to main memory
			uint32_t       __reserved__   :7;
			uint32_t       mSize          :18; //!< Size of object
		};
		uint32_t mU32;
	};
	void        *mOriginalPtr;       //!< Pointer value before patch
	void        *mPatchedPtr;        //!< Pointer value after patch
	void       **mpPtr;              //!< Pointer to object pointer
} __attribute__((aligned(16)));

/*! @brief Patch pointer of main memory address with cached LS address
           temporary area in LS is allocated from cache
@param tType  Type of pointer
@param tMode  RO_MODE: Pointer is read-only, RW_MODE: pointer is writable
@param tAttr  NO_VTABLE: Pointer doesn't have pointer to VTABLE, HAS_VTABLE: pointer has pointer to VTABLE
@param tCache Cache implementation 
*/
template<class tType, PatchMode tMode = RO_MODE, ClassAttribute tAttr = NO_VTABLE, class tCache = DefaultCache<DefaultHeap> > class PatchObject: protected PatchObjectBase<tCache>
{
public:

	/*! @brief Get size of cache object
	@return          Size of cache object
	*/
	inline __attribute__((always_inline)) size_t getSize() { return this->mSize; }

	/*! @brief Constructor
	@param[in,out] ptr                            Poiner to be patched
	@param[in] n                                  Number of elements(1 for single object, and >1 for array)
	@param[in] isAutoRelease                      0:Object is released on destructor
                                                  1: Object is not released on destructor
	*/
	__CELL_SWCACHE_ALWAYS_INLINE PatchObject(tType *&ptr, unsigned int n = 1, unsigned int isAutoRelease = 1)
		:PatchObjectBase<tCache>((void *&)ptr, sizeof(tType), tMode, tAttr, n, isAutoRelease)
	{
		__CELL_SWCACHE_PATCH_DPRINTF_1_IN("PatchObject(tType *&ptr=%s:%p, unsigned int n=%u, unsigned int isAutoRelease=%u): this=%p",
			((uintptr_t)ptr < 0x40000) ? "lsa" : "ea", ptr, n, isAutoRelease, this);
		__CELL_SWCACHE_DPRINTF_1_OUT(" patched pointer=%p", this->mPatchedPtr);
	}

	/*! @brief Patch pointer again.This method is used when pointer is overwritten
	           with another main memory address after previous pointer patch.
	*/
	__CELL_SWCACHE_ALWAYS_INLINE void repatch()
	{
		__CELL_SWCACHE_PATCH_DPRINTF_1_IN("repatch(): this=%p, ptr=%p, org_ptr=%s:%p, new_ptr=%s:%p", this,
			this->mPatchedPtr, ((uintptr_t)this->mOriginalPtr<0x40000)?"lsa":"ea", this->mOriginalPtr, ((uintptr_t)*(this->mpPtr)<0x40000)?"lsa":"ea", *(this->mpPtr));
		release(/* noPointerRevert = */1);
		this->grabSPUMemInline(*(this->mpPtr), this->mSize, tMode, tAttr);
		__CELL_SWCACHE_DPRINTF_1_OUT(" patched pointer=%p", this->mPatchedPtr);
	}

	/*! @brief Writeback the object content on cache to main memory
               This method doesn't free object after write back.
	*/
	__CELL_SWCACHE_ALWAYS_INLINE void flush()
	{
		__CELL_SWCACHE_PATCH_DPRINTF_1("flush(): this=%p, ptr=%p, org_ptr=%s:%p", this,
			*(this->mpPtr), ((uintptr_t)this->mOriginalPtr<0x40000)?"lsa":"ea", this->mOriginalPtr);
		tCache::flush(*(this->mpPtr), this->getSystemDmaTag());
	}

	/*! @brief Release cache object
	@param[in] noPointerRevert                    0: Pointer value is reverted to original value after release
                                                  Otherwise: Pointer value is not reverted
	*/
	__CELL_SWCACHE_ALWAYS_INLINE void release(unsigned int noPointerRevert = 0)
	{
		if (__builtin_expect(this->mPatchedPtr != this->mOriginalPtr, 1))
		{
			tCache::release(this->mPatchedPtr, 1);
			if (!noPointerRevert) *(this->mpPtr) = this->mOriginalPtr;
		}
	}

	/*! @brief Wait for DMA completion */
	static inline __CELL_SWCACHE_ATTRIBUTE_ALWAYS_INLINE void wait()
	{
		register uint32_t dmaTagMask = CacheResource<tCache>::setFetchDmaTag(32);
		__CELL_SWCACHE_PATCH_DPRINTF_1("wait(): dmaTagMask = %#x", dmaTagMask);
		CELL_SWCACHE_DMA_WAIT_TAG_STATUS_ALL(dmaTagMask);
	}

	/*! @brief Destructor */
	__CELL_SWCACHE_ALWAYS_INLINE ~PatchObject()
	{
		__CELL_SWCACHE_PATCH_DPRINTF_1("~PatchObject(): this=%p, ptr=%p,org_ptr=%s:%p", this,
			*(this->mpPtr), ((uintptr_t)this->mOriginalPtr<0x40000)?"lsa":"ea", this->mOriginalPtr);
		if (__builtin_expect(this->mIsAutoRelease, 1)) release();
	}

};

__CELL_SWCACHE_END

#ifndef CELL_SWCACHE_DEBUG
#define __CELL_SWCACHE_PATCH_TRACE__(message,ptr)
#define __CELL_SWCACHE_PATCH_TRACE2__(message,type,ptr)
#else
#define __CELL_SWCACHE_PATCH_TRACE__(message,ptr) __CELL_SWCACHE_DPRINTF(message " %s(%s:%p,size=%#x)", ((uintptr_t)ptr<0x40000)?"lsa":"ea", ptr, sizeof(*ptr));
#define __CELL_SWCACHE_PATCH_TRACE2__(message,type,ptr) __CELL_SWCACHE_DPRINTF(message " %s:%s(%s:%p,size=%#x)", _STR(type),_STR(ptr),((uintptr_t)ptr<0x40000)?"lsa":"ea", ptr, sizeof(*ptr));
#endif

#ifdef CELL_SWCACHE_SIZE_CHECK
#define __CELL_SWCACHE_PATCH_CHECK(ptr)		__CELL_SWCACHE_ASSERT(!cell::swcache::DefaultCache<cell::swcache::DefaultHeap>::MemBlockHeader::getHeader((void*)ptr) | ptr##PatchObject.getSize() <= cell::swcache::DefaultCache<cell::swcache::DefaultHeap>::MemBlockHeader::getHeader(ptr)->size, \
	"%s(=%p) is patched with size=%u, but cache hit detected to the cache object with smaller size = %u", _STR(ptr), ptr, ptr##PatchObject.getSize(), cell::swcache::DefaultCache<cell::swcache::DefaultHeap>::MemBlockHeader::getHeader(ptr)->size)
#else
#define __CELL_SWCACHE_PATCH_CHECK(ptr)
#endif

#define CELL_SWCACHE_PATCH_CONST_OBJECT(           type, ptr) __CELL_SWCACHE_PATCH_TRACE2__("patch const object"                  , type,ptr) cell::swcache::PatchObject<type>                                                    ptr##PatchObject(ptr);  __CELL_SWCACHE_PATCH_CHECK(ptr)
#define CELL_SWCACHE_PATCH_CONST_OBJECT_AND_BLOCK( type, ptr) __CELL_SWCACHE_PATCH_TRACE2__("patch const object and block"        , type,ptr) cell::swcache::PatchObject<type>                                                    ptr##PatchObject(ptr);  __CELL_SWCACHE_PATCH_CHECK(ptr)       ;cell::swcache::PatchObject<type>::wait()
#define CELL_SWCACHE_PATCH_OBJECT(                 type, ptr) __CELL_SWCACHE_PATCH_TRACE2__("patch object"                        , type,ptr) cell::swcache::PatchObject<type, cell::swcache::RW_MODE>                            ptr##PatchObject(ptr);  __CELL_SWCACHE_PATCH_CHECK(ptr)
#define CELL_SWCACHE_PATCH_OBJECT_AND_BLOCK(       type, ptr) __CELL_SWCACHE_PATCH_TRACE2__("patch object and block"              , type,ptr) cell::swcache::PatchObject<type, cell::swcache::RW_MODE>                            ptr##PatchObject(ptr);  __CELL_SWCACHE_PATCH_CHECK(ptr)       ;cell::swcache::PatchObject<type>::wait()

#define CELL_SWCACHE_PATCH_CONST_VCLASS(           type, ptr) __CELL_SWCACHE_PATCH_TRACE2__("patch const class w/vtable", type,ptr) cell::swcache::PatchObject<type, cell::swcache::RO_MODE, cell::swcache::HAS_VTABLE> ptr##PatchObject(ptr);  __CELL_SWCACHE_PATCH_CHECK(ptr)
#define CELL_SWCACHE_PATCH_VCLASS(                 type, ptr) __CELL_SWCACHE_PATCH_TRACE2__("patch class w/vtable"      , type,ptr) cell::swcache::PatchObject<type, cell::swcache::RW_MODE, cell::swcache::HAS_VTABLE> ptr##PatchObject(ptr);  __CELL_SWCACHE_PATCH_CHECK(ptr)

#define CELL_SWCACHE_PATCH_CONST_ARRAY(            type, ptr, n) __CELL_SWCACHE_PATCH_TRACE2__("patch const array"                , type,ptr) cell::swcache::PatchObject<type>                                                    ptr##PatchObject(ptr, n);  __CELL_SWCACHE_PATCH_CHECK(ptr)
#define CELL_SWCACHE_PATCH_CONST_ARRAY_AND_BLOCK(  type, ptr, n) __CELL_SWCACHE_PATCH_TRACE2__("patch const array and block"      , type,ptr) cell::swcache::PatchObject<type>                                                    ptr##PatchObject(ptr, n);  __CELL_SWCACHE_PATCH_CHECK(ptr)    ;cell::swcache::PatchObject<type>::wait()
#define CELL_SWCACHE_PATCH_ARRAY(                  type, ptr, n) __CELL_SWCACHE_PATCH_TRACE2__("patch array"                      , type,ptr) cell::swcache::PatchObject<type, cell::swcache::RW_MODE>                            ptr##PatchObject(ptr, n);  __CELL_SWCACHE_PATCH_CHECK(ptr)
#define CELL_SWCACHE_PATCH_ARRAY_AND_BLOCK(        type, ptr, n) __CELL_SWCACHE_PATCH_TRACE2__("patch array and block"            , type,ptr) cell::swcache::PatchObject<type, cell::swcache::RW_MODE>                            ptr##PatchObject(ptr, n);  __CELL_SWCACHE_PATCH_CHECK(ptr)    ;cell::swcache::PatchObject<type>::wait()

#define CELL_SWCACHE_REPATCH(                      ptr) do {__CELL_SWCACHE_PATCH_TRACE__("repatch"          , ptr) ptr##PatchObject.repatch();  __CELL_SWCACHE_PATCH_CHECK(ptr);}while(0)
#define CELL_SWCACHE_REPATCH_AND_BLOCK(            ptr) do {__CELL_SWCACHE_PATCH_TRACE__("repatch and block", ptr) ptr##PatchObject.repatch();  __CELL_SWCACHE_PATCH_CHECK(ptr); ptr##PatchObject.wait();}while(0)

#define CELL_SWCACHE_FLUSH(                        ptr) do {__CELL_SWCACHE_PATCH_TRACE__("flush"            , ptr) ptr##PatchObject.flush();}while(0)
#define CELL_SWCACHE_RELEASE(                      ptr) do {__CELL_SWCACHE_PATCH_TRACE__("release"          , ptr) ptr##PatchObject.release();}while(0)
#define CELL_SWCACHE_WAIT                               cell::swcache::PatchObject<void>::wait()

#else /* #ifdef __SPU__ */

#define CELL_SWCACHE_PATCH_CONST_OBJECT(type, ptr) (void)ptr
#define CELL_SWCACHE_PATCH_CONST_OBJECT_AND_BLOCK(type, ptr) (void)ptr
#define CELL_SWCACHE_PATCH_OBJECT(type, ptr) (void)ptr
#define CELL_SWCACHE_PATCH_OBJECT_AND_BLOCK(type, ptr) (void)ptr

#define CELL_SWCACHE_PATCH_CONST_VCLASS(type, ptr) (void)ptr
#define CELL_SWCACHE_PATCH_VCLASS(type, ptr) (void)ptr

#define CELL_SWCACHE_PATCH_CONST_ARRAY(type, ptr, n) (void)ptr
#define CELL_SWCACHE_PATCH_CONST_ARRAY_AND_BLOCK(type, ptr, n) (void)ptr
#define CELL_SWCACHE_PATCH_ARRAY(type, ptr, n) (void)ptr
#define CELL_SWCACHE_PATCH_ARRAY_AND_BLOCK(type, ptr, n) (void)ptr

#define CELL_SWCACHE_REPATCH(ptr) (void)ptr
#define CELL_SWCACHE_REPATCH_AND_BLOCK(ptr) (void)ptr

#define CELL_SWCACHE_FLUSH(ptr) (void)ptr
#define CELL_SWCACHE_RELEASE(ptr) (void)ptr
#define CELL_SWCACHE_WAIT

#endif /* #ifdef __SPU__ */

#endif /* __CELL_SWCACHE_PATCH_OBJECT_H__ */
