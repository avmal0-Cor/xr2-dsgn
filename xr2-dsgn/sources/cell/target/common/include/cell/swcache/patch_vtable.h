/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2008 Sony Computer Entertainment Inc.
 * All Rights Reserved.
 */

#ifndef __CELL_SWCACHE_PATCH_VTABLE_H__
#define __CELL_SWCACHE_PATCH_VTABLE_H__

/* standard C++ header */
#include <stdint.h>
#include <stddef.h> /* size_t */
#include <stdlib.h> /* malloc */
#include <stdarg.h>

#include <cell/swcache/define.h>

#define CELL_SWCACHE_VFUNC(x) (void *)&x,&x

__CELL_SWCACHE_BEGIN

typedef uintptr_t Vtab;

//! @brief header for VTABLE pointer -> Class id table
class ClassIdTabHeader {
public:
	// constants
	static const size_t MAX_SIZE_VTAB           = 4*1024;     //!< Maximum size of VTABLE
	static const uint32_t __VTABLE_MAGIC_WORD__ =             //!< Magic word embedded in VTABLE header
		((unsigned)'V' << 24) |
		((unsigned)'T' << 16) |
		((unsigned)'A' << 8 ) |
		 (unsigned)'B';

	// typedef
	//! @brief VTABLE pointer -> Class id mapping
	typedef struct Vtab2id {
		Vtab    *mpVtab;    //!< VTABLE pointer
		uint32_t mSize :18; //!< Size of class
		uint32_t mId   :14; //!< Class id
	} Vtab2id;

	//! @brief VTABLE header
	typedef struct VtableHeader {
		uint32_t mMagicWord;       //!< Magic word
		uint32_t mSizeOfClass :18; //!< Size of class
		uint32_t mId          :14; //!< Class id
		Vtab  *mpOrgVtab;          //!< Original VTABLE pointer
		uint32_t __pad__[1];
	} __attribute__((aligned(16))) VtableHeader;

	typedef Vtab *(*FpBuildVtable)(uint16_t id, Vtab *pVtab); //!< typedef of build vtable function pointer

	// functions
private:

	static ClassIdTabHeader *setTabOnSPU(ClassIdTabHeader *pTabOnSPU = 0)
	{
		static ClassIdTabHeader *spTabOnSPU = 0;
		ClassIdTabHeader *old = spTabOnSPU;
		if (pTabOnSPU) spTabOnSPU = pTabOnSPU;
		return old;
	}

	static FpBuildVtable __setFpBuildVtable(FpBuildVtable fp = 0)
	{
		static FpBuildVtable sfpBuildVtable = 0;
		FpBuildVtable old = sfpBuildVtable;
		if (fp) sfpBuildVtable = fp;
		return old;
	}

	static unsigned int max(unsigned int x, unsigned int y) { return (x > y) ? x : y; }

public:

	/*! @brief get VTABLE header pointer
	@param[in] pClass         Pointer to class
	@return                   Pointer to VTABLE header
	*/
	static VtableHeader *getVtabHeader(void *pClass) { return (*(ClassIdTabHeader::VtableHeader **)pClass)-1; }

	/*! @brief obtain size of this table
	@return                   Size of this table
	*/
	unsigned int getSize() {
		return sizeof(ClassIdTabHeader) + mSize * sizeof(Vtab2id);
	}

	//! @brief Constructor
	ClassIdTabHeader() : mSize(0) {}

#ifdef __SPU__
	/*! @brief Get VTABLE pointer -> Class id table
	@return                   Pointer to table
	*/
	static ClassIdTabHeader *getClassIdTabOnSPU() { return setTabOnSPU(); }

	/*! @brief Set VTABLE pointer -> Class id table
	@param[in] pTab           Pointer to table
	*/
	static void setClassIdTabOnSPU(ClassIdTabHeader *pTab) { (void)setTabOnSPU(pTab); }

	/*! @brief Get Build VTABLE function pointer
	@return                   function pointer
	*/
	static FpBuildVtable getFpBuildVtable() { return __setFpBuildVtable(); }

	/*! @brief Set Build VTABLE function pointer
	@param[in] fp             function pointer
	*/
	static void setFpBuildVtable(FpBuildVtable fp) { (void)__setFpBuildVtable(fp); }

