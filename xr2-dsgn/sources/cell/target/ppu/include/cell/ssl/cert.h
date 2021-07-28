/*  SCE CONFIDENTIAL                                      */
/*  PlayStation(R)3 Programmer Tool Runtime Library 310.001 */
/*  Copyright (C) 2009 Sony Computer Entertainment Inc.   */
/*  All Rights Reserved.                                  */
#ifndef __CELL_SSL_CERT_H__
#define __CELL_SSL_CERT_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef void* CellSslCert;
#define CellSslCertName void*

/* SSL certificate loader flags */
#define CELL_SSL_LOAD_CERT_ALL       (CELL_SSL_LOAD_CERT_SCE |   \
								      CELL_SSL_LOAD_CERT_NORMAL)
#define CELL_SSL_LOAD_CERT_SCE       (CELL_SSL_LOAD_CERT_SCE01 | \
								      CELL_SSL_LOAD_CERT_SCE02 | \
								      CELL_SSL_LOAD_CERT_SCE03 | \
								      CELL_SSL_LOAD_CERT_SCE04 | \
								      CELL_SSL_LOAD_CERT_SCE05)
#define CELL_SSL_LOAD_CERT_NORMAL    (CELL_SSL_LOAD_CERT_BALTIMORE_CT      | \
                                      CELL_SSL_LOAD_CERT_CLASS1_PCA_G2_V2  | \
                                      CELL_SSL_LOAD_CERT_CLASS1_PCA_G3_V2  | \
                                      CELL_SSL_LOAD_CERT_CLASS1_PCA_SS_V4  | \
                                      CELL_SSL_LOAD_CERT_CLASS2_PCA_G2_V2  | \
                                      CELL_SSL_LOAD_CERT_CLASS2_PCA_G3_V2  | \
                                      CELL_SSL_LOAD_CERT_CLASS2_PCA_SS_V4  | \
                                      CELL_SSL_LOAD_CERT_CLASS3_PCA_G2_V2  | \
                                      CELL_SSL_LOAD_CERT_CLASS3_PCA_G3_V2  | \
                                      CELL_SSL_LOAD_CERT_CLASS3_PCA_SS_V4  | \
                                      CELL_SSL_LOAD_CERT_CLASS4_PCA_G2_V2  | \
                                      CELL_SSL_LOAD_CERT_CLASS4_PCA_G3_V2  | \
                                      CELL_SSL_LOAD_CERT_ENTRUST_NET_SS_CA | \
                                      CELL_SSL_LOAD_CERT_EQUIFAX_SEC_CA    | \
                                      CELL_SSL_LOAD_CERT_EQUIFAX_SEC_EBUS1 | \
                                      CELL_SSL_LOAD_CERT_GEOTRUST_GCA      | \
                                      CELL_SSL_LOAD_CERT_GLOBALSIGN_RCA    | \
                                      CELL_SSL_LOAD_CERT_GTE_CT_G_RCA      | \
                                      CELL_SSL_LOAD_CERT_GTE_CT_ROOT       | \
                                      CELL_SSL_LOAD_CERT_RSA1024_V1        | \
                                      CELL_SSL_LOAD_CERT_RSA2048_V3        | \
                                      CELL_SSL_LOAD_CERT_RSA_SECURE_SERVER | \
                                      CELL_SSL_LOAD_CERT_THAWTE_PREM_SCA   | \
                                      CELL_SSL_LOAD_CERT_THAWTE_SCA        | \
                                      CELL_SSL_LOAD_CERT_VALICERT_CLASS2   | \
                                      CELL_SSL_LOAD_CERT_VERISIGN_TSA_CA   | \
                                      CELL_SSL_LOAD_CERT_AAA_CERT_SERVICES | \
                                      CELL_SSL_LOAD_CERT_ADDTRUST_EXT_CA   | \
                                      CELL_SSL_LOAD_CERT_UTN_USERFIRST_HW  | \
									  CELL_SSL_LOAD_CERT_EQUIFAX_SEC_G_EBUS1)

#define CELL_SSL_LOAD_CERT_SCE01               (0x0000000000000001ULL)
#define CELL_SSL_LOAD_CERT_SCE02               (0x0000000000000002ULL)
#define CELL_SSL_LOAD_CERT_SCE03               (0x0000000000000004ULL)
#define CELL_SSL_LOAD_CERT_SCE04               (0x0000000000000008ULL)
#define CELL_SSL_LOAD_CERT_SCE05               (0x0000000000000010ULL)

#define CELL_SSL_LOAD_CERT_BALTIMORE_CT        (0x0000000000000020ULL)

#define CELL_SSL_LOAD_CERT_CLASS1_PCA_G2_V2    (0x0000000000000040ULL)
#define CELL_SSL_LOAD_CERT_CLASS1_PCA_G3_V2    (0x0000000000000080ULL)
#define CELL_SSL_LOAD_CERT_CLASS1_PCA_SS_V4    (0x0000000000000100ULL)

