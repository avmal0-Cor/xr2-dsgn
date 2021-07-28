/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2006 Sony Computer Entertainment Inc. 
* All Rights Reserved.
*/
/* inttypes.h standard header */
#ifndef _INTTYPES
#define _INTTYPES
#include <stdint.h>

 #if _BITS_BYTE != 8
  #error <inttypes.h> assumes 8 bits per byte
 #endif /* _BITS_BYTE != 8 */

 #if _INTPTR == 0
  #define _PTR_FMT	""

 #elif _INTPTR == 1
  #define _PTR_FMT	"l"

 #else /* _INTPTR */
  #define _PTR_FMT	"ll"
 #endif /* _INTPTR */

_C_STD_BEGIN
_C_LIB_DECL
		/* TYPE DEFINITIONS */
 #ifndef _LLDIV_T
typedef struct
	{	/* result of long long divide */
	_Longlong quot;
	_Longlong rem;
	} _Lldiv_t;
 #endif /* _LLDIV_T */

typedef _Lldiv_t imaxdiv_t;

		/* FUNCTION DECLARATIONS */
intmax_t imaxabs(intmax_t);
imaxdiv_t imaxdiv(intmax_t, intmax_t);

intmax_t strtoimax(const char *_Restrict,
	char **_Restrict, int);
uintmax_t strtoumax(const char *_Restrict,
	char **_Restrict, int);
intmax_t wcstoimax(const _Wchart *_Restrict,
	_Wchart **_Restrict, int);
uintmax_t wcstoumax(const _Wchart *_Restrict,
	_Wchart **_Restrict, int);
