/* SCE CONFIDENTIAL
 PlayStation(R)3 Programmer Tool Runtime Library 320.001
 * Copyright (C) 2009 Sony Computer Entertainment Inc.
 * All Rights Reserved.
 */
#ifndef __CELL_GEM_LIBGEM_H__
#define __CELL_GEM_LIBGEM_H__

#include <stdint.h>
#include <sys/types.h>
#include <cell/spurs.h>
#ifndef __SPU__
#include <sys/paths.h>
#include <vec_types.h>
#endif

#define CELL_GEM_VERSION 2 ///< Used for API backwards compatibility.

#define CELL_GEM_MAX_NUM (4)   ///< Maximum number of motion controllers

/* CellGemInfo: Status */
#define CELL_GEM_STATUS_DISCONNECTED    (0)     ///< Device is disconnected
#define CELL_GEM_STATUS_READY						(1)			///< Device is connected and ready

/**
 * @brief Structure containing device connection information.
 *
 * This structure is used by the cellGemGetInfo() function.
 * The PlayStation&reg;3 has 7 ports for all types of controllers.  They are normally 
 * assigned sequentially starting with 1 first, but for the motion controller the 
 * ports are assigned in reverse sequential order starting with 7.
 *
 * The "gem slot" relates to the <c><i>gem_num</i></c> argument that is in so many libgem functions.  
 * It is the logical numbering of the motion controllers, starting with 0 and assigned sequentially.
 * Typically, motion controller 0 is assigned port 7 of the PlayStation&reg;3;  motion controller 1
 * is assigned port 6, and so on.
 * 
 * libgem only uses the <c><i>gem_num</i></c> as an argument, but the CellGemInfo structure lets you 
 * find out what the PlayStation&reg;3 port number is (what the PlayStation&reg;3 XMB&trade; displays as the 
 * controller number).
 *
 * @see 
 * cellGemGetInfo
*/
typedef struct CellGemInfo
{
    uint32_t max_connect;			///< The number of devices that can be connected and active, as specified by cellGemInit()
    uint32_t now_connect;				 ///< The number of devices currently connected and active
    uint32_t status[CELL_GEM_MAX_NUM]; ///< Connection status (<c>DISCONNECTED</c> or <c>READY</c>) for each gem slot (0 - 3)
    uint32_t port[CELL_GEM_MAX_NUM]; ///< libpad port number (1-7) assigned to each motion controller; assignment begins with 7 and counts down.  Equals 0 when disconnected.
} CellGemInfo;

/* CellGemPadData : Bit assignment of digital buttons. */
#define CELL_GEM_CTRL_SQUARE    (1 << 7)   ///< Square button
#define CELL_GEM_CTRL_CROSS     (1 << 6)   ///< Cross button
#define CELL_GEM_CTRL_CIRCLE    (1 << 5)   ///< Circle button
#define CELL_GEM_CTRL_TRIANGLE  (1 << 4)   ///< Triangle button
#define CELL_GEM_CTRL_TICK      (1 << 2)   ///< Tick (checkmark) button (the big button)
#define CELL_GEM_CTRL_TRIGGER   (1 << 1)   ///< Trigger button
#define CELL_GEM_CTRL_START     (1 << 3)   ///< Start button
#define CELL_GEM_CTRL_SELECT    (1 << 0)   ///< Select button


#define CELL_GEM_SPHERE_RADIUS_MM 22.5f    ///< Radius of the physical sphere

#define CELL_GEM_LATENCY_OFFSET -22.0f     ///< Average time offset (in milliseconds) of sensing

#define CELL_GEM_MIN_CAMERA_EXPOSURE 80			///< Minimum camera exposure that can be used by libgem
#define CELL_GEM_MAX_CAMERA_EXPOSURE 511		///< Maximum camera exposure that can be used by libgem

/* Flags for cellGemGetState */
#define CELL_GEM_STATE_FLAG_CURRENT_TIME 0                 ///< State timing matched to the current system time when cellGemGetState() is called
#define CELL_GEM_STATE_FLAG_LATEST_IMAGE_TIME 1            ///< State timing matched to latest camera image prior to when cellGemGetState() is called

/* cellGemGetState return codes */
#define CELL_GEM_NOT_CONNECTED 1     ///< The specified device has not established a connection to the PlayStation&reg;3
#define CELL_GEM_SPHERE_NOT_CALIBRATED 2   ///< The sphere of the specified device is not calibrated.
#define CELL_GEM_SPHERE_CALIBRATING 3      ///< The sphere of the specified device is in the process of calibrating.
#define CELL_GEM_COMPUTING_AVAILABLE_COLORS 4   ///< libgem is determining the available tracking hues for the sphere LED.
#define CELL_GEM_HUE_NOT_SET 5       ///< The hue of the sphere is not set.
#define CELL_GEM_NO_VIDEO 6          ///< No usable video is specified.

/* cellGemGetStatusFlags masks */
#define CELL_GEM_FLAG_CALIBRATION_OCCURRED							(1 << 0)
#define CELL_GEM_FLAG_CALIBRATION_SUCCEEDED							(1 << 1)
#define CELL_GEM_FLAG_CALIBRATION_FAILED_CANT_FIND_SPHERE			(1 << 2)
#define CELL_GEM_FLAG_CALIBRATION_FAILED_MOTION_DETECTED			(1 << 3)
#define CELL_GEM_FLAG_CALIBRATION_FAILED_BRIGHT_LIGHTING			(1 << 4)
#define CELL_GEM_FLAG_CALIBRATION_WARNING_MOTION_DETECTED			(1 << 5)
#define CELL_GEM_FLAG_CALIBRATION_WARNING_BRIGHT_LIGHTING			(1 << 6)
#define CELL_GEM_FLAG_LIGHTING_CHANGED								(1 << 7) //not yet implemented
#define CELL_GEM_FLAG_WRONG_CAMERA_FIELD_OF_VIEW_SETTING			(1 << 8) //not yet implemented
#define CELL_GEM_FLAG_CAMERA_PITCH_ANGLE_CHANGED					(1 << 9) //not yet implemented
#define CELL_GEM_FLAG_VARIABLE_MAGNETIC_FIELD						(1 << 10) //not yet implemented
#define CELL_GEM_FLAG_WEAK_MAGNETIC_FIELD							(1 << 11) //not yet implemented
#define CELL_GEM_ALL_FLAGS											0xffffffffffffffffull

