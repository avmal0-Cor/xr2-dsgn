/*
	SCE CONFIDENTIAL
	PlayStation(R)3 Programmer Tool Runtime Library 310.001
	Copyright (C) 2009 Sony Computer Entertainment Inc.
	All Rights Reserved.
*/
/**
	\file fios_types.h

	Types and enumerations for the File I/O Scheduler.
*/

#ifndef __CELL_FIOS_TYPES_H__
#define __CELL_FIOS_TYPES_H__

#include <cell/error.h>
#include <cell/fios/fios_platform_imp.h>

// Include this for the SceNpDrmKey:
#include <np/drm.h>

namespace cell {
namespace fios {

/* Forward declarations */
class scheduler;
class media;
class filehandle;
class op;

/**
	\brief Error code.
	Many operations return an error code.
	\see	e_ERRORS
*/
typedef int32_t err_t;

/**
	\brief	Errors
*/

enum e_ERRORS
{
	/*
	  CELL_ERROR_FACILITY_SYSTEM_SERVICE 0x001
	  libfios : 0x8001_0701 - 0x8001_07ff
	*/

	/** No error occurred */
	CELL_FIOS_NOERROR = CELL_OK,
	
	/** I/O is in progress */
        CELL_FIOS_ERROR_INPROGRESS = CELL_ERROR_CAST(0x80010701),
	
	/** Callback return code: event was not handled */
        CELL_FIOS_ERROR_UNKNOWNCALLBACKEVENT = CELL_ERROR_CAST(0x80010702),
	
	/** Callback return code: no buffer ready yet, try again later */
        CELL_FIOS_ERROR_NOTREADY = CELL_ERROR_CAST(0x80010703),
	
	/** Deadline reached in op::waitListDeadline() or equivalent, and I/O has not completed yet */
        CELL_FIOS_ERROR_TIMEOUT = CELL_ERROR_CAST(0x80010704),

	/** Bad path argument, or file not found */
        CELL_FIOS_ERROR_BADPATH = CELL_ERROR_CAST(0x80010705),

	/** Bad filehandle argument */
        CELL_FIOS_ERROR_BADHANDLE = CELL_ERROR_CAST(0x80010706),

	/** Bad parameter (invalid or illegal) */
        CELL_FIOS_ERROR_BADPARAM = CELL_ERROR_CAST(0x80010707),

	/** Operation was cancelled */
        CELL_FIOS_ERROR_CANCELLED = CELL_ERROR_CAST(0x80010708),

	/** End-of-file was reached during a read; some data may have been returned prior to the EOF. Check the actual bytes transferred. */
        CELL_FIOS_ERROR_EOF = CELL_ERROR_CAST(0x80010709),

	/** Out of memory. */
        CELL_FIOS_ERROR_NOMEM = CELL_ERROR_CAST(0x8001070A),

	/** Bad offset during seek */
        CELL_FIOS_ERROR_BADOFFSET = CELL_ERROR_CAST(0x8001070B),

	/** Callback was NULL when it shouldn't have been */
        CELL_FIOS_ERROR_BADCALLBACK = CELL_ERROR_CAST(0x8001070C),

	/** Some platform-specific error occurred that is not enumerated here (for example, too many open files) */
        CELL_FIOS_ERROR_PLATFORM = CELL_ERROR_CAST(0x8001070D),

	/** Access was denied, you don't have appropriate security permission */
        CELL_FIOS_ERROR_ACCESS = CELL_ERROR_CAST(0x8001070E),

	/** Decompression failed. */
        CELL_FIOS_ERROR_DECOMPRESSION = CELL_ERROR_CAST(0x8001070F),

	/** Write not supported by media or file access permissions */
        CELL_FIOS_ERROR_READONLY = CELL_ERROR_CAST(0x80010710),

	/** Block APIs not supported by media */
        CELL_FIOS_ERROR_NOBLOCK = CELL_ERROR_CAST(0x80010711),

	/** Path APIs not supported by media */
        CELL_FIOS_ERROR_NOPATH = CELL_ERROR_CAST(0x80010712),

