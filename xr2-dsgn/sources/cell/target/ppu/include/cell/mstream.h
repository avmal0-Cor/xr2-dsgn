/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001 
* Copyright (C) 2009 Sony Computer Entertainment Inc.
* All Rights Reserved.
*/
/* mstream.h Multistream header */

#ifndef _MS_HEADER_H_
#define _MS_HEADER_H_

//Cell only defines
#ifdef __CELLOS_LV2__
#include <cell/audio.h>
#include <cell/spurs/types.h>
#endif


/**********************************************************************************************************
D E F I N E S
**********************************************************************************************************/

/** Version of MultiStream.	*/
#define CELL_MS_VERSION	            (2.80) 

/** Maximum value for the number of streams allowed. Use in code to ensure values are within appropriate range. */
#define CELL_MS_MAX_STREAMS	    (512)	    

/** Maximum value for the number of channels allowed. Use in code to ensure values are within appropriate range. */
#define CELL_MS_MAX_CHANNEL         (7)         

/** Size (in bytes) of a single DSP page. All SPU DSP effects (.PIC files) have to fit
* into a page. Multiple pages can be used if more RAM is required. See the CellMSSystemConfig
* structure documentation for more information on the dspPageCount parameter. */
#define CELL_MS_DSP_PAGESIZE        (12288)

/** Indicates that points on an EQ graph are connected by a cubic exponential line.  Used in CellEQSection structure. */
#define CELL_MS_EQ_CUBIC	    (2)
/** Indicates that points on an EQ graph are connected by a curve. Used in CellEQSection structure. */	        
#define CELL_MS_EQ_CURVE            (1)
/** Indicates that points on an EQ graph are connected by straight lines. Used in CellEQSection structure. */        
#define CELL_MS_EQ_LINE	            (0)   
/** Specifies maximum value for the number of FFT EQ bands. Use in 
* code to ensure values are within appropriate range.	*/
#define CELL_MS_EQ_MAX_BANDS        (512)


/** Flags that can be passed into cellMSSystemConfigureSysUtilEx(unsigned int _flags)
* Title supports LPCM / DOLBY (CELL_MS_AUDIOMODESELECT_SUPPORTSLPCM MUST be one of 
* the flags passed to cellMSSystemConfigureSysUtilEx since LPCM support is a TRC requirement!) */
#define CELL_MS_AUDIOMODESELECT_SUPPORTSLPCM	(1)
#define CELL_MS_AUDIOMODESELECT_SUPPORTSDOLBY	(2)
#define CELL_MS_AUDIOMODESELECT_SUPPORTSDTS	(4)
#define CELL_MS_AUDIOMODESELECT_PREFERDOLBY	(8)
#define CELL_MS_AUDIOMODESELECT_PREFERDTS	(16)


/** Indicates that stream requires more data. */
#define CELL_MS_CALLBACK_MOREDATA     (1)	    

/** Indicates that the stream is closed if the auto-close flag was selected 
* (see the CellMSInfo structure for additional information). */
#define CELL_MS_CALLBACK_CLOSESTREAM  (2)	    

/** Indicates that the dry processing is finished for the stream but wet processing
* still continues. For example, if the stream uses a reverb DSP effect, the user 
* may require the reverb to continue naturally, even though the actual stream 
* data finished playing. */
#define CELL_MS_CALLBACK_FINISHEDDRY  (3)

/** Indicates that processing is complete for both wet and dry streams. */	    
#define CELL_MS_CALLBACK_FINISHSTREAM (4)	    

/** Indicates there has been an error decoding the MP3 for this stream. */
#define CELL_MS_CALLBACK_MP3DECODEERROR (5)	 

/** Specifies that the function requires access to a bus rather than a stream. 
* Used in cellMSCore* functions. The channel number is or'd with this constant 
* to access a desired bus. */
#define CELL_MS_BUS_FLAG        (0x80000)

/** Causes all streams to stop. */     
#define CELL_MS_MASTER_BUS	(CELL_MS_BUS_FLAG | 0)
#define CELL_MS_SUBBUS_1	(CELL_MS_BUS_FLAG | 1)
#define CELL_MS_SUBBUS_2	(CELL_MS_BUS_FLAG | 2)
#define CELL_MS_SUBBUS_3	(CELL_MS_BUS_FLAG | 3) 
#define CELL_MS_SUBBUS_4	(CELL_MS_BUS_FLAG | 4)
#define CELL_MS_SUBBUS_5	(CELL_MS_BUS_FLAG | 5) 
#define CELL_MS_SUBBUS_6	(CELL_MS_BUS_FLAG | 6) 
#define CELL_MS_SUBBUS_7	(CELL_MS_BUS_FLAG | 7) 
#define CELL_MS_SUBBUS_8	(CELL_MS_BUS_FLAG | 8) 
#define CELL_MS_SUBBUS_9	(CELL_MS_BUS_FLAG | 9) 
#define CELL_MS_SUBBUS_10	(CELL_MS_BUS_FLAG | 10)
#define CELL_MS_SUBBUS_11	(CELL_MS_BUS_FLAG | 11)
#define CELL_MS_SUBBUS_12	(CELL_MS_BUS_FLAG | 12)
#define CELL_MS_SUBBUS_13	(CELL_MS_BUS_FLAG | 13)
#define CELL_MS_SUBBUS_14	(CELL_MS_BUS_FLAG | 14)
#define CELL_MS_SUBBUS_15	(CELL_MS_BUS_FLAG | 15)
#define CELL_MS_SUBBUS_16	(CELL_MS_BUS_FLAG | 16)
#define CELL_MS_SUBBUS_17	(CELL_MS_BUS_FLAG | 17)
#define CELL_MS_SUBBUS_18	(CELL_MS_BUS_FLAG | 18)
#define CELL_MS_SUBBUS_19	(CELL_MS_BUS_FLAG | 19)
#define CELL_MS_SUBBUS_20	(CELL_MS_BUS_FLAG | 20)
#define CELL_MS_SUBBUS_21	(CELL_MS_BUS_FLAG | 21)
#define CELL_MS_SUBBUS_22	(CELL_MS_BUS_FLAG | 22)
#define CELL_MS_SUBBUS_23	(CELL_MS_BUS_FLAG | 23)
#define CELL_MS_SUBBUS_24	(CELL_MS_BUS_FLAG | 24)
#define CELL_MS_SUBBUS_25	(CELL_MS_BUS_FLAG | 25)
#define CELL_MS_SUBBUS_26	(CELL_MS_BUS_FLAG | 26)
#define CELL_MS_SUBBUS_27	(CELL_MS_BUS_FLAG | 27)
#define CELL_MS_SUBBUS_28	(CELL_MS_BUS_FLAG | 28)
#define CELL_MS_SUBBUS_29	(CELL_MS_BUS_FLAG | 29)
#define CELL_MS_SUBBUS_30	(CELL_MS_BUS_FLAG | 30)
#define CELL_MS_SUBBUS_31	(CELL_MS_BUS_FLAG | 31)

#define CELL_MS_STREAM_NOFLAGS	           (0)
#define CELL_MS_STREAM_AUTOCLOSE	       (1)	
#define CELL_MS_NOFLAGS                    (0)
#define CELL_MS_DEBUG_FLAG                 (1) 
#define CELL_MS_ROUTABLE_STREAMS_FLAG	   (2)
#define CELL_MS_EXTERNAL_BUFFERS		   (4)

/** Disables the MultiStream SPU printf server. */	  
#define CELL_MS_DISABLE_SPU_PRINTF_SERVER  (8)	  

#define CELL_MS_TD_ONLY_128						(16)	  
#define CELL_MS_TD_ONLY_256						(32)	  
#define CELL_MS_TD_ONLY_512						(64)	 
#define CELL_MS_SEPERATE_VOLUMES_PER_ROUTE		(128)
#define CELL_MS_SEPARATE_VOLUMES_PER_ROUTE		CELL_MS_SEPERATE_VOLUMES_PER_ROUTE
#define CELL_MS_FORCE_OPENAL_PPU				(256)
#define CELL_MS_USE_SMOOTH_ENVELOPE				(4096)


#define CELL_MS_ALLOC_DSPHANDLE	           (-1)
#define CELL_MS_ALLOC_PAGE                 (-1)
#define CELL_MS_DSPOFF                     (-1)

#define CELL_MS_16BIT_BIG                      (0)
#define CELL_MS_16BIT_LITTLE                   (1)
#define CELL_MS_32BIT_FLOAT                    (2)
#define CELL_MS_ADPCM                          (3)
#define CELL_MS_ATRAC_LOW                      (4)
#define CELL_MS_ATRAC_MEDIUM                   (5)
#define CELL_MS_ATRAC_HIGH                     (6)        
#define CELL_MS_MP3							   (7)

#define CELL_MS_STOP                           (0)
#define CELL_MS_KEYOFF	                       (-1)

