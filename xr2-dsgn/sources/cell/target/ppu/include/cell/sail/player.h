/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2009 Sony Computer Entertainment Inc.
 * All Rights Reserved.
 */

#ifndef __CELL_SAIL_PLAYER_H__
#define __CELL_SAIL_PLAYER_H__

#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
extern "C" {
#endif

#define CELL_SAIL_MAX_LENGTH_PROTOCOL_NAME_WITH_NULL_TERMINATE (32)
#define CELL_SAIL_MAX_NUM_DESCRIPTORS                           (2)
#define CELL_SAIL_PROTOCOL_CELL_FS  "x-cell-fs"

typedef void (*CellSailPlayerFuncNotified)(void          *pCbArg,
                                           CellSailEvent  event,
                                           uint64_t       arg0,
                                           uint64_t       arg1);

typedef enum {
    CELL_SAIL_PLAYER_STATE_INITIALIZED      = 0,
    CELL_SAIL_PLAYER_STATE_BOOT_TRANSITION  = 1,
    CELL_SAIL_PLAYER_STATE_CLOSED           = 2,
    CELL_SAIL_PLAYER_STATE_OPEN_TRANSITION  = 3,
    CELL_SAIL_PLAYER_STATE_OPENED           = 4,
    CELL_SAIL_PLAYER_STATE_START_TRANSITION = 5,
    CELL_SAIL_PLAYER_STATE_RUNNING          = 6,
    CELL_SAIL_PLAYER_STATE_STOP_TRANSITION  = 7,
    CELL_SAIL_PLAYER_STATE_CLOSE_TRANSITION = 8,
    CELL_SAIL_PLAYER_STATE_LOST             = 9,

    _CELL_SAIL_PLAYER_STATE_TYPE_NUM_OF_ELEMENTS = 10, /*DO NOT USE*/
} CellSailPlayerStateType;


typedef enum {
    CELL_SAIL_PLAYER_CALL_NONE           = 0,

    CELL_SAIL_PLAYER_CALL_BOOT           = 1,

    CELL_SAIL_PLAYER_CALL_OPEN_STREAM    = 2,
    CELL_SAIL_PLAYER_CALL_CLOSE_STREAM   = 3,

    CELL_SAIL_PLAYER_CALL_OPEN_ES_AUDIO  = 4,
    CELL_SAIL_PLAYER_CALL_OPEN_ES_VIDEO  = 5,
    CELL_SAIL_PLAYER_CALL_OPEN_ES_USER   = 6,
    CELL_SAIL_PLAYER_CALL_CLOSE_ES_AUDIO = 7,
    CELL_SAIL_PLAYER_CALL_CLOSE_ES_VIDEO = 8,
    CELL_SAIL_PLAYER_CALL_CLOSE_ES_USER  = 9,

    CELL_SAIL_PLAYER_CALL_START          = 10,
    CELL_SAIL_PLAYER_CALL_STOP           = 11,
    CELL_SAIL_PLAYER_CALL_NEXT           = 12,

    CELL_SAIL_PLAYER_CALL_REOPEN_ES_AUDIO  = 13,
    CELL_SAIL_PLAYER_CALL_REOPEN_ES_VIDEO  = 14,
    CELL_SAIL_PLAYER_CALL_REOPEN_ES_USER   = 15,

    _CELL_SAIL_PLAYER_CALL_TYPE_NUM_OF_ELEMENTS = 16, /*DO NOT USE*/
} CellSailPlayerCallType;


typedef enum {
    CELL_SAIL_PLAYER_PRESET_AV_SYNC = 0, /* deprecated. same as 59_94HZ */
    CELL_SAIL_PLAYER_PRESET_AS_IS   = 1,
    CELL_SAIL_PLAYER_PRESET_AV_SYNC_59_94HZ     = 2,
    CELL_SAIL_PLAYER_PRESET_AV_SYNC_29_97HZ     = 3,
    CELL_SAIL_PLAYER_PRESET_AV_SYNC_50HZ        = 4,
    CELL_SAIL_PLAYER_PRESET_AV_SYNC_25HZ        = 5,
    CELL_SAIL_PLAYER_PRESET_AV_SYNC_AUTO_DETECT = 6,
} CellSailPlayerPresetType;


typedef enum {
    CELL_SAIL_PARAMETER_ENABLE_VPOST = 0,

    /* player */
    CELL_SAIL_PARAMETER_CONTROL_QUEUE_DEPTH         = 1,
    CELL_SAIL_PARAMETER_CONTROL_PPU_THREAD_PRIORITY = 2,

    /* spurs */
    CELL_SAIL_PARAMETER_SPURS_NUM_OF_SPUS         = 3,
    CELL_SAIL_PARAMETER_SPURS_SPU_THREAD_PRIORITY = 4,
    CELL_SAIL_PARAMETER_SPURS_PPU_THREAD_PRIORITY = 5,
    CELL_SAIL_PARAMETER_SPURS_EXIT_IF_NO_WORK     = 6,

    /* source */
    CELL_SAIL_PARAMETER_IO_PPU_THREAD_PRIORITY = 7,

    /* demux */
    CELL_SAIL_PARAMETER_DMUX_PPU_THREAD_PRIORITY   = 8,
    CELL_SAIL_PARAMETER_DMUX_SPU_THREAD_PRIORITY   = 9, /* deprecated */
    CELL_SAIL_PARAMETER_DMUX_NUM_OF_SPUS           = 10,
    CELL_SAIL_PARAMETER_DMUX_SPURS_TASK_PRIORITIES = 11,

    /* adec */
    CELL_SAIL_PARAMETER_ADEC_PPU_THREAD_PRIORITY   = 12,
    CELL_SAIL_PARAMETER_ADEC_SPU_THREAD_PRIORITY   = 13, /* deprecated */
    CELL_SAIL_PARAMETER_ADEC_NUM_OF_SPUS           = 14,
    CELL_SAIL_PARAMETER_ADEC_SPURS_TASK_PRIORITIES = 15,

    /* vdec */
    CELL_SAIL_PARAMETER_VDEC_PPU_THREAD_PRIORITY   = 16,
    CELL_SAIL_PARAMETER_VDEC_SPU_THREAD_PRIORITY   = 17, /* deprecated */
    CELL_SAIL_PARAMETER_VDEC_M2V_NUM_OF_SPUS       = 18,
    CELL_SAIL_PARAMETER_VDEC_AVC_NUM_OF_SPUS       = 19,
    CELL_SAIL_PARAMETER_VDEC_SPURS_TASK_PRIORITIES = 20,

    /* vpost */
    CELL_SAIL_PARAMETER_VPOST_PPU_THREAD_PRIORITY   = 21, /* deprecated */
    CELL_SAIL_PARAMETER_VPOST_SPU_THREAD_PRIORITY   = 22, /* deprecated */
    CELL_SAIL_PARAMETER_VPOST_NUM_OF_SPUS           = 23,
    CELL_SAIL_PARAMETER_VPOST_SPURS_TASK_PRIORITIES = 24,

    /* graphics adapter */
    CELL_SAIL_PARAMETER_GRAPHICS_ADAPTER_BUFFER_RELEASE_DELAY = 25,

    /* av sync */
    CELL_SAIL_PARAMETER_AV_SYNC_ES_AUDIO = 26,
    CELL_SAIL_PARAMETER_AV_SYNC_ES_VIDEO = 27,
    CELL_SAIL_PARAMETER_AV_SYNC_ES_USER  = 28, /* not available */

    /* control */
    CELL_SAIL_PARAMETER_CONTROL_PPU_THREAD_STACK_SIZE  = 29,

    CELL_SAIL_PARAMETER_RESERVED0_= 30, /* DO NOT USE */
    CELL_SAIL_PARAMETER_RESERVED1 = 31, /* DO NOT USE */

    /* apost */
    CELL_SAIL_PARAMETER_ENABLE_APOST_SRC = 32,

    /* file i/o interface */
    CELL_SAIL_PARAMETER_FS = 33,

    CELL_SAIL_PARAMETER_IO_PPU_THREAD_STACK_SIZE = 34,
    CELL_SAIL_PARAMETER_VIDEO_PERFORMANCE_POLICY = 35,

    _CELL_SAIL_PARAMETER_TYPE_NUM_OF_ELEMENTS = 36, /* DO NOT USE */


    CELL_SAIL_PARAMETER_SOURCE_PPU_THREAD_PRIORITY =
    CELL_SAIL_PARAMETER_IO_PPU_THREAD_PRIORITY,

    CELL_SAIL_PARAMETER_DMUX_SPURS_TASK_PRIORITY =
    CELL_SAIL_PARAMETER_DMUX_SPURS_TASK_PRIORITIES, /* deprecated */
    CELL_SAIL_PARAMETER_VDEC_SPURS_TASK_PRIORITY =
    CELL_SAIL_PARAMETER_VDEC_SPURS_TASK_PRIORITIES, /* deprecated */
    CELL_SAIL_PARAMETER_ADEC_SPURS_TASK_PRIORITY =
    CELL_SAIL_PARAMETER_ADEC_SPURS_TASK_PRIORITIES, /* deprecated */
    CELL_SAIL_PARAMETER_VPOST_SPURS_TASK_PRIORITY =
    CELL_SAIL_PARAMETER_VPOST_SPURS_TASK_PRIORITIES, /* deprecated */

} CellSailParameterType;


typedef enum {
    CELL_SAIL_MEDIA_STATE_FINE = 0,
    CELL_SAIL_MEDIA_STATE_BAD  = 1,
    CELL_SAIL_MEDIA_STATE_LOST = 2,
} CellSailMediaStateType;

typedef struct CellSailPlayer {
    uint64_t internalData[128];
} CellSailPlayer;



// init/fin
int cellSailPlayerInitialize(CellSailPlayer             *pSelf,
                             int                         playerPreset,
                             CellSailMemAllocator       *pAllocator,
                             CellSailPlayerFuncNotified  pHandler,
                             void                       *pArg,
                             unsigned                    maxAudioStreamNum,
                             unsigned                    maxVideoStreamNum,
                             unsigned                    maxUserStreamNum,
                             unsigned                    queueDepth);

typedef struct CellSailPlayerResource {
	CellSpurs *pSpurs;
    uint32_t   reserved0;
    uint32_t   reserved1;
    uint32_t   reserved2;
} CellSailPlayerResource;

typedef struct CellSailPlayerAttribute {
    int      playerPreset;
    unsigned maxAudioStreamNum;
    unsigned maxVideoStreamNum;
    unsigned maxUserStreamNum;
    unsigned queueDepth;
    uint32_t reserved0;
    uint32_t reserved1;
    uint32_t reserved2;
} CellSailPlayerAttribute;

int cellSailPlayerInitialize2(CellSailPlayer             *pSelf,
                              CellSailMemAllocator       *pAllocator,
                              CellSailPlayerFuncNotified  pHandler,
                              void                       *pArg,
                              CellSailPlayerAttribute    *pAttribute,
                              CellSailPlayerResource     *pResource);

int cellSailPlayerFinalize(CellSailPlayer* pSelf);


/* config */
int cellSailPlayerRegisterSource(CellSailPlayer *pSelf,
                                 const char     *pProtocolName,
                                 CellSailSource *pSource);

int cellSailPlayerGetRegisteredProtocols(CellSailPlayer  *pSelf,
                                         char           **apProtocolNames,
                                         unsigned        *pNum);


int cellSailPlayerSetRendererAudio(CellSailPlayer        *pSelf,
                                   int                    index,
                                   CellSailRendererAudio *pRenderer);

int cellSailPlayerSetRendererVideo(CellSailPlayer        *pSelf,
                                   int                    index,
                                   CellSailRendererVideo *pRenderer);

int cellSailPlayerSetSoundAdapter(CellSailPlayer       *pSelf,
                                  int                   index,
                                  CellSailSoundAdapter *pAdapter);

int cellSailPlayerSetGraphicsAdapter(CellSailPlayer          *pSelf,
                                     int                      index,
                                     CellSailGraphicsAdapter *pAdapter);

int cellSailPlayerSetAuReceiver(CellSailPlayer     *pSelf,
                                int                 index,
                                CellSailAuReceiver *pReceiver);

int cellSailPlayerSetParameter(CellSailPlayer *pSelf,
                               int             parameterType,
                               uint64_t        param0,
                               uint64_t        param1);
int cellSailPlayerGetParameter(CellSailPlayer *pSelf,
                               int             parameterType,
                               uint64_t       *pParam0,
                               uint64_t       *pParam1);

int cellSailPlayerSubscribeEvent(CellSailPlayer *pSelf,
                                 int             eventTypes,
                                 uint64_t        arg);
int cellSailPlayerUnsubscribeEvent(CellSailPlayer *pSelf,
                                   int             eventTypes);


int cellSailPlayerReplaceEventHandler(
    CellSailPlayer              *pSelf,
    CellSailPlayerFuncNotified   pNewHandler,
    void                        *pNewArg,
    CellSailPlayerFuncNotified  *ppOldHandler,
    void                       **ppOldArg);

/* bootup */
int cellSailPlayerBoot(CellSailPlayer *pSelf, uint64_t userParam);

/* descriptor */
int cellSailPlayerCreateDescriptor(CellSailPlayer      *pSelf,
                                   int                  streamType,
                                   void                *pMediaInfo,
                                   const char          *pUri,
                                   CellSailDescriptor **ppDesc);
int cellSailPlayerDestroyDescriptor(CellSailPlayer     *pSelf,
                                    CellSailDescriptor *pDesc);


/* connection */
int cellSailPlayerAddDescriptor(CellSailPlayer     *pSelf,
                                CellSailDescriptor *pDesc);
int cellSailPlayerRemoveDescriptor(CellSailPlayer      *pSelf,
                                   CellSailDescriptor **ppDesc);
int cellSailPlayerGetDescriptorCount(CellSailPlayer *pSelf);

CellSailDescriptor* cellSailPlayerGetCurrentDescriptor(CellSailPlayer *pSelf);



int cellSailPlayerOpenStream(CellSailPlayer *pSelf, uint64_t userParam);
int cellSailPlayerCloseStream(CellSailPlayer *pSelf, uint64_t userParam);


int cellSailPlayerOpenEsAudio(CellSailPlayer *pSelf,
                              int             index,
                              void           *pSpecifier,
                              bool            enable,
                              uint64_t        userParam);
int cellSailPlayerOpenEsVideo(CellSailPlayer *pSelf,
                              int             index,
                              void           *pSpecifier,
                              bool            enable,
                              uint64_t        userParam);
int cellSailPlayerOpenEsUser(CellSailPlayer *pSelf,
                             int             index,
                             void           *pSpecifier,
                             bool            enable,
                             uint64_t        userParam);

int cellSailPlayerCloseEsAudio(CellSailPlayer *pSelf,
                               int             index,
                               uint64_t        userParam);
int cellSailPlayerCloseEsVideo(CellSailPlayer *pSelf,
                               int             index,
                               uint64_t        userParam);
int cellSailPlayerCloseEsUser(CellSailPlayer *pSelf,
                              int             index,
                              uint64_t        userParam);

int cellSailPlayerReopenEsAudio(CellSailPlayer *pSelf,
                                int             index,
                                void           *pSpecifier,
                                bool            enable,
                                uint64_t        userParam);
int cellSailPlayerReopenEsVideo(CellSailPlayer *pSelf,
                                int             index,
                                void           *pSpecifier,
                                bool            enable,
                                uint64_t        userParam);
int cellSailPlayerReopenEsUser(CellSailPlayer *pSelf,
                               int             index,
                               void           *pSpecifier,
                               bool            enable,
                               uint64_t        userParam);


/* session */
int cellSailPlayerStart(CellSailPlayer       *pSelf,
                        CellSailStartCommand *pCommand,
                        uint64_t              userParam);

int cellSailPlayerStop(CellSailPlayer *pSelf,
                       uint64_t        userParam);

int cellSailPlayerNext(CellSailPlayer       *pSelf,
                       CellSailStartCommand *pCommand,
                       uint64_t              userParam);

int cellSailPlayerSetPaused(CellSailPlayer *pSelf, bool paused);
int cellSailPlayerIsPaused(CellSailPlayer *pSelf);


/* mode */
int cellSailPlayerSetRepeatMode(CellSailPlayer       *pSelf,
                                int                   repeatMode,
                                CellSailStartCommand *pStartCommand);
int cellSailPlayerGetRepeatMode(CellSailPlayer       *pSelf,
                                CellSailStartCommand *pStartCommand);

int cellSailPlayerSetEsAudioMuted(CellSailPlayer *pSelf,
                                  int             index,
                                  bool            muted);
int cellSailPlayerSetEsVideoMuted(CellSailPlayer *pSelf,
                                  int             index,
                                  bool            muted);
int cellSailPlayerIsEsAudioMuted(CellSailPlayer *pSelf,
                                 int             index);
int cellSailPlayerIsEsVideoMuted(CellSailPlayer *pSelf,
                                 int             index);

int cellSailPlayerCancel(CellSailPlayer *pSelf);


#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
}
#endif

#endif /* __CELL_SAIL_PLAYER_H__ */

