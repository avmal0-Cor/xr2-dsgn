/*   SCE CONFIDENTIAL                                       */
/*   PlayStation(R)3 Programmer Tool Runtime Library 310.001 */
/*   Copyright (C) 2006 Sony Computer Entertainment Inc.    */
/*   All Rights Reserved.                                   */
#ifndef __CELL_PAD_LIBPAD_DBG_H__
#define __CELL_PAD_LIBPAD_DBG_H__

#include <sys/types.h>
#include <cell/pad/pad_codes.h>


#define CELL_PAD_LDD_INSERT_DATA_INTO_GAME_MODE_ON  (1)
#define CELL_PAD_LDD_INSERT_DATA_INTO_GAME_MODE_OFF (0)

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

int32_t cellPadDbgLddRegisterController(uint32_t device_capability);
int32_t cellPadDbgLddSetDataInsertMode(int32_t handle, uint8_t mode);
int32_t cellPadDbgPeriphRegisterDevice(uint16_t vid, uint16_t pid, uint32_t pclass_type, uint32_t pclass_profile, void* mapping);
#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif //__CELL_PAD_LIBPAD_DBG_H__

