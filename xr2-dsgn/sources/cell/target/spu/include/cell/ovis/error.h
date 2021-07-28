/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2006 Sony Computer Entertainment Inc.
* All Rights Reserved.
*/

#ifndef __CELL_OVIS_RETURN_CODE_H__
#define __CELL_OVIS_RETURN_CODE_H__

#include <cellstatus.h>
#include <cell/error.h>


#define CELL_ERROR_MINOR_FACILITY_OVIS          0x4
#define CELL_ERROR_MAKE_OVIS_ERROR(id) \
	(CELL_ERROR_MAKE_ERROR(CELL_ERROR_FACILITY_SPU, (CELL_ERROR_MINOR_FACILITY_OVIS << 8) | (id)))


/** An invalid argument value is specified.
 *
 * e.g. An out-of-range argument or an invalid flag.
 */
#define CELL_OVIS_ERROR_INVAL  CELL_ERROR_CAST(0x80410402)

#define CELL_OVIS_ERROR_ABORT  CELL_ERROR_CAST(0x8041040C)


/** Alignment is invalid. */
#define CELL_OVIS_ERROR_ALIGN   CELL_ERROR_CAST(0x80410410)

#endif /* __CELL_OVIS_RETURN_CODE_H__ */
