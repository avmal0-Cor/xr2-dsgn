/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2007 Sony Computer Entertainment Inc.
 * All Rights Reserved.
 */

#ifndef __CELL_DAISY_ERROR_H__
#define __CELL_DAISY_ERROR_H__

#include <cell/error.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/*
  CELL_ERROR_FACILITY_SPU				 0x041
  libdaisy		: 0x8041_0501 - 0x8041_05ff
*/

#define CELL_DAISY_MAKE_ERROR(status) \
	CELL_ERROR_MAKE_ERROR(CELL_ERROR_FACILITY_SPU, status)

#define CELL_ERROR_MINOR_FACILITY_DAISY          0x5
#define CELL_ERROR_MAKE_DAISY_ERROR(id) \
	(CELL_ERROR_MAKE_ERROR(CELL_ERROR_FACILITY_SPU, (CELL_ERROR_MINOR_FACILITY_DAISY << 8) | (id)))
#define CELL_ERROR_IMPORT_SYNC_ERROR(id)\
	((ret == CELL_SYNC_ERROR_INVAL |ret == CELL_SYNC_ERROR_BUSY |ret == CELL_SYNC_ERROR_AGAIN) ?\
	 ((CELL_ERROR_MAKE_ERROR(CELL_ERROR_FACILITY_SPU, (CELL_ERROR_MINOR_FACILITY_DAISY << 8) | (id & 0xff))) | 0x10) : id)

/*  The function to end push/pop is called,
 *  although no pending push/pop exist.
 */
/* ERR: CELL_DAISY_MAKE_ERROR(0x0501) */
#define CELL_DAISY_ERROR_NO_BEGIN CELL_ERROR_CAST(0x80410501)

/* invalid port attach to glue */
/* ERR: CELL_DAISY_MAKE_ERROR(0x0502) */
#define CELL_DAISY_ERROR_INVALID_PORT_ATTACH CELL_ERROR_CAST(0x80410502)

/* called functionality is not implemented */
/* ERR: CELL_DAISY_MAKE_ERROR(0x0503) */
#define CELL_DAISY_ERROR_NOT_IMPLEMENTED CELL_ERROR_CAST(0x80410503)

/* ERR: CELL_DAISY_ERROR_AGAIN(0x0511) */
#define CELL_DAISY_ERROR_AGAIN CELL_ERROR_CAST(0x80410511)

/* ERR: CELL_DAISY_ERROR_INVAL(0x0512) */
#define CELL_DAISY_ERROR_INVAL CELL_ERROR_CAST(0x80410512) 

/* ERR: CELL_DAISY_ERROR_PERM(0x0509) */
#define CELL_DAISY_ERROR_PERM CELL_ERROR_CAST(0x80410509)

/* ERR: CELL_DAISY_ERROR_BUSY(0x051A) */
#define CELL_DAISY_ERROR_BUSY CELL_ERROR_CAST(0x8041051A) 

/* ERR: CELL_DAISY_ERROR_STAT(0x050F) */
#define CELL_DAISY_ERROR_STAT CELL_ERROR_CAST(0x8041050F)

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __CELL_DAISY_ERROR_H__ */

/*
 * Local Variables:
 * mode:C++
 * tab-width:4
 * End:
 * vim:ts=4:sw=4:
 */
