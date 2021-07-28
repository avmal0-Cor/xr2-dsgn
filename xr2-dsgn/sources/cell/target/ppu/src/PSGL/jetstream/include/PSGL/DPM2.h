/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 *                Copyright (C) 2009 Sony Computer Entertainment Inc.
 *                                               All Rights Reserved.
 */

/*
 * DESCRIPTION:
 * Definitions for debugger runtime system version 2
 *
 * Do not confuse this with Debug.h
 *
 * See https://rdgfx/wiki/PSGL_Debugger for more documentation.
 *
 * DPM2 is similar in function from DPMv1. The big difference is that
 * it splits the idea of "debugger logic" from debugger support
 * services. The kernel of DPM2 contains basic services:
 *    - A call register
 *    - State save/restore
 *    - Controller management

 * A DPMv2 controller is a separate function that implements a the
 * logic usually associated with DPM. Eventually, DPMv2 will have the following
 * controllers:
 *    - PerfHUD Kernel
 *    - Performance Analyzer Kernel
 *    - PSGL Debugger Kernel
 *
 * AUTHORS: Nat Duca, Alan Heirich
 *
 * DATE: 2006-Jan-27
 */

#ifndef _JSDPM2_h
#define _JSDPM2_h


#include <PSGL/psgl.h>
#include <PSGL/platform.h>
#include <PSGL/Debug.h>
#include <PSGL/Types.h>

#if defined(JS_PLATFORM_CELL) && defined(JS_USE_NV)
#include "NV/NVPM.h"
#endif

#if defined(JS_PLATFORM_CELL)
#include <sys/ppu_thread.h>
#endif

#include <string.h>
/////////////////////////////////////////////////////////////////////////////////
// Basics
/////////////////////////////////////////////////////////////////////////////////

#define	_JS_DPM_NAME				"PSGL DPM2 threadless runtime"
#define	_JS_DPM_VERSION				"built on " __DATE__ " " __TIME__

#ifdef JS_DEBUG
#define _JS_DPM_MEMORY_CHECK		// extra code for checking buffer overflows
#define _JS_DPM_COMPILE_FOR_TEST	// extra code for printing debug output
#endif

#define _JS_DPM_ASSERT(STRING) _JS_REPORT_EXTRA(PSGL_REPORT_ASSERT,"DPM assertion at %s:%d : %s failed\n",__FILE__,__LINE__,STRING)

# ifndef	_JS_DPM_STORAGE_CLASS
#  define _JS_DPM_STORAGE_CLASS	extern
# endif

#define _JS_DPM_GOOD_RETURN	0 /* return value from _jsdpmInit() */

/////////////////////////////////////////////////////////////////////////////////
// DPMv2 State
/////////////////////////////////////////////////////////////////////////////////

