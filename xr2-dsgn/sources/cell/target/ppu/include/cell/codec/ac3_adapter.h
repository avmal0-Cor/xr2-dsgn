/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2006 Sony Computer Entertainment Inc. 
* All Rights Reserved.
*/

#ifndef __AC3_ADAPTER_H__
#define __AC3_ADAPTER_H__

#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

#define CELL_ADEC_ERROR_AC3_BUSY CELL_ERROR_CAST(0x80612500)
#define CELL_ADEC_ERROR_AC3_EMPTY CELL_ERROR_CAST(0x80612501)
#define CELL_ADEC_ERROR_AC3_PARAM CELL_ERROR_CAST(0x80612502)
#define CELL_ADEC_ERROR_AC3_FRAME CELL_ERROR_CAST(0x80612503)

enum CELL_ADEC_AC3_WORD_SZ {
	CELL_ADEC_AC3_WORD_SZ_INT16 = 0,
	CELL_ADEC_AC3_WORD_SZ_FLOAT = 1
};

enum CELL_ADEC_AC3_OUTPUT_MODE {
	CELL_ADEC_AC3_OUTPUT_MODE_RESERVED = 0,
	CELL_ADEC_AC3_OUTPUT_MODE_1_0 = 1,
	CELL_ADEC_AC3_OUTPUT_MODE_2_0 = 2,
	CELL_ADEC_AC3_OUTPUT_MODE_3_0 = 3,
	CELL_ADEC_AC3_OUTPUT_MODE_2_1 = 4,
	CELL_ADEC_AC3_OUTPUT_MODE_3_1 = 5,
	CELL_ADEC_AC3_OUTPUT_MODE_2_2 = 6,
	CELL_ADEC_AC3_OUTPUT_MODE_3_2 = 7
};

enum CELL_ADEC_AC3_LFE {
	CELL_ADEC_AC3_LFE_NOT_PRESENT = 0,
	CELL_ADEC_AC3_LFE_PRESENT = 1
};

enum CELL_ADEC_AC3_COMPRESSION_MODE {
	CELL_ADEC_AC3_COMPRESSION_MODE_CUSTOM_ANALOG = 0,
	CELL_ADEC_AC3_COMPRESSION_MODE_CUSTOM_DIGITAL = 1,
	CELL_ADEC_AC3_COMPRESSION_MODE_LINE_OUT = 2,
	CELL_ADEC_AC3_COMPRESSION_MODE_RF_REMOD = 3
};

enum CELL_ADEC_AC3_STEREO_MODE {
	CELL_ADEC_AC3_STEREO_MODE_AUTO_DETECT = 0,
	CELL_ADEC_AC3_STEREO_MODE_DOLBY_SURROUND_COMPATIBLE = 1,
	CELL_ADEC_AC3_STEREO_MODE_STEREO = 2
};

enum CELL_ADEC_AC3_DUALMONO_MODE {
	CELL_ADEC_AC3_DUALMONO_MODE_STEREO = 0,
	CELL_ADEC_AC3_DUALMONO_MODE_LEFT_MONO = 1,
	CELL_ADEC_AC3_DUALMONO_MODE_RIGHT_MONO = 2,
	CELL_ADEC_AC3_DUALMONO_MODE_MIXED_MONO = 3
};

enum CELL_ADEC_AC3_KARAOKE_CAPABLE_MODE {
	CELL_ADEC_AC3_KARAOKE_CAPABLE_MODE_NO_VOCAL = 0,
	CELL_ADEC_AC3_KARAOKE_CAPABLE_MODE_LEFT_VOCAL = 1,
	CELL_ADEC_AC3_KARAOKE_CAPABLE_MODE_RIGHT_VOCAL = 2,
	CELL_ADEC_AC3_KARAOKE_CAPABLE_MODE_BOTH_VOCAL = 3
};

enum CELL_ADEC_AC3_INPUT_CHANNEL {
	CELL_ADEC_AC3_INPUT_CHANNEL_L = 0,
	CELL_ADEC_AC3_INPUT_CHANNEL_C = 1,
	CELL_ADEC_AC3_INPUT_CHANNEL_R = 2,
	CELL_ADEC_AC3_INPUT_CHANNEL_l = 3,
	CELL_ADEC_AC3_INPUT_CHANNEL_r = 4,
	CELL_ADEC_AC3_INPUT_CHANNEL_s = 5
};

typedef struct {
    /* bit length of output PCM sample 
	   0: 16 bit integer
	   1: 32 bit float  */
    int8_t wordSize;

    /* Output channel configuration
       0: reserved
       1: 1/0 (C)
       2: 2/0 (L, R)
       3: 3/0 (L, C, R)
       4: 2/1 (L, R, l)
       5: 3/1 (L, C, R, l)
       6: 2/2 (L, R, l, r)
       7: 3/2 (L, C, R, l, r)  */
    int8_t outputMode;

    /* Output LFE channel present
       0: not present
       1:  present  */
    int8_t outLfeOn;

    /* Dynamic range compression cut scale factor  */
    float drcCutScaleFactor;

    /* Dynamic range compression boost scale factor  */
    float drcBoostScaleFactor;

    /* Dynamic range compression mode 
       0: custom mode, analog dialnorm
       1: custom mode, digital dialnorm
       2: line out mode
       3: RF remod mode  */
    int8_t compressionMode;

    /* number of output channel  */
    int8_t numberOfChannels;

    /* stereo output mode
       0: auto detect
       1: Dolby Surround compatible (Lt/Rt)
       2: stereo (Lo/Ro)  */
    int8_t stereoMode;

    /* dual mono reproduction mode
       0: stereo
       1: left mono
       2: right mono
       3: mixed mono  */
    int8_t dualmonoMode;

    /* karaoke capable reproduction mode
       0: no vocal
       1: left vocal
       2: right vocal
       3: both vocal  */
    int8_t karaokeCapableMode;

    /* PCM scale factor */
    float pcmScaleFactor;

    /* Channel routing information  */
    int32_t channelPointer0;
    int32_t channelPointer1;
    int32_t channelPointer2;
    int32_t channelPointer3;
    int32_t channelPointer4;
    int32_t channelPointer5;
} CellAdecParamAc3;

typedef struct {
	uint32_t codecType;
	uint32_t versionInfo;
	uint32_t totalCallCarryRun;
	uint32_t totalCallNum;
	uint32_t bitrateValue;
	uint32_t pcmSize;
	uint32_t esSizeBit;
	int32_t errorCode;
	uint8_t libInfoFlag;
	uint8_t validity;
	uint8_t channelValue;
	uint8_t fsIndex;
	uint8_t outputQuantization;
	uint8_t outputChannel;
	uint8_t monoFlag;
	uint32_t bsi[2];
	uint16_t frmSizeCod;
	uint8_t acmod;
	uint8_t lfeOn;
	uint8_t karaokeMode;
	uint8_t cmixlev;
	uint8_t surmixlev;
	uint8_t dsurmod;
	uint8_t copyright;
	uint8_t original;
	uint8_t bsmod;
	uint8_t bsid;
	uint8_t xbsi1e;
	uint8_t dmixmod;
	uint8_t xbsi2e;
	uint8_t dsurexmod;
	uint8_t dheadphonmod;
	uint8_t adconvtyp;
	uint8_t crcErrorFlag;
	uint8_t execDmixType;
} CellAdecBsiAc3;

#if defined(__cplusplus)
}
#endif /* defined(__cplusplus) */

#endif
