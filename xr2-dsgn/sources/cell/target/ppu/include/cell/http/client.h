/*  SCE CONFIDENTIAL                                      */
/*  PlayStation(R)3 Programmer Tool Runtime Library 310.001 */
/*  Copyright (C) 2008 Sony Computer Entertainment Inc.   */
/*  All Rights Reserved.                                  */
#ifndef __CELL_HTTP_CLIENT_H__
#define __CELL_HTTP_CLIENT_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

struct CellHttpClient;
struct CellHttpTransaction;
typedef struct CellHttpClient* CellHttpClientId;
typedef struct CellHttpTransaction* CellHttpTransId;

typedef const void* CellHttpSslId;

typedef struct CellHttpsData {
	char *ptr;
	size_t size;
} CellHttpsData;

/*E These macros are the size of the buffers passed in
 *  through the CellHttpAuthenticationCB callback.
 *  NOTE: these strings require NULL-termination within the 256 bytes provided.
 */
#define CELL_HTTP_MAX_USERNAME (256)
#define CELL_HTTP_MAX_PASSWORD (256)

/****
  HTTP Callback Definitions
 ****/
typedef int (*CellHttpAuthenticationCallback)(CellHttpTransId transId,
											  const char *realm,
											  const CellHttpUri *uri,
											  char *username,
											  char *password,
											  bool *save,
											  void *userArg);
typedef int (*CellHttpTransactionStateCallback)(CellHttpTransId transId,
												int32_t state,
												void *userArg);
typedef int (*CellHttpRedirectCallback)(CellHttpTransId transId,
										const CellHttpStatusLine *response,
										const CellHttpUri *from,
										const CellHttpUri *to,
										void *userArg);

typedef int (*CellHttpsSslCallback)(uint32_t verifyErr,
									CellSslCert const sslCerts[],
									int certNum,
									const char *hostname,
									CellHttpSslId id,
									void *userArg);

typedef int (*CellHttpCookieSendCallback)(CellHttpTransId transId,
										  const CellHttpUri *uri,
										  const char *cookieValue,
										  void *userArg);

typedef int (*CellHttpCookieRecvCallback)(CellHttpTransId transId,
										  const CellHttpUri *uri,
										  const char *cookieValue,
										  void *userArg);

/*E http initialization/termination functions */
int cellHttpInit(void *pool, size_t poolSize);
int cellHttpEnd(void);

/*E https initialization/termination functions */
int cellHttpsInit(size_t caCertNum, const CellHttpsData *caList);
int cellHttpsEnd(void);

/*E library-wide proxy settings */
int cellHttpSetProxy(const CellHttpUri *proxy);
int cellHttpGetProxy(CellHttpUri *proxy, void *pool, size_t poolSize, size_t *required);

/*E cookie related functions */
int cellHttpInitCookie(void *pool, size_t poolSize);
int cellHttpEndCookie(void);
int cellHttpAddCookieWithClientId(const CellHttpUri *uri, const char *cookie, CellHttpClientId clientId);
int cellHttpSessionCookieFlush(CellHttpClientId clientId);
int cellHttpCookieExportWithClientId(void *buffer, size_t size, size_t *exportSize,CellHttpClientId clientId);
int cellHttpCookieImportWithClientId(const void *buffer, size_t size, CellHttpClientId clientId);

int cellHttpClientSetCookieSendCallback(CellHttpClientId clientId, CellHttpCookieSendCallback cbfunc, void *userArg);
int cellHttpClientSetCookieRecvCallback(CellHttpClientId clientId, CellHttpCookieRecvCallback cbfunc, void *userArg);

