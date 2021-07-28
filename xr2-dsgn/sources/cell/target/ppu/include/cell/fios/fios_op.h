/*
	SCE CONFIDENTIAL
	PlayStation(R)3 Programmer Tool Runtime Library 310.001
	Copyright (C) 2009 Sony Computer Entertainment Inc.
	All Rights Reserved.
*/
/**
	\file fios_op.h

	Class definitions for the fios::op class.
*/
#ifndef __CELL_FIOS_OP_H__
#define __CELL_FIOS_OP_H__

#include <cell/fios/fios_types.h>
#include <cell/fios/fios_common.h>
#include <cell/fios/fios_base.h>
#include <cell/fios/fios_platform.h>
#include <cell/fios/fios_time.h>
#include <cell/fios/fios_watch.h>
#include <stdarg.h>

namespace cell {
namespace fios {

// Forward declarations
class opWaitData;

/**
	The operation object, which is returned by every async operation.
*/
class FIOS_LINKAGE op
{
public:
	/** \brief Waits for an operation to complete.

	    This is a special sync variant of wait which deletes the operation afterward.
		\param[out]  pFulfilledSize    Returns the fulfilled size, as in #getFulfilledSize(). May be NULL.
	    \return The result of the operation. */
	err_t syncWait(
		off_t *pFulfilledSize = NULL);
	
#ifndef DOXYGEN_IGNORE
	/**
		\brief Waits for a va_list of arguments to complete with a deadline.

		If all operations complete successfully, the result is #CELL_FIOS_NOERROR.
		If at least one operation did not complete by the deadline, the
		result is #CELL_FIOS_ERROR_TIMEOUT. Otherwise, the result code is
		the error from one of the operations in the list that completed
		with an error.
		\param[in] deadline  Deadline by which the operations must complete, or 0 to wait forever.
		\param[in] pOp       First argument in the list.
		\param[in] ap        Argument list.
		\return #CELL_FIOS_NOERROR if all operations completed successfully, #CELL_FIOS_ERROR_TIMEOUT if at least one operation did not complete by the deadline, or otherwise an error from some operation that finished with an error.
		\see #waitListDeadline(), #waitListTimeout(), #waitList(), #waitMultipleDeadline(), #waitMultipleTimeout(), #waitMultiple(), #wait(), #waitDeadline(), #waitTimeout()
	*/
	static err_t vwaitMultipleDeadline(
		abstime_t deadline,
		op *pOp,
		va_list ap);
#endif // DOXYGEN_IGNORE

	/**
		\brief Waits for all operation arguments to complete with a deadline.

		If all operations complete successfully, the result is #CELL_FIOS_NOERROR.
		If at least one operation did not complete by the deadline, the
		result is #CELL_FIOS_ERROR_TIMEOUT. Otherwise, the result code is
		the error from one of the operations in the list that completed
		with an error.
		\param[in] deadline  Deadline by which the operations must complete, or 0 to wait forever.
		\param[in] pOp       Operation handle.
		\param[in] ...       List of additional operations, terminated by NULL.
		\return #CELL_FIOS_NOERROR if all operations completed successfully, #CELL_FIOS_ERROR_TIMEOUT if at least one operation did not complete by the deadline, or otherwise an error from some operation that finished with an error.
		\see #waitListDeadline(), #waitListTimeout(), #waitList(), #waitMultipleTimeout(), #waitMultiple(), #wait(), #waitDeadline(), #waitTimeout()
	*/
	static err_t waitMultipleDeadline(
		abstime_t deadline,
		op *pOp,
		...) FIOS_ATTRIBUTE_SENTINEL;
	
	/**
		\brief Waits for all operation arguments to complete with a relative timeout.

		If all operations complete successfully, the result is #CELL_FIOS_NOERROR.
		If at least one operation did not complete before the timeout
		interval elapsed, the result is #CELL_FIOS_ERROR_TIMEOUT. Otherwise, the
		result code is the error from one of the operations in the list that
		completed with an error.
		\param[in] timeout   Timeout interval, or 0 to wait forever.
		\param[in] pOp       Operation handle.
		\param[in] ...       List of additional operations, terminated by NULL.
		\return #CELL_FIOS_NOERROR if all operations completed successfully, #CELL_FIOS_ERROR_TIMEOUT if at least one operation did not complete by the end of the timeout interval, or otherwise an error from some operation that finished with an error.
		\see #waitListDeadline(), #waitListTimeout(), #waitList(), #waitMultipleDeadline(), #waitMultiple(), #wait(), #waitDeadline(), #waitTimeout()
	*/
	static err_t waitMultipleTimeout(
		abstime_t timeout,
		op *pOp,
		...) FIOS_ATTRIBUTE_SENTINEL;
	
