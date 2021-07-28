/* SCE CONFIDENTIAL
 PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2005 Sony Computer Entertainment Inc.
 * All Rights Reserved.
 */

#ifndef __CELL_SOUND_MIXER_DEFINE_H__
#define __CELL_SOUND_MIXER_DEFINE_H__

#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <sys/types.h>
#include <sys/sys_time.h>

#define	CELL_SURMIXER_OUTPUT_2CH   2
#define	CELL_SURMIXER_OUTPUT_6CH   6
#define	CELL_SURMIXER_OUTPUT_8CH   8


#define CELL_SURBUS_CH0							(0)
#define CELL_SURBUS_CH1							(1)
#define CELL_SURBUS_CH2							(2)
#define CELL_SURBUS_CH3							(3)
#define CELL_SURBUS_CH4							(4)
#define CELL_SURBUS_CH5							(5)
#define CELL_SURBUS_CH6							(6)
#define CELL_SURBUS_CH7							(7)
#define CELL_SURBUS_LEFT						CELL_SURBUS_CH0
#define CELL_SURBUS_RIGHT						CELL_SURBUS_CH1
#define CELL_SURBUS_CENTER						CELL_SURBUS_CH2
#define CELL_SURBUS_LFE							CELL_SURBUS_CH3
#define CELL_SURBUS_LEFT_SUR					CELL_SURBUS_CH4
#define CELL_SURBUS_RIGHT_SUR					CELL_SURBUS_CH5
#define CELL_SURBUS_LEFT_EXT					CELL_SURBUS_CH6
#define CELL_SURBUS_RIGHT_EXT					CELL_SURBUS_CH7
#define CELL_SURBUS_REVERB_LEFT					(8)
#define CELL_SURBUS_REVERB_RIGHT 				(9)
#define CELL_SURBUS_REVERB_LEFT_SUR				(10)
#define CELL_SURBUS_REVERB_RIGHT_SUR			(11)

#define CELL_SURMIXER_PARAM_TOTALLEVEL_LINEAR	(8)
#define CELL_SURMIXER_PARAM_REVERBLEVEL_LINEAR	(9)
#define CELL_SURMIXER_PARAM_TOTALMUTE			(12)

#define CELL_SURMIXER_PARAM_TOTALLEVEL			(40)
#define CELL_SURMIXER_PARAM_REVERBLEVEL			(41)
#define CELL_SURMIXER_PARAM_TOTALLEVEL_DB		(40)
#define CELL_SURMIXER_PARAM_REVERBLEVEL_DB		(41)

#define CELL_SURMIXER_CONT_MUTEON				(1.0)
#define CELL_SURMIXER_CONT_MUTEOFF				(0.0)
#define CELL_SURMIXER_CONT_DBSWITCHON			(1)
#define CELL_SURMIXER_CONT_DBSWITCHOFF			(0)
#define  CELL_SURMIXER_CONT_PAUSE_OFF               (0)
#define  CELL_SURMIXER_CONT_PAUSE_ON                (1)
#define  CELL_SURMIXER_CONT_PAUSE_ON_IMMEDIATE      (2)


#define CELL_SURMIXER_CHSTRIP_TYPE1A			(1)
#define CELL_SURMIXER_CHSTRIP_TYPE2A			(2)
#define CELL_SURMIXER_CHSTRIP_TYPE6A			(3)
#define CELL_SURMIXER_CHSTRIP_TYPE8A			(4)

#define CELL_SURMIXER_CH1PARAM_LEVEL			(0)
#define CELL_SURMIXER_CH1PARAM_EXPRESSIONLEVEL	(1)
#define CELL_SURMIXER_CH1PARAM_CENTERLEVEL		(2)
#define CELL_SURMIXER_CH1PARAM_REVERBSENDLEVEL	(3)
#define CELL_SURMIXER_CH1PARAM_MUTE				(4)
#define CELL_SURMIXER_CH1PARAM_REVSENDPOSITION	(5)
#define CELL_SURMIXER_CH1PARAM_POSITION			(6)

#define CELL_SURMIXER_CH2PARAM_LEVEL			(0)
#define CELL_SURMIXER_CH2PARAM_EXPRESSIONLEVEL	(1)
#define CELL_SURMIXER_CH2PARAM_CENTERLEVEL		(2)
#define CELL_SURMIXER_CH2PARAM_REVERBSENDLEVEL	(3)
#define CELL_SURMIXER_CH2PARAM_MUTE				(4)
#define CELL_SURMIXER_CH2PARAM_REVSENDPOSITION	(5)
#define CELL_SURMIXER_CH2PARAM_POSITION			(6)

