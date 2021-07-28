/*   SCE CONFIDENTIAL                                       */
/*   PlayStation(R)3 Programmer Tool Runtime Library 310.001 */
/*   Copyright (C) 2008 Sony Computer Entertainment Inc.    */
/*   All Rights Reserved.                                   */

#ifndef _CELL_LIBCAMERA_H_
#define _CELL_LIBCAMERA_H_

#include <cell/error.h>
#include <sys/types.h>
#include <sys/event.h>
#include <sys/memory.h>

#ifdef __cplusplus
extern "C" {
#endif

#define CELL_CAMERA_MAKE_ERROR(status) \
  CELL_ERROR_MAKE_ERROR(CELL_ERROR_FACILITY_MICCAM, status)

#define CELL_CAMERA_ERROR_ALREADY_INIT       CELL_CAMERA_MAKE_ERROR(0x801) // libcamera was already init
#define CELL_CAMERA_ERROR_NOT_INIT           CELL_CAMERA_MAKE_ERROR(0x803) // libcamera has not been inited
#define CELL_CAMERA_ERROR_PARAM              CELL_CAMERA_MAKE_ERROR(0x804) // parameter error
#define CELL_CAMERA_ERROR_ALREADY_OPEN       CELL_CAMERA_MAKE_ERROR(0x805) // device is aready open
#define CELL_CAMERA_ERROR_NOT_OPEN           CELL_CAMERA_MAKE_ERROR(0x806) // device is closed
#define CELL_CAMERA_ERROR_DEVICE_NOT_FOUND   CELL_CAMERA_MAKE_ERROR(0x807) // device not found, could be unplugged
#define CELL_CAMERA_ERROR_DEVICE_DEACTIVATED CELL_CAMERA_MAKE_ERROR(0x808) // device was deactivated; 
#define CELL_CAMERA_ERROR_NOT_STARTED        CELL_CAMERA_MAKE_ERROR(0x809) // device is not started
#define CELL_CAMERA_ERROR_FORMAT_UNKNOWN     CELL_CAMERA_MAKE_ERROR(0x80a) // unknown format passed to Open
#define CELL_CAMERA_ERROR_RESOLUTION_UNKNOWN CELL_CAMERA_MAKE_ERROR(0x80b) // unknown resolution passed to Open
#define CELL_CAMERA_ERROR_BAD_FRAMERATE      CELL_CAMERA_MAKE_ERROR(0x80c) // framerate passed to Open is not supported
#define CELL_CAMERA_ERROR_TIMEOUT            CELL_CAMERA_MAKE_ERROR(0x80d) // timeout occurred; function may not have completed
#define CELL_CAMERA_ERROR_BUSY               CELL_CAMERA_MAKE_ERROR(0x80e) // asynchronous processing error
#define CELL_CAMERA_ERROR_FATAL              CELL_CAMERA_MAKE_ERROR(0x80f) // generic internal error

#define CELL_CAMERA_MAX_CAMERAS 1

typedef enum CellCameraType {
  CELL_CAMERA_TYPE_UNKNOWN,
  CELL_CAMERA_EYETOY,
  CELL_CAMERA_EYETOY2,
  CELL_CAMERA_USBVIDEOCLASS,
} CellCameraType;

typedef enum CellCameraFormat {
  CELL_CAMERA_FORMAT_UNKNOWN,
  CELL_CAMERA_JPG,
  CELL_CAMERA_RAW8,
  CELL_CAMERA_YUV422,
  CELL_CAMERA_RAW10,
  CELL_CAMERA_RGBA, // not yet supported
  CELL_CAMERA_YUV420, // not yet supported
  CELL_CAMERA_V_Y1_U_Y0,

  CELL_CAMERA_Y0_U_Y1_V = CELL_CAMERA_YUV422,
} CellCameraFormat;

typedef enum CellCameraResolution {
  CELL_CAMERA_RESOLUTION_UNKNOWN,
  CELL_CAMERA_VGA,        // 640x480
  CELL_CAMERA_QVGA,       // 320x240
  CELL_CAMERA_WGA,        // 640x360
  CELL_CAMERA_SPECIFIED_WIDTH_HEIGHT, // not yet supported
} CellCameraResolution;

typedef enum CellCameraAttribute {
  CELL_CAMERA_GAIN,       // 0..255 [auto]
  CELL_CAMERA_REDBLUEGAIN,  // 0..255 [auto]
  CELL_CAMERA_SATURATION,   // 0..255 [0xa4]
  CELL_CAMERA_EXPOSURE,     // 0..255 [auto]
  CELL_CAMERA_BRIGHTNESS,   // 0..255 [0x60]
  CELL_CAMERA_AEC,          // 0..1 [1] automatic exposure control
  CELL_CAMERA_AGC,          // 0..1,[1] automatic gain control
  CELL_CAMERA_AWB,          // 0..1,[1] automatic white balance
  CELL_CAMERA_ABC,          // 0..1,[0] automatic brightness
  CELL_CAMERA_LED,          // 0..255, 0..255 [0,0] frames on,frames off
  CELL_CAMERA_AUDIOGAIN,    // 0..7 [5]
  CELL_CAMERA_QS,           // 0..30 [0] minimum compression level
  CELL_CAMERA_NONZEROCOEFFS,// 0..64, 0..64 [32,32] # lum coeffs,# chrom coeffs
  CELL_CAMERA_YUVFLAG,      // 0..1 [0] 0=YCbCr, 1=YUV
  CELL_CAMERA_JPEGFLAG,     // 0..1 [1] 0=IPU stream, 1=JPEG stream
  CELL_CAMERA_BACKLIGHTCOMP,// 0..1 [0] backlight compensation flag
  CELL_CAMERA_MIRRORFLAG,   // 0..1 [1] 0=camera style, 1=mirror-style
  CELL_CAMERA_MEASUREDQS,   // 0..30 [Read-only] dynamic compression level
  CELL_CAMERA_422FLAG,      // 0..1 [1] 0=yuv420 format, 1=yuv422 format
  CELL_CAMERA_USBLOAD,      // 0..4 [4] 4=896 byte payload, 3=768, 2=512
  CELL_CAMERA_GAMMA,        // 0..1 [1] gamma correction flag
  CELL_CAMERA_GREENGAIN,    // 0..255 [auto] green gain (adjusted by AWB)
  CELL_CAMERA_AGCLIMIT,     // 0..5 [4] Gain limit for AGC, 0=2x, 1=4x,..4=32x,5=64x
  CELL_CAMERA_DENOISE,      // 0..1 [1] Denoise filter flag, 1=on
  CELL_CAMERA_FRAMERATEADJUST, // 0..3 [0] auto-adjust framerate if gain is high to allow more exposure time 
                            // 0=no adjustment allowed, 1=max reduction 1/2 framerate, 
                            // 2 = max reduction 1/4 framerate, 3 = max reduction 1/8 framerate  
  CELL_CAMERA_PIXELOUTLIERFILTER, // 0..1 [1] Hardware filter for outlier measurements (default is on)
  CELL_CAMERA_AGCLOW,       // 0..255 [48]
  CELL_CAMERA_AGCHIGH,      // 0..255 [64]

  CELL_CAMERA_FORMATCAP=100,// bit-or of (1 << cellCameraFormat)
  CELL_CAMERA_FORMATINDEX,  // format index for get attribute
  CELL_CAMERA_NUMFRAME,     // # of frames of current format
  CELL_CAMERA_FRAMEINDEX,   // frame index for get attribute
  CELL_CAMERA_FRAMESIZE,    // width and height of current frame
  CELL_CAMERA_INTERVALTYPE, // interval type (0=continuous, 1..255=Descrete)
  CELL_CAMERA_INTERVALINDEX,// interval index for get attribute
  CELL_CAMERA_INTERVALVALUE,// interval value (in 100ns units)
  CELL_CAMERA_COLORMATCHING,// color matching informations
  CELL_CAMERA_PLFREQ,       // 0..2 [device-depend] 0=disabled, 1=50Hz, 2=60Hz
  CELL_CAMERA_DEVICEID,     // vendor/product ID
  CELL_CAMERA_DEVICECAP,    // camera/processing capabilities
  CELL_CAMERA_DEVICESPEED,  // cellUsbdGetDeviceSpeed
  CELL_CAMERA_UVCREQCODE,   // USB Video Class Request Code
  CELL_CAMERA_UVCREQDATA,   // USB Video Class Request Data
  CELL_CAMERA_DEVICEID2,	// idVendor, idProduct, bcdDevice (Read Only)

  CELL_CAMERA_READMODE=300,	// 0: call the read function, 1: read directly from double buffer
  CELL_CAMERA_GAMEPID,		//
  CELL_CAMERA_PBUFFER,		//
  CELL_CAMERA_READFINISH,	//

  CELL_CAMERA_ATTRIBUTE_UNKNOWN=500,
} CellCameraAttribute;

//event flag
#define CELL_CAMERA_EFLAG_FRAME_UPDATE	0x00000001
#define CELL_CAMERA_EFLAG_OPEN			0x00000002
#define CELL_CAMERA_EFLAG_CLOSE			0x00000004
#define CELL_CAMERA_EFLAG_START			0x00000008
#define CELL_CAMERA_EFLAG_STOP			0x00000010
#define CELL_CAMERA_EFLAG_RESET			0x00000020

// COLORMATCHING, Color Primaries (*arg1, bit23-16)
#define CELL_CAMERA_CM_CP_UNSPECIFIED   0
#define CELL_CAMERA_CM_CP_BT709_sRGB    1
#define CELL_CAMERA_CM_CP_BT470_2M      2
#define CELL_CAMERA_CM_CP_BT470_2BG     3
#define CELL_CAMERA_CM_CP_SMPTE170M     4
#define CELL_CAMERA_CM_CP_SMPTE240M     5

// COLORMATCHING, Transfer Characteristic (*arg1, bit15-8)
#define CELL_CAMERA_CM_TC_UNSPECIFIED   0
#define CELL_CAMERA_CM_TC_BT709         1
#define CELL_CAMERA_CM_TC_BT470_2M      2
#define CELL_CAMERA_CM_TC_BT470_2BG     3
#define CELL_CAMERA_CM_TC_SMPTE170M     4
#define CELL_CAMERA_CM_TC_SMPTE240M     5
#define CELL_CAMERA_CM_TC_LINEAR        6
#define CELL_CAMERA_CM_TC_sRGB          7

// COLORMATCHING, Matrix Coefficient (*arg1, bit7-0)
#define CELL_CAMERA_CM_MC_UNSPECIFIED   0
#define CELL_CAMERA_CM_MC_BT709         1
#define CELL_CAMERA_CM_MC_FCC           2
#define CELL_CAMERA_CM_MC_BT470_2BG     3
#define CELL_CAMERA_CM_MC_SMPTE170M     4
#define CELL_CAMERA_CM_MC_SMPTE240M     5

// PLFREQ (Power Line Frequency) (*arg1)
#define CELL_CAMERA_PLFREQ_DISABLED     0
#define CELL_CAMERA_PLFREQ_50Hz         1
#define CELL_CAMERA_PLFREQ_60Hz         2

// DEVICECAP, Camera Terminal Capabilities (*arg1)
#define CELL_CAMERA_CTC_SCANNING_MODE                   (1 <<  0)
#define CELL_CAMERA_CTC_AUTO_EXPOSURE_MODE              (1 <<  1)
#define CELL_CAMERA_CTC_AUTO_EXPOSURE_PRIORITY          (1 <<  2)
#define CELL_CAMERA_CTC_EXPOSURE_TIME_ABS               (1 <<  3)
#define CELL_CAMERA_CTC_EXPOSURE_TIME_REL               (1 <<  4)
#define CELL_CAMERA_CTC_FOCUS_ABS                       (1 <<  5)
#define CELL_CAMERA_CTC_FOCUS_REL                       (1 <<  6)
#define CELL_CAMERA_CTC_IRIS_ABS                        (1 <<  7)
#define CELL_CAMERA_CTC_IRIS_REL                        (1 <<  8)
#define CELL_CAMERA_CTC_ZOOM_ABS                        (1 <<  9)
#define CELL_CAMERA_CTC_ZOOM_REL                        (1 << 10)
#define CELL_CAMERA_CTC_PANTILT_ABS                     (1 << 11)
#define CELL_CAMERA_CTC_PANTILT_REL                     (1 << 12)
#define CELL_CAMERA_CTC_ROLL_ABS                        (1 << 13)
#define CELL_CAMERA_CTC_ROLL_REL                        (1 << 14)
#define CELL_CAMERA_CTC_RESERVED_15                     (1 << 15)
#define CELL_CAMERA_CTC_RESERVED_16                     (1 << 16)
#define CELL_CAMERA_CTC_FOCUS_AUTO                      (1 << 17)
#define CELL_CAMERA_CTC_PRIVACY                         (1 << 18)

// DEVICECAP, Processing Unit Capabilities (*arg2)
#define CELL_CAMERA_PUC_BRIGHTNESS                      (1 <<  0)
#define CELL_CAMERA_PUC_CONTRAST                        (1 <<  1)
#define CELL_CAMERA_PUC_HUE                             (1 <<  2)
#define CELL_CAMERA_PUC_SATURATION                      (1 <<  3)
#define CELL_CAMERA_PUC_SHARPNESS                       (1 <<  4)
#define CELL_CAMERA_PUC_GAMMA                           (1 <<  5)
#define CELL_CAMERA_PUC_WHITE_BALANCE_TEMPERATURE       (1 <<  6)
#define CELL_CAMERA_PUC_WHITE_BALANCE_COMPONENT         (1 <<  7)
#define CELL_CAMERA_PUC_BACKLIGHT_COMPENSATION          (1 <<  8)
#define CELL_CAMERA_PUC_GAIN                            (1 <<  9)
#define CELL_CAMERA_PUC_POWER_LINE_FREQUENCY            (1 << 10)
#define CELL_CAMERA_PUC_HUE_AUTO                        (1 << 11)
#define CELL_CAMERA_PUC_WHITE_BALANCE_TEMPERATURE_AUTO  (1 << 12)
#define CELL_CAMERA_PUC_WHITE_BALANCE_COMPONENT_AUTO    (1 << 13)
#define CELL_CAMERA_PUC_DIGITAL_MULTIPLIER              (1 << 14)
#define CELL_CAMERA_PUC_DIGITAL_MULTIPLIER_LIMIT        (1 << 15)
#define CELL_CAMERA_PUC_ANALOG_VIDEO_STANDARD           (1 << 16)
#define CELL_CAMERA_PUC_ANALOG_VIDEO_LOCK_STATUS        (1 << 17)

// UVCREQCODE Control Selector (arg1)
#define CELL_CAMERA_CS_SHIFT	0			// Control Selector
#define CELL_CAMERA_CS_BITS		0x000000ff
#define CELL_CAMERA_CAP_SHIFT	8			// DeviceCap bit shift
#define CELL_CAMERA_CAP_BITS	0x0000ff00
#define CELL_CAMERA_NUM_SHIFT	16			// # of Members
#define CELL_CAMERA_NUM_BITS	0x000f0000
#define CELL_CAMERA_NUM_1		0x00010000
#define CELL_CAMERA_NUM_2		0x00020000
#define CELL_CAMERA_NUM_3		0x00030000
#define CELL_CAMERA_NUM_4		0x00040000
#define CELL_CAMERA_LEN_SHIFT	20			// Length of a Member
#define CELL_CAMERA_LEN_BITS	0x00f00000
#define CELL_CAMERA_LEN_1		0x00100000
#define CELL_CAMERA_LEN_2		0x00200000
#define CELL_CAMERA_LEN_4		0x00400000
#define CELL_CAMERA_ID_SHIFT	24			// ID bits
#define CELL_CAMERA_ID_BITS		0x0f000000
#define CELL_CAMERA_ID_CT		0x01000000  // Camera Terminal
#define CELL_CAMERA_ID_SU		0x02000000  // Selector Unit
#define CELL_CAMERA_ID_PU		0x04000000  // Processing Unit

// UVCREQCODE Camera Terminal Control
#define CELL_CAMERA_UVC_SCANNING_MODE			0x01110001
#define CELL_CAMERA_UVC_AUTO_EXPOSURE_MODE		0x01110102
#define CELL_CAMERA_UVC_AUTO_EXPOSURE_PRIORITY	0x01110203
#define CELL_CAMERA_UVC_EXPOSURE_TIME_ABS		0x01410304
#define CELL_CAMERA_UVC_EXPOSURE_TIME_REL		0x01110405
#define CELL_CAMERA_UVC_FOCUS_ABS				0x01210506
#define CELL_CAMERA_UVC_FOCUS_REL				0x01120607
#define CELL_CAMERA_UVC_FOCUS_AUTO				0x01111108
#define CELL_CAMERA_UVC_IRIS_ABS				0x01210709
#define CELL_CAMERA_UVC_IRIS_REL				0x0111080a
#define CELL_CAMERA_UVC_ZOOM_ABS				0x0121090b
#define CELL_CAMERA_UVC_ZOOM_REL				0x01130a0c
#define CELL_CAMERA_UVC_PANTILT_ABS				0x01420b0d
#define CELL_CAMERA_UVC_PANTILT_REL				0x01140c0e
#define CELL_CAMERA_UVC_ROLL_ABS				0x01210d0f
#define CELL_CAMERA_UVC_ROLL_REL				0x01120e10
#define CELL_CAMERA_UVC_PRIVACY					0x01111211

// UVCREQCODE Selector Unit Control
#define CELL_CAMERA_UVC_INPUT_SELECT	0x02110101

// UVCREQCODE Processing Unit Control
#define CELL_CAMERA_UVC_BACKLIGHT_COMPENSATION			0x04210801
#define CELL_CAMERA_UVC_BRIGHTNESS						0x04210002
#define CELL_CAMERA_UVC_CONTRAST						0x04210103
#define CELL_CAMERA_UVC_GAIN							0x04210904
#define CELL_CAMERA_UVC_POWER_LINE_FREQUENCY			0x04110a05
#define CELL_CAMERA_UVC_HUE								0x04210206
#define CELL_CAMERA_UVC_HUE_AUTO						0x04110b10
#define CELL_CAMERA_UVC_SATURATION						0x04210307
#define CELL_CAMERA_UVC_SHARPNESS						0x04210408
#define CELL_CAMERA_UVC_GAMMA							0x04210509
#define CELL_CAMERA_UVC_WHITE_BALANCE_TEMPERATURE		0x0421060a
#define CELL_CAMERA_UVC_WHITE_BALANCE_TEMPERATURE_AUTO	0x04110c0b
#define CELL_CAMERA_UVC_WHITE_BALANCE_COMPONENT			0x0422070c
#define CELL_CAMERA_UVC_WHITE_BALANCE_COMPONENT_AUTO	0x04110d0d
#define CELL_CAMERA_UVC_DIGITAL_MULTIPLIER				0x04210e0e
#define CELL_CAMERA_UVC_DIGITAL_MULTIPLIER_LIMIT		0x04210f0f
#define CELL_CAMERA_UVC_ANALOG_VIDEO_STANDARD			0x04111011
#define CELL_CAMERA_UVC_ANALOG_VIDEO_LOCK_STATUS		0x04111112

// UVCREQCODE Request code bits (arg2)
#define CELL_CAMERA_RC_CUR		0x81	// Current
#define CELL_CAMERA_RC_MIN		0x82	// Minimum
#define CELL_CAMERA_RC_MAX		0x83	// Maximum
#define CELL_CAMERA_RC_RES		0x84	// Resolution
#define CELL_CAMERA_RC_LEN		0x85	// Length
#define CELL_CAMERA_RC_INFO		0x86	// Info
#define CELL_CAMERA_RC_DEF		0x87	// Default

typedef struct CellCameraInfo {
  // filled in by application as inputs for open
  CellCameraFormat format;
  CellCameraResolution resolution;
  int framerate;

  // filled in by open
  unsigned char *buffer;
  int bytesize;
  int width;
  int height;
  int dev_num;
  int guid;
} CellCameraInfo;

#define CELL_CAMERA_INFO_VER_100    0x0100	//18x
#define CELL_CAMERA_INFO_VER_101    0x0101	//190
#define CELL_CAMERA_INFO_VER_200    0x0200	//280
#define CELL_CAMERA_INFO_VER        CELL_CAMERA_INFO_VER_200

//read_mode
#define CELL_CAMERA_READ_FUNCCALL	0	//call cellCameraRead or cellCameraReadEx
#define CELL_CAMERA_READ_DIRECT		1	//read double buffer directly

typedef struct CellCameraInfoEx {
  // filled in by application as inputs for open
  CellCameraFormat format;
  CellCameraResolution resolution;
  int framerate;

  // filled in by open
  unsigned char *buffer;
  int bytesize;
  int width;
  int height;
  int dev_num;
  int guid;

	// extended member
	int info_ver;
	sys_memory_container_t container;	//VER_100

	int read_mode;			//VER_200
	unsigned char* pbuf[2];	//VER_200
} CellCameraInfoEx;

#define CELL_CAMERA_READ_VER_100	0x0100	//270
#define CELL_CAMERA_READ_VER		CELL_CAMERA_READ_VER_100

typedef struct CellCameraReadEx {
	int version;
	unsigned int frame;
	unsigned int bytesread;
	system_time_t timestamp;
	unsigned char* pbuf;
} CellCameraReadEx;

int cellCameraInit(void); // says that I am going to be a client of Camera service
int cellCameraEnd(void); // says that I am no longer a client of Camera service, closes if open

// get a global unique identifier for a device. Can be called even if device is not open
int cellCameraGetDeviceGUID(int dev_num, uint32_t* ptr_guid);

// get the device type.  Can be done even if device is not open
int cellCameraGetType(int dev_num, CellCameraType *type);

// do we need this?
// returns physical usb port that camera is plugged into
//int cellCameraGetPort(int dev_num); 

// query if camera is attached
int cellCameraIsAttached( int dev_num);

// query if camera is available
int cellCameraIsAvailable(int dev_num);

// query if camera is open
int cellCameraIsOpen( int dev_num);

// query if camera is started
int cellCameraIsStarted( int dev_num);

// get buffer size for open camera
int cellCameraGetBufferSize(int devnum, CellCameraInfoEx* info);

// cellCameraOpen with memory container
int cellCameraOpenEx(int devnum, CellCameraInfoEx* info);

// close camera service (frees memory); stops camera if started
int cellCameraClose( int dev_num);

// start streaming.  device compatible with open service must be attached
int cellCameraStart( int dev_num);

// stop streaming
int cellCameraStop( int dev_num); 

// reset camera hardware.  Must be done when camera is plugged in.
int cellCameraReset( int dev_num);

// read data into buffer in CellCameraInfo.  Also returns framenum and actual bytes read
int cellCameraRead( int dev_num, unsigned int* frame, unsigned int* bytesread); 

// cellCameraRead with timestamp
int cellCameraReadEx(int dev_num, CellCameraReadEx* pRead);

// cellCameraGetBufferInfo with memory container
int cellCameraGetBufferInfoEx(int dev_num, CellCameraInfoEx *info);

// set camera attribute
int cellCameraSetAttribute( int dev_num, CellCameraAttribute attribute, uint32_t arg1, uint32_t arg2);
// get camera attribute
int cellCameraGetAttribute( int dev_num, CellCameraAttribute attribute, uint32_t* arg1, uint32_t* arg2);

// notifying completion of data reading.
int cellCameraReadComplete(int devnum, uint32_t bufnum, uint32_t arg2);

// Extension Unit Control
int cellCameraPrepExtensionUnit(int devnum, uint8_t* guidExtensionCode);
int cellCameraCtrlExtensionUnit(int devnum, uint8_t bRequest, uint16_t wValue, uint16_t wLength, uint8_t* pData);
int cellCameraSetExtensionUnit(int devnum, uint16_t wValue, uint16_t wLength, uint8_t* pData);
int cellCameraGetExtensionUnit(int devnum, uint16_t wValue, uint16_t wLength, uint8_t* pData);

// signal that application should defer settings to other applications if
// camera is shared
//int cellCameraSetCooperativeBehavior( int flag);

#define CELL_CAMERA_ATTACH   1
#define CELL_CAMERA_DETACH   0
#define CELL_CAMERA_FRAME_UPDATE 2
#define CELL_CAMERA_OPEN     3
#define CELL_CAMERA_CLOSE    4
#define CELL_CAMERA_START    5
#define CELL_CAMERA_STOP     6
#define CELL_CAMERA_RESET    7

// callback to capture audio input system events "CellCAMERA_ATTACH,CellCAMERA_DETACH"
int cellCameraSetNotifyEventQueue( sys_ipc_key_t );
int cellCameraRemoveNotifyEventQueue( sys_ipc_key_t );

int cellCameraSetNotifyEventQueue2(sys_ipc_key_t key, uint64_t source, uint64_t flag);
int cellCameraRemoveNotifyEventQueue2(sys_ipc_key_t key);

#ifdef __cplusplus
};
#endif

#endif   // _CELL_LIBCAMERA_H_


