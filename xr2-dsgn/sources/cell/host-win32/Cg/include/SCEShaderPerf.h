/*   SCE CONFIDENTIAL                                       */
/*   PlayStation(R)3 Programmer Tool Runtime Library 310.001 */
/*   Copyright (C) 2008 Sony Computer Entertainment Inc.    */
/*   All Rights Reserved.                                   */

#ifndef __SCE_SHADER_PERF__
#define __SCE_SHADER_PERF__

#ifndef uint32_t
#define uint32_t unsigned long
#endif

#ifndef uint64_t
#define uint64_t unsigned long long
#endif

#ifdef __cplusplus
extern "C" {
#endif

///////////////////////////////////////////////////////////////////////////////
// Error/return codes
typedef enum {
	SCESP_ERROR_UNKNOWN                     = -5, //
	SCESP_ERROR_PATH_NOT_FOUND              = -4, // Error finding path to NVShaderPerf.dll.  Please reinstall NVShaderPerf
	SCESP_ERROR_LIBRARY_NOT_FOUND           = -2, // Error finding NVShaderPerf.dll.  Please reinstall NVShaderPerf
	SCESP_ERROR_NVSP_INIT                   = -1, // Error initializing NVShaderPerf dll

	SCESP_OK                                =  0,

	SCESP_ERROR_NVSP_INTERNAL               =  2, // Got NVidia dll's internal error
	SCESP_ERROR_BAD_ARGUMENTS               =  4, // Bad arguements were set to optionArray of sceShaderPerfMeasurement[FromFile]()
	SCESP_ERROR_FILE_NOT_FOUND              =  6, // Input shader file is not found
	SCESP_ERROR_SHADER_TYPE_UNSUPPORTED     =  7, // Shader file passed is not currently supported
	SCESP_ERROR_FAILURE_OPENING_OUTPUT_FILE = 10, // There was an error opening one or both of the output files
	SCESP_ERROR_CGBIN_FILE_INVALID          = 16, // The .Cgbin file that was attempted to be opened was determined to be invalid
	SCESP_ERROR_SIMULATION_ERROR            = 17, // Error while running performance simulation
	SCESP_ERROR_ENUMERATION_ENDED_EARLY     = 18, // Error happened in front end of NVShaderPerf.dll
} SceSpResult;

///////////////////////////////////////////////////////////////////////////////
// Struct
typedef struct _SceSpMeasurementResult
{
	SceSpResult nResult;   // This is the result from trying to run this file/code through performance analysis

	uint32_t nCycles;
	uint32_t nRRegisters;
	uint64_t nThroughput;
} SceSpMeasurementResult;

///////////////////////////////////////////////////////////////////////////////
// Functions
SceSpResult sceShaderPerfInit(void);
void sceShaderPerfExit(void);

SceSpResult sceShaderPerfMeasureFromFile(const char* sourceFileName, 
										 const char* optionArray[], 
										 SceSpMeasurementResult* result);

SceSpResult sceShaderPerfMeasure(const char* sourceBuffer, 
								 size_t cgbinSize, 
								 const char* optionArray[], 
								 SceSpMeasurementResult* result);

#ifdef __cplusplus
}
#endif

#endif /* __SCE_SHADER_PERF__ */
