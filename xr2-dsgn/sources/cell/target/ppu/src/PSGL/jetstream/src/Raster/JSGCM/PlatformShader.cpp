/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 *                Copyright (C) 2009 Sony Computer Entertainment Inc.
 *                                               All Rights Reserved.
 */

#include <PSGL/psgl.h>
#include <JSGCM/JSGcmUtils.h>
#include <JSGCM/JSGcmShader.h>

#if (OS_VERSION_NUMERIC >= 0x160)
#include <cell/cgb/cgb_struct.h>
#else
#include "../../../../sdk/1_5_0/target/common/include/cell/cgb/cgb_struct.h"
#endif

//#include <cell/cgb/cgb_struct.h> //if this file is not found, copy the one in jetstream/sdk/1_5_0 into your SDK

/**
 * @addtogroup PSGL
 * <!-- was Cgb -->
 *
 * @{
 */
/**

* @short Converts a mapped address to a video memory offset

This function converts a mapped address to a video memory offset. 

The RSX&trade; works with video memory offsets, not a mapped address.  As a result, to fill the 
<c><i>offset</i></c>  member of the <c>CellCgbFragmentProgramConfiguration</c> struct, use this function to convert the mapped address
returned by glMapBuffer() to a video memory offset.

@code
char *buffer = (char *)glMapBuffer(...);
CellCgbFragmentProgramConfiguration conf;
psglAddressToOffset(buffer,&conf.offset);
@endcode

@param address Address of the RSX&trade; memory address to convert
@param offset A pointer to a <c>GLuint</c> that will receive the video memory offset corresponding to <I><c>address</c></I>

@sa CellCgbFragmentProgramConfiguration (see <i>libcgb Reference</i>), glMapBuffer, cellCgbGetFragmentConfiguration(see <i>libcgb Reference</i>)
*/
void psglAddressToOffset(const void *address, GLuint *offset)
{
    int32_t ret = cellGcmAddressToOffset( address, offset );
    
    if ( ret != CELL_OK )
    {
        JS_ASSERT( 0 );
    }
}

/**
* @short Sets the value of a vertex program constant register

This function differs from cgGLSetVertexRegister4fv() in that this function allows you
to set any register from 0 to 467, while cgGLSetVertexRegister4fv() only allows you to set registers
from 0 to 255.

@param reg The register to set in the range 0 to 467
@param value A pointer to a <c>float[4]</c> array. The address must be aligned on 32 bits

@sa psglSetVertexProgramRegisterBlock, cgGLSetVertexRegister4fv, psglSetVertexProgramTransformBranchBits
*/
void psglSetVertexProgramRegister( GLuint reg, const void * __restrict value )
{
    GCM_FUNC( cellGcmSetVertexProgramParameterBlock, reg, 1, (const float*)value );
}

/**
* @short Sets the value of a block of vertex program constant registers

This function differs from cgGLSetVertexRegisterBlock() in that this function allows you
to set any register from 0 to 467, while cgGLSetVertexRegisterBlock() only allows you to set registers
from 0 to 255.

@param reg The register to set in the range 0 to 467
@param count The number of register to set
@param value A pointer to a <c>float[4]</c> array. The address must be aligned on 32 bits

@sa psglSetVertexProgramRegister, cgGLSetVertexRegisterBlock, psglSetVertexProgramTransformBranchBits
*/
void psglSetVertexProgramRegisterBlock( GLuint reg, GLuint count, const void * __restrict value )
{
    GCM_FUNC( cellGcmSetVertexProgramParameterBlock, reg, count, (const float*)value );
}

/**
@short Sets the values for all boolean registers

psglSetVertexProgramTransformBranchBits() sets the boolean registers.
This enables you to bypass Cg and set the static control flow booleans directly. 

The register values will be overwritten whenever a Cg program is bound.

@param		values	The values for all 32 boolean registers

@par Errors

@note
The boolean register consists of 32 bits, referenced by index.  
Uniforms in vertex programs can be associated with a given register by binding 
to one of the <i>Bxx</i> semantics, where <i>xx</i> is an index from 0 to 31.
This index can be retrieved by calling <c>cellCgbMapGetVertexUniformRegister()</c> on a 
boolean parameter which has been associated with a register.

@note
By default, static control flow is enabled for uniform boolean variables with
explicit bindings.  To try to bind all uniform boolean variables for static 
control flow, you must compile your Cg program with the <c>-scf all</c> option.

@sa psglSetVertexProgramRegister, psglSetVertexProgramRegisterBlock, cgGLSetBoolVertexRegisters
*/
void psglSetVertexProgramTransformBranchBits( GLuint values )
{
	GCM_FUNC( cellGcmSetTransformBranchBits, values );
}

