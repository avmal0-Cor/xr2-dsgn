/* SCE CONFIDENTIAL
 PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2005 Sony Computer Entertainment Inc.
 * All Rights Reserved.
 */

#ifndef _CELL_SYNTH2_SD_COMPAT_H
#define _CELL_SYNTH2_SD_COMPAT_H

/* return value */
#define SCESD_OK			0
#define SCESD_ERROR			(-1)

//#define SCESD_EINVALID_ARGUMENT		(-100)
//#define SCESD_EINVALID_STATUS			(-201)
//#define SCESD_EILLEGAL_CONTEXT		(-202)
//#define SCESD_EBUSY					(-210)

//#define SCESD_ENO_RESOURCES			(-301)

//#define SCESD_TRANSFER_FINISHED		1
//#define SCESD_TRANSFER_CONTINUE		0

/* CORE */
#define SD_CORE_0		CELL_SOUND_SYNTH2_CORE_0
#define SD_CORE_1		CELL_SOUND_SYNTH2_CORE_1
//#define SD_CORE_MAX

/* VOICE */
//#define SD_MAX_VOICE	CELL_SOUND_SYNTH2_VOICES_PER_CORE
//#define SD_VOICE_MAX	CELL_SOUND_SYNTH2_VOICES_PER_CORE
#define SD_MAX_VOICE	24
#define SD_VOICE_MAX	24

#define SD_VOICE_00		CELL_SOUND_SYNTH2_VOICE_00
#define SD_VOICE_01		CELL_SOUND_SYNTH2_VOICE_01
#define SD_VOICE_02		CELL_SOUND_SYNTH2_VOICE_02
#define SD_VOICE_03		CELL_SOUND_SYNTH2_VOICE_03
#define SD_VOICE_04		CELL_SOUND_SYNTH2_VOICE_04
#define SD_VOICE_05		CELL_SOUND_SYNTH2_VOICE_05
#define SD_VOICE_06		CELL_SOUND_SYNTH2_VOICE_06
#define SD_VOICE_07		CELL_SOUND_SYNTH2_VOICE_07
#define SD_VOICE_08		CELL_SOUND_SYNTH2_VOICE_08
#define SD_VOICE_09		CELL_SOUND_SYNTH2_VOICE_09
#define SD_VOICE_10		CELL_SOUND_SYNTH2_VOICE_10
#define SD_VOICE_11		CELL_SOUND_SYNTH2_VOICE_11
#define SD_VOICE_12		CELL_SOUND_SYNTH2_VOICE_12
#define SD_VOICE_13		CELL_SOUND_SYNTH2_VOICE_13
#define SD_VOICE_14		CELL_SOUND_SYNTH2_VOICE_14
#define SD_VOICE_15		CELL_SOUND_SYNTH2_VOICE_15
#define SD_VOICE_16		CELL_SOUND_SYNTH2_VOICE_16
#define SD_VOICE_17		CELL_SOUND_SYNTH2_VOICE_17
#define SD_VOICE_18		CELL_SOUND_SYNTH2_VOICE_18
#define SD_VOICE_19		CELL_SOUND_SYNTH2_VOICE_19
#define SD_VOICE_20		CELL_SOUND_SYNTH2_VOICE_20
#define SD_VOICE_21		CELL_SOUND_SYNTH2_VOICE_21
#define SD_VOICE_22		CELL_SOUND_SYNTH2_VOICE_22
#define SD_VOICE_23		CELL_SOUND_SYNTH2_VOICE_23
#define SD_VOICE_XX		CELL_SOUND_SYNTH2_VOICE_XX	   
#define SD_VOICE(n)		CELL_SOUND_SYNTH2_VOICE(n)

