/* SCE CONFIDENTIAL
 PlayStation(R)3 Programmer Tool Runtime Library 310.001
 *
 *      Copyright (C) 2006 Sony Computer Entertainment Inc.
 *                        All Rights Reserved.
 *
 *
 */

#ifndef __CELL_LIBATRAC3PLUS_H__
#define __CELL_LIBATRAC3PLUS_H__

#include <stdint.h>
#include <cell/error.h>
#include <cell/spurs.h>

#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
extern "C" {
#endif

/*J エラーコードの定義       (FACLITY=CELL_ERROR_FACILITY_CODEC 0x061) */
/*J                           libatrac3plus : 0x80610300 - 0x806103ff) */
/*E Error code definition    (FACLITY=CELL_ERROR_FACILITY_CODEC 0x061) */
/*E                           libatrac3plus : 0x80610300 - 0x806103ff) */

#define CELL_ATRAC_ERROR_FACILITY_ATRAC              CELL_ERROR_CAST(0x80610300)

#define CELL_ATRAC_OK                                CELL_ERROR_CAST(0x00000000)

#define CELL_ATRAC_ERROR_API_FAIL                    CELL_ERROR_CAST(0x80610301)

#define CELL_ATRAC_ERROR_READSIZE_OVER_BUFFER        CELL_ERROR_CAST(0x80610311)
#define CELL_ATRAC_ERROR_UNKNOWN_FORMAT              CELL_ERROR_CAST(0x80610312)
#define CELL_ATRAC_ERROR_READSIZE_IS_TOO_SMALL       CELL_ERROR_CAST(0x80610313)
#define CELL_ATRAC_ERROR_ILLEGAL_SAMPLING_RATE       CELL_ERROR_CAST(0x80610314)
#define CELL_ATRAC_ERROR_ILLEGAL_DATA                CELL_ERROR_CAST(0x80610315)

#define CELL_ATRAC_ERROR_NO_DECODER                  CELL_ERROR_CAST(0x80610321)
#define CELL_ATRAC_ERROR_UNSET_DATA                  CELL_ERROR_CAST(0x80610322)
#define CELL_ATRAC_ERROR_DECODER_WAS_CREATED         CELL_ERROR_CAST(0x80610323)

#define CELL_ATRAC_ERROR_ALLDATA_WAS_DECODED         CELL_ERROR_CAST(0x80610331)
#define CELL_ATRAC_ERROR_NODATA_IN_BUFFER            CELL_ERROR_CAST(0x80610332)
#define CELL_ATRAC_ERROR_NOT_ALIGNED_OUT_BUFFER      CELL_ERROR_CAST(0x80610333)
#define CELL_ATRAC_ERROR_NEED_SECOND_BUFFER          CELL_ERROR_CAST(0x80610334)

#define CELL_ATRAC_ERROR_ALLDATA_IS_ONMEMORY         CELL_ERROR_CAST(0x80610341)
#define CELL_ATRAC_ERROR_ADD_DATA_IS_TOO_BIG         CELL_ERROR_CAST(0x80610342)

#define CELL_ATRAC_ERROR_NONEED_SECOND_BUFFER        CELL_ERROR_CAST(0x80610351)

#define CELL_ATRAC_ERROR_UNSET_LOOP_NUM              CELL_ERROR_CAST(0x80610361)

#define CELL_ATRAC_ERROR_ILLEGAL_SAMPLE              CELL_ERROR_CAST(0x80610371)
#define CELL_ATRAC_ERROR_ILLEGAL_RESET_BYTE          CELL_ERROR_CAST(0x80610372)

#define CELL_ATRAC_ERROR_ILLEGAL_PPU_THREAD_PRIORITY CELL_ERROR_CAST(0x80610381)
#define CELL_ATRAC_ERROR_ILLEGAL_SPU_THREAD_PRIORITY CELL_ERROR_CAST(0x80610382)

/*J 特殊な状態におけるデコードできる残りフレーム数 */
/*E Remain Frame for some special cases */
#define CELL_ATRAC_ALLDATA_IS_ON_MEMORY             (-1)
#define CELL_ATRAC_NONLOOP_STREAM_DATA_IS_ON_MEMORY (-2)
#define CELL_ATRAC_LOOP_STREAM_DATA_IS_ON_MEMORY    (-3)

#define CELL_ATRAC_HANDLE_SIZE (512)

typedef struct {
	uint8_t uiWorkMem[CELL_ATRAC_HANDLE_SIZE];
} CellAtracHandle __attribute__((aligned(8)));

typedef struct {
	uint8_t  *pucWriteAddr;
	uint32_t uiWritableByte;
	uint32_t uiMinWriteByte;
	uint32_t uiReadPosition;
} CellAtracBufferInfo;


typedef struct {
	CellSpurs  *pSpurs;
	uint8_t    priority[8];
} CellAtracExtRes;


/*J ワークメモリサイズの取得関数 */
int32_t cellAtracSetDataAndGetMemSize(CellAtracHandle *pHandle,
									  uint8_t         *pucBufferAddr,
									  uint32_t        uiReadByte,
									  uint32_t        uiBufferByte,
									  uint32_t        *puiWorkMemByte);

/*J デコーダの作成及び解放 */
int32_t cellAtracCreateDecoder(CellAtracHandle *pHandle,
							   uint8_t         *pucWorkMem,
							   uint32_t          uiPpuThreadPriority,
							   uint32_t          uiSpuThreadPriority);

int32_t cellAtracCreateDecoderExt(CellAtracHandle *pHandle,
								  uint8_t         *pucWorkMem,
								  uint32_t        uiPpuThreadPriority,
								  CellAtracExtRes *pExtRes);

int32_t cellAtracDeleteDecoder(CellAtracHandle *pHandle);

/*J デコードを行なう */
int32_t cellAtracDecode(CellAtracHandle *pHandle,
						float           *pfOutAddr,
						uint32_t        *puiSamples,
						uint32_t        *puifinishflag,
						int32_t         *piRemainframe);

/*J データの継ぎ足しを行なう */
int32_t cellAtracAddStreamData(CellAtracHandle *pHandle,
							   uint32_t  uiAddByte);

/*J セカンドバッファ関連API */
int32_t cellAtracGetSecondBufferInfo(CellAtracHandle *pHandle,
									 uint32_t  *puiReadPosition,
									 uint32_t  *puiDataByte);

int32_t cellAtracSetSecondBuffer(CellAtracHandle *pHandle,
								 uint8_t         *pucSecondBufferAddr,
								 uint32_t        uiSecondBufferByte);

/*J 情報取得系API */
int32_t cellAtracGetRemainFrame(CellAtracHandle *pHandle,
								int32_t   *piRemainframe);

int32_t cellAtracGetVacantSize(CellAtracHandle *pHandle,
							   uint32_t   *puiVacantSize);

int32_t cellAtracGetStreamDataInfo(CellAtracHandle *pHandle,
								   uint8_t    **ppucWritePointer,
								   uint32_t    *puiWritableByte,
								   uint32_t    *puiReadPosition);

int32_t cellAtracGetChannel(CellAtracHandle *pHandle,
							uint32_t        *puiChannel);

int32_t cellAtracGetMaxSample(CellAtracHandle *pHandle,
							  uint32_t        *puiMaxSample);

int32_t cellAtracGetNextSample(CellAtracHandle *pHandle,
							   uint32_t        *puiNextSample);

int32_t cellAtracGetSoundInfo(CellAtracHandle *pHandle,
							  int32_t         *piEndSample,
							  int32_t         *piLoopStartSample,
							  int32_t         *piLoopEndSample);

int32_t cellAtracGetNextDecodePosition(CellAtracHandle *pHandle,
									   uint32_t *puiSamplePosition);

int32_t cellAtracGetBitrate(CellAtracHandle *pHandle,
							uint32_t	*puiBitrate);

int32_t cellAtracGetLoopInfo(CellAtracHandle *pHandle,
							 int32_t         *piLoopNum,
							 uint32_t        *puiLoopStatus);

int32_t cellAtracIsSecondBufferNeeded(CellAtracHandle *pHandle);

/*J ループ回数変更API */
int32_t cellAtracSetLoopNum(CellAtracHandle *pHandle, 
							int32_t  iLoopNum);

/*J 再生位置変更API */
int32_t cellAtracGetBufferInfoForResetting(CellAtracHandle     *pHandle, 
										   uint32_t            uiSample,
										   CellAtracBufferInfo *pBufferInfo);

int32_t cellAtracResetPlayPosition(CellAtracHandle *pHandle, 
								   uint32_t        uiSample,
								   uint32_t        uiWriteByte);

/*J 内部エラー取得関数 */
int32_t cellAtracGetInternalErrorInfo(CellAtracHandle *pHandle, 
									  int32_t *piResult);

#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
}
#endif

#endif /* __CELL_LIBATRAC3PLUS_H__ */
