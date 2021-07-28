/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2006 Sony Computer Entertainment Inc. 
* All Rights Reserved.
*/
/* yvals.h values header for conforming compilers on various systems */
#ifndef _YVALS
#define _YVALS
#include <sys/types.h> /* reffer definition of size_t */

#ifndef __EDG__
#  define __EDG__ 0
#endif
#define _IS_EMBEDDED 0
#define _ALT_NS 0
#define _WIN32_C_LIB 0
#define _POSIX_C_LIB 1
#ifdef __cplusplus
#define __STDC_VERSION__ 0
#endif
#define _HAS_C9X_FAST_FMA 0
#define _HAS_C9X_IMAGINARY_TYPE 0
#define _FEVAL 0

#include <stdarg.h>


#define _CPPLIB_VER	402

/* You can predefine (on the compile command line, for example):

_ALT_NS=1 -- to use namespace _Dinkum_std for C++
_ALT_NS=2 -- to use namespace _Dinkum_std for C++ and C
_C_AS_CPP -- to compile C library as C++
_C_IN_NS -- to define C names in std/_Dinkum_std instead of global namespace
_C99 -- to turn ON C99 library support
_NO_EX -- to turn OFF use of try/throw
_NO_MT -- to turn OFF thread synchronization
_NO_NS -- to turn OFF use of namespace declarations
_STL_DB (or _STLP_DEBUG) -- to turn ON iterator/range debugging
__NO_LONG_LONG -- to define _Longlong as long, not long long

You can change (in this header):

_COMPILER_TLS -- from 0 to 1 if _TLS_QUAL is not nil
_EXFAIL -- from 1 to any nonzero value for EXIT_FAILURE
_FILE_OP_LOCKS -- from 0 to 1 for file atomic locks
_GLOBAL_LOCALE -- from 0 to 1 for shared locales instead of per-thread
_HAS_IMMUTABLE_SETS -- from 1 to 0 to permit alterable set elements
_HAS_STRICT_CONFORMANCE -- from 0 to 1 to disable nonconforming extensions
_HAS_TRADITIONAL_IOSTREAMS -- from 1 to 0 to omit old iostreams functions
_HAS_TRADITIONAL_ITERATORS -- from 0 to 1 for vector/string pointer iterators
_HAS_TRADITIONAL_POS_TYPE -- from 0 to 1 for streampos same as streamoff
_HAS_TRADITIONAL_STL -- from 1 to 0 to omit old STL functions
_IOSTREAM_OP_LOCKS -- from 0 to 1 for iostream atomic locks
_TLS_QUAL -- from nil to compiler TLS qualifier, such as __declspec(thread)
_USE_EXISTING_SYSTEM_NAMES -- from 1 to 0 to disable mappings (_Open to open)

Include directories needed to compile with Dinkum C:

C -- include/c
C99 -- include/c (define _C99)
Standard C++ -- include/c include

Include directories needed to compile with native C:

C -- none
C99 -- N/A
Standard C++ -- include
 */

 /* Dinkumware configuration defines for ppu-lv2.  These are dinkumware
    implementation details, so we set them here, rather than having the
    compiler set them. */
 #undef _C99
 #define _C99	1
 #ifndef _C_IN_GLOBAL
 /* We set _C_IN_NS so it compliant with C++ standard.  An end user
    can prevent it from being set by setting _C_IN_GLOBAL. */
 #define _C_IN_NS 1
 #endif


 #ifndef __STDC_HOSTED__
  #define __STDC_HOSTED__	1
 #endif /* __STDC_HOSTED__ */

 #ifndef __STDC_IEC_559__
  #define __STDC_IEC_559__	1
 #endif /* __STDC_IEC_559__ */

 #ifndef __STDC_IEC_559_COMPLEX__
  #define __STDC_IEC_559_COMPLEX__	1
 #endif /* __STDC_IEC_559_COMPLEX__ */

 #ifndef __STDC_ISO_10646__
  #define __STDC_ISO_10646__	200009L	/* match glibc */
 #endif /* __STDC_ISO_10646__ */

		/* DETERMINE MACHINE TYPE */

 #if defined(__PPC__) || defined(__PPC64__)
  #define _D0		0	/* 0: big endian, 3: little endian floating-point */
  #define _DLONG	0	/* 0: 64, 1: 80, 2: 128 long double bits */
  #define _LBIAS	0x3fe	/* 64 long double bits */
  #define _LOFF		4	/* 64 long double bits */
  #define _FPP_TYPE	_FPP_PPC	/* PowerPC FPP */
  #define _MACH_I32	int

  #ifdef __LP32__
   #define _MACH_PDT	int
   #define _MACH_SZT	unsigned int	/* unused */
   #define _INTPTR	0	/* 0 => int, 1 => long, 2 => long long */
  #else
   #define _MACH_PDT	long
   #define _MACH_SZT	unsigned long	/* unused */
   #define _INTPTR	1	/* 0 => int, 1 => long, 2 => long long */
  #endif

 #else /* system detector */