	/**
		\brief Waits for all operation arguments to complete.

		If all operations complete successfully, the result is #CELL_FIOS_NOERROR.
		Otherwise, the result code is the error from one of the operations
		in the list that completed with an error.
		\param[in] pOp       Operation handle.
		\param[in] ...       List of additional operations, terminated by NULL.
		\return #CELL_FIOS_NOERROR if all operations completed successfully, otherwise an error from some operation that finished with an error.
		\see #waitListDeadline(), #waitListTimeout(), #waitList(), #waitMultipleDeadline(), #waitMultipleTimeout(), #wait(), #waitDeadline(), #waitTimeout()
	*/
	static err_t waitMultiple(
		op *pOp,
		...) FIOS_ATTRIBUTE_SENTINEL;
	
	/** \brief Waits for a list of operations to complete with a deadline.

		If all operations complete successfully, the result is #CELL_FIOS_NOERROR.
		If at least one operation did not complete by the deadline, the
		result is #CELL_FIOS_ERROR_TIMEOUT. Otherwise, the result code is
		the error from one of the operations in the list that completed
		with an error.
		\param[in] numOps    Number of operations to wait for.
		\param[in] pOpList   List of operations.
		\param[in] deadline  Deadline by which the operations must complete, or 0 to wait forever.
		\return #CELL_FIOS_NOERROR if all operations completed successfully, #CELL_FIOS_ERROR_TIMEOUT if at least one operation did not complete by the deadline, or otherwise an error from some operation that finished with an error.
		\see #waitListTimeout(), #waitList(), #waitMultipleDeadline(), #waitMultipleTimeout(), #waitMultiple(), #wait(), #waitDeadline(), #waitTimeout()
	*/
	static err_t waitListDeadline(
		unsigned int numOps,
		op * const *pOpList,
		abstime_t deadline);
	
	/** \brief Waits for a list of operations to complete with a relative timeout.

		If all operations complete successfully, the result is #CELL_FIOS_NOERROR.
		If at least one operation did not complete before the timeout
		interval elapsed, the result is #CELL_FIOS_ERROR_TIMEOUT. Otherwise, the
		result code is the error from one of the operations in the list that
		completed with an error.
		\param[in] numOps    Number of operations to wait for.
		\param[in] pOpList   List of operations.
		\param[in] timeout   Timeout interval, or 0 to wait forever.
		\return #CELL_FIOS_NOERROR if all operations completed successfully, #CELL_FIOS_ERROR_TIMEOUT if at least one operation did not complete by the end of the timeout interval, or otherwise an error from some operation that finished with an error.
		\see #waitListDeadline(), #waitList(), #waitMultipleDeadline(), #waitMultipleTimeout(), #waitMultiple(), #wait(), #waitDeadline(), #waitTimeout()
	*/
	static inline err_t waitListTimeout(
		unsigned int numOps,
		op * const *pOpList,
		abstime_t timeout) { return waitListDeadline(numOps,pOpList,timeoutToDeadline(timeout)); }
	
	/** \brief Waits for a list of operations to complete.

		If all operations complete successfully, the result is #CELL_FIOS_NOERROR.
		Otherwise, the result code is the error from one of the operations in
		the list that completed with an error.
		\param[in] numOps    Number of operations to wait for.
		\param[in] pOpList   List of operations.
		\return #CELL_FIOS_NOERROR if all operations completed successfully, #CELL_FIOS_ERROR_TIMEOUT if at least one operation did not complete by the deadline, or otherwise an error from some operation that finished with an error.
		\see #waitListDeadline(), #waitListTimeout(), #waitMultipleDeadline(), #waitMultipleTimeout(), #waitMultiple(), #wait(), #waitDeadline(), #waitTimeout()
	*/
	static inline err_t waitList(
		unsigned int numOps,
		op * const *pOpList) { return waitListDeadline(numOps,pOpList,0); }

	/** \brief Waits for an operation to complete with a deadline.

		If the operation has not completed by the deadline, the
		result is #CELL_FIOS_ERROR_TIMEOUT. Otherwise, the result code is the
		result from the operation.
		\param[in] deadline  Deadline, or 0 to wait forever.
		\return Result from the operation.
		\see #waitListDeadline(), #waitListTimeout(), #waitList(), #waitMultipleDeadline(), #waitMultipleTimeout(), #waitMultiple(), #wait(), #waitTimeout()
	*/
	inline err_t waitDeadline(abstime_t deadline) { op *pOp = this; return op::waitListDeadline(1,&pOp,deadline); }
	
