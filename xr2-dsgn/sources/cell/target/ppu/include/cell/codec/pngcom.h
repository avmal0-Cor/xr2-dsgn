/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2008 Sony Computer Entertainment Inc.
 * All Rights Reserved.
 */
#ifndef CELL_PNG_H
#define CELL_PNG_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif /*E __cplusplus */

/*E Ancillary Chunk : Other type definitions */
typedef enum{
    CELL_PNG_TEXT    = 0,        
	CELL_PNG_ZTXT    = 1,    
	CELL_PNG_ITXT    = 2    
}CellPngTxtType;

typedef struct CellPngPLTEentry{
	uint8_t red;
	uint8_t green;
	uint8_t blue;
} CellPngPLTEentry;

typedef struct CellPngPaletteEntries
{
   uint16_t red;
   uint16_t green;
   uint16_t blue;
   uint16_t alpha;
   uint16_t frequency;
} CellPngPaletteEntries;

typedef struct CellPngSPLTentry
{
   char                      *paletteName;
   uint8_t                   sampleDepth;
   CellPngPaletteEntries     *paletteEntries;
   uint32_t                  paletteEntriesNumber;
} CellPngSPLTentry;

typedef enum{
    CELL_PNG_BEFORE_PLTE    = 1,        
	CELL_PNG_BEFORE_IDAT    = 2,    
	CELL_PNG_AFTER_IDAT     = 8    
} CellPngUnknownLocation;


/*E Ancillary Chunk : Structures */
typedef struct CellPngTextInfo {
	CellPngTxtType        txtType;
	char                  *keyword;
	char                  *text;
	uint32_t              textLength;
	char                  *languageTag;
	char                  *translatedKeyword;
} CellPngTextInfo;

typedef struct CellPngPLTE{
	uint32_t            paletteEntriesNumber;
	CellPngPLTEentry    *paletteEntries;
} CellPngPLTE;

typedef struct CellPngGAMA{
	double gamma;
} CellPngGAMA;

typedef struct CellPngSRGB{
	uint32_t renderingIntent;
} CellPngSRGB;

typedef struct CellPngICCP{
	char     *profileName;        
	char     *profile;            
	uint32_t profileLength;      
} CellPngICCP;

typedef struct CellPngSBIT
{
	uint32_t red;  
	uint32_t green;
	uint32_t blue;
	uint32_t gray; 
	uint32_t alpha;
} CellPngSBIT;

typedef struct CellPngTRNS
{
	char     *alphaForPaletteIndex;
	uint32_t alphaForPaletteIndexNumber;
	uint16_t red;  
	uint16_t green;
	uint16_t blue;
	uint16_t gray; 
} CellPngTRNS;

typedef struct CellPngHIST
{
	uint16_t   *histgramEntries;
	uint32_t   histgramEntriesNumber;
} CellPngHIST;

typedef struct CellPngTIME
{
   uint16_t year;
   uint8_t  month;
   uint8_t  day;  
   uint8_t  hour; 
   uint8_t  minute;
   uint8_t  second;
} CellPngTIME;

typedef struct CellPngBKGD
{
	uint8_t  paletteIndex;
	uint32_t red;   
	uint32_t green;
	uint32_t blue;
	uint32_t gray;  
} CellPngBKGD;

typedef struct CellPngSPLT
{
   CellPngSPLTentry     *sPLTentries;
   uint32_t             sPLTentriesNumber;
} CellPngSPLT;

typedef struct CellPngOFFS
{
	int32_t xPosition; 
	int32_t yPosition;
	uint32_t unitSpecifier;
} CellPngOFFS;

typedef struct CellPngPHYS
{
	uint32_t xAxis;
	uint32_t yAxis;
	uint32_t unitSpecifier;
} CellPngPHYS;

typedef struct CellPngSCAL
{
	uint32_t unitSpecifier;
	double   pixelWidth;
	double   pixelHeight;
} CellPngSCAL;

typedef struct CellPngCHRM
{
	double whitePointX;
	double whitePointY;
	double redX;
	double redY;
	double greenX;
	double greenY;
	double blueX;
	double blueY;
}CellPngCHRM;

typedef struct CellPngPCAL
{
	char     *calibrationName;
	int32_t  x0;
	int32_t  x1;	
	uint32_t equationType;
	uint32_t numberOfParameters;
	char     *unitName;
	char     **parameter;
}CellPngPCAL;

typedef struct CellPngUnknownChunk
{
    char                      chunkType[5];
    char                      *chunkData;
    uint32_t                  length;
	CellPngUnknownLocation    location;
}CellPngUnknownChunk;

#ifdef __cplusplus
}
#endif /*E __cplusplus */

#endif /*E CELL_PNG_H */
