# SCE CONFIDENTIAL
# PlayStation(R)3 Programmer Tool Runtime Library 310.001
#                 Copyright (C) 2009 Sony Computer Entertainment Inc.
#                                                All Rights Reserved.


# Makefile to build libPSGL.a

JS_TOP=..
include $(JS_TOP)/build/defs.mak

SOURCES = JS.cpp Light.cpp Matrix.cpp VertexArrays.cpp Texture.cpp Image.cpp
SOURCES += PlatformInit.cpp Raster.cpp Fragment.cpp FixedPoint.cpp FrameBuffer.cpp
SOURCES += PerfCounter.cpp Segment.cpp Buffer.cpp
SOURCES += AsyncCopy.cpp Sync.cpp PlatformReport.cpp Vertex.cpp
SOURCES += FramebufferObject.cpp Query.cpp AttribSet.cpp
SOURCES += FixedPipelineTexture.cpp TextureInternal.cpp

ifeq ($(JS_RUNTIME),hud)
PLATFORM_CFLAGS += -I./Raster/NV/
SOURCES	+= DPM2.cpp
endif

ifneq ($(PTHREAD_LIB),)
SOURCES += PThread/PlatformThread.cpp
else
SOURCES += PlatformThread.cpp
endif

#we can add flags per c file is necessary.
JS.cpp_EXTRA_FLAGS=-DJS_LIBRARY_DATE='"$(JS_LIBRARY_DATE)"' -DJS_LIBRARY_VERSION='"$(JS_LIBRARY_VERSION)"' -DJS_LIBRARY_VERSION_NUMBER='"$(JS_LIBRARY_VERSION_NUMBER)"'


EXPORT_CFLAGS = -DPSGL_EXPORT_SYMBOLS
#XXX this is a hack until I figure out exactly how to build libraries included in dlls on windows with
#our make system, when the lib has entrypoints
# axel.
EXPORT_CFLAGS += -DCGDLL_EXPORTS -DCGGLDLL_EXPORTS
export EXPORT_CFLAGS

vpath %.cpp $(JS_PLATFORM_SUBDIR)

LIBRARY      =  libPSGL
SCOPE_NEW_DELETE=1

# NV/PerfMon
SUBDIRS = Utils Raster Device Cg
ifeq ($(JS_RUNTIME),hud)
SUBDIRS += QM HUD
LINK_IN_ARCHIVE += $(LIBDIR)/libHUD.a $(LIBDIR)/libQM.a ../../hud_common/libpsgl_hudc.a
endif


#LINK_IN_ARCHIVE +=  $(LIBDIR)/libRaster.a $(LIBDIR)/libDevice.a $(LIBDIR)/libCg.a $(LIBDIR)/libUtils.a $(LIBDIR)/libLua.a $(JS_LINK_IN_ARCHIVE)
LINK_IN_ARCHIVE +=  $(LIBDIR)/libRaster.a $(LIBDIR)/libDevice.a $(LIBDIR)/libCg.a $(LIBDIR)/libUtils.a $(JS_LINK_IN_ARCHIVE)

ifeq ($(NEED_APUSIM),1)
	SUBDIRS += $(LIBAPUSIMDIR)
	LINK_IN_ARCHIVE += $(LIBAPUSIM)
endif

# always include elf reading on all platforms, since this is our primary ABI
SUBDIRS += $(LIBELFDIR)
LINK_IN_ARCHIVE += $(LIBELF)

ifeq ($(JS_PLATFORM),cell)
	SUBDIRS += spu/image spu/api
	OTHERDIR = $(JS_TOP)/tools/src/JSPerfTools/other
	LINK_IN_ARCHIVE += $(OBJDIR)/spu/image/image.o
	ifeq ($(JS_RUNTIME),hud)
#		LINK_IN_ARCHIVE += $(OTHERDIR)/C_agent_perf_shm_if.o
		SOURCES	+= Cell/shm_if.cpp
	endif
	SOURCES += spuInterface.cpp
endif

ifeq ($(JS_RSXFIFODUMP),1)
	LINK_IN_ARCHIVE += $(JS_TOP)/../hud_common/librsxfifodump/prebuilt/cell/librsxfifodump_I.a
#LINK_IN_ARCHIVE += $(JS_TOP)/../hud_common/librsxfifodump/prebuilt/cell_debug/librsxfifodump_I.a
endif

ifeq ($(JS_RUNTIME),hud)
	PLATFORM_CFLAGS += -I$(ROOTDIR)/../hud_common/include
endif

# JSGCM includes for GCM specific helpers for Allocating memory, Sizzling data, etc.
ifeq ($(JS_RASTERIZER),JSGCM)
	SOURCES += JSGCM/JSGcmUtils.cpp JSGCM/GmmAlloc.cpp JSGCM/JSGcmSwizzleData.cpp JSGCM/JSGcmTransferData.cpp
	SOURCES += JSGCM/JSGcmCopySurface.cpp JSGCM/JSGcmConsistency.cpp JSGCM/JSGcmZCull.cpp JSGCM/JSGcmInitState.cpp
	SOURCES += JSGCM/JSGcmFifo.cpp JSGCM/JSGcmBindPS.cpp JSGCM/JSGcmBindVS.cpp JSGCM/JSGcmCgUtils.cpp JSGCM/JSRSXFifoDump.cpp

	ifeq ($(JS_RUNTIME),hud)
		SUBDIRS += NV/PerfMon
        LINK_IN_ARCHIVE += $(LIBDIR)/libNVPerfMon.a
	endif

	ifeq ($(JS_PLATFORM),cell)
		SOURCES+=JSGCM/JSGcmFifoCell.cpp
	else
		echo "JSGCM fifo only support on CELL currently"
	endif

        # for shader pool support we need to fix this.
	SUBDIRS += JSGCM/ShaderPool
	LINK_IN_ARCHIVE += $(LIBDIR)/libGcmShader.a

endif




all: makeindirs-all $(LIBRARY)

# For debugging.  This is a target that we can temporarily add to "all:" in order to get preprocessor output.
preprocess:
	echo $(PREPROCESS)
	$(foreach P,$(PREPROCESS),$(MAKE) $(P); )


clean: makeindirs-clean clean-objs clean-libs clean-gens

include $(JS_TOP)/build/rules.mak
