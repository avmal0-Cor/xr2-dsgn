# SCE CONFIDENTIAL
# PlayStation(R)3 Programmer Tool Runtime Library 310.001
#                 Copyright (C) 2009 Sony Computer Entertainment Inc.
#                                                All Rights Reserved.

$(OBJDIR)/%.vpo: %.cg $(OBJDIR)/dirstamp
	@$(COLOR_COMPILE)
	@$(SHORT_DESC_ECHO) Compiling $(notdir $<) to $(notdir $@) using profile $(NV_VP_PROFILE)
	$(COMMAND_QUIET)$(CGC) $(CGCFLAGS) -profile $(NV_VP_PROFILE) -o $@ $<

$(OBJDIR)/%.fpo: %.cg $(OBJDIR)/dirstamp
	@$(COLOR_COMPILE)
	@$(SHORT_DESC_ECHO) Compiling $(notdir $<) to $(notdir $@) using profile $(NV_FP_PROFILE)
	$(COMMAND_QUIET)$(CGC) $(CGCFLAGS) -profile $(NV_FP_PROFILE) -o $@ $<

$(OBJDIR)/%.vpe: $(OBJDIR)/%.vpo $(OBJDIR)/dirstamp
	@$(COLOR_COMPILE)
	@$(SHORT_DESC_ECHO) Compiling $(notdir $<) to $(notdir $@) using profile $(NV_VP_PROFILE)
	$(CGNV2ELF) $< $@

$(OBJDIR)/%.fpe: $(OBJDIR)/%.fpo $(OBJDIR)/dirstamp
	@$(COLOR_COMPILE)
	@$(SHORT_DESC_ECHO) Compiling $(notdir $<) to $(notdir $@) using profile $(NV_FP_PROFILE)
	$(CGNV2ELF) $< $@

ifneq ($(EXE),)

# how to build a shaders.bin file...
$(SHADERFILE): $(LOGFILE)
	@$(COLOR_COMPILE)
	@$(SHORT_DESC_ECHO) Compiling shaders
	$(COMMAND_QUIET)$(JS_TOP)/tools/js_shader_builder -d -c $(CGC) -p $(NV_FP_PROFILE) -o $@ $<

endif
