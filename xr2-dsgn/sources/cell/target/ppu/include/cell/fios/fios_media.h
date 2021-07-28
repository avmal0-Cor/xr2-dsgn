/*
	SCE CONFIDENTIAL
	PlayStation(R)3 Programmer Tool Runtime Library 310.001
	Copyright (C) 2009 Sony Computer Entertainment Inc.
	All Rights Reserved.
*/
/**
	\internal
	\file fios_media.h

	Class definitions for the fios::media class.
*/
#ifndef __CELL_FIOS_MEDIA_H__
#define __CELL_FIOS_MEDIA_H__

#include <cell/fios/fios_types.h>
#include <cell/fios/fios_base.h>
#include <cell/fios/fios_platform.h>
#include <string.h>

namespace cell {
namespace fios {

#ifndef DOXYGEN_IGNORE
/**
	\internal
	Media actions
	\see mediaioparams
*/
enum e_MEDIAACTION
{
	// Startup queries; normally only issued once
	kMEDIAACTION_INIT,                 //!< get parameters: pBuffer is a pointer to struct mediacapabilities.
	kMEDIAACTION_INITUNKNOWNS,         //!< initialize unknowns to decent guesses. pBuffer is a pointer to an array of abstime_t[numCoefficients].
	kMEDIAACTION_TERM,                 //!< we're closing down. Clean up.
	
	// Normal operations; issued repeatedly
	kMEDIAACTION_READMEDIASTATE,       //!< Media is queried for state, and pMediaState is filled in with the result.
	
	// Block operations: optional
	kMEDIAACTION_PATHTOBLOCKS,           //!< pPath => file, offset <= starting block, actCount = size in bytes (not necessarily block multiple)
	kMEDIAACTION_GETDIRENTRIES,          //!< pPath => file or directory, offset => child index to start from, pBuffer => pointer filled in with a single direntry_t
	kMEDIAACTION_READBLOCKS,             //!< offset => starting byte from beginning of disk, pBuffer => destination, reqCount => size in bytes, actCount <= bytes actually read
	kMEDIAACTION_WRITEBLOCKS,            //!< offset => starting byte from beginning of disk, pBuffer => destination, reqCount => size in bytes, actCount <= bytes actually written
	
	// Path operations: the preferred API
	kMEDIAACTION_STAT,                   //!< pPath => file, flags <= stat flags from #e_STATFLAGS, reqCount => sizeof(stat_t), actCount <= sizeof(stat_t), pBuffer <= pointer filled in with stat_t
	kMEDIAACTION_OPEN,                   //!< pPath => file, flags => FIOS open flags, mediaFD <= new media-specific filehandle, actCount <= size in bytes
	kMEDIAACTION_READ,                   //!< pPath => file, mediaFD => media-specific filehandle to read or kINVALID_FILEHANDLE to attempt a direct read, pBuffer => destination, offset => offset, reqCount => size in bytes, actCount <= bytes actually read
	kMEDIAACTION_WRITE,                  //!< pPath => file, mediaFD => media-specific filehandle to write or kINVALID_FILEHANDLE to attempt a direct read, pBuffer => data to write, offset => offset, reqCount => size in bytes, actCount <= bytes actually written
	kMEDIAACTION_SYNC,                   //!< pPath => file, mediaFD => media-specific filehandle to sync
	kMEDIAACTION_CLOSE,                  //!< pPath => file, mediaFD => media-specific filehandle to close
	kMEDIAACTION_CREATEDIRECTORY,        //!< pPath => directory to create
	kMEDIAACTION_UNLINK,                 //!< pPath => file or directory to delete
	kMEDIAACTION_RESOLVE,                //!< pPath <=> file or directory to resolve, offset <=> offset to resolve, len <=> length to resolve, pBuffer => media to resolve to or NULL

	// Cache operations: optional, usually only implemented by cache layers
	kMEDIAACTION_PREFETCH,                //!< pPath => file, offset => offset, reqCount => size in bytes or FIOS_OFF_T_MAX