/* #error unknown compilation environment, guess big-endian */
  #define _D0		0	/* 0: big endian, 3: little endian floating-point */
  #define _DLONG	0	/* 0: 64, 1: 80, 2: 128 long double bits */
  #define _LBIAS	0x3fe	/* 64 long double bits */
  #define _LOFF		4	/* 64 long double bits */
 #endif /* system detector */

		/* DETERMINE _Ptrdifft AND _Sizet FROM MACHINE TYPE */

 #if defined(_MACH_I32)
typedef _MACH_I32 _Int32t;
typedef unsigned _MACH_I32 _Uint32t;

 #else /* defined(_MACH_I32) */
typedef long _Int32t;
typedef unsigned long _Uint32t;
 #endif /* defined(_MACH_I32) */

 #if defined(_MACH_PDT)
typedef _MACH_PDT _Ptrdifft;

 #else /* defined(_MACH_PDT) */
typedef int _Ptrdifft;
 #endif /* defined(_MACH_PDT) */

 #if defined(_MACH_SZT)
typedef _MACH_SZT _Sizet;

 #else /* defined(_MACH_SZT) */
typedef unsigned int _Sizet;
 #endif /* defined(_MACH_SZT) */

 #if !defined(_HAS_C9X) && defined(_C99)
  #define _HAS_C9X	1
 #endif /* !defined(_HAS_C9X) etc. */

		/* EXCEPTION CONTROL */
 #ifndef _HAS_EXCEPTIONS
  #ifndef _NO_EX	/* don't simplify */
   #define _HAS_EXCEPTIONS	1	/* 1 for try/throw logic */

  #else	/* _NO_EX */
   #define _HAS_EXCEPTIONS	0
  #endif /* _NO_EX */

 #endif /* _HAS_EXCEPTIONS */

		/* NAMING PROPERTIES */
/* #define _STD_LINKAGE	defines C names as extern "C++" */
/* #define _STD_USING	defines C names in namespace std or _Dinkum_std */

 #ifndef _HAS_NAMESPACE
  #ifndef _NO_NS	/* don't simplify */
   #define _HAS_NAMESPACE	1	/* 1 for C++ names in std */

  #else	/* _NO_NS */
   #define _HAS_NAMESPACE	0
  #endif /* _NO_NS */

 #endif /* _HAS_NAMESPACE */

 #if !defined(_STD_USING) && defined(__cplusplus) \
	&& (defined(_C_IN_NS) || 1 < _ALT_NS)
//  SN_TARGET_PS3	
  #define _STD_USING	/* exports C names to global, else reversed */

 #elif defined(_STD_USING) && !defined(__cplusplus)
  #undef _STD_USING	/* define only for C++ */
 #endif /* !defined(_STD_USING) */

		/* THREAD AND LOCALE CONTROL */
 #ifndef _MULTI_THREAD

  #ifndef _NO_MT
   #define _MULTI_THREAD	1	/* 0 for no thread locks */
  #else
   #define _MULTI_THREAD	0
  #endif	/* _NO_MT */

 #endif /* _MULTI_THREAD */

#define _GLOBAL_LOCALE	0	/* 0 for per-thread locales, 1 for shared */

/* LV2-MODIFIED */
#define _FILE_OP_LOCKS	1	/* 0 for no FILE locks, 1 for atomic */
#define _IOSTREAM_OP_LOCKS	1	/* 0 for no iostream locks, 1 for atomic */
/* LV2-MODIFIED END */

		/* THREAD-LOCAL STORAGE */
