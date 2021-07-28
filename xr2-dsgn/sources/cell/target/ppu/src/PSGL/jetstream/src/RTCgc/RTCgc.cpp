/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 *                Copyright (C) 2009 Sony Computer Entertainment Inc.
 *                                               All Rights Reserved.
 */

 
/*
 * 
 */

#include <Cg/cg.h>
#include <Cg/CgCommon.h>
#include <Cg/cgc.h>

void cgRTCgcInit( void )
{
    _cgRTCgcCompileProgramHook = &compile_program_from_string;
    _cgRTCgcFreeCompiledProgramHook = &free_compiled_program;
}

void cgRTCgcFree( void )
{
    _cgRTCgcCompileProgramHook = 0;
    _cgRTCgcFreeCompiledProgramHook = 0;
}
