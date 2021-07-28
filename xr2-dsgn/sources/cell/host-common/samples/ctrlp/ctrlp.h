/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2008 Sony Computer Entertainment Inc.
* All Rights Reserved.
*/

/*
* 
*
*   ctrlp.h
*
*   Date            Design      Log
*  ----------------------------------------------------
*   2008-08-20      kumagae    first
*
*/
#ifndef _CTRLP_H_
#define _CTRLP_H_

#define CTRLP_PAD_MAX_CODES (24)
#define CTRLP_PAD_MAX_DATA (8)

#ifdef WIN32
#define uint32_t unsigned int
#define uint16_t unsigned short
#define uint8_t   unsigned char
#define int32_t  int
#define int16_t  short int
#define int8_t   signed char
#endif

typedef enum Deci3CtrlpCode{
	DECI3_CTRLP_CODE_INIT			= 0,
	DECI3_CTRLP_CODE_INITR			= 1,
	DECI3_CTRLP_CODE_REC_START		= 2,
	DECI3_CTRLP_CODE_REC_STARTR		= 3,
	DECI3_CTRLP_CODE_REC_STOP		= 4,
	DECI3_CTRLP_CODE_REC_STOPR		= 5,
	DECI3_CTRLP_CODE_REC_DATA		= 6,
	DECI3_CTRLP_CODE_REC_DATAR		= 7,
	DECI3_CTRLP_CODE_PLAY_START		= 8,
	DECI3_CTRLP_CODE_PLAY_STARTR	= 9,
	DECI3_CTRLP_CODE_PLAY_STOP		= 10,
	DECI3_CTRLP_CODE_PLAY_STOPR		= 11,
	DECI3_CTRLP_CODE_PLAY_DATA		= 12,
	DECI3_CTRLP_CODE_PLAY_DATAR		= 13,
}Deci3CtrlpCode;

typedef enum Deci3CtrlpError{
	DECI3_CTRLP_OK		= 0UL,
	DECI3_CTRLP_EINVAL	= 0x80010002UL,
	DECI3_CTRLP_ENOMEM	= 0x80010004UL,
	DECI3_CTRLP_EBUSY	= 0x8001000AUL,
	DECI3_CTRLP_EUNSUP  = 0x80010037UL,
	DECI3_CTRLP_EUNKNOWN= 0x80010038UL
}Deci3CtrlpError;

typedef struct Deci3CtrlpHeader{
	uint16_t code;
	uint16_t size;
}Deci3CtrlpHeader;

typedef struct Deci3CtrlpInitr{
	Deci3CtrlpHeader header;
	uint32_t version;
}Deci3CtrlpInitr;

typedef struct Deci3CtrlpRecStart{
	Deci3CtrlpHeader header;
}Deci3CtrlpRecStart;

typedef struct Deci3CtrlpRecStop{
	Deci3CtrlpHeader header;
}Deci3CtrlpRecStop;

typedef struct Deci3CtrlpPlayStart{
	Deci3CtrlpHeader header;
}Deci3CtrlpPlayStart;

typedef struct Deci3CtrlpPlayStop{
	Deci3CtrlpHeader header;
}Deci3CtrlpPlayStop;

typedef struct Deci3CtrlpRecStartr{
	Deci3CtrlpHeader header;
	uint32_t result;
}Deci3CtrlpRecStartr;

typedef struct Deci3CtrlpRecStopr{
	Deci3CtrlpHeader header;
	uint32_t result;
}Deci3CtrlpRecStopr;

typedef struct Deci3CtrlpPlayStartr{
	Deci3CtrlpHeader Dheader;
	uint32_t result;
}Deci3CtrlpPlayStartr;

typedef struct Deci3CtrlpPlayStopr{
	Deci3CtrlpHeader header;
	uint32_t result;
}Deci3CtrlpPlayStopr;

typedef struct Deci3CtrlpRecDatar{
	Deci3CtrlpHeader header;
}Deci3CtrlpRecDatar;

typedef struct Deci3CtrlpPadData{
        uint32_t pad_time_hi;
        uint32_t pad_time_lo;
        uint32_t reserved0;
        uint32_t reserved1;
        uint8_t port_no;
        uint8_t port_status;
        uint8_t len;
        uint8_t reserved2;
		uint32_t reserved3;
        uint16_t        button[CTRLP_PAD_MAX_CODES];
}Deci3CtrlpPadData;

typedef struct Deci3CtrlpPlayData{
	Deci3CtrlpHeader header;
	Deci3CtrlpPadData data[CTRLP_PAD_MAX_DATA];
}Deci3CtrlpPlayData;

typedef struct Deci3CtrlpPlayDatar{
	Deci3CtrlpHeader header;
	uint32_t result;
}Deci3CtrlpPlayDatar;

#ifdef	__cplusplus
extern "C" {
#endif	/* __cplusplus */


#ifdef	__cplusplus
}
#endif	/* __cplusplus */

#endif /* _CTRLP_H_ */
