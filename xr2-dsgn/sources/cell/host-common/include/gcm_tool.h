/*   SCE CONFIDENTIAL                                       */
/*   PlayStation(R)3 Programmer Tool Runtime Library 310.001 */
/*   Copyright (C) 2006 Sony Computer Entertainment Inc.    */
/*   All Rights Reserved.                                   */

#ifndef __CELL_GCM_TOOL_H__
#define __CELL_GCM_TOOL_H__

#ifdef WIN32
typedef signed char		int8_t;
typedef signed short	int16_t;
typedef signed int		int32_t;
typedef signed long long	int64_t;
typedef unsigned char	uint8_t;
typedef unsigned short	uint16_t;
typedef unsigned int	uint32_t;
typedef unsigned long long	uint64_t;
#else
#include <stdint.h>
#endif

#include <string.h>

#define CELL_GCM_LITTLE_ENDIAN 1
#define CELL_GCM_CAST_UINT32 1
#ifndef CELL_GCM_MEMCPY
#define CELL_GCM_MEMCPY cellGcmMemoryCopyUint32
#endif

#include <Cg/cg.h>
#include <cell/gcm/gcm_error.h>
#include <cell/gcm/gcm_macros.h>
#include <cell/gcm/gcm_helper.h>
#include <cell/gcm/gcm_methods.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include <cell/gcm/gcm_enum.h>
#include <cell/gcm/gcm_struct.h>

// prototypes

// C style functions
#include <cell/gcm/gcm_command_c.h>

#ifdef __cplusplus
}
#endif /* __cplusplus */

// C++ style functions
#ifdef __cplusplus

namespace cell {
namespace Gcm {

#include <cell/gcm/gcm_command_cpp_explicit.h>

} /* namespace Gcm */
} /* namespace cell */

#endif /* __cplusplus */

#endif	/* __CELL_GCM_TOOL_H__ */
