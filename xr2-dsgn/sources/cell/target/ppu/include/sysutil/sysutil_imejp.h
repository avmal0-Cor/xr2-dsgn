/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2009 Sony Computer Entertainment Inc.
* All Rights Reserved.
*/

#ifndef _CELL_SYSUTIL_IMEJP_H_
#define _CELL_SYSUTIL_IMEJP_H_

#include "sysutil_common.h"

#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
extern "C" {
#endif	/* defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus) */

/* constant definitions------------------------------------------------------ */

/* ImeJpHandle */
typedef void *CellImeJpHandle;

/*J オプション辞書のパス */
#define CELL_IMEJP_DIC_PATH_MAXLENGTH		256
typedef int8_t CellImeJpAddDic[CELL_IMEJP_DIC_PATH_MAXLENGTH];

/*J IMEJP のエラー */
/* #define CELL_SYSUTIL_ERROR_BASE_IMEJP		(0x8002bf00) */
#define CELL_IMEJP_ERROR_ERR 					(0x8002bf01)	/*J ( CELL_SYSUTIL_ERROR_BASE_IMEJP | 0x01 ) API呼び出しエラー */
#define CELL_IMEJP_ERROR_CONTEXT 				(0x8002bf11)	/*J ( CELL_SYSUTIL_ERROR_BASE_IMEJP | 0x11 ) IMEコンテキストハンドルが不正、あるいはオープンされていない */
#define CELL_IMEJP_ERROR_ALREADY_OPEN			(0x8002bf21)	/*J ( CELL_SYSUTIL_ERROR_BASE_IMEJP | 0x21 ) すでに使用しているコンテナIDに対してオープンが呼ばれた */
#define CELL_IMEJP_ERROR_DIC_OPEN 				(0x8002bf31)	/*J ( CELL_SYSUTIL_ERROR_BASE_IMEJP | 0x31 ) 辞書オープンエラー */
#define CELL_IMEJP_ERROR_PARAM	 				(0x8002bf41)	/*J ( CELL_SYSUTIL_ERROR_BASE_IMEJP | 0x41 ) パラメータが不正 */
#define CELL_IMEJP_ERROR_IME_ALREADY_IN_USE		(0x8002bf51)	/*J ( CELL_SYSUTIL_ERROR_BASE_IMEJP | 0x51 ) すでにIMEモジュールが使用中である */
#define CELL_IMEJP_ERROR_OTHER 					(0x8002bfff)	/*J ( CELL_SYSUTIL_ERROR_BASE_IMEJP | 0xff ) それ以外のエラー、内部エラー */

/*J 入力モード設定(cellImeJpSetKanaInputMode) */
#define CELL_IMEJP_ROMAN_INPUT			0		/*J ローマ字入力モード	*/
#define CELL_IMEJP_KANA_INPUT			1		/*J かな入力モード 		*/

/*J 正規化入力種別設定(cellImeJpSetInputCharType) */
#define CELL_IMEJP_DSPCHAR_HIRA			1		/*J ひらがな			*/
#define CELL_IMEJP_DSPCHAR_FKANA		2		/*J カタカナ			*/
#define CELL_IMEJP_DSPCHAR_RAW			3		/*J 全角無変換			*/
#define CELL_IMEJP_DSPCHAR_HRAW			5		/*J 半角無変換			*/

/*J 固定入力文字種設定(cellImeJpSetFixInputMode) */
#define CELL_IMEJP_FIXINMODE_OFF		0		/*J 固定解除			*/
#define CELL_IMEJP_FIXINMODE_HIRA		1		/*J ひらがな固定		*/
#define CELL_IMEJP_FIXINMODE_FKANA		2		/*J カタカナ固定		*/
#define CELL_IMEJP_FIXINMODE_RAW		3		/*J 全角無変換固定		*/
#define CELL_IMEJP_FIXINMODE_HRAW		5		/*J 半角無変換固定		*/

/*J IME状態の取得(cellImeJpGetStatus) */
#define CELL_IMEJP_BEFORE_INPUT			0		/*J 未入力				*/
#define CELL_IMEJP_BEFORE_CONVERT		1		/*J 変換前				*/
#define CELL_IMEJP_CONVERTING			2		/*J 変換中				*/
#define CELL_IMEJP_CANDIDATE_EMPTY		3		/*J 候補切れ(候補なし)	*/
#define CELL_IMEJP_POSTCONVERT_KANA		4		/*J 後変換(カタカナ)	*/
#define CELL_IMEJP_POSTCONVERT_HALF		5		/*J 後変換(半角)		*/
#define CELL_IMEJP_POSTCONVERT_RAW		6		/*J 後変換(無変換)		*/
#define CELL_IMEJP_CANDIDATES			7		/*J 候補一覧			*/
#define CELL_IMEJP_MOVE_CLAUSE_GAP		8		/*J 文節区切り直し		*/

/*J 後変換(cellImeJpPostConvert) */
#define CELL_IMEJP_POSTCONV_HIRA		1		/*J ひらがな後変換		*/
#define CELL_IMEJP_POSTCONV_KANA		2		/*J カタカナ後変換		*/
#define CELL_IMEJP_POSTCONV_HALF		3		/*J 半角後変換			*/
#define CELL_IMEJP_POSTCONV_RAW			4		/*J 無変換後変換		*/

/*J 注目文節移動(cellImeJpMoveFocusCls) */
#define CELL_IMEJP_FOCUS_NEXT			0		/*J 一文節右に移動		*/
#define CELL_IMEJP_FOCUS_BEFORE			1		/*J 一文節左に移動		*/
#define CELL_IMEJP_FOCUS_TOP			2		/*J 先頭の文節に移動	*/
#define CELL_IMEJP_FOCUS_END			3		/*J 最後の文節に移動	*/

/*J 編集・変換の制御結果 */
#define CELL_IMEJP_RET_NONE				0		/*J 出力なし			*/
#define CELL_IMEJP_RET_THROUGH			1		/*J スルー出力あり		*/
#define CELL_IMEJP_RET_CONFIRMED		2		/*J 確定出力あり		*/


/* (推測系)推測変換リスト用構造体 */
typedef struct CellImeJpPredictItem{
	uint16_t KanaYomi[31];				/*J 半角読み文字列(終端保証)	*/
	uint16_t Hyoki[61];					/*J 表記(終端保証)				*/
}CellImeJpPredictItem;


/*J プロトタイプ宣言 */
/*
 * 開始/終了関係関数
 */
/*J IMEライブラリの使用開始 */
int cellImeJpOpen( sys_memory_container_t container_id, CellImeJpHandle *hImeJpHandle, const CellImeJpAddDic addDicPath );
int cellImeJpOpen2( sys_memory_container_t container_id, CellImeJpHandle *hImeJpHandle, const CellImeJpAddDic addDicPath );
int cellImeJpOpen3( sys_memory_container_t container_id, CellImeJpHandle *hImeJpHandle, const CellImeJpAddDic *addDicPath );

/*J IMEライブラリの終了 */
int cellImeJpClose( CellImeJpHandle hImeJpHandle );

/*
 * 各種設定関係関数
 */
/*J 入力モード設定 */
int cellImeJpSetKanaInputMode( CellImeJpHandle hImeJpHandle, int16_t inputOption );

/*J 入力文字種の設定 */
int cellImeJpSetInputCharType( CellImeJpHandle hImeJpHandle, int16_t charTypeOption );

/*J 固定入力文字種設定 */
int cellImeJpSetFixInputMode( CellImeJpHandle hImeJpHandle, int16_t fixInputMode );

/*J IMEの内部状態リセット */
int cellImeJpReset( CellImeJpHandle hImeJpHandle );

/*
 * 入力状態の取得関係関数
 */
/*J IME状態の取得 */
int cellImeJpGetStatus( CellImeJpHandle hImeJpHandle, int16_t *pInputStatus );

/*
 * 変換・編集関係関数
 */
/*J 一文字入力 */
int cellImeJpEnterChar( CellImeJpHandle hImeJpHandle, uint16_t inputChar, int16_t *pOutputStatus );

/*J 一文字入力(文字種自動判別表示) */
int cellImeJpEnterCharExt( CellImeJpHandle hImeJpHandle, uint16_t inputChar, int16_t *pOutputStatus );

/*J 文字列入力 */
int cellImeJpEnterString( CellImeJpHandle hImeJpHandle, const uint16_t *pInputString, int16_t *pOutputStatus );

/*J 文字列入力(文字種自動判別表示) */
int cellImeJpEnterStringExt( CellImeJpHandle hImeJpHandle, const uint16_t *pInputString, int16_t *pOutputStatus );

/*J 一文字移動(右) */
int cellImeJpModeCaretRight( CellImeJpHandle hImeJpHandle );

/*J 一文字移動(左) */
int cellImeJpModeCaretLeft( CellImeJpHandle hImeJpHandle );

/*J 一文字削除(カーソルの左側) */
int cellImeJpBackspaceWord( CellImeJpHandle hImeJpHandle );

/*J 一文字削除(カーソルの位置) */
int cellImeJpDeleteWord( CellImeJpHandle hImeJpHandle );

/*J 全文字削除 */
int cellImeJpAllDeleteConvertString( CellImeJpHandle hImeJpHandle );

/*J 前方変換 */
int cellImeJpConvertForward( CellImeJpHandle hImeJpHandle );

/*J 後方変換 */
int cellImeJpConvertBackward( CellImeJpHandle hImeJpHandle );

/*J 注目文節確定 */
int cellImeJpCurrentPartConfirm( CellImeJpHandle hImeJpHandle, int16_t listItem );

/*J 全確定 */
int cellImeJpAllConfirm( CellImeJpHandle hImeJpHandle );

/*J 変換キャンセル */
int cellImeJpConvertCancel( CellImeJpHandle hImeJpHandle );

/*J 全変換キャンセル */
int cellImeJpAllConvertCancel( CellImeJpHandle hImeJpHandle );

/*J 注目文節伸張 */
int cellImeJpExtendConvertArea( CellImeJpHandle hImeJpHandle );

/*J 注目文節縮小 */
int cellImeJpShortenConvertArea( CellImeJpHandle hImeJpHandle );

/*J 変換候補リストサイズ取得 */
int cellImeJpGetCandidateListSize ( CellImeJpHandle hImeJpHandle, int16_t *pListSize );

/*J 変換候補リスト取得 */
int cellImeJpGetCandidateList( CellImeJpHandle hImeJpHandle, int16_t *plistNum, uint16_t *pCandidateString );

/*J 仮候補選択 */
int cellImeJpTemporalConfirm( CellImeJpHandle hImeJpHandle, int16_t selectIndex );

/*J 後変換 */
int cellImeJpPostConvert( CellImeJpHandle hImeJpHandle, int16_t postType );

/*J 注目文節移動 */
int cellImeJpMoveFocusClause( CellImeJpHandle hImeJpHandle, int16_t moveType );

/*J フォーカス位置取得 */
int cellImeJpGetFocusTop( CellImeJpHandle hImeJpHandle, int16_t *pFocusTop );

/*J フォーカス長取得 */
int cellImeJpGetFocusLength( CellImeJpHandle hImeJpHandle, int16_t *pFocusLength );

/*J 確定文字列の読み取得 */
int cellImeJpGetConfirmYomiString( CellImeJpHandle hImeJpHandle, uint16_t *pYomiString );

/*J 確定文字列取得 */
int cellImeJpGetConfirmString( CellImeJpHandle hImeJpHandle, uint16_t *pConfirmString );

/*J 未確定文字列の読み取得 */
int cellImeJpGetConvertYomiString( CellImeJpHandle hImeJpHandle, uint16_t *pYomiString );

/*J 未確定文字列取得 */
int cellImeJpGetConvertString( CellImeJpHandle hImeJpHandle, uint16_t *pConvertString );

/*J 変換候補選択位置取得(1起点) */
int cellImeJpGetCandidateSelect( CellImeJpHandle hImeJpHandle, int16_t *pIndex );

/* (推測系)推測リスト取得 */
int cellImeJpGetPredictList( CellImeJpHandle hImeJpHandle, uint16_t *pYomiString, int32_t itemNum, int32_t *plistCount, CellImeJpPredictItem *pPredictItem );

/* (推測系)推測確定 */
int cellImeJpConfirmPrediction( CellImeJpHandle hImeJpHandle, CellImeJpPredictItem *pPredictItem );


#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
}
#endif	/* defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus) */

#endif /*_CELL_SYSUTIL_IMEJP_H_*/