#ifdef __PME__
#define _COMPILER_TLS	0	/* 1 if compiler supports TLS directly */
#define _TLS_QUAL	/* TLS qualifier, such as __declspec(thread), if any */
#else // __PME__
#define _COMPILER_TLS	1	/* 1 if compiler supports TLS directly */
#define _TLS_QUAL __thread	/* TLS qualifier, such as __declspec(thread), if any */
#endif // __PME__

 #define _HAS_IMMUTABLE_SETS	1
 #define _HAS_IMMUTABLE_SETS	1
 #define _HAS_TRADITIONAL_IOSTREAMS	0
 #define _HAS_TRADITIONAL_ITERATORS	0
 #define _HAS_TRADITIONAL_POS_TYPE	0
 #define _HAS_TRADITIONAL_STL	0

 #define _USE_EXISTING_SYSTEM_NAMES	1 /* _Open => open etc. */

 #if !defined(_HAS_STRICT_CONFORMANCE)
  #define _HAS_STRICT_CONFORMANCE	0	/* enable nonconforming extensions */
 #endif /* !defined(_HAS_STRICT_CONFORMANCE) */

 #if !defined(_HAS_ITERATOR_DEBUGGING) \
	&& (defined(_STL_DB) || defined(_STLP_DEBUG))
  #define _HAS_ITERATOR_DEBUGGING	1	/* for range checks, etc. */
 #else
  #define _HAS_ITERATOR_DEBUGGING       0
 #endif /* define _HAS_ITERATOR_DEBUGGING */

		/* NAMESPACE CONTROL */

 #if defined(__cplusplus)

 #if _HAS_NAMESPACE
namespace std {}

 #if defined(_C_AS_CPP)
  #define _NO_CPP_INLINES	/* just for compiling C library as C++ */
 #endif /* _C_AS_CPP */

 #if 0 < _ALT_NS

  #if defined(_C_AS_CPP)	/* define library in _Dinkum_std */
   #define _STD_BEGIN	namespace _Dinkum_std {_C_LIB_DECL
   #define _STD_END		_END_C_LIB_DECL }

  #else /* _C_AS_CPP */
   #define _STD_BEGIN	namespace _Dinkum_std {
   #define _STD_END		}
  #endif /* _C_AS_CPP */

  #if _ALT_NS == 1	/* define C library in global namespace */
    #define _C_STD_BEGIN
    #define _C_STD_END
    #define _CSTD		::
    #define _STD			::_Dinkum_std::

  #else	/* define both C and C++ in namespace _Dinkum_std */
    #define _C_STD_BEGIN	namespace _Dinkum_std {
    #define _C_STD_END	}
    #define _CSTD		::_Dinkum_std::
    #define _STD			::_Dinkum_std::
  #endif /* _ALT_NS */

