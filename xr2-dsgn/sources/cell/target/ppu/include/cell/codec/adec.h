/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2005 Sony Computer Entertainment Inc. 
* All Rights Reserved.
*/
/******************************************************************
*	filename:a_dec.h
*	function:
*	auther:Koichi Chiba
*	history: 2005.01.31 v0.1.0.0 New
*			 2005.03.11 v0.1.0.1 modification enum SCE_ADEC_CHANNEL
*******************************************************************/

#ifndef	 __CELL_A_DEC_H__
#define	 __CELL_A_DEC_H__

#include "adec_base.h"
#include <stdint.h>
#include <cell/spurs.h>
#include <cell/codec/types.h>

#if CELL_DEC_CELL_LV2
#include <stdbool.h>
#else
#define bool int
#endif

#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */


enum CELL_ADEC_CORE_TYPE{			/* Audio Codec Type */
	CELL_ADEC_TYPE_RESERVED1,		/* reserved */
	CELL_ADEC_TYPE_LPCM_PAMF,		/* LPCM */
	CELL_ADEC_TYPE_AC3,				/* AC-3 */
	CELL_ADEC_TYPE_ATRACX,			/* ATRACX */
	CELL_ADEC_TYPE_MP3,				/* MP3 */
	CELL_ADEC_TYPE_ATRAC3,			/* ATRAX3 */
	CELL_ADEC_TYPE_MPEG_L2,			/* MPEG2 L2 */
	CELL_ADEC_TYPE_RESERVED5,		/* reserved */
	CELL_ADEC_TYPE_RESERVED6,		/* reserved */
	CELL_ADEC_TYPE_RESERVED7,		/* reserved */
	CELL_ADEC_TYPE_RESERVED8,		/* reserved */
	CELL_ADEC_TYPE_CELP,			/* CELP */
	CELL_ADEC_TYPE_RESERVED10,		/* reserved */
	CELL_ADEC_TYPE_ATRACX_2CH,		/* ATRACX 2ch */
	CELL_ADEC_TYPE_ATRACX_6CH,		/* ATRACX 6ch */
	CELL_ADEC_TYPE_ATRACX_8CH,		/* ATRACX 8ch */
	CELL_ADEC_TYPE_M4AAC,			/* MPEG4 AAC */
	CELL_ADEC_TYPE_RESERVED12,		/* reserved */
	CELL_ADEC_TYPE_RESERVED13,		/* reserved */
	CELL_ADEC_TYPE_RESERVED14,		/* reserved */
	CELL_ADEC_TYPE_RESERVED15,		/* reserved */
	CELL_ADEC_TYPE_RESERVED16,		/* reserved */
	CELL_ADEC_TYPE_RESERVED17,		/* reserved */
	CELL_ADEC_TYPE_RESERVED18,		/* reserved */
	CELL_ADEC_TYPE_RESERVED19,		/* reserved */
	CELL_ADEC_TYPE_CELP8,			/* CELP8 */
	CELL_ADEC_TYPE_RESERVED20,		/* reserved */
	CELL_ADEC_TYPE_RESERVED21,		/* reserved */
	CELL_ADEC_TYPE_RESERVED22,		/* reserved */
	CELL_ADEC_TYPE_RESERVED23,		/* reserved */
};

enum CELL_ADEC_CHANNEL{			/* Audio Codec output channel number */
	CELL_ADEC_CH_RESERVED1,		/* reserved */
	CELL_ADEC_CH_MONO,			/* mono(1/0) */
	CELL_ADEC_CH_RESERVED2,		/* reserved */
	CELL_ADEC_CH_STEREO,		/* stereo(2/0) */
	CELL_ADEC_CH_3_0,			/* 3/0 mode */
	CELL_ADEC_CH_2_1,			/* 2/1 mode */
	CELL_ADEC_CH_3_1,			/* 3/1 mode */
	CELL_ADEC_CH_2_2,			/* 2/2 mode */
	CELL_ADEC_CH_3_2,			/* 3/2 mode */
	CELL_ADEC_CH_3_2_LFE,		/* 3/2+LFE mode */
	CELL_ADEC_CH_3_4,			/* 3/4 mode */
	CELL_ADEC_CH_3_4_LFE,		/* 3/4+LFE */
	CELL_ADEC_CH_RESERVED3,		/* reserved */
};	

enum CELL_ADEC_SAMPEL_RATE{		/* Sampling Rate */
	CELL_ADEC_FS_RESERVED1 = 0,	/* reserved */
	CELL_ADEC_FS_48kHz = 1,		/* Fs=48kHz */
	CELL_ADEC_FS_16kHz = 2,		/* Fs=16kHz */
	CELL_ADEC_FS_8kHz = 5,		/* Fs=8kHz */
};

