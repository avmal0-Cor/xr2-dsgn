/*  SCE CONFIDENTIAL                                      */
/*  PlayStation(R)3 Programmer Tool Runtime Library 310.001 */
/*  Copyright (C) 2006 Sony Computer Entertainment Inc.   */
/*  All Rights Reserved.                                  */
#ifndef __CELL_SSL_SSL_H__
#define __CELL_SSL_SSL_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define CELL_SSL_CIPHER_UNKNOWN                         (-1)
#define CELL_SSL_CIPHER_RSA_WITH_3DES_EDE_CBC_SHA       (0)
#define CELL_SSL_CIPHER_RSA_WITH_RC4_128_SHA            (1)
#define CELL_SSL_CIPHER_RSA_WITH_RC4_128_MD5            (2)
#define CELL_SSL_CIPHER_RSA_WITH_DES_CBC_SHA            (3)
#define CELL_SSL_CIPHER_RSA_EXPORT1024_WITH_DES_CBC_SHA (4)
#define CELL_SSL_CIPHER_RSA_EXPORT1024_WITH_RC4_56_SHA  (5)
#define CELL_SSL_CIPHER_RSA_EXPORT_WITH_DES40_CBC_SHA   (6)
#define CELL_SSL_CIPHER_RSA_EXPORT_WITH_RC2_CBC_40_MD5  (7)
#define CELL_SSL_CIPHER_RSA_EXPORT_WITH_RC4_40_MD5      (8)

#define CELL_SSL_VERSION_UNKNOWN (-1)
#define CELL_SSL_VERSION_TLS1    (0)
#define CELL_SSL_VERSION_SSL3    (1)

int cellSslInit(void *pool, size_t poolSize);
int cellSslEnd(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __CELL_SSL_SSL_H__ */
