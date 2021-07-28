/*
	SCE CONFIDENTIAL
	PlayStation(R)3 Programmer Tool Runtime Library 310.001
	Copyright (C) 2007 Sony Computer Entertainment Inc.
	All Rights Reserved.
*/
/**
	\internal
	\file fios_ps3ppu.h

	PS3 PPU-specific functions for FIOS.
*/

#ifndef __CELL_FIOS_PS3PPU_H__
#define __CELL_FIOS_PS3PPU_H__

#include <cell/fios/ps3/ppu/fios_media_ps3ppu.h>

namespace cell {
namespace fios {


/** \brief Loads the PS3 module dependencies.

	This function is automatically called by FIOSInit, but you can also
	manually invoke it if you wish to help get the modules for several of your
	dependencies loaded at the same time.
*/
extern FIOS_LINKAGE void FIOSPS3LoadModuleDependencies();

/** \brief Explicit list of FIOS PS3 module dependencies.

	This is an array of PS3 sysmodule IDs, terminated by CELL_SYSMODULE_INVALID.
	These are the modules loaded by FIOSPS3LoadModuleDependencies(). */
extern FIOS_LINKAGE uint16_t g_FIOSPS3ModuleDependencies[];



#ifndef FIOS_SIZET_MAX
#define FIOS_SIZET_MAX   (size_t)(0xFFFFFFFF)
#endif



}; /* namespace fios */
}; /* namespace cell */

#endif /* __CELL_FIOS_PS3PPU_H__ */
