/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 *                Copyright (C) 2009 Sony Computer Entertainment Inc.
 *                                               All Rights Reserved.
 */

#include <cgb_private.h>
#include <string.h>
#include <stdio.h>

//documentation
/** @file */

/** @enum CellCgbProfile

@short Profile of a shader program

This enum indicates whether the Cgb shader program is a vertex program or a fragment program

@param CellCgbVertexProfile Vertex program profile
@param CellCgbFragmentProfile Fragment program profile

*/

/** @struct CellCgbProgram

@short Cgb program handle

This structure is used as a handle to a shader binary for the libcgb functions.
It stores indices into a Cgb program binary for fast lookup.

@param data Reserved member

@sa cellCgbRead

*/

/** @struct CellCgbVertexProgramConfiguration

@short Vertex program configuration structure

This structure defines the configuration of the vertex engine, and must be passed at runtime to
the graphics API function.

@param instructionSlot The instruction slot to use to load the vertex program into the instruction registers
@param instructionCount The number of instructions in the vertex program
@param attributeInputMask The attribute input mask
@param registerCount The number of registers used by this vertex program

@sa cellCgbGetVertexConfiguration

*/

/** @struct CellCgbFragmentProgramConfiguration

@short Fragment program configuration structure

This structure defines the configuration of the fragment engine, and must be passed at runtime to the
graphics API function.

@note Before passing this structure to a graphics API, <I><c>offset</c></I> needs to be set by the user to the
video offset where the microcode is stored.
The register count can be increased if needed to influence the distribution of the quads across the fragment pipes.
The other fields should not be modified.

@param offset Offset in video memory where the fragment program microcode is stored
@param attributeInputMask  Attribute input mask
@param texCoordsInputMask  Texcoord input mask
@param texCoords2D Texcoord 2D control
@param texCoordsCentroid Texcoord centroid control
@param fragmentControl Fragment control
@param registerCount Number of register needed by the fragment program

@sa cellCgbGetFragmentConfiguration

*/

// vertex and fragment configuration structures

//#if !defined(WIN32) || !defined(LINUX)
#if __CELLOS_LV2__
#define ENDSWAP(a)	a
#else

#define ENDSWAP(a) convert_endianness(a)
template< typename T > inline T convert_endianness( const T value)
{
	if ( sizeof( T ) == 1 )
	{
		return value;
	}
	if ( sizeof( T ) == 2 )
	{
		return ( ((value & 0x00FF) << 8)
			| ((value & 0xFF00) >> 8) );
	}
	if ( sizeof( T ) == 4 )
	{
		return ( ((value & 0x000000FF) << 24)
			| ((value & 0x0000FF00) << 8)
			| ((value & 0x00FF0000) >> 8)
			| ((value & 0xFF000000) >> 24) );
	}
	if ( sizeof( T ) == 8 )
	{
		T result = value;
		for ( int ii = 0; ii < 4; ++ii )
		{
			char ch = *( (( char* ) &result) + ii );
			*( (( char* ) &result) +      ii  ) = *( (( char* ) &result) + (7 - ii) );
			*( (( char* ) &result) + (7 - ii) ) = ch;
		}
		return result;
	}
	// exception
	return value;
}

#endif



#define PAD16(a) ((unsigned int)(a)+15 & ~(16-1))


