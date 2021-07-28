/*
	SCE CONFIDENTIAL
	PlayStation(R)3 Programmer Tool Runtime Library 310.001
	Copyright (C) 2009 Sony Computer Entertainment Inc.
	All Rights Reserved.
*/
/**
	\file fios_caching.h

	Class definitions for FIOS cache layers. This includes RAM caches and disk-based
	caches. 
*/
#ifndef __CELL_FIOS_CACHING_H__
#define __CELL_FIOS_CACHING_H__

#include <cell/fios/fios_types.h>
#include <cell/fios/fios_base.h>
#include <cell/fios/fios_platform.h>
#include <cell/fios/fios_paths.h>

namespace cell {
namespace fios {

class cache;
class ramcache;
class schedulercache;

#ifndef DOXYGEN_IGNORE
// Turn on or off experimental optimization that involves
// freeing up several blocks in a burst.
#ifndef FIOS_USE_BATCH_FREE_OPT
	#define FIOS_USE_BATCH_FREE_OPT   (0)
#endif

// Turn on or off experimental optimization that involves
// marking modified disk blocks as dirty and updating
// index in a burst.
#ifndef FIOS_USE_DIRTY_BLOCK_OPT
	#define FIOS_USE_DIRTY_BLOCK_OPT  (0)
#endif
#endif // DOXYGEN_IGNORE

/**
	\brief A generic cache layer.
	This layer is a shared base class for both RAM caches and disk-based caches.
*/
class FIOS_LINKAGE cache : public mediafilter
{
public:
#ifndef DOXYGEN_IGNORE
	/** \internal
		\brief In-memory cache block descriptor. */
	typedef struct descriptor {
		datetime_t accessDate;       //!< Last time this block was accessed
		datetime_t modificationDate; //!< Modification date on the original file
		datetime_t creationDate;     //!< Creation date on the original file
		off_t      offset;           //!< Offset of this block in the file
		uint32_t        persistent : 1;   //!< Persistence flag (only used by disk caches).
		uint32_t        dirty : 1;        //!< Dirty flag set if memory descriptor does not match disk descriptor. (only used by disk caches).
		uint32_t        dataLength : 30;  //!< Normally m_blockSize, may be less for blocks at the end of the file
		md5path    path;             //!< Path handle for the file we're caching
	} descriptor;

	/** \brief Cache block index. */
	typedef int32_t index_t;
	
	/** \brief Cache index max value. */
	#define FIOS_CACHE_INDEX_T_MAX           int32_t(0x7FFFFFFFL)
	
	/** \internal
		\brief Cache block request.

		Every incoming mediarequest is broken down into zero or more blockrequests. A block request is
		always aligned with the start of the file. */
	typedef struct blockrequest {
		const char *pPath;           //!< File, or NULL for block access
		off_t       offset;          //!< Offset inside file/disk (used to look up block in index)
		size_t      offsetInBlock;   //!< Offset into the block
		size_t      reqCount;        //!< Length desired
		size_t      actCount;        //!< Length actually provided
	} blockrequest;
#endif // DOXYGEN_IGNORE

	FIOS_OBJECT_NEW_AND_DELETE(object,kMemPurposeFIOSMediaObject|kPoolIDLongTerm)
	
protected:	
	/**
		\brief Constructor for the cache filter layer.

		This class is abstract and you cannot instantiate it directly. You may either subclass
		it yourself, or use the built-in concrete subclasses #cell::fios::ramcache and #cell::fios::schedulercache.
		
		The cache layer is able to hold a maximum of (numBlocks * blockSize) bytes of data.
		In the current implementation read requests are typically made in single blocks, so the
		block size should be relatively large -- at least 64K.
		
		Performance tips:
		 * Block sizes should be multiples of 2048 bytes (one Blu-ray sector).
		 * For best performance, don't let the number of blocks get too large.
		 
		If you want to create a 1MiB RAM cache, try 16 x 64KiB blocks. If you want a 2GiB disk cache,
		try 2048 x 1MiB blocks.
		
		This class functions as a writethrough
		cache -- any writes to the media will invalidate the appropriate cache block(s) and then
		get passed through. The policy for read caching is determined by each individual
		subclass.
		
		\param[in]   pNextMedia    Next media in the stack.
		\param[in]   numBlocks     Number of blocks to keep in the cache.
		\param[in]   blockSize     Size of each block. Usually a multiple of 2048, with a recommended minimum of 64K.
	*/
	cache(media *pNextMedia, index_t numBlocks, size_t blockSize);
	
	/** \brief Destructor for the cache filter layer. */
	virtual ~cache();
	
public:
#ifndef DOXYGEN_IGNORE
	/** \brief Starts an I/O operation.
		\param[in,out] pIOP           I/O parameters describing the operation.
		\see media::executeIOP */
	virtual void executeIOP(
		mediaioparams *pIOP);
	
