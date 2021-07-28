/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2009 Sony Computer Entertainment Inc.
* All Rights Reserved.
*/
/* stdlib.h standard header */
#ifndef _STDLIB
#define _STDLIB
#ifndef _YVALS
 #include <yvals.h>
#endif /* _YVALS */

#include <sys/types.h> /* reffer definition of size_t */

_C_STD_BEGIN

		/* MACROS */
#ifndef NULL
 #define NULL	_NULL
#endif /* NULL */

#define EXIT_FAILURE	_EXFAIL
#define EXIT_SUCCESS	0

 #if !_MULTI_THREAD || _COMPILER_TLS && !_GLOBAL_LOCALE
_C_LIB_DECL
_END_C_LIB_DECL
  #define MB_CUR_MAX	((_Sizet)3)

 #else /* !_MULTI_THREAD || _COMPILER_TLS && !_GLOBAL_LOCALE */
  #define MB_CUR_MAX	((_Sizet)3)
 #endif /* !_MULTI_THREAD || _COMPILER_TLS && !_GLOBAL_LOCALE */

 #if _ILONG
  #define RAND_MAX	0x3fffffff

 #else /* _ILONG */
  #define RAND_MAX	0x7fff
 #endif /* _ILONG */

 #ifndef _NO_RETURN
  #define _NO_RETURN(fun)	void fun
 #endif /* _NO_RETURN */

#define CELL_SPU_LS_PARAM(heap_size, stack_size)	 \
__asm__(".pushsection .data\n"				 \
        ".global _cell_spu_ls_param\n"            	 \
        ".align 4\n"					 \
        ".type _cell_spu_ls_param, @object\n"		 \
        ".size _cell_spu_ls_param, 16\n"		 \
        "_cell_spu_ls_param:\n"				 \
        ".long " #heap_size ", " #stack_size " , 0, 0\n" \
        ".popsection\n"					 \
        ".pushsection .cell_spu_ls_param\n"		 \
        ".long " #heap_size ", " #stack_size "\n"	 \
        ".popsection\n"					 \
);

		/* TYPE DEFINITIONS */

#ifndef _WCHART
 #define _WCHART
typedef _Wchart wchar_t;
#endif /* _WCHART */

typedef struct
	{	/* result of int divide */
	int quot;
	int rem;
	} div_t;

typedef struct
	{	/* result of long divide */
	long quot;
	long rem;
	} ldiv_t;

 #ifndef _LLDIV_T
typedef struct
	{	/* result of long long divide */
	_Longlong quot;
	_Longlong rem;
	} _Lldiv_t;
 #endif /* _LLDIV_T */

 #if _HAS_C9X
typedef _Lldiv_t lldiv_t;
 #endif /* _IS_C9X */

		/* DECLARATIONS */
_EXTERN_C /* low-level functions */
_NO_RETURN(exit(int));
_NO_RETURN(_Exit(int));	/* added with C99 */
_END_EXTERN_C

_C_LIB_DECL
int abs(int);
long labs(long);
_NO_RETURN(abort(void));
void *calloc(size_t, size_t);
div_t div(int, int);
void free(void *);
ldiv_t ldiv(long, long);
void *malloc(size_t);
void *(memalign)(size_t boundary, size_t size_arg);  /* !!!GAC added nonstandard memalign*/
int mblen(const char *, size_t);
size_t mbstowcs(wchar_t *_Restrict,
	const char *_Restrict, size_t);
int mbtowc(wchar_t *_Restrict, const char *_Restrict, size_t);
int rand(void);
void srand(unsigned int);
void *realloc(void *, size_t);
void *reallocalign(void *, size_t size_arg, size_t boundary);
long strtol(const char *_Restrict, char **_Restrict, int);
size_t wcstombs(char *_Restrict,
	const wchar_t *_Restrict, size_t);
int wctomb(char *, wchar_t);

unsigned long _Stoul(const char *, char **, int);
float _Stof(const char *, char **, long);
double _Stod(const char *, char **, long);
long double _Stold(const char *, char **, long);
_Longlong _Stoll(const char *, char **, int);
_ULonglong _Stoull(const char *, char **, int);

//size_t _Getmbcurmax(void);

 #if _HAS_C9X
_Longlong llabs(_Longlong);
lldiv_t lldiv(_Longlong, _Longlong);
 #endif /* _IS_C9X */

_END_C_LIB_DECL

 #if defined(__cplusplus) && !defined(_NO_CPP_INLINES)
_EXTERN_C
typedef void _Atexfun(void);
typedef int _Cmpfun(const void *, const void *);

int atexit(void (*)(void));
void *bsearch(const void *, const void *,
	size_t, size_t, _Cmpfun *);
void qsort(void *, size_t, size_t, _Cmpfun *);
_END_EXTERN_C

  #if defined(_HAS_STRICT_LINKAGE)
