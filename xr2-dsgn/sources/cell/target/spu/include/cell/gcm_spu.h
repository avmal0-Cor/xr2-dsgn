/*   SCE CONFIDENTIAL                                       */
/*   PlayStation(R)3 Programmer Tool Runtime Library 310.001 */
/*   Copyright (C) 2006 Sony Computer Entertainment Inc.    */
/*   All Rights Reserved.                                   */

#ifndef __CELL_GCM_SPU_H__
#define __CELL_GCM_SPU_H__

#include <stdint.h>
#include <stdbool.h>
#include <spu_intrinsics.h>

#ifndef CELL_GCM_MEMCPY
# define CELL_GCM_MEMCPY __builtin_memcpy
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

// ea address to offset
int32_t cellGcmSpuAddressToOffset(const uint16_t *eaTable, const uintptr_t localAddress, const uintptr_t localSize, const uintptr_t address, uint32_t *offset);

// label, report, notify data
uintptr_t cellGcmSpuGetLabelAddress(const uint32_t index, const uintptr_t base);
uintptr_t cellGcmSpuGetReportDataAddressLocation(const uint32_t index, const uint32_t location, const uint16_t *table, const uintptr_t base);
uintptr_t cellGcmSpuGetNotifyDataAddress(const uint32_t index, const uint16_t *table);

// spu dma put,get funcs for vec_uint4 ls
vec_uint4 cellGcmSpuGetDmaPutVecUint4Ls(uintptr_t ls, uintptr_t ea, uint32_t val);
vec_uint4 cellGcmSpuGetDmaGetVecUint4Ls(uintptr_t ls, uintptr_t ea);


/* flow control function */
void cellGcmSetupContextData(CellGcmContextData *context, const uint32_t *addr, const uint32_t size, CellGcmContextCallback callback);

/* cg functions */
#include<cell/gcm/gcm_cg_func.h>

/* C style functions */
#include <cell/gcm/gcm_command_c.h>

/* Cg extension */
#include <cell/gcm/gcm_cg_extension.h>

#ifdef __cplusplus
}
#endif /* __cplusplus */

// C++ style functions
#ifdef __cplusplus

namespace cell {
namespace Gcm {

#include <cell/gcm/gcm_command_cpp_explicit.h>

} /* namespace Gcm */
} /* namespace Cell */

#endif /* __cplusplus */


#endif	/* __CELL_GCM_SPU_H__ */
