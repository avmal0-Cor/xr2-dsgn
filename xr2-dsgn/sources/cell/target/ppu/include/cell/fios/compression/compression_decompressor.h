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

#ifndef __CELL_COMPRESSION_DECOMPRESSOR_H__
#define __CELL_COMPRESSION_DECOMPRESSOR_H__

#include <cell/fios/compression/compression_types.h>

namespace cell {
namespace fios {
/**	\brief Namespace for decompressors which imprement a specific compression algorithm. */
namespace Compression {


#ifndef DOXYGEN_IGNORE
/** \internal
	\brief The Decompressor object defines flags and settings
	common to both sync and async decompressors. */
class COMPRESSION_EXPORT Decompressor
{
public:
    Decompressor() {}
    virtual ~Decompressor() {}

    struct DecompressionParameters {
		/** \brief Data format that this decompressor can decode.
			\see e_FORMATS */ // in compression/include/compression_types.h
		uint32_t m_format;

        uint32_t m_sourceMaxSize;                     //!< Largest input that this decompressor can accept.

		uint32_t m_destinationMaxSize;                //!< Largest output that this decompressor can output.

        uint32_t m_contextAlignment : 8;              //!< Log-2 of the alignment needed for compression context.
        uint32_t m_contextSize : 24;                  //!< Size of the compression context required.

        uint32_t m_sourceAlignment : 8;               //!< Log-2 of the alignment needed for pSrc.
        uint32_t m_destinationAlignment : 8;          //!< Log-2 of the alignment needed for pDst.
        uint32_t m_flagDecompressInPlaceDMA : 1;            //!< Whether this decompressor can do in-place decompression, when the src/dst is DMA-safe memory
        uint32_t m_flagDecompressInPlaceNonDMA : 1;         //!< Whether this decompressor can do in-place decompression, when the src/dst is non-DMAable memory
        uint32_t m_flagDecompressStreamsToOutputDMA : 1;    //!< Whether this decompressor streams to its output, when the dst is DMA-safe memory
        uint32_t m_flagDecompressStreamsToOutputNonDMA : 1; //!< Whether this decompressor streams to its output, when the dst is non-DMAable memory
		uint32_t m_flagPartialCopyoutDMA : 1;               //!< Whether this decompressor can do a partial-copyout without a temporary buffer, when the dst is DMA-safe memory 
		uint32_t m_flagPartialCopyoutNonDMA : 1;            //!< Whether this decompressor can do a partial-copyout without a temporary buffer, when the dst is non-DMAable memory 
        uint32_t m_unused : 10;
    } m_dp;
};


/** \internal
	\brief The SyncDecompressor defines an interface for
	synchronous decompression. */
class COMPRESSION_EXPORT SyncDecompressor : public Decompressor
{
protected:
	SyncDecompressor() {}
public:
	virtual ~SyncDecompressor() {}
	
	/** \brief Decompresses a buffer of data.
		\param[in]     pJob            Job description.
		\return 0 for success. Any other value indicates a codec-specific error. */
	virtual int decompress(JobDescription *pJob) = 0;
};
#endif // DOXYGEN_IGNORE

/** \brief The AsyncDecompressor defines an interface for asynchronous decompression.
	An AsyncDecompressor can trivially be used as a SyncDecompressor. */
class COMPRESSION_EXPORT AsyncDecompressor : public SyncDecompressor
{
protected:
	/** Constructor. */
	AsyncDecompressor() {}
public:
	/** Deconstructor. */
	virtual ~AsyncDecompressor() {}

#ifndef DOXYGEN_IGNORE
	/** \internal
		\brief Starts an async decompression of a buffer of data.
		\param[in]     pJob            Job description.
		\return A cell::fios::Compression::TaskPtr which can be passed to decompressionTaskWait() to get the result. */
	virtual TaskPtr decompressAsync(JobDescription *pJob) = 0;
	
	/** \internal
		\brief Polls for async completion of a previous async decompression request.
		After this returns true, you must still call decompressionTaskWait to
		destroy the task and find out how big the decompressed data is.
		\param[in]     pTask           Pointer previously returned by decompressAsync.
		\return True if the decompression task is complete. */
	virtual bool decompressionTaskIsComplete(TaskPtr pTask) = 0;
	
	/** \internal
		\brief Waits for async decompression of a buffer of data to be complete.
		\param[in]     pTask             Pointer previously returned by decompressAsync.
		\param[out]    pDecompressedLen  Final size of the decompressed data.
		\return 0 for success. Any other value indicates a codec-specific error. */
	virtual int decompressionTaskWait(TaskPtr pTask, uint32_t *pDecompressedLen) = 0;
	
	// Trivial SyncDecompressor implementation
	virtual int decompress(JobDescription *pJob) {
		return decompressionTaskWait(decompressAsync(pJob),&pJob->decompressedLength);
	}
#endif // DOXYGEN_IGNORE
};
}; /* namespace Compression */
}; /* namespace fios */
}; /* namespace cell */

#endif /* __CELL_COMPRESSION_DECOMPRESSOR_H__ */