/*E client-specific functions */
int cellHttpCreateClient(CellHttpClientId *clientId);
int cellHttpDestroyClient(CellHttpClientId clientId);
int cellHttpClientSetProxy(CellHttpClientId clientId, const CellHttpUri *proxy);
int cellHttpClientGetProxy(CellHttpClientId clientId, CellHttpUri *proxy, void *pool, size_t poolSize, size_t *required);
int cellHttpClientSetVersion(CellHttpClientId clientId, uint32_t major, uint32_t minor);
int cellHttpClientGetVersion(CellHttpClientId clientId, uint32_t *major, uint32_t *minor);
int cellHttpClientSetPipeline(CellHttpClientId clientId, bool enable);
int cellHttpClientGetPipeline(CellHttpClientId clientId, bool *enable);
int cellHttpClientSetKeepAlive(CellHttpClientId clientId, bool enable);
int cellHttpClientGetKeepAlive(CellHttpClientId clientId, bool *enable);
int cellHttpClientSetAutoRedirect(CellHttpClientId clientId, bool enable);
int cellHttpClientGetAutoRedirect(CellHttpClientId clientId, bool *enable);
int cellHttpClientSetAutoAuthentication(CellHttpClientId clientId, bool enable);
int cellHttpClientGetAutoAuthentication(CellHttpClientId clientId, bool *enable);
int cellHttpClientSetAuthenticationCacheStatus(CellHttpClientId clientId, bool enable);
int cellHttpClientGetAuthenticationCacheStatus(CellHttpClientId clientId, bool *enable);
int cellHttpClientSetCookieStatus(CellHttpClientId clientId, bool enable);
int cellHttpClientGetCookieStatus(CellHttpClientId clientId, bool *enable);
int cellHttpClientSetAuthenticationCallback(CellHttpClientId clientId, CellHttpAuthenticationCallback cbfunc, void *userArg);
int cellHttpClientSetTransactionStateCallback(CellHttpClientId clientId, CellHttpTransactionStateCallback cbfunc, void *userArg);
int cellHttpClientSetRedirectCallback(CellHttpClientId clientId, CellHttpRedirectCallback cbfunc, void *userArg);
int cellHttpClientSetUserAgent(CellHttpClientId clientId, const char *userAgent);
int cellHttpClientGetUserAgent(CellHttpClientId clientId, char *userAgent, size_t size, size_t *required);
int cellHttpClientCloseAllConnections(CellHttpClientId clientId);
int cellHttpClientCloseConnections(CellHttpClientId clientId, const CellHttpUri *uri);
int cellHttpClientPollConnections(CellHttpClientId clientId, CellHttpTransId *transId, int64_t usec);
int cellHttpClientSetRecvTimeout(CellHttpClientId clientId, int64_t usec);
int cellHttpClientGetRecvTimeout(CellHttpClientId clientId, int64_t *usec);
int cellHttpClientSetSendTimeout(CellHttpClientId clientId, int64_t usec);
int cellHttpClientGetSendTimeout(CellHttpClientId clientId, int64_t *usec);
int cellHttpClientSetConnTimeout(CellHttpClientId clientId, int64_t usec);
int cellHttpClientGetConnTimeout(CellHttpClientId clientId, int64_t *usec);
int cellHttpClientSetTotalPoolSize(CellHttpClientId clientId, size_t poolSize);
int cellHttpClientGetTotalPoolSize(CellHttpClientId clientId, size_t *poolSize);
int cellHttpClientSetPerHostPoolSize(CellHttpClientId clientId, size_t poolSize);
int cellHttpClientGetPerHostPoolSize(CellHttpClientId clientId, size_t *poolSize);
int cellHttpClientSetPerHostKeepAliveMax(CellHttpClientId clientId, size_t maxSize);
int cellHttpClientGetPerHostKeepAliveMax(CellHttpClientId clientId, size_t *maxSize);
int cellHttpClientSetPerPipelineMax(CellHttpClientId clientId, size_t pipeMax);
int cellHttpClientGetPerPipelineMax(CellHttpClientId clientId, size_t *pipeMax);
int cellHttpClientSetResponseBufferMax(CellHttpClientId clientId, size_t max);
int cellHttpClientGetResponseBufferMax(CellHttpClientId clientId, size_t *max);
int cellHttpClientSetRecvBufferSize(CellHttpClientId clientId, int size);
int cellHttpClientGetRecvBufferSize(CellHttpClientId clientId, int *size);
int cellHttpClientGetAllHeaders(CellHttpClientId clientId, CellHttpHeader **headers, size_t *items, void *pool, size_t poolSize, size_t *required);
int cellHttpClientGetHeader(CellHttpClientId clientId, CellHttpHeader *header, const char *name, void *pool, size_t poolSize, size_t *required);
int cellHttpClientSetHeader(CellHttpClientId clientId, const CellHttpHeader *header);
int cellHttpClientAddHeader(CellHttpClientId clientId, const CellHttpHeader *header);
int cellHttpClientDeleteHeader(CellHttpClientId clientId, const char *name);