	// Object loading actions (for DRM object files)
	kMEDIAACTION_LOADSPU,                //!< Load a SPU SELF file
	kMEDIAACTION_LOADPRX,                //!< Load a SPRX file
}; /* enum e_MEDIAACTION */
#endif // DOXYGEN_IGNORE

/**
	\internal
	\brief Structure describing the media.
	\see media::getCapabilities()
*/
struct mediacapabilities
{
	uint32_t blockIO : 1;          /**< [out] Set to 1 if the media object supports block I/O. */
	uint32_t pathIO : 1;           /**< [out] Set to 1 if the media object supports path I/O. */
	uint32_t write : 1;            /**< [out] Set to 1 if the media object supports kMEDIAACTION_WRITE. */
	uint32_t getdirentries : 1;    /**< [out] Set to 1 if the media object supports kMEDIAACTION_GETDIRENTRIES. */
	uint32_t sync : 1;             /**< [out] Set to 1 if the media object supports kMEDIAACTION_SYNC. */
	uint32_t unused : 27;          /**< [out] Unused bits should be cleared to zero. */
	
	off_t blockCount;         /**< [out] Number of blocks on the media. Only valid when blockIO = 1. */
	size_t blockSize;         /**< [out] Size of one block on the media. Only valid when blockIO = 1. */
	size_t minBlockRead;      /**< [out] Minimum read size in blocks. Usually 0. Only valid when blockIO = 1. */
	size_t maxBlockRead;      /**< [out] Maximum read size in blocks. Might be SIZE_MAX. Only valid when blockIO = 1. */
	size_t zeroCostBlockRead; /**< [out] Smallest efficient read size in blocks. For example, if it's just about as fast to read 16 blocks as it is 1 block, then this value should be 16. Only valid when blockIO = 1. */
	
