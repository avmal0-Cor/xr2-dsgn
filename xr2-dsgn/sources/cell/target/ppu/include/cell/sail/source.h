/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2008 Sony Computer Entertainment Inc.
 * All Rights Reserved.
 */

#ifndef __CELL_SAIL_SOURCE_H__
#define __CELL_SAIL_SOURCE_H__

#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
extern "C" {
#endif


typedef struct CellSailSourceStartCommand {
    uint64_t  startFlags;
    int64_t   startArg;
    int64_t   lengthArg;
    uint64_t  optionalArg0;
    uint64_t  optionalArg1;
} CellSailSourceStartCommand;


typedef struct CellSailSourceStreamingProfile {
    uint32_t  reserved0;
    uint32_t  numItems;
    uint32_t  maxBitrate;
    uint32_t  reserved1;
    uint64_t  duration;
    uint64_t  streamSize;
} CellSailSourceStreamingProfile;

enum {
    CELL_SAIL_SOURCE_SEEK_ABSOLUTE_BYTE_POSITION = 1<<0,
};

enum {
    CELL_SAIL_SOURCE_CAPABILITY_NONE      = 0,
    CELL_SAIL_SOURCE_CAPABILITY_SEEK_ABSOLUTE_BYTE_POSITION = 1<<0,

    CELL_SAIL_SOURCE_CAPABILITY_PAUSE     = 1<<4,
    CELL_SAIL_SOURCE_CAPABILITY_GAPLESS   = 1<<5,
    CELL_SAIL_SOURCE_CAPABILITY_EOS       = 1<<6,

    CELL_SAIL_SOURCE_CAPABILITY_SEEK_ABSOLUTE_TIME_POSITION = 1 << 7,

    // obsolete
    //CELL_SAIL_SOURCE_CAPABILITY_SEEK_MP4_SCALE_AND_TIME = 1 << 7,
    // typo
    //CELL_SAIL_SOURCE_CAPABILITY_SEEK_MP4_SCALE_ANT_TIME = 1 << 7,
};

// main
typedef int (*CellSailSourceFuncMakeup)(void       *pArg,
                                        const char *pProtocolName);
typedef int (*CellSailSourceFuncCleanup)(void *pArg);
typedef void (*CellSailSourceFuncOpen)(
    void                              *pArg,
    int                                streamType,
    void                              *pMediaInfo,
    const char                        *pUri,
    const CellSailSourceStreamingProfile *pProfile);
typedef void (*CellSailSourceFuncClose)(void *pArg);
typedef void (*CellSailSourceFuncStart)(
    void                             *pArg,
    const CellSailSourceStartCommand *pCommand,
    uint32_t                          sessionId);
typedef void (*CellSailSourceFuncStop)(void *pArg);
typedef void (*CellSailSourceFuncCancel)(void *pArg);

typedef int (*CellSailSourceFuncCheckout)(
    void                            *pArg,
    const CellSailSourceBufferItem **ppItem);
typedef int (*CellSailSourceFuncCheckin)(
    void                           *pArg,
    const CellSailSourceBufferItem *pItem);
typedef int (*CellSailSourceFuncClear)(void  *pArg);

typedef int (*CellSailSourceFuncRead)(void       *pArg,
                                      int         streamType,
                                      void       *pMediaInfo,
                                      const char *pUri,
                                      uint64_t    offset,
                                      uint8_t    *pBuf,
                                      uint32_t    size,
                                      uint64_t   *pTotalSize);
typedef int (*CellSailSourceFuncReadSync)(void       *pArg,
                                          int         streamType,
                                          void       *pMediaInfo,
                                          const char *pUri,
                                          uint64_t    offset,
                                          uint8_t    *pBuf,
                                          uint32_t    size,
                                          uint64_t   *pTotalSize);

typedef int (*CellSailSourceFuncGetCapabilities)(
    void       *pArg,
    int         streamType,
    void       *pMediaInfo,
    const char *pUri,
    uint64_t   *pCapability);

typedef int (*CellSailSourceFuncInquireCapability)(
    void       *pArg,
    int         streamType,
    void       *pMediaInfo,
    const char *pUri,
    const CellSailSourceStartCommand *pCommand);

typedef struct CellSailSourceFuncs {
    CellSailSourceFuncMakeup    pMakeup;
    CellSailSourceFuncCleanup   pCleanup;
    CellSailSourceFuncOpen      pOpen;
    CellSailSourceFuncClose     pClose;
    CellSailSourceFuncStart     pStart;
    CellSailSourceFuncStop      pStop;
    CellSailSourceFuncCancel    pCancel;
    CellSailSourceFuncCheckout  pCheckout;
    CellSailSourceFuncCheckin   pCheckin;
    CellSailSourceFuncClear     pClear;
    CellSailSourceFuncRead      pRead;
    CellSailSourceFuncReadSync  pReadSync;
    CellSailSourceFuncGetCapabilities   pGetCapabilities;
    CellSailSourceFuncInquireCapability pInquireCapability;
} CellSailSourceFuncs;

typedef struct CellSailSource {
    uint64_t internalData[20];
} CellSailSource;


int cellSailSourceInitialize(CellSailSource            *pSelf,
                             const CellSailSourceFuncs *pFuncs,
                             void                      *pArg);
int cellSailSourceFinalize(CellSailSource *pSelf);


void cellSailSourceNotifyCallCompleted(CellSailSource *pSelf, int status);

void cellSailSourceNotifyInputEos(CellSailSource *pSelf);
void cellSailSourceNotifyStreamOut(CellSailSource *pSelf, uint32_t num);

void cellSailSourceNotifySessionError(CellSailSource *pSelf,
                                      int             error,
                                      bool            isFatal);
void cellSailSourceNotifyMediaStateChanged(CellSailSource *pSelf,
                                           int             state);


enum {
    CELL_SAIL_SOURCE_DIAG_NOTIFY_CALL_COMPLETED      = 0,
    CELL_SAIL_SOURCE_DIAG_NOTIFY_INPUT_EOS           = 1,
    CELL_SAIL_SOURCE_DIAG_NOTIFY_STREAM_OUT          = 2,
    CELL_SAIL_SOURCE_DIAG_NOTIFY_SESSION_ERROR       = 3,
    CELL_SAIL_SOURCE_DIAG_NOTIFY_MEDIA_STATE_CHANGED = 4,
};

typedef void (*CellSailSourceFuncDiagNotify)(void *pHandlerArg,
                                             int   type,
                                             int   arg0,
                                             int   arg1);

CellSailSourceFuncDiagNotify
cellSailSourceSetDiagHandler(CellSailSource               *pSelf,
                             CellSailSourceFuncDiagNotify  pHandler,
                             void                         *pArg);

#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
}
#endif

#endif /*__CELL_SAIL_SOURCE_H__*/

