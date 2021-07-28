/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 *               Copyright (C) 2008 Sony Computer Entertainment Inc.
 *                                              All Rights Reserved.
 */

#ifndef CELL_JPGDEC_H
#define CELL_JPGDEC_H

#include <stdint.h>

#include <sys/spu_thread.h>
#include <cell/spurs.h>
#include <cell/error.h>

#ifdef __cplusplus
extern "C" {
#endif /*E __cplusplus */

typedef void *CellJpgDecMainHandle;
typedef void *CellJpgDecSubHandle;

#define CELL_JPGDEC_STAT_OK  CELL_OK
/*E
 *E CELL_ERROR_FACILITY_CODEC    0x061
 *E libjpgcodec  : 0x8061_1101 - 0x8061_11ff
*/

#define CELL_JPGDEC_MAKE_ERROR(status) \
    CELL_ERROR_MAKE_ERROR(CELL_ERROR_FACILITY_CODEC, status)

/*E ERR: CELL_JPGDEC_MAKE_ERROR(0x1101) */
#define CELL_JPGDEC_ERROR_HEADER                 CELL_ERROR_CAST(0x80611101)

/*E ERR: CELL_JPGDEC_MAKE_ERROR(0x1102) */
#define CELL_JPGDEC_ERROR_STREAM_FORMAT          CELL_ERROR_CAST(0x80611102)

/*E ERR: CELL_JPGDEC_MAKE_ERROR(0x1103) */
#define CELL_JPGDEC_ERROR_ARG                    CELL_ERROR_CAST(0x80611103)

/*E ERR: CELL_JPGDEC_MAKE_ERROR(0x1104) */
#define CELL_JPGDEC_ERROR_SEQ                    CELL_ERROR_CAST(0x80611104)

/*E ERR: CELL_JPGDEC_MAKE_ERROR(0x1105) */
#define CELL_JPGDEC_ERROR_BUSY                   CELL_ERROR_CAST(0x80611105)

/*E ERR: CELL_JPGDEC_MAKE_ERROR(0x1106) */
#define CELL_JPGDEC_ERROR_FATAL                  CELL_ERROR_CAST(0x80611106)

/*E ERR: CELL_JPGDEC_MAKE_ERROR(0x1107) */
#define CELL_JPGDEC_ERROR_OPEN_FILE              CELL_ERROR_CAST(0x80611107)

/*E ERR: CELL_JPGDEC_MAKE_ERROR(0x1108) */
#define CELL_JPGDEC_ERROR_SPU_UNSUPPORT          CELL_ERROR_CAST(0x80611108)

/*E ERR: CELL_JPGDEC_MAKE_ERROR(0x1109) */
#define CELL_JPGDEC_ERROR_CB_PARAM               CELL_ERROR_CAST(0x80611109)

typedef enum {
    CELL_JPGDEC_FILE   = 0, 
    CELL_JPGDEC_BUFFER = 1  
} CellJpgDecStreamSrcSel;

typedef enum {
    CELL_JPGDEC_SPU_THREAD_DISABLE = 0, 
    CELL_JPGDEC_SPU_THREAD_ENABLE  = 1     
} CellJpgDecSpuThreadEna;

typedef enum {
    CELL_JPGDEC_QUALITY = 0,  
    CELL_JPGDEC_FAST    = 5      
} CellJpgDecMethod;

typedef enum {
    CELL_JPGDEC_TOP_TO_BOTTOM = 0, 
    CELL_JPGDEC_BOTTOM_TO_TOP = 1  
} CellJpgDecOutputMode;

typedef enum {
    CELL_JPG_UNKNOWN                 = 0,
    CELL_JPG_GRAYSCALE               = 1,
    CELL_JPG_RGB                     = 2,
    CELL_JPG_YCbCr                   = 3,
    CELL_JPG_RGBA                    = 10,
    CELL_JPG_UPSAMPLE_ONLY           = 11,
    CELL_JPG_ARGB                    = 20,
    CELL_JPG_GRAYSCALE_TO_ALPHA_RGBA = 40,
    CELL_JPG_GRAYSCALE_TO_ALPHA_ARGB = 41
} CellJpgDecColorSpace;

typedef enum {
    CELL_JPGDEC_CONTINUE = 0,
    CELL_JPGDEC_STOP     = 1
} CellJpgDecCommand;

typedef enum {
    CELL_JPGDEC_DEC_STATUS_FINISH = 0,
    CELL_JPGDEC_DEC_STATUS_STOP   = 1
} CellJpgDecDecodeStatus;

typedef void 
*(*CellJpgDecCbControlMalloc)(
    uint32_t                           size,
    void                              *cbCtrlMallocArg );

typedef int32_t
(*CellJpgDecCbControlFree)(
    void                              *ptr,
    void                              *cbCtrlFreeArg );

typedef struct CellJpgDecThreadInParam {
    CellJpgDecSpuThreadEna             spuThreadEnable;
    uint32_t                           ppuThreadPriority;
    uint32_t                           spuThreadPriority;
    CellJpgDecCbControlMalloc          cbCtrlMallocFunc;
    void                              *cbCtrlMallocArg;
    CellJpgDecCbControlFree            cbCtrlFreeFunc;
    void                              *cbCtrlFreeArg;
} CellJpgDecThreadInParam;

typedef struct CellJpgDecThreadOutParam {
    uint32_t                           jpegCodecVersion;
} CellJpgDecThreadOutParam;

typedef struct CellJpgDecSrc {
    CellJpgDecStreamSrcSel             srcSelect;
    const char                        *fileName;
    int64_t                            fileOffset;
    uint32_t                           fileSize;
    void                              *streamPtr;
    uint32_t                           streamSize;
    CellJpgDecSpuThreadEna             spuThreadEnable;
} CellJpgDecSrc;

typedef struct CellJpgDecOpnInfo {
    uint32_t                           initSpaceAllocated;
} CellJpgDecOpnInfo;

typedef struct CellJpgDecInfo {
    uint32_t                           imageWidth;
    uint32_t                           imageHeight;
    uint32_t                           numComponents;
    CellJpgDecColorSpace               jpegColorSpace;
} CellJpgDecInfo;

typedef struct CellJpgDecInParam {
    volatile CellJpgDecCommand        *commandPtr;
    uint32_t                           downScale;
    CellJpgDecMethod                   method;
    CellJpgDecOutputMode               outputMode;
    CellJpgDecColorSpace               outputColorSpace;
    uint8_t                            outputColorAlpha;
    uint8_t                            reserved[3];
} CellJpgDecInParam;

typedef struct CellJpgDecOutParam {
    uint64_t                           outputWidthByte;
    uint32_t                           outputWidth;
    uint32_t                           outputHeight;
    uint32_t                           outputComponents;
    CellJpgDecOutputMode               outputMode;
    CellJpgDecColorSpace               outputColorSpace;
    uint32_t                           downScale;
    uint32_t                           useMemorySpace;
} CellJpgDecOutParam;

typedef struct CellJpgDecDataOutInfo {
    float                              mean;
    uint32_t                           outputLines;
    CellJpgDecDecodeStatus             status;
} CellJpgDecDataOutInfo;

typedef struct CellJpgDecDataCtrlParam {
    uint64_t                           outputBytesPerLine;
} CellJpgDecDataCtrlParam;

int32_t
cellJpgDecCreate(
    CellJpgDecMainHandle              *mainHandle,
    const CellJpgDecThreadInParam     *threadInParam,
    CellJpgDecThreadOutParam          *threadOutParam );

int32_t
cellJpgDecOpen(
    CellJpgDecMainHandle               mainHandle, 
    CellJpgDecSubHandle               *subHandle, 
    const CellJpgDecSrc               *src, 
    CellJpgDecOpnInfo                 *openInfo );

int32_t
cellJpgDecReadHeader(
    CellJpgDecMainHandle               mainHandle, 
    CellJpgDecSubHandle                subHandle, 
    CellJpgDecInfo                    *info );

int32_t
cellJpgDecSetParameter(
    CellJpgDecMainHandle               mainHandle, 
    CellJpgDecSubHandle                subHandle, 
    const CellJpgDecInParam           *inParam, 
    CellJpgDecOutParam                *outParam );

int32_t
cellJpgDecDecodeData(
    CellJpgDecMainHandle               mainHandle, 
    CellJpgDecSubHandle                subHandle, 
    uint8_t                           *data, 
    const CellJpgDecDataCtrlParam     *dataCtrlParam,
    CellJpgDecDataOutInfo             *dataOutInfo );

int32_t
cellJpgDecClose(
    CellJpgDecMainHandle               mainHandle, 
    CellJpgDecSubHandle                subHandle );

int32_t
cellJpgDecDestroy(
    CellJpgDecMainHandle               mainHandle );

typedef enum {
    CELL_JPGDEC_MCU_MODE   = 0,      
    CELL_JPGDEC_LINE_MODE  = 1  
} CellJpgDecBufferMode;

typedef enum {
    CELL_JPGDEC_RECEIVE_EVENT    = 0,
    CELL_JPGDEC_TRYRECEIVE_EVENT = 1
} CellJpgDecSpuMode;

typedef struct CellJpgDecStrmInfo{
    uint32_t                           decodedStrmSize;
} CellJpgDecStrmInfo; 

typedef struct CellJpgDecStrmParam {
    void                              *strmPtr;
    uint32_t                           strmSize;
} CellJpgDecStrmParam;

typedef struct CellJpgDecDispInfo{
    uint64_t                           outputFrameWidthByte;
    uint32_t                           outputFrameHeight;
    uint64_t                           outputStartXByte;
    uint32_t                           outputStartY;
    uint64_t                           outputWidthByte;
    uint32_t                           outputHeight;
    uint32_t                           outputComponents;
    void                              *outputImage;
} CellJpgDecDispInfo; 

typedef struct CellJpgDecDispParam {
    void                              *nextOutputImage;
} CellJpgDecDispParam;

typedef int32_t
(*CellJpgDecCbControlStream)(
    CellJpgDecStrmInfo                *strmInfo,
    CellJpgDecStrmParam               *strmParam,
    void                              *cbCtrlStrmArg );

typedef int32_t
(*CellJpgDecCbControlDisp)(
    CellJpgDecDispInfo                *dispInfo,
    CellJpgDecDispParam               *dispParam,
    void                              *cbCtrlDispArg );

typedef struct CellJpgDecCbCtrlStrm{
    CellJpgDecCbControlStream          cbCtrlStrmFunc;
    void                              *cbCtrlStrmArg;
} CellJpgDecCbCtrlStrm; 

typedef struct CellJpgDecCbCtrlDisp{
    CellJpgDecCbControlDisp            cbCtrlDispFunc;
    void                              *cbCtrlDispArg;
} CellJpgDecCbCtrlDisp; 

typedef struct CellJpgDecExtThreadInParam {
    CellSpurs                         *spurs;
    uint8_t                            priority[8];
    uint32_t                           maxContention; 
} CellJpgDecExtThreadInParam;

typedef struct CellJpgDecExtThreadOutParam {
    uint32_t                           reserved;
} CellJpgDecExtThreadOutParam;

typedef struct CellJpgDecExtInfo {
    uint64_t                           coefBufferSize;
    uint32_t                           mcuWidth; 
} CellJpgDecExtInfo;

typedef struct CellJpgDecExtInParam {
    void                              *coefBufferPtr;
    CellJpgDecBufferMode               bufferMode;
    uint32_t                           outputCounts;
    CellJpgDecSpuMode                  spuMode;
} CellJpgDecExtInParam;

typedef struct CellJpgDecExtOutParam {
    uint64_t                           outputWidthByte; 
    uint32_t                           outputHeight;
    uint32_t                           oneMcuWidth;
    uint32_t                           oneMcuHeight;
} CellJpgDecExtOutParam;

int32_t 
cellJpgDecExtCreate(
    CellJpgDecMainHandle              *mainHandle,
    const CellJpgDecThreadInParam     *threadInParam,
    CellJpgDecThreadOutParam          *threadOutParam,
    const CellJpgDecExtThreadInParam  *extThreadInParam,
    CellJpgDecExtThreadOutParam       *extThreadOutParam );

int32_t
cellJpgDecExtOpen(
    CellJpgDecMainHandle               mainHandle, 
    CellJpgDecSubHandle               *subHandle, 
    const CellJpgDecSrc               *src, 
    CellJpgDecOpnInfo                 *openInfo,
    const CellJpgDecCbCtrlStrm        *cbCtrlStrm );

int32_t
cellJpgDecExtReadHeader(
    CellJpgDecMainHandle               mainHandle, 
    CellJpgDecSubHandle                subHandle, 
    CellJpgDecInfo                    *info,
    CellJpgDecExtInfo                 *extInfo );
                                  
int32_t
cellJpgDecExtSetParameter(
    CellJpgDecMainHandle               mainHandle, 
    CellJpgDecSubHandle                subHandle, 
    const CellJpgDecInParam           *inParam, 
    CellJpgDecOutParam                *outParam,
    const CellJpgDecExtInParam        *extInParam, 
    CellJpgDecExtOutParam             *extOutParam );

int32_t
cellJpgDecExtDecodeData(
    CellJpgDecMainHandle               mainHandle, 
    CellJpgDecSubHandle                subHandle,
    uint8_t                           *data, 
    const CellJpgDecDataCtrlParam     *dataCtrlParam,
    CellJpgDecDataOutInfo             *dataOutInfo, 
    const CellJpgDecCbCtrlDisp        *cbCtrlDisp,
    CellJpgDecDispParam               *dispParam );

#ifdef __cplusplus
}
#endif /*E __cplusplus */

#endif /*E CELL_JPGDEC_H */
