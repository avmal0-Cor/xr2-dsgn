/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2008 Sony Computer Entertainment Inc.
 * All Rights Reserved.
 */

#ifndef CELL_PNGDEC_H
#define CELL_PNGDEC_H

#include <stdint.h>
#include <sys/spu_thread.h>
#include <cell/spurs.h>
#include <cell/error.h>
#include <cell/codec/pngcom.h>

#ifdef __cplusplus
extern "C" {
#endif /*E __cplusplus */

#define CELL_PNGDEC_MAKE_ERROR(status) \
	CELL_ERROR_MAKE_ERROR(CELL_ERROR_FACILITY_CODEC, status)

/*E ERR: CELL_PNGDEC_MAKE_ERROR(0x1201) */
#define CELL_PNGDEC_ERROR_HEADER		 CELL_ERROR_CAST(0x80611201)

/*E ERR: CELL_PNGDEC_MAKE_ERROR(0x1202) */
#define CELL_PNGDEC_ERROR_STREAM_FORMAT	 CELL_ERROR_CAST(0x80611202)

/*E ERR: CELL_PNGDEC_MAKE_ERROR(0x1203) */
#define CELL_PNGDEC_ERROR_ARG	         CELL_ERROR_CAST(0x80611203)

/*E ERR: CELL_PNGDEC_MAKE_ERROR(0x1204) */
#define CELL_PNGDEC_ERROR_SEQ	         CELL_ERROR_CAST(0x80611204)

/*E ERR: CELL_PNGDEC_MAKE_ERROR(0x1205) */
#define CELL_PNGDEC_ERROR_BUSY	         CELL_ERROR_CAST(0x80611205)

/*E ERR: CELL_PNGDEC_MAKE_ERROR(0x1206) */
#define CELL_PNGDEC_ERROR_FATAL	         CELL_ERROR_CAST(0x80611206)

/*E ERR: CELL_PNGDEC_MAKE_ERROR(0x1207) */
#define CELL_PNGDEC_ERROR_OPEN_FILE	     CELL_ERROR_CAST(0x80611207)

/*E ERR: CELL_PNGDEC_MAKE_ERROR(0x1208) */
#define CELL_PNGDEC_ERROR_SPU_UNSUPPORT	 CELL_ERROR_CAST(0x80611208)

/*E ERR: CELL_PNGDEC_MAKE_ERROR(0x1209) */
#define CELL_PNGDEC_ERROR_SPU_ERROR	     CELL_ERROR_CAST(0x80611209)

/*E ERR: CELL_PNGDEC_MAKE_ERROR(0x120a) */
#define CELL_PNGDEC_ERROR_CB_PARAM       CELL_ERROR_CAST(0x8061120a)

typedef void *CellPngDecMainHandle;
typedef void *CellPngDecSubHandle;

typedef enum {
    CELL_PNGDEC_FILE      = 0, 
    CELL_PNGDEC_BUFFER    = 1     
} CellPngDecStreamSrcSel;

typedef enum {
    CELL_PNGDEC_SPU_THREAD_DISABLE    = 0,    
    CELL_PNGDEC_SPU_THREAD_ENABLE     = 1        
} CellPngDecSpuThreadEna;

typedef enum {
    CELL_PNGDEC_TOP_TO_BOTTOM    = 0,    
    CELL_PNGDEC_BOTTOM_TO_TOP    = 1        
} CellPngDecOutputMode;

typedef enum {
    CELL_PNGDEC_GRAYSCALE       = 1,        
    CELL_PNGDEC_RGB             = 2,        
    CELL_PNGDEC_PALETTE         = 4, 
    CELL_PNGDEC_GRAYSCALE_ALPHA = 9,        
    CELL_PNGDEC_RGBA            = 10,        
    CELL_PNGDEC_ARGB            = 20        
} CellPngDecColorSpace;

typedef enum {
    CELL_PNGDEC_NO_INTERLACE    = 0,        
    CELL_PNGDEC_ADAM7_INTERLACE = 1    
} CellPngDecInterlaceMode;

typedef enum {
    CELL_PNGDEC_1BYTE_PER_NPIXEL = 0,        
    CELL_PNGDEC_1BYTE_PER_1PIXEL = 1    
} CellPngDecPackFlag;

typedef enum {
    CELL_PNGDEC_STREAM_ALPHA    = 0,        
    CELL_PNGDEC_FIX_ALPHA       = 1    
} CellPngDecAlphaSelect;

typedef enum {
    CELL_PNGDEC_CONTINUE        = 0,        
    CELL_PNGDEC_STOP            = 1    
} CellPngDecCommand;

typedef enum {
    CELL_PNGDEC_DEC_STATUS_FINISH        = 0,        
    CELL_PNGDEC_DEC_STATUS_STOP          = 1    
} CellPngDecDecodeStatus;

typedef void *(*CellPngDecCbControlMalloc)(
										   uint32_t  size,
										   void      *cbCtrlMallocArg
										   );

typedef int32_t(*CellPngDecCbControlFree)(
										  void       *ptr,
										  void       *cbCtrlFreeArg
										  );

/*E Decoder : Structurs */
typedef struct CellPngDecThreadInParam {
    CellPngDecSpuThreadEna         spuThreadEnable;
    uint32_t                       ppuThreadPriority;
    uint32_t                       spuThreadPriority;
	CellPngDecCbControlMalloc      cbCtrlMallocFunc;
    void                           *cbCtrlMallocArg;
    CellPngDecCbControlFree        cbCtrlFreeFunc;
    void                           *cbCtrlFreeArg;
} CellPngDecThreadInParam;

typedef struct CellPngDecExtThreadInParam {
    CellSpurs                      *spurs;
    uint8_t                        priority[8];
    uint32_t                       maxContention;
} CellPngDecExtThreadInParam;

typedef struct CellPngDecThreadOutParam {
    uint32_t         pngCodecVersion;
} CellPngDecThreadOutParam;

typedef struct CellPngDecExtThreadOutParam {
    uint32_t         reserved;
} CellPngDecExtThreadOutParam;

typedef struct CellPngDecSrc {
    CellPngDecStreamSrcSel        srcSelect;
    const char                    *fileName;
    int64_t                       fileOffset;
    uint32_t                      fileSize;
    void                          *streamPtr;
    uint32_t                      streamSize;
    CellPngDecSpuThreadEna        spuThreadEnable;
} CellPngDecSrc;

typedef struct CellPngDecOpnInfo {
    uint32_t                      initSpaceAllocated;
} CellPngDecOpnInfo;

typedef struct CellPngDecInfo {
    uint32_t                      imageWidth;
    uint32_t                      imageHeight;
    uint32_t                      numComponents;
    CellPngDecColorSpace          colorSpace;
    uint32_t                      bitDepth;
	CellPngDecInterlaceMode       interlaceMethod;
	uint32_t                      chunkInformation;
} CellPngDecInfo;

typedef struct CellPngDecInParam {
    volatile CellPngDecCommand   *commandPtr;
    CellPngDecOutputMode         outputMode;
    CellPngDecColorSpace         outputColorSpace;
    uint32_t                     outputBitDepth;
    CellPngDecPackFlag           outputPackFlag;
    CellPngDecAlphaSelect        outputAlphaSelect;
    uint32_t                     outputColorAlpha;
} CellPngDecInParam;

typedef struct CellPngDecOutParam {
    uint64_t                  outputWidthByte;
    uint32_t                  outputWidth;
    uint32_t                  outputHeight;
    uint32_t                  outputComponents;
    uint32_t                  outputBitDepth;
    CellPngDecOutputMode      outputMode;
    CellPngDecColorSpace      outputColorSpace;
    uint32_t                  useMemorySpace;
} CellPngDecOutParam;

typedef struct CellPngDecDataCtrlParam {
	uint64_t                      outputBytesPerLine;
} CellPngDecDataCtrlParam;

typedef struct CellPngDecDataOutInfo {
	uint32_t                      chunkInformation;
	uint32_t                      numText; 
	uint32_t                      numUnknownChunk;
    CellPngDecDecodeStatus        status;
} CellPngDecDataOutInfo;

/*E Decoder Functions */
int32_t cellPngDecCreate(
						 CellPngDecMainHandle              *mainHandle,
						 const CellPngDecThreadInParam     *threadInParam,
						 CellPngDecThreadOutParam          *threadOutParam
						 );

int32_t cellPngDecExtCreate(
						 CellPngDecMainHandle              *mainHandle,
						 const CellPngDecThreadInParam     *threadInParam,
						 CellPngDecThreadOutParam          *threadOutParam,
						 const CellPngDecExtThreadInParam  *extThreadInParam,
						 CellPngDecExtThreadOutParam       *extThreadOutParam
						 );

int32_t cellPngDecOpen(
					   CellPngDecMainHandle                mainHandle, 
					   CellPngDecSubHandle                 *subHandle, 
					   const CellPngDecSrc                 *src, 
					   CellPngDecOpnInfo                   *openInfo
					   );

int32_t cellPngDecReadHeader(
							 CellPngDecMainHandle          mainHandle, 
							 CellPngDecSubHandle           subHandle, 
							 CellPngDecInfo                *info
							 );

int32_t cellPngDecSetParameter(
							   CellPngDecMainHandle        mainHandle, 
							   CellPngDecSubHandle         subHandle, 
							   const CellPngDecInParam     *inParam, 
							   CellPngDecOutParam          *outParam
							   );

int32_t cellPngDecDecodeData(
							 CellPngDecMainHandle          mainHandle, 
							 CellPngDecSubHandle           subHandle, 
							 uint8_t                       *data,
							 const CellPngDecDataCtrlParam *dataCtrlParam,
							 CellPngDecDataOutInfo         *dataOutInfo
							 );

int32_t cellPngDecClose(
						CellPngDecMainHandle               mainHandle, 
						CellPngDecSubHandle                subHandle
						);

int32_t cellPngDecDestroy(
						  CellPngDecMainHandle             mainHandle
						  );

/*E Decoder Ancillary Chunk Functions */
int32_t cellPngDecGetTextChunk(
							   CellPngDecMainHandle          mainHandle, 
							   CellPngDecSubHandle           subHandle, 
							   uint32_t                      *textInfoNum,
							   CellPngTextInfo               *textInfo[]
							   );

int32_t cellPngDecGetPLTE(
						  CellPngDecMainHandle          mainHandle, 
						  CellPngDecSubHandle           subHandle, 
						  CellPngPLTE                   *plte
						  );

int32_t cellPngDecGetgAMA(
						  CellPngDecMainHandle          mainHandle, 
						  CellPngDecSubHandle           subHandle, 
						  CellPngGAMA                   *gama
						  );

int32_t cellPngDecGetsRGB(
						  CellPngDecMainHandle          mainHandle, 
						  CellPngDecSubHandle           subHandle,
						  CellPngSRGB                   *srgb
						  );

int32_t cellPngDecGetiCCP(
						  CellPngDecMainHandle          mainHandle, 
						  CellPngDecSubHandle           subHandle,
						  CellPngICCP                   *iccp
						  );

int32_t cellPngDecGetsBIT(
						  CellPngDecMainHandle          mainHandle, 
						  CellPngDecSubHandle           subHandle,
						  CellPngSBIT                   *sbit
						  );

int32_t cellPngDecGettRNS(
						  CellPngDecMainHandle          mainHandle, 
						  CellPngDecSubHandle           subHandle,
						  CellPngTRNS                   *trns
						  );

int32_t cellPngDecGethIST(
						  CellPngDecMainHandle          mainHandle, 
						  CellPngDecSubHandle           subHandle,
						  CellPngHIST                   *hist
						  );

int32_t cellPngDecGettIME(
						  CellPngDecMainHandle          mainHandle, 
						  CellPngDecSubHandle           subHandle,
						  CellPngTIME                   *time
						  );

int32_t cellPngDecGetbKGD(
						  CellPngDecMainHandle          mainHandle, 
						  CellPngDecSubHandle           subHandle,  
						  CellPngBKGD                   *bkgd
						  );

int32_t cellPngDecGetsPLT(
						  CellPngDecMainHandle          mainHandle, 
						  CellPngDecSubHandle           subHandle,  
						  CellPngSPLT                   *splt
						  );

int32_t cellPngDecGetoFFs(
						  CellPngDecMainHandle          mainHandle, 
						  CellPngDecSubHandle           subHandle,  
						  CellPngOFFS                   *offs
						  );

int32_t cellPngDecGetpHYs(
						  CellPngDecMainHandle          mainHandle, 
						  CellPngDecSubHandle           subHandle, 
						  CellPngPHYS                   *phys
						  );

int32_t cellPngDecGetsCAL(
						  CellPngDecMainHandle          mainHandle, 
						  CellPngDecSubHandle           subHandle, 
						  CellPngSCAL                   *scal
						  );

int32_t cellPngDecGetcHRM(
						  CellPngDecMainHandle          mainHandle, 
						  CellPngDecSubHandle           subHandle,  
						  CellPngCHRM                   *chrm
						  );

int32_t cellPngDecGetpCAL(
						  CellPngDecMainHandle          mainHandle, 
						  CellPngDecSubHandle           subHandle,  
						  CellPngPCAL                   *pcal
						  );



int32_t cellPngDecGetUnknownChunks(
								   CellPngDecMainHandle          mainHandle, 
								   CellPngDecSubHandle           subHandle,  
								   CellPngUnknownChunk           *unknownChunk[],
								   uint32_t                      *unknownChunkNumber
								   );


typedef enum {
    CELL_PNGDEC_LINE_MODE    = 1
} CellPngDecBufferMode;

typedef enum {
    CELL_PNGDEC_RECEIVE_EVENT       = 0,
    CELL_PNGDEC_TRYRECEIVE_EVENT    = 1
} CellPngDecSpuMode;

typedef struct CellPngDecStrmInfo{
	uint32_t         decodedStrmSize;
} CellPngDecStrmInfo;	

typedef struct CellPngDecStrmParam {
	void             *strmPtr;
	uint32_t         strmSize;
} CellPngDecStrmParam;

typedef int32_t(*CellPngDecCbControlStream)(
											CellPngDecStrmInfo        *strmInfo,
											CellPngDecStrmParam       *strmParam,
											void                      *cbCtrlStrmArg
											);

typedef struct CellPngDecDispInfo{
	uint64_t                    outputFrameWidthByte;
	uint32_t                    outputFrameHeight;
	uint64_t                    outputStartXByte;
	uint32_t                    outputStartY;
	uint64_t                    outputWidthByte;
	uint32_t                    outputHeight;
	uint32_t                    outputBitDepth;
	uint32_t                    outputComponents;
	uint32_t                    nextOutputStartY;
	uint32_t                    scanPassCount;
	void                        *outputImage;
} CellPngDecDispInfo;	

typedef struct CellPngDecDispParam {
	void                        *nextOutputImage;
} CellPngDecDispParam;

typedef int32_t(*CellPngDecCbControlDisp)(
										  CellPngDecDispInfo        *dispInfo,
										  CellPngDecDispParam       *dispParam,
										  void                      *cbCtrlDispArg
										  );

typedef struct CellPngDecOpnParam {
    uint32_t                    selectChunk;
} CellPngDecOpnParam;

typedef struct CellPngDecCbCtrlStrm{
	CellPngDecCbControlStream   cbCtrlStrmFunc;
	void                        *cbCtrlStrmArg;
} CellPngDecCbCtrlStrm;	

typedef struct CellPngDecExtInfo {
	uint64_t	            reserved;
} CellPngDecExtInfo;

typedef struct CellPngDecExtInParam {
	CellPngDecBufferMode    bufferMode;
	uint32_t                outputCounts;
	CellPngDecSpuMode       spuMode;
} CellPngDecExtInParam;

typedef struct CellPngDecExtOutParam {
	uint64_t                outputWidthByte;
	uint32_t                outputHeight;
} CellPngDecExtOutParam;

typedef struct CellPngDecCbCtrlDisp{
	CellPngDecCbControlDisp   cbCtrlDispFunc;
	void                      *cbCtrlDispArg;
} CellPngDecCbCtrlDisp;	

int32_t cellPngDecExtOpen(
						  CellPngDecMainHandle 	             mainHandle, 
						  CellPngDecSubHandle 	             *subHandle, 
						  const CellPngDecSrc 		         *src, 
						  CellPngDecOpnInfo 	             *openInfo,
						  const CellPngDecCbCtrlStrm         *cbCtrlStrm,
						  const CellPngDecOpnParam           *opnParam
						  );
int32_t cellPngDecExtReadHeader(
								CellPngDecMainHandle         mainHandle, 
								CellPngDecSubHandle 	     subHandle, 
								CellPngDecInfo 		         *info,
								CellPngDecExtInfo            *extInfo
								);
int32_t cellPngDecExtSetParameter(
								  CellPngDecMainHandle       mainHandle, 
								  CellPngDecSubHandle        subHandle, 
								  const CellPngDecInParam 	 *inParam, 
								  CellPngDecOutParam 	     *outParam,
								  const CellPngDecExtInParam *extInParam, 
								  CellPngDecExtOutParam      *extOutParam
								  );

int32_t cellPngDecExtDecodeData(
								CellPngDecMainHandle          mainHandle, 
								CellPngDecSubHandle           subHandle,
								uint8_t                       *data, 
								const CellPngDecDataCtrlParam *dataCtrlParam,
								CellPngDecDataOutInfo 	      *dataOutInfo,
								const CellPngDecCbCtrlDisp    *cbCtrlDisp,
								CellPngDecDispParam           *dispParam
								);

#ifdef __cplusplus
}
#endif /*E __cplusplus */

#endif /*E CELL_PNGDEC_H */
