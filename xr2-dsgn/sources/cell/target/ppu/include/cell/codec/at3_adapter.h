/* SCE CONFIDENTIAL
 PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2008 Sony Computer Entertainment Inc.
 * All Rights Reserved.
 */

#ifndef _INCLUDE_AT3_ADAPTER_H
#define _INCLUDE_AT3_ADAPTER_H

#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

#define CELL_ADEC_ATRAC3_WORD_SZ_16BIT (0x02)
#define CELL_ADEC_ATRAC3_WORD_SZ_24BIT (0x03)
#define CELL_ADEC_ATRAC3_WORD_SZ_32BIT (0x04)
#define CELL_ADEC_ATRAC3_WORD_SZ_FLOAT (0x84)

typedef struct {
    int32_t nch;           /* チャンネル数 */
    int32_t isJoint;       /* Dual-stereo:0 Joint-stereo:1 */
    int32_t nbytes;        /* 1AUのバイト数 */
    int32_t bw_pcm;        /* 出力PCMサンプルの型、ビット長
                              0x02 short int,16bit
                              0x03 int,24bit(not use)
                              0x04 int,32bit(not use)
                              0x84 float,32bit
                           */
} CellAdecParamAtrac3;


/* ATRAC3 BSI */
typedef struct {
    int32_t nch;           /* Channel number */
    int32_t isJoint;       /* Dual-stereo:0 Joint-stereo:1 */
    int32_t nbytes;        /* Bytes per one channel */
}CellAdecAtrac3Info;


/*==========================================================================
  Eeeor code for error callback
  ==========================================================================*/
#define CELL_ADEC_ERROR_AT3_OFFSET CELL_ERROR_CAST(0x80612100)

#define CELL_ADEC_ERROR_AT3_OK     CELL_ERROR_CAST(0x80612100)

/* Adapter level error */
#define CELL_ADEC_ERROR_AT3_BUSY   CELL_ERROR_CAST(0x80612164)
#define CELL_ADEC_ERROR_AT3_EMPTY  CELL_ERROR_CAST(0x80612165)

/* Core level error */
#define CELL_ADEC_ERROR_AT3_ERROR  CELL_ERROR_CAST(0x80612180)

#if defined(__cplusplus)
}
#endif /* defined(__cplusplus) */

#endif /* _INCLUDE_AT3_ADAPTER_H */