	/** Media is gone and callback indicates it won't return. All operations are terminated with this error until the media returns. */
        CELL_FIOS_ERROR_MEDIAGONE = CELL_ERROR_CAST(0x80010713),

	/** Request had #kOPF_IMMED set but I/O was required */
        CELL_FIOS_ERROR_CANTIMMED = CELL_ERROR_CAST(0x80010714),
};

/*!
	\brief File offset or size.

	This type is parallel to the POSIX off_t, and is used for both offsets and
	sizes. It's large (normally 64 bits) so that it can represent offsets in
	files larger than 2GB, and explicitly signed so that it may contain
	negative offsets.
*/
typedef int64_t off_t;

/** \brief Maximum value that can be represented by a fios::off_t. */
#define FIOS_OFF_T_MAX       cell::fios::off_t(0x7FFFFFFFFFFFFFFFLL)

/** \brief Minimum (negative) value that can be represented by a fios::off_t. */
#define FIOS_OFF_T_MIN       cell::fios::off_t(-FIOS_OFF_T_MAX - 1)

/**
	\brief Absolute time.

	This type is a cross-platform absolute time incrementer with an arbitrary
	base and constant frequency. It's used for deadlines and timeouts. To make
	certain math calculations easier it's always a signed value with at least
	64 bits.
	
	This time format is not designed for persistence; in many implementations
	it represents the number of "ticks" of arbitrary length (often clock cycles)
	since the system booted.

	\see	FIOSTime
*/
typedef platform::abstime_t abstime_t;

/**
	\brief Calendar time.

	This type is a compact, persistent, cross-platform UTC-based calendar time with a
	nanosecond resolution. Functions are provided to convert it to and from each
	platform's common native time specifications. This format uses the Unix epoch
	of January 1, 1970 CE and reserves the high bit for future expansion. It
	will overflow near the end of the year 2262 CE.
	
	This time format is designed for persistence and cross-platform usage
	when necessary. Each value represents a fixed point in time that is
	consistent across platforms.
*/
typedef uint64_t datetime_t;

/**
	\brief Priority.

	All operations are given a priority value. When multiple operations are
	running at the same time and cannot all be completed by the deadline, the
	priority is used to resolve conflicts: higher numerical priorities are
	given precedence. For example, a priority of 10 is given precedence
	over a priority of 3.
	\see	e_PRIORITIES
*/
typedef int8_t prio_t;

#ifndef DOXYGEN_IGNORE
/**
	\brief Media event

	Enumerated value passed to the mediacallback_proc to indicate an event
	that must be handled.
	\see	e_MEDIAEVENTS
	\see	mediacallback_proc
*/
typedef uint8_t mediaevent_t;
#endif // DOXYGEN_IGNORE

/**
	\brief Operation event

	Enumerated value passed to the #opcallback_proc to indicate an event
	that may be handled.
	\see	e_OPEVENTS, opcallback_proc
*/
typedef uint8_t opevent_t;

/**
	\brief Whence value for seeks

	This type is used for seek operations on fios::filehandle.
	\see	e_WHENCE
*/
typedef uint8_t whence_t;

#ifndef DOXYGEN_IGNORE
/**
	\brief IO flags used by the media object.
	\see e_IOFLAGS
*/
typedef uint16_t ioflags_t;

/**
	\brief Identifies a statistic that can be requested from a scheduler
	\see	e_STATTYPE, scheduler::getStatistics()
*/
typedef uint8_t stattype_t;

/**
	\brief Identifies a media action.
	\see e_MEDIAACTION, media::executeIOP(), mediaioparams
*/
typedef int mediaaction_t;

/**
	\brief Coefficients of a media operation's performance characteristic equation.
	\see media::executeIOP(), mediaioparams
*/
typedef int pcecoefficient_t;
#endif // DOXYGEN_IGNORE

/**
	\brief  Priority values

	Priorities range from -128 to 127, with -128 being the lowest priority and 127 the
	highest. The choice of priorities is entirely up to the client.
	
	Remember that priority values are only used when the scheduler is unable to meet the
	deadlines requested; a lower-priority request may be serviced before a higher-priority
	request as long as both are completed by their respective deadlines.
*/
enum e_PRIORITIES
{
	kPRIO_MIN      = -128,  /**< Minimum priority */
	kPRIO_DEFAULT  = 0,     /**< Suggested default priority */
	kPRIO_MAX      = 127    /**< Maximum priority */
};

#ifndef DOXYGEN_IGNORE
/**
	\brief	Media events
	\see mediacallback_proc
*/
enum e_MEDIAEVENTS
{
	/**
		\brief Media was unexpectedly ejected or otherwise gone.

		Put up some UI indicating that the media is gone and wait
		for it to return. If a scheduler is associated with the media, it is
		suspended until the callback has returned: new I/O requests are accepted
		but none are serviced until the callback is done.
		
		You can poll the current media by calling media::readIdentifier() to see
		if the currently-inserted media is the same as the old media. You should
		not return until the old media has been reinserted.
		
		If your callback returns #CELL_FIOS_ERROR_MEDIAGONE then all operations are cancelled; if
		your callback returns #CELL_FIOS_NOERROR then the scheduler (if any) is resumed. These are
		the only defined behaviors; other return values currently have the same effect as
		#CELL_FIOS_ERROR_MEDIAGONE but this may change in the future.
	*/
	kMEDIAEVENT_GONE	= 1,
	/**
		\brief Media I/O has completed.

		Parameter is a pointer to the struct mediaioparams for the request that
		is about to be issued.  Normally for internal use only: Your callback only
		receives this message if you are calling the media driver directly and
		explicitly put your callback into the struct mediaioparams.
	*/
	kMEDIAEVENT_IOSTARTING = 2,
	/**
		\brief Media I/O has completed.

		Parameter is a pointer to the struct mediaioparams for the request that
		just completed.  Normally for internal use only: Your callback only
		receives this message if you are calling the media driver directly and
		explicitly put your callback into the struct mediaioparams.
	*/
	kMEDIAEVENT_IOCOMPLETE = 3
};

/**
	\brief IO flags for media
*/
enum e_IOFLAGS
{
	/** Set to indicate that an I/O is speculative and should not be executed. */
	kIOF_SPECULATIVE = (1<<0),
	/** Set to indicate that an I/O should not use any caches but always read or write directly to the media. */
	kIOF_DONTUSECACHE = (1<<1),
	/** Set to indicate that the data returned from an I/O should not be saved in any RAM caches. */
	kIOF_DONTFILLRAMCACHE = (1<<2),
	/** Set to indicate that the data returned from an I/O should not be saved in any disk caches. */
	kIOF_DONTFILLDISKCACHE = (1<<3),
	/** Set to indicate that the data returned from an I/O should remain *permanently* in the disk cache. */
	kIOF_CACHEPERSIST = (1<<4),
	/** Set to indicate that the buffer for this I/O is in uncached memory such as VRAM. */
	kIOF_UNCACHEDBUFFER = (1<<5),
	/** \brief Set to indicate that the buffer for this I/O is in non-DMAable memory such as the stack on PS3.
		
		On PS3, this flag is used by the #EdgeDecompressor classes. It must be set when reading from a psarc archive into
		any memory that the SPU is not allowed to touch. When it's set, a small amount of extra CPU and RAM will be used
		to double-buffer the transaction. 
	*/
	kIOF_NONDMABUFFER = (1<<6),
	/** \brief Set to indicate that this request should only be serviced if it can be done without I/O.
		Ops with this flag set will bypass scheduling entirely, but may fail wih #CELL_FIOS_ERROR_CANTIMMED. */
	kIOF_IMMED = (1<<7),
	
