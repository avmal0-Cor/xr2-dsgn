/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2007 Sony Computer Entertainment Inc.
* All Rights Reserved.
*/

#ifndef _CELL_SYSUTIL_SYSCACHE_H_
#define _CELL_SYSUTIL_SYSCACHE_H_

#include "sysutil_common.h"

#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
extern "C" {
#endif	/* defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus) */

/*J 正常終了コード */
#define CELL_SYSCACHE_RET_OK_CLEARED		(0)	/*J 正常終了。キャッシュの内容はクリアされた */
#define CELL_SYSCACHE_RET_OK_RELAYED		(1) /*J 正常終了。キャッシュの内容は前回使用時のままで使用可能となった */

/*J エラー終了コード */
#define CELL_SYSCACHE_ERROR_ACCESS_ERROR	(0x8002bc01)	/*J 0x8002bc01 HDDアクセスエラー */
#define CELL_SYSCACHE_ERROR_INTERNAL		(0x8002bc02)	/*J 0x8002bc02 内部エラー */

/*J エラー終了コード−アプリケーション開発時までに対処する必要のあるもの */
#define CELL_SYSCACHE_ERROR_PARAM			(0x8002bc03)	/*J 0x8002bc03 指定されたパラメータが不正 */
#define CELL_SYSCACHE_ERROR_NOTMOUNTED		(0x8002bc04)	/*J 0x8002bc04 システムキャッシュが使用可能状態でない */


typedef enum {
	CELL_SYSCACHE_ID_SIZE		= 32,	/*J キャッシュIDサイズ */
	CELL_SYSCACHE_PATH_MAX		= 1055	/*J キャッシュ領域へのファイルパスの最大長 */
} CellSysCacheParamSize;

typedef struct{									/*J システムキャッシュ利用パラメータ */
	char cacheId[CELL_SYSCACHE_ID_SIZE];		/*J 品番(ABCD12345) */
	char getCachePath[CELL_SYSCACHE_PATH_MAX];	/*J キャッシュ領域のファイルパス */
	void *reserved;
} CellSysCacheParam ;


/*J ユーティリティ関数 ------------------------------------------------------------------------------ */
/*J システムキャッシュのマウント */
int cellSysCacheMount(
	CellSysCacheParam *param ) ;

/*J システムキャッシュ領域の初期化 */
int cellSysCacheClear( void ) ;

#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
}
#endif	/* defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus) */

#endif /*_CELL_SYSUTIL_SYSCACHE_H_*/
