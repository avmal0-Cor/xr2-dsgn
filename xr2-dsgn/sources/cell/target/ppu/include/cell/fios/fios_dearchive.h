/*
	SCE CONFIDENTIAL
	PlayStation(R)3 Programmer Tool Runtime Library 310.001
	Copyright (C) 2009 Sony Computer Entertainment Inc.
	All Rights Reserved.
*/
/** \file fios_dearchive.h

	Class definitions for the fios::dearchiver class. The dearchiver
	derives from fios::mediafilter, which itself derives from fios::media.
*/
#ifndef __CELL_FIOS_DEARCHIVE_H__
#define __CELL_FIOS_DEARCHIVE_H__

#include <cell/fios/fios_types.h>
#include <cell/fios/fios_base.h>
#include <cell/fios/fios_platform.h>
#include <cell/fios/compression/compression_decompressor.h>

#ifndef DOXYGEN_IGNORE
namespace cell { namespace fios { namespace PSArc { struct TOCEntry; } } }
#endif

namespace cell {
namespace fios {

/** \brief Media filter layer that reads files encoded in .psarc files.
	When this layer is in place, you get special behavior when one or more
	.psarc files are open: the contents of the archive show
	up in the filesystem hierarchy. Relative psarc files appear as a folder,
	while absolute psarc files can scatter their contents across the whole tree.<br>
	<br>
	Reading directory contents may be very expensive when an archive is open.
	By default, the dearchiver layer does not allocate the data structures in
	memory to allow it to satisfy directory reads. If you make a directory read
	call, additional memory is allocated to hold the manifests of every file
	that you have open.<br>
	<br>
	File data is decompressed using a Compression::AsyncDecompressor object.
	This is an abstract interface to a background decompressor of your choice,
	which can run either in a secondary thread or (on PS3) as a SPURS task. The
	decompressor must match the algorithms used in your archives.
*/
class FIOS_LINKAGE dearchiver : public mediafilter
{
public:
	FIOS_OBJECT_NEW_AND_DELETE(dearchiver,kMemPurposeFIOSMediaObject|kPoolIDLongTerm)
	
public:
	/** \brief The search method defines the order in which absolute archives are searched. */
	typedef enum e_SEARCHMETHOD {
		kSearchOldestToNewest,   //!< If a path is in more than one archive, files from the oldest archive filehandle are preferred.
		kSearchNewestToOldest,   //!< If a path is in more than one archive, files from the newest archive filehandle are preferred.
	} searchmethod_t;
	
	/** \brief Creates a dearchiver object (one decompressor, one archive extension).
		\param[in]   pNextMedia          Next media in the stack.
		\param[in]   pDecompressor       Decompressor interface to use.
		\param[in]   searchMethod        Search method to use when multiple archives are open.
		\param[in]   allowOverrides      Whether to check for on-disk files to override files in the archive. [default = false]
		\param[in]   pArchiveExtension   Extension used to detect archives. [default = ".psarc"]
		\see e_SEARCHMETHOD */
	dearchiver(media *pNextMedia,
		Compression::AsyncDecompressor *pDecompressor,
		searchmethod_t searchMethod = kSearchOldestToNewest,
		bool allowOverrides = false,
		const char *pArchiveExtension = ".psarc");
	
	/** \brief Creates a dearchiver object (multiple decompressors, one archive extension).
		\param[in]   pNextMedia          Next media in the stack.
		\param[in]   numDecompressors    Number of decompressors to use, up to 4.
		\param[in]   pDecompressorArray  Array of decompressors. This array is copied internally and does not need to persist.
		\param[in]   searchMethod        Search method to use when multiple archives are open.
		\param[in]   allowOverrides      Whether to check for on-disk files to override files in the archive. [default = false]
		\param[in]   pArchiveExtension   Extension used to detect archives. [default = ".psarc"]
		\see e_SEARCHMETHOD */
	dearchiver(media *pNextMedia,
		size_t numDecompressors,
		Compression::AsyncDecompressor **pDecompressorArray,
		searchmethod_t searchMethod = kSearchOldestToNewest,
		bool allowOverrides = false,
		const char *pArchiveExtension = ".psarc");