#define CELL_SURMIXER_CH6PARAM_LEVEL			(0)
#define CELL_SURMIXER_CH6PARAM_EXPRESSIONLEVEL	(1)
#define CELL_SURMIXER_CH6PARAM_REVERBSENDLEVEL	(2)
#define CELL_SURMIXER_CH6PARAM_CENTER_REVERBSENDLEVEL (3)
#define CELL_SURMIXER_CH6PARAM_MUTE				(4)

#define CELL_SURMIXER_CH8PARAM_LEVEL			(0)
#define CELL_SURMIXER_CH8PARAM_EXPRESSIONLEVEL	(1)
#define CELL_SURMIXER_CH8PARAM_REVERBSENDLEVEL	(2)
#define CELL_SURMIXER_CH8PARAM_CENTER_REVERBSENDLEVEL (3)
#define CELL_SURMIXER_CH8PARAM_MUTE				(4)


#define CELL_SURMIXER_CH1CONT_MUTE_OFF			(0)
#define CELL_SURMIXER_CH1CONT_MUTE_ON			(1)
#define CELL_SURMIXER_CH2CONT_MUTE_OFF			(0)
#define CELL_SURMIXER_CH2CONT_MUTE_ON			(1)
#define CELL_SURMIXER_CH6CONT_MUTE_OFF			(0)
#define CELL_SURMIXER_CH6CONT_MUTE_ON			(1)
#define CELL_SURMIXER_CH8CONT_MUTE_OFF			(0)
#define CELL_SURMIXER_CH8CONT_MUTE_ON			(1)

#define CELL_SSPLAYER_ONESHOT					(0)
#define CELL_SSPLAYER_ONESHOT_CONT				(2)
#define CELL_SSPLAYER_LOOP_ON					(16)

#define CELL_SSPLAYER_STATE_ERROR		(0xffffffff)
#define CELL_SSPLAYER_STATE_NOTREADY	(0x88888888)
#define CELL_SSPLAYER_STATE_OFF			(0x00)
#define CELL_SSPLAYER_STATE_PAUSE		(0x01)
#define CELL_SSPLAYER_STATE_CLOSING		(0x08)
#define CELL_SSPLAYER_STATE_ON			(0x20)


#define CELL_SURMIXER_CENT2RATIO(x)	(pow(2.0,(x)/1200.0))
#define CELL_SURMIXER_SHORT2FLOAT	(1.0/32768.0)


