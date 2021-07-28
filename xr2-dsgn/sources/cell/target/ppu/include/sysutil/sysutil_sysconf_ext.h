/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2008 Sony Computer Entertainment Inc.
* All Rights Reserved.
*/

#ifndef _CELL_SYSUTIL_SYSCONF_EXT_H_
#define _CELL_SYSUTIL_SYSCONF_EXT_H_

#include "sysutil_common.h"

#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
extern "C" {
#endif	/* defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus) */

/*J Bluetooth(R)デバイスタイプ */
typedef enum {
	CELL_SYSCONF_BT_DEVICE_TYPE_AUDIO   = 1,   /*J オーディオデバイス */
	CELL_SYSCONF_BT_DEVICE_TYPE_HID     = 2,   /*J ヒューマンインターフェイスデバイス */ 
} CellSysconfBtDeviceType;

/*J BluetoothR(R)デバイス状態 */
typedef enum {
	CELL_SYSCONF_BT_DEVICE_STATE_UNAVAILABLE = 0,   /*J 未接続 */
	CELL_SYSCONF_BT_DEVICE_STATE_AVAILABLE   = 1,   /*J 接続 */
} CellSysconfBtDeviceState;

/*J Bluetooth(R)機器情報 */
typedef struct CellSysconfBtDeviceInfo {
	uint64_t deviceId;                      /*E device id */
	CellSysconfBtDeviceType deviceType;     /*E device type */
	CellSysconfBtDeviceState state;         /*E device state */
	char name[64];                          /*E device name */
	uint32_t reserved[4];                   /*E reserved */
} CellSysconfBtDeviceInfo;

/*J Bluetooth(R)機器情報リスト */
typedef struct CellSysconfBtDeviceList {
	CellSysconfBtDeviceInfo device[16];
} CellSysconfBtDeviceList;

/*J Bluetooth(R)機器リストを取得する */
int cellSysconfBtGetDeviceList( CellSysconfBtDeviceList * deviceList );

#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
}
#endif	/* defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus) */

#endif /*_CELL_SYSUTIL_SYSCONF_EXT_H_*/
