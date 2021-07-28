/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2007 Sony Computer Entertainment Inc.
* All Rights Reserved.
*/

#ifndef _CELL_SYSUTIL_USERINFO_H_
#define _CELL_SYSUTIL_USERINFO_H_


#include "sysutil_common.h"
#include <sys/memory.h>

#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
extern "C" {
#endif	/* defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus) */

/*J 正常終了コード */
#define CELL_USERINFO_RET_OK			(0)
#define CELL_USERINFO_RET_CANCEL		(1)

#define CELL_USERINFO_ERROR_BUSY		(0x8002c301) /*J 同時起動しようとした  マスター提出時までに対処する必要がある */
#define CELL_USERINFO_ERROR_INTERNAL	(0x8002c302) /*J 内部エラー */
#define CELL_USERINFO_ERROR_PARAM		(0x8002c303) /*J パラメータが不正  マスター提出時までに対処する必要がある */
#define CELL_USERINFO_ERROR_NOUSER		(0x8002c304) /*J 指定されたユーザが存在しなかった */

typedef enum {								/*J 各種サイズ */
	CELL_USERINFO_USER_MAX		= 16,		/*J ユーザ数最大値 */
	CELL_USERINFO_TITLE_SIZE	= 256,		/*J ユーザ選択ダイアログに表示されるタイトル最大サイズ */
	CELL_USERINFO_USERNAME_SIZE	= 64		/*J ユーザネーム最大サイズ終端のNULL含む */
} CellUserInfoParamSize;

typedef enum {								/*J 画面に表示するユーザリストのタイプ */
	CELL_USERINFO_LISTTYPE_ALL	= 0,		/*J カレントユーザを含めた全てのユーザを表示 */
	CELL_USERINFO_LISTTYPE_NOCURRENT		/*J カレントユーザを除いたユーザを表示 */
} CellUserInfoListType;

typedef enum {									/*J ユーザ選択リストの初期フォーカス位置 */
	CELL_USERINFO_FOCUS_LISTHEAD = 0xffffffff	/*J リスト先頭のアイコン */
} CellUserInfoFocus;

/*J 構造体：引数向け -------------------------------------------------------------------------------- */

typedef struct {							/*J ユーザ情報構造体 */
	CellSysutilUserId id;					/*J ユーザID */
	char name[CELL_USERINFO_USERNAME_SIZE];	/*J ユーザ名 */
} CellUserInfoUserStat;

typedef struct {							/* ユーザIDリスト */
	CellSysutilUserId userId[CELL_USERINFO_USER_MAX];
} CellUserInfoUserList;

typedef struct {						/*J ユーザ選択リスト指定構造体 */
	char *title;						/*J 画面上に表示するタイトル名 */
	CellSysutilUserId focus;			/*J フォーカス位置指定 */
	unsigned int fixedListNum;			/*J 固定リストサイズ */
	CellUserInfoUserList *fixedList;	/*J 固定リスト */
	void *reserved;
} CellUserInfoListSet;

typedef struct {						/*J リストタイプ指定構造体 */
	char *title;						/*J 画面上に表示するタイトル名 */
	CellSysutilUserId focus;			/*J フォーカス位置指定 */
	CellUserInfoListType type;			/*J リストタイプ */
	void *reserved;
} CellUserInfoTypeSet;

/*J コールバック関数 -------------------------------------------------------------------------------- */
/*J ユーザ選択リスト終了コールバック関数 */
typedef void (*CellUserInfoFinishCallback)( int result,
											CellUserInfoUserStat *selectedUser,
											void *userdata );

/*J ユーティリティ関数 ------------------------------------------------------------------------------ */

/*J ユーザ情報取得（同期関数）*/
int cellUserInfoGetStat( CellSysutilUserId id, CellUserInfoUserStat *stat );

/*J ユーザリスト取得（同期関数）*/
int cellUserInfoGetList(
		unsigned int *listNum,					/*J ユーザリストの項目数を受け取るポインタ */
		CellUserInfoUserList *listBuf,			/*J CellUserInfoListSetに合わせた ユーザリストを受け取るバッファ */
		CellSysutilUserId *currentUserId		/*J カレントユーザのユーザIDを受け取るポインタ */
		);

/*J リスト指定型ユーザ選択（非同期関数）*/
int cellUserInfoSelectUser_SetList(
		CellUserInfoListSet *setList,			/*J ユーザ選択リストのユーザリスト指定 */
		CellUserInfoFinishCallback funcSelect,	/*J ユーザ選択終了時に呼ばれるコールバック関数 */
		sys_memory_container_t container,		/*J メモリコンテナ */
		void *userdata							/*J コールバック関数に渡されるデータ */
		);
 
/*J リストタイプ型ユーザ選択（非同期関数）*/
int cellUserInfoSelectUser_ListType(
		CellUserInfoTypeSet *listType,			/*J ユーザ選択リストのリストタイプ指定 */
		CellUserInfoFinishCallback funcSelect,	/*J ユーザ選択終了時に呼ばれるコールバック関数 */
		sys_memory_container_t container,		/*J メモリコンテナ */
		void *userdata							/*J コールバック関数に渡されるデータ */
		);

/*J オーバーレイ表示設定 */
void cellUserInfoEnableOverlay( int enable );

#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
}
#endif	/* defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus) */

#endif /*_CELL_SYSUTIL_USERINFO_H_*/
