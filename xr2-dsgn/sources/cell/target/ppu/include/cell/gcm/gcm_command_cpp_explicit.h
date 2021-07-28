/*   SCE CONFIDENTIAL                                       */
/*   PlayStation(R)3 Programmer Tool Runtime Library 310.001 */
/*   Copyright (C) 2006 Sony Computer Entertainment Inc.    */
/*   All Rights Reserved.                                   */

#ifndef __CELL_GCM_COMMAND_CPP_EXPLICIT_H__
#define __CELL_GCM_COMMAND_CPP_EXPLICIT_H__

// C++ style functions and explicit context

// Member methods and data types
#define CELL_GCM_MODE		CELL_GCM_CPP_EXPLICIT_METHOD
#define CELL_GCM_THIS		this
#define CELL_GCM_CURRENT	current
#define CELL_GCM_END		end
#define CELL_GCM_CALLBACK	callback
#define CELL_GCM_MEASURE	0


// CellGcmContext->Function(...);

#define CELL_GCM_INLINE 0
#define CELL_GCM_UNSAFE 0
struct CellGcmContext : public CellGcmContextData
{
#include <cell/gcm/gcm_global_implementation.h>
};
#undef CELL_GCM_INLINE
#undef CELL_GCM_UNSAFE


// Unsafe::CellGcmContext->Function(...);

namespace Unsafe
{
#define CELL_GCM_INLINE 0
#define CELL_GCM_UNSAFE 1
	struct CellGcmContext : public CellGcmContextData
	{
#include <cell/gcm/gcm_global_implementation.h>
	};
#undef CELL_GCM_INLINE
#undef CELL_GCM_UNSAFE
}

// Inline::CellGcmContext->Function(...);

namespace Inline
{
#define CELL_GCM_INLINE 1
#define CELL_GCM_UNSAFE 0
	struct CellGcmContext : public CellGcmContextData
	{
#include <cell/gcm/gcm_global_implementation.h>
	};
#undef CELL_GCM_INLINE
#undef CELL_GCM_UNSAFE
}

// UnsafeInline::CellGcmContext->Function(...);

namespace UnsafeInline
{
#define CELL_GCM_INLINE 1
#define CELL_GCM_UNSAFE 1
	struct CellGcmContext : public CellGcmContextData
	{
#include <cell/gcm/gcm_global_implementation.h>
	};
#undef CELL_GCM_INLINE
#undef CELL_GCM_UNSAFE
}

#undef CELL_GCM_MEASURE

// Space Measurements using actual code.
#define CELL_GCM_MEASURE 1

// Measure::CellGcmContext->Function(...);

namespace Measure
{
#define CELL_GCM_INLINE 0
#define CELL_GCM_UNSAFE 1
	struct CellGcmContext : public CellGcmContextData
	{
#include <cell/gcm/gcm_global_implementation.h>
	};
#undef CELL_GCM_INLINE
#undef CELL_GCM_UNSAFE
}

// MeasureInline::CellGcmContext->Function(...);

namespace MeasureInline
{
#define CELL_GCM_INLINE 1
#define CELL_GCM_UNSAFE 1
	struct CellGcmContext : public CellGcmContextData
	{
#include <cell/gcm/gcm_global_implementation.h>
	};
#undef CELL_GCM_INLINE
#undef CELL_GCM_UNSAFE
}

#undef CELL_GCM_MEASURE

#undef CELL_GCM_MODE
#undef CELL_GCM_CPP_MEMBER
#undef CELL_GCM_THIS
#undef CELL_GCM_CURRENT
#undef CELL_GCM_END
#undef CELL_GCM_CALLBACK



#define CELL_GCM_MODE 		CELL_GCM_CPP_EXPLICIT_NAMESPACE
#define CELL_GCM_THIS 		thisContext
#define CELL_GCM_MEASURE 	0

// Unsafe::cellGcmFunction(CellGcmContext, ...);

namespace Unsafe
{
#define CELL_GCM_INLINE 0
#define CELL_GCM_UNSAFE 1
#include <cell/gcm/gcm_global_implementation.h>
#undef CELL_GCM_UNSAFE
#undef CELL_GCM_INLINE
}

// Inline::cellGcmFunction(CellGcmContext, ...);

namespace Inline
{
#define CELL_GCM_INLINE 1
#define CELL_GCM_UNSAFE 0
#include <cell/gcm/gcm_global_implementation.h>
#undef CELL_GCM_UNSAFE
#undef CELL_GCM_INLINE
}

// UnsafeInline::cellGcmFunction(CellGcmContext, ...);

namespace UnsafeInline
{
#define CELL_GCM_INLINE 1
#define CELL_GCM_UNSAFE 1
#include <cell/gcm/gcm_global_implementation.h>
#undef CELL_GCM_UNSAFE
#undef CELL_GCM_INLINE
}

#undef CELL_GCM_MEASURE

// Measure
#define CELL_GCM_MEASURE 1

// Measure::cellGcmFunction(CellGcmContext, ...);

namespace Measure
{
#define CELL_GCM_INLINE 0
#define CELL_GCM_UNSAFE 1
#include <cell/gcm/gcm_global_implementation.h>
#undef CELL_GCM_UNSAFE
#undef CELL_GCM_INLINE
}

// MeasureInline::cellGcmFunction(CellGcmContext, ...);

namespace MeasureInline
{
#define CELL_GCM_INLINE 1
#define CELL_GCM_UNSAFE 1
#include <cell/gcm/gcm_global_implementation.h>
#undef CELL_GCM_UNSAFE
#undef CELL_GCM_INLINE
}

// MeasureSize
#undef	CELL_GCM_MEASURE
#define CELL_GCM_MEASURE 2

// MeasureSize::cellGcmFunction(CellGcmContext, ...);

namespace MeasureSize
{
#define CELL_GCM_INLINE 0
#define CELL_GCM_UNSAFE 1
#include <cell/gcm/gcm_global_implementation.h>
#undef CELL_GCM_UNSAFE
#undef CELL_GCM_INLINE
}

// MeasureSizeInline::cellGcmFunction(CellGcmContext, ...);

namespace MeasureSizeInline
{
#define CELL_GCM_INLINE 1
#define CELL_GCM_UNSAFE 1
#include <cell/gcm/gcm_global_implementation.h>
#undef CELL_GCM_UNSAFE
#undef CELL_GCM_INLINE
}


#undef CELL_GCM_MODE
#undef CELL_GCM_THIS
#undef CELL_GCM_MEASURE

#endif /* __CELL_GCM_COMMAND_CPP_EXPLICIT_H__ */
