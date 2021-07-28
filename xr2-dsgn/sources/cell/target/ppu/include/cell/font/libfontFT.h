/*   SCE CONFIDENTIAL                                        */
/* PlayStation(R)3 Programmer Tool Runtime Library 310.001 */
/*   Copyright (C) 2007 Sony Computer Entertainment Inc.     */
/*   All Rights Reserved.                                    */
#ifndef __LIBFONT_FT_H__
#define __LIBFONT_FT_H__

#include <types.h>
#include <cellstatus.h>
#include <cell/error.h>

#include <cell/font/error.h>
#include <cell/font/ps3fontset.h>
#include <cell/font/libfont.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define CELL_FONT_LIBRARY_TYPE_FREETYPE  (2)

typedef struct CellFontLibraryConfigFT
{
	void* library;
	CellFontMemoryInterface MemoryIF;
	
} CellFontLibraryConfigFT;

typedef struct CellFontRendererConfigFT
{
	struct {
		void*        buffer;
		uint32_t   initSize;
		uint32_t    maxSize;
		uint32_t expandSize;
		uint32_t  resetSize;
	} BufferingPolicy;
	
} CellFontRendererConfigFT;


static inline void CellFontLibraryConfigFT_initialize( CellFontLibraryConfigFT* config )
{
	config->library = (void*)0;
	CellFontMemoryInterface_initialize( &config->MemoryIF );
	return;
}

static inline int cellFontInitLibraryFreeType( CellFontLibraryConfigFT* config, const CellFontLibrary**lib )
{
	extern void cellFontFTGetStubRevisionFlags( uint64_t*revisionFlags );
	extern int cellFontInitLibraryFreeTypeWithRevision( uint64_t,
	                                                    CellFontLibraryConfigFT*,
	                                                    const CellFontLibrary**lib );
	uint64_t revisionFlags = 0LL;
	
	cellFontFTGetStubRevisionFlags( &revisionFlags );
	return cellFontInitLibraryFreeTypeWithRevision( revisionFlags, config, lib );
}

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __LIBFONT_FT_H__ */


