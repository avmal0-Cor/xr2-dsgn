# SCE CONFIDENTIAL
# PlayStation(R)3 Programmer Tool Runtime Library 310.001
#                 Copyright (C) 2009 Sony Computer Entertainment Inc.
#                                                All Rights Reserved.

## Source sub-directory
ifeq ($(ROOTDIR),)
#        ifeq ($(shell which readlink),) # mingw doesn't ship with readlink
        ifeq ($(OSTYPE),msys)
            ROOTDIR:=$(shell cd $(JS_TOP); pwd)
        else
            ROOTDIR:=$(shell readlink -f $(JS_TOP))
        endif
endif
SUBDIR=$(subst $(ROOTDIR)/,,$(CURDIR)/)

## Platform-specific
ifeq ($(JS_HOST_PLATFORM),)
ifeq ($(shell uname),Linux)
	JS_HOST_PLATFORM=linux
else
	JS_HOST_PLATFORM=win32
	JS_HOST_EXE_SUFFIX=.exe
endif
export JS_HOST_PLATFORM
export JS_HOST_EXE_SUFFIX
endif

JS_PLATFORM ?= cell
JS_RASTERIZER ?= JSGCM

GOOD_PLATFORM=0
ifneq ($(findstring -,$(JS_PLATFORM)),)
	ifneq ($(findstring GL,$(JS_PLATFORM)),)
		JS_RASTERIZER=GL
	endif
	ifneq ($(findstring debug,$(JS_PLATFORM)),)
		JS_DEBUG=1
	endif
	ifneq ($(findstring opt,$(JS_PLATFORM)),)
		JS_DEBUG=0
	endif
	ifneq ($(findstring cell,$(JS_PLATFORM)),)
		JS_PLATFORM=cell
		GOOD_PLATFORM=1
	endif
	ifneq ($(findstring linux,$(JS_PLATFORM)),)
		JS_PLATFORM=linux
		GOOD_PLATFORM=1
	endif
	ifneq ($(findstring win32,$(JS_PLATFORM)),)
		JS_PLATFORM=win32
		GOOD_PLATFORM=1
	endif
	ifneq ($(GOOD_PLATFORM),1)
		JS_PLATFORM=
	endif
endif
## - default platform
ifeq ($(JS_PLATFORM),)
	JS_PLATFORM=$(JS_HOST_PLATFORM)
	GOOD_PLATFORM=1
endif


# this is required here instead of in the platform because spudefs uses it.
DEFAULT_OS_VERSION=1_7_0
OS_VERSION ?= $(DEFAULT_OS_VERSION)$(OS_EXTRA_VERSION)


# assume OS_VERSION is of the form x_y_z.$$$$$$, where x, y and z are single digits
# OS_VERSION_BASE will be x_y_z
# OS_VERSION_NUMERIC will be xyz (a decimal number)

ifeq ($(OS_VERSION_BASE),)
export OS_VERSION_BASE := $(shell echo "$(OS_VERSION)" |sed -e 's/^\([0-9]_[0-9]_[0-9]\).*$$/\1/g')
endif
ifeq ($(OS_VERSION_NUMERIC),)
export OS_VERSION_NUMERIC := $(shell echo "$(OS_VERSION_BASE)" |sed -e 's/_//g')
endif

ifeq ($(shell [ $(OS_VERSION_NUMERIC) -ge 080 ] && echo 1),1)
OS_VERSION_080_PLUS=1
endif
ifeq ($(shell [ $(OS_VERSION_NUMERIC) -ge 081 ] && echo 1),1)
OS_VERSION_081_PLUS=1
endif
ifeq ($(shell [ $(OS_VERSION_NUMERIC) -ge 082 ] && echo 1),1)
OS_VERSION_082_PLUS=1
endif
ifeq ($(shell [ $(OS_VERSION_NUMERIC) -ge 083 ] && echo 1),1)
OS_VERSION_083_PLUS=1
endif
ifeq ($(shell [ $(OS_VERSION_NUMERIC) -ge 085 ] && echo 1),1)
OS_VERSION_085_PLUS=1
endif
ifeq ($(shell [ $(OS_VERSION_NUMERIC) -ge 086 ] && echo 1),1)
OS_VERSION_086_PLUS=1
endif
ifeq ($(shell [ $(OS_VERSION_NUMERIC) -ge 090 ] && echo 1),1)
OS_VERSION_090_PLUS=1
endif
ifeq ($(shell [ $(OS_VERSION_NUMERIC) -ge 100 ] && echo 1),1)
OS_VERSION_100_PLUS=1
endif

ifeq ($(JS_OSDIR),)

