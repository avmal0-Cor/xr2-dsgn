# SCE CONFIDENTIAL
# PlayStation(R)3 Programmer Tool Runtime Library 310.001
#                 Copyright (C) 2009 Sony Computer Entertainment Inc.
#                                                All Rights Reserved.

# RS XXX again this is to test setting up a new rasterizer and device with PSGL
# in prep for creating the jsGCM Rasterizer and new JSGCM device

JS_RASTER_SUBDIR=JSGCM
JS_DEVICE_SUBDIR=JSGCM

# need to add the GCM include path
PLATFORM_INCLUDE+=-I$(JS_OSDIR)/ppu/include
# now need to add the internal GCM include path
PLATFORM_INCLUDE+=-I$(JS_OSDIR)/non_release_image/target/ppu/include
PLATFORM_INCLUDE+=-I$(JS_OSDIR)/non_release_image/lv2/target/ppu/include

PLATFORM_CFLAGS += -D_JS_USE_JSGCM
PLATFORM_CFLAGS	+= -D_JS_USE_RESC
PLATFORM_CFLAGS	+= -D_JS_USE_NEW_SYSUTILS
PLATFORM_CFLAGS	+= -D_JS_USE_ASM_090
PLATFORM_CFLAGS	+= -D_JS_USE_INLINE_GCM
PLATFORM_CFLAGS	+= -D_JS_USE_SYSMODULE
PLATFORM_LIBS	+= -lresc_stub
ifeq ($(JS_RUNTIME),gcmhud)
PLATFORM_CFLAGS += $(GCM_HUD_CFLAGS)
PLATFORM_LIBS   += $(GCM_HUD_LFLAGS)
else
PLATFORM_LIBS   += -lgcm_cmd -lgcm_sys_stub
endif
PLATFORM_LIBS   += -lsysmodule_stub -lsysutil_stub

CGC_LIBS = -lPSGLcgc -lcgc

ifeq ($(JS_RUNTIME_CGC),1)
    PLATFORM_CFLAGS += -DJS_RUNTIME_CGC=1
endif

#sce-cgc is taken from the sdk instead of our tools directory 
#also allows the user to define it to an alternate location for testing
CGC ?= $(JS_OSDIR)/host-$(JS_HOST_PLATFORM)/Cg/bin/sce-cgc
CGNV2ELF=$(JS_TOP)/tools/cgnv2elf$(JS_HOST_EXE_SUFFIX)
CGCFLAGS=-q

JS_DRIVER_EXTRA = rsx
ifneq (,$(findstring rsx,$(JS_DRIVER_EXTRA)))
	NV_VP_PROFILE    = sce_vp_rsx
	NV_FP_PROFILE    = sce_fp_rsx
else
	NV_VP_PROFILE    = sce_vp_TypeB
	NV_FP_PROFILE    = sce_fp_TypeB
endif

#VPSHADER_OBJS=$(patsubst %.cg,$(OBJDIR)/%.vpo,$(VPSHADER_SOURCES))
#FPSHADER_OBJS=$(patsubst %.cg,$(OBJDIR)/%.fpo,$(FPSHADER_SOURCES))
VPSHADER_OBJS=$(patsubst %.cg,$(OBJDIR)/%.vpe,$(VPSHADER_SOURCES))
FPSHADER_OBJS=$(patsubst %.cg,$(OBJDIR)/%.fpe,$(FPSHADER_SOURCES))