	/** \brief Immediately returns the predicted PCE coefficients for an I/O request.
		\param[in,out] pIOP           I/O parameters describing the operation.
		\see media::speculateIOP */
	virtual void speculateIOP(
		mediaioparams *pIOP);
#endif // DOXYGEN_IGNORE
	
	/** \brief Flushes the cache. Any stored data is invalidated. */
	virtual void flush();
	
	/** \brief Partially flushes the cache to free up a requested amount of space.

		This call is most useful for segmented caches, such as the scheduler cache when
		configured with singleFile = false. In that situation it can be used to free up
		space on the cache media.
		\param[in]   bytesToFree       Number of bytes to free. */
	virtual void flushPartial(off_t bytesToFree);
	
	/** \brief Flushes a specified file from the cache.

		This call can be used when you know (via some external agency) that a file has
		changed and it needs to be reloaded from the media.
		\param[in]   pPath             Path to file to flush from the cache. */
	virtual void flushFile(const char *pPath) { flushFilePartial(pPath, 0, FIOS_OFF_T_MAX); }
	
	/** \brief Flushes part of a specified file from the cache.

		This call can be used when you know (via some external agency) that part of a file has
		changed and it needs to be reloaded from the media.
		\param[in]   pPath             Path to file to flush from the cache.
		\param[in]   offset            Offset within the file to flush.
		\param[in]   length            Number of bytes from the offset to flush. */
	virtual void flushFilePartial(const char *pPath, off_t offset, off_t length);
	
	/** \brief Indicates whether the cache already contains the data for a given request.

		Because of the dynamic nature of the cache the answer may change as more I/O
		is issued. It's possible that this function could return TRUE and yet the data
		could be evicted by the time you try to read it, or vice-versa.
		\note This function is synchronous and may block briefly there is I/O running.
		\note Side-effect: This logs an "access" to any cache blocks containing the data, moving them to the end of the LRU ordering and delaying their eviction.
		\param[in]   pPath             Path to file to query.
		\param[in]   offset            Offset within the file to query.
		\param[in]   len               Number of bytes from the offset that we're asking about.
		\result True if the data is in the cache, false otherwise. */
	bool isRequestInCache(const char *pPath, off_t offset, off_t len);

	/** \brief Indicates whether the cache already contains data for a given mediarequest.

		Because of the dynamic nature of the cache the answer may change as more I/O
		is issued. It's possible that this function could return TRUE and yet the data
		could be evicted by the time you try to read it, or vice-versa.
		\note This function is synchronous and may block briefly there is I/O running.
		\note Side-effect: This logs an "access" to any cache blocks containing the data, moving them to the end of the LRU ordering and delaying their eviction.
		\param[in]   pRange      filerange_t describing the query.
		\result True if the request is entirely in the cache, false otherwise. */
	bool isRequestInCache(filerange_t *pRange);
	
#ifndef DOXYGEN_IGNORE
	/** \brief Dumps cache hit stats to stdout.

		This function is automatically called by the media object destructor, but can be called manually if desired.
		\param[in]   pName             What to call the cache in the output.
	*/
	void dumpCacheStats(const char *pName);
	
	/** \brief Changes whether FIOS checks for stale data or not.

		With this flag set, FIOS does a (potentially expensive) modification-date check whenever a
		file is opened to check for stale data. It's useful during development, because then the cache
		automatically picks up any changed files on the host; but should normally be disabled when
		running from optical disc.
		\param[in]   doCheck   If true, check modification dates on file open. If false, assume the source data hasn't changed.
		\return Returns the previous setting.
		\see getCheckModification
	*/
	inline bool setCheckModification(bool doCheck) { bool oldCheck = m_checkModification; m_checkModification = doCheck; return oldCheck; }

	/** \brief Returns whether FIOS checks for stale data or not.

		This function indicates whether FIOS checks for stale data by doing a (potentially expensive)
		modification-date check. 
		\return Returns the value of checkModification.
		\see setCheckModification
	*/
	inline bool getCheckModification() { return m_checkModification; }

	/** \brief Changes whether FIOS warns about a mod date change or not.

		By default, if your cache has "checkModification" turned on, the cache layer will log to the
		console when it ignores stale data in the cache. This function allows you to modify that behavior.
		\note This function only affects whether FIOS logs a warning to the console, and does not change the checkModification flag.
		\param[in]   doWarn            If true, warn about mod date changes. If false, be quiet.
		\return  Returns the previous warning setting.
		\see getWarnOnStaleData
	*/
	inline bool setWarnOnStaleData(bool doWarn) { bool oldWarn = m_warnModChanged; m_warnModChanged = doWarn; return oldWarn; }

