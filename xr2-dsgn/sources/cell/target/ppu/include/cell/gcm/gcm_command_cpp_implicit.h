/*   SCE CONFIDENTIAL                                       */
/*   PlayStation(R)3 Programmer Tool Runtime Library 310.001 */
/*   Copyright (C) 2006 Sony Computer Entertainment Inc.    */
/*   All Rights Reserved.                                   */

#ifndef __CELL_GCM_COMMAND_CPP_IMPLICIT_H__
#define __CELL_GCM_COMMAND_CPP_IMPLICIT_H__

// C++ style functions and implicit context

#ifndef CELL_GCM_DENY_IMPLICIT_ARG

#define CELL_GCM_MODE		CELL_GCM_CPP_IMPLICIT
#define CELL_GCM_THIS 		gCellGcmCurrentContext
#define CELL_GCM_MEASURE 	0

// cellGcmFunction(...);

#define CELL_GCM_INLINE 0
#define CELL_GCM_UNSAFE 0
#include <cell/gcm/gcm_global_implementation.h>
#undef CELL_GCM_UNSAFE
#undef CELL_GCM_INLINE

// Unsafe::cellGcmFunction(...);

namespace Unsafe
{
#define CELL_GCM_INLINE 0
#define CELL_GCM_UNSAFE 1
#include <cell/gcm/gcm_global_implementation.h>
#undef CELL_GCM_UNSAFE
#undef CELL_GCM_INLINE
}

// Inline::cellGcmFunction(...);

namespace Inline
{
#define CELL_GCM_INLINE 1
#define CELL_GCM_UNSAFE 0
#include <cell/gcm/gcm_global_implementation.h>
#undef CELL_GCM_UNSAFE
#undef CELL_GCM_INLINE
}

// UnsafeInline::cellGcmFunction(...);

namespace UnsafeInline
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

#endif /* CELL_GCM_DENY_IMPLICIT_ARG */

#endif /* __CELL_GCM_COMMAND_CPP_IMPLICIT_H__ */
