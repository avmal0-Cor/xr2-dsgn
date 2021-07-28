/*
 *     SCE CONFIDENTIAL
 *     PlayStation(R)3 Programmer Tool Runtime Library 310.001
 *     Copyright (C) 2008 Sony Computer Entertainment Inc.
 *     All Rights Reserved.
 */
/*
* libpad for Windows
*
*   libpad.h
*
*   Date            Design      Log
*  ----------------------------------------------------
*   2007-08-08      shinsuke    Ported from PLAYSTATION(R)3 Runtime Library
*   2007-12-06      shinsuke    Added EOL to the last line
*   2007-12-18      shinsuke    Added CELL_PAD_CAPABILITY_ACTUATOR
*                                     CELL_PAD_IS_ACTUATOR()
*   2007-04-18      shinsuke    Added CELL_PAD_SETTING_*
*                                     CELL_PAD_DEV_TYPE_*
*                                     CellPadInfo2
*                                     cellPadGetInfo2()
*                                     cellPadSetPortSetting()
*                                     
*                                     
*
*/
/*J libpad ヘッダ */
/*E libpad header */
#ifndef _LIBPAD_H_
#define _LIBPAD_H_

/*******************************************************************************/
/*J 定数定義 */
/*E Constant Definitions */
/*******************************************************************************/

#define CELL_MAX_PADS		(127)
#define CELL_PAD_MAX_CODES	(64)
#define CELL_PAD_MAX_PORT_NUM (7)

/*E CellPadInfo2: System Info */
#define CELL_PAD_INFO_INTERCEPTED		(1 << 0) /*E input intercepted */
/*E CellPadInfo2: Port Status */
#define CELL_PAD_STATUS_DISCONNECTED    (0)         /*E Game Pad is disconnected  */
#define CELL_PAD_STATUS_CONNECTED       (1 << 0)    /*E Game Pad is connected.    */
#define CELL_PAD_STATUS_ASSIGN_CHANGES  (1 << 1)    /*E */
/*E CellPadInfo2: Port Setting */
#define CELL_PAD_SETTING_PRESS_ON   (1 << 1)    /* Press mode turn on */
#define CELL_PAD_SETTING_SENSOR_ON  (1 << 2)    /* Sensor mode turn on */
#define CELL_PAD_SETTING_PRESS_OFF  (0)
#define CELL_PAD_SETTING_SENSOR_OFF (0)
/*E CellPadInfo2: Capability */
#define CELL_PAD_CAPABILITY_PS3_CONFORMITY  (1 << 0)    /* PS3 Conformity Controller */
#define CELL_PAD_CAPABILITY_PRESS_MODE      (1 << 1)    /* Press mode supported */
#define CELL_PAD_CAPABILITY_SENSOR_MODE     (1 << 2)    /* Sensor mode supported */
#define CELL_PAD_CAPABILITY_HP_ANALOG_STICK (1 << 3)    /* High Precision ANALOG STICK */
#define CELL_PAD_CAPABILITY_ACTUATOR        (1 << 4)    /* Motor supported */
/*E CellPadInfo2: Device Type */
#define CELL_PAD_DEV_TYPE_STANDARD  (0)         /*E not special */
#define CELL_PAD_DEV_TYPE_LDD       (5)         /*E Custom Controller */

/*E CellPadData : Bit assignment of digital buttons. */
/*E "CELL_PAD_BTN_OFFSET_DIGITAL1" assignment */
#define CELL_PAD_CTRL_LEFT		(1 << 7)
#define CELL_PAD_CTRL_DOWN		(1 << 6)
#define CELL_PAD_CTRL_RIGHT		(1 << 5)
#define CELL_PAD_CTRL_UP		(1 << 4)
#define CELL_PAD_CTRL_START		(1 << 3)
#define CELL_PAD_CTRL_R3		(1 << 2)
#define CELL_PAD_CTRL_L3		(1 << 1)
#define CELL_PAD_CTRL_SELECT	(1 << 0)

/*E "CELL_PAD_BTN_OFFSET_DIGITAL2" assignment */
#define CELL_PAD_CTRL_SQUARE	(1 << 7)
#define CELL_PAD_CTRL_CROSS		(1 << 6)
#define CELL_PAD_CTRL_CIRCLE	(1 << 5)
#define CELL_PAD_CTRL_TRIANGLE	(1 << 4)
#define CELL_PAD_CTRL_R1		(1 << 3)
#define CELL_PAD_CTRL_L1		(1 << 2)
#define CELL_PAD_CTRL_R2		(1 << 1)
#define CELL_PAD_CTRL_L2		(1 << 0)

