/*   SCE CONFIDENTIAL                                       */
/*   PlayStation(R)3 Programmer Tool Runtime Library 310.001 */
/*   Copyright (C) 2006 Sony Computer Entertainment Inc.    */
/*   All Rights Reserved.                                   */
#ifndef __LIBKEY2CHAR_H__
#define __LIBKEY2CHAR_H__

#ifdef __cplusplus
extern "C" {
#endif	/* __cplusplus */
#include <cell/keyboard.h>
#include <cell/error.h>

/*J CellKey2CharHandleハンドルのサイズ	*/
#define SCE_KEY2CHAR_HANDLE_SIZE	128

/*J CellKey2CharHandle	*/
typedef uint8_t CellKey2CharHandle[SCE_KEY2CHAR_HANDLE_SIZE];

/*J キーボードデータの構造体 */
typedef struct CellKey2CharKeyData{
	uint32_t led;
	uint32_t mkey;
	uint16_t keycode;
}CellKey2CharKeyData;

/*J cellKey2CharSetMode()で与えるモード */
typedef enum{
	CELL_KEY2CHAR_MODE_ENGLISH,		/*J 英数 */
	CELL_KEY2CHAR_MODE_NATIVE,		/*J 各国語（かな、キリル文字）、中国繁体字（注音）*/
	CELL_KEY2CHAR_MODE_NATIVE2		/*J 中国繁体字（倉頡）*/
};

/*J libkey2char API */
int32_t cellKey2CharOpen(CellKey2CharHandle handle);
int32_t cellKey2CharClose(CellKey2CharHandle handle);
int32_t cellKey2CharGetChar(CellKey2CharHandle handle, CellKey2CharKeyData *kdata, uint16_t **charCode, uint32_t *charNum, bool *processed);
int32_t cellKey2CharSetMode(CellKey2CharHandle handle, int32_t mode);
int32_t cellKey2CharSetArrangement(CellKey2CharHandle handle, uint32_t arrange);

/*J エラーコード */
/*
	CELL_ERROR_FACILITY_HID				0x8012
	libkey2char							0x8012_1301 - 0x8012_13ff
*/
#define CELL_K2C_MAKE_ERROR(status) CELL_ERROR_MAKE_ERROR(CELL_ERROR_FACILITY_HID, status)
#define CELL_K2C_OK										CELL_ERROR_CAST(0x00000000)	/*J 正常終了 */
#define CELL_K2C_ERROR_FATAL							CELL_ERROR_CAST(0x80121301)	/*J 致命的なエラー */
#define CELL_K2C_ERROR_INVALID_HANDLE					CELL_ERROR_CAST(0x80121302)	/*J ハンドル不正 */
#define CELL_K2C_ERROR_INVALID_PARAMETER				CELL_ERROR_CAST(0x80121303)	/*J パラメータ不正 */
#define CELL_K2C_ERROR_ALREADY_INITIALIZED				CELL_ERROR_CAST(0x80121304)	/*J すでに初期化済み */
#define CELL_K2C_ERROR_UNINITIALIZED					CELL_ERROR_CAST(0x80121305)	/*J 初期化されていない */
#define CELL_K2C_ERROR_OTHER							CELL_ERROR_CAST(0x80121306)	/*J それ以外のエラー */

#ifdef __cplusplus
}
#endif	/* __cplusplus */

#endif //__LIBKEY2CHAR_H__
