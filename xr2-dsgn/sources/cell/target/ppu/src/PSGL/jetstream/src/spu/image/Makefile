# SCE CONFIDENTIAL
# PlayStation(R)3 Programmer Tool Runtime Library 310.001
#                 Copyright (C) 2009 Sony Computer Entertainment Inc.
#                                                All Rights Reserved.

JS_TOP=../../..
include $(JS_TOP)/build/defs.mak

IMAGE_ELF=$(OBJDIR)/image.elf
IMAGE_ELF_EMBED=$(OBJDIR)/image.o

all: $(IMAGE_ELF_EMBED)

dump: $(OBJDIR)/image.elf.dump

%.dump:%
	spu-objdump -drt $< >$@

clean: clean-objs
	@$(COLOR_CLEAN)
	@$(SHORT_DESC_ECHO) Cleaning $(notdir  $(IMAGE_ELF_EMBED) $(IMAGE_ELF))
	$(COMMAND_QUIET)rm -f $(IMAGE_ELF_EMBED) $(IMAGE_ELF)

$(IMAGE_ELF_EMBED): $(IMAGE_ELF)
	@$(COLOR_LINK)
	@$(SHORT_DESC_ECHO) Embedding $(notdir $<) into $(notdir $@)
	$(COMMAND_QUIET)cd $(dir $<); $(EMBED_SPU_ELF) $(notdir $<) $(notdir $@)

$(IMAGE_ELF): image.c $(OBJDIR)/dirstamp
	@$(COLOR_COMPILE)
	@$(SHORT_DESC_ECHO) Compiling $< to $(notdir $@)
	$(COMMAND_QUIET)$(SPU_GCC) $(SPU_CFLAGS) $(SPU_RAW_FLAG) -o $@ $< 
	$(COMMAND_QUIET)$(SPU_STRIP) $@
#	perl $(JS_TOP)/tools/replace_hbr.pl $@

include $(JS_TOP)/build/rules.mak
