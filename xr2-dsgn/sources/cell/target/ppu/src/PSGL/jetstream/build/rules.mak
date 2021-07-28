# SCE CONFIDENTIAL
# PlayStation(R)3 Programmer Tool Runtime Library 310.001
#                 Copyright (C) 2009 Sony Computer Entertainment Inc.
#                                                All Rights Reserved.

ifeq ($(JS_PLATFORM),cell)
# only the cell platform has special rules for now.
	include $(JS_TOP)/build/$(JS_PLATFORM)rules.mak
endif

ifneq ($(DEP_FILES),)
	DEPENDS_FILE=$(OBJDIR)/Depends
endif

# static pattern rule. 
# This prevents  .o files to be rebuilt when coming from some other part of the build process
# (e.g. spu elf embedded in .o)
# if somebody figures out a way to have only the proper dirstamp as a prerequisite, I'll be glad to hear how.
# (axel)
$(COBJS): $(OBJDIR)/%.o: %.c $(DIRSTAMPS)
	@$(COLOR_COMPILE)
	@$(SHORT_DESC_ECHO) Compiling $(notdir $<) to $(notdir $@)
	$(COMMAND_QUIET)$(CC) $(CFLAGS) $($<_EXTRA_FLAGS) -o $@ -c $<

$(OBJDIR)/%.o: %.cramfs $(DIRSTAMPS)
	$(OBJCOPY) -I binary -O $(TARGET_ELF)-$(TARGET_ARCHITECTURE)$(TARGET_ARCHITECTURE_EXTRA) -B$(TARGET_ARCHITECTURE)  $< $@

$(OBJDIR)/%.o.d: %.cramfs $(DIRSTAMPS)
	touch $@


$(OBJDIR)/%.o.d: %.c $(DIRSTAMPS)
	@$(COLOR_DEPS)
	@$(SHORT_DESC_ECHO) Making deps for $(notdir $<) to $(notdir $@)
	$(COMMAND_QUIET)$(CXX) -M -MP $(CXXFLAGS) -o $@ -MT $(basename $@) -c $<
	$(COMMAND_QUIET)sed -i -e 's|$(basename $@)|& $@|' $@

$(OBJDIR)/%.i: %.c $(DIRSTAMPS)
	@$(COLOR_COMPILE)
	@$(SHORT_DESC_ECHO) Preprocessing $(notdir $<) to $(notdir $@)
	$(COMMAND_QUIET)$(CXX) -E -D_EOL_=_EOL_ $(CXXFLAGS) $< | sed -e "s'_EOL_'\n'g" > $@


$(CXXOBJS) : $(OBJDIR)/%.o: %.cpp $(DIRSTAMPS)
	@$(COLOR_COMPILE)
	@$(SHORT_DESC_ECHO) Compiling $(notdir $<) to $(notdir $@)
	$(COMMAND_QUIET)$(CXX) $(CXXFLAGS) $($<_EXTRA_FLAGS) -o $@ -c $<

$(OBJDIR)/%.o.d: %.cpp $(DIRSTAMPS)
	@$(COLOR_DEPS)
	@$(SHORT_DESC_ECHO) Making deps for $(notdir $<) to $(notdir $@)
	$(COMMAND_QUIET)$(CXX) -M -MP $(CXXFLAGS) -o $@ -MT $(basename $@) -c $<
	$(COMMAND_QUIET)sed -i -e 's|$(basename $@)|& $@|' $@

#
# generated perfmon experiment configurations
#

PARSE_PMCONFIG_TOOL	= $(JS_TOP)/tools/parse_pmconfig.pl

$(GENDIR)/pmconfigs%.c: $(PERFMONDIR)/pmconfigs%.txt $(GENDIR)/dirstamp $(PARSE_PMCONFIG_TOOL)
	@$(COLOR_GEN)
	@$(SHORT_DESC_ECHO) Generating $(notdir $@) from $(notdir $<)
	$(COMMAND_QUIET)perl $(PARSE_PMCONFIG_TOOL) < $< > $@


# shader build rules are raster specific.

# Directories needed
#
#$(OBJDIR):		
#			mkdir -p $@

#$(OBJDIR)/Drawing:		
#			mkdir -p $@
.PHONY: curdir allclean clean clean-archive clean-objs clean-deps $(LIBRARY) clean-gens
#.PHONY: makeindirs-all makeindirs-clean

clean-gens: clean-var-GENS

deps: makeindirs-deps Makefile $(DEP_FILES) $(OBJDIR)/dirstamp
ifneq ($(DEPENDS_FILE),)
	@$(COLOR_DEPS)
	@$(SHORT_DESC_ECHO) Making depends file Depends
	$(COMMAND_QUIET)rm -f $(DEPENDS_FILE)
	$(COMMAND_QUIET)for i in $(DEP_FILES); do echo "-include $$i" >>$(DEPENDS_FILE); done
