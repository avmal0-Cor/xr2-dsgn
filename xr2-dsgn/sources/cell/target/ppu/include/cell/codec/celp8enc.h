/* SCE CONFIDENTIAL
 PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2008 Sony Computer Entertainment Inc.
 * All Rights Reserved.
 */

#ifndef  __CELL_CELP8ENC_H__
#define  __CELL_CELP8ENC_H__

#include <stdbool.h>
#include <stdint.h>
#include <cell/error.h>
#include <cell/spurs.h>

#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*
    CELL_ERROR_FACILITY_CODEC               0x061
        libcelp8enc : 0x8061_40a1 - 0x8061_40bf
*/

#define CELL_CELP8ENC_ERROR_FAILED        CELL_ERROR_CAST(0x806140a1)
#define CELL_CELP8ENC_ERROR_SEQ           CELL_ERROR_CAST(0x806140a2)
#define CELL_CELP8ENC_ERROR_ARG           CELL_ERROR_CAST(0x806140a3)
#define CELL_CELP8ENC_ERROR_CORE_FAILED   CELL_ERROR_CAST(0x806140b1)
#define CELL_CELP8ENC_ERROR_CORE_SEQ      CELL_ERROR_CAST(0x806140b2)
#define CELL_CELP8ENC_ERROR_CORE_ARG      CELL_ERROR_CAST(0x806140b3)

enum CELL_CELP8ENC_SAMPEL_RATE{      /* Sampling Rate */
    CELL_CELP8ENC_FS_8kHz = 1,      /* Fs=8kHz */
};

typedef struct {
    uint32_t    workMemSize;        /* work mem size */
    uint32_t    celpEncVerUpper;    /* version info (upper) */
    uint32_t    celpEncVerLower;    /* version info (lower) */
}CellCelp8EncAttr;

typedef struct {
    uint32_t    totalMemSize;       /* total memory size */
    void        *startAddr;         /* start addr. */
    uint32_t    ppuThreadPriority;
    uint32_t    spuThreadPriority;
    size_t      ppuThreadStackSize;
}CellCelp8EncResource;

typedef struct {
    uint32_t    excitationMode;     /* Excitation Mode */
    uint32_t    sampleRate;         /* Sampling Frequency = Bandwidth */
    uint32_t    configuration;      /* Configuration */
    uint32_t    wordSize;           /* byte-width of PCM */
    uint8_t     *outBuff;           /* buffer for header output */
    uint32_t    outSize;            /* output size */
}CellCelp8EncParam;


enum CELL_CELP8ENC_EXCITATION_MODE{  /* Excitation Mode */
    CELL_CELP8ENC_EXCITATION_MODE_MPE = 0,
};

enum CELL_CELP8ENC_MPE_CONFIG{       /* MPE configuration */
    CELL_CELP8ENC_MPE_CONFIG_0  = 0, /* MPE0  */
    CELL_CELP8ENC_MPE_CONFIG_2  = 2, /* MPE2  */
    CELL_CELP8ENC_MPE_CONFIG_6  = 6, /* MPE6  */
    CELL_CELP8ENC_MPE_CONFIG_9  = 9, /* MPE9  */
    CELL_CELP8ENC_MPE_CONFIG_12 = 12,/* MPE12 */
    CELL_CELP8ENC_MPE_CONFIG_15 = 15,/* MPE15 */
    CELL_CELP8ENC_MPE_CONFIG_18 = 18,/* MPE18 */
    CELL_CELP8ENC_MPE_CONFIG_21 = 21,/* MPE21 */
    CELL_CELP8ENC_MPE_CONFIG_24 = 24,/* MPE24 */
    CELL_CELP8ENC_MPE_CONFIG_26 = 26 /* MPE26 */
};

enum CELL_CELP8ENC_WORD_SZ{          /* byte-width of PCM */
    CELL_CELP8ENC_WORD_SZ_FLOAT      /* 32bit float */
};

typedef struct {
    void                *startAddr;
    uint32_t            size;
}CellCelp8EncAuInfo;

typedef struct {
    void                *startAddr;
    uint32_t            size;
}CellCelp8EncPcmInfo;

typedef struct {
    uint32_t    totalMemSize;       /* total memory size */
    void        *startAddr;         /* start addr. */
    CellSpurs   *spurs;             /* SPURS instance addr. */
    uint8_t     priority[8];        /* SPURS taskset priority */
    uint32_t    maxContention;      /* SPURS taskset max contention */
}CellCelp8EncResourceEx;

typedef void *CellCelp8EncHandle;
/* ################################################## */

int32_t cellCelp8EncQueryAttr(
    CellCelp8EncAttr *attr
);

int32_t cellCelp8EncOpen(
    CellCelp8EncResource *res,
    CellCelp8EncHandle *handle
);

int32_t  cellCelp8EncClose(
    CellCelp8EncHandle handle
);

int32_t  cellCelp8EncStart(
    CellCelp8EncHandle handle,
    CellCelp8EncParam *param
);

int32_t  cellCelp8EncEnd(
    CellCelp8EncHandle handle
);

int32_t  cellCelp8EncEncodeFrame(
    CellCelp8EncHandle handle,
    CellCelp8EncPcmInfo *frameInfo
);

int32_t  cellCelp8EncGetAu(
    CellCelp8EncHandle handle,
    void *outBuffer,
    CellCelp8EncAuInfo *auItem
);

int32_t cellCelp8EncWaitForOutput(
    CellCelp8EncHandle handle
);

int32_t cellCelp8EncOpenEx(
    CellCelp8EncResourceEx *res,
    CellCelp8EncHandle *handle
);

#if defined(__cplusplus)
}
#endif /* defined(__cplusplus) */

#endif /* __CELL_CELP8ENC_H__ */
