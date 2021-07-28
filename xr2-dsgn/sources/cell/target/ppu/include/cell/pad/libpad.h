/*   SCE CONFIDENTIAL                                       */
/*   PlayStation(R)3 Programmer Tool Runtime Library 310.001 */
/*   Copyright (C) 2006 Sony Computer Entertainment Inc.    */
/*   All Rights Reserved.                                   */
#ifndef __CELL_PAD_LIBPAD_H__
#define __CELL_PAD_LIBPAD_H__

#include <sys/types.h>
#include <cell/pad/pad_codes.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

int32_t cellPadInit(uint32_t max_connect);
int32_t cellPadEnd(void);
int32_t cellPadClearBuf(uint32_t port_no);
int32_t cellPadGetData(uint32_t port_no, CellPadData *data);
int32_t cellPadGetDataExtra(uint32_t port_no, uint32_t* device_type, CellPadData *data);
int32_t cellPadSetActDirect(uint32_t port_no, CellPadActParam* param);
int32_t cellPadGetInfo2(CellPadInfo2 *info);
int32_t cellPadSetPortSetting(uint32_t port_no, uint32_t port_setting);
/* for LDD Controller */
int32_t cellPadLddRegisterController(void);
int32_t cellPadLddUnregisterController(int32_t handle);
int32_t cellPadLddDataInsert(int32_t handle, CellPadData *data);
int32_t cellPadLddGetPortNo(int32_t handle);
/* for Peripheral class device API */
int32_t cellPadPeriphGetInfo(CellPadPeriphInfo *info);
int32_t cellPadPeriphGetData(uint32_t port_no, CellPadPeriphData *data);
#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif //__CELL_PAD_LIBPAD_H__

