# SCE CONFIDENTIAL
# PlayStation(R)3 Programmer Tool Runtime Library 310.001
#                 Copyright (C) 2009 Sony Computer Entertainment Inc.
#                                                All Rights Reserved.

JS_TOP=../..
include $(JS_TOP)/build/defs.mak

SOURCES=BlockAlloc.cpp Malloc.cpp Log.cpp NameSpace.cpp FixedAlloc.cpp TexNameSpace.cpp Report.cpp EnumMap.cpp rbtree.cpp jsStackTrace.cpp StackTrace.cpp
SOURCES += STLCheck.cpp


ARCHIVE=$(LIBDIR)/libUtils.a

all: $(ARCHIVE)
clean: clean-objs clean-archive

include $(JS_TOP)/build/rules.mak
