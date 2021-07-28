/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2008 Sony Computer Entertainment Inc.
 * All Rights Reserved.
 */

#ifndef __CELL_SAIL_MP4_H__
#define __CELL_SAIL_MP4_H__

#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
extern "C" {
#endif


enum {
    CELL_SAIL_MP4_MEDIA_SOUND = CELL_SAIL_4CC('s','o','u','n'),
    CELL_SAIL_MP4_MEDIA_AUDIO = CELL_SAIL_4CC('s','o','u','n'),
    CELL_SAIL_MP4_MEDIA_VIDEO = CELL_SAIL_4CC('v','i','d','e'),
    CELL_SAIL_MP4_MEDIA_OD    = CELL_SAIL_4CC('o','d','s','m'),
};


enum {
    /* MPEG4 Audio ES */
    CELL_SAIL_MP4_AUDIO_SAMPLE_CODING_UNSPECIFIED = 0,
    CELL_SAIL_MP4_AUDIO_SAMPLE_CODING_MP4A = CELL_SAIL_4CC('m','p','4','a'),
};

enum {
    /* MPEG4 Video ES */
    CELL_SAIL_MP4_VIDEO_SAMPLE_CODING_UNSPECIFIED = 0,
    CELL_SAIL_MP4_VIDEO_SAMPLE_CODING_MP4V = CELL_SAIL_4CC('m','p','4','v'),
    CELL_SAIL_MP4_VIDEO_SAMPLE_CODING_AVC1 = CELL_SAIL_4CC('a','v','c','1'),
};

enum {
    CELL_SAIL_MP4_TRACK_REFERENCE_SYNC = CELL_SAIL_4CC('s','y','n','c'),
};

typedef struct {
    uint64_t internalData[16];
} CellSailMp4Movie;

typedef struct {
    uint64_t internalData[6];
} CellSailMp4Track;

typedef struct {
    uint16_t second;
    uint16_t minute;
    uint16_t hour;
    uint16_t day;
    uint16_t month;
    uint16_t year;
    uint16_t reserved0;
    uint16_t reserved1;
} CellSailMp4DateTime;


//movie header (moov/mvhd)
typedef struct {
    CellSailMp4DateTime creationDateTime;
    CellSailMp4DateTime modificationDateTime;
    uint32_t trackCount;
    uint32_t movieTimeScale;
    uint64_t movieDuration;
    uint32_t reserved[16];
} CellSailMp4MovieInfo;


// track header (moov/track*/tkhd)
typedef struct {
    bool     isTrackEnable;
    uint8_t  reserved0[3];
    uint32_t trackId;
    uint64_t trackDuration; /* in movie timescale */
    int16_t  layer;
    int16_t  alternateGroup;
    uint16_t reserved1[2];
    uint32_t trackWidth;
    uint32_t trackHeight;

    uint16_t language;  /* moov/track/mdia/mdhd */
    uint16_t reserved2;
    uint32_t mediaType; /* moov/track/mdia/hdlr */
    uint32_t reserved3[6];
} CellSailMp4TrackInfo;


/*
 * ftyp
 * [isom]
 */
int
cellSailMp4MovieGetBrand(CellSailMp4Movie *pSelf,
                         uint32_t         *pMajorBrand,
                         uint32_t         *pMinorVersion);
/*
 * ftyp
 * [isom]
 */
int
cellSailMp4MovieIsCompatibleBrand(CellSailMp4Movie *pSelf,
                                  uint32_t          brand);

/*
 * moov/
 * moov/mvhd
 * [isom]
 */
int
cellSailMp4MovieGetMovieInfo(CellSailMp4Movie     *pSelf,
                             CellSailMp4MovieInfo *pInfo);

/*
 * moov/
 * [isom]
 *
 * @param trackIndex 0-
 */
int cellSailMp4MovieGetTrackByIndex(CellSailMp4Movie *pSelf,
                                    int               trackIndex,
                                    CellSailMp4Track *pTrack);

int cellSailMp4MovieGetTrackById(CellSailMp4Movie *pSelf,
                                 uint32_t          trackId,
                                 CellSailMp4Track *pTrack);
/*
 * @param index 0-
 */
int cellSailMp4MovieGetTrackByTypeAndIndex(CellSailMp4Movie *pSelf,
                                           uint32_t          mediaType,
                                           uint32_t          coding,
                                           int               index,
                                           CellSailMp4Track *pTrack);
/*
 * moov/trak/tkhd
 * [isom]
 */
int cellSailMp4TrackGetTrackInfo(CellSailMp4Track     *pSelf,
                                 CellSailMp4TrackInfo *pTrackInfo);



/*
 * moov/trak/tref
 * [isom]
 */
int cellSailMp4TrackGetTrackReferenceCount(CellSailMp4Track *pSelf,
                                           uint32_t          referenceType);
/*
 * moov/trak/tref
 * [isom]
 *
 * @param index 0-
 */
int cellSailMp4TrackGetTrackReference(CellSailMp4Track *pSelf,
                                      uint32_t          referenceType,
                                      int               index,
                                      CellSailMp4Track *pReferenceTrack);                    

static inline int cellSailMp4ConvertTimeScale(uint64_t  fromDuration,
                                              uint32_t  fromTimeScale,
                                              uint32_t  toTimeScale,
                                              uint64_t *pToDuration)
{
    if (0 == fromTimeScale) {
        return -1;
    }
    *pToDuration = (fromDuration * toTimeScale) / fromTimeScale;
    return 0;
}



#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
}
#endif

#endif /* __CELL_SAIL_MP4_H__ */