/* Error codes */
#define CELL_GEM_ERROR_RESOURCE_ALLOCATION_FAILED   CELL_ERROR_CAST(0x80121801)
#define CELL_GEM_ERROR_ALREADY_INITIALIZED          CELL_ERROR_CAST(0x80121802)
#define CELL_GEM_ERROR_UNINITIALIZED                CELL_ERROR_CAST(0x80121803)
#define CELL_GEM_ERROR_INVALID_PARAMETER            CELL_ERROR_CAST(0x80121804)
#define CELL_GEM_ERROR_INVALID_ALIGNMENT            CELL_ERROR_CAST(0x80121805)
#define CELL_GEM_ERROR_UPDATE_NOT_FINISHED          CELL_ERROR_CAST(0x80121806)
#define CELL_GEM_ERROR_UPDATE_NOT_STARTED           CELL_ERROR_CAST(0x80121807)
#define CELL_GEM_ERROR_CONVERT_NOT_FINISHED          CELL_ERROR_CAST(0x80121808)
#define CELL_GEM_ERROR_CONVERT_NOT_STARTED           CELL_ERROR_CAST(0x80121809)

/** 
 * @brief Structure containing device keypad state information.
 *
 * This data is retrieved by cellGemGetState() and cellGemGetInertialState().
 *
 * @see 
 * CellGemState, CellGemInertialState
*/
typedef struct CellGemPadData{
	uint16_t digitalbuttons;  ///< State of each of the button controls
	uint16_t analog_trigger;  ///< Analog value of trigger
} CellGemPadData;


/** 
 * @brief The state of the device as computed from video and inertial sensors.
 *
 * Structure containing the state of the device as computed from video and inertial sensors. 
 * The elements are computed in &#8220;world&#8221; coordinates as distinct from camera or device coordinates.
 * Both world and camera coordinate systems have their origin at the camera&#8216;s center of projection.
 * 
 * Camera coordinates are defined as:
 * <ul><li>x axis increases to the right as the user faces the camera.</li>
 * <li>y axis increases up in the image plane.</li>
 * <li>z axis is the direction through the camera&#8216;s optical center, and increases towards 
 * the user (away from the camera).</li></ul>
 * 
 * World coordinates are offset from camera coordinates by the pitch angle of the camera; 
 * they are defined as:
 * <ul><li>x axis increases right as the user faces the camera.</li>
 * <li>y axis increases up, parallel to gravity.</li>
 * <li>z axis is the direction through the camera&#8216;s optical center projected onto the ground,
 * and increases towards the user (away from the camera).</li></ul>
 * 
 * The device coordinate system has its origin at the center of the sphere, with coordinates as follows: 
 * <ul><li>y axis increases up from the button face.</li>
 * <li>z axis increasing from the center of the sphere towards and aligned with the base of the handle.</li>
 * <li>x axis increasing right perpendicular from the button face.</li></ul>
 *
 * @see 
 * CellGemPadData, cellGemGetState
*/
typedef struct CellGemState {
    vec_float4 pos;	       ///< Position of the center of the sphere in world coordinates (mm)
    vec_float4 vel;          ///< Velocity of sphere in world coordinates (mm/s)
    vec_float4 accel;        ///< Acceleration of sphere in world coordinates (mm/s^2)
    vec_float4 quat;         ///< Quaternion orientation of device in world (<c>x</c>,<c>y</c>,<c>z</c>,<c>w</c>)
    vec_float4 angvel;       ///< Angular velocity of device in world coordinates (radians/s)
    vec_float4 angaccel;     ///< Angular acceleration of device in world coordinates (radians/s^2)
    vec_float4 handle_pos;	 ///< Position near the player&#8216;s hand in world coordinates (mm)
    vec_float4 handle_vel;   ///< Velocity near the player&#8216;s hand in world coordinates (mm/s)
    vec_float4 handle_accel; ///< Acceleration near the player&#8216;s hand in world coordinates (mm/s^2)
    CellGemPadData pad;      ///< State of the device button controls
    system_time_t timestamp; ///< System timestamp
    int count_since_last_seen;  ///< Count of inertial sensor states since last visible camera frame
    int update_mode;         ///< When 0, the update is based on inertial sensor and image; when 1, the position and velocity update is estimated only from inertial sensor
    float temperature;       ///< The temperature of the sensor (used internally) 
    unsigned char visible;   ///< Whether the sphere is visible on camera at the time of this state
	float camera_pitch_angle;///< The camera pitch angle that was used for computing this state
} CellGemState;


/** 
 * @brief Inertial sensor data.
 *
 * Structure containing the data returned from the inertia sensors. The sensor readings are in sensor-local coordinates.
 * This data is raw data from the sensor in the handle of the device. If you use this data instead of the device
 * positional data, you need to perform the appropriate transformations to adjust the position from the sensor to the
 * center of the sphere. 
 * @note
 * We recommend that you use the device coordinate data (CellGemState) rather than this sensor data.
 * @see
 * cellGemGetInertialState, CellGemState
*/
typedef struct CellGemInertialState {
	vec_float4 accelerometer;   ///< Accelerometer readings in sensor coordinates (G units)
	vec_float4 gyro;            ///< Gyro readings in sensor coordinates (radians/s)
	CellGemPadData pad;         ///< State of the device button controls
	system_time_t timestamp;    ///< System timestamp
	int counter;                ///< Numbers the sensor data sequentially
	float temperature;          ///< The temperature of the sensor (used internally) 
} CellGemInertialState;


/** 
 * @brief Computer vision data.
 *
 * Structure containing the data returned from the computer camera.
 *
 * The sphere coordinates are calculated with the origin in the upper left of image 
 * with <c><i>u</i></c> increasing to the right and <c><i>v</i></c> increasing down.
 * 
 * The projection coordinates are calculated with the origin at the center of the image 
 * with <c><i>x</i></c> increasing right and <c><i>y</i></c> increasing up. The 3D position 
 * of the sphere in the camera 
 * coordinate system is given by:
 * 
 * <c>(<i>projectionx</i> * <i>distance</i>, <i>projectiony</i> * <i>distance</i>, <i>distance</i>)</c>
 * 
 * @see
 * cellGemGetImageState
*/
typedef struct CellGemImageState {
	system_time_t frame_timestamp;   ///< Timestamp returned by <c>cellCameraReadEx()</c>
	system_time_t timestamp;         ///< Timestamp for when the sphere was actually imaged; includes rolling shutter and exposure adjustments.
	float u;                         ///< Horizontal pixel coordinate of center of the sphere
	float v;                         ///< Vertical pixel coordinate of center of the sphere
	float r;                         ///< Radius of sphere in pixels
	float projectionx;               ///< Horizontal normalized image coordinate
	float projectiony;               ///< Vertical normalized image coordinate
	float distance;                  ///< Distance from the plane located at the center of projection and parallel to the image plane
	unsigned char visible;           ///< Whether visible on camera at the time of this state
	unsigned char r_valid;           ///< Whether or not <c><i>r</i></c> was computed this frame. Set to 0, <c><i>r</i></c> and <c><i>distance</i></c> are data from previous camera frames
} CellGemImageState;