	size_t mediaStateSize;       /**< [out] Size of a media state. */
	size_t numberOfCoefficients; /**< [out] Number of PCE coefficients. */
	size_t maxConcurrentOps;     /**< [out] Maximum number of concurrent operations that can be run effectively. (ie, there is no substantial benefit to running more operations than this at a time) */
}; /* struct mediacapabilities */

/** \internal Initializer which can be used to set a #cell::fios::mediacapabilities to all zeroes. */
#define FIOS_MEDIACAPABILITIES_INITIALIZER     { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }


#ifndef DOXYGEN_IGNORE
/**
	\internal
	\brief Structure fully describing an I/O request.

	This is normally of interest only if you are writing your own mediafilter.
*/
class mediarequest
{
public:
	mediaaction_t what;                     /**< [in]     Used to indicate the type of request. */
	volatile err_t err;                     /**< [out]    Error from the operation */
	pathreference_t pPath;                  /**< [in]     Pathname (path I/O) or NULL (block I/O) */
	mediafd mediaFD;                        /**< [in,out] Filehandle (path I/O, or open of file in a PSARC-MSELF) or NULL (block I/O) */
	off_t offset;                           /**< [in]     Byte offset in file (path I/O) or byte offset from start of disk (block I/O) */
	off_t mselfOffset;                      /**< [in]     On open, if mediaFD set offset from the start of PSARC-MSELF */
	off_t reqCount;                         /**< [in]     Request size in bytes */
	off_t actCount;                         /**< [out]    Actual transfer size in bytes */
	void *pBuffer;                          /**< [in]     Data buffer for the I/O */
	ioflags_t ioflags;                      /**< [in]     Flags for this I/O */
	
#endif

public:
	inline bool isDataTransfer() const {
		return ((what == kMEDIAACTION_READ) || (what == kMEDIAACTION_WRITE) ||
				 (what == kMEDIAACTION_READBLOCKS) || (what == kMEDIAACTION_WRITEBLOCKS) ||
				 (what == kMEDIAACTION_PREFETCH));
	}
	inline bool isRead() const {
		return ((what == kMEDIAACTION_READ) || (what == kMEDIAACTION_READBLOCKS));
	}
	inline bool isWrite() const {
		return ((what == kMEDIAACTION_WRITE) || (what == kMEDIAACTION_WRITEBLOCKS));
	}
	inline bool sameFile(const mediarequest &other) const {
		return (((mediaFD != mediafd::kINVALID_FILEHANDLE) && (mediaFD == other.mediaFD)) ||
				(((const char *)pPath != NULL) && ((const char *)other.pPath != NULL) && !pathcmp(pPath,other.pPath)));
	}
	inline bool sameFilehandle(const mediarequest &other) const {
		return ((mediaFD != mediafd::kINVALID_FILEHANDLE) && (mediaFD == other.mediaFD));
	}
	inline bool overlaps(const mediarequest &other) const {
		// If it's file I/O then the two requests must be to the same file.
		if (((what == kMEDIAACTION_READ) || (what == kMEDIAACTION_WRITE)) && !sameFile(other))
			return false;
		// Same file or it's block I/O. Check for overlap condition. 
		//return !(((offset+reqCount) <= other.offset) || ((other.offset+other.reqCount) <= offset));   // !(a|b)
		return (((offset+reqCount) > other.offset) && ((other.offset+other.reqCount) > offset));        // !a & !b
	}

}; /* class mediarequest */

#ifndef DOXYGEN_IGNORE
/** \internal
	Initializer which can be used to set a #cell::fios::mediarequest to all zeroes. */
#define FIOS_MEDIAREQUEST_INITIALIZER     { 0, 0, 0, FIOS_MEDIAFD_INITIALIZER, 0, 0, 0, 0, 0, 0 }


/**
	\internal
	\brief Internal structure used to actually or speculatively execute I/O.

	This is normally of interest only if you are writing your own mediafilter.
*/
struct mediaioparams
{
	struct mediaioparams * FIOS_RESTRICT m_pNext;   /**<          Used for queuing requests by media objects. */
	platform::thread * FIOS_RESTRICT pThread;       /**<          Used by caller to store thread, if any */
	op * FIOS_RESTRICT pOp;                         /**<          Used by caller to store op, if any */
	abstime_t startTime;                            /**<          Used by caller to store start time */
	abstime_t stopTime;                             /**<          Used by caller to store completion time */
	bool readyToDispatch;                           /**<          Used by caller for dispatch logic */
	mediarequest * FIOS_RESTRICT pRequest;          /**< [in]     Pointer to the request */
	ioflags_t additionalFlags;                      /**< [in]     Additional kIOF flags */
	uint32_t issueNumber;                                /**< [in]     Issue order */
	void * FIOS_RESTRICT pMediaState;               /**< [in,out] Media state */
	pcecoefficient_t * FIOS_RESTRICT pCoefficients; /**< [out]    PCE coefficients describing the cost of this operation from this media state. */	
	mediacallback_proc pCallback;                   /**< [in]     Called when I/O is complete with #kMEDIAEVENT_IOCOMPLETE */
	void * FIOS_RESTRICT pCallbackContext;          /**< [in]     Context for callback */
}; /* struct mediaioparams */

/** \internal
	Initializer which can be used to set a #cell::fios::mediacapabilities to all zeroes. */
#define FIOS_MEDIAIOPARAMS_INITIALIZER     { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
#endif

/**
	\internal
	\brief Abstract base class describing the interface to media objects.
*/
class FIOS_LINKAGE media : public object
{
protected:
	        media(const char *pPrimaryIdentifier);   /**< Should be instantiated by concrete subclasses only. */
public:
	virtual ~media();  /**< \brief Destructor. */
	FIOS_OBJECT_NEW_AND_DELETE(object,kMemPurposeFIOSMediaObject|kPoolIDLongTerm)
	
public:
	
	/* ------------------------------------------------------------------------ */
	/* Media iteration */
	/**
		\brief Returns a global list of all media objects.

		This returns a pointer to the head of the linked list of media objects. To
		walk the list, use nextMedia().
		
		\return A pointer to the head of the linked list of media objects.
	*/
	static media * getFirstMedia();
	
	/**
		\brief Returns the next media object in the global list.
	*/
	media * nextMedia() const;
	
	/* ------------------------------------------------------------------------ */
	/* Identifiers */
	/**
		\brief Returns the media's primary identifier string.
		
		A media object responds to at least one identifier string, and possibly more.
		This function returns the primary identifier (the most specific identifier)
		for the media: for example, if a media object responds to both "umd0" and "umd", then
		it returns "umd0" here.
		
		\return The primary media identifier string.
	*/
	const char * getPrimaryIdentifier() const;
	