#define CELL_MS_STREAM_OFF                     (0)
#define CELL_MS_STREAM_CLOSED                  (32)
#define CELL_MS_STREAM_ON                      (1)
#define CELL_MS_STREAM_PRIORITY_MUTE	       (2)
#define CELL_MS_STREAM_PAUSE	               (4)
#define CELL_MS_STREAM_PAUSE_SUB		       (8)	      
#define CELL_MS_STREAM_PENDING                 (16)       

#define CELL_MS_ENVELOPE_NO_LOOP               (255)
#define CELL_MS_ENVELOPE_LIN	               (0)
#define CELL_MS_ENVELOPE_EXP	               (1)	      

#define CELL_MS_SURROUND_GLOBAL                (0x80000)
#define CELL_MS_SURROUND_ACTIVE_STREAM	       (-1)
#define CELL_MS_SURROUND_ACTIVE_OFF	       (-2)
#define CELL_MS_SURROUND_NONE	                    (0)
#define CELL_MS_SURROUND_FALSE                      (0)	
#define CELL_MS_SURROUND_TRUE	                    (1)
#define CELL_MS_SURROUND_SOURCE_RELATIVE	    (0x202)
#define CELL_MS_SURROUND_CONE_INNER_ANGLE	    (0x1001)
#define CELL_MS_SURROUND_CONE_OUTER_ANGLE	    (0x1002)
#define CELL_MS_SURROUND_PITCH	                    (0x1003)
#define CELL_MS_SURROUND_POSITION	            (0x1004)	
#define CELL_MS_SURROUND_DIRECTION                  (0x1005)
#define CELL_MS_SURROUND_VELOCITY	            (0x1006)	
#define CELL_MS_SURROUND_GAIN	                    (0x100A)
#define CELL_MS_SURROUND_MIN_GAIN	            (0x100D)	
#define CELL_MS_SURROUND_MAX_GAIN	            (0x100E)	
#define CELL_MS_SURROUND_ORIENTATION	            (0x100F)
#define CELL_MS_SURROUND_REFERENCE_DISTANCE	    (0x1020)	       
#define CELL_MS_SURROUND_ROLLOFF_FACTOR	            (0x1021)
#define CELL_MS_SURROUND_CONE_OUTER_GAIN	    (0x1022)
#define CELL_MS_SURROUND_MAX_DISTANCE	            (0x1023)	
#define CELL_MS_SURROUND_DOPPLER_FACTOR             (0xC000)
#define CELL_MS_SURROUND_DOPPLER_VELOCITY           (0xC001)
#define CELL_MS_SURROUND_SPEED_OF_SOUND	            (0xC003)
#define CELL_MS_SURROUND_DISTANCE_MODEL             (0xD000)
#define CELL_MS_SURROUND_INVERSE_DISTANCE	    (0xD001)
#define CELL_MS_SURROUND_INVERSE_DISTANCE_CLAMPED   (0xD002)	
#define CELL_MS_SURROUND_LINEAR_DISTANCE            (0xD003)	
#define CELL_MS_SURROUND_LINEAR_DISTANCE_CLAMPED    (0xD004)	
#define CELL_MS_SURROUND_EXPONENT_DISTANCE	    (0xD005)
#define CELL_MS_SURROUND_EXPONENT_DISTANCE_CLAMPED  (0xD006)

/*
Bits:
1 = LFE on
2 = FC on
4 = FL,FR,RL,RR on
*/

#define CELL_MS_SURROUND_OUTPUT_LFE 1
#define CELL_MS_SURROUND_OUTPUT_FC  2
#define CELL_MS_SURROUND_OUTPUT_FLFR_RLRR 4

/** MultiStream addition to OpenAL specification **/
#define CELL_MS_SURROUND_USER_ROLLOFF_CURVE		(0xDFFF)

/** Source output volume routing for OpenAL calculations (0.1 - LFE channel only) */
#define CELL_MS_SURROUND_OUTPUT_0_1	CELL_MS_SURROUND_OUTPUT_LFE
/** Source output volume routing for OpenAL calculations (4.0 - 4 speakers, LFE & FC set to 0) */
#define CELL_MS_SURROUND_OUTPUT_4_0 CELL_MS_SURROUND_OUTPUT_FLFR_RLRR
/** Source output volume routing for OpenAL calculations (4.0 - 5 speakers, FC set to 0) */
#define CELL_MS_SURROUND_OUTPUT_4_1 CELL_MS_SURROUND_OUTPUT_FLFR_RLRR | CELL_MS_SURROUND_OUTPUT_LFE
/** Source output volume routing for OpenAL calculations (5.0 - 5 speakers, LFE set to 0) */
#define CELL_MS_SURROUND_OUTPUT_5_0 CELL_MS_SURROUND_OUTPUT_FLFR_RLRR | CELL_MS_SURROUND_OUTPUT_FC
/** Source output volume routing for OpenAL calculations (5.1 - 6 speakers) */
#define CELL_MS_SURROUND_OUTPUT_5_1 CELL_MS_SURROUND_OUTPUT_FLFR_RLRR | CELL_MS_SURROUND_OUTPUT_FC | CELL_MS_SURROUND_OUTPUT_LFE

#define CELL_MS_SURROUND_CONFIG_SPEAKERS_45_135 8	// Sets speaker angles at 90 degrees (starting at 45 degrees)
#define CELL_MS_SURROUND_CONFIG_SPEAKERS_30_110 16	// Sets panning angles to match 5.0 speaker arrangement


#define CELL_MSFX_VOCODER_BYPASS_OFF	        (0)	    
#define CELL_MSFX_VOCODER_BYPASS_ON             (1)
#define CELL_MSFX_VOCODER_BANDS                 (64)         

#define CELL_MSFX_IR_BUFFER_SIZE	            (4096)      

/** Compressor DSP defines, used in flags parameter **/
// Detection mode switching, peak mode only supported in time domain compressors
#define CELL_MSFX_COMPRESSOR_RMS_MODE               (0)
#define CELL_MSFX_COMPRESSOR_PEAK_MODE              (1)

// Multi-channel calculation switching
// This will either take the maximum volume or the averaged volume
// in multi-channel mode
#define CELL_MSFX_COMPRESSOR_CHAN_VOLUMES_AVERAGE   (0)
#define CELL_MSFX_COMPRESSOR_CHAN_VOLUMES_MAX       (2)



#define CELL_MS_DSPALLSLOTS                     (-1)

#define CELL_MS_ERROR 8000000                             ///< Base error value.
#define CELL_MS_ERROR_SYSPAUSED (CELL_MS_ERROR + 1)       ///< The system is paused.
#define CELL_MS_ERROR_INVAL (CELL_MS_ERROR + 2)           ///< A parameter is invalid.
#define CELL_MS_ERROR_WRONGSTATE (CELL_MS_ERROR + 3)      ///< A state is wrong.
#define CELL_MS_ERROR_RESOURCEUNAVAIL (CELL_MS_ERROR + 4) ///< A system resource is unavailable.
#define CELL_MS_ERROR_CIRCULARDEPEND (CELL_MS_ERROR + 5)  ///< There is a circular dependency.
#define CELL_MS_ERROR_BUSMISMATCH (CELL_MS_ERROR + 6)     ///< There is a bus mismatch. 
#define CELL_MS_ERROR_INVALIDELF (CELL_MS_ERROR + 7)      ///< An ELF file is invalid.
#define CELL_MS_ERROR_FILEACCESS (CELL_MS_ERROR + 8)      ///< Access to a file failed.
#define CELL_MS_ERROR_INVALIDENV (CELL_MS_ERROR + 9)      ///< An envelope is invalid.
#define CELL_MS_ERROR_OUTOFRANGE (CELL_MS_ERROR + 10)     ///< Out of range.
#define CELL_MS_ERROR_BADMODE (CELL_MS_ERROR + 11)        ///< There is a bad mode. 
#define CELL_MS_ERROR_NOFREESTREAMS (CELL_MS_ERROR + 12)  ///< There are no free streams.
#define CELL_MS_ERROR_SURROUNDNOTINIT (CELL_MS_ERROR + 13)///< Surround sound is not initialised.
#define CELL_MS_ERROR_INVALID_CHANNEL (CELL_MS_ERROR + 14)///< A channel is invalid.
#define CELL_MS_ERROR_INVALID_TYPE (CELL_MS_ERROR + 15)   ///< A type is invalid.
#define CELL_MS_ERROR_AUDIO_CONFIGURE (CELL_MS_ERROR +16) ///< cellMSSystemConfigureSysUtil failed processing cellAudioOutConfigure function
#define CELL_MS_ERROR_AUDIO_GETSTATE (CELL_MS_ERROR +17)  ///< cellMSSystemConfigureSysUtil failed processing cellAudioOutGetState function

/** Turns priority system off for stream. */
#define CELL_MS_PRIORITY_OFF	               (-1)	

/** Sets maximum amount of streams that can be played on a bank to unlimited. */       
#define CELL_MS_PRIORITY_UNLIMITED    (CELL_MS_MAX_STREAMS)

/** Sets the threshold level of a bank to maximum. */ 
#define CELL_MS_PRIORITY_MAX_THRESHOLD (10000000)	        

