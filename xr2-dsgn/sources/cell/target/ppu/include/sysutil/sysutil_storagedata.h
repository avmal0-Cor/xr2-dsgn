/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2007 Sony Computer Entertainment Inc.
* All Rights Reserved.
*/

#ifndef _CELL_SYSUTIL_STORAGEDATA_H_
#define _CELL_SYSUTIL_STORAGEDATA_H_

#include "sysutil_common.h"
#include <sys/memory.h>

#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
extern "C" {
#endif	/* defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus) */

/*J 正常終了コード */
#define CELL_STORAGEDATA_RET_OK					(0)
#define CELL_STORAGEDATA_RET_CANCEL				(1)

#define CELL_STORAGEDATA_ERROR_BUSY				(0x8002be01)	/*J 同時起動しようとした  マスター提出時までに対処する必要がある */
#define CELL_STORAGEDATA_ERROR_INTERNAL			(0x8002be02)	/*J 内部エラー */
#define CELL_STORAGEDATA_ERROR_PARAM			(0x8002be03)	/*J パラメータが不正  マスター提出時までに対処する必要がある */
#define CELL_STORAGEDATA_ERROR_ACCESS_ERROR		(0x8002be04)	/*J HDD アクセスエラー */
#define CELL_STORAGEDATA_ERROR_FAILURE			(0x8002be05)	/*J コピー失敗 */

typedef enum {									/*J 形式バージョン */
	CELL_STORAGEDATA_VERSION_CURRENT = 0,		/*J 最新の形式 */
} CellStorageDataVersion;

typedef enum {									/*J 各種サイズ */
	CELL_STORAGEDATA_HDD_PATH_MAX	= 1055,		/*J HDD パス名最大サイズ（デバイス名を含む） */
												/*J dstHddDir、srcHddFile の最大サイズ */
	CELL_STORAGEDATA_MEDIA_PATH_MAX	= 1024,		/*J ストレージメディア パス名最大サイズ（デバイス名を含まない） */
												/*J srcMediaFile、dstMediaDir の最大サイズ */
	CELL_STORAGEDATA_FILESIZE_MAX	= 1024 * 1024 * 1024,
												/*J fileSizeMax の最大サイズ */
	CELL_STORAGEDATA_TITLE_MAX		= 256		/*J コピーダイアログに表示されるタイトル最大サイズ */
} CellStorageDataParamSize;

#define CELL_STORAGEDATA_IMPORT_FILENAME	"IMPORT.BIN"
#define CELL_STORAGEDATA_EXPORT_FILENAME	"EXPORT.BIN"

/*J 構造体：引数向け -------------------------------------------------------------------------------- */
typedef struct {				/*J 読み込み/書き出し共通指定用 */
	unsigned int fileSizeMax;	/*J ファイルサイズ制限 */
	char *title;				/*J タイトル名 */
	void *reserved;
} CellStorageDataSetParam;

/*J コールバック関数 -------------------------------------------------------------------------------- */
/*J ファイルコピー完了コールバック関数 */
typedef void (*CellStorageDataFinishCallback)( int result, void *userdata);

/*J ユーティリティ関数 ------------------------------------------------------------------------------ */
/*J ファイルコピー：取り込み */
int cellStorageDataImport( unsigned int version,
						   char *srcMediaFile,	/*J コピー元のストレージメディアファイル名 */
						   char *dstHddDir,		/*J コピー先の HDD ディレクトリ名 */
						   CellStorageDataSetParam *param,
						   CellStorageDataFinishCallback funcFinish,
						   sys_memory_container_t container,
						   void *userdata );

/*J ファイルコピー：書き出し */
int cellStorageDataExport( unsigned int version,
						   char *srcHddFile,	/*J コピー元の HDD ファイル名 */
						   char *dstMediaDir,	/*J コピー先のストレージメディアディレクトリ名 */
						   CellStorageDataSetParam *param,
						   CellStorageDataFinishCallback funcFinish,
						   sys_memory_container_t container,
						   void *userdata );

/*J ファイル移動 ： 取り込み */
int cellStorageDataImportMove( unsigned int version,
						   char *srcMediaFile,	/*J 移動元のストレージメディアファイル名 */
						   char *dstHddDir,		/*J 移動先の HDD ディレクトリ名 */
						   CellStorageDataSetParam *param,
						   CellStorageDataFinishCallback funcFinish,
						   sys_memory_container_t container,
						   void *userdata );

#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
}
#endif	/* defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus) */

#endif /*_CELL_SYSUTIL_STORAGEDATA_H_*/
