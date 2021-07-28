/*
	SCE CONFIDENTIAL
	PlayStation(R)3 Programmer Tool Runtime Library 310.001
	Copyright (C) 2009 Sony Computer Entertainment Inc.
	All Rights Reserved.
*/
/**
	\file fios_scheduler.h

	Class definitions for the fios::scheduler class.
*/
#ifndef __CELL_FIOS_SCHEDULER_H__
#define __CELL_FIOS_SCHEDULER_H__

// For SPU SELF and SPRX support
#include <sys/spu_image.h>
#include <sys/prx.h>

#include <cell/fios/fios_types.h>
#include <cell/fios/fios_base.h>
#include <cell/fios/fios_platform.h>
#include <cell/fios/fios_media.h>
#include <cell/fios/fios_op.h>
#include <cell/fios/fios_filehandle.h>

namespace cell {
namespace fios {

class cache;

/** \brief This struct is used to define scheduler attributes upon creation. */
typedef struct scheduler_attr
{
	allocator *pAllocator;	         //!< Custom allocator. Optional: May be NULL, in which case the scheduler will use the global FIOS allocator set by #FIOSInit().
	uint32_t numOps;                 //!< Number of preallocated ops. Set this to the maximum number of simultaneous I/O requests you expect to have in flight at a time.
	uint32_t numFiles;               //!< Number of preallocated filehandles. Set this to the maximum number of filehandles that you expect to have open simultaneously.
	abstime_t prefetchDelay;         //!< How long the scheduler must be idle before prefetching begins
	abstime_t prefetchThrottle;      //!< How long to delay between individual prefetch requests
} scheduler_attr;

/** Initializer which can be used to set a scheduler_attr struct to default values. You will still need to fill in required parameters. */
#define FIOS_SCHEDULER_ATTR_INITIALIZER \
	{ 0, cell::fios::scheduler::kDefaultNumOps, cell::fios::scheduler::kDefaultNumFiles, 0, 0 };

/** \brief This class is the main entry point for clients of FIOS.
	All accesses that generate I/O are available as calls to the scheduler.
	The scheduler is able to queue a large number of requests and optimize
	their order to complete them all in the shortest amount of time possible.

	There is exactly one scheduler object for each media object, and there is
	normally one media object for each physical drive connected to the machine.
	Schedulers act independently of each other; if more than one scheduler has
	been instantiated (because more than one media object is available), then
	all the schedulers will run their I/O in parallel.

	The main application is responsible for creating and configuring scheduler
	objects. You should create a scheduler for the media where your game files
	reside, and then make it the default scheduler with #setDefault().
	This is also your chance to set up the media layer and instantiate any cache
	or emulation layers you want.
*/
class FIOS_LINKAGE scheduler : public object
{
public:
		/** \brief Default values for scheduler parameters */
		enum e_DEFAULTS {
			kDefaultNumOps = 128,         /**< Number of operations that are statically allocated by default. */
			kDefaultNumFiles = 64,        /**< Number of filehandles that are statically allocated by default. */
		};
		
		/** \brief Factory method to create a scheduler object. (deprecated)

			Normally only the main application creates scheduler objects, and it does so exactly once for each type
			of media that it will access. Middleware should use #getSchedulerForMedia() or #getSchedulerForMediaIdentifier()
			instead.

			This call fails and return NULL if the media already has a scheduler associated with it.
			\param[in] pMedia      Media object.
			\param[in] pAllocator  Memory allocator that this scheduler should use. May be NULL, in which case the scheduler will use the FIOS global allocator specified in #FIOSInit().
			\param[in] sizeOfManagedBufferSpace   Ignored. (Formerly: memory to reserve for stream buffer management.)
			\param[in] numOps      Number of operations to statically allocate. Additional operations may be allocated, but will fall back to dynamic allocation.
			\param[in] numFiles    Number of filehandles to statically allocate. Additional filehandles may be allocated, but will fall back to dynamic allocation.
			\param[in] numStreams  Ignored. (Formerly: Number of streams to statically allocate.)
			\return Newly-allocated scheduler. This scheduler may be retrieved later with #getSchedulerForMedia() or #getSchedulerForMediaIdentifier().
		*/
		static scheduler * createSchedulerForMedia(
			media *pMedia,
			allocator *pAllocator, // see fios_configuration.h
			size_t sizeOfManagedBufferSpace = 0,
			uint32_t numOps = kDefaultNumOps,
			uint32_t numFiles = kDefaultNumFiles,
			uint32_t numStreams = 0);
		
		/** \brief Factory method to create a scheduler object.

			Normally only the main application creates scheduler objects, and it does so exactly once for each type
			of media that it will access. Middleware should use #getSchedulerForMedia() or #getSchedulerForMediaIdentifier()
			instead.

			This call will fail and return NULL if the media already has a scheduler associated with it.
			\param[in] pMedia      Media object.
			\param[in] pAttributes Scheduler attributes. See #scheduler_attr for details. May be NULL.
			\return Newly-allocated scheduler. This scheduler may be retrieved later with #getSchedulerForMedia() or #getSchedulerForMediaIdentifier().
		*/
		static scheduler * createSchedulerForMedia(
			media *pMedia,
			const scheduler_attr *pAttributes = NULL);

		/** \brief Factory method to find a scheduler object.
			\param[in] pMedia       Media object.
			\return A scheduler which can be used to access the media, or NULL if no scheduler is available for that media.
		*/
		static inline scheduler * getSchedulerForMedia(media *pMedia) { return (pMedia == NULL) ? NULL:pMedia->getScheduler(); }
		
		/** \brief Factory method to find a scheduler object from a media identifier.
			\param[in] pIdentifier   Media identifier.
			\return A scheduler which can be used to access the media, or NULL if no media with that identifier exists or no scheduler is available for it.
		*/
		static inline scheduler * getSchedulerForMediaIdentifier(const char *pIdentifier) { return getSchedulerForMedia(media::getMediaForIdentifier(pIdentifier)); }
		
		/** \brief Destroys and deallocates a scheduler object allocated by a factory method.

			If there are any requests in the queue, the scheduler may need some time to shut down.
			All scheduled I/O operations are allowed to finish. The scheduler will
			refuse any further I/O that might be requested from the callbacks.
			
			Please note that FIOS keeps the performance data that it has gathered on your drive's real-world
			performance with the scheduler, so terminating the scheduler with destroyScheduler will reset the
			performance data to less accurate default values.
			\param[in] pScheduler    Scheduler object to destroy.
		*/
		static void destroyScheduler(
			scheduler *pScheduler);
		
