/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2004 Sony Computer Entertainment Inc. 
* All Rights Reserved.
*/
#ifndef _SCE_STRING_H_
#define _SCE_STRING_H_

#include <spu_intrinsics.h>

_C_STD_BEGIN

_C_LIB_DECL

// Loading from this table takes one instruction, generating the shuffle
// pattern would take three and still require a load.  If you call
// memcpy (or other functions that share this table) more than a few
// times, there will be a savings in memory size.
extern const vector unsigned char __Shuffles[];

_END_C_LIB_DECL


#ifndef _NO_INLINE_DEFINITIONS
#include <bits/spec.h>
#include <bits/memchr.h>
#include <bits/memcmp.h>
#include <bits/memcpy.h>
#include <bits/memmove.h>
#include <bits/memset.h>
#include <bits/strcat.h>
#include <bits/strchr.h>
#include <bits/strcmp.h>
#include <bits/strcpy.h>
#include <bits/strcspn.h>
#include <bits/strlen.h>
#include <bits/strncat.h>
#include <bits/strncmp.h>
#include <bits/strpbrk.h>
#include <bits/strrchr.h>
#include <bits/strspn.h>
#include <bits/strstr.h>
#include <bits/strncpy.h>
#endif

_C_STD_END

#endif

#if defined(_STD_USING)
using _CSTD __Shuffles;
#endif