	/** \brief Creates a dearchiver object (multiple decompressors, multiple archive extensions).
		\param[in]   pNextMedia          Next media in the stack.
		\param[in]   numDecompressors    Number of decompressors to use, up to 4.
		\param[in]   pDecompressorArray  Array of decompressors. This array is copied internally and does not need to persist.
		\param[in]   searchMethod        Search method to use when multiple archives are open.
		\param[in]   allowOverrides      Whether to check for on-disk files to override files in the archive. [default = false]
		\param[in]   numExtensions            Number of extensions used to detect archives.
		\param[in]   pArchiveExtensionArray   Array of extensions used to detect archives. These are used by reference and should be string constants or globals.
		\see e_SEARCHMETHOD */
	dearchiver(media *pNextMedia,
		size_t numDecompressors,
		Compression::AsyncDecompressor **pDecompressorArray,
		searchmethod_t searchMethod,
		bool allowOverrides,
		size_t numExtensions,
		const char **pArchiveExtensionArray);

protected:
#ifndef DOXYGEN_IGNORE
	/** \brief Common initializer for the above constructors.
		\internal
		\param[in]   numDecompressors    Number of decompressors to use, up to 4.
		\param[in]   pDecompressorArray  Array of decompressors. This array is copied internally and does not need to persist.
		\param[in]   numExtensions            Number of extensions used to detect archives.
		\param[in]   pArchiveExtensionArray   Array of extensions used to detect archives. These are used by reference and should be string constants or globals. */
	void init(
		size_t numDecompressors,
		Compression::AsyncDecompressor **pDecompressorArray,
		size_t numExtensions,
		const char **pArchiveExtensionArray);
#endif // DOXYGEN_IGNORE
	
public:

	/** \brief Destroys a dearchiver object. */
	virtual ~dearchiver();
	
#ifndef DOXYGEN_IGNORE
	/** \brief Returns the debugging name of a variable by its index.
		\param[in] index    Index of the variable to query.
		\return Name of the variable as a C-string, or NULL if the index is unknown. */
	virtual const char * getVariableName(size_t index) const;
	
	/** \brief Releases all non-essential memory.

		Aggressively frees all memory possible. Currently equivalent to calling
		freeTOCs(), freeReadBuffers(), and freeManifests(). This function can
		temporarily free up memory, but additional allocations may be necessary
		the next time any I/O request arrives. */
	inline void freeAllNonEssentialMemory() { freeTOCs(); freeReadBuffers(); freeManifests(); }
	
	/** \brief Releases archive TOCs.

		Frees the archive table-of-contents info for all archives. This function
		can temporarily free up memory, but the TOC is an essential part of
		accessing an archive and needs to be reloaded from disk before any
		I/O request can be processed. */
	void freeTOCs();
	
	/** \brief Releases read buffers.

		Frees the read buffers which are shared across all archives. Currently
		there are 3 read buffers, each of which is the size of an archive block
		(default 64 KiB), for a default total of 192 KiB. This function can
		temporarily free up memory, and some I/O requests can be processed with
		less than a full set of buffers, but they will start to be reallocated the
		next time an I/O request to an archive arrives. */
	void freeReadBuffers();
	
	/** \brief Releases archive manifests.

		Frees the archive manifests for all archives. The manifest contains the
		full path of every file in the archive, which can be quite large for some
		archives. It's only allocated in response to a scheduler::readDirectory call
		and can be safely freed at any time. The next time a scheduler::readDirectory
		call is processed it is reloaded from disk. */
	virtual void freeManifests();
	
	/** \brief Changes whether this layer allows on-disk files to override the contents of archives.
		\param[in] newValue     True if this layer should allow on-disk files to override the contents of archives, false otherwise. */
	void setAllowOverrides(bool newValue);
	
	/** \brief Returns a value indicating whether this layer allows on-disk files to override the contents of archives.
		\return True if this layer allows on-disk files to override the contents of archives, false otherwise. */
	inline bool getAllowOverrides() const { return m_allowOverrides; }
	
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
	
	/** \brief Queries the number of files open inside an archive.
		\param[in]   pArchivePath      Path to an open archive.
		\result  Number of files open inside an archive. Zero if the archive is not open, or if there are no files open inside it. */
	int32_t countOpenFilesInArchive(
		const char *pArchivePath) const;

protected:
	/** \internal
		@{ */
    
	/** Internal class used to track open archives. */
	class ArchiveTracker;
	
