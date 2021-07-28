/*
	SCE CONFIDENTIAL
	PlayStation(R)3 Programmer Tool Runtime Library 310.001
	Copyright (C) 2009 Sony Computer Entertainment Inc.
	All Rights Reserved.
*/
/**
	\file fios_memory.h

	Memory allocator API for the File I/O Scheduler.
*/
#ifndef __CELL_FIOS_MEMORY_H__
#define __CELL_FIOS_MEMORY_H__

#include <sys/types.h>

namespace cell {
namespace fios {

/** \brief Memory size type. */
typedef uint32_t MemSize;

/** \brief Attributes of memory.
  Definitions of each bit are as follows.<br>
  0 -  4bit #e_MEMALIGN    5bits<br>
  5 -  7bit #e_MEMOPTION   3bits<br>
  8 - 15bit #e_MEMPOOL     8bits<br>
 16 - 31bit #e_MEMPURPOSE 16bits<br>
*/
typedef uint32_t MemFlags;   // Allocator flags type.  Or together 0 or 1 each from MemAlign,
                        // MemOption, PoolID, or MemPurpose.

/** \brief Memory allocator API. */
class allocator
{
public:
    /** \brief Allocate memory.
        \param[in] size	 Size of memory.
        \param[in] flags Attributes of the memory.
        \param[in] pFile File name for debug.
        \param[in] line  Line number for debug.
        \return Pointer to the allocated memory.
     */
    virtual void* Allocate(MemSize size, MemFlags flags,
        const char* pFile = 0, int line = 0) = 0;

    /** \brief Deallocate memory.
        \param[in] pMemory Pointer to the memory.
        \param[in] flags   Attributes of the memory.
        \param[in] pFile File name for debug.
        \param[in] line  Line number for debug.
     */
    virtual void Deallocate(void* pMemory, MemFlags flags,
        const char* pFile = 0, int line = 0) = 0;

    /** \brief Reallocate memory.
        \param[in] pMemory Pointer to the old memory.
        \param[in] newSize Size of the new memory.
        \param[in] flags   Attributes of the memory.
        \param[in] pFile   File name for debug.
        \param[in] line    Line number for debug.
        \return Pointer to the newly allocated memory.
    */
    virtual void* Reallocate(void* pMemory, MemSize newSize,
        MemFlags flags, const char* pFile = 0, int line = 0) = 0;

    /** \brief Constructor. */
    allocator() {}
    /** \brief Deconstructor. */
    virtual ~allocator() {}
};

#ifndef DOXYGEN_IGNORE
// These are the bitfields within the memory flags.
enum e_MEMMASKS
{
    kAlignmentMask            = 0x0000001FU,
    kPoolIDMask               = 0x0000FF00U,
    kMemPurposeMask           = 0xFFFF0000U
};
#endif // DOXYGEN_IGNORE

/**
	Alignment field type. It is the exponent base two of the
	alignment, i.e.:  Alignment = 2 ^ value.
*/
enum e_MEMALIGN {
	kMemAlignDefault            = 0, //!<
	kMemAlignShift              = 0, //!<
	kMemAlignMask               = (0x1fU << kMemAlignShift), //!<
	kMemAlign8                  = (0x03U << kMemAlignShift), //!<
	kMemAlign4k                 = (0x0cU << kMemAlignShift), //!<
	kMemAlignCacheLine          = 0x00000007 //!<
       // cache line = 128 bytes = 2^7
};

/**
	\brief Memory pool id.
 */
enum e_MEMPOOL
{
    kPoolIDLongTerm           = 0x00000100U, //!< A memory pool id for long term use.
    kPoolIDMediumTerm         = 0x00000200U, //!< A memory pool id for medium term use.
    kPoolIDShortTerm          = 0x00000300U, //!< A memory pool id for short term use.

