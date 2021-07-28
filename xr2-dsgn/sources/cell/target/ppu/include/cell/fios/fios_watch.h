/*
	SCE CONFIDENTIAL
	PlayStation(R)3 Programmer Tool Runtime Library 310.001
	Copyright (C) 2009 Sony Computer Entertainment Inc.
	All Rights Reserved.
*/
/** \file fios_watch.h

	Class definitions for the fios::event class for use with resource watch.
*/

#ifndef __CELL_FIOS_WATCH_H__
#define __CELL_FIOS_WATCH_H__

// Control whether profile calls are enabled or not. Profiling adds about
// 15000 bytes, including conditional calls to profiling functions sprinkled
// throughout the FIOS code.
#if !defined(FIOS_PROFILING)
#define FIOS_PROFILING	1
#endif

#include <cell/fios/fios_common.h>

#if FIOS_PROFILING

namespace cell {
namespace fios {
namespace watch {

void ReportEventOp(const op *pOp, const char *type);
void ReportEventOpAccepted(const op *pOp);
void ReportEventOpExecuting(const op *pOp);
void ReportEventOpCompleted(const op *pOp);

// index_t should be used for the index argument here, but including
// fios_caching.h here causes problems:
void ReportEventCacheHit(const op *pOp, const char *pCacheID, off_t offset, off_t length, int32_t index);
void ReportEventCacheEvict(const op *pOp, const char *pCacheID, off_t offset, off_t length, int32_t index);
void ReportEventCacheFlush(const char *pCacheID, off_t offset, off_t length, int32_t index, const char *reason);
void ReportEventCacheFill(const op *pOp, const char *pCacheID, off_t offset, off_t length, int32_t index);
void ReportEventCacheReadthrough(const op *pOp, const char *pCacheID, off_t offset, off_t length, int32_t index);
void ReportEventCachePrefetchHit(const op *pOp, const char *pCacheID, off_t offset, off_t length, int32_t index);

void ReportEventArchiveOpen(const op *pOp);
void ReportEventArchiveClose(const op *pOp);
void ReportEventArchiveDecompressedCacheHit(const op *pOp, off_t offset, off_t length);
void ReportEventArchiveCompressedCacheHit(const op *pOp, off_t offset, off_t length);
void ReportEventArchiveCompressedRead(const op *pOp, off_t offset, off_t length);
void ReportEventArchiveUncompressedRead(const op *pOp, off_t offset, off_t length);
void ReportEventArchiveDecompressionStart(const op *pOp, off_t offset, off_t length);
void ReportEventArchiveDecompressionFinished(const op *pOp);

void ReportEventOverlay(const op *pOp, const char *pState, const char *pOrig, const char *pNew);

void ReportEventMediaAccess(const op *pOp, const char *pAPIName, const char *pNativePath);
void ReportEventMediaIO(const op *pOp,  const char *pAPIName, off_t length, const char *pNativePath);
void ReportEventMediaSeek(const op *pOp,  const char *pAPIName, off_t offset, const char *pNativePath);

}; /* namespace watch */
}; /* namespace fios */
}; /* namespace cell */

// Note, using: 
// #pragma warning(disable:4127)
// in this file did not work, so just use " {}" and no semicolons wherever
// these macros are referenced.

#define	FIOS_EVENT_OP_ACCEPTED(_pOp) \
	{ if (FIOS_UNLIKELY(g_FIOSParams.profiling & kProfileOps)) { \
		fios::watch::ReportEventOpAccepted(_pOp); \
	} }
#define	FIOS_EVENT_OP_EXECUTING(_pOp) \
	{ if (FIOS_UNLIKELY(g_FIOSParams.profiling & kProfileOps)) { \
		fios::watch::ReportEventOpExecuting(_pOp); \
	} }
#define	FIOS_EVENT_OP_COMPLETED(_pOp) \
	{ if (FIOS_UNLIKELY(g_FIOSParams.profiling & kProfileOps)) { \
		fios::watch::ReportEventOpCompleted(_pOp); \
	} }

