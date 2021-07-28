/* SCE CONFIDENTIAL
 PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2009 Sony Computer Entertainment Inc.
 * All Rights Reserved.
 */

#ifndef CELL_PNGENC_H
#define CELL_PNGENC_H

#include <stdint.h>
#include <stdbool.h>
#include <cell/spurs.h>
#include <cell/error.h>
#include <cell/codec/pngcom.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define CELL_PNGENC_ERROR_ARG                  CELL_ERROR_CAST(0x80611291)
#define CELL_PNGENC_ERROR_SEQ                  CELL_ERROR_CAST(0x80611292)
#define CELL_PNGENC_ERROR_BUSY                 CELL_ERROR_CAST(0x80611293)
#define CELL_PNGENC_ERROR_EMPTY                CELL_ERROR_CAST(0x80611294)
#define CELL_PNGENC_ERROR_RESET                CELL_ERROR_CAST(0x80611295)
#define CELL_PNGENC_ERROR_FATAL                CELL_ERROR_CAST(0x80611296)

#define CELL_PNGENC_ERROR_STREAM_ABORT         CELL_ERROR_CAST(0x806112A1)
#define CELL_PNGENC_ERROR_STREAM_SKIP          CELL_ERROR_CAST(0x806112A2)
#define CELL_PNGENC_ERROR_STREAM_OVERFLOW      CELL_ERROR_CAST(0x806112A3)
#define CELL_PNGENC_ERROR_STREAM_FILE_OPEN     CELL_ERROR_CAST(0x806112A4)

typedef enum {
    CELL_PNGENC_COLOR_SPACE_GRAYSCALE       = 1,
    CELL_PNGENC_COLOR_SPACE_RGB             = 2,
    CELL_PNGENC_COLOR_SPACE_PALETTE         = 4,
    CELL_PNGENC_COLOR_SPACE_GRAYSCALE_ALPHA = 9,
    CELL_PNGENC_COLOR_SPACE_RGBA            = 10,
    CELL_PNGENC_COLOR_SPACE_ARGB            = 20
} CellPngEncColorSpace;

typedef enum {
    CELL_PNGENC_COMPR_LEVEL_0,
    CELL_PNGENC_COMPR_LEVEL_1,
    CELL_PNGENC_COMPR_LEVEL_2,
    CELL_PNGENC_COMPR_LEVEL_3,
    CELL_PNGENC_COMPR_LEVEL_4,
    CELL_PNGENC_COMPR_LEVEL_5,
    CELL_PNGENC_COMPR_LEVEL_6,
    CELL_PNGENC_COMPR_LEVEL_7,
    CELL_PNGENC_COMPR_LEVEL_8,
    CELL_PNGENC_COMPR_LEVEL_9
} CellPngEncCompressionLevel;

typedef enum {
    CELL_PNGENC_FILTER_TYPE_NONE  = 0x08,
    CELL_PNGENC_FILTER_TYPE_SUB   = 0x10,
    CELL_PNGENC_FILTER_TYPE_UP    = 0x20,
    CELL_PNGENC_FILTER_TYPE_AVG   = 0x40,
    CELL_PNGENC_FILTER_TYPE_PAETH = 0x80,
    CELL_PNGENC_FILTER_TYPE_ALL   = 0xF8
} CellPngEncFilterType;

typedef enum {
    CELL_PNGENC_CHUNK_TYPE_PLTE,      /* PLTE */
    CELL_PNGENC_CHUNK_TYPE_TRNS,      /* tRNS */
    CELL_PNGENC_CHUNK_TYPE_CHRM,      /* cHRM */
    CELL_PNGENC_CHUNK_TYPE_GAMA,      /* gAMA */
    CELL_PNGENC_CHUNK_TYPE_ICCP,      /* iCCP */
    CELL_PNGENC_CHUNK_TYPE_SBIT,      /* sBIT */
    CELL_PNGENC_CHUNK_TYPE_SRGB,      /* sRGB */
    CELL_PNGENC_CHUNK_TYPE_TEXT,      /* tEXt,zTXt,iTXt */
    CELL_PNGENC_CHUNK_TYPE_BKGD,      /* bKGD */
    CELL_PNGENC_CHUNK_TYPE_HIST,      /* hIST */
    CELL_PNGENC_CHUNK_TYPE_PHYS,      /* pHYs */
    CELL_PNGENC_CHUNK_TYPE_SPLT,      /* sPLT */
    CELL_PNGENC_CHUNK_TYPE_TIME,      /* tIME */
    CELL_PNGENC_CHUNK_TYPE_OFFS,      /* oFFs */
    CELL_PNGENC_CHUNK_TYPE_PCAL,      /* pCAL */
    CELL_PNGENC_CHUNK_TYPE_SCAL,      /* sCAL */
    CELL_PNGENC_CHUNK_TYPE_UNKNOWN    /* other chunks */
} CellPngEncChunkType;

