/*
	SCE CONFIDENTIAL
	PlayStation(R)3 Programmer Tool Runtime Library 310.001
	Copyright (C) 2009 Sony Computer Entertainment Inc.
	All Rights Reserved.
*/
/**
	\internal
	\file fios_base.h

	Base class for all FIOS classes.
*/

#ifndef __CELL_FIOS_BASE_H__
#define __CELL_FIOS_BASE_H__

#include <cell/fios/fios_types.h>

namespace cell {
namespace fios {

/**
	\internal
	\brief Base class for most FIOS objects.
	This class provides shared functionality for most of the major classes in FIOS. Currently
	it provides new and delete operators, and some basic validity checking in debug builds.
*/
class FIOS_LINKAGE object
{
public:
	/** \brief Standard new. Overridden to use FIOS's memory allocator.
		\param numBytes Number of bytes to allocate.
		\param memPurpose Memory purpose from fios_memory.h, passed to allocator
                \param alignment Alignment.
		\return Allocated pointer.
	*/
	static void * operator new(size_t numBytes, uint32_t memPurpose, uint32_t alignment);
	
	/** \brief Standard delete. Overridden to use FIOS's memory allocator.
		\param pPtr Pointer to deallocate.
		\param memPurpose Memory purpose from fios_memory.h, passed to allocator
                \param alignment Alignment.
	*/
	static void operator delete(void *pPtr, uint32_t memPurpose, uint32_t alignment);
	
	/** \brief Array new. Overridden to use FIOS's memory allocator.
		\param numBytes Number of bytes to allocate.
		\param memPurpose Memory purpose from fios_memory.h, passed to allocator
                \param alignment Alignment.
		\return Allocated pointer.
	*/
	static void * operator new[](size_t numBytes, uint32_t memPurpose, uint32_t alignment);
	
	/** \brief Array delete. Overridden to use FIOS's memory allocator.
		\param pPtr Pointer to deallocate.
		\param memPurpose Memory purpose from fios_memory.h, passed to allocator
                \param alignment Alignment.
	*/
	static void operator delete[](void *pPtr, uint32_t memPurpose, uint32_t alignment);

	/** \brief Placement new.
		\param numBytes Number of bytes to allocate.
		\param pPlacement Placement.
		\return Allocated pointer, which is the same as the pPlacement parameter.
	*/
	inline static void * operator new(size_t numBytes, void *pPlacement) { FIOS_UNUSED(numBytes); return pPlacement; }

	/** \brief Placement delete, required by some compilers to match placement new.
		\param pPtr Pointer to deallocate.
		\param pPlacement Placement.
	*/
	inline static void operator delete(void *pPtr, void *pPlacement) { FIOS_UNUSED(pPtr); FIOS_UNUSED(pPlacement); }
	
public:

#ifndef DOXYGEN_IGNORE
	/** \internal
		\brief Constructor. Sets a check value in debug builds. */
	object();
	/** \internal
	    Destructor. Clears the check value in debug builds. */
	~object();
	/** \internal
		\brief Check value */
	uint32_t m_checkValue;
	/** \internal
		\brief Class ID, visible in debugger */
	uint32_t m_classID;

	/** \internal */
	/*@{*/
	enum e_CLASSIDS
	{
		kGeneric = 0x6F626A20,      // 'obj '
		kOp = 0x6F702020,           // 'op  ',
		kFilehandle = 0x66682020,   // 'fh  ',
		kScheduler = 0x73636864,    // 'schd',
		kMedia = 0x6D646961,        // 'mdia',
		kCacheMedia = 0x63616368,   // 'cach',
		kThread = 0x74687264,       // 'thrd',
		kMutex = 0x6D757478,        // 'mutx',
		kCond = 0x636F6E64,         // 'cond',
		kRWLock = 0x72776C6B        // 'rwlk' 
	};
	/*@}*/
#endif

}; /* class object */


/**
   \internal
   \def FIOS_OBJECT_NEW_AND_DELETE(type,purpose)
   \brief Macro used by FIOS's classes to implement placement new/delete, and to redirect standard new/delete calls to the FIOS allocator. */
#define FIOS_OBJECT_NEW_AND_DELETE(type,purpose) \
	inline static void * operator new(size_t numBytes) { return cell::fios::object::operator new(numBytes,purpose,FIOS_ALIGNOF(type)); } \
	inline static void operator delete(void *pPtr) { cell::fios::object::operator delete(pPtr,purpose,FIOS_ALIGNOF(type)); } \
	inline static void * operator new[](size_t numBytes) { return cell::fios::object::operator new[](numBytes,purpose,FIOS_ALIGNOF(type)); } \
	inline static void operator delete[](void *pPtr) { cell::fios::object::operator delete[](pPtr,purpose,FIOS_ALIGNOF(type)); } \
	inline static void * operator new(size_t numBytes, uint32_t memPurpose) { return cell::fios::object::operator new(numBytes,memPurpose,FIOS_ALIGNOF(type)); } \
	inline static void operator delete(void *pPtr, uint32_t memPurpose) { cell::fios::object::operator delete(pPtr,memPurpose,FIOS_ALIGNOF(type)); } \
	inline static void * operator new[](size_t numBytes, uint32_t memPurpose) { return cell::fios::object::operator new[](numBytes,memPurpose,FIOS_ALIGNOF(type)); } \
	inline static void operator delete[](void *pPtr, uint32_t memPurpose) { cell::fios::object::operator delete[](pPtr,memPurpose,FIOS_ALIGNOF(type)); } \
	inline static void * operator new(size_t numBytes, void *pPlacement) { FIOS_UNUSED(numBytes); return pPlacement; } \
	inline static void operator delete(void *pPtr, void *pPlacement) { FIOS_UNUSED(pPtr); FIOS_UNUSED(pPlacement); }



}; /* namespace fios */
}; /* namespace cell */
#endif /* __CELL_FIOS_BASE_H__ */

