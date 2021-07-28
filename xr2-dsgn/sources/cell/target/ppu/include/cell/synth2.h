/* SCE CONFIDENTIAL
 PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2006 Sony Computer Entertainment Inc.
 * All Rights Reserved.
 */

//----------------------------------------------
//	libsynth2 definitions
//----------------------------------------------
#ifndef __CELL_SOUND_SYNTH2_H__
#define __CELL_SOUND_SYNTH2_H__

#include <stdint.h>
#include <cell/error.h>

#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
extern "C" {
#endif


//----------------------------------------------
//	error macros
//----------------------------------------------
#define CELL_SOUND_SYNTH2_MAKE_ERROR(status) \
	CELL_ERROR_MAKE_ERROR(CELL_ERROR_FACILITY_SOUND, status)

#define CELL_SOUND_SYNTH2_OK	CELL_OK

/* ERR: CELL_SOUND_SYNTH2_MAKE_ERROR(0x0201) */
#define CELL_SOUND_SYNTH2_ERROR_FATAL						CELL_ERROR_CAST(0x80310201)

/* ERR: CELL_SOUND_SYNTH2_MAKE_ERROR(0x0202) */
#define CELL_SOUND_SYNTH2_ERROR_INVALID_PARAMETER			CELL_ERROR_CAST(0x80310202)

/* ERR: CELL_SOUND_SYNTH2_MAKE_ERROR(0x0203) */
#define CELL_SOUND_SYNTH2_ERROR_ALREADY_INITIALIZED			CELL_ERROR_CAST(0x80310203)


//----------------------------------------------
#define CELL_SOUND_SYNTH2_VOICE_PER_CORE	(32)
#define CELL_SOUND_SYNTH2_CORE_PER_SYSTEM	(4)
#define CELL_SOUND_SYNTH2_VOICE_PER_SYSTEM	(128)

typedef struct {
	uint16_t core;
	uint16_t mode;
	int16_t depth_L;
	int16_t depth_R;
	uint16_t delay;
	uint16_t feedback;
} CellSoundSynth2EffectAttr;

//	initialize and exit function
int32_t cellSoundSynth2Init(int16_t flag);
int32_t cellSoundSynth2Exit(void);
int32_t cellSoundSynth2Config(int16_t,int32_t);

//	Register wrapper functions
uint32_t cellSoundSynth2GetAddr(uint16_t reg);
uint16_t cellSoundSynth2GetParam(uint16_t reg);
uint32_t cellSoundSynth2GetSwitch(uint16_t reg);
int32_t cellSoundSynth2SetAddr(uint16_t reg,uint32_t value);
void cellSoundSynth2SetParam(uint16_t reg, uint16_t value);
void cellSoundSynth2SetSwitch(uint16_t reg, uint32_t value);

//	Effect setup functions
int32_t cellSoundSynth2SetEffectMode(int16_t reg,CellSoundSynth2EffectAttr *attr);
int32_t cellSoundSynth2SetEffectAttr(int16_t reg,CellSoundSynth2EffectAttr *attr);

//	Service functions
uint16_t cellSoundSynth2Note2Pitch (uint16_t center_note, uint16_t center_fine, uint16_t note, int16_t fine);
uint16_t cellSoundSynth2Pitch2Note(uint16_t center_note, uint16_t center_fine,uint16_t pitch);

//	Data transfer function
int32_t cellSoundSynth2VoiceTrans(int16_t channel,uint16_t mode,uint8_t *m_addr,uint32_t s_addr,uint32_t size);
int32_t cellSoundSynth2VoiceTransStatus(int16_t channel,int16_t flag);

//	Core-common function
void cellSoundSynth2SetCoreAttr(uint16_t entry,uint16_t value);

//	Sound generate function
int32_t cellSoundSynth2Generate(uint16_t n,float *Lout,float *Rout,float *Ls,float *Rs);


//----------------------------------------------
//	reg : 00ff.ffff.vvvv.v0cc
//			c : Core select
//			v : voice numver
//			f : function number
//----------------------------------------------
#define CELL_SOUND_SYNTH2_CORE_0			(0x0000)
#define CELL_SOUND_SYNTH2_CORE_1			(0x0001)
#define CELL_SOUND_SYNTH2_CORE_2			(0x0002)
#define CELL_SOUND_SYNTH2_CORE_3			(0x0003)
#define CELL_SOUND_SYNTH2_CORE(n)			(n&0x03)

#define CELL_SOUND_SYNTH2_CORE_BY_VOICE(n)	(n>>5)

#define CELL_SOUND_SYNTH2_FRONT				(0)
#define CELL_SOUND_SYNTH2_REAR				(1)

//----------------------------------------------
#define CELL_SOUND_SYNTH2_VOICE_BIT			(3)

#define CELL_SOUND_SYNTH2_VOICE_00			(0<<CELL_SOUND_SYNTH2_VOICE_BIT)
#define CELL_SOUND_SYNTH2_VOICE_01			(1<<CELL_SOUND_SYNTH2_VOICE_BIT)
#define CELL_SOUND_SYNTH2_VOICE_02			(2<<CELL_SOUND_SYNTH2_VOICE_BIT)
#define CELL_SOUND_SYNTH2_VOICE_03			(3<<CELL_SOUND_SYNTH2_VOICE_BIT)
#define CELL_SOUND_SYNTH2_VOICE_04			(4<<CELL_SOUND_SYNTH2_VOICE_BIT)
#define CELL_SOUND_SYNTH2_VOICE_05			(5<<CELL_SOUND_SYNTH2_VOICE_BIT)
#define CELL_SOUND_SYNTH2_VOICE_06			(6<<CELL_SOUND_SYNTH2_VOICE_BIT)
#define CELL_SOUND_SYNTH2_VOICE_07			(7<<CELL_SOUND_SYNTH2_VOICE_BIT)
#define CELL_SOUND_SYNTH2_VOICE_08			(8<<CELL_SOUND_SYNTH2_VOICE_BIT)
#define CELL_SOUND_SYNTH2_VOICE_09			(9<<CELL_SOUND_SYNTH2_VOICE_BIT)
#define CELL_SOUND_SYNTH2_VOICE_10			(10<<CELL_SOUND_SYNTH2_VOICE_BIT)
#define CELL_SOUND_SYNTH2_VOICE_11			(11<<CELL_SOUND_SYNTH2_VOICE_BIT)
#define CELL_SOUND_SYNTH2_VOICE_12			(12<<CELL_SOUND_SYNTH2_VOICE_BIT)
#define CELL_SOUND_SYNTH2_VOICE_13			(13<<CELL_SOUND_SYNTH2_VOICE_BIT)
#define CELL_SOUND_SYNTH2_VOICE_14			(14<<CELL_SOUND_SYNTH2_VOICE_BIT)
#define CELL_SOUND_SYNTH2_VOICE_15			(15<<CELL_SOUND_SYNTH2_VOICE_BIT)
#define CELL_SOUND_SYNTH2_VOICE_16			(16<<CELL_SOUND_SYNTH2_VOICE_BIT)
#define CELL_SOUND_SYNTH2_VOICE_17			(17<<CELL_SOUND_SYNTH2_VOICE_BIT)
#define CELL_SOUND_SYNTH2_VOICE_18			(18<<CELL_SOUND_SYNTH2_VOICE_BIT)
#define CELL_SOUND_SYNTH2_VOICE_19			(19<<CELL_SOUND_SYNTH2_VOICE_BIT)
#define CELL_SOUND_SYNTH2_VOICE_20			(20<<CELL_SOUND_SYNTH2_VOICE_BIT)
#define CELL_SOUND_SYNTH2_VOICE_21			(21<<CELL_SOUND_SYNTH2_VOICE_BIT)
#define CELL_SOUND_SYNTH2_VOICE_22			(22<<CELL_SOUND_SYNTH2_VOICE_BIT)
#define CELL_SOUND_SYNTH2_VOICE_23			(23<<CELL_SOUND_SYNTH2_VOICE_BIT)
#define CELL_SOUND_SYNTH2_VOICE_24			(24<<CELL_SOUND_SYNTH2_VOICE_BIT)
#define CELL_SOUND_SYNTH2_VOICE_25			(25<<CELL_SOUND_SYNTH2_VOICE_BIT)
#define CELL_SOUND_SYNTH2_VOICE_26			(26<<CELL_SOUND_SYNTH2_VOICE_BIT)
#define CELL_SOUND_SYNTH2_VOICE_27			(27<<CELL_SOUND_SYNTH2_VOICE_BIT)
#define CELL_SOUND_SYNTH2_VOICE_28			(28<<CELL_SOUND_SYNTH2_VOICE_BIT)
#define CELL_SOUND_SYNTH2_VOICE_29			(29<<CELL_SOUND_SYNTH2_VOICE_BIT)
#define CELL_SOUND_SYNTH2_VOICE_30			(30<<CELL_SOUND_SYNTH2_VOICE_BIT)
#define CELL_SOUND_SYNTH2_VOICE_31			(31<<CELL_SOUND_SYNTH2_VOICE_BIT)
#define CELL_SOUND_SYNTH2_VOICE(n)			( (((n)&0x1f)<<CELL_SOUND_SYNTH2_VOICE_BIT)|CELL_SOUND_SYNTH2_CORE(n>>5) )

//----------------------------------------------
//	API function selection MACRO
//----------------------------------------------
#define CELL_SOUND_SYNTH2_API_BIT			(CELL_SOUND_SYNTH2_VOICE_BIT+5)
#define	CELL_SOUND_SYNTH2_P_MVOLL			(0x00<<CELL_SOUND_SYNTH2_API_BIT)
#define	CELL_SOUND_SYNTH2_P_MVOLR			(0x01<<CELL_SOUND_SYNTH2_API_BIT)
#define CELL_SOUND_SYNTH2_S_KON				(0x04<<CELL_SOUND_SYNTH2_API_BIT)
#define	CELL_SOUND_SYNTH2_S_KOFF			(0x06<<CELL_SOUND_SYNTH2_API_BIT)
#define	CELL_SOUND_SYNTH2_S_NON				(0x0a<<CELL_SOUND_SYNTH2_API_BIT)
#define	CELL_SOUND_SYNTH2_S_ENDX			(0x0e<<CELL_SOUND_SYNTH2_API_BIT)

#define	CELL_SOUND_SYNTH2_P_MVOLXL			(0x1c<<CELL_SOUND_SYNTH2_API_BIT)
#define	CELL_SOUND_SYNTH2_P_MVOLXR			(0x1d<<CELL_SOUND_SYNTH2_API_BIT)

#define	CELL_SOUND_SYNTH2_VP_PITCH			(0x22<<CELL_SOUND_SYNTH2_API_BIT)
#define	CELL_SOUND_SYNTH2_VA_SSA			(0x23<<CELL_SOUND_SYNTH2_API_BIT)
#define	CELL_SOUND_SYNTH2_VP_ADSR1			(0x24<<CELL_SOUND_SYNTH2_API_BIT)
#define	CELL_SOUND_SYNTH2_VP_ADSR2			(0x25<<CELL_SOUND_SYNTH2_API_BIT)
#define	CELL_SOUND_SYNTH2_VP_ENVX			(0x26<<CELL_SOUND_SYNTH2_API_BIT)
#define	CELL_SOUND_SYNTH2_VA_LSAX			(0x27<<CELL_SOUND_SYNTH2_API_BIT)
#define	CELL_SOUND_SYNTH2_VA_NAX			(0x28<<CELL_SOUND_SYNTH2_API_BIT)
#define	CELL_SOUND_SYNTH2_C_EFFECT_ENABLE	(0x29<<CELL_SOUND_SYNTH2_API_BIT)

#define CELL_SOUND_SYNTH2_VP_CHVOLL			(0x30<<CELL_SOUND_SYNTH2_API_BIT)
#define CELL_SOUND_SYNTH2_VP_CHVOLR			(0x31<<CELL_SOUND_SYNTH2_API_BIT)
#define CELL_SOUND_SYNTH2_VP_CHVOLEL		(0x32<<CELL_SOUND_SYNTH2_API_BIT)
#define CELL_SOUND_SYNTH2_VP_CHVOLER		(0x33<<CELL_SOUND_SYNTH2_API_BIT)
#define CELL_SOUND_SYNTH2_VP_CHVOLLS		(0x34<<CELL_SOUND_SYNTH2_API_BIT)
#define CELL_SOUND_SYNTH2_VP_CHVOLRS		(0x35<<CELL_SOUND_SYNTH2_API_BIT)
#define CELL_SOUND_SYNTH2_VP_CHVOLELS		(0x36<<CELL_SOUND_SYNTH2_API_BIT)
#define CELL_SOUND_SYNTH2_VP_CHVOLERS		(0x37<<CELL_SOUND_SYNTH2_API_BIT)

//----------------------------------------------
//	ADSR
//----------------------------------------------
#define CELL_SOUND_SYNTH2_ADSR_MASK_AR		(0xff00)
#define CELL_SOUND_SYNTH2_ADSR_MASK_DR		(0x00f0)
#define CELL_SOUND_SYNTH2_ADSR_MASK_SL		(0x000f)
#define CELL_SOUND_SYNTH2_ADSR_MASK_SR		(0xffc0)
#define CELL_SOUND_SYNTH2_ADSR_MASK_RR		(0x003f)
#define CELL_SOUND_SYNTH2_ADSR_MASK_AM		(0x8000)
#define CELL_SOUND_SYNTH2_ADSR_MASK_SM		(0xe000)

#define CELL_SOUND_SYNTH2_ADSR_A_LINEAR		(0)
#define CELL_SOUND_SYNTH2_ADSR_A_EXP		(0x8000)
#define CELL_SOUND_SYNTH2_ADSR_ARATE(x)		(((x)&0x7f)<<8)
#define CELL_SOUND_SYNTH2_ADSR_DRATE(x)		(((x)&0x0f)<<4)
#define CELL_SOUND_SYNTH2_ADSR_SLEVEL(x)		(((x)&0x0f))

#define CELL_SOUND_SYNTH2_ADSR_S_LINEAR_INC	(0<<13)
#define CELL_SOUND_SYNTH2_ADSR_S_LINEAR_DEC	(2<<13)
#define CELL_SOUND_SYNTH2_ADSR_S_EXP_INC	(4<<13)
#define CELL_SOUND_SYNTH2_ADSR_S_EXP_DEC	(6<<13)
#define CELL_SOUND_SYNTH2_ADSR_SRATE(x)		(((x)&0x7f)<<6)
#define CELL_SOUND_SYNTH2_ADSR_R_LINEAR		(0)
#define CELL_SOUND_SYNTH2_ADSR_R_EXP		(0x20)
#define CELL_SOUND_SYNTH2_ADSR_RRATE(x)		(((x)&0x01f))

#define CELL_SOUND_SYNTH2_ADSR1(x, y, z, w) ((x)|CELL_SOUND_SYNTH2_ADSR_ARATE(y)|CELL_SOUND_SYNTH2_ADSR_DRATE(z)|CELL_SOUND_SYNTH2_ADSR_SLEVEL(w))
#define CELL_SOUND_SYNTH2_ADSR2(x, y, z, w) ((x)|CELL_SOUND_SYNTH2_ADSR_SRATE(y)|(z)|CELL_SOUND_SYNTH2_ADSR_RRATE(w))

//----------------------------------------------
//	Effect mode
//----------------------------------------------
#define CELL_SOUND_SYNTH2_REV_MODE_OFF		0
#define CELL_SOUND_SYNTH2_REV_MODE_ROOM		1
#define CELL_SOUND_SYNTH2_REV_MODE_STUDIO_A	2
#define CELL_SOUND_SYNTH2_REV_MODE_STUDIO_B	3
#define CELL_SOUND_SYNTH2_REV_MODE_STUDIO_C	4
#define CELL_SOUND_SYNTH2_REV_MODE_HALL		5
#define CELL_SOUND_SYNTH2_REV_MODE_SPACE	6
#define CELL_SOUND_SYNTH2_REV_MODE_ECHO		7
#define CELL_SOUND_SYNTH2_REV_MODE_DELAY	8
#define CELL_SOUND_SYNTH2_REV_MODE_PIPE		9

//----------------------------------------------
#define CELL_SOUND_SYNTH2_TRANS_MODE_WRITE	0
#define CELL_SOUND_SYNTH2_TRANS_MODE_READ	1

//----------------------------------------------
//	Initialize mode
//----------------------------------------------
#define CELL_SOUND_SYNTH2_INIT_MODE_MEM_INT	0
#define CELL_SOUND_SYNTH2_INIT_MODE_MEM_EXT	(1<<0)

//----------------------------------------------
//	System config
//----------------------------------------------
#define CELL_SOUND_SYNTH2_CONFIG_SPU_THREAD_PRIORITY	1




#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
}	//	end of "extern C" definition
#endif

#endif	/*	__CELL_SOUND_SYNTH2_H__	*/
