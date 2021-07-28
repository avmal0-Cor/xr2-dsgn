/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 *                Copyright (C) 2009 Sony Computer Entertainment Inc.
 *                                               All Rights Reserved.
 */

// DESCRIPTION:	Platform-independent EA types

#ifndef SPU_TYPES_H
#define SPU_TYPES_H

typedef unsigned long long jsEA;
typedef unsigned long jsIntPtr;
#define JS_PTR_TO_EA(ptr) ((jsEA)(jsIntPtr)(ptr))
#define JS_EA_TO_PTR(ea) ((void *)(jsIntPtr)(ea))

#endif