/*E The press mode support */
#define CELL_PAD_PRESS_MODE_OFF 			(0)
#define CELL_PAD_PRESS_MODE_ON				(1)

#define CELL_PAD_INFO_SUPPORTED_PRESS_MODE	(1)

/*E The sensor mode support */
#define CELL_PAD_SENSOR_MODE_OFF			(0)
#define CELL_PAD_SENSOR_MODE_ON				(1)

#define CELL_PAD_INFO_SUPPORTED_SENSOR_MODE (1)

#define CELL_PAD_IS_PS3_CONFORMITY(a) ((a)->info[0]&CELL_PAD_CAPABILITY_PS3_CONFORMITY ? 1:0)
#define CELL_PAD_IS_PRESS_MODE(a) ((a)->info[0]&CELL_PAD_CAPABILITY_PRESS_MODE ? 1:0)
#define CELL_PAD_IS_SENSOR_MODE(a) ((a)->info[0]&CELL_PAD_CAPABILITY_SENSOR_MODE ? 1:0)
#define CELL_PAD_IS_HP_ANALOG_STICK(a) ((a)->info[0]&CELL_PAD_CAPABILITY_HP_ANALOG_STICK ? 1:0)
#define CELL_PAD_IS_ACTUATOR(a) ((a)->info[0]&CELL_PAD_CAPABILITY_ACTUATOR ? 1:0)

#define CELL_PAD_CAPABILITY_INFO_MAX (32)

#define CELL_PAD_ACTUATOR_MAX	(2)

/*******************************************************************************/
/*J エラーコード */
/*E Error Codes */
/*******************************************************************************/

#define CELL_PAD_OK									(0x00000000)

/*J 致命的エラー */
/*E fatal error */
#define CELL_PAD_ERROR_FATAL						(0x80121101)

/*J パラメタ不正 */
/*E invalid parameter */
#define CELL_PAD_ERROR_INVALID_PARAMETER			(0x80121102)

/*J 既に初期化されている */
/*E library is already initialized */
#define CELL_PAD_ERROR_ALREADY_INITIALIZED			(0x80121103)

/*J 未初期化 */
/*E library is not inisialized */
#define CELL_PAD_ERROR_UNINITIALIZED				(0x80121104)

/*J リソース獲得エラー */
/*E resource allocation failed */
#define CELL_PAD_ERROR_RESOURCE_ALLOCATION_FAILED	(0x80121105)

/*J データ読み取りエラー */
/*E data read failed */
#define CELL_PAD_ERROR_DATA_READ_FAILED				(0x80121106)

/*J デバイスが存在しない */
/*E device is not exist */
#define CELL_PAD_ERROR_NO_DEVICE					(0x80121107)

/*J 非対応のコントローラ */
/*E the gamepad is not supported */
#define CELL_PAD_ERROR_UNSUPPORTED_GAMEPAD			(0x80121108)

/*J 最大サポートデバイス数オーバー */
/*E too many devices */
#define CELL_PAD_ERROR_TOO_MANY_DEVICES				(0x80121109)

/*J デバイスがビジー状態 */
/*E device is busy now */
#define CELL_PAD_ERROR_EBUSY						(0x8012110a)

/*******************************************************************************/
/*J 型定義 */
/*E Type Definitions */
/*******************************************************************************/

typedef	LONG	int32_t;
typedef	ULONG	uint32_t;
typedef	SHORT	int16_t;
typedef	USHORT	uint16_t;
typedef	CHAR	int8_t;
typedef	UCHAR	uint8_t;

typedef struct CellPadInfo2
{
    uint32_t max_connect;
    uint32_t now_connect;
    uint32_t system_info;
    uint32_t port_status[CELL_PAD_MAX_PORT_NUM];
    uint32_t port_setting[CELL_PAD_MAX_PORT_NUM];
    uint32_t device_capability[CELL_PAD_MAX_PORT_NUM];
    uint32_t device_type[CELL_PAD_MAX_PORT_NUM];
} CellPadInfo2;

typedef struct CellPadInfo{
	uint32_t	max_connect;
	uint32_t	now_connect;
	uint32_t	info;
	uint16_t	vendor_id[CELL_MAX_PADS];
	uint16_t	product_id[CELL_MAX_PADS];
	uint8_t		status[CELL_MAX_PADS];
}CellPadInfo;