	/** Combined flags, used to indicate that the data returned from an I/O should not be saved in any caches. This
		is the same as specifying both #kIOF_DONTFILLRAMCACHE and #kIOF_DONTFILLDISKCACHE. */
	kIOF_DONTFILLCACHE = (kIOF_DONTFILLRAMCACHE | kIOF_DONTFILLDISKCACHE),
	/** Combined flags, used to indicate that no caching should be performed on this I/O at all. This is the
		same as specifying #kIOF_DONTUSECACHE, #kIOF_DONTFILLRAMCACHE, and #kIOF_DONTFILLDISKCACHE. */
	kIOF_NOCACHE = (kIOF_DONTUSECACHE | kIOF_DONTFILLRAMCACHE | kIOF_DONTFILLDISKCACHE),

	/** \brief Set if this requires opening an EDATA file. */
	kIOF_EDATA = (1<<12),
	/** \brief Set if this requires opening an SDATA file. */
	kIOF_SDATA = (1<<13),
	/** \internal
	 * \brief Combined DRM op flags.*/
	kIOF_DRM = (kIOF_EDATA | kIOF_SDATA),
	/** \internal
	 * \brief Set to open an MSELF file.*/
	kIOF_MSELF = (1<<15),
};

/**
	\brief Callback used by media drivers.

	This callback should block until the event is completely handled.
	\param[in] pContext    User-specified context pointer
	\param[in] media       Media object that generated this event
	\param[in] event       Event type
	\param[in,out] pParam  Event parameter (may be NULL)
	\return            Return #CELL_FIOS_NOERROR if the event was handled.
	\see	e_MEDIAEVENTS
*/
typedef err_t (*mediacallback_proc)(
	void *pContext,
	media *pMedia,
	mediaevent_t event,
	void *pParam);

/**
	\brief	Load statistic types

	These values are used as input to scheduler::getStatistics() to query the status
	of the scheduler for runtime performance monitoring. 

	\see	stattype_t, scheduler::getStatistics()
*/
enum e_FIOS_STATTYPE {
	/** \brief Number of operations
		
		This statistic is a value which indicates the number of outstanding
		operations at a time.  The current value is the number of outstanding
		operations in the queue right now, and the lifetime value is the maximum number
		of operations that have ever been in flight.
	*/
	kSTAT_NUMOPS = 1,
	