/**

@short Reads a shader binary file

This function parses a shader binary file and fills a CellCgbProgram structure,
which is used as a handle by the libcgb functions.

@param binary A buffer containing the shader in the Cgb format

@param size The size of the shader, in bytes

@param program A pointer to a CellCgbProgram structure that receives the result of the function

@return Returns <c>CELL_CGB_OK</c> if successful, or <c>CELL_CGB_ERROR_FAILED</c> if <I><c>binary</c></I> does not contain
a valid Cgb file.

*/
int32_t cellCgbRead(const void *binary, const uint32_t size, CellCgbProgram *program)
{
	const CellCgbHeader *header = (const CellCgbHeader *)binary;
	program->header = header;

	if (header->profile != CellCgbVertexProfile &&
		header->profile != CellCgbFragmentProfile)
		return CELL_CGB_ERROR_FAILED;

	int offset = header->profile == CellCgbVertexProfile ?
		sizeof(CellCgbVertexConfigurationInternal) : sizeof(CellCgbFragmentConfigurationInternal);

	program->ucode = (const char *)binary + PAD16(sizeof(CellCgbHeader) + offset);

	//explore the format to fill the program structure

	const CellCgbLevelAHeader *headerA;
	if (ENDSWAP(program->header->content) & CELL_CGB_CONSTANT_TABLE){
	    //explore the format to fill the program structure
	    headerA = (const CellCgbLevelAHeader *)((const char*)program->ucode + ENDSWAP(header->ucode_size));
	    program->constantCount = ENDSWAP(headerA->constant_count);
	    program->constantResources = (const unsigned short*)(headerA + 1);
	    program->constantValues = (const float*)((const char*)binary + ( PAD16((const char*)(program->constantResources + program->constantCount) - (const char*)binary )));
	}
	else
	{
	    headerA = NULL;
	    program->constantCount = 0;
	    program->constantResources = NULL;
	    program->constantValues = NULL;
	}
	
	const CellCgbLevelBHeader *headerB;
	if(ENDSWAP(program->header->content) & CELL_CGB_LOOKUP_TABLE)
	{
	    if(headerA != NULL)
			headerB = (const CellCgbLevelBHeader *)((const char*)headerA + ENDSWAP(headerA->block_size));
	    else
			headerB = (const CellCgbLevelBHeader *)((const char*)program->ucode + ENDSWAP(header->ucode_size));

	    program->mapEntryCount = ENDSWAP(headerB->entry_count);
	    program->mapEntries = (const CellCgbMapEntry*)(headerB + 1);
	    program->fpOffsets = (const unsigned short*)(program->mapEntries + program->mapEntryCount);
	    program->stringTable = (const char*)(program->fpOffsets + ENDSWAP(headerB->fp_offset_count));
	}
	else
	{
	    headerB = NULL;
	    program->mapEntryCount = 0;
	    program->mapEntries = NULL;
	    program->fpOffsets = NULL;
	    program->stringTable = NULL;
	}

	if (ENDSWAP(program->header->content) & CELL_CGB_PARAMETER_INFO)
	{
		const CellCgbLevelCHeader *headerC = (const CellCgbLevelCHeader *)((const char*)headerB + ENDSWAP(headerB->block_size));
		program->parameterInfos = (const CellCgbParameterInfo*)((const char*)headerC + sizeof(CellCgbLevelCHeader));
	}
	else
	{
		program->parameterInfos = NULL;
	}

	return 0;
}

/**

@short Gets the profile of the shader.

This function returns the profile of the program.

@param program The Cgb program handle

@return Returns <c>CellCgbVertexProfile</c> if the shader is a vertex program, or <c>CellCgbFragmentProfile</c>
if the shader is a fragment program.

@sa cellCgbRead
*/
CellCgbProfile cellCgbGetProfile(const CellCgbProgram *program)
{
	return (CellCgbProfile)program->header->profile;
}

/**

@short Gets vertex engine configuration information.

This function retrieves the hardware configuration of the vertex engine needed to execute the shader.
The CellCgbVertexProgramConfiguration structure is filled as a hardware ready block of data, so
its fields have target endianness. The fields must be swapped if they are accessed for individual use
on the host.

@param program The Cgb program handle

@param conf A pointer to the vertex program configuration structure to be filled

@return Returns <c>CELL_CGB_OK</c> if successful, or <c>CELL_CGB_ERROR_FAILED</c> if the shader is not a vertex program.

@sa cellCgbRead

*/
int32_t cellCgbGetVertexConfiguration(const CellCgbProgram *program, CellCgbVertexProgramConfiguration *conf)
{
	if (program->header->profile != CellCgbVertexProfile)
		return CELL_CGB_ERROR_FAILED;
	const CellCgbVertexConfigurationInternal * vertexConf = (const CellCgbVertexConfigurationInternal *)(program->header+1);
	conf->instructionSlot = vertexConf->instructionSlot;
	conf->attributeInputMask = vertexConf->attributeInputMask;
	conf->instructionCount = ENDSWAP((uint16_t)(ENDSWAP(program->header->ucode_size) / 16));
	conf->registerCount = vertexConf->registerCount;
	return CELL_CGB_OK;
}

