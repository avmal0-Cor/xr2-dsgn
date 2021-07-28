/* SCE CONFIDENTIAL
 PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2008 Sony Computer Entertainment Inc.
 * All Rights Reserved.
 */

#ifndef _INCLUDE_MP3_ADAPTER_H
#define _INCLUDE_MP3_ADAPTER_H

#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

typedef struct {
    int32_t bw_pcm;
} CellAdecParamMP3;

#define MP3_BW_FLOAT   (4)
#define MP3_BW_16BIT   (3)
#define MP3_BW_18BIT   (2) // not supported
#define MP3_BW_20BIT   (1) // not supported
#define MP3_BW_24BIT   (0) // not supported

#define CELL_ADEC_MP3_WORD_SZ_16BIT MP3_BW_16BIT
#define CELL_ADEC_MP3_WORD_SZ_FLOAT MP3_BW_FLOAT

typedef struct {
    volatile unsigned int  ui_header;           /* MPEG Header 値 */
    volatile unsigned int  ui_main_data_begin;  /* main_data_begin の値 */
    volatile unsigned int  ui_main_data_remain_size; /* 残っている main_data のサイズ */
    volatile unsigned int  ui_main_data_now_size;    /* 今回読み込まれる main_data のサイズ */
    volatile unsigned char uc_crc;              /* CRC あり/なし(0=あり,1=なし) */
    volatile unsigned char uc_mode;             /* channel mode (00b:stereo
                                                   01b:joint stereo
                                                   10b:dual
                                                   11b:mono) */
    volatile unsigned char uc_mode_extension;   /* mode extension */
    volatile unsigned char uc_copyright;        /* copyright 情報 */
    volatile unsigned char uc_original;         /* original 情報 */
    volatile unsigned char uc_emphasis;         /* emphasis 情報 */
    volatile unsigned char uc_crc_error_flag;   /* CRC が一致しなかった場合，ON */
    volatile int i_error_code;                  /* Added at 2006.1.20 */
} CellAdecMP3Info;


/*==========================================================================
  Eeeor code for error callback
  ==========================================================================*/
#define CELL_ADEC_ERROR_MP3_OFFSET (0x80612700U)

#define CELL_ADEC_ERROR_MP3_OK                  CELL_ERROR_CAST(0x80612700)

/* Adapter level error */
#define CELL_ADEC_ERROR_MP3_BUSY                CELL_ERROR_CAST(0x80612764)
#define CELL_ADEC_ERROR_MP3_EMPTY               CELL_ERROR_CAST(0x80612765)

/* Core level error */
#define CELL_ADEC_ERROR_MP3_ERROR               CELL_ERROR_CAST(0x80612781)
#define CELL_ADEC_ERROR_MP3_LOST_SYNC           CELL_ERROR_CAST(0x80612782)
#define CELL_ADEC_ERROR_MP3_NOT_L3              CELL_ERROR_CAST(0x80612783)
#define CELL_ADEC_ERROR_MP3_BAD_BITRATE         CELL_ERROR_CAST(0x80612784)
#define CELL_ADEC_ERROR_MP3_BAD_SFREQ           CELL_ERROR_CAST(0x80612785)
#define CELL_ADEC_ERROR_MP3_BAD_EMPHASIS        CELL_ERROR_CAST(0x80612786)
#define CELL_ADEC_ERROR_MP3_BAD_BLKTYPE         CELL_ERROR_CAST(0x80612787)
#define CELL_ADEC_ERROR_MP3_BAD_VERSION         CELL_ERROR_CAST(0x8061278c)
#define CELL_ADEC_ERROR_MP3_BAD_MODE            CELL_ERROR_CAST(0x8061278d)
#define CELL_ADEC_ERROR_MP3_BAD_MODE_EXT        CELL_ERROR_CAST(0x8061278e)
#define CELL_ADEC_ERROR_MP3_HUFFMAN_NUM         CELL_ERROR_CAST(0x80612796)
#define CELL_ADEC_ERROR_MP3_HUFFMAN_CASE_ID     CELL_ERROR_CAST(0x80612797)
#define CELL_ADEC_ERROR_MP3_SCALEFAC_COMPRESS   CELL_ERROR_CAST(0x80612798)
#define CELL_ADEC_ERROR_MP3_HGETBIT             CELL_ERROR_CAST(0x80612799)
#define CELL_ADEC_ERROR_MP3_FLOATING_EXCEPTION  CELL_ERROR_CAST(0x8061279a)
#define CELL_ADEC_ERROR_MP3_ARRAY_OVERFLOW      CELL_ERROR_CAST(0x8061279b)
#define CELL_ADEC_ERROR_MP3_STEREO_PROCESSING   CELL_ERROR_CAST(0x8061279c)
#define CELL_ADEC_ERROR_MP3_JS_BOUND            CELL_ERROR_CAST(0x8061279d)
#define CELL_ADEC_ERROR_MP3_PCMOUT              CELL_ERROR_CAST(0x8061279e)

#if defined(__cplusplus)
}
#endif /* defined(__cplusplus) */

#endif /* _INCLUDE_MP3_ADAPTER_H */