	/** \brief Number of open files

		This statistic is a value which indicates the number of open files at a time.
		The current value is the number of open files right now, and the lifetime
		value is the maximum number of files that that have ever been open.
	*/
	kSTAT_NUMFILES = 2,
	
	/** \brief Percentage load

		This statistic is a value from 0 to 100 which indicates what percentage of
		the scheduler's time over the stated period has been spent servicing I/O
		requests.  For example, if the scheduler has been running for 1000 seconds
		and has spent 600 seconds servicing I/O requests, the value returned for
		its lifetime utilization is 60 (representing 60% load).

		The current value represents the value for the past two seconds, and the
		lifetime value represents the value for the scheduler's lifetime.
	*/
	kSTAT_TOTALLOAD = 3,
	
	/** \brief Estimated percentage of the scheduler's busy time spent seeking.
		
		This statistic is a value from 0 to 100 which indicates what percentage of the
		scheduler's busy time (as returned by #kSTAT_TOTALLOAD) has been spent waiting for
		seeks, layer changes, head alignments, and any other operation that is tied
		to seeking rather than data access.

		This can be used to evaluate the performance of a disk layout.  A perfectly
		optimal disk layout has a seek load of 0 because all files are laid
		out in the exact order they are used and no seeks are ever performed.
		Higher values indicate less optimal layouts.  In practical terms, of course,
		seek load is unavoidable, but minimizing this statistic ensures an
		optimal layout.

		The sum of the statistics #kSTAT_IOLOAD + #kSTAT_SEEKLOAD should be 100.

		The current value represents the value for the past two seconds, and the
		lifetime value represents the value for the scheduler's lifetime.
	*/
	kSTAT_SEEKLOAD = 5,
	
