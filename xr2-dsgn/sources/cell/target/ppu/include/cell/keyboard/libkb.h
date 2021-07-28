/*   SCE CONFIDENTIAL                                       */
/*   PlayStation(R)3 Programmer Tool Runtime Library 310.001 */
/*   Copyright (C) 2006 Sony Computer Entertainment Inc.    */
/*   All Rights Reserved.                                   */
#ifndef __CELL_KEYBOARD_LIBKB_H__
#define __CELL_KEYBOARD_LIBKB_H__

#include <sys/types.h>
#include <cell/keyboard/kb_codes.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

int32_t cellKbInit(uint32_t max_connect);
int32_t cellKbEnd(void);
uint16_t cellKbCnvRawCode(uint32_t arrange, uint32_t mkey,
						  uint32_t led, uint16_t rawcode);
int32_t cellKbGetInfo(CellKbInfo *info);
int32_t cellKbRead(uint32_t port_no, CellKbData * data);
int32_t cellKbSetCodeType(uint32_t port_no, uint32_t type);
int32_t cellKbSetReadMode(uint32_t port_no, uint32_t mode);
int32_t cellKbClearBuf(uint32_t port_no);
int32_t cellKbSetLEDStatus(uint32_t port_no, uint8_t led);
int32_t cellKbGetConfiguration(uint32_t port_no, CellKbConfig *config);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif //__CELL_KEYBOARD_LIBKB_H__


