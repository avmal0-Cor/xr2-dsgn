/*   SCE CONFIDENTIAL                                       */
/*   PlayStation(R)3 Programmer Tool Runtime Library 310.001 */
/*   Copyright (C) 2006 Sony Computer Entertainment Inc.    */
/*   All Rights Reserved.                                   */

#ifndef __CELL_GCM_ERROR_H__
#define __CELL_GCM_ERROR_H__

#ifndef __CELL_CELL_ERROR_H__
#include <cell/error.h>	// target/common/include/cell/error.h
#endif

/*
CELL_ERROR_FACILITY_GFX		0x021
libgcm			: 0x8021_0001 - 0x8021_00ff
*/
#define CELL_GCM_MAKE_ERROR(status) \
	CELL_ERROR_MAKE_ERROR(CELL_ERROR_FACILITY_GFX, status)

/* ERR: CELL_GCM_MAKE_ERROR(0x00ff) */
#define CELL_GCM_ERROR_FAILURE				CELL_ERROR_CAST(0x802100ff)

/* ERR: CELL_GCM_MAKE_ERROR(0x0001) */
#define CELL_GCM_ERROR_NO_IO_PAGE_TABLE		CELL_ERROR_CAST(0x80210001)

/* ERR: CELL_GCM_MAKE_ERROR(0x0002) */
#define CELL_GCM_ERROR_INVALID_ENUM			CELL_ERROR_CAST(0x80210002)

/* ERR: CELL_GCM_MAKE_ERROR(0x0003) */
#define CELL_GCM_ERROR_INVALID_VALUE		CELL_ERROR_CAST(0x80210003)

/* ERR: CELL_GCM_MAKE_ERROR(0x0004) */
#define CELL_GCM_ERROR_INVALID_ALIGNMENT	CELL_ERROR_CAST(0x80210004)

/* ERR: CELL_GCM_MAKE_ERROR(0x0005) */
#define CELL_GCM_ERROR_ADDRESS_OVERWRAP		CELL_ERROR_CAST(0x80210005)

#endif	/* __CELL_GCM_ERROR_H__ */