enum CELL_ADEC_BIT_LENGTH{			/* Bit per word */
	CELL_ADEC_BIT_LENGTH_RESERVED1,	/* reserved */
	CELL_ADEC_BIT_LENGTH_16,		/* 16bit/sample */
	CELL_ADEC_BIT_LENGTH_20,		/* 20bit/sample */
	CELL_ADEC_BIT_LENGTH_24,		/* 24bit/sample */
};

typedef struct {
	uint32_t					audioCodecType;		/* Audio Codec Type */
}CellAdecType;


typedef struct {
	uint32_t	workMemSize;		/* work mem size */
	uint32_t	adecVerUpper;		/* version info (upper) */
	uint32_t	adecVerLower;		/* version info (lower) */
}CellAdecAttr;

typedef struct {
	uint32_t		totalMemSize;	/* total memory size */
	void			*startAddr;		/* start addr. */
	uint32_t		ppuThreadPriority;
	uint32_t		spuThreadPriority;
	size_t			ppuThreadStackSize;
}CellAdecResource;

typedef enum {
	CELL_ADEC_MSG_TYPE_AUDONE,
	CELL_ADEC_MSG_TYPE_PCMOUT,
	CELL_ADEC_MSG_TYPE_ERROR,
	CELL_ADEC_MSG_TYPE_SEQDONE,
}CellAdecMsgType;	

typedef void *CellAdecHandle;

/* Callback Function ################################ */

typedef int32_t (*CellAdecCbMsg)(
	CellAdecHandle	handle,
	CellAdecMsgType	msgType,
	int32_t			msgData,
	void 			*callbackArg
);
/* ################################################## */

/* for callback function */
typedef struct {
	CellAdecCbMsg	callbackFunc;			/* pointer to callback function */
	void			*callbackArg;			/* pointer to callback arg. */
}CellAdecCb;

typedef CellCodecTimeStamp CellAdecTimeStamp;

/* AU informateion */
typedef struct {
	void					*startAddr;		/* opinter to AU data */
	uint32_t				size;		/* AU date size */
	CellCodecTimeStamp		pts;		/* PTS */
	uint64_t				userData;	/* user data */
}CellAdecAuInfo;

/* bsi infomation */
typedef struct {
	void			*bsiInfo;			/* pointer to BSI*/
}CellAdecPcmAttr;

typedef struct {
	uint32_t				pcmHandle;			/* handle */
	uint32_t				status;				/* status */
	void					*startAddr;		/* pointer to PCM data */
	uint32_t				size;				/* PCM data size */
	CellAdecPcmAttr		pcmAttr;			/* PCM attribute */
	CellAdecAuInfo		auInfo;			/* AU Information */
}CellAdecPcmItem;

typedef struct {
	uint32_t					channelNumber;		/* Number of output channels */
	uint32_t					sampleRate;		/* Sampling Frequency */
	uint32_t					sizeOfWord;		/* Quantization */
	uint32_t					audioPayloadSize;	/* Audio payload asize */
} CellAdecParamLpcm;

/* LPCM BSI */
typedef struct {
	uint32_t	channelNumber;		/* channel num. */
	uint32_t	sampleRate;		/* Fs */
	uint32_t	outputDataSize;	/* output data size(byte) */
}CellAdecLpcmInfo;

/* ResourceEx */
typedef struct {
	uint32_t	totalMemSize;	/* total memory size */
	void		*startAddr;		/* start addr. */
	uint32_t	ppuThreadPriority;
	size_t		ppuThreadStackSize;
	CellSpurs	*spurs;
	uint8_t		priority[8];
	uint32_t	maxContention;
}CellAdecResourceEx;

int32_t		cellAdecQueryAttr(
	CellAdecType		*type,
	CellAdecAttr		*mem_size
);

int32_t	cellAdecOpen(
	CellAdecType		*type,
	CellAdecResource	*res,
	CellAdecCb			*cb,
	CellAdecHandle		*handle
);

int32_t		cellAdecClose(
	CellAdecHandle		handle
);

int32_t		cellAdecStartSeq(
	CellAdecHandle		handle,
	void				*param
);

int32_t		cellAdecEndSeq(
	CellAdecHandle		handle
);

int32_t		cellAdecDecodeAu(
	CellAdecHandle		handle,
	CellAdecAuInfo		*auInfo
);

int32_t		cellAdecGetPcm(
	CellAdecHandle		handle,
	void				*outBuff
);

int32_t cellAdecGetPcmItem(
	CellAdecHandle			handle,
	const CellAdecPcmItem	**pcmItem
);

int32_t cellAdecOpenEx( CellAdecType *type,
						  CellAdecResourceEx *res_ex,
						  CellAdecCb *cb,
						  CellAdecHandle *handle
);

#if defined(__cplusplus)
}
#endif /* defined(__cplusplus) */

#endif /* __CELL_A_DEC_H__ */
