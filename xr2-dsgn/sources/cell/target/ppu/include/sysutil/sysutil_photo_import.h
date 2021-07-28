/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2007 Sony Computer Entertainment Inc.
* All Rights Reserved.
*/

#ifndef _CELL_SYSUTIL_PHOTO_IMPORT_H_
#define _CELL_SYSUTIL_PHOTO_IMPORT_H_

#include "sysutil_common.h"
#include <sys/fs_external.h>

#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
extern "C" {
#endif	/* defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus) */


/*J 定数 -------------------------------------------------------------------------------------- */

#define CELL_PHOTO_IMPORT_RET_OK				(0)
#define CELL_PHOTO_IMPORT_RET_CANCEL			(1)

#define CELL_PHOTO_IMPORT_ERROR_BUSY			(0x8002c701)	/*J 0x8002c701 同時起動しようとした アプリケーション開発時までに対処する必要がある */
#define CELL_PHOTO_IMPORT_ERROR_INTERNAL		(0x8002c702)	/*J 0x8002c702 内部エラー */
#define CELL_PHOTO_IMPORT_ERROR_PARAM			(0x8002c703)	/*J 0x8002c703 パラメータが不正 アプリケーション開発時までに対処する必要がある */
#define CELL_PHOTO_IMPORT_ERROR_ACCESS_ERROR	(0x8002c704)	/*J 0x8002c704 HDD アクセスエラー */
#define CELL_PHOTO_IMPORT_ERROR_COPY			(0x8002c705)	/*J 0x8002c705 コピー失敗 */
#define CELL_PHOTO_IMPORT_ERROR_INITIALIZE		(0x8002c706)	/*J 0x8002c706 初期化エラー */

typedef enum {									/*J 形式バージョン */
	CELL_PHOTO_IMPORT_VERSION_CURRENT = 0,		/*J 最新の形式 */
} CellPhotoImportVersion;

typedef enum {												/*J 各種サイズ */
	CELL_PHOTO_IMPORT_HDD_PATH_MAX				= 1055,		/*J HDD パス名最大サイズ（デバイス名を含む） */
	CELL_PHOTO_IMPORT_PHOTO_TITLE_MAX_LENGTH	= 64,		/*J photo titleの最大文字数 */
	CELL_PHOTO_IMPORT_GAME_TITLE_MAX_SIZE		= 128,		/*J game titleの最大サイズ */
	CELL_PHOTO_IMPORT_GAME_COMMENT_MAX_SIZE		= 1024,		/*J game titleの最大サイズ */
} CellPhotoImportParamSize;

typedef enum {
	CELL_PHOTO_IMPORT_FT_UNKNOWN = 0,
	CELL_PHOTO_IMPORT_FT_JPEG,
	CELL_PHOTO_IMPORT_FT_PNG,
	CELL_PHOTO_IMPORT_FT_GIF,
	CELL_PHOTO_IMPORT_FT_BMP,
	CELL_PHOTO_IMPORT_FT_TIFF,
} CellPhotoImportFormatType;

typedef enum {
	CELL_PHOTO_IMPORT_TEX_ROT_0 = 0,
	CELL_PHOTO_IMPORT_TEX_ROT_90,
	CELL_PHOTO_IMPORT_TEX_ROT_180,
	CELL_PHOTO_IMPORT_TEX_ROT_270,
} CellPhotoImportTexRot;

typedef struct {				/*J 読み込み指定用 */
	unsigned int fileSizeMax;	/*J ファイルサイズ制限 */
	void *reserved1;
	void *reserved2;
} CellPhotoImportSetParam;

typedef struct {
	int width;
	int height;
	CellPhotoImportFormatType format;
	CellPhotoImportTexRot rotate;
} CellPhotoImportFileDataSub;

typedef struct {
	char dstFileName[CELL_FS_MAX_FS_FILE_NAME_LENGTH];				/*J コピーされたファイルのファイル名 */
	char photo_title[CELL_PHOTO_IMPORT_PHOTO_TITLE_MAX_LENGTH*3];	/*J フォトタイトル名 */
	char game_title[CELL_PHOTO_IMPORT_GAME_TITLE_MAX_SIZE];			/*J ゲームタイトル名 */
	char game_comment[CELL_PHOTO_IMPORT_GAME_COMMENT_MAX_SIZE];		/*J ゲームコメント */
	char padding;  //for alignment
	CellPhotoImportFileDataSub *data_sub;
	void *reserved;
} CellPhotoImportFileData;

/*J 完了コールバック関数 */
typedef void (*CellPhotoImportFinishCallback)( int result, CellPhotoImportFileData *filedata, void *userdata);

/*J ユーティリティ関数 ------------------------------------------------------------------------------ */

/*J Photo Import */
int cellPhotoImport(	unsigned int version,						/*J Photo Importのバージョン番号 */
						char *dstHddPath,							/*J コピー先のHDDのパス */
						CellPhotoImportSetParam *param,				/*J Import用パラメータ */
						sys_memory_container_t container,			/*J Utilityが使用するメモリコンテナ */
						CellPhotoImportFinishCallback funcFinish,	/*J 終了時に呼ばれるコールバック関数 */
						void *userdata								/*J コールバック関数に渡される */
					);

/*J Photo Import（コンテナ不要版）*/
int cellPhotoImport2(	unsigned int version,						/*J Photo Importのバージョン番号 */
						char *dstHddPath,							/*J コピー先のHDDのパス */
						CellPhotoImportSetParam *param,				/*J Import用パラメータ */
						CellPhotoImportFinishCallback funcFinish,	/*J 終了時に呼ばれるコールバック関数 */
						void *userdata								/*J コールバック関数に渡される */
					);

#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
}
#endif	/* defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus) */

#endif /*_CELL_SYSUTIL_PHOTO_IMPORT_H_*/
