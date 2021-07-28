/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2009 Sony Computer Entertainment Inc.
* All Rights Reserved.
*/
/* legacy.h sample header to aid users of pre-SDK releases of SCREAM */

// Please note: These headers are designed for users of previous version of
// scream and provide a mapping between old function names and new SDK
// function names.  A lot of unnecessary wrapper functions and macros
// have been removed from this release version.  This header does not 
// provide mappings for deprecated functions.  If the developer wishes
// to make use of this header to aid porting of legacy code, build 
// errors resulting from calls to deprecated functions should be resolved by
// replacing those calls with explicit calls to supported functions.
//
// This header serves as an upgrade aid, rather than as a header for providing
// complete backwards compatibility.

#ifndef SCREAM_LEGACY
#define SCREAM_LEGACY

#define PRIORITY_NO_STEAL							CELL_SCREAM_PRIORITY_NO_STEAL

#define SND_PARAMS_STRUCT							CELL_SCREAM_SND_PARAMS_STRUCT

#define SSS_FLAGS_QUIET								CELL_SCREAM_SSS_FLAGS_QUIET
#define SSS_FLAGS_SILENT							CELL_SCREAM_SSS_FLAGS_SILENT

#define snd_StopSoundSystem							cellScreamStopSoundSystem
#define snd_SystemRunning							cellScreamSystemRunning
#define snd_ReleaseLibWaveChannel					cellScreamReleaseLibWaveChannel
#define snd_ReclaimLibWaveChannel					cellScreamReclaimLibWaveChannel

#define SND_LOAD_ERR_COUDNT_OPEN_FILE				CELL_SCREAM_LOAD_ERR_COUDNT_OPEN_FILE
#define SND_LOAD_ERR_READING_FILE					CELL_SCREAM_LOAD_ERR_READING_FILE
#define SND_LOAD_ERR_MEMORY							CELL_SCREAM_LOAD_ERR_MEMORY
#define SND_LOAD_ERR_SYNTH_MEMORY					CELL_SCREAM_LOAD_ERR_SYNTH_MEMORY
#define SND_LOAD_ERR_IN_PROGRESS					CELL_SCREAM_LOAD_ERR_IN_PROGRESS
#define SND_LOAD_ERR_DMA							CELL_SCREAM_LOAD_ERR_DMA
#define SND_LOAD_ERR_RPC							CELL_SCREAM_LOAD_ERR_RPC
#define SND_LOAD_ERR_INVALID_FORMAT					CELL_SCREAM_LOAD_ERR_INVALID_FORMAT

#define	snd_BankLoadEx								cellScreamBankLoadEx
#define snd_BankLoad								cellScreamBankLoad
#define	snd_BankLoadFromMemEx						cellScreamBankLoadFromMemEx
#define snd_BankLoadFromMem							cellScreamBankLoadFromMem
#define	snd_UnloadBank								cellScreamUnloadBank
#define	snd_ResolveBankXREFS						cellScreamResolveBankXREFS
#define snd_GetLastLoadError						cellScreamGetLastLoadError

#define MASTER_VOLUME_SFX							CELL_SCREAM_MASTER_VOLUME_SFX
#define MASTER_VOLUME_MUSIC							CELL_SCREAM_MASTER_VOLUME_MUSIC
#define MASTER_VOLUME_DIALOG						CELL_SCREAM_MASTER_VOLUME_DIALOG
#define MASTER_VOLUME_USER_1						CELL_SCREAM_MASTER_VOLUME_USER_1
#define MASTER_VOLUME_USER_2						CELL_SCREAM_MASTER_VOLUME_USER_2
#define MASTER_VOLUME_USER_3						CELL_SCREAM_MASTER_VOLUME_USER_3
#define MASTER_VOLUME_USER_4						CELL_SCREAM_MASTER_VOLUME_USER_4
#define MASTER_VOLUME_USER_5						CELL_SCREAM_MASTER_VOLUME_USER_5
#define MASTER_VOLUME_USER_6						CELL_SCREAM_MASTER_VOLUME_USER_6
#define MASTER_VOLUME_USER_7						CELL_SCREAM_MASTER_VOLUME_USER_7
#define MASTER_VOLUME_USER_8						CELL_SCREAM_MASTER_VOLUME_USER_8
#define MASTER_VOLUME_USER_9						CELL_SCREAM_MASTER_VOLUME_USER_9
#define MASTER_VOLUME_USER_10						CELL_SCREAM_MASTER_VOLUME_USER_10
#define MASTER_VOLUME_USER_11						CELL_SCREAM_MASTER_VOLUME_USER_11
#define MASTER_VOLUME_USER_12						CELL_SCREAM_MASTER_VOLUME_USER_12
#define MASTER_VOLUME_EXTERNAL						CELL_SCREAM_MASTER_VOLUME_EXTERNAL
#define MASTER_VOLUME_GLOBAL						CELL_SCREAM_MASTER_VOLUME_GLOBAL
#define MASTER_VOLUME_MAX							CELL_SCREAM_MASTER_VOLUME_MAX

