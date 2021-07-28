/*  SCE CONFIDENTIAL                                      */
/*  PlayStation(R)3 Programmer Tool Runtime Library 310.001 */
/*  Copyright (C) 2009 Sony Computer Entertainment Inc.   */
/*  All Rights Reserved.                                  */
#ifndef _SCE_NP_UTIL_H_
#define _SCE_NP_UTIL_H_

#ifdef __cplusplus
extern "C" {
#endif	/* __cplusplus */

int sceNpUtilCmpNpId(const SceNpId *npid1, const SceNpId *npid2);
int sceNpUtilCmpNpIdInOrder(const SceNpId *npid1, const SceNpId *npid2, int *order);

int sceNpUtilBuildCdnUrl(const char *url, char *buf, size_t bufSize, size_t *required, void *option );


typedef struct SceNpUtilBandwidthTestResult {
	double upload_bps;
	double download_bps;
	int result;
	char padding[4];
} SceNpUtilBandwidthTestResult;

#define SCE_NP_UTIL_BANDWIDTH_TEST_STATUS_NONE		0
#define SCE_NP_UTIL_BANDWIDTH_TEST_STATUS_RUNNING	1
#define SCE_NP_UTIL_BANDWIDTH_TEST_STATUS_FINISHED	2

int sceNpUtilBandwidthTestInitStart(int prio, size_t stack);
int sceNpUtilBandwidthTestGetStatus(void);
int sceNpUtilBandwidthTestShutdown(struct SceNpUtilBandwidthTestResult *result);
int sceNpUtilBandwidthTestAbort(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _SCE_NP_UTIL_H_ */
