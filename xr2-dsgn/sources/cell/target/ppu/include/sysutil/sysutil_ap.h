/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2009 Sony Computer Entertainment Inc.
 * All Rights Reserved.
 */

#ifndef _CELL_SYSUTIL_AP_H_
#define _CELL_SYSUTIL_AP_H_

#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
extern "C" {
#endif	/* defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus) */

#include <sys/memory.h>

#define CELL_SYSUTIL_AP_ERROR_OUT_OF_MEMORY         ( 0x8002cd00 )
#define CELL_SYSUTIL_AP_ERROR_FATAL                 ( 0x8002cd01 )
#define CELL_SYSUTIL_AP_ERROR_INVALID_VALUE         ( 0x8002cd02 )
#define CELL_SYSUTIL_AP_ERROR_NOT_INITIALIZED       ( 0x8002cd03 )
#define CELL_SYSUTIL_AP_ERROR_ZERO_REGISTERED       ( 0x8002cd13 )
#define CELL_SYSUTIL_AP_ERROR_NETIF_DISABLED        ( 0x8002cd14 )
#define CELL_SYSUTIL_AP_ERROR_NETIF_NO_CABLE        ( 0x8002cd15 )
#define CELL_SYSUTIL_AP_ERROR_NETIF_CANNOT_CONNECT  ( 0x8002cd16 )

#define CELL_SYSUTIL_AP_TYPE_USE_GAME_SETTING       ( 0x00000001 )
#define CELL_SYSUTIL_AP_TYPE_USE_SYSTEM_SETTING     ( 0x00000002 )
#define CELL_SYSUTIL_AP_TYPE_USE_REMOTEPLAY_SETTING ( 0x00000003 )

#define CELL_SYSUTIL_AP_TITLE_ID_LEN (9)
#define CELL_SYSUTIL_AP_SSID_LEN     (32)
#define CELL_SYSUTIL_AP_WPA_KEY_LEN  (64)

#define CELL_SYSUTIL_AP_DISABLE      (0x00)
#define CELL_SYSUTIL_AP_AVAILABLE    (0x01)

#define CELL_SYSUTIL_AP_MEMORY_CONTAINER_SIZE (1*1024*1024)

typedef struct
{
	char data[CELL_SYSUTIL_AP_TITLE_ID_LEN];
	char padding[3];

} CellSysutilApTitleId;

typedef struct
{	
	char data[CELL_SYSUTIL_AP_SSID_LEN + 1];
	char padding[3];

} CellSysutilApSsid;

typedef struct
{
	char data[CELL_SYSUTIL_AP_WPA_KEY_LEN + 1];
	char padding[3];

} CellSysutilApWpaKey;

typedef struct
{
	int                  type;
	int                  wlanFlag;
	CellSysutilApTitleId titleId;
	CellSysutilApSsid    ssid;
	CellSysutilApWpaKey  wpakey;

} CellSysutilApParam;

int cellSysutilApGetRequiredMemSize (void);

int cellSysutilApOn ( CellSysutilApParam* pParam, sys_memory_container_t container );

int cellSysutilApOff (void);

#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
}
#endif	/* defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus) */

#endif /*_CELL_SYSUTIL_AP_H_*/