	/** \brief Returns whether FIOS warns about a mod date change.

		This function indicates whether FIOS will warn in the console about stale modification dates
		when "checkModification" is on.
		\note This is only for the log message, and is not the same as the "checkModification" flag.
		\return  Returns the value of the warning setting.
		\see setWarnOnStaleData
	*/
	inline bool getWarnOnStaleData() const { return m_warnModChanged; }
	
	/** \brief Returns the cache block size.
		\return Size of a single cache block, in bytes.
		\see getBlockCount */
	inline size_t getBlockSize() const { return m_blockSize; }
	
	/** \brief Returns the number of blocks in the cache.
		\return Number of blocks in the cache.
		\see getBlockSize */
	inline index_t getBlockCount() const { return m_numBlocks; }
	
protected:
	/** \internal */
	/*@{*/
	platform::rwlock m_rwLock;      //!< rwlock that controls access to the cache and most member variables.
	platform::mutex  m_fillFunnel;  //!< This makes sure we only run one cache-fill at a time.
	bool             m_disabled;    //!< Disabled state
	bool             m_prefetch;    //!< Whether this layer supports prefetch requests.
	bool             m_checkModification; //!< Indicates whether we keep checking the modification dates
	bool             m_warnModChanged; //!< Whether to warn when a cache block is disregarded because of a mod date change
	size_t           m_blockSize;   //!< Size of a cache block
	index_t          m_numBlocks;   //!< Number of cache blocks
	descriptor *     m_index;       //!< Array of descriptor[m_numBlocks]
	bool             m_loadedIndex; //!< Whether we've loaded the index yet.
	platform::atomicInt<index_t> m_lastLRU; //!< The last LRU index that we returned.
	platform::atomicU32         m_cacheHitCount;  //!< The number of cache hits
	platform::atomicInt<off_t>  m_cacheHitBytes;  //!< The total size, in bytes, of the cache hits
	platform::atomicU32         m_cacheMissCount; //!< The number of cache misses
	platform::atomicU32         m_cacheFillCount; //!< The number of cache misses that resulted in a fill
	platform::atomicInt<off_t>  m_cacheFillBytes; //!< The total size, in bytes, of cache misses that resulted in a fill
	
	/** \internal
		\brief Gets the mod/creation dates from the cache, if present.

		A read or write lock must be held.
		\param[in]   pPath          The path to search for.
		\param[out]  pModDate       Filled in with the file's last seen modification date, if it's in the cache, otherwise 0.
		\param[out]  pCreateDate    Filled in with the file's last seen creation date, if it's in the cache, otherwise 0.
	*/
	void getDatesFromCache(const char *pPath, datetime_t *pModDate, datetime_t *pCreateDate)
		FIOS_ATTRIBUTE_GCC((__nonnull__));
	
	/** \internal
		\brief Counts the number of cache block requests that comprise an incoming request.

		No locks are needed.
		\param[in]   pRequest       The incoming request.
		\return Number of block requests, zero or more. */
	index_t countBlocksInRequest(const mediarequest *pRequest)
		FIOS_ATTRIBUTE_GCC((__nonnull__));
	
	/** \internal
		\brief Breaks up an incoming request into cache block requests.

		No locks are needed.
		\param[in]   pRequest       The incoming request.
		\param[in]   n              The portion of the request, which should be >= 0 and < countBlocksInRequest(pRequest).
		\param[out]  pBlockRequest  Filled in with the block request upon successful completion.
		\return Returns pBlockRequest as a convenience. */
	blockrequest * getNthBlockOfRequest(const mediarequest *pRequest, index_t n, blockrequest *pBlockRequest)
		FIOS_ATTRIBUTE_GCC((__nonnull__));
	
	/** \internal
		\brief Returns the block index for this request, if it's in the cache and not busy.

		Caller must hold a read OR write lock.
		\param[in]  pBlockRequest    Block request from the client request.
		\return If the block is in the cache, a valid non-negative index. If the block is not in the cache, the return value is -1. If the block is in the cache but busy, the return value is -2. */
	index_t lookup(blockrequest *pBlockRequest)
		FIOS_ATTRIBUTE_GCC((__nonnull__));
	
	/** \internal
		\brief Returns the block index for this request, if it's in the cache.

		Caller must hold a read lock. If the block is busy, this function will delay until it's no longer busy.
		\param[in]  pBlockRequest    Block request from the client request.
		\return If the block is in the cache, a valid non-negative index. If the block is not in the cache, the return value is -1. */
	index_t lookupForRead(blockrequest *pBlockRequest)
		FIOS_ATTRIBUTE_GCC((__nonnull__));
	
	/** \internal
		\brief Returns the block index of the least-recently-used block in the cache.

		Caller must hold a read OR write lock. Persistent blocks are not returned unless every block
		is either persistent or busy.
		\param[in]  wantEmpty     If true, the first empty block found (if any) is returned. If false, empty blocks are not returned.
		\return Index of the LRU block that fits all criteria, or an arbitrary but valid index if none were found. */
	index_t getLRU(bool wantEmpty);
	
