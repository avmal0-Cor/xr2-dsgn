/*   SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
*   Copyright (C) 2009 Sony Computer Entertainment Inc.
*   All Rights Reserved.
*/


#ifndef __CELL_FONT_ERROR_H__
#define __CELL_FONT_ERROR_H__

#include <cell/error.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/*
 CELL_ERROR_FACILITY_HID     0x054
  libfont   : 0x8054_0000 - 0x8054_00ff
#define CELL_FONT_MAKE_ERROR(status) \
            CELL_ERROR_MAKE_ERROR(CELL_ERROR_FACILITY_FONT, status)
*/
#include <sdk_version.h>
#if CELL_SDK_VERSION < 0x080000
#ifndef CELL_ERROR_CAST
#define CELL_ERROR_CAST(err) (err)
#endif
#endif

#define CELL_FONT_OK                               CELL_OK
#define CELL_FONT_ERROR_FATAL                      CELL_ERROR_CAST(0x80540001)
#define CELL_FONT_ERROR_INVALID_PARAMETER          CELL_ERROR_CAST(0x80540002)
#define CELL_FONT_ERROR_UNINITIALIZED              CELL_ERROR_CAST(0x80540003)
#define CELL_FONT_ERROR_INITIALIZE_FAILED          CELL_ERROR_CAST(0x80540004)
#define CELL_FONT_ERROR_INVALID_CACHE_BUFFER       CELL_ERROR_CAST(0x80540005)
#define CELL_FONT_ERROR_ALREADY_INITIALIZED        CELL_ERROR_CAST(0x80540006)
#define CELL_FONT_ERROR_ALLOCATION_FAILED          CELL_ERROR_CAST(0x80540007)
#define CELL_FONT_ERROR_NO_SUPPORT_FONTSET         CELL_ERROR_CAST(0x80540008)
#define CELL_FONT_ERROR_OPEN_FAILED                CELL_ERROR_CAST(0x80540009)
#define CELL_FONT_ERROR_READ_FAILED                CELL_ERROR_CAST(0x8054000a)
#define CELL_FONT_ERROR_FONT_OPEN_FAILED           CELL_ERROR_CAST(0x8054000b)
#define CELL_FONT_ERROR_FONT_NOT_FOUND             CELL_ERROR_CAST(0x8054000c)
#define CELL_FONT_ERROR_FONT_OPEN_MAX              CELL_ERROR_CAST(0x8054000d)
#define CELL_FONT_ERROR_FONT_CLOSE_FAILED          CELL_ERROR_CAST(0x8054000e)
#define CELL_FONT_ERROR_ALREADY_OPENED             CELL_ERROR_CAST(0x8054000f)

#define CELL_FONT_ERROR_NO_SUPPORT_FUNCTION        CELL_ERROR_CAST(0x80540010)
#define CELL_FONT_ERROR_NO_SUPPORT_CODE            CELL_ERROR_CAST(0x80540011)
#define CELL_FONT_ERROR_NO_SUPPORT_GLYPH           CELL_ERROR_CAST(0x80540012)

#define CELL_FONT_ERROR_BUFFER_SIZE_NOT_ENOUGH     CELL_ERROR_CAST(0x80540016)

#define CELL_FONT_ERROR_RENDERER_ALREADY_BIND      CELL_ERROR_CAST(0x80540020)
#define CELL_FONT_ERROR_RENDERER_UNBIND            CELL_ERROR_CAST(0x80540021)
#define CELL_FONT_ERROR_RENDERER_INVALID           CELL_ERROR_CAST(0x80540022)
#define CELL_FONT_ERROR_RENDERER_ALLOCATION_FAILED CELL_ERROR_CAST(0x80540023)
#define CELL_FONT_ERROR_ENOUGH_RENDERING_BUFFER    CELL_ERROR_CAST(0x80540024)

#define CELL_FONT_ERROR_NO_SUPPORT_SURFACE         CELL_ERROR_CAST(0x80540040)

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __CELL_FONT_ERROR_H__ */