#define CELL_MS_SULPHA_NOT_ENABLED 	(0)
#define CELL_MS_SULPHA_ENABLED_INACTIVE (1)
#define CELL_MS_SULPHA_FILE_CONNECTED 	(2)
#define CELL_MS_SULPHA_DECI3_CONNECTED 	(3)

#define CELL_MS_SULPHA_NAMED_TYPE_DSP (0)
#define CELL_MS_SULPHA_NAMED_TYPE_STREAM (1)
#define CELL_MS_SULPHA_NAMED_TYPE_SUB (2)

/**********************************************************************************************************
E N U M S
**********************************************************************************************************/

typedef enum CELL_MS_DOMAINS {
	CELL_MS_TIMEDOMAIN = 0, ///< Time domain.
	CELL_MS_FREQDOMAIN = 1  ///< Frequency domain.
} CELL_MS_DOMAINS;

typedef enum CELL_MS_DSPMASKS {
	CELL_MS_INMASK = 0, ///< In mask.
	CELL_MS_OUTMASK = 1 ///< Out mask.
} CELL_MS_DSPMASKS;

typedef enum CELL_MS_DSPSLOTS {
	CELL_MS_DSP_SLOT_0 = 0, ///< DSP slot number 0.
	CELL_MS_DSP_SLOT_1 = 1, ///< DSP slot number 1.
	CELL_MS_DSP_SLOT_2 = 2, ///< DSP slot number 2.
	CELL_MS_DSP_SLOT_3 = 3, ///< DSP slot number 3.
	CELL_MS_DSP_SLOT_4 = 4, ///< DSP slot number 4.
	CELL_MS_DSP_SLOT_5 = 5, ///< DSP slot number 5.
	CELL_MS_DSP_SLOT_6 = 6, ///< DSP slot number 6.
	CELL_MS_DSP_SLOT_7 = 7  ///< DSP slot number 7.
} CELL_MS_DSPSLOTS;

typedef enum CELL_MS_ROUTES {
	CELL_MS_ROUTE_OFF = 0,        ///< Sub bus routing is turned off.
	CELL_MS_ROUTE_ON = 1,         ///< Sub bus routing is turned on.
	CELL_MS_ROUTE_ALL_OFF = 2,    ///< All sub bus routing is turned off.
	CELL_MS_ROUTE_VOLUME_ONLY = 3 ///< Sub bus routing remains unchanged;
	///< only the volume is modified.
} CELL_MS_ROUTES;

typedef enum CELL_MS_CHANNELS {
	CELL_MS_WET_STREAM = 0, ///< Channel number 0.
	CELL_MS_CHANNEL_0 = 0,  ///< Channel number 0.
	CELL_MS_CHANNEL_1 = 1,  ///< Channel number 1.
	CELL_MS_CHANNEL_2 = 2,  ///< Channel number 2.
	CELL_MS_CHANNEL_3 = 3,  ///< Channel number 3.
	CELL_MS_CHANNEL_4 = 4,  ///< Channel number 4.
	CELL_MS_CHANNEL_5 = 5,  ///< Channel number 5.
	CELL_MS_CHANNEL_6 = 6,  ///< Channel number 6.
	CELL_MS_CHANNEL_7 = 7   ///< Channel number 7.
} CELL_MS_CHANNELS;

typedef enum CELL_MS_SPEAKERS {
	CELL_MS_SPEAKER_FL  = 0,  ///< Front left speaker.
	CELL_MS_SPEAKER_FR  = 1,  ///< Front right speaker.
	CELL_MS_SPEAKER_FC  = 2,  ///< Front center speaker.
	CELL_MS_SPEAKER_LFE = 3,  ///< LFE speaker.
	CELL_MS_SPEAKER_RL  = 4,  ///< Rear left speaker.
	CELL_MS_SPEAKER_RR  = 5,  ///< Rear right speaker.
	CELL_MS_SPEAKER_EL = 6,  ///< Extended left speaker.
	CELL_MS_SPEAKER_ER = 7   ///< Extended right speaker.
} CELL_MS_SPEAKERS;

typedef enum CELL_MS_EQSTATE {
	CELL_MS_EQDISABLED = 0, ///< EQ functionality is off.
	CELL_MS_EQENABLED = 1   ///< EQ functionality is on.
} CELL_MS_EQSTATE;

typedef enum CELL_MS_PAUSE {
	CELL_MS_NOTPAUSED = 0, ///< Pause is off.
	CELL_MS_PAUSED = 1     ///< Pause is on.
} CELL_MS_PAUSE;

typedef enum CELL_MS_PATH {
	CELL_MS_DRY = 0,        ///< Dry sub bus routing only.
	CELL_MS_WET = 1,        ///< Wet sub bus routing only.
	CELL_MS_WET_AND_DRY = 2 ///< Both wet and dry sub bus routing.
} CELL_MS_PATH;

typedef enum CELL_MS_BYPASS {
	CELL_MS_NOTBYPASSED = 0, ///< Bypass status is off.
	CELL_MS_BYPASSED = 1     ///< Bypass status is on.
} CELL_MS_BYPASS;

typedef enum CELL_MS_ENVSTATE {
	CELL_MS_ENVELOPE_OFF = 0, ///< Envelope processing is off.
	CELL_MS_ENVELOPE_ON = 1   ///< Envelope processing is on.
} CELL_MS_ENVSTATE;

typedef enum CELL_MS_PRIORITY_BANKS {
	CELL_MS_PRIORITY_BANK_0 = 0,  ///< Priority bank number 0.
	CELL_MS_PRIORITY_BANK_1 = 1,  ///< Priority bank number 1.
	CELL_MS_PRIORITY_BANK_2 = 2,  ///< Priority bank number 2.
	CELL_MS_PRIORITY_BANK_3 = 3,  ///< Priority bank number 3.
	CELL_MS_PRIORITY_BANK_4 = 4,  ///< Priority bank number 4.
	CELL_MS_PRIORITY_BANK_5 = 5,  ///< Priority bank number 5.
	CELL_MS_PRIORITY_BANK_6 = 6,  ///< Priority bank number 6.
	CELL_MS_PRIORITY_BANK_7 = 7,  ///< Priority bank number 7.
	CELL_MS_PRIORITY_BANK_8 = 8,  ///< Priority bank number 8.
	CELL_MS_PRIORITY_BANK_9 = 9,  ///< Priority bank number 9.
	CELL_MS_PRIORITY_BANK_10 = 10,///< Priority bank number 10.
	CELL_MS_PRIORITY_BANK_11 = 11,///< Priority bank number 11.
	CELL_MS_PRIORITY_BANK_12 = 12,///< Priority bank number 12.
	CELL_MS_PRIORITY_BANK_13 = 13,///< Priority bank number 13.
	CELL_MS_PRIORITY_BANK_14 = 14,///< Priority bank number 14.
	CELL_MS_PRIORITY_BANK_15 = 15,///< Priority bank number 15.
	CELL_MS_PRIORITY_BANK_16 = 16,///< Priority bank number 16.
	CELL_MS_PRIORITY_BANK_17 = 17,///< Priority bank number 17.
	CELL_MS_PRIORITY_BANK_18 = 18,///< Priority bank number 18.
	CELL_MS_PRIORITY_BANK_19 = 19,///< Priority bank number 19.
	CELL_MS_PRIORITY_BANK_20 = 20,///< Priority bank number 20.
	CELL_MS_PRIORITY_BANK_21 = 21,///< Priority bank number 21.
	CELL_MS_PRIORITY_BANK_22 = 22,///< Priority bank number 22.
	CELL_MS_PRIORITY_BANK_23 = 23,///< Priority bank number 23.
	CELL_MS_PRIORITY_BANK_24 = 24,///< Priority bank number 24.
	CELL_MS_PRIORITY_BANK_25 = 25,///< Priority bank number 25.
	CELL_MS_PRIORITY_BANK_26 = 26,///< Priority bank number 26.
	CELL_MS_PRIORITY_BANK_27 = 27,///< Priority bank number 27.
	CELL_MS_PRIORITY_BANK_28 = 28,///< Priority bank number 28.
	CELL_MS_PRIORITY_BANK_29 = 29,///< Priority bank number 29.
	CELL_MS_PRIORITY_BANK_30 = 30,///< Priority bank number 30.
	CELL_MS_PRIORITY_BANK_31 = 31 ///< Priority bank number 31.
} CELL_MS_PRIORITY_BANKS;