/**

@short Gets fragment engine configuration information.

This function retrieves the hardware configuration of the fragment engine needed to execute the shader.
The CellCgbFragmentProgramConfiguration structure is filled as a hardware ready block of data, so
its fields have target endianness. The fields must be swapped if they are accessed for individual use
on the host.

@param program The Cgb program handle

@param conf A pointer on a fragment program configuration structure to be filled

@return Returns <c>CELL_CGB_OK</c> if successful, or <c>CELL_CGB_ERROR_FAILED</c> if the shader is not a fragment program.

@sa cellCgbRead
*/
int32_t cellCgbGetFragmentConfiguration(const CellCgbProgram *program, CellCgbFragmentProgramConfiguration *conf)
{
	if (program->header->profile != CellCgbFragmentProfile)
		return CELL_CGB_ERROR_FAILED;

	const CellCgbFragmentConfigurationInternal * fragmentConf = (const CellCgbFragmentConfigurationInternal *)(program->header+1);
	conf->attributeInputMask = fragmentConf->attributeInputMask;
	conf->texCoordsInputMask = fragmentConf->texCoordsInputMask;
	conf->texCoords2D = fragmentConf->texCoords2D;
	conf->texCoordsCentroid = fragmentConf->texCoordsCentroid;
	conf->fragmentControl = fragmentConf->fragmentControl;
	conf->registerCount = fragmentConf->registerCount;
	return CELL_CGB_OK;
}

/**
@short Gets the vertex program <c><i>attributeOutputMask</i></c>.

Gets the vertex program <c><i>attributeOutputMask</i></c>, which can subsequently be set with 
libgcm by hand, or by PSGL (SDK 2.0.0 or later) with <c>psglSetUserClipPlanes()</c>.  
See the <i>PlayStation&reg; Graphics Library Reference</i> for further information
about <c>psglSetUserClipPlanes()</c>.

See the <c>cellGcmSetVertexAttribOutputMask()</c> function in the <i>libgcm Reference</i> for
a description of each bit in the <c><i>attributeOutputMask</i></c>.

sce-cgc only writes the vertex attributes that are actually used in the shader program as 
the output information. That is, only those attributes explicitly specified in OUT for a 
vertex shader program are output as the vertex attributes. 

For input attributes explicitly specified in IN for a fragment shader program, the 
runtime library (either PSGL or gcm) sets the vertex output attributes according to 
the fragment program input attributes.

The <c>cellCgbGetVertexAttributeOutputMask</c> function can be used, among other things, 
in conjunction with user-defined clip planes. When using user-defined clip planes, 
you must do the following:

<ol>
<li>Call the <c>cellGcmSetUserClipPlaneControl()</c> function, to specify which of the 
six user-defined clip planes are enabled, and which comparison method to use for each.</li>
<li>Modify a vertex shader so it writes to at least one of the following vertex output 
semantics: <c>CLP0</c>, <c>CLP1</c>, <c>CLP2</c>, <c>CLP3</c>, <c>CLP4</c>, and <c>CLP5</c>. 
The value written in each one represents the distance of the vertex to the clip plane.</li>
</ol>

While there is nothing special to do in the fragment shader, the fragment shader binary 
must still be told to expect user-defined clip plane values, or else no clipping  
takes place.

To do this, you can perform the following:

@code
  // Find out which interpolator (including user-defined clip planes)
  // are output by the vertex shader
  uint32_t usedInterpolators = 
		cellGcmCgGetAttribOutputMask(myVShaderWithClippingPtr);

  // Let the fragment shader know which interpolator to expect (including
  // user-defined clip planes)
  cellGcmCgSetAttribOutputMask (myFShaderPtr, usedInterpolators);

  // At this point, when the fragment shader is used in conjunction with
  // the vertex shader, it will perform user-defined clipping, provided
  // cellGcmSetUserClipPlaneControl() was called to enable clip planes.
  ...
@endcode

@param program The Cgb program handle

@param attributeOutputMask The returned vertex <c><i>attributeOutputMask</i></c>

@return Returns <c>CELL_CGB_OK</c> if successful, or <c>CELL_CGB_ERROR_FAILED</c> if the 
program is not a vertex program.

@note
The mask value is returned with the target endian byte order; when it is used on the host, 
endian swap might be required. 
@par
For examples of usage, see samples <c>/samples/sdk/graphics/cgb/userclip_gcm</c> 
and <c>/samples/sdk/graphics/cgb/userclip_psgl</c>.
*/
int32_t cellCgbGetVertexAttributeOutputMask(const CellCgbProgram *program, uint32_t *attributeOutputMask)
{
	if (program->header->profile != CellCgbVertexProfile)
		return CELL_CGB_ERROR_FAILED;
	const CellCgbVertexConfigurationInternal * vertexConf = (const CellCgbVertexConfigurationInternal *)(program->header+1);
	*attributeOutputMask = vertexConf->attributeOutputMask;
	return CELL_CGB_OK;
	
};