_END_C_LIB_DECL

		/* PRINT FORMAT MACROS */
 #define PRId8	"hhd"
 #define PRId16	"hd"
 #define PRId32	"d"
 #define PRIdLEAST8	"hhd"
 #define PRIdLEAST16	"hd"
 #define PRIdLEAST32	"d"
 #define PRIdFAST8	"d"
 #define PRIdFAST16	"d"
 #define PRIdFAST32	"d"

 #define PRIi8	"hhi"
 #define PRIi16	"hi"
 #define PRIi32	"i"
 #define PRIiLEAST8	"hhi"
 #define PRIiLEAST16	"hi"
 #define PRIiLEAST32	"i"
 #define PRIiFAST8	"i"
 #define PRIiFAST16	"i"
 #define PRIiFAST32	"i"

 #define PRIo8	"hho"
 #define PRIo16	"ho"
 #define PRIo32	"o"
 #define PRIoLEAST8	"hho"
 #define PRIoLEAST16	"ho"
 #define PRIoLEAST32	"o"
 #define PRIoFAST8	"o"
 #define PRIoFAST16	"o"
 #define PRIoFAST32	"o"

 #define PRIu8	"hhu"
 #define PRIu16	"hu"
 #define PRIu32	"u"
 #define PRIuLEAST8	"hhu"
 #define PRIuLEAST16	"hu"
 #define PRIuLEAST32	"u"
 #define PRIuFAST8	"u"
 #define PRIuFAST16	"u"
 #define PRIuFAST32	"u"

 #define PRIx8	"hhx"
 #define PRIx16	"hx"
 #define PRIx32	"x"
 #define PRIxLEAST8	"hhx"
 #define PRIxLEAST16	"hx"
 #define PRIxLEAST32	"x"
 #define PRIxFAST8	"x"
 #define PRIxFAST16	"x"
 #define PRIxFAST32	"x"

 #define PRIX8	"hhX"
 #define PRIX16	"hX"
 #define PRIX32	"X"
 #define PRIXLEAST8	"hhX"
 #define PRIXLEAST16	"hX"
 #define PRIXLEAST32	"X"
 #define PRIXFAST8	"X"
 #define PRIXFAST16	"X"
 #define PRIXFAST32	"X"

 #define PRId64			"lld"
 #define PRIdLEAST64	"lld"
 #define PRIdFAST64	"lld"
 #define PRIdMAX	"lld"
 #define PRIdPTR	_PTR_FMT "d"

 #define PRIi64			"lli"
 #define PRIiLEAST64	"lli"
 #define PRIiFAST64	"lli"
 #define PRIiMAX	"lli"
 #define PRIiPTR	_PTR_FMT "i"

 #define PRIo64			"llo"
 #define PRIoLEAST64	"llo"
 #define PRIoFAST64	"llo"
 #define PRIoMAX	"llo"
 #define PRIoPTR	_PTR_FMT "o"

 #define PRIu64			"llu"
 #define PRIuLEAST64	"llu"
 #define PRIuFAST64	"llu"
 #define PRIuMAX	"llu"
 #define PRIuPTR	_PTR_FMT "u"

 #define PRIx64			"llx"
 #define PRIxLEAST64	"llx"
 #define PRIxFAST64	"llx"
 #define PRIxMAX	"llx"
 #define PRIxPTR	_PTR_FMT "x"

 #define PRIX64			"llX"
 #define PRIXLEAST64	"llX"
 #define PRIXFAST64	"llX"
 #define PRIXMAX	"llX"
 #define PRIXPTR	_PTR_FMT "X"

		/* SCAN FORMAT MACROS */
 #define SCNd8	"hhd"
 #define SCNd16	"hd"
 #define SCNd32	"d"
 #define SCNdLEAST8	"hhd"
 #define SCNdLEAST16	"hd"
 #define SCNdLEAST32	"d"
 #define SCNdFAST8	"d"
 #define SCNdFAST16	"d"
 #define SCNdFAST32	"d"

 #define SCNi8	"hhi"
 #define SCNi16	"hi"
 #define SCNi32	"i"
 #define SCNiLEAST8	"hhi"
 #define SCNiLEAST16	"hi"
 #define SCNiLEAST32	"i"
 #define SCNiFAST8	"i"
 #define SCNiFAST16	"i"
 #define SCNiFAST32	"i"

 #define SCNo8	"hho"
 #define SCNo16	"ho"
 #define SCNo32	"o"
 #define SCNoLEAST8	"hho"
 #define SCNoLEAST16	"ho"
 #define SCNoLEAST32	"o"
 #define SCNoFAST8	"o"
 #define SCNoFAST16	"o"
 #define SCNoFAST32	"o"

 #define SCNu8	"hhu"
 #define SCNu16	"hu"
 #define SCNu32	"u"
 #define SCNuLEAST8	"hhu"
 #define SCNuLEAST16	"hu"
 #define SCNuLEAST32	"u"
 #define SCNuFAST8	"u"
 #define SCNuFAST16	"u"
 #define SCNuFAST32	"u"

 #define SCNx8	"hhx"
 #define SCNx16	"hx"
 #define SCNx32	"x"
 #define SCNxLEAST8	"hhx"
 #define SCNxLEAST16	"hx"
 #define SCNxLEAST32	"x"
 #define SCNxFAST8	"x"
 #define SCNxFAST16	"x"
 #define SCNxFAST32	"x"

 #define SCNX8	"hhX"
 #define SCNX16	"hX"
 #define SCNX32	"X"
 #define SCNXLEAST8	"hhX"
 #define SCNXLEAST16	"hX"
 #define SCNXLEAST32	"X"
 #define SCNXFAST8	"X"
 #define SCNXFAST16	"X"
 #define SCNXFAST32	"X"

 #define SCNd64			"lld"
 #define SCNdLEAST64	"lld"
 #define SCNdFAST64	"lld"
 #define SCNdMAX	"lld"
 #define SCNdPTR	_PTR_FMT "d"

 #define SCNi64			"lli"
 #define SCNiLEAST64	"lli"
 #define SCNiFAST64	"lli"
 #define SCNiMAX	"lli"
 #define SCNiPTR	_PTR_FMT "i"

 #define SCNo64			"llo"
 #define SCNoLEAST64	"llo"
 #define SCNoFAST64	"llo"
 #define SCNoMAX	"llo"
 #define SCNoPTR	_PTR_FMT "o"

 #define SCNu64			"llu"
 #define SCNuLEAST64	"llu"
 #define SCNuFAST64	"llu"
 #define SCNuMAX	"llu"
 #define SCNuPTR	_PTR_FMT "u"

 #define SCNx64			"llx"
 #define SCNxLEAST64	"llx"
 #define SCNxFAST64	"llx"
 #define SCNxMAX	"llx"
 #define SCNxPTR	_PTR_FMT "x"

 #define SCNX64			"llX"
 #define SCNXLEAST64	"llX"
 #define SCNXFAST64	"llX"
 #define SCNXMAX	"llX"
 #define SCNXPTR	_PTR_FMT "X"

#ifdef __cplusplus
		// OVERLOADS, FOR C++
 #ifndef _LLDIV_T
inline intmax_t abs(intmax_t _Left)
	{	// compute abs
	return (imaxabs(_Left));
	}

inline imaxdiv_t div(intmax_t _Left, intmax_t _Right)
	{	// compute div
	return (imaxdiv(_Left, _Right));
	}
typedef _Lldiv_t imaxdiv_t;
 #endif /* _LLDIV_T */

#endif /* __cplusplus */

 #ifndef _LLDIV_T
  #define _LLDIV_T
 #endif /* _LLDIV_T */

_C_STD_END
#endif /* _INTTYPES */

#ifdef _STD_USING
using _CSTD imaxdiv_t;

using _CSTD abs; using _CSTD div;
using _CSTD imaxabs; using _CSTD imaxdiv;
using _CSTD strtoimax; using _CSTD strtoumax;
using _CSTD wcstoimax; using _CSTD wcstoumax;
#endif /* _STD_USING */

/*
 * Copyright (c) 1992-2003 by P.J. Plauger.  ALL RIGHTS RESERVED.
 * Consult your license regarding permissions and restrictions.
V4.02:0216 */
