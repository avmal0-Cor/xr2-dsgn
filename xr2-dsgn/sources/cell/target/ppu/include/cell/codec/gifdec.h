/* SCE CONFIDENTIAL
 PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2009 Sony Computer Entertainment Inc.
 * All Rights Reserved.
 */

#ifndef CELL_GIFDEC_H
#define CELL_GIFDEC_H

#include <stdint.h>
#include <cell/error.h>
#include <cell/spurs.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/*
  CELL_ERROR_FACILITY_CODEC        0x061
  libgifdec: 0x8061_1300 - 0x8061_137f
*/

#define CELL_GIFDEC_MAKE_ERROR(status)                         \
    CELL_ERROR_MAKE_ERROR(CELL_ERROR_FACILITY_CODEC, status)

/* ERR: CELL_GIFDEC_MAKE_ERROR(0x1300) */
#define CELL_GIFDEC_ERROR_OPEN_FILE     CELL_ERROR_CAST(0x80611300)

/* ERR: CELL_GIFDEC_MAKE_ERROR(0x1301) */
#define CELL_GIFDEC_ERROR_STREAM_FORMAT CELL_ERROR_CAST(0x80611301)

/* ERR: CELL_GIFDEC_MAKE_ERROR(0x1302) */
#define CELL_GIFDEC_ERROR_SEQ           CELL_ERROR_CAST(0x80611302)

/* ERR: CELL_GIFDEC_MAKE_ERROR(0x1303) */
#define CELL_GIFDEC_ERROR_ARG           CELL_ERROR_CAST(0x80611303)

/* ERR: CELL_GIFDEC_MAKE_ERROR(0x1304) */
#define CELL_GIFDEC_ERROR_FATAL         CELL_ERROR_CAST(0x80611304)

/* ERR: CELL_GIFDEC_MAKE_ERROR(0x1305) */
#define CELL_GIFDEC_ERROR_SPU_UNSUPPORT CELL_ERROR_CAST(0x80611305)

/* ERR: CELL_GIFDEC_MAKE_ERROR(0x1306) */
#define CELL_GIFDEC_ERROR_SPU_ERROR     CELL_ERROR_CAST(0x80611306)

/* ERR: CELL_GIFDEC_MAKE_ERROR(0x1307) */
#define CELL_GIFDEC_ERROR_CB_PARAM      CELL_ERROR_CAST(0x80611307)

typedef enum {
    CELL_GIFDEC_FILE      = 0,
    CELL_GIFDEC_BUFFER    = 1
} CellGifDecStreamSrcSel;

typedef enum {
    CELL_GIFDEC_SPU_THREAD_DISABLE    = 0,
    CELL_GIFDEC_SPU_THREAD_ENABLE     = 1
} CellGifDecSpuThreadEna;

typedef void* (*CellGifDecCbControlMalloc)(
    uint32_t    size,
    void       *cbCtrlMallocArg
);

typedef int32_t (*CellGifDecCbControlFree)(
    void       *ptr,
    void       *cbCtrlFreeArg
);

typedef struct CellGifDecThreadInParam {
    CellGifDecSpuThreadEna      spuThreadEnable;
    uint32_t                    ppuThreadPriority;
    uint32_t                    spuThreadPriority;
    CellGifDecCbControlMalloc   cbCtrlMallocFunc;
    void                       *cbCtrlMallocArg;
    CellGifDecCbControlFree     cbCtrlFreeFunc;
    void                       *cbCtrlFreeArg;
} CellGifDecThreadInParam;

typedef struct CellGifDecThreadOutParam {
    uint32_t                    gifCodecVersion;
} CellGifDecThreadOutParam;

typedef struct CellGifDecExtThreadInParam {
    CellSpurs                  *spurs;
    uint8_t                     priority[8];
    uint32_t                    maxContention;
} CellGifDecExtThreadInParam;

typedef struct CellGifDecExtThreadOutParam {
    uint32_t                    reserved;
} CellGifDecExtThreadOutParam;

typedef struct CellGifDecSrc {
    CellGifDecStreamSrcSel      srcSelect;
    const char                 *fileName;
    int64_t                     fileOffset;
    uint32_t                    fileSize;
    void                       *streamPtr;
    uint32_t                    streamSize;
    CellGifDecSpuThreadEna      spuThreadEnable;
} CellGifDecSrc;

