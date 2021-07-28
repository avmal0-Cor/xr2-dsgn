/*   SCE CONFIDENTIAL                                       */
/*   PlayStation(R)3 Programmer Tool Runtime Library 310.001 */
/*   Copyright (C) 2009 Sony Computer Entertainment Inc.    */
/*   All Rights Reserved.                                   */

#ifndef __SYS_SYS_SYS_FIXED_ADDR_H__
#define __SYS_SYS_SYS_FIXED_ADDR_H__

/* process address space mapping area */
#define TEXT_SEGMENT_BASE_ADDR  0x00010000UL

/* the base address of overlay/fixedaddr-prx area for loading a PPU/SPU shared segment */
#define OVERLAY_PPU_SPU_SHARED_SEGMENT_BASE_ADDR	0x30000000UL
#define FIXEDADDR_PRX_BASE_ADDR				0x30000000UL
#define FIXEDADDR_PRX_BASE_ADDR_SIZE			0x10000000UL

/* the base address and size of mmapper's fixed area */
#define MMAPPER_FIXED_AREA_BASE_ADDR                    0xB0000000UL
#define MMAPPER_FIXED_AREA_SIZE                         0x10000000UL

/* the base address of RSX frame buffer */
#define RSX_FB_BASE_ADDR        0xC0000000UL

/* the base address of raw spu */
#define RAW_SPU_BASE_ADDR       0xE0000000UL

/* the base address of spu thread */
#define SPU_THREAD_BASE_ADDR    0xF0000000UL

#endif /* __SYS_SYS_SYS_FIXED_ADDR_H__ */
