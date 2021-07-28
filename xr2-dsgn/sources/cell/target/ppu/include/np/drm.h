/*  SCE CONFIDENTIAL                                      */
/*  PlayStation(R)3 Programmer Tool Runtime Library 310.001 */
/*  Copyright (C) 2007 Sony Computer Entertainment Inc.   */
/*  All Rights Reserved.                                  */
#ifndef _SCE_NP_DRM_H_
#define _SCE_NP_DRM_H_

#ifdef __cplusplus
extern "C" {
#endif	/* __cplusplus */

/*
 * constant
 */
#define SCE_NP_DRM_OPEN_FLAG  (1<<1)

#define SCE_NP_DRM_EXITSPAWN2_EXIT_WO_FINI 0x4000000000000000ULL

#define SCE_NP_DRM_TIME_INFO_ENDLESS       0x7FFFFFFFFFFFFFFFULL

/*
 * Np Drm Struct
 */
typedef struct {
	uint8_t keydata[16];
} SceNpDrmKey;

typedef struct {
	uint64_t flag;
} SceNpDrmOpenArg;

/*
 * Np Drm API
 */

/* DRM rights check */
int sceNpDrmIsAvailable(
	const SceNpDrmKey *k_licensee,
	const char *path
	);

/* DRM rights check (without callback polling) */
int sceNpDrmIsAvailable2(
	const SceNpDrmKey *k_licensee,
	const char *path
	);

/* Game Purchase license verification */
int sceNpDrmVerifyUpgradeLicense(
	const char *content_id
	);

/* Game Purchase license verification (without callback polling) */
int sceNpDrmVerifyUpgradeLicense2(
	const char *content_id
	);

/* Execute Game Purchase for "Game Purchase" enabled games */
int sceNpDrmExecuteGamePurchase( void );

/* Get remaining time (usec) */
int sceNpDrmGetTimelimit(
	const char* path,
	uint64_t* time_remain
	);

/* exitspawn() with NPDRM */
int sceNpDrmProcessExitSpawn2(
	const SceNpDrmKey *k_licensee,
	const char *path,
	char const *argv[],
	char const *envp[],
	sys_addr_t data,
	size_t data_size,
	int prio,
	uint64_t flags
	);


/* legacy API : sys_game_process_exitspawn() with NPDRM */
int sceNpDrmProcessExitSpawn(
	const SceNpDrmKey *k_licensee,
	const char *path,
	char const *argv[],
	char const *envp[],
	sys_addr_t data,
	size_t data_size,
	int prio,
	uint64_t flags
	);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _SCE_NP_DRM_H_ */