#define snd_SetMasterVolume							cellScreamSetMasterVolume
#define snd_GetMasterVolume							cellScreamGetMasterVolume

#define VOLUME_DUCKER_ONE							CELL_SCREAM_VOLUME_DUCKER_ONE

#define DuckerDef									CellScreamDuckerDef
#define DuckerDefPtr								CellScreamDuckerDefPtr

#define snd_SetMasterVolumeDucker					cellScreamSetMasterVolumeDucker

#define SPEAKER_MODE_MONO							CELL_SCREAM_SPEAKER_MODE_MONO
#define SPEAKER_MODE_STEREO							CELL_SCREAM_SPEAKER_MODE_STEREO
#define SPEAKER_MODE_MATRIX_SURROUND				CELL_SCREAM_SPEAKER_MODE_MATRIX_SURROUND
#define SPEAKER_MODE_SURROUND						CELL_SCREAM_SPEAKER_MODE_SURROUND
#define SPEAKER_MODE_DISCRETE_6CH					CELL_SCREAM_SPEAKER_MODE_DISCRETE_6CH
#define SPEAKER_MODE_DISCRETE_8CH					CELL_SCREAM_SPEAKER_MODE_DISCRETE_8CH
#define SPEAKER_MODE_PHANTOM_CENTER					CELL_SCREAM_SPEAKER_MODE_PHANTOM_CENTER
#define SPEAKER_MODE_PAN_EXCLUDES_CENTER			CELL_SCREAM_SPEAKER_MODE_PAN_EXCLUDES_CENTER

#define snd_SetPlaybackMode							cellScreamSetPlaybackMode
#define snd_GetPlaybackMode							cellScreamGetPlaybackMode

#define	snd_SetGroupVoiceRange						cellScreamSetGroupVoiceRange

#define ORIGINAL_VOLUME								CELL_SCREAM_ORIGINAL_VOLUME
#define CURRENT_VOLUME								CELL_SCREAM_CURRENT_VOLUME
#define ORIGINAL_PAN								CELL_SCREAM_ORIGINAL_PAN
#define CURRENT_PAN									CELL_SCREAM_CURRENT_PAN
#define ORIGINAL_EL									CELL_SCREAM_ORIGINAL_EL
#define CURRENT_EL									CELL_SCREAM_CURRENT_EL
#define ORIGINAL_VOLSCALE_FULLRANGE					CELL_SCREAM_ORIGINAL_VOLSCALE_FULLRANGE
#define CURRENT_VOLSCALE_FULLRANGE					CELL_SCREAM_CURRENT_VOLSCALE_FULLRANGE
#define ORIGINAL_VOLSCALE_LFE						CELL_SCREAM_ORIGINAL_VOLSCALE_LFE
#define CURRENT_VOLSCALE_LFE						CELL_SCREAM_CURRENT_VOLSCALE_LFE
#define ORIGINAL_VOLSCALE_DRY						CELL_SCREAM_ORIGINAL_VOLSCALE_DRY
#define CURRENT_VOLSCALE_DRY						CELL_SCREAM_CURRENT_VOLSCALE_DRY
#define ORIGINAL_VOLSCALE_WET						CELL_SCREAM_ORIGINAL_VOLSCALE_WET
#define CURRENT_VOLSCALE_WET						CELL_SCREAM_CURRENT_VOLSCALE_WET
#define ORIGINAL_VOLSCALE_SENDS						CELL_SCREAM_ORIGINAL_VOLSCALE_SENDS
#define CURRENT_VOLSCALE_SENDS						CELL_SCREAM_CURRENT_VOLSCALE_SENDS

