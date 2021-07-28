/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2006 Sony Computer Entertainment Inc. 
* All Rights Reserved.
*/
/* stdio.h standard header */
#ifndef _STDIO
#define _STDIO
#ifndef _YVALS
 #include <yvals.h>
#endif /* _YVALS */

#include <sys/types.h> /* reffer definition of size_t */

_C_STD_BEGIN

 #ifndef _HAS_DINKUM_CLIB
  #define _HAS_DINKUM_CLIB	1
 #endif /* _HAS_DINKUM_CLIB */

 #if defined(_WIN32_C_LIB)	/* compiler test */ \
	&& !defined(__BORLANDC__) && !defined(__MINGW32__)
 #define fflush	_DNK_fflush	/* kludge for VC++ */
 #endif /* defined(_WIN32_C_LIB) */

 #if defined(__MWERKS__)	/* compiler test */
 #define fclose	_DNK_fclose
 #define fopen	_DNK_fopen
 #endif /* defined(__MWERKS__) */

		/* MACROS */
#ifndef NULL
 #define NULL	 	_NULL
#endif /* NULL */

#define _IOFBF		0
#define _IOLBF		1
#define _IONBF		2

#define BUFSIZ		512
#define EOF			(-1)
#define FILENAME_MAX	_FNAMAX
#define FOPEN_MAX		_FOPMAX
#define L_tmpnam		_TNAMAX
#define TMP_MAX			32

#define SEEK_SET	0
#define SEEK_CUR	1
#define SEEK_END	2

#define stdin		(&_CSTD _Stdin)
#define stdout		(&_CSTD _Stdout)
#define stderr		(&_CSTD _Stderr)

 #if _MULTI_THREAD
  #define _Lockfile(str)	_Lockfilelock(str)
  #define _Unlockfile(str)	_Unlockfilelock(str)

 #else /* _MULTI_THREAD */
  #define _Lockfile(x)		(void)0
  #define _Unlockfile(x)	(void)0
 #endif /* _MULTI_THREAD */

		/* TYPE DEFINITIONS */
 #ifndef _MBSTATET
  #define _MBSTATET
typedef struct _Mbstatet
	{	/* state of a multibyte translation */
	unsigned long _Wchar;
	unsigned short _Byte, _State;
	} _Mbstatet;
 #endif /* _MBSTATET */

typedef struct fpos_t
	{	/* file position */
	long _Off;	/* can be system dependent */
	_Mbstatet _Wstate;
	} fpos_t;

 #define _FPOSOFF(fp)	((fp)._Off)

typedef struct _Filet
	{	/* file control information */
	unsigned short _Mode;
	unsigned char _Idx;
	_FD_TYPE _Handle;

	unsigned char *_Buf, *_Bend, *_Next;
	unsigned char *_Rend, *_Wend, *_Rback;

	_Wchart *_WRback, _WBack[2];
	unsigned char *_Rsave, *_WRend, *_WWend;

	_Mbstatet _Wstate;
	char *_Tmpnam;
	unsigned char _Back[_MBMAX], _Cbuf;
	} _Filet;
typedef _Filet FILE;

		/* declarations */
_C_LIB_DECL
extern FILE _Stdin, _Stdout, _Stderr;

int sprintf(char *_Restrict, const char *_Restrict, ...);
int sscanf(const char *_Restrict, const char *_Restrict, ...);
int vsprintf(char *_Restrict, const char *_Restrict, _Va_list);

 #if _MULTI_THREAD
void _Lockfilelock(_Filet *);
void _Unlockfilelock(_Filet *);
 #endif /* _MULTI_THREAD */

extern FILE *_Files[FOPEN_MAX];

 #if _HAS_C9X
int snprintf(char *_Restrict, size_t,
	const char *_Restrict, ...);
int vsnprintf(char *_Restrict, size_t,
	const char *_Restrict, _Va_list);
int vsscanf(const char *_Restrict,
	const char *_Restrict, _Va_list);

 #else /* _IS_C9X */

int snprintf(char *_Restrict, size_t,
	const char *_Restrict, ...);
int vsnprintf(char *_Restrict, size_t,
	const char *_Restrict, _Va_list);

 #endif /* _IS_C9X */
_END_C_LIB_DECL

_C_STD_END
#endif /* _STDIO */

 #if defined(_STD_USING)

using _CSTD fpos_t; using _CSTD FILE;
using _CSTD sprintf; using _CSTD sscanf;
using _CSTD vsprintf;

 #if _HAS_C9X
using _CSTD snprintf; using _CSTD vsnprintf; using _CSTD vsscanf;

 #else /* _IS_C9X */

using _CSTD snprintf; using _CSTD vsnprintf;

 #endif /* _IS_C9X */

 #endif /* defined(_STD_USING) */

/*
 * Copyright (c) 1992-2003 by P.J. Plauger.  ALL RIGHTS RESERVED.
 * Consult your license regarding permissions and restrictions.
V4.02:0216 */