/* param */
#define SD_VP_VOLL		CELL_SOUND_SYNTH2_VP_VOLL		
#define SD_VP_VOLR		CELL_SOUND_SYNTH2_VP_VOLR		
#define SD_VP_PITCH		CELL_SOUND_SYNTH2_VP_PITCH		
#define SD_VP_ADSR1		CELL_SOUND_SYNTH2_VP_ADSR1		
#define SD_VP_ADSR2		CELL_SOUND_SYNTH2_VP_ADSR2		
#define SD_VP_ENVX		CELL_SOUND_SYNTH2_VP_ENVX		
#define SD_VP_VOLXL		CELL_SOUND_SYNTH2_VP_VOLXL		
#define SD_VP_VOLXR		CELL_SOUND_SYNTH2_VP_VOLXR		
#define SD_P_MMIX		CELL_SOUND_SYNTH2_P_MMIX		
#define SD_P_MVOLL		CELL_SOUND_SYNTH2_P_MVOLL		
#define SD_P_MVOLR		CELL_SOUND_SYNTH2_P_MVOLR		
#define SD_P_EVOLL		CELL_SOUND_SYNTH2_P_EVOLL		
#define SD_P_EVOLR		CELL_SOUND_SYNTH2_P_EVOLR		
#define SD_P_AVOLL		CELL_SOUND_SYNTH2_P_AVOLL		
#define SD_P_AVOLR		CELL_SOUND_SYNTH2_P_AVOLR		
#define SD_P_BVOLL		CELL_SOUND_SYNTH2_P_BVOLL		
#define SD_P_BVOLR		CELL_SOUND_SYNTH2_P_BVOLR		
#define SD_P_MVOLXL		CELL_SOUND_SYNTH2_P_MVOLXL		
#define SD_P_MVOLXR		CELL_SOUND_SYNTH2_P_MVOLXR		

/* switch */
#define SD_S_PMON		CELL_SOUND_SYNTH2_S_PMON		
#define SD_S_NON		CELL_SOUND_SYNTH2_S_NON		
#define SD_S_KON		CELL_SOUND_SYNTH2_S_KON		
#define SD_S_KOFF		CELL_SOUND_SYNTH2_S_KOFF		
#define SD_S_ENDX		CELL_SOUND_SYNTH2_S_ENDX		
#define SD_S_VMIXL		CELL_SOUND_SYNTH2_S_VMIXL		
#define SD_S_VMIXEL		CELL_SOUND_SYNTH2_S_VMIXEL		
#define SD_S_VMIXR		CELL_SOUND_SYNTH2_S_VMIXR		
#define SD_S_VMIXER		CELL_SOUND_SYNTH2_S_VMIXER		

/* address */
//#define SD_A_ESA
//#define SD_A_EEA
//#define SD_A_TSA
//#define SD_A_IRQA
#define SD_VA_SSA		CELL_SOUND_SYNTH2_VA_SSA
#define SD_VA_LSAX		CELL_SOUND_SYNTH2_VA_LSAX
#define SD_VA_NAX		CELL_SOUND_SYNTH2_VA_NAX

/* CoreAttr */
#define SD_C_EFFECT_ENABLE		CELL_SOUND_SYNTH2_C_EFFECT_ENABLE
//#define SD_C_IRQ_ENABLE
//#define SD_C_MUTE_ENABLE
//#define SD_C_NOISE_CLK
//#define SD_C_SPDIF_MODE

/* SPDIF OUT */
//#define SD_SPDIF_OUT_PCM
//#define SD_SPDIF_OUT_BITSTREAM
//#define SD_SPDIF_OUT_OFF
//#define SD_SPDIF_OUT_BYPASS

//#define SD_SPDIF_COPY_NORMAL
//#define SD_SPDIF_COPY_PROHIBIT

//#define SD_SPDIF_MEDIA_CD
//#define SD_SPDIF_MEDIA_DVD

/* MIX */
//#define SD_MMIX_SINER
//#define SD_MMIX_SINEL
//#define SD_MMIX_SINR
//#define SD_MMIX_SINL
//#define SD_MMIX_MINER
//#define SD_MMIX_MINEL
//#define SD_MMIX_MINR
//#define SD_MMIX_MINL
//#define SD_MMIX_MSNDER
//#define SD_MMIX_MSNDEL
//#define SD_MMIX_MSNDR
//#define SD_MMIX_MSNDL

/* ADSR */
#define SD_ADSR_MASK_AR			CELL_SOUND_SYNTH2_ADSR_MASK_AR
#define SD_ADSR_MASK_DR			CELL_SOUND_SYNTH2_ADSR_MASK_DR
#define SD_ADSR_MASK_SL			CELL_SOUND_SYNTH2_ADSR_MASK_SL
#define SD_ADSR_MASK_SR			CELL_SOUND_SYNTH2_ADSR_MASK_SR
#define SD_ADSR_MASK_RR			CELL_SOUND_SYNTH2_ADSR_MASK_RR	
#define SD_ADSR_MASK_AM			CELL_SOUND_SYNTH2_ADSR_MASK_AM
#define SD_ADSR_MASK_SM			CELL_SOUND_SYNTH2_ADSR_MASK_SM