//user clip plane control mask
/**
@short Gets the vertex program <c><i>userClipMask</i></c>.

Gets the vertex program <c><i>userClipMask</i></c>, which can subsequently be set with 
libgcm using <c>cellGcmSetUserClipPlaneControl()</c>, or by PSGL 
(SDK 2.0.0 or later) with <c>psglSetUserClipPlanes()</c>. 
See the <i>libgcm Reference</i> or the <i>PlayStation&reg; Graphics Library Reference</i> 
for further information.

@param program The Cgb program handle

@param userClipPlaneControlMask The returned vertex <c><i>userClipMask</i></c>

@return Returns <c>CELL_CGB_OK</c> if successful, or <c>CELL_CGB_ERROR_FAILED</c> if the program 
is not a vertex program.

@note
When setting the clip control mask with PSGL, you must also enable the coinciding 
<c>GL_CLIP_PLANES</c> with <c>glEnable(GL_CLIP_PLANE0...5)</c>.
@par
The mask value is returned with the target endian byte order; when 
it is used on the host, endian swap might be required
@par
For examples of usage, see samples <c>/samples/sdk/graphics/cgb/userclip_gcm</c> 
and <c>/samples/sdk/graphics/cgb/userclip_psgl</c>.
*/
int32_t cellCgbGetUserClipPlaneControlMask(const CellCgbProgram *program, uint32_t *userClipPlaneControlMask)
{
	if (program->header->profile != CellCgbVertexProfile)
		return CELL_CGB_ERROR_FAILED;
	const CellCgbVertexConfigurationInternal * vertexConf = (const CellCgbVertexConfigurationInternal *)(program->header+1);
	*userClipPlaneControlMask = vertexConf->userClipMask;
	return CELL_CGB_OK;	
};


/**

@short Gets the size of a program's microcode.

This function returns the size, in bytes, of a program's microcode.

@param program The Cgb program handle

@note
The return value will have native endian byte order. For example, it will have 
big endian byte order on the target, and little endian byte order on the PC host. 
No endian swap should be required for the host tool.

@return Returns the size of the program's microcode.

@sa cellCgbRead
*/
uint32_t cellCgbGetUCodeSize(const CellCgbProgram *program)
{
	return (uint32_t)ENDSWAP(program->header->ucode_size);
}

/**

@short Gets a program's microcode.

This function returns a <c>const</c> pointer to the microcode of the program stored in the Cgb file.
@note When fragment program microcode is copied to memory, following alignment requirement has to be met.
<ul>
<li>Local memory: 64 Byte alignment</li>
<li>Main memory: 128 Byte alignment</li>
</ul>


@param program The Cgb program handle

@return Returns a <c>const</c> pointer to the program's microcode.

@sa cellCgbRead
*/
const void* cellCgbGetUCode(const CellCgbProgram *program)
{
	return program->ucode;
}

/**

@short Gets the number of constants and the default values for the vertex program.

This function returns the number of constant values and the default values for a vertex program, including:
<ul>
<li>The internal constants generated by the compiler</li>
<li>The constant defined by the user</li>
<li>The uniforms that have a default value</li>
</ul>

@param program The Cgb program handle

@return Returns the number of constants and the defaults values for the vertex program.

@note 
The return value will have native endian byte order. For example, it will have big endian 
byte order on the target, and little endian byte order on the PC host. No endian swap 
should be required for the host tool.

@sa cellCgbGetVertexConstantValues, cellCgbRead
*/
uint32_t cellCgbGetVertexConstantCount(const CellCgbProgram *program)
{
	return (uint32_t)program->constantCount;
}

