/*   SCE CONFIDENTIAL                                       */
/*   PlayStation(R)3 Programmer Tool Runtime Library 310.001 */
/*   Copyright (C) 2006 Sony Computer Entertainment Inc.    */
/*   All Rights Reserved.                                   */
#ifndef __CELL_MOUSE_MOUSE_CODES_H__
#define __CELL_MOUSE_MOUSE_CODES_H__

#include <sys/types.h>
#define CELL_MAX_MICE 127
#define CELL_MOUSE_MAX_CODES 64

/*E CellMouseInfo : common information of mouse. */
#define CELL_MOUSE_INFO_INTERCEPTED			(1 << 0) /*E input intercepted */

/*E CellMouseInfo : connected status.  */
#define CELL_MOUSE_STATUS_CONNECTED			(1) /*E mouse is connected    */
#define CELL_MOUSE_STATUS_DISCONNECTED		(0) /*E mouse is disconnected */

typedef struct CellMouseInfo{
  uint32_t max_connect;
  uint32_t now_connect;
  uint32_t info;
  uint16_t vendor_id[CELL_MAX_MICE];
  uint16_t product_id[CELL_MAX_MICE];
  uint8_t status[CELL_MAX_MICE];
}CellMouseInfo;

typedef struct CellMouseInfoTablet{
  uint32_t is_supported;
  uint32_t mode;
}CellMouseInfoTablet;

#define CELL_MOUSE_INFO_TABLET_NOT_SUPPORTED 0
#define CELL_MOUSE_INFO_TABLET_SUPPORTED     1
#define CELL_MOUSE_INFO_TABLET_MOUSE_MODE    1
#define CELL_MOUSE_INFO_TABLET_TABLET_MODE   2

typedef struct CellMouseRawData{
  int32_t len;
  uint8_t data[CELL_MOUSE_MAX_CODES];
}CellMouseRawData;

#define CELL_MOUSE_DATA_UPDATE 1
#define CELL_MOUSE_DATA_NON    0

#define CELL_MOUSE_BUTTON_1 (1 << 0) /*E Button 1 */
#define CELL_MOUSE_BUTTON_2 (1 << 1) /*E Button 2 */
#define CELL_MOUSE_BUTTON_3 (1 << 2) /*E Button 3 */
#define CELL_MOUSE_BUTTON_4 (1 << 3) /*E Button 4 */
#define CELL_MOUSE_BUTTON_5 (1 << 4) /*E Button 5 */
#define CELL_MOUSE_BUTTON_6 (1 << 5) /*E Button 6 */
#define CELL_MOUSE_BUTTON_7 (1 << 6) /*E Button 7 */
#define CELL_MOUSE_BUTTON_8 (1 << 7) /*E Button 8 */

typedef struct CellMouseData{
  uint8_t update;
  uint8_t buttons;
  int8_t  x_axis;
  int8_t  y_axis;
  int8_t  wheel;
  int8_t  tilt;	
}CellMouseData;

#define CELL_MOUSE_MAX_DATA_LIST_NUM  8
typedef struct CellMouseDataList{
	uint32_t list_num;
	CellMouseData list[CELL_MOUSE_MAX_DATA_LIST_NUM];
}CellMouseDataList;

typedef struct CellMouseTabletData
{
  int32_t len;
  uint8_t data[CELL_MOUSE_MAX_CODES];
} CellMouseTabletData;

typedef struct CellMouseTabletDataList
{
	uint32_t list_num;
	CellMouseTabletData list[CELL_MOUSE_MAX_DATA_LIST_NUM];
} CellMouseTabletDataList;

#endif /* __CELL_MOUSE_MOUSE_CODES_H__ */

