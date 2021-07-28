# SCE CONFIDENTIAL
# PlayStation(R)3 Programmer Tool Runtime Library 310.001
#                 Copyright (C) 2009 Sony Computer Entertainment Inc.
#                                                All Rights Reserved.


# Makefile to generate needed rasterization shaders.
# These shaders are generated as C header files with the shader binary, which are then 
# included by the using files.

JS_TOP=../../..
include $(JS_TOP)/build/defs.mak

TARGETS_VERSIONS=40
SHADER_SOURCES=vpFixedPipeline.cg fpOneTexture.cg fpColorOnly.cg
TARGETS=$(foreach ver,$(TARGETS_VERSIONS),$(patsubst %.cg,%_$(ver).h,$(SHADER_SOURCES)))

all: $(TARGETS)
clean:
	rm -f $(TARGETS)

vp%_40.h: vp%.cg
	$(CGC) -profile sce_vp_$(JS_DRIVER_EXTRA) -unroll none -ao $@ $<
	rm -f ucode.bin

fp%_40.h: fp%.cg
	$(CGC) -profile sce_fp_$(JS_DRIVER_EXTRA) -ao $@ $<
	rm -f ucode.bin

include $(JS_TOP)/build/rules.mak