	/** \brief Estimated percentage of the scheduler's busy time spent reading or writing data.

		This statistic is a value from 0 to 100 which indicates what percentage of the
		scheduler's busy time (as returned by #kSTAT_TOTALLOAD) has been spent on data access
		rather than seeking.

		This can be used to evaluate the performance of a disk layout. A perfectly
		optimal disk layout has an I/O load of 100 because all files are
		laid out in the exact order they are used and only data access operations
		are performed -- never any seeks.  Lower values indicate less optimal
		layouts. In practical terms, of course, an I/O load of 100 is unachievable,
		but maximizing this statistic ensures an optimal layout.

		The sum of the statistics #kSTAT_IOLOAD + #kSTAT_SEEKLOAD should be 100.

		The current value represents the value for the past two seconds, and the
		lifetime value represents the value for the scheduler's lifetime.
	*/
	kSTAT_IOLOAD = 6,
	
	/** \brief Percentage of requests that missed their deadlines.

		This statistic is a value from 0 to 100 which indicates what percentage of
		I/O requests were not completed until past their deadline.
		
		Requests issued with #kDEADLINE_NOW and #kDEADLINE_ASAP will
		essentially always "miss" their deadlines, so they are not counted as part
		of this statistic. The inverse is true for requests issued with
		#kDEADLINE_LATER; these will always meet their deadline, so they are
		not counted either.  Only requests where a valid deadline is specified are
		considered.
		
		This can be used to evaluate the effectiveness of a client's preloading
		strategy. Ideally your miss rate should be 0, and this value is normally
		achievable. A non-zero miss rate may be the source of a performance problem.
		To reduce your miss rate, schedule your requests further in advance or
		reduce the amount of I/O you have in flight.

		The current value represents the value for the past two seconds, and the
		lifetime value represents the value for the scheduler's lifetime.
	*/
	kSTAT_MISSRATE = 7
};
#endif // DOXYGEN_IGNORE

/**
	\brief Flags used in scheduler::openFile()
	\see scheduler::openFile(), scheduler::openFileSync()
*/
enum e_OPENFLAGS { /* open flags */
	kO_READ = (1<<0),              /**< Open with read privileges */
	kO_RDONLY = kO_READ,           /**< Synonym for #kO_READ, used to signify read-only */
	kO_WRITE = (1<<1),             /**< Open with write privileges */
	kO_WRONLY = kO_WRITE,          /**< Synonym for #kO_WRITE, used to signify write-only */
	kO_RDWR = kO_READ|kO_WRITE,    /**< Synonym for #kO_READ and #kO_WRITE together, used to signify read-write */
	kO_APPEND = (1<<2),            /**< Write-append-only */
	kO_CREAT = (1<<3),             /**< Create file if it doesn't exist */
	kO_TRUNC = (1<<4),             /**< Truncate file if it already exists */
	
