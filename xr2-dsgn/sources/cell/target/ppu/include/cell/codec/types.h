/* SCE CONFIDENTIAL
 PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2008 Sony Computer Entertainment Inc. 
 * All Rights Reserved.
 */  

#ifndef __CELL_CODEC_TYPES_H__
#define __CELL_CODEC_TYPES_H__

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define CELL_CODEC_PTS_INVALID  0xffffffff

#define CELL_CODEC_DTS_INVALID  0xffffffff

typedef struct CellCodecTimeStamp {
	uint32_t upper;
	uint32_t lower;
} CellCodecTimeStamp;

typedef struct CellCodecEsFilterId {
	uint32_t filterIdMajor;
	uint32_t filterIdMinor;
	uint32_t supplementalInfo1;
	uint32_t supplementalInfo2;
} CellCodecEsFilterId;

#ifdef __cplusplus
}
#endif

#endif /*__CELL_CODEC_TYPES_H__*/
