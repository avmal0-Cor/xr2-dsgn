/* 
   SCE CONFIDENTIAL
   PlayStation(R)3 Programmer Tool Runtime Library 310.001
   Copyright (C) 2008 Sony Computer Entertainment Inc.
   All Rights Reserved.
*/

#ifndef __CELL_HEAD_TRACKER_H__
#define __CELL_HEAD_TRACKER_H__

#include <cell/head_tracker/head_tracker_error.h>

#define CELL_HEAD_TRACKER_INPUT_IMAGE_WIDTH  320
#define CELL_HEAD_TRACKER_INPUT_IMAGE_HEIGHT 240

#define CELL_HEAD_TRACKER_PPU_TO_SPU_DATA_SIZE 512

#define CELL_HEAD_TRACKER_RESULT_NUM_MAX 8
#define CELL_HEAD_TRACKER_RESULT_POINT_OF_CURVE_NUM_MAX 64

#define CELL_HEAD_TRACKER_VGA_IMAGE_WIDTH  640
#define CELL_HEAD_TRACKER_VGA_IMAGE_HEIGHT 480


#define CELL_HEAD_TRACKER_MASK_OFF                           0
#define CELL_HEAD_TRACKER_MASK_BY_BACKGROUND_IMAGE           1
#define CELL_HEAD_TRACKER_MASK_BY_DYNAMIC_BACKGROUND_IMAGE   2




#if defined(__PPU__)
typedef struct CellHeadTrackerResult{

    float posX;
    float posY;
    float roll;
    float padding00[1];

    float headWidth;
    float headHeight;
    float padding01[2];

    uint32_t instanceID;
    int32_t  nFace;
    uint32_t padding02[2];

    uint32_t numPointOfCurve;
    uint32_t padding04[3];

    float pointOfCurveX[CELL_HEAD_TRACKER_RESULT_POINT_OF_CURVE_NUM_MAX];
    float pointOfCurveY[CELL_HEAD_TRACKER_RESULT_POINT_OF_CURVE_NUM_MAX];

} __attribute__((aligned(16))) CellHeadTrackerResult;


typedef void* (*CellHeadTrackerMemalignCallback)( size_t boundary, size_t size );
typedef void  (*CellHeadTrackerFreeCallback)( void* ptr );

typedef struct CellHeadTrackerAttribute{

    uint32_t maxOfTrackingInstance;
    uint32_t imageWidth;
    uint32_t imageHeight;
    uint32_t padding00;

    uint32_t setThreshOfHeadRoll;
    float    maxOfHeadRoll;
    float    minOfHeadRoll;
    uint32_t padding01[1];

    CellHeadTrackerMemalignCallback memalignCallback;
    CellHeadTrackerFreeCallback     freeCallback;

    CellHeadTrackerAttribute():
        maxOfTrackingInstance(0),
        imageWidth(0),
        imageHeight(0),
        setThreshOfHeadRoll(0),
        maxOfHeadRoll(0.0f),
        minOfHeadRoll(0.0f){
    }

} __attribute__((aligned(16))) CellHeadTrackerAttribute;


int32_t cellHeadTrackerInit( const uint32_t maxOfTrackingInstance,
                             const uint32_t imageWidth, 
                             const uint32_t imageHeight );
int32_t cellHeadTrackerInitWithAttribute( const CellHeadTrackerAttribute* attr );
int32_t cellHeadTrackerExit();

int32_t cellHeadTrackerSetDataOfInitConvertImage( uint8_t* data );
int32_t cellHeadTrackerSetDataOfExitConvertImage( uint8_t* data );
int32_t cellHeadTrackerSetDataOfExecConvertImageForTracking( const int32_t  nTask,
                                                             const int32_t  numOfUseTask,
                                                             const uint8_t* imageSrcYAddr,
                                                             const uint8_t* imageSrcUAddr,
                                                             const uint8_t* imageSrcVAddr,
                                                             uint8_t* data );

int32_t cellHeadTrackerSetDataOfExecConvertImageYUV422ToYUV444( const int32_t  nTask,
                                                                const int32_t  numOfUseTask,
                                                                const int32_t  imageWidth,
                                                                const int32_t  imageHeight,
                                                                const uint8_t* imageSrcYUV422Ptr,
                                                                uint8_t*       imageDstYPtr, 
                                                                uint8_t*       imageDstUPtr, 
                                                                uint8_t*       imageDstVPtr,
                                                                uint8_t*       data );