/**
* @short Binds a vertex program defined by a hardware vertex configuration to the current state

This function binds the vertex program defined by the hardware vertex configuration <c><i>conf</i></c>. It also sets
the pointer where the vertex program microcode is stored in the PSGL context. This is a low level function that bypasses 
Cg and allows you to set the hardware configuration directly.

You can use this function with cgGLBindProgram(). You can also use a fragment program controlled by the Cg 
runtime with a vertex program set by this function, and you can also mix vertex programs
using the Cg runtime and vertex programs set by this function in the same frame.

In order for the vertex program to be in effect, you must call: 
<code>
cgGLEnableProfile(CG_PROFILE_SCE_VP_RSX)
</code>

The <c>CellCgbVertexProgramConfiguration</c> structure is returned by the Cgb library.

@note Because PSGL is validating states only when the draw call is issued, be extremely careful with
<c>ucodeStorageAddress</c>. psglSetVertexProgramConfiguration() does not make a copy of the vertex program microcode, but still uses the 
address later in the frame when the context is validated at the time of the draw call. It is only at that time 
that the microcode is uploaded in the vertex instruction registers using <I><c>ucodeStorageAddress</c></I>. You must store the microcode.

@param conf The vertex program hardware configuration
@param ucodeStorageAddress Address where the microcode will be stored

@sa cgGLBindProgram, psglSetFragmentProgramConfigurationMemoryLocation
*/
void psglSetVertexProgramConfiguration( const CellCgbVertexProgramConfiguration *conf, const void *ucodeStorageAddress )
{
	//GCM_FUNC( cellGcmSetVertexProgramLoad, conf, ucodeStorageAddress);

	//use the standard PSGL system to set the shader
    PSGLcontext* LContext = _CurrentContext;
    _CGprogram *program = LContext->ShellVertexProgram;
    LContext->BoundVertexProgram = program;

    //static settings, could be set just once
    program->header.profile = CG_PROFILE_SCE_VP_RSX;

    program->ucode = ucodeStorageAddress;
    program->header.instructionCount = conf->instructionCount;
    program->header.attributeInputMask = conf->attributeInputMask;

    program->header.vertexProgram.registerCount = conf->registerCount;
    //program->header.vertexProgram.attributeOutputMask is set by the fragment configuration  or by cgGlBindFragmentProgram
    program->header.vertexProgram.userClipMask = 0; //don't know what to set here

    LContext->needValidate |= PSGL_VALIDATE_VERTEX_PROGRAM;
    LContext->needValidate &= ~PSGL_VALIDATE_VERTEX_CONSTANTS;
}

#define swapU32_16(v) (((v)&0xffff)<<16 | (v)>>16 )
/**
* @short Sets a fragment program embedded constant

This function sets a fragment program embedded constant at a given offset in video memory.

The constant needs to be in big endian format. This function uses an inline transfer to send the data to the RSX&trade;.
The data is half swapped before the transfer &mdash; you do not need to do it. 

In a fragment program, an embedded
constant is always 4 floats, and two embedded constants cannot be consecutive. However it is possible to send less
data if you know that the fragment program will not use the 4 floats.

The video offset can be obtained by adding the offset set in the <c>CellCgbFragmentProgramConfiguration</c> structure
and the fragment uniform offsets obtained using a function like <c>cellCgbMapGetFragmentUniformOffsets</c>.
Because a fragment uniform value may need to be stored at several locations in a fragment program, this function is 
generally called in a loop for each uniform.

@note 
While this function continues to be supported, it is recommended that 
you use the function psglSetFragmentProgramEmbeddedConstantMemoryLocation().

@param dstVidOffset A video memory offset.
@param srcHostAdr The value to set.
@param sizeInWords The size, in words, of the value to set (4 for a fully embedded constant).

@sa cgGLBindProgram, psglSetFragmentProgramConfiguration, psglSetFragmentProgramConfigurationMemoryLocation, psglSetFragmentProgramEmbeddedConstantMemoryLocation, CellCgbFragmentProgramConfiguration (see the <i>libcgb Reference</i>)
*/
void psglSetFragmentProgramEmbeddedConstant( const GLuint dstVidOffset, const GLfloat *srcHostAdr, const GLuint sizeInWords)
{
	GCM_FUNC( cellGcmSetTransferLocation, CELL_GCM_LOCATION_LOCAL );
	
	void *pointer=NULL;
	GCM_FUNC( cellGcmSetInlineTransferPointer, dstVidOffset, sizeInWords, &pointer);
	float *fp = (float*)pointer;
	float *src = (float*)srcHostAdr;
	for (uint32_t j=0; j<sizeInWords;j++)
	{
		*fp = cellGcmSwap16Float32(*src);
		fp++;src++;
	}
}