	/** \internal
		\brief Called to trigger loading of the cache index for a persistent cache such as a disk cache.

		Caller must hold a write lock. */
	virtual void loadIndex() {}
	
	/** \internal
		\brief Called to trigger saving of the cache index for a persistent cache such as a disk cache.

		Caller must hold a write lock. */
	virtual void saveIndex() {}
	
	/** \internal
		\brief Invalidates the specified block from the cache.

		Caller must hold a write lock.
		\param[in]  blockIndex     Block to invalidate.
		\param[in]  reason         Text string identifying the caller (for profile output). */
	virtual void invalidateBlock(index_t blockIndex, const char *reason);
	
	/** \internal
		\brief Called to trigger a write of the specified block's information out to persistent storage, if applicable.

		Caller must hold a read OR write lock.
		\param[in]  blockIndex     Block to update. */
	virtual void updateBlock(index_t blockIndex) { FIOS_UNUSED(blockIndex); }
	
	/** \internal
		\brief Determines the cache strategy by deciding whether a given block request should be cached.

		Caller must hold a read lock.
		\param[in]  pRequest      The original client request.
		\param[in]  i             Which block of the request is being considered for caching, where 0 <= i < n.
		\param[in]  n             The total number of blocks in this request.
		\param[in]  pBlockRequest The block request under consideration.
		\return True if the block should be cached, false if it's better to just read through. */
	virtual bool shouldCache(mediarequest *pRequest, index_t i, index_t n, blockrequest *pBlockRequest) = 0;
	
	/** \internal
		\brief Fills the cache with a given block, and copies out the data to the client.

		Caller does not hold ANY locks; implementation must acquire write locks when necessary.
		\param[in]  pIOP          The current mediaioparams, used in any I/O to the next media.
		\param[in]  pBlockRequest The block request.
		\param[in]  pDstBuffer    A pointer to the client's buffer, adjusted so that pDstBuffer[0] corresponds to the first byte of the block.
		\return   True if the fill-and-copyout succeeded, false otherwise. Returning false causes the cache to attempt a readthrough. */
	virtual bool fillCacheAndCopyOut(mediaioparams *pIOP, blockrequest *pBlockRequest, uint8_t *pDstBuffer) = 0;
	
	/** \internal
		\brief Updates a cache block with the specified data from a client buffer.

		Caller must hold a write lock.
		\note This function is not currently used! It may eventually be used if the cache becomes more than a simple writethrough.
		\param[in]  pIOP          The current mediaioparams, used in any I/O to the next media.
		\param[in]  blockIndex    Index of the block to update.
		\param[in]  pSrcBuffer    A pointer to the client's buffer, adjusted so that pSrcBuffer[0] corresponds to the first byte of the block.
		\param[in]  pBlockRequest The block request.
		\return    True if the set operation succeeded, false otherwise. Returning false causes the cache to invalidate the block. */
	virtual bool setCacheBlock(mediaioparams *pIOP, index_t blockIndex, const uint8_t *pSrcBuffer, blockrequest *pBlockRequest) = 0;
	
	/** \internal
		\brief Reads data from a cache block into a client buffer.

		Caller must hold a read lock.
		\param[in]  pIOP          The current mediaioparams, used in any I/O to the next media.
		\param[in]  blockIndex    Index of the block to retrieve.
		\param[in]  pDstBuffer    A pointer to the client's buffer, adjusted so that pDstBuffer[0] corresponds to the first byte of the block.
		\param[in]  pBlockRequest The block request.
		\return    True if the get operation succeeded, false otherwise. Returning false causes the cache to attempt a readthrough. */
	virtual bool getCacheBlock(mediaioparams *pIOP, index_t blockIndex, uint8_t *pDstBuffer, blockrequest *pBlockRequest) = 0;

#endif // DOXYGEN_IGNORE

	/*@}*/
}; /* class cache */


/**
	\brief A simple RAM cache.
	This layer is designed to cache read requests in RAM. It uses a generic policy which should
	work reasonably well for many clients.
*/
class FIOS_LINKAGE ramcache : public cache
{
public:
	/** \brief Constructor. 
	
		The RAM cache occupies (numBlocks * blockSize) bytes of memory for data
		storage, plus numBlocks * sizeof(cache::descriptor) for the index.

		Here are some example RAM cache parameters, as numBlocks x blockSize:
		 - 512KiB RAM cache: 16 x 32KiB
		 - 1MiB RAM cache:   16 x 64KiB
		 - 2MiB RAM cache:   16 x 128KiB	

		See the base class constructor, cache::cache(), for more information about cache performance.

		\param[in]   pNextMedia     Next media in the stack.
		\param[in]   numBlocks      Number of cache blocks.
		\param[in]   blockSize      Size of each cache block.
		\see cache::cache()
	*/
	ramcache(media *pNextMedia, index_t numBlocks, size_t blockSize);
	