	/*! @brief Create fake VTABLE with specified virtual functions
	           This function is helper function to implement FpBuildVtable function pointer by user
	@param[in] id             Class id
	@param[in] pOrgVtab       Original(on main memory) VTABLE pointer
	@param[in] size_of_class  Size of class
	@param[in] ...            Array of pair of virtual function pointers and offset address in VTABLE of that function terminated by NULL
	@return                   Pointer to created VTABLE
	*/
	static Vtab *buildVtable(uint16_t id, Vtab *pOrgVtab, uint16_t size_of_class, ...)
	{
		__CELL_SWCACHE_DPRINTF_3_IN("%s: id=%d, pOrgVtab=%p, size_of_class=%d", __PRETTY_FUNCTION__, id, pOrgVtab, size_of_class);
		//! vtable directory
		static ClassIdTabHeader::Vtab2id spVtabDir[32] = {
			{0,0,0x3fff},
			{0,0,0x3fff},
			{0,0,0x3fff},
			{0,0,0x3fff},
			{0,0,0x3fff},
			{0,0,0x3fff},
			{0,0,0x3fff},
			{0,0,0x3fff},
			{0,0,0x3fff},
			{0,0,0x3fff},
			{0,0,0x3fff},
			{0,0,0x3fff},
			{0,0,0x3fff},
			{0,0,0x3fff},
			{0,0,0x3fff},
			{0,0,0x3fff},
			{0,0,0x3fff},
			{0,0,0x3fff},
			{0,0,0x3fff},
			{0,0,0x3fff},
			{0,0,0x3fff},
			{0,0,0x3fff},
			{0,0,0x3fff},
			{0,0,0x3fff},
			{0,0,0x3fff},
			{0,0,0x3fff},
			{0,0,0x3fff},
			{0,0,0x3fff},
			{0,0,0x3fff},
			{0,0,0x3fff},
			{0,0,0x3fff},
			{0,0,0x3fff}
		};

		// search VTABLE directory
		uint16_t hash = id & 31; 
		for(int i = 0; ; i++, hash = ((hash + 1) & 31))
		{
			__CELL_SWCACHE_ASSERT(i < 32, "VTABLE directory is full"); // fails if VTABLE directory is full
			if (__builtin_expect(spVtabDir[hash].mId == 0x3fff, 1))
			{
				// free directory slot found
				break;
			}
			if (__builtin_expect(spVtabDir[hash].mId == id, 0))
			{
				// hit on already created directory entry
				return spVtabDir[hash].mpVtab;
			}
		}

		// temporary area for vtable
		Vtab pTempVtab[MAX_SIZE_VTAB/sizeof(Vtab)] __attribute__((aligned(16)));

		// register virtual function pointer
#ifdef _DEBUG
		// clear vtable so that unregistred virtual function s routed to 0x0, and stopped
		__builtin_memset(pTempVtab, 0, MAX_SIZE_VTAB);

		// stops after benaching to 0x0, so SPU interrupt cannot be used
		*((uint32_t *)0) = 0x28000000; /* stopd 0,0,0 */
#endif
		unsigned size_of_vtab = 0;
		va_list ap;
		va_start(ap, size_of_class);
		for(;;) {
			void (*funcPtr)() = va_arg(ap, void (*)());
			if (!funcPtr) break;
			uintptr_t ofst = va_arg(ap, uintptr_t);
			size_of_vtab  = max(((ofst&~3) + sizeof(void (*)()) + 15)&~15, size_of_vtab);
			pTempVtab[ofst>>2] = (uintptr_t)funcPtr;
		}
		va_end(ap);

		// allocate area for VTABLE
		VtableHeader *pVtabHeader = (VtableHeader *)malloc(sizeof(VtableHeader) + size_of_vtab);
		__CELL_SWCACHE_ASSERT(pVtabHeader, "Cannot allocate heap for VTABLE(size=%u)", sizeof(VtableHeader) + size_of_vtab);
		// fill VTABLE header
		pVtabHeader->mMagicWord   = __VTABLE_MAGIC_WORD__;
		pVtabHeader->mId          = id;
		pVtabHeader->mSizeOfClass = size_of_class;
		pVtabHeader->mpOrgVtab    = pOrgVtab;
		Vtab *pVtab = (Vtab *)(pVtabHeader+1);
		// copy VTABLE from temporary area
		qword *__restrict__ pSrc = (qword *__restrict__)(uintptr_t)pTempVtab;
		qword *__restrict__ pDst = (qword *__restrict__)(uintptr_t)pVtab;
		for(unsigned count = size_of_vtab/sizeof(qword); count; count--)
		{
			*pDst++ = *pSrc++;
		}

		// set directory
		spVtabDir[hash].mpVtab = pVtab;
		spVtabDir[hash].mId    = id;
		spVtabDir[hash].mSize  = size_of_class;
		__CELL_SWCACHE_DPRINTF_3_OUT(" FakeVtab = %p, size_of_class=%d", pVtab, size_of_class);

		return pVtab;
	}

