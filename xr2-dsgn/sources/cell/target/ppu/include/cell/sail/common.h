/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2008 Sony Computer Entertainment Inc.
 * All Rights Reserved.
 */

#ifndef __CELL_SAIL_COMMON_H__
#define __CELL_SAIL_COMMON_H__

#include <stdint.h>
#include <cell/cell_fs.h>

#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
extern "C" {
#endif

/* 0x8061_0700 0x8061_07FF */

#define CELL_SAIL_ERROR_INVALID_ARG        CELL_ERROR_CAST(0x80610701)
#define CELL_SAIL_ERROR_INVALID_STATE      CELL_ERROR_CAST(0x80610702)
#define CELL_SAIL_ERROR_UNSUPPORTED_STREAM CELL_ERROR_CAST(0x80610703)
#define CELL_SAIL_ERROR_INDEX_OUT_OF_RANGE CELL_ERROR_CAST(0x80610704)
#define CELL_SAIL_ERROR_EMPTY              CELL_ERROR_CAST(0x80610705)
#define CELL_SAIL_ERROR_FULLED             CELL_ERROR_CAST(0x80610706)
#define CELL_SAIL_ERROR_USING              CELL_ERROR_CAST(0x80610707)
#define CELL_SAIL_ERROR_NOT_AVAILABLE      CELL_ERROR_CAST(0x80610708)
#define CELL_SAIL_ERROR_CANCEL             CELL_ERROR_CAST(0x80610709)

#define CELL_SAIL_ERROR_MEMORY             CELL_ERROR_CAST(0x806107F0)
#define CELL_SAIL_ERROR_INVALID_FD         CELL_ERROR_CAST(0x806107F1)
#define CELL_SAIL_ERROR_FATAL              CELL_ERROR_CAST(0x806107FF)


typedef enum {       
    CELL_SAIL_EVENT_EMPTY                 = 0, /* NEVER USED */

    CELL_SAIL_EVENT_ERROR_OCCURED         = 1,
    CELL_SAIL_EVENT_PLAYER_CALL_COMPLETED = 2,

    CELL_SAIL_EVENT_PLAYER_STATE_CHANGED  = 3,
    CELL_SAIL_EVENT_STREAM_OPENED         = 4,
    CELL_SAIL_EVENT_STREAM_CLOSED         = 5,
    CELL_SAIL_EVENT_SESSION_STARTED       = 6,
    CELL_SAIL_EVENT_PAUSE_STATE_CHANGED   = 7,
    CELL_SAIL_EVENT_SOURCE_EOS            = 8,

    CELL_SAIL_EVENT_ES_OPENED             = 9,
    CELL_SAIL_EVENT_ES_CLOSED             = 10,

    CELL_SAIL_EVENT_MEDIA_STATE_CHANGED   = 11,

    _CELL_SAIL_EVENT_TYPE_NUM_OF_ELEMENTS = 12, /*DO NOT USE*/
} CellSailEventType;

typedef union CellSailEvent {
    struct {
        uint32_t major;
        uint32_t minor;
    } u32x2;
    struct {
        uint64_t value;
    } u64;
} CellSailEvent;


typedef enum {
    CELL_SAIL_STREAM_PAMF = 0,
    CELL_SAIL_STREAM_MP4  = 1,
    CELL_SAIL_STREAM_AVI  = 2,

    CELL_SAIL_STREAM_UNSPECIFIED = -1,
} CellSailStreamType;

enum {
    CELL_SAIL_ES_AUDIO = 0,
    CELL_SAIL_ES_VIDEO = 1,
    CELL_SAIL_ES_USER  = 2,
};

enum {
    CELL_SAIL_AUDIO_CODING_UNSPECIFIED  = -1,
    CELL_SAIL_AUDIO_CODING_LPCM_FLOAT32 = 1,
};

enum {
    CELL_SAIL_AUDIO_CHNUM_UNSPECIFIED    = -1,
    CELL_SAIL_AUDIO_CH_NUM_UNSPECIFIED   = -1,
    CELL_SAIL_AUDIO_AUSAMPLE_UNSPECIFIED = -1,
};

enum {
    CELL_SAIL_AUDIO_SAMPLE_NUM_UNSPECIFIED = -1,
};

enum {
    CELL_SAIL_AUDIO_FS_32000HZ = 32000,
    CELL_SAIL_AUDIO_FS_44100HZ = 44100,
    CELL_SAIL_AUDIO_FS_48000HZ = 48000,

    CELL_SAIL_AUDIO_FS_96000HZ = 96000,
    CELL_SAIL_AUDIO_FS_88200HZ = 88200,
    CELL_SAIL_AUDIO_FS_64000HZ = 64000,
    //CELL_SAIL_AUDIO_FS_48000HZ = 48000,
    //CELL_SAIL_AUDIO_FS_44100HZ = 44100,
    //CELL_SAIL_AUDIO_FS_32000HZ = 32000,
    CELL_SAIL_AUDIO_FS_24000HZ = 24000,
    CELL_SAIL_AUDIO_FS_22050HZ = 22050,
    CELL_SAIL_AUDIO_FS_16000HZ = 16000,
    CELL_SAIL_AUDIO_FS_12000HZ = 12000,
    CELL_SAIL_AUDIO_FS_11025HZ = 11025,
    CELL_SAIL_AUDIO_FS_8000HZ  = 8000,
    CELL_SAIL_AUDIO_FS_7350HZ  = 7350,

    CELL_SAIL_AUDIO_FS_192000HZ  = 192000,
    //CELL_SAIL_AUDIO_FS_11024HZ   = 11025,
    CELL_SAIL_AUDIO_FS_UNSPECIFIED  = -1,

};

enum {
	CELL_SAIL_AUDIO_CH_LAYOUT_UNDEFINED = 0,

    // monoral
	CELL_SAIL_AUDIO_CH_LAYOUT_1CH       = 1,

    // 1. Front Left
    // 2. Front Right
	CELL_SAIL_AUDIO_CH_LAYOUT_2CH_LR    = 2,

    // 1. Front Left
    // 2. Front Center
    // 3. Front Right
    // for m4aac ac3
	CELL_SAIL_AUDIO_CH_LAYOUT_3CH_LCR = 3,

    // 1. Front Left
    // 2. Front Center
    // 3. Surround
    // for m4aac ac3
	CELL_SAIL_AUDIO_CH_LAYOUT_3CH_LRc = 4,

    // 1. Front Left
    // 2. Front Center
    // 3. Front Right
    // 4. Surround
    // for m4aac ac3
	CELL_SAIL_AUDIO_CH_LAYOUT_4CH_LCRc = 5,

    // 1. Front Left
    // 2. Front Right
    // 3. Surround Left
    // 4. Surround Right
    // for m4aac
	CELL_SAIL_AUDIO_CH_LAYOUT_4CH_LRlr = 6,

    // 1. Front Left
    // 2. Front Center
    // 3. Front Right
    // 4. Surround Left
    // 5. Surround Right
    // for m4aac
	CELL_SAIL_AUDIO_CH_LAYOUT_5CH_LCRlr = 7,

    // 1. Front Left
    // 2. Front Center
    // 3. Front Right
    // 4. Surround Left
    // 5. Surround Right
    // 6. LFE
    // for lpcm ac3 m4aac
	CELL_SAIL_AUDIO_CH_LAYOUT_6CH_LCRlrE = 8,

    // 1. Front Left
    // 2. Front Center
    // 3. Front Right
    // 4. Back Left
    // 5. Back Right
    // 6. LFE
    // for at3plus
	CELL_SAIL_AUDIO_CH_LAYOUT_6CH_LCRxyE = 9,

    // 1. Front Left
    // 2. Front Center
    // 3. Front Right
    // 4. Back Left
    // 5. Back Right
    // 6. Back Center
    // 7. LFE
    // (for at3plus) 
	CELL_SAIL_AUDIO_CH_LAYOUT_7CH_LCRxycE  = 10,

    // 1. Front Left
    // 2. Front Center
    // 3. Front Right
    // 4. LFE
    // 5. Surround Left
    // 6. Surround Right
    // 7. Back Left  (Left-Extend)
    // 8. Back Right (Right-Extend)
    // for lpcm at3plus
	CELL_SAIL_AUDIO_CH_LAYOUT_8CH_LRCElrxy = 11,

	CELL_SAIL_AUDIO_CH_LAYOUT_2CH_DUAL  = 12,

	CELL_SAIL_AUDIO_CH_LAYOUT_UNSPECIFIED = -1,
};

typedef struct {
    int8_t  coding;
    int8_t  chNum;
    int16_t sampleNum;
    int32_t fs; // sampling frequency
    int32_t chLayout;
    int32_t reserved0;
    int64_t reserved1;
} CellSailAudioFormat;


enum {
    CELL_SAIL_VIDEO_CODING_UNSPECIFIED         = -1,
    CELL_SAIL_VIDEO_CODING_ARGB_INTERLEAVED    =  0,
    CELL_SAIL_VIDEO_CODING_RGBA_INTERLEAVED    =  1,
    CELL_SAIL_VIDEO_CODING_YUV422_U_Y0_V_Y1    =  2,
    CELL_SAIL_VIDEO_CODING_YUV420_PLANAR       =  3,

    // suported by libcamera
    CELL_SAIL_VIDEO_CODING_YUV422_Y0_U_Y1_V    = 4,
    CELL_SAIL_VIDEO_CODING_YUV422_V_Y1_U_Y0    = 9,

    CELL_SAIL_VIDEO_CODING_YUV422_Y1_V_Y0_U    = 10,

    CELL_SAIL_VIDEO_CODING_JPEG                = 11,
    CELL_SAIL_VIDEO_CODING_RAW8_BAYER_BGGR     = 12,

    _CELL_SAIL_VIDEO_CODING_TYPE_NUM_OF_ELEMENTS = 13,


    CELL_SAIL_VIDEO_CODING_UYVY422_INTERLEAVED   = 2,
    CELL_SAIL_VIDEO_CODING_YUYV422_INTERLEAVED   = 4,
    CELL_SAIL_VIDEO_CODING_VYUY422_REVERSE_INTERLEAVED = 9,
    CELL_SAIL_VIDEO_CODING_RAW8_BAYER_GRBG       = 12,
};

enum {
    CELL_SAIL_VIDEO_COLOR_MATRIX_UNSPECIFIED = -1,
    CELL_SAIL_VIDEO_COLOR_MATRIX_BT601       =  0,
    CELL_SAIL_VIDEO_COLOR_MATRIX_BT709       =  1,

    _CELL_SAIL_VIDEO_COLOR_MATRIX_TYPE_NUM_OF_ELEMENTS = 2,
};

enum {
    CELL_SAIL_VIDEO_SCAN_UNSPECIFIED = -1,
    CELL_SAIL_VIDEO_SCAN_PROGRESSIVE =  0,
    CELL_SAIL_VIDEO_SCAN_INTERLACE   =  1,

    _CELL_SAIL_VIDEO_SCAN_TYPE_NUM_OF_ELEMENTS = 2,
};

enum {
    CELL_SAIL_VIDEO_FRAME_RATE_UNSPECIFIED  = -1,
    CELL_SAIL_VIDEO_FRAME_RATE_24000_1001HZ =  0,
    CELL_SAIL_VIDEO_FRAME_RATE_24HZ         =  1,
    CELL_SAIL_VIDEO_FRAME_RATE_25HZ         =  2,
    CELL_SAIL_VIDEO_FRAME_RATE_30000_1001HZ =  3,
    CELL_SAIL_VIDEO_FRAME_RATE_30HZ         =  4,
    CELL_SAIL_VIDEO_FRAME_RATE_50HZ         =  5,
    CELL_SAIL_VIDEO_FRAME_RATE_60000_1001HZ =  6,
    CELL_SAIL_VIDEO_FRAME_RATE_60HZ         =  7,

    _CELL_SAIL_VIDEO_FRAME_RATE_TYPE_NUM_OF_ELEMENTS = 8,
};

enum {
    CELL_SAIL_VIDEO_ASPECT_RATIO_UNSPECIFIED = -1,
    CELL_SAIL_VIDEO_ASPECT_RATIO_1_1         =  1, // 1920x1080 1280x720
    CELL_SAIL_VIDEO_ASPECT_RATIO_12_11       =  2, // 720x576 normal
    CELL_SAIL_VIDEO_ASPECT_RATIO_10_11       =  3, // 720x480 normal
    CELL_SAIL_VIDEO_ASPECT_RATIO_16_11       =  4, // 720x576 wide
    CELL_SAIL_VIDEO_ASPECT_RATIO_40_33       =  5, // 720x480 wide
    CELL_SAIL_VIDEO_ASPECT_RATIO_4_3         = 14, // 1440x1080
};

enum {
    CELL_SAIL_VIDEO_WIDTH_UNSPECIFIED          = -1,
    CELL_SAIL_VIDEO_HEIGHT_UNSPECIFIED         = -1,
    CELL_SAIL_VIDEO_PITCH_UNSPECIFIED          = -1,
    CELL_SAIL_VIDEO_BITS_PER_COLOR_UNSPECIFIED = -1,
    CELL_SAIL_VIDEO_ALPHA_UNSPECIFIED          = -1,
};


enum {
    CELL_SAIL_VIDEO_COLOR_RANGE_UNSPECIFIED  = -1,
    CELL_SAIL_VIDEO_COLOR_RANGE_LIMITED      = 1,
    CELL_SAIL_VIDEO_COLOR_RANGE_FULL         = 0,
};

typedef struct {
    int8_t   coding;
    int8_t   scan;
    int8_t   bitsPerColor;
    int8_t   frameRate;
    int16_t  width;
    int16_t  height;
    int32_t  pitch;
    int32_t  alpha;
    int8_t   colorMatrix;
    int8_t   aspectRatio;
    int8_t   colorRange;
    int8_t   reserved1;
    int32_t  reserved2;
    int64_t  reserved3;
} CellSailVideoFormat;


typedef struct {
    void       *pAu;
    uint32_t    size;
    int         status;
    uint32_t    sessionId;
    uint64_t    pts;
    uint64_t    dts;
    uint64_t    reserved;
} CellSailAuInfo;


typedef void*(*CellSailMemAllocatorFuncAlloc)(void   *pArg,
                                              size_t  boundary,
                                              size_t  size);
typedef void (*CellSailMemAllocatorFuncFree)(void   *pArg,
                                             size_t  boundary,
                                             void   *pMemory);

typedef struct CellSailMemAllocatorFuncs {
    CellSailMemAllocatorFuncAlloc pAlloc;
    CellSailMemAllocatorFuncFree  pFree;
} CellSailMemAllocatorFuncs;


typedef struct CellSailMemAllocator {
    CellSailMemAllocatorFuncs callbacks;
    void                     *pArg;
} CellSailMemAllocator;

int cellSailMemAllocatorInitialize(
    CellSailMemAllocator            *pSelf,
    const CellSailMemAllocatorFuncs *pCallback,
    void                            *pArg);


typedef enum {
    CELL_SAIL_REPEAT_ONCE = 0,
    CELL_SAIL_REPEAT_LOOP = 1,
    CELL_SAIL_REPEAT_NEXT = 2,

    _CELL_SAIL_REPEAT_TYPE_NUM_OF_ELEMENTS, /*DO NOT USE*/
} CellSailRepeatType;

typedef enum {
    CELL_SAIL_START_NOT_SPECIFIED      = 0,
    CELL_SAIL_START_NORMAL             = 1<<0, //1

    CELL_SAIL_START_TIME_SCALE         = 1<<2, //4
    CELL_SAIL_START_EP_SKIP            = 1<<4, //16
    CELL_SAIL_START_EP_SKIP_REVERSE    = 1<<5, //32
    CELL_SAIL_START_FRAME_STEP         = 1<<6, //64
} CellSailPlayerStartType;

typedef enum {
    CELL_SAIL_SEEK_NOT_SPECIFIED          = 0,
    CELL_SAIL_SEEK_ABSOLUTE_BYTE_POSITION = 1<<0, // for pamf
    CELL_SAIL_SEEK_RELATIVE_BYTE_POSITION = 1<<1, // not implemented
    CELL_SAIL_SEEK_ABSOLUTE_TIME_POSITION = 1<<4, // mp4, avi
    CELL_SAIL_SEEK_CURRENT_POSITION       = 1<<6,

    // obsolete
    CELL_SAIL_SEEK_MP4_SCALE_AND_TIME     = 1<<4, // for mp4
} CellSailSeekType;

typedef enum {
    CELL_SAIL_TERMINUS_NOT_SPECIFIED          = 0,
    CELL_SAIL_TERMINUS_EOS                    = 1<<0,
    CELL_SAIL_TERMINUS_ABSOLUTE_BYTE_POSITION = 1<<1, // for pamf
    CELL_SAIL_TERMINUS_RELATIVE_BYTE_POSITION = 1<<2, // not implemented
    CELL_SAIL_TERMINUS_ABSOLUTE_TIME_POSITION = 1<<5, // for mp4, avi

    // obsolete
    CELL_SAIL_TERMINUS_MP4_SCALE_AND_TIME     = 1<<5, // for mp4
    // typo
    CELL_SAIL_TERMINUS_MP4_SCALE_ANT_TIME     = 1<<5, // for mp4
} CellSailTerminusType;

typedef enum {
    CELL_SAIL_START_FLAG_NOT_SPECIFIED = 0,
    CELL_SAIL_START_FLAG_UNFLUSH       = 1<<0,
    CELL_SAIL_START_FLAG_PAUSE_BEGIN   = 1<<1,
    CELL_SAIL_START_FLAG_PAUSE_END     = 1<<2,


    CELL_SAIL_START_FLAG_COMPLETE_STREAM = 1<<3,
    CELL_SAIL_START_FLAG_STICKY          = 1<<4,

    // obsolete
    CELL_SAIL_START_FLAG_PAUSE         = 1<<1,

} CellSailStartFlagType;


typedef struct CellSailStartCommand {
    uint32_t  startType;
    uint32_t  seekType;
    uint32_t  terminusType;
    uint32_t  flags;
    uint32_t  startArg;
    uint32_t  reserved;
    uint64_t  seekArg;
    uint64_t  terminusArg;
} CellSailStartCommand;


typedef struct CellSailSourceBufferItem {
    uint8_t  *pBuf;
    size_t    size;
    uint32_t  sessionId;
    uint32_t  reserved;
} CellSailSourceBufferItem;


typedef struct CellSailFuture {
    sys_mutex_t       mutex;
    sys_cond_t        cond;
    volatile uint32_t flags;
    int               result;
    uint64_t          userParam;
} CellSailFuture;


int cellSailFutureInitialize(CellSailFuture *pSelf);
int cellSailFutureFinalize(CellSailFuture *pSelf);
int cellSailFutureReset(CellSailFuture *pSelf, bool wait);
int cellSailFutureSet(CellSailFuture *pSelf, int result);
int cellSailFutureGet(CellSailFuture *pSelf, usecond_t timeout, int *pResult);
int cellSailFutureIsDone(CellSailFuture *pSelf, int *pResult);


#define CELL_SAIL_INVALID_TIMESTAMP (0xFFFFFFFFFFFFFFFFULL)

#define _CELL_SAIL_SYNC_SHIFT_NUM (8)
#define _CELL_SAIL_SYNC_MODE_ADJUST     (1<<2)
#define _CELL_SAIL_SYNC_MODE_ADJUST_ODD (1<<3)

enum {
    // buffering
    CELL_SAIL_SYNC_UNDERFLOW       = 1,
    //reserved                     = 2,

    // sync status
    CELL_SAIL_SYNC_ON_TIME         = 1 << 2,
    CELL_SAIL_SYNC_MAYBE_ON_TIME   = 2 << 2,
    CELL_SAIL_SYNC_EARLY           = 3 << 2,
    CELL_SAIL_SYNC_LATE            = 4 << 2,
    CELL_SAIL_SYNC_NO_SYNC         = 5 << 2,
    CELL_SAIL_SYNC_NO_PTS          = 6 << 2,

    CELL_SAIL_SYNC_NOT_READY       = 7 << 2,
    CELL_SAIL_SYNC_DISABLED        = 8 << 2,
    CELL_SAIL_SYNC_PAUSED          = 9 << 2,
    //CELL_SAIL_SYNC_DISABLED_PAUSED = 10 << 2,
    CELL_SAIL_SYNC_MUTED           = 11 << 2,
    CELL_SAIL_SYNC_DONE            = 12 << 2,
    // reserved 13 << 2 
    // reserved 14 << 2 
    // reserved 15 << 2 

    //CELL_SAIL_SYNC_FIRST_FRAME  = 64,
    //CELL_SAIL_SYNC_LAST_FRAME   = 128,


    // frame status
    CELL_SAIL_SYNC_NO_FRAME     = 0,
    // not ready
    // underflow
    // early

    // paused
    // disabled
    // disabled paused

    CELL_SAIL_SYNC_REPEATED     = 1 << _CELL_SAIL_SYNC_SHIFT_NUM,
    // underflow
    // early
    // on time

    // paused

    CELL_SAIL_SYNC_NEXT         = 2 << _CELL_SAIL_SYNC_SHIFT_NUM,
    // no sync
    // maybe on time
    // on time
    // late

    CELL_SAIL_SYNC_SKIPPED_ONE  = 3 << _CELL_SAIL_SYNC_SHIFT_NUM,
    // maybe on time
    // on time
};

#define CELL_SAIL_SYNC_IS_FRAME_AVAILABLE(x) \
    ((x) >= CELL_SAIL_SYNC_REPEATED)
#define CELL_SAIL_SYNC_GET_SKIPPED_FRAME_NUM(x) \
    (((x) - CELL_SAIL_SYNC_NEXT) >> _CELL_SAIL_SYNC_SHIFT_NUM )


#define CELL_SAIL_4CC(a, b, c, d) \
    (((uint32_t)((uint8_t)a)<<24) | ((uint32_t)((uint8_t)b)<<16) | \
     ((uint32_t)((uint8_t)c)<< 8) | ((uint32_t)((uint8_t)d)    ))


typedef int (*CellSailFsFuncOpen)(const char *pPath,
                                  int         flag,
                                  int        *pFd,
                                  void       *arg  /*must be NULL*/,
                                  uint64_t    size /*must be 0*/);
typedef int (*CellSailFsFuncOpenSecond)(const char *pPath,
                                        int         flag,
                                        int         fd,
                                        void       *arg  /*must be NULL*/,
                                        uint64_t    size /*must be 0*/);
typedef int (*CellSailFsFuncClose)(int fd);

typedef int (*CellSailFsFuncFstat)(int                fd,
                                   struct CellFsStat *pStat);

typedef int (*CellSailFsFuncRead)(int       fd,
                                  void     *pBuf,
                                  uint64_t  numBytes,
                                  uint64_t *pNumRead);
typedef int (*CellSailFsFuncLseek)(int       fd,
                                   int64_t   offset,
                                   int       whence,
                                   uint64_t *pPosition);
typedef int (*CellSailFsFuncCancel)(int fd);

typedef struct CellSailFsReadFuncs {
    CellSailFsFuncOpen       pOpen;
    CellSailFsFuncOpenSecond pOpenSecond;
    CellSailFsFuncClose      pClose;
    CellSailFsFuncFstat      pFstat;
    CellSailFsFuncRead       pRead;
    CellSailFsFuncLseek      pLseek;
    CellSailFsFuncCancel     pCancel;
    uint32_t                 reserved[2];
} CellSailFsReadFuncs;

typedef struct CellSailFsRead {
    uint32_t            capability;
    CellSailFsReadFuncs funcs;
} CellSailFsRead;


#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
}
#endif

#endif /* __CELL_SAIL_COMMON_H__ */
