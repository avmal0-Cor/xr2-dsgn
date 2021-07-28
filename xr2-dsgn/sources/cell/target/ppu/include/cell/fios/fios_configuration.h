/*
	SCE CONFIDENTIAL
	PlayStation(R)3 Programmer Tool Runtime Library 310.001
	Copyright (C) 2009 Sony Computer Entertainment Inc.
	All Rights Reserved.
*/
/**
	\internal
	\file fios_configuration.h

	Configuration switches for the File I/O Scheduler. You may edit or override this
	header file to control the behavior of FIOS.
*/

#ifndef __CELL_FIOS_CONFIGURATION_H__
#define __CELL_FIOS_CONFIGURATION_H__

/** Set to 1 to enable debug behaviors of FIOS, including debug and informational output. */
#ifndef FIOSDEBUG
# if (defined(NDEBUG) && NDEBUG)
#  define FIOSDEBUG         0
# elif (defined(DEBUG) && DEBUG)
#  define FIOSDEBUG         1
# elif (defined(_DEBUG) && _DEBUG)
#  define FIOSDEBUG         1
# else
#  define FIOSDEBUG         0
# endif
#endif

#ifndef FIOS_CASE_INSENSITIVE_PATHS
/** Compile-time setting: controls case-sensitivity in FIOS.
	If set to 1, FIOS does all path operations with case-insensitive comparisons.
    If set to 0, FIOS does all path operations with case-sensitive comparisons.
	
	This setting only affects FIOS and does not control the underlying OS and filesystem, which may
	still be case-sensitive regardless. On Win32 this setting also controls whether FIOS uses
	FILE_FLAG_POSIX_SEMANTICS and whether it adds some additional checks to enforce case-sensitive access.
	
	Case-insensitivity is moderately expensive, and console filesystems are always case-sensitive.
	Thus we recommend enforcing case-sensitivity as early as possible in your development cycle. */
# define FIOS_CASE_INSENSITIVE_PATHS     0  // default: case-sensitive
#endif

#ifndef FIOS_CONSTANT_LOG2
/** Preprocessor helper: log2 function for numeric constants. */
# define FIOS_CONSTANT_LOG2(val)    ((val <= 1) ? 0:      (val <= 2) ? 1:\
									 (val <= 4) ? 2:      (val <= 8) ? 3:\
									 (val <= 16) ? 4:     (val <= 32) ? 5:\
									 (val <= 64) ? 6:     (val <= 128) ? 7:\
									 (val <= 256) ? 8:    (val <= 512) ? 9:\
									 (val <= 1024) ? 10:  (val <= 2048) ? 11:12)
#endif

#ifndef FIOS_ALIGNOF
/** Preprocessor helper: Returns the log2 of the alignment of a class or other object. */
# define FIOS_ALIGNOF(t)	FIOS_CONSTANT_LOG2(__alignof(t))
#endif

#endif /* __CELL_FIOS_CONFIGURATION_H__ */
