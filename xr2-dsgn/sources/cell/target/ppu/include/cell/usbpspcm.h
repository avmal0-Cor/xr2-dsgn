/* SCE CONFIDENTIAL
 PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2007 Sony Computer Entertainment Inc.
 * All Rights Reserved.
 */

#ifndef __CELL_USBPSPCM_H__
#define __CELL_USBPSPCM_H__

#include <sys/types.h>

#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
extern "C" {
#endif

/*J タイトルチェック用文字列の最大長 */
/*E Maximum length of string for title check */
#define CELL_USBPSPCM_MAX_TITLE_LENGTH      64

/*J データパケットのサイズ */
/*E Size of data packet */
#define CELL_USBPSPCM_DATA_SIZE             0x1000

/*J 挿抜状態 */
/*E Connection state */
#define CELL_USBPSPCM_STAT_DISCONNECT       0
#define CELL_USBPSPCM_STAT_CONNECT          1

/*J 送受信モード */
/*E Mode of send/receive data */
#define CELL_USBPSPCM_MODE_NORMAL           0x00U
#define CELL_USBPSPCM_MODE_SEQUENCIAL       0x01U


/*J 初期化パラメータ */
/*E Parameter of initialization */
typedef struct CellUsbPspcmInitParam {
	size_t      size;
	int         numDev;
	int         numCh;
	void        *pool;
	size_t      poolSize;
	int         priority;
	size_t      stackSize;
	const char  *defaultTitle;
} CellUsbPspcmInitParam;

/*J 挿抜状態 */
/*E Connection state */
typedef struct CellUsbPspcmConnectStat {
	int         addr;
	uint32_t    stat;
	char        title[CELL_USBPSPCM_MAX_TITLE_LENGTH];
} CellUsbPspcmConnectStat;

/*J 挿抜処理関数 */
/*E Function for the notification of connection/disconnection */
typedef int (CellUsbPspcmConnectFunc)(const CellUsbPspcmConnectStat *pStat);


/*J エラーコード */
/*E Error codes */
#define CELL_USBPSPCM_ERROR_NOT_INITIALIZED  CELL_ERROR_CAST(0x80110401)
#define CELL_USBPSPCM_ERROR_ALREADY          CELL_ERROR_CAST(0x80110402)
#define CELL_USBPSPCM_ERROR_INVALID          CELL_ERROR_CAST(0x80110403)
#define CELL_USBPSPCM_ERROR_NO_MEMORY        CELL_ERROR_CAST(0x80110404)
#define CELL_USBPSPCM_ERROR_BUSY             CELL_ERROR_CAST(0x80110405)
#define CELL_USBPSPCM_ERROR_INPROGRESS       CELL_ERROR_CAST(0x80110406)
#define CELL_USBPSPCM_ERROR_NO_SPACE         CELL_ERROR_CAST(0x80110407)
#define CELL_USBPSPCM_ERROR_CANCELED         CELL_ERROR_CAST(0x80110408)
#define CELL_USBPSPCM_ERROR_RESETTING        CELL_ERROR_CAST(0x80110409)
#define CELL_USBPSPCM_ERROR_RESET_END        CELL_ERROR_CAST(0x8011040A)
#define CELL_USBPSPCM_ERROR_CLOSED           CELL_ERROR_CAST(0x8011040B)
#define CELL_USBPSPCM_ERROR_NO_DATA          CELL_ERROR_CAST(0x8011040C)


/*J プロトタイプ宣言 */
/*E Prototype declaration */
int cellUsbPspcmInit(const CellUsbPspcmInitParam *pInitParam);
int cellUsbPspcmEnd(void);
int cellUsbPspcmCalcPoolSize(const CellUsbPspcmInitParam *pInitParam);
int cellUsbPspcmRegister(CellUsbPspcmConnectFunc *pFunc);
int cellUsbPspcmUnregister(CellUsbPspcmConnectFunc *pFunc);
int cellUsbPspcmGetAddr(void);
int cellUsbPspcmBind(int addr);
int cellUsbPspcmBindAsync(int addr);
int cellUsbPspcmWaitBindAsync(int addr);
int cellUsbPspcmPollBindAsync(int addr);
int cellUsbPspcmCancelBind(int addr);
int cellUsbPspcmClose(int chIdx);
int cellUsbPspcmSend(int chIdx, uint8_t *buf, size_t size, uint32_t mode);
int cellUsbPspcmSendAsync(int chIdx, uint8_t *buf, size_t size, uint32_t mode);
int cellUsbPspcmWaitSendAsync(int chIdx);
int cellUsbPspcmPollSendAsync(int chIdx);
int cellUsbPspcmRecv(int chIdx, uint8_t *buf, size_t size, uint32_t mode);
int cellUsbPspcmRecvAsync(int chIdx, uint8_t *buf, size_t size, uint32_t mode);
int cellUsbPspcmWaitRecvAsync(int chIdx);
int cellUsbPspcmPollRecvAsync(int chIdx);
int cellUsbPspcmReset(int chIdx);
int cellUsbPspcmResetAsync(int chIdx);
int cellUsbPspcmWaitResetAsync(int chIdx);
int cellUsbPspcmPollResetAsync(int chIdx);
int cellUsbPspcmWaitData(int chIdx);
int cellUsbPspcmPollData(int chIdx);
int cellUsbPspcmCancelWaitData(int chIdx);

#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
}
#endif

#endif /* __CELL_USBPSPCM_H__ */
