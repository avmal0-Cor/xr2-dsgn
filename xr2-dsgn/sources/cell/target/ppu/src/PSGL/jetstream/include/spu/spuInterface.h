/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 *                Copyright (C) 2009 Sony Computer Entertainment Inc.
 *                                               All Rights Reserved.
 */

#ifndef SPU_INTERFACE_H
#define SPU_INTERFACE_H

#include <sys/raw_spu.h>
#include <PSGL/readelf.h>
#include <spu/spuTypes.h>

#include <PSGL/sdk_030_to_040_compat.h>

int _jsGetSpu( void );
sys_raw_spu_t _jsGetRawSpuId( int spu );

int loadElf( int spu, ELF_t *elf );
int runSpu( int spu );
int runSpuIfPaused( int spu );
int pauseSpu( int spu );
int waitForSpu( int spu );
void setSpuNextPC( int spu, unsigned int pc );
void copyToSpuImmediate( int spu, unsigned int LSaddress, const void *EA, unsigned int size );

unsigned int js_lwarx( void *ea );
int  js_stwcx( unsigned int value, void *ea );
void js_st( unsigned int value, void *ea );

#endif