typedef enum  CELL_MSFX_FILTERMODE {
	CELL_MSFX_FILTERMODE_OFF,                        ///< Value = 0. No effect.
	CELL_MSFX_FILTERMODE_LOWPASS_RESONANT,           ///< Low pass two-pole resonant filter.Accepts cut-off and resonant Q.
	CELL_MSFX_FILTERMODE_HIGHPASS_RESONANT,          ///< High pass two-pole resonant filter. Accepts cut-off and resonant Q
	CELL_MSFX_FILTERMODE_BANDPASS_PEAK,              ///< Band pass filter with tunable gain in db at the peak freq
	CELL_MSFX_FILTERMODE_BANDPASS_ZERO,              ///< Band pass filter, maximum output is 0dB relative to input. tunable Q
	CELL_MSFX_FILTERMODE_NOTCH,                      ///< A notch filter, Removes frequencies around cut-off freq. bandwidth controlled by Q
	CELL_MSFX_FILTERMODE_PEAK,                       ///< Peak/Notch EQ, frequency controls center frequency,gain controls dB gain at center freq
	CELL_MSFX_FILTERMODE_HIGHSHELF,                  ///< High shelving
	CELL_MSFX_FILTERMODE_LOWSHELF,                   ///< Low shelving filter/EQ.
	CELL_MSFX_FILTERMODE_LOWPASS_ONEPOLE,            ///< Low pass one-pole
	CELL_MSFX_FILTERMODE_HIGHPASS_ONEPOLE,           ///< High pass one-pole
	CELL_MSFX_FILTERMODE_ALLPASS,                    ///< All pass filter.
	CELL_MSFX_FILTERMODE_LOWPASS_RESONANT_NORMALIZED,///< Low pass resonant
    CELL_MSFX_FILTERMODE_LFE_6_POLE,                 ///< 6 pole (36dB per octave) special mode for Parametric EQ designed for LFE channel.
    CELL_MSFX_FILTERMODE_LFE_8_POLE                  ///< 8 pole (48dB per octave) special mode for Parametric EQ designed for LFE channel.
} CELL_MSFX_FILTERMODE;

typedef enum CELL_MSFX_I3DL2_REFLECTIONS 
{
    CELL_MSFX_ROOM1_LEFT = 0,
    CELL_MSFX_ROOM1_RIGHT,
    CELL_MSFX_ROOM2_LEFT,
    CELL_MSFX_ROOM2_RIGHT,
    CELL_MSFX_ROOM3_LEFT,
    CELL_MSFX_ROOM3_RIGHT

} CELL_MSFX_I3DL2_REFLECTIONS;

typedef enum CELL_MSFX_I3DL2_LATE_MODEL
{
    CELL_MSFX_ROOM1_LATE_LEFT = 0,
    CELL_MSFX_ROOM1_LATE_RIGHT

} CELL_MSFX_I3DL2_LATE_MODEL;

typedef enum CELL_MSFX_SLOTMASK_MODE
{
    CELL_MSFX_SLOTMASK_MODE_MONO = 1,   ///< L
    CELL_MSFX_SLOTMASK_MODE_STEREO,     ///< L, R
    CELL_MSFX_SLOTMASK_MODE_2_1,        ///< L, R, LFE
    CELL_MSFX_SLOTMASK_MODE_4_0,        ///< L, R, LS, RS
    CELL_MSFX_SLOTMASK_MODE_5_0,        ///< L, R, C, LS, RS
    CELL_MSFX_SLOTMASK_MODE_5_1,        ///< L, R, C, LFE, LS, RS
    CELL_MSFX_SLOTMASK_MODE_6_0,        ///< L, R, LS, RS, LE, RE
    CELL_MSFX_SLOTMASK_MODE_7_0,        ///< L, R, C, LS, RS, LE, RE
    CELL_MSFX_SLOTMASK_MODE_7_1         ///< L, R, C, LFE, LS, RS, LE, RS
} CELL_MSFX_SLOTMASK_MODE;


/**********************************************************************************************************
T Y P E S
**********************************************************************************************************/

typedef struct CellMSSystemConfig {
	int channelCount; ///< Maximum number of streams to be available in the system
	int subCount;     ///< Maximum number of sub busses to be available in the system
	int dspPageCount; ///< Number of 16kb DSP pages to be available in the
	///< system. You can reset any of these pages using
	///< the cellMSDSPResetPage() function.
	int flags;        ///< Flags to be passed to the system.
} CellMSSystemConfig;


typedef struct CellMSSystemInformation {
	int totalCycleCount;               ///< Total number of SPU cycles used in the last pass
	int miscCycleCount;                ///< Total number of MultiStream overhead cyccles used in last pass
	int totalNumberOfStreamsProcessed; ///< Total number of streams processed in last pass
	int totalMemoryBandwidth;          ///< Total system bandwidth (system in, system out, and DSP) in last pass
	int MemoryBandwidthIn;             ///< Total bandwidth (system in only) used in last pass
	///< used in last pass.
	int MemoryBandwidthOut;            ///< Total bandwidth (system out only) used in last pass
	int DSPBandwidthIn;                ///< Total bandwidth (DSP in only)  used in last pass
	int DSPBandwidthOut;               ///< Total bandwidth (DSP out only)  used in last pass
} CellMSSystemInformation;


typedef struct CellMSDebugInformation {
	CellMSSystemInformation *systemInfo; ///< Pointer to the CellMSSystemInformation structure
	int *subStreamCnt;                   ///< Pointer to the sub bus counters,
	int *streamCycleCount;               ///< Pointer to the stream cycle
	int *streamDSPCycleCount;            ///< Pointer to the stream DSP
	int *subCycleCount;                  ///< Pointer to the sub bus cycle
	int *subDSPCycleCount;               ///< Pointer to the sub bus DSP
} CellMSDebugInformation;


typedef struct CellMSComplexNum {
	float r; ///< Real part of a complex number.
	float i; ///< Imaginary part of a complex number.
} CellMSComplexNum;


typedef struct CellMSWaveSection
{
	unsigned int  numberOfChannels; ///< Number of channels that have been interleaved
	unsigned int  lengthOfChannel;  ///< Length of one channel in bytes
	unsigned int  sampleRate;       ///< Sample rate of the file
	unsigned char endian;     		///< CELL_MS_16BIT_LITTLE or CELL_MS_16BIT_BIG
	unsigned char reserved;			///< Reserved
} CellMSWaveSection;

typedef struct CellMSSection1 {
	unsigned char Header[4];   ///< Always contains the ASCII "MULT".
	unsigned int DataSize;     ///< Size (in bytes) of the data following the CellMSSection1 header, including any ASCII text string appended to the data.
	unsigned char ID[8];       ///< Identity of the data type in ASCII data.
	unsigned short IDValue;    ///< Reserved for future use.
	unsigned char Version[4];  ///< SDK release version of the header format in ASCII (aa.b.c)
	unsigned char Date[6];     ///< Date the file was created in ASCII, dd.mm.yyyy
	unsigned int InfoSize;     ///< Size (in bytes) of an ASCII text string, stored at end of file
	unsigned char SVersion[4]; ///< Version number (in ASCII) of the following data segment
	unsigned int InitData[8];  ///< This area can contain any data which may be required for any purpose
} CellMSSection1;


typedef struct CellMSEnvPoint {
	unsigned int X; ///< Time of the point in thousandths of a second. The
	///< value can be any non-negative integer.
	float Y;        ///< Amplitude of the point. The value ranges from 0
	///< (silence) to 1 (full amplitude).
} CellMSEnvPoint;


#define CELL_MS_ENVELOPE_MAX_POINTS          33
typedef struct CellMSEnvelope {
	CellMSEnvPoint Point[CELL_MS_ENVELOPE_MAX_POINTS];
	unsigned int ReleaseRate;
	unsigned int NumPoints;
	unsigned int LoopStart;
	unsigned int Type;
} CellMSEnvelope;

typedef struct CellEQMorphSection {
	float startVals[512]; ///< First set of FFT data for morphing.
	float endVals[512];   ///< Second set of FFT data for morphing.
} CellEQMorphSection;


typedef struct CellEQPoint {
	unsigned char Tag[8]; ///< Tag for the point. Allows easy access to any number of points by referring to this tag
	float x;              ///< Frequency in Hz (0-24000).
	float y;              ///< Percentage (0-n), where 0 = zero amplitude,
	///< 1 = 100%, 2= 200%, 0.5 = 50%.
} CellEQPoint;

typedef struct CellEQSection {
	unsigned int curveSwitch; ///< Indicates whether the points on the graph (linear, cubic, curve)
	unsigned int nPoints;     ///< Number of points contained in the EQ graph.
	CellEQPoint *xy;          ///< An array of EQ data points.
} CellEQSection;

typedef struct CellMSDSP {
	int memoryUsed;  ///< Size of SPU RAM DSP memory used.
	int memoryAvail; ///< Size of available SPU RAM DSP memory.
	int handle;      ///< Handle by which this DSP effect is referenced
	///< by other DSP functions.
	int page;        ///< DSP page to load the DSP into.
} CellMSDSP;

typedef struct CellMSInfo {
	int SubBusGroup;    		   ///< Sub bus for stream to use. This
	void *FirstBuffer;             ///< Address of first input data buffer
	unsigned int FirstBufferSize;  ///< Size of first input data buffer
	void *SecondBuffer;            ///< Address of second input data buffer
	unsigned int SecondBufferSize; ///< Size of second input data buffer
	int Pitch;                     ///< Playback frequency (in Hz).
	int numChannels;               ///< Number of channels in data (PCM is interleaved within data (1-8).)
	int initialOffset;             ///< Number of samples to delay before playback starts (0-511)
	int inputType;                 ///< Input data type (16bit big or little endian, float32 etc.).
	int flags;                     ///< Flags for handling cases such as
} CellMSInfo;