/**
* @short Sets a fragment program embedded constant in either local or main memory.

This function sets a fragment program embedded constant at a given offset in video memory, either local or main memory.

The constant needs to be in big endian format. This function uses an inline transfer to send the data to the RSX&trade;.
The data is half swapped before the transfer &mdash; you do not need to do it. 

In a fragment program, an embedded
constant is always 4 floats, and two embedded constants cannot be consecutive. However it is possible to send less
data if you know that the fragment program will not use the 4 floats.

The video offset can be obtained by adding the offset set in the <c>CellCgbFragmentProgramConfiguration</c> structure
and the fragment uniform offsets obtained using a function like <c>cellCgbMapGetFragmentUniformOffsets</c>.
Because a fragment uniform value may need to be stored at several locations in a fragment program, this function is 
generally called in a loop for each uniform.

@note 
This function supersedes psglSetFragmentProgramEmbeddedConstant().

@param dstVidOffset A video memory offset.
@param srcHostAdr The value to set.
@param sizeInWords The size, in words, of the value to set (4 for a fully embedded constant).
@param inLocalMemory The location of the fragment shader program. True indicates the program is in local memory; False indicates that the program is in main memory.

@sa cgGLBindProgram, psglSetFragmentProgramConfigurationMemoryLocation, CellCgbFragmentProgramConfiguration (see the <i>libcgb Reference</i>)
*/
void psglSetFragmentProgramEmbeddedConstantMemoryLocation(const GLuint dstVidOffset, const GLfloat *srcHostAdr, const GLuint sizeInWords, bool inLocalMemory)
{
	if ( inLocalMemory )
	{
		GCM_FUNC( cellGcmSetTransferLocation, CELL_GCM_LOCATION_LOCAL );
		
		void *pointer=NULL;
		GCM_FUNC( cellGcmSetInlineTransferPointer, dstVidOffset, sizeInWords, &pointer);
		float *fp = (float*)pointer;
		float *src = (float*)srcHostAdr;
		for (uint32_t j=0; j<sizeInWords;j++)
		{
			*fp = cellGcmSwap16Float32(*src);
			fp++;src++;
		}
	}
	else  // fragment program is in main memory just copy and swap to target embedded constant parameter location 
	{
		// swap while coping 
		float *src = (float*)srcHostAdr;
		float *dst = (float*)dstVidOffset; 
		for (uint32_t j=0; j<sizeInWords;j++)
		{
			*dst = cellGcmSwap16Float32(*src);
			dst++; src++;
		}	
	}
}
#undef swapU32_16

