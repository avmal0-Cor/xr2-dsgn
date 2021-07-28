/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 *                Copyright (C) 2009 Sony Computer Entertainment Inc.
 *                                               All Rights Reserved.
 */

 
/*
 * 
 */

#ifndef JS_DPM_V2
#error JS_DPM_V2 is not defined. Cannot build DPMv2.
#endif

#include <PSGL/psgl.h>
#include <PSGL/private.h>
#include <GLES/egl.h>

#include <PSGL/DPM2.h> /* redirects to DPM2.h */

#include <sys/ppu_thread.h>

///////////////////////////////////////////////////////////////////////////////
// Data
///////////////////////////////////////////////////////////////////////////////

int             _jsdpmState = _JS_DPM_STATE_OFF;

jsdpmCallback   _jsdpmCallbackStack[6];
#ifdef JS_DEBUG
const char*     _jsdpmCallbackStackDebug[6];
#endif
int             _jsdpmCallbackStackPos = -1;

_JS_DPM_CALL    _jsdpmCallRegister;	// holds the current DPM call /* ONLY */

// Enable flags
int           _jsdpmEnableCallArgStorage = 0;

// Legacy stuff... why is this even here?
jsUint64					_jsdpmSystemTimeAtCaptureStart;	// mark the start of the frame


int						_jsdpmTestSignal;				// how the test framework interacts with the perf tool

int						_jsdpmExperimentState;			// sequence of performance measurement experiments
int						_jsdpmMaxExperimentState;		// max in sequence

//unsigned long int			_jsdpmFrameCounter;	  			// number of buffer swaps

#if 1
#define DPM2_STARTUP_ROUTINE dpm2_qm_startup
#else
#define DPM2_STARTUP_ROUTINE dpm2_hud_startup
#endif
_JS_EXTERN_C APIENTRY jsdpmCallbackReturn DPM2_STARTUP_ROUTINE();

///////////////////////////////////////////////////////////////////////////////
// _jsdpmInit
///////////////////////////////////////////////////////////////////////////////
int _jsdpmInit( GLuint enable )
{
    static int	firstInit = 1;
    if ( !firstInit )	return	_JS_DPM_GOOD_RETURN;
    firstInit = 0;
    printf( "%s [%s]\n", _JS_DPM_NAME, _JS_DPM_VERSION );

    /* Activate the DPM2 client kernel */
    _jsdpmPushCallback( DPM2_STARTUP_ROUTINE );

    _jsdpmState = _JS_DPM_STATE_APP_RUN;
    return _JS_DPM_GOOD_RETURN;
}

///////////////////////////////////////////////////////////////////////////////
// _jsdpm Array Marshalling and Demarshalling
///////////////////////////////////////////////////////////////////////////////
void _jsdpmCaptureDeferredArray( PSGLcontext * context, GLuint index, int argumentIndex, GLint count )
{
    _JS_DPM_ASSERT( "CaptureDeferredArray not implemented" );
}
void _jsdpmReplayDeferredArray( PSGLcontext * context, GLuint index, int argumentIndex )
{
    _JS_DPM_ASSERT( "ReplayDeferredArray not implemented" );
}

inline int 	_jsdpmTexImageTypeToSize( GLenum type )
{
    _JS_DPM_ASSERT( "TexImageTypeToSize not implemented" );
    return 1;
}

///////////////////////////////////////////////////////////////////////////////
// _jsdpm Array Marshalling and Demarshalling
///////////////////////////////////////////////////////////////////////////////
void _jsdpmBeginPMQuery( int experimentState )
{
    _JS_DPM_ASSERT( "BeginPMQuery not implemented!" );
}

void _jsdpmEndPMQuery( int experimentState )
{
    _JS_DPM_ASSERT( "EndPMQuery not implented." );
}


///////////////////////////////////////////////////////////////////////////

int _jsdpmExitHandler()
{
    _JS_DPM_PSEUDO_ENTRY_0( psglExit, JSDPM_CALL_TYPE_CONTEXT, 0 );
    return 1;
}

///////////////////////////////////////////////////////////////////////////////
// _jsdpmTerminate
///////////////////////////////////////////////////////////////////////////////
void _jsdpmTerminate()
{
    _jsdpmExitHandler(); /* wakes up thread one last time ... */
}

///////////////////////////////////////////////////////////////////////////
void _jsdpmSetCallArgStorageEnabled( int bEnable ) { _jsdpmEnableCallArgStorage = bEnable; }

int  _jsdpmGetCallbackStackDepth()               { return _jsdpmCallbackStackPos + 1; }
jsdpmCallback _jsdpmGetCallback()                { return _jsdpmCallbackStack[_jsdpmCallbackStackPos]; }
jsdpmCallback _jsdpmGetCallbackFromStack( int iDistFromTop )
{
    int iVal = _jsdpmCallbackStackPos - iDistFromTop;
    JS_ASSERT( iVal >= 0 && iVal < _jsdpmCallbackStackPos );
    return _jsdpmCallbackStack[iVal];
}


#ifdef JS_DEBUG
void _jsdpmPushCallbackDebug( jsdpmCallback pCallback, const char* szName )
{
    ++_jsdpmCallbackStackPos;
    _jsdpmCallbackStack[_jsdpmCallbackStackPos] = pCallback;
    _jsdpmCallbackStackDebug[_jsdpmCallbackStackPos] = szName;
}
void _jsdpmSetCallbackDebug( jsdpmCallback pCallback, const char* szName )
{
    _jsdpmCallbackStack[_jsdpmCallbackStackPos] = pCallback;
    _jsdpmCallbackStackDebug[_jsdpmCallbackStackPos] = szName;
}
void _jsdpmSetCallbackOnStackDebug( jsdpmCallback pCallback, const char* szName, int iDistFromTop )
{
    int iVal = _jsdpmCallbackStackPos - iDistFromTop;
    JS_ASSERT( iVal >= 0 && iVal < _jsdpmCallbackStackPos );
    _jsdpmCallbackStack[iVal] = pCallback;
    _jsdpmCallbackStackDebug[iVal] = szName;
}
#else
void _jsdpmPushCallback( jsdpmCallback pCallback )
{
    _jsdpmCallbackStack[++_jsdpmCallbackStackPos] = pCallback;
}
void _jsdpmSetCallback( jsdpmCallback pCallback )
{
    _jsdpmCallbackStack[_jsdpmCallbackStackPos] = pCallback;
}
void _jsdpmSetCallbackOnStack( jsdpmCallback pCallback, int iDistFromTop )
{
    int iVal = _jsdpmCallbackStackPos - iDistFromTop;
    JS_ASSERT( iVal >= 0 && iVal < _jsdpmCallbackStackPos );
    _jsdpmCallbackStack[iVal] = pCallback;
}

#endif

jsdpmCallback _jsdpmPopCallback()
{
    jsdpmCallback pRet = _jsdpmCallbackStack[_jsdpmCallbackStackPos];
    _jsdpmCallbackStack[_jsdpmCallbackStackPos] = NULL;
#ifdef JS_DEBUG
    _jsdpmCallbackStackDebug[_jsdpmCallbackStackPos] = NULL;
#endif
    --_jsdpmCallbackStackPos;
    return pRet;
}

#ifdef JS_DEBUG
_JS_DPM_CALL* _jsdpmGetCallRegister()
{
    JS_ASSERT( _jsdpmCallRegister.hasArgs == 1 );
    return &_jsdpmCallRegister;
}
#endif