		/** \brief Shuts down a scheduler and cancels any outstanding operations.

			This function is intended for situations when you need your application to
			terminate quickly after an interrupt-like event. Like #destroyScheduler(),
			it shuts down the scheduler and tells it not to accept any further ops.
			
			There are two main differences between #shutdownAndCancelOps() and #destroyScheduler().
			When you call this function:
			 - The scheduler object is shut down, but NOT destroyed.
			 - All outstanding ops -- except closeFile ops -- are immediately cancelled and waited for.
			
		    This function is safe to call more than once, and it's also safe to call
			#destroyScheduler() after calling this.
			
			Scheduler shutdown is FINAL. Although the scheduler object remains valid,
			there is no way to restart the scheduler thread after this method has been called.
			If you just want to cancel all outstanding ops without shutting down the
			scheduler, see #cancelAllOps(). If you want to temporarily stop the scheduler
			thread from running, see #suspend() and #resume(). 
		*/
		void shutdownAndCancelOps();
		
		/** \brief Cancels any outstanding operations.

			This function can be called to cancel all I/O at once. The scheduler remains
			functional and new ops can still be issued. 
			\param[in] cancelCloses   True = "cancel all ops", false = "cancel all ops except closeFile ops". */
		void cancelAllOps(bool cancelCloses = false);
		
		/** \brief Closes any open files.

			This function can be called to close all open files in response to an
			emergency shutdown event. All filehandles will be marked invalid and cannot be
			used again. The scheduler remains functional and new ops can still be issued. */
		void closeAllFiles();
		
		/** \brief Factory method to retrieve the default scheduler.
			\return The default scheduler, or NULL if no default scheduler has been set.
		*/
		static scheduler * getDefaultScheduler();
		
		/** \brief Sets the default scheduler.
			\param[in] pScheduler    The new scheduler to become the default. (may be NULL)
		*/
		static void setDefaultScheduler(scheduler *pScheduler);
		
		/** \brief Sets this scheduler as the default scheduler.
			\see #setDefaultScheduler()
		*/
		inline void setDefault() { scheduler::setDefaultScheduler(this); }

protected:
#ifndef DOXYGEN_IGNORE
		/** \internal */
		/*@{*/
		FIOS_OBJECT_NEW_AND_DELETE(scheduler,kMemPurposeFIOSSchedulerObject|kPoolIDLongTerm)

		/** \internal
			\brief Creates a scheduler object.

			Clients should not use this method directly. Instead they should use either #createSchedulerForMedia(),
			#getSchedulerForMedia(), or #getSchedulerForMediaIdentifier().
			\param[in] pMedia        Media object to control. This media object must not already have an associated scheduler.
			\param[in] pAttr         Scheduler attributes. Cannot be NULL.
			\see #getDefaultScheduler(), #getSchedulerForMedia(), #getSchedulerForMediaIdentifier(), #scheduler_attr
		*/
		        scheduler(
		            media *pMedia,
					scheduler_attr *pAttr);

		/** \brief Destroys a scheduler object.	*/
		virtual ~scheduler();

		/*@}*/
#endif // DOXYGEN_IGNORE
public:
		/** \brief Gets the maximum possible number of operations in-flight before dynamic allocation will be used.
			\return Number of operations
		*/
		inline uint32_t getMaxOpCount() const { return m_numStaticOps; }
		
		/** \brief Gets the media object that was passed in at creation time.
			\return Media object
		*/
		inline media * getMedia() const { return m_pMedia; }
		
		/** \brief Gets the allocator associated with the scheduler.
			\return Memory allocator
		*/
		inline allocator * getAllocator() const { return m_pAllocator; } // see fios_configuration.h
		
		/** \brief Gets the effective allocator associated with the scheduler.
			\return If non-NULL, the scheduler's allocator. Otherwise the global allocator is returned.
		*/
		allocator * getEffectiveAllocator() const;
		
		/** \brief Checks to see whether the scheduler is idle.

			A scheduler is considered idle if no I/O is being processed. A scheduler may be
			idle even if file handles are open, as long as they are idle and no I/O is
			scheduled for any of them. A suspended scheduler is not considered idle
			if there is pending I/O that has been suspended.
			
			\return              True if the scheduler is idle, false otherwise.
		*/
		bool isIdle() const;
		
		/** \brief   Temporarily suspends a scheduler.

			A scheduler may be suspended to stop all I/O temporarily. This call increments
			the scheduler's suspend count; while the suspend count is non-zero the scheduler
			is suspended.  New I/O requests will be accepted while the scheduler is suspended,
			but they will not be processed until the scheduler's suspend count reaches zero. 
			
			\param[in] pWhy    Optional string which can be used to indicate why the scheduler is being suspended. (may be NULL)
			\see #getSuspendCount(), #resume()
		*/
		void suspend(const char *pWhy = NULL);

		/** \brief   Returns the scheduler's current suspend count.

			The suspend count indicates how many times the scheduler has been
			suspended. Upon creation a scheduler's suspend count will be zero. This
			value is only modified by calls to #suspend() and
			#resume(), which should be equally balanced.
			
			\return              The current suspend count.
		*/
		inline uint32_t getSuspendCount() const { return m_suspendCount; }
		
		/** \brief   Checks to see whether a scheduler is suspended.
			\return  True if the scheduler is suspended, false otherwise.
		*/
		inline bool isSuspended() const { return (getSuspendCount() > 0); }
		
		/** \brief   Resumes a scheduler after suspension.

			This call decrements the scheduler's suspend count.  If the suspend count reaches
			zero as a result, the scheduler is resumed and I/O is started immediately. If the
			suspend count is already zero, this call has no effect.
			\param[in] pWhy     Optional string which can be used to indicate why the scheduler was suspended. (may be NULL)
		*/
		void resume(const char *pWhy = NULL);

protected:
#ifndef DOXYGEN_IGNORE
		/** \internal
			\brief Common bottleneck for #stat(), #getFileSize(), #fileExists(), #directoryExists(), and #itemExists().
			\stdparam
			\param[in]  pPath    Path on the media.
			\param[out] pBuffer  Upon successful completion, filled in with either a boolean, off_t, or stat_t (depending on stage).
			\param[in]  stage    Op stage that selects between stat or file/dir/item existence.
			\syncreturn */
		op * statCommon(
			const opattr_t *pAttr,
			const char *pPath,
			void *pBuffer,
			fios::op::opstage_t stage);

#endif // DOXYGEN_IGNORE
public:
		/** \brief Returns the size of a file.
			\stdparam
			\param[in]   pPath      Path to a file on the media.
			\param[out]  pByteSize  Upon successful completion, filled in with the size of the file.
			\asyncreturn
		*/
		op * getFileSize(
			const opattr_t *pAttr,
			const char *pPath,
			off_t *pByteSize) { return statCommon(pAttr,pPath,pByteSize,fios::op::kOPSTAGE_GETFILESIZE_STAT); }
		
		/** \brief Returns the size of a file (sync)

			This sync call is equivalent to #getFileSize() followed by op::syncWait().
			\stdparam
			\param[in]   pPath     Path to a file on the media.
			\param[out]  pByteSize  Upon successful completion, filled in with the size of the file.
			\see #getFileSize(), op::wait()
			\syncreturn
		*/
		inline err_t getFileSizeSync(
			const opattr_t *pAttr,
			const char *pPath,
			off_t *pByteSize) { return getFileSize(pAttr,pPath,pByteSize)->syncWait(); }
		
