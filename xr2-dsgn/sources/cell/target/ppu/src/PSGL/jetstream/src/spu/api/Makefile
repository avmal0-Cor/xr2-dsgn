# SCE CONFIDENTIAL
# PlayStation(R)3 Programmer Tool Runtime Library 310.001
#                 Copyright (C) 2009 Sony Computer Entertainment Inc.
#                                                All Rights Reserved.

JS_TOP=../../..
include $(JS_TOP)/build/defs.mak

API_LIB=$(LIBDIR)/libspuPSGL.a

API_OBJ=$(OBJDIR)/api.o

SPU_CFLAGS+=-I$(JS_TOP)/include/export-spu

all: $(API_LIB)

clean: clean-objs
	@$(COLOR_CLEAN)
	@$(SHORT_DESC_ECHO) Cleaning $(notdir  $(API_OBJ) $(API_LIB))
	$(COMMAND_QUIET)rm -f $(API_OBJ) $(API_LIB)

$(API_LIB): $(LIBDIR)/dirstamp $(API_OBJ)
	@$(COLOR_COMPILE)
	@$(SHORT_DESC_ECHO) Compiling $< to $(notdir $@)
	$(COMMAND_QUIET)$(SPU_AR) rc $@ $(API_OBJ)
	$(COMMAND_QUIET)$(SPU_RANLIB) $@

$(API_OBJ): api.c $(OBJDIR)/dirstamp 
	@$(COLOR_COMPILE)
	@$(SHORT_DESC_ECHO) Compiling $< to $(notdir $@)
	$(COMMAND_QUIET)$(SPU_GCC) -fpic -fno-zero-initialized-in-bss $(SPU_CFLAGS) -c -o $@ $< 
#	perl $(JS_TOP)/tools/replace_hbr.pl $@

include $(JS_TOP)/build/rules.mak
