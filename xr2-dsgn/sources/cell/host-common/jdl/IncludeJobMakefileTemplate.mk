#########################################################
# SCE CONFIDENTIAL
# PlayStation(R)3 Programmer Tool Runtime Library 310.001
# Copyright (C) 2009 Sony Computer Entertainment Inc.
# All Rights Reserved.
#########################################################

#########################################################
##
##  JDL GENERATED FILE - DO NOT EDIT THIS FILE BY HAND
## 
#########################################################


%(FOR_METHODS)\
MK_TARGET += $(JobFileName_macro)$(jobFilePostfix).mk
%(END)\
%(IF_EQUAL:genPrx:1)\
MK_TARGET += $(managedFilePrefix)$(class)_prx.mk
%(ELSE)%(FOR_METHODS)\
PPU_SRCS += $(spuBinaryPrefix)$(managedFilePrefix)$(JobFileName_macro)$(jobFilePostfix).bin
%(END)%(END)\

PPU_INCDIRS += -I$(build2JdlPath)$(jdl2HeaderPath)
PPU_SRCS += $(build2JdlPath)$(jdl2CommonCppPath)$(managedFilePrefix)$(class).cpp


# Local Variables:
# mode: Makefile
# End:
