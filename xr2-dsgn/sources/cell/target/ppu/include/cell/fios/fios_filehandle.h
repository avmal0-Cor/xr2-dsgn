/*
	SCE CONFIDENTIAL
	PlayStation(R)3 Programmer Tool Runtime Library 310.001
	Copyright (C) 2009 Sony Computer Entertainment Inc.
	All Rights Reserved.
*/
/**
	\file fios_filehandle.h

	Class definitions for the fios::filehandle class.
*/
#ifndef __CELL_FIOS_FILEHANDLE_H__
#define __CELL_FIOS_FILEHANDLE_H__

#include <cell/fios/fios_types.h>
#include <cell/fios/fios_base.h>
#include <cell/fios/fios_platform.h>

namespace cell {
namespace fios {

/**
	\brief File handle class
*/
class FIOS_LINKAGE filehandle : public object
{
protected:
#ifndef DOXYGEN_IGNORE
	/** Constructor is protected; don't instantiate this object directly.
		\param[in] pPathBuffer    Buffer to use when FIOS is configured for static paths.
		\see scheduler::openFile, scheduler::openFileSync */
	filehandle(char *pPathBuffer);
public:
	/** \brief Destructor. */
	~filehandle();
#endif // DOXYGEN_IGNORE

	/** \brief Returns the scheduler associated with this filehandle.
		\return Scheduler object */
	inline scheduler * getScheduler() const { return m_pScheduler; }
	
#ifndef DOXYGEN_IGNORE
	/** \brief Returns the media filehandle associated with this object.
		\return Media filehandle */
	inline const mediafd & getMediaFilehandle() const { return m_mediaFD; }
#endif // DOXYGEN_IGNORE
	
	/** \brief Returns the file's flags.
		\return Flags
		\see e_OPENFLAGS
	*/
	inline uint32_t getFlags() const { return m_flags; }

	/** \brief Seeks within a file.

		This call does not actually seek the media filehandle; it simply
		updates the offset within the object.
		\param[in]  offset  New offset
		\param[in]  whence  Where to seek from
		\param[out] pResult Final offset (may be NULL)
		\syncreturn
	*/
	err_t seek(
		off_t offset,
		whence_t whence = kSEEK_SET,
		off_t *pResult = NULL);
	
	/** \brief Gets the file's offset
		\return Offset */
	off_t getOffset() const;
	
	/** \brief Gets the file's size
		\return Size in bytes */
	inline off_t getFileSize() const { return m_fileSize; }
	
	/** \brief Gets the file's path
		\return C-string representing the file path */
	inline const char * getPath() const { return m_path; }
	
#ifndef DOXYGEN_IGNORE
	/** \brief Placement new.
		\param[in] size         Size to allocate.
		\param[in] pPlacement   Placement.
		\return Returns pPlacement.
	*/
	static void * operator new(size_t size, void *pPlacement) { FIOS_UNUSED(size); return pPlacement; }
	
	/** \brief Placement delete.
		\param[in] pMem         Pointer to deallocate.
		\param[in] pPlacement   Placement.
	*/
	static void operator delete(void *pMem, void *pPlacement) { FIOS_UNUSED(pMem); FIOS_UNUSED(pPlacement); }
	
private:
	friend class scheduler;            /**< This class is manipulated directly by the scheduler class. */
	friend class collections::list<filehandle*>;       /**< The list class needs our next ptr. */
	friend class collections::atomicList<filehandle*>; /**< The atomic list class needs our next ptr. */
	scheduler * m_pScheduler;          /**< Scheduler associated with this filehandle */
	filehandle * m_pNext;              /**< Next pointer for queuing */
	mediafd     m_mediaFD;             /**< Media filehandle; if equal to mediafd::kINVALID_FILEHANDLE then this is a direct-from-disk filehandle */
	path_t      m_path;                /**< Path used to open this file */
	off_t       m_diskOffset;          /**< Byte offset from start of disk */
	off_t       m_fileOffset;          /**< Offset within file */
	off_t       m_fileSize;            /**< Size of the file */
	uint32_t    m_flags;               /**< Filehandle flags; see e_OPENFLAGS */
	uint32_t    m_pendingRequests;     /**< Number of pending mediarequests. The filehandle is not closed as long as this is non-zero. */

	/** \brief Gets the file's byte address on disk.
		\return Byte offset from start of disk.
	*/
	off_t getDiskOffset() const;

#endif // DOXYGEN_IGNORE
	
}; /* class filehandle */
}; /* namespace fios */
}; /* namespace cell */

#endif /* __CELL_FIOS_FILEHANDLE_H__ */