		/** \brief Indicates whether a file exists at a path.
			\note This function will return FALSE if a directory exists at the path instead of a file.
			If you wish to check for any item, use #itemExists().
			\stdparam
			\param[in]  pPath      Path to a file on the media.
			\param[out] pExists    Upon successful completion, a TRUE or FALSE value will be put here.
			\asyncreturn
		*/
		op * fileExists(
			const opattr_t *pAttr,
			const char *pPath,
			bool *pExists) { return statCommon(pAttr,pPath,pExists,fios::op::kOPSTAGE_FILEEXISTS_STAT); }
		
		/** \brief Indicates whether a file exists. (sync)

			This sync call is equivalent to #fileExists() followed by op::syncWait().
			
			\note This function will return FALSE if a directory exists at the path instead of a file.
			If you wish to check for any item, use #itemExistsSync().
			\stdparam
			\param[in]  pPath    Path to a file on the media.
			\param[out] pExists  Upon successful completion, filled in with a value indicating the presence of the file.
			\syncreturn
		*/
		inline err_t fileExistsSync(
			const opattr_t *pAttr,
			const char *pPath,
			bool *pExists) { return fileExists(pAttr,pPath,pExists)->syncWait(); }
		
		/** \brief Indicates whether a directory exists at a path.

			This function will return FALSE if a file exists at the path instead of a directory.
			If you wish to check for any item, use #itemExists().
			\stdparam
			\param[in]  pPath    Path to a directory on the media.
			\param[out] pExists  Upon successful completion, filled in with a value indicating the presence of the directory.
			\asyncreturn
		*/
		op * directoryExists(
			const opattr_t *pAttr,
			const char *pPath,
			bool *pExists) { return statCommon(pAttr,pPath,pExists,fios::op::kOPSTAGE_DIREXISTS_STAT); }
		
		/** \brief Indicates whether a directory exists. (sync)

			This sync call is equivalent to #directoryExists() followed by op::syncWait().

			This function will return FALSE if a file exists at the path instead of a directory.
			If you wish to check for any item, use #itemExistsSync().
			\stdparam
			\param[in]  pPath    Path to a directory on the media.
			\param[out] pExists  Upon successful completion, filled in with a value indicating the presence of the directory.
			\syncreturn
		*/
		inline err_t directoryExistsSync(
			const opattr_t *pAttr,
			const char *pPath,
			bool *pExists) { return directoryExists(pAttr,pPath,pExists)->syncWait(); }
		
		/** \brief Indicates whether any item exists at a path.
			\note This function returns TRUE if any item (file, directory, or other) exists at the
			path. If you wish to check for one or the other, use #fileExists() or 
			#directoryExists().
			\stdparam
			\param[in]  pPath    Path on the media.
			\param[out] pExists  Upon successful completion, filled in with a value indicating the presence of an item.
			\asyncreturn
		*/
		op * itemExists(
			const opattr_t *pAttr,
			const char *pPath,
			bool *pExists) { return statCommon(pAttr,pPath,pExists,fios::op::kOPSTAGE_ITEMEXISTS_STAT); }
		
		/** \brief Indicates whether any item exists at a path. (sync)

			This sync call is equivalent to #itemExists() followed by op::syncWait().

			\note This function will return TRUE if any item (file, directory, or other) exists at the
			path. If you wish to check for one or the other, use #fileExists() or 
			#directoryExists().
			\stdparam
			\param[in]  pPath    Path on the media.
			\param[out] pExists  Upon successful completion, filled in with a value indicating the presence of an item.
			\syncreturn
		*/
		inline err_t itemExistsSync(
			const opattr_t *pAttr,
			const char *pPath,
			bool *pExists) { return itemExists(pAttr,pPath,pExists)->syncWait(); }

		/** \brief Returns a full set of status information for a file or directory.
			\note Because it may require more I/O to get all the information needed by #stat(), you should
			only use this call if you are looking for information other than size and existence.
			If you just want to check for existence, use #fileExists(), #directoryExists(), or
			#itemExists() instead. If you just want to get a file's size (and indirectly
			test for existence), use #getFileSize() instead.
			\stdparam
			\param[in]  pPath     Path on the media.
			\param[out] pStatus   Upon successful completion, filled in with a fios::stat_t.
			\asyncreturn
		*/
		op * stat(
			const opattr_t *pAttr,
			const char *pPath,
			stat_t *pStatus) { return statCommon(pAttr,pPath,pStatus,fios::op::kOPSTAGE_CALLBACK); }
		
		/** \brief Returns a full set of status information for a file or directory.

			This sync call is equivalent to #stat() followed by op::syncWait().
			\stdparam
			\param[in]  pPath    Path on the media.
			\param[out] pStatus  Upon successful completion, filled in with a fios::stat_t.
			\syncreturn
		*/
		inline err_t statSync(
			const opattr_t *pAttr,
			const char *pPath,
			stat_t *pStatus) { return stat(pAttr,pPath,pStatus)->syncWait(); }
		
		/** \brief Reads bytes from a file without opening it first.

			This call is equivalent to open/seek/read/close.
			\stdparam
			\param[in]  pPath      Path to a file on the media.
			\param[out] pBuf       Buffer to receive file data.
			\param[in]  byteOffset Offset within the file to read from.
			\param[in]  byteCount  Requested number of bytes to read.
			\asyncreturn
		*/
		op * readFile(
			const opattr_t *pAttr,
			const char *pPath,
			void *pBuf,
			off_t byteOffset,
			off_t byteCount);
		
		/** \brief  Reads bytes from a file without opening it. (sync)

			This sync call is equivalent to #readFile() followed by op::syncWait().

			\stdparam
			\param[in]  pPath      Path to a file on the media.
			\param[out] pBuf       Buffer to receive file data.
			\param[in]  byteOffset Offset within the file to read from.
			\param[in]  byteCount  Requested number of bytes to read.
			\syncreturn
		*/
		inline err_t readFileSync(
			const opattr_t *pAttr,
			const char *pPath,
			void *pBuf,
			off_t byteOffset,
			off_t byteCount) { return readFile(pAttr,pPath,pBuf,byteOffset,byteCount)->syncWait(); }
		
		/** \brief  Writes bytes to a file without opening it.

			This call is equivalent to open/seek/write/close. As with a normal write, this call
			will overwrite bytes within the file until the end of the file is reached. Once the
			end of the file is reached any remaining data is appended.
			\stdparam
			\param[in]  pPath      Path to a file on the media.
			\param[in]  pBuf       Buffer with file data to write.
			\param[in]  byteOffset Offset within the file to write to.
			\param[in]  byteCount  Requested number of bytes to write.
			\asyncreturn
		*/
		op * writeFile(
			const opattr_t *pAttr,
			const char *pPath,
			const void *pBuf,
			off_t byteOffset,
			off_t byteCount);