typedef int _Cmpfun2(const void *, const void *);

inline int atexit(void (*_Pfn)(void))
	{	// register a function to call at exit
	return (atexit((_Atexfun *)_Pfn));
	}

inline void *bsearch(const void *_Key, const void *_Base,
	size_t _Nelem, size_t _Size, _Cmpfun2 *_Cmp)
	{	// search by binary chop
	return (bsearch(_Key, _Base, _Nelem, _Size, (_Cmpfun *)_Cmp));
	}

inline void qsort(void *_Base,
	size_t _Nelem, size_t _Size, _Cmpfun2 *_Cmp)
	{	// sort
	qsort(_Base, _Nelem, _Size, (_Cmpfun *)_Cmp);
	}
  #endif /* _HAS_STRICT_LINKAGE */

		// INLINES, FOR C++
_C_LIB_DECL
inline double atof(const char *_Str)
	{	// convert string to double
	return (_Stod(_Str, 0, 0));
	}

inline float atoff(const char *_Str)
	{	// convert string to float
	return (_Stof(_Str, 0, 0));
	}

inline int atoi(const char *_Str)
	{	// convert string to int
	return ((int)_Stoul(_Str, 0, 10));
	}

inline long atol(const char *_Str)
	{	// convert string to long
	return ((long)_Stoul(_Str, 0, 10));
	}

 #if defined(__APPLE__) && 3 <= __GNUC__	/* compiler test */
double strtod(const char *_Restrict,
	char **_Restrict);
unsigned long strtoul(const char *_Restrict,
	char **_Restrict, int);

 #else /* defined(__APPLE__) && 3 <= __GNUC__ */
inline double strtod(const char *_Restrict _Str,
	char **_Restrict _Endptr)
	{	// convert string to double, with checking
	return (_Stod(_Str, _Endptr, 0));
	}

inline unsigned long strtoul(const char *_Restrict _Str,
	char **_Restrict _Endptr, int _Base)
	{	// convert string to unsigned long, with checking
	return (_Stoul(_Str, _Endptr, _Base));
	}
 #endif /* defined(__APPLE__) && 3 <= __GNUC__ */

 #if _HAS_C9X
inline _Longlong atoll(const char *_Str)
	{	// convert string to long long
	return ((_Longlong)_Stoull(_Str, 0, 10));
	}

inline float strtof(const char *_Restrict _Str,
	char **_Restrict _Endptr)
	{	// convert string to float, with checking
	return (_Stof(_Str, _Endptr, 0));
	}

inline long double strtold(const char *_Restrict _Str,
	char **_Restrict _Endptr)
	{	// convert string to long double, with checking
	return (_Stold(_Str, _Endptr, 0));
	}

inline _Longlong strtoll(const char *_Restrict _Str,
	char **_Restrict _Endptr, int _Base)
	{	// convert string to long long, with checking
	return (_Stoll(_Str, _Endptr, _Base));
	}

inline _ULonglong strtoull(const char *_Restrict _Str,
	char **_Restrict _Endptr, int _Base)
	{	// convert string to unsigned long long, with checking
	return (_Stoull(_Str, _Endptr, _Base));
	}
 #endif /* _IS_C9X */

_END_C_LIB_DECL

 	// OVERLOADS, FOR C++

  #if !defined(_LLDIV_T) || defined(_LONGLONG)          
inline long abs(long _Left)                             
   {  // compute abs                                    
   return (labs(_Left));                                
   }                                                    
                                                        
inline ldiv_t div(long _Left, long _Right)              
   {  // compute quotient and remainder                 
   return (ldiv(_Left, _Right));                        
   }                                                    
  #endif /* !defined(_LLDIV_T) || defined(_LONGLONG) */ 
                                                        
 #if _HAS_C9X                                           
                                                        
  #if !defined(_LLDIV_T) && defined(_LONGLONG)          
inline _Longlong abs(_Longlong _Left)                   
   {  // compute abs                                    
   return (llabs(_Left));                               
   }                                                    
                                                        
inline _Lldiv_t div(_Longlong _Left, _Longlong _Right)  
   {  // compute quotient and remainder                 
   return (lldiv(_Left, _Right));                       
   }                                                    
  #endif /* !defined(_LLDIV_T) && defined(_LONGLONG) */ 
                                                        
 #endif /* _IS_C9X */                                   

 #else /* defined(__cplusplus) && !defined(_NO_CPP_INLINES) */
_C_LIB_DECL
		/* DECLARATIONS AND MACRO OVERRIDES, FOR C */
typedef int _Cmpfun(const void *, const void *);

/* */ int atexit(void (*)(void));
void *bsearch(const void *, const void *,
	size_t, size_t, _Cmpfun *);
void qsort(void *, size_t, size_t, _Cmpfun *);

