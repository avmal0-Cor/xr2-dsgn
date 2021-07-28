/*   SCE CONFIDENTIAL                                       */
/*   PlayStation(R)3 Programmer Tool Runtime Library 310.001 */
/*   Copyright (C) 2009 Sony Computer Entertainment Inc.    */
/*   All Rights Reserved.                                   */

#ifndef __CELL_CGB_H__
#define __CELL_CGB_H__

#if defined(WIN32) || defined(LINUX) || defined(__linux__)
typedef int int32_t;
typedef unsigned int uint32_t;
typedef unsigned short uint16_t;
typedef unsigned char uint8_t;

#include "cgb/cgb_struct.h"
#else
#include <cell/cgb/cgb_struct.h>
#endif

#define CELL_CGB_OK 0
#define CELL_CGB_ERROR_FAILED 0xffffffff
#define CELL_CGB_PROGRAM_STRUCTURE_SIZE 48

typedef enum CellCgbProfile
{
	CellCgbVertexProfile,
	CellCgbFragmentProfile
} CellCgbProfile;

#ifndef __CELL_CGB_PRIVATE_H__

typedef struct CellCgbProgram
{
	char data[CELL_CGB_PROGRAM_STRUCTURE_SIZE];
} CellCgbProgram;

#endif


#ifdef __cplusplus
extern "C" {
#endif
	
//program
uint32_t cellCgbGetSize(const void *binary);
int32_t cellCgbRead(const void *binary, const uint32_t size, CellCgbProgram *program);
CellCgbProfile cellCgbGetProfile(const CellCgbProgram *program);

//hardware configurations
int32_t cellCgbGetVertexConfiguration(const CellCgbProgram *program, CellCgbVertexProgramConfiguration *conf);
int32_t cellCgbGetFragmentConfiguration(const CellCgbProgram *program, CellCgbFragmentProgramConfiguration *conf);

//attribute output mask
int32_t cellCgbGetVertexAttributeOutputMask(const CellCgbProgram *program, uint32_t *attributeOutputMask);
//user clip plane control mask
int32_t cellCgbGetUserClipPlaneControlMask(const CellCgbProgram *program, uint32_t *userClipPlaneControlMask);

// typo fix for so that older samples will still build and run 
#define cellCgbGetVertexAttributOutputMask cellCgbGetVertexAttributeOutputMask

//ucode
uint32_t cellCgbGetUCodeSize(const CellCgbProgram *program);
const void* cellCgbGetUCode(const CellCgbProgram *program);

//vertex program constants, internals and defaults
uint32_t cellCgbGetVertexConstantCount(const CellCgbProgram *program);
//vertex constant values are returned in big endian
void cellCgbGetVertexConstantValues(const CellCgbProgram *program, uint32_t value_index, uint16_t *reg, const float **value);

//resource map table: provides lookups between parameter names and hardware resources
uint32_t cellCgbMapLookup(CellCgbProgram *program, const char *name);
uint16_t cellCgbMapGetValue(CellCgbProgram *program, const uint32_t map_index);
uint32_t cellCgbMapGetLength(const CellCgbProgram *program);
void cellCgbMapGetName(CellCgbProgram *program, const uint32_t map_index, char *name, uint32_t *size);

//constants
//default values are returned padded on 4 floats ( even for matrices ) and in big endian
void cellCgbMapGetVertexUniformRegister(const CellCgbProgram *program, const uint32_t map_index, uint16_t *reg, const float **default_values);
void cellCgbMapGetFragmentUniformOffsets(const CellCgbProgram *program, const uint32_t map_index, uint16_t *offsets, uint32_t *count);
void cellCgbMapGetFragmentUniformRegister(const CellCgbProgram *program, const uint32_t map_index, uint16_t *reg);

//helpers
//the lookup table is unable to perform error checking
static inline uint32_t cellCgbMapLookupTextureUnit(CellCgbProgram *program, const char *name) 
{
	uint32_t res = cellCgbMapLookup(program,name);
	return  res != CELL_CGB_ERROR_FAILED ? cellCgbMapGetValue(program,res) : res;
}
static inline uint32_t cellCgbMapLookupAttribute(CellCgbProgram *program, const char *name) 
{
	uint32_t res = cellCgbMapLookup(program,name);
	return  res != CELL_CGB_ERROR_FAILED ? cellCgbMapGetValue(program,res) : res;
}

#ifdef __cplusplus
}
#endif

#endif