	kO_DYNALLOC = (1<<7),          /**< Filehandle is or should be dynamically allocated. */
	kO_EDATA = (1<<8),          /**< Open an EDATA file */
	kO_SDATA = (1<<9),          /**< Open an SDATA file */
	kO_DRM = (kO_EDATA | kO_SDATA),
	kO_MSELF = (1<<11),
	// Note: change the size of ioflags_t if we go past 16 bits here
};

/**
	\brief  Whence values.
	\see    scheduler::seekFileSync(), filehandle::seek()
*/
enum e_WHENCE { /* whence */
	kSEEK_SET = 0,     /**< Absolute seek location from the beginning of file. */
	kSEEK_CUR = 1,     /**< Relative seek location, based on current position. */
	kSEEK_END = 2      /**< Relative seek location, from the end of file. */
};

/**
	\brief  Stat flags.
	\see    #kMEDIAACTION_STAT
*/
enum e_STATFLAGS { /* stat flags */
	kSTAT_DIRECTORY = (1<<0),       /**< item is a directory */
	kSTAT_READABLE = (1<<1),        /**< item is readable */
	kSTAT_WRITABLE = (1<<2)         /**< item is writable */
};


/** \brief  File or directory status
	\see    scheduler::stat() */
typedef struct stat_t {
	off_t         fileLocation;       /**< File location as a byte address from the start of the media. May be 0 if unavailable. */
	off_t         fileSize;           /**< File size in bytes. Zero for directories. */
	datetime_t    accessDate;         /**< Last time the file was read/accessed. May be 0 if not supported by the OS. */
	datetime_t    modificationDate;   /**< Last time the file was modified. May be 0 if not supported by the OS. */
	datetime_t    creationDate;       /**< Time the file was created. May be 0 if not supported by the OS. */
	uint32_t      statFlags;          /**< FIOS stat flags; see #e_STATFLAGS. */
	int64_t       nativeUID;          /**< Native POSIX UID. May be 0 if not supported by the OS. */
	int64_t       nativeGID;          /**< Native POSIX GID. May be 0 if not supported by the OS. */
	int64_t       nativeDevice;       /**< Native POSIX device number. May be 0 if not supported by the OS. */
	int64_t       nativeInode;        /**< Native POSIX inode number. May be 0 if not supported by the OS. */
	int64_t       nativeMode;         /**< Native POSIX mode. May be 0 if not supported by the OS. */
} stat_t;

/** Initializer which can be used to set a stat_t to all zeroes. */
#define FIOS_STAT_INITIALIZER    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }

/**
	\brief Used to return directory entries.
	\see #kMEDIAACTION_GETDIRENTRIES
*/
typedef struct direntry_t {
	off_t    fileLocation;            /**< File location as a byte address from the start of the media. Set to 0 if unavailable. */
	off_t    fileSize;                /**< File size in bytes. Set to 0 for directories. */
	uint32_t statFlags;               /**< Stat flags; see #e_STATFLAGS */
	uint16_t nameLength;              /**< Name length, for convenience. */
	uint16_t fullPathLength;          /**< Full path length, for convenience. */
	uint16_t offsetToName;            /**< Offset from start of full path to start of filename. */
	char     fullPath[FIOS_PATH_MAX]; /**< Full pathname. */
} direntry_t;

/** Initializer which can be used to set a direntry_t to all zeroes. */
#define FIOS_DIRENTRY_INITIALIZER      { 0, 0, 0, 0, 0, 0, "" }

/** \brief Used to contain a (path,offset,length) tuple.
	\see scheduler::resolve(), scheduler::resolveSync() */
typedef struct filerange_t {
	char      path[FIOS_PATH_MAX]; /**< File path */
	off_t     offset;              /**< Offset within a file. Set to 0 if you don't care. */
	off_t     length;              /**< Length from the offset. Set to #FIOS_OFF_T_MAX if you don't care. */
} filerange_t;

#ifndef DOXYGEN_IGNORE
/**
	\internal
	\brief Media file descriptor

	Each media object may have its own notion of a file descriptor. For example, the
	dearchiving layer may need a special fake file descriptor for items within an
	archive that references the containing archive. So this class encapsulates
	a media-layer-specific filehandle.

	This class is normally only useful if you are writing your own mediafilter.
*/
class FIOS_LINKAGE mediafd
#endif

{
public:
	inline mediafd() { reset(); }
	inline mediafd(const mediafd &other) { m_issuer = other.m_issuer; m_ptr = other.m_ptr; m_ptr2 = other.m_ptr2; m_int = other.m_int; m_nativeFD = other.m_nativeFD; }
	inline ~mediafd() {}
	
	inline mediafd & operator = (const mediafd &other) { m_issuer = other.m_issuer; m_ptr = other.m_ptr; m_ptr2 = other.m_ptr2; m_int = other.m_int; m_nativeFD = other.m_nativeFD; return *this; }
	inline bool operator == (const mediafd &other) const { return (m_issuer == other.m_issuer) && (m_ptr == other.m_ptr) && (m_ptr2 == other.m_ptr2) && (m_int == other.m_int) && (m_nativeFD == other.m_nativeFD); }
	inline bool operator != (const mediafd &other) const { return !operator ==(other); }
	
	inline mediafd & reset(media *pOwner = NULL) { m_issuer = pOwner; m_ptr = NULL; m_ptr2 = 0; m_int = 0; m_nativeFD = platform::kINVALID_FILEHANDLE; return *this; }

	media *              m_issuer;    //!< Media object that issued this filehandle
	void *               m_ptr;       //!< Storage for the media object's use
	void *               m_ptr2;      //!< Storage for the media object's use
	off_t                m_int;       //!< Storage for the media object's use
	platform::nativefd_t m_nativeFD;  //!< Storage for the media object's use
	
	// The invalid filehandle.
	static const mediafd kINVALID_FILEHANDLE;
};

#ifndef DOXYGEN_IGNORE
/** \internal
	Initializer which can be used to set a #cell::fios::mediafd to an invalid value. */
#define FIOS_MEDIAFD_INITIALIZER   cell::fios::mediafd::kINVALID_FILEHANDLE
#endif // DOXYGEN_INGNORE

/**
	\brief	Operation events
	\see opcallback_proc
*/
enum e_OPEVENTS { /* operation events */