		/** \brief  Writes bytes to a file without opening it. (sync)

			This sync call is equivalent to #writeFile() followed by op::syncWait().
			
			\stdparam
			\param[in]  pPath      Path to a file on the media.
			\param[in]  pBuf       Buffer with file data to write.
			\param[in]  byteOffset Offset within the file to read from.
			\param[in]  byteCount  Requested number of bytes to read.
			\syncreturn
		*/
		inline err_t writeFileSync(
			const opattr_t *pAttr,
			const char *pPath,
			const void *pBuf,
			off_t byteOffset,
			off_t byteCount) { return writeFile(pAttr,pPath,pBuf,byteOffset,byteCount)->syncWait(); }
		
		/** \brief	Creates a directory.
			\stdparam
			\param[in]  pPath              Path on the media to the directory that you want to create.
			\asyncreturn
		*/
		op * createDirectory(
			const opattr_t *pAttr,
			const char *pPath);
		
		/** \brief  Creates a directory (sync).
			\stdparam
			\param[in]  pPath              Path on the media to the directory that you want to create.
			\syncreturn
		*/
		inline err_t createDirectorySync(
			const opattr_t *pAttr,
			const char *pPath) { return createDirectory(pAttr,pPath)->syncWait(); }
		
		/** \brief Iterates through a directory.

			This call retrieves one child from a directory. It's best for light use, where you
			only have one directory being iterated at a time. Entries are not returned in any specific
			order.
			
			Entries always represent real files or directories. Special entries such as "." and
			".." are not returned.
			\stdparam
			\param[in]  pPath       Directory to get children from.
			\param[in]  childIndex  Zero-based index representing the child to retrieve. Increment to iterate.
			\param[out] pChildEntry Receives the directory entry.
			\asyncreturn
		*/
		op * readDirectory(
			const opattr_t *pAttr,
			const char *pPath,
			off_t childIndex,
			direntry_t *pChildEntry);
		
		/** \brief Iterates through a directory (sync)

			This call is equivalent to #readDirectory() followed by op::syncWait().
			\stdparam
			\param[in]  pPath       Directory to get children from.
			\param[in]  childIndex  Zero-based index representing the child to retrieve. Increment to iterate.
			\param[out] pChildEntry Receives the directory entry.
			\syncreturn
		*/
		inline err_t readDirectorySync(
			const opattr_t *pAttr,
			const char *pPath,
			off_t childIndex,
			direntry_t *pChildEntry) { return readDirectory(pAttr,pPath,childIndex,pChildEntry)->syncWait(); }
		
		/** \brief Deletes a file or directory.

			This call deletes a file or directory. Directories must be empty,
			or the call will fail with #CELL_FIOS_ERROR_ACCESS. 
			\stdparam
			\param[in]  pPath       File or directory to delete.
			\asyncreturn
		*/
		op * unlink(
			const opattr_t *pAttr, 
			const char *pPath);
		
		/** \brief Deletes a file or directory. (sync)

			This call is equivalent to #unlink() followed by op::syncWait().
			\stdparam
			\param[in]  pPath       File or directory to delete.
			\syncreturn
		*/
		inline err_t unlinkSync(
			const opattr_t *pAttr,
			const char *pPath) { return unlink(pAttr,pPath)->syncWait(); }

		/** \brief Prefetches a file into the disk cache.

			If (and only if) you have a fios::schedulercache in the media stack, this call will
			prefetch a file's data into the cache so that it can be read quickly later on.
			This prefetch runs at a low priority so that it will only run in the unused moments
			when the drive would otherwise be idle, and won't interfere with your
			other I/O requests. 
			
			For example, on PS3 with a scheduler pointing at /dev_bdvd and the cache
			files on /dev_hdd1, this call can be used to asynchronously copy the file from
			Blu-ray to the hard disk. When the file is accessed later it will be read at
			HDD speed (~20MiB/s) instead of Blu-ray speed (~8MiB/s).
			
			\note The #kOPF_CACHEPERSIST flag can be used here to keep the prefetched data in the cache permanently.
			
			\stdparam
			\param[in]  pPath        File to prefetch.
			\param[in]  byteOffset   Offset within the file where the prefetch should start. Default = 0.
			\param[in]  byteCount    Length that the prefetch should cache. Default = whole file.
			\asyncreturn */
		op * prefetchFile(
			const opattr_t *pAttr,
			const char *pPath,
			off_t byteOffset = 0,
			off_t byteCount = FIOS_OFF_T_MAX);
		
		/** \brief Prefetches a file into the disk cache.

			This call is equivalent to #prefetchFile() followed by op::syncWait().
			\stdparam
			\param[in]  pPath        File to prefetch.
			\param[in]  byteOffset   Offset within the file where the prefetch should start. Default = 0.
			\param[in]  byteCount    Length that the prefetch should cache. Default = #FIOS_OFF_T_MAX = whole file.
			\syncreturn */
		inline err_t prefetchFileSync(
			const opattr_t *pAttr,
			const char *pPath,
			off_t byteOffset = 0,
			off_t byteCount = FIOS_OFF_T_MAX) { return prefetchFile(pAttr,pPath,byteOffset,byteCount)->syncWait(); }
		
		/** \brief Queries whether a portion of a file is in the cache.

			This call is equivalent to #resolveSync() followed by cache::isRequestInCache().
			It cannot tell you when a whole file is in the cache unless the file's length is known.
			\param[in]  pCacheMedia   Cache mediafilter that you are querying.
			\param[in]  pPath         Path to file.
			\param[in]  byteOffset    Offset within the file.
			\param[in]  byteCount     Length to check.
			\return True if the entire specified section of the file is in the cache, false otherwise. */
		bool isRequestInCache(
			fios::cache *pCacheMedia,
			const char *pPath,
			off_t byteOffset,
			off_t byteCount) FIOS_ATTRIBUTE_GCC((__nonnull__));
		
		/** \brief Resolves a (path,offset,len) tuple to a native or media-relative tuple.
			
			This routine is used to send a (path,offset,len) tuple through the stack to
			be translated so that it can be used with either native APIs or a particular
			media object.
			
			Example 1: resolving a path to check cache contents. If you have an archive
			which may be partially in a HDD cache, you could translate
			("/archive/contents.dat",0,FIOS_OFF_T_MAX) to
			("/archive.psarc",offsetInArchive,lengthInArchive) for the cache,
			and then use it in cache::isRequestInCache().
			
			Example 2: resolving a path for inspection or use by native APIs. If you have
			an overlay set up so that files may come from multiple locations, you can translate
			"/overlay/file.dat" to the corresponding native path, which might be any
			of the overlay sources -- e.g. "/app_home/file.dat", "/dev_bdvd/file.dat",
			or "/dev_hdd1/file.dat".
			
			\note The data in a resolved request may be compressed or otherwise transformed
			(encrypted, patched, etc) by media layers in your media stack. Please take
			care when passing a resolved path to native APIs!
			\stdparam
			\param[in,out] pFileRange    (Path,Offset,Length) tuple to resolve. 
			\param[in]     pTargetMedia  NULL to get a native path, otherwise the target media.
			\asyncreturn */
		op * resolve(
			const opattr_t *pAttr,
			filerange_t *pFileRange,
			media *pTargetMedia = NULL);
		