endif

# rule to generate directories. Uses a time stamp in each directory.
%/dirstamp: 
	@$(COLOR_SHELL)
	@$(SHORT_DESC_ECHO) Creating directory $*
	$(COMMAND_QUIET)mkdir -p $*
	$(COMMAND_QUIET)touch $*/dirstamp

allclean:
	@$(COLOR_CLEAN)
	@$(SHORT_DESC_ECHO) Cleaning build directory $(BUILDDIR)/$(BUILD_NAME)
	$(COMMAND_QUIET)$(RM) -r $(BUILDDIR)/$(BUILD_NAME)

$(ARCHIVE): $(LIBDIR)/dirstamp $(OBJS)
	@$(COLOR_LINK)
	@$(SHORT_DESC_ECHO) Creating archive $(notdir $@) from objects $(notdir $(OBJS))
	$(COMMAND_QUIET)$(AR) rcs $@ $(OBJS)

$(LIBRARY): $(BUILD_LIBRARIES)

$(LIBRARYA): $(LIBDIR)/dirstamp $(OBJS) makeindirs-all
	@$(COLOR_LINK)
	@$(SHORT_DESC_ECHO)
	@$(SHORT_DESC_ECHO) Linking static library $(notdir $(LIBRARYA)) from $(notdir $(OBJS) $(LINK_IN_ARCHIVE))
ifeq ($(SCOPE_NEW_DELETE),1)
	$(COMMAND_QUIET) $(RM) $(LIBRARYA)
	$(COMMAND_QUIET) $(LD) -r -o $(LIBRARYA).tmp $(OBJS) $(LINK_IN_FLAG) $(LINK_IN_ARCHIVE) $(NOLINK_IN_FLAG) $(SO_LIBS_FLAGS)
	$(COMMAND_QUIET) $(OBJCOPY) $(SCOPE_NEW_DELETE_OBJCOPY_FLAGS) $(HACK_REMOVE_TLS_OBJCOPY_FLAGS) $(LIBRARYA).tmp $(LIBRARYA)
	$(COMMAND_QUIET) $(RM) $(LIBRARYA).tmp
else
	$(COMMAND_QUIET) $(RM) $(LIBRARYA)
	$(COMMAND_QUIET) $(LD) -r -o $(LIBRARYA) $(OBJS) $(LINK_IN_FLAG) $(LINK_IN_ARCHIVE) $(NOLINK_IN_FLAG) $(SO_LIBS_FLAGS)
endif

$(LIBRARYSO): $(LIBDIR)/dirstamp $(OBJS) makeindirs-all
	@$(COLOR_LINK)
	@$(SHORT_DESC_ECHO)
	@$(SHORT_DESC_ECHO) Linking shared library $(notdir $(LIBRARYSO)) from $(notdir $(OBJS) $(LINK_IN_ARCHIVE) $(PLATFORM_LIBS))
	$(COMMAND_QUIET) $(RM) $(LIBRARYSO)
	$(COMMAND_QUIET) $(SO) -o $(LIBRARYSO) $(OBJS) $(SO_LINK_IN_FLAG) $(LINK_IN_ARCHIVE) $(SO_NOLINK_IN_FLAG) $(SO_LIBS_FLAGS) $(PLATFORM_LIBS)

#this really should only be for win32 targets
#pass the host for remote execution of LIB if cross compiling
$(VCLIBIMPORT): $(LIBRARYSO)
	@$(COLOR_LINK)
	@$(SHORT_DESC_ECHO)
	@$(SHORT_DESC_ECHO) Making MSVC import library $(notdir $(LIBRARYDEF)) for $(notdir $(LIBRARYSO))
	$(COMMAND_QUIET) $(LIBTOOL) $(JS_HOST_PLATFORM) $(LIBRARYDEF) $(VCLIBIMPORT)

clean-archive: clean-var-ARCHIVE

clean-libs: clean-var-LIBRARYA clean-var-LIBRARYSO clean-var-LIBIMPORT

clean-objs: clean-var-OBJS clean-var-VPSHADER_OBJS clean-var-FPSHADER_OBJS

clean-deps: makeindirs-clean-deps clean-var-DEP_FILES clean-var-DEPENDS_FILE

ifeq ($(SUBDIRS),)
makeindirs-%:
	echo makeindirs: Nothing to do

