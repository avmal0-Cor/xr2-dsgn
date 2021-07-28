/*                 -*- mode: c; tab-width: 4; indent-tabs-mode: t; -*- */
/*  SCE CONFIDENTIAL                                      */
/*  PlayStation(R)3 Programmer Tool Runtime Library 310.001 */
/*  Copyright (C) 2006 Sony Computer Entertainment Inc.   */
/*  All Rights Reserved.                                  */
#ifndef __CELL_RTC_RTCSVC_H__
#define __CELL_RTC_RTCSVC_H__

#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
extern "C" {
#endif	/* defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus) */

/*J フィールドごとに分解した時刻表現 */
/*E representation of time using individual members */
typedef struct CellRtcDateTime {
	unsigned short year;
	unsigned short month;
	unsigned short day;
	unsigned short hour;
	unsigned short minute;
	unsigned short second;
	unsigned int   microsecond;
} CellRtcDateTime;

/*J UTC 0001/01/01 00:00:00からの累積時間(1usec刻み) */
/*E Cumulative numbver of Ticks from 0001/01/01 00:00:00 */
typedef struct CellRtcTick {
	uint64_t tick;
} CellRtcTick;


/*J プロトタイプ宣言 */
/*E prototype declaration */

/*J 現在時刻を取得する関数 */
/*E Get current time(UTC) in Tick representation */
#define cellRtcGetCurrentTickUtc(_tick)		cellRtcGetCurrentTick((_tick))
int cellRtcGetCurrentTick(CellRtcTick *pTick);

/*J 指定されたタイムゾーンで現在時刻を取得 */
/*E Get current time in specified time zone */
#define cellRtcGetCurrentClockUtc(_p)		cellRtcGetCurrentClock((_p),(0))
int cellRtcGetCurrentClock(CellRtcDateTime *pTime, int iTimeZone);

/*J 地方時で取得 サマータイム設定の影響あり */
/*E Get current time (local time) */
int cellRtcGetCurrentClockLocalTime(CellRtcDateTime *pTime);

/*J 指定されたUTC時刻を地方時に変換 */
/*E Convert Tick-representation UTC time into local time */
int cellRtcConvertUtcToLocalTime(const CellRtcTick *pUtc, CellRtcTick *pLocalTime);

/*J 指定されたローカル時刻をUTC時刻に変換 */
/*E Convert Tick-representation local time into UTC time */
int cellRtcConvertLocalTimeToUtc(const CellRtcTick *pLocalTime, CellRtcTick *pUtc);

/*J うるう年かどうか判別 */
/*E Check leap year */
int cellRtcIsLeapYear(int year);

/*J 指定された月の日数を取得 */
/*E Get number of days in specified month */
int cellRtcGetDaysInMonth(int year, int month);

/*J 指定された日が何曜日が返す */
/*E Calculate day of the week */
#define CELL_RTC_DAYOFWEEK_SUNDAY		0
#define CELL_RTC_DAYOFWEEK_MONDAY		1
#define CELL_RTC_DAYOFWEEK_TUESDAY		2
#define CELL_RTC_DAYOFWEEK_WEDNESDAY		3
#define CELL_RTC_DAYOFWEEK_THURSDAY		4
#define CELL_RTC_DAYOFWEEK_FRIDAY		5
#define CELL_RTC_DAYOFWEEK_SATURDAY		6
int cellRtcGetDayOfWeek(int year, int month, int day);

/*J 各フィールドの範囲が正しいかどうかチェック */
/*E Check range of each field */
int cellRtcCheckValid(const CellRtcDateTime *pTime);


/*J 各種ライブラリとの互換形式 */
/*E Format Conversion Functions */
#if !defined(CELL_RTC_DONT_INCLUDE_SYSTIME)

/*J POSIX time_t形式の時間を設定 */
/*E Get time information in POSIX time_t format */
#define cellRtcConvertTime_tToDateTime(_timet, _pdatetime)	cellRtcSetTime_t((_pdatetime), (_timet))
int cellRtcSetTime_t(CellRtcDateTime *pTime, time_t llTime);

/*J POSIX time_t形式の時間を取得 */
/*E Set time information in POSIX time_t format */
#define cellRtcConvertDateTimeToTime_t(_pdatetime, _ptimet)	cellRtcGetTime_t((_pdatetime), (_ptimet))
int cellRtcGetTime_t(const CellRtcDateTime *pTime, time_t *pllTime);

#endif	/* !defined(CELL_RTC_DONT_INCLUDE_SYSTIME) */


/*J MS-DOS形式の時間を設定 */
/*E Set time information in MS-DOS format */
#define cellRtcConvertDosTimeToDateTime(_dostime, _pdatetime)	cellRtcSetDosTime((_pdatetime), (_dostime))
int cellRtcSetDosTime(CellRtcDateTime *pTime, unsigned int uiDosTime);

/*J MS-DOS形式の時間を取得 */
/*E Get time information in MS-DOS format */
#define cellRtcConvertDateTimeToDosTime(_pdatetime, _pdostime)	cellRtcGetDosTime((_pdatetime), (_pdostime))
int cellRtcGetDosTime(const CellRtcDateTime *pTime, unsigned int *puiDosTime);

/*J Win32 FILETIME形式の時間を設定 */
/*E Set time information in Win32 FILETIME format */
#define cellRtcConvertWin32TimeToDateTime(_pw32time, _pdatetime)	cellRtcSetWin32FileTime((_pdatetime), (_pw32time))
int cellRtcSetWin32FileTime(CellRtcDateTime *pTime, uint64_t ulWin32Time);

/*J Win32 FILETIME形式の時間を取得 */
/*E Get time information in Win32 FILETIME format */
#define cellRtcConvertDateTimeToWin32Time(_pdatetime, _pw32time)	cellRtcGetWin32FileTime((_pdatetime), (_pw32time))
int cellRtcGetWin32FileTime(const CellRtcDateTime *pTime, uint64_t *ulWin32Time);

/*J CellRtcDateTime構造体とCellRtcTick構造体の変換 */
/*E conversion between CellRtcDateTime and CellRtcTick */

/*J CellRtcTick形式の時間を設定 */
/*E Set time information according to Tick format */
#define cellRtcConvertTickToDateTime(_ptick, _pdatetime)			cellRtcSetTick((_pdatetime), (_ptick))
int cellRtcSetTick(CellRtcDateTime *pTime, const CellRtcTick *pTick);

/*J CellRtcTick形式の時間を取得 */
/*E Get time information in Tick format */
#define cellRtcConvertDateTimeToTick(_pdatetime, _ptick)			cellRtcGetTick((_pdatetime), (_ptick))
int cellRtcGetTick(const CellRtcDateTime *pTime, CellRtcTick *pTick);


int cellRtcCompareTick(const CellRtcTick *pTick1, const CellRtcTick *pTick2);



/*J 時間の加減算 */
/*E Time Manipurator Functions */

/*E Add specified number of Ticks to time having Tick representation */
int cellRtcTickAddTicks(CellRtcTick *pTick0, const CellRtcTick *pTick1, int64_t lAdd);

/*E Add specified number of microseconds to time having Tick representation */
int cellRtcTickAddMicroseconds(CellRtcTick *pTick0, const CellRtcTick *pTick1, int64_t lAdd);

/*E Add specified number of seconds to time having Tick representation */
int cellRtcTickAddSeconds(CellRtcTick *pTick0, const CellRtcTick *pTick1, int64_t lAdd);

/*E Add specified number of minutes to time having Tick representation */
int cellRtcTickAddMinutes(CellRtcTick *pTick0, const CellRtcTick *pTick1, int64_t lAdd);

/*E Add specified number of houres to time having Tick representation */
int cellRtcTickAddHours(CellRtcTick *pTick0, const CellRtcTick *pTick1, int lAdd);

/*E Add specified number of days to time having Tick representation */
int cellRtcTickAddDays(CellRtcTick *pTick0, const CellRtcTick *pTick1, int lAdd);

/*E Add specified number of weeks to time having Tick representation */
int cellRtcTickAddWeeks(CellRtcTick *pTick0, const CellRtcTick *pTick1, int lAdd);

/*E Add specified number of months to time having Tick representation */
int cellRtcTickAddMonths(CellRtcTick *pTick0, const CellRtcTick *pTick1, int lAdd);

/*E Add specified number of years to time having Tick representation */
int cellRtcTickAddYears(CellRtcTick *pTick0, const CellRtcTick *pTick1, int lAdd);



/*J 文字列へのフォーマット関数 */
/*E Formatting Functions */

/*J 指定されたタイムゾーンでRFC2822形式にフォーマット */
/*E Format Tick-representation UTC time in RFC2822 format */
int cellRtcFormatRfc2822(char *pszDateTime, const CellRtcTick *pUtc, int iTimeZoneMinutes);

/*J RFC2822形式にフォーマット */
/*E Format Tick-representation UTC time in RFC2822 format */
int cellRtcFormatRfc2822LocalTime(char *pszDateTime, const CellRtcTick *pUtc);

/*J 指定されたタイムゾーンでRFC3339形式にフォーマット */
/*E Format Tick-representation UTC time in RFC3339(ISO8601) format */
int cellRtcFormatRfc3339(char *pszDateTime, const CellRtcTick *pUtc, int iTimeZoneMinutes);

/*J RFC3339形式にフォーマット */
/*E Format Tick-representation UTC time in RFC3339(ISO8601) format */
int cellRtcFormatRfc3339LocalTime(char *pszDateTime, const CellRtcTick *pUtc);

/*J 文字列の時刻をパース */
/*E Parse time information represented as a string */
int cellRtcParseDateTime(CellRtcTick *pUtc, const char *pszDateTime);

/*J RFC3339形式の文字列を時刻としてパース */
/*E Parse time information represented in RFC3339 format */
int cellRtcParseRfc3339(CellRtcTick *pUtc, const char *pszDateTime);



/*J CellRtcDateTime構造体の操作 */
/*E Time Information Manipulation Functions */
static __inline__ int cellRtcSetYear(CellRtcDateTime *pTime, int year);
static __inline__ int cellRtcSetMonth(CellRtcDateTime *pTime, int month);
static __inline__ int cellRtcSetDay(CellRtcDateTime *pTime, int day);
static __inline__ int cellRtcSetHour(CellRtcDateTime *pTime, int hour);
static __inline__ int cellRtcSetMinute(CellRtcDateTime *pTime, int minute);
static __inline__ int cellRtcSetSecond(CellRtcDateTime *pTime, int second);
static __inline__ int cellRtcSetMicrosecond(CellRtcDateTime *pTime, int microsecond);

static __inline__ int cellRtcGetYear(const CellRtcDateTime *pTime);
static __inline__ int cellRtcGetMonth(const CellRtcDateTime *pTime);
static __inline__ int cellRtcGetDay(const CellRtcDateTime *pTime);
static __inline__ int cellRtcGetHour(const CellRtcDateTime *pTime);
static __inline__ int cellRtcGetMinute(const CellRtcDateTime *pTime);
static __inline__ int cellRtcGetSecond(const CellRtcDateTime *pTime);
static __inline__ int cellRtcGetMicrosecond(const CellRtcDateTime *pTime);

/*J 年フィールドの設定(1〜9999) */
/*E Set year field(1-9999) */
static __inline__ int cellRtcSetYear(CellRtcDateTime *pTime, int year)
{
	if (year<1 || year>9999) {
		return (CELL_RTC_ERROR_INVALID_ARG);
	}
#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
	pTime->year = static_cast<unsigned short>(year);
#else	/* defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus) */
	pTime->year = (unsigned short)year;
#endif	/* defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus) */
	return (0);
}

/*J 月フィールドの設定(1〜12) */
/*E Set month field(1-12) */
static __inline__ int cellRtcSetMonth(CellRtcDateTime *pTime, int month)
{
	if (month<1 || month>12) {
		return (CELL_RTC_ERROR_INVALID_ARG);
	}
#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
	pTime->month = static_cast<unsigned short>(month);
#else	/* defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus) */
	pTime->month = (unsigned short)month;
#endif	/* defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus) */
	return (0);
}

/*J 日フィールドの設定(1〜31) */
/*E Set day field(1-31) */
static __inline__ int cellRtcSetDay(CellRtcDateTime *pTime, int day)
{
	if (day<1 || day>31) {
		return (CELL_RTC_ERROR_INVALID_ARG);
	}
#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
	pTime->day = static_cast<unsigned short>(day);
#else	/* defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus) */
	pTime->day = (unsigned short)day;
#endif	/* defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus) */
	return (0);
}

/*J 時フィールドの設定(0〜23) */
/*E Set hour field(0-23) */
static __inline__ int cellRtcSetHour(CellRtcDateTime *pTime, int hour)
{
	if (hour<0 || hour>23) {
		return (CELL_RTC_ERROR_INVALID_ARG);
	}
#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
	pTime->hour = static_cast<unsigned short>(hour);
#else	/* defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus) */
	pTime->hour = (unsigned short)hour;
#endif	/* defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus) */
	return (0);
}

/*J 分フィールドの設定(0〜59) */
/*E Set minute field(0-59) */
static __inline__ int cellRtcSetMinute(CellRtcDateTime *pTime, int minute)
{
	if (minute<0 || minute>59) {
		return (CELL_RTC_ERROR_INVALID_ARG);
	}
#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
	pTime->minute = static_cast<unsigned short>(minute);
#else	/* defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus) */
	pTime->minute = (unsigned short)minute;
#endif	/* defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus) */
	return (0);
}

/*J 秒フィールドの設定(0〜59) */
/*E Set second field(0-59) */
static __inline__ int cellRtcSetSecond(CellRtcDateTime *pTime, int second)
{
	if (second<0 || second>59) {
		return (CELL_RTC_ERROR_INVALID_ARG);
	}
#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
	pTime->second = static_cast<unsigned short>(second);
#else	/* defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus) */
	pTime->second = (unsigned short)second;
#endif	/* defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus) */
	return (0);
}

/*J μ秒フィールドの設定(0〜999999) */
/*E Set microsecond field(0-999999) */
static __inline__ int cellRtcSetMicrosecond(CellRtcDateTime *pTime, int microsecond)
{
	if (microsecond<0 || microsecond>999999) {
		return (CELL_RTC_ERROR_INVALID_ARG);
	}
#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
	pTime->microsecond = static_cast<unsigned int>(microsecond);
#else	/* defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus) */
	pTime->microsecond = (unsigned int)microsecond;
#endif	/* defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus) */
	return (0);
}

/*J 年フィールドの取得(0〜9999) */
/*E Get year field(0-9999) */
static __inline__ int cellRtcGetYear(const CellRtcDateTime *pTime)
{
	return (pTime->year);
}

/*J 月フィールドの取得(1〜12) */
/*E Get month field(1-12) */
static __inline__ int cellRtcGetMonth(const CellRtcDateTime *pTime)
{
	return (pTime->month);
}

/*J 日フィールドの取得(1〜31) */
/*E Get day field(1-31) */
static __inline__ int cellRtcGetDay(const CellRtcDateTime *pTime)
{
	return (pTime->day);
}

/*J 時フィールドの取得(0〜23) */
/*E Get hour field(0-23) */
static __inline__ int cellRtcGetHour(const CellRtcDateTime *pTime)
{
	return (pTime->hour);
}

/*J 分フィールドの取得(0〜59) */
/*E Get minute field(0-59) */
static __inline__ int cellRtcGetMinute(const CellRtcDateTime *pTime)
{
	return (pTime->minute);
}

/*J 秒フィールドの取得(0〜59) */
/*E Get second field(0-59) */
static __inline__ int cellRtcGetSecond(const CellRtcDateTime *pTime)
{
	return (pTime->second);
}

/*J μ秒フィールドの取得(0〜999999) */
/*E Get microsecond field(0-999999) */
static __inline__ int cellRtcGetMicrosecond(const CellRtcDateTime *pTime)
{
	return (pTime->microsecond);
}


#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
}
#endif	/* defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus) */

#endif	/* __CELL_RTC_RTCSVC_H__ */

/* Local variables: */
/* tab-width: 4 */
/* End: */
/* vi:set tabstop=4: */