		/** \brief Resolves a path to either a media-relative path or a native path. (sync)

			This call is equivalent to #resolve() followed by op::syncWait().
			\stdparam
			\param[in,out] pFileRange    (Path,Offset,Length) tuple to resolve. 
			\param[in]     pTargetMedia  NULL to get a native path, otherwise the target media.
			\asyncreturn */
		inline err_t resolveSync(
			const opattr_t *pAttr,
			filerange_t *pFileRange,
			media *pTargetMedia = NULL) { return resolve(pAttr,pFileRange,pTargetMedia)->syncWait(); }

		/** \brief Open a file.
			\stdparam
			\param[in]  pPath     Path to a file on the media.
			\param[in]  openFlags Open flags.
			\param[out] pNewFH    Upon successful completion, pointer to the newly-opened filehandle.
			\asyncreturn
			\see e_OPENFLAGS
		*/
		op * openFile(
			const opattr_t *pAttr,
			const char *pPath,
			uint32_t openFlags,
			filehandle **pNewFH);

		/** \brief Open a file. (sync)

			This sync call is equivalent to #openFile() followed by op::syncWait().
			\stdparam
			\param[in]  pPath      Path to a file on the media.
			\param[in]  openFlags  Open flags.
			\param[out] pNewFH     Upon successful completion, pointer to file handle.
			\syncreturn
			\see #e_OPENFLAGS, #openFile(), op::syncWait()
		*/
		inline err_t openFileSync(
			const opattr_t *pAttr,
			const char *pPath,
			uint32_t openFlags,
			filehandle **pNewFH) { return openFile(pAttr,pPath,openFlags,pNewFH)->syncWait(); }
		
		/** \brief      Seek within an open file.

			This call changes the current position within the file, and returns the updated
			position. It can be used to query the current position by using #kSEEK_CUR
			with an offset of 0.
			
			This call is equivalent to filehandle::seek().
			
			In the current implementation, this call operates immediately and the operation
			attributes are ignored.
			\stdparam
			\param[in]  pFH       File handle.
			\param[in]  offset    Offset to seek to.
			\param[in]  whence    How to seek (from start, current location, or end).
			\param[out] pResult   New offset (may be NULL).
			\syncreturn
			\see #e_WHENCE, filehandle::seek()
		*/
		err_t seekFileSync(
			const opattr_t *pAttr,
			filehandle *pFH,
			off_t offset,
			whence_t whence,
			off_t *pResult = NULL);
		
		/** \brief		Read from an open file.
			\stdparam
			\param[in]  pFH        File handle.
			\param[out] pBuf       Buffer to receive file data.
			\param[in]  byteCount  Number of bytes to read.
			\param[in]  offset     Offset inside the file at which to read. If the offset is -1, the current offset is used and the file pointer is immediately incremented by byteCount. If any other offset is used, the file pointer remains the same.
			\asyncreturn
		*/
		op * readFile(
			const opattr_t *pAttr,
			filehandle *pFH,
			void *pBuf,
			off_t byteCount,
			off_t offset = -1);

		/** \brief      Read from an open file (sync).

			This sync call is equivalent to #readFile() followed by op::syncWait().
			\stdparam
			\param[in]  pFH        File handle.
			\param[out] pBuf       Buffer to receive file data.
			\param[in]  byteCount  Number of bytes to read.
			\param[in]  offset     Offset inside the file at which to read. If the offset is -1, the current offset is used and the file pointer is immediately incremented by byteCount. If any other offset is used, the file pointer remains the same.
			\syncreturn
			\see #readFile(), op::syncWait()
		*/
		inline err_t readFileSync(
			const opattr_t *pAttr,
			filehandle *pFH,
			void *pBuf,
			off_t byteCount,
			off_t offset = -1) { return readFile(pAttr,pFH,pBuf,byteCount,offset)->syncWait(); }
		
		/** \brief      Write to an open file.
			\stdparam
			\param[in]  pFH        File handle.
			\param[in]  pBuf       Buffer with file data to write.
			\param[in]  byteCount  Number of bytes to write.
			\param[in]  offset     Offset inside the file at which to write. If the offset is -1, the current offset is used and the file pointer is immediately incremented by byteCount. If any other offset is used, the file pointer remains the same.
			\asyncreturn
		*/
		op * writeFile(
			const opattr_t *pAttr,
			filehandle *pFH,
			const void *pBuf,
			off_t byteCount,
			off_t offset = -1);

		/** \brief      Write to an open file (sync)
			\stdparam
			\param[in]  pFH       File handle.
			\param[in]  pBuf      Buffer with file data to write.
			\param[in]  byteCount Number of bytes to write.
			\param[in]  offset     Offset inside the file at which to write. If the offset is -1, the current offset is used and the file pointer is immediately incremented by byteCount. If any other offset is used, the file pointer remains the same.
			\syncreturn
			\see #writeFile(), op::syncWait()
		*/
		inline err_t writeFileSync(
			const opattr_t *pAttr,
			filehandle *pFH,
			const void *pBuf,
			off_t byteCount,
			off_t offset = -1) { return writeFile(pAttr,pFH,pBuf,byteCount,offset)->syncWait(); }
		
		/** \brief     Flush changes to a file that is open for writing.
			
			This function may fail if the underlying media doesn't support the sync command. For example,
			on PS3 syncFile only works with files from HDD0 and HDD1, and will return an error for files on HOSTFS. 
			\stdparam
			\param[in]  pFH       File handle.
			\asyncreturn
		*/
		op * syncFile(
			const opattr_t *pAttr,
			filehandle *pFH);
		
		/** \brief     Flush changes to a file that is open for writing (sync).
			\stdparam
			\param[in]  pFH       File handle.
			\syncreturn
			\see #syncFile(), op::syncWait()
		*/
		inline err_t syncFileSync(
			const opattr_t *pAttr,
			filehandle *pFH) { return syncFile(pAttr, pFH)->syncWait(); }
		
		/** \brief     Close an open file.

			The scheduler will wait until all pending I/O on the file handle is complete
			before releasing its memory and deleting the object.
			\stdparam
			\param[in]   pFH      File handle.
			\asyncreturn
		*/
		op * closeFile(
			const opattr_t *pAttr,
			filehandle *pFH);
		
		/** \brief    Close an open file (sync)
			\stdparam
			\param[in]   pFH      File handle. This will be disposed and should not be referenced after this call completes.
			\syncreturn
			\see #closeFile(), op::syncWait()
		*/
		inline err_t closeFileSync(
			const opattr_t *pAttr,
			filehandle *pFH) { return closeFile(pAttr,pFH)->syncWait(); }
		