typedef struct CellMSSurroundInfo {
	float volumes[8];      ///< Eight volume levels calculated by surround sound process (using OpenAL 1.1 standard algorithms). Note that
	///< surround sound processing only calculates volume levels for five speakers (FL, FR, FC, RL, and RR). LFE, EXL, and EXR are set to 0.
	int pitch;             ///< Frequency (in Hz) calculated by surround sound process.
	float distance;        ///< Distance between listener and source calculated by surround sound process.
} CellMSSurroundInfo;

typedef struct CellMSFXVocoderInfo {
	float FFTBuffer[512*2];              ///< Work buffer containing the carrier signal
	float Band[CELL_MSFX_VOCODER_BANDS]; ///< Volume for the vocoder EQ bands
	long long Bypass;                    ///< Bit mask to enable or disable for each of the 64 bands
	float ModulatorMix;                  ///< Dry modulator mix level. Value ranges
	///< from 0 to <i>n</i>, where:
	///< 0 = off, 1 = original level,
	///< 2 = twice original level.
} CellMSFXVocoderInfo;

typedef struct CellMSFXCompressorInfo {
	float AnalysisPower;                ///< Value that compressor outputs.
	int firstBand;                      ///< First frequency band the compressor uses.
	int lastBand;                       ///< Last frequency band the compressor uses. Value
	float ratio;                        ///< Amount the volume is reduced in relation to how far above threshold the signal is.
	float threshold;                    ///< Threshold for automatic volume reduction.
	float attack;                       ///< Rate (in seconds) at which volume is reduced when volume exceeds threshold. Value ranges from 0 to n.
	float release;                      ///< Rate (in seconds) at which the volume returns when the volume no longer exceeds threshold.
	float makeupGain;                   ///< Volume control for the compressed audio signal.Value ranges from 0 to n, where 1 = no makeup, 2 = double amplitude.
	float CompressorCur;                ///< Used by the compressor algorithm.
	float multBy;                       ///< Used by the compressor algorithm.
	float softKnee;                     ///< Soft knee range (in decibels)
    int   flags;                        ///< Mode switching flags
} CellMSFXCompressorInfo;


typedef struct CellMSFXFilter {
	CELL_MSFX_FILTERMODE FilterMode; ///< Selected filter mode.
	float fFrequency;                ///< Cut-off frequency of the filter.
	float fResonance;                ///< Amount of resonant Q for the filter.This parameter only affects certain filter modes. Typical ranges are 0.2 to 10.0.
	float fGain;                     ///< Decibel gain or cut-off of the filter. This parameter only affects certain filter modes.
} CellMSFXFilter;

typedef struct CellMSFXDistortion {
	float fA;       ///< A parameter for the distortion algorithm. Value ranges from 0 to 10.
	float fB;       ///< B parameter for the distortion algorithm. Value	ranges from 0 to 10.
	float fLimit;   ///< Limiter on the audio output of the polynomial stage. Value ranges from 0 to 1
	float fGate;    ///< Noise gate on the audio output. Value ranges from 0-1
	float fWetGain; ///< Wet gain. Amplitude of the distorted signal. Value ranges from 0-4 (1=original volume, 2 = twice volume)
	float fDryGain; ///< Dry gain. Amount of original signal to mix with the distorted signal. Value ranges from 0 to 4
} CellMSFXDistortion;

typedef struct CellMSFXReverbParams {
	float fWetVolume;  ///< Linear scalar for wet volume (1 == original volume, 0.5 == half volume, and so on.)
	float fDryVolume;  ///< Linear volume scaling for dry output.
	float fPreDelayMs; ///< Millisecond pre-delay for input to reverb; adds a small delay creating further illusions of space 0 to 148.1 milliseconds.
	float fLowPassHz;  ///< Low pass cut-off frequency in Hz (-6dB).
	float fHighPassHz; ///< High pass cut-off frequency in Hz (-6dB).
	float fRoomSize;   ///< Size of room. 0 to 1 where 0 = tiny and 1 = huge. Larger values increase decay time.
	float fDamping;    ///< 0 to 1 frequency dependent reflection damping. 0 = highly reflective, 1 = highly absorptive
	float fDiffusion;  ///< Similar to damping. 0 to 1 where 0 = no late absorption/diffraction of sound and 1 = greater absorption of higher frequencies over time.
	float fEarlyLate;  ///< 0 to 1, where 0 = closer to listener (early reflections only) and 1 = further away (late reflections only).
	float fLeftRight;  ///< 0 to 1 applies stereo dispersion to early and late reflections, where 0 = fully left and 1= fully right.
	float fFrontBack;  ///< 0 to 1 applies changes to early reflections to model distance from source to listener.
} CellMSFXReverbParams;

typedef struct CellMSFXDelay {
	CellMSFXFilter InFilters[2];  ///< Series input filters. See CellMSFXFilter for information about the parameters required within this structure.
	CellMSFXFilter TapFilters[4]; ///< Per delay tap filter. Each "Tap" represents a delay offset in milliseconds.
	float TapDelayMs[4];          ///< Millisecond delay per tap. Value should be in the range of 5.4 to 1000 milliseconds.
	float TapVolume[4];           ///< Output volume per tap linear. Each "Tap" represents a delay offset in milliseconds.
	float TapFeedback[4];         ///< Feedback per tap linear. Each "Tap" represents a delay offset in milliseconds.
	float DryVolume;              ///< Dry throughput volume linear.
	float InputVolume;            ///< Volume into delay line linear. 
} CellMSFXDelay;


typedef struct CellMSFXRingMod {
	float fFrequency; ///< Frequency in Hz of the sine wave for multiplication.
	float fAmplitude; ///< Volume, in a linear format, of multiplication sine wave, where 1 = normal, 0.5 = half volume..
} CellMSFXRingMod;


typedef struct CellMSFXParaEQ {
	CellMSFXFilter Filters[4]; ///< Array of four filters that make up the parametric equaliser. These are applied in series.
} CellMSFXParaEQ;


typedef struct CellMSFXPitchShift {
	float fCents;	///< Pitch shift frequency in cents. There are 100 cents in a semitone and 12 semitones in a musical octave.

} CellMSFXPitchShift;

typedef struct CellMSFXSoftClipper
{
	float fDrive;
	float fGain;
} CellMSFXSoftClipper;

typedef struct CellMSFXAuxSend
{
	int nSendChannels;
	int nSendsPerChannel;
} CellMSFXAuxSend;

typedef struct CellMSFXI3DL2Params
{
    float Room;                 // room (main) volume control in mB                     -10000 to 0 mB ( 100 mB == 1 dB )
    float Room_HF;              // high frequency content input in mB, at HF_reference  -10000 to 0 mB
    float Decay_time;           // decay time of late reverberation in seconds          0.1 to 20 seconds
    float Decay_HF_ratio;       // ratio of high frequency decay in late reverb         0.1 to 2.0
    float Reflections;          // volume of early reflections in mB                    -10000 to 1000 mB
    float Reflections_delay;    // delay from input signal until first reflection       0.0 to 0.3 seconds
    float Reverb;               // volume of late reverb in mB                          -10000 to 2000 mB
    float Reverb_delay;         // delay from first reflection until late reverb        0.0 to 0.1 seconds
    float Diffusion;            // echo density in late reverberation                   0 to 100 %
    float Density;              // modal density in late reverberation                  0 to 100 %
    float HF_reference;         // reference frequency for Room_HF and Decay_HF_ratio   20 to 20000 Hertz

    // extended parameters ( not in standard )
    int                          MixChannel;             // channel index to crossfade / mix signal with for stereo -1 == off, 0 to 7 == mix chan
    float                        MixAmount;              // percentage to cross-mix with MixChannel   0 to 100 %
    CELL_MSFX_I3DL2_REFLECTIONS  EarlyReflectionPattern; //
    CELL_MSFX_I3DL2_LATE_MODEL   LateReverbPattern;      //
    float                        EarlyReflectionScaler;  // scaler 0 -> 100% for early reflections. 0 == single reflection 100 == widely spread reflections
    float                        LF_reference;           // reference low frequency                              20 to 20000 Hertz
    float                        Room_LF;                // low frequency content input in mB, at LF_reference   -10000 to 0 mB
} CellMSFXI3DL2Params;

typedef struct CellMSFXEarlyReflection
{
    float fVol;             ///< linear volume
    int   nOffsetSamps;     ///< maximum offset in samples 0 to 4800
} CellMSFXEarlyReflection;

typedef struct CellMSFXMeter
{
    float fThresholdLinear;          ///< Threshold to test against

    // output values
    float fPeakLinearLastFrame;      ///< Peak in last frame
    float fRMSLinearLastFrame;       ///< RMS of last frame
    int   nSampsOverThresholdFrame;  ///< Number of samples over fThresholdLinear in last frame

    float fAverageRMS;               ///< RMS value over multiple frames

    // stored values (between frames)
    float fMaxPeakLinear;            ///< Largest seen peak (multiple packets)
    float fMaxRMSLinear;             ///< Largest seen RMS of a frame
    int   nMaxSampsOverThreshold;    ///< Largest number of event over threshold in a packet

} CellMSFXMeter;