typedef struct CellGemCameraState {
	int exposure;						// camera exposure setting in image rows (same as libcamera setting)
	float exposure_time;				// camera exposure time (in seconds)
	float gain;							// camera effective gain (mininum is 1.0, max is 4.0)
	float pitch_angle;					// pitch angle used for CellGemState computation
	float pitch_angle_estimate;			// current estimate of pitch angle
} CellGemCameraState;


/** 
 * @brief Device initialization data.
 *
 * Structure containing the data with which the device is initialized.
 *
 * @see
 * cellGemAttributeInit
*/
typedef struct CellGemAttribute {
	uint32_t version;                  ///< Specifies the libgem library version for which this application was built.
	uint32_t max_connect;              ///< Specifies the maximum number of devices (up to <c>CELL_GEM_MAX_NUM</c>).
	void * memory_ptr;             ///< Specifies the memory for libgem.  If NULL, libgem will allocate its own memory.
	CellSpurs *spurs;                  ///< Required. Specifies a pointer to <c>CellSpurs</c>.
	uint8_t spu_priorities[8];         ///< Specifies an array of SPU priorities to use for GEM SPURS workload.
} CellGemAttribute;


/**
 * @brief Options for the <c><i>output_format</i></c> member of the structure CellGemVideoConvertAttribute.
 * 
 * Enumeration of the mode used to manipulate video output.
 * 
 * @see CellGemVideoConvertAttribute
 * 
 */
typedef enum
{
	CELL_GEM_NO_VIDEO_OUTPUT=1,					///< Disable video output.
	CELL_GEM_RGBA_640x480,							///< RGBA output; 640*480*4-byte output buffer required.
	CELL_GEM_YUV_640x480,								///< YUV output; 640*480+640*480+640*480-byte output buffer required (contiguous).
	CELL_GEM_YUV422_640x480,						///< YUV output; 640*480+320*480+320*480-byte outpu buffer required (contiguous).
	CELL_GEM_YUV411_640x480,						///< YUV411 output; 640*480+320*240+320*240-byte output buffer required (contiguous).
	CELL_GEM_RGBA_320x240,							///< RGBA output; 320*240*4-byte output buffer required.
	CELL_GEM_BAYER_RESTORED,						///< Bayer pattern output, 640x480, gamma and white balance applied, output buffer required.
	CELL_GEM_BAYER_RESTORED_RGGB,				///< Restored bayer output, 2x2 pixels rearranged into 320x240 RG1G2B.
	CELL_GEM_BAYER_RESTORED_RASTERIZED, ///< Restored bayer output, R,G1,G2,B rearranged into 4 contiguous 320x240 1-channel rasters.
} CellGemVideoConvertFormatEnum;


/**
 * @brief Flags for the <c><i>conversion_flags</i></c> member of the structure CellGemVideoConvertAttribute. 
 *
 * Enumeration of flags used to manipulate video output.
 * 
 * @see
 * CellGemVideoConvertAttribute
*/
typedef enum {
	CELL_GEM_AUTO_WHITE_BALANCE=0x1, ///< Dynamically adjust the red, green, and blue gains based on sphere color.
	CELL_GEM_GAMMA_BOOST=0x2, ///< Boost the dark areas to improve display.
	CELL_GEM_COMBINE_PREVIOUS_INPUT_FRAME=0x4, ///< Blend with the previous frame to reduce noise.
} CellGemVideoConvertFlagEnum;


/** 
 * @brief Video conversion initialization data.
 *
 * Structure containing the data with which the video conversion mode is initialized.
 *
 * @see
 * CellGemVideoConvertFormatEnum, CellGemVideoConvertFlagEnum, cellGemAttributeInit, 
 * cellGemVideoConvertAttributeInit, cellGemVideoConvertAttributeInitRgba
*/
typedef struct CellGemVideoConvertAttribute {
	int version;              ///< Library version: set to <c>CELL_GEM_VERSION</c>
	int output_format;        ///< Output format, one of CellGemVideoConvertFormatEnum
	int conversion_flags;     ///< Logical OR of conversion flags, from CellGemVideoConvertFlagEnum
	float gain;               ///< Overall gain adjustment
	float red_gain;           ///< Red gain. Usually <c>1.0f</c> if <c>CELL_GEM_AUTO_WHITE_BALANCE</c> is enabled
	float green_gain;         ///< Green gain. Usually <c>1.0f</c> if <c>CELL_GEM_AUTO_WHITE_BALANCE</c> is enabled
	float blue_gain;          ///< Blue gain. Usually <c>1.0f</c> if <c>CELL_GEM_AUTO_WHITE_BALANCE</c> is enabled
	void *buffer_memory;      ///< Optional. Used for summing multiple Bayer frames to improve image quality
	void *video_data_out;     ///< Output buffer for video frame. Overwritten between cellGemUpdateStart() and cellGemUpdateFinish().
	uint8_t alpha;            ///< For RGBA output, this is the alpha value [0,255]
} CellGemVideoConvertAttribute;


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

int32_t cellGemGetMemorySize(int max_connect);


/** 
 * @brief Initializes the CellGemAttribute structure.
 *
 * This function initializes the device by setting values in the CellGemAttribute structure.
 * 
 * @param attr           Pointer to struct to be initialized
 * @param max_connect    The maximum number of devices (up to <c>CELL_GEM_MAX_NUM</c>)
 * @param spurs          Required. A pointer to <c><i>CellSpurs</i></c>
 * @param spu_priorities Array of SPU priorities to use for GEM SPURS workload

 *
 *
 * @see
 * CellGemAttribute, cellGemInit
*/
static inline void cellGemAttributeInit(CellGemAttribute* attr, 
										uint32_t max_connect,
										void * memory_ptr,
										CellSpurs *spurs, 
										const uint8_t spu_priorities[8])
{
	attr->version = CELL_GEM_VERSION;
	attr->max_connect = max_connect;
	attr->spurs = spurs;
	attr->memory_ptr = memory_ptr;
	for(int i=0; i<8; ++i) attr->spu_priorities[i] = spu_priorities[i];
}


