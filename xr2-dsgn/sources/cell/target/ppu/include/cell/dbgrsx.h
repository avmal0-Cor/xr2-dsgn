/*   SCE CONFIDENTIAL                                       */
/*   PlayStation(R)3 Programmer Tool Runtime Library 310.001 */
/*   Copyright (C) 2006 Sony Computer Entertainment Inc.    */
/*   All Rights Reserved.                                   */

#ifndef __CELL_DBG_RSX_H__
#define __CELL_DBG_RSX_H__

#include <stdio.h>
#include <stdint.h>
#include <cell/error.h>
#include <cell/dbgrsx/dbgrsx_enum.h>
#include <cell/dbgrsx/dbgrsx_struct.h>
#include <cell/dbgrsx/dbgrsx_error.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

	/*	Prototypes	*/
	int32_t	cellDbgRsxInit(void);
	int32_t cellDbgRsxExit(void);


	int32_t cellDbgRsxGetTileRegion(uint8_t index, CellDbgRsxTileRegion *region);
	int32_t cellDbgRsxGetZcullRegion(uint8_t index, CellDbgRsxZcullRegion *zcregion);
	int32_t cellDbgRsxGetZcullStatus(uint8_t index, CellDbgRsxZcullStatus *zcstatus);
	int32_t cellDbgRsxGetZcullMatch(CellDbgRsxZcullMatch *zcmatch);

	uint32_t cellDbgRsxGetZcullNearRamSize(uint32_t pixel_width, uint32_t pixel_height);
	uint32_t cellDbgRsxGetZcullFarRamSize(uint32_t pixel_width, uint32_t pixel_height);
	uint32_t cellDbgRsxGetZcullStencilRamSize(uint32_t pixel_width, uint32_t pixel_height, uint32_t antialias);

	int32_t cellDbgRsxGetZcullNearRamTextureSize(uint32_t pixel_width, uint32_t pixel_height , uint32_t *tex_width , uint32_t *tex_height , uint32_t *tex_format);
	int32_t cellDbgRsxGetZcullFarRamTextureSize(uint32_t pixel_width, uint32_t pixel_height , uint32_t *tex_width , uint32_t *tex_height , uint32_t *tex_format);
	int32_t cellDbgRsxGetZcullStencilRamTextureSize(uint32_t pixel_width, uint32_t pixel_height , uint32_t antialias , uint32_t *tex_width , uint32_t *tex_height , uint32_t *tex_format);

	int32_t cellDbgRsxGetZcullNearRam(uint16_t *data, uint32_t pixel_width, uint32_t pixel_height, uint32_t zcstart, uint32_t zdir, uint32_t zformat);
	int32_t cellDbgRsxGetZcullFarRam(uint16_t *data, uint32_t pixel_width, uint32_t pixel_height, uint32_t zcstart, uint32_t zdir, uint32_t zformat);
	int32_t cellDbgRsxGetZcullStencilRam(uint8_t *data, uint32_t pixel_width, uint32_t pixel_height, uint32_t zcstart, uint32_t antialias);

	int32_t cellDbgRsxEnableCursor(const CellDbgRsxCursor *cursor, const uint32_t x, const uint32_t y);
	int32_t cellDbgRsxDisableCursor(void);
	int32_t cellDbgRsxSetCursorPosition(const uint32_t x, const uint32_t y);
	int32_t cellDbgRsxGetCursorStatus(int32_t *status);

	int32_t cellDbgRsxSetCursorDefaultValues(CellDbgRsxCursor *cursor);
	int32_t cellDbgRsxSetCursorBitmap(CellDbgRsxCursor *cursor, const uint8_t *source_bitmap);
	int32_t cellDbgRsxSetCursorPalette(CellDbgRsxCursor *cursor, const uint8_t *source_palette);
	int32_t cellDbgRsxSetCursorMagnificationFactor(CellDbgRsxCursor *cursor, const uint32_t magnification_factor);

	int32_t cellDbgRsxGetInterruptErrorStatus(uint32_t *status);
	int32_t cellDbgRsxGetInterruptFifoErrorStatus(uint32_t *status);
	int32_t cellDbgRsxGetInterruptIoifErrorStatus(uint32_t *status);
	int32_t cellDbgRsxGetInterruptGraphicsErrorStatus(uint64_t *status);
	int32_t cellDbgRsxGetInterruptMiscErrorStatus(uint32_t *status);

	int32_t cellDbgRsxGetFifoCacheIndex(CellDbgRsxFifoCache *cache, int32_t index);
	int32_t cellDbgRsxGetFifoCache(CellDbgRsxFifoCache *cache);
	int32_t cellDbgRsxGetGraphicsFifo(CellDbgRsxGraphicsFifo *fifo);

	int32_t cellDbgRsxGetFifoErrorState(int32_t id, CellDbgRsxErrorState *state);
	int32_t cellDbgRsxGetIoifErrorState(int32_t id, CellDbgRsxErrorState *state);
	int32_t cellDbgRsxGetGraphicsErrorState(int32_t id, CellDbgRsxErrorState *state);

	int32_t cellDbgRsxGetGraphicsBundleState(int32_t id, uint32_t *data);


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif	/* __CELL_DBG_RSX_H__ */
