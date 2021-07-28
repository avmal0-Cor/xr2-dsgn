/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2009 Sony Computer Entertainment Inc.
* All Rights Reserved.
*/

#ifndef _CELL_SYSUTIL_BGMPLAYBACK_H_
#define _CELL_SYSUTIL_BGMPLAYBACK_H_

#include "sysutil_common.h"
#include "sysutil_search_types.h"

#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
extern "C" {
#endif	/* defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus) */


#define CELL_SYSUTIL_BGMPLAYBACK_OK							0				/*J	正常終了	*/
#define CELL_SYSUTIL_BGMPLAYBACK_ERROR_PARAM				(0x8002b101)	/*J	(CELL_SYSUTIL_ERROR_BASE_COMMON | 0x01)	パラメータが不正 */
#define CELL_SYSUTIL_BGMPLAYBACK_ERROR_BUSY					(0x8002b102)	/*J	(CELL_SYSUTIL_ERROR_BASE_COMMON | 0x02)	必要な処理が終了していないため要求された処理が実行できなかった	*/
#define CELL_SYSUTIL_BGMPLAYBACK_ERROR_GENERIC				(0x8002b1FF)	/*J	(CELL_SYSUTIL_ERROR_BASE_COMMON | 0xFF)	未知のエラーが発生	*/

#define CELL_SYSUTIL_BGMPLAYBACK_EX_OK						0				/*J	正常終了	*/
#define CELL_SYSUTIL_BGMPLAYBACK_EX_ERROR_PARAM				(0x8002d301)	/*J	(CELL_SYSUTIL_ERROR_BASE_BGMPLAYBACK_EX | 0x01)	パラメータが不正 */
#define CELL_SYSUTIL_BGMPLAYBACK_EX_ERROR_ALREADY_SETPARAM	(0x8002d302)	/*J	(CELL_SYSUTIL_ERROR_BASE_BGMPLAYBACK_EX | 0x02)	設定済みのため受付不可 */
#define CELL_SYSUTIL_BGMPLAYBACK_EX_ERROR_DISABLE_SETPARAM	(0x8002d303)	/*J	(CELL_SYSUTIL_ERROR_BASE_BGMPLAYBACK_EX | 0x03)	禁止中のため受付不可 */
#define CELL_SYSUTIL_BGMPLAYBACK_EX_ERROR_GENERIC			(0x8002d3FF)	/*J	(CELL_SYSUTIL_ERROR_BASE_BGMPLAYBACK_EX | 0xFF)	未知のエラーが発生 */


#define CELL_SYSUTIL_BGMPLAYBACK_STATUS_PLAY				(0)				/*J BGM再生中 */
#define CELL_SYSUTIL_BGMPLAYBACK_STATUS_STOP				(1)				/*J BGM停止中 */

#define CELL_SYSUTIL_BGMPLAYBACK_STATUS_ENABLE				(0)				/*J BGM許可中 */
#define CELL_SYSUTIL_BGMPLAYBACK_STATUS_DISABLE				(1)				/*J BGM禁止中 */


/*J システムBGM再生状態サイズ */
#define CELL_SYSUTIL_BGMPLAYBACK_STATUS						32

typedef struct{
    uint8_t  playerState;
    uint8_t  enableState;
	CellSearchContentId  contentId;
    uint8_t  currentFadeRatio;
	char reserved[CELL_SYSUTIL_BGMPLAYBACK_STATUS - 3 - sizeof(CellSearchContentId)];
} CellSysutilBgmPlaybackStatus;

#define CELL_SYSUTIL_BGMPLAYBACK_FADE_INVALID				-1				/*J FADE無効 */

typedef struct{
	int32_t  systemBgmFadeInTime;
	int32_t  systemBgmFadeOutTime;
	int32_t  gameBgmFadeInTime;
	int32_t  gameBgmFadeOutTime;
	char reserved[8];
} CellSysutilBgmPlaybackExtraParam;

/*J システムBGM機能による音楽再生状態 */
#define CELL_SYSUTIL_BGMPLAYBACK_STATUS2					8

typedef struct{
	uint8_t  playerState;
	char reserved[CELL_SYSUTIL_BGMPLAYBACK_STATUS2 - 1];
} CellSysutilBgmPlaybackStatus2;


/*J システムBGM再生禁止 */
int cellSysutilDisableBgmPlayback( void );
/*J システムBGM再生許可 */
int cellSysutilEnableBgmPlayback( void );
/*J システムBGM再生状態の取得 */
int cellSysutilGetBgmPlaybackStatus( CellSysutilBgmPlaybackStatus *status );

/*J システムBGMデフォルトパラメータ設定(拡張機能) */
int cellSysutilSetBgmPlaybackExtraParam( CellSysutilBgmPlaybackExtraParam *param );
/*J システムBGM再生禁止(拡張機能) */
int cellSysutilDisableBgmPlaybackEx( CellSysutilBgmPlaybackExtraParam *param );
/*J システムBGM再生許可(拡張機能) */
int cellSysutilEnableBgmPlaybackEx( CellSysutilBgmPlaybackExtraParam *param );

/*J システムBGM機能による音楽再生状態の取得 */
int cellSysutilGetBgmPlaybackStatus2( CellSysutilBgmPlaybackStatus2 *status );

#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
}
#endif	/* defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus) */

#endif /*_CELL_SYSUTIL_BGMPLAYBACK_H_*/