	/**
		\brief Returns the list of identifiers for the media.
		
		A media object responds to at least one identifier string, and possibly more.
		This function returns all identifiers that the media object responds to. The
		primary identifier is listed first. These are pointers to internal structures
		and may become invalid if the media is deleted or identifiers are added.
		
		\param[out] pIdentifierCount  Filled in with the number of identifiers. (may be NULL)
		\param[out] pIdentifierList   Filled in with the list of identifiers. (may be NULL)
	*/
	void getIdentifiers(
		unsigned int *pIdentifierCount,
		const char * const **pIdentifierList) const;
	
	/**
		\brief Add an alias for a media object.

		A media object responds to at least one identifier string, and possibly more.
		This function adds an alias that the media object should respond to. For
		example, you may add an alias "cdrom" for a media object "cdrom0".
		
		By convention and for thread-safety, media objects should only have identifiers
		added to them during initialization.
		
		\param[in] pIdentifier   New identifier to associate with this media object.
	*/
	void addIdentifier(
		const char *pIdentifier);
	
	/**
		\brief Retrieves a media object by its identifier.

		It's possible that multiple media objects may have the same identifier. If this
		is the case, the oldest media object is returned.

		\param[in] pIdentifier    Identifier to search for.
		\return Media object that matches the identifier, or NULL.
	*/
	static media * getMediaForIdentifier(
		const char *pIdentifier);
	
	/**
		\brief Retrieves a media object from a fully-qualified path.

		On some systems, paths are often passed around as identifier:/path/to/file.
		This function extracts the identifier and finds the associated media
		object, then optionally returns the original path.
		\param[in]  pPathWithIdentifier   Fully qualified path such as identifier:/path/to/file
		\param[out] pPath                 Filled in with a pointer to the path only, eg /path/to/file. (may be NULL)
		\param[in]  separator             Separator character, normally ':'.
		\return Media object that matches the path's identifier, or NULL.
	*/
	static media * getMediaForPath(
		const char *pPathWithIdentifier,
		const char **pPath = NULL,
		char separator = ':')
		FIOS_ATTRIBUTE_GCC((__nonnull__(1)));
	
	/**
		\brief Returns this media object's associated scheduler.
	*/
	scheduler * getScheduler() const;

	/**
		\brief Sets a new associated scheduler.

		Fails silently if the media already has a scheduler associated with it.
		\param[in] pScheduler    New scheduler to associate with the media.
	*/
	void setScheduler(scheduler *pScheduler);
	
	/** \brief Returns the previous media in the stack.
		\return The previous media in the stack.
	*/
	inline media * getPreviousMediaInStack() const {
		return m_pPreviousMediaInStack;
	}
	/** \brief Returns the next media in the stack.
		\return The next media in the stack.
	*/
	inline media * getNextMediaInStack() const {
		return m_pNextMediaInStack;
	}
	
	/** \brief Sets the previous media in the stack.

		\param[in]   pNew      The new value.

		This is normally used by internal FIOS classes (such as fios::mediafilter) and you
		will probably never need it.
		\return The old previous media in the stack. */
	inline media * setPreviousMediaInStack(media *pNew) {
		media *pOld = m_pPreviousMediaInStack;
		m_pPreviousMediaInStack = pNew;
		return pOld;
	}
	
