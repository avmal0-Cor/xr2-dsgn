/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2009 Sony Computer Entertainment Inc.
* All Rights Reserved.
*/

#ifndef _CELL_SYSUTIL_BGDL_H_
#define _CELL_SYSUTIL_BGDL_H_

#include "sysutil_common.h"

#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
extern "C" {
#endif	/* defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus) */


/*J エラー定数 -------------------------------------------------------------------------------------- */

#define CELL_BGDL_UTIL_RET_OK				(0)

#define CELL_BGDL_UTIL_ERROR_BUSY			(CELL_SYSUTIL_ERROR_BASE_BGDL+1)	/*J 0x8002ce01 同時起動しようとした アプリケーション開発時までに対処する必要がある */
#define CELL_BGDL_UTIL_ERROR_INTERNAL		(CELL_SYSUTIL_ERROR_BASE_BGDL+2)	/*J 0x8002ce02 内部エラー */
#define CELL_BGDL_UTIL_ERROR_PARAM			(CELL_SYSUTIL_ERROR_BASE_BGDL+3)	/*J 0x8002ce03 パラメータが不正 アプリケーション開発時までに対処する必要がある */
#define CELL_BGDL_UTIL_ERROR_ACCESS_ERROR	(CELL_SYSUTIL_ERROR_BASE_BGDL+4)	/*J 0x8002ce04 HDD アクセスエラー */
#define CELL_BGDL_UTIL_ERROR_INITIALIZE		(CELL_SYSUTIL_ERROR_BASE_BGDL+5)	/*J 0x8002ce05 初期化エラー */

/*J 状態を表す定数 ---------------------------------------------------------------------------------- */
typedef enum {
	CELL_BGDL_STATE_ERROR = 0,	/*J 何らかのエラーにより中断 */
	CELL_BGDL_STATE_PAUSE,		/*J 中断状態 */
	CELL_BGDL_STATE_READY, 		/*J 待機状態 */
	CELL_BGDL_STATE_RUN,		/*J BGDL中 */
	CELL_BGDL_STATE_COMPLETE,	/*J BGDL完了 */
} CellBGDLState;

/*J 動作モードを表す定数 ---------------------------------------------------------------------------- */
typedef enum {
	CELL_BGDL_MODE_AUTO = 0,	/*J 自動モード。ネットワーク関連APIの使用状況に応じ、自動的にBGDLを停止します。 */
	CELL_BGDL_MODE_ALWAYS_ALLOW,/*J 常時許可モード。ネットワーク関連APIの使用状況によらず、常にBGDLを行います。 */
} CellBGDLMode;

/*J 情報構造体 -------------------------------------------------------------------------------------- */
typedef struct {
	uint64_t received_size;		/*J 受信済みサイズ */
	uint64_t content_size;		/*J コンテンツサイズ */
	CellBGDLState  state;		/*J 状態定数 */
	void* reserved;				/*J リザーブ領域 */
} CellBGDLInfo;


/*J ユーティリティ関数 ------------------------------------------------------------------------------ */

/*J 情報取得 ---------------------------------------------------------------------------------
	指定したcontent_idがBGDLキューに存在する数を返します。存在しない場合はゼロです。
	情報そのものは、"info"に構造体配列として格納されます。
	エラーがあった場合、CELL_BGDL_UTIL_xxx, CELL_SYSUTIL_ERROR_xxxのいずれかを返します。
*/
int cellBGDLGetInfo(
	const char *content_id,		/*J コンテンツID */
	CellBGDLInfo *info,			/*J 取得したBGDL情報を格納するバッファへのポインタ */
	int num	            		/*J 取得するエントリの最大数 */
);

/*J 動作モード設定 ---------------------------------------------------------------------------------
	指定した動作モードに切り替えます。
	エラーがあった場合、CELL_BGLD_UTIL_xxx, CELL_SYSUTIL_ERROR_xxxのいずれかを返します。
*/
int cellBGDLSetMode(
	CellBGDLMode mode			/*J 動作モード */
);

/*J 動作モード取得 ---------------------------------------------------------------------------------
	現在の動作モードを取得します。
	エラーがあった場合、CELL_BGLD_UTIL_xxx, CELL_SYSUTIL_ERROR_xxxのいずれかを返します。
*/
int cellBGDLGetMode(
	CellBGDLMode *mode			/*J 動作モード */
);

#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
}
#endif	/* defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus) */

#endif /*_CELL_SYSUTIL_BGDL_H_*/
