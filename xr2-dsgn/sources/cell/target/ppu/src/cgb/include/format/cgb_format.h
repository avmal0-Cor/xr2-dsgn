/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 *                Copyright (C) 2009 Sony Computer Entertainment Inc.
 *                                               All Rights Reserved.
 */


#ifndef CGB_FORMAT_HEADER
#define CGB_FORMAT_HEADER

// ----------------------------------------------
// This header defines the CGB file format
// the format consists in the concatenation of the following structures:
//
// CellCgbHeader header;
// CellCgbVertexConfigurationInternal or CellCgbFragmentConfigurationInternal depending on header->profile
// char ucode[ucode_size];
// [block level A] see below
// [block level B] see below
// [block level C] see below

#define CELL_CGB_CONSTANT_TABLE 1
#define CELL_CGB_LOOKUP_TABLE 2
#define CELL_CGB_PARAMETER_INFO 4

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;

#if defined(SPU)
#define __PACKED__ __attribute__((packed)) 
#else
#define __PACKED__
#endif

//3 int = 12 bytes
typedef struct __PACKED__ CellCgbHeader
{
		 unsigned int fourcc; //should be 'C' 'G' 'B' '\0'
		 unsigned short format_version; //0 for now
		 unsigned short compiler_version;
		 unsigned short ucode_size; //size of the ucode in bytes
		 unsigned char profile; //CellCgbVertexProfile (0) or CellCgbFragmentProfile (1) as defined in cgb.h
		 unsigned char content; // combination of CELL_CGB_CONSTANT_TABLE,CELL_CGB_LOOKUP_TABLE and CELL_CGB_PARAMETER_TREE
} CellCgbHeader;

// The CellCgbVertexConfigurationInternal and CellCgbFragmentConfigurationInternal both get padded out to be 20 bytes long
// during the conversion process.  This is so the total of the size of CellCgbHeader and the config are 32 bytes long.
// This is neccessary so that the ucode section which follows is aligned on 16 bytes.

// 16 bytes
typedef struct __PACKED__ CellCgbVertexConfigurationInternal
{
		 uint16_t attributeInputMask;
		 uint8_t		 registerCount;
		 uint8_t		 unused0;
		 uint32_t attributeOutputMask;
		 uint32_t userClipMask; // 4bit x 6 = 24 bit
		 uint16_t instructionSlot;
		 uint16_t unused1;
} CellCgbVertexConfigurationInternal;

// 20 bytes
typedef struct __PACKED__ CellCgbFragmentConfigurationInternal
{
		 uint32_t attributeInputMask;
		 uint16_t texCoordsInputMask;
		 uint16_t texCoords2D;
		 uint16_t texCoordsCentroid;
		 uint16_t unused0;
		 uint32_t fragmentControl;
		 uint8_t registerCount;
		 uint8_t unused1;
		 uint16_t unused2;
} CellCgbFragmentConfigurationInternal;

// ----------------------------------------------
// A constant data

typedef struct __PACKED__ CellCgbLevelAHeader
{
		 unsigned short block_size;
		 unsigned short constant_count;
} CellCgbLevelAHeader;

//data ( start and size aligned on 16 bytes )
/*
		 CellCgbLevelAHeader header;
		 fp only: unsigned short constant_resource_indices[]; // header.default_count; (1 short = 2 bytes )
		 unsigned short constant_resources[]; // header.block_size - header.constant_count*( 9 shorts ) aligned on 32 bits
		 float4 constant_values[]; // header.default_count; (16 bytes aligned) (8 short = 16 bytes) ( offset is block_size - constant_count*16 bytes )
*/


// ----------------------------------------------
// B name lookup table

typedef struct __PACKED__ CellCgbMapEntry
{
		 unsigned int name_offset;
		 unsigned short parent;
		 unsigned short resource;
} CellCgbMapEntry;

typedef struct __PACKED__ CellCgbLevelBHeader
{
		 unsigned short block_size;
		 unsigned short entry_count;
		 unsigned short fp_offset_count;
} CellCgbLevelBHeader;

//data ( start and size aligned on 4 bytes )
/*
CellCgbLevelBHeader header;
CellCgbMapEntry entries[]; // header.entry_count
fp only: unsigned short constant_resources[]; // header.block_size - header.constant_count*( 9 shorts )
*/


// ----------------------------------------------
// C cg parameter tree

typedef struct __PACKED__ CellCgbParameterInfo
{
		 unsigned short cg_type;
		 unsigned short cg_resource;
		 unsigned short flags;
		 unsigned short value_count;
} CellCgbParameterInfo;

typedef struct __PACKED__ CellCgbLevelCHeader
{
		 unsigned short block_size;
		 unsigned short info_count;
} CellCgbLevelCHeader;

//data ( start and size aligned on 4 bytes )
/*
CellCgbLevelCHeader header;
CellCgbParameterInfo infos[];
*/

// shader configuration

//CgParameterEntry flags

//variability
#define CGPV_MASK 0x03
#define CGPV_VARYING 0x00
#define CGPV_UNIFORM 0x01
#define CGPV_CONSTANT 0x02
#define CGPV_MIXED 0x03

		 //direction
#define CGPD_MASK 0x0C
#define CGPD_IN 0x00
#define CGPD_OUT 0x04
#define CGPD_INOUT 0x08

typedef struct __PACKED__ CellCgeShaderConfiguration
{
		 //28 bytes
		 unsigned short profile; // Vertex / Fragment
		 unsigned short compilerVersion;
		 unsigned int instructionCount;
		 unsigned int attributeInputMask;
		 union
		 {
		 		 struct
		 		 {
		 		 		 //16 bytes
		 		 		 unsigned int instructionSlot;
		 		 		 unsigned int registerCount;
		 		 		 unsigned int attributeOutputMask;
		 		 		 unsigned int userClipMask;
		 		 } vertexProgram;
		 		 struct
		 		 {
		 		 		 //12 bytes
		 		 		 unsigned int partialTexType;
		 		 		 unsigned short texcoordInputMask;
		 		 		 unsigned short texcoord2d;
		 		 		 unsigned short texcoordCentroid;
		 		 		 unsigned char registerCount;
		 		 		 unsigned char flags; //combination of CGF_OUTPUTFROMH0,CGF_DEPTHREPLACE,CGF_PIXELKILL
		 		 } fragmentProgram;
		 };
} CellCgeShaderConfiguration;

#endif //CGB_FORMAT_HEADER

