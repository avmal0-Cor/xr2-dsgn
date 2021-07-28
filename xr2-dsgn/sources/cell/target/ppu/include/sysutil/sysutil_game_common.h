/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2009 Sony Computer Entertainment Inc.
* All Rights Reserved.
*/

#ifndef _CELL_SYSUTIL_GAME_COMMON_H_
#define _CELL_SYSUTIL_GAME_COMMON_H_

#include "sysutil_common.h"

#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
extern "C" {
#endif	/* defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus) */

/*J 正常終了コード */
#define CELL_GAME_RET_OK				(0)
#define CELL_GAME_RET_CANCEL			(1)		/*J 操作がキャンセルされた */
#define CELL_GAME_RET_NONE				(2)		/*J コンテンツが存在しない */

/*J エラー終了コード */
#define CELL_GAME_ERROR_NOTFOUND		(0x8002cb04)	/*J 0x8002cb04 指定コンテンツが見つからない */
#define CELL_GAME_ERROR_BROKEN			(0x8002cb05)	/*J 0x8002cb05 指定コンテンツが破損している */
#define CELL_GAME_ERROR_INTERNAL		(0x8002cb06)	/*J 0x8002cb06 内部エラー */
#define CELL_GAME_ERROR_PARAM			(0x8002cb07)	/*J 0x8002cb07 指定されたパラメータが不正 */
#define CELL_GAME_ERROR_NOAPP			(0x8002cb08)	/*J 0x8002cb08 対象アプリケーションが存在しない */
#define CELL_GAME_ERROR_ACCESS_ERROR	(0x8002cb09)	/*J 0x8002cb09 HDDアクセスエラー */

#define CELL_GAME_ERROR_NOSPACE			(0x8002cb20)	/*J 0x8002cb20 HDDの空き容量が不足 */
#define CELL_GAME_ERROR_NOTSUPPORTED	(0x8002cb21)	/*J 0x8002cb21 サポートされていない処理 */
#define CELL_GAME_ERROR_FAILURE			(0x8002cb22)	/*J 0x8002cb22 コンテンツが特定できなかった */
#define CELL_GAME_ERROR_BUSY			(0x8002cb23)	/*J 0x8002cb23 二重でCheck()が呼ばれた */
#define CELL_GAME_ERROR_IN_SHUTDOWN		(0x8002cb24)	/*J 0x8002cb24 システムの終了処理中である */
#define CELL_GAME_ERROR_INVALID_ID		(0x8002cb25)	/*J 0x8002cb25 指定されたIDが不正 */
#define CELL_GAME_ERROR_EXIST			(0x8002cb26)	/*J 0x8002cb26 指定コンテンツが既に存在する */
#define CELL_GAME_ERROR_NOTPATCH		(0x8002cb27)	/*J 0x8002cb27 パッチ起動されていない */
#define CELL_GAME_ERROR_INVALID_THEME_FILE		(0x8002cb28)	/*J 0x8002cb28 テーマファイルのフォーマットが不正 */

#define CELL_GAME_ERROR_BOOTPATH		(0x8002cb50)	/*J 0x8002cb50 起動バイナリのディレクトリパス長エラー (デバッガ起動時のみ) */

typedef enum CellGameParamSize {			/*J 各種サイズ */
	CELL_GAME_PATH_MAX			= 128,		/*J contentInfoPath、usrdirPathの最大サイズ */
	CELL_GAME_DIRNAME_SIZE		= 32,		/*J ディレクトリ名最大サイズ */

	CELL_GAME_TITLE_SIZE 		= 128,		/*J タイトル名最大サイズ */
	CELL_GAME_TITLEID_SIZE		= 10,		/*J titleIDサイズ */
	CELL_GAME_HDDGAMEPATH_SIZE	= 128,		/*J HOME用データディレクトリパス最大サイズ */
	CELL_GAME_THEMEFILENAME_SIZE	= 48	/*J テーマファイル名最大サイズ */
} CellGameParamSize;

typedef enum CellGameGameType {		/*J ゲームに関連するタイプ */
	CELL_GAME_GAMETYPE_SYS = 0,		/*J システムソフトウェア */
	CELL_GAME_GAMETYPE_DISC,		/*J ディスク起動ゲーム */
	CELL_GAME_GAMETYPE_HDD,			/*J Hdd起動ゲーム */
	CELL_GAME_GAMETYPE_GAMEDATA,	/*J ゲームデータ */
	CELL_GAME_GAMETYPE_HOME			/*J Home */
} CellGameGameType;

#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
}
#endif	/* defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus) */

#endif /*_CELL_SYSUTIL_GAME_COMMON_H_*/
