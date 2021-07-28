/*  SCE CONFIDENTIAL                                      */
/*  PlayStation(R)3 Programmer Tool Runtime Library 310.001 */
/*  Copyright (C) 2006 Sony Computer Entertainment Inc.   */
/*  All Rights Reserved.                                  */
#ifndef __CELL_RTC_ERROR_H__
#define __CELL_RTC_ERROR_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/*
	CELL_ERROR_FACILITY_SYSTEM_SERVICE	 0x001
		librtc		: 0x8001_0601 - 0x8001_06ff
 */

#define CELL_RTC_MAKE_ERROR(status) \
	CELL_ERROR_MAKE_ERROR(CELL_ERROR_FACILITY_SYSTEM_SERVICE, (0x0600|status))

/*J 初期化処理が行われていない           */
/* ERR: CELL_RTC_MAKE_ERROR(0x0001) */
#define CELL_RTC_ERROR_NOT_INITIALIZED			0x80010601

/*J ポインタを意味する引数が不正         */
/* ERR: CELL_RTC_MAKE_ERROR(0x0002) */
#define CELL_RTC_ERROR_INVALID_POINTER			0x80010602

/*J 一般的な意味で引数が不正             */
/* ERR: CELL_RTC_MAKE_ERROR(0x0003) */
#define CELL_RTC_ERROR_INVALID_VALUE			0x80010603

/*J 一般的な意味で引数が不正             */
/* ERR: CELL_RTC_MAKE_ERROR(0x0004) */
#define CELL_RTC_ERROR_INVALID_ARG				0x80010604

/*E The feature is not supported     */
/* ERR: CELL_RTC_MAKE_ERROR(0x0005) */
#define CELL_RTC_ERROR_NOT_SUPPORTED			0x80010605

/*E The clock is not available       */
/* ERR: CELL_RTC_MAKE_ERROR(0x0006) */
#define CELL_RTC_ERROR_NO_CLOCK					0x80010606

/*E error occured in parsing (unknown format?) */
/* ERR: CELL_RTC_MAKE_ERROR(0x0007) */
#define CELL_RTC_ERROR_BAD_PARSE				0x80010607

/*E The year value is invalid       */
/* ERR: CELL_RTC_MAKE_ERROR(0x0021) */
#define CELL_RTC_ERROR_INVALID_YEAR				0x80010621

/*E The month value is invalid       */
/* ERR: CELL_RTC_MAKE_ERROR(0x0022) */
#define CELL_RTC_ERROR_INVALID_MONTH			0x80010622

/*E The day value is invalid       */
/* ERR: CELL_RTC_MAKE_ERROR(0x0023) */
#define CELL_RTC_ERROR_INVALID_DAY				0x80010623

/*E The hour value is invalid       */
/* ERR: CELL_RTC_MAKE_ERROR(0x0024) */
#define CELL_RTC_ERROR_INVALID_HOUR				0x80010624

/*E The minute value is invalid       */
/* ERR: CELL_RTC_MAKE_ERROR(0x0025) */
#define CELL_RTC_ERROR_INVALID_MINUTE			0x80010625

/*E The second value is invalid       */
/* ERR: CELL_RTC_MAKE_ERROR(0x0026) */
#define CELL_RTC_ERROR_INVALID_SECOND			0x80010626

/*E The microsecond value is invalid  */
/* ERR: CELL_RTC_MAKE_ERROR(0x0027) */
#define CELL_RTC_ERROR_INVALID_MICROSECOND		0x80010627

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __CELL_RTC_ERROR_H__ */
