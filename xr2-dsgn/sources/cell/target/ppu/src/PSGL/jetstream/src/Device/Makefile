# SCE CONFIDENTIAL
# PlayStation(R)3 Programmer Tool Runtime Library 310.001
#                 Copyright (C) 2009 Sony Computer Entertainment Inc.
#                                                All Rights Reserved.


# Device object makefile

JS_TOP=../..
include $(JS_TOP)/build/defs.mak

SOURCES=Device.cpp PlatformDevice.cpp

vpath %.cpp $(JS_DEVICE_SUBDIR)

ARCHIVE=$(LIBDIR)/libDevice.a

ifeq ($(JS_USE_GL),1)
	SOURCES+=GL/GLCommon.cpp
endif

ifeq ($(JS_RASTERIZER),JSGCM)
	PLATFORM_CFLAGS += $(FIONA_INCLUDES)
	SOURCES += PlatformDeviceInit.cpp
endif 



all: $(ARCHIVE)
clean: clean-objs clean-archive

include $(JS_TOP)/build/rules.mak
