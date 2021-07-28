/* SCE CONFIDENTIAL
* PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2006 Sony Computer Entertainment Inc.
* All Rights Reserved.
*/
/* ctype.h standard header */
#ifndef _CTYPE
#define _CTYPE
#ifndef _YVALS
 #include <yvals.h>
#endif /* _YVALS */

_C_STD_BEGIN

		/* _Ctype code bits */
#define _XB		0x400 /* extra blank */
#define _XA		0x200 /* extra alphabetic */
#define _XS		0x100 /* extra space */
#define _BB		0x80 /* BEL, BS, etc. */
#define _CN		0x40 /* CR, FF, HT, NL, VT */
#define _DI		0x20 /* '0'-'9' */
#define _LO		0x10 /* 'a'-'z' */
#define _PU		0x08 /* punctuation */
#define _SP		0x04 /* space */
#define _UP		0x02 /* 'A'-'Z' */
#define _XD		0x01 /* '0'-'9', 'A'-'F', 'a'-'f' */

_C_LIB_DECL
typedef const short *_Ctype_t;

extern const short *__ctype_ptr;

_Ctype_t _Getpctype(void);
_Ctype_t _Getptolower(void);
_Ctype_t _Getptoupper(void);

 #if !_MULTI_THREAD || _COMPILER_TLS && !_GLOBAL_LOCALE
// for special vsh 
//extern _TLS_QUAL _Ctype_t _Ctype;
//extern _TLS_QUAL _Ctype_t _Tolotab;
//extern _TLS_QUAL _Ctype_t _Touptab;

extern _Ctype_t _Ctype;
extern _Ctype_t _Tolotab;
extern _Ctype_t _Touptab;

  #define _Getpctype()		_Ctype
  #define _Getptolower()	_Tolotab
  #define _Getptoupper()	_Touptab
 #endif /* !_MULTI_THREAD || _COMPILER_TLS && !_GLOBAL_LOCALE */

#define _Getchrtype(x)	_Getpctype()[(int)(x)]
#define _CTolower(x)	_Getptolower()[(int)(x)]
#define _CToupper(x)	_Getptoupper()[(int)(x)]
_END_C_LIB_DECL

 #if defined(__cplusplus) && !defined(_NO_CPP_INLINES)
		// INLINES, for C++
_C_LIB_DECL
inline int isalnum(int _Ch)
	{	// check alnum bits
	return ((__ctype_ptr)[_Ch] & (_DI|_LO|_UP|_XA));
	}

inline int isalpha(int _Ch)
	{	// check alpha bits
	return ((__ctype_ptr)[_Ch] & (_LO|_UP|_XA));
	}

inline int iscntrl(int _Ch)
	{	// check cntrl bits
	return ((__ctype_ptr)[_Ch] & _BB);
	}

inline int isdigit(int _Ch)
	{	// check digit bits
	return ((__ctype_ptr)[_Ch] & _DI);
	}

inline int isgraph(int _Ch)
	{	// check graph bits
	return ((__ctype_ptr)[_Ch] & (_DI|_LO|_PU|_UP|_XA));
	}

inline int islower(int _Ch)
	{	// check alnum bits
	return ((__ctype_ptr)[_Ch] & _LO);
	}

inline int isprint(int _Ch)
	{	// check print bits
	return ((__ctype_ptr)[_Ch] & (_DI|_LO|_PU|_SP|_UP|_XA));
	}

inline int ispunct(int _Ch)
	{	// check punct bits
	return ((__ctype_ptr)[_Ch] & _PU);
	}

inline int _Isspace(int _Ch)
	{	// check space bits
	return ((__ctype_ptr)[_Ch] & (_CN|_SP|_XS));
	}

inline int isspace(int _Ch)
	{	// check space bits
	return (_Isspace(_Ch));
	}

inline int isupper(int _Ch)
	{	// check upper bits
	return ((__ctype_ptr)[_Ch] & _UP);
	}

inline int isxdigit(int _Ch)
	{	// check xdigit bits
	return ((__ctype_ptr)[_Ch] & _XD);
	}

 #if _HAS_C9X
inline int isblank(int _Ch)
	{	// check blank bits
	return ((__ctype_ptr)[_Ch] & (_SP|_XB));
	}
 #endif /* _IS_C9X */

inline int tolower(int _Ch)
	{	// convert to lower case
	int __x=_Ch; 
	return (isupper(__x) ? (__x - 'A' + 'a') : __x);
	}

inline int toupper(int _Ch)
	{	// convert to upper case
	int __x=_Ch; 
	return (islower(__x) ? (__x - 'a' + 'A') : __x);
	}

inline int isalnum_ascii(int _Ch)
	{	// check alnum bits
	return ((__ctype_ptr)[_Ch] & (_DI|_LO|_UP|_XA));
	}

inline int isalpha_ascii(int _Ch)
	{	// check alpha bits
	return ((__ctype_ptr)[_Ch] & (_LO|_UP|_XA));
	}

inline int iscntrl_ascii(int _Ch)
	{	// check cntrl bits
	return ((__ctype_ptr)[_Ch] & _BB);
	}

inline int isdigit_ascii(int _Ch)
	{	// check digit bits
	return ((__ctype_ptr)[_Ch] & _DI);
	}

inline int isgraph_ascii(int _Ch)
	{	// check graph bits
	return ((__ctype_ptr)[_Ch] & (_DI|_LO|_PU|_UP|_XA));
	}

inline int islower_ascii(int _Ch)
	{	// check alnum bits
	return ((__ctype_ptr)[_Ch] & _LO);
	}

inline int isprint_ascii(int _Ch)
	{	// check print bits
	return ((__ctype_ptr)[_Ch] & (_DI|_LO|_PU|_SP|_UP|_XA));
	}

