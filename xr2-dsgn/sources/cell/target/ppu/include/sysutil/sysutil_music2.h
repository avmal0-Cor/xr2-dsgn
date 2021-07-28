/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2009 Sony Computer Entertainment Inc.
* All Rights Reserved.
*/

#ifndef _CELL_SYSUTIL_MUSIC2_H_
#define _CELL_SYSUTIL_MUSIC2_H_

#include <cell/spurs/types.h>
#include <cell/spurs/system_workload.h>

#include "sysutil_common.h"
#include "sysutil_search_types.h"
#include "sysutil_music_playback_common.h"

#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
extern "C" {
#endif	/* defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus) */


#define CELL_SYSUTIL_MUSIC2_INITIALIZING_FINISHED  1
#define CELL_SYSUTIL_MUSIC2_SHUTDOWN_FINISHED      4 // 3(SDK103) -> 4(SDK110)
#define CELL_SYSUTIL_MUSIC2_LOADING_FINISHED       5
#define CELL_SYSUTIL_MUSIC2_UNLOADING_FINISHED     7
#define CELL_SYSUTIL_MUSIC2_RELEASED               9
#define CELL_SYSUTIL_MUSIC2_GRABBED               11

#define CELL_MUSIC2_EVENT_STATUS_NOTIFICATION			(0)	/*J	再生中に発生したステータスの通知	*/
#define CELL_MUSIC2_EVENT_INITIALIZE_RESULT				(1)	/*J	cellMusicInitialize2()の実行結果	*/
#define CELL_MUSIC2_EVENT_FINALIZE_RESULT				(2)	/*J	cellMusicFinalize2()の実行結果	*/
#define CELL_MUSIC2_EVENT_SELECT_CONTENTS_RESULT		(3)	/*J	cellMusicSelectContents2()の実行結果	*/
#define CELL_MUSIC2_EVENT_SET_PLAYBACK_COMMAND_RESULT	(4)	/*J	cellMusicSetPlaybackCommand2()の実行結果	*/
#define CELL_MUSIC2_EVENT_SET_VOLUME_RESULT				(5)	/*J	cellMusicSetVolume2()の実行結果	*/
#define CELL_MUSIC2_EVENT_SET_SELECTION_CONTEXT_RESULT	(6)	/*J	cellMusicSetSelectionContext2()の実行結果	*/
#define CELL_MUSIC2_EVENT_UI_NOTIFICATION				(7)	/*J	UIに関する通知	*/

#define CELL_MUSIC2_OK									0			/*J	正常終了	*/
#define CELL_MUSIC2_CANCELED							1			/*J	操作がキャンセルされた	*/
#define CELL_MUSIC2_PLAYBACK_FINISHED					(0x8002c101)	/*J	(CELL_SYSUTIL_ERROR_BASE_MUSIC | 0x01)	最終コンテンツの終端まで到達して再生が停止した	*/
#define CELL_MUSIC2_ERROR_PARAM							(0x8002c102)	/*J	(CELL_SYSUTIL_ERROR_BASE_MUSIC | 0x02)	パラメータが不正 */
#define CELL_MUSIC2_ERROR_BUSY							(0x8002c103)	/*J	(CELL_SYSUTIL_ERROR_BASE_MUSIC | 0x03)	必要な処理が終了していないため要求された処理が実行できなかった	*/
#define CELL_MUSIC2_ERROR_NO_ACTIVE_CONTENT				(0x8002c104)	/*J	(CELL_SYSUTIL_ERROR_BASE_MUSIC | 0x04)	開いているコンテンツがない	*/
#define CELL_MUSIC2_ERROR_NO_MATCH_FOUND				(0x8002c105)	/*J	(CELL_SYSUTIL_ERROR_BASE_MUSIC | 0x05)	渡されたコンテキストに相当するコンテンツが見つからなかった	*/
#define CELL_MUSIC2_ERROR_INVALID_CONTEXT				(0x8002c106)	/*J	(CELL_SYSUTIL_ERROR_BASE_MUSIC | 0x06)	コンテキストが不正	*/
#define CELL_MUSIC2_ERROR_PLAYBACK_FAILURE				(0x8002c107)	/*J	(CELL_SYSUTIL_ERROR_BASE_MUSIC | 0x07)	再生にエラーが発生して停止した	*/
#define CELL_MUSIC2_ERROR_NO_MORE_CONTENT				(0x8002c108)	/*J	(CELL_SYSUTIL_ERROR_BASE_MUSIC | 0x08)	次/前のコンテンツが存在しない	*/
#define CELL_MUSIC2_DIALOG_OPEN							(0x8002c109)	/*J	(CELL_SYSUTIL_ERROR_BASE_MUSIC | 0x09)	メッセージダイアログが表示される	*/
#define CELL_MUSIC2_DIALOG_CLOSE						(0x8002c10A)	/*J	(CELL_SYSUTIL_ERROR_BASE_MUSIC | 0x0A)	メッセージダイアログが閉じられた	*/
#define CELL_MUSIC2_ERROR_GENERIC						(0x8002c1FF)	/*J	(CELL_SYSUTIL_ERROR_BASE_MUSIC | 0xFF)	未知のエラーが発生	*/


/*J 動作モード */
#define CELL_MUSIC2_PLAYER_MODE_NORMAL					(0)	/*J ノーマルモード */

/*J 走行コマンド */
#define CELL_MUSIC2_PB_CMD_STOP							(0)	/*J 停止 */
#define CELL_MUSIC2_PB_CMD_PLAY							(1)	/*J 再生 */
#define CELL_MUSIC2_PB_CMD_PAUSE						(2)	/*J 一時停止 */
#define CELL_MUSIC2_PB_CMD_NEXT							(3)	/*J 次トラック */
#define CELL_MUSIC2_PB_CMD_PREV							(4)	/*J 前トラック */
#define CELL_MUSIC2_PB_CMD_FASTFORWARD					(5)	/*J 早送り */
#define CELL_MUSIC2_PB_CMD_FASTREVERSE					(6)	/*J 早戻し */

/*J 走行ステータス */
#define CELL_MUSIC2_PB_STATUS_STOP						(0)	/*J 停止 */
#define CELL_MUSIC2_PB_STATUS_PLAY						(1)	/*J 再生 */
#define CELL_MUSIC2_PB_STATUS_PAUSE						(2)	/*J 一時停止 */
#define CELL_MUSIC2_PB_STATUS_FASTFORWARD				(3)	/*J 早送り */
#define CELL_MUSIC2_PB_STATUS_FASTREVERSE				(4)	/*J 早戻し */

/*J コールバック関数 */
typedef void (*CellMusic2Callback)(uint32_t event, void *param, void *userData);
#define MUSIC2_SYSTEM_CALLBACK(music2_callback, event, param, userData) static void music2_callback(uint32_t event, void *param, void *userData)

/*J 初期化 */
int cellMusicInitialize2( int mode, int spuPriority, CellMusic2Callback func, void *userData );
/*J 初期化(システムワークロード) */
int cellMusicInitialize2SystemWorkload( int mode, CellMusic2Callback func, void *userData,
										CellSpurs *spurs, const uint8_t priority[8], const CellSpursSystemWorkloadAttribute* attr );
/*J 終了 */
int cellMusicFinalize2( void );
/*J UIを表示してコンテンツ選択を開始する */
int cellMusicSelectContents2( void );
/*J 音量設定 */
int cellMusicSetVolume2( float level );
/*J 走行系コマンド発行 */
int cellMusicSetPlaybackCommand2( int command, void *param );
/*J 選択コンテキスト設定 */
int cellMusicSetSelectionContext2( CellMusicSelectionContext *context );
/*J 音量取得 */
int cellMusicGetVolume2( float *level );
/*J 走行系状態取得 */
int cellMusicGetPlaybackStatus2( int *status );
/*J 選択コンテキスト取得 */
int cellMusicGetSelectionContext2( CellMusicSelectionContext *context );
/*J コンテンツID取得 */
int cellMusicGetContentsId2( CellSearchContentId *contents_id );

#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
}
#endif	/* defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus) */

#endif /*_CELL_SYSUTIL_MUSIC2_H_*/