/**

@short Gets information about the vertex program constants.

This function returns the register and the value stored at <c><I>value_index</I></c> in the constants
table.  The floats pointed to by <c><i>value</i></c> will have target endianness so they must be swapped for use on host.

Each time the vertex program is bound at runtime, those values must be set in the vertex registers
in order for the vertex program to work correctly.

For uniforms with default, this is not mandatory if the user code wants to override the default.
However such information about the type of the constant is not stored at that low level.
The constant table only associates register to <c>float[4]</c> values.

@param program The Cgb program handle
@param value_index The index of the value, in the range 0 to the count returned by
cellCgbGetVertexConstantCount()
@param reg The vertex constant register in which the shader microcode is expecting the value
@param value A pointer to a <c>const float[4]</c> containing the value to set into the register in target endianness

@note
The vertex constant register value is returned with native endian byte order. 
For example, it will have big endian byte order on the target, and little endian byte 
order on the PC host. No endian swap should be required for the host tool.

@sa cellCgbGetVertexConstantCount, cellCgbRead
*/
void cellCgbGetVertexConstantValues(const CellCgbProgram *program, uint32_t value_index, uint16_t *reg, const float **value)
{
	if (program->header->profile != CellCgbVertexProfile || value_index >= program->constantCount)
	{
		*reg = 0xffff;
		    return;
	}
	*reg =  ENDSWAP(*(program->constantResources + value_index));
	*value = program->constantValues + value_index*4;
}

//return the first marker found going from end to start
static const char * _getMarkerReverse(const char *start, const char *end)
{
	const char *p = end;
	while (p>start)
	{
		if (*p == '[' || *p == '.')
			return p;
		p--;
	}
	return p;
}

/**

@short Looks up a hardware resource by name.

This function returns the index of the hardware resource entry in the map table associated with a given name.
The index can then be passed to cellCgbMapGetValue() to retrieve the hardware resource.

Some inline helper functions are provided to get the resource directly from the name.
The Map table associates names and hardware resources in a very efficient way, and does not store any parameter
information.

There are several types of hardware resources. For a vertex program:
<ul>
<li>static control flow Boolean register index (range: 0 - 31)</li>
<li>float constant register (range: 0 - 467)</li>
<li>input attribute (range: 0 - 15)</li>
<li>texture unit (range: 0 - 3)</li>
</ul>

For a fragment program:
<ul>
<li>fragment embedded constant offsets</li>
<li>texture unit (range: 0 - 15)</li>
</ul>

@param program The Cgb program handle
@param name The name of the parameter as defined in the source of the Cg program

@return Returns the index of the map entry if the search succeeds, or <c>CELL_CGB_ERROR_FAILED</c> if the name
cannot be found.

@note
The return value will have native endian byte order. For example, it will have big endian 
byte order on the target, and little endian byte order on the PC host. No endian swap should 
be required for the host tool.

@sa cellCgbMapGetValue, cellCgbMapLookupTextureUnit, cellCgbMapLookupAttribute, cellCgbRead
*/
uint32_t cellCgbMapLookup(CellCgbProgram *program, const char *name)
{
	//I need to split the name
	const char *end = name + strlen(name);
	const char *start = _getMarkerReverse(name,end);
	size_t length = end - start;

	//linear lookup in this first version
	for (int i=0;i<(int)program->mapEntryCount;i++)
	{
		int head = *start == '[' || *start == '.'? 1 : 0; //remove the [ ] in the comparison, else just remove the "."
		int tail = *start == '[' ? 1 : 0; //remove the ] in the comparison
		const char *nodeName = program->stringTable + ENDSWAP(program->mapEntries[i].name_offset);
		if (!strncmp(nodeName, start+head, length-head-tail))
		{
			//we found an item with the correct child name, is it the right one
			char buffer[MAX_PARAMETERNAME_LENGTH];
			unsigned int size = sizeof(buffer);
			cellCgbMapGetName(program,i,buffer,&size);
			if (!strcmp(buffer,name))
				return i;
		}
	}
	return CELL_CGB_ERROR_FAILED;
}

/** @fn cellCgbMapLookupTextureUnit(CellCgbProgram *program, const char *name)
@short Looks up a texture unit by name

This function returns the texture unit associated with a given name.
The texture unit is an index between 0 and 15 for a fragment program,  and between 0 and 3 for a vertex program.
This function is provided as an inline function to help write code that is easier to read &mdash;
it does not validate the type of the resource associated with <c><I>name</I></c>.

Arrays must be treated carefully. Generally, Cgb is made for users who know about their data and
this is especially true
for arrays. If an array of vertex uniforms is fully referenced and if the register allocation has been
sequential, the array
will be packed. In that case, passing the array name returns an index, and cellCgbMapGetValue()
returns the start
register of the array. A function that is able to set a block of vertex register can be used to set registers.
The user has to know the size of the array, but this information is not stored at that level.
This is a simple associative table between name and resource.

When the array is not packed, asking for <c>arrayname[x]</c> returns the index of the map entry.
In optimized cases, the developer always knows which data needs to be set into the
array, and knows what to ask for.
Cg or the compiler output must be used to browse the shader parameters.

@param program The Cgb program handle
@param name The name of the sampler parameter defined in the source of the Cg program

@return Returns the texture unit index if the search succeeds, or <c>CELL_CGB_ERROR_FAILED</c> if the name cannot be found

@note
The return value will have native endian byte order. For example, it will have big endian 
byte order on the target, and little endian byte order on the PC host. No endian swap 
should be required for the host tool.

@sa cellCgbMapLookup, cellCgbMapLookupAttribute, cellCgbRead
*/