# on 0.8.0+, no legacy variable is used, the OS is in SVN. to override, define JS_OSDIR.
ifeq ($(OS_VERSION_080_PLUS),1)

# needs absolute path
JS_OSDIR ?= $(ROOTDIR)/../sdk/$(OS_VERSION)

else


# CELL_SDK
JS_OSDIR ?= $(CELL_SDK)

#last resort, /usr/local/cell/$version, for pre-0.8.0
JS_OSDIR ?= /usr/local/cell/$(OS_VERSION)

endif

export JS_OSDIR

endif

ifeq ($(JS_DEBUG),1)
# on debug, enable debug channel. This still requires the user to set
# _psglDebugChannel=1
	JS_DEBUG_CHANNEL=1
endif


## - platform-specific include
include $(JS_TOP)/build/$(JS_PLATFORM)defs.mak


## - Rasterizer : LibGcm, Rasterizer
ifeq ($(JS_RASTERIZER),JSGCM)
	include $(JS_TOP)/build/jsGcmDefs.mak
endif


include $(JS_TOP)/build/spudefs.mak


ifeq ($(GCM_HUD),yes) # pull from the rsxtools tree...
else
GCM_HUD_LFLAGS ?= $(JS_TOP)/prebuilt/cell-elf32/lib/libgcm_hud.a -lgcm_pm -lfs_stub -lsheap_stub
GCM_HUD_CFLAGS ?= -I$(JS_TOP)/prebuilt/cell-elf32/include
endif


ifeq ($(JS_RUNTIME),hud)
	JS_RUNTIMENAME=-hud
else
        ifeq ($(JS_RUNTIME),gcmhud)
		JS_RUNTIMENAME=-gcmhud
	else
		JS_RUNTIMENAME=
	        ifeq ($(JS_PERF),1)
		        BUILD_EXTRA_NAME:=$(BUILD_EXTRA_NAME)-perf
		endif
        endif
endif


ifeq ($(JS_DEBUG),1)
	JS_DEBUGNAME=debug
	JS_DEBUG_SUFFIX=_debug
else
	JS_DEBUGNAME=opt
	JS_DEBUG_SUFFIX=
endif

ifeq ($(JS_NO_ERROR_CHECK),1)
	JS_CHECKNAME=-nocheck
else
	JS_CHECKNAME=
endif

# export config for sub-makes
export JS_PLATFORM
export JS_RUNTIME
export JS_RASTERIZER
export JS_DEBUG

ifneq ($(JS_DEBUG),1)
ifeq ($(JS_DEBUG_CHANNEL),1)
BUILD_EXTRA_NAME:=$(BUILD_EXTRA_NAME)-channel
endif
endif

BUILD_NAME = $(JS_PLATFORM)-$(JS_RASTERIZER)-$(BUILD_NAME_TAIL)-$(OS_VERSION)
BUILD_NAME_TAIL = $(JS_DEBUGNAME)$(JS_CHECKNAME)$(JS_RUNTIMENAME)$(BUILD_EXTRA_NAME)$(JS_DRIVER_EXTRA)$(JS_NV_LOW_LEVEL_EXTRA)


## Directories.
##
# by default, build in a global directory.
# since some people (like me, axel) like to have build directories along with the tree
# to allow multiple check outs, offer the option.
# set JS_LOCAL_BUILD to get a build directory at the top of jetstream.
ifeq ($(BUILDDIR),)
ifeq ($(JS_LOCAL_BUILD),)
	BUILDDIR     =  $(JS_TOP)/../jsbuild
else
	BUILDDIR     =  $(ROOTDIR)/jsbuild
endif
endif

JSINCDIR     =	$(JS_TOP)/include


RUNDIR       =  $(BUILDDIR)/$(BUILD_NAME)/run
BINDIR	     =  $(BUILDDIR)/$(BUILD_NAME)/bin
OBJDIR	     =  $(BUILDDIR)/$(BUILD_NAME)/obj/$(SUBDIR)
GENDIR	     =  $(OBJDIR)/gen
LIBDIR       =  $(BUILDDIR)/$(BUILD_NAME)/lib
# make sure the Doxygen configuration file also points to same directory
DOCDIR       =  $(BUILDDIR)/docs

OBJDIRS	     =  $(sort $(OBJDIR) $(addprefix $(OBJDIR)/,$(dir $(SOURCES))))

#internal libraries
LIBELFDIR =  $(JS_TOP)/src/libelf
LIBELF	 =  $(LIBDIR)/libelf.a

## Tools
##
SHELL	     =  /bin/sh
RM           =	/bin/rm -f
WC           =  wc -l