    kPoolIDShift              = 8U, //!<
};

/**
	Memory purpose field constants.  Describes usage of memory.
*/
enum e_MEMPURPOSE
{
    kMemPurposeDefault                          = 0U,
    kMemPurposeShift                            = 16U,
    kMemPurposeFIOSFirst                        = 0x10000000U,
    kMemPurposeFIOSLast                         = 0x10FF0000U,
    kMemPurposeCompressionFirst                 = 0x11000000U,
    kMemPurposeCompressionLast                  = 0x11FF0000U
};

#ifndef DOXYGEN_IGNORE
#define FIOS_ALIGNMENT_FROM_MEMFLAGS(f)              (1 << ((f) & cell::fios::kAlignmentMask))
#define FIOS_LOG2ALIGNMENT_FROM_MEMFLAGS(f)          ((f) & cell::fios::kAlignmentMask)
#define FIOS_MEMFLAGS_FROM_ALIGNMENT(a)              (FIOS_CONSTANT_LOG2(a) & cell::fios::kAlignmentMask)
#define FIOS_MEMFLAGS_FROM_LOG2ALIGNMENT(l2a)        ((l2a) & cell::fios::kAlignmentMask)
#define FIOS_POOLID(p)                               ((p) & cell::fios::kPoolIDMask)
#define FIOS_MEMPURPOSE(f)                           ((f) & cell::fios::kMemPurposeMask)


// The annotation is used by FIOS to report the source of its allocations.
#ifndef FIOS_MEMORY_ANNOTATION
# if FIOSDEBUG
#  define FIOS_MEMORY_ANNOTATION  __FILE__, __LINE__
# else
#  define FIOS_MEMORY_ANNOTATION  0, 0
# endif
#endif
#endif // DOXYGEN_IGNORE

/** Default allocator allocation flags. */
const MemFlags kMemFlagsDefault = 0;

#ifndef DOXYGEN_IGNORE
    // Extract alignment from flags.
#define FIOS_MEM_ALIGN_FROM_FLAGS(flags) \
    (1 << (((flags) & cell::fios::kMemAlignMask) >> cell::fios::kMemAlignShift))
#endif // DOXYGEN_IGNORE

/** Memory manager option field flag constants. */
enum e_MEMOPTION
{
    kMemOptionDefault            = 0x00,
    kMemOptionShift              = 5,
    kMemOptionMask               = (0x07U << kMemOptionShift),

        // Select high end of arena in two-sided heap.
    kMemOptionHighMemory         = (0x01U << kMemOptionShift),
        // Clear memory (i.e. calloc).
    kMemOptionClearMemory        = (0x02U << kMemOptionShift),

        // These are available for client usage
    kMemOptionFlag3              = (0x04U << kMemOptionShift),
};

#ifndef DOXYGEN_IGNORE
/**
	\internal
	In the memory API, FIOS has a bank of 256 memory purposes
	starting at kMemPurposeFIOSFirst.
*/
enum e_FIOS_MEMPURPOSES
{
	kMemPurposeFIOSMediaObject                       = kMemPurposeFIOSFirst+(0<<kMemPurposeShift),   //!< A media object. Failure causes media initialization to fail.
	kMemPurposeFIOSMediaIdentifierList               = kMemPurposeFIOSFirst+(1<<kMemPurposeShift),   //!< Array containing media identifiers. Failure causes media initialization to fail.
	kMemPurposeFIOSMediaIdentifier                   = kMemPurposeFIOSFirst+(2<<kMemPurposeShift),   //!< Individual media identifiers. Failure causes media initialization to fail.
	kMemPurposeFIOSMediaIOThreadStack                = kMemPurposeFIOSFirst+(3<<kMemPurposeShift),   //!< I/O thread stack, tunable via FIOS_IOTHREAD_STACKSIZE. Failure causes media initialization to fail.
	kMemPurposeFIOSMediaComputeThreadStack           = kMemPurposeFIOSFirst+(4<<kMemPurposeShift),   //!< Compute thread stack, tunable via FIOS_COMPUTETHREAD_STACKSIZE. Failure causes media initialization to fail.
	kMemPurposeFIOSCacheIndex                        = kMemPurposeFIOSFirst+(11<<kMemPurposeShift),  //!< In-RAM copy of the cache index. Failure disables the RAM cache.
	kMemPurposeFIOSRAMCache                          = kMemPurposeFIOSFirst+(12<<kMemPurposeShift),  //!< Storage for RAM cache blocks. Failure disables the RAM cache.
	kMemPurposeFIOSDiskCacheIOBuffers                = kMemPurposeFIOSFirst+(13<<kMemPurposeShift),  //!< I/O buffers for the disk cache. Failure disables the disk cache.
	kMemPurposeFIOSOverlayFD                         = kMemPurposeFIOSFirst+(14<<kMemPurposeShift),  //!< Tracking structure used when opening a file on a media-to-media overlay.
	kMemPurposeFIOSOverlayArray                      = kMemPurposeFIOSFirst+(15<<kMemPurposeShift),  //!< Array of overlay descriptors for the fios::overlay media filter layer.
	