/** @fn cellCgbMapLookupAttribute(CellCgbProgram *program, const char *name)
@short Looks up a vertex input attribute by name

This function returns the attribute channel associated with a given parameter during the compilation of the vertex program.
The attribute channel is an index between 0 and 15.
This function is provided as an inline function to help you to write code that is easier to read &mdash;
it does not validate the type of the resource associated with <c><I>name</I></c>.

@param program The Cgb program handle
@param name The name of the sampler parameter defined in the source of the Cg program

@return The vertex input attribute if the search succeeds, or <c>CELL_CGB_ERROR_FAILED</c> if the name cannot be found

@note
The return value will have native endian byte order. For example, it will have big endian byte 
order on the target, and little endian byte order on the PC host. 
No endian swap should be required for the host tool.

@sa cellCgbMapLookup, cellCgbMapLookupTextureUnit, cellCgbRead
*/

/**
@short Gets the hardware resource of a given map entry.

This function returns the hardware resource of a given map entry.
Use this function to retrieve attribute channel indices and texture units.
The behavior of the function is not specified if <c><i>map_index</i></c> has not been returned by cellCgbMapLookup(),
or is not in the range 0 to cellCgbMapGetLength()-1

@param program The Cgb program handle
@param map_index The index in the resource map to read

@return Returns the resource at the given map address.

@note
The return value will have native endian byte order. For example, it will have big 
endian byte order on the target, and little endian byte order on the PC host. 
No endian swap should be required for the host tool.

@sa cellCgbMapLookup, cellCgbMapLookupTextureUnit, cellCgbMapLookupAttribute, cellCgbRead
*/
uint16_t cellCgbMapGetValue(CellCgbProgram *program, const uint32_t map_index)
{
	const CellCgbMapEntry *entry = program->mapEntries + map_index;
	return (uint16_t)ENDSWAP((short)entry->resource); //0xffff should come up as 0xffffffff, will happen in the map_index doesn't correspound to a terminal node
}

/**
* @short Gets the length of the hardware resource map.

This function returns the length of the hardware resource map, and can be used to iterate over all of the resources.
This function iterates over entries that are not necessarily tied to a real hardware resource, but which sometimes are
used to store the substrings of a parameter name, such as when a struct has been defined in the source of the Cg program.

@param program The Cgb program handle

@return Returns the length of the map table, in number of entries.

@note
The return value will have native endian byte order. For example, it will have big 
endian byte order on the target, and little endian byte order on the PC host. 
No endian swap should be required for the host tool.

@sa cellCgbMapGetName, cellCgbMapLookup, cellCgbRead
*/
uint32_t cellCgbMapGetLength(const CellCgbProgram *program)
{
	return (uint32_t)program->mapEntryCount;
}