	/** \brief Waits for an operation to complete with a relative timeout.
		\param[in] timeout   Timeout interval, or 0 to wait forever.
		\return Result from the operation.
		\see #waitListDeadline(), #waitListTimeout(), #waitList(), #waitMultipleDeadline(), #waitMultipleTimeout(), #waitMultiple(), #wait(), #waitDeadline()
	*/
	inline err_t waitTimeout(abstime_t timeout) { op *pOp = this; return op::waitListDeadline(1,&pOp,timeoutToDeadline(timeout)); }
	
	/** \brief Waits for an operation to complete.
		\return Result from the operation.
		\see #waitListDeadline(), #waitListTimeout(), #waitList(), #waitMultipleDeadline(), #waitMultipleTimeout(), #waitMultiple(), #waitDeadline(), #waitTimeout()
	*/
	inline err_t wait() { op *pOp = this; return op::waitListDeadline(1,&pOp,0); }
	
	/** \brief Returns the error code for an operation. If the operation has not completed, returns CELL_FIOS_ERROR_INPROGRESS.
		\return Result from the operation.
		\see op::wait
	*/
	inline err_t getError() { return isDone() ? wait() : CELL_FIOS_ERROR_INPROGRESS; }
	
	/** \brief Cancels an operation.
		\see scheduler::cancelOp()
	*/
	void cancel(); // inlined in fios_scheduler.h
	
#ifndef DOXYGEN_IGNORE
	/** \brief Placement new.
		\param[in] size         Size to allocate.
		\param[in] pPlacement   Placement.
		\return Returns pPlacement.
	*/
	static void * operator new(size_t size, void *pPlacement) { FIOS_UNUSED(size); return pPlacement; }
	
	/** \brief Placement delete.
		\param[in] pMem         Pointer to deallocate.
		\param[in] pPlacement   Placement.
	*/
	static void operator delete(void *pMem, void *pPlacement) { FIOS_UNUSED(pMem); FIOS_UNUSED(pPlacement); }
	
	/** \brief Frees an operation pointer.
		\param[in] pMem   Operation pointer to delete.
	*/
	static void operator delete(void *pMem); // inlined in fios_scheduler.h
#endif // DOXYGEN_IGNORE
	
	/** \brief Indicates whether an operation is complete.
		\return True if the operation has completed, false otherwise. */
	inline bool isDone() const { return (m_done != 0); }
	
	/** \brief Checks the cancellation status of this operation.
		This function does a non-atomic load since the cancel state is never used as a gate; it's
		okay if we get a slightly stale value.
		\return True if the operation has been cancelled, false otherwise. */
	inline bool isCancelled() const { return FIOS_UNLIKELY((m_opflags.m_value & kOPF_CANCELLED) == kOPF_CANCELLED); }
	
	/** \brief Indicates whether an operation is for a prefetch or not.
		\return True if the operation is a prefetch. */
	inline bool isPrefetch() const { return ((m_opStage == op::kOPSTAGE_PREFETCH_OPEN) ||
				(m_opStage == op::kOPSTAGE_PREFETCH_CONTINUE) || (m_opStage == op::kOPSTAGE_PREFETCH_CLOSE));
	}
	
	/**
		\brief  Returns the requested data size in bytes.

		This call returns the requested data size from the original call. For calls such
		as scheduler::readFile(), this is the number of bytes requested. For calls which
		return a file handle or other object, this is the size of the output parameter.
		\result              Request size in bytes.
	*/
	inline off_t getRequestedSize() const { return m_opReqCount; }

	/**
		\brief  Returns the actual fulfilled size in bytes.

		This call returns the number of bytes from the request that have been retrieved
		so far. If the operation has completed successfully, this is equal to the
		request size. If the operation completed with error code #CELL_FIOS_ERROR_EOF, this
		may be any value from 0 to the request size according to how much data was read
		before the end of the file. If an operation is not complete, this may be any
		value from 0 to the request size.
		\result              Actual transfer size in bytes.
	*/
	off_t getFulfilledSize() const;
	
	/**
		\brief  Returns the original request offset in bytes.

		This call returns the requested offset from the original call.
		\result              Offset from the original call, or 0 if not applicable. */
	inline off_t getOffset() const { return m_opOffset; }
	
	/**
		\brief  Gets the user pointer.
		\return The user pointer set at creation.
	*/
	void * getUserPtr() const { return m_attr.userPtr; }
	
	/**
		\brief  Gets the user tag.
		\return The user tag set at creation.
	*/
	uint32_t getUserTag() const { return m_attr.userTag; }

