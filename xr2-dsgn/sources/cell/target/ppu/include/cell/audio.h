/* SCE CONFIDENTIAL
 PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2009 Sony Computer Entertainment Inc.
 * All Rights Reserved.
 */
#ifndef __CELL_AUDIO_H__
#define __CELL_AUDIO_H__

#include <sys/event.h>

#if defined(__LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
extern "C" {
#endif // defined(__LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)

#include <cell/error.h>

// define
#define CELL_AUDIO_MAX_PORT        4
#define CELL_AUDIO_BLOCK_SAMPLES   256

#define CELL_AUDIO_CREATEEVENTFLAG_SPU      0x00000001      // Create queue to be used on SPU.

#define CELL_AUDIO_EVENTFLAG_BEFOREMIX      0x80000000      // Send events before mixing is done.

#define CELL_AUDIO_STATUS_CLOSE   0x1010
#define CELL_AUDIO_STATUS_READY   1
#define CELL_AUDIO_STATUS_RUN     2

#define CELL_AUDIO_PORT_2CH  2
#define CELL_AUDIO_PORT_8CH  8

#define CELL_AUDIO_BLOCK_8    8
#define CELL_AUDIO_BLOCK_16   16
#define CELL_AUDIO_BLOCK_32   32

#define CELL_AUDIO_PORTATTR_BGM         (0x0000000000000010ULL)
#define CELL_AUDIO_PORTATTR_INITLEVEL   (0x0000000000001000ULL)
#define CELL_AUDIO_PORTATTR_OUT_STREAM1     (0x0000000000000001ULL)
#define CELL_AUDIO_PORTATTR_OUT_SECONDARY   (0x0000000000000001ULL)

#define CELL_AUDIO_MISC_ACCVOL_ALLDEVICE    (0x0000ffffUL)
/* error code */
/*
	CELL_ERROR_FACILITY_SOUND			0x031
	libaudio							0x8031_0701 - 0x8031_07ff
 */
#define CELL_AUDIO_MAKE_ERROR(status) CELL_ERROR_MAKE_ERROR(CELL_ERROR_FACILITY_SOUND, status)
#define CELL_AUDIO_ERROR_ALREADY_INIT       CELL_ERROR_CAST(0x80310701) // aready init
#define CELL_AUDIO_ERROR_AUDIOSYSTEM        CELL_ERROR_CAST(0x80310702) // error in AudioSystem.
#define CELL_AUDIO_ERROR_NOT_INIT           CELL_ERROR_CAST(0x80310703) // not init
#define CELL_AUDIO_ERROR_PARAM              CELL_ERROR_CAST(0x80310704) // param error
#define CELL_AUDIO_ERROR_PORT_FULL          CELL_ERROR_CAST(0x80310705) // audio port is full
#define CELL_AUDIO_ERROR_PORT_ALREADY_RUN   CELL_ERROR_CAST(0x80310706) // audio port is aready run
#define CELL_AUDIO_ERROR_PORT_NOT_OPEN      CELL_ERROR_CAST(0x80310707) // audio port is close
#define CELL_AUDIO_ERROR_PORT_NOT_RUN       CELL_ERROR_CAST(0x80310708) // audio port is not run
#define CELL_AUDIO_ERROR_TRANS_EVENT        CELL_ERROR_CAST(0x80310709) // trans event error
#define CELL_AUDIO_ERROR_PORT_OPEN          CELL_ERROR_CAST(0x8031070a) // error in port open
#define CELL_AUDIO_ERROR_SHAREDMEMORY       CELL_ERROR_CAST(0x8031070b) // error in shared memory
#define CELL_AUDIO_ERROR_MUTEX              CELL_ERROR_CAST(0x8031070c) // error in mutex
#define CELL_AUDIO_ERROR_EVENT_QUEUE        CELL_ERROR_CAST(0x8031070d) // error in event queue
#define CELL_AUDIO_ERROR_AUDIOSYSTEM_NOT_FOUND  CELL_ERROR_CAST(0x8031070e) //
#define CELL_AUDIO_ERROR_TAG_NOT_FOUND          CELL_ERROR_CAST(0x8031070f) //


// struct
typedef struct{
	uint64_t   nChannel;
	uint64_t   nBlock;
	uint64_t   attr;
	float      level;
} CellAudioPortParam;

typedef struct{
	sys_addr_t   readIndexAddr;
	uint32_t     status;
	uint64_t     nChannel;
	uint64_t     nBlock;
	uint32_t     portSize;
	sys_addr_t   portAddr;
} CellAudioPortConfig;


// function proto type
int cellAudioInit(void);
int cellAudioPortOpen(CellAudioPortParam *audioParam, uint32_t *portNum);
int cellAudioPortStart(uint32_t portNum);
int cellAudioPortStop(uint32_t portNum);
int cellAudioGetPortConfig(uint32_t portNum, CellAudioPortConfig *portConfig);
int cellAudioPortClose(uint32_t portNum);
int cellAudioQuit(void);

int cellAudioGetPortBlockTag(uint32_t portNum, uint64_t index, uint64_t *frameTag);
//                                 ----->           ---->            <------
int cellAudioGetPortTimestamp(uint32_t portNum, uint64_t frameTag, uint64_t *timeStamp);
//                                 ----->            ------->            <------

int cellAudioSetPortLevel(uint32_t portNum, float linearVol );

int cellAudioSetNotifyEventQueue(sys_ipc_key_t);
int cellAudioSetNotifyEventQueueEx(sys_ipc_key_t QueueKey, uint32_t iFlags);
int cellAudioRemoveNotifyEventQueue(sys_ipc_key_t);
int cellAudioRemoveNotifyEventQueueEx(sys_ipc_key_t QueueKey, uint32_t iFlags);

int cellAudioCreateNotifyEventQueue(sys_event_queue_t *id, sys_ipc_key_t *key);
int cellAudioCreateNotifyEventQueueEx(sys_event_queue_t *Queue, sys_ipc_key_t *QueueKey, uint32_t iFlags);

int cellAudioAddData(uint32_t portNum,float *src, unsigned int samples, float volume);
int cellAudioAdd2chData(uint32_t portNum,float *src, unsigned int samples, float volume);
int cellAudioAdd6chData(uint32_t portNum, float *src, float volume);

int cellAudioMiscSetAccessoryVolume(uint32_t , float );

int cellAudioSendAck(uint64_t data3);

#if defined(__LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
}
#endif // defined(__LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)

#endif
