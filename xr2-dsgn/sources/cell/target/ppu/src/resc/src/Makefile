#   SCE CONFIDENTIAL                                      
#   PlayStation(R)3 Programmer Tool Runtime Library 310.001
#   Copyright (C) 2007 Sony Computer Entertainment Inc.   
#   All Rights Reserved.                                  

CELL_SDK ?= /usr/local/cell
CELL_MK_DIR ?= $(CELL_SDK)/samples/mk

include $(CELL_MK_DIR)/sdk.makedef.mk

LIBNAME				:= libresc
INSTALL_DIR			:= ../lib

PPU_CXXWARNFLAGS	= $(CXXWARNFLAGS_L2)

PPU_INCDIRS			+= -I../include

PPU_LIB_TARGET		= $(INSTALL_DIR)/$(LIBNAME).a

PPU_SRCS 			= export_c.cc rescmain.cc shader.cc util.cc \
					   RescBilinear.vp RescBilinear.fp RescBilinearPal.fp \
					   RescInterlaceFilter.fp RescInterlaceFilterPal.fp

PPU_CXXFLAGS		+= -fno-exceptions -fno-rtti

# この値が１以上で定義してあると、デバッグ出力が得られる。通常は１がよい
ifdef CELL_RESC_DEBUG
  PPU_CXXFLAGS		+= -DCELL_RESC_DEBUG=$(CELL_RESC_DEBUG)
else
  PPU_DBG_FLAGS		=  
endif

include $(CELL_MK_DIR)/sdk.target.mk
