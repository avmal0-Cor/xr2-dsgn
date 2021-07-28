/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2009 Sony Computer Entertainment Inc.
 * All Rights Reserved.
 */

#ifndef __CELL_SWCACHE_POINTER_H__
#define __CELL_SWCACHE_POINTER_H__

/* standard C++ headers */
#include <stdint.h>

/* Lv-2 OS headers */
#include <sys/types.h>

#ifdef __SPU__
#include <spu_mfcio.h>
#include <cell/swcache/default_cache.h>
#include <cell/swcache/DMA_tag.h>
#endif
#include <cell/swcache/types.h>

#define __CELL_SWCACHE_POINTER_DPRINTF(format,...)\
	__CELL_SWCACHE_DPRINTF("%s" format, "cell::swcache::Pointer::", ##__VA_ARGS__)
#define __CELL_SWCACHE_POINTER_DPRINTF_IN(format,...)\
	__CELL_SWCACHE_DPRINTF_IN("%s" format, "cell::swcache::Pointer::", ##__VA_ARGS__)
#define __CELL_SWCACHE_POINTER_ASSERT(x,fmt,...)\
	__CELL_SWCACHE_ASSERT(x, "%s::" fmt, "cell::swcache::Pointer", ##__VA_ARGS__);

__CELL_SWCACHE_BEGIN

#ifdef __SPU__
template<class type, class tCache> class PointerBase : protected CacheResource<tCache>
#else
template<class type> class PointerBase
#endif
{
protected:
	const static unsigned int IS_GRABBED_SA  = 19;
	const static unsigned int IS_GRABBED     = 1 << IS_GRABBED_SA;

		//! @brief Default constructor
	PointerBase():mU32(0){}

	/*! @brief Copy constructor
	@param[in] pointer PointerBase class
	*/
	__CELL_SWCACHE_ALWAYS_INLINE PointerBase(const PointerBase &pointer)
		: mpPPUAddress(pointer.mpPPUAddress)
#ifndef __SPU__
		, mU32(0)
#endif
	{}

	/*! @brief Constructor
	@param[in] PPUaddress main memory address of object
	*/
	__CELL_SWCACHE_ALWAYS_INLINE PointerBase(type *PPUaddress)
		: mpPPUAddress(PPUaddress)
#ifndef __SPU__
		, mU32(0)
#endif
	{}

#ifdef __SPU__
private:
	//! @brief grab cache object in local storage
	// @param[in] size Size of object[in bytes]
	// @param[in] mode RO_MODE or RW_MODE
	// @param[in] attr NO_VTABLE or HAS_VTABLE
	void grabSPUMem(size_t size, PatchMode mode, ClassAttribute attr)
	{
		register unsigned int dmaTag = 0;
		if (__builtin_expect((uintptr_t)mpPPUAddress, 1))
		{
			// get a dma channel
			dmaTag = (unsigned int)this->reserveChannel();
		}
		uint32_t ea;
		unsigned int actual_dmaTag = dmaTag;
		void *ptr = (void *)mpPPUAddress;
#ifndef CELL_SWCACHE_SPU_NO_ALWAYS_INLINE
		register int status = tCache::grab(ptr, size, (mode == RW_MODE) ? IS_WRITABLE : 0, actual_dmaTag, ea, attr == HAS_VTABLE, 1);
#else
		register int status = tCache::grab(ptr, size, (mode == RW_MODE) ? IS_WRITABLE : 0, actual_dmaTag, ea, attr == HAS_VTABLE, 0);
#endif
		__CELL_SWCACHE_POINTER_ASSERT(status >= 0 ||
			status == CELL_SWCACHE_ERROR_NULL_POINTER ||
			status == CELL_SWCACHE_ERROR_NON_CACHED_POINTER,
			"grabSPUMem() cache grab failed ret = %#x : ptr=%p,size=%#x, dmaTag=%d",
			status, ptr, size, actual_dmaTag);
		register unsigned isLoading = 0;
		if (status >= 0)
		{
			// this is cached object
			isLoading = status & IS_LOADING;
			if (__builtin_expect(status & IS_LOADING, 0))
			{
				if (__builtin_expect((status & 0xf) == NOT_CACHED, 0))
					if (__builtin_expect(this->yield() == CELL_OK, 0)) verifyDMARead();
			}
		}
		if (__builtin_expect(actual_dmaTag != dmaTag || status < 0 || !mIsLoading, 0))
		{
			this->releaseChannel(dmaTag);
		}
		mU32 = IS_GRABBED | isLoading | (uintptr_t)ptr;
	}

protected:
	//! @brief grab cache object in local storage(inline version)
	// @param[in] size Size of object[in bytes]
	// @param[in] mode RO_MODE or RW_MODE
	// @param[in] attr NO_VTABLE or HAS_VTABLE
	__CELL_SWCACHE_ALWAYS_INLINE void	grabSPUMemInline(size_t size, PatchMode mode, ClassAttribute attr)
	{
#ifndef CELL_SWCACHE_SPU_NO_ALWAYS_INLINE
		void *spu_ptr = tCache::grabIfCached(mpPPUAddress, (mode == RW_MODE) ? IS_WRITABLE : 0);
		if (__builtin_expect((uintptr_t)spu_ptr, 1))
		{
			mU32 = (uintptr_t)spu_ptr | // mpSPUMem, mIsLoading
				IS_GRABBED; // mIsGrabbed
		} else
#endif
			grabSPUMem(size, mode, attr);
#ifdef CELL_SWCACHE_SIZE_CHECK
		__CELL_SWCACHE_POINTER_ASSERT(!tCache::MemBlockHeader::getHeader((void*)(uintptr_t)mpSPUMem) | (size <= tCache::MemBlockHeader::getHeader((void*)(uintptr_t)mpSPUMem)->size),
			"grabSPUMemInline(): Object with size of %u at %p in PPU address detect cache hit to cache object with smaller size of %u",
			size, mpPPUAddress, tCache::MemBlockHeader::getHeader((void*)(uintptr_t)mpSPUMem)->size);
#endif
	}

	//! @brief Make sure that the DMA read has completed correctly. This doesn't exist on the PPU
	__CELL_SWCACHE_ALWAYS_INLINE void	verifyDMARead()
	{
		if (__builtin_expect(mU32 & IS_LOADING, 0))
		{
			__CELL_SWCACHE_POINTER_DPRINTF("verifyDMARead(): lsa:%#x,ea:%p", mpSPUMem, mpPPUAddress);
			register int status = tCache::waitForGrab((void *)mpSPUMem);
			if (__builtin_expect(status == DMA_COMPLETED, 0))
			{
				this->releaseChannel(tCache::getDmaTag((void *)mpSPUMem));
			}
			mU32 &= ~IS_LOADING;
		}
	}

	//! @brief release the memory if we have some
	__CELL_SWCACHE_ALWAYS_INLINE void releaseSPUMem()
	{
		__CELL_SWCACHE_POINTER_DPRINTF("releaseSPUMem(): lsa:%#x,ea:%p", mpSPUMem, mpPPUAddress);
		if (__builtin_expect(mpSPUMem && (mpSPUMem != (uintptr_t)mpPPUAddress), 1)) tCache::release((void *)mpSPUMem, 1);
		mU32 = 0;
	}
#endif /* #ifdef __SPU__ */

	// member variables
protected:
	type	*mpPPUAddress;	      //!< on the PPU, this is the real memory.  On the SPU, this is the main memory address
	__extension__ union {
		__extension__ struct {
			uint32_t __reserved__	: 12;
			uint32_t mIsGrabbed     :  1; //!< 1: Object is grabbed, 0: Object is not grabbed
			uint32_t mIsLoading		:  1; //!< 1: DMA is still running, 0:DMA for fetching object is completed
			uint32_t mpSPUMem       : 18; //!< Pointer to cached object on local storage
		};
		uint32_t mU32;
	};
} __attribute__((aligned(8)));

/*! @brief Auto-ptr style template which transparently
           works on both the SPU and the PPU.  It automatically
           handles all of the DMA transfers.

           Because of the extra information required, Pointer
           has a sizeof() == 8 bytes.  It contains a pointer for the
           PPU address, a pointer for the SPU local storage address,
           and bookkeeping variables to keep track of data transfers.
@param type   Type of pointer
@param tMode  RO_MODE: Pointer is read-only, RW_MODE: pointer is writable
@param tAttr  NO_VTABLE: Pointer doesn't have pointer to VTABLE, HAS_VTABLE: pointer has pointer to VTABLE
@param tCache Cache implementation 
*/
#ifdef __SPU__
template <class type, PatchMode tMode = RO_MODE, ClassAttribute tAttr = NO_VTABLE, class tCache = DefaultCache<DefaultHeap> > class Pointer : protected PointerBase<void,tCache>
#else
template <class type, PatchMode tMode = RO_MODE, ClassAttribute tAttr = NO_VTABLE> class Pointer : protected PointerBase<type>
#endif
{
	public :
		//! @brief Constructor
		__CELL_SWCACHE_ALWAYS_INLINE Pointer()
		{
			__CELL_SWCACHE_POINTER_DPRINTF("Pointer(): this=%p", this);
		}

		/*! @brief Copy constructor
		@param[in] pointer Pointer class
		*/
		__CELL_SWCACHE_ALWAYS_INLINE Pointer(const Pointer &pointer)
#ifdef __SPU__
			: PointerBase<void,tCache>(pointer)
#else
			: PointerBase<type>(pointer)
#endif
		{
			__CELL_SWCACHE_POINTER_DPRINTF("Pointer(const Pointer &pointer): this=%p, pointer.mpPPUAddress=%p", this, pointer.mpPPUAddress);
			grab();
		}

		/*! @brief Constructor
		@param[in] PPUaddress main memory address of object
		*/
		__CELL_SWCACHE_ALWAYS_INLINE Pointer(type *PPUaddress)
#ifdef __SPU__
			: PointerBase<void,tCache>((void *)PPUaddress)
#else
			: PointerBase<type>(PPUaddress)
#endif
		{
			__CELL_SWCACHE_POINTER_DPRINTF("Pointer(type *PPUaddress=%p): this=%p", PPUaddress, this);
			grab();
		}

		//! @brief Destructor
		__CELL_SWCACHE_ALWAYS_INLINE ~Pointer()
		{
			__CELL_SWCACHE_POINTER_DPRINTF("~Pointer(): %sthis=%p,lsa:%#x,ea:%p", this->mIsLoading ? "IS_LOADING ":"", this, this->mpSPUMem, this->mpPPUAddress);
			release(
		#ifdef CELL_SWCACHE_ASSERT_VERBOSE
				"~Pointer()"
		#endif
				);
		}

		//! @brief grab SPU address
		__CELL_SWCACHE_ALWAYS_INLINE void grab()
		{
		#ifdef __SPU__
			__CELL_SWCACHE_POINTER_DPRINTF("grab(): %sthis=%p,lsa:%#x,ea:%p", this->mIsLoading ? "IS_LOADING ":"", this, this->mpSPUMem, this->mpPPUAddress);
		#if CELL_SWCACHE_SAFE_LV >= 1
			__CELL_SWCACHE_POINTER_ASSERT(!tCache::existsReference(this), "grab() is called for valid object this=%p,lsa:%#x,ea:%p", this, this->mpSPUMem, this->mpPPUAddress);
		#endif
			this->grabSPUMemInline(sizeof(type), tMode, tAttr);
		#if CELL_SWCACHE_SAFE_LV >= 1
			if (this->mpSPUMem)
			{
				__CELL_SWCACHE_POINTER_ASSERT(tCache::registerReference(this) != CELL_SWCACHE_ERROR_MEM_LEAKED, "grab() is called to Pointer instance which has been already initialized this=%p,lsa:%#x,ea:%p", this, this->mpSPUMem, this->mpPPUAddress);
			}
		#endif
		#endif
		}

		//! @brief release cache
		__CELL_SWCACHE_ALWAYS_INLINE void release(const char *funcname = 0)
		{
			(void)funcname;
		#ifdef __SPU__
			__CELL_SWCACHE_POINTER_DPRINTF("release(): %sthis=%p,lsa:%#x,ea:%p", this->mIsLoading ? "IS_LOADING ":"", this, this->mpSPUMem, this->mpPPUAddress);
		#if CELL_SWCACHE_SAFE_LV >= 1
			if (this->mpSPUMem)
			{
				__CELL_SWCACHE_POINTER_ASSERT(tCache::existsReference(this), "%s is called for invalid object this=%p,lsa:%#x,ea:%p", funcname ? : "release()", this, this->mpSPUMem, this->mpPPUAddress);
				tCache::unregisterReference(this);
			}
		#endif
			this->releaseSPUMem();
		#endif
		}

		//! @brief Write back the memory from the SPU LS to the main memory
		__CELL_SWCACHE_ALWAYS_INLINE void flush()
		{
			__CELL_SWCACHE_POINTER_DPRINTF("flush(): %sthis=%p,lsa:%#x,ea:%p", this->mIsLoading ? "IS_LOADING ":"", this, this->mpSPUMem, this->mpPPUAddress);
		#ifdef __SPU__
		#if CELL_SWCACHE_SAFE_LV >= 1
			__CELL_SWCACHE_POINTER_ASSERT(tCache::existsReference(this), "flush() is called for invalid object this=%p,lsa:%#x,ea:%p", this, this->mpSPUMem, this->mpPPUAddress);
		#endif
			tCache::flush((void *)this->mpSPUMem, this->getSystemDmaTag());
		#endif
		}

		//! @brief Set the data to write back to main memory on release
		__CELL_SWCACHE_ALWAYS_INLINE void setWritable()
		{
			__CELL_SWCACHE_POINTER_DPRINTF("setWritable(): %sthis=%p,lsa:%#x,ea:%p", this->mIsLoading ? "IS_LOADING ":"", this, this->mpSPUMem, this->mpPPUAddress);
		#ifdef __SPU__
		#if CELL_SWCACHE_SAFE_LV >= 1
			__CELL_SWCACHE_POINTER_ASSERT(tCache::existsReference(this), "setWritable() is called for invalid object this=%p,lsa:%#x,ea:%p", this, this->mpSPUMem, this->mpPPUAddress);
		#endif
			tCache::changeToWritable((void *)this->mpSPUMem);
		#endif
		}
		
		/*! @brief operator=.  Only available on the PPU
		@param[in] pPtr		EA to set the swcache::Pointer to
		@param[out] this	A reference to the object
		*/
		#ifdef __PPU__
		__CELL_SWCACHE_ALWAYS_INLINE Pointer &operator=(type *pPtr)
		{
			__CELL_SWCACHE_POINTER_DPRINTF("operator=(type *pPtr=%p): %sthis=%p", pPtr, this->mIsLoading ? "IS_LOADING ":"", this);
			this->mpPPUAddress = pPtr;
			this->mpSPUMem = 0;
			this->mIsLoading = 0;
			this->mIsGrabbed = 0;
			return(*this);
		}
		#endif
		
		/*! @brief operator=.  Only available on the SPU
		@param[in] pPtr		Change where the swcache::Pointer is pointing 
		@param[out] this	A reference to the object
		*/
		#ifdef __SPU__
		__CELL_SWCACHE_ALWAYS_INLINE Pointer &operator=(const Pointer &pPtr)
		{
			__CELL_SWCACHE_POINTER_DPRINTF("operator=(const Pointer &pPtr): %sthis=%p,mIsGrabbed=%d, mpSPUMem=%p,mpPPUAddress=%p,pPtr.mpPPUAddress=%p", this->mIsLoading ? "IS_LOADING ":"", this, this->mIsGrabbed, this->mpSPUMem, this->mpPPUAddress, pPtr.mpPPUAddress);
			// save off the PPU Address since we might be doing something like:
			//		pLinkedList = pLinkedList->pNext
			// if we don't, the release will clobber the pNext var
			updatePPUAddress((type *)pPtr.mpPPUAddress
		#ifdef CELL_ASSERT_VERBOSE
				, "operator=(const Pointer &)"
		#endif
				);

			return(*this);
		}
		#endif

		/*! @brief operator* to de-reference the pointer
		@param[out] reference	A reference to the underlying object
		*/
		__CELL_SWCACHE_ALWAYS_INLINE type &operator*()
		{
			__CELL_SWCACHE_POINTER_ASSERT(this->mpPPUAddress, "operator*(): NULL pointer is accessed!!");
		#ifdef __SPU__
			__CELL_SWCACHE_POINTER_DPRINTF_IN("operator*(): %sthis=%p, lsa:%#x,ea:%p", this->mIsLoading?"IS_LOADING ":"", this, this->mpSPUMem, this->mpPPUAddress);
			__CELL_SWCACHE_POINTER_ASSERT(this->mpSPUMem,	"operator*(): (SAFE_LV=%d) Dereference to Pointer object@%p without grab",
				CELL_SWCACHE_SAFE_LV, this);
		#if CELL_SWCACHE_SAFE_LV >= 1
			unsigned int dmaTag;
			uint32_t ea;
			int status = tCache::query((void *)this->mpSPUMem, sizeof(type), dmaTag, ea); (void)status;
			__CELL_SWCACHE_POINTER_ASSERT(status == CELL_SWCACHE_ERROR_NON_CACHED_POINTER || (uintptr_t)this->mpPPUAddress == ea,
				"operator*(): (SAFE_LV=%d) Stale LS pointer(=%p) is stored in Pointer object@%p ea:%p",
				CELL_SWCACHE_SAFE_LV, this->mpSPUMem, this, this->mpPPUAddress);
			__CELL_SWCACHE_POINTER_ASSERT(tCache::existsReference(this),
				"operator*(): (SAFE_LV=%d) Dereference to Pointer object@%p without grab",
				CELL_SWCACHE_SAFE_LV, this);
		#endif
			this->verifyDMARead();
			__CELL_SWCACHE_DPRINTF_OUT(" lsa:%#x", this->mpSPUMem);
			return(*((type *)this->mpSPUMem));
		#else
			__CELL_SWCACHE_POINTER_DPRINTF("operator*(): this=%p, lsa:%#x,ea:%p", this, this->mpSPUMem, this->mpPPUAddress);
			return(*((type *)this->mpPPUAddress));
		#endif
		}

		/*! @brief operator-> to access the pointer
		@param[out] pointer		A pointer to the underlying object
		*/
		__CELL_SWCACHE_ALWAYS_INLINE type *operator->()
		{
			__CELL_SWCACHE_POINTER_ASSERT(this->mpPPUAddress, "operator->(): NULL pointer is accessed!! this=%p", this);
		#ifdef __SPU__
			__CELL_SWCACHE_POINTER_DPRINTF_IN("operator->(): %sthis=%p, lsa:%#x,ea:%p", this->mIsLoading?"IS_LOADING ":"", this, this->mpSPUMem, this->mpPPUAddress);
			__CELL_SWCACHE_POINTER_ASSERT(this->mpSPUMem,	"operator->(): (SAFE_LV=%d) Dereference to Pointer object@%p without grab",
				CELL_SWCACHE_SAFE_LV, this);
		#if CELL_SWCACHE_SAFE_LV >= 1
			unsigned int dmaTag;
			uint32_t ea;
			int status = tCache::query((void *)this->mpSPUMem, sizeof(type), dmaTag, ea); (void)status;
			__CELL_SWCACHE_POINTER_ASSERT(status == CELL_SWCACHE_ERROR_NON_CACHED_POINTER || (uintptr_t)this->mpPPUAddress == ea,
				"operator->(): (SAFE_LV=%d) Stale LS pointer(=%p) is stored in Pointer object@%p ea:%p",
				CELL_SWCACHE_SAFE_LV, this->mpSPUMem, this, this->mpPPUAddress);
			__CELL_SWCACHE_POINTER_ASSERT(tCache::existsReference(this),
				"operator->(): (SAFE_LV=%d) Dereference to Pointer object@%p without grab",
				CELL_SWCACHE_SAFE_LV, this);
		#endif
			this->verifyDMARead();
			__CELL_SWCACHE_DPRINTF_OUT(" lsa:%#x", this->mpSPUMem);
			return((type *)this->mpSPUMem);
		#else
			__CELL_SWCACHE_POINTER_DPRINTF("operator->(): this=%p, lsa:%#x,ea:%p", this, this->mpSPUMem, this->mpPPUAddress);
			return((type *)this->mpPPUAddress);
		#endif
		}

		/*! @brief operator bool
		@param[out] bool The boolean operator
		*/
		__CELL_SWCACHE_ALWAYS_INLINE operator bool()
		{
			__CELL_SWCACHE_POINTER_DPRINTF("operator bool(): this=%p, ea:%p", this, this->mpPPUAddress);
			return(this->mpPPUAddress);
		}

		/*! @brief operator bool const
		@param[out] bool The boolean operator const
		*/
		__CELL_SWCACHE_ALWAYS_INLINE operator bool() const
		{
			__CELL_SWCACHE_POINTER_DPRINTF("operator bool() const: this=%p, ea:%p", this, this->mpPPUAddress);
			return(this->mpPPUAddress);
		}

		/*! @brief operator void *const
		@param[out] void* The void *operator
		*/
		__CELL_SWCACHE_ALWAYS_INLINE operator void *()
		{
			__CELL_SWCACHE_POINTER_DPRINTF("operator void *(): this=%p, ea:%p", this, this->mpPPUAddress);
			return(this->mpPPUAddress);
		}

		/*! @brief operator void * const
		@param[out] void* The void * operator const
		*/
		__CELL_SWCACHE_ALWAYS_INLINE operator void *() const
		{
			__CELL_SWCACHE_POINTER_DPRINTF("operator void *() const: this=%p, ea:%p", this, this->mpPPUAddress);
			return(this->mpPPUAddress);
		}
#ifdef __PPU__
		/*! @brief operator type *const
		@param[out] type* The type *operator
		*/
		__CELL_SWCACHE_ALWAYS_INLINE operator type *()
		{
			__CELL_SWCACHE_POINTER_DPRINTF("operator void *(): this=%p, ea:%p", this, this->mpPPUAddress);
			return(this->mpPPUAddress);
		}

		/*! @brief operator type * const
		@param[out] type* The type * operator const
		*/
		__CELL_SWCACHE_ALWAYS_INLINE operator type *() const
		{
			__CELL_SWCACHE_POINTER_DPRINTF("operator void *() const: this=%p, ea:%p", this, this->mpPPUAddress);
			return(this->mpPPUAddress);
		}
#endif
		/*! @brief operator++ Prefix increment
		*/
		__CELL_SWCACHE_ALWAYS_INLINE Pointer &operator++()
		{
			__CELL_SWCACHE_POINTER_DPRINTF_IN("operator++()[prefix]: %sthis=%p, lsa:%#x,ea:%p", this->mIsLoading?"IS_LOADING ":"",this, this->mpSPUMem, this->mpPPUAddress);
			updatePPUAddress((type *)(this->mpPPUAddress) + 1
		#ifdef CELL_ASSERT_VERBOSE
				, "operator++()[prefix]"
		#endif
				);
			__CELL_SWCACHE_DPRINTF_OUT(" ea:%p", this->mpPPUAddress);
			return(*this);
		}

		/*! @brief operator-- Prefix decrement
		*/
		__CELL_SWCACHE_ALWAYS_INLINE Pointer &operator--()
		{
			__CELL_SWCACHE_POINTER_DPRINTF_IN("operator--()[prefix]: %sthis=%p, lsa:%#x,ea:%p", this->mIsLoading?"IS_LOADING ":"",this, this->mpSPUMem, this->mpPPUAddress);
			updatePPUAddress((type *)(this->mpPPUAddress) - 1
		#ifdef CELL_ASSERT_VERBOSE
				, "operator--()[prefix]"
		#endif
				);
			__CELL_SWCACHE_DPRINTF_OUT(" ea:%p", this->mpPPUAddress);
			return(*this);
		}
	
		/*! @brief operator++ Postfix increment
		*/
		__CELL_SWCACHE_ALWAYS_INLINE Pointer operator++(int)
		{
			__CELL_SWCACHE_POINTER_DPRINTF_IN("operator++()[postfix]: %sthis=%p, lsa:%#x,ea:%p", this->mIsLoading?"IS_LOADING ":"",this, this->mpSPUMem, this->mpPPUAddress);
			Pointer tmp((type *)(this->mpPPUAddress));
			updatePPUAddress((type *)(this->mpPPUAddress) + 1
		#ifdef CELL_ASSERT_VERBOSE
				, "operator++()[postfix]"
		#endif
				);
			__CELL_SWCACHE_DPRINTF_OUT(" ea:%p", this->mpPPUAddress);
			return(tmp);
		}

		/*! @brief operator-- Postfix decrement
		*/
		__CELL_SWCACHE_ALWAYS_INLINE Pointer operator--(int)
		{
			__CELL_SWCACHE_POINTER_DPRINTF_IN("operator--()[postfix]: %sthis=%p, lsa:%#x,ea:%p", this->mIsLoading?"IS_LOADING ":"",this, this->mpSPUMem, this->mpPPUAddress);
			Pointer tmp((type *)(this->mpPPUAddress));
			updatePPUAddress((type *)(this->mpPPUAddress) - 1
		#ifdef CELL_ASSERT_VERBOSE
				, "operator--()[postfix]"
		#endif
				);
			__CELL_SWCACHE_DPRINTF_OUT(" ea:%p", this->mpPPUAddress);
			return(tmp);
		}

		/*! @brief operator+ addition
		*/
		__CELL_SWCACHE_ALWAYS_INLINE Pointer operator+(int val) const
		{
			__CELL_SWCACHE_POINTER_DPRINTF("operator+(int val=%d): %sthis=%p, lsa:%#x,ea:%p", val, this->mIsLoading?"IS_LOADING ":"",this, this->mpSPUMem, this->mpPPUAddress);
			Pointer tmp((type *)(this->mpPPUAddress) + val);
			return(tmp);
		}

		/*! @brief operator- subtraction
		*/
		__CELL_SWCACHE_ALWAYS_INLINE Pointer operator-(int val) const
		{
			__CELL_SWCACHE_POINTER_DPRINTF("operator-(int val=%d): %sthis=%p, lsa:%#x,ea:%p", val, this->mIsLoading?"IS_LOADING ":"",this, this->mpSPUMem, this->mpPPUAddress);
			Pointer tmp((type *)(this->mpPPUAddress) - val);
			return(tmp);
		}

		/*! @brief operator+= addition
		*/
		__CELL_SWCACHE_ALWAYS_INLINE Pointer &operator+=(int val)
		{
			__CELL_SWCACHE_POINTER_DPRINTF_IN("operator+=(int val=%d): %sthis=%p, lsa:%#x,ea:%p", val, this->mIsLoading?"IS_LOADING ":"",this, this->mpSPUMem, this->mpPPUAddress);
			updatePPUAddress((type *)(this->mpPPUAddress) + val
		#ifdef CELL_ASSERT_VERBOSE
				, "operator+=(int)"
		#endif
				);
			__CELL_SWCACHE_DPRINTF_OUT(" ea:%p", this->mpPPUAddress);
			return(*this);
		}

		/*! @brief operator- subtraction
		*/
		__CELL_SWCACHE_ALWAYS_INLINE Pointer &operator-=(int val)
		{
			__CELL_SWCACHE_POINTER_DPRINTF_IN("operator-=(int val=%d): %sthis=%p, lsa:%#x,ea:%p", val, this->mIsLoading?"IS_LOADING ":"",this, this->mpSPUMem, this->mpPPUAddress);
			updatePPUAddress((type *)(this->mpPPUAddress) - val
		#ifdef CELL_ASSERT_VERBOSE
				, "operator-=(int)"
		#endif
				);
			__CELL_SWCACHE_DPRINTF_OUT(" ea:%p", this->mpPPUAddress);
			return(*this);
		}

#ifdef __SPU__
		template<class tmpType, PatchMode tmpMode1, PatchMode tmpMode2, ClassAttribute tmpAttr1, ClassAttribute tmpAttr2, class tmpCache> friend __CELL_SWCACHE_INLINE bool operator==(const Pointer<tmpType, tmpMode1, tmpAttr1, tmpCache> &a, const Pointer<tmpType, tmpMode2, tmpAttr2, tmpCache> &b);
		template<class tmpType, PatchMode tmpMode, ClassAttribute tmpAttr, class tmpCache> friend __CELL_SWCACHE_INLINE bool operator==(const Pointer<tmpType, tmpMode, tmpAttr, tmpCache> &a, const tmpType *b);
		template<class tmpType, PatchMode tmpMode, ClassAttribute tmpAttr, class tmpCache> friend __CELL_SWCACHE_INLINE bool operator==(const tmpType *a, const Pointer<tmpType, tmpMode, tmpAttr, tmpCache> &b);

		template<class tmpType, PatchMode tmpMode1, PatchMode tmpMode2, ClassAttribute tmpAttr1, ClassAttribute tmpAttr2, class tmpCache> friend __CELL_SWCACHE_INLINE bool operator!=(const Pointer<tmpType, tmpMode1, tmpAttr1, tmpCache> &a, const Pointer<tmpType, tmpMode2, tmpAttr2, tmpCache> &b);
		template<class tmpType, PatchMode tmpMode, ClassAttribute tmpAttr, class tmpCache> friend __CELL_SWCACHE_INLINE bool operator!=(const Pointer<tmpType, tmpMode, tmpAttr, tmpCache> &a, const tmpType *b);
		template<class tmpType, PatchMode tmpMode, ClassAttribute tmpAttr, class tmpCache> friend __CELL_SWCACHE_INLINE bool operator!=(const tmpType *a, const Pointer<tmpType, tmpMode, tmpAttr, tmpCache> &b);

		template<class tmpType, PatchMode tmpMode1, PatchMode tmpMode2, ClassAttribute tmpAttr1, ClassAttribute tmpAttr2, class tmpCache> friend __CELL_SWCACHE_INLINE bool operator>(const Pointer<tmpType, tmpMode1, tmpAttr1, tmpCache> &a, const Pointer<tmpType, tmpMode2, tmpAttr2, tmpCache> &b);
		template<class tmpType, PatchMode tmpMode, ClassAttribute tmpAttr, class tmpCache> friend __CELL_SWCACHE_INLINE bool operator>(const Pointer<tmpType, tmpMode, tmpAttr, tmpCache> &a, const tmpType *b);
		template<class tmpType, PatchMode tmpMode, ClassAttribute tmpAttr, class tmpCache> friend __CELL_SWCACHE_INLINE bool operator>(const tmpType *a, const Pointer<tmpType, tmpMode, tmpAttr, tmpCache> &b);

		template<class tmpType, PatchMode tmpMode1, PatchMode tmpMode2, ClassAttribute tmpAttr1, ClassAttribute tmpAttr2, class tmpCache> friend __CELL_SWCACHE_INLINE bool operator<(const Pointer<tmpType, tmpMode1, tmpAttr1, tmpCache> &a, const Pointer<tmpType, tmpMode2, tmpAttr2, tmpCache> &b);
		template<class tmpType, PatchMode tmpMode, ClassAttribute tmpAttr, class tmpCache> friend __CELL_SWCACHE_INLINE bool operator<(const Pointer<tmpType, tmpMode, tmpAttr, tmpCache> &a, const tmpType *b);
		template<class tmpType, PatchMode tmpMode, ClassAttribute tmpAttr, class tmpCache> friend __CELL_SWCACHE_INLINE bool operator<(const tmpType *a, const Pointer<tmpType, tmpMode, tmpAttr, tmpCache> &b);

		template<class tmpType, PatchMode tmpMode1, PatchMode tmpMode2, ClassAttribute tmpAttr1, ClassAttribute tmpAttr2, class tmpCache> friend __CELL_SWCACHE_INLINE bool operator>=(const Pointer<tmpType, tmpMode1, tmpAttr1, tmpCache> &a, const Pointer<tmpType, tmpMode2, tmpAttr2, tmpCache> &b);
		template<class tmpType, PatchMode tmpMode, ClassAttribute tmpAttr, class tmpCache> friend __CELL_SWCACHE_INLINE bool operator>=(const Pointer<tmpType, tmpMode, tmpAttr, tmpCache> &a, const tmpType *b);
		template<class tmpType, PatchMode tmpMode, ClassAttribute tmpAttr, class tmpCache> friend __CELL_SWCACHE_INLINE bool operator>=(const tmpType *a, const Pointer<tmpType, tmpMode, tmpAttr, tmpCache> &b);

		template<class tmpType, PatchMode tmpMode1, PatchMode tmpMode2, ClassAttribute tmpAttr1, ClassAttribute tmpAttr2, class tmpCache> friend __CELL_SWCACHE_INLINE bool operator<=(const Pointer<tmpType, tmpMode1, tmpAttr1, tmpCache> &a, const Pointer<tmpType, tmpMode2, tmpAttr2, tmpCache> &b);
		template<class tmpType, PatchMode tmpMode, ClassAttribute tmpAttr, class tmpCache> friend __CELL_SWCACHE_INLINE bool operator<=(const Pointer<tmpType, tmpMode, tmpAttr, tmpCache> &a, const tmpType *b);
		template<class tmpType, PatchMode tmpMode, ClassAttribute tmpAttr, class tmpCache> friend __CELL_SWCACHE_INLINE bool operator<=(const tmpType *a, const Pointer<tmpType, tmpMode, tmpAttr, tmpCache> &b);
#else // #ifdef __SPU__
		template<class tmpType, PatchMode tmpMode1, PatchMode tmpMode2, ClassAttribute tmpAttr1, ClassAttribute tmpAttr2> friend __CELL_SWCACHE_INLINE bool operator==(const Pointer<tmpType,tmpMode1,tmpAttr1> &a, const Pointer<tmpType,tmpMode2,tmpAttr2> &b);

		template<class tmpType, PatchMode tmpMode1, PatchMode tmpMode2, ClassAttribute tmpAttr1, ClassAttribute tmpAttr2> friend __CELL_SWCACHE_INLINE bool operator!=(const Pointer<tmpType,tmpMode1,tmpAttr1> &a, const Pointer<tmpType,tmpMode2,tmpAttr2> &b);

		template<class tmpType, PatchMode tmpMode1, PatchMode tmpMode2, ClassAttribute tmpAttr1, ClassAttribute tmpAttr2> friend __CELL_SWCACHE_INLINE bool operator>(const Pointer<tmpType,tmpMode1,tmpAttr1> &a, const Pointer<tmpType,tmpMode2,tmpAttr2> &b);

		template<class tmpType, PatchMode tmpMode1, PatchMode tmpMode2, ClassAttribute tmpAttr1, ClassAttribute tmpAttr2> friend __CELL_SWCACHE_INLINE bool operator<(const Pointer<tmpType,tmpMode1,tmpAttr1> &a, const Pointer<tmpType,tmpMode2,tmpAttr2> &b);

		template<class tmpType, PatchMode tmpMode1, PatchMode tmpMode2, ClassAttribute tmpAttr1, ClassAttribute tmpAttr2> friend __CELL_SWCACHE_INLINE bool operator>=(const Pointer<tmpType,tmpMode1,tmpAttr1> &a, const Pointer<tmpType,tmpMode2,tmpAttr2> &b);

		template<class tmpType, PatchMode tmpMode1, PatchMode tmpMode2, ClassAttribute tmpAttr1, ClassAttribute tmpAttr2> friend __CELL_SWCACHE_INLINE bool operator<=(const Pointer<tmpType,tmpMode1,tmpAttr1> &a, const Pointer<tmpType,tmpMode2,tmpAttr2> &b);
#endif // #ifdef __SPU__

		class Grab
		{
			Pointer &mObj;
			int mSetWritable;
		public:
			Grab(Pointer &obj, int setWritable = 0)
				: mObj        (obj         )
				, mSetWritable(setWritable )
			{
				mObj.grab();
			}

			~Grab()
			{
				if (mSetWritable) mObj.setWritable();
				mObj.release();
			}
		};

	protected :

		__CELL_SWCACHE_ALWAYS_INLINE void updatePPUAddress(type *newAddr, const char *funcname = 0)
		{
			(void)funcname;
		#ifdef __SPU__
			if (this->mU32 & PointerBase<void,tCache>::IS_GRABBED)
			{
				release(funcname);
				this->mpPPUAddress = (void *)newAddr;
				grab();
			} else {
				this->mpPPUAddress = (void *)newAddr;
				this->mU32 = 0;
			}
		#else
			this->mpPPUAddress = newAddr;
			this->mpSPUMem = 0;
			this->mIsLoading = 0;
			this->mIsGrabbed = 0;
		#endif
		}
};

#ifdef __SPU__
template<class tmpType, PatchMode tmpMode1, PatchMode tmpMode2, ClassAttribute tmpAttr1, ClassAttribute tmpAttr2, class tmpCache> __CELL_SWCACHE_INLINE bool operator==(const Pointer<tmpType, tmpMode1, tmpAttr1, tmpCache> &a, const Pointer<tmpType, tmpMode2, tmpAttr2, tmpCache> &b)
	{ return(a.mpPPUAddress == b.mpPPUAddress); }	
template<class tmpType, PatchMode tmpMode, ClassAttribute tmpAttr, class tmpCache> __CELL_SWCACHE_INLINE bool operator==(const Pointer<tmpType, tmpMode, tmpAttr, tmpCache> &a, const tmpType *b)
	{ return(a.mpPPUAddress == b); }
template<class tmpType, PatchMode tmpMode, ClassAttribute tmpAttr, class tmpCache> __CELL_SWCACHE_INLINE bool operator==(const tmpType *a, const Pointer<tmpType, tmpMode, tmpAttr, tmpCache> &b)
{ return(a == b.mpPPUAddress); }
template<class tmpType, PatchMode tmpMode1, PatchMode tmpMode2, ClassAttribute tmpAttr1, ClassAttribute tmpAttr2, class tmpCache> __CELL_SWCACHE_INLINE bool operator!=(const Pointer<tmpType, tmpMode1, tmpAttr1, tmpCache> &a, const Pointer<tmpType, tmpMode2, tmpAttr2, tmpCache> &b)
{ return(a.mpPPUAddress != b.mpPPUAddress); }	
template<class tmpType, PatchMode tmpMode, ClassAttribute tmpAttr, class tmpCache> __CELL_SWCACHE_INLINE bool operator!=(const Pointer<tmpType, tmpMode, tmpAttr, tmpCache> &a, const tmpType *b)
{ return(a.mpPPUAddress != b); }
template<class tmpType, PatchMode tmpMode, ClassAttribute tmpAttr, class tmpCache> __CELL_SWCACHE_INLINE bool operator!=(const tmpType *a, const Pointer<tmpType, tmpMode, tmpAttr, tmpCache> &b)
{ return(a != b.mpPPUAddress); }
template<class tmpType, PatchMode tmpMode1, PatchMode tmpMode2, ClassAttribute tmpAttr1, ClassAttribute tmpAttr2, class tmpCache> __CELL_SWCACHE_INLINE bool operator>(const Pointer<tmpType, tmpMode1, tmpAttr1, tmpCache> &a, const Pointer<tmpType, tmpMode2, tmpAttr2, tmpCache> &b)
{ return(a.mpPPUAddress > b.mpPPUAddress); }	
template<class tmpType, PatchMode tmpMode, ClassAttribute tmpAttr, class tmpCache> __CELL_SWCACHE_INLINE bool operator>(const Pointer<tmpType, tmpMode, tmpAttr, tmpCache> &a, const tmpType *b)
{ return(a.mpPPUAddress > b); }
template<class tmpType, PatchMode tmpMode, ClassAttribute tmpAttr, class tmpCache> __CELL_SWCACHE_INLINE bool operator>(const tmpType *a, const Pointer<tmpType, tmpMode, tmpAttr, tmpCache> &b)
{ return(a > b.mpPPUAddress); }
template<class tmpType, PatchMode tmpMode1, PatchMode tmpMode2, ClassAttribute tmpAttr1, ClassAttribute tmpAttr2, class tmpCache> __CELL_SWCACHE_INLINE bool operator<(const Pointer<tmpType, tmpMode1, tmpAttr1, tmpCache> &a, const Pointer<tmpType, tmpMode2, tmpAttr2, tmpCache> &b)
{ return(a.mpPPUAddress < b.mpPPUAddress); }	
template<class tmpType, PatchMode tmpMode, ClassAttribute tmpAttr, class tmpCache> __CELL_SWCACHE_INLINE bool operator<(const Pointer<tmpType, tmpMode, tmpAttr, tmpCache> &a, const tmpType *b)
{ return(a.mpPPUAddress < b); }
template<class tmpType, PatchMode tmpMode, ClassAttribute tmpAttr, class tmpCache> __CELL_SWCACHE_INLINE bool operator<(const tmpType *a, const Pointer<tmpType, tmpMode, tmpAttr, tmpCache> &b)
{ return(a < b.mpPPUAddress); }
template<class tmpType, PatchMode tmpMode1, PatchMode tmpMode2, ClassAttribute tmpAttr1, ClassAttribute tmpAttr2, class tmpCache> __CELL_SWCACHE_INLINE bool operator>=(const Pointer<tmpType, tmpMode1, tmpAttr1, tmpCache> &a, const Pointer<tmpType, tmpMode2, tmpAttr2, tmpCache> &b)
{ return(a.mpPPUAddress >= b.mpPPUAddress); }	
template<class tmpType, PatchMode tmpMode, ClassAttribute tmpAttr, class tmpCache> __CELL_SWCACHE_INLINE bool operator>=(const Pointer<tmpType, tmpMode, tmpAttr, tmpCache> &a, const tmpType *b)
{ return(a.mpPPUAddress >= b); }
template<class tmpType, PatchMode tmpMode, ClassAttribute tmpAttr, class tmpCache> __CELL_SWCACHE_INLINE bool operator>=(const tmpType *a, const Pointer<tmpType, tmpMode, tmpAttr, tmpCache> &b)
{ return(a >= b.mpPPUAddress); }
template<class tmpType, PatchMode tmpMode1, PatchMode tmpMode2, ClassAttribute tmpAttr1, ClassAttribute tmpAttr2, class tmpCache> __CELL_SWCACHE_INLINE bool operator<=(const Pointer<tmpType, tmpMode1, tmpAttr1, tmpCache> &a, const Pointer<tmpType, tmpMode2, tmpAttr2, tmpCache> &b)
{ return(a.mpPPUAddress <= b.mpPPUAddress); }	
template<class tmpType, PatchMode tmpMode, ClassAttribute tmpAttr, class tmpCache> __CELL_SWCACHE_INLINE bool operator<=(const Pointer<tmpType, tmpMode, tmpAttr, tmpCache> &a, const tmpType *b)
{ return(a.mpPPUAddress <= b); }
template<class tmpType, PatchMode tmpMode, ClassAttribute tmpAttr, class tmpCache> __CELL_SWCACHE_INLINE bool operator<=(const tmpType *a, const Pointer<tmpType, tmpMode, tmpAttr, tmpCache> &b)
{ return(a <= b.mpPPUAddress); }

#else	// #ifdef __SPU__

template<class tmpType, PatchMode tmpMode1, PatchMode tmpMode2, ClassAttribute tmpAttr1, ClassAttribute tmpAttr2> __CELL_SWCACHE_INLINE bool operator==(const Pointer<tmpType,tmpMode1,tmpAttr1> &a, const Pointer<tmpType,tmpMode2,tmpAttr2> &b)
{ return(a.mpPPUAddress == b.mpPPUAddress); }
template<class tmpType, PatchMode tmpMode1, PatchMode tmpMode2, ClassAttribute tmpAttr1, ClassAttribute tmpAttr2> __CELL_SWCACHE_INLINE bool operator!=(const Pointer<tmpType,tmpMode1,tmpAttr1> &a, const Pointer<tmpType,tmpMode2,tmpAttr2> &b)
{ return(a.mpPPUAddress != b.mpPPUAddress); }
template<class tmpType, PatchMode tmpMode1, PatchMode tmpMode2, ClassAttribute tmpAttr1, ClassAttribute tmpAttr2> __CELL_SWCACHE_INLINE bool operator>(const Pointer<tmpType,tmpMode1,tmpAttr1> &a, const Pointer<tmpType,tmpMode2,tmpAttr2> &b)
{ return(a.mpPPUAddress > b.mpPPUAddress); }
template<class tmpType, PatchMode tmpMode1, PatchMode tmpMode2, ClassAttribute tmpAttr1, ClassAttribute tmpAttr2> __CELL_SWCACHE_INLINE bool operator<(const Pointer<tmpType,tmpMode1,tmpAttr1> &a, const Pointer<tmpType,tmpMode2,tmpAttr2> &b)
{ return(a.mpPPUAddress < b.mpPPUAddress); }
template<class tmpType, PatchMode tmpMode1, PatchMode tmpMode2, ClassAttribute tmpAttr1, ClassAttribute tmpAttr2> __CELL_SWCACHE_INLINE bool operator>=(const Pointer<tmpType,tmpMode1,tmpAttr1> &a, const Pointer<tmpType,tmpMode2,tmpAttr2> &b)
{ return(a.mpPPUAddress >= b.mpPPUAddress); }
template<class tmpType, PatchMode tmpMode1, PatchMode tmpMode2, ClassAttribute tmpAttr1, ClassAttribute tmpAttr2> __CELL_SWCACHE_INLINE bool operator<=(const Pointer<tmpType,tmpMode1,tmpAttr1> &a, const Pointer<tmpType,tmpMode2,tmpAttr2> &b)
{ return(a.mpPPUAddress <= b.mpPPUAddress); }

#endif // #ifdef __SPU__

__CELL_SWCACHE_END

#endif // __CELL_SWCACHE_POINTER_H__
