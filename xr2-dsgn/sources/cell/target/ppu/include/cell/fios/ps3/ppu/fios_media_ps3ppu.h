/*
	SCE CONFIDENTIAL
	PlayStation(R)3 Programmer Tool Runtime Library 320.001
	Copyright (C) 2009 Sony Computer Entertainment Inc.
	All Rights Reserved.
*/
/**
	\file fios_media_ps3ppu.h

	Class definitions for the fios::ps3media class.
*/
#ifndef __CELL_FIOS_PS3PPU_MEDIA_H__
#define __CELL_FIOS_PS3PPU_MEDIA_H__

#include <cell/fios/fios_types.h>
#include <cell/fios/fios_platform.h>
#include <cell/cell_fs.h>
#include <sys/paths.h>
#if CELL_SDK_VERSION >= 0x180000
#include <np/drm_inline.h>
#endif

namespace cell {
namespace fios {

/* \internal
	The header file containing SYS_DEV_PS2DISC is not normally distributed, so
	hard code its value here. */
#define FIOS_SYS_DEV_PS2DISC	"/dev_ps2disc"
#define FIOS_SYS_DEV_PS2DISC1	"/dev_ps2disc1"

#ifndef DOXYGEN_IGNORE
/* \internal
	Number of filehandles that PS3 supports. */
#define FIOS_PS3MAXFILES    256
#endif // DOXYGEN_IGNORE

/**
	\brief Media class for generic PS3 drives.

	This class is the lowest common denominator and works for all types of
	media -- hard disks, Blu-ray, and network drives.
*/
class FIOS_LINKAGE ps3media : public media
{
public:
	FIOS_OBJECT_NEW_AND_DELETE(ps3media,kMemPurposeFIOSMediaObject|kPoolIDLongTerm)

	/** \brief Constructor from a PS3 filesystem path.

		The path should point to a directory in the PS3 filesystem tree. Common values include:
		
		* SYS_APP_HOME (default)<br>
		* SYS_HOST_ROOT<br>
		* SYS_DEV_BDVD "/PS3_GAME/USRDIR"<br>
		* a path on HDD0, received by callback from cellGameDataCheckCreate2()<br>
		* a path on HDD0, received by callback from cellHddGameCheck()<br>
		* a path on HDD1, received from cellSysCacheMount()<br>
		
		\param pPath    Path to use as the media root.
	*/
	ps3media(
		const char *pPath = SYS_APP_HOME);
	
	/** Destructor. */
	virtual ~ps3media();

#ifndef DOXYGEN_IGNORE
	/**
		\brief Returns the debugging name of a variable by its index.
		\param[in] index    Index of the variable to query.
		\return Name of the variable as a C-string, or NULL if the index is unknown.
	*/
	virtual const char *getVariableName(size_t index) const;

	/**
		\brief Returns a unique identifier for the currently loaded media.
		\see media::readIdentifier
		\return A unique identifier for the media, or 0 if no media is present.
	*/
	virtual uint64_t readIdentifier();
	
	/** \brief   Temporarily suspends a media.

		A media may be suspended to stop all I/O temporarily. This call increments
		the media's suspend count; while the suspend count is non-zero any new media
		requests will be blocked, and will not resume until the media's suspend count
		reaches zero. This interface is only useful when mixing non-FIOS requests (such
		as cellFsRead) with FIOS ones, where the non-FIOS requests have higher priority.
		Suspending one media can end up blocking requests to other FIOS media.
		
		\param[in] pWhy    Optional string which can be used to indicate why the media is being suspended. (may be NULL)
		\see ps3media::getSuspendCount(), ps3media::resume()
	*/
	void suspend(const char *pWhy = NULL);

	/** \brief   Returns the media's current suspend count.

		The suspend count indicates how many times the media has been suspended.
		Upon creation a media's suspend count will be zero. This value is only
		modified by calls to ps3media::suspend() and ps3media::resume(), which
		should be equally balanced.
		
		\return              The current suspend count.
	*/
	inline uint32_t getSuspendCount() const { return m_suspendCount; }
	
	/** \brief   Checks to see whether a media is suspended.
		\return  True if the media is suspended, false otherwise.
	*/
	inline bool isSuspended() const { return (getSuspendCount() > 0); }
	