typedef struct CellMSSurroundPoint {
	float X; /// X Distance from listener (0.0f-1.0f)
	float Y; /// Y Amplitude at X point (0.0f-1.0f, 0=silence. 1=full amplitude)
} CellMSSurroundPoint;

typedef void  (*CELL_MS_CALLBACK_FUNC) (int streamNumber, void * userData, int callbackType, void *readBuffer, int readSize);

typedef struct CellMSMP3FrameHeader {
	unsigned int Sync;
	unsigned int ID;
	unsigned int Layer;
	unsigned int ProtBit;
	unsigned int BitRate;
	unsigned int Frequency;
	unsigned int PadBit;
	unsigned int PrivBit;
	unsigned int Mode;
	unsigned int ModeExt;
	unsigned int Copy;
	unsigned int Home;
	unsigned int Emphesis;
	float PacketTime;
	unsigned int PacketSize;
	unsigned int ID3;			// 16 bit value (VV,vv), VV=Major version, vv=revision (example: ID3v2.4). Set to 0 if packet is not an ID3 packet
	unsigned int Tag;			// 0 = packet is not a Tag packet, 1=Packet is a Tag packet
} CellMSMP3FrameHeader;


typedef struct CellMSMSFHeader
{
    char header[3];			// Will be ASCII: 'MSF'
    char version;			// Revision of tool used to create MSF file
    int compressionType;	// Compression type of the output (matches CELL_MS_MP3 / CELL_MS_ADPCM etc..)
	int channels;			// Number of audio channels (1=mono, 2=stereo...)
    int sampleSize;			// Number of bytes of sample data following the 64 bytes of header data
    int sampleRate;			// Playback sample rate (in Hz)
    int miscInfo;			// Bits 0-3 = number of Loops stored.
    						// Bit 4    = File created with -loop option. 
    						// Bit 5    = File is Constant bitrate (0) or Variable bit rate (1) (if compressionType=CELL_MS_MP3)
    int LoopMarkers[10];	// 10 loop markers.
							// [0],[2],[4],[6],[8] = Loop start offset (in bytes)
							// [1],[3],[5],[7],[9] = Loop Size (in bytes)
} CellMSMSFHeader;

typedef struct 
{
	char sendSubData[32][2][8]; //Wether we are sending bus wet and dry information.
	char sendStreamData[512];
	char compressBusToADPCM;
	char sendDebugInformation;
	int namedObjects;
} CellMSSulphaConfiguration;


#define CELL_MS_SULPHA_WIREFRAME_BOX (0)
#define CELL_MS_SULPHA_BOX (1)
#define CELL_MS_SULPHA_WIREFRAME_SPHERE (2)
#define CELL_MS_SULPHA_SPHERE (3)

typedef struct 
{
	unsigned char red;
	unsigned char green;
	unsigned char blue;
	unsigned char alpha;
} CellMSSulpha3dObjectRGBA;

typedef struct  
{
	float x;
	float y;
	float z;
} CellMSSulpha3dObjectPosition;

typedef struct  
{
	float width;
	float height;
	float depth;
} CellMSSulpha3dObjectDimensions;

typedef struct
{
	int type;
	CellMSSulpha3dObjectPosition position;
	CellMSSulpha3dObjectDimensions dimension;
	CellMSSulpha3dObjectRGBA colour;
} CellMSSulpha3dObject;


/**********************************************************************************************************
P R O T O T Y P E S
**********************************************************************************************************/

