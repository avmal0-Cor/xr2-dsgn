/*  SCE CONFIDENTIAL                                      */
/*  PlayStation(R)3 Programmer Tool Runtime Library 310.001 */
/*  Copyright (C) 2008 Sony Computer Entertainment Inc.   */
/*  All Rights Reserved.                                  */
#ifndef __CELL_HTTP_ERROR_H__
#define __CELL_HTTP_ERROR_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define CELL_HTTP_NET_ERROR_TYPE_MASK               CELL_ERROR_CAST(0xffffff00)
#define CELL_HTTP_NET_ERROR_MASK                    CELL_ERROR_CAST(0xff)

#define CELL_HTTP_NET_ERROR(_eHttp, _eNet) \
	CELL_ERROR_CAST(_eHttp | (_eNet & CELL_HTTP_NET_ERROR_MASK))

#define CELL_HTTP_IS_NET_RESOLVER_ERROR(_eHttp) \
	((_eHttp & CELL_HTTP_NET_ERROR_TYPE_MASK) == CELL_HTTP_ERROR_NET_RESOLVER)

/*
	CELL_ERROR_FACILITY_HTTP			 0x071
		libhttp		: 0x8071_0001 - 0x8071_0fff
		   https    : 0x8071_0a01 - 0x8071_0aff
		libhttp_util: 0x8071_1001 - 0x8071_10ff
 */

#define CELL_HTTP_MAKE_ERROR(status) \
	CELL_ERROR_MAKE_ERROR(CELL_ERROR_FACILITY_HTTP, status)

/*E library has already been initialized */
/* ERR: CELL_HTTP_MAKE_ERROR(0x0001) */
#define CELL_HTTP_ERROR_ALREADY_INITIALIZED         CELL_ERROR_CAST(0x80710001)

/*E the library has not yet been initialized */
/* ERR: CELL_HTTP_MAKE_ERROR(0x0002) */
#define CELL_HTTP_ERROR_NOT_INITIALIZED             CELL_ERROR_CAST(0x80710002)

/*E tried to malloc, but got NULL (no memory) */
/* ERR: CELL_HTTP_MAKE_ERROR(0x0003) */
#define CELL_HTTP_ERROR_NO_MEMORY                   CELL_ERROR_CAST(0x80710003)

/*E no buffer was provided */
/* ERR: CELL_HTTP_MAKE_ERROR(0x0004) */
#define CELL_HTTP_ERROR_NO_BUFFER                   CELL_ERROR_CAST(0x80710004)

/*E invalid string (probably NULL) */
/* ERR: CELL_HTTP_MAKE_ERROR(0x0005) */
#define CELL_HTTP_ERROR_NO_STRING                   CELL_ERROR_CAST(0x80710005)

/*E insufficient buffer */
/* ERR: CELL_HTTP_MAKE_ERROR(0x0006) */
#define CELL_HTTP_ERROR_INSUFFICIENT                CELL_ERROR_CAST(0x80710006)

/* ERR: CELL_HTTP_MAKE_ERROR(0x0007) */
#define CELL_HTTP_ERROR_INVALID_URI                 CELL_ERROR_CAST(0x80710007)

/* ERR: CELL_HTTP_MAKE_ERROR(0x0008) */
#define CELL_HTTP_ERROR_INVALID_HEADER              CELL_ERROR_CAST(0x80710008)

/* ERR: CELL_HTTP_MAKE_ERROR(0x0009) */
#define CELL_HTTP_ERROR_BAD_METHOD                  CELL_ERROR_CAST(0x80710009)

/*E Bad Client ID */
/* ERR: CELL_HTTP_MAKE_ERROR(0x0010) */
#define CELL_HTTP_ERROR_BAD_CLIENT                  CELL_ERROR_CAST(0x80710010)

/*E Bad Transaction ID */
/* ERR: CELL_HTTP_MAKE_ERROR(0x0011) */
#define CELL_HTTP_ERROR_BAD_TRANS                   CELL_ERROR_CAST(0x80710011)

/*E no connection available */
/* ERR: CELL_HTTP_MAKE_ERROR(0x0012) */
#define CELL_HTTP_ERROR_NO_CONNECTION               CELL_ERROR_CAST(0x80710012)

