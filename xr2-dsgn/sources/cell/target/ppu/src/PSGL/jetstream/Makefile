# SCE CONFIDENTIAL
# PlayStation(R)3 Programmer Tool Runtime Library 310.001
#                 Copyright (C) 2009 Sony Computer Entertainment Inc.
#                                                All Rights Reserved.


## Version information
##
JS_LIBRARY_VERSION_NUMBER =	OpenGL ES-CM 1.0
JS_LIBRARY_DATE	     :=	$(shell LC_ALL=C date)
JS_LIBRARY_VERSION   =	$(JS_LIBRARY_VERSION_NUMBER)-$(BUILD_NAME_TAIL) SCE $(JS_LIBRARY_DATE)
export JS_LIBRARY_VERSION_NUMBER
export JS_LIBRARY_VERSION
export JS_LIBRARY_DATE

AUTOBUILD_USER=autobuild
AUTOBUILD_HOST=rdgfx

JS_TOP = .
include $(JS_TOP)/build/defs.mak


all:		target_print_long libs tags

SUBDIRS=src src/JSU src/PSGLFX

ifeq ($(JS_RASTERIZER),JSGCM)
	SUBDIRS+=src/RTCgc
endif


libs: $(MANUAL_SUBDIRS) makeindirs-all


clean: target_print_long makeindirs-clean
	@$(COLOR_CLEAN)
	@$(SHORT_DESC_ECHO) Cleaning core
	$(COMMAND_QUIET)$(RM) core

.PHONY: target_print_long target_print target_print_short tags
target_print_long:
	@$(COLOR_SHELL)
	@$(SHORT_DESC_ECHO) Building platform: $(BUILD_NAME)
	@$(SHORT_DESC_ECHO)

target_print_short: target_print

target_print:
	@echo -n $(BUILD_NAME)

TAG_DIRS = src \
           include

tags:
		@$(COLOR_SHELL)
		@$(SHORT_DESC_ECHO) Creating tags
#ifneq ($(shell which ctags),)			# mingw doesn't ship with ctags
ifneq ($(OSTYPE),msys)
		$(COMMAND_QUIET)-ctags -f tags -R $(TAG_DIRS)
		$(COMMAND_QUIET)-ctags -e -f TAGS -R $(TAG_DIRS)
endif



test:
	@$(SHORT_DESC_ECHO) Making all in test/jstest
	$(COMMAND_QUIET)$(MAKE) --no-print-directory -C test/jstest
	@$(SHORT_DESC_ECHO) Making all in test/ogles
	$(COMMAND_QUIET)$(MAKE) --no-print-directory -C test/ogles

runtest:
	@$(COLOR_MAKE)
	@$(SHORT_DESC_ECHO) Making run in test/jstest
	$(COMMAND_QUIET)$(MAKE) --no-print-directory -C test/jstest run
	@$(SHORT_DESC_ECHO) Making run in test/ogles
	$(COMMAND_QUIET)$(MAKE) --no-print-directory -C test/ogles run

check_images:
	@$(COLOR_MAKE)
	@$(SHORT_DESC_ECHO) Making check in test/jstest
	$(COMMAND_QUIET)$(MAKE) --no-print-directory -C test/jstest check

check_functions:
	@$(COLOR_SHELL)
	$(COMMAND_QUIET)tools/check_functions

reformat_source:
	@$(COLOR_SHELL)
	$(COMMAND_QUIET)tools/REFORMAT_SOURCE_FILES src
rsync:
	@$(COLOR_SHELL)
	@$(SHORT_DESC_ECHO) Syncing build directory to build server $(AUTOBUILD_HOST)
	$(COMMAND_QUIET)rsync -e ssh --delete -Crptgvz $(BUILDDIR)/$(BUILD_NAME) $(AUTOBUILD_USER)@$(AUTOBUILD_HOST):$(BUILDDIR)

oldapiref:
	@$(COLOR_SHELL)
	@$(SHORT_DESC_ECHO) Creating API reference documentation
	$(COMMAND_QUIET)doxygen docs/Doxyfile

apiref:
	@$(COLOR_SHELL)
	@$(SHORT_DESC_ECHO) Creating new look API reference documentation
	$(COMMAND_QUIET)mkdir -p $(DOCDIR)
	$(COMMAND_QUIET)cp docs/Doxy.file $(DOCDIR)
	$(COMMAND_QUIET)sed -e 's|@BUILDDIR@|$(BUILDDIR)|g' -i $(DOCDIR)/Doxy.file
	$(COMMAND_QUIET)doxygen $(DOCDIR)/Doxy.file
	$(COMMAND_QUIET)xsltproc $(DOCDIR)/xml/combine.xslt $(DOCDIR)/xml/index.xml > $(DOCDIR)/xml/combined.xml
	@$(COLOR_MAKE)
	@$(SHORT_DESC_ECHO) Making all in docs/refman
	$(COMMAND_QUIET)$(MAKE) --no-print-directory -C docs/refman

newapiref:
	@$(COLOR_SHELL)
	@$(SHORT_DESC_ECHO) Testing new script for API reference documentation
	cd docs/psglproj; ../lrb/lrbuild -lpsgl_out/log/log.txt psgl_docbuild.xml idd:output-dir=psgl_out; cp -r psgl_out $(DOCDIR)


.PHONY: test runtest pushdocs libs check_images apiref oldapiref

include $(JS_TOP)/build/rules.mak
