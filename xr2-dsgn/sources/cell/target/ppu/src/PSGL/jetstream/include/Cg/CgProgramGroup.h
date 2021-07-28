/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 *                Copyright (C) 2009 Sony Computer Entertainment Inc.
 *                                               All Rights Reserved.
 */

#ifndef CGPROGRAMGROUP_HEADER
#define CGPROGRAMGROUP_HEADER

#include "Cg/cgBinary.h"

typedef struct _CGnamedProgram
{
    const char *name;
    CGprogram program;
    int refCount;
}
_CGnamedProgram;

typedef struct _CGprogramGroup
{
    struct _CGprogramGroup *next;
    CGcontext ctx;
    unsigned int *constantTable;
    unsigned int *stringTable;
    unsigned int programCount;
    _CGnamedProgram *programs;
    int refCount; //total number of program refCounted
    bool userCreated;
    char *filedata;
    char *name;
}
_CGprogramGroup;

/* Program groups */
typedef struct _CGprogramGroup *CGprogramGroup;

CGprogramGroup _jsCgCreateProgramGroup( CGcontext ctx, const char *name, void *ptr, int size );
CGprogramGroup _jsCgCreateProgramGroupFromFile( CGcontext ctx, const char *group_file );
void _jsCgDestroyProgramGroup( CGprogramGroup group );

int _jsCgGetProgramCount( CGprogramGroup group );
CGprogram _jsCgGetProgram( CGprogramGroup group, const char *name );
int _jsCgGetProgramIndex( CGprogramGroup group, const char *name );
CGprogram _jsCgGetProgramAtIndex( CGprogramGroup group, unsigned int index );
const char *_jsCgGetProgramGroupName( CGprogramGroup group );

#endif