#if defined (__cplusplus) && (!(defined (__CPLUSLIBS)))
extern "C"
{
#endif


    extern int _jsdpmState;

#ifdef JS_DPM

    _JS_DPM_STORAGE_CLASS int             _jsdpmState;
    _JS_DPM_STORAGE_CLASS int             _jsdpmSwitchArg;

    _JS_DPM_STORAGE_CLASS jsUint64					_jsdpmSystemTimeAtCaptureStart;	// mark the start of the frame


    _JS_DPM_STORAGE_CLASS int						_jsdpmTestSignal;				// how the test framework interacts with the perf tool

    _JS_DPM_STORAGE_CLASS int						_jsdpmExperimentState;			// sequence of performance measurement experiments
    _JS_DPM_STORAGE_CLASS int						_jsdpmMaxExperimentState;		// max in sequence

#if 0
    _JS_DPM_STORAGE_CLASS unsigned long int			_jsdpmFrameCounter;	  			// number of buffer swaps
    _JS_DPM_STORAGE_CLASS unsigned long int			_jsdpmDrawCallCounter; 			// number of draw calls
#endif

// Enable flags
    _JS_DPM_STORAGE_CLASS int           _jsdpmEnableCallArgStorage;

    /* defined in NVUtils.h */
// extern int        _jsdpmEnableNVAllocHooks;

    /* do I need these? */

//_JS_DPM_STORAGE_CLASS int						_jsdpmSavedState[MAX_NESTING];	// used for temporary state save/restore
//_JS_DPM_STORAGE_CLASS int						_jsdpmStateStack;				// stack pointer for saved states

#endif // def JS_DPM

/////////////////////////////////////////////////////////////////////////////////
// State flags
/////////////////////////////////////////////////////////////////////////////////
#define _JS_DPM_STATE_OFF           0 // DPM is basically off at this
    // point and the app is rushing ahead with frames
#define	_JS_DPM_STATE_APP_RUN       1 // DPM is on at this point and
    // each call hands control to the Controller
#define _JS_DPM_STATE_CTL_RUN       3 // The Controller is running DPM on
#define _JS_DPM_STATE_DS_RUN        4 // A secondary GL data source is running instead of the main app

// backward compatibility
#define	_JS_DPM_STATE_CLEAR				0 // DPM not operating
#define	_JS_DPM_STATE_CAPTURE			-1 // capturing (trace, timing)
#define	_JS_DPM_STATE_REPLAY			-2	// in replay
#define	_JS_DPM_STATE_TRACELESS		-3	// free running with spoofed clock
#define	_JS_DPM_STATE_SUSPENDED		-4	// use to block other state-dependent events

#define	_JS_DPM_STATE(STATE)     			(_jsdpmState == _JS_DPM_STATE_##STATE)

/////////////////////////////////////////////////////////////////////////////////
// Misc
/////////////////////////////////////////////////////////////////////////////////

#if	(defined(JS_DPM) && defined(JS_DEBUG))	// make this depend on debug state
#define	_JS_DPM_TRACK(TEXT)	{char b[256];sprintf(b,"(%s %d: %s)",__FILE__,__LINE__,TEXT);_jsdpmSendText(b);}
#else
#define	_JS_DPM_TRACK(TEXT)
#endif


/////////////////////////////////////////////////////////////////////////////////
// performance monitoring
/////////////////////////////////////////////////////////////////////////////////
#define	MAX_EXPERIMENTS						1024	// increase this if compilation or runtime fails
//#define	MAX_SIMULTANEOUS_EXPERIMENT_EVENTS	_JS_DPM_MAX_SIMULTANEOUS_EXPERIMENT_EVENTS
#define	MAX_SIMULTANEOUS_EXPERIMENT_EVENTS	29
#define	MAX_EXPERIMENT_CONFIG_STRING_LENGTH	512

    typedef	enum	{ no_clock = 1, hclk, gclk, sclk, rclk, mclk, iclk }
    CLOCK_DOMAIN;

    typedef enum	{ ELAPSED = 0x1, GETREPORT = 0x2, COUNT = 0x4, INC = 0x8, FB_DRAMC_1st = 0x10, FB_DRAMC_2nd = 0x20 } QUERY_TYPE;

#define	NOCOMBINE	0x8000	// OR with clock domain or query type
#define CLOCK_DOMAIN_MASK ~NOCOMBINE

    typedef struct
    {
        // These fields are compile-time specified.
        char			*name;				// experiment name
        CLOCK_DOMAIN	clockDomain;		// which part of the chip the signals come from
        QUERY_TYPE		queryType;
        char			*configString;		// the PM config string
        unsigned		enumerant;
        int				experimentNumber;	// runtime assigned
    }
    PERFMON_EXPERIMENT_STRUCT;


    typedef struct
    {
        void *			pmConfig;
        int				needsZcullReset;
        int				simultaneousEvents;
        unsigned int	elapsed;
        jsUint64		eventCount[MAX_SIMULTANEOUS_EXPERIMENT_EVENTS];
        PERFMON_EXPERIMENT_STRUCT *	experiments[MAX_SIMULTANEOUS_EXPERIMENT_EVENTS];
    }
    EXPERIMENT_STRUCT;

/////////////////////////////////////////////////////////////////////////////////
// Special compilation for debugging
/////////////////////////////////////////////////////////////////////////////////

#ifndef	_EOL_
#define	_EOL_	/*nothing*/
#endif


#if defined (__cplusplus) && (!(defined (__CPLUSLIBS)))
}	// Close scope of 'extern "C"' declaration which encloses file.
#endif

/////////////////////////////////////////////////////////////////////////////////
// backward compatibility with DPMv2
/////////////////////////////////////////////////////////////////////////////////
PSGLuint64 APIENTRY PSGLGetSystemTime();	// the spoof
PSGLuint64 APIENTRY _jsGetSystemTime();	// the truth
void		_jsdpmBeginPMQuery( int experimentState );
void		_jsdpmEndPMQuery( int experimentState );

