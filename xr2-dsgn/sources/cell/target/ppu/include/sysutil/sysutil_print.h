/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2007 Sony Computer Entertainment Inc.
* All Rights Reserved.
*/

#ifndef _CELL_SYSUTIL_PRINT_H_
#define _CELL_SYSUTIL_PRINT_H_

#include "sysutil_common.h"

#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
extern "C" {
#endif	/* defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus) */

/*J 起動モード */
enum
{
	CELL_PRINT_MODE_NORMAL = 0   /*J ノーマル */
};

/* 印字データのデータフォーマット */
enum
{
	CELL_PRINT_PARAM_COLOR_FORMAT_R8G8B8 = 0   /*J RGB24bit */
};

/*J プリンタの状態 */
enum
{
	CELL_PRINT_STATUS_IDLE			= 0,	/*J 印字可能 */
	CELL_PRINT_STATUS_PRINTING		= 1,	/*J エラー発生中 */
	CELL_PRINT_STATUS_OFFLINE		= 2,	/*J プリンタ未接続 */
	CELL_PRINT_STATUS_BUSY			= 3,	/*J 他の機能動作中のため使用不可 */
	CELL_PRINT_STATUS_CANCELLING	= 4,	/*J キャンセル処理中 */
	CELL_PRINT_STATUS_ERROR			= 5		/*J 印刷中 */
};

/*J プリンタのエラーコード */
#define CELL_PRINT_ERRORSTATUS_COMMON_OFFSET	(0x80000000)
enum {
    CELL_PRINT_ERRORSTATUS_NONE = 0x00000000,
    CELL_PRINT_ERRORSTATUS_COMM = CELL_PRINT_ERRORSTATUS_COMMON_OFFSET,   /*J プリンタ通信エラー */
    CELL_PRINT_ERRORSTATUS_COVEROPEN,                /*J カバー・オープン */
    CELL_PRINT_ERRORSTATUS_INKCOVEROPEN,             /*J インク(カートリッジ)カバー・オープン */
    CELL_PRINT_ERRORSTATUS_NOPAPER,                  /*J 用紙無し */
    CELL_PRINT_ERRORSTATUS_NOTRAY,                   /*J 用紙トレイ無し */
    CELL_PRINT_ERRORSTATUS_SIZE_TYPE_PATH,           /*J 給紙方法ミスマッチ */
    CELL_PRINT_ERRORSTATUS_DOUBLEFEED,               /*J 用紙給紙エラー(重送エラー) */
    CELL_PRINT_ERRORSTATUS_FEEDERROR,                /*J 用紙給紙エラー(その他のエラー) */
    CELL_PRINT_ERRORSTATUS_PAPERJAM,                 /*J 紙詰まり */
    CELL_PRINT_ERRORSTATUS_TRAYCLOSE,                /*J 排紙トレイ・クローズ */
    CELL_PRINT_ERRORSTATUS_NOINK,                    /*J インク無し */
    CELL_PRINT_ERRORSTATUS_NOCARTRIDGE,              /*J インクカートリッジ無し */
    CELL_PRINT_ERRORSTATUS_COVERFLOW,                /*J インクカートリッジ廃インクフル */
    CELL_PRINT_ERRORSTATUS_CFAIL,                    /*J インクカートリッジ認識エラー */
    CELL_PRINT_ERRORSTATUS_CFACTORY,                 /*J インクカートリッジエラー(工場出荷状態) */
    CELL_PRINT_ERRORSTATUS_BATTERYVOLTAGE,           /*J バッテリ電圧異常 */
    CELL_PRINT_ERRORSTATUS_BATTERYTEMPERATURE,       /*J バッテリ温度異常 */
    CELL_PRINT_ERRORSTATUS_BATTERYEMPTY,             /*J バッテリ残量無し(印刷不可) */
    CELL_PRINT_ERRORSTATUS_SERVICEREQ,               /*J プリンタ調整が必要 */
    CELL_PRINT_ERRORSTATUS_GENERAL,                  /*J プリンタに依存したエラーが発生 */
    CELL_PRINT_ERRORSTATUS_FATAL,                    /*J プリンタに致命的エラーが発生 */
};

/*J プリンタ詳細設定画面の押下種別 */
enum {
	CELL_PRINT_RESULT_OK = 0,		/* 処理正常終了 */
	CELL_PRINT_RESULT_CANCEL,		/* キャンセル終了 */
	CELL_PRINT_RESULT_ERROR,		/* エラー終了 */
};

/*J 印字継続可能かどうか表す定数 */
#define CELL_PRINT_CONTINUE_DISABLED	0	/*J 印字継続不可能 */
#define CELL_PRINT_CONTINUE_ENABLED		1	/*J 印字継続可能 */

/*J 起動パラメータの構造体 */
typedef struct {
	unsigned int mode;					/*J ユーティリティ起動モード */
	unsigned char reserved[32];			/*J 予約 */
} CellPrintLoadParam;

/*J プリンタの状態を格納する構造体 */
typedef struct {
	int status;							/*J プリンタの状態 */
	int errorStatus;					/*J エラー発生時のエラーコード */
	int continueEnabled;				/*J エラー発生時処理を継続可能かどうかを示す */
	unsigned char reserved[32];			/*J 予約 */
} CellPrintStatus;

/*J エラーコード */
/* #define CELL_SYSUTIL_ERROR_BASE_PRINT		(0x8002c400) */
#define CELL_PRINT_ERROR_INTERNAL				(0x8002c401)	/*J 0x8002c401 内部エラー */
#define CELL_PRINT_ERROR_NO_MEMORY				(0x8002c402)	/*J 0x8002c402 メモリ確保に失敗 */
#define CELL_PRINT_ERROR_PRINTER_NOT_FOUND		(0x8002c403)	/*J 0x8002c403 プリンタが見つからない */
#define CELL_PRINT_ERROR_INVALID_PARAM			(0x8002c404)	/*J 0x8002c404 パラメータが不正 */
#define CELL_PRINT_ERROR_INVALID_FUNCTION		(0x8002c405)	/*J 0x8002c405 API呼び出しが不正 */
#define CELL_PRINT_ERROR_NOT_SUPPORT			(0x8002c406)	/*J 0x8002c406 プリンタが対応していない */
#define CELL_PRINT_ERROR_OCCURRED				(0x8002c407)	/*J 0x8002c407 API実行エラー */
#define CELL_PRINT_ERROR_CANCELED_BY_PRINTER	(0x8002c408)	/*J 0x8002c408 印刷ジョブがキャンセルされた */


/*J プロトタイプ宣言 */

/*J 結果通知コールバック関数 */
typedef void ( *CellPrintCallback )( int result, void *userdata );

/*J ロード */
int cellPrintLoadAsync( CellPrintCallback function, 
						void *userdata, 
						const CellPrintLoadParam *param, 
						sys_memory_container_t container );
/*J ロード */
int cellPrintLoadAsync2( CellPrintCallback function, 
						 void *userdata, 
						 const CellPrintLoadParam *param );

/*J アンロード */
int cellPrintUnloadAsync( CellPrintCallback function, void *userdata );
/*J プリント設定ダイアログをオープン */
int cellPrintOpenConfig( CellPrintCallback function, void *userdata );
/*J プリントジョブ開始 */
int cellPrintStartJob( int totalPage, int colorFormat );
/*J プリントジョブ終了 */
int cellPrintEndJob( void );
/*J プリントジョブ強制終了 */
int cellPrintCancelJob( void );
/*J プリントページ開始 */
int cellPrintStartPage( void );
/*J プリントページ終了 */
int cellPrintEndPage( void );
/*J 印字可能領域を取得 */
int cellPrintGetPrintableArea( int *pixelWidth, int *pixelHeight );
/*J 印字データ転送 */
int cellPrintSendBand( const unsigned char *buff, int buffsize, int *sendsize );
/*J プリンタの状態取得 */
int cellPrintGetStatus( CellPrintStatus *status );

#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
}
#endif	/* defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus) */

#endif /*_CELL_SYSUTIL_PRINT_H_*/