/*E no request has been sent yet */
/* ERR: CELL_HTTP_MAKE_ERROR(0x0013) */
#define CELL_HTTP_ERROR_NO_REQUEST_SENT             CELL_ERROR_CAST(0x80710013)

/*E request has already been built */
/* ERR: CELL_HTTP_MAKE_ERROR(0x0014) */
#define CELL_HTTP_ERROR_ALREADY_BUILT               CELL_ERROR_CAST(0x80710014)

/*E request has already been sent */
/* ERR: CELL_HTTP_MAKE_ERROR(0x0015) */
#define CELL_HTTP_ERROR_ALREADY_SENT                CELL_ERROR_CAST(0x80710015)

/*E that header doesn't exist */
/* ERR: CELL_HTTP_MAKE_ERROR(0x0016) */
#define CELL_HTTP_ERROR_NO_HEADER                   CELL_ERROR_CAST(0x80710016)

/*E no content-length was defined */
/* ERR: CELL_HTTP_MAKE_ERROR(0x0017) */
#define CELL_HTTP_ERROR_NO_CONTENT_LENGTH           CELL_ERROR_CAST(0x80710017)

/*E exceeded max auto-redirects(default:5) */
/* ERR: CELL_HTTP_MAKE_ERROR(0x0018) */
#define CELL_HTTP_ERROR_TOO_MANY_REDIRECTS          CELL_ERROR_CAST(0x80710018)

/*E exceeded max auto-authentication tries(default:5) */
/* ERR: CELL_HTTP_MAKE_ERROR(0x0019) */
#define CELL_HTTP_ERROR_TOO_MANY_AUTHS              CELL_ERROR_CAST(0x80710019)

/*E no connection for the transaction is available, try again later? */
/* ERR: CELL_HTTP_MAKE_ERROR(0x0020) */
#define CELL_HTTP_ERROR_TRANS_NO_CONNECTION         CELL_ERROR_CAST(0x80710020)

/*E tried to callback to the application, but failed */
/* ERR: CELL_HTTP_MAKE_ERROR(0x0021) */
#define CELL_HTTP_ERROR_CB_FAILED                   CELL_ERROR_CAST(0x80710021)

/*E the connection has no transactions in the pipeline --internal error */
/* ERR: CELL_HTTP_MAKE_ERROR(0x0022) */
#define CELL_HTTP_ERROR_NOT_PIPED                   CELL_ERROR_CAST(0x80710022)

/*E there is another request next in the pipeline */
/* ERR: CELL_HTTP_MAKE_ERROR(0x0023) */
#define CELL_HTTP_ERROR_OUT_OF_ORDER_PIPE           CELL_ERROR_CAST(0x80710023)

/*E this transaction has been aborted */
/* ERR: CELL_HTTP_MAKE_ERROR(0x0024) */
#define CELL_HTTP_ERROR_TRANS_ABORTED               CELL_ERROR_CAST(0x80710024)

/*E the pipeline this transaction was using has been aborted,
 *  and thus broke the pipeline
 */
/* ERR: CELL_HTTP_MAKE_ERROR(0x0025) */
#define CELL_HTTP_ERROR_BROKEN_PIPELINE             CELL_ERROR_CAST(0x80710025)

/*E the request made is for an unavailable service */
/* ERR: CELL_HTTP_MAKE_ERROR(0x0026) */
#define CELL_HTTP_ERROR_UNAVAILABLE                 CELL_ERROR_CAST(0x80710026)

/*E an invalid value was found in processing */
/* ERR: CELL_HTTP_MAKE_ERROR(0x0027) */
#define CELL_HTTP_ERROR_INVALID_VALUE               CELL_ERROR_CAST(0x80710027)

/*E Cannot authenticate the transaction (not enough info) */
/* ERR: CELL_HTTP_MAKE_ERROR(0x0028) */
#define CELL_HTTP_ERROR_CANNOT_AUTHENTICATE         CELL_ERROR_CAST(0x80710028)

/*E the cookie is not valid */
/* ERR: CELL_HTTP_MAKE_ERROR(0x0041) */
#define CELL_HTTP_ERROR_COOKIE_NOT_FOUND            CELL_ERROR_CAST(0x80710041)

