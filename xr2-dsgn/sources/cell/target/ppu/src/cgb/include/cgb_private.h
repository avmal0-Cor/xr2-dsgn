/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 *                Copyright (C) 2009 Sony Computer Entertainment Inc.
 *                                               All Rights Reserved.
 */

#ifndef __CELL_CGB_PRIVATE_H__
#define __CELL_CGB_PRIVATE_H__

#include "format/cgb_format.h"
#include <stddef.h>

#define MAX_PARAMETERNAME_LENGTH 256

// types definition

// This is the internal definition of the CellCgbProgram.
// It is defined externally in cgb.h as an anonymous block of data
// with a predefined size that MUST match the actual size here.
// If this is ever updated, CELL_CGB_PROGRAM_STRUCTURE_SIZE must be as well.

typedef struct CellCgbProgram
{
	const CellCgbHeader *header;

	const char *ucode;

	size_t constantCount;
	const unsigned short *constantResources;
	const float *constantValues;

	size_t mapEntryCount;
	const CellCgbMapEntry *mapEntries;
	size_t fpOffsetCount;
	const unsigned short *fpOffsets;
	const char *stringTable;

	unsigned int uniformCount;

        const CellCgbParameterInfo* parameterInfos;

} CellCgbProgram;

//#if defined(WIN32) || defined(LINUX)
#if !defined(__CELLOS_LV2__)
#include <cgb.h>
#else
#include <cell/cgb.h>
#endif

#endif // #ifndef __CELL_CGB_PRIVATE_H__