	/** \brief Destructor. */
	virtual ~ramcache();

	FIOS_OBJECT_NEW_AND_DELETE(ramcache,kMemPurposeFIOSMediaObject|kPoolIDLongTerm)

protected:
#ifndef DOXYGEN_IGNORE
	/** \internal */
	/*@{*/
	/** \internal
		\brief Determines the cache strategy by deciding whether a given block request should be cached.

		The goal of the default RAM cache is to provide read-clustering, where a series of small reads will
		turn into one large read. To achieve this, we implement a policy where all blocks of small requests
		(n<2) are cached. For larger requests, only the last block will be cached, and then only if it is
		less than a full cache block in size. Doing so keeps us from evicting too much data from the cache
		when a large request comes through.
		\param[in]  pRequest      The original client request.
		\param[in]  i             Which block of the request is being considered for caching, where 0 <= i < n.
		\param[in]  n             The total number of blocks in this request.
		\param[in]  pBlockRequest The block request under consideration.
		\return True if the block should be cached, false if it's better to just read through.
		\see cache::shouldCache */
	virtual bool shouldCache(mediarequest *pRequest, index_t i, index_t n, blockrequest *pBlockRequest);
	
	/** \internal
		\brief Fills the cache with a given block, and copies out the data to the client.

		Caller does not hold ANY locks; implementation must acquire write locks when necessary.
		\param[in]  pIOP          The current mediaioparams, used in any I/O to the next media.
		\param[in]  pBlockRequest The block request.
		\param[in]  pDstBuffer    A pointer to the client's buffer, adjusted so that pDstBuffer[0] corresponds to the first byte of the block.
		\return   True if the fill-and-copyout succeeded, false otherwise. Returning false will cause the cache to attempt a readthrough.
		\see cache::fillCacheAndCopyOut */
	virtual bool fillCacheAndCopyOut(mediaioparams *pIOP, blockrequest *pBlockRequest, uint8_t *pDstBuffer);

	/** \internal
		\brief Updates a cache block with the specified data from a client buffer.

		Caller must hold a write lock.
		\note This function is not currently used! It may eventually be used if the cache becomes more than a simple writethrough.
		\param[in]  pIOP          The current mediaioparams, used in any I/O to the next media.
		\param[in]  blockIndex    Index of the block to update.
		\param[in]  pSrcBuffer    A pointer to the client's buffer, adjusted so that pSrcBuffer[0] corresponds to the first byte of the block.
		\param[in]  pBlockRequest The block request.
		\return    True if the set operation succeeded, false otherwise. Returning false will cause the cache to invalidate the block.
		\see cache::setCacheBlock */
	virtual bool setCacheBlock(mediaioparams *pIOP, index_t blockIndex, const uint8_t *pSrcBuffer, blockrequest *pBlockRequest);

	/** \internal
		\brief Reads data from a cache block into a client buffer.

		Caller must hold a read lock.
		\param[in]  pIOP          The current mediaioparams, used in any I/O to the next media.
		\param[in]  blockIndex    Index of the block to retrieve.
		\param[in]  pDstBuffer    A pointer to the client's buffer, adjusted so that pDstBuffer[0] corresponds to the first byte of the block.
		\param[in]  pBlockRequest The block request.
		\return    True if the get operation succeeded, false otherwise. Returning false will cause the cache to attempt a readthrough.
		\see cache::getCacheBlock */
	virtual bool getCacheBlock(mediaioparams *pIOP, index_t blockIndex, uint8_t *pDstBuffer, blockrequest *pBlockRequest);

	uint8_t *   m_cacheBuffer;
	/*@}*/
#endif // DOXYGEN_IGNORE
}; /* class ramcache */

/**
	\brief A simple disk-to-disk cache.
	This layer is designed to cache read requests to another piece of media. It uses a generic policy
	which should work reasonably well for many clients. You may wish to create your own subclass with
	a policy that is tuned more specifically to the needs of your game.
*/
class FIOS_LINKAGE schedulercache : public cache
{
public:
	/** \brief Constructor. 
		
		The scheduler cache can be kept as a single file or as multiple files, as decided by the
		singleFile parameter. The benefit of a multiple-file cache is that it can be partially
		flushed to intelligently free up disk space when needed without sacrificing too much data.
		Single-file caches can only be flushed completely.
		
		On the other hand, single-file caches are generally higher performance than multi-file caches.
		This is because the multi-file cache requires additional bookkeeping work (inside the host
		filesystem itself), and that typically requires extra I/O.
		
		Cache blocks encode a disc ID to ensure that the data is coming from the right media. This
		disc identifier should be unique to your title and disc. If you are writing a multi-disc
		game, each disc should be assigned a separate unique ID.
		
		See the base class constructor, cache::cache(), for more information about cache performance.
		\param[in]  pNextMedia        Next media in the media stack.
		\param[in]  pFasterScheduler  Faster scheduler on which the disk cache is kept.
		\param[in]  pCacheDirectory   Directory to hold the cache files
		\param[in]  discID            Unique 64-bit identifier for this game disc.
		\param[in]  singleFile        If true, the cache data is kept as a single file: faster, but no partial flushing. If false, the cache data is kept as one file per block: supports partial flushing, but slightly slower. If you're not sure, set this flag to TRUE.
		\param[in]  checkModification If true, the cache issues extra I/O to check the original file modification date against the cache contents. This is useful during development, but should be disabled for the final product.
		\param[in]  numBlocks         Number of cache blocks.
		\param[in]  blockSize         Size of each cache block. */
	schedulercache(
		media *pNextMedia,
		scheduler *pFasterScheduler,
		const char *pCacheDirectory,
		uint64_t discID,
		bool singleFile,
		bool checkModification,
		index_t numBlocks,
		size_t blockSize);
	