#define SND_HANDLE_STARTING							CELL_SCREAM_HANDLE_STARTING

#define SND_PAN_FL									CELL_SCREAM_PAN_FL
#define SND_PAN_FR									CELL_SCREAM_PAN_FR
#define SND_PAN_SR									CELL_SCREAM_PAN_SR
#define SND_PAN_SL									CELL_SCREAM_PAN_SL
#define SND_PAN_C									CELL_SCREAM_PAN_C
#define SND_PAN_LFE									CELL_SCREAM_PAN_LFE
#define SND_PAN_SBL									CELL_SCREAM_PAN_SBL
#define SND_PAN_SBR									CELL_SCREAM_PAN_SBR

#define SPP_MASK_BANK_BY_PTR						CELL_SCREAM_SPP_MASK_BANK_BY_PTR
#define SPP_MASK_SND_BY_INDEX						CELL_SCREAM_SPP_MASK_SND_BY_INDEX
#define SPP_MASK_VOL								CELL_SCREAM_SPP_MASK_VOL
#define SPP_MASK_PAN								CELL_SCREAM_SPP_MASK_PAN
#define SPP_MASK_AZ									CELL_SCREAM_SPP_MASK_AZ
#define SPP_MASK_PITCHMOD							CELL_SCREAM_SPP_MASK_PITCHMOD
#define SPP_MASK_PITCHBEND							CELL_SCREAM_SPP_MASK_PITCHBEND
#define SPP_MASK_REGISTERS							CELL_SCREAM_SPP_MASK_REGISTERS
#define SPP_MASK_FILTER								CELL_SCREAM_SPP_MASK_FILTER
#define SPP_MASK_EL									CELL_SCREAM_SPP_MASK_EL
#define SPP_MASK_VOLSCALE_FULLRANGE					CELL_SCREAM_SPP_MASK_VOLSCALE_FULLRANGE
#define SPP_MASK_VOLSCALE_LFE						CELL_SCREAM_SPP_MASK_VOLSCALE_LFE
#define SPP_MASK_VOLSCALE_DRY						CELL_SCREAM_SPP_MASK_VOLSCALE_DRY
#define SPP_MASK_VOLSCALE_WET						CELL_SCREAM_SPP_MASK_VOLSCALE_WET
#define SPP_MASK_VOLSCALE_SENDS						CELL_SCREAM_SPP_MASK_VOLSCALE_SENDS
#define SPP_MASK_IS_CHILD							CELL_SCREAM_SPP_MASK_IS_CHILD

#define SndPlayParams								CellScreamSndPlayParams
#define SndPlayParamsPtr							CellScreamSndPlayParamsPtr

#define	snd_PlaySoundEx								cellScreamPlaySoundEx
#define	snd_GetSoundReg								cellScreamGetSoundReg
#define	snd_SetSoundReg								cellScreamSetSoundReg
#define	snd_SetAllSoundReg							cellScreamSetAllSoundReg

#define	snd_GetSFXGlobalReg							cellScreamGetSFXGlobalReg
#define	snd_SetSFXGlobalReg							cellScreamSetSFXGlobalReg

#define	snd_GetSoundUserDataPtr						cellScreamGetSoundUserDataPtr

#define	snd_SoundIsStillPlaying						cellScreamSoundIsStillPlaying
#define	snd_IsSoundALooper							cellScreamIsSoundALooper

#define	snd_PauseSound								cellScreamPauseSound
#define	snd_ContinueSound							cellScreamContinueSound
#define	snd_StopSound								cellScreamStopSound
#define	snd_StopAllSoundsInBank						cellScreamStopAllSoundsInBank
#define	snd_StopAllSounds							cellScreamStopAllSounds

#define PITCH_BEND_MIN								CELL_SCREAM_PITCH_BEND_MIN
#define PITCH_BEND_MAX								CELL_SCREAM_PITCH_BEND_MAX


