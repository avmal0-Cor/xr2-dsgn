/*  SCE CONFIDENTIAL                                      */
/*  PlayStation(R)3 Programmer Tool Runtime Library 310.001 */
/*  Copyright (C) 2006 Sony Computer Entertainment Inc.   */
/*  All Rights Reserved.                                  */
#ifndef __CELL_SSL_ERROR_H__
#define __CELL_SSL_ERROR_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * SSL Error codes
 */
/*
	CELL_ERROR_FACILITY_SSL			 0x074
		libssl		: 0x8074_0001 - 0x8074_0fff
 */

#define CELL_SSL_MAKE_ERROR(status) \
	CELL_ERROR_MAKE_ERROR(CELL_ERROR_FACILITY_SSL, status)

/* library has already been initialized */
/* ERR: CELL_SSL_MAKE_ERROR(0x0001) */
#define CELL_SSL_ERROR_NOT_INITIALIZED              CELL_ERROR_CAST(0x80740001)

/* the library has not yet been initialized */
/* ERR: CELL_SSL_MAKE_ERROR(0x0002) */
#define CELL_SSL_ERROR_ALREADY_INITIALIZED          CELL_ERROR_CAST(0x80740002)

/* failed trying to initialize ssl library */
/* ERR: CELL_SSL_MAKE_ERROR(0x0003) */
#define CELL_SSL_ERROR_INITIALIZATION_FAILED        CELL_ERROR_CAST(0x80740003)

/* no buffer was provided */
/* ERR: CELL_SSL_MAKE_ERROR(0x0004) */
#define CELL_SSL_ERROR_NO_BUFFER                    CELL_ERROR_CAST(0x80740004)

/* the pointer to the SSL Certificate provided is invalid */
/* ERR: CELL_SSL_MAKE_ERROR(0x0005) */
#define CELL_SSL_ERROR_INVALID_CERTIFICATE          CELL_ERROR_CAST(0x80740005)

/* failed to get the information requested */
/* ERR: CELL_SSL_MAKE_ERROR(0x0006) */
#define CELL_SSL_ERROR_UNRETRIEVABLE                CELL_ERROR_CAST(0x80740006)

/* ERR: CELL_SSL_MAKE_ERROR(0x0007) */
#define CELL_SSL_ERROR_INVALID_FORMAT               CELL_ERROR_CAST(0x80740007)

/* ERR: CELL_SSL_MAKE_ERROR(0x0008) */
#define CELL_SSL_ERROR_NOT_FOUND                    CELL_ERROR_CAST(0x80740008)

/* ERR: CELL_SSL_MAKE_ERROR(0x0009) */
#define CELL_SSL_ERROR_READ_FAILED                  CELL_ERROR_CAST(0x80740009)

/* invalid time element */
/* ERR: CELL_SSL_MAKE_ERROR(0x0031) */
#define CELL_SSL_ERROR_INVALID_TIME                 CELL_ERROR_CAST(0x80740031)

/* invalid time element with negative values */
/* ERR: CELL_SSL_MAKE_ERROR(0x0032) */
#define CELL_SSL_ERROR_INVALID_NEGATIVE_TIME        CELL_ERROR_CAST(0x80740032)

/* value in the time element is incorrect */
/* ERR: CELL_SSL_MAKE_ERROR(0x0033) */
#define CELL_SSL_ERROR_INCORRECT_TIME               CELL_ERROR_CAST(0x80740033)

/* type in the time element is not defined correctly */
/* ERR: CELL_SSL_MAKE_ERROR(0x0034) */
#define CELL_SSL_ERROR_UNDEFINED_TIME_TYPE          CELL_ERROR_CAST(0x80740034)

/* no available memory */
/* ERR: CELL_SSL_MAKE_ERROR(0x0035) */
#define CELL_SSL_ERROR_NO_MEMORY                    CELL_ERROR_CAST(0x80740035)

/* no string to process */
/* ERR: CELL_SSL_MAKE_ERROR(0x0036) */
#define CELL_SSL_ERROR_NO_STRING                    CELL_ERROR_CAST(0x80740036)

/* unknown certificate load flag */
/* ERR: CELL_SSL_MAKE_ERROR(0x0005) */
#define CELL_SSL_ERROR_UNKNOWN_LOAD_CERT            CELL_ERROR_CAST(0x80740037)

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _CELL_HTTP_ERROR_H */