namespace _Dinkum_std {}
namespace std {
	using namespace _Dinkum_std;
		}

 #elif defined(_STD_USING)

  #if defined(_C_AS_CPP)	/* define library in std */
   #define _STD_BEGIN	namespace std {_C_LIB_DECL
   #define _STD_END		_END_C_LIB_DECL }

  #else /* _C_AS_CPP */
   #define _STD_BEGIN	namespace std {
   #define _STD_END		}
  #endif /* _C_AS_CPP */

   #define _C_STD_BEGIN	namespace std {
   #define _C_STD_END	}
   #define _CSTD		::std::
   #define _STD			::std::

 #else /* _ALT_NS == 0 && !defined(_STD_USING) */

  #if defined(_C_AS_CPP)	/* define C++ library in std, C in global */
   #define _STD_BEGIN	_C_LIB_DECL
   #define _STD_END		_END_C_LIB_DECL

  #else /* _C_AS_CPP */
   #define _STD_BEGIN	namespace std {
   #define _STD_END		}
  #endif /* _C_AS_CPP */

   #define _C_STD_BEGIN	
   #define _C_STD_END	
   #define _CSTD		::
   #define _STD			::std::
 #endif /* _ALT_NS etc */

  #define _X_STD_BEGIN	namespace std {
  #define _X_STD_END	}
  #define _XSTD			::std::

  #if defined(_STD_USING)
   #undef _GLOBAL_USING		/* c* in std namespace, *.h imports to global */

  #else
   #define _GLOBAL_USING	/* *.h in global namespace, c* imports to std */
  #endif /* defined(_STD_USING) */

  #if defined(_STD_LINKAGE)
   #define _C_LIB_DECL		extern "C++" {	/* C has extern "C++" linkage */

  #else /* defined(_STD_LINKAGE) */
   #define _C_LIB_DECL		extern "C" {	/* C has extern "C" linkage */
  #endif /* defined(_STD_LINKAGE) */

  #define _END_C_LIB_DECL	}
  #define _EXTERN_C			extern "C" {
  #define _END_EXTERN_C		}

 #else /* _HAS_NAMESPACE */
  #define _STD_BEGIN
  #define _STD_END
  #define _STD	::

  #define _X_STD_BEGIN
  #define _X_STD_END
  #define _XSTD	::

  #define _C_STD_BEGIN
  #define _C_STD_END
  #define _CSTD	::

  #define _C_LIB_DECL		extern "C" {
  #define _END_C_LIB_DECL	}
  #define _EXTERN_C			extern "C" {
  #define _END_EXTERN_C		}
 #endif /* _HAS_NAMESPACE */

 #else /* __cplusplus */
  #define _STD_BEGIN
  #define _STD_END
  #define _STD

  #define _X_STD_BEGIN
  #define _X_STD_END
  #define _XSTD

  #define _C_STD_BEGIN
  #define _C_STD_END
  #define _CSTD

  #define _C_LIB_DECL
  #define _END_C_LIB_DECL
  #define _EXTERN_C
  #define _END_EXTERN_C
 #endif /* __cplusplus */

/* Inline definitions are only supported in C99 or C++. */
 #if defined(_NO_INLINE_DEFINITIONS)
  #define _INLINE_DEF
 #elif 199901L <= __STDC_VERSION__ || defined(__cplusplus)
  #ifdef __GNUC__
     /* GCC doesn't handle C99 inline definitions correctly, but will
      * give the same behaviour if we explicitly say 'extern'. */
   #define _INLINE_DEF extern inline
  #else
   #define _INLINE_DEF inline
  #endif
 #elif defined(__SNC__)
   #define _INLINE_DEF extern inline
 #else
  #define _INLINE_DEF 
  #define _NO_INLINE_DEFINITIONS
 #endif

 #if 199901L <= __STDC_VERSION__

 #if defined(__cplusplus)
  #if defined(__GNUC__) 
   #define _Restrict __restrict
  #else
   #define _Restrict
  #endif

 #else /* defined(__cplusplus) */
  #define _Restrict restrict
 #endif /* defined(__cplusplus) */

 #if defined(__GNUC__) && defined(__cplusplus)
  #define _C99_float_complex	float __complex__
  #define _C99_double_complex	double __complex__
  #define _C99_ldouble_complex	long double __complex__
 #endif /* defined(__GNUC__) && defined(__cplusplus) */

 #else /* 199901L <= __STDC_VERSION__ */
 #define _Restrict
 #endif /* 199901L <= __STDC_VERSION__ */

 #ifdef __cplusplus
_STD_BEGIN
typedef bool _Bool;
_STD_END
 #endif /* __cplusplus */

		/* VC++ COMPILER PARAMETERS */
 #define _CRTIMP
 #define _CDECL

 #ifndef __NO_LONG_LONG
  #define _LONGLONG	long long
  #define _ULONGLONG	unsigned long long
  #define _LLONG_MAX	0x7fffffffffffffffLL
  #define _ULLONG_MAX	0xffffffffffffffffULL
 #endif /* __NO_LONG_LONG */

_C_STD_BEGIN
		/* FLOATING-POINT PROPERTIES */
#define _DBIAS	0x3fe	/* IEEE format double and float */
#define _DOFF	4
#define _FBIAS	0x7e
#define _FOFF	7
#define _FRND	1

		/* INTEGER PROPERTIES */
#define _BITS_BYTE	8
#define _C2			1	/* 0 if not 2's complement */
#define _MBMAX		3	/* MB_LEN_MAX */
#define _ILONG		1	/* 0 if 16-bit int */

 #if defined(__CHAR_UNSIGNED__) || defined(_CHAR_UNSIGNED)
  #define _CSIGN	0	/* 0 if char is not signed */
 #else
  #define _CSIGN	1
 #endif