/*E the cookie contains an invalid domain */
/* ERR: CELL_HTTP_MAKE_ERROR(0x0042) */
#define CELL_HTTP_ERROR_COOKIE_INVALID_DOMAIN       CELL_ERROR_CAST(0x80710042)

/*E cache has already been initialized */
/* ERR: CELL_HTTP_MAKE_ERROR(0x0043) */
#define CELL_HTTP_ERROR_CACHE_ALREADY_INITIALIZED   CELL_ERROR_CAST(0x80710043)

/*E cache has not yet been initialized */
/* ERR: CELL_HTTP_MAKE_ERROR(0x0044) */
#define CELL_HTTP_ERROR_CACHE_NOT_INITIALIZED       CELL_ERROR_CAST(0x80710044)

/*E status/header line exceeds maximum allowed */
/* ERR: CELL_HTTP_MAKE_ERROR(0x0045) */
#define CELL_HTTP_ERROR_LINE_EXCEEDS_MAX            CELL_ERROR_CAST(0x80710045)

/*E basic auth is disabled, yet basic auth is required for authorization */
/* ERR: CELL_HTTP_MAKE_ERROR(0x0046) */
#define CELL_HTTP_ERROR_REQUIRES_BASIC_AUTH         CELL_ERROR_CAST(0x80710046)

/*** internal errors (0x005#) ***/
/*E catchall for unexpected errors */
/* ERR: CELL_HTTP_MAKE_ERROR(0x0051) */
#define CELL_HTTP_ERROR_UNKNOWN                     CELL_ERROR_CAST(0x80710051)

/*E Internal failure */
/* ERR: CELL_HTTP_MAKE_ERROR(0x0052) */
#define CELL_HTTP_ERROR_INTERNAL                    CELL_ERROR_CAST(0x80710052)

/*E not a removable connection -- not free or non-existant */
/* ERR: CELL_HTTP_MAKE_ERROR(0x0053) */
#define CELL_HTTP_ERROR_NONREMOVABLE                CELL_ERROR_CAST(0x80710053)

/*E Bad Connection */
/* ERR: CELL_HTTP_MAKE_ERROR(0x0054) */
#define CELL_HTTP_ERROR_BAD_CONN                    CELL_ERROR_CAST(0x80710054)

/*E Bad Connection Manager */
/* ERR: CELL_HTTP_MAKE_ERROR(0x0055) */
#define CELL_HTTP_ERROR_BAD_MAN                     CELL_ERROR_CAST(0x80710055)

/*E no connection pool */
/* ERR: CELL_HTTP_MAKE_ERROR(0x0056) */
#define CELL_HTTP_ERROR_NO_POOL                     CELL_ERROR_CAST(0x80710056)

/*E no request (structure), not been created? */
/* ERR: CELL_HTTP_MAKE_ERROR(0x0057) */
#define CELL_HTTP_ERROR_NO_REQUEST                  CELL_ERROR_CAST(0x80710057)

/*E internal failure */
/* ERR: CELL_HTTP_MAKE_ERROR(0x0058) */
#define CELL_HTTP_ERROR_LOCK_FAILED                 CELL_ERROR_CAST(0x80710058)

/*E invalid data */
/* ERR: CELL_HTTP_MAKE_ERROR(0x0059) */
#define CELL_HTTP_ERROR_INVALID_DATA                CELL_ERROR_CAST(0x80710059)

/*E lost connection in the middle of chunk processing */
/* ERR: CELL_HTTP_MAKE_ERROR(0x005a) */
#define CELL_HTTP_ERROR_BROKEN_CHUNK                CELL_ERROR_CAST(0x8071005a)

/*E error occured in setting up for decoding */
/* ERR: CELL_HTTP_MAKE_ERROR(0x005b) */
#define CELL_HTTP_ERROR_DECODE_SETUP                CELL_ERROR_CAST(0x8071005b)

/*E error occured in setting up for decoding */
/* ERR: CELL_HTTP_MAKE_ERROR(0x005c) */
#define CELL_HTTP_ERROR_DECODE_STREAM               CELL_ERROR_CAST(0x8071005c)

/*E the decode stream has been interrupted */
/* ERR: CELL_HTTP_MAKE_ERROR(0x005c) */
#define CELL_HTTP_ERROR_BROKEN_DECODE_STREAM        CELL_ERROR_CAST(0x8071005d)

