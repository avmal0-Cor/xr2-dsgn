/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2007 Sony Computer Entertainment Inc.
* All Rights Reserved.
*/

#ifndef _CELL_SYSUTIL_MUSIC_EXPORT_UTIL_H_
#define _CELL_SYSUTIL_MUSIC_EXPORT_UTIL_H_

#include "sysutil_common.h"

#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
extern "C" {
#endif	/* defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus) */


/*J 定数 -------------------------------------------------------------------------------------- */

#define CELL_MUSIC_EXPORT_UTIL_RET_OK				(0)
#define CELL_MUSIC_EXPORT_UTIL_RET_CANCEL			(1)

#define CELL_MUSIC_EXPORT_UTIL_ERROR_BUSY			(0x8002c601)	/*J 0x8002c601 同時起動しようとした アプリケーション開発時までに対処する必要がある */
#define CELL_MUSIC_EXPORT_UTIL_ERROR_INTERNAL		(0x8002c602)	/*J 0x8002c602 内部エラー */
#define CELL_MUSIC_EXPORT_UTIL_ERROR_PARAM			(0x8002c603)	/*J 0x8002c603 パラメータが不正 アプリケーション開発時までに対処する必要がある */
#define CELL_MUSIC_EXPORT_UTIL_ERROR_ACCESS_ERROR	(0x8002c604)	/*J 0x8002c604 HDD アクセスエラー */
#define CELL_MUSIC_EXPORT_UTIL_ERROR_DB_INTERNAL	(0x8002c605)	/*J 0x8002c605 データベース登録失敗 */
#define CELL_MUSIC_EXPORT_UTIL_ERROR_DB_REGIST		(0x8002c606)	/*J 0x8002c606 データベース登録失敗 */
#define CELL_MUSIC_EXPORT_UTIL_ERROR_SET_META		(0x8002c607)	/*J 0x8002c607 メタデータ設定失敗 */
#define CELL_MUSIC_EXPORT_UTIL_ERROR_FLUSH_META		(0x8002c608)	/*J 0x8002c608 メタデータ書き戻し失敗 */
#define CELL_MUSIC_EXPORT_UTIL_ERROR_MOVE			(0x8002c609)	/*J 0x8002c609 ムーブ失敗 */
#define CELL_MUSIC_EXPORT_UTIL_ERROR_INITIALIZE		(0x8002c60a)	/*J 0x8002c60a 初期化エラー */

typedef enum {									/*J 形式バージョン */
	CELL_MUSIC_EXPORT_UTIL_VERSION_CURRENT = 0,		/*J 最新の形式 */
} CellMusicUtilVersion;

typedef enum {											/*J 各種サイズ */
	CELL_MUSIC_EXPORT_UTIL_HDD_PATH_MAX			= 1055,		/*J HDD パス名最大サイズ（デバイス名を含む） */
	CELL_MUSIC_EXPORT_UTIL_MUSIC_TITLE_MAX_LENGTH	= 64,		/*J music titleの最大文字数 */
	CELL_MUSIC_EXPORT_UTIL_GAME_TITLE_MAX_LENGTH	= 64,		/*J game titleの最大文字数 */
	CELL_MUSIC_EXPORT_UTIL_GAME_COMMENT_MAX_SIZE	= 1024,		/*J game commentの最大サイズ（バイト数） */
} CellMusicUtilParamSize;

typedef struct {
	char *title;
	char *game_title;
	char *artist;
	char *genre;
	char *game_comment;		/*J Gameタイトル・NULLなら指定無し */
	char *reserved1;		/*J Gameコメント・NULLなら指定無し */
	void *reserved2;
} CellMusicExportSetParam;

/*J コールバック関数 -------------------------------------------------------------------------------- */

/*J 完了コールバック関数 */
typedef void (*CellMusicExportUtilFinishCallback)( int result, void *userdata);


/*J ユーティリティ関数 ------------------------------------------------------------------------------ */


/*J Music Utilityの初期化処理 */
int cellMusicExportInitialize(		unsigned int version,					/*J Music Utilityのバージョン番号 */
								sys_memory_container_t container,		/*J Utilityが使用するメモリコンテナ */
								CellMusicExportUtilFinishCallback funcFinish,	/*J 終了時に呼ばれるコールバック関数 */
								void *userdata							/*J コールバック関数に渡される */
);

/*J Music Utilityの初期化処理（メモリコンテナ不要版） */
int cellMusicExportInitialize2(		unsigned int version,					/*J Music Utilityのバージョン番号 */
								CellMusicExportUtilFinishCallback funcFinish,	/*J 終了時に呼ばれるコールバック関数 */
								void *userdata							/*J コールバック関数に渡される */
);


/*J MusicのファイルをXMB側にコピーして登録する */
int cellMusicExportFromFile(
								const char *srcHddDir,		/*J ファイルのあるHDD ディレクトリ名 */
								const char *srcHddFile,		/*J ファイル名 */
								CellMusicExportSetParam *param,
								CellMusicExportUtilFinishCallback funcFinish,	/*J 終了時に呼ばれるコールバック関数 */
								void *userdata							/*J コールバック関数に渡される */
);


/*J Music Utilityの終了処理 */
int cellMusicExportFinalize(	CellMusicExportUtilFinishCallback funcFinish,	/*J 終了時に呼ばれるコールバック関数 */
								void *userdata							/*J コールバック関数に渡される */
);

/*J Photo Utilityの登録の進捗を返す */
int cellMusicExportProgress( 	CellMusicExportUtilFinishCallback funcFinish,	/*J 終了時に呼ばれるコールバック関数 */
								void *userdata							/*J コールバック関数に渡される */
);

#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
}
#endif	/* defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus) */

#endif /*_CELL_SYSUTIL_MUSIC_EXPORT_UTIL_H_*/