/**
* @short Binds a fragment program defined by a hardware fragment configuration to the current state

This function binds the fragment program defined by the hardware fragment configuration <c><i>conf</i></c>.
The <c><i>offset</i></c> member of the <c>CellCgbFragmentProgramConfiguration</c> structure must contain a video memory offset 
on the corresponding fragment program microcode.
The following sequence of calls shows how to allocate and upload the microcode:

@code
// allocate video memory for fragment program, use texture reference context buffer
GLuint fragmentProgramBufferID;
glGenBuffers(1,&fragmentProgramBufferID);
glBindBuffer(GL_TEXTURE_REFERENCE_BUFFER_SCE,fragmentProgramBufferID);
glBufferData(GL_TEXTURE_REFERENCE_BUFFER_SCE, ucodeSize, NULL,GL_STATIC_DRAW);
void *localMemoryAddress = glMapBuffer(GL_TEXTURE_REFERENCE_BUFFER_SCE,GL_READ_WRITE);
// copy micro code to local memory
memcpy(localMemoryAddress, ucode, ucodeSize); 
@endcode

<c><i>localMemoryAddress</i></c> must be converted to a video memory offset using psglAddressToOffset() and stored in the
<c><i>offset</i></c> member of the <c>CellCgbFragmentProgramConfiguration</c> structure.

You should not call glUnmapBuffer() after the memcpy() following glMapBuffer().  Calling glUnmapBuffer() 
allows the PSGL memory manager to defragment memory by moving memory allocations. This 
process might move the fragment program, so  you should keep glBufferData mapped throughout the duration 
of the fragment program's use. 

This is a low level function which bypasses Cg and allows you to set the hardware configuration directly.

This function can be used with cgGLBindProgram(). You can use a vertex program controlled by the Cg 
runtime with a fragment program set by this function. You can also mix fragment programs
using the Cg runtime and fragment programs set by this function in the same frame.

In order for the fragment program to be in effect, you must call <c>cgGLEnableProfile(CG_PROFILE_SCE_FP_RSX)</c>.
The <c>CellCgbFragmentProgramConfiguration</c> structure is returned by the Cgb library.

@note 
While this function continues to be supported, it is recommended that you use the 
function psglSetFragmentProgramConfigurationMemoryLocation().

@param conf The fragment program hardware configuration

@sa cgGLBindProgram, psglSetVertexProgramConfiguration, psglSetFragmentProgramConfigurationMemoryLocation, CellCgbFragmentProgramConfiguration (see the <i>libcgb Reference</i>)
*/
void psglSetFragmentProgramConfiguration( const CellCgbFragmentProgramConfiguration *conf )
{

	//use the standard PSGL system to set the shader
    PSGLcontext* LContext = _CurrentContext;
    _CGprogram *program = LContext->ShellFragmentProgram;
    LContext->BoundFragmentProgram = program;

    //static settings, could be set just once
    program->header.profile = CG_PROFILE_SCE_FP_RSX;
    program->header.instructionCount = 1; // to avoid to have a count of 0 instruction tested later

    //Cgb for PSGL is a bit convoluted... here is the usage according to docs
    //
    //    char *buffer = (char *)glMapBuffer(...);
    //    CellCgbFragmentProgramConfiguration conf;
    //    psglAddressToOffset(buffer,&conf.offset);
    //
    //Look up the id corresponding to the offset. However, it's possible that users
    //are managing their own sub-heap for fp and passing us an offset within an 
    //allocation, gmmFPOffsetToId will check for this and store the offset in 
    //out pointer if there's one. 
    //Note that we only support local memory for now.
    program->loadProgramId = gmmFPOffsetToId( conf->offset, &(program->loadProgramOffset), true );	
	program->inLocalMemory = true; 

    program->header.attributeInputMask = conf->attributeInputMask;
    program->header.fragmentProgram.texcoordInputMask = conf->texCoordsInputMask;
    program->header.fragmentProgram.texcoord2d = conf->texCoords2D;
    program->header.fragmentProgram.texcoordCentroid = conf->texCoordsCentroid;
    program->header.fragmentProgram.registerCount = conf->registerCount;
    program->header.fragmentProgram.flags = 0;
    if (conf->fragmentControl & 0xE)
    	program->header.fragmentProgram.flags |= CGF_DEPTHREPLACE;
    if (!(conf->fragmentControl & 0x40))
    	program->header.fragmentProgram.flags |= CGF_OUTPUTFROMH0;
    if (conf->fragmentControl &  0x80)
    	program->header.fragmentProgram.flags |= CGF_PIXELKILL;

    //set the vertex output mask to the fragment input mask
    LContext->ShellVertexProgram->header.vertexProgram.attributeOutputMask = conf->attributeInputMask;

    LContext->needValidate |= PSGL_VALIDATE_FRAGMENT_PROGRAM; 
}