#define SD_ADSR_A_LINEAR		CELL_SOUND_SYNTH2_ADSR_A_LINEAR
#define SD_ADSR_A_EXP			CELL_SOUND_SYNTH2_ADSR_A_EXP
#define SD_ADSR_ARATE(x)		CELL_SOUND_SYNTH2_ADSR_ARATE(x)
#define SD_ADSR_DRATE(x)		CELL_SOUND_SYNTH2_ADSR_DRATE(x)
#define SD_ADSR_SLEVEL(x)		CELL_SOUND_SYNTH2_ADSR_SLEVEL(x)

#define SD_ADSR_S_LINEAR_INC	CELL_SOUND_SYNTH2_ADSR_S_LINEAR_INC
#define SD_ADSR_S_LINEAR_DEC	CELL_SOUND_SYNTH2_ADSR_S_LINEAR_DEC
#define SD_ADSR_S_EXP_INC		CELL_SOUND_SYNTH2_ADSR_S_EXP_INC
#define SD_ADSR_S_EXP_DEC		CELL_SOUND_SYNTH2_ADSR_S_EXP_DEC
#define SD_ADSR_SRATE(x)		CELL_SOUND_SYNTH2_ADSR_SRATE(x)
#define SD_ADSR_R_LINEAR		CELL_SOUND_SYNTH2_ADSR_R_LINEAR
#define SD_ADSR_R_EXP			CELL_SOUND_SYNTH2_ADSR_R_EXP
#define SD_ADSR_RRATE(x)		CELL_SOUND_SYNTH2_ADSR_RRATE(x)

#define SD_ADSR1(x, y, z, w)	CELL_SOUND_SYNTH2_ADSR1(x, y, z, w)
#define SD_ADSR2(x, y, z, w)	CELL_SOUND_SYNTH2_ADSR2(x, y, z, w)

/* Batch command */
//#define SD_BSET_PARAM
//#define SD_BGET_PARAM
//#define SD_BSET_SWITCH
//#define SD_BGET_SWITCH
//#define SD_BSET_ADDR
//#define SD_BGET_ADDR
//#define SD_BSET_CORE
//#define SD_BGET_CORE
//#define SD_WRITE_IOP
//#define SD_WRITE_EE
//#define SD_RETURN_EE

/* Transfer */
#define SD_TRANS_MODE_WRITE		CELL_SOUND_SYNTH2_TRANS_MODE_WRITE
#define SD_TRANS_MODE_READ		CELL_SOUND_SYNTH2_TRANS_MODE_READ
//#define SD_TRANS_MODE_STOP
//#define SD_TRANS_MODE_WRITE_FROM

// DMA transfer is the same as I/O transfer in this environment
#define SD_TRANS_BY_DMA			0
#define SD_TRANS_BY_IO			0

//#define SD_BLOCK_ONESHOT
//#define SD_BLOCK_LOOP
//#define SD_BLOCK_HANDLER

//#define SD_BLOCK_C0_VOICE1
//#define SD_BLOCK_C0_VOICE3
//#define SD_BLOCK_C1_SINL
//#define SD_BLOCK_C1_SINR
//#define SD_BLOCK_C1_VOICE1
//#define SD_BLOCK_C1_VOICE3
//#define SD_BLOCK_C0_MEMOUTL
//#define SD_BLOCK_C0_MEMOUTR
//#define SD_BLOCK_C0_MEMOUTEL
//#define SD_BLOCK_C0_MEMOUTER
//#define SD_BLOCK_C1_MEMOUTL
//#define SD_BLOCK_C1_MEMOUTR
//#define SD_BLOCK_C1_MEMOUTEL
//#define SD_BLOCK_C1_MEMOUTER

//#define SD_BLOCK_COUNT(x)

// Waveform data transferring is synchronous call
#define SD_TRANS_STATUS_WAIT	0
#define SD_TRANS_STATUS_CHECK	0

/* Effect */
#define SD_REV_MODE_OFF			CELL_SOUND_SYNTH2_REV_MODE_OFF			
#define SD_REV_MODE_ROOM		CELL_SOUND_SYNTH2_REV_MODE_ROOM		
#define SD_REV_MODE_STUDIO_A	CELL_SOUND_SYNTH2_REV_MODE_STUDIO_A	
#define SD_REV_MODE_STUDIO_B	CELL_SOUND_SYNTH2_REV_MODE_STUDIO_B	
#define SD_REV_MODE_STUDIO_C	CELL_SOUND_SYNTH2_REV_MODE_STUDIO_C	
#define SD_REV_MODE_HALL		CELL_SOUND_SYNTH2_REV_MODE_HALL		
#define SD_REV_MODE_SPACE		CELL_SOUND_SYNTH2_REV_MODE_SPACE		
#define SD_REV_MODE_ECHO		CELL_SOUND_SYNTH2_REV_MODE_ECHO		
#define SD_REV_MODE_DELAY		CELL_SOUND_SYNTH2_REV_MODE_DELAY		
#define SD_REV_MODE_PIPE		CELL_SOUND_SYNTH2_REV_MODE_PIPE
#define SD_REV_MODE_MAX			(CELL_SOUND_SYNTH2_REV_MODE_PIPE + 1)
//#define SD_REV_MODE_CLEAR_WA
//#define SD_REV_MODE_CLEAR_CH0
//#define SD_REV_MODE_CLEAR_CH1
//#define SD_REV_MODE_CLEAR_CH_MASK

