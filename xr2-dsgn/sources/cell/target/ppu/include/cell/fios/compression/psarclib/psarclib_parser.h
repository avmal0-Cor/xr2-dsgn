/*
	SCE CONFIDENTIAL
	PlayStation(R)3 Programmer Tool Runtime Library 310.001
	Copyright (C) 2007 Sony Computer Entertainment Inc.
	All Rights Reserved.
*/
/**
	\file psarclib_parser.h
	Defines the cell::fios::PSArc::ArchiveParser class for parsing the contents of .psarc files.
*/
#ifndef __CELL_PSARCLIB_PARSER_H__
#define __CELL_PSARCLIB_PARSER_H__

#include <cell/fios/compression/compression_defines.h>
#include <cell/fios/compression/psarclib/psarclib_format.h>

namespace cell {
namespace fios {
namespace PSArc {

/** \brief  Archive file parser.
    This class is designed as a general-purpose .psarc file parser. It does no
    actual I/O -- all reading is handled by the caller, who then passes the data
    into the parser.
 */

class COMPRESSION_EXPORT ArchiveParser
{
public:
    /** \brief  Creates an archive parser.
        Before you can create an ArchiveParser, you must read the first
        sizeof(ArchiveHeader) bytes of the file into RAM.
        \param[in]  pHeader   The header of the archive file. */
    ArchiveParser(const ArchiveHeader *pHeader);
    
    /** \brief  Destructor. */
    virtual ~ArchiveParser();
    
    /* ----------- Parser Context ---------------- */
    
    class Context
    {
    public:
        Context() : valid(false) {}
		~Context() {}
    protected:
        friend class ArchiveParser;
        bool valid;
        uint32_t  tocIndex;
        uint32_t  blockNumber;
        uint64_t  offsetInFile;
        uint32_t  lastBlock;
        uint32_t  lastBlockSize;
    };
    
    /* ----------- File information ---------------- */
    
    /** \brief  Indicates whether the ArchiveParser can read this file.
        This function is the only place where we check the archive version and format. 
        \note DO NOT attempt to use this parser if this function returns false, or else it will probably crash.
        \return True if the archive is accessible, false otherwise. */
    bool        isReadable() const;
    
    /** \brief  Returns the algorithm used in the archive.
		Use this to verify that the algorithm used in the archive is one that you can handle.
        \return Algorithm code. */
    inline uint32_t  getAlgorithm() const { return m_archiveHeader.algorithm; }
    
    /** \brief  Returns the number of files in the archive.
        \return Number of files in the archive. This count includes the manifest file. */
    inline uint32_t  getNumberOfFiles() const { return m_archiveHeader.numFiles; }
    
    /** \brief  Returns the uncompressed block size of the archive file.
        \return Block size in bytes. */
    inline uint32_t  getBlockSize() const { return m_archiveHeader.blockSize; }
    
    /** \brief  Indicates whether the archive is absolute.
        \return True if the archive contains absolute paths. */
    inline bool isAbsolute() const   { return (m_archiveHeader.archiveFlags & kArchiveFlagAbsolute) != 0; }
    
    /** \brief  Indicates whether the archive is relative.
        \return True if the archive contains relative paths. */
    inline bool isRelative() const   { return (m_archiveHeader.archiveFlags & kArchiveFlagAbsolute) == 0; }
    
	/** \brief  Indicates whether the archive's TOC is sorted.
		\return True if the archive's TOC is sorted. */
	inline bool isSorted() const     { return (m_archiveHeader.archiveFlags & kArchiveFlagSortedTOC) != 0; }
	
    /** \brief  Converts a byte length (or offset) to a block count.
        \param[in]  bytes      Byte length.
        \return Number of blocks needed to hold that many bytes. */
    inline uint32_t bytesToBlocks(uint64_t bytes) const { return (uint32_t)((bytes + m_archiveHeader.blockSize - 1) / m_archiveHeader.blockSize); }
    
    /* ------------ Reading the TOC --------------- */
    
    /** \brief  Returns the location of the archive TOC in the file.
        \return Location of the TOC, as a byte offset from the start of the header. */
    inline uint64_t  getTOCLocation() const { return m_archiveHeader.offsetToTOC; }
    
    /** \brief  Returns the length of the archive TOC in the file.
        \return Length of the TOC, in bytes. */
    inline uint64_t  getTOCLength() const { return m_archiveHeader.tocSize; }
    
	/** \brief  Sorts the TOC so that lookups can be performed faster.
		Once you've called this function, you are promising that you'll always call us with a sorted TOC. */
	void sortTOC(void *pArchiveTOC);
	
    /* ------------ File Lookup ----------------- */
    
    /** \brief  Returns the TOC entry for the manifest.
        The manifest file contains a list of the archive's contents separated by newlines.
        \param[in]   pArchiveTOC      TOC of the archive.
        \return  Returns the TOC entry for the manifest file, or NULL if not found. */
    const TOCEntry * getManifestEntry(const void *pArchiveTOC) const;
    
