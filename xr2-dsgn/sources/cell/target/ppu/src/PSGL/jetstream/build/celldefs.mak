# SCE CONFIDENTIAL
# PlayStation(R)3 Programmer Tool Runtime Library 310.001
#                 Copyright (C) 2009 Sony Computer Entertainment Inc.
#                                                All Rights Reserved.

PU_PREFIX=ppu

PU_TOOL_DIR=$(JS_OSDIR)/host-$(JS_HOST_PLATFORM)/$(PU_PREFIX)/bin
export DT_TOOL_DIR=$(JS_OSDIR)/host-$(JS_HOST_PLATFORM)/bin

# brio.a requires stdc++. go figure.
OS_LIBC=-lstdc++

CC           = $(PU_TOOL_DIR)/$(PU_PREFIX)-lv2-gcc
CXX          = $(PU_TOOL_DIR)/$(PU_PREFIX)-lv2-g++
AR           = $(PU_TOOL_DIR)/$(PU_PREFIX)-lv2-ar
LD           = $(PU_TOOL_DIR)/$(PU_PREFIX)-lv2-ld
OBJCOPY      = $(PU_TOOL_DIR)/$(PU_PREFIX)-lv2-objcopy
SO           = $(CC) -shared
SOEXT        = so

MAKE_FSELF = $(JS_OSDIR)/host-$(JS_HOST_PLATFORM)/bin/make_fself$(JS_HOST_EXE_SUFFIX)

TARGET_ELF = elf64
ifneq ($(OS_VERSION_080_PLUS),)
LV2_ABI = elf32
MARCH_CFLAGS = 
TARGET_ARCHITECTURE = powerpc
TARGET_ARCHITECTURE_EXTRA=-celloslv2
else
LV2_ABI = elf64
MARCH_CFLAGS = -mpowerpc64 
TARGET_ARCHITECTURE = powerpc
endif

BUILD_LIBRARIES=$(LIBRARYA)

JS_PLATFORM_SUBDIR = Cell

# in the C code, OS_VERSION_NUMERIC is defined as hex to prevent problems with the leading 0.
# That allows for comparisons in the code (version is more than x.y.z)
CFLAGS_PLATFORM_DEFINE = -DJS_PLATFORM_CELL -DOS_VERSION_$(OS_VERSION) -DOS_VERSION_NUMERIC=0x$(OS_VERSION_NUMERIC)

PLATFORM_INCLUDE += -I$(JSINCDIR)/STL
PLATFORM_CFLAGS += $(MARCH_CFLAGS) -fno-exceptions 

DEFAULT_REMOTE_ROOT=/app_home/
ifdef REMOTE_ROOT
PLATFORM_CFLAGS += -DREMOTE_ROOT="\"$(REMOTE_ROOT)\""
endif

ifeq ($(OS_VERSION_090_PLUS),1)
BUS_DRIVER = -lio_stub
else
BUS_DRIVER = -lio
endif

# We probably should not put our libraries along with the system
# libs because some linker flags change the lib paths that are searched.
PLATFORM_LIBS += $(SO_LINK_IN_FLAG) $(BUS_DRIVER) $(SO_NOLINK_IN_FLAG) $(OS_LIBC)

ifeq ($(JS_RUNTIME),hud)
ifneq ($(OS_VERSION_090_PLUS),1)
PLATFORM_LIBS += -lkb
endif
ifeq "$(shell if test -e $(JS_OSDIR)/target/ppu/lib/libperf.a; then echo 1; fi)" "1" # since 092
PLATFORM_LIBS += -lperf
endif
endif

SDKBUILT_LIBDIR=$(JS_OSDIR)/target/ppu/lib

#EXE_LDFLAGS = -Wl,--gc-sections -static

ifeq ($(JS_RASTERIZER),)
	JS_RASTERIZER=NoRaster
endif

#exe stuff
JS_DEPS=$(JS_LIBS)

