/*   SCE CONFIDENTIAL                                       */
/*   PlayStation(R)3 Programmer Tool Runtime Library 310.001 */
/*   Copyright (C) 2006 Sony Computer Entertainment Inc.    */
/*   All Rights Reserved.                                   */
#ifndef __CELL_KEYBOARD_KB_CODES_H__
#define __CELL_KEYBOARD_KB_CODES_H__

#include <sys/types.h>

#define CELL_KB_MAX_KEYBOARDS  127

/* keycode data types */
#define CELL_KB_RAWDAT	    0x8000U  /* raw keyboard data */
#define CELL_KB_KEYPAD	    0x4000U  /* keypad code data */

/* meta keys */
#define CELL_KB_MKEY_L_CTRL	    (1U<<0)  /* 0:Release 1:Push */
#define CELL_KB_MKEY_L_SHIFT	(1U<<1)  /* 0:Release 1:Push */
#define CELL_KB_MKEY_L_ALT	    (1U<<2)  /* 0:Release 1:Push */
#define CELL_KB_MKEY_L_WIN	    (1U<<3)  /* 0:Release 1:Push */
#define CELL_KB_MKEY_R_CTRL	    (1U<<4)  /* 0:Release 1:Push */
#define CELL_KB_MKEY_R_SHIFT	(1U<<5)  /* 0:Release 1:Push */
#define CELL_KB_MKEY_R_ALT	    (1U<<6)  /* 0:Release 1:Push */
#define CELL_KB_MKEY_R_WIN	    (1U<<7)  /* 0:Release 1:Push */
/*
 * on Macintosh keyboards, the OPTION and APPLE keys are the
 * same as the ALT and WIN keys.
 */

/* led codes */
#define CELL_KB_LED_NUM_LOCK	(1U<<0)  /* 0:OFF 1:ON */
#define CELL_KB_LED_CAPS_LOCK	(1U<<1)  /* 0:OFF 1:ON */
#define CELL_KB_LED_SCROLL_LOCK (1U<<2)  /* 0:OFF 1:ON */
#define CELL_KB_LED_COMPOSE	    (1U<<3)  /* 0:OFF 1:ON */
#define CELL_KB_LED_KANA	    (1U<<4)  /* 0:OFF 1:ON */

/* ******* rawdata that can't be converted to ASCII Codes  ********/
/*
 * 1  when the CELL_KB_RAWDAT bit is set, then the code
 *    will be one of the following
 * 2  Codes other those listed below may also be returned
 */
#define CELL_KEYC_NO_EVENT	    0x00
#define CELL_KEYC_E_ROLLOVER	0x01
#define CELL_KEYC_E_POSTFAIL	0x02
#define CELL_KEYC_E_UNDEF	    0x03
#define CELL_KEYC_ESCAPE	    0x29
#define CELL_KEYC_106_KANJI	    0x35  /* the half-width/full width Kanji key */
#define CELL_KEYC_CAPS_LOCK	    0x39
#define CELL_KEYC_F1		    0x3a
#define CELL_KEYC_F2		    0x3b
#define CELL_KEYC_F3		    0x3c
#define CELL_KEYC_F4		    0x3d
#define CELL_KEYC_F5		    0x3e
#define CELL_KEYC_F6		    0x3f
#define CELL_KEYC_F7		    0x40
#define CELL_KEYC_F8		    0x41
#define CELL_KEYC_F9		    0x42
#define CELL_KEYC_F10		    0x43
#define CELL_KEYC_F11		    0x44
#define CELL_KEYC_F12		    0x45
#define CELL_KEYC_PRINTSCREEN	0x46
#define CELL_KEYC_SCROLL_LOCK	0x47
#define CELL_KEYC_PAUSE	        0x48
#define CELL_KEYC_INSERT	    0x49
#define CELL_KEYC_HOME	        0x4a
#define CELL_KEYC_PAGE_UP	    0x4b
#define CELL_KEYC_DELETE	    0x4c
#define CELL_KEYC_END		    0x4d
#define CELL_KEYC_PAGE_DOWN	    0x4e
#define CELL_KEYC_RIGHT_ARROW	0x4f
#define CELL_KEYC_LEFT_ARROW	0x50
#define CELL_KEYC_DOWN_ARROW	0x51
#define CELL_KEYC_UP_ARROW	    0x52
#define CELL_KEYC_NUM_LOCK	    0x53
#define CELL_KEYC_APPLICATION	0x65  /* Application Key */
#define CELL_KEYC_KANA	        0x88  /* Katakana/Hiragana/Romaji key */
#define CELL_KEYC_HENKAN	    0x8a  /* Conversion key */
#define CELL_KEYC_MUHENKAN	    0x8b  /* no Conversion key */


