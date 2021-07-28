/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 *                Copyright (C) 2009 Sony Computer Entertainment Inc.
 *                                               All Rights Reserved.
 */

 
 /*
 * DESCRIPTION:
 * Jetstream platform-dependent reporting utility - CELL
 */

#include <PSGL/psgl.h>
#include <stdio.h>
#include <PSGL/private.h>

static FILE* consoleHandle = 0;

void _jsInitConsole( GLuint enable )
{
    //logical_console_open(&console, id);
    if ( enable )
    {
        consoleHandle = stderr;
        fprintf( consoleHandle, "%s\n", "\nPSGL console initialized\n" );
        fflush( consoleHandle );
    }
    else consoleHandle = 0;
}

void _jsExitConsole()
{
    //if (console!=-1U) logical_console_close(console);
    consoleHandle = 0;
}

void _jsPlatformReport( const char *s )
{
    if ( !s ) return;
    if ( consoleHandle != 0 )
    {
        fprintf( consoleHandle, "%s\n", s );
        fflush( consoleHandle );
    }
    else printf( "%s\n", s );
}

#ifdef JS_DPM
static struct { char *name; void( *f )(); }
functionPointerLookupTable[] =
    {
#define _JS_API_ENTRY_POINTER	1
#include "jsapientrypointer.cpp"
        { NULL, NULL }
    };

void * _jsAPIENTRYPointer( char * name )
{
    int i = 0;
    while ( functionPointerLookupTable[i].name != NULL )
        if ( !strcmp( functionPointerLookupTable[i++].name, name ) )
            return ( void * )functionPointerLookupTable[i - 1].f;
    printf( "error, nonexistent function '%s'\n", name );
    JS_ASSERT( NULL );
    return NULL;
}
#endif
