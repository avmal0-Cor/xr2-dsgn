/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2007 Sony Computer Entertainment Inc.
* All Rights Reserved.
*/

#ifndef _CELL_SYSUTIL_PHOTO_DECODE_H_
#define _CELL_SYSUTIL_PHOTO_DECODE_H_

#include "sysutil_common.h"

#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
extern "C" {
#endif	/* defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus) */


/*J 定数 -------------------------------------------------------------------------------------- */

#define CELL_PHOTO_DECODE_RET_OK				(0)
#define CELL_PHOTO_DECODE_RET_CANCEL			(1)

#define CELL_PHOTO_DECODE_ERROR_BUSY			(0x8002c901)	/*J 0x8002c901 同時起動しようとした アプリケーション開発時までに対処する必要がある */
#define CELL_PHOTO_DECODE_ERROR_INTERNAL		(0x8002c902)	/*J 0x8002c902 内部エラー */
#define CELL_PHOTO_DECODE_ERROR_PARAM			(0x8002c903)	/*J 0x8002c903 パラメータが不正 アプリケーション開発時までに対処する必要がある */
#define CELL_PHOTO_DECODE_ERROR_ACCESS_ERROR	(0x8002c904)	/*J 0x8002c904 HDD アクセスエラー */
#define CELL_PHOTO_DECODE_ERROR_INITIALIZE		(0x8002c905)	/*J 0x8002c905 初期化エラー */
#define CELL_PHOTO_DECODE_ERROR_DECODE			(0x8002c906)	/*J 0x8002c906 デコードエラー */

typedef enum {									/*J 形式バージョン */
	CELL_PHOTO_DECODE_VERSION_CURRENT = 0,		/*J 最新の形式 */
} CellPhotoDecodeVersion;

/*J Photo Decode用のパラメータ */
typedef struct {
	void* dstBuffer;
	uint16_t width;   /*J デコード用バッファの幅 */
	uint16_t height;  /*J デコード用バッファの高さ */
	void* reserved1;
	void* reserved2;
} CellPhotoDecodeSetParam;

/*J 結果用構造体 */
typedef struct {
	uint16_t width;   /*J 実際にデコードした画像の幅 */
	uint16_t height;  /*J 実際にデコードした画像の高さ */
	void* reserved1;
	void* reserved2;
} CellPhotoDecodeReturnParam;

/*J 完了コールバック関数 */
typedef void (*CellPhotoDecodeFinishCallback)( int result, void *userdata);

/*J ユーティリティ関数 ------------------------------------------------------------------------------ */
/*J PhotoDecodeユーティリティの初期化 */
int cellPhotoDecodeInitialize( unsigned int version,					/*J Photo Decodeのバージョン番号 */
							sys_memory_container_t container1,			/*J Utilityが使用するメモリコンテナ */
							sys_memory_container_t container2,			/*J デコード用バッファ */
							CellPhotoDecodeFinishCallback funcFinish,	/*J 終了時に呼ばれるコールバック関数 */
							void *userdata								/*J コールバック関数に渡される */
						);

	/*J PhotoDecodeユーティリティの初期化(container1不要版）*/
int cellPhotoDecodeInitialize2( unsigned int version,					/*J Photo Decodeのバージョン番号 */
							sys_memory_container_t container2,			/*J デコード用バッファ */
							CellPhotoDecodeFinishCallback funcFinish,	/*J 終了時に呼ばれるコールバック関数 */
							void *userdata								/*J コールバック関数に渡される */
						);

/*J Photo Decode */
int cellPhotoDecodeFromFile( const char *srcPath,								/*J デコードするファイルのパス */
							const char *srcFilename,							/*J デコードするファイル名 */
							const CellPhotoDecodeSetParam *set_param,			/*J デコーダに渡すパラメータ */
							CellPhotoDecodeReturnParam    *return_param 		/*J デコード結果として返されるパラメータ */
						);

int cellPhotoDecodeFinalize( CellPhotoDecodeFinishCallback funcFinish,	/*J 終了時に呼ばれるコールバック関数 */
							void *userdata								/*J コールバック関数に渡される */
						);

#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
}
#endif	/* defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus) */

#endif /*_CELL_SYSUTIL_PHOTO_DECODE_H_*/
