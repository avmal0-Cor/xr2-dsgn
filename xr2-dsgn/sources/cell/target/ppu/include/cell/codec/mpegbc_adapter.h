/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2007 Sony Computer Entertainment Inc. 
* All Rights Reserved.
*/
/*---------- Maintenance history -------------------------------------------*
 * Date     Author      Comment
 * 07/04/23 Y.Kitano    <MS-0004206> fix how to define error code
 *--------------------------------------------------------------------------*/

#ifndef	 __MPEG2BC_ADAPTER_H__
#define	 __MPEG2BC_ADAPTER_H__

#include <cell/error.h>

#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

#ifndef __MPEG2BCL1_ADAPTER_H__
/****************************/
/* Codec parameters			*/
/****************************/
typedef struct {
	uint32_t	channelNumber;		/* Extention bit stream present	1=exsist*/
	uint32_t	downmix;			/* DownMix 1 = 2ch downmix is done	*/
	uint32_t	lfeUpSample;		/* LFE upsampling mode 0=not done, used for only test	*/
} CellAdecParamMpmc;

/****************************/
/* BSI structure			*/
/****************************/
typedef struct {
	uint32_t	channelNumber;			/* decoded channel number		*/
	uint32_t	sampleFreq;				/* sampling frequency			*/
	uint32_t	errorPprotection;		/* CRC check data present		*/
	uint32_t	bitrateIndex;			/* Bitrate index value			*/
	uint32_t	stereoMode;				/* Stereo mode					*/
	uint32_t	stereoModeEextention;	/* Extention stereo mode		*/
	uint32_t	emphasis;				/* Emphasis mode				*/
	uint32_t	copyright;				/* Copyright					*/
	uint32_t	original;				/* Original or copy				*/
	uint32_t	surroundMode;			/* Sourround mode				*/
	uint32_t	centerMode;				/* Center mode					*/
	uint32_t	audioMmix;				/* Audio signal mix mode		*/
	uint32_t	outputFramSize;			/* Output frame size per 1 fram	*/
	uint32_t	multiCodecMode;			/* Multi codec data present		*/
	uint32_t	lfePresent;				/* LFE data present				*/
	uint32_t	channelCoufiguration;	/* Channel coufiguration		*/
} CellAdecMpmcInfo;
#endif //__MPEG2BCL1_ADAPTER_H__
/********************************************************************************/
/* Difinitions for BSI (start)													*/
/********************************************************************************/
/****************************/
/* Sampling frequency		*/
/****************************/
#define CELL_ADEC_BSI_M2BC_SAMPLE_FREQUENCY_44				0	/* 44.1kHz		*/
#define CELL_ADEC_BSI_M2BC_SAMPLE_FREQUENCY_48				1	/* 48kHz		*/
#define CELL_ADEC_BSI_M2BC_SAMPLE_FREQUENCY_32				2	/* 32kHz		*/

/****************************/
/* Error protection			*/
/****************************/
#define CELL_ADEC_BSI_M2BC_ERROR_PROTECTION_NONE			0	/* none			*/
#define CELL_ADEC_BSI_M2BC_ERROR_PROTECTION_EXIST			1	/* exist		*/

/****************************/
/* Bitrate Index			*/
/****************************/
#define CELL_ADEC_BSI_M2BC_BITRATE_32						1	/* 32kbit/s		*/
#define CELL_ADEC_BSI_M2BC_BITRATE_48						2	/* 48kbit/s		*/
#define CELL_ADEC_BSI_M2BC_BITRATE_56						3	/* 56kbit/s		*/
#define CELL_ADEC_BSI_M2BC_BITRATE_64						4	/* 64kbit/s		*/
#define CELL_ADEC_BSI_M2BC_BITRATE_80						5	/* 80kbit/s		*/
#define CELL_ADEC_BSI_M2BC_BITRATE_96						6	/* 96kbit/s		*/
#define CELL_ADEC_BSI_M2BC_BITRATE_112						7	/* 112kbit/s	*/
#define CELL_ADEC_BSI_M2BC_BITRATE_128						8	/* 128kbit/s	*/
#define CELL_ADEC_BSI_M2BC_BITRATE_160						9	/* 160kbit/s	*/
#define CELL_ADEC_BSI_M2BC_BITRATE_192						10	/* 192kbit/s	*/
#define CELL_ADEC_BSI_M2BC_BITRATE_224						11	/* 224kbit/s	*/
#define CELL_ADEC_BSI_M2BC_BITRATE_256						12	/* 256kbit/s	*/
#define CELL_ADEC_BSI_M2BC_BITRATE_320						13	/* 320kbit/s	*/
#define CELL_ADEC_BSI_M2BC_BITRATE_384						14	/* 384kbit/s	*/

/****************************/
/* Stereo Mode				*/
/****************************/
#define CELL_ADEC_BSI_M2BC_STEREO_MODE_STERO				0	/* Stereo		*/
#define CELL_ADEC_BSI_M2BC_STEREO_MODE_JOINTSTERO			1	/* Joint Stereo	*/
#define CELL_ADEC_BSI_M2BC_STEREO_MODE_DUAL					2	/* Dual channel	*/
#define CELL_ADEC_BSI_M2BC_STEREO_MODE_MONO					3	/* Mono			*/

