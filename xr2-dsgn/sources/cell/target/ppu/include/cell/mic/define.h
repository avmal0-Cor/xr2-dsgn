/*****************************************************************
 * SCE CONFIDENTIAL
 PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (c) 2009 Sony Computer Entertainment Inc.
 * All Rights Reserved.
 *
 * R & D Division, Sony Computer Entertainment Inc.
 *****************************************************************/

#ifndef _MIOS_MIC_INPUT_H
#define _MIOS_MIC_INPUT_H

#include <sys/types.h>

#if defined(__LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
extern "C" {
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////
/*      Claims of "Mic In" Interface (including MIOS and libmic)
 * -----------------------------------------------------------------------------------------------------
 *     Claims of "Mic In" Interface (including MIOS and libmic)
 * A) support multiple userland <libmic> client, a client may or may not own device
 * B) a client may own one or more device(s) in exclusive fasion ("One-on-One")
 * C) devices availability is managed by XMB "Audio Device Settings"
 *
 * Example (1): One headset and one eyetoy are plugged in system
 *     The VSH AVChat client exclusively uses EyetoyMic for voice chat
 *     The game client can not use EyetoyMic since it is already owned by somebody else
 *     The game client can use headset for speech recognition for example
 *
 * Comment: you may use EyetoyMic for family chat in home in example(1)
 *          in the same time, you prefer use headset for speech command&control
 *          thus, the noisy family chat does not interfer speech recognizer in game
 *
 * Example (2):  One microphone device
 *      based on "first-come-first-own", if the mic is owned by previous VSH client, 
 *      then current game client "Open" request will not be served
 *
 * Example (3): 3 persons play online socom game on one PS3, one headset for each person for purpose  
 *      of private communication, another EyetoyMic is used as group microphone sits in front of them 
 *      to capture all 3 persons's voices. 
 *
 * Comment
 *      The local game client can attach up to 8 microphones, headset delivers chat in private channel,
 *      while FarField EyetoyMic provides group chat
 *      
 * Device scheduling policy: "First-Come-First-Own" 
 */

#define  MaxNumMicInputs        (8)     /* maximumly microphones/clients at a time */
#define  MAX_MICS_PERMISSABLE   (4)     /* Maximum number of mics we have space for */

#define  NullDeviceID           (-1)    /* null device id, stands for "No Such Device) */

#define  SYSMICIN_KEYBASE               (0x8000CA7211071000ULL)     /* key space resevered for "MicIn" server/client */
#define  EQUEUE_KEY_MICIN_ACCESSPOINT   (0x8000CA7211072abcULL)     /* the "MicIn" service connection port  */
#define  LIBMIC_KEYBASE                 (0x8000000000000100ULL)     /* key subspace reserved for <Libmic> client    */

#define  CELL_MIC_STARTFLAG_LATENCY_4   0x00000001                  /* Request 4 ms RAW latency */
#define  CELL_MIC_STARTFLAG_LATENCY_2   0x00000002                  /* Request 2 ms RAW latency */
#define  CELL_MIC_STARTFLAG_LATENCY_1   0x00000003                  /* Request 1 ms RAW latency */

/*
 * Format type I, does not include /freqtable/gain/volume/agc/mute, they should be treated as config
 */
typedef struct CellMicInputFormatI 
{
    uint8_t    bNrChannels;     /* number of channels > 0, */
    uint8_t    bSubframeSize;   /* number of bytes per sample slot > 0, */
    uint8_t    bBitResolution;  /* sample resolution bits > 0, */
    uint8_t    bDataType;       /* [0, 1] --> little/big endian, flexible to extend to future special data type, */
    uint32_t   uiSampRate;      /* audio sample rate, */
} CellMicInputFormatI;

typedef struct CellMicInputStream
{
    uint32_t   uiBufferBottom;  /* the ringbuffer bottom position,  */
    uint32_t   uiBufferSize;    /* the ringbuffer size in bytes     */
    uint32_t   uiBuffer;        /* the base offset of ringbuffer    */
} CellMicInputStream;

typedef struct CellMicInputDefinition
{
    volatile uint32_t   uiDevId;
    CellMicInputStream  data;
    CellMicInputFormatI aux_format; /* the auxilary data format     */
    CellMicInputFormatI raw_format; /* the device raw data format   */
    CellMicInputFormatI sig_format; /* the processed signal format  */
} CellMicInputDefinition;

/* 
 * command type for server/client communication
 */
typedef enum CellMicCommand
{ 
    CELLMIC_INIT,       /* command to init server/client,   */
    CELLMIC_END,        /* command to destory server/client, */
    CELLMIC_ATTACH,     /* callback by "device manager" to server/client if device is attached, */
    CELLMIC_DETACH,     /* callback by "device manager" to server/client if device is detached, */
    CELLMIC_SWITCH,     /* callback by "device manager" to server/client if device preference list is changed */
    CELLMIC_DATA,       /* callback by server to client, when audio data is available, */
    CELLMIC_OPEN,       /* command to open device, issued by client,   */
    CELLMIC_CLOSE,      /* command to close device, issued by client,  */
    CELLMIC_START,      /* command to start device, issued by client,   */
    CELLMIC_STOP,       /* command to stop device, issued by client,  */
    CELLMIC_QUERY,      /* command to query service, */
    CELLMIC_CONFIG,     /* command to config device, issued by client, */
    CELLMIC_CALLBACK,   /* command to config callback service, issued by client, */
    CELLMIC_RESET,      /* command to reset input stream, issued by client,  */
    CELLMIC_STATUS,     /* command to get device status without open, issued by client */
    CELLMIC_IPC,        /* command to setup IPC */
    CELLMIC_CALLBACK2,  /* command to setup extended callback service, issued by client */
    CELLMIC_WEAK,       /* SYSTEM RESERVED */
    CELLMIC_INIT2,      /* command to init server/client,   */
} CellMicCommand;

////////////////////////////////////////////////////////////////////////////////////////////////////////
//used by/for <cellMicCommand> to encode/decode data transfer of <cellMicCommand>
#define MAKE_UINT32(whi, wlo)  ((uint32_t)(((uint16_t)(wlo)) | (((uint32_t)((uint16_t)(whi))) << 16)))
#define TAKE_LOUINT16(a)       ((uint16_t)(((uint32_t)(a)) & 0xffff))
#define TAKE_HIUINT16(a)       ((uint16_t)((((uint32_t)(a)) >> 16) & 0xffff))

#define MAKE_UINT64(whi, wlo)  ((uint64_t)(((uint32_t)(wlo)) | ((uint64_t)((uint32_t)(whi))) << 32))
#define TAKE_LOUINT32(a)       ((uint32_t)(((uint64_t)(a)) & 0xffffffffULL))
#define TAKE_HIUINT32(a)       ((uint32_t)((((uint64_t)(a)) >> 32) & 0xffffffffULL))

#define MAKE_UINT64_321616(a, b, c)   MAKE_UINT64(a, MAKE_UINT32(b, c))
#define TAKE_UINT64_321616(u64, a, b, c)  \
{   \
    (a)=TAKE_HIUINT32(u64); \
    uint32_t tmp = TAKE_LOUINT32(u64); \
    (b) = TAKE_HIUINT16(tmp); \
    (c) = TAKE_LOUINT16(tmp); \
}
#define MAKE_UINT64_16161616(a,b,c,d) MAKE_UINT64(MAKE_UINT32(a,b),MAKE_UINT32(c,d))
#define TAKE_UINT64_16161616(u64,a,b,c,d) \
{   \
    uint32_t hi32 = TAKE_HIUINT32(u64); \
    uint32_t lo32 = TAKE_LOUINT32(u64); \
    (a) = TAKE_HIUINT16(hi32); (b) = TAKE_LOUINT16(hi32);   \
    (c) = TAKE_HIUINT16(lo32); (d) = TAKE_LOUINT16(lo32);   \
}
//////////////////////////////////////////////////////////////////////////////////////////////
#if defined(__LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
}
#endif
/* end of microphone Input Interface */
#endif // _MIOS_MIC_INPUT_H


