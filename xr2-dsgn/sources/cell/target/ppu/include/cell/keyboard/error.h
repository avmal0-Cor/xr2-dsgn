/*   SCE CONFIDENTIAL                                       */
/*   PlayStation(R)3 Programmer Tool Runtime Library 310.001 */
/*   Copyright (C) 2006 Sony Computer Entertainment Inc.    */
/*   All Rights Reserved.                                   */
#ifndef __CELL_KEYBOARD_ERROR_H__
#define __CELL_KEYBOARD_ERROR_H__

#include <cell/error.h>	

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/*
  CELL_ERROR_FACILITY_HID       0x012
  libkb : 0x8012_1001 - 0x8012_10ff
*/

#define CELL_KB_MAKE_ERROR(status) \
	CELL_ERROR_MAKE_ERROR(CELL_ERROR_FACILITY_HID, status)

#define CELL_KB_OK	CELL_OK

/* ERR: CELL_KB_MAKE_ERROR(0x1001) */
#define CELL_KB_ERROR_FATAL							CELL_ERROR_CAST(0x80121001)

/* ERR: CELL_KB_MAKE_ERROR(0x1002) */
#define CELL_KB_ERROR_INVALID_PARAMETER				CELL_ERROR_CAST(0x80121002)

/* ERR: CELL_KB_MAKE_ERROR(0x1003) */
#define CELL_KB_ERROR_ALREADY_INITIALIZED			CELL_ERROR_CAST(0x80121003)

/* ERR: CELL_KB_MAKE_ERROR(0x1004) */
#define CELL_KB_ERROR_UNINITIALIZED					CELL_ERROR_CAST(0x80121004)

/* ERR: CELL_KB_MAKE_ERROR(0x1005) */
#define CELL_KB_ERROR_RESOURCE_ALLOCATION_FAILED	CELL_ERROR_CAST(0x80121005)

/* ERR: CELL_KB_MAKE_ERROR(0x1006) */
#define CELL_KB_ERROR_DATA_READ_FAILED				CELL_ERROR_CAST(0x80121006)

/* ERR: CELL_KB_MAKE_ERROR(0x1007) */
#define CELL_KB_ERROR_NO_DEVICE						CELL_ERROR_CAST(0x80121007)

/* ERR: CELL_KB_MAKE_ERROR(0x1008) */
#define CELL_KB_ERROR_SYS_SETTING_FAILED			CELL_ERROR_CAST(0x80121008)

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __CELL_KEYBOARD_ERROR_H__ */

