////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.10.2008
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <xray/engine/library_dependencies.h>
#include <xray/engine/library_linkage.h>
#include <xray/render/engine/library_linkage.h>

#include <ogg/library_linkage.h>
#include <vorbis/library_linkage.h>
#pragma comment( lib, "x3daudio.lib" )

#include <ode/library_linkage.h>

#include <xray/game/library_linkage.h>

#ifdef XRAY_STATIC_LIBRARIES
	#include <xray/linkage_helper.h>
	XRAY_INCLUDE_TO_LINKAGE(sound_library_linkage)
#endif // #ifdef XRAY_STATIC_LIBRARIES
		
#if XRAY_PLATFORM_XBOX_360
#	pragma comment( lib, "xapilib.lib" )
#endif // #if XRAY_PLATFORM_XBOX_360