	/** \brief Destructor */
	virtual ~schedulercache();

	/** \brief Flushes the cache. Any stored data is invalidated. */
	virtual void flush();

	FIOS_OBJECT_NEW_AND_DELETE(schedulercache,kMemPurposeFIOSMediaObject|kPoolIDLongTerm)
	
#if FIOS_USE_BATCH_FREE_OPT
	void setMaxBatchFree(int32_t max)        { m_maxBatchFree = max; }
	int32_t getMaxBatchFree()                  { return m_maxBatchFree; }
	void setDeleteEmptyBlocks(bool flag) { m_deleteEmptyBlocks = flag; }
	int32_t getDeleteEmptyBlocks()             { return m_deleteEmptyBlocks; }
#endif

#if FIOS_USE_DIRTY_BLOCK_OPT
	/** \brief m_maxDirtyBlocks default value. */
	#define FIOS_CACHE_MAX_DIRTY_BLOCKS           16
	void setMaxDirtyBlocks(int32_t max)      { m_maxDirtyBlocks = max; }
	int32_t getMaxDirtyBlocks()                { return m_maxDirtyBlocks; }
#endif

protected:
#ifndef DOXYGEN_IGNORE
	/** \internal */
	/*@{*/
	static const uint64_t kCacheSignature;       //!< \internal Cache signature. "fioscach" in ASCII.
	
	enum {
		kCacheVersion      = 3,                            //!< Cache version, incremented every time the file format changes.
		kIOBufferSize      = 64*1024,                      //!< Size of each I/O buffer. We chose 64K buffers because this is a sweet spot on many systems, where extra memory costs start to be balanced by diminishing returns.
		kIOBufferAlignment = 12                            //!< Alignment (log 2) of each I/O buffer. We chose 4096-byte alignment (2^12) because it's a typical VM page alignment and is therefore a sweet spot on many systems.
	};
	
	/** \internal
		\brief Header for both the cache index, and the monolithic cache file (if singleFile = true). */
	typedef struct fileheader {
		uint64_t   signature;            //!< Set to kCacheSignature
		uint64_t   discID;               //!< Disc ID that this cache represents
		uint16_t   version;              //!< Set to kCacheVersion
		uint16_t   headerSize;           //!< Set to sizeof(fileheader)
		uint16_t   descriptorSize;       //!< Set to sizeof(diskdescriptor)
		uint16_t   pathMax;              //!< Set to FIOS_PATH_MAX
		uint32_t   numBlocks;            //!< Number of blocks in the cache
		uint32_t   blockSize;            //!< Size of each cache block
		uint8_t    singleFile;           //!< Corresponds to the singleFile parameter to the constructor.
		uint8_t    reserved[7];          //!< Reserved for future expansion
		uint32_t   knownFlags;           //!< List of known flags: all values from #e_DISKDESCRIPTORFLAGS
	} FIOS_ATTRIBUTE((__packed__)) fileheader;
	
	/** \internal
		\brief Disk-based version of a cache block descriptor.

		All elements are stored in network byte order, and each descriptor is padded to be a single sector. */
	typedef struct diskdescriptor {
		datetime_t accessDate;       //!< Last time this block was accessed
		datetime_t modificationDate; //!< Modification time of the original file
		datetime_t creationDate;     //!< Creation time of the original file
		int64_t offset;                  //!< Offset of this block in the file
		uint32_t dataLength;              //!< Normally m_blockSize. May be less for blocks at the end of the file
		uint32_t flags;                   //!< Flags for this cache block. See #e_DISKDESCRIPTORFLAGS
		uint64_t md5[2];                  //!< MD5 of path to this file
	} FIOS_ATTRIBUTE((__packed__)) diskdescriptor;
	
