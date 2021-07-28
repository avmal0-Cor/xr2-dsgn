/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2008 Sony Computer Entertainment Inc.
* All Rights Reserved.
*/

#ifndef __LIB_CGC_H__
#define __LIB_CGC_H__

#include <stdlib.h>

#if defined(_WIN32)
#define DLLEXPORT __declspec(dllexport)
#else
#define DLLEXPORT
#endif


//
// ----------------------------------------------------------------------------------
//
//                   API 2.0

enum {
    SCECGC_ERROR_NO_IHANDLER = -6,
    SCECGC_ERROR_NO_PROGRAM = -5,
    SCECGC_ERROR_CANT_OPEN_FILE = -4,
    SCECGC_ERROR_BAD_ALLOC = -3,
    SCECGC_ERROR_INVALID_OPTION = -2,
    SCECGC_ERROR_UNEXPECTED = -1,
    SCECGC_OK = 0
    // positive value represents compilation error count
};

typedef int  CGCstatus;

typedef void CGCbin;

typedef void CGCcontext;

typedef struct {
    void* (*malloc)( void* arg, size_t size );	// Memory allocation callback
    void  ( *free )( void* arg, void* ptr );	// Memory freeing callback
    void* arg;
} CGCmem;

typedef enum {
    SCECGC_LOCAL_INCLUDE,
    SCECGC_SYSTEM_INCLUDE
} SCECGC_INCLUDE_TYPE;

typedef struct {
    int (*open)( SCECGC_INCLUDE_TYPE type,
		 const char* filename,
		 char** data, size_t* size );
    int (*close)( const char* data );

} CGCinclude;


#ifdef __cplusplus
extern "C" {
#define SCECGC_DEFAULT_0 =0
#else
#define SCECGC_DEFAULT_0
#endif

DLLEXPORT CGCcontext* sceCgcNewContext( CGCmem* pool  SCECGC_DEFAULT_0 );
DLLEXPORT void	      sceCgcDeleteContext( CGCcontext* context );

DLLEXPORT CGCbin*     sceCgcNewBin( CGCmem* pool  SCECGC_DEFAULT_0 );
DLLEXPORT void*       sceCgcGetBinData( CGCbin* bin );
DLLEXPORT size_t      sceCgcGetBinSize( CGCbin* bin );
DLLEXPORT CGCstatus   sceCgcStoreBinData( CGCbin* bin, void* data, size_t size );
DLLEXPORT void	      sceCgcDeleteBin( CGCbin* bin );

DLLEXPORT CGCstatus   sceCgcCompileString( CGCcontext*   context,
					   const char*   sourceString,
					   const char*   profile,
					   const char*   entry,
					   const char**  options,
					   CGCbin*       shaderBinary,
					   CGCbin*       messages  SCECGC_DEFAULT_0,
					   CGCbin*       asciiOutput  SCECGC_DEFAULT_0,
					   CGCinclude*   includeHandler  SCECGC_DEFAULT_0 );
#ifndef __CELLOS_LV2__
DLLEXPORT CGCstatus   sceCgcCompileFile( CGCcontext*   context,
					 const char*   sourceFile,
					 const char*   profile,
					 const char*   entry,
					 const char**  options,
					 CGCbin*       shaderBinary,
					 CGCbin*       messages  SCECGC_DEFAULT_0,
					 CGCbin*       asciiOutput  SCECGC_DEFAULT_0,
					 CGCinclude*   includeHandler  SCECGC_DEFAULT_0 );
#endif

#ifdef __cplusplus
}
#endif

//
// ----------------------------------------------------------------------------------
//
//                   Old API

#ifdef __cplusplus
extern "C" {
#endif

DLLEXPORT
int compile_program_from_string( const char* prog,
				 const char* profile_string,
				 const char* entry,
				 const char** args,
				 char** bout );
DLLEXPORT
void free_compiled_program ( char* bout );

#ifdef __cplusplus
}
#endif

#endif /* __LIB_CGC_H_ */
