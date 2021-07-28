/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2007 Sony Computer Entertainment Inc. 
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

/** #if defined(_WIN32_C_LIB)      \ **/
 #if _WIN32_C_LIB	/* compiler test */ \
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

#ifndef BUFSIZ
#define BUFSIZ		(64*1024)
#endif
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
/* This macro is refered to by iosfwd, stdexcept, xlocale */
#define _Mbstinit(x)	_Mbstatet x = {0UL, 0, 0}

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
	struct _Filet *_Next_elem;
	unsigned char _Back[_MBMAX], _Cbuf;
#ifndef __PME__
	_Rmtx *_stream_mtx;
#endif /** __PME__ **/
	uint64_t fpos;
	uint64_t _Rsize;
	uint64_t _offset;
	unsigned char *_BRend;
	unsigned short _Bflag;
	} _Filet;
typedef _Filet FILE;

		/* declarations */
_C_LIB_DECL
extern FILE _Stdin, _Stdout, _Stderr;

void clearerr(FILE *);
int fclose(FILE *);
int feof(FILE *);
int ferror(FILE *);
int fflush(FILE *);
int fgetc(FILE *);
int fgetpos(FILE *_Restrict, fpos_t *_Restrict);
char *fgets(char *_Restrict, int, FILE *_Restrict);
FILE *fopen(const char *_Restrict, const char *_Restrict);
FILE *fdopen(int, const char *_Restrict);
int fileno(FILE *);
int fprintf(FILE *_Restrict, const char *_Restrict, ...);
int fputc(int, FILE *);
int fputs(const char *_Restrict, FILE *_Restrict);
size_t fread(void *_Restrict, size_t, size_t, FILE *_Restrict);
FILE *freopen(const char *_Restrict, const char *_Restrict,
	FILE *_Restrict);
int fscanf(FILE *_Restrict , const char *_Restrict, ...);
int fseek(FILE *, long, int);
int fsetpos(FILE *, const fpos_t *);
long ftell(FILE *);
size_t fwrite(const void *_Restrict, size_t, size_t,
	FILE *_Restrict);
char *gets(char *);
void perror(const char *);
int printf(const char *_Restrict, ...);
int puts(const char *);
int remove(const char *);
int rename(const char *, const char *);
void rewind(FILE *);
int scanf(const char *_Restrict, ...);
void setbuf(FILE *_Restrict , char *_Restrict);
int setvbuf(FILE *_Restrict , char *_Restrict, int, size_t);
int sprintf(char *_Restrict, const char *_Restrict, ...);
int sscanf(const char *_Restrict, const char *_Restrict, ...);
int ungetc(int, FILE *);
int vfprintf(FILE *_Restrict, const char *_Restrict, _Va_list);
int vprintf(const char *_Restrict, _Va_list);
int vsprintf(char *_Restrict, const char *_Restrict, _Va_list);

long _Fgpos(FILE *, fpos_t *);
int _Flocale(FILE *, const char *, int);
void _Fsetlocale(FILE *, int);
int _Fspos(FILE *, const fpos_t *, long, int);

 #if _MULTI_THREAD
void _Lockfilelock(_Filet *);
void _Unlockfilelock(_Filet *);
 #endif /* _MULTI_THREAD */

 #if _HAS_C9X
int snprintf(char *_Restrict, size_t,
	const char *_Restrict, ...);
int vsnprintf(char *_Restrict, size_t,
	const char *_Restrict, _Va_list);
int vfscanf(FILE *_Restrict,
	const char *_Restrict, _Va_list);
int vscanf(const char *_Restrict, _Va_list);
int vsscanf(const char *_Restrict,
	const char *_Restrict, _Va_list);

 #else /* _IS_C9X */

int snprintf(char *_Restrict, size_t,
	const char *_Restrict, ...);
int vsnprintf(char *_Restrict, size_t,
	const char *_Restrict, _Va_list);

 #endif /* _IS_C9X */
_END_C_LIB_DECL

 #if _MULTI_THREAD && _FILE_OP_LOCKS
		/* declarations only */
