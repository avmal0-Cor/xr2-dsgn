/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2008 Sony Computer Entertainment Inc.
 * All Rights Reserved.
 */

#ifndef __CELL_SAIL_RECORDER_H__
#define __CELL_SAIL_RECORDER_H__

#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
extern "C" {
#endif

typedef enum {
    CELL_SAIL_RECORDER_STATE_INITIALIZED      = 0,
    CELL_SAIL_RECORDER_STATE_BOOT_TRANSITION  = 1,
    CELL_SAIL_RECORDER_STATE_CLOSED           = 2,
    CELL_SAIL_RECORDER_STATE_OPEN_TRANSITION  = 3,
    CELL_SAIL_RECORDER_STATE_OPENED           = 4,
    CELL_SAIL_RECORDER_STATE_START_TRANSITION = 5,
    CELL_SAIL_RECORDER_STATE_RUNNING          = 6,
    CELL_SAIL_RECORDER_STATE_STOP_TRANSITION  = 7,
    CELL_SAIL_RECORDER_STATE_CLOSE_TRANSITION = 8,
    CELL_SAIL_RECORDER_STATE_LOST             = 9,

    _CELL_SAIL_RECORDER_STATE_TYPE_NUM_OF_ELEMENTS = 10, /*DO NOT USE*/
} CellSailRecorderStateType;


typedef enum {
    CELL_SAIL_RECORDER_CALL_NONE           = 0,

    CELL_SAIL_RECORDER_CALL_BOOT           = 32,
    CELL_SAIL_RECORDER_CALL_OPEN_STREAM    = 33,
    CELL_SAIL_RECORDER_CALL_CLOSE_STREAM   = 34,
    CELL_SAIL_RECORDER_CALL_START          = 41,
    CELL_SAIL_RECORDER_CALL_STOP           = 42,
} CellSailRecorderCallType;



typedef void (*CellSailRecorderFuncNotified)(void          *pCbArg,
                                             CellSailEvent  event,
                                             uint64_t       arg0,
                                             uint64_t       arg1);
typedef int (*CellSailRecorderFuncEnumerateFormat)(void  *pCbArg,
                                                   int    streamType,
                                                   int    streamIndex,
                                                   int    formatType,
                                                   void  *pFormat,
                                                   int    remaining);

typedef struct CellSailRecorderResource {
    size_t     thisSize;
	CellSpurs *pSpurs;
    void      *pReserved;
} CellSailRecorderResource;


enum {
    CELL_SAIL_RECORDER_PRESET_AV_SYNC = 0,
    CELL_SAIL_RECORDER_PRESET_AS_IS   = 1,
};

typedef struct CellSailRecorderAttribute {
    size_t   thisSize;
    uint32_t preset;
    uint32_t maxAudioStreamNum;
    uint32_t maxVideoStreamNum;
    uint32_t maxUserStreamNum;
    uint32_t queueDepth;
    uint32_t timeScale;
} CellSailRecorderAttribute;



enum {
    /* recorder */
    //CELL_SAIL_PARAMETER_CONTROL_PPU_THREAD_PRIORITY   = 2,
    //CELL_SAIL_PARAMETER_CONTROL_PPU_THREAD_STACK_SIZE = 29,
    CELL_SAIL_PARAMETER_TASK_HANDLER_PPU_THREAD_PRIORITY   = 64,
    CELL_SAIL_PARAMETER_TASK_HANDLER_PPU_THREAD_STACK_SIZE = 65,
    CELL_SAIL_PARAMETER_COMMON_SPURS_TASKSET_MAX_CONTENTION = 66,
    CELL_SAIL_PARAMETER_COMMON_SPURS_TASKSET_PRIORITIES     = 67,

    /* audio pre process */
    CELL_SAIL_PARAMETER_ENABLE_APRE_SRC                = 68,
    CELL_SAIL_PARAMETER_APRE_SRC_NUM_OF_SPUS           = 69,
    CELL_SAIL_PARAMETER_APRE_SRC_SPURS_TASK_PRIORITIES = 70,

    /* audio encoder */
    CELL_SAIL_PARAMETER_AENC_AAC_NUM_OF_SPUS           = 71,
    CELL_SAIL_PARAMETER_AENC_SPURS_TASK_PRIORITIES     = 72,

    /* video pre process */
    CELL_SAIL_PARAMETER_ENABLE_VPRE                = 73,
    CELL_SAIL_PARAMETER_VPRE_NUM_OF_SPUS           = 74,
    CELL_SAIL_PARAMETER_VPRE_SPURS_TASK_PRIORITIES = 75,

    /* video encoder */
    CELL_SAIL_PARAMETER_VENC_M4V_NUM_OF_SPUS           = 76,
    CELL_SAIL_PARAMETER_VENC_AVC_NUM_OF_SPUS           = 77,
    //CELL_SAIL_PARAMETER_VENC_AVC_NUM_OF_SPUS         = 78,
    CELL_SAIL_PARAMETER_VENC_SPURS_TASK_PRIORITIES     = 79,
    CELL_SAIL_PARAMETER_VENC_PRE_NUM_OF_SPUS           = 80,
    CELL_SAIL_PARAMETER_VENC_PRE_SPURS_TASK_PRIORITIES = 81,

    /* file i/o interface */
    CELL_SAIL_PARAMETER_IO_WRITE_PPU_THREAD_PRIORITY   = 82,
    CELL_SAIL_PARAMETER_IO_WRITE_PPU_THREAD_STACK_SIZE = 83,
};

typedef struct CellSailRecorder {
    uint64_t internalData[1024];
} CellSailRecorder __attribute__((aligned(128)));


int cellSailRecorderInitialize(CellSailRecorder                   *pSelf,
                               CellSailMemAllocator               *pAllocator,
                               CellSailRecorderFuncNotified        pNotified,
                               void                               *pArg,
                               CellSailRecorderAttribute          *pAttribute,
                               CellSailRecorderResource           *pResource);

int cellSailRecorderFinalize(CellSailRecorder* pSelf);


int cellSailRecorderSetFeederAudio(CellSailRecorder    *pSelf,
                                   int                  index,
                                   CellSailFeederAudio *pFeed);

int cellSailRecorderSetFeederVideo(CellSailRecorder    *pSelf,
                                   int                  index,
                                   CellSailFeederVideo *pFeed);

int cellSailRecorderSetParameter(CellSailRecorder *pSelf,
                                 int               parameterType,
                                 uint64_t          param0,
                                 uint64_t          param1);
int cellSailRecorderGetParameter(CellSailRecorder *pSelf,
                                 int               parameterType,
                                 uint64_t         *pParam0,
                                 uint64_t         *pParam1);

int cellSailRecorderSubscribeEvent(CellSailRecorder *pRecorder,
                                   int               eventType,
                                   uint64_t          arg);

int cellSailRecorderUnsubscribeEvent(CellSailRecorder *pRecorder,
                                     int               eventType);

int cellSailRecorderReplaceEventHandler(CellSailRecorder              *pRecorder,
                                        CellSailRecorderFuncNotified   pNewHandler,
                                        void                          *pNewArg,
                                        CellSailRecorderFuncNotified  *ppOldHandler,
                                        void                         **ppOldArg);

int cellSailRecorderBoot(CellSailRecorder *pRecorder, uint64_t userParam);


/* descriptor */
int cellSailRecorderCreateProfile(CellSailRecorder *pSelf,
                                  int               streamType,
                                  CellSailProfile **ppProfile);

int cellSailRecorderDestroyProfile(CellSailRecorder *pSelf,
                                   CellSailProfile  *pProfile);

int cellSailRecorderOpenStream(CellSailRecorder *pSelf,
                               CellSailProfile  *pProfile,
                               int               sinkType,
                               const char       *pBasePath,
                               void             *pMediaInfo,
                               CellSailRecorderFuncEnumerateFormat pEnum,
                               uint64_t          userParam);
int cellSailRecorderCloseStream(CellSailRecorder *pSelf,
                                uint64_t          userParam);


/* session */

typedef struct CellSailRecorderStartCommand {
    size_t   thisSize;  // must be sizeof(CellSailRecorderStartCommand)
    int      startType; // must be 0
    uint64_t startTimeStamp;
    uint64_t duration;
} CellSailRecorderStartCommand;

int cellSailRecorderStart(CellSailRecorder             *pSelf,
                          CellSailRecorderStartCommand *pCommand,
                          const char                   *pId,
                          uint64_t                      userParam);

typedef struct CellSailRecorderStopCommand {
    size_t   thisSize; // must be sizeof(CellSailRecorderStopCommand)
    int      stopType; // must be 0
    uint64_t stopTimeStamp;
} CellSailRecorderStopCommand;

int cellSailRecorderStop(CellSailRecorder            *pSelf,
                         CellSailRecorderStopCommand *pCommand,
                         uint64_t                     userParam);

int cellSailRecorderCancel(CellSailRecorder *pSelf);

int cellSailRecorderCreateVideoConverter(CellSailRecorder        *pSelf,
                                         uint32_t                 queueDepth,
                                         CellSailVideoFormat     *pIn,
                                         CellSailVideoFormat     *pOut,
                                         void                    *pReserved,
                                         CellSailVideoConverterFuncProcessDone  pDone,
                                         void                                  *pDoneArg,
                                         CellSailVideoConverter **ppConv);
int cellSailRecorderDestroyVideoConverter(CellSailRecorder       *pSelf,
                                          CellSailVideoConverter *ppConv);


#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
}
#endif

#endif /* __CELL_SAIL_RECORDER_H__ */

