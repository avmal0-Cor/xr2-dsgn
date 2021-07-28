/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2009 Sony Computer Entertainment Inc.
* All Rights Reserved.
*/

#ifndef _CELL_SYSUTIL_COMMON_H_
#define _CELL_SYSUTIL_COMMON_H_

#include <sys/types.h>

#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
extern "C" {
#endif	/* defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus) */


#define CELL_SYSUTIL_ERROR_BASE_INTERNAL		(0x8002b000)
#define CELL_SYSUTIL_ERROR_BASE_COMMON 			(0x8002b100)
#define CELL_SYSUTIL_ERROR_BASE_SYSTEMPARAM		(0x8002b200)
#define CELL_SYSUTIL_ERROR_BASE_MSGDIALOG		(0x8002b300)
#define CELL_SYSUTIL_ERROR_BASE_SAVEDATA		(0x8002b400)
#define CELL_SYSUTIL_ERROR_BASE_OSKDIALOG		(0x8002b500)
#define CELL_SYSUTIL_ERROR_BASE_GAMEDATA		(0x8002b600)
#define CELL_SYSUTIL_ERROR_BASE_AVC				(0x8002b700)
#define CELL_SYSUTIL_ERROR_BASE_NETCTL			(0x8002b800)
#define CELL_SYSUTIL_ERROR_BASE_WEBBROWSER		(0x8002b900)
#define CELL_SYSUTIL_ERROR_BASE_HDDGAME			(0x8002ba00)
#define CELL_SYSUTIL_ERROR_BASE_SYSCONF			(0x8002bb00)
#define CELL_SYSUTIL_ERROR_BASE_SYSCACHE		(0x8002bc00)
#define CELL_SYSUTIL_ERROR_BASE_DISCGAME		(0x8002bd00)
#define CELL_SYSUTIL_ERROR_BASE_STORAGEDATA		(0x8002be00)
#define CELL_SYSUTIL_ERROR_BASE_IMEJP			(0x8002bf00)
#define CELL_SYSUTIL_ERROR_BASE_FILE_SELECT		(0x8002c000)
#define CELL_SYSUTIL_ERROR_BASE_MUSIC			(0x8002c100)
#define CELL_SYSUTIL_ERROR_BASE_PHOTO_EXPORT_UTIL (0x8002c200)
#define CELL_SYSUTIL_ERROR_BASE_USERINFO		(0x8002c300)
#define CELL_SYSUTIL_ERROR_BASE_PRINT			(0x8002c400)
#define CELL_SYSUTIL_ERROR_BASE_REC				(0x8002c500)
#define CELL_SYSUTIL_ERROR_BASE_MUSIC_EXPORT_UTIL (0x8002c600)
#define CELL_SYSUTIL_ERROR_BASE_PHOTO_IMPORT	(0x8002c700)
#define CELL_SYSUTIL_ERROR_BASE_SEARCH			(0x8002c800)
#define CELL_SYSUTIL_ERROR_BASE_PHOTO_DECODE	(0x8002c900)
#define CELL_SYSUTIL_ERROR_BASE_VIDEO_EXPORT_UTIL (0x8002ca00)
#define CELL_SYSUTIL_ERROR_BASE_GAME			(0x8002cb00)
#define CELL_SYSUTIL_ERROR_BASE_GAMEUPDATE		(0x8002cc00)
#define CELL_SYSUTIL_ERROR_BASE_AP              (0x8002cd00)
#define CELL_SYSUTIL_ERROR_BASE_BGDL			(0x8002ce00)
#define CELL_SYSUTIL_ERROR_BASE_VIDEO_UPLOAD_UTIL	(0x8002d000)
#define CELL_SYSUTIL_ERROR_BASE_SCREENSHOT		(0x8002d100)
#define CELL_SYSUTIL_ERROR_BASE_AUTHDIALOG		(0x8002d200)
#define CELL_SYSUTIL_ERROR_BASE_BGMPLAYBACK_EX	(0x8002d300)
#define CELL_SYSUTIL_ERROR_BASE_SYSTEM_CHAT     (0x8002d400)

#define CELL_SYSUTIL_ERROR_TYPE					(0x8002b101)  /* 0x8002b101 */
#define CELL_SYSUTIL_ERROR_VALUE				(0x8002b102)  /* 0x8002b102 */
#define CELL_SYSUTIL_ERROR_SIZE					(0x8002b103)  /* 0x8002b103 */
#define CELL_SYSUTIL_ERROR_NUM					(0x8002b104)  /* 0x8002b104 */
#define CELL_SYSUTIL_ERROR_BUSY					(0x8002b105)  /* 0x8002b105 */
#define CELL_SYSUTIL_ERROR_STATUS				(0x8002b106)  /* 0x8002b106 */
#define CELL_SYSUTIL_ERROR_MEMORY				(0x8002b107)  /* 0x8002b107 */

/*J 共通イベントコールバックの識別子 */
#define CELL_SYSUTIL_REQUEST_EXITGAME			(0x0101)	/*J ゲーム終了通知 */
#define CELL_SYSUTIL_DRAWING_BEGIN				(0x0121)	/*J システムによるオーバーレイ描画(XMB/通知ウインドウ/システムユーティリティ)の開始 */
#define CELL_SYSUTIL_DRAWING_END				(0x0122)	/*J システムによるオーバーレイ描画(XMB/通知ウインドウ/システムユーティリティ)の終了 */
#define CELL_SYSUTIL_SYSTEM_MENU_OPEN			(0x0131)	/*J システムのメニュー(XMB)が開いた */
#define CELL_SYSUTIL_SYSTEM_MENU_CLOSE			(0x0132)	/*J システムのメニュー(XMB)が閉じた */
#define CELL_SYSUTIL_BGMPLAYBACK_PLAY			(0x0141)	/*J BGM再生中 */
#define CELL_SYSUTIL_BGMPLAYBACK_STOP			(0x0142)	/*J BGM停止中 */

#define CELL_SYSUTIL_NP_INVITATION_SELECTED		(0x0151)
#define CELL_SYSUTIL_NP_DATA_MESSAGE_SELECTED (0x0152)

#define CELL_SYSUTIL_SYSCHAT_START						(0x0161)	/*J システムチャット開始中 */
#define CELL_SYSUTIL_SYSCHAT_STOP						(0x0162)	/*J システムチャット停止中 */
#define CELL_SYSUTIL_SYSCHAT_VOICE_STREAMING_RESUMED	(0x0163)	/*J システムチャット音声ストリーミング再開 */
#define CELL_SYSUTIL_SYSCHAT_VOICE_STREAMING_PAUSED		(0x0164)	/*J システムチャット音声ストリーミング一時停止 */

/*J 表示メッセージのロケール */
#define CELL_SYSUTIL_LANG_JAPANESE		(0)		/*J 日本語 */
#define CELL_SYSUTIL_LANG_ENGLISH		(1)		/*J 英語 */
#define CELL_SYSUTIL_LANG_FRENCH		(2)		/*J フランス語 */
#define CELL_SYSUTIL_LANG_SPANISH		(3)		/*J スペイン語 */
#define CELL_SYSUTIL_LANG_GERMAN		(4)		/*J ドイツ語 */
#define CELL_SYSUTIL_LANG_ITALIAN		(5)		/*J イタリア語 */
#define CELL_SYSUTIL_LANG_DUTCH			(6)		/*J オランダ語 */
#define CELL_SYSUTIL_LANG_PORTUGUESE	(7)		/*J ポルトガル語 */
#define CELL_SYSUTIL_LANG_RUSSIAN		(8)		/*J ロシア語 */
#define CELL_SYSUTIL_LANG_KOREAN		(9)		/*J 韓国語 */
#define CELL_SYSUTIL_LANG_CHINESE_T		(10)	/*J 中国語(繁体字) */
#define CELL_SYSUTIL_LANG_CHINESE_S		(11)	/*J 中国語(簡体字) */
#define CELL_SYSUTIL_LANG_FINNISH		(12)	/*J フィンランド語 */
#define CELL_SYSUTIL_LANG_SWEDISH 		(13)	/*J スウェーデン語 */
#define CELL_SYSUTIL_LANG_DANISH 		(14)	/*J デンマーク語 */
#define CELL_SYSUTIL_LANG_NORWEGIAN  	(15)	/*J ノルウェー語 */
#define CELL_SYSUTIL_LANG_POLISH  		(16)	/*J ポーランド語 */

/*J 決定ボタンアサイン */
#define CELL_SYSUTIL_ENTER_BUTTON_ASSIGN_CIRCLE	(0)	/*J o ボタン決定 */
#define CELL_SYSUTIL_ENTER_BUTTON_ASSIGN_CROSS	(1)	/*J x ボタン決定 */ 

/*J 日付フォーマット */
#define CELL_SYSUTIL_DATE_FMT_YYYYMMDD	(0)		/*J 年/月/日 */
#define CELL_SYSUTIL_DATE_FMT_DDMMYYYY	(1)		/*J 日/月/年 */
#define CELL_SYSUTIL_DATE_FMT_MMDDYYYY	(2)		/*J 月/日/年 */

/*J 時刻フォーマット */
#define CELL_SYSUTIL_TIME_FMT_CLOCK12	(0)		/*J 12時間 */
#define CELL_SYSUTIL_TIME_FMT_CLOCK24	(1)		/*J 24時間 */

/*J ゲーム視聴年齢制限 */
#define CELL_SYSUTIL_GAME_PARENTAL_OFF		(0)		/*J 切 */
#define CELL_SYSUTIL_GAME_PARENTAL_LEVEL01	(1)		/*J レベル1 */
#define CELL_SYSUTIL_GAME_PARENTAL_LEVEL02	(2)		/*J レベル2 */
#define CELL_SYSUTIL_GAME_PARENTAL_LEVEL03	(3)		/*J レベル3 */
#define CELL_SYSUTIL_GAME_PARENTAL_LEVEL04	(4)		/*J レベル4 */
#define CELL_SYSUTIL_GAME_PARENTAL_LEVEL05	(5)		/*J レベル5 */
#define CELL_SYSUTIL_GAME_PARENTAL_LEVEL06	(6)		/*J レベル6 */
#define CELL_SYSUTIL_GAME_PARENTAL_LEVEL07	(7)		/*J レベル7 */
#define CELL_SYSUTIL_GAME_PARENTAL_LEVEL08	(8)		/*J レベル8 */
#define CELL_SYSUTIL_GAME_PARENTAL_LEVEL09	(9)		/*J レベル9 */
#define CELL_SYSUTIL_GAME_PARENTAL_LEVEL10	(10)	/*J レベル10 */
#define CELL_SYSUTIL_GAME_PARENTAL_LEVEL11	(11)	/*J レベル11 */

/*J ゲーム視聴年齢制限レベル0の起動制限 */
#define CELL_SYSUTIL_GAME_PARENTAL_LEVEL0_RESTRICT_OFF	(0)	/*J 制限しない(起動する) */
#define CELL_SYSUTIL_GAME_PARENTAL_LEVEL0_RESTRICT_ON	(1)	/*J 制限する(起動しない) */

/*J インターネットブラウザ起動制限 */
#define CELL_SYSUTIL_INTERNET_BROWSER_START_RESTRICT_OFF	(0)	/*J 制限しない(起動する) */
#define CELL_SYSUTIL_INTERNET_BROWSER_START_RESTRICT_ON		(1)	/*J 制限する(起動しない) */

/*J ライセンス地域 */
#define CELL_SYSUTIL_LICENSE_AREA_J		(0) /* SCEJ */
#define CELL_SYSUTIL_LICENSE_AREA_A		(1) /* SCEA */
#define CELL_SYSUTIL_LICENSE_AREA_E		(2) /* SCEE */
#define CELL_SYSUTIL_LICENSE_AREA_H		(3) /* SCEH */
#define CELL_SYSUTIL_LICENSE_AREA_K		(4) /* SCEK */
#define CELL_SYSUTIL_LICENSE_AREA_C		(5) /* SCH */
#define CELL_SYSUTIL_LICENSE_AREA_OTHER	(100) /* Other */

/*J カメラちらつき防止設定 */
#define CELL_SYSUTIL_CAMERA_PLFREQ_DISABLED			(0)	/*J 切 */
#define CELL_SYSUTIL_CAMERA_PLFREQ_50HZ				(1)	/*J 50Hz */
#define CELL_SYSUTIL_CAMERA_PLFREQ_60HZ				(2)	/*J 60Hz */
#define CELL_SYSUTIL_CAMERA_PLFREQ_DEVICE_DEPEND	(4)	/*J カメラの既定値 */

/*J 振動設定 */
#define CELL_SYSUTIL_PAD_RUMBLE_OFF					(0)	/*J 切 */
#define CELL_SYSUTIL_PAD_RUMBLE_ON					(1)	/*J 入 */

/*J キーボードの入力方式(日本語キーボード) */
#define CELL_SYSUTIL_KEYBOARD_ENTRY_METHOD_ROMAJI_INPUT		(0)	/*J ローマ字入力 */
#define CELL_SYSUTIL_KEYBOARD_ENTRY_METHOD_KANA_INPUT		(1)	/*J かな入力 */
/*J キーボードの入力方式(中国語キーボード) */
#define CELL_SYSUTIL_KEYBOARD_ENTRY_METHOD_ZHUYIN_INPUT		(0) /* zhuyin */
#define CELL_SYSUTIL_KEYBOARD_ENTRY_METHOD_PINYIN_INPUT		(1) /* pinyin */
#define CELL_SYSUTIL_KEYBOARD_ENTRY_METHOD_CANGJIE_INPUT	(2)	/* cangjie */


#define CELL_SYSUTIL_USERID_CURRENT		(0)
#define CELL_SYSUTIL_USERID_MAX			(99999999)
typedef unsigned int CellSysutilUserId;

typedef void (*CellSysutilCallback)( uint64_t status, uint64_t param, void * userdata );

int cellSysutilCheckCallback(void);
int cellSysutilRegisterCallback( int slot, CellSysutilCallback func, void * userdata );
int cellSysutilUnregisterCallback( int slot );


#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
}
#endif	/* defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus) */

#endif /*_CELL_SYSUTIL_COMMON_H_*/
