/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2006 Sony Computer Entertainment Inc.
* All Rights Reserved.
*/




#ifndef __CELL_OVIS_MAPPER_H__
#define __CELL_OVIS_MAPPER_H__


#include <stdint.h>
#include <cell/ovis/debug.h>

#define sOvisLsSize 0x040000

#ifdef __cplusplus
extern "C" {
#endif

int _cellOvisStartMapping(_ovly_table_t __ovly_info, 
						  uint64_t ea_ovly_table, 
						  uint32_t tag, uint32_t offset);

void _cellOvisWaitMapping(_ovly_table_t *__ovly_info, uint32_t tag);

#ifdef __cplusplus
}
#endif



#define CELL_OVIS_DEFINE_MAPPER(secname)\
extern _ovly_table_t __ovly_info_##secname ;\
__attribute__((always_inline))\
static inline int cellOvisStartMapping_##secname(uint64_t ea_ovly_table, uint32_t tag) \
{\
    if((ea_ovly_table%128)!=0){ return CELL_OVIS_ERROR_ALIGN; }	\
	if(tag>=32){ return CELL_OVIS_ERROR_INVAL; }	\
    uint32_t offset; \
	uint32_t temp; \
	__asm__ ( " brsl %[out], .Label%= \n" \
		 ".Label%=: \n" \
		 " ila %[temp], .Label%= \n" \
		 " sf %[out], %[temp], %[out] \n" \
		 : [out] "=r" (offset), \
		 [temp] "=r" (temp) ); \
   return _cellOvisStartMapping(__ovly_info_##secname, ea_ovly_table, tag, offset);\
}\
static inline int cellOvisWaitMapping_##secname(uint32_t tag) \
{\
   if(tag>=32){ return CELL_OVIS_ERROR_INVAL; }	\
   _cellOvisWaitMapping(&__ovly_info_##secname, tag);\
   return CELL_OK;\
}


#endif /* __CELL_OVIS_MAPPER_H__ */