/** 
 * @brief Initializes libgem.
 * 
 * This function performs the required initialization for using libgem.
 * 
 * @param attr   Attribute structure initialized with cellGemAttributeInit()
 *
 * @return
 * <c>CELL_OK</c> for normal termination.
 * <c>CELL_GEM_ERROR_RESOURCE_ALLOCATION_FAILED</c> if failed to allocate resource.
 * <c>CELL_GEM_ERROR_INVALID_PARAMETER</c> if invalid parameter.
 * <c>CELL_GEM_ERROR_ALREADY_INITIALIZED</c> if libgem already initialized.
 * @see
 * cellGemAttributeInit
*/
int32_t cellGemInit(const CellGemAttribute *attr);


/** 
 * @brief Terminates libgem.
 * 
 * This function ends the operation of libgem, freeing allocated memory.
 * @return
 * <c>CELL_OK</c> for normal termination.
 * <c>CELL_GEM_ERROR_UNINITIALIZED</c> if libgem not initialized.
 *
*/
int32_t cellGemEnd(void);


/** 
 * @brief Prepares the PlayStation&reg;Eye camera for tracking.
 * 
 * This function prepares the PlayStation&reg;Eye camera for libgem tracking. You need to call this function once after 
 * cellGemInit() and after <c>cellCameraReset()</c> for proper operation of libgem.
 * The camera attributes affected are: AGC, AEC, AWB, GAMMA, PIXELOUTLIERFILTER (all disabled).
 * In addition, GAIN, EXPOSURE, REDBLUE_GAIN, and GREEN_GAIN are adjusted as needed by libgem.
 *
 * @param   max_exposure		The setting for the maximum allowed exposure (80-511).
								Sets the maximum possible # of image rows of exposure time (511 rows in each camera frame).
								Note that a longer exposure time means decreased image noise but increased motion blur, which 
								has a negative effect on libgem tracking performance.
 * @param   image_quality		Adjusts the image quality level (0.0-1.0).  A setting of 1.0 is the best possible quality while 
								0.0 is the best possible tracking. 
 *
 * @return
 * <c>CELL_OK</c> for normal termination.
 * <c>CELL_GEM_ERROR_UNINITIALIZED</c> if libgem not initialized.
 * @see
 * <c>cellCameraReset</c> (see <i>libcamera Reference</i>), cellGemInit
*/
int32_t cellGemPrepareCamera(int max_exposure, float image_quality);


/** 
 * @brief Determines required video output buffer size based on the format of the specified video.
 * 
 * This function determines the required video output buffer size based on video format. 
 * The <c><i>video_data_out</i></c> member of CellGemVideoConvertAttribute structure points to the 
 * video output buffer.
 * 
 * @param output_format Convert format, one of CellGemVideoConvertFormatEnum
 *
 * @return
 * -1 if unknown <c><i>output_format</i></c>. Otherwise, the required size of video output buffer.
 *
 * @see
 * CellGemVideoConvertAttribute, CellGemVideoConvertFormatEnum, cellGemVideoConvertAttributeInit
*/
static inline int32_t cellGemGetVideoConvertSize(int output_format)
{
	switch(output_format)
	{
	case CELL_GEM_NO_VIDEO_OUTPUT:
		return 0;
	case CELL_GEM_RGBA_640x480:
		return 640*480*4;
	case CELL_GEM_YUV_640x480:
		return 640*480*3;
	case CELL_GEM_RGBA_320x240:
		return 320*240*4;
	case CELL_GEM_YUV411_640x480:
		return 640*480+320*240+320*240;
	case CELL_GEM_YUV422_640x480:
		return 640*480+320*480+320*480;
	case CELL_GEM_BAYER_RESTORED:
		return 640*480;
	case CELL_GEM_BAYER_RESTORED_RGGB:
		return 320*240*4;
	case CELL_GEM_BAYER_RESTORED_RASTERIZED:
		return 320*240*4;
	default:
		return -1;
	}
}


/** 
 * @brief Initializes the CellGemVideoConvertAttribute structure.
 * 
 * This function initializes the CellGemVideoConvertAttribute structure.
 * 
 * @param retAttr Attribute structure to initialize
 * @param output_format Output format, one of CellGemVideoConvertFormatEnum
 * @param conversion_flags Logical OR of conversion flags, from CellGemVideoConvertFlagEnum
 * @param gain       Overall gain adjustment
 * @param red_gain   Red gain; Usually <c>1.0f</c> if <c>CELL_GEM_AUTO_WHITE_BALANCE</c> is enabled
 * @param green_gain Green gain; Usually <c>1.0f</c> if <c>CELL_GEM_AUTO_WHITE_BALANCE</c> is enabled
 * @param blue_gain  Blue gain; Usually <c>1.0f</c> if <c>CELL_GEM_AUTO_WHITE_BALANCE</c> is enabled
 * @param buffer_memory  Optional. Used for summing multiple Bayer frames to improve image quality
 * @param video_data_out  Output buffer for video frame. Overwritten between cellGemUpdateStart() and cellGemUpdateFinish().
 * @param alpha  For RGBA output, this is the alpha value [0,255]
 *
 * @see
 * CellGemVideoConvertAttribute, CellGemVideoConvertFlagEnum, 
 * CellGemVideoConvertFormatEnum, cellGemPrepareVideoConvert, cellGemVideoConvertAttributeInitRgba, 
 * cellGemGetVideoConvertSize
*/
static inline void cellGemVideoConvertAttributeInit(CellGemVideoConvertAttribute* retAttr,
													int output_format,
													int conversion_flags,
													float gain,
													float red_gain,
													float green_gain,
													float blue_gain,
													void *buffer_memory,
													void *video_data_out,
													uint8_t alpha)
{
	retAttr->version = CELL_GEM_VERSION;
	retAttr->output_format = output_format;
	retAttr->conversion_flags = conversion_flags;
	retAttr->gain = gain;
	retAttr->red_gain = red_gain;
	retAttr->green_gain = green_gain;
	retAttr->blue_gain = blue_gain;
	retAttr->buffer_memory = buffer_memory;
	retAttr->video_data_out = video_data_out;
	retAttr->alpha = alpha;
}