/* ******* Raw keycodes for ASCII convertible keys *******/
/*
 * 1  These are the raw codes for keys that usually
 *    used for the ASCII code displayed.
 * 2  Codes other than the following may also be returned.
 */
#define CELL_KEYC_A			    0x04
#define CELL_KEYC_B			    0x05
#define CELL_KEYC_C			    0x06
#define CELL_KEYC_D			    0x07
#define CELL_KEYC_E			    0x08
#define CELL_KEYC_F			    0x09
#define CELL_KEYC_G			    0x0A
#define CELL_KEYC_H			    0x0B
#define CELL_KEYC_I			    0x0C
#define CELL_KEYC_J			    0x0D
#define CELL_KEYC_K			    0x0E
#define CELL_KEYC_L			    0x0F
#define CELL_KEYC_M			    0x10
#define CELL_KEYC_N			    0x11
#define CELL_KEYC_O			    0x12
#define CELL_KEYC_P			    0x13
#define CELL_KEYC_Q			    0x14
#define CELL_KEYC_R			    0x15
#define CELL_KEYC_S			    0x16
#define CELL_KEYC_T			    0x17
#define CELL_KEYC_U			    0x18
#define CELL_KEYC_V			    0x19
#define CELL_KEYC_W			    0x1A
#define CELL_KEYC_X			    0x1B
#define CELL_KEYC_Y			    0x1C
#define CELL_KEYC_Z			    0x1D
#define CELL_KEYC_1			    0x1E
#define CELL_KEYC_2			    0x1F
#define CELL_KEYC_3			    0x20
#define CELL_KEYC_4			    0x21
#define CELL_KEYC_5			    0x22
#define CELL_KEYC_6			    0x23
#define CELL_KEYC_7			    0x24
#define CELL_KEYC_8			    0x25
#define CELL_KEYC_9			    0x26
#define CELL_KEYC_0			    0x27
#define CELL_KEYC_ENTER		    0x28
#define CELL_KEYC_ESC			0x29
#define CELL_KEYC_BS			0x2A
#define CELL_KEYC_TAB			0x2B
#define CELL_KEYC_SPACE		    0x2C
#define CELL_KEYC_MINUS		    0x2D
#define CELL_KEYC_EQUAL_101		0x2E  /* = and + */
#define CELL_KEYC_ACCENT_CIRCONFLEX_106 0x2E  /* ^ and ~ */
#define CELL_KEYC_LEFT_BRACKET_101	    0x2F  /* [ */
#define CELL_KEYC_ATMARK_106		    0x2F  /* @ */
#define CELL_KEYC_RIGHT_BRACKET_101	    0x30  /* ] */
#define CELL_KEYC_LEFT_BRACKET_106	    0x30  /* [ */
#define CELL_KEYC_BACKSLASH_101	        0x31  /* \ and | */
#define CELL_KEYC_RIGHT_BRACKET_106	    0x32  /* ] */
#define CELL_KEYC_SEMICOLON		    0x33  /* ; */
#define CELL_KEYC_QUOTATION_101	    0x34  /* ' and " */
#define CELL_KEYC_COLON_106		    0x34  /* : and * */
#define CELL_KEYC_COMMA		        0x36
#define CELL_KEYC_PERIOD		    0x37
#define CELL_KEYC_SLASH		        0x38
#define CELL_KEYC_CAPS_LOCK		    0x39
#define CELL_KEYC_KPAD_NUMLOCK	    0x53
#define CELL_KEYC_KPAD_SLASH		0x54
#define CELL_KEYC_KPAD_ASTERISK	    0x55
#define CELL_KEYC_KPAD_MINUS		0x56
#define CELL_KEYC_KPAD_PLUS		    0x57
#define CELL_KEYC_KPAD_ENTER		0x58
#define CELL_KEYC_KPAD_1		    0x59
#define CELL_KEYC_KPAD_2		    0x5A
#define CELL_KEYC_KPAD_3		    0x5B
#define CELL_KEYC_KPAD_4		    0x5C
#define CELL_KEYC_KPAD_5		    0x5D
#define CELL_KEYC_KPAD_6		    0x5E
#define CELL_KEYC_KPAD_7		    0x5F
#define CELL_KEYC_KPAD_8		    0x60
#define CELL_KEYC_KPAD_9		    0x61
#define CELL_KEYC_KPAD_0		    0x62
#define CELL_KEYC_KPAD_PERIOD		0x63
#define CELL_KEYC_BACKSLASH_106	    0x87
#define CELL_KEYC_YEN_106		    0x89

