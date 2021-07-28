# SCE CONFIDENTIAL
# PlayStation(R)3 Programmer Tool Runtime Library 310.001
#                 Copyright (C) 2009 Sony Computer Entertainment Inc.
#                                                All Rights Reserved.

JS_TOP=../..
include $(JS_TOP)/build/defs.mak

SOURCES = JSU.cpp

EXPORT_CFLAGS = -DPSGLU_EXPORT_SYMBOLS

LIBRARY = libPSGLU
SCOPE_NEW_DELETE=1

LIBS = -L$(LIBDIR) -lPSGL

all: $(LIBRARY)

clean: clean-objs clean-libs

include $(JS_TOP)/build/rules.mak