		/** \brief Loads an SPU image into memory.
				
			This async call corresponds to the native API sys_spu_image_open. Please
			refer to the document "Cell OS Lv-2 System Call and Library Reference" for
			parameter details. Other SPU program image system calls can be used upon
			succesful completion of this call.
					
			\stdparam
			\param[in]  pPath      Same as sys_spu_image_open's "path" argument.
			\param[in]  pImage     Same as sys_spu_image_open's "img" argument.    
			\asyncreturn
		*/
		op * loadSPUImage(
			const opattr_t *pAttr,
			const char *pPath,
			sys_spu_image_t *pImage);
				
		/** \brief Loads an SPU image into memory. (sync)
				
			This sync call is equivalent to scheduler::loadSPUImage() followed by op::syncWait().
					
			\stdparam
			\param[in]  pPath      Same as sys_spu_image_open's "path" argument.
			\param[in]  pImage     Same as sys_spu_image_open's "img" argument.    
			\syncreturn
		*/
		inline err_t loadSPUImageSync(
			const opattr_t *pAttr,
			const char *pPath,
			sys_spu_image_t *pImage) { return loadSPUImage(pAttr,pPath,pImage)->syncWait(); }

		/** \brief Loads a PRX into memory.
				
			This async call corresponds to the native APIs sys_prx_load_module and
			sys_prx_load_module_on_memcontainer. Please refer to the document
			"Cell OS Lv-2 System Call and Library Reference (PRX)" for parameter
			details. Other PRX API's must be used after succesful completion of this call.
					
			\stdparam
			\param[in]  pPath      Same as sys_prx_load_module's "path" argument.
			\param[in]  container  Same as sys_prx_load_module_on_memcontainer's "mem_container" argument. Specify SYS_MEMORY_CONTAINER_ID_INVALID to use sys_prx_load_module instead.
			\param[in]  flags      Same as sys_prx_load_module's "flags" argument.
			\param[in]  pOpt       Same as sys_prx_load_module's "pOpt" argument.
			\param[out] pPRXid     Upon successful completion, returns the sys_prx_id_t.
			\asyncreturn
		*/
		op * loadPRX(
			const opattr_t *pAttr,
			const char *pPath,
			sys_memory_container_t container,
			sys_prx_flags_t flags,
			sys_prx_load_module_option_t *pOpt,
			sys_prx_id_t *pPRXid);
				
		/** \brief Loads a PRX into memory. (sync)
				
			This sync call is equivalent to scheduler::loadPRX() followed by op::syncWait().
					
			\stdparam
			\param[in]  pPath      Same as sys_prx_load_module's "path" argument.
			\param[in]  container  Same as sys_prx_load_module_on_memcontainer's "mem_container" argument. Specify SYS_MEMORY_CONTAINER_ID_INVALID to use sys_prx_load_module instead.
			\param[in]  flags      Same as sys_prx_load_module's "flags" argument.
			\param[in]  pOpt       Same as sys_prx_load_module's "pOpt" argument.
			\param[out] pPRXid     Upon successful completion, returns the sys_prx_id_t.
			\syncreturn
		*/
		inline err_t loadPRXSync(
			const opattr_t *pAttr,
			const char *pPath,
			sys_memory_container_t container,
			uint64_t flags,
			sys_prx_load_module_option_t *pOpt,
			sys_prx_id_t *pPRXid) { return loadPRX(pAttr,pPath,container,flags,pOpt,pPRXid)->syncWait(); }

		/** \brief Cancels an operation.

			If the operation has not yet executed, its error code will be set to #CELL_FIOS_ERROR_CANCELLED
			and it will be removed from the queue. If a callback is set, the callback will be invoked
			with a #kOPEVENT_COMPLETED event.
			
			\note Because I/O happens asynchronously, it's possible that the operation may be executing or
			already completed when you issue this call, in which case it's too late to cancel! If
			that happens the operation will not be cancelled; instead the error code will be set to
			the result of the operation as usual.
			
			This function does not dispose the operation handle; you should still delete the operation
			to reclaim the memory used by the object. Since deleting an operation does an implicit
			cancel, you may wish to simply delete operations to cancel them.
			\param[in]  pOp     Operation handle.
			\see op::cancel()
		*/
		void cancelOp(
			op *pOp);
		
		/** \brief Deletes an operation.

			If the operation has not yet completed, it is canceled first with cancelOp().
			The operation pointer may be reused for another operation later.
			\param[in]  pOp     Operation handle.
			\see #cancelOp()
		*/
		void deleteOp(
			op *pOp);
		
		/** \brief Reschedules an operation.

			If the operation has not yet executed, its priority and deadline are changed and
			it is moved to the incoming queue for rescheduling.

			\note
			Because I/O happens asynchronously, it's possible that the operation may be executing or
			already completed when you issue this call, in which case this call has no effect.
			\param[in]  pOp       Operation handle.
			\param[in]  deadline  New deadline.
			\param[in]  priority  New priority.
		*/
		void rescheduleOp(
			op *pOp,
			abstime_t deadline,
			prio_t priority);
		
		/** \brief Returns the estimated time at which an operation will complete

			This call returns the scheduler's current best estimate of when the operation
			will be complete.  This value may change at any time, either because additional
			I/O requests were received or because the scheduling algorithm finds an optimization
			opportunity.
			
			\note
			The scheduler's optimization algorithm does not always push operations forward. Any
			individual operation may be pushed later in the schedule, rather than earlier, as
			long as it does not violate any deadlines and results in a shorter overall time for
			the list of scheduled I/Os as a whole.
			\param[in]  pOp    Operation handle
			\return   The current estimate for the completion time.
		*/
		abstime_t getEstimatedCompletion(
			const op *pOp) const;
		
		/** \brief Performs an action on all operations in the queue.

			This can be used to selectively cancel operations or otherwise
			modify them -- for example, you could use an action to 
			cancel a whole series of level-related operations at once.
			
			The action callback should do its job quickly, because I/O and scheduling
			may be blocked until it completes.
			\param[in]  pAction         Action callback.
			\param[in]  pActionContext  Action context.
		*/
		void iterateOps(
			void (*pAction)(void *context, op *pOp),
			void *pActionContext);
		
		/** \brief   Temporarily suspends prefetch operations.

			Prefetches may be suspended temporarily. This call increments a
			prefetch suspend count, while the count is non-zero no prefetch
			operations will be issued. New prefetch requests (submitted using
			prefetchFile) will be accepted, but they will not be processed
			until the prefetch suspend count reaches zero. 
			
			\see scheduler::resumePrefetch()
		*/
		void suspendPrefetch(const char *pWhy = NULL);