        /** \internal */
	enum e_DISKDESCRIPTORFLAGS {
		kCachePersist    = (uint32_t)(1<<0)    //!< \internal A block with this flag is "persistent": It will not be evicted from the cache to make room for other data, and will not be flushed by a flushPartial() call. It can still be explicitly removed via flush(), flushFile(), and flushFilePartial().
	};
	
	/** \internal
		\brief Called to trigger loading of the cache index.

		Caller must hold a write lock. */
	virtual void loadIndex();
	
	/** \internal
		\brief Called to trigger a write of the specified block's information out to persistent storage.

		Caller must hold a read OR write lock.
		\param[in]  blockIndex     Block to update. */
	virtual void updateBlock(index_t blockIndex);
	
	/** \internal
		\brief Called to trigger saving of the cache index for a persistent cache such as a disk cache.

		Caller must hold a write lock. */
	virtual void saveIndex();

	/** \internal
		\brief Determines the cache strategy by deciding whether a given block request should be cached.

		The policy of the scheduler cache is opposite that of the RAM cache; the policy is to cache ALL data
		so that the next time it's accessed 
		\param[in]  pRequest      The original client request.
		\param[in]  i             Which block of the request is being considered for caching, where 0 <= i < n.
		\param[in]  n             The total number of blocks in this request.
		\param[in]  pBlockRequest The block request under consideration.
		\return True if the block should be cached, false if it's better to just read through. */
	virtual bool shouldCache(mediarequest *pRequest, index_t i, index_t n, blockrequest *pBlockRequest);
	
	/** \internal
		\brief Fills the cache with a given block, and copies out the data to the client.

		Caller does not hold ANY locks; implementation must acquire write locks when necessary.
		\param[in]  pIOP          The current mediaioparams, used in any I/O to the next media.
		\param[in]  pBlockRequest The block request.
		\param[in]  pDstBuffer    A pointer to the client's buffer, adjusted so that pDstBuffer[0] corresponds to the first byte of the block.
		\return   True if the fill-and-copyout succeeded, false otherwise. Returning false will cause the cache to attempt a readthrough.
		\see cache::fillCacheAndCopyOut */
	virtual bool fillCacheAndCopyOut(mediaioparams *pIOP, blockrequest *pBlockRequest, uint8_t *pDstBuffer);

	/** \internal
		\brief Updates a cache block with the specified data from a client buffer.

		Caller must hold a write lock.
		\note This function is not currently used! It may eventually be used if the cache becomes more than a simple writethrough.
		\param[in]  pIOP          The current mediaioparams, used in any I/O to the next media.
		\param[in]  blockIndex    Index of the block to update.
		\param[in]  pSrcBuffer    A pointer to the client's buffer, adjusted so that pSrcBuffer[0] corresponds to the first byte of the block.
		\param[in]  pBlockRequest The block request.
		\return    True if the set operation succeeded, false otherwise. Returning false will cause the cache to invalidate the block.
		\see cache::setCacheBlock */
	virtual bool setCacheBlock(mediaioparams *pIOP, index_t blockIndex, const uint8_t *pSrcBuffer, blockrequest *pBlockRequest);

	/** \internal
		\brief Reads data from a cache block into a client buffer.

		Caller must hold a read lock.
		\param[in]  pIOP          The current mediaioparams, used in any I/O to the next media.
		\param[in]  blockIndex    Index of the block to retrieve.
		\param[in]  pDstBuffer    A pointer to the client's buffer, adjusted so that pDstBuffer[0] corresponds to the first byte of the block.
		\param[in]  pBlockRequest The block request.
		\return    True if the get operation succeeded, false otherwise. Returning false will cause the cache to attempt a readthrough.
		\see cache::getCacheBlock */
	virtual bool getCacheBlock(mediaioparams *pIOP, index_t blockIndex, uint8_t *pDstBuffer, blockrequest *pBlockRequest);
	
	uint64_t     m_discID;            //!< Disc ID that this cache represents
	bool         m_singleFile;        //!< Indicates whether this is a single-file cache or a multi-file cache
	scheduler *  m_pFasterScheduler;  //!< Target scheduler that holds the cache files
	staticpath_t m_cacheDirectory;    //!< Cache directory that holds the cache files
	filehandle * m_pCacheIndex;       //!< Cache index file.
	filehandle * m_pCacheFile;        //!< Used when configured for a single cache file
	size_t       m_sectorSize;        //!< Native sector size of the media (on the OTHER scheduler)
	
	platform::mutex m_bufferLock;     //!< I/O buffer lock. If code is going to hold a write lock on m_rwLock as well as this lock, the write lock should always be acquired first!
	uint8_t *    m_pIOBuffer[2];   //!< I/O buffers, access protected by m_bufferLock
	
