/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 *                Copyright (C) 2009 Sony Computer Entertainment Inc.
 *                                               All Rights Reserved.
 */


/*
 * Define performance counter categories, counter variables, and output formatting.
 *
 * If JS_PERFORMANCE_INFO is #defined, this file declares externs for the counter variables,
 * and must be #included whereever performance statistics are collected using
 * the _JS_PERFORMANCE_START and  _JS_PERFORMANCE_STOP macros.
 */


#ifndef _JS_PERF_COUNTERS_H_
#define _JS_PERF_COUNTERS_H_

#include <PSGL/Utils.h>

#define COUNTER_LIST \
COUNTER(StreamTimes,    MallocFreeCounter,                "Memory allocation time:                        [%f]\n") \
COUNTER(DrawDetails,    RasterDrawCounter,                "    PlatformDraw time:                         [%f]\n") \
COUNTER(DrawDetails,    AttributesCounter,                "      Attributes setup time:                   [%f]\n") \
COUNTER(DrawDetails,    RasterMemcpyCounter,              "        Raster memcpy time:                    [%f]\n") \
COUNTER(DrawDetails,    AttributeConverCounter,           "          Attribute Convert time:              [%f]\n") \
COUNTER(DrawDetails,    IndicesCounter,                   "      Indices setup time:                      [%f]\n") \
COUNTER(DrawDetails,    NVDrawCounter,                    "      NVGL Draw time:                          [%f]\n") \
COUNTER(DrawDetails,    RasterDrawEndCounter,             "      Draw End time:                           [%f]\n") \
COUNTER(CoreDetails,    FastMemcpyCounter,                "PSGL memcpy total                              [%f]\n") \
COUNTER(CoreDetails,    SPUMemcpyCounter,                 "   SPU copy                                    [%f]\n") \
\
COUNTER(CoreDetails,    ComputeMinMaxIndicesCounter,      "Overhead from non-VBO drawing:                 [%f]\n") \
\
COUNTER(CoreDetails,    NVAllocAllocCounter,              "NVAllocAlloc time                              [%f]\n") \
COUNTER(CoreDetails,    NVAllocExtendCounter,             "NVAllocExtend time                             [%f]\n") \
COUNTER(CoreDetails,    NVAllocFreeCounter,               "NVAllocFree time                               [%f]\n") \
COUNTER(CoreDetails,    NVAllocSweepCounter,              "NVAllocSweep time                              [%f]\n") \
\
COUNTER(ValidateDetails,ValidateStatesCounter,            "ValidateStates time:                           [%f]\n") \
COUNTER(ValidateDetails,ValidateTextureCounter,           "  ValidateTexture time:                        [%f]\n") \
COUNTER(ValidateDetails,UploadTextureCounter,             "    Upload Textures time:                      [%f]\n") \
COUNTER(ValidateDetails,ValidateVertexPipeCounter,        "  ValidateVertexPipeline time:                 [%f]\n") \
COUNTER(ValidateDetails,ValidateVertexProgramCounter,     "    ValidateVertexProgram time:                [%f]\n") \
COUNTER(ValidateDetails,ValidateVertexConstantsCounter,   "    ValidateVertexConstants time:              [%f]\n") \
COUNTER(ValidateDetails,ValidateFragmentProgramCounter,   "  ValidateFragmentProgram time:                [%f]\n") \
COUNTER(ValidateDetails,ValidateFPSharedConstantsCounter, "  ValidateFragmentProgramSharedConstants time: [%f]\n") \
COUNTER(ValidateDetails,ValidateSurfacesCounter,          "  ValidateSurfaces time:                       [%f]\n") \
COUNTER(ValidateDetails,ValidateFFXCounter,               "  Validate fixed function pipe time:           [%f]\n") \
\
COUNTER(StreamDetails,  SwapBuffersCounter,               "SwapBuffers stall time:                        [%f]\n") \
COUNTER(StreamDetails,  GPUFinishFenceCounter,            "Fence time:                                    [%f]\n") \
COUNTER(StreamDetails,  GPUWaitFifoCounter,               "Wait for GPU space stall time:                 [%f]\n") \
COUNTER(StreamDetails,  GPUFinishCounter,                 "Finish time:                                   [%f]\n") \
COUNTER(StreamDetails,  ReadPixelsStallCounter,           "  ReadPixels stall time                        [%f]\n") \
COUNTER(StreamDetails,  TextureCopyBackStallCounter,      "Texture copy-back stall time                   [%f]\n") \
COUNTER(StreamDetails,  NVMemcpyCounter,                  "NV memcpy time                                 [%f]\n") \
COUNTER(StreamDetails,  DMACmdCounter,                    "dmaCmd time                                    [%f]\n") \
COUNTER(StreamDetails,  CopyTexImageCounter,              "CopyTexImage (partial, 2D)                     [%f]\n") \
COUNTER(StreamDetails,  GPUTransferFifoCounter,           "Fifo Transfer                                  [%f]\n") \
COUNTER(StreamDetails,  RasterToImageCounter,             "Raster to Image                                [%f]\n") \
COUNTER(StreamDetails,  ImageToRasterCounter,             "Image to Raster                                [%f]\n") \
COUNTER(StreamDetails,  BounceAllocCounter,               "Bounce Allocator                               [%f]\n") \
\
COUNTER(CallTimes,      DrawArraysCounter,                "DrawArrays time:                               [%f]\n") \
COUNTER(CallTimes,      DrawElementsCounter,              "DrawElements time:                             [%f]\n") \
COUNTER(CallTimes,      DrawRangeElementsCounter,         "DrawRangeElements time:                        [%f]\n") \
COUNTER(CallTimes,      ClearCounter,                     "glClear time                                   [%f]\n") \
COUNTER(CallTimes,      ReadPixelsCounter,                "glReadPixels time                              [%f]\n") \
COUNTER(CallTimes,      MapBufferCounter,                 "glMapBuffer time                               [%f]\n") \
COUNTER(CallTimes,      BufferDataCounter,                "glBufferData time                              [%f]\n") \
COUNTER(CallTimes,      BufferSubDataCounter,             "glBufferSubData time                           [%f]\n") \
COUNTER(CallTimes,      UnmapBufferCounter,               "glUnmapBuffer time                             [%f]\n") \
\
COUNTER(HUDTimes,       HUDEntryPointCounter,             "HUD per-call overhead                          [%f]\n") \
COUNTER(HUDTimes,       HUDRsxElapsedMeasruementCounter,  "HUD per-call Rsx timing overhead               [%f]\n") \
\
COUNTER(Unused,         Spare0,                           "spare0                                         [%f]\n") \
COUNTER(Unused,         Spare1,                           "spare1                                         [%f]\n") \
COUNTER(Unused,         Spare2,                           "spare2                                         [%f]\n") \
COUNTER(Unused,         Spare3,                           "spare3                                         [%f]\n") \
COUNTER(Unused,         Spare4,                           "spare4                                         [%f]\n") \
COUNTER(Unused,         Spare5,                           "spare5                                         [%f]\n") \
COUNTER(Unused,         Spare6,                           "spare6                                         [%f]\n") \
COUNTER(Unused,         Spare7,                           "spare7                                         [%f]\n") \
COUNTER(Unused,         Spare8,                           "spare8                                         [%f]\n") \
COUNTER(Unused,         Spare9,                           "spare9                                         [%f]\n") \
COUNTER(Unused,         Spare10,                          "spare10                                        [%f]\n")

#ifdef JS_PERFORMANCE_INFO

// use COUNTER_LIST to declare externs to the performance counter variables.
#define COUNTER(Group,Name,Text) extern PerformanceCounter_t Name;
COUNTER_LIST
#undef COUNTER

#define _SPB(X) _JS_PERFORMANCE_START(Spare##X)
#define _SPE(X) _JS_PERFORMANCE_STOP(Spare##X)

#endif

#endif