		/** \brief   Returns the scheduler's current prefetch suspend count.

			The prefetch suspend count indicates how many times the prefetch has been
			suspended. Upon creation a scheduler's suspend count will be zero. This
			value is only modified by calls to scheduler::suspendPrefetch() and
			scheduler::resumePrefetch(), which should be equally balanced.
			
			\return              The current prefetch suspend count.
		*/
		inline uint32_t getPrefetchSuspendCount() const { return m_prefetchSuspendCount; }
		
		/** \brief   Checks to see whether a scheduler is suspended.
			\return  True if the scheduler is suspended, false otherwise.
		*/
		inline bool isPrefetchSuspended() const { return (getPrefetchSuspendCount() > 0); }

		/** \brief   Resumes prefetch operations after a suspension.

			This call decrements the prefetch suspend count.  If the count
			reaches zero any outstanding prefetch operations are resumed, if
			the suspend count is already zero, this call has no effect.

			\see scheduler::suspendPrefetch()
		*/
		void resumePrefetch(const char *pWhy = NULL);

		/** Sets the time to wait before prefetching starts.
			\param[in] interval    Interval to wait; higher values make prefetches start more slowly. */
		void setPrefetchDelay(abstime_t interval) { m_prefetchDelay = interval; }
		
		/** Sets the time to wait in between individual prefetch requests.
			\param[in] interval    Interval to wait; higher values make prefetches run more slowly. */
		void setPrefetchThrottle(abstime_t interval) { m_prefetchThrottle = interval; }
		
protected:
#ifndef DOXYGEN_IGNORE
		/** \internal
			\brief Allocates an operation.

			Used by all functions that return new operations.
			\param[in]  pAttr        Operation attributes (may be NULL).
			\return Newly-allocated operation, or NULL.
		*/
		op * allocateOp(
			const opattr_t *pAttr);
		/** \internal
			\brief Deallocates an operation.
			Called by deleteOp. The operation should be in the completed list.
			\param[in]  pOp         Operation to delete.
		*/
		void deallocateOp(
			op *pOp);

		/** \internal
			\brief Moves a newly-created operation to the incoming queue.

			Used by all functions that create an operation.
			\param[in] pOp   Operation to accept.
			\return Returns the operation.
		*/
		op * acceptOp(op *pOp);

		/** \internal
			\brief Moves an operation to the completed list.

			Used by all functions that complete an operation, including functions
			that create an operation that executes immediately. This does not remove
			the operation from any list it might be in; that must be done prior to this call.
			\param[in] pOp   Operation to complete.
			\return Returns the operation.
		*/
		op * completeOp(op *pOp);

#endif // DOXYGEN_IGNORE
private:
#ifndef DOXYGEN_IGNORE
	friend class op;                       /**< The op class needs to access some internal variables */
	friend class filehandle;               /**< The filehandle class needs the m_fhLock. */

	// Class variables.
	static platform::atomicPtr<scheduler *> s_pDefaultScheduler; /**< Default scheduler. */
	
	// Unchanging member variables. Set up in the constructor and never changed.
	mutable platform::mutex m_objectLock;      /**< Object lock for accessing member variables. */
	mutable platform::mutex m_opLock;          /**< Object lock for accessing operations that belong to this scheduler. */
	mutable platform::rwlock m_opCallbackLock; /**< Rwlock for accessing operation callbacks. */
	mutable platform::mutex m_completedLock;   /**< Object lock guarding the completed list. */
	platform::thread m_schedulerThread;    /**< Scheduler thread */
	mutable platform::mutex m_ioLock;      /**< Mutex used for m_ioCond, m_idleCond, and other I/O related stuff */
	mutable platform::cond m_ioCond;       /**< Condition used when waiting for I/O or new operations */
	mutable platform::cond m_idleCond;     /**< Condition used to signal when the scheduler is idle. */
	mutable platform::mutex m_fhLock;      /**< Lock that protects filehandles. */
	media *m_pMedia;                       /**< Media object controlled by this scheduler. */
	uint32_t m_numStaticOps;                    /**< Number of statically allocated ops. */
	uint32_t m_numStaticFiles;                  /**< Number of statically allocated filehandles. */
	void *m_staticOpAllocation;            /**< Static op allocation. */
	void *m_staticFileAllocation;          /**< Static file allocation. */
	mutable allocator *m_pAllocator;       /**< Allocator for this scheduler */
	size_t m_maxConcurrentOps;             /**< Max concurrent ops, from media::getMaxConcurrentOps */
	
	bool m_ioDone;                         /**< Whether any ops in the issue list are done. Protected by m_ioLock. */
	
	// Changeable member variables
	platform::atomicU32 m_suspendCount;    /**< Suspend count */
	platform::atomicBool m_acceptingNewIO; /**< Whether this scheduler is accepting new I/O. Normally true, set to false during deletion. */
	
	// Worker thread stuff
	platform::mutex m_workerLock;
	platform::cond m_workerCond;
	bool m_workerThreadsRunning;
	size_t m_cacheHitWorker;               /**< Index of the cache-hit worker, or size_t(-1) if none. */
	uint32_t m_issueNumber;
	platform::atomicU32 m_dispatchNumber;
	platform::atomicInt<abstime_t> m_lastDispatchTime;
	platform::atomicU32 m_prefetchSuspendCount;    /**< Suspend prefetch count */
	abstime_t m_prefetchDelay;
	abstime_t m_prefetchThrottle;
	platform::atomicBool m_prefetching;
	
	platform::atomicU32 m_ioGeneration;    /**< Live generation count, bumped every time an I/O completion callback occurs */
	uint32_t m_myGeneration;                    /**< The last generation we handled in the scheduler loop. */
	
	// PCE math -- everything here protected by m_ioLock and only accessed by the scheduler thread
	abstime_t *m_pUnknownEstimate;         /**< Estimates of unknown values. Array of size m_pMedia->getNumberOfCoefficients() */
	abstime_t m_Emax;                      /**< Maximum error seen by the scheduler after estimates have stabilized (after m_opTotal > 50) */
	void *m_pCurrentMediaState;            /**< Pointer to current media state (state right now). Access protected by m_ioLock. */
	void *m_pProjectedMediaState;          /**< Pointer to projected media state (state after committed I/Os have completed). Access protected by m_ioLock. */
	abstime_t m_projectedCompletion;       /**< Projected time at which already-committed I/O will complete. */
	
	// Op lists
	collections::atomicList<op *> m_pFreeOps;      /**< Free operations, available for allocation */
	collections::atomicList<op *> m_pIncomingOps;  /**< Incoming operations, not yet scheduled */
	collections::list<op *> m_pScheduleQueue;      /**< Queued and scheduled ops. Access protected by m_opLock. */
	collections::list<op *> m_pPrefetchQueue;      /**< Prefetch ops. Access protected by m_opLock. */
	collections::list<op *> m_pIssueList;          /**< Issued ops waiting for completion. Access protected by m_opLock. */
	collections::list<op *> m_pCompletedOps;       /**< Completed but not-yet-deleted ops. Access protected by m_completedLock. */
	platform::atomicU32 m_opCount;              /**< Total operation count. Includes incoming, scheduled, and completed ops (but not free ops) */
	platform::atomicU32 m_activeOpCount;        /**< Active operations currently executing. Never exceeds m_pMedia->getMaxConcurrentOps(). */
	platform::atomicU32 m_opTotal;              /**< Number of operations that have completed in total. */
	platform::atomicU32 m_opHighWaterMark;      /**< Highest number of operations allocated at any one time. */
	uint32_t m_overlapCompletions;                   /**< Number of ops completed via our overlap logic */
	mediaioparams *m_pIOPs;                     /**< I/O request structs. Access from scheduler protected by m_opLock, but media may access directly. */
	