/* (0x006#) */
/*E disk cache path is invalid */
/* ERR: CELL_HTTP_MAKE_ERROR(0x0060) */
#define CELL_HTTP_ERROR_INVALID_DCACHE_PATH         CELL_ERROR_CAST(0x80710060)

/*E disk cache has already been initialized */
/* ERR: CELL_HTTP_MAKE_ERROR(0x0061) */
#define CELL_HTTP_ERROR_DCACHE_ALREADY_INITIALIZED  CELL_ERROR_CAST(0x80710061)

/*E disk cache has not yet been initialized */
/* ERR: CELL_HTTP_MAKE_ERROR(0x0062) */
#define CELL_HTTP_ERROR_DCACHE_NOT_INITIALIZED      CELL_ERROR_CAST(0x80710062)

/*E too many disk cache entries */
/* ERR: CELL_HTTP_MAKE_ERROR(0x0063) */
#define CELL_HTTP_ERROR_TOO_MANY_DCACHE_ENTRY       CELL_ERROR_CAST(0x80710063)

/*E duplicated cache entry is found */
/* ERR: CELL_HTTP_MAKE_ERROR(0x0064) */
#define CELL_HTTP_ERROR_DUP_DCACHE_ENTRY            CELL_ERROR_CAST(0x80710064)

/*E unknown disk cache writing error */
/* ERR: CELL_HTTP_MAKE_ERROR(0x0065) */
#define CELL_HTTP_ERROR_WRITE_DCACHE                CELL_ERROR_CAST(0x80710065)

/*E unknown disk cache reading error */
/* ERR: CELL_HTTP_MAKE_ERROR(0x0066) */
#define CELL_HTTP_ERROR_READ_DCACHE                 CELL_ERROR_CAST(0x80710066)

/*E too large cache entry */
/* ERR: CELL_HTTP_MAKE_ERROR(0x0067) */
#define CELL_HTTP_ERROR_CACHE_TOO_LARGE             CELL_ERROR_CAST(0x80710067)

/*E invalid disk cache version */
/* ERR: CELL_HTTP_MAKE_ERROR(0x0068) */
#define CELL_HTTP_ERROR_INVALID_DCACHE_VERSION      CELL_ERROR_CAST(0x80710068)

/*E disk cache file is broken */
/* ERR: CELL_HTTP_MAKE_ERROR(0x0069) */
#define CELL_HTTP_ERROR_DCACHE_FILE_BROKEN          CELL_ERROR_CAST(0x80710069)

/*E disk cache exceeds maximum allowd */
/* ERR: CELL_HTTP_MAKE_ERROR(0x006a) */
#define CELL_HTTP_ERROR_DCACHE_EXCEEDS_MAX          CELL_ERROR_CAST(0x8071006a)

/*E disk cache is busy */
/* ERR: CELL_HTTP_MAKE_ERROR(0x006b) */
#define CELL_HTTP_ERROR_DCACHE_BUSY                 CELL_ERROR_CAST(0x8071006b)

/*E disk cache index file is broken */
/* ERR: CELL_HTTP_MAKE_ERROR(0x006c) */
#define CELL_HTTP_ERROR_DCACHE_INDEX_BROKEN         CELL_ERROR_CAST(0x8071006c)

/*E invalid index node type */
/* ERR: CELL_HTTP_MAKE_ERROR(0x006d) */
#define CELL_HTTP_ERROR_INVALID_DCACHE_INDEX_NODE   CELL_ERROR_CAST(0x8071006d)

/*E detect inconsistency in cache file */
/* ERR: CELL_HTTP_MAKE_ERROR(0x006e) */
#define CELL_HTTP_ERROR_DCACHE_FILE_INCONSISTENCY   CELL_ERROR_CAST(0x8071006e)

/*E too long url */
/* ERR: CELL_HTTP_MAKE_ERROR(0x006f) */
#define CELL_HTTP_ERROR_DCACHE_URI_TOO_LONG         CELL_ERROR_CAST(0x8071006f)

/*E eof in reading */
/* ERR: CELL_HTTP_MAKE_ERROR(0x0070) */
#define CELL_HTTP_ERROR_READ_DCACHE_EOF             CELL_ERROR_CAST(0x80710070)

