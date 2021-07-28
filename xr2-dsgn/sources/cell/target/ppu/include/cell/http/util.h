/*  SCE CONFIDENTIAL                                      */
/*  PlayStation(R)3 Programmer Tool Runtime Library 310.001 */
/*  Copyright (C) 2006 Sony Computer Entertainment Inc.   */
/*  All Rights Reserved.                                  */
#ifndef __CELL_HTTP_UTIL_H__
#define __CELL_HTTP_UTIL_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef struct CellHttpUri {
	const char *scheme;   /* http or https                  */
	const char *hostname; /* www.foo.com; 192.168.0.1       */
	const char *username;
	const char *password;
	const char *path;     /* /; /bar.html; /mydir/some.txt  */
	uint32_t port;        /* port number in host byte order */
	uint8_t reserved[4];
} CellHttpUri;

typedef struct CellHttpUriPath {
	const char *path;
	const char *query;
	const char *fragment;
} CellHttpUriPath;

typedef struct CellHttpRequestLine {
	const char *method;
	const char *path;
	const char *protocol;
	uint32_t majorVersion;
	uint32_t minorVersion;
} CellHttpRequestLine;

typedef struct CellHttpStatusLine {
	const char *protocol;
	uint32_t majorVersion;
	uint32_t minorVersion;
	const char *reasonPhrase;
	int32_t statusCode;
	uint8_t reserved[4];
} CellHttpStatusLine;

typedef struct CellHttpHeader {
	const char *name;
	const char *value;
} CellHttpHeader;

/****
  Encoding functions
 ****/
int cellHttpUtilEscapeUri(char *out, size_t outSize, const unsigned char *in, size_t inSize, size_t *required);
int cellHttpUtilUnescapeUri(unsigned char *out, size_t size, const char *in, size_t *required);
int cellHttpUtilFormUrlEncode(char *out, size_t outSize, const unsigned char *in, size_t inSize, size_t *required);
int cellHttpUtilFormUrlDecode(unsigned char *out, size_t size, const char *in, size_t *required);

#define CELL_HTTP_UTIL_BASE64_ENC_BUF_SIZE(_size) ((_size + 2) / 3 * 4)
#define CELL_HTTP_UTIL_BASE64_DEC_BUF_SIZE(_size) (_size / 4 * 3)
int cellHttpUtilBase64Encoder(char *out, const void *input, size_t len);
int cellHttpUtilBase64Decoder(void *output, const char *in, size_t len);


/****
  Copy functions
 ****/
int cellHttpUtilCopyUri(CellHttpUri *dest, const CellHttpUri *src, void *pool, size_t poolSize, size_t *required);
int cellHttpUtilCopyHeader(CellHttpHeader *dest, const CellHttpHeader *src, void *pool, size_t poolSize, size_t *required);
int cellHttpUtilCopyStatusLine(CellHttpStatusLine *dest, const CellHttpStatusLine *src, void *pool, size_t poolSize, size_t *required);
int cellHttpUtilMergeUriPath(CellHttpUri *uri, const CellHttpUri *src, const char *path, void *pool, size_t poolSize, size_t *required);
int cellHttpUtilAppendHeaderValue(CellHttpHeader *dest, const CellHttpHeader *src, const char *value, void *pool, size_t poolSize, size_t *required);

/****
  Parser functions
 ****/
int cellHttpUtilParseUri(CellHttpUri *uri, const char *str, void *pool, size_t size, size_t *required);
int cellHttpUtilParseUriPath(CellHttpUriPath *path, const char *str, void *pool, size_t size, size_t *required);
/*E basically the same as cellHttpUtilParseUri(), but it does not require
 *  a scheme, so the pattern is  username:password@hostname:port
 */
int cellHttpUtilParseProxy(CellHttpUri *proxy, const char *str, void *pool, size_t size, size_t *required);
int cellHttpUtilParseStatusLine(CellHttpStatusLine *resp, const char *str, size_t len, void *pool, size_t size, size_t *required, size_t *parsedLength);
int cellHttpUtilParseHeader(CellHttpHeader *header, const char *str, size_t len, void *pool, size_t size, size_t *required, size_t *parsedLength);

/****
  Builder functions
 ****/
/*E makes the request line, Method Path Proto/D.d CRLF */
int cellHttpUtilBuildRequestLine(const CellHttpRequestLine *req, char *buf, size_t len, size_t *required);
/*E only creates one header, Name: Value CRLF */
int cellHttpUtilBuildHeader(const CellHttpHeader *header, char *buf, size_t len, size_t *required);

#define CELL_HTTP_UTIL_URI_FLAG_FULL_URI       (0x00000000)
#define CELL_HTTP_UTIL_URI_FLAG_NO_SCHEME      (0x00000001)
#define CELL_HTTP_UTIL_URI_FLAG_NO_CREDENTIALS (0x00000002)
#define CELL_HTTP_UTIL_URI_FLAG_NO_PASSWORD    (0x00000004)
#define CELL_HTTP_UTIL_URI_FLAG_NO_PATH        (0x00000008)
int cellHttpUtilBuildUri(const CellHttpUri *uri, char *buf, size_t len, size_t *required, int32_t flags);

int cellHttpUtilSweepPath(char *dst, const char *src, size_t srcSize);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __CELL_HTTP_UTIL_H__ */
