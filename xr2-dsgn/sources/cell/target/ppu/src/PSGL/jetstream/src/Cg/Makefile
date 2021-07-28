# SCE CONFIDENTIAL
# PlayStation(R)3 Programmer Tool Runtime Library 310.001
#                 Copyright (C) 2009 Sony Computer Entertainment Inc.
#                                                All Rights Reserved.

JS_TOP=../..
include $(JS_TOP)/build/defs.mak

CGBIO_DIR := $(JS_TOP)/../cgc/cgbio
CGNVRT_DIR := $(JS_TOP)/tools/src/cgnv2elf

INC_DIR += $(CGBIO_DIR) $(CGNVRT_DIR)
LOCAL_CFLAGS += $(addprefix -I,$(INC_DIR))
LOCAL_CFLAGS += $(addprefix -D,$(LOCAL_DEFINES))

SOURCES= CgCommon.cpp CgParameter.cpp CgTokens.cpp CgContext.cpp CgProgram.cpp CgGL.cpp

SOURCES += $(CGNVRT_DIR)/cgnv2rt.cpp
SOURCES += $(CGBIO_DIR)/cgbio.cpp $(CGBIO_DIR)/cgbiimpl.cpp $(CGBIO_DIR)/cgboimpl.cpp $(CGBIO_DIR)/nvbiimpl.cpp $(CGBIO_DIR)/nvboimpl.cpp
	
# note the +=, it gets flags from the parent makefile.
EXPORT_CFLAGS+=-DCGDLL_EXPORTS -DCGGLDLL_EXPORTS

ARCHIVE=$(LIBDIR)/libCg.a

all: $(ARCHIVE)
clean: clean-objs clean-archive

include $(JS_TOP)/build/rules.mak

