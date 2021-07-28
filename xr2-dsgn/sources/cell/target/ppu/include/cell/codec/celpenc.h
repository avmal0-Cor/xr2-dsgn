/* SCE CONFIDENTIAL
 PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2006 Sony Computer Entertainment Inc.
 * All Rights Reserved.
 */

#ifndef  __CELL_CELPENC_H__
#define  __CELL_CELPENC_H__

#include <stdbool.h>
#include <stdint.h>
#include <cell/error.h>
#include <cell/spurs.h>

#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*
    CELL_ERROR_FACILITY_CODEC               0x061
        libcelpenc  : 0x8061_4001 - 0x8061_40ff
*/

#define CELL_CELPENC_ERROR_FAILED        CELL_ERROR_CAST(0x80614001)
#define CELL_CELPENC_ERROR_SEQ           CELL_ERROR_CAST(0x80614002)
#define CELL_CELPENC_ERROR_ARG           CELL_ERROR_CAST(0x80614003)
#define CELL_CELPENC_ERROR_CORE_FAILED   CELL_ERROR_CAST(0x80614081)
#define CELL_CELPENC_ERROR_CORE_SEQ      CELL_ERROR_CAST(0x80614082)
#define CELL_CELPENC_ERROR_CORE_ARG      CELL_ERROR_CAST(0x80614083)


enum CELL_CELPENC_SAMPEL_RATE{      /* Sampling Rate */
    CELL_CELPENC_FS_16kHz = 2,      /* Fs=16kHz */
};

typedef struct {
    uint32_t    workMemSize;        /* work mem size */
    uint32_t    celpEncVerUpper;    /* version info (upper) */
    uint32_t    celpEncVerLower;    /* version info (lower) */
}CellCelpEncAttr;

typedef struct {
    uint32_t    totalMemSize;       /* total memory size */
    void        *startAddr;         /* start addr. */
    uint32_t    ppuThreadPriority;
    uint32_t    spuThreadPriority;
    size_t      ppuThreadStackSize;
}CellCelpEncResource;

typedef struct {
    uint32_t    excitationMode;     /* Excitation Mode */
    uint32_t    sampleRate;         /* Sampling Frequency = Bandwidth */
    uint32_t    configuration;      /* Configuration */
    uint32_t    wordSize;           /* byte-width of PCM */
    uint8_t     *outBuff;           /* buffer for header output */
    uint32_t    outSize;            /* output size */
}CellCelpEncParam;


enum CELL_CELPENC_EXCITATION_MODE{  /* Excitation Mode */
    CELL_CELPENC_EXCITATION_MODE_RPE = 1,
};

enum CELL_CELPENC_RPE_CONFIG{       /* RPE configuration */
    CELL_CELPENC_RPE_CONFIG_0,      /* RPE0 */
    CELL_CELPENC_RPE_CONFIG_1,      /* RPE1 */
    CELL_CELPENC_RPE_CONFIG_2,      /* RPE2 */
    CELL_CELPENC_RPE_CONFIG_3,      /* RPE3 */
};

enum CELL_CELPENC_WORD_SZ{          /* byte-width of PCM */
    CELL_CELPENC_WORD_SZ_INT16_LE,  /* 16bit signed integer(Little Endian) */
    CELL_CELPENC_WORD_SZ_FLOAT      /* 32bit float */
};

typedef struct {
    void                *startAddr;
    uint32_t            size;
}CellCelpEncAuInfo;

typedef struct {
    void                *startAddr;
    uint32_t            size;
}CellCelpEncPcmInfo;

typedef struct {
    uint32_t    totalMemSize;       /* total memory size */
    void        *startAddr;         /* start addr. */
    CellSpurs   *spurs;             /* SPURS instance addr. */
    uint8_t     priority[8];        /* SPURS taskset priority */
    uint32_t    maxContention;      /* SPURS taskset max contention */
}CellCelpEncResourceEx;

typedef void *CellCelpEncHandle;
/* ################################################## */

int32_t cellCelpEncQueryAttr(
    CellCelpEncAttr *attr
);

int32_t cellCelpEncOpen(
    CellCelpEncResource *res,
    CellCelpEncHandle *handle
);

int32_t  cellCelpEncClose(
    CellCelpEncHandle handle
);

int32_t  cellCelpEncStart(
    CellCelpEncHandle handle,
    CellCelpEncParam *param
);

int32_t  cellCelpEncEnd(
    CellCelpEncHandle handle
);

int32_t  cellCelpEncEncodeFrame(
    CellCelpEncHandle handle,
    CellCelpEncPcmInfo *frameInfo
);

int32_t  cellCelpEncGetAu(
    CellCelpEncHandle handle,
    void *outBuffer,
    CellCelpEncAuInfo *auItem
);

int32_t cellCelpEncWaitForOutput(
    CellCelpEncHandle handle
);

int32_t cellCelpEncOpenEx(
    CellCelpEncResourceEx *res,
    CellCelpEncHandle *handle
);

#if defined(__cplusplus)
}
#endif /* defined(__cplusplus) */

#endif /* __CELL_CELPENC_H__ */