#define SND_SET_PARAMS_MASK_VOL						CELL_SCREAM_SET_PARAMS_MASK_VOL
#define SND_SET_PARAMS_MASK_PAN						CELL_SCREAM_SET_PARAMS_MASK_PAN
#define SND_SET_PARAMS_MASK_AZ						CELL_SCREAM_SET_PARAMS_MASK_AZ
#define SND_SET_PARAMS_MASK_DOLBY_SAFE_PAN			CELL_SCREAM_SET_PARAMS_MASK_DOLBY_SAFE_PAN
#define SND_SET_PARAMS_MASK_PITCH_MOD				CELL_SCREAM_SET_PARAMS_MASK_PITCH_MOD
#define SND_SET_PARAMS_MASK_PITCH_BEND				CELL_SCREAM_SET_PARAMS_MASK_PITCH_BEND
#define SND_SET_PARAMS_MASK_REGISTERS				CELL_SCREAM_SET_PARAMS_MASK_REGISTERS
#define SND_SET_PARAMS_MASK_FILTER					CELL_SCREAM_SET_PARAMS_MASK_FILTER
#define SND_SET_PARAMS_MASK_EL						CELL_SCREAM_SET_PARAMS_MASK_EL
#define SND_SET_PARAMS_MASK_VOLSCALE_FULLRANGE		CELL_SCREAM_SET_PARAMS_MASK_VOLSCALE_FULLRANGE
#define SND_SET_PARAMS_MASK_VOLSCALE_LFE			CELL_SCREAM_SET_PARAMS_MASK_VOLSCALE_LFE
#define SND_SET_PARAMS_MASK_VOLSCALE_DRY			CELL_SCREAM_SET_PARAMS_MASK_VOLSCALE_DRY
#define SND_SET_PARAMS_MASK_VOLSCALE_WET			CELL_SCREAM_SET_PARAMS_MASK_VOLSCALE_WET
#define SND_SET_PARAMS_MASK_VOLSCALE_SENDS			CELL_SCREAM_SET_PARAMS_MASK_VOLSCALE_SENDS

#define SetSoundParams								CellScreamSetSoundParams
#define SetSoundParamsPtr							CellScreamSetSoundParamsPtr

#define snd_SetSoundParamsEx						cellScreamSetSoundParamsEx

#define PAUSE_GROUP_SFX								CELL_SCREAM_PAUSE_GROUP_SFX
#define PAUSE_GROUP_MUSIC							CELL_SCREAM_PAUSE_GROUP_MUSIC
#define PAUSE_GROUP_DIALOG							CELL_SCREAM_PAUSE_GROUP_DIALOG
#define PAUSE_GROUP_USER_1							CELL_SCREAM_PAUSE_GROUP_USER_1
#define PAUSE_GROUP_USER_2							CELL_SCREAM_PAUSE_GROUP_USER_2
#define PAUSE_GROUP_USER_3							CELL_SCREAM_PAUSE_GROUP_USER_3
#define PAUSE_GROUP_USER_4							CELL_SCREAM_PAUSE_GROUP_USER_4
#define PAUSE_GROUP_USER_5							CELL_SCREAM_PAUSE_GROUP_USER_5
#define PAUSE_GROUP_USER_6							CELL_SCREAM_PAUSE_GROUP_USER_6
#define PAUSE_GROUP_USER_7							CELL_SCREAM_PAUSE_GROUP_USER_7
#define PAUSE_GROUP_USER_8							CELL_SCREAM_PAUSE_GROUP_USER_8
#define PAUSE_GROUP_USER_9							CELL_SCREAM_PAUSE_GROUP_USER_9
#define PAUSE_GROUP_USER_10							CELL_SCREAM_PAUSE_GROUP_USER_10
#define PAUSE_GROUP_USER_11							CELL_SCREAM_PAUSE_GROUP_USER_11
#define PAUSE_GROUP_USER_12							CELL_SCREAM_PAUSE_GROUP_USER_12
#define PAUSE_GROUP_ALL								CELL_SCREAM_PAUSE_GROUP_ALL

#define snd_PauseAllSoundsInGroup					cellScreamPauseAllSoundsInGroup
#define snd_ContinueAllSoundsInGroup				cellScreamContinueAllSoundsInGroup
#define snd_StopAllSoundsInGroup					cellScreamStopAllSoundsInGroup

#define	DELTA_FROM_CURRENT							CELL_SCREAM_DELTA_FROM_CURRENT				
#define	DELTA_FROM_DESTINATION						CELL_SCREAM_DELTA_FROM_DESTINATION			
#define	DELTA_TIMED_CHANGE							CELL_SCREAM_DELTA_TIMED_CHANGE				
#define MAX_DOLBY_PAN_RATE	  						CELL_SCREAM_MAX_DOLBY_PAN_RATE	  			
#define AUTO_VOLUME_OFF_AND_STOP					CELL_SCREAM_AUTO_VOLUME_OFF_AND_STOP		

