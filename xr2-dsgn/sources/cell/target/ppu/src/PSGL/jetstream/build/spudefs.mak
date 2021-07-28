# SCE CONFIDENTIAL
# PlayStation(R)3 Programmer Tool Runtime Library 310.001
#                 Copyright (C) 2009 Sony Computer Entertainment Inc.
#                                                All Rights Reserved.

SPU_TOOL_DIR=$(JS_OSDIR)/host-$(JS_HOST_PLATFORM)/spu/bin

SPU_TOOL_PREFIX=spu-lv2

SPU_GCC=$(SPU_TOOL_DIR)/$(SPU_TOOL_PREFIX)-gcc
SPU_AR=$(SPU_TOOL_DIR)/$(SPU_TOOL_PREFIX)-ar
SPU_RANLIB=$(SPU_TOOL_DIR)/$(SPU_TOOL_PREFIX)-ranlib
SPU_STRIP=$(SPU_TOOL_DIR)/$(SPU_TOOL_PREFIX)-strip

SPU_CFLAGS=$(CFLAGS_DEBUG) $(CFLAGS_OPTIMIZE) $(CFLAGS_PLATFORM_DEFINE) $(CFLAGS_TNL_SYNC)  -I$(JSINCDIR)/export -I$(JSINCDIR) -std=gnu9x

ifneq ($(OS_VERSION),0_3_0)
SPU_RAW_FLAG=-mraw
endif

EMBED_SPU_ELF=$(OBJCOPY) -I binary -O $(TARGET_ELF)-$(TARGET_ARCHITECTURE)$(TARGET_ARCHITECTURE_EXTRA) -B$(TARGET_ARCHITECTURE) 
EMBED_SPU_ELF+=--set-section-align .data=7 --set-section-pad .data=128 --rename-section .data=.spu_image.$(notdir $<)

ifeq ($(NEED_APUSIM),1)
	LIBAPUSIMDIR =  $(JS_TOP)/src/libapusim
	LIBAPUSIM	 =  $(LIBDIR)/libapusim.a
	PLATFORM_CFLAGS += -I$(LIBAPUSIMDIR) -DUSE_APUSIM
	PLATFORM_LIBS += $(SOCK_LIB)
endif
