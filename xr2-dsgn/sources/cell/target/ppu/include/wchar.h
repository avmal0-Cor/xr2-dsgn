/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2005 Sony Computer Entertainment Inc. 
* All Rights Reserved.
*/
/* wchar.h standard header */
#ifndef _WCHAR
#define _WCHAR
#ifndef _YVALS
 #include <yvals.h>
#endif /* _YVALS */

#include <sys/types.h> /* reffer definition of size_t */

_C_STD_BEGIN

		/* MACROS */
#ifndef NULL
 #define NULL 	_NULL
#endif /* NULL */

#define WCHAR_MIN	_WCMIN
#define WCHAR_MAX	_WCMAX
#define WEOF	((wint_t)(-1))

#ifndef _Mbstinit
#define _Mbstinit(x)	_Mbstatet x = {0UL, 0, 0}
#endif /* _Mbstinit */

		/* TYPE DEFINITIONS */
 #ifndef _MBSTATET
  #define _MBSTATET
typedef struct _Mbstatet
	{	/* state of a multibyte translation */
	unsigned long _Wchar;
	unsigned short _Byte, _State;
	} _Mbstatet;
 #endif /* _MBSTATET */

typedef _Mbstatet mbstate_t;

struct tm;
struct _Filet;

 #ifndef _WCHART
  #define _WCHART
typedef _Wchart wchar_t;
 #endif /* _WCHART */

 #ifndef _WINTT
  #define _WINTT
typedef _Wintt wint_t;
 #endif /* _WINTT */

_C_LIB_DECL
		/* stdio DECLARATIONS */
wint_t fgetwc(struct _Filet *);
wchar_t *fgetws(wchar_t *_Restrict, int,
	struct _Filet *_Restrict);
wint_t fputwc(wchar_t, struct _Filet *);
int fputws(const wchar_t *_Restrict,
	struct _Filet *_Restrict);
int fwide(struct _Filet *, int);
int fwprintf(struct _Filet *_Restrict,
	const wchar_t *_Restrict, ...);
int fwscanf(struct _Filet *_Restrict,
	const wchar_t *_Restrict, ...);
wint_t getwc(struct _Filet *);
wint_t getwchar(void);
wint_t putwc(wchar_t, struct _Filet *);
wint_t putwchar(wchar_t);
int swprintf(wchar_t *_Restrict, size_t,
	const wchar_t *_Restrict, ...);
int swscanf(const wchar_t *_Restrict,
	const wchar_t *_Restrict, ...);
wint_t ungetwc(wint_t, struct _Filet *);
int vfwprintf(struct _Filet *_Restrict,
	const wchar_t *_Restrict, _Va_list);
int vswprintf(wchar_t *_Restrict, size_t,
	const wchar_t *_Restrict, _Va_list);
int vwprintf(const wchar_t *_Restrict, _Va_list);
int wprintf(const wchar_t *_Restrict, ...);
int wscanf(const wchar_t *_Restrict, ...);

 #if _HAS_C9X
int vfwscanf(struct _Filet *_Restrict,
	const wchar_t *_Restrict, _Va_list);
int vswscanf(const wchar_t *_Restrict,
	const wchar_t *_Restrict, _Va_list);
int vwscanf(const wchar_t *_Restrict, _Va_list);
 #endif /* _IS_C9X */

		/* stdlib DECLARATIONS */
size_t mbrlen(const char *_Restrict,
	size_t, mbstate_t *_Restrict);
size_t mbrtowc(wchar_t *_Restrict, const char *,
	size_t, mbstate_t *_Restrict);
size_t mbsrtowcs(wchar_t *_Restrict,
	const char **_Restrict, size_t, mbstate_t *_Restrict);
int mbsinit(const mbstate_t *);
size_t wcrtomb(char *_Restrict,
	wchar_t, mbstate_t *_Restrict);
size_t wcsrtombs(char *_Restrict,
	const wchar_t **_Restrict, size_t, mbstate_t *_Restrict);
long wcstol(const wchar_t *_Restrict,
	wchar_t **_Restrict, int);

 #if _HAS_C9X
_Longlong wcstoll(const wchar_t *_Restrict,
	wchar_t **_Restrict, int);
_ULonglong wcstoull(const wchar_t *_Restrict,
	wchar_t **_Restrict, int);
 #endif /* _IS_C9X */

		/* string DECLARATIONS */
wchar_t *wcscat(wchar_t *_Restrict, const wchar_t *_Restrict);
int wcscmp(const wchar_t *, const wchar_t *);
wchar_t *wcscpy(wchar_t *_Restrict, const wchar_t *_Restrict);
size_t wcslen(const wchar_t *);
int wcsncmp(const wchar_t *, const wchar_t *, size_t);
wchar_t *wcsncpy(wchar_t *_Restrict,
	const wchar_t *_Restrict, size_t);

