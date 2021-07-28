/*   SCE CONFIDENTIAL                                       */
/*   PlayStation(R)3 Programmer Tool Runtime Library 320.001 */
/*   Copyright (C) 2006 Sony Computer Entertainment Inc.    */
/*   All Rights Reserved.                                   */
#ifndef __CELL_PAD_PAD_CODES_H__
#define __CELL_PAD_PAD_CODES_H__

#include <sys/types.h>

#define CELL_MAX_PADS 127
#define CELL_PAD_MAX_CODES (64)
#define CELL_PAD_MAX_PORT_NUM (7)

/*E CellPadInfo2: System Info */
#define CELL_PAD_INFO_INTERCEPTED       (1 << 0)    /*E input intercepted */
/*E CellPadInfo2: Port Status */
#define CELL_PAD_STATUS_DISCONNECTED    (0)         /*E Game Pad is disconnected  */
#define CELL_PAD_STATUS_CONNECTED       (1 << 0)    /*E Game Pad is connected.    */
#define CELL_PAD_STATUS_ASSIGN_CHANGES  (1 << 1)    /*E */
/*E CellPadInfo2: Port Setting */
#define CELL_PAD_SETTING_PRESS_ON   (1 << 1)    /* Press mode turn on */
#define CELL_PAD_SETTING_SENSOR_ON  (1 << 2)    /* Sensor mode turn on */
#define CELL_PAD_SETTING_PRESS_OFF  (0)
#define CELL_PAD_SETTING_SENSOR_OFF (0)
/*E CellPadInfo2: Capability */
#define CELL_PAD_CAPABILITY_PS3_CONFORMITY  (1 << 0)    /* PS3 Conformity Controller */
#define CELL_PAD_CAPABILITY_PRESS_MODE      (1 << 1)    /* Press mode supported */
#define CELL_PAD_CAPABILITY_SENSOR_MODE     (1 << 2)    /* Sensor mode supported */
#define CELL_PAD_CAPABILITY_HP_ANALOG_STICK (1 << 3)    /* High Precision ANALOG STICK */
#define CELL_PAD_CAPABILITY_ACTUATOR        (1 << 4)    /* Motor supported */
/*E CellPadInfo2: Device Type */
#define CELL_PAD_DEV_TYPE_STANDARD  (0)         /*E not special */
#define CELL_PAD_DEV_TYPE_LDD       (5)         /*E Custom Controller */

typedef struct CellPadInfo2
{
    uint32_t max_connect;
    uint32_t now_connect;
    uint32_t system_info;
    uint32_t port_status[CELL_PAD_MAX_PORT_NUM];
    uint32_t port_setting[CELL_PAD_MAX_PORT_NUM];
    uint32_t device_capability[CELL_PAD_MAX_PORT_NUM];
    uint32_t device_type[CELL_PAD_MAX_PORT_NUM];
} CellPadInfo2;

/*E CellPadData : Byte offset of button data.  */
typedef enum CellPadButtonDataOffset{
    CELL_PAD_BTN_OFFSET_DIGITAL1 = 2,
    CELL_PAD_BTN_OFFSET_DIGITAL2 = 3,
    CELL_PAD_BTN_OFFSET_ANALOG_RIGHT_X = 4,
    CELL_PAD_BTN_OFFSET_ANALOG_RIGHT_Y = 5,
    CELL_PAD_BTN_OFFSET_ANALOG_LEFT_X  = 6,
    CELL_PAD_BTN_OFFSET_ANALOG_LEFT_Y  = 7,
    CELL_PAD_BTN_OFFSET_PRESS_RIGHT    = 8,
    CELL_PAD_BTN_OFFSET_PRESS_LEFT     = 9,
    CELL_PAD_BTN_OFFSET_PRESS_UP       = 10,
    CELL_PAD_BTN_OFFSET_PRESS_DOWN     = 11,
    CELL_PAD_BTN_OFFSET_PRESS_TRIANGLE = 12,
    CELL_PAD_BTN_OFFSET_PRESS_CIRCLE   = 13,
    CELL_PAD_BTN_OFFSET_PRESS_CROSS    = 14,
    CELL_PAD_BTN_OFFSET_PRESS_SQUARE   = 15,
    CELL_PAD_BTN_OFFSET_PRESS_L1       = 16,
    CELL_PAD_BTN_OFFSET_PRESS_R1       = 17,
    CELL_PAD_BTN_OFFSET_PRESS_L2       = 18,
    CELL_PAD_BTN_OFFSET_PRESS_R2       = 19,
    CELL_PAD_BTN_OFFSET_SENSOR_X       = 20,
    CELL_PAD_BTN_OFFSET_SENSOR_Y       = 21,
    CELL_PAD_BTN_OFFSET_SENSOR_Z       = 22,
    CELL_PAD_BTN_OFFSET_SENSOR_G       = 23
}CellPadButtonDataOffset;