/**
* @short Binds a fragment program defined by a hardware fragment configuration to the current state,
whether in local or main memory.

This function binds the fragment program defined by the hardware fragment configuration <c><i>conf</i></c>. 
The function allows you to specify whether the fragment program is stored in local or main memory.
The <c><i>offset</i></c> member of the <c>CellCgbFragmentProgramConfiguration</c> structure must contain a video memory offset 
on the corresponding fragment program microcode.
The following sequence of calls shows how to allocate and upload the microcode from main memory:

@code
// allocate video memory for fragment program, use texture reference context buffer
GLuint fragmentProgramBufferID;
glGenBuffers(1,&fragmentProgram8BufferID);
glBindBuffer(GL_TEXTURE_REFERENCE_BUFFER_SCE,fragmentProgramBufferID);
glBufferData(GL_TEXTURE_REFERENCE_BUFFER_SCE, ucodeSize, NULL,GL_STATIC_DRAW);
void *mainMemoryAddress = glMapBuffer(GL_TEXTURE_REFERENCE_BUFFER_SCE,GL_READ_WRITE);
// copy micro code to memory
memcpy(mainMemoryAddress, ucode, ucodeSize); 
@endcode

<c><i>mainMemoryAddress</i></c> must be converted to a video memory offset using psglAddressToOffset() and stored in the
<c><i>offset</i></c> member of the <c>CellCgbFragmentProgramConfiguration</c> structure.

You should not call glUnmapBuffer() after the memcpy() following glMapBuffer().  Calling glUnmapBuffer() 
allows the PSGL memory manager to defragment memory by moving memory allocations. This 
process might move the fragment program, so  you should keep glBufferData mapped throughout the duration 
of the fragment program's use. 

This is a low level function which bypasses Cg and allows you to set the hardware configuration directly.

This function can be used with cgGLBindProgram(). You can use a vertex program controlled by the Cg 
runtime with a fragment program set by this function. You can also mix fragment programs
using the Cg runtime and fragment programs set by this function in the same frame.

In order for the fragment program to be in effect, you must call <c>cgGLEnableProfile(CG_PROFILE_SCE_FP_RSX)</c>.
The <c>CellCgbFragmentProgramConfiguration</c> structure is returned by the Cgb library.

@note 
This function supersedes psglSetFragmentProgramConfiguration().

@param conf The fragment program hardware configuration.
@param inLocalMemory The location of the fragment shader program. True indicates the program is in local memory; False indicates that the program is in main memory. 

@sa cgGLBindProgram, psglSetVertexProgramConfiguration, CellCgbFragmentProgramConfiguration (see the <i>libcgb Reference</i>)
*/
void psglSetFragmentProgramConfigurationMemoryLocation( const CellCgbFragmentProgramConfiguration *conf, bool inLocalMemory )
{

	//use the standard PSGL system to set the shader
    PSGLcontext* LContext = _CurrentContext;
    _CGprogram *program = LContext->ShellFragmentProgram;
    LContext->BoundFragmentProgram = program;

    //static settings, could be set just once
    program->header.profile = CG_PROFILE_SCE_FP_RSX;
    program->header.instructionCount = 1; // to avoid to have a count of 0 instruction tested later

    //Cgb for PSGL is a bit convoluted... here is the usage according to docs
    //
    //    char *buffer = (char *)glMapBuffer(...);
    //    CellCgbFragmentProgramConfiguration conf;
    //    psglAddressToOffset(buffer,&conf.offset);
    //
    //Look up the id corresponding to the offset. However, it's possible that users
    //are managing their own sub-heap for fp and passing us an offset within an 
    //allocation, gmmFPOffsetToId will check for this and store the offset in 
    //out pointer if there's one. 
    program->loadProgramId = gmmFPOffsetToId( conf->offset, &(program->loadProgramOffset), inLocalMemory );
	program->inLocalMemory = inLocalMemory; 

    program->header.attributeInputMask = conf->attributeInputMask;
    program->header.fragmentProgram.texcoordInputMask = conf->texCoordsInputMask;
    program->header.fragmentProgram.texcoord2d = conf->texCoords2D;
    program->header.fragmentProgram.texcoordCentroid = conf->texCoordsCentroid;
    program->header.fragmentProgram.registerCount = conf->registerCount;
    program->header.fragmentProgram.flags = 0;
    if (conf->fragmentControl & 0xE)
    	program->header.fragmentProgram.flags |= CGF_DEPTHREPLACE;
    if (!(conf->fragmentControl & 0x40))
    	program->header.fragmentProgram.flags |= CGF_OUTPUTFROMH0;
    if (conf->fragmentControl &  0x80)
    	program->header.fragmentProgram.flags |= CGF_PIXELKILL;

    //set the vertex output mask to the fragment input mask
    LContext->ShellVertexProgram->header.vertexProgram.attributeOutputMask = conf->attributeInputMask;

    LContext->needValidate |= PSGL_VALIDATE_FRAGMENT_PROGRAM; 
}



