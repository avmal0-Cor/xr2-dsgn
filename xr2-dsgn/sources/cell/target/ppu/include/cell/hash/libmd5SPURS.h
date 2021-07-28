/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2006-2007 Sony Computer Entertainment Inc.
* All Rights Reserved.
*/


//DOM-IGNORE-BEGIN
#ifndef _CELL_LIBMD5_SPU_H_
#define _CELL_LIBMD5_SPU_H_


#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
extern "C" {
#endif	/* defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus) */


#define CELL_MD5_DIGEST_SIZE    16

#include "libhash.h"

#define CELL_MD5_BUFFQSIZE 32
//DOM-IGNORE-END

typedef struct {
	CellSpursTaskId task_id;
	CellSpursTaskset *taskset;
	CellSpursQueue *queue;
	CellSpursEventFlag *event;
	void *context_store;
	void *qdata;
	int free_flags;
} CellMd5SpuInstance;//*** Note to editor, the contents of this structure should be hidden, in the same way that CellSpurs is ***
//Summary:
//Instance of an MD5 SPU task
//
//Description:
//This is the datatype of a MD5 SPU instance, and holds information about the SPURS usage for this particular instance.
//It is not necessary or appropriate for the application to handle the members of this datatype directly.




/* Summary
   Create a MD5 SPURS task.
   Conditions
   Multi-thread safe.
   Parameters
   md5 :         Pointer to a MD5 SPURS work area.
   taskset :        Pointer to a SPURS task set instance.
   queue :          Address of a SPURS event queue instance that can be
                    used by this SPURS task, or NULL if this should be
                    allocated dynamically.
   event :          Address of a SPURS event flag instance that can be used
                    by this SPURS task, or NULL if this should be allocated
                    dynamically.
   context_store :  Address of the context storage area (16\-byte aligned,
                    preferably 128\-byte aligned), or NULL if this should
                    be allocated dynamically.
   queue_depth :    Number of entries in the queue. This should be at least
                    1+(number of buffers).
   Returns
   <table>
   Macro                                   Value        \Description
   -------------------------------------   -----------  --------------------------
   CELL_OK                                 0x00000000   Function returned successfully
   CELL_ERROR_HASH_NULL_POINTER            0x80f00001   <c>taskset</c> or <c>md5</c>
                                                         is a null pointer.
   CELL_ERROR_HASH_QUEUE_CREATE_FAILED     0x80f00002   SPURS event queue could not be created, 
                                                         or the event queue could not be 
													     attached to a SPURS queue.
   CELL_ERROR_HASH_FLAG_CREATE_FAILED      0x80f00003   SPURS event flag could not be created, 
                                                         or the event flag could not be attached
													     to a SPURS queue.
   CELL_ERROR_HASH_INVALID_ALIGN           0x80f00004   <c>queue</c> or <c>event</c> is not 
                                                         aligned to a 128\-byte boundary, or 
													     <c>context_store</c> is not aligned to 
													     a 16\-byte boundary.
   CELL_ERROR_HASH_INVALID_NOMEM           0x80f00005   Insufficient memory.
   CELL_ERROR_HASH_TASK_CREATE_FAILED      0x80f00006   Failed to create SPURS task.

   </table>
                                         

   Description
   This function creates a new SPURS task for calculating the
   MD5 hash value of some data. For <c>md5</c> and <c>taskset</c>,
   these should be provided to the function and allocated and
   initialized accordingly.
   
   The structure <c>md5</c> is used to hold information about
   the MD5 SPURS task that is created by this function and
   should not be altered.
   
   For <c>queue</c>, <c>event</c>, <c>context_store</c>, these
   can either be provided to the function, or set to NULL. When
   one or more of these parameters is set to NULL, the memory
   required by the parameter(s) would be created
   during the initialization process. Internally, <c>memalign</c>
   is used to allocate memory, so if you want to use your own
   memory allocation functions, then memory should be allocated
   to these parameters before calling this function.
   
   For efficiency, ensure that the queue depth is at least the
   same size as the number of buffers that you will send as well
   as one extra buffer for ending the task.
   See Also
   <c>cellMd5SpuEndTask</c>                                                                              */
int cellMd5SpuCreateTask(CellMd5SpuInstance *md5,
						  CellSpursTaskset *taskset,
						  CellSpursQueue *queue, 
						  CellSpursEventFlag *event, 
						  void *context_store,
						  int queue_depth);

/* Summary
   Process a chunk of data.
   Conditions
   Multi-thread safe.
   Parameters
   md5 :  Pointer to a MD5 SPURS work area
   data :    Effective address of data to be processed on the PPU
             (16\-byte aligned)
   size :    Number of bytes to process
   flag :    Event flag which is set once processing is complete
   wait :    Waiting mode
   Returns
   <c>CELL_OK</c>(0) is returned for success.

   If <c>md5==NULL</c> or <c>data==0</c>, then 
   <c>CELL_ERROR_HASH_NULL_POINTER</c> is returned.

   If any other error occurs, an error code from <c>cellSpursQueuePush</c>
   (if <c>wait==1</c>) or cell <c>SpursQueueTryPush</c> (if <c>wait==0</c>)
   is returned.
   Description
   This function adds more data to the MD5 SPURS task for
   processing. If the task is already processing data, then this
   request will be queued for later execution.
   
   If <c>wait</c> is set to 1, the function will not return
   until the new data can be added to the SPURS queue.
   
   If <c>wait</c> is set to 0, the function will return
   immediately, although if there is no space in the SPURS queue, an
   error will be returned.
   
   For <c>flag,</c> this should be a bit pattern that represents
   this process. Typically you would use <c>1\<\<buffer_number</c>.
   This flag is used to check for the status of the transfer in <c>cellMd5SpuProcessData</c>.
   Note
   For efficiency ensure that the SPURS queue is at least the same
   size as the number of buffers that you will send as well as
   one extra for ending the task.
   
   The data may not be processed immediately, so the data should
   not be overwritten or changed until the status of this
   operation is checked by calling <c>cellMd5SpuCheckStatus</c>.
   See Also
   <c>cellMd5SpuCreateTask</c>, <c>cellMd5SpuCheckStatus</c>                               */
int cellMd5SpuProcessData(CellMd5SpuInstance *md5,
						   uint64_t data,
						   unsigned int size,
						   uint16_t flag,
						   int wait);

/* Summary
   Check status of an MD5 process.
   Conditions
   Multi-thread safe.
   Parameters
   md5 :  Pointer to a MD5 SPURS work area.
   flag :    Event flag for the process that is being checked.
   wait :    Waiting mode.
   Returns
   <c>CELL_OK</c>(0) is returned for success.

   If <c>md5==NULL</c>, then <c>CELL_ERROR_HASH_NULL_POINTER</c> is returned.

   If any other error occurs, an error code from <c>cellSpursEventFlagWait</c>
   (if <c>wait==1</c>) or <c>cellSpursEventFlagTryWait</c> (if <c>wait==0</c>)
   is returned.
   Description
   This function returns the status of an MD5 SPURS process
   that was initiated using <c>cellMd5SpuProcessData</c>. If <c>wait</c>
   is set to 1, the function will not return until the process
   that is being checked completes. If <c>wait</c> is set to 0,
   the function will return immediately, with return values as
   specified in <c>cellSpursEventFlagTryWait</c>.
   See Also
   <c>cellMd5SpuCreateTask</c>, <c>cellMd5SpuProcessData</c>             */
int cellMd5SpuCheckStatus(CellMd5SpuInstance *md5,
							  uint16_t flag,
							  int wait);

/* Summary
   End a MD5 SPURS task.
   Conditions
   Multi-thread safe.
   Parameters
   md5 :  Pointer to a MD5 SPURS work area
   hash :  Effective address to store the hash value for the data on
           the PPU. This area should be at least 16 bytes in size
           and 16\-byte aligned.
   Returns
   <c>CELL_OK</c>(0) is returned for success.

   If <c>md5==NULL</c> or <c>hash==0</c>, 
   then <c>CELL_ERROR_HASH_NULL_POINTER</c> is returned.

   <c>CELL_ERROR_HASH_INVALID_ALIGN</c> is returned if <c>hash</c> is not aligned
   to a 16-byte boundary.

   If any other error occurs, an error code from <c>cellSpursQueuePush</c>
   or <c>cellSpursEventFlagWait</c> is returned.

   Description
   This function ends the MD5 SPURS as stored in the <c>md5</c>
   parameter. It will wait for all processes in the SPURS queue to
   complete before updating the value in <c>hash</c> and
   terminating the SPURS task.
   Note
   This function is blocking. For maximum efficiency you should
   ensure all previous data is processed by calling <c>cellMd5SpuCheckStatus</c>
   before calling this function.
   See Also
   <c>cellMd5SpuCreateTask</c>, <c>cellMd5SpuProcessData</c>,
   <c>cellMd5SpuCheckStatus</c>                                                  */
int cellMd5SpuEndTask(CellMd5SpuInstance *md5,
					   uint64_t hash);




#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
}
#endif	/* defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus) */

#endif	/* _CELL_LIBMD5_SPU_H_ */
