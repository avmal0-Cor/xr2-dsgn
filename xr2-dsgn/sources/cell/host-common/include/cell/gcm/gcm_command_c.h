/*   SCE CONFIDENTIAL                                       */
/*   PlayStation(R)3 Programmer Tool Runtime Library 310.001 */
/*   Copyright (C) 2006 Sony Computer Entertainment Inc.    */
/*   All Rights Reserved.                                   */

#ifndef __CELL_GCM_COMMAND_C_H__
#define __CELL_GCM_COMMAND_C_H__

// C style functions

#define CELL_GCM_MODE CELL_GCM_C
#define CELL_GCM_THIS thisContext
#define CELL_GCM_CURRENT thisContext->current
#define CELL_GCM_END thisContext->end
#define CELL_GCM_CALLBACK thisContext->callback

#define CELL_GCM_MEASURE 0

// cellGcmFunctionUnsafe(CellGcmContext, ...);

#define CELL_GCM_INLINE 0
#define CELL_GCM_UNSAFE 1
#include <cell/gcm/gcm_prototypes.h>
#undef CELL_GCM_UNSAFE
#undef CELL_GCM_INLINE

// cellGcmFunctionUnsafeInline(CellGcmContext, ...);

#define CELL_GCM_INLINE 1
#define CELL_GCM_UNSAFE 1
#include <cell/gcm/gcm_prototypes.h>
#include <cell/gcm/gcm_implementation.h>
#undef CELL_GCM_UNSAFE
#undef CELL_GCM_INLINE

// cellGcmFunction(CellGcmContext, ...);

#define CELL_GCM_INLINE 0
#define CELL_GCM_UNSAFE 0
#include <cell/gcm/gcm_prototypes.h>
#undef CELL_GCM_UNSAFE
#undef CELL_GCM_INLINE

// cellGcmFunctionInline(CellGcmContext, ...);

#define CELL_GCM_INLINE 1
#define CELL_GCM_UNSAFE 0
#include <cell/gcm/gcm_prototypes.h>
#include <cell/gcm/gcm_implementation.h>
#undef CELL_GCM_UNSAFE
#undef CELL_GCM_INLINE

#undef CELL_GCM_MEASURE

#define CELL_GCM_MEASURE 1

// cellGcmFunctionMeasure(CellGcmContext, ...);

#define CELL_GCM_INLINE 0
#define CELL_GCM_UNSAFE 1
#include <cell/gcm/gcm_prototypes.h>
#undef CELL_GCM_UNSAFE
#undef CELL_GCM_INLINE

// cellGcmFunctionMeasureInline(CellGcmContext, ...);

#define CELL_GCM_INLINE 1
#define CELL_GCM_UNSAFE 1
#include <cell/gcm/gcm_prototypes.h>
#include <cell/gcm/gcm_measure_implementation.h>
#undef CELL_GCM_UNSAFE
#undef CELL_GCM_INLINE


// cellGcmFunctionMeasureSize(current, ...);
#undef	CELL_GCM_CURRENT
#undef	CELL_GCM_MEASURE
#define CELL_GCM_CURRENT current
#define CELL_GCM_MEASURE 2

#define CELL_GCM_INLINE 0
#define CELL_GCM_UNSAFE 1
#include <cell/gcm/gcm_prototypes.h>
#undef CELL_GCM_UNSAFE
#undef CELL_GCM_INLINE

// cellGcmFunctionMeasureSizeInline(current, ...);
#define CELL_GCM_INLINE 1
#define CELL_GCM_UNSAFE 1
#include <cell/gcm/gcm_prototypes.h>
#include <cell/gcm/gcm_measure_implementation.h>
#undef CELL_GCM_UNSAFE
#undef CELL_GCM_INLINE


#undef CELL_GCM_MEASURE

#undef CELL_GCM_MODE
#undef CELL_GCM_THIS
#undef CELL_GCM_CURRENT
#undef CELL_GCM_END
#undef CELL_GCM_CALLBACK

#endif /* __CELL_GCM_COMMAND_C_H__ */
