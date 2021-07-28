/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2009 Sony Computer Entertainment Inc.
* All Rights Reserved.
*/

#ifndef _CELL_SYSUTIL_GAME_EXEC_H_
#define _CELL_SYSUTIL_GAME_EXEC_H_

#include "sysutil_common.h"
#include "sysutil_game_common.h"

#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
extern "C" {
#endif	/* defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus) */

/*J 連携系 ---------------------------------------------------------- */
/*J 起動元ゲームの情報取得 */
/*J @param type [out] 起動元ゲームのタイプ (システムソフトウェアから起動は区別可) */
/*J @param dirName [out] 起動元ゲームのdirName(ディスク起動ゲームの場合はtitleIdをコピー)  */
/*J @param execdata [out] 起動元ゲームが設定した起動時指定データ(NULLで受け取らない) */
/*J @return エラーコード */
int cellGameGetBootGameInfo( unsigned int *type, char *dirName, unsigned int *execdata ) ;

/*J 起動元ゲームへ返すゲーム定義データ設定 */
/*J @param execdata [in] ゲーム終了時に、起動元ゲームへ渡す起動時指定データを設定しておく */
/*J                      注意：設定しない場合は 0 となる */
/*J @return エラーコード */
int cellGameSetExitParam( unsigned int execdata ) ;

/*J Home との連携系 ------------------------------------------------- */
/*J 起動オプションファイルが置かれたディレクトリパスの取得 */
/*J @param commonPath [out]   共通オプションファイルディレクトリパス(サイズは 128bytes) */
/*J @param personalPath [out] 個別オプションファイルディレクトリパス(サイズは 128bytes) */
int cellGameGetHomeLaunchOptionPath( char *commonPath, char *personalPath ) ;

/*J 連係データファイルを保存するディレクトリパスの取得 */
/*J @param exportPath [out]   連係データファイルディレクトリパス(サイズは 128bytes) */
int cellGameGetHomeDataExportPath( char *exportPath ) ;

/*J 連係データファイルを取得するディレクトリパスの取得 */
/*J @param importPath [out]   連係データファイルディレクトリパス(サイズは 128bytes) */
int cellGameGetHomeDataImportPath( char *importPath ) ;

#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
}
#endif	/* defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus) */

#endif /*_CELL_SYSUTIL_GAME_EXEC_H_*/