typedef struct CellGifDecOpnInfo {
    uint32_t                    initSpaceAllocated;
} CellGifDecOpnInfo;

typedef struct CellGifDecInfo {
    uint32_t                    SWidth;
    uint32_t                    SHeight;
    uint32_t                    SGlobalColorTableFlag;
    uint32_t                    SColorResolution;
    uint32_t                    SSortFlag;
    uint32_t                    SSizeOfGlobalColorTable;
    uint32_t                    SBackGroundColor;
    uint32_t                    SPixelAspectRatio;
} CellGifDecInfo;

typedef enum {
    CELL_GIFDEC_CONTINUE        = 0,
    CELL_GIFDEC_STOP            = 1
} CellGifDecCommand;

typedef enum {
    CELL_GIFDEC_RGBA            = 10,
    CELL_GIFDEC_ARGB            = 20
} CellGifDecColorSpace;

typedef struct CellGifDecInParam {
    volatile CellGifDecCommand *commandPtr;
    CellGifDecColorSpace        colorSpace;
    uint8_t                     outputColorAlpha1;
    uint8_t                     outputColorAlpha2;
    uint8_t                     reserved[2];
} CellGifDecInParam;

typedef struct CellGifDecOutParam {
    uint64_t                    outputWidthByte;
    uint32_t                    outputWidth;
    uint32_t                    outputHeight;
    uint32_t                    outputComponents;
    uint32_t                    outputBitDepth;
    CellGifDecColorSpace        outputColorSpace;
    uint32_t                    useMemorySpace;
} CellGifDecOutParam;

typedef enum CellGifDecRecordType {
    CELL_GIFDEC_RECORD_TYPE_IMAGE_DESC  = 1,
    CELL_GIFDEC_RECORD_TYPE_EXTENSION   = 2,
    CELL_GIFDEC_RECORD_TYPE_TERMINATE   = 3
} CellGifDecRecordType;

typedef struct CellGifDecExtension {
    uint8_t                     label;
    uint8_t                    *data;
} CellGifDecExtension;

typedef enum {
    CELL_GIFDEC_DEC_STATUS_FINISH       = 0,
    CELL_GIFDEC_DEC_STATUS_STOP         = 1,
} CellGifDecDecodeStatus;

typedef struct CellGifDecDataOutInfo {
    CellGifDecRecordType        recordType;
    CellGifDecExtension         outExtension;
    CellGifDecDecodeStatus      status;
} CellGifDecDataOutInfo;

typedef struct CellGifDecDataCtrlParam {
    uint64_t                    outputBytesPerLine;
} CellGifDecDataCtrlParam;

typedef void *CellGifDecMainHandle;
typedef void *CellGifDecSubHandle;

int32_t cellGifDecCreate(
    CellGifDecMainHandle             *mainHandle,
    const CellGifDecThreadInParam    *threadInParam,
    CellGifDecThreadOutParam         *threadOutParam
);

int32_t cellGifDecExtCreate(
    CellGifDecMainHandle             *mainHandle,
    const CellGifDecThreadInParam    *threadInParam,
    CellGifDecThreadOutParam         *threadOutParam,
    const CellGifDecExtThreadInParam *extThreadInParam,
    CellGifDecExtThreadOutParam      *extThreadOutParam
);

int32_t cellGifDecOpen(
    CellGifDecMainHandle              mainHandle,
    CellGifDecSubHandle              *subHandle,
    const CellGifDecSrc              *src,
    CellGifDecOpnInfo                *openInfo
);

int32_t cellGifDecReadHeader(
    CellGifDecMainHandle              mainHandle,
    CellGifDecSubHandle               subHandle,
    CellGifDecInfo                   *info
);

int32_t cellGifDecSetParameter(
    CellGifDecMainHandle              mainHandle,
    CellGifDecSubHandle               subHandle,
    const CellGifDecInParam          *inParam,
    CellGifDecOutParam               *outParam
);

int32_t cellGifDecDecodeData(
    CellGifDecMainHandle              mainHandle,
    CellGifDecSubHandle               subHandle,
    uint8_t                          *data,
    const CellGifDecDataCtrlParam    *dataCtrlParam,
    CellGifDecDataOutInfo            *dataOutInfo
);

int32_t cellGifDecClose(
    CellGifDecMainHandle              mainHandle,
    CellGifDecSubHandle               subHandle
);

