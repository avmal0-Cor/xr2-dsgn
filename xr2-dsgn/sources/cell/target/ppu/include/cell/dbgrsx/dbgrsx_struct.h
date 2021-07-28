/*   SCE CONFIDENTIAL                                       */
/*   PlayStation(R)3 Programmer Tool Runtime Library 310.001 */
/*   Copyright (C) 2006 Sony Computer Entertainment Inc.    */
/*   All Rights Reserved.                                   */

#ifndef __CELL_DBG_RSX_STRUCT_H__
#define __CELL_DBG_RSX_STRUCT_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


	/*	Structures	*/
	typedef struct CellDbgRsxTileRegion{
		uint32_t	enable;		// binding enable bit
		uint32_t	bank;		// memory bank sense

		uint32_t	start;		// start address
		uint32_t	limit;		// limit address
		uint32_t	pitch;		// pitch size

		uint32_t	base_tag;	// compression tag base address
		uint32_t	limit_tag;	// compression tag limit address

		uint32_t	compmode;	// compression mode
	} __attribute__ ((aligned(4))) CellDbgRsxTileRegion;

	typedef struct CellDbgRsxZcullRegion{
		uint32_t	enable;		// binding enable bit
		uint32_t	format;		// depth format
		uint32_t	antialias;	// antialiasing format

		uint32_t	width;		// width size
		uint32_t	height;		// height size

		uint32_t	start;		// cullram address
		uint32_t	offset;		// depth buffer address
	} __attribute__ ((aligned(4))) CellDbgRsxZcullRegion;

	typedef struct CellDbgRsxZcullStatus{
		uint32_t	valid;		// valid bit
		uint32_t	zdir;		// zdir format
		uint32_t	zformat;
		uint32_t	sfunc;
		uint32_t	sref;
		uint32_t	smask;

		uint16_t	pushbacklimit;
		uint16_t	moveforwardlimit;
	} __attribute__ ((aligned(4))) CellDbgRsxZcullStatus;

	typedef struct CellDbgRsxZcullMatch{
		uint32_t		valid;		// valid bit
		uint32_t		region;		// index of matching zcull region
	} __attribute__ ((aligned(4))) CellDbgRsxZcullMatch;

	typedef struct CellDbgRsxCursor {
		uint32_t bitmap[128];
		uint16_t palette[16][4];
		uint32_t magnification_factor;
	} __attribute__ ((aligned(4))) CellDbgRsxCursor;

	// Fifo Cache Struct
	typedef struct CellDbgRsxFifoCache{
		uint16_t	type;			// method type
		uint16_t	addr;			// method address
		uint32_t	data;			// method data
	} __attribute__ ((aligned(4))) CellDbgRsxFifoCache;


	// Graphics Fifo Struct
	typedef struct CellDbgRsxGraphicsFifo{
		uint16_t	type;			// method type
		uint16_t	addr;			// method address
		uint32_t	data1;			// method data low
		uint32_t	data2;			// method data high
	} __attribute__ ((aligned(4))) CellDbgRsxGraphicsFifo;


	// union Error State
	typedef union CellDbgRsxErrorState{
		struct {
			uint32_t	value[4];
		}ui32;

		struct {
			uint16_t	value[8];
		}ui16;
		
		struct {
			uint8_t		value[16];
		}ui8;
	} __attribute__ ((aligned(4))) CellDbgRsxErrorState;


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif	/* __CELL_DBG_RSX_STRUCT_H__ */
