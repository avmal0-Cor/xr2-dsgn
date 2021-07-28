/* SCE CONFIDENTIAL
 PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2008 Sony Computer Entertainment Inc.
 * All Rights Reserved.
 */

#ifndef CELL_JPGENC_H
#define CELL_JPGENC_H

#include <stdint.h>
#include <stdbool.h>
#include <cell/spurs.h>
#include <cell/error.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define CELL_JPGENC_ERROR_ARG                  CELL_ERROR_CAST(0x80611191)
#define CELL_JPGENC_ERROR_SEQ                  CELL_ERROR_CAST(0x80611192)
#define CELL_JPGENC_ERROR_BUSY                 CELL_ERROR_CAST(0x80611193)
#define CELL_JPGENC_ERROR_EMPTY                CELL_ERROR_CAST(0x80611194)
#define CELL_JPGENC_ERROR_RESET                CELL_ERROR_CAST(0x80611195)
#define CELL_JPGENC_ERROR_FATAL                CELL_ERROR_CAST(0x80611196)

#define CELL_JPGENC_ERROR_STREAM_ABORT         CELL_ERROR_CAST(0x806111A1)
#define CELL_JPGENC_ERROR_STREAM_SKIP          CELL_ERROR_CAST(0x806111A2)
#define CELL_JPGENC_ERROR_STREAM_OVERFLOW      CELL_ERROR_CAST(0x806111A3)
#define CELL_JPGENC_ERROR_STREAM_FILE_OPEN     CELL_ERROR_CAST(0x806111A4)

typedef enum {
    CELL_JPGENC_COLOR_SPACE_GRAYSCALE = 1,
    CELL_JPGENC_COLOR_SPACE_RGB       = 2,
    CELL_JPGENC_COLOR_SPACE_YCbCr     = 3,
    CELL_JPGENC_COLOR_SPACE_RGBA      = 10,
    CELL_JPGENC_COLOR_SPACE_ARGB      = 20
} CellJpgEncColorSpace;

typedef enum {
    CELL_JPGENC_SAMPLING_FMT_YCbCr444,
    CELL_JPGENC_SAMPLING_FMT_YCbCr422,
    CELL_JPGENC_SAMPLING_FMT_YCbCr420,
    CELL_JPGENC_SAMPLING_FMT_YCbCr411,
    CELL_JPGENC_SAMPLING_FMT_FULL
} CellJpgEncSamplingFormat;

typedef enum {
    CELL_JPGENC_DCT_METHOD_QUALITY = 0,
    CELL_JPGENC_DCT_METHOD_FAST    = 5
} CellJpgEncDctMethod;

typedef enum {
    CELL_JPGENC_COMPR_MODE_CONSTANT_QUALITY,
    CELL_JPGENC_COMPR_MODE_STREAM_SIZE_LIMIT
} CellJpgEncCompressionMode;

typedef enum {
    CELL_JPGENC_LOCATION_FILE,
    CELL_JPGENC_LOCATION_BUFFER
} CellJpgEncLocation;


typedef void *CellJpgEncHandle;

typedef struct CellJpgEncExParam{
    int32_t   index;
    intptr_t  value;
} CellJpgEncExParam;

typedef struct CellJpgEncConfig{
    uint32_t                  maxWidth;
    uint32_t                  maxHeight;
    CellJpgEncColorSpace      encodeColorSpace;
    CellJpgEncSamplingFormat  encodeSamplingFormat;
    bool                      enableSpu;
    CellJpgEncExParam        *exParamList;
    uint32_t                  exParamNum;
} CellJpgEncConfig;

typedef struct CellJpgEncAttr{
    size_t    memSize;
    uint8_t   cmdQueueDepth;
    uint32_t  versionUpper;
    uint32_t  versionLower;
} CellJpgEncAttr;

typedef struct CellJpgEncResource{
    void     *memAddr;
    size_t    memSize;
    int32_t   ppuThreadPriority;
    int32_t   spuThreadPriority;
} CellJpgEncResource;

typedef struct CellJpgEncResourceEx{
    void      *memAddr;
    size_t     memSize;
    int32_t    ppuThreadPriority;
    CellSpurs *spurs;
    uint8_t    priority[8];
} CellJpgEncResourceEx;

typedef struct CellJpgEncPicture{
    uint32_t              width;
    uint32_t              height;
    CellJpgEncColorSpace  colorSpace;
    void                 *pictureAddr;
    uint64_t              userData;
} CellJpgEncPicture;

typedef struct CellJpgEncPicture2{
    uint32_t              width;
    uint32_t              height;
    uint32_t              pitchWidth;
    CellJpgEncColorSpace  colorSpace;
    void                 *pictureAddr;
    uint64_t              userData;
} CellJpgEncPicture2;

typedef struct CellJpgEncMarkerSegment{
    uint32_t  markerCode;
    uint8_t  *markerSegmentData;
    uint32_t  markerSegmentDataLength;
} CellJpgEncMarkerSegment;

typedef struct CellJpgEncEncodeParam{
    bool                       enableSpu;
    uint32_t                   restartInterval;
    CellJpgEncDctMethod        dctMethod;
    CellJpgEncCompressionMode  compressionMode;
    uint32_t                   quality;
    CellJpgEncMarkerSegment   *markerSegmentList;
    uint32_t                   markerSegmentNum;
} CellJpgEncEncodeParam;

typedef struct CellJpgEncOutputParam{
    CellJpgEncLocation  location;
    const char         *streamFileName;
    void               *streamAddr;
    size_t              limitSize;
} CellJpgEncOutputParam;

typedef struct CellJpgEncStreamInfo{
    int32_t             state;
    CellJpgEncLocation  location;
    const char         *streamFileName;
    void               *streamAddr;
    size_t              limitSize;
    size_t              streamSize;
    uint32_t            processedLine;
    uint32_t            quality;
    uint64_t            userData;
} CellJpgEncStreamInfo;


int32_t cellJpgEncQueryAttr(
    const CellJpgEncConfig *config,
    CellJpgEncAttr         *attr
);

int32_t cellJpgEncOpen(
    const CellJpgEncConfig   *config,
    const CellJpgEncResource *resource,
    CellJpgEncHandle         *handle
);

int32_t cellJpgEncOpenEx(
    const CellJpgEncConfig     *config,
    const CellJpgEncResourceEx *resourceEx,
    CellJpgEncHandle           *handle
);

int32_t cellJpgEncWaitForInput(
    CellJpgEncHandle  handle,
    bool              block
);

int32_t cellJpgEncEncodePicture(
    CellJpgEncHandle             handle,
    const CellJpgEncPicture     *picture,
    const CellJpgEncEncodeParam *encodeParam,
    const CellJpgEncOutputParam *outputParam
);

int32_t cellJpgEncEncodePicture2(
    CellJpgEncHandle             handle,
    const CellJpgEncPicture2    *picture,
    const CellJpgEncEncodeParam *encodeParam,
    const CellJpgEncOutputParam *outputParam
);

int32_t cellJpgEncWaitForOutput(
    CellJpgEncHandle  handle,
    uint32_t         *streamInfoNum,
    bool              block
);

int32_t cellJpgEncGetStreamInfo(
    CellJpgEncHandle      handle,
    CellJpgEncStreamInfo *streamInfo
);

int32_t cellJpgEncReset(
    CellJpgEncHandle  handle
);

int32_t cellJpgEncClose(
    CellJpgEncHandle  handle
);


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* CELL_JPGENC_H */
