/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2006 Sony Computer Entertainment Inc. 
* All Rights Reserved.
*/
#ifndef _cggl_sce_h
#define _cggl_sce_h

#include <Cg/NV/cgGL.h>

#ifdef __cplusplus
extern "C" {
#endif

CGGL_API void cgGLSetParameterElementFunc(CGparameter param,
											 GLenum func,
											 GLuint frequency);
CGGL_API void cgGLAttribPointer(GLuint index,
								   GLint fsize,
								   GLenum type,
								   GLboolean normalize,
								   GLsizei stride,
								   const GLvoid *pointer);

CGGL_API void cgGLAttribElementFunc(GLuint index,
									   GLenum func,
									   GLuint frequency);
    
CGGL_API void cgGLAttribValues(GLuint index,
			       GLfloat x,
			       GLfloat y,
			       GLfloat z,
			       GLfloat w);

CGGL_API void cgGLEnableAttrib(GLuint index);

CGGL_API void cgGLDisableAttrib(GLuint index);

CGGL_API void cgGLSetVertexRegister4fv(unsigned int index, const float *v);
CGGL_API void cgGLSetVertexRegisterBlock(unsigned int index, unsigned int count, const float *v);
CGGL_API void cgGLSetFragmentRegister4fv(unsigned int index, const float *v);
CGGL_API void cgGLSetFragmentRegisterBlock(unsigned int index, unsigned int count, const float *v);


void cgGLSetParameter1b(CGparameter param, CGbool v);
void cgGLSetProgramBoolVertexRegisters(CGprogram prog, unsigned int values);
void cgGLSetProgramBoolVertexRegistersBlock(CGprogram prog, unsigned int index, unsigned int count, const CGbool *v);
void cgGLSetBoolVertexRegisters(unsigned int values);
void cgGLSetBoolVertexRegistersSharingMask(CGcontext ctx, unsigned int values);


CGGL_API unsigned int cgGLGetRegisterCount(CGprogram prog);
CGGL_API void cgGLSetRegisterCount(CGprogram prog, const unsigned int regCount);

/** @addtogroup Cg
 *@{
 */
/**
@short Initialize the Cg runtime compiler

This function initializes the Cg runtime compiler, and must be called prior to the first use of 
cgCreateProgram() or cgCreateProgramFromFile() with the <i>program_type</i> parameter set to <code>CG_SOURCE</code>.

To use the runtime compiler, you must link with <code>libcgc.a</code> and <code>libPSGLcgc.a</code>.

@sa       cgCreateProgram, cgCreateProgramFromFile
**/
CGGL_API void cgRTCgcInit();

/**
 * @}
 */
// ------------------------------------------------------

#ifdef __cplusplus
}
#endif

#endif