## Be quiet
##
## JS_QUIET is the combination of the following letters :
## C = don't show commands that are executed
## D = don't show the short description for the command
## c = don't use ANSI colors
##
## Default (empty) is "Dc" (like normal make)
## To show everything, set JS_QUIET to A
ifeq ($(JS_QUIET),)
	JS_QUIET=Dc
endif
ifeq ($(findstring C,$(JS_QUIET)),)
	PRINT_COMMAND=/bin/echo
else
	COMMAND_QUIET=@
	PRINT_COMMAND=true
	MAKE_SILENT_FLAG=-s
endif
ifeq ($(findstring D,$(JS_QUIET)),)
	SHORT_DESC_ECHO=/bin/echo
else
	SHORT_DESC_ECHO=true
endif
ifeq ($(findstring c,$(JS_QUIET)),)
	COLOR_ECHO=/bin/echo
else
	COLOR_ECHO=true
endif

#These define Colors for each type of action
#If you want to deactivate colors, please use 'export JS_QUIET=c'
# red
COLOR_CLEAN=$(COLOR_ECHO) -e  -n '\033[31m'
# yellow
COLOR_MAKE=$(COLOR_ECHO) -e -n '\033[33m'
# green
COLOR_COMPILE=$(COLOR_ECHO) -e -n '\033[32m'
# blue
COLOR_LINK=$(COLOR_ECHO) -e -n '\033[34m'
# default foreground
COLOR_SHELL=$(COLOR_ECHO) -e -n '\033[39m'
COLOR_GEN=$(COLOR_ECHO) -e -n '\033[39m'
COLOR_DEFAULT=$(COLOR_ECHO) -e -n '\033[39m'
# cyan
COLOR_DEPS=$(COLOR_ECHO) -e -n '\033[36m'
# default
## compilation flags
##
ifeq	($(JS_DEBUG),1)
	CFLAGS_DEBUG=-g -DJS_DEBUG=1 -DJS_REPORT
	CFLAGS_OPTIMIZE=
	CXXFLAGS_OPTIMIZE=
else
	CFLAGS_DEBUG=-DNDEBUG

# [RSTENSON] until I can figure out the problem with opt 02 and 03 I will override the 03 default for PSGL opt
# to be 01 here only for JSGCM/Opt so that we won't affect the NV opt build settings .
	ifeq ($(JS_RASTERIZER),JSGCM)
		CFLAGS_OPTIMIZE=-O3
		CXXFLAGS_OPTIMIZE=-O3
	else
		CFLAGS_OPTIMIZE=-O3
		CXXFLAGS_OPTIMIZE=-O3
	endif
endif
ifeq ($(JS_RUNTIME),hud)
	CFLAGS_RUNTIME=-DJS_DPM -DJS_DPM_V2 -DJS_REPORT -DJS_ALLOCINFO
	JS_PERF=1
endif
ifeq ($(JS_RUNTIME),gcmhud)
	CFLAGS_RUNTIME=-DGCM_HUD
endif

ifeq ($(JS_PERF),1)
	CFLAGS_PERF=-DJS_PERFORMANCE_INFO -DJS_APIENTRY_TIMERS
endif
ifeq ($(JS_NO_ERROR_CHECK),1)
	CFLAGS_NOCHECK=-DJS_NO_ERROR_CHECK
endif

ifneq ($(JS_NO_WARN_ERROR),1)
	CFLAGS_ERR=-Werror -fno-strict-aliasing
endif

JS_AUTHOR_CFLAGS ?=

JSCFLAGS	= $(PLATFORM_INCLUDE) -I$(JSINCDIR)/export -I$(JSINCDIR) -I$(JSINCDIR)/protected -I$(JSINCDIR)/$(JS_RASTERIZER) -I$(JSINCDIR)/$(JS_PLATFORM_SUBDIR) -I$(GENDIR) $(CG_DEVELOPMENT_INCLUDE) -I$(PREBUILT_DIR)/include -DJS

COMMON_FLAGS = $(CFLAGS_DEBUG) $(CFLAGS_PLATFORM_DEFINE) $(CFLAGS_TNL_SYNC) $(CFLAGS_PERF) $(CFLAGS_RUNTIME) $(CFLAGS_NOCHECK) $(JSCFLAGS) $(JS_AUTHOR_CFLAGS) $(PLATFORM_CFLAGS) $(LOCAL_CFLAGS) $(CFLAGS_ERR)
CFLAGS      =	$(COMMON_FLAGS) $(CFLAGS_OPTIMIZE) -std=gnu9x -Wall
CXXFLAGS     =	$(COMMON_FLAGS) $(CXXFLAGS_OPTIMIZE) -fno-common -fno-exceptions -fno-rtti -Wall
LDFLAGS      =	-Wl,-Map,$(JSLIBA).map -nostartfiles -L$(LIBDIR) -lm $(LOCAL_LDFLAGS)
EXE_LDFLAGS  =  -L$(SDKBUILT_LIBDIR) -L$(PREBUILT_DIR)/lib