/** 
 * @brief Initializes the CellGemVideoConvertAttribute structure.
 * 
 * This function initializes the CellGemVideoConvertAttribute structure with defaults for
 * RGBA 640x480 video output.
 * 
 * @param retAttr Attribute structure to initialize
 * @param rgba_out Pointer to 640x480x4 RGBA output buffer
 * @param frame_buffer Optional 640x480x1 128-byte-aligned buffer for adding previous frame to improve image quality
 *
 * @see
 * cellGemPrepareVideoConvert, cellGemVideoConvertAttributeInit, 
 * CellGemVideoConvertAttribute, cellGemGetVideoConvertSize
*/
static inline void cellGemVideoConvertAttributeInitRgba(CellGemVideoConvertAttribute* retAttr,
														void *rgba_out, void *frame_buffer)
{
	int conversion_flags = CELL_GEM_AUTO_WHITE_BALANCE|CELL_GEM_GAMMA_BOOST;
	if(frame_buffer) conversion_flags |= CELL_GEM_COMBINE_PREVIOUS_INPUT_FRAME;
	cellGemVideoConvertAttributeInit(retAttr,
		CELL_GEM_RGBA_640x480,
		conversion_flags,
		1.0f,1.0f,1.0f,1.0f,
		frame_buffer,
		rgba_out,
		255);
}


/** 
 * @brief Initializes libgem video output mode.
 * 
 * If this function is never called, video output processing is disabled.
 * Call this function whenever the video output settings change.
 * 
 * @param attr Attribute structure initialized with cellGemVideoConvertAttributeInit()
 *
 * @return
 * <c>CELL_OK</c> for normal termination.
 * <c>CELL_GEM_ERROR_UNINITIALIZED</c> if libgem not initialized.
 * <c>CELL_GEM_ERROR_INVALID_PARAMETER</c> if invalid parameter.
 * <c>CELL_GEM_ERROR_INVALID_ALIGNMENT</c> if invalid buffer alignment.
 *
 * @see
 * CellGemVideoConvertAttribute, cellGemVideoConvertAttributeInit, cellGemVideoConvertAttributeInitRgba
*/
int32_t cellGemPrepareVideoConvert(const CellGemVideoConvertAttribute* attr);

/** 
 * @brief Start conversion of video obtained from the camera.
 * 
 * This function starts asynchronously converting a video frame from 
 * <c>cellCameraReadEx()</c>.  It uses a separate job chain from the
 * update start and finish, so it can be done independently.
 * 
 * @param   camera_frame   Values from <c>CellCameraInfoEx.buffer</c> (from PlayStation&reg;Eye <c>camera.h</c>)
 *
 * @return
 * <c>CELL_OK</c> for normal termination.
 * <c>CELL_GEM_ERROR_UNINITIALIZED</c> if libgem not initialized.
 * <c>CELL_GEM_ERROR_INVALID_PARAMETER</c> if invalid parameter.
 * <c>CELL_GEM_ERROR_INVALID_ALIGNMENT</c> if invalid buffer alignment.
 * @see
 * <c> cellConvertVideoStart</c>, <c>CellCameraInfoEx</c>, <c>cellCameraReadEx</c> (see <i>libcamera Reference</i>)
*/
int32_t cellGemConvertVideoStart(const void *video_frame);

/** 
 * @brief Wait for asynchronous SPU processing of cellGemConvertVideoStart() to complete.
 * 
 * This function waits for asynchronous SPU processing of cellGemConvertVideoStart() to complete.
 * The output image buffer is only valid after calling this function.
 * 
 * @return
 * <c>CELL_OK</c> for normal termination.
 * <c>CELL_GEM_ERROR_UNINITIALIZED</c> if libgem not initialized.
 * @see
 * <c>cellGemConvertVideoStart</c>
*/
int32_t cellGemConvertVideoFinish();

/** 
 * @brief Start processing video obtained from the camera.
 * 
 * This function starts asynchronously processing a video frame from 
 * <c>cellCameraReadEx()</c> to track position of connected devices.
 * 
 * @param   camera_frame   Values from <c>CellCameraInfoEx.buffer</c> (from PlayStation&reg;Eye <c>camera.h</c>)
 * @param   timestamp      Result from <c>cellCameraReadEx()</c>
 *
 * @return
 * <c>CELL_OK</c> for normal termination.
 * <c>CELL_GEM_ERROR_UNINITIALIZED</c> if libgem not initialized.
 * <c>CELL_GEM_ERROR_INVALID_PARAMETER</c> if invalid parameter.
 * <c>CELL_GEM_ERROR_INVALID_ALIGNMENT</c> if invalid buffer alignment.
 * @see
 * <c>CellCameraInfoEx</c>, <c>cellCameraReadEx</c> (see <i>libcamera Reference</i>)
*/
int32_t cellGemUpdateStart(const void *camera_frame, system_time_t timestamp);


/** 
 * @brief Wait for asynchronous SPU processing of cellGemUpdateStart() to complete.
 * 
 * This function waits for asynchronous SPU processing of cellGemUpdateStart() to complete.
 * It must be called for new camera and inertial sensor data to be utilized.
 * 
 * @return
 * <c>CELL_OK</c> for normal termination.
 * <c>CELL_GEM_ERROR_UNINITIALIZED</c> if libgem not initialized.
 * @see
 * cellGemUpdateStart
*/
int32_t cellGemUpdateFinish();

/** 
 * @brief Retrieves libgem status flags.
 * 
 * This function retrieves the latched libgem state including calibration feedback
 * and runtime warnings. The full extent of the flags is described in the overview.
 * 
 * @param   gem_num     The device from which the data is retrieved (0 - 3)
 * @param   flags		Pointer to variable recieving the flags
 *
 * @return
 * <c>CELL_OK</c> for normal termination.
 * <c>CELL_GEM_ERROR_UNINITIALIZED</c> if libgem not initialized.
 * <c>CELL_GEM_ERROR_INVALID_PARAMETER</c> if invalid parameter.
 * @see
 *  <c>cellGemClearStatusFlags</c> 
*/
int32_t cellGemGetStatusFlags(int gem_num, uint64_t* flags);

/** 
 * @brief Clears some or all of libgem status flags.
 * 
 * This function clears libgem status flags. The flags to be cleared must be marked
 * with a binary 1 in the mask. To clear all flags use CELL_GEM_ALL_FLAGS.
 * 
 * @param   gem_num     The device from which the data is retrieved (0 - 3)
 * @param   mask		Mask indicating which flags are to be cleared
 *
 * @return
 * <c>CELL_OK</c> for normal termination.
 * <c>CELL_GEM_ERROR_UNINITIALIZED</c> if libgem not initialized.
 * <c>CELL_GEM_ERROR_INVALID_PARAMETER</c> if invalid parameter.
 * @see
 *  <c>cellGemGetStatusFlags</c> 
*/
int32_t cellGemClearStatusFlags(int gem_num, uint64_t mask);