#define CELL_SSL_LOAD_CERT_CLASS2_PCA_G2_V2    (0x0000000000000200ULL)
#define CELL_SSL_LOAD_CERT_CLASS2_PCA_G3_V2    (0x0000000000000400ULL)
#define CELL_SSL_LOAD_CERT_CLASS2_PCA_SS_V4    (0x0000000000000800ULL)

#define CELL_SSL_LOAD_CERT_CLASS3_PCA_G2_V2    (0x0000000000001000ULL)
#define CELL_SSL_LOAD_CERT_CLASS3_PCA_G3_V2    (0x0000000000002000ULL)
#define CELL_SSL_LOAD_CERT_CLASS3_PCA_SS_V4    (0x0000000000004000ULL)

#define CELL_SSL_LOAD_CERT_CLASS4_PCA_G2_V2    (0x0000000000008000ULL)
#define CELL_SSL_LOAD_CERT_CLASS4_PCA_G3_V2    (0x0000000000010000ULL)

#define CELL_SSL_LOAD_CERT_ENTRUST_NET_SS_CA   (0x0000000000020000ULL)
#define CELL_SSL_LOAD_CERT_EQUIFAX_SEC_CA      (0x0000000000040000ULL)
#define CELL_SSL_LOAD_CERT_EQUIFAX_SEC_EBUS1   (0x0000000000080000ULL)

#define CELL_SSL_LOAD_CERT_GEOTRUST_GCA        (0x0000000000100000ULL)
#define CELL_SSL_LOAD_CERT_GLOBALSIGN_RCA      (0x0000000000200000ULL)

#define CELL_SSL_LOAD_CERT_GTE_CT_G_RCA        (0x0000000000400000ULL)
#define CELL_SSL_LOAD_CERT_GTE_CT_ROOT         (0x0000000000800000ULL)

#define CELL_SSL_LOAD_CERT_RSA1024_V1          (0x0000000001000000ULL)
#define CELL_SSL_LOAD_CERT_RSA2048_V3          (0x0000000002000000ULL)
#define CELL_SSL_LOAD_CERT_RSA_SECURE_SERVER   (0x0000000004000000ULL)
#define CELL_SSL_LOAD_CERT_THAWTE_PREM_SCA     (0x0000000008000000ULL)
#define CELL_SSL_LOAD_CERT_THAWTE_SCA          (0x0000000010000000ULL)
#define CELL_SSL_LOAD_CERT_VALICERT_CLASS2     (0x0000000020000000ULL)
#define CELL_SSL_LOAD_CERT_VERISIGN_TSA_CA     (0x0000000040000000ULL)
#define CELL_SSL_LOAD_CERT_AAA_CERT_SERVICES   (0x0000000080000000ULL)
#define CELL_SSL_LOAD_CERT_ADDTRUST_EXT_CA     (0x0000000100000000ULL)
#define CELL_SSL_LOAD_CERT_UTN_USERFIRST_HW    (0x0000000200000000ULL)
#define CELL_SSL_LOAD_CERT_EQUIFAX_SEC_G_EBUS1 (0x0000000400000000ULL)

/* Max size for a certificate's MD5 Fingerprint */
#define CELL_SSL_MD5_FINGERPRINT_MAX_SIZE   (20)

/* SSL certificate loader */
int cellSslCertificateLoader(uint64_t flag, char *buffer, size_t size, size_t *required);

/* SSL certificate information get functions */
int cellSslCertGetSerialNumber(const CellSslCert sslCert, const uint8_t **sboData, size_t *sboLength);
int cellSslCertGetPublicKey(const CellSslCert sslCert, const uint8_t **sboData, size_t *sboLength);
int cellSslCertGetRsaPublicKeyModulus(const CellSslCert sslCert, const uint8_t **sboData, size_t *sboLength);
int cellSslCertGetRsaPublicKeyExponent(const CellSslCert sslCert, const uint8_t **sboData, size_t *sboLength);
int cellSslCertGetNotBefore(const CellSslCert sslCert, CellRtcTick *begin);
int cellSslCertGetNotAfter(const CellSslCert sslCert, CellRtcTick *limit);
int cellSslCertGetSubjectName(const CellSslCert sslCert, const CellSslCertName *certName);
int cellSslCertGetIssuerName(const CellSslCert sslCert, const CellSslCertName *certName);

int cellSslCertGetNameEntryCount(const CellSslCertName certName, uint32_t *entryCount);
int cellSslCertGetNameEntryInfo(const CellSslCertName certName, uint32_t entryNum, const char **oidName, const uint8_t **value, size_t *valueLength, int32_t flag);
int cellSslCertGetMd5Fingerprint(const CellSslCert sslCert, const uint8_t *buf, uint32_t *plen);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __CELL_SSL_CERT_H__ */
