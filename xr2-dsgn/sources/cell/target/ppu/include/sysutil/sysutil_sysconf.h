/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2007 Sony Computer Entertainment Inc.
* All Rights Reserved.
*/

#ifndef _CELL_SYSUTIL_SYSCONF_H_
#define _CELL_SYSUTIL_SYSCONF_H_

#include "sysutil_common.h"

#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
extern "C" {
#endif	/* defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus) */

/*J システム設定ダイアログの種類 */
typedef enum
{
	CELL_SYSCONF_TYPE_AUDIO_DEVICE_SETTINGS		= 0x00000001,	/*J 音声機器設定 */
	CELL_SYSCONF_TYPE_VOICE_CHANGER				= 0x00000002,	/*J Voice Changer */
	CELL_SYSCONF_TYPE_BLUETOOTH_DEVICE_UTILITY	= 0x00000003,	/*J Bluetooth機器管理 */
} CellSysconfType;

/*J システム設定ダイアログの終了結果 */
typedef enum
{
	CELL_SYSCONF_RESULT_OK       = 0,   /*J 処理は正常に終了 */
	CELL_SYSCONF_RESULT_CANCELED = 1    /*J ユーザ操作によってキャンセルされた */
} CellSysconfResult;

/*J エラーコード */
#define CELL_SYSCONF_ERROR_PARAM					(0x8002bb01)	/*J 0x8002bb01 パラメータが不正 */

/*J システム設定ダイアログ結果通知コールバック関数 */
typedef void (*CellSysconfCallback)( int result, void *userdata );

int cellSysconfOpen( unsigned int type,			/*J 起動タイプ CellSysconfTypeの値を設定します。 */
					 CellSysconfCallback func,	/*J システム設定結果通知コールバック関数指定 */
					 void *userdata,			/*J コールバック関数で受け取るユーザデータ */
					 void *extparam,			/*J 拡張用パラメータ */
					 sys_memory_container_t id	/*J メモリコンテナID */
);

int cellSysconfAbort( void );

#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
}
#endif	/* defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus) */

#endif /*_CELL_SYSUTIL_SYSCONF_H_*/