else
makeindirs-%:
	@for i in $(SUBDIRS);\
	do \
	$(COLOR_MAKE); \
	$(SHORT_DESC_ECHO) Making $* in $$i; \
	$(PRINT_COMMAND) $(MAKE) -C $$i $* ROOTDIR=$(ROOTDIR); \
	$(MAKE) $(MAKE_SILENT_FLAG) -C $$i $* ROOTDIR=$(ROOTDIR) || exit 1;\
	$(COLOR_MAKE); \
	$(SHORT_DESC_ECHO) Done in $$i; \
	$(SHORT_DESC_ECHO); \
	done
endif
curdir:
	@echo $(CURDIR)


ifeq ($(JS_RASTERIZER),JSGCM)
include $(JS_TOP)/build/jsGcmRules.mak
endif



# rules to build/run JS executables and tools.
run: all run-only

ifeq ($(EXE),)
run-only:

else

VPATH += $(LIBDIR)
ifneq ($(NO_GRAPHICS),)
#override graphics stuff
GRAPHICS_LIBS=
JS_DEPS=
endif

$(EXE): $(OBJS) $(BINDIR)/dirstamp $(JS_DEPS)
	@$(COLOR_LINK)
	@$(SHORT_DESC_ECHO)
	@$(SHORT_DESC_ECHO) Linking program $(notdir $@) from $(notdir $(OBJS))
	$(COMMAND_QUIET)$(CXX) $(CFLAGS) $(PLATFORM_LIBPATH) $(EXE_LDFLAGS) $(OBJS) $(LIBS) $(GRAPHICS_LIBS) $(PLATFORM_LIBS) -o $@

$(EXE).self: $(EXE)
	$(MAKE_FSELF) $(EXE) $@

$(EXE).self-sys: $(EXE)
	$(MAKE_SELF_INTERNAL) -input $(EXE) -output $@ sce_test_system

install_shaders_simple: $(RUNDIR)/dirstamp
	@$(COLOR_SHELL)
	@$(SHORT_DESC_ECHO) Installing shaders $(VPSHADER_OBJS) $(FPSHADER_OBJS) along with program $(notdir $(RUN_EXE))
	@cp -f $(VPSHADER_OBJS) $(FPSHADER_OBJS) $(RUNDIR)

run-only: $(PRERUN)
	@$(COLOR_SHELL)
	@echo "NOHOLD" > .dpmrc
	@$(SHORT_DESC_ECHO) Running program $(notdir $(RUN_EXE))
	$(COMMAND_QUIET)LD_LIBRARY_PATH=$(LIBDIR) PATH="$(LIBDIR):$(PATH)" $(LAUNCHER) $(EXE) $(RUN_PARAMS) $(EXTRA_PARAMS)

run-sn: $(PRERUN)
	@$(COLOR_SHELL)
	@echo "NOHOLD" > .dpmrc
	@$(SHORT_DESC_ECHO) Running program $(notdir $(RUN_EXE))
	ps3run.exe -r -f $(RUNDIR) -h $(RUNDIR) $(RUN_EXE) $(EXTRA_PARAMS)

gdb: all gdb-only

gdb-only: $(PRERUN) $(RUNDIR)/dirstamp
	@$(COLOR_SHELL)
	@$(SHORT_DESC_ECHO) Debugging program $(notdir $(RUN_EXE))
	@echo "set args $(RUN_PARAMS) $(EXTRA_PARAMS)" > $(RUNDIR)/.params
	@echo "ld $(notdir $(RUN_EXE))" >> $(RUNDIR)/.params
	@$(COLOR_DEFAULT)
	$(COMMAND_QUIET)LD_LIBRARY_PATH=$(LIBDIR) PATH="$(LIBDIR):$(PATH)" $(GDB_CMDLINE)

clean-exe: clean-var-EXE clean-var-EXE_SELF



# log stuff, to generate states stuff
.PHONY: log

log: $(LOGFILE)

clean-log: clean-var-LOGFILE

$(LOGFILE): $(RUN_EXE) $(OBJDIR)/dirstamp
	@$(COLOR_SHELL)
	@$(SHORT_DESC_ECHO) Getting missing states
	$(COMMAND_QUIET)$(RM) -f $(SHADERFILE)
	$(COMMAND_QUIET)LD_LIBRARY_PATH=$(LIBDIR) PATH="$(LIBDIR):$(PATH)" $(LAUNCHER) $(RUN_EXE) $(RUN_PARAMS) >$@

endif

clean-var-%:
	@$(COLOR_CLEAN)
	@$(SHORT_DESC_ECHO) Cleaning $(notdir $($*))
	$(COMMAND_QUIET)$(RM) -f $($*)


#dependencies
ifneq ($(DEPENDS_FILE),)
-include $(DEPENDS_FILE)
endif