	mutable platform::rwlock m_rwLock;                    //!< Read-write lock for access to data members.
	platform::mutex m_readGroupLock;                      //!< Mutex to ensure that all reads from a single incoming read are grouped together (no extra seeks) 
	size_t m_numDecompressors;                            //!< Number of available decompressors.
	Compression::AsyncDecompressor *m_pDecompressors[4]; //!< Array of available decompressors.
	bool m_disabled;                                      //!< Affects whether this layer intercepts calls or not. 
	searchmethod_t m_searchMethod;                        //!< Search method when multiple archives are open. 
	platform::atomicBool m_allowOverrides;                //!< When true, the dearchiver looks for a "real" on-disk file and prefers that over files in the archive. 
	size_t m_numExtensions;                               //!< Number of archive extensions.
	const char *m_pArchiveExtensions[8];                  //!< Array of extensions that indicate an archive file. Normally ".psarc".
	size_t m_archiveExtensionLen[8];                      //!< Length of each string in m_pArchiveExtensions.
	collections::list<ArchiveTracker*> m_archiveList;     //!< List of ArchiveTrackers for open archives. 
	size_t m_nextMediaCoefficients;                       //!< Number of coefficients provided by the next media in the stack. 
	
	// Buffer management
	enum { kNumBuffers = 3 };
	platform::mutex m_bufferLock;                         //!< Lock used to protect buffer trackers.
	platform::cond m_bufferCond;                          //!< Condition so threads can wait for a buffer.
	uint32_t m_bufferAlignment;                           //!< Buffer alignment
	typedef struct BufferTracker {
		uint8_t *pBuffer;                                      //!< Buffer pointer.
		uint32_t size;                                         //!< Size of this buffer.
		int readers;                                      //!< Number of readers for this buffer.
		int writers;                                      //!< Number of writers for this buffer (0 or 1).
		bool compressed;                                  //!< Whether the buffer is compressed.
		ArchiveTracker *pArchive;                         //!< Which archive the buffer came from.
		uint32_t tocIndex;                                     //!< Which file in the archive the buffer came from.
		uint32_t blockNumber;                                  //!< Which block in the file the buffer came from.
	} BufferTracker;
	BufferTracker m_buffers[kNumBuffers];                 //!< \internal Buffer tracker array.
	uint8_t * bufferLookup(bool compressed, uint32_t blockSize, ArchiveTracker *pArchive, uint32_t tocIndex, uint32_t blockNumber);
	uint8_t * bufferLookup2(bool compressed, uint32_t blockSize, ArchiveTracker *pArchive, uint32_t tocIndex, uint32_t blockNumber);
	uint8_t * bufferMalloc(uint8_t *pOldBuffer, uint32_t blockSize);
	uint8_t * bufferAllocate(bool compressed, bool dontUseCache, uint32_t blockSize, ArchiveTracker *pArchive, uint32_t tocIndex, uint32_t blockNumber, bool *cacheHit);
	void bufferFilled(uint8_t *pPtr);
	void bufferRelease(uint8_t *pPtr, bool okToCacheContents);
	void bufferReleasePair(uint8_t *pPtr[2], bool okToCacheContents);
	void bufferInvalidateArchive(ArchiveTracker *pArchive);
	
	// Manifest information
	collections::array<const char *> m_manifestPathTable;   //!< \internal Sorted array of full paths for all files in all open archives.
    
	// Lookup cache
	typedef struct LookupCacheEntry {
		staticpath_t                 path;              //!< Full path
		ArchiveTracker *             pArchive;          //!< Archive tracker
		const PSArc::TOCEntry *pTOCEntry;         //!< TOC entry
		uint8_t                           context[32];       //!< PSArc::ArchiveParser::Context
	} LookupCacheEntry;
	enum { kLookupCacheSize = 8 };
	platform::mutex m_lookupCacheLock;                  //!< \internal Lock protecting m_lookupCache.
	LookupCacheEntry m_lookupCache[kLookupCacheSize];   //!< \internal Cache used by findFile and executeReadFile.
	uint32_t m_lookupCacheIndex;                             //!< \internal Next index to use for the cache.
	
	/** \brief Resolves a path.

		This is used to translate paths through the archiver.
		\param[in,out] pIOP          Media I/O params for any necessary I/O.
		\param[in,out] pPathBuffer   Path to resolve.
		\param[in,out] pOffset       Offset to resolve.
		\param[in,out] pByteCount    ByteCount to resolve.
		\return  If the path would be passed down to the next media layer, the next layer's path is filled in and true is returned. Otherwise it returns false. */
	bool resolvePath(mediaioparams *pIOP, char *pPathBuffer, off_t *pOffset, off_t *pByteCount)
		FIOS_ATTRIBUTE_GCC((__nonnull__));
	