_C_LIB_DECL
int getc(FILE *);
int getchar(void);
int putc(int, FILE *);
int putchar(int);
_END_C_LIB_DECL

 #else /* _MULTI_THREAD && _FILE_OP_LOCKS */

  #if defined(__cplusplus) && !defined(_NO_CPP_INLINES)
		// INLINES, for C++

 #if !defined(__GNUC__)	/* compiler test */
_C_LIB_DECL
 #endif /* !defined(__GNUC__) */

inline int getc(FILE *_Str)
	{	// get a character
	return ((_Str->_Next < _Str->_Rend
		? *_Str->_Next++ : fgetc(_Str)));
	}

inline int getchar()
	{	// get a character from stdin
	return ((stdin->_Next < stdin->_Rend
	? *stdin->_Next++ : fgetc(stdin)));
	}

inline int putc(int _Ch, FILE *_Str)
	{	// put a character
	return ((_Str->_Next < _Str->_Wend
		? (*_Str->_Next++ = (char)_Ch) : fputc(_Ch, _Str)));
	}

inline int putchar(int _Ch)
	{	// put a character to stdout
	return ((stdout->_Next < stdout->_Wend
	? (*stdout->_Next++ = (char)_Ch) : fputc(_Ch, stdout)));
	}

 #if !defined(__GNUC__)	/* compiler test */
_END_C_LIB_DECL
 #endif /* !defined(__GNUC__) */

  #else /* defined(__cplusplus) && !defined(_NO_CPP_INLINES) */
		/* declarations and macro overrides, for C */
_C_LIB_DECL
int getc(FILE *);
int getchar(void);
int putc(int, FILE *);
int putchar(int);
_END_C_LIB_DECL

 #define getc(str)	((str)->_Next < (str)->_Rend \
	? *(str)->_Next++ : (fgetc)(str))

 #define getchar()	(stdin->_Next < stdin->_Rend \
	? *stdin->_Next++ : (fgetc)(stdin))

 #define putc(ch, str)	((str)->_Next < (str)->_Wend \
	? (*(str)->_Next++ = ch) : (fputc)(ch, str))

 #define putchar(ch)	(stdout->_Next < stdout->_Wend \
	? (*stdout->_Next++ = ch) : (fputc)(ch, stdout))
  #endif /* defined(__cplusplus) && !defined(_NO_CPP_INLINES) */

 #endif /* _MULTI_THREAD && _FILE_OP_LOCKS */
_C_STD_END
#endif /* _STDIO */

 #if defined(_STD_USING)

using _CSTD fpos_t; using _CSTD FILE;
using _CSTD clearerr; using _CSTD fclose; using _CSTD feof;
using _CSTD ferror; using _CSTD fflush; using _CSTD fgetc;
using _CSTD fgetpos; using _CSTD fgets; using _CSTD fopen;
using _CSTD fileno; using _CSTD fdopen;
using _CSTD fprintf; using _CSTD fputc; using _CSTD fputs;
using _CSTD fread; using _CSTD freopen; using _CSTD fscanf;
using _CSTD fseek; using _CSTD fsetpos; using _CSTD ftell;
using _CSTD fwrite; using _CSTD getc; using _CSTD getchar;
using _CSTD gets; using _CSTD perror;
using _CSTD putc; using _CSTD putchar;
using _CSTD printf; using _CSTD puts; using _CSTD remove;
using _CSTD rename; using _CSTD rewind; using _CSTD scanf;
using _CSTD setbuf; using _CSTD setvbuf; using _CSTD sprintf;
using _CSTD sscanf;
using _CSTD ungetc; using _CSTD vfprintf; using _CSTD vprintf;
using _CSTD vsprintf;

 #if _HAS_C9X
using _CSTD snprintf; using _CSTD vsnprintf;
using _CSTD vfscanf; using _CSTD vscanf; using _CSTD vsscanf;

 #else /* _IS_C9X */

using _CSTD snprintf; using _CSTD vsnprintf;

 #endif /* _IS_C9X */

 #endif /* defined(_STD_USING) */

/*
 * Copyright (c) 1992-2003 by P.J. Plauger.  ALL RIGHTS RESERVED.
 * Consult your license regarding permissions and restrictions.
V4.02:0216 */
