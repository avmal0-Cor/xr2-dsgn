/*   SCE CONFIDENTIAL                                       */
/*   PlayStation(R)3 Programmer Tool Runtime Library 310.001 */
/*   Copyright (C) 2006 Sony Computer Entertainment Inc.    */
/*   All Rights Reserved.                                   */

#ifndef __CELL_GCM_STRUCT_H__
#define __CELL_GCM_STRUCT_H__

// cgb_struct.h includes type definition of 
// CellCgbVertexProgramConfiguration and 
// CellCgbFragmentProgramConfiguration
//
#include <cell/cgb/cgb_struct.h>
#include <Cg/cgBinary.h>

typedef struct CellGcmControl{
	volatile uint32_t put;
	volatile uint32_t get;
	volatile uint32_t ref;
}CellGcmControl;

typedef struct CellGcmConfig{
	void		*localAddress;
	void		*ioAddress;
	uint32_t	localSize;
	uint32_t	ioSize;
	uint32_t	memoryFrequency;
	uint32_t	coreFrequency;
}CellGcmConfig;

typedef struct CellGcmSurface{
	uint8_t		type;
	uint8_t		antialias;

	uint8_t		colorFormat;
	uint8_t		colorTarget;
	uint8_t		colorLocation[CELL_GCM_MRT_MAXCOUNT];
	uint32_t	colorOffset[CELL_GCM_MRT_MAXCOUNT];
	uint32_t	colorPitch[CELL_GCM_MRT_MAXCOUNT];

	uint8_t		depthFormat;
	uint8_t		depthLocation;
	uint8_t		_padding[2];
	uint32_t	depthOffset;
	uint32_t	depthPitch;

	uint16_t	width;
	uint16_t	height;
	uint16_t	x;
	uint16_t	y;
}CellGcmSurface;

typedef struct CellGcmTexture{
	uint8_t		format;
	uint8_t		mipmap;
	uint8_t		dimension;
	uint8_t		cubemap;

	uint32_t	remap;

	uint16_t	width;
	uint16_t	height;
	uint16_t	depth;
	uint8_t		location;
	uint8_t		_padding;

	uint32_t	pitch;
	uint32_t	offset;
}CellGcmTexture;

struct CellGcmContextData;
typedef int32_t (*CellGcmContextCallback)(struct CellGcmContextData *, uint32_t);

typedef struct CellGcmContextData{
	uint32_t *__restrict begin;
	uint32_t *__restrict end;
	uint32_t *__restrict current;
	CellGcmContextCallback callback;
}CellGcmContextData;

typedef struct CellGcmOffsetTable{
	uint16_t	*ioAddress;	// ea -> io
	uint16_t	*eaAddress;	// io -> ea
}CellGcmOffsetTable;

typedef union CellGcmCast{
	uint32_t	u;
	float		f;
}CellGcmCast;

typedef struct CellGcmReportData{
	uint64_t	timer;
	uint32_t	value;
	uint32_t	zero;
}CellGcmReportData;

typedef struct CellGcmNotifyData{
	uint64_t	timer;
	uint64_t	zero;
}CellGcmNotifyData;

typedef struct CellGcmTransferScale{
	uint32_t 	conversion;
	uint32_t 	format;
	uint32_t 	operation;
	int16_t 	clipX;
	int16_t 	clipY;
	uint16_t 	clipW;
	uint16_t 	clipH;
	int16_t 	outX;
	int16_t 	outY;
	uint16_t 	outW;
	uint16_t 	outH;
	int32_t 	ratioX;
	int32_t 	ratioY;

	uint16_t 	inW;
	uint16_t 	inH;
	uint16_t 	pitch;
	uint8_t 	origin;
	uint8_t 	interp;
	uint32_t 	offset;
	uint16_t 	inX;
	uint16_t 	inY;
}CellGcmTransferScale;

typedef struct CellGcmTransferSurface{
	uint32_t 	format;
	uint16_t 	pitch;
	uint16_t	_padding;
	uint32_t	offset;
}CellGcmTransferSurface;

typedef struct CellGcmTransferSwizzle{
	uint16_t	format;
	uint8_t		width;
	uint8_t		height;
	uint32_t	offset;
}CellGcmTransferSwizzle;

typedef struct CellGcmTileInfo{
	uint32_t	tile;
	uint32_t	limit;
	uint32_t	pitch;
	uint32_t	format;
}CellGcmTileInfo;

typedef struct CellGcmZcullInfo{
	uint32_t	region;
	uint32_t	size;
	uint32_t	start;
	uint32_t	offset;
	uint32_t	status0;
	uint32_t	status1;
}CellGcmZcullInfo;

typedef struct CellGcmDisplayInfo{
	uint32_t	offset;
	uint32_t	pitch;
	uint32_t	width;
	uint32_t	height;
}CellGcmDisplayInfo;

#endif /* __CELL_GCM_STRUCT_H__ */