// Test signals - modify the DPM actions
#define	_JS_DPM_TEST_CLEAR				0x00	// no modifications
#define	_JS_DPM_TEST_PERF_PER_DRAW		0x01	// measure performance per draw call
#define	_JS_DPM_TEST_IMAGE 				0x02	// automatic image validation
#define	_JS_DPM_TEST_GLERROR			0x10	// test for glError on replay
#define	_JS_DPM_TEST(TEST)				(_jsdpmTestSignal & _JS_DPM_TEST_##TEST)

#if 0 /* xxxx */
#define	_JS_DPM_TRACE_OFF	{ _jsdpmSavedState[_jsdpmStateStack++] = _jsdpmState; _jsdpmState = _JS_DPM_STATE_OFF; /*_JS_DPM_TRACK("TRACE_OFF")*/ }
#define	_JS_DPM_TRACE_ON	{ _jsdpmState = _jsdpmSavedState[--_jsdpmStateStack]; /*_JS_DPM_TRACK("TRACE_ON")*/ }
#else
#define _JS_DPM_TRACE_OFF
#define _JS_DPM_TRACE_ON
#endif

/* wtf is this? */
typedef struct
{
    jsTexNameSpace	textureNameSpace;
    jsTexNameSpace	bufferObjectNameSpace;
    jsTexNameSpace	fenceObjectNameSpace;
    jsTexNameSpace	eventObjectNameSpace;
}
_JS_DPM_TEX_NAME_SPACE_STRUCT;

///////////////////////////////////////////////////////////////////////////

// Trace information for a single call, excluding argument data
#ifdef JS_DPM
#include <PSGL/DPM2_protected.h>

/* core DPM registers */
_JS_DPM_STORAGE_CLASS void*                  _jsdpmCallRegisterArgBase;		// storage to hold call arguments


// manipulation macros
#define _JS_DPM_CALL_ARGUMENT_PTR(CALL,INDEX)	((CALL)->argumentPtr[INDEX - 1])
#define _JS_DPM_CURRENT_ARGUMENT_PTR(INDEX)		_JS_DPM_CALL_ARGUMENT_PTR(&_jsdpmCallRegister, INDEX)
#define _JS_DPM_CURRENT_CALL_STRUCT_PTR (&_jsdpmCallRegister)
#endif

#include <PSGL/DPM2_Macros.h>

/////////////////////////////////////////////////////////////////////////////////
// PSGL-SIDE Function prototypes
/////////////////////////////////////////////////////////////////////////////////

int			_jsdpmInit( GLuint enable );
void		_jsdpmTerminate();

//void 		_jsdpmReportAPIEntry(char *functionName, int argumentCount);
//void 		_jsdpmReportAPIArgument(int index, char *sourceName, void *sourceAddress, int size, char *storageType);

void _jsdpmCaptureDeferredArray( PSGLcontext * context, GLuint index, int argumentIndex, GLint count );
void _jsdpmReplayDeferredArray( PSGLcontext * context, GLuint index, int argumentIndex );
inline int 	_jsdpmTexImageTypeToSize( GLenum type );


/////////////////////////////////////////////////////////////////////////////////
// CONTROLLER-SIDE Function prototypes
/////////////////////////////////////////////////////////////////////////////////
//#include <PSGL/DPM2_Control.h>

// Fake API entries that show up in PSGL
///////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
extern "C"
{
#endif

    void psglSwapAfter( void );
    void psglDrawMiddle( void );
    void psglDrawAfter( void );

#ifdef __cplusplus
}
#endif

///////////////////////////////////////////////////////////////////////////

#ifdef JS_DPM
extern int           _jsdpmCallbackStackPos;
extern jsdpmCallback _jsdpmCallbackStack[6];
#ifdef JS_DEBUG
extern const char* _jsdpmCallbackStackDebug[6];
#endif
#endif // JS_DPM

#ifdef JS_DPM
#ifdef __cplusplus
extern "C"
{
#endif
    unsigned int  _jsdpmEstimateDataAreaSize();


// call register access
#define  _jsdpmGetCallPtr() \
  (_jsdpmCallRegister.function)
#define  _jsdpmGetCallName() \
  (_jsdpmCallRegister.functionName)
#define  _jsdpmGetCallType() \
  (_jsdpmCallRegister.functionType)
#define  _jsdpmGetCallPseudo() \
  (_jsdpmCallRegister.pseudoCall)



#ifdef JS_DEBUG
    _JS_DPM_CALL* _jsdpmGetCallRegister();
#else
#define _jsdpmGetCallRegister() (&_jsdpmCallRegister)
#endif // JS_DEBUG

#ifdef __cplusplus
}
#endif
#endif // JS_DPM



#endif	// _JSDPM2_h
