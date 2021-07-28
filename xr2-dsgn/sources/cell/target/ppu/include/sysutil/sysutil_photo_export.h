/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2007 Sony Computer Entertainment Inc.
* All Rights Reserved.
*/

#ifndef _CELL_SYSUTIL_PHOTO_EXPORT_UTIL_H_
#define _CELL_SYSUTIL_PHOTO_EXPORT_UTIL_H_

#include "sysutil_common.h"

#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
extern "C" {
#endif	/* defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus) */


/*J 定数 -------------------------------------------------------------------------------------- */

#define CELL_PHOTO_EXPORT_UTIL_RET_OK				(0)
#define CELL_PHOTO_EXPORT_UTIL_RET_CANCEL			(1)

#define CELL_PHOTO_EXPORT_UTIL_ERROR_BUSY			(0x8002c201)	/*J 0x8002c201 同時起動しようとした アプリケーション開発時までに対処する必要がある */
#define CELL_PHOTO_EXPORT_UTIL_ERROR_INTERNAL		(0x8002c202)	/*J 0x8002c202 内部エラー */
#define CELL_PHOTO_EXPORT_UTIL_ERROR_PARAM			(0x8002c203)	/*J 0x8002c203 パラメータが不正 アプリケーション開発時までに対処する必要がある */
#define CELL_PHOTO_EXPORT_UTIL_ERROR_ACCESS_ERROR	(0x8002c204)	/*J 0x8002c204 HDD アクセスエラー */
#define CELL_PHOTO_EXPORT_UTIL_ERROR_DB_INTERNAL	(0x8002c205)	/*J 0x8002c205 データベース登録失敗 */
#define CELL_PHOTO_EXPORT_UTIL_ERROR_DB_REGIST		(0x8002c206)	/*J 0x8002c206 データベース登録失敗 */
#define CELL_PHOTO_EXPORT_UTIL_ERROR_SET_META		(0x8002c207)	/*J 0x8002c207 メタデータ設定失敗 */
#define CELL_PHOTO_EXPORT_UTIL_ERROR_FLUSH_META		(0x8002c208)	/*J 0x8002c208 メタデータ書き戻し失敗 */
#define CELL_PHOTO_EXPORT_UTIL_ERROR_MOVE			(0x8002c209)	/*J 0x8002c209 ムーブ失敗 */
#define CELL_PHOTO_EXPORT_UTIL_ERROR_INITIALIZE		(0x8002c20a)	/*J 0x8002c20a 初期化エラー */

typedef enum {									/*J 形式バージョン */
	CELL_PHOTO_EXPORT_UTIL_VERSION_CURRENT = 0,		/*J 最新の形式 */
} CellPhotoExportUtilVersion;

typedef enum {											/*J 各種サイズ */
	CELL_PHOTO_EXPORT_UTIL_HDD_PATH_MAX			= 1055,		/*J HDD パス名最大サイズ（デバイス名を含む） */
	CELL_PHOTO_EXPORT_UTIL_PHOTO_TITLE_MAX_LENGTH	= 64,		/*J photo titleの最大文字数 */
	CELL_PHOTO_EXPORT_UTIL_GAME_TITLE_MAX_LENGTH	= 64,		/*J game titleの最大最大文字数 */
	CELL_PHOTO_EXPORT_UTIL_GAME_COMMENT_MAX_SIZE	= 1024,		/*J game commentの最大サイズ（バイト数） */
} CellPhotoExportUtilParamSize;

typedef struct {
	char *photo_title;		/*J Photoのタイトル */
	char *game_title;		/*J Gameタイトル・NULLなら指定無し */
	char *game_comment;		/*J Gameコメント・NULLなら指定無し */
	void *reserved;
} CellPhotoExportSetParam;

/*J コールバック関数 -------------------------------------------------------------------------------- */

/*J 完了コールバック関数 */
typedef void (*CellPhotoExportUtilFinishCallback)( int result, void *userdata);


/*J ユーティリティ関数 ------------------------------------------------------------------------------ */


/*J Photo Utilityの初期化処理 */
int cellPhotoExportInitialize(
								unsigned int version,					/*J Photo Utilityのバージョン番号 */
								sys_memory_container_t container,		/*J Utilityが使用するメモリコンテナ */
								CellPhotoExportUtilFinishCallback funcFinish,	/*J 終了時に呼ばれるコールバック関数 */
								void *userdata							/*J コールバック関数に渡される */
);

/*J Photo Utilityの初期化処理（メモリコンテナ不要版） */
int cellPhotoExportInitialize2(
								unsigned int version,					/*J Photo Utilityのバージョン番号 */
								CellPhotoExportUtilFinishCallback funcFinish,	/*J 終了時に呼ばれるコールバック関数 */
								void *userdata							/*J コールバック関数に渡される */
);


/*J PhotoのファイルをXMB側にコピーして登録する */
int cellPhotoExportFromFile(
								const char *srcHddDir,					/*J ファイルのあるHDD ディレクトリ名 */
								const char *srcHddFile,					/*J ファイル名 */
								CellPhotoExportSetParam *param,
								CellPhotoExportUtilFinishCallback funcFinish,	/*J 終了時に呼ばれるコールバック関数 */
								void *userdata							/*J コールバック関数に渡される */
);


/*J Photo Utilityの終了処理 */
int cellPhotoExportFinalize(	CellPhotoExportUtilFinishCallback funcFinish,	/*J 終了時に呼ばれるコールバック関数 */
								void *userdata							/*J コールバック関数に渡される */
);

/*J Photo Utilityの登録の進捗を返す */
int cellPhotoExportProgress( 	CellPhotoExportUtilFinishCallback funcFinish,	/*J 終了時に呼ばれるコールバック関数 */
								void *userdata							/*J コールバック関数に渡される */
);


#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
}
#endif	/* defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus) */

#endif /*_CELL_SYSUTIL_PHOTO_EXPORT_UTIL_H_*/
