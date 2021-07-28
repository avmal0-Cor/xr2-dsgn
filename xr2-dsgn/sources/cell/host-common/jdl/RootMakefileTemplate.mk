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

# What $(managedFilePrefix)$(class).mk does:
# * Adds make targets for jobs and prx.
# * Adds $(managedFilePrefix)$(class).cpp to PPU_SRCS.
include $(managedFilePrefix)$(class).mk

#
# Edit this makefile to build your application.
#

PPU_LDLIBS += -lspurs_stub -lsysmodule_stub
PPU_SRCS += $(build2JdlPath)$(jdl2PpuCppPath)main.cpp
PPU_TARGET = $(build2TargetPath)main.elf


include $($)(CELL_MK_DIR)/sdk.target.mk

# Local Variables:
# mode: Makefile
# End:

