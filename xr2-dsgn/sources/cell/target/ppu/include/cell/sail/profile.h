/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2009 Sony Computer Entertainment Inc.
 * All Rights Reserved.
 */

#ifndef __CELL_SAIL_PROFILE_H__
#define __CELL_SAIL_PROFILE_H__

#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
extern "C" {
#endif

enum {
    CELL_SAIL_ENCODER_AUDIO_UNSPECIFIED = -1,
    CELL_SAIL_ENCODER_VIDEO_UNSPECIFIED = -1,

    CELL_SAIL_ENCODER_AUDIO_M4AAC  = 0x01,

    CELL_SAIL_ENCODER_VIDEO_M4V    = 0x21,
    CELL_SAIL_ENCODER_VIDEO_AVC    = 0x22,
    CELL_SAIL_ENCODER_VIDEO_M4HD   = 0x24,
};



// common video encoder command
enum {
    CELL_SAIL_ENC_COMMAND_AVE_BITRATE        = 0,
    CELL_SAIL_ENC_COMMAND_MAX_BITRATE        = 1,
    CELL_SAIL_ENC_COMMAND_MAX_AU_SIZE        = 2,
    CELL_SAIL_ENC_COMMAND_ASPECT_RATIO_INFO  = 3,

    CELL_SAIL_ENC_COMMAND_M4V_VOS_VOL_HEADER = 5,
};


// m4v enc mode
enum {
    CELL_SAIL_M4V_ENC_MODE_UNSPECIFIED             = -1,
    CELL_SAIL_M4V_ENC_MODE_NO_DELAY                = 0x101,
    CELL_SAIL_M4V_ENC_MODE_NO_DELAY_VBV_CONSTRAINT = 0x102,
    CELL_SAIL_M4V_ENC_MODE_FAST                    = 0x111,
    CELL_SAIL_M4V_ENC_MODE_FAST_VBV_CONSTRAINT     = 0x112,
    CELL_SAIL_M4V_ENC_MODE_LONG_DELAY              = 0x200,
};

// m4v preset
enum {
    CELL_SAIL_M4V_ENC_PRESET_UNSPECIFIED = -1,
    CELL_SAIL_M4V_ENC_PRESET_ASP_L3   = 0x00, /* ASP@L3 QVGA  29.97P */
    CELL_SAIL_M4V_ENC_PRESET_ASP_L5   = 0x01, /* ASP@L5 VGA   29.97P */
    CELL_SAIL_M4V_ENC_PRESET_ASP_L5_W = 0x02, /* ASP@L5 SD    29.97P */
    CELL_SAIL_M4V_ENC_PRESET_ASP_L1   = 0x03, /* ASP@L1 QQVGA_29.97P */
    CELL_SAIL_M4V_ENC_PRESET_MP_L4    = 0x04, /* MP@L4  VGA   59.94P */

    CELL_SAIL_M4V_ENC_PRESET_SP_L3  = 0x10, /* SP@L3  QVGA  29.97P */
    CELL_SAIL_M4V_ENC_PRESET_SP_L4a = 0x11, /* SP@L4a VGA   29.97P */
    CELL_SAIL_M4V_ENC_PRESET_SP_L5  = 0x12, /* SP@L5  SD    29.97P */
    CELL_SAIL_M4V_ENC_PRESET_SP_L2  = 0x13, /* SP@L2  QQVGA 29.97P */
};

// m4v ex param
enum {
    CELL_SAIL_M4V_ENC_EX_PARAM_MAX_AU_SIZE = 32468,
    CELL_SAIL_M4V_ENC_EX_PARAM_VOS_HEADER  = 64001,
    CELL_SAIL_M4V_ENC_EX_PARAM_VOL_HEADER  = 64002,
    CELL_SAIL_M4V_ENC_EX_PARAM_GOP_HEADER  = 64003,
};

typedef struct {
    int index;
    int value;
} CellSailM4vEncExParam;

typedef struct {
    size_t                 thisSize;
    void                  *pReserved;
    CellSailM4vEncExParam *pExParams;
    uint32_t               numExParams;
} CellSailM4vEncConfiguration;



// avc enc mode
enum {
    CELL_SAIL_AVC_ENC_MODE_UNSPECIFIED                   = -1,
    CELL_SAIL_AVC_ENC_MODE_NO_DELAY                      = 0x101,
    CELL_SAIL_AVC_ENC_MODE_NO_DELAY_HRD_CONSTRAINT       = 0x102,
    CELL_SAIL_AVC_ENC_MODE_NO_DELAY_CAVLC                = 0x111,
    CELL_SAIL_AVC_ENC_MODE_NO_DELAY_CAVLC_HRD_CONSTRAINT = 0x112,
    CELL_SAIL_AVC_ENC_MODE_MIDDLE_DELAY                  = 0x310,
    CELL_SAIL_AVC_ENC_MODE_LONG_DELAY                    = 0x200,
};

// avc preset
enum {
    CELL_SAIL_AVC_ENC_PRESET_UNSPECIFIED = -1,

    CELL_SAIL_AVC_ENC_PRESET_L11_176x144_30P_128KBPS  = 0x031,
    CELL_SAIL_AVC_ENC_PRESET_L12_320x240_15P_384KBPS  = 0x05,
    CELL_SAIL_AVC_ENC_PRESET_L13_320x240_30P_768KBPS  = 0x001,
    CELL_SAIL_AVC_ENC_PRESET_L21_320x240_30P_768KBPS  = 0x007,
    CELL_SAIL_AVC_ENC_PRESET_L30_640x480_30P_2MBPS    = 0x100,
    CELL_SAIL_AVC_ENC_PRESET_L31_1280x720_30P_8MBPS   = 0x050,
    CELL_SAIL_AVC_ENC_PRESET_L40_1920x1088_30P_16MBPS = 0x200,
};

// avc ex param
enum {
    CELL_SAIL_AVC_ENC_EX_PARAM_DEFAULT_M                  = 16003,
    CELL_SAIL_AVC_ENC_EX_PARAM_MAX_VCL_CPB_SIZE           = 42881,
    CELL_SAIL_AVC_ENC_EX_PARAM_MAX_NAL_CPB_SIZE           = 42865,
    CELL_SAIL_AVC_ENC_EX_PARAM_VCL_TRACE_BITRATE          = 42880,
    CELL_SAIL_AVC_ENC_EX_PARAM_NAL_TRACE_BITRATE          = 42864,

    CELL_SAIL_AVC_ENC_EX_PARAM_IDR_PERIOD                 = 14663,
    CELL_SAIL_AVC_ENC_EX_PARAM_CONSTRAINT_SET0_FLAG       =  7680,
    CELL_SAIL_AVC_ENC_EX_PARAM_CONSTRAINT_SET1_FLAG       =  7681,
    CELL_SAIL_AVC_ENC_EX_PARAM_CONSTRAINT_SET2_FLAG       =  7682,
    CELL_SAIL_AVC_ENC_EX_PARAM_MAX_LOAD_CONTROL           =  4741,
    CELL_SAIL_AVC_ENC_EX_PARAM_MAX_SLICE_NUM              =  1042,
};

typedef struct {
    int index;
    int value;
} CellSailAvcEncExParam;


typedef struct {
    size_t                 thisSize;
    void                  *pReserved;
    CellSailAvcEncExParam *pExParams;
    unsigned int           numExParams;
} CellSailAvcEncConfiguration;



// m4hd enc mode
enum {
    CELL_SAIL_M4HD_ENC_MODE_UNSPECIFIED = -1,
};

// m4hd preset
enum {
    // Advaced Simple Profile L5 (up to 720x675)
    CELL_SAIL_M4HD_ENC_PRESET_ASP = 1,

    // Advaced Coding Efficiency Profile L4 (up to 1280x720)
    CELL_SAIL_M4HD_ENC_PRESET_ACE = 2,
};

//
// MP4
//
typedef struct CellSailMp4MovieParameter {
    size_t   thisSize;           // must be sizeof(CellSailMp4MovieParameter)
    int      flags;              // must be -1;
    uint32_t timeScale;          // -1: default(90kHz)
    uint32_t majorBrand;         // -1: default("MP42")
    uint32_t compatibleBrandNum; // -1: default(equals to 0)
    uint32_t compatibleBrands[8]; 
    uint32_t interleaveDuration; // -1: default(500msec) (unit: "timeScale" above)
} CellSailMp4MovieParameter;


//
// parames for generic stream
//
typedef struct CellSailStreamParameter {
    size_t  thisSize; // must be sizeof(CellSailStreamParameter)
    int     heapSize; // -1: default(1MB) (unit: bytes)
} CellSailStreamParameter;


//
// parames for generic audio encoder
//

enum {
    CELL_SAIL_BS_AUDIO_FLAG_UNSPECIFIED = -1,
};

typedef struct CellSailBsAudioParameter {
    size_t thisSize; // must be sizeof(CellSailBsVideoParameter)

    /*
     * CELL_SAIL_ENCODER_AUDIO_UNSPECIFIED
     * CELL_SAIL_ENCODER_AUDIO_M4AAC
     */
    int codecType;    // -1:default(aac)
    int flags;        // must be -1
    int reserved0;    // must be -1
    int reserved1;    // must be -1
    int maxBitrate;   // -1:default
    int aveBitrate;   // -1:default
    int sampleScale;  //  -1:default
    int reserved2[3]; // must be -1

    // must be -1 except for chNum
    CellSailAudioFormat format;
} CellSailBsAudioParameter;


//
// parames for generic video encoder
//

enum {
    CELL_SAIL_BS_VIDEO_FLAG_UNSPECIFIED = -1,
};

typedef struct CellSailBsVideoParameter {
    size_t thisSize; // must be sizeof(CellSailBsVideoParameter)

    /*
     * CELL_SAIL_ENCODER_VIDEO_UNSPECIFIED
     * CELL_SAIL_ENCODER_VIDEO_M4V
     * CELL_SAIL_ENCODER_VIDEO_AVC
     * CELL_SAIL_ENCODER_VIDEO_M4HD
     */
    int codecType;   // -1:default(m4v)
    int flags;       // must be -1
    int majorMode;   // -1:default
    int minorMode;   // -1:default
    int maxBitrate;  // -1:default
    int aveBitrate;  // -1:default
    int sampleScale; // -1:default
    int gopDuration; // -1:default
    int reserved[2]; // must be -1

    // must be -1 except for width, height
    CellSailVideoFormat format;
} CellSailBsVideoParameter;


typedef struct CellSailProfile {
    uint64_t internalData[512];
} CellSailProfile;



int cellSailProfileSetStreamParameter(
    CellSailProfile         *pSelf,
    CellSailStreamParameter *pContainerParameter,
    void                    *pContainerSpecific);

int cellSailProfileSetEsAudioParameter(
    CellSailProfile          *pSelf,
    unsigned                  index,
    CellSailBsAudioParameter *pAudioParameter,
    void                     *pCodecSpecific);

int cellSailProfileSetEsVideoParameter(
    CellSailProfile          *pSelf,
    unsigned                  index,
    CellSailBsVideoParameter *pVideoParameter,
    void                     *pCodecSpecific);



static inline void
cellSailM4vEncConfigurationSet(
    CellSailM4vEncConfiguration *pConfig,
    CellSailM4vEncExParam       *pExParams,
    unsigned int                 numExParams)
{
    pConfig->thisSize    = sizeof(CellSailM4vEncConfiguration);
    pConfig->pReserved   = NULL;
    pConfig->pExParams   = pExParams;
    pConfig->numExParams = numExParams;
}

static inline void
cellSailAvcEncConfigurationSet(
    CellSailAvcEncConfiguration *pConfig,
    CellSailAvcEncExParam       *pExParams,
    unsigned int                 numExParams)
{
    pConfig->thisSize    = sizeof(CellSailAvcEncConfiguration);
    pConfig->pReserved   = NULL;
    pConfig->pExParams   = pExParams;
    pConfig->numExParams = numExParams;
}


#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
}
#endif

#endif /* __CELL_SAIL_PROFILE_H__ */
/*[eof]*/