double atof(const char *);
float atoff(const char *);
int atoi(const char *);
long atol(const char *);
double strtod(const char *_Restrict, char **_Restrict);
unsigned long strtoul(const char *_Restrict,
	char **_Restrict, int);

#define atof(str)	_Stod(str, 0, 0)
#define atoff(str)	_Stof(str, 0, 0)
#define atoi(str)	(int)_Stoul(str, 0, 10)
#define atol(str)	(long)_Stoul(str, 0, 10)
#define strtod(str, endptr)	_Stod(str, endptr, 0)
#define strtoul(str, endptr, base)	_Stoul(str, endptr, base)

 #if _HAS_C9X
_Longlong atoll(const char *);
float strtof(const char *_Restrict,
	char **_Restrict);
long double strtold(const char *_Restrict,
	char **_Restrict);
_Longlong strtoll(const char *_Restrict,
	char **_Restrict, int);
_ULonglong strtoull(const char *_Restrict,
	char **_Restrict, int);

#define atoll(str)	(_Longlong)_Stoull(str, 0, 10)
#define strtof(str, endptr)	_Stof(str, endptr, 0)
#define strtold(str, endptr)	_Stold(str, endptr, 0)
#define strtoll(str, endptr, base)	_Stoll(str, endptr, base)
#define strtoull(str, endptr, base)	_Stoull(str, endptr, base)
 #endif /* _IS_C9X */

_END_C_LIB_DECL
 #endif /* defined(__cplusplus) && !defined(_NO_CPP_INLINES) */

 #ifndef _LLDIV_T
  #define _LLDIV_T
 #endif /* _LLDIV_T */

_C_STD_END
#include <spu_intrinsics.h>
_C_STD_BEGIN
_C_LIB_DECL
_INLINE_DEF int get_state_TT800(void **state);
_INLINE_DEF void init_TT800(unsigned int s);
_INLINE_DEF void init_by_array_TT800(unsigned int init_key[], int key_length);
_INLINE_DEF unsigned int rand_int32_TT800(void);
_INLINE_DEF unsigned int rand_int31_TT800(void);
_INLINE_DEF float rand_real1_TT800(void);
_INLINE_DEF float rand_real2_TT800(void);
_INLINE_DEF float rand_real3_TT800(void);
_INLINE_DEF vector unsigned int rand_int32_TT800_i4(void);
_INLINE_DEF vector unsigned int rand_int31_TT800_i4(void);
_INLINE_DEF vector float rand_real1_TT800_f4(void);
_INLINE_DEF vector float rand_real2_TT800_f4(void);
_INLINE_DEF vector float rand_real3_TT800_f4(void);
#ifndef _NO_INLINE_DEFINITIONS
#include <bits/spec.h>
#include <bits/tt800.h>
#endif
_END_C_LIB_DECL

_C_STD_END
#endif /* _STDLIB */

#include <bits/sce_stdlib.h>
#include <bits/more_random.h>

 #if defined(_STD_USING)

using _CSTD div_t; using _CSTD ldiv_t;

using _CSTD abort; using _CSTD abs; using _CSTD atexit;
using _CSTD atof; using _CSTD atoi; using _CSTD atol;
using _CSTD bsearch; using _CSTD calloc; using _CSTD div;
using _CSTD exit; using _CSTD free; 
using _CSTD labs; using _CSTD ldiv; using _CSTD malloc;
using _CSTD mblen; using _CSTD mbstowcs; using _CSTD mbtowc;
using _CSTD qsort; using _CSTD rand; using _CSTD realloc;
using _CSTD srand; using _CSTD strtod; using _CSTD strtol;
using _CSTD strtoul; 
using _CSTD wcstombs; using _CSTD wctomb;
using _CSTD memalign;  /* !!!GAC added nonstandard memalign*/
using _CSTD atoff;
using _CSTD reallocalign;

 #if _HAS_C9X
using _CSTD lldiv_t;

using _CSTD atoll; using _CSTD llabs; using _CSTD lldiv;
using _CSTD strtof; using _CSTD strtold;
using _CSTD strtoll; using _CSTD strtoull;
using _CSTD _Exit;
 #endif /* _IS_C9X */

using _CSTD get_state_TT800; using _CSTD init_TT800; using _CSTD init_by_array_TT800;
using _CSTD rand_int32_TT800; using _CSTD rand_int31_TT800;
using _CSTD rand_real1_TT800; using _CSTD rand_real2_TT800; using _CSTD rand_real3_TT800;
using _CSTD rand_int32_TT800_i4; using _CSTD rand_int31_TT800_i4;
using _CSTD rand_real1_TT800_f4; using _CSTD rand_real2_TT800_f4; using _CSTD rand_real3_TT800_f4;

 #endif /* defined(_STD_USING) */

/*
 * Copyright (c) 1992-2003 by P.J. Plauger.  ALL RIGHTS RESERVED.
 * Consult your license regarding permissions and restrictions.
V4.02:0216 */
