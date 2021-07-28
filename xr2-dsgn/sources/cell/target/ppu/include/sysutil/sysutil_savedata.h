/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2007 Sony Computer Entertainment Inc.
* All Rights Reserved.
*/

#ifndef _CELL_SYSUTIL_SAVEDATA_H_
#define _CELL_SYSUTIL_SAVEDATA_H_

#include "sysutil_common.h"
#include <sys/memory.h>

#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
extern "C" {
#endif	/* defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus) */

/*J 正常終了コード */
#define CELL_SAVEDATA_RET_OK				(0)
#define CELL_SAVEDATA_RET_CANCEL			(1)

/*J エラーコード */
#define CELL_SAVEDATA_ERROR_CBRESULT		(0x8002b401)	/*J コールバック関数にエラーを返された */
#define CELL_SAVEDATA_ERROR_ACCESS_ERROR	(0x8002b402)	/*J HDD アクセスエラー */
#define CELL_SAVEDATA_ERROR_INTERNAL		(0x8002b403)	/*J 内部エラー */
#define CELL_SAVEDATA_ERROR_PARAM			(0x8002b404)	/*J パラメータが不正 パラメータが不正 マスター提出時までに対処する必要がある */
#define CELL_SAVEDATA_ERROR_NOSPACE			(0x8002b405)	/*J HDD 空き不足  パラメータが不正 マスター提出時までに対処する必要がある */
#define CELL_SAVEDATA_ERROR_BROKEN			(0x8002b406)	/*J セーブデータを破損判定した */
#define CELL_SAVEDATA_ERROR_FAILURE			(0x8002b407)	/*J セーブ/ロード/削除処理失敗 */
#define CELL_SAVEDATA_ERROR_BUSY			(0x8002b408)	/*J セーブデータユーティリティのAPIを同時に呼び出した */
#define CELL_SAVEDATA_ERROR_NOUSER			(0x8002b409)	/*J 指定されたユーザが存在しない */
#define CELL_SAVEDATA_ERROR_SIZEOVER		(0x8002b40a)	/*J セーブデータのサイズが最大値を超えた */
#define CELL_SAVEDATA_ERROR_NODATA			(0x8002b40b)	/*J 指定されたセーブデータが見つからない */
#define CELL_SAVEDATA_ERROR_NOTSUPPORTED	(0x8002b40c)	/*J サポートされていない処理 */


typedef enum {									/*J セーブデータの形式バージョン */
	CELL_SAVEDATA_VERSION_CURRENT = 0,			/*J 最新の形式 */
} CellSaveDataVersion;

typedef enum {									/*J 各種サイズ */
	CELL_SAVEDATA_DIRNAME_SIZE			= 32,	/*J ディレクトリ名最大サイズ */
	CELL_SAVEDATA_FILENAME_SIZE			= 13,	/*J ファイル名最大サイズ */
	CELL_SAVEDATA_SECUREFILEID_SIZE		= 16,	/*J 保護ありデータファイルの暗号IDサイズ */
	CELL_SAVEDATA_PREFIX_SIZE			= 256,	/*J 複数指定プレフィクス最大サイズ */

	CELL_SAVEDATA_LISTITEM_MAX			= 2048,	/*J リスト選択型のリスト表示項目最大数 */
	CELL_SAVEDATA_SECUREFILE_MAX		= 113,	/*J 保護ありデータファイルの最大数 */
	CELL_SAVEDATA_DIRLIST_MAX			= 2048,	/*J ディレクトリリスト取得時のリスト項目最大数 */
	CELL_SAVEDATA_INVALIDMSG_MAX		= 256,	/*J CELL_SAVEDATA_CBRESULT_ERR_INVALID時のメッセージ最大サイズ */
	CELL_SAVEDATA_INDICATORMSG_MAX		= 64	/*J オートセーブ/ロード用インジケータ文章最大サイズ */
} CellSaveDataParamSize;

typedef enum {									/*J PARAM.SFOのパラメータ */
	CELL_SAVEDATA_SYSP_TITLE_SIZE 		= 128,	/*J タイトル名最大サイズ */
	CELL_SAVEDATA_SYSP_SUBTITLE_SIZE	= 128,	/*J サブタイトル名最大サイズ */
	CELL_SAVEDATA_SYSP_DETAIL_SIZE		= 1024,	/*J 詳細情報最大サイズ */
	CELL_SAVEDATA_SYSP_LPARAM_SIZE		= 8		/*J データパラメータサイズ */
} CellSaveDataSystemParamSize;

typedef enum {									/*J ソート種別 */
	CELL_SAVEDATA_SORTTYPE_MODIFIEDTIME = 0,	/*J 更新日付順 */
	CELL_SAVEDATA_SORTTYPE_SUBTITLE				/*J サブタイトル名順 */
} CellSaveDataSortType;

typedef enum {									/*J ソート順 */
	CELL_SAVEDATA_SORTORDER_DESCENT = 0,		/*J 降順(新しいものが上) */
	CELL_SAVEDATA_SORTORDER_ASCENT				/*J 昇順 */
} CellSaveDataSortOrder;

typedef enum {									/*J アイコンの位置 */
	CELL_SAVEDATA_ICONPOS_HEAD = 0,				/*J 先頭 */
	CELL_SAVEDATA_ICONPOS_TAIL					/*J 末尾 */
} CellSaveDataIconPosition;

typedef enum {									/*J リスト選択型の初期フォーカス位置 */
	CELL_SAVEDATA_FOCUSPOS_DIRNAME = 0,			/*J ディレクトリ名指定 */
	CELL_SAVEDATA_FOCUSPOS_LISTHEAD,			/*J リスト先頭のアイコン */
	CELL_SAVEDATA_FOCUSPOS_LISTTAIL,			/*J リスト末尾のアイコン */
	CELL_SAVEDATA_FOCUSPOS_LATEST,				/*J 更新日時が最新データのアイコン */
	CELL_SAVEDATA_FOCUSPOS_OLDEST,				/*J 更新日時が最古データのアイコン */
	CELL_SAVEDATA_FOCUSPOS_NEWDATA				/*J 新規セーブデータのアイコン */
} CellSaveDataFocusPosition;

typedef enum {									/*J 新規セーブデータか既存のデータかを示すフラグ用 */
	CELL_SAVEDATA_ISNEWDATA_NO = 0,				/*J 既存のデータ */
	CELL_SAVEDATA_ISNEWDATA_YES					/*J 新規セーブデータ(ディレクトリが存在しない) */
} CellSaveDataIsNewData;

typedef enum {									/*J オートタイプのエラーダイアログ出力設定 */
	CELL_SAVEDATA_ERRDIALOG_NONE = 0,			/*J 一切出力しない */
	CELL_SAVEDATA_ERRDIALOG_ALWAYS,				/*J 常に出力する */
	CELL_SAVEDATA_ERRDIALOG_NOREPEAT			/*J 同じエラーは連続出力しない */
} CellSaveDataErrDialog;

												/*J 操作条件 */
#define CELL_SAVEDATA_ATTR_NORMAL		(0)		/*J なし */
#define CELL_SAVEDATA_ATTR_NODUPLICATE	(1<<0)	/*J コピー禁止 */

														/*J バインド情報 */
#define CELL_SAVEDATA_BINDSTAT_OK				(0)		/*J 全てOK */
#define CELL_SAVEDATA_BINDSTAT_ERR_CONSOLE		(1<<0)	/*J 本体バインドエラー */
#define CELL_SAVEDATA_BINDSTAT_ERR_DISC			(1<<1)	/*J ディスクバインドエラー */
#define CELL_SAVEDATA_BINDSTAT_ERR_PROGRAM		(1<<2)	/*J プログラムバインドエラー */
#define CELL_SAVEDATA_BINDSTAT_ERR_NOACCOUNTID	(1<<3)	/*J アカウントIDを持たないユーザのセーブデータ */
#define CELL_SAVEDATA_BINDSTAT_ERR_NOUSER		(1<<3)	/*J 互換用に残しています */
#define CELL_SAVEDATA_BINDSTAT_ERR_ACCOUNTID	(1<<4)	/*J 他のアカウントIDを持つユーザのセーブデータ */
#define CELL_SAVEDATA_BINDSTAT_ERR_OTHERS		(1<<4)	/*J 互換用に残しています */
#define CELL_SAVEDATA_BINDSTAT_ERR_NOUSERID		(1<<5)	/*J ユーザ情報の無いセーブデータ */
#define CELL_SAVEDATA_BINDSTAT_ERR_USERID		(1<<6)	/*J 他のユーザのセーブデータ */
#define CELL_SAVEDATA_BINDSTAT_ERR_NOOWNER		(1<<8)	/*J 所有者が不明のセーブデータ */
#define CELL_SAVEDATA_BINDSTAT_ERR_OWNER		(1<<9)	/*J 他の所有者のセーブデータ */
#define CELL_SAVEDATA_BINDSTAT_ERR_LOCALOWNER	(1<<10)	/*J アカウントIDを持っていない所有者に保存された */

typedef enum {									/*J セーブデータをいったん削除するか指定用 */
	CELL_SAVEDATA_RECREATE_NO = 0,				/*J そのまま使用：破損化あり */
	CELL_SAVEDATA_RECREATE_NO_NOBROKEN,			/*J そのまま使用：破損化なし */
	CELL_SAVEDATA_RECREATE_YES,					/*J いったん削除し、所有者情報は引き継ぐ */
	CELL_SAVEDATA_RECREATE_YES_RESET_OWNER,		/*J いったん削除し、所有者情報を再設定 */
} CellSaveDataReCreateMode;

#define CELL_SAVEDATA_DISABLE_TROPHY_OWNERSHIP_CHECK	(1<<16)

typedef enum {									/*J ファイル種別 */
	CELL_SAVEDATA_FILETYPE_SECUREFILE = 0,		/*J 保護ありデータファイル */
	CELL_SAVEDATA_FILETYPE_NORMALFILE,			/*J 保護なしデータファイル */
	CELL_SAVEDATA_FILETYPE_CONTENT_ICON0,		/*J コンテンツ情報：ICON0.PNG */
	CELL_SAVEDATA_FILETYPE_CONTENT_ICON1,		/*J コンテンツ情報：ICON1.PAM */
	CELL_SAVEDATA_FILETYPE_CONTENT_PIC1,		/*J コンテンツ情報：PIC1.PNG */
	CELL_SAVEDATA_FILETYPE_CONTENT_SND0			/*J コンテンツ情報：SND0.AT3 */
} CellSaveDataFileType;

typedef enum {									/*J ファイル操作 */
	CELL_SAVEDATA_FILEOP_READ = 0,				/*J 読み込み */
	CELL_SAVEDATA_FILEOP_WRITE,					/*J 書き込み */
	CELL_SAVEDATA_FILEOP_DELETE,				/*J 削除 */
	CELL_SAVEDATA_FILEOP_WRITE_NOTRUNC			/*J	ファイルを切り詰めない書き込み */
} CellSaveDataFileOperation;

												/*J 動作オプション */
#define CELL_SAVEDATA_OPTION_NONE		(0)		/*J オプション指定なし */
#define CELL_SAVEDATA_OPTION_NOCONFIRM	(1<<0)	/*J ユーザに確認なしで処理を開始する */

typedef enum {									/*J オートセーブ/ロード用インジケータ表示位置 */
	CELL_SAVEDATA_INDICATORPOS_LOWER_RIGHT = 0,	/*J 右下 */
	CELL_SAVEDATA_INDICATORPOS_LOWER_LEFT,		/*J 左下 */
	CELL_SAVEDATA_INDICATORPOS_UPPER_RIGHT,		/*J 右上 */
	CELL_SAVEDATA_INDICATORPOS_UPPER_LEFT,		/*J 左上 */
	CELL_SAVEDATA_INDICATORPOS_CENTER			/*J 中央 */
} CellSaveDataAutoIndicatorDispPosition;

typedef enum {											/*J オートセーブ/ロード用インジケータメッセージ揃え位置 */
	CELL_SAVEDATA_INDICATORPOS_MSGALIGN_LEFT	= 0<<4,	/*J 左揃え */
	CELL_SAVEDATA_INDICATORPOS_MSGALIGN_RIGHT	= 1<<4,	/*J 右揃え */
	CELL_SAVEDATA_INDICATORPOS_MSGALIGN_CENTER	= 2<<4	/*J 中央揃え */
} CellSaveDataAutoIndicatorDispMsgAlign;

typedef enum {									/*J オートセーブ/ロード用インジケータ表示モード */
	CELL_SAVEDATA_INDICATORMODE_FIXED = 0,		/*J 点滅無し */
	CELL_SAVEDATA_INDICATORMODE_BLINK,			/*J 点滅 */
	CELL_SAVEDATA_INDICATORMODE_BLINK_FAST,		/*J 速い点滅 */
	CELL_SAVEDATA_INDICATORMODE_BLINK_SLOW,		/*J 遅い点滅 */
} CellSaveDataAutoIndicatorDispMode;

/*J 構造体：引数向け -------------------------------------------------------------------------------- */
typedef struct {								/*J セーブデータリスト指定用 */
	unsigned int sortType;						/*J ソート種別 */
	unsigned int sortOrder;						/*J ソート順 */
	char *dirNamePrefix;						/*J ディレクトリ名prefix指定 */
	void *reserved;
} CellSaveDataSetList;

typedef struct {								/*J バッファ指定用 */
	unsigned int dirListMax;					/*J ディレクトリリスト最大取得数(CELL_SAVEDATA_DIRLIST_MAX以下) */
	unsigned int fileListMax;					/*J ファイルリスト最大取得数(特に制限なし) */
	unsigned int reserved[6];

	unsigned int bufSize;						/*J 汎用バッファサイズ */
	void *buf;									/*J 汎用バッファ */
} CellSaveDataSetBuf;

typedef struct {		  						/*J 新規セーブデータアイコン指定 */
	char *title;								/*J タイトル名 */
	unsigned int iconBufSize;					/*J 静止画アイコンサイズ */
	void *iconBuf;								/*J 静止画アイコンバッファ */
	void *reserved;
} CellSaveDataNewDataIcon;

typedef struct {								/*J オートセーブ/ロード用インジケータ指定 */
	unsigned int dispPosition;					/*J 表示位置 */
	unsigned int dispMode;						/*J 表示モード */
	char *dispMsg;								/*J 表示文章 */
	unsigned int picBufSize;					/*J 画像サイズ */
	void *picBuf;								/*J 画像バッファ */
	void *reserved;
} CellSaveDataAutoIndicator;

/*J 構造体：コールバック関数向け -------------------------------------------------------------------- */

typedef struct {								/*J ディレクトリリスト取得設定用 */
	char dirName[CELL_SAVEDATA_DIRNAME_SIZE];	/*J ディレクトリ名 */
	char listParam[CELL_SAVEDATA_SYSP_LPARAM_SIZE];	/*J データパラメータ(CellSaveDataListSetでは不使用) */
	char reserved[8];
} CellSaveDataDirList;

typedef struct {								/*J リスト用新規セーブデータ指定 */
	unsigned int iconPosition;					/*J アイコン位置指定 */
	char *dirName;								/*J 作成される場合のディレクトリ名 */

	CellSaveDataNewDataIcon *icon;				/*J アイコンデータ指定*/
	void *reserved;
} CellSaveDataListNewData;

typedef struct {								/*J データリストコールバック取得用 */
	unsigned int dirNum;						/*J 検索条件を満たすディレクトリの総数 */
	unsigned int dirListNum;					/*J リストに取得した項目数 */
	CellSaveDataDirList *dirList;				/*J ディレクトリリスト(CellSaveDataSetBuf *bufを使用) */
	char reserved[64];
} CellSaveDataListGet;

typedef struct {								/*J データリストコールバック設定用 */
	unsigned int focusPosition;					/*J フォーカス位置指定 */
	char *focusDirName;							/*J フォーカス位置指定がディレクトリ名指定の場合に使用 */
	unsigned int fixedListNum;					/*J 画面表示するリスト項目数 */
	CellSaveDataDirList *fixedList;				/*J 画面表示するリスト指定 */
	CellSaveDataListNewData *newData;			/*J 新規セーブデータ指定。NULLで指定なし */
	void *reserved;
} CellSaveDataListSet;

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
typedef struct {								/*J 固定セーブ／ロード データ指定コールバック設定用 */
	char *dirName;								/*J セーブデータのディレクトリ名 */

	CellSaveDataNewDataIcon *newIcon;			/*J 新規セーブデータの場合のアイコンデータ指定 */
	unsigned int option;						/*J 動作オプションフラグ */
} CellSaveDataFixedSet;

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

typedef struct {								/*J PARAM.SFO パラメータ取得設定用 */
	char title[CELL_SAVEDATA_SYSP_TITLE_SIZE];
	char subTitle[CELL_SAVEDATA_SYSP_SUBTITLE_SIZE];
	char detail[CELL_SAVEDATA_SYSP_DETAIL_SIZE];
	unsigned int attribute;						/*J 操作条件 */
	char reserved2[4];
	char listParam[CELL_SAVEDATA_SYSP_LPARAM_SIZE];	/*J リストパラメータ */
	char reserved[256];
} CellSaveDataSystemFileParam;

typedef struct {								/*J ディレクトリステータス */
	time_t st_atime;							/*J アクセス日時 */
	time_t st_mtime;							/*J 更新日時 */
	time_t st_ctime;							/*J 作成日時 */
	char dirName[CELL_SAVEDATA_DIRNAME_SIZE];	/*J ディレクトリ名 */
} CellSaveDataDirStat;

typedef struct {								/*J ファイルステータス */
	unsigned int fileType;						/*J ファイル種別 */
	char reserved1[4];
	uint64_t st_size;							/*J ファイルサイズ */
	time_t st_atime;							/*J アクセス日時 */
	time_t st_mtime;							/*J 更新日時 */
	time_t st_ctime;							/*J 作成日時 */
	char fileName[CELL_SAVEDATA_FILENAME_SIZE];	/*J ファイル名 */
	char reserved2[3];
} CellSaveDataFileStat;

typedef struct {								/*J データステータスコールバック取得用 */
	int hddFreeSizeKB;							/*J HDD空き容量(キロバイト単位) */

	unsigned int isNewData;						/*J 新規セーブデータか既存のデータかを示すフラグ */
	CellSaveDataDirStat dir;					/*J ユーザに選択されたデータ(新規セーブデータの場合はディレクトリ名のみ有効) */
	CellSaveDataSystemFileParam getParam;		/*J PARAM.SFO情報(新規セーブデータの場合は構造体すべてが0クリアされている) */
	unsigned int bind;							/*J バインド情報 */

	int sizeKB;									/*J セーブデータの現在のサイズ(新規セーブデータの場合は0) */
	int sysSizeKB;								/*J システムファイルのサイズ */

	unsigned int fileNum;						/*J すべてのファイル数(システムファイル除く) */
	unsigned int fileListNum;					/*J リストに取得した項目数(システムファイル除く) */
	CellSaveDataFileStat *fileList;				/*J ファイルリスト(CellSaveDataSetBuf *bufを使用) */
	char reserved[64];
} CellSaveDataStatGet;

typedef struct {								/*J データステータスコールバック設定用 */
	CellSaveDataSystemFileParam *setParam;		/*J PARAM.SFO情報設定。&getParamを代入可能。NULLの場合は更新しない */
	unsigned int reCreateMode;					/*J セーブデータをいったん削除するかどうか */
	CellSaveDataAutoIndicator *indicator;		/*J インジケータ指定 */
} CellSaveDataStatSet;

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

typedef struct {								/*J ファイル操作コールバック取得用 */
	unsigned int excSize;						/*J 直前のファイル操作コールバックでバッファに読み書きしたサイズ */
	char reserved[64];
} CellSaveDataFileGet;

typedef struct {								/*J ファイル操作コールバック設定用 */
	unsigned int fileOperation;					/*J ファイル操作を指定 */
	void *reserved;

	unsigned int fileType;						/*J ファイル種別指定 */
	unsigned char secureFileId[CELL_SAVEDATA_SECUREFILEID_SIZE];
												/*J 暗号ID。保護ありデータファイルの場合のみ */
	char *fileName;								/*J ファイル名。保護あり/保護なしデータファイルの場合のみ */

	unsigned int fileOffset;					/*J ファイルを読み書きする先頭からのoffset */
	unsigned int fileSize;						/*J ファイルを読み書きするサイズ */
	unsigned int fileBufSize;					/*J ファイルバッファサイズ */
	void *fileBuf;								/*J ファイルバッファポインタ */
} CellSaveDataFileSet;

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

typedef struct {								/*J 処理完了コールバック取得用 */
	int excResult;								/*J 名前 処理の結果 */
	char dirName[CELL_SAVEDATA_DIRNAME_SIZE];	/*J 選択されたセーブデータのディレクトリ名 */
	int sizeKB;									/*J 正常終了した場合のセーブデータのサイズ（HDD上でのサイズ）*/
	int hddFreeSizeKB;							/*J 現在のHDD空き容量 */
	char reserved[64];
} CellSaveDataDoneGet;

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
/*J コールバック関数終了コード */
#define CELL_SAVEDATA_CBRESULT_OK_LAST_NOCONFIRM	(2)		/*J 正常終了：次のコールバック呼び出しは不要。確認ダイアログ無しのユーティリティ終了処理 */
#define CELL_SAVEDATA_CBRESULT_OK_LAST				(1)		/*J 正常終了：次のコールバック呼び出しは不要。ユーティリティ終了処理 */
#define CELL_SAVEDATA_CBRESULT_OK_NEXT				(0)		/*J 正常終了：次のコールバック呼び出しを要求 */
#define CELL_SAVEDATA_CBRESULT_ERR_NOSPACE			(-1)	/*J 空き容量不足 */
#define CELL_SAVEDATA_CBRESULT_ERR_FAILURE			(-2)	/*J セーブ/ロード処理失敗 */
#define CELL_SAVEDATA_CBRESULT_ERR_BROKEN			(-3)	/*J セーブデータ破損判定 */
#define CELL_SAVEDATA_CBRESULT_ERR_NODATA			(-4)	/*J データがない */
#define CELL_SAVEDATA_CBRESULT_ERR_INVALID			(-5)	/*J 汎用エラー(バインドNGなど) */

typedef struct {								/*J コールバック関数の結果指定 */
	int result;									/*J コールバック関数終了コード指定 */

	unsigned int progressBarInc;				/*J プログレスバー用増分パラメータ */

	int errNeedSizeKB;							/*J 空き容量不足エラーの場合の不足サイズ。キロバイト単位 */
	char *invalidMsg;							/*J 汎用エラーの場合のメッセージ */
	void *userdata;								/*J ユーザ指定データ */
} CellSaveDataCBResult;

/*J コールバック関数 -------------------------------------------------------------------------------- */
/*J データリストコールバック関数 */
typedef void (*CellSaveDataListCallback)( CellSaveDataCBResult *cbResult,
										  CellSaveDataListGet *get,
										  CellSaveDataListSet *set );

/*J データ指定コールバック関数 */
typedef void (*CellSaveDataFixedCallback)( CellSaveDataCBResult *cbResult,
										   CellSaveDataListGet *get,
										   CellSaveDataFixedSet *set );

/*J データステータスコールバック関数 */
typedef void (*CellSaveDataStatCallback)( CellSaveDataCBResult *cbResult,
										  CellSaveDataStatGet *get,
										  CellSaveDataStatSet *set );

/*J ファイル操作コールバック関数 */
typedef void (*CellSaveDataFileCallback)( CellSaveDataCBResult *cbResult,
										  CellSaveDataFileGet *get,
										  CellSaveDataFileSet *set );

/*J 処理完了コールバック関数 */
typedef void (*CellSaveDataDoneCallback)( CellSaveDataCBResult *cbResult,
										  CellSaveDataDoneGet *get );

/*J ユーティリティ関数 ------------------------------------------------------------------------------ */
/*J リスト選択型セーブ2 */
int cellSaveDataListSave2( unsigned int version,
						CellSaveDataSetList *setList,
						CellSaveDataSetBuf *setBuf,
						CellSaveDataListCallback funcList,
						CellSaveDataStatCallback funcStat,
						CellSaveDataFileCallback funcFile,
						sys_memory_container_t container,
						void *userdata );

/*J リスト選択型ロード2 */
int cellSaveDataListLoad2( unsigned int version,
						CellSaveDataSetList *setList,
						CellSaveDataSetBuf *setBuf,
						CellSaveDataListCallback funcList,
						CellSaveDataStatCallback funcStat,
						CellSaveDataFileCallback funcFile,
						sys_memory_container_t container,
						void *userdata );

/*J 固定型セーブ2 */
int cellSaveDataFixedSave2( unsigned int version,
						CellSaveDataSetList *setList,
						CellSaveDataSetBuf *setBuf,
						CellSaveDataFixedCallback funcFixed,
						CellSaveDataStatCallback funcStat,
						CellSaveDataFileCallback funcFile,
						sys_memory_container_t container,
						void *userdata );

/*J 固定型ロード2 */
int cellSaveDataFixedLoad2( unsigned int version,
						CellSaveDataSetList *setList,
						CellSaveDataSetBuf *setBuf,
						CellSaveDataFixedCallback funcFixed,
						CellSaveDataStatCallback funcStat,
						CellSaveDataFileCallback funcFile,
						sys_memory_container_t container,
						void *userdata );

/*J オートセーブ2 */
int cellSaveDataAutoSave2( unsigned int version,
						const char *dirName,
						unsigned int errDialog,
						CellSaveDataSetBuf *setBuf,
						CellSaveDataStatCallback funcStat,
						CellSaveDataFileCallback funcFile,
						sys_memory_container_t container,
						void *userdata );

/*J オートロード2 */
int cellSaveDataAutoLoad2( unsigned int version,
						const char *dirName,
						unsigned int errDialog,
						CellSaveDataSetBuf *setBuf,
						CellSaveDataStatCallback funcStat,
						CellSaveDataFileCallback funcFile,
						sys_memory_container_t container,
						void *userdata );

/*J リスト取得型オートセーブ */
int cellSaveDataListAutoSave( unsigned int version,
						unsigned int errDialog,
						CellSaveDataSetList *setList,
						CellSaveDataSetBuf *setBuf,
						CellSaveDataFixedCallback funcFixed,
						CellSaveDataStatCallback funcStat,
						CellSaveDataFileCallback funcFile,
						sys_memory_container_t container,
						void *userdata );

/*J リスト取得型オートロード */
int cellSaveDataListAutoLoad( unsigned int version,
						unsigned int errDialog,
						CellSaveDataSetList *setList,
						CellSaveDataSetBuf *setBuf,
						CellSaveDataFixedCallback funcFixed,
						CellSaveDataStatCallback funcStat,
						CellSaveDataFileCallback funcFile,
						sys_memory_container_t container,
						void *userdata );

/*J 全セーブデータ対象リスト選択型削除2 */
int cellSaveDataDelete2( sys_memory_container_t container );

/*J リスト選択型セーブデータ取り込み（同期関数）*/
int cellSaveDataListImport( CellSaveDataSetList *setList,
						unsigned int maxSizeKB,
						CellSaveDataDoneCallback funcDone,
						sys_memory_container_t container,
						void *userdata );

/*J リスト選択型セーブデータ書き出し（同期関数）*/
int cellSaveDataListExport(	CellSaveDataSetList *setList,
						unsigned int maxSizeKB,
						CellSaveDataDoneCallback funcDone,
						sys_memory_container_t container,
						void *userdata );

/*J 固定型セーブデータ取り込み（同期関数）*/
int cellSaveDataFixedImport( const char *dirName,
						unsigned int maxSizeKB,
						CellSaveDataDoneCallback funcDone,
						sys_memory_container_t container,
						void *userdata );

/*J 固定型セーブデータ書き出し（同期関数）*/
int cellSaveDataFixedExport( const char *dirName,
						unsigned int maxSizeKB,
						CellSaveDataDoneCallback funcDone,
						sys_memory_container_t container,
						void *userdata );

/*J リスト選択型削除 */
int cellSaveDataListDelete(	CellSaveDataSetList *setList,
						CellSaveDataSetBuf *setBuf,
						CellSaveDataListCallback funcList,
						CellSaveDataDoneCallback funcDone,
						sys_memory_container_t container,
						void *userdata );

/*J 固定型削除 */
int cellSaveDataFixedDelete( CellSaveDataSetList *setList,
						CellSaveDataSetBuf *setBuf,
						CellSaveDataFixedCallback funcFixed,
						CellSaveDataDoneCallback funcDone,
						sys_memory_container_t container,
						void *userdata );

/*J リストアイテム情報取得 */
int cellSaveDataGetListItem( const char *dirName,
						CellSaveDataDirStat *dir,
						CellSaveDataSystemFileParam *sysFileParam,
						unsigned int *bind,
						int *sizeKB );

/*J ユーザ指定型API ------------------------------------------*/

/*J ユーザ指定リスト選択型セーブ */
int cellSaveDataUserListSave( unsigned int version,
							CellSysutilUserId userId,
							CellSaveDataSetList *setList,
							CellSaveDataSetBuf *setBuf,
							CellSaveDataListCallback funcList,
							CellSaveDataStatCallback funcStat,
							CellSaveDataFileCallback funcFile,
							sys_memory_container_t container,
							void *userdata );

/*J ユーザ指定リスト選択型ロード */
int cellSaveDataUserListLoad( unsigned int version,
							CellSysutilUserId userId,
							CellSaveDataSetList *setList,
							CellSaveDataSetBuf *setBuf,
							CellSaveDataListCallback funcList,
							CellSaveDataStatCallback funcStat,
							CellSaveDataFileCallback funcFile,
							sys_memory_container_t container,
							void *userdata );

/*J ユーザ指定固定型セーブ */
int cellSaveDataUserFixedSave( unsigned int version,
							CellSysutilUserId userId,
							CellSaveDataSetList *setList,
							CellSaveDataSetBuf *setBuf,
							CellSaveDataFixedCallback funcFixed,
							CellSaveDataStatCallback funcStat,
							CellSaveDataFileCallback funcFile,
							sys_memory_container_t container,
							void *userdata );

/*J ユーザ指定固定型ロード */
int cellSaveDataUserFixedLoad( unsigned int version,
							CellSysutilUserId userId,
							CellSaveDataSetList *setList,
							CellSaveDataSetBuf *setBuf,
							CellSaveDataFixedCallback funcFixed,
							CellSaveDataStatCallback funcStat,
							CellSaveDataFileCallback funcFile,
							sys_memory_container_t container,
							void *userdata );

/*J ユーザ指定オートセーブ */
int cellSaveDataUserAutoSave( unsigned int version,
							CellSysutilUserId userId,
							const char *dirName,
							unsigned int errDialog,
							CellSaveDataSetBuf *setBuf,
							CellSaveDataStatCallback funcStat,
							CellSaveDataFileCallback funcFile,
							sys_memory_container_t container,
							void *userdata );

/*J ユーザ指定オートロード */
int cellSaveDataUserAutoLoad( unsigned int version,
							CellSysutilUserId userId,
							const char *dirName,
							unsigned int errDialog,
							CellSaveDataSetBuf *setBuf,
							CellSaveDataStatCallback funcStat,
							CellSaveDataFileCallback funcFile,
							sys_memory_container_t container,
							void *userdata );

/*J ユーザ指定リスト取得型オートセーブ */
int cellSaveDataUserListAutoSave( unsigned int version,
							CellSysutilUserId userId,
							unsigned int errDialog,
							CellSaveDataSetList *setList,
							CellSaveDataSetBuf *setBuf,
							CellSaveDataFixedCallback funcFixed,
							CellSaveDataStatCallback funcStat,
							CellSaveDataFileCallback funcFile,
							sys_memory_container_t container,
							void *userdata );

/*J ユーザ指定リスト取得型オートロード */
int cellSaveDataUserListAutoLoad( unsigned int version,
							CellSysutilUserId userId,
							unsigned int errDialog,
							CellSaveDataSetList *setList,
							CellSaveDataSetBuf *setBuf,
							CellSaveDataFixedCallback funcFixed,
							CellSaveDataStatCallback funcStat,
							CellSaveDataFileCallback funcFile,
							sys_memory_container_t container,
							void *userdata );


/*J ユーザ指定リスト選択型セーブデータ取り込み（同期関数）*/
int cellSaveDataUserListImport(	CellSysutilUserId userId,
							CellSaveDataSetList *setList,
							unsigned int maxSizeKB,
							CellSaveDataDoneCallback funcDone,
							sys_memory_container_t container,
							void *userdata );

/*J ユーザ指定リスト選択型セーブデータ書き出し（同期関数）*/
int cellSaveDataUserListExport(	CellSysutilUserId userId,
							CellSaveDataSetList *setList,
							unsigned int maxSizeKB,
							CellSaveDataDoneCallback funcDone,
							sys_memory_container_t container,
							void *userdata );

/*J ユーザ指定固定型セーブデータ取り込み（同期関数）*/
int cellSaveDataUserFixedImport( CellSysutilUserId userId,
							const char *dirName,
							unsigned int maxSizeKB,
							CellSaveDataDoneCallback funcDone,
							sys_memory_container_t container,
							void *userdata );

/*J ユーザ指定固定型セーブデータ書き出し（同期関数）*/
int cellSaveDataUserFixedExport( CellSysutilUserId userId,
							const char *dirName,
							unsigned int maxSizeKB,
							CellSaveDataDoneCallback funcDone,
							sys_memory_container_t container,
							void *userdata );

/*J ユーザ指定リスト選択型削除 */
int cellSaveDataUserListDelete(	CellSysutilUserId userId,
							CellSaveDataSetList *setList,
							CellSaveDataSetBuf *setBuf,
							CellSaveDataListCallback funcList,
							CellSaveDataDoneCallback funcDone,
							sys_memory_container_t container,
							void *userdata );

/*J ユーザ指定固定型削除 */
int cellSaveDataUserFixedDelete( CellSysutilUserId userId,
							CellSaveDataSetList *setList,
							CellSaveDataSetBuf *setBuf,
							CellSaveDataFixedCallback funcFixed,
							CellSaveDataDoneCallback funcDone,
							sys_memory_container_t container,
							void *userdata );

/*J ユーザ指定リストアイテム情報取得 */
int cellSaveDataUserGetListItem( CellSysutilUserId userId,
							const char *dirName,
							CellSaveDataDirStat *dir,
							CellSaveDataSystemFileParam *sysFileParam,
							unsigned int *bind,
							int *sizeKB );

/*J オーバーレイ表示設定 */
void cellSaveDataEnableOverlay( int enable );

#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
}
#endif	/* defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus) */

#endif /*_CELL_SYSUTIL_SAVEDATA_H_*/
