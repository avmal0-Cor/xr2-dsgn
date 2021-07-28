/*   SCE CONFIDENTIAL                                       */
/*   PlayStation(R)3 Programmer Tool Runtime Library 310.001 */
/*   Copyright (C) 2006 Sony Computer Entertainment Inc.    */
/*   All Rights Reserved.                                   */

#define	CELL_GCM_FUNCTION_MACROS
#include <cell/gcm/gcm_function_macros.h>

#include <cell/gcm/gcm_prototypes_sub.h>

#ifdef __PPU__
#include <cell/gcm/gcm_prototypes_ppu.h>
#endif

#ifndef CELL_GCM_LITTLE_ENDIAN
#if (CELL_GCM_INLINE==0)
#include <cell/gcm/gcm_prototypes_pm.h>
#endif
#endif

#undef	CELL_GCM_FUNCTION_MACROS
#include <cell/gcm/gcm_function_macros.h>
