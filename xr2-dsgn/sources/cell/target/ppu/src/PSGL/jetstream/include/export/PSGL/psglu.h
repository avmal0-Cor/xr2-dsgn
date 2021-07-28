/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 *                Copyright (C) 2009 Sony Computer Entertainment Inc.
 *                                               All Rights Reserved.
 */

/*
 * PSGL Utility Library
 */

#ifndef _PSGLU_h
#define _PSGLU_h

#include <PSGL/export.h>
#include <PSGL/psgl.h>

#ifdef __cplusplus
extern "C"
{
#endif


// Error codes
//
#define GLU_INVALID_ENUM                   100900
#define GLU_INVALID_VALUE                  100901
#define GLU_OUT_OF_MEMORY                  100902
#define GLU_INVALID_OPERATION              100904


// StringName
//
#define GLU_VERSION                        100800
#define GLU_EXTENSIONS                     100801


    extern PSGLU_EXPORT void gluPerspectivef( GLfloat fovy, GLfloat aspect, GLfloat zNear, GLfloat zFar );
    extern PSGLU_EXPORT void gluLookAtf( GLfloat Xeye, GLfloat Yeye, GLfloat Zeye, GLfloat Xtarget, GLfloat Ytarget, GLfloat Ztarget, GLfloat Xup, GLfloat Yup, GLfloat Zup );
    extern PSGLU_EXPORT void gluOrtho2Df( GLfloat left, GLfloat right, GLfloat bottom, GLfloat top );
    extern PSGLU_EXPORT const GLubyte* gluErrorString( GLenum error );
    extern PSGLU_EXPORT const GLubyte* gluGetString( GLenum error );
    extern PSGLU_EXPORT GLint gluScaleImage( GLenum format, GLsizei wIn, GLsizei hIn, GLenum typeIn, const void *dataIn, GLsizei wOut, GLsizei hOut, GLenum typeOut, GLvoid* dataOut );
    extern PSGLU_EXPORT GLint gluBuild2DMipmaps( GLenum target, GLint internalFormat, GLsizei width, GLsizei height, GLenum format, GLenum type, const void *data );


#ifdef __cplusplus
}
#endif

#endif
