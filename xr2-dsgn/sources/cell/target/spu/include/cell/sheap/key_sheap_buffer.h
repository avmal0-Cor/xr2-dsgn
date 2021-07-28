/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2006 Sony Computer Entertainment Inc.
* All Rights Reserved.
*/



#ifndef __CELL_SHEAP_KEY_SHEAP_BUFFER_H__
#define __CELL_SHEAP_KEY_SHEAP_BUFFER_H__

/**
 *  @addtogroup SHEAP SHEAP
 */
/**
 *  @addtogroup PU PU
 *  @ingroup SHEAP 
 */
/**
 *  @addtogroup CellKeySheapBuffer
 *  @ingroup PU 
 */
/*@{*/

#include <stdint.h>
#include <cell/sheap/key_sheap.h>
#include <cell/sheap/error.h>
#include <cell/sheap/sheap_types.h>


#ifdef __cplusplus
extern "C" {
#endif

int 
cellKeySheapBufferNew(CellKeySheapBuffer*, uint64_t ksheap, CellSheapKey key, uint64_t size);

void
cellKeySheapBufferDelete(CellKeySheapBuffer* obj);

#ifdef __cplusplus
}
#endif


#if __GNUC__ >= 3
__attribute__((always_inline))
#endif
static inline uint64_t 
cellKeySheapBufferGetEa(CellKeySheapBuffer* obj)
{
	uint64_t ea = obj->ea;
	return ea;
}

#if __GNUC__ >= 3
__attribute__((always_inline))
#endif
static inline uint64_t 
cellKeySheapBufferGetSize(CellKeySheapBuffer* obj)
{
  return obj->size;
}



#endif /* __CELL_SHEAP_KEY_SHEAP_BUFFER_H__ */
