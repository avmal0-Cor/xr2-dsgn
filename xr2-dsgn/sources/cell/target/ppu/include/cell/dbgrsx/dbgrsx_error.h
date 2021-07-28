/*   SCE CONFIDENTIAL                                       */
/*   PlayStation(R)3 Programmer Tool Runtime Library 310.001 */
/*   Copyright (C) 2006 Sony Computer Entertainment Inc.    */
/*   All Rights Reserved.                                   */

#ifndef __CELL_DBG_RSX_ERROR_H__
#define __CELL_DBG_RSX_ERROR_H__

#ifndef __CELL_CELL_ERROR_H__
#include <cell/error.h>	// target/common/include/cell/error.h
#endif

/*
CELL_ERROR_FACILITY_GFX		0x021
libdbgrsx		: 0x8021_0501 - 0x8021_05ff
*/
#define CELL_DBG_RSX_MAKE_ERROR(status) \
	CELL_ERROR_MAKE_ERROR(CELL_ERROR_FACILITY_DBG_RSX, status)

#define CELL_DBG_RSX_ERROR_FAILURE			CELL_ERROR_CAST(0x802105ff)

#define CELL_DBG_RSX_ERROR_INVALID_ENUM			CELL_ERROR_CAST(0x80210501)

#define CELL_DBG_RSX_ERROR_INVALID_VALUE		CELL_ERROR_CAST(0x80210502)

#define CELL_DBG_RSX_ERROR_INVALID_ALIGNMENT	CELL_ERROR_CAST(0x80210503)

#endif	/* __CELL_DBG_RSX_ERROR_H__ */
