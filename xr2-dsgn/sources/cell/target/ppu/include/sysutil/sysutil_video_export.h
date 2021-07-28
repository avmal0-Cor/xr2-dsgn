/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2007 Sony Computer Entertainment Inc.
* All Rights Reserved.
*/

#ifndef _CELL_SYSUTIL_VIDEO_EXPORT_UTIL_H_
#define _CELL_SYSUTIL_VIDEO_EXPORT_UTIL_H_

#include "sysutil_common.h"

#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
extern "C" {
#endif	/* defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus) */


/*J 定数 -------------------------------------------------------------------------------------- */

#define CELL_VIDEO_EXPORT_UTIL_RET_OK				(0)
#define CELL_VIDEO_EXPORT_UTIL_RET_CANCEL			(1)

#define CELL_VIDEO_EXPORT_UTIL_ERROR_BUSY			(0x8002ca01)	/*J 0x8002ca01 同時起動しようとした アプリケーション開発時までに対処する必要がある */
#define CELL_VIDEO_EXPORT_UTIL_ERROR_INTERNAL		(0x8002ca02)	/*J 0x8002ca02 内部エラー */
#define CELL_VIDEO_EXPORT_UTIL_ERROR_PARAM			(0x8002ca03)	/*J 0x8002ca03 パラメータが不正 アプリケーション開発時までに対処する必要がある */
#define CELL_VIDEO_EXPORT_UTIL_ERROR_ACCESS_ERROR	(0x8002ca04)	/*J 0x8002ca04 HDD アクセスエラー */
#define CELL_VIDEO_EXPORT_UTIL_ERROR_DB_INTERNAL	(0x8002ca05)	/*J 0x8002ca05 データベース登録失敗 */
#define CELL_VIDEO_EXPORT_UTIL_ERROR_DB_REGIST		(0x8002ca06)	/*J 0x8002ca06 データベース登録失敗 */
#define CELL_VIDEO_EXPORT_UTIL_ERROR_SET_META		(0x8002ca07)	/*J 0x8002ca07 メタデータ設定失敗 */
#define CELL_VIDEO_EXPORT_UTIL_ERROR_FLUSH_META		(0x8002ca08)	/*J 0x8002ca08 メタデータ書き戻し失敗 */
#define CELL_VIDEO_EXPORT_UTIL_ERROR_MOVE			(0x8002ca09)	/*J 0x8002ca09 ムーブ失敗 */
#define CELL_VIDEO_EXPORT_UTIL_ERROR_INITIALIZE		(0x8002ca0a)	/*J 0x8002ca0a 初期化エラー */

typedef enum {									/*J 形式バージョン */
	CELL_VIDEO_EXPORT_UTIL_VERSION_CURRENT = 0,		/*J 最新の形式 */
} CellVideoUtilVersion;

typedef enum {											/*J 各種サイズ */
	CELL_VIDEO_EXPORT_UTIL_HDD_PATH_MAX			= 1055,		/*J HDD パス名最大サイズ（デバイス名を含む） */
	CELL_VIDEO_EXPORT_UTIL_VIDEO_TITLE_MAX_LENGTH	= 64,		/*J video titleの最大文字数 */
	CELL_VIDEO_EXPORT_UTIL_GAME_TITLE_MAX_LENGTH	= 64,		/*J game titleの最大サイズ（バイト数） */
	CELL_VIDEO_EXPORT_UTIL_GAME_COMMENT_MAX_SIZE	= 1024,		/*J game commentの最大サイズ（バイト数） */
} CellVideoUtilParamSize;

typedef struct {
	char *title;
	char *game_title;		/*J Gameタイトル・NULLなら指定無し */
	char *game_comment;		/*J Gameコメント・NULLなら指定無し */
	void *reserved1;
	void *reserved2;
} CellVideoExportSetParam;

/*J コールバック関数 -------------------------------------------------------------------------------- */

/*J 完了コールバック関数 */
typedef void (*CellVideoExportUtilFinishCallback)( int result, void *userdata);


/*J ユーティリティ関数 ------------------------------------------------------------------------------ */


/*J Video Utilityの初期化処理 */
int cellVideoExportInitialize(		unsigned int version,					/*J Video Utilityのバージョン番号 */
								sys_memory_container_t container,		/*J Utilityが使用するメモリコンテナ */
								CellVideoExportUtilFinishCallback funcFinish,	/*J 終了時に呼ばれるコールバック関数 */
								void *userdata							/*J コールバック関数に渡される */
);

/*J Video Utilityの初期化処理（メモリコンテナ不要版） */
int cellVideoExportInitialize2(		unsigned int version,					/*J Video Utilityのバージョン番号 */
								CellVideoExportUtilFinishCallback funcFinish,	/*J 終了時に呼ばれるコールバック関数 */
								void *userdata							/*J コールバック関数に渡される */
);

/*J VideoのファイルをXMB側にコピーして登録する */
int cellVideoExportFromFile(
								const char *srcHddDir,		/*J ファイルのあるHDD ディレクトリ名 */
								const char *srcHddFile,		/*J ファイル名 */
								CellVideoExportSetParam *param,
								CellVideoExportUtilFinishCallback funcFinish,	/*J 終了時に呼ばれるコールバック関数 */
								void *userdata							/*J コールバック関数に渡される */
);


/*J Video Utilityの終了処理 */
int cellVideoExportFinalize(	CellVideoExportUtilFinishCallback funcFinish,	/*J 終了時に呼ばれるコールバック関数 */
								void *userdata							/*J コールバック関数に渡される */
);

/*J Video Utilityの登録の進捗を返す */
int cellVideoExportProgress( 	CellVideoExportUtilFinishCallback funcFinish,	/*J 終了時に呼ばれるコールバック関数 */
								void *userdata							/*J コールバック関数に渡される */
);

#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
}
#endif	/* defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus) */

#endif /*_CELL_SYSUTIL_VIDEO_EXPORT_UTIL_H_*/