#define _MAX_EXP_DIG	8	/* for parsing numerics */
#define _MAX_INT_DIG	32
#define _MAX_SIG_DIG	36

 #if defined(_LONGLONG)
typedef _LONGLONG _Longlong;
typedef _ULONGLONG _ULonglong;

 #else /* defined(_LONGLONG) */
typedef long _Longlong;
typedef unsigned long _ULonglong;
 #define _LLONG_MAX		0x7fffffffL
 #define _ULLONG_MAX	0xffffffffUL
 #endif /* defined(_LONGLONG) */

		/* wchar_t AND wint_t PROPERTIES */

 #ifdef __cplusplus
  #define _WCHART
typedef wchar_t _Wchart;
 #endif /* __cplusplus */

 #if defined(__WCHAR_TYPE__) && defined(__WINT_TYPE__)
  #define _WCMIN	0x0
  #define _WCMAX	0xffff	// assume unsigned 16-bit wchar_t
  #define _WIMIN	(-_WIMAX - _C2)
  #define _WIMAX	0x7fffffff

  #ifndef __cplusplus
typedef __WCHAR_TYPE__ _Wchart;
  #endif /* __cplusplus */
typedef __WINT_TYPE__ _Wintt;

 #elif defined(__SNC__)
 /* Values for the SN Systems SNC for PS3 compiler. */
  #define _WCMIN	0x0
  #define _WCMAX	0xffff
  #define _WIMIN	(-_WIMAX - _C2)
  #define _WIMAX	0x7fffffff
  

  #ifndef __cplusplus
typedef unsigned short _Wchart;
  #endif /* __cplusplus */
typedef int _Wintt;

 #else /* default wchar_t/wint_t */
  #define _WCMIN	(-_WCMAX - _C2)
  #define _WCMAX	0x7fffffff
  #define _WIMIN	(-_WIMAX - _C2)
  #define _WIMAX	0x7fffffff

  #ifndef __cplusplus
typedef long _Wchart;
  #endif /* __cplusplus */
typedef long _Wintt;

 #endif /* compiler/library type */

		/* POINTER PROPERTIES */
#define _NULL		0	/* 0L if pointer same as long */

		/* signal PROPERTIES */

#define _SIGABRT	6
#define _SIGMAX		44

		/* stdarg PROPERTIES */
typedef va_list _Va_list;

 #if _HAS_C9X

 #ifndef va_copy
_EXTERN_C
void _Vacopy(va_list *, va_list);
_END_EXTERN_C
  #define va_copy(apd, aps)	_Vacopy(&(apd), aps)
 #endif /* va_copy */

 #endif /* _IS_C9X */

		/* stdlib PROPERTIES */
#define _EXFAIL	1	/* EXIT_FAILURE */

_EXTERN_C
void _Atexit(void (*)(void));
_END_EXTERN_C

		/* stdio PROPERTIES */
#define _FNAMAX	260
#define _FOPMAX	256
#define _TNAMAX	16

 #define _FD_TYPE	signed short
  #define _FD_NO(str) ((str)->_Handle)
 #define _FD_VALID(fd)	(0 <= (fd))	/* fd is signed integer */
 #define _FD_INVALID	(-1)
 #define _SYSCH(x)	x
typedef char _Sysch_t;

		/* STORAGE ALIGNMENT PROPERTIES */
#define _MEMBND	4U /* sixteen-byte boundaries (2^^4) */

		/* time PROPERTIES */
/** LV2_MODIFIED **/
#define _CPS	1000000
#define _TBIAS	((70 * 365LU + 17) * 86400)
_C_STD_END

_EXTERN_C
#ifdef LV2_MODIFIED
#include <sys/synchronization.h>
typedef struct {
  unsigned int cnt;
  sys_ppu_thread_t owner;
  sys_lwmutex_t mtx;
} _Rmtx;
#else
typedef void *_Rmtx;
#endif
_END_EXTERN_C

		/* MULTITHREAD PROPERTIES */

 #if _MULTI_THREAD
