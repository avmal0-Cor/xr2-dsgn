/*****************************************************************
 * SCE CONFIDENTIAL
 PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (c) 2009 Sony Computer Entertainment Inc.
 * All Rights Reserved.
 *
 * R & D Division, Sony Computer Entertainment Inc.
 *****************************************************************/

#ifndef __LIBMIC_INTERFACE__H
#define __LIBMIC_INTERFACE__H

#include <sys/types.h>
#include <sys/event.h>

#if defined(__LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
extern "C" {
#endif

// define maximum number of microphones can be possibly available
#define  CELL_MAX_MICS  MaxNumMicInputs  //(8)

/***
 *** attributes definition in public domain
 ***/
typedef enum CellMicDeviceAttr
{
    CELLMIC_DEVATTR_GAIN   = 10,    // 0..5 [4], the device analog preamp setting
    CELLMIC_DEVATTR_VOLUME = 201,   // 0..241 [145], 0=mute audio. the device digital volume control
    CELLMIC_DEVATTR_AGC    = 202,   // 0..241 [0], 0=disable AGC, the reference AGC volume
    CELLMIC_DEVATTR_CHANVOL= 301,   // arg1:chan,0..4, 0=all channels. arg2=0..100 [60], 0=mute audio. 
    CELLMIC_DEVATTR_LED    = 9,     // boolean 0/1 [0], the LED control, such as Eyetoy and Playstation3 Eye LED light control
    CELLMIC_DEVATTR_DSPTYPE= 302,   // arg1:the DSP signal type: 0=clean, 1=clock_synced_raw, 2=background noise, 4=echo noise, 8=linear_ec
} CellMicDeviceAttr;

typedef enum CellMicSignalAttr   
{             
    CELLMIC_SIGATTR_BKNGAIN   = 0,  // float, 0(db)..25(db). [6(db)], background noise level presented in signal (decible)
    CELLMIC_SIGATTR_REVERB    = 9,  // int, 24(ms)..2400(ms) [240ms], average audio reverberation time presented in signal (milli-second)
    CELLMIC_SIGATTR_AGCLEVEL  = 26, // int, 820..28000, [18000]. target audio volume level for automatic-gain-control(AGC)
    CELLMIC_SIGATTR_VOLUME    = 301,// int, 0...5 [4], 0=mute signal, the signal volume level control
    CELLMIC_SIGATTR_PITCHSHIFT= 331,// boolean 0/1 [1], switch to turn ON/OFF pitchshift that is adjusted by system "voice changer" menu
} CellMicSignalAttr;

typedef enum CellMicSignalState
{
    CELLMIC_SIGSTATE_LOCTALK = 0,  //int,   local voice detected if>0, 
    CELLMIC_SIGSTATE_FARTALK = 1,  //int,   far-end voice detected if > 0, 
    CELLMIC_SIGSTATE_NSR     = 3,  //float, noise-supression-ratio in decibel, 
    CELLMIC_SIGSTATE_AGC     = 4,  //float, target volume level by automatic gain control
    CELLMIC_SIGSTATE_MICENG  = 5,  //float, microphone signal energy in decibel
    CELLMIC_SIGSTATE_SPKENG  = 6,  //float, loudspeaker signal energy in decibel
} CellMicSignalState;

typedef enum CellMicType
{
    CELLMIC_TYPE_UNDEF     = -1,
    CELLMIC_TYPE_UNKNOWN   =  0,
    CELLMIC_TYPE_EYETOY1   =  1,
    CELLMIC_TYPE_EYETOY2   =  2,
    CELLMIC_TYPE_USBAUDIO  =  3,
    CELLMIC_TYPE_BLUETOOTH =  4,
    CELLMIC_TYPE_A2DP      =  5,
} CellMicType;

/***
 *** user interface declaration
 ***/

//init libmic client
int cellMicInit(void);

// shutdown libmic client
int cellMicEnd(void);

// get a global unique identifier for a device. Can be called even if device is not open
// return the device GUID to <ptr_devid>
int cellMicGetDeviceGUID( int dev_num, uint32_t* ptr_guid );

//Get device type: ( BTHeadset/Headset/Headset-earphone/USBCamera/ET1/ET2, etc...)
//return the type to <ptr_type>, it maybe "principle type" and "subclass-type" ?
int cellMicGetType( int dev_num,  int* ptr_type );

// query if microphone is attached
int cellMicIsAttached( int dev_num );

// query if microphone is open
int cellMicIsOpen( int dev_num );

//Get the device attribute
int cellMicGetDeviceAttr( int dev_num, CellMicDeviceAttr dev_attrib, int *arg1, int *arg2 );

//Set the device attribute
int cellMicSetDeviceAttr( int dev_num, CellMicDeviceAttr dev_attrib, int arg1, int arg2 );

//get the microphone signal processing attribute
int cellMicGetSignalAttr( int dev_num, CellMicSignalAttr sig_attrib, void* value );     

//Set the microphone signal processing attribute
int cellMicSetSignalAttr( int dev_num, CellMicSignalAttr sig_attrib, void* value );

//get the microphone signal runtime state 
int cellMicGetSignalState( int dev_num, CellMicSignalState sig_state, void* value );

//Open the Microphone channel and start the audio input stream
int cellMicOpen( int dev_num, int samprate );

//Stop the audio input stream and close the microphone channel
int cellMicClose( int dev_num);

// start streaming.  device compatible with open service must be attached
int cellMicStart( int dev_num );

// start streaming.  device compatible with open service must be attached
int cellMicStartEx( int dev_num, uint32_t iFlags );

// stop streaming
int cellMicStop( int dev_num ); 

// reset streaming
int cellMicReset( int dev_num ); 

// try to read <max_bytes> from input stream
// return actual bytes been copied to <data>
int cellMicRead( int dev_num, void* data, int max_bytes );

// Get input stream format
int cellMicGetFormat( int dev_num, CellMicInputFormatI* format );

// callback to capture audio input system events "CELLMIC_ATTACH,CELLMIC_DETACH,CELLMIC_SWITCH,CELLMIC_DATA"
int cellMicSetNotifyEventQueue( sys_ipc_key_t );
int cellMicSetNotifyEventQueue2( sys_ipc_key_t key, uint64_t source, uint64_t flag );
int cellMicRemoveNotifyEventQueue( sys_ipc_key_t );

/***
 *** Extended interface declaration for advanced user (such as speech recognition, sound tracking/detection, etc...) 
 ***/
typedef enum CellMicSignalType
{
    CELLMIC_SIGTYPE_NULL  = 0x00,  //null signal type(no signal output, user may want to get signal state but not signal itself)
    CELLMIC_SIGTYPE_DSP   = 0x01,  //signal that is after digital-signal-processing(DSP)/filtering to raw data
    CELLMIC_SIGTYPE_AUX   = 0x02,  //auxilary signals(such as: template/reference/modulation signals) 
    CELLMIC_SIGTYPE_RAW   = 0x04,  //raw data(no DSP or filtering to orginal data, but the data might be restructured)
} CellMicSignalType;

//Open device with fine control (streaming buffer size, device raw data sampling rate, types of streaming signals, etc..)
int cellMicOpenEx( int dev_num, int raw_samprate, int raw_channel, int dsp_samprate, int bufms, CellMicSignalType sigtype );

//for microphone device raw data
int cellMicOpenRaw( int dev_num, int samprate, int channel);

//read microphone raw data
//this data is provided if <CELLMIC_SIGTYPE_RAW> is given in <cellMicOpenEx> function, or uses <cellMicOpenRaw>
int cellMicReadRaw( int dev_num, void* data, int max_bytes );

//Read auxilary data (such as: such as: template/reference/modulation signals)
//this data is provided if <CELLMIC_SIGTYPE_AUX> is given in <cellMicOpenEx> function
int cellMicReadAux( int dev_num, void* data, int max_bytes );

//Read data that is after digital-signal-processing(DSP)/filtering to raw data
//this data is provided if <CELLMIC_SIGTYPE_DSP> is given in <cellMicOpenEx> function, or uses <cellMicOpen>
int cellMicReadDsp( int dev_num, void* data, int max_bytes );

// Get microphone raw data format
// This format is valid if <CELLMIC_SIGTYPE_RAW> flag is given in <cellMicOpenEx> function, or uses <cellMicOpenRaw>
int cellMicGetFormatRaw( int dev_num, CellMicInputFormatI* format );

// Get auxilary stream format
// This format is valid if <CELLMIC_SIGTYPE_AUX> flag is given in <cellMicOpenEx> function
int cellMicGetFormatAux( int dev_num, CellMicInputFormatI* format );

// Get dsp stream format
// This format is valid if <CELLMIC_SIGTYPE_DSP> flag is given in <cellMicOpenEx> function, or uses <cellMicOpen>
int cellMicGetFormatDsp( int dev_num, CellMicInputFormatI* format );

/***
 *** Get MIC input status without open the device
 ***/
typedef struct CellMicStatus
{
    int raw_samprate;   /* the RAW data stream sampling rate */
    int dsp_samprate;   /* the DSP data stream sampling rate */
    int dsp_volume;     /* the DSP data stream volume set by application, range=[0, 5] */
    int isStart;        /* true(1) if audio streaming is started, else false(0) */
    int isOpen;         /* true(1) if this device is already opened by client, else false(0) */
    int local_voice;    /* confidence score if there is local voice detected in microphone channel, range=[0, 10] */
    int remote_voice;   /* confidence score if there is remote voice detected in audio output channel, range=[0, 10] */
    float mic_energy;   /* signal strength in decibel in microphone channel */
    float spk_energy;   /* signal strength in decibel in audio output channel */
} CellMicStatus; 

// Get MicIn status without open the device, used by system utility setting to monitor device status in runtime
int cellMicGetStatus( int dev_num, CellMicStatus* status);
//////////////////////////////////////////////////////////////////////////////////////////////
#if defined(__LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
}
#endif
/* end of microphone Input Interface */
#endif // __LIBMIC_INTERFACE__H

