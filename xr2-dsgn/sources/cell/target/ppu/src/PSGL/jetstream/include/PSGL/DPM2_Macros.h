/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 *                Copyright (C) 2009 Sony Computer Entertainment Inc.
 *                                               All Rights Reserved.
 */
/*
 * DESCRIPTION:	
 *   Utility functions used for building and debuggig the DPMv2 
 *   kernel and controllers.
 *
 *   Derived from DPM.h by Nat Duca, 2006/01/27
 *
 * AUTHORS:	Alan Heirich, Nat Duca
 * DATE:	2006-Jan-27
 * 
 */

#ifndef _JS_DPM2_CALLREGISTER_H
#define _JS_DPM2_CALLREGISTER_H

#include <PSGL/Utils.h>
#include <PSGL/PerfCounters.h>

/////////////////////////////////////////////////////////////////////////////////
// Standard API entry and exit
/////////////////////////////////////////////////////////////////////////////////

#ifdef JS_DPM
#define _JS_DPM_STANDARD_API_PREAMBLE(FUNCTION,FN_TYPE,RETURN,COUNT,PSEUDO)\
  if(JS_LIKELY(_JS_DPM_STATE(APP_RUN))) { /* don't let this hook enable if we're making calls from the Controller */	_EOL_	\
    _JS_PERFORMANCE_START(HUDEntryPointCounter); _EOL_ \
    jsdpmCallbackReturn eRet = JSDPM_NEXT; _EOL_\
    /* set call register                                      */ _EOL_ \
    _jsdpmCallRegister.hasArgs = 0; _EOL_ \
    _jsdpmCallRegister.pseudoCall = PSEUDO; _EOL_ \
    _jsdpmCallRegister.function = (void* (*)())(FUNCTION); _EOL_ \
    _jsdpmCallRegister.functionName = (char*)(#FUNCTION); _EOL_ \
    _jsdpmCallRegister.functionType = FN_TYPE; _EOL_\
    _jsdpmCallRegister.parameterCount = COUNT; _EOL_\
    if(JS_UNLIKELY(_jsdpmEnableCallArgStorage)) { _EOL_ \
      _jsdpmCallRegister.hasArgs = 1; _EOL_
      // here the PSGL arguments are packed into the call struct ...
      // deferred arguments don't get packed, though.

#define _JS_DPM_STANDARD_API_LEADOUT(FUNCTION,FN_TYPE,RETURN) \
      _jsdpmState = _JS_DPM_STATE_CTL_RUN; _EOL_\
      eRet = _jsdpmGetCallback()(); _EOL_ \
      _jsdpmState = _JS_DPM_STATE_APP_RUN; _EOL_\
    } else {_EOL_\
      _jsdpmState = _JS_DPM_STATE_CTL_RUN; _EOL_\
      eRet = _jsdpmGetCallback()(); _EOL_ \
      _jsdpmState = _JS_DPM_STATE_APP_RUN; _EOL_\
    } _EOL_\
    _EOL_ \
    if(JS_UNLIKELY(eRet == JSDPM_SKIP)) { _EOL_ \
      _JS_PERFORMANCE_STOP(HUDEntryPointCounter); _EOL_ \
      return RETURN; _EOL_ \
    } _EOL_ \
    _JS_PERFORMANCE_STOP(HUDEntryPointCounter); _EOL_ \
  }	// if DPM_STATE(APP_RUN)
#else
#define _JS_DPM_STANDARD_API_PREAMBLE(FUNCTION,FN_TYPE,RETURN,COUNT,PSEUDO)
#define _JS_DPM_STANDARD_API_LEADOUT(FUNCTION,FN_TYPE,RETURN)
#endif

/////////////////////////////////////////////////////////////////////////////////
// Macros for entering a standard API routine and performing capture and replay.


// Arguments are function name, return type or nothing for void functions, argument count,
// then a list of arguments with (name, size, type) for each argument.  name is the
// argument name as used in the source code.  size is the amount of storage required to
// hold a copy of the argument data.  type is INPUT_BY_VALUE, INPUT_BY_REFERENCE,
// INPUT_DEFERRED or INPUT_OUTPUT.
//
// Note: if the number of arguments supported by the _JS_DPM_API_ENTRY_x macros increases,
// the number of dummy arguments in DPM.c : replayFrameFromTrace() must increase to match.
//
/////////////////////////////////////////////////////////////////////////////////
#include <PSGL/DPM2_Entry_Macros.h>

/////////////////////////////////////////////////////////////////////////////////
// Trace capture and replay macros
/////////////////////////////////////////////////////////////////////////////////


#define _JS_DPM_API_PROCESS_ARGUMENT(INDEX,NAME,SIZE,TYPE,ARGTYPE) \
	_JS_DPM_API_STORE_ARGUMENT(INDEX,NAME,SIZE,TYPE,ARGTYPE) \


/////////////////////////////////////////////////////////////////////////////////
// Macros for capturing individual singleton arguments
/////////////////////////////////////////////////////////////////////////////////

#if 0
#define _JS_DPM_API_STORE_ARGUMENT(INDEX,NAME,SIZE,TYPE,ARGTYPE) \
	_JS_DPM_API_STORE_ARGUMENT_##TYPE(INDEX,NAME,SIZE,TYPE,ARGTYPE)

#define _JS_DPM_API_STORE_ARGUMENT_INPUT_BY_VALUE(INDEX,NAME,SIZE,TYPE,ARGTYPE)		  \
	_JS_DPM_API_ENTRY_ALLOCATE_AND_STORE(INDEX,NAME,&(NAME),SIZE,_NOBRACKET,TYPE,ARGTYPE)

#define _JS_DPM_API_STORE_ARGUMENT_INPUT_BY_REFERENCE(INDEX,NAME,SIZE,TYPE,ARGTYPE)	  \
	_JS_DPM_API_ENTRY_ALLOCATE_AND_STORE(INDEX,NAME,NAME,SIZE,_NOBRACKET,TYPE,ARGTYPE)	

#define _JS_DPM_API_STORE_ARGUMENT_INPUT_DEFERRED(INDEX,NAME,SIZE,TYPE,ARGTYPE)		  \
	_JS_DPM_API_ENTRY_ALLOCATE_AND_STORE(INDEX,NAME,NAME,SIZE,_NOBRACKET,TYPE,ARGTYPE)

#define _JS_DPM_API_STORE_ARGUMENT_INPUT_OUTPUT(INDEX,NAME,SIZE,TYPE,ARGTYPE)		  \
	_JS_DPM_API_ENTRY_ALLOCATE_AND_STORE(INDEX,NAME,&(NAME),SIZE,_NOBRACKET,TYPE,ARGTYPE)	
#else
#define _JS_DPM_API_STORE_ARGUMENT(INDEX,NAME,SIZE,TYPE,ARGTYPE) \
	_JS_DPM_API_ENTRY_ALLOCATE_AND_STORE(INDEX,NAME,&(NAME),SIZE,_NOBRACKET,TYPE,ARGTYPE)
#endif


/////////////////////////////////////////////////////////////////////////////////
// Make sure data is aligned properly
/////////////////////////////////////////////////////////////////////////////////

#ifdef JS_PLATFORM_CELL	// to do - expand this to ensure the pointer has the same alignment as the data, NULL SOURCE_POINTER means don't align
#define _JS_DPM_ALIGN_POINTER(POINTER,SOURCE_POINTER,SIZE) \
	{	_EOL_ \
		unsigned int size = ((SIZE) <= 16) ? SIZE : 16;	_EOL_ \
		unsigned int mask = size - 1;	_EOL_ \
		unsigned int pmask = ((unsigned long) (POINTER) & mask);	_EOL_ \
		(POINTER) += pmask ? ((SIZE) - pmask) : 0;  _EOL_ \
	} _EOL_
#else
#define _JS_DPM_ALIGN_POINTER(POINTER,SOURCE_POINTER,SIZE)
#endif


/////////////////////////////////////////////////////////////////////////////////
// Allocate storage for one argument, copy the argument data into
// storage, and set the argument pointer to the storage location.
/////////////////////////////////////////////////////////////////////////////////

#ifdef JS_DPM
#if JS_DPM_V2
#define _JS_DPM_API_ENTRY_ALLOCATE_AND_STORE(INDEX,SOURCE_NAME,SOURCE_POINTER,SIZE,BRACKET,TYPE,ARGTYPE) \
	{\
     _jsdpmCallRegister.argumentPtr[INDEX-1] = (void*)SOURCE_POINTER;\
     _jsdpmCallRegister.argumentSize[INDEX-1] = SIZE;\
     _jsdpmCallRegister.argumentNames[INDEX-1] = (const char*)#SOURCE_NAME;\
     _jsdpmCallRegister.argumentType[INDEX-1] = ARGTYPE;\
  } // end of allocate_and_store
#else
#define _JS_DPM_API_ENTRY__AND_STORE(INDEX,SOURCE_POINTER,SIZE,BRACKET,TYPE,ARGTYPE)	\
	if (_JS_DPM_STATE(CAPTURE))	_EOL_ \
	{	_EOL_ \
		_JS_DPM_ALIGN_POINTER(_jsdpmCurrentStorageBase,SOURCE_POINTER,sizeof(SOURCE_POINTER))	_EOL_ \
		_JS_DPM_MEMORY_CHECK_RANGE(_jsdpmCurrentStorageBase) _EOL_ \
		_JS_DPM_MEMORY_CHECK_RANGE(_jsdpmCurrentStorageBase + SIZE) _EOL_ \
		int breaking = _jsdpmBreakAtCall > 0 && _jsdpmCallCounter >= _jsdpmBreakAtCall; _EOL_	\
		if (_jsdpmReportAPIArguments && !breaking)	_jsdpmReportAPIArgument(INDEX,#SOURCE_POINTER,(void*)SOURCE_POINTER,SIZE,#TYPE);	_EOL_	\
		memcpy(_jsdpmCurrentStorageBase,SOURCE_POINTER,SIZE); _EOL_ \
		_JS_DPM_CURRENT_ARGUMENT_PTR(INDEX) = _jsdpmCurrentStorageBase;	_EOL_ \
		/*_JS_DPM_PRINT_6("\t(%d) %s\t%s size %d ptr at [%p] data at [%p]\n",INDEX,#SOURCE_POINTER,#TYPE,(int)(SIZE),&_JS_DPM_CURRENT_ARGUMENT_PTR(INDEX),_jsdpmCurrentStorageBase);*/	_EOL_ \
		/*_JS_DPM_PRINT_VALUE##BRACKET(_JS_DPM_CURRENT_ARGUMENT_PTR(INDEX),SOURCE_POINTER)*/	_EOL_ \
		_jsdpmCurrentStorageBase += SIZE;	_EOL_ \
		_jsdpmArgumentStorageSize = (_jsdpmCurrentStorageBase - _jsdpmArguments);	_EOL_ \
	}	_EOL_	\
	else if (_JS_DPM_STATE(TRACELESS))	_EOL_	\
	{	\
		int breaking = _jsdpmBreakAtCall > 0 && _jsdpmCallCounter >= _jsdpmBreakAtCall; _EOL_	\
		if (_jsdpmReportAPIArguments && !breaking)	_jsdpmReportAPIArgument(INDEX,#SOURCE_POINTER,(void*)SOURCE_POINTER,SIZE,#TYPE);	_EOL_	\
	}
#endif
#else
#define _JS_DPM_API_ENTRY_ALLOCATE_AND_STORE(INDEX,SOURCE_NAME,SOURCE_POINTER,SIZE,BRACKET,TYPE,ARGTYPE)
#endif



/////////////////////////////////////////////////////////////////////////////////
//	Check for memory overflows
/////////////////////////////////////////////////////////////////////////////////

#ifdef _JS_DPM_MEMORY_CHECK
#define _JS_DPM_MEMORY_CHECK_RANGE(base)	\
	if (base >= _jsdpmMaxStorageAddress || base < _jsdpmArguments) \
	{ \
		char buffer[1024]; \
		sprintf(buffer, "memory check failed, location = %p, maxStorageAddress=%p, arguments=%p\n", base, _jsdpmMaxStorageAddress, _jsdpmArguments); \
		_jsdpmSendText(buffer);	\
		printf(buffer); _jsBackTrace(5);	\
		_JS_DPM_ASSERT("memory check failed, out of range storage address");	\
printf("at %s %d\n",__FILE__,__LINE__);exit(1); \
	}
#else
#define _JS_DPM_MEMORY_CHECK_RANGE(base)
#endif
	


/////////////////////////////////////////////////////////////////////////////////
// Macros for capturing an output buffer at routine exit.
/////////////////////////////////////////////////////////////////////////////////

#define _JS_DPM_API_RETURN(INDEX,OUTPUT_POINTER,SIZE) 	// NOP


/////////////////////////////////////////////////////////////////////////////////
// Macros for replaying individual singleton arguments.
// Retrieve the stored argument value and overwrite the local variable.
/////////////////////////////////////////////////////////////////////////////////

#ifdef JS_DPM
# ifdef JS_DPM_V2
#  define _JS_DPM_API_REPLAY_SINGLETON(INDEX,NAME,SIZE,TYPE) if(SIZE);
# else
#error Foo
#  define _JS_DPM_API_REPLAY_SINGLETON(INDEX,NAME,SIZE,TYPE) \
	_JS_DPM_API_REPLAY_SINGLETON_##TYPE(INDEX,NAME,SIZE,TYPE)

#define _JS_DPM_API_REPLAY_SINGLETON_INPUT_BY_VALUE(INDEX,NAME,SIZE,TYPE)		  \
	if (_JS_DPM_STATE(REPLAY)) _EOL_ \
	{ 	_EOL_ \
		int breaking = _jsdpmBreakAtCall > 0 && _jsdpmCallCounter >= _jsdpmBreakAtCall; _EOL_	\
		if (_jsdpmReportAPIArguments && !breaking)	_jsdpmReportAPIArgument(INDEX,#NAME,(void*)&NAME,SIZE,#TYPE);	_EOL_	\
		_JS_DPM_MEMORY_CHECK_RANGE(_JS_DPM_CURRENT_ARGUMENT_PTR(INDEX))	_EOL_ \
		memcpy(&(NAME),_JS_DPM_CURRENT_ARGUMENT_PTR(INDEX),SIZE); 	_EOL_ \
	} _EOL_	// _JS_DPM_IN_REPLAY_MODE

#define _JS_DPM_API_REPLAY_SINGLETON_INPUT_BY_REFERENCE(INDEX,NAME,SIZE,TYPE)	  \
	_JS_DPM_API_ENTRY_REPLAY_OVERWRITE_ARGUMENT(INDEX,NAME,SIZE,TYPE)
	
#define _JS_DPM_API_REPLAY_SINGLETON_INPUT_DEFERRED(INDEX,NAME,SIZE,TYPE)		  \
	_JS_DPM_API_ENTRY_REPLAY_OVERWRITE_ARGUMENT(INDEX,NAME,SIZE,TYPE)

#define _JS_DPM_API_REPLAY_SINGLETON_INPUT_OUTPUT(INDEX,NAME,SIZE,TYPE)		  \
	_JS_DPM_API_ENTRY_REPLAY_OVERWRITE_ARGUMENT(INDEX,NAME,SIZE,TYPE)

#define _JS_DPM_API_ENTRY_REPLAY_OVERWRITE_ARGUMENT(INDEX,NAME,SIZE,TYPE)	\
	if (_JS_DPM_STATE(REPLAY))	_EOL_ \
	{	_EOL_ \
 	    memcpy(&(NAME),(void *)&(_JS_DPM_CURRENT_ARGUMENT_PTR(INDEX)),sizeof(void *));	_EOL_ \
		int breaking = _jsdpmBreakAtCall > 0 && _jsdpmCallCounter >= _jsdpmBreakAtCall; _EOL_	\
		if (_jsdpmReportAPIArguments && !breaking)	_jsdpmReportAPIArgument(INDEX,#NAME,(void*)&NAME,SIZE,#TYPE);	_EOL_	\
	} _EOL_	// _JS_DPM_IN_REPLAY_MODE

# endif
#else
#define _JS_DPM_API_REPLAY_SINGLETON(INDEX,NAME,SIZE,TYPE) 	// NOP
#endif

/***************************************************************************/

#endif // _JSDPM2_CALLREGISTER_H