/** 
 * @brief Retrieves the latest device state.
 * 
 * Retrieves the state of the device combining the results of cellGemUpdateStart(), cellGemUpdateFinish(), and data from
 * inertial sensors. 
 * 
 * @param   gem_num     The device from which the data is retrieved (0 - 3)
 * @param   flag	      The type of position data to retrieve:
 *			   <ul><li><c>CELL_GEM_STATE_FLAG_CURRENT_TIME</c>: State matched to the current system time 
 *                                 when cellGemGetState() is called.  This is the best choice to use 
 *                                 when the video is not displayed.</li>
 *			   <li><c>CELL_GEM_STATE_FLAG_LATEST_IMAGE_TIME</c>: State matched to time the sphere was 
 *                                 imaged in the latest camera frame. This is the best choice to use 
 *																 when the video is displayed.</li>
 * @param   time_offset The time offset (in milliseconds) relative to either the current time or latest image time.  
                        Note: a negative value means earlier in time.
 * @param   gem_state   The latest device state organized as a CellGemState structure, for return.
 *
 * @return
 * <c>CELL_OK</c> for normal termination.
 * <c>CELL_GEM_ERROR_UNINITIALIZED</c> if libgem not initialized.
 * <c>CELL_GEM_ERROR_INVALID_PARAMETER</c> if invalid parameter.
 * 
 * @see
 * CellGemState, cellGemUpdateFinish, cellGemUpdateStart
*/
int32_t	cellGemGetState(uint32_t gem_num, uint32_t flag, float time_offset, CellGemState* gem_state);


/** 
 * @brief Retrieves the low-level device inertial sensor state.
 * 
 * This function retrieves the latest inertial sensor readings.
 * Sensor readings are in device coordinates (not world coordinates). Refer to the coordinate description at CellGemState.
 *
 * @note
 * Accelerometer position relative to the center of the sphere is 
 * provided by cellGemGetAccelerometerPositionInDevice().
 * 
 * @note
 * We generally recommend that you use cellGemGetState() to retrieve the high-level device state 
 * rather than use this low-level inertial data.
 * 
 * @param    gem_num	     The device from which the data is retrieved (0 - 3)
 * @param    history_index   An offset from the last reading into past readings, with a maximum value of 31
 * @param    gem_inertial_state	     The latest sensor readings organized as a CellGemInertialState structure
 *
 * @return
 * <c>CELL_OK</c> for normal termination.
 * <c>CELL_GEM_ERROR_UNINITIALIZED</c> if libgem not initialized.
 * <c>CELL_GEM_ERROR_INVALID_PARAMETER</c> if invalid parameter.
 * @see
 * CellGemInertialState, cellGemGetState, cellGemGetAccelerometerPositionInDevice
*/
 int32_t	cellGemGetInertialState(uint32_t gem_num, uint32_t history_index, CellGemInertialState* gem_inertial_state);


/** 
 * @brief Retrieves accelerometer position offset from the sphere.
 * 
 * This function retrieves the accelerometer position as it is offset from the center of the sphere, in device coordinates.
 * Refer to the coordinate description at CellGemState.
 * 
 * @param    gem_num	    The device for which the data is retrieved (0 - 3)
 * @param    pos		    Position offset (in mm)
 *
 * @return
 * <c>CELL_OK</c> for normal termination.
 * <c>CELL_GEM_ERROR_UNINITIALIZED</c> if libgem not initialized.
 * <c>CELL_GEM_ERROR_INVALID_PARAMETER</c> if invalid parameter.
 * @see
 * CellGemState, CellGemInertialState, cellGemGetInertialState
*/
int32_t cellGemGetAccelerometerPositionInDevice(uint32_t gem_num, vec_float4* pos);


/** 
 * @brief Retrieves the state from vision processing of the latest image.
 * 
 * Retrieves the tracking result from vision processing of the latest image performed with 
 * cellGemUpdateStart() and cellGemUpdateFinish().
 * 
 * @param    gem_num	  The device for which the data is retrieved (0 - 3)
 * @param    gem_image_state	  The latest image state organized as a CellGemImageState structure
 *
 * @return
 * <c>CELL_OK</c> for normal termination.
 * <c>CELL_GEM_ERROR_UNINITIALIZED</c> if libgem not initialized.
 * <c>CELL_GEM_ERROR_INVALID_PARAMETER</c> if invalid parameter.
 * @see
 * CellGemImageState, cellGemUpdateFinish, cellGemUpdateStart
*/
int32_t cellGemGetImageState(uint32_t gem_num, CellGemImageState* gem_image_state);

/** 
 * @brief Retrieves the state of the camera.
 * 
 * Retrieves the state specifying current camera parameters.
 * 
 * @param    gem_camera_state	  The latest camera state organized as a CellGemImageState structure
 *
 * @return
 * <c>CELL_OK</c> for normal termination.
 * <c>CELL_GEM_ERROR_UNINITIALIZED</c> if libgem not initialized.
 * <c>CELL_GEM_ERROR_INVALID_PARAMETER</c> if invalid parameter.
 * @see
 * CellGemCameraState
*/
int32_t cellGemGetCameraState(CellGemCameraState* gem_camera_state);


/** 
 * @brief Forces the sphere to an exact RGB color.
 * 
 * This function allows you to force the sphere to be a specific color.
 * Positional tracking is disabled if the color is set this way. Also, the forced
 * color may worsen tracking for other devices if multiple devices are in use.
 * 
 * @param    gem_num	    The device for which the color is set (0 - 3)
 * @param    r				Red component of color, 0.0 to 1.0
 * @param    g				Green component of color, 0.0 to 1.0
 * @param    b				Blue component of color, 0.0 to 1.0
 *
 * @return
 * <c>CELL_OK</c> for normal termination.
 * <c>CELL_GEM_ERROR_UNINITIALIZED</c> if libgem not initialized.
 * <c>CELL_GEM_ERROR_INVALID_PARAMETER</c> if invalid parameter.
 * @see
 * cellGemGetRGB, cellGemTrackHue
*/
int32_t cellGemForceRGB(uint32_t gem_num, float r, float g, float b);