/*E end of node */
/* ERR: CELL_HTTP_MAKE_ERROR(0x0071) */
#define CELL_HTTP_ERROR_END_OF_DCACHE_INDEX_NODE    CELL_ERROR_CAST(0x80710071)

/*E no cache memory */
/* ERR: CELL_HTTP_MAKE_ERROR(0x0072) */
#define CELL_HTTP_ERROR_NO_CACHE_MEMORY             CELL_ERROR_CAST(0x80710072)

/*E broken cache file */
/* ERR: CELL_HTTP_MAKE_ERROR(0x0073) */
#define CELL_HTTP_ERROR_DCACHE_BROKEN               CELL_ERROR_CAST(0x80710073)

/*E too many cache file for writing */
/* ERR: CELL_HTTP_MAKE_ERROR(0x0074) */
#define CELL_HTTP_ERROR_DCACHE_TOO_MANY_WRITE       CELL_ERROR_CAST(0x80710074)

/*E too many cache file for reading */
/* ERR: CELL_HTTP_MAKE_ERROR(0x0075) */
#define CELL_HTTP_ERROR_DCACHE_TOO_MANY_READ        CELL_ERROR_CAST(0x80710075)

/*E fatal error have been occured */
/* ERR: CELL_HTTP_MAKE_ERROR(0x0076) */
#define CELL_HTTP_ERROR_DCACHE_FATAL		        CELL_ERROR_CAST(0x80710076)

/*E unsupported feature */
/* ERR: CELL_HTTP_MAKE_ERROR(0x0077) */
#define CELL_HTTP_ERROR_DCACHE_UNSUPPORTED_FEATURE  CELL_ERROR_CAST(0x80710077)

/*E cache index node is alread open */
/* ERR: CELL_HTTP_MAKE_ERROR(0x0078) */
#define CELL_HTTP_ERROR_DCACHE_INDEX_IS_ALREADY_OPEN CELL_ERROR_CAST(0x80710078)
/*E cache index node is now opening */
/* ERR: CELL_HTTP_MAKE_ERROR(0x0079) */
#define CELL_HTTP_ERROR_DCACHE_INDEX_IS_OPENING		CELL_ERROR_CAST(0x80710079)

/*E diskcache unknown error */
/* ERR: CELL_HTTP_MAKE_ERROR(0x0080) */
#define CELL_HTTP_ERROR_DCACHE_UNKNOWN				CELL_ERROR_CAST(0x80710080)

/*E cache index node is already closed */
/* ERR: CELL_HTTP_MAKE_ERROR(0x0081) */
#define CELL_HTTP_ERROR_DCACHE_INDEX_IS_CLOSED		CELL_ERROR_CAST(0x80710081)

/*E opening diskcache is aborted */
/* ERR: CELL_HTTP_MAKE_ERROR(0x0082) */
#define CELL_HTTP_ERROR_DCACHE_ABORTED				CELL_ERROR_CAST(0x80710082)

/*E cache index node is closing */
/* ERR: CELL_HTTP_MAKE_ERROR(0x0085) */
#define CELL_HTTP_ERROR_DCACHE_INDEX_IS_CLOSING		CELL_ERROR_CAST(0x80710083)

/*E unknown index state */
/* ERR: CELL_HTTP_MAKE_ERROR(0x0084) */
#define CELL_HTTP_ERROR_DCACHE_UNKNOWN_INDEX_STATE	CELL_ERROR_CAST(0x80710084)

/* Network Errors (0x009#) */

/*E received EOF from the socket */
/* ERR: CELL_HTTP_MAKE_ERROR(0x0091) */
#define CELL_HTTP_ERROR_NET_FIN                     CELL_ERROR_CAST(0x80710091)

/*E timed out while connecting */
/* ERR: CELL_HTTP_MAKE_ERROR(0x0092) */
#define CELL_HTTP_ERROR_NET_CONNECT_TIMEOUT         CELL_ERROR_CAST(0x80710092)

/*E timed out while polling */
/* ERR: CELL_HTTP_MAKE_ERROR(0x0093) */
#define CELL_HTTP_ERROR_NET_SELECT_TIMEOUT          CELL_ERROR_CAST(0x80710093)