int wcscoll(const wchar_t *, const wchar_t *);
size_t wcscspn(const wchar_t *, const wchar_t *);
wchar_t *wcsncat(wchar_t *_Restrict,
	const wchar_t *_Restrict, size_t);
size_t wcsspn(const wchar_t *, const wchar_t *);
wchar_t *wcstok(wchar_t *_Restrict, const wchar_t *_Restrict,
	wchar_t **_Restrict);
size_t wcsxfrm(wchar_t *_Restrict,
	const wchar_t *_Restrict, size_t);
int wmemcmp(const wchar_t *, const wchar_t *, size_t);
wchar_t *wmemcpy(wchar_t *_Restrict,
	const wchar_t *_Restrict, size_t);
wchar_t *wmemmove(wchar_t *, const wchar_t *, size_t);
wchar_t *wmemset(wchar_t *, wchar_t, size_t);
 
		/* time DECLARATIONS */
size_t wcsftime(wchar_t *_Restrict, size_t,
	const wchar_t *_Restrict, const struct tm *_Restrict);

wint_t _Btowc(int);
int _Wctob(wint_t);
double _WStod(const wchar_t *, wchar_t **, long);
float _WStof(const wchar_t *, wchar_t **, long);
long double _WStold(const wchar_t *, wchar_t **, long);
unsigned long _WStoul(const wchar_t *, wchar_t **, int);
_END_C_LIB_DECL

 #if defined(__cplusplus) && !defined(_NO_CPP_INLINES)
		// INLINES AND OVERLOADS, FOR C++
  #define _WConst_return const

_C_LIB_DECL
const wchar_t *wmemchr(const wchar_t *, wchar_t, size_t);
_END_C_LIB_DECL

inline wchar_t *wmemchr(wchar_t *_Str, wchar_t _Ch, size_t _Num) {
  // call with const first argument
  return (const_cast<wchar_t *>(wmemchr((const wchar_t *)_Str, _Ch, _Num)));
}

 #else /* defined(__cplusplus) && !defined(_NO_CPP_INLINES) */
  #define _WConst_return

_C_LIB_DECL
wchar_t *wmemchr(const wchar_t *, wchar_t, size_t);
_END_C_LIB_DECL
 #endif /* defined(__cplusplus) && !defined(_NO_CPP_INLINES) */

 #include <xwcstod.h>
 #include <xwstr.h>
_C_STD_END
#endif /* _WCHAR */

 #if defined(_STD_USING)

using _CSTD mbstate_t; using _CSTD tm; using _CSTD wint_t;

using _CSTD btowc; using _CSTD fgetwc; using _CSTD fgetws; using _CSTD fputwc;
using _CSTD fputws; using _CSTD fwide; using _CSTD fwprintf;
using _CSTD fwscanf; using _CSTD getwc; using _CSTD getwchar;
using _CSTD mbrlen; using _CSTD mbrtowc; using _CSTD mbsrtowcs;
using _CSTD mbsinit; using _CSTD putwc; using _CSTD putwchar;
using _CSTD swprintf; using _CSTD swscanf; using _CSTD ungetwc;
using _CSTD vfwprintf; using _CSTD vswprintf; using _CSTD vwprintf;
using _CSTD wcrtomb; using _CSTD wprintf; using _CSTD wscanf;
using _CSTD wcsrtombs; using _CSTD wcstol; using _CSTD wcscat;
using _CSTD wcschr; using _CSTD wcscmp; using _CSTD wcscoll;
using _CSTD wcscpy; using _CSTD wcscspn; using _CSTD wcslen;
using _CSTD wcsncat; using _CSTD wcsncmp; using _CSTD wcsncpy;
using _CSTD wcspbrk; using _CSTD wcsrchr; using _CSTD wcsspn;
using _CSTD wcstod; using _CSTD wcstoul; using _CSTD wcsstr;
using _CSTD wcstok; using _CSTD wcsxfrm; using _CSTD wctob;
using _CSTD wmemchr; using _CSTD wmemcmp; using _CSTD wmemcpy;
using _CSTD wmemmove; using _CSTD wmemset; using _CSTD wcsftime;

 #if _HAS_C9X
using _CSTD vfwscanf; using _CSTD vswscanf; using _CSTD vwscanf;
using _CSTD wcstof; using _CSTD wcstold;
using _CSTD wcstoll; using _CSTD wcstoull;
 #endif /* _IS_C9X */

 #endif /* defined(_STD_USING) */

/*
 * Copyright (c) 1992-2003 by P.J. Plauger.  ALL RIGHTS RESERVED.
 * Consult your license regarding permissions and restrictions.
V4.02:0216 */