/*E CellPadData : Bit assignment of digital buttons. */
/*E "CELL_PAD_BTN_OFFSET_DIGITAL1" assignment */
#define CELL_PAD_CTRL_LEFT      (1 << 7)
#define CELL_PAD_CTRL_DOWN      (1 << 6)
#define CELL_PAD_CTRL_RIGHT     (1 << 5)
#define CELL_PAD_CTRL_UP        (1 << 4)
#define CELL_PAD_CTRL_START     (1 << 3)
#define CELL_PAD_CTRL_R3        (1 << 2)
#define CELL_PAD_CTRL_L3        (1 << 1)
#define CELL_PAD_CTRL_SELECT    (1 << 0)

/*E "CELL_PAD_BTN_OFFSET_DIGITAL2" assignment */
#define CELL_PAD_CTRL_SQUARE    (1 << 7)
#define CELL_PAD_CTRL_CROSS     (1 << 6)
#define CELL_PAD_CTRL_CIRCLE    (1 << 5)
#define CELL_PAD_CTRL_TRIANGLE  (1 << 4)
#define CELL_PAD_CTRL_R1        (1 << 3)
#define CELL_PAD_CTRL_L1        (1 << 2)
#define CELL_PAD_CTRL_R2        (1 << 1)
#define CELL_PAD_CTRL_L2        (1 << 0)

/*E PS BUTTON for cellPadLddDataInsert */
#define CELL_PAD_CTRL_LDD_PS    (1 << 0)

typedef struct CellPadData{
  int32_t len;
  uint16_t button[CELL_PAD_MAX_CODES];
}CellPadData;

#define CELL_PAD_ACTUATOR_MAX   (2)
typedef struct CellPadActParam
{
    uint8_t motor[CELL_PAD_ACTUATOR_MAX];
    uint8_t reserved[6];
} CellPadActParam;


/* Peripheral class device */

typedef enum {
    CELL_PAD_PCLASS_TYPE_STANDARD = 0,
    CELL_PAD_PCLASS_TYPE_GUITAR  = 1,
    CELL_PAD_PCLASS_TYPE_DRUM    = 2,
    CELL_PAD_PCLASS_TYPE_DJ      = 3,
}  CellPadPariphClassType;

typedef struct {
    uint32_t max_connect;
    uint32_t now_connect;
    uint32_t system_info;
    uint32_t port_status[CELL_PAD_MAX_PORT_NUM];
    uint32_t port_setting[CELL_PAD_MAX_PORT_NUM];
    uint32_t device_capability[CELL_PAD_MAX_PORT_NUM];
    uint32_t device_type[CELL_PAD_MAX_PORT_NUM];
    uint32_t pclass_type[CELL_PAD_MAX_PORT_NUM];
    uint32_t pclass_profile[CELL_PAD_MAX_PORT_NUM];
} CellPadPeriphInfo;

typedef struct {
  uint32_t pclass_type;
  uint32_t pclass_profile;
  int32_t len;
  uint16_t button[CELL_PAD_MAX_CODES];
} CellPadPeriphData;

