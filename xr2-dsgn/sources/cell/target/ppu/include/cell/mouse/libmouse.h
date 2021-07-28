/*   SCE CONFIDENTIAL                                       */
/*   PlayStation(R)3 Programmer Tool Runtime Library 310.001 */
/*   Copyright (C) 2006 Sony Computer Entertainment Inc.    */
/*   All Rights Reserved.                                   */
#ifndef __CELL_MOUSE_LIBMOUSE_H__
#define __CELL_MOUSE_LIBMOUSE_H__

#include <sys/types.h>
#include <cell/mouse/mouse_codes.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

int32_t cellMouseInit(uint32_t max_connect);
int32_t cellMouseEnd(void);
int32_t cellMouseClearBuf(uint32_t port_no);
int32_t cellMouseGetInfo(CellMouseInfo *info);
int32_t cellMouseGetRawData(uint32_t port_no, CellMouseRawData *data);
int32_t cellMouseGetData(uint32_t port_no, CellMouseData *data);
int32_t cellMouseGetDataList(uint32_t port_no, CellMouseDataList *data);
int32_t cellMouseGetTabletDataList(uint32_t port_no, CellMouseTabletDataList *data);
int32_t cellMouseInfoTabletMode(uint32_t port_no, CellMouseInfoTablet *info);
int32_t cellMouseSetTabletMode(uint32_t port_no, uint32_t mode);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif //__CELL_MOUSE_LIBMOUSE_H__