int cellHttpClientSetSslCallback(CellHttpClientId clientId, CellHttpsSslCallback cbfunc, void *userArg);
int cellHttpClientSetSslClientCertificate(CellHttpClientId clientId, const CellHttpsData *cert, const CellHttpsData *privKey);

/*E transaction-specific functions */
int cellHttpCreateTransaction(CellHttpTransId *transId, CellHttpClientId clientId, const char *method, const CellHttpUri *uri);
int cellHttpDestroyTransaction(CellHttpTransId transId);
int cellHttpSendRequest(CellHttpTransId transId, const void *buf, size_t size, size_t *sent);
int cellHttpRecvResponse(CellHttpTransId transId, void *buf, size_t size, size_t *recvd);
int cellHttpTransactionGetUri(CellHttpTransId transId, CellHttpUri *uri, void *pool, size_t poolSize, size_t *required);
int cellHttpTransactionCloseConnection(CellHttpTransId transId);
int cellHttpTransactionReleaseConnection(CellHttpTransId transId, int *sid);
int cellHttpTransactionAbortConnection(CellHttpTransId transId);

int cellHttpRequestSetContentLength(CellHttpTransId transId, uint64_t totalSize);
int cellHttpRequestGetContentLength(CellHttpTransId transId, uint64_t *totalSize);
int cellHttpRequestGetChunkedTransferStatus(CellHttpTransId transId, bool *enable);
int cellHttpRequestSetChunkedTransferStatus(CellHttpTransId transId, bool enable);
int cellHttpRequestGetAllHeaders(CellHttpTransId transId, CellHttpHeader **headers, size_t *items, void *pool, size_t poolSize, size_t *required);
int cellHttpRequestGetHeader(CellHttpTransId transId, CellHttpHeader *header, const char *name, void *pool, size_t poolSize, size_t *required);
int cellHttpRequestSetHeader(CellHttpTransId transId, const CellHttpHeader *header);
int cellHttpRequestAddHeader(CellHttpTransId transId, const CellHttpHeader *header);
int cellHttpRequestDeleteHeader(CellHttpTransId transId, const char *name);

int cellHttpResponseGetContentLength(CellHttpTransId transId, uint64_t *length);
int cellHttpResponseGetStatusCode(CellHttpTransId transId, int32_t *code);
int cellHttpResponseGetAllHeaders(CellHttpTransId transId, CellHttpHeader **headers, size_t *items, void *pool, size_t poolSize, size_t *required);
int cellHttpResponseGetHeader(CellHttpTransId transId, CellHttpHeader *header, const char *name, void *pool, size_t poolSize, size_t *required);
int cellHttpResponseGetStatusLine(CellHttpTransId transId, CellHttpStatusLine* status, void *pool, size_t poolSize, size_t *required);

int cellHttpTransactionGetSslCipherName(CellHttpTransId transId, char *name, size_t size, size_t *required);
int cellHttpTransactionGetSslCipherId(CellHttpTransId transId, int32_t *id);
int cellHttpTransactionGetSslCipherVersion(CellHttpTransId transId, char *version, size_t size, size_t *required);
int cellHttpTransactionGetSslCipherBits(CellHttpTransId transId, int32_t *effectiveBits, int32_t *algorithmBits);
/*E buffer for the cipher string must be at least 128 bytes */
int cellHttpTransactionGetSslCipherString(CellHttpTransId transId, char *buffer, size_t size);
int cellHttpTransactionGetSslVersion(CellHttpTransId transId, int32_t *version);
int cellHttpTransactionGetSslId(CellHttpTransId transId, CellHttpSslId *id);


