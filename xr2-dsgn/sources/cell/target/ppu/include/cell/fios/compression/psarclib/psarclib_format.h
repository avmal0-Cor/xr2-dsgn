/*
	SCE CONFIDENTIAL
	PlayStation(R)3 Programmer Tool Runtime Library 310.001
	Copyright (C) 2009 Sony Computer Entertainment Inc.
	All Rights Reserved.
*/
/**
	\file psarclib_format.h
	Defines the format of a .psarc file.
*/
#ifndef __CELL_PSARCLIB_FORMAT_H__
#define __CELL_PSARCLIB_FORMAT_H__

#include <cell/fios/compression/compression_types.h>

namespace cell {
namespace fios {
namespace PSArc {

/*
	Here's what a typical .psarc file looks like.
    
	+-------------+
	|    Header   |                          Magic number, TOC size, file count, global flags, etc.
	+-------------+ +-----------------+
	|             | |                 |
	|             | |                 |
	|             | |   TOC Entries   |      An array of (ArchiveHeader.numFiles x TOCEntry)
	|             | |                 |      
	|     TOC     | |                 |
	|             | +-----------------+
	|             | |                 |      A list of compressed block sizes stored as N-byte ints. The integer size, N,
	|             | | Block Size List |      is based on the uncompressed block size. A value of zero is used to indicate
	|             | |                 |      that a block is stored uncompressed.
	+-------------+ +-----------------+
	|             | 
	|             | 
	|    Data     |                          The remainder of the file is filled with individual compressed blocks.
	|             |
	|             |
	
	With psarc v1, these three elements had to exist in this order with no padding or separation.
	With psarc v3, the header is still at the beginning, but the TOC and Data are pointed to from the
	header and may be relocated. The above layout is still the typical form.

	For PS3 targets, it's possible to create a PSARC-MSELF hybrid file which keeps an extra 64KB of
	padding at the beginning of the file to store the MSELF TOC. Thus, in a PSARC-MSELF
	hybrid file, the PSARC header begins at offset 65536 within the file.
*/

enum e_PSARC {
	kPSARCMagicNumber    = 0x50534152,   /**< Magic number that indicates this is a .psarc file. Equivalent to 'PSAR' in ASCII. */
	kPSARCFormatVersion1 = 0x00010000,   /**< File format version 1. */
	kPSARCFormatVersion3 = 0x00030000    /**< File format version 3. */
};

enum e_PSARCMSELF {
	kMSELFMagicNumber       = 0x4D534600,  /**< Magic number that indicates this is a .psarc.mself file. Equivalent to 'MSF\0' in ASCII. */
	kPSARCMSELFHeaderOffset = 65536        /**< Offset of the PSARC header from the beginning of the file. */
};

enum e_ALGORITHM {
    kAlgorithmUncompressed = cell::fios::Compression::kUncompressed, /**< The null compression algorithm: blocks are stored uncompressed. */
    kAlgorithmZLIB         = cell::fios::Compression::kZLIB,         /**< Blocks are compressed with zlib's deflate algorithm. Equivalent to ASCII 'zlib'. */
    kAlgorithmLZO1X        = cell::fios::Compression::kLZO1X,        /**< Blocks are compressed with LZO 1x. Equivalent to ASCII 'lzox'. */
    kAlgorithmLZO1Y        = cell::fios::Compression::kLZO1Y,        /**< Blocks are compressed with LZO 1y. Equivalent to ASCII 'lzoy'. */
    kAlgorithmLZMA         = cell::fios::Compression::kLZMA,         /**< Blocks are compressed with LZMA. Equivalent to ASCII 'lzma'. */
    kAlgorithmBZIP2        = cell::fios::Compression::kBZIP2         /**< Blocks are compressed with BZIP2. Equivalent to ASCII 'bzp2'. */
};

enum e_GLOBALFLAGS {
    kArchiveFlagIgnoreCase   = (1UL << 0),    /**< When computing MD5 hashes of filenames, uppercase the names first. */
    kArchiveFlagAbsolute     = (1UL << 1),    /**< Archive pathnames are absolute (starting with /), as opposed to relative. */
	kArchiveFlagSortedTOC    = (1UL << 2)     /**< The table of contents in this archive is sorted to enable binary searching. */
};

/** \brief Format of a psarc version 3 archive file header.
    The v3 archive header is 48 bytes long. This structure can be used to handle both v1 and v3 archives,
    if you handle it carefully and observe the differences.
    All integers are stored in network (big-endian) byte order on disk.
*/
struct ArchiveHeader {
    uint32_t   magic;         /**< Set to kPSARCMagicNumber */
    uint32_t   version;       /**< Set to kPSARCFormatVersion */
    uint32_t   algorithm;     /**< Algorithm used in this archive. */
    uint32_t   tocSize;       /**< [v3] Size of the TOC chunk (entries + block list).
	                          [v1] Size of the TOC chunk + size of ArchiveHeader_v1 (32 bytes). */
    uint32_t   tocEntrySize;  /**< Size of a single entry in the table of contents. Currently 30, but may be expanded in the future. */
    uint32_t   numFiles;      /**< Number of files in the archive. */
    uint32_t   blockSize;     /**< Size of data blocks before compression. */
    uint32_t   archiveFlags;  /**< Global archive flags. */
	uint64_t   offsetToTOC;   /**< [v3] Offset to TOC and block list from start of this header. */
	uint64_t   offsetToData;  /**< [v3] Offset to compressed data from start of this header. */
};

/** \brief Format of a psarc version 1 archive file header.
	The v1 archive header is 32 bytes long.
	All integers are stored in network (big-endian) byte order on disk.
*/
struct ArchiveHeader_v1 {
    uint32_t   magic;         /**< Set to kPSARCMagicNumber */
    uint32_t   version;       /**< Set to kPSARCFormatVersion */
    uint32_t   algorithm;     /**< Algorithm used in this archive. */
    uint32_t   tocSize;       /**< Size of the TOC chunk + size of ArchiveHeader_v1 (32 bytes). */
    uint32_t   tocEntrySize;  /**< Size of a single entry in the table of contents. Currently 30, but may be expanded in the future. */
    uint32_t   numFiles;      /**< Number of files in the archive. */
    uint32_t   blockSize;     /**< Size of data blocks before compression. */
    uint32_t   archiveFlags;  /**< Global archive flags. */
};

/** \brief Format of a psarc TOC entry.
    Each TOC entry is at least 30 bytes long.
	All integers are stored in network (big-endian) byte order on disk.
*/
struct TOCEntry {
    uint8_t    md5[16];         /**< MD5 hash of the archive path to this file. A null hash (all zeroes) is used for the manifest file. */
    uint8_t    blockIndex[4];   /**< Index of the compressed block where the file starts. Beginning with v3, an index of -1 (all FF) means this file is stored uncompressed and has no block list entries. */
    uint8_t    originalSize[5]; /**< Original size of file, as a 40-bit int */
    uint8_t    startOffset[5];  /**< Offset of the first compressed block of this file from the start of the psarc file, as a 40-bit int */
};


}; /* namespace PSArc */
}; /* namespace fios */
}; /* namespace cell */


#endif /* __CELL_PSARCLIB_FORMAT_H__ */
