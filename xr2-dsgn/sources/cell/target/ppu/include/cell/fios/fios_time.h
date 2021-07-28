/*
	SCE CONFIDENTIAL
	PlayStation(R)3 Programmer Tool Runtime Library 310.001
	Copyright (C) 2007 Sony Computer Entertainment Inc.
	All Rights Reserved.
*/
/**
	\file fios_time.h

	Time functions and definitions.
*/

#ifndef __CELL_FIOS_TIME_H__
#define __CELL_FIOS_TIME_H__

#include <cell/fios/fios_types.h>
#include <cell/fios/fios_platform.h>

namespace cell {
namespace fios {

#define kDEADLINE_NOW      ((cell::fios::abstime_t)1)                      /**< The shortest possible deadline. Serviced as soon as possible. */
#define kDEADLINE_ASAP     ((cell::fios::abstime_t)2)                      /**< The second-shortest possible deadline. */
#define kDEADLINE_LATER    ((cell::fios::abstime_t)0x7FFFFFFFFFFFFFFFLL)   /**< An infinite deadline. Serviced whenever possible but without a strict deadline. */

#define kTIME_NULL         ((cell::fios::abstime_t)0)                      /**< The null time, which can be used as an invalid marker. */
#define kTIME_EARLIEST     ((cell::fios::abstime_t)1)                      /**< The earliest possible time. */
#define kTIME_LATEST       ((cell::fios::abstime_t)0x7FFFFFFFFFFFFFFFLL)   /**< The latest possible time. */

#define kDATE_NULL         ((cell::fios::datetime_t)0)                     /**< The null date, which can be used as an invalid marker. */
#define kDATE_EARLIEST     ((cell::fios::datetime_t)1)                     /**< The earliest possible date. */
#define kDATE_LATEST       ((cell::fios::datetime_t)0x7FFFFFFFFFFFFFFFULL) /**< The latest possible date. */
#define kDATE_HIGHBIT      ((cell::fios::datetime_t)0x8000000000000000ULL) /**< The high bit of the date, which is reserved for future expansion. */

/**
	\brief   Returns the current time.
	
	This function is typically used to generate a deadline. For example, a deadline of
	"800 milliseconds from now" may be generated as FIOSGetCurrentTime() +
	FIOSMillisecondsToAbstime(800).
	
	Rather than calling this directly, you may prefer to use macros such as
	FIOSRelativeSeconds(), FIOSRelativeMilliseconds(), FIOSRelativeMicroseconds(),
	or FIOSRelativeNanoseconds() which indirectly use this function.
	
	\return  The current time.
*/
inline abstime_t	FIOSGetCurrentTime() { return platform::currentTime(); }

/**
	\brief   Converts an #abstime_t to nanoseconds.
	\param[in]   abstime     The input abstime value.
	\return                  The corresponding value in nanoseconds, rounded up.
*/
inline int64_t FIOSAbstimeToNanoseconds(abstime_t abstime) { return platform::timeToNanoseconds(abstime); }

/**
	\brief   Converts a nanoseconds value to an #abstime_t.
	\param[in]   nanos       The input value in nanoseconds.
	\return                  The corresponding value as an #abstime_t.
*/
inline abstime_t	FIOSNanosecondsToAbstime(uint64_t nanos) { return platform::nanosecondsToTime(nanos); }

/**
	\brief   Converts a relative nanoseconds value to an #abstime_t.
	\param[in]   nanos      The input value in nanoseconds.
	\return                 An #abstime_t representing that many nanoseconds from now.
*/
inline abstime_t FIOSRelativeNanoseconds(uint64_t nanos) { return FIOSGetCurrentTime() + FIOSNanosecondsToAbstime(nanos); }

/**
	\brief   Converts an #abstime_t to microseconds.
	\param[in]   abstime    The input abstime value.
	\return                 The corresponding value in microseconds, rounded up.
*/
inline int64_t FIOSAbstimeToMicroseconds(abstime_t abstime) { return ((FIOSAbstimeToNanoseconds(abstime) + 999) / 1000); }

/**
	\brief   Converts a microseconds value to an #abstime_t.
	\param[in]   mics       The input value in microseconds.
	\return                 The corresponding value as an #abstime_t.
*/
inline abstime_t FIOSMicrosecondsToAbstime(uint64_t mics) { return FIOSNanosecondsToAbstime((uint64_t)((mics)*(uint64_t)1000)); }

/**
	\brief   Converts a relative microseconds value to an #abstime_t.
	\param[in]   mics       The input value in microseconds.
	\return                 An #abstime_t representing that many microseconds from now.
*/
inline abstime_t FIOSRelativeMicroseconds(uint64_t mics) { return (FIOSGetCurrentTime() + FIOSMicrosecondsToAbstime(mics)); }

/**
	\brief   Converts an #abstime_t to milliseconds.
	\param[in]   abstime    The input abstime value.
	\return                 The corresponding value in milliseconds, rounded up.
*/
inline int64_t FIOSAbstimeToMilliseconds(abstime_t abstime) { return ((FIOSAbstimeToNanoseconds(abstime) + 999999) / 1000000); }

/**
	\brief   Converts a milliseconds value to an #abstime_t.
	\param[in]   mills      The input value in milliseconds.
	\return                 The corresponding value as an #abstime_t.
*/
inline abstime_t FIOSMillisecondsToAbstime(uint64_t mills) { return FIOSNanosecondsToAbstime((uint64_t)((mills)*(uint64_t)1000000)); }

/**
	\brief   Converts a relative milliseconds value to an #abstime_t.
	\param[in]   mills      The input value in milliseconds.
	\return                 An #abstime_t representing that many milliseconds from now.
*/
inline abstime_t FIOSRelativeMilliseconds(uint64_t mills) { return (FIOSGetCurrentTime() + FIOSMillisecondsToAbstime(mills)); }

/**
	\brief   Converts an #abstime_t to seconds.
	\param[in]   abstime    The input abstime value.
	\return                 The corresponding value in seconds, rounded up.
*/
inline int64_t FIOSAbstimeToSeconds(abstime_t abstime) { return ((FIOSAbstimeToNanoseconds(abstime) + 999999999) / 1000000000); }

/**
	\brief   Converts a seconds value to an #abstime_t.
	\param[in]   secs       The input value in seconds.
	\return                 The corresponding value as an #abstime_t.
*/
inline abstime_t FIOSSecondsToAbstime(uint64_t secs) { return FIOSNanosecondsToAbstime((uint64_t)((secs)*(uint64_t)1000000000)); }

/**
	\brief   Converts a relative seconds value to an #abstime_t.
	\param[in]   secs       The input value in seconds, using any integer or float type.
	\return                 An #abstime_t representing that many seconds from now.
*/
inline abstime_t FIOSRelativeSeconds(uint64_t secs) { return (FIOSGetCurrentTime() + FIOSSecondsToAbstime(secs)); }



/**
	\brief   Returns the current date.

	This function can be used to determine the current calendar date. Unlike #abstime_t, the value
	returned can be stored in a file and will still be valid when read later. 

	\note The value returned WILL be affected by changes in the system's current date/time settings. It is
	therefore not useful for wall-clock timing because the system date/time can change literally at any moment.
	For example, if you call this function twice in a row the difference could be years apart if the user
	(or an automatic NTP sync) has modified the system date and time in between the calls.
	
	\return  The current date.
*/
inline datetime_t FIOSGetCurrentDate() { return platform::currentDate(); }

/**
	\brief   Convert an #abstime_t to a #datetime_t.

	There may be some loss of fidelity when converting an absolute time to a date. Rounding
	may occur in either direction. If the #abstime_t is outside the range that can be represented
	by a #datetime_t, it is pinned to either the maximum or minimum date and this function returns false.
	
	\note This conversion will be affected by the system's current date/time settings. Since these
	settings can change at any time, this means that this function may return different values each time it's
	called with the same parameter!
	
	\param[in]    abstime     The input abstime value.
	\param[out]   pDate       Filled in with the output #datetime_t, or the closest #datetime_t that can be represented.
	\return                   True if the conversion could be made, false otherwise. If this function returns false, the returned date is pinned to #kDATE_EARLIEST or #kDATE_LATEST, whichever is closer to the actual date.
*/
inline bool FIOSAbstimeToDate(abstime_t abstime, datetime_t *pDate) { return platform::timeToDate(abstime, pDate); }

/**
	\brief    Convert a #datetime_t to an #abstime_t.

	Many dates may not be representable as an #abstime_t. There may be some loss of fidelity when converting
	an absolute time to a date. Rounding may occur in either direction. If the #datetime_t is outside the
	range that can be represented by an #abstime_t, it is pinned to either the maximum or minimum #abstime_t value
	and this function returns false.
	
	\note This conversion will be affected by the system's current date/time settings. Since these
	settings can change at any time, this means that this function may return different values each time it's
	called with the same parameter!
	
	\note Be aware that converting to an #abstime_t can fail! Because #abstime_t is often implemented
	as "ticks since system boot", a date as simple as "yesterday" might not be representable as an #abstime_t. You
	must be prepared to handle failure if you call this function.
	
	\param[in]    date       The input date.
	\param[out]   pAbstime   Filled in with the output #abstime_t, or the closest #abstime_t that can be represented.
	\return                  True if the conversion could be made, false otherwise. If this function returns false, the returned time is pinned to #kTIME_EARLIEST or #kTIME_LATEST, whichever is closer to the actual time.
*/
inline bool FIOSDateToAbstime(datetime_t date, abstime_t *pAbstime) { return platform::dateToTime(date, pAbstime); }

/**
	\brief   Convert a platform's native calendar-date representation to a #datetime_t.

	There may be some loss of fidelity when converting a native date to a date. Rounding may
	occur in either direction. If the input date is outside the range that can be represented
	by a #datetime_t, it is pinned to either the maximum or minimum date and this function returns false.
	
	The native date representation is defined in the appropriate fios_platform_imp_ps3.h file for the platform.
	This is usually struct tm, on platforms that define it.

	\param[in]    pNativeDate The input date value.
	\param[out]   pDate       Filled in with the output #datetime_t, or the closest #datetime_t that can be represented.
	\return                   True if the conversion could be made, false otherwise. If this function returns false, the returned date is pinned to #kDATE_EARLIEST or #kDATE_LATEST, whichever is closer to the actual date.
*/
inline bool FIOSNativeDateToDate(const platform::nativedate_t *pNativeDate, datetime_t *pDate) { return platform::nativeDateToDate(pNativeDate, pDate); }

/**
	\brief   Convert a #datetime_t to a platform's native calendar-date representation.

	There may be some loss of fidelity when converting a date to a native date. Rounding may
	occur in either direction. If the input date is outside the range that can be represented by the
	platform's native date format, it is pinned to either the maximum or minimum date and this function returns false.

	The native date representation is defined in the fios_platform_imp_ps3.h.
	This is usually struct tm, on platforms that define it.

	\param[in]   date         The input date.
	\param[out]  pNativeDate  Filled in with the output date, or the closest native date that can be represented.
	\return                   True if the conversion could be made, false otherwise. If this function returns false, the returned date is pinned to the closest date that can be represented.
*/
inline bool FIOSDateToNativeDate(datetime_t date, platform::nativedate_t *pNativeDate) { return platform::dateToNativeDate(date, pNativeDate); }

/** \brief Finds the difference between two dates in nanoseconds.
	\param[in]   dateA        The ending date.
	\param[in]   dateB        The starting date.
	\return      The difference between the dates in nanoseconds, for example, dateA - dateB. If either date is invalid (such as, high bit is set) then zero is returned.
*/
inline int64_t FIOSDateDifference(datetime_t dateB, datetime_t dateA) {
	return ((dateA | dateB) & kDATE_HIGHBIT) ? 0:(int64_t(dateA) - int64_t(dateB));
}

/** \brief Adds nanoseconds to a date.
	\param[in]   date         The starting date.
	\param[in]   nanos        Number of nanoseconds to add.
	\return      A date which is the given number of nanoseconds after the input date. If the date is invalid then it is returned unmodified. */
inline datetime_t FIOSDateAdd(datetime_t date, int64_t nanos) {
	return (date & kDATE_HIGHBIT) ? date:datetime_t(date+nanos);
}
}; /* namespace fios */
}; /* namespace cell */

/* Promote these functions to the top-level namespace so that they can be used without namespace qualifiers. */
using cell::fios::FIOSGetCurrentTime;
using cell::fios::FIOSAbstimeToNanoseconds;
using cell::fios::FIOSAbstimeToMicroseconds;
using cell::fios::FIOSAbstimeToMilliseconds;
using cell::fios::FIOSAbstimeToSeconds;
using cell::fios::FIOSNanosecondsToAbstime;
using cell::fios::FIOSMicrosecondsToAbstime;
using cell::fios::FIOSMillisecondsToAbstime;
using cell::fios::FIOSSecondsToAbstime;
using cell::fios::FIOSRelativeNanoseconds;
using cell::fios::FIOSRelativeMicroseconds;
using cell::fios::FIOSRelativeMilliseconds;
using cell::fios::FIOSRelativeSeconds;

using cell::fios::FIOSGetCurrentDate;
using cell::fios::FIOSAbstimeToDate;
using cell::fios::FIOSDateToAbstime;
using cell::fios::FIOSNativeDateToDate;
using cell::fios::FIOSDateToNativeDate;
using cell::fios::FIOSDateDifference;
using cell::fios::FIOSDateAdd;

#endif /* __CELL_FIOS_TIME_H__ */