int32_t cellGifDecDestroy(
    CellGifDecMainHandle              mainHandle
);


typedef struct CellGifDecStrmParam {
    void                        *strmPtr;
    uint32_t                     strmSize;
} CellGifDecStrmParam;

typedef struct CellGifDecStrmInfo {
    uint32_t                     decodedStrmSize;
} CellGifDecStrmInfo;

typedef int32_t (*CellGifDecCbControlStream)(
    CellGifDecStrmInfo          *strmInfo,
    CellGifDecStrmParam         *strmParam,
    void                        *cbCtrlStrmArg
);

typedef struct CellGifDecCbCtrlStrm {
    CellGifDecCbControlStream    cbCtrlStrmFunc;
    void                        *cbCtrlStrmArg;
} CellGifDecCbCtrlStrm;

typedef enum {
    CELL_GIFDEC_NO_INTERLACE    = 0,
    CELL_GIFDEC_INTERLACE       = 1
} CellGifDecInterlaceMode;

typedef struct CellGifDecDispInfo {
    uint64_t                     outputFrameWidthByte;
    uint32_t                     outputFrameHeight;
    uint64_t                     outputStartXByte;
    uint32_t                     outputStartY;
    uint64_t                     outputWidthByte;
    uint32_t                     outputHeight;
    uint32_t                     outputBitDepth;
    uint32_t                     outputComponents;
    uint32_t                     scanPassCount;
    void                        *outputImage;
    CellGifDecInterlaceMode      interlaceFlag;
} CellGifDecDispInfo;

typedef struct CellGifDecDispParam {
    void                        *nextOutputImage;
} CellGifDecDispParam;

typedef int32_t (*CellGifDecCbControlDisp)(
    CellGifDecDispInfo          *dispInfo,
    CellGifDecDispParam         *dispParam,
    void                        *cbCtrlDispArg
);

typedef struct CellGifDecCbCtrlDisp {
    CellGifDecCbControlDisp      cbCtrlDispFunc;
    void                        *cbCtrlDispArg;
} CellGifDecCbCtrlDisp;

typedef struct CellGifDecExtInfo {
    uint64_t                     reserved;
} CellGifDecExtInfo;

typedef enum {
    CELL_GIFDEC_LINE_MODE               = 1,
} CellGifDecBufferMode;

typedef enum {
    CELL_GIFDEC_RECEIVE_EVENT           = 0,
    CELL_GIFDEC_TRYRECEIVE_EVENT        = 1,
} CellGifDecSpuMode;

typedef struct CellGifDecExtInParam {
    CellGifDecBufferMode         bufferMode;
    uint32_t                     outputCounts;
    CellGifDecSpuMode            spuMode;
} CellGifDecExtInParam;

typedef struct CellGifDecExtOutParam {
    uint64_t                     outputWidthByte;
    uint32_t                     outputHeight;
} CellGifDecExtOutParam;

int32_t cellGifDecExtOpen(
    CellGifDecMainHandle             mainHandle,
    CellGifDecSubHandle             *subHandle,
    const CellGifDecSrc             *src,
    CellGifDecOpnInfo               *openInfo,
    const CellGifDecCbCtrlStrm      *cbCtrlStrm
);

int32_t cellGifDecExtReadHeader(
    CellGifDecMainHandle             mainHandle,
    CellGifDecSubHandle              subHandle,
    CellGifDecInfo                  *info,
    CellGifDecExtInfo               *extInfo
);

int32_t cellGifDecExtSetParameter(
    CellGifDecMainHandle             mainHandle,
    CellGifDecSubHandle              subHandle,
    const CellGifDecInParam         *inParam,
    CellGifDecOutParam              *outParam,
    const CellGifDecExtInParam      *extInParam,
    CellGifDecExtOutParam           *extOutParam
);

int32_t cellGifDecExtDecodeData(
    CellGifDecMainHandle             mainHandle,
    CellGifDecSubHandle              subHandle,
    uint8_t                         *data,
    const CellGifDecDataCtrlParam   *dataCtrlParam,
    CellGifDecDataOutInfo           *dataOutInfo,
    const CellGifDecCbCtrlDisp      *cbCtrlDisp,
    CellGifDecDispParam             *dispParam
);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* CELL_GIFDEC_H */
