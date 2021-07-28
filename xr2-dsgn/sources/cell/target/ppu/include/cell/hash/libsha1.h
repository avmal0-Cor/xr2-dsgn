/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2006-2007 Sony Computer Entertainment Inc.
* All Rights Reserved.
*/


//DOM-IGNORE-BEGIN
#ifndef _CELL_LIBSHA1_PPU_H_
#define _CELL_LIBSHA1_PPU_H_


#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
extern "C" {
#endif	/* defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus) */


#define CELL_SHA1_BLOCK_SIZE	64
#define CELL_SHA1_DIGEST_SIZE	20

#include "libhash.h"

//DOM-IGNORE-END

typedef struct {
	unsigned int       h[5];
	unsigned short     usRemains;
	unsigned short     usComputed;
#if defined(__GNUC__)
	__extension__ unsigned long long ulTotalLen;
#else	/* defined(__GNUC__) */
	unsigned long long ulTotalLen;
#endif	/* defined(__GNUC__) */
	unsigned char      buf[CELL_SHA1_BLOCK_SIZE];
} CellSha1WorkArea;//*** Note to editor, the contents of this structure should be hidden, in the same way that CellSpurs is ***
//Summary:
//Work area for computing an SHA-1 digest
//
//Description:
//This is the datatype used as a work area when calculating a SHA-1 digest, and holds information such as the current digest value, as well as any data
//that is yet to be processed (up to 64 bytes).
//It is not necessary or appropriate for the application to handle the members of this datatype directly.


/* Summary
   Compute SHA-1 digest.
   Conditions
   Multi-thread safe.
   Parameters
   plain :   Pointer to data for which the digest value is to be
             computed.
   len :     Size of data (in bytes) for which the digest value is to
             be computed.
   digest :  Pointer to area where the computed digest value is
             returned. This area needs to be 20 bytes in size.
   Returns
   <table>
   Macro                           Value        \Description
   -----------------------------   -----------  --------------------------
   CELL_OK                         0x00000000   Function returned successfully
   CELL_ERROR_HASH_NULL_POINTER    0x80f00001   <c>plain</c> or <c>digest</c> is a null
                                                 pointer.
   </table>
   Description
   This function computes the SHA-1 digest value for the data as
   specified by <c>plain</c> and <c>len.</c> Use this function
   when all the data required to compute the digest value is
   available in memory.
   Note
   For efficiency, ensure that <c>plain</c> is 16-byte aligned.       */
int  cellSha1Digest(const void *plain, unsigned int len, unsigned char *digest);

/* Summary
   Initialize SHA-1 digest work area.
   Conditions
   Multi-thread safe.
   Parameters
   workarea :  Pointer to a <c>CellSha1WorkArea</c> structure.
   Returns
   <table>
   Macro                           Value        \Description
   -----------------------------   -----------  --------------------------
   CELL_OK                         0x00000000   Function returned successfully
   CELL_ERROR_HASH_NULL_POINTER    0x80f00001   <c>workarea</c> is a null
                                                 pointer.
   </table>
   
   Description
   This function initializes the SHA-1 digest work area which is
   used to compute the SHA-1 digest value. Call this function
   before processing data by calling <c>cellSha1BlockUpdate</c>.          */
int  cellSha1BlockInit(CellSha1WorkArea *workarea);

/* Summary
   Process a chunk of data.
   Conditions
   Multi-thread safe.
   Parameters
   workarea :  Pointer to a <c>CellSha1WorkArea</c> structure.
   plain :     Pointer to data to be processed.
   len :       Number of bytes to process.
   Returns
   <table>
   Macro                            Value         \Description
   ------------------------------   ------------  ------------------------------------------
   CELL_OK                          0x00000000    Function returned successfully
   CELL_ERROR_HASH_NULL_POINTER     0x80f00001    <c>workarea</c> or <c>plain</c> is a null
                                                   pointer.
   CELL_ERROR_HASH_INVALID_STATE    0x80f00007    <c>cellSha1BlockResult</c> has been
                                                   called on <c>workarea</c> so it is in the
                                                   wrong state to process further data.
   </table>


   Description
   This function processes the data passed to it as specified by
   <c>plain</c> and <c>len</c>, and updates <c>workarea</c>
   accordingly. It can be called any number of times once <c>workarea</c>
   has been initialized by calling <c>cellSha1BlockInit</c>,
   which is useful for large chunks of data, for example when
   streaming data.                                                                          */
int  cellSha1BlockUpdate(CellSha1WorkArea *workarea, const void *plain, unsigned int len);


/* Summary
   Get computed SHA-1 digest.
   Conditions
   Multi-thread safe.
   Parameters
   workarea :  Pointer to a <c>CellSha1WorkArea</c> structure.
   digest :    Effective address to store the digest value for the
               data. This area should be at least 20 bytes in size.
   Returns
   <table>
   Macro                            Value         \Description
   ------------------------------   ------------  ------------------------------------------  
   CELL_OK                          0x00000000    Function returned successfully
   CELL_ERROR_HASH_NULL_POINTER     0x80f00001    <c>workarea</c> or <c>digest</c>
                                                   is a null pointer.
   </table>											 
   Description
   This function processes any remaining data within the work
   area which may contain up to 64 bytes remaining from previous
   computation. Once all data has been processed, the 20 bytes
   at <c>digest</c> are updated with the SHA-1 digest value.
   Until <c>cellSha1BlockInit</c> is called to reinitialize the <c>workarea</c>,
   the SHA-1 digest value is preserved, and the structure is no
   longer valid for <c>cellSha1BlockUpdate</c> operations.                        */
int  cellSha1BlockResult(CellSha1WorkArea *workarea, unsigned char *digest);


#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
}
#endif	/* defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus) */

#endif	/* _CELL_LIBSHA1_PPU_H_ */
