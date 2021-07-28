/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001 
* Copyright (C) 2009 Sony Computer Entertainment Inc.
* All Rights Reserved.
*/
/* scream.h SCREAM header */
#ifndef _SCREAM_H
#define _SCREAM_H

#if (defined(_cplusplus) || defined(__cplusplus))
extern "C"
{
#endif


/*****************************************************************************
	Basic data types
*****************************************************************************/
#include <sys/integertypes.h>			// From CELL SDK


#if (!defined(_cplusplus) && !defined(__cplusplus))
#define bool int
#ifndef false
	#define false (0)
#endif
#ifndef true
	#define true (1)
#endif
#endif // (!defined(_cplusplus) && !defined(__cplusplus))




/*****************************************************************************
	System
*****************************************************************************/

typedef void* (*CELL_SCREAM_MEMALLOC_FUNC)(int32_t bytes, int32_t use, int32_t *act_bytes);
typedef void  (*CELL_SCREAM_MEMFREE_FUNC)(void *mem);

/*****************************************************************************
	MultiStream functions and definitions for SCREAM
*****************************************************************************/
// Scream/MultiStream error codes
#define		CELL_SCREAM_ERROR_NONE						0
#define		CELL_SCREAM_ERROR_NULL_PARAM				-1
#define		CELL_SCREAM_ERROR_SYSTEM_ALREADY_STARTED	-2
#define		CELL_SCREAM_ERROR_BAD_RETURNS				-3
#define		CELL_SCREAM_ERROR_SYSTEM_NOT_RUNNING		-4
#define		CELL_SCREAM_ERROR_BAD_FLAGS					-5
#define		CELL_SCREAM_ERROR_BAD_MS_BUS				-6

/**
 * @brief
 * Structure to hold DSP handles for MultiStream DSP effects that SCREAM requires.
 * 
 * Structure to hold DSP handles for MultiStream DSP effects that SCREAM requires.
 * 
 * These handles must be obtained by loading DSP effects using multistream.  If
 * an effect is not required by SCREAM (for example, if distortion is not used)
 * then -1 can be passed as the handle value.  Not that disabling an effect
 * in this way disables it for all voices.
 * 
 * @sa
 * <code>CellScreamInit</code>
 * @sa
 * MultiStream documentation relating to effects and DSPs
 */
typedef struct _CellScreamMSDSPHandles
{
	int peq;													///< Handle to parametric EQ effect
	int distortion;												///< Handle to distortion effect
	int auxSend;												///< Handle to auxilliary send plugin
	int filter;													///< Handle to post send filter
} CellScreamMSDSPHandles;



/**
 * @brief
 * Structure to hold data required for SCREAM initialisation.
 * 
 * Structure to hold data required for SCREAM initialisation.
 * 
 * This structure is required when calling the <code>CellStartSoundSystem</code)
 * function and contains all the information needed to start SCREAM.
 *
 * SCREAM uses callback functions for memory allocation and release.  These are
 * not optional as SCREAM provides no defaults, therefore pfMalloc and pfFree
 * must refer to valid functions.
 *
 * SCREAM uses a send/return routing structure as shown in the Overview documentation.
 * This is implemented using MulitStream auxilliary send and return plugins.  Handles to
 * these effects must be passed in the dspHandles struture.  The 8 sends must also know
 * the return address of the auxilliary return plugins and the channels to which they should route.
 * This is covered in the MultiStream documentation.  It is only necessary to specify return
 * addresses for channels that are used, otherwise NULL pointers can be specified.
 *
 * If ppAuxRetAddrs or pAuxRetChanls is NULL then the sends are disabled.
 * 
 * @sa
 * <code>CellScreamStartSoundSystem</code>
 * @sa
 * <code>CellScreamMSDSPHandles</code>
 *
 * MultiStream documentation relating to busses and effects
 * @sa
 * MultiStream documentation covering axuilliary send and returns.
  */
typedef struct _CellScreamInit
{
	int priority;												///< Priority level of the SCREAM PPU thread
	CELL_SCREAM_MEMALLOC_FUNC pfMalloc;							///< Function pointer to custom malloc function
	CELL_SCREAM_MEMFREE_FUNC pfFree;							///< Function pointer to custom free function
	int	subBus;													///< MultiStream bus which SCREAM will use, should generally be a time domain bus
	CellScreamMSDSPHandles	dspHandles;							///< Handles to MultiStream effects which SCREAM will use
	void ** ppAuxRetAddrs;										///< Pointer to array of 8 Aux Return DSP addresses
	int * pAuxRetChnls;											///< Pointer to array of 8 Aux Return Channels (one per address)
	int msFlags;												///< MultiStream flags used during MultiStream configuration
} CellScreamInit;

/**
 * @brief
 * Synchronises SCREAM to MultiStream.
 * 
 * Synchronises SCREAM to MultiStream.
 * 
 * SCREAM timing relies on MultiStream timing.  Whenever MultiStream is triggered using
 * cellMSSystemSignalSPU, this function should be called to maintain synchronisation.
 * 
 * @return
 * None.
 *
 * @sa
 * MultiStream function: cellMSSystemSignalSPU
 */
void cellScreamMSSync( void );

uint32_t	cellScreamGetTick(void);
void cellScreamRegisterMainMemAllocator(CELL_SCREAM_MEMALLOC_FUNC AllocProc, CELL_SCREAM_MEMFREE_FUNC FreeProc);
extern CELL_SCREAM_MEMALLOC_FUNC g_AllocProc;
extern CELL_SCREAM_MEMFREE_FUNC  g_FreeProc;


/*****************************************************************************
	Initialization
*****************************************************************************/

// Initialization flags for use with cellScreamStartSoundSystemEx
#define CELL_SCREAM_SSS_FLAGS_QUIET							(1L<< 0)	// no messages for missing named sounds
#define CELL_SCREAM_SSS_FLAGS_SILENT						(1L<< 1)	// no messages at all (other then version banner shown on startup)

int32_t cellScreamStartSoundSystem( CellScreamInit* init );
int32_t	cellScreamStopSoundSystem(void);
int32_t	cellScreamSystemRunning(void);

#define CELL_SCREAM_DEFAULT_LFO_UPDATE			2			// rate at which SCREAM's LFOs will be updated. 
													// supply to cellScreamStartSoundSystemEx. 0 means NO LFOs!
													// otherwise, every x ticks the LFOs will be updated.
// these are SCREAM specific...
#define CELL_SCREAM_LOAD_ERR_COUDNT_OPEN_FILE	0x100
#define CELL_SCREAM_LOAD_ERR_READING_FILE		0x101
#define CELL_SCREAM_LOAD_ERR_MEMORY				0x102

#define CELL_SCREAM_LOAD_ERR_SYNTH_MEMORY		0x103
#define CELL_SCREAM_LOAD_ERR_IN_PROGRESS		0x104
#define CELL_SCREAM_LOAD_ERR_DMA				0x105
#define CELL_SCREAM_LOAD_ERR_RPC				0x106

#define CELL_SCREAM_LOAD_ERR_INVALID_FORMAT		0x107



/*****************************************************************************
	SPEAKER MODES
*****************************************************************************/

#define CELL_SCREAM_SPEAKER_MODE_MONO				(1<<0)
#define CELL_SCREAM_SPEAKER_MODE_STEREO				(1<<1)
#define CELL_SCREAM_SPEAKER_MODE_MATRIX_SURROUND	(1<<2) 
#define CELL_SCREAM_SPEAKER_MODE_SURROUND			CELL_SCREAM_SPEAKER_MODE_MATRIX_SURROUND // kept for compatibility - use SPEAKER_MDOE_MATRIX_SURROUND instead
#define CELL_SCREAM_SPEAKER_MODE_DISCRETE_6CH		(1<<3)
#define CELL_SCREAM_SPEAKER_MODE_DISCRETE_8CH		(1<<4)

// These modes can be OR'd with the modes above, but not all combinations
// will have an effect (STEREO | PHANTOM_CENTER won't do anything different
// from STEREO, for instance).
#define CELL_SCREAM_SPEAKER_MODE_PHANTOM_CENTER       (1<<8)  // Load a pan table with a phantom center channel. Panned sounds and
                                                  // grains flagged 'send center' will be sent to the front left and right
                                                  // instead. DISCRETE_6CH | PHANTOM_CENTER gives you a 5.1 channel layout
                                                  // (3 at the front, 2 at the sides) but will simulate the center channel
                                                  // using the front left and right channels.
#define CELL_SCREAM_SPEAKER_MODE_PAN_EXCLUDES_CENTER  (1<<9)  // Panned sounds will be rendered with a phantom center channel, but
                                                  // 'send center' grains will still go to the center channel. This is
                                                  // useful, for example, if you want to keep your center channel for
                                                  // dialog or interface sounds only (for example).

void cellScreamSetPlaybackMode(int32_t mode);
int32_t  cellScreamGetPlaybackMode(void);

/*****************************************************************************
	Data Handling
*****************************************************************************/
struct CellScreamSoundBank *cellScreamBankLoadEx(int8_t *name, int32_t offset, uint32_t spu_mem_loc, uint32_t spu_mem_size, bool no_check);
struct CellScreamSoundBank *cellScreamBankLoadFromMemEx(void *loc, uint32_t spu_mem_loc, uint32_t spu_mem_size, bool no_check);
void   cellScreamResolveBankXREFS(void);
void   cellScreamUnloadBank(struct CellScreamSoundBank *bank);
int32_t    cellScreamGetLastLoadError(void);

#define CELL_SCREAM_VOLUME_DUCKER_ONE (1L<<16)

// Volume group definitions.
#define CELL_SCREAM_MASTER_VOLUME_SFX		0
#define CELL_SCREAM_MASTER_VOLUME_MUSIC 	1
#define CELL_SCREAM_MASTER_VOLUME_DIALOG	2
#define CELL_SCREAM_MASTER_VOLUME_USER_1	3
#define CELL_SCREAM_MASTER_VOLUME_USER_2	4
#define CELL_SCREAM_MASTER_VOLUME_USER_3	5
#define CELL_SCREAM_MASTER_VOLUME_USER_4	6
#define CELL_SCREAM_MASTER_VOLUME_USER_5	7
#define CELL_SCREAM_MASTER_VOLUME_USER_6	8
#define CELL_SCREAM_MASTER_VOLUME_USER_7	9
#define CELL_SCREAM_MASTER_VOLUME_USER_8	10
#define CELL_SCREAM_MASTER_VOLUME_USER_9	11
#define CELL_SCREAM_MASTER_VOLUME_USER_10	12
#define CELL_SCREAM_MASTER_VOLUME_USER_11	13
#define CELL_SCREAM_MASTER_VOLUME_USER_12	14
#define CELL_SCREAM_MASTER_VOLUME_EXTERNAL	15
#define CELL_SCREAM_MASTER_VOLUME_GLOBAL	16
#define CELL_SCREAM_MASTER_VOLUME_MAX		1024

#define CELL_SCREAM_PAUSE_GROUP_SFX		(1L	<< CELL_SCREAM_MASTER_VOLUME_SFX)
#define CELL_SCREAM_PAUSE_GROUP_MUSIC 	(1L << CELL_SCREAM_MASTER_VOLUME_MUSIC)
#define CELL_SCREAM_PAUSE_GROUP_DIALOG	(1L << CELL_SCREAM_MASTER_VOLUME_DIALOG)
#define CELL_SCREAM_PAUSE_GROUP_USER_1	(1L << CELL_SCREAM_MASTER_VOLUME_USER_1)
#define CELL_SCREAM_PAUSE_GROUP_USER_2	(1L << CELL_SCREAM_MASTER_VOLUME_USER_2)
#define CELL_SCREAM_PAUSE_GROUP_USER_3	(1L << CELL_SCREAM_MASTER_VOLUME_USER_3)
#define CELL_SCREAM_PAUSE_GROUP_USER_4	(1L << CELL_SCREAM_MASTER_VOLUME_USER_4)
#define CELL_SCREAM_PAUSE_GROUP_USER_5	(1L << CELL_SCREAM_MASTER_VOLUME_USER_5)
#define CELL_SCREAM_PAUSE_GROUP_USER_6	(1L << CELL_SCREAM_MASTER_VOLUME_USER_6)
#define CELL_SCREAM_PAUSE_GROUP_USER_7	(1L << CELL_SCREAM_MASTER_VOLUME_USER_7)
#define CELL_SCREAM_PAUSE_GROUP_USER_8	(1L << CELL_SCREAM_MASTER_VOLUME_USER_8)
#define CELL_SCREAM_PAUSE_GROUP_USER_9	(1L << CELL_SCREAM_MASTER_VOLUME_USER_9)
#define CELL_SCREAM_PAUSE_GROUP_USER_10	(1L << CELL_SCREAM_MASTER_VOLUME_USER_10)
#define CELL_SCREAM_PAUSE_GROUP_USER_11	(1L << CELL_SCREAM_MASTER_VOLUME_USER_11)
#define CELL_SCREAM_PAUSE_GROUP_USER_12	(1L << CELL_SCREAM_MASTER_VOLUME_USER_12)
#define CELL_SCREAM_PAUSE_GROUP_ALL		(0xFFFFFFFF)

/*****************************************************************************
	System Configuration
*****************************************************************************/
typedef struct CellScreamDuckerDef
{
	int32_t		source_group; 		// as index
	uint32_t	target_groups; 		// as flags
	int32_t		full_duck_mult; 	// 16 bit fixed point
	int32_t 	attack_time;		// 16 bit fixed point
	int32_t 	release_time;  		// 16 bit fixed point
	int32_t		current_duck_mult; 	// 16 but fixed point
} CellScreamDuckerDef, *CellScreamDuckerDefPtr;

void cellScreamSetMasterVolume(int32_t which, int32_t vol);
int32_t  cellScreamGetMasterVolume(int32_t which);
void cellScreamSetMasterVolumeDucker(int32_t which, CellScreamDuckerDefPtr state); // there are 4 duckers so which can be 0 - 3, state == NULL will clear out the ducker!
void cellScreamSetGroupVoiceRange(int32_t group, int32_t min, int32_t max);


/*****************************************************************************
	Playing, Stopping and Modifying Sounds
*****************************************************************************/

// Used in mask member of SetSoundParams struct.
#define CELL_SCREAM_SET_PARAMS_MASK_VOL					(1L<<0)
#define CELL_SCREAM_SET_PARAMS_MASK_PAN					(1L<<1)
#define CELL_SCREAM_SET_PARAMS_MASK_AZ					(1L<<1)
#define CELL_SCREAM_SET_PARAMS_MASK_DOLBY_SAFE_PAN		(1L<<2)
#define CELL_SCREAM_SET_PARAMS_MASK_PITCH_MOD			(1L<<3)
#define CELL_SCREAM_SET_PARAMS_MASK_PITCH_BEND			(1L<<4)
#define CELL_SCREAM_SET_PARAMS_MASK_REGISTERS			(1L<<5)
#define CELL_SCREAM_SET_PARAMS_MASK_FILTER				(1L<<6)
#define CELL_SCREAM_SET_PARAMS_MASK_EL					(1L<<8)
#define CELL_SCREAM_SET_PARAMS_MASK_VOLSCALE_FULLRANGE	(1L<<9)
#define CELL_SCREAM_SET_PARAMS_MASK_VOLSCALE_LFE		(1L<<10)
#define CELL_SCREAM_SET_PARAMS_MASK_VOLSCALE_DRY		(1L<<11)
#define CELL_SCREAM_SET_PARAMS_MASK_VOLSCALE_WET		(1L<<12)
#define CELL_SCREAM_SET_PARAMS_MASK_VOLSCALE_SENDS		(1L<<13)

#define CELL_SCREAM_SPP_MASK_VOL				CELL_SCREAM_SET_PARAMS_MASK_VOL
#define CELL_SCREAM_SPP_MASK_PAN				CELL_SCREAM_SET_PARAMS_MASK_AZ
#define CELL_SCREAM_SPP_MASK_AZ					CELL_SCREAM_SET_PARAMS_MASK_AZ
#define CELL_SCREAM_SPP_MASK_PITCHMOD			CELL_SCREAM_SET_PARAMS_MASK_PITCH_MOD
#define CELL_SCREAM_SPP_MASK_PITCHBEND			CELL_SCREAM_SET_PARAMS_MASK_PITCH_BEND
#define CELL_SCREAM_SPP_MASK_REGISTERS			CELL_SCREAM_SET_PARAMS_MASK_REGISTERS
#define CELL_SCREAM_SPP_MASK_FILTER				CELL_SCREAM_SET_PARAMS_MASK_FILTER
#define CELL_SCREAM_SPP_MASK_EL					CELL_SCREAM_SET_PARAMS_MASK_EL
#define CELL_SCREAM_SPP_MASK_VOLSCALE_FULLRANGE	CELL_SCREAM_SET_PARAMS_MASK_VOLSCALE_FULLRANGE
#define CELL_SCREAM_SPP_MASK_VOLSCALE_LFE		CELL_SCREAM_SET_PARAMS_MASK_VOLSCALE_LFE
#define CELL_SCREAM_SPP_MASK_VOLSCALE_DRY    	CELL_SCREAM_SET_PARAMS_MASK_VOLSCALE_DRY
#define CELL_SCREAM_SPP_MASK_VOLSCALE_WET		CELL_SCREAM_SET_PARAMS_MASK_VOLSCALE_WET
#define CELL_SCREAM_SPP_MASK_VOLSCALE_SENDS		CELL_SCREAM_SET_PARAMS_MASK_VOLSCALE_SENDS

#define CELL_SCREAM_SPP_MASK_BANK_BY_PTR		((uint32_t)(1L<<29))
#define CELL_SCREAM_SPP_MASK_SND_BY_INDEX		((uint32_t)(1L<<30))
#define CELL_SCREAM_SPP_MASK_IS_CHILD			((uint32_t)(1L<<31))

#define CELL_SCREAM_ORIGINAL_VOLUME             1024	
#define CELL_SCREAM_CURRENT_VOLUME              0x7FFFFFFF
#define CELL_SCREAM_ORIGINAL_PAN                (-1)
#define CELL_SCREAM_CURRENT_PAN                 (-2)
#define CELL_SCREAM_ORIGINAL_EL                 (-1)
#define CELL_SCREAM_CURRENT_EL                  (-2)
#define CELL_SCREAM_ORIGINAL_VOLSCALE_FULLRANGE (-1)
#define CELL_SCREAM_CURRENT_VOLSCALE_FULLRANGE  (-2)
#define CELL_SCREAM_ORIGINAL_VOLSCALE_LFE       (-1)
#define CELL_SCREAM_CURRENT_VOLSCALE_LFE        (-2)
#define CELL_SCREAM_ORIGINAL_BUSS_SENDS         (-1.0f) // legacy
#define CELL_SCREAM_CURRENT_BUSS_SENDS          (-2.0f) // legacy
#define CELL_SCREAM_ORIGINAL_VOLSCALE_DRY       (-1.0f)
#define CELL_SCREAM_CURRENT_VOLSCALE_DRY        (-2.0f)
#define CELL_SCREAM_ORIGINAL_VOLSCALE_WET       (-1.0f)
#define CELL_SCREAM_CURRENT_VOLSCALE_WET        (-2.0f)
#define CELL_SCREAM_ORIGINAL_VOLSCALE_SENDS     (-1.0f)
#define CELL_SCREAM_CURRENT_VOLSCALE_SENDS      (-2.0f)

#define CELL_SCREAM_HANDLE_STARTING 0xFFFFFFFF

#define CELL_SCREAM_PITCH_BEND_MIN	-32768
#define CELL_SCREAM_PITCH_BEND_MAX	32767	

// automated effects
#define	CELL_SCREAM_DELTA_FROM_CURRENT				2
#define	CELL_SCREAM_DELTA_FROM_DESTINATION			3
#define	CELL_SCREAM_DELTA_TIMED_CHANGE				4
#define CELL_SCREAM_MAX_DOLBY_PAN_RATE	  			30
#define CELL_SCREAM_AUTO_VOLUME_OFF_AND_STOP		-4

#define CELL_SCREAM_PAN_FL	(-20000)
#define CELL_SCREAM_PAN_FR	(-20001)
#define CELL_SCREAM_PAN_SR	(-20002)
#define CELL_SCREAM_PAN_SL	(-20003)
#define CELL_SCREAM_PAN_C	(-20004)
#define CELL_SCREAM_PAN_LFE	(-20005)
#define CELL_SCREAM_PAN_SBL	(-20006)
#define CELL_SCREAM_PAN_SBR	(-20007)

#define CELL_SCREAM_FLT_BQ_OFF		0 	// Bypass (transparent) 
#define CELL_SCREAM_FLT_BQ_LPF		1	// Low pass				
#define CELL_SCREAM_FLT_BQ_HPF		2	// High pass            
#define CELL_SCREAM_FLT_BQ_APF		3	// All pass             
#define CELL_SCREAM_FLT_BQ_BPF		4	// Band pass            
#define CELL_SCREAM_FLT_BQ_NOTCH	5	// Notch                
#define CELL_SCREAM_FLT_BQ_PEQ		6	// Peaking EQ           
#define CELL_SCREAM_FLT_BQ_LSH		7	// Low shelf            
#define CELL_SCREAM_FLT_BQ_HSH 		8	// High shelf           

#define CELL_SCREAM_BQ_MIN_GAIN				0.0f	// max gain when peaking or shelving
#define CELL_SCREAM_BQ_MAX_GAIN				32.0f	// max gain when peaking or shelving
#define CELL_SCREAM_BQ_MAX_EEQ				32.0f	// max Q in traditional EE definition
#define CELL_SCREAM_BQ_MAX_SHELFQ			1.0f	// max shelf slope factor of biquad default (12dB/oct)

typedef struct CellScreamIIRFilterParams
{
	int32_t		type;
	float		gain;
	float 	cutoff;
	float 	resonance;
} CellScreamIIRFilterParams;

#define CELL_SCREAM_NUM_BUSS_SENDS	8
typedef struct CellScreamBussRoutingMatrix
{
	float		wet_master;
	float		scale[CELL_SCREAM_NUM_BUSS_SENDS];
	float		dry_master;
} CellScreamBussRoutingMatrix;


#define CELL_SCREAM_SND_PARAMS_STRUCT \
	                      \
	int32_t						vol; \
	int32_t						pan; \
	int16_t						pitch_mod; \
	int16_t						pitch_bend; \
	int8_t						reg[4]; \
	uint32_t					mask; \
	CellScreamIIRFilterParams	filter; \
	CellScreamBussRoutingMatrix aux_buss_sends; \
	uint32_t					size; \
	int32_t						el; \
	int16_t						volscale_fullrange; \
	int16_t						volscale_lfe

typedef struct CellScreamSndPlayParams
{

	CELL_SCREAM_SND_PARAMS_STRUCT;

	union bank_tag
	{
		char             name[8];
		struct CellScreamSoundBank     *ptr;
	} bank_spec;

    union snd_tag
    {
		char             name[16];
		int16_t          index;
	} snd_spec;

} CellScreamSndPlayParams, *CellScreamSndPlayParamsPtr;

typedef struct CellScreamSetSoundParams
{

	CELL_SCREAM_SND_PARAMS_STRUCT;

} CellScreamSetSoundParams, *CellScreamSetSoundParamsPtr;

uint32_t  cellScreamPlaySoundEx(CellScreamSndPlayParamsPtr params);
uint32_t  cellScreamSoundIsStillPlaying(uint32_t handle);
int32_t  cellScreamIsSoundALooper(struct CellScreamSoundBank *bank, int32_t sound);

uint32_t  cellScreamSetSoundParamsEx(uint32_t handle, CellScreamSetSoundParamsPtr params); 

int8_t   cellScreamGetSoundReg(uint32_t handle, int32_t which);
void cellScreamSetSoundReg(uint32_t handle, int32_t which, int8_t val);
int8_t   cellScreamGetSFXGlobalReg(int32_t which);
void cellScreamSetSFXGlobalReg(int32_t which, int8_t val);
uint32_t* cellScreamGetSoundUserDataPtr(struct CellScreamSoundBank *bank, char *bank_name, int32_t sound_index, char *sound_name);

void cellScreamPauseSound(uint32_t handle);
void cellScreamPauseAllSoundsInGroup(uint32_t groups);
void cellScreamContinueSound(uint32_t handle);
void cellScreamContinueAllSoundsInGroup(uint32_t groups);
void cellScreamStopSound(uint32_t handle);
void cellScreamStopAllSoundsInGroup(uint32_t groups);
void cellScreamStopAllSoundsInBank(struct CellScreamSoundBank *bank, int32_t silence);
void cellScreamStopAllSounds(void);

// Automated Effects
void cellScreamAutoVol(uint32_t handle, int32_t vol, int32_t delta_time,	int32_t delta_from);
void cellScreamAutoPan(uint32_t handle, int32_t pan, int32_t dir, int32_t delta_time,	int32_t delta_from);
void cellScreamAutoPitch(uint32_t handle, int32_t pitch, int32_t delta_time, int32_t delta_from);
void cellScreamAutoPitchBend(uint32_t handle, int32_t pb, int32_t delta_time, int32_t delta_from);

// possible return values from cellScreamGetVoiceStatus...
#define CELL_SCREAM_VS_FREE			0
#define CELL_SCREAM_VS_PLAYING		1
#define CELL_SCREAM_VS_ALLOCATED	2
#define CELL_SCREAM_VS_EXTERN		3
#define CELL_SCREAM_PRIORITY_NO_STEAL	127

// These constants tell the allocator function what the requested memory will be
// used for...
#define CELL_SCREAM_MEM_USE_BANK_HEADER							1
#define CELL_SCREAM_MEM_USE_BANK_HEADER_WITH_EMBEDDED_MMD		2
#define CELL_SCREAM_MEM_USE_MMD									3
#define CELL_SCREAM_MEM_USE_TEMP_SCRATCH_MEMORY					8
#define CELL_SCREAM_MEM_USE_BANK_BODY							10
#define CELL_SCREAM_MEM_USE_BANK								11

 
/*****************************************************************************
	Voice Allocator Interface
*****************************************************************************/
int32_t  cellScreamGetVoiceStatus(int32_t voice);
int32_t  cellScreamLockVoiceAllocatorEx(bool block, uint32_t ownerID);
void cellScreamUnlockVoiceAllocator(void);
int32_t  cellScreamExternVoiceAlloc(int32_t vol_group, int32_t priority);
void cellScreamExternVoiceFree(int32_t voice);


/*****************************************************************************
	Utility Functions
*****************************************************************************/
int32_t  cellScreamGetDopplerPitchMod(int32_t approaching_mph);

#if (defined(_cplusplus) || defined(__cplusplus))
}  //extern "C"
#endif
////////////////////////////////////////////////////////////////////////////////


// Helper function macros for data manipulation.
#define cellScreamBankLoad(x,y)      cellScreamBankLoadEx((x),(y),0,0,false)
#define cellScreamBankLoadFromMem(x) cellScreamBankLoadFromMemEx((x),0,0,false);

////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////

#endif