/**
* @short Sets a vertex program&rsquo;s <c><i>userClipMask</i></c> and <c><i>attributeOutputMask</i></c>.

This function sets a vertex program&rsquo;s <c><i>userClipMask</i></c> and <c><i>attributeOutputMask</i></c>. 
First use the libcgb functions <c>cellCgbGetUserClipPlaneControlMask()</c> and
<c>cellCgbGetVertexAttribOutputMask()</c> to obtain these two masks.  

Then, after setting up the vertex and fragment program configurations with <c>psglSetVertexProgramConfiguration()</c> and 
<c>psglSetFragmentProgramConfigurationMemoryLocation()</c>, call <c>psglSetUserClipPlanes()</c> to set
the two masks in the vertex program.

Finally, you must also enable the specific user clip planes by calling 
<c>glEnable(GL_CLIP_PLANE0...5)</c>, which ensures that the proper GL state is set and that clip
settings in the shader aren&rsquo;t automatically disabled by PSGL.

For an example showing usage of these functions, see <c>/samples/sdk/graphics/cgb/userclip_psgl.</c>

@note 
You must call the functions <c>psglSetVertexProgramConfiguration()</c> and 
<c>psglSetFragmentProgramConfigurationMemoryLocation()</c> before calling the function <c>psglSetUserClipPlanes()</c>, 
because <c>psglSetUserClipPlanes()</c> needs to modify the <c><i>userClipMask</i></c> and 
<c><i>attributeOutputMask</i></c> values previously set by the functions 
<c>psglSetVertexProgramConfiguration()</c> and <c>psglSetFragmentProgramConfigurationMemoryLocation()</c>. 

@param userClipControlMask The clip control mask to which to set the vertex program&rsquo;s <c><i>userClipMask</i></c>.
@param vertexOutputAttributeMask The attribute output mask to which to set the vertex program&rsquo;s <c><i>attributeOutputMask</i></c>

@sa psglSetVertexProgramConfiguration, psglSetFragmentProgramConfigurationMemoryLocation, 
cellCgbGetVertexAttribOutputMask, cellCgbGetUserClipPlaneControlMask. 
(See the <i>libcgb Reference</i>.)
*/void psglSetUserClipPlanes( const GLuint userClipControlMask, const GLuint vertexOutputAttributeMask )
{

	//use the standard PSGL system to set the shader
    PSGLcontext* LContext = _CurrentContext;

	// we must have a bound vertex program in order to set the user clip and voutattribmask
	if ( LContext->ShellVertexProgram && LContext->ShellFragmentProgram )
	{
		// This is for setting the sce-cgc user clip control mask for GE, LT, and Disabled for the 
		// 6 control clip planes 
		LContext->ShellVertexProgram->header.vertexProgram.userClipMask = userClipControlMask; 
		// This goes along with clip control, but is the vertex output that contains the user clip semantics that 
		// were passed from the vertex, but aren't needed by the fragment input.  Store this here for setting later 
		LContext->ShellVertexProgram->header.vertexProgram.attributeOutputMask = vertexOutputAttributeMask;

		// Going to make sure the Fragment shader input mask is already setup.
		// Going to go a head and or in the user clip plans now 
		LContext->ShellFragmentProgram->header.attributeInputMask |= ( vertexOutputAttributeMask & ( 0x3f << 6 ) ); 		
	}
	else
	{
		// WARN!!! if no Bound Vertex Program to update  		
		_JS_REPORT_EXTRA(PSGL_REPORT_DEBUG,"MUST call psglSetVertexProgramConfiguration and psglSetFragmentProgramConfiguration before making this call to psglSetUserClipPlanes!");
	}
}

/**
 * @}
 */
