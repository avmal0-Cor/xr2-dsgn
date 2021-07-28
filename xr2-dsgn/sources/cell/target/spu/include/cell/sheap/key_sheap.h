/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2006 Sony Computer Entertainment Inc.
* All Rights Reserved.
*/


#ifndef __CELL_SHEAP_KEY_SHEAP_H__
#define __CELL_SHEAP_KEY_SHEAP_H__

#include <stdint.h>
#include <cell/sheap/sheap_base.h>
#include <cell/sheap/error.h>
#include <cell/sheap/sheap_types.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


int
cellKeySheapInitialize(uint64_t ea_sheap,
					   uint64_t size,
					   uint32_t tag);


#define cellKeySheapAllocate(ea_sheap, size) cellSheapAllocate((ea_sheap), (size)) 

#define cellKeySheapFree(ea_sheap,ptr) cellSheapFree((ea_sheap),(ptr))



#define cellKeySheapQueryMax(ea_sheap) cellSheapQueryMax(ea_sheap)
#define cellKeySheapQueryFree(ea_sheap) cellSheapQueryFree(ea_sheap)

#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif /* __CELL_SHEAP_KEY_SHEAP_H__ */