/**
 * SSL Callback's verify error codes
 */
/*E no error */
#define CELL_HTTPS_VERIFY_ERROR_NONE                              (0x00000000U)
/*E the server provided no certificates to verify */
#define CELL_HTTPS_VERIFY_ERROR_NO_CERT                           (0x00000001U)
/*E internal error */
#define CELL_HTTPS_VERIFY_ERROR_BAD_SSL                           (0x00000002U)
/*E internal error */
#define CELL_HTTPS_VERIFY_ERROR_BAD_CLIENT                        (0x00000004U)
/*E none of the certificates match the cerficate chain */
#define CELL_HTTPS_VERIFY_ERROR_UNKNOWN_CA                        (0x00000008U)
/*E the SSL chain is broken, should NOT be verified */
#define CELL_HTTPS_VERIFY_ERROR_BAD_CHAIN                         (0x00000010U)
/*E internal error -- out of memory */
#define CELL_HTTPS_VERIFY_ERROR_NO_MEMORY                         (0x00000020U)
/*E not able to verify the certificate with CA public key */
#define CELL_HTTPS_VERIFY_ERROR_NOT_VERIFIABLE                    (0x00000040U)
/*E certificate is not a CA, verify fails */
#define CELL_HTTPS_VERIFY_ERROR_INVALID_CERT                      (0x00000080U)
/*E basic constraints are not up held correctly, should NOT be verified */
#define CELL_HTTPS_VERIFY_ERROR_BAD_CONSTRAINT                    (0x00000100U)
/*E couldn't verify the next certificate in the chain, should NOT be verified*/
#define CELL_HTTPS_VERIFY_ERROR_VERIFY_FAILED                     (0x00000200U)
/*E the common name provided in the certificate does not match the server */
#define CELL_HTTPS_VERIFY_ERROR_COMMON_NAME                       (0x00000400U)
/*E passed the notAfter date */
#define CELL_HTTPS_VERIFY_ERROR_EXPIRED                           (0x00000800U)
/*E the notBefore date is in the future */
#define CELL_HTTPS_VERIFY_ERROR_NOT_YET_VALID                     (0x00001000U)


/****
  HTTP Methods
 ****/
static const char CELL_HTTP_METHOD_OPTIONS[] = "OPTIONS";
static const char CELL_HTTP_METHOD_GET[]     = "GET";
static const char CELL_HTTP_METHOD_HEAD[]    = "HEAD";
static const char CELL_HTTP_METHOD_POST[]    = "POST";
static const char CELL_HTTP_METHOD_PUT[]     = "PUT";
static const char CELL_HTTP_METHOD_DELETE[]  = "DELETE";
static const char CELL_HTTP_METHOD_TRACE[]   = "TRACE";

/****
  HTTP Transaction States
 ****/
#define CELL_HTTP_TRANSACTION_STATE_GETTING_CONNECTION      (1)
#define CELL_HTTP_TRANSACTION_STATE_PREPARING_REQUEST       (2)
#define CELL_HTTP_TRANSACTION_STATE_SENDING_REQUEST         (3)
#define CELL_HTTP_TRANSACTION_STATE_SENDING_BODY            (4)
#define CELL_HTTP_TRANSACTION_STATE_WAITING_FOR_REPLY       (5)
#define CELL_HTTP_TRANSACTION_STATE_READING_REPLY           (6)
#define CELL_HTTP_TRANSACTION_STATE_SETTING_REDIRECTION     (7)
#define CELL_HTTP_TRANSACTION_STATE_SETTING_AUTHENTICATION  (8)

/****
  HTTP Status Codes
 ****/
	/* Informational 1xx */
#define CELL_HTTP_STATUS_CODE_Continue                        (100)
#define CELL_HTTP_STATUS_CODE_Switching_Protocols             (101)
#define CELL_HTTP_STATUS_CODE_Processing                      (102) /* RFC2518 WebDAV */
	/* Successful 2xx */