	/**
		\brief   Operation completed.

		This operation has completed, either successfully or unsuccessfully. Your
		callback should retrieve the error code with op::wait() and react appropriately.
		You should not block for a significant length of time in this callback; doing so
		may delay other I/O requests!

		It's safe to call most other FIOS APIs from this callback, including async I/O APIs
		that create a new operation, or even deleting the operation that you were called about.
		Do not call sync I/O APIs or otherwise wait for any operation other than the one you
		were called about; this causes a deadlock.
		
		Typical use of this callback is to do something to receive the data (copy it,
		signal a semaphore, wake a thread, etc) and then delete the operation. 
		
		\note
		Deleting the operation from this callback causes your callback to be re-entered
		with a #kOPEVENT_DELETE message, which you should be prepared to handle.
		
		Your callback should return #CELL_FIOS_NOERROR. Any other return value is ignored.
	*/
	kOPEVENT_COMPLETED = 1,

	/**
		\brief   Operation is being deleted.

		This operation is being deleted. Your callback can use this as an opportunity to
		clean up any memory or resources that might have been allocated for the request.
		This callback is executed directly from the thread that deleted the operation.

		Your callback should return #CELL_FIOS_NOERROR. Any other return value is ignored.
	*/
	kOPEVENT_DELETE = 2
};

/**
	\brief Callback used by operations.

	This callback should be quick and not run for any significant length of time
	(either processing data or waiting for another operation). Doing so may delay
	other I/O requests.
	
	You may delete the incoming operation as part of handling a completion event,
	but be aware that this causes your callback to be re-entered with a
	deletion event.
	\param[in] pContext    User-specified context pointer
	\param[in] pOp         Operation that generated this event
	\param[in] event       Event type
	\param[in,out] pParam  Event parameter (may be NULL)
	\return            Return #CELL_FIOS_NOERROR if the event was handled.
	\see    e_OPEVENTS
*/
typedef err_t (*opcallback_proc)(
	void *pContext,
	op *pOp,
	opevent_t event,
	void *pParam);

/**
	\brief Operation flags
*/
enum e_OPFLAGS
{
	/** Set to indicate that an I/O is optional, and no good if it comes in late. If it cannot
		complete before the deadline it is cancelled. */
	kOPF_OPTIONAL = (1<<0),
	/** Set to indicate that an I/O should not use any caches but always read or write directly to the media. */
	kOPF_DONTUSECACHE = (1<<1),
	/** Set to indicate that the data returned from an I/O should not be saved in any RAM caches. */
	kOPF_DONTFILLRAMCACHE = (1<<2),
	/** Set to indicate that the data returned from an I/O should not be saved in any disk caches. */
	kOPF_DONTFILLDISKCACHE = (1<<3),
	/** Set to indicate that the data returned from an I/O should remain *permanently* in the disk cache. */
	kOPF_CACHEPERSIST = (1<<4),
	/** Set to indicate that the buffer for this I/O is in uncached memory such as VRAM. */
	kOPF_UNCACHEDBUFFER = (1<<5),
	/** Set to indicate that the buffer for this I/O is in non-DMAable memory such as the stack on PS3.
		
		On PS3, this flag is used by the EdgeDecompressor classes. It must be set when reading from a psarc archive into
		any memory that the SPU is not allowed to touch. When it's set, a small amount of extra CPU and RAM will be used
		to double-buffer the transaction. 
	*/
	kOPF_NONDMABUFFER = (1<<6),
	/** \brief Set to indicate that this request should only be serviced if it can be done without I/O.
		Ops with this flag set will bypass scheduling entirely, but may fail with #CELL_FIOS_ERROR_CANTIMMED. */
	kOPF_IMMED = (1<<7),

