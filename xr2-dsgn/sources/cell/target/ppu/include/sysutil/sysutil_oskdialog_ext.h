/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2009 Sony Computer Entertainment Inc.
* All Rights Reserved.
*/

#ifndef _CELL_SYSUTIL_OSKDIALOG_EXT_H_
#define _CELL_SYSUTIL_OSKDIALOG_EXT_H_

#include "sysutil_oskdialog.h"
#include "sysutil_common.h"

#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
extern "C" {
#endif	/* defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus) */


/*J フックの対象とするキーの種類 */
#define CELL_OSKDIALOG_EVENT_HOOK_TYPE_FUNCTION_KEY		0x01	/*J ファンクションキーなどの特殊キーをフックの対象とする */
#define CELL_OSKDIALOG_EVENT_HOOK_TYPE_ASCII_KEY		0x02	/*J "a-z"などのアスキーキーをフックの対象とする */

/*J 表示倍率の限界値 */
#define CELL_OSKDIALOG_SCALE_MAX	1.05f		/*J 表示倍率の最大値 */
#define CELL_OSKDIALOG_SCALE_MIN	0.80f		/*J 表示倍率の最小値 */

/*J 入力文字フィルタコールバックで戻り値として設定 */
typedef enum {
	CELL_OSKDIALOG_NOT_CHANGE = 0,		/*J 変更なし */
	CELL_OSKDIALOG_CHANGE_WORD,			/*J 変更あり */
} CellOskDialogFilterCallbackReturnValue;

/*J キーイベントによるアクション指定の種別 */
typedef enum {
	CELL_OSKDIALOG_CHANGE_NO_EVENT			= 0,	/*J 変更無し */
	CELL_OSKDIALOG_CHANGE_EVENT_CANCEL		= 1,	/*J イベント自体をなかったことにする */
	CELL_OSKDIALOG_CHANGE_WORDS_INPUT		= 3,	/*J 確定文字入力へのイベント読み替え(変換エンジン使用の際は未確定文字化) */
	CELL_OSKDIALOG_CHANGE_WORDS_INSERT		= 4,	/*J 確定文字挿入のイベント読み替え(既存の未確定文字列はキャンセルされる) */
	CELL_OSKDIALOG_CHANGE_WORDS_REPLACE_ALL = 6,	/*J 既存の確定文字を全て削除して確定文字を差し替える */
} CellOskDialogActionValue;

/*J 強制終了の種別 */
typedef enum {
	CELL_OSKDIALOG_CLOSE_CONFIRM = 0,
	CELL_OSKDIALOG_CLOSE_CANCEL,
} CellOskDialogFinishReason;

/*J キーボードメッセージ構造体 */
typedef struct {
	uint32_t led;
	uint32_t mkey;
	uint16_t keycode;
} CellOskDialogKeyMessage;

/*J 入力文字フィルタコールバック定義 */
typedef int (*cellOskDialogConfirmWordFilterCallback)( uint16_t *pConfirmString, int32_t wordLength );

/*J USB入力キーフックコールバック定義 */
typedef bool (*cellOskDialogHardwareKeyboardEventHookCallback)( CellOskDialogKeyMessage *keyMessage, uint32_t *action, void *pActionInfo );

/*J PSボタン検知コールバック定義 */
typedef bool (*cellOskDialogForceFinishCallback)( void );

/*J 関数定義 */
int cellOskDialogExtRegisterConfirmWordFilterCallback( cellOskDialogConfirmWordFilterCallback pCallback );
int cellOskDialogExtRegisterKeyboardEventHookCallback( uint16_t hookEventMode, cellOskDialogHardwareKeyboardEventHookCallback pCallback );
int cellOskDialogExtSetInitialScale( float initialScale );
int cellOskDialogExtUpdateInputText( void );

int cellOskDialogExtInputDeviceLock( void );
int cellOskDialogExtInputDeviceUnlock( void );
int cellOskDialogExtSendFinishMessage( CellOskDialogFinishReason finishReason );

int cellOskDialogExtRegisterForceFinishCallback( cellOskDialogForceFinishCallback pCallback );

int cellOskDialogExtSetBaseColor( float red, float green, float blue, float alpha );


#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
}
#endif	/* defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus) */

#endif /*_CELL_SYSUTIL_OSKDIALOG_EXT_H_*/