#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
extern "C" {
#endif


/**********************************************************************************************************
S Y S T E M
**********************************************************************************************************/
int  cellMSSystemGetNeededMemorySize (const CellMSSystemConfig *systemConfig); 
int cellMSSystemInitSPUThread(const void *systemMemory, const CellMSSystemConfig *systemConfig, const int priority);
void  cellMSSystemSetGlobalCallbackFunc (const CELL_MS_CALLBACK_FUNC func); 
void  cellMSSystemSetGlobalCallbackData (void *Data); 
void cellMSSystemSetPause(const CELL_MS_PAUSE pause);
CELL_MS_PAUSE  cellMSSystemGetPause(void);
void  *cellMSSystemClose (void); 
float  *cellMSSystemSignalSPU(void);
char  *cellMSSystemGetCallbacksNeeded(void);
void  cellMSSystemFreeCallbacksNeeded(void);
void  cellMSSystemGenerateCallbacks(void);
int  cellMSSystemGetDebugPointer(CellMSDebugInformation *dbgInfo);
void  cellMSSystemGetInfo(int *freeStreams, int *usedStreams);
int  cellMSSystemGetLastError(void);

#ifdef __CELLOS_LV2__
int  cellMSSystemConfigureLibAudio (CellAudioPortParam *audioParam, CellAudioPortConfig *portConfig); 
int cellMSSystemConfigureSysUtil(void);
int cellMSSystemConfigureSysUtilEx(unsigned int _flags);
int cellMSSystemInitSPURS(const void *systemMemory, const CellMSSystemConfig *systemConfig, const CellSpurs *spurs, const uint8_t *priorities);
#endif

/**********************************************************************************************************
S T R E A M
**********************************************************************************************************/
void  cellMSStreamSetFirstRead (const int streamNo, const void *Addr, const int size); 
void  cellMSStreamSetSecondRead (const int streamNo, const void *Addr, const int size); 
void  cellMSStreamSetCallbackData (const int streamNo, const void *Data); 
void  cellMSStreamSetCallbackFunc (const int streamNo, const CELL_MS_CALLBACK_FUNC func); 
int  cellMSStreamSetSub (const int streamNo, const int busNo); 
void  cellMSStreamSetPitch (const int streamNo, const int pitch); 
void cellMSStreamSetEnvBypass(const int streamNo, const CELL_MS_BYPASS Switch);
int cellMSStreamSetRouting(const int streamNo, const int busNo, float subGain);
int cellMSStreamSetRoutingDry(const int streamNo, const int busNo, float drySubGain);
int cellMSStreamSetRoutingWet(const int streamNo, const int busNo, float wetSubGain);
void  *cellMSStreamGetCallbackData (const int streamNo); 
void  *cellMSStreamGetCallbackFunc (const int streamNo); 
int cellMSStreamGetStatus(const int streamNo);
int cellMSStreamGetRoutingWetDry(const int streamNo, const int busNo, float *dryGain, float *wetGain);
float cellMSStreamGetRouting(const int streamNo, const int busNo);
int cellMSStreamGetRoutingMask(const int streamNo);
void  cellMSStreamClose (const int streamNo); 
int  cellMSStreamOpen (void);
void  cellMSStreamPlay (const int streamNo);
int  cellMSStreamSetInfo(const int StreamNo, const CellMSInfo *data);
int  cellMSStreamGetInfo (const int streamNo, CellMSInfo *data);
CELL_MS_BYPASS cellMSStreamGetEnvBypass(const int streamNo);
void  cellMSStreamGetCurrentPlayPos (const int streamNo, int *currentOffset, int *bufferAddr); 
void  cellMSStreamSetFirstReadSkip (const int streamNo, const void *Addr, const int size, const int sampleSkip, const int reserved); 
void  cellMSStreamSetSecondReadSkip (const int streamNo, const void *Addr, const int size, const int sampleSkip, const int reserved); 
float  cellMSStreamGetCurrentEnv (const int streamNo); 
int  cellMSStreamSetPriorityLevel (int ch, float priorityLevel); 
int  cellMSStreamSetPriorityBank (int ch, const CELL_MS_PRIORITY_BANKS priorityBank); 
int  cellMSStreamSetPriorityBankThreshold (const CELL_MS_PRIORITY_BANKS priorityBank, float threshold); 
int  cellMSStreamSetPriorityBankMaxLimit (const CELL_MS_PRIORITY_BANKS priorityBank, int limit); 
int cellMSStreamGetPriorityBank (int ch );
float cellMSStreamGetPriorityLevel (int ch );
float  cellMSStreamGetPriorityBankThreshold (const CELL_MS_PRIORITY_BANKS priorityBank );
int cellMSStreamGetPriorityBankMaxLimit (const CELL_MS_PRIORITY_BANKS priorityBank );
int  cellMSStreamSetSurroundInfo (int streamNo, float *vols, int pitch);
int  cellMSStreamGetSurroundInfo (int streamNo, CellMSSurroundInfo *info);
void cellMSStreamSetWetInGains(int chNo, const float* ptr);
int cellMSStreamGetMP3FramePos(const int streamNo, int *currentSampleOffset, int *packetSampleSize, int *currentSeekAddr);
int cellMSStreamSetRouting8(const int streamNo, const int busNo, float *drySubGain);
int cellMSStreamSetRoutingWet8(const int streamNo, const int busNo, float *wetSubGain);
int cellMSStreamSetRoutingDry8(const int streamNo, const int busNo, float *drySubGain);
void cellMSStreamClearRouting(int streamNum, CELL_MS_PATH path);
int cellMSStreamGetMP3FrameInfo(int streamNum, int *numFrames, int *totalSizeOfFrames);

/**********************************************************************************************************
C O R E
**********************************************************************************************************/
void  cellMSCoreSetWetInGain (int chNo, const float inGain);
int  cellMSCoreSetVolume1 (int chNo, const CELL_MS_PATH type, const CELL_MS_SPEAKERS speakerNo, const CELL_MS_CHANNELS channelNo, const float volume);
int  cellMSCoreSetVolume64 (int chNo, const CELL_MS_PATH type, const float *volumes);
int  cellMSCoreSetVolume8 (int chNo, const CELL_MS_PATH type, const CELL_MS_CHANNELS channelNo, const float *volumes);
int  cellMSCoreSetEQ (int chNo, const float *eqData);
void cellMSCoreSetEQEnable(int chNo, const CELL_MS_PATH type, const CELL_MS_EQSTATE eqSwitch);
void  cellMSCoreSetBypass (int chNo, const CELL_MS_PATH type, const CELL_MS_BYPASS bypass); 
void cellMSCoreSetPause(int chNo, const CELL_MS_PAUSE pause);
void  cellMSCoreSetDSP (int chNo, const CELL_MS_DSPSLOTS slot, const int dspHandle);
void  cellMSCoreBypassDSP (int chNo, const CELL_MS_DSPSLOTS slot, const CELL_MS_BYPASS bypass);
int  cellMSCoreInit (int chNo); 
int  cellMSCoreRoutingInit (int typeBitMask); 
int  cellMSCoreSetRouting (int busNo, int destBus, const CELL_MS_PATH type, const CELL_MS_ROUTES onOff, const float dryVolume, float wetVolume);
int cellMSCoreSetRouting8(int busNo, int destBus, const CELL_MS_PATH type, const CELL_MS_ROUTES onOff, const float *dryVolume, const float *wetVolume);;
void  cellMSCoreSetMask (int chNo, const CELL_MS_DSPMASKS dspMask, const CELL_MS_DSPSLOTS dspSlot, const int maskSlot, const char bitMask);
void  cellMSCoreSetMasks (int chNo, const CELL_MS_DSPMASKS dspMask, const CELL_MS_DSPSLOTS dspSlot, const char *bitMasks);
int cellMSCoreGetMask (int chNo, const CELL_MS_DSPMASKS dspMask, const CELL_MS_DSPSLOTS dspSlot, const int maskSlot);
char *cellMSCoreGetMasks(int chNo, const CELL_MS_DSPMASKS dspMask, const CELL_MS_DSPSLOTS dspSlot);
float  cellMSCoreGetWetInGain (int chNo); 
float cellMSCoreGetVolume1 (int chNo, const CELL_MS_PATH type, const CELL_MS_SPEAKERS speakerNo, const CELL_MS_CHANNELS channelNo);
float *cellMSCoreGetVolume8 (int chNo, const CELL_MS_PATH type, const CELL_MS_CHANNELS channelNo);
float *cellMSCoreGetVolume64 (int chNo, const CELL_MS_PATH type);
float  *cellMSCoreGetEQ (int chNo); 
CELL_MS_EQSTATE  cellMSCoreGetEQEnable(int chNo, const CELL_MS_PATH type);
CELL_MS_BYPASS  cellMSCoreGetBypass (int chNo, const CELL_MS_PATH type); 
CELL_MS_PAUSE  cellMSCoreGetPause(int chNo);
int cellMSCoreGetDSPHandle(int chNo, const CELL_MS_DSPSLOTS slot);
void  *cellMSCoreGetDSPParamAddr (int chNo, const CELL_MS_DSPSLOTS slot);
void  cellMSCoreStop (int chNo, const int flag);
int cellMSCoreGetRouting(int busNo, int destBus, int *dryStatus, int *wetStatus, float *dryVolume, float *wetVolume);
int cellMSCoreGetRouting8(int busNo, int destBus, int *dryStatus, int *wetStatus, float *dryVolumes, float *wetVolumes);

/**********************************************************************************************************
E Q
**********************************************************************************************************/
int  cellMSEQGetFFTBand(float hz);
int  cellMSEQSetPoint(CellEQSection *section, int index, float x, float y);
int  cellMSEQOffsetPoint(CellEQSection *section, int index, float offX, float offY);
CellEQPoint  *cellMSEQGetPoint(CellEQSection *section, int index);
int  cellMSEQGetNumberOfPoints(CellEQSection *section);
int  cellMSEQGet512Points(CellEQSection *section, float *values);
int  cellMSEQInitMorphEQ(CellEQMorphSection *mSection, CellEQSection *start, CellEQSection *end);
int  cellMSEQGet512PointsMorphed(CellEQMorphSection *mSection, float pos, float *values);
int  cellMSEQSetTag(CellEQSection *section, unsigned char *tag, float x, float y);
int  cellMSEQOffsetTag(CellEQSection *section, unsigned char *tag, float offX, float offY);


/**********************************************************************************************************
E N V E L O P E
**********************************************************************************************************/
int  cellMSEnvSetup (const int streamNo, CellMSEnvelope *env); 


/**********************************************************************************************************
D S P   H A N D L I N G
**********************************************************************************************************/
int  cellMSDSPResetPage (const int pageNum);
int  cellMSDSPLoadDSPFromMemory (const void *DSPEffect, CellMSDSP *user, const int handle, const int page); 
int  cellMSDSPLoadDSPFromFile (const char *DSPName, const void *WS, CellMSDSP *user, const int handle, const int page);
int  cellMSDSPCalcMemoryNeededForChunks (const int sizeOfFileInBytes); 
int  cellMSDSPCreateFFTChunks (short *sampleData, const int fileSize, CellMSComplexNum *chunks, int flag);


/**********************************************************************************************************
S U R R O U N D   F U N C T I O N A L I T Y
**********************************************************************************************************/
int  cellMSSurroundInit (void *memoryHeap, int sourceMax, int listenerMax );
int  cellMSSurroundClose (void);
int  cellMSSurroundGetInfoSize (int sourceMax, int listenerMax );
int  cellMSSurroundInitSource (unsigned int ch);
int  cellMSSurroundInitListener (unsigned int listener);
int  cellMSSurroundSetListener (unsigned int ch, unsigned int listener);
int  cellMSSurroundActive (unsigned int ch, int flag, unsigned int listener, CELL_MS_PATH type);
int  cellMSSurroundSourcef (unsigned int ch, int eParam, float flValue );
int  cellMSSurroundSource3f (unsigned int ch, int eParam, float flValue1, float flValue2, float flValue3);
int  cellMSSurroundSourcefv (unsigned int ch, int eParam, float *pflValues);
int  cellMSSurroundListenerf (unsigned int listener, int eParam, float flValue);
int  cellMSSurroundListener3f (unsigned int listener, int eParam, float flValue1, float flValue2, float flValue3);
int  cellMSSurroundListenerfv (unsigned int listener, int eParam, float *pflValues);
int  cellMSSurroundDopplerFactor (float value);
int  cellMSSurroundDopplerVelocity (float value);
int  cellMSSurroundSpeedOfSound (float flSpeedOfSound);
int  cellMSSurroundDistanceModel (unsigned int value);
int  cellMSSurroundSetAutoUpdate (int type);
int  cellMSSurroundUpdate (void);
int  cellMSSurroundApplyToStreams (void);
int cellMSSurroundConfig(unsigned int Output, unsigned int Layout);
int cellMSSurroundSourceOutput(unsigned int ch, unsigned int Output);
int cellMSSurroundSourceRollOff (unsigned int ch, unsigned int points, CellMSSurroundPoint *data);
int cellMSSurroundSourceDistanceModel(unsigned int ch, unsigned int value);

/**********************************************************************************************************
A T R A C
**********************************************************************************************************/
int  cellMSATRACGetNeededMemorySize (int channels); 
int  cellMSATRACInit (int channels, void *ATRACMemory); 
void  cellMSATRACClose (void); 

/**********************************************************************************************************
 M P 3
**********************************************************************************************************/
int cellMSMP3GetNeededMemorySize(int channels);
int cellMSMP3Init(int channels, void *memory);
int cellMSMP3GetFrameInfo(void* data,CellMSMP3FrameHeader *hdr);

/**********************************************************************************************************
S U L P H A 
**********************************************************************************************************/
int cellMSSulphaInit(void *memoryAddr, int memorySize, int namedObjects);
void cellMSSulphaShutdown(void);
int cellMSSulphaFileConnect(const char *fileName, const CellMSSulphaConfiguration *configuration);
int cellMSSulphaFileDisconnect(void);
int cellMSSulphaDECI3Start(void);
int cellMSSulphaSetDefaultConfig(CellMSSulphaConfiguration *config);
int cellMSSulphaGetStatus(void);

int cellMSSulphaNameObject(int type, int location, const char *Name);
int cellMSSulphaClearName(int ID);
int cellMSSulphaNameStaticSample(void *location, int size, const char *Name);
int cellMSSulphaAddComment(const char *str);

int cellMSSulphaAdd3dShape(CellMSSulpha3dObject *object);
void cellMSSulphaDelete3dShape(int id);

/**********************************************************************************************************
E Q  D S P
**********************************************************************************************************/
int cellMSFXEQInit(void *addr, float *buffer);
int cellMSFXEQSet(void *addr, float *buffer, int slot);
float  *cellMSFXEQGet(void *addr, int slot);


/**********************************************************************************************************
R I N G M O D  D S P
**********************************************************************************************************/
int cellMSFXRingModSetFrequency(void *addr, float hz, int slot);
int cellMSFXRingModSetAmplitude(void *addr, float amplitude, int slot);
int  cellMSFXRingModInit(void *addr, float hz, float amplitude);
int  cellMSFXRingModGetFrequency(void *addr, float *hz, int slot);
int  cellMSFXRingModGetAmplitude(void *addr, float *amplitude, int slot);

/**********************************************************************************************************
F I L T E R   D S P
**********************************************************************************************************/
int  cellMSFXFilterInit(void *paramList, void *pFilterMemBlock, const CellMSFXFilter *pSettings);
int  cellMSFXFilterSet(void *paramList, const CellMSFXFilter *pSettings, int slot);
int  cellMSFXFilterGet(void *addr, CellMSFXFilter *pSettings, int slot);
int  cellMSFXFilterGetNeededMemorySize(const int maxChannel);

/**********************************************************************************************************
P I T C H S H I F T   D S P
**********************************************************************************************************/
int  cellMSFXPitchShiftInit(void *paramList, void *pitchMemBlock, float pitch, int numActivePitches);
int  cellMSFXPitchShiftSet(void *paramList, float cents, int slot);
int  cellMSFXPitchShiftGet(void *paramList, float *cents, int slot);
int  cellMSFXPitchShiftGetNeededMemorySize(int numActivePitches);

/**********************************************************************************************************
D I S T O R T I O N  D S P
**********************************************************************************************************/
int cellMSFXDistortionInit(void *addr, CellMSFXDistortion *data);
int cellMSFXDistortionSet(void *addr, CellMSFXDistortion *data);
int cellMSFXDistortionGet(void *addr, CellMSFXDistortion *data);

/**********************************************************************************************************
C O M P R E S S O R   D S P
**********************************************************************************************************/
int cellMSFXCompressorInitCalc(void *addr, CellMSFXCompressorInfo *buffer);
int cellMSFXCompressorInitApply(void *addr, CellMSFXCompressorInfo *buffer);

/**********************************************************************************************************
M U L T I - C H A N N E L   C O M P R E S S O R   D S P
**********************************************************************************************************/
int cellMSFXMultiCompressorInitCalc( void *addr, CellMSFXCompressorInfo *pBuffers, const int nChannels);
int cellMSFXMultiCompressorInitApply(void *addr, CellMSFXCompressorInfo *pBuffers, const int nChannels );
int cellMSFXMultiCompressorApplyAddInput(void *addr, const int nApplyChannel, const int nCalcChannel );
int cellMSFXMultiCompressorApplyResetInputs(void *addr, const int nApplyChannel );

/**********************************************************************************************************
I R E V E R B   D S P
**********************************************************************************************************/
int cellMSFXIRDSPInfoGetNeededMemorySize(int num_impulses, void **impulses);
int cellMSFXIRDSPImpulseGetNeededMemorySize(int fileSize);
int cellMSFXIRDSPCreateImpulse(void *impulse_mem, int fileSize, char *fileData, int endian);
int cellMSFXIRDSPInitInfo(void *reverb_dsp_info, int num_impulses, void **array_impulses);
int cellMSFXIRDSPInit(void *paramList, void *info);
int  cellMSFXIRDSPClearAccumBuffers(void *reverb_dsp_info);

/**********************************************************************************************************
V O C O D E R   D S P
**********************************************************************************************************/

int  cellMSFXVocoderSetAllBandsBypass(void *addr, long long switches);
int  cellMSFXVocoderInitCarrier(void *addr, CellMSFXVocoderInfo *buffer);
int  cellMSFXVocoderInitModulator(void *addr, CellMSFXVocoderInfo *buffer);
int  cellMSFXVocoderSetEQBand(void *addr, unsigned int band, float value);
int  cellMSFXVocoderSetAllEQBands(void *addr, float *values);
int  cellMSFXVocoderSetBandBypass(void *addr, unsigned int band, unsigned char vSwitch);
int  cellMSFXVocoderSetModulatorVolume(void *addr, float volume);

/**********************************************************************************************************
T I M E   D O M A I N   R E V E R B   D S P
**********************************************************************************************************/
int cellMSFXReverbInit(void *addr, void *pReverbMemBlock, const CellMSFXReverbParams *pParams, int numActiveReverbChannels);
int cellMSFXReverbSetParams(void *pReverbMemBlock, const CellMSFXReverbParams *pParams, int slot);
int cellMSFXReverbGetNeededMemorySize(int numReverbs);

/**********************************************************************************************************
I 3 D L 2   R E V E R B   D S P
**********************************************************************************************************/
int cellMSFXI3DL2Init(void* addr, void* pAllocatedMemory, const CellMSFXI3DL2Params* pParams, const int numChannels);
int cellMSFXI3DL2SetParams(void* addr, void* pAllocatedMemory, const CellMSFXI3DL2Params* pParams, const int slot);
int cellMSFXI3DL2GetNeededMemorySize(const int numChannels);


/**********************************************************************************************************
D E L A Y   D S P
**********************************************************************************************************/
int cellMSFXDelayInit(void *paramList, void *pDelayMemBlock, const CellMSFXDelay *pParams, int numActiveDelayChannels);
int cellMSFXDelaySetParams(void *pDelayMemBlock, const CellMSFXDelay *pParams, int nSlot);
int cellMSFXDelayGetNeededMemorySize(int numDelays);

/**********************************************************************************************************
P A R A M E T R I C   E Q   D S P
**********************************************************************************************************/

int  cellMSFXParaEQInit(void *paramList, void *pParaEQMemBlock, const CellMSFXParaEQ *pSettings, const int numActiveChannels);
int  cellMSFXParaEQSet(void *paramList, const CellMSFXParaEQ *pSettings, int slot);
int  cellMSFXParaEQGetNeededMemorySize(const int maxChannel);
int  cellMSFXParaEQSetLFE(void* paramList, const int slot, const CELL_MSFX_FILTERMODE nLFE_Mode );

/**********************************************************************************************************
S O F T   C L I P P I N G   D I S T O R T I O N   D S P
**********************************************************************************************************/
int cellMSFXSoftClipInit( void* paramList, const CellMSFXSoftClipper* pSettings, const int nChannels );
int cellMSFXSoftClipSet( void* paramList, const CellMSFXSoftClipper* pSettings, const int nSlot );

/**********************************************************************************************************
A U X I L I A R Y   S E N D   A N D   R E T U R N   D S P
**********************************************************************************************************/
int cellMSFXAuxSendGetNeededMemorySize( const CellMSFXAuxSend* pInfo );
int cellMSFXAuxSendInit( void* paramList, void* pAuxSendMemory, const CellMSFXAuxSend* pInfo );
int cellMSFXAuxSendSet( void* pAuxSendMemory, const int nSendChannel, const int nSendIndex, const void* pAuxReturnMemory, const int nReturnChannel, const float fVolume );
int cellMSFXAuxSendSetVolumes( void* pAuxSendMemory, const int nSendChannel, const float* pfVolumes, const int nVolumes );
int cellMSFXAuxReturnGetNeededMemorySize( const int nReturnChannels );
int cellMSFXAuxReturnInit( void* paramList, void* pAuxReturnMemory, const int nReturnChannels, const int nCoreId );
int cellMSFXAuxReturnSetThruMix( void* paramList, const int nReturnChan, const float fThruMix );

/**********************************************************************************************************
D S P   H E L P E R   F U N C T I O N
**********************************************************************************************************/
int cellMSFXSetupSlotMasks( const int nCore, const CELL_MS_DSPSLOTS nDspSlot, const CELL_MSFX_SLOTMASK_MODE eMode );

/**********************************************************************************************************
F D N   R E V E R B   D S P
**********************************************************************************************************/
int cellMSFXFdnInit(void* pParamAddrSlot1, void* pParamAddrSlot2, void* pAllocatedMemory, const CellMSFXI3DL2Params* pParams, const CELL_MSFX_SLOTMASK_MODE eChannelMode, const float fDryVolmB, const CellMSFXEarlyReflection* pReflectionPattern);
int cellMSFXFdnSetParams(void* pAllocatedMemory, const CellMSFXI3DL2Params* pParams, const int slot, const float fDryVolmB, const CellMSFXEarlyReflection* pReflectionPattern);
int cellMSFXFdnGetNeededMemorySize(const int numChannels, const int nLateReverb);

/**********************************************************************************************************
M E T E R   D S P
**********************************************************************************************************/
int cellMSFXMeterInit( void* pParamAddrSlot1, const int nChans, const float fThreshold, CellMSFXMeter* pOutputArray );

#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
}
#endif

#endif

