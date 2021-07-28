/*	
	SCE CONFIDENTIAL
	PlayStation(R)3 Programmer Tool Runtime Library 310.001
	Copyright (C) 2009 Sony Computer Entertainment Inc.
	All Rights Reserved.
*/
/**
	\internal
	\file fios_decompressor.h

	Class definitions for the fios::decompressor family of classes. The decompressor wraps
	the algorithms used by the fios::dearchiver class.
*/
#ifndef __CELL_FIOS_DECOMPRESSOR_H__
#define __CELL_FIOS_DECOMPRESSOR_H__

#include <cell/fios/fios_types.h>
#include <cell/fios/fios_base.h>
#include <cell/fios/fios_platform.h>
#include <cell/fios/compression/compression_decompressor.h>

namespace cell {
namespace fios {


// --------------------------------------------------------------------

/** \internal
	\brief Provides an interface to use a Compression::SyncDecompressor with FIOS.
	
	The SyncCompressor is run asynchronously on a simple background thread with low priority
	(specifically, as a #cell::fios::platform::kComputeThread). Jobs are queued and run one at
	a time.

	A single instance of this class can be shared among multiple dearchiver layers. */
class FIOS_LINKAGE async_decompression_wrapper : public Compression::AsyncDecompressor
{
public:
	FIOS_OBJECT_NEW_AND_DELETE(async_decompression_wrapper,kMemPurposeFIOSDecompressorObject|kPoolIDLongTerm)
	
	/** \brief Constructor from a Compression::SyncDecompressor.
		\param pDecompressor      Pointer to an instance of Compression::SyncCompressor. */
	async_decompression_wrapper(Compression::SyncDecompressor *pDecompressor);
	/** \brief Destructor. */
	virtual ~async_decompression_wrapper();

	/** \internal \brief Starts an async decompression of a buffer of data.
		\param[in]     pJob            Job description.
		\return A Compression::TaskPtr which can be passed to decompressionTaskWait() to get the result. */
	virtual Compression::TaskPtr decompressAsync(Compression::JobDescription *pJob);
	
	/** \internal \brief Polls for async completion of a previous async decompression request.
		After this returns true, you must still call decompressionTaskWait to
		destroy the task and find out how big the decompressed data is.
		\param[in]     pTask           Pointer previously returned by decompressAsync.
		\return True if the decompression task is complete. */
	virtual bool decompressionTaskIsComplete(Compression::TaskPtr pTask);
	
	/** \internal \brief Waits for async decompression of a buffer of data to be complete.
		\param[in]     pTask             Pointer previously returned by decompressAsync.
		\param[out]    pDecompressedLen  Final size of the decompressed data.
		\return 0 for success. Any other value indicates a codec-specific error. */
	virtual int decompressionTaskWait(Compression::TaskPtr pTask, uint32_t *pDecompressedLen);
	
	static void decompressorThreadEntry(platform::thread *pThread, void *pDecompressor);
	void decompressorThread();
	
protected:
	typedef struct sync_job
	{
		sync_job *                        m_pNext;
		err_t                             m_err;
		Compression::JobDescription m_job;
	} sync_job;
	enum { kMaxJobs = 4 };
	sync_job                       m_jobs[kMaxJobs];

	Compression::SyncDecompressor * m_pDecompressor;
	void *                         m_pDecompressorContext;
	platform::thread               m_decompressorThread;
	platform::mutex                m_mutex;
	platform::cond                 m_jobStartCond;
	platform::cond                 m_jobFinishCond;
	collections::list<sync_job*>   m_freeJobs;
	collections::list<sync_job*>   m_activeJobs;
	bool                           m_teardown;
	bool                           m_threadFinished;
};

// ------------------------------------------------------------------------

}; /* namespace fios */
}; /* namespace cell */

#endif // __CELL_FIOS_DECOMPRESSOR_H__