typedef enum {
  CELL_KB_LED_MODE_MANUAL = 0,
  CELL_KB_LED_MODE_AUTO1  = 1,
  CELL_KB_LED_MODE_AUTO2  = 2,
} CellKbLedMode;

typedef enum {
  CELL_KB_CODETYPE_RAW   = 0,
  CELL_KB_CODETYPE_ASCII = 1,
} CellKbCodeType;

typedef enum {
  CELL_KB_MAPPING_101           = 0,
  CELL_KB_MAPPING_106           = 1,
  CELL_KB_MAPPING_106_KANA      = 2,
  CELL_KB_MAPPING_GERMAN_GERMANY,
  CELL_KB_MAPPING_SPANISH_SPAIN,
  CELL_KB_MAPPING_FRENCH_FRANCE,
  CELL_KB_MAPPING_ITALIAN_ITALY,
  CELL_KB_MAPPING_DUTCH_NETHERLANDS,
  CELL_KB_MAPPING_PORTUGUESE_PORTUGAL,
  CELL_KB_MAPPING_RUSSIAN_RUSSIA,
  CELL_KB_MAPPING_ENGLISH_UK,
  CELL_KB_MAPPING_KOREAN_KOREA,
  CELL_KB_MAPPING_NORWEGIAN_NORWAY,
  CELL_KB_MAPPING_FINNISH_FINLAND,
  CELL_KB_MAPPING_DANISH_DENMARK,
  CELL_KB_MAPPING_SWEDISH_SWEDEN,
  CELL_KB_MAPPING_CHINESE_TRADITIONAL,
  CELL_KB_MAPPING_CHINESE_SIMPLIFIED,
  CELL_KB_MAPPING_SWISS_FRENCH_SWITZERLAND,
  CELL_KB_MAPPING_SWISS_GERMAN_SWITZERLAND,
  CELL_KB_MAPPING_CANADIAN_FRENCH_CANADA,
  CELL_KB_MAPPING_BELGIAN_BELGIUM,
  CELL_KB_MAPPING_POLISH_POLAND
} CellKbMappingType;

#define CELL_KB_ARRANGEMENT_101 CELL_KB_MAPPING_101
#define CELL_KB_ARRANGEMENT_106 CELL_KB_MAPPING_106
#define CELL_KB_ARRANGEMENT_106_KANA CELL_KB_MAPPING_106_KANA

typedef enum {
  CELL_KB_RMODE_INPUTCHAR = 0,
  CELL_KB_RMODE_PACKET    = 1,
} CellKbReadMode;

/*E CellKbInfo : common information of keyboard. */
#define CELL_KB_INFO_INTERCEPTED      (1 << 0) /*E input intercepted */

/*E CellKbInfo : connected status.  */
#define CELL_KB_STATUS_CONNECTED       (1) /*E keybaord is connected.*/
#define CELL_KB_STATUS_DISCONNECTED    (0) /*E keyboard is disconnected */
typedef struct CellKbInfo{
  uint32_t max_connect;
  uint32_t now_connect;
  uint32_t info;
  uint8_t status[CELL_KB_MAX_KEYBOARDS];
}CellKbInfo;

#define CELL_KB_MAX_KEYCODES 62

typedef struct CellKbData{
  uint32_t   led;
  uint32_t   mkey;
  int32_t     len;
  uint16_t keycode[CELL_KB_MAX_KEYCODES];
}CellKbData;

typedef struct CellKbConfig{
	uint32_t arrange;
	uint32_t read_mode;
	uint32_t code_type;
}CellKbConfig;

#endif /* __CELL_KEYBOARD_KB_CODES_H__ */

