/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2004 Sony Computer Entertainment Inc. 
* All Rights Reserved.
*/
/* limits.h standard header -- 8-bit version */
#ifndef _LIMITS
#define _LIMITS
#ifndef _YVALS
 #include <yvals.h>
#endif /* _YVALS */

 #if _BITS_BYTE != 8
  #error <limits.h> assumes 8 bits per byte
 #endif /* _BITS_BYTE != 8 */

 #if defined(_LIMITS_H___)
  #undef MB_LEN_MAX
  #define MB_LEN_MAX	_MBMAX

 #else /* defined(_LIMITS_H___) */
  #define _LIMITS_H___
		/* CHAR PROPERTIES */
  #define CHAR_BIT	_BITS_BYTE

  #if _CSIGN
   #define CHAR_MAX	SCHAR_MAX
   #define CHAR_MIN	SCHAR_MIN

  #else /* _CSIGN */
   #define CHAR_MAX	UCHAR_MAX
   #define CHAR_MIN	0
  #endif /* _CSIGN */

		/* INT PROPERTIES */

  #if _ILONG
   #define INT_MAX		0x7fffffff
   #define INT_MIN		(-INT_MAX - _C2)
   #define UINT_MAX	(INT_MAX * 2U + 1)

  #else /* _ILONG */
   #define INT_MAX		SHRT_MAX
   #define INT_MIN		SHRT_MIN
   #define UINT_MAX	USHRT_MAX
  #endif /* _ILONG */

		/* MULTIBYTE PROPERTIES */
  #define MB_LEN_MAX	_MBMAX

		/* SIGNED CHAR PROPERTIES */
  #define SCHAR_MAX	0x7f
  #define SCHAR_MIN	(-SCHAR_MAX - _C2)

		/* SHORT PROPERTIES */
  #define SHRT_MAX	0x7fff
  #define SHRT_MIN	(-SHRT_MAX - _C2)

		/* LONG PROPERTIES */
#ifdef __powerpc64__
  #define LONG_MAX	0x7fffffffffffffffL
#else
  #define LONG_MAX	0x7fffffffL
#endif
  #define LONG_MIN	(-LONG_MAX - _C2)

		/* UNSIGNED PROPERTIES */
  #define UCHAR_MAX	0xff
  #define USHRT_MAX	0xffff
  #define ULONG_MAX	(LONG_MAX * 2UL + 1)
 #endif /* defined(_LIMITS_H___) */

 #if _HAS_C9X
#define LLONG_MAX	_LLONG_MAX
#define LLONG_MIN	(-_LLONG_MAX - _C2)
#define ULLONG_MAX	_ULLONG_MAX
 #endif /* _IS_C9X */

#endif /* _LIMITS */

/*
 * Copyright (c) 1992-2003 by P.J. Plauger.  ALL RIGHTS RESERVED.
 * Consult your license regarding permissions and restrictions.
V4.02:0216 */