typedef enum {
    CELL_PNGENC_LOCATION_FILE,
    CELL_PNGENC_LOCATION_BUFFER
} CellPngEncLocation;


typedef void *CellPngEncHandle;

typedef struct CellPngEncExParam{
    int32_t   index;
    intptr_t  value;
} CellPngEncExParam;

typedef struct CellPngEncConfig{
    uint32_t            maxWidth;
    uint32_t            maxHeight;
    uint32_t            maxBitDepth;
    bool                enableSpu;
    uint32_t            addMemSize;
    CellPngEncExParam  *exParamList;
    uint32_t            exParamNum;
} CellPngEncConfig;

typedef struct CellPngEncAttr{
    size_t    memSize;
    uint8_t   cmdQueueDepth;
    uint32_t  versionUpper;
    uint32_t  versionLower;
} CellPngEncAttr;

typedef struct CellPngEncResource{
    void     *memAddr;
    size_t    memSize;
    int32_t   ppuThreadPriority;
    int32_t   spuThreadPriority;
} CellPngEncResource;

typedef struct CellPngEncResourceEx{
    void      *memAddr;
    size_t     memSize;
    int32_t    ppuThreadPriority;
    CellSpurs *spurs;
    uint8_t    priority[8];
} CellPngEncResourceEx;

typedef struct CellPngEncPicture{
    uint32_t              width;
    uint32_t              height;
    uint32_t              pitchWidth;
    CellPngEncColorSpace  colorSpace;
    uint32_t              bitDepth;
    bool                  packedPixel;
    void                 *pictureAddr;
    uint64_t              userData;
} CellPngEncPicture;

typedef struct CellPngEncAncillaryChunk{
    CellPngEncChunkType  chunkType;
    void                *chunkData;
} CellPngEncAncillaryChunk;

typedef struct CellPngEncEncodeParam{
    bool                        enableSpu;
    CellPngEncColorSpace        encodeColorSpace;
    CellPngEncCompressionLevel  compressionLevel;
    uint32_t                    filterType;
    CellPngEncAncillaryChunk   *ancillaryChunkList;
    uint32_t                    ancillaryChunkNum;
} CellPngEncEncodeParam;

typedef struct CellPngEncOutputParam{
    CellPngEncLocation  location;
    const char         *streamFileName;
    void               *streamAddr;
    size_t              limitSize;
} CellPngEncOutputParam;

typedef struct CellPngEncStreamInfo{
    int32_t             state;
    CellPngEncLocation  location;
    const char         *streamFileName;
    void               *streamAddr;
    size_t              limitSize;
    size_t              streamSize;
    uint32_t            processedLine;
    uint64_t            userData;
} CellPngEncStreamInfo;


int32_t cellPngEncQueryAttr(
    const CellPngEncConfig *config,
    CellPngEncAttr         *attr
);

int32_t cellPngEncOpen(
    const CellPngEncConfig   *config,
    const CellPngEncResource *resource,
    CellPngEncHandle         *handle
);

int32_t cellPngEncOpenEx(
    const CellPngEncConfig     *config,
    const CellPngEncResourceEx *resourceEx,
    CellPngEncHandle           *handle
);

int32_t cellPngEncWaitForInput(
    CellPngEncHandle  handle,
    bool              block
);

int32_t cellPngEncEncodePicture(
    CellPngEncHandle             handle,
    const CellPngEncPicture     *picture,
    const CellPngEncEncodeParam *encodeParam,
    const CellPngEncOutputParam *outputParam
);

int32_t cellPngEncWaitForOutput(
    CellPngEncHandle  handle,
    uint32_t         *streamInfoNum,
    bool              block
);

int32_t cellPngEncGetStreamInfo(
    CellPngEncHandle      handle,
    CellPngEncStreamInfo *streamInfo
);

int32_t cellPngEncReset(
    CellPngEncHandle  handle
);

int32_t cellPngEncClose(
    CellPngEncHandle  handle
);


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* CELL_PNGENC_H */
