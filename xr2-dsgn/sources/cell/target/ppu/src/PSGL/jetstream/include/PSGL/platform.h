/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 *                Copyright (C) 2009 Sony Computer Entertainment Inc.
 *                                               All Rights Reserved.
 */


/*
 * Constant and type definitions defined for the target platform.
 */

#ifndef _PLATFORM_H
#define _PLATFORM_H

#include <PSGL/psgl.h>

#define JS_LITTLE_ENDIAN 0x1234
#define JS_BIG_ENDIAN 0x4321

typedef char JSint8;
typedef unsigned char jsUint8;
typedef short JSint16;
typedef unsigned short jsUint16;
typedef int JSint32;
typedef unsigned int jsUint32;
typedef long long int JSint64;
typedef	PSGLuint64 jsUint64;

#define JS_ENDIAN JS_BIG_ENDIAN
#define JS_ALIGN_FAST_TRANSFER 128
#ifndef REMOTE_ROOT
#define REMOTE_ROOT "/app_home/"
#endif

#define SPU_TNL_ASYNC

#endif