	/** Combined flags, used to indicate that the data returned from an I/O should not be saved in any caches. This
		is the same as specifying both #kOPF_DONTFILLRAMCACHE and #kOPF_DONTFILLDISKCACHE. */
	kOPF_DONTFILLCACHE = (kOPF_DONTFILLRAMCACHE | kOPF_DONTFILLDISKCACHE),
	/** Combined flags, used to indicate that no caching should be performed on this I/O at all. This is the
		same as specifying #kOPF_DONTUSECACHE, #kOPF_DONTFILLRAMCACHE, and #kOPF_DONTFILLDISKCACHE. */
	kOPF_NOCACHE = (kOPF_DONTUSECACHE | kOPF_DONTFILLRAMCACHE | kOPF_DONTFILLDISKCACHE),

#ifndef DOXYGEN_IGNORE
	/** \internal
		\brief Internal flag used to indicate that this operation is being deleted. */
	kOPF_DELETED = (1<<8),
	/** \internal
		\brief Internal flag used to indicate that this operation is being cancelled. */
	kOPF_CANCELLED = (1<<9),
	/** \internal
	    \brief Internal flag used to indicate that this operation is ready to have its
		callback invoked. */
	kOPF_CALLBACK = (1<<10),
	/** \internal
		\brief Set to indicate that this operation was dynamically allocated. */
	kOPF_DYNALLOC = (1<<11),
#endif // DOXYGEN_IGNORE
	/** \brief Set if this requires opening an EDATA file. */
	kOPF_EDATA = (1<<12),
	/** \brief Set if this requires opening an SDATA file. */
	kOPF_SDATA = (1<<13),
#ifndef DOXYGEN_IGNORE
	/** \internal
	 * \brief Combined FIOS DRM op flags.*/
	kOPF_DRM = (kOPF_EDATA | kOPF_SDATA),
#endif // DOXYGEN_IGNORE
};

/**
	\brief Context struct used to simplify creating an operation.

	If you are using tags to identify your I/O, remember that not all I/O
	in the queue belongs to you! Do not assume that userPtr is
	a valid pointer unless the userTag is one of your unique tags.
*/
typedef struct opattr_t
{
	abstime_t        deadline;           /**< Deadline for the operation. */
	prio_t           priority;           /**< Priority for the operation. When I/O cannot be satisfied by the deadline, operations with higher numerical priorities are given precedence.
	                                          \see prio_t, e_PRIORITIES */
	opcallback_proc  pCallback;          /**< Callback for the operation (may be NULL). If you are making a sync call, or if you are going to call some variant of op::wait(), you generally don't need to set a callback. */
	void *           pCallbackContext;   /**< Callback context pointer (may be NULL). */
	uint32_t         opflags;            /**< Operation flags to control caching behavior and other details.
										      \see e_OPFLAGS */
	uint32_t         userTag;            /**< Tag for your use. Can be used to identify your operations with scheduler::iterateOps(). */
	void *           userPtr;            /**< Pointer for your use. Any additional data you want to associate with the operation can be put here. */
	const SceNpDrmKey *pLicense;         /**< Pointer to a license key for EDATA files */
} opattr_t;

/** Initializer which can be used to set an opattr_t to all zeroes. */
#define FIOS_OPATTR_INITIALIZER           { 0, 0, 0, 0, 0, 0, 0, 0 }

}; /* namespace fios */
}; /* namespace cell */

#endif /* __CELL_FIOS_TYPES_H__ */