//#define SD_REV_SIZE_OFF
//#define SD_REV_SIZE_ROOM
//#define SD_REV_SIZE_STUDIO_A
//#define SD_REV_SIZE_STUDIO_B
//#define SD_REV_SIZE_STUDIO_C
//#define SD_REV_SIZE_HALL
//#define SD_REV_SIZE_SPACE
//#define SD_REV_SIZE_ECHO
//#define SD_REV_SIZE_DELAY
//#define SD_REV_SIZE_PIPE

/* SPU2 local memory address */
//#define SD_SOUNDOUT_TOPADDR
//#define SD_SOUNDIN_TOPADDR
//#define SD_USERAREA_TOPADDR

/* Initialize mode */
#define SD_INIT_COLD	0
#define SD_INIT_HOT		1

/* typedef */
//#define SD_TRANS_CBProc
//#define SD_IRQ_CBProc

//typedef int (*sceSdTransIntrHandler)(...);
//typedef int (*sceSdSpu2IntrHandler)(...);
//typedef int (*sceSdBlockTransHandler)(...);

/* structure definition */
//typedef struct {
//	// ...
//} sceSdBatch;

typedef CellSoundSynth2EffectAttr sceSdEffectAttr;

#define sceSdInit(flag)					cellSoundSynth2Init(flag)
#define sceSdSetParam(reg,value)		cellSoundSynth2SetParam(reg,value)
#define sceSdGetParam(reg)				cellSoundSynth2GetParam(reg)
#define sceSdSetSwitch(reg,value)		cellSoundSynth2SetSwitch(reg,value)
#define sceSdGetSwitch(reg)				cellSoundSynth2GetSwitch(reg)
#define sceSdSetAddr(reg,value)			cellSoundSynth2SetAddr(reg,value)
#define sceSdGetAddr(reg)				cellSoundSynth2GetAddr(reg)
#define sceSdSetCoreAttr(reg,value)		cellSoundSynth2SetCoreAttr(reg,value)
//#define sceSdGetCoreAttr(reg)
#define sceSdNote2Pitch(cn,cf,n,f)		cellSoundSynth2Note2Pitch(cn,cf,n,f)
#define sceSdPitch2Note(cn,cf,p)		cellSoundSynth2Pitch2Note(cn,cf,p)
//#define sceSdProcBatch(batch,returns[],num)
//#define sceSdProcBatchEx(batch,returns[],num,voice)
#define sceSdVoiceTrans(c,m,ma,sa,s)	cellSoundSynth2VoiceTrans(c,m,ma,sa,s)
//#define sceSdBlockTrans(c,m,ma,s, ...)
//#define sceSdStopTrans(c)
#define sceSdVoiceTransStatus(c,f)		cellSoundSynth2VoiceTransStatus(c,f)
//#define sceSdBlockTransStatus(c,f)
#define sceSdSetEffectAttr(c,a)			cellSoundSynth2SetEffectAttr(c,a)
//#define sceSdGetEffectAttr(c,a)
//#define sceSdClearEffectWorkArea(c,ch,m)
//#define sceSdCleanEffectWorkArea(c,ch,m)

#define sceSdSetEffectMode(c,p)			cellSoundSynth2SetEffectMode(c,p)
//#define sceSdSetEffectModeParams(c,a)

//#define sceSdSetSpu2IntrHandler(f,a)
//#define sceSdSetTransIntrHandler(c,f,a)
//#define sceSdGetTransIntrHandlerArgument(a)
//#define sceSdGetSpu2IntrHandlerArgument ()

// synth2 only
#define sceSdExit()						cellSoundSynth2Exit()
#define sceSdGenerate(n,l,r)			cellSoundSynth2Generate(n,l,r)

#endif /* _CELL_SYNTH2_SD_COMPAT_H */

/*
 * Local variables:
 * indent-tabs-mode: t
 * tab-width: 4
 * End:
 */