_EXTERN_C
void _Locksyslock(int);
void _Unlocksyslock(int);
_END_EXTERN_C

 #else /* _MULTI_THREAD */
  #define _Locksyslock(x)	(void)0
  #define _Unlocksyslock(x)	(void)0
 #endif /* _MULTI_THREAD */

		/* LOCK MACROS */
 #define _LOCK_LOCALE	0
 #define _LOCK_MALLOC	1
 #define _LOCK_STREAM	2
 #define _LOCK_DEBUG	3
#ifdef __PME__
 #define _MAX_LOCK	4	/* one more than highest lock number */
#else // __PME__
 #define _LOCK_FS_FD    4
 #define _MAX_LOCK      5
#endif // __PME__

 #if _IOSTREAM_OP_LOCKS
  #define _MAYBE_LOCK

 #else /* _IOSTREAM_OP_LOCKS */
  #define _MAYBE_LOCK	\
	if (_Locktype == _LOCK_MALLOC || _Locktype == _LOCK_DEBUG)
 #endif /* _IOSTREAM_OP_LOCKS */

 #ifdef __cplusplus
_STD_BEGIN
extern "C++" {	// in case of _C_AS_CPP
		// CLASS _Lockit
class _Lockit
	{	// lock while object in existence -- MUST NEST
public:

  #if !_MULTI_THREAD
   #define _LOCKIT(x)

	explicit _Lockit()
		{	// do nothing
		}

	explicit _Lockit(int)
		{	// do nothing
		}

	~_Lockit()
		{	// do nothing
		}

  #else /* non-Windows multithreading */
   #define _LOCKIT(x)	lockit x

	explicit _Lockit()
		: _Locktype(_LOCK_MALLOC)
		{	// set default lock
		_MAYBE_LOCK
			_Locksyslock(_Locktype);
		}

	explicit _Lockit(int _Type)
		: _Locktype(_Type)
		{	// set the lock
		_MAYBE_LOCK
			_Locksyslock(_Locktype);
		}

	~_Lockit()
		{	// clear the lock
		_MAYBE_LOCK
			_Unlocksyslock(_Locktype);
		}

private:
	int _Locktype;
public:
	_Lockit(const _Lockit&);			// not defined
	_Lockit& operator=(const _Lockit&);	// not defined
  #endif /* _MULTI_THREAD */

	};

class _Mutex
	{	// lock under program control
public:

  #if !_MULTI_THREAD || !_IOSTREAM_OP_LOCKS
    void _Lock()
		{	// do nothing
		}

	void _Unlock()
		{	// do nothing
	}

  #else /* !_MULTI_THREAD || !_IOSTREAM_OP_LOCKS */
	_Mutex();
	~_Mutex();
	void _Lock();
	void _Unlock();

private:
	_Mutex(const _Mutex&);				// not defined
	_Mutex& operator=(const _Mutex&);	// not defined
	_Rmtx *_Mtx;
  #endif /* !_MULTI_THREAD || !_IOSTREAM_OP_LOCKS */

	};
}	// extern "C++"
_STD_END
 #endif /* __cplusplus */

		/* MISCELLANEOUS MACROS */
#define _ATEXIT_T	void

#define _MAX	max
#define _MIN	min

#define _TEMPLATE_STAT

 #if 0 < __GNUC__
  #define _NO_RETURN(fun)	void fun __attribute__((__noreturn__))
 #else /* compiler selector */
  #define _NO_RETURN(fun)	void fun
 #endif /* compiler selector */

 #if _HAS_NAMESPACE

 #ifdef __cplusplus

  #if defined(_STD_USING)
_STD_BEGIN
using ::va_list;
_STD_END
  #endif /* !defined(_C_AS_CPP) && defined(_GLOBAL_USING) */

 #endif /* __cplusplus */
 #endif /* _HAS_NAMESPACE */

#ifdef __PME__
#define OPEN_MAX (20UL)
#define PATH_MAX (512UL)
#define NAME_MAX (255UL)
#define PAGE_SIZE (4096UL)
#define ARG_MAX  (4096UL)
#define BUFSIZ          (4*1024)
#endif

#endif /* _YVALS */

/*
 * Copyright (c) 1992-2003 by P.J. Plauger.  ALL RIGHTS RESERVED.
 * Consult your license regarding permissions and restrictions.
V4.02:0216 */
