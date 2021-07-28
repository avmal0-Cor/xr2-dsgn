/*
	SCE CONFIDENTIAL
	PlayStation(R)3 Programmer Tool Runtime Library 310.001
	Copyright (C) 2008 Sony Computer Entertainment Inc.
	All Rights Reserved.
*/
/**
	\file	edgelzma_decompressor.h
	
	EDGE LZMA decompression interface.
*/

#ifndef __CELL_EDGELZMA_DECOMPRESSOR_H__
#define __CELL_EDGELZMA_DECOMPRESSOR_H__

#include <cell/fios/compression/edge/edge_decompressor.h>
#include <cell/fios/compression/edge/edgelzma.h>

namespace cell {
namespace fios {
namespace Compression {

/**	\brief PS3 SPU sync/async decompressor for LZMA data.
	The EdgeLZMADecompressor class is a concrete subclass of #EdgeDecompressor,
	which is specialized to call the EDGE LZMA APIs and run decompression on the PS3 SPU.
	
	This class understands the same data format as the #LZMADecompressor
	class, and can be used as a drop-in replacement for it, subject to a few
	constraints. To create compressed data that this decompressor will understand,
	use #LZMACompressor.
	
	\note The EdgeLZMADecompressor REQUIRES that the correct, final size of the
	decompressed buffer is known in advance and passed in job.decompressedLength.
	The LZMADecompressor will accept any size that is large enough to hold the
	output data. But with EdgeLZMADecompressor you must know the EXACT output
	size in advance.
*/
class EdgeLZMADecompressor : public EdgeDecompressor
{
public:
	/** \brief Constructor with no arguments.
		You must manually call init() to set the CellSpurs and CellSpursTaskset before use. */
	EdgeLZMADecompressor() : EdgeDecompressor()
		{ m_dp.m_format = kLZMA; }

	/** \brief Constructor with CellSpurs pointer.
		\param[in] pSpurs   SPURS instance to use.
		A CellSpursTaskset will be automatically created. */
	EdgeLZMADecompressor(CellSpurs *pSpurs) : EdgeDecompressor()
		{ m_dp.m_format = kLZMA; init(pSpurs); }

	/** \brief Constructor with CellSpurs and CellSpursTaskset. 
		\param[in] pSpurs   SPURS instance to use.
		\param[in] pTaskset SPURS taskset to use. */
	EdgeLZMADecompressor(CellSpurs *pSpurs, CellSpursTaskset *pTaskset) : EdgeDecompressor()
		{ m_dp.m_format = kLZMA; init(pSpurs, pTaskset); }
	
	/** \brief Destructor. */
	virtual ~EdgeLZMADecompressor()
	{
		// Shut down the inflate queue.
		if (m_inflateQueue != NULL) {
			edgeLzmaShutdownInflateQueue(m_inflateQueue);
			m_inflateQueue = NULL;
		}
	}
	
#ifndef DOXYGEN_IGNORE
	/** \brief Returns the required task context save size.
		\return Size needed for task context. */
	virtual size_t getTaskContextSaveSize() const
	{
		return edgeLzmaGetInflateTaskContextSaveSize();
	}
	
	/** \brief Returns the buffer size needed for a given queue length.
		\param[in] inflateQueueLength  Queue length
		\return Buffer size needed for a queue of that length. */
	virtual size_t getInflateQueueSize(uint32_t inflateQueueLength) const
	{
		return edgeLzmaGetInflateQueueSize(inflateQueueLength);
	}
	
	/** \brief Creates the inflate queue.
		\return Inflate queue. */
	virtual void * createInflateQueue()
	{
		return edgeLzmaCreateInflateQueue(
			m_pSpurs,
			m_inflateQueueLength,
			m_queueBuffer,
			sizeof(m_queueBuffer));
	}

	/** \brief Creates the inflate SPU task.
		\return Inflate task. */
	virtual CellSpursTaskId createInflateTask()
	{
		return edgeLzmaCreateInflateTask(m_pTaskset, m_taskContext, m_inflateQueue);
	}

	/** \brief Adds an inflate queue element.
		\param[in]     pCompressed           Compressed data to expand.
		\param[in]     compressedLength      Length of the compressed data.
		\param[in]     pCopyOutLocation      Destination for partial copyout of decompressed data.
		\param[in]     copyOutOffset         Number of bytes to skip when doing the partial copyout.
		\param[in]     copyOutLength         Number of bytes to copy out.
		\param[in]     copyOutRemainder      Number of remainder bytes left over at the end of the decompressed data.
		\param[in,out] pWorkToDoCounter      Work-to-do flag, decremented when the work is complete.
		\param[in]     pEventFlag            Event flag to signal when the work is complete.
		\param[in]     eventFlagBits         Event flag bits which should be signalled. */
	virtual void addInflateQueueElementPartialCopyOut(
		const uint8_t * pCompressed,
		uint32_t  compressedLength,
		uint8_t * pCopyOutLocation,
		uint32_t  copyOutOffset,
		uint32_t  copyOutLength,
		uint32_t  copyOutRemainder,
		uint32_t *pWorkToDoCounter,
		CellSpursEventFlag *pEventFlag,
		uint16_t  eventFlagBits)
	{
        const uint8_t *pProperties = pCompressed;
        //const uint8_t *pSize = pCompressed + 5;
        const uint8_t *pRawStream = pCompressed + 5 + 8;
        
        // Check for streams that are too short
        if (compressedLength <= 13)
        {
            pRawStream = 0;
            compressedLength = 0;
        }
        else
            compressedLength -= 13;
        
        edgeLzmaAddInflateQueueElementPartialCopyOut(
            m_inflateQueue,
            pProperties, 5,
            pRawStream, compressedLength,
            pCopyOutLocation, copyOutOffset, copyOutLength, copyOutRemainder,
            pWorkToDoCounter, pEventFlag, eventFlagBits, kEdgeLzmaInflateTask_Inflate);
	}
#endif // DOXYGEN_IGNORE
};


}; /* namespace Compression */
}; /* namespace fios */
}; /* namespace cell */

#endif /*  __CELL_EDGELZMA_DECOMPRESSOR_H__ */