/** 
 * @brief Produces a vibration in the device.
 * 
 * This function allows you to produce vibration in the device to provide
 * feedback to the user.
 * 
 * @param    gem_num	    The device for which the vibration is set (0 - 3)
 * @param    rumble         Intensity of the vibration, from 0 to 255
 *
 * @return
 * <c>CELL_OK</c> for normal termination.
 * <c>CELL_GEM_ERROR_UNINITIALIZED</c> if libgem not initialized.
 * <c>CELL_GEM_ERROR_INVALID_PARAMETER</c> if invalid parameter.
 * @see
 * cellGemGetRumble
*/
int32_t cellGemSetRumble(uint32_t gem_num, unsigned char rumble);


/** 
 * @brief Retrieves the value of the device vibration.
 * 
 * This function allows you to retrieve the vibration setting.
 * 
 * @param    gem_num	    The device for which the vibration setting is retrieved (0 - 3)
 * @param    rumble         Intensity of the vibration, from 0 to 255
 *
 * @return
 * <c>CELL_OK</c> for normal termination.
 * <c>CELL_GEM_ERROR_UNINITIALIZED</c> if libgem not initialized.
 * <c>CELL_GEM_ERROR_INVALID_PARAMETER</c> if invalid parameter.
 * @see
 * cellGemSetRumble
*/
int32_t cellGemGetRumble(uint32_t gem_num,unsigned char* rumble);


/** 
 * @brief Retrieves the exact RGB color of the sphere LED.
 * 
 * This function retrieves the current color of the device sphere.
 * 
 * @param    gem_num	The device for which the color is retrieved (0 - 3)
 * @param    r		Pointer to variable receiving red component of color
 * @param    g		Pointer to variable receiving green component of color
 * @param    b		Pointer to variable receiving blue component of color
 *
 * @return
 * <c>CELL_OK</c> for normal termination.
 * <c>CELL_GEM_ERROR_UNINITIALIZED</c> if libgem not initialized.
 * <c>CELL_GEM_ERROR_INVALID_PARAMETER</c> if invalid parameter.
 * @see
 * cellGemForceRGB, cellGemTrackHue
*/
int32_t cellGemGetRGB(uint32_t gem_num, float* r, float* g, float* b);


/** 
 * @brief Retrieves the current tracked hue.
 * 
 * This function retrieves the tracking hue the device.
 * 
 * @param    gem_num	The device for which the hue is retrieved (0 - 3)
 * @param    hue		Pointer to variable receiving the hue
 *
 * @return
 * <c>CELL_OK</c> for normal termination.
 * <c>CELL_GEM_ERROR_NOT_A_HUE</c> indicates that the device is not currently tracked and the received hue is invalid.
 * <c>CELL_GEM_ERROR_UNINITIALIZED</c> if libgem not initialized.
 * <c>CELL_GEM_ERROR_INVALID_PARAMETER</c> if invalid parameter.
 * @see
 * cellGemTrackHue
*/
int32_t cellGemGetTrackerHue(uint32_t gem_num, int* hue);


/** 
 * @brief Converts a point in HSV space to RGB space using libgem's internal converter.
 * 
 * This function is provided because there is no single standard method for HSV to RGB conversion.
 * This version of the library treats HSV as a hexacone. (http://en.wikipedia.org/wiki/Hue)
 * Using this function will allow the application display hues using RGB values.
 * This function can also be used to convert hues for a cellGemForceRGB call.
 * 
 * @param    h		Input hue angle. Values outside of [0-359] will be wrapped.
 * @param    s		Input saturation, range [0-1]
 * @param    v		Input value (brightness), range [0-1] 
 * @param    r		Pointer to variable receiving output red component of color
 * @param    g		Pointer to variable receiving output green component of color
 * @param    b		Pointer to variable receiving output blue component of color

 * @return
 * <c>CELL_OK</c> for normal termination.
 * <c>CELL_GEM_ERROR_INVALID_PARAMETER</c> if invalid parameter.
*/
int32_t cellGemHSVtoRGB(float h, float s, float v, float* r, float* g, float* b);


/** 
 * @brief Query the library about whether or not a hue will be trackable.
 * 
 * This function checks if a given hue will succeed in tracking 
 * without actually changing the sphere to that hue.
 * 
 * @param    gem_num	The device for which the data is retrieved (0 - 3)
 * @param    hue		The hue to check (0-359 degrees inclusive)
 *
 * @return
 * <c>1</c> for trackable.
 * <c>0</c> for not trackable.
 * @note
 * Hues outside of the 0-359 range are wrapped back into this range.
 * The function will always return 0 during sphere calibration.
 * @see
 * cellGemTrackHue
*/
int32_t cellGemIsTrackableHue(uint32_t gem_num, int hue);


/** 
 * @brief Set a given hue for tracking and begin tracking.
 * 
 * This function sets the sphere color to a given hue.  If tracking is
 * already occurring, tracking will be maintained.  If tracking has not started yet,
 * tracking will begin. If a hue cannot be tracked, the function will return 0 and 
 * make no changes to the current sphere color or the hue to be tracked.
 * 
 * @param    gem_num	The device for which the data is retrieved (0 - 3)
 * @param    hue		The hue to set (0-359 degrees inclusive)
 *
 * @return
 * <c>1</c> for successful hue set.
 * <c>0</c> for failure (hue unchanged).
 * @note
 * Hues outside of the 0-359 range are wrapped back into this range.
 * The function will always return 0 during sphere calibration.
 * @see
 * cellGemIsTrackableHue
*/
int32_t cellGemTrackHue(uint32_t gem_num, int hue);


/** 
 * @brief Retrieves the estimated color of the lighting in the current environment at the current exposure.
 * 
 * This function retrieves the color that the unlit sphere appears to the camera.
 * The actual values are computed during sphere calibration for the last device that was calibrated.
 * These values can be used for game lighting in augmented reality or image white balance.
 * 
 * @param    r		Pointer to variable receiving red component of color
 * @param    g		Pointer to variable receiving green component of color
 * @param    b		Pointer to variable receiving blue component of color
 *
 * @return
 * <c>CELL_OK</c> for normal termination.
 * <c>CELL_GEM_ERROR_LIGHTING_NOT_CALIBRATED</c> indicates that lighting has not been calibrated for. Values returned are 128,128,128.
 * <c>CELL_GEM_ERROR_UNINITIALIZED</c> if libgem not initialized.
 * <c>CELL_GEM_ERROR_INVALID_PARAMETER</c> if invalid parameter.
 * @note
 * <c>CELL_GEM_ERROR_LIGHTING_NOT_CALIBRATED</c> is returned after initial connection and can be corrected by performing 
 * a calibration with cellGemCalibrate() on at least one device.
 * @see
 * cellGemCalibrate
*/
int32_t cellGemGetEnvironmentLightingColor(float* r, float* g, float* b);