OBJS=$(addprefix $(OBJDIR)/,$(addsuffix .o,$(basename $(SOURCES) $(CRAMFS_FILE))))
GENS=$(addprefix $(GENDIR)/,$(GEN_FILES))
CSOURCES=$(filter %.c,$(SOURCES))
CXXSOURCES=$(filter %.cpp,$(SOURCES))

COBJS=$(addprefix $(OBJDIR)/,$(addsuffix .o,$(basename $(CSOURCES))))
CXXOBJS=$(addprefix $(OBJDIR)/,$(addsuffix .o,$(basename $(CXXSOURCES))))
DIRSTAMPS=$(addsuffix dirstamp,$(dir $(OBJS)))
PREPROCESS=$(addprefix $(OBJDIR)/,$(addsuffix .i,$(basename $(SOURCES))))

#DEP_OBJ_FILES=$(addprefix $(OBJDIR)/,$(addsuffix .d,$(SOURCES)))
DEP_OBJ_FILES=$(addsuffix .d,$(OBJS))
DEP_FILES=$(strip $(DEP_OBJ_FILES) $(LOCAL_DEP_FILES))

LIBRARYA=$(LIBDIR)/$(LIBRARY).a
LIBRARYSO=$(LIBDIR)/$(LIBRARY).$(SOEXT)
LIBRARYDEF=$(LIBDIR)/$(LIBRARY).def
LIBIMPORT=$(LIBRARYSO).a
VCLIBIMPORT=$(LIBDIR)/$(LIBRARY).lib

LINK_IN_FLAG   = --whole-archive
NOLINK_IN_FLAG = --no-whole-archive
SO_LINK_IN_FLAG   = -Wl,$(LINK_IN_FLAG)
SO_NOLINK_IN_FLAG = -Wl,$(NOLINK_IN_FLAG)

LIBHACK_LDFLAGS=-liohack -lz -Wl,--wrap -Wl,fclose  -Wl,--wrap -Wl,fread -Wl,--wrap -Wl,fopen  -Wl,--wrap -Wl,feof -Wl,--wrap -Wl,fseek -Wl,--wrap -Wl,ftell -Wl,--wrap -Wl,rewind

# wrapping placement new seems to cause a problem with cgc, so don't do it. it doesn't really matter for that one.
#NEW_DELETE_SYMBOLS=$(foreach f,_Znwj _Znaj _ZdlPv _ZdaPv,$(f) $(f)j $(f)Pv)

# _Znw is operator new
# _Zna is operator new[]
# _Zdl is operator delete
# _Zda is operator delete[]
# suffixes represent parameters, in order: j is 'unsigned int', Pv is 'void *'
NEW_DELETE_SYMBOLS=$(foreach f,_Znwj _Znaj _ZdlPv _ZdaPv,$(f) $(f)j)
SCOPE_NEW_DELETE_OBJCOPY_FLAGS=$(foreach s,$(NEW_DELETE_SYMBOLS), --redefine-sym .$(s)=.__PSGL__$(s))
SCOPE_NEW_DELETE_OBJCOPY_FLAGS+=$(foreach s,$(NEW_DELETE_SYMBOLS), --redefine-sym $(s)=__PSGL__$(s))
SCOPE_NEW_DELETE_OBJCOPY_FLAGS+=$(foreach s,$(NEW_DELETE_SYMBOLS), --rename-section .gnu.linkonce.t.$(s)=.gnu.linkonce.t.__PSGL__$(s))

JS_LIBS=-lPSGLFX -lPSGL -lPSGLU
GRAPHICS_LIBS=-L$(LIBDIR) $(JS_LIBS) $(CGC_LIBS) -lpng -lz

# remove all default rules from make, to prevent unused pattern matching.
.SUFFIXES:

ifeq ($(JS_RSXFIFODUMP),1)
# on rsxfifodump, enable debug channel. This requires the user to set
# _psglDebugChannel=4 or =5 for RSXFIFODUMP facilities.
        JS_DEBUG_CHANNEL=1
	PLATFORM_CFLAGS += -DJS_RSXFIFODUMP
	PLATFORM_CFLAGS += -I$(JS_TOP)/../hud_common/librsxfifodump/include/public
endif