	/**
		\brief  Returns the data buffer.

		This call returns the data buffer for the operation. For operations such as
		scheduler::readFile() and scheduler::writeFile(), this is the buffer supplied
		by the caller. For operations which return a file handle or other object, this
		is a pointer to the output parameter.
		\return  Data buffer.
	*/
	inline void * getBuffer() const { return m_opBuffer; }

	/**
		\brief  Returns the deadline.
		\return  This operation's deadline.
	*/
	inline abstime_t getDeadline() const { return m_attr.deadline; }

	/**
		\brief  Returns the priority of an operation.
		\return Priority value.
	*/
	inline prio_t getPriority() const { return m_attr.priority; }
	
	/**
		\brief  Returns the flags of an operation.
		\return Operation flags.
		\see e_OPFLAGS
	*/
	inline uint32_t getFlags() const { return m_attr.opflags; }
	
	/**
		\brief  Returns a pointer to the license key.
		\return Address of the license key.
	*/
	const SceNpDrmKey *getLicense() const { return m_attr.pLicense; }

	/**
		\brief	Returns a unique serial number for this operation.

		Each client request, such as scheduler::readFile(), receives a unique serial
		number that remains active until the op is complete and deleted.
		This is primarily useful for debug logging.
		
		Serial numbers start at 1 and are incremented with each new request.
		It's theoretically possible for them to wrap if you issue more than
		2^32 ops. This falls into the realm of "highly unlikely" since
		it'd require allocating a new op every 10ms for over a year.
		\return Unique serial number.
	*/
	inline uint32_t getSerialNumber() const { return m_serialNumber; }
	
	/** \brief Returns the op's parent scheduler.
		\return fios::scheduler object. */
	inline fios::scheduler * getScheduler() const { return m_pScheduler; }
	
	/** \brief Returns the path for this operation. */
	inline const char * getPath() const { return m_path; }

	/**
		\brief  Returns the estimated time at which an operation will complete.

		This call returns the scheduler's current best estimate of when the operation
		will be complete.  This value may change at any time, either because additional
		I/O requests were received or because the scheduling algorithm finds an optimization
		opportunity.
		
		\note
		The scheduler's optimization algorithm does not always push operations forward. Any
		individual operation may be pushed later in the schedule, rather than earlier, as
		long as it does not violate any deadlines and results in a shorter overall time for
		the list of scheduled I/Os as a whole.
		
		\return   The current estimated time that the operation will complete.
	*/
	abstime_t getEstimatedCompletion() const; // inlined in fios_scheduler.h

#ifndef DOXYGEN_IGNORE
	/**
		\brief  Sets a new deadline.

		This causes the operation to be rescheduled with the new deadline.  If the
		operation is currently executing then this call has no effect.
		\param[in]  deadline   The new deadline.
	*/
	void setDeadline(
		abstime_t deadline); // inlined in fios_scheduler.h
	
	/**
		\brief  Sets a new priority.
		\param[in]  priority   The new priority.
	*/
	void setPriority(
		prio_t priority); // inlined in fios_scheduler.h

	/**
		\brief  Sets a new priority and deadline.
		\param[in]  deadline   The new deadline.
		\param[in]  priority   The new priority.
	*/
	void setDeadlineAndPriority(
		abstime_t deadline,
		prio_t priority); // inlined in fios_scheduler.h
	
	/**
		\brief Invokes the operation callback.
		\param[in]     event               Event type.
		\param[in,out] pParam              Pointer to event-specific parameters.
		\return Error returned by the callback.
	*/
	err_t callback(
		opevent_t event,
		void *pParam = NULL);

private:
	/** \brief Stage definitions, for operations that require more than one operation. */
	typedef enum e_OPSTAGES {
		kOPSTAGE_INVALID = 0,             /**< Invalid stage. */
		kOPSTAGE_CALLBACK = 1,            /**< The last stage for many operations. After the mediarequest completes, the callback is called. */
		kOPSTAGE_EOFCALLBACK,             /**< Same as kOPSTAGE_CALLBACK, but changes the return value to CELL_FIOS_ERROR_EOF. */
		kOPSTAGE_GETFILESIZE_STAT,        /**< kMEDIAACTION_STAT for getFileSize, output translation must be done. */
		kOPSTAGE_FILEEXISTS_STAT,         /**< kMEDIAACTION_STAT for fileExists, output translation must be done. */
		kOPSTAGE_DIREXISTS_STAT,          /**< kMEDIAACTION_STAT for directoryExists, output translation must be done. */
		kOPSTAGE_ITEMEXISTS_STAT,         /**< kMEDIAACTION_STAT for itemExists, output translation must be done. */
		kOPSTAGE_OPENFILE_OPEN,           /**< open stage of openFile, output translation must be done. */
		kOPSTAGE_CLOSEFILE_DEALLOCATE,    /**< deallocate stage of closeFile, deallocate the FH */
		kOPSTAGE_PREFETCH_OPEN,           /**< start prefetch - open the file */
		kOPSTAGE_PREFETCH_CONTINUE,       /**< handled next part of a prefetch request, check for completion */
		kOPSTAGE_PREFETCH_CLOSE           /**< finish a prefetch request, close the file */
	} opstage_t;
	