	kMemPurposeFIOSSchedulerObject                   = kMemPurposeFIOSFirst+(16<<kMemPurposeShift),  //!< A scheduler object. Failure causes scheduler initialization to fail and is passed on to the caller of createSchedulerForMedia().
	kMemPurposeFIOSSchedulerStreamPages              = kMemPurposeFIOSFirst+(17<<kMemPurposeShift),  //!< Buffers for managed stream I/O. Size comes from a parameter to the scheduler's constructor. Failure means no space is used for managed streams.
	kMemPurposeFIOSSchedulerStreamPageTrackers       = kMemPurposeFIOSFirst+(18<<kMemPurposeShift),  //!< Array of structs to track the managed stream pages. Failure means no space is used for managed streams.
	kMemPurposeFIOSSchedulerPreallocatedOps          = kMemPurposeFIOSFirst+(19<<kMemPurposeShift),  //!< Array of preallocated op objects. Size comes from a parameter to the scheduler's constructor * sizeof(op). Failure means no ops will be preallocated.
	kMemPurposeFIOSSchedulerPreallocatedFilehandles  = kMemPurposeFIOSFirst+(20<<kMemPurposeShift),  //!< Array of preallocated filehandle objects. Size comes from a parameter to the scheduler's constructor * sizeof(filehandle). Failure means no filehandles will be preallocated.
	kMemPurposeFIOSSchedulerPreallocatedStreams      = kMemPurposeFIOSFirst+(21<<kMemPurposeShift),  //!< Array of preallocated stream objects. Size comes from a parameter to the scheduler's constructor * sizeof(stream). Failure means no streams will be preallocated.
	kMemPurposeFIOSSchedulerMediaIOParams            = kMemPurposeFIOSFirst+(22<<kMemPurposeShift),  //!< Array of I/O control structures and pcecoefficients to allow the scheduler to track N simultaneous I/O requests to the media. Failure causes scheduler initialization to fail and is passed on to the caller of createSchedulerForMedia().
	kMemPurposeFIOSSchedulerUnknownEstimates         = kMemPurposeFIOSFirst+(23<<kMemPurposeShift),  //!< Array of abstime_t values for the media's unknowns. Failure causes scheduler initialization to fail and is passed on to the caller of createSchedulerForMedia().
	kMemPurposeFIOSSchedulerMediaState               = kMemPurposeFIOSFirst+(24<<kMemPurposeShift),  //!< Most recent media state.  Failure causes scheduler initialization to fail and is passed on to the caller of createSchedulerForMedia().
	kMemPurposeFIOSSchedulerThreadStack              = kMemPurposeFIOSFirst+(25<<kMemPurposeShift),  //!< Scheduler thread stack, tunable via FIOS_SCHEDULERTHREAD_STACKSIZE. Failure causes scheduler initialization to fail and is passed on to the caller of createSchedulerForMedia().
	
	kMemPurposeFIOSWebDAVHTTPBuffer                  = kMemPurposeFIOSFirst+(26<<kMemPurposeShift),  //!< Temporary buffer used for a WebDAV HTTP request or response.
	kMemPurposeFIOSWebDAVDirectoryBuffer             = kMemPurposeFIOSFirst+(27<<kMemPurposeShift),  //!< Buffer used for WebDAV readDirectory().
	kMemPurposeFIOSOverlayReadDirectory              = kMemPurposeFIOSFirst+(28<<kMemPurposeShift),  //!< Array of direntry_t objects used by the fios::overlay layer to handle a complicated readDirectory request. Failure means the directory listing will not contain all items.
	
	kMemPurposeFIOSFilehandleObject                  = kMemPurposeFIOSFirst+(48<<kMemPurposeShift),  //!< Dynamically allocated filehandle object. Only used when kO_DYNALLOC is set, or when open filehandles >= preallocated filehandles. Failure is passed on to caller of openFile().
	kMemPurposeFIOSFilehandlePath                    = kMemPurposeFIOSFirst+(49<<kMemPurposeShift),  //!< Filehandle path, kept for the lifetime of the filehandle. This allocation may go away in the future. Failure is passed on to caller of openFile().
	kMemPurposeFIOSOpPath                            = kMemPurposeFIOSFirst+(50<<kMemPurposeShift),  //!< Op path, kept for the lifetime of the op. This allocation may go away in the future. Failure is passed on to caller (op fails with #CELL_FIOS_ERROR_NOMEM).
	kMemPurposeFIOSOpWaitMutex                       = kMemPurposeFIOSFirst+(51<<kMemPurposeShift),  //!< Op wait mutex singleton. Exists for the duration between FIOSInit and FIOSTerminate.
	kMemPurposeFIOSOpWaitCond                        = kMemPurposeFIOSFirst+(52<<kMemPurposeShift),  //!< Op wait cond singleton. Exists for the duration between FIOSInit and FIOSTerminate.
	