/*E CellPadPeriphInfo: Periph device class profile - Guitar */
#define CELL_PAD_PCLASS_PROFILE_GUITAR_FRET_1      (1<<0)
#define CELL_PAD_PCLASS_PROFILE_GUITAR_FRET_2      (1<<1)
#define CELL_PAD_PCLASS_PROFILE_GUITAR_FRET_3      (1<<2)
#define CELL_PAD_PCLASS_PROFILE_GUITAR_FRET_4      (1<<3)
#define CELL_PAD_PCLASS_PROFILE_GUITAR_FRET_5      (1<<4)
#define CELL_PAD_PCLASS_PROFILE_GUITAR_STRUM_UP    (1<<5)
#define CELL_PAD_PCLASS_PROFILE_GUITAR_STRUM_DOWN  (1<<6)
#define CELL_PAD_PCLASS_PROFILE_GUITAR_WHAMMYBAR   (1<<7)
#define CELL_PAD_PCLASS_PROFILE_GUITAR_FRET_H1     (1<<8) /* ROCKBAND Stratocaster */
#define CELL_PAD_PCLASS_PROFILE_GUITAR_FRET_H2     (1<<9)
#define CELL_PAD_PCLASS_PROFILE_GUITAR_FRET_H3     (1<<10)
#define CELL_PAD_PCLASS_PROFILE_GUITAR_FRET_H4     (1<<11)
#define CELL_PAD_PCLASS_PROFILE_GUITAR_FRET_H5     (1<<12)
#define CELL_PAD_PCLASS_PROFILE_GUITAR_5WAY_EFFECT (1<<13)
#define CELL_PAD_PCLASS_PROFILE_GUITAR_TILT_SENS   (1<<14)

/*E CellPadPeriphData : Periph device class button data - Guitar */
typedef enum {
    /* Basic */
    CELL_PAD_PCLASS_BTN_OFFSET_GUITAR_FRET_1 = 24,
    CELL_PAD_PCLASS_BTN_OFFSET_GUITAR_FRET_2,
    CELL_PAD_PCLASS_BTN_OFFSET_GUITAR_FRET_3,
    CELL_PAD_PCLASS_BTN_OFFSET_GUITAR_FRET_4,
    CELL_PAD_PCLASS_BTN_OFFSET_GUITAR_FRET_5,
    CELL_PAD_PCLASS_BTN_OFFSET_GUITAR_STRUM_UP,
    CELL_PAD_PCLASS_BTN_OFFSET_GUITAR_STRUM_DOWN,
    CELL_PAD_PCLASS_BTN_OFFSET_GUITAR_WHAMMYBAR, /* 128-255 */
    /* Optional */
    CELL_PAD_PCLASS_BTN_OFFSET_GUITAR_FRET_H1,     /* ROCKBAND Stratocaster */
    CELL_PAD_PCLASS_BTN_OFFSET_GUITAR_FRET_H2,
    CELL_PAD_PCLASS_BTN_OFFSET_GUITAR_FRET_H3,
    CELL_PAD_PCLASS_BTN_OFFSET_GUITAR_FRET_H4,
    CELL_PAD_PCLASS_BTN_OFFSET_GUITAR_FRET_H5 = 36,
    CELL_PAD_PCLASS_BTN_OFFSET_GUITAR_5WAY_EFFECT = 37, 
    CELL_PAD_PCLASS_BTN_OFFSET_GUITAR_TILT_SENS = 38,
} CellPadPeriphGuitarBtnDataOffset;

/*E CellPadPeriphInfo: Periph device class profile - Drums */
#define CELL_PAD_PCLASS_PROFILE_DRUM_SNARE         (1<<0)
#define CELL_PAD_PCLASS_PROFILE_DRUM_TOM           (1<<1)
#define CELL_PAD_PCLASS_PROFILE_DRUM_TOM2          (1<<2)
#define CELL_PAD_PCLASS_PROFILE_DRUM_TOM_FLOOR     (1<<3)
#define CELL_PAD_PCLASS_PROFILE_DRUM_KICK          (1<<4)
#define CELL_PAD_PCLASS_PROFILE_DRUM_CYM_HiHAT     (1<<5)
#define CELL_PAD_PCLASS_PROFILE_DRUM_CYM_CRASH     (1<<6)
#define CELL_PAD_PCLASS_PROFILE_DRUM_CYM_RIDE      (1<<7)
#define CELL_PAD_PCLASS_PROFILE_DRUM_KICK2         (1<<8)