	/** \brief Internal helper function to look up a context from the lookup cache.
		\param[in]  path              Path that you're accessing.
		\param[out] pContext          Filled in with a PSArc::ArchiveParser::Context if successful; untouched if not. */
	void lookupContext(const pathreference_t &path, void *pContext);
	
	/** \brief Internal helper function to stash a context into the lookup cache.
		\param[in]  path              Path that you're accessing.
		\param[out] pContext          A PSArc::ArchiveParser::Context to remember. */
	void rememberContext(const pathreference_t &path, const void *pContext);
	
	/** \brief Internal helper function to see if a path ends with an archive extension.
		May be overridden by subclasses if you want to do more complicated archive detection.
		\param[in]  pPath             Path to check.
		\return  True if the path points to an archive, false otherwise. */
	virtual bool pathIsArchive(const char *pPath);
	
	/** \brief Internal helper function to strip an archive extension when opening a relative archive.

		May be overridden by subclasses if you want to do more complicated archive detection.
		\param[in]  pPath             Path to strip.
		\return  Length of the path up to the archive extension. */
	virtual size_t lengthWithoutArchiveExtension(const char *pPath);
	
	/** \brief Internal helper function to see if a path represents a relative archive.
		\param[in]  pPath             Path to check.
		\param[out] pOutArchiveStat   Returns the stat information for the archive. May be NULL.
		\return  True if the path is the fake directory created by an open relative archive. */
	bool pathIsArchiveAsFolder(const char *pPath, fios::stat_t *pOutArchiveStat = NULL);
	
	/** \brief Internal helper to load an archive's TOC.

		This allocates memory and loads the TOC.
		\param[in]  pIOP         I/O parameters to use for the read.
		\param[in]  pArchive     Archive from which to load the TOC.
		\return  True if the TOC could be loaded (allocation and read succeeded), false otherwise. */
	bool loadArchiveTOC(mediaioparams *pIOP, ArchiveTracker *pArchive);
	
	/** \brief Internal helper to find a file in open archives.

		This function may need to perform I/O if freeTOCs() has been called.
		\param[in]  pIOP         I/O parameters describing the operation.
		\param[out] pOutArchive  Returns the archive tracker.
		\param[out] pOutTOCEntry Returns the TOC entry for the file.
		\return  True if the file was found in an open archive, false otherwise. */
	bool findFile(mediaioparams *pIOP, ArchiveTracker **pOutArchive, const PSArc::TOCEntry **pOutTOCEntry);
	
	/** \brief Internal helper function to resolve embedded archive requests.

		If a request is made to an embedded archive, this function tries to
		turn it into a request to the outermost archive so that we can avoid
		recursion. This function may reload the parent archive's TOC, if necessary.
		\param[in]  pIOP         I/O parameters to use for a TOC reload.
		\param[in]  pRequest     Media request to resolve. */
	void resolveEmbeds(mediaioparams *pIOP, mediarequest *pRequest);
	
	/** \brief Internal helper function to load an archive manifest.

		Write lock must be held.
		\param[in]  pIOP         I/O parameters to use for manifest load.
		\param[in]  pArchive     Archive from which to load the manifest.
		\return  True if the TOC could be loaded (allocation and read succeeded), false otherwise. */
	bool loadArchiveManifest(mediaioparams *pIOP, ArchiveTracker *pArchive);

	/** \brief Internal helper function to rebuild the manifest path table.

		Write lock must be held.
		\return  True if the manifest path table could be rebuild (allocation succeeded), false otherwise. */
	bool rebuildManifestPathTable();
	
	/** \brief Searches the manifest path table for files that are immediate children of the given directory.

		Read or write lock must be held.
		\param[in]  pDirectory   Directory that we're looking for children of.
		\return  Index of the first manifest path table entry, or -1 if none were found. */
	int32_t searchManifestPathTable(const char *pDirectory);
	
	/** \brief Returns whether the directory is an immediate parent of the given manifest path table entry.
		\param[in]  index        Index of the manifest path table entry.
		\param[in]  pDirectory   Directory that we're looking for children of.
		\return  True if it is, false if it's not. */
	bool manifestEntryIsDirectoryChild(int32_t index, const char *pDirectory);
	
