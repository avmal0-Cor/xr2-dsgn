/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2009 Sony Computer Entertainment Inc.
* All Rights Reserved.
*/

#ifndef _CELL_SYSUTIL_MUSIC_DECODE_H_
#define _CELL_SYSUTIL_MUSIC_DECODE_H_

#include <cell/spurs/types.h>
#include <cell/spurs/system_workload.h>

#include "sysutil_common.h"
#include "sysutil_search_types.h"
#include "sysutil_music_playback_common.h"

#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
extern "C" {
#endif	/* defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus) */

// Music Utility（必須＋デコード機能用メモリコンテナサイズ）
#define CELL_MUSIC_DECODE_MEMORY_CONTAINER_SIZE		(12 * 1024 * 1024)

#define CELL_SYSUTIL_MUSIC_DECODE_INITIALIZING_FINISHED  1
#define CELL_SYSUTIL_MUSIC_DECODE_SHUTDOWN_FINISHED      4 // 3(SDK103) -> 4(SDK110)
#define CELL_SYSUTIL_MUSIC_DECODE_LOADING_FINISHED       5
#define CELL_SYSUTIL_MUSIC_DECODE_UNLOADING_FINISHED     7
#define CELL_SYSUTIL_MUSIC_DECODE_RELEASED               9
#define CELL_SYSUTIL_MUSIC_DECODE_GRABBED               11

#define CELL_MUSIC_DECODE_EVENT_STATUS_NOTIFICATION				(0)	/*J	デコード中に発生したステータスの通知	*/
#define CELL_MUSIC_DECODE_EVENT_INITIALIZE_RESULT				(1)	/*J	cellMusicDecodeInitialize()の実行結果	*/
#define CELL_MUSIC_DECODE_EVENT_FINALIZE_RESULT					(2)	/*J	cellMusicDecodeFinalize()の実行結果	*/
#define CELL_MUSIC_DECODE_EVENT_SELECT_CONTENTS_RESULT			(3)	/*J	cellMusicDecodeSelectContents()の実行結果	*/
#define CELL_MUSIC_DECODE_EVENT_SET_DECODE_COMMAND_RESULT		(4)	/*J cellMusicDecodeSetDecodeCommand() の実行結果 */
#define CELL_MUSIC_DECODE_EVENT_SET_SELECTION_CONTEXT_RESULT	(5)	/*J	cellMusicDecodeSetSelectionContext()の実行結果	*/
#define CELL_MUSIC_DECODE_EVENT_UI_NOTIFICATION					(6)	/*J	UIに関する通知	*/
#define CELL_MUSIC_DECODE_EVENT_NEXT_CONTENTS_READY_RESULT		(7)	/*J 次のコンテンツのデコード開始を許可した際の通知 */

#define CELL_MUSIC_DECODE_OK									0				/*J	正常終了	*/
#define CELL_MUSIC_DECODE_CANCELED								1				/*J	操作がキャンセルされた	*/
#define CELL_MUSIC_DECODE_DECODE_FINISHED						(0x8002c101)	/*J	(CELL_SYSUTIL_ERROR_BASE_MUSIC | 0x01)	最終コンテンツの終端まで到達してデコードが停止した	*/
#define CELL_MUSIC_DECODE_ERROR_PARAM							(0x8002c102)	/*J	(CELL_SYSUTIL_ERROR_BASE_MUSIC | 0x02)	パラメータが不正 */
#define CELL_MUSIC_DECODE_ERROR_BUSY							(0x8002c103)	/*J	(CELL_SYSUTIL_ERROR_BASE_MUSIC | 0x03)	必要な処理が終了していないため要求された処理が実行できなかった	*/
#define CELL_MUSIC_DECODE_ERROR_NO_ACTIVE_CONTENT				(0x8002c104)	/*J	(CELL_SYSUTIL_ERROR_BASE_MUSIC | 0x04)	開いているコンテンツがない	*/
#define CELL_MUSIC_DECODE_ERROR_NO_MATCH_FOUND					(0x8002c105)	/*J	(CELL_SYSUTIL_ERROR_BASE_MUSIC | 0x05)	渡されたコンテキストに相当するコンテンツが見つからなかった	*/
#define CELL_MUSIC_DECODE_ERROR_INVALID_CONTEXT					(0x8002c106)	/*J	(CELL_SYSUTIL_ERROR_BASE_MUSIC | 0x06)	コンテキストが不正	*/
#define CELL_MUSIC_DECODE_ERROR_DECODE_FAILURE					(0x8002c107)	/*J	(CELL_SYSUTIL_ERROR_BASE_MUSIC | 0x07)	デコードにエラーが発生して停止した	*/
#define CELL_MUSIC_DECODE_ERROR_NO_MORE_CONTENT					(0x8002c108)	/*J	(CELL_SYSUTIL_ERROR_BASE_MUSIC | 0x08)	次/前のコンテンツが存在しない	*/
#define CELL_MUSIC_DECODE_DIALOG_OPEN							(0x8002c109)	/*J	(CELL_SYSUTIL_ERROR_BASE_MUSIC | 0x08)	メッセージダイアログが表示される	*/
#define CELL_MUSIC_DECODE_DIALOG_CLOSE							(0x8002c10A)	/*J	(CELL_SYSUTIL_ERROR_BASE_MUSIC | 0x09)	メッセージダイアログが閉じられた	*/
#define CELL_MUSIC_DECODE_ERROR_NO_LPCM_DATA					(0x8002c10B)	/*J (CELL_SYSUTIL_ERROR_BASE_MUSIC | 0x0A)	取得できるデコードデータ存在しなかった	*/
#define CELL_MUSIC_DECODE_NEXT_CONTENTS_READY					(0x8002c10C)	/*J (CELL_SYSUTIL_ERROR_BASE_MUSIC | 0x0B)	次のコンテンツのデコードの準備が完了した	*/
#define CELL_MUSIC_DECODE_ERROR_GENERIC							(0x8002c1FF)	/*J	(CELL_SYSUTIL_ERROR_BASE_MUSIC | 0xFF)	未知のエラーが発生	*/

/*J デコード機能 動作モード */
#define CELL_MUSIC_DECODE_MODE_NORMAL							(0)	/*J ノーマルモード */

/* J デコード制御コマンド */
#define CELL_MUSIC_DECODE_CMD_STOP								(0) /* J デコード停止 */
#define CELL_MUSIC_DECODE_CMD_START								(1) /* J デコード開始 */
#define CELL_MUSIC_DECODE_CMD_NEXT								(2) /* J 次トラック */
#define CELL_MUSIC_DECODE_CMD_PREV								(3) /* J 前トラック */

/* J デコード制御ステータス */
#define CELL_MUSIC_DECODE_STATUS_DORMANT						(0) /* J デコード停止 */
#define CELL_MUSIC_DECODE_STATUS_DECODING						(1) /* J デコード中 */

/* J デコードデータポジション */
#define CELL_MUSIC_DECODE_POSITION_NONE							(0) /* J 位置情報なし */
#define CELL_MUSIC_DECODE_POSITION_START						(1) /* J リードしたデコードデータの先頭がコンテンツの始端 */
#define CELL_MUSIC_DECODE_POSITION_MID							(2) /* J リードしたデコードデータがコンテンツの途中 */
#define CELL_MUSIC_DECODE_POSITION_END							(3) /* J リードしたデコードデータの最後がコンテンツの終端 */
#define CELL_MUSIC_DECODE_POSITION_END_LIST_END					(4) /* J リードしたデコードデータの最後がコンテンツの終端(コンテンツはリスト終端曲) */

/*J コールバック関数 */
typedef void (*CellMusicDecodeCallback)(uint32_t event, void *param, void *userData);
#define MUSIC_DECODE_SYSTEM_CALLBACK(music_decode_callback, event, param, userData) static void music_decode_callback(uint32_t event, void *param, void *userData)

/*J デコード機能 初期化 */ 
int cellMusicDecodeInitialize( int mode, sys_memory_container_t container, int spuPriority, CellMusicDecodeCallback func, void *userData); 
/*J デコード機能 初期化(システムワークロード) */ 
int cellMusicDecodeInitializeSystemWorkload( int mode, sys_memory_container_t container, CellMusicDecodeCallback func, void *userData, int spuUsageRate,
											CellSpurs *spurs, const uint8_t priority[8], const CellSpursSystemWorkloadAttribute* attr );
/*J デコード機能 終了 */ 
int cellMusicDecodeFinalize( void ); 
/*J デコード機能 デコード制御系コマンド発行 */
int cellMusicDecodeSetDecodeCommand( int command );
/*J デコード機能 デコード制御系状態取得 */
int cellMusicDecodeGetDecodeStatus( int *status );
/*J デコード機能 デコードデータ取得 */
int cellMusicDecodeRead( void *buf, uint32_t *startTime, uint64_t reqSize, uint64_t *readSize, int *position );
/*J UIを表示してコンテンツ選択を開始する */
int cellMusicDecodeSelectContents( void );
/*J 選択コンテキスト設定 */
int cellMusicDecodeSetSelectionContext( CellMusicSelectionContext *context );
/*J 選択コンテキスト取得 */
int cellMusicDecodeGetSelectionContext( CellMusicSelectionContext *context );
/*J コンテンツID取得 */
int cellMusicDecodeGetContentsId( CellSearchContentId *contents_id );

#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
}
#endif	/* defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus) */

#endif /*_CELL_SYSUTIL_MUSIC_DECODE_H_*/
