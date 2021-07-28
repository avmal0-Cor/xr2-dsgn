/* SCE CONFIDENTIAL
 PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2006 Sony Computer Entertainment Inc.
 * All Rights Reserved.
 */
#ifndef _CELL_SND3_H
#define _CELL_SND3_H

#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
extern "C" {
#endif

#include <cell/error.h>
#include <cell/synth2.h>
#include <sys/types.h>

/* error macro */
/*
	CELL_ERROR_FACILITY_SOUND			 0x031
	libsnd3								 0x8031_0301 - 0x8031_03ff
 */
#define CELL_SND3_MAKE_ERROR(status) CELL_ERROR_MAKE_ERROR(CELL_ERROR_FACILITY_SOUND, status)
#define CELL_SND3_ERROR_PARAM             CELL_ERROR_CAST(0x80310301)  /* bad argument               */
#define CELL_SND3_ERROR_CREATE_MUTEX      CELL_ERROR_CAST(0x80310302)  /* can not create mutex       */
#define CELL_SND3_ERROR_SYNTH             CELL_ERROR_CAST(0x80310303)  /* error in soft synth        */
#define CELL_SND3_ERROR_ALREADY           CELL_ERROR_CAST(0x80310304)  /* already initial            */
#define CELL_SND3_ERROR_NOTINIT           CELL_ERROR_CAST(0x80310305)  /* not initial                */
#define CELL_SND3_ERROR_SMFFULL           CELL_ERROR_CAST(0x80310306)  /* sfm bind full              */
#define CELL_SND3_ERROR_HD3ID             CELL_ERROR_CAST(0x80310307)  /* bad hd3ID                  */
#define CELL_SND3_ERROR_SMF               CELL_ERROR_CAST(0x80310308)  /* bad SMF address            */
#define CELL_SND3_ERROR_SMFCTX            CELL_ERROR_CAST(0x80310309)  /* bad CellSnd3SmfCtx address */
#define CELL_SND3_ERROR_FORMAT            CELL_ERROR_CAST(0x8031030a)  /* bad SMF format             */
#define CELL_SND3_ERROR_SMFID             CELL_ERROR_CAST(0x8031030b)  /* bad smdID                  */
#define CELL_SND3_ERROR_SOUNDDATAFULL     CELL_ERROR_CAST(0x8031030c)  /* sound data bind full       */
#define CELL_SND3_ERROR_VOICENUM          CELL_ERROR_CAST(0x8031030d)  /* bad voice num              */
#define CELL_SND3_ERROR_RESERVEDVOICE     CELL_ERROR_CAST(0x8031030e)  /* voice is reserved by user  */
#define CELL_SND3_ERROR_REQUESTQUEFULL    CELL_ERROR_CAST(0x8031030f)  /* request queue if full      */
#define CELL_SND3_ERROR_OUTPUTMODE        CELL_ERROR_CAST(0x80310310)  /* out of output mode         */

#define CELL_SND3_DATA_CTX_SIZE   (32)
#define CELL_SND3_SMF_CTX_SIZE    (352)

#define CELL_SND3_VOICESTATUS_RESERVED         (0)
#define CELL_SND3_VOICESTATUS_IDLE             (1)
#define CELL_SND3_VOICESTATUS_NOTEON           (2)
#define CELL_SND3_VOICESTATUS_BUSY             (3)
#define CELL_SND3_VOICESTATUS_NOTEOFF          (4)
#define CELL_SND3_VOICESTATUS_RELEASE          (5)
#define CELL_SND3_VOICESTATUS_CONTINUE         (6)
#define CELL_SND3_VOICESTATUS_SUSTAIN          (7)
#define CELL_SND3_VOICESTATUS_NOTEOFFSUSTAIN   (8)
#define CELL_SND3_VOICESTATUS_NOTEONSUSTAIN    (9)

#define CELL_SND3_OFF            (0)
#define CELL_SND3_ON             (1)
#define CELL_SND3_INFINITY       (0xffffffff)

#define CELL_SND3_PLAYSTATUS_READY       (0)   /* ready play    */
#define CELL_SND3_PLAYSTATUS_PLAY        (1)   /* now playing   */
#define CELL_SND3_PLAYSTATUS_STOP        (2)   /* play is stop  */
#define CELL_SND3_PLAYSTATUS_ERROR       (3)   /* error in play */

#define CELL_SND3_OUTPUTSAMPLES_USB   (384)
#define CELL_SND3_OUTPUTSAMPLES_RSX   (256)

#define CELL_SND3_OUTPUT_2CH   (0)
#define CELL_SND3_OUTPUT_4CH   (1)

#define CELL_SND3_MEM_INT   (0)
#define CELL_SND3_MEM_EXT   (0x80000000)

/* key on parameter */
typedef struct {
	uint8_t   vel;        /* velocity  */
	uint8_t   pan;        /* pan       */
	uint8_t   panEx;      /* FR-pan    */
	int32_t   addPitch;   /* add pitch */
} CellSnd3KeyOnParam;

/* voice bit context */
#define CELL_SND3_MAX_CORE 4
typedef struct{
	uint32_t   core[CELL_SND3_MAX_CORE];
} CellSnd3VoiceBitCtx;

/* sound data context */
typedef struct{
	int8_t   system[CELL_SND3_DATA_CTX_SIZE];
} CellSnd3DataCtx;

/* SMF context */
typedef struct{
	int8_t   system[CELL_SND3_SMF_CTX_SIZE];
} CellSnd3SmfCtx;

/* request queue context */
typedef struct{
	void       *frontQueue;
	uint32_t   frontQueueSize;
	void       *rearQueue;
	uint32_t   rearQueueSize;
} CellSnd3RequestQueueCtx;

/* initial/exit functions */
int32_t cellSnd3Init(uint32_t maxVoice, uint32_t samples,
					 CellSnd3RequestQueueCtx *queue);
int32_t cellSnd3Exit(void);

/* service functions */
uint16_t cellSnd3Note2Pitch(uint16_t center_note, uint16_t center_fine,
							uint16_t note, int16_t fine);
uint16_t cellSnd3Pitch2Note(uint16_t center_note, uint16_t center_fine,
							uint16_t pitch);

/* sound work functions */
int32_t cellSnd3SetOutputMode(uint32_t mode);
int32_t cellSnd3Synthesis(float *pOutL, float *pOutR);
int32_t cellSnd3SynthesisEx(float *pOutL, float *pOutR, float *pOutRL, float *pOutRR);  /* -plus- */
int32_t cellSnd3BindSoundData(CellSnd3DataCtx *snd3Ctx, void *hd3,
							  uint32_t synthMemOffset);
int32_t cellSnd3UnbindSoundData(uint32_t hd3ID);
int32_t cellSnd3NoteOnByTone(uint32_t hd3ID, uint32_t toneIndex, uint32_t note,
							 uint32_t keyOnID, CellSnd3KeyOnParam *keyOnParam);
int32_t cellSnd3KeyOnByTone(uint32_t hd3ID, uint32_t toneIndex, uint32_t pitch,
							uint32_t keyOnID, CellSnd3KeyOnParam *keyOnParam);
int32_t cellSnd3VoiceNoteOnByTone(uint32_t hd3ID, uint32_t voiceNum,
								  uint32_t toneIndex, uint32_t note,
								  uint32_t keyOnID,
								  CellSnd3KeyOnParam *keyOnParam);
int32_t cellSnd3VoiceKeyOnByTone(uint32_t hd3ID, uint32_t voiceNum,
								 uint32_t toneIndex, uint32_t pitch,
								 uint32_t keyOnID,
								 CellSnd3KeyOnParam *keyOnParam);
int32_t cellSnd3VoiceSetReserveMode(uint32_t voiceNum, uint32_t reserveMode);
int32_t cellSnd3VoiceSetSustainHold(uint32_t voiceNum, uint32_t sustainHold);
int32_t cellSnd3VoiceKeyOff(uint32_t voiceNum);
int32_t cellSnd3VoiceSetPitch(uint32_t voiceNum, int32_t addPitch);
int32_t cellSnd3VoiceSetVelocity(uint32_t voiceNum, uint32_t velocity);
int32_t cellSnd3VoiceSetPanpot(uint32_t voiceNum, uint32_t panpot);
int32_t cellSnd3VoiceSetPanpotEx(uint32_t voiceNum, uint32_t panpotEx);
int32_t cellSnd3VoiceSetPitchBend(uint32_t voiceNum, uint32_t bendValue);
int32_t cellSnd3VoiceAllKeyOff(void);
int32_t cellSnd3VoiceGetEnvelope(uint32_t voiceNum);
int32_t cellSnd3VoiceGetStatus(uint32_t voiceNum);
int32_t cellSnd3KeyOffByID(uint32_t keyOnID);
int32_t cellSnd3GetVoice(uint32_t midiChannel, uint32_t keyOnID,
						 CellSnd3VoiceBitCtx *voiceBit);
int32_t cellSnd3GetVoiceByID(uint32_t ID, CellSnd3VoiceBitCtx *voiceBit);
int32_t cellSnd3NoteOn(uint32_t hd3ID, uint32_t midiChannel,
					   uint32_t midiProgram, uint32_t midiNote,
					   uint32_t sustain, CellSnd3KeyOnParam *keyOnParam,
					   uint32_t keyOnID);
int32_t cellSnd3NoteOff(uint32_t midiChannel, uint32_t midiNote,
						uint32_t keyOnID);
int32_t cellSnd3SetSustainHold(uint32_t midiChannel, uint32_t sustainHold,
							   uint32_t keyOnID);

/* SMF functions */
int32_t cellSnd3SMFBind(CellSnd3SmfCtx *smfCtx, void *smf, uint32_t hd3ID);
int32_t cellSnd3SMFUnbind(uint32_t smfID);
int32_t cellSnd3SMFPlay(uint32_t smfID, uint32_t playVelocity,
						uint32_t playPan,uint32_t playCount);
int32_t cellSnd3SMFPlayEx(uint32_t smfID, uint32_t playVelocity, uint32_t playPan,
						  uint32_t playPanEx, uint32_t playCount);
int32_t cellSnd3SMFPause(uint32_t smfID);
int32_t cellSnd3SMFResume(uint32_t smfID);
int32_t cellSnd3SMFStop(uint32_t smfID);
int32_t cellSnd3SMFAddTempo(uint32_t smfID, int32_t addTempo);
int32_t cellSnd3SMFGetTempo(uint32_t smfID);
int32_t cellSnd3SMFSetPlayVelocity(uint32_t smfID, uint32_t playVelocity);
int32_t cellSnd3SMFGetPlayVelocity(uint32_t smfID);
int32_t cellSnd3SMFSetPlayPanpot(uint32_t smfID, uint32_t playPanpot);
int32_t cellSnd3SMFSetPlayPanpotEx(uint32_t smfID, uint32_t playPanpotEx);
int32_t cellSnd3SMFGetPlayPanpot(uint32_t smfID);
int32_t cellSnd3SMFGetPlayPanpotEx(uint32_t smfID);
int32_t cellSnd3SMFGetPlayStatus(uint32_t smfID);
int32_t cellSnd3SMFSetPlayChannel(uint32_t smfID, uint32_t playChannelBit);
int32_t cellSnd3SMFGetPlayChannel(uint32_t smfID, uint32_t *playChannelBit);
int32_t cellSnd3SMFGetKeyOnID(uint32_t smfID, uint32_t midiChannel,
							  uint32_t *keyOnID);

/* effect functions */
int32_t cellSnd3SetEffectType(uint16_t effectType, int16_t returnVol, uint16_t delay, uint16_t feedback);

#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
}
#endif	/* defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus) */

#endif	/* _CELL_SND3_H */
