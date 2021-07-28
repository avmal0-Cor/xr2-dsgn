/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2008 Sony Computer Entertainment Inc. 
* All Rights Reserved.
*/


/* dbg.h */

#ifndef _CELL_DBG_H
#define _CELL_DBG_H

#include <stdint.h>
#include <stddef.h>
#include <sys/types.h>
#include <cell/error.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/*
	CELL_ERROR_FACILITY_DEBUG			 0x051
		libdbg			: 0x8051_0001 - 0x8051_00ff
*/

#define CELL_DBG_MAKE_ERROR(status) \
	CELL_ERROR_MAKE_ERROR(CELL_ERROR_FACILITY_DEBUG, status)

/* ERR: CELL_DBG_MAKE_ERROR(0x0002) */
#define CELL_DBG_EINVAL						CELL_ERROR_CAST(0x80510002)

/* ERR: CELL_DBG_MAKE_ERROR(0x000C) */
#define CELL_DBG_EABORT						CELL_ERROR_CAST(0x8051000C)

/* ERR: CELL_DBG_MAKE_ERROR(0x000E) */
#define CELL_DBG_ENULL						CELL_ERROR_CAST(0x8051000E)


int cellDbgPpuThreadCountStackFrames(uint32_t *count);
int cellDbgPpuThreadGetStackBackTrace(uint32_t skip, uint32_t number, uintptr_t *backtrace, uint64_t *option);

#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif /* _CELL_DBG_H */
///////////////////////////////////////////////////////////////////

/* end of dbg.h */