	// Filehandle lists
	collections::list<filehandle *> m_pFreeFilehandles;       /**< Free filehandles, available for allocation. */
	collections::list<filehandle *> m_pAllocatedFilehandles;  /**< Allocated filehandles. Access protected by m_objectLock. */
	uint32_t m_fhCount;                               /**< Active filehandle count. Access protected by m_objectLock. */
	uint32_t m_fhTotal;                               /**< Number of filehandles opened in total. */
	uint32_t m_fhHighWaterMark;                       /**< Highest number of filehandles open at any one time. Access protected by m_objectLock. */
		
	/** \brief Action that can be used to cancel ops. */
	static void cancelOpAction(void *, op *pOp);

	/** \brief Allocates a filehandle object.
		\param[in]  flags   Flags for the object.
		\param[in]  pPath   Path for the object to store.
		\return Newly-allocated filehandle, or NULL.
	*/
	filehandle * allocateFH(
		uint32_t flags,
		const char *pPath);
	
	/** \brief Deallocates a filehandle object.
		\param[in]  pFH    Filehandle to delete.
	*/
	void deallocateFH(
		filehandle *pFH);

	/** \brief Entrypoint for the scheduler thread.
		\param[in]  pThread    Thread pointer.
		\param[in]  pArg       Argument, which is a scheduler instance pointer.
	*/
	static void threadEntry(
		platform::thread *pThread,
		void *pArg);
	
	/** \brief Entrypoint for the media worker threads.
		\param[in]  pThread    Thread pointer.
		\param[in]  pArg       Argument, which is a scheduler instance pointer.
	*/
	static void mediathreadEntry(
		platform::thread *pThread,
		void *pArg);

	/** \brief Whether the scheduler thread has work to do.
		\return True if there is work to do, false otherwise. */
	bool schedulerShouldRun();
	
	/** \brief Issues a new I/O for each free slot in the concurrency list. */
	void dispatchNewRequests();
	
	/** \brief Runs the scheduling algorithm globally on the entire queue. */
	void reorderQueue();
	
	/** \brief Processes the immediate cancellation of a list of ops.

		m_opLock must be held, but may be unlocked during processing. The ops will be moved directly into the completed queue (or deleted).
		\param[in] pOpList       List of ops to cancel. */
	void cancelImmediately(
		op *pOpList);
	
	/** \brief Accumulates a batch of PCE coefficients from a completed I/O into our estimate
		\param[in] pCoefficients Coefficients from the completed I/O
		\param[in] elapsedTime   Time elapsed during the I/O
	*/
	void
	accumulatePCECoefficients(
		const pcecoefficient_t *pCoefficients,
		abstime_t elapsedTime);
	
	/** \brief Advances an op forward one stage.
	    \param[in] pOp         Operation pointer.
		\param[in] pRequest    The struct mediarequest from the previous stage of the operation.
	*/
	void
	stageCompleteOp(
		op *pOp,
	    mediarequest *pRequest);
	
	/** \brief Utility function used internally to peek and see if an op will be a cache hit.
		\param[in] pOp         Operation pointer.
		\result                True if the op will hit in the cache, false otherwise. */
	bool
	isCacheHit(op *pOp);

	/** \brief Executes a request.
		\param[in]     pOp         Operation pointer.
		\param[in,out] pIOP        A struct mediaioparams to be used for execution. Normally from m_pIOPs.
	*/
	void
	dispatchRequest(
		op *pOp,
		mediaioparams *pIOP);
	
	/** \brief Callback that media driver invokes after operation is complete.
		\param[in]    pContext     Context, in this case a pointer to the scheduler object.
		\param[in]    pMedia       Media object that issued the callback.
		\param[in]    event        Event that triggered the callback, in this case kMEDIAEVENT_IOCOMPLETE.
		\param[in]    pParam       Event parameter, in this case a pointer to a mediaioparams struct.
		\syncreturn
	*/
	static err_t
	requestCallback(
		void *pContext,
		media *pMedia,
		mediaevent_t event,
		void *pParam);
	
	/** \brief Moves an op to the schedule queue.
		\param[in] pOp   Operation to schedule.
		This O(n) routine walks the entire queue of I/O, simulating I/O and looking for
		the best spot for the incoming op.
	*/
	void
	scheduleOp(
		op *pOp);
	
	/** \brief Speculatively executes an op.
		\param[in]     pOp           Operation to speculatively execute.
		\param[in,out] pP            Priority of the highest-priority op whose deadline has been violated so far. May be NULL.
		\param[in,out] pN            Number of ops of priority (*pP) whose deadlines have been violated so far. May be NULL.
		\param[in,out] pT            Time, updated by this call.
		\param[in,out] pMediaState   Media state, updated by this call.
		\return TRUE if the op failed to meet its deadline.
	*/
	bool
	speculateOp(
		op *       pOp,
		prio_t *   pP,
		uint32_t *      pN,
		abstime_t *pT,
		void *pMediaState);
	
	/** \brief Scheduler thread.
		This function contains the scheduler's main thread loop.
	*/
	void thread();

	/** \brief Media worker threads.
		This function contains the loop for the worker thread(s).
	*/
	void mediathread(
		platform::thread *pThread);
#endif // DOXYGEN_IGNORE
};

#ifndef DOXYGEN_IGNORE
/* Some additional inlines are listed here due to circular dependencies. */
inline void      op::cancel() { return m_pScheduler->cancelOp(this); }
inline void      op::operator delete(void *pMem) { op *pOp = reinterpret_cast<op*>(pMem); pOp->m_pScheduler->deleteOp(pOp); }
inline abstime_t op::getEstimatedCompletion() const { return m_pScheduler->getEstimatedCompletion(this); }
inline void      op::setDeadline(abstime_t deadline) { m_pScheduler->rescheduleOp(this, deadline, getPriority()); }
inline void      op::setPriority(prio_t priority) { m_pScheduler->rescheduleOp(this, getDeadline(), priority); }
inline void      op::setDeadlineAndPriority(abstime_t deadline, prio_t priority) { m_pScheduler->rescheduleOp(this, deadline, priority); }
#endif // DOXYGEN_IGNORE

}; /* namespace fios */
}; /* namespace cell */


#endif /* __CELL_FIOS_SCHEDULER_H__ */