	/* ------------------------------------------------------------------------ */
	/** \brief Normalizes a media-relative path.

		This function creates a normalized representation of a path. It's used internally
		by FIOS to normalize incoming paths before they are seen by the media layers, but
		can also be called directly if you wish.
		
		Normalization includes doing the following:
		- A slash is prepended if it's not present. "foo/file.txt" becomes "/foo/file.txt".
		- Backslashes are converted to slashes. "\foo\file.txt" becomes "/foo/file.txt".
		- Redundant slashes are removed. "/foo//file.txt" becomes "/foo/file.txt".
		- Trailing slashes are removed. "/foo/bar/" becomes "/foo/bar". This happens
		   regardless of whether bar is a directory or not. The only situation where
		   a trailing slash is left in place is the root directory, which is always
		   represented by a single slash "/".
		- Dot entries are removed. "/foo/./file.txt" becomes "/foo/file.txt".
		- Dot-dot processing deletes one level from the path without checking to
		   make sure that that any directory ever existed there. For example, the path
		   "/foo/bar/../file.txt" is normalized into "/foo/file.txt" whether or not
		   the directory "/foo/bar" exists.
 		- Extra dot-dots beyond the root cause failure. For example, the path "/foo/../../../../" is invalid.
		- This function fails if the output path or any intermediate path generated during
		   construction exceeds #FIOS_PATH_MAX characters, including the terminating NULL.
		   
		So as an extreme example, a path such as "foo\\/\./bar\\\/..///file.txt" is
		normalized into "/foo/file.txt".
		
		\param[in]     pPath            Path to normalize.
		\param[out]    pNormalizedPath  Filled in with the normalized path. This buffer is normally FIOS_PATH_MAX bytes, and must be at least strlen(pPath)+2 bytes.
		\return Returns TRUE if the path was successfully normalized, or FALSE if the incoming path was illegal.
	*/
	static bool normalizePath(
		const char * FIOS_RESTRICT pPath,
		char * FIOS_RESTRICT pNormalizedPath) FIOS_ATTRIBUTE_GCC((__nonnull__));
	
#ifndef DOXYGEN_IGNORE
	/**
		\internal
		\brief Refreshes the capabilities struct of a media object.

		Issues a synchronous kMEDIAACTION_INIT to fill in the media's capabilities struct.
	*/
	void init();
	
	/**
		\internal
		\brief Tells the media object to initialize the unknowns.

		Issues a synchronous kMEDIAACTION_INITUNKNOWNS.
	*/
	void initUnknowns(abstime_t *pUnknowns);
	
	/**
		\internal
		\brief Tells the media object it's time to clean up.

		Issues a synchronous kMEDIAACTION_TERM.
	*/
	void term();
#endif

	/**
		\brief Returns the media's callback.
		\param[out] pCallbackContext     Filled in with the callback's context pointer. (may be NULL)
		\return The media's mediacallback_proc.
	*/
	mediacallback_proc getCallback(void **pCallbackContext = NULL) const;

	/**
		\brief Sets a new media callback.
		\param[in]  pNewCallback		    The new callback.
		\param[in]  pNewCallbackContext  The new callback's context pointer.
		\param[out] pOldCallback         Filled in with the old callback. (may be NULL)
		\param[out] pOldCallbackContext  Filled in with the old callback's context pointer. (may be NULL)
	*/
	void setCallback(
		mediacallback_proc pNewCallback,
		void * pNewCallbackContext,
		mediacallback_proc *pOldCallback = NULL,
		void ** pOldCallbackContext = NULL);
	
	/**
		\brief Invokes the media callback.
		\param[in]     event               Event type.
		\param[in,out] pParam              Pointer to event-specific parameters.
		\return Error returned by the callback.
	*/
	err_t callback(
		mediaevent_t event,
		void *pParam = NULL);
	
	/**
		\brief Remembers a media identifier for later use.
		\param[in] newIdentifier The media identifier to remember.
		\see getCachedIdentifier()
	*/
	void setCachedIdentifier(uint64_t newIdentifier);
	
	/**
		\brief Returns the previously-remembered media identifier.
		\see setCachedIdentifier();
	*/
	uint64_t getCachedIdentifier() const;
	
	/* ------------------------------------------------------------------------ */
	/* Media capabilities */
	/**
		\brief Returns the media's capabilities.
		\return A constant reference to a struct #cell::fios::mediacapabilities.
	*/
	const mediacapabilities & getCapabilities() const { return m_capabilities; }

	/**
		\brief Returns whether this object supports block I/O.
		\return Boolean value.
	*/
	inline bool supportsBlockIO() const { return m_capabilities.blockIO ? true:false; }

	/**
		\brief Returns whether this object supports path I/O.
		\return Boolean value.
	*/
	inline bool supportsPathIO() const { return m_capabilities.pathIO ? true:false; }
	
	/**
		\brief Returns the number of blocks on the media.
		\return Number of blocks on the media.
	*/
	inline off_t getBlockCount() const { return m_capabilities.blockCount; }
	
