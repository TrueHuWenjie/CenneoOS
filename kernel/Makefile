# Date:2017/6/8 2:21
# Author:HuWenjie<huwenjie@cenneo.com>
# Ghost Bird File System Reader
# Makefile
#
# BSD 2-Clause License
#
# Copyright (c) 2017, Ghost Bird Operating System Project Developers.
# All rights reserved.

sinclude $(CURDIR)/../Makefile.variable

DIRS := $(shell find . -maxdepth 9 -type d)

C_SRCS := $(foreach dir, $(DIRS), $(wildcard $(dir)/*.c))
CPP_SRCS := $(foreach dir, $(DIRS), $(wildcard $(dir)/*.cpp))
ASM_SRCS := $(foreach dir, $(DIRS), $(wildcard $(dir)/*.asm))

OBJS := $(patsubst %.c, %.o, $(C_SRCS))
OBJS += $(patsubst %.cpp, %.o, $(CPP_SRCS))
OBJS += $(patsubst %.asm, %.o, $(ASM_SRCS))
OBJS := $(filter %_start.o, $(OBJS)) $(filter-out %_start.o, $(OBJS))

OBJCOPYFLAGS := binary -R .note -R .comment -S \
--set-section-flags .bss=alloc,load,contents

LDFLAGS  := -m elf_i386
CFLAGS   := -m32 -fno-stack-protector -I $(CURDIR)/includes
CPPFLAGS := -m32 -fno-stack-protector -I $(CURDIR)/includes
ASMFLAGS := -f elf

TARGET = ../image/data/KERNEL.BIN

sinclude $(OBJS:.o=.d)

all: $(TARGET)

$(TARGET): kernel.o
	@echo [OBJCOPY] $@
	@$(OBJCOPY) -O $(OBJCOPYFLAGS) kernel.o $(TARGET)

kernel.o: $(OBJS)
	@echo [LD] $@
	@$(LD) $(LDFLAGS) -o kernel.o -Ttext 0x400000 $(OBJS)

%.o: %.asm
	@echo [CC] $<
	@$(NS) $(ASMFLAGS) -o $@ $<

%.o: %.c
	@echo [CC] $@
	@$(CC) -c $(CFLAGS) $< -o $@

%.o: %.cpp
	@echo [C++] $@
	@$(CXX) -c $(CPPFLAGS) $< -o $@

%.d: %.asm
	@$(NS) -M $(ASMFLAGS) $< > $@; \
	$(SED) -i 's,^:,$*.o $@:,g' $@;

%.d: %.c
	@$(CC) -M $(CFLAGS) $< > $@; \
	$(SED) -i 's,$(notdir $*)\.o[ :]*,$*.o $@ : ,g' $@;

%.d: %.cpp
	@$(CXX) -M $(CPPFLAGS) $< > $@; \
	$(SED) -i 's,$(notdir $*)\.o[ :]*,$*.o $@ : ,g' $@;

clean:
	@echo [RM] Removing files...
	@$(RM) $(TARGET) kernel.o $(OBJS) $(OBJS:.o=.d)
