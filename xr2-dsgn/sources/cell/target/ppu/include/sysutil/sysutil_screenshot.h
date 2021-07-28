/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2008 Sony Computer Entertainment Inc.
* All Rights Reserved.
*/

#ifndef _CELL_SYSUTIL_SCREENSHOT_H_
#define _CELL_SYSUTIL_SCREENSHOT_H_

#include <sys/types.h>
#include "sysutil_common.h"

#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
extern "C"
{
#endif /* defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus) */

#define CELL_SCREENSHOT_OK									(0)	/*J 正常終了 */
#define CELL_SCREENSHOT_ERROR_INTERNAL						(0x8002d101)	/*J 内部エラー */
#define CELL_SCREENSHOT_ERROR_PARAM							(0x8002d102)	/*J パラメータが不正 */
#define CELL_SCREENSHOT_ERROR_DECODE						(0x8002d103)	/*J 画像デコードに失敗 */
#define CELL_SCREENSHOT_ERROR_NOSPACE						(0x8002d104)	/*J HDD 空き容量不足 */
#define CELL_SCREENSHOT_ERROR_UNSUPPORTED_COLOR_FORMAT		(0x8002d105)	/*J 現在のフレームバッファのカラーフォーマットは非サポート */

typedef enum {											/*J 各種サイズ */
	CELL_SCREENSHOT_PHOTO_TITLE_MAX_LENGTH	= 64,		/*J photo titleの最大文字数 */
	CELL_SCREENSHOT_GAME_TITLE_MAX_LENGTH	= 64,		/*J game titleの最大最大文字数 */
	CELL_SCREENSHOT_GAME_COMMENT_MAX_SIZE	= 1024,		/*J game commentの最大サイズ（バイト数） */
} CellScreenShotParamSize;

typedef struct {
	const char *photo_title;	/*J Photoのタイトル */
	const char *game_title;		/*J Gameタイトル・NULLなら指定無し */
	const char *game_comment;	/*J Gameコメント・NULLなら指定無し */
	void *reserved;
} CellScreenShotSetParam;

/*J スクリーンショットを許可する */
int cellScreenShotEnable(void);

/*J スクリーンショットを禁止する */
int cellScreenShotDisable(void);

/*J スクリーンショットのパラメータを設定する */
int cellScreenShotSetParameter(
							const CellScreenShotSetParam *param	/*J スクリーンショットのパラメータ */
);

/*J スクリーンショットファイルへのオーバーレイイメージを設定する */
int cellScreenShotSetOverlayImage(
							const char *srcDir,					/*J ファイルのあるディレクトリ名 */
							const char *srcFile,				/*J ファイル名 */
							int32_t offset_x,					/*J スクリーンショット左上からのX座標オフセット */
							int32_t offset_y					/*J スクリーンショット左上からのY座標オフセット */
);

#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
}
#endif /* defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus) */

#endif /*_CELL_SYSUTIL_SCREENSHOT_H_*/
