/*   SCE CONFIDENTIAL                                        */
/*   PlayStation(R)3 Programmer Tool Runtime Library 310.001 */
/*   Copyright (C) 2009 Sony Computer Entertainment Inc.     */
/*   All Rights Reserved.                                    */
#ifndef __LIBFONT_GCM_H__
#define __LIBFONT_GCM_H__

#include <cell/font.h>
#include <cell/gcm.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef struct CellFontInitGraphicsConfigGcm {
	uint32_t configType;
	
	struct {
		void* address;
		uint32_t size;
	
	} GraphicsMemory;
	
	struct {
		void* address;
		uint32_t size;
	
	} MappedMainMemory;
	
	struct {
		int16_t slotNumber;
		int16_t slotCount;
	
	} VertexShader;
	
} CellFontInitGraphicsConfigGcm;

#define CELL_FONT_INIT_GRAPHICS_CONFIG_TYPE_DEFAULT (0)
#define CELL_FONT_VERTEX_SHADER_SLOT_COUNT_DEFAULT (63)

static inline void CellFontInitGraphicsConfigGcm_initialize( CellFontInitGraphicsConfigGcm* configGcm )
{
	configGcm->configType = CELL_FONT_INIT_GRAPHICS_CONFIG_TYPE_DEFAULT;
	configGcm->GraphicsMemory.address  = (void*)0;
	configGcm->GraphicsMemory.size     = 0;
	configGcm->MappedMainMemory.address = (void*)0;
	configGcm->MappedMainMemory.size    = 0;
	configGcm->VertexShader.slotNumber = 0;
	configGcm->VertexShader.slotCount  = CELL_FONT_VERTEX_SHADER_SLOT_COUNT_DEFAULT;
}

typedef struct CellFontRenderSurfaceGcm
{
	CellFontRenderSurface  RenderSurface;
	void* SystemClosed[ 96 - sizeof(CellFontRenderSurface)/sizeof(void*) ];

} CellFontRenderSurfaceGcm;

#define CELL_FONT_GRAPHICS_TYPE_GCM  (1)

int cellFontInitGraphicsGcm( CellFontInitGraphicsConfigGcm*, const CellFontGraphics** );

int cellFontGraphicsSetGcmPacketMode( CellFontGraphicsDrawContext*, uint32_t packetMode );
int cellFontGraphicsGetGcmPacketMode( CellFontGraphicsDrawContext*, uint32_t*packetMode );
#define CELL_FONT_GRAPHICS_GCM_PACKET_WRITE_DISABLE (0x00008000)
#define CELL_FONT_GRAPHICS_GCM_PACKET_WRITE_ENABLE  (0x80000000)
#define CELL_FONT_GRAPHICS_GCM_PACKET_CALL_DISABLE  (0x00000001)
#define CELL_FONT_GRAPHICS_GCM_PACKET_CALL_ENABLE   (0x00010000)

int  cellFontRenderSurfaceGcmInit( CellFontRenderSurfaceGcm*, CellGcmSurface* );
void cellFontRenderSurfaceGcmSetScissor( CellFontRenderSurfaceGcm*,
                                         int x0, int y0, uint32_t w, uint32_t h );

int cellFontGraphicsGcmSetClearSurface( CellGcmContextData* p,
                                        CellFontRenderSurfaceGcm* renderSurfGcm,
                                        uint32_t clearEnable,
                                        CellFontGraphicsDrawContext* gdc );

int cellFontGraphicsGcmSetDrawGlyph( CellGcmContextData* p,
                             CellFontRenderSurfaceGcm* surf, float sx, float sy,
                             CellFontVertexesGlyph *outv, CellFontGraphicsDrawContext* fontGC );

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __LIBFONT_GCM_H__ */