	/** \internal
		\brief Rounds an incoming size upward to the next multiple of another size.
		\param[in] inSize      The size to round.
		\param[in] multipleOf  The size that we need a multiple of.
		\return The first multiple of multipleOf that is greater than or equal to inSize. */
	static inline size_t roundToMultiple(size_t inSize, size_t multipleOf) { return multipleOf * ((inSize+multipleOf-1)/multipleOf); }
	
	/** \internal
		\brief Rounds an incoming size to the cache media's sector size.
		\param[in] inSize      The size to round.
		\return The first multiple of the cache media's sector size that is greater than or equal to inSize. */
	inline size_t roundToSectorSize(size_t inSize) const { return roundToMultiple(inSize, m_sectorSize); }

	/** \internal
		\brief Gets the desired file header size.
		\return The file header size, which will be an exact multiple of the sector size of the cache media. */
	inline size_t getFileHeaderSize() const { return roundToSectorSize(roundToMultiple(sizeof(fileheader),kIOBufferAlignment)); }

	/** \internal
		\brief Gets the desired disk descriptor size.
		\return The disk descriptor size, which will be an exact multiple of the sector size of the cache media. */
	inline size_t getDiskDescriptorSize() const { return roundToSectorSize(sizeof(diskdescriptor)); }
	
	/** \internal
		\brief Creates a directory tree, equivalent to POSIX mkdir -p.
		\note This is generic and could be moved elsewhere.
		\param[in] pScheduler   Scheduler to create the directories on.
		\param[in] pDirectory   Path to create.
		\syncreturn */
	static err_t createDirectories(scheduler *pScheduler, const char *pDirectory);
	
	/** \internal
		\brief Opens a cache file with the given name.
		\param[in]  pFilename    Filename to use
		\return The open filehandle. */
	filehandle * openCacheFile(const char *pFilename);
	
	/** \internal
		\brief Deletes old cache files unless they are reusable.
		\return True if the cache files are reusable, false if they are not reusable (and were deleted). */
	bool deleteCacheFilesUnlessReusable();
	
	/** \internal
		\brief Preallocates space for a cache file to avoid fragmentation.
		\param[in]  pFH       Filehandle to resize
		\param[in]  size      New size for the file.
		\return True on success, false on failure. */
	bool resizeCacheFile(filehandle *pFH, off_t size);

#if FIOS_USE_BATCH_FREE_OPT || FIOS_USE_DIRTY_BLOCK_OPT
	/** \internal
		\brief Called to trigger a write of the specified block's information out to persistent storage.
		Caller must hold a read OR write lock.
		\param[in]  blockIndex     Block to update. 
		\return Operation object that can be used to sync all the pending writes. */
	fios::op * updateBlockNoSync(index_t blockIndex, diskdescriptor *pDescriptor);
#endif

#if FIOS_USE_BATCH_FREE_OPT
	int32_t m_maxBatchFree;         //!< Maxmimum number of blocks to free in a burst.
	bool m_deleteEmptyBlocks;   //!< if true then delete blocks as they are marked EMPTY.

	/** \internal
		\brief Find and free several blocks and sync after last write.
		Caller must hold a read OR write lock.
		\param[in]  numBlocks     Number of blocks to free. 
		\return Error code. */
	err_t freeMultipleBlocks(int32_t numBlocks);
#endif

#if FIOS_USE_DIRTY_BLOCK_OPT
	// A "dirty block" is a block whose index has been modified in RAM but no on disk.
	int32_t m_numDirtyBlocks;   //!< Number of blocks that have been marked dirty.
	int32_t m_maxDirtyBlocks;   //!< Maximuim number of dirty blocks allowed before we update the disk.

	/** \internal
		\brief Set the "dirty" bit for the specified block and increment m_numDirtyBlocks
		\param[in]  blockIndex     Block to mark. */
	void setBlockDirtyBit(index_t blockIndex) {
		if (m_index[blockIndex].dirty == 0) {
			m_numDirtyBlocks += 1;
			m_index[blockIndex].dirty = 1;
		}
	}
	/** \internal
		\brief Clear the "dirty" bit for the specified block and decrement m_numDirtyBlocks
		\param[in]  blockIndex     Block to mark. */
	void clearBlockDirtyBit(index_t blockIndex) {
		if (m_index[blockIndex].dirty == 1) {
			m_numDirtyBlocks -= 1;
			m_index[blockIndex].dirty = 0;
		}
	}

	/** \internal
		\brief Update all the dirty blocks by writing the RAM index to disk.

		Caller must hold a read OR write lock.
		\param[in]  startIndex     Index of block to start searching for dirty blocks. Apps call this with 0.
		\return Error code. */
	// TODO Should this be called when the cache is closed.
	err_t updateDirtyBlocks(index_t startIndex);
#endif

	/*@}*/
#endif
}; /* class schedulercache */
}; /* namespace fios */
}; /* namespace cell */

#endif /* __CELL_FIOS_CACHING_H__ */
