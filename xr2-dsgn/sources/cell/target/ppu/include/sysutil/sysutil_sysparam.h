/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 320.001
 * Copyright (C) 2009 Sony Computer Entertainment Inc.
 * All Rights Reserved.
 */
#ifndef _CELL_SYSUTIL_SYSPARAM_H_
#define _CELL_SYSUTIL_SYSPARAM_H_

#include "sysutil_common.h"

#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
extern "C" {
#endif  /* defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus) */

#define CELL_SYSUTIL_SYSTEMPARAM_ID_LANG							(0x0111)
#define CELL_SYSUTIL_SYSTEMPARAM_ID_ENTER_BUTTON_ASSIGN				(0x0112)
#define CELL_SYSUTIL_SYSTEMPARAM_ID_NICKNAME						(0x0113)
#define CELL_SYSUTIL_SYSTEMPARAM_ID_DATE_FORMAT						(0x0114)
#define CELL_SYSUTIL_SYSTEMPARAM_ID_TIME_FORMAT						(0x0115)
#define CELL_SYSUTIL_SYSTEMPARAM_ID_TIMEZONE						(0x0116)
#define CELL_SYSUTIL_SYSTEMPARAM_ID_SUMMERTIME						(0x0117)
#define CELL_SYSUTIL_SYSTEMPARAM_ID_GAME_PARENTAL_LEVEL				(0x0121)
#define CELL_SYSUTIL_SYSTEMPARAM_ID_GAME_PARENTAL_LEVEL0_RESTRICT	(0x0123)
#define CELL_SYSUTIL_SYSTEMPARAM_ID_INTERNET_BROWSER_START_RESTRICT	(0x0125)
#define CELL_SYSUTIL_SYSTEMPARAM_ID_CURRENT_USERNAME				(0x0131)
#define CELL_SYSUTIL_SYSTEMPARAM_ID_CURRENT_USER_HAS_NP_ACCOUNT		(0x0141)
#define CELL_SYSUTIL_SYSTEMPARAM_ID_CAMERA_PLFREQ					(0x0151)
#define CELL_SYSUTIL_SYSTEMPARAM_ID_PAD_RUMBLE						(0x0152)
#define CELL_SYSUTIL_SYSTEMPARAM_ID_KEYBOARD_TYPE					(0x0153)
#define CELL_SYSUTIL_SYSTEMPARAM_ID_JAPANESE_KEYBOARD_ENTRY_METHOD	(0x0154)
#define CELL_SYSUTIL_SYSTEMPARAM_ID_CHINESE_KEYBOARD_ENTRY_METHOD	(0x0155)

#define CELL_SYSUTIL_SYSTEMPARAM_NICKNAME_SIZE			(128)
#define CELL_SYSUTIL_SYSTEMPARAM_CURRENT_USERNAME_SIZE	(64)


/* --------------------------- Error Code ----------------------------- */
enum {
	/* #define CELL_SYSUTIL_ERROR_BASE_SYSTEMPARAM		(0x8002b200) */
	/* video */
	CELL_VIDEO_OUT_SUCCEEDED						= 0,
	CELL_VIDEO_OUT_ERROR_NOT_IMPLEMENTED			= (0x8002b220),
	CELL_VIDEO_OUT_ERROR_ILLEGAL_CONFIGURATION		= (0x8002b221),
	CELL_VIDEO_OUT_ERROR_ILLEGAL_PARAMETER			= (0x8002b222),
	CELL_VIDEO_OUT_ERROR_PARAMETER_OUT_OF_RANGE		= (0x8002b223),
	CELL_VIDEO_OUT_ERROR_DEVICE_NOT_FOUND			= (0x8002b224),
	CELL_VIDEO_OUT_ERROR_UNSUPPORTED_VIDEO_OUT		= (0x8002b225),
	CELL_VIDEO_OUT_ERROR_UNSUPPORTED_DISPLAY_MODE	= (0x8002b226),
	CELL_VIDEO_OUT_ERROR_CONDITION_BUSY				= (0x8002b227),
	CELL_VIDEO_OUT_ERROR_VALUE_IS_NOT_SET			= (0x8002b228),

	/* audio */
	CELL_AUDIO_OUT_SUCCEEDED						= 0,
	CELL_AUDIO_OUT_ERROR_NOT_IMPLEMENTED			= (0x8002b240),
	CELL_AUDIO_OUT_ERROR_ILLEGAL_CONFIGURATION		= (0x8002b241),
	CELL_AUDIO_OUT_ERROR_ILLEGAL_PARAMETER			= (0x8002b242),
	CELL_AUDIO_OUT_ERROR_PARAMETER_OUT_OF_RANGE		= (0x8002b243),
	CELL_AUDIO_OUT_ERROR_DEVICE_NOT_FOUND			= (0x8002b244),
	CELL_AUDIO_OUT_ERROR_UNSUPPORTED_AUDIO_OUT		= (0x8002b245),
	CELL_AUDIO_OUT_ERROR_UNSUPPORTED_SOUND_MODE		= (0x8002b246),
	CELL_AUDIO_OUT_ERROR_CONDITION_BUSY             = (0x8002b247),

	CELL_AUDIO_IN_SUCCEEDED							= 0,
	CELL_AUDIO_IN_ERROR_NOT_IMPLEMENTED				= (0x8002b260),
	CELL_AUDIO_IN_ERROR_ILLEGAL_CONFIGURATION		= (0x8002b261),
	CELL_AUDIO_IN_ERROR_ILLEGAL_PARAMETER			= (0x8002b262),
	CELL_AUDIO_IN_ERROR_PARAMETER_OUT_OF_RANGE		= (0x8002b263),
	CELL_AUDIO_IN_ERROR_DEVICE_NOT_FOUND			= (0x8002b264),
	CELL_AUDIO_IN_ERROR_UNSUPPORTED_AUDIO_IN		= (0x8002b265),
	CELL_AUDIO_IN_ERROR_UNSUPPORTED_SOUND_MODE		= (0x8002b266),
	CELL_AUDIO_IN_ERROR_CONDITION_BUSY				= (0x8002b267)

};

/* ----------------------------- Video -------------------------------- */

typedef enum CellVideoOut {
	CELL_VIDEO_OUT_PRIMARY,
	CELL_VIDEO_OUT_SECONDARY
} CellVideoOut;

typedef enum CellVideoOutResolutionId {
	CELL_VIDEO_OUT_RESOLUTION_UNDEFINED  = 0,
	CELL_VIDEO_OUT_RESOLUTION_1080       = 1,
	CELL_VIDEO_OUT_RESOLUTION_720        = 2,
	CELL_VIDEO_OUT_RESOLUTION_480        = 4,
	CELL_VIDEO_OUT_RESOLUTION_576        = 5,
	CELL_VIDEO_OUT_RESOLUTION_1600x1080  = 10,
	CELL_VIDEO_OUT_RESOLUTION_1440x1080  = 11,
	CELL_VIDEO_OUT_RESOLUTION_1280x1080  = 12,
	CELL_VIDEO_OUT_RESOLUTION_960x1080   = 13
	,
	CELL_VIDEO_OUT_RESOLUTION_720_3D_FRAME_PACKING        = 0x81,
	CELL_VIDEO_OUT_RESOLUTION_1024x720_3D_FRAME_PACKING   = 0x88,
	CELL_VIDEO_OUT_RESOLUTION_960x720_3D_FRAME_PACKING    = 0x89,
	CELL_VIDEO_OUT_RESOLUTION_800x720_3D_FRAME_PACKING    = 0x8a,
	CELL_VIDEO_OUT_RESOLUTION_640x720_3D_FRAME_PACKING    = 0x8b
} CellVideoOutResolutionId;

typedef enum CellVideoOutScanMode {
	CELL_VIDEO_OUT_SCAN_MODE_INTERLACE,
	CELL_VIDEO_OUT_SCAN_MODE_PROGRESSIVE
} CellVideoOutScanMode;

typedef enum CellVideoOutScanMode2 {
	CELL_VIDEO_OUT_SCAN_MODE2_AUTO,
	CELL_VIDEO_OUT_SCAN_MODE2_INTERLACE,
	CELL_VIDEO_OUT_SCAN_MODE2_PROGRESSIVE
} CellVideoOutScanMode2;

typedef enum CellVideoOutRefreshRate {
	CELL_VIDEO_OUT_REFRESH_RATE_AUTO    = 0x0000,
	CELL_VIDEO_OUT_REFRESH_RATE_59_94HZ = 0x0001,
	CELL_VIDEO_OUT_REFRESH_RATE_50HZ    = 0x0002,
	CELL_VIDEO_OUT_REFRESH_RATE_60HZ    = 0x0004,
	CELL_VIDEO_OUT_REFRESH_RATE_30HZ    = 0x0008
} CellVideoOutRefreshRate;

typedef enum CellVideoOutPortType {
	CELL_VIDEO_OUT_PORT_NONE          = 0x00,
	CELL_VIDEO_OUT_PORT_HDMI          = 0x01,
	CELL_VIDEO_OUT_PORT_NETWORK       = 0x41,
	CELL_VIDEO_OUT_PORT_COMPOSITE_S   = 0x81,
	CELL_VIDEO_OUT_PORT_D             = 0x82,
	CELL_VIDEO_OUT_PORT_COMPONENT     = 0x83,
	CELL_VIDEO_OUT_PORT_RGB           = 0x84,
	CELL_VIDEO_OUT_PORT_AVMULTI_SCART = 0x85,
	CELL_VIDEO_OUT_PORT_DSUB          = 0x86
} CellVideoOutPortType;

typedef enum CellVideoOutDisplayAspect {
    CELL_VIDEO_OUT_ASPECT_AUTO,
	CELL_VIDEO_OUT_ASPECT_4_3,
	CELL_VIDEO_OUT_ASPECT_16_9
} CellVideoOutDisplayAspect;

typedef enum CellVideoOutBufferColorFormat {
	CELL_VIDEO_OUT_BUFFER_COLOR_FORMAT_X8R8G8B8,
	CELL_VIDEO_OUT_BUFFER_COLOR_FORMAT_X8B8G8R8,
	CELL_VIDEO_OUT_BUFFER_COLOR_FORMAT_R16G16B16X16_FLOAT
} CellVideoOutBufferColorFormat;

typedef enum CellVideoOutOutputState {
	CELL_VIDEO_OUT_OUTPUT_STATE_ENABLED,
	CELL_VIDEO_OUT_OUTPUT_STATE_DISABLED,
	CELL_VIDEO_OUT_OUTPUT_STATE_PREPARING
} CellVideoOutOutputState;

typedef enum CellVideoOutDeviceState {
	CELL_VIDEO_OUT_DEVICE_STATE_UNAVAILABLE,
	CELL_VIDEO_OUT_DEVICE_STATE_AVAILABLE,
} CellVideoOutDeviceState;

typedef enum CellVideoOutColorSpace {
	CELL_VIDEO_OUT_COLOR_SPACE_RGB   = 0x01,
	CELL_VIDEO_OUT_COLOR_SPACE_YUV   = 0x02,
	CELL_VIDEO_OUT_COLOR_SPACE_XVYCC = 0x04
} CellVideoOutColorSpace;

typedef enum CellVideoOutDebugMonitorType {
	CELL_VIDEO_OUT_DEBUG_MONITOR_TYPE_UNDEFINED     = 0,
	CELL_VIDEO_OUT_DEBUG_MONITOR_TYPE_480I_59_94HZ  = 1,
	CELL_VIDEO_OUT_DEBUG_MONITOR_TYPE_576I_50HZ     = 2,
	CELL_VIDEO_OUT_DEBUG_MONITOR_TYPE_480P_59_94HZ  = 3,
	CELL_VIDEO_OUT_DEBUG_MONITOR_TYPE_576P_50HZ     = 4,
	CELL_VIDEO_OUT_DEBUG_MONITOR_TYPE_1080I_59_94HZ = 5,
	CELL_VIDEO_OUT_DEBUG_MONITOR_TYPE_720P_59_94HZ  = 7,
	CELL_VIDEO_OUT_DEBUG_MONITOR_TYPE_1080P_59_94HZ = 9,
	CELL_VIDEO_OUT_DEBUG_MONITOR_TYPE_WXGA_60HZ     = 11,
	CELL_VIDEO_OUT_DEBUG_MONITOR_TYPE_SXGA_60HZ     = 12,
	CELL_VIDEO_OUT_DEBUG_MONITOR_TYPE_WUXGA_60HZ    = 13
} CellVideoOutDebugMonitorType;

typedef struct CellVideoOutColorInfo {
	uint16_t                      redX;
	uint16_t                      redY;
	uint16_t                      greenX;
	uint16_t                      greenY;
	uint16_t                      blueX;
	uint16_t                      blueY;
	uint16_t                      whiteX;
	uint16_t                      whiteY;
	uint32_t                      gamma;
} CellVideoOutColorInfo;

typedef struct CellVideoOutKSVList {
	uint8_t                       ksv[32*5];
	uint8_t                       reserved[4];
	uint32_t                      count;
} CellVideoOutKSVList;

typedef enum CellVideoOutDisplayConversion {
	CELL_VIDEO_OUT_DISPLAY_CONVERSION_NONE			= 0x00,
	CELL_VIDEO_OUT_DISPLAY_CONVERSION_TO_WXGA	    = 0x01,
	CELL_VIDEO_OUT_DISPLAY_CONVERSION_TO_SXGA       = 0x02,
	CELL_VIDEO_OUT_DISPLAY_CONVERSION_TO_WUXGA	    = 0x03,
	CELL_VIDEO_OUT_DISPLAY_CONVERSION_TO_1080	    = 0x05,
	CELL_VIDEO_OUT_DISPLAY_CONVERSION_TO_REMOTEPLAY = 0x10
#if defined(_ENABLE_3D_VIDEO_OUT)
	,
	CELL_VIDEO_OUT_DISPLAY_CONVERSION_TO_720_3D_FRAME_PACKING = 0x80
#endif //defined(_ENABLE_3D_VIDEO_OUT)
} CellVideoOutDisplayConversion;

typedef struct CellVideoOutDisplayMode {
	uint8_t                       resolutionId;
	uint8_t                       scanMode;
	uint8_t                       conversion;
	uint8_t                       aspect;
	uint8_t                       reserved[2];
	uint16_t                      refreshRates;
} CellVideoOutDisplayMode;

typedef struct CellVideoOutResolution {
	uint16_t                      width;
	uint16_t                      height;
} CellVideoOutResolution;

typedef struct CellVideoOutDeviceInfo {
	uint8_t                       portType;
	uint8_t                       colorSpace;
	uint16_t                      latency;
	uint8_t                       availableModeCount;
	uint8_t                       state;
	uint8_t                       reserved[6];
	CellVideoOutColorInfo         colorInfo;
	CellVideoOutDisplayMode       availableModes[32];
	CellVideoOutKSVList           ksvList;
} CellVideoOutDeviceInfo;

typedef struct CellVideoOutState {
	uint8_t                       state;
	uint8_t                       colorSpace;
	uint8_t                       reserved[6];
	CellVideoOutDisplayMode       displayMode;
} CellVideoOutState;

typedef struct CellVideoOutConfiguration {
	uint8_t                       resolutionId;
	uint8_t                       format;
	uint8_t                       aspect;
	uint8_t                       reserved[9];
	uint32_t                      pitch;
} CellVideoOutConfiguration;

typedef enum CellVideoOutEvent {
	CELL_VIDEO_OUT_EVENT_DEVICE_CHANGED,
	CELL_VIDEO_OUT_EVENT_OUTPUT_DISABLED,
	CELL_VIDEO_OUT_EVENT_DEVICE_AUTHENTICATED,
	CELL_VIDEO_OUT_EVENT_OUTPUT_ENABLED
} CellVideoOutEvent;

typedef enum CellVideoOutCopyControl {
	CELL_VIDEO_OUT_COPY_CONTROL_COPY_FREE,
	CELL_VIDEO_OUT_COPY_CONTROL_COPY_ONCE,
	CELL_VIDEO_OUT_COPY_CONTROL_COPY_NEVER
} CellVideoOutCopyControl;

typedef struct CellVideoOutOption {
	uint32_t                      reserved;
} CellVideoOutOption;

typedef int (*CellVideoOutCallback)(
	uint32_t slot, uint32_t videoOut, uint32_t deviceIndex,
	uint32_t event, CellVideoOutDeviceInfo* info, void* userData
);

int cellVideoOutRegisterCallback(
	uint32_t slot, CellVideoOutCallback function, void* userData
);

int cellVideoOutUnregisterCallback(uint32_t slot);

int cellVideoOutGetNumberOfDevice(uint32_t videoOut);

int cellVideoOutGetState(
	uint32_t videoOut, uint32_t deviceIndex, CellVideoOutState* state
);

int cellVideoOutGetDeviceInfo(
	uint32_t videoOut, uint32_t deviceIndex, CellVideoOutDeviceInfo* info
);

int cellVideoOutConfigure(
	uint32_t videoOut, CellVideoOutConfiguration* config,
	CellVideoOutOption* option, uint32_t waitForEvent
);

int cellVideoOutGetConfiguration(
	uint32_t videoOut, CellVideoOutConfiguration* config,
	CellVideoOutOption* option
);

int cellVideoOutGetResolution(
	uint32_t resolutionId, CellVideoOutResolution* resolution
);

int cellVideoOutGetResolutionAvailability(
	uint32_t videoOut, uint32_t resolutionId, uint32_t aspect, uint32_t option
);

int cellVideoOutSetCopyControl(
	uint32_t videoOut, uint32_t control
);
    
int cellVideoOutDebugSetMonitorType(
	uint32_t videoOut, uint32_t monitorType
);

int cellVideoOutSetGamma(
	uint32_t videoOut, float gamma
);

int cellVideoOutGetScreenSize(
	uint32_t videoOut, float* screenSize
);

/* ----------------------------- Audio -------------------------------- */

typedef enum CellAudioOut {
	CELL_AUDIO_OUT_PRIMARY,
	CELL_AUDIO_OUT_SECONDARY
} CellAudioOut;

typedef enum CellAudioOutPortType {
	CELL_AUDIO_OUT_PORT_HDMI,
	CELL_AUDIO_OUT_PORT_SPDIF,
	CELL_AUDIO_OUT_PORT_ANALOG,
	CELL_AUDIO_OUT_PORT_USB,
	CELL_AUDIO_OUT_PORT_BLUETOOTH,
	CELL_AUDIO_OUT_PORT_NETWORK
} CellAudioOutPortType;

typedef enum CellAudioOutputState {
	CELL_AUDIO_OUT_OUTPUT_STATE_ENABLED,
	CELL_AUDIO_OUT_OUTPUT_STATE_DISABLED,
	CELL_AUDIO_OUT_OUTPUT_STATE_PREPARING
} CellAudioOutOutputState;

typedef enum CellAudioOutDeviceState {
	CELL_AUDIO_OUT_DEVICE_STATE_UNAVAILABLE,
	CELL_AUDIO_OUT_DEVICE_STATE_AVAILABLE,
} CellAudioOutDeviceState;

typedef enum CellAudioOutCodingType {
	CELL_AUDIO_OUT_CODING_TYPE_LPCM = 0,
	CELL_AUDIO_OUT_CODING_TYPE_AC3,
	CELL_AUDIO_OUT_CODING_TYPE_MPEG1,
	CELL_AUDIO_OUT_CODING_TYPE_MP3,
	CELL_AUDIO_OUT_CODING_TYPE_MPEG2,
	CELL_AUDIO_OUT_CODING_TYPE_AAC,
	CELL_AUDIO_OUT_CODING_TYPE_DTS,
	CELL_AUDIO_OUT_CODING_TYPE_ATRAC,
	CELL_AUDIO_OUT_CODING_TYPE_BITSTREAM = 0xff
} CellAudioOutCodingType;

typedef enum CellAudioOutFs {
	CELL_AUDIO_OUT_FS_32KHZ                     = 0x01,
	CELL_AUDIO_OUT_FS_44KHZ                     = 0x02,
	CELL_AUDIO_OUT_FS_48KHZ                     = 0x04,
	CELL_AUDIO_OUT_FS_88KHZ                     = 0x08,
	CELL_AUDIO_OUT_FS_96KHZ                     = 0x10,
	CELL_AUDIO_OUT_FS_176KHZ                    = 0x20,
	CELL_AUDIO_OUT_FS_192KHZ                    = 0x40
} CellAudioOutFs;

typedef enum CellAudioOutChnum {
	CELL_AUDIO_OUT_CHNUM_2                      = 2,
	CELL_AUDIO_OUT_CHNUM_4                      = 4,
	CELL_AUDIO_OUT_CHNUM_6                      = 6,
	CELL_AUDIO_OUT_CHNUM_8                      = 8
} CellAudioOutChnum;

typedef enum CellAudioOutSbitOrBitrate {
	CELL_AUDIO_OUT_SBIT_NONE                    = 0x00,
	CELL_AUDIO_OUT_SBIT_16BIT                   = 0x01,
	CELL_AUDIO_OUT_SBIT_20BIT                   = 0x02,
	CELL_AUDIO_OUT_SBIT_24BIT                   = 0x04
} CellAudioOutSbitOrBitrate;

typedef enum CellAudioOutSpeakerLayout {
	CELL_AUDIO_OUT_SPEAKER_LAYOUT_DEFAULT       = 0x00000000,
	CELL_AUDIO_OUT_SPEAKER_LAYOUT_2CH           = 0x00000001,
	CELL_AUDIO_OUT_SPEAKER_LAYOUT_6CH_LREClr    = 0x00010000,
	CELL_AUDIO_OUT_SPEAKER_LAYOUT_8CH_LREClrxy  = 0x40000000
} CellAudioOutSpeakerLayout;

typedef struct CellAudioOutSoundMode {
	uint8_t                       type;
	uint8_t                       channel;
	uint8_t                       fs;
	uint8_t                       reserved;
	uint32_t                      layout;
} CellAudioOutSoundMode;

typedef struct CellAudioOutState {
	uint8_t                       state;
    uint8_t                       encoder;
	uint8_t                       reserved[6];
	uint32_t                      downMixer;
	CellAudioOutSoundMode         soundMode;
} CellAudioOutState;

typedef struct CellAudioOutDeviceInfo {
	uint8_t                       portType;
	uint8_t                       availableModeCount;
	uint8_t                       state;
	uint8_t                       reserved[3];
	uint16_t                      latency;
	CellAudioOutSoundMode         availableModes[16];
} CellAudioOutDeviceInfo;

typedef enum CellAudioOutDownMixer {
	CELL_AUDIO_OUT_DOWNMIXER_NONE,
	CELL_AUDIO_OUT_DOWNMIXER_TYPE_A,
	CELL_AUDIO_OUT_DOWNMIXER_TYPE_B
} CellAudioOutDownMixer;

typedef struct CellAudioOutConfiguration {
	uint8_t                       channel;
	uint8_t                       encoder;
	uint8_t                       reserved[10];
	uint32_t                      downMixer;
} CellAudioOutConfiguration;

typedef struct CellAudioOutOption {
	uint32_t                      reserved;
} CellAudioOutOption;

typedef enum CellAudioOutEvent {
	CELL_AUDIO_OUT_EVENT_DEVICE_CHANGED,
	CELL_AUDIO_OUT_EVENT_OUTPUT_DISABLED,
	CELL_AUDIO_OUT_EVENT_DEVICE_AUTHENTICATED,
	CELL_AUDIO_OUT_EVENT_OUTPUT_ENABLED
} CellAudioOutEvent;

typedef int (*CellAudioOutCallback)(
	uint32_t slot, uint32_t audioOut, uint32_t deviceIndex,
	uint32_t event, CellAudioOutDeviceInfo* info, void* userData
);

typedef uint32_t CellAudioOutLatency;

typedef enum CellAudioOutCopyControl {
	CELL_AUDIO_OUT_COPY_CONTROL_COPY_FREE,
	CELL_AUDIO_OUT_COPY_CONTROL_COPY_ONCE,
	CELL_AUDIO_OUT_COPY_CONTROL_COPY_NEVER
} CellAudioOutCopyControl;


int cellAudioOutRegisterCallback(
	uint32_t slot, CellAudioOutCallback function, void* userData
);

int cellAudioOutUnregisterCallback(uint32_t slot);

int cellAudioOutGetNumberOfDevice(uint32_t audioOut);

int cellAudioOutGetDeviceInfo(
	uint32_t audioOut, uint32_t deviceIndex, CellAudioOutDeviceInfo* info
);

int cellAudioOutGetState(
	uint32_t audioOut, uint32_t deviceIndex, CellAudioOutState* state
);

int cellAudioOutConfigure(
	uint32_t audioOut, CellAudioOutConfiguration* config,
	CellAudioOutOption* option, uint32_t waitForEvent
);

int cellAudioOutGetConfiguration(
	uint32_t audioOut, CellAudioOutConfiguration* config,
	CellAudioOutOption* option
);

int cellAudioOutSetCopyControl(
	uint32_t audioOut, uint32_t control
);

/* ----------------------------- Audio In -------------------------------- */

typedef enum CellAudioInDeviceMode {
	CELL_AUDIO_IN_SINGLE_DEVICE_MODE            = 0,
	CELL_AUDIO_IN_MULTI_DEVICE_MODE             = 1
} CellAudioInDeviceMode;

typedef enum CellAudioInPortType {
	CELL_AUDIO_IN_PORT_USB                      = 3,
	CELL_AUDIO_IN_PORT_BLUETOOTH                = 4
} CellAudioInPortType;

typedef enum CellAudioInDeviceState {
	CELL_AUDIO_IN_DEVICE_STATE_UNAVAILABLE      = 0,
	CELL_AUDIO_IN_DEVICE_STATE_AVAILABLE        = 1
} CellAudioInDeviceState;

typedef enum CellAudioInCodingType {
	CELL_AUDIO_IN_CODING_TYPE_LPCM              = 0
} CellAudioInCodingType;

typedef enum CellAudioInChnum {
	CELL_AUDIO_IN_CHNUM_NONE                    = 0,
	CELL_AUDIO_IN_CHNUM_1                       = 1,
	CELL_AUDIO_IN_CHNUM_2                       = 2
} CellAudioInChnum;

typedef enum CellAudioInFs {
	CELL_AUDIO_IN_FS_UNDEFINED                  = 0x00,
	CELL_AUDIO_IN_FS_8KHZ                       = 0x01,
	CELL_AUDIO_IN_FS_12KHZ                      = 0x02,
	CELL_AUDIO_IN_FS_16KHZ                      = 0x04,
	CELL_AUDIO_IN_FS_24KHZ                      = 0x08,
	CELL_AUDIO_IN_FS_32KHZ                      = 0x10,
	CELL_AUDIO_IN_FS_48KHZ                      = 0x20,
} CellAudioInFs;

typedef struct CellAudioInSoundMode {
	uint8_t type;
	uint8_t channel;
	uint16_t fs;
	uint8_t reserved[4];
} CellAudioInSoundMode;

typedef struct CellAudioInDeviceInfo {
	uint8_t portType;
	uint8_t availableModeCount;
	uint8_t state;
	uint8_t deviceNumber;
	uint8_t reserved[12];
	uint64_t deviceId;
	uint64_t type;
	char name[64];
	CellAudioInSoundMode availableModes[16];
} CellAudioInDeviceInfo;

typedef struct CellAudioInDeviceConfiguration {
	uint8_t volume;
	uint8_t reserved[31];
} CellAudioInDeviceConfiguration;

typedef struct CellAudioInRegistrationOption {
	uint32_t                      reserved;
} CellAudioInRegistrationOption;

int cellAudioInSetDeviceMode(
	uint32_t deviceMode
);

int cellAudioInRegisterDevice(
	uint64_t deviceType,
	const char* name,
	CellAudioInRegistrationOption* option,
	CellAudioInDeviceConfiguration* config
);

int cellAudioInUnregisterDevice(
	uint32_t deviceNumber
);

int cellAudioInGetDeviceInfo(
	uint32_t deviceNumber,
	uint32_t deviceIndex,
	CellAudioInDeviceInfo* info
);

int cellAudioInGetAvailableDeviceInfo(
	uint32_t count,
	CellAudioInDeviceInfo info[]
);

int cellAudioOutGetSoundAvailability(        
	uint32_t audioOut, uint32_t type, uint32_t fs, uint32_t option
);

int cellAudioOutGetSoundAvailability2(        
	uint32_t audioOut, uint32_t type, uint32_t fs, uint32_t ch, uint32_t option
);

int cellSysutilGetSystemParamInt( int id, int *value );
int cellSysutilGetSystemParamString( int id, char *buf, unsigned int bufsize );

#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
}
#endif /* defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus) */

#endif /*_CELL_SYSUTIL_SYSPARAM_H_*/
