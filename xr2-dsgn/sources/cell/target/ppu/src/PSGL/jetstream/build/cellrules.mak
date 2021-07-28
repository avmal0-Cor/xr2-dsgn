# SCE CONFIDENTIAL
# PlayStation(R)3 Programmer Tool Runtime Library 310.001
#                 Copyright (C) 2009 Sony Computer Entertainment Inc.
#                                                All Rights Reserved.

$(TARGET_INIT_RUNFILE): $(SOURCE_INIT_RUNFILE) $(RUNDIR)/dirstamp
	cp -f $< $@

copy_files_for_run: $(RUNDIR)/dirstamp
ifneq ($(strip $(SHADERFILE)$(RUNFILE)),)
	@cp -rf $(SHADERFILE) $(RUN_FILES) $(RUNDIR)
endif