/****************************/
/* Stereo Mode Extension	*/
/****************************/
#define CELL_ADEC_BSI_M2BC_STEREO_EXMODE_0					0	/* subbands 4-31*/
#define CELL_ADEC_BSI_M2BC_STEREO_EXMODE_1					1	/* subbands 8-31*/
#define CELL_ADEC_BSI_M2BC_STEREO_EXMODE_2					2	/* subbands12-31*/
#define CELL_ADEC_BSI_M2BC_STEREO_EXMODE_3					3	/* subbands16-31*/

/****************************/
/* Emphasis					*/
/****************************/
#define CELL_ADEC_BSI_M2BC_EMPHASIS_NONE					0	/* none			*/
#define CELL_ADEC_BSI_M2BC_EMPHASIS_50_15					1	/* 50/15ms		*/
#define CELL_ADEC_BSI_M2BC_EMPHASIS_CCITT					3	/* CCITT J.17	*/

/****************************/
/* Copyright bit			*/
/****************************/
#define CELL_ADEC_BSI_M2BC_COPYRIGHT_NONE					0	/* none			*/
#define CELL_ADEC_BSI_M2BC_COPYRIGHT_ON						0	/* protected	*/

/****************************/
/* Original/Copy			*/
/****************************/
#define CELL_ADEC_BSI_M2BC_ORIGINAL_COPY					0	/* copy			*/
#define CELL_ADEC_BSI_M2BC_ORIGINAL_ORIGINAL				1	/* original		*/

/****************************/
/* Surround Mode			*/
/****************************/
#define CELL_ADEC_BSI_M2BC_SURROUND_NONE					0	/* none			*/
#define CELL_ADEC_BSI_M2BC_SURROUND_MONO					1	/* mono			*/
#define CELL_ADEC_BSI_M2BC_SURROUND_STEREO					2	/* stereo		*/
#define CELL_ADEC_BSI_M2BC_SURROUND_SECOND					3	/* second stereo*/

/****************************/
/* Center Mode				*/
/****************************/
#define CELL_ADEC_BSI_M2BC_CENTER_NONE						0	/* none			*/
#define CELL_ADEC_BSI_M2BC_CENTER_EXIST						1	/* exist		*/
#define CELL_ADEC_BSI_M2BC_CENTER_FHANTOM					3	/* fhantom coding*/

/****************************/
/* LFE Present				*/
/****************************/
#define CELL_ADEC_BSI_M2BC_LFE_NONE							0	/* none			*/
#define CELL_ADEC_BSI_M2BC_LFE_EXIST						1	/* exist		*/

/****************************/
/* Audio Mix Mode			*/
/****************************/
#define CELL_ADEC_BSI_M2BC_AUDIOMIX_LARGE					0	/* Large listerning room */
#define CELL_ADEC_BSI_M2BC_AUDIOMIX_SMALLE					1	/* Small listerning room */

/****************************/
/* MC extention data present*/
/****************************/
#define CELL_ADEC_BSI_M2BC_MCEXTENSION_2CH					0	/* for 2ch		*/
#define CELL_ADEC_BSI_M2BC_MCEXTENSION_5CH					1	/* for 5ch		*/
#define CELL_ADEC_BSI_M2BC_MCEXTENSION_7CH					2	/* for 7ch		*/

/****************************/
/* Channel configuration	*/
/****************************/
#define CELL_ADEC_BSI_M2BC_CH_CONFIG_MONO					0	/* 1			*/
#define CELL_ADEC_BSI_M2BC_CH_CONFIG_DUAL					1	/* 1 + 1		*/
#define CELL_ADEC_BSI_M2BC_CH_CONFIG_L_R					2	/* 2/0			*/
#define CELL_ADEC_BSI_M2BC_CH_CONFIG_L_R_S					3	/* 2/1			*/
#define CELL_ADEC_BSI_M2BC_CH_CONFIG_L_R_C					4	/* 3/0			*/
#define CELL_ADEC_BSI_M2BC_CH_CONFIG_L_R_LS_RS				5	/* 2/2			*/
#define CELL_ADEC_BSI_M2BC_CH_CONFIG_L_R_C_S				6	/* 3/1			*/
#define CELL_ADEC_BSI_M2BC_CH_CONFIG_L_R_C_LS_RS			7	/* 3/2			*/
#define CELL_ADEC_BSI_M2BC_CH_CONFIG_LL_RR_CC_LS_RS_LC_RC	8	/* 5/2			*/
#define CELL_ADEC_BSI_M2BC_CH_CONFIG_MONO_SECONDSTEREO		9	/* 1 + 2		*/
#define CELL_ADEC_BSI_M2BC_CH_CONFIG_L_R_SECONDSTEREO		10	/* 2 + 2		*/
#define CELL_ADEC_BSI_M2BC_CH_CONFIG_L_R_C_SECONDSTEREO		11	/* 3 + 2		*/
/********************************************************************************/
/* Difinitions for BSI (end)													*/
/********************************************************************************/

