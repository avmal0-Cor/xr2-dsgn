/* SCE CONFIDENTIAL
* PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2008 Sony Computer Entertainment Inc. 
* All Rights Reserved.
*/
/* string.h standard header */
#ifndef _STRING
#define _STRING

#ifndef _YVALS
 #include <yvals.h>
#endif /* _YVALS */

#include <sys/types.h> /* reffer definition of size_t */

_C_STD_BEGIN

		/* macros */
#ifndef NULL
 #define NULL	_NULL
#endif /* NULL */

		/* type definitions */

		/* declarations */
_C_LIB_DECL
int memcmp(const void *, const void *, size_t);
void *memcpy(void *_Restrict, const void *_Restrict, size_t);
void *memcpy16(void *_Restrict, const void *_Restrict, size_t);
void *memset(void *, int, size_t);
char *strcat(char *_Restrict, const char *_Restrict);
int strcmp(const char *, const char *);
char *strcpy(char *_Restrict, const char *_Restrict);
size_t strlen(const char *);

void *memmove(void *, const void *, size_t);
int strcoll(const char *, const char *);
size_t strcspn(const char *, const char *);
char *strerror(int);
char *strncat(char *_Restrict, const char *_Restrict, size_t);
int strncmp(const char *, const char *, size_t);
char *strncpy(char *_Restrict, const char *_Restrict, size_t);
size_t strspn(const char *, const char *);
char *strtok(char *_Restrict, const char *_Restrict);
size_t strxfrm(char *_Restrict, const char *_Restrict, size_t);
char *strdup(const char *);
int strcasecmp(const char *, const char *);
#define strcasecmp_ascii(x, y) strcasecmp(x, y)

int strncasecmp(const char *, const char *, size_t);
#define strncasecmp_ascii(x, y, n) strncasecmp(x, y, n)
char *strtok_r(char *_Restrict, const char *_Restrict,char **_Restrict);

_END_C_LIB_DECL

 #if defined(__cplusplus) && !defined(_NO_CPP_INLINES)
		// INLINES AND OVERLOADS, for C++
 #define _Const_return const

_C_LIB_DECL
const char *strchr(const char *, int);
const char *strpbrk(const char *, const char *);
const char *strrchr(const char *, int);
const char *strstr(const char *, const char *);
_END_C_LIB_DECL

inline char *strchr(char *_Str, int _Ch) {
  // call with const first argument
  return (const_cast<char *>(_CSTD strchr((const char *)_Str, _Ch)));
}

inline char *strpbrk(char *_Str1, const char *_Str2) {
  // call with const first argument
  return (const_cast<char *>(_CSTD strpbrk((const char *)_Str1, _Str2)));
}

inline char *strrchr(char *_Str, int _Ch) {
  // call with const first argument
  return (const_cast<char *>(_CSTD strrchr((const char *)_Str, _Ch)));
}

inline char *strstr(char *_Str1, const char *_Str2) {
  // call with const first argument
  return (const_cast<char *>(_CSTD strstr((const char *)_Str1, _Str2)));
}

 #else /* defined(__cplusplus) && !defined(_NO_CPP_INLINES)*/
_C_LIB_DECL
 #define _Const_return

char *strchr(const char *, int);
char *strpbrk(const char *, const char *);
char *strrchr(const char *, int);
char *strstr(const char *, const char *);
_END_C_LIB_DECL
 #endif /* defined(__cplusplus) && !defined(_NO_CPP_INLINES) */

 #ifndef _MEMCHR
  #define _MEMCHR

  #if defined(__cplusplus) && !defined(_NO_CPP_INLINES)
_C_LIB_DECL
const void *memchr(const void *, int, size_t);
_END_C_LIB_DECL

inline void *memchr(void *_Str, int _Ch, size_t _Num) {
  // call with const first argument
  return (const_cast<void *>(_CSTD memchr((const void *)_Str, _Ch, _Num)));
}

  #else /* defined(__cplusplus) && !defined(_NO_CPP_INLINES) */
_C_LIB_DECL
void *memchr(const void *, int, size_t);
_END_C_LIB_DECL
  #endif /* defined(__cplusplus) && !defined(_NO_CPP_INLINES) */

 #endif /* _MEMCHR */
_C_STD_END
#endif /* _STRING */

 #if defined(_STD_USING)

using _CSTD memchr; using _CSTD memcmp;
using _CSTD memcpy; using _CSTD memcpy16; using _CSTD memmove; using _CSTD memset;
using _CSTD strcat; using _CSTD strchr; using _CSTD strcmp;
using _CSTD strcoll; using _CSTD strcpy; using _CSTD strcspn;
using _CSTD strerror; using _CSTD strlen; using _CSTD strncat;
using _CSTD strncmp; using _CSTD strncpy; using _CSTD strpbrk;
using _CSTD strrchr; using _CSTD strspn; using _CSTD strstr;
using _CSTD strtok; using _CSTD strxfrm; using _CSTD strdup;
using _CSTD strcasecmp; using _CSTD strncasecmp; using _CSTD strtok_r;
 #endif /* defined(_STD_USING) */

/*
 * Copyright (c) 1992-2003 by P.J. Plauger.  ALL RIGHTS RESERVED.
 * Consult your license regarding permissions and restrictions.
V4.02:0216 */