/**
@short Gets the name at a given index in the map table.

This function fills the buffer <c><i>name</i></c> with the name at <c><i>map_index</i></c>.
The function fails if the buffer passed is not big enough, <c><i>size</i></c> then contains the size needed.
If the function succeeds, <c><i>size</i></c> contains the length of the name written to the buffer.

@param program The Cgb program handle
@param map_index The index in the resource map to read
@param name The buffer to be filled with the name at <I><c>map_index</c></I>
@param size The size of the buffer passed

@sa cellCgbMapGetLength, cellCgbMapLookup, cellCgbRead
*/
void cellCgbMapGetName(CellCgbProgram *program, const uint32_t map_index, char *name, uint32_t *size)
{
	const CellCgbMapEntry *entry = program->mapEntries + map_index;

	//fill the buffer containing the name of the entry backward to avoid intermediate copies
	char buffer[MAX_PARAMETERNAME_LENGTH];
	char *cursor = buffer + MAX_PARAMETERNAME_LENGTH-1;
	*cursor = '\0';

	const char *entryName = program->stringTable + ENDSWAP(entry->name_offset);
	size_t entryNameLength = strlen(entryName);

	bool arrayFirst;
	if (entryName[0]>='0' && entryName[0]<='9')
	{
		//we have an array item
		arrayFirst = true;
		cursor -= entryNameLength + 2;
		sprintf(cursor,"[%s]",entryName);
	}
	else
	{
		arrayFirst = false;
		cursor -= entryNameLength;
		strcpy(cursor,entryName);
	}

	while (ENDSWAP(entry->parent) != 0xffff)
	{
		if (arrayFirst)
			arrayFirst = false;
		else
		{
			cursor--;
			*cursor = '.';
		}
		entry = program->mapEntries + ENDSWAP(entry->parent);
		entryName = program->stringTable + ENDSWAP(entry->name_offset);
		entryNameLength = strlen(entryName);

		if (entryName[0]>='0' && entryName[0]<='9')
		{
			//we have an array item
			arrayFirst = true;
			cursor--;
			*cursor = ']';
			cursor-= entryNameLength;
			strncpy(cursor,entryName,entryNameLength); //no ending 0
			cursor--;
			*cursor = '[';
		}
		else
		{
			arrayFirst = false;
			cursor -= entryNameLength;
			strncpy(cursor,entryName,entryNameLength); //no ending 0
		}
	}

	//enough space ?
	unsigned int spaceNeeded = (unsigned int )(buffer + MAX_PARAMETERNAME_LENGTH-1 - cursor)+1;
	if (name && *size >= spaceNeeded)
	{
		strcpy(name,cursor);
	}
	else if (name && *size > 0)
	{
		name[0] = '\0';
	}
	*size = spaceNeeded;
}

/**
@short Gets the vertex uniform register of a given map entry.

This function returns the vertex uniform register stored at a given map entry. The value is returned
in parameter <c><i>reg</i></c>, which is set to <c>0xffff</c> if the shader program is not a vertex program.

@param program The Cgb program handle
@param map_index The index in the resource map to read
@param reg A pointer to a <c>uint16_t</c> to receive the register index
@param default_values An optional pointer that can receive a pointer on the start of the <c><I>default_values</I></c> for that uniform (can be NULL)

@note
The register index value will have native endian byte order. For example, it will have big 
endian byte order on the target, and little endian byte order on the PC host. 
No endian swap should be required for the host tool.

@sa cellCgbMapLookup, cellCgbMapGetFragmentUniformOffsets, cellCgbMapGetFragmentUniformRegister
*/
void cellCgbMapGetVertexUniformRegister(const CellCgbProgram *program, const uint32_t map_index, uint16_t *reg, const float **default_values)
{
	if (program->header->profile != CellCgbVertexProfile)
	{
		*reg = (uint16_t)-1;
		return;
	}
	const CellCgbMapEntry *entry = program->mapEntries + map_index;
	unsigned short reg0 = ENDSWAP(entry->resource);
	*reg = (int32_t)(short)reg0;
	if (reg0 != 0xffff && default_values)
	{
		int count = (int)program->constantCount;
		//search for a potential default value
		for (int i=0;i<count;i++)
		{
			//if this was orderd by reg I could exit early
			if (ENDSWAP(program->constantResources[i]) == reg0)
			{
				*default_values = program->constantValues + 4*i;
				return;
			}
		}
		*default_values = NULL;
	}
}

