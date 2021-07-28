/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2007 Sony Computer Entertainment Inc.
* All Rights Reserved.
*/

#ifndef _CELL_SYSUTIL_MSGDIALOG_H_
#define _CELL_SYSUTIL_MSGDIALOG_H_

#include "sysutil_common.h"

#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
extern "C" {
#endif	/* defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus) */


#define CELL_MSGDIALOG_STRING_SIZE				(512)		/*J メッセージストリングのサイズ(byte) */
															/*E Maximum size of specified messages(byte) */
#define CELL_MSGDIALOG_PROGRESSBAR_STRING_SIZE	(64)		/*J プログレスバー付加メッセージストリングのサイズ(byte) */
															/*E Size of the string to display in the progress bar(byte) */
/*J
 * ダイアログの機能
 *
 *  31                   16 15   14 13     12 11   10 9    8    7 6       4       3    2    1    0
 * +-----------------------+-------+---------+-------+------+----+---------+-------+----+----+----+
 * |    R e s e r v e d    | Rsrvd | PrgType | Rsrvd | DCId | DC | BtnType | Rsrvd | BG | SM | ST |
 * +-----------------------+-------+---------+-------+------+----+---------+-------+----+----+----+
 *
 *  bit 31-16: Reserved
 *      0x0000= 常にゼロ固定
 *
 *  bit 15,14: Reserved
 *      00= 常にゼロ固定
 *
 *  bit 13,12: プログレスバーの種別(Progress bar Type)
 *      0= プログレスバーなし
 *      1= プログレスバー1本
 *      2= プログレスバー2本
 *
 *  bit 11,10: Reserved
 *      00= 常にゼロ固定
 *
 *  bit 9,8: デフォルトカーソル位置(Default Cursor Index)
 *      もっとも左に位置するボタンからのインデックス値
 *      ボタンなしの場合、0
 *      Yes/No ボタンの場合、0= Yes ボタン 1= No ボタン
 *      OK ボタンの場合、0= OK ボタン
 *         :
 *
 *  bit 7: 戻るボタンの無効化(Disable Cancel button)
 *      0= 戻るボタンを無効化しない
 *      1= 戻るボタンを無効化する
 *
 *  bit 6-4: ボタンの種別(Button Type)
 *      0= ボタンなし
 *      1= Yes/No ボタン
 *      2= OK ボタン
 *
 *  bit 3: Reserved
 *      0= 常にゼロ固定
 *
 *  bit 2: 背景の表示(BG Visible)
 *      0= 背景を表示する
 *      1= 背景を表示しない(透明)
 *
 *  bit 1: 起動音のミュート(SE Mute)
 *      0= ミュートしない
 *      1= ミュートする
 *         "bit 0: 起動音の種別(SE Type)" の指定によらず起動音は鳴らなくなります。
 *
 *  bit 0: 起動音の種別(SE Type)
 *      0= エラー音
 *      1= 通常起動音
 */
/*E
 * Dialog feature
 *
 *  31                   16 15   14 13     12 11   10 9    8    7 6       4       3    2    1    0
 * +-----------------------+-------+---------+-------+------+----+---------+-------+----+----+----+
 * |    R e s e r v e d    | Rsrvd | PrgType | Rsrvd | DCId | DC | BtnType | Rsrvd | BG | SM | ST |
 * +-----------------------+-------+---------+-------+------+----+---------+-------+----+----+----+
 *
 *  bit 31-16: Reserved
 *      0x00= Always fixed to 0
 *
 *  bit 15,14: Reserved
 *      00= Always fixed to 0
 *
 *  bit 13,12: Progress bar Type
 *      0= No progress bar
 *      1= 1 progress bar
 *      2= 2 progress bars
 *
 *  bit 11,10: Reserved
 *      00= Always fixed to 0
 *
 *  bit 9,8: Default cursor position (Default Cursor Index)
 *      Index value from the leftmost button
 *      0 when no button is provided
 *      0= Yes button and 1= No button when Yes/No buttons are provided
 *      0= OK button when OK button is provided
 *         :
 *
 *  bit 7: Disable the cancel button(Disable Cancel button)
 *      0= Do not disable the cancel button
 *      1= Disable the cancel button
 *
 *  bit 6-4: Button type(Button Type)
 *      0= No button is provided
 *      1= Yes/No buttons
 *      2= OK button
 *
 *  bit 3: Reserved
 *      0= Always fixed to 0
 *
 *  bit 2: Background display(BG Visible)
 *      0= Display the background
 *      1= Do not display the background(transparence)
 *
 *  bit 1: Mute the startup sound (SE Mute)
 *      0= Do not mute
 *      1= Mute
 *         The startup sound will be muted regardless of whether "bit 0: Startup sound type(SE Type)" is specified
 *
 *  bit 0: Startup sound type(SE Type)
 *      0= Error sound
 *      1= Normal startup sound
 */
/*J bit 0: 起動音の種別(SE Type) */
/*E bit 0: Startup sound type(SE Type) */
#define CELL_MSGDIALOG_TYPE_SE_TYPE_ERROR			(0<<0)		/*J エラー音 */
																/*E Error sound */
#define CELL_MSGDIALOG_TYPE_SE_TYPE_NORMAL			(1<<0)		/*J 通常起動音 */
																/*E Normal startup sound */

/*J bit 1: 起動音のミュート(SE Mute) */
/*E bit 1: Mute the startup sound (SE Mute) */
#define CELL_MSGDIALOG_TYPE_SE_MUTE_OFF				(0<<1)		/*J ミュートしない */
																/*E Do not mute */
#define CELL_MSGDIALOG_TYPE_SE_MUTE_ON				(1<<1)		/*J ミュートする */
																/*E Mute */

/*J bit 2: 背景の表示(BG Visible) */
/*E bit 2: Background display(BG Visible) */
#define CELL_MSGDIALOG_TYPE_BG_VISIBLE				(0<<2)		/*J 背景を表示する */
																/*E Display the background */
#define CELL_MSGDIALOG_TYPE_BG_INVISIBLE			(1<<2)		/*J 背景を表示しない(透明) */
																/*E Do not display the background(transparence) */

/*J bit 6-4: ボタンの種別(Button Type) */
/*E bit 6-4: Button type (Button Type) */
#define CELL_MSGDIALOG_TYPE_BUTTON_TYPE_NONE		(0<<4)		/*J ボタンなし */
																/*E No button is provided */
#define CELL_MSGDIALOG_TYPE_BUTTON_TYPE_YESNO		(1<<4)		/*J Yes/No ボタン */
																/*E Yes/No buttons */
#define CELL_MSGDIALOG_TYPE_BUTTON_TYPE_OK			(2<<4)		/*J OK ボタン */
																/*E OK button */

/*J bit 7: 戻るボタンの無効化(Disable Cancel button) */
/*E bit 7: Disable the cancel button (Disable Cancel button) */
#define CELL_MSGDIALOG_TYPE_DISABLE_CANCEL_OFF		(0<<7)		/*J 戻るボタンを無効化しない */
																/*E Do not disable the cancel button */
#define CELL_MSGDIALOG_TYPE_DISABLE_CANCEL_ON		(1<<7)		/*J 戻るボタンを無効化する */
																/*E Disable the cancel button */

/*J bit 9,8: デフォルトカーソル位置(Default Cursor Index) */
/*E bit 9,8: Default cursor position (Default Cursor Index) */
#define CELL_MSGDIALOG_TYPE_DEFAULT_CURSOR_NONE		(0<<8)		/*J ボタンなし */
																/*E No button is provided */
#define CELL_MSGDIALOG_TYPE_DEFAULT_CURSOR_YES		(0<<8)		/*J Yes ボタン */
																/*E Yes button */
#define CELL_MSGDIALOG_TYPE_DEFAULT_CURSOR_NO		(1<<8)		/*J No ボタン */
																/*E No button */
#define CELL_MSGDIALOG_TYPE_DEFAULT_CURSOR_OK		(0<<8)		/*J OK ボタン */
																/*E OK button */

/*J bit 13,12: プログレスバーの種別(Progress bar Type) */
/*E bit 13,12: Progress bar Type */
#define CELL_MSGDIALOG_TYPE_PROGRESSBAR_NONE		(0<<12)		/*J プログレスバーなし */
																/*E No progress bar */
#define CELL_MSGDIALOG_TYPE_PROGRESSBAR_SINGLE		(1<<12)		/*J プログレスバー1本 */
																/*E 1 progress bar */
#define CELL_MSGDIALOG_TYPE_PROGRESSBAR_DOUBLE		(2<<12)		/*J プログレスバー2本 */
																/*E 2 progress bars */

/*J ボタン付きダイアログの押されたボタンの結果 */
/*E Pressed button result type */
typedef enum
{
	CELL_MSGDIALOG_BUTTON_NONE    = -1,		/*J ボタンは押されていない(cellMsgDialogCloseによる終了の場合) */
											/*E No button is pressed(when ended by cellMsgDialogClose) */
	CELL_MSGDIALOG_BUTTON_INVALID = 0,		/*J 無効なボタンが押された */
											/*E An invalid button is pressed */
	CELL_MSGDIALOG_BUTTON_OK      = 1,		/*J [OK]を決定ボタンで選択した */
											/*E [OK] was selected for the decision button */
	CELL_MSGDIALOG_BUTTON_YES     = 1,		/*J [Yes]を決定ボタンで選択した */
											/*E [Yes] was selected for the decision button */
	CELL_MSGDIALOG_BUTTON_NO      = 2,		/*J [No]を決定ボタンで選択した */
											/*E [No] was selected for the decision button */
	CELL_MSGDIALOG_BUTTON_ESCAPE  = 3		/*J 戻るボタンが押された */
											/*E The back button was pressed */
} CellMsgDialogButtonType;

/*J 対象とするプログレスバー */
/*E Target progress bar */
typedef enum
{
	CELL_MSGDIALOG_PROGRESSBAR_INDEX_SINGLE       = 0,	/*J プログレスバー1本の時のプログレスバー */
														/*E Single progress bar */
	CELL_MSGDIALOG_PROGRESSBAR_INDEX_DOUBLE_UPPER = 0,	/*J プログレスバー2本の時の上側プログレスバー */
														/*E The upper progress bar when there are 2 */
	CELL_MSGDIALOG_PROGRESSBAR_INDEX_DOUBLE_LOWER = 1	/*J プログレスバー2本の時の下側プログレスバー */
														/*E The lower progress bar when there are 2 */
} CellMsgDialogProgressBarIndex;

/*J エラーコード */
/*E Error code */
#define CELL_MSGDIALOG_ERROR_PARAM					(0x8002b301)	/*J パラメータが不正 */
																	/*E Parameter is invalid */
#define CELL_MSGDIALOG_ERROR_DIALOG_NOT_OPENED		(0x8002b302)	/*J ダイアログが開かれていない */
																	/*E The dialogue is not opened */

/*J メッセージダイアログ結果通知コールバック関数 */
/*E A callback function that notifies the result from the message dialogue */
typedef void (*CellMsgDialogCallback)( int buttonType, void *userData );

/*J メッセージダイアログを文字列指定で開く */
/*E Specify a string to open the message dialogue */
int cellMsgDialogOpen2( unsigned int type,			/*J ダイアログの機能をビットOR演算子で組み合わせて設定します。 */
													/*E The dialogue features are set using the bit OR operators */
						const char *msgString,		/*J 表示する文字列 (UTF-8で指定。NULL終端文字を含む) */
													/*E A string to display(Specify in UTF-8, including the NULL termination character) */
						CellMsgDialogCallback func,	/*J メッセージダイアログ結果通知コールバック関数指定 */
													/*E Specify the call back function that notifies the result from the message dialogue */
						void *userData,				/*J コールバック関数で受け取るユーザデータ */
													/*E User data received by the call back function */
						void *extParam				/*J 拡張用パラメータ */
													/*E Extension parameter */
);

/*J メッセージダイアログをエラー番号指定で開く */
int cellMsgDialogOpenErrorCode( unsigned int errorNum,			/*J エラー番号 */
																/*E Error number */
								CellMsgDialogCallback func,		/*J メッセージダイアログ結果通知コールバック関数指定 */
																/*E Specify the call back function that notifies the result from the message dialogue */
								void *userData,					/*J コールバック関数で受け取るユーザデータ */
																/*E User data received by the call back function */
								void *extParam					/*J 拡張用パラメータ */
																/*E Extension parameter */
);

/*J メッセージダイアログ内のプログレスバーに付加表示される文字列を設定する */
/*E Set a string to display in the progress bar within the message dialogue */
int cellMsgDialogProgressBarSetMsg( unsigned int progressbarIndex,	/*J 対象とするプログレスバー(CellMsgDialogProgressBarIndex) */
																	/*E Target progress bar(CellMsgDialogProgressBarIndex) */
									const char *msgString			/*J プログレスバーに付加表示する文字列 (UTF-8で指定。NULL終端文字を含む) */
																	/*E A string to display in the progress bar(Specify in UTF-8, including the NULL termination character) */
);

/*J メッセージダイアログ内のプログレスバーの進捗率をリセット(0%)する */
/*E Reset the progress bar in the message dialogue to 0% */
int cellMsgDialogProgressBarReset( unsigned int progressbarIndex	/*J 対象とするプログレスバー(CellMsgDialogProgressBarIndex) */
																	/*E Target progress bar(CellMsgDialogProgressBarIndex) */
);

/*J メッセージダイアログ内のプログレスバーの進捗率を加算する */
/*E Increase the progress rate of the progress bar in the message dialogue */
int cellMsgDialogProgressBarInc( unsigned int progressbarIndex,		/*J 対象とするプログレスバー(CellMsgDialogProgressBarIndex) */
																	/*E Target progress bar(CellMsgDialogProgressBarIndex) */
								 unsigned int delta					/*J 加算する進捗率(%) */
								 									/*E Progress rate to increase(%) */
);

/*J メッセージダイアログを強制的に閉じる */
/*E Forcibly close the message dialogue */
int cellMsgDialogAbort( void );

/*J メッセージダイアログを閉じる */
/*E Close the message dialogue */
int cellMsgDialogClose( float delayTime								/*J ダイアログを閉じはじめるまでの遅延時間(msec) */
																	/*E Delay time to close the dialogue(msec) */
);




/*J 以下は互換性のため残してありますが、使用しないでください。 */
/*E The following have been retained for compatibility. Do not use.  */
typedef enum
{
	CELL_MSGDIALOG_DIALOG_TYPE_ERROR	= 0x00000000,	/*J エラーダイアログ */
														/*E Error dialog */
	CELL_MSGDIALOG_DIALOG_TYPE_NORMAL	= 0x00000001,	/*J 通常ダイアログ */
														/*E Normal dialog */

	CELL_MSGDIALOG_BUTTON_TYPE_NONE		= 0x00000000,	/*J [Yes/No]ボタン無し */
														/*E No [Yes/No] buttons */
	CELL_MSGDIALOG_BUTTON_TYPE_YESNO	= 0x00000010,	/*J [Yes/No]ボタン有 */
														/*E With [Yes/No] buttons */

	CELL_MSGDIALOG_DEFAULT_CURSOR_YES	= 0x00000000,	/*J デフォルトカーソル[Yes] */
														/*E Default cursor [Yes] */
	CELL_MSGDIALOG_DEFAULT_CURSOR_NO	= 0x00000100	/*J デフォルトカーソル[No] */
														/*E Default cursor [No] */
} CellMsgDialogType;

int cellMsgDialogOpen( unsigned int type,			/*J 起動タイプ CellMsgDialogTypeの値をビットOR演算子で組み合わせて設定します。 */
													/*E The dialogue features are set using the bit OR operators */
					   const char *msgString,		/*J 表示する文字列 (UTF-8で指定。NULL終端文字を含む) */
													/*E A string to display(Specify in UTF-8, including the NULL termination character) */
					   CellMsgDialogCallback func,	/*J メッセージダイアログ結果通知コールバック関数指定 */
													/*E Specify the call back function that notifies the result from the message dialogue */
					   void *userData,				/*J コールバック関数で受け取るユーザデータ */
													/*E User data received by the call back function */
					   void *extparam				/*J 拡張用パラメータ */
													/*E Extension parameter */
);


#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
}
#endif	/* defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus) */

#endif /*_CELL_SYSUTIL_MSGDIALOG_H_*/