#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
extern "C" {
#endif


#ifndef __CELL_AAN_HANDLE
#define __CELL_AAN_HANDLE
typedef void * CellAANHandle ;
#endif /* __CELL_AAN_HANDLE */



typedef int (*CellSurMixerNotifyCallbackFunction)(void *, uint32_t, uint32_t);

typedef struct CellSurMixerConfig {
	int32_t		priority;
	uint32_t	chStrips1;
	uint32_t	chStrips2;
	uint32_t	chStrips6;
	uint32_t	chStrips8;
} CellSurMixerConfig;

typedef struct CellSurMixerPosition {
	float	x;
	float	y;
	float	z;
} CellSurMixerPosition;

typedef struct CellSurMixerChStripParam {
	uint32_t	param;
	void		*attribute;
	int			dBSwitch;
	float		floatVal;
	int			intVal;
} CellSurMixerChStripParam;

typedef struct CellSSPlayerConfig {
	uint32_t	channels;
	uint32_t	outputMode;
} CellSSPlayerConfig;

typedef struct CellSSPlayerWaveParam {
	void		*addr;
	int			format;
	uint32_t	samples;
	uint32_t	loopStartOffset;
	uint32_t	startOffset;
} CellSSPlayerWaveParam;

typedef struct CellSSPlayerCommonParam {
	uint32_t	loopMode;
	uint32_t	attackMode;
} CellSSPlayerCommonParam;

typedef CellSurMixerPosition CellSSSPlayerPosition;

typedef struct CellSSPlayerRuntimeInfo {
	float	level;
	float	speed;
	CellSurMixerPosition position;
} CellSSPlayerRuntimeInfo;

extern int cellAANConnect(CellAANHandle receive,
						  uint32_t receivePortNo,
						  CellAANHandle source,
						  uint32_t sourcePortNo);
extern int cellAANDisconnect(CellAANHandle receive,
						  uint32_t receivePortNo,
						  CellAANHandle source,
						  uint32_t sourcePortNo);
extern int cellAANAddData(CellAANHandle handle,
						  uint32_t port,
						  uint32_t offset,
						  float *addr,
						  uint32_t samples);

extern int cellSurMixerCreate(const CellSurMixerConfig *config);
extern int cellSurMixerGetAANHandle(CellAANHandle *handle);
extern int cellSurMixerChStripGetAANPortNo(unsigned int *portNo,
										   uint32_t type,
										   unsigned int order);
extern int cellSurMixerSetNotifyCallback(CellSurMixerNotifyCallbackFunction f,
										 void *myAttribute);
extern int cellSurMixerRemoveNotifyCallback(CellSurMixerNotifyCallbackFunction f);
extern int cellSurMixerSurBusAddData(uint32_t busNo,
									 uint32_t offset,
									 float *addr,
									 uint32_t samples);
extern int cellSurMixerStart(void);
extern int cellSurMixerFinalize (void);
extern int cellSurMixerSetParameter(uint32_t param, float value);
extern int cellSurMixerChStripSetParameter(uint32_t type,
										   uint32_t index,
										   CellSurMixerChStripParam *param);
extern void cellSurMixerBeep(void *arg);
extern int  cellSurMixerPause(uint32_t sw );

extern int cellSurMixerGetCurrentBlockTag(uint64_t *tag );
int cellSurMixerGetTimestamp(uint64_t tag, usecond_t *stamp );


extern int cellSSPlayerCreate(CellAANHandle *handle,
							  CellSSPlayerConfig *config);
extern int cellSSPlayerSetWave(CellAANHandle handle,
							   CellSSPlayerWaveParam *waveInfo,
							   CellSSPlayerCommonParam *commonInfo);
extern int cellSSPlayerSetParam(CellAANHandle handle,
								CellSSPlayerRuntimeInfo *arg);
extern int cellSSPlayerPlay(CellAANHandle handle,
							CellSSPlayerRuntimeInfo *info);
extern int cellSSPlayerStop(CellAANHandle handle,
							uint32_t mode);
extern int32_t cellSSPlayerGetState( CellAANHandle handle );
extern int cellSSPlayerRemove(CellAANHandle handle);


extern float cellSurMixerUtilGetLevelFromDB(float dB);
extern float cellSurMixerUtilGetLevelFromDBIndex(int index);
extern float cellSurMixerUtilNoteToRatio(unsigned char refNote, unsigned char note);

#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
}
#endif


/* cellSurMixerUtilGetLevelFromDBIndex():
 *    This module provide dB-like, log-scale, value for audio application.
 *    Argument and return are actual values.
 *   (offset) and (dB) are just notes */
/*
  argument (offset)         return      (dB)
  +-------+--------+---------------+---------+
       511      175    1.961172e+03    65.85
        :        :              :
       368       32    4.000000e+00    12.04
        :        :              :
       352       16    2.000000e+00     6.02
        :        :              :
       338        2    1.090508e+00     0.75
       337        1    1.044274e+00     0.38
    *  336        0    1.000000e+00     0.00
       335       -1    9.576033e-01    -0.38
       334       -2    9.170040e-01    -0.75
        :        :              :
       320      -16    5.000000e-01    -6.02
        :        :              :
       304      -32    2.500000e-01   -12.04
        :        :              :
         1     -335    4.979485e-07  -126.05
         0     -336    0.0              -inf
        -1     +335   -4.979485e-07  -126.05
        :        :              :
      -304      +32   -2.500000e-01   -12.04
        :        :              :
      -320      +16   -5.000000e-01    -6.02
        :        :              :
      -334        2   -9.170040e-01    -0.75
      -335        1   -9.576033e-01    -0.38
    * -336        0   -1.000000e+00     0.00
      -337       -1   -1.044274e+00     0.38
      -338       -2   -1.090508e+00     0.75
        :        :              :
      -352      -16   -2.000000e+00     6.02
        :        :              :
      -368      -32   -4.000000e+00    12.04
        :        :              :
      -511     -175   -1.961172e+03    65.85
  +-------+--------+---------------+---------+
*/

/*
 * Local variables:
 * indent-tabs-mode: t
 * tab-width: 4
 * End:
 */
#endif /* __CELL_SOUND_MIXER_DEFINE_H__ */
