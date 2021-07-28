/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2009 Sony Computer Entertainment Inc.
* All Rights Reserved.
*/

#ifndef _CELL_SYSUTIL_GAMECONTENT_H_
#define _CELL_SYSUTIL_GAMECONTENT_H_

#include <cell/cell_fs.h>
#include "sysutil_common.h"
#include "sysutil_game_common.h"

#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
extern "C" {
#endif	/* defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus) */

/*J 起動属性 */
#define CELL_GAME_ATTRIBUTE_PATCH		(1<<0)	/*J パッチ起動 */
#define CELL_GAME_ATTRIBUTE_APP_HOME	(1<<1)	/*J /app_home/PS3_GAMEからの起動 */
#define CELL_GAME_ATTRIBUTE_DEBUG		(1<<2)	/*J デバッグモードでの起動 */
#define CELL_GAME_ATTRIBUTE_XMBBUY		(1<<3)	/*J ゲーム購入ユーティリティからの再起動 */
#define CELL_GAME_ATTRIBUTE_COMMERCE2_BROWSER	(1<<4)	/*J Commerce2Browserからの再起動 */
#define CELL_GAME_ATTRIBUTE_INVITE_MESSAGE		(1<<5)	/*J タイトル招待からの起動 */
#define CELL_GAME_ATTRIBUTE_CUSTOM_DATA_MESSAGE	(1<<6)	/*J データ添付状態での起動 */
#define CELL_GAME_ATTRIBUTE_WEB_BROWSER	(1<<8)			/*J WebBrowserからの再起動 */

/*J テーマファイルインストールの動作オプション */
#define CELL_GAME_THEME_OPTION_NONE		(0)		/*J オプション指定なし */
#define CELL_GAME_THEME_OPTION_APPLY	(1<<0)	/*J インストール後適用する */

/*J コールバック関数終了コード */
#define CELL_GAME_CBRESULT_OK			(0)		/*J データ格納終了 */
#define CELL_GAME_CBRESULT_OK_CANCEL	(1)		/*J テーマインストールをキャンセルして終了 */

typedef enum CellGameSystemParamSize {		/*J PARAM.SFOのパラメータ */
	CELL_GAME_SYSP_LANGUAGE_NUM = 20,		/*J 多国語対応言語数 */
	CELL_GAME_SYSP_TITLE_SIZE 	= 128,		/*J タイトル名最大サイズ */
	CELL_GAME_SYSP_TITLEID_SIZE	= 10,		/*J 品番サイズ */
	CELL_GAME_SYSP_VERSION_SIZE	= 6,		/*J コンテンツバージョンサイズ */
	CELL_GAME_SYSP_PS3_SYSTEM_VER_SIZE = 8,	/*J Runtime Library リリースバージョンサイズ */
	CELL_GAME_SYSP_APP_VER_SIZE	= 6			/*J システムパッチ用バージョンサイズ */
} CellGameSystemParamSize;

enum {									/*J PARAM.SFO情報取得、設定用ID */
	CELL_GAME_PARAMID_TITLE	= 0,		/*J カレントタイトル名 */
	CELL_GAME_PARAMID_TITLE_DEFAULT,	/*J デフォルトタイトル名 */
	CELL_GAME_PARAMID_TITLE_JAPANESE,	/*J 日本語 */
	CELL_GAME_PARAMID_TITLE_ENGLISH,	/*J 英語 */
	CELL_GAME_PARAMID_TITLE_FRENCH,		/*J フランス語 */
	CELL_GAME_PARAMID_TITLE_SPANISH,	/*J スペイン語 */
	CELL_GAME_PARAMID_TITLE_GERMAN,		/*J ドイツ語 */
	CELL_GAME_PARAMID_TITLE_ITALIAN,	/*J イタリア語 */
	CELL_GAME_PARAMID_TITLE_DUTCH,		/*J オランダ語 */
	CELL_GAME_PARAMID_TITLE_PORTUGUESE,	/*J ポルトガル語 */
	CELL_GAME_PARAMID_TITLE_RUSSIAN,	/*J ロシア語 */
	CELL_GAME_PARAMID_TITLE_KOREAN,		/*J 韓国語 */
	CELL_GAME_PARAMID_TITLE_CHINESE_T,	/*J 中国語（繁体字）*/
	CELL_GAME_PARAMID_TITLE_CHINESE_S,	/*J 中国語（簡体字）*/
	CELL_GAME_PARAMID_TITLE_FINNISH,	/*J フィンランド語 */
	CELL_GAME_PARAMID_TITLE_SWEDISH,	/*J スウェーデン語 */
	CELL_GAME_PARAMID_TITLE_DANISH,		/*J デンマーク語 */
	CELL_GAME_PARAMID_TITLE_NORWEGIAN,	/*J ノルウェー語 */
	CELL_GAME_PARAMID_TITLE_POLISH,		/*J ポーランド語 */

	CELL_GAME_PARAMID_TITLE_ID = 100,	/*J 品番*/
	CELL_GAME_PARAMID_VERSION,			/*J コンテンツバージョン */
	CELL_GAME_PARAMID_PARENTAL_LEVEL,	/*J パレンタルロックレベル */
	CELL_GAME_PARAMID_RESOLUTION,		/*J 対応解像度 */
	CELL_GAME_PARAMID_SOUND_FORMAT,		/*J 対応音声形式 */
	CELL_GAME_PARAMID_PS3_SYSTEM_VER,	/*J Runtime Library リリースバージョン */
	CELL_GAME_PARAMID_APP_VER			/*J ゲームアップデート用バージョン */
};

enum {											/*J エラーダイアログのタイプ */
	CELL_GAME_ERRDIALOG_BROKEN_GAMEDATA = 0,	/*J 破損エラー (ゲームデータ用メッセージを表示) */
	CELL_GAME_ERRDIALOG_BROKEN_HDDGAME,			/*J 破損エラー (Hdd起動ゲーム用メッセージを表示) */
	CELL_GAME_ERRDIALOG_NOSPACE,				/*J HDD 空き容量不足エラー */

	CELL_GAME_ERRDIALOG_BROKEN_EXIT_GAMEDATA = 100,	/*J 破損エラー、強制終了付き (ゲームデータ用メッセージを表示) */
	CELL_GAME_ERRDIALOG_BROKEN_EXIT_HDDGAME,		/*J 破損エラー、強制終了付き (Hdd起動ゲーム用メッセージを表示) */
	CELL_GAME_ERRDIALOG_NOSPACE_EXIT				/*J HDD 空き容量不足エラー、強制終了付き */
};

typedef enum CellGameResolution {				/*J 対応描画解像度設定 *//*E Set the supported rendering resolution */
	CELL_GAME_RESOLUTION_1080  = 0x08,			/*J 1080 *//*E 1080 */
	CELL_GAME_RESOLUTION_720   = 0x04,			/*J 720 *//*E 720 */
	CELL_GAME_RESOLUTION_576SQ = 0x20,			/*J 576のスクイーズ *//*E Wide screen of 576 */
	CELL_GAME_RESOLUTION_576   = 0x02,			/*J 576 *//*E 576 */
	CELL_GAME_RESOLUTION_480SQ = 0x10,			/*J 480のスクイーズ *//*E Wide screen of 480 */
	CELL_GAME_RESOLUTION_480   = 0x01			/*J 480 *//*E 480 */
} CellGameResolution;

typedef enum CellGameSoundFormat {				/*J 対応音声形式設定 *//*E Set the supported sound format */
	CELL_GAME_SOUNDFORMAT_71LPCM   = 0x10,		/*J 7.1ch LPCM */
	CELL_GAME_SOUNDFORMAT_51LPCM   = 0x04,		/*J 5.1ch LPCM */
	CELL_GAME_SOUNDFORMAT_51DDENC  = 0x102,		/*J Dolby Digital */
	CELL_GAME_SOUNDFORMAT_51DTSENC = 0x202,		/*J DTS Digital Surround */
	CELL_GAME_SOUNDFORMAT_2LPCM    = 0x01,		/*J 2ch LPCM */
} CellGameSoundFormat;

typedef enum CellGameDiscType {			/*J ディスクタイプ */
	CELL_GAME_DISCTYPE_OTHER = 0,		/*J その他のディスク */
	CELL_GAME_DISCTYPE_PS3,				/*J PS3 ゲームディスク */
	CELL_GAME_DISCTYPE_PS2				/*J PS2 ゲームディスク */
} CellGameDiscType;

typedef struct CellGameContentSize {		/*J ディレクトリサイズ情報 */
	int hddFreeSizeKB;			/*J HDD空き容量(キロバイト単位) */
	int sizeKB;					/*J ディレクトリの現在のサイズ(未確定の場合 -1) */
	int sysSizeKB;				/*J システムファイルのサイズ */
} CellGameContentSize;

typedef struct CellGameSetInitParams {			/*J PARAM.SFOパラメータ初期値設定用 */
	char title[CELL_GAME_SYSP_TITLE_SIZE];		/*J タイトル名：デフォルト */
	char titleId[CELL_GAME_SYSP_TITLEID_SIZE];	/*J 品番 */
	char reserved0[2];
	char version[CELL_GAME_SYSP_VERSION_SIZE];	/*J コンテンツバージョン */
	char reserved1[66];
} CellGameSetInitParams;

#define CELL_GAME_SIZEKB_NOTCALC	(-1)	/*J ディレクトリサイズが計算されていない */
#define CELL_GAME_THEMEINSTALL_BUFSIZE_MIN	(4096)	/*J テーマインストール用バッファの最小サイズ */

/*J コールバック関数 -------------------------------------------------------------------------------- */
/*J テーマデータ設定コールバック関数 */
typedef int (*CellGameThemeInstallCallback)( unsigned int fileOffset, unsigned int readSize, void *buf );

/*J ユーティリティ関数 ------------------------------------------------------------------------------ */
/*J コンテンツへのアクセス準備 */
/*J @param type [in] コンテンツタイプ (ディスク起動ゲーム or Hdd起動ゲーム or ゲームデータ) */
/*J @param dirName [in] ディレクトリ名 (ディスク起動ゲームの場合はNULL)  */
/*J @param size [out] コンテンツサイズ情報。NULLを指定された場合設定しない。 */
/*J @return エラーコード */
int cellGameDataCheck( unsigned int type, const char *dirName, CellGameContentSize *size ) ;

/*J コンテンツへのアクセス準備(起動ゲーム情報取得) */
/*J @param type [out] データのタイプ (ディスク起動ゲーム or Hdd起動ゲーム) */
/*J @param attributes [out] 起動データ情報  */
/*J @param size [out] コンテンツサイズ情報。NULLを指定された場合設定しない。 */
/*J @param dirName [out] ディレクトリ名。 NULLを指定された場合設定しない。ディスク起動ゲームの場合は設定されない。  */
/*J @return エラーコード */
int cellGameBootCheck( unsigned int *type, unsigned int *attributes, CellGameContentSize *size, char *dirName ) ;

/*J コンテンツへのアクセス準備(パッチ情報取得) */
/*J @param size [out] コンテンツサイズ情報。NULLを指定された場合設定しない。 */
/*J @param reserved 予約領域（NULLを設定すること）*/
/*J @return エラーコード */
int cellGamePatchCheck( CellGameContentSize *size, void *reserved ) ;

/*J ゲームデータを作成 */
/*J @param init [in] PARAM.SFOパラメータ初期値設定 */
/*J @param tmp_contentInfoPath [out]  一時コンテンツ情報ディレクトリパス名。NULLを指定された場合設定しない。 */
/*J @param tmp_usrdirPath [out]  一時アプリケーションディレクトリパス名。NULLを指定された場合設定しない。 */
/*J @return エラーコード */
int cellGameCreateGameData( CellGameSetInitParams *init, char *tmp_contentInfoPath, char *tmp_usrdirPath ) ;

/*J PARAM.SFOパラメータ取得（整数型） */
/*J @param id [in] 取得データ名 */
/*J @param value [out] データ */
/*J @return エラーコード */
int cellGameGetParamInt( int id, int *value );

/*J PARAM.SFOパラメータ取得（文字列型） */
/*J @param id [in] 取得データ名 */
/*J @param buf [out] データ格納バッファ */
/*J @param bufsize [in] データ格納バッファサイズ */
/*J @return エラーコード */
int cellGameGetParamString( int id, char *buf, unsigned int bufsize );

/*J コンテンツのディレクトリサイズ取得 */
/*J @param sizeKB [out] ディレクトリサイズ */
int cellGameGetSizeKB( int *sizeKB );

/*J PARAM.SFOパラメータ設定（文字列型） */
/*J @param id [in] 取得データ名 */
/*J @param buf [in] データ格納バッファ */
/*J @return エラーコード */
int cellGameSetParamString( int id, const char *buf );

/*J ディスクのコンテンツ情報更新用ディレクトリパスの取得 */
/*J @param updatePath [out]   コンテンツ情報更新用ディレクトリパス(サイズは 128bytes) */
int cellGameGetDiscContentInfoUpdatePath( char *updatePath ) ;

/*J コンテンツへのアクセス許可 */
/*J @param contentInfoPath [out]  コンテンツ情報ディレクトリパス名。NULLを指定されたらエラー */
/*J @param usrdirPath [out]  アプリケーションディレクトリパス名。NULLを指定されたらエラー */
/*J @return エラーコード */
int cellGameContentPermit( char *contentInfoPath, char *usrdirPath  )  ;

/*J エラーダイアログを表示　Permit後でも利用可能 */
/*J @param type [in] エラータイプ */
/*J @param errNeedSizeKB [in]  空き容量不足エラーの場合の不足サイズ。キロバイト単位 */
/*J @param dirName [in] ディレクトリ名。破損エラーの場合のタイトル名表示に用いる。  */
int cellGameContentErrorDialog( int type, int errNeedSizeKB, const char *dirName ) ;

/*J テーマファイルのインストール　Permit後でも利用可能 */
/*J @param usrdirPath [in] アプリケーションディレクトリパス名 */
/*J @param fileName [in] ファイル名 */
/*J @param option [in]  オプション指定 */
int cellGameThemeInstall(const char *usrdirPath, const char *fileName, unsigned int option );

/*J メモリ上のテーマファイルのインストール　Permit後でも利用可能 */
/*J @param fileSize [in] テーマのファイルサイズ */
/*J @param bufSize [in] バッファサイズ (CELL_GAME_THEMEINSTALL_BUFSIZE_MIN以上にする必要があります) */
/*J @param buf [in] データ格納バッファ */
/*J @param func [in] テーマデータ格納コールバック関数 */
/*J @param option [in]  オプション指定 */
int cellGameThemeInstallFromBuffer(
		unsigned int fileSize, unsigned int bufSize, void *buf, CellGameThemeInstallCallback func, unsigned int option );

/*J ローカルHTMLファイル用ディレクトリパスの取得 */
/*J @param contentPath [out]   ローカルHTMLファイル用ディレクトリパス(サイズは 128bytes) */
int cellGameGetLocalWebContentPath( char *contentPath ) ;

/*J ディスクゲーム系API */
/*J コールバック関数 -------------------------------------------------------------------------------- */
/*J ディスク排出コールバック関数 */
typedef void (*CellGameDiscEjectCallback)( void );

/*J ディスク挿入コールバック関数 */
typedef void (*CellGameDiscInsertCallback)( unsigned int discType, char *titleId );

/*J ディスク入れ換え通知コールバック登録 */
/*J @param funcEject [in] ディスク排出コールバック関数 */
/*J @param funcInsert [in] ディスク挿入コールバック関数 */
int cellGameRegisterDiscChangeCallback( CellGameDiscEjectCallback funcEject, CellGameDiscInsertCallback funcInsert );

/*J ディスク入れ換え通知コールバック解除 */
int cellGameUnregisterDiscChangeCallback( void );


#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
}
#endif	/* defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus) */

#endif /*_CELL_SYSUTIL_GAMECONTENT_H_*/