	friend class scheduler;               /**< This class is manipulated directly by the scheduler class. */
	friend class collections::list<op*>;       /**< The list class needs our next ptr. */
	friend class collections::atomicList<op*>; /**< The atomic list class needs our next ptr. */
	friend void watch::ReportEventOp(const op*, const char*);
	friend void watch::ReportEventOpCompleted(const op*);

	mutable op *   m_pNext;               /**< Next pointer for linked lists */
	opattr_t       m_attr;                /**< This operation's attributes */
	platform::atomicU32 m_opflags;        /**< Operation flags, including those set after the client's opflags have been set. */
	path_t         m_path;                /**< Normalized path for this operation */
	uint32_t       m_serialNumber;        /**< Unique serial number for this op */
	mediarequest   m_request;             /**< Current media request represented by this operation. One operation may generate multiple media requests! */
	platform::atomicBool   m_done;        /**< Whether this operation has completed. */
	scheduler      *m_pScheduler;         /**< Scheduler associated with this operation. */
	filehandle     *m_pFH;                /**< Filehandle, if this op is associated with a filehandle (open, read, write, close). */
	off_t          m_opReqCount;          /**< Original request size (may not be equal to request size) */
	off_t          m_opActCount;          /**< Actual bytes transferred */
	off_t          m_opOffset;            /**< Original offset (may not be equal to request offset) */
	void *         m_opBuffer;            /**< Original buffer (may not be equal to request buffer) */
	abstime_t      m_estimatedCompletion; /**< Estimated completion time */
	opstage_t      m_opStage;             /**< For multi-stage ops, the current stage. */
	err_t          m_stickyErr;           /**< For multi-stage ops, an error preserved from an earlier stage. */
	collections::list<opWaitData *>  m_pWaitData;  /**< Info on threads waiting for this op in a wait() call. Protected by s_pWaitMutex. */
	
	/** \brief Resets an operation and makes it ready for reuse */
	void reset();
	
	/** \brief Copies data from a completed op
		\param[in] pCompletedOp    Recently completed op that may overlap this one.
		\return TRUE if this operation was completed by the given data */
	bool copyDataFrom(op *pCompletedOp);
	
	/** \brief Used by the implementation of #waitListDeadline().
		\param[in] pOpList   List of operations
		\param[in] numOps    Number of entries in pOpList
		\param[in] pWaitData Wait data to add or remove
		\param[in] add       True = add the waitData to the ops, false = remove the waitData from the ops. */
	static void addOrRemoveWaitData(op * const *pOpList, unsigned int numOps, opWaitData *pWaitData, bool add);
	
	/** \internal
		\brief Converts a timeout interval to an absolute deadline.
		\param[in] timeout        Timeout interval, or 0 for no timeout.
		\return Deadline value, or 0 if no timeout. */
	static abstime_t timeoutToDeadline(abstime_t timeout);
#endif // DOXYGEN_IGNORE
	
public:
#ifndef DOXYGEN_IGNORE
	/** \internal */
	static platform::atomicPtr<platform::mutex *>   s_pWaitMutex;    //!< \internal Mutex used for #wait().
	/** \internal */
	static platform::atomicPtr<platform::cond *>    s_pWaitCond;     //!< \internal Cond used for #wait().
	/** \internal */
	static void initializeStatics(platform::mutex **pOutMutex = NULL, platform::cond **pOutCond = NULL);
	/** \internal */
	static void terminateStatics();

	/** \internal
		\brief Constructor. Sets a check value in debug builds.
		\param[in] pPathBuffer    Buffer to use when FIOS is configured for static paths. */
	op(char *pPathBuffer);
	/** \internal
		\brief Check value */
	uint32_t m_checkValue;
	/** \internal
		\brief Class ID */
	uint32_t m_classID;
#endif // DOXYGEN_IGNORE
}; /* class op */
}; /* namespace fios */
}; /* namespace cell */

#endif /* __CELL_FIOS_OP_H__ */