#define snd_AutoVol									cellScreamAutoVol
#define snd_AutoPan									cellScreamAutoPan
#define snd_AutoPitch								cellScreamAutoPitch
#define snd_AutoPitchBend							cellScreamAutoPitchBend

#define snd_GetDopplerPitchMod						cellScreamGetDopplerPitchMod

#define snd_GetTick									cellScreamGetTick

#define VS_FREE										CELL_SCREAM_VS_FREE			
#define VS_PLAYING									CELL_SCREAM_VS_PLAYING		
#define VS_ALLOCATED								CELL_SCREAM_VS_ALLOCATED	
#define VS_EXTERN									CELL_SCREAM_VS_EXTERN		
#define VS_STREAM									CELL_SCREAM_VS_STREAM		
#define snd_GetVoiceStatus							cellScreamGetVoiceStatus

#define snd_LockVoiceAllocatorEx					cellScreamLockVoiceAllocatorEx

#define snd_ExternVoiceAlloc						cellScreamExternVoiceAlloc
#define snd_ExternVoiceFree							cellScreamExternVoiceFree
#define snd_UnlockVoiceAllocator					cellScreamUnlockVoiceAllocator

#define ExternSndMemAlloc								CELL_SCREAM_MEMALLOC_FUNC
#define SND_MEM_USE_BANK_HEADER							CELL_SCREAM_MEM_USE_BANK_HEADER							
#define SND_MEM_USE_BANK_HEADER_WITH_EMBEDDED_MMD		CELL_SCREAM_MEM_USE_BANK_HEADER_WITH_EMBEDDED_MMD		
#define SND_MEM_USE_MMD									CELL_SCREAM_MEM_USE_MMD									
#define SND_MEM_USE_TEMP_SCRATCH_MEMORY					CELL_SCREAM_MEM_USE_TEMP_SCRATCH_MEMORY					
#define SND_MEM_USE_BANK_BODY							CELL_SCREAM_MEM_USE_BANK_BODY							
#define SND_MEM_USE_BANK								CELL_SCREAM_MEM_USE_BANK								
#define SND_MEM_USE_ALIGN16								CELL_SCREAM_MEM_USE_ALIGN16								

#define ExternSndMemFree							CELL_SCREAM_MEMFREE_FUNC
#define snd_RegisterMainMemAllocator				cellScreamRegisterMainMemAllocator


//
// From original snd_data.h header
//

#define FLT_BQ_OFF									CELL_SCREAM_FLT_BQ_OFF		
#define FLT_BQ_LPF									CELL_SCREAM_FLT_BQ_LPF		
#define FLT_BQ_HPF									CELL_SCREAM_FLT_BQ_HPF		
#define FLT_BQ_APF									CELL_SCREAM_FLT_BQ_APF		
#define FLT_BQ_BPF									CELL_SCREAM_FLT_BQ_BPF		
#define FLT_BQ_NOTCH								CELL_SCREAM_FLT_BQ_NOTCH	
#define FLT_BQ_PEQ									CELL_SCREAM_FLT_BQ_PEQ		
#define FLT_BQ_LSH									CELL_SCREAM_FLT_BQ_LSH		
#define FLT_BQ_HSH 									CELL_SCREAM_FLT_BQ_HSH 		

#define BQ_MIN_GAIN									CELL_SCREAM_BQ_MIN_GAIN
#define BQ_MAX_GAIN									CELL_SCREAM_BQ_MAX_GAIN
#define BQ_MAX_EEQ									CELL_SCREAM_BQ_MAX_EEQ
#define BQ_MAX_SHELFQ								CELL_SCREAM_BQ_MAX_SHELFQ

#define SndIIRFilterParams							CellScreamIIRFilterParams
#define SndBussRoutingMatrix						CellScreamBussRoutingMatrix

#define NUM_BUSS_SENDS								CELL_SCREAM_NUM_BUSS_SENDS	

#define SoundBank									CellScreamSoundBank
#define SoundBankPtr								CellScreamSoundBankPtr

#endif // ndef CELL_SCREAM_LEGACY