	/*! @brief Patch VTABLE pointer
	@param[in]       Pointer to class
	@retval CELL_SWCACHE_ERROR_NULL_POINTER Class pointer or VTABLE pointer is NULL
	@retval CELL_SWCACHE_ERROR_ALIGN        Class pointer is not 4bytes aligned
	@retval CELL_SWCACHE_ERROR_STAT         VTABLE pointer cannot be patched
	*/
	int patchVtab(Vtab **pClass)
	{
		if (__builtin_expect(!pClass, 0)) return CELL_SWCACHE_ERROR_NULL_POINTER;
		else if (__builtin_expect((uintptr_t)pClass%4, 0)) return CELL_SWCACHE_ERROR_ALIGN;
		Vtab *pVtab = *pClass;
		if (__builtin_expect(!pVtab, 0)) return CELL_SWCACHE_ERROR_NULL_POINTER;
		__CELL_SWCACHE_DPRINTF_2_IN("%s: pClass=%p", __PRETTY_FUNCTION__, pClass);
		Vtab2id *pInfo = queryVtab((const Vtab *)((uintptr_t)pVtab - 8));
		int ret = CELL_OK;
		if (__builtin_expect(pInfo != 0, 0))
		{
			// id found on table
			__CELL_SWCACHE_DPRINTF_1(" Call user's callback with id = %d, pVtab=%p", (unsigned)pInfo->mId, pVtab);
			*pClass = (*getFpBuildVtable())(pInfo->mId, pVtab);
		} else if (((VtableHeader *)(uintptr_t)*pClass-1)->mMagicWord != __VTABLE_MAGIC_WORD__) {
			ret = CELL_SWCACHE_ERROR_STAT;
		}
		if (!ret)
		{
			__CELL_SWCACHE_DPRINTF_2_OUT(" patch result *pClass = %p", *pClass);
		} else {
			__CELL_SWCACHE_DPRINTF_2_OUT(" patch failed %#x", ret);
		}
		return ret;
	}
#endif /* #ifdef __SPU__ */
protected:
	/*! @brief Search for VTABLE in table
	@param[in] pVtable        Pointer to VTABLE
	@param[in] pV2iTab        Pointer to VTABLE pointer -> Class id table
	@param[in] size           Table entry count
	@return                   The nearest VTABLE pointer value found in table.
                              This value is no less than pVtab
	*/
	Vtab2id *searchVtab(const Vtab *pVtab, Vtab2id *pV2iTab, uint32_t size)
	{
		if (size < 16)
		{
			// linear search
			while(size--)
			{
				if (pV2iTab->mpVtab >= pVtab) break;
				pV2iTab++;
			}
			return pV2iTab;
		} else {
			// binary search
			if (pV2iTab[size/2].mpVtab > pVtab)
				return searchVtab(pVtab, pV2iTab, size/2);
			else if (pV2iTab[size/2].mpVtab < pVtab)
				return searchVtab(pVtab, &pV2iTab[size/2+1], size-size/2-1);
			else
				return &pV2iTab[size/2];
		}
	}

	/*! @brief Query existence of VTABLE
	@param[in] pVtab          Pointer to VTABLE
	@retval    NULL           Specified VTABLE is not found in table
	@retval    Otherwise      Pointer to table entry
	*/
	Vtab2id *queryVtab(const Vtab *pVtab)
	{
		Vtab2id *pV2iTab = searchVtab(pVtab, (Vtab2id *)(this+1), mSize);
		return (pV2iTab->mpVtab == pVtab) ? pV2iTab : 0;
	}

	/*! @breif Add VTABLE to table. This is helper function to create table
	@param[in] pVtable        pointer to VTABLE
	@param[in] id             Class id
	*/
	void insertVtab(char pVtab[], uint16_t id)
	{
		Vtab2id *pV2iTab = &((Vtab2id *)(this + 1))[mSize - 1];
		for(unsigned count = mSize; count; count--)
		{
			__CELL_SWCACHE_ASSERT((uintptr_t)pV2iTab->mpVtab != (uintptr_t)pVtab, "%s: VTABLE@%p is already registered", __PRETTY_FUNCTION__, pVtab);
			if ((uintptr_t)pV2iTab->mpVtab < (uintptr_t)pVtab) break;
			pV2iTab[1] = pV2iTab[0]; // shift
			pV2iTab--;
		}
		// insert
		pV2iTab[1].mId    = id;
		pV2iTab[1].mpVtab = (Vtab *)(uintptr_t)pVtab;
		mSize++;
	}

	// member variables
	uint32_t mSize;   //!< Entry count in this table
	uint32_t __pad__[3];
} __attribute__((aligned(16)));

__CELL_SWCACHE_END

#endif /* __CELL_SWCACHE_PATCH_VTABLE_H__ */