/*E CellPadPeriphData : Periph device class button data - Drums */
typedef enum {
    CELL_PAD_PCLASS_BTN_OFFSET_DRUM_SNARE = 24,
    CELL_PAD_PCLASS_BTN_OFFSET_DRUM_TOM,
    CELL_PAD_PCLASS_BTN_OFFSET_DRUM_TOM2,
    CELL_PAD_PCLASS_BTN_OFFSET_DRUM_TOM_FLOOR,
    CELL_PAD_PCLASS_BTN_OFFSET_DRUM_KICK,
    CELL_PAD_PCLASS_BTN_OFFSET_DRUM_CYM_HiHAT,
    CELL_PAD_PCLASS_BTN_OFFSET_DRUM_CYM_CRASH,
    CELL_PAD_PCLASS_BTN_OFFSET_DRUM_CYM_RIDE,
    CELL_PAD_PCLASS_BTN_OFFSET_DRUM_KICK2,
} CellPadPeriphDrumBtnDataOffset;

/*E CellPadPeriphInfo: Periph device class profile - DJ */
#define CELL_PAD_PCLASS_PROFILE_DJ_MIXER_ATTACK     (1<<0)
#define CELL_PAD_PCLASS_PROFILE_DJ_MIXER_CROSSFADER (1<<1)
#define CELL_PAD_PCLASS_PROFILE_DJ_MIXER_DSP_DIAL   (1<<2)
#define CELL_PAD_PCLASS_PROFILE_DJ_DECK1_STREAM1    (1<<3)
#define CELL_PAD_PCLASS_PROFILE_DJ_DECK1_STREAM2    (1<<4)
#define CELL_PAD_PCLASS_PROFILE_DJ_DECK1_STREAM3    (1<<5)
#define CELL_PAD_PCLASS_PROFILE_DJ_DECK1_PLATTER    (1<<6)
#define CELL_PAD_PCLASS_PROFILE_DJ_DECK2_STREAM1    (1<<7)
#define CELL_PAD_PCLASS_PROFILE_DJ_DECK2_STREAM2    (1<<8)
#define CELL_PAD_PCLASS_PROFILE_DJ_DECK2_STREAM3    (1<<9)
#define CELL_PAD_PCLASS_PROFILE_DJ_DECK2_PLATTER    (1<<10)

/*E CellPadPeriphData : Periph device class button data - DJ */
typedef enum {
    CELL_PAD_PCLASS_BTN_OFFSET_DJ_MIXER_ATTACK     =  24,
    CELL_PAD_PCLASS_BTN_OFFSET_DJ_MIXER_CROSSFADER =  25,
    CELL_PAD_PCLASS_BTN_OFFSET_DJ_MIXER_DSP_DIAL   =  26,
    CELL_PAD_PCLASS_BTN_OFFSET_DJ_DECK1_STREAM1    =  27,
    CELL_PAD_PCLASS_BTN_OFFSET_DJ_DECK1_STREAM2    =  28,
    CELL_PAD_PCLASS_BTN_OFFSET_DJ_DECK1_STREAM3    =  29,
    CELL_PAD_PCLASS_BTN_OFFSET_DJ_DECK1_PLATTER    =  30,
    CELL_PAD_PCLASS_BTN_OFFSET_DJ_DECK2_STREAM1    =  31,
    CELL_PAD_PCLASS_BTN_OFFSET_DJ_DECK2_STREAM2    =  32,
    CELL_PAD_PCLASS_BTN_OFFSET_DJ_DECK2_STREAM3    =  33,
    CELL_PAD_PCLASS_BTN_OFFSET_DJ_DECK2_PLATTER    =  34,
} CellPadPeriphDJBtnDataOffset;

#endif /*  __CELL_PAD_PAD_CODES_H__ */

