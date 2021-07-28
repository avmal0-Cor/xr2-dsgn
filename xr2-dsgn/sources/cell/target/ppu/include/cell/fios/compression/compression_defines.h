/*
	SCE CONFIDENTIAL
	PlayStation(R)3 Programmer Tool Runtime Library 310.001
	Copyright (C) 2008 Sony Computer Entertainment Inc.
	All Rights Reserved.
*/
#ifndef __CELL_COMPRESSION_DEFINES_H__
#define __CELL_COMPRESSION_DEFINES_H__

/** \internal
	\def COMPRESSION_EXPORT
	\brief Generates compiler export/import directives like __declspec(dllimport) on some platforms. */
#define COMPRESSION_EXPORT


#ifndef COMPRESSION_CONSTANT_LOG2
/** Preprocessor log2 function for numeric constants. */
# define COMPRESSION_CONSTANT_LOG2(val)     \
	((val <= 1) ? 0:     (val <= 2) ? 1:     (val <= 4) ? 2:      (val <= 8) ? 3:\
     (val <= 16) ? 4:    (val <= 32) ? 5:    (val <= 64) ? 6:     (val <= 128) ? 7:\
     (val <= 256) ? 8:   (val <= 512) ? 9:   (val <= 1024) ? 10:  (val <= 2048) ? 11:\
	 (val <= 4096) ? 12: (val <= 8192) ? 13: (val <= 16384) ? 14: (val <= 32768) ? 15:16)
#endif

#ifndef COMPRESSION_ALIGNOF
/** Returns the log2 of the alignment of a class or other object. */
# define COMPRESSION_ALIGNOF(t)	COMPRESSION_CONSTANT_LOG2(__alignof(t))
#endif

#ifndef COMPRESSION_ISALIGNED
/** Returns true if a pointer value is properly aligned. */
# define COMPRESSION_ISALIGNED(expr,log2align) \
	((((ptrdiff_t)(expr)) & (((ptrdiff_t)1<<(log2align))-1)) == 0)
#endif

#ifndef COMPRESSION_ISALIGNEDFOR
/** Returns true if a pointer value is properly aligned to hold a given type. */
# define COMPRESSION_ISALIGNEDFOR(expr,t) \
	((((ptrdiff_t)(expr)) & (((ptrdiff_t)1<<COMPRESSION_ALIGNOF(t))-1)) == 0)
#endif


#endif /* __CELL_COMPRESSION_DEFINES_H__ */


