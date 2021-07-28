/* SCE CONFIDENTIAL
 PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2008 Sony Computer Entertainment Inc.
 * All Rights Reserved.
 */

#ifndef  __CELL_ADEC_CELP8_H__
#define  __CELL_ADEC_CELP8_H__

#include <stdint.h>
#include <cell/error.h>

#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

#define CELL_ADEC_ERROR_CELP8_BUSY         CELL_ERROR_CAST(0x80612ea1)
#define CELL_ADEC_ERROR_CELP8_EMPTY        CELL_ERROR_CAST(0x80612ea2)
#define CELL_ADEC_ERROR_CELP8_ARG          CELL_ERROR_CAST(0x80612ea3)
#define CELL_ADEC_ERROR_CELP8_SEQ          CELL_ERROR_CAST(0x80612ea4)
#define CELL_ADEC_ERROR_CELP8_CORE_FATAL   CELL_ERROR_CAST(0x80612eb1)
#define CELL_ADEC_ERROR_CELP8_CORE_ARG     CELL_ERROR_CAST(0x80612eb2)
#define CELL_ADEC_ERROR_CELP8_CORE_SEQ     CELL_ERROR_CAST(0x80612eb3)

enum CELL_ADEC_CELP8_EXCITATION_MODE{   /* Excitation Mode */
    CELL_ADEC_CELP8_EXCITATION_MODE_MPE = 0,
};

enum CELL_ADEC_CELP8_MPE_CONFIG{        /* MPE configuration */
    CELL_ADEC_CELP8_MPE_CONFIG_0  = 0,  /* MPE0  */
    CELL_ADEC_CELP8_MPE_CONFIG_2  = 2,  /* MPE2  */
    CELL_ADEC_CELP8_MPE_CONFIG_6  = 6,  /* MPE6  */
    CELL_ADEC_CELP8_MPE_CONFIG_9  = 9,  /* MPE9  */
    CELL_ADEC_CELP8_MPE_CONFIG_12 = 12, /* MPE12 */
    CELL_ADEC_CELP8_MPE_CONFIG_15 = 15, /* MPE15 */
    CELL_ADEC_CELP8_MPE_CONFIG_18 = 18, /* MPE18 */
    CELL_ADEC_CELP8_MPE_CONFIG_21 = 21, /* MPE21 */
    CELL_ADEC_CELP8_MPE_CONFIG_24 = 24, /* MPE24 */
    CELL_ADEC_CELP8_MPE_CONFIG_26 = 26  /* MPE26 */
};

enum CELL_ADEC_CELP8_WORD_SZ{       /* byte-width of PCM */
    CELL_ADEC_CELP8_WORD_SZ_FLOAT   /* 32bit float */
};

/* CELP8 Parameters */
typedef struct {
    uint32_t    excitationMode; /* Excitation Mode */
    uint32_t    sampleRate;     /* Sampling Frequency */
    uint32_t    configuration;  /* Configuration */
    uint32_t    wordSize;       /* byte-width of PCM */
}CellAdecParamCelp8;

/* CELP8 BSI */
typedef struct {
    uint32_t    excitationMode; /* Excitation Mode */
    uint32_t    sampleRate;     /* Sampling Frequency */
    uint32_t    configuration;  /* Configuration */
    uint32_t    wordSize;       /* byte-width of PCM */
}CellAdecCelp8Info;


#if defined(__cplusplus)
}
#endif /* defined(__cplusplus) */

#endif /* __CELL_ADEC_CELP8_H__ */
