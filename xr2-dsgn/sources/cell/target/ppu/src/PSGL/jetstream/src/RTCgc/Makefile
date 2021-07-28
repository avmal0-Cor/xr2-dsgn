# SCE CONFIDENTIAL
# PlayStation(R)3 Programmer Tool Runtime Library 310.001
#                 Copyright (C) 2009 Sony Computer Entertainment Inc.
#                                                All Rights Reserved.

JS_TOP=../..
include $(JS_TOP)/build/defs.mak

SOURCES  = RTCgc.cpp

# note the +=, it gets flags from the parent makefile.
EXPORT_CFLAGS+=-DPSGLCGC_EXPORT_SYMBOLS

LIBRARY = libPSGLcgc
SCOPE_NEW_DELETE=1

LIBS = -L$(LIBDIR) -lPSGL
LIBS+=-lcgc -lstdc++

all: $(LIBRARY)
clean: clean-objs clean-libs

include $(JS_TOP)/build/rules.mak