#define CELL_HTTP_STATUS_CODE_OK                              (200)
#define CELL_HTTP_STATUS_CODE_Created                         (201)
#define CELL_HTTP_STATUS_CODE_Accepted                        (202)
#define CELL_HTTP_STATUS_CODE_NonAuthoritive_Information      (203)
#define CELL_HTTP_STATUS_CODE_No_Content                      (204)
#define CELL_HTTP_STATUS_CODE_Reset_Content                   (205)
#define CELL_HTTP_STATUS_CODE_Partial_Content                 (206)
#define CELL_HTTP_STATUS_CODE_MultiStatus                     (207) /* RFC2518 WebDAV */
	/* Redirection 3xx */
#define CELL_HTTP_STATUS_CODE_Multiple_Choices                (300)
#define CELL_HTTP_STATUS_CODE_Moved_Permanently               (301)
#define CELL_HTTP_STATUS_CODE_Moved_Temporarily               (302) /* RFC1945 */
#define CELL_HTTP_STATUS_CODE_Found CELL_HTTP_STATUS_CODE_Moved_Temporarily /* RFC2616 */
#define CELL_HTTP_STATUS_CODE_See_Other                       (303)
#define CELL_HTTP_STATUS_CODE_Not_Modified                    (304)
#define CELL_HTTP_STATUS_CODE_Use_Proxy                       (305)
	/*CELL_HTTP_STATUS_CODE_UNUSED                            (306) */
#define CELL_HTTP_STATUS_CODE_Temporary_Redirect              (307)
	/* Client Error 4xx */
#define CELL_HTTP_STATUS_CODE_Bad_Request                     (400)
#define CELL_HTTP_STATUS_CODE_Unauthorized                    (401)
#define CELL_HTTP_STATUS_CODE_Payment_Required                (402)
#define CELL_HTTP_STATUS_CODE_Forbidden                       (403)
#define CELL_HTTP_STATUS_CODE_Not_Found                       (404)
#define CELL_HTTP_STATUS_CODE_Method_Not_Allowed              (405)
#define CELL_HTTP_STATUS_CODE_Not_Acceptable                  (406)
#define CELL_HTTP_STATUS_CODE_Proxy_Authentication_Required   (407)
#define CELL_HTTP_STATUS_CODE_Request_Timeout                 (408)
#define CELL_HTTP_STATUS_CODE_Conflict                        (409)
#define CELL_HTTP_STATUS_CODE_Gone                            (410)
#define CELL_HTTP_STATUS_CODE_Length_Required                 (411)
#define CELL_HTTP_STATUS_CODE_Precondition_Failed             (412)
#define CELL_HTTP_STATUS_CODE_Request_Entity_Too_Large        (413)
#define CELL_HTTP_STATUS_CODE_RequestURI_Too_Long             (414)
#define CELL_HTTP_STATUS_CODE_Unsupported_Media_Type          (415)
#define CELL_HTTP_STATUS_CODE_Requested_Range_Not_Satisfiable (416)
#define CELL_HTTP_STATUS_CODE_Expectation_Failed              (417)
#define CELL_HTTP_STATUS_CODE_Unprocessable_Entity            (422) /* RFC2518 WebDAV */
#define CELL_HTTP_STATUS_CODE_Locked                          (423) /* RFC2518 WebDAV */
#define CELL_HTTP_STATUS_CODE_Failed_Dependency               (424) /* RFC2518 WebDAV */
#define CELL_HTTP_STATUS_CODE_Upgrade_Required                (426) /* RFC2817 TLS/HTTP */
	/* Server Error 5xx */
#define CELL_HTTP_STATUS_CODE_Internal_Server_Error           (500)
#define CELL_HTTP_STATUS_CODE_Not_Implemented                 (501)
#define CELL_HTTP_STATUS_CODE_Bad_Gateway                     (502)
#define CELL_HTTP_STATUS_CODE_Service_Unavailable             (503)
#define CELL_HTTP_STATUS_CODE_Gateway_Timeout                 (504)
#define CELL_HTTP_STATUS_CODE_HTTP_Version_Not_Supported      (505)
#define CELL_HTTP_STATUS_CODE_Insufficient_Storage            (507)  /* RFC2518 WebDAV */


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __CELL_HTTP_CLIENT_H__ */
