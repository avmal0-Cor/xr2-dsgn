%(/*)
#########################################################
# SCE CONFIDENTIAL
# PlayStation(R)3 Programmer Tool Runtime Library 310.001
# Copyright (C) 2009 Sony Computer Entertainment Inc.
# All Rights Reserved.
#########################################################
%(*/)\
///////////////////////////////////////////////////////////////////////////////
//
//  This file can be edited by hand.
//  It is only generated if it is not found.
// 
///////////////////////////////////////////////////////////////////////////////

#ifndef %(MAKE_SYMBOL)$(_FILENAME_)%(END)
#define %(MAKE_SYMBOL)$(_FILENAME_)%(END)


// This header is included by generated files that depend on parameter types
// specified in $(class).jdl.
//
// Use this header to
//  * Include other headers to define types.
//  * Override JDL_ASSERT(b) to your own assertion macro.
//  * Override JDL_WARN(b, str) to your own warning macro.
// 
// Use these defines to determine what source included this file:
//  * CELL_JDL_JOB_DESCRIPTOR - included from $(managedFilePrefix)$(class).cpp.
//  * CELL_JDL_JOB_MAIN - included from one of the SPURS job SPU sources.


#endif

