/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2007 Sony Computer Entertainment Inc.
 * All Rights Reserved.
 */

#ifndef __CELL_SAIL_SOUND_ADAPTER_H__
#define __CELL_SAIL_SOUND_ADAPTER_H__

//#include <cell/audio.h>
//#include <cell/mixer.h>

#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
extern "C" {
#endif

typedef int (*CellSailSoundAdapterFuncMakeup)(void *pArg);
typedef int (*CellSailSoundAdapterFuncCleanup)(void *pArg);
typedef int (*CellSailSoundAdapterFuncFormatChanged)(
    void                *pArg,
    CellSailAudioFormat *pFormat,
    uint32_t             counter);


typedef struct CellSailSoundAdapterFuncs {
    CellSailSoundAdapterFuncMakeup        pMakeup;
    CellSailSoundAdapterFuncCleanup       pCleanup;
    CellSailSoundAdapterFuncFormatChanged pFormatChanged;
} CellSailSoundAdapterFuncs;


typedef struct CellSailSoundFrameInfo {
    float   *pBuffer;
    uint32_t sessionId;
    uint32_t tag;
    int      status;
    uint64_t pts;
} CellSailSoundFrameInfo;


typedef struct CellSailSoundAdapter {
    uint64_t internalData[32];
} CellSailSoundAdapter;


int cellSailSoundAdapterInitialize(
    CellSailSoundAdapter            *pSelf,
    const CellSailSoundAdapterFuncs *pFuncs,
    void                            *pArg);
                                   
int cellSailSoundAdapterFinalize(
    CellSailSoundAdapter *pSelf);

int cellSailSoundAdapterSetPreferredFormat(
    CellSailSoundAdapter      *pSelf,
    const CellSailAudioFormat *pFormat);

int cellSailSoundAdapterGetFrame(
    CellSailSoundAdapter   *pSelf,
    uint32_t                samples,
    CellSailSoundFrameInfo *pInfo);

int cellSailSoundAdapterGetFormat(
    CellSailSoundAdapter *pSelf,
    CellSailAudioFormat  *pFormat);

void cellSailSoundAdapterUpdateAvSync(
    CellSailSoundAdapter *pSelf,
    uint32_t              tag,
    uint64_t              timeStamp);

int cellSailSoundAdapterPtsToTimePosition(
    CellSailSoundAdapter *pSelf,
    uint64_t              pts,
    uint64_t             *pTime);

#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
}
#endif

#endif /* __CELL_SAIL_SOUND_ADAPTER_H__ */
