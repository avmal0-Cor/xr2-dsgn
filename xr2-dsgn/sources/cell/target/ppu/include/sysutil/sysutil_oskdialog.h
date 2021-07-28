/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2007 Sony Computer Entertainment Inc.
* All Rights Reserved.
*/

#ifndef _CELL_SYSUTIL_OSKDIALOG_H_
#define _CELL_SYSUTIL_OSKDIALOG_H_

#include "sysutil_common.h"

#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
extern "C" {
#endif	/* defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus) */

/*J OSK の状態 */
#define CELL_SYSUTIL_OSKDIALOG_LOADED				(0x0502)		/*J ダイアログ表示中（入力操作中） */
#define CELL_SYSUTIL_OSKDIALOG_FINISHED				(0x0503)		/*J ユーザが入力完了またはキャンセル操作をした、または強制終了が呼び出された */
#define CELL_SYSUTIL_OSKDIALOG_UNLOADED				(0x0504)		/*J ダイアログが閉じられた */
#define CELL_SYSUTIL_OSKDIALOG_INPUT_ENTERED		(0x0505)		/*J ユーザによる入力確定があった */
#define CELL_SYSUTIL_OSKDIALOG_INPUT_CANCELED		(0x0506)		/*J ユーザによる入力キャンセルがあった */
#define CELL_SYSUTIL_OSKDIALOG_INPUT_DEVICE_CHANGED	(0x0507)		/*J 入力対象デバイスが変更された */
#define CELL_SYSUTIL_OSKDIALOG_DISPLAY_CHANGED		(0x0508)		/*J ダイアログの表示／非表示が変更された */

/* constant definitions------------------------------------------------------ */
/*J OSK のエラー */
#define CELL_OSKDIALOG_ERROR_IME_ALREADY_IN_USE (0x8002b501)	/*J ( CELL_SYSUTIL_ERROR_BASE_OSKDIALOG | 0x01 ) 文字入力エンジンが使用中 */
#define CELL_OSKDIALOG_ERROR_GET_SIZE_ERROR		(0x8002b502)	/*J ( CELL_SYSUTIL_ERROR_BASE_OSKDIALOG | 0x02 ) ダイアログサイズ取得エラー */
#define CELL_OSKDIALOG_ERROR_UNKNOWN 			(0x8002b503)	/*J ( CELL_SYSUTIL_ERROR_BASE_OSKDIALOG | 0x03 ) 未知のエラーが発生 */
#define CELL_OSKDIALOG_ERROR_PARAM	 			(0x8002b504)	/*J ( CELL_SYSUTIL_ERROR_BASE_OSKDIALOG | 0x04 ) パラメータが不正 */

/*J OSK ユーティリティの状態 */
typedef enum {
	CELL_OSKDIALOG_LOCAL_STATUS_NONE						= (0),		/*J 未初期化 */
	CELL_OSKDIALOG_LOCAL_STATUS_INITIALIZING				= (1),		/*J 初期化中 */
	CELL_OSKDIALOG_LOCAL_STATUS_INITIALIZED					= (2),		/*J 初期化済み */
	CELL_OSKDIALOG_LOCAL_STATUS_OPENED						= (3),		/*J サービス開始 */
	CELL_OSKDIALOG_LOCAL_STATUS_CLOSING						= (4),		/*J サービスを終了中 */
	CELL_OSKDIALOG_LOCAL_STATUS_CLOSED						= (5),		/*J サービスを終了 */
	CELL_OSKDIALOG_LOCAL_STATUS_CONTINUE					= (6),		/*J ユーザー選択通知、サービスを続行中 */
	CELL_OSKDIALOG_LOCAL_STATUS_CHANGED_TO_KEYBOARD_INPUT	= (7),		/*J 対象入力デバイスをキーボードに変更 */
	CELL_OSKDIALOG_LOCAL_STATUS_CHANGED_TO_PAD_INPUT		= (8),		/*J 対象入力デバイスをパッドに変更 */
	CELL_OSKDIALOG_LOCAL_STATUS_HIDE_OSK					= (9),		/*J 確定続行により非表示化完了 */
	CELL_OSKDIALOG_LOCAL_STATUS_SHOW_OSK					= (10), 	/*J 入力検知により、表示開始 */
} CellOskDialogLocalStatus;

/*J 入力モード */
#define CELL_OSKDIALOG_PANELMODE_DEFAULT				0x00000000	/*J デフォルトパネル */
#define CELL_OSKDIALOG_PANELMODE_DEFAULT_NO_JAPANESE	0x00000200	/*J デフォルトパネル(日本語禁止) */

#define CELL_OSKDIALOG_PANELMODE_GERMAN					0x00000001	/*J ドイツ語+数字入力 */
#define CELL_OSKDIALOG_PANELMODE_ENGLISH				0x00000002	/*J 英語+数字入力 */
#define CELL_OSKDIALOG_PANELMODE_SPANISH				0x00000004	/*J スペイン語+数字入力 */
#define CELL_OSKDIALOG_PANELMODE_FRENCH					0x00000008	/*J フランス語+数字入力 */
#define CELL_OSKDIALOG_PANELMODE_ITALIAN				0x00000010	/*J イタリア語+数字入力 */
#define CELL_OSKDIALOG_PANELMODE_DUTCH					0x00000020	/*J オランダ語+数字入力 */
#define CELL_OSKDIALOG_PANELMODE_PORTUGUESE				0x00000040	/*J ポルトガル語+数字入力 */
#define CELL_OSKDIALOG_PANELMODE_RUSSIAN				0x00000080	/*J ロシア語+数字入力 */
#define CELL_OSKDIALOG_PANELMODE_JAPANESE				0x00000100	/*J 日本語入力 */
#define CELL_OSKDIALOG_PANELMODE_KOREAN					0x00001000	/*J 韓語+数字入力 */
#define CELL_OSKDIALOG_PANELMODE_DANISH					0x00020000	/*J デンマーク語+数字入力 */
#define CELL_OSKDIALOG_PANELMODE_SWEDISH				0x00040000	/*J スウェーデン語+数字入力 */
#define CELL_OSKDIALOG_PANELMODE_NORWEGIAN				0x00080000	/*J ノルウェー語+数字入力 */
#define CELL_OSKDIALOG_PANELMODE_FINNISH				0x00100000	/*J フィンランド語+数字入力 */
#define CELL_OSKDIALOG_PANELMODE_POLISH					0x00000400	/*J ポーランド語+数字入力 */

#define CELL_OSKDIALOG_PANELMODE_CHINA_TRADITIONAL		0x00004000	/*J 繁体字中国語 */
#define CELL_OSKDIALOG_PANELMODE_TRADITIONAL_CHINESE	0x00004000	/*J 繁体字中国語 */
#define CELL_OSKDIALOG_PANELMODE_SIMPLIFIED_CHINESE		0x00008000	/*J 簡体字中国語 */

#define CELL_OSKDIALOG_PANELMODE_JAPANESE_HIRAGANA		0x00200000	/*J ひらがな入力 */
#define CELL_OSKDIALOG_PANELMODE_JAPANESE_KATAKANA		0x00400000	/*J カタカナ入力 */
#define CELL_OSKDIALOG_PANELMODE_ALPHABET_FULL_WIDTH	0x00800000	/*J 全角英数 */
#define CELL_OSKDIALOG_PANELMODE_ALPHABET				0x01000000	/*J 半角英数 */
#define CELL_OSKDIALOG_PANELMODE_LATIN					0x02000000	/*J ラテン+数字入力 */
#define CELL_OSKDIALOG_PANELMODE_NUMERAL_FULL_WIDTH		0x04000000	/*J 全角数字 */
#define CELL_OSKDIALOG_PANELMODE_NUMERAL				0x08000000	/*J 半角数字 */
#define CELL_OSKDIALOG_PANELMODE_URL					0x10000000	/*J URL入力 */
#define CELL_OSKDIALOG_PANELMODE_PASSWORD				0x20000000	/*J パスワード入力 */

/*J 座標指定のオプション */
#define CELL_OSKDIALOG_LAYOUTMODE_X_ALIGN_LEFT		0x00000200	/*J x座標指定基準点-ダイアログ左   */
#define CELL_OSKDIALOG_LAYOUTMODE_X_ALIGN_CENTER	0x00000400	/*J x座標指定基準点-ダイアログ中央 */
#define CELL_OSKDIALOG_LAYOUTMODE_X_ALIGN_RIGHT		0x00000800	/*J x座標指定基準点-ダイアログ右   */
#define CELL_OSKDIALOG_LAYOUTMODE_Y_ALIGN_TOP		0x00001000	/*J y座標指定基準点-ダイアログ上端 */
#define CELL_OSKDIALOG_LAYOUTMODE_Y_ALIGN_CENTER	0x00002000	/*J y座標指定基準点-ダイアログ中央 */
#define CELL_OSKDIALOG_LAYOUTMODE_Y_ALIGN_BOTTOM	0x00004000	/*J y座標指定基準点-ダイアログ下端 */

/*J 禁止動作設定フラグ */
#define CELL_OSKDIALOG_NO_SPACE				0x00000001	/*J スペースの入力禁止 */
#define CELL_OSKDIALOG_NO_RETURN			0x00000002	/*J 改行の禁止（1ライン固定入力） */
#define CELL_OSKDIALOG_NO_INPUT_ANALOG		0x00000008	/*J 右スティックを用いた操作の禁止 */
#define CELL_OSKDIALOG_NO_STARTUP_EFFECT	0x00001000	/*J 起動時のフェードインで行う、大きさと移動のアニメーションを禁止 */

/*J 有効パネル設定フラグ */
#define CELL_OSKDIALOG_10KEY_PANEL		0x00000001	/*J テンキーパネル */
#define CELL_OSKDIALOG_FULLKEY_PANEL	0x00000002	/*J フルキーボードパネル */

/*J OSKダイアログストリングのサイズ */
#define CELL_OSKDIALOG_STRING_SIZE		(512)

/*J イベント取得権設定フラグ */
#define CELL_OSKDIALOG_DEVICE_MASK_PAD	0x000000ff 	/*J コントローライベントを取得しない */

/*J OSKダイアログの初期起動設定 */
typedef enum {
	CELL_OSKDIALOG_INITIAL_PANEL_LAYOUT_SYSTEM	= (0),		/*J ユーザーがシステムソフトウェアで使用していたキーレイアウトで起動 */
	CELL_OSKDIALOG_INITIAL_PANEL_LAYOUT_10KEY	= (1),		/*J テンキーパネルで起動 */
	CELL_OSKDIALOG_INITIAL_PANEL_LAYOUT_FULLKEY	= (2),		/*J フルキーボードパネルで起動 */
}CellOskDialogInitialKeyLayout;

/*J 各入力フィールド個別の戻り値 */
typedef enum {
	CELL_OSKDIALOG_INPUT_FIELD_RESULT_OK			= (0),	/*J 正常に終了した */
	CELL_OSKDIALOG_INPUT_FIELD_RESULT_CANCELED		= (1),	/*J ユーザがキャンセル操作をした */
	CELL_OSKDIALOG_INPUT_FIELD_RESULT_ABORT			= (2),	/*J 強制終了 */
	CELL_OSKDIALOG_INPUT_FIELD_RESULT_NO_INPUT_TEXT	= (3),	/*J 入力文字列がない */
} CellOskDialogInputFieldResult;

/*J OSK 入力デバイス */
typedef enum {
	CELL_OSKDIALOG_INPUT_DEVICE_PAD			= (0),		/*J コントローラ */
	CELL_OSKDIALOG_INPUT_DEVICE_KEYBOARD	= (1),		/*J キーボード */
} CellOskDialogInputDevice;

/*J ウィンドウ分離型で入力確定・キャンセル時の動作設定 */
typedef enum {
	CELL_OSKDIALOG_CONTINUOUS_MODE_NONE			= (0),		/*J 確定続行オプションを使用しない */
	CELL_OSKDIALOG_CONTINUOUS_MODE_REMAIN_OPEN	= (1),		/*J 確定続行オプションを使用(オンスクリーンキーボードダイアログは常に表示) */
	CELL_OSKDIALOG_CONTINUOUS_MODE_HIDE			= (2),		/*J 確定続行オプションを使用(初期状態=非表示) */
	CELL_OSKDIALOG_CONTINUOUS_MODE_SHOW			= (3),		/*J 確定続行オプションを使用(初期状態=表示) */
} CellOskDialogContinuousMode;

/*J 表示/非表示の通知パラメータ */
typedef enum {
	CELL_OSKDIALOG_DISPLAY_STATUS_HIDE		 	= (0),		/*J ダイアログ非表示化完了 */
	CELL_OSKDIALOG_DISPLAY_STATUS_SHOW		 	= (1),		/*J ダイアログ表示開始 */
} CellOskDialogDisplayStatus;

/* simple variable type definitions------------------------------------------ */
/* nothing */
/* structure definitions----------------------------------------------------- */

/*J 入力フィールド情報の構造体 */
typedef struct {
	uint16_t* message;		/*J OSKアプリの左上にガイドとして表示される文字列 */
	uint16_t* init_text;	/*J 初期入力文字列 */
	int limit_length;		/*J 入力文字の制限値 (単位は「文字」(バイトではありません))*/
} CellOskDialogInputFieldInfo;

/*J コールバック用構造体 */
typedef struct {
	CellOskDialogInputFieldResult result;
	int numCharsResultString;
	uint16_t *pResultString;
} CellOskDialogCallbackReturnParam;

/*J 入力コントロールの位置、サイズ情報構造体 */
typedef struct{
	float x;
	float y;
} CellOskDialogPoint;

/*J ウィンドウのレイアウト情報 */
typedef struct {
	int layoutMode;						/*J 表示領域の基準点に対するオプション */
	CellOskDialogPoint position;		/*J 表示位置をスクリーン座標系で指定 */
} CellOskDialogLayoutInfo;

/*J OSKの種類 */
typedef enum {
	CELL_OSKDIALOG_TYPE_SINGLELINE_OSK	= (0),						/*J 単一行入力ダイアログ */
	CELL_OSKDIALOG_TYPE_MULTILINE_OSK	= (1),						/*J 複数行入力ダイアログ */
	CELL_OSKDIALOG_TYPE_FULL_KEYBOARD_SINGLELINE_OSK,				/*J 単一行フルキーボード入力ダイアログ */
	CELL_OSKDIALOG_TYPE_FULL_KEYBOARD_MULTILINE_OSK,				/*J 複数行フルキーボード入力ダイアログ */
	CELL_OSKDIALOG_TYPE_SEPARATE_SINGLELINE_FIELD_WINDOW,			/*J 分離型の単一行入力フィールドウィンドウ */
	CELL_OSKDIALOG_TYPE_SEPARATE_MULTILINE_FIELD_WINDOW,			/*J 分離型の複数行入力フィールドウィンドウ */
	CELL_OSKDIALOG_TYPE_SEPARATE_INPUT_PANEL_WINDOW,				/*J 分離型テンキーパネルのコントローラ操作部ウィンドウ */
	CELL_OSKDIALOG_TYPE_SEPARATE_FULL_KEYBOARD_INPUT_PANEL_WINDOW,	/*J 分離型フルキーボードパネルのコントローラ操作部ウィンドウ */
	CELL_OSKDIALOG_TYPE_SEPARATE_CANDIDATE_WINDOW,					/*J 分離型のキーボード入力時の候補表示ウィンドウ */
} CellOskDialogType;

/*J OSK ユーティリティ起動パラメータ */
typedef struct {
	/*J OSKの基本定義 */
	unsigned int allowOskPanelFlg;		/*J 使用するパネルをフラグで選択 (英数入力、ひらがな入力など) */ 
	unsigned int firstViewPanel;		/*J 最初に表示するパネルを選択 */ 
	CellOskDialogPoint controlPoint;	/*J OSKの初期表示位置 */
	int prohibitFlgs;					/*J 禁止条項フラグ */
} CellOskDialogParam;

/*J OSK分離ウィンドウ設定構造体 */
typedef struct {
	CellOskDialogContinuousMode continuousMode;			/*J 続行モード */
	int deviceMask;										/*J ダイアログでイベントを取得しないデバイス */
	int inputFieldWindowWidth;							/*J 入力フィールドウィンドウの幅 */
	float inputFieldBackgroundTrans;					/*J 入力フィールドウィンドウの背景の透過率 */
	CellOskDialogLayoutInfo *inputFieldLayoutInfo;		/*J 入力フィールドウィンドウのレイアウト情報（NULL不可） */
	CellOskDialogLayoutInfo *inputPanelLayoutInfo;		/*J パネルウィンドウのレイアウト情報（NULL可） */
	void *reserved;										/*J 予約領域（NULL） */
} CellOskDialogSeparateWindowOption;

int cellOskDialogLoadAsync( sys_memory_container_t mem_container, const CellOskDialogParam* dialogParam, const CellOskDialogInputFieldInfo* inputFieldInfo );
int cellOskDialogUnloadAsync(CellOskDialogCallbackReturnParam* OutputInfo);
int cellOskDialogAbort(void);
int cellOskDialogGetSize( uint16_t *width, uint16_t *height, CellOskDialogType dialogType);
int cellOskDialogSetLayoutMode( int32_t layoutMode);
int cellOskDialogAddSupportLanguage( uint32_t supportLanguage);
int cellOskDialogSetKeyLayoutOption( uint32_t option);
int cellOskDialogDisableDimmer(void);
int cellOskDialogSetInitialKeyLayout( CellOskDialogInitialKeyLayout initialKeyLayout);
int cellOskDialogSetInitialInputDevice( CellOskDialogInputDevice inputDevice );
int cellOskDialogSetSeparateWindowOption( CellOskDialogSeparateWindowOption* windowOption );
int cellOskDialogSetDeviceMask( uint32_t deviceMask );
int cellOskDialogGetInputText( CellOskDialogCallbackReturnParam* OutputInfo );

#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
}
#endif	/* defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus) */

#endif /*_CELL_SYSUTIL_OSKDIALOG_H_*/
