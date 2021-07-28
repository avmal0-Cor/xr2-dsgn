# SCE CONFIDENTIAL
# PlayStation(R)3 Programmer Tool Runtime Library 310.001
#                 Copyright (C) 2009 Sony Computer Entertainment Inc.
#                                                All Rights Reserved.


# Makefile to build libRaster.a

JS_TOP=../..
include $(JS_TOP)/build/defs.mak

SOURCES=PlatformRaster.cpp PlatformTexture.cpp PlatformFBops.cpp PlatformTNL.cpp PlatformBuffer.cpp \
	PlatformSync.cpp PlatformFramebuffer.cpp PlatformQuery.cpp PlatformShader.cpp

include $(JS_RASTER_SUBDIR)/MakeSources

vpath %.cpp $(JS_RASTER_SUBDIR)

ARCHIVE=$(LIBDIR)/libRaster.a

all: $(ARCHIVE)
clean: clean-objs clean-archive

include $(JS_TOP)/build/rules.mak