	/** \brief   Resumes a media after suspension.

		This call decrements the media's suspend count.  If the suspend count reaches
		zero as a result, the media is resumed. If the suspend count is already
		zero, this call has no effect. If there are any pending media requests, the
		media in order for scheduler::destroyScheduler() to return.
		\param[in] pWhy     Optional string which can be used to indicate why the media was suspended. (may be NULL)
	*/
	void resume(const char *pWhy = NULL);
	
#endif // DOXYGEN_IGNORE

protected:
#ifndef DOXYGEN_IGNORE
	/** \internal
	    @{ */
	platform::mutex m_dirLock;       //!< Lock protecting kMEDIAACTION_GETDIRENTRIES.
	int m_dirFD;                     //!< Directory filehandle for #kMEDIAACTION_GETDIRENTRIES, or -1 if not open.
	off_t m_dirOffset;               //!< How many entries have been retrieved from the directory so far.
	platform::rwlock m_fdLock;       //!< Lock to protect internal fd state when we are doing an lseek/read. Will be obsolete once we get a pread API.
	platform::mutex m_suspendLock;   //!< lock for suspend
	platform::cond m_suspendCond;    //!< if suspended, sleep on this condition
	platform::atomicU32 m_suspendCount; //!< If > 0, media is suspended.
	CellFsDirent m_dirent;           //!< Most recent directory entry retrieved.
	char m_dirPath[FIOS_PATH_MAX];   //!< Path of the open directory filehandle.
	char m_mountPath[256];           //!< Mount path for the media.
	bool m_isHDD;					 //!< True if accessing HDD0 or HDD1.
	
	/* Stuff for handling fd indirection */
	static platform::nativefd_t allocateIndirectFD();
	static void deallocateIndirectFD(platform::nativefd_t pFD);
	static platform::nativefd_t findMselfFD(int mselfFD);
	static void deallocateIndirectFDLock();
	friend void FIOSTerminate();

	/** Called by worker threads to execute I/O.
	    \param[in,out]  pIOP   Request to dispatch. */
	void executeIOP(mediaioparams *pIOP);

	/** Converts an incoming path to a filename.
		\param[in]  pPath      User path.
		\param[out] pFilename  PS3 path. Buffer should be at least CELL_FS_MAX_MP_LENGTH+CELL_FS_MAX_FS_PATH_LENGTH+1 chars.
		\return Returns pFilename.
	*/
	char * mediaPathToPS3Path(
		const char *pPath,
		char * pFilename) const;
	
	/** Converts FIOS open flags to PS3 flags
		\param[in]  fiosOpenFlags    FIOS open flags
		\return Returns PS3 open flags.
	*/
	static int getOpenFlags(int fiosOpenFlags);

	/** Converts a PS3 filesystem error code to a FIOS error code.
	    \param[in]  err        Error to convert. Defaults to errno.
		\return A FIOS error code.
	*/
	err_t ps3ErrorToFIOSError(
		CellFsErrno err = errno);

        /** Closes the filehandle used by readdir. */
	void closeReaddirFD();

	/** Implementation of getdirentries.
		\param[in,out]  pIOP        Request for kMEDIAACTION_GETDIRENTRIES.
		\param[in]      pFilename   Native PS3 filename. (not a media path!)
		\return True if a retry is needed, false if the operation is complete.
	*/
	bool getDirEntriesImp(mediaioparams *pIOP, char *pFilename);
	
	/** Check on the media for possible disc ejection, and handle error recovery if needed.
		\param[in]      pIOP        pIOP to check/handle cancelled ops
		\param[in]      filename    Buffer to hold native filenames.
	*/
	bool checkOnMedia(mediaioparams *pIOP, char filename[]);

	/** Reopen the file, return 0 to continue, else -1
		\param[in]      pFD         pFD to be reopened
		\param[in]      filename    Buffer to hold native filenames.
	*/
	int reOpen(platform::nativefd_t pFD, char filename[]);
#endif // DOXYGEN_IGNORE
	
	/*@}*/
};


#ifndef DOXYGEN_IGNORE
/** The genericmedia class is a convenience type, which is typedefed to the simplest media access layer on each platform. */
typedef ps3media genericmedia;
#endif // DOXYGEN_IGNORE

}; /* namespace fios */
}; /* namespace cell */

#endif /* __CELL_FIOS_PS3PPU_MEDIA_H__ */