    /** \brief  Looks up a file in an archive.
        \param[in]   pArchiveTOC      TOC of the archive.
        \param[in]   pArchivePath     File to look up.
        \return  Returns the TOC entry for the file, or NULL if not found. */
    const TOCEntry * lookupFileEntry(const void *pArchiveTOC, const char *pArchivePath) const;
    
	/** \brief  Converts a TOC entry to a TOC index.
		TOC entry pointers are good for immediate use. However, they can go bad if the TOC
		is flushed and reloaded. Sometimes it may be better for clients to store a TOC index instead.
		\param[in]   pArchiveTOC      TOC of the archive.
		\param[in]   pTOCEntry        TOC entry for the file.
		\return  Returns a TOC index. */
	inline uint32_t getTOCIndexFromEntry(const void *pArchiveTOC, const TOCEntry *pTOCEntry) const
	{ return (uint32_t)((reinterpret_cast<const uint8_t*>(pTOCEntry) - reinterpret_cast<const uint8_t*>(pArchiveTOC)) / m_archiveHeader.tocEntrySize); }
	
	/** \brief  Converts a TOC entry to a TOC index.
		TOC entry pointers are good for immediate use. However, they can go bad if the TOC is flushed
		and reloaded. Sometimes it may be better for clients to store a TOC index instead.
		\param[in]   pArchiveTOC      TOC of the archive.
		\param[in]   tocIndex         TOC index of the file.
		\return  Returns a TOC entry. */
	inline const TOCEntry * getTOCEntryFromIndex(const void *pArchiveTOC, uint32_t tocIndex) const
	{ return reinterpret_cast<const TOCEntry*>(reinterpret_cast<const uint8_t*>(pArchiveTOC) + m_archiveHeader.tocEntrySize*tocIndex); }
	
    /** \brief  Returns the size of a file.
        \param[in]   pArchiveTOC      TOC of the archive.
        \param[in]   pTOCEntry        TOC entry for the file.
        \return  Returns the size of the file in bytes. */
    uint64_t getFileSize(const void *pArchiveTOC, const TOCEntry *pTOCEntry) const;
    
	/** \brief  Returns the start of a file inside the archive.
		\param[in]   pArchiveTOC      TOC of the archive.
		\param[in]   pTOCEntry        TOC entry for the file.
		\return  Returns the start location of the file in bytes from the start of the archive. */
	uint64_t getFileOffset(const void *pArchiveTOC, const TOCEntry *pTOCEntry) const;
	
    /** \brief  Returns the size, location, and compression method of the Nth block of a file.
        \param[in]     pArchiveTOC      TOC of the archive.
        \param[in]     pTOCEntry        TOC entry for the file.
        \param[in,out] pContext         Context used to speed up sequential requests. This is required and cannot be NULL.
        \param[in]     n                Which block of the file to return.
        \param[out]    pOutSize         Size of the compressed block.
        \param[out]    pOutLocation     Offset of the compressed block from the start of the .psarc file.
        \param[out]    pOutIsCompressed Whether the block is compressed.
        \return  True if this was a valid request, false if parameters didn't work (e.g. an invalid block was requested) */
    bool  findNthBlockOfFile(const void *pArchiveTOC, const TOCEntry *pTOCEntry, Context *pContext, uint32_t n, uint64_t *pOutSize, uint64_t *pOutLocation, bool *pOutIsCompressed) const;
    
    /** \brief  Checks to see if a particular file is stored uncompressed.
        \param[in]   pArchiveTOC      TOC of the archive.
        \param[in]   pTOCEntry        TOC entry for the file.
        \return Returns true if the whole file is entirely uncompressed, false if any portion of the file may be compressed. */
    bool fileIsUncompressed(const void *pArchiveTOC, const TOCEntry *pTOCEntry) const;

	/** \brief  Checks to see if a particular region of a file is stored uncompressed.
        \param[in]     pArchiveTOC      TOC of the archive.
        \param[in]     pTOCEntry        TOC entry for the file.
		\param[in]     offset           Offset of the region from the start of the file.
		\param[in]     size             Size of the region.
		\return  True if this region of the file is stored uncompressed. */
	bool  fileRegionIsUncompressed(const void *pArchiveTOC, const TOCEntry *pTOCEntry, uint64_t offset, uint64_t size) const;

protected:
    ArchiveHeader    m_archiveHeader;         /**< Converted from network to host byte order, for easy access */
    uint8_t               m_blockListIntegerSize;  /**< Size of the integers in the block list. Between 1-4 depending on the archive's block size. */
    bool             m_sortedTOC;             /**< Whether we'll always be called with a sorted TOC or not. */
	
    /** \brief  Looks up a file in an archive by MD5.
        \param[in]   pArchiveTOC      TOC of the archive.
        \param[in]   pDigest          MD5 digest of the file to look up.
        \return  Returns the TOC entry for the file, or NULL if not found. */
    const TOCEntry * lookupFileEntry(const void *pArchiveTOC, const uint8_t *pDigest) const;    
};


}; /* namespace PSArc */
}; /* namespace fios */
}; /* namespace cell */

#endif /* __CELL_PSARCLIB_PARSER_H__ */
