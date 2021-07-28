#   SCE CONFIDENTIAL                                      
#   PlayStation(R)3 Programmer Tool Runtime Library 310.001
#   Copyright (C) 2007 Sony Computer Entertainment Inc.   
#   All Rights Reserved.                                  

CELL_SDK	?= /usr/local/cell
OBJS_DIR	= objs

CXX			= g++
AR			= ar
RANLIB		= ranlib
CXXFLAGS	= -Wall -O2 $(INCDIRS)

LDFLAGS		=
LDLIBS		= $(CELL_SDK)/host-linux/lib/libgcm_tool.a

SRCS		= main.cpp context.cpp
OBJS		= $(patsubst %, $(OBJS_DIR)/%.o, $(basename $(SRCS)))
MD_FILES	= $(patsubst %, $(OBJS_DIR)/%.d, $(basename $(SRCS)))

INCDIRS		= -I$(CELL_SDK)/target/common/include -I$(CELL_SDK)/host-common/include -I$(CELL_SDK)/host-linux/Cg/include

OBJS_DEPENDS	:= $(SRCS)
OBJS_DEPENDS	:= $(patsubst %.cpp, $(OBJS_DIR)/$(basename %).o, $(OBJS_DEPENDS))
CLEAN_OBJS      =   $(strip $(OBJS) $(MD_FILES))

TARGET		= simple


$(TARGET): $(OBJS_DEPENDS)
	$(CXX) $(LDFLAGS) $(OBJS) $(LDLIBS) -o $@

$(OBJS): $(OBJS_DIR)/%.o: %.cpp
	@mkdir -p $(dir $(@))
	$(CXX) -c $(CXXFLAGS) -Wp,-MMD,$(@:.o=.d),-MT,$@ -o $@ $<

clean:
	rm -fr $(CLEAN_OBJS)
	rm -fr $(OBJS_DIR)

-include $(MD_FILES)
