/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2006 Sony Computer Entertainment Inc.
* All Rights Reserved.
*/

#ifndef __CELL_SHEAP_RETURN_CODE_H__
#define __CELL_SHEAP_RETURN_CODE_H__ 1
#include <cellstatus.h>
#include <cell/error.h>


#define CELL_ERROR_MINOR_FACILITY_SHEAP          0x3
#define CELL_ERROR_MAKE_SHEAP_ERROR(id) \
	(CELL_ERROR_MAKE_ERROR(CELL_ERROR_FACILITY_SPU, (CELL_ERROR_MINOR_FACILITY_SHEAP << 8) | (id)))




/** ------------------------------------------------------------------------- */




/** The resource is temporarily unavailable. 
 *
 * e.g. The number of threads in the system is exceeding the limit.
 */
#define CELL_SHEAP_ERROR_AGAIN  CELL_ERROR_CAST(0x80410301)

/** An invalid argument value is specified.
 *
 * e.g. An out-of-range argument or an invalid flag.
 */
#define CELL_SHEAP_ERROR_INVAL  CELL_ERROR_CAST(0x80410302)

/** The feature is not yet implemented. */ 
#define CELL_SHEAP_ERROR_NOSYS  CELL_ERROR_CAST(0x80410303)

/** Memory allocation failure */
#define CELL_SHEAP_ERROR_NOMEM  CELL_ERROR_CAST(0x80410304)

/** The resource (process, thread, sheaphronous object, etc) with the
 * specified identifier does not exist.
 */
#define CELL_SHEAP_ERROR_SRCH       CELL_ERROR_CAST(0x80410305)

/** The file does not exist. */
#define CELL_SHEAP_ERROR_NOENT  CELL_ERROR_CAST(0x80410306)

/**  The file is not a valid ELF file. (The file is in unrecognized format.) */
#define CELL_SHEAP_ERROR_NOEXEC  CELL_ERROR_CAST(0x80410307)

/** Resource deadlock is avoided. */
#define CELL_SHEAP_ERROR_DEADLK     CELL_ERROR_CAST(0x80410308)

/** The operation is not permitted. */
#define CELL_SHEAP_ERROR_PERM       CELL_ERROR_CAST(0x80410309)

/** The device or resource is busy */
#define CELL_SHEAP_ERROR_BUSY  CELL_ERROR_CAST(0x8041030A)

/** The operation is aborted */
#define CELL_SHEAP_ERROR_ABORT  CELL_ERROR_CAST(0x8041030C)

/** Invalid memory access */
#define CELL_SHEAP_ERROR_FAULT  CELL_ERROR_CAST(0x8041030D)

/** Failed in awaiting a child, or no such child exists. */
#define CELL_SHEAP_ERROR_CHILD  CELL_ERROR_CAST(0x8041030E)

/** State of the target thread is invalid. */
#define CELL_SHEAP_ERROR_STAT       CELL_ERROR_CAST(0x8041030F)

/** Alignment is invalid. */
#define CELL_SHEAP_ERROR_ALIGN      CELL_ERROR_CAST(0x80410310)

/** Shortage of the kernel resources */
#define CELL_SHEAP_ERROR_KRESOURCE  CELL_ERROR_CAST(0x80410311)

#define CELL_SHEAP_ERROR_SHORTAGE   CELL_ERROR_CAST(0x80410312)
#define CELL_SHEAP_ERROR_UNKNOWNKEY CELL_ERROR_CAST(0x80410313)




#endif /* __CELL_SHEAP_RETURN_CODE_H__ */
