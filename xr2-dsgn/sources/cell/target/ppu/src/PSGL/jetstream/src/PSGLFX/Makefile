# SCE CONFIDENTIAL
# PlayStation(R)3 Programmer Tool Runtime Library 310.001
#                 Copyright (C) 2009 Sony Computer Entertainment Inc.
#                                                All Rights Reserved.


# Cg Runtime makefile


JS_TOP=../..
include $(JS_TOP)/build/defs.mak

SOURCES  = Effect.cpp Context.cpp Technique.cpp
SOURCES += Annotation.cpp Pass.cpp StateAssignment.cpp
SOURCES += State.cpp GL.cpp Parameter.cpp Program.cpp

# note the +=, it gets flags from the parent makefile.
EXPORT_CFLAGS+=-DPSGLFX_EXPORT_SYMBOLS

LIBRARY = libPSGLFX
SCOPE_NEW_DELETE=1

LIBS = -L$(LIBDIR) -lPSGL

all: $(LIBRARY)
clean: clean-objs clean-libs

include $(JS_TOP)/build/rules.mak
