/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2008 Sony Computer Entertainment Inc.
 * All Rights Reserved.
 */

#ifndef __CELL_SAIL_AVI_H__
#define __CELL_SAIL_AVI_H__

#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
extern "C" {
#endif


#define CELL_SAIL_AVI_FCC_auds CELL_SAIL_4CC('a','u','d','s')
#define CELL_SAIL_AVI_FCC_vids CELL_SAIL_4CC('v','i','d','s')

#define CELL_SAIL_AVI_FCC_divx CELL_SAIL_4CC('d','i','v','x')
#define CELL_SAIL_AVI_FCC_DIVX CELL_SAIL_4CC('D','I','V','X')

#define CELL_SAIL_AVI_FCC_DX50 CELL_SAIL_4CC('D','X','5','0')
#define CELL_SAIL_AVI_FCC_dx50 CELL_SAIL_4CC('d','x','5','0')

#define CELL_SAIL_AVI_FCC_div3 CELL_SAIL_4CC('d','i','v','3')
#define CELL_SAIL_AVI_FCC_DIV3 CELL_SAIL_4CC('D','I','V','3')
#define CELL_SAIL_AVI_FCC_div4 CELL_SAIL_4CC('d','i','v','4')
#define CELL_SAIL_AVI_FCC_DIV4 CELL_SAIL_4CC('D','I','V','4')

#define CELL_SAIL_AVI_FCC_dxsb CELL_SAIL_4CC('d','x','s','b')
#define CELL_SAIL_AVI_FCC_DXSB CELL_SAIL_4CC('D','X','S','B')


#define CELL_SAIL_AVI_FCC_NULL (0)


typedef struct CellSailAviMovie {
    uint64_t internalData[16];
} CellSailAviMovie;

typedef struct CellSailAviStream {
    uint64_t internalData[2];
} CellSailAviStream;

typedef struct CellSailAviMovieInfo {
    uint32_t maxBytesPerSec;
    uint32_t flags;
    uint32_t reserved0;
    uint32_t streams;
    uint32_t suggestedBufferSize;
    uint32_t width;
    uint32_t height;
    uint32_t scale;
    uint32_t rate;
    uint32_t length;
    uint32_t reserved1;
    uint32_t reserved2;
} CellSailAviMovieInfo;

typedef struct CellSailAviMainHeader {
    uint32_t microSecPerFrame;
    uint32_t maxBytesPerSec;
    uint32_t paddingGranularity;
    uint32_t flags;
    uint32_t totalFrames;
    uint32_t initialFrames;
    uint32_t streams;
    uint32_t suggestedBufferSize;
    uint32_t width;
    uint32_t height;
    uint32_t reserved[4];
} CellSailAviMainHeader;

// bitmaps for CellSailAviMovieInfo::flags and  CellSailAviMainHeader::flags
enum {
    CELL_SAIL_AVIF_HASINDEX       = 0x00000010,
    CELL_SAIL_AVIF_MUSTUSEINDEX   = 0x00000020,
    CELL_SAIL_AVIF_ISINTERLEAVED  = 0x00000100,
    CELL_SAIL_AVIF_WASCAPTUREFILE = 0x00010000,
    CELL_SAIL_AVIF_COPYRIGHTED    = 0x00020000,

    CELL_SAIL_AVIF_TRUSTCKTYPE    = 0x00000800, // Open-DML only
};

/*E for OpenDML */
typedef struct CellSailAviExtendedHeader {
    uint32_t  totalFrames;
} CellSailAviExtendedHeader;

typedef struct CellSailAviStreamHeader {
    uint32_t fccType;
    uint32_t fccHandler;
    uint32_t flags;
    uint16_t priority;
    uint16_t language;
    uint32_t initialFrames;
    uint32_t scale;
    uint32_t rate;
    uint32_t start;
    uint32_t length;
    uint32_t suggestedBufferSize;
    uint32_t quality;
    uint32_t sampleSize;
    struct {
        uint16_t left;
        uint16_t top;
        uint16_t right;
        uint16_t bottom;
    } frame;
} CellSailAviStreamHeader;


// constants for CellSailAviStreamHeader::fccType
enum {
    CELL_SAIL_AVI_STREAM_AUDIO = CELL_SAIL_AVI_FCC_auds,
    CELL_SAIL_AVI_STREAM_VIDEO = CELL_SAIL_AVI_FCC_vids,
    CELL_SAIL_AVI_STREAM_ANY   = CELL_SAIL_AVI_FCC_NULL,
};

// constants for CellSailAviStreamHeader::fccHandler
enum {
    // for audio
    CELL_SAIL_AVI_HANDLER_UNSPECIFIED = CELL_SAIL_AVI_FCC_NULL,

    // for DivX 6.x, 5.x, 4.x
    CELL_SAIL_AVI_HANDLER_divx = CELL_SAIL_AVI_FCC_divx,
    CELL_SAIL_AVI_HANDLER_DIVX = CELL_SAIL_AVI_FCC_DIVX,

    // for DivX 3.11
    CELL_SAIL_AVI_HANDLER_div3 = CELL_SAIL_AVI_FCC_div3,
    CELL_SAIL_AVI_HANDLER_DIV3 = CELL_SAIL_AVI_FCC_DIV3,
    CELL_SAIL_AVI_HANDLER_div4 = CELL_SAIL_AVI_FCC_div4,
    CELL_SAIL_AVI_HANDLER_DIV4 = CELL_SAIL_AVI_FCC_DIV4,
};

// bitmaps for CellSailAviStreamHeader::flags
enum {
    CELL_SAIL_AVISF_DISABLED         = 0x00000001,
    CELL_SAIL_AVISF_VIDEO_PALCHANGES = 0x00010000,
};


typedef struct CellSailBitmapInfoHeader {
    uint32_t  size;
    int32_t   width;
    int32_t   height;
    uint16_t  planes;
    uint16_t  bitCount;
    uint32_t  compression;
    uint32_t  sizeImage;
    int32_t   xPelsPerMeter;
    int32_t   yPelsPerMeter;
    uint32_t  clrUsed;
    uint32_t  clrImportant;
} CellSailBitmapInfoHeader;

// constants for CellSailAviStreamHeader::fccHandler
enum {
    CELL_SAIL_AVI_COMPRESSION_UNSPECIFIED = CELL_SAIL_AVI_FCC_NULL,

    // for DivX 6.x, 5.x
    CELL_SAIL_AVI_COMPRESSION_DX50 = CELL_SAIL_AVI_FCC_DX50,
    CELL_SAIL_AVI_COMPRESSION_dx50 = CELL_SAIL_AVI_FCC_dx50,

    // for DivX 4.x
    CELL_SAIL_AVI_COMPRESSION_divx = CELL_SAIL_AVI_FCC_divx,
    CELL_SAIL_AVI_COMPRESSION_DIVX = CELL_SAIL_AVI_FCC_DIVX,

    // for DivX 3.11
    CELL_SAIL_AVI_COMPRESSION_div3 = CELL_SAIL_AVI_FCC_div3,
    CELL_SAIL_AVI_COMPRESSION_DIV3 = CELL_SAIL_AVI_FCC_DIV3,
    CELL_SAIL_AVI_COMPRESSION_div4 = CELL_SAIL_AVI_FCC_div4,
    CELL_SAIL_AVI_COMPRESSION_DIV4 = CELL_SAIL_AVI_FCC_DIV4,

    CELL_SAIL_AVI_COMPRESSION_dxsb = CELL_SAIL_AVI_FCC_dxsb,
    CELL_SAIL_AVI_COMPRESSION_DXSB = CELL_SAIL_AVI_FCC_DXSB,

    /*
    CELL_SAIL_AVI_VIDEO_COMPRESSION_xvid = CELL_SAIL_AVI_FCC_xvid,
    CELL_SAIL_AVI_VIDEO_COMPRESSION_XVID = CELL_SAIL_AVI_FCC_XVID,
    CELL_SAIL_AVI_VIDEO_COMPRESSION_3ivx = CELL_SAIL_AVI_FCC_3ivx,
    CELL_SAIL_AVI_VIDEO_COMPRESSION_3IVX = CELL_SAIL_AVI_FCC_3IVX,
    CELL_SAIL_AVI_VIDEO_COMPRESSION_3iv2 = CELL_SAIL_AVI_FCC_3iv2,
    CELL_SAIL_AVI_VIDEO_COMPRESSION_3IV2 = CELL_SAIL_AVI_FCC_3IV2,
    CELL_SAIL_AVI_VIDEO_COMPRESSION_MJPG = CELL_SAIL_AVI_FCC_MJPG,
    */
};




#pragma pack(1)

typedef struct CellSailWaveFormatEx {
    uint16_t  formatTag;
    uint16_t  channels;
    uint32_t  samplesPerSec;
    uint32_t  avgBytesPerSec;
    uint16_t  blockAlign;
    uint16_t  bitsPerSample;
    uint16_t  cbSize;
} CellSailWaveFormatEx;

enum {
    CELL_SAIL_WAVE_FORMAT_PCM         = 0x0001,
    CELL_SAIL_WAVE_FORMAT_MPEG        = 0x0050,
    CELL_SAIL_WAVE_FORMAT_MPEGLAYER3  = 0x0055,
    CELL_SAIL_WAVE_FORMAT_AC3         = 0x2000,
    CELL_SAIL_WAVE_FORMAT_UNSPECIFIED = 0xFFFF,
};

typedef struct CellSailMpeg1WaveFormat {
    CellSailWaveFormatEx wfx;
    uint16_t headLayer;
    uint32_t headBitrate;
    uint16_t headMode;
    uint16_t headModeExt;
    uint16_t headEmphasis;
    uint16_t headFlags;
    uint32_t PTSLow;
    uint32_t PTSHigh;
} CellSailMpeg1WaveFormat;

// for CellSailAviMpeg1WaveFormat::headLayer
enum {
    CELL_SAIL_ACM_MPEG_LAYER1 = 0x0001,
    CELL_SAIL_ACM_MPEG_LAYER2 = 0x0002,
    CELL_SAIL_ACM_MPEG_LAYER3 = 0x0004,
};

// for CellSailAviMpeg1WaveFormat::headMode
enum {
    CELL_SAIL_ACM_MPEG_STEREO        = 0x0001,
    CELL_SAIL_ACM_MPEG_JOINTSTEREO   = 0x0002,
    CELL_SAIL_ACM_MPEG_DUALCHANNEL   = 0x0004,
    CELL_SAIL_ACM_MPEG_SINGLECHANNEL = 0x0008,
};

// for CellSailAviMpeg1WaveFormat::headFlags
enum {
    CELL_SAIL_ACM_MPEG_PRIVATEBIT    = 0x0001,
    CELL_SAIL_ACM_MPEG_COPYRIGHT     = 0x0002,
    CELL_SAIL_ACM_MPEG_ORIGINALHOME  = 0x0004,
    CELL_SAIL_ACM_MPEG_PROTECTIONBIT = 0x0008,
    CELL_SAIL_ACM_MPEG_ID_MPEG1      = 0x0010,
};

typedef struct CellSailMpegLayer3WaveFormat {
    CellSailWaveFormatEx wfx;
    uint16_t ID;
    uint32_t flags;
    uint16_t blockSize;
    uint16_t framesPerBlock;
    uint16_t codecDelay;
} CellSailMpegLayer3WaveFormat;

enum {
    CELL_SAIL_MPEGLAYER3_ID_UNKNOWN           = 0,
    CELL_SAIL_MPEGLAYER3_ID_MPEG              = 1,
    CELL_SAIL_MPEGLAYER3_ID_CONSTANTFRAMESIZE = 2,
    CELL_SAIL_MPEGLAYER3_FLAG_PADDING_ISO     = 0x00000000,
    CELL_SAIL_MPEGLAYER3_FLAG_PADDING_ON      = 0x00000001,
    CELL_SAIL_MPEGLAYER3_FLAG_PADDING_OFF     = 0x00000002,
};

#pragma pack()




typedef struct CellSailAviMeidaType {
    uint32_t fccType;
    uint32_t fccHandler;
    union {
        struct {
            uint16_t formatTag;
            uint16_t reserved;
            union {
                struct {
                    uint16_t headLayer;
                    uint16_t reserved;
                } mpeg;
            } u;
        } audio;

        struct {
            uint32_t compression;
            uint32_t reserved;
        } video;

        struct {
            uint32_t compression;
            uint32_t reserved;
        } any;
    } u;
} CellSailAviMediaType;

int cellSailAviMovieGetMovieInfo(CellSailAviMovie     *pMovie,
                                 CellSailAviMovieInfo *pInfo);

int cellSailAviMovieGetStreamByIndex(CellSailAviMovie  *pMovie,
                                     uint32_t           streamIndex,
                                     CellSailAviStream *pStream);
int cellSailAviMovieGetStreamByTypeAndIndex(CellSailAviMovie     *pMovie,
                                            CellSailAviMediaType *pMediaType,
                                            uint32_t              index,
                                            CellSailAviStream    *pStream);

int cellSailAviMovieGetHeader(CellSailAviMovie          *pMovie,
                              CellSailAviMainHeader     *pMainHeader,
                              CellSailAviExtendedHeader *pExtendedHeader);

int cellSailAviStreamGetMediaType(CellSailAviStream    *pStream,
                                  CellSailAviMediaType *pMediaType);

int cellSailAviStreamGetHeader(CellSailAviStream       *pStream,
                               CellSailAviStreamHeader *pStreamHeader,
                               void                    *pSpecificInfo,
                               size_t                   specificInfoSize);
                                      

#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
}
#endif

#endif /* __CELL_SAIL_AVI_H__ */