#define	FIOS_EVENT_CACHE_HIT(_pOp, _pCacheID, _offset, _length, _index) \
	{ if (FIOS_UNLIKELY(g_FIOSParams.profiling & kProfileCache)) { \
		fios::watch::ReportEventCacheHit(_pOp, _pCacheID, _offset, _length, _index); \
	} }
#define	FIOS_EVENT_CACHE_EVICT(_pOp, _pCacheID, _offset, _length, _index) \
	{ if (FIOS_UNLIKELY(g_FIOSParams.profiling & kProfileCache)) { \
		fios::watch::ReportEventCacheEvict(_pOp, _pCacheID, _offset, _length, _index); \
	} }
#define	FIOS_EVENT_CACHE_FLUSH(_pCacheID, _offset, _length, _index, _reason) \
	{ if (FIOS_UNLIKELY(g_FIOSParams.profiling & kProfileCache)) { \
		fios::watch::ReportEventCacheFlush(_pCacheID, _offset, _length, _index, _reason); \
	} }
#define	FIOS_EVENT_CACHE_FILL(_pOp, _pCacheID, _offset, _length, _index) \
	{ if (FIOS_UNLIKELY(g_FIOSParams.profiling & kProfileCache)) { \
		fios::watch::ReportEventCacheFill(_pOp, _pCacheID, _offset, _length, _index); \
	} }
#define	FIOS_EVENT_CACHE_READ_THROUGH(_pOp, _pCacheID, _offset, _length) \
	{ if (FIOS_UNLIKELY(g_FIOSParams.profiling & kProfileCache)) { \
		fios::watch::ReportEventCacheReadthrough(_pOp, _pCacheID, _offset, _length, _index); \
	} }
#define	FIOS_EVENT_CACHE_PREFETCH_HIT(_pOp, _pCacheID, _offset, _length, _index) \
	{ if (FIOS_UNLIKELY(g_FIOSParams.profiling & kProfileCache)) { \
		fios::watch::ReportEventCachePrefetchHit(_pOp, _pCacheID, _offset, _length, _index); \
	} }

#define	FIOS_EVENT_ARCHIVE_OPEN(_pOp) \
	{ if (FIOS_UNLIKELY(g_FIOSParams.profiling & kProfileDearchiver)) { \
		fios::watch::ReportEventArchiveOpen(_pOp); \
	} }
#define	FIOS_EVENT_ARCHIVE_CLOSE(_pOp) \
	{ if (FIOS_UNLIKELY(g_FIOSParams.profiling & kProfileDearchiver)) { \
		fios::watch::ReportEventArchiveClose(_pOp); \
	} }
#define	FIOS_EVENT_ARCHIVE_DECOMPRESSED_CACHE_HIT(_pOp, _offset, _length) \
	{ if (FIOS_UNLIKELY(g_FIOSParams.profiling & kProfileDearchiver)) { \
		fios::watch::ReportEventArchiveDecompressedCacheHit(_pOp, _offset, _length); \
	} }
#define	FIOS_EVENT_ARCHIVE_COMPRESSED_CACHE_HIT(_pOp, _offset, _length) \
	{ if (FIOS_UNLIKELY(g_FIOSParams.profiling & kProfileDearchiver)) { \
		fios::watch::ReportEventArchiveCompressedCacheHit(_pOp, _offset, _length); \
	} }
#define	FIOS_EVENT_ARCHIVE_COMPRESSED_READ(_pOp, _offset, _length) \
	{ if (FIOS_UNLIKELY(g_FIOSParams.profiling & kProfileDearchiver)) { \
		fios::watch::ReportEventArchiveCompressedRead(_pOp, _offset, _length); \
	} }
#define	FIOS_EVENT_ARCHIVE_UNCOMPRESSED_READ(_pOp, _offset, _length) \
	{ if (FIOS_UNLIKELY(g_FIOSParams.profiling & kProfileDearchiver)) { \
		fios::watch::ReportEventArchiveUncompressedRead(_pOp, _offset, _length); \
	} }
#define	FIOS_EVENT_ARCHIVE_DECOMPRESSION_START(_pOp, _offset, _length) \
	{ if (FIOS_UNLIKELY(g_FIOSParams.profiling & kProfileDearchiver)) { \
		fios::watch::ReportEventArchiveDecompressionStart(_pOp, _offset, _length); \
	} }
