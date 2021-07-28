/*   SCE CONFIDENTIAL                                       */
/*   PlayStation(R)3 Programmer Tool Runtime Library 310.001 */
/*   Copyright (C) 2006 Sony Computer Entertainment Inc.    */
/*   All Rights Reserved.                                   */
#ifndef __CELL_PAD_ERROR_H__
#define __CELL_PAD_ERROR_H__

#include <cell/error.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/*
  CELL_ERROR_FACILITY_HID       0x012
  libpad : 0x8012_1101 - 0x8012_11ff
*/

#define CELL_PAD_MAKE_ERROR(status) \
	CELL_ERROR_MAKE_ERROR(CELL_ERROR_FACILITY_HID, status)

#define CELL_PAD_OK	CELL_OK

/* ERR: CELL_PAD_MAKE_ERROR(0x1101) */
#define CELL_PAD_ERROR_FATAL						CELL_ERROR_CAST(0x80121101)

/* ERR: CELL_PAD_MAKE_ERROR(0x1102) */
#define CELL_PAD_ERROR_INVALID_PARAMETER			CELL_ERROR_CAST(0x80121102)

/* ERR: CELL_PAD_MAKE_ERROR(0x1103) */
#define CELL_PAD_ERROR_ALREADY_INITIALIZED			CELL_ERROR_CAST(0x80121103)

/* ERR: CELL_PAD_MAKE_ERROR(0x1104) */
#define CELL_PAD_ERROR_UNINITIALIZED				CELL_ERROR_CAST(0x80121104)

/* ERR: CELL_PAD_MAKE_ERROR(0x1105) */
#define CELL_PAD_ERROR_RESOURCE_ALLOCATION_FAILED	CELL_ERROR_CAST(0x80121105)

/* ERR: CELL_PAD_MAKE_ERROR(0x1106) */
#define CELL_PAD_ERROR_DATA_READ_FAILED				CELL_ERROR_CAST(0x80121106)

/* ERR: CELL_PAD_MAKE_ERROR(0x1107) */
#define CELL_PAD_ERROR_NO_DEVICE					CELL_ERROR_CAST(0x80121107)

/* ERR: CELL_PAD_MAKE_ERROR(0x1108) */
#define CELL_PAD_ERROR_UNSUPPORTED_GAMEPAD			CELL_ERROR_CAST(0x80121108)

/* ERR: CELL_PAD_MAKE_ERROR(0x0001) */
#define CELL_PAD_ERROR_TOO_MANY_DEVICES				CELL_ERROR_CAST(0x80121109)

/* ERR: CELL_PAD_MAKE_ERROR(0x000A) */
#define CELL_PAD_ERROR_EBUSY						CELL_ERROR_CAST(0x8012110a)

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __CELL_PAD_ERROR_H__ */

