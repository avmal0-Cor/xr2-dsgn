/*
	SCE CONFIDENTIAL
	PlayStation(R)3 Programmer Tool Runtime Library 310.001
	Copyright (C) 2008 Sony Computer Entertainment Inc.
	All Rights Reserved.
*/
/**
	\file	compression_decompressor.h
	
	Abstract interfaces for decompressors
*/

#ifndef __CELL_COMPRESSION_TYPES_H__
#define __CELL_COMPRESSION_TYPES_H__

#include <cell/fios/fios_memory.h>
#include <cell/fios/compression/compression_defines.h>

namespace cell {
namespace fios {
namespace Compression {


/** \brief Compressed stream formats.
	These enums describe the data format, not a specific codec implementation.
	Multiple codecs may be able to create or parse a given data format. For
	example, #kZLIB data can be parsed by the #EdgeZlibDecompressor. */
enum e_FORMATS
{
    kUncompressed = 0,            /**< The null compression algorithm: data is stored uncompressed. */
    kZLIB         = 0x7A6C6962,   /**< zlib: 2 bytes of zlib header, then the compressed data stream. Equivalent to ASCII 'zlib'. */
    kLZMA         = 0x6C7A6D61,   /**< LZMA: 5 bytes of header, 8 bytes of little-endian size, then the compressed data stream. Equivalent to ASCII 'lzma'. */
    kLZO1X        = 0x6C7A6F78,   /**< LZO 1x. Equivalent to ASCII 'lzox'. */
    kLZO1Y        = 0x6C7A6F79,   /**< LZO 1y. Equivalent to ASCII 'lzoy'. */
    kBZIP2        = 0x627A7032    /**< BZIP2. Equivalent to ASCII 'bzp2'. */
};


/** \brief Job description.
	This struct is used for both compression and decompression jobs. See the documentation
	for the function you are calling to see how the parameters are used. */
typedef struct JobDescription
{
	cell::fios::allocator *pAllocator;          //!< [in]     Allocator. (Not always necessary.)
	void *                   pContext;            //!< [in]     Subclass-specific context. (Not always necessary.)
	int                      level;               //!< [in]     Level to use when compressing.
	uint8_t *                pCompressed;         //!< [in,out] Compressed data.
	uint32_t                 compressedLength;    //!< [in,out] Compressed data length.
	uint8_t *                pDecompressed;       //!< [in,out] Decompressed data.
	uint32_t                 decompressedLength;  //!< [in,out] Decompressed data length.
	uint8_t *                pCopyOut;            //!< [in,out] Copy-out location when decompressing with partial copyout.
	uint32_t                 copyOutOffset;       //!< [in]     Copy-out offset when decompressing with partial copyout.
	uint32_t                 copyOutLength;       //!< [in]     Copy-out length when decompressing with partial copyout.
} JobDescription;

/** \brief Static initializer for a JobDescription struct, to zero it out. */
#define COMPRESSION_JOBDESCRIPTION_INITIALIZER \
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }

/** \brief Async task pointer, used for both compression and decompression tasks. */
typedef void * TaskPtr;

}; // namespace Compression
}; // namespace fios
}; // namespace cell

#endif /* __CELL_COMPRESSION_TYPES_H__ */