#define	FIOS_EVENT_ARCHIVE_DECOMPRESSION_FINISHED(_pOp) \
	{ if (FIOS_UNLIKELY(g_FIOSParams.profiling & kProfileDearchiver)) { \
		fios::watch::ReportEventArchiveDecompressionFinished(_pOp); \
	} }

#define	FIOS_EVENT_OVERLAY(_pOp, _state, _orig, _new) \
	{ if (FIOS_UNLIKELY(g_FIOSParams.profiling & kProfileOverlay)) { \
		fios::watch::ReportEventOverlay(_pOp, _state, _orig, _new); \
	} }

#define	FIOS_EVENT_MEDIA_ACCESS(_pOp, _pAPIName, _pNativePath) \
	{ if (FIOS_UNLIKELY(g_FIOSParams.profiling & kProfileMediaAccess)) { \
		fios::watch::ReportEventMediaAccess(_pOp, _pAPIName, _pNativePath); \
	} }
#define	FIOS_EVENT_MEDIA_IO(_pOp, _pAPIName, _len, _pNativePath) \
	{ if (FIOS_UNLIKELY(g_FIOSParams.profiling & kProfileMediaAccess)) { \
		fios::watch::ReportEventMediaIO(_pOp, _pAPIName, _len, _pNativePath); \
	} }
#define	FIOS_EVENT_MEDIA_SEEK(_pOp, _pAPIName, _off, _pNativePath) \
	{ if (FIOS_UNLIKELY(g_FIOSParams.profiling & kProfileMediaAccess)) { \
		fios::watch::ReportEventMediaSeek(_pOp, _pAPIName, _off, _pNativePath); \
	} }

#else

// These macros have no effect if !FIOS_PROFILING:

#define	FIOS_EVENT_OP_ACCEPTED(_pOp) { }
#define	FIOS_EVENT_OP_EXECUTING(_pOp) { }
#define	FIOS_EVENT_OP_COMPLETED(_pOp) { }
#define	FIOS_EVENT_CACHE_HIT(_pOp, _pCacheID, _offset, _length, _index) { }
#define	FIOS_EVENT_CACHE_EVICT(_pOp, _pCacheID, _offset, _length, _index) { }
#define	FIOS_EVENT_CACHE_FLUSH(_pOp, _pCacheID, _offset, _length, _index) { }
#define	FIOS_EVENT_CACHE_FILL(_pOp, _pCacheID, _offset, _length, _index) { }
#define	FIOS_EVENT_CACHE_READTHROUGH(_pOp, _pCacheID, _offset, _length, _index) { }
#define	FIOS_EVENT_CACHE_PREFETCH_HIT(_pOp, _pCacheID, _offset, _length, _index) { }
#define	FIOS_EVENT_ARCHIVE_OPEN(_pOp) { }
#define	FIOS_EVENT_ARCHIVE_CLOSE(_pOp) { }
#define	FIOS_EVENT_ARCHIVE_DECOMPRESSED_CACHE_HIT(_pOp, _offset, _length) { }
#define	FIOS_EVENT_ARCHIVE_COMPRESSED_CACHE_HIT(_pOp, _offset, _length) { }
#define	FIOS_EVENT_ARCHIVE_COMPRESSED_READ(_pOp, _offset, _length) { }
#define	FIOS_EVENT_ARCHIVE_UNCOMPRESSED_READ(_pOp, _offset, _length) { }
#define	FIOS_EVENT_ARCHIVE_DECOMPRESSION_START(_pOp, _offset, _length) { }
#define	FIOS_EVENT_ARCHIVE_DECOMPRESSION_FINISHED(_pOp) { }
#define	FIOS_EVENT_OVERLAY(_pOp, _state, _orig, _new) { }
#define	FIOS_EVENT_MEDIA_ACCESS(_pOp, _pAPIName, _pNativePath) { }
#define	FIOS_EVENT_MEDIA_IO(_pOp, _pAPIName, _len, _pNativePath) { }
#define	FIOS_EVENT_MEDIA_SEEK(_pOp, _pAPIName, _off, _pNativePath) { }

#endif

#endif // __CELL_FIOS_WATCH_H__