/********************************************************************************/
/* Error code definitions														*/
/********************************************************************************/
#define CELL_ADEC_ERROR_M2BC_FATAL			CELL_ERROR_CAST(0x80612b01)
#define CELL_ADEC_ERROR_M2BC_SEQ			CELL_ERROR_CAST(0x80612b02)	/* Not Used */
#define CELL_ADEC_ERROR_M2BC_ARG			CELL_ERROR_CAST(0x80612b03)
#define CELL_ADEC_ERROR_M2BC_BUSY			CELL_ERROR_CAST(0x80612b04)
#define CELL_ADEC_ERROR_M2BC_EMPTY			CELL_ERROR_CAST(0x80612b05)

/* Common header contents error */
#define CELL_ADEC_ERROR_M2BC_SYNCF			CELL_ERROR_CAST(0x80612b11)
#define CELL_ADEC_ERROR_M2BC_LAYER			CELL_ERROR_CAST(0x80612b12)
#define CELL_ADEC_ERROR_M2BC_BITRATE		CELL_ERROR_CAST(0x80612b13)
#define CELL_ADEC_ERROR_M2BC_SAMPLEFREQ		CELL_ERROR_CAST(0x80612b14)
#define CELL_ADEC_ERROR_M2BC_VERSION		CELL_ERROR_CAST(0x80612b15)
#define CELL_ADEC_ERROR_M2BC_MODE_EXT		CELL_ERROR_CAST(0x80612b16)	/* Not Used */
#define CELL_ADEC_ERROR_M2BC_UNSUPPORT		CELL_ERROR_CAST(0x80612b17)	/* <MS-2323>*/

/* Extension file header contents error */
#define CELL_ADEC_ERROR_M2BC_OPENBS_EX		CELL_ERROR_CAST(0x80612b21)
#define CELL_ADEC_ERROR_M2BC_SYNCF_EX		CELL_ERROR_CAST(0x80612b22)
#define CELL_ADEC_ERROR_M2BC_CRCGET_EX		CELL_ERROR_CAST(0x80612b23)
#define CELL_ADEC_ERROR_M2BC_CRC_EX			CELL_ERROR_CAST(0x80612b24)

/* Data sampling & CRC error (proper to MPEG I part) */
#define CELL_ADEC_ERROR_M2BC_CRCGET			CELL_ERROR_CAST(0x80612b31)
#define CELL_ADEC_ERROR_M2BC_CRC			CELL_ERROR_CAST(0x80612b32)
#define CELL_ADEC_ERROR_M2BC_BITALLOC		CELL_ERROR_CAST(0x80612b33)
#define CELL_ADEC_ERROR_M2BC_SCALE			CELL_ERROR_CAST(0x80612b34)
#define CELL_ADEC_ERROR_M2BC_SAMPLE			CELL_ERROR_CAST(0x80612b35)
#define CELL_ADEC_ERROR_M2BC_OPENBS			CELL_ERROR_CAST(0x80612b36)

/* Data sampling & CRC error (MPEG II muliti codec part) */
#define CELL_ADEC_ERROR_M2BC_MC_CRCGET		CELL_ERROR_CAST(0x80612b41)
#define CELL_ADEC_ERROR_M2BC_MC_CRC			CELL_ERROR_CAST(0x80612b42)
#define CELL_ADEC_ERROR_M2BC_MC_BITALLOC	CELL_ERROR_CAST(0x80612b43)
#define CELL_ADEC_ERROR_M2BC_MC_SCALE		CELL_ERROR_CAST(0x80612b44)
#define CELL_ADEC_ERROR_M2BC_MC_SAMPLE		CELL_ERROR_CAST(0x80612b45)
#define CELL_ADEC_ERROR_M2BC_MC_HEADER		CELL_ERROR_CAST(0x80612b46)
#define CELL_ADEC_ERROR_M2BC_MC_STATUS		CELL_ERROR_CAST(0x80612b47)

/* Data sampling & CRC error (MPEG II extention muliti codec part) */
#define CELL_ADEC_ERROR_M2BC_AG_CCRCGET		CELL_ERROR_CAST(0x80612b51)
#define CELL_ADEC_ERROR_M2BC_AG_CRC			CELL_ERROR_CAST(0x80612b52)
#define CELL_ADEC_ERROR_M2BC_AG_BITALLOC	CELL_ERROR_CAST(0x80612b53)
#define CELL_ADEC_ERROR_M2BC_AG_SCALE		CELL_ERROR_CAST(0x80612b54)
#define CELL_ADEC_ERROR_M2BC_AG_SAMPLE		CELL_ERROR_CAST(0x80612b55)
#define CELL_ADEC_ERROR_M2BC_AG_STATUS		CELL_ERROR_CAST(0x80612b57)

#if defined(__cplusplus)
}
#endif /* defined(__cplusplus) */

#endif /* __MPEG2BC_ADAPTER_H__ */