	/**
		\brief Returns the size of one block (sector) on the media.
		\return Size of a media sector.
	*/
	inline size_t getBlockSize() const { return m_capabilities.blockSize ? m_capabilities.blockSize:2048; }

	/**
		\brief Returns the minimum read size in blocks.
		\return Minimum read size in blocks.
	*/
	inline size_t getMinBlockRead() const { return m_capabilities.minBlockRead; }

	/**
		\brief Returns the maximum read size in blocks.
		\return Maximum read size in blocks.
	*/
	inline size_t getMaxBlockRead() const { return m_capabilities.maxBlockRead; }
	
	/**
		\brief Returns the zero-cost read size in blocks.
		\return Smallest efficient read size in blocks.
	*/
	inline size_t getZeroCostBlockRead() const { return m_capabilities.zeroCostBlockRead; }

	/**
		\brief Returns the size of a media state.
		\return Size of a media state.
	*/
	inline size_t getMediaStateSize() const { return m_capabilities.mediaStateSize; }

	/**
		\brief Returns the number of coefficients in the performance characteristic equation.
		\return Number of PCE coefficients.
	*/
	inline size_t getNumberOfCoefficients() const { return m_capabilities.numberOfCoefficients; }

	/**
		\brief Returns the maximum number of concurrent operations.
		\return Maximum number of concurrent operations that can be run efficiently.
	*/
	inline size_t getMaxConcurrentOps() const { return m_capabilities.maxConcurrentOps; }
	
#ifndef DOXYGEN_IGNORE
	/**
		\internal
		\brief Synchronously executes a single request without an IOP.
		\param[in,out] pRequest    Request to execute.
	*/
	inline void executeRequest(mediarequest *pRequest) {
		mediaioparams iop = FIOS_MEDIAIOPARAMS_INITIALIZER;
		iop.pRequest = pRequest;
		pRequest->err = CELL_FIOS_ERROR_INPROGRESS;
		executeIOP(&iop);
	}
#endif

	/* ------------------------------------------------------------------------ */
	/* Methods that may optionally be implemented by subclasses                 */
	/**
		\brief Immediately returns the predicted PCE coefficients for an I/O request.

		This function is used by the scheduler to predict I/O completion times.
		It always returns the results immediately, without blocking, and should be
		as efficient as possible.
		\param[in,out] pIOP           I/O parameters describing the request.
	*/
	virtual void speculateIOP(
		mediaioparams *pIOP)
		FIOS_ATTRIBUTE_GCC((__nonnull__))
	{
		pIOP->additionalFlags |= kIOF_SPECULATIVE;
		executeIOP(pIOP);
	}

	/* ------------------------------------------------------------------------ */
	/* Methods that must be implemented by subclasses                           */
	
	/**
		\brief Returns the debugging name of a variable by its index.
		\param[in] index    Index of the variable to query.
		\return Name of the variable as a C-string, or NULL if the index is unknown.
	*/
	virtual const char *getVariableName(size_t index) const = 0;
	
	/**
		\brief Returns a unique identifier for the currently loaded media.

		This always performs immediate I/O to identify the media. It's generally
		used during unexpected ejection events to make sure the user puts the correct
		media back into the drive.

		The identifier returned is a summary or checksum value computed
		in a media-specific way -- for example, a hash of a CD-ROM's TOC. It
		usually can't be guaranteed to be perfectly unique, but it should be unique
		enough that it's very unlikely that two pieces of media share the same
		identifier. 
		
		The identifier value of 0 is reserved to mean that no media is present.

		Subclasses should take care to watch out for the possibility that media might
		exist with a naturally computed identifier of 0, as they need to return
		a non-zero value (for example: 1).
		
		\return A unique identifier for the media, or 0 if no media is present.
	*/
	virtual uint64_t readIdentifier() = 0;
	
	/**
		\brief Synchronously executes a single I/O operation.

		This function is called by the scheduler's worker threads to execute a single I/O request. Concrete subclasses
		should satisfy the request synchronously, fill in PCE coefficients, and set the error code in
		the mediaioparams before returning.
		\param[in,out] pIOP           I/O parameters describing the operation.
	*/
	virtual void executeIOP(
		mediaioparams *pIOP)
		FIOS_ATTRIBUTE_GCC((__nonnull__)) = 0;
	
