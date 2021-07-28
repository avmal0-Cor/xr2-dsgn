//DOM-IGNORE-BEGIN

/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2006-2007 Sony Computer Entertainment Inc.
* All Rights Reserved.
*/

#ifndef _CELL_LIBMD5_H
#define _CELL_LIBMD5_H


#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
extern "C" {
#endif	/* defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus) */


#define CELL_MD5_BLOCK_SIZE		64
#define CELL_MD5_BLOCK_SIZE4    16

#define CELL_MD5_DIGEST_SIZE    16

#include "libhash.h"

//DOM-IGNORE-END

typedef struct {
	unsigned int       h[4];
	unsigned int       pad;
	unsigned short     usRemains;
	unsigned short     usComputed;
#if defined(__GNUC__)
	__extension__ unsigned long long ulTotalLen;
#else	/* defined(__GNUC__) */
	unsigned long long ulTotalLen;
#endif	/* defined(__GNUC__) */
	unsigned char      buf[CELL_MD5_BLOCK_SIZE];
} CellMd5WorkArea;//*** Note to editor, the contents of this structure should be hidden, in the same way that CellSpurs is ***
//Summary:
//Work area for computing an MD5 digest
//
//Description:
//This is the datatype used as a work area when calculating a MD5 digest, and holds information such as the current digest value, as well as any data
//that is yet to be processed (up to 64 bytes).
//It is not necessary or appropriate for the application to handle the members of this datatype directly.



/* Summary
   Compute MD5 digest.
   Conditions
   Multi-thread safe.
   Parameters
   plain :   Pointer to data for which the digest value is to be
             computed.
   len :     Size of data (in bytes) for which the digest value is to
             be computed.
   digest :  Pointer to area where the computed digest value is
             returned. This area needs to be 16 bytes in size.
   Returns
   <table>
   Macro                           Value        \Description
   -----------------------------   -----------  --------------------------
   CELL_OK                         0x00000000   Function returned successfully
   CELL_ERROR_HASH_NULL_POINTER    0x80f00001   <c>plain</c> or <c>digest</c> is a null
                                                 pointer.
   </table>
   Description
   This function computes the MD5 digest value for the data as
   specified by <c>plain</c> and <c>len.</c> Use this function
   when all the data required to compute the digest value is
   available in memory.
   Note
   For efficiency, ensure that <c>plain</c> is 16-byte aligned.       */
int  cellMd5Digest(const void *plain, unsigned int len, unsigned char *digest);


/* Summary
   Initialize MD5 digest work area.
   Conditions
   Multi-thread safe.
   Parameters
   workarea :  Pointer to a <c>CellMd5WorkArea</c> structure.
   Returns
   <table>
   Macro                           Value        \Description
   -----------------------------   -----------  --------------------------
   CELL_OK                         0x00000000   Function returned successfully
   CELL_ERROR_HASH_NULL_POINTER    0x80f00001   <c>workarea</c> is a null
                                                 pointer.
   </table>
   
   Description
   This function initializes the MD5 digest work area which is
   used to compute the MD5 digest value. Call this function
   before processing data by calling <c>cellMd5BlockUpdate</c>.          */
int  cellMd5BlockInit(CellMd5WorkArea *workarea);


/* Summary
   Process a chunk of data.
   Conditions
   Multi-thread safe.
   Parameters
   workarea :  Pointer to a <c>CellMd5WorkArea</c> structure.
   plain :     Pointer to data to be processed.
   len :       Number of bytes to process.
   Returns
   <table>
   Macro                            Value         \Description
   ------------------------------   ------------  ------------------------------------------
   CELL_OK                          0x00000000    Function returned successfully
   CELL_ERROR_HASH_NULL_POINTER     0x80f00001    <c>workarea</c> or <c>plain</c> is a null
                                                   pointer.
   CELL_ERROR_HASH_INVALID_STATE    0x80f00007    <c>cellMd5BlockResult</c> has been
                                                   called on <c>workarea</c> so it is in the
                                                   wrong state to process further data.
   </table>


   Description
   This function processes the data passed to it as specified by
   <c>plain</c> and <c>len</c>, and updates <c>workarea</c>
   accordingly. It can be called any number of times once <c>workarea</c>
   has been initialized by calling <c>cellMd5BlockInit</c>,
   which is useful for large chunks of data, for example when
   streaming data.                                                                          */
int  cellMd5BlockUpdate(CellMd5WorkArea *workarea, const void *plain, unsigned int len);

/* Summary
   Get computed MD5 digest.
   Conditions
   Multi-thread safe.
   Parameters
   workarea :  Pointer to a <c>CellMd5WorkArea</c> structure.
   digest :    Effective address to store the digest value for the
               data. This area should be at least 16 bytes in size.
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
   computation. Once all data has been processed, the 16 bytes
   at <c>digest</c> are updated with the MD5 digest value.
   Until <c>cellMd5BlockInit</c> is called to reinitialize the <c>workarea</c>,
   the MD5 digest value is preserved, and the structure is no
   longer valid for <c>cellMd5BlockUpdate</c> operations.                        */
int  cellMd5BlockResult(CellMd5WorkArea *workarea, unsigned char *digest);


#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
}
#endif	/* defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus) */

#endif	/* _SCE_LIBMD5_H */
