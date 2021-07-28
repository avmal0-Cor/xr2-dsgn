/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2008 Sony Computer Entertainment Inc.
 * All Rights Reserved.
 */

#ifndef __CELL_SAIL_FEEDER_AUDIO_H__
#define __CELL_SAIL_FEEDER_AUDIO_H__

#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
extern "C" {
#endif

typedef int (*CellSailFeederAudioFuncMakeup)(void *pArg);
typedef int (*CellSailFeederAudioFuncCleanup)(void *pArg);

typedef void (*CellSailFeederAudioFuncOpen)(
    void                      *pArg,
    const CellSailAudioFormat *pSuggestedFormat,
    uint64_t                   reserved);
typedef void (*CellSailFeederAudioFuncClose)(void *pArg);
typedef void (*CellSailFeederAudioFuncStart)(void    *pArg,
                                             uint64_t reserved);
typedef void (*CellSailFeederAudioFuncStop)(void    *pArg,
                                            uint64_t reserved);

typedef void (*CellSailFeederAudioFuncCancel)(void *pArg);
typedef int (*CellSailFeederAudioFuncGetFormat)(
    void                *pArg,
    uint64_t             reserved,
    CellSailAudioFormat *pFormat);

typedef int (*CellSailFeederAudioFuncCheckout)(
    void                      *pArg,
    CellSailFeederFrameInfo **pInfo);
typedef int (*CellSailFeederAudioFuncCheckin)(
    void                     *pArg,
    CellSailFeederFrameInfo *pInfo);
typedef int (*CellSailFeederAudioFuncClear)(void  *pArg);


typedef struct CellSailFeederAudioFuncs {
    CellSailFeederAudioFuncMakeup   pMakeup;
    CellSailFeederAudioFuncCleanup  pCleanup;

    CellSailFeederAudioFuncOpen      pOpen;
    CellSailFeederAudioFuncClose     pClose;
    CellSailFeederAudioFuncStart     pStart;
    CellSailFeederAudioFuncStop      pStop;
    CellSailFeederAudioFuncCancel    pCancel;
    CellSailFeederAudioFuncGetFormat pGetFormat;

    CellSailFeederAudioFuncCheckout pCheckout;
    CellSailFeederAudioFuncCheckin  pCheckin;
    CellSailFeederAudioFuncClear    pClear;
} CellSailFeederAudioFuncs;


typedef struct CellSailFeederAudioAttribute {
    size_t   thisSize;
    uint32_t reserved;
} CellSailFeederAudioAttribute;


typedef struct CellSailFeederAudio {
    uint64_t internalData[64];
} CellSailFeederAudio;


int cellSailFeederAudioInitialize(
    CellSailFeederAudio            *pSelf,
    const CellSailFeederAudioFuncs *pFuncs,
    void                           *pArg,
    CellSailFeederAudioAttribute   *pAttr); // must be NULL

int cellSailFeederAudioFinalize(CellSailFeederAudio* pSelf);

void cellSailFeederAudioNotifyCallCompleted(CellSailFeederAudio *pSelf,
                                            int                  status);
void cellSailFeederAudioNotifyFrameOut(CellSailFeederAudio *pSelf,
                                       uint32_t             num);
void cellSailFeederAudioNotifySessionEnd(CellSailFeederAudio *pSelf);

void cellSailFeederAudioNotifySessionError(CellSailFeederAudio *pSelf,
                                           int                  error,
                                           bool                 isFatal);

#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
}
#endif

#endif /*__CELL_SAIL_FEEDER_AUDIO_H__*/
/*[eof]*/