	/** \brief Returns whether the directory is an ancestor of the given manifest path table entry.
		\param[in]  index           Index of the manifest path table entry.
		\param[in]  pDirectory      Directory that we're looking for descendants of.
		\param[out] pOutChildFolder Filled in with the immediate child of pDirectory which is the entry's ancestor. May be NULL.
		\return  True if it is, false if it's not. */
	bool manifestEntryIsDirectoryDescendant(int32_t index, const char *pDirectory, char *pOutChildFolder);
	
	/** \brief Returns the full path corresponding to the manifest path table entry.
		\param[in]  index        Index of the manifest path table entry.
		\param[out] pBuffer      Buffer to hold the full path. Must be FIOS_PATH_MAX bytes or larger.
		\return  Returns pBuffer. */
	const char * manifestEntryPath(int32_t index, char *pBuffer);
	
	/** \brief Internal helper function to perform a stat request.
		\param[in,out]  pIOP     I/O parameters describing the operation. */
	void executeStat(mediaioparams *pIOP);

	/** \brief Internal helper function to perform an open request on an archive.
		\param[in,out]  pIOP     I/O parameters describing the operation. */
	void executeOpenArchive(mediaioparams *pIOP);
	
	/** \brief Internal helper function to perform an open request on a file.
		\param[in,out]  pIOP     I/O parameters describing the operation. */
	void executeOpenFile(mediaioparams *pIOP);

	/** \brief Internal helper function to perform a close request on an archive.
		\param[in,out]  pIOP     I/O parameters describing the operation. */
	void executeCloseArchive(mediaioparams *pIOP);
	
	/** \brief Internal helper function to perform a close request on a file.
		\param[in,out]  pIOP     I/O parameters describing the operation. */
	void executeCloseFile(mediaioparams *pIOP);
	
	/** \brief Internal helper function to perform a read directory request.
		\param[in,out]  pIOP     I/O parameters describing the operation. */
	void executeReadDirectory(mediaioparams *pIOP);
	
	/** \brief Internal helper function to perform a read directory request on an archive path.
		\param[in,out]  pIOP     I/O parameters describing the operation. */
	void executeReadArchiveDirectory(mediaioparams *pIOP);
	
	/** \brief Internal helper function for executeReadDirectory and executeReadArchiveDirectory.
		\param[in,out]  pIOP     I/O parameters describing the operation.
		\param[in]      index    Manifest table index. */
	void fillDirEntryFromManifest(mediaioparams *pIOP, int32_t index);
	
	/** \brief Internal helper function to perform a read file request.
		\param[in,out]  pIOP     I/O parameters describing the operation. */
	void executeReadFile(mediaioparams *pIOP);
	
	/** \brief Internal helper function to perform a read file request from a specified archive.
		\param[in,out]  pIOP     I/O parameters describing the operation.
		\param[in]      pArchive Archive containing the file.
		\param[in]      tocIndex Index of the file in the TOC. */
	void executeReadFileFromArchive(mediaioparams *pIOP, ArchiveTracker *pArchive, uint32_t tocIndex);
	
	/** \brief Internal helper to read a special file (SDATA/SPRX/SELF) file from an archive.
		\param[in,out]  pIOP     I/O parameters describing the operation.
		\param[in]      pArchive Archive containing the file.
		\param[in]      tocIndex Index of the file in the TOC. */
	void executeReadSpecialFromArchive(mediaioparams *pIOP, ArchiveTracker *pArchive, uint32_t tocIndex);

	/** \brief Internal helper to get the offset of a special (SDATA/SPRX/SELF) file in an archive.
	  	\param[in]		pRequest mediarequest for the operation
		\patam[in]		pTOCEntry the TOC entry for the file
		\param[in]      pArchive Archive containing the file. */
	off_t getSpecialOffset(mediarequest *pRequest, const PSArc::TOCEntry
						   *pTOCEntry, ArchiveTracker *pArchive);

	/** \brief Internal helper function to perform a prefetch request from a specified archive.
		\param[in,out]  pIOP     I/O parameters describing the operation. */
	void executePrefetch(mediaioparams *pIOP);
	
	/*@}*/
#endif // DOXYGEN_IGNORE
}; // class dearchiver
}; // namespace fios
}; // namespace cell

#endif // __CELL_FIOS_DEARCHIVE_H__
