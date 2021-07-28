/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2008 Sony Computer Entertainment Inc.
 * All Rights Reserved.
 */

#ifndef __CELL_SAIL_RENDERER_AUDIO_H__
#define __CELL_SAIL_RENDERER_AUDIO_H__

#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
extern "C" {
#endif

typedef struct CellSailAudioFrameInfo {
    void     *pPcm;
    int       status;
    uint64_t  pts;
    uint64_t  reserved;
} CellSailAudioFrameInfo;

// main
typedef int (*CellSailRendererAudioFuncMakeup)(void *pArg);
typedef int (*CellSailRendererAudioFuncCleanup)(void *pArg);
typedef void (*CellSailRendererAudioFuncOpen)(
    void                      *pArg,
    const CellSailAudioFormat *pInfo,
    uint32_t                   frameNum);
typedef void (*CellSailRendererAudioFuncClose)(void *pArg);
typedef void (*CellSailRendererAudioFuncStart)(void *pArg, bool buffering);
typedef void (*CellSailRendererAudioFuncStop)(void *pArg, bool flush);
typedef void (*CellSailRendererAudioFuncCancel)(void *pArg);
typedef int (*CellSailRendererAudioFuncCheckout)(void                    *pArg,
                                                 CellSailAudioFrameInfo **ppInfo);
typedef int (*CellSailRendererAudioFunkCheckin)(void                   *pArg,
                                                CellSailAudioFrameInfo *pInfo);

typedef struct CellSailRendererAudioFuncs {
    CellSailRendererAudioFuncMakeup   pMakeup;
    CellSailRendererAudioFuncCleanup  pCleanup;
    CellSailRendererAudioFuncOpen     pOpen;
    CellSailRendererAudioFuncClose    pClose;
    CellSailRendererAudioFuncStart    pStart;
    CellSailRendererAudioFuncStop     pStop;
    CellSailRendererAudioFuncCancel   pCancel;
    CellSailRendererAudioFuncCheckout pCheckout;
    CellSailRendererAudioFunkCheckin  pCheckin;
} CellSailRendererAudioFuncs;

typedef struct CellSailRendererAudio {
    uint64_t internalData[32];
} CellSailRendererAudio;

typedef struct CellSailRendererAudioAttribute {
    size_t               thisSize;
    CellSailAudioFormat *pPreferredFormat;
} CellSailRendererAudioAttribute;

// public 
int cellSailRendererAudioInitialize(CellSailRendererAudio                *pSelf,
                                    const CellSailRendererAudioFuncs     *pFuncs,
                                    void                                 *pArg,
                                    const CellSailRendererAudioAttribute *pAttr);
int cellSailRendererAudioFinalize(CellSailRendererAudio* pSelf);

void cellSailRendererAudioNotifyCallCompleted(CellSailRendererAudio *pSelf,
                                              int                    status);

void cellSailRendererAudioNotifyFrameDone(CellSailRendererAudio *pSelf);

void cellSailRendererAudioNotifyOutputEos(CellSailRendererAudio *pSelf);


#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
}
#endif

#endif /*__CELL_SAIL_RENDERER_AUDIO_H__*/
/*[eof]*/