int32_t cellHeadTrackerSetDataOfExecConvertImageRAW8ToYUV444( const int32_t  nTask, 
                                                              const int32_t  numOfUseTask,
                                                              const int32_t  imageWidth,
                                                              const int32_t  imageHeight,
                                                              const uint8_t* imageSrcRAW8Ptr,
                                                              const float    rWBRatio,
                                                              const float    gWBRatio,
                                                              const float    bWBRatio,
                                                              uint8_t*       imageDstYPtr, 
                                                              uint8_t*       imageDstUPtr, 
                                                              uint8_t*       imageDstVPtr,
                                                              uint8_t*       data );

int32_t cellHeadTrackerSetDataOfExecConvertImageVGAToQVGA( const int32_t  nTask,
                                                           const int32_t  numOfUseTask,
                                                           const int32_t  imageSrcWidth,
                                                           const int32_t  imageSrcHeight,
                                                           const uint8_t* imageSrcYAddr,
                                                           const uint8_t* imageSrcUAddr,
                                                           const uint8_t* imageSrcVAddr,
                                                           const int32_t  imageDstWidth,
                                                           const int32_t  imageDstHeight,
                                                           uint8_t* imageDstYAddr,
                                                           uint8_t* imageDstUAddr,
                                                           uint8_t* imageDstVAddr,
                                                           uint8_t* data );

int32_t cellHeadTrackerSetDataOfInitTracking( uint8_t* data );
int32_t cellHeadTrackerSetDataOfExitTracking( uint8_t* data );
int32_t cellHeadTrackerSetDataOfExecTracking( const uint32_t nTask, 
                                              const uint32_t numOfUseTask,
                                              uint8_t* data );
int32_t cellHeadTrackerSetDataOfExecCalibration( const uint32_t nTask, 
                                                 const uint32_t numOfUseTask,
                                                 uint8_t* data );


int32_t cellHeadTrackerGenerateInput( const int32_t  numOfFace,
                                      const float*   faceX,
                                      const float*   faceY,
                                      const float*   faceW,
                                      const float*   faceH,
                                      const float*   faceRoll,
                                      const float*   facePitch,
                                      const float*   faceYaw,
                                      const float*   faceRotateConfidence);
int32_t cellHeadTrackerGenerateOutput( uint32_t* numInstance, 
                                       CellHeadTrackerResult* result );


int32_t cellHeadTrackerFinishTracking( const uint32_t instanceID );






int32_t cellHeadTrackerSetModeForMaskOfInputImage( const int32_t mode );

int32_t cellHeadTrackerSetBackgroundImage( const uint8_t* imageYAddr, 
                                           const uint8_t* imageUAddr, 
                                           const uint8_t* imageVAddr );
void    cellHeadTrackerResetCalibration();
int32_t cellHeadTrackerIsCompleteCalibration();
void    cellHeadTrackerGenerateCalibrationResult();

int32_t cellHeadTrackerGetCalibrationParam( int32_t* param00, int32_t* param01, int32_t* param02, 
                                            float*   param10, float*   param11, float*   param12, 
                                            int32_t* param20, int32_t* param21, int32_t* param22 );
void    cellHeadTrackerSetCalibrationParam( const int32_t param00, const int32_t param01, const int32_t param02, 
                                            const float   param10, const float   param11, const float   param12, 
                                            const int32_t param20, const int32_t param21, const int32_t param22 );

uint8_t* cellHeadTrackerGetPtrOfEdgeImage();
uint8_t* cellHeadTrackerGetPtrOfMaskImage();

#endif


#if defined(__SPU__)
int32_t cellHeadTrackerInitConvertImage( uint8_t* data );
int32_t cellHeadTrackerInitConvertImageWithSetDMATag( uint8_t* data, 
                                                      const int32_t dmaTag00, 
                                                      const int32_t dmaTag01, 
                                                      const int32_t dmaTag02,
                                                      const int32_t dmaTag03 );
int32_t cellHeadTrackerExitConvertImage( uint8_t* data );
int32_t cellHeadTrackerExecConvertImageForTracking( uint8_t* data );

int32_t cellHeadTrackerExecConvertImageYUV422ToYUV444( uint8_t* data );
int32_t cellHeadTrackerExecConvertImageRAW8ToYUV444(   uint8_t* data );
int32_t cellHeadTrackerExecConvertImageVGAToQVGA(      uint8_t* data );

int32_t cellHeadTrackerInitTracking( uint8_t* data );
int32_t cellHeadTrackerInitTrackingWithSetDMATag( uint8_t* data,
                                                  const int32_t dmaTag00, 
                                                  const int32_t dmaTag01, 
                                                  const int32_t dmaTag02,
                                                  const int32_t dmaTag03 );
int32_t cellHeadTrackerExitTracking( uint8_t* data );
int32_t cellHeadTrackerExecTracking( uint8_t* data );
int32_t cellHeadTrackerExecCalibration( uint8_t* data );
#endif


#endif /* __CELL_HEAD_TRACKER_H__ */