/*E timed out while polling */
/* ERR: CELL_HTTP_MAKE_ERROR(0x0094) */
#define CELL_HTTP_ERROR_NET_SEND_TIMEOUT            CELL_ERROR_CAST(0x80710094)

/*E For these Network related errors, the last two digits
 *  will further describe the network error, which comes from the
 *  network socket API's errno or h_errno, in the case of the resolver.
 */
/* ERR: CELL_HTTP_MAKE_ERROR(0x0100) */
#define CELL_HTTP_ERROR_NET_RESOLVER                CELL_ERROR_CAST(0x80710100)

/* ERR: CELL_HTTP_MAKE_ERROR(0x0200) */
#define CELL_HTTP_ERROR_NET_ABORT                   CELL_ERROR_CAST(0x80710200)

/* ERR: CELL_HTTP_MAKE_ERROR(0x0300) */
#define CELL_HTTP_ERROR_NET_OPTION                  CELL_ERROR_CAST(0x80710300)

/* ERR: CELL_HTTP_MAKE_ERROR(0x0400) */
#define CELL_HTTP_ERROR_NET_SOCKET                  CELL_ERROR_CAST(0x80710400)

/* ERR: CELL_HTTP_MAKE_ERROR(0x0500) */
#define CELL_HTTP_ERROR_NET_CONNECT                 CELL_ERROR_CAST(0x80710500)

/* ERR: CELL_HTTP_MAKE_ERROR(0x0600) */
#define CELL_HTTP_ERROR_NET_SEND                    CELL_ERROR_CAST(0x80710600)

/* ERR: CELL_HTTP_MAKE_ERROR(0x0700) */
#define CELL_HTTP_ERROR_NET_RECV                    CELL_ERROR_CAST(0x80710700)

/* ERR: CELL_HTTP_MAKE_ERROR(0x0800) */
#define CELL_HTTP_ERROR_NET_SELECT                  CELL_ERROR_CAST(0x80710800)


/*
 * HTTPS Error codes
 */
/*E cannot load the certificate */
/* ERR: CELL_HTTP_MAKE_ERROR(0x0a01) */
#define CELL_HTTPS_ERROR_CERTIFICATE_LOAD           CELL_ERROR_CAST(0x80710a01)

/*E ssl memory call failed */
/* ERR: CELL_HTTP_MAKE_ERROR(0x0a02) */
#define CELL_HTTPS_ERROR_BAD_MEMORY                 CELL_ERROR_CAST(0x80710a02)

/*E failed to create ssl context */
/* ERR: CELL_HTTP_MAKE_ERROR(0x0a03) */
#define CELL_HTTPS_ERROR_CONTEXT_CREATION           CELL_ERROR_CAST(0x80710a03)

/*E failed to create the ssl connection */
/* ERR: CELL_HTTP_MAKE_ERROR(0x0a04) */
#define CELL_HTTPS_ERROR_CONNECTION_CREATION        CELL_ERROR_CAST(0x80710a04)

/*E failed to associate the socket with ssl connection */
/* ERR: CELL_HTTP_MAKE_ERROR(0x0a05) */
#define CELL_HTTPS_ERROR_SOCKET_ASSOCIATION         CELL_ERROR_CAST(0x80710a05)

/*E SSL Connect Handshake error */
/* ERR: CELL_HTTP_MAKE_ERROR(0x0a06) */
#define CELL_HTTPS_ERROR_HANDSHAKE                  CELL_ERROR_CAST(0x80710a06)

/*E failed to retrieve SSL certificate */
/* ERR: CELL_HTTP_MAKE_ERROR(0x0a07) */
#define CELL_HTTPS_ERROR_LOOKUP_CERTIFICATE         CELL_ERROR_CAST(0x80710a07)

/*E libssl is not available (has not been initialized?) */
/* ERR: CELL_HTTP_MAKE_ERROR(0x0a08) */
#define CELL_HTTPS_ERROR_NO_SSL                     CELL_ERROR_CAST(0x80710a08)

/*E cannot load the private key */
/* ERR: CELL_HTTP_MAKE_ERROR(0x0a09) */
#define CELL_HTTPS_ERROR_KEY_LOAD                   CELL_ERROR_CAST(0x80710a09)

