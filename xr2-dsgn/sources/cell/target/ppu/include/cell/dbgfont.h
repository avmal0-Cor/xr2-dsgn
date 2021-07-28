/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2008 Sony Computer Entertainment Inc. 
* All Rights Reserved.
*/


#ifndef __CELL_DBGFONT_H__
#define __CELL_DBGFONT_H__


#include <stdint.h>
#include <cell/error.h>
#include <cell/gcm.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/* Initialize Structure for PSGL */
typedef struct CellDbgFontConfig {
	uint32_t bufSize;
	uint32_t screenWidth;
	uint32_t screenHeight;
} CellDbgFontConfig;

/* Initialize Structure for libgcm */
typedef struct CellDbgFontConfigGcm {
	sys_addr_t localBufAddr;
	uint32_t localBufSize;
	sys_addr_t mainBufAddr;
	uint32_t mainBufSize;
	uint32_t option;
	uint32_t screenWidth;
	uint32_t screenHeight;
} CellDbgFontConfigGcm;

/* Common Console Structure */
typedef struct CellDbgFontConsoleConfig {
	float posLeft;
	float posTop;
	uint32_t cnsWidth;
	uint32_t cnsHeight;
	float scale;
	uint32_t color;
} CellDbgFontConsoleConfig;

typedef int32_t CellDbgFontConsoleId;


#define CELL_DBGFONT_STDOUT_ID (-1)

/* Macros for libgcm */
#define CELL_DBGFONT_TEXTURE_SIZE  (98304)
#define CELL_DBGFONT_FRAGMENT_SIZE (128)
#define CELL_DBGFONT_VERTEX_SIZE   (64)
#define CELL_DBGFONT_VERTEX_LOCAL  (0x00000000)
#define CELL_DBGFONT_VERTEX_MAIN   (0x00000010)
#define CELL_DBGFONT_TEXTURE_LOCAL (0x00000000)
#define CELL_DBGFONT_TEXTURE_MAIN  (0x00000020)
#define CELL_DBGFONT_SYNC_OFF      (0x00000000)
#define CELL_DBGFONT_SYNC_ON       (0x00000100)
#define CELL_DBGFONT_VIEWPORT_OFF  (0x00000000)
#define CELL_DBGFONT_VIEWPORT_ON   (0x00000200)
#define CELL_DBGFONT_MINFILTER_NEAREST   (0x00000000)
#define CELL_DBGFONT_MINFILTER_LINEAR    (0x00001000)
#define CELL_DBGFONT_MAGFILTER_NEAREST   (0x00000000)
#define CELL_DBGFONT_MAGFILTER_LINEAR    (0x00002000)


/* Error Code */
/*
	CELL_ERROR_FACILITY_DEBUG	0x051
	libdbgfont			0x8051_0101 - 0x8051_01ff
	#define CELL_DBGFONT_MAKE_ERROR(status) \
		CELL_ERROR_MAKE_ERROR(CELL_ERROR_FACILITY_DEBUG, status)
*/


/* ERR: CELL_DBGFONT_MAKE_ERROR(0x0104) */
#define CELL_DBGFONT_ERROR_MALLOC		(CELL_ERROR_CAST(0x80510104))

/* ERR: CELL_DBGFONT_MAKE_ERROR(0x0105) */
#define CELL_DBGFONT_ERROR_TEMP_BUFFER		(CELL_ERROR_CAST(0x80510105))

/* ERR: CELL_DBGFONT_MAKE_ERROR(0x0106) */
#define CELL_DBGFONT_ERROR_LABEL_BUFFER		(CELL_ERROR_CAST(0x80510106))

/* ERR: CELL_DBGFONT_MAKE_ERROR(0x0107) */
#define CELL_DBGFONT_ERROR_PARAM		(CELL_ERROR_CAST(0x80510107))

/* ERR: CELL_DBGFONT_MAKE_ERROR(0x0108) */
#define CELL_DBGFONT_ERROR_EXISTS		(CELL_ERROR_CAST(0x80510108))

/* ERR: CELL_DBGFONT_MAKE_ERROR(0x0109) */
#define CELL_DBGFONT_ERROR_NOT_INIT		(CELL_ERROR_CAST(0x80510109))

/* ERR: CELL_DBGFONT_MAKE_ERROR(0x010a) */
#define CELL_DBGFONT_ERROR_CG_PROFILE		(CELL_ERROR_CAST(0x8051010a))


/* ERR: CELL_DBGFONT_MAKE_ERROR(0x0111) */
#define CELL_DBGFONT_ERROR_CONSOLE_ID		(CELL_ERROR_CAST(0x80510111))

/* ERR: CELL_DBGFONT_MAKE_ERROR(0x0112) */
#define CELL_DBGFONT_ERROR_CONSOLE_FULL		(CELL_ERROR_CAST(0x80510112))




/* Common Functions for PSGL */
int cellDbgFontInit(const CellDbgFontConfig *config);
int cellDbgFontExit(void);
int cellDbgFontDraw(void);

/* Common Functions for libgcm */
int cellDbgFontInitGcm(const CellDbgFontConfigGcm *config);
int cellDbgFontExitGcm(void);
int cellDbgFontDrawGcm(void);

/* Label Type Functions */
int32_t cellDbgFontPuts(
					const float x, 
					const float y, 
					const float scale, 
					const uint32_t color, 
					const char *string);
int32_t cellDbgFontPrintf(
					const float x, 
					const float y, 
					const float scale, 
					const uint32_t color, 
					const char *string, ...);
int32_t cellDbgFontVprintf(
					const float x, 
					const float y, 
					const float scale, 
					const uint32_t color,
					const char *string,
					const va_list argp);

/* Console Type Functions */
CellDbgFontConsoleId cellDbgFontConsoleOpen(const CellDbgFontConsoleConfig *ccfg);
int cellDbgFontConsoleClose(const CellDbgFontConsoleId id);
int cellDbgFontConsoleClear(const CellDbgFontConsoleId id);
int32_t cellDbgFontConsolePuts(const CellDbgFontConsoleId id, const char *string);
int32_t cellDbgFontConsolePrintf(const CellDbgFontConsoleId id, const char *string, ...);
int32_t cellDbgFontConsoleVprintf(const CellDbgFontConsoleId id, const char *string, va_list argp);
int cellDbgFontConsoleEnable(const CellDbgFontConsoleId id);
int cellDbgFontConsoleDisable(const CellDbgFontConsoleId id);


#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif /* __CELL_DBGFONT_H__ */
