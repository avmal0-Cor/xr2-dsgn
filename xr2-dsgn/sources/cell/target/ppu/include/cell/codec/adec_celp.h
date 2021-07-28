/* SCE CONFIDENTIAL
 PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2006 Sony Computer Entertainment Inc.
 * All Rights Reserved.
 */

#ifndef	 __CELL_ADEC_CELP_H__
#define	 __CELL_ADEC_CELP_H__

#include <stdint.h>
#include <cell/error.h>

#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

#define CELL_ADEC_ERROR_CELP_BUSY         CELL_ERROR_CAST(0x80612e01)
#define CELL_ADEC_ERROR_CELP_EMPTY        CELL_ERROR_CAST(0x80612e02)
#define CELL_ADEC_ERROR_CELP_ARG          CELL_ERROR_CAST(0x80612e03)
#define CELL_ADEC_ERROR_CELP_SEQ          CELL_ERROR_CAST(0x80612e04)
#define CELL_ADEC_ERROR_CELP_CORE_FATAL   CELL_ERROR_CAST(0x80612e81)
#define CELL_ADEC_ERROR_CELP_CORE_ARG     CELL_ERROR_CAST(0x80612e82)
#define CELL_ADEC_ERROR_CELP_CORE_SEQ     CELL_ERROR_CAST(0x80612e83)

enum CELL_ADEC_CELP_EXCITATION_MODE{	/* Excitation Mode */
	CELL_ADEC_CELP_EXCITATION_MODE_RPE = 1,
};

enum CELL_ADEC_CELP_RPE_CONFIG{		/* RPE configuration */
	CELL_ADEC_CELP_RPE_CONFIG_0,	/* RPE0 */
	CELL_ADEC_CELP_RPE_CONFIG_1,	/* RPE1 */
	CELL_ADEC_CELP_RPE_CONFIG_2,	/* RPE2 */
	CELL_ADEC_CELP_RPE_CONFIG_3,	/* RPE3 */
};

enum CELL_ADEC_CELP_WORD_SZ{		/* byte-width of PCM */
	CELL_ADEC_CELP_WORD_SZ_INT16_LE,/* 16bit signed integer(Little Endian) */
	CELL_ADEC_CELP_WORD_SZ_FLOAT	/* 32bit float */
};

/* CELP Parameters */
typedef struct {
	uint32_t	excitationMode;	/* Excitation Mode */
	uint32_t	sampleRate;		/* Sampling Frequency */
	uint32_t	configuration;	/* Configuration */
	uint32_t	wordSize;		/* byte-width of PCM */
}CellAdecParamCelp;

/* CELP BSI */
typedef struct {
	uint32_t	excitationMode;	/* Excitation Mode */
	uint32_t	sampleRate;		/* Sampling Frequency */
	uint32_t	configuration;	/* Configuration */
	uint32_t	wordSize;		/* byte-width of PCM */
}CellAdecCelpInfo;


#if defined(__cplusplus)
}
#endif /* defined(__cplusplus) */

#endif /* __CELL_ADEC_CELP_H__ */