	/* ------------------------------------------------------------------------ */

protected:
	/** \internal */
	/*@{*/
	friend class mediafilter;
	static platform::atomicPtr<platform::mutex*> s_mediaListLock;  /**< Global lock for media list. */
	static media *s_pMediaList;              /**< Global list of media. */

	mutable platform::mutex m_objectLock;    /**< Object lock for accessing member variables. */
	media *m_pNext;                          /**< Pointer to next media object in global list. */
	media *m_pPreviousMediaInStack;          /**< Pointer to next-higher media object in stack. */
	media *m_pNextMediaInStack;              /**< Pointer to next-lower media object in stack. */
	scheduler *m_pScheduler;                 /**< Pointer to associated scheduler. */
	uint64_t m_cachedIdentifier;                  /**< Cached identifier for this media object. */
	
	unsigned int m_identifierListCount;      /**< Number of valid identifiers in the array */
	unsigned int m_identifierListAllocation; /**< Capacity of the identifier array */
	char **m_pIdentifierList;                /**< Identifier array */
	
	mediacallback_proc m_pCallback;          /**< Callback. */
	void * m_pCallbackContext;               /**< Callback context. */
	struct mediacapabilities m_capabilities; /**< Cached capabilities. */

	virtual void registerMedia();
	void unregisterMedia();
	static void initializeClassLock();
	static void terminateClassLock();
	friend FIOS_LINKAGE void FIOSTerminate(void);
	/*@}*/
	/*@}*/
}; /* class media */



/** \internal \brief Mix-in class for media filter layers. */
class FIOS_LINKAGE mediafilter : public media
{
protected: // Should be instantiated by concrete subclasses only
	mediafilter(const char *pPrimaryIdentifier, media *pNextMedia);
public:
	virtual ~mediafilter();
	FIOS_OBJECT_NEW_AND_DELETE(object,kMemPurposeFIOSMediaObject|kPoolIDLongTerm)

	/** \brief Registers a media object.

		The default mediafilter implementation calls through to register the bottom of the stack first,
		and then registers itself afterward.
	*/
	virtual void registerMedia() {
		if (m_pNextMediaInStack) m_pNextMediaInStack->registerMedia();
		media::registerMedia();
	}
	
	/** \brief Passes an I/O operation through to the next media in the stack.
		\param[in] pIOP   Pointer to I/O params.
	*/
	void passthruIOP(mediaioparams *pIOP) {
		// Handle kMEDIAACTION_RESOLVE automatically.
		if (FIOS_UNLIKELY(pIOP->pRequest->what == kMEDIAACTION_RESOLVE)) {
			if (FIOS_LIKELY(pIOP->pRequest->mediaFD.m_issuer == NULL) &&
				(pIOP->pRequest->mediaFD.m_ptr == this)) {
				pIOP->pRequest->err = CELL_FIOS_NOERROR;
				return;
			}
		}
		if (FIOS_LIKELY(m_pNextMediaInStack != NULL))
			m_pNextMediaInStack->executeIOP(pIOP);
	}
	/**
		\brief Returns the debugging name of a variable by its index.

		The default mediafilter implementation passes calls through to the next media in the stack.
		\param[in] index    Index of the variable to query.
		\return Name of the variable as a C-string, or NULL if the index is unknown.
		\see media::getVariableName
	*/
	virtual const char *getVariableName(size_t index) const {
		return (m_pNextMediaInStack) ? m_pNextMediaInStack->getVariableName(index):NULL;
	}
	/**
		\brief Returns a unique identifier for the currently loaded media.

		The default mediafilter implementation passes calls through to the next media in the stack.
		\return A unique identifier for the media, or 0 if no media is present.
		\see media::readIdentifier
	*/
	virtual uint64_t readIdentifier() {
		return (m_pNextMediaInStack) ? m_pNextMediaInStack->readIdentifier():0;
	}
};
}; /* namespace fios */
}; /* namespace cell */


#endif /* __CELL_FIOS_MEDIA_H__ */
