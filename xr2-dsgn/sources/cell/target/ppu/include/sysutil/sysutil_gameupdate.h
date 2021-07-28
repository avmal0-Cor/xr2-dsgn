/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2007 Sony Computer Entertainment Inc.
* All Rights Reserved.
*/


#ifndef _CELL_SYSUTIL_GAMEUPDATE_H_
#define _CELL_SYSUTIL_GAMEUPDATE_H_

#include <sys/types.h>
#include <sysutil/sysutil_gamecontent.h>

#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
extern "C" {
#endif	/* defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus) */

#define CELL_GAMEUPDATE_ERROR_NOT_INITIALIZED				(0x8002cc01)
#define CELL_GAMEUPDATE_ERROR_ALREADY_INITIALIZED			(0x8002cc02)
#define CELL_GAMEUPDATE_ERROR_INVALID_ADDR					(0x8002cc03)
#define CELL_GAMEUPDATE_ERROR_INVALID_SIZE					(0x8002cc04)
#define CELL_GAMEUPDATE_ERROR_INVALID_MEMORY_CONTAINER		(0x8002cc05)
#define CELL_GAMEUPDATE_ERROR_INSUFFICIENT_MEMORY_CONTAINER	(0x8002cc06)
#define CELL_GAMEUPDATE_ERROR_BUSY							(0x8002cc07)
#define CELL_GAMEUPDATE_ERROR_NOT_START						(0x8002cc08)
#define CELL_GAMEUPDATE_ERROR_LOAD_FAILED					(0x8002cc09)

#define CELL_GAMEUPDATE_RESULT_STATUS_NO_UPDATE		0
#define CELL_GAMEUPDATE_RESULT_STATUS_UPDATE_FOUND	1
#define CELL_GAMEUPDATE_RESULT_STATUS_MAINTENANCE	2
#define CELL_GAMEUPDATE_RESULT_STATUS_ERROR			3
#define CELL_GAMEUPDATE_RESULT_STATUS_CANCELLED		4
#define CELL_GAMEUPDATE_RESULT_STATUS_FINISHED		5
#define CELL_GAMEUPDATE_RESULT_STATUS_ABORTED		6
#define CELL_GAMEUPDATE_RESULT_STATUS_SYSTEM_UPDATE_NEEDED	7

typedef int CellGameUpdateResultStatus;

typedef void (*CellGameUpdateCallback)(
	CellGameUpdateResultStatus status,
	int error_code,
	void *userdata
);

typedef struct {
	CellGameUpdateResultStatus status;
	int error_code;
	char app_ver[CELL_GAME_SYSP_APP_VER_SIZE];
	char padding[2];
} CellGameUpdateResult;

typedef void (*CellGameUpdateCallbackEx)(
	CellGameUpdateResult *result,
	void *userdata
);

typedef struct {
	size_t size;
	sys_memory_container_t cid;
} CellGameUpdateParam;

int cellGameUpdateInit( void );
int cellGameUpdateCheckStartAsync( const CellGameUpdateParam *param, CellGameUpdateCallback cb_func, void *userdata );
int cellGameUpdateCheckFinishAsync( CellGameUpdateCallback cb_func, void *userdata );
int cellGameUpdateCheckStartWithoutDialogAsync( CellGameUpdateCallback cb_func, void *userdata );
int cellGameUpdateCheckAbort( void );
int cellGameUpdateTerm( void );
int cellGameUpdateCheckStartAsyncEx( const CellGameUpdateParam *param , CellGameUpdateCallbackEx cb_func, void *userdata );
int cellGameUpdateCheckFinishAsyncEx( CellGameUpdateCallbackEx cb_func, void *userdata );
int cellGameUpdateCheckStartWithoutDialogAsyncEx( CellGameUpdateCallbackEx cb_func, void *userdata );

#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
}
#endif	/* defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus) */

#endif /*_CELL_SYSUTIL_GAMEUPDATE_H_*/
