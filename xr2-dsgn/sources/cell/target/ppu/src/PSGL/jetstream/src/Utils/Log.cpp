/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 *                Copyright (C) 2009 Sony Computer Entertainment Inc.
 *                                               All Rights Reserved.
 */

 
/*
 * 
 */

#include <PSGL/psgl.h>
#include <PSGL/private.h>
#include <stdio.h>
#include <stdarg.h>

static int		_CurrentMessageID = 1;
static FILE*		_LogDestination = NULL;
static JSlogFunction	_LogFunction = NULL;


//========================================
// Log a message
//========================================
int jsLog( int type, char* message, ... )
{
    char		LTmpStr[4096];
    va_list	LAL;


    {
        va_start( LAL, message );				// message is the last argument before the varargs
        vsnprintf( LTmpStr, 4096, message, LAL );
        va_end( LAL );


        if ( _LogFunction ) _LogFunction( _CurrentMessageID, type, LTmpStr );
        else
        {
#ifndef JS_PLATFORM_CELL
            if ( _LogDestination == NULL ) _LogDestination = stdout;

            fprintf( _LogDestination, "%d > %s\n", _CurrentMessageID, LTmpStr );fflush( stdout );
#else
            printf( "%d > %s\n", _CurrentMessageID, LTmpStr );
#endif
        }

        _CurrentMessageID++;
        return _CurrentMessageID -1;
    }
    return 0;
}


//==================================================
// Set destination for log messages
//==================================================
void jsLogSetDestination( void* destination )
{
    _LogDestination = ( FILE* )destination;
}


//==================================================
// Return current destination for log messages
//==================================================
void* _jsLogGetDestination()
{
    return( _LogDestination );
}


//==================================================
// Set custom function that handles log messages
//==================================================
void jsLogSetFunction( JSlogFunction function )
{
    _LogFunction = function;
}


//================================================================
// Return current custom function that handles log messages
//================================================================
JSlogFunction jsLogGetFunction()
{
    return( _LogFunction );
}