inline int ispunct_ascii(int _Ch)
	{	// check punct bits
	return ((__ctype_ptr)[_Ch] & _PU);
	}

inline int _Isspace_ascii(int _Ch)
	{	// check space bits
	return ((__ctype_ptr)[_Ch] & (_CN|_SP|_XS));
	}

inline int isspace_ascii(int _Ch)
	{	// check space bits
	return (_Isspace_ascii(_Ch));
	}

inline int isupper_ascii(int _Ch)
	{	// check upper bits
	return ((__ctype_ptr)[_Ch] & _UP);
	}

inline int isxdigit_ascii(int _Ch)
	{	// check xdigit bits
	return ((__ctype_ptr)[_Ch] & _XD);
	}

 #if _HAS_C9X
inline int isblank_ascii(int _Ch)
	{	// check blank bits
	return ((__ctype_ptr)[_Ch] & (_SP|_XB));
	}
 #endif /* _IS_C9X */

inline int tolower_ascii(int _Ch)
	{	// convert to lower case
	int __x=_Ch; 
	return (isupper(__x) ? (__x - 'A' + 'a') : __x);
	}

inline int toupper_ascii(int _Ch)
	{	// convert to upper case
	int __x=_Ch; 
	return (islower(__x) ? (__x - 'a' + 'A') : __x);
	}


_END_C_LIB_DECL

 #else /* defined(__cplusplus) && !defined(_NO_CPP_INLINES) */
		/* declarations and macro overrides, for C */
_C_LIB_DECL
int isalnum(int);
int isalpha(int);
int iscntrl(int);
int isdigit(int);
int isgraph(int);
int islower(int);
int isprint(int);
int ispunct(int);
int isspace(int);
int isupper(int);
int isxdigit(int);
int tolower(int);
int toupper(int);

 #if _HAS_C9X
int isblank(int);
 #endif /* _IS_C9X */

_END_C_LIB_DECL

  /** for compatibility **/
  #define isalnum_ascii(c)	isalnum(c)
  #define isalpha_ascii(c)	isalpha(c)
  #define iscntrl_ascii(c)	iscntrl(c)
  #define isdigit_ascii(c)	isdigit(c)
  #define isgraph_ascii(c)	isgraph(c)
  #define islower_ascii(c)	islower(c)
  #define isprint_ascii(c)	isprint(c)
  #define ispunct_ascii(c)	ispunct(c)
  #define isspace_ascii(c)	isspace(c)
  #define isupper_ascii(c)	isupper(c)
  #define isxdigit_ascii(c)	isxdigit(c)
  #define toupper_ascii(c)        toupper(c)
  #define tolower_ascii(c)        tolower(c)

  #define isalnum(c)	((__ctype_ptr)[(int)(c)] & (_DI|_LO|_UP|_XA))
  #define isalpha(c)	((__ctype_ptr)[(int)(c)] & (_LO|_UP|_XA))
  #define iscntrl(c)	((__ctype_ptr)[(int)(c)] & _BB)
  #define isdigit(c)	((__ctype_ptr)[(int)(c)] & _DI)
  #define isgraph(c)	((__ctype_ptr)[(int)(c)] & (_DI|_LO|_PU|_UP|_XA))
  #define islower(c)	((__ctype_ptr)[(int)(c)] & _LO)
  #define isprint(c)	((__ctype_ptr)[(int)(c)] & (_DI|_LO|_PU|_SP|_UP|_XA))
  #define ispunct(c)	((__ctype_ptr)[(int)(c)] & _PU)
  #define isspace(c)	((__ctype_ptr)[(int)(c)] & (_CN|_SP|_XS))
  #define isupper(c)	((__ctype_ptr)[(int)(c)] & _UP)
  #define isxdigit(c)	((__ctype_ptr)[(int)(c)] & _XD)

  #define tolower(c) \
  __extension__ ({int __x=(c); isupper(__x) ? ((__x) - 'A' + 'a') : __x;})
  #define toupper(c) \
  __extension__ ({int __x=(c); islower(__x) ? ((__x) - 'a' + 'A') : __x;})


 #if _HAS_C9X
  #define isblank_ascii(c)	isblank(c)
  #define isblank(c)	((__ctype_ptr)[(int)(c)] & (_SP|_XB))
 #endif /* _IS_C9X */

 #endif /* defined(__cplusplus) && !defined(_NO_CPP_INLINES) */

_C_STD_END
#endif /* _CTYPE */

#ifdef _STD_USING
using _CSTD isalnum; using _CSTD isalpha; using _CSTD iscntrl;
using _CSTD isdigit; using _CSTD isgraph; using _CSTD islower;
using _CSTD isprint; using _CSTD ispunct; using _CSTD isspace;
using _CSTD isupper; using _CSTD isxdigit; using _CSTD tolower;
using _CSTD toupper;
using _CSTD isalnum_ascii; using _CSTD isalpha_ascii; using _CSTD iscntrl_ascii;
using _CSTD isdigit_ascii; using _CSTD isgraph_ascii; using _CSTD islower_ascii;
using _CSTD isprint_ascii; using _CSTD ispunct_ascii; using _CSTD isspace_ascii;
using _CSTD isupper_ascii; using _CSTD isxdigit_ascii; using _CSTD tolower_ascii;
using _CSTD toupper_ascii;

 #if _HAS_C9X
using _CSTD isblank;
using _CSTD isblank_ascii;
 #endif /* _IS_C9X */

#endif /* _STD_USING */

/*
 * Copyright (c) 1992-2003 by P.J. Plauger.  ALL RIGHTS RESERVED.
 * Consult your license regarding permissions and restrictions.
V4.02:0216 */