/*E the private key and client certificate do not match */
/* ERR: CELL_HTTP_MAKE_ERROR(0x0a0b) */
#define CELL_HTTPS_ERROR_CERT_KEY_MISMATCH          CELL_ERROR_CAST(0x80710a0a)

/*E the private key needs client certificate */
/* ERR: CELL_HTTP_MAKE_ERROR(0x0a0b) */
#define CELL_HTTPS_ERROR_KEY_NEEDS_CERT             CELL_ERROR_CAST(0x80710a0b)

/*E the client certificate needs private key */
/* ERR: CELL_HTTP_MAKE_ERROR(0x0a0c) */
#define CELL_HTTPS_ERROR_CERT_NEEDS_KEY             CELL_ERROR_CAST(0x80710a0c)

/*E internal error code */
/* ERR: CELL_HTTP_MAKE_ERROR(0x0a0d) */
#define CELL_HTTPS_ERROR_RETRY_CONNECTION           CELL_ERROR_CAST(0x80710a0d)


/*E Network failed on SSL Connect
 *  error is reported as the other network errors in libhttp,
 *  using CELL_HTTP_NET_ERROR() macro, refer to error.h
 */
/* ERR: CELL_HTTP_MAKE_ERROR(0x0b00) */
#define CELL_HTTPS_ERROR_NET_SSL_CONNECT            CELL_ERROR_CAST(0x80710b00)

/*E Network failed on SSL Send
 *  error is reported as the other network errors in libhttp,
 *  using CELL_HTTP_NET_ERROR() macro, refer to error.h
 */
/* ERR: CELL_HTTP_MAKE_ERROR(0x0c00) */
#define CELL_HTTPS_ERROR_NET_SSL_SEND               CELL_ERROR_CAST(0x80710c00)

/*E Network failed on SSL Receive
 *  error is reported as the other network errors in libhttp,
 *  using CELL_HTTP_NET_ERROR() macro, refer to error.h
 */
/* ERR: CELL_HTTP_MAKE_ERROR(0x0d00) */
#define CELL_HTTPS_ERROR_NET_SSL_RECV               CELL_ERROR_CAST(0x80710d00)


/*
 * libhttp_util error codes (0x10##)
 */
/* ERR: CELL_HTTP_MAKE_ERROR(0x1001) */
#define CELL_HTTP_UTIL_ERROR_NO_MEMORY              CELL_ERROR_CAST(0x80711001)

/* ERR: CELL_HTTP_MAKE_ERROR(0x1002) */
#define CELL_HTTP_UTIL_ERROR_NO_BUFFER              CELL_ERROR_CAST(0x80711002)

/* ERR: CELL_HTTP_MAKE_ERROR(0x1003) */
#define CELL_HTTP_UTIL_ERROR_NO_STRING              CELL_ERROR_CAST(0x80711003)

/* ERR: CELL_HTTP_MAKE_ERROR(0x1004) */
#define CELL_HTTP_UTIL_ERROR_INSUFFICIENT           CELL_ERROR_CAST(0x80711004)

/* ERR: CELL_HTTP_MAKE_ERROR(0x1005) */
#define CELL_HTTP_UTIL_ERROR_INVALID_URI            CELL_ERROR_CAST(0x80711005)

/* ERR: CELL_HTTP_MAKE_ERROR(0x1006) */
#define CELL_HTTP_UTIL_ERROR_INVALID_HEADER         CELL_ERROR_CAST(0x80711006)

/* ERR: CELL_HTTP_MAKE_ERROR(0x1007) */
#define CELL_HTTP_UTIL_ERROR_INVALID_REQUEST        CELL_ERROR_CAST(0x80711007)

/* ERR: CELL_HTTP_MAKE_ERROR(0x1008) */
#define CELL_HTTP_UTIL_ERROR_INVALID_RESPONSE       CELL_ERROR_CAST(0x80711008)

/* ERR: CELL_HTTP_MAKE_ERROR(0x1009) */
#define CELL_HTTP_UTIL_ERROR_INVALID_LENGTH         CELL_ERROR_CAST(0x80711009)

/* ERR: CELL_HTTP_MAKE_ERROR(0x100a) */
#define CELL_HTTP_UTIL_ERROR_INVALID_CHARACTER      CELL_ERROR_CAST(0x8071100a)

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _CELL_HTTP_ERROR_H */
