/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2006-2007 Sony Computer Entertainment Inc.
* All Rights Reserved.
*/

//DOM-IGNORE-BEGIN
#ifndef _CELL_LIBSHA256_H
#define _CELL_LIBSHA256_H


#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
extern "C" {
#endif	/* defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus) */


#define CELL_SHA256_BLOCK_SIZE		64
#define CELL_SHA256_DIGEST_SIZE		32

#include "libhash.h"

//DOM-IGNORE-END

typedef struct {
	unsigned int       h[8];
	unsigned int       pad;
	unsigned short     usRemains;
	unsigned short     usComputed;
#if defined(__GNUC__)
	__extension__ unsigned long long ulTotalLen;
#else	/* defined(__GNUC__) */
	unsigned long long ulTotalLen;
#endif	/* defined(__GNUC__) */
	unsigned char      buf[CELL_SHA256_BLOCK_SIZE];
} CellSha256WorkArea;//*** Note to editor, the contents of this structure should be hidden, in the same way that CellSpurs is ***
//Summary:
//Work area for computing an SHA-256 digest
//
//Description:
//This is the datatype used as a work area when calculating a SHA-256 digest, and holds information such as the current digest value, as well as any data
//that is yet to be processed (up to 64 bytes).
//It is not necessary or appropriate for the application to handle the members of this datatype directly.



/* Summary
   Compute SHA-256 digest.
   Conditions
   Multi-thread safe.
   Parameters
   plain :   Pointer to data for which the digest value is to be
             computed.
   len :     Size of data (in bytes) for which the digest value is to
             be computed.
   digest :  Pointer to area where the computed digest value is
             returned. This area needs to be 32 bytes in size.
   Returns
   <table>
   Macro                           Value        \Description
   -----------------------------   -----------  --------------------------
   CELL_OK                         0x00000000   Function returned successfully
   CELL_ERROR_HASH_NULL_POINTER    0x80f00001   <c>plain</c> or <c>digest</c> is a null
                                                 pointer.
   </table>
   Description
   This function computes the SHA-256 digest value for the data as
   specified by <c>plain</c> and <c>len.</c> Use this function
   when all the data required to compute the digest value is
   available in memory.
   Note
   For efficiency, ensure that <c>plain</c> is 16-byte aligned.       */
int  cellSha256Digest(const void *plain, unsigned int len, unsigned char *digest);

/* Summary
   Initialize SHA-256 digest work area.
   Conditions
   Multi-thread safe.
   Parameters
   workarea :  Pointer to a <c>CellSha256WorkArea</c> structure.
   Returns
   <table>
   Macro                           Value        \Description
   -----------------------------   -----------  --------------------------
   CELL_OK                         0x00000000   Function returned successfully
   CELL_ERROR_HASH_NULL_POINTER    0x80f00001   <c>workarea</c> is a null
                                                 pointer.
   </table>
   
   Description
   This function initializes the SHA-256 digest work area which is
   used to compute the SHA-256 digest value. Call this function
   before processing data by calling <c>cellSha256BlockUpdate</c>.          */
int  cellSha256BlockInit(CellSha256WorkArea *workarea);

/* Summary
   Process a chunk of data.
   Conditions
   Multi-thread safe.
   Parameters
   workarea :  Pointer to a <c>CellSha256WorkArea</c> structure.
   plain :     Pointer to data to be processed.
   len :       Number of bytes to process.
   Returns
   <table>
   Macro                            Value         \Description
   ------------------------------   ------------  ------------------------------------------
   CELL_OK                          0x00000000    Function returned successfully
   CELL_ERROR_HASH_NULL_POINTER     0x80f00001    <c>workarea</c> or <c>plain</c> is a null
                                                   pointer.
   CELL_ERROR_HASH_INVALID_STATE    0x80f00007    <c>cellSha256BlockResult</c> has been
                                                   called on <c>workarea</c> so it is in the
                                                   wrong state to process further data.
   </table>


   Description
   This function processes the data passed to it as specified by
   <c>plain</c> and <c>len</c>, and updates <c>workarea</c>
   accordingly. It can be called any number of times once <c>workarea</c>
   has been initialized by calling <c>cellSha256BlockInit</c>,
   which is useful for large chunks of data, for example when
   streaming data.                                                                          */
int  cellSha256BlockUpdate(CellSha256WorkArea *workarea, const void *plain, unsigned int len);

/* Summary
   Get computed SHA-256 digest.
   Conditions
   Multi-thread safe.
   Parameters
   workarea :  Pointer to a <c>CellSha256WorkArea</c> structure.
   digest :    Effective address to store the digest value for the
               data. This area should be at least 32 bytes in size.
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
   computation. Once all data has been processed, the 32 bytes
   at <c>digest</c> are updated with the SHA-256 digest value.
   Until <c>cellSha256BlockInit</c> is called to reinitialize the <c>workarea</c>,
   the SHA-256 digest value is preserved, and the structure is no
   longer valid for <c>cellSha256BlockUpdate</c> operations.                        */
int  cellSha256BlockResult(CellSha256WorkArea *workarea, unsigned char *digest);


#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
}
#endif	/* defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus) */

#endif	/* _CELL_LIBSHA256_H */