/** 
 * @brief Retrieves device connection information.
 * 
 * This function retrieves device connection information for all currently connected devices.
 * 
 * @param   info	Device connection information organized as a CellGemInfo structure
 *
 * @return
 * <c>CELL_OK</c> for normal termination.
 * @see
 * CellGemInfo
*/
int32_t cellGemGetInfo(CellGemInfo *info);


/** 
 * @brief Resets a device to the state it was in when first connected.
 * 
 * Call this function to reset a device to the state it was in when first connected.
 * This function is automatically called internally upon initial connection.
 * 
 * @param    gem_num	    The device for which the state is set (0 - 3)
 *
 * @return
 * <c>CELL_OK</c> for normal termination.
 * <c>CELL_GEM_ERROR_UNINITIALIZED</c> if libgem not initialized.
 * <c>CELL_GEM_ERROR_INVALID_PARAMETER</c> if invalid parameter.
 * @see
 * cellGemGetInfo
*/
int32_t cellGemReset(uint32_t gem_num);


/** 
 * @brief Initiates calibration.
 * 
 * This function begins the calibration procedure. The device should be held still during calibration; 
 * however, the system can accommodate minor movements (such as the device held by an unstable hand).
 * The device must be pointed at the PlayStation Eye when calibrating, as calibration adjusts the
 * camera pitch angle.
 * Calibration also clears the magnetic map entries created by cellGemSetYaw().
 * 
 * @param    gem_num	    The device to which to apply the setting (0 - 3)
 *
 * @return
 * <c>CELL_OK</c> for normal termination.
 * <c>CELL_GEM_ERROR_UNINITIALIZED</c> if libgem not initialized.
 * <c>CELL_GEM_ERROR_INVALID_PARAMETER</c> if invalid parameter.
*/
int32_t cellGemCalibrate(uint32_t gem_num);


/** 
 * @brief Invalidates calibration.
 * 
 * This function invalidates the calibration. It is used to put the device into an uncalibrated
 * state to necessitate recalibration. Use it to signal that a device needs to be recalibrated, perhaps if the system detected a
 * change in the lighting environment during calibration of another device.
 * 
 * @param    gem_num	    The device to which to apply the setting (0 - 3)
 *
 * @return
 * <c>CELL_OK</c> for normal termination.
 * <c>CELL_GEM_ERROR_UNINITIALIZED</c> if libgem not initialized.
 * <c>CELL_GEM_ERROR_INVALID_PARAMETER</c> if invalid parameter.
*/
int32_t cellGemInvalidateCalibration(int32_t gem_num);


/** 
 * @brief Controls the performance of state filtering
 * 
 * This function sets whether to perform filtering to increase smoothness of 
 * CellGemState output. Enabling filtering can cause some increase of tracking latency. The 
 * default behavior does not perform filtering.
 * 
 * @param    gem_num	    The device for which to apply the setting (0 - 3)
 * @param    flag           Set to 1 to enable state filtering
 *
 * @return
 * <c>CELL_OK</c> for normal termination.
 * <c>CELL_GEM_ERROR_UNINITIALIZED</c> if libgem not initialized.
 * <c>CELL_GEM_ERROR_INVALID_PARAMETER</c> if invalid parameter.
 * @see 
 * CellGemState, cellGemGetState
*/
int32_t cellGemFilterState(uint32_t gem_num, uint32_t flag);


/** 
 * @brief Controls the use of automatic camera pitch angle computation.
 * 
 * This function controls whether libgem uses data from the magnetometer to correct 
 * cellGemState.  The magnetometer is most useful for applications requiring precise
 * yaw angle, such as pointing or shooting.
 * 
 * @param    gem_num	The device for which to apply the setting (0 - 3)
 * @param    flag			Set to 0 to disable use of the magnetometer
 *
 * @return
 * <c>CELL_OK</c> for normal termination.
 * <c>CELL_GEM_ERROR_UNINITIALIZED</c> if libgem not initialized.
*/
int32_t cellGemEnableMagnetometer(uint32_t gem_num, int flag);


/** 
 * @brief Controls the use of automatic camera pitch angle computation.
 * 
 * This function controls whether libgem's automatically computed estimation of the camera 
 * pitch angle is used for CellGemState computation.  libgem always computes an
 * estimate of the camera pitch angle; this controls whether or not it is constantly applied.
 * 
 * @param    flag	    Set to 0 to disable automatic update of estimated pitch angle
 *
 * @return
 * <c>CELL_OK</c> for normal termination.
 * <c>CELL_GEM_ERROR_UNINITIALIZED</c> if libgem not initialized.
*/
int32_t cellGemEnableCameraPitchAngleCorrection(int flag);

/** 
 * @brief Specifies the current direction of the <c>z</c> axis of the device
 * in world space.
 * 
 * This function allows setting the device&#8216;s <c>z</c> direction to correct the 
 * yaw angle.  It can be used to manually correct yaw drift while the device is pointing at the camera 
 * or other known target.  
 *
 * The primary use for this function is to call it when the device is visible 
 * and known to be pointed directly at the camera.  In such a case, the <c>z</c> direction is
 * the device position as returned in CellGemState (see any of the samples).
 * 
 * When the magnetometer correction is enabled, cellGemSetYaw adds a reference point to a
 * temporary magnetic map of the environment.  This will allow for more precise angle tracking
 * near this location.  If the magnetometer is enabled and cellGemSetYaw is called, the controller
 * must be kept nearly stationary for 0.5seconds so that a magnetic reading can be taken.  If
 * successful, the sphere LED will flicker off for 30ms to signify a magnetic reference was added.
 * Refer to the coordinate description in CellGemState.
 * 
 * @param    gem_num	    The device for which to apply the setting (0 - 3)
 * @param    z_direction    The <c>z</c> direction of the device in world coordinates
 *
 * @return
 * <c>CELL_OK</c> for normal termination.
 * <c>CELL_GEM_ERROR_UNINITIALIZED</c> if libgem not initialized.
 * <c>CELL_GEM_ERROR_INVALID_PARAMETER</c> if invalid parameter.
 * @see
 * CellGemState
*/
int32_t cellGemSetYaw(uint32_t gem_num, vec_float4 z_direction);


#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif
