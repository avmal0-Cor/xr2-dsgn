#########################################################
# SCE CONFIDENTIAL
# Copyright (C) 2009 Sony Computer Entertainment Inc.
# All Rights Reserved.
#########################################################

#########################################################
##
##  This file can be edited by hand.
##  It is only generated if it is not found.
## 
#########################################################

CELL_MK_DIR ?= $($)(CELL_SDK)/samples/mk
include $($)(CELL_MK_DIR)/sdk.makedef.mk

SPU_INCDIRS += -I$(build2JdlPath)$(jdl2HeaderPath)
SPU_SRCS = $(build2JdlPath)$(jdl2SpuCppPath)$(managedFilePrefix)$(JobFileName_macro)$(jobFilePostfix).cpp
%(IF_NOT_EQUAL:genJobImpl:0)\
SPU_SRCS += $(build2JdlPath)$(jdl2SpuCppPath)$(JobFileName_macro)_impl.cpp
%(END)\
SPU_TARGET = $(managedFilePrefix)$(JobFileName_macro)$(jobFilePostfix).elf
SPU_CFLAGS += $(spursJobBuildFlag) $(picBuildFlag) $(jobCompileFlagsOpt)
SPU_CXXFLAGS += $(spursJobBuildFlag) $(picBuildFlag) $(jobCompileFlagsOpt)
SPU_LDFLAGS += $(spursJobBuildFlag) $(picBuildFlag) $(jobLinkFlagsOpt)

EXTRA_TARGET += $(spuBinaryPrefix)$(managedFilePrefix)$(JobFileName_macro)$(jobFilePostfix).bin

$(spuBinaryPrefix)$(managedFilePrefix)$(JobFileName_macro)$(jobFilePostfix).bin : $(managedFilePrefix)$(JobFileName_macro)$(jobFilePostfix).elf
	$($)(SPU_OBJCOPY) --set-section-flags .bss=load,contents -O binary $< $@

CLEANFILES = $(spuBinaryPrefix)$(managedFilePrefix)$(JobFileName_macro)$(jobFilePostfix).bin

include $($)(CELL_MK_DIR)/sdk.target.mk