/**
@short Gets the offsets of a uniform in fragment microcode for a given map entry.

This function returns the offsets of the fragment uniform embedded constants stored at a given map entry.
The <c><i>offsets</i></c> array is filled with the offsets, which are given in bytes and are relative to the
beginning of the microcode.
<c><I>count</I></c> receives the number of embedded locations or offsets in the microcode for that uniform.
This function does not retrieve default values, which are already stored at the offset returned in the
microcode for fragment programs.

@param program The Cgb program handle
@param map_index The index in the resource map to read
@param offsets A pointer of an array of <c>uint16_t*</c> which is filled with the embedded constant offsets
@param count The size of the offset array passed in as input, and the number of embedded constant offsets of the uniform in the output

@note
The offsets and count values will have native endian byte order. For example, they will 
have big endian byte order on the target, and little endian byte order on the PC host. 
No endian swap should be required for the host tool.

@sa cellCgbMapLookup, cellCgbMapGetVertexUniformRegister
*/
void cellCgbMapGetFragmentUniformOffsets(const CellCgbProgram *program, const uint32_t map_index, uint16_t *offsets, uint32_t *count)
{
	if (program->header->profile != CellCgbFragmentProfile)
	{
		*count = 0;
		return;
	}
	const CellCgbMapEntry *entry = program->mapEntries + map_index;
	unsigned short resource = ENDSWAP(entry->resource);
	if (resource <1024 || resource == 0xffff)
	{
		//this is not an index in the offset table
		*count = 0;
		return;
	}
	resource-= 1024;

	unsigned short offset = resource + 1; // +1 to skip the "virtual" register, this is the register that the user can assign, it won't be returned for now
	uint32_t tmpCount = ENDSWAP(program->fpOffsets[offset++]);
	if (offsets && *count >= tmpCount)
	{
		for (int i=0;i<(int)tmpCount;i++)
			offsets[i] = ENDSWAP(program->fpOffsets[offset+i]);
		//needed ??
		for (int i=(int)tmpCount;i<(int)*count;i++)
			offsets[i] = 0xffff;
	}
	else if (offsets)
	{
		for (int i=0;i<(int)*count;i++)
			offsets[i] = 0xffff;
	}
	*count = tmpCount;
}

/**
@short Gets the fragment uniform register of a given map entry.

This function returns the fragment uniform register stored at a given map entry. The value is returned in parameter <c><i>reg</i></c>,
which is set to <c>0xffff</c> if the shader program is not a fragment program, or if there is no register assigned to this entry.
Fragment registers are assigned by the user in the Cg code with a semantic, they don't represent a real resource on the RSX&trade;.

@param program The Cgb program handle
@param map_index The index in the resource map to read
@param reg A pointer to a <c>uint16_t</c> to receive the register index

@note
The register index value will have native endian byte order. For example, 
it will have big endian byte order on the target, and little endian byte order 
on the PC host. No endian swap should be required for the host tool.

@sa cellCgbMapLookup, cellCgbMapGetFragmentUniformOffsets, cellCgbMapGetVertexUniformRegister
*/
void cellCgbMapGetFragmentUniformRegister(const CellCgbProgram *program, const uint32_t map_index, uint16_t *reg)
{
	if (program->header->profile != CellCgbFragmentProfile)
	{
		*reg = 0xffff;
		return;
	}
	const CellCgbMapEntry *entry = program->mapEntries + map_index;
	unsigned short resource = ENDSWAP(entry->resource);
	if (resource <1024 || resource == 0xffff)
	{
		//this is not an index in the offset table
		*reg = 0xffff;
		return;
	}
	resource-= 1024;
	*reg = ENDSWAP(program->fpOffsets[resource]);
}

/**
@short Gets the size of a Cgb file.

This function returns the size of the Cgb file passed in the parameter <I><c>binary</c></I>.

@param binary A buffer containing the shader in the Cgb format

@return Returns the size of the program's microcode.

@note
The return value will have native endian byte order. For example, it will have 
big endian byte order on the target, and little endian byte order on the PC host. 
No endian swap should be required for the host tool.

@sa cellCgbRead
*/
uint32_t cellCgbGetSize(const void *binary)
{
	const CellCgbHeader *header = (const CellCgbHeader *)binary;
	CellCgbProfile profile = (CellCgbProfile)header->profile;
	if (header->profile != CellCgbVertexProfile && header->profile != CellCgbFragmentProfile)
		return CELL_CGB_ERROR_FAILED;

	//get the header and ucode size
	unsigned int size = sizeof(CellCgbHeader);
	size += profile == CellCgbVertexProfile  ? sizeof(CellCgbVertexConfigurationInternal) : sizeof(CellCgbVertexConfigurationInternal);
	size = PAD16(size); //16 byte alignment of the ucode
	size += ENDSWAP(header->ucode_size); //add ucode size

	//adds the size of each block which follows directly the header/ucode block,
	//the size of the block is stored in the first short of each block
	//the block_mask defines which blocks are present, in the context of this function only the number of blocks is important
	unsigned char block_mask = ENDSWAP(header->content);
	const unsigned char* block_sizes = (const unsigned char*)binary;
	while (block_mask) //loop for each bit set in block_mask
	{
		unsigned short* block_size_pointer = (unsigned short*)(block_sizes + size);
		size += ENDSWAP( *block_size_pointer );
		block_mask &= (block_mask-1); //this will remove the least significant bit set
	}
	return size;
}

