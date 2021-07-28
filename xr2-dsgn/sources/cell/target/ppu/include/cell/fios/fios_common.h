/*
    SCE CONFIDENTIAL
    PlayStation(R)3 Programmer Tool Runtime Library 310.001
    Copyright (C) 2007 Sony Computer Entertainment Inc.
    All Rights Reserved.
*/
/**
    \internal
    \file fios_common.h

    File I/O runtime library common definitions.
*/
#ifndef __CELL_FIOS_COMMON_H__
#define __CELL_FIOS_COMMON_H__


#ifndef FIOS_LINKAGE
/** Generates compiler import/export directives when needed. */
#define FIOS_LINKAGE
#endif

/** \internal
	\def FIOS_ATTRIBUTE
	\param[in]  attr   Attribute definition.
	\brief Declares attributes that are compatible with both GCC and SNC. */
#define FIOS_ATTRIBUTE(attr)        __attribute__(attr)

/** \internal
	\def FIOS_GCC_VERSION
	\brief Returns true if we're being compiled with at least this version of gcc. Always false for other compilers. */
#if defined(__GNUC__)
# define FIOS_GCC_VERSION(major,minor,patch)    \
	((__GNUC__ > (major)) || \
	 (__GNUC__ == (major) && __GNUC_MINOR__ > (minor)) || \
	 (__GNUC__ == (major) && __GNUC_MINOR__ == (minor) && __GNUC_PATCHLEVEL__ == (patch)))
#else
# define FIOS_GCC_VERSION(major,minor,patch)     0
#endif

/** \internal
	\def FIOS_ATTRIBUTE_GCC
	\brief Declares attributes that are compatible with GCC but not SNC. */
#if defined(__GNUC__) && !defined(__SNC__)
# define FIOS_ATTRIBUTE_GCC(attr)       __attribute__(attr)
#else
# define FIOS_ATTRIBUTE_GCC(attr)
#endif

/** \internal
	\def FIOS_ATTRIBUTE_SENTINEL
	\brief Declares __attribute__((__sentinel__)) on compilers that support it. */
#define FIOS_ATTRIBUTE_SENTINEL       FIOS_ATTRIBUTE((__sentinel__))

/** \internal
	\def FIOS_ATTRIBUTE_NONNULL
	\brief Declares __attribute__((__nonnull__)) on compilers that support it. */
#define FIOS_ATTRIBUTE_NONNULL(x)       FIOS_ATTRIBUTE(x)

/** \internal
	\brief Hints to the compiler that the boolean expr will typically evaluate to true.
	\param[in]  expr    Expression which will typically evaluate to true.
	\return Evaluates to the value of expr. */
#define FIOS_LIKELY(expr)            __builtin_expect(!!(expr), 1)

/** \internal
	\brief Hints to the compiler that the boolean expr will typically evaluate to false.
	\param[in]  expr    Expression which will typically evaluate to false.
	\return Evaluates to the value of expr. */
#define FIOS_UNLIKELY(expr)           __builtin_expect((expr), 0)

/** \internal
	\brief Declares a parameter to be unused without generating a warning.
	\param[in]  param   Unused parameter. */
#define FIOS_UNUSED(param)      (void)&param

/** \internal
	\brief Marks a variable or argument with the "restrict" qualifier, if the compiler supports it. */
#if defined(__GNUC__)
# define FIOS_RESTRICT          __restrict__
#else
# define FIOS_RESTRICT
#endif

#endif // __CELL_FIOS_COMMON_H__