/*E CellPadData : Byte offset of button data.  */
typedef enum CellPadButtonDataOffset{
	CELL_PAD_BTN_OFFSET_DIGITAL1 = 2,
	CELL_PAD_BTN_OFFSET_DIGITAL2 = 3,
	CELL_PAD_BTN_OFFSET_ANALOG_RIGHT_X = 4,
	CELL_PAD_BTN_OFFSET_ANALOG_RIGHT_Y = 5,
	CELL_PAD_BTN_OFFSET_ANALOG_LEFT_X  = 6,
	CELL_PAD_BTN_OFFSET_ANALOG_LEFT_Y  = 7,
	CELL_PAD_BTN_OFFSET_PRESS_RIGHT    = 8,
	CELL_PAD_BTN_OFFSET_PRESS_LEFT     = 9,
	CELL_PAD_BTN_OFFSET_PRESS_UP       = 10,
	CELL_PAD_BTN_OFFSET_PRESS_DOWN     = 11,
	CELL_PAD_BTN_OFFSET_PRESS_TRIANGLE = 12,
	CELL_PAD_BTN_OFFSET_PRESS_CIRCLE   = 13,
	CELL_PAD_BTN_OFFSET_PRESS_CROSS    = 14,
	CELL_PAD_BTN_OFFSET_PRESS_SQUARE   = 15,
	CELL_PAD_BTN_OFFSET_PRESS_L1       = 16,
	CELL_PAD_BTN_OFFSET_PRESS_R1       = 17,
	CELL_PAD_BTN_OFFSET_PRESS_L2       = 18,
	CELL_PAD_BTN_OFFSET_PRESS_R2       = 19,
	CELL_PAD_BTN_OFFSET_SENSOR_X       = 20,
	CELL_PAD_BTN_OFFSET_SENSOR_Y       = 21,
	CELL_PAD_BTN_OFFSET_SENSOR_Z       = 22,
	CELL_PAD_BTN_OFFSET_SENSOR_G       = 23
}CellPadButtonDataOffset;

typedef struct CellPadData{
	int32_t		len;
	uint16_t	button[CELL_PAD_MAX_CODES];
} CellPadData;

/*E Capability infomation */
typedef enum CellPadCapabilityInfoOffset
{
	CELL_PAD_CAP_OFFSET_FUNC1 = 0,
	CELL_PAD_CAP_OFFSET_FUNC2 = 1,
	CELL_PAD_CAP_RESERVED = 2
} CellPadCapabilityOffset;

typedef struct CellPadCapabilityInfo
{
	uint32_t	info[CELL_PAD_CAPABILITY_INFO_MAX];
} CellPadCapabilityInfo;

typedef struct CellPadActParam
{
	uint8_t		motor[CELL_PAD_ACTUATOR_MAX];
	uint8_t		reserved[6];
} CellPadActParam;

/*******************************************************************************/
/*J 関数宣言 */
/*E Functions */
/*******************************************************************************/

#ifdef	__cplusplus
extern "C" {
#endif	/* __cplusplus */

int32_t cellPadInit(uint32_t max_connect);
int32_t cellPadEnd(void);
int32_t cellPadClearBuf(uint32_t port_no);
int32_t cellPadGetData(uint32_t port_no, CellPadData *data);
int32_t cellPadSetActDirect(uint32_t port_no, CellPadActParam* param);
int32_t cellPadGetInfo2(CellPadInfo2 *info);
int32_t cellPadSetPortSetting(uint32_t port_no, uint32_t port_setting);

/*J 廃止予定 */
/*E will be removed.*/
int32_t cellPadGetCapabilityInfo(uint32_t port_no, CellPadCapabilityInfo *data);
int32_t cellPadGetInfo(CellPadInfo *info);
int32_t cellPadSetPressMode(uint32_t port_no, uint32_t mode);
int32_t cellPadInfoPressMode(uint32_t port_no);
int32_t cellPadSetSensorMode(uint32_t port_no, uint32_t mode);
int32_t cellPadInfoSensorMode(uint32_t port_no);

#if 0
/*J libpad for Windows では非サポート */
/*E not supperted in Windows version */
int32_t cellPadGetRawData(uint32_t port_no, CellPadData *data);
int32_t cellPadGetDataExtra(uint32_t port_no, uint32_t* device_type, CellPadData *data);
int32_t cellPadLddRegisterController(void);
int32_t cellPadLddUnregisterController(int32_t handle);
int32_t cellPadLddDataInsert(int32_t handle, CellPadData *data);
int32_t cellPadLddGetPortNo(int32_t handle);

int32_t cellPadDbgLddRegisterController(uint32_t device_capability);
int32_t cellPadDbgLddSetDataInsertMode(int32_t handle, uint8_t mode);
#endif

#ifdef	__cplusplus
}
#endif	/* __cplusplus */

#endif /* _LIBPAD_H_ */