	kMemPurposeFIOSOpObject                          = kMemPurposeFIOSFirst+(64<<kMemPurposeShift),  //!< Dynamically allocated op. Only used when allocated ops >= preallocated ops. Failure is passed on to client who made the request.
	kMemPurposeFIOSThreadObject                      = kMemPurposeFIOSFirst+(65<<kMemPurposeShift),  //!< Thread object.
	kMemPurposeFIOSMutexObject                       = kMemPurposeFIOSFirst+(66<<kMemPurposeShift),  //!< Mutex object.
	kMemPurposeFIOSCondObject                        = kMemPurposeFIOSFirst+(67<<kMemPurposeShift),  //!< Cond object.
	kMemPurposeFIOSRWLockObject                      = kMemPurposeFIOSFirst+(68<<kMemPurposeShift),  //!< RWLock object.

	kMemPurposeFIOSPathStorage                       = kMemPurposeFIOSFirst+(80<<kMemPurposeShift),  //!< Path storage object, allocated once and kept globally.
	kMemPurposeFIOSPathStorageHash                   = kMemPurposeFIOSFirst+(81<<kMemPurposeShift),  //!< Path storage hashtable. May be reallocated to grow.
	kMemPurposeFIOSPathStorageArray                  = kMemPurposeFIOSFirst+(82<<kMemPurposeShift),  //!< Path storage array. May be reallocated to grow.
	kMemPurposeFIOSPathStoragePath                   = kMemPurposeFIOSFirst+(83<<kMemPurposeShift),  //!< Path storage path, allocated once and kept forever.
	
	kMemPurposeFIOSDearchiverArchiveTracker			 = kMemPurposeFIOSFirst+(96<<kMemPurposeShift),  //!< Archive tracker, one per open archive.
	kMemPurposeFIOSDearchiverArchiveTOC              = kMemPurposeFIOSFirst+(97<<kMemPurposeShift),  //!< Archive TOC, one per open archive. Size depends on the contents of the archive. Can be temporarily jettisoned via fios::dearchiver::freeTOCs().
	kMemPurposeFIOSDearchiverArchiveManifest         = kMemPurposeFIOSFirst+(98<<kMemPurposeShift),  //!< Archive manifest, used to fulfill readDirectory() requests. Size depends on the contents of the archive. Can be temporarily jettisoned via fios::dearchiver::freeManifests().
	kMemPurposeFIOSDearchiverArchiveManifestIndex    = kMemPurposeFIOSFirst+(99<<kMemPurposeShift),  //!< Index to all open archive manifests, used to fulfill readDirectory() requests. Size depends on the contents of the archive. Can be temporarily jettisoned via fios::dearchiver::freeManifests().
	kMemPurposeFIOSDearchiverIOBuffer                = kMemPurposeFIOSFirst+(100<<kMemPurposeShift), //!< Shared buffers used for I/O and decompression. There are three of these. Size is based on the archive's block size, which defaults to 64 KiB. Can be temporarily jettisoned via fios::dearchiver::freeBuffers().
	kMemPurposeFIOSDecompressorObject                = kMemPurposeFIOSFirst+(101<<kMemPurposeShift), //!< Decompressor object used by the fios::dearchiver layer.
	kMemPurposeFIOSDecompressorContext               = kMemPurposeFIOSFirst+(102<<kMemPurposeShift), //!< Decompression context used for an instance of fios::sync_decompressor.
	kMemPurposeFIOSDecompressorThreadStack           = kMemPurposeFIOSFirst+(103<<kMemPurposeShift)  //!< Decompressor thread stack, tunable via FIOS_COMPUTETHREAD_STACKSIZE.
};
#endif // DOXYGEN_IGNORE

}; /* namespace fios */
}; /* namespace cell */

#endif	//	__CELL_FIOS_MEMORY_H__